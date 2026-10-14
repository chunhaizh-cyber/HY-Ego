#include "解释.初始化最小结构.h"
#include "../领域/新_语言类.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <utility>

namespace 海中鱼巣 {
namespace {

struct 词视图 final {
    std::string_view 词面;
    初始化源范围 来源;
};
struct 行视图 final {
    初始化源范围 来源;
    std::vector<词视图> 词组;
};
struct 模板结构项 final {
    std::string_view 固定词;
    std::optional<初始化源角色> 角色;
};
struct 最小模板定义 final {
    初始化最小模板 模板;
    std::size_t 词数;
    std::array<模板结构项, 6> 结构;
};

constexpr 模板结构项 固定词(std::string_view 词面) noexcept {
    return {词面, std::nullopt};
}
constexpr 模板结构项 角色项(初始化源角色 角色) noexcept {
    return {{}, 角色};
}
constexpr std::array<最小模板定义, 5> 最小结构表{{
    {初始化最小模板::概念归类, 4, {
        角色项(初始化源角色::下位概念), 固定词("是"), 固定词("一种"),
        角色项(初始化源角色::上位概念)}},
    {初始化最小模板::特征整数, 6, {
        角色项(初始化源角色::主体概念), 固定词("的"),
        角色项(初始化源角色::特征类型), 固定词("是"), 固定词("整数"),
        角色项(初始化源角色::整数值)}},
    {初始化最小模板::特征概念, 6, {
        角色项(初始化源角色::主体概念), 固定词("的"),
        角色项(初始化源角色::特征类型), 固定词("是"), 固定词("概念"),
        角色项(初始化源角色::特征值概念)}},
    {初始化最小模板::组成定义, 5, {
        角色项(初始化源角色::部分概念), 固定词("是"),
        角色项(初始化源角色::整体概念), 固定词("的"), 固定词("一部分")}},
    {初始化最小模板::显式别名, 3, {
        角色项(初始化源角色::主体概念), 固定词("又称"),
        角色项(初始化源角色::别名词面)}}
}};

// 仅用于公开严格校验已经拒绝的文本，定位首个失败序列；不规范化或分词。
std::size_t 定位UTF8失败字节(const std::string& 原文) noexcept {
    std::size_t 位置 = 0;
    while (位置 < 原文.size()) {
        const auto 首 = static_cast<unsigned char>(原文[位置]);
        std::size_t 长度 = 0;
        std::uint32_t 标量 = 0;
        if (首 <= 0x7F) { 长度 = 1; 标量 = 首; }
        else if (首 >= 0xC2 && 首 <= 0xDF) { 长度 = 2; 标量 = 首 & 0x1F; }
        else if (首 >= 0xE0 && 首 <= 0xEF) { 长度 = 3; 标量 = 首 & 0x0F; }
        else if (首 >= 0xF0 && 首 <= 0xF4) { 长度 = 4; 标量 = 首 & 0x07; }
        else return 位置;
        if (长度 > 原文.size() - 位置) return 位置;
        for (std::size_t i = 1; i < 长度; ++i) {
            const auto 后续 = static_cast<unsigned char>(原文[位置 + i]);
            if ((后续 & 0xC0) != 0x80) return 位置;
            标量 = (标量 << 6) | (后续 & 0x3F);
        }
        if ((长度 == 2 && 标量 < 0x80) || (长度 == 3 && 标量 < 0x800)
            || (长度 == 4 && 标量 < 0x10000)
            || (标量 >= 0xD800 && 标量 <= 0xDFFF) || 标量 > 0x10FFFF
            || 标量 == 0xFEFF || 标量 == 0xFFFD) return 位置;
        位置 += 长度;
    }
    return 原文.size();
}

// 只在已校验文本的非空剩余区间调用，单调推进一个完整Unicode标量。
std::uint32_t 读取已校验标量(
    const std::string& 原文, std::size_t& 字节位置) noexcept {
    const auto 首 = static_cast<unsigned char>(原文[字节位置++]);
    if (首 <= 0x7F) return 首;
    const std::size_t 后续数 = 首 <= 0xDF ? 1 : (首 <= 0xEF ? 2 : 3);
    std::uint32_t 标量 = 首 & (后续数 == 1 ? 0x1F : (后续数 == 2 ? 0x0F : 0x07));
    for (std::size_t i = 0; i < 后续数; ++i)
        标量 = (标量 << 6) | (static_cast<unsigned char>(原文[字节位置++]) & 0x3F);
    return 标量;
}

bool 非标准空白或控制(std::uint32_t 标量) noexcept {
    return (标量 < 0x20 && 标量 != '\n' && 标量 != '\r')
        || (标量 >= 0x7F && 标量 <= 0x9F)
        || 标量 == 0xA0 || 标量 == 0x1680
        || (标量 >= 0x2000 && 标量 <= 0x200A)
        || 标量 == 0x2028 || 标量 == 0x2029 || 标量 == 0x202F
        || 标量 == 0x205F || 标量 == 0x3000;
}
bool 不支持标点(std::uint32_t 标量) noexcept {
    switch (标量) {
    case '.': case ',': case ';': case ':': case '!': case '?':
    case '"': case '\'': case '\\':
    case 0x3002: case 0xFF0C: case 0xFF1B: case 0xFF1A: case 0xFF01: case 0xFF1F:
    case 0x2018: case 0x2019: case 0x201C: case 0x201D:
    case 0x300C: case 0x300D: case 0x300E: case 0x300F:
        return true;
    default: return false;
    }
}

struct 整数解析结果 final {
    std::optional<std::int64_t> 值;
    std::optional<初始化诊断原因> 拒绝原因;
};
整数解析结果 解析标准I64(std::string_view 词面) noexcept {
    const auto 不合法 = []() {
        return 整数解析结果{std::nullopt, 初始化诊断原因::整数字面量不合法};
    };
    if (词面.empty()) return 不合法();
    const std::size_t 首位 = 词面.front() == '-' ? 1 : 0;
    if (首位 == 词面.size()) return 不合法();
    if (词面[首位] == '0' && (首位 != 0 || 词面.size() != 1)) return 不合法();
    for (std::size_t i = 首位; i < 词面.size(); ++i)
        if (词面[i] < '0' || 词面[i] > '9') return 不合法();
    std::int64_t 值 = 0;
    const auto [终点, 错误] = std::from_chars(词面.data(), 词面.data() + 词面.size(), 值);
    if (错误 == std::errc::result_out_of_range)
        return {std::nullopt, 初始化诊断原因::整数越界};
    if (错误 != std::errc{} || 终点 != 词面.data() + 词面.size()) return 不合法();
    return {值, std::nullopt};
}

初始化解释诊断 范围诊断(
    初始化诊断原因 原因, const 初始化源范围& 范围) {
    return {原因, 范围.行号, 范围.字节起点, 范围.字节终点,
        范围.标量起点, 范围.标量终点, std::nullopt};
}
初始化最小解释结果 文本拒绝(初始化解释状态 状态,
    初始化诊断原因 原因, const 初始化源范围& 范围) {
    return {初始化工程控制::已完成, 状态, std::nullopt, 范围诊断(原因, 范围)};
}
初始化最小解释结果 资源拒绝(
    初始化额度种类 种类, 初始化解释诊断 诊断) {
    诊断.额度种类 = 种类;
    return {初始化工程控制::资源未完成, std::nullopt, std::nullopt, std::move(诊断)};
}

bool 模板完整匹配(const 最小模板定义& 模板, const 行视图& 行) noexcept {
    if (行.词组.size() != 模板.词数) return false;
    for (std::size_t i = 0; i < 模板.词数; ++i)
        if (!模板.结构[i].角色 && 行.词组[i].词面 != 模板.结构[i].固定词) return false;
    return true;
}

void 核验完整材料(const 初始化定义材料& 材料, std::size_t 标量数) {
    const auto 范围有效 = [&](const 初始化源范围& 范围) {
        return 范围.行号 != 0 && 范围.字节起点 < 范围.字节终点
            && 范围.字节终点 <= 材料.原文.size()
            && 范围.标量起点 < 范围.标量终点 && 范围.标量终点 <= 标量数;
    };
    for (std::size_t i = 0; i < 材料.符号组.size(); ++i) {
        const auto& 符号 = 材料.符号组[i];
        if (符号.引用.局部编号 != i || !范围有效(符号.来源)
            || std::string_view(材料.原文).substr(符号.来源.字节起点,
                符号.来源.字节终点 - 符号.来源.字节起点) != 符号.词面)
            throw std::logic_error("initialization symbol/source invariant");
    }
    const auto 核验引用 = [&](初始化符号引用 引用, 初始化符号角色类型 类型) {
        if (引用.局部编号 >= 材料.符号组.size()
            || 材料.符号组[引用.局部编号].角色类型 != 类型)
            throw std::logic_error("initialization symbol type invariant");
    };
    for (std::size_t i = 0; i < 材料.定义组.size(); ++i) {
        const auto& 项 = 材料.定义组[i];
        if (项.局部编号 != i || !范围有效(项.来源)
            || 项.作用域 != 初始化定义作用域::概念定义声明)
            throw std::logic_error("initialization definition/source invariant");
        const 最小模板定义* 模板 = nullptr;
        for (const auto& 定义 : 最小结构表)
            if (定义.模板 == 项.模板) 模板 = &定义;
        if (!模板) throw std::logic_error("initialization template identity invariant");
        std::size_t 角色数量 = 0;
        for (std::size_t 词序 = 0; 词序 < 模板->词数; ++词序) {
            const auto& 角色 = 模板->结构[词序].角色;
            if (!角色) continue;
            if (角色数量 == 项.角色来源组.size()
                || 项.角色来源组[角色数量].角色 != *角色
                || 项.角色来源组[角色数量].行内词序 != 词序)
                throw std::logic_error("initialization role/template invariant");
            ++角色数量;
        }
        if (角色数量 != 项.角色来源组.size())
            throw std::logic_error("initialization source role arity invariant");
        for (const auto& 角色 : 项.角色来源组)
            if (!范围有效(角色.来源) || 角色.来源.行号 != 项.来源.行号
                || 角色.来源.字节起点 < 项.来源.字节起点
                || 角色.来源.字节终点 > 项.来源.字节终点
                || 角色.来源.标量起点 < 项.来源.标量起点
                || 角色.来源.标量终点 > 项.来源.标量终点)
                throw std::logic_error("initialization role/source invariant");
        switch (项.模板) {
        case 初始化最小模板::概念归类: {
            const auto& 值 = std::get<初始化概念归类材料>(项.载荷);
            核验引用(值.下位, 初始化符号角色类型::概念引用);
            核验引用(值.上位, 初始化符号角色类型::概念引用);
            break;
        }
        case 初始化最小模板::特征整数:
        case 初始化最小模板::特征概念: {
            const auto& 值 = std::get<初始化特征定义材料>(项.载荷);
            核验引用(值.主体概念, 初始化符号角色类型::概念引用);
            核验引用(值.特征类型, 初始化符号角色类型::特征类型引用);
            if (项.模板 == 初始化最小模板::特征概念)
                核验引用(std::get<初始化符号引用>(值.值), 初始化符号角色类型::概念引用);
            else (void)std::get<std::int64_t>(值.值);
            break;
        }
        case 初始化最小模板::组成定义: {
            const auto& 值 = std::get<初始化组成定义材料>(项.载荷);
            核验引用(值.部分, 初始化符号角色类型::概念引用);
            核验引用(值.整体, 初始化符号角色类型::概念引用);
            break;
        }
        case 初始化最小模板::显式别名: {
            const auto& 值 = std::get<初始化名称提议材料>(项.载荷);
            核验引用(值.概念, 初始化符号角色类型::概念引用);
            if (值.别名词面.empty()) throw std::logic_error("initialization alias invariant");
            break;
        }
        default: throw std::logic_error("initialization template invariant");
        }
    }
}

初始化最小解释结果 解释标准文本内部(
    const std::string& 原文, const 初始化最小资源额度& 额度) {
    const std::array<std::pair<初始化额度种类, std::size_t>, 7> 配置{{
        {初始化额度种类::源字节, 额度.最大源字节数},
        {初始化额度种类::源标量, 额度.最大源标量数},
        {初始化额度种类::非空行, 额度.最大非空行数},
        {初始化额度种类::单行词, 额度.最大单行词数},
        {初始化额度种类::单词字节, 额度.最大单词字节数},
        {初始化额度种类::单词标量, 额度.最大单词标量数},
        {初始化额度种类::角色符号, 额度.最大角色符号数}
    }};
    for (const auto& [种类, 数量] : 配置)
        if (数量 == 0) {
            初始化解释诊断 诊断{初始化诊断原因::额度配置无效};
            诊断.额度种类 = 种类;
            return {初始化工程控制::额度配置无效, std::nullopt,
                std::nullopt, std::move(诊断)};
        }
    if (原文.size() > 额度.最大源字节数) {
        初始化解释诊断 诊断{初始化诊断原因::额度到限};
        诊断.字节终点 = 原文.size();
        return 资源拒绝(初始化额度种类::源字节, std::move(诊断));
    }
    if (原文.empty())
        return 文本拒绝(初始化解释状态::空材料, 初始化诊断原因::空材料, {});
    if (!新_语言类::严格UTF8字节串有效(原文)) {
        const auto 位置 = 定位UTF8失败字节(原文);
        if (位置 == 原文.size()) throw std::logic_error("UTF8 validator/locator disagreement");
        初始化解释诊断 诊断{初始化诊断原因::UTF8不合法};
        诊断.字节起点 = 位置;
        诊断.字节终点 = 位置 + 1;
        诊断.行号 = 1 + static_cast<std::size_t>(
            std::count(原文.begin(), 原文.begin() + 位置, '\n'));
        return {初始化工程控制::已完成, 初始化解释状态::格式失败,
            std::nullopt, std::move(诊断)};
    }

    std::vector<行视图> 行组;
    行视图 当前行;
    std::size_t 字节位置 = 0, 标量位置 = 0;
    std::optional<初始化源范围> 当前词;
    const auto 结束词 = [&](std::size_t 字节终点, std::size_t 标量终点)
        -> std::optional<初始化最小解释结果> {
        if (!当前词) return std::nullopt;
        auto 范围 = *当前词;
        范围.字节终点 = 字节终点;
        范围.标量终点 = 标量终点;
        if (当前行.词组.size() == 额度.最大单行词数)
            return 资源拒绝(初始化额度种类::单行词,
                范围诊断(初始化诊断原因::额度到限, 范围));
        当前行.词组.push_back({std::string_view(原文).substr(范围.字节起点,
            范围.字节终点 - 范围.字节起点), 范围});
        当前词.reset();
        return std::nullopt;
    };
    const auto 结束行 = [&](std::size_t 字节终点, std::size_t 标量终点)
        -> std::optional<初始化最小解释结果> {
        if (!当前词 && !当前行.词组.empty())
            return 文本拒绝(初始化解释状态::格式失败, 初始化诊断原因::分隔不合法,
                {当前行.来源.行号, 字节终点 - 1, 字节终点, 标量终点 - 1, 标量终点});
        if (auto 失败 = 结束词(字节终点, 标量终点)) return 失败;
        当前行.来源.字节终点 = 字节终点;
        当前行.来源.标量终点 = 标量终点;
        if (!当前行.词组.empty()) {
            if (行组.size() == 额度.最大非空行数)
                return 资源拒绝(初始化额度种类::非空行,
                    范围诊断(初始化诊断原因::额度到限, 当前行.来源));
            行组.push_back(std::move(当前行));
        }
        return std::nullopt;
    };

    while (字节位置 < 原文.size()) {
        const auto 字节起点 = 字节位置, 标量起点 = 标量位置;
        const auto 标量 = 读取已校验标量(原文, 字节位置);
        const 初始化源范围 单项范围{当前行.来源.行号,
            字节起点, 字节位置, 标量起点, 标量起点 + 1};
        if (标量位置 == 额度.最大源标量数)
            return 资源拒绝(初始化额度种类::源标量,
                范围诊断(初始化诊断原因::额度到限, 单项范围));
        ++标量位置;
        if (非标准空白或控制(标量))
            return 文本拒绝(初始化解释状态::格式失败,
                初始化诊断原因::分隔不合法, 单项范围);
        if (不支持标点(标量))
            return 文本拒绝(初始化解释状态::格式失败,
                初始化诊断原因::不支持标点, 单项范围);
        if (标量 == '\r' || 标量 == '\n') {
            if (标量 == '\r') {
                if (字节位置 == 原文.size() || 原文[字节位置] != '\n')
                    return 文本拒绝(初始化解释状态::格式失败,
                        初始化诊断原因::分隔不合法, 单项范围);
                if (标量位置 == 额度.最大源标量数)
                    return 资源拒绝(初始化额度种类::源标量,
                        范围诊断(初始化诊断原因::额度到限,
                            {当前行.来源.行号, 字节位置, 字节位置 + 1,
                                标量位置, 标量位置 + 1}));
                ++字节位置;
                ++标量位置;
            }
            const auto 下一行号 = 当前行.来源.行号 + 1;
            if (auto 失败 = 结束行(字节起点, 标量起点)) return std::move(*失败);
            当前行 = {{下一行号, 字节位置, 字节位置, 标量位置, 标量位置}, {}};
        } else if (标量 == ' ') {
            if (!当前词)
                return 文本拒绝(初始化解释状态::格式失败,
                    初始化诊断原因::分隔不合法, 单项范围);
            if (auto 失败 = 结束词(字节起点, 标量起点)) return std::move(*失败);
        } else {
            if (!当前词) 当前词 = 单项范围;
            const 初始化源范围 词范围{当前行.来源.行号,
                当前词->字节起点, 字节位置, 当前词->标量起点, 标量位置};
            if (字节位置 - 当前词->字节起点 > 额度.最大单词字节数)
                return 资源拒绝(初始化额度种类::单词字节,
                    范围诊断(初始化诊断原因::额度到限, 词范围));
            if (标量位置 - 当前词->标量起点 > 额度.最大单词标量数)
                return 资源拒绝(初始化额度种类::单词标量,
                    范围诊断(初始化诊断原因::额度到限, 词范围));
        }
    }
    if (auto 失败 = 结束行(字节位置, 标量位置)) return std::move(*失败);
    if (行组.empty())
        return 文本拒绝(初始化解释状态::空材料, 初始化诊断原因::空材料, {});

    初始化定义材料 候选;
    for (const auto& 行 : 行组) {
        const 最小模板定义* 命中 = nullptr;
        for (const auto& 模板 : 最小结构表)
            if (模板完整匹配(模板, 行)) {
                if (命中) throw std::logic_error("multiple complete initialization templates");
                命中 = &模板;
            }
        if (!命中)
            return 文本拒绝(初始化解释状态::缺规则,
                初始化诊断原因::无完整模板, 行.来源);

        std::optional<std::int64_t> 整数;
        if (命中->模板 == 初始化最小模板::特征整数) {
            const auto 解析 = 解析标准I64(行.词组[5].词面);
            if (!解析.值)
                return 文本拒绝(初始化解释状态::定义不合法,
                    解析.拒绝原因.value(), 行.词组[5].来源);
            整数 = *解析.值;
        }
        初始化定义项 项;
        项.局部编号 = 候选.定义组.size();
        项.模板 = 命中->模板;
        项.来源 = 行.来源;
        std::array<初始化符号引用, 3> 引用{};
        std::size_t 引用数量 = 0;
        for (std::size_t i = 0; i < 命中->词数; ++i) {
            const auto& 角色 = 命中->结构[i].角色;
            if (!角色) continue;
            const auto& 词 = 行.词组[i];
            项.角色来源组.push_back({*角色, i, 词.来源});
            if (*角色 == 初始化源角色::整数值 || *角色 == 初始化源角色::别名词面)
                continue;
            if (候选.符号组.size() == 额度.最大角色符号数)
                return 资源拒绝(初始化额度种类::角色符号,
                    范围诊断(初始化诊断原因::额度到限, 词.来源));
            if (引用数量 == 引用.size()) throw std::logic_error("initialization role count invariant");
            const 初始化符号引用 新引用{候选.符号组.size()};
            候选.符号组.push_back({新引用,
                *角色 == 初始化源角色::特征类型 ? 初始化符号角色类型::特征类型引用
                    : 初始化符号角色类型::概念引用,
                std::string(词.词面), 词.来源});
            引用[引用数量++] = 新引用;
        }
        switch (命中->模板) {
        case 初始化最小模板::概念归类:
            if (引用数量 != 2) throw std::logic_error("initialization classification arity");
            项.载荷 = 初始化概念归类材料{引用[0], 引用[1]};
            break;
        case 初始化最小模板::特征整数:
            if (引用数量 != 2) throw std::logic_error("initialization integer arity");
            项.载荷 = 初始化特征定义材料{引用[0], 引用[1], 整数.value()};
            break;
        case 初始化最小模板::特征概念:
            if (引用数量 != 3) throw std::logic_error("initialization concept value arity");
            项.载荷 = 初始化特征定义材料{引用[0], 引用[1], 引用[2]};
            break;
        case 初始化最小模板::组成定义:
            if (引用数量 != 2) throw std::logic_error("initialization composition arity");
            项.载荷 = 初始化组成定义材料{引用[0], 引用[1]};
            break;
        case 初始化最小模板::显式别名:
            if (引用数量 != 1) throw std::logic_error("initialization alias arity");
            项.载荷 = 初始化名称提议材料{引用[0], std::string(行.词组[2].词面)};
            break;
        default: throw std::logic_error("initialization template dispatch invariant");
        }
        候选.定义组.push_back(std::move(项));
    }
    候选.原文 = 原文;
    核验完整材料(候选, 标量位置);
    初始化最小解释结果 结果;
    结果.解释状态 = 初始化解释状态::已完整解释;
    结果.材料 = std::move(候选);
    return 结果;
}

} // namespace

初始化最小解释结果 解释初始化标准文本(
    const std::string& 原文, const 初始化最小资源额度& 额度) {
    return 解释标准文本内部(原文, 额度);
}

} // namespace 海中鱼巣
