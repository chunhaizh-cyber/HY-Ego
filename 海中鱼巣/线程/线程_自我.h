#pragma once

#include "../业务/应用服务.本能根运行初始化.h"
#include "../业务/应用服务.自我根需求复核.h"
#include "../业务/应用服务.自我线程正式上下文.h"
#include "../领域/任务治理.本能根任务核心.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

#define 海中鱼巣_定义自我线程身份(类型名) \
    struct 类型名 final { \
        std::uint64_t 值 = 0; \
        [[nodiscard]] bool 完整() const noexcept; \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

海中鱼巣_定义自我线程身份(自我线程消息身份);
海中鱼巣_定义自我线程身份(自我线程原请求身份);
海中鱼巣_定义自我线程身份(自我线程需求身份);

#undef 海中鱼巣_定义自我线程身份

struct 自我线程创建请求_v1 final {
    std::uint64_t 邮箱容量 = 0;
    自我线程正式上下文请求 正式上下文;
    本能根运行锚点 本能根;
    [[nodiscard]] bool 完整() const noexcept;
    friend bool operator==(const 自我线程创建请求_v1&,
                           const 自我线程创建请求_v1&) = default;
};

enum class 自我线程生命周期状态 : std::uint8_t {
    未创建 = 0, 创建中 = 1, 已停门 = 2, 治理中 = 3, 等待依赖 = 4,
    正在停止 = 5, 已停止 = 6, 启动失败 = 7, 内部错误 = 8,
};

enum class 自我线程外部调用状态 : std::uint8_t {
    成功 = 1, 精确重复 = 2, 待实现 = 3, 依赖未就绪 = 4,
    // 数值5已退役，稳定数值不复用。
    引用冲突 = 6, 不可比较 = 7, 资源失败 = 8,
    入口拒绝 = 9, 内部错误 = 10,
};

enum class 自我线程操作状态 : std::uint8_t {
    成功 = 1, 精确重复 = 2, 入口拒绝 = 3, 选择冲突 = 4, 依赖未就绪 = 5,
    队列已满 = 6, 消息冲突 = 7,
    // 数值8已退役，不复用。
    等待超时 = 9, 资源失败 = 10, 内部错误 = 11,
    根初始化代际已结束 = 12,
};

struct 自我线程停门见证_v1 final {
    std::uint64_t 入口序号 = 0;
    std::uint64_t 停门序号 = 0;
    bool 治理运行门开启 = false;
    std::uint64_t 已冻结批次数量 = 0;
    [[nodiscard]] bool 完整() const noexcept;
    friend bool operator==(const 自我线程停门见证_v1&,
                           const 自我线程停门见证_v1&) = default;
};

struct 自我线程创建结果_v1 final {
    自我线程操作状态 状态 = 自我线程操作状态::入口拒绝;
    自我线程生命周期状态 生命周期 = 自我线程生命周期状态::未创建;
    std::optional<自我线程停门见证_v1> 见证;
    bool 写业务事实 = false;
    [[nodiscard]] bool 成功() const noexcept;
};

struct 自我线程操作结果_v1 final {
    自我线程操作状态 状态 = 自我线程操作状态::入口拒绝;
    自我线程生命周期状态 生命周期 = 自我线程生命周期状态::未创建;
    bool 写业务事实 = false;
    [[nodiscard]] bool 成功() const noexcept;
};

struct 自我线程诊断快照_v1 final {
    自我线程生命周期状态 生命周期 = 自我线程生命周期状态::未创建;
    bool 治理运行门开启 = false;
    bool 线程已进入 = false;
    bool 线程已完成 = false;
    std::uint64_t 邮箱数量 = 0;
    std::uint64_t 邮箱容量 = 0;
    std::optional<自我线程消息身份> 当前消息;
    std::uint64_t 已冻结批次数量 = 0;
    std::uint64_t 成功治理批次数量 = 0;
    自我线程外部调用状态 最近外部调用 = 自我线程外部调用状态::待实现;
    bool 内部错误锁存 = false;
};

enum class 自我线程复核触发根 : std::uint8_t {
    双根 = 1, 安全根 = 2, 服务根 = 3
};

struct 自我线程根需求复核消息_v2 final {
    自我线程消息身份 消息;
    自我线程原请求身份 原请求;
    自我线程复核触发根 触发根 = 自我线程复核触发根::双根;
    自我线程需求身份 正式需求定位;
    本能根任务初始化意图身份_v1 安全根任务意图;
    本能根任务初始化意图身份_v1 服务根任务意图;
    friend bool operator==(const 自我线程根需求复核消息_v2&,
                           const 自我线程根需求复核消息_v2&) = default;
};

inline constexpr std::uint64_t 根治理可恢复失败重试间隔毫秒_v1 = 10;

struct 自我到任务管理本能根承接消息_v1 final {
    自我线程消息身份 消息;
    自我线程消息身份 来源复核消息;
    自我线程原请求身份 原请求;
    不可变本能根任务初始化包_v1 初始化包;
    friend bool operator==(const 自我到任务管理本能根承接消息_v1&,
                           const 自我到任务管理本能根承接消息_v1&) = default;
};

enum class 根治理意图投递状态_v1 : std::uint8_t {
    已接收=1, 精确重复=2, 入口拒绝=3, 队列已满=4,
    消息冲突=5, 尚未就绪=6, 正在停止=7, 资源失败=8, 内部不一致=9
};

struct 根治理意图投递结果_v1 final {
    根治理意图投递状态_v1 状态=根治理意图投递状态_v1::入口拒绝;
    自我线程消息身份 消息;
};

class 自我线程根治理意图接收端口_v1 {
public:
    virtual ~自我线程根治理意图接收端口_v1()=default;
    virtual bool 已就绪() const noexcept=0;
    virtual 根治理意图投递结果_v1 提交(
        const 自我到任务管理本能根承接消息_v1&) noexcept=0;
};

class 自我线程 final {
public:
    自我线程() noexcept;
    ~自我线程() noexcept;
    自我线程(const 自我线程&) = delete;
    自我线程& operator=(const 自我线程&) = delete;
    自我线程(自我线程&&) = delete;
    自我线程& operator=(自我线程&&) = delete;

    [[nodiscard]] 自我线程创建结果_v1 创建并停在治理运行门(
        const 自我线程创建请求_v1&,
        自我线程正式上下文提供者&,
        自我线程根需求复核端口&,
        本能根任务核心端口_v1&,
        自我线程根治理意图接收端口_v1&,
        std::uint64_t 等待毫秒) noexcept;
    [[nodiscard]] 自我线程操作结果_v1 复核前置并开放治理运行门(
        std::uint64_t 等待毫秒) noexcept;
    [[nodiscard]] 自我线程操作结果_v1 提交根需求复核消息(
        const 自我线程根需求复核消息_v2&) noexcept;
    [[nodiscard]] 自我线程操作结果_v1 请求停止() noexcept;
    [[nodiscard]] 自我线程操作结果_v1 等待停止(std::uint64_t 等待毫秒) noexcept;
    [[nodiscard]] 自我线程诊断快照_v1 读取诊断快照() const noexcept;

private:
    struct 实现;
    实现* 实现_ = nullptr;
};

} // namespace 海中鱼巣
