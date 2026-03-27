module;
#ifdef HY_HEADLESS_NO_MFC
#include "hy_windows.h"
#else
#include <afx.h>
#endif

#include <atomic>
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <fstream>
#include <functional>
#include <future>
#include <limits>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <variant>
#include <utility>
#include <vector>

export module 自我模块;

import 基础数据类型模块;
import 通用函数模块;
import 主信息定义模块;
import 数据仓库模块;
import 语素环境模块;
import 特征类型定义模块;
import 状态模块;
import 二次特征模块;
import 动态模块;

import 世界树环境模块;
import 语言环境模块;
import 需求环境模块;
import 方法模块;
import 方法环境模块;
import 任务模块;
import 度量模板注册表模块;
import 日志模块;

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
        if (当前活动自我_.load(std::memory_order_acquire) == this) {
            当前活动自我_.store(nullptr, std::memory_order_release);
        }
    }

    自我类(const 自我类&) = delete;
    自我类& operator=(const 自我类&) = delete;

    // ==========================================================
    // 生命周期
    // ==========================================================
    
    void 初始化自我(const std::string& 调用点 = "自我类::初始化") {
        (void)调用点;
        当前活动自我_.store(this, std::memory_order_release);
        初始化世界骨架环境(调用点 + "/世界骨架");
        const 时间戳 now = 结构体_时间戳::当前_微秒();
        (void)世界树.取或创建自我内部世界(now, 调用点 + "/默认自我");
        重置时序步长为默认值_();
        初始化自我特征类型与默认值_(now, 调用点 + "/初始化自我特征");
        // 方法/需求是环境单例：需求集、方法集
        // 默认模板：建议在世界树初始化后调用一次
        try {
            度量模板注册表类::实例().注册默认模板();
        }
        catch (...) {
            // 允许重复注册或失败不致命
        }
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

    I64 获取安全值() const noexcept {
        return 读取自我I64特征当前值_(
            特征类型定义类::类型_自我_安全值,
            (生命值上限_() / 2),
            "自我类::获取安全值");
    }
    I64 获取服务值() const noexcept {
        return std::get<I64>(*世界树.读取特征快照(
            世界树.自我指针,
            特征类型定义类::类型_自我_服务值,
            "自我类::获取服务值"));
    }
    I64 获取物理安全值() const noexcept {
        return 读取自我I64特征当前值_(
            特征类型定义类::类型_自我_物理安全,
            生命值上限_(),
            "自我类::获取物理安全值");
    }
    I64 获取风险安全值() const noexcept {
        return 读取自我I64特征当前值_(
            特征类型定义类::类型_自我_风险安全,
            生命值上限_(),
            "自我类::获取风险安全值");
    }

    bool 是否已消亡() const noexcept { return 获取安全值() == 0; }

    I64 根目标安全值() const noexcept { return 生命值上限_(); }
    I64 根目标服务值() const noexcept { return 生命值上限_(); }
    I64 安全根方向差值() const noexcept { return 根目标安全值() - 获取安全值(); }

    // 供调度线程调用的统一数值更新入口。
    void 安全值增加(
        I64 delta,
        const std::string& 原因类别 = "安全值增加",
        const std::string& 原因说明 = {}) noexcept {
        if (delta > 0) 安全值_上升_(delta, 原因类别, 原因说明);
        else if (delta < 0) 安全值_下降_(-delta, 原因类别, 原因说明);
    }
    void 安全值减少(
        I64 delta,
        const std::string& 原因类别 = "安全值减少",
        const std::string& 原因说明 = {}) noexcept {
        if (delta > 0) 安全值_下降_(delta, 原因类别, 原因说明);
        else if (delta < 0) 安全值_上升_(-delta, 原因类别, 原因说明);
    }
    void 服务值增加(
        I64 delta,
        const std::string& 原因类别 = "服务值增加",
        const std::string& 原因说明 = {}) noexcept {
        if (delta > 0) 服务值_上升_(delta, 原因类别, 原因说明);
        else if (delta < 0) 服务值_下降_(-delta, 原因类别, 原因说明);
    }
    void 服务值减少(
        I64 delta,
        const std::string& 原因类别 = "服务值减少",
        const std::string& 原因说明 = {}) noexcept {
        if (delta > 0) 服务值_下降_(delta, 原因类别, 原因说明);
        else if (delta < 0) 服务值_上升_(-delta, 原因类别, 原因说明);
    }
    I64 时序正向步长() const noexcept { return temporal_forward_step_.load(); }
    I64 时序反向步长() const noexcept { return temporal_backward_step_.load(); }
    I64 服务时序衰减步长() const noexcept { return (std::max<I64>)(1, service_decay_step_.load()); }
    void 设置服务时序衰减步长(I64 步长) noexcept {
        service_decay_step_.store((std::max<I64>)(1, 步长));
    }
    bool 是否待机状态() const noexcept { return standby_mode_.load(); }
    static 自我类* 当前活动自我() noexcept {
        return 当前活动自我_.load(std::memory_order_acquire);
    }
    运行时动态主信息记录* 记录自我动作动态(
        const 词性节点类* 动作词,
        场景节点类* 输入场景 = nullptr,
        场景节点类* 输出场景 = nullptr,
        bool 成功 = true,
        std::int64_t 错误码 = 0,
        时间戳 开始时间 = 0,
        时间戳 结束时间 = 0,
        枚举_动作事件相位 动作相位 = 枚举_动作事件相位::未定义,
        const std::string& 调用点 = "自我类::记录自我动作动态",
        状态节点类* 初始状态 = nullptr,
        状态节点类* 结果状态 = nullptr)
    {
        if (!动作词) return nullptr;
        if (开始时间 == 0) 开始时间 = 结构体_时间戳::当前_微秒();
        if (结束时间 == 0) 结束时间 = 开始时间;
        if (结束时间 < 开始时间) std::swap(开始时间, 结束时间);

        auto* 内部世界 = 确保自我内部世界_(结束时间, 调用点);
        auto* 自我存在 = 确保自我存在_(结束时间, 调用点);
        if (!内部世界 || !自我存在) return nullptr;

        auto* 动作特征 = 世界树.确保特征(
            自我存在,
            动作词,
            动作词,
            调用点 + "/动作特征");
        if (!动作特征) return nullptr;

        auto* 动态信息 = 动态集.创建动作动态主信息(
            内部世界,
            自我存在,
            动作特征,
            开始时间,
            结束时间);
        if (!动态信息) return 动态信息;

        auto 状态节点转运行时记录 = [](状态节点类* 状态) -> std::optional<运行时状态主信息记录> {
            auto* 状态信息 = (状态 && 状态->主信息)
                ? dynamic_cast<状态节点主信息类*>(状态->主信息)
                : nullptr;
            if (!状态信息) return std::nullopt;
            运行时状态主信息记录 out{};
            out.状态域 = 状态信息->状态域;
            out.收到时间 = 状态信息->收到时间;
            out.发生时间 = 状态信息->发生时间;
            out.状态主体 = 状态信息->状态主体;
            out.状态特征 = 状态信息->状态特征;
            out.状态值 = 状态信息->状态值;
            out.对应信息节点 = 状态信息->对应信息节点;
            out.是否变化 = 状态信息->是否变化;
            out.变化原因类别 = 状态信息->变化原因类别;
            out.变化原因说明 = 状态信息->变化原因说明;
            return out;
        };
        auto 追加唯一状态记录 = [](std::vector<运行时状态主信息记录>& out, const std::optional<运行时状态主信息记录>& 状态) {
            if (!状态.has_value()) return;
            const auto 已有 = std::find_if(out.begin(), out.end(), [&](const 运行时状态主信息记录& 当前) {
                return 当前.状态域 == 状态->状态域
                    && 当前.收到时间 == 状态->收到时间
                    && 当前.发生时间 == 状态->发生时间
                    && 当前.状态主体 == 状态->状态主体
                    && 当前.状态特征 == 状态->状态特征
                    && 特征快照相等(当前.状态值, 状态->状态值)
                    && 当前.对应信息节点 == 状态->对应信息节点
                    && 当前.是否变化 == 状态->是否变化
                    && 当前.变化原因类别 == 状态->变化原因类别
                    && 当前.变化原因说明 == 状态->变化原因说明;
            });
            if (已有 == out.end()) out.push_back(*状态);
        };
        const auto 初始状态记录 = 状态节点转运行时记录(初始状态);
        const auto 结果状态记录 = 状态节点转运行时记录(结果状态);
        if (初始状态记录.has_value()) 动态信息->初始状态 = *初始状态记录;
        if (结果状态记录.has_value()) 动态信息->结果状态 = *结果状态记录;
        追加唯一状态记录(动态信息->事件列表, 初始状态记录);
        追加唯一状态记录(动态信息->事件列表, 结果状态记录);
        追加唯一状态记录(动态信息->状态路径列表, 初始状态记录);
        追加唯一状态记录(动态信息->状态路径列表, 结果状态记录);
        动态信息->来源类型 = 枚举_动态来源类型::内部机制动作;
        动态信息->来源动作名 = 动作词;
        动态信息->来源动作相位 = 动作相位;
        动态信息->来源输入场景 = 输入场景;
        动态信息->来源输出场景 = 输出场景;
        动态信息->来源执行成功 = 成功;
        动态信息->来源错误码 = 错误码;
        return 动态信息;
    }

    bool 应用服务归零待机保护(
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用服务归零待机保护",
        状态节点类** out主结果状态 = nullptr) noexcept
    {
        重算时序步长_按服务值_();

        if (获取服务值() != 0) {
            standby_mode_.store(false);
            return false;
        }

        standby_mode_.store(true);
        constexpr I64 最小非零安全值 = 1;
        const I64 旧安全值 = 获取安全值();
        if (旧安全值 == 最小非零安全值) return false;

        const 时间戳 结果时间 = 结构体_时间戳::当前_微秒();
        (void)写入自我I64特征当前值_(
            特征类型定义类::类型_自我_安全值,
            最小非零安全值,
            结果时间,
            调用点 + "/写安全值");
        if (out主结果状态) *out主结果状态 = nullptr;
        return true;
    }

    bool 应用定时衰减服务值(
        I64 delta = 0,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用定时衰减服务值",
        状态节点类** out主结果状态 = nullptr) noexcept
    {
        if (delta <= 0) {
            delta = 服务时序衰减步长();
        }
        delta = (std::max<I64>)(1, delta);
        const I64 旧服务值 = 获取服务值();
        const I64 新服务值 = 通用函数模块::饱和减(旧服务值, delta);
        if (新服务值 == 旧服务值) return false;

        const 时间戳 结果时间 = 结构体_时间戳::当前_微秒();
        (void)写入自我I64特征当前值_(
            特征类型定义类::类型_自我_服务值,
            新服务值,
            结果时间,
            调用点 + "/写服务值");
        重算时序步长_按服务值_();
        if (out主结果状态) *out主结果状态 = nullptr;
        return true;
    }

    bool 应用风险安全回归(
        I64 delta = 1,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::应用风险安全回归",
        状态节点类** out主结果状态 = nullptr) noexcept
    {
        delta = (std::max<I64>)(1, delta);
        const I64 旧风险安全值 = 获取风险安全值();
        const I64 目标风险安全值 = 风险安全回归目标值_();
        if (旧风险安全值 <= 目标风险安全值) return false;

        const I64 候选值 = 通用函数模块::饱和减(旧风险安全值, delta);
        const I64 新风险安全值 = (std::max<I64>)(候选值, 目标风险安全值);
        if (新风险安全值 == 旧风险安全值) return false;

        const 时间戳 结果时间 = (now > 0) ? now : 结构体_时间戳::当前_微秒();
        (void)写入自我I64特征当前值_(
            特征类型定义类::类型_自我_风险安全,
            新风险安全值,
            结果时间,
            调用点 + "/写风险安全");
        if (out主结果状态) *out主结果状态 = nullptr;
        return true;
    }

    状态节点类* 记录基础方向概念(
        const std::string& 概念名,
        I64 值,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::记录基础方向概念")
    {
        const auto* 概念类型 = 语素集.添加词性词("基础概念_" + 概念名, "名词");
        auto* 自我存在 = 确保自我存在_(now, 调用点);
        if (!概念类型 || !自我存在) return nullptr;
        (void)世界树.写入特征_I64(
            自我存在,
            概念类型,
            值,
            {},
            调用点 + "/基础概念/" + 概念名);
        return nullptr;
    }

    // 兼容旧实验逻辑：保留接口，但主调度链不再调用。
    // 当前正式结算入口改为：自我线程类::私有_按叶子需求结算安全服务值(...)
    void 应用任务结果_默认策略(bool 成功) noexcept {
        if (成功) {
            服务值_上升_(50, "任务成功奖励");
            return;
        }
        服务值_下降_(10, "任务失败惩罚");
        安全值_下降_(5, "任务失败惩罚");
    }


    // 当前这组接口只保留“服务归零待机保护”这条底线规则。
    // 旧的“安全时序回归”链路已移除。
    void 按时序规则更新安全值(
        I64 待完成任务数,
        I64 待学习任务数,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::按时序规则更新安全值",
        状态节点类** out主结果状态 = nullptr) noexcept
    {
        (void)待完成任务数;
        (void)待学习任务数;
        if (获取服务值() == 0) {
            (void)应用服务归零待机保护(now, 调用点, out主结果状态);
        }
    }

    void 按时序规则更新安全服务值(
        I64 待完成任务数,
        I64 待学习任务数,
        时间戳 now = 结构体_时间戳::当前_微秒(),
        const std::string& 调用点 = "自我类::按时序规则更新安全服务值",
        状态节点类** out主结果状态 = nullptr) noexcept
    {
        按时序规则更新安全值(待完成任务数, 待学习任务数, now, 调用点, out主结果状态);
    }

    结构_根任务权重& 根任务权重() noexcept { return roots_; }
    const 结构_根任务权重& 根任务权重() const noexcept { return roots_; }

    // ==========================================================
    // 注入：世界树与差异度（可选）
    // ==========================================================
    //世界树类& 世界树() noexcept { return 世界树_; }
   // const 世界树类& 世界树() const noexcept { return 世界树_; }

private:
    inline static std::atomic<自我类*> 当前活动自我_{ nullptr };

    struct 结构_尝试学习参数队列项 {
        方法节点类* 方法首节点 = nullptr;
        场景节点类* 参数场景 = nullptr;
        需求节点类* 来源需求 = nullptr;
        时间戳 入队时间 = 0;
    };

    场景节点类* 确保自我现实场景_(时间戳 now, const std::string& 调用点)
    {
        return 世界树.取或创建自我现实场景(now, 调用点);
    }

    场景节点类* 确保自我内部世界_(时间戳 now, const std::string& 调用点)
    {
        return 世界树.取或创建自我内部世界(now, 调用点);
    }

    存在节点类* 确保自我存在_(时间戳 now, const std::string& 调用点)
    {
        return 世界树.取或创建自我存在(now, 调用点);
    }

    void 确保自我I64特征已初始化_(
        存在节点类* 自我存在,
        const 词性节点类* 特征类型,
        I64 默认值,
        const std::string& 调用点) noexcept
    {
        if (!自我存在 || !特征类型) return;
        (void)世界树.确保特征(自我存在, 特征类型, 特征类型, 调用点 + "/确保特征");
        const auto 快照 = 世界树.读取特征快照(自我存在, 特征类型, 调用点 + "/读取快照");
        if (快照.has_value() && std::holds_alternative<I64>(*快照)) return;
        (void)世界树.写入特征_I64(自我存在, 特征类型, 默认值, {}, 调用点 + "/写默认值");
    }

    void 确保自我指针特征已初始化_(
        存在节点类* 自我存在,
        const 词性节点类* 特征类型,
        std::uintptr_t 默认值,
        const std::string& 调用点) noexcept
    {
        if (!自我存在 || !特征类型) return;
        (void)世界树.确保特征(自我存在, 特征类型, 特征类型, 调用点 + "/确保特征");
        const auto 快照 = 世界树.读取特征快照(自我存在, 特征类型, 调用点 + "/读取快照");
        if (快照.has_value() && std::holds_alternative<指针句柄>(*快照)) return;
        (void)世界树.写入特征_指针(自我存在, 特征类型, 默认值, {}, 调用点 + "/写默认值");
    }

    void 初始化自我特征类型与默认值_(时间戳 now, const std::string& 调用点) noexcept
    {
        特征类型定义类::初始化特征类型定义模块_依赖语素();

        auto* 自我存在 = 确保自我存在_(now, 调用点 + "/自我存在");
        if (!自我存在) return;

        const I64 默认最大安全值 = (std::numeric_limits<I64>::max)();
        const I64 默认安全值 = 生命值上限_() / 4;
        const I64 默认服务值 = 生命值上限_() / 2;

        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_安全值, 默认安全值, 调用点 + "/安全值");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_服务值, 默认服务值, 调用点 + "/服务值");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_物理安全, 默认最大安全值, 调用点 + "/物理安全");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_风险安全, 默认最大安全值, 调用点 + "/风险安全");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_情绪ID, 0, 调用点 + "/情绪ID");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_情绪强度, 0, 调用点 + "/情绪强度");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_外显情绪ID, 0, 调用点 + "/外显情绪ID");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_外显情绪强度, 0, 调用点 + "/外显情绪强度");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_待学习方法数量, 0, 调用点 + "/待学习方法数量");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_外设可用性, 0, 调用点 + "/外设可用性");
        确保自我I64特征已初始化_(自我存在, 特征类型定义类::类型_自我_尝试学习状态, 0, 调用点 + "/尝试学习状态");

        确保自我指针特征已初始化_(自我存在, 特征类型定义类::类型_自我_当前主需求, 0, 调用点 + "/当前主需求");
        确保自我指针特征已初始化_(自我存在, 特征类型定义类::类型_自我_当前主任务, 0, 调用点 + "/当前主任务");
        确保自我指针特征已初始化_(自我存在, 特征类型定义类::类型_自我_当前主方法, 0, 调用点 + "/当前主方法");

        const auto* 待机状态特征 = 语素集.添加词性词("待机状态", "名词");
        const auto* 时序正向步长特征 = 语素集.添加词性词("时序正向步长", "名词");
        const auto* 时序反向步长特征 = 语素集.添加词性词("时序反向步长", "名词");
        const auto* 服务时序衰减步长特征 = 语素集.添加词性词("服务时序衰减步长", "名词");

        确保自我I64特征已初始化_(自我存在, 待机状态特征, 0, 调用点 + "/待机状态");
        确保自我I64特征已初始化_(自我存在, 时序正向步长特征, 1, 调用点 + "/时序正向步长");
        确保自我I64特征已初始化_(自我存在, 时序反向步长特征, 1, 调用点 + "/时序反向步长");
        确保自我I64特征已初始化_(自我存在, 服务时序衰减步长特征, 1, 调用点 + "/服务时序衰减步长");
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

        return 状态集.创建内部状态(
            场景,
            自我存在,
            feat,
            特征快照值{ (I64)0 },
            枚举_存在状态事件::创建,
            false,
            now,
            [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
            },
            调用点 + "/初始化零状态");
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
        if (hmi->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID || hmi->动作句柄.本能ID == 0) return false;

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
        auto r = 方法集.调用本能函数(static_cast<std::uint64_t>(hmi->动作句柄.本能ID), ctx, cancel);
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

        const bool ok = 执行尝试学习参数场景_(item->方法首节点, item->参数场景, now, "自我类::消费尝试学习参数队列");
        if (ok) {
            服务值_上升_(5, "尝试学习成功奖励");
        }
        else {
            服务值_下降_(2, "尝试学习失败惩罚");
            安全值_下降_(1, "尝试学习失败惩罚");
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

            const I64 innateId = hmi->动作句柄.本能ID;

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
            if (获取安全值() == 0) {
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
                安全值_下降_(100, "异常风险惩罚");
                lastError_ = e.what();
            }
            catch (...) {
                安全值_下降_(100, "异常风险惩罚");
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
            // 无需求时仍按时间规则持续衰减服务值。
            const auto* 动作词 = 语素集.添加词性词("定时衰减服务值", "动词");
            const 时间戳 动作开始 = 结构体_时间戳::当前_微秒();
            (void)记录自我动作动态(
                动作词,
                nullptr,
                nullptr,
                true,
                0,
                动作开始,
                动作开始,
                枚举_动作事件相位::开始运行,
                "自我类::TickOnce_/定时衰减/开始");
            状态节点类* 主结果状态 = nullptr;
            const bool 成功 = 应用定时衰减服务值(
                服务时序衰减步长(),
                动作开始,
                "自我类::TickOnce_/定时衰减",
                &主结果状态);
            const 时间戳 动作结束 = 结构体_时间戳::当前_微秒();
            (void)记录自我动作动态(
                动作词,
                nullptr,
                nullptr,
                成功,
                成功 ? 0 : -1,
                动作结束,
                动作结束,
                成功 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
                "自我类::TickOnce_/定时衰减/结束",
                nullptr,
                主结果状态);
            return;
        }

        auto* nmi = needNode->主信息;

        // 3) 用需求签名召回方法（粗筛）
        const auto& sig = nmi->需求签名;
        auto heads = 方法集.召回_按影响度量签名(sig, 10);

        if (heads.empty()) {
            // 4) 无法召回：提高学习权重并对该需求加权，避免饿死
            roots_.学习 = roots_.学习 + 1;
            需求集.调整权重(needNode, 1);
            // 可选：触发尝试学习（当前只做最小兜底）
            触发尝试学习_(needNode);
            return;
        }

        // 5) 选一个可执行的方法（当前只执行本能函数）
        方法节点类* chosen = nullptr;
        for (auto* h : heads) {
            auto* hmi = h ? dynamic_cast<方法首节点主信息类*>(h->主信息) : nullptr;
            if (!hmi) continue;
        if (hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID && hmi->动作句柄.本能ID != 0) {
                chosen = h;
                break;
            }
        }

        if (!chosen) {
            // 候选都不可执行：当作学习缺口
            roots_.学习 = roots_.学习 + 1;
            需求集.调整权重(needNode, 1);
            触发尝试学习_(needNode);
            return;
        }

        // 6) 执行（丢到 worker）
        auto res = 执行方法_同步等待(chosen, cfg_.执行超时_微秒);

        if (res.成功) {
            // 7) 成功：服务值上升，需求权重下降或直接删除
            服务值_上升_(50, "任务执行成功奖励");
            // 简化策略：成功就删除需求（后续可换成“降权 + 留档”）
            (void)需求集.删除需求(needNode);
        }
        else {
            // 8) 失败：轻度下降安全/服务，并推动学习
            服务值_下降_(10, "任务执行失败惩罚");
            安全值_下降_(5, "任务执行失败惩罚");
            roots_.学习 = roots_.学习 + 2;
            需求集.调整权重(needNode, 1);
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
        auto 条件状态 = 收集尝试学习条件状态_(mi);
        const auto& sig = mi->需求签名;
        auto heads = 方法集.召回_按影响度量签名(sig, 3, "自我类::触发尝试学习_召回");
        if (heads.empty()) {
            return;
        }

        for (auto* head : heads) {
            if (!head) continue;
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
                return;
            }
        }
    }

    // ==========================================================
    // 安全/服务值更新（饱和）
    // ==========================================================

    static constexpr I64 生命值上限_() noexcept {
        return (std::numeric_limits<I64>::max)();
    }

    static constexpr I64 风险安全回归目标值_() noexcept {
        return ((生命值上限_() / 5) * 4) + (((生命值上限_() % 5) * 4) / 5);
    }

    I64 读取自我I64特征当前值_(
        const 词性节点类* 特征类型,
        I64 缺省值,
        const std::string& 调用点) const noexcept
    {
        if (!特征类型) return 缺省值;
        auto* 自我存在 = 世界树.自我指针;
        if (!自我存在) return 缺省值;

        const auto 快照 = 世界树.读取特征快照(自我存在, 特征类型, 调用点);
        if (!快照.has_value()) return 缺省值;
        if (const auto* 值 = std::get_if<I64>(&*快照)) {
            return *值;
        }
        return 缺省值;
    }

    bool 写入自我I64特征当前值_(
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now,
        const std::string& 调用点) noexcept
    {
        if (!特征类型) return false;
        auto* 自我存在 = 确保自我存在_(now, 调用点);
        if (!自我存在) return false;
        (void)世界树.写入特征_I64(
            自我存在,
            特征类型,
            值,
            {},
            调用点);
        return true;
    }

    void 重置时序步长为默认值_() noexcept {
        temporal_forward_step_.store(1);
        temporal_backward_step_.store(1);
        service_decay_step_.store(1);
        standby_mode_.store(false);
    }

    void 重算时序步长_按服务值_() noexcept {
        const I64 srv = 获取服务值();
        if (srv == 0) {
            temporal_forward_step_.store(1);
            temporal_backward_step_.store(1);
            return;
        }
        // 服务值越高：正向步长越大，反向步长越小。
        constexpr I64 放大倍率 = 4; // 步长范围：[1,5]
        const long double 比例 = static_cast<long double>(srv) / static_cast<long double>(生命值上限_());
        const I64 正向 = 1 + static_cast<I64>(比例 * static_cast<long double>(放大倍率));
        const I64 反向 = 1 + static_cast<I64>((1.0L - 比例) * static_cast<long double>(放大倍率));
        temporal_forward_step_.store((std::max<I64>)(1, 正向));
        temporal_backward_step_.store((std::max<I64>)(1, 反向));
    }

    void 安全值_上升_(
        I64 delta,
        const std::string& 原因类别 = "安全值上升",
        const std::string& 原因说明 = {}) noexcept {
        const 时间戳 now = 结构体_时间戳::当前_微秒();
        (void)写入自我I64特征当前值_(
            特征类型定义类::类型_自我_安全值,
            通用函数模块::饱和加(获取安全值(), delta),
            now,
            "自我类::安全值_上升_/写当前值");
        (void)原因类别;
        (void)原因说明;
    }
    void 安全值_下降_(
        I64 delta,
        const std::string& 原因类别 = "安全值下降",
        const std::string& 原因说明 = {}) noexcept {
        const 时间戳 now = 结构体_时间戳::当前_微秒();
        (void)写入自我I64特征当前值_(
            特征类型定义类::类型_自我_安全值,
            通用函数模块::饱和减(获取安全值(), delta),
            now,
            "自我类::安全值_下降_/写当前值");
        (void)原因类别;
        (void)原因说明;
    }
    void 服务值_上升_(
        I64 delta,
        const std::string& 原因类别 = "服务值上升",
        const std::string& 原因说明 = {}) noexcept {
        const 时间戳 now = 结构体_时间戳::当前_微秒();
        (void)写入自我I64特征当前值_(
            特征类型定义类::类型_自我_服务值,
            通用函数模块::饱和加(获取服务值(), delta),
            now,
            "自我类::服务值_上升_/写当前值");
        重算时序步长_按服务值_();
        (void)原因类别;
        (void)原因说明;
    }
    void 服务值_下降_(
        I64 delta,
        const std::string& 原因类别 = "服务值下降",
        const std::string& 原因说明 = {}) noexcept {
        const 时间戳 now = 结构体_时间戳::当前_微秒();
        (void)写入自我I64特征当前值_(
            特征类型定义类::类型_自我_服务值,
            通用函数模块::饱和减(获取服务值(), delta),
            now,
            "自我类::服务值_下降_/写当前值");
        重算时序步长_按服务值_();
        (void)原因类别;
        (void)原因说明;
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
            os << "safety=" << 获取安全值() << "\n";
            os << "service=" << 获取服务值() << "\n";
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

    std::atomic<I64> temporal_forward_step_{ 1 };
    std::atomic<I64> temporal_backward_step_{ 1 };
    std::atomic<I64> service_decay_step_{ 1 };
    std::atomic_bool standby_mode_{ false };
    结构_根任务权重 roots_{};
    std::mutex 尝试学习参数队列锁_{};
    std::deque<结构_尝试学习参数队列项> 尝试学习参数队列_{};

    // 诊断
    std::string lastError_{};
};







