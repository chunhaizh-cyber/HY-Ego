
module;
#include <afx.h>

export module 外设模块;

import 基础数据类型模块;

import <string>;
import <variant>;
import <vector>;
import <deque>;
import <optional>;
import <mutex>;
import <condition_variable>;
import <thread>;
import <atomic>;
import <chrono>;
import <cstdint>;

// ============================================================================
// 外设模块（统一接口 + 运行基类）
// 目标：
//  1) 外设控制（独立）：启动/停止/指令/状态机
//  2) 外设数据获取（接口）：统一数据包头 + 取数语义（取最新 / 等待下一包）
//  3) 外设信息写入世界树（内部）：不在本模块做（由上层写入器/会话完成）
//
// 推荐用法：
//   class D455相机外设 : public 外设运行基类<结构体_原始场景帧> { ... };
//   cam.启动(启动参数);
//   外设数据包<结构体_原始场景帧> pkg;
//   cam.等待下一包(pkg, 50);
// ============================================================================

export enum class 枚举_外设状态 : std::uint8_t {
    未启动 = 0,
    启动中,
    运行中,
    暂停中,
    停止中,
    已停止,
    离线,
    故障,
};

export struct 外设状态信息 {
    枚举_外设状态 状态 = 枚举_外设状态::未启动;
    int           错误码 = 0;
    std::string   消息;
    时间戳        最后变化时间 = 0;
};

export enum class 枚举_取数结果 : std::int8_t {
    成功 = 0,
    无新数据 = 1,
    超时 = 2,
    已停止 = 3,
    故障 = 4,
};

export enum class 枚举_外设指令 : std::int16_t {
    未定义 = 0,
    暂停采集,
    恢复采集,
    重新枚举设备,
    重置,
    自检,
    设置参数,          // 参数名/参数值_ 生效
    自定义 = 1000,
};

export using 外设参数值 = std::variant<std::int64_t, double, bool, std::string>;

export struct 外设指令 {
    枚举_外设指令 类型 = 枚举_外设指令::未定义;
    std::string   参数名;      // e.g. "exposure" / "fps" / "laser_power"
    外设参数值    参数值_{};
};

export struct 外设指令回执 {
    bool        成功 = false;
    int         错误码 = 0;
    std::string 消息;
};

export struct 外设启动参数 {
    std::string 设备ID;           // 空=自动选择
    int 期望FPS = 30;
    int 宽 = 640;
    int 高 = 480;

    // 通用扩展参数（避免每种外设都改结构）
    // 例如：{"align","depth_to_color"}, {"exposure",8000}
    struct KV {
        std::string key;
        外设参数值  val;
    };
    std::vector<KV> 扩展;
};

export struct 外设数据包头 {
    std::string   设备ID;
    std::uint64_t 序号 = 0;        // 单调递增
    时间戳        时间_设备 = 0;    // 设备侧时间戳（若未知可为0）
    时间戳        时间_系统 = 0;    // 本机收到时刻
    std::uint32_t 丢包计数 = 0;    // 队列溢出/丢弃累计
    std::uint32_t 标志位 = 0;      // 自定义：关键帧/校准更新/状态变化等
    int           状态码 = 0;      // 可映射设备错误码
};

export template<class T>
struct 外设数据包 {
    外设数据包头 头;
    T 数据;
};

// =========================================================================
// 统一外设接口（数据T由外设决定）
// =========================================================================
export template<class 数据T>
class 外设接口 {
public:
    virtual ~外设接口() = default;

    // A) 外设控制（独立）
    virtual bool 启动(const 外设启动参数& p) = 0;
    virtual void 停止() = 0;
    virtual 外设指令回执 指令(const 外设指令& cmd) = 0;
    virtual 外设状态信息 状态() const = 0;

    // B) 外设数据获取（接口）
    virtual 枚举_取数结果 取最新(外设数据包<数据T>& out) = 0;                         // 非阻塞
    virtual 枚举_取数结果 等待下一包(外设数据包<数据T>& out, int 超时毫秒) = 0;       // 阻塞
};

// =========================================================================
// 外设运行基类：提供线程+缓冲+规范取数语义
// 派生类只需实现：启动设备/停止设备/处理指令/抓取数据
// =========================================================================
export template<class 数据T>
class 外设运行基类 : public 外设接口<数据T> {
public:
    外设运行基类() = default;
    virtual ~外设运行基类() { 停止(); }

    bool 启动(const 外设启动参数& p) override {
        std::scoped_lock lk(m_);
        if (状态_.状态 == 枚举_外设状态::运行中 || 状态_.状态 == 枚举_外设状态::启动中) return true;

        params_ = p;
        设置状态_已加锁(枚举_外设状态::启动中, 0, "启动中");

        if (!启动设备(p)) {
            设置状态_已加锁(枚举_外设状态::故障, -1, "启动设备失败");
            return false;
        }

        running_.store(true);
        paused_.store(false);
        stop_requested_.store(false);

        采集线程_ = std::thread([this] { 采集循环_(); });
        设置状态_已加锁(枚举_外设状态::运行中, 0, "运行中");
        return true;
    }

    void 停止() override {
        {
            std::scoped_lock lk(m_);
            if (状态_.状态 == 枚举_外设状态::未启动 || 状态_.状态 == 枚举_外设状态::已停止) return;
            设置状态_已加锁(枚举_外设状态::停止中, 0, "停止中");
            stop_requested_.store(true);
            running_.store(false);
            paused_.store(false);
        }
        cv_.notify_all();
        if (采集线程_.joinable()) 采集线程_.join();

        try { 停止设备(); }
        catch (...) {}

        {
            std::scoped_lock lk(m_);
            queue_.clear();
            latest_.reset();
            设置状态_已加锁(枚举_外设状态::已停止, 0, "已停止");
        }
        cv_.notify_all();
    }

    外设指令回执 指令(const 外设指令& cmd) override {
        switch (cmd.类型) {
        case 枚举_外设指令::暂停采集:
            paused_.store(true);
            return { true, 0, "已暂停" };
        case 枚举_外设指令::恢复采集:
            paused_.store(false);
            return { true, 0, "已恢复" };
        default:
            break;
        }
        return 处理指令_设备(cmd);
    }

    外设状态信息 状态() const override {
        std::scoped_lock lk(m_);
        return 状态_;
    }

    枚举_取数结果 取最新(外设数据包<数据T>& out) override {
        std::scoped_lock lk(m_);
        if (状态_.状态 == 枚举_外设状态::故障) return 枚举_取数结果::故障;
        if (!latest_.has_value()) {
            if (状态_.状态 == 枚举_外设状态::已停止 || 状态_.状态 == 枚举_外设状态::停止中) return 枚举_取数结果::已停止;
            return 枚举_取数结果::无新数据;
        }
        out = *latest_;
        return 枚举_取数结果::成功;
    }

    枚举_取数结果 等待下一包(外设数据包<数据T>& out, int 超时毫秒) override {
        std::unique_lock lk(m_);
        if (状态_.状态 == 枚举_外设状态::故障) return 枚举_取数结果::故障;

        auto pred = [&] { return !queue_.empty() || stop_requested_.load(); };

        if (!pred()) {
            if (超时毫秒 < 0) cv_.wait(lk, pred);
            else if (!cv_.wait_for(lk, std::chrono::milliseconds(超时毫秒), pred))
                return 枚举_取数结果::超时;
        }

        if (queue_.empty()) {
            return (状态_.状态 == 枚举_外设状态::已停止 || 状态_.状态 == 枚举_外设状态::停止中)
                ? 枚举_取数结果::已停止
                : 枚举_取数结果::无新数据;
        }

        out = std::move(queue_.front());
        queue_.pop_front();
        return 枚举_取数结果::成功;
    }

protected:
    // 派生类要实现的“设备控制”
    virtual bool 启动设备(const 外设启动参数& p) = 0;
    virtual void 停止设备() = 0;
    virtual 外设指令回执 处理指令_设备(const 外设指令& cmd) = 0;

    // 派生类要实现的“抓取一包数据”
    virtual bool 抓取数据_阻塞(数据T& out) = 0;

    // 可选：设备时间戳提取（默认尝试 d.时间 字段）
    virtual 时间戳 取设备时间戳(const 数据T& d) const {
        if constexpr (requires { d.时间; }) {
            return static_cast<时间戳>(d.时间);
        }
        return 0;
    }

    const 外设启动参数& 启动参数() const noexcept { return params_; }

    void 置故障(const std::string& msg, int err = -1) {
        std::scoped_lock lk(m_);
        设置状态_已加锁(枚举_外设状态::故障, err, msg);
        running_.store(false);
        stop_requested_.store(true);
        cv_.notify_all();
    }

private:
    void 设置状态_已加锁(枚举_外设状态 st, int err, const std::string& msg) {
        状态_.状态 = st;
        状态_.错误码 = err;
        状态_.消息 = msg;
        状态_.最后变化时间 = 结构体_时间戳::当前();
    }

    void 推入数据包_已加锁(外设数据包<数据T>&& pkg) {
        latest_ = pkg;
        if (queue_.size() >= queue_capacity_) {
            queue_.pop_front();
            ++dropped_;
        }
        pkg.头.丢包计数 = dropped_;
        queue_.push_back(std::move(pkg));
        cv_.notify_one();
    }

    void 采集循环_() {
        while (running_.load()) {
            if (paused_.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                continue;
            }

            数据T data{};
            bool ok = false;
            try { ok = 抓取数据_阻塞(data); }
            catch (...) { ok = false; }

            if (!ok) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                continue;
            }

            外设数据包<数据T> pkg;
            pkg.头.设备ID = params_.设备ID;
            pkg.头.序号 = ++seq_;
            pkg.头.时间_系统 = 结构体_时间戳::当前();
            pkg.头.时间_设备 = 取设备时间戳(data);
            pkg.数据 = std::move(data);

            {
                std::scoped_lock lk(m_);
                if (状态_.状态 == 枚举_外设状态::故障) break;
                推入数据包_已加锁(std::move(pkg));
            }
        }
    }

private:
    mutable std::mutex m_;
    std::condition_variable cv_;

    外设启动参数 params_{};
    外设状态信息 状态_{};

    std::thread 采集线程_;
    std::atomic<bool> running_{ false };
    std::atomic<bool> paused_{ false };
    std::atomic<bool> stop_requested_{ false };

    std::uint64_t seq_ = 0;
    std::uint32_t dropped_ = 0;

    std::size_t queue_capacity_ = 6;
    std::deque<外设数据包<数据T>> queue_;
    std::optional<外设数据包<数据T>> latest_;
};
