#include "词典.离线数据源.h"

#include <algorithm>
#include <array>
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

namespace 海中鱼巣 {
namespace {

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
    std::string& 包头余部, std::string& 诊断, 行处理&& 处理) {
    std::ifstream 输入(文件, std::ios::binary);
    if (!输入.is_open()) { 诊断 = "无法打开"; return false; }
    输入.exceptions(std::ios::badbit);
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
    return true;
}

// 读释义辅助表（7250 §3.4）：任一缺失或不合法即整体未就绪，不部分启用。
释义辅助表 载入释义辅助表(const std::filesystem::path& 目录) {
    释义辅助表 表;
    std::error_code 错误;
    const auto 繁简文件 = 目录 / std::filesystem::path(u8"繁简表.tsv");
    const auto 词头文件 = 目录 / std::filesystem::path(u8"词头表.tsv");
    const auto 词性文件 = 目录 / std::filesystem::path(u8"词性表.tsv");
    for (const auto* 文件 : {&繁简文件, &词头文件, &词性文件}) {
        if (!std::filesystem::is_regular_file(*文件, 错误) || 错误) {
            表.诊断 = "释义辅助表未构建（词头表、繁简表、词性表须由 工具/词典/构建词典辅助表.py 生成）";
            return 表;
        }
    }
    std::string 余部;
    std::string 诊断;
    // 繁简表：P 行为词组（繁简等长、至少两字），C 行为单字；来自 OpenCC，版本在包头。
    if (!读辅助表(繁简文件, "HY-EGO-TRAD-SIMP\t2\t", 余部, 诊断, [&](std::string_view 行) {
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
        }) || 余部.empty()) {
        表.诊断 = "繁简表：" + (诊断.empty() ? std::string("包头缺版本") : 诊断);
        表.繁简.clear();
        表.繁简词组.clear();
        return 表;
    }
    std::string 规范;
    // 词头表在构建时已转简体；载入时仍按繁简表规范化一次，残留繁体与运行时规范化口径一致。
    if (!读辅助表(词头文件, "HY-EGO-HEADWORDS\t2\tzh-s", 余部, 诊断, [&](std::string_view 行) {
            if (行.empty() || 行.find('\t') != std::string_view::npos || !按表规范化(表, 行, 规范))
                return false;
            表.最长词头标量 = std::max(表.最长词头标量, 标量数(规范));
            表.规范化词头.insert(规范);
            return true;
        }) || !余部.empty()) {
        表.诊断 = "词头表：" + (诊断.empty() ? std::string("包头不符") : 诊断);
        return 表;
    }
    if (!读辅助表(词性文件, "HY-EGO-POS\t1\t", 余部, 诊断, [&](std::string_view 行) {
            const auto 制表 = 行.find('\t');
            if (制表 == 0 || 制表 == std::string_view::npos || 制表 + 1 == 行.size()) return false;
            return 表.词性.emplace(std::string(行.substr(0, 制表)), std::string(行.substr(制表 + 1))).second;
        }) || 余部.empty()) {
        表.诊断 = "词性表：" + (诊断.empty() ? std::string("缺来源摘要") : 诊断);
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
        // 优先使用整体转为简体的包（2026-11-13 用户裁决），缺失时退回原始繁体为主的包。
        std::error_code 错误;
        文件 = 目录 / std::filesystem::path(u8"词典.简体.tsv");
        if (!std::filesystem::is_regular_file(文件, 错误) || 错误) {
            错误.clear();
            文件 = 目录 / std::filesystem::path(u8"词典.tsv");
        }
        const bool 目录存在 = std::filesystem::exists(目录, 错误);
        if (错误) return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "无法读取目录属性");
        if (!目录存在) return 打开失败(词典操作状态::未配置, {}, 文件, 0, "词典目录不存在");
        const bool 是目录 = std::filesystem::is_directory(目录, 错误);
        if (错误 || !是目录) {
            return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "指定路径不是可读目录");
        }
        const bool 文件存在 = std::filesystem::exists(文件, 错误);
        if (错误) return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "无法读取文件属性");
        if (!文件存在) return 打开失败(词典操作状态::未配置, {}, 文件, 0, "词典文件不存在");
        const bool 普通文件 = std::filesystem::is_regular_file(文件, 错误);
        if (错误 || !普通文件) {
            return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "词典不是普通可读文件");
        }
        std::ifstream 输入(文件, std::ios::binary);
        if (!输入.is_open()) return 打开失败(词典操作状态::读取失败, {}, 文件, 0, "无法打开词典文件");
        // 让 getline 的资源异常继续上送，不能被流状态吞成普通读取失败。
        输入.exceptions(std::ios::badbit);

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
        for (auto& [键, 义项组] : 候选->索引) {
            (void)键;
            std::sort(义项组.begin(), 义项组.end(), [](const auto& 左, const auto& 右) {
                return 查询键{左.记录键, 左.义项键} < 查询键{右.记录键, 右.义项键};
            });
        }
        // 辅助表只服务读释义；未构建或不合法不令词典打开失败，由读释义按诊断报告。
        候选->辅助 = 载入释义辅助表(目录);
        词典打开结果 结果;
        结果.包标识 = 包标识;
        结果.数据源 = std::shared_ptr<const 离线词典数据源>(new 离线词典数据源(候选));
        结果.词面数 = 候选->索引.size();
        结果.义项数 = 义项数;
        结果.状态 = 词典操作状态::已完成;
        return 结果;
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
