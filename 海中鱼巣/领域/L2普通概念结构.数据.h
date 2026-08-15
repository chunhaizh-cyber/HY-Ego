#pragma once

#ifndef L2_ORDINARY_CONCEPT_STRUCTURE_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#include "不可变材料.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2普通概念结构合同版本 = 1;

enum class L2概念治理生命周期状态 : std::uint8_t {
    活跃 = 1,
    冷却 = 2,
    退役 = 3
};

struct L2概念特征值引用 final {
    L2特征实例身份 特征实例;
    稳定编码 值稳定编码;
    friend bool operator==(const L2概念特征值引用&,
        const L2概念特征值引用&) = default;
};

struct L2概念动作引用 final {
    稳定编码 值;
    friend bool operator==(const L2概念动作引用&,
        const L2概念动作引用&) = default;
};

using L2概念世界关系端点 = std::variant<L2场景身份, L2存在身份,
    L2特征定义身份, L2特征实例身份, L2状态身份, L2动态身份,
    L2因果身份, L2概念动作引用>;

enum class L2概念世界关系类别 : std::uint8_t {
    场景父子 = 1,
    场景成员 = 2,
    场景宿主存在 = 3,
    特征宿主 = 4,
    特征定义 = 5,
    状态主体 = 6,
    状态特征实例 = 7,
    动态主体 = 8,
    动态前状态 = 9,
    动态后状态 = 10,
    因果条件状态 = 11,
    因果动作引用 = 12,
    因果结果状态 = 13
};

struct L2概念世界关系引用 final {
    L2概念世界关系类别 类别 = L2概念世界关系类别::场景父子;
    稳定编码 关系稳定编码;
    L2概念世界关系端点 源;
    L2概念世界关系端点 目标;
    friend bool operator==(const L2概念世界关系引用&,
        const L2概念世界关系引用&) = default;
};

using L2概念定义目标 = std::variant<L2概念身份, L2场景身份,
    L2存在身份, L2特征定义身份, L2特征实例身份,
    L2概念特征值引用, L2状态身份, L2动态身份, L2因果身份,
    L2概念世界关系引用>;

struct L2概念签名值式项 final {
    std::uint64_t 角色 = 0;
    std::uint64_t 顺序 = 0;
    L2原始值材料 值;
    friend bool operator==(const L2概念签名值式项&,
        const L2概念签名值式项&) = default;
};

struct L2概念定义引用项 final {
    std::uint64_t 角色 = 0;
    std::uint64_t 顺序 = 0;
    L2概念定义目标 目标;
    friend bool operator==(const L2概念定义引用项&,
        const L2概念定义引用项&) = default;
};

struct L2概念本体根归属事实 final {
    L2概念身份 本体根;
    L2概念身份 概念;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念本体根归属事实&,
        const L2概念本体根归属事实&) = default;
};

struct L2概念签名规则事实 final {
    L2概念身份 概念;
    不可变材料身份 规则材料;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念签名规则事实&,
        const L2概念签名规则事实&) = default;
};

struct L2概念签名值式事实 final {
    L2概念身份 概念;
    L2概念签名值式项 项;
    稳定编码 值稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念签名值式事实&,
        const L2概念签名值式事实&) = default;
};

struct L2概念定义关系事实 final {
    L2概念身份 概念;
    L2概念定义引用项 项;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念定义关系事实&,
        const L2概念定义关系事实&) = default;
};

struct L2概念直接上位事实 final {
    L2概念身份 上位概念;
    L2概念身份 下位概念;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念直接上位事实&,
        const L2概念直接上位事实&) = default;
};

struct L2概念来源证据事实 final {
    L2概念身份 概念;
    不可变材料身份 证据材料;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念来源证据事实&,
        const L2概念来源证据事实&) = default;
};

struct L2概念治理生命周期事实 final {
    L2概念身份 概念;
    L2概念治理生命周期状态 状态 = L2概念治理生命周期状态::活跃;
    稳定编码 事实稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念治理生命周期事实&,
        const L2概念治理生命周期事实&) = default;
};

struct L2普通概念事实 final {
    L2概念身份 概念;
    L2节点族身份来源事实 身份来源;
    L2概念本体根归属事实 本体根归属;
    L2概念签名规则事实 签名规则;
    std::vector<L2概念签名值式事实> 签名值式项;
    std::vector<L2概念定义关系事实> 定义关系;
    std::vector<L2概念直接上位事实> 直接上位;
    std::vector<L2概念来源证据事实> 来源证据;
    L2概念治理生命周期事实 治理生命周期;
    friend bool operator==(const L2普通概念事实&,
        const L2普通概念事实&) = default;
};

struct L2普通概念建立请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通概念合同版本 = L2普通概念结构合同版本;
    L2结构幂等身份 幂等身份;
    L2概念身份 本体根;
    不可变材料身份 签名规则材料;
    std::vector<L2概念签名值式项> 签名值式项;
    std::vector<L2概念定义引用项> 初始定义;
    std::vector<L2概念身份> 直接上位;
    std::vector<不可变材料身份> 来源证据材料;
    friend bool operator==(const L2普通概念建立请求&,
        const L2普通概念建立请求&) = default;
};

struct L2普通概念写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通概念事实> 概念;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通概念写入结果&,
        const L2普通概念写入结果&) = default;
};

struct L2普通概念读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通概念合同版本 = L2普通概念结构合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念身份 概念;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2普通概念读取请求&,
        const L2普通概念读取请求&) = default;
};

struct L2普通概念读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2普通概念事实> 概念;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通概念读取结果&,
        const L2普通概念读取结果&) = default;
};

// 诊断责任：无适用错误分支；只校验闭合关系类别、非零关系身份与强类型端点方向。
inline bool L2概念世界关系引用有效(
    const L2概念世界关系引用& 引用) noexcept {
    if (!有效(引用.关系稳定编码)) return false;
    const auto 场景到场景 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2场景身份>(&引用.源);
        const auto* 目标 = std::get_if<L2场景身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值) && *源 != *目标;
    };
    const auto 场景到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2场景身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 特征实例到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2特征实例身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 特征实例到定义 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2特征实例身份>(&引用.源);
        const auto* 目标 = std::get_if<L2特征定义身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 状态到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2状态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 状态到特征实例 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2状态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2特征实例身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 动态到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2动态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 动态到状态 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2动态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2状态身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 因果到状态 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2因果身份>(&引用.源);
        const auto* 目标 = std::get_if<L2状态身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 因果到动作 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2因果身份>(&引用.源);
        const auto* 目标 = std::get_if<L2概念动作引用>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    switch (引用.类别) {
    case L2概念世界关系类别::场景父子: return 场景到场景();
    case L2概念世界关系类别::场景成员:
    case L2概念世界关系类别::场景宿主存在: return 场景到存在();
    case L2概念世界关系类别::特征宿主: return 特征实例到存在();
    case L2概念世界关系类别::特征定义: return 特征实例到定义();
    case L2概念世界关系类别::状态主体: return 状态到存在();
    case L2概念世界关系类别::状态特征实例: return 状态到特征实例();
    case L2概念世界关系类别::动态主体: return 动态到存在();
    case L2概念世界关系类别::动态前状态:
    case L2概念世界关系类别::动态后状态: return 动态到状态();
    case L2概念世界关系类别::因果条件状态:
    case L2概念世界关系类别::因果结果状态: return 因果到状态();
    case L2概念世界关系类别::因果动作引用: return 因果到动作();
    default: return false;
    }
}

// 诊断责任：无适用错误分支；只校验定义目标的强类型值式形状，不读取目标事实。
inline bool L2概念定义目标有效(const L2概念定义目标& 目标) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L2概念世界关系引用>)
            return L2概念世界关系引用有效(值);
        else if constexpr (std::is_same_v<类型, L2概念特征值引用>)
            return 有效(值.特征实例.值) && 有效(值.值稳定编码);
        else
            return 有效(值.值);
    }, 目标);
}

// 诊断责任：无适用错误分支；只校验普通概念建立请求的值式合同、排序和闭合强类型引用。
inline bool L2普通概念建立请求有效(
    const L2普通概念建立请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.普通概念合同版本 != L2普通概念结构合同版本
        || 请求.请求头.期望事实代次 == 0
        || !L2结构幂等身份有效(请求.幂等身份)
        || !有效(请求.本体根.值) || !有效(请求.签名规则材料.值)
        || 请求.直接上位.empty() || 请求.来源证据材料.empty()) return false;
    const auto 材料有效 = [](const L2原始值材料& 材料) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) return true;
            else if constexpr (std::is_same_v<类型, L2独立材料引用>)
                return 有效(值.编码);
            else return !值.empty();
        }, 材料);
    };
    const auto 键小于 = [](std::uint64_t 左角色, std::uint64_t 左顺序,
        std::uint64_t 右角色, std::uint64_t 右顺序) noexcept {
        return 左角色 < 右角色 || (左角色 == 右角色 && 左顺序 < 右顺序);
    };
    for (std::size_t 索引 = 0; 索引 < 请求.签名值式项.size(); ++索引) {
        const auto& 项 = 请求.签名值式项[索引];
        if (项.角色 == 0 || 项.顺序 == 0 || !材料有效(项.值)) return false;
        if (索引 != 0 && !键小于(请求.签名值式项[索引 - 1].角色,
                请求.签名值式项[索引 - 1].顺序, 项.角色, 项.顺序)) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 请求.初始定义.size(); ++索引) {
        const auto& 项 = 请求.初始定义[索引];
        if (项.角色 == 0 || 项.顺序 == 0 || !L2概念定义目标有效(项.目标))
            return false;
        if (索引 != 0 && !键小于(请求.初始定义[索引 - 1].角色,
                请求.初始定义[索引 - 1].顺序, 项.角色, 项.顺序)) return false;
    }
    for (const auto& 值式项 : 请求.签名值式项)
        for (const auto& 定义项 : 请求.初始定义)
            if (值式项.角色 == 定义项.角色 && 值式项.顺序 == 定义项.顺序)
                return false;
    for (std::size_t 索引 = 0; 索引 < 请求.直接上位.size(); ++索引) {
        if (!有效(请求.直接上位[索引].值)
            || (索引 != 0
                && !(请求.直接上位[索引 - 1].值 < 请求.直接上位[索引].值)))
            return false;
    }
    for (std::size_t 索引 = 0; 索引 < 请求.来源证据材料.size(); ++索引) {
        if (!有效(请求.来源证据材料[索引].值)
            || (索引 != 0
                && !(请求.来源证据材料[索引 - 1].值
                    < 请求.来源证据材料[索引].值))) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；当前与历史读取参数按类别互斥。
inline bool L2普通概念读取请求有效(
    const L2普通概念读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.普通概念合同版本 != L2普通概念结构合同版本
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.概念.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；只判断普通概念负载在具名截止的值式完整性。
inline bool L2普通概念事实完整(
    const L2普通概念事实& 事实, std::uint64_t 截止) noexcept {
    const auto 生命周期完整 = [截止](const L2生命周期& 生命周期) noexcept {
        return L2生命周期完整(生命周期)
            && 生命周期.创建事实代次 <= 截止
            && (!生命周期.退出事实代次
                || *生命周期.退出事实代次 <= 截止);
    };
    const auto 材料有效 = [](const L2原始值材料& 材料) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) return true;
            else if constexpr (std::is_same_v<类型, L2独立材料引用>)
                return 有效(值.编码);
            else return !值.empty();
        }, 材料);
    };
    const auto 键小于 = [](std::uint64_t 左角色, std::uint64_t 左顺序,
        std::uint64_t 右角色, std::uint64_t 右顺序) noexcept {
        return 左角色 < 右角色 || (左角色 == 右角色 && 左顺序 < 右顺序);
    };
    const bool 治理状态有效 = 事实.治理生命周期.状态
            == L2概念治理生命周期状态::活跃
        || 事实.治理生命周期.状态 == L2概念治理生命周期状态::冷却
        || 事实.治理生命周期.状态 == L2概念治理生命周期状态::退役;
    if (截止 == 0 || !有效(事实.概念.值)
        || !L2节点族身份来源事实完整(事实.身份来源, 截止)
        || 事实.身份来源.节点族 != L2节点族::概念
        || 事实.身份来源.节点稳定编码 != 事实.概念.值
        || !有效(事实.本体根归属.本体根.值)
        || 事实.本体根归属.本体根 == 事实.概念
        || 事实.本体根归属.概念 != 事实.概念
        || !有效(事实.本体根归属.关系稳定编码)
        || !生命周期完整(事实.本体根归属.生命周期)
        || 事实.签名规则.概念 != 事实.概念
        || !有效(事实.签名规则.规则材料.值)
        || !有效(事实.签名规则.关系稳定编码)
        || !生命周期完整(事实.签名规则.生命周期)
        || 事实.直接上位.empty() || 事实.来源证据.empty()
        || 事实.治理生命周期.概念 != 事实.概念 || !治理状态有效
        || !有效(事实.治理生命周期.事实稳定编码)
        || !生命周期完整(事实.治理生命周期.生命周期)) return false;
    for (std::size_t 索引 = 0; 索引 < 事实.签名值式项.size(); ++索引) {
        const auto& 项 = 事实.签名值式项[索引];
        if (项.概念 != 事实.概念 || 项.项.角色 == 0 || 项.项.顺序 == 0
            || !材料有效(项.项.值) || !有效(项.值稳定编码)
            || !生命周期完整(项.生命周期)) return false;
        if (索引 != 0 && !键小于(事实.签名值式项[索引 - 1].项.角色,
                事实.签名值式项[索引 - 1].项.顺序,
                项.项.角色, 项.项.顺序)) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 事实.定义关系.size(); ++索引) {
        const auto& 项 = 事实.定义关系[索引];
        if (项.概念 != 事实.概念 || 项.项.角色 == 0 || 项.项.顺序 == 0
            || !L2概念定义目标有效(项.项.目标)
            || !有效(项.关系稳定编码) || !生命周期完整(项.生命周期))
            return false;
        if (索引 != 0 && !键小于(事实.定义关系[索引 - 1].项.角色,
                事实.定义关系[索引 - 1].项.顺序,
                项.项.角色, 项.项.顺序)) return false;
    }
    for (const auto& 值式项 : 事实.签名值式项)
        for (const auto& 定义项 : 事实.定义关系)
            if (值式项.项.角色 == 定义项.项.角色
                && 值式项.项.顺序 == 定义项.项.顺序) return false;
    for (std::size_t 索引 = 0; 索引 < 事实.直接上位.size(); ++索引) {
        const auto& 上位 = 事实.直接上位[索引];
        if (!有效(上位.上位概念.值) || 上位.上位概念 == 事实.概念
            || 上位.下位概念 != 事实.概念 || !有效(上位.关系稳定编码)
            || !生命周期完整(上位.生命周期)
            || (索引 != 0
                && !(事实.直接上位[索引 - 1].上位概念.值 < 上位.上位概念.值)))
            return false;
    }
    for (std::size_t 索引 = 0; 索引 < 事实.来源证据.size(); ++索引) {
        const auto& 证据 = 事实.来源证据[索引];
        if (证据.概念 != 事实.概念 || !有效(证据.证据材料.值)
            || !有效(证据.关系稳定编码) || !生命周期完整(证据.生命周期)
            || (索引 != 0
                && !(事实.来源证据[索引 - 1].证据材料.值 < 证据.证据材料.值)))
            return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；只判断写结果是否为完整的首次提交或原结果精确重复形状。
inline bool L2普通概念写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次 || !概念
        || !L2普通概念事实完整(*概念, 结果头.事实截止代次)
        || 概念->治理生命周期.状态 != L2概念治理生命周期状态::活跃)
        return false;
    const auto 身份来源当前 = [](const L2节点族身份来源事实& 来源) noexcept {
        return !来源.所有者生命周期.退出事实代次
            && !来源.节点生命周期.退出事实代次
            && !来源.族锚点生命周期.退出事实代次
            && !来源.关系类型生命周期.退出事实代次
            && !来源.归属关系生命周期.退出事实代次;
    };
    if (!身份来源当前(概念->身份来源)
        || 概念->本体根归属.生命周期.退出事实代次
        || 概念->签名规则.生命周期.退出事实代次
        || 概念->治理生命周期.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->签名值式项)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->定义关系)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->直接上位)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->来源证据)
        if (项.生命周期.退出事实代次) return false;
    return true;
}

// 诊断责任：无适用错误分支；只判断当前或历史读取结果的完整已读取形状。
inline bool L2普通概念读取结果::成功() const noexcept {
    const std::uint64_t 投影截止 = 读取类别 == L2读取类别::历史
        ? 历史截止事实代次 : 结果头.事实截止代次;
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次 || !概念
        || !L2普通概念事实完整(*概念, 投影截止)) return false;
    if (读取类别 == L2读取类别::历史)
        return 历史截止事实代次 != 0
            && 结果头.事实截止代次 == 历史截止事实代次;
    if (读取类别 != L2读取类别::当前 || 历史截止事实代次 != 0)
        return false;
    const auto 身份来源当前 = [](const L2节点族身份来源事实& 来源) noexcept {
        return !来源.所有者生命周期.退出事实代次
            && !来源.节点生命周期.退出事实代次
            && !来源.族锚点生命周期.退出事实代次
            && !来源.关系类型生命周期.退出事实代次
            && !来源.归属关系生命周期.退出事实代次;
    };
    if (!身份来源当前(概念->身份来源)
        || 概念->本体根归属.生命周期.退出事实代次
        || 概念->签名规则.生命周期.退出事实代次
        || 概念->治理生命周期.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->签名值式项)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->定义关系)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->直接上位)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->来源证据)
        if (项.生命周期.退出事实代次) return false;
    return true;
}

} // namespace 海中鱼巣
