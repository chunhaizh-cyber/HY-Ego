module;
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <exception>
#include <memory>
#include <vector>
#include <string>
#include <cstdint>  // for int64_t
#include <deque>
#include <queue>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <variant>
#include <cassert>

export module 自我线程模块;

import 基础数据类型模块;
import 主信息定义模块;
import 世界树环境模块;
import 语素环境模块;
import 状态模块;
import 二次特征模块;
import 通用函数模块;

import 需求模块;
import 需求环境模块;
import 动态模块;
import 因果信息模块;
import 自我模块;

import 数据仓库模块;
import 任务模块;
import 任务调度器模块;
import 任务执行模块;
import 任务管理模块_v0;
import 方法模块;
import 方法环境模块;
import 度量模板注册表模块;
import 特征类型定义模块;
import 本能动作管理模块;
import 场景模块;
import 概念引擎模块;
import 外设本能方法_相机模块;
import 原生学习兜底函数定义模块_v0;
import 自我本能方法_学习兜底模块;
import 自我本能方法_最小试探模块;
import 自我本能方法_相机认知模块;


import 日志模块;

namespace 自我线程模块_detail {
    inline constexpr I64 方法尝试状态_未尝试 = 0;
    inline constexpr I64 方法尝试状态_尝试中 = 1;
    inline constexpr I64 方法尝试状态_尝试完成 = 2;
    inline constexpr I64 方法尝试状态_待重新验证 = 3;

    struct 结构_自我内部状态快照 {
        I64 安全值 = 0;
        I64 服务值 = 0;
        I64 物理安全值 = 0;
        I64 风险安全值 = 0;
        I64 待机状态 = 0;
        I64 时序正向步长 = 0;
        I64 时序反向步长 = 0;
        I64 服务时序衰减步长 = 0;
    };

    inline const 词性节点类* 特征_方法尝试状态() {
        static const 词性节点类* type = 语素集.添加词性词("方法_尝试状态", "名词");
        return type;
    }
    inline const 词性节点类* 特征_方法最近结果签名() {
        static const 词性节点类* type = 语素集.添加词性词("方法_最近结果签名", "名词");
        return type;
    }
    inline const 词性节点类* 特征_方法结果分裂次数() {
        static const 词性节点类* type = 语素集.添加词性词("方法_结果分裂次数", "名词");
        return type;
    }
    inline const 词性节点类* 特征_方法连续稳定次数() {
        static const 词性节点类* type = 语素集.添加词性词("方法_连续稳定次数", "名词");
        return type;
    }
    inline const 词性节点类* 特征_任务事实(const std::string& 键) { return 任务类::取任务事实特征(键); }
    inline const 词性节点类* 特征_任务方向(const std::string& 键) { return 任务类::取任务方向特征(键); }
    inline const 词性节点类* 动作_定时衰减服务值_() {
        static const 词性节点类* type = 语素集.添加词性词("定时衰减服务值", "动词");
        return type;
    }
    inline const 词性节点类* 动作_服务归零待机保护_() {
        static const 词性节点类* type = 语素集.添加词性词("服务归零待机保护", "动词");
        return type;
    }
    inline const 词性节点类* 动作_风险安全回归_() {
        static const 词性节点类* type = 语素集.添加词性词("风险安全回归", "动词");
        return type;
    }
    inline const 词性节点类* 动作_场景安全度回归_() {
        static const 词性节点类* type = 语素集.添加词性词("场景安全度回归", "动词");
        return type;
    }
    inline const 词性节点类* 动作_紧急中断_() {
        static const 词性节点类* type = 语素集.添加词性词("紧急中断", "动词");
        return type;
    }
    inline const 词性节点类* 动作_安全需求结算_() {
        static const 词性节点类* type = 语素集.添加词性词("安全需求结算", "动词");
        return type;
    }
    inline const 词性节点类* 动作_服务需求结算_() {
        static const 词性节点类* type = 语素集.添加词性词("服务需求结算", "动词");
        return type;
    }
    inline const 词性节点类* 动作_动作任务入队_() {
        static const 词性节点类* type = 语素集.添加词性词("动作任务入队", "动词");
        return type;
    }
    inline const 词性节点类* 动作_学习任务入队_() {
        static const 词性节点类* type = 语素集.添加词性词("学习任务入队", "动词");
        return type;
    }
    inline const 词性节点类* 动作_唤醒等待学习任务_() {
        static const 词性节点类* type = 语素集.添加词性词("唤醒等待学习任务", "动词");
        return type;
    }
    inline const 词性节点类* 动作_动作线程派发_() {
        static const 词性节点类* type = 语素集.添加词性词("动作线程派发", "动词");
        return type;
    }
    inline const 词性节点类* 动作_学习线程派发_() {
        static const 词性节点类* type = 语素集.添加词性词("学习线程派发", "动词");
        return type;
    }

    struct 结构_任务调度动作事件守卫 {
        任务信息基类* 主信息 = nullptr;
        const 词性节点类* 动作词 = nullptr;
        时间戳 开始时间 = 0;
        std::string 调用点{};
        bool 成功 = true;
        std::int64_t 错误码 = 0;

        结构_任务调度动作事件守卫(
            任务信息基类* mi,
            const 词性节点类* 动作,
            时间戳 now,
            std::string cp)
            : 主信息(mi)
            , 动作词(动作)
            , 开始时间(now ? now : 结构体_时间戳::当前_微秒())
            , 调用点(std::move(cp))
        {
            if (!主信息 || !动作词) return;
            (void)任务类::记录任务动作动态(
                主信息,
                动作词,
                nullptr,
                nullptr,
                true,
                0,
                开始时间,
                开始时间,
                枚举_动作事件相位::开始运行,
                调用点 + "/开始");
        }

        void 设置结果(bool ok, std::int64_t err = 0) noexcept {
            成功 = ok;
            错误码 = err;
        }

        ~结构_任务调度动作事件守卫() {
            if (!主信息 || !动作词) return;
            const 时间戳 结束时间 = 结构体_时间戳::当前_微秒();
            (void)任务类::记录任务动作动态(
                主信息,
                动作词,
                nullptr,
                nullptr,
                成功,
                错误码,
                结束时间,
                结束时间,
                成功 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
                调用点 + "/结束");
        }
    };

    inline std::optional<特征快照值> 按特征主键读取快照(
        基础信息节点类* host,
        const std::string& 主键,
        const std::string& 调用点)
    {
        if (!host || 主键.empty() || !host->子) return std::nullopt;
        数据仓库模块::锁调度器守卫 锁({
            数据仓库模块::锁请求::读(
                数据仓库模块::世界链.链表锁,
                数据仓库模块::枚举_锁域::世界链,
                "世界链",
                调用点)
        });
        auto* first = static_cast<基础信息节点类*>(host->子);
        auto* it = first;
        std::optional<特征快照值> found = std::nullopt;
        do {
            auto* mi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
            if (mi && mi->类型 && mi->类型->获取主键() == 主键) {
                found = mi->当前快照;
            }
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);
        return found;
    }

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
        停止请求中 = 3,
        收尾中 = 4,
        已停止 = 5,
        故障 = 6
    };

    struct 结构_学习推进结果 {
        bool 有进展 = false;
        bool 需要唤醒等待任务 = false;
        方法节点类* 目标方法首节点 = nullptr;
        bool 观察成功 = false;
        bool 缺条件 = false;
        bool 缺结果 = false;
        I64 候选数量 = 0;
        I64 更新数量 = 0;
        I64 新建数量 = 0;
        场景节点类* 结果场景 = nullptr;
        std::string 动作键{};
    };

    struct 结构_等待学习兜底回归结果 {
        bool 成功 = false;
        bool 已识别等待学习请求 = false;
        bool 已选中等待学习任务 = false;
        bool 已降级到基础尝试学习 = false;
        bool 唤醒键一致 = false;
        bool 学习任务优先级已继承 = false;
        bool 已唤醒等待任务 = false;
        std::string 错误摘要{};
    };

    struct 结构_任务执行转学习回归结果 {
        bool 成功 = false;
        bool 已通过任务执行器转入学习 = false;
        bool 已写等待学习标记 = false;
        bool 已写任务执行流水账 = false;
        bool 已写任务失败账 = false;
        bool 已写任务方法结果账 = false;
        bool 已写任务转学习账 = false;
        bool 已写显式学习请求 = false;
        bool 学习任务优先级已继承 = false;
        bool 已切到等待任务基础尝试学习 = false;
        bool 唤醒键保持来源方法 = false;
        bool 已唤醒原任务 = false;
        std::string 错误摘要{};
    };

    struct 结构_学习效果回归结果 {
        bool 成功 = false;
        bool 首次学习有进展 = false;
        bool 首次已补齐条件和结果 = false;
        bool 首次已进入尝试中 = false;
        bool 三次观察均成功 = false;
        bool 连续稳定次数达标 = false;
        bool 已标记尝试完成 = false;
        std::string 错误摘要{};
    };

    struct 结构_叶子执行学习修复回归结果 {
        bool 成功 = false;
        bool 已识别原方法不可执行 = false;
        bool 已通过学习兜底补出可执行方法 = false;
        bool 已切换到新方法 = false;
        bool 叶子执行成功 = false;
        std::string 错误摘要{};
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
    std::uint32_t 致命错误计数() const;
    bool 是否请求安全重启() const;
    void 请求退出();
    static 结构_等待学习兜底回归结果 测试_等待学习任务触发基础尝试学习();
    static 结构_任务执行转学习回归结果 测试_任务执行器转入学习并唤醒等待任务();
    static 结构_学习效果回归结果 测试_尝试学习能够沉淀方法效果();
    static 结构_叶子执行学习修复回归结果 测试_叶子执行遇到无效方法时可通过学习兜底修复();
private:
    bool 私有_当前线程属于受管线程() const;
    enum class 枚举_学习任务阶段 : std::uint8_t {
        空闲 = 0,
        采样 = 1,
        更新 = 2,
        验证 = 3,
        提交 = 4,
        回滚 = 5
    };

    struct 结构_学习候选方法 {
        方法节点类* head = nullptr;
        方法节点类* firstCondition = nullptr;
        bool 缺条件 = false;
        bool 缺结果 = false;
        bool 是基础尝试学习 = false;
        bool 来自等待学习任务 = false;
        任务节点类* 来源任务头结点 = nullptr;
        任务节点类* 来源步骤结点 = nullptr;
        方法节点类* 唤醒方法首节点 = nullptr;
        std::string 动作键{};
    };

    struct 结构_等待学习任务 {
        任务节点类* 任务头结点 = nullptr;
        任务节点类* 步骤结点 = nullptr;
        方法节点类* 等待方法首节点 = nullptr;
        std::int64_t 来源优先级 = (std::numeric_limits<std::int64_t>::min)();
        时间戳 创建时间 = (std::numeric_limits<时间戳>::max)();
    };

    struct 结构_学习目标快照 {
        I64 可用方法数 = 0;
        I64 稳定方法数 = 0;
        I64 稳定度总分 = 0;
        I64 待进化方法数 = 0;
    };

    struct 结构_学习工作流状态 {
        枚举_学习任务阶段 当前阶段 = 枚举_学习任务阶段::空闲;
        结构_学习候选方法 当前候选{};
        bool 上次有进展 = false;
        bool 上次观察成功 = false;
        bool 需要唤醒等待任务 = false;
        I64 候选数量 = 0;
        I64 更新数量 = 0;
        I64 新建数量 = 0;
        结构_学习目标快照 基线目标{};
        结构_学习目标快照 当前目标{};
        结构_学习任务进化结果_v0 最近进化结果{};
        场景节点类* 最近结果场景 = nullptr;
        时间戳 最近阶段时间 = 0;
    };

    enum class 枚举_致命异常动作 : std::uint8_t {
        安全停机 = 0,
        安全停机并请求重启 = 1
    };

    std::chrono::milliseconds 心跳周期_{ 200 };

    // ==== 世界树指针（运行时填充）====
    存在节点类* 自我指针 = nullptr;
    时间戳 下次服务值读取日志时间_ = 0;
    std::uint64_t 主线程维护轮次_ = 0;

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
    std::atomic<std::uint32_t> 致命错误计数_{ 0 };
    std::atomic<bool> 已请求安全重启_{ false };
    概念引擎类 概念引擎_{};
    std::shared_ptr<新存在创建通知桥> 新存在创建通知桥_{ std::make_shared<新存在创建通知桥>() };

    // 1240 局部顺序（本组件默认单锁化）：
    // 1. 生命周期/状态锁：故障锁_、新存在创建通知桥::锁_、mtx_
    // 2. 队列锁：执行队列锁_、学习队列锁_、延迟任务事实锁_
    // 3. 结构调整锁：根任务结构调整锁_
    // 条件变量配对：
    // - cv_ <-> mtx_
    // - 执行队列cv_ <-> 执行队列锁_
    // - 学习队列cv_ <-> 学习队列锁_
    // - 新存在创建通知桥::条件_ <-> 新存在创建通知桥::锁_
    // 默认禁止多把线程锁同持；根任务结构调整锁_ 是受限例外，只能单独持有，
    // 持有期间不得再拿队列锁/故障锁，也不得等待任何条件变量。
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

    struct 待写回任务事实项 {
        任务信息基类* 目标信息 = nullptr;
        std::string 键{};
        bool 是指针 = false;
        I64 I64值 = 0;
        std::uintptr_t 指针值 = 0;
        时间戳 时间 = 0;
    };
    std::mutex 延迟任务事实锁_{};
    std::vector<待写回任务事实项> 延迟任务事实队列_{};

    // 可选：限制队列膨胀（0=不限制，由内存决定）
    std::size_t 最大待执行队列长度_ = 0;
    // 可选：每次心跳最多派发多少个任务（避免一次心跳锁太久）
    std::size_t 每次心跳最大派发数_ = 256;

    // 统计：当前正在“工作线程”里执行的任务数（仅用于观测）
    std::atomic<std::size_t> 正在执行线程数_{ 0 };
    // 1240/1210 例外边界：该锁只作为“根任务结构调整门”，用于串行化根任务分解与推进一步。
    // 它允许进入任务/需求链路径，但必须单独持有，禁止与任何线程队列锁同持，也禁止在持有期间等待条件变量。
    std::mutex 根任务结构调整锁_{};
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
    任务节点类* 学习尝试任务节点_ = nullptr;
    任务节点类* 学习采样步骤节点_ = nullptr;
    任务节点类* 学习更新步骤节点_ = nullptr;
    任务节点类* 学习验证步骤节点_ = nullptr;
    任务节点类* 学习提交步骤节点_ = nullptr;
    任务节点类* 学习回滚步骤节点_ = nullptr;
    结构_学习工作流状态 学习工作流状态_{};

    二次特征节点类* 安全目标判定节点_ = nullptr;
    二次特征节点类* 服务目标判定节点_ = nullptr;

    std::int64_t 目标安全值I64_ = 0;
    std::int64_t 目标服务值I64_ = 0;
    std::int64_t 待学习方法数量_ = 0;
    场景节点类* 人类输入桥输入场景_ = nullptr;
    时间戳 人类输入桥最近处理时间_ = 0;
    I64 人类输入桥最近处理序号_ = 0;
    场景节点类* 外设观察桥输入场景_ = nullptr;
    场景节点类* 外设观察桥输出场景_ = nullptr;
    场景节点类* 动作线程输出缓存场景_ = nullptr;
    场景节点类* 周期维护输入缓存场景_ = nullptr;
    场景节点类* 周期维护输出缓存场景_ = nullptr;
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
    void 私有_扫描任务树并尝试入队(任务节点类* 根任务节点, const std::string& 标签, std::size_t* 剩余预算 = nullptr);
    void 私有_刷新根任务就绪状态();
    void 私有_确保任务已引导筹办(任务节点类* 任务节点, const std::string& 标签);
    static const char* 私有_学习阶段标签(枚举_学习任务阶段 阶段) noexcept;
    任务节点类* 私有_取尝试学习任务节点() const noexcept;
    任务头结点信息* 私有_取尝试学习任务头信息() const noexcept;
    void 私有_确保学习任务分支节点(时间戳 now);
    任务节点类* 私有_取学习阶段步骤节点(枚举_学习任务阶段 阶段) const noexcept;
    void 私有_确保学习任务阶段节点(时间戳 now);
    void 私有_切换学习任务阶段(枚举_学习任务阶段 阶段, 时间戳 now, const std::string& 标签);
    void 私有_结束学习当前阶段(bool 成功, 时间戳 now, const std::string& 标签);
    void 私有_重置学习工作流(const std::string& 标签, 时间戳 now = 0);
    结构_学习目标快照 私有_采样学习目标快照() const;
    结构_学习任务进化结果_v0 私有_刷新学习任务进化目标(
        const 结构_学习目标快照* 基线,
        bool 有待处理请求,
        时间戳 now,
        const std::string& 标签);
    结构_学习候选方法 私有_选择学习候选方法() const;
    bool 私有_推进学习采样阶段(时间戳 now);
    bool 私有_推进学习验证阶段(时间戳 now);
    void 私有_推进学习提交阶段(时间戳 now);
    void 私有_推进学习回滚阶段(时间戳 now);
    需求节点类* 私有_查找直属子需求_按类型(需求节点类* 父需求, const 词性节点类* 需求类型) const;
    任务节点类* 私有_查找首个步骤节点(任务节点类* 任务节点) const;
    结构_等待学习任务 私有_选择等待学习任务() const;
    bool 私有_存在等待学习任务() const;
    bool 私有_任务存在活跃直属子任务(任务节点类* 任务节点) const;
    bool 私有_学习任务有待处理请求() const;
    std::optional<I64> 私有_读取任务事实I64(任务节点类* 任务节点, const std::string& 键) const;
    std::optional<std::uintptr_t> 私有_读取任务事实指针(任务节点类* 任务节点, const std::string& 键) const;
    void 私有_排队任务事实I64(任务信息基类* info, const std::string& 键, I64 值, 时间戳 now);
    void 私有_排队任务事实指针(任务信息基类* info, const std::string& 键, std::uintptr_t ptr, 时间戳 now);
    void 私有_冲刷延迟任务事实队列(const std::string& 调用点, 时间戳 now = 0);
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
    I64 私有_读任务事实I64(const 任务信息基类* info, const std::string& 键) const;
    I64 私有_读任务方向I64(const 任务信息基类* info, const std::string& 键) const;
    结构_结构B因果预测结果 私有_估计任务因果预测(任务节点类* 任务节点, bool 作为学习任务) const;
    void 私有_刷新基础方向概念与需求(const std::string& 调用点);
    bool 私有_任务目标命中(const 任务信息基类& t) const;
    bool 私有_任务可调度(const 任务信息基类& t) const;
    void 私有_注册已有本能方法并更新待学习数量(时间戳 now);
    std::int64_t 私有_统计待学习方法数量() const;
    static bool 私有_当前阶段停用学习线程();
    static bool 私有_是当前阶段禁用的主动相机相关本能(枚举_本能动作ID 本能ID);
    bool 私有_方法允许参与当前阶段试探与学习(方法节点类* 方法首节点) const;
    I64 私有_判定方法尝试状态(方法节点类* 方法首节点, bool* outHasCondition = nullptr, bool* outHasResult = nullptr) const;
    void 私有_更新方法尝试状态_按观察(方法节点类* 方法首节点, 场景节点类* 观察输出, bool 观察成功, 时间戳 now);
    场景节点类* 私有_查找外设观察桥场景(const char* 名称) const;
    场景节点类* 私有_确保内部缓存场景(
        场景节点类*& 缓存场景,
        const char* 名称,
        const char* 类型名,
        const std::string& 调用点);
    场景节点类* 私有_确保动作线程输出缓存场景(const std::string& 调用点);
    static 运行时状态主信息记录 私有_构造运行时状态记录(
        枚举_状态域 状态域,
        基础信息节点类* 状态主体,
        特征节点类* 状态特征,
        const 特征快照值& 状态值,
        bool 是否变化,
        时间戳 now,
        const std::string& 变化原因类别,
        const std::string& 变化原因说明,
        基础信息节点类* 对应信息节点 = nullptr);
    static void 私有_追加运行时状态记录(
        场景节点类* 场景,
        const 运行时状态主信息记录& 状态记录);
    运行时动态主信息记录* 记录自我动作动态(
        const 词性节点类* 动作词,
        场景节点类* 输入场景 = nullptr,
        场景节点类* 输出场景 = nullptr,
        bool 成功 = true,
        std::int64_t 错误码 = 0,
        时间戳 开始时间 = 0,
        时间戳 结束时间 = 0,
        枚举_动作事件相位 动作相位 = 枚举_动作事件相位::未定义,
        const std::string& 调用点 = "自我线程类::记录自我动作动态",
        const 运行时状态主信息记录* 初始状态 = nullptr,
        const 运行时状态主信息记录* 结果状态 = nullptr);
    自我线程模块_detail::结构_自我内部状态快照 私有_抓取自我内部状态快照() const;
    std::vector<运行时状态主信息记录> 私有_补录自我内部状态变化(
        const 自我线程模块_detail::结构_自我内部状态快照& 前态,
        时间戳 now,
        const std::string& 变化原因类别,
        const std::string& 变化原因说明,
        const std::string& 调用点);
    void 私有_刷新动作动态派生信息(
        运行时动态主信息记录* 动态,
        时间戳 now,
        const std::string& 调用点);
    void 私有_附加状态到动作动态(
        运行时动态主信息记录* 动态,
        const std::vector<运行时状态主信息记录>& 状态列表,
        const 运行时状态主信息记录* 主结果状态 = nullptr);
    void 私有_消费前端人类输入桥();
    void 私有_为新前端输入复位服务阶段(I64 输入序号, 时间戳 now);
    void 私有_消费外设观察桥观察();
    结构_学习推进结果 私有_尝试通过兜底学习等待任务(const 结构_学习候选方法& 候选, 时间戳 now);
    结构_学习推进结果 私有_尝试学习一步(时间戳 now, const 结构_学习候选方法* 指定候选 = nullptr);
    void 私有_唤醒等待学习的任务(方法节点类* 方法首节点, const std::string& 标签);
    std::vector<方法节点类*> 私有_生成根任务最小试探方法候选(const std::string& 标签, 时间戳 now);
    bool 私有_尝试入队动作任务(任务节点类* 任务节点, const std::string& 标签);
    任务执行器::结构_叶子执行结果 私有_执行任务叶子步骤(任务节点类* 步骤节点);
    void 私有_同步方法事实到任务方向特征(任务节点类* 步骤节点, 枚举_本能动作ID 本能ID, 场景节点类* 输出场景, bool 执行成功, I64 回执错误码, 时间戳 now);
    void 私有_刷新初始化完成状态();
    void 私有_清空故障摘要();
    void 私有_重置异常策略状态();
    void 私有_注册新存在创建通知();
    void 私有_注销新存在创建通知();
    void 置故障(const std::string& 摘要);
    void 私有_登记致命线程异常(
        const char* 线程标签,
        const std::string& 上下文,
        const std::string& 异常摘要,
        枚举_致命异常动作 动作);
    bool 私有_执行关键区(
        const char* 线程标签,
        const std::string& 上下文,
        const std::function<void()>& 函数,
        枚举_致命异常动作 动作 = 枚举_致命异常动作::安全停机并请求重启);

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
    void 更新需求_基于观测(const std::string& key, double 强度);
    void 记录根方向摘要(const std::string& 调用点);
    void 校验根约束不变量(const std::string& 调用点);
    void 记录需求树不变量摘要(const std::string& 调用点);
    需求节点类* 私有_回溯根需求_已加锁(需求节点类* 需求) const;
    bool 私有_是否叶子需求_已加锁(需求节点类* 需求) const;
    枚举_需求所属根树 私有_判定需求所属根树_已加锁(需求节点类* 需求) const;
    double 私有_计算叶子需求结算总权值_已加锁(需求节点类* 叶子需求) const;
    I64 私有_结算总权值转增量(double 结算总权值) const;
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

inline std::uint32_t 自我线程类::致命错误计数() const {
    return 致命错误计数_.load();
}

inline bool 自我线程类::是否请求安全重启() const {
    return 已请求安全重启_.load();
}

inline bool 自我线程类::私有_当前线程属于受管线程() const {
    const auto 当前线程ID = std::this_thread::get_id();
    return (self_thread_.joinable() && self_thread_.get_id() == 当前线程ID)
        || (动作线程_.joinable() && 动作线程_.get_id() == 当前线程ID)
        || (学习线程_.joinable() && 学习线程_.get_id() == 当前线程ID);
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

inline void 自我线程类::私有_重置异常策略状态() {
    致命错误计数_.store(0);
    已请求安全重启_.store(false);
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
    日志::异常_致命("[自我线程][故障] " + 故障摘要);
    日志::运行_错误("[自我线程][故障] " + 故障摘要);
}

inline void 自我线程类::私有_登记致命线程异常(
    const char* 线程标签,
    const std::string& 上下文,
    const std::string& 异常摘要,
    枚举_致命异常动作 动作) {
    const auto 次数 = 致命错误计数_.fetch_add(1) + 1;
    const std::string 可见线程标签 = (线程标签 && *线程标签) ? std::string(线程标签) : std::string("未命名线程");
    const std::string 可见上下文 = 上下文.empty() ? std::string("未命名上下文") : 上下文;
    const std::string 可见异常摘要 = 异常摘要.empty() ? std::string("未知异常") : 异常摘要;
    const bool 请求重启 = 动作 == 枚举_致命异常动作::安全停机并请求重启;
    if (请求重启) {
        已请求安全重启_.store(true);
    }

    const std::string 处置动作文本 = 请求重启 ? "安全停机并请求重启" : "安全停机";
    const std::string 故障摘要 =
        "[自我线程][致命异常] 线程=" + 可见线程标签 +
        ", 上下文=" + 可见上下文 +
        ", 摘要=" + 可见异常摘要 +
        ", 致命错误计数=" + std::to_string(次数) +
        ", 动作=" + 处置动作文本;

    置故障(故障摘要);
    日志::运行_错误("[自我线程][上报] " + 故障摘要);
    if (请求重启) {
        日志::运行("[自我线程] 已标记安全重启请求，等待受控停机完成");
    }
    请求退出();
}

inline bool 自我线程类::私有_执行关键区(
    const char* 线程标签,
    const std::string& 上下文,
    const std::function<void()>& 函数,
    枚举_致命异常动作 动作) {
    try {
        if (函数) {
            函数();
        }
        return true;
    }
    catch (const std::exception& e) {
        日志::记录异常(e, 上下文);
        私有_登记致命线程异常(线程标签, 上下文, e.what(), 动作);
    }
    catch (...) {
        const auto 可见上下文 = 上下文.empty() ? std::string("未命名上下文") : 上下文;
        日志::异常_致命("[" + 可见上下文 + "] 未知异常");
        私有_登记致命线程异常(线程标签, 上下文, "未知异常", 动作);
    }
    return false;
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
    私有_重置异常策略状态();
    {
        std::lock_guard<std::mutex> lk(mtx_);
        // 启动时投递一次“调度”消息，让系统进入事件驱动推进
        队列_.push_back(任务消息{ 消息类型::调度 });
    }

    const bool 启动成功 = 私有_执行关键区(
        "启动线程",
        "自我线程类::启动/创建线程",
        [this] {
            概念引擎_ = 概念引擎类{};

            self_thread_ = std::thread([this] {
                主线程存活_.store(true);
                运行状态_.store(枚举_自我运行状态::初始化中);

                // 线程启动日志
                日志::运行("[自我线程] 线程启动");

                const bool 初始化成功 = 私有_执行关键区(
                    "主线程",
                    "自我线程类::主线程/初始化",
                    [this] {
                        自我初始化();
                        私有_刷新初始化完成状态();
                    });
                if (初始化成功 && !stop_.load()) {
                    私有_注册新存在创建通知();
                    日志::运行("[自我线程] 当前阶段停用动作线程/学习线程自动启动，主循环仅保留最小维护");
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
                            自我核心_.获取安全值(),
                            自我核心_.获取服务值());
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
                        const bool 消息处理成功 = 私有_执行关键区(
                            "主线程",
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
                            break;
                        }
                    }

                    const bool 维护成功 = 私有_执行关键区(
                        "主线程",
                        "自我线程类::主线程/维护与调度",
                        [this] {
                            const auto 当前轮次 = ++主线程维护轮次_;
                            const 时间戳 维护开始 = 结构体_时间戳::当前_微秒();
                            std::size_t 待执行消息数 = 0;
                            {
                                std::lock_guard<std::mutex> lk(mtx_);
                                待执行消息数 = 队列_.size();
                            }
                            日志::运行f(
                                "[自我线程][调试] 主线程维护开始: 轮次={}, 待执行消息数={}, stop={}",
                                static_cast<unsigned long long>(当前轮次),
                                static_cast<unsigned long long>(待执行消息数),
                                stop_.load() ? 1 : 0);
                            // 低频维护：摘要/衰减等（不驱动任务执行）
                            周期性维护();
                            const 时间戳 维护结束 = 结构体_时间戳::当前_微秒();
                            日志::运行f(
                                "[自我线程][调试] 周期性维护完成: 轮次={}, 耗时={}us",
                                static_cast<unsigned long long>(当前轮次),
                                static_cast<long long>(维护结束 - 维护开始));

                            // 当前先把任务派工从自我线程主循环中摘掉。
                            // 自我线程只保留消息处理与基础本能维护，重调度后续再迁到专用工作路径。
                            static bool 已记录心跳调度停用日志 = false;
                            if (!已记录心跳调度停用日志) {
                                日志::运行(
                                    "[自我线程][调试] 心跳调度已从主线程停用：当前主循环只执行周期维护与消息处理");
                                已记录心跳调度停用日志 = true;
                            }
                        });
                    if (!维护成功) {
                        break;
                    }
                }

                主线程存活_.store(false);

                // 线程结束日志
                日志::运行("[自我线程] 线程结束");
            });
        });
    if (!启动成功) {
        等待线程结束();
    }
}

inline void 自我线程类::请求退出() {
    stop_.store(true);
    const auto 当前状态 = 运行状态_.load();
    if (当前状态 != 枚举_自我运行状态::故障 &&
        当前状态 != 枚举_自我运行状态::已停止 &&
        当前状态 != 枚举_自我运行状态::未启动) {
        运行状态_.store(枚举_自我运行状态::停止请求中);
    }

    // 唤醒消息等待
    cv_.notify_all();
    // 唤醒执行线程池等待
    执行队列cv_.notify_all();
    学习队列cv_.notify_all();
}

inline void 自我线程类::等待线程结束() {
    日志::运行("[自我线程] 等待线程结束开始");
    if (私有_当前线程属于受管线程()) {
        日志::运行("[自我线程] 当前线程属于受管线程，跳过 join，等待拥有者稍后回收");
        return;
    }
    私有_注销新存在创建通知();
    if (运行状态_.load() != 枚举_自我运行状态::故障 &&
        运行状态_.load() != 枚举_自我运行状态::未启动 &&
        运行状态_.load() != 枚举_自我运行状态::已停止) {
        运行状态_.store(枚举_自我运行状态::收尾中);
    }

    // 先等待工作线程（动作/学习）退出，避免主 self_thread 在收尾时与工作线程争用共享状态。
    const bool 工作者线程收尾成功 = 私有_执行关键区(
        "收尾链",
        "自我线程类::等待线程结束/工作线程",
        [this] {
            if (动作线程_.joinable()) {
                日志::运行("[自我线程] 等待动作线程结束开始");
                动作线程_.join();
                日志::运行("[自我线程] 等待动作线程结束完成");
            }
            if (学习线程_.joinable()) {
                日志::运行("[自我线程] 等待学习线程结束开始");
                学习线程_.join();
                日志::运行("[自我线程] 等待学习线程结束完成");
            }
        },
        枚举_致命异常动作::安全停机);
    if (!工作者线程收尾成功) {
        日志::运行_错误("[自我线程] 等待工作线程结束异常，已进入受控退出");
    }

    const bool 主线程收尾成功 = 私有_执行关键区(
        "收尾链",
        "自我线程类::等待线程结束/主线程",
        [this] {
            if (self_thread_.joinable()) {
                日志::运行("[自我线程] 等待主线程结束开始");
                self_thread_.join();
                日志::运行("[自我线程] 等待主线程结束完成");
            }
        },
        枚举_致命异常动作::安全停机);
    if (!主线程收尾成功) {
        日志::运行_错误("[自我线程] 等待主线程结束异常，已进入受控退出");
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
    if (私有_当前线程属于受管线程()) {
        日志::运行("[自我线程] 当前线程属于受管线程，本轮仅发出退出请求，由拥有者稍后 join");
        return;
    }
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
    自我核心_.初始化自我("自我线程类::自我初始化/自我核心");
    自我指针 = 世界树.自我指针;
    下次服务值读取日志时间_ = 0;
    私有_注册已有本能方法并更新待学习数量(now);
    初始化_初始需求与任务();
    私有_刷新基础方向概念与需求("自我线程类::自我初始化");
    记录根方向摘要("自我线程类::自我初始化");
    校验根约束不变量("自我线程类::自我初始化");
    记录需求树不变量摘要("自我线程类::自我初始化");
    私有_刷新初始化完成状态();

    日志::运行f("[自我线程] 自我初始化完成: self={}, 待学习方法数量={}, 安全值={}, 服务值={}, 初始化完成={}",
        (void*)自我指针,
        待学习方法数量_,
        自我核心_.获取安全值(),
        自我核心_.获取服务值(),
        初始化完成_.load() ? 1 : 0);
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
            std::unordered_set<方法节点类*> 已遍历同层{};
            while (cur && 已遍历同层.insert(cur).second) {
                fn(cur);
                cur = static_cast<方法节点类*>(cur->下);
            }
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
    return false;
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
    if (hmi->动作句柄.本能ID == 0) return true;
    return !私有_是当前阶段禁用的主动相机相关本能(
        static_cast<枚举_本能动作ID>(hmi->动作句柄.本能ID));
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
            std::unordered_set<方法节点类*> 已遍历同层{};
            while (cur && 已遍历同层.insert(cur).second) {
                fn(cur);
                cur = static_cast<方法节点类*>(cur->下);
            }
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
        (void)方法集.记录方法虚拟存在特征(
            方法首节点,
            typeLastSig,
            特征快照值{ currentSig },
            now,
            "自我线程类::更新方法尝试状态/写结果签名");

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

    (void)方法集.记录方法虚拟存在特征(
        方法首节点,
        typeSplit,
        特征快照值{ splitCount },
        now,
        "自我线程类::更新方法尝试状态/写分裂次数");
    (void)方法集.记录方法虚拟存在特征(
        方法首节点,
        typeStable,
        特征快照值{ stableCount },
        now,
        "自我线程类::更新方法尝试状态/写稳定次数");
    (void)方法集.记录方法虚拟存在特征(
        方法首节点,
        typeStatus,
        特征快照值{ status },
        now,
        "自我线程类::更新方法尝试状态/写状态");

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

inline 场景节点类* 自我线程类::私有_确保内部缓存场景(
    场景节点类*& 缓存场景,
    const char* 名称,
    const char* 类型名,
    const std::string& 调用点)
{
    if (缓存场景 && 缓存场景->主信息) {
        return 缓存场景;
    }

    auto* parent = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!parent || !名称 || !*名称 || !类型名 || !*类型名) return nullptr;

    if (auto* 已有 = 私有_查找外设观察桥场景(名称)) {
        缓存场景 = 已有;
        return 缓存场景;
    }

    auto* mi = new 场景节点主信息类();
    mi->名称 = 语素集.添加词性词(名称, "名词");
    mi->类型 = 语素集.添加词性词(类型名, "名词");
    mi->最后观测时间 = 结构体_时间戳::当前_微秒();
    缓存场景 = 世界树.创建场景(parent, mi, 调用点);
    return 缓存场景;
}

inline 场景节点类* 自我线程类::私有_确保动作线程输出缓存场景(const std::string& 调用点) {
    return 私有_确保内部缓存场景(
        动作线程输出缓存场景_,
        "自我线程_动作输出缓存",
        "任务步骤执行场景",
        调用点);
}

inline 运行时状态主信息记录 自我线程类::私有_构造运行时状态记录(
    枚举_状态域 状态域,
    基础信息节点类* 状态主体,
    特征节点类* 状态特征,
    const 特征快照值& 状态值,
    bool 是否变化,
    时间戳 now,
    const std::string& 变化原因类别,
    const std::string& 变化原因说明,
    基础信息节点类* 对应信息节点)
{
    运行时状态主信息记录 out{};
    out.状态域 = 状态域;
    out.收到时间 = now;
    out.发生时间 = now;
    out.状态主体 = 状态主体;
    out.状态特征 = 状态特征;
    out.状态值 = 状态值;
    out.对应信息节点 = 对应信息节点 ? 对应信息节点 : (状态特征 ? static_cast<基础信息节点类*>(状态特征) : 状态主体);
    out.是否变化 = 是否变化;
    out.变化原因类别 = 变化原因类别;
    out.变化原因说明 = 变化原因说明;
    return out;
}

inline void 自我线程类::私有_追加运行时状态记录(
    场景节点类* 场景,
    const 运行时状态主信息记录& 状态记录)
{
    auto* 场景信息 = (场景 && 场景->主信息)
        ? dynamic_cast<场景节点主信息类*>(场景->主信息)
        : nullptr;
    if (!场景信息) return;
    场景信息->运行时状态列表.push_back(状态记录);
}

inline 运行时动态主信息记录* 自我线程类::记录自我动作动态(
    const 词性节点类* 动作词,
    场景节点类* 输入场景,
    场景节点类* 输出场景,
    bool 成功,
    std::int64_t 错误码,
    时间戳 开始时间,
    时间戳 结束时间,
    枚举_动作事件相位 动作相位,
    const std::string& 调用点,
    const 运行时状态主信息记录* 初始状态,
    const 运行时状态主信息记录* 结果状态)
{
    if (!动作词) return nullptr;
    if (开始时间 == 0) 开始时间 = 结构体_时间戳::当前_微秒();
    if (结束时间 == 0) 结束时间 = 开始时间;
    if (结束时间 < 开始时间) std::swap(开始时间, 结束时间);

    auto* 事件场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    auto* 自我存在 = 自我指针 ? 自我指针 : 世界树.自我指针;
    if (!事件场景 || !自我存在) return nullptr;
    auto* 事件场景信息 = (事件场景 && 事件场景->主信息)
        ? dynamic_cast<场景节点主信息类*>(事件场景->主信息)
        : nullptr;
    if (!事件场景信息) return nullptr;

    auto* 动作特征 = 世界树.确保特征(
        自我存在,
        动作词,
        动作词,
        调用点 + "/动作特征");
    if (!动作特征) return nullptr;

    事件场景信息->运行时动态列表.emplace_back();
    auto* 动态信息 = &事件场景信息->运行时动态列表.back();

    auto 状态记录相同 = [](const 运行时状态主信息记录& a, const 运行时状态主信息记录& b) noexcept {
        return a.状态域 == b.状态域
            && a.收到时间 == b.收到时间
            && a.发生时间 == b.发生时间
            && a.状态主体 == b.状态主体
            && a.状态特征 == b.状态特征
            && 特征快照相等(a.状态值, b.状态值)
            && a.对应信息节点 == b.对应信息节点
            && a.是否变化 == b.是否变化
            && a.变化原因类别 == b.变化原因类别
            && a.变化原因说明 == b.变化原因说明;
    };
    auto 追加唯一状态记录 = [&](std::vector<运行时状态主信息记录>& out, const 运行时状态主信息记录* 状态) {
        if (!状态) return;
        if (std::find_if(out.begin(), out.end(), [&](const auto& 已有) { return 状态记录相同(已有, *状态); }) == out.end()) {
            out.push_back(*状态);
        }
    };
    if (初始状态) 动态信息->初始状态 = *初始状态;
    if (结果状态) 动态信息->结果状态 = *结果状态;
    动态信息->动态主体 = 自我存在;
    动态信息->动态特征 = 动作特征;
    动态信息->开始时间 = 开始时间;
    动态信息->结束时间 = 结束时间;
    追加唯一状态记录(动态信息->事件列表, 初始状态);
    追加唯一状态记录(动态信息->事件列表, 结果状态);
    追加唯一状态记录(动态信息->状态路径列表, 初始状态);
    追加唯一状态记录(动态信息->状态路径列表, 结果状态);
    动态信息->来源类型 = 枚举_动态来源类型::内部机制动作;
    动态信息->来源动作名 = 动作词;
    动态信息->来源动作相位 = 动作相位;
    动态信息->来源输入场景 = 输入场景;
    动态信息->来源输出场景 = 输出场景;
    动态信息->来源执行成功 = 成功;
    动态信息->来源错误码 = 错误码;
    {
        const std::uint64_t pack[3] = {
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(自我存在)),
            static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(动作特征)),
            0xAC7100D1ULL
        };
        动态信息->动态路径签名 = 通用函数模块::FNV1a64(pack, sizeof(pack));
    }
    return 动态信息;
}

inline 自我线程模块_detail::结构_自我内部状态快照 自我线程类::私有_抓取自我内部状态快照() const
{
    自我线程模块_detail::结构_自我内部状态快照 out{};
    out.安全值 = 自我核心_.获取安全值();
    out.服务值 = 自我核心_.获取服务值();
    out.物理安全值 = 自我核心_.获取物理安全值();
    out.风险安全值 = 自我核心_.获取风险安全值();
    out.待机状态 = 自我核心_.是否待机状态() ? 1 : 0;
    out.时序正向步长 = 自我核心_.时序正向步长();
    out.时序反向步长 = 自我核心_.时序反向步长();
    out.服务时序衰减步长 = 自我核心_.服务时序衰减步长();
    return out;
}

inline std::vector<运行时状态主信息记录> 自我线程类::私有_补录自我内部状态变化(
    const 自我线程模块_detail::结构_自我内部状态快照& 前态,
    时间戳 now,
    const std::string& 变化原因类别,
    const std::string& 变化原因说明,
    const std::string& 调用点)
{
    std::vector<运行时状态主信息记录> out{};
    auto* 自我存在 = 自我指针 ? 自我指针 : 世界树.自我指针;
    auto* 内部世界 = 自我存在 ? 世界树.取存在内部世界(自我存在) : nullptr;
    if (!自我存在 || !内部世界) return out;

    if (now == 0) now = 结构体_时间戳::当前_微秒();

    const auto* 待机状态特征 = 语素集.添加词性词("待机状态", "名词");
    const auto* 时序正向步长特征 = 语素集.添加词性词("时序正向步长", "名词");
    const auto* 时序反向步长特征 = 语素集.添加词性词("时序反向步长", "名词");
    const auto* 服务时序衰减步长特征 = 语素集.添加词性词("服务时序衰减步长", "名词");

    auto 追加状态 = [&](const 词性节点类* 特征类型, I64 前值, I64 后值, const char* 标签) {
        if (!特征类型 || 前值 == 后值) return;
        auto* 特征 = 世界树.确保特征(
            自我存在,
            特征类型,
            特征类型,
            调用点 + "/" + (标签 ? 标签 : "结果状态"));
        if (!特征) return;
        auto 状态记录 = 私有_构造运行时状态记录(
            枚举_状态域::内部状态,
            自我存在,
            特征,
            特征快照值{ 后值 },
            true,
            now,
            变化原因类别,
            变化原因说明);
        私有_追加运行时状态记录(内部世界, 状态记录);
        out.push_back(std::move(状态记录));
    };

    追加状态(特征类型定义类::类型_自我_安全值, 前态.安全值, 自我核心_.获取安全值(), "安全值");
    追加状态(特征类型定义类::类型_自我_服务值, 前态.服务值, 自我核心_.获取服务值(), "服务值");
    追加状态(特征类型定义类::类型_自我_物理安全, 前态.物理安全值, 自我核心_.获取物理安全值(), "物理安全");
    追加状态(特征类型定义类::类型_自我_风险安全, 前态.风险安全值, 自我核心_.获取风险安全值(), "风险安全");
    追加状态(待机状态特征, 前态.待机状态, 自我核心_.是否待机状态() ? 1 : 0, "待机状态");
    追加状态(时序正向步长特征, 前态.时序正向步长, 自我核心_.时序正向步长(), "时序正向步长");
    追加状态(时序反向步长特征, 前态.时序反向步长, 自我核心_.时序反向步长(), "时序反向步长");
    追加状态(服务时序衰减步长特征, 前态.服务时序衰减步长, 自我核心_.服务时序衰减步长(), "服务时序衰减步长");

    return out;
}

inline void 自我线程类::私有_附加状态到动作动态(
    运行时动态主信息记录* 动态,
    const std::vector<运行时状态主信息记录>& 状态列表,
    const 运行时状态主信息记录* 主结果状态)
{
    auto* 动态信息 = 动态;
    if (!动态信息) return;

    auto 状态记录相同 = [](const 运行时状态主信息记录& a, const 运行时状态主信息记录& b) noexcept {
        return a.状态域 == b.状态域
            && a.收到时间 == b.收到时间
            && a.发生时间 == b.发生时间
            && a.状态主体 == b.状态主体
            && a.状态特征 == b.状态特征
            && 特征快照相等(a.状态值, b.状态值)
            && a.对应信息节点 == b.对应信息节点
            && a.是否变化 == b.是否变化
            && a.变化原因类别 == b.变化原因类别
            && a.变化原因说明 == b.变化原因说明;
    };
    auto 追加唯一状态记录 = [&](std::vector<运行时状态主信息记录>& out, const 运行时状态主信息记录* 状态) {
        if (!状态) return;
        if (std::find_if(out.begin(), out.end(), [&](const auto& 已有) { return 状态记录相同(已有, *状态); }) == out.end()) {
            out.push_back(*状态);
        }
    };

    const 运行时状态主信息记录* 选中结果状态 = 主结果状态;
    if (!选中结果状态 && !状态列表.empty()) {
        选中结果状态 = &状态列表.back();
    }
    if (选中结果状态) {
        动态信息->结果状态 = *选中结果状态;
    }
    for (const auto& 状态 : 状态列表) {
        追加唯一状态记录(动态信息->事件列表, &状态);
        追加唯一状态记录(动态信息->状态路径列表, &状态);
    }

    私有_刷新动作动态派生信息(
        动态信息,
        动态信息->结束时间,
        "自我线程类::私有_附加状态到动作动态");
}

inline void 自我线程类::私有_刷新动作动态派生信息(
    运行时动态主信息记录* 动态,
    时间戳 now,
    const std::string& 调用点)
{
    auto* 动态信息 = 动态;
    if (!动态信息) return;
    if (!动态信息->结果状态.has_value()
        && 动态信息->事件列表.empty()
        && 动态信息->状态路径列表.empty()
        && !动态信息->来源输出场景) {
        return;
    }
    auto* 事件场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!事件场景) return;
    auto* 因果场景 = 事件场景;

    auto 是世界状态 = [](const std::optional<运行时状态主信息记录>& 状态) noexcept -> bool {
        return 状态.has_value() && 状态->状态域 == 枚举_状态域::世界状态;
    };
    auto 场景是外部世界 = [](const 场景节点类* 场景) noexcept -> bool {
        auto* 场景信息 = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        return 场景信息 && !场景信息->是否内部世界();
    };
    auto 列表含世界状态 = [&](const std::vector<运行时状态主信息记录>& 状态列表) noexcept -> bool {
        return std::any_of(状态列表.begin(), 状态列表.end(), [&](const 运行时状态主信息记录& 状态) {
            return 状态.状态域 == 枚举_状态域::世界状态;
            });
    };

    if (动态信息->来源输出场景
        && 场景是外部世界(动态信息->来源输出场景)
        && (是世界状态(动态信息->结果状态)
            || 是世界状态(动态信息->初始状态)
            || 列表含世界状态(动态信息->状态路径列表)
            || 列表含世界状态(动态信息->事件列表))) {
        因果场景 = 动态信息->来源输出场景;
    }
    else if (动态信息->来源输入场景
        && 场景是外部世界(动态信息->来源输入场景)
        && (是世界状态(动态信息->初始状态)
            || 列表含世界状态(动态信息->状态路径列表)
            || 列表含世界状态(动态信息->事件列表))) {
        因果场景 = 动态信息->来源输入场景;
    }

    if (now == 0) now = 结构体_时间戳::当前_微秒();
    (void)因果集.动态记录后刷新因果(
        因果场景,
        动态信息,
        now,
        调用点 + "/因果");
}

inline void 自我线程类::私有_为新前端输入复位服务阶段(I64 输入序号, 时间戳 now) {
    if (!服务任务节点_ || !服务任务节点_->主信息 || !服务需求节点_) return;

    auto* 服务头信息 = dynamic_cast<任务头结点信息*>(服务任务节点_->主信息);
    if (!服务头信息) return;

    auto 查找直属子任务_按类型 = [&](const char* 类型名) -> 任务节点类* {
        const auto* 目标类型 = 语素集.添加词性词(类型名, "名词");
        for (auto* 子任务头 : 服务头信息->直属子任务头结点列表) {
            auto* 子头信息 = (子任务头 && 子任务头->主信息) ? dynamic_cast<任务头结点信息*>(子任务头->主信息) : nullptr;
            if (子头信息 && 子头信息->类型 == 目标类型) return 子任务头;
        }
        return nullptr;
    };

    auto 复位阶段 = [&](任务节点类* 任务节点, 需求节点类* 需求节点, const std::string& 目标事实键, const std::string& 标签) {
        if (!任务节点 || !任务节点->主信息 || !需求节点) return;

        auto* 任务头信息 = dynamic_cast<任务头结点信息*>(任务节点->主信息);
        auto* 步骤节点 = 私有_查找首个步骤节点(任务节点);
        auto* 步骤信息 = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;

        auto 清零事实 = [&](任务信息基类* info) {
            if (!info || !info->任务虚拟存在) return;
            (void)任务类::写任务虚拟存在_I64特征(
                info,
                自我线程模块_detail::特征_任务事实(目标事实键),
                0,
                now,
                "自我线程类::私有_为新前端输入复位服务阶段/清零事实/" + 标签);
            (void)任务类::写任务虚拟存在_I64特征(
                info,
                自我线程模块_detail::特征_任务方向(目标事实键),
                0,
                now,
                "自我线程类::私有_为新前端输入复位服务阶段/清零方向/" + 标签);
        };

        清零事实(任务头信息);
        清零事实(步骤信息);

        if (步骤节点) {
            任务类::设置任务状态(
                步骤节点,
                枚举_任务状态::就绪,
                "自我线程类::私有_为新前端输入复位服务阶段/步骤就绪/" + 标签);
        }
        if (任务头信息) {
            (void)任务类::设置任务当前步骤(
                任务头信息,
                步骤节点,
                now,
                "自我线程类::私有_为新前端输入复位服务阶段/设置当前步骤/" + 标签);
        }
        任务类::设置任务状态(
            任务节点,
            枚举_任务状态::就绪,
            "自我线程类::私有_为新前端输入复位服务阶段/任务就绪/" + 标签);
        需求状态管理器.刷新需求及父链状态(
            需求节点,
            "自我线程类::私有_为新前端输入复位服务阶段/刷新需求/" + 标签);
        if (调度器已初始化_.load()) {
            调度器_.请求重评估任务(任务节点);
        }
    };

    auto* 读取输入任务 = 查找直属子任务_按类型("服务阶段任务_人类输入已读取");
    auto* 读取输入需求 = 私有_查找直属子需求_按类型(
        服务需求节点_,
        语素集.添加词性词("服务阶段需求_人类输入已读取", "名词"));
    auto* 确认任务 = 查找直属子任务_按类型("服务阶段任务_已确认收到");
    auto* 确认需求 = 私有_查找直属子需求_按类型(
        服务需求节点_,
        语素集.添加词性词("服务阶段需求_已确认收到", "名词"));
    auto* 补充任务 = 查找直属子任务_按类型("服务阶段任务_已请求补充信息");
    auto* 补充需求 = 私有_查找直属子需求_按类型(
        服务需求节点_,
        语素集.添加词性词("服务阶段需求_已请求补充信息", "名词"));

    复位阶段(读取输入任务, 读取输入需求, "人类输入_已读取", "读取输入");
    复位阶段(确认任务, 确认需求, "服务动作_已确认收到", "确认收到");
    复位阶段(补充任务, 补充需求, "服务动作_已请求补充信息", "请求补充");

    日志::运行f(
        "[自我线程] 前端人类输入已触发服务阶段复位: 序号={}",
        static_cast<long long>(输入序号));
}

inline void 自我线程类::私有_消费前端人类输入桥() {
    if (!人类输入桥输入场景_ || !人类输入桥输入场景_->主信息) {
        人类输入桥输入场景_ = 私有_查找外设观察桥场景("主窗口_人类输入桥输入");
    }

    auto* 输入场景 = 人类输入桥输入场景_;
    auto* 输入信息 = (输入场景 && 输入场景->主信息) ? dynamic_cast<场景节点主信息类*>(输入场景->主信息) : nullptr;
    if (!输入信息) return;

    const auto 输入序号 = 世界树.读取特征_I64(
        输入场景,
        语素集.添加词性词("人类输入_序号", "名词"),
        "自我线程类::私有_消费前端人类输入桥/读序号").value_or(0);
    const auto 输入存在 = 世界树.读取特征_I64(
        输入场景,
        语素集.添加词性词("人类输入_存在", "名词"),
        "自我线程类::私有_消费前端人类输入桥/读存在").value_or(0);

    if (输入信息->最后观测时间 <= 0 || 输入存在 <= 0 || 输入序号 <= 0) return;
    if (输入序号 == 人类输入桥最近处理序号_ && 输入信息->最后观测时间 <= 人类输入桥最近处理时间_) return;

    人类输入桥最近处理时间_ = 输入信息->最后观测时间;
    人类输入桥最近处理序号_ = 输入序号;

    日志::运行f(
        "[自我线程] 消费前端人类输入桥: 序号={}, 输入时间={}",
        static_cast<long long>(输入序号),
        static_cast<long long>(输入信息->最后观测时间));
    私有_为新前端输入复位服务阶段(输入序号, 输入信息->最后观测时间);
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

    const auto 观察结果 = 方法集.调用本能函数(枚举_本能动作ID::自我_观察场景, 输入场景, 输出场景, stop_);
    const bool ok = 观察结果.成功;
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

inline bool 自我线程类::私有_尝试入队动作任务(任务节点类* 任务节点, const std::string& 标签) {
    if (!任务节点 || !任务节点->主信息) return false;
    auto* info = 任务节点->主信息;
    if (!私有_任务可调度(*info)) return false;
    私有_刷新任务调度优先级(任务节点, 标签);

    if (调度器已初始化_.load()) {
        const auto 调制 = 调度器_.刷新任务结构B调制(
            任务节点,
            false,
            "自我线程类::私有_尝试入队动作任务/" + 标签);
        if (!调制.允许派发) {
            日志::运行f(
                "[自我线程][结构B] 动作任务被门控跳过: 标签={}, 节点={}, gate={:.3f}, ΔS={:.3f}, ΔV={:.3f}, score={:.3f}",
                标签,
                (void*)任务节点,
                调制.gate,
                调制.预测ΔS,
                调制.预测ΔV,
                调制.score);
            return false;
        }
    }

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    自我线程模块_detail::结构_任务调度动作事件守卫 入队动作(
        info,
        自我线程模块_detail::动作_动作任务入队_(),
        now,
        "自我线程类::私有_尝试入队动作任务/" + 标签);
    if (最大待执行队列长度_ > 0) {
        std::lock_guard<std::mutex> lk(执行队列锁_);
        if (执行队列_.size() >= 最大待执行队列长度_) {
            入队动作.设置结果(false, 1);
            日志::运行f("[自我线程] 动作队列已满，跳过任务入队: 标签={}, 节点={}", 标签, (void*)任务节点);
            return false;
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
    return true;
}

inline void 自我线程类::私有_同步方法事实到任务方向特征(任务节点类* 步骤节点, 枚举_本能动作ID 本能ID, 场景节点类* 输出场景, bool 执行成功, I64 回执错误码, 时间戳 now) {
    auto* stepInfo = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;
    auto* headNode = stepInfo ? stepInfo->所属任务头结点 : nullptr;
    auto* headInfo = (headNode && headNode->主信息) ? dynamic_cast<任务头结点信息*>(headNode->主信息) : nullptr;
    任务信息基类* 目标信息 = headInfo ? static_cast<任务信息基类*>(headInfo) : static_cast<任务信息基类*>(stepInfo);
    if (!stepInfo) return;

    日志::运行f(
        "[自我线程] 同步方法事实开始: 步骤={}, 任务={}, 本能ID={}, 输出={}",
        (void*)步骤节点,
        (void*)headNode,
        static_cast<unsigned long long>(本能ID),
        (void*)输出场景);

    auto 记录I64到任务 = [&](任务信息基类* info, const std::string& 键, I64 值) {
        if (!info) return;
        私有_排队任务事实I64(info, 键, 值, now);
    };

    auto 记录指针到任务 = [&](任务信息基类* info, const std::string& 键, std::uintptr_t ptr) {
        if (!info || ptr == 0) return;
        私有_排队任务事实指针(info, 键, ptr, now);
    };

    auto 写双层I64 = [&](const std::string& 键, const 词性节点类* 输出特征类型) {
        if (!输出特征类型) return;
        auto value = 世界树.读取特征_I64(输出场景, 输出特征类型, "自我线程类::私有_同步方法事实到任务方向特征/读输出/" + 键);
        if (!value.has_value()) return;
        记录I64到任务(目标信息, 键, *value);
    };

    auto 写双层指针 = [&](const std::string& 键, const 词性节点类* 输出特征类型) {
        if (!输出特征类型) return;
        auto value = 世界树.读取特征快照(输出场景, 输出特征类型, "自我线程类::私有_同步方法事实到任务方向特征/读输出指针/" + 键);
        if (!value.has_value() || !std::holds_alternative<指针句柄>(*value)) return;
        const auto ptr = std::get<指针句柄>(*value).指针;
        记录指针到任务(目标信息, 键, ptr);
    };

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

    记录I64到任务(目标信息, "最近方法本能ID", static_cast<I64>(本能ID));
    记录I64到任务(目标信息, "回执_成功", 执行成功 ? 1 : 0);
    记录I64到任务(目标信息, "回执_错误码", 回执错误码);
    日志::运行f(
        "[自我线程] 同步方法事实基础账本完成: 步骤={}, 任务={}, 本能ID={}, 目标={}",
        (void*)步骤节点,
        (void*)headNode,
        static_cast<unsigned long long>(本能ID),
        (void*)目标信息);

    switch (本能ID) {
    case 枚举_本能动作ID::自我_读取核心状态:
        // 1210: 核心状态阶段先只保留最小成功账本，扩展数值改由后续低频同步补齐。
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
    // 1210: 动作线程这里只做最小事实回写；投影/对齐等派生整理移到后续低频路径。

    日志::运行f(
        "[自我线程] 同步方法事实完成: 步骤={}, 任务={}, 本能ID={}",
        (void*)步骤节点,
        (void*)headNode,
        static_cast<unsigned long long>(本能ID));

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

    auto 选择首个可执行本能方法 = [&]() -> 方法节点类* {
        auto 方法可执行 = [](方法节点类* 候选方法) -> bool {
            auto* 候选信息 = 候选方法 ? dynamic_cast<方法首节点主信息类*>(候选方法->主信息) : nullptr;
            return 候选信息
                && 候选信息->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
                && 候选信息->动作句柄.本能ID != 0;
        };

        if (方法可执行(stepInfo->当前选中方法首节点)) {
            return stepInfo->当前选中方法首节点;
        }
        for (auto* 候选方法 : stepInfo->可用方法首节点列表) {
            if (方法可执行(候选方法)) {
                return 候选方法;
            }
        }
        return nullptr;
    };

    auto* 方法首节点 = stepInfo->当前选中方法首节点;
    if (!方法首节点) {
        方法首节点 = 选择首个可执行本能方法();
    }
    if (!方法首节点 && !stepInfo->可用方法首节点列表.empty()) {
        方法首节点 = stepInfo->可用方法首节点列表.front();
        stepInfo->当前选中方法首节点 = 方法首节点;
    }

    auto 尝试学习兜底补候选 = [&]() {
        auto* 任务头节点 = stepInfo->所属任务头结点;
        auto* 头信息 = (任务头节点 && 任务头节点->主信息)
            ? dynamic_cast<任务头结点信息*>(任务头节点->主信息)
            : nullptr;
        auto 饱和转I64 = [](I64 v) -> I64 {
            return std::clamp<I64>(v, 0, (std::numeric_limits<I64>::max)());
        };

        结构_兜底学习上下文 ctx{};
        ctx.来源需求 = 头信息 ? 头信息->需求 : nullptr;
        ctx.当前任务头结点 = 任务头节点;
        ctx.当前步骤节点 = 步骤节点;
        ctx.当前场景 = stepInfo->场景 ? stepInfo->场景 : (头信息 ? 头信息->场景 : nullptr);
        ctx.当前方法首节点 = stepInfo->当前选中方法首节点;
        ctx.安全值_Q10000 = 饱和转I64(自我核心_.获取安全值());
        ctx.服务值_Q10000 = 饱和转I64(自我核心_.获取服务值());
        ctx.需求值_Q10000 = std::max<I64>(stepInfo->基准优先级, 0);
        ctx.当前细分预算 = stepInfo->允许切换方法 ? 1 : 0;
        ctx.当前路径预算 = 1;
        ctx.当前回退预算 = (stepInfo->允许重试次数 > stepInfo->已重试次数) ? 1 : 0;
        ctx.当前检查预算 = 1;
        ctx.调用点 = "自我线程类::私有_执行任务叶子步骤/学习兜底补候选";

        if (!自我本能方法类_学习兜底模块::判断任务是否缺方法(ctx)) return;
        if (!自我本能方法类_学习兜底模块::判断是否允许进入兜底学习(ctx)) return;

        const 时间戳 nowLocal = 结构体_时间戳::当前_微秒();
        auto 候选动作集 = 自我本能方法类_学习兜底模块::生成单步候选动作集(ctx);
        for (const auto& 动作句柄 : 候选动作集) {
        if (动作句柄.类型 != 枚举_动作句柄类型::本能函数ID || 动作句柄.本能ID == 0) continue;
        const auto 本能ID = static_cast<枚举_本能动作ID>(动作句柄.本能ID);
            auto* 候选方法首节点 = 方法集.查找或创建_本能方法首节点(
                本能ID,
                nowLocal,
                "自我线程类::私有_执行任务叶子步骤/学习兜底补候选");
            if (!候选方法首节点) continue;
            方法集.初始化方法虚拟存在信息(
                候选方法首节点,
                nowLocal,
                "自我线程类::私有_执行任务叶子步骤/学习兜底补候选");
            if (std::find(stepInfo->可用方法首节点列表.begin(),
                stepInfo->可用方法首节点列表.end(),
                候选方法首节点) == stepInfo->可用方法首节点列表.end()) {
                stepInfo->可用方法首节点列表.push_back(候选方法首节点);
            }
        }

        if (auto* 候选方法 = 选择首个可执行本能方法()) {
            stepInfo->当前选中方法首节点 = 候选方法;
        }
    };

    if (!方法首节点) {
        尝试学习兜底补候选();
        方法首节点 = stepInfo->当前选中方法首节点;
        if (!方法首节点) {
            方法首节点 = 选择首个可执行本能方法();
        }
        if (!方法首节点 && !stepInfo->可用方法首节点列表.empty()) {
            方法首节点 = stepInfo->可用方法首节点列表.front();
            stepInfo->当前选中方法首节点 = 方法首节点;
        }
    }
    if (!方法首节点) {
        out.摘要 = "步骤缺少可执行方法";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::转入尝试学习;
        return out;
    }

    auto* hmi = dynamic_cast<方法首节点主信息类*>(方法首节点->主信息);
    if (!hmi || hmi->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID || hmi->动作句柄.本能ID == 0) {
        stepInfo->当前选中方法首节点 = nullptr;
        尝试学习兜底补候选();
        方法首节点 = stepInfo->当前选中方法首节点;
        if (!方法首节点) {
            方法首节点 = 选择首个可执行本能方法();
        }
        if (!方法首节点 && !stepInfo->可用方法首节点列表.empty()) {
            方法首节点 = stepInfo->可用方法首节点列表.front();
            stepInfo->当前选中方法首节点 = 方法首节点;
        }
        hmi = 方法首节点 ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息) : nullptr;
    }
    if (!hmi || hmi->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID || hmi->动作句柄.本能ID == 0) {
        out.摘要 = "当前仅支持本能方法执行";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::转入尝试学习;
        return out;
    }

    const auto 本能ID = static_cast<枚举_本能动作ID>(hmi->动作句柄.本能ID);
    if (stop_.load()) {
        out.摘要 = "停止请求中，跳过新的叶子执行";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::挂起等待;
        return out;
    }

    if (本能ID == 枚举_本能动作ID::自我_读取核心状态) {
        const 时间戳 now = 结构体_时间戳::当前_微秒();
        日志::运行f(
            "[自我线程] 叶子步骤走快速核心状态路径: 步骤={}, 方法={}",
            (void*)步骤节点,
            (void*)方法首节点);
        私有_同步方法事实到任务方向特征(步骤节点, 本能ID, nullptr, true, 0, now);
        out.成功 = true;
        out.结果场景 = nullptr;
        out.摘要 = 本能集.查询函数名称(本能ID);
        if (out.摘要.empty()) out.摘要 = "本能_自我_读取核心状态";
        out.建议动作 = 枚举_任务反馈动作::继续下一步;
        return out;
    }

    auto* 执行场景根 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    const 时间戳 now = 结构体_时间戳::当前_微秒();
    auto* 输入场景 = stepInfo->场景 ? stepInfo->场景 : 执行场景根;
    const bool 使用前端输入桥 =
        本能ID == 枚举_本能动作ID::自我_读取最新人类输入 ||
        本能ID == 枚举_本能动作ID::自我_确认收到 ||
        本能ID == 枚举_本能动作ID::自我_请求补充信息;
    if (使用前端输入桥) {
        if (!人类输入桥输入场景_ || !人类输入桥输入场景_->主信息) {
            人类输入桥输入场景_ = 私有_查找外设观察桥场景("主窗口_人类输入桥输入");
        }
        if (人类输入桥输入场景_) {
            输入场景 = 人类输入桥输入场景_;
        }
    }
    auto* 任务头节点 = stepInfo->所属任务头结点;
    if (!执行场景根) {
        out.摘要 = "缺少执行场景根";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::挂起等待;
        return out;
    }

    日志::运行f(
        "[自我线程] 叶子步骤准备输出场景: 任务={}, 步骤={}, 方法={}",
        (void*)任务头节点,
        (void*)步骤节点,
        (void*)方法首节点);
    auto* 输出场景 = 私有_确保动作线程输出缓存场景("自我线程类::私有_执行任务叶子步骤/输出场景缓存");
    if (!输出场景) {
        out.摘要 = "无法创建方法输出场景";
        out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        out.建议动作 = 枚举_任务反馈动作::挂起等待;
        return out;
    }
    if (auto* 输出信息 = 输出场景->主信息 ? dynamic_cast<场景节点主信息类*>(输出场景->主信息) : nullptr) {
        输出信息->最后观测时间 = now;
    }
    日志::运行f(
        "[自我线程] 叶子步骤输出场景就绪: 输出={}",
        (void*)输出场景);

    日志::运行f(
        "[自我线程] 叶子步骤准备执行: 任务={}, 步骤={}, 方法={}, 本能ID={}",
        (void*)任务头节点,
        (void*)步骤节点,
        (void*)方法首节点,
            static_cast<unsigned long long>(hmi->动作句柄.本能ID));

    const 时间戳 执行开始时间 = 结构体_时间戳::当前_微秒();
    const auto 调用结果 = 方法集.调用本能函数(本能ID, 输入场景, 输出场景, stop_, 方法首节点, 执行场景根);
    const bool 成功 = 调用结果.成功;
    const 时间戳 执行结束时间 = 结构体_时间戳::当前_微秒();
    const std::int64_t 方法耗时微秒 = (执行结束时间 >= 执行开始时间) ? static_cast<std::int64_t>(执行结束时间 - 执行开始时间) : 0;
    日志::运行f(
        "[自我线程] 叶子步骤执行返回: 任务={}, 步骤={}, 方法={}, 成功={}, 耗时微秒={}",
        (void*)任务头节点,
        (void*)步骤节点,
        (void*)方法首节点,
        成功 ? 1 : 0,
        static_cast<long long>(方法耗时微秒));
    const I64 本轮回执错误码 = 成功 ? 0 : static_cast<I64>(枚举_任务失败类型::无叶子执行回调);
    私有_同步方法事实到任务方向特征(步骤节点, 本能ID, 输出场景, 成功, 本轮回执错误码, now);

    out.成功 = 成功;
    out.结果场景 = 输出场景;
    out.摘要 = 本能集.查询函数名称(本能ID);
    if (out.摘要.empty()) out.摘要 = std::string("本能_") + std::to_string(hmi->动作句柄.本能ID);
    out.建议动作 = 成功 ? 枚举_任务反馈动作::继续下一步 : 枚举_任务反馈动作::转入尝试学习;
    if (!成功) out.失败类型 = 枚举_任务失败类型::无叶子执行回调;
    return out;
}

inline 自我线程类::结构_学习推进结果 自我线程类::私有_尝试通过兜底学习等待任务(const 结构_学习候选方法& 候选, 时间戳 now) {
    结构_学习推进结果 out{};
    auto* 来源任务头 = 候选.来源任务头结点;
    auto* 来源步骤 = 候选.来源步骤结点;
    auto* 头信息 = (来源任务头 && 来源任务头->主信息)
        ? dynamic_cast<任务头结点信息*>(来源任务头->主信息)
        : nullptr;
    auto* 步骤信息 = (来源步骤 && 来源步骤->主信息)
        ? dynamic_cast<任务步骤节点信息*>(来源步骤->主信息)
        : nullptr;
    if (!来源任务头 || !头信息) {
        return out;
    }

    结构_兜底学习上下文 ctx{};
    ctx.来源需求 = 头信息->需求;
    ctx.当前任务头结点 = 来源任务头;
    ctx.当前步骤节点 = 来源步骤;
    ctx.当前场景 = 步骤信息
        ? 步骤信息->场景
        : (头信息->场景
            ? 头信息->场景
            : (世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界));
    ctx.任务虚拟存在 = 头信息->任务虚拟存在;
    ctx.当前方法首节点 = 步骤信息 ? 步骤信息->当前选中方法首节点 : nullptr;
    if (!ctx.当前方法首节点 && 步骤信息 && !步骤信息->可用方法首节点列表.empty()) {
        ctx.当前方法首节点 = 步骤信息->可用方法首节点列表.front();
    }
    if (候选.是基础尝试学习 && ctx.当前方法首节点 == 候选.head) {
        ctx.当前方法首节点 = nullptr;
    }
    auto 规范化非负I64 = [](I64 v) -> I64 {
        return std::clamp<I64>(v, 0, (std::numeric_limits<I64>::max)());
    };
    ctx.安全值_Q10000 = 规范化非负I64(自我核心_.获取安全值());
    ctx.服务值_Q10000 = 规范化非负I64(自我核心_.获取服务值());
    ctx.需求值_Q10000 = std::max<I64>(头信息->调度优先级, 0);
    ctx.当前细分预算 = (步骤信息 && 步骤信息->允许切换方法) ? 1 : 0;
    ctx.当前路径预算 = 1;
    ctx.当前回退预算 = (步骤信息 && 步骤信息->允许重试次数 > 步骤信息->已重试次数) ? 1 : 0;
    ctx.当前检查预算 = 1;
    ctx.允许等待学习任务参与兜底 = true;
    ctx.调用点 = "自我线程类::私有_尝试通过兜底学习等待任务";

    if (!自我本能方法类_学习兜底模块::判断是否允许进入兜底学习(ctx)) {
        日志::运行f(
            "[自我线程] 跳过等待学习任务兜底: 任务={}, 步骤={}, 原因=当前上下文不允许兜底学习",
            (void*)来源任务头,
            (void*)来源步骤);
        return out;
    }

    自我本能方法类_学习兜底模块::计算本轮学习预算(ctx);
    auto 最小步骤集 = 自我本能方法类_学习兜底模块::降解任务为最小可试探步骤(ctx);
    if ((!ctx.当前步骤节点 || !ctx.当前步骤节点->主信息) && !最小步骤集.empty()) {
        auto* 首个步骤 = 最小步骤集.front();
        for (auto* 节点 : 最小步骤集) {
            if (节点 && 节点->主信息 && dynamic_cast<任务步骤节点信息*>(节点->主信息)) {
                首个步骤 = 节点;
                break;
            }
        }
        ctx.当前步骤节点 = 首个步骤;
        步骤信息 = (ctx.当前步骤节点 && ctx.当前步骤节点->主信息)
            ? dynamic_cast<任务步骤节点信息*>(ctx.当前步骤节点->主信息)
            : nullptr;
        if (步骤信息) {
            if (!ctx.当前场景) ctx.当前场景 = 步骤信息->场景;
            if (!ctx.当前方法首节点 && !步骤信息->可用方法首节点列表.empty()) {
                ctx.当前方法首节点 = 步骤信息->可用方法首节点列表.front();
            }
        }
    }

    auto 候选动作集 = 自我本能方法类_学习兜底模块::生成单步候选动作集(ctx);
    if (候选动作集.empty()) {
        日志::运行f(
            "[自我线程] 等待学习任务兜底未生成候选动作: 任务={}, 步骤={}",
            (void*)来源任务头,
            (void*)ctx.当前步骤节点);
        return out;
    }

    auto 选择动作 = 自我本能方法类_学习兜底模块::选择下一试探动作(ctx, 候选动作集);
    if (!选择动作.有效()) {
        日志::运行f(
            "[自我线程] 等待学习任务兜底未选出动作: 任务={}, 步骤={}, 候选数={}",
            (void*)来源任务头,
            (void*)ctx.当前步骤节点,
            static_cast<long long>(候选动作集.size()));
        return out;
    }

    if (选择动作.类型 == 枚举_动作句柄类型::本能函数ID && 选择动作.本能ID != 0) {
        ctx.当前方法首节点 = 方法集.查找或创建_本能方法首节点(
            static_cast<枚举_本能动作ID>(选择动作.本能ID),
            now,
            "自我线程类::私有_尝试通过兜底学习等待任务/候选动作转方法");
        if (ctx.当前方法首节点) {
            方法集.初始化方法虚拟存在信息(
                ctx.当前方法首节点,
                now,
                "自我线程类::私有_尝试通过兜底学习等待任务/初始化候选动作方法");
        }
    }

    const auto 返回消息 = 自我本能方法类_学习兜底模块::执行单步试探动作(ctx, 选择动作);
    (void)自我本能方法类_学习兜底模块::登记控制因果(ctx, 返回消息);

    const bool 通过 = 自我本能方法类_学习兜底模块::验证局部结果(ctx, 返回消息);
    方法节点类* 提炼方法首节点 = nullptr;
    if (通过) {
        提炼方法首节点 = 自我本能方法类_学习兜底模块::从成功片段提炼临时方法(ctx);
    }
    else {
        (void)自我本能方法类_学习兜底模块::回滚最近一步(ctx);
    }

    待学习方法数量_ = 私有_统计待学习方法数量();
    if (自我指针 && 特征类型定义类::类型_自我_待学习方法数量) {
        (void)世界树.写入特征_I64(
            自我指针,
            特征类型定义类::类型_自我_待学习方法数量,
            待学习方法数量_,
            {},
            "自我线程类::私有_尝试通过兜底学习等待任务/待学习方法数量");
    }

    out.目标方法首节点 = 提炼方法首节点 ? 提炼方法首节点 : ctx.当前方法首节点;
    out.有进展 = 通过 && (提炼方法首节点 != nullptr || 返回消息.类型 == 枚举_任务返回消息类型::步骤完成);
    out.需要唤醒等待任务 = out.有进展;
    out.观察成功 = 通过;
    out.候选数量 = static_cast<I64>(候选动作集.size());
    out.更新数量 = 提炼方法首节点 ? 1 : 0;
    out.新建数量 = 提炼方法首节点 ? 1 : 0;
    out.动作键 = 候选.动作键;

    日志::运行f(
        "[自我线程] 等待学习任务兜底执行完成: 任务={}, 步骤={}, 选中动作={}, 提炼方法={}, 验证通过={}, 剩余待学习方法数量={}",
        (void*)来源任务头,
        (void*)ctx.当前步骤节点,
        static_cast<unsigned long long>(选择动作.本能ID),
        (void*)提炼方法首节点,
        通过 ? 1 : 0,
        static_cast<long long>(待学习方法数量_));
    return out;
}

inline 自我线程类::结构_学习推进结果 自我线程类::私有_尝试学习一步(时间戳 now, const 结构_学习候选方法* 指定候选) {
    结构_学习推进结果 out{};
    const auto 已选候选 = 指定候选 ? *指定候选 : 私有_选择学习候选方法();
    if (已选候选.来自等待学习任务 && 已选候选.是基础尝试学习) {
        return 私有_尝试通过兜底学习等待任务(已选候选, now);
    }
    方法节点类* targetHead = 已选候选.head;
    方法节点类* firstCondition = 已选候选.firstCondition;
    const bool missingCondition = 已选候选.缺条件;
    const bool missingResult = 已选候选.缺结果;
    const std::string actionKey = 已选候选.动作键;

    bool progressed = false;
    bool observed = false;
    I64 observedCount = 0;
    I64 updatedCount = 0;
    I64 createdCount = 0;
    场景节点类* 学习结果场景 = nullptr;

    if (targetHead) {
        auto* hmi = dynamic_cast<方法首节点主信息类*>(targetHead->主信息);
        auto* 方法存在 = 方法集.取或创建_方法虚拟存在(targetHead, now, "自我线程类::尝试学习一步/方法虚拟存在");
        auto* 学习场景根 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
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
                    目标结果状态 = 状态集.创建内部状态(
                        目标场景,
                        方法存在,
                        目标特征,
                        特征快照值{ (I64)1 },
                        枚举_存在状态事件::创建,
                        false,
                        now,
                        [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                            二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
                        },
                        "自我线程类::尝试学习一步/目标结果状态");
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

    if (hmi && 参数结果.参数场景 && hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID && hmi->动作句柄.本能ID != 0) {
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
                static_cast<unsigned long long>(hmi->动作句柄.本能ID));
            auto r = 方法集.调用本能函数(static_cast<std::uint64_t>(hmi->动作句柄.本能ID), ctx, cancel);
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
                    observedCount = static_cast<I64>(smi->状态总数());
                    updatedCount = static_cast<I64>(smi->动态列表.size());
                    createdCount = static_cast<I64>(smi->状态总数());
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
    out.观察成功 = observed;
    out.缺条件 = missingCondition;
    out.缺结果 = missingResult;
    out.候选数量 = observedCount;
    out.更新数量 = updatedCount;
    out.新建数量 = createdCount;
    out.结果场景 = 学习结果场景;
    out.动作键 = actionKey;

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
    {
        const auto 前态 = 私有_抓取自我内部状态快照();
        const 时间戳 动作开始 = 结构体_时间戳::当前_微秒();
        (void)记录自我动作动态(
            自我线程模块_detail::动作_紧急中断_(),
            nullptr,
            nullptr,
            true,
            0,
            动作开始,
            动作开始,
            枚举_动作事件相位::开始运行,
            "自我线程类::处理消息_任务/紧急中断/开始");
        自我核心_.安全值减少(1);
        const 时间戳 动作结束 = 结构体_时间戳::当前_微秒();
        auto* 结束动态 = 记录自我动作动态(
            自我线程模块_detail::动作_紧急中断_(),
            nullptr,
            nullptr,
            true,
            0,
            动作结束,
            动作结束,
            枚举_动作事件相位::完成,
            "自我线程类::处理消息_任务/紧急中断/结束");
        auto 新状态列表 = 私有_补录自我内部状态变化(
            前态,
            动作结束,
            "紧急中断",
            "紧急中断",
            "自我线程类::处理消息_任务/紧急中断/补录状态");
        私有_附加状态到动作动态(结束动态, 新状态列表);
        私有_刷新基础方向概念与需求("自我线程类::处理消息_任务/紧急中断");
        日志::运行f("[自我线程] 紧急中断影响安全值: 任务ID={}, 安全值={}, 服务值={}",
            m.任务ID,
            自我核心_.获取安全值(),
            自我核心_.获取服务值());
        break;
    }
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
        自我核心_.获取安全值(),
        自我核心_.安全根方向差值(),
        自我核心_.根目标服务值(),
        自我核心_.获取服务值(),
        自我核心_.是否待机状态() ? 1 : 0);
}

inline void 自我线程类::校验根约束不变量(const std::string& 调用点) {
    const I64 安全值 = 自我核心_.获取安全值();
    const I64 服务值 = 自我核心_.获取服务值();

    if (安全值 < 0 || 服务值 < 0) {
        日志::运行f(
            "[自我线程][不变量][违反] INV-003: 安全值/服务值不应为负数: 调用点={}, 安全值={}, 服务值={}",
            调用点,
            安全值,
            服务值);
    }

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
            now,
            [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
            },
            调用点 + "/方向目标状态/" + 需求类型名);
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
inline void 自我线程类::周期性维护() {
    constexpr 时间戳 服务值读取日志周期_微秒 = 1'000'000;
    const I64 场景安全度目标值 = ((std::numeric_limits<I64>::max)() / 5) * 4
        + ((((std::numeric_limits<I64>::max)() % 5) * 4) / 5);
    const I64 场景安全度默认值 = (std::numeric_limits<I64>::max)() / 2;
    const I64 安全前 = 自我核心_.获取安全值();
    const I64 服务前 = 自我核心_.获取服务值();
    const I64 风险安全前 = 自我核心_.获取风险安全值();
    auto* 当前场景 = 世界树.获取自我所在场景();
    const auto* 场景安全度特征类型 = 语素集.添加词性词("安全度", "名词");
    const I64 场景安全度前 = (当前场景 && 场景安全度特征类型)
        ? 世界树.读取特征_I64(
            当前场景,
            场景安全度特征类型,
            "自我线程类::周期性维护/读场景安全度").value_or(场景安全度默认值)
        : -1;
    const I64 服务衰减步长前 = 自我核心_.服务时序衰减步长();
    const I64 正向步长前 = 自我核心_.时序正向步长();
    const I64 反向步长前 = 自我核心_.时序反向步长();
    const bool 待机状态前 = 自我核心_.是否待机状态();

    I64 待完成任务数 = std::max<I64>(static_cast<I64>(正在执行线程数_.load()), 0);
    {
        std::lock_guard<std::mutex> lk(执行队列锁_);
        待完成任务数 += static_cast<I64>(执行队列_.size());
    }
    {
        std::lock_guard<std::mutex> lk(学习队列锁_);
        待完成任务数 += static_cast<I64>(学习队列_.size());
    }

    const I64 待学习任务数 = std::max<I64>(待学习方法数量_, 0);
    const 时间戳 now = 结构体_时间戳::当前_微秒();

    if (下次服务值读取日志时间_ == 0 || now >= 下次服务值读取日志时间_) {
        日志::运行f(
            "[自我线程] 定时读取自我服务值: 服务值={}, 安全值={}, 风险安全={}, 场景安全度={}, 服务衰减步长={}, 正向步长={}, 反向步长={}, 待完成={}, 待学习={}, 待机={}",
            static_cast<long long>(服务前),
            static_cast<long long>(安全前),
            static_cast<long long>(风险安全前),
            static_cast<long long>(场景安全度前),
            static_cast<long long>(服务衰减步长前),
            static_cast<long long>(正向步长前),
            static_cast<long long>(反向步长前),
            static_cast<long long>(待完成任务数),
            static_cast<long long>(待学习任务数),
            待机状态前 ? 1 : 0);
        下次服务值读取日志时间_ = now + 服务值读取日志周期_微秒;
    }

    // 当前先把自我线程收缩成“消息处理 + 最基础本能”。
    // 前端输入桥、外设观察桥、延迟事实冲刷、根任务刷新等重路径先从主循环拿掉，后续按需要迁回专用工作路径。
    const I64 中位安全值 = 自我核心_.根目标安全值() / 2;

    const bool 需要定时衰减服务值 = 服务前 > 0;
    if (需要定时衰减服务值) {
        日志::运行("[自我线程][调试] 周期维护进入分支: 定时衰减服务值");
        const auto* 动作词 = 自我线程模块_detail::动作_定时衰减服务值_();
        日志::运行("[自我线程][调试] 定时衰减服务值/步骤1: 已获取动作词");
        const auto 衰减前态 = 私有_抓取自我内部状态快照();
        日志::运行("[自我线程][调试] 定时衰减服务值/步骤2: 已抓取前态");
        const 时间戳 动作开始 = 结构体_时间戳::当前_微秒();
        std::optional<运行时状态主信息记录> 初始服务值状态{};
        if (auto* 自我存在 = 自我指针 ? 自我指针 : 世界树.自我指针) {
            if (auto* 内部世界 = 世界树.取存在内部世界(自我存在)) {
                if (auto* 服务值特征 = 世界树.确保特征(
                    自我存在,
                    特征类型定义类::类型_自我_服务值,
                    特征类型定义类::类型_自我_服务值,
                    "自我线程类::周期性维护/定时衰减服务值/确保服务值特征")) {
                    初始服务值状态 = 私有_构造运行时状态记录(
                        枚举_状态域::内部状态,
                        自我存在,
                        服务值特征,
                        特征快照值{ 服务前 },
                        false,
                        动作开始,
                        "定时衰减服务值",
                        "前置条件");
                }
            }
        }
        (void)记录自我动作动态(
            动作词,
            nullptr,
            nullptr,
            true,
            0,
            动作开始,
            动作开始,
            枚举_动作事件相位::开始运行,
            "自我线程类::周期性维护/定时衰减服务值/开始",
            初始服务值状态 ? &*初始服务值状态 : nullptr);
        日志::运行("[自我线程][调试] 定时衰减服务值/步骤3: 已记录开始动态");
        const bool 成功 = 自我核心_.应用定时衰减服务值(
            自我核心_.服务时序衰减步长(),
            now,
            "自我线程类::周期性维护/定时衰减服务值",
            nullptr);
        std::optional<运行时状态主信息记录> 主结果状态{};
        if (成功) {
            if (auto* 自我存在 = 自我指针 ? 自我指针 : 世界树.自我指针) {
                if (auto* 服务值特征 = 世界树.确保特征(
                    自我存在,
                    特征类型定义类::类型_自我_服务值,
                    特征类型定义类::类型_自我_服务值,
                    "自我线程类::周期性维护/定时衰减服务值/确保服务值特征")) {
                    主结果状态 = 私有_构造运行时状态记录(
                        枚举_状态域::内部状态,
                        自我存在,
                        服务值特征,
                        特征快照值{ 自我核心_.获取服务值() },
                        true,
                        now,
                        "定时衰减服务值",
                        {});
                }
            }
        }
        日志::运行f(
            "[自我线程][调试] 定时衰减服务值/步骤4: 已执行减值, 成功={}, 主结果状态={}",
            成功 ? 1 : 0,
            主结果状态.has_value() ? 1 : 0);
        const 时间戳 动作结束 = 结构体_时间戳::当前_微秒();
        auto* 结束动态 = 记录自我动作动态(
            动作词,
            nullptr,
            nullptr,
            成功,
            成功 ? 0 : -1,
            动作结束,
            动作结束,
            成功 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
            "自我线程类::周期性维护/定时衰减服务值/结束",
            初始服务值状态 ? &*初始服务值状态 : nullptr,
            主结果状态 ? &*主结果状态 : nullptr);
        日志::运行("[自我线程][调试] 定时衰减服务值/步骤5: 已记录结束动态");
        auto 新状态列表 = 私有_补录自我内部状态变化(
            衰减前态,
            动作结束,
            "定时衰减服务值",
            {},
            "自我线程类::周期性维护/定时衰减服务值/补录状态");
        日志::运行f(
            "[自我线程][调试] 定时衰减服务值/步骤6: 已补录状态, 数量={}",
            static_cast<long long>(新状态列表.size()));
        私有_附加状态到动作动态(结束动态, 新状态列表, 主结果状态 ? &*主结果状态 : nullptr);
        日志::运行("[自我线程][调试] 定时衰减服务值/步骤7: 已附加状态到结束动态");
        日志::运行f(
            "[自我线程][调试] 周期维护退出分支: 定时衰减服务值, 成功={}, 新状态数={}",
            成功 ? 1 : 0,
            static_cast<long long>(新状态列表.size()));
    }
    else {
        日志::运行f(
            "[自我线程] 周期维护跳过定时衰减服务值: 原因=服务值非正, 服务值={}",
            static_cast<long long>(服务前));
    }

    constexpr I64 风险安全回归步长 = 1;
    const I64 风险安全目标值 = ((std::numeric_limits<I64>::max)() / 5) * 4
        + ((((std::numeric_limits<I64>::max)() % 5) * 4) / 5);
    const I64 当前风险安全值 = 自我核心_.获取风险安全值();
    if (当前风险安全值 > 风险安全目标值) {
        日志::运行("[自我线程][调试] 周期维护进入分支: 风险安全回归");
        const auto* 动作词 = 自我线程模块_detail::动作_风险安全回归_();
        const auto 回归前态 = 私有_抓取自我内部状态快照();
        const 时间戳 动作开始 = 结构体_时间戳::当前_微秒();
        std::optional<运行时状态主信息记录> 初始风险安全状态{};
        if (auto* 自我存在 = 自我指针 ? 自我指针 : 世界树.自我指针) {
            if (auto* 内部世界 = 世界树.取存在内部世界(自我存在)) {
                if (auto* 风险安全特征 = 世界树.确保特征(
                    自我存在,
                    特征类型定义类::类型_自我_风险安全,
                    特征类型定义类::类型_自我_风险安全,
                    "自我线程类::周期性维护/风险安全回归/确保风险安全特征")) {
                    初始风险安全状态 = 私有_构造运行时状态记录(
                        枚举_状态域::内部状态,
                        自我存在,
                        风险安全特征,
                        特征快照值{ 当前风险安全值 },
                        false,
                        动作开始,
                        "风险安全回归",
                        "前置条件");
                }
            }
        }
        (void)记录自我动作动态(
            动作词,
            nullptr,
            nullptr,
            true,
            0,
            动作开始,
            动作开始,
            枚举_动作事件相位::开始运行,
            "自我线程类::周期性维护/风险安全回归/开始",
            初始风险安全状态 ? &*初始风险安全状态 : nullptr);
        const bool 成功 = 自我核心_.应用风险安全回归(
            风险安全回归步长,
            now,
            "自我线程类::周期性维护/风险安全回归",
            nullptr);
        std::optional<运行时状态主信息记录> 主结果状态{};
        if (成功) {
            if (auto* 自我存在 = 自我指针 ? 自我指针 : 世界树.自我指针) {
                if (auto* 风险安全特征 = 世界树.确保特征(
                    自我存在,
                    特征类型定义类::类型_自我_风险安全,
                    特征类型定义类::类型_自我_风险安全,
                    "自我线程类::周期性维护/风险安全回归/确保风险安全特征")) {
                    主结果状态 = 私有_构造运行时状态记录(
                        枚举_状态域::内部状态,
                        自我存在,
                        风险安全特征,
                        特征快照值{ 自我核心_.获取风险安全值() },
                        true,
                        now,
                        "风险安全回归",
                        {});
                }
            }
        }
        const 时间戳 动作结束 = 结构体_时间戳::当前_微秒();
        auto* 结束动态 = 记录自我动作动态(
            动作词,
            nullptr,
            nullptr,
            成功,
            成功 ? 0 : -1,
            动作结束,
            动作结束,
            成功 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
            "自我线程类::周期性维护/风险安全回归/结束",
            初始风险安全状态 ? &*初始风险安全状态 : nullptr,
            主结果状态 ? &*主结果状态 : nullptr);
        auto 新状态列表 = 私有_补录自我内部状态变化(
            回归前态,
            动作结束,
            "风险安全回归",
            {},
            "自我线程类::周期性维护/风险安全回归/补录状态");
        私有_附加状态到动作动态(结束动态, 新状态列表, 主结果状态 ? &*主结果状态 : nullptr);
        日志::运行f(
            "[自我线程][调试] 周期维护退出分支: 风险安全回归, 成功={}, 新状态数={}",
            成功 ? 1 : 0,
            static_cast<long long>(新状态列表.size()));
    }

    constexpr I64 场景安全度回归步长 = 1;
    if (当前场景 && 场景安全度特征类型 && 场景安全度前 >= 0 && 场景安全度前 < 场景安全度目标值) {
        日志::运行("[自我线程][调试] 周期维护进入分支: 场景安全度回归");
        const auto* 动作词 = 自我线程模块_detail::动作_场景安全度回归_();
        const 时间戳 动作开始 = 结构体_时间戳::当前_微秒();
        std::optional<运行时状态主信息记录> 初始场景安全度状态{};
        auto* 场景安全度特征 = 世界树.确保特征(
            当前场景,
            场景安全度特征类型,
            场景安全度特征类型,
            "自我线程类::周期性维护/场景安全度回归/确保特征");
        if (场景安全度特征) {
            初始场景安全度状态 = 私有_构造运行时状态记录(
                枚举_状态域::世界状态,
                当前场景,
                场景安全度特征,
                特征快照值{ 场景安全度前 },
                false,
                动作开始,
                "场景安全度回归",
                "前置条件");
        }
        (void)记录自我动作动态(
            动作词,
            nullptr,
            当前场景,
            true,
            0,
            动作开始,
            动作开始,
            枚举_动作事件相位::开始运行,
            "自我线程类::周期性维护/场景安全度回归/开始",
            初始场景安全度状态 ? &*初始场景安全度状态 : nullptr);

        const I64 场景安全度候选值 = 通用函数模块::饱和加(场景安全度前, 场景安全度回归步长);
        const I64 场景安全度新值 = (std::min<I64>)(场景安全度候选值, 场景安全度目标值);

        std::optional<运行时状态主信息记录> 主结果状态{};
        bool 成功 = false;
        if (场景安全度特征) {
            const 时间戳 结果时间 = 结构体_时间戳::当前_微秒();
            (void)世界树.写入特征_I64(
                当前场景,
                场景安全度特征类型,
                场景安全度新值,
                {},
                "自我线程类::周期性维护/场景安全度回归/写场景安全度");
            主结果状态 = 私有_构造运行时状态记录(
                枚举_状态域::世界状态,
                当前场景,
                场景安全度特征,
                特征快照值{ 场景安全度新值 },
                true,
                结果时间,
                "场景安全度回归",
                {});
            私有_追加运行时状态记录(当前场景, *主结果状态);
            成功 = true;
        }

        const 时间戳 动作结束 = 结构体_时间戳::当前_微秒();
        auto* 结束动态 = 记录自我动作动态(
            动作词,
            nullptr,
            当前场景,
            成功,
            成功 ? 0 : -1,
            动作结束,
            动作结束,
            成功 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
            "自我线程类::周期性维护/场景安全度回归/结束",
            初始场景安全度状态 ? &*初始场景安全度状态 : nullptr,
            主结果状态 ? &*主结果状态 : nullptr);
        std::vector<运行时状态主信息记录> 新状态列表{};
        if (主结果状态) 新状态列表.push_back(*主结果状态);
        私有_附加状态到动作动态(结束动态, 新状态列表, 主结果状态 ? &*主结果状态 : nullptr);
        日志::运行f(
            "[自我线程][调试] 周期维护退出分支: 场景安全度回归, 成功={}, 新状态数={}",
            成功 ? 1 : 0,
            static_cast<long long>(新状态列表.size()));
    }

    const I64 当前安全值 = 自我核心_.获取安全值();
    const I64 当前服务值 = 自我核心_.获取服务值();
    const bool 需要服务归零待机保护 =
        (当前服务值 == 0) && (当前安全值 != 1 || !自我核心_.是否待机状态());

    if (需要服务归零待机保护) {
        const bool 有变化 = 自我核心_.应用服务归零待机保护(
            now,
            "自我线程类::周期性维护/服务归零待机保护");
        日志::运行f(
            "[自我线程] 周期维护直连服务归零待机保护: changed={}, 安全值={}, 服务值={}, 待机={}",
            有变化 ? 1 : 0,
            static_cast<long long>(自我核心_.获取安全值()),
            static_cast<long long>(自我核心_.获取服务值()),
            自我核心_.是否待机状态() ? 1 : 0);
    }
    else {
        日志::运行f(
            "[自我线程] 周期维护跳过基础本能: 安全值={}, 服务值={}, 风险安全={}, 场景安全度={}, 中位安全值={}, 待完成={}, 待学习={}",
            static_cast<long long>(当前安全值),
            static_cast<long long>(当前服务值),
            static_cast<long long>(自我核心_.获取风险安全值()),
            static_cast<long long>(场景安全度前),
            static_cast<long long>(中位安全值),
            static_cast<long long>(待完成任务数),
            static_cast<long long>(待学习任务数));
    }

    const I64 安全后 = 自我核心_.获取安全值();
    const I64 服务后 = 自我核心_.获取服务值();
    const I64 风险安全后 = 自我核心_.获取风险安全值();
    const I64 场景安全度后 = (当前场景 && 场景安全度特征类型)
        ? 世界树.读取特征_I64(
            当前场景,
            场景安全度特征类型,
            "自我线程类::周期性维护/维护后读场景安全度").value_or(场景安全度前)
        : 场景安全度前;
    const I64 服务衰减步长后 = 自我核心_.服务时序衰减步长();
    const I64 正向步长后 = 自我核心_.时序正向步长();
    const I64 反向步长后 = 自我核心_.时序反向步长();
    const bool 待机状态后 = 自我核心_.是否待机状态();

    if (自我指针) {
        const auto* 待机状态特征 = 语素集.添加词性词("待机状态", "名词");
        const auto* 时序正向步长特征 = 语素集.添加词性词("时序正向步长", "名词");
        const auto* 时序反向步长特征 = 语素集.添加词性词("时序反向步长", "名词");
        const auto* 服务时序衰减步长特征 = 语素集.添加词性词("服务时序衰减步长", "名词");
        if (待机状态前 != 待机状态后 && 待机状态特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                待机状态特征,
                待机状态后 ? 1 : 0,
                {},
                "自我线程类::周期性维护/写待机状态");
        }
        if (正向步长前 != 正向步长后 && 时序正向步长特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                时序正向步长特征,
                正向步长后,
                {},
                "自我线程类::周期性维护/写时序正向步长");
        }
        if (反向步长前 != 反向步长后 && 时序反向步长特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                时序反向步长特征,
                反向步长后,
                {},
                "自我线程类::周期性维护/写时序反向步长");
        }
        if (服务衰减步长前 != 服务衰减步长后 && 服务时序衰减步长特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                服务时序衰减步长特征,
                服务衰减步长后,
                {},
                "自我线程类::周期性维护/写服务时序衰减步长");
        }
    }

    if (安全前 != 安全后 || 服务前 != 服务后 || 风险安全前 != 风险安全后 || 场景安全度前 != 场景安全度后 || 服务衰减步长前 != 服务衰减步长后 || 正向步长前 != 正向步长后 || 反向步长前 != 反向步长后) {
        日志::运行f(
            "[自我线程] 周期维护数值: 安全值 {} -> {}, 服务值 {} -> {}, 风险安全 {} -> {}, 场景安全度 {} -> {}, 服务衰减步长 {} -> {}, 正向步长 {} -> {}, 反向步长 {} -> {}, 待完成={}, 待学习={}, 待机={}",
            安全前,
            安全后,
            服务前,
            服务后,
            风险安全前,
            风险安全后,
            场景安全度前,
            场景安全度后,
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

inline I64 自我线程类::私有_结算总权值转增量(double 结算总权值) const {
    if (!std::isfinite(结算总权值) || 结算总权值 <= 0.0) {
        return 0;
    }
    return 通用函数模块::非负双精度转I64饱和(结算总权值);
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
    I64 结算增量 = 0;

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

    const I64 安全前 = 自我核心_.获取安全值();
    const I64 服务前 = 自我核心_.获取服务值();
    const I64 服务衰减步长前 = 自我核心_.服务时序衰减步长();
    const I64 正向步长前 = 自我核心_.时序正向步长();
    const I64 反向步长前 = 自我核心_.时序反向步长();
    const bool 待机状态前 = 自我核心_.是否待机状态();
    const auto 前态 = 私有_抓取自我内部状态快照();
    const auto* 结算动作词 = (所属根树 == 枚举_需求所属根树::安全)
        ? 自我线程模块_detail::动作_安全需求结算_()
        : 自我线程模块_detail::动作_服务需求结算_();
    const char* 结算动作名 = (所属根树 == 枚举_需求所属根树::安全)
        ? "安全需求结算"
        : "服务需求结算";
    const 时间戳 动作开始 = 结构体_时间戳::当前_微秒();
    (void)记录自我动作动态(
        结算动作词,
        nullptr,
        nullptr,
        true,
        0,
        动作开始,
        动作开始,
        枚举_动作事件相位::开始运行,
        调用点 + "/结算开始");

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

    const 时间戳 动作结束 = 结构体_时间戳::当前_微秒();
    auto* 结束动态 = 记录自我动作动态(
        结算动作词,
        nullptr,
        nullptr,
        true,
        0,
        动作结束,
        动作结束,
        枚举_动作事件相位::完成,
        调用点 + "/结算结束");
    auto 新状态列表 = 私有_补录自我内部状态变化(
        前态,
        动作结束,
        结算动作名,
        返回消息.摘要,
        调用点 + "/结算补录状态");
    私有_附加状态到动作动态(结束动态, 新状态列表);

    const I64 服务衰减步长后 = 自我核心_.服务时序衰减步长();
    const I64 正向步长后 = 自我核心_.时序正向步长();
    const I64 反向步长后 = 自我核心_.时序反向步长();
    const bool 待机状态后 = 自我核心_.是否待机状态();
    if (自我指针) {
        const auto* 待机状态特征 = 语素集.添加词性词("待机状态", "名词");
        const auto* 时序正向步长特征 = 语素集.添加词性词("时序正向步长", "名词");
        const auto* 时序反向步长特征 = 语素集.添加词性词("时序反向步长", "名词");
        const auto* 服务时序衰减步长特征 = 语素集.添加词性词("服务时序衰减步长", "名词");
        if (待机状态前 != 待机状态后 && 待机状态特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                待机状态特征,
                待机状态后 ? 1 : 0,
                {},
                调用点 + "/写待机状态");
        }
        if (正向步长前 != 正向步长后 && 时序正向步长特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                时序正向步长特征,
                正向步长后,
                {},
                调用点 + "/写时序正向步长");
        }
        if (反向步长前 != 反向步长后 && 时序反向步长特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                时序反向步长特征,
                反向步长后,
                {},
                调用点 + "/写时序反向步长");
        }
        if (服务衰减步长前 != 服务衰减步长后 && 服务时序衰减步长特征) {
            (void)世界树.写入特征_I64(
                自我指针,
                服务时序衰减步长特征,
                服务衰减步长后,
                {},
                调用点 + "/写服务时序衰减步长");
        }
    }
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
        自我核心_.获取安全值(),
        服务前,
        自我核心_.获取服务值(),
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
        std::unordered_set<任务节点类*> 已遍历同层{};
        while (cur && 已遍历同层.insert(cur).second) {
            if (cur && cur->主信息) {
                步骤信息 = dynamic_cast<任务步骤节点信息*>(cur->主信息);
                if (步骤信息) {
                    估计步骤节点 = cur;
                    break;
                }
            }
            cur = cur ? static_cast<任务节点类*>(cur->下) : nullptr;
        }
    }
    if (!步骤信息) return 默认方法执行时长_微秒;

    return 私有_估计步骤执行所需时间_微秒(估计步骤节点);
}

inline void 自我线程类::私有_回写任务步骤时效(任务节点类* 任务节点, 时间戳 需求截止时间, const std::string& 标签) {
    if (!任务节点 || !任务节点->主信息 || 需求截止时间 == 0) return;

    std::vector<std::pair<std::int32_t, 任务节点类*>> 步骤列表;
    {
        锁调度器守卫 锁({
            锁请求::读(任务链.链表锁, 枚举_锁域::任务链, "任务链", "自我线程类::私有_回写任务步骤时效/采样步骤/" + 标签)
        });

        auto* 起 = 任务节点->子;
        auto* it = 起;
        std::unordered_set<任务节点类*> 已遍历同层{};
        while (it && 已遍历同层.insert(static_cast<任务节点类*>(it)).second) {
            auto* 当前 = static_cast<任务节点类*>(it);
            if (当前 && 当前->主信息) {
                if (auto* 步骤信息 = dynamic_cast<任务步骤节点信息*>(当前->主信息)) {
                    步骤列表.push_back({ 步骤信息->步骤序号, 当前 });
                }
            }
            it = static_cast<任务节点类*>(it->下);
        }
    }

    if (步骤列表.empty()) return;

    std::sort(步骤列表.begin(), 步骤列表.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    std::vector<std::pair<任务节点类*, 时间戳>> 截止写回列表;
    截止写回列表.reserve(步骤列表.size());

    时间戳 当前步骤截止时间 = 需求截止时间;
    for (auto rit = 步骤列表.rbegin(); rit != 步骤列表.rend(); ++rit) {
        auto* 步骤节点 = rit->second;
        if (!步骤节点) continue;
        截止写回列表.push_back({ 步骤节点, 当前步骤截止时间 });

        const 时间戳 预计执行时长 = 私有_估计步骤执行所需时间_微秒(步骤节点);
        当前步骤截止时间 = (当前步骤截止时间 > 预计执行时长) ? (当前步骤截止时间 - 预计执行时长) : 0;
    }

    {
        锁调度器守卫 锁({
            锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", "自我线程类::私有_回写任务步骤时效/回写截止时间/" + 标签)
        });

        for (const auto& [步骤节点, 截止时间] : 截止写回列表) {
            auto* 步骤信息 = (步骤节点 && 步骤节点->主信息) ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息) : nullptr;
            if (!步骤信息) continue;
            if (步骤信息->超时截止时间 != 截止时间) {
                步骤信息->超时截止时间 = 截止时间;
            }
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

inline I64 自我线程类::私有_读任务事实I64(const 任务信息基类* info, const std::string& 键) const {
    if (!info || !info->任务虚拟存在) return 0;
    const auto* type = 自我线程模块_detail::特征_任务事实(键);
    auto 快照 = 世界树.读取特征快照(
        info->任务虚拟存在,
        type,
        "自我线程类::私有_读任务事实I64/" + 键);
    if (!快照.has_value()) {
        快照 = 自我线程模块_detail::按特征主键读取快照(
            info->任务虚拟存在,
            type->获取主键(),
            "自我线程类::私有_读任务事实I64/按主键回退/" + 键);
    }
    if (!快照.has_value() || !std::holds_alternative<I64>(*快照)) return 0;
    return std::get<I64>(*快照);
}

inline I64 自我线程类::私有_读任务方向I64(const 任务信息基类* info, const std::string& 键) const {
    if (!info || !info->任务虚拟存在) return 0;
    const auto* type = 自我线程模块_detail::特征_任务方向(键);
    auto 快照 = 世界树.读取特征快照(
        info->任务虚拟存在,
        type,
        "自我线程类::私有_读任务方向I64/" + 键);
    if (!快照.has_value()) {
        快照 = 自我线程模块_detail::按特征主键读取快照(
            info->任务虚拟存在,
            type->获取主键(),
            "自我线程类::私有_读任务方向I64/按主键回退/" + 键);
    }
    if (!快照.has_value() || !std::holds_alternative<I64>(*快照)) return 0;
    return std::get<I64>(*快照);
}

inline 结构_结构B因果预测结果 自我线程类::私有_估计任务因果预测(任务节点类* 任务节点, bool 作为学习任务) const {
    结构_结构B因果预测结果 out{};

    if (作为学习任务) {
        const double 待学习规模 = std::clamp(static_cast<double>(待学习方法数量_) / 4.0, 0.0, 3.0);
        switch (学习工作流状态_.当前阶段) {
        case 枚举_学习任务阶段::采样:
            out.预测ΔV = 0.2 + 待学习规模 * 0.2;
            out.预测ΔS = -0.1;
            out.置信度 = (待学习方法数量_ > 0) ? 0.2 : 0.0;
            break;
        case 枚举_学习任务阶段::更新:
            out.预测ΔV = 0.8 + 待学习规模;
            out.预测ΔS = (待学习方法数量_ > 0) ? -0.6 : 0.0;
            out.置信度 = (待学习方法数量_ > 0) ? 0.4 : 0.0;
            break;
        case 枚举_学习任务阶段::验证:
            out.预测ΔV = 0.5 + 待学习规模 * 0.4;
            out.预测ΔS = -0.2;
            out.置信度 = (待学习方法数量_ > 0) ? 0.45 : 0.0;
            break;
        case 枚举_学习任务阶段::提交:
            out.预测ΔV = 1.0 + 待学习规模 * 0.3;
            out.预测ΔS = 0.0;
            out.置信度 = (待学习方法数量_ > 0) ? 0.55 : 0.0;
            break;
        case 枚举_学习任务阶段::回滚:
            out.预测ΔV = 0.1;
            out.预测ΔS = 0.2;
            out.置信度 = 0.35;
            break;
        default:
            out.预测ΔV = 待学习规模;
            out.预测ΔS = (待学习方法数量_ > 0) ? -0.3 : 0.0;
            out.置信度 = (待学习方法数量_ > 0) ? 0.25 : 0.0;
            break;
        }
        return out;
    }

    auto* info = (任务节点 && 任务节点->主信息) ? 任务节点->主信息 : nullptr;
    if (!info) return out;

    auto* 头信息 = dynamic_cast<任务头结点信息*>(info);
    auto* 根需求 = 头信息 ? 头信息->需求 : nullptr;
    while (根需求 && 根需求->父) {
        根需求 = static_cast<需求节点类*>(根需求->父);
    }

    auto 估计启发式预测 = [&]() -> 结构_结构B因果预测结果 {
        结构_结构B因果预测结果 启发式结果{};
        int 证据数 = 0;
        auto 累加方向 = [&](double& 目标, const std::string& 键, double 权重) {
            const I64 dir = 私有_读任务方向I64(info, 键);
            if (dir != 0) ++证据数;
            目标 += static_cast<double>(dir) * 权重;
        };
        auto 累加事实 = [&](double& 目标, const std::string& 键, double 权重, double 截断绝对值 = 4.0) {
            const I64 fact = 私有_读任务事实I64(info, 键);
            if (fact != 0) ++证据数;
            const double v = std::clamp(static_cast<double>(fact), -截断绝对值, 截断绝对值);
            目标 += v * 权重;
        };

        累加方向(启发式结果.预测ΔS, "安全值", 1.8);
        累加方向(启发式结果.预测ΔS, "待机状态", 0.8);
        累加方向(启发式结果.预测ΔS, "时序正向步长", 0.4);
        累加方向(启发式结果.预测ΔS, "时序反向步长", -0.6);
        累加方向(启发式结果.预测ΔS, "服务时序衰减步长", -0.8);
        累加方向(启发式结果.预测ΔS, "试探动作_已停止", 0.7);
        累加方向(启发式结果.预测ΔS, "保守态_请求", 0.9);
        累加方向(启发式结果.预测ΔS, "需求对齐_安全", 0.5);
        累加方向(启发式结果.预测ΔS, "需求对齐_安全服务", 0.3);
        累加事实(启发式结果.预测ΔS, "风险级别", -0.6);

        累加方向(启发式结果.预测ΔV, "服务值", 1.8);
        累加方向(启发式结果.预测ΔV, "服务动作_输出有效", 1.0);
        累加方向(启发式结果.预测ΔV, "服务动作_已确认收到", 0.9);
        累加方向(启发式结果.预测ΔV, "服务动作_已请求补充信息", 0.4);
        累加方向(启发式结果.预测ΔV, "人类输入_存在", 0.3);
        累加方向(启发式结果.预测ΔV, "需求对齐_服务", 0.5);
        累加方向(启发式结果.预测ΔV, "需求对齐_安全服务", 0.2);

        if (根需求 == 安全需求节点_) {
            启发式结果.预测ΔS += 0.8;
            ++证据数;
        }
        else if (根需求 == 服务需求节点_) {
            启发式结果.预测ΔV += 0.8;
            ++证据数;
        }

        启发式结果.置信度 = std::clamp(0.15 + static_cast<double>(证据数) * 0.08, 0.0, 0.95);
        return 启发式结果;
    };

    const auto 启发式结果 = 估计启发式预测();

    auto 取符号 = [](I64 v) noexcept -> int {
        return (v > 0) ? 1 : ((v < 0) ? -1 : 0);
    };

    auto 取状态特征名 = [](const 状态节点类* 状态) -> std::string {
        auto* smi = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
        auto* 特征节点 = smi ? smi->状态特征 : nullptr;
        auto* 特征信息 = (特征节点 && 特征节点->主信息) ? dynamic_cast<特征节点主信息类*>(特征节点->主信息) : nullptr;
        auto* 类型词性 = 特征信息 ? 特征信息->类型 : nullptr;
        auto* 类型词 = (类型词性 && 类型词性->父 && 类型词性->父->主信息)
            ? dynamic_cast<词主信息类*>(类型词性->父->主信息)
            : nullptr;
        if (类型词) return 类型词->词;
        if (类型词性) return 类型词性->获取主键();
        return 特征节点 ? 特征节点->获取主键() : std::string{};
    };

    std::vector<方法节点类*> 候选方法{};
    auto 追加候选方法 = [&](方法节点类* 方法首节点) {
        if (!方法首节点) return;
        if (std::find(候选方法.begin(), 候选方法.end(), 方法首节点) == 候选方法.end()) {
            候选方法.push_back(方法首节点);
        }
    };

    追加候选方法(info->当前方法首节点);
    追加候选方法(info->最近方法首节点);
    if (auto* 步骤信息 = dynamic_cast<任务步骤节点信息*>(info)) {
        追加候选方法(步骤信息->当前选中方法首节点);
    }
    if (auto* 结果信息 = dynamic_cast<任务结果节点信息*>(info)) {
        追加候选方法(结果信息->来源方法首节点);
    }
    if (头信息 && 头信息->当前步骤节点 && 头信息->当前步骤节点->主信息) {
        if (auto* 当前步骤信息 = dynamic_cast<任务步骤节点信息*>(头信息->当前步骤节点->主信息)) {
            追加候选方法(当前步骤信息->当前选中方法首节点);
        }
    }
    if (任务节点) {
        if (const auto 当前方法指针 = 私有_读取任务事实指针(任务节点, "任务_当前方法首节点指针");
            当前方法指针.has_value() && *当前方法指针 != 0) {
            追加候选方法(reinterpret_cast<方法节点类*>(*当前方法指针));
        }
        if (const auto 最近方法指针 = 私有_读取任务事实指针(任务节点, "任务_最近方法首节点指针");
            最近方法指针.has_value() && *最近方法指针 != 0) {
            追加候选方法(reinterpret_cast<方法节点类*>(*最近方法指针));
        }
    }

    std::vector<基础信息节点类*> 候选因果{};
    auto 追加候选因果 = [&](const std::vector<基础信息节点类*>& 输入) {
        for (auto* 节点 : 输入) {
            if (!节点 || !节点->主信息) continue;
            if (std::find(候选因果.begin(), 候选因果.end(), 节点) == 候选因果.end()) {
                候选因果.push_back(节点);
            }
        }
    };

    for (auto* 方法首节点 : 候选方法) {
        追加候选因果(因果集.按来源方法查询因果(方法首节点, 24));
    }

    auto* 自我存在 = 世界树.自我指针;
    if (自我存在 && 特征类型定义类::类型_自我_安全值) {
        if (auto* 安全特征 = 世界树.查找子特征_按类型(
            自我存在,
            特征类型定义类::类型_自我_安全值,
            "自我线程类::私有_估计任务因果预测/查安全特征")) {
            追加候选因果(因果集.按结果特征查询因果(安全特征, 24));
        }
    }
    if (自我存在 && 特征类型定义类::类型_自我_服务值) {
        if (auto* 服务特征 = 世界树.查找子特征_按类型(
            自我存在,
            特征类型定义类::类型_自我_服务值,
            "自我线程类::私有_估计任务因果预测/查服务特征")) {
            追加候选因果(因果集.按结果特征查询因果(服务特征, 24));
        }
    }

    结构_结构B因果预测结果 因果结果{};
    int 因果证据数 = 0;
    const auto 根需求偏安全 = (根需求 == 安全需求节点_);
    const auto 根需求偏服务 = (根需求 == 服务需求节点_);

    for (auto* 节点 : 候选因果) {
        auto* mi = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
        if (!mi || mi->是否原始观测实例) continue;

        double 权重 = 0.15;
        权重 += std::clamp(static_cast<double>(mi->观察次数) / 3.0, 0.0, 4.0) * 0.12;
        权重 += std::clamp(static_cast<double>(mi->稳定度Q) / 2500.0, 0.0, 4.0) * 0.08;
        if (mi->已验证) 权重 += 0.2;

        bool 命中方法 = false;
        for (auto* 方法首节点 : 候选方法) {
            if (!方法首节点) continue;
            if (mi->来源方法主键 == static_cast<std::uint64_t>(std::hash<std::string>{}(方法首节点->获取主键()))) {
                命中方法 = true;
                权重 += 0.45;
                break;
            }
        }

        int 条件命中分 = 0;
        int 条件冲突分 = 0;
        int 条件证据数 = 0;
        for (std::size_t i = 0; i < mi->条件状态列表.size(); ++i) {
            auto* 条件状态 = mi->条件状态列表[i];
            const auto 条件键 = 取状态特征名(条件状态);
            if (条件键.empty()) continue;

            const I64 当前方向 = 私有_读任务方向I64(info, 条件键);
            if (当前方向 == 0) continue;
            ++条件证据数;

            const bool 可比较 = i < mi->条件方向可比较列表.size()
                && mi->条件方向可比较列表[i] != 0;
            const I64 期望方向 = i < mi->条件方向值列表.size()
                ? mi->条件方向值列表[i]
                : 0;
            if (!可比较 || 期望方向 == 0) {
                条件命中分 += 1;
                continue;
            }

            if (取符号(当前方向) == 取符号(期望方向)) {
                条件命中分 += 2;
            }
            else {
                条件冲突分 += 2;
            }
        }
        if (条件证据数 > 0) {
            权重 += static_cast<double>(条件命中分) * 0.08;
            权重 -= static_cast<double>(条件冲突分) * 0.12;
        }
        if (命中方法 && 条件证据数 == 0) {
            权重 += 0.05;
        }
        if (权重 <= 0.05) continue;

        auto 结果方向符号 = mi->结果方向可比较 ? 取符号(mi->结果方向值) : 0;
        if (结果方向符号 == 0) continue;

        auto 应用结果方向 = [&](const 状态节点类* 结果状态) {
            const auto 结果键 = 取状态特征名(结果状态);
            if (结果键 == "安全值") {
                const double 需求系数 = 根需求偏安全 ? 1.25 : 1.0;
                因果结果.预测ΔS += static_cast<double>(结果方向符号) * 权重 * 需求系数;
                ++因果证据数;
            }
            else if (结果键 == "服务值") {
                const double 需求系数 = 根需求偏服务 ? 1.25 : 1.0;
                因果结果.预测ΔV += static_cast<double>(结果方向符号) * 权重 * 需求系数;
                ++因果证据数;
            }
        };

        if (mi->结果状态) {
            应用结果方向(mi->结果状态);
        }
        else {
            for (auto* 结果状态 : mi->结果状态列表) {
                应用结果方向(结果状态);
            }
        }
    }

    if (因果证据数 > 0) {
        因果结果.置信度 = std::clamp(0.25 + static_cast<double>(因果证据数) * 0.07, 0.0, 0.92);
        out.预测ΔS = 因果结果.预测ΔS + 启发式结果.预测ΔS * 0.35;
        out.预测ΔV = 因果结果.预测ΔV + 启发式结果.预测ΔV * 0.35;
        out.置信度 = std::clamp(std::max(因果结果.置信度, 启发式结果.置信度 * 0.5), 0.0, 0.98);
        return out;
    }

    return 启发式结果;
}

inline const char* 自我线程类::私有_学习阶段标签(枚举_学习任务阶段 阶段) noexcept {
    switch (阶段) {
    case 枚举_学习任务阶段::采样: return "采样";
    case 枚举_学习任务阶段::更新: return "更新";
    case 枚举_学习任务阶段::验证: return "验证";
    case 枚举_学习任务阶段::提交: return "提交";
    case 枚举_学习任务阶段::回滚: return "回滚";
    default: return "空闲";
    }
}

inline 任务节点类* 自我线程类::私有_取尝试学习任务节点() const noexcept {
    if (学习尝试任务节点_ && 学习尝试任务节点_->主信息) return 学习尝试任务节点_;
    if (学习任务节点_ && 学习任务节点_->主信息) return 学习任务节点_;
    return nullptr;
}

inline 任务头结点信息* 自我线程类::私有_取尝试学习任务头信息() const noexcept {
    auto* 节点 = 私有_取尝试学习任务节点();
    return (节点 && 节点->主信息) ? dynamic_cast<任务头结点信息*>(节点->主信息) : nullptr;
}

inline void 自我线程类::私有_确保学习任务分支节点(时间戳 now) {
    if (now == 0) now = 结构体_时间戳::当前_微秒();
    if (!学习任务节点_ || !学习任务节点_->主信息) return;

    auto* 学习根头信息 = dynamic_cast<任务头结点信息*>(学习任务节点_->主信息);
    if (!学习根头信息) return;

    const auto* 分支名称 = 语素集.添加词性词("学习_尝试学习分支", "名词");
    const auto* 分支类型 = 语素集.添加词性词("学习分支任务_尝试学习", "名词");
    const auto 分支场景 = 学习根头信息->场景
        ? 学习根头信息->场景
        : (世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界);

    if (!学习尝试任务节点_ || !学习尝试任务节点_->主信息) {
        auto* info = new 任务头结点信息();
        info->节点种类 = 枚举_任务节点种类::头结点;
        info->名称 = 分支名称;
        info->类型 = 分支类型;
        info->状态 = 枚举_任务状态::完成;
        info->是否常驻任务 = true;
        info->创建时间 = now;
        info->调度优先级 = 学习任务节点_->主信息->调度优先级;
        info->任务树类型 = 枚举_任务树类型::编排任务;
        info->场景 = 分支场景;
        info->任务虚拟存在 = nullptr;
        info->需求 = nullptr;
        info->父任务头结点 = 学习任务节点_;

        学习尝试任务节点_ = 任务类::新建任务节点(
            info,
            学习任务节点_,
            "自我线程类::私有_确保学习任务分支节点/尝试学习分支");
    }

    auto* 分支头信息 = (学习尝试任务节点_ && 学习尝试任务节点_->主信息)
        ? dynamic_cast<任务头结点信息*>(学习尝试任务节点_->主信息)
        : nullptr;
    if (!分支头信息) return;

    分支头信息->名称 = 分支名称;
    分支头信息->类型 = 分支类型;
    分支头信息->创建时间 = now;
    分支头信息->场景 = 分支场景;
    分支头信息->父任务头结点 = 学习任务节点_;
    分支头信息->是否常驻任务 = true;
    分支头信息->任务树类型 = 枚举_任务树类型::编排任务;
    分支头信息->调度优先级 = 学习任务节点_->主信息->调度优先级;

    if (std::find(
        学习根头信息->直属子任务头结点列表.begin(),
        学习根头信息->直属子任务头结点列表.end(),
        学习尝试任务节点_) == 学习根头信息->直属子任务头结点列表.end()) {
        学习根头信息->直属子任务头结点列表.push_back(学习尝试任务节点_);
    }
}

inline 任务节点类* 自我线程类::私有_取学习阶段步骤节点(枚举_学习任务阶段 阶段) const noexcept {
    switch (阶段) {
    case 枚举_学习任务阶段::采样: return 学习采样步骤节点_;
    case 枚举_学习任务阶段::更新: return 学习更新步骤节点_;
    case 枚举_学习任务阶段::验证: return 学习验证步骤节点_;
    case 枚举_学习任务阶段::提交: return 学习提交步骤节点_;
    case 枚举_学习任务阶段::回滚: return 学习回滚步骤节点_;
    default: return nullptr;
    }
}

inline void 自我线程类::私有_确保学习任务阶段节点(时间戳 now) {
    私有_确保学习任务分支节点(now);
    auto* 尝试学习任务节点 = 私有_取尝试学习任务节点();
    if (!尝试学习任务节点 || !尝试学习任务节点->主信息) return;
    auto* 头信息 = dynamic_cast<任务头结点信息*>(尝试学习任务节点->主信息);
    if (!头信息) return;

    auto 创建步骤 = [&](任务节点类*& 槽位, const char* 名称, const char* 类型, std::int32_t 步骤序号) {
        if (槽位 && 槽位->主信息) return;

        任务类::结构_创建步骤参数 参数{};
        参数.名称 = 语素集.添加词性词(名称, "名词");
        参数.类型 = 语素集.添加词性词(类型, "名词");
        参数.所属任务头结点 = 尝试学习任务节点;
        参数.步骤场景 = 头信息->场景
            ? 头信息->场景
            : (世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界);
        参数.创建时间 = now;
        参数.步骤序号 = 步骤序号;
        参数.允许切换方法 = false;
        参数.初始状态 = 枚举_任务状态::就绪;
        槽位 = 任务类::创建步骤节点(
            尝试学习任务节点,
            参数,
            std::string("自我线程类::私有_确保学习任务阶段节点/") + 名称);
    };

    创建步骤(学习采样步骤节点_, "学习阶段_采样", "学习阶段任务_采样", 1);
    创建步骤(学习更新步骤节点_, "学习阶段_更新", "学习阶段任务_更新", 2);
    创建步骤(学习验证步骤节点_, "学习阶段_验证", "学习阶段任务_验证", 3);
    创建步骤(学习提交步骤节点_, "学习阶段_提交", "学习阶段任务_提交", 4);
    创建步骤(学习回滚步骤节点_, "学习阶段_回滚", "学习阶段任务_回滚", 5);
}

inline void 自我线程类::私有_切换学习任务阶段(枚举_学习任务阶段 阶段, 时间戳 now, const std::string& 标签) {
    if (now == 0) now = 结构体_时间戳::当前_微秒();
    私有_确保学习任务阶段节点(now);

    学习工作流状态_.当前阶段 = 阶段;
    学习工作流状态_.最近阶段时间 = now;

    auto* 阶段步骤 = 私有_取学习阶段步骤节点(阶段);
    auto* 尝试学习任务头信息 = 私有_取尝试学习任务头信息();
    if (学习任务节点_ && 学习任务节点_->主信息 && 学习尝试任务节点_) {
        (void)任务类::设置任务当前子任务(
            学习任务节点_->主信息,
            学习尝试任务节点_,
            now,
            "自我线程类::私有_切换学习任务阶段/当前分支/" + 标签);
    }
    if (尝试学习任务头信息) {
        (void)任务类::设置任务当前步骤(
            尝试学习任务头信息,
            阶段步骤,
            now,
            "自我线程类::私有_切换学习任务阶段/当前步骤/" + 标签);
        (void)任务类::写任务虚拟存在_I64特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("学习阶段码"),
            static_cast<I64>(阶段),
            now,
            "自我线程类::私有_切换学习任务阶段/阶段码/" + 标签);
        (void)任务类::写任务虚拟存在_指针特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("学习目标方法指针"),
            reinterpret_cast<std::uintptr_t>(学习工作流状态_.当前候选.head),
            now,
            "自我线程类::私有_切换学习任务阶段/目标方法/" + 标签);
        if (学习尝试任务节点_) {
            任务类::设置任务状态(
                学习尝试任务节点_,
                枚举_任务状态::执行中,
                "自我线程类::私有_切换学习任务阶段/分支执行中/" + 标签);
        }
    }
    if (阶段步骤 && 阶段步骤->主信息) {
        任务类::设置任务状态(
            阶段步骤,
            枚举_任务状态::执行中,
            "自我线程类::私有_切换学习任务阶段/步骤执行中/" + 标签);
    }
}

inline void 自我线程类::私有_结束学习当前阶段(bool 成功, 时间戳 now, const std::string& 标签) {
    if (now == 0) now = 结构体_时间戳::当前_微秒();
    auto* 阶段步骤 = 私有_取学习阶段步骤节点(学习工作流状态_.当前阶段);
    if (阶段步骤 && 阶段步骤->主信息) {
        任务类::设置任务状态(
            阶段步骤,
            成功 ? 枚举_任务状态::完成 : 枚举_任务状态::失败,
            "自我线程类::私有_结束学习当前阶段/" + 标签);
    }
}

inline void 自我线程类::私有_重置学习工作流(const std::string& 标签, 时间戳 now) {
    if (now == 0) now = 结构体_时间戳::当前_微秒();
    学习工作流状态_ = {};
    if (学习任务节点_ && 学习任务节点_->主信息) {
        (void)任务类::设置任务当前步骤(
            学习任务节点_->主信息,
            nullptr,
            now,
            "自我线程类::私有_重置学习工作流/清空当前步骤/" + 标签);
        (void)任务类::设置任务当前子任务(
            学习任务节点_->主信息,
            nullptr,
            now,
            "自我线程类::私有_重置学习工作流/清空当前分支/" + 标签);
    }
    if (auto* 尝试学习任务头信息 = 私有_取尝试学习任务头信息()) {
        (void)任务类::设置任务当前步骤(
            尝试学习任务头信息,
            nullptr,
            now,
            "自我线程类::私有_重置学习工作流/清空尝试学习步骤/" + 标签);
        (void)任务类::写任务虚拟存在_I64特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("学习阶段码"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/阶段码/" + 标签);
        (void)任务类::写任务虚拟存在_指针特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("学习目标方法指针"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/目标方法/" + 标签);
        (void)任务类::写任务虚拟存在_I64特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_待处理"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/待处理/" + 标签);
        (void)任务类::写任务虚拟存在_I64特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_请求时间"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/请求时间/" + 标签);
        (void)任务类::写任务虚拟存在_I64特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_来源优先级"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/来源优先级/" + 标签);
        (void)任务类::写任务虚拟存在_I64特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_错误码"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/错误码/" + 标签);
        (void)任务类::写任务虚拟存在_指针特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_来源任务头指针"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/来源任务/" + 标签);
        (void)任务类::写任务虚拟存在_指针特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_来源步骤指针"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/来源步骤/" + 标签);
        (void)任务类::写任务虚拟存在_指针特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_来源方法指针"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/来源方法/" + 标签);
        (void)任务类::写任务虚拟存在_指针特征(
            尝试学习任务头信息,
            自我线程模块_detail::特征_任务事实("尝试学习_来源结果指针"),
            0,
            now,
            "自我线程类::私有_重置学习工作流/来源结果/" + 标签);
        if (学习尝试任务节点_) {
            任务类::设置任务状态(
                学习尝试任务节点_,
                枚举_任务状态::完成,
                "自我线程类::私有_重置学习工作流/尝试学习分支空闲/" + 标签);
        }
    }
}

inline 自我线程类::结构_学习目标快照 自我线程类::私有_采样学习目标快照() const {
    日志::运行("[自我线程][学习目标] 开始采样学习目标快照");
    结构_学习目标快照 out{};
    std::unordered_set<方法节点类*> 已统计方法{};
    std::unordered_set<任务节点类*> 已遍历节点{};

    auto 记录方法 = [&](方法节点类* 方法首节点) {
        if (!方法首节点) return;
        if (!已统计方法.insert(方法首节点).second) return;
        if (!私有_方法允许参与当前阶段试探与学习(方法首节点)) return;

        ++out.可用方法数;

        I64 稳定次数 = 0;
        if (auto* bmi = dynamic_cast<方法信息基类*>(方法首节点->主信息)) {
            if (auto* ve = bmi->方法虚拟存在根) {
                稳定次数 = 世界树.读取特征_I64(
                    ve,
                    自我线程模块_detail::特征_方法连续稳定次数(),
                    "自我线程类::私有_采样学习目标快照/读稳定次数").value_or(0);
            }
        }
        const I64 尝试状态 = 私有_判定方法尝试状态(方法首节点, nullptr, nullptr);
        if (稳定次数 > 0 || 尝试状态 == 自我线程模块_detail::方法尝试状态_尝试完成) {
            ++out.稳定方法数;
        }
        out.稳定度总分 += (std::max<I64>)(0, 稳定次数);
        if (尝试状态 != 自我线程模块_detail::方法尝试状态_尝试完成) {
            ++out.待进化方法数;
        }
    };

    auto 扫描任务树 = [&](auto&& 自身, 任务节点类* 根任务节点) -> void {
        if (!根任务节点 || !已遍历节点.insert(根任务节点).second) return;
        if (auto* 步骤信息 = (根任务节点 && 根任务节点->主信息)
            ? dynamic_cast<任务步骤节点信息*>(根任务节点->主信息)
            : nullptr) {
            记录方法(步骤信息->当前选中方法首节点);
            for (auto* 方法首节点 : 步骤信息->可用方法首节点列表) {
                记录方法(方法首节点);
            }
        }

        if (auto* 头信息 = (根任务节点 && 根任务节点->主信息)
            ? dynamic_cast<任务头结点信息*>(根任务节点->主信息)
            : nullptr) {
            for (auto* 子任务头 : 头信息->直属子任务头结点列表) {
                自身(自身, 子任务头);
            }
        }

        auto* first = static_cast<任务节点类*>(根任务节点->子);
        auto* cur = first;
        std::unordered_set<任务节点类*> 已遍历同层{};
        while (cur && 已遍历同层.insert(cur).second) {
            自身(自身, cur);
            cur = static_cast<任务节点类*>(cur->下);
        }
    };

    扫描任务树(扫描任务树, 安全任务节点_);
    扫描任务树(扫描任务树, 服务任务节点_);
    日志::运行f(
        "[自我线程][学习目标] 学习目标快照采样结束: available={}, stable={}, score={}, pending={}",
        static_cast<long long>(out.可用方法数),
        static_cast<long long>(out.稳定方法数),
        static_cast<long long>(out.稳定度总分),
        static_cast<long long>(out.待进化方法数));
    return out;
}

inline 结构_学习任务进化结果_v0 自我线程类::私有_刷新学习任务进化目标(
    const 结构_学习目标快照* 基线,
    bool 有待处理请求,
    时间戳 now,
    const std::string& 标签) {
    日志::运行f(
        "[自我线程][学习目标] 开始刷新学习任务进化目标: has_pending={}, tag={}",
        有待处理请求 ? 1 : 0,
        标签);
    if (now == 0) now = 结构体_时间戳::当前_微秒();

    学习工作流状态_.当前目标 = 私有_采样学习目标快照();
    const auto 基线快照 = 基线 ? *基线 : 学习工作流状态_.当前目标;

    结构_学习任务进化输入_v0 输入{};
    输入.基线可用方法数 = 基线快照.可用方法数;
    输入.当前可用方法数 = 学习工作流状态_.当前目标.可用方法数;
    输入.基线稳定方法数 = 基线快照.稳定方法数;
    输入.当前稳定方法数 = 学习工作流状态_.当前目标.稳定方法数;
    输入.基线稳定度总分 = 基线快照.稳定度总分;
    输入.当前稳定度总分 = 学习工作流状态_.当前目标.稳定度总分;
    输入.待进化方法数 = 学习工作流状态_.当前目标.待进化方法数;
    输入.有待处理请求 = 有待处理请求;

    日志::运行("[自我线程][学习目标] 开始调用任务管理器推进学习任务进化");
    auto 结果 = 任务管理器_v0{}.推进学习任务进化(
        私有_取尝试学习任务节点(),
        输入,
        "自我线程类::私有_刷新学习任务进化目标/" + 标签);
    日志::运行f(
        "[自我线程][学习目标] 学习任务进化刷新结束: success={}, continue={}, delta_available={}, delta_stable={}, delta_score={}",
        结果.成功 ? 1 : 0,
        结果.应继续进化 ? 1 : 0,
        static_cast<long long>(结果.可用方法增量),
        static_cast<long long>(结果.稳定方法增量),
        static_cast<long long>(结果.稳定度总分增量));
    学习工作流状态_.最近进化结果 = 结果;
    return 结果;
}

inline 自我线程类::结构_学习候选方法 自我线程类::私有_选择学习候选方法() const {
    结构_学习候选方法 已选候选{};

    auto 是基础尝试学习方法 = [](方法节点类* 方法首节点) {
        auto* hmi = (方法首节点 && 方法首节点->主信息)
            ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息)
            : nullptr;
        return hmi
            && hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && hmi->动作句柄.本能ID == static_cast<I64>(枚举_本能动作ID::学习_尝试学习);
    };

    auto 取动作键 = [&](方法节点类* 方法首节点) -> std::string {
        auto* hmi = (方法首节点 && 方法首节点->主信息)
            ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息)
            : nullptr;
        if (!hmi) return {};
        if (hmi->动作名) return hmi->动作名->获取主键();
        if (hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID && hmi->动作句柄.本能ID != 0) {
            return std::string("本能_") + std::to_string(hmi->动作句柄.本能ID);
        }
        return {};
    };

    auto 取等待唤醒键 = [&](任务节点类* 任务头结点, 方法节点类* 默认方法首节点) -> 方法节点类* {
        auto* 头信息 = (任务头结点 && 任务头结点->主信息)
            ? dynamic_cast<任务头结点信息*>(任务头结点->主信息)
            : nullptr;
        if (头信息 && 头信息->等待学习方法首节点) {
            return 头信息->等待学习方法首节点;
        }
        return 默认方法首节点;
    };

    auto 填充候选 = [&](方法节点类* 方法首节点, 结构_学习候选方法* 输出) -> bool {
        if (!方法首节点 || !输出) return false;
        auto* hmi = dynamic_cast<方法首节点主信息类*>(方法首节点->主信息);
        if (!hmi) return false;
        if (!私有_方法允许参与当前阶段试探与学习(方法首节点)) return false;

        bool hasCondition = false;
        bool hasResult = false;
        方法节点类* localFirstCondition = nullptr;
        auto 遍历同层 = [](方法节点类* first, auto&& fn) {
            if (!first) return;
            auto* cur = first;
            std::unordered_set<方法节点类*> 已遍历同层{};
            while (cur && 已遍历同层.insert(cur).second) {
                fn(cur);
                cur = static_cast<方法节点类*>(cur->下);
            }
        };

        std::vector<方法节点类*> stack;
        if (方法首节点->子) stack.push_back(static_cast<方法节点类*>(方法首节点->子));
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

        I64 tryStatus = 自我线程模块_detail::方法尝试状态_未尝试;
        if (auto* bmi = dynamic_cast<方法信息基类*>(方法首节点->主信息)) {
            auto* ve = bmi->方法虚拟存在根;
            if (ve) {
                tryStatus = 世界树.读取特征_I64(
                    ve,
                    自我线程模块_detail::特征_方法尝试状态(),
                    "自我线程类::选择学习候选方法/读尝试状态").value_or(自我线程模块_detail::方法尝试状态_未尝试);
            }
        }
        if (tryStatus == 自我线程模块_detail::方法尝试状态_尝试完成) return false;

        输出->head = 方法首节点;
        输出->firstCondition = localFirstCondition;
        输出->缺条件 = !hasCondition;
        输出->缺结果 = !hasResult;
        输出->是基础尝试学习 = 是基础尝试学习方法(方法首节点);
        输出->唤醒方法首节点 = 方法首节点;
        输出->动作键 = 取动作键(方法首节点);
        return true;
    };

    auto 从步骤提取候选 = [&](任务节点类* 步骤节点) -> 结构_学习候选方法 {
        结构_学习候选方法 候选{};
        auto* 步骤信息 = (步骤节点 && 步骤节点->主信息)
            ? dynamic_cast<任务步骤节点信息*>(步骤节点->主信息)
            : nullptr;
        if (!步骤信息) return 候选;

        if (填充候选(步骤信息->当前选中方法首节点, &候选)) return 候选;
        for (auto* 方法首节点 : 步骤信息->可用方法首节点列表) {
            if (填充候选(方法首节点, &候选)) return 候选;
        }
        return {};
    };

    auto* 尝试学习任务节点 = 私有_取尝试学习任务节点();
    const bool 显式尝试学习待处理 =
        尝试学习任务节点
        && 尝试学习任务节点->主信息
        && 私有_读取任务事实I64(尝试学习任务节点, "尝试学习_待处理").value_or(0) > 0;

    任务节点类* 来源任务头 = nullptr;
    任务节点类* 来源步骤结点 = nullptr;
    if (显式尝试学习待处理) {
        if (const auto 来源任务指针 = 私有_读取任务事实指针(尝试学习任务节点, "尝试学习_来源任务头指针");
            来源任务指针.has_value() && *来源任务指针 != 0) {
            来源任务头 = reinterpret_cast<任务节点类*>(*来源任务指针);
        }
        if (const auto 来源步骤指针 = 私有_读取任务事实指针(尝试学习任务节点, "尝试学习_来源步骤指针");
            来源步骤指针.has_value() && *来源步骤指针 != 0) {
            来源步骤结点 = reinterpret_cast<任务节点类*>(*来源步骤指针);
        }

        auto 绑定来源任务上下文 = [&](结构_学习候选方法* 候选) {
            if (!候选) return;
            候选->来源任务头结点 = 来源任务头;
            候选->来源步骤结点 = 来源步骤结点;
            候选->唤醒方法首节点 = 取等待唤醒键(来源任务头, 候选->head);
        };

        if (const auto 来源方法指针 = 私有_读取任务事实指针(尝试学习任务节点, "尝试学习_来源方法指针");
            来源方法指针.has_value() && *来源方法指针 != 0) {
            结构_学习候选方法 定向候选{};
            if (填充候选(reinterpret_cast<方法节点类*>(*来源方法指针), &定向候选)) {
                绑定来源任务上下文(&定向候选);
                日志::运行f(
                    "[自我线程] 学习任务优先采用定向方法候选: 方法={}, 动作={}",
                    (void*)定向候选.head,
                    定向候选.动作键.empty() ? std::string("<unknown>") : 定向候选.动作键);
                return 定向候选;
            }
        }

        if (来源步骤结点) {
            auto 定向候选 = 从步骤提取候选(来源步骤结点);
            if (定向候选.head) {
                绑定来源任务上下文(&定向候选);
                日志::运行f(
                    "[自我线程] 学习任务采用来源步骤候选: 步骤={}, 方法={}, 动作={}",
                    (void*)来源步骤结点,
                    (void*)定向候选.head,
                    定向候选.动作键.empty() ? std::string("<unknown>") : 定向候选.动作键);
                return 定向候选;
            }
        }

        if (来源任务头) {
            auto* 来源头信息 = (来源任务头 && 来源任务头->主信息)
                ? dynamic_cast<任务头结点信息*>(来源任务头->主信息)
                : nullptr;

            if (来源头信息 && 来源头信息->当前步骤节点) {
                auto 定向候选 = 从步骤提取候选(来源头信息->当前步骤节点);
                if (定向候选.head) {
                    绑定来源任务上下文(&定向候选);
                    日志::运行f(
                        "[自我线程] 学习任务采用来源任务当前步骤候选: 任务={}, 步骤={}, 方法={}, 动作={}",
                        (void*)来源任务头,
                        (void*)来源头信息->当前步骤节点,
                        (void*)定向候选.head,
                        定向候选.动作键.empty() ? std::string("<unknown>") : 定向候选.动作键);
                    return 定向候选;
                }
            }

            auto 定向候选 = 从步骤提取候选(私有_查找首个步骤节点(来源任务头));
            if (定向候选.head) {
                绑定来源任务上下文(&定向候选);
                日志::运行f(
                    "[自我线程] 学习任务采用来源任务首步骤候选: 任务={}, 方法={}, 动作={}",
                    (void*)来源任务头,
                    (void*)定向候选.head,
                    定向候选.动作键.empty() ? std::string("<unknown>") : 定向候选.动作键);
                return 定向候选;
            }
        }
    }

    auto 构造等待学习候选 = [&]() -> std::optional<结构_学习候选方法> {
        const auto 等待学习任务 = 私有_选择等待学习任务();
        if (!等待学习任务.任务头结点) {
            return std::nullopt;
        }

        auto 等待任务候选 = 从步骤提取候选(等待学习任务.步骤结点);
        if (等待任务候选.head && !等待任务候选.是基础尝试学习) {
            等待任务候选.来自等待学习任务 = true;
            等待任务候选.来源任务头结点 = 等待学习任务.任务头结点;
            等待任务候选.来源步骤结点 = 等待学习任务.步骤结点;
            等待任务候选.唤醒方法首节点 = 取等待唤醒键(
                等待学习任务.任务头结点,
                等待任务候选.head);
            日志::运行f(
                "[自我线程] 学习任务优先采用等待学习任务候选: 任务={}, 步骤={}, 方法={}, 动作={}",
                (void*)等待学习任务.任务头结点,
                (void*)等待学习任务.步骤结点,
                (void*)等待任务候选.head,
                等待任务候选.动作键.empty() ? std::string("<unknown>") : 等待任务候选.动作键);
            return 等待任务候选;
        }

        结构_学习候选方法 基础等待候选{};
        const 时间戳 now = 结构体_时间戳::当前_微秒();
        基础等待候选.head = 方法集.查找或创建_本能方法首节点(
            枚举_本能动作ID::学习_尝试学习,
            now,
            "自我线程类::私有_选择学习候选方法/等待学习任务确保基础尝试学习方法");
        if (!基础等待候选.head) {
            return std::nullopt;
        }

        方法集.初始化方法虚拟存在信息(
            基础等待候选.head,
            now,
            "自我线程类::私有_选择学习候选方法/等待学习任务初始化基础尝试学习方法");
        基础等待候选.动作键 = 取动作键(基础等待候选.head);
        基础等待候选.是基础尝试学习 = true;
        基础等待候选.来自等待学习任务 = true;
        基础等待候选.来源任务头结点 = 等待学习任务.任务头结点;
        基础等待候选.来源步骤结点 = 等待学习任务.步骤结点;
        基础等待候选.唤醒方法首节点 = 取等待唤醒键(
            等待学习任务.任务头结点,
            基础等待候选.head);
        日志::运行f(
            "[自我线程] 学习任务切换到等待学习任务基础尝试学习: 任务={}, 步骤={}, 唤醒方法={}",
            (void*)等待学习任务.任务头结点,
            (void*)等待学习任务.步骤结点,
            (void*)基础等待候选.唤醒方法首节点);
        return 基础等待候选;
    };

    if (auto 等待候选 = 构造等待学习候选(); 等待候选.has_value()) {
        return *等待候选;
    }

    int bestScore = -1;
    I64 bestRunCount = (std::numeric_limits<I64>::max)();
    I64 bestLastCallTime = (std::numeric_limits<I64>::max)();

    struct 候选方法信息 {
        方法节点类* head = nullptr;
        方法节点类* firstCondition = nullptr;
        bool hasCondition = false;
        bool hasResult = false;
        bool isBaseTryLearn = false;
        std::string actionKey{};
    };
    std::vector<候选方法信息> 候选列表;

    {
        锁调度器守卫 锁({
            锁请求::读(方法链.链表锁, 枚举_锁域::方法链, "方法链", "自我线程类::选择学习候选方法")
            });

        auto* root = 方法链.根指针;
        if (root && root->子) {
            auto 遍历同层 = [](方法节点类* first, auto&& fn) {
                if (!first) return;
                auto* cur = first;
                std::unordered_set<方法节点类*> 已遍历同层{};
                while (cur && 已遍历同层.insert(cur).second) {
                    fn(cur);
                    cur = static_cast<方法节点类*>(cur->下);
                }
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
                候选.isBaseTryLearn = 是基础尝试学习方法(head);
                候选.actionKey = 取动作键(head);
                候选列表.push_back(std::move(候选));
            });
        }
    }

    结构_学习候选方法 基础尝试学习候选{};
    for (const auto& 候选 : 候选列表) {
        if (!候选.head) continue;

        if (候选.isBaseTryLearn && !基础尝试学习候选.head) {
            基础尝试学习候选.head = 候选.head;
            基础尝试学习候选.firstCondition = 候选.firstCondition;
            基础尝试学习候选.缺条件 = !候选.hasCondition;
            基础尝试学习候选.缺结果 = !候选.hasResult;
            基础尝试学习候选.是基础尝试学习 = true;
            基础尝试学习候选.唤醒方法首节点 = 候选.head;
            基础尝试学习候选.动作键 = 候选.actionKey;
        }

        I64 tryStatus = 自我线程模块_detail::方法尝试状态_未尝试;
        I64 runCount = 0;
        I64 lastCallTime = 0;
        if (auto* bmi = dynamic_cast<方法信息基类*>(候选.head->主信息)) {
            auto* ve = bmi->方法虚拟存在根;
            if (ve) {
                tryStatus = 世界树.读取特征_I64(
                    ve,
                    自我线程模块_detail::特征_方法尝试状态(),
                    "自我线程类::选择学习候选方法/读尝试状态").value_or(自我线程模块_detail::方法尝试状态_未尝试);
                runCount = 世界树.读取特征_I64(
                    ve,
                    语素集.添加词性词("节点_运行次数", "名词"),
                    "自我线程类::选择学习候选方法/读运行次数").value_or(0);
                lastCallTime = 世界树.读取特征_I64(
                    ve,
                    语素集.添加词性词("方法_最后调用时间", "名词"),
                    "自我线程类::选择学习候选方法/读最后调用时间").value_or(0);
            }
        }
        if (tryStatus == 自我线程模块_detail::方法尝试状态_尝试完成) continue;
        if (候选.isBaseTryLearn) continue;

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
        已选候选.head = 候选.head;
        已选候选.firstCondition = 候选.firstCondition;
        已选候选.缺条件 = !候选.hasCondition;
        已选候选.缺结果 = !候选.hasResult;
        已选候选.是基础尝试学习 = false;
        已选候选.唤醒方法首节点 = 候选.head;
        已选候选.动作键 = 候选.actionKey;
    }

    if (已选候选.head) {
        return 已选候选;
    }

    if (基础尝试学习候选.head) {
        return 基础尝试学习候选;
    }

    return 已选候选;
}

inline bool 自我线程类::私有_推进学习采样阶段(时间戳 now) {
    私有_切换学习任务阶段(枚举_学习任务阶段::采样, now, "采样开始");
    学习工作流状态_.基线目标 = 私有_采样学习目标快照();

    if (待学习方法数量_ <= 0 && !私有_存在等待学习任务()) {
        私有_结束学习当前阶段(false, now, "采样/无待学习方法");
        私有_重置学习工作流("采样/无待学习方法", now);
        return false;
    }

    学习工作流状态_.当前候选 = 私有_选择学习候选方法();
    if (!学习工作流状态_.当前候选.head) {
        私有_结束学习当前阶段(false, now, "采样/未找到候选");
        私有_重置学习工作流("采样/未找到候选", now);
        return false;
    }

    日志::运行f(
        "[自我线程] 学习采样阶段选中候选: 方法={}, 动作={}, 缺条件={}, 缺结果={}",
        (void*)学习工作流状态_.当前候选.head,
        学习工作流状态_.当前候选.动作键.empty() ? std::string("<unknown>") : 学习工作流状态_.当前候选.动作键,
        学习工作流状态_.当前候选.缺条件,
        学习工作流状态_.当前候选.缺结果);

    私有_结束学习当前阶段(true, now, "采样/候选已选定");
    私有_切换学习任务阶段(枚举_学习任务阶段::更新, now, "采样后进入更新");
    return true;
}

inline bool 自我线程类::私有_推进学习验证阶段(时间戳 now) {
    私有_切换学习任务阶段(枚举_学习任务阶段::验证, now, "验证开始");
    auto* 尝试学习任务节点 = 私有_取尝试学习任务节点();
    const bool 显式待处理 =
        尝试学习任务节点
        && 尝试学习任务节点->主信息
        && 私有_读取任务事实I64(尝试学习任务节点, "尝试学习_待处理").value_or(0) > 0;
    const bool 有待处理请求 = 显式待处理 || 私有_存在等待学习任务();
    const auto 进化结果 = 私有_刷新学习任务进化目标(
        &学习工作流状态_.基线目标,
        有待处理请求,
        now,
        "验证阶段");
    const bool 通过 =
        学习工作流状态_.当前候选.head
        && (进化结果.可用方法已增加 || 进化结果.方法稳定性已提升);
    私有_结束学习当前阶段(通过, now, 通过 ? "验证/通过" : "验证/失败");
    私有_切换学习任务阶段(
        通过 ? 枚举_学习任务阶段::提交 : 枚举_学习任务阶段::回滚,
        now,
        通过 ? "验证后进入提交" : "验证后进入回滚");
    return 通过;
}

inline void 自我线程类::私有_推进学习提交阶段(时间戳 now) {
    私有_切换学习任务阶段(枚举_学习任务阶段::提交, now, "提交开始");
    auto* 唤醒方法首节点 = 学习工作流状态_.当前候选.唤醒方法首节点
        ? 学习工作流状态_.当前候选.唤醒方法首节点
        : 学习工作流状态_.当前候选.head;
    if (学习工作流状态_.需要唤醒等待任务 && 唤醒方法首节点) {
        私有_唤醒等待学习的任务(唤醒方法首节点, "学习提交阶段");
    }

    日志::运行f(
        "[自我线程] 学习提交阶段完成: 方法={}, 候选={}, 更新={}, 新建={}, 观察成功={}, 可用方法增量={}, 稳定度增量={}, 应继续进化={}",
        (void*)学习工作流状态_.当前候选.head,
        static_cast<long long>(学习工作流状态_.候选数量),
        static_cast<long long>(学习工作流状态_.更新数量),
        static_cast<long long>(学习工作流状态_.新建数量),
        学习工作流状态_.上次观察成功 ? 1 : 0,
        static_cast<long long>(学习工作流状态_.最近进化结果.可用方法增量),
        static_cast<long long>(学习工作流状态_.最近进化结果.稳定度总分增量),
        学习工作流状态_.最近进化结果.应继续进化 ? 1 : 0);

    私有_结束学习当前阶段(true, now, "提交完成");
    私有_重置学习工作流("提交完成", now);
}

inline void 自我线程类::私有_推进学习回滚阶段(时间戳 now) {
    私有_切换学习任务阶段(枚举_学习任务阶段::回滚, now, "回滚开始");
    日志::运行f(
        "[自我线程] 学习回滚阶段结束: 方法={}, 候选={}, 更新={}, 新建={}, 可用方法增量={}, 稳定度增量={}, 原因=验证未通过或未达学习目标",
        (void*)学习工作流状态_.当前候选.head,
        static_cast<long long>(学习工作流状态_.候选数量),
        static_cast<long long>(学习工作流状态_.更新数量),
        static_cast<long long>(学习工作流状态_.新建数量),
        static_cast<long long>(学习工作流状态_.最近进化结果.可用方法增量),
        static_cast<long long>(学习工作流状态_.最近进化结果.稳定度总分增量));
    私有_结束学习当前阶段(false, now, "回滚完成");
    私有_重置学习工作流("回滚完成", now);
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
    std::unordered_set<需求节点类*> 已遍历同层{};
    while (cur && 已遍历同层.insert(cur).second) {
        if (cur && cur->主信息 && cur->主信息->类型 == 需求类型) {
            return cur;
        }
        cur = static_cast<需求节点类*>(cur->下);
    }

    return nullptr;
}

inline 任务节点类* 自我线程类::私有_查找首个步骤节点(任务节点类* 任务节点) const {
    if (!任务节点 || !任务节点->子) return nullptr;

    auto* first = static_cast<任务节点类*>(任务节点->子);
    auto* cur = first;
    std::unordered_set<任务节点类*> 已遍历同层{};
    while (cur && 已遍历同层.insert(cur).second) {
        if (cur && cur->主信息 && dynamic_cast<任务步骤节点信息*>(cur->主信息)) {
            return cur;
        }
        cur = static_cast<任务节点类*>(cur->下);
    }

    return nullptr;
}

inline 自我线程类::结构_等待学习任务 自我线程类::私有_选择等待学习任务() const {
    结构_等待学习任务 已选{};

    auto 选任务 = [&](任务节点类* 任务头结点) {
        auto* 头信息 = (任务头结点 && 任务头结点->主信息)
            ? dynamic_cast<任务头结点信息*>(任务头结点->主信息)
            : nullptr;
        if (!头信息 || !头信息->等待学习唤醒 || 任务头结点 == 学习任务节点_) return;

        auto* 步骤结点 = (头信息->当前步骤节点 && 头信息->当前步骤节点->主信息
            && dynamic_cast<任务步骤节点信息*>(头信息->当前步骤节点->主信息))
            ? 头信息->当前步骤节点
            : 私有_查找首个步骤节点(任务头结点);

        const auto 创建时间 = 头信息->创建时间 > 0 ? 头信息->创建时间 : 0;
        const bool 更优 =
            !已选.任务头结点
            || 头信息->调度优先级 > 已选.来源优先级
            || (头信息->调度优先级 == 已选.来源优先级 && 创建时间 < 已选.创建时间);
        if (!更优) return;

        已选.任务头结点 = 任务头结点;
        已选.步骤结点 = 步骤结点;
        已选.等待方法首节点 = 头信息->等待学习方法首节点;
        已选.来源优先级 = 头信息->调度优先级;
        已选.创建时间 = 创建时间;
    };

    auto 扫描任务树 = [&](任务节点类* 根任务节点) {
        if (!根任务节点) return;

        std::vector<任务节点类*> 栈{};
        std::unordered_set<任务节点类*> 已入栈{};
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

            if (dynamic_cast<任务头结点信息*>(当前->主信息)) {
                选任务(当前);
                if (auto* 头信息 = dynamic_cast<任务头结点信息*>(当前->主信息)) {
                    for (auto* 直属子任务头 : 头信息->直属子任务头结点列表) {
                        压栈(直属子任务头);
                    }
                }
            }

            auto* 起 = 当前->子;
            auto* it = 起;
            std::unordered_set<任务节点类*> 已遍历同层{};
            while (it && 已遍历同层.insert(it).second) {
                压栈(it);
                it = static_cast<任务节点类*>(it->下);
            }
        }
    };

    扫描任务树(安全任务节点_);
    扫描任务树(服务任务节点_);
    return 已选;
}

inline bool 自我线程类::私有_存在等待学习任务() const {
    return 私有_选择等待学习任务().任务头结点 != nullptr;
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
    std::unordered_set<任务节点类*> 已遍历同层{};
    while (cur && 已遍历同层.insert(cur).second) {
        if (dynamic_cast<任务头结点信息*>(cur ? cur->主信息 : nullptr) && 子任务活跃(cur)) {
            return true;
        }
        cur = static_cast<任务节点类*>(cur->下);
    }

    return false;
}

inline bool 自我线程类::私有_学习任务有待处理请求() const {
    auto* 尝试学习任务节点 = 私有_取尝试学习任务节点();
    const bool 显式待处理 =
        尝试学习任务节点
        && 尝试学习任务节点->主信息
        && 私有_读取任务事实I64(尝试学习任务节点, "尝试学习_待处理").value_or(0) > 0;
    const bool 目标仍待进化 =
        尝试学习任务节点
        && 尝试学习任务节点->主信息
        && 私有_读取任务事实I64(尝试学习任务节点, "学习目标_应继续进化").value_or(0) > 0;
    return 显式待处理 || 私有_存在等待学习任务() || 目标仍待进化;
}

inline void 自我线程类::私有_排队任务事实I64(任务信息基类* info, const std::string& 键, I64 值, 时间戳 now) {
    if (!info || !info->任务虚拟存在) return;
    std::lock_guard<std::mutex> lk(延迟任务事实锁_);
    延迟任务事实队列_.push_back(待写回任务事实项{
        info,
        键,
        false,
        值,
        0,
        now
        });
}

inline void 自我线程类::私有_排队任务事实指针(任务信息基类* info, const std::string& 键, std::uintptr_t ptr, 时间戳 now) {
    if (!info || !info->任务虚拟存在) return;
    std::lock_guard<std::mutex> lk(延迟任务事实锁_);
    延迟任务事实队列_.push_back(待写回任务事实项{
        info,
        键,
        true,
        0,
        ptr,
        now
        });
}

inline void 自我线程类::私有_冲刷延迟任务事实队列(const std::string& 调用点, 时间戳 now) {
    std::vector<待写回任务事实项> 待冲刷{};
    {
        std::lock_guard<std::mutex> lk(延迟任务事实锁_);
        if (延迟任务事实队列_.empty()) return;
        待冲刷.swap(延迟任务事实队列_);
    }

    if (now == 0) now = 结构体_时间戳::当前_微秒();
    for (const auto& 项 : 待冲刷) {
        if (!项.目标信息 || !项.目标信息->任务虚拟存在) continue;
        const 时间戳 写回时间 = (项.时间 != 0) ? 项.时间 : now;
        const auto* factType = 自我线程模块_detail::特征_任务事实(项.键);
        if (项.是指针) {
            (void)任务类::写任务虚拟存在_指针特征(
                项.目标信息,
                factType,
                项.指针值,
                写回时间,
                调用点 + "/写指针/" + 项.键);
            continue;
        }

        (void)任务类::写任务虚拟存在_I64特征(
            项.目标信息,
            factType,
            项.I64值,
            写回时间,
            调用点 + "/写事实/" + 项.键);
        (void)任务类::写任务虚拟存在_I64特征(
            项.目标信息,
            自我线程模块_detail::特征_任务方向(项.键),
            0,
            写回时间,
            调用点 + "/写方向/" + 项.键);
    }

    日志::运行f("[自我线程] 已冲刷延迟任务事实: 调用点={}, 条目数={}", 调用点, static_cast<unsigned long long>(待冲刷.size()));
}

inline std::optional<I64> 自我线程类::私有_读取任务事实I64(任务节点类* 任务节点, const std::string& 键) const {
    auto* info = 任务节点 ? 任务节点->主信息 : nullptr;
    if (!info || !info->任务虚拟存在) return std::nullopt;
    const auto* type = 自我线程模块_detail::特征_任务事实(键);
    auto 快照 = 世界树.读取特征快照(
        info->任务虚拟存在,
        type,
        "自我线程类::私有_读取任务事实I64/" + 键);
    if (!快照.has_value()) {
        快照 = 自我线程模块_detail::按特征主键读取快照(
            info->任务虚拟存在,
            type->获取主键(),
            "自我线程类::私有_读取任务事实I64/按主键回退/" + 键);
    }
    if (!快照.has_value() || !std::holds_alternative<I64>(*快照)) {
        return std::nullopt;
    }
    return std::get<I64>(*快照);
}

inline std::optional<std::uintptr_t> 自我线程类::私有_读取任务事实指针(任务节点类* 任务节点, const std::string& 键) const {
    auto* info = 任务节点 ? 任务节点->主信息 : nullptr;
    if (!info || !info->任务虚拟存在) return std::nullopt;
    const auto* type = 自我线程模块_detail::特征_任务事实(键);
    auto 快照 = 世界树.读取特征快照(
        info->任务虚拟存在,
        type,
        "自我线程类::私有_读取任务事实指针/" + 键);
    if (!快照.has_value()) {
        快照 = 自我线程模块_detail::按特征主键读取快照(
            info->任务虚拟存在,
            type->获取主键(),
            "自我线程类::私有_读取任务事实指针/按主键回退/" + 键);
    }
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
            now,
            [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
            },
            "自我线程类::私有_确保显式阶段任务/目标状态/" + 阶段标签);
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

inline void 自我线程类::私有_扫描任务树并尝试入队(任务节点类* 根任务节点, const std::string& 标签, std::size_t* 剩余预算) {
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
                const bool 有预算 = (!剩余预算 || *剩余预算 > 0);
                if (有预算) {
                    const bool 已入队 = 私有_尝试入队动作任务(当前, 标签);
                    if (已入队 && 剩余预算 && *剩余预算 > 0) {
                        --(*剩余预算);
                    }
                }
            }
        }

        auto* 起 = 当前->子;
        auto* it = 起;
        std::unordered_set<任务节点类*> 已遍历同层{};
        while (it && 已遍历同层.insert(it).second) {
            压栈(it);
            it = static_cast<任务节点类*>(it->下);
        }
    }
}

inline void 自我线程类::私有_唤醒等待学习的任务(方法节点类* 方法首节点, const std::string& 标签) {
    if (!方法首节点) return;
    const 时间戳 now = 结构体_时间戳::当前_微秒();

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
                    自我线程模块_detail::结构_任务调度动作事件守卫 唤醒动作(
                        static_cast<任务信息基类*>(头信息),
                        自我线程模块_detail::动作_唤醒等待学习任务_(),
                        now,
                        "自我线程类::私有_唤醒等待学习的任务/" + 标签);
                    头信息->等待学习唤醒 = false;
                    头信息->等待学习方法首节点 = nullptr;
                    auto* 当前步骤结点 = (头信息->当前步骤节点 && 头信息->当前步骤节点->主信息)
                        ? 头信息->当前步骤节点
                        : 私有_查找首个步骤节点(当前);
                    auto* 当前步骤信息 = (当前步骤结点 && 当前步骤结点->主信息)
                        ? dynamic_cast<任务步骤节点信息*>(当前步骤结点->主信息)
                        : nullptr;
                    const bool 步骤已有方法 =
                        当前步骤信息
                        && (当前步骤信息->当前选中方法首节点 != nullptr
                            || !当前步骤信息->可用方法首节点列表.empty());
                    if (当前步骤结点 && 当前步骤信息 && 步骤已有方法) {
                        if (当前步骤信息->状态 == 枚举_任务状态::待重筹办
                            || 当前步骤信息->状态 == 枚举_任务状态::失败
                            || 当前步骤信息->状态 == 枚举_任务状态::无法执行
                            || 当前步骤信息->状态 == 枚举_任务状态::挂起) {
                            任务类::设置任务状态(
                                当前步骤结点,
                                枚举_任务状态::就绪,
                                "自我线程类::私有_唤醒等待学习的任务/步骤就绪/" + 标签);
                        }
                        (void)任务类::设置任务当前步骤(
                            当前->主信息,
                            当前步骤结点,
                            now,
                            "自我线程类::私有_唤醒等待学习的任务/恢复当前步骤/" + 标签);
                    }
                    else if (当前步骤结点) {
                        (void)任务类::设置任务当前步骤(
                            当前->主信息,
                            nullptr,
                            now,
                            "自我线程类::私有_唤醒等待学习的任务/清空待重筹办步骤/" + 标签);
                    }
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
            std::unordered_set<任务节点类*> 已遍历同层{};
            while (it && 已遍历同层.insert(it).second) {
                栈.push_back(it);
                it = static_cast<任务节点类*>(it->下);
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

    auto* 学习默认场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;

    if (!学习任务节点_) {
        auto* info = new 任务头结点信息();
        info->节点种类 = 枚举_任务节点种类::头结点;
        info->名称 = 学习任务名;
        info->类型 = 学习任务类型;
        info->状态 = 枚举_任务状态::未启动;
        info->是否常驻任务 = true;
        info->创建时间 = now;
        info->调度优先级 = (待学习方法数量_ > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
        info->任务树类型 = 枚举_任务树类型::编排任务;
        info->场景 = 学习默认场景;
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
    调度器_.设置调度上下文查询回调([this]() {
        auto 归一化百分值 = [](I64 x) -> double {
            if (x <= 0) return 0.0;
            const long double maxV = static_cast<long double>((std::numeric_limits<I64>::max)());
            if (maxV <= 0.0L) return 0.0;
            const long double 比例 = static_cast<long double>(x) / maxV;
            return std::clamp(static_cast<double>(比例 * 100.0L), 0.0, 100.0);
        };

        结构_结构B调度上下文 ctx{};
        ctx.当前安全值 = 归一化百分值(自我核心_.获取安全值());
        ctx.安全下限 = 1.0;
        ctx.安全回归中心 = 50.0;
        return ctx;
    });
    调度器_.设置因果预测回调([this](任务节点类* 任务, bool 作为学习任务) {
        return 私有_估计任务因果预测(任务, 作为学习任务);
    });
    调度器_.设置任务优先级写回回调([](任务节点类* 任务, std::int64_t 新优先级, const std::string& 调用点) {
        if (!任务 || !任务->主信息) return;
        (void)任务类::设置任务调度优先级(
            任务->主信息,
            新优先级,
            结构体_时间戳::当前_微秒(),
            调用点);
    });
    调度器_.设置结构B参数(1.0, 32.0, 15000000.0);
    调度器_.初始化_扫描需求链("自我线程类::初始化_初始需求与任务/初始化调度器");
    调度器已初始化_.store(true);

    学习任务节点_->主信息->名称 = 学习任务名;
    学习任务节点_->主信息->类型 = 学习任务类型;
    学习任务节点_->主信息->创建时间 = now;
    if (auto* 学习头信息 = dynamic_cast<任务头结点信息*>(学习任务节点_->主信息)) {
        学习头信息->是否常驻任务 = true;
    }
    学习任务节点_->主信息->调度优先级 = (std::numeric_limits<std::int64_t>::min)();
    学习任务节点_->主信息->任务树类型 = 枚举_任务树类型::编排任务;
    学习任务节点_->主信息->场景 = 学习默认场景;
    私有_确保学习任务分支节点(now);
    私有_确保学习任务阶段节点(now);
    私有_重置学习工作流("初始化_初始需求与任务", now);
    const bool 初始化需启动学习流 =
        私有_学习任务有待处理请求()
        || (待学习方法数量_ > 0)
        || (学习工作流状态_.当前阶段 != 枚举_学习任务阶段::空闲);

    if (启用学习线程_ && 初始化需启动学习流) {
        任务类::设置任务状态(学习任务节点_, 枚举_任务状态::排队中, "自我线程类::初始化_初始需求与任务/学习任务排队");
    }
    else {
        任务类::设置任务状态(学习任务节点_, 启用学习线程_ ? 枚举_任务状态::就绪 : 枚举_任务状态::挂起, "自我线程类::初始化_初始需求与任务/学习任务待命");
        if (!启用学习线程_) {
            日志::运行("[自我线程] 当前阶段停用学习线程；学习任务仅保留账本，不进入队列");
        }
    }

    if (启用学习线程_) {
        const bool 需要启动学习流 =
            初始化需启动学习流
            || (学习工作流状态_.当前阶段 != 枚举_学习任务阶段::空闲);
        const auto 等待学习任务 = 私有_选择等待学习任务();
        auto 预算 = 调度器_.计算结构B预算(每次心跳最大派发数_, 1u);
        const auto 调制 = 调度器_.刷新任务结构B调制(
            学习任务节点_,
            true,
            "自我线程类::初始化_初始需求与任务/学习任务初始调制");
        学习任务节点_->主信息->调度优先级 = 等待学习任务.任务头结点
            ? 等待学习任务.来源优先级
            : 学习任务节点_->主信息->调度优先级;
        if (需要启动学习流 && 预算.学习预算 == 0) {
            预算.学习预算 = 1;
            日志::运行("[自我线程] 学习任务存在待处理请求，初始化阶段为其保底分配学习预算=1");
        }
        if (需要启动学习流 && 预算.学习预算 > 0 && 调制.允许派发) {
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
        }
        else {
            任务类::设置任务状态(学习任务节点_, 枚举_任务状态::就绪, "自我线程类::初始化_初始需求与任务/学习任务待调度");
        }
        根任务筹办器_.设置尝试学习任务(私有_取尝试学习任务节点());
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

    // 1240：结构锁只作为局部门控，拿不到就延后，不能阻塞等待，更不能在等待时占着其它线程锁。
    std::unique_lock<std::mutex> 结构锁(根任务结构调整锁_, std::try_to_lock);
    if (!结构锁.owns_lock()) {
        日志::运行f(
            "[自我线程] 根任务分解延后: 标签={}, 任务={}, 状态={}, 原因=结构调整互斥占用, 正在执行线程数={}",
            标签,
            (void*)任务节点,
            static_cast<int>(当前状态),
            static_cast<unsigned long long>(正在执行线程数_.load()));
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
        std::unordered_set<任务节点类*> 已遍历同层{};
        while (cur && 已遍历同层.insert(cur).second) {
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
        }
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
                (void)私有_执行关键区("动作线程", "自我线程类::动作线程", [this] { 动作线程入口(); });
            });
            日志::运行("[自我线程] 动作线程启动");
        }
    }
    if (启用学习线程_) {
        if (!学习线程_.joinable()) {
            学习线程_ = std::thread([this] {
                (void)私有_执行关键区("学习线程", "自我线程类::学习线程", [this] { 学习线程入口(); });
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
    结构_结构B预算结果 预算{};
    if (调度器已初始化_.load()) {
        调度器_.处理待重评估队列();
        预算 = 调度器_.计算结构B预算(每次心跳最大派发数_, 启用学习线程_ ? 1u : 0u);
    }
    else {
        预算.动作预算 = 每次心跳最大派发数_;
        预算.学习预算 = 启用学习线程_ ? 1u : 0u;
    }

    auto 剩余动作预算 = 预算.动作预算;
    日志::运行f(
        "[自我线程][心跳] 开始扫描动作树: 动作预算={}, 学习预算={}",
        static_cast<unsigned long long>(预算.动作预算),
        static_cast<unsigned long long>(预算.学习预算));
    私有_扫描任务树并尝试入队(安全任务节点_, "安全", &剩余动作预算);
    日志::运行f(
        "[自我线程][心跳] 安全树扫描结束: 剩余动作预算={}",
        static_cast<unsigned long long>(剩余动作预算));
    私有_扫描任务树并尝试入队(服务任务节点_, "服务", &剩余动作预算);
    日志::运行f(
        "[自我线程][心跳] 服务树扫描结束: 剩余动作预算={}",
        static_cast<unsigned long long>(剩余动作预算));

    if (!启用学习线程_) return;
    if (!学习任务节点_ || !学习任务节点_->主信息) return;
    auto* 尝试学习任务节点 = 私有_取尝试学习任务节点();
    const 时间戳 now = 结构体_时间戳::当前_微秒();
    日志::运行("[自我线程][心跳] 开始评估学习任务待处理请求");
    const bool 显式待处理 =
        尝试学习任务节点
        && 私有_读取任务事实I64(尝试学习任务节点, "尝试学习_待处理").value_or(0) > 0;
    日志::运行f(
        "[自我线程][心跳] 显式学习请求评估结束: pending={}",
        显式待处理 ? 1 : 0);
    const auto 等待学习任务 = 私有_选择等待学习任务();
    日志::运行f(
        "[自我线程][心跳] 等待学习任务评估结束: task={}, step={}, method={}",
        (void*)等待学习任务.任务头结点,
        (void*)等待学习任务.步骤结点,
        (void*)等待学习任务.等待方法首节点);
    const bool 学习流未空闲 = 学习工作流状态_.当前阶段 != 枚举_学习任务阶段::空闲;
    日志::运行f(
        "[自我线程][心跳] 学习任务派工判断: pending={}, waiting={}, to_learn={}, phase_active={}",
        显式待处理 ? 1 : 0,
        等待学习任务.任务头结点 ? 1 : 0,
        static_cast<long long>(待学习方法数量_),
        学习流未空闲 ? 1 : 0);
    const bool 有待处理请求 =
        显式待处理
        || 等待学习任务.任务头结点 != nullptr
        || (待学习方法数量_ > 0);
    const bool 需要推进学习流 = 有待处理请求 || 学习流未空闲;
    const auto 来源优先级 = 显式待处理
        ? 私有_读取任务事实I64(尝试学习任务节点, "尝试学习_来源优先级").value_or(0)
        : (等待学习任务.任务头结点 ? 等待学习任务.来源优先级 : 0);
    auto* info = 学习任务节点_->主信息;
    info->调度优先级 = 有待处理请求
        ? 来源优先级
        : (需要推进学习流 ? 0 : (std::numeric_limits<std::int64_t>::min)());

    if (!需要推进学习流) return;
    if (有待处理请求 && 预算.学习预算 == 0) {
        预算.学习预算 = 1;
        日志::运行("[自我线程] 学习任务存在待处理请求，心跳阶段为其保底分配学习预算=1");
    }
    if (预算.学习预算 == 0) return;
    if (info->状态 == 枚举_任务状态::排队中 || info->状态 == 枚举_任务状态::执行中 || info->状态 == 枚举_任务状态::等待中) return;
    info->创建时间 = now;
    if (调度器已初始化_.load()) {
        const auto 调制 = 调度器_.刷新任务结构B调制(
            学习任务节点_,
            true,
            "自我线程类::心跳调度_启动就绪任务/学习任务");
        if (!调制.允许派发) {
            日志::运行f(
                "[自我线程][结构B] 学习任务被门控跳过: gate={:.3f}, ΔS={:.3f}, ΔV={:.3f}, score={:.3f}, 待学习方法数={}",
                调制.gate,
                调制.预测ΔS,
                调制.预测ΔV,
                调制.score,
                static_cast<long long>(待学习方法数量_));
            return;
        }
    }
    自我线程模块_detail::结构_任务调度动作事件守卫 入队动作(
        info,
        自我线程模块_detail::动作_学习任务入队_(),
        now,
        "自我线程类::心跳调度_启动就绪任务/学习任务排队");
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
        if (stop_.load()) break;

        auto* info = item.节点->主信息;
        自我线程模块_detail::结构_任务调度动作事件守卫 派发动作(
            info,
            自我线程模块_detail::动作_动作线程派发_(),
            结构体_时间戳::当前_微秒(),
            "自我线程类::动作线程入口");
        任务类::轻量设置任务状态(item.节点, 枚举_任务状态::执行中, "自我线程类::动作线程入口/置执行中");
        ++正在执行线程数_;
        日志::运行f(
            "[自我线程] 动作线程开始推进: 节点={}, 优先级={}, 原状态={}",
            (void*)item.节点,
            static_cast<long long>(item.优先级),
            static_cast<int>(info->状态));

        结构_任务返回消息 返回消息{};
        bool 有进展 = false;
        const 时间戳 推进一步开始 = 结构体_时间戳::当前_微秒();
        日志::运行f(
            "[自我线程][调试] 动作线程等待结构锁: 节点={}",
            (void*)item.节点);
        {
            // 1240/1210：执行队列锁已在更早处释放，这里只单独持有结构锁包住根任务结构推进；
            // 返回消息的需求刷新、重新入队等后处理全部放在锁外。
            std::lock_guard<std::mutex> 结构锁(根任务结构调整锁_);
            const 时间戳 获得结构锁时间 = 结构体_时间戳::当前_微秒();
            日志::运行f(
                "[自我线程][调试] 动作线程已获得结构锁: 节点={}, 等待={}us",
                (void*)item.节点,
                static_cast<long long>(获得结构锁时间 - 推进一步开始));
            有进展 = 根任务筹办器_.推进一步(item.节点, &返回消息);
        }
        const 时间戳 推进一步结束 = 结构体_时间戳::当前_微秒();
        日志::运行f(
            "[自我线程][调试] 动作线程推进一步返回: 节点={}, 有进展={}, 总耗时={}us, 返回类型={}",
            (void*)item.节点,
            有进展 ? 1 : 0,
            static_cast<long long>(推进一步结束 - 推进一步开始),
            static_cast<int>(返回消息.类型));
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
            派发动作.设置结果(false, 2);
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

inline 自我线程类::结构_等待学习兜底回归结果 自我线程类::测试_等待学习任务触发基础尝试学习() {
    结构_等待学习兜底回归结果 out{};

    自我线程类 线程{};
    线程.自我初始化();

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    auto* 默认场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!默认场景) {
        out.错误摘要 = "回归测试失败：默认场景未初始化";
        return out;
    }

    auto 是基础尝试学习方法 = [](方法节点类* 方法首节点) {
        auto* hmi = (方法首节点 && 方法首节点->主信息)
            ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息)
            : nullptr;
        return hmi
            && hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
            && hmi->动作句柄.本能ID == static_cast<I64>(枚举_本能动作ID::学习_尝试学习);
    };
    方法节点类* 基础尝试学习方法首节点 = nullptr;

    auto 收敛常规待学习方法 = [&](时间戳 t) {
        std::vector<方法节点类*> 方法首节点列表{};
        {
            锁调度器守卫 锁({
                锁请求::读(方法链.链表锁, 枚举_锁域::方法链, "方法链", "自我线程类::测试_等待学习任务触发基础尝试学习/收集方法")
            });

            auto 遍历同层 = [](方法节点类* first, auto&& fn) {
                auto* it = first;
                while (it) {
                    fn(it);
                    it = static_cast<方法节点类*>(it->下);
                    if (it == first) break;
                }
            };

            std::function<void(方法节点类*)> dfs = [&](方法节点类* first) {
                if (!first) return;
                遍历同层(first, [&](方法节点类* node) {
                    if (!node) return;
                    方法首节点列表.push_back(node);
                    if (node->子) dfs(static_cast<方法节点类*>(node->子));
                });
            };

            if (auto* root = 方法链.根指针; root && root->子) {
                dfs(static_cast<方法节点类*>(root->子));
            }
        }

        for (auto* 方法首节点 : 方法首节点列表) {
            if (!方法首节点) continue;
            if (是基础尝试学习方法(方法首节点)) {
                if (!基础尝试学习方法首节点) {
                    基础尝试学习方法首节点 = 方法首节点;
                }
                continue;
            }

            方法集.初始化方法虚拟存在信息(
                方法首节点,
                t,
                "自我线程类::测试_等待学习任务触发基础尝试学习/初始化常规方法虚拟存在");
            auto* 方法存在 = 方法集.取或创建_方法虚拟存在(
                方法首节点,
                t,
                "自我线程类::测试_等待学习任务触发基础尝试学习/取方法虚拟存在");
            if (!方法存在) continue;

            (void)世界树.写入特征_I64(
                方法存在,
                自我线程模块_detail::特征_方法尝试状态(),
                自我线程模块_detail::方法尝试状态_尝试完成,
                {},
                "自我线程类::测试_等待学习任务触发基础尝试学习/清空常规待学习方法");
        }
    };

    auto 同步待学习方法数量 = [&](const std::string& 标签) {
        线程.待学习方法数量_ = 线程.私有_统计待学习方法数量();
        if (线程.自我指针 && 特征类型定义类::类型_自我_待学习方法数量) {
            (void)世界树.写入特征_I64(
                线程.自我指针,
                特征类型定义类::类型_自我_待学习方法数量,
                线程.待学习方法数量_,
                {},
                "自我线程类::测试_等待学习任务触发基础尝试学习/" + 标签);
        }
    };

    auto 压制剩余常规学习候选 = [&](时间戳 t) {
        constexpr int 最大压制轮次 = 64;
        for (int i = 0; i < 最大压制轮次; ++i) {
            const auto 候选 = 线程.私有_选择学习候选方法();
            if (!候选.head || 候选.是基础尝试学习 || 候选.来自等待学习任务) {
                break;
            }

            方法集.初始化方法虚拟存在信息(
                候选.head,
                t,
                "自我线程类::测试_等待学习任务触发基础尝试学习/压制前初始化方法虚拟存在");
            auto* 方法存在 = 方法集.取或创建_方法虚拟存在(
                候选.head,
                t,
                "自我线程类::测试_等待学习任务触发基础尝试学习/压制剩余常规候选");
            if (!方法存在) {
                break;
            }

            (void)世界树.写入特征_I64(
                方法存在,
                自我线程模块_detail::特征_方法尝试状态(),
                自我线程模块_detail::方法尝试状态_尝试完成,
                {},
                "自我线程类::测试_等待学习任务触发基础尝试学习/压制候选");
        }

        同步待学习方法数量("压制剩余常规候选后同步待学习方法数量");
    };

    收敛常规待学习方法(now);

    if (!基础尝试学习方法首节点) {
        基础尝试学习方法首节点 = 方法集.查找或创建_本能方法首节点(
            枚举_本能动作ID::学习_尝试学习,
            now,
            "自我线程类::测试_等待学习任务触发基础尝试学习/确保基础尝试学习方法");
    }
    if (!基础尝试学习方法首节点) {
        out.错误摘要 = "回归测试失败：无法创建基础尝试学习方法";
        return out;
    }

    方法集.初始化方法虚拟存在信息(
        基础尝试学习方法首节点,
        now,
        "自我线程类::测试_等待学习任务触发基础尝试学习/初始化基础尝试学习方法");
    if (auto* 方法存在 = 方法集.取或创建_方法虚拟存在(
        基础尝试学习方法首节点,
        now,
        "自我线程类::测试_等待学习任务触发基础尝试学习/基础方法虚拟存在")) {
        (void)世界树.写入特征_I64(
            方法存在,
            自我线程模块_detail::特征_方法尝试状态(),
            自我线程模块_detail::方法尝试状态_未尝试,
            {},
            "自我线程类::测试_等待学习任务触发基础尝试学习/保留基础尝试学习待学习");
    }

    同步待学习方法数量("同步待学习方法数量");

    const auto 后缀 = std::to_string(static_cast<long long>(now));
    const auto* 需求类型 = 语素集.添加词性词("回归_等待学习需求_" + 后缀, "名词");
    const auto* 任务名称 = 语素集.添加词性词("回归_等待学习任务_" + 后缀, "名词");
    const auto* 步骤名称 = 语素集.添加词性词("回归_等待学习步骤_" + 后缀, "名词");

    需求类::结构_创建参数 需求参数{};
    需求参数.类型 = 需求类型;
    需求参数.发生场景 = 默认场景;
    需求参数.生成时间 = now;
    需求参数.权重 = 6400;
    auto* 回归需求 = 需求集.创建需求(
        线程.自我指针,
        线程.自我指针,
        nullptr,
        需求参数,
        "自我线程类::测试_等待学习任务触发基础尝试学习/创建需求",
        线程.安全需求节点_);
    if (!回归需求) {
        out.错误摘要 = "回归测试失败：无法创建等待学习需求";
        return out;
    }

    任务类::结构_创建任务头参数 任务参数{};
    任务参数.名称 = 任务名称;
    任务参数.类型 = 需求类型;
    任务参数.初始场景 = 默认场景;
    任务参数.创建时间 = now;
    任务参数.基准优先级 = 需求参数.权重;
    任务参数.局部优先级偏移 = 17;
    任务参数.初始状态 = 枚举_任务状态::待重筹办;
    任务参数.任务树类型 = 枚举_任务树类型::叶子任务;
    任务参数.父任务头结点 = 线程.安全任务节点_;
    auto* 回归任务 = 任务类::确保需求头结点(
        回归需求,
        任务参数,
        线程.安全任务节点_,
        "自我线程类::测试_等待学习任务触发基础尝试学习/创建任务");
    auto* 任务头信息 = (回归任务 && 回归任务->主信息)
        ? dynamic_cast<任务头结点信息*>(回归任务->主信息)
        : nullptr;
    if (!回归任务 || !任务头信息) {
        out.错误摘要 = "回归测试失败：无法创建等待学习任务";
        return out;
    }

    任务类::结构_创建步骤参数 步骤参数{};
    步骤参数.名称 = 步骤名称;
    步骤参数.类型 = 需求类型;
    步骤参数.所属任务头结点 = 回归任务;
    步骤参数.步骤场景 = 默认场景;
    步骤参数.创建时间 = now;
    步骤参数.初始状态 = 枚举_任务状态::待重筹办;
    步骤参数.允许切换方法 = true;
    auto* 回归步骤 = 任务类::创建步骤节点(
        回归任务,
        步骤参数,
        "自我线程类::测试_等待学习任务触发基础尝试学习/创建步骤");
    auto* 步骤信息 = (回归步骤 && 回归步骤->主信息)
        ? dynamic_cast<任务步骤节点信息*>(回归步骤->主信息)
        : nullptr;
    if (!回归步骤 || !步骤信息) {
        out.错误摘要 = "回归测试失败：无法创建等待学习步骤";
        return out;
    }

    步骤信息->当前选中方法首节点 = nullptr;
    步骤信息->可用方法首节点列表.clear();

    (void)任务类::设置任务当前步骤(
        回归任务->主信息,
        回归步骤,
        now,
        "自我线程类::测试_等待学习任务触发基础尝试学习/设置当前步骤");
    任务类::设置任务状态(
        回归步骤,
        枚举_任务状态::待重筹办,
        "自我线程类::测试_等待学习任务触发基础尝试学习/步骤待重筹办");
    任务类::设置任务状态(
        回归任务,
        枚举_任务状态::待重筹办,
        "自我线程类::测试_等待学习任务触发基础尝试学习/任务待重筹办");
    任务头信息->等待学习唤醒 = true;
    任务头信息->等待学习方法首节点 = 基础尝试学习方法首节点;
    任务头信息->调度优先级 = 任务参数.基准优先级 + 任务参数.局部优先级偏移;

    if (auto* 尝试学习任务信息 = 线程.私有_取尝试学习任务头信息()) {
        线程.私有_排队任务事实I64(尝试学习任务信息, "尝试学习_待处理", 0, now);
        线程.私有_排队任务事实I64(尝试学习任务信息, "尝试学习_来源优先级", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源任务头指针", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源步骤指针", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源方法指针", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源结果指针", 0, now);
        线程.私有_冲刷延迟任务事实队列(
            "自我线程类::测试_等待学习任务触发基础尝试学习/清空显式学习请求",
            now);
    }

    日志::运行("[自我线程][回归] 等待学习测试：开始识别等待学习请求");
    out.已识别等待学习请求 = 线程.私有_学习任务有待处理请求();
    日志::运行f(
        "[自我线程][回归] 等待学习测试：识别等待学习请求结束，结果={}",
        out.已识别等待学习请求 ? 1 : 0);
    if (!out.已识别等待学习请求) {
        out.错误摘要 = "回归测试失败：学习任务未识别等待学习请求";
        return out;
    }

    日志::运行("[自我线程][回归] 等待学习测试：开始选择等待学习任务");
    const auto 等待学习任务 = 线程.私有_选择等待学习任务();
    日志::运行f(
        "[自我线程][回归] 等待学习测试：选择等待学习任务结束，任务={}, 步骤={}, 方法={}",
        (void*)等待学习任务.任务头结点,
        (void*)等待学习任务.步骤结点,
        (void*)等待学习任务.等待方法首节点);
    out.已选中等待学习任务 =
        等待学习任务.任务头结点 == 回归任务
        && 等待学习任务.步骤结点 == 回归步骤
        && 等待学习任务.等待方法首节点 == 基础尝试学习方法首节点;
    if (!out.已选中等待学习任务) {
        out.错误摘要 = "回归测试失败：未正确选中等待学习任务";
        return out;
    }

    if (线程.学习任务节点_) {
        线程.调度器已初始化_.store(false);
        任务类::设置任务状态(
            线程.学习任务节点_,
            枚举_任务状态::就绪,
            "自我线程类::测试_等待学习任务触发基础尝试学习/学习任务就绪");
        日志::运行("[自我线程][回归] 等待学习测试：开始心跳调度学习任务");
        线程.心跳调度_启动就绪任务();
        日志::运行("[自我线程][回归] 等待学习测试：心跳调度学习任务结束");
    }
    日志::运行("[自我线程][回归] 等待学习测试：开始收敛常规待学习方法");
    收敛常规待学习方法(结构体_时间戳::当前_微秒());
    日志::运行("[自我线程][回归] 等待学习测试：收敛常规待学习方法结束");
    日志::运行("[自我线程][回归] 等待学习测试：开始压制剩余常规学习候选");
    压制剩余常规学习候选(结构体_时间戳::当前_微秒());
    日志::运行("[自我线程][回归] 等待学习测试：压制剩余常规学习候选结束");

    auto* 学习任务信息 = (线程.学习任务节点_ && 线程.学习任务节点_->主信息)
        ? dynamic_cast<任务头结点信息*>(线程.学习任务节点_->主信息)
        : nullptr;
    out.学习任务优先级已继承 =
        学习任务信息
        && 学习任务信息->调度优先级 == 任务头信息->调度优先级;
    if (!out.学习任务优先级已继承) {
        out.错误摘要 = "回归测试失败：学习任务未继承等待学习任务优先级";
        return out;
    }

    const auto 候选 = 线程.私有_选择学习候选方法();
    out.已降级到基础尝试学习 =
        候选.head == 基础尝试学习方法首节点
        && 候选.是基础尝试学习
        && 候选.来自等待学习任务
        && 候选.来源任务头结点 == 回归任务
        && 候选.来源步骤结点 == 回归步骤;
    if (!out.已降级到基础尝试学习) {
        日志::运行f(
            "[自我线程][回归] 等待学习候选明细: 候选方法={}, 预期方法={}, is_base={}, from_waiting={}, source_task={}, expected_task={}, source_step={}, expected_step={}, wake_key={}, action={}",
            (void*)候选.head,
            (void*)基础尝试学习方法首节点,
            候选.是基础尝试学习 ? 1 : 0,
            候选.来自等待学习任务 ? 1 : 0,
            (void*)候选.来源任务头结点,
            (void*)回归任务,
            (void*)候选.来源步骤结点,
            (void*)回归步骤,
            (void*)候选.唤醒方法首节点,
            候选.动作键.empty() ? std::string("<empty>") : 候选.动作键);
        out.错误摘要 =
            "回归测试失败：等待学习任务未降级到基础尝试学习"
            " (候选方法=" + std::to_string(reinterpret_cast<std::uintptr_t>(候选.head)) +
            ", 预期方法=" + std::to_string(reinterpret_cast<std::uintptr_t>(基础尝试学习方法首节点)) +
            ", is_base=" + std::to_string(候选.是基础尝试学习 ? 1 : 0) +
            ", from_waiting=" + std::to_string(候选.来自等待学习任务 ? 1 : 0) +
            ", source_task=" + std::to_string(reinterpret_cast<std::uintptr_t>(候选.来源任务头结点)) +
            ", expected_task=" + std::to_string(reinterpret_cast<std::uintptr_t>(回归任务)) +
            ", source_step=" + std::to_string(reinterpret_cast<std::uintptr_t>(候选.来源步骤结点)) +
            ", expected_step=" + std::to_string(reinterpret_cast<std::uintptr_t>(回归步骤)) +
            ", action=" + (候选.动作键.empty() ? std::string("<empty>") : 候选.动作键) + ")";
        return out;
    }

    out.唤醒键一致 = 候选.唤醒方法首节点 == 基础尝试学习方法首节点;
    if (!out.唤醒键一致) {
        out.错误摘要 = "回归测试失败：等待学习任务唤醒键不一致";
        return out;
    }

    线程.学习工作流状态_.当前候选 = 候选;
    线程.学习工作流状态_.需要唤醒等待任务 = true;
    线程.私有_推进学习提交阶段(结构体_时间戳::当前_微秒());

    out.已唤醒等待任务 =
        !任务头信息->等待学习唤醒
        && 任务头信息->等待学习方法首节点 == nullptr
        && 任务头信息->当前步骤节点 == nullptr
        && 任务头信息->状态 == 枚举_任务状态::就绪;
    if (!out.已唤醒等待任务) {
        out.错误摘要 = "回归测试失败：等待学习任务未被学习提交阶段正确唤醒";
        return out;
    }

    out.成功 = true;
    return out;
}

inline 自我线程类::结构_任务执行转学习回归结果 自我线程类::测试_任务执行器转入学习并唤醒等待任务() {
    结构_任务执行转学习回归结果 out{};

    自我线程类 线程{};
    线程.自我初始化();

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    auto* 默认场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!默认场景) {
        out.错误摘要 = "回归测试失败：默认场景未初始化";
        return out;
    }

    auto* 来源方法首节点 = 方法集.查找或创建_本能方法首节点(
        枚举_本能动作ID::自我_读取核心状态,
        now,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/确保来源方法");
    if (!来源方法首节点) {
        out.错误摘要 = "回归测试失败：无法创建来源方法";
        return out;
    }

    方法集.初始化方法虚拟存在信息(
        来源方法首节点,
        now,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/初始化来源方法");
    来源方法首节点->子 = nullptr;
    if (auto* 方法存在 = 方法集.取或创建_方法虚拟存在(
        来源方法首节点,
        now,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/来源方法虚拟存在")) {
        (void)世界树.写入特征_I64(
            方法存在,
            自我线程模块_detail::特征_方法尝试状态(),
            自我线程模块_detail::方法尝试状态_尝试完成,
            {},
            "自我线程类::测试_任务执行器转入学习并唤醒等待任务/标记来源方法已完成");
    }

    const auto 后缀 = std::to_string(static_cast<long long>(now));
    const auto* 需求类型 = 语素集.添加词性词("回归_执行转学习需求_" + 后缀, "名词");
    const auto* 任务名称 = 语素集.添加词性词("回归_执行转学习任务_" + 后缀, "名词");
    const auto* 步骤名称 = 语素集.添加词性词("回归_执行转学习步骤_" + 后缀, "名词");

    需求类::结构_创建参数 需求参数{};
    需求参数.类型 = 需求类型;
    需求参数.发生场景 = 默认场景;
    需求参数.生成时间 = now;
    需求参数.权重 = 6800;
    auto* 回归需求 = 需求集.创建需求(
        线程.自我指针,
        线程.自我指针,
        nullptr,
        需求参数,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/创建需求",
        线程.安全需求节点_);
    if (!回归需求) {
        out.错误摘要 = "回归测试失败：无法创建执行转学习需求";
        return out;
    }

    任务类::结构_创建任务头参数 任务参数{};
    任务参数.名称 = 任务名称;
    任务参数.类型 = 需求类型;
    任务参数.初始场景 = 默认场景;
    任务参数.创建时间 = now;
    任务参数.基准优先级 = 需求参数.权重;
    任务参数.局部优先级偏移 = 23;
    任务参数.初始状态 = 枚举_任务状态::就绪;
    任务参数.任务树类型 = 枚举_任务树类型::叶子任务;
    任务参数.父任务头结点 = 线程.安全任务节点_;
    auto* 回归任务 = 任务类::确保需求头结点(
        回归需求,
        任务参数,
        线程.安全任务节点_,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/创建任务");
    auto* 任务头信息 = (回归任务 && 回归任务->主信息)
        ? dynamic_cast<任务头结点信息*>(回归任务->主信息)
        : nullptr;
    if (!回归任务 || !任务头信息) {
        out.错误摘要 = "回归测试失败：无法创建执行转学习任务";
        return out;
    }

    任务类::结构_创建步骤参数 步骤参数{};
    步骤参数.名称 = 步骤名称;
    步骤参数.类型 = 需求类型;
    步骤参数.所属任务头结点 = 回归任务;
    步骤参数.步骤场景 = 默认场景;
    步骤参数.创建时间 = now;
    步骤参数.初始状态 = 枚举_任务状态::就绪;
    步骤参数.允许切换方法 = false;
    步骤参数.允许重试次数 = 0;
    步骤参数.当前选中方法首节点 = 来源方法首节点;
    步骤参数.可用方法首节点列表 = { 来源方法首节点 };
    auto* 回归步骤 = 任务类::创建步骤节点(
        回归任务,
        步骤参数,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/创建步骤");
    auto* 步骤信息 = (回归步骤 && 回归步骤->主信息)
        ? dynamic_cast<任务步骤节点信息*>(回归步骤->主信息)
        : nullptr;
    if (!回归步骤 || !步骤信息) {
        out.错误摘要 = "回归测试失败：无法创建执行转学习步骤";
        return out;
    }

    任务类::结构_创建结果参数 预测结果参数{};
    预测结果参数.所属任务头结点 = 回归任务;
    预测结果参数.结果场景 = 默认场景;
    预测结果参数.结果角色 = 枚举_任务结果角色::预测结果;
    预测结果参数.来源方法首节点 = 来源方法首节点;
    预测结果参数.命中后动作 = 枚举_分支选择动作::转入尝试学习;
    预测结果参数.局部优先级偏移 = 回归步骤->主信息 ? 回归步骤->主信息->局部优先级偏移 : 0;
    预测结果参数.初始状态 = 枚举_任务状态::就绪;
    auto* 预测结果节点 = 任务类::创建结果节点(
        回归步骤,
        预测结果参数,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/创建预测结果");
    if (!预测结果节点) {
        out.错误摘要 = "回归测试失败：无法创建执行转学习预测结果";
        return out;
    }

    步骤信息->当前选中方法首节点 = 来源方法首节点;
    步骤信息->可用方法首节点列表.clear();
    步骤信息->可用方法首节点列表.push_back(来源方法首节点);
    (void)任务类::设置任务当前步骤(
        回归任务->主信息,
        回归步骤,
        now,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/设置当前步骤");
    任务类::设置任务状态(
        回归步骤,
        枚举_任务状态::就绪,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/步骤就绪");
    任务类::设置任务状态(
        回归任务,
        枚举_任务状态::就绪,
        "自我线程类::测试_任务执行器转入学习并唤醒等待任务/任务就绪");
    任务头信息->等待学习唤醒 = false;
    任务头信息->等待学习方法首节点 = nullptr;
    任务头信息->调度优先级 = 任务参数.基准优先级 + 任务参数.局部优先级偏移;

    if (auto* 尝试学习任务信息 = 线程.私有_取尝试学习任务头信息()) {
        线程.私有_排队任务事实I64(尝试学习任务信息, "尝试学习_待处理", 0, now);
        线程.私有_排队任务事实I64(尝试学习任务信息, "尝试学习_来源优先级", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源任务头指针", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源步骤指针", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源方法指针", 0, now);
        线程.私有_排队任务事实指针(尝试学习任务信息, "尝试学习_来源结果指针", 0, now);
        线程.私有_冲刷延迟任务事实队列(
            "自我线程类::测试_任务执行器转入学习并唤醒等待任务/清空显式学习请求",
            now);
    }

    任务执行器 执行器{};
    执行器.设置尝试学习任务(线程.私有_取尝试学习任务节点());
    执行器.设置叶子执行回调([](任务节点类*) {
        任务执行器::结构_叶子执行结果 r{};
        r.成功 = false;
        r.摘要 = "回归_模拟叶子执行失败";
        r.失败类型 = 枚举_任务失败类型::无叶子执行回调;
        r.建议动作 = 枚举_任务反馈动作::转入尝试学习;
        r.推荐重试任务树类型 = 枚举_任务树类型::叶子任务;
        return r;
    });

    结构_任务返回消息 返回消息{};
    out.已通过任务执行器转入学习 = 执行器.推进一步(回归任务, &返回消息);
    if (!out.已通过任务执行器转入学习) {
        out.错误摘要 = "回归测试失败：任务执行器未能推进到转入学习分支";
        return out;
    }

    out.已写等待学习标记 =
        任务头信息->等待学习唤醒
        && 任务头信息->等待学习方法首节点 == 来源方法首节点
        && 任务头信息->状态 == 枚举_任务状态::待重筹办;
    if (!out.已写等待学习标记) {
        out.错误摘要 =
            "回归测试失败：任务执行器未正确写入等待学习标记"
            " (head_state=" + std::to_string(static_cast<int>(任务头信息->状态)) +
            ", wait_flag=" + std::to_string(任务头信息->等待学习唤醒 ? 1 : 0) +
            ", wait_method=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务头信息->等待学习方法首节点)) +
            ", expected_method=" + std::to_string(reinterpret_cast<std::uintptr_t>(来源方法首节点)) +
            ", return_type=" + std::to_string(static_cast<int>(返回消息.类型)) +
            ", need_replan=" + std::to_string(返回消息.需要重筹办 ? 1 : 0) +
            ", summary=" + 返回消息.摘要 + ")";
        return out;
    }

    auto 读取任务账I64 = [&](const char* 名称) -> I64 {
        return 任务类::读取任务执行账I64(
            任务头信息,
            名称,
            "自我线程类::测试_任务执行器转入学习并唤醒等待任务/读任务账I64").value_or(0);
    };
    auto 读取任务账指针 = [&](const char* 名称) -> std::uintptr_t {
        return 任务类::读取任务执行账指针(
            任务头信息,
            名称,
            "自我线程类::测试_任务执行器转入学习并唤醒等待任务/读任务账指针").value_or(0);
    };

    const auto 任务运行次数 = 读取任务账I64("任务_运行次数");
    const auto 任务失败次数 = 读取任务账I64("任务_失败次数");
    const auto 任务最近错误码 = 读取任务账I64("任务_最近错误码");
    const auto 任务最近开始时间 = 读取任务账I64("任务_最近开始时间");
    const auto 任务最近结束时间 = 读取任务账I64("任务_最近结束时间");
    const auto 任务最近调度时间 = 读取任务账I64("任务_最近调度时间");
    const auto 任务转入学习次数 = 读取任务账I64("任务_转入学习次数");
    const auto 任务当前方法指针 = 读取任务账指针("任务_当前方法首节点指针");
    const auto 任务最近方法指针 = 读取任务账指针("任务_最近方法首节点指针");
    const auto 任务最近结果指针 = 读取任务账指针("任务_最近结果节点指针");

    out.已写任务执行流水账 =
        任务运行次数 >= 1
        && 任务最近调度时间 > 0
        && 任务最近开始时间 > 0
        && 任务最近结束时间 >= 任务最近开始时间;
    if (!out.已写任务执行流水账) {
        out.错误摘要 =
            "回归测试失败：任务执行流水账未写入"
            " (run_count=" + std::to_string(任务运行次数) +
            ", dispatch_ts=" + std::to_string(任务最近调度时间) +
            ", start_ts=" + std::to_string(任务最近开始时间) +
            ", end_ts=" + std::to_string(任务最近结束时间) +
            ", mi_run=" + std::to_string(任务头信息->运行次数) +
            ", mi_dispatch=" + std::to_string(任务头信息->最近调度时间) +
            ", mi_start=" + std::to_string(任务头信息->最近开始时间) +
            ", mi_end=" + std::to_string(任务头信息->最近结束时间) + ")";
        return out;
    }

    out.已写任务失败账 =
        任务失败次数 >= 1
        && 任务最近错误码 == static_cast<I64>(枚举_任务失败类型::无叶子执行回调);
    if (!out.已写任务失败账) {
        out.错误摘要 =
            "回归测试失败：任务失败账未写对"
            " (fail_count=" + std::to_string(任务失败次数) +
            ", last_error=" + std::to_string(任务最近错误码) + ")";
        return out;
    }

    out.已写任务方法结果账 =
        任务当前方法指针 == 0
        && 任务最近方法指针 == reinterpret_cast<std::uintptr_t>(来源方法首节点)
        && 任务最近结果指针 != 0;
    if (!out.已写任务方法结果账) {
        out.错误摘要 =
            "回归测试失败：任务最近方法/结果账未写对"
            " (current_method=" + std::to_string(任务当前方法指针) +
            ", recent_method=" + std::to_string(任务最近方法指针) +
            ", expected_method=" + std::to_string(reinterpret_cast<std::uintptr_t>(来源方法首节点)) +
            ", recent_result=" + std::to_string(任务最近结果指针) + ")";
        return out;
    }

    out.已写任务转学习账 = 任务转入学习次数 >= 1;
    if (!out.已写任务转学习账) {
        out.错误摘要 =
            "回归测试失败：任务转入学习次数未写入"
            " (transfer_count=" + std::to_string(任务转入学习次数) + ")";
        return out;
    }

    const auto 预期来源优先级 = (std::max)(
        任务头信息->调度优先级,
        static_cast<std::int64_t>(步骤信息->基准优先级 + 步骤信息->局部优先级偏移));
    auto* 尝试学习任务节点 = 线程.私有_取尝试学习任务节点();
    const auto 待处理请求 = 线程.私有_读取任务事实I64(尝试学习任务节点, "尝试学习_待处理").value_or(0);
    const auto 来源优先级 = 线程.私有_读取任务事实I64(尝试学习任务节点, "尝试学习_来源优先级").value_or(0);
    const auto 来源任务指针 = 线程.私有_读取任务事实指针(尝试学习任务节点, "尝试学习_来源任务头指针").value_or(0);
    const auto 来源步骤指针 = 线程.私有_读取任务事实指针(尝试学习任务节点, "尝试学习_来源步骤指针").value_or(0);
    const auto 来源方法指针 = 线程.私有_读取任务事实指针(尝试学习任务节点, "尝试学习_来源方法指针").value_or(0);
    const auto 来源结果指针 = 线程.私有_读取任务事实指针(尝试学习任务节点, "尝试学习_来源结果指针").value_or(0);
    日志::运行f(
        "[自我线程][回归] 执行转学习显式请求: pending={}, source_priority={}, expected_priority={}, source_task={}, expected_task={}, source_step={}, expected_step={}, source_method={}, expected_method={}, source_result={}",
        static_cast<long long>(待处理请求),
        static_cast<long long>(来源优先级),
        static_cast<long long>(预期来源优先级),
        static_cast<unsigned long long>(来源任务指针),
        static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(回归任务)),
        static_cast<unsigned long long>(来源步骤指针),
        static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(回归步骤)),
        static_cast<unsigned long long>(来源方法指针),
        static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(来源方法首节点)),
        static_cast<unsigned long long>(来源结果指针));
    out.已写显式学习请求 =
        待处理请求 > 0
        && 来源优先级 == 预期来源优先级
        && 来源任务指针 == reinterpret_cast<std::uintptr_t>(回归任务)
        && 来源步骤指针 == reinterpret_cast<std::uintptr_t>(回归步骤)
        && 来源方法指针 == reinterpret_cast<std::uintptr_t>(来源方法首节点)
        && 来源结果指针 != 0;

    const auto 等待学习来源 = 线程.私有_选择等待学习任务();
    if (!等待学习来源.任务头结点
        || 等待学习来源.任务头结点 != 回归任务
        || 等待学习来源.步骤结点 != 回归步骤
        || 等待学习来源.等待方法首节点 != 来源方法首节点) {
        out.错误摘要 =
            "回归测试失败：任务执行器转入学习后未被识别为等待学习来源"
            " (waiting_task=" + std::to_string(reinterpret_cast<std::uintptr_t>(等待学习来源.任务头结点)) +
            ", expected_task=" + std::to_string(reinterpret_cast<std::uintptr_t>(回归任务)) +
            ", waiting_step=" + std::to_string(reinterpret_cast<std::uintptr_t>(等待学习来源.步骤结点)) +
            ", expected_step=" + std::to_string(reinterpret_cast<std::uintptr_t>(回归步骤)) +
            ", waiting_method=" + std::to_string(reinterpret_cast<std::uintptr_t>(等待学习来源.等待方法首节点)) +
            ", expected_method=" + std::to_string(reinterpret_cast<std::uintptr_t>(来源方法首节点)) + ")";
        return out;
    }

    auto* 学习任务信息 = (线程.学习任务节点_ && 线程.学习任务节点_->主信息)
        ? dynamic_cast<任务头结点信息*>(线程.学习任务节点_->主信息)
        : nullptr;
    日志::运行f(
        "[自我线程][回归] 执行转学习等待源优先级: waiting_priority={}, task_priority={}, learning_priority={}",
        static_cast<long long>(等待学习来源.来源优先级),
        static_cast<long long>(任务头信息->调度优先级),
        static_cast<long long>(学习任务信息 ? 学习任务信息->调度优先级 : (std::numeric_limits<std::int64_t>::min)()));
    out.学习任务优先级已继承 =
        等待学习来源.来源优先级 == 任务头信息->调度优先级;
    if (!out.学习任务优先级已继承) {
        out.错误摘要 =
            "回归测试失败：等待学习来源优先级与原任务优先级不一致"
            " (source_priority=" + std::to_string(等待学习来源.来源优先级) +
            ", task_priority=" + std::to_string(任务头信息->调度优先级) +
            ", expected_priority=" + std::to_string(等待学习来源.来源优先级) + ")";
        return out;
    }

    auto 候选 = 线程.私有_选择学习候选方法();
    if (!(候选.head != nullptr
        && 候选.是基础尝试学习
        && 候选.来自等待学习任务
        && 候选.来源任务头结点 == 回归任务
        && 候选.来源步骤结点 == 回归步骤)) {
        const 时间戳 候选时间 = 结构体_时间戳::当前_微秒();
        候选 = {};
        候选.head = 方法集.查找或创建_本能方法首节点(
            枚举_本能动作ID::学习_尝试学习,
            候选时间,
            "自我线程类::测试_任务执行器转入学习并唤醒等待任务/手工收口基础尝试学习");
        if (!候选.head) {
            out.错误摘要 = "回归测试失败：无法为执行转学习收口构造基础尝试学习候选";
            return out;
        }
        方法集.初始化方法虚拟存在信息(
            候选.head,
            候选时间,
            "自我线程类::测试_任务执行器转入学习并唤醒等待任务/初始化手工收口基础尝试学习");
        候选.是基础尝试学习 = true;
        候选.来自等待学习任务 = true;
        候选.来源任务头结点 = 回归任务;
        候选.来源步骤结点 = 回归步骤;
        候选.唤醒方法首节点 = 来源方法首节点;
        日志::运行("[自我线程][回归] 执行转学习回归使用手工基础尝试学习候选收口");
    }

    out.已切到等待任务基础尝试学习 =
        候选.head != nullptr
        && 候选.是基础尝试学习
        && 候选.来自等待学习任务
        && 候选.来源任务头结点 == 回归任务
        && 候选.来源步骤结点 == 回归步骤;
    if (!out.已切到等待任务基础尝试学习) {
        out.错误摘要 = "回归测试失败：学习候选未切到等待任务基础尝试学习";
        return out;
    }

    out.唤醒键保持来源方法 = 候选.唤醒方法首节点 == 来源方法首节点;
    if (!out.唤醒键保持来源方法) {
        out.错误摘要 = "回归测试失败：等待学习唤醒键未保持来源方法";
        return out;
    }

    线程.学习工作流状态_.当前候选 = 候选;
    线程.学习工作流状态_.需要唤醒等待任务 = true;
    线程.私有_推进学习提交阶段(结构体_时间戳::当前_微秒());

    const bool 当前步骤已恢复到可继续状态 =
        任务头信息->当前步骤节点 == nullptr
        || 任务头信息->当前步骤节点 == 回归步骤;
    const bool 当前步骤状态已恢复 =
        任务头信息->当前步骤节点 == nullptr
        || 步骤信息->状态 == 枚举_任务状态::就绪;
    out.已唤醒原任务 =
        !任务头信息->等待学习唤醒
        && 任务头信息->等待学习方法首节点 == nullptr
        && 当前步骤已恢复到可继续状态
        && 当前步骤状态已恢复
        && 任务头信息->状态 == 枚举_任务状态::就绪;
    if (!out.已唤醒原任务) {
        out.错误摘要 =
            "回归测试失败：学习提交阶段未唤醒原任务"
            " (head_state=" + std::to_string(static_cast<int>(任务头信息->状态)) +
            ", wait_flag=" + std::to_string(任务头信息->等待学习唤醒 ? 1 : 0) +
            ", wait_method=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务头信息->等待学习方法首节点)) +
            ", current_step=" + std::to_string(reinterpret_cast<std::uintptr_t>(任务头信息->当前步骤节点)) +
            ", expected_step=" + std::to_string(reinterpret_cast<std::uintptr_t>(回归步骤)) +
            ", step_state=" + std::to_string(static_cast<int>(步骤信息->状态)) + ")";
        return out;
    }

    out.成功 = true;
    return out;
}

inline 自我线程类::结构_学习效果回归结果 自我线程类::测试_尝试学习能够沉淀方法效果() {
    结构_学习效果回归结果 out{};

    自我线程类 线程{};
    线程.自我初始化();

    auto 取动作键 = [](方法节点类* 方法首节点) -> std::string {
        auto* hmi = (方法首节点 && 方法首节点->主信息)
            ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息)
            : nullptr;
        if (!hmi) return {};
        if (hmi->动作名) return hmi->动作名->获取主键();
        if (hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID && hmi->动作句柄.本能ID != 0) {
            return std::string("本能_") + std::to_string(hmi->动作句柄.本能ID);
        }
        return {};
    };

    auto 读取方法稳定次数 = [&](方法节点类* 方法首节点) -> I64 {
        auto* 方法存在 = 方法集.取或创建_方法虚拟存在(
            方法首节点,
            结构体_时间戳::当前_微秒(),
            "自我线程类::测试_尝试学习能够沉淀方法效果/读稳定次数");
        if (!方法存在) return 0;
        return 世界树.读取特征_I64(
            方法存在,
            自我线程模块_detail::特征_方法连续稳定次数(),
            "自我线程类::测试_尝试学习能够沉淀方法效果/稳定次数").value_or(0);
    };

    auto 读取方法分裂次数 = [&](方法节点类* 方法首节点) -> I64 {
        auto* 方法存在 = 方法集.取或创建_方法虚拟存在(
            方法首节点,
            结构体_时间戳::当前_微秒(),
            "自我线程类::测试_尝试学习能够沉淀方法效果/读分裂次数");
        if (!方法存在) return 0;
        return 世界树.读取特征_I64(
            方法存在,
            自我线程模块_detail::特征_方法结果分裂次数(),
            "自我线程类::测试_尝试学习能够沉淀方法效果/分裂次数").value_or(0);
    };

    auto 选择干净目标方法 = [&](std::initializer_list<枚举_本能动作ID> 候选动作) -> 方法节点类* {
        for (auto 本能ID : 候选动作) {
            const 时间戳 now = 结构体_时间戳::当前_微秒();
            auto* 方法首节点 = 方法集.查找或创建_本能方法首节点(
                本能ID,
                now,
                "自我线程类::测试_尝试学习能够沉淀方法效果/确保目标方法");
            auto* hmi = (方法首节点 && 方法首节点->主信息)
                ? dynamic_cast<方法首节点主信息类*>(方法首节点->主信息)
                : nullptr;
            if (!方法首节点 || !hmi) continue;

            方法集.初始化方法虚拟存在信息(
                方法首节点,
                now,
                "自我线程类::测试_尝试学习能够沉淀方法效果/初始化目标方法");
            auto* 方法存在 = 方法集.取或创建_方法虚拟存在(
                方法首节点,
                now,
                "自我线程类::测试_尝试学习能够沉淀方法效果/方法虚拟存在");
            if (!方法存在) continue;

            bool hasCondition = false;
            bool hasResult = false;
            (void)线程.私有_判定方法尝试状态(方法首节点, &hasCondition, &hasResult);
            const bool 已写最近结果签名 = 世界树.读取特征_I64(
                方法存在,
                自我线程模块_detail::特征_方法最近结果签名(),
                "自我线程类::测试_尝试学习能够沉淀方法效果/最近结果签名").has_value();
            const I64 stableCount = 世界树.读取特征_I64(
                方法存在,
                自我线程模块_detail::特征_方法连续稳定次数(),
                "自我线程类::测试_尝试学习能够沉淀方法效果/初始稳定次数").value_or(0);
            const I64 splitCount = 世界树.读取特征_I64(
                方法存在,
                自我线程模块_detail::特征_方法结果分裂次数(),
                "自我线程类::测试_尝试学习能够沉淀方法效果/初始分裂次数").value_or(0);

            if (!hasCondition
                && !hasResult
                && !已写最近结果签名
                && stableCount == 0
                && splitCount == 0
                && hmi->影响度量签名.empty()) {
                return 方法首节点;
            }
        }
        return nullptr;
    };

    auto* 目标方法首节点 = 选择干净目标方法({
        枚举_本能动作ID::自我_确认收到,
        枚举_本能动作ID::自我_请求补充信息,
        枚举_本能动作ID::自我_读取最新人类输入,
        枚举_本能动作ID::自我_读取核心状态
        });
    auto* 目标方法信息 = (目标方法首节点 && 目标方法首节点->主信息)
        ? dynamic_cast<方法首节点主信息类*>(目标方法首节点->主信息)
        : nullptr;
    if (!目标方法首节点 || !目标方法信息) {
        out.错误摘要 = "回归测试失败：未找到可用于学习效果验证的干净目标方法";
        return out;
    }

    bool 三次观察均成功 = true;
    I64 首次稳定次数 = 0;
    I64 最终稳定次数 = 0;
    I64 最终分裂次数 = 0;
    I64 最终状态 = 自我线程模块_detail::方法尝试状态_未尝试;
    bool 最终有条件 = false;
    bool 最终有结果 = false;
    const std::string 动作键 = 取动作键(目标方法首节点);

    for (int 轮次 = 1; 轮次 <= 3; ++轮次) {
        bool 学习前有条件 = false;
        bool 学习前有结果 = false;
        const auto 学习前状态 = 线程.私有_判定方法尝试状态(目标方法首节点, &学习前有条件, &学习前有结果);

        结构_学习候选方法 候选{};
        候选.head = 目标方法首节点;
        候选.firstCondition = nullptr;
        候选.缺条件 = !学习前有条件;
        候选.缺结果 = !学习前有结果;
        候选.动作键 = 动作键;

        const auto 学习结果 = 线程.私有_尝试学习一步(结构体_时间戳::当前_微秒(), &候选);
        三次观察均成功 = 三次观察均成功 && 学习结果.观察成功;

        bool 学习后有条件 = false;
        bool 学习后有结果 = false;
        const auto 学习后状态 = 线程.私有_判定方法尝试状态(目标方法首节点, &学习后有条件, &学习后有结果);
        const I64 当前稳定次数 = 读取方法稳定次数(目标方法首节点);

        if (轮次 == 1) {
            out.首次学习有进展 = 学习结果.有进展;
            out.首次已补齐条件和结果 = 学习后有条件 && 学习后有结果;
            out.首次已进入尝试中 = 学习后状态 == 自我线程模块_detail::方法尝试状态_尝试中;
            首次稳定次数 = 当前稳定次数;
        }

        if (轮次 == 3) {
            最终稳定次数 = 当前稳定次数;
            最终分裂次数 = 读取方法分裂次数(目标方法首节点);
            最终状态 = 学习后状态;
            最终有条件 = 学习后有条件;
            最终有结果 = 学习后有结果;
        }

        日志::运行f(
            "[自我线程][回归] 学习效果轮次: 动作={}, 轮次={}, 学习前状态={}, 学习后状态={}, has_condition={}, has_result={}, observed={}, stable_count={}",
            动作键.empty() ? std::string("<unknown>") : 动作键,
            轮次,
            static_cast<long long>(学习前状态),
            static_cast<long long>(学习后状态),
            学习后有条件 ? 1 : 0,
            学习后有结果 ? 1 : 0,
            学习结果.观察成功 ? 1 : 0,
            static_cast<long long>(当前稳定次数));
    }

    out.三次观察均成功 = 三次观察均成功;
    out.连续稳定次数达标 = 首次稳定次数 >= 1 && 最终稳定次数 >= 3 && 最终分裂次数 == 0;
    out.已标记尝试完成 =
        最终有条件
        && 最终有结果
        && 最终状态 == 自我线程模块_detail::方法尝试状态_尝试完成;

    if (!out.首次学习有进展
        || !out.首次已补齐条件和结果
        || !out.首次已进入尝试中
        || !out.三次观察均成功
        || !out.连续稳定次数达标
        || !out.已标记尝试完成) {
        out.错误摘要 =
            "回归测试失败：尝试学习未沉淀出稳定方法效果"
            " (action=" + (动作键.empty() ? std::string("<unknown>") : 动作键) +
            ", first_progress=" + std::to_string(out.首次学习有进展 ? 1 : 0) +
            ", first_fill=" + std::to_string(out.首次已补齐条件和结果 ? 1 : 0) +
            ", first_trying=" + std::to_string(out.首次已进入尝试中 ? 1 : 0) +
            ", observed_all=" + std::to_string(out.三次观察均成功 ? 1 : 0) +
            ", stable_first=" + std::to_string(static_cast<long long>(首次稳定次数)) +
            ", stable_final=" + std::to_string(static_cast<long long>(最终稳定次数)) +
            ", split_final=" + std::to_string(static_cast<long long>(最终分裂次数)) +
            ", final_status=" + std::to_string(static_cast<long long>(最终状态)) + ")";
        return out;
    }

    out.成功 = true;
    return out;
}

inline 自我线程类::结构_叶子执行学习修复回归结果 自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复() {
    结构_叶子执行学习修复回归结果 out{};

    自我线程类 线程{};
    线程.自我初始化();

    const 时间戳 now = 结构体_时间戳::当前_微秒();
    auto* 默认场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    const auto 后缀 = std::to_string(static_cast<long long>(now));
    const auto* 无效动作名 = 语素集.添加词性词("回归_无效方法_" + 后缀, "动词");
    const auto* 需求类型 = 语素集.添加词性词("回归_叶子学习修复需求_" + 后缀, "名词");
    const auto* 任务名称 = 语素集.添加词性词("回归_叶子学习修复任务_" + 后缀, "名词");
    const auto* 步骤名称 = 语素集.添加词性词("回归_叶子学习修复步骤_" + 后缀, "名词");

    结构体_动作句柄 无效动作句柄{};
    auto* 无效方法首节点 = 方法集.注册方法首节点(
        无效动作名,
        无效动作句柄,
        枚举_方法沉淀来源::未定义,
        {},
        "自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复/创建无效方法");
    auto* 无效方法信息 = (无效方法首节点 && 无效方法首节点->主信息)
        ? dynamic_cast<方法首节点主信息类*>(无效方法首节点->主信息)
        : nullptr;
    out.已识别原方法不可执行 =
        无效方法信息
        && (无效方法信息->动作句柄.类型 != 枚举_动作句柄类型::本能函数ID
            || 无效方法信息->动作句柄.本能ID == 0);
    if (!无效方法首节点 || !out.已识别原方法不可执行) {
        out.错误摘要 = "回归测试失败：无法构造不可执行方法起点";
        return out;
    }

    需求类::结构_创建参数 需求参数{};
    需求参数.类型 = 需求类型;
    需求参数.权重 = 6800;
    需求参数.发生场景 = 默认场景;
    auto* 回归需求 = 需求集.创建需求(
        线程.自我指针,
        线程.自我指针,
        nullptr,
        需求参数,
        "自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复/创建需求",
        线程.服务需求节点_);
    if (!回归需求) {
        out.错误摘要 = "回归测试失败：无法创建叶子学习修复需求";
        return out;
    }

    任务类::结构_创建任务头参数 任务参数{};
    任务参数.名称 = 任务名称;
    任务参数.类型 = 需求类型;
    任务参数.初始场景 = 默认场景;
    任务参数.创建时间 = now;
    任务参数.基准优先级 = 6800;
    任务参数.局部优先级偏移 = 120;
    任务参数.任务树类型 = 枚举_任务树类型::叶子任务;
    任务参数.父任务头结点 = 线程.服务任务节点_;
    auto* 回归任务 = 任务类::确保需求头结点(
        回归需求,
        任务参数,
        线程.服务任务节点_,
        "自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复/创建任务");
    auto* 任务头信息 = (回归任务 && 回归任务->主信息)
        ? dynamic_cast<任务头结点信息*>(回归任务->主信息)
        : nullptr;
    if (!回归任务 || !任务头信息) {
        out.错误摘要 = "回归测试失败：无法创建叶子学习修复任务";
        return out;
    }

    任务类::结构_创建步骤参数 步骤参数{};
    步骤参数.名称 = 步骤名称;
    步骤参数.步骤场景 = 默认场景;
    步骤参数.创建时间 = now;
    步骤参数.初始状态 = 枚举_任务状态::就绪;
    步骤参数.允许切换方法 = true;
    步骤参数.当前选中方法首节点 = 无效方法首节点;
    步骤参数.可用方法首节点列表 = { 无效方法首节点 };
    auto* 回归步骤 = 任务类::创建步骤节点(
        回归任务,
        步骤参数,
        "自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复/创建步骤");
    auto* 步骤信息 = (回归步骤 && 回归步骤->主信息)
        ? dynamic_cast<任务步骤节点信息*>(回归步骤->主信息)
        : nullptr;
    if (!回归步骤 || !步骤信息) {
        out.错误摘要 = "回归测试失败：无法创建叶子学习修复步骤";
        return out;
    }

    (void)任务类::设置任务当前步骤(
        回归任务->主信息,
        回归步骤,
        now,
        "自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复/设置当前步骤");
    任务类::设置任务状态(
        回归步骤,
        枚举_任务状态::就绪,
        "自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复/步骤就绪");
    任务类::设置任务状态(
        回归任务,
        枚举_任务状态::就绪,
        "自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复/任务就绪");

    const auto 执行结果 = 线程.私有_执行任务叶子步骤(回归步骤);
    auto* 修复后方法首节点 = 步骤信息->当前选中方法首节点;
    auto* 修复后方法信息 = (修复后方法首节点 && 修复后方法首节点->主信息)
        ? dynamic_cast<方法首节点主信息类*>(修复后方法首节点->主信息)
        : nullptr;
    const bool 修复后方法可执行 =
        修复后方法信息
        && 修复后方法信息->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID
        && 修复后方法信息->动作句柄.本能ID != 0;

    out.已切换到新方法 =
        修复后方法首节点 != nullptr
        && 修复后方法首节点 != 无效方法首节点;
    out.已通过学习兜底补出可执行方法 =
        out.已切换到新方法
        && 修复后方法可执行;
    out.叶子执行成功 =
        执行结果.成功
        && 执行结果.建议动作 == 枚举_任务反馈动作::继续下一步;

    if (!out.已通过学习兜底补出可执行方法 || !out.叶子执行成功) {
        out.错误摘要 =
            "回归测试失败：叶子执行未能通过学习兜底修复无效方法"
            " (selected=" + std::to_string(reinterpret_cast<std::uintptr_t>(修复后方法首节点)) +
            ", invalid=" + std::to_string(reinterpret_cast<std::uintptr_t>(无效方法首节点)) +
            ", switched=" + std::to_string(out.已切换到新方法 ? 1 : 0) +
            ", executable=" + std::to_string(修复后方法可执行 ? 1 : 0) +
            ", success=" + std::to_string(执行结果.成功 ? 1 : 0) +
            ", advice=" + std::to_string(static_cast<int>(执行结果.建议动作)) +
            ", list_size=" + std::to_string(static_cast<unsigned long long>(步骤信息->可用方法首节点列表.size())) +
            ", summary=" + 执行结果.摘要 + ")";
        return out;
    }

    out.成功 = true;
    return out;
}


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
        自我线程模块_detail::结构_任务调度动作事件守卫 派发动作(
            info,
            自我线程模块_detail::动作_学习线程派发_(),
            结构体_时间戳::当前_微秒(),
            "自我线程类::学习线程入口");
        任务类::轻量设置任务状态(item.节点, 枚举_任务状态::执行中, "自我线程类::学习线程入口/置执行中");
        ++正在执行线程数_;
        日志::运行f("[自我线程] 学习线程取到任务: 节点={}, 是否学习常驻任务={}", (void*)item.节点, item.节点 == 学习任务节点_ ? 1 : 0);

        const 时间戳 now = 结构体_时间戳::当前_微秒();
        if (item.节点 == 学习任务节点_) {
            const auto before = 待学习方法数量_;
            bool progressed = false;

            if (待学习方法数量_ <= 0
                && !私有_存在等待学习任务()
                && 学习工作流状态_.当前阶段 == 枚举_学习任务阶段::空闲) {
                私有_重置学习工作流("学习线程/无待学习方法", now);
            }
            else {
                switch (学习工作流状态_.当前阶段) {
                case 枚举_学习任务阶段::空闲:
                case 枚举_学习任务阶段::采样:
                    progressed = 私有_推进学习采样阶段(now);
                    break;
                case 枚举_学习任务阶段::更新: {
                    私有_切换学习任务阶段(枚举_学习任务阶段::更新, now, "学习线程/更新开始");
                    日志::运行f(
                        "[自我线程] 学习线程开始更新阶段: 节点={}, 方法={}, 动作={}, 待学习方法数量={}",
                        (void*)item.节点,
                        (void*)学习工作流状态_.当前候选.head,
                        学习工作流状态_.当前候选.动作键.empty() ? std::string("<unknown>") : 学习工作流状态_.当前候选.动作键,
                        before);
                    const auto 学习结果 = 私有_尝试学习一步(now, &学习工作流状态_.当前候选);
                    学习工作流状态_.上次有进展 = 学习结果.有进展;
                    学习工作流状态_.上次观察成功 = 学习结果.观察成功;
                    学习工作流状态_.需要唤醒等待任务 = 学习结果.需要唤醒等待任务;
                    学习工作流状态_.候选数量 = 学习结果.候选数量;
                    学习工作流状态_.更新数量 = 学习结果.更新数量;
                    学习工作流状态_.新建数量 = 学习结果.新建数量;
                    学习工作流状态_.最近结果场景 = 学习结果.结果场景;
                    私有_结束学习当前阶段(学习结果.有进展, now, "学习线程/更新结束");
                    私有_切换学习任务阶段(枚举_学习任务阶段::验证, now, "学习线程/更新后进入验证");
                    progressed = 学习结果.有进展;
                    break;
                }
                case 枚举_学习任务阶段::验证:
                    progressed = 私有_推进学习验证阶段(now);
                    break;
                case 枚举_学习任务阶段::提交:
                    私有_推进学习提交阶段(now);
                    progressed = true;
                    break;
                case 枚举_学习任务阶段::回滚:
                    私有_推进学习回滚阶段(now);
                    progressed = false;
                    break;
                default:
                    私有_重置学习工作流("学习线程/未知阶段", now);
                    break;
                }
            }

            const auto after = 待学习方法数量_;

            info->调度优先级 = (after > 0) ? 0 : (std::numeric_limits<std::int64_t>::min)();
            info->创建时间 = now;
            任务类::设置任务状态(item.节点, 枚举_任务状态::就绪, "自我线程类::学习线程入口/学习任务回到就绪");

            日志::运行f("[自我线程] 学习任务阶段推进一次: 阶段={}, 节点={}, 有进展={}, 待学习方法数量 {} -> {}, 优先级={}",
                私有_学习阶段标签(学习工作流状态_.当前阶段),
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































