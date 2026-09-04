#pragma once

#ifndef L2_TASK_RESULT_CONSUMPTION_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

struct L2正式成员分配优先级引用 final {
    L2自我结果争用成员优先级身份 身份;
    L2成员消费资格材料 消费资格;
    std::uint64_t 优先级序号 = 0;
    std::uint64_t 同级顺序序号 = 0;
    friend bool operator==(const L2正式成员分配优先级引用&,
        const L2正式成员分配优先级引用&) = default;
};

enum class L2成员消费分配状态 : std::uint8_t {
    共同引用 = 1, 已分配 = 2, 未获分配 = 3, 结果已耗尽 = 4
};

struct L2成员消费分配结果事实 final {
    L2成员消费资格材料 消费资格;
    std::uint64_t 处理顺序 = 0;
    std::optional<L2正式成员分配优先级引用> 正式优先级;
    L2成员消费分配状态 成员状态 = L2成员消费分配状态::共同引用;
    std::uint64_t 分配前剩余 = 0;
    std::uint64_t 实际分配单位 = 0;
    std::uint64_t 分配后剩余 = 0;
    friend bool operator==(const L2成员消费分配结果事实&,
        const L2成员消费分配结果事实&) = default;
};

struct L2任务结果消费分配记录事实 final {
    L2任务结果消费分配记录身份 身份;
    L2任务身份 任务;
    L2任务轮次身份 任务轮次;
    L2任务实际结果身份 正式结果;
    L2任务结果消费形态 消费形态 = L2任务结果消费形态::可重复引用;
    std::uint64_t 冻结成员集合版本 = 0;
    L2任务结果消费容量合同 容量合同;
    std::vector<L2成员消费资格材料> 冻结成员;
    std::optional<L2自我结果争用优先级批次身份> 优先级批次;
    std::vector<L2正式成员分配优先级引用> 正式优先级组;
    std::vector<L2成员消费分配结果事实> 逐成员结果;
    std::uint64_t 初始剩余 = 0;
    std::uint64_t 最终剩余 = 0;
    std::uint64_t 来源共同事实截止 = 0;
    std::uint32_t 关系规则版本 = 1;
    std::uint32_t 优先级规则版本 = 1;
    std::uint32_t 同级规则版本 = 1;
    L2生命周期 生命周期;
    friend bool operator==(const L2任务结果消费分配记录事实&,
        const L2任务结果消费分配记录事实&) = default;
};

struct L2提交任务结果消费分配记录请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务结果消费分配记录事实 候选记录;
    friend bool operator==(const L2提交任务结果消费分配记录请求&,
        const L2提交任务结果消费分配记录请求&) = default;
};

struct L2任务结果消费分配记录读取请求 final {
    std::uint32_t 合同版本 = 1;
    L2任务结果消费分配记录身份 记录;
    L2任务实际结果身份 正式结果;
    std::uint64_t 非零读取截止 = 0;
};

enum class L2任务结果消费分配记录状态 : std::uint8_t {
    已形成 = 1, 精确重复 = 2, 未找到 = 3, 幂等冲突 = 4,
    入口拒绝 = 5, 许可拒绝 = 6, 当前性漂移 = 7, 成员漂移 = 8,
    优先级合同缺失 = 9, 容量合同缺失 = 10, 资源失败 = 11,
    内部错误 = 12, 已可能发布 = 13
};

struct L2任务结果消费分配提交见证 final {
    L1结构所有者身份 所有者;
    L2结构幂等身份 幂等身份;
    std::uint64_t 提交事实代次 = 0;
    std::uint64_t 提交结果序号 = 0;
};

struct L2提交任务结果消费分配记录结果 final {
    L2任务结果消费分配记录状态 状态 = L2任务结果消费分配记录状态::内部错误;
    std::optional<L2任务结果消费分配记录事实> 记录;
    std::optional<L2任务结果消费分配提交见证> 提交见证;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (状态 == L2任务结果消费分配记录状态::已形成
                || 状态 == L2任务结果消费分配记录状态::精确重复)
            && 记录 && !提交见证 && 本次正式读回截止 != 0;
    }
};

struct L2任务结果消费分配记录读取结果 final {
    L2任务结果消费分配记录状态 状态 = L2任务结果消费分配记录状态::内部错误;
    std::optional<L2任务结果消费分配记录事实> 记录;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept {
        return (状态 == L2任务结果消费分配记录状态::已形成
                || 状态 == L2任务结果消费分配记录状态::精确重复)
            && 记录 && 本次正式读回截止 != 0;
    }
};

}
