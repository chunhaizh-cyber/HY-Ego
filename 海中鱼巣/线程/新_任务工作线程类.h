#pragma once

#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <optional>

#include "../领域/新_任务类.h"

namespace 海中鱼巣 {

struct 新任务工作运行配置 final {
    std::uint64_t 邮箱容量 = 0;
    friend bool operator==(const 新任务工作运行配置&,
        const 新任务工作运行配置&) = default;
};

enum class 新任务工作生命周期 : std::uint8_t {
    未启动, 启动中, 等待工作, 处理工作, 停止中, 已停止, 启动失败, 内部错误
};

enum class 新任务工作操作状态 : std::uint8_t {
    已完成 = 1, 无变化, 入口拒绝, 已在运行, 尚未启动,
    等待超时, 不能等待自身, 所有者冲突, 资源失败, 内部错误
};

struct 新任务工作操作结果 final {
    新任务工作操作状态 状态 = 新任务工作操作状态::入口拒绝;
    新任务工作生命周期 生命周期 = 新任务工作生命周期::未启动;
};

enum class 新任务工作种类 : std::uint8_t {
    查找方法 = 1,
    检查条件 = 2,
    执行本能动作 = 3,
    获取本能结果 = 4
};

struct 新任务工作请求 final {
    std::uint64_t 工作编号 = 0;
    新任务工作种类 种类 = 新任务工作种类::查找方法;
    稳定编码 任务节点;
    // 两个W3工作分别只携带自己的强类型材料；W3-B不得携带动作请求。
    std::optional<新任务治理本能执行请求> 本能执行请求;
    std::optional<新任务治理结果获取请求> 本能结果获取请求;
    friend bool operator==(const 新任务工作请求&,
        const 新任务工作请求&) = default;
};

enum class 新任务工作处理状态 : std::uint8_t {
    已发布有候选 = 1,
    已发布完整无候选,
    已完成条件筹办,
    任务状态已变化,
    服务未就绪,
    查找未完成,
    候选发布未完成,
    本能动作已返回,
    本能结果获取已返回,
    需核查,
    停止取消,
    内部错误
};

struct 新任务工作处理结果 final {
    新任务工作请求 原请求;
    新任务工作处理状态 状态 = 新任务工作处理状态::内部错误;
    std::optional<新任务治理方法召回结果> 方法召回;
    std::optional<新任务治理条件筹办结果> 条件筹办;
    std::optional<新任务条件子需求提议> 条件子需求提议;
    std::optional<新任务治理本能执行结果> 本能执行;
    std::optional<新任务治理结果获取结果> 本能结果获取;
};

enum class 新任务工作接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 身份冲突, 任务已占用, 邮箱已满,
    尚未启动, 已停止接收, 入口拒绝, 资源失败, 内部错误
};

struct 新任务工作接收结果 final {
    新任务工作接收状态 状态 = 新任务工作接收状态::入口拒绝;
    std::shared_future<新任务工作处理结果> 处理结果;
};

struct 新任务工作快照 final {
    新任务工作生命周期 生命周期 = 新任务工作生命周期::未启动;
    std::uint64_t 活动工作数 = 0;
    bool 停止已请求 = false;
    bool 线程已完成 = false;
};

// 当前承载查找方法、条件/参数核验、W3-A动作和W3-B结果获取工作。
// 注入的任务服务必须比本对象存活更久；
// 完成通知只用于唤醒manager读取共享结果，不携带业务事实。
class 新_任务工作线程类 final {
public:
    explicit 新_任务工作线程类(
        新_任务类& 任务服务,
        std::function<void()> 完成通知 = {}) noexcept;
    ~新_任务工作线程类() noexcept;
    新_任务工作线程类(const 新_任务工作线程类&) = delete;
    新_任务工作线程类& operator=(const 新_任务工作线程类&) = delete;
    新_任务工作线程类(新_任务工作线程类&&) = delete;
    新_任务工作线程类& operator=(新_任务工作线程类&&) = delete;

    新任务工作操作结果 启动(
        const 新任务工作运行配置& 配置, std::uint64_t 等待毫秒) noexcept;
    新任务工作接收结果 提交工作(const 新任务工作请求& 请求) noexcept;
    // manager完成独立读回后确认交接，随后本工作才释放活动容量。
    新任务工作操作状态 确认结果已接收(
        std::uint64_t 工作编号, 稳定编码 任务节点) noexcept;
    新任务工作操作结果 请求停止() noexcept;
    新任务工作操作结果 等待停止(std::uint64_t 等待毫秒) noexcept;
    新任务工作快照 读取快照() const noexcept;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
