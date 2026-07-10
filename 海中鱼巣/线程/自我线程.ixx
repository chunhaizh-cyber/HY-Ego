// 文件规则：自我线程只编排系统初始化、治理批次和驻留停止；领域处理必须在队列锁外委托既有路由，不得裸写核心仓库。
module;

#include "../领域/初始化.世界树.ixx"
#include "../领域/初始化.语素.ixx"
#include "../领域/初始化.需求.ixx"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <optional>
#include <system_error>
#include <thread>
#include <utility>

export module 海中鱼巣.线程.自我线程;

import 海中鱼巣.线程.自我治理消息协议;
import 海中鱼巣.线程.有界自我治理队列;

export namespace 海中鱼巣 {

enum class 自我线程状态 : std::uint32_t {
    未启动 = 0,
    初始化中 = 1,
    运行中 = 2,
    正在停止 = 3,
    已停止 = 4,
    初始化失败 = 5,
    治理失败 = 6
};

enum class 自我线程拒绝原因 : std::uint32_t {
    无 = 0,
    参数无效 = 1,
    重复启动 = 2,
    初始化失败 = 3,
    未启动 = 4,
    治理依赖无效 = 5
};

struct 自我线程操作结果 {
    bool 成功 = false;
    bool 复用初始化结果 = false;
    自我线程拒绝原因 拒绝原因 = 自我线程拒绝原因::无;
};

struct 自我线程初始化快照 {
    语素初始化结果 语素;
    世界树初始化结果 世界树;
    自我根需求初始化结果 根需求;
    std::uint64_t 初始化代次 = 0;

    bool 成功() const {
        return 语素.成功()
            && 世界树.成功()
            && 根需求.成功()
            && 初始化代次 != 0;
    }
};

enum class 自我线程治理处理状态 : std::uint32_t {
    已完成 = 1,
    逻辑内返回 = 2,
    内部不一致 = 3
};

enum class 自我线程治理循环状态 : std::uint32_t {
    未接入 = 0,
    等待初始化 = 1,
    等待材料 = 2,
    处理批次 = 3,
    已停止 = 4,
    内部不一致 = 5
};

struct 自我线程治理处理结果 {
    自我线程治理处理状态 状态 = 自我线程治理处理状态::逻辑内返回;
};

using 自我线程治理处理器 = std::function<自我线程治理处理结果(
    const 自我治理消息&,
    const 自我线程初始化快照&)>;

struct 自我线程治理快照 {
    自我线程治理循环状态 状态 = 自我线程治理循环状态::未接入;
    std::uint64_t 冻结批次数 = 0;
    std::uint64_t 完成批次数 = 0;
    std::uint64_t 中断批次数 = 0;
    std::uint64_t 冻结消息数 = 0;
    std::uint64_t 处理消息数 = 0;
    std::uint64_t 完成消息数 = 0;
    std::uint64_t 逻辑内返回数 = 0;
    std::uint64_t 内部不一致数 = 0;
    std::uint64_t 最后冻结批次序号 = 0;
};

class 自我线程 {
public:
    自我线程(
        语素初始化服务& 语素初始化,
        世界树初始化服务& 世界树初始化,
        需求初始化服务& 需求初始化)
        : 语素初始化_(语素初始化),
        世界树初始化_(世界树初始化),
          需求初始化_(需求初始化) {
    }

    自我线程(
        语素初始化服务& 语素初始化,
        世界树初始化服务& 世界树初始化,
        需求初始化服务& 需求初始化,
        有界自我治理邮箱& 治理邮箱,
        自我线程治理处理器 治理处理器)
        : 语素初始化_(语素初始化),
          世界树初始化_(世界树初始化),
          需求初始化_(需求初始化),
          治理邮箱_(&治理邮箱),
          治理处理器_(std::move(治理处理器)) {
    }

    ~自我线程() {
        停止请求.store(true);
        if (治理邮箱_ != nullptr) {
            (void)治理邮箱_->请求停止并唤醒();
        }
        状态条件.notify_all();
        if (线程.joinable()) {
            线程.join();
        }
    }

    自我线程(const 自我线程&) = delete;
    自我线程& operator=(const 自我线程&) = delete;

    自我线程状态 当前状态() const {
        return 状态.load();
    }

    bool 线程已进入() const {
        return 已进入.load();
    }

    bool 线程可收口() const {
        return !线程.joinable();
    }

    自我线程治理快照 读取治理快照() const {
        return {
            治理状态.load(),
            冻结批次数.load(),
            完成批次数.load(),
            中断批次数.load(),
            冻结消息数.load(),
            处理消息数.load(),
            完成消息数.load(),
            逻辑内返回数.load(),
            内部不一致数.load(),
            最后冻结批次序号.load()
        };
    }

    bool 等待治理完成批次数(
        std::uint64_t 目标批次数,
        std::chrono::milliseconds 最长等待) const {
        if (治理邮箱_ == nullptr || 目标批次数 == 0 || 最长等待.count() <= 0) {
            return false;
        }
        std::unique_lock<std::mutex> 锁(快照互斥);
        const bool 已达到或终止 = 状态条件.wait_for(锁, 最长等待, [this, 目标批次数]() {
            const auto 当前状态值 = 状态.load();
            return 完成批次数.load() >= 目标批次数
                || 当前状态值 == 自我线程状态::治理失败
                || 当前状态值 == 自我线程状态::已停止
                || 当前状态值 == 自我线程状态::初始化失败;
        });
        return 已达到或终止 && 完成批次数.load() >= 目标批次数;
    }

    自我线程操作结果 启动(const 自我根需求初始化参数& 参数) {
        const auto 已有快照 = 读取初始化快照();
        if (已有快照.has_value() && 已有快照->成功()) {
            return {true, true, 自我线程拒绝原因::重复启动};
        }
        if (状态.load() != 自我线程状态::未启动 || 线程.joinable()) {
            return 拒绝(自我线程拒绝原因::重复启动);
        }
        if (!参数.有效()) {
            return 拒绝(自我线程拒绝原因::参数无效);
        }
        const bool 已接入治理邮箱 = 治理邮箱_ != nullptr;
        const bool 已接入治理处理器 = static_cast<bool>(治理处理器_);
        if (已接入治理邮箱 != 已接入治理处理器) {
            return 拒绝(自我线程拒绝原因::治理依赖无效);
        }

        停止请求.store(false);
        已进入.store(false);
        重置治理读数();
        状态.store(自我线程状态::初始化中);
        try {
            线程 = std::thread([this, 参数]() {
                执行初始化(参数);
            });
        } catch (const std::system_error&) {
            状态.store(自我线程状态::初始化失败);
            状态条件.notify_all();
            return 拒绝(自我线程拒绝原因::初始化失败);
        }
        return {true, false, 自我线程拒绝原因::无};
    }

    bool 等待初始化完成(std::chrono::milliseconds 最长等待) const {
        std::unique_lock<std::mutex> 锁(快照互斥);
        const bool 已结束初始化 = 状态条件.wait_for(锁, 最长等待, [this]() {
            const auto 当前 = 状态.load();
            return 当前 == 自我线程状态::运行中
                || 当前 == 自我线程状态::正在停止
                || 当前 == 自我线程状态::已停止
                || 当前 == 自我线程状态::初始化失败
                || 当前 == 自我线程状态::治理失败;
        });
        return 已结束初始化
            && 初始化快照.has_value()
            && 初始化快照->成功();
    }

    std::optional<自我线程初始化快照> 读取初始化快照() const {
        std::lock_guard<std::mutex> 锁(快照互斥);
        return 初始化快照;
    }

    自我线程操作结果 请求停止() {
        const auto 当前 = 状态.load();
        if (当前 == 自我线程状态::未启动) {
            return 拒绝(自我线程拒绝原因::未启动);
        }
        停止请求.store(true);
        if (当前 == 自我线程状态::运行中) {
            状态.store(自我线程状态::正在停止);
        }
        if (治理邮箱_ != nullptr) {
            (void)治理邮箱_->请求停止并唤醒();
        }
        状态条件.notify_all();
        return {true, false, 自我线程拒绝原因::无};
    }

    自我线程操作结果 收口等待() {
        const auto 当前 = 状态.load();
        if (当前 == 自我线程状态::未启动) {
            return 拒绝(自我线程拒绝原因::未启动);
        }
        停止请求.store(true);
        if (治理邮箱_ != nullptr) {
            (void)治理邮箱_->请求停止并唤醒();
        }
        状态条件.notify_all();
        if (线程.joinable()) {
            线程.join();
        }
        if (状态.load() != 自我线程状态::初始化失败
            && 状态.load() != 自我线程状态::治理失败) {
            状态.store(自我线程状态::已停止);
        }
        return {true, false, 自我线程拒绝原因::无};
    }

private:
    static 自我线程操作结果 拒绝(自我线程拒绝原因 原因) {
        return {false, false, 原因};
    }

    void 重置治理读数() {
        治理状态.store(治理邮箱_ == nullptr
            ? 自我线程治理循环状态::未接入
            : 自我线程治理循环状态::等待初始化);
        冻结批次数.store(0);
        完成批次数.store(0);
        中断批次数.store(0);
        冻结消息数.store(0);
        处理消息数.store(0);
        完成消息数.store(0);
        逻辑内返回数.store(0);
        内部不一致数.store(0);
        最后冻结批次序号.store(0);
    }

    void 执行初始化(const 自我根需求初始化参数& 参数) {
        已进入.store(true);
        状态条件.notify_all();
        if (停止请求.load()) {
            治理状态.store(治理邮箱_ == nullptr
                ? 自我线程治理循环状态::未接入
                : 自我线程治理循环状态::已停止);
            状态.store(自我线程状态::已停止);
            状态条件.notify_all();
            return;
        }

        const auto 语素结果 = 语素初始化_.初始化基础显示语素();
        if (!语素结果.成功()) {
            标记初始化失败();
            return;
        }
        const auto 世界树结果 = 世界树初始化_.初始化自我世界树();
        if (!世界树结果.成功()) {
            标记初始化失败();
            return;
        }
        const auto 根需求结果 = 需求初始化_.初始化两个根需求(世界树结果, 参数);
        自我线程初始化快照 新快照{语素结果, 世界树结果, 根需求结果, 1};
        if (!新快照.成功()) {
            标记初始化失败();
            return;
        }

        const auto 治理初始化快照 = 新快照;
        {
            std::lock_guard<std::mutex> 锁(快照互斥);
            初始化快照 = std::move(新快照);
        }
        if (停止请求.load()) {
            治理状态.store(治理邮箱_ == nullptr
                ? 自我线程治理循环状态::未接入
                : 自我线程治理循环状态::已停止);
            状态.store(自我线程状态::正在停止);
            状态条件.notify_all();
            状态.store(自我线程状态::已停止);
            状态条件.notify_all();
            return;
        }

        状态.store(自我线程状态::运行中);
        状态条件.notify_all();
        if (治理邮箱_ != nullptr) {
            执行治理循环(治理初始化快照);
            return;
        }
        std::unique_lock<std::mutex> 锁(快照互斥);
        状态条件.wait(锁, [this]() {
            return 停止请求.load();
        });
        状态.store(自我线程状态::正在停止);
        锁.unlock();
        状态.store(自我线程状态::已停止);
        状态条件.notify_all();
    }

    void 执行治理循环(const 自我线程初始化快照& 治理初始化快照) {
        while (!停止请求.load()) {
            治理状态.store(自我线程治理循环状态::等待材料);
            状态条件.notify_all();
            const auto 冻结结果 = 治理邮箱_->等待并冻结下一批(std::chrono::seconds(10));
            if (冻结结果.状态 == 自我治理冻结状态::等待超时) {
                continue;
            }
            if (冻结结果.状态 == 自我治理冻结状态::已停止) {
                break;
            }
            if (冻结结果.状态 != 自我治理冻结状态::已冻结
                || 冻结结果.批次.批次序号() == 0
                || 冻结结果.批次.数量() == 0) {
                标记治理失败();
                return;
            }

            治理状态.store(自我线程治理循环状态::处理批次);
            冻结批次数.fetch_add(1);
            冻结消息数.fetch_add(冻结结果.批次.数量());
            最后冻结批次序号.store(冻结结果.批次.批次序号());
            状态条件.notify_all();

            bool 完整处理批次 = true;
            for (const auto& 消息 : 冻结结果.批次.读取消息组()) {
                if (停止请求.load()) {
                    完整处理批次 = false;
                    break;
                }
                if (消息.类型 == 自我治理消息类型::停止请求) {
                    停止请求.store(true);
                    完整处理批次 = false;
                    break;
                }

                自我线程治理处理结果 处理结果;
                try {
                    处理结果 = 治理处理器_(消息, 治理初始化快照);
                } catch (...) {
                    内部不一致数.fetch_add(1);
                    状态条件.notify_all();
                    标记治理失败();
                    return;
                }
                处理消息数.fetch_add(1);
                if (处理结果.状态 == 自我线程治理处理状态::已完成) {
                    完成消息数.fetch_add(1);
                } else if (处理结果.状态 == 自我线程治理处理状态::逻辑内返回) {
                    逻辑内返回数.fetch_add(1);
                } else {
                    内部不一致数.fetch_add(1);
                    状态条件.notify_all();
                    标记治理失败();
                    return;
                }
                状态条件.notify_all();
            }

            if (完整处理批次) {
                完成批次数.fetch_add(1);
            } else {
                中断批次数.fetch_add(1);
            }
            状态条件.notify_all();
        }

        治理状态.store(自我线程治理循环状态::已停止);
        if (状态.load() != 自我线程状态::治理失败) {
            状态.store(自我线程状态::正在停止);
            状态条件.notify_all();
            状态.store(自我线程状态::已停止);
        }
        状态条件.notify_all();
    }

    void 标记治理失败() {
        治理状态.store(自我线程治理循环状态::内部不一致);
        状态.store(自我线程状态::治理失败);
        停止请求.store(true);
        if (治理邮箱_ != nullptr) {
            (void)治理邮箱_->请求停止并唤醒();
        }
        状态条件.notify_all();
    }

    void 标记初始化失败() {
        治理状态.store(治理邮箱_ == nullptr
            ? 自我线程治理循环状态::未接入
            : 自我线程治理循环状态::已停止);
        状态.store(自我线程状态::初始化失败);
        状态条件.notify_all();
    }

    语素初始化服务& 语素初始化_;
    世界树初始化服务& 世界树初始化_;
    需求初始化服务& 需求初始化_;
    有界自我治理邮箱* 治理邮箱_ = nullptr;
    自我线程治理处理器 治理处理器_;
    std::atomic<自我线程状态> 状态 = 自我线程状态::未启动;
    std::atomic<bool> 停止请求 = false;
    std::atomic<bool> 已进入 = false;
    std::atomic<自我线程治理循环状态> 治理状态 = 自我线程治理循环状态::未接入;
    std::atomic<std::uint64_t> 冻结批次数 = 0;
    std::atomic<std::uint64_t> 完成批次数 = 0;
    std::atomic<std::uint64_t> 中断批次数 = 0;
    std::atomic<std::uint64_t> 冻结消息数 = 0;
    std::atomic<std::uint64_t> 处理消息数 = 0;
    std::atomic<std::uint64_t> 完成消息数 = 0;
    std::atomic<std::uint64_t> 逻辑内返回数 = 0;
    std::atomic<std::uint64_t> 内部不一致数 = 0;
    std::atomic<std::uint64_t> 最后冻结批次序号 = 0;
    mutable std::mutex 快照互斥;
    mutable std::condition_variable 状态条件;
    std::optional<自我线程初始化快照> 初始化快照;
    std::thread 线程;
};

}
