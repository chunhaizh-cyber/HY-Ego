#pragma once

#include "业务/新_世界树类.h"
#include "线程/新_自我线程类.h"
#include "线程/新_任务管理线程类.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint64_t 普通应用默认邮箱容量 = 256;
inline constexpr std::uint64_t 普通应用默认资源重试间隔毫秒 = 100;
inline constexpr std::uint64_t 普通应用默认线程进入等待毫秒 = 5000;
inline constexpr std::uint64_t 普通应用默认线程停止回收等待毫秒 = 5000;

struct 普通应用配置 final {
    std::uint64_t 自我线程邮箱容量 = 普通应用默认邮箱容量;
    std::uint64_t 任务管理线程邮箱容量 = 普通应用默认邮箱容量;
    std::uint64_t 资源重试间隔毫秒 = 普通应用默认资源重试间隔毫秒;
    std::uint64_t 线程进入等待毫秒 = 普通应用默认线程进入等待毫秒;
    std::uint64_t 线程停止回收等待毫秒 =
        普通应用默认线程停止回收等待毫秒;
    // 双根权重来自具名运行配置；普通应用默认让两根等权。
    std::int64_t 安全根初始权重 = 1;
    std::int64_t 服务根初始权重 = 1;

    friend bool operator==(const 普通应用配置&, const 普通应用配置&) = default;
};

enum class 普通应用装配状态 : std::uint8_t {
    已装配 = 1,
    入口拒绝,
    世界树初始化失败,
    自我治理基础初始化失败,
    自我初始化失败,
    本能函数登记失败,
    线程绑定失败,
    消息配置失败,
    资源失败,
    内部不一致
};

struct 普通应用装配结果 final {
    普通应用装配状态 状态 = 普通应用装配状态::入口拒绝;
    std::optional<新世界初始化结果> 世界树;
    std::optional<新自我治理基础结果> 自我治理基础;
    std::optional<新自我线程初始化结果> 自我;
    std::optional<新自我线程操作结果> 任务管理绑定;
    std::optional<新自我线程操作结果> 消息配置;

    bool 成功() const noexcept;
};

enum class 普通应用治理操作状态 : std::uint8_t {
    已完成 = 1,
    无变化,
    上下文不存在,
    任务管理启动失败,
    自我线程启动失败,
    自我线程停止失败,
    任务管理停止失败,
    内部不一致
};

struct 普通应用治理操作结果 final {
    普通应用治理操作状态 状态 = 普通应用治理操作状态::上下文不存在;
    std::optional<新任务管理操作结果> 任务管理;
    std::optional<新自我线程操作结果> 自我线程;

    bool 成功() const noexcept;
};

// 建立新版世界树、自我所在场景、唯一自我、需求双根、方法根和线程绑定；
// 本函数不创建OS线程。
普通应用装配结果 构造普通应用上下文(
    const 普通应用配置& 配置) noexcept;
普通应用装配结果 构造普通应用上下文() noexcept;

// 先启动manager（其内部启动worker），再启动self；self启动后立即执行首次
// 双根复核和合法任务交接。
普通应用治理操作结果 启动普通应用治理线程() noexcept;

// 先停止并join self，再停止并join manager及其内部worker。
普通应用治理操作结果 停止并回收普通应用治理线程() noexcept;

} // namespace 海中鱼巣
