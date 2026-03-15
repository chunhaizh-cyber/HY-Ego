
module;

export module 自我线程模块;

import 基础数据类型模块;
import 主信息定义模块;
import 世界树环境模块;
import 语素环境模块;
import 状态模块;
import 通用函数模块;

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
import 场景模块;
import 概念引擎模块;
import 外设本能方法_相机模块;
import 自我本能方法_最小试探模块;
import 自我本能方法_相机认知模块;


import <thread>;
import <atomic>;
import <mutex>;
import <condition_variable>;
import <functional>;
import <chrono>;
import <exception>;
import <memory>;
import 日志模块;

import<cassert>;
import <vector>;
import <string>;
import <cstdint>;  // for int64_t
import <deque>;
import <queue>;
import <map>;
import <algorithm>;
import <unordered_set>;
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
    enum class 枚举_自我运行状态 {
        未启动 = 0,
        初始化中 = 1,
        运行中 = 2,
        退出中 = 3,
        已停止 = 4,
        故障 = 5
    };

    struct 结构_学习推进结果 {
        bool 有进展 = false;
        bool 需要唤醒等待任务 = false;
        方法节点类* 目标方法首节点 = nullptr;
    };

    自我线程类() = default;
    ~自我线程类() { 停止(); }
    void 启动();
    void 停止();
    void 等待线程结束();
    bool 是否正在运行() const;
    bool 是否初始化完成() const;
    bool 是否健康运行() const;
    std::string 最近故障摘要() const;
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
        调度 = 9,           // 内部调度：尝试执行一个“就绪任务”的下一步
        新存在创建 = 11     // payload: 存在节点指针
    };


    struct 任务消息 {
        消息类型 类型{};
        std::string 任务ID;     // 可空
        任务节点类* 任务节点指针 = nullptr; // 可空：优先用于内部（避免用字符串反查）
        存在节点类* 存在节点指针 = nullptr; // 可空：用于新存在创建通知
        std::string 需求键;     // 可空
        double 数值 = 0.0;       // 复用：进度/观测强度/新权重
        std::uint64_t 序号 = 0;  // 复用：异步回执序号（0=不区分序号）
        bool 标志 = false;       // 复用：成功/失败等
        std::string 文本;        // 可空：摘要/解释/外部句柄
    };
private:
    struct 新存在创建通知桥 {
        std::mutex 锁_{};
        std::condition_variable 条件_{};
        std::function<void(存在节点类*)> 投递器_{};
        bool 已注销_ = false;
        std::size_t 在途通知数_ = 0;

        struct 结构_在途通知守卫 {
            新存在创建通知桥* 桥 = nullptr;
            ~结构_在途通知守卫() {
                if (!桥) return;
                {
                    std::lock_guard<std::mutex> lk(桥->锁_);
                    if (桥->在途通知数_ > 0) {
                        --桥->在途通知数_;
                    }
                }
                桥->条件_.notify_all();
            }
        };

        void 注册(std::function<void(存在节点类*)> 投递器) {
            std::lock_guard<std::mutex> lk(锁_);
            投递器_ = std::move(投递器);
            已注销_ = false;
        }

        void 注销并等待() {
            std::unique_lock<std::mutex> lk(锁_);
            已注销_ = true;
            投递器_ = {};
            条件_.wait(lk, [this] { return 在途通知数_ == 0; });
        }

        void 通知(存在节点类* 节点) {
            if (!节点) return;

            std::function<void(存在节点类*)> 投递器;
            {
                std::lock_guard<std::mutex> lk(锁_);
                if (已注销_ || !投递器_) return;
                ++在途通知数_;
                投递器 = 投递器_;
            }

            结构_在途通知守卫 守卫{ this };
            投递器(节点);
        }
    };

    // ==== 内部状态 ====
    std::atomic<bool> stop_{ false };
    std::thread self_thread_{};
    std::atomic<枚举_自我运行状态> 运行状态_{ 枚举_自我运行状态::未启动 };
    std::atomic<bool> 初始化完成_{ false };
    std::atomic<bool> 主线程存活_{ false };
    mutable std::mutex 故障锁_{};
    std::string 最近故障摘要_{};
    概念引擎类 概念引擎_{};
    std::shared_ptr<新存在创建通知桥> 新存在创建通知桥_{ std::make_shared<新存在创建通知桥>() };

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
    场景节点类* 外设观察桥输入场景_ = nullptr;
    场景节点类* 外设观察桥输出场景_ = nullptr;
    时间戳 外设观察桥最近处理时间_ = 0;

    bool 初始需求任务已就绪_ = false;

    // ====== 任务调度器：事件驱动（替代全量扫描任务链） ======
    任务调度器 调度器_{};
    任务执行器 根任务筹办器_{};
    std::atomic<bool> 调度器已初始化_{ false };

    // ====== 内部：初始化 + 调度 ======
    void 初始化_初始需求与任务();
    void 启动工作线程();
    void 停止工作线程();
    void 动作线程入口();
    void 学习线程入口();

    void 心跳调度_启动就绪任务();
    void 私有_扫描任务树并尝试入队(任务节点类* 根任务节点, const std::string& 标签);
    void 私有_刷新根任务就绪状态();
    void 私有_确保任务已引导筹办(任务节点类* 任务节点, const std::string& 标签);
    需求节点类* 私有_查找直属子需求_按类型(需求节点类* 父需求, const 词性节点类* 需求类型) const;
    任务节点类* 私有_查找首个步骤节点(任务节点类* 任务节点) const;
    bool 私有_任务存在活跃直属子任务(任务节点类* 任务节点) const;
    std::optional<I64> 私有_读取任务事实I64(任务节点类* 任务节点, const std::string& 键) const;
    std::optional<std::uintptr_t> 私有_读取任务事实指针(任务节点类* 任务节点, const std::string& 键) const;
    任务节点类* 私有_确保显式阶段任务(
        任务节点类* 根任务节点,
        需求节点类* 根需求节点,
        const std::string& 阶段标签,
        const std::string& 阶段需求类型名,
        const std::string& 阶段任务名,
        const std::string& 阶段任务类型名,
        const std::string& 阶段步骤名,
        const std::string& 目标事实键,
        I64 目标值,
        std::initializer_list<枚举_本能动作ID> 方法列表,
        std::int64_t 局部优先级偏移 = 0);
    bool 私有_分解安全根任务(任务节点类* 根任务节点, 需求节点类* 根需求节点, 时间戳 now);
    bool 私有_分解服务根任务(任务节点类* 根任务节点, 需求节点类* 根需求节点, 时间戳 now);
    std::size_t 私有_统计直属子需求数_已加锁(需求节点类* 需求) const;
    double 私有_计算需求节点承接总权值_已加锁(需求节点类* 需求) const;
    double 私有_计算需求子树叶子总权值_已加锁(需求节点类* 需求, double 节点总权值) const;
    std::int64_t 私有_压缩需求权值到调度优先级(double 权值) const;
    时间戳 私有_估计步骤执行所需时间_微秒(任务节点类* 步骤节点) const;
    时间戳 私有_估计任务执行所需时间_微秒(任务节点类* 任务节点) const;
    void 私有_回写任务步骤时效(任务节点类* 任务节点, 时间戳 需求截止时间, const std::string& 标签);
    void 私有_刷新任务调度优先级(任务节点类* 任务节点, const std::string& 标签);
    void 私有_刷新基础方向概念与需求(const std::string& 调用点);
    bool 私有_任务目标命中(const 任务信息基类& t) const;
    bool 私有_任务可调度(const 任务信息基类& t) const;
    void 私有_初始化自我特征(时间戳 now);
    void 私有_注册已有本能方法并更新待学习数量(时间戳 now);
    std::int64_t 私有_统计待学习方法数量() const;
    static bool 私有_当前阶段停用学习线程();
    static bool 私有_是当前阶段禁用的主动相机相关本能(枚举_本能动作ID 本能ID);
    bool 私有_方法允许参与当前阶段试探与学习(方法节点类* 方法首节点) const;
    I64 私有_判定方法尝试状态(方法节点类* 方法首节点, bool* outHasCondition = nullptr, bool* outHasResult = nullptr) const;
    void 私有_更新方法尝试状态_按观察(方法节点类* 方法首节点, 场景节点类* 观察输出, bool 观察成功, 时间戳 now);
    场景节点类* 私有_查找外设观察桥场景(const char* 名称) const;
    void 私有_消费外设观察桥观察();
    结构_学习推进结果 私有_尝试学习一步(时间戳 now);
    void 私有_唤醒等待学习的任务(方法节点类* 方法首节点, const std::string& 标签);
    std::vector<方法节点类*> 私有_生成根任务最小试探方法候选(const std::string& 标签, 时间戳 now);
    void 私有_尝试入队动作任务(任务节点类* 任务节点, const std::string& 标签);
    任务执行器::结构_叶子执行结果 私有_执行任务叶子步骤(任务节点类* 步骤节点);
    void 私有_同步方法事实到任务方向特征(任务节点类* 步骤节点, 枚举_本能动作ID 本能ID, 场景节点类* 输出场景, 时间戳 now);
    void 私有_刷新初始化完成状态();
    void 私有_清空故障摘要();
    void 私有_注册新存在创建通知();
    void 私有_注销新存在创建通知();
    void 置故障(const std::string& 摘要);

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
    enum class 枚举_需求所属根树 : std::uint8_t {
        未知 = 0,
        安全 = 1,
        服务 = 2
    };

    // ====== 内部工具 ======
    void 处理消息_任务(const 任务消息& m);
    void 处理消息_需求(const 任务消息& m);
    void 处理消息_权重(const 任务消息& m);
    void 处理消息_存在(const 任务消息& m);

    void 周期性维护();                 // 与外部数据结构交互
    void 同步自我核心特征到世界树(const std::string& 调用点);
    void 更新需求_基于观测(const std::string& key, double 强度);
    void 记录根方向摘要(const std::string& 调用点);
    void 校验根约束不变量(const std::string& 调用点);
    void 记录需求树不变量摘要(const std::string& 调用点);
    需求节点类* 私有_回溯根需求_已加锁(需求节点类* 需求) const;
    bool 私有_是否叶子需求_已加锁(需求节点类* 需求) const;
    枚举_需求所属根树 私有_判定需求所属根树_已加锁(需求节点类* 需求) const;
    double 私有_计算叶子需求结算总权值_已加锁(需求节点类* 叶子需求) const;
    U64 私有_结算总权值转增量(double 结算总权值) const;
    bool 私有_按叶子需求结算安全服务值(
        const 结构_任务返回消息& 返回消息,
        枚举_需求运行状态 旧需求状态,
        枚举_需求运行状态 新需求状态,
        const std::string& 调用点);
    bool 私有_按任务节点异步回执结算安全服务值(
        任务节点类* 任务节点,
        bool 成功,
        const std::string& 摘要,
        const std::string& 调用点);

    // 安全裁剪
    static double clip01(double x) { return std::clamp(x, 0.0, 1.0); }

};

// ==============================
// ✅ 自我线程模块：基础线程控制实现
// 说明：本模块先保证“启动/退出/停止/消息队列”可用，
// 具体的自我初始化/需求-任务逻辑可在后续逐步填充。
// ==============================

inline bool 自我线程类::是否正在运行() const {
    return 主线程存活_.load();
}

inline bool 自我线程类::是否初始化完成() const {
    return 初始化完成_.load();
}

inline bool 自我线程类::是否健康运行() const {
    return 运行状态_.load() == 枚举_自我运行状态::运行中
        && 初始化完成_.load()
        && !自我核心_.是否已消亡();
}

inline std::string 自我线程类::最近故障摘要() const {
    std::lock_guard<std::mutex> lk(故障锁_);
    return 最近故障摘要_;
}

inline void 自我线程类::私有_刷新初始化完成状态() {
    const bool 已完成 =
        安全需求节点_ != nullptr &&
        服务需求节点_ != nullptr &&
        安全任务节点_ != nullptr &&
        服务任务节点_ != nullptr &&
        调度器已初始化_.load();
    初始化完成_.store(已完成);
}

inline void 自我线程类::私有_清空故障摘要() {
    std::lock_guard<std::mutex> lk(故障锁_);
    最近故障摘要_.clear();
}

inline void 自我线程类::私有_注册新存在创建通知() {
    if (stop_.load()) return;

    auto 桥 = 新存在创建通知桥_;
    if (!桥) return;

    桥->注册([this](存在节点类* 新存在节点) {
        if (stop_.load()) return;
        if (!新存在节点) return;
        任务消息 msg{};
        msg.类型 = 消息类型::新存在创建;
        msg.存在节点指针 = 新存在节点;
        推送消息(std::move(msg));
    });

    if (stop_.load()) {
        桥->注销并等待();
        return;
    }

    世界树.设置新存在创建回调([弱桥 = std::weak_ptr<新存在创建通知桥>(桥)](存在节点类* 新存在节点) {
        auto 桥强 = 弱桥.lock();
        if (!桥强) return;
        桥强->通知(新存在节点);
    });

    if (stop_.load()) {
        世界树.设置新存在创建回调({});
        桥->注销并等待();
    }
}

inline void 自我线程类::私有_注销新存在创建通知() {
    世界树.设置新存在创建回调({});
    auto 桥 = 新存在创建通知桥_;
    if (!桥) return;
    桥->注销并等待();
}

inline void 自我线程类::置故障(const std::string& 摘要) {
    const auto 故障摘要 = 摘要.empty() ? std::string("未知故障") : 摘要;
    {
        std::lock_guard<std::mutex> lk(故障锁_);
        最近故障摘要_ = 故障摘要;
    }
    初始化完成_.store(false);
    运行状态_.store(枚举_自我运行状态::故障);
    日志::运行_错误("[自我线程][故障] " + 故障摘要);
}

inline void 自我线程类::启动() {
    if (是否正在运行()) return;
    if (self_thread_.joinable() || 动作线程_.joinable() || 学习线程_.joinable()) {
        请求退出();
        停止工作线程();
        等待线程结束();
    }

    初始化完成_.store(false);
    主线程存活_.store(false);
    运行状态_.store(枚举_自我运行状态::初始化中);
    stop_.store(false);
    启用学习线程_ = !私有_当前阶段停用学习线程();
    私有_清空故障摘要();
    {
        std::lock_guard<std::mutex> lk(mtx_);
        // 启动时投递一次“调度”消息，让系统进入事件驱动推进
        队列_.push_back(任务消息{ 消息类型::调度 });
    }

    const bool 启动成功 = 通用函数模块::安全执行_记录异常(
        "自我线程类::启动/创建线程",
        [this] {
            概念引擎_ = 概念引擎类{};

            self_thread_ = std::thread([this] {
                主线程存活_.store(true);
                运行状态_.store(枚举_自我运行状态::初始化中);

                // 线程启动日志
                日志::运行("[自我线程] 线程启动");

                const bool 初始化成功 = 通用函数模块::安全执行_记录异常(
                    "自我线程类::主线程/初始化",
                    [this] {
                        自我初始化();
                        私有_刷新初始化完成状态();
                    });
                if (!初始化成功) {
                    置故障("初始化失败");
                    请求退出();
                }
                else if (!stop_.load()) {
                    私有_注册新存在创建通知();
                    启动工作线程();
                    if (!初始化完成_.load()) {
                        置故障("初始化未完成: 根需求、根任务或调度器未就绪");
                        请求退出();
                    }
                    else {
                        运行状态_.store(枚举_自我运行状态::运行中);
                    }
                }

                while (!stop_.load()) {
                    if (自我核心_.是否已消亡()) {
                        日志::运行f(
                            "[自我线程][不变量] INV-004: 安全值归零触发线程退出: 安全值={}, 服务值={}",
                            自我核心_.安全值(),
                            自我核心_.服务值());
                        置故障("自我核心已消亡: 安全值归零");
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
                        const auto 消息类型文本 = std::to_string(static_cast<int>(msg.类型));
                        const auto 消息上下文 = std::string("自我线程类::主线程/处理消息 type=") + 消息类型文本;
                        const bool 消息处理成功 = 通用函数模块::安全执行_记录异常(
                            消息上下文,
                            [this, &msg] {
                                switch (msg.类型) {
                                case 消息类型::任务创建:
                                case 消息类型::任务开始:
                                case 消息类型::任务进度:
                                case 消息类型::任务完成:
                                case 消息类型::任务失败:
                                case 消息类型::任务异步回执:
                                case 消息类型::紧急中断:
                                    处理消息_任务(msg);
                                    break;
                                case 消息类型::需求观测:
                                    处理消息_需求(msg);
                                    break;
                                case 消息类型::权重外部调整:
                                    处理消息_权重(msg);
                                    break;
                                case 消息类型::新存在创建:
                                    处理消息_存在(msg);
                                    break;
                                case 消息类型::心跳:
                                case 消息类型::调度:
                                default:
                                    break;
                                }
                            });
                        if (!消息处理成功) {
                            置故障("处理消息失败: type=" + 消息类型文本);
                            请求退出();
                            break;
                        }
                    }

                    const bool 维护成功 = 通用函数模块::安全执行_记录异常(
                        "自我线程类::主线程/维护与调度",
                        [this] {
                            // 低频维护：摘要/衰减等（不驱动任务执行）
                            周期性维护();

                            // 事件驱动调度：推进“就绪任务”的下一步
                            心跳调度_启动就绪任务();
                        });
                    if (!维护成功) {
                        置故障("主循环维护失败");
                        请求退出();
                        break;
                    }
                }

                主线程存活_.store(false);

                // 线程结束日志
                日志::运行("[自我线程] 线程结束");
            });
        });
    if (!启动成功) {
        置故障("启动线程失败");
        请求退出();
        等待线程结束();
    }
}

inline void 自我线程类::请求退出() {
    stop_.store(true);
    const auto 当前状态 = 运行状态_.load();
    if (当前状态 != 枚举_自我运行状态::故障 &&
        当前状态 != 枚举_自我运行状态::已停止 &&
        当前状态 != 枚举_自我运行状态::未启动) {
        运行状态_.store(枚举_自我运行状态::退出中);
    }

    // 唤醒消息等待
    cv_.notify_all();
    // 唤醒执行线程池等待
    执行队列cv_.notify_all();
    学习队列cv_.notify_all();
}

inline void 自我线程类::等待线程结束() {
    日志::运行("[自我线程] 等待线程结束开始");
    私有_注销新存在创建通知();

    // 先等待工作线程（动作/学习）退出，避免主 self_thread 在收尾时与工作线程争用共享状态。
    const bool 工作者线程收尾成功 = 通用函数模块::安全执行_记录异常(
        "自我线程类::等待线程结束/工作线程",
        [this] {
            if (动作线程_.joinable()) {
                日志::运行("[自我线程] 等待动作线程结束开始");
                if (std::this_thread::get_id() == 动作线程_.get_id()) {
                    动作线程_.detach();
                }
                else {
                    动作线程_.join();
                }
                日志::运行("[自我线程] 等待动作线程结束完成");
            }
            if (学习线程_.joinable()) {
                日志::运行("[自我线程] 等待学习线程结束开始");
                if (std::this_thread::get_id() == 学习线程_.get_id()) {
                    学习线程_.detach();
                }
                else {
                    学习线程_.join();
                }
                日志::运行("[自我线程] 等待学习线程结束完成");
            }
        });
    if (!工作者线程收尾成功) {
        置故障("等待工作线程结束失败");
    }

    const bool 主线程收尾成功 = 通用函数模块::安全执行_记录异常(
        "自我线程类::等待线程结束/主线程",
        [this] {
            if (self_thread_.joinable()) {
                日志::运行("[自我线程] 等待主线程结束开始");
                if (std::this_thread::get_id() == self_thread_.get_id()) {
                    // 避免 self-join
                    self_thread_.detach();
                }
                else {
                    self_thread_.join();
                }
                日志::运行("[自我线程] 等待主线程结束完成");
            }
        });
    if (!主线程收尾成功) {
        置故障("等待主线程结束失败");
    }

    初始化完成_.store(false);
    主线程存活_.store(false);
    if (运行状态_.load() != 枚举_自我运行状态::故障 &&
        运行状态_.load() != 枚举_自我运行状态::未启动) {
        运行状态_.store(枚举_自我运行状态::已停止);
    }
    日志::运行("[自我线程] 等待线程结束完成");
}


inline void 自我线程类::停止() {
    日志::运行("[自我线程] 停止开始");
    const bool 有线程对象 =
        self_thread_.joinable() ||
        动作线程_.joinable() ||
        学习线程_.joinable();
    if (!有线程对象 && !是否正在运行()) {
        初始化完成_.store(false);
        if (运行状态_.load() != 枚举_自我运行状态::故障 &&
            运行状态_.load() != 枚举_自我运行状态::未启动) {
            运行状态_.store(枚举_自我运行状态::已停止);
        }
        日志::运行("[自我线程] 停止快速返回");
        return;
    }
    请求退出();
    停止工作线程();
    等待线程结束();
    日志::运行("[自我线程] 停止完成");
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
    私有_刷新初始化完成状态();

    日志::运行f("[自我线程] 自我初始化完成: self={}, 待学习方法数量={}, 安全值={}, 服务值={}, 初始化完成={}",
        (void*)自我指针,
        待学习方法数量_,
        自我核心_.安全值(),
        自我核心_.服务值(),
        初始化完成_.load() ? 1 : 0);
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

inline bool 自我线程类::私有_当前阶段停用学习线程() {
    return true;
}

inline bool 自我线程类::私有_是当前阶段禁用的主动相机相关本能(枚举_本能动作ID 本能ID) {
    switch (本能ID) {
    case 枚举_本能动作ID::外设_相机启动:
    case 枚举_本能动作ID::外设_相机关闭:
    case 枚举_本能动作ID::外设_设置相机参数:
    case 枚举_本能动作ID::外设_获取相机参数:
    case 枚举_本能动作ID::外设_获取帧信息:
    case 枚举_本能动作ID::外设_获取相机状态:
    case 枚举_本能动作ID::外设_冻结自动曝光:
    case 枚举_本能动作ID::外设_请求关键帧:
    case 枚举_本能动作ID::外设_重启相机:
    case 枚举_本能动作ID::外设_读取深度相机状态:
    case 枚举_本能动作ID::外设_获取单帧深度图:
    case 枚举_本能动作ID::外设_获取短窗深度序列:
    case 枚举_本能动作ID::外设_提取近场障碍事实:
    case 枚举_本能动作ID::外设_提取可通行空域事实:
    case 枚举_本能动作ID::外设_提取前景存在候选事实:
    case 枚举_本能动作ID::外设_提取深度变化事实:
    case 枚举_本能动作ID::自我_读取帧特征信息:
    case 枚举_本能动作ID::自我_获取画面识别率:
    case 枚举_本能动作ID::自我_查找存在:
    case 枚举_本能动作ID::自我_生成存在:
    case 枚举_本能动作ID::自我_更新存在:
    case 枚举_本能动作ID::自我_观察场景:
    case 枚举_本能动作ID::自我_计算未知区域:
    case 枚举_本能动作ID::自我_计算稳定度:
    case 枚举_本能动作ID::自我_标记未观测到:
        return true;
    default:
        return false;
    }
}

inline bool 自我线程类::私有_方法允许参与当前阶段试探与学习(方法节点类* 方法首节点) const {
    auto* hmi = 方法首节点 && 方法首节点->主信息
        ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息)
        : nullptr;
    if (!hmi) return true;
    if (hmi->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID) return true;
    if (hmi->动作句柄.本能ID_U64 == 0) return true;
    return !私有_是当前阶段禁用的主动相机相关本能(
        static_cast<枚举_本能动作ID>(hmi->动作句柄.本能ID_U64));
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
        if (!私有_方法允许参与当前阶段试探与学习(head)) continue;
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

inline 场景节点类* 自我线程类::私有_查找外设观察桥场景(const char* 名称) const {
    if (!名称 || !*名称) return nullptr;
    auto* parent = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!parent) return nullptr;

    const auto* 目标名称 = 语素集.添加词性词(名称, "名词");
    for (auto* 场景 : 场景类::获取子场景(parent)) {
        auto* 信息 = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        if (信息 && 信息->名称 == 目标名称) return 场景;
    }
    return nullptr;
}

inline void 自我线程类::私有_消费外设观察桥观察() {
    if (!本能集.有(枚举_本能动作ID::自我_观察场景)) return;

    if (!外设观察桥输入场景_ || !外设观察桥输入场景_->主信息) {
        外设观察桥输入场景_ = 私有_查找外设观察桥场景("主窗口_自我观察输入");
    }
    if (!外设观察桥输出场景_ || !外设观察桥输出场景_->主信息) {
        外设观察桥输出场景_ = 私有_查找外设观察桥场景("主窗口_自我观察输出");
    }

    auto* 输入场景 = 外设观察桥输入场景_;
    auto* 输出场景 = 外设观察桥输出场景_;
    auto* 输入信息 = (输入场景 && 输入场景->主信息) ? dynamic_cast<场景节点主信息类*>(输入场景->主信息) : nullptr;
    if (!输入信息 || !输出场景) return;
    if (输入信息->最后观测时间 <= 0 || 输入信息->最后观测时间 <= 外设观察桥最近处理时间_) return;

    场景节点类* 候选场景 = nullptr;
    const auto* 候选名称 = 语素集.添加词性词("候选存在集", "名词");
    for (auto* 场景 : 场景类::获取子场景(输入场景)) {
        auto* 信息 = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        if (信息 && 信息->名称 == 候选名称) {
            候选场景 = 场景;
            break;
        }
    }
    if (!候选场景) return;

    const auto 候选数 = 场景类::获取子存在(候选场景).size();
    日志::运行f(
        "[自我线程] 消费外设观察桥: 候选={}, 输入时间={}",
        候选数,
        static_cast<long long>(输入信息->最后观测时间));

    const bool ok = 本能集.调用(枚举_本能动作ID::自我_观察场景, 输入场景, 输出场景);
    外设观察桥最近处理时间_ = 输入信息->最后观测时间;

    const auto 读I64 = [&](const char* 名称) -> I64 {
        return 世界树.读取特征_I64(
            输出场景,
            语素集.添加词性词(名称, "名词"),
            std::string("自我线程类::私有_消费外设观察桥观察/读") + 名称).value_or(0);
    };

    日志::运行f(
        "[自我线程] 外设观察桥消费结果: 成功={}, 候选={}, 更新={}, 新建={}, 清理={}, 当前存在={}, 识别存在={}",
        ok ? 1 : 0,
        读I64("观察候选数量"),
        读I64("观察更新数量"),
        读I64("观察新建数量"),
        读I64("观察清理数量"),
        读I64("观察当前存在数量"),
        读I64("观察识别存在数量"));
}

inline void 自我线程类::私有_注册已有本能方法并更新待学习数量(时间戳 now) {
    外设本能方法类_相机模块{}.注册默认本能动作_外设相机();
    自我本能方法类_最小试探模块{}.注册默认本能动作_自我最小试探();
    自我本能方法类_相机认知模块{}.注册默认本能动作_自我相机认知();
    日志::运行("[自我线程] 当前阶段禁用主动相机相关试探/学习；相机仅保留主窗口链");

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
        if (私有_是当前阶段禁用的主动相机相关本能(id)) return;
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
        追加方法(枚举_本能动作ID::外设_提取前景存在候选事实);
        追加方法(枚举_本能动作ID::自我_观察场景);
        追加方法(枚举_本能动作ID::自我_停止当前试探并进入保守态);
    }
    else if (标签 == "服务") {
        追加方法(枚举_本能动作ID::自我_读取最新人类输入);
        追加方法(枚举_本能动作ID::自我_确认收到);
        追加方法(枚举_本能动作ID::自我_请求补充信息);
        追加方法(枚举_本能动作ID::外设_读取深度相机状态);
        追加方法(枚举_本能动作ID::外设_提取前景存在候选事实);
        追加方法(枚举_本能动作ID::自我_观察场景);
    }
    else {
        追加方法(枚举_本能动作ID::自我_读取核心状态);
        追加方法(枚举_本能动作ID::外设_提取前景存在候选事实);
        追加方法(枚举_本能动作ID::自我_观察场景);
    }

    return out;
}

inline void 自我线程类::私有_尝试入队动作任务(任务节点类* 任务节点, const std::string& 标签) {
    if (!任务节点 || !任务节点->主信息) return;
    auto* info = 任务节点->主信息;
    if (!私有_任务可调度(*info)) return;
    私有_刷新任务调度优先级(任务节点, 标签);

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    if (最大待执行队列长度_ > 0) {
        std::lock_guard<std::mutex> lk(执行队列锁_);
        if (执行队列_.size() >= 最大待执行队列长度_) {
            日志::运行f("[自我线程] 动作队列已满，跳过任务入队: 标签={}, 节点={}", 标签, (void*)任务节点);
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
    日志::运行f(
        "[自我线程] 动作任务已入队: 标签={}, 节点={}, 状态={}, 优先级={}",
        标签,
        (void*)任务节点,
        static_cast<int>(info->状态),
        static_cast<long long>(info->调度优先级));
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
    auto* 任务头节点 = stepInfo->所属任务头结点;
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

    日志::运行f(
        "[自我线程] 叶子步骤准备执行: 任务={}, 步骤={}, 方法={}, 本能ID={}",
        (void*)任务头节点,
        (void*)步骤节点,
        (void*)方法首节点,
        static_cast<unsigned long long>(hmi->动作句柄.本能ID_U64));

    const 时间戳 执行开始时间 = 结构体_时间戳::当前_微秒();
    const bool 成功 = 本能集.调用(本能ID, 输入场景, 输出场景);
    const 时间戳 执行结束时间 = 结构体_时间戳::当前_微秒();
    const std::int64_t 方法耗时微秒 = (执行结束时间 >= 执行开始时间) ? static_cast<std::int64_t>(执行结束时间 - 执行开始时间) : 0;
    日志::运行f(
        "[自我线程] 叶子步骤执行返回: 任务={}, 步骤={}, 方法={}, 成功={}, 耗时微秒={}",
        (void*)任务头节点,
        (void*)步骤节点,
        (void*)方法首节点,
        成功 ? 1 : 0,
        static_cast<long long>(方法耗时微秒));
    私有_同步方法事实到任务方向特征(步骤节点, 本能ID, 输出场景, now);

    out.成功 = 成功;
    out.结果场景 = 输出场景;
    out.摘要 = 本能集.查询函数名称(本能ID);
    if (out.摘要.empty()) out.摘要 = std::string("本能_") + std::to_string(hmi->动作句柄.本能ID_U64);
    out.建议动作 = 成功 ? 枚举_任务反馈动作::继续下一步 : 枚举_任务反馈动作::转入尝试学习;
    if (!成功) out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
    return out;
}
inline 自我线程类::结构_学习推进结果 自我线程类::私有_尝试学习一步(时间戳 now) {
    结构_学习推进结果 out{};
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
                if (!私有_方法允许参与当前阶段试探与学习(head)) return;

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

        日志::运行f(
            "[自我线程] 学习目标已选定: 方法={}, 动作={}, 缺条件={}, 缺结果={}",
            (void*)targetHead,
            actionKey.empty() ? std::string("<unknown>") : actionKey,
            missingCondition,
            missingResult);

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
            日志::运行f(
                "[自我线程] 学习线程准备调用本能: 方法={}, 动作={}, 本能ID={}",
                (void*)targetHead,
                actionKey.empty() ? std::string("<unknown>") : actionKey,
                static_cast<unsigned long long>(hmi->动作句柄.本能ID_U64));
            auto r = 方法集.调用本能函数(hmi->动作句柄.本能ID_U64, ctx, cancel);
            日志::运行f(
                "[自我线程] 学习线程本能返回: 方法={}, 动作={}, 成功={}",
                (void*)targetHead,
                actionKey.empty() ? std::string("<unknown>") : actionKey,
                r.成功);
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

    out.目标方法首节点 = targetHead;
    out.有进展 = progressed || observed;
    out.需要唤醒等待任务 = (targetHead != nullptr) && out.有进展;

    日志::运行f("[自我线程] 尝试学习一步: 动作={}, 补条件={}, 补结果={}, 观察成功={}, 候选={}, 更新={}, 新建={}, 剩余待学习方法数量={}",
        actionKey.empty() ? std::string("<unknown>") : actionKey,
        missingCondition,
        missingResult,
        observed,
        observedCount,
        updatedCount,
        createdCount,
        待学习方法数量_);
    return out;
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

inline void 自我线程类::处理消息_存在(const 任务消息& m) {
    if (!m.存在节点指针) return;
    概念引擎_.处理新存在(m.存在节点指针);
}

inline void 自我线程类::处理消息_任务(const 任务消息& m) {
    switch (m.类型) {
    case 消息类型::任务异步回执:
        if (m.任务节点指针) {
            (void)私有_按任务节点异步回执结算安全服务值(
                m.任务节点指针,
                m.标志,
                m.文本,
                "自我线程类::处理消息_任务/任务异步回执");
        }
        break;
    case 消息类型::任务完成:
    case 消息类型::任务失败:
        if (m.任务节点指针) {
            (void)私有_按任务节点异步回执结算安全服务值(
                m.任务节点指针,
                m.类型 == 消息类型::任务完成,
                m.文本,
                "自我线程类::处理消息_任务/兼容旧完成失败消息");
        }
        else {
            日志::运行f(
                "[自我线程] 跳过旧任务结算消息: 类型={}, 任务ID={}, 原因=未携带任务节点，无法按叶子需求结算",
                static_cast<int>(m.类型),
                m.任务ID);
        }
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

    const auto 统计I64状态数 = [&](const 特征节点类* 目标特征) -> std::size_t {
        if (!内部世界 || !内部世界->主信息 || !目标特征) return 0;
        auto* smi = dynamic_cast<场景节点主信息类*>(内部世界->主信息);
        if (!smi) return 0;

        std::size_t 数量 = 0;
        for (auto* s : smi->状态列表) {
            auto* mi = (s && s->主信息) ? dynamic_cast<状态节点主信息类*>(s->主信息) : nullptr;
            if (!mi) continue;
            if (mi->状态域 != 枚举_状态域::内部状态) continue;
            if (mi->状态主体 != 自我存在) continue;
            if (mi->状态特征 != 目标特征) continue;
            if (!std::holds_alternative<I64>(mi->状态值)) continue;
            ++数量;
        }
        return 数量;
    };

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

    私有_消费外设观察桥观察();

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


inline std::size_t 自我线程类::私有_统计直属子需求数_已加锁(需求节点类* 需求) const {
    if (!需求 || !需求->子) return 0;

    std::size_t 数量 = 0;
    auto* first = static_cast<需求节点类*>(需求->子);
    auto* cur = first;
    do {
        if (cur && cur->主信息) {
            ++数量;
        }
        cur = cur ? static_cast<需求节点类*>(cur->下) : nullptr;
    } while (cur && cur != first);
    return 数量;
}

inline double 自我线程类::私有_计算需求节点承接总权值_已加锁(需求节点类* 需求) const {
    if (!需求 || !需求->主信息) return 0.0;

    std::vector<需求节点类*> 路径;
    for (auto* cur = 需求; cur; cur = static_cast<需求节点类*>(cur->父)) {
        if (!cur->主信息) return 0.0;
        路径.push_back(cur);
    }
    std::reverse(路径.begin(), 路径.end());

    double 当前总权值 = 0.0;
    for (std::size_t i = 0; i < 路径.size(); ++i) {
        auto* 当前需求 = 路径[i];
        当前总权值 += static_cast<double>(当前需求->主信息->权重);
        if (i + 1 >= 路径.size()) {
            return 当前总权值;
        }

        const auto 子数量 = 私有_统计直属子需求数_已加锁(当前需求);
        if (子数量 == 0) {
            return 当前总权值;
        }
        当前总权值 /= static_cast<double>(子数量);
    }
    return 当前总权值;
}

inline double 自我线程类::私有_计算需求子树叶子总权值_已加锁(需求节点类* 需求, double 节点总权值) const {
    if (!需求 || !需求->主信息) return 0.0;

    std::vector<需求节点类*> 子需求列表;
    if (需求->子) {
        auto* first = static_cast<需求节点类*>(需求->子);
        auto* cur = first;
        do {
            if (cur && cur->主信息) {
                子需求列表.push_back(cur);
            }
            cur = cur ? static_cast<需求节点类*>(cur->下) : nullptr;
        } while (cur && cur != first);
    }

    if (子需求列表.empty()) {
        return 节点总权值;
    }

    const double 每子继承权值 = 节点总权值 / static_cast<double>(子需求列表.size());
    double 汇总 = 0.0;
    for (auto* 子需求 : 子需求列表) {
        const double 子总权值 = 每子继承权值 + static_cast<double>(子需求->主信息->权重);
        汇总 += 私有_计算需求子树叶子总权值_已加锁(子需求, 子总权值);
    }
    return 汇总;
}

inline 需求节点类* 自我线程类::私有_回溯根需求_已加锁(需求节点类* 需求) const {
    auto* 当前 = 需求;
    while (当前 && 当前->父) {
        当前 = static_cast<需求节点类*>(当前->父);
    }
    return 当前;
}

inline bool 自我线程类::私有_是否叶子需求_已加锁(需求节点类* 需求) const {
    if (!需求 || !需求->主信息) return false;
    return 需求->子 == nullptr;
}

inline 自我线程类::枚举_需求所属根树 自我线程类::私有_判定需求所属根树_已加锁(需求节点类* 需求) const {
    auto* 根需求 = 私有_回溯根需求_已加锁(需求);
    if (根需求 == 安全需求节点_) return 枚举_需求所属根树::安全;
    if (根需求 == 服务需求节点_) return 枚举_需求所属根树::服务;
    return 枚举_需求所属根树::未知;
}

inline double 自我线程类::私有_计算叶子需求结算总权值_已加锁(需求节点类* 叶子需求) const {
    if (!叶子需求 || !叶子需求->主信息) return 0.0;
    if (!私有_是否叶子需求_已加锁(叶子需求)) return 0.0;
    return 私有_计算需求节点承接总权值_已加锁(叶子需求);
}

inline U64 自我线程类::私有_结算总权值转增量(double 结算总权值) const {
    if (!std::isfinite(结算总权值) || 结算总权值 <= 0.0) {
        return 0;
    }
    return 通用函数模块::非负双精度转U64饱和(结算总权值);
}

inline bool 自我线程类::私有_按叶子需求结算安全服务值(
    const 结构_任务返回消息& 返回消息,
    枚举_需求运行状态 旧需求状态,
    枚举_需求运行状态 新需求状态,
    const std::string& 调用点)
{
    if (!返回消息.来源任务 || !返回消息.来源任务->主信息) return false;
    if (!返回消息.对应需求 || !返回消息.对应需求->主信息) return false;

    auto* 头信息 = dynamic_cast<任务头结点信息*>(返回消息.来源任务->主信息);
    if (!头信息) return false;
    if (头信息->是否真根任务) return false;

    if (返回消息.类型 != 枚举_任务返回消息类型::步骤完成) return false;
    if (返回消息.已进入下一阶段) return false;
    if (返回消息.需要重筹办) return false;
    if (!返回消息.需求已满足) return false;

    if (旧需求状态 == 枚举_需求运行状态::已满足) return false;
    if (新需求状态 != 枚举_需求运行状态::已满足) return false;

    枚举_需求所属根树 所属根树 = 枚举_需求所属根树::未知;
    double 结算总权值 = 0.0;
    U64 结算增量 = 0;

    {
        锁调度器守卫 锁({
            锁请求::读(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点 + "/需求链读锁")
        });

        if (!私有_是否叶子需求_已加锁(返回消息.对应需求)) {
            return false;
        }

        所属根树 = 私有_判定需求所属根树_已加锁(返回消息.对应需求);
        if (所属根树 == 枚举_需求所属根树::未知) {
            return false;
        }

        结算总权值 = 私有_计算叶子需求结算总权值_已加锁(返回消息.对应需求);
        结算增量 = 私有_结算总权值转增量(结算总权值);
    }

    if (结算增量 == 0) {
        日志::运行f(
            "[自我线程] 叶子需求结算跳过: 需求={}, 所属根树={}, 结算总权值={:.3f}, 增量=0",
            (void*)返回消息.对应需求,
            static_cast<int>(所属根树),
            结算总权值);
        return false;
    }

    const U64 安全前 = 自我核心_.安全值();
    const U64 服务前 = 自我核心_.服务值();

    switch (所属根树) {
    case 枚举_需求所属根树::安全:
        自我核心_.安全值增加(结算增量);
        break;
    case 枚举_需求所属根树::服务:
        自我核心_.服务值增加(结算增量);
        break;
    default:
        return false;
    }

    同步自我核心特征到世界树(调用点 + "/同步安全服务值");
    私有_刷新基础方向概念与需求(调用点 + "/刷新方向需求");
    记录根方向摘要(调用点 + "/记录根方向");
    校验根约束不变量(调用点 + "/校验根约束");
    私有_刷新根任务就绪状态();

    日志::运行f(
        "[自我线程] 叶子需求结算完成: 根树={}, 需求={}, 任务={}, 结算总权值={:.3f}, 增量={}, 安全值 {} -> {}, 服务值 {} -> {}, 摘要={}",
        所属根树 == 枚举_需求所属根树::安全 ? "安全" : "服务",
        (void*)返回消息.对应需求,
        (void*)返回消息.来源任务,
        结算总权值,
        结算增量,
        安全前,
        自我核心_.安全值(),
        服务前,
        自我核心_.服务值(),
        返回消息.摘要);

    return true;
}

inline bool 自我线程类::私有_按任务节点异步回执结算安全服务值(
    任务节点类* 任务节点,
    bool 成功,
    const std::string& 摘要,
    const std::string& 调用点)
{
    if (!任务节点 || !任务节点->主信息) return false;

    auto 推导所属任务头 = [](任务节点类* 节点) -> 任务节点类* {
        if (!节点 || !节点->主信息) return nullptr;
        if (dynamic_cast<任务头结点信息*>(节点->主信息)) {
            return 节点;
        }
        if (auto* 步骤信息 = dynamic_cast<任务步骤节点信息*>(节点->主信息)) {
            return 步骤信息->所属任务头结点;
        }
        if (auto* 结果信息 = dynamic_cast<任务结果节点信息*>(节点->主信息)) {
            return 结果信息->所属任务头结点;
        }
        return nullptr;
    };

    auto* 任务头 = 推导所属任务头(任务节点);
    auto* 头信息 = (任务头 && 任务头->主信息) ? dynamic_cast<任务头结点信息*>(任务头->主信息) : nullptr;
    if (!任务头 || !头信息 || !头信息->需求) return false;

    const auto 旧需求状态 = 需求状态管理器.查询需求状态(头信息->需求);

    需求状态管理器.刷新需求及父链状态(
        头信息->需求,
        调用点 + "/刷新需求父链");

    const auto 新需求状态 = 需求状态管理器.查询需求状态(头信息->需求);

    结构_任务返回消息 伪返回消息{};
    伪返回消息.类型 = 成功 ? 枚举_任务返回消息类型::步骤完成 : 枚举_任务返回消息类型::步骤失败;
    伪返回消息.来源任务 = 任务头;
    伪返回消息.来源步骤 = nullptr;
    伪返回消息.相关子任务 = nullptr;
    伪返回消息.对应需求 = 头信息->需求;
    伪返回消息.需求已满足 = (新需求状态 == 枚举_需求运行状态::已满足);
    伪返回消息.已进入下一阶段 = false;
    伪返回消息.需要重筹办 = false;
    伪返回消息.摘要 = 摘要;

    if (!成功) {
        return false;
    }

    return 私有_按叶子需求结算安全服务值(
        伪返回消息,
        旧需求状态,
        新需求状态,
        调用点 + "/叶子结算");
}

inline std::int64_t 自我线程类::私有_压缩需求权值到调度优先级(double 权值) const {
    if (!std::isfinite(权值) || 权值 == 0.0) return 0;

    const double 符号 = 权值 < 0.0 ? -1.0 : 1.0;
    const double 绝对值 = std::fabs(权值);
    const double 压缩值 = std::log2(绝对值 + 1.0) * 1024.0;
    const double 带符号压缩值 = std::round(压缩值) * 符号;
    const double 上限 = static_cast<double>((std::numeric_limits<std::int64_t>::max)());
    const double 下限 = static_cast<double>((std::numeric_limits<std::int64_t>::min)());
    if (带符号压缩值 >= 上限) return (std::numeric_limits<std::int64_t>::max)();
    if (带符号压缩值 <= 下限) return (std::numeric_limits<std::int64_t>::min)();
    return static_cast<std::int64_t>(带符号压缩值);
}

inline 时间戳 自我线程类::私有_估计步骤执行所需时间_微秒(任务节点类* 步骤节点) const {
    constexpr 时间戳 默认方法执行时长_微秒 = 1000000;

    auto* 步骤信息 = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;
    if (!步骤信息) return 默认方法执行时长_微秒;

    auto* 方法首节点 = 步骤信息->当前选中方法首节点;
    if (!方法首节点 && !步骤信息->可用方法首节点列表.empty()) {
        方法首节点 = 步骤信息->可用方法首节点列表.front();
    }
    auto* 方法信息 = (方法首节点 && 方法首节点->主信息) ? dynamic_cast<方法信息基类*>(方法首节点->主信息) : nullptr;
    auto* 方法虚拟存在 = 方法信息 ? 方法信息->方法虚拟存在根 : nullptr;
    if (!方法虚拟存在) return 默认方法执行时长_微秒;

    const auto* 平均耗时类型 = 语素集.添加词性词("节点_平均耗时_ms_EMA", "名词");
    const auto* 最近耗时类型 = 语素集.添加词性词("节点_最近耗时_ms", "名词");
    I64 耗时毫秒 = 世界树.读取特征_I64(
        方法虚拟存在,
        平均耗时类型,
        "自我线程类::私有_估计步骤执行所需时间_微秒/读平均耗时").value_or(0);
    if (耗时毫秒 <= 0) {
        耗时毫秒 = 世界树.读取特征_I64(
            方法虚拟存在,
            最近耗时类型,
            "自我线程类::私有_估计步骤执行所需时间_微秒/读最近耗时").value_or(0);
    }
    if (耗时毫秒 <= 0) return 默认方法执行时长_微秒;
    return static_cast<时间戳>(耗时毫秒) * 1000;
}

inline 时间戳 自我线程类::私有_估计任务执行所需时间_微秒(任务节点类* 任务节点) const {
    constexpr 时间戳 默认方法执行时长_微秒 = 1000000;

    auto* 头信息 = (任务节点 && 任务节点->主信息) ? dynamic_cast<任务头结点信息*>(任务节点->主信息) : nullptr;
    if (!头信息) return 默认方法执行时长_微秒;

    auto* 步骤节点 = 头信息->当前步骤节点;
    auto* 估计步骤节点 = 步骤节点;
    任务步骤节点信息* 步骤信息 = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;
    if (!步骤信息 && 任务节点->子) {
        auto* first = static_cast<任务节点类*>(任务节点->子);
        auto* cur = first;
        do {
            if (cur && cur->主信息) {
                步骤信息 = dynamic_cast<任务步骤节点信息*>(cur->主信息);
                if (步骤信息) {
                    估计步骤节点 = cur;
                    break;
                }
            }
            cur = cur ? static_cast<任务节点类*>(cur->下) : nullptr;
        } while (cur && cur != first);
    }
    if (!步骤信息) return 默认方法执行时长_微秒;

    return 私有_估计步骤执行所需时间_微秒(估计步骤节点);
}

inline void 自我线程类::私有_回写任务步骤时效(任务节点类* 任务节点, 时间戳 需求截止时间, const std::string& 标签) {
    if (!任务节点 || !任务节点->主信息 || 需求截止时间 == 0) return;

    std::vector<std::pair<std::int32_t, 任务节点类*>> 步骤列表;
    {
        锁调度器守卫 锁({
            锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", "自我线程类::私有_回写任务步骤时效/" + 标签)
        });

        auto* 起 = 任务节点->子;
        auto* it = 起;
        while (it) {
            auto* 当前 = static_cast<任务节点类*>(it);
            if (当前 && 当前->主信息) {
                if (auto* 步骤信息 = dynamic_cast<任务步骤节点信息*>(当前->主信息)) {
                    步骤列表.push_back({ 步骤信息->步骤序号, 当前 });
                }
            }
            it = static_cast<任务节点类*>(it->下);
            if (it == 起) break;
        }

        if (步骤列表.empty()) return;
        std::sort(步骤列表.begin(), 步骤列表.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        时间戳 当前步骤截止时间 = 需求截止时间;
        for (auto rit = 步骤列表.rbegin(); rit != 步骤列表.rend(); ++rit) {
            auto* 步骤节点 = rit->second;
            auto* 步骤信息 = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;
            if (!步骤信息) continue;

            if (步骤信息->超时截止时间 != 当前步骤截止时间) {
                步骤信息->超时截止时间 = 当前步骤截止时间;
            }

            const 时间戳 预计执行时长 = 私有_估计步骤执行所需时间_微秒(步骤节点);
            当前步骤截止时间 = (当前步骤截止时间 > 预计执行时长) ? (当前步骤截止时间 - 预计执行时长) : 0;
        }
    }
}

inline void 自我线程类::私有_刷新任务调度优先级(任务节点类* 任务节点, const std::string& 标签) {
    auto* 头信息 = (任务节点 && 任务节点->主信息) ? dynamic_cast<任务头结点信息*>(任务节点->主信息) : nullptr;
    if (!头信息 || !头信息->需求) return;

    double 叶子总权值 = 0.0;
    时间戳 需求截止时间 = 0;
    std::int64_t 根类优先级基线 = 0;
    {
        锁调度器守卫 锁({
            锁请求::读(需求链.链表锁, 枚举_锁域::需求链, "需求链", "自我线程类::私有_刷新任务调度优先级/" + 标签)
        });

        if (头信息->需求->主信息) {
            需求截止时间 = 头信息->需求->主信息->需求有效截止;
        }
        auto* 根需求 = 头信息->需求;
        while (根需求 && 根需求->父) {
            根需求 = static_cast<需求节点类*>(根需求->父);
        }
        if (根需求 == 安全需求节点_) 根类优先级基线 = 2000000000LL;
        else if (根需求 == 服务需求节点_) 根类优先级基线 = 1000000000LL;

        const double 节点总权值 = 私有_计算需求节点承接总权值_已加锁(头信息->需求);
        叶子总权值 = 私有_计算需求子树叶子总权值_已加锁(头信息->需求, 节点总权值);
    }

    if (需求截止时间 > 0) {
        私有_回写任务步骤时效(任务节点, 需求截止时间, 标签);
        if (auto* 当前步骤信息 = (头信息->当前步骤节点 && 头信息->当前步骤节点->主信息)
            ? dynamic_cast<任务步骤节点信息*>(头信息->当前步骤节点->主信息)
            : nullptr) {
            if (当前步骤信息->超时截止时间 > 0) {
                需求截止时间 = 当前步骤信息->超时截止时间;
            }
        }
    }

    const double 合成后权值 = 叶子总权值 * std::max(0.0, W_需求合成_);
    const std::int64_t 权值优先级 = 私有_压缩需求权值到调度优先级(合成后权值);
    const 时间戳 now = 结构体_时间戳::当前_微秒();
    std::int64_t 时间紧急基线 = 0;
    if (需求截止时间 > 0) {
        const 时间戳 预计执行时长 = 私有_估计任务执行所需时间_微秒(任务节点);
        const 时间戳 剩余时间 = (需求截止时间 > now) ? (需求截止时间 - now) : 0;
        if (需求截止时间 <= now || 预计执行时长 > 剩余时间) {
            时间紧急基线 = 200000000LL;
        }
        else if ((剩余时间 - 预计执行时长) < 预计执行时长) {
            时间紧急基线 = 100000000LL;
        }
    }

    auto 饱和加I64 = [](std::int64_t a, std::int64_t b) -> std::int64_t {
        if (b > 0 && a > (std::numeric_limits<std::int64_t>::max)() - b) return (std::numeric_limits<std::int64_t>::max)();
        if (b < 0 && a < (std::numeric_limits<std::int64_t>::min)() - b) return (std::numeric_limits<std::int64_t>::min)();
        return a + b;
    };

    const std::int64_t 新基准优先级 = 饱和加I64(饱和加I64(根类优先级基线, 时间紧急基线), 权值优先级);
    std::int64_t 新调度优先级 = 0;
    if (头信息->局部优先级偏移 > 0
        && 新基准优先级 > (std::numeric_limits<std::int64_t>::max)() - 头信息->局部优先级偏移) {
        新调度优先级 = (std::numeric_limits<std::int64_t>::max)();
    }
    else if (头信息->局部优先级偏移 < 0
        && 新基准优先级 < (std::numeric_limits<std::int64_t>::min)() - 头信息->局部优先级偏移) {
        新调度优先级 = (std::numeric_limits<std::int64_t>::min)();
    }
    else {
        新调度优先级 = 新基准优先级 + 头信息->局部优先级偏移;
    }

    if (头信息->基准优先级 != 新基准优先级) {
        (void)任务类::设置任务基准优先级(
            头信息,
            新基准优先级,
            now,
            "自我线程类::私有_刷新任务调度优先级/基准/" + 标签);
    }
    if (头信息->调度优先级 != 新调度优先级) {
        (void)任务类::设置任务调度优先级(
            头信息,
            新调度优先级,
            now,
            "自我线程类::私有_刷新任务调度优先级/调度/" + 标签);
    }
}

inline bool 自我线程类::私有_任务可调度(const 任务信息基类& t) const {
    if (t.状态 == 枚举_任务状态::完成) return false;
    if (t.状态 == 枚举_任务状态::取消) return false;
    if (t.状态 == 枚举_任务状态::失败) return false;
    if (t.状态 == 枚举_任务状态::无法执行) return false;
    if (t.状态 == 枚举_任务状态::执行中) return false;
    if (t.状态 == 枚举_任务状态::待重筹办) return false;
    if (t.状态 == 枚举_任务状态::排队中) return false;
    if (t.状态 == 枚举_任务状态::等待中) return false;
    if (t.状态 == 枚举_任务状态::挂起) return false;
    return true;
}

inline 需求节点类* 自我线程类::私有_查找直属子需求_按类型(需求节点类* 父需求, const 词性节点类* 需求类型) const {
    if (!父需求 || !需求类型 || !父需求->子) return nullptr;

    auto* first = static_cast<需求节点类*>(父需求->子);
    auto* cur = first;
    do {
        if (cur && cur->主信息 && cur->主信息->类型 == 需求类型) {
            return cur;
        }
        cur = static_cast<需求节点类*>(cur->下);
    } while (cur && cur != first);

    return nullptr;
}

inline 任务节点类* 自我线程类::私有_查找首个步骤节点(任务节点类* 任务节点) const {
    if (!任务节点 || !任务节点->子) return nullptr;

    auto* first = static_cast<任务节点类*>(任务节点->子);
    auto* cur = first;
    do {
        if (cur && cur->主信息 && dynamic_cast<任务步骤节点信息*>(cur->主信息)) {
            return cur;
        }
        cur = static_cast<任务节点类*>(cur->下);
    } while (cur && cur != first);

    return nullptr;
}

inline bool 自我线程类::私有_任务存在活跃直属子任务(任务节点类* 任务节点) const {
    auto* 头信息 = (任务节点 && 任务节点->主信息) ? dynamic_cast<任务头结点信息*>(任务节点->主信息) : nullptr;
    if (!头信息) return false;

    auto 是否终结 = [](枚举_任务状态 状态) noexcept {
        return 状态 == 枚举_任务状态::完成
            || 状态 == 枚举_任务状态::取消
            || 状态 == 枚举_任务状态::失败
            || 状态 == 枚举_任务状态::无法执行;
    };

    auto 子任务活跃 = [&](任务节点类* 子任务头) -> bool {
        auto* 子头信息 = (子任务头 && 子任务头->主信息) ? dynamic_cast<任务头结点信息*>(子任务头->主信息) : nullptr;
        if (!子头信息) return false;

        const bool 需求未满足 = 子头信息->需求
            && 需求状态管理器.查询需求状态(子头信息->需求) != 枚举_需求运行状态::已满足;
        return !是否终结(子头信息->状态) || 需求未满足;
    };

    for (auto* 子任务头 : 头信息->直属子任务头结点列表) {
        if (子任务活跃(子任务头)) return true;
    }

    if (!任务节点->子) return false;
    auto* first = static_cast<任务节点类*>(任务节点->子);
    auto* cur = first;
    do {
        if (dynamic_cast<任务头结点信息*>(cur ? cur->主信息 : nullptr) && 子任务活跃(cur)) {
            return true;
        }
        cur = static_cast<任务节点类*>(cur->下);
    } while (cur && cur != first);

    return false;
}

inline std::optional<I64> 自我线程类::私有_读取任务事实I64(任务节点类* 任务节点, const std::string& 键) const {
    auto* info = 任务节点 ? 任务节点->主信息 : nullptr;
    if (!info || !info->任务虚拟存在) return std::nullopt;

    return 世界树.读取特征_I64(
        info->任务虚拟存在,
        自我线程模块_detail::特征_任务事实(键),
        "自我线程类::私有_读取任务事实I64/" + 键);
}

inline std::optional<std::uintptr_t> 自我线程类::私有_读取任务事实指针(任务节点类* 任务节点, const std::string& 键) const {
    auto* info = 任务节点 ? 任务节点->主信息 : nullptr;
    if (!info || !info->任务虚拟存在) return std::nullopt;

    auto 快照 = 世界树.读取特征快照(
        info->任务虚拟存在,
        自我线程模块_detail::特征_任务事实(键),
        "自我线程类::私有_读取任务事实指针/" + 键);
    if (!快照.has_value() || !std::holds_alternative<指针句柄>(*快照)) {
        return std::nullopt;
    }

    return std::get<指针句柄>(*快照).指针;
}

inline 任务节点类* 自我线程类::私有_确保显式阶段任务(
    任务节点类* 根任务节点,
    需求节点类* 根需求节点,
    const std::string& 阶段标签,
    const std::string& 阶段需求类型名,
    const std::string& 阶段任务名,
    const std::string& 阶段任务类型名,
    const std::string& 阶段步骤名,
    const std::string& 目标事实键,
    I64 目标值,
    std::initializer_list<枚举_本能动作ID> 方法列表,
    std::int64_t 局部优先级偏移)
{
    auto* 根头信息 = (根任务节点 && 根任务节点->主信息) ? dynamic_cast<任务头结点信息*>(根任务节点->主信息) : nullptr;
    auto* 根需求信息 = (根需求节点 && 根需求节点->主信息) ? 根需求节点->主信息 : nullptr;
    if (!根头信息 || !根需求信息) return nullptr;

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    const auto* 阶段需求类型 = 语素集.添加词性词(阶段需求类型名, "名词");
    const auto* 阶段任务名称 = 语素集.添加词性词(阶段任务名, "名词");
    const auto* 阶段任务类型 = 语素集.添加词性词(阶段任务类型名, "名词");
    const auto* 阶段步骤名称 = 语素集.添加词性词(阶段步骤名, "名词");
    const auto* 阶段步骤类型 = 语素集.添加词性词("显式阶段步骤", "名词");
    const auto* 阶段场景名称 = 语素集.添加词性词(阶段标签, "名词");
    bool 新建阶段任务 = false;
    bool 新建阶段需求 = false;

    auto* 阶段需求 = 私有_查找直属子需求_按类型(根需求节点, 阶段需求类型);
    任务节点类* 阶段任务节点 = (阶段需求 && 阶段需求->主信息) ? 阶段需求->主信息->相关任务 : nullptr;

    if (!阶段任务节点) {
        for (auto* 子任务头 : 根头信息->直属子任务头结点列表) {
            auto* 子头信息 = (子任务头 && 子任务头->主信息) ? dynamic_cast<任务头结点信息*>(子任务头->主信息) : nullptr;
            if (!子头信息) continue;
            if (子头信息->类型 == 阶段任务类型 || 子头信息->名称 == 阶段任务名称) {
                阶段任务节点 = 子任务头;
                break;
            }
        }
    }

    auto* 场景父 = 根需求信息->需求场景
        ? static_cast<基础信息节点类*>(根需求信息->需求场景)
        : (根头信息->场景
            ? static_cast<基础信息节点类*>(根头信息->场景)
            : static_cast<基础信息节点类*>(世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界));
    auto* 阶段场景 = 世界树.取或创建子场景_按名称(
        场景父,
        阶段场景名称,
        now,
        "自我线程类::私有_确保显式阶段任务/阶段场景/" + 阶段标签);
    if (!阶段场景) return nullptr;

    if (!阶段任务节点) {
        任务类::结构_创建任务头参数 阶段任务参数{};
        阶段任务参数.名称 = 阶段任务名称;
        阶段任务参数.类型 = 阶段任务类型;
        阶段任务参数.初始场景 = 阶段场景;
        阶段任务参数.创建时间 = now;
        阶段任务参数.初始状态 = 枚举_任务状态::就绪;
        阶段任务参数.任务树类型 = 枚举_任务树类型::叶子任务;
        阶段任务参数.自动关联到需求 = false;
        阶段任务参数.局部优先级偏移 = 局部优先级偏移;
        阶段任务节点 = 任务类::创建任务头结点(
            根需求节点,
            阶段任务参数,
            根任务节点,
            "自我线程类::私有_确保显式阶段任务/创建阶段任务/" + 阶段标签);
        if (!阶段任务节点) return nullptr;
        新建阶段任务 = true;
    }

    (void)任务类::更新任务头结点场景(
        阶段任务节点,
        阶段场景,
        now,
        "自我线程类::私有_确保显式阶段任务/同步阶段场景/" + 阶段标签);

    if (!阶段需求) {
        auto* 任务虚拟存在 = 阶段任务节点->主信息 ? 阶段任务节点->主信息->任务虚拟存在 : nullptr;
        if (!任务虚拟存在) return 阶段任务节点;

        auto* 目标特征 = 世界树.确保特征(
            任务虚拟存在,
            自我线程模块_detail::特征_任务事实(目标事实键),
            自我线程模块_detail::特征_任务事实(目标事实键),
            "自我线程类::私有_确保显式阶段任务/目标特征/" + 阶段标签);
        if (!目标特征) return 阶段任务节点;

        auto* 目标状态 = 状态集.创建世界状态(
            阶段场景,
            任务虚拟存在,
            目标特征,
            特征快照值{ 目标值 },
            枚举_存在状态事件::创建,
            true,
            now);
        if (!目标状态) return 阶段任务节点;

        需求类::结构_创建参数 阶段需求参数{};
        阶段需求参数.类型 = 阶段需求类型;
        阶段需求参数.发生场景 = 阶段场景;
        阶段需求参数.生成时间 = now;
        阶段需求参数.权重 = 根需求信息->权重;
        阶段需求 = 需求集.创建需求(
            根需求信息->需求主体 ? 根需求信息->需求主体 : 自我指针,
            任务虚拟存在,
            目标状态,
            阶段需求参数,
            "自我线程类::私有_确保显式阶段任务/创建阶段需求/" + 阶段标签,
            根需求节点);
        新建阶段需求 = 阶段需求 != nullptr;
    }

    if (!阶段需求) return 阶段任务节点;

    (void)任务类::更新任务头结点需求(
        阶段任务节点,
        阶段需求,
        true,
        "自我线程类::私有_确保显式阶段任务/绑定阶段需求/" + 阶段标签);

    需求状态管理器.登记需求及子树(
        阶段需求,
        "自我线程类::私有_确保显式阶段任务/登记需求/" + 阶段标签);
    if (调度器已初始化_.load()) {
        调度器_.登记需求(阶段需求);
    }

    std::vector<方法节点类*> 阶段方法列表{};
    for (auto 本能ID : 方法列表) {
        if (私有_是当前阶段禁用的主动相机相关本能(本能ID)) continue;
        if (!本能集.有(本能ID)) continue;
        auto* 方法首节点 = 方法集.查找或创建_本能方法首节点(
            本能ID,
            now,
            "自我线程类::私有_确保显式阶段任务/阶段方法/" + 阶段标签);
        if (!方法首节点) continue;
        方法集.初始化方法虚拟存在信息(
            方法首节点,
            now,
            "自我线程类::私有_确保显式阶段任务/初始化阶段方法/" + 阶段标签);
        if (std::find(阶段方法列表.begin(), 阶段方法列表.end(), 方法首节点) == 阶段方法列表.end()) {
            阶段方法列表.push_back(方法首节点);
        }
    }

    auto* 阶段步骤节点 = 私有_查找首个步骤节点(阶段任务节点);
    if (!阶段步骤节点 && !阶段方法列表.empty()) {
        任务类::结构_创建步骤参数 阶段步骤参数{};
        阶段步骤参数.名称 = 阶段步骤名称;
        阶段步骤参数.类型 = 阶段步骤类型;
        阶段步骤参数.所属任务头结点 = 阶段任务节点;
        阶段步骤参数.步骤场景 = 阶段场景;
        阶段步骤参数.步骤序号 = 0;
        阶段步骤参数.初始状态 = 枚举_任务状态::就绪;
        阶段步骤参数.允许切换方法 = 阶段方法列表.size() > 1;
        阶段步骤参数.可用方法首节点列表 = 阶段方法列表;
        阶段步骤参数.当前选中方法首节点 = 阶段方法列表.front();
        阶段步骤参数.局部优先级偏移 = 局部优先级偏移;
        阶段步骤节点 = 任务类::创建步骤节点(
            阶段任务节点,
            阶段步骤参数,
            "自我线程类::私有_确保显式阶段任务/创建阶段步骤/" + 阶段标签);
    }

    auto* 阶段步骤信息 = (阶段步骤节点 && 阶段步骤节点->主信息)
        ? dynamic_cast<任务步骤节点信息*>(阶段步骤节点->主信息)
        : nullptr;
    if (阶段步骤信息 && 阶段步骤信息->可用方法首节点列表.empty() && !阶段方法列表.empty()) {
        阶段步骤信息->可用方法首节点列表 = 阶段方法列表;
    }
    if (阶段步骤信息 && !阶段步骤信息->当前选中方法首节点 && !阶段步骤信息->可用方法首节点列表.empty()) {
        阶段步骤信息->当前选中方法首节点 = 阶段步骤信息->可用方法首节点列表.front();
    }

    需求状态管理器.刷新需求及父链状态(
        阶段需求,
        "自我线程类::私有_确保显式阶段任务/刷新需求状态/" + 阶段标签);
    const bool 阶段已满足 = 需求状态管理器.查询需求状态(阶段需求) == 枚举_需求运行状态::已满足;
    if (新建阶段任务 || 新建阶段需求) {
        日志::运行f(
            "[自我线程] 显式阶段就绪: 标签={}, 新建任务={}, 新建需求={}, 任务={}, 需求={}, 已满足={}",
            阶段标签,
            新建阶段任务 ? 1 : 0,
            新建阶段需求 ? 1 : 0,
            (void*)阶段任务节点,
            (void*)阶段需求,
            阶段已满足 ? 1 : 0);
    }
    日志::运行f(
        "[自我线程] 显式阶段进入复位: 标签={}, 任务={}, 需求={}, 已满足={}",
        阶段标签,
        (void*)阶段任务节点,
        (void*)阶段需求,
        阶段已满足 ? 1 : 0);

    auto 步骤已终结 = [&](任务步骤节点信息* 步骤信息) noexcept {
        if (!步骤信息) return true;
        return 步骤信息->状态 == 枚举_任务状态::完成
            || 步骤信息->状态 == 枚举_任务状态::取消
            || 步骤信息->状态 == 枚举_任务状态::失败
            || 步骤信息->状态 == 枚举_任务状态::无法执行;
    };

    if (阶段已满足) {
        if (阶段任务节点->主信息 && 阶段任务节点->主信息->状态 != 枚举_任务状态::完成) {
            任务类::设置任务状态(
                阶段任务节点,
                枚举_任务状态::完成,
                "自我线程类::私有_确保显式阶段任务/阶段满足转完成/" + 阶段标签);
        }
        if (阶段任务节点->主信息) {
            (void)任务类::设置任务当前步骤(
                阶段任务节点->主信息,
                nullptr,
                now,
                "自我线程类::私有_确保显式阶段任务/清空当前步骤/" + 阶段标签);
        }
    }
    else {
        if (阶段步骤节点 && 步骤已终结(阶段步骤信息)) {
            任务类::设置任务状态(
                阶段步骤节点,
                枚举_任务状态::就绪,
                "自我线程类::私有_确保显式阶段任务/阶段步骤复位/" + 阶段标签);
        }
        if (阶段任务节点->主信息) {
            (void)任务类::设置任务当前步骤(
                阶段任务节点->主信息,
                阶段步骤节点,
                now,
                "自我线程类::私有_确保显式阶段任务/设置当前步骤/" + 阶段标签);
        }

        const auto 当前任务状态 = 阶段任务节点->主信息 ? 阶段任务节点->主信息->状态 : 枚举_任务状态::未定义;
        if (当前任务状态 == 枚举_任务状态::完成
            || 当前任务状态 == 枚举_任务状态::取消
            || 当前任务状态 == 枚举_任务状态::失败
            || 当前任务状态 == 枚举_任务状态::无法执行
            || 当前任务状态 == 枚举_任务状态::待重筹办
            || 当前任务状态 == 枚举_任务状态::挂起
            || 当前任务状态 == 枚举_任务状态::未启动
            || 当前任务状态 == 枚举_任务状态::未定义) {
            任务类::设置任务状态(
                阶段任务节点,
                枚举_任务状态::就绪,
                "自我线程类::私有_确保显式阶段任务/阶段任务复位/" + 阶段标签);
        }
    }
    auto* 阶段任务头信息 = (阶段任务节点 && 阶段任务节点->主信息)
        ? dynamic_cast<任务头结点信息*>(阶段任务节点->主信息)
        : nullptr;
    const int 阶段任务状态 = 阶段任务节点 && 阶段任务节点->主信息
        ? static_cast<int>(阶段任务节点->主信息->状态)
        : -1;
    auto* 阶段当前步骤 = 阶段任务头信息 ? 阶段任务头信息->当前步骤节点 : nullptr;
    日志::运行f(
        "[自我线程] 显式阶段完成复位: 标签={}, 任务状态={}, 当前步骤={}",
        阶段标签,
        阶段任务状态,
        (void*)阶段当前步骤);

    if (调度器已初始化_.load()) {
        日志::运行f(
            "[自我线程] 显式阶段准备重评估: 标签={}, 任务={}",
            阶段标签,
            (void*)阶段任务节点);
        调度器_.请求重评估任务(阶段任务节点);
        调度器_.请求重评估任务(根任务节点);
        调度器_.处理待重评估队列(8);
        日志::运行f(
            "[自我线程] 显式阶段完成重评估: 标签={}, 任务={}",
            阶段标签,
            (void*)阶段任务节点);
    }

    return 阶段任务节点;
}

inline bool 自我线程类::私有_分解安全根任务(任务节点类* 根任务节点, 需求节点类* 根需求节点, 时间戳 now) {
    (void)now;
    auto* 自检任务 = 私有_确保显式阶段任务(
        根任务节点,
        根需求节点,
        "安全阶段_自检",
        "安全阶段需求_核心状态已读取",
        "安全阶段_核心状态已读取",
        "安全阶段任务_核心状态已读取",
        "安全阶段步骤_读取核心状态",
        "回执_成功",
        1,
        { 枚举_本能动作ID::自我_读取核心状态 },
        6000);

    auto* 自检需求 = 私有_查找直属子需求_按类型(
        根需求节点,
        语素集.添加词性词("安全阶段需求_核心状态已读取", "名词"));
    if (!自检任务 || !自检需求) return false;
    if (需求状态管理器.查询需求状态(自检需求) != 枚举_需求运行状态::已满足) {
        return true;
    }

    const bool 需要保守 = 私有_读取任务事实I64(自检任务, "待机状态").value_or(0) > 0
        || 私有_读取任务事实I64(自检任务, "时序反向步长").value_or(0) > 0
        || 私有_读取任务事实I64(自检任务, "服务时序衰减步长").value_or(0) > 0;
    if (!需要保守) return false;

    auto* 保守任务 = 私有_确保显式阶段任务(
        根任务节点,
        根需求节点,
        "安全阶段_保守态",
        "安全阶段需求_保守态已建立",
        "安全阶段_保守态已建立",
        "安全阶段任务_保守态已建立",
        "安全阶段步骤_进入保守态",
        "保守态_请求",
        1,
        { 枚举_本能动作ID::自我_停止当前试探并进入保守态 },
        5000);

    auto* 保守需求 = 私有_查找直属子需求_按类型(
        根需求节点,
        语素集.添加词性词("安全阶段需求_保守态已建立", "名词"));
    if (!保守任务 || !保守需求) return false;
    return 需求状态管理器.查询需求状态(保守需求) != 枚举_需求运行状态::已满足;
}

inline bool 自我线程类::私有_分解服务根任务(任务节点类* 根任务节点, 需求节点类* 根需求节点, 时间戳 now) {
    (void)now;
    auto* 读取输入任务 = 私有_确保显式阶段任务(
        根任务节点,
        根需求节点,
        "服务阶段_读取输入",
        "服务阶段需求_人类输入已读取",
        "服务阶段_人类输入已读取",
        "服务阶段任务_人类输入已读取",
        "服务阶段步骤_读取最新输入",
        "人类输入_已读取",
        1,
        { 枚举_本能动作ID::自我_读取最新人类输入 },
        6000);

    auto* 读取输入需求 = 私有_查找直属子需求_按类型(
        根需求节点,
        语素集.添加词性词("服务阶段需求_人类输入已读取", "名词"));
    if (!读取输入任务 || !读取输入需求) return false;
    if (需求状态管理器.查询需求状态(读取输入需求) != 枚举_需求运行状态::已满足) {
        return true;
    }

    const bool 有输入 = 私有_读取任务事实I64(读取输入任务, "人类输入_存在").value_or(0) > 0;
    if (!有输入) return false;

    const bool 输入对象有效 = 私有_读取任务事实指针(读取输入任务, "人类输入_对象指针").value_or(0) != 0;
    if (输入对象有效) {
        auto* 确认任务 = 私有_确保显式阶段任务(
            根任务节点,
            根需求节点,
            "服务阶段_确认收到",
            "服务阶段需求_已确认收到",
            "服务阶段_已确认收到",
            "服务阶段任务_已确认收到",
            "服务阶段步骤_确认收到",
            "服务动作_已确认收到",
            1,
            { 枚举_本能动作ID::自我_确认收到 },
            5000);
        auto* 确认需求 = 私有_查找直属子需求_按类型(
            根需求节点,
            语素集.添加词性词("服务阶段需求_已确认收到", "名词"));
        if (!确认任务 || !确认需求) return false;
        return 需求状态管理器.查询需求状态(确认需求) != 枚举_需求运行状态::已满足;
    }

    auto* 补充任务 = 私有_确保显式阶段任务(
        根任务节点,
        根需求节点,
        "服务阶段_请求补充",
        "服务阶段需求_已请求补充信息",
        "服务阶段_已请求补充信息",
        "服务阶段任务_已请求补充信息",
        "服务阶段步骤_请求补充信息",
        "服务动作_已请求补充信息",
        1,
        { 枚举_本能动作ID::自我_请求补充信息 },
        5000);
    auto* 补充需求 = 私有_查找直属子需求_按类型(
        根需求节点,
        语素集.添加词性词("服务阶段需求_已请求补充信息", "名词"));
    if (!补充任务 || !补充需求) return false;
    return 需求状态管理器.查询需求状态(补充需求) != 枚举_需求运行状态::已满足;
}

inline void 自我线程类::私有_扫描任务树并尝试入队(任务节点类* 根任务节点, const std::string& 标签) {
    if (!根任务节点) return;

    std::vector<任务节点类*> 栈;
    std::unordered_set<任务节点类*> 已入栈;
    auto 压栈 = [&](任务节点类* 节点) {
        if (!节点) return;
        if (!已入栈.insert(节点).second) return;
        栈.push_back(节点);
    };
    压栈(根任务节点);

    while (!栈.empty()) {
        auto* 当前 = 栈.back();
        栈.pop_back();
        if (!当前 || !当前->主信息) continue;

        if (auto* 当前头信息 = dynamic_cast<任务头结点信息*>(当前->主信息)) {
            for (auto* 直属子任务头 : 当前头信息->直属子任务头结点列表) {
                压栈(直属子任务头);
            }
        }

        if (dynamic_cast<任务头结点信息*>(当前->主信息) && 当前 != 学习任务节点_) {
            私有_确保任务已引导筹办(当前, 标签);
            if (!私有_任务存在活跃直属子任务(当前)) {
                私有_尝试入队动作任务(当前, 标签);
            }
        }

        auto* 起 = 当前->子;
        auto* it = 起;
        while (it) {
            压栈(it);
            it = static_cast<任务节点类*>(it->下);
            if (it == 起) break;
        }
    }
}

inline void 自我线程类::私有_唤醒等待学习的任务(方法节点类* 方法首节点, const std::string& 标签) {
    if (!方法首节点) return;

    auto 唤醒一棵任务树 = [&](任务节点类* 根任务节点) {
        if (!根任务节点) return 0;

        int 唤醒数 = 0;
        std::vector<任务节点类*> 栈;
        栈.push_back(根任务节点);
        while (!栈.empty()) {
            auto* 当前 = 栈.back();
            栈.pop_back();
            if (!当前 || !当前->主信息) continue;

            if (auto* 头信息 = dynamic_cast<任务头结点信息*>(当前->主信息)) {
                if (头信息->等待学习唤醒 && 头信息->等待学习方法首节点 == 方法首节点) {
                    头信息->等待学习唤醒 = false;
                    头信息->等待学习方法首节点 = nullptr;
                    if (当前->主信息->状态 == 枚举_任务状态::待重筹办
                        || 当前->主信息->状态 == 枚举_任务状态::无法执行
                        || 当前->主信息->状态 == 枚举_任务状态::挂起) {
                        任务类::设置任务状态(当前, 枚举_任务状态::就绪, "自我线程类::私有_唤醒等待学习的任务/" + 标签);
                    }
                    ++唤醒数;
                }
            }

            auto* 起 = 当前->子;
            auto* it = 起;
            while (it) {
                栈.push_back(it);
                it = static_cast<任务节点类*>(it->下);
                if (it == 起) break;
            }
        }
        return 唤醒数;
    };

    const int 安全唤醒数 = 唤醒一棵任务树(安全任务节点_);
    const int 服务唤醒数 = 唤醒一棵任务树(服务任务节点_);
    const int 总唤醒数 = 安全唤醒数 + 服务唤醒数;
    if (总唤醒数 > 0) {
        日志::运行f("[自我线程] 学习后精确唤醒任务: 标签={}, 方法={}, 唤醒数={}",
            标签,
            (void*)方法首节点,
            总唤醒数);
    }
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
    初始需求任务已就绪_ = false;
    调度器已初始化_.store(false);

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

    if (auto* 安全头信息 = 安全任务节点_ ? dynamic_cast<任务头结点信息*>(安全任务节点_->主信息) : nullptr) {
        安全头信息->是否真根任务 = true;
    }
    if (auto* 服务头信息 = 服务任务节点_ ? dynamic_cast<任务头结点信息*>(服务任务节点_->主信息) : nullptr) {
        服务头信息->是否真根任务 = true;
    }

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
        info->是否常驻任务 = true;
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

    需求状态管理器.初始化_扫描需求链("自我线程类::初始化_初始需求与任务/初始化需求状态");
    调度器_.设置需求状态查询回调([](需求节点类* 需求) {
        return 需求状态管理器.查询需求状态(需求);
    });
    调度器_.设置任务状态写回回调([](任务节点类* 任务, 枚举_任务状态 新状态, const std::string& 调用点) {
        任务类::设置任务状态(任务, 新状态, 调用点);
    });
    调度器_.初始化_扫描需求链("自我线程类::初始化_初始需求与任务/初始化调度器");
    调度器已初始化_.store(true);

    学习任务节点_->主信息->名称 = 学习任务名;
    学习任务节点_->主信息->类型 = 学习任务类型;
    学习任务节点_->主信息->创建时间 = now;
    if (auto* 学习头信息 = dynamic_cast<任务头结点信息*>(学习任务节点_->主信息)) {
        学习头信息->是否常驻任务 = true;
    }
    学习任务节点_->主信息->调度优先级 = (待学习方法数量_ > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
    if (启用学习线程_) {
        任务类::设置任务状态(学习任务节点_, 枚举_任务状态::排队中, "自我线程类::初始化_初始需求与任务/学习任务排队");
    }
    else {
        任务类::设置任务状态(学习任务节点_, 枚举_任务状态::挂起, "自我线程类::初始化_初始需求与任务/学习任务挂起");
        日志::运行("[自我线程] 当前阶段停用学习线程；学习任务仅保留账本，不进入队列");
    }
    学习任务节点_->主信息->任务树类型 = 枚举_任务树类型::叶子任务;
    学习任务节点_->主信息->场景 = 世界树.虚拟世界;

    if (启用学习线程_) {
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
    }
    else {
        根任务筹办器_.设置尝试学习任务(nullptr);
    }
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

inline void 自我线程类::私有_确保任务已引导筹办(任务节点类* 任务节点, const std::string& 标签) {
    auto* 头信息 = (任务节点 && 任务节点->主信息) ? dynamic_cast<任务头结点信息*>(任务节点->主信息) : nullptr;
    if (!头信息 || !头信息->需求 || !头信息->需求->主信息) return;
    if (!头信息->是否真根任务) return;

    const auto 当前状态 = 任务节点->主信息->状态;
    if (当前状态 != 枚举_任务状态::待重筹办
        && 当前状态 != 枚举_任务状态::就绪
        && 当前状态 != 枚举_任务状态::筹办中) {
        return;
    }

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    bool 阶段子任务已接管 = false;
    if (任务节点 == 安全任务节点_ || 头信息->需求 == 安全需求节点_) {
        阶段子任务已接管 = 私有_分解安全根任务(任务节点, 头信息->需求, now);
    }
    else if (任务节点 == 服务任务节点_ || 头信息->需求 == 服务需求节点_) {
        阶段子任务已接管 = 私有_分解服务根任务(任务节点, 头信息->需求, now);
    }
    if (阶段子任务已接管 || 私有_任务存在活跃直属子任务(任务节点)) {
        return;
    }

    auto 是否终结 = [](枚举_任务状态 状态) noexcept {
        return 状态 == 枚举_任务状态::完成
            || 状态 == 枚举_任务状态::取消
            || 状态 == 枚举_任务状态::失败
            || 状态 == 枚举_任务状态::无法执行;
    };

    auto* 当前步骤信息 = (头信息->当前步骤节点 && 头信息->当前步骤节点->主信息)
        ? dynamic_cast<任务步骤节点信息*>(头信息->当前步骤节点->主信息)
        : nullptr;
    bool 已有可继续步骤 = 当前步骤信息 && !是否终结(当前步骤信息->状态);
    std::int32_t 已有步骤数 = 0;
    if (任务节点->子) {
        auto* first = static_cast<任务节点类*>(任务节点->子);
        auto* cur = first;
        do {
            if (cur && cur->主信息) {
                auto* 步骤信息 = dynamic_cast<任务步骤节点信息*>(cur->主信息);
                if (!步骤信息) {
                    cur = cur ? static_cast<任务节点类*>(cur->下) : nullptr;
                    continue;
                }
                ++已有步骤数;
                if (!是否终结(步骤信息->状态)) {
                    已有可继续步骤 = true;
                }
            }
            cur = cur ? static_cast<任务节点类*>(cur->下) : nullptr;
        } while (cur && cur != first);
    }
    if (已有可继续步骤) return;

    if (当前步骤信息 && 是否终结(当前步骤信息->状态)) {
        (void)任务类::设置任务当前步骤(
            任务节点->主信息,
            nullptr,
            now,
            "自我线程类::私有_确保任务已引导筹办/清理已终结当前步骤/" + 标签);
    }

    auto* 筹办场景 = 头信息->场景 ? 头信息->场景 : (世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界);
    if (!任务类::进入筹办阶段(任务节点, 筹办场景, "自我线程类::私有_确保任务已引导筹办/" + 标签)) {
        日志::运行(
            "[自我线程][不变量][违反] 真根任务无法进入筹办阶段: 标签=" + 标签 +
            ", 任务=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务节点)));
        return;
    }

    auto* step = 根任务筹办器_.仅筹办一步(任务节点);
    if (step) {
        if (任务节点->主信息 && 任务节点->主信息->状态 == 枚举_任务状态::筹办中) {
            任务类::设置任务状态(任务节点, 枚举_任务状态::就绪, "自我线程类::私有_确保任务已引导筹办/筹办完成待执行/" + 标签);
        }
        日志::运行(
            "[自我线程] 真根任务已引导进入筹办并生成首批步骤: 标签=" + 标签 +
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

        auto* 试探步骤 = 任务类::创建步骤节点(任务节点, 试探步骤参数, "自我线程类::私有_确保任务已引导筹办/创建最小试探步骤");
        if (试探步骤) {
            (void)任务类::设置任务当前步骤(任务节点->主信息, 试探步骤, now, "自我线程类::私有_确保任务已引导筹办/设置最小试探步骤");
            任务类::设置任务状态(任务节点, 枚举_任务状态::就绪, "自我线程类::私有_确保任务已引导筹办/最小试探待执行/" + 标签);
            日志::运行f(
                "[自我线程] 真根任务引导筹办后生成最小试探步骤: 标签={}, 任务={}, 步骤={}, 方法数={}",
                标签,
                (void*)任务节点,
                (void*)试探步骤,
                最小试探方法列表.size());
            return;
        }
    }

    (void)任务类::设置任务当前步骤(
        任务节点->主信息,
        nullptr,
        now,
        "自我线程类::私有_确保任务已引导筹办/清空当前步骤并待重筹办/" + 标签);
    任务类::设置任务状态(任务节点, 枚举_任务状态::待重筹办, "自我线程类::私有_确保任务已引导筹办/暂无方法保持待重筹办/" + 标签);
    日志::运行(
        "[自我线程] 真根任务本轮筹办未生成可执行步骤，将保持待重筹办: 标签=" + 标签 +
        ", 任务=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务节点)));
}

inline void 自我线程类::私有_刷新根任务就绪状态() {
    auto 刷新一个根任务 = [&](任务节点类* 任务节点, 需求节点类* 需求节点, const std::string& 标签) {
        if (!任务节点 || !任务节点->主信息 || !需求节点) return;

        需求状态管理器.刷新需求及父链状态(需求节点, "自我线程类::私有_刷新根任务就绪状态/" + 标签);
        if (调度器已初始化_.load()) {
            调度器_.请求重评估任务(任务节点);
            调度器_.处理待重评估队列(4);
        }
    };

    刷新一个根任务(安全任务节点_, 安全需求节点_, "安全");
    刷新一个根任务(服务任务节点_, 服务需求节点_, "服务");
}


bool 自我线程类::私有_任务目标命中(const 任务信息基类& t) const
{
    auto* 头信息 = dynamic_cast<const 任务头结点信息*>(&t);
    if (!头信息 || !头信息->需求 || 头信息->是否真根任务) return false;
    return 需求状态管理器.查询需求状态(头信息->需求) == 枚举_需求运行状态::已满足;
}

inline void 自我线程类::启动工作线程() {
    // 0~2 工作者线程：动作执行 / 学习采样
    if (启用动作线程_) {
        if (!动作线程_.joinable()) {
            动作线程_ = std::thread([this] {
                if (!通用函数模块::安全执行_记录异常("自我线程类::动作线程", [this] { 动作线程入口(); })) {
                    置故障("动作线程异常");
                    请求退出();
                }
            });
            日志::运行("[自我线程] 动作线程启动");
        }
    }
    if (启用学习线程_) {
        if (!学习线程_.joinable()) {
            学习线程_ = std::thread([this] {
                if (!通用函数模块::安全执行_记录异常("自我线程类::学习线程", [this] { 学习线程入口(); })) {
                    置故障("学习线程异常");
                    请求退出();
                }
            });
            日志::运行("[自我线程] 学习线程启动");
        }
    }
    else {
        日志::运行("[自我线程] 当前阶段跳过学习线程启动");
    }
}

inline void 自我线程类::停止工作线程() {
    // stop_ 由 请求退出 设置；这里只负责唤醒等待
    执行队列cv_.notify_all();
    学习队列cv_.notify_all();
}

inline void 自我线程类::心跳调度_启动就绪任务() {
    if (调度器已初始化_.load()) {
        调度器_.处理待重评估队列();
    }

    私有_扫描任务树并尝试入队(安全任务节点_, "安全");
    私有_扫描任务树并尝试入队(服务任务节点_, "服务");

    if (!启用学习线程_) return;
    if (!学习任务节点_ || !学习任务节点_->主信息) return;

    auto* info = 学习任务节点_->主信息;
    if (info->状态 == 枚举_任务状态::排队中 || info->状态 == 枚举_任务状态::执行中 || info->状态 == 枚举_任务状态::等待中) return;

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    info->调度优先级 = (待学习方法数量_ > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
    info->创建时间 = now;
    任务类::设置任务状态(学习任务节点_, 枚举_任务状态::排队中, "自我线程类::心跳调度_启动就绪任务/学习任务排队");

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
        日志::运行f(
            "[自我线程] 动作线程开始推进: 节点={}, 优先级={}, 原状态={}",
            (void*)item.节点,
            static_cast<long long>(item.优先级),
            static_cast<int>(info->状态));

        结构_任务返回消息 返回消息{};
        const bool 有进展 = 根任务筹办器_.推进一步(item.节点, &返回消息);
        if (调度器已初始化_.load() && 返回消息.类型 != 枚举_任务返回消息类型::未定义) {
            枚举_需求运行状态 旧需求状态 = 枚举_需求运行状态::未知;
            if (返回消息.对应需求) {
                旧需求状态 = 需求状态管理器.查询需求状态(返回消息.对应需求);
            }

            需求状态管理器.处理任务返回消息(返回消息, "自我线程类::动作线程入口/刷新需求状态");

            枚举_需求运行状态 新需求状态 = 枚举_需求运行状态::未知;
            if (返回消息.对应需求) {
                新需求状态 = 需求状态管理器.查询需求状态(返回消息.对应需求);

                (void)私有_按叶子需求结算安全服务值(
                    返回消息,
                    旧需求状态,
                    新需求状态,
                    "自我线程类::动作线程入口/叶子需求结算");
            }

            调度器_.提交任务返回消息(返回消息);
            调度器_.处理待重评估队列();

            if (返回消息.类型 == 枚举_任务返回消息类型::生成子任务
                && 返回消息.相关子任务
                && 返回消息.相关子任务->主信息) {
                日志::运行f(
                    "[自我线程] 生成子任务直接入队: 父任务={}, 子任务={}",
                    (void*)返回消息.来源任务,
                    (void*)返回消息.相关子任务);
                私有_尝试入队动作任务(返回消息.相关子任务, "子任务");
            }
        }

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
        任务类::设置任务状态(item.节点, 枚举_任务状态::执行中, "自我线程类::学习线程入口/置执行中");
        ++正在执行线程数_;
        日志::运行f("[自我线程] 学习线程取到任务: 节点={}, 是否学习常驻任务={}", (void*)item.节点, item.节点 == 学习任务节点_ ? 1 : 0);

        const 时间戳 now = 结构体_时间戳::当前_微秒();
        if (item.节点 == 学习任务节点_) {
            const auto before = 待学习方法数量_;
            日志::运行f("[自我线程] 学习线程开始尝试学习一步: 节点={}, 待学习方法数量={}", (void*)item.节点, before);
            const auto 学习结果 = 私有_尝试学习一步(now);
            const bool progressed = 学习结果.有进展;
            const auto after = 待学习方法数量_;

            if (学习结果.需要唤醒等待任务 && 学习结果.目标方法首节点) {
                私有_唤醒等待学习的任务(学习结果.目标方法首节点, "学习线程");
            }

            info->调度优先级 = (after > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
            info->创建时间 = now;
            任务类::设置任务状态(item.节点, 枚举_任务状态::就绪, "自我线程类::学习线程入口/学习任务回到就绪");

            日志::运行f("[自我线程] 尝试学习任务执行一次: 节点={}, 有进展={}, 待学习方法数量 {} -> {}, 优先级={}",
                (void*)item.节点,
                progressed,
                before,
                after,
                info->调度优先级);
        }
        else {
            任务类::设置任务状态(item.节点, 枚举_任务状态::完成, "自我线程类::学习线程入口/非驻留学习任务完成");
            日志::运行f("[自我线程] 学习线程执行了非驻留学习任务: 节点={}", (void*)item.节点);
        }

        if (正在执行线程数_.load() > 0) --正在执行线程数_;
    }
}






























