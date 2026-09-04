#pragma once

#ifndef L2_DEMAND_SETTLEMENT_NO_INCLUDES
#include <cstdint>
#include <optional>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

enum class L2需求本轮核算结论 : std::uint8_t {
    已满足 = 1, 未满足 = 2, 未获分配 = 3, 材料不足 = 4, 不可比较 = 5
};

struct L2需求本轮核算事实 final {
    L2需求本轮核算身份 身份;
    L2需求身份 需求;
    L2任务身份 来源任务;
    L2任务轮次身份 来源任务轮次;
    L2任务实际结果身份 来源正式结果;
    L2任务结果消费分配记录身份 消费分配记录;
    L2成员消费资格材料 消费资格;
    L2成员消费分配状态 分配状态 = L2成员消费分配状态::未获分配;
    L2需求本轮核算结论 结论 = L2需求本轮核算结论::材料不足;
    std::optional<L2需求满足记录身份> 正式满足记录;
    L2状态身份 比较时实际状态;
    L2目标状态合同身份 目标状态合同;
    稳定编码 比较注册;
    std::uint64_t 来源共同事实截止 = 0;
    std::uint32_t 核算规则版本 = 1;
    L2生命周期 生命周期;
    friend bool operator==(const L2需求本轮核算事实&,
        const L2需求本轮核算事实&) = default;
};

struct L2需求本轮核算写入请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2需求本轮核算事实 候选事实;
};

struct L2需求本轮核算读取请求 final {
    std::uint32_t 合同版本 = 1;
    L2需求本轮核算身份 核算;
    L2需求身份 需求;
    L2任务身份 来源任务;
    L2任务轮次身份 来源任务轮次;
    std::uint64_t 非零读取截止 = 0;
};

enum class L2需求本轮核算写入状态 : std::uint8_t {
    已形成 = 1, 精确重复 = 2, 幂等冲突 = 3, 入口拒绝 = 4,
    许可拒绝 = 5, 当前性漂移 = 6, 分配记录漂移 = 7,
    资源失败 = 8, 内部错误 = 9, 已可能发布 = 10
};

enum class L2需求本轮核算读取状态 : std::uint8_t {
    已读取 = 1, 未找到 = 2, 入口拒绝 = 3,
    许可拒绝 = 4, 资源失败 = 5, 内部错误 = 6
};

struct L2需求本轮核算提交见证 final {
    L1结构所有者身份 所有者;
    L2结构幂等身份 幂等身份;
    std::uint64_t 提交事实代次 = 0;
    std::uint64_t 提交结果序号 = 0;
};

struct L2需求本轮核算写入结果 final {
    L2需求本轮核算写入状态 状态 = L2需求本轮核算写入状态::内部错误;
    std::optional<L2需求本轮核算事实> 核算事实;
    std::optional<L2需求本轮核算提交见证> 提交见证;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (状态 == L2需求本轮核算写入状态::已形成
                || 状态 == L2需求本轮核算写入状态::精确重复)
            && 核算事实 && !提交见证 && 本次正式读回截止 != 0;
    }
};

struct L2需求本轮核算读取结果 final {
    L2需求本轮核算读取状态 状态 = L2需求本轮核算读取状态::内部错误;
    std::optional<L2需求本轮核算事实> 核算事实;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return 状态 == L2需求本轮核算读取状态::已读取
            && 核算事实 && 本次正式读回截止 != 0;
    }
};

}
