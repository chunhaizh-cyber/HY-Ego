#pragma once

#ifndef SELF_REALITY_EXECUTION_AUTHORIZATION_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "../L2自我治理结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 自我现实执行授权合同版本 = 1;
inline constexpr std::uint32_t 自我现实执行授权规则版本 = 1;

struct 自我现实执行授权请求 final {
    std::uint32_t 合同版本 = 自我现实执行授权合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
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
    std::uint32_t 授权规则版本 = 自我现实执行授权规则版本;
    std::uint64_t 发生时间 = 0;
    std::uint64_t 有效截止时间 = 0;
    friend bool operator==(const 自我现实执行授权请求&,
        const 自我现实执行授权请求&) = default;
};

struct 自我现实执行授权读取请求 final {
    std::uint32_t 合同版本 = 自我现实执行授权合同版本;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务执行轮次身份 执行轮次;
    L2自我现实执行授权身份 授权;
    std::uint64_t 非零读取截止 = 0;
    friend bool operator==(const 自我现实执行授权读取请求&,
        const 自我现实执行授权读取请求&) = default;
};

struct 自我现实执行授权收口请求 final {
    std::uint32_t 合同版本 = 自我现实执行授权合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 自我;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务执行轮次身份 执行轮次;
    L2自我现实执行授权身份 授权;
    自我现实执行授权收口种类 种类 = 自我现实执行授权收口种类::已失效;
    std::uint64_t 来源共同事实截止 = 0;
    std::uint64_t 发生时间 = 0;
    std::uint32_t 收口规则版本 = 1;
    friend bool operator==(const 自我现实执行授权收口请求&,
        const 自我现实执行授权收口请求&) = default;
};

enum class 自我现实执行授权状态 : std::uint8_t {
    已授权 = 1,
    精确重复 = 2,
    无需授权 = 3,
    拒绝 = 4,
    等待 = 5,
    已失效 = 6,
    已撤回 = 7,
    已消费 = 8,
    幂等冲突 = 9,
    当前性漂移 = 10,
    许可拒绝 = 11,
    资源失败 = 12,
    内部错误 = 13,
    已可能发布 = 14
};

#define 定义自我现实执行授权结果(类型名) \
    struct 类型名 final { \
        自我现实执行授权状态 状态 = 自我现实执行授权状态::内部错误; \
        std::optional<L2自我现实执行授权事实> 授权; \
        std::optional<L2自我现实执行授权收口事实> 收口; \
        std::optional<L2自我治理结构提交见证> 提交见证; \
        std::uint64_t 本次正式读回截止 = 0; \
        bool 成功() const noexcept { \
            const bool 授权成功 = (状态 == 自我现实执行授权状态::已授权 \
                    || 状态 == 自我现实执行授权状态::精确重复) \
                && 授权.has_value() && !收口 && !提交见证 \
                && 本次正式读回截止 != 0; \
            const bool 无需授权成功 = 状态 == 自我现实执行授权状态::无需授权 \
                && !授权 && !收口 && !提交见证; \
            const bool 收口成功 = (状态 == 自我现实执行授权状态::已消费 \
                    || 状态 == 自我现实执行授权状态::已撤回 \
                    || 状态 == 自我现实执行授权状态::已失效 \
                    || 状态 == 自我现实执行授权状态::精确重复) \
                && 授权.has_value() && 收口.has_value() && !提交见证 \
                && 本次正式读回截止 != 0; \
            return 授权成功 || 无需授权成功 || 收口成功; \
        } \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义自我现实执行授权结果(自我现实执行授权结果);
定义自我现实执行授权结果(自我现实执行授权读取结果);
定义自我现实执行授权结果(自我现实执行授权收口结果);

#undef 定义自我现实执行授权结果

} // namespace 海中鱼巣
