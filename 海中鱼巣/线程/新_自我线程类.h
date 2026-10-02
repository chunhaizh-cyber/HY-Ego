#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "../业务/新_世界树类.h"

namespace 海中鱼巣 {

// 只缓存进程内已经正式建立并读回的世界事实身份；稳定编码本身就是
// self 身份，不再建立运行代次、逻辑线程身份或第二份 self 事实。
extern 稳定编码 新版自我所在场景节点;
extern 稳定编码 新版自我节点;

struct 新自我线程初始化请求 final {
    // 指定时直接使用既有场景；未指定时在“新场景父节点”或世界根下
    // 建立一个新的自我所在场景。两个字段不能同时有值。
    std::optional<稳定编码> 既有自我所在场景;
    std::optional<稳定编码> 新场景父节点;
    新存在建立请求 自我建立请求;
    新状态强时间 初始状态时间;

    bool 完整() const noexcept;
    friend bool operator==(
        const 新自我线程初始化请求&,
        const 新自我线程初始化请求&) noexcept;
};

enum class 新自我线程生命周期 : std::uint8_t {
    未初始化 = 0,
    已初始化 = 1,
    启动中 = 2,
    等待治理消息 = 3,
    正在停止 = 4,
    已停止 = 5,
    启动失败 = 6,
    内部错误 = 7
};

enum class 新自我线程操作状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    初始化请求不完整 = 4,
    世界树初始化失败 = 5,
    自我所在场景失败 = 6,
    自我建立失败 = 7,
    唯一自我冲突 = 8,
    尚未初始化 = 9,
    尚未启动 = 10,
    已在运行 = 11,
    等待超时 = 12,
    不能等待自身 = 13,
    结构不一致 = 14,
    资源失败 = 15,
    内部错误 = 16
};

struct 新自我线程初始化结果 final {
    新自我线程操作状态 状态 = 新自我线程操作状态::入口拒绝;
    std::optional<新世界初始化结果> 世界树结果;
    std::optional<新场景建立结果> 场景建立结果;
    std::optional<新场景成员建立结果> 自我建立结果;
    std::optional<稳定编码> 自我所在场景;
    std::optional<稳定编码> 自我存在;

    bool 成功() const noexcept;
};

struct 新自我线程操作结果 final {
    新自我线程操作状态 状态 = 新自我线程操作状态::入口拒绝;
    新自我线程生命周期 生命周期 = 新自我线程生命周期::未初始化;
    bool 成功() const noexcept;
};

struct 新自我线程快照 final {
    新自我线程生命周期 生命周期 = 新自我线程生命周期::未初始化;
    std::optional<稳定编码> 自我所在场景;
    std::optional<稳定编码> 自我存在;
    bool 线程已进入 = false;
    bool 停止已请求 = false;
    bool 线程已完成 = false;
};

// 第一片只承载唯一自我的世界事实初始化和常驻线程生命周期。
// 治理消息必须在后继明确具体消息类型与处理函数后才能进入本类。
class 新_自我线程类 final {
public:
    // 世界树服务必须比本对象存活更久。
    explicit 新_自我线程类(新_世界树类& 世界树服务) noexcept;
    ~新_自我线程类() noexcept;

    新_自我线程类(const 新_自我线程类&) = delete;
    新_自我线程类& operator=(const 新_自我线程类&) = delete;
    新_自我线程类(新_自我线程类&&) = delete;
    新_自我线程类& operator=(新_自我线程类&&) = delete;

    // 本函数在调用线程中完成世界树、自我所在场景和唯一自我存在的
    // 建立及读回；不会在内部创建 OS 线程。
    新自我线程初始化结果 初始化(
        const 新自我线程初始化请求& 请求) noexcept;

    // 创建 OS 线程并等待其进入常驻等待态。停止采用独立的进程内控制
    // 信号，不伪装成治理消息。
    新自我线程操作结果 启动(std::uint64_t 等待毫秒) noexcept;
    新自我线程操作结果 请求停止() noexcept;
    新自我线程操作结果 等待停止(std::uint64_t 等待毫秒) noexcept;
    新自我线程快照 读取快照() const noexcept;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
