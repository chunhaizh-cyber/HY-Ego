module;

#include <string>

export module 任务管理模块_v0;

import 元层核心模块;
import 主信息定义模块;
import 任务模块;

export struct 结构_任务管理结果
{
    bool 成功 = false;
    std::string 错误摘要;
    需求节点类* 需求 = nullptr;
    任务节点类* 任务头结点 = nullptr;
    任务节点类* 步骤节点 = nullptr;
    任务节点类* 结果节点 = nullptr;
    方法节点类* 方法首节点 = nullptr;
};

export struct 结构_任务分解调制结果_v0
{
    bool 成功 = false;
    std::string 错误摘要;
    double 当前安全值 = 50.0;
    double 当前服务值 = 0.0;
    double 安全下限 = 1.0;
    double 风险权重 = 0.0;
    int 分解深度上限 = 1;
    bool 允许探索新子任务 = false;
    int 执行范围收缩级别 = 0;
    int 备用方法数量 = 1;
    int 自检强度 = 0;
    任务类::结构_创建步骤参数 调制后步骤参数{};
};

export struct 结构_学习任务进化输入_v0
{
    std::int64_t 基线可用方法数 = 0;
    std::int64_t 当前可用方法数 = 0;
    std::int64_t 基线稳定方法数 = 0;
    std::int64_t 当前稳定方法数 = 0;
    std::int64_t 基线稳定度总分 = 0;
    std::int64_t 当前稳定度总分 = 0;
    std::int64_t 待进化方法数 = 0;
    bool 有待处理请求 = false;
};

export struct 结构_学习任务进化结果_v0
{
    bool 成功 = false;
    std::string 错误摘要;
    bool 可用方法已增加 = false;
    bool 方法稳定性已提升 = false;
    bool 应继续进化 = false;
    std::int64_t 可用方法增量 = 0;
    std::int64_t 稳定方法增量 = 0;
    std::int64_t 稳定度总分增量 = 0;
};

export class 任务管理器_v0 final
{
public:
    结构_任务管理结果 创建任务头(
        需求节点类* 需求,
        const 任务类::结构_创建任务头参数& 参数 = {},
        任务节点类* 父节点 = nullptr,
        const std::string& 调用点 = "任务管理器_v0::创建任务头");

    结构_任务管理结果 确保需求任务头(
        需求节点类* 需求,
        const 任务类::结构_创建任务头参数& 参数 = {},
        任务节点类* 父节点 = nullptr,
        const std::string& 调用点 = "任务管理器_v0::确保需求任务头");

    结构_任务管理结果 分解任务(
        任务节点类* 任务头结点,
        const 任务类::结构_创建步骤参数& 参数 = {},
        const std::string& 调用点 = "任务管理器_v0::分解任务");

    bool 绑定方法(
        任务节点类* 步骤节点,
        方法节点类* 方法首节点,
        bool 设为当前方法 = true,
        const std::string& 调用点 = "任务管理器_v0::绑定方法");

    bool 开始任务(
        任务节点类* 任务头结点,
        const std::string& 调用点 = "任务管理器_v0::开始任务");

    结构_任务分解调制结果_v0 计算任务分解调制(
        const 结构_元实体& 元任务实体,
        const 任务类::结构_创建步骤参数& 基础参数 = {},
        const std::string& 调用点 = "任务管理器_v0::计算任务分解调制") const;

    结构_任务管理结果 失败任务(
        任务节点类* 任务头结点,
        const std::string& 调用点 = "任务管理器_v0::失败任务");

    结构_任务管理结果 完成任务(
        任务节点类* 任务头结点,
        const std::string& 调用点 = "任务管理器_v0::完成任务");

    结构_任务管理结果 写入反馈(
        任务节点类* 任务头结点,
        const 任务类::结构_创建结果参数& 参数 = {},
        const std::string& 调用点 = "任务管理器_v0::写入反馈");

    结构_学习任务进化结果_v0 推进学习任务进化(
        任务节点类* 学习任务头结点,
        const 结构_学习任务进化输入_v0& 输入 = {},
        const std::string& 调用点 = "任务管理器_v0::推进学习任务进化");
};
