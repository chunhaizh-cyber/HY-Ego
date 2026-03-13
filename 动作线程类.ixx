export module 动作线程模块;

import <algorithm>;
import <atomic>;
import <condition_variable>;
import <cstdint>;
import <exception>;
import <functional>;
import <future>;
import <memory>;
import <mutex>;
import <queue>;
import <string>;
import <thread>;
import <unordered_map>;
import <utility>;
import <vector>;

import 基础数据类型模块;
import 主信息定义模块;
import 日志模块;

// 依赖按“指针”使用：动作线程不关心内部实现，只负责调度与串行执行。
import 世界树模块;
import 三维场景管理模块;
import 本能动作管理模块; // 最小动作函数注册表
// =========================================================
// 动作线程：串行执行“一系列最小动作函数（本能动作）”，并返回结果。
//
// 设计目标：
// - 线程内串行：保证“动作序列”按顺序执行，方便在未来做因果记录/回放。
// - 轻锁：
//   - 队列锁只做 push/pop，不在锁内执行动作。
//   - 注册表采用 copy-on-write 快照：读无锁、写少量复制。
// - 提交鲁棒：线程未启动 / 正在退出 / 队列满 时，返回“已就绪 future”，避免调用方永久等待。
// =========================================================

export struct 动作执行上下文 {
    世界树类* 世界树 = nullptr;
    三维场景管理类* 场景管理 = nullptr;

    // 可选：关联任务/方法树（动作线程不解析，只透传给动作回调）
    任务节点类* 任务节点 = nullptr;
    任务信息基类* 任务 = nullptr;

    // 兼容字段：历史调用只传一个场景时仍可用.
    场景节点类* 场景 = nullptr;
    场景节点类* 输入场景 = nullptr;
    场景节点类* 输出场景 = nullptr;
    存在节点类* 目标存在 = nullptr;

    方法节点类* 方法根 = nullptr;

    // 本次请求标识
    std::uint64_t 请求ID = 0;
};

export struct 结构体_动作步骤结果 {
    枚举_本能动作ID 动作ID = 枚举_本能动作ID::未定义;

    std::int64_t 成功码 = 0;      // 0=成功；非0=失败原因
    std::int64_t 质量_Q10000 = 0; // 0..10000（由动作回调填）

    // 成本两份：
    // - 成本_us：真实耗时
    // - 成本_Q10000：如需归一化再用
    std::uint64_t 成本_us = 0;
    std::int64_t 成本_Q10000 = 0;

    std::string 备注;            // 可存简短错误/调试信息
};

export struct 结构体_动作执行结果 {
    std::uint64_t 请求ID = 0;
    std::int64_t 总成功码 = 0;         // 任一步失败则为首个失败码

    时间戳 开始_us = 0;
    时间戳 结束_us = 0;

    std::vector<结构体_动作步骤结果> 步骤结果;

    std::uint64_t 总耗时_us() const {
        return (结束_us >= 开始_us) ? (结束_us - 开始_us) : 0;
    }
};

export struct 结构体_动作执行请求 {
    // 若为 0，动作线程会自动分配
    std::uint64_t 请求ID = 0;

    // 执行序列：最小动作函数ID列表
    std::vector<枚举_本能动作ID> 执行序列;

    // 上下文（透传给动作回调）
    // 兼容字段：历史调用只传一个场景时仍可用.
    场景节点类* 场景 = nullptr;
    场景节点类* 输入场景 = nullptr;
    场景节点类* 输出场景 = nullptr;
    存在节点类* 目标存在 = nullptr;
    任务节点类* 任务节点 = nullptr;
    方法节点类* 方法根 = nullptr;

    // 失败策略：
    // - false：遇到首个失败就停止
    // - true：失败也继续跑完序列（用于诊断或补偿）
    bool 失败继续 = false;
};

export class 动作线程类 {
public:
    using 最小动作函数 = std::function<结构体_动作步骤结果(动作执行上下文& ctx)>;

private:
    struct 队列项 {
        结构体_动作执行请求 req;
        std::promise<结构体_动作执行结果> prom;
    };

    // ------------------------------
    // 统一错误码（与旧版本保持兼容范围）
    // ------------------------------
    static constexpr std::int64_t 错误_未启动 = -10000;
    static constexpr std::int64_t 错误_动作未注册 = -10001;
    static constexpr std::int64_t 错误_动作异常 = -10002;
    static constexpr std::int64_t 错误_动作未知异常 = -10003;
    static constexpr std::int64_t 错误_队列已满 = -10004;

    // ------------------------------
    // 环境指针（允许启动后更新，因此用 atomic 指针避免数据竞争）
    // ------------------------------
    std::atomic<世界树类*> 世界树_{ nullptr };
    std::atomic<三维场景管理类*> 场景管理_{ nullptr };

    // ------------------------------
    // 注册表：copy-on-write 快照
    // - 读：atomic_load(reg_snapshot_)，无锁
    // - 写：reg_write_mtx_ 保护下复制一份 map 再 atomic_store
    // ------------------------------
    using FnHandle = std::shared_ptr<const 最小动作函数>;
    using 注册表 = std::unordered_map<std::uint16_t, FnHandle>;

    std::mutex reg_write_mtx_;
    std::atomic<std::shared_ptr<const 注册表>> reg_snapshot_{ std::make_shared<const 注册表>() };

    // ------------------------------
    // 队列：只在锁内 push/pop，不做重计算
    // - max_queue_=0 表示无限制
    // ------------------------------
    std::mutex q_mtx_;
    std::condition_variable cv_;
    std::queue<队列项> q_;
    std::atomic<std::size_t> max_queue_{ 0 };

    // stop_=true：请求退出（线程将“排空队列后”退出）
    std::atomic<bool> stop_{ false };
    // running_=true：线程主循环正在运行
    std::atomic<bool> running_{ false };
    std::thread th_;

    std::atomic<std::uint64_t> next_id_{ 1 };

public:
    动作线程类() = default;
    ~动作线程类() { 停止(true); }

    动作线程类(const 动作线程类&) = delete;
    动作线程类& operator=(const 动作线程类&) = delete;

public:
    // 绑定外部系统（可在启动前或后调用；启动前绑定更推荐）
    void 绑定环境(世界树类* 世界树, 三维场景管理类* 场景管理) {
        世界树_.store(世界树, std::memory_order_release);
        场景管理_.store(场景管理, std::memory_order_release);
    }

    // 队列上限（0=无限制）。用于防止极端情况下队列无限增长。
    void 设置队列上限(std::size_t max) { max_queue_.store(max, std::memory_order_release); }
    std::size_t 队列上限() const { return max_queue_.load(std::memory_order_acquire); }

    // 注意：深度需要拿队列锁，因此非 const。
    std::size_t 队列深度() {
        std::lock_guard<std::mutex> lk(q_mtx_);
        return q_.size();
    }

    bool 运行中() const { return running_.load(std::memory_order_acquire); }

public:
    // 注册 / 覆盖 某个动作的执行体
    void 注册本能(枚举_本能动作ID id, 最小动作函数 fn) {
        if (!fn) return;

        const auto key = static_cast<std::uint16_t>(id);
        auto handle = std::make_shared<const 最小动作函数>(std::move(fn));

        std::lock_guard<std::mutex> lk(reg_write_mtx_);
        auto cur = reg_snapshot_.load();

        auto next = std::make_shared<注册表>(cur ? *cur : 注册表{});
        (*next)[key] = std::move(handle);

        reg_snapshot_.store(std::const_pointer_cast<const 注册表>(next));
    }

    // 移除某个动作
    void 取消注册(枚举_本能动作ID id) {
        const auto key = static_cast<std::uint16_t>(id);

        std::lock_guard<std::mutex> lk(reg_write_mtx_);
        auto cur = reg_snapshot_.load();
        if (!cur || cur->empty()) return;

        auto next = std::make_shared<注册表>(*cur);
        next->erase(key);
        reg_snapshot_.store(std::const_pointer_cast<const 注册表>(next));
    }

    bool 已注册(枚举_本能动作ID id) const {
        const auto key = static_cast<std::uint16_t>(id);
        auto cur = reg_snapshot_.load();
        if (!cur) return false;
        return cur->find(key) != cur->end();
    }

    std::vector<枚举_本能动作ID> 列出已注册动作() const {
        std::vector<枚举_本能动作ID> out;
        auto cur = reg_snapshot_.load();
        if (!cur || cur->empty()) return out;

        out.reserve(cur->size());
        for (const auto& kv : *cur) {
            out.push_back(static_cast<枚举_本能动作ID>(kv.first));
        }
        std::sort(out.begin(), out.end(), [](auto a, auto b) {
            return static_cast<std::uint16_t>(a) < static_cast<std::uint16_t>(b);
            });
        return out;
    }

public:
    // 启动线程
    void 启动() {
        // 若上次线程已结束但仍 joinable，先回收线程对象
        if (th_.joinable() && !running_.load(std::memory_order_acquire)) {
            等待线程结束();
        }

        if (running_.exchange(true)) return;

        stop_.store(false);
        th_ = std::thread([this] { 线程函数_(); });
        日志::运行("[动作线程] 启动");
    }

    // 仅请求退出（不等待）。线程会“排空队列后”退出。
    void 请求退出() {
        stop_.store(true);
        cv_.notify_all();
    }

    // 等待线程结束（join）。
    // 注意：如果在动作线程自身调用，将直接返回（避免 self-join）。
    void 等待线程结束() {
        if (!th_.joinable()) return;
        if (std::this_thread::get_id() == th_.get_id()) return;
        th_.join();
    }

    // 停止线程：
    // - wait=true：请求退出并等待线程结束
    // - wait=false：仅请求退出（可在稍后调用 等待线程结束 / 或由析构回收）
    void 停止(bool wait = true) {
        if (!th_.joinable() && !running_.load(std::memory_order_acquire)) return;

        请求退出();
        if (wait) 等待线程结束();

        // 若 wait=false，running_ 会在线程真正退出时置 false。
        if (wait) {
            running_.store(false, std::memory_order_release);
            日志::运行("[动作线程] 停止");
        }
    }

public:
    // 提交执行请求（拷贝版，保持兼容）
    std::future<结构体_动作执行结果> 提交(const 结构体_动作执行请求& reqIn) {
        结构体_动作执行请求 tmp = reqIn;
        return 提交(std::move(tmp));
    }

    // 提交执行请求（移动版，避免拷贝执行序列）
    std::future<结构体_动作执行结果> 提交(结构体_动作执行请求&& reqIn) {
        if (reqIn.请求ID == 0) reqIn.请求ID = next_id_.fetch_add(1);

        // 不接受新请求的几种情况：未启动/正在退出
        if (!running_.load(std::memory_order_acquire) || stop_.load()) {
            return 立即返回失败_(reqIn.请求ID, 错误_未启动, "动作线程未启动或正在退出");
        }

        const std::size_t limit = max_queue_.load(std::memory_order_acquire);

        队列项 item{};
        item.req = std::move(reqIn);
        auto fut = item.prom.get_future();

        {
            std::lock_guard<std::mutex> lk(q_mtx_);

            // 二次检查：避免竞态下 stop 刚置位
            if (stop_.load()) {
                // 直接返回“就绪 future”
                return 立即返回失败_(item.req.请求ID, 错误_未启动, "动作线程正在退出");
            }

            if (limit > 0 && q_.size() >= limit) {
                return 立即返回失败_(item.req.请求ID, 错误_队列已满, "动作队列已满");
            }

            q_.push(std::move(item));
        }

        cv_.notify_one();
        return fut;
    }

    // 便捷：直接提交序列
    std::future<结构体_动作执行结果> 提交执行序列(
        std::vector<枚举_本能动作ID> 序列,
        场景节点类* 场景,
        场景节点类* 输出场景 = nullptr,
        存在节点类* 目标存在 = nullptr,
        bool 失败继续 = false,
        任务节点类* 任务节点 = nullptr,
        方法节点类* 方法根 = nullptr)
    {
        结构体_动作执行请求 r{};
        r.执行序列 = std::move(序列);
        r.场景 = 场景;
        r.输入场景 = 场景;
        r.输出场景 = 输出场景 ? 输出场景 : 场景;
        r.目标存在 = 目标存在;
        r.失败继续 = 失败继续;
        r.任务节点 = 任务节点;
        r.方法根 = 方法根;
        return 提交(std::move(r));
    }

private:
    static std::future<结构体_动作执行结果> 立即返回失败_(std::uint64_t reqId, std::int64_t code, std::string remark) {
        std::promise<结构体_动作执行结果> p;
        auto fut = p.get_future();

        结构体_动作执行结果 out{};
        out.请求ID = reqId;
        out.总成功码 = code;
        out.开始_us = 结构体_时间戳::当前_微秒();
        out.结束_us = out.开始_us;

        if (!remark.empty()) {
            结构体_动作步骤结果 step{};
            step.动作ID = 枚举_本能动作ID::未定义;
            step.成功码 = code;
            step.备注 = std::move(remark);
            out.步骤结果.push_back(std::move(step));
        }

        p.set_value(std::move(out));
        return fut;
    }

    FnHandle 取动作函数_(枚举_本能动作ID id) const {
        const auto key = static_cast<std::uint16_t>(id);
        auto cur = reg_snapshot_.load();
        if (!cur) return {};
        auto it = cur->find(key);
        if (it == cur->end()) return {};
        return it->second;
    }

    void 线程函数_() {
        // 线程启动时 running_ 已设为 true。
        while (true) {
            队列项 item{};
            bool has = false;

            {
                std::unique_lock<std::mutex> lk(q_mtx_);
                cv_.wait(lk, [this] {
                    return stop_.load(std::memory_order_acquire) || !q_.empty();
                });

                if (!q_.empty()) {
                    item = std::move(q_.front());
                    q_.pop();
                    has = true;
                } else if (stop_.load(std::memory_order_acquire)) {
                    break;
                }
            }

            if (!has) {
                continue;
            }

            结构体_动作执行结果 out{};
            out.请求ID = item.req.请求ID;
            out.开始_us = 结构体_时间戳::当前_微秒();

            动作执行上下文 ctx{};
            ctx.世界树 = 世界树_.load(std::memory_order_acquire);
            ctx.场景管理 = 场景管理_.load(std::memory_order_acquire);
            ctx.任务节点 = item.req.任务节点;
            ctx.输入场景 = item.req.输入场景 ? item.req.输入场景 : item.req.场景;
            ctx.输出场景 = item.req.输出场景 ? item.req.输出场景 : ctx.输入场景;
            ctx.场景 = ctx.输出场景 ? ctx.输出场景 : ctx.输入场景;
            ctx.目标存在 = item.req.目标存在;
            ctx.方法根 = item.req.方法根;
            ctx.请求ID = out.请求ID;

            const bool fail_fast = !item.req.失败继续;

            for (auto 动作ID : item.req.执行序列) {
                auto fn = 取动作函数_(动作ID);
                if (!fn) {
                    结构体_动作步骤结果 step{};
                    step.动作ID = 动作ID;
                    step.成功码 = 错误_动作未注册;
                    step.备注 = "动作未注册";
                    out.步骤结果.push_back(std::move(step));

                    if (out.总成功码 == 0) {
                        out.总成功码 = 错误_动作未注册;
                    }
                    if (fail_fast) {
                        break;
                    }
                    continue;
                }

                try {
                    auto step = (*fn)(ctx);
                    if (step.动作ID == 枚举_本能动作ID::未定义) {
                        step.动作ID = 动作ID;
                    }
                    if (step.成功码 != 0 && out.总成功码 == 0) {
                        out.总成功码 = step.成功码;
                    }
                    out.步骤结果.push_back(std::move(step));

                    if (fail_fast && out.总成功码 != 0) {
                        break;
                    }
                }
                catch (const std::exception& ex) {
                    结构体_动作步骤结果 step{};
                    step.动作ID = 动作ID;
                    step.成功码 = 错误_动作异常;
                    step.备注 = ex.what();
                    out.步骤结果.push_back(std::move(step));

                    if (out.总成功码 == 0) {
                        out.总成功码 = 错误_动作异常;
                    }
                    if (fail_fast) {
                        break;
                    }
                }
                catch (...) {
                    结构体_动作步骤结果 step{};
                    step.动作ID = 动作ID;
                    step.成功码 = 错误_动作未知异常;
                    step.备注 = "动作抛出未知异常";
                    out.步骤结果.push_back(std::move(step));

                    if (out.总成功码 == 0) {
                        out.总成功码 = 错误_动作未知异常;
                    }
                    if (fail_fast) {
                        break;
                    }
                }
            }

            out.结束_us = 结构体_时间戳::当前_微秒();
            item.prom.set_value(std::move(out));
        }

        running_.store(false, std::memory_order_release);
        日志::运行("[动作线程] 线程退出");
    }
};



