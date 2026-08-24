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
inline constexpr std::uint64_t L2状态动态原子发布幂等域 = 0xA2C0'0201ULL;

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

inline bool L2状态动态原子发布请求有效(
    const L2状态动态原子发布请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.主体存在.值) && 有效(请求.旧当前状态.值)
        && 有效(请求.新状态特征实例.值)
        && 有效(请求.来源方法稳定编码);
}

} // namespace 海中鱼巣
