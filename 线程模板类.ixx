module;
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <atomic>
#include <string>

export module 线程模板模块;

// 需要 export，否则别的模块（如 场景实时显示线程模块）拿不到 消息 类型
export struct 消息 {
    std::string 内容;
};

export class 线程模板 {
public:
    virtual ~线程模板() { 停止(); }

    void 启动() {
        bool expected = false;
        if (!运行标志.compare_exchange_strong(expected, true)) return; // already running
        工作线程 = std::thread([this] { 线程函数(); });
    }

    void 停止() {
        // 允许在工作线程内部调用：只请求退出，不 join 自己
        if (工作线程.joinable() && std::this_thread::get_id() == 工作线程.get_id()) {
            请求退出();
            return;
        }
        请求退出();
        if (工作线程.joinable()) 工作线程.join();
    }

    void 请求退出() {
        运行标志.store(false);
        条件变量.notify_all();
    }

    bool 正在运行() const { return 运行标志.load(); }

    void 发送消息(std::string 内容) {
        {
            std::lock_guard<std::mutex> lk(互斥锁);
            消息队列.push(消息{ std::move(内容) });
        }
        条件变量.notify_one();
    }

    void 设置Tick间隔(std::chrono::milliseconds dt) {
        tick间隔 = dt;
        条件变量.notify_all();
    }

protected:
    // 可选生命周期钩子
    virtual void 线程开始() {}
    virtual void 线程结束() {}

    // 必须实现：处理消息
    virtual void 处理消息(const 消息&) = 0;

    // 可选：空闲时被周期调用（用于刷新 viz / 轮询硬件等）
    virtual void 线程空闲Tick() {}

private:
    void 线程函数() {
        try { 线程开始(); }
        catch (...) {}

        while (运行标志.load()) {
            消息 msg;
            bool hasMsg = false;

            {
                std::unique_lock<std::mutex> lk(互斥锁);

                if (tick间隔.count() > 0) {
                    条件变量.wait_for(lk, tick间隔, [&] {
                        return !消息队列.empty() || !运行标志.load();
                        });
                }
                else {
                    条件变量.wait(lk, [&] {
                        return !消息队列.empty() || !运行标志.load();
                        });
                }

                if (!运行标志.load()) break;

                if (!消息队列.empty()) {
                    msg = std::move(消息队列.front());
                    消息队列.pop();
                    hasMsg = true;
                }
            }

            if (hasMsg) {
                try { 处理消息(msg); }
                catch (...) {}
            }
            else {
                // tick 唤醒（或 spurious wakeup）
                try { 线程空闲Tick(); }
                catch (...) {}
            }
        }

        try { 线程结束(); }
        catch (...) {}
    }

private:
    std::queue<消息> 消息队列;
    std::mutex 互斥锁;
    std::condition_variable 条件变量;

    std::atomic_bool 运行标志{ false };
    std::thread 工作线程;

    std::chrono::milliseconds tick间隔{ 0 };
};
