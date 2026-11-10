#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "新_场景类.h"
#include "新_本能函数集.h"
#include "新_方法公共类型.h"
#include "任务执行采用.数据.h"

namespace 海中鱼巣 {

enum class 新方法问题种类 : std::uint8_t {
    方向不符 = 1, 值域不符, 预计变化未发生, 发生计划外变化,
    // 尝试执行（5260 §5 第8项，R96）：所采用配对结果为空，执行数据直接交学习补全（TRIAL-LEARNING-DIRECT）。
    尝试执行
};
// TRIAL-LEARNING-DIRECT（2026-11-10）：执行后直接交学习的内存材料，不再复制进模拟世界归档。
struct 新方法问题反馈材料 final {
    稳定编码 任务节点, 执行实例节点, 来源抽象方法节点;
    新任务本能采用材料 任务采用;
    std::optional<新本能函数本次条件结果对> 本次运行对;
    稳定编码 影响评估节点;
    std::int64_t 原方向验证值 = 0, 原值域验证值 = 0;
    std::vector<新方法问题种类> 问题组;
    std::vector<稳定编码> 符合预测状态组, 预计但未发生项组, 计划外状态组;
    std::vector<新状态信息> 实际状态组;
    friend bool operator==(const 新方法问题反馈材料&, const 新方法问题反馈材料&) = default;
};
// 键为执行实例节点；材料随消息交付，任务侧不再保留。
struct 新方法问题学习消息 final {
    稳定编码 方法节点, 任务节点, 执行实例节点;
    std::shared_ptr<const 新方法问题反馈材料> 材料;
    friend bool operator==(const 新方法问题学习消息& a, const 新方法问题学习消息& b) {
        return a.方法节点 == b.方法节点 && a.任务节点 == b.任务节点 && a.执行实例节点 == b.执行实例节点
            && (a.材料 == b.材料 || (a.材料 && b.材料 && *a.材料 == *b.材料));
    }
};

} // namespace 海中鱼巣
