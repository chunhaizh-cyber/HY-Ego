#pragma once

#ifndef 任务筹办当前就绪与执行冻结数据_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>

#include "../L2任务结构.数据.h"
#include "任务特征方法查找与条件提议.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 任务筹办当前就绪合同版本_v2 = 2;

struct 任务条件当前就绪证据 final {
    L2方法条件事实 条件;
    L2存在身份 绑定目标;
    std::optional<L2场景身份> 场景;
    std::optional<L2存在场景成员引用事实> 场景成员;
    L2状态事实 当前状态;
    L2特征比较具名关系 具名关系 =
        L2特征比较具名关系::当前达到目标;
    std::uint8_t 允许关系位 = 0;
    friend bool operator==(const 任务条件当前就绪证据&,
        const 任务条件当前就绪证据&) = default;
};

struct 任务输入规格冻结项 final {
    L2方法输入规格事实 输入规格;
    L2存在身份 绑定目标;
    friend bool operator==(const 任务输入规格冻结项&,
        const 任务输入规格冻结项&) = default;
};

struct 任务参数绑定冻结项 final {
    L2方法输入规格身份 来源输入规格;
    std::uint64_t 作用对象角色 = 0;
    L2方法规格材料 参数规格;
    L2存在身份 绑定目标;
    friend bool operator==(const 任务参数绑定冻结项&,
        const 任务参数绑定冻结项&) = default;
};

struct 任务限制条件复判证据 final {
    L2方法限制条件事实 限制条件;
    L2存在身份 绑定目标;
    L2方法限制解释器身份 解释器;
    std::uint32_t 解释规则版本 = 0;
    L2任务限制当前事实证据引用 当前事实证据;
    bool 已满足 = false;
    friend bool operator==(const 任务限制条件复判证据&,
        const 任务限制条件复判证据&) = default;
};

enum class 任务筹办当前就绪结论 : std::uint8_t {
    已就绪 = 1,
    条件未满足 = 2,
    输入待绑定 = 3,
    限制待解释 = 4,
    方法结构待补齐 = 5,
    正式选择待收束 = 6,
    当前性漂移 = 7,
    引用冲突 = 8,
    数量预算不足 = 9,
    资源失败 = 10,
    内部错误 = 11,
    入口拒绝 = 12,
    许可拒绝 = 13,
    幂等冲突 = 14
};

struct 任务筹办当前就绪请求_v2 final {
    std::uint32_t 合同版本 = 任务筹办当前就绪合同版本_v2;
    L2结构请求头 请求头;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务筹办轮次权威记录身份 筹办轮次;
    任务特征方法查找结果 方法查找结果;
    L2任务筹办正式选择基础材料_v2 正式选择基础材料;
    std::uint64_t 最大来源需求成员数 = 0;
    std::uint64_t 最大条件状态数 = 0;
    std::uint64_t 最大场景成员数 = 0;
    friend bool operator==(const 任务筹办当前就绪请求_v2&,
        const 任务筹办当前就绪请求_v2&) noexcept;
};

bool operator==(const 任务筹办当前就绪请求_v2&,
    const 任务筹办当前就绪请求_v2&) noexcept;

struct 任务筹办当前就绪结果_v2 final {
    任务筹办当前就绪结论 结论 = 任务筹办当前就绪结论::内部错误;
    std::uint64_t 共同事实截止 = 0;
    std::optional<L2普通方法事实> 当前方法;
    std::vector<L2需求事实> 来源需求组;
    std::vector<任务条件当前就绪证据> 条件证据组;
    std::vector<任务输入规格冻结项> 输入冻结组;
    std::vector<任务参数绑定冻结项> 参数冻结组;
    std::vector<任务限制条件复判证据> 限制证据组;
    std::optional<L2方法结果事实> 主轴结果;
    std::optional<L2方法动作入口事实> 动作入口;
    std::vector<任务冻结动作项> 完整有序冻结动作范围;
    std::optional<L2方法完整有序动作组读取结果> 方法动作组正式读回;
    std::optional<L2发布任务筹办正式选择结果_v2> 正式选择结果;
    friend bool operator==(const 任务筹办当前就绪结果_v2&,
        const 任务筹办当前就绪结果_v2&) = default;
};

} // namespace 海中鱼巣
