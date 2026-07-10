// 文件规则：运行消息协议只定义线程间请求、材料、回执和调度信号；不得定义线程类、承载机器事实或写业务结构。
module;

#include <cstdint>

export module 海中鱼巣.线程.运行消息协议;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 线程模块协议版本 = 1;

enum class 运行线程角色 : std::uint32_t {
    未知 = 0,
    运行宿主 = 1,
    任务管理 = 2,
    任务工作 = 3,
    缓存统计 = 4,
    事件日志 = 5,
    外设采样材料 = 6
};

enum class 运行消息类型 : std::uint32_t {
    未知 = 0,
    调度周期 = 1,
    停止 = 2,
    任务执行请求 = 3,
    任务拒绝回执 = 4,
    结果回执 = 5,
    缓存刷新 = 6,
    事件日志 = 7,
    外部材料 = 8
};

enum class 运行消息优先级 : std::uint32_t {
    低 = 0,
    普通 = 1,
    高 = 2,
    停止 = 3
};

enum class 入队拒绝原因 : std::uint32_t {
    无 = 0,
    容量为零 = 1,
    消息编号无效 = 2,
    队列已满 = 3,
    队列已停止 = 4,
    消息承载事实 = 5,
    幂等键冲突 = 6,
    句柄版本过期 = 7,
    同一任务序号倒退 = 8
};

struct 运行消息 {
    std::uint64_t 消息编号 = 0;
    运行消息类型 类型 = 运行消息类型::未知;
    运行线程角色 来源角色 = 运行线程角色::未知;
    运行线程角色 目标角色 = 运行线程角色::未知;
    std::uint64_t 目标任务 = 0;
    std::uint64_t 目标材料 = 0;
    std::uint64_t 句柄版本 = 0;
    std::uint64_t 当前句柄版本 = 0;
    std::uint64_t 发生时间戳 = 0;
    运行消息优先级 优先级 = 运行消息优先级::普通;
    std::uint64_t 幂等键 = 0;
    std::uint64_t 批次号 = 0;
    std::uint64_t 材料摘要 = 0;
    std::uint64_t 任务编号 = 0;
    std::uint64_t 任务序号 = 0;
    bool 只读材料说明 = true;
    bool 承载机器事实 = false;
};

struct 入队结果 {
    bool 已入队 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    入队拒绝原因 拒绝原因 = 入队拒绝原因::无;
};

struct 出队结果 {
    bool 有消息 = false;
    bool 队列已停止 = false;
    运行消息 消息;
};

}
