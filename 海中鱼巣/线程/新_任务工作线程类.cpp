#include "新_任务工作线程类.h"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <new>
#include <thread>
#include <vector>

namespace 海中鱼巣 {
namespace {
std::mutex 工作所有者互斥;
const 新_任务工作线程类* 工作所有者 = nullptr;

std::chrono::milliseconds 工作等待时长(std::uint64_t 毫秒) noexcept {
    const auto 最大 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::duration::max() / 4).count();
    return std::chrono::milliseconds((std::min)(
        毫秒, static_cast<std::uint64_t>(最大)));
}
}

struct 新_任务工作线程类::实现 final {
    实现(新_世界树类& 世界, std::function<void()> 通知) noexcept
        : 世界树(&世界), 完成通知(std::move(通知)) {}

    新_世界树类* 世界树;
    std::function<void()> 完成通知;
    mutable std::mutex 互斥;
    std::mutex 加入互斥;
    std::condition_variable 变化;
    std::thread 线程;
    新任务工作运行配置 配置;
    新任务工作生命周期 生命周期 = 新任务工作生命周期::未启动;
    bool 创建已尝试 = false;
    bool 已进入 = false;
    bool 停止 = false;
    bool 已完成 = false;
    bool 已加入 = false;

    struct 工作项 final {
        新任务工作请求 请求;
        std::promise<新任务工作处理结果> 承诺;
        std::shared_future<新任务工作处理结果> 结果 = 承诺.get_future().share();
        bool 已开始 = false;
        bool 已结束 = false;
    };
    std::vector<std::shared_ptr<工作项>> 活动;

    新任务工作操作结果 操作结果(新任务工作操作状态 状态) const noexcept {
        return {状态, 生命周期};
    }

    void 发出完成通知() noexcept {
        if (!完成通知) return;
        try { 完成通知(); }
        catch (...) {}
    }

    bool 取消未开始工作已加锁() {
        bool 有取消 = false;
        for (const auto& 项 : 活动) {
            if (项->已开始 || 项->已结束) continue;
            项->已结束 = true;
            项->承诺.set_value({项->请求,
                新任务工作处理状态::停止取消, std::nullopt});
            有取消 = true;
        }
        return 有取消;
    }

    void 运行() noexcept {
        try {
            std::unique_lock 锁(互斥);
            已进入 = true;
            变化.notify_all();
            while (!停止) {
                const auto 位置 = std::find_if(活动.begin(), 活动.end(),
                    [](const auto& 项) { return !项->已开始 && !项->已结束; });
                if (位置 == 活动.end()) {
                    生命周期 = 新任务工作生命周期::等待工作;
                    变化.wait(锁, [this] {
                        return 停止 || std::any_of(活动.begin(), 活动.end(),
                            [](const auto& 项) {
                                return !项->已开始 && !项->已结束;
                            });
                    });
                    continue;
                }
                const auto 项 = *位置;
                项->已开始 = true;
                生命周期 = 新任务工作生命周期::处理工作;
                锁.unlock();

                新任务工作处理结果 结果;
                结果.原请求 = 项->请求;
                if (项->请求.种类 == 新任务工作种类::查找方法) {
                    结果.方法召回 = 世界树->查询任务方法候选并发布(
                        项->请求.任务节点);
                    switch (结果.方法召回->状态) {
                    case 新世界任务方法召回状态::已发布有候选:
                        结果.状态 = 新任务工作处理状态::已发布有候选; break;
                    case 新世界任务方法召回状态::已发布完整无候选:
                        结果.状态 = 新任务工作处理状态::已发布完整无候选; break;
                    case 新世界任务方法召回状态::任务状态已变化:
                        结果.状态 = 新任务工作处理状态::任务状态已变化; break;
                    case 新世界任务方法召回状态::尚未初始化:
                        结果.状态 = 新任务工作处理状态::服务未就绪; break;
                    case 新世界任务方法召回状态::方法查询未完成:
                        结果.状态 = 新任务工作处理状态::查找未完成; break;
                    case 新世界任务方法召回状态::候选发布未完成:
                        结果.状态 = 新任务工作处理状态::候选发布未完成; break;
                    case 新世界任务方法召回状态::任务不存在:
                    case 新世界任务方法召回状态::需求不存在:
                    case 新世界任务方法召回状态::结构不一致:
                    case 新世界任务方法召回状态::资源失败:
                        结果.状态 = 新任务工作处理状态::需核查; break;
                    }
                } else {
                    结果.状态 = 新任务工作处理状态::内部错误;
                }

                锁.lock();
                项->承诺.set_value(std::move(结果));
                项->已结束 = true;
                变化.notify_all();
                锁.unlock();
                发出完成通知();
                锁.lock();
            }
            生命周期 = 新任务工作生命周期::停止中;
            const bool 有取消 = 取消未开始工作已加锁();
            生命周期 = 新任务工作生命周期::已停止;
            已完成 = true;
            变化.notify_all();
            锁.unlock();
            if (有取消) 发出完成通知();
        } catch (...) {
            bool 需要通知 = false;
            try {
                {
                    std::lock_guard 锁(互斥);
                    生命周期 = 新任务工作生命周期::内部错误;
                    for (const auto& 项 : 活动) {
                        if (项->已结束) continue;
                        try {
                            项->承诺.set_value({项->请求,
                                新任务工作处理状态::内部错误, std::nullopt});
                            项->已结束 = true;
                            需要通知 = true;
                        } catch (...) {}
                    }
                    已完成 = true;
                    变化.notify_all();
                }
                if (需要通知) 发出完成通知();
            } catch (...) { std::terminate(); }
        }
    }
};

新_任务工作线程类::新_任务工作线程类(
    新_世界树类& 世界树, std::function<void()> 完成通知) noexcept
    : 实现_(new (std::nothrow) 实现(世界树, std::move(完成通知))) {}

新_任务工作线程类::~新_任务工作线程类() noexcept {
    if (!实现_) return;
    (void)请求停止();
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        if (实现_->线程.joinable()) {
            if (实现_->线程.get_id() == std::this_thread::get_id()) std::terminate();
            实现_->线程.join();
        }
        std::lock_guard 所有者锁(工作所有者互斥);
        if (工作所有者 == this) 工作所有者 = nullptr;
    } catch (...) { std::terminate(); }
}

新任务工作操作结果 新_任务工作线程类::启动(
    const 新任务工作运行配置& 配置, std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新任务工作操作状态::资源失败,
        新任务工作生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!配置.邮箱容量 || 配置.邮箱容量 > 实现_->活动.max_size()
            || !等待毫秒) return 实现_->操作结果(新任务工作操作状态::入口拒绝);
        if (实现_->创建已尝试) return 实现_->操作结果(
            实现_->已进入 && !实现_->停止 && !实现_->已完成
                && 实现_->配置 == 配置
                ? 新任务工作操作状态::已在运行
                : 新任务工作操作状态::入口拒绝);
        {
            std::lock_guard 所有者锁(工作所有者互斥);
            if (工作所有者 && 工作所有者 != this) {
                return 实现_->操作结果(新任务工作操作状态::所有者冲突);
            }
            工作所有者 = this;
        }
        实现_->创建已尝试 = true;
        实现_->配置 = 配置;
        实现_->生命周期 = 新任务工作生命周期::启动中;
        try { 实现_->线程 = std::thread([p = 实现_.get()] { p->运行(); }); }
        catch (...) {
            实现_->生命周期 = 新任务工作生命周期::启动失败;
            实现_->已完成 = true;
            return 实现_->操作结果(新任务工作操作状态::资源失败);
        }
        if (!实现_->变化.wait_for(锁, 工作等待时长(等待毫秒), [this] {
            return 实现_->已进入 || 实现_->已完成;
        })) return 实现_->操作结果(新任务工作操作状态::等待超时);
        return 实现_->操作结果(实现_->已完成
            ? 新任务工作操作状态::内部错误
            : 新任务工作操作状态::已完成);
    } catch (...) {
        return {新任务工作操作状态::内部错误,
            新任务工作生命周期::内部错误};
    }
}

新任务工作接收结果 新_任务工作线程类::提交工作(
    const 新任务工作请求& 请求) noexcept {
    新任务工作接收结果 结果;
    if (!实现_) {
        结果.状态 = 新任务工作接收状态::资源失败;
        return 结果;
    }
    if (!请求.工作编号 || 请求.种类 != 新任务工作种类::查找方法
        || !有效(请求.任务节点)) return 结果;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            结果.状态 = 新任务工作接收状态::已停止接收;
            return 结果;
        }
        if (!实现_->已进入) {
            结果.状态 = 新任务工作接收状态::尚未启动;
            return 结果;
        }
        for (const auto& 项 : 实现_->活动) {
            if (项->请求.工作编号 == 请求.工作编号) {
                if (项->请求 == 请求) {
                    结果.状态 = 新任务工作接收状态::精确重复;
                    结果.处理结果 = 项->结果;
                } else {
                    结果.状态 = 新任务工作接收状态::身份冲突;
                }
                return 结果;
            }
            if (项->请求.任务节点 == 请求.任务节点) {
                结果.状态 = 新任务工作接收状态::任务已占用;
                return 结果;
            }
        }
        if (实现_->活动.size() >= 实现_->配置.邮箱容量) {
            结果.状态 = 新任务工作接收状态::邮箱已满;
            return 结果;
        }
        const auto 项 = std::make_shared<实现::工作项>();
        项->请求 = 请求;
        实现_->活动.push_back(项);
        结果.状态 = 新任务工作接收状态::已接收;
        结果.处理结果 = 项->结果;
        实现_->变化.notify_all();
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务工作接收状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务工作接收状态::内部错误;
    }
    return 结果;
}

新任务工作操作状态 新_任务工作线程类::确认结果已接收(
    std::uint64_t 工作编号, 稳定编码 任务节点) noexcept {
    if (!实现_ || !工作编号 || !有效(任务节点)) {
        return 新任务工作操作状态::入口拒绝;
    }
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = std::find_if(实现_->活动.begin(), 实现_->活动.end(),
            [&](const auto& 项) {
                return 项->请求.工作编号 == 工作编号
                    && 项->请求.任务节点 == 任务节点;
            });
        if (位置 == 实现_->活动.end()) return 新任务工作操作状态::无变化;
        if (!(*位置)->已结束) return 新任务工作操作状态::入口拒绝;
        实现_->活动.erase(位置);
        实现_->变化.notify_all();
        return 新任务工作操作状态::已完成;
    } catch (...) {
        return 新任务工作操作状态::内部错误;
    }
}

新任务工作操作结果 新_任务工作线程类::请求停止() noexcept {
    if (!实现_) return {新任务工作操作状态::资源失败,
        新任务工作生命周期::内部错误};
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!实现_->创建已尝试) {
            return 实现_->操作结果(新任务工作操作状态::尚未启动);
        }
        if (实现_->停止 || 实现_->已完成) {
            return 实现_->操作结果(新任务工作操作状态::无变化);
        }
        实现_->停止 = true;
        实现_->生命周期 = 新任务工作生命周期::停止中;
        实现_->变化.notify_all();
        return 实现_->操作结果(新任务工作操作状态::已完成);
    } catch (...) {
        return {新任务工作操作状态::内部错误,
            新任务工作生命周期::内部错误};
    }
}

新任务工作操作结果 新_任务工作线程类::等待停止(
    std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新任务工作操作状态::资源失败,
        新任务工作生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!实现_->线程.joinable()) return 实现_->操作结果(
            实现_->已加入 ? 新任务工作操作状态::无变化
                           : 新任务工作操作状态::尚未启动);
        if (实现_->线程.get_id() == std::this_thread::get_id()) {
            return 实现_->操作结果(新任务工作操作状态::不能等待自身);
        }
        if (!实现_->变化.wait_for(锁, 工作等待时长(等待毫秒),
            [this] { return 实现_->已完成; })) {
            return 实现_->操作结果(新任务工作操作状态::等待超时);
        }
        锁.unlock();
        实现_->线程.join();
        锁.lock();
        实现_->已加入 = true;
        return 实现_->操作结果(实现_->生命周期
                == 新任务工作生命周期::内部错误
            ? 新任务工作操作状态::内部错误
            : 新任务工作操作状态::已完成);
    } catch (...) {
        return {新任务工作操作状态::内部错误,
            新任务工作生命周期::内部错误};
    }
}

新任务工作快照 新_任务工作线程类::读取快照() const noexcept {
    新任务工作快照 结果;
    if (!实现_) {
        结果.生命周期 = 新任务工作生命周期::内部错误;
        return 结果;
    }
    try {
        std::lock_guard 锁(实现_->互斥);
        结果.生命周期 = 实现_->生命周期;
        结果.活动工作数 = 实现_->活动.size();
        结果.停止已请求 = 实现_->停止;
        结果.线程已完成 = 实现_->已完成;
    } catch (...) {
        结果.生命周期 = 新任务工作生命周期::内部错误;
    }
    return 结果;
}

} // namespace 海中鱼巣
