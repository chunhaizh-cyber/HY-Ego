#pragma once

#include <cstdint>
#include <future>
#include <memory>
#include <optional>
#include "../业务/新_世界树类.h"

namespace 海中鱼巣 {

struct 新任务管理运行配置 final {
    std::uint64_t 邮箱容量 = 0;
    std::uint64_t 资源重试间隔毫秒 = 0;
    friend bool operator==(const 新任务管理运行配置&,
        const 新任务管理运行配置&) = default;
};

enum class 新任务管理生命周期 : std::uint8_t {
    未启动, 启动中, 等待消息, 处理消息, 停止中, 已停止, 启动失败, 内部错误
};

enum class 新任务管理操作状态 : std::uint8_t {
    已完成 = 1, 无变化, 入口拒绝, 已在运行, 尚未启动,
    等待超时, 不能等待自身, 服务未就绪, 所有者冲突, 资源失败, 内部错误
};

struct 新任务管理操作结果 final {
    新任务管理操作状态 状态 = 新任务管理操作状态::入口拒绝;
    新任务管理生命周期 生命周期 = 新任务管理生命周期::未启动;
};

enum class 新任务承接依据 : std::uint8_t {
    显式请求 = 1,
    自我首次复核 = 2,
    自我状态变化复核 = 3
};

// 唯一业务用途是承接需求；由自我决议的调用链提供具体需求。
struct 新任务承接消息 final {
    std::uint64_t 消息编号 = 0;
    稳定编码 需求节点;
    新任务承接依据 依据 = 新任务承接依据::显式请求;
    // 只有“自我状态变化复核”携带正式状态节点；它同时作为该根的
    // 单调决议身份，防止旧状态通知在任务终结后重新启用任务。
    std::optional<稳定编码> 来源状态节点;
    friend bool operator==(const 新任务承接消息&, const 新任务承接消息&) = default;
};

enum class 新任务承接处理状态 : std::uint8_t {
    已承接 = 1, 已重新启用, 既有终结, 需求当前已满足,
    入口拒绝, 服务未就绪, 需核查, 停止取消, 内部错误
};

struct 新任务承接处理结果 final {
    新任务承接消息 原消息;
    新任务承接处理状态 状态 = 新任务承接处理状态::内部错误;
    std::optional<新世界任务承接结果> 承接;
};

enum class 新任务承接接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 过期决议, 身份冲突, 邮箱已满, 尚未启动,
    已停止接收, 入口拒绝, 资源失败, 内部错误
};

struct 新任务承接接收结果 final {
    新任务承接接收状态 状态 = 新任务承接接收状态::入口拒绝;
    // 接纳不等于已建任务。结果交付后容量释放，由调用者持有共享结果。
    std::shared_future<新任务承接处理结果> 处理结果;
};

struct 新任务管理快照 final {
    新任务管理生命周期 生命周期 = 新任务管理生命周期::未启动;
    std::uint64_t 活动消息数 = 0;
    bool 停止已请求 = false;
    bool 线程已完成 = false;
};

// M1仅承接、读回和反馈，不筹办、不调度现实动作。
class 新_任务管理线程类 final {
public:
    // 世界树必须比本对象存活更久；绑定本对象的self必须先回收。
    explicit 新_任务管理线程类(新_世界树类& 世界树) noexcept;
    ~新_任务管理线程类() noexcept;
    新_任务管理线程类(const 新_任务管理线程类&) = delete;
    新_任务管理线程类& operator=(const 新_任务管理线程类&) = delete;
    新_任务管理线程类(新_任务管理线程类&&) = delete;
    新_任务管理线程类& operator=(新_任务管理线程类&&) = delete;

    新任务管理操作结果 启动(
        const 新任务管理运行配置& 配置, std::uint64_t 等待毫秒) noexcept;
    新任务承接接收结果 提交消息(const 新任务承接消息& 消息) noexcept;
    新任务管理操作结果 请求停止() noexcept;
    新任务管理操作结果 等待停止(std::uint64_t 等待毫秒) noexcept;
    新任务管理快照 读取快照() const noexcept;

private:
    friend class 新_自我线程类;
    bool 使用世界树(const 新_世界树类& 世界树) const noexcept;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
