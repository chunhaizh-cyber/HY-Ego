module;
#include <afx.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <fstream>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>

export module 自我模块;

import 基础数据类型模块;
import 主信息定义模块;
import 数据仓库模块;
import 语素环境模块;
import 特征类型定义模块;
import 状态模块;
import 二次特征模块;

import 世界树环境模块;
import 需求环境模块;
import 方法模块;
import 方法环境模块;
import 任务模块;
import 度量模板注册表模块;

// ============================================================
// 自我模块（Self）
// ------------------------------------------------------------
// 目标：
//  1) 只保留 1 个 Self 线程：调度/决策/聚合
//  2) 配 0~2 个 Worker 线程：执行/学习采样等重活
//  3) 以“需求 -> 召回方法 -> 执行 -> 反馈”形成最小闭环
//  4) 不做兼容：旧的多余字段/接口不再支持
// ============================================================

export class 自我类 {
public:
    struct 结构_自我配置 {
        std::uint32_t 工作者数量 = 1;          // 0~2
        时间戳 Tick间隔_微秒 = 2000;           // Self 循环节拍
        时间戳 执行超时_微秒 = 2000000;        // 单步执行超时（2s）
        std::uint64_t 调试_最大循环次数 = 0;   // 0 表示不限（调试期可设）
        std::string 退出事件文件 = "self_exit_event.txt";
    };

    // 三条常驻根任务的“权重货币”（先用标量，后续可全部迁移到虚拟存在特征）
    struct 结构_根任务权重 {
        std::int64_t 安全 = 10000;
        std::int64_t 服务 = 10000;
        std::int64_t 学习 = 5000;
    };

public:
    explicit 自我类(const 结构_自我配置& cfg = {})
        : cfg_(cfg)
    {
        if (cfg_.工作者数量 > 2) cfg_.工作者数量 = 2;
        if (cfg_.Tick间隔_微秒 == 0) cfg_.Tick间隔_微秒 = 1000;
    }

    ~自我类() {
        请求停止();
        等待停止();
    }

    自我类(const 自我类&) = delete;
    自我类& operator=(const 自我类&) = delete;

    // ==========================================================
    // 生命周期
    // ==========================================================
    
    void 初始化自我(const std::string& 调用点 = "自我类::初始化") {
        (void)调用点;
        世界树.初始化();
        // 方法/需求是环境单例：需求集、方法集
        // 默认模板：建议在世界树初始化后调用一次
        try {
            度量模板注册表类::实例().注册默认模板();
        }
        catch (...) {
            // 允许重复注册或失败不致命
        }
        重算时序步长_按服务值_();
        刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::初始化");
    }

    void 启动() {
        if (running_.exchange(true)) return;
        stop_.store(false);

        // 启动 worker
        worker_.启动(cfg_.工作者数量);

        // 启动 self
        selfThread_ = std::thread([this] { this->SelfLoop_(); });
    }

    void 请求停止() {
        stop_.store(true);
        worker_.请求停止();
    }

    void 等待停止() {
        if (selfThread_.joinable()) selfThread_.join();
        worker_.等待停止();
        running_.store(false);
    }

    bool 是否运行中() const noexcept { return running_.load(); }

    // ==========================================================
    // 状态（安全/服务值）
    // ==========================================================

    U64 安全值() const noexcept { return safety_.load(); }
    U64 服务值() const noexcept { return service_.load(); }

    void 设置安全值(U64 v) noexcept {
        safety_.store(v);
        刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::设置安全值");
    }
    void 设置服务值(U64 v) noexcept {
        service_.store(v);
        重算时序步长_按服务值_();
        刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::设置服务值");
    }

    bool 是否已消亡() const noexcept { return safety_.load() == 0; }

    // 供调度线程调用的统一数值更新入口。
    void 安全值增加(U64 delta) noexcept { 安全值_上升_(delta); }
    void 安全值减少(U64 delta) noexcept { 安全值_下降_(delta); }
    void 服务值增加(U64 delta) noexcept { 服务值_上升_(delta); }
    void 服务值减少(U64 delta) noexcept { 服务值_下降_(delta); }
    U64 时序正向步长() const noexcept { return temporal_forward_step_.load(); }
    U64 时序反向步长() const noexcept { return temporal_backward_step_.load(); }
    bool 是否待机状态() const noexcept { return standby_mode_.load(); }

    // 默认任务结果策略：成功偏服务增长，失败同时惩罚服务与安全。
    void 应用任务结果_默认策略(bool 成功) noexcept {
        if (成功) {
            服务值_上升_(50);
            return;
        }
        服务值_下降_(10);
        安全值_下降_(5);
    }


    // 时序维护：向中位回归 + 内部状态压力 + 服务值耦合步长。
    // 规则：
    // 1) 默认时序步长为 1（正向/反向），并随服务值实时重算；
    // 2) 服务值越高：正向步长越大、反向步长越小；
    // 3) 服务值为 0：安全值置为最小非零并进入待机，不触发消亡。
    void 按时序规则更新安全值(
        U64 待完成任务数,
        U64 待学习任务数,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::按时序规则更新安全值") noexcept
    {
        重算时序步长_按服务值_();

        const U64 srv = service_.load();
        U64 s = safety_.load();

        if (srv == 0) {
            standby_mode_.store(true);
            constexpr U64 最小非零安全值 = 1;
            if (s != 最小非零安全值) {
                safety_.store(最小非零安全值);
                刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, now, 调用点 + "/服务为零待机");
            }
            return;
        }

        standby_mode_.store(false);

        const U64 中位值 = (U64_MAX / 2);
        const U64 正向 = temporal_forward_step_.load();
        const U64 反向 = temporal_backward_step_.load();

        if (s < 中位值) {
            s = sat_add(s, 正向);
        }
        else if (s > 中位值) {
            s = sat_sub(s, 反向);
        }

        // 内部状态压力：当前先使用默认步长 1（有积压即扣减）。
        const U64 总待办 = sat_add(待完成任务数, 待学习任务数);
        const U64 内部压力步长 = (总待办 > 0) ? 1 : 0;
        if (内部压力步长 > 0) {
            s = sat_sub(s, 内部压力步长);
        }

        if (s != safety_.load()) {
            safety_.store(s);
            刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, now, 调用点 + "/时序更新");
        }
    }

    结构_根任务权重& 根任务权重() noexcept { return roots_; }
    const 结构_根任务权重& 根任务权重() const noexcept { return roots_; }

    // ==========================================================
    // 注入：世界树与差异度（可选）
    // ==========================================================
    //世界树类& 世界树() noexcept { return 世界树_; }
   // const 世界树类& 世界树() const noexcept { return 世界树_; }

private:
    static constexpr I64 自我尝试学习状态_空闲 = 0;
    static constexpr I64 自我尝试学习状态_准备中 = 1;
    static constexpr I64 自我尝试学习状态_已生成 = 2;
    static constexpr I64 自我尝试学习状态_失败 = 3;

    struct 结构_尝试学习参数队列项 {
        方法节点类* 方法首节点 = nullptr;
        场景节点类* 参数场景 = nullptr;
        需求节点类* 来源需求 = nullptr;
        时间戳 入队时间 = 0;
    };
    static bool 快照相等_(const std::optional<特征快照值>& oldValue, const 特征快照值& newValue) noexcept
    {
        return oldValue.has_value() && *oldValue == newValue;
    }

    场景节点类* 确保自我内部世界_(时间戳 now, const std::string& 调用点)
    {
        特征类型定义类::初始化特征类型定义模块_依赖语素();
        if (!世界树.虚拟世界) {
            const 词性节点类* 名称 = 特征类型定义类::名_自我内部世界
                ? 特征类型定义类::名_自我内部世界
                : 语素集.添加词性词("自我内部世界", "名词");
            世界树.虚拟世界 = 世界树.取或创建子场景_按名称(nullptr, 名称, now, 调用点 + "/内部世界");
        }
        return 世界树.虚拟世界;
    }

    存在节点类* 确保自我存在_(时间戳 now, const std::string& 调用点)
    {
        特征类型定义类::初始化特征类型定义模块_依赖语素();
        if (世界树.自我指针) return 世界树.自我指针;
        auto* 内部世界 = 确保自我内部世界_(now, 调用点);
        if (!内部世界) return nullptr;

        const 词性节点类* 自我类型 = 特征类型定义类::型_自我
            ? 特征类型定义类::型_自我
            : 语素集.添加词性词("自我类型", "名词");
        auto* self = 世界树.取或创建子存在_按类型(内部世界, 自我类型, now, 调用点 + "/自我存在");
        if (!self) return nullptr;

        if (特征类型定义类::名_自我) {
            (void)世界树.写入名称(self, 特征类型定义类::名_自我, now, 调用点 + "/自我名称");
        }
        世界树.自我指针 = self;
        return self;
    }

    状态节点类* 记录自我特征_(const 词性节点类* 特征类型, const 特征快照值& 值, 时间戳 now, const std::string& 调用点)
    {
        if (!特征类型) return nullptr;
        auto* 内部世界 = 确保自我内部世界_(now, 调用点);
        auto* 自我存在 = 确保自我存在_(now, 调用点);
        if (!内部世界 || !自我存在) return nullptr;

        const auto 旧值 = 世界树.读取特征快照(自我存在, 特征类型, 调用点);
        const bool 值发生变化 = !快照相等_(旧值, 值);
        const auto 事件 = 旧值.has_value() ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建;

        if (auto* pi = std::get_if<I64>(&值)) {
            (void)世界树.写入特征_I64(自我存在, 特征类型, *pi, {}, 调用点);
        }
        else if (auto* ph = std::get_if<指针句柄>(&值)) {
            (void)世界树.写入特征_指针(自我存在, 特征类型, ph->指针, {}, 调用点);
        }
        else {
            return nullptr;
        }

        auto* feat = 世界树.确保特征(自我存在, 特征类型, 特征类型, 调用点);
        return 状态集.记录内部特征状态(
            内部世界,
            自我存在,
            feat,
            值,
            事件,
            值发生变化,
            now,
            [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
            },
            调用点);
    }

    I64 判定外设可用性_() const
    {
        特征类型定义类::初始化特征类型定义模块_依赖语素();
        auto* root = 世界树.世界根();
        if (!root) return 0;
        const 词性节点类* 运行状态 = 语素集.添加词性词("运行状态", "名词");
        for (auto* e : 世界树.获取子存在(root, "自我类::判定外设可用性")) {
            if (!e || !e->主信息) continue;
            auto* emi = dynamic_cast<存在节点主信息类*>(e->主信息);
            if (!emi || emi->类型 != 特征类型定义类::型_外设摄像机) continue;
            const auto snap = 世界树.读取特征快照(e, 运行状态, "自我类::判定外设可用性");
            if (!snap.has_value()) return 1;
            if (auto* pv = std::get_if<I64>(&*snap)) return (*pv != 0) ? 1 : 0;
            return 1;
        }
        return 0;
    }

    void 刷新自我融合特征_(需求节点类* 当前主需求, 任务节点类* 当前主任务, 方法节点类* 当前主方法, I64 尝试学习状态, 时间戳 now, const std::string& 调用点)
    {
        (void)确保自我存在_(now, 调用点);
        (void)记录自我特征_(特征类型定义类::类型_自我_安全值, 特征快照值{ (I64)safety_.load() }, now, 调用点 + "/安全值");
        (void)记录自我特征_(特征类型定义类::类型_自我_服务值, 特征快照值{ (I64)service_.load() }, now, 调用点 + "/服务值");
        (void)记录自我特征_(特征类型定义类::类型_自我_待学习方法数量, 特征快照值{ (I64)roots_.学习 }, now, 调用点 + "/待学习方法数量");
        (void)记录自我特征_(特征类型定义类::类型_自我_当前主需求, 特征快照值{ 指针句柄{ reinterpret_cast<std::uintptr_t>(当前主需求) } }, now, 调用点 + "/当前主需求");
        (void)记录自我特征_(特征类型定义类::类型_自我_当前主任务, 特征快照值{ 指针句柄{ reinterpret_cast<std::uintptr_t>(当前主任务) } }, now, 调用点 + "/当前主任务");
        (void)记录自我特征_(特征类型定义类::类型_自我_当前主方法, 特征快照值{ 指针句柄{ reinterpret_cast<std::uintptr_t>(当前主方法) } }, now, 调用点 + "/当前主方法");
        (void)记录自我特征_(特征类型定义类::类型_自我_外设可用性, 特征快照值{ 判定外设可用性_() }, now, 调用点 + "/外设可用性");
        (void)记录自我特征_(特征类型定义类::类型_自我_尝试学习状态, 特征快照值{ 尝试学习状态 }, now, 调用点 + "/尝试学习状态");
    }

    const 词性节点类* 类型_内部尝试学习需求_() const
    {
        return 语素集.添加词性词("内部尝试学习需求", "名词");
    }

    const 词性节点类* 名称_内部尝试学习任务_() const
    {
        return 语素集.添加词性词("内部尝试学习任务", "名词");
    }

    场景节点类* 确保自我尝试学习场景_(时间戳 now, const std::string& 调用点)
    {
        auto* 内部世界 = 确保自我内部世界_(now, 调用点);
        if (!内部世界) return nullptr;
        auto* 名称 = 语素集.添加词性词("场景_自我尝试学习", "名词");
        return 世界树.取或创建子场景_按名称(内部世界, 名称, now, 调用点 + "/尝试学习场景");
    }

    bool 是内部尝试学习需求_(const 需求节点类* need) const
    {
        auto* mi = need ? need->主信息 : nullptr;
        return mi && mi->类型 == 类型_内部尝试学习需求_();
    }

    状态节点类* 确保内部尝试学习目标状态_(场景节点类* 场景, 存在节点类* 自我存在, 时间戳 now, const std::string& 调用点)
    {
        if (!场景 || !场景->主信息 || !自我存在) return nullptr;
        auto* feat = 世界树.确保特征(自我存在, 特征类型定义类::类型_自我_待学习方法数量, 特征类型定义类::类型_自我_待学习方法数量, 调用点);
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!feat || !smi) return nullptr;

        for (auto it = smi->状态列表.rbegin(); it != smi->状态列表.rend(); ++it) {
            auto* s = *it;
            auto* mi = (s && s->主信息) ? dynamic_cast<状态节点主信息类*>(s->主信息) : nullptr;
            if (!mi) continue;
            if (mi->状态域 != 枚举_状态域::内部状态) continue;
            if (mi->状态主体 != 自我存在) continue;
            if (mi->状态特征 != feat) continue;
            if (std::holds_alternative<I64>(mi->状态值) && std::get<I64>(mi->状态值) == 0) return s;
        }

        return 状态集.创建内部状态(场景, 自我存在, feat, 特征快照值{ (I64)0 }, 枚举_存在状态事件::创建, false, now);
    }

    任务节点类* 创建内部尝试学习任务_(需求节点类* need, 场景节点类* 场景, 时间戳 now, const std::string& 调用点)
    {
        if (!need || !场景) return nullptr;
        auto* tmi = new 任务头结点信息();
        tmi->名称 = 名称_内部尝试学习任务_();
        tmi->类型 = 类型_内部尝试学习需求_();
        tmi->节点种类 = 枚举_任务节点种类::头结点;
        tmi->状态 = 枚举_任务状态::就绪;
        tmi->创建时间 = now;
        tmi->基准优先级 = (std::max<std::int64_t>)(1, roots_.学习);
        tmi->局部优先级偏移 = 0;
        tmi->调度优先级 = tmi->基准优先级;
        tmi->场景 = 场景;
        tmi->任务树类型 = 枚举_任务树类型::叶子任务;
        tmi->需求 = need;
        return 任务类::新建任务节点(tmi, nullptr, 调用点);
    }

    需求节点类* 确保内部尝试学习需求_(时间戳 now, const std::string& 调用点)
    {
        if (roots_.学习 <= 0) return nullptr;
        auto* 自我存在 = 确保自我存在_(now, 调用点);
        auto* 场景 = 确保自我尝试学习场景_(now, 调用点);
        auto* 目标状态 = 确保内部尝试学习目标状态_(场景, 自我存在, now, 调用点);
        if (!自我存在 || !场景 || !目标状态) return nullptr;

        for (auto* need : 需求集.获取全部需求(调用点 + "/现有需求")) {
            auto* mi = need ? need->主信息 : nullptr;
            if (!mi) continue;
            if (!是内部尝试学习需求_(need)) continue;
            if (mi->需求主体 != 自我存在) continue;
            if (mi->被需求状态 != 目标状态) continue;
            if (!mi->相关任务) mi->相关任务 = 创建内部尝试学习任务_(need, 场景, now, 调用点 + "/补任务");
            return need;
        }

        需求类::结构_创建参数 p{};
        p.类型 = 类型_内部尝试学习需求_();
        p.发生场景 = 场景;
        p.生成时间 = now;
        p.权重 = (std::max<std::int64_t>)(1, roots_.学习);
        auto* need = 需求集.创建需求(自我存在, 自我存在, 目标状态, p, 调用点 + "/创建需求");
        auto* mi = need ? need->主信息 : nullptr;
        if (mi) mi->相关任务 = 创建内部尝试学习任务_(need, 场景, now, 调用点 + "/创建任务");
        return need;
    }

    场景节点类* 创建尝试学习输出场景_(场景节点类* parent, 时间戳 now, const std::string& 调用点)
    {
        auto* mi = new 场景节点主信息类();
        mi->名称 = 语素集.添加词性词(std::string("场景_方法尝试学习输出_") + std::to_string(now), "名词");
        return 世界树.创建场景(parent, mi, 调用点);
    }

    bool 执行尝试学习参数场景_(方法节点类* 方法首节点, 场景节点类* 参数场景, 时间戳 now, const std::string& 调用点)
    {
        auto* hmi = 方法首节点 ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息) : nullptr;
        if (!hmi || !参数场景) return false;
        if (hmi->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID || hmi->动作句柄.本能ID_U64 == 0) return false;

        auto* 输出场景 = 创建尝试学习输出场景_(static_cast<场景节点类*>(参数场景->父), now, 调用点 + "/输出场景");
        if (!输出场景) return false;

        结构_方法上下文 ctx{};
        ctx.now = now;
        ctx.方法首节点 = 方法首节点;
        ctx.输入观测包场景 = 参数场景;
        ctx.任务条件场景 = 参数场景;
        ctx.调用覆盖场景 = 输出场景;
        ctx.当前场景 = 参数场景;
        ctx.条件场景 = 参数场景;

        std::atomic_bool cancel{ false };
        auto r = 方法集.调用本能函数(hmi->动作句柄.本能ID_U64, ctx, cancel);
        return r.成功;
    }

    bool 尝试学习参数入队_(方法节点类* 方法首节点, 场景节点类* 参数场景, 需求节点类* 来源需求, 时间戳 now)
    {
        if (!方法首节点 || !参数场景) return false;
        std::scoped_lock lk(尝试学习参数队列锁_);
        尝试学习参数队列_.push_back({ 方法首节点, 参数场景, 来源需求, now });
        return true;
    }

    std::optional<结构_尝试学习参数队列项> 取下一尝试学习参数_()
    {
        std::scoped_lock lk(尝试学习参数队列锁_);
        if (尝试学习参数队列_.empty()) return std::nullopt;
        auto item = 尝试学习参数队列_.front();
        尝试学习参数队列_.pop_front();
        return item;
    }

    bool 消费一个尝试学习参数队列项_(时间戳 now)
    {
        auto item = 取下一尝试学习参数_();
        if (!item.has_value()) return false;

        auto* need = item->来源需求;
        auto* nmi = need ? need->主信息 : nullptr;
        刷新自我融合特征_(need, nmi ? nmi->相关任务 : nullptr, item->方法首节点, 自我尝试学习状态_准备中, now, "自我类::消费尝试学习参数队列/执行前");

        const bool ok = 执行尝试学习参数场景_(item->方法首节点, item->参数场景, now, "自我类::消费尝试学习参数队列");
        if (ok) {
            服务值_上升_(5);
            刷新自我融合特征_(need, nmi ? nmi->相关任务 : nullptr, item->方法首节点, 自我尝试学习状态_已生成, now, "自我类::消费尝试学习参数队列/执行成功");
        }
        else {
            服务值_下降_(2);
            安全值_下降_(1);
            刷新自我融合特征_(need, nmi ? nmi->相关任务 : nullptr, item->方法首节点, 自我尝试学习状态_失败, now, "自我类::消费尝试学习参数队列/执行失败");
        }
        return true;
    }

    // ==========================================================
    // Worker 线程池（最小实现：FIFO job + future）
    // ==========================================================
    class 工作者池 {
    public:
        using JobFn = std::function<void(std::atomic_bool& cancel)>;

        工作者池() = default;

        void 启动(std::uint32_t n) {
            if (n == 0) return;
            stop_.store(false);
            cancelAll_.store(false);
            for (std::uint32_t i = 0; i < n; ++i) {
                threads_.emplace_back([this] { this->Loop_(); });
            }
        }

        void 请求停止() {
            stop_.store(true);
            cancelAll_.store(true);
            cv_.notify_all();
        }

        void 等待停止() {
            {
                std::unique_lock lk(mtx_);
                cv_.notify_all();
            }
            for (auto& t : threads_) {
                if (t.joinable()) t.join();
            }
            threads_.clear();
            {
                std::unique_lock lk(mtx_);
                q_.clear();
            }
        }

        template<class R>
        std::future<R> 提交(std::function<R(std::atomic_bool& cancel)> fn) {
            auto task = std::make_shared<std::packaged_task<R(std::atomic_bool&)>>(std::move(fn));
            std::future<R> fut = task->get_future();
            {
                std::unique_lock lk(mtx_);
                q_.push_back([task](std::atomic_bool& cancel) { (*task)(cancel); });
            }
            cv_.notify_one();
            return fut;
        }

        void 取消全部() {
            cancelAll_.store(true);
        }

    private:
        void Loop_() {
            while (!stop_.load()) {
                JobFn job;
                {
                    std::unique_lock lk(mtx_);
                    cv_.wait(lk, [&] { return stop_.load() || !q_.empty(); });
                    if (stop_.load()) break;
                    job = std::move(q_.front());
                    q_.pop_front();
                }

                // 每个 job 有一个本地 cancel，且受 cancelAll 影响
                std::atomic_bool cancel{ false };
                if (cancelAll_.load()) cancel.store(true);

                try {
                    if (job) job(cancel);
                }
                catch (...) {
                    // Worker 内吞掉异常：交给 future 的调用者处理
                }
            }
        }

        std::mutex mtx_;
        std::condition_variable cv_;
        std::deque<JobFn> q_;
        std::vector<std::thread> threads_;
        std::atomic_bool stop_{ false };
        std::atomic_bool cancelAll_{ false };
    };

private:

    // ==========================================================
    // 方法执行（最小闭环：仅本能函数）
    // ==========================================================

    inline 结构_方法执行结果 执行方法_同步等待(方法节点类* 方法首节点, 时间戳 timeoutUs)
    {
        auto* hmi = 方法首节点 ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息) : nullptr;
        if (!hmi) return {};
        if (hmi->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID) return {};

        const std::uint64_t innateId = hmi->动作句柄.本能ID_U64;

        // 组装上下文：当前只填 now
        结构_方法上下文 ctx;
        ctx.now = 结构体_时间戳::当前_微秒();
        ctx.方法首节点 = 方法首节点;
      //  ctx.当前场景 = nullptr;
      //  ctx.条件场景 = nullptr;

        // 投递到 worker
        auto fut = worker_.提交<结构_方法执行结果>(
            [this, innateId, ctx](std::atomic_bool& cancel) mutable {
                // cancel 由 worker 管理，Self 侧只设置 cancelAll
                return 方法集.调用本能函数(innateId, ctx, cancel);
            }
        );

        if (timeoutUs == 0) {
            // 不设超时：阻塞等待
            return fut.get();
        }

        auto st = fut.wait_for(std::chrono::microseconds(timeoutUs));
        if (st == std::future_status::ready) {
            return fut.get();
        }

        // 超时：发出取消（全局），并返回失败
        worker_.取消全部();

        结构_方法执行结果 r{};
        r.成功 = false;
      //  r.成功码 = -1;
      //  r.解释 = "timeout";
        return r;
    }
   
    // ==========================================================
    // Self 主循环
    // ==========================================================

    void SelfLoop_() {
        const auto startUs = 结构体_时间戳::当前_微秒();
        std::uint64_t tickCount = 0;

        while (!stop_.load()) {
            // 退出硬条件：安全值归零（服务值为零进入待机，不视为消亡）
            if (safety_.load() == 0) {
                保存退出事件_("安全值归零", startUs, tickCount);
                break;
            }
            // 调试上限
            if (cfg_.调试_最大循环次数 != 0 && tickCount >= cfg_.调试_最大循环次数) {
                保存退出事件_("达到调试最大循环次数", startUs, tickCount);
                break;
            }

            try {
                TickOnce_();
            }
            catch (const std::exception& e) {
                // 异常视为危险信号：轻度下降安全值
                安全值_下降_(100);
                lastError_ = e.what();
            }
            catch (...) {
                安全值_下降_(100);
                lastError_ = "unknown";
            }

            ++tickCount;

            // 节拍
            std::this_thread::sleep_for(std::chrono::microseconds(cfg_.Tick间隔_微秒));
        }

        // 让 worker 退出
        worker_.请求停止();
        worker_.等待停止();
        running_.store(false);
    }

    void TickOnce_() {
        // 1) 扫描需求去重（轻量）
        (void)需求集.扫描并合并重复需求();

        const auto tickNow = 结构体_时间戳::当前_微秒();
        (void)确保内部尝试学习需求_(tickNow, "自我类::TickOnce_/内部学习需求");

        if (消费一个尝试学习参数队列项_(tickNow)) {
            return;
        }

        // 2) 选择下一需求
        auto* needNode = 需求集.选择下一需求();
        if (!needNode || !needNode->主信息) {
            // 无需求：服务值慢慢下降（模拟“无人需求会掉服务值”）
            服务值_下降_(1);
            刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::TickOnce_/无需求");
            return;
        }

        auto* nmi = needNode->主信息;
        刷新自我融合特征_(needNode, nmi ? nmi->相关任务 : nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::TickOnce_/选中需求");

        // 3) 用需求签名召回方法（粗筛）
        const auto& sig = nmi->需求签名;
        auto heads = 方法集.召回_按影响度量签名(sig, 10);

        if (heads.empty()) {
            // 4) 无法召回：提高学习权重并对该需求加权，避免饿死
            roots_.学习 = roots_.学习 + 1;
            需求集.调整权重(needNode, 1);
            刷新自我融合特征_(needNode, nmi ? nmi->相关任务 : nullptr, nullptr, 自我尝试学习状态_准备中, 结构体_时间戳::当前_微秒(), "自我类::TickOnce_/无方法召回");
            // 可选：触发尝试学习（当前只做最小兜底）
            触发尝试学习_(needNode);
            return;
        }

        // 5) 选一个可执行的方法（当前只执行本能函数）
        方法节点类* chosen = nullptr;
        for (auto* h : heads) {
            auto* hmi = h ? dynamic_cast<方法首节点主信息类*>(h->主信息) : nullptr;
            if (!hmi) continue;
            if (hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID && hmi->动作句柄.本能ID_U64 != 0) {
                chosen = h;
                break;
            }
        }

        if (!chosen) {
            // 候选都不可执行：当作学习缺口
            roots_.学习 = roots_.学习 + 1;
            需求集.调整权重(needNode, 1);
            刷新自我融合特征_(needNode, nmi ? nmi->相关任务 : nullptr, nullptr, 自我尝试学习状态_准备中, 结构体_时间戳::当前_微秒(), "自我类::TickOnce_/无可执行方法");
            触发尝试学习_(needNode);
            return;
        }

        // 6) 执行（丢到 worker）
        刷新自我融合特征_(needNode, nmi ? nmi->相关任务 : nullptr, chosen, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::TickOnce_/执行前");
        auto res = 执行方法_同步等待(chosen, cfg_.执行超时_微秒);

        if (res.成功) {
            // 7) 成功：服务值上升，需求权重下降或直接删除
            服务值_上升_(50);
            // 简化策略：成功就删除需求（后续可换成“降权 + 留档”）
            刷新自我融合特征_(needNode, nmi ? nmi->相关任务 : nullptr, chosen, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::TickOnce_/执行成功");
            (void)需求集.删除需求(needNode);
        }
        else {
            // 8) 失败：轻度下降安全/服务，并推动学习
            服务值_下降_(10);
            安全值_下降_(5);
            roots_.学习 = roots_.学习 + 2;
            需求集.调整权重(needNode, 1);
            刷新自我融合特征_(needNode, nmi ? nmi->相关任务 : nullptr, chosen, 自我尝试学习状态_准备中, 结构体_时间戳::当前_微秒(), "自我类::TickOnce_/执行失败");
        }
    }


    std::vector<状态节点类*> 收集尝试学习条件状态_(const 需求主信息类* mi) const {
        std::vector<状态节点类*> out;
        if (!mi || !mi->需求场景 || !mi->需求场景->主信息) return out;

        auto* smi = dynamic_cast<场景节点主信息类*>(mi->需求场景->主信息);
        if (!smi) return out;

        out.reserve(smi->状态列表.size());
        for (auto* s : smi->状态列表) {
            if (!s || s == mi->被需求状态) continue;
            out.push_back(s);
        }
        return out;
    }

    // ==========================================================
    // 学习兜底（最小版：只做记录与权重调节）
    // ==========================================================
    void 触发尝试学习_(需求节点类* need) {
        if (!need || !need->主信息) return;

        auto* mi = need->主信息;
        if (!mi->被需求状态) return;

        const auto now = 结构体_时间戳::当前_微秒();
        刷新自我融合特征_(need, mi->相关任务, nullptr, 自我尝试学习状态_准备中, now, "自我类::触发尝试学习_/开始");
        auto 条件状态 = 收集尝试学习条件状态_(mi);
        const auto& sig = mi->需求签名;
        auto heads = 方法集.召回_按影响度量签名(sig, 3, "自我类::触发尝试学习_召回");
        if (heads.empty()) {
            刷新自我融合特征_(need, mi->相关任务, nullptr, 自我尝试学习状态_失败, now, "自我类::触发尝试学习_/无候选");
            return;
        }

        for (auto* head : heads) {
            if (!head) continue;
            刷新自我融合特征_(need, mi->相关任务, head, 自我尝试学习状态_准备中, now, "自我类::触发尝试学习_/候选方法");
            auto 结果 = 方法集.生成尝试学习参数(
                head,
                条件状态,
                mi->被需求状态,
                8,
                now,
                "自我类::触发尝试学习_生成参数");

            if (结果.参数场景) {
                (void)尝试学习参数入队_(head, 结果.参数场景, need, now);
                roots_.学习 = (roots_.学习 > 0) ? (roots_.学习 - 1) : 0;
                if (是内部尝试学习需求_(need)) {
                    if (roots_.学习 <= 0) {
                        (void)需求集.删除需求(need, "自我类::触发尝试学习_/删除内部需求");
                    }
                    else {
                        (void)需求集.调整权重(need, -1, "自我类::触发尝试学习_/内部需求降权");
                    }
                }
                刷新自我融合特征_(need, mi->相关任务, head, 自我尝试学习状态_已生成, now, "自我类::触发尝试学习_/生成成功");
                return;
            }
        }
        刷新自我融合特征_(need, mi->相关任务, nullptr, 自我尝试学习状态_失败, now, "自我类::触发尝试学习_/结束");
    }

    // ==========================================================
    // 安全/服务值更新（饱和）
    // ==========================================================

    static U64 sat_add(U64 a, U64 b) noexcept {
        const U64 r = a + b;
        if (r < a) return U64_MAX;
        return r;
    }
    static U64 sat_sub(U64 a, U64 b) noexcept {
        return (a > b) ? (a - b) : 0;
    }

    void 重算时序步长_按服务值_() noexcept {
        const U64 srv = service_.load();
        if (srv == 0) {
            temporal_forward_step_.store(1);
            temporal_backward_step_.store(1);
            return;
        }
        // 服务值越高：正向步长越大，反向步长越小。
        constexpr U64 放大倍率 = 4; // 步长范围：[1,5]
        const U64 正向 = 1 + (srv * 放大倍率) / U64_MAX;
        const U64 反向 = 1 + ((U64_MAX - srv) * 放大倍率) / U64_MAX;
        temporal_forward_step_.store((std::max<U64>)(1, 正向));
        temporal_backward_step_.store((std::max<U64>)(1, 反向));
    }

    void 安全值_上升_(U64 delta) noexcept {
        safety_.store(sat_add(safety_.load(), delta));
        刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::安全值_上升_");
    }
    void 安全值_下降_(U64 delta) noexcept {
        safety_.store(sat_sub(safety_.load(), delta));
        刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::安全值_下降_");
    }
    void 服务值_上升_(U64 delta) noexcept {
        service_.store(sat_add(service_.load(), delta));
        重算时序步长_按服务值_();
        刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::服务值_上升_");
    }
    void 服务值_下降_(U64 delta) noexcept {
        service_.store(sat_sub(service_.load(), delta));
        重算时序步长_按服务值_();
        刷新自我融合特征_(nullptr, nullptr, nullptr, 自我尝试学习状态_空闲, 结构体_时间戳::当前_微秒(), "自我类::服务值_下降_");
    }

    // ==========================================================
    // 退出事件落盘（最小版：文本文件）
    // ==========================================================

    void 保存退出事件_(const std::string& reason, 时间戳 startUs, std::uint64_t ticks) {
        try {
            std::ofstream os(cfg_.退出事件文件, std::ios::out | std::ios::trunc);
            if (!os.good()) return;
            const auto now = 结构体_时间戳::当前_微秒();
            os << "reason=" << reason << "\n";
            os << "start_us=" << startUs << "\n";
            os << "now_us=" << now << "\n";
            os << "ticks=" << ticks << "\n";
            os << "safety=" << safety_.load() << "\n";
            os << "service=" << service_.load() << "\n";
            os << "last_error=" << lastError_ << "\n";
            os.close();
        }
        catch (...) {
        }
    }

private:
    结构_自我配置 cfg_{};

    std::atomic_bool running_{ false };
    std::atomic_bool stop_{ false };

    std::thread selfThread_;
    工作者池 worker_;

    // 全局状态：数值尺度 0..U64_MAX
    std::atomic<U64> safety_{ (U64_MAX / 2) };
    std::atomic<U64> service_{ (U64_MAX / 2) };
    std::atomic<U64> temporal_forward_step_{ 1 };
    std::atomic<U64> temporal_backward_step_{ 1 };
    std::atomic_bool standby_mode_{ false };
    结构_根任务权重 roots_{};
    std::mutex 尝试学习参数队列锁_{};
    std::deque<结构_尝试学习参数队列项> 尝试学习参数队列_{};

    // 诊断
    std::string lastError_{};
};







