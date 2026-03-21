module;

#include <cstdint>
#include <string>
#include <vector>

export module 原生学习兜底函数定义模块_v0;

import 基础数据类型模块;
import 主信息定义模块;
import 本能动作管理模块;

// ============================================================================
// 原生学习兜底函数定义模块_v0
// ----------------------------------------------------------------------------
// 规范：
//  - 本模块属于“本能函数模块相关模块”，但只承载定义层与桥接层对象。
//  - 不承载具体业务实现；具体兜底学习动作应放在自我本能方法相关模块内。
//  - 不另起第二套公共动作枚举；统一复用 枚举_本能动作ID + 动作句柄.本能ID。
// ============================================================================

export enum class 枚举_学习域动作分组_v0 : std::uint8_t {
    未定义 = 0,
    学习总入口 = 1,
    学习沙盒 = 2,
    触发与许可 = 10,
    检索与复用 = 11,
    本能试探 = 12,
    验证与回退 = 13,
    总结与沉淀 = 14,
    退出与移交 = 15
};

export struct 结构_学习域本能动作分段_v0 {
    static constexpr std::uint32_t 学习总入口_起 = 3000;
    static constexpr std::uint32_t 学习总入口_止 = 3000;

    static constexpr std::uint32_t 学习沙盒_起 = 3100;
    static constexpr std::uint32_t 学习沙盒_止 = 3199;

    static constexpr std::uint32_t 触发与许可_起 = 3200;
    static constexpr std::uint32_t 触发与许可_止 = 3299;

    static constexpr std::uint32_t 检索与复用_起 = 3300;
    static constexpr std::uint32_t 检索与复用_止 = 3399;

    static constexpr std::uint32_t 本能试探_起 = 3400;
    static constexpr std::uint32_t 本能试探_止 = 3499;

    static constexpr std::uint32_t 验证与回退_起 = 3500;
    static constexpr std::uint32_t 验证与回退_止 = 3599;

    static constexpr std::uint32_t 总结与沉淀_起 = 3600;
    static constexpr std::uint32_t 总结与沉淀_止 = 3699;

    static constexpr std::uint32_t 退出与移交_起 = 3700;
    static constexpr std::uint32_t 退出与移交_止 = 3799;
};

export constexpr I64 转为本能ID_v0(枚举_本能动作ID 本能动作) noexcept
{
    return static_cast<I64>(本能动作);
}

export constexpr bool 是学习域本能动作ID_v0(I64 本能ID) noexcept
{
    return 本能ID >= 结构_学习域本能动作分段_v0::学习总入口_起
        && 本能ID <= 结构_学习域本能动作分段_v0::退出与移交_止;
}

export constexpr bool 是学习域本能动作ID_v0(枚举_本能动作ID 本能动作) noexcept
{
    return 是学习域本能动作ID_v0(转为本能ID_v0(本能动作));
}

export constexpr bool 是原生学习兜底本能动作ID_v0(I64 本能ID) noexcept
{
    return 本能ID >= 结构_学习域本能动作分段_v0::触发与许可_起
        && 本能ID <= 结构_学习域本能动作分段_v0::退出与移交_止;
}

export constexpr bool 是原生学习兜底本能动作ID_v0(枚举_本能动作ID 本能动作) noexcept
{
    return 是原生学习兜底本能动作ID_v0(转为本能ID_v0(本能动作));
}

export constexpr 枚举_学习域动作分组_v0 计算学习域动作分组_v0(I64 本能ID) noexcept
{
    using 分段 = 结构_学习域本能动作分段_v0;
    if (本能ID >= 分段::学习总入口_起 && 本能ID <= 分段::学习总入口_止) {
        return 枚举_学习域动作分组_v0::学习总入口;
    }
    if (本能ID >= 分段::学习沙盒_起 && 本能ID <= 分段::学习沙盒_止) {
        return 枚举_学习域动作分组_v0::学习沙盒;
    }
    if (本能ID >= 分段::触发与许可_起 && 本能ID <= 分段::触发与许可_止) {
        return 枚举_学习域动作分组_v0::触发与许可;
    }
    if (本能ID >= 分段::检索与复用_起 && 本能ID <= 分段::检索与复用_止) {
        return 枚举_学习域动作分组_v0::检索与复用;
    }
    if (本能ID >= 分段::本能试探_起 && 本能ID <= 分段::本能试探_止) {
        return 枚举_学习域动作分组_v0::本能试探;
    }
    if (本能ID >= 分段::验证与回退_起 && 本能ID <= 分段::验证与回退_止) {
        return 枚举_学习域动作分组_v0::验证与回退;
    }
    if (本能ID >= 分段::总结与沉淀_起 && 本能ID <= 分段::总结与沉淀_止) {
        return 枚举_学习域动作分组_v0::总结与沉淀;
    }
    if (本能ID >= 分段::退出与移交_起 && 本能ID <= 分段::退出与移交_止) {
        return 枚举_学习域动作分组_v0::退出与移交;
    }
    return 枚举_学习域动作分组_v0::未定义;
}

export constexpr 枚举_学习域动作分组_v0 计算学习域动作分组_v0(枚举_本能动作ID 本能动作) noexcept
{
    return 计算学习域动作分组_v0(转为本能ID_v0(本能动作));
}

export inline 结构体_动作句柄 构造学习域动作句柄_v0(枚举_本能动作ID 本能动作)
{
    结构体_动作句柄 句柄{};
    句柄.类型 = 枚举_动作句柄类型::本能函数ID;
    句柄.本能ID = 转为本能ID_v0(本能动作);
    return 句柄;
}

export inline bool 是学习域动作句柄_v0(const 结构体_动作句柄& 动作句柄) noexcept
{
    return 动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
        && 是学习域本能动作ID_v0(动作句柄.本能ID);
}

export struct 结构_兜底学习上下文
{
    需求节点类* 来源需求 = nullptr;
    任务节点类* 当前任务头结点 = nullptr;
    任务节点类* 当前步骤节点 = nullptr;
    任务节点类* 当前结果节点 = nullptr;

    场景节点类* 当前场景 = nullptr;

    存在节点类* 任务虚拟存在 = nullptr;
    存在节点类* 方法虚拟存在根 = nullptr;

    方法节点类* 当前方法首节点 = nullptr;

    I64 安全值_Q10000 = 0;
    I64 服务值_Q10000 = 0;
    I64 需求值_Q10000 = 0;

    I64 当前细分预算 = 0;
    I64 当前路径预算 = 0;
    I64 当前回退预算 = 0;
    I64 当前检查预算 = 0;

    std::string 调用点{};
};

// 最小闭环接口约束：
//  - 这里只定义签名和接口槽位，不提供实现。
//  - 具体实现建议由自我本能方法相关模块给出。
export using 函数_判断任务是否缺方法_v0 =
    bool(*)(const 结构_兜底学习上下文& 上下文);

export using 函数_判断是否允许进入兜底学习_v0 =
    bool(*)(const 结构_兜底学习上下文& 上下文);

export using 函数_计算本轮学习预算_v0 =
    void(*)(结构_兜底学习上下文& 上下文);

export using 函数_检索相似方法_v0 =
    std::vector<方法节点类*>(*)(const 结构_兜底学习上下文& 上下文);

export using 函数_降解任务为最小可试探步骤_v0 =
    std::vector<任务节点类*>(*)(const 结构_兜底学习上下文& 上下文);

export using 函数_生成单步候选动作集_v0 =
    std::vector<结构体_动作句柄>(*)(const 结构_兜底学习上下文& 上下文);

export using 函数_选择下一试探动作_v0 =
    结构体_动作句柄(*)(const 结构_兜底学习上下文& 上下文,
        const std::vector<结构体_动作句柄>& 候选动作集);

export using 函数_执行单步试探动作_v0 =
    结构_任务返回消息(*)(const 结构_兜底学习上下文& 上下文,
        const 结构体_动作句柄& 动作句柄);

export using 函数_验证局部结果_v0 =
    bool(*)(const 结构_兜底学习上下文& 上下文,
        const 结构_任务返回消息& 返回消息);

export using 函数_回滚最近一步_v0 =
    bool(*)(const 结构_兜底学习上下文& 上下文);

export using 函数_登记控制因果_v0 =
    因果节点类* (*)(const 结构_兜底学习上下文& 上下文,
        const 结构_任务返回消息& 返回消息);

export using 函数_从成功片段提炼临时方法_v0 =
    方法节点类* (*)(const 结构_兜底学习上下文& 上下文);

export struct 结构_原生学习兜底最小闭环接口_v0
{
    函数_判断任务是否缺方法_v0 判断任务是否缺方法 = nullptr;
    函数_判断是否允许进入兜底学习_v0 判断是否允许进入兜底学习 = nullptr;
    函数_计算本轮学习预算_v0 计算本轮学习预算 = nullptr;
    函数_检索相似方法_v0 检索相似方法 = nullptr;
    函数_降解任务为最小可试探步骤_v0 降解任务为最小可试探步骤 = nullptr;
    函数_生成单步候选动作集_v0 生成单步候选动作集 = nullptr;
    函数_选择下一试探动作_v0 选择下一试探动作 = nullptr;
    函数_执行单步试探动作_v0 执行单步试探动作 = nullptr;
    函数_验证局部结果_v0 验证局部结果 = nullptr;
    函数_回滚最近一步_v0 回滚最近一步 = nullptr;
    函数_登记控制因果_v0 登记控制因果 = nullptr;
    函数_从成功片段提炼临时方法_v0 从成功片段提炼临时方法 = nullptr;

    bool 完整() const noexcept
    {
        return 判断任务是否缺方法
            && 判断是否允许进入兜底学习
            && 计算本轮学习预算
            && 检索相似方法
            && 降解任务为最小可试探步骤
            && 生成单步候选动作集
            && 选择下一试探动作
            && 执行单步试探动作
            && 验证局部结果
            && 回滚最近一步
            && 登记控制因果
            && 从成功片段提炼临时方法;
    }
};
