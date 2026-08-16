module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <array>
#include <condition_variable>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#define IMMUTABLE_MATERIAL_NO_INCLUDES

export module 海中鱼巣.领域.服务.不可变材料;

export import 海中鱼巣.领域.合同.L2结构公共;
import 海中鱼巣.核心.服务.L1事实基座;

export {
#include "不可变材料.数据.h"
}

namespace 海中鱼巣::不可变材料内部 {

inline constexpr L1所有者范围写入幂等身份 材料登记幂等身份{
    0x4C32'4D41'5452'4701ULL};
inline constexpr std::uint8_t 材料私有幂等域 = 0x30;

inline constexpr L1所有者范围写集本地键 系统签发者本地键{0x3000'1001U};
inline constexpr L1所有者范围写集本地键 严格UTF8格式本地键{0x3000'1002U};
inline constexpr L1所有者范围写集本地键 通用字节格式本地键{0x3000'1003U};
inline constexpr L1所有者范围写集本地键 格式属性类型本地键{0x3000'1101U};
inline constexpr L1所有者范围写集本地键 长度属性类型本地键{0x3000'1102U};
inline constexpr L1所有者范围写集本地键 摘要属性类型本地键{0x3000'1103U};

inline constexpr L1所有者范围写集本地键 材料节点本地键{0x3000'2001U};
inline constexpr L1所有者范围写集本地键 格式值本地键{0x3000'2101U};
inline constexpr L1所有者范围写集本地键 长度值本地键{0x3000'2102U};
inline constexpr L1所有者范围写集本地键 摘要值本地键{0x3000'2103U};

class 唯一句柄 final {
public:
    唯一句柄() noexcept = default;
    explicit 唯一句柄(HANDLE 值) noexcept : 值_(值) {}
    唯一句柄(const 唯一句柄&) = delete;
    唯一句柄& operator=(const 唯一句柄&) = delete;
    唯一句柄(唯一句柄&& 来源) noexcept : 值_(来源.释放()) {}
    唯一句柄& operator=(唯一句柄&& 来源) noexcept {
        if (this != &来源) 重置(来源.释放());
        return *this;
    }
    ~唯一句柄() { 重置(); }
    HANDLE 取得() const noexcept { return 值_; }
    explicit operator bool() const noexcept {
        return 值_ != nullptr && 值_ != INVALID_HANDLE_VALUE;
    }
    HANDLE 释放() noexcept {
        const auto 结果 = 值_;
        值_ = INVALID_HANDLE_VALUE;
        return 结果;
    }
    void 重置(HANDLE 新值 = INVALID_HANDLE_VALUE) noexcept {
        if (*this) CloseHandle(值_);
        值_ = 新值;
    }
private:
    HANDLE 值_ = INVALID_HANDLE_VALUE;
};

struct 路径句柄事实 final {
    唯一句柄 句柄;
    std::filesystem::path 规范路径;
    DWORD 卷序列 = 0;
    BY_HANDLE_FILE_INFORMATION 信息{};
};

enum class 文件裁决 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 资源失败 = 3, 内部不一致 = 4
};

struct 文件校验结果 final {
    文件裁决 裁决 = 文件裁决::内部不一致;
    std::uint64_t 长度 = 0;
    材料SHA256摘要 摘要;
};

// 诊断责任：无适用错误分支；纯值枚举映射。
L2结构状态 映射文件裁决(文件裁决 裁决) noexcept {
    switch (裁决) {
    case 文件裁决::入口拒绝: return L2结构状态::入口拒绝;
    case 文件裁决::资源失败: return L2结构状态::资源失败;
    case 文件裁决::成功:
    case 文件裁决::内部不一致: return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；路径纯值校验不读取文件系统。
bool 路径字面量有效(const std::filesystem::path& 路径) noexcept {
    if (路径.empty() || !路径.is_absolute()) return false;
    for (const auto& 分量 : 路径)
        if (分量 == L"." || 分量 == L"..") return false;
    return true;
}

// 诊断责任：向上送出；Windows 路径查询失败由调用边界映射。
std::optional<std::filesystem::path> 读取句柄规范路径(HANDLE 句柄) {
    const DWORD 标志 = FILE_NAME_NORMALIZED | VOLUME_NAME_DOS;
    const DWORD 长度 = GetFinalPathNameByHandleW(句柄, nullptr, 0, 标志);
    if (长度 == 0) return std::nullopt;
    std::wstring 缓冲(长度, L'\0');
    const DWORD 写入 = GetFinalPathNameByHandleW(
        句柄, 缓冲.data(), static_cast<DWORD>(缓冲.size()), 标志);
    if (写入 == 0 || 写入 >= 缓冲.size()) return std::nullopt;
    缓冲.resize(写入);
    return std::filesystem::path(std::move(缓冲));
}

// 诊断责任：向上送出；句柄打开和元数据失败由构造或公开入口映射。
std::optional<路径句柄事实> 打开路径事实(
    const std::filesystem::path& 路径, bool 要求目录, DWORD 访问 = GENERIC_READ,
    DWORD 共享 = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE) {
    const DWORD 标志 = FILE_FLAG_OPEN_REPARSE_POINT
        | (要求目录 ? FILE_FLAG_BACKUP_SEMANTICS : FILE_ATTRIBUTE_NORMAL);
    唯一句柄 句柄(CreateFileW(路径.c_str(), 访问, 共享, nullptr,
        OPEN_EXISTING, 标志, nullptr));
    if (!句柄) return std::nullopt;
    BY_HANDLE_FILE_INFORMATION 信息{};
    if (!GetFileInformationByHandle(句柄.取得(), &信息)) return std::nullopt;
    if ((信息.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) return std::nullopt;
    const bool 是目录 = (信息.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    if (是目录 != 要求目录) return std::nullopt;
    auto 规范路径 = 读取句柄规范路径(句柄.取得());
    if (!规范路径) return std::nullopt;
    return 路径句柄事实{std::move(句柄), std::move(*规范路径),
        信息.dwVolumeSerialNumber, 信息};
}

// 诊断责任：无适用错误分支；已规范化 Windows 路径只作句柄证据后的边界比较。
bool 规范路径位于目录内(const std::filesystem::path& 子路径,
    const std::filesystem::path& 目录) noexcept {
    std::wstring 子 = 子路径.native();
    std::wstring 根 = 目录.native();
    while (!根.empty() && (根.back() == L'\\' || 根.back() == L'/')) 根.pop_back();
    if (子.size() <= 根.size()) return false;
    if (CompareStringOrdinal(子.data(), static_cast<int>(根.size()),
            根.data(), static_cast<int>(根.size()), TRUE) != CSTR_EQUAL)
        return false;
    return 子[根.size()] == L'\\' || 子[根.size()] == L'/';
}

// 诊断责任：向上送出；目录 I/O 失败由构造或写入边界映射。
std::optional<路径句柄事实> 确保普通子目录(
    const 路径句柄事实& 根, const std::filesystem::path& 路径) {
    if (!CreateDirectoryW(路径.c_str(), nullptr)) {
        const DWORD 错误 = GetLastError();
        if (错误 != ERROR_ALREADY_EXISTS) return std::nullopt;
    }
    auto 结果 = 打开路径事实(路径, true);
    if (!结果 || 结果->卷序列 != 根.卷序列
        || !规范路径位于目录内(结果->规范路径, 根.规范路径)) return std::nullopt;
    return 结果;
}

struct 严格UTF8校验器 final {
    std::uint32_t 码点 = 0;
    std::uint8_t 剩余 = 0;
    std::uint8_t 首续最小 = 0x80;
    std::uint8_t 首续最大 = 0xBF;
    std::uint64_t 总数 = 0;
    std::array<std::uint8_t, 3> 开头{};
    bool 有效 = true;

    void 输入(std::uint8_t 字节) noexcept {
        if (!有效) return;
        if (总数 < 开头.size()) 开头[static_cast<std::size_t>(总数)] = 字节;
        ++总数;
        if (剩余 == 0) {
            if (字节 <= 0x7F) { 码点 = 字节; return; }
            首续最小 = 0x80; 首续最大 = 0xBF;
            if (字节 >= 0xC2 && 字节 <= 0xDF) {
                码点 = 字节 & 0x1F; 剩余 = 1; return;
            }
            if (字节 >= 0xE0 && 字节 <= 0xEF) {
                码点 = 字节 & 0x0F; 剩余 = 2;
                if (字节 == 0xE0) 首续最小 = 0xA0;
                if (字节 == 0xED) 首续最大 = 0x9F;
                return;
            }
            if (字节 >= 0xF0 && 字节 <= 0xF4) {
                码点 = 字节 & 0x07; 剩余 = 3;
                if (字节 == 0xF0) 首续最小 = 0x90;
                if (字节 == 0xF4) 首续最大 = 0x8F;
                return;
            }
            有效 = false; return;
        }
        if (字节 < 首续最小 || 字节 > 首续最大) { 有效 = false; return; }
        首续最小 = 0x80; 首续最大 = 0xBF;
        码点 = (码点 << 6) | (字节 & 0x3F);
        --剩余;
        if (剩余 == 0 && 码点 == 0xFFFD) 有效 = false;
    }

    bool 完成() const noexcept {
        return 有效 && 总数 != 0 && 剩余 == 0
            && !(总数 >= 3 && 开头[0] == 0xEF
                && 开头[1] == 0xBB && 开头[2] == 0xBF);
    }
};

class SHA256计算器 final {
public:
    SHA256计算器() {
        if (BCryptOpenAlgorithmProvider(&算法_, BCRYPT_SHA256_ALGORITHM,
                nullptr, 0) < 0) return;
        DWORD 长度 = 0, 已写 = 0;
        if (BCryptGetProperty(算法_, BCRYPT_OBJECT_LENGTH,
                reinterpret_cast<PUCHAR>(&长度), sizeof(长度), &已写, 0) < 0
            || 已写 != sizeof(长度) || 长度 == 0) return;
        对象_.resize(长度);
        if (BCryptCreateHash(算法_, &散列_, 对象_.data(),
                static_cast<ULONG>(对象_.size()), nullptr, 0, 0) < 0) return;
        有效_ = true;
    }
    SHA256计算器(const SHA256计算器&) = delete;
    SHA256计算器& operator=(const SHA256计算器&) = delete;
    ~SHA256计算器() {
        if (散列_) BCryptDestroyHash(散列_);
        if (算法_) BCryptCloseAlgorithmProvider(算法_, 0);
    }
    bool 输入(const std::uint8_t* 数据, std::size_t 长度) noexcept {
        return 有效_ && 长度 <= std::numeric_limits<ULONG>::max()
            && BCryptHashData(散列_, const_cast<PUCHAR>(数据),
                static_cast<ULONG>(长度), 0) >= 0;
    }
    std::optional<材料SHA256摘要> 完成() noexcept {
        if (!有效_) return std::nullopt;
        材料SHA256摘要 结果;
        if (BCryptFinishHash(散列_, 结果.字节.data(),
                static_cast<ULONG>(结果.字节.size()), 0) < 0) return std::nullopt;
        有效_ = false;
        return 结果;
    }
private:
    BCRYPT_ALG_HANDLE 算法_ = nullptr;
    BCRYPT_HASH_HANDLE 散列_ = nullptr;
    std::vector<std::uint8_t> 对象_;
    bool 有效_ = false;
};

// 诊断责任：向上送出；句柄读失败保留具名文件裁决。
文件校验结果 校验文件句柄(HANDLE 句柄, bool 校验UTF8) {
    if (SetFilePointer(句柄, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
        return {文件裁决::资源失败};
    SHA256计算器 计算器;
    严格UTF8校验器 UTF8;
    std::array<std::uint8_t, 64 * 1024> 缓冲{};
    std::uint64_t 总长 = 0;
    for (;;) {
        DWORD 已读 = 0;
        if (!ReadFile(句柄, 缓冲.data(), static_cast<DWORD>(缓冲.size()),
                &已读, nullptr)) return {文件裁决::资源失败};
        if (已读 == 0) break;
        if (总长 > std::numeric_limits<std::uint64_t>::max() - 已读)
            return {文件裁决::资源失败};
        总长 += 已读;
        if (!计算器.输入(缓冲.data(), 已读)) return {文件裁决::资源失败};
        if (校验UTF8)
            for (DWORD 序号 = 0; 序号 < 已读; ++序号) UTF8.输入(缓冲[序号]);
    }
    auto 摘要 = 计算器.完成();
    if (!摘要) return {文件裁决::资源失败};
    if (校验UTF8 && !UTF8.完成()) return {文件裁决::入口拒绝};
    return {文件裁决::成功, 总长, *摘要};
}

// 诊断责任：向上送出；双句柄完整比较不根据摘要猜相等。
文件裁决 逐字节比较(HANDLE 左, HANDLE 右) {
    LARGE_INTEGER 零{};
    if (!SetFilePointerEx(左, 零, nullptr, FILE_BEGIN)
        || !SetFilePointerEx(右, 零, nullptr, FILE_BEGIN)) return 文件裁决::资源失败;
    std::array<std::uint8_t, 64 * 1024> 左缓冲{}, 右缓冲{};
    for (;;) {
        DWORD 左长 = 0, 右长 = 0;
        if (!ReadFile(左, 左缓冲.data(), static_cast<DWORD>(左缓冲.size()),
                &左长, nullptr)
            || !ReadFile(右, 右缓冲.data(), static_cast<DWORD>(右缓冲.size()),
                &右长, nullptr)) return 文件裁决::资源失败;
        if (左长 != 右长) return 文件裁决::内部不一致;
        if (左长 == 0) return 文件裁决::成功;
        if (!std::equal(左缓冲.begin(), 左缓冲.begin() + 左长,
                右缓冲.begin())) return 文件裁决::内部不一致;
    }
}

// 诊断责任：无适用错误分支；摘要编码只形成确定物理定位。
std::wstring 摘要十六进制(const 材料SHA256摘要& 摘要) {
    static constexpr wchar_t 数字[] = L"0123456789abcdef";
    std::wstring 结果;
    结果.resize(64);
    for (std::size_t 序号 = 0; 序号 < 摘要.字节.size(); ++序号) {
        结果[序号 * 2] = 数字[摘要.字节[序号] >> 4];
        结果[序号 * 2 + 1] = 数字[摘要.字节[序号] & 0x0F];
    }
    return 结果;
}

// 诊断责任：无适用错误分支；网络序摘要分组是纯值转换。
std::vector<std::uint64_t> 摘要U64组(const 材料SHA256摘要& 摘要) {
    std::vector<std::uint64_t> 结果(4, 0);
    for (std::size_t 组 = 0; 组 < 4; ++组)
        for (std::size_t 字节 = 0; 字节 < 8; ++字节)
            结果[组] = (结果[组] << 8) | 摘要.字节[组 * 8 + 字节];
    return 结果;
}

// 诊断责任：无适用错误分支；确定值材料反向解码。
std::optional<材料SHA256摘要> 从U64组读取摘要(
    const L1所有者范围原始值材料& 材料) noexcept {
    const auto* 组 = std::get_if<std::vector<std::uint64_t>>(&材料);
    if (!组 || 组->size() != 4) return std::nullopt;
    材料SHA256摘要 结果;
    for (std::size_t 索引 = 0; 索引 < 4; ++索引)
        for (std::size_t 字节 = 0; 字节 < 8; ++字节)
            结果.字节[索引 * 8 + 字节] = static_cast<std::uint8_t>(
                (*组)[索引] >> ((7 - 字节) * 8));
    return 结果;
}

// 诊断责任：无适用错误分支；L1 状态完整覆盖映射。
L2结构状态 映射写入状态(L1所有者范围写入状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围写入状态::成功: return L2结构状态::已提交;
    case L1所有者范围写入状态::精确重复: return L2结构状态::精确重复;
    case L1所有者范围写入状态::入口拒绝: return L2结构状态::入口拒绝;
    case L1所有者范围写入状态::许可拒绝: return L2结构状态::许可拒绝;
    case L1所有者范围写入状态::未找到: return L2结构状态::未找到;
    case L1所有者范围写入状态::已退出: return L2结构状态::已退出;
    case L1所有者范围写入状态::事实代次漂移: return L2结构状态::事实代次漂移;
    case L1所有者范围写入状态::幂等冲突: return L2结构状态::幂等冲突;
    case L1所有者范围写入状态::引用冲突: return L2结构状态::引用冲突;
    case L1所有者范围写入状态::资源失败: return L2结构状态::资源失败;
    case L1所有者范围写入状态::内部不一致: return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；L1 读取状态完整覆盖映射。
L2结构状态 映射读取状态(L1所有者范围读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::成功: return L2结构状态::已读取;
    case L1所有者范围读取状态::入口拒绝: return L2结构状态::入口拒绝;
    case L1所有者范围读取状态::许可拒绝: return L2结构状态::许可拒绝;
    case L1所有者范围读取状态::未找到: return L2结构状态::未找到;
    case L1所有者范围读取状态::已退出: return L2结构状态::已退出;
    case L1所有者范围读取状态::属性未设置: return L2结构状态::属性未设置;
    case L1所有者范围读取状态::事实代次漂移: return L2结构状态::事实代次漂移;
    case L1所有者范围读取状态::资源失败: return L2结构状态::资源失败;
    case L1所有者范围读取状态::内部不一致: return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；中性来源读取状态映射。
L2结构状态 映射中性来源状态(L1中性读取状态 状态) noexcept {
    switch (状态) {
    case L1中性读取状态::成功: return L2结构状态::已读取;
    case L1中性读取状态::入口拒绝: return L2结构状态::入口拒绝;
    case L1中性读取状态::未找到: return L2结构状态::未找到;
    case L1中性读取状态::已退出: return L2结构状态::已退出;
    case L1中性读取状态::资源失败: return L2结构状态::资源失败;
    case L1中性读取状态::属性未设置:
    case L1中性读取状态::内部不一致: return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；从确定映射中查找唯一编码。
std::optional<稳定编码> 查找编码(const L1所有者范围写入结果& 结果,
    L1所有者范围写集本地键 键) noexcept {
    const auto 位置 = std::find_if(结果.新编码映射.begin(), 结果.新编码映射.end(),
        [键](const auto& 项) noexcept { return 项.first == 键; });
    if (位置 == 结果.新编码映射.end()) return std::nullopt;
    if (std::find_if(std::next(位置), 结果.新编码映射.end(),
            [键](const auto& 项) noexcept { return 项.first == 键; })
        != 结果.新编码映射.end()) return std::nullopt;
    return 位置->second;
}

} // namespace 海中鱼巣::不可变材料内部

export namespace 海中鱼巣 {

inline constexpr L1所有者范围建立幂等身份 不可变材料所有者建立身份{
    0x4C32'4D41'5445'5201ULL};

class 不可变材料服务;

struct 不可变材料服务构造输入 final {
    不可变材料存储配置 存储配置;
    L1所有者范围交付 所有者交付;
    不可变材料服务构造输入() = default;
    不可变材料服务构造输入(const 不可变材料服务构造输入&) = delete;
    不可变材料服务构造输入& operator=(
        const 不可变材料服务构造输入&) = delete;
    不可变材料服务构造输入(不可变材料服务构造输入&&) noexcept = default;
    不可变材料服务构造输入& operator=(
        不可变材料服务构造输入&&) noexcept = default;
};

enum class 不可变材料服务构造状态 : std::uint8_t {
    成功 = 1, 配置无效 = 2, 所有者交付无效 = 3,
    目录资源失败 = 4, 登记失败 = 5, 资源失败 = 6, 内部不一致 = 7
};

struct 不可变材料服务构造结果 final {
    不可变材料服务构造状态 状态 =
        不可变材料服务构造状态::内部不一致;
    std::uint32_t 合同版本 = L2结构合同版本;
    std::unique_ptr<不可变材料服务> 服务;
    bool 成功() const noexcept;
};

class 不可变材料服务 final {
public:
    不可变材料服务() = delete;
    不可变材料服务(const 不可变材料服务&) = delete;
    不可变材料服务& operator=(const 不可变材料服务&) = delete;
    ~不可变材料服务();

    不可变材料写入结果 写入不可变材料(const 不可变材料写入请求& 请求);
    不可变材料读取结果 读取不可变材料(const 不可变材料读取请求& 请求) const;
    不可变材料退出结果 退出不可变材料(const 不可变材料退出请求& 请求);
    不可变材料登记读取结果 读取不可变材料登记(
        const 不可变材料登记读取请求& 请求) const;

    // 诊断责任：无适用错误分支；只比较构造时借入的 L1 服务实例地址。
    bool 使用同一L1事实基座实例(
        const L1事实基座服务& 第一层服务) const noexcept;

private:
    struct 实现;
    explicit 不可变材料服务(std::unique_ptr<实现> 实现值) noexcept;
    friend 不可变材料服务构造结果 构造不可变材料服务(
        const L1事实基座服务&, 不可变材料服务构造输入&&) noexcept;
    std::unique_ptr<实现> 实现_;
};

不可变材料服务构造结果 构造不可变材料服务(
    const L1事实基座服务& 第一层读取服务,
    不可变材料服务构造输入&& 输入) noexcept;

} // namespace 海中鱼巣

namespace 海中鱼巣 {

struct 不可变材料服务::实现 final {
    struct 候选文件 final {
        不可变材料内部::路径句柄事实 路径事实;
        std::uint64_t 长度 = 0;
        材料SHA256摘要 摘要;
    };

    struct 内容键 final {
        稳定编码 格式;
        std::uint64_t 长度 = 0;
        材料SHA256摘要 摘要;
    };

    struct 内容键比较 final {
        bool operator()(const 内容键& 左, const 内容键& 右) const noexcept {
            if (左.格式 != 右.格式) return 左.格式 < 右.格式;
            if (左.长度 != 右.长度) return 左.长度 < 右.长度;
            return 左.摘要.字节 < 右.摘要.字节;
        }
    };

    struct 写入协调项 final {
        不可变材料写入请求 请求;
        bool 执行中 = true;
        std::optional<不可变材料写入结果> 结果;
        std::condition_variable 完成条件;
    };

    struct 内容协调项 final {
        稳定编码 来源{};
        L2结构幂等身份 原幂等身份;
        bool 执行中 = true;
        std::optional<不可变材料写入结果> 结果;
        std::condition_variable 完成条件;
    };

    struct 材料读取内部结果 final {
        L2结构状态 状态 = L2结构状态::内部不一致;
        std::uint64_t 观察事实代次 = 0;
        std::optional<不可变材料事实> 材料;
        std::array<稳定编码, 3> 值编码{};
    };

    const L1事实基座服务& 第一层读取服务;
    L1所有者范围写端口 第一层写入端口;
    L1结构所有者身份 所有者;
    不可变材料内部::路径句柄事实 根;
    不可变材料内部::路径句柄事实 候选目录;
    不可变材料内部::路径句柄事实 对象目录;
    不可变材料内部::路径句柄事实 SHA256目录;
    不可变材料登记事实 登记;
    bool 登记首次发布 = false;

    mutable std::mutex 协调锁;
    std::unordered_map<std::uint64_t, std::shared_ptr<写入协调项>> 请求账;
    std::map<内容键, std::shared_ptr<内容协调项>, 内容键比较> 内容索引;

    实现(const L1事实基座服务& L1, L1所有者范围写端口&& 端口,
        L1结构所有者身份 所有者值,
        不可变材料内部::路径句柄事实&& 根值,
        不可变材料内部::路径句柄事实&& 候选值,
        不可变材料内部::路径句柄事实&& 对象值,
        不可变材料内部::路径句柄事实&& SHA256值,
        const 不可变材料登记事实& 登记值, bool 首次)
        : 第一层读取服务(L1), 第一层写入端口(std::move(端口)),
          所有者(所有者值), 根(std::move(根值)), 候选目录(std::move(候选值)),
          对象目录(std::move(对象值)), SHA256目录(std::move(SHA256值)),
          登记(登记值), 登记首次发布(首次) {}

    // 诊断责任：无适用错误分支；纯值失败结果形成。
    static 不可变材料写入结果 写入失败(L2结构状态 状态,
        std::uint64_t 截止 = 0) noexcept {
        return {{L2结构合同版本, 状态, 截止, std::nullopt}, std::nullopt};
    }

    // 诊断责任：无适用错误分支；纯值失败结果形成。
    static 不可变材料退出结果 退出失败(L2结构状态 状态,
        std::uint64_t 截止 = 0) noexcept {
        return {{L2结构合同版本, 状态, 截止, std::nullopt}, std::nullopt};
    }

    // 诊断责任：无适用错误分支；纯值失败结果形成。
    static 不可变材料读取结果 读取失败(const 不可变材料读取请求& 请求,
        L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
        return {{L2结构合同版本, 状态, 截止, std::nullopt},
            请求.读取类别, 请求.历史截止事实代次, std::nullopt};
    }

    // 诊断责任：无适用错误分支；读取请求只接受冻结的两种截止形状。
    static bool 读取请求有效(const 不可变材料读取请求& 请求) noexcept {
        if (请求.请求头.合同版本 != L2结构合同版本
            || 请求.请求头.期望事实代次 == 0 || !有效(请求.身份.值)) return false;
        if (请求.读取类别 == L2读取类别::当前)
            return 请求.历史截止事实代次 == 0;
        if (请求.读取类别 == L2读取类别::历史)
            return 请求.历史截止事实代次 != 0
                && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
        return false;
    }

    // 诊断责任：无适用错误分支；登记读取请求只接受冻结的两种截止形状。
    static bool 登记请求有效(const 不可变材料登记读取请求& 请求) noexcept {
        if (请求.请求头.合同版本 != L2结构合同版本
            || 请求.请求头.期望事实代次 == 0) return false;
        if (请求.读取类别 == L2读取类别::当前)
            return 请求.历史截止事实代次 == 0;
        if (请求.读取类别 == L2读取类别::历史)
            return 请求.历史截止事实代次 != 0
                && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
        return false;
    }

    // 诊断责任：向上送出；句柄证明失败保留入口或资源状态。
    std::pair<L2结构状态, std::optional<候选文件>> 打开候选(
        const 不可变材料写入请求& 请求) const {
        if (!不可变材料内部::路径字面量有效(请求.隔离候选路径))
            return {L2结构状态::入口拒绝, std::nullopt};
        const DWORD 属性 = GetFileAttributesW(请求.隔离候选路径.c_str());
        if (属性 == INVALID_FILE_ATTRIBUTES
            || (属性 & FILE_ATTRIBUTE_DIRECTORY) != 0
            || (属性 & FILE_ATTRIBUTE_REPARSE_POINT) != 0)
            return {L2结构状态::入口拒绝, std::nullopt};
        auto 路径事实 = 不可变材料内部::打开路径事实(
            请求.隔离候选路径, false, GENERIC_READ, FILE_SHARE_READ);
        if (!路径事实) return {L2结构状态::资源失败, std::nullopt};
        if (路径事实->卷序列 != 根.卷序列
            || !不可变材料内部::规范路径位于目录内(
                路径事实->规范路径, 候选目录.规范路径))
            return {L2结构状态::入口拒绝, std::nullopt};
        const bool UTF8 = 请求.格式角色 == 不可变材料格式角色::严格UTF8;
        const auto 校验 = 不可变材料内部::校验文件句柄(
            路径事实->句柄.取得(), UTF8);
        if (校验.裁决 != 不可变材料内部::文件裁决::成功)
            return {不可变材料内部::映射文件裁决(校验.裁决), std::nullopt};
        if (校验.长度 != 请求.字节长度 || 校验.摘要 != 请求.摘要)
            return {L2结构状态::入口拒绝, std::nullopt};
        return {L2结构状态::已读取,
            候选文件{std::move(*路径事实), 校验.长度, 校验.摘要}};
    }

    // 诊断责任：向上送出；来源只能通过正式中性节点读取成立。
    std::pair<L2结构状态, std::optional<稳定编码>> 读取来源(
        const 不可变材料写入请求& 请求) const {
        稳定编码 来源{};
        if (std::holds_alternative<不可变材料系统签发来源>(请求.来源))
            来源 = 登记.系统签发者节点;
        else if (const auto* 普通 =
                std::get_if<不可变材料已发布节点来源>(&请求.来源))
            来源 = 普通->节点;
        if (!有效(来源)) return {L2结构状态::入口拒绝, std::nullopt};
        const auto 读取 = 第一层读取服务.读取中性当前节点(
            {L1中性CRUD合同版本, 来源});
        if (读取.状态 != L1中性读取状态::成功) {
            const auto 状态 = 不可变材料内部::映射中性来源状态(读取.状态);
            return {状态, std::nullopt};
        }
        if (读取.合同版本 != L1中性CRUD合同版本
            || 读取.查询编码 != 来源 || 读取.读取事实代次 == 0
            || !读取.事实 || 读取.事实->编码 != 来源
            || 读取.事实->创建事实代次 == 0
            || 读取.事实->创建事实代次 > 读取.读取事实代次
            || 读取.事实->退出事实代次)
            return {L2结构状态::内部不一致, std::nullopt};
        if (读取.读取事实代次 != 请求.请求头.期望事实代次)
            return {L2结构状态::事实代次漂移, std::nullopt};
        return {L2结构状态::已读取, 来源};
    }

    // 诊断责任：向上送出；最终对象完整复读不使用路径缓存补成功。
    不可变材料内部::文件裁决 校验最终对象(
        const std::filesystem::path& 路径, const 材料SHA256摘要& 摘要,
        std::uint64_t 长度, HANDLE 比较句柄 = INVALID_HANDLE_VALUE) const {
        auto 最终 = 不可变材料内部::打开路径事实(
            路径, false, GENERIC_READ, FILE_SHARE_READ);
        if (!最终) return 不可变材料内部::文件裁决::资源失败;
        if (最终->卷序列 != 根.卷序列
            || !不可变材料内部::规范路径位于目录内(
                最终->规范路径, SHA256目录.规范路径))
            return 不可变材料内部::文件裁决::内部不一致;
        const auto 校验 = 不可变材料内部::校验文件句柄(
            最终->句柄.取得(), false);
        if (校验.裁决 != 不可变材料内部::文件裁决::成功)
            return 校验.裁决;
        if (校验.长度 != 长度 || 校验.摘要 != 摘要)
            return 不可变材料内部::文件裁决::内部不一致;
        if (比较句柄 != INVALID_HANDLE_VALUE)
            return 不可变材料内部::逐字节比较(
                比较句柄, 最终->句柄.取得());
        return 不可变材料内部::文件裁决::成功;
    }

    struct 物理发布结果 final {
        L2结构状态 状态 = L2结构状态::内部不一致;
        std::filesystem::path 最终路径;
        bool 既有对象 = false;
    };

    // 诊断责任：向上送出；临时写入、不可覆盖改名和未知结果均结构化返回。
    物理发布结果 发布物理对象(候选文件& 候选) {
        const std::wstring 十六进制 = 不可变材料内部::摘要十六进制(候选.摘要);
        const auto 分片路径 = SHA256目录.规范路径 / 十六进制.substr(0, 2);
        auto 分片 = 不可变材料内部::确保普通子目录(根, 分片路径);
        if (!分片 || 分片->卷序列 != 根.卷序列)
            return {L2结构状态::资源失败};
        const auto 最终路径 = 分片->规范路径 / (十六进制 + L".bin");
        const DWORD 最终属性 = GetFileAttributesW(最终路径.c_str());
        if (最终属性 != INVALID_FILE_ATTRIBUTES) {
            const auto 裁决 = 校验最终对象(最终路径, 候选.摘要,
                候选.长度, 候选.路径事实.句柄.取得());
            return {裁决 == 不可变材料内部::文件裁决::成功
                    ? L2结构状态::已读取
                    : 不可变材料内部::映射文件裁决(裁决),
                最终路径, true};
        }

        std::array<std::uint8_t, 16> 随机{};
        if (BCryptGenRandom(nullptr, 随机.data(), static_cast<ULONG>(随机.size()),
                BCRYPT_USE_SYSTEM_PREFERRED_RNG) < 0)
            return {L2结构状态::资源失败};
        static constexpr wchar_t 数字[] = L"0123456789abcdef";
        std::wstring 随机名 = L".候选-";
        for (const auto 字节 : 随机) {
            随机名.push_back(数字[字节 >> 4]);
            随机名.push_back(数字[字节 & 0x0F]);
        }
        随机名 += L".tmp";
        const auto 临时路径 = 分片->规范路径 / 随机名;
        不可变材料内部::唯一句柄 临时(CreateFileW(临时路径.c_str(),
            GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL, nullptr));
        if (!临时) return {L2结构状态::资源失败};

        LARGE_INTEGER 零{};
        if (!SetFilePointerEx(候选.路径事实.句柄.取得(), 零, nullptr, FILE_BEGIN)) {
            临时.重置(); DeleteFileW(临时路径.c_str());
            return {L2结构状态::资源失败};
        }
        std::array<std::uint8_t, 64 * 1024> 缓冲{};
        bool 写入成功 = true;
        for (;;) {
            DWORD 已读 = 0;
            if (!ReadFile(候选.路径事实.句柄.取得(), 缓冲.data(),
                    static_cast<DWORD>(缓冲.size()), &已读, nullptr)) {
                写入成功 = false; break;
            }
            if (已读 == 0) break;
            DWORD 总写 = 0;
            while (总写 < 已读) {
                DWORD 本次 = 0;
                if (!WriteFile(临时.取得(), 缓冲.data() + 总写,
                        已读 - 总写, &本次, nullptr) || 本次 == 0) {
                    写入成功 = false; break;
                }
                总写 += 本次;
            }
            if (!写入成功) break;
        }
        if (写入成功) 写入成功 = FlushFileBuffers(临时.取得()) != FALSE;
        if (写入成功) {
            const auto 复读 = 不可变材料内部::校验文件句柄(临时.取得(), false);
            写入成功 = 复读.裁决 == 不可变材料内部::文件裁决::成功
                && 复读.长度 == 候选.长度 && 复读.摘要 == 候选.摘要
                && 不可变材料内部::逐字节比较(
                    候选.路径事实.句柄.取得(), 临时.取得())
                    == 不可变材料内部::文件裁决::成功;
        }
        临时.重置();
        if (!写入成功) {
            DeleteFileW(临时路径.c_str());
            return {L2结构状态::资源失败};
        }

        if (MoveFileExW(临时路径.c_str(), 最终路径.c_str(),
                MOVEFILE_WRITE_THROUGH)) {
            const auto 裁决 = 校验最终对象(最终路径, 候选.摘要,
                候选.长度, 候选.路径事实.句柄.取得());
            return {裁决 == 不可变材料内部::文件裁决::成功
                    ? L2结构状态::已读取
                    : 不可变材料内部::映射文件裁决(裁决),
                最终路径, false};
        }
        const DWORD 错误 = GetLastError();
        const auto 裁决 = 校验最终对象(最终路径, 候选.摘要,
            候选.长度, 候选.路径事实.句柄.取得());
        DeleteFileW(临时路径.c_str());
        if (裁决 == 不可变材料内部::文件裁决::成功)
            return {L2结构状态::已读取, 最终路径, true};
        if (错误 == ERROR_ALREADY_EXISTS || 错误 == ERROR_FILE_EXISTS)
            return {L2结构状态::内部不一致, 最终路径, true};
        return {裁决 == 不可变材料内部::文件裁决::内部不一致
                ? L2结构状态::内部不一致 : L2结构状态::资源失败,
            最终路径, false};
    }

    // 诊断责任：向上送出；材料四事实读回形成唯一结构化状态。
    材料读取内部结果 读取材料截止(不可变材料身份 身份,
        L2读取类别 类别, std::uint64_t 截止,
        std::optional<std::uint64_t> 期望当前代次) const {
        const auto 前代次 = 第一层读取服务.读取中性当前事实代次(
            {L1中性CRUD合同版本});
        if (前代次.状态 != L1中性读取状态::成功
            || 前代次.合同版本 != L1中性CRUD合同版本
            || 前代次.事实代次 == 0)
            return {不可变材料内部::映射中性来源状态(前代次.状态)};
        if (期望当前代次 && 前代次.事实代次 != *期望当前代次)
            return {L2结构状态::事实代次漂移, 前代次.事实代次};

        const auto 节点读取 = 第一层读取服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 身份.值});
        if (节点读取.状态 != L1所有者范围读取状态::成功)
            return {不可变材料内部::映射读取状态(节点读取.状态),
                节点读取.读取事实代次};
        const auto* 节点 = 节点读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
        if (节点读取.合同版本 != L1所有者范围CRUD合同版本
            || 节点读取.查询编码 != 身份.值 || !节点
            || 节点->编码 != 身份.值 || 节点->写入所有者 != 所有者
            || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
            || 节点->创建事实代次 == 0)
            return {L2结构状态::内部不一致, 节点读取.读取事实代次};
        if (截止 < 节点->创建事实代次)
            return {L2结构状态::未找到, 前代次.事实代次};
        if (类别 == L2读取类别::当前 && 节点->退出事实代次)
            return {L2结构状态::已退出, 前代次.事实代次};
        if (节点->当前属性.size() != 3)
            return {L2结构状态::内部不一致, 前代次.事实代次};

        std::array<稳定编码, 3> 值编码{};
        for (const auto& 槽 : 节点->当前属性) {
            std::size_t 位置 = 3;
            if (槽.属性类型节点 == 登记.格式属性类型) 位置 = 0;
            else if (槽.属性类型节点 == 登记.长度属性类型) 位置 = 1;
            else if (槽.属性类型节点 == 登记.摘要属性类型) 位置 = 2;
            if (位置 == 3 || 有效(值编码[位置]) || !有效(槽.当前值))
                return {L2结构状态::内部不一致, 前代次.事实代次};
            值编码[位置] = 槽.当前值;
        }
        if (!有效(值编码[0]) || !有效(值编码[1]) || !有效(值编码[2])
            || 值编码[0] == 值编码[1] || 值编码[0] == 值编码[2]
            || 值编码[1] == 值编码[2])
            return {L2结构状态::内部不一致, 前代次.事实代次};

        std::array<L1所有者范围值事实, 3> 值事实{};
        const 稳定编码 类型组[] = {登记.格式属性类型,
            登记.长度属性类型, 登记.摘要属性类型};
        for (std::size_t 序号 = 0; 序号 < 3; ++序号) {
            const auto 读取 = 第一层读取服务.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 值编码[序号]});
            const auto* 值 = 读取.事实
                ? std::get_if<L1所有者范围值事实>(&*读取.事实) : nullptr;
            if (读取.状态 != L1所有者范围读取状态::成功
                || 读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.查询编码 != 值编码[序号] || !值
                || 值->编码 != 值编码[序号] || 值->写入所有者 != 所有者
                || 值->所属节点 != 身份.值 || 值->属性类型节点 != 类型组[序号]
                || 值->创建事实代次 != 节点->创建事实代次
                || 值->来源节点 != 值事实[0].来源节点 && 序号 != 0)
                return {L2结构状态::内部不一致, 读取.读取事实代次};
            值事实[序号] = *值;
        }
        if (!有效(值事实[0].来源节点)
            || 值事实[1].来源节点 != 值事实[0].来源节点
            || 值事实[2].来源节点 != 值事实[0].来源节点)
            return {L2结构状态::内部不一致, 前代次.事实代次};

        const auto* 格式组 = std::get_if<std::vector<std::uint64_t>>(&值事实[0].材料);
        const auto* 长度组 = std::get_if<std::vector<std::uint64_t>>(&值事实[1].材料);
        const auto 摘要 = 不可变材料内部::从U64组读取摘要(值事实[2].材料);
        if (!格式组 || 格式组->size() != 1 || !长度组 || 长度组->size() != 1
            || (*长度组)[0] == 0 || !摘要)
            return {L2结构状态::内部不一致, 前代次.事实代次};
        const 稳定编码 格式编码{(*格式组)[0]};
        不可变材料格式角色 格式角色 = static_cast<不可变材料格式角色>(0);
        if (格式编码 == 登记.严格UTF8格式身份.值)
            格式角色 = 不可变材料格式角色::严格UTF8;
        else if (格式编码 == 登记.通用字节格式身份.值)
            格式角色 = 不可变材料格式角色::通用字节;
        else return {L2结构状态::内部不一致, 前代次.事实代次};

        const auto 最终路径 = SHA256目录.规范路径
            / 不可变材料内部::摘要十六进制(*摘要).substr(0, 2)
            / (不可变材料内部::摘要十六进制(*摘要) + L".bin");
        const auto 文件 = 校验最终对象(最终路径, *摘要, (*长度组)[0]);
        if (文件 != 不可变材料内部::文件裁决::成功)
            return {文件 == 不可变材料内部::文件裁决::资源失败
                    ? L2结构状态::内部不一致
                    : 不可变材料内部::映射文件裁决(文件),
                前代次.事实代次};

        const auto 后代次 = 第一层读取服务.读取中性当前事实代次(
            {L1中性CRUD合同版本});
        if (后代次.状态 != L1中性读取状态::成功
            || 后代次.事实代次 != 前代次.事实代次)
            return {后代次.状态 == L1中性读取状态::成功
                    ? L2结构状态::事实代次漂移
                    : 不可变材料内部::映射中性来源状态(后代次.状态),
                后代次.事实代次};

        const auto 退出 = [&](std::optional<std::uint64_t> 值)
            -> std::optional<std::uint64_t> {
            return 值 && *值 <= 截止 ? 值 : std::nullopt;
        };
        const auto 节点退出 = 退出(节点->退出事实代次);
        for (const auto& 值 : 值事实)
            if (退出(值.退出事实代次) != 节点退出)
                return {L2结构状态::内部不一致, 后代次.事实代次};
        不可变材料事实 结果{{节点->编码}, 格式角色, {格式编码},
            (*长度组)[0], *摘要, 值事实[0].来源节点,
            {节点->创建事实代次, 节点退出},
            不可变材料物理定位{最终路径}};
        return {L2结构状态::已读取, 后代次.事实代次,
            std::move(结果), 值编码};
    }

    // 诊断责任：向上送出；正式写集结果和首次截止读回共同裁决成功。
    不可变材料写入结果 提交材料写集(const 不可变材料写入请求& 请求,
        稳定编码 来源, 不可变材料格式身份 格式,
        const std::filesystem::path& 最终路径) {
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 请求.请求头.期望事实代次;
        写集.写入幂等身份 = {请求.幂等身份.值};
        写集.节点 = {{不可变材料内部::材料节点本地键,
            节点种类::普通, std::nullopt}};
        写集.值 = {
            {不可变材料内部::格式值本地键,
                不可变材料内部::材料节点本地键, 登记.格式属性类型,
                std::vector<std::uint64_t>{格式.值.值}, 来源},
            {不可变材料内部::长度值本地键,
                不可变材料内部::材料节点本地键, 登记.长度属性类型,
                std::vector<std::uint64_t>{请求.字节长度}, 来源},
            {不可变材料内部::摘要值本地键,
                不可变材料内部::材料节点本地键, 登记.摘要属性类型,
                不可变材料内部::摘要U64组(请求.摘要), 来源}};
        写集.属性槽变更 = {
            {不可变材料内部::材料节点本地键, 登记.格式属性类型,
                不可变材料内部::格式值本地键},
            {不可变材料内部::材料节点本地键, 登记.长度属性类型,
                不可变材料内部::长度值本地键},
            {不可变材料内部::材料节点本地键, 登记.摘要属性类型,
                不可变材料内部::摘要值本地键}};
        const auto 写入 = 第一层写入端口.提交所有者范围中性写集(写集);
        const auto 状态 = 不可变材料内部::映射写入状态(写入.状态);
        if (状态 != L2结构状态::已提交 && 状态 != L2结构状态::精确重复)
            return 写入失败(状态, 写入.事实代次);
        const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
            && 写入.是否形成内存权威发布
            && 写入.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
            && !写入.是否形成内存权威发布
            && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
        const auto 节点编码 = 不可变材料内部::查找编码(
            写入, 不可变材料内部::材料节点本地键);
        if (写入.合同版本 != L1所有者范围CRUD合同版本
            || 写入.所有者 != 所有者
            || 写入.写入幂等身份.值 != 请求.幂等身份.值
            || 写入.事实代次 == 0 || (!首次完整 && !重复完整)
            || 写入.新编码映射.size() != 4 || !节点编码)
            return 写入失败(L2结构状态::内部不一致, 写入.事实代次);
        const auto 读回 = 读取材料截止({*节点编码}, L2读取类别::历史,
            写入.事实代次, std::nullopt);
        if (读回.状态 != L2结构状态::已读取 || !读回.材料
            || 读回.材料->身份.值 != *节点编码
            || 读回.材料->格式身份 != 格式
            || 读回.材料->字节长度 != 请求.字节长度
            || 读回.材料->摘要 != 请求.摘要
            || 读回.材料->来源节点 != 来源
            || 读回.材料->生命周期.创建事实代次 != 写入.事实代次
            || 读回.材料->生命周期.退出事实代次
            || !读回.材料->物理定位
            || 读回.材料->物理定位->摘要对象绝对路径 != 最终路径)
            return 写入失败(L2结构状态::内部不一致, 写入.事实代次);
        return {{L2结构合同版本, 状态, 写入.事实代次,
            写入.事实代次}, std::move(读回.材料)};
    }

    // 诊断责任：向上送出；缓存命中仍从文件与 L1 首次截止读回。
    不可变材料写入结果 重读首次结果(
        const 不可变材料写入结果& 首次) const {
        if (!首次.材料 || !首次.结果头.变更事实代次)
            return 写入失败(L2结构状态::内部不一致);
        const auto 读回 = 读取材料截止(首次.材料->身份,
            L2读取类别::历史, *首次.结果头.变更事实代次, std::nullopt);
        if (读回.状态 != L2结构状态::已读取 || !读回.材料
            || 读回.材料->生命周期.创建事实代次
                != *首次.结果头.变更事实代次)
            return 写入失败(L2结构状态::内部不一致, 读回.观察事实代次);
        return {{L2结构合同版本, L2结构状态::精确重复,
            *首次.结果头.变更事实代次, *首次.结果头.变更事实代次},
            std::move(读回.材料)};
    }

    // 诊断责任：本地记录；全部异常在唯一公开边界映射为结构化状态。
    不可变材料写入结果 写入(const 不可变材料写入请求& 请求) {
        if (请求.请求头.合同版本 != L2结构合同版本
            || 请求.请求头.期望事实代次 == 0
            || 请求.规则版本 != 不可变材料规则版本
            || !L2结构幂等身份有效(请求.幂等身份)
            || 请求.幂等身份.值 == 不可变材料内部::材料登记幂等身份.值
            || 请求.字节长度 == 0
            || (请求.格式角色 != 不可变材料格式角色::严格UTF8
                && 请求.格式角色 != 不可变材料格式角色::通用字节))
            return 写入失败(L2结构状态::入口拒绝);

        std::shared_ptr<写入协调项> 请求项;
        {
            std::unique_lock<std::mutex> 锁(协调锁);
            const auto 位置 = 请求账.find(请求.幂等身份.值);
            if (位置 != 请求账.end()) {
                请求项 = 位置->second;
                if (请求项->请求 != 请求)
                    return 写入失败(L2结构状态::幂等冲突);
                while (请求项->执行中) 请求项->完成条件.wait(锁);
                if (请求项->结果 && 请求项->结果->成功()) {
                    const auto 首次 = *请求项->结果;
                    锁.unlock();
                    return 重读首次结果(首次);
                }
                请求项->执行中 = true;
                请求项->结果.reset();
            } else {
                请求项 = std::make_shared<写入协调项>();
                请求项->请求 = 请求;
                请求账.emplace(请求.幂等身份.值, 请求项);
            }
        }
        const auto 完成请求 = [&](不可变材料写入结果 结果) {
            std::lock_guard<std::mutex> 锁(协调锁);
            请求项->结果 = 结果;
            请求项->执行中 = false;
            请求项->完成条件.notify_all();
            return 结果;
        };

        const auto [来源状态, 来源] = 读取来源(请求);
        if (!来源) return 完成请求(写入失败(来源状态));
        auto [候选状态, 候选] = 打开候选(请求);
        if (!候选) return 完成请求(写入失败(候选状态));
        const 不可变材料格式身份 格式{
            请求.格式角色 == 不可变材料格式角色::严格UTF8
                ? 登记.严格UTF8格式身份.值 : 登记.通用字节格式身份.值};
        const 内容键 键{格式.值, 请求.字节长度, 请求.摘要};

        std::shared_ptr<内容协调项> 内容项;
        {
            std::unique_lock<std::mutex> 锁(协调锁);
            auto 位置 = 内容索引.find(键);
            if (位置 != 内容索引.end()) {
                内容项 = 位置->second;
                if (内容项->来源 != *来源) {
                    锁.unlock();
                    return 完成请求(写入失败(L2结构状态::幂等冲突));
                }
                while (内容项->执行中) 内容项->完成条件.wait(锁);
                if (内容项->结果 && 内容项->结果->成功()) {
                    const auto 首次 = *内容项->结果;
                    锁.unlock();
                    return 完成请求(重读首次结果(首次));
                }
                if (内容项->原幂等身份 != 请求.幂等身份) {
                    锁.unlock();
                    return 完成请求(写入失败(L2结构状态::资源失败));
                }
                内容项->执行中 = true;
                内容项->结果.reset();
            } else {
                内容项 = std::make_shared<内容协调项>();
                内容项->来源 = *来源;
                内容项->原幂等身份 = 请求.幂等身份;
                内容索引.emplace(键, 内容项);
            }
        }
        const auto 完成内容 = [&](不可变材料写入结果 结果) {
            {
                std::lock_guard<std::mutex> 锁(协调锁);
                内容项->结果 = 结果;
                内容项->执行中 = false;
                内容项->完成条件.notify_all();
            }
            return 完成请求(std::move(结果));
        };

        const auto 物理 = 发布物理对象(*候选);
        if (物理.状态 != L2结构状态::已读取)
            return 完成内容(写入失败(物理.状态));
        if (物理.既有对象 && !登记首次发布
            && 内容项->原幂等身份 == 请求.幂等身份
            && !内容项->结果)
            return 完成内容(写入失败(L2结构状态::内部不一致));
        return 完成内容(提交材料写集(请求, *来源, 格式, 物理.最终路径));
    }

    // 诊断责任：本地记录；读取异常不泄漏半载荷。
    不可变材料读取结果 读取(const 不可变材料读取请求& 请求) const {
        if (!读取请求有效(请求)) return 读取失败(请求, L2结构状态::入口拒绝);
        const std::uint64_t 截止 = 请求.读取类别 == L2读取类别::当前
            ? 请求.请求头.期望事实代次 : 请求.历史截止事实代次;
        const auto 读回 = 读取材料截止(请求.身份, 请求.读取类别,
            截止, 请求.请求头.期望事实代次);
        if (读回.状态 != L2结构状态::已读取)
            return 读取失败(请求, 读回.状态, 读回.观察事实代次);
        return {{L2结构合同版本, L2结构状态::已读取, 截止, std::nullopt},
            请求.读取类别, 请求.历史截止事实代次, 读回.材料};
    }

    // 诊断责任：本地记录；退出闭包只由当前三个权威槽形成。
    不可变材料退出结果 退出(const 不可变材料退出请求& 请求) {
        if (请求.请求头.合同版本 != L2结构合同版本
            || 请求.请求头.期望事实代次 == 0
            || 请求.规则版本 != 不可变材料规则版本
            || !L2结构幂等身份有效(请求.幂等身份)
            || 请求.幂等身份.值 == 不可变材料内部::材料登记幂等身份.值
            || !有效(请求.身份.值)) return 退出失败(L2结构状态::入口拒绝);
        const auto 当前 = 读取材料截止(请求.身份, L2读取类别::当前,
            请求.请求头.期望事实代次, 请求.请求头.期望事实代次);
        if (当前.状态 != L2结构状态::已读取 || !当前.材料)
            return 退出失败(当前.状态, 当前.观察事实代次);
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 请求.请求头.期望事实代次;
        写集.写入幂等身份 = {请求.幂等身份.值};
        写集.退出事实 = {请求.身份.值, 当前.值编码[0],
            当前.值编码[1], 当前.值编码[2]};
        const auto 写入 = 第一层写入端口.提交所有者范围中性写集(写集);
        const auto 状态 = 不可变材料内部::映射写入状态(写入.状态);
        if (状态 != L2结构状态::已提交 && 状态 != L2结构状态::精确重复)
            return 退出失败(状态, 写入.事实代次);
        const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
            && 写入.是否形成内存权威发布
            && 写入.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
            && !写入.是否形成内存权威发布
            && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
        if (写入.合同版本 != L1所有者范围CRUD合同版本
            || 写入.所有者 != 所有者
            || 写入.写入幂等身份.值 != 请求.幂等身份.值
            || 写入.事实代次 == 0 || (!首次完整 && !重复完整)
            || !写入.新编码映射.empty())
            return 退出失败(L2结构状态::内部不一致, 写入.事实代次);
        const auto 历史 = 读取材料截止(请求.身份, L2读取类别::历史,
            写入.事实代次, std::nullopt);
        if (历史.状态 != L2结构状态::已读取 || !历史.材料
            || !历史.材料->生命周期.退出事实代次
            || *历史.材料->生命周期.退出事实代次 != 写入.事实代次)
            return 退出失败(L2结构状态::内部不一致, 写入.事实代次);
        return {{L2结构合同版本, 状态, 写入.事实代次,
            写入.事实代次}, 历史.材料};
    }

    // 诊断责任：本地记录；登记每次从 L1 六节点正式读回。
    不可变材料登记读取结果 读取登记(
        const 不可变材料登记读取请求& 请求) const {
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            return 不可变材料登记读取结果{{L2结构合同版本, 状态, 截止,
                std::nullopt}, 请求.读取类别, 请求.历史截止事实代次,
                std::nullopt};
        };
        if (!登记请求有效(请求)) return 失败(L2结构状态::入口拒绝);
        const auto 前 = 第一层读取服务.读取中性当前事实代次(
            {L1中性CRUD合同版本});
        if (前.状态 != L1中性读取状态::成功)
            return 失败(不可变材料内部::映射中性来源状态(前.状态));
        if (前.事实代次 != 请求.请求头.期望事实代次)
            return 失败(L2结构状态::事实代次漂移, 前.事实代次);
        const std::uint64_t 截止 = 请求.读取类别 == L2读取类别::当前
            ? 请求.请求头.期望事实代次 : 请求.历史截止事实代次;
        if (截止 < 登记.事实截止代次) return 失败(L2结构状态::未找到, 截止);
        const 稳定编码 编码组[] = {登记.系统签发者节点,
            登记.严格UTF8格式身份.值, 登记.通用字节格式身份.值,
            登记.格式属性类型, 登记.长度属性类型, 登记.摘要属性类型};
        for (std::size_t 序号 = 0; 序号 < 6; ++序号) {
            const auto 读取 = 第一层读取服务.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 编码组[序号]});
            const auto* 节点 = 读取.事实
                ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
            const bool 属性类型 = 序号 >= 3;
            if (读取.状态 != L1所有者范围读取状态::成功 || !节点
                || 节点->编码 != 编码组[序号] || 节点->写入所有者 != 所有者
                || 节点->创建事实代次 != 登记.事实截止代次
                || 节点->退出事实代次 || !节点->当前属性.empty()
                || (属性类型 && (节点->种类 != 节点种类::属性类型
                    || 节点->属性类型表示
                        != L1所有者范围值表示种类::U64组))
                || (!属性类型 && (节点->种类 != 节点种类::普通
                    || 节点->属性类型表示)))
                return 失败(L2结构状态::内部不一致, 读取.读取事实代次);
        }
        const auto 后 = 第一层读取服务.读取中性当前事实代次(
            {L1中性CRUD合同版本});
        if (后.状态 != L1中性读取状态::成功
            || 后.事实代次 != 前.事实代次)
            return 失败(L2结构状态::事实代次漂移, 后.事实代次);
        auto 输出 = 登记;
        输出.事实截止代次 = 截止;
        return {{L2结构合同版本, L2结构状态::已读取, 截止, std::nullopt},
            请求.读取类别, 请求.历史截止事实代次, 输出};
    }
};

// 诊断责任：无适用错误分支；构造结果纯值谓词。
bool 不可变材料服务构造结果::成功() const noexcept {
    return 状态 == 不可变材料服务构造状态::成功
        && 合同版本 == L2结构合同版本 && 服务 != nullptr;
}

不可变材料服务::不可变材料服务(std::unique_ptr<实现> 实现值) noexcept
    : 实现_(std::move(实现值)) {}

inline 不可变材料服务::~不可变材料服务() = default;

bool 不可变材料服务::使用同一L1事实基座实例(
    const L1事实基座服务& 第一层服务) const noexcept {
    return 实现_ && &实现_->第一层读取服务 == &第一层服务;
}

// 诊断责任：本地记录；异常映射不改变已经发布的权威事实。
不可变材料写入结果 不可变材料服务::写入不可变材料(
    const 不可变材料写入请求& 请求) {
    try { return 实现_->写入(请求); }
    catch (const std::bad_alloc&) { return 实现::写入失败(L2结构状态::资源失败); }
    catch (const std::length_error&) { return 实现::写入失败(L2结构状态::资源失败); }
    catch (...) { return 实现::写入失败(L2结构状态::内部不一致); }
}

// 诊断责任：本地记录；异常映射不泄漏半载荷。
不可变材料读取结果 不可变材料服务::读取不可变材料(
    const 不可变材料读取请求& 请求) const {
    try { return 实现_->读取(请求); }
    catch (const std::bad_alloc&) { return 实现::读取失败(请求, L2结构状态::资源失败); }
    catch (const std::length_error&) { return 实现::读取失败(请求, L2结构状态::资源失败); }
    catch (...) { return 实现::读取失败(请求, L2结构状态::内部不一致); }
}

// 诊断责任：本地记录；异常映射不回滚已发布退出。
不可变材料退出结果 不可变材料服务::退出不可变材料(
    const 不可变材料退出请求& 请求) {
    try { return 实现_->退出(请求); }
    catch (const std::bad_alloc&) { return 实现::退出失败(L2结构状态::资源失败); }
    catch (const std::length_error&) { return 实现::退出失败(L2结构状态::资源失败); }
    catch (...) { return 实现::退出失败(L2结构状态::内部不一致); }
}

// 诊断责任：本地记录；异常映射不泄漏部分登记。
不可变材料登记读取结果 不可变材料服务::读取不可变材料登记(
    const 不可变材料登记读取请求& 请求) const {
    try { return 实现_->读取登记(请求); }
    catch (...) {
        return {{L2结构合同版本, L2结构状态::内部不一致, 0, std::nullopt},
            请求.读取类别, 请求.历史截止事实代次, std::nullopt};
    }
}

namespace {

// 诊断责任：向上送出；owner 交付只接受同实例、同身份和独占端口。
std::optional<L1所有者范围写端口> 验证并移动材料交付(
    const L1事实基座服务& L1, L1所有者范围交付&& 交付,
    L1结构所有者身份& 所有者) {
    const auto& 建立 = 交付.建立结果;
    const bool 首次完整 = 建立.状态 == L1所有者范围管理状态::成功
        && 建立.是否形成内存权威发布
        && 建立.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 建立.状态 == L1所有者范围管理状态::精确重复
        && !建立.是否形成内存权威发布
        && 建立.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (建立.合同版本 != L1所有者范围CRUD合同版本
        || 建立.建立幂等身份 != 不可变材料所有者建立身份
        || 建立.事实代次 == 0 || (!首次完整 && !重复完整)
        || !建立.所有者事实 || !交付.写入端口
        || 建立.所有者事实->范围种类 != L1所有者范围种类::独占结构范围
        || 建立.所有者事实->创建事实代次 == 0
        || 建立.所有者事实->退出事实代次
        || 交付.写入端口->所有者身份() != 建立.所有者事实->所有者
        || !交付.写入端口->有效() || !交付.写入端口->绑定于(L1))
        return std::nullopt;
    const auto 当前 = L1.读取当前结构所有者(
        {L1所有者范围CRUD合同版本, 建立.所有者事实->所有者});
    if (当前.状态 != L1所有者范围读取状态::成功
        || 当前.合同版本 != L1所有者范围CRUD合同版本
        || 当前.查询所有者 != 建立.所有者事实->所有者
        || !当前.所有者事实 || *当前.所有者事实 != *建立.所有者事实)
        return std::nullopt;
    所有者 = 建立.所有者事实->所有者;
    return std::move(*交付.写入端口);
}

struct 登记建立结果 final {
    不可变材料服务构造状态 状态 = 不可变材料服务构造状态::内部不一致;
    不可变材料登记事实 登记;
    bool 首次发布 = false;
};

// 诊断责任：向上送出；固定六节点只由一份 owner 写集建立并读回。
登记建立结果 建立材料登记(const L1事实基座服务& L1,
    L1所有者范围写端口& 端口, L1结构所有者身份 所有者) {
    const auto 所有者读取 = L1.读取当前结构所有者(
        {L1所有者范围CRUD合同版本, 所有者});
    if (所有者读取.状态 != L1所有者范围读取状态::成功
        || !所有者读取.所有者事实
        || 所有者读取.所有者事实->所有者 != 所有者
        || 所有者读取.所有者事实->创建事实代次 == 0
        || 所有者读取.所有者事实->退出事实代次)
        return {所有者读取.状态 == L1所有者范围读取状态::资源失败
                ? 不可变材料服务构造状态::资源失败
                : 不可变材料服务构造状态::登记失败};
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 所有者读取.所有者事实->创建事实代次;
    写集.写入幂等身份 = 不可变材料内部::材料登记幂等身份;
    写集.节点 = {
        {不可变材料内部::系统签发者本地键, 节点种类::普通, std::nullopt},
        {不可变材料内部::严格UTF8格式本地键, 节点种类::普通, std::nullopt},
        {不可变材料内部::通用字节格式本地键, 节点种类::普通, std::nullopt},
        {不可变材料内部::格式属性类型本地键, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {不可变材料内部::长度属性类型本地键, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {不可变材料内部::摘要属性类型本地键, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组}};
    const auto 写入 = 端口.提交所有者范围中性写集(写集);
    if (写入.状态 == L1所有者范围写入状态::资源失败)
        return {不可变材料服务构造状态::资源失败};
    if (写入.状态 != L1所有者范围写入状态::成功
        && 写入.状态 != L1所有者范围写入状态::精确重复)
        return {不可变材料服务构造状态::登记失败};
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (写入.合同版本 != L1所有者范围CRUD合同版本
        || 写入.所有者 != 所有者
        || 写入.写入幂等身份 != 不可变材料内部::材料登记幂等身份
        || 写入.事实代次 == 0 || (!首次完整 && !重复完整)
        || 写入.新编码映射.size() != 6)
        return {不可变材料服务构造状态::内部不一致};
    const auto 签发 = 不可变材料内部::查找编码(
        写入, 不可变材料内部::系统签发者本地键);
    const auto UTF8 = 不可变材料内部::查找编码(
        写入, 不可变材料内部::严格UTF8格式本地键);
    const auto 字节 = 不可变材料内部::查找编码(
        写入, 不可变材料内部::通用字节格式本地键);
    const auto 格式类型 = 不可变材料内部::查找编码(
        写入, 不可变材料内部::格式属性类型本地键);
    const auto 长度类型 = 不可变材料内部::查找编码(
        写入, 不可变材料内部::长度属性类型本地键);
    const auto 摘要类型 = 不可变材料内部::查找编码(
        写入, 不可变材料内部::摘要属性类型本地键);
    if (!签发 || !UTF8 || !字节 || !格式类型 || !长度类型 || !摘要类型)
        return {不可变材料服务构造状态::内部不一致};
    const 稳定编码 编码组[] = {*签发, *UTF8, *字节, *格式类型, *长度类型, *摘要类型};
    for (std::size_t 序号 = 0; 序号 < 6; ++序号) {
        const auto 读取 = L1.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码组[序号]});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        const bool 属性类型 = 序号 >= 3;
        if (读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->编码 != 编码组[序号] || 节点->写入所有者 != 所有者
            || 节点->创建事实代次 != 写入.事实代次
            || 节点->退出事实代次 || !节点->当前属性.empty()
            || (属性类型 && (节点->种类 != 节点种类::属性类型
                || 节点->属性类型表示 != L1所有者范围值表示种类::U64组))
            || (!属性类型 && (节点->种类 != 节点种类::普通
                || 节点->属性类型表示)))
            return {不可变材料服务构造状态::内部不一致};
    }
    return {不可变材料服务构造状态::成功,
        {*签发, {*UTF8}, {*字节}, *格式类型, *长度类型, *摘要类型,
            写入.事实代次}, 首次完整};
}

} // namespace

// 诊断责任：本地记录；构造失败不发布服务实例且不退出既有登记。
不可变材料服务构造结果 构造不可变材料服务(
    const L1事实基座服务& 第一层读取服务,
    不可变材料服务构造输入&& 输入) noexcept {
    const auto 失败 = [](不可变材料服务构造状态 状态) {
        return 不可变材料服务构造结果{状态, L2结构合同版本, nullptr};
    };
    try {
        const auto& 配置 = 输入.存储配置;
        if (配置.合同版本 != L2结构合同版本
            || !不可变材料内部::路径字面量有效(配置.受控根.绝对路径))
            return 失败(不可变材料服务构造状态::配置无效);
        const DWORD 属性 = GetFileAttributesW(配置.受控根.绝对路径.c_str());
        if (属性 == INVALID_FILE_ATTRIBUTES
            || (属性 & FILE_ATTRIBUTE_DIRECTORY) == 0
            || (属性 & FILE_ATTRIBUTE_REPARSE_POINT) != 0)
            return 失败(不可变材料服务构造状态::配置无效);
        auto 根 = 不可变材料内部::打开路径事实(配置.受控根.绝对路径, true);
        if (!根) return 失败(不可变材料服务构造状态::目录资源失败);
        auto 候选 = 不可变材料内部::确保普通子目录(*根, 根->规范路径 / L"候选");
        auto 对象 = 不可变材料内部::确保普通子目录(*根, 根->规范路径 / L"对象");
        if (!候选 || !对象)
            return 失败(不可变材料服务构造状态::目录资源失败);
        auto SHA256 = 不可变材料内部::确保普通子目录(*根,
            对象->规范路径 / L"sha256");
        if (!SHA256)
            return 失败(不可变材料服务构造状态::目录资源失败);

        L1结构所有者身份 所有者{};
        auto 端口 = 验证并移动材料交付(
            第一层读取服务, std::move(输入.所有者交付), 所有者);
        if (!端口) return 失败(不可变材料服务构造状态::所有者交付无效);
        const auto 登记 = 建立材料登记(第一层读取服务, *端口, 所有者);
        if (登记.状态 != 不可变材料服务构造状态::成功)
            return 失败(登记.状态);
        auto 实现值 = std::make_unique<不可变材料服务::实现>(
            第一层读取服务, std::move(*端口), 所有者,
            std::move(*根), std::move(*候选), std::move(*对象),
            std::move(*SHA256), 登记.登记, 登记.首次发布);
        auto 服务 = std::unique_ptr<不可变材料服务>(
            new 不可变材料服务(std::move(实现值)));
        return {不可变材料服务构造状态::成功, L2结构合同版本,
            std::move(服务)};
    } catch (const std::bad_alloc&) {
        return 失败(不可变材料服务构造状态::资源失败);
    } catch (const std::length_error&) {
        return 失败(不可变材料服务构造状态::资源失败);
    } catch (...) {
        return 失败(不可变材料服务构造状态::内部不一致);
    }
}

} // namespace 海中鱼巣
