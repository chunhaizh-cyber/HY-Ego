module;
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <deque>
#include <unordered_map>
#include <chrono>
#include <atomic>
#include <string>
#include <utility>
#include <functional>



export module 线程模板模块;
import 日志模块;
// ===========================
// 通用消息（可扩展）
// ===========================
export struct 消息 {
    std::string key;     // 合并键；空表示不合并
    std::string 内容;
};

// ===========================
// 线程模板类：
// - 支持普通队列消息
// - 支持“按 key 合并（last-wins）”消息，防止高频更新堆积
// - 支持 tick（可选）：无消息时周期性唤醒
// ===========================
export class 线程模板类 {
public:
    using 处理函数 = std::function<void(const 消息&)>;

public:
    线程模板类() = default;
    explicit 线程模板类(处理函数 fn) : 处理_(std::move(fn)) {}
    ~线程模板类() { 停止(); }

    线程模板类(const 线程模板类&) = delete;
    线程模板类& operator=(const 线程模板类&) = delete;

public:
    void 设置处理函数(处理函数 fn) {
        std::unique_lock lk(mtx_);
        处理_ = std::move(fn);
    }

    void 设置tick(std::chrono::milliseconds tick) {
        tick间隔_.store((std::int64_t)tick.count(), std::memory_order_relaxed);
        cv_.notify_all();
    }

    bool 是否正在运行() const noexcept { return running_.load(std::memory_order_acquire); }

    void 启动() {
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) return;

        stop_.store(false, std::memory_order_release);
        worker_ = std::thread([this] { this->线程函数_(); });
    }

    void 停止() {
        if (!running_.load(std::memory_order_acquire)) return;

        stop_.store(true, std::memory_order_release);
        cv_.notify_all();

        if (worker_.joinable()) worker_.join();
        running_.store(false, std::memory_order_release);

        std::unique_lock lk(mtx_);
        q_.clear();
        merge_index_.clear();
    }

public:
    // 普通发送：不合并
    void 发送消息(std::string 内容) { 发送消息(消息{ "", std::move(内容) }); }

    void 发送消息(消息 m) {
        if (!running_.load(std::memory_order_acquire)) return;
        {
            std::unique_lock lk(mtx_);
            q_.push_back(std::move(m));
        }
        cv_.notify_one();
    }

    // 合并发送：同 key 只保留最后一条
    void 发送合并消息(std::string key, std::string 内容) {
        if (key.empty()) { 发送消息(std::move(内容)); return; }
        if (!running_.load(std::memory_order_acquire)) return;

        {
            std::unique_lock lk(mtx_);
            auto it = merge_index_.find(key);
            if (it != merge_index_.end()) {
                it->second->内容 = std::move(内容);
            }
            else {
                q_.push_back(消息{ key, std::move(内容) });
                auto iter = std::prev(q_.end());
                merge_index_.emplace(std::move(key), iter);
            }
        }
        cv_.notify_one();
    }

private:
    void 线程函数_() {
        // 线程启动日志
        日志::运行("[线程模板] 线程启动");
        
        while (!stop_.load(std::memory_order_acquire)) {

            消息 msg;
            bool has_msg = false;

            {
                std::unique_lock lk(mtx_);

                auto tick_ms = tick间隔_.load(std::memory_order_relaxed);
                if (q_.empty()) {
                    if (tick_ms > 0) {
                        cv_.wait_for(lk, std::chrono::milliseconds(tick_ms), [&] {
                            return stop_.load(std::memory_order_acquire) || !q_.empty();
                            });
                    }
                    else {
                        cv_.wait(lk, [&] {
                            return stop_.load(std::memory_order_acquire) || !q_.empty();
                            });
                    }
                }

                if (stop_.load(std::memory_order_acquire)) break;

                if (!q_.empty()) {
                    msg = std::move(q_.front());
                    q_.pop_front();
                    has_msg = true;

                    if (!msg.key.empty()) merge_index_.erase(msg.key);
                }
            }

            if (!has_msg) continue;

            try { if (处理_) 处理_(msg); } catch (...) {}
        }
        
        // 线程结束日志
        日志::运行("[线程模板] 线程结束");
    }

private:
    std::mutex mtx_;
    std::condition_variable cv_;

    std::deque<消息> q_;
    std::unordered_map<std::string, std::deque<消息>::iterator> merge_index_;

    std::atomic_bool running_{ false };
    std::atomic_bool stop_{ false };
    std::thread worker_;

    std::atomic<std::int64_t> tick间隔_{ 0 }; // ms，0 表示不用 tick

    处理函数 处理_{};
};

