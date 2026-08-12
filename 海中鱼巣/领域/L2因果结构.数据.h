#pragma once

#ifndef L2_CAUSAL_STRUCTURE_NO_INCLUDES
#include <algorithm>
#include <cstdint>
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2因果结构类型登记规则版本 = 1;

#define 定义L2因果结构类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2因果结构类型身份(L2因果条件关系类型身份);
定义L2因果结构类型身份(L2因果动作关系类型身份);
定义L2因果结构类型身份(L2因果结果关系类型身份);
定义L2因果结构类型身份(L2因果来源属性类型身份);

#undef 定义L2因果结构类型身份

enum class L2因果结构类型登记状态 : std::uint8_t {
    已提交 = 1, 精确重复 = 2, 已读取 = 3, 登记未加载 = 4,
    入口拒绝 = 5, 许可拒绝 = 6, 事实代次漂移 = 7,
    幂等冲突 = 8, 资源失败 = 9, 内部不一致 = 10
};

struct L2因果结构类型登记请求 final {
    L2结构请求头 请求头;
    std::uint32_t 规则版本 = L2因果结构类型登记规则版本;
    L2结构幂等身份 幂等身份{1};
    friend bool operator==(const L2因果结构类型登记请求&,
        const L2因果结构类型登记请求&) = default;
};

struct L2因果结构类型登记 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    std::uint32_t 规则版本 = L2因果结构类型登记规则版本;
    L2结构幂等身份 首次幂等身份{1};
    std::uint64_t 首次期望事实代次 = 0;
    std::uint64_t 建立事实代次 = 0;
    std::uint64_t 事实截止代次 = 0;
    L2因果条件关系类型身份 条件关系类型;
    L2因果动作关系类型身份 动作关系类型;
    L2因果结果关系类型身份 结果关系类型;
    L2因果来源属性类型身份 来源属性类型;
    friend bool operator==(const L2因果结构类型登记&,
        const L2因果结构类型登记&) = default;
};

struct L2因果结构类型登记结果 final {
    L2因果结构类型登记状态 状态 = L2因果结构类型登记状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<L2因果结构类型登记> 登记;
    bool 成功() const noexcept;
    friend bool operator==(const L2因果结构类型登记结果&,
        const L2因果结构类型登记结果&) = default;
};

struct L2因果事实 final {
    L2因果身份 身份;
    std::vector<L2有序身份引用> 条件状态;
    std::optional<稳定编码> 动作;
    std::vector<L2有序身份引用> 结果状态;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2因果事实& 左, const L2因果事实& 右) {
        return 左.身份 == 右.身份 && 左.条件状态 == 右.条件状态
            && 左.动作.has_value() == 右.动作.has_value()
            && (!左.动作 || *左.动作 == *右.动作)
            && 左.结果状态 == 右.结果状态
            && 左.来源稳定编码 == 右.来源稳定编码 && 左.生命周期 == 右.生命周期;
    }
};

struct L2因果身份来源读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2因果身份 因果;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2因果身份来源读取请求&,
        const L2因果身份来源读取请求&) = default;
};

struct L2因果身份来源读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2节点族身份来源事实> 身份来源;
    bool 成功() const noexcept;
    friend bool operator==(const L2因果身份来源读取结果&,
        const L2因果身份来源读取结果&) = default;
};

struct L2因果自有关系引用 final {
    稳定编码 族归属关系稳定编码;
    std::vector<稳定编码> 条件关系稳定编码;
    std::optional<稳定编码> 动作关系稳定编码;
    std::vector<稳定编码> 结果关系稳定编码;
    稳定编码 来源值稳定编码;
    friend bool operator==(const L2因果自有关系引用& 左,
        const L2因果自有关系引用& 右) {
        return 左.族归属关系稳定编码 == 右.族归属关系稳定编码
            && 左.条件关系稳定编码 == 右.条件关系稳定编码
            && 左.动作关系稳定编码.has_value() == 右.动作关系稳定编码.has_value()
            && (!左.动作关系稳定编码
                || *左.动作关系稳定编码 == *右.动作关系稳定编码)
            && 左.结果关系稳定编码 == 右.结果关系稳定编码
            && 左.来源值稳定编码 == 右.来源值稳定编码;
    }
};

struct L2因果新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    std::vector<L2有序身份引用> 条件状态;
    std::optional<稳定编码> 动作;
    std::vector<L2有序身份引用> 结果状态;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2因果新增请求& 左,
        const L2因果新增请求& 右) {
        return 左.请求头 == 右.请求头 && 左.幂等身份 == 右.幂等身份
            && 左.条件状态 == 右.条件状态
            && 左.动作.has_value() == 右.动作.has_value()
            && (!左.动作 || *左.动作 == *右.动作)
            && 左.结果状态 == 右.结果状态
            && 左.来源稳定编码 == 右.来源稳定编码;
    }
};

struct L2因果写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2因果事实> 因果;
    std::optional<L2因果自有关系引用> 自有事实;
    bool 成功() const noexcept;
    friend bool operator==(const L2因果写入结果&,
        const L2因果写入结果&) = default;
};

struct L2因果完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2因果身份 因果;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2因果完整读取请求&,
        const L2因果完整读取请求&) = default;
};

struct L2因果完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2因果事实> 因果;
    std::optional<L2因果自有关系引用> 自有事实;
    friend bool operator==(const L2因果完整读取结果&,
        const L2因果完整读取结果&) = default;
};

struct L2按条件状态当前因果读取请求 final {
    L2结构请求头 请求头;
    L2状态身份 条件状态;
    friend bool operator==(const L2按条件状态当前因果读取请求&,
        const L2按条件状态当前因果读取请求&) = default;
};
struct L2按动作当前因果读取请求 final {
    L2结构请求头 请求头;
    稳定编码 动作;
    friend bool operator==(const L2按动作当前因果读取请求&,
        const L2按动作当前因果读取请求&) = default;
};
struct L2按结果状态当前因果读取请求 final {
    L2结构请求头 请求头;
    L2状态身份 结果状态;
    friend bool operator==(const L2按结果状态当前因果读取请求&,
        const L2按结果状态当前因果读取请求&) = default;
};
struct L2当前因果组读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2因果事实> 因果;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前因果组读取结果&,
        const L2当前因果组读取结果&) = default;
};

inline bool L2因果结构类型登记请求有效(
    const L2因果结构类型登记请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.规则版本 == L2因果结构类型登记规则版本
        && 请求.幂等身份 == L2结构幂等身份{1};
}

inline bool L2因果结构类型登记完整(
    const L2因果结构类型登记& 登记) noexcept {
    const 稳定编码 编码组[] = {登记.条件关系类型.值, 登记.动作关系类型.值,
        登记.结果关系类型.值, 登记.来源属性类型.值};
    if (登记.合同版本 != L2结构合同版本
        || 登记.规则版本 != L2因果结构类型登记规则版本
        || 登记.首次幂等身份 != L2结构幂等身份{1}
        || 登记.首次期望事实代次 == 0 || 登记.建立事实代次 == 0
        || 登记.事实截止代次 < 登记.建立事实代次) return false;
    for (std::size_t 左 = 0; 左 < std::size(编码组); ++左) {
        if (!有效(编码组[左])) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return false;
    }
    return true;
}

inline bool L2因果有序引用组有效(
    const std::vector<L2有序身份引用>& 组) noexcept {
    if (组.empty() || 组.size() > 0x0FFF) return false;
    for (std::size_t 索引 = 0; 索引 < 组.size(); ++索引) {
        if (组[索引].顺序 == 0 || !有效(组[索引].目标)) return false;
        if (索引 && !(组[索引 - 1].顺序 < 组[索引].顺序
                || (组[索引 - 1].顺序 == 组[索引].顺序
                    && 组[索引 - 1].目标 < 组[索引].目标))) return false;
    }
    return true;
}

inline bool L2因果事实完整(const L2因果事实& 因果,
    std::uint64_t 截止) noexcept {
    return 截止 != 0 && 有效(因果.身份.值)
        && L2因果有序引用组有效(因果.条件状态)
        && (!因果.动作 || 有效(*因果.动作))
        && L2因果有序引用组有效(因果.结果状态)
        && 有效(因果.来源稳定编码) && L2生命周期完整(因果.生命周期)
        && 因果.生命周期.创建事实代次 <= 截止
        && (!因果.生命周期.退出事实代次
            || *因果.生命周期.退出事实代次 <= 截止);
}

inline bool L2因果新增请求有效(const L2因果新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 请求.幂等身份.值 != 0x4C32'4944'5352'4306ULL
        && 请求.幂等身份.值 != 0x2200'0000'0000'0001ULL
        && L2因果有序引用组有效(请求.条件状态)
        && (!请求.动作 || 有效(*请求.动作))
        && L2因果有序引用组有效(请求.结果状态)
        && 有效(请求.来源稳定编码);
}

inline bool L2因果完整读取请求有效(
    const L2因果完整读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.因果.值)) return false;
    if (请求.读取类别 == L2读取类别::当前) return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

inline bool L2因果身份来源读取请求有效(
    const L2因果身份来源读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.因果.值)) return false;
    if (请求.读取类别 == L2读取类别::当前) return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

inline bool L2按条件状态当前因果读取请求有效(
    const L2按条件状态当前因果读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.条件状态.值);
}
inline bool L2按动作当前因果读取请求有效(
    const L2按动作当前因果读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.动作);
}
inline bool L2按结果状态当前因果读取请求有效(
    const L2按结果状态当前因果读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.结果状态.值);
}

inline bool L2因果结构类型登记结果::成功() const noexcept {
    return (状态 == L2因果结构类型登记状态::已提交
            || 状态 == L2因果结构类型登记状态::精确重复
            || 状态 == L2因果结构类型登记状态::已读取)
        && 事实截止代次 != 0 && 登记
        && 登记->事实截止代次 == 事实截止代次
        && L2因果结构类型登记完整(*登记);
}

inline bool L2因果身份来源读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 身份来源 && L2节点族身份来源事实完整(*身份来源,
            读取类别 == L2读取类别::历史 ? 历史截止事实代次
                : 结果头.事实截止代次)
        && 身份来源->节点族 == L2节点族::因果;
}

inline bool L2因果写入结果::成功() const noexcept {
    if ((结果头.状态 != L2结构状态::已提交
            && 结果头.状态 != L2结构状态::精确重复)
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !因果 || !自有事实 || !L2因果事实完整(*因果, 结果头.事实截止代次)
        || !有效(自有事实->族归属关系稳定编码)
        || !有效(自有事实->来源值稳定编码)
        || 自有事实->条件关系稳定编码.size() != 因果->条件状态.size()
        || 自有事实->结果关系稳定编码.size() != 因果->结果状态.size()
        || static_cast<bool>(自有事实->动作关系稳定编码) != static_cast<bool>(因果->动作))
        return false;
    std::vector<稳定编码> 编码{自有事实->族归属关系稳定编码};
    编码.insert(编码.end(), 自有事实->条件关系稳定编码.begin(),
        自有事实->条件关系稳定编码.end());
    if (自有事实->动作关系稳定编码) 编码.push_back(*自有事实->动作关系稳定编码);
    编码.insert(编码.end(), 自有事实->结果关系稳定编码.begin(),
        自有事实->结果关系稳定编码.end());
    编码.push_back(自有事实->来源值稳定编码);
    std::sort(编码.begin(), 编码.end());
    return std::adjacent_find(编码.begin(), 编码.end()) == 编码.end()
        && std::all_of(编码.begin(), 编码.end(), [](稳定编码 编码值) {
            return 有效(编码值); });
}

inline bool L2当前因果组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    for (std::size_t 索引 = 0; 索引 < 因果.size(); ++索引) {
        if (!L2因果事实完整(因果[索引], 结果头.事实截止代次)
            || 因果[索引].生命周期.退出事实代次
            || (索引 && !(因果[索引 - 1].身份.值 < 因果[索引].身份.值))) return false;
    }
    return true;
}

} // namespace 海中鱼巣
