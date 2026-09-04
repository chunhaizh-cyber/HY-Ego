#pragma once

#ifndef L2_SELF_GOVERNANCE_STRUCTURE_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#include "L2任务结构.数据.h"
#endif

namespace 海中鱼巣 {

enum class 自我现实执行授权收口种类 : std::uint8_t {
    已消费 = 1,
    已撤回 = 2,
    已失效 = 3
};

struct L2自我现实执行授权事实 final {
    L2自我现实执行授权身份 身份;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务筹办轮次权威记录身份 筹办轮次;
    L2任务执行轮次身份 执行轮次;
    L2任务执行冻结身份 冻结;
    std::uint64_t 冻结版本 = 0;
    L2实例方法身份 实例方法;
    L2方法身份 方法;
    稳定编码 方法内容版本;
    std::vector<任务冻结动作项> 有序冻结动作范围;
    std::uint64_t 来源共同事实截止 = 0;
    std::uint32_t 授权规则版本 = 1;
    std::uint64_t 发生时间 = 0;
    std::uint64_t 有效截止时间 = 0;
    L2生命周期 生命周期;
    friend bool operator==(const L2自我现实执行授权事实&,
        const L2自我现实执行授权事实&) = default;
};

struct L2自我现实执行授权收口事实 final {
    L2自我现实执行授权收口身份 身份;
    L2自我现实执行授权身份 授权;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务执行轮次身份 执行轮次;
    自我现实执行授权收口种类 种类 = 自我现实执行授权收口种类::已失效;
    std::uint64_t 发生时间 = 0;
    std::uint32_t 收口规则版本 = 1;
    L2生命周期 生命周期;
    friend bool operator==(const L2自我现实执行授权收口事实&,
        const L2自我现实执行授权收口事实&) = default;
};

struct 自我结果争用优先级批次事实 final {
    L2自我结果争用优先级批次身份 身份;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务实际结果身份 正式结果;
    std::uint64_t 冻结成员集合版本 = 0;
    L2任务结果消费容量合同 容量合同;
    std::vector<L2自我结果争用成员优先级身份> 成员优先级;
    稳定编码 来源规则身份;
    std::uint32_t 关系规则版本 = 1;
    std::uint32_t 优先级规则版本 = 1;
    std::uint32_t 同级规则版本 = 1;
    std::uint64_t 来源共同事实截止 = 0;
    L2生命周期 生命周期;
    friend bool operator==(const 自我结果争用优先级批次事实&,
        const 自我结果争用优先级批次事实&) = default;
};

struct 自我结果争用成员优先级事实 final {
    L2自我结果争用成员优先级身份 身份;
    L2自我结果争用优先级批次身份 批次;
    L2成员消费资格材料 消费资格;
    std::uint64_t 优先级序号 = 0;
    std::uint64_t 同级顺序序号 = 0;
    std::vector<稳定编码> 理由证据组;
    L2生命周期 生命周期;
    friend bool operator==(const 自我结果争用成员优先级事实&,
        const 自我结果争用成员优先级事实&) = default;
};

struct L2自我结果争用优先级完整事实 final {
    自我结果争用优先级批次事实 批次;
    std::vector<自我结果争用成员优先级事实> 成员事实组;
    friend bool operator==(const L2自我结果争用优先级完整事实&,
        const L2自我结果争用优先级完整事实&) = default;
};

struct 自我任务后继需求依据 final {
    L2需求身份 需求;
    L2状态身份 当前状态;
    稳定编码 当前优先级事实;
    friend bool operator==(const 自我任务后继需求依据&,
        const 自我任务后继需求依据&) = default;
};

struct 自我任务后继安全依据 final {
    稳定编码 安全事实;
    friend bool operator==(const 自我任务后继安全依据&,
        const 自我任务后继安全依据&) = default;
};

struct 自我任务后继整体治理依据 final {
    稳定编码 治理事实;
    friend bool operator==(const 自我任务后继整体治理依据&,
        const 自我任务后继整体治理依据&) = default;
};

struct L2自我任务后继决议事实 final {
    稳定编码 身份;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 已收束任务轮次;
    L2任务轮次结算身份 轮次结算;
    自我任务后继指令 指令 = 自我任务后继指令::保持等待;
    std::vector<自我任务后继需求依据> 当前需求依据;
    std::vector<自我任务后继安全依据> 当前安全依据;
    std::vector<自我任务后继整体治理依据> 当前整体治理依据;
    std::uint64_t 来源共同事实截止 = 0;
    std::uint32_t 决议规则版本 = 1;
    std::uint64_t 发生时间 = 0;
    L2生命周期 生命周期;
    friend bool operator==(const L2自我任务后继决议事实&,
        const L2自我任务后继决议事实&) = default;
};

enum class L2自我治理结构事实种类 : std::uint8_t {
    现实执行授权 = 1,
    现实执行授权收口 = 2,
    结果争用优先级批次 = 3,
    任务后继决议 = 4
};

using L2自我治理结构事实载荷 = std::variant<
    L2自我现实执行授权事实,
    L2自我现实执行授权收口事实,
    L2自我结果争用优先级完整事实,
    L2自我任务后继决议事实>;

struct L2自我治理结构提交见证 final {
    L1结构所有者身份 所有者;
    L2结构幂等身份 幂等身份;
    std::uint64_t 提交事实代次 = 0;
    std::uint64_t 提交结果序号 = 0;
    friend bool operator==(const L2自我治理结构提交见证&,
        const L2自我治理结构提交见证&) = default;
};

struct L2自我治理结构提交请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2自我治理结构事实种类 种类 = L2自我治理结构事实种类::现实执行授权;
    L2自我治理结构事实载荷 候选事实;
    friend bool operator==(const L2自我治理结构提交请求&,
        const L2自我治理结构提交请求&) = default;
};

struct L2自我治理结构读取请求 final {
    std::uint32_t 合同版本 = 1;
    L2存在身份 自我;
    L2自我治理结构事实种类 种类 = L2自我治理结构事实种类::现实执行授权;
    稳定编码 事实身份;
    std::uint64_t 非零读取截止 = 0;
    friend bool operator==(const L2自我治理结构读取请求&,
        const L2自我治理结构读取请求&) = default;
};

struct L2自我治理结构提交结果 final {
    L2结构结果头 结果头;
    std::optional<L2自我治理结构事实载荷> 事实;
    std::optional<L2自我治理结构提交见证> 提交见证;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (结果头.状态 == L2结构状态::已提交
                || 结果头.状态 == L2结构状态::精确重复)
            && 事实.has_value() && !提交见证 && 本次正式读回截止 != 0;
    }
    friend bool operator==(const L2自我治理结构提交结果&,
        const L2自我治理结构提交结果&) = default;
};

struct L2自我治理结构读取结果 final {
    L2结构结果头 结果头;
    std::optional<L2自我治理结构事实载荷> 事实;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 事实.has_value() && 本次正式读回截止 != 0;
    }
    friend bool operator==(const L2自我治理结构读取结果&,
        const L2自我治理结构读取结果&) = default;
};

} // namespace 海中鱼巣
