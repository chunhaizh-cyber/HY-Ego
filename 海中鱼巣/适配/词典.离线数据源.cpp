#include "词典.离线数据源.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <limits>
#include <map>
#include <new>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <utility>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>

namespace 海中鱼巣 {
namespace {

class 批次数据错误 final : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class 摘要计算 final {
public:
    摘要计算() {
        if (BCryptOpenAlgorithmProvider(&算法_, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0)
            throw std::runtime_error("无法打开 SHA256 提供者");
        if (BCryptCreateHash(算法_, &句柄_, nullptr, 0, nullptr, 0, 0) < 0) {
            BCryptCloseAlgorithmProvider(算法_, 0);
            算法_ = nullptr;
            throw std::runtime_error("无法建立 SHA256 计算");
        }
    }
    ~摘要计算() {
        if (句柄_) BCryptDestroyHash(句柄_);
        if (算法_) BCryptCloseAlgorithmProvider(算法_, 0);
    }
    摘要计算(const 摘要计算&) = delete;
    摘要计算& operator=(const 摘要计算&) = delete;
    void 追加(const char* 数据, std::size_t 长度) {
        if (长度 > std::numeric_limits<ULONG>::max()
            || BCryptHashData(句柄_, reinterpret_cast<PUCHAR>(const_cast<char*>(数据)),
                static_cast<ULONG>(长度), 0) < 0)
            throw std::runtime_error("SHA256 计算失败");
    }
    std::string 完成() {
        std::array<unsigned char, 32> 字节{};
        if (BCryptFinishHash(句柄_, 字节.data(), static_cast<ULONG>(字节.size()), 0) < 0)
            throw std::runtime_error("SHA256 完成失败");
        std::string 文本;
        for (const auto 值 : 字节) {
            文本 += "0123456789abcdef"[值 >> 4];
            文本 += "0123456789abcdef"[值 & 15];
        }
        return 文本;
    }
private:
    BCRYPT_ALG_HANDLE 算法_ = nullptr;
    BCRYPT_HASH_HANDLE 句柄_ = nullptr;
};

struct 批次文件 final {
    std::uint64_t 字节数 = 0, 数据行数 = 0;
    std::string 摘要;
};

// 摘要计算位于解析器实际读取的字节流下方，避免核验后重开文件的间隙。
class 核验缓冲 final : public std::streambuf {
public:
    explicit 核验缓冲(const std::filesystem::path& 文件) : 文件_(文件, std::ios::binary) {
        if (!文件_.is_open()) throw std::runtime_error("无法打开词典批次文件");
    }
    void 核验(const 批次文件& 预期) {
        if (!文件_.eof() || 字节数_ != 预期.字节数 || 摘要_.完成() != 预期.摘要)
            throw 批次数据错误("批次文件长度或摘要不符");
    }
protected:
    int_type underflow() override {
        if (gptr() && gptr() < egptr()) return traits_type::to_int_type(*gptr());
        文件_.read(缓冲_.data(), static_cast<std::streamsize>(缓冲_.size()));
        const auto 数 = 文件_.gcount();
        if (文件_.bad() || (文件_.fail() && !文件_.eof()))
            throw std::runtime_error("词典批次读取失败");
        if (数 == 0) return traits_type::eof();
        if (static_cast<std::uint64_t>(数) > std::numeric_limits<std::uint64_t>::max() - 字节数_)
            throw std::length_error("词典文件长度溢出");
        字节数_ += static_cast<std::uint64_t>(数);
        摘要_.追加(缓冲_.data(), static_cast<std::size_t>(数));
        setg(缓冲_.data(), 缓冲_.data(), 缓冲_.data() + 数);
        return traits_type::to_int_type(*gptr());
    }
private:
    std::ifstream 文件_;
    摘要计算 摘要_;
    std::array<char, 65536> 缓冲_{};
    std::uint64_t 字节数_ = 0;
};

class 核验输入 final : public std::istream {
public:
    explicit 核验输入(const std::filesystem::path& 文件) : std::istream(nullptr), 缓冲_(文件) {
        rdbuf(&缓冲_);
        exceptions(std::ios::badbit);
    }
    void 核验(const 批次文件& 预期) { 缓冲_.核验(预期); }
private:
    核验缓冲 缓冲_;
};

bool 是摘要(std::string_view 文本) {
    return 文本.size() == 64 && std::all_of(文本.begin(), 文本.end(), [](char 字) {
        return (字 >= '0' && 字 <= '9') || (字 >= 'a' && 字 <= 'f');
    });
}

bool 文件存在(const std::filesystem::path& 路径) {
    // throwing overload 保留属性故障；只有真正不存在才走旧包回退。
    if (!std::filesystem::exists(路径)) return false;
    if (!std::filesystem::is_regular_file(路径)) throw std::runtime_error("词典路径不是普通文件");
    return true;
}

std::string 读小文件(const std::filesystem::path& 路径, std::size_t 上限) {
    std::ifstream 输入(路径, std::ios::binary);
    if (!输入) throw std::runtime_error("无法读取词典指向或清单");
    std::string 内容(上限 + 1, '\0');
    输入.read(内容.data(), static_cast<std::streamsize>(内容.size()));
    if (输入.bad() || (!输入.eof() && 输入.fail())) throw std::runtime_error("读取词典清单失败");
    内容.resize(static_cast<std::size_t>(输入.gcount()));
    if (内容.size() > 上限) throw 批次数据错误("词典指向或清单超过格式上限");
    return 内容;
}

struct 词典批次 final {
    std::filesystem::path 目录;
    std::string 包标识, 来源摘要;
    std::map<std::string, 批次文件> 文件组;
};

std::optional<词典批次> 读取批次(const std::filesystem::path& 根) {
    const auto 指向 = 根 / std::filesystem::path(u8"当前批次.txt");
    if (!文件存在(指向)) return std::nullopt;
    auto 标识 = 读小文件(指向, 65);
    if (标识.size() != 65 || 标识.back() != '\n' || !是摘要(std::string_view(标识).substr(0, 64)))
        throw 批次数据错误("当前词典批次指向不合法");
    标识.pop_back();
    词典批次 批次;
    批次.目录 = 根 / std::filesystem::path(u8"批次") / 标识;
    const auto 清单路径 = 批次.目录 / std::filesystem::path(u8"批次清单.tsv");
    if (!文件存在(清单路径)) throw 批次数据错误("当前词典批次清单缺失");
    const auto 清单 = 读小文件(清单路径, 16384);
    摘要计算 摘要;
    摘要.追加(清单.data(), 清单.size());
    if (摘要.完成() != 标识) throw 批次数据错误("词典清单与批次标识不符");
    std::vector<std::vector<std::string>> 行组;
    for (std::size_t 起 = 0; 起 < 清单.size();) {
        const auto 终 = 清单.find('\n', 起);
        if (终 == std::string::npos) throw 批次数据错误("批次清单缺末尾换行");
        std::vector<std::string> 列;
        for (auto 位 = 起;;) {
            const auto 隔 = 清单.find('\t', 位);
            const auto 止 = 隔 == std::string::npos || 隔 > 终 ? 终 : 隔;
            列.push_back(清单.substr(位, 止 - 位));
            if (止 == 终) break;
            位 = 止 + 1;
        }
        行组.push_back(std::move(列));
        起 = 终 + 1;
    }
    if (行组.size() != 10 || 行组[0].size() != 4
        || 行组[0][0] != "HY-EGO-DICTIONARY-BATCH" || 行组[0][1] != "1"
        || 行组[0][2].empty() || !是摘要(行组[0][3])
        || 行组[1] != std::vector<std::string>{"OpenCC", "ver.1.4.2"})
        throw 批次数据错误("词典批次清单格式不符");
    批次.包标识 = 行组[0][2];
    批次.来源摘要 = 行组[0][3];
    const std::set<std::string> 表名{"词典.简体.tsv", "繁简表.tsv", "词头表.tsv", "词性表.tsv"};
    const std::set<std::string> 说明名{"来源清单.json", "LICENSE.md", "ATTRIBUTION.md", "OpenCC-LICENSE.txt"};
    for (std::size_t 序 = 2; 序 < 行组.size(); ++序) {
        const auto& 列 = 行组[序];
        if (列.size() != 4 || (!表名.contains(列[0]) && !说明名.contains(列[0])) || !是摘要(列[2]))
            throw 批次数据错误("词典批次文件项不合法");
        批次文件 声明;
        auto 读整数 = [](const std::string& 文本, std::uint64_t& 值) {
            const auto 结果 = std::from_chars(文本.data(), 文本.data() + 文本.size(), 值);
            return !文本.empty() && 结果.ec == std::errc{} && 结果.ptr == 文本.data() + 文本.size();
        };
        if (!读整数(列[1], 声明.字节数) || !声明.字节数 || !读整数(列[3], 声明.数据行数)
            || (表名.contains(列[0]) ? 声明.数据行数 == 0 : 声明.数据行数 != 0))
            throw 批次数据错误("词典批次数量不合法");
        声明.摘要 = 列[2];
        if (!批次.文件组.emplace(列[0], 声明).second) throw 批次数据错误("词典批次文件项重复");
        const auto 路径 = 批次.目录 / std::filesystem::path(std::u8string(
            reinterpret_cast<const char8_t*>(列[0].data()), 列[0].size()));
        if (!文件存在(路径)) throw 批次数据错误("词典批次文件缺失");
        if (说明名.contains(列[0])) {
            核验输入 输入(路径);
            std::array<char, 65536> 缓冲{};
            while (输入.read(缓冲.data(), static_cast<std::streamsize>(缓冲.size()))) {}
            if (!输入.eof()) throw std::runtime_error("词典说明未完整读取");
            输入.核验(声明);
        }
    }
    return 批次;
}

using 词面键 = std::pair<std::string, std::string>;
using 查询键 = std::pair<std::string_view, std::string_view>;

struct 词面键比较 final {
    using is_transparent = void; // 标准库异构查找标记，避免每个前缀复制查询键。

    bool operator()(const 词面键& 左, const 词面键& 右) const noexcept {
        return 查询键{左.first, 左.second} < 查询键{右.first, 右.second};
    }
    bool operator()(const 词面键& 左, const 查询键& 右) const noexcept {
        return 查询键{左.first, 左.second} < 右;
    }
    bool operator()(const 查询键& 左, const 词面键& 右) const noexcept {
        return 左 < 查询键{右.first, 右.second};
    }
};

// 只负责 Unicode 解码，不调用语言领域服务，不修复或规范化来源字节。
bool 读取标量(std::string_view 文本, std::size_t& 位置, std::uint32_t& 码点) noexcept {
    if (位置 >= 文本.size()) return false;
    const auto 首字节 = static_cast<unsigned char>(文本[位置]);
    std::size_t 长度 = 0;
    if (首字节 <= 0x7F) {
        长度 = 1;
        码点 = 首字节;
    } else if (首字节 >= 0xC2 && 首字节 <= 0xDF) {
        长度 = 2;
        码点 = 首字节 & 0x1F;
    } else if (首字节 >= 0xE0 && 首字节 <= 0xEF) {
        长度 = 3;
        码点 = 首字节 & 0x0F;
    } else if (首字节 >= 0xF0 && 首字节 <= 0xF4) {
        长度 = 4;
        码点 = 首字节 & 0x07;
    } else {
        return false;
    }
    if (长度 > 文本.size() - 位置) return false;
    for (std::size_t 偏移 = 1; 偏移 < 长度; ++偏移) {
        const auto 字节 = static_cast<unsigned char>(文本[位置 + 偏移]);
        if ((字节 & 0xC0) != 0x80) return false;
        码点 = (码点 << 6) | (字节 & 0x3F);
    }
    if ((长度 == 2 && 码点 < 0x80)
        || (长度 == 3 && 码点 < 0x800)
        || (长度 == 4 && 码点 < 0x10000)
        || (码点 >= 0xD800 && 码点 <= 0xDFFF)
        || 码点 > 0x10FFFF) {
        return false;
    }
    位置 += 长度;
    return true;
}

bool 字段有效(std::string_view 字段, bool 身份字段) noexcept {
    std::size_t 位置 = 0;
    while (位置 < 字段.size()) {
        std::uint32_t 码点 = 0;
        if (!读取标量(字段, 位置, 码点)
            || 码点 == 0 || 码点 == 0xFEFF || 码点 == 0xFFFD || 码点 == 0x7F) {
            return false;
        }
        if (码点 < 0x20
            && (身份字段 || (码点 != '\t' && 码点 != '\n' && 码点 != '\r'))) {
            return false;
        }
    }
    return true;
}

bool 身份有效(std::string_view 字段) noexcept {
    return !字段.empty() && 字段有效(字段, true);
}

bool 解转义(std::string_view 原字段, std::string& 字段) {
    字段.clear();
    字段.reserve(原字段.size());
    for (std::size_t 位置 = 0; 位置 < 原字段.size(); ++位置) {
        const auto 字节 = static_cast<unsigned char>(原字段[位置]);
        if (字节 < 0x20 || 字节 == 0x7F) return false;
        if (字节 != '\\') {
            字段.push_back(static_cast<char>(字节));
            continue;
        }
        if (++位置 == 原字段.size()) return false;
        switch (原字段[位置]) {
        case '\\': 字段.push_back('\\'); break;
        case 't': 字段.push_back('\t'); break;
        case 'n': 字段.push_back('\n'); break;
        case 'r': 字段.push_back('\r'); break;
        default: return false;
        }
    }
    return true;
}

template<std::size_t 字段数>
bool 拆分行(std::string_view 行, std::array<std::string, 字段数>& 字段组) {
    std::size_t 起点 = 0;
    for (std::size_t 序号 = 0; 序号 < 字段数; ++序号) {
        const auto 终点 = 行.find('\t', 起点);
        if ((序号 + 1 < 字段数 && 终点 == std::string_view::npos)
            || (序号 + 1 == 字段数 && 终点 != std::string_view::npos)) {
            return false;
        }
        const auto 长度 = 终点 == std::string_view::npos ? 行.size() - 起点 : 终点 - 起点;
        if (!解转义(行.substr(起点, 长度), 字段组[序号])) return false;
        if (终点 != std::string_view::npos) 起点 = 终点 + 1;
    }
    return true;
}

template<class 结果类型>
结果类型 查询失败(词典操作状态 状态, std::string_view 包标识, std::string_view 诊断) noexcept {
    结果类型 结果;
    结果.状态 = 状态;
    // 诊断为工程辅助；分配失败也保留原状态，不能从 noexcept 边界泄漏。
    try {
        结果.包标识.assign(包标识);
        结果.诊断.assign(诊断);
    } catch (...) {
    }
    return 结果;
}

词典打开结果 打开失败(词典操作状态 状态, std::string_view 包标识,
    const std::filesystem::path& 文件, std::size_t 行号, std::string_view 原因) noexcept {
    auto 结果 = 查询失败<词典打开结果>(状态, 包标识, {});
    try {
        const auto 路径字节 = 文件.u8string();
        结果.诊断.assign(reinterpret_cast<const char*>(路径字节.data()), 路径字节.size());
        结果.诊断 += "：第";
        结果.诊断 += std::to_string(行号);
        结果.诊断 += "行：";
        结果.诊断.append(原因);
    } catch (...) {
        结果.诊断.clear();
    }
    return 结果;
}

bool 原文边界(std::string_view 原文, std::vector<std::size_t>& 边界组) {
    边界组.push_back(0);
    std::size_t 位置 = 0;
    while (位置 < 原文.size()) {
        std::uint32_t 码点 = 0;
        if (!读取标量(原文, 位置, 码点) || 码点 == 0 || 码点 == 0xFEFF) return false;
        边界组.push_back(位置);
    }
    return true;
}

void 追加标量(std::string& 输出, std::uint32_t 码点) {
    if (码点 < 0x80) 输出.push_back(static_cast<char>(码点));
    else if (码点 < 0x800) {
        输出.push_back(static_cast<char>(0xC0 | (码点 >> 6)));
        输出.push_back(static_cast<char>(0x80 | (码点 & 0x3F)));
    } else if (码点 < 0x10000) {
        输出.push_back(static_cast<char>(0xE0 | (码点 >> 12)));
        输出.push_back(static_cast<char>(0x80 | ((码点 >> 6) & 0x3F)));
        输出.push_back(static_cast<char>(0x80 | (码点 & 0x3F)));
    } else {
        输出.push_back(static_cast<char>(0xF0 | (码点 >> 18)));
        输出.push_back(static_cast<char>(0x80 | ((码点 >> 12) & 0x3F)));
        输出.push_back(static_cast<char>(0x80 | ((码点 >> 6) & 0x3F)));
        输出.push_back(static_cast<char>(0x80 | (码点 & 0x3F)));
    }
}

// 恰好一个合法标量的文本取其码点，否则返回空。
std::optional<std::uint32_t> 唯一标量(std::string_view 文本) noexcept {
    std::size_t 位置 = 0;
    std::uint32_t 码点 = 0;
    if (!读取标量(文本, 位置, 码点) || 位置 != 文本.size()) return std::nullopt;
    return 码点;
}

std::size_t 标量数(std::string_view 文本) noexcept {
    std::size_t 位置 = 0, 数 = 0;
    std::uint32_t 码点 = 0;
    while (位置 < 文本.size() && 读取标量(文本, 位置, 码点)) ++数;
    return 数;
}

struct 释义辅助表 final {
    bool 就绪 = false;
    std::string 诊断;
    std::unordered_map<std::uint32_t, std::uint32_t> 繁简;           // OpenCC 单字（多值已取第一个）
    std::unordered_map<std::string, std::string> 繁简词组;           // OpenCC 词组，繁简等长
    std::size_t 最长词组标量 = 0;
    std::unordered_set<std::string> 规范化词头;
    std::size_t 最长词头标量 = 0;
    std::string 词性来源摘要;
    std::unordered_map<std::string, std::string> 词性; // 记录行号 -> 原始词性
};

// 与 OpenCC t2s 一致：自左向右先取最长词组，否则逐字；词组繁简等长，输出与原文逐标量对齐。
bool 按表规范化(const 释义辅助表& 表, std::string_view 文本, std::string& 输出) {
    输出.clear();
    输出.reserve(文本.size());
    std::vector<std::size_t> 边界{0};
    for (std::size_t 位置 = 0; 位置 < 文本.size();) {
        std::uint32_t 码点 = 0;
        if (!读取标量(文本, 位置, 码点)) return false;
        边界.push_back(位置);
    }
    const auto 总数 = 边界.size() - 1;
    std::string 片;
    for (std::size_t 起 = 0; 起 < 总数;) {
        bool 命中 = false;
        for (std::size_t 长 = std::min(表.最长词组标量, 总数 - 起); 长 >= 2; --长) {
            片.assign(文本.substr(边界[起], 边界[起 + 长] - 边界[起]));
            const auto 词组 = 表.繁简词组.find(片);
            if (词组 != 表.繁简词组.end()) {
                输出 += 词组->second;
                起 += 长;
                命中 = true;
                break;
            }
        }
        if (命中) continue;
        std::size_t 位置 = 边界[起];
        std::uint32_t 码点 = 0;
        (void)读取标量(文本, 位置, 码点);
        const auto 映射 = 表.繁简.find(码点);
        追加标量(输出, 映射 == 表.繁简.end() ? 码点 : 映射->second);
        ++起;
    }
    return true;
}

// 逐行读取辅助表；包头前缀不符、行不合法或读取失败时返回 false 并写诊断。
template<class 行处理>
bool 读辅助表(const std::filesystem::path& 文件, std::string_view 包头前缀,
    std::string& 包头余部, std::string& 诊断, const 批次文件& 预期, 行处理&& 处理) {
    核验输入 输入(文件);
    std::string 行;
    if (!std::getline(输入, 行)) { 诊断 = "缺包头"; return false; }
    if (!行.empty() && 行.back() == '\r') 行.pop_back();
    if (行.rfind(包头前缀, 0) != 0) { 诊断 = "包头不符"; return false; }
    包头余部 = 行.substr(包头前缀.size());
    std::size_t 行号 = 1;
    while (std::getline(输入, 行)) {
        ++行号;
        if (!行.empty() && 行.back() == '\r') 行.pop_back();
        if (!字段有效(行, false) || !处理(std::string_view(行))) {
            诊断 = "第" + std::to_string(行号) + "行不合法";
            return false;
        }
    }
    if (输入.bad() || !输入.eof()) { 诊断 = "读取未完整结束"; return false; }
    输入.核验(预期);
    if (行号 - 1 != 预期.数据行数) throw 批次数据错误("辅助表数据行数与批次清单不符");
    return true;
}

// 读释义辅助表（7250 §3.4）：任一缺失或不合法即整体未就绪，不部分启用。
释义辅助表 载入释义辅助表(const 词典批次& 批次) {
    释义辅助表 表;
    const auto& 目录 = 批次.目录;
    const auto 繁简文件 = 目录 / std::filesystem::path(u8"繁简表.tsv");
    const auto 词头文件 = 目录 / std::filesystem::path(u8"词头表.tsv");
    const auto 词性文件 = 目录 / std::filesystem::path(u8"词性表.tsv");
    std::string 余部;
    std::string 诊断;
    // 繁简表：P 行为词组（繁简等长、至少两字），C 行为单字；来自 OpenCC，版本在包头。
    if (!读辅助表(繁简文件, "HY-EGO-TRAD-SIMP\t2\t", 余部, 诊断, 批次.文件组.at("繁简表.tsv"), [&](std::string_view 行) {
            if (行.size() < 2 || 行[1] != '\t') return false;
            const auto 种类 = 行[0];
            const auto 余 = 行.substr(2);
            const auto 制表 = 余.find('\t');
            if (制表 == std::string_view::npos) return false;
            const auto 繁 = 余.substr(0, 制表);
            const auto 简 = 余.substr(制表 + 1);
            if (种类 == 'C') {
                const auto 甲 = 唯一标量(繁);
                const auto 乙 = 唯一标量(简);
                return 甲 && 乙 && 表.繁简.emplace(*甲, *乙).second;
            }
            const auto 长 = 标量数(繁);
            if (种类 != 'P' || 长 < 2 || 标量数(简) != 长 || !字段有效(简, true)) return false;
            表.最长词组标量 = std::max(表.最长词组标量, 长);
            return 表.繁简词组.emplace(std::string(繁), std::string(简)).second;
        }) || 余部 != "opencc-1.4.2") {
        表.诊断 = "繁简表：" + (诊断.empty() ? std::string("包头缺版本") : 诊断);
        表.繁简.clear();
        表.繁简词组.clear();
        return 表;
    }
    std::string 规范;
    // 词头表在构建时已转简体；载入时仍按繁简表规范化一次，残留繁体与运行时规范化口径一致。
    if (!读辅助表(词头文件, "HY-EGO-HEADWORDS\t2\tzh-s", 余部, 诊断, 批次.文件组.at("词头表.tsv"), [&](std::string_view 行) {
            if (行.empty() || 行.find('\t') != std::string_view::npos || !按表规范化(表, 行, 规范))
                return false;
            表.最长词头标量 = std::max(表.最长词头标量, 标量数(规范));
            表.规范化词头.insert(规范);
            return true;
        }) || !余部.empty()) {
        表.诊断 = "词头表：" + (诊断.empty() ? std::string("包头不符") : 诊断);
        return 表;
    }
    if (!读辅助表(词性文件, "HY-EGO-POS\t1\t", 余部, 诊断, 批次.文件组.at("词性表.tsv"), [&](std::string_view 行) {
            const auto 制表 = 行.find('\t');
            if (制表 == 0 || 制表 == std::string_view::npos || 制表 + 1 == 行.size()) return false;
            return 表.词性.emplace(std::string(行.substr(0, 制表)), std::string(行.substr(制表 + 1))).second;
        }) || 余部 != 批次.来源摘要) {
        表.诊断 = "词性表：" + (诊断.empty() ? std::string("来源摘要与批次不符") : 诊断);
        return 表;
    }
    表.词性来源摘要 = std::move(余部);
    表.就绪 = true;
    return 表;
}

} // namespace

struct 离线词典数据源::实现 final {
    std::string 包标识;
    std::map<词面键, std::vector<词典义项材料>, 词面键比较> 索引;
    释义辅助表 辅助;
};

离线词典数据源::离线词典数据源(std::shared_ptr<const 实现> 内容)
    : 实现_(std::move(内容)) {
}

词典打开结果 离线词典数据源::打开(const std::filesystem::path& 目录) noexcept {
    std::filesystem::path 文件;
    std::string 包标识;
    std::size_t 行号 = 0;
    try {
        if (目录.empty()) {
            return 打开失败(词典操作状态::输入不合法, {}, 目录, 0, "词典目录为空");
        }
        std::error_code 错误;
        const bool 目录存在 = std::filesystem::exists(目录, 错误);
        if (错误) return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "无法读取目录属性");
        if (!目录存在) return 打开失败(词典操作状态::未配置, {}, 文件, 0, "词典目录不存在");
        const bool 是目录 = std::filesystem::is_directory(目录, 错误);
        if (错误 || !是目录) {
            return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "指定路径不是可读目录");
        }
        const auto 批次 = 读取批次(目录);
        文件 = (批次 ? 批次->目录 : 目录) / std::filesystem::path(u8"词典.简体.tsv");
        if (!批次 && !文件存在(文件)) 文件 = 目录 / std::filesystem::path(u8"词典.tsv");
        const bool 文件存在 = std::filesystem::exists(文件, 错误);
        if (错误) return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "无法读取文件属性");
        if (!文件存在) return 打开失败(词典操作状态::未配置, {}, 文件, 0, "词典文件不存在");
        const bool 普通文件 = std::filesystem::is_regular_file(文件, 错误);
        if (错误 || !普通文件) {
            return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "词典不是普通可读文件");
        }
        核验输入 输入(文件);

        std::string 行;
        if (!std::getline(输入, 行)) {
            return 打开失败(输入.bad() || !输入.eof() ? 词典操作状态::读取失败 : 词典操作状态::数据错误,
                {}, 文件, 1, "无法读取包头");
        }
        行号 = 1;
        if (!行.empty() && 行.back() == '\r') 行.pop_back();
        std::array<std::string, 4> 包头;
        if (!拆分行(行, 包头)
            || 包头[0] != "HY-EGO-DICTIONARY" || 包头[1] != "1" || 包头[2] != "zh"
            || !身份有效(包头[3])) {
            return 打开失败(词典操作状态::数据错误, {}, 文件, 行号, "包头格式、版本或包标识不合法");
        }
        包标识 = std::move(包头[3]);
        if (批次 && 包标识 != 批次->包标识)
            throw 批次数据错误("词典包标识与批次清单不符");
        auto 候选 = std::make_shared<实现>();
        候选->包标识 = 包标识;
        std::set<词面键> 义项唯一键;
        std::size_t 义项数 = 0;
        while (std::getline(输入, 行)) {
            if (行号 == std::numeric_limits<std::size_t>::max()) {
                throw std::length_error("词典行号不可表示");
            }
            ++行号;
            if (!行.empty() && 行.back() == '\r') 行.pop_back();
            std::array<std::string, 13> 字段组;
            if (!拆分行(行, 字段组)) {
                return 打开失败(词典操作状态::数据错误, 包标识, 文件, 行号, "必须为13列且仅使用四种合法转义");
            }
            for (std::size_t 序号 = 0; 序号 < 字段组.size(); ++序号) {
                const bool 必填 = 序号 < 5 || 序号 >= 8;
                if ((必填 && 字段组[序号].empty()) || !字段有效(字段组[序号], 序号 < 4)) {
                    return 打开失败(词典操作状态::数据错误, 包标识, 文件, 行号, "必填字段为空或字段编码、字符不合法");
                }
            }
            if (批次 && !字段组[0].starts_with(批次->来源摘要 + ":"))
                throw 批次数据错误("词典记录来源与批次不符");
            if (!义项唯一键.emplace(字段组[0], 字段组[3]).second) {
                return 打开失败(词典操作状态::数据错误, 包标识, 文件, 行号, "记录键与义项键重复");
            }
            词面键 键{字段组[1], 字段组[2]};
            词典义项材料 材料{
                std::move(字段组[0]), std::move(字段组[1]), std::move(字段组[2]),
                std::move(字段组[3]), std::move(字段组[4]), std::move(字段组[5]),
                std::move(字段组[6]), std::move(字段组[7]), std::move(字段组[8]),
                std::move(字段组[9]), std::move(字段组[10]), std::move(字段组[11]),
                std::move(字段组[12])};
            候选->索引[std::move(键)].push_back(std::move(材料));
            if (义项数 == std::numeric_limits<std::size_t>::max()) {
                throw std::length_error("词典义项数不可表示");
            }
            ++义项数;
        }
        if (输入.bad() || !输入.eof()) {
            return 打开失败(词典操作状态::读取失败, 包标识, 文件, 行号, "读取词典未完整结束");
        }
        if (批次) {
            const auto& 声明 = 批次->文件组.at("词典.简体.tsv");
            输入.核验(声明);
            if (义项数 != 声明.数据行数) throw 批次数据错误("词典义项数与批次清单不符");
        }
        for (auto& [键, 义项组] : 候选->索引) {
            (void)键;
            std::sort(义项组.begin(), 义项组.end(), [](const auto& 左, const auto& 右) {
                return 查询键{左.记录键, 左.义项键} < 查询键{右.记录键, 右.义项键};
            });
        }
        if (批次) {
            候选->辅助 = 载入释义辅助表(*批次);
            if (!候选->辅助.就绪) throw 批次数据错误(候选->辅助.诊断);
        } else {
            候选->辅助.诊断 = "须构建同批词典包；散文件仅提供只读查询，不启用释义辅助表";
        }
        词典打开结果 结果;
        结果.包标识 = 包标识;
        结果.数据源 = std::shared_ptr<const 离线词典数据源>(new 离线词典数据源(候选));
        结果.词面数 = 候选->索引.size();
        结果.义项数 = 义项数;
        结果.状态 = 词典操作状态::已完成;
        return 结果;
    } catch (const 批次数据错误& 错误) {
        return 打开失败(词典操作状态::数据错误, 包标识, 文件, 行号, 错误.what());
    } catch (const std::bad_alloc&) {
        return 打开失败(词典操作状态::资源失败, 包标识, 文件, 行号, "词典打开内存分配失败");
    } catch (const std::length_error&) {
        return 打开失败(词典操作状态::资源失败, 包标识, 文件, 行号, "词典打开容量不可表示");
    } catch (...) {
        return 打开失败(词典操作状态::读取失败, 包标识, 文件, 行号, "词典打开发生工程异常");
    }
}

词典精确查询结果 离线词典数据源::精确查询(const 词典精确查询请求& 请求) const noexcept {
    try {
        if (!身份有效(请求.词头语种) || !身份有效(请求.词面)) {
            return 查询失败<词典精确查询结果>(词典操作状态::输入不合法, 实现_->包标识,
                "精确查询的词头语种或词面不合法");
        }
        const auto 位置 = 实现_->索引.find(查询键{请求.词头语种, 请求.词面});
        词典精确查询结果 结果;
        结果.包标识 = 实现_->包标识;
        if (位置 == 实现_->索引.end()) {
            结果.状态 = 词典操作状态::未命中;
        } else {
            结果.义项组 = 位置->second;
            结果.状态 = 词典操作状态::已完成;
        }
        return 结果;
    } catch (const std::bad_alloc&) {
        return 查询失败<词典精确查询结果>(词典操作状态::资源失败, 实现_->包标识, "精确查询内存分配失败");
    } catch (const std::length_error&) {
        return 查询失败<词典精确查询结果>(词典操作状态::资源失败, 实现_->包标识, "精确查询容量不可表示");
    } catch (...) {
        return 查询失败<词典精确查询结果>(词典操作状态::读取失败, 实现_->包标识, "精确查询发生工程异常");
    }
}

词典前缀查询结果 离线词典数据源::查询词面前缀(const 词典前缀查询请求& 请求) const noexcept {
    try {
        std::vector<std::size_t> 边界组;
        if (!身份有效(请求.词头语种) || !原文边界(请求.原文, 边界组)
            || 请求.起始标量 >= 边界组.size()) {
            return 查询失败<词典前缀查询结果>(词典操作状态::输入不合法, 实现_->包标识,
                "前缀查询的语种、原文编码或标量起点不合法");
        }
        词典前缀查询结果 结果;
        结果.包标识 = 实现_->包标识;
        const std::string_view 原文 = 请求.原文;
        const auto 起始字节 = 边界组[请求.起始标量];
        for (std::size_t 结束标量 = 请求.起始标量 + 1; 结束标量 < 边界组.size(); ++结束标量) {
            const auto 前缀 = 原文.substr(起始字节, 边界组[结束标量] - 起始字节);
            const auto 位置 = 实现_->索引.find(查询键{请求.词头语种, 前缀});
            if (位置 != 实现_->索引.end()) {
                结果.命中组.push_back(词典前缀命中{请求.起始标量, 结束标量, 位置->second});
            }
        }
        结果.状态 = 结果.命中组.empty() ? 词典操作状态::未命中 : 词典操作状态::已完成;
        return 结果;
    } catch (const std::bad_alloc&) {
        return 查询失败<词典前缀查询结果>(词典操作状态::资源失败, 实现_->包标识, "前缀查询内存分配失败");
    } catch (const std::length_error&) {
        return 查询失败<词典前缀查询结果>(词典操作状态::资源失败, 实现_->包标识, "前缀查询容量不可表示");
    } catch (...) {
        return 查询失败<词典前缀查询结果>(词典操作状态::读取失败, 实现_->包标识, "前缀查询发生工程异常");
    }
}

bool 离线词典数据源::释义辅助表就绪() const noexcept {
    return 实现_->辅助.就绪;
}

std::string 离线词典数据源::释义辅助表诊断() const {
    return 实现_->辅助.诊断;
}

std::optional<std::string> 离线词典数据源::繁简规范化(std::string_view 原文) const {
    std::string 输出;
    if (!实现_->辅助.就绪 || !按表规范化(实现_->辅助, 原文, 输出)) return std::nullopt;
    return 输出;
}

bool 离线词典数据源::是规范化词头(std::string_view 规范化词面) const noexcept {
    try {
        const auto& 表 = 实现_->辅助;
        return 表.就绪 && 表.规范化词头.count(std::string(规范化词面)) != 0;
    } catch (...) {
        return false;
    }
}

std::size_t 离线词典数据源::最长词头标量数() const noexcept {
    return 实现_->辅助.就绪 ? 实现_->辅助.最长词头标量 : 0;
}

std::optional<std::string> 离线词典数据源::读取记录词性(std::string_view 记录键) const {
    const auto& 表 = 实现_->辅助;
    const auto 分隔 = 记录键.rfind(':');
    if (!表.就绪 || 分隔 == std::string_view::npos || 记录键.substr(0, 分隔) != 表.词性来源摘要)
        return std::nullopt;
    const auto 位置 = 表.词性.find(std::string(记录键.substr(分隔 + 1)));
    if (位置 == 表.词性.end()) return std::nullopt;
    return 位置->second;
}

} // namespace 海中鱼巣
