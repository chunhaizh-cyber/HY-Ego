#include "新_自我工作线程类.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <new>
#include <thread>

namespace 海中鱼巣 {
namespace {
std::chrono::milliseconds 有界时长(std::uint64_t 毫秒) noexcept {
    const auto 最大 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::duration::max() / 4).count();
    return std::chrono::milliseconds((std::min)(
        毫秒, static_cast<std::uint64_t>(最大)));
}
}

struct 新_自我工作线程类::实现 final {
    实现(新_需求类& 需求, 新_任务类& 任务, std::function<void()> 通知) noexcept
        : 需求服务(&需求), 任务服务(&任务), 完成通知(std::move(通知)) {}

    新_需求类* 需求服务;
    新_任务类* 任务服务;
    std::function<void()> 完成通知;
    mutable std::mutex 互斥;
    std::mutex 加入互斥;
    std::condition_variable 变化;
    std::thread 线程;
    新自我工作生命周期 生命周期 = 新自我工作生命周期::未启动;
    bool 创建已尝试 = false;
    bool 已进入 = false;
    bool 停止 = false;
    bool 已完成 = false;
    bool 已加入 = false;

    struct 工作项 final {
        新自我需求树扫描请求 请求;
        std::promise<新自我需求树扫描结果> 承诺;
        std::shared_future<新自我需求树扫描结果> 结果 = 承诺.get_future().share();
        bool 已开始 = false;
        bool 已结束 = false;
    };
    std::shared_ptr<工作项> 活动;

    新自我工作操作结果 操作结果(新自我工作操作状态 状态) const noexcept {
        return {状态, 生命周期};
    }

    void 通知完成() noexcept {
        if (!完成通知) return;
        try { 完成通知(); } catch (...) {}
    }

    新自我需求树扫描结果 扫描(const 新自我需求树扫描请求& 请求) noexcept {
        新自我需求树扫描结果 结果;
        结果.原请求 = 请求;
        try {
            const auto 需求 = 需求服务->查询全部需求();
            if (需求.状态 != 新需求操作状态::已找到) {
                结果.状态 = 需求.状态 == 新需求操作状态::尚未初始化
                    ? 新自我需求树扫描状态::需求服务未就绪
                    : (需求.状态 == 新需求操作状态::资源失败
                        ? 新自我需求树扫描状态::资源失败
                        : 新自我需求树扫描状态::需求树结构不一致);
                return 结果;
            }
            const auto 任务 = 任务服务->查询全部任务();
            if (任务.状态 != 新任务操作状态::已找到) {
                结果.状态 = 任务.状态 == 新任务操作状态::尚未初始化
                    ? 新自我需求树扫描状态::任务服务未就绪
                    : (任务.状态 == 新任务操作状态::资源失败
                        ? 新自我需求树扫描状态::资源失败
                        : 新自我需求树扫描状态::任务结构不一致);
                return 结果;
            }

            结果.需求节点组 = 需求.需求节点组;
            结果.需求数量 = static_cast<std::uint64_t>(需求.需求节点组.size());
            结果.任务数量 = static_cast<std::uint64_t>(任务.任务节点组.size());
            if (请求.种类 == 新自我工作种类::需求任务数量核对) {
                结果.需求节点组.clear();
                结果.状态 = 新自我需求树扫描状态::已完成;
                return 结果;
            }
            std::map<稳定编码, std::vector<稳定编码>> 需求任务;
            for (const auto 任务节点 : 任务.任务节点组) {
                const auto 信息 = 任务服务->获取任务(任务节点);
                if (!信息) {
                    结果.状态 = 新自我需求树扫描状态::任务结构不一致;
                    return 结果;
                }
                if (!std::binary_search(需求.需求节点组.begin(),
                        需求.需求节点组.end(), 信息->需求节点)) {
                    结果.无需求任务节点组.push_back(任务节点);
                    continue;
                }
                需求任务[信息->需求节点].push_back(任务节点);
            }
            for (const auto 需求节点 : 需求.需求节点组) {
                const auto 位置 = 需求任务.find(需求节点);
                if (位置 == 需求任务.end()) {
                    结果.缺少任务的需求节点组.push_back(需求节点);
                } else if (位置->second.size() != 1) {
                    结果.多任务需求节点组.push_back(需求节点);
                }
            }
            if (!结果.多任务需求节点组.empty()
                || !结果.无需求任务节点组.empty()) {
                结果.状态 = 新自我需求树扫描状态::任务结构不一致;
                return 结果;
            }
            结果.状态 = 新自我需求树扫描状态::已完成;
            return 结果;
        } catch (...) {
            结果.状态 = 新自我需求树扫描状态::资源失败;
            return 结果;
        }
    }

    void 运行() noexcept {
        try {
            std::unique_lock 锁(互斥);
            已进入 = true;
            变化.notify_all();
            while (!停止) {
                if (!活动 || 活动->已开始 || 活动->已结束) {
                    生命周期 = 新自我工作生命周期::等待工作;
                    变化.wait(锁, [this] {
                        return 停止 || (活动 && !活动->已开始 && !活动->已结束);
                    });
                    continue;
                }
                const auto 项 = 活动;
                项->已开始 = true;
                生命周期 = 新自我工作生命周期::处理工作;
                锁.unlock();
                auto 结果 = 扫描(项->请求);
                锁.lock();
                项->承诺.set_value(std::move(结果));
                项->已结束 = true;
                变化.notify_all();
                锁.unlock();
                通知完成();
                锁.lock();
            }
            生命周期 = 新自我工作生命周期::停止中;
            if (活动 && !活动->已开始 && !活动->已结束) {
                新自我需求树扫描结果 结果;
                结果.原请求 = 活动->请求;
                结果.状态 = 新自我需求树扫描状态::停止取消;
                活动->承诺.set_value(std::move(结果));
                活动->已结束 = true;
            }
            生命周期 = 新自我工作生命周期::已停止;
            已完成 = true;
            变化.notify_all();
        } catch (...) {
            try {
                std::lock_guard 锁(互斥);
                生命周期 = 新自我工作生命周期::内部错误;
                if (活动 && !活动->已结束) {
                    新自我需求树扫描结果 结果;
                    结果.原请求 = 活动->请求;
                    结果.状态 = 新自我需求树扫描状态::内部错误;
                    活动->承诺.set_value(std::move(结果));
                    活动->已结束 = true;
                }
                已完成 = true;
                变化.notify_all();
            } catch (...) { std::terminate(); }
            通知完成();
        }
    }
};

新_自我工作线程类::新_自我工作线程类(
    新_需求类& 需求服务, 新_任务类& 任务服务,
    std::function<void()> 完成通知) noexcept
    : 实现_(new (std::nothrow) 实现(
        需求服务, 任务服务, std::move(完成通知))) {}

新_自我工作线程类::~新_自我工作线程类() noexcept {
    if (!实现_) return;
    (void)请求停止();
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        if (实现_->线程.joinable()) {
            if (实现_->线程.get_id() == std::this_thread::get_id()) std::terminate();
            实现_->线程.join();
        }
    } catch (...) { std::terminate(); }
}

新自我工作操作结果 新_自我工作线程类::启动(std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新自我工作操作状态::资源失败, 新自我工作生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!等待毫秒) return 实现_->操作结果(新自我工作操作状态::入口拒绝);
        if (实现_->创建已尝试) return 实现_->操作结果(
            实现_->已进入 && !实现_->停止 && !实现_->已完成
                ? 新自我工作操作状态::已在运行
                : 新自我工作操作状态::入口拒绝);
        实现_->创建已尝试 = true;
        实现_->生命周期 = 新自我工作生命周期::启动中;
        try { 实现_->线程 = std::thread([p = 实现_.get()] { p->运行(); }); }
        catch (...) {
            实现_->生命周期 = 新自我工作生命周期::启动失败;
            实现_->已完成 = true;
            return 实现_->操作结果(新自我工作操作状态::资源失败);
        }
        if (!实现_->变化.wait_for(锁, 有界时长(等待毫秒), [this] {
            return 实现_->已进入 || 实现_->已完成;
        })) return 实现_->操作结果(新自我工作操作状态::等待超时);
        return 实现_->操作结果(实现_->已完成
            ? 新自我工作操作状态::内部错误
            : 新自我工作操作状态::已完成);
    } catch (...) {
        return {新自我工作操作状态::内部错误, 新自我工作生命周期::内部错误};
    }
}

新自我工作接收结果 新_自我工作线程类::提交工作(
    const 新自我需求树扫描请求& 请求) noexcept {
    新自我工作接收结果 结果;
    if (!实现_ || !请求.工作编号
        || (请求.种类 != 新自我工作种类::需求任务数量核对
            && 请求.种类 != 新自我工作种类::需求树全局扫描)) return 结果;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            结果.状态 = 新自我工作接收状态::已停止接收;
            return 结果;
        }
        if (!实现_->已进入) {
            结果.状态 = 新自我工作接收状态::尚未启动;
            return 结果;
        }
        if (实现_->活动) {
            if (实现_->活动->请求 == 请求) {
                结果.状态 = 新自我工作接收状态::精确重复;
                结果.处理结果 = 实现_->活动->结果;
            } else if (实现_->活动->请求.工作编号 == 请求.工作编号) {
                结果.状态 = 新自我工作接收状态::身份冲突;
            } else {
                结果.状态 = 新自我工作接收状态::已有扫描;
            }
            return 结果;
        }
        实现_->活动 = std::make_shared<实现::工作项>();
        实现_->活动->请求 = 请求;
        结果.状态 = 新自我工作接收状态::已接收;
        结果.处理结果 = 实现_->活动->结果;
        实现_->变化.notify_all();
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新自我工作接收状态::资源失败;
    } catch (...) {
        结果.状态 = 新自我工作接收状态::内部错误;
    }
    return 结果;
}

新自我工作操作状态 新_自我工作线程类::确认结果已接收(
    std::uint64_t 工作编号) noexcept {
    if (!实现_ || !工作编号) return 新自我工作操作状态::入口拒绝;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!实现_->活动) return 新自我工作操作状态::无变化;
        if (实现_->活动->请求.工作编号 != 工作编号 || !实现_->活动->已结束)
            return 新自我工作操作状态::入口拒绝;
        实现_->活动.reset();
        实现_->变化.notify_all();
        return 新自我工作操作状态::已完成;
    } catch (...) { return 新自我工作操作状态::内部错误; }
}

新自我工作操作结果 新_自我工作线程类::请求停止() noexcept {
    if (!实现_) return {新自我工作操作状态::资源失败, 新自我工作生命周期::内部错误};
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!实现_->创建已尝试) return 实现_->操作结果(新自我工作操作状态::尚未启动);
        if (实现_->停止 || 实现_->已完成) return 实现_->操作结果(新自我工作操作状态::无变化);
        实现_->停止 = true;
        实现_->生命周期 = 新自我工作生命周期::停止中;
        实现_->变化.notify_all();
        return 实现_->操作结果(新自我工作操作状态::已完成);
    } catch (...) { return {新自我工作操作状态::内部错误, 新自我工作生命周期::内部错误}; }
}

新自我工作操作结果 新_自我工作线程类::等待停止(std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新自我工作操作状态::资源失败, 新自我工作生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!实现_->线程.joinable()) return 实现_->操作结果(
            实现_->已加入 ? 新自我工作操作状态::无变化 : 新自我工作操作状态::尚未启动);
        if (实现_->线程.get_id() == std::this_thread::get_id())
            return 实现_->操作结果(新自我工作操作状态::不能等待自身);
        if (!实现_->变化.wait_for(锁, 有界时长(等待毫秒), [this] { return 实现_->已完成; }))
            return 实现_->操作结果(新自我工作操作状态::等待超时);
        锁.unlock();
        实现_->线程.join();
        锁.lock();
        实现_->已加入 = true;
        return 实现_->操作结果(实现_->生命周期 == 新自我工作生命周期::内部错误
            ? 新自我工作操作状态::内部错误 : 新自我工作操作状态::已完成);
    } catch (...) { return {新自我工作操作状态::内部错误, 新自我工作生命周期::内部错误}; }
}

新自我工作快照 新_自我工作线程类::读取快照() const noexcept {
    新自我工作快照 结果;
    if (!实现_) { 结果.生命周期 = 新自我工作生命周期::内部错误; return 结果; }
    try {
        std::lock_guard 锁(实现_->互斥);
        结果.生命周期 = 实现_->生命周期;
        结果.有活动扫描 = static_cast<bool>(实现_->活动);
        结果.停止已请求 = 实现_->停止;
        结果.线程已完成 = 实现_->已完成;
    } catch (...) { 结果.生命周期 = 新自我工作生命周期::内部错误; }
    return 结果;
}

} // namespace 海中鱼巣
