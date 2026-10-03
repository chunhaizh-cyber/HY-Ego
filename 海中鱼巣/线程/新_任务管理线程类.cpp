#include "新_任务管理线程类.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <new>
#include <thread>
#include <vector>

namespace 海中鱼巣 {
namespace {
std::mutex 管理所有者互斥;
const 新_任务管理线程类* 管理所有者 = nullptr;

std::chrono::milliseconds 等待时长(std::uint64_t 毫秒) noexcept {
    const auto 最大 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::duration::max() / 4).count();
    return std::chrono::milliseconds((std::min)(毫秒, static_cast<std::uint64_t>(最大)));
}
}

struct 新_任务管理线程类::实现 final {
    explicit 实现(新_世界树类& 世界) noexcept : 世界树(&世界) {}
    新_世界树类* 世界树;
    mutable std::mutex 互斥;
    std::mutex 加入互斥;
    std::condition_variable 变化;
    std::thread 线程;
    新任务管理运行配置 配置;
    新任务管理生命周期 生命周期 = 新任务管理生命周期::未启动;
    bool 创建已尝试 = false, 已进入 = false, 停止 = false, 已完成 = false, 已加入 = false;
    struct 消息项 final {
        新任务承接消息 消息;
        std::promise<新任务承接处理结果> 承诺;
        std::shared_future<新任务承接处理结果> 结果 = 承诺.get_future().share();
        std::chrono::steady_clock::time_point 可处理时点;
    };
    std::vector<std::shared_ptr<消息项>> 活动;

    新任务管理操作结果 操作结果(新任务管理操作状态 状态) const noexcept {
        return {状态, 生命周期};
    }

    void 终结待办(新任务承接处理状态 状态) {
        for (const auto& 项 : 活动) 项->承诺.set_value({项->消息, 状态, std::nullopt});
        活动.clear();
    }

    void 运行() noexcept {
        try {
            std::unique_lock 锁(互斥);
            已进入 = true;
            变化.notify_all();
            while (!停止) {
                生命周期 = 新任务管理生命周期::等待消息;
                if (活动.empty()) {
                    变化.wait(锁, [this] { return 停止 || !活动.empty(); });
                    continue;
                }
                const auto 最早 = std::min_element(活动.begin(), 活动.end(),
                    [](const auto& 左, const auto& 右) { return 左->可处理时点 < 右->可处理时点; });
                const auto 项 = *最早;
                if (项->可处理时点 > std::chrono::steady_clock::now()) {
                    变化.wait_until(锁, 项->可处理时点);
                    continue;
                }
                生命周期 = 新任务管理生命周期::处理消息;
                锁.unlock();
                新任务承接处理结果 结果;
                结果.原消息 = 项->消息;
                结果.承接 = 世界树->承接需求任务(项->消息.需求节点);
                switch (结果.承接->状态) {
                case 新世界任务承接状态::已读回:
                    结果.状态 = 新任务承接处理状态::已承接; break;
                case 新世界任务承接状态::既有终结任务:
                    结果.状态 = 新任务承接处理状态::既有终结; break;
                case 新世界任务承接状态::需求不存在:
                    结果.状态 = 新任务承接处理状态::入口拒绝; break;
                case 新世界任务承接状态::尚未初始化:
                    结果.状态 = 新任务承接处理状态::服务未就绪; break;
                default:
                    结果.状态 = 新任务承接处理状态::需核查; break;
                }
                锁.lock();
                // 只重试明确未进入写入的资源失败。多步写失败不能重放创建。
                if (!结果.承接->写入已尝试
                    && 结果.承接->状态 == 新世界任务承接状态::资源失败) {
                    if (!停止) {
                        项->可处理时点 = std::chrono::steady_clock::now()
                            + 等待时长(配置.资源重试间隔毫秒);
                        continue;
                    }
                    结果.状态 = 新任务承接处理状态::停止取消;
                }
                项->承诺.set_value(std::move(结果));
                活动.erase(std::find(活动.begin(), 活动.end(), 项));
            }
            终结待办(新任务承接处理状态::停止取消);
            生命周期 = 新任务管理生命周期::已停止;
            已完成 = true;
            变化.notify_all();
        } catch (...) {
            try {
                std::lock_guard 锁(互斥);
                生命周期 = 新任务管理生命周期::内部错误;
                try { 终结待办(新任务承接处理状态::内部错误); }
                catch (...) { 活动.clear(); }
                已完成 = true;
                变化.notify_all();
            } catch (...) { std::terminate(); }
        }
    }
};

新_任务管理线程类::新_任务管理线程类(新_世界树类& 世界树) noexcept
    : 实现_(new (std::nothrow) 实现(世界树)) {}

bool 新_任务管理线程类::使用世界树(const 新_世界树类& 世界树) const noexcept {
    return 实现_ && 实现_->世界树 == &世界树;
}

新_任务管理线程类::~新_任务管理线程类() noexcept {
    if (!实现_) return;
    (void)请求停止();
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        if (实现_->线程.joinable()) {
            if (实现_->线程.get_id() == std::this_thread::get_id()) std::terminate();
            实现_->线程.join();
        }
        std::lock_guard 所有者锁(管理所有者互斥);
        if (管理所有者 == this) 管理所有者 = nullptr;
    } catch (...) { std::terminate(); }
}

新任务管理操作结果 新_任务管理线程类::启动(
    const 新任务管理运行配置& 配置, std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!配置.邮箱容量 || 配置.邮箱容量 > 实现_->活动.max_size()
            || !配置.资源重试间隔毫秒 || !等待毫秒
            || static_cast<std::uint64_t>(等待时长(配置.资源重试间隔毫秒).count())
                != 配置.资源重试间隔毫秒)
            return 实现_->操作结果(新任务管理操作状态::入口拒绝);
        if (实现_->创建已尝试) return 实现_->操作结果(
            实现_->已进入 && !实现_->停止 && !实现_->已完成 && 实现_->配置 == 配置
                ? 新任务管理操作状态::已在运行 : 新任务管理操作状态::入口拒绝);
        {
            std::lock_guard 所有者锁(管理所有者互斥);
            if (管理所有者 && 管理所有者 != this)
                return 实现_->操作结果(新任务管理操作状态::所有者冲突);
            管理所有者 = this;
        }
        实现_->创建已尝试 = true;
        实现_->配置 = 配置;
        实现_->生命周期 = 新任务管理生命周期::启动中;
        锁.unlock();
        const bool 就绪 = 实现_->世界树->初始化任务集合();
        锁.lock();
        if (!就绪 || 实现_->停止) {
            实现_->生命周期 = 新任务管理生命周期::启动失败;
            实现_->已完成 = true;
            实现_->变化.notify_all();
            return 实现_->操作结果(新任务管理操作状态::服务未就绪);
        }
        try { 实现_->线程 = std::thread([p = 实现_.get()] { p->运行(); }); }
        catch (...) {
            实现_->生命周期 = 新任务管理生命周期::启动失败;
            实现_->已完成 = true;
            return 实现_->操作结果(新任务管理操作状态::资源失败);
        }
        if (!实现_->变化.wait_for(锁, 等待时长(等待毫秒), [this] {
            return 实现_->已进入 || 实现_->已完成;
        })) return 实现_->操作结果(新任务管理操作状态::等待超时);
        return 实现_->操作结果(实现_->已完成
            ? 新任务管理操作状态::内部错误 : 新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

新任务承接接收结果 新_任务管理线程类::提交消息(const 新任务承接消息& 消息) noexcept {
    新任务承接接收结果 结果;
    if (!实现_) { 结果.状态 = 新任务承接接收状态::资源失败; return 结果; }
    if (!消息.消息编号 || !有效(消息.需求节点)) return 结果;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            结果.状态 = 新任务承接接收状态::已停止接收; return 结果;
        }
        if (!实现_->已进入) { 结果.状态 = 新任务承接接收状态::尚未启动; return 结果; }
        for (const auto& 项 : 实现_->活动) {
            if (项->消息.消息编号 != 消息.消息编号) continue;
            if (项->消息 == 消息) {
                结果.状态 = 新任务承接接收状态::精确重复;
                结果.处理结果 = 项->结果;
            } else 结果.状态 = 新任务承接接收状态::身份冲突;
            return 结果;
        }
        if (实现_->活动.size() >= 实现_->配置.邮箱容量) {
            结果.状态 = 新任务承接接收状态::邮箱已满; return 结果;
        }
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息 = 消息;
        项->可处理时点 = std::chrono::steady_clock::now();
        实现_->活动.push_back(项);
        结果.处理结果 = 项->结果;
        结果.状态 = 新任务承接接收状态::已接收;
        实现_->变化.notify_all();
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = 新任务承接接收状态::资源失败; }
    catch (...) { 结果.状态 = 新任务承接接收状态::内部错误; }
    return 结果;
}

新任务管理操作结果 新_任务管理线程类::请求停止() noexcept {
    if (!实现_) return {新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!实现_->创建已尝试) return 实现_->操作结果(新任务管理操作状态::尚未启动);
        if (实现_->停止 || 实现_->已完成) return 实现_->操作结果(新任务管理操作状态::无变化);
        实现_->停止 = true;
        实现_->生命周期 = 新任务管理生命周期::停止中;
        实现_->变化.notify_all();
        return 实现_->操作结果(新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

新任务管理操作结果 新_任务管理线程类::等待停止(std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!实现_->线程.joinable()) return 实现_->操作结果(实现_->已加入
            ? 新任务管理操作状态::无变化 : 新任务管理操作状态::尚未启动);
        if (实现_->线程.get_id() == std::this_thread::get_id())
            return 实现_->操作结果(新任务管理操作状态::不能等待自身);
        if (!实现_->变化.wait_for(锁, 等待时长(等待毫秒), [this] { return 实现_->已完成; }))
            return 实现_->操作结果(新任务管理操作状态::等待超时);
        锁.unlock();
        实现_->线程.join();
        锁.lock();
        实现_->已加入 = true;
        return 实现_->操作结果(实现_->生命周期 == 新任务管理生命周期::内部错误
            ? 新任务管理操作状态::内部错误 : 新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

新任务管理快照 新_任务管理线程类::读取快照() const noexcept {
    新任务管理快照 结果;
    if (!实现_) { 结果.生命周期 = 新任务管理生命周期::内部错误; return 结果; }
    try {
        std::lock_guard 锁(实现_->互斥);
        结果.生命周期 = 实现_->生命周期;
        结果.活动消息数 = 实现_->活动.size();
        结果.停止已请求 = 实现_->停止;
        结果.线程已完成 = 实现_->已完成;
    } catch (...) { 结果.生命周期 = 新任务管理生命周期::内部错误; }
    return 结果;
}

} // namespace 海中鱼巣
