#pragma once

#ifndef SELF_TASK_SUCCESSOR_DECISION_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "../L2自我治理结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 自我任务后继决议合同版本 = 1;
inline constexpr std::uint32_t 自我任务后继决议规则版本 = 1;

struct 自我任务后继决议请求 final {
    std::uint32_t 合同版本 = 自我任务后继决议合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 已收束任务轮次;
    L2任务轮次结算身份 轮次结算;
    自我任务后继指令 指令 = 自我任务后继指令::保持等待;
    std::vector<自我任务后继需求依据> 当前需求依据;
    std::vector<自我任务后继安全依据> 当前安全依据;
    std::vector<自我任务后继整体治理依据> 当前整体治理依据;
    std::uint64_t 来源共同事实截止 = 0;
    std::uint32_t 决议规则版本 = 自我任务后继决议规则版本;
    std::uint64_t 发生时间 = 0;
    friend bool operator==(const 自我任务后继决议请求&,
        const 自我任务后继决议请求&) = default;
};

struct 自我任务后继决议读取请求 final {
    std::uint32_t 合同版本 = 自我任务后继决议合同版本;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 已收束任务轮次;
    稳定编码 决议身份;
    std::uint64_t 非零读取截止 = 0;
    friend bool operator==(const 自我任务后继决议读取请求&,
        const 自我任务后继决议读取请求&) = default;
};

enum class 自我任务后继决议状态 : std::uint8_t {
    已形成 = 1,
    精确重复 = 2,
    未找到 = 3,
    幂等冲突 = 4,
    入口拒绝 = 5,
    许可拒绝 = 6,
    当前性漂移 = 7,
    材料不足 = 8,
    资源失败 = 9,
    内部错误 = 10,
    已可能发布 = 11
};

struct 自我任务后继决议结果 final {
    自我任务后继决议状态 状态 = 自我任务后继决议状态::内部错误;
    std::optional<L2自我任务后继决议事实> 决议;
    std::optional<L2自我治理结构提交见证> 提交见证;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (状态 == 自我任务后继决议状态::已形成
                || 状态 == 自我任务后继决议状态::精确重复)
            && 决议.has_value() && !提交见证 && 本次正式读回截止 != 0;
    }
    friend bool operator==(const 自我任务后继决议结果&,
        const 自我任务后继决议结果&) = default;
};

struct 自我任务后继决议读取结果 final {
    自我任务后继决议状态 状态 = 自我任务后继决议状态::内部错误;
    std::optional<L2自我任务后继决议事实> 决议;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (状态 == 自我任务后继决议状态::已形成
                || 状态 == 自我任务后继决议状态::精确重复)
            && 决议.has_value() && 本次正式读回截止 != 0;
    }
    friend bool operator==(const 自我任务后继决议读取结果&,
        const 自我任务后继决议读取结果&) = default;
};

} // namespace 海中鱼巣
