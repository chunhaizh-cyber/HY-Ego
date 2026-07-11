// 文件规则：事件日志段实现只负责规范编解码和候选文件发布；不得写机器事实或自动修复损坏段。
#include "事件日志段服务.h"

#include "容错检查.h"

#include <algorithm>
#include <array>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace 海中鱼巣 {
namespace {

constexpr std::array<std::uint8_t, 8> 事件段魔数{
    static_cast<std::uint8_t>('H'), static_cast<std::uint8_t>('Y'),
    static_cast<std::uint8_t>('E'), static_cast<std::uint8_t>('V'),
    static_cast<std::uint8_t>('S'), static_cast<std::uint8_t>('E'),
    static_cast<std::uint8_t>('G'), static_cast<std::uint8_t>('1')
};
constexpr std::uint32_t 字节序标记 = 0x01020304U;
constexpr std::size_t 段头字节数 = 64;
constexpr std::size_t 最小帧字节数 = 112;
constexpr std::uint64_t FNV偏移基 = 14695981039346656037ULL;
constexpr std::uint64_t FNV质数 = 1099511628211ULL;

struct 路径解析结果 {
    事件段路径材料 材料;
    bool 配置有效 = false;
};

struct 严格解析结果 {
    事件段读取结果 读取;
    std::vector<std::uint8_t> 原字节;
};

bool 路径文本相同(const std::filesystem::path& 左, const std::filesystem::path& 右) {
    const std::wstring 左文本 = 左.lexically_normal().native();
    const std::wstring 右文本 = 右.lexically_normal().native();
    return CompareStringOrdinal(
        左文本.c_str(), static_cast<int>(左文本.size()),
        右文本.c_str(), static_cast<int>(右文本.size()), TRUE) == CSTR_EQUAL;
}

bool 含点路径段(const std::filesystem::path& 路径) {
    return std::any_of(路径.begin(), 路径.end(), [](const auto& 路径段) {
        return 路径段 == L"." || 路径段 == L"..";
    });
}

路径解析结果 解析路径(const 事件段配置& 配置) {
    路径解析结果 结果;
    if (配置.段编号 == 0
        || 配置.最大段字节数 < 段头字节数
        || 配置.最大段字节数 > 事件日志段服务::最大允许段字节数) {
        结果.材料.拒绝原因 = 事件段拒绝原因::配置无效;
        return 结果;
    }
    if (配置.日志根目录.empty() || !配置.日志根目录.is_absolute() || 含点路径段(配置.日志根目录)) {
        结果.材料.拒绝原因 = 事件段拒绝原因::路径越界;
        return 结果;
    }

    const auto 正式目录 = (日志目录路径() / L"事件段").lexically_normal();
    const auto 自检目录 = (正式目录 / L"自检").lexically_normal();
    const auto 请求目录 = 配置.日志根目录.lexically_normal();
    if (!路径文本相同(请求目录, 正式目录) && !路径文本相同(请求目录, 自检目录)) {
        结果.材料.拒绝原因 = 事件段拒绝原因::路径越界;
        return 结果;
    }

    const std::wstring 文件名 = L"事件段_" + std::to_wstring(配置.段编号) + L".bin";
    结果.材料.目标路径 = 请求目录 / 文件名;
    结果.材料.临时路径 = 请求目录 / (文件名 + L".tmp");
    结果.材料.拒绝原因 = 事件段拒绝原因::无;
    结果.配置有效 = true;
    return 结果;
}

void 追加U32(std::vector<std::uint8_t>& 字节组, std::uint32_t 值) {
    for (std::uint32_t 位移 = 0; 位移 < 32; 位移 += 8) {
        字节组.push_back(static_cast<std::uint8_t>((值 >> 位移) & 0xffU));
    }
}

void 追加U64(std::vector<std::uint8_t>& 字节组, std::uint64_t 值) {
    for (std::uint32_t 位移 = 0; 位移 < 64; 位移 += 8) {
        字节组.push_back(static_cast<std::uint8_t>((值 >> 位移) & 0xffULL));
    }
}

void 覆写U32(std::vector<std::uint8_t>& 字节组, std::size_t 偏移, std::uint32_t 值) {
    for (std::uint32_t 字节序号 = 0; 字节序号 < 4; ++字节序号) {
        字节组[偏移 + 字节序号] = static_cast<std::uint8_t>((值 >> (字节序号 * 8)) & 0xffU);
    }
}

void 覆写U64(std::vector<std::uint8_t>& 字节组, std::size_t 偏移, std::uint64_t 值) {
    for (std::uint32_t 字节序号 = 0; 字节序号 < 8; ++字节序号) {
        字节组[偏移 + 字节序号] = static_cast<std::uint8_t>((值 >> (字节序号 * 8)) & 0xffULL);
    }
}

bool 读取U32(const std::vector<std::uint8_t>& 字节组, std::size_t& 偏移, std::uint32_t& 值) {
    if (偏移 > 字节组.size() || 字节组.size() - 偏移 < 4) {
        return false;
    }
    值 = 0;
    for (std::uint32_t 字节序号 = 0; 字节序号 < 4; ++字节序号) {
        值 |= static_cast<std::uint32_t>(字节组[偏移 + 字节序号]) << (字节序号 * 8);
    }
    偏移 += 4;
    return true;
}

bool 读取U64(const std::vector<std::uint8_t>& 字节组, std::size_t& 偏移, std::uint64_t& 值) {
    if (偏移 > 字节组.size() || 字节组.size() - 偏移 < 8) {
        return false;
    }
    值 = 0;
    for (std::uint32_t 字节序号 = 0; 字节序号 < 8; ++字节序号) {
        值 |= static_cast<std::uint64_t>(字节组[偏移 + 字节序号]) << (字节序号 * 8);
    }
    偏移 += 8;
    return true;
}

std::uint64_t 计算FNV(const std::vector<std::uint8_t>& 字节组, std::size_t 起点, std::size_t 终点) {
    std::uint64_t 哈希 = FNV偏移基;
    for (std::size_t 索引 = 起点; 索引 < 终点; ++索引) {
        哈希 ^= 字节组[索引];
        哈希 *= FNV质数;
    }
    return 哈希;
}

std::optional<std::string> 严格转为UTF8(std::wstring_view 文本) {
    if (文本.empty()) {
        return std::string{};
    }
    if (文本.size() > static_cast<std::size_t>((std::numeric_limits<int>::max)())) {
        return std::nullopt;
    }
    const int 输入长度 = static_cast<int>(文本.size());
    const int 输出长度 = WideCharToMultiByte(
        CP_UTF8, WC_ERR_INVALID_CHARS, 文本.data(), 输入长度, nullptr, 0, nullptr, nullptr);
    if (输出长度 <= 0) {
        return std::nullopt;
    }
    std::string 结果(static_cast<std::size_t>(输出长度), '\0');
    if (WideCharToMultiByte(
            CP_UTF8, WC_ERR_INVALID_CHARS, 文本.data(), 输入长度,
            结果.data(), 输出长度, nullptr, nullptr) != 输出长度) {
        return std::nullopt;
    }
    if (转为UTF8(文本) != 结果) {
        return std::nullopt;
    }
    return 结果;
}

std::optional<std::wstring> 严格转为宽文本(const std::uint8_t* 字节, std::size_t 字节数) {
    if (字节数 == 0) {
        return std::wstring{};
    }
    if (字节数 > static_cast<std::size_t>((std::numeric_limits<int>::max)())) {
        return std::nullopt;
    }
    const int 输入长度 = static_cast<int>(字节数);
    const int 输出长度 = MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS, reinterpret_cast<const char*>(字节), 输入长度, nullptr, 0);
    if (输出长度 <= 0) {
        return std::nullopt;
    }
    std::wstring 结果(static_cast<std::size_t>(输出长度), L'\0');
    if (MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS, reinterpret_cast<const char*>(字节), 输入长度,
            结果.data(), 输出长度) != 输出长度) {
        return std::nullopt;
    }
    return 结果;
}

bool 读取文件字节(
    const std::filesystem::path& 路径,
    std::uint64_t 最大字节数,
    std::vector<std::uint8_t>& 字节组) {
    HANDLE 文件 = CreateFileW(
        路径.c_str(), GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (文件 == INVALID_HANDLE_VALUE) {
        return false;
    }

    LARGE_INTEGER 文件大小{};
    if (!GetFileSizeEx(文件, &文件大小)
        || 文件大小.QuadPart < 0
        || static_cast<std::uint64_t>(文件大小.QuadPart) > 最大字节数) {
        CloseHandle(文件);
        return false;
    }

    字节组.assign(static_cast<std::size_t>(文件大小.QuadPart), 0);
    std::size_t 已读取 = 0;
    while (已读取 < 字节组.size()) {
        const DWORD 本轮大小 = static_cast<DWORD>((std::min)(
            字节组.size() - 已读取,
            static_cast<std::size_t>((std::numeric_limits<DWORD>::max)())));
        DWORD 本轮读取 = 0;
        if (!ReadFile(文件, 字节组.data() + 已读取, 本轮大小, &本轮读取, nullptr)
            || 本轮读取 != 本轮大小) {
            CloseHandle(文件);
            字节组.clear();
            return false;
        }
        已读取 += 本轮读取;
    }
    CloseHandle(文件);
    return true;
}

bool 写入临时文件(const std::filesystem::path& 路径, const std::vector<std::uint8_t>& 字节组) {
    HANDLE 文件 = CreateFileW(
        路径.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, nullptr);
    if (文件 == INVALID_HANDLE_VALUE) {
        return false;
    }

    std::size_t 已写入 = 0;
    bool 成功 = true;
    while (已写入 < 字节组.size()) {
        const DWORD 本轮大小 = static_cast<DWORD>((std::min)(
            字节组.size() - 已写入,
            static_cast<std::size_t>((std::numeric_limits<DWORD>::max)())));
        DWORD 本轮写入 = 0;
        if (!WriteFile(文件, 字节组.data() + 已写入, 本轮大小, &本轮写入, nullptr)
            || 本轮写入 != 本轮大小) {
            成功 = false;
            break;
        }
        已写入 += 本轮写入;
    }
    if (成功) {
        成功 = FlushFileBuffers(文件) != FALSE;
    }
    if (CloseHandle(文件) == FALSE) {
        成功 = false;
    }
    return 成功;
}

事件段读取结果 解析事件段字节(
    const std::vector<std::uint8_t>& 字节组,
    const 事件段配置& 配置) {
    auto 拒绝 = [&](事件段拒绝原因 原因) {
        return 事件段读取结果{
            事件段状态::既有段不可读, 原因, 配置.段编号,
            0, 0, 0, static_cast<std::uint64_t>(字节组.size()), {}};
    };
    if (字节组.size() < 段头字节数 || 字节组.size() > 配置.最大段字节数) {
        return 拒绝(事件段拒绝原因::段长度不匹配);
    }
    if (!std::equal(事件段魔数.begin(), 事件段魔数.end(), 字节组.begin())) {
        return 拒绝(事件段拒绝原因::魔数不匹配);
    }

    std::size_t 偏移 = 8;
    std::uint32_t 版本 = 0;
    std::uint32_t 字节序 = 0;
    std::uint64_t 段编号 = 0;
    std::uint64_t 记录数 = 0;
    std::uint64_t 首序号 = 0;
    std::uint64_t 末序号 = 0;
    std::uint64_t 声明字节数 = 0;
    std::uint64_t 声明段头校验 = 0;
    if (!读取U32(字节组, 偏移, 版本)
        || !读取U32(字节组, 偏移, 字节序)
        || !读取U64(字节组, 偏移, 段编号)
        || !读取U64(字节组, 偏移, 记录数)
        || !读取U64(字节组, 偏移, 首序号)
        || !读取U64(字节组, 偏移, 末序号)
        || !读取U64(字节组, 偏移, 声明字节数)
        || !读取U64(字节组, 偏移, 声明段头校验)) {
        return 拒绝(事件段拒绝原因::段长度不匹配);
    }
    if (版本 != 事件日志段服务::格式版本) {
        return 拒绝(事件段拒绝原因::未知格式版本);
    }
    if (字节序 != 字节序标记) {
        return 拒绝(事件段拒绝原因::字节序不匹配);
    }
    if (段编号 != 配置.段编号) {
        return 拒绝(事件段拒绝原因::段编号不匹配);
    }
    if (声明字节数 != 字节组.size()) {
        return 拒绝(事件段拒绝原因::段长度不匹配);
    }
    if (声明段头校验 != 计算FNV(字节组, 0, 段头字节数 - 8)) {
        return 拒绝(事件段拒绝原因::段头校验不一致);
    }
    if ((记录数 == 0 && (首序号 != 0 || 末序号 != 0))
        || (记录数 != 0 && (首序号 != 1 || 末序号 != 记录数))
        || 记录数 > (字节组.size() - 段头字节数) / 最小帧字节数) {
        return 拒绝(事件段拒绝原因::序号不连续);
    }

    std::vector<事件材料> 事件组;
    事件组.reserve(static_cast<std::size_t>(记录数));
    for (std::uint64_t 记录索引 = 0; 记录索引 < 记录数; ++记录索引) {
        const std::size_t 帧起点 = 偏移;
        std::uint32_t 帧总长度 = 0;
        if (!读取U32(字节组, 偏移, 帧总长度)
            || 帧总长度 < 最小帧字节数
            || 帧总长度 > 事件日志段服务::最大帧字节数
            || 帧起点 > 字节组.size()
            || 字节组.size() - 帧起点 < 帧总长度) {
            return 拒绝(事件段拒绝原因::帧长度无效);
        }
        const std::size_t 帧终点 = 帧起点 + 帧总长度;
        std::uint32_t 负载版本 = 0;
        事件材料 材料;
        std::uint32_t 类别值 = 0;
        std::uint32_t 结果值 = 0;
        std::uint32_t 入口字节数 = 0;
        std::uint32_t 摘要字节数 = 0;
        if (!读取U32(字节组, 偏移, 负载版本)
            || !读取U64(字节组, 偏移, 材料.结构事件顺序号)
            || !读取U64(字节组, 偏移, 材料.结构事件时间戳)
            || !读取U32(字节组, 偏移, 类别值)
            || !读取U32(字节组, 偏移, 结果值)
            || !读取U64(字节组, 偏移, 材料.仓库编号)
            || !读取U64(字节组, 偏移, 材料.节点编号)
            || !读取U64(字节组, 偏移, 材料.节点版本)
            || !读取U64(字节组, 偏移, 材料.主信息编号)
            || !读取U64(字节组, 偏移, 材料.主信息版本)
            || !读取U64(字节组, 偏移, 材料.关系编号)
            || !读取U64(字节组, 偏移, 材料.关系版本)
            || !读取U64(字节组, 偏移, 材料.索引键)
            || !读取U32(字节组, 偏移, 入口字节数)
            || !读取U32(字节组, 偏移, 摘要字节数)) {
            return 拒绝(事件段拒绝原因::帧长度无效);
        }
        if (负载版本 != 1) {
            return 拒绝(事件段拒绝原因::负载版本不匹配);
        }
        if (材料.结构事件顺序号 != 记录索引 + 1 || 材料.结构事件时间戳 == 0) {
            return 拒绝(事件段拒绝原因::序号不连续);
        }
        if (入口字节数 == 0
            || 入口字节数 > 事件日志段服务::最大入口名称字节数
            || 摘要字节数 > 事件日志段服务::最大摘要字节数
            || 偏移 > 帧终点
            || 帧终点 - 偏移 != static_cast<std::size_t>(入口字节数) + 摘要字节数 + 8) {
            return 拒绝(事件段拒绝原因::帧长度无效);
        }
        const auto 入口 = 严格转为宽文本(字节组.data() + 偏移, 入口字节数);
        偏移 += 入口字节数;
        const auto 摘要 = 严格转为宽文本(字节组.data() + 偏移, 摘要字节数);
        偏移 += 摘要字节数;
        std::uint64_t 声明帧校验 = 0;
        if (!入口.has_value() || !摘要.has_value() || !读取U64(字节组, 偏移, 声明帧校验)) {
            return 拒绝(事件段拒绝原因::UTF8无效);
        }
        if (偏移 != 帧终点
            || 声明帧校验 != 计算FNV(字节组, 帧起点, 帧终点 - 8)) {
            return 拒绝(事件段拒绝原因::帧校验不一致);
        }
        材料.入口名称 = 入口.value();
        材料.摘要 = 摘要.value();
        材料.类别 = static_cast<事件类别>(类别值);
        材料.写入结果 = static_cast<事件写入结果>(结果值);
        材料.请求裁决运行期事实 = false;
        材料.请求恢复运行期结构 = false;
        if (!事件材料是否可写入(材料)) {
            return 拒绝(事件段拒绝原因::帧材料无效);
        }
        事件组.push_back(std::move(材料));
    }
    if (偏移 != 字节组.size()) {
        return 拒绝(事件段拒绝原因::段长度不匹配);
    }
    return {
        事件段状态::可读取,
        事件段拒绝原因::无,
        配置.段编号,
        事件日志段服务::格式版本,
        首序号,
        末序号,
        static_cast<std::uint64_t>(字节组.size()),
        std::move(事件组)
    };
}

std::optional<std::vector<std::uint8_t>> 编码帧(const 事件材料& 材料) {
    const auto 入口 = 严格转为UTF8(材料.入口名称);
    const auto 摘要 = 严格转为UTF8(材料.摘要);
    if (!入口.has_value() || !摘要.has_value()
        || 入口->empty()
        || 入口->size() > 事件日志段服务::最大入口名称字节数
        || 摘要->size() > 事件日志段服务::最大摘要字节数) {
        return std::nullopt;
    }

    std::vector<std::uint8_t> 帧;
    帧.reserve(最小帧字节数 + 入口->size() + 摘要->size());
    追加U32(帧, 0);
    追加U32(帧, 1);
    追加U64(帧, 材料.结构事件顺序号);
    追加U64(帧, 材料.结构事件时间戳);
    追加U32(帧, static_cast<std::uint32_t>(材料.类别));
    追加U32(帧, static_cast<std::uint32_t>(材料.写入结果));
    追加U64(帧, 材料.仓库编号);
    追加U64(帧, 材料.节点编号);
    追加U64(帧, 材料.节点版本);
    追加U64(帧, 材料.主信息编号);
    追加U64(帧, 材料.主信息版本);
    追加U64(帧, 材料.关系编号);
    追加U64(帧, 材料.关系版本);
    追加U64(帧, 材料.索引键);
    追加U32(帧, static_cast<std::uint32_t>(入口->size()));
    追加U32(帧, static_cast<std::uint32_t>(摘要->size()));
    帧.insert(帧.end(), 入口->begin(), 入口->end());
    帧.insert(帧.end(), 摘要->begin(), 摘要->end());
    const std::uint64_t 总长度 = 帧.size() + 8;
    if (总长度 > 事件日志段服务::最大帧字节数) {
        return std::nullopt;
    }
    覆写U32(帧, 0, static_cast<std::uint32_t>(总长度));
    追加U64(帧, 计算FNV(帧, 0, 帧.size()));
    return 帧;
}

std::optional<std::vector<std::uint8_t>> 编码事件段(
    std::uint64_t 段编号,
    const std::vector<事件材料>& 事件组) {
    std::vector<std::uint8_t> 字节组;
    字节组.reserve(段头字节数 + 事件组.size() * 最小帧字节数);
    字节组.insert(字节组.end(), 事件段魔数.begin(), 事件段魔数.end());
    追加U32(字节组, 事件日志段服务::格式版本);
    追加U32(字节组, 字节序标记);
    追加U64(字节组, 段编号);
    追加U64(字节组, static_cast<std::uint64_t>(事件组.size()));
    追加U64(字节组, 事件组.empty() ? 0 : 事件组.front().结构事件顺序号);
    追加U64(字节组, 事件组.empty() ? 0 : 事件组.back().结构事件顺序号);
    追加U64(字节组, 0);
    追加U64(字节组, 0);
    for (const auto& 事件 : 事件组) {
        const auto 帧 = 编码帧(事件);
        if (!帧.has_value()) {
            return std::nullopt;
        }
        字节组.insert(字节组.end(), 帧->begin(), 帧->end());
    }
    覆写U64(字节组, 48, static_cast<std::uint64_t>(字节组.size()));
    覆写U64(字节组, 56, 计算FNV(字节组, 0, 56));
    return 字节组;
}

严格解析结果 读取并解析(const std::filesystem::path& 路径, const 事件段配置& 配置) {
    严格解析结果 结果;
    if (!读取文件字节(路径, 配置.最大段字节数, 结果.原字节)) {
        结果.读取 = {
            事件段状态::既有段不可读,
            事件段拒绝原因::文件读取异常,
            配置.段编号,
            0, 0, 0, 0, {}};
        return 结果;
    }
    结果.读取 = 解析事件段字节(结果.原字节, 配置);
    return 结果;
}

事件段追加结果 追加拒绝(
    const 事件段配置& 配置,
    事件段状态 状态,
    事件段拒绝原因 原因) {
    return {状态, 原因, 配置.段编号, 0, 0, 0, false, false};
}

}

事件日志段服务::事件日志段服务(事件段配置 配置)
    : 配置_(std::move(配置)) {
}

事件段路径材料 事件日志段服务::读取段路径材料() const {
    std::lock_guard 锁(服务锁_);
    return 解析路径(配置_).材料;
}

事件段读取结果 事件日志段服务::读取事件段() const {
    std::lock_guard 锁(服务锁_);
    return 读取事件段_已加锁();
}

事件段读取结果 事件日志段服务::读取事件段_已加锁() const {
    const auto 路径 = 解析路径(配置_);
    if (!路径.配置有效) {
        return {
            事件段状态::已拒绝, 路径.材料.拒绝原因, 配置_.段编号,
            0, 0, 0, 0, {}};
    }
    std::error_code 错误码;
    const bool 目标存在 = std::filesystem::exists(路径.材料.目标路径, 错误码);
    if (错误码) {
        return {
            事件段状态::既有段不可读, 事件段拒绝原因::文件读取异常,
            配置_.段编号, 0, 0, 0, 0, {}};
    }
    if (!目标存在) {
        return {
            事件段状态::空段候选, 事件段拒绝原因::无,
            配置_.段编号, 格式版本, 0, 0, 0, {}};
    }
    return 读取并解析(路径.材料.目标路径, 配置_).读取;
}

事件段追加结果 事件日志段服务::追加结构事件(const 事件材料& 材料) {
    事件段追加结果 结果;
    {
        std::lock_guard 锁(服务锁_);
        结果 = 追加结构事件_已加锁(材料);
    }
    if (结果.状态 == 事件段状态::内部写入异常) {
        (void)追根因检查(false, L"事件段前置通过后的写入、发布或读回未达到内部预期。");
    }
    return 结果;
}

事件段追加结果 事件日志段服务::追加结构事件_已加锁(const 事件材料& 材料) {
    const auto 路径 = 解析路径(配置_);
    if (!路径.配置有效) {
        return 追加拒绝(配置_, 事件段状态::已拒绝, 路径.材料.拒绝原因);
    }
    if (!事件材料是否可写入(材料)) {
        return 追加拒绝(配置_, 事件段状态::已拒绝, 事件段拒绝原因::材料无效);
    }
    if (材料.结构事件时间戳 == 0) {
        return 追加拒绝(配置_, 事件段状态::已拒绝, 事件段拒绝原因::时间戳无效);
    }
    if (材料.结构事件顺序号 != 0) {
        return 追加拒绝(配置_, 事件段状态::已拒绝, 事件段拒绝原因::调用方预填序号);
    }
    const auto 入口 = 严格转为UTF8(材料.入口名称);
    const auto 摘要 = 严格转为UTF8(材料.摘要);
    if (!入口.has_value() || !摘要.has_value()) {
        return 追加拒绝(配置_, 事件段状态::已拒绝, 事件段拒绝原因::UTF8无效);
    }
    if (入口->empty()
        || 入口->size() > 最大入口名称字节数
        || 摘要->size() > 最大摘要字节数) {
        return 追加拒绝(配置_, 事件段状态::已拒绝, 事件段拒绝原因::字段超限);
    }

    std::error_code 错误码;
    const bool 临时存在 = std::filesystem::exists(路径.材料.临时路径, 错误码);
    if (错误码) {
        return 追加拒绝(配置_, 事件段状态::已拒绝, 事件段拒绝原因::文件读取异常);
    }
    if (临时存在) {
        return 追加拒绝(配置_, 事件段状态::临时段冲突, 事件段拒绝原因::临时段冲突);
    }

    const bool 目标存在 = std::filesystem::exists(路径.材料.目标路径, 错误码);
    if (错误码) {
        return 追加拒绝(配置_, 事件段状态::既有段不可读, 事件段拒绝原因::文件读取异常);
    }
    std::vector<std::uint8_t> 原目标字节;
    std::vector<事件材料> 事件组;
    if (目标存在) {
        auto 既有段 = 读取并解析(路径.材料.目标路径, 配置_);
        if (!既有段.读取.成功()) {
            return 追加拒绝(配置_, 事件段状态::既有段不可读, 既有段.读取.拒绝原因);
        }
        原目标字节 = std::move(既有段.原字节);
        事件组 = std::move(既有段.读取.事件组);
    }
    const std::uint64_t 末序号 = 事件组.empty() ? 0 : 事件组.back().结构事件顺序号;
    if (末序号 == (std::numeric_limits<std::uint64_t>::max)()) {
        return 追加拒绝(配置_, 事件段状态::段已满, 事件段拒绝原因::段已满);
    }

    事件材料 已分配材料 = 材料;
    已分配材料.结构事件顺序号 = 末序号 + 1;
    const auto 新帧 = 编码帧(已分配材料);
    if (!新帧.has_value()) {
        return 追加拒绝(配置_, 事件段状态::段已满, 事件段拒绝原因::字段超限);
    }
    事件组.push_back(已分配材料);
    const auto 候选字节 = 编码事件段(配置_.段编号, 事件组);
    if (!候选字节.has_value()) {
        return 追加拒绝(配置_, 事件段状态::内部写入异常, 事件段拒绝原因::内部不一致);
    }
    if (候选字节->size() > 配置_.最大段字节数) {
        return 追加拒绝(配置_, 事件段状态::段已满, 事件段拒绝原因::段已满);
    }

    std::filesystem::create_directories(配置_.日志根目录, 错误码);
    if (错误码) {
        return 追加拒绝(配置_, 事件段状态::内部写入异常, 事件段拒绝原因::目录创建异常);
    }
    if (!写入临时文件(路径.材料.临时路径, 候选字节.value())) {
        auto 结果 = 追加拒绝(
            配置_, 事件段状态::内部写入异常, 事件段拒绝原因::临时段写入异常);
        结果.临时段保留 = std::filesystem::exists(路径.材料.临时路径, 错误码);
        return 结果;
    }

    const auto 临时读回 = 读取并解析(路径.材料.临时路径, 配置_);
    if (!临时读回.读取.成功()
        || 临时读回.原字节 != 候选字节.value()
        || 临时读回.读取.事件组.size() != 事件组.size()
        || 临时读回.读取.末序号 != 已分配材料.结构事件顺序号) {
        auto 结果 = 追加拒绝(
            配置_, 事件段状态::内部写入异常, 事件段拒绝原因::临时段读回异常);
        结果.临时段保留 = true;
        return 结果;
    }

    if (MoveFileExW(
            路径.材料.临时路径.c_str(), 路径.材料.目标路径.c_str(),
            MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) == FALSE) {
        auto 结果 = 追加拒绝(
            配置_, 事件段状态::内部写入异常, 事件段拒绝原因::发布替换异常);
        std::vector<std::uint8_t> 当前目标字节;
        if (目标存在) {
            结果.旧目标保持可读 = 读取文件字节(
                路径.材料.目标路径, 配置_.最大段字节数, 当前目标字节)
                && 当前目标字节 == 原目标字节
                && 解析事件段字节(当前目标字节, 配置_).成功();
        } else {
            结果.旧目标保持可读 = !std::filesystem::exists(路径.材料.目标路径, 错误码);
        }
        结果.临时段保留 = std::filesystem::exists(路径.材料.临时路径, 错误码);
        return 结果;
    }

    const auto 最终读回 = 读取并解析(路径.材料.目标路径, 配置_);
    if (!最终读回.读取.成功()
        || 最终读回.原字节 != 候选字节.value()
        || 最终读回.读取.事件组.size() != 事件组.size()
        || 最终读回.读取.末序号 != 已分配材料.结构事件顺序号) {
        auto 结果 = 追加拒绝(
            配置_, 事件段状态::内部写入异常, 事件段拒绝原因::发布后读回异常);
        结果.段记录数 = static_cast<std::uint64_t>(最终读回.读取.事件组.size());
        结果.段字节数 = 最终读回.读取.段字节数;
        return 结果;
    }

    return {
        事件段状态::已追加,
        事件段拒绝原因::无,
        配置_.段编号,
        已分配材料.结构事件顺序号,
        static_cast<std::uint64_t>(事件组.size()),
        static_cast<std::uint64_t>(候选字节->size()),
        true,
        false
    };
}

}
