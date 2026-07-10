// 文件规则：自我线程只编排系统初始化、发布只读快照并管理驻留停止；不得实现治理循环、任务方法执行或裸写核心仓库。
module;

#include "../领域/初始化.世界树.ixx"
#include "../领域/初始化.语素.ixx"
#include "../领域/初始化.需求.ixx"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <optional>
#include <system_error>
#include <thread>
#include <utility>

export module 海中鱼巣.线程.自我线程;

export namespace 海中鱼巣 {

enum class 自我线程状态 : std::uint32_t {
    未启动 = 0,
    初始化中 = 1,
    运行中 = 2,
    正在停止 = 3,
    已停止 = 4,
    初始化失败 = 5
};

enum class 自我线程拒绝原因 : std::uint32_t {
    无 = 0,
    参数无效 = 1,
    重复启动 = 2,
    初始化失败 = 3,
    未启动 = 4
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

    ~自我线程() {
        停止请求.store(true);
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

        停止请求.store(false);
        已进入.store(false);
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
                || 当前 == 自我线程状态::初始化失败;
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
        状态条件.notify_all();
        return {true, false, 自我线程拒绝原因::无};
    }

    自我线程操作结果 收口等待() {
        const auto 当前 = 状态.load();
        if (当前 == 自我线程状态::未启动) {
            return 拒绝(自我线程拒绝原因::未启动);
        }
        停止请求.store(true);
        状态条件.notify_all();
        if (线程.joinable()) {
            线程.join();
        }
        if (状态.load() != 自我线程状态::初始化失败) {
            状态.store(自我线程状态::已停止);
        }
        return {true, false, 自我线程拒绝原因::无};
    }

private:
    static 自我线程操作结果 拒绝(自我线程拒绝原因 原因) {
        return {false, false, 原因};
    }

    void 执行初始化(const 自我根需求初始化参数& 参数) {
        已进入.store(true);
        状态条件.notify_all();
        if (停止请求.load()) {
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

        {
            std::lock_guard<std::mutex> 锁(快照互斥);
            初始化快照 = std::move(新快照);
        }
        if (停止请求.load()) {
            状态.store(自我线程状态::正在停止);
            状态条件.notify_all();
            状态.store(自我线程状态::已停止);
            状态条件.notify_all();
            return;
        }

        状态.store(自我线程状态::运行中);
        状态条件.notify_all();
        std::unique_lock<std::mutex> 锁(快照互斥);
        状态条件.wait(锁, [this]() {
            return 停止请求.load();
        });
        状态.store(自我线程状态::正在停止);
        锁.unlock();
        状态.store(自我线程状态::已停止);
        状态条件.notify_all();
    }

    void 标记初始化失败() {
        状态.store(自我线程状态::初始化失败);
        状态条件.notify_all();
    }

    语素初始化服务& 语素初始化_;
    世界树初始化服务& 世界树初始化_;
    需求初始化服务& 需求初始化_;
    std::atomic<自我线程状态> 状态 = 自我线程状态::未启动;
    std::atomic<bool> 停止请求 = false;
    std::atomic<bool> 已进入 = false;
    mutable std::mutex 快照互斥;
    mutable std::condition_variable 状态条件;
    std::optional<自我线程初始化快照> 初始化快照;
    std::thread 线程;
};

}
