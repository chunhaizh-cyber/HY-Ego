#pragma once

#ifndef SELF_RESULT_CONTENTION_PRIORITY_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "../L2自我治理结构.数据.h"
#endif

namespace 海中鱼巣 {

struct 自我结果争用成员优先级候选 final {
    L2成员消费资格材料 消费资格;
    std::uint64_t 优先级序号 = 0;
    std::uint64_t 同级顺序序号 = 0;
    std::vector<稳定编码> 理由证据组;
    friend bool operator==(const 自我结果争用成员优先级候选&,
        const 自我结果争用成员优先级候选&) = default;
};

struct 自我结果争用成员优先级请求 final {
    std::uint32_t 合同版本 = 1;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务实际结果身份 正式结果;
    std::uint64_t 冻结成员集合版本 = 0;
    L2任务结果消费容量合同 容量合同;
    std::vector<自我结果争用成员优先级候选> 候选组;
    稳定编码 来源规则身份;
    std::uint32_t 关系规则版本 = 1;
    std::uint32_t 优先级规则版本 = 1;
    std::uint32_t 同级规则版本 = 1;
    std::uint64_t 来源共同事实截止 = 0;
    friend bool operator==(const 自我结果争用成员优先级请求&,
        const 自我结果争用成员优先级请求&) = default;
};

struct 自我结果争用优先级读取请求 final {
    std::uint32_t 合同版本 = 1;
    L2存在身份 自我;
    L2任务实际结果身份 正式结果;
    L2自我结果争用优先级批次身份 批次;
    std::uint64_t 非零读取截止 = 0;
    friend bool operator==(const 自我结果争用优先级读取请求&,
        const 自我结果争用优先级读取请求&) = default;
};

enum class 自我结果争用优先级状态 : std::uint8_t {
    已形成 = 1,
    精确重复 = 2,
    未找到 = 3,
    幂等冲突 = 4,
    入口拒绝 = 5,
    许可拒绝 = 6,
    成员漂移 = 7,
    证据不足 = 8,
    当前性漂移 = 9,
    资源失败 = 10,
    内部错误 = 11,
    已可能发布 = 12
};

struct 自我结果争用优先级结果 final {
    自我结果争用优先级状态 状态 = 自我结果争用优先级状态::内部错误;
    std::optional<自我结果争用优先级批次事实> 批次;
    std::vector<自我结果争用成员优先级事实> 成员事实组;
    std::optional<L2自我治理结构提交见证> 提交见证;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (状态 == 自我结果争用优先级状态::已形成
                || 状态 == 自我结果争用优先级状态::精确重复)
            && 批次.has_value() && !成员事实组.empty() && !提交见证
            && 批次->成员优先级.size() == 成员事实组.size()
            && 本次正式读回截止 != 0;
    }
    friend bool operator==(const 自我结果争用优先级结果&,
        const 自我结果争用优先级结果&) = default;
};

struct 自我结果争用优先级读取结果 final {
    自我结果争用优先级状态 状态 = 自我结果争用优先级状态::内部错误;
    std::optional<自我结果争用优先级批次事实> 批次;
    std::vector<自我结果争用成员优先级事实> 成员事实组;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (状态 == 自我结果争用优先级状态::已形成
                || 状态 == 自我结果争用优先级状态::精确重复)
            && 批次.has_value() && !成员事实组.empty()
            && 批次->成员优先级.size() == 成员事实组.size()
            && 本次正式读回截止 != 0;
    }
    friend bool operator==(const 自我结果争用优先级读取结果&,
        const 自我结果争用优先级读取结果&) = default;
};

} // namespace 海中鱼巣
