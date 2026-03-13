
module;

export module 自我线程模块;

import 基础数据类型模块;
import 主信息定义模块;
import 世界树环境模块;
import 语素环境模块;
import 状态模块;

import 需求模块;
import 需求环境模块;
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
import 自我本能方法_最小试探模块;
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
import <variant>;

namespace 自我线程模块_detail {
    inline constexpr I64 方法尝试状态_未尝试 = 0;
    inline constexpr I64 方法尝试状态_尝试中 = 1;
    inline constexpr I64 方法尝试状态_尝试完成 = 2;
    inline constexpr I64 方法尝试状态_待重新验证 = 3;

    inline const 词性节点类* 特征_方法尝试状态() { return 语素集.添加词性词("方法_尝试状态", "名词"); }
    inline const 词性节点类* 特征_方法最近结果签名() { return 语素集.添加词性词("方法_最近结果签名", "名词"); }
    inline const 词性节点类* 特征_方法结果分裂次数() { return 语素集.添加词性词("方法_结果分裂次数", "名词"); }
    inline const 词性节点类* 特征_方法连续稳定次数() { return 语素集.添加词性词("方法_连续稳定次数", "名词"); }
    inline const 词性节点类* 特征_任务事实(const std::string& 键) { return 语素集.添加词性词("任务_事实_" + 键, "名词"); }
    inline const 词性节点类* 特征_任务方向(const std::string& 键) { return 语素集.添加词性词("任务_方向_" + 键, "名词"); }

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
    需求节点类* 安全顺向需求节点_ = nullptr;
    需求节点类* 服务顺向需求节点_ = nullptr;
    需求节点类* 安全服务同向需求节点_ = nullptr;

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
    任务执行器 根任务筹办器_{};
    bool 调度器已初始化_ = false;

    // ====== 内部：初始化 + 调度 ======
    void 初始化_初始需求与任务();
    void 启动工作线程();
    void 停止工作线程();
    void 动作线程入口();
    void 学习线程入口();

    void 心跳调度_启动就绪任务();
    void 私有_刷新根任务就绪状态();
    void 私有_推动根任务进入筹办(任务节点类* 任务节点, const std::string& 标签);
    void 私有_刷新基础方向概念与需求(const std::string& 调用点);
    bool 私有_任务目标命中(const 任务信息基类& t) const;
    bool 私有_任务可调度(const 任务信息基类& t) const;
    void 私有_初始化自我特征(时间戳 now);
    void 私有_注册已有本能方法并更新待学习数量(时间戳 now);
    std::int64_t 私有_统计待学习方法数量() const;
    I64 私有_判定方法尝试状态(方法节点类* 方法首节点, bool* outHasCondition = nullptr, bool* outHasResult = nullptr) const;
    void 私有_更新方法尝试状态_按观察(方法节点类* 方法首节点, 场景节点类* 观察输出, bool 观察成功, 时间戳 now);
    bool 私有_尝试学习一步(时间戳 now);
    std::vector<方法节点类*> 私有_生成根任务最小试探方法候选(const std::string& 标签, 时间戳 now);
    void 私有_尝试入队动作任务(任务节点类* 任务节点, const std::string& 标签);
    任务执行器::结构_叶子执行结果 私有_执行任务叶子步骤(任务节点类* 步骤节点);
    void 私有_同步方法事实到任务方向特征(任务节点类* 步骤节点, 枚举_本能动作ID 本能ID, 场景节点类* 输出场景, 时间戳 now);

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
    void 记录根方向摘要(const std::string& 调用点);
    void 校验根约束不变量(const std::string& 调用点);
    void 记录需求树不变量摘要(const std::string& 调用点);

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
                日志::运行f(
                    "[自我线程][不变量] INV-004: 安全值归零触发线程退出: 安全值={}, 服务值={}",
                    自我核心_.安全值(),
                    自我核心_.服务值());
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
    私有_刷新基础方向概念与需求("自我线程类::自我初始化");
    记录根方向摘要("自我线程类::自我初始化");
    校验根约束不变量("自我线程类::自我初始化");
    记录需求树不变量摘要("自我线程类::自我初始化");

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
    自我本能方法类_最小试探模块{}.注册默认本能动作_自我最小试探();
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




inline std::vector<方法节点类*> 自我线程类::私有_生成根任务最小试探方法候选(const std::string& 标签, 时间戳 now) {
    std::vector<方法节点类*> out{};
    auto 追加方法 = [&](枚举_本能动作ID id) {
        if (!本能集.有(id)) return;
        auto* head = 方法集.查找或创建_本能方法首节点(id, now, "自我线程类::私有_生成根任务最小试探方法候选/" + 标签);
        if (!head) return;
        方法集.初始化方法虚拟存在信息(head, now, "自我线程类::私有_生成根任务最小试探方法候选/" + 标签);
        if (std::find(out.begin(), out.end(), head) == out.end()) out.push_back(head);
    };

    if (标签 == "安全") {
        追加方法(枚举_本能动作ID::自我_读取核心状态);
        追加方法(枚举_本能动作ID::外设_获取相机状态);
        追加方法(枚举_本能动作ID::外设_读取深度相机状态);
        追加方法(枚举_本能动作ID::自我_观察场景);
        追加方法(枚举_本能动作ID::自我_停止当前试探并进入保守态);
    }
    else if (标签 == "服务") {
        追加方法(枚举_本能动作ID::自我_读取最新人类输入);
        追加方法(枚举_本能动作ID::自我_确认收到);
        追加方法(枚举_本能动作ID::自我_请求补充信息);
        追加方法(枚举_本能动作ID::外设_读取深度相机状态);
        追加方法(枚举_本能动作ID::自我_观察场景);
    }
    else {
        追加方法(枚举_本能动作ID::自我_读取核心状态);
        追加方法(枚举_本能动作ID::自我_观察场景);
    }

    return out;
}

inline void 自我线程类::私有_尝试入队动作任务(任务节点类* 任务节点, const std::string& 标签) {
    if (!任务节点 || !任务节点->主信息) return;
    auto* info = 任务节点->主信息;
    if (!私有_任务可调度(*info)) return;

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    if (最大待执行队列长度_ > 0) {
        std::lock_guard<std::mutex> lk(执行队列锁_);
        if (执行队列_.size() >= 最大待执行队列长度_) {
            日志::运行f("[自我线程] 动作队列已满，跳过根任务入队: 标签={}, 节点={}", 标签, (void*)任务节点);
            return;
        }
    }

    任务类::设置任务状态(任务节点, 枚举_任务状态::排队中, "自我线程类::私有_尝试入队动作任务/" + 标签);
    info->创建时间 = now;

    {
        std::lock_guard<std::mutex> lk(执行队列锁_);
        执行队列_.push(执行队列项{
            任务节点,
            info->调度优先级,
            now,
            ++执行入队序号_
        });
    }
    执行队列cv_.notify_one();
}

inline void 自我线程类::私有_同步方法事实到任务方向特征(任务节点类* 步骤节点, 枚举_本能动作ID 本能ID, 场景节点类* 输出场景, 时间戳 now) {
    auto* stepInfo = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;
    auto* headNode = stepInfo ? stepInfo->所属任务头结点 : nullptr;
    auto* headInfo = (headNode && headNode->主信息) ? dynamic_cast<任务头结点信息*>(headNode->主信息) : nullptr;
    if (!stepInfo || !输出场景) return;

    auto 记录I64到任务 = [&](任务信息基类* info, const std::string& 键, I64 值) {
        if (!info) return;
        const auto* factType = 自我线程模块_detail::特征_任务事实(键);
        const auto* dirType = 自我线程模块_detail::特征_任务方向(键);
        I64 direction = 0;
        if (info->任务虚拟存在 && factType) {
            auto oldValue = 世界树.读取特征_I64(info->任务虚拟存在, factType, "自我线程类::私有_同步方法事实到任务方向特征/读旧值/" + 键);
            if (oldValue.has_value()) {
                if (值 > *oldValue) direction = 1;
                else if (值 < *oldValue) direction = -1;
            }
        }
        (void)任务类::写任务虚拟存在_I64特征(info, factType, 值, now, "自我线程类::私有_同步方法事实到任务方向特征/写事实/" + 键);
        (void)任务类::写任务虚拟存在_I64特征(info, dirType, direction, now, "自我线程类::私有_同步方法事实到任务方向特征/写方向/" + 键);
    };

    auto 记录指针到任务 = [&](任务信息基类* info, const std::string& 键, std::uintptr_t ptr) {
        if (!info || ptr == 0) return;
        const auto* factType = 自我线程模块_detail::特征_任务事实(键);
        (void)任务类::写任务虚拟存在_指针特征(info, factType, ptr, now, "自我线程类::私有_同步方法事实到任务方向特征/写指针/" + 键);
    };

    auto 写双层I64 = [&](const std::string& 键, const 词性节点类* 输出特征类型) {
        if (!输出特征类型) return;
        auto value = 世界树.读取特征_I64(输出场景, 输出特征类型, "自我线程类::私有_同步方法事实到任务方向特征/读输出/" + 键);
        if (!value.has_value()) return;
        记录I64到任务(stepInfo, 键, *value);
        if (headInfo) 记录I64到任务(headInfo, 键, *value);
    };

    auto 写双层指针 = [&](const std::string& 键, const 词性节点类* 输出特征类型) {
        if (!输出特征类型) return;
        auto value = 世界树.读取特征快照(输出场景, 输出特征类型, "自我线程类::私有_同步方法事实到任务方向特征/读输出指针/" + 键);
        if (!value.has_value() || !std::holds_alternative<指针句柄>(*value)) return;
        const auto ptr = std::get<指针句柄>(*value).指针;
        记录指针到任务(stepInfo, 键, ptr);
        if (headInfo) 记录指针到任务(headInfo, 键, ptr);
    };

    const auto* 回执成功 = 语素集.添加词性词("回执_成功", "名词");
    const auto* 回执错误码 = 语素集.添加词性词("回执_错误码", "名词");
    const auto* 人类输入存在 = 语素集.添加词性词("人类输入_存在", "名词");
    const auto* 人类输入序号 = 语素集.添加词性词("人类输入_序号", "名词");
    const auto* 人类输入对象指针 = 语素集.添加词性词("人类输入_对象指针", "名词");
    const auto* 人类输入来源码 = 语素集.添加词性词("人类输入_来源码", "名词");
    const auto* 人类输入已读取 = 语素集.添加词性词("人类输入_已读取", "名词");
    const auto* 服务动作已确认收到 = 语素集.添加词性词("服务动作_已确认收到", "名词");
    const auto* 服务动作已请求补充信息 = 语素集.添加词性词("服务动作_已请求补充信息", "名词");
    const auto* 服务动作输出有效 = 语素集.添加词性词("服务动作_输出有效", "名词");
    const auto* 服务动作关联输入序号 = 语素集.添加词性词("服务动作_关联输入序号", "名词");
    const auto* 服务动作关联输入指针 = 语素集.添加词性词("服务动作_关联输入指针", "名词");
    const auto* 服务动作补充原因码 = 语素集.添加词性词("服务动作_补充原因码", "名词");
    const auto* 试探动作已停止 = 语素集.添加词性词("试探动作_已停止", "名词");
    const auto* 保守态请求 = 语素集.添加词性词("保守态_请求", "名词");
    const auto* 试探停止原因码 = 语素集.添加词性词("试探停止原因码", "名词");
    const auto* 风险级别 = 语素集.添加词性词("风险级别", "名词");
    const auto* 待机状态 = 语素集.添加词性词("待机状态", "名词");
    const auto* 时序正向步长 = 语素集.添加词性词("时序正向步长", "名词");
    const auto* 时序反向步长 = 语素集.添加词性词("时序反向步长", "名词");
    const auto* 服务时序衰减步长 = 语素集.添加词性词("服务时序衰减步长", "名词");

    记录I64到任务(stepInfo, "最近方法本能ID", static_cast<I64>(本能ID));
    if (headInfo) 记录I64到任务(headInfo, "最近方法本能ID", static_cast<I64>(本能ID));
    写双层I64("回执_成功", 回执成功);
    写双层I64("回执_错误码", 回执错误码);

    switch (本能ID) {
    case 枚举_本能动作ID::自我_读取核心状态:
        写双层I64("安全值", 特征类型定义类::类型_自我_安全值);
        写双层I64("服务值", 特征类型定义类::类型_自我_服务值);
        写双层I64("待机状态", 待机状态);
        写双层I64("时序正向步长", 时序正向步长);
        写双层I64("时序反向步长", 时序反向步长);
        写双层I64("服务时序衰减步长", 服务时序衰减步长);
        break;
    case 枚举_本能动作ID::自我_读取最新人类输入:
        写双层I64("人类输入_存在", 人类输入存在);
        写双层I64("人类输入_序号", 人类输入序号);
        写双层I64("人类输入_来源码", 人类输入来源码);
        写双层I64("人类输入_已读取", 人类输入已读取);
        写双层指针("人类输入_对象指针", 人类输入对象指针);
        break;
    case 枚举_本能动作ID::自我_确认收到:
        写双层I64("服务动作_已确认收到", 服务动作已确认收到);
        写双层I64("服务动作_输出有效", 服务动作输出有效);
        写双层I64("服务动作_关联输入序号", 服务动作关联输入序号);
        写双层I64("人类输入_来源码", 人类输入来源码);
        写双层指针("服务动作_关联输入指针", 服务动作关联输入指针);
        break;
    case 枚举_本能动作ID::自我_请求补充信息:
        写双层I64("服务动作_已请求补充信息", 服务动作已请求补充信息);
        写双层I64("服务动作_输出有效", 服务动作输出有效);
        写双层I64("服务动作_补充原因码", 服务动作补充原因码);
        写双层I64("服务动作_关联输入序号", 服务动作关联输入序号);
        写双层I64("人类输入_来源码", 人类输入来源码);
        写双层指针("服务动作_关联输入指针", 服务动作关联输入指针);
        break;
    case 枚举_本能动作ID::自我_停止当前试探并进入保守态:
        写双层I64("试探动作_已停止", 试探动作已停止);
        写双层I64("保守态_请求", 保守态请求);
        写双层I64("试探停止原因码", 试探停止原因码);
        写双层I64("风险级别", 风险级别);
        break;

    default:
        break;
    }
    auto 读任务方向I64 = [&](任务信息基类* info, const std::string& 键) -> I64 {
        if (!info || !info->任务虚拟存在) return 0;
        return 世界树.读取特征_I64(
            info->任务虚拟存在,
            自我线程模块_detail::特征_任务方向(键),
            "自我线程类::私有_同步方法事实到任务方向特征/读任务方向/" + 键).value_or(0);
    };

    auto 读需求目标方向 = [&](需求节点类* 需求节点, const std::string& 键) -> I64 {
        if (!需求节点) return 0;
        const auto 判定 = 需求集.判断需求状态(需求节点, "自我线程类::私有_同步方法事实到任务方向特征/读需求目标/" + 键);
        if (!判定.有目标 || !std::holds_alternative<I64>(判定.目标值)) return 0;
        return std::get<I64>(判定.目标值);
    };

    auto 计算安全投影方向 = [&](任务信息基类* info) -> I64 {
        I64 score = 0;
        score += 读任务方向I64(info, "安全值") * 3;
        score += 读任务方向I64(info, "待机状态") * -3;
        score += 读任务方向I64(info, "时序正向步长") * 1;
        score += 读任务方向I64(info, "时序反向步长") * -1;
        score += 读任务方向I64(info, "服务时序衰减步长") * -1;
        score += 读任务方向I64(info, "试探动作_已停止") * 1;
        score += 读任务方向I64(info, "保守态_请求") * 1;
        score += 读任务方向I64(info, "风险级别") * -2;
        if (score > 0) return 1;
        if (score < 0) return -1;
        return 0;
    };

    auto 计算服务投影方向 = [&](任务信息基类* info) -> I64 {
        I64 score = 0;
        score += 读任务方向I64(info, "服务值") * 3;
        score += 读任务方向I64(info, "服务动作_输出有效") * 2;
        score += 读任务方向I64(info, "服务动作_已确认收到") * 2;
        score += 读任务方向I64(info, "服务动作_已请求补充信息") * 1;
        score += 读任务方向I64(info, "人类输入_存在") * 1;
        if (score > 0) return 1;
        if (score < 0) return -1;
        return 0;
    };

    auto 计算对齐值 = [](I64 投影方向, I64 需求方向) -> I64 {
        if (投影方向 == 0 || 需求方向 == 0) return 0;
        return (投影方向 == 需求方向) ? 1 : -1;
    };

    auto 写任务投影与对齐 = [&](任务信息基类* info, const std::string& 前缀) {
        if (!info) return;
        const I64 安全投影 = 计算安全投影方向(info);
        const I64 服务投影 = 计算服务投影方向(info);
        I64 安全服务投影 = 0;
        if (安全投影 != 0 && 服务投影 != 0) 安全服务投影 = (安全投影 == 服务投影) ? 1 : -1;

        const I64 安全目标方向 = 读需求目标方向(安全顺向需求节点_, 前缀 + "/安全");
        const I64 服务目标方向 = 读需求目标方向(服务顺向需求节点_, 前缀 + "/服务");
        const I64 安全服务目标方向 = 读需求目标方向(安全服务同向需求节点_, 前缀 + "/安全服务");

        记录I64到任务(info, "投影方向_安全", 安全投影);
        记录I64到任务(info, "投影方向_服务", 服务投影);
        记录I64到任务(info, "投影方向_安全服务", 安全服务投影);
        记录I64到任务(info, "需求对齐_安全", 计算对齐值(安全投影, 安全目标方向));
        记录I64到任务(info, "需求对齐_服务", 计算对齐值(服务投影, 服务目标方向));
        记录I64到任务(info, "需求对齐_安全服务", 计算对齐值(安全服务投影, 安全服务目标方向));
    };

    写任务投影与对齐(stepInfo, "步骤");
    if (headInfo) 写任务投影与对齐(headInfo, "任务头");

}

inline 任务执行器::结构_叶子执行结果 自我线程类::私有_执行任务叶子步骤(任务节点类* 步骤节点) {
    任务执行器::结构_叶子执行结果 out{};
    auto* stepInfo = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;
    if (!stepInfo) {
        out.摘要 = "步骤节点无效";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::转入尝试学习;
        return out;
    }

    auto* 方法首节点 = stepInfo->当前选中方法首节点;
    if (!方法首节点 && !stepInfo->可用方法首节点列表.empty()) {
        方法首节点 = stepInfo->可用方法首节点列表.front();
        stepInfo->当前选中方法首节点 = 方法首节点;
    }
    if (!方法首节点) {
        out.摘要 = "步骤缺少可执行方法";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::转入尝试学习;
        return out;
    }

    auto* hmi = dynamic_cast<方法首节点主信息类*>(方法首节点->主信息);
    if (!hmi || hmi->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID || hmi->动作句柄.本能ID_U64 == 0) {
        out.摘要 = "当前仅支持本能方法执行";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::转入尝试学习;
        return out;
    }

    const auto 本能ID = static_cast<枚举_本能动作ID>(hmi->动作句柄.本能ID_U64);
    auto* 执行场景根 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    const 时间戳 now = 结构体_时间戳::当前_微秒();
    auto* 输入场景 = stepInfo->场景 ? stepInfo->场景 : 执行场景根;
    if (!执行场景根) {
        out.摘要 = "缺少执行场景根";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::挂起等待;
        return out;
    }

    auto* 输出场景信息 = new 场景节点主信息类();
    输出场景信息->名称 = 语素集.添加词性词(std::string("步骤执行输出_") + std::to_string(now), "名词");
    输出场景信息->类型 = 语素集.添加词性词("任务步骤执行场景", "名词");
    输出场景信息->最后观测时间 = now;
    auto* 输出场景 = 世界树.创建场景(执行场景根, 输出场景信息, "自我线程类::私有_执行任务叶子步骤/创建输出场景");
    if (!输出场景) {
        delete 输出场景信息;
        out.摘要 = "无法创建方法输出场景";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::挂起等待;
        return out;
    }

    方法集.标记方法调用开始(方法首节点, 输入场景, now, "自我线程类::私有_执行任务叶子步骤/开始");
    const bool 成功 = 本能集.调用(本能ID, 输入场景, 输出场景);
    方法集.标记方法调用结束(方法首节点, 输出场景, 成功, 成功 ? 0 : -1, now, "自我线程类::私有_执行任务叶子步骤/结束");
    私有_同步方法事实到任务方向特征(步骤节点, 本能ID, 输出场景, now);

    out.成功 = 成功;
    out.结果场景 = 输出场景;
    out.摘要 = 本能集.查询函数名称(本能ID);
    if (out.摘要.empty()) out.摘要 = std::string("本能_") + std::to_string(hmi->动作句柄.本能ID_U64);
    out.建议动作 = 成功 ? 枚举_任务反馈动作::继续下一步 : 枚举_任务反馈动作::转入尝试学习;
    if (!成功) out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
    return out;
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
        私有_刷新基础方向概念与需求("自我线程类::处理消息_任务/紧急中断");
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

inline void 自我线程类::记录根方向摘要(const std::string& 调用点) {
    日志::运行f(
        "[自我线程][不变量] 根方向摘要: 调用点={}, 目标安全值={}, 当前安全值={}, 差值={}, 目标服务值={}, 当前服务值={}, 待机={}",
        调用点,
        自我核心_.根目标安全值(),
        自我核心_.安全值(),
        自我核心_.安全根方向差值(),
        自我核心_.根目标服务值(),
        自我核心_.服务值(),
        自我核心_.是否待机状态() ? 1 : 0);
}

inline void 自我线程类::校验根约束不变量(const std::string& 调用点) {
    const U64 安全值 = 自我核心_.安全值();
    const U64 服务值 = 自我核心_.服务值();

    if (自我核心_.是否已消亡() && 安全值 != 0) {
        日志::运行f(
            "[自我线程][不变量][违反] INV-004: 已消亡状态必须对应安全值归零: 调用点={}, 安全值={}, 服务值={}",
            调用点,
            安全值,
            服务值);
    }

    if (服务值 == 0 && 安全值 == 0) {
        日志::运行f(
            "[自我线程][不变量][违反] INV-004/INV-005: 服务值为0时安全值不应直接归零: 调用点={}, 安全值={}, 服务值={}",
            调用点,
            安全值,
            服务值);
    }
}

inline void 自我线程类::记录需求树不变量摘要(const std::string& 调用点) {
    const auto 校验 = 需求集.校验自我安全服务根需求拓扑(
        安全需求节点_,
        服务需求节点_,
        调用点 + "/根需求拓扑校验");

    if (!校验.拓扑合法()) {
        日志::运行(
            "[自我线程][不变量][违反] INV-007/INV-008: 顶层需求拓扑不合法: 调用点=" + 调用点 +
            ", 顶层需求数=" + std::to_string(校验.顶层需求数) +
            ", 非根顶层需求数=" + std::to_string(校验.非根顶层需求数) +
            ", 安全根存在=" + std::to_string(校验.安全根存在 ? 1 : 0) +
            ", 服务根存在=" + std::to_string(校验.服务根存在 ? 1 : 0));
        return;
    }

    日志::运行(
        "[自我线程][不变量] 需求树摘要: 调用点=" + 调用点 +
        ", 顶层需求数=" + std::to_string(校验.顶层需求数) +
        ", 安全根存在=" + std::to_string(校验.安全根存在 ? 1 : 0) +
        ", 服务根存在=" + std::to_string(校验.服务根存在 ? 1 : 0));
}

inline void 自我线程类::私有_刷新基础方向概念与需求(const std::string& 调用点) {
    auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    auto* 自我存在 = 世界树.自我指针 ? 世界树.自我指针 : 自我指针;
    if (!内部世界 || !自我存在) return;

    特征类型定义类::初始化特征类型定义模块_依赖语素();
    if (!特征类型定义类::类型_自我_安全值 || !特征类型定义类::类型_自我_服务值) return;

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    auto* 安全特征 = 世界树.确保特征(自我存在, 特征类型定义类::类型_自我_安全值, 特征类型定义类::类型_自我_安全值, 调用点 + "/安全特征");
    auto* 服务特征 = 世界树.确保特征(自我存在, 特征类型定义类::类型_自我_服务值, 特征类型定义类::类型_自我_服务值, 调用点 + "/服务特征");
    if (!安全特征 || !服务特征) return;

    const auto 安全方向结果 = 状态集.比较最近两次I64状态方向(
        内部世界,
        枚举_状态域::内部状态,
        自我存在,
        安全特征);
    const auto 服务方向结果 = 状态集.比较最近两次I64状态方向(
        内部世界,
        枚举_状态域::内部状态,
        自我存在,
        服务特征);

    const I64 安全方向 = 安全方向结果.可比较 ? 安全方向结果.方向 : 0;
    const I64 服务方向 = 服务方向结果.可比较 ? 服务方向结果.方向 : 0;
    I64 安全服务方向关系 = 0;
    if (安全方向 != 0 && 服务方向 != 0) {
        安全服务方向关系 = (安全方向 == 服务方向) ? 1 : -1;
    }

    (void)自我核心_.记录基础方向概念("安全方向", 安全方向, now, 调用点 + "/记录概念");
    (void)自我核心_.记录基础方向概念("服务方向", 服务方向, now, 调用点 + "/记录概念");
    (void)自我核心_.记录基础方向概念("安全服务方向关系", 安全服务方向关系, now, 调用点 + "/记录概念");

    auto 确保方向需求 = [&](需求节点类*& 缓存节点,
                         需求节点类* 父需求,
                         const std::string& 需求类型名,
                         const std::string& 概念键,
                         I64 目标方向值) {
        if (缓存节点 || !父需求) return;

        const auto* 需求类型 = 语素集.添加词性词(需求类型名, "名词");
        const auto* 概念类型 = 语素集.添加词性词("基础概念_" + 概念键, "名词");
        if (!需求类型 || !概念类型) return;

        auto* 目标特征 = 世界树.确保特征(自我存在, 概念类型, 概念类型, 调用点 + "/确保方向需求特征/" + 需求类型名);
        if (!目标特征) return;

        auto* 目标状态 = 状态集.创建内部状态(
            内部世界,
            自我存在,
            目标特征,
            特征快照值{ 目标方向值 },
            枚举_存在状态事件::创建,
            true,
            now);
        if (!目标状态) return;

        需求类::结构_创建参数 参数{};
        参数.类型 = 需求类型;
        参数.发生场景 = 内部世界;
        参数.生成时间 = now;
        参数.权重 = 1;

        缓存节点 = 需求集.创建需求(
            自我存在,
            自我存在,
            目标状态,
            参数,
            调用点 + "/创建方向需求/" + 需求类型名,
            父需求);

        if (缓存节点) {
            日志::运行(
                "[自我线程] 已生成基础方向需求: 类型=" + 需求类型名 +
                ", 父需求=" + std::to_string(reinterpret_cast<std::uintptr_t>(父需求)) +
                ", 节点=" + std::to_string(reinterpret_cast<std::uintptr_t>(缓存节点)));
        }
    };

    if (安全方向结果.可比较) {
        确保方向需求(安全顺向需求节点_, 安全需求节点_, "安全顺向需求", "安全方向", 1);
    }
    if (服务方向结果.可比较) {
        确保方向需求(服务顺向需求节点_, 服务需求节点_, "服务顺向需求", "服务方向", 1);
    }
    if (安全方向结果.可比较 && 服务方向结果.可比较) {
        确保方向需求(安全服务同向需求节点_, 服务需求节点_, "安全服务同向需求", "安全服务方向关系", 1);
    }

    if (安全方向结果.可比较 || 服务方向结果.可比较) {
        日志::运行f(
            "[自我线程] 基础方向概念已刷新: 安全方向={}, 服务方向={}, 安全服务方向关系={}, 安全顺向需求={}, 服务顺向需求={}, 安全服务同向需求={}",
            安全方向,
            服务方向,
            安全服务方向关系,
            安全顺向需求节点_ ? 1 : 0,
            服务顺向需求节点_ ? 1 : 0,
            安全服务同向需求节点_ ? 1 : 0);
    }
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

    const auto* 待机状态特征 = 语素集.添加词性词("待机状态", "名词");
    const auto* 时序正向步长特征 = 语素集.添加词性词("时序正向步长", "名词");
    const auto* 时序反向步长特征 = 语素集.添加词性词("时序反向步长", "名词");
    const auto* 服务时序衰减步长特征 = 语素集.添加词性词("服务时序衰减步长", "名词");

    if (待机状态特征) {
        (void)世界树.写入特征_I64(
            自我指针,
            待机状态特征,
            自我核心_.是否待机状态() ? 1 : 0,
            {},
            调用点 + "/写待机状态");
    }
    if (时序正向步长特征) {
        (void)世界树.写入特征_I64(
            自我指针,
            时序正向步长特征,
            转I64饱和(自我核心_.时序正向步长()),
            {},
            调用点 + "/写时序正向步长");
    }
    if (时序反向步长特征) {
        (void)世界树.写入特征_I64(
            自我指针,
            时序反向步长特征,
            转I64饱和(自我核心_.时序反向步长()),
            {},
            调用点 + "/写时序反向步长");
    }
    if (服务时序衰减步长特征) {
        (void)世界树.写入特征_I64(
            自我指针,
            服务时序衰减步长特征,
            转I64饱和(自我核心_.服务时序衰减步长()),
            {},
            调用点 + "/写服务时序衰减步长");
    }
}

inline void 自我线程类::更新安全值_基于任务(bool 完成, bool 成功) {
    if (!完成) return;

    const U64 安全前 = 自我核心_.安全值();
    const U64 服务前 = 自我核心_.服务值();

    自我核心_.应用任务结果_默认策略(成功);
    同步自我核心特征到世界树("自我线程类::更新安全值_基于任务");
    私有_刷新基础方向概念与需求("自我线程类::更新安全值_基于任务");
    记录根方向摘要("自我线程类::更新安全值_基于任务");
    校验根约束不变量("自我线程类::更新安全值_基于任务");
    私有_刷新根任务就绪状态();

    日志::运行f("[自我线程] 任务结果更新安全服务值: 成功={}, 安全值 {} -> {}, 服务值 {} -> {}",
        成功 ? 1 : 0,
        安全前,
        自我核心_.安全值(),
        服务前,
        自我核心_.服务值());
}

inline void 自我线程类::周期性维护() {
    const U64 安全前 = 自我核心_.安全值();
    const U64 服务前 = 自我核心_.服务值();
    const U64 服务衰减步长前 = 自我核心_.服务时序衰减步长();
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

    自我核心_.按时序规则更新安全服务值(
        待完成任务数,
        待学习任务数,
        now,
        "自我线程类::周期性维护");

    同步自我核心特征到世界树("自我线程类::周期性维护");
    私有_刷新基础方向概念与需求("自我线程类::周期性维护");
    校验根约束不变量("自我线程类::周期性维护");
    私有_刷新根任务就绪状态();

    const U64 安全后 = 自我核心_.安全值();
    const U64 服务后 = 自我核心_.服务值();
    const U64 服务衰减步长后 = 自我核心_.服务时序衰减步长();
    const U64 正向步长后 = 自我核心_.时序正向步长();
    const U64 反向步长后 = 自我核心_.时序反向步长();

    if (安全前 != 安全后 || 服务前 != 服务后 || 服务衰减步长前 != 服务衰减步长后 || 正向步长前 != 正向步长后 || 反向步长前 != 反向步长后) {
        日志::运行f(
            "[自我线程] 周期维护数值: 安全值 {} -> {}, 服务值 {} -> {}, 服务衰减步长 {} -> {}, 正向步长 {} -> {}, 反向步长 {} -> {}, 待完成={}, 待学习={}, 待机={}",
            安全前,
            安全后,
            服务前,
            服务后,
            服务衰减步长前,
            服务衰减步长后,
            正向步长前,
            正向步长后,
            反向步长前,
            反向步长后,
            待完成任务数,
            待学习任务数,
            自我核心_.是否待机状态() ? 1 : 0);
        记录根方向摘要("自我线程类::周期性维护");
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
    const 词性节点类* 安全任务名 = 语素集.添加词性词("安全任务", "名词");
    const 词性节点类* 服务任务名 = 语素集.添加词性词("服务任务", "名词");
    const 词性节点类* 根任务类型 = 语素集.添加词性词("根需求任务", "名词");
    const std::int64_t 目标上限 = (std::numeric_limits<std::int64_t>::max)();
    auto* 需求父场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;

    目标安全值I64_ = 目标上限;
    目标服务值I64_ = 目标上限;

    const auto 根需求结果 = 需求集.确保自我安全服务根需求(
        自我指针,
        需求父场景,
        特征类型定义类::类型_自我_安全值,
        特征类型定义类::类型_自我_服务值,
        目标安全值I64_,
        目标服务值I64_,
        now,
        "自我线程类::初始化_初始需求与任务/根需求");

    安全需求节点_ = 根需求结果.安全需求;
    服务需求节点_ = 根需求结果.服务需求;

    if (!根需求结果.完整可用()) {
        日志::运行(
            "[自我线程][不变量][违反] INV-007/INV-008: 自我根需求未能完整建立: 安全根=" + std::to_string(安全需求节点_ ? 1 : 0) +
            ", 服务根=" + std::to_string(服务需求节点_ ? 1 : 0));
    }
    else {
        日志::运行(
            "[自我线程] 自我根需求已就绪: 新建安全根=" + std::to_string(根需求结果.新建安全需求 ? 1 : 0) +
            ", 新建服务根=" + std::to_string(根需求结果.新建服务需求 ? 1 : 0) +
            ", 安全目标值=" + std::to_string(目标安全值I64_) +
            ", 服务目标值=" + std::to_string(目标服务值I64_));
    }

    任务类::结构_创建任务头参数 安全根任务参数{};
    安全根任务参数.名称 = 安全任务名;
    安全根任务参数.类型 = 根任务类型;
    安全根任务参数.初始场景 = 需求父场景;
    安全根任务参数.创建时间 = now;
    安全根任务参数.基准优先级 = 目标安全值I64_;
    安全根任务参数.初始状态 = 枚举_任务状态::就绪;
    安全根任务参数.任务树类型 = 枚举_任务树类型::编排任务;
    安全根任务参数.自动关联到需求 = true;

    任务类::结构_创建任务头参数 服务根任务参数 = 安全根任务参数;
    服务根任务参数.名称 = 服务任务名;
    服务根任务参数.基准优先级 = 目标服务值I64_;

    安全任务节点_ = 安全需求节点_
        ? 任务类::确保需求头结点(安全需求节点_, 安全根任务参数, nullptr, "自我线程类::初始化_初始需求与任务/安全根任务")
        : nullptr;
    服务任务节点_ = 服务需求节点_
        ? 任务类::确保需求头结点(服务需求节点_, 服务根任务参数, nullptr, "自我线程类::初始化_初始需求与任务/服务根任务")
        : nullptr;

    if ((安全需求节点_ && !安全任务节点_) || (服务需求节点_ && !服务任务节点_)) {
        日志::运行(
            "[自我线程][不变量][违反] 根任务未能完整建立: 安全任务=" + std::to_string(安全任务节点_ ? 1 : 0) +
            ", 服务任务=" + std::to_string(服务任务节点_ ? 1 : 0));
    }
    else if (安全任务节点_ || 服务任务节点_) {
        日志::运行(
            "[自我线程] 根任务已就绪: 安全任务=" + std::to_string(安全任务节点_ ? 1 : 0) +
            ", 服务任务=" + std::to_string(服务任务节点_ ? 1 : 0));
    }

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

    if (!学习任务节点_ || !学习任务节点_->主信息) {
        初始需求任务已就绪_ = false;
        return;
    }

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
    根任务筹办器_.设置尝试学习任务(学习任务节点_);
    根任务筹办器_.设置叶子执行回调([this](任务节点类* 步骤节点) {
        return 私有_执行任务叶子步骤(步骤节点);
    });

    私有_刷新根任务就绪状态();
    初始需求任务已就绪_ = (安全需求节点_ != nullptr && 服务需求节点_ != nullptr && 安全任务节点_ != nullptr && 服务任务节点_ != nullptr && 学习任务节点_ != nullptr);

    日志::运行f("[自我线程] 已生成初始需求/任务: 学习任务={}, 安全任务={}, 服务任务={}, 根需求任务就绪={}",
        (void*)学习任务节点_,
        (void*)安全任务节点_,
        (void*)服务任务节点_,
        初始需求任务已就绪_ ? 1 : 0);
}

inline void 自我线程类::私有_推动根任务进入筹办(任务节点类* 任务节点, const std::string& 标签) {
    auto* 头信息 = (任务节点 && 任务节点->主信息) ? dynamic_cast<任务头结点信息*>(任务节点->主信息) : nullptr;
    if (!头信息 || !头信息->需求 || !头信息->需求->主信息) return;

    bool 已有步骤 = 头信息->当前步骤节点 != nullptr;
    std::int32_t 已有步骤数 = 0;
    if (任务节点->子) {
        auto* first = static_cast<任务节点类*>(任务节点->子);
        auto* cur = first;
        do {
            if (cur && cur->主信息 && dynamic_cast<任务步骤节点信息*>(cur->主信息)) {
                ++已有步骤数;
                已有步骤 = true;
            }
            cur = cur ? static_cast<任务节点类*>(cur->下) : nullptr;
        } while (cur && cur != first);
    }
    if (已有步骤) return;

    auto* 筹办场景 = 头信息->场景 ? 头信息->场景 : (世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界);
    if (!任务类::进入筹办阶段(任务节点, 筹办场景, "自我线程类::私有_推动根任务进入筹办/" + 标签)) {
        日志::运行(
            "[自我线程][不变量][违反] 根任务无法进入筹办阶段: 标签=" + 标签 +
            ", 任务=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务节点)));
        return;
    }

    auto* step = 根任务筹办器_.仅筹办一步(任务节点);
    if (step) {
        if (任务节点->主信息 && 任务节点->主信息->状态 == 枚举_任务状态::筹办中) {
            任务类::设置任务状态(任务节点, 枚举_任务状态::就绪, "自我线程类::私有_推动根任务进入筹办/筹办完成待执行/" + 标签);
        }
        日志::运行(
            "[自我线程] 根任务已进入筹办并生成首批步骤: 标签=" + 标签 +
            ", 任务=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务节点)) +
            ", 步骤=" + std::to_string(reinterpret_cast<std::uintptr_t>(step)));
        return;
    }

    auto 最小试探方法列表 = 私有_生成根任务最小试探方法候选(标签, 结构体_时间戳::当前_微秒());
    if (!最小试探方法列表.empty()) {
        任务类::结构_创建步骤参数 试探步骤参数{};
        试探步骤参数.名称 = 语素集.添加词性词(标签 + "_最小试探步骤", "名词");
        试探步骤参数.类型 = 语素集.添加词性词("根任务最小试探步骤", "名词");
        试探步骤参数.所属任务头结点 = 任务节点;
        试探步骤参数.步骤场景 = 筹办场景;
        试探步骤参数.步骤序号 = 已有步骤数;
        试探步骤参数.初始状态 = 枚举_任务状态::就绪;
        试探步骤参数.允许切换方法 = true;
        试探步骤参数.可用方法首节点列表 = 最小试探方法列表;
        试探步骤参数.当前选中方法首节点 = 最小试探方法列表.front();

        auto* 试探步骤 = 任务类::创建步骤节点(任务节点, 试探步骤参数, "自我线程类::私有_推动根任务进入筹办/创建最小试探步骤");
        if (试探步骤) {
            (void)任务类::设置任务当前步骤(任务节点->主信息, 试探步骤, 结构体_时间戳::当前_微秒(), "自我线程类::私有_推动根任务进入筹办/设置最小试探步骤");
            任务类::设置任务状态(任务节点, 枚举_任务状态::就绪, "自我线程类::私有_推动根任务进入筹办/最小试探待执行/" + 标签);
            日志::运行f(
                "[自我线程] 根任务进入筹办后生成最小试探步骤: 标签={}, 任务={}, 步骤={}, 方法数={}",
                标签,
                (void*)任务节点,
                (void*)试探步骤,
                最小试探方法列表.size());
            return;
        }
    }

    任务类::结构_创建步骤参数 占位步骤参数{};
    占位步骤参数.名称 = 语素集.添加词性词(标签 + "_首批步骤", "名词");
    占位步骤参数.类型 = 语素集.添加词性词("根任务首批步骤", "名词");
    占位步骤参数.所属任务头结点 = 任务节点;
    占位步骤参数.步骤场景 = 筹办场景;
    占位步骤参数.步骤序号 = 已有步骤数;
    占位步骤参数.初始状态 = 枚举_任务状态::等待中;
    占位步骤参数.允许切换方法 = true;

    auto* 占位步骤 = 任务类::创建步骤节点(任务节点, 占位步骤参数, "自我线程类::私有_推动根任务进入筹办/创建占位步骤");
    if (!占位步骤) {
        日志::运行(
            "[自我线程][不变量][违反] 根任务筹办未生成步骤且占位步骤创建失败: 标签=" + 标签 +
            ", 任务=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务节点)));
        return;
    }

    (void)任务类::设置任务当前步骤(任务节点->主信息, 占位步骤, 结构体_时间戳::当前_微秒(), "自我线程类::私有_推动根任务进入筹办/设置占位步骤");
    任务类::设置任务状态(任务节点, 枚举_任务状态::等待中, "自我线程类::私有_推动根任务进入筹办/等待进一步分解/" + 标签);
    日志::运行(
        "[自我线程] 根任务进入筹办但暂无可执行方法，已创建占位步骤: 标签=" + 标签 +
        ", 任务=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务节点)) +
        ", 步骤=" + std::to_string(reinterpret_cast<std::uintptr_t>(占位步骤)));
}

inline void 自我线程类::私有_刷新根任务就绪状态() {
    auto 刷新一个根任务 = [&](任务节点类* 任务节点, 需求节点类* 需求节点, const std::string& 标签) {
        if (!任务节点 || !任务节点->主信息 || !需求节点) return;

        const auto 判定 = 需求集.判断需求状态(需求节点, "自我线程类::私有_刷新根任务就绪状态/" + 标签);
        const auto 新状态 = 判定.已满足 ? 枚举_任务状态::等待中 : 枚举_任务状态::就绪;
        const bool 状态已变化 = (任务节点->主信息->状态 != 新状态);
        if (状态已变化) {
            任务类::设置任务状态(任务节点, 新状态, "自我线程类::私有_刷新根任务就绪状态/" + 标签);
            日志::运行(
                "[自我线程] 根任务状态刷新: 标签=" + 标签 +
                ", 新状态=" + std::to_string(static_cast<int>(新状态)) +
                ", 已满足=" + std::to_string(判定.已满足 ? 1 : 0));
        }

        if (新状态 == 枚举_任务状态::就绪) {
            私有_推动根任务进入筹办(任务节点, 标签);
        }
    };

    刷新一个根任务(安全任务节点_, 安全需求节点_, "安全");
    刷新一个根任务(服务任务节点_, 服务需求节点_, "服务");
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
    私有_尝试入队动作任务(安全任务节点_, "安全");
    私有_尝试入队动作任务(服务任务节点_, "服务");

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
    while (!stop_.load()) {
        执行队列项 item{};
        bool has = false;

        {
            std::unique_lock<std::mutex> lk(执行队列锁_);
            执行队列cv_.wait(lk, [&] { return stop_.load() || !执行队列_.empty(); });
            if (stop_.load()) break;
            if (!执行队列_.empty()) {
                item = 执行队列_.top();
                执行队列_.pop();
                has = true;
            }
        }

        if (!has || !item.节点 || !item.节点->主信息) continue;

        auto* info = item.节点->主信息;
        任务类::设置任务状态(item.节点, 枚举_任务状态::执行中, "自我线程类::动作线程入口/置执行中");
        ++正在执行线程数_;

        const bool 有进展 = 根任务筹办器_.推进一步(item.节点);
        if (!有进展 && item.节点->主信息 && item.节点->主信息->状态 == 枚举_任务状态::执行中) {
            任务类::设置任务状态(item.节点, 枚举_任务状态::就绪, "自我线程类::动作线程入口/推进失败回到就绪");
        }

        私有_刷新根任务就绪状态();
        日志::运行f("[自我线程] 动作线程推进任务一次: 节点={}, 有进展={}, 当前状态={}",
            (void*)item.节点,
            有进展,
            item.节点->主信息 ? static_cast<int>(item.节点->主信息->状态) : 0);

        if (正在执行线程数_.load() > 0) --正在执行线程数_;
    }
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






























