module;

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

export module 自治宿主模块;

import 日志模块;
import 自我线程模块;

export struct 结构_自治宿主配置 {
    bool 允许自动重启 = true;
    std::chrono::milliseconds 监控周期{ 1000 };
    std::chrono::milliseconds 重启退避{ 1500 };
    std::uint32_t 最大自动重启次数 = 5; // 0 表示不限次数
};

export struct 结构_自治宿主快照 {
    bool 宿主运行中 = false;
    bool 请求停止 = false;
    bool 自我线程运行中 = false;
    bool 初始化完成 = false;
    bool 健康运行 = false;
    bool 请求安全重启 = false;
    std::uint32_t 致命错误计数 = 0;
    std::uint32_t 已执行重启次数 = 0;
    std::string 最近故障摘要;
    std::string 宿主故障摘要;
};

export class 自治宿主类 {
public:
    explicit 自治宿主类(结构_自治宿主配置 配置 = {}) : 配置_(配置) {}
    ~自治宿主类() { 停止(); }

    void 启动();
    void 请求停止();
    void 等待线程结束();
    void 停止();
    bool 是否正在运行() const;
    结构_自治宿主快照 快照() const;

private:
    mutable std::mutex 互斥_;
    std::condition_variable 条件_;
    结构_自治宿主配置 配置_{};
    std::unique_ptr<自我线程类> 自我线程_;
    std::thread 监督线程_;
    bool 监督线程运行中_ = false;
    bool 请求停止_ = false;
    std::uint32_t 已执行重启次数_ = 0;
    std::string 宿主故障摘要_;

    std::unique_ptr<自我线程类> 私有_摘除当前自我线程_已加锁();
    void 私有_创建并启动自我线程(const std::string& 原因, bool 计入重启次数);
    bool 私有_需要重启当前自我线程_已加锁(std::string* 原因) const;
    void 私有_登记宿主故障(const std::string& 摘要);
    void 监督线程入口();
};

inline void 自治宿主类::启动() {
    std::unique_lock<std::mutex> 锁(互斥_);
    if (监督线程_.joinable()) {
        if (监督线程运行中_) return;
        锁.unlock();
        等待线程结束();
        锁.lock();
        if (监督线程_.joinable()) return;
    }

    请求停止_ = false;
    已执行重启次数_ = 0;
    宿主故障摘要_.clear();
    监督线程运行中_ = true;
    监督线程_ = std::thread([this] { 监督线程入口(); });
}

inline void 自治宿主类::请求停止() {
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        请求停止_ = true;
    }
    条件_.notify_all();
}

inline void 自治宿主类::等待线程结束() {
    if (监督线程_.joinable()) {
        if (std::this_thread::get_id() == 监督线程_.get_id()) {
            日志::运行("[自治宿主] 当前线程命中监督线程 self-join 风险，等待拥有者稍后回收");
            return;
        }
        监督线程_.join();
    }
    std::unique_ptr<自我线程类> 待停止线程;
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        待停止线程 = 私有_摘除当前自我线程_已加锁();
        监督线程运行中_ = false;
    }

    if (待停止线程) {
        日志::运行("[自治宿主] 停止剩余自我线程");
        待停止线程->停止();
    }
}

inline void 自治宿主类::停止() {
    请求停止();
    等待线程结束();
}

inline bool 自治宿主类::是否正在运行() const {
    std::lock_guard<std::mutex> 锁(互斥_);
    return 监督线程运行中_ && !请求停止_;
}

inline 结构_自治宿主快照 自治宿主类::快照() const {
    std::lock_guard<std::mutex> 锁(互斥_);

    结构_自治宿主快照 结果{};
    结果.宿主运行中 = 监督线程运行中_;
    结果.请求停止 = 请求停止_;
    结果.已执行重启次数 = 已执行重启次数_;
    结果.宿主故障摘要 = 宿主故障摘要_;

    if (自我线程_) {
        结果.自我线程运行中 = 自我线程_->是否正在运行();
        结果.初始化完成 = 自我线程_->是否初始化完成();
        结果.健康运行 = 自我线程_->是否健康运行();
        结果.请求安全重启 = 自我线程_->是否请求安全重启();
        结果.致命错误计数 = 自我线程_->致命错误计数();
        结果.最近故障摘要 = 自我线程_->最近故障摘要();
    }

    return 结果;
}

inline std::unique_ptr<自我线程类> 自治宿主类::私有_摘除当前自我线程_已加锁() {
    return std::move(自我线程_);
}

inline void 自治宿主类::私有_创建并启动自我线程(const std::string& 原因, bool 计入重启次数) {
    auto 新线程 = std::make_unique<自我线程类>();
    日志::运行("[自治宿主] 启动自我线程: 原因=" + 原因);
    新线程->启动();

    bool 需要立即停止 = false;
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        if (请求停止_) {
            需要立即停止 = true;
        }
        else {
            if (计入重启次数) ++已执行重启次数_;
            自我线程_ = std::move(新线程);
        }
    }

    if (需要立即停止 && 新线程) {
        日志::运行("[自治宿主] 自我线程刚启动即收到宿主停止请求，转入受控停止");
        新线程->停止();
    }
}

inline bool 自治宿主类::私有_需要重启当前自我线程_已加锁(std::string* 原因) const {
    if (!自我线程_) {
        if (原因) *原因 = "宿主检测到自我线程缺失";
        return true;
    }

    if (自我线程_->是否请求安全重启()) {
        if (原因) {
            *原因 = "自我线程请求安全重启: " + 自我线程_->最近故障摘要();
        }
        return true;
    }

    if (!自我线程_->是否正在运行()) {
        if (原因) {
            *原因 = "自我线程已退出: " + 自我线程_->最近故障摘要();
        }
        return true;
    }

    return false;
}

inline void 自治宿主类::私有_登记宿主故障(const std::string& 摘要) {
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        宿主故障摘要_ = 摘要;
    }
    日志::运行_错误("[自治宿主][故障] " + 摘要);
}

inline void 自治宿主类::监督线程入口() {
    日志::运行("[自治宿主] 监督线程启动");
    私有_创建并启动自我线程("首次启动", false);

    while (true) {
        std::unique_ptr<自我线程类> 待停止线程;
        std::string 重启原因;
        bool 需要创建新线程 = false;
        bool 应退出监督线程 = false;

        {
            std::unique_lock<std::mutex> 锁(互斥_);
            if (条件_.wait_for(锁, 配置_.监控周期, [this] { return 请求停止_; })) {
                待停止线程 = 私有_摘除当前自我线程_已加锁();
                应退出监督线程 = true;
            }
            else if (私有_需要重启当前自我线程_已加锁(&重启原因)) {
                const bool 允许本次重启 =
                    配置_.允许自动重启 &&
                    (配置_.最大自动重启次数 == 0 || 已执行重启次数_ < 配置_.最大自动重启次数);

                待停止线程 = 私有_摘除当前自我线程_已加锁();
                if (允许本次重启) {
                    需要创建新线程 = true;
                }
                else {
                    请求停止_ = true;
                    宿主故障摘要_ = "已达到自动重启上限或已禁用自动重启: " + 重启原因;
                    应退出监督线程 = true;
                }
            }
        }

        if (待停止线程) {
            日志::运行("[自治宿主] 停止当前自我线程: 原因=" + (重启原因.empty() ? std::string("宿主停止") : 重启原因));
            待停止线程->停止();
        }

        if (需要创建新线程) {
            if (配置_.重启退避.count() > 0) {
                std::this_thread::sleep_for(配置_.重启退避);
            }
            私有_创建并启动自我线程(重启原因, true);
            continue;
        }

        if (应退出监督线程) {
            {
                std::lock_guard<std::mutex> 锁(互斥_);
                监督线程运行中_ = false;
            }
            if (!宿主故障摘要_.empty()) {
                日志::运行_错误("[自治宿主] 监督线程结束: " + 宿主故障摘要_);
            }
            else {
                日志::运行("[自治宿主] 监督线程结束");
            }
            return;
        }
    }
}
