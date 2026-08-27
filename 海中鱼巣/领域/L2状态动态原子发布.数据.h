#pragma once

#ifndef L2_STATE_DYNAMIC_ATOMIC_PUBLISH_NO_INCLUDES
#include <cstdint>
#include <optional>
#include "L2结构公共.数据.h"
#include "L2状态结构.数据.h"
#include "L2动态结构.数据.h"
#include "../核心/L1所有者范围CRUD.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2状态动态原子发布合同版本 = 1;
inline constexpr std::uint32_t L2状态动态迁移专属合同版本_v2 = 2;
inline constexpr std::uint64_t L2状态动态原子发布幂等域 = 0xA2C0'0201ULL;

// 以下旧原子发布 DTO 仅作源兼容和历史诊断；生产入口不再新增事实。

enum class L2状态动态原子发布状态 : std::uint8_t {
    已提交 = 1, 精确重复 = 2, 入口拒绝 = 3, 许可拒绝 = 4,
    未找到 = 5, 已退出 = 6, 事实代次漂移 = 7, 幂等冲突 = 8,
    引用冲突 = 9, 资源失败 = 10, 内部不一致 = 11
};

struct L2状态动态原子发布请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 主体存在;
    L2状态身份 旧当前状态;
    L2特征实例身份 新状态特征实例;
    L2原始值材料 新状态精确值材料;
    L2原始值材料 新状态时间值材料;
    L2原始值材料 动态起始时间材料;
    L2原始值材料 动态结束时间材料;
    稳定编码 来源方法稳定编码;
    friend bool operator==(const L2状态动态原子发布请求&,
        const L2状态动态原子发布请求&) = default;
};

struct L2状态动态原子发布结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 新状态;
    std::optional<L2状态自有关系引用> 新状态自有关系;
    std::optional<L2动态事实> 新动态;
    std::optional<L2动态自有关系引用> 新动态自有关系;
    std::optional<L2状态自有关系退出投影> 已退出旧状态关系;
    friend bool operator==(const L2状态动态原子发布结果&,
        const L2状态动态原子发布结果&) = default;
};

enum class L2状态动态迁移操作状态_v2 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    待迁移 = 4,
    已有异义当前 = 5,
    已可能发布 = 6,
    入口拒绝 = 8,
    许可拒绝 = 9,
    事实代次漂移 = 10,
    幂等冲突 = 11,
    引用冲突 = 12,
    资源失败 = 13,
    内部不一致 = 14
};

struct L2状态动态迁移请求头_v2 final {
    L2结构请求头 共同请求头{};
    std::uint32_t 迁移专属合同版本 = L2状态动态迁移专属合同版本_v2;
    friend bool operator==(const L2状态动态迁移请求头_v2&,
        const L2状态动态迁移请求头_v2&) = default;
    bool 有效() const noexcept;
};

struct L2状态动态迁移请求_v2 final {
    L2状态动态迁移请求头_v2 请求头{};
    L2结构幂等身份 幂等身份{};
    L2存在身份 主体存在{};
    L2状态身份 旧当前状态{};
    L2特征实例身份 新状态特征实例{};
    L2原始值材料 新状态精确值材料{};
    L2原始值材料 新状态时间值材料{};
    L2原始值材料 动态起始时间材料{};
    L2原始值材料 动态结束时间材料{};
    稳定编码 来源方法稳定编码{};
    L2状态当前选择关系引用 旧当前选择{};
    friend bool operator==(const L2状态动态迁移请求_v2&,
        const L2状态动态迁移请求_v2&) = default;
    bool 有效() const noexcept;
};

struct L2状态动态迁移结果_v2 final {
    L2结构结果头 结果头{};
    std::uint32_t 原子发布合同版本 = L2状态动态迁移专属合同版本_v2;
    L2状态动态迁移操作状态_v2 迁移操作状态 =
        L2状态动态迁移操作状态_v2::入口拒绝;
    std::optional<L2状态事实> 新状态{};
    std::optional<L2状态自有关系引用> 新状态自有关系{};
    std::optional<L2动态事实> 新动态{};
    std::optional<L2动态自有关系引用> 新动态自有关系{};
    std::optional<L2状态当前选择关系引用> 新当前选择关系{};
    std::optional<L2状态当前选择关系引用> 已退出旧当前选择关系{};
    std::uint64_t 共同事实截止代次 = 0;
    friend bool operator==(const L2状态动态迁移结果_v2&,
        const L2状态动态迁移结果_v2&) = default;
    bool 成功() const noexcept;
};

inline bool L2状态动态原子发布请求有效(
    const L2状态动态原子发布请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.主体存在.值) && 有效(请求.旧当前状态.值)
        && 有效(请求.新状态特征实例.值)
        && 有效(请求.来源方法稳定编码);
}

inline bool L2状态动态迁移请求头_v2::有效() const noexcept {
    return L2结构请求头合同有效(共同请求头)
        && 共同请求头.期望事实代次 != 0
        && 迁移专属合同版本 == L2状态动态迁移专属合同版本_v2;
}

inline bool L2状态动态迁移请求_v2::有效() const noexcept {
    return 请求头.有效()
        && L2结构幂等身份有效(幂等身份)
        && 海中鱼巣::有效(主体存在.值)
        && 海中鱼巣::有效(旧当前状态.值)
        && 海中鱼巣::有效(新状态特征实例.值)
        && L2状态原始值材料有效(新状态精确值材料)
        && L2状态原始值材料有效(新状态时间值材料)
        && L2动态原始值材料有效(动态起始时间材料)
        && L2动态原始值材料有效(动态结束时间材料)
        && 动态起始时间材料.index() == 动态结束时间材料.index()
        && 海中鱼巣::有效(来源方法稳定编码)
        && L2状态当前选择关系引用完整(
            旧当前选择, 请求头.共同请求头.期望事实代次)
        && 旧当前选择.特征实例 == 新状态特征实例
        && 旧当前选择.状态 == 旧当前状态
        && !旧当前选择.生命周期.退出事实代次
        ;
}

} // namespace 海中鱼巣
