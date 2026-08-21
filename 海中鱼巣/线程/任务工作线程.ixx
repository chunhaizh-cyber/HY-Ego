// 文件规则：任务工作线程只运行已形成工作结果的尾部队列、形成回执并上交任务管理线程；不得执行方法、形成状态动态或裁决任务完成。
module;

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <optional>
#include <thread>
#include <utility>

export module 海中鱼巣.线程.任务工作线程;

import 海中鱼巣.线程.任务结果回执协议;
import 海中鱼巣.线程.任务管理线程;

export namespace 海中鱼巣 {

enum class 任务工作线程状态 : std::uint8_t {
    未启动 = 1,
    运行中 = 2,
    正在排空 = 3,
    已停止 = 4,
    内部错误 = 5
};

enum class 任务工作线程提交状态 : std::uint8_t {
    已接收 = 1,
    已拒绝 = 2,
    资源等待 = 3
};

struct 任务工作线程操作结果 final {
    bool 成功 = false;
    任务工作线程状态 状态 = 任务工作线程状态::未启动;
};

struct 任务工作线程提交结果 final {
    任务工作线程提交状态 状态 = 任务工作线程提交状态::已拒绝;
    std::uint64_t 待处理数量 = 0;

    bool 成功() const noexcept {
        return 状态 == 任务工作线程提交状态::已接收;
    }
};

struct 任务工作线程快照 final {
    任务工作线程状态 状态 = 任务工作线程状态::未启动;
    std::uint64_t 队列容量 = 0;
    std::uint64_t 待处理数量 = 0;
    std::uint64_t 已接收数量 = 0;
    std::uint64_t 已形成回执数量 = 0;
    std::uint64_t 已上交数量 = 0;
    std::uint64_t 已拒绝数量 = 0;
    std::optional<任务结果回执形成结果> 最后形成结果;
    std::optional<任务结果回执提交结果> 最后上交结果;
};

class 任务工作线程 final {
public:
    任务工作线程(
        std::uint64_t 结果队列容量,
        任务管理线程& 上行目标) noexcept
        : 结果队列容量_(结果队列容量), 上行目标_(上行目标) {
    }

    ~任务工作线程() noexcept {
        (void)停止接收并排空();
        (void)收口等待();
    }

    任务工作线程(const 任务工作线程&) = delete;
    任务工作线程& operator=(const 任务工作线程&) = delete;
    任务工作线程(任务工作线程&&) = delete;
    任务工作线程& operator=(任务工作线程&&) = delete;

    任务工作线程操作结果 启动() noexcept {
        std::lock_guard<std::mutex> 锁(锁_);
        if (状态_ != 任务工作线程状态::未启动 || 结果队列容量_ == 0) {
            return {false, 状态_};
        }
        try {
            状态_ = 任务工作线程状态::运行中;
            停止接收_ = false;
            线程_ = std::thread([this]() noexcept { 运行结果尾部(); });
            return {true, 状态_};
        } catch (...) {
            状态_ = 任务工作线程状态::内部错误;
            return {false, 状态_};
        }
    }

    任务工作线程提交结果 提交已形成任务工作结果(
        const 任务工作结果材料& 材料) noexcept {
        try {
            std::lock_guard<std::mutex> 锁(锁_);
            if (状态_ != 任务工作线程状态::运行中 || 停止接收_) {
                return {任务工作线程提交状态::已拒绝,
                    static_cast<std::uint64_t>(待处理_.size())};
            }
            if (待处理_.size() >= 结果队列容量_) {
                return {任务工作线程提交状态::资源等待,
                    static_cast<std::uint64_t>(待处理_.size())};
            }
            待处理_.push_back(材料);
            ++已接收数量_;
            条件_.notify_one();
            return {任务工作线程提交状态::已接收,
                static_cast<std::uint64_t>(待处理_.size())};
        } catch (...) {
            return {任务工作线程提交状态::已拒绝, 0};
        }
    }

    任务工作线程操作结果 停止接收并排空() noexcept {
        std::lock_guard<std::mutex> 锁(锁_);
        if (状态_ == 任务工作线程状态::未启动
            || 状态_ == 任务工作线程状态::已停止) {
            return {状态_ == 任务工作线程状态::已停止, 状态_};
        }
        停止接收_ = true;
        if (状态_ != 任务工作线程状态::内部错误) {
            状态_ = 任务工作线程状态::正在排空;
        }
        条件_.notify_all();
        return {状态_ == 任务工作线程状态::正在排空, 状态_};
    }

    任务工作线程操作结果 收口等待() noexcept {
        if (线程_.joinable()) 线程_.join();
        std::lock_guard<std::mutex> 锁(锁_);
        if (状态_ != 任务工作线程状态::内部错误) {
            状态_ = 任务工作线程状态::已停止;
        }
        return {状态_ == 任务工作线程状态::已停止, 状态_};
    }

    任务工作线程快照 读取快照() const noexcept {
        try {
            std::lock_guard<std::mutex> 锁(锁_);
            return {状态_, 结果队列容量_,
                static_cast<std::uint64_t>(待处理_.size()),
                已接收数量_, 已形成回执数量_, 已上交数量_, 已拒绝数量_,
                最后形成结果_, 最后上交结果_};
        } catch (...) {
            return {任务工作线程状态::内部错误};
        }
    }

private:
    void 运行结果尾部() noexcept {
        for (;;) {
            任务工作结果材料 材料;
            {
                std::unique_lock<std::mutex> 锁(锁_);
                条件_.wait(锁, [this]() noexcept {
                    return 停止接收_ || !待处理_.empty();
                });
                if (待处理_.empty()) {
                    if (停止接收_) break;
                    continue;
                }
                材料 = std::move(待处理_.front());
                待处理_.pop_front();
            }

            const auto 形成 = 形成任务结果回执(材料);
            std::optional<任务结果回执提交结果> 上交;
            if (形成.成功()) {
                上交 = 上行目标_.提交任务结果回执(
                    *形成.回执, 材料.发生时间戳);
            }
            {
                std::lock_guard<std::mutex> 锁(锁_);
                最后形成结果_ = 形成;
                if (形成.成功()) ++已形成回执数量_;
                if (上交) {
                    最后上交结果_ = *上交;
                    if (上交->成功()) ++已上交数量_;
                    else ++已拒绝数量_;
                } else {
                    ++已拒绝数量_;
                }
            }
        }
    }

    std::uint64_t 结果队列容量_ = 0;
    任务管理线程& 上行目标_;
    mutable std::mutex 锁_;
    std::condition_variable 条件_;
    std::deque<任务工作结果材料> 待处理_;
    任务工作线程状态 状态_ = 任务工作线程状态::未启动;
    bool 停止接收_ = false;
    std::uint64_t 已接收数量_ = 0;
    std::uint64_t 已形成回执数量_ = 0;
    std::uint64_t 已上交数量_ = 0;
    std::uint64_t 已拒绝数量_ = 0;
    std::optional<任务结果回执形成结果> 最后形成结果_;
    std::optional<任务结果回执提交结果> 最后上交结果_;
    std::thread 线程_;
};

} // namespace 海中鱼巣
