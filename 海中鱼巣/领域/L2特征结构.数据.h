#pragma once

#ifndef L2_FEATURE_STRUCTURE_NO_INCLUDES
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

struct L2特征定义事实 final {
    L2特征定义身份 身份;
    L2生命周期 生命周期;
    std::vector<L2属性事实> 固定属性;
    friend bool operator==(const L2特征定义事实&,
        const L2特征定义事实&) = default;
};

struct L2特征定义新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    friend bool operator==(const L2特征定义新增请求&,
        const L2特征定义新增请求&) = default;
};

struct L2特征定义写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2特征定义事实> 特征定义;
    friend bool operator==(const L2特征定义写入结果&,
        const L2特征定义写入结果&) = default;
};

struct L2特征定义固定属性新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2特征定义身份 特征定义;
    L2属性提交项 新属性;
    friend bool operator==(const L2特征定义固定属性新增请求&,
        const L2特征定义固定属性新增请求&) = default;
};

struct L2特征定义固定属性换代请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2特征定义身份 特征定义;
    稳定编码 旧值稳定编码;
    L2属性提交项 新属性;
    friend bool operator==(const L2特征定义固定属性换代请求&,
        const L2特征定义固定属性换代请求&) = default;
};

struct L2特征定义固定属性写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2属性事实> 当前属性;
    std::optional<L2属性事实> 已退出属性;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征定义固定属性写入结果&,
        const L2特征定义固定属性写入结果&) = default;
};

struct L2特征定义退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2特征定义身份 特征定义;
    std::vector<稳定编码> 当前固定属性值;
    friend bool operator==(const L2特征定义退出请求&,
        const L2特征定义退出请求&) = default;
};

struct L2特征定义退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2特征定义事实> 已退出特征定义;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征定义退出结果&,
        const L2特征定义退出结果&) = default;
};

struct L2特征定义完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2特征定义身份 特征定义;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2特征定义完整读取请求&,
        const L2特征定义完整读取请求&) = default;
};

struct L2特征定义完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2特征定义事实> 特征定义;
    friend bool operator==(const L2特征定义完整读取结果&,
        const L2特征定义完整读取结果&) = default;
};

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2特征定义新增请求有效(
    const L2特征定义新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份);
}

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2特征定义固定属性新增请求有效(
    const L2特征定义固定属性新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.特征定义.值) && L2属性提交项有效(请求.新属性);
}

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2特征定义固定属性换代请求有效(
    const L2特征定义固定属性换代请求& 请求) noexcept {
    return 有效(请求.旧值稳定编码)
        && L2特征定义固定属性新增请求有效({请求.请求头, 请求.幂等身份,
            请求.特征定义, 请求.新属性});
}

// 诊断责任：无适用错误分支；显式属性闭包须严格升序且无重复。
inline bool L2特征定义退出请求有效(
    const L2特征定义退出请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !L2结构幂等身份有效(请求.幂等身份)
        || !有效(请求.特征定义.值)) return false;
    for (std::size_t 索引 = 0; 索引 < 请求.当前固定属性值.size(); ++索引) {
        if (!有效(请求.当前固定属性值[索引])
            || (索引 != 0
                && !(请求.当前固定属性值[索引 - 1] < 请求.当前固定属性值[索引])))
            return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；纯值完整读取请求判断不读取事实。
inline bool L2特征定义完整读取请求有效(
    const L2特征定义完整读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.特征定义.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；只判断特征定义完整值式投影及稳定属性顺序。
inline bool L2特征定义事实完整(
    const L2特征定义事实& 特征定义, std::uint64_t 截止) noexcept {
    if (截止 == 0 || !有效(特征定义.身份.值)
        || !L2生命周期完整(特征定义.生命周期)
        || 特征定义.生命周期.创建事实代次 > 截止
        || (特征定义.生命周期.退出事实代次
            && *特征定义.生命周期.退出事实代次 > 截止)) return false;
    for (std::size_t 索引 = 0; 索引 < 特征定义.固定属性.size(); ++索引) {
        const auto& 属性 = 特征定义.固定属性[索引];
        if (!L2属性事实截止投影完整(属性, 截止) || 属性.退出事实代次)
            return false;
        if (索引 != 0) {
            const auto& 前项 = 特征定义.固定属性[索引 - 1];
            if (!(前项.属性类型身份 < 属性.属性类型身份)
                || 前项.属性类型身份 == 属性.属性类型身份) return false;
        }
    }
    return true;
}

// 诊断责任：无适用错误分支；只判断属性写成功载荷与首次截止一致。
inline bool L2特征定义固定属性写入结果::成功() const noexcept {
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

// 诊断责任：无适用错误分支；只判断定义和全部属性在同一代次退出。
inline bool L2特征定义退出结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !已退出特征定义
        || 已退出特征定义->生命周期.退出事实代次
            != 结果头.事实截止代次) return false;
    for (std::size_t 索引 = 0;
        索引 < 已退出特征定义->固定属性.size(); ++索引) {
        const auto& 属性 = 已退出特征定义->固定属性[索引];
        if (!L2属性事实截止投影完整(属性, 结果头.事实截止代次)
            || 属性.退出事实代次 != 结果头.事实截止代次
            || (索引 != 0
                && !(已退出特征定义->固定属性[索引 - 1].属性类型身份
                    < 属性.属性类型身份))) return false;
    }
    return 有效(已退出特征定义->身份.值)
        && L2生命周期完整(已退出特征定义->生命周期);
}

} // namespace 海中鱼巣
