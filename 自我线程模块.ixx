
module;

export module 自我线程模块;

import 基础数据类型模块;
import 主信息定义模块;
import 世界树环境模块;
import 语素环境模块;
import 状态模块;

import 需求模块;
import 动态模块;
import 自我模块;

import 数据仓库模块;
import 任务模块;
import 任务调度器模块;
import 任务执行模块;
import 方法模块;
import 方法环境模块;
import 度量模板注册表模块;
import 特征类型定义模块;
import 本能动作管理模块;
import 外设本能方法_相机模块;
import 自我本能方法_相机认知模块;


import <thread>;
import <atomic>;
import <mutex>;
import <condition_variable>;
import <functional>;
import <chrono>;
import 日志模块;

import<cassert>;
import <vector>;
import <string>;
import <cstdint>;  // for int64_t
import <deque>;
import <queue>;
import <map>;
import <algorithm>;
import <format>;
import <cmath>;
import <limits>;

namespace 自我线程模块_detail {
    inline constexpr I64 方法尝试状态_未尝试 = 0;
    inline constexpr I64 方法尝试状态_尝试中 = 1;
    inline constexpr I64 方法尝试状态_尝试完成 = 2;
    inline constexpr I64 方法尝试状态_待重新验证 = 3;

    inline const 词性节点类* 特征_方法尝试状态() { return 语素集.添加词性词("方法_尝试状态", "名词"); }
    inline const 词性节点类* 特征_方法最近结果签名() { return 语素集.添加词性词("方法_最近结果签名", "名词"); }
    inline const 词性节点类* 特征_方法结果分裂次数() { return 语素集.添加词性词("方法_结果分裂次数", "名词"); }
    inline const 词性节点类* 特征_方法连续稳定次数() { return 语素集.添加词性词("方法_连续稳定次数", "名词"); }

    inline I64 压缩结果签名I64(std::uint64_t x) noexcept {
        return static_cast<I64>(x & 0x7fffffffffffffffULL);
    }
}

// = (存在节点类*)世界树.添加存在("自我", "自我");
using namespace 数据仓库模块;
export  class 自我线程类 {
public:
    自我线程类() = default;
    ~自我线程类() { 停止(); }
    void 启动();
    void 停止();
    void 等待线程结束();
    bool 是否正在运行() const;
    void 请求退出();
private:
    std::chrono::milliseconds 心跳周期_{ 200 };

    // ==== 世界树指针（运行时填充）====
    存在节点类* 自我指针 = nullptr;

    void 自我初始化();
    //私有消息信息
public:
    enum class 消息类型 {
        任务创建 = 0,        // payload: 任务
        任务开始 = 1,        // payload: task_id
        任务进度 = 2,        // payload: task_id, progress [0,1]
        任务完成 = 3,        // payload: task_id, 成功标志
        任务失败 = 4,        // payload: task_id
        任务异步回执 = 10,   // payload: 任务节点指针, 标志=成功/失败, 数值=进度/结果, 文本=摘要
        需求观测 = 5,        // payload: demand_key, 强度
        权重外部调整 = 6,    // payload: demand_key, 新权重(可选)
        心跳 = 7,            // 无payload，用于周期性维护（也可内部触发）
        紧急中断 = 8,        // payload: task_id, 数值=抢占强度(默认1.0), 标志=是否强制抢占
        调度 = 9           // 内部调度：尝试执行一个“就绪任务”的下一步
    };


    struct 任务消息 {
        消息类型 类型{};
        std::string 任务ID;     // 可空
        任务节点类* 任务节点指针 = nullptr; // 可空：优先用于内部（避免用字符串反查）
        std::string 需求键;     // 可空
        double 数值 = 0.0;       // 复用：进度/观测强度/新权重
        std::uint64_t 序号 = 0;  // 复用：异步回执序号（0=不区分序号）
        bool 标志 = false;       // 复用：成功/失败等
        std::string 文本;        // 可空：摘要/解释/外部句柄
    };
private:
    // ==== 内部状态 ====
    std::atomic<bool> stop_{ false };
    std::thread self_thread_{};

    // 线程安全消息队列
    std::mutex mtx_;
    std::condition_variable cv_;
    std::deque<任务消息> 队列_;

    // ===== 工作者线程（0~2条）：动作执行 / 学习采样 =====
    // 说明：
    // - 自我线程只做调度与决策（轻量、不可阻塞）
    // - 工作者线程负责重活：执行方法/单步推进/学习采样
    // - 默认：动作线程=开；学习线程=开（可关）
    //
    // 注意：这里不再做“线程池”并发；先把闭环跑稳，再考虑扩展并发度。

    bool 启用动作线程_ = true;
    bool 启用学习线程_ = true;

    std::thread 动作线程_{};
    std::thread 学习线程_{};

    // 动作执行队列（复用原“执行队列”结构：按优先级出队）
    std::mutex 执行队列锁_;
    std::condition_variable 执行队列cv_;

    struct 执行队列项 {
        任务节点类* 节点 = nullptr;
        std::int64_t 优先级 = 0;
        时间戳 创建时间 = 0;
        std::uint64_t 入队序号 = 0;
    };
    struct 执行队列比较 {
        bool operator()(const 执行队列项& a, const 执行队列项& b) const {
            if (a.优先级 != b.优先级) return a.优先级 < b.优先级;         // 高优先级先出
            if (a.创建时间 != b.创建时间) return a.创建时间 > b.创建时间;  // 老任务先出
            return a.入队序号 > b.入队序号;                                // 稳定化
        }
    };
    std::priority_queue<执行队列项, std::vector<执行队列项>, 执行队列比较> 执行队列_{};
    std::atomic<std::uint64_t> 执行入队序号_{ 0 };

    // 学习队列（独立队列，避免学习任务挤占动作执行）
    std::mutex 学习队列锁_;
    std::condition_variable 学习队列cv_;
    std::priority_queue<执行队列项, std::vector<执行队列项>, 执行队列比较> 学习队列_{};
    std::atomic<std::uint64_t> 学习入队序号_{ 0 };

    // 可选：限制队列膨胀（0=不限制，由内存决定）
    std::size_t 最大待执行队列长度_ = 0;
    // 可选：每次心跳最多派发多少个任务（避免一次心跳锁太久）
    std::size_t 每次心跳最大派发数_ = 256;

    // 统计：当前正在“工作线程”里执行的任务数（仅用于观测）
    std::atomic<std::size_t> 正在执行线程数_{ 0 };
    // 规则计算下沉到自我类，自我线程只负责调用与写回。
    自我类 自我核心_{};

    // 自我线程内部“焦点”记录（便于调试/记忆/外显）
    std::string 执行焦点任务ID_{};
    std::string 强制抢占任务ID_{};  // 紧急中断时短暂强制执行
    std::chrono::steady_clock::time_point 上次维护_ = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point 上次摘要_ = std::chrono::steady_clock::now();


    // ====== 最小可执行闭环：初始需求 / 初始任务 ======
    需求节点类* 安全需求节点_ = nullptr;
    需求节点类* 服务需求节点_ = nullptr;

    任务节点类* 安全任务节点_ = nullptr;
    任务节点类* 服务任务节点_ = nullptr;
    任务节点类* 学习任务节点_ = nullptr;

    二次特征节点类* 安全目标判定节点_ = nullptr;
    二次特征节点类* 服务目标判定节点_ = nullptr;

    std::int64_t 目标安全值I64_ = 0;
    std::int64_t 目标服务值I64_ = 0;
    std::int64_t 待学习方法数量_ = 0;

    bool 初始需求任务已就绪_ = false;

    // ====== 任务调度器：事件驱动（替代全量扫描任务链） ======
    任务调度器 调度器_{};
    bool 调度器已初始化_ = false;

    // ====== 内部：初始化 + 调度 ======
    void 初始化_初始需求与任务();
    void 启动工作线程();
    void 停止工作线程();
    void 动作线程入口();
    void 学习线程入口();

    void 心跳调度_启动就绪任务();
    void 私有_刷新根任务就绪状态();
    bool 私有_任务目标命中(const 任务信息基类& t) const;
    bool 私有_任务可调度(const 任务信息基类& t) const;
    void 私有_初始化自我特征(时间戳 now);
    void 私有_注册已有本能方法并更新待学习数量(时间戳 now);
    std::int64_t 私有_统计待学习方法数量() const;
    I64 私有_判定方法尝试状态(方法节点类* 方法首节点, bool* outHasCondition = nullptr, bool* outHasResult = nullptr) const;
    void 私有_更新方法尝试状态_按观察(方法节点类* 方法首节点, 场景节点类* 观察输出, bool 观察成功, 时间戳 now);
    bool 私有_尝试学习一步(时间戳 now);

    二次特征节点类* 私有_创建二次特征条件节点_值区间(const std::string& 度量链键, const I64区间& 区间, const std::string& 调用点);


    // ====== 可调参数 ======

    // 需求衰减（把未持续观测的需求逐步回落到目标）
    double 需求衰减速率_每秒_ = 0.05; // 每秒将 (当前-目标) 向目标靠近5%
    // 权重自适应步长
    double 权重学习率_ = 0.1;         // 越大，响应越快
    // 指数平滑
    double EMA_alpha_ = 0.2;
    // 优先级权重（线性模型系数）
    double W_需求合成_ = 1.0;         // 需求总权重项
    double W_紧急度_ = 0.6;
    double W_基础重要_ = 0.4;
    double W_成本_ = -0.2;
    double W_风险_ = -0.4;
    double W_安全_ = 0.5;         // 安全值越低，越需要偏保守（见下）


    ////////////////////////自我的方法函数//////////////////////////////////////

public:


    void 推送消息(struct 任务消息 msg);

    // ===== 外部异步回执入口（最小实现） =====
    // 说明：
    // - 当某个任务在“等待中”状态等待外部结果时，外部模块可调用此函数回传结果。
    // - 回执会把任务标记为“就绪”，从而在下一次心跳调度时继续执行下一步。
    void 推送任务异步回执(任务节点类* 任务节点, bool 成功, double 数值 = 0.0, const std::string& 摘要 = "");
    void 推送任务异步回执_带序号(任务节点类* 任务节点, std::uint64_t 序号, bool 成功, double 数值 = 0.0, const std::string& 摘要 = "");

    // ===== 性能配置（可选） =====
    // 说明：
    // - 这不是“精力限制”，只是线程池大小的工程限额（CPU/内存）。
    // - 建议在 启动() 之前调用；若已启动，会记录警告并尽量不破坏当前运行。
    void 设置最大待执行队列长度(std::size_t 队列长度);
    void 设置每次心跳最大派发数(std::size_t 派发数);
private:
    // ====== 内部工具 ======
    void 处理消息_任务(const 任务消息& m);
    void 处理消息_需求(const 任务消息& m);
    void 处理消息_权重(const 任务消息& m);

    void 周期性维护();                 // 与外部数据结构交互
    void 同步自我核心特征到世界树(const std::string& 调用点);
    void 更新安全值_基于任务(bool 完成, bool 成功);
    void 更新需求_基于观测(const std::string& key, double 强度);

    // 安全裁剪
    static double clip01(double x) { return std::clamp(x, 0.0, 1.0); }

};

// ==============================
// ✅ 自我线程模块：基础线程控制实现
// 说明：本模块先保证“启动/退出/停止/消息队列”可用，
// 具体的自我初始化/需求-任务逻辑可在后续逐步填充。
// ==============================

inline bool 自我线程类::是否正在运行() const {
    // joinable 表示线程对象存在；stop_ 为 true 时认为正在退出
    return self_thread_.joinable() && !stop_.load();
}

inline void 自我线程类::启动() {
    if (是否正在运行()) return;

    stop_.store(false);
    {
        std::lock_guard<std::mutex> lk(mtx_);
        // 启动时投递一次“调度”消息，让系统进入事件驱动推进
        队列_.push_back(任务消息{ 消息类型::调度 });
    }
    启动工作线程();

    // 启动工作线程
    self_thread_ = std::thread([this] {
        // 线程启动日志
        日志::运行("[自我线程] 线程启动");

        // 线程入口：先做一次初始化（如果你已有实现，就会在这里真正执行）
        try { 自我初始化(); }
        catch (...) {}

        while (!stop_.load()) {
            if (自我核心_.是否已消亡()) {
                日志::运行("[自我线程] 自我消亡，线程退出");
                请求退出();
                break;
            }

            任务消息 msg{};
            bool has = false;

            {
                std::unique_lock<std::mutex> lk(mtx_);
                bool 超时 = false;
                if (!cv_.wait_for(lk, 心跳周期_, [&] { return stop_.load() || !队列_.empty(); })) {
                    超时 = true; // 心跳触发
                }
                if (stop_.load()) break;

                if (!队列_.empty()) {
                    msg = std::move(队列_.front());
                    队列_.pop_front();
                    has = true;
                }
            }

            if (has) {
                // 按类型路由（你后续可完善具体处理）
                switch (msg.类型) {
                case 消息类型::任务创建:
                case 消息类型::任务开始:
                case 消息类型::任务进度:
                case 消息类型::任务完成:
                case 消息类型::任务失败:
                case 消息类型::任务异步回执:
                case 消息类型::紧急中断:
                    try { 处理消息_任务(msg); }
                    catch (...) {}
                    break;
                case 消息类型::需求观测:
                    try { 处理消息_需求(msg); }
                    catch (...) {}
                    break;
                case 消息类型::权重外部调整:
                    try { 处理消息_权重(msg); }
                    catch (...) {}
                    break;
                case 消息类型::心跳:
                case 消息类型::调度:
                default:
                    break;
                }
            }
            // 低频维护：摘要/衰减等（不驱动任务执行）
            周期性维护();

            // 事件驱动调度：推进“就绪任务”的下一步
            心跳调度_启动就绪任务();
        }

        // 线程结束日志
        日志::运行("[自我线程] 线程结束");
        });
}

inline void 自我线程类::请求退出() {
    stop_.store(true);

    // 唤醒消息等待
    cv_.notify_all();
    // 唤醒执行线程池等待
    执行队列cv_.notify_all();
    学习队列cv_.notify_all();
}

inline void 自我线程类::等待线程结束() {
    if (self_thread_.joinable()) {
        if (std::this_thread::get_id() == self_thread_.get_id()) {
            // 避免 self-join
            self_thread_.detach();
        }
        else {
            self_thread_.join();
        }
    }


    // 停止工作线程（动作/学习）
    try {
        if (动作线程_.joinable()) {
            if (std::this_thread::get_id() == 动作线程_.get_id()) 动作线程_.detach();
            else 动作线程_.join();
        }
        if (学习线程_.joinable()) {
            if (std::this_thread::get_id() == 学习线程_.get_id()) 学习线程_.detach();
            else 学习线程_.join();
        }
    }
    catch (...) {}
}


inline void 自我线程类::停止() {
    请求退出();
    等待线程结束();
}

inline void 自我线程类::推送消息(struct 任务消息 msg) {
    {
        std::lock_guard<std::mutex> lk(mtx_);
        队列_.push_back(std::move(msg));
    }
    cv_.notify_one();
}

inline void 自我线程类::推送任务异步回执(任务节点类* 任务节点, bool 成功, double 数值, const std::string& 摘要) {
    任务消息 m{};
    m.类型 = 消息类型::任务异步回执;
    m.任务节点指针 = 任务节点;
    m.标志 = 成功;
    m.数值 = 数值;
    m.文本 = 摘要;

    // 任务ID 仅用于日志/UI，优先在处理端再补齐；这里可为空
    推送消息(std::move(m));
}
inline void 自我线程类::推送任务异步回执_带序号(任务节点类* 任务节点, std::uint64_t 序号, bool 成功, double 数值, const std::string& 摘要) {
    任务消息 m{};
    m.类型 = 消息类型::任务异步回执;
    m.任务节点指针 = 任务节点;
    m.序号 = 序号;
    m.标志 = 成功;
    m.数值 = 数值;
    m.文本 = 摘要;
    推送消息(std::move(m));
}





inline void 自我线程类::设置最大待执行队列长度(std::size_t 队列长度) {
    最大待执行队列长度_ = 队列长度;
}

inline void 自我线程类::设置每次心跳最大派发数(std::size_t 派发数) {
    每次心跳最大派发数_ = std::max<std::size_t>(1, 派发数);
}

// ==============================
// ✅ 自我线程：最小可运行实现
// 说明：
// - 先实现“能跑 + 能更新 + 能选焦点 + 能抢占”的闭环。
// - 与世界树/需求树/任务树的真实写回，先用日志作为“虚拟实现”。
// - 你后续只需要把：更新需求树 / 更新任务树 / 写自我特征值 这三处替换掉即可。
// ==============================

inline void 自我线程类::自我初始化() {
    const 时间戳 now = 结构体_时间戳::当前_微秒();
    特征类型定义类::初始化特征类型定义模块_依赖语素();

    自我核心_.初始化自我("自我线程类::自我初始化/自我核心");

    私有_初始化自我特征(now);
    私有_注册已有本能方法并更新待学习数量(now);
    初始化_初始需求与任务();
    同步自我核心特征到世界树("自我线程类::自我初始化/同步安全服务值");

    日志::运行f("[自我线程] 自我初始化完成: self={}, 待学习方法数量={}, 安全值={}, 服务值={}",
        (void*)自我指针,
        待学习方法数量_,
        自我核心_.安全值(),
        自我核心_.服务值());
}

inline void 自我线程类::私有_初始化自我特征(时间戳 now) {
    特征类型定义类::初始化特征类型定义模块_依赖语素();

    if (!世界树.虚拟世界) {
        const 词性节点类* 虚拟世界名 = 语素集.添加词性词("虚拟世界", "名词");
        世界树.虚拟世界 = 世界树.取或创建子场景_按名称(nullptr, 虚拟世界名, now, "自我线程类::初始化自我特征/虚拟世界");
    }

    const 词性节点类* 自我类型 = 语素集.添加词性词("自我", "名词");
    自我指针 = 世界树.取或创建子存在_按类型(世界树.虚拟世界, 自我类型, now, "自我线程类::初始化自我特征/自我存在");
    世界树.自我指针 = 自我指针;

    if (!自我指针) return;

    (void)世界树.写入特征_I64(自我指针, 特征类型定义类::类型_自我_待学习方法数量, 0, {}, "自我线程类::初始化自我特征/待学习方法数量");
}

inline I64 自我线程类::私有_判定方法尝试状态(方法节点类* 方法首节点, bool* outHasCondition, bool* outHasResult) const {
    bool hasCondition = false;
    bool hasResult = false;

    if (方法首节点) {
        锁调度器守卫 锁({
            锁请求::读(方法链.链表锁, 枚举_锁域::方法链, "方法链", "自我线程类::判定方法尝试状态")
        });

        auto 遍历同层 = [](方法节点类* first, auto&& fn) {
            if (!first) return;
            auto* cur = first;
            do {
                fn(cur);
                cur = static_cast<方法节点类*>(cur->下);
            } while (cur && cur != first);
        };

        std::vector<方法节点类*> stack;
        if (方法首节点->子) stack.push_back(static_cast<方法节点类*>(方法首节点->子));
        while (!stack.empty()) {
            方法节点类* first = stack.back();
            stack.pop_back();
            遍历同层(first, [&](方法节点类* node) {
                if (!node) return;
                if (dynamic_cast<方法条件节点主信息类*>(node->主信息)) hasCondition = true;
                if (dynamic_cast<方法结果节点主信息类*>(node->主信息)) hasResult = true;
                if (node->子) stack.push_back(static_cast<方法节点类*>(node->子));
            });
            if (hasCondition && hasResult) break;
        }
    }

    if (outHasCondition) *outHasCondition = hasCondition;
    if (outHasResult) *outHasResult = hasResult;

    if (!hasCondition || !hasResult) return 自我线程模块_detail::方法尝试状态_未尝试;

    auto* ve = 方法集.取或创建_方法虚拟存在(方法首节点, 0, "自我线程类::判定方法尝试状态/方法虚拟存在");
    if (!ve) return 自我线程模块_detail::方法尝试状态_未尝试;

    auto s = 世界树.读取特征_I64(ve, 自我线程模块_detail::特征_方法尝试状态(), "自我线程类::判定方法尝试状态/读状态");
    if (!s) return 自我线程模块_detail::方法尝试状态_未尝试;
    return *s;
}

inline std::int64_t 自我线程类::私有_统计待学习方法数量() const {
    std::vector<方法节点类*> heads;
    {
        锁调度器守卫 锁({
            锁请求::读(方法链.链表锁, 枚举_锁域::方法链, "方法链", "自我线程类::统计待学习方法数量")
            });

        auto* root = 方法链.根指针;
        if (!root || !root->子) return 0;

        auto 遍历同层 = [](方法节点类* first, auto&& fn) {
            if (!first) return;
            auto* cur = first;
            do {
                fn(cur);
                cur = static_cast<方法节点类*>(cur->下);
            } while (cur && cur != first);
        };

        遍历同层(static_cast<方法节点类*>(root->子), [&](方法节点类* head) {
            if (!head || !dynamic_cast<方法首节点主信息类*>(head->主信息)) return;
            heads.push_back(head);
        });
    }

    std::int64_t count = 0;
    for (auto* head : heads) {
        if (私有_判定方法尝试状态(head, nullptr, nullptr) != 自我线程模块_detail::方法尝试状态_尝试完成) ++count;
    }

    return count;
}

inline void 自我线程类::私有_更新方法尝试状态_按观察(方法节点类* 方法首节点, 场景节点类* 观察输出, bool 观察成功, 时间戳 now) {
    if (!方法首节点) return;

    auto* ve = 方法集.取或创建_方法虚拟存在(方法首节点, now, "自我线程类::更新方法尝试状态/方法虚拟存在");
    if (!ve) return;

    bool hasCondition = false;
    bool hasResult = false;
    (void)私有_判定方法尝试状态(方法首节点, &hasCondition, &hasResult);

    const auto typeStatus = 自我线程模块_detail::特征_方法尝试状态();
    const auto typeLastSig = 自我线程模块_detail::特征_方法最近结果签名();
    const auto typeSplit = 自我线程模块_detail::特征_方法结果分裂次数();
    const auto typeStable = 自我线程模块_detail::特征_方法连续稳定次数();

    I64 splitCount = 世界树.读取特征_I64(ve, typeSplit, "自我线程类::更新方法尝试状态/读分裂次数").value_or(0);
    I64 stableCount = 世界树.读取特征_I64(ve, typeStable, "自我线程类::更新方法尝试状态/读稳定次数").value_or(0);
    auto lastSig = 世界树.读取特征_I64(ve, typeLastSig, "自我线程类::更新方法尝试状态/读结果签名");

    I64 status = 自我线程模块_detail::方法尝试状态_尝试中;
    if (!hasCondition || !hasResult) {
        status = 自我线程模块_detail::方法尝试状态_未尝试;
    }
    else if (观察成功 && 观察输出) {
        const I64 currentSig = 自我线程模块_detail::压缩结果签名I64(方法集.生成结果签名_基础(观察输出, 96, "自我线程类::更新方法尝试状态/结果签名"));
        (void)世界树.写入特征_I64(ve, typeLastSig, currentSig, {}, "自我线程类::更新方法尝试状态/写结果签名");

        if (!lastSig.has_value()) {
            stableCount = 1;
            status = 自我线程模块_detail::方法尝试状态_尝试中;
        }
        else if (*lastSig != currentSig) {
            ++splitCount;
            stableCount = 0;
            status = 自我线程模块_detail::方法尝试状态_待重新验证;
        }
        else {
            stableCount += 1;
            status = (stableCount >= 3)
                ? 自我线程模块_detail::方法尝试状态_尝试完成
                : 自我线程模块_detail::方法尝试状态_尝试中;
        }
    }

    (void)世界树.写入特征_I64(ve, typeSplit, splitCount, {}, "自我线程类::更新方法尝试状态/写分裂次数");
    (void)世界树.写入特征_I64(ve, typeStable, stableCount, {}, "自我线程类::更新方法尝试状态/写稳定次数");
    (void)世界树.写入特征_I64(ve, typeStatus, status, {}, "自我线程类::更新方法尝试状态/写状态");
}

inline void 自我线程类::私有_注册已有本能方法并更新待学习数量(时间戳 now) {
    外设本能方法类_相机模块{}.注册默认本能动作_外设相机();
    自我本能方法类_相机认知模块{}.注册默认本能动作_自我相机认知();

    for (auto id : 本能集.枚举已注册ID()) {
        auto* head = 方法集.查找或创建_本能方法首节点(static_cast<std::uint64_t>(id), now, "自我线程类::注册已有本能方法");
        if (!head) continue;
        (void)本能集.补全方法信息(id, head);

        auto* ve = 方法集.取或创建_方法虚拟存在(head, now, "自我线程类::注册已有本能方法/方法虚拟存在");
        if (ve && !世界树.读取特征_I64(ve, 自我线程模块_detail::特征_方法尝试状态(), "自我线程类::注册已有本能方法/读尝试状态").has_value()) {
            (void)世界树.写入特征_I64(ve, 自我线程模块_detail::特征_方法尝试状态(), 自我线程模块_detail::方法尝试状态_未尝试, {}, "自我线程类::注册已有本能方法/写尝试状态");
            (void)世界树.写入特征_I64(ve, 自我线程模块_detail::特征_方法结果分裂次数(), 0, {}, "自我线程类::注册已有本能方法/写分裂次数");
            (void)世界树.写入特征_I64(ve, 自我线程模块_detail::特征_方法连续稳定次数(), 0, {}, "自我线程类::注册已有本能方法/写稳定次数");
        }
    }

    待学习方法数量_ = 私有_统计待学习方法数量();
    if (自我指针 && 特征类型定义类::类型_自我_待学习方法数量) {
        (void)世界树.写入特征_I64(自我指针, 特征类型定义类::类型_自我_待学习方法数量, 待学习方法数量_, {}, "自我线程类::注册已有本能方法/待学习方法数量");
    }
}




inline bool 自我线程类::私有_尝试学习一步(时间戳 now) {
    方法节点类* targetHead = nullptr;
    方法节点类* firstCondition = nullptr;
    bool missingCondition = false;
    bool missingResult = false;
    std::string actionKey{};
    int bestScore = -1;
    I64 bestRunCount = (std::numeric_limits<I64>::max)();
    I64 bestLastCallTime = (std::numeric_limits<I64>::max)();
    struct 候选方法信息 {
        方法节点类* head = nullptr;
        方法节点类* firstCondition = nullptr;
        bool hasCondition = false;
        bool hasResult = false;
        std::string actionKey{};
    };
    std::vector<候选方法信息> 候选列表;

    {
        锁调度器守卫 锁({
            锁请求::读(方法链.链表锁, 枚举_锁域::方法链, "方法链", "自我线程类::尝试学习一步")
            });

        auto* root = 方法链.根指针;
        if (root && root->子) {
            auto 遍历同层 = [](方法节点类* first, auto&& fn) {
                if (!first) return;
                auto* cur = first;
                do {
                    fn(cur);
                    cur = static_cast<方法节点类*>(cur->下);
                } while (cur && cur != first);
            };

            遍历同层(static_cast<方法节点类*>(root->子), [&](方法节点类* head) {
                if (!head) return;
                auto* hmi = dynamic_cast<方法首节点主信息类*>(head->主信息);
                if (!hmi) return;

                bool hasCondition = false;
                bool hasResult = false;
                方法节点类* localFirstCondition = nullptr;
                std::vector<方法节点类*> stack;
                if (head->子) stack.push_back(static_cast<方法节点类*>(head->子));

                while (!stack.empty()) {
                    方法节点类* first = stack.back();
                    stack.pop_back();
                    遍历同层(first, [&](方法节点类* node) {
                        if (!node) return;
                        if (!localFirstCondition) {
                            if (dynamic_cast<方法条件节点主信息类*>(node->主信息)) localFirstCondition = node;
                        }
                        if (dynamic_cast<方法条件节点主信息类*>(node->主信息)) hasCondition = true;
                        if (dynamic_cast<方法结果节点主信息类*>(node->主信息)) hasResult = true;
                        if (node->子) stack.push_back(static_cast<方法节点类*>(node->子));
                    });
                    if (hasCondition && hasResult) break;
                }

                候选方法信息 候选{};
                候选.head = head;
                候选.firstCondition = localFirstCondition;
                候选.hasCondition = hasCondition;
                候选.hasResult = hasResult;
                if (hmi->动作名) 候选.actionKey = hmi->动作名->获取主键();
                候选列表.push_back(std::move(候选));
            });
        }
    }

    for (const auto& 候选 : 候选列表) {
        if (!候选.head) continue;

        I64 tryStatus = 自我线程模块_detail::方法尝试状态_未尝试;
        I64 runCount = 0;
        I64 lastCallTime = 0;
        if (auto* bmi = dynamic_cast<方法信息基类*>(候选.head->主信息)) {
            auto* ve = bmi->方法虚拟存在根;
            if (ve) {
                tryStatus = 世界树.读取特征_I64(
                    ve,
                    自我线程模块_detail::特征_方法尝试状态(),
                    "自我线程类::尝试学习一步/读尝试状态").value_or(自我线程模块_detail::方法尝试状态_未尝试);
                runCount = 世界树.读取特征_I64(
                    ve,
                    语素集.添加词性词("节点_运行次数", "名词"),
                    "自我线程类::尝试学习一步/读运行次数").value_or(0);
                lastCallTime = 世界树.读取特征_I64(
                    ve,
                    语素集.添加词性词("方法_最后调用时间", "名词"),
                    "自我线程类::尝试学习一步/读最后调用时间").value_or(0);
            }
        }
        if (tryStatus == 自我线程模块_detail::方法尝试状态_尝试完成) continue;

        int score = 1;
        if (!候选.hasCondition) score += 2;
        if (!候选.hasResult) score += 2;
        if (tryStatus == 自我线程模块_detail::方法尝试状态_待重新验证) score += 1;

        const bool 更优 =
            (score > bestScore) ||
            (score == bestScore && runCount < bestRunCount) ||
            (score == bestScore && runCount == bestRunCount && lastCallTime < bestLastCallTime);
        if (!更优) continue;

        bestScore = score;
        bestRunCount = runCount;
        bestLastCallTime = lastCallTime;
        targetHead = 候选.head;
        firstCondition = 候选.firstCondition;
        missingCondition = !候选.hasCondition;
        missingResult = !候选.hasResult;
        actionKey = 候选.actionKey;
    }

    bool progressed = false;
    bool observed = false;
    I64 observedCount = 0;
    I64 updatedCount = 0;
    I64 createdCount = 0;
    场景节点类* 学习结果场景 = nullptr;

    if (targetHead) {
        auto* hmi = dynamic_cast<方法首节点主信息类*>(targetHead->主信息);
        auto* 方法存在 = 方法集.取或创建_方法虚拟存在(targetHead, now, "自我线程类::尝试学习一步/方法虚拟存在");
        auto* 学习场景根 = 世界树.虚拟世界;
        状态节点类* 目标结果状态 = nullptr;

        if (方法存在 && 学习场景根) {
            auto* 成功特征类型 = 语素集.添加词性词("方法_最近成功标记", "名词");
            auto* 目标特征 = 世界树.确保特征(
                方法存在,
                成功特征类型,
                成功特征类型,
                "自我线程类::尝试学习一步/目标特征");
            if (目标特征) {
                auto* mi = new 场景节点主信息类();
                mi->名称 = 语素集.添加词性词(std::string("学习_方法目标_") + std::to_string(now), "名词");
                auto* 目标场景 = 世界树.创建场景(学习场景根, mi, "自我线程类::尝试学习一步/目标场景");
                if (目标场景) {
                    目标结果状态 = 状态集.创建内部状态(目标场景, 方法存在, 目标特征, 特征快照值{ (I64)1 }, 枚举_存在状态事件::创建, false, now);
                }
            }
        }

        std::vector<状态节点类*> 空条件状态;
        auto 参数结果 = 方法集.生成尝试学习参数(
            targetHead,
            空条件状态,
            目标结果状态,
            8,
            now,
            "自我线程类::尝试学习一步/生成参数");

        if (参数结果.参数场景) {
            方法集.同步场景特征到方法虚拟存在(targetHead, 参数结果.参数场景, now, "自我线程类::尝试学习一步/同步参数到方法虚拟存在");
        }

        auto* 尝试条件节点 = 方法集.取或创建_条件节点_按场景(
            targetHead,
            参数结果.参数场景,
            now,
            64,
            {},
            "自我线程类::尝试学习一步/取或创建尝试条件节点");
        if (尝试条件节点) {
            firstCondition = 尝试条件节点;
            progressed = true;
        }
        else if (missingCondition) {
            firstCondition = 方法集.取或创建_条件节点_按签名(targetHead, 0ull, now, 64, "自我线程类::尝试学习一步/补条件");
            progressed = (firstCondition != nullptr) || progressed;
        }

        if (hmi && 参数结果.参数场景 && hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID && hmi->动作句柄.本能ID_U64 != 0) {
            auto* outMi = new 场景节点主信息类();
            outMi->名称 = 语素集.添加词性词(std::string("学习_执行输出_") + std::to_string(now), "名词");
            outMi->类型 = 语素集.添加词性词("学习过程场景", "名词");
            outMi->最后观测时间 = now;
            auto* 输出场景 = 学习场景根 ? 世界树.创建场景(学习场景根, outMi, "自我线程类::尝试学习一步/执行输出") : nullptr;

            结构_方法上下文 ctx{};
            ctx.now = now;
            ctx.方法首节点 = targetHead;
            ctx.输入观测包场景 = 参数结果.参数场景;
            ctx.任务条件场景 = 参数结果.参数场景;
            ctx.调用覆盖场景 = 输出场景;
            ctx.当前场景 = 参数结果.参数场景;
            ctx.条件场景 = 参数结果.参数场景;

            std::atomic_bool cancel{ false };
            auto r = 方法集.调用本能函数(hmi->动作句柄.本能ID_U64, ctx, cancel);
            observed = r.成功;
            学习结果场景 = r.结果存在集场景 ? r.结果存在集场景 : (r.结果事件集场景 ? r.结果事件集场景 : 输出场景);

            if (学习结果场景) {
                auto* smi = 学习结果场景->主信息 ? dynamic_cast<场景节点主信息类*>(学习结果场景->主信息) : nullptr;
                if (smi) {
                    observedCount = static_cast<I64>(smi->状态列表.size());
                    updatedCount = static_cast<I64>(smi->动态列表.size());
                    createdCount = static_cast<I64>(smi->状态列表.size());
                }
            }

            if (!observed) {
                学习结果场景 = 方法集.创建方法失败结果场景(targetHead, now, "自我线程类::尝试学习一步/失败结果场景");
            }

            if (学习结果场景) {
                auto* parent = firstCondition ? firstCondition : targetHead;
                const std::uint64_t resSig = 方法集.生成结果签名_基础(
                    学习结果场景,
                    96,
                    observed ? "自我线程类::尝试学习一步/真实结果签名" : "自我线程类::尝试学习一步/失败结果签名");
                auto* resultNode = 方法集.取或创建_结果节点_按签名(
                    parent,
                    resSig,
                    学习结果场景,
                    now,
                    64,
                    observed ? "自我线程类::尝试学习一步/取或创建真实结果" : "自我线程类::尝试学习一步/取或创建失败结果");
                if (resultNode) 方法集.吸收结果节点摘要到影响签名(targetHead, resultNode, now, "自我线程类::尝试学习一步/吸收影响签名");
                progressed = (resultNode != nullptr) || progressed;
            }
            progressed = observed || progressed;
            私有_更新方法尝试状态_按观察(targetHead, 学习结果场景, observed, now);
        }
        else if (targetHead) {
            私有_更新方法尝试状态_按观察(targetHead, nullptr, false, now);
        }
    }

    待学习方法数量_ = 私有_统计待学习方法数量();
    if (自我指针 && 特征类型定义类::类型_自我_待学习方法数量) {
        (void)世界树.写入特征_I64(自我指针, 特征类型定义类::类型_自我_待学习方法数量, 待学习方法数量_, {}, "自我线程类::尝试学习一步/待学习方法数量");
    }

    日志::运行f("[自我线程] 尝试学习一步: 动作={}, 补条件={}, 补结果={}, 观察成功={}, 候选={}, 更新={}, 新建={}, 剩余待学习方法数量={}",
        actionKey.empty() ? std::string("<unknown>") : actionKey,
        missingCondition,
        missingResult,
        observed,
        observedCount,
        updatedCount,
        createdCount,
        待学习方法数量_);
    return progressed || observed;
}

inline void 自我线程类::处理消息_需求(const 任务消息& m) {
    if (m.需求键.empty()) return;

   

    日志::运行f("[自我线程] 需求观测: key={}, 强度={:.3f}", m.需求键, m.数值);
}

inline void 自我线程类::处理消息_权重(const 任务消息& m) {
    if (m.需求键.empty()) return;
    // 现在直接与外部需求树交互
  //  日志::运行f("[自我线程] 权重外部调整: key={}, 权重={:.3f}", m.需求键, m.数值);
}

inline void 自我线程类::处理消息_任务(const 任务消息& m) {
    switch (m.类型) {
    case 消息类型::任务完成:
        更新安全值_基于任务(true, m.标志);
        break;
    case 消息类型::任务失败:
        更新安全值_基于任务(true, false);
        break;
    case 消息类型::任务异步回执:
        更新安全值_基于任务(true, m.标志);
        break;
    case 消息类型::紧急中断:
        自我核心_.安全值减少(1);
        同步自我核心特征到世界树("自我线程类::处理消息_任务/紧急中断");
        日志::运行f("[自我线程] 紧急中断影响安全值: 任务ID={}, 安全值={}, 服务值={}",
            m.任务ID,
            自我核心_.安全值(),
            自我核心_.服务值());
        break;
    default:
        break;
    }
}

inline void 自我线程类::更新需求_基于观测(const std::string& key, double 强度) {
    // 现在直接与外部需求树交互
    日志::运行f("[自我线程] 更新需求: key={}, 强度={:.3f}", key, 强度);
}

inline void 自我线程类::同步自我核心特征到世界树(const std::string& 调用点) {
    if (!自我指针) return;
    const auto 转I64饱和 = [](U64 v) -> I64 {
        const U64 上限 = static_cast<U64>((std::numeric_limits<I64>::max)());
        return static_cast<I64>(std::min<U64>(v, 上限));
    };

    if (特征类型定义类::类型_自我_安全值) {
        (void)世界树.写入特征_I64(
            自我指针,
            特征类型定义类::类型_自我_安全值,
            转I64饱和(自我核心_.安全值()),
            {},
            调用点 + "/写安全值");
    }
    if (特征类型定义类::类型_自我_服务值) {
        (void)世界树.写入特征_I64(
            自我指针,
            特征类型定义类::类型_自我_服务值,
            转I64饱和(自我核心_.服务值()),
            {},
            调用点 + "/写服务值");
    }
}

inline void 自我线程类::更新安全值_基于任务(bool 完成, bool 成功) {
    if (!完成) return;

    const U64 安全前 = 自我核心_.安全值();
    const U64 服务前 = 自我核心_.服务值();

    自我核心_.应用任务结果_默认策略(成功);
    同步自我核心特征到世界树("自我线程类::更新安全值_基于任务");

    日志::运行f("[自我线程] 任务结果更新安全服务值: 成功={}, 安全值 {} -> {}, 服务值 {} -> {}",
        成功 ? 1 : 0,
        安全前,
        自我核心_.安全值(),
        服务前,
        自我核心_.服务值());
}

inline void 自我线程类::周期性维护() {
    const U64 安全前 = 自我核心_.安全值();
    const U64 服务值 = 自我核心_.服务值();
    const U64 正向步长前 = 自我核心_.时序正向步长();
    const U64 反向步长前 = 自我核心_.时序反向步长();

    U64 待完成任务数 = static_cast<U64>(正在执行线程数_.load());
    {
        std::lock_guard<std::mutex> lk(执行队列锁_);
        待完成任务数 += static_cast<U64>(执行队列_.size());
    }
    {
        std::lock_guard<std::mutex> lk(学习队列锁_);
        待完成任务数 += static_cast<U64>(学习队列_.size());
    }

    const U64 待学习任务数 = (待学习方法数量_ > 0) ? static_cast<U64>(待学习方法数量_) : 0;
    const 时间戳 now = 结构体_时间戳::当前_微秒();

    自我核心_.按时序规则更新安全值(
        待完成任务数,
        待学习任务数,
        now,
        "自我线程类::周期性维护");

    同步自我核心特征到世界树("自我线程类::周期性维护");

    const U64 安全后 = 自我核心_.安全值();
    const U64 正向步长后 = 自我核心_.时序正向步长();
    const U64 反向步长后 = 自我核心_.时序反向步长();

    if (安全前 != 安全后 || 正向步长前 != 正向步长后 || 反向步长前 != 反向步长后) {
        日志::运行f(
            "[自我线程] 周期维护安全值: 安全值 {} -> {}, 服务值={}, 正向步长={}, 反向步长={}, 待完成={}, 待学习={}, 待机={}",
            安全前,
            安全后,
            服务值,
            正向步长后,
            反向步长后,
            待完成任务数,
            待学习任务数,
            自我核心_.是否待机状态() ? 1 : 0);
    }
}






// ============================================================
// ✅ 初始需求/任务 + 优先级调度（最小可执行闭环）
// ============================================================

inline 二次特征节点类* 自我线程类::私有_创建二次特征条件节点_值区间(
    const std::string& 度量链键,
    const I64区间& 区间,
    const std::string& 调用点)
{
 
}


inline bool 自我线程类::私有_任务可调度(const 任务信息基类& t) const {
    if (t.状态 == 枚举_任务状态::完成) return false;
    if (t.状态 == 枚举_任务状态::取消) return false;
    if (t.状态 == 枚举_任务状态::失败) return false;
    if (t.状态 == 枚举_任务状态::执行中) return false;
    if (t.状态 == 枚举_任务状态::筹办中) return false;
    if (t.状态 == 枚举_任务状态::排队中) return false;
    if (t.状态 == 枚举_任务状态::等待中) return false;
    if (t.状态 == 枚举_任务状态::挂起) return false;
    return true;
}

inline void 自我线程类::初始化_初始需求与任务() {
    const 时间戳 now = 结构体_时间戳::当前_微秒();
    const 词性节点类* 学习任务名 = 语素集.添加词性词("学习_尝试学习", "名词");
    const 词性节点类* 学习任务类型 = 语素集.添加词性词("尝试学习任务", "名词");

    if (!学习任务节点_) {
        auto* info = new 任务头结点信息();
        info->节点种类 = 枚举_任务节点种类::头结点;
        info->名称 = 学习任务名;
        info->类型 = 学习任务类型;
        info->状态 = 枚举_任务状态::未启动;
        info->创建时间 = now;
        info->调度优先级 = (待学习方法数量_ > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
        info->任务树类型 = 枚举_任务树类型::叶子任务;
        info->场景 = 世界树.虚拟世界;
        info->任务虚拟存在 = nullptr;
        info->需求 = nullptr;

        学习任务节点_ = 任务类::新建任务节点(info, nullptr, "自我线程类::初始化_初始需求与任务/学习任务");
    }

    if (!学习任务节点_ || !学习任务节点_->主信息) return;

    学习任务节点_->主信息->名称 = 学习任务名;
    学习任务节点_->主信息->类型 = 学习任务类型;
    学习任务节点_->主信息->创建时间 = now;
    学习任务节点_->主信息->调度优先级 = (待学习方法数量_ > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
    学习任务节点_->主信息->状态 = 枚举_任务状态::排队中;
    学习任务节点_->主信息->任务树类型 = 枚举_任务树类型::叶子任务;
    学习任务节点_->主信息->场景 = 世界树.虚拟世界;

    {
        std::lock_guard<std::mutex> lk(学习队列锁_);
        学习队列_.push(执行队列项{
            学习任务节点_,
            学习任务节点_->主信息->调度优先级,
            now,
            ++学习入队序号_
        });
    }
    学习队列cv_.notify_one();

    日志::运行f("[自我线程] 已生成尝试学习任务并入学习队列: 节点={}, 优先级={}, 待学习方法数量={}",
        (void*)学习任务节点_,
        学习任务节点_->主信息->调度优先级,
        待学习方法数量_);
}

inline void 自我线程类::私有_刷新根任务就绪状态() {
  
}

bool 自我线程类::私有_任务目标命中(const 任务信息基类& t) const
{
    return false;
}

inline void 自我线程类::启动工作线程() {
    // 0~2 工作者线程：动作执行 / 学习采样
    if (启用动作线程_) {
        if (!动作线程_.joinable()) {
            动作线程_ = std::thread([this] { 动作线程入口(); });
            日志::运行("[自我线程] 动作线程启动");
        }
    }
    if (启用学习线程_) {
        if (!学习线程_.joinable()) {
            学习线程_ = std::thread([this] { 学习线程入口(); });
            日志::运行("[自我线程] 学习线程启动");
        }
    }
}

inline void 自我线程类::停止工作线程() {
    // stop_ 由 请求退出 设置；这里只负责唤醒等待
    执行队列cv_.notify_all();
    学习队列cv_.notify_all();
}

inline void 自我线程类::心跳调度_启动就绪任务() {
    if (!学习任务节点_ || !学习任务节点_->主信息) return;

    auto* info = 学习任务节点_->主信息;
    if (info->状态 == 枚举_任务状态::排队中 || info->状态 == 枚举_任务状态::执行中 || info->状态 == 枚举_任务状态::等待中) return;

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    info->调度优先级 = (待学习方法数量_ > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
    info->创建时间 = now;
    info->状态 = 枚举_任务状态::排队中;

    {
        std::lock_guard<std::mutex> lk(学习队列锁_);
        学习队列_.push(执行队列项{
            学习任务节点_,
            info->调度优先级,
            now,
            ++学习入队序号_
        });
    }
    学习队列cv_.notify_one();
}

inline void 自我线程类::动作线程入口() {
  
}




export 自我线程类 自我;


inline void 自我线程类::学习线程入口() {
    while (!stop_.load()) {
        执行队列项 item{};
        bool has = false;

        {
            std::unique_lock<std::mutex> lk(学习队列锁_);
            学习队列cv_.wait(lk, [&] { return stop_.load() || !学习队列_.empty(); });
            if (stop_.load()) break;
            if (!学习队列_.empty()) {
                item = 学习队列_.top();
                学习队列_.pop();
                has = true;
            }
        }

        if (!has || !item.节点 || !item.节点->主信息) continue;

        auto* info = item.节点->主信息;
        info->状态 = 枚举_任务状态::执行中;
        ++正在执行线程数_;

        const 时间戳 now = 结构体_时间戳::当前_微秒();
        if (item.节点 == 学习任务节点_) {
            const auto before = 待学习方法数量_;
            const bool progressed = 私有_尝试学习一步(now);
            const auto after = 待学习方法数量_;

            info->调度优先级 = (after > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
            info->创建时间 = now;
            info->状态 = 枚举_任务状态::就绪;

            日志::运行f("[自我线程] 尝试学习任务执行一次: 节点={}, 有进展={}, 待学习方法数量 {} -> {}, 优先级={}",
                (void*)item.节点,
                progressed,
                before,
                after,
                info->调度优先级);
        }
        else {
            info->状态 = 枚举_任务状态::完成;
            日志::运行f("[自我线程] 学习线程执行了非驻留学习任务: 节点={}", (void*)item.节点);
        }

        if (正在执行线程数_.load() > 0) --正在执行线程数_;
    }
}















