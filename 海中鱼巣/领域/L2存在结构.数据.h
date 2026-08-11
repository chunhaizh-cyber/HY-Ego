#pragma once

#ifndef L2_EXISTENCE_STRUCTURE_NO_INCLUDES
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

struct L2存在事实 final {
    L2存在身份 身份;
    L2生命周期 生命周期;
    std::vector<L2属性事实> 固定属性;
    friend bool operator==(const L2存在事实&, const L2存在事实&) = default;
};

struct L2存在节点新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    friend bool operator==(const L2存在节点新增请求&,
        const L2存在节点新增请求&) = default;
};

struct L2存在节点读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2存在身份 存在;
    friend bool operator==(const L2存在节点读取请求&,
        const L2存在节点读取请求&) = default;
};

struct L2存在节点写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2存在事实> 存在;
    friend bool operator==(const L2存在节点写入结果&,
        const L2存在节点写入结果&) = default;
};

struct L2存在节点读取结果 final {
    L2结构结果头 结果头;
    std::optional<L2存在事实> 存在;
    friend bool operator==(const L2存在节点读取结果&,
        const L2存在节点读取结果&) = default;
};

struct L2存在固定属性新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 存在;
    L2属性提交项 新属性;
    friend bool operator==(const L2存在固定属性新增请求&,
        const L2存在固定属性新增请求&) = default;
};

struct L2存在固定属性换代请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 存在;
    稳定编码 旧值稳定编码;
    L2属性提交项 新属性;
    friend bool operator==(const L2存在固定属性换代请求&,
        const L2存在固定属性换代请求&) = default;
};

struct L2存在固定属性写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2属性事实> 当前属性;
    std::optional<L2属性事实> 已退出属性;
    bool 成功() const noexcept;
    friend bool operator==(const L2存在固定属性写入结果&,
        const L2存在固定属性写入结果&) = default;
};

struct L2存在退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 存在;
    std::vector<稳定编码> 当前固定属性值;
    friend bool operator==(const L2存在退出请求&,
        const L2存在退出请求&) = default;
};

struct L2存在退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2存在事实> 已退出存在;
    bool 成功() const noexcept;
    friend bool operator==(const L2存在退出结果&,
        const L2存在退出结果&) = default;
};

struct L2存在完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2存在身份 存在;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2存在完整读取请求&,
        const L2存在完整读取请求&) = default;
};

struct L2存在完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2存在事实> 存在;
    friend bool operator==(const L2存在完整读取结果&,
        const L2存在完整读取结果&) = default;
};

struct L2存在场景成员引用事实 final {
    稳定编码 关系稳定编码;
    L2场景身份 场景;
    L2存在身份 存在;
    std::uint64_t 顺序 = 0;
    L2生命周期 生命周期;
    friend bool operator==(const L2存在场景成员引用事实&,
        const L2存在场景成员引用事实&) = default;
};

struct L2存在当前场景成员引用读取请求 final {
    L2结构请求头 请求头;
    L2存在身份 存在;
    friend bool operator==(const L2存在当前场景成员引用读取请求&,
        const L2存在当前场景成员引用读取请求&) = default;
};

struct L2存在当前场景成员引用读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2存在场景成员引用事实> 引用;
    bool 成功() const noexcept;
    friend bool operator==(const L2存在当前场景成员引用读取结果&,
        const L2存在当前场景成员引用读取结果&) = default;
};

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2存在节点新增请求有效(const L2存在节点新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份);
}

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2存在节点读取请求有效(const L2存在节点读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && (请求.读取类别 == L2读取类别::当前
            || 请求.读取类别 == L2读取类别::历史)
        && 有效(请求.存在.值);
}

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2存在固定属性新增请求有效(
    const L2存在固定属性新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.存在.值) && L2属性提交项有效(请求.新属性);
}

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2存在固定属性换代请求有效(
    const L2存在固定属性换代请求& 请求) noexcept {
    return 有效(请求.旧值稳定编码)
        && L2存在固定属性新增请求有效({请求.请求头, 请求.幂等身份,
            请求.存在, 请求.新属性});
}

// 诊断责任：无适用错误分支；显式属性闭包须严格升序且无重复。
inline bool L2存在退出请求有效(const L2存在退出请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !L2结构幂等身份有效(请求.幂等身份)
        || !有效(请求.存在.值)) return false;
    for (std::size_t 索引 = 0; 索引 < 请求.当前固定属性值.size(); ++索引) {
        if (!有效(请求.当前固定属性值[索引])
            || (索引 != 0
                && !(请求.当前固定属性值[索引 - 1] < 请求.当前固定属性值[索引])))
            return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；纯值完整读取请求判断不读取事实。
inline bool L2存在完整读取请求有效(
    const L2存在完整读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.存在.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；纯值反向查询请求判断不读取事实。
inline bool L2存在当前场景成员引用读取请求有效(
    const L2存在当前场景成员引用读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.存在.值);
}

// 诊断责任：无适用错误分支；只判断 E1 存在值式事实的完整形状。
inline bool L2存在E1事实完整(const L2存在事实& 存在) noexcept {
    return 有效(存在.身份.值) && L2生命周期完整(存在.生命周期)
        && 存在.固定属性.empty();
}

// 诊断责任：无适用错误分支；只判断最终存在完整值式投影及稳定属性顺序。
inline bool L2存在事实完整(
    const L2存在事实& 存在, std::uint64_t 截止) noexcept {
    if (截止 == 0 || !有效(存在.身份.值) || !L2生命周期完整(存在.生命周期)
        || 存在.生命周期.创建事实代次 > 截止
        || (存在.生命周期.退出事实代次
            && *存在.生命周期.退出事实代次 > 截止)) return false;
    for (std::size_t 索引 = 0; 索引 < 存在.固定属性.size(); ++索引) {
        const auto& 属性 = 存在.固定属性[索引];
        if (!L2属性事实截止投影完整(属性, 截止) || 属性.退出事实代次)
            return false;
        if (索引 != 0) {
            const auto& 前项 = 存在.固定属性[索引 - 1];
            if (!(前项.属性类型身份 < 属性.属性类型身份)
                || 前项.属性类型身份 == 属性.属性类型身份) return false;
        }
    }
    return true;
}

// 诊断责任：无适用错误分支；只判断反向成员引用的完整值式形状。
inline bool L2存在场景成员引用事实完整(
    const L2存在场景成员引用事实& 引用, std::uint64_t 截止) noexcept {
    return 截止 != 0 && 有效(引用.关系稳定编码) && 有效(引用.场景.值)
        && 有效(引用.存在.值) && 引用.顺序 != 0
        && L2生命周期完整(引用.生命周期)
        && 引用.生命周期.创建事实代次 <= 截止
        && (!引用.生命周期.退出事实代次
            || *引用.生命周期.退出事实代次 <= 截止);
}

// 诊断责任：无适用错误分支；只判断属性写成功载荷与首次截止一致。
inline bool L2存在固定属性写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次 || !当前属性
        || !L2属性事实截止投影完整(*当前属性, 结果头.事实截止代次)
        || 当前属性->退出事实代次) return false;
    if (!已退出属性) return true;
    return L2属性事实截止投影完整(*已退出属性, 结果头.事实截止代次)
        && 已退出属性->退出事实代次 == 结果头.事实截止代次
        && 已退出属性->属性类型身份 == 当前属性->属性类型身份
        && 已退出属性->值稳定编码 != 当前属性->值稳定编码;
}

// 诊断责任：无适用错误分支；只判断存在和全部属性在同一代次退出。
inline bool L2存在退出结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次 || !已退出存在
        || 已退出存在->生命周期.退出事实代次 != 结果头.事实截止代次)
        return false;
    for (std::size_t 索引 = 0; 索引 < 已退出存在->固定属性.size(); ++索引) {
        const auto& 属性 = 已退出存在->固定属性[索引];
        if (!L2属性事实截止投影完整(属性, 结果头.事实截止代次)
            || 属性.退出事实代次 != 结果头.事实截止代次
            || (索引 != 0
                && !(已退出存在->固定属性[索引 - 1].属性类型身份
                    < 属性.属性类型身份))) return false;
    }
    return 有效(已退出存在->身份.值)
        && L2生命周期完整(已退出存在->生命周期);
}

// 诊断责任：无适用错误分支；只判断反向查询成功头和稳定有序完整引用组。
inline bool L2存在当前场景成员引用读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    for (std::size_t 索引 = 0; 索引 < 引用.size(); ++索引) {
        const auto& 项 = 引用[索引];
        if (!L2存在场景成员引用事实完整(项, 结果头.事实截止代次)
            || 项.生命周期.退出事实代次) return false;
        for (std::size_t 前索引 = 0; 前索引 < 索引; ++前索引)
            if (引用[前索引].场景 == 项.场景
                || 引用[前索引].关系稳定编码 == 项.关系稳定编码) return false;
        if (索引 != 0) {
            const auto& 前项 = 引用[索引 - 1];
            const bool 严格在前 = 前项.顺序 < 项.顺序
                || (前项.顺序 == 项.顺序 && (前项.场景.值 < 项.场景.值
                    || (前项.场景 == 项.场景
                        && 前项.关系稳定编码 < 项.关系稳定编码)));
            if (!严格在前) return false;
        }
    }
    return true;
}

} // namespace 海中鱼巣
