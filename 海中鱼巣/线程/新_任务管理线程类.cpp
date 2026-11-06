#include "新_任务管理线程类.h"
#include "../领域/新_本能函数集.h"
#include "../领域/新_方法类.h"
#include "../业务/新_世界树类.h"
#include "../核心/日志系统.h"
#include <sstream>
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

namespace 海中鱼巣 {

namespace {
// 诊断责任：向上送出。仅本次因果读取/接纳的工程控制，不改业务状态。
void 检查因果提交停止(std::stop_token 停止) {
    if (停止.stop_requested()) throw 新任务因果读取取消{};
}

// 诊断责任：向上送出。只获取manager原互斥，局部停车不持共享等待仓。
std::unique_lock<std::mutex> 获取因果提交锁(
    std::mutex& 互斥, std::stop_token 停止) {
    检查因果提交停止(停止);
    std::unique_lock<std::mutex> 锁(互斥, std::defer_lock);
    if (!停止.stop_possible()) {
        锁.lock();
        return 锁;
    }
    std::mutex 等待互斥;
    std::condition_variable_any 等待;
    while (!锁.try_lock()) {
        检查因果提交停止(停止);
        std::unique_lock 等待锁(等待互斥);
        (void)等待.wait_for(等待锁, 停止, std::chrono::milliseconds(10), [] { return false; });
        检查因果提交停止(停止);
    }
    检查因果提交停止(停止);
    return 锁;
}

#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
const wchar_t* 因果承接诊断状态名(新任务治理承接状态 状态) noexcept {
    switch (状态) {
#define HY_CAUSAL_ADOPT_STATE(名称) case 新任务治理承接状态::名称: return HY_EGO_WIDEN(#名称);
    HY_CAUSAL_ADOPT_STATE(已读回) HY_CAUSAL_ADOPT_STATE(已重新启用)
    HY_CAUSAL_ADOPT_STATE(既有终结任务) HY_CAUSAL_ADOPT_STATE(需求当前已满足)
    HY_CAUSAL_ADOPT_STATE(尚未初始化) HY_CAUSAL_ADOPT_STATE(需求不存在)
    HY_CAUSAL_ADOPT_STATE(任务操作未完成) HY_CAUSAL_ADOPT_STATE(读回不一致)
    HY_CAUSAL_ADOPT_STATE(资源失败)
#undef HY_CAUSAL_ADOPT_STATE
    }
    return L"未知承接状态";
}
void 记录管理因果阶段(const wchar_t* 阶段, 稳定编码 父任务,
    稳定编码 子需求 = {}, const wchar_t* 状态 = nullptr,
    std::optional<稳定编码> 子任务 = {}) noexcept {
    try {
        static std::atomic<std::uint64_t> 条数{0};
        const auto 序号 = 条数.fetch_add(1, std::memory_order_relaxed);
        if (序号 > 256) return;
        if (序号 == 256) {
            (void)记录调试日志(L"MAINLINE_PHASE", L"管理因果等待", L"日志截断",
                L"因果等待阶段投影已达256条，仅停止诊断，不裁剪真实任务。");
            return;
        }
        std::wostringstream 文本;
        文本 << L"父任务引用=" << 父任务.值;
        if (子需求.值) 文本 << L"；子需求引用=" << 子需求.值;
        if (状态) 文本 << L"；状态=" << 状态;
        if (子任务) 文本 << L"；子任务引用=" << 子任务->值;
        (void)记录调试日志(L"MAINLINE_PHASE", L"管理因果等待", 阶段, 文本.str());
    } catch (...) {}
}
void 记录管理停止阶段(const wchar_t* 阶段) noexcept {
    try {
        (void)记录调试日志(L"MAINLINE_PHASE", L"任务管理停止", 阶段,
            L"仅观察原工作池停止和回执收束，不跳过或取消真实结果。");
    } catch (...) {}
}
#endif
bool 是定义前提变化完整回执(const 新任务工作处理结果& 返回,const 新任务工作请求& 请求) {
    return 返回.原请求==请求&&请求.种类==新任务工作种类::检查条件
        &&返回.状态==新任务工作处理状态::执行组织定义前提已变化
        &&返回.执行组织交付&&返回.执行组织交付->状态==新任务执行组织状态::定义前提已变化
        &&!返回.执行组织交付->组织&&!返回.条件筹办&&!返回.条件子需求提议
        &&!返回.方法召回&&!返回.本能执行&&!返回.本能结果获取&&!返回.调度材料
        &&!返回.本能结果检测&&!返回.最终消费&&!返回.贡献治理;
}
// worker回执和self直调共用同一正式结果核验；返回值只表示已完成入账及收尾。
bool 核验最终消费正式结果(const 新服务值最终来源定位& 来源,
    const 新服务值最终消费结果& 结果, const 新服务值最终消费结果& 正式) {
    if (结果.来源 != 来源 || 正式.来源 != 来源)
        throw std::logic_error("最终消费独立读回来源不一致");
    if (结果.状态 == 新服务值最终消费状态::资源延后
        || 结果.状态 == 新服务值最终消费状态::已准备待续) {
        if (正式.状态 != 新服务值最终消费状态::已完成) return false;
        if (!正式.消费节点 || !正式.收尾阶段节点)
            throw std::logic_error("资源延后后正式完成缺少U/J5");
        return true;
    }
    const bool 完成 = 结果.状态 == 新服务值最终消费状态::已完成
        || 结果.状态 == 新服务值最终消费状态::精确重复;
    if (完成) {
        if (!结果.消费节点 || !结果.收尾阶段节点)
            throw std::logic_error("完成回执缺少U/J5");
        if (正式.状态 == 新服务值最终消费状态::资源延后) return false;
        if (正式.状态 != 新服务值最终消费状态::已完成
            || 正式.消费节点 != 结果.消费节点 || 正式.收尾阶段节点 != 结果.收尾阶段节点
            || 正式.普通动作节点 != 结果.普通动作节点)
            throw std::logic_error("最终消费U/J5独立读回不一致");
    } else if (结果.消费节点 || 结果.普通动作节点 || 结果.收尾阶段节点
        || (结果.状态 != 新服务值最终消费状态::来源条件不具备
            && 结果.状态 != 新服务值最终消费状态::来源已变化
            && 结果.状态 != 新服务值最终消费状态::等待配置
            && 结果.状态 != 新服务值最终消费状态::等待同源工作)) {
        throw std::logic_error("未消费结果不是合法零动作等待");
    }
    return 完成;
}

bool 学习筹办安全状态(新任务状态 状态) noexcept {
    return 状态 == 新任务状态::已建立 || 状态 == 新任务状态::待查找方法
        || 状态 == 新任务状态::待检查条件 || 状态 == 新任务状态::等待条件;
}

bool 条件筹办为业务等待(const 新任务治理条件筹办结果& 结果) noexcept {
    if (结果.状态 != 新任务治理条件筹办状态::已完成
        || 结果.候选已完整核验 || !结果.方法选择状态) return false;
    switch (*结果.方法选择状态) {
    case 新任务方法选择状态::等待条件:
    case 新任务方法选择状态::匹配不唯一:
    case 新任务方法选择状态::目标已满足:
    case 新任务方法选择状态::无适用结果:
    case 新任务方法选择状态::幅度材料不足:
    case 新任务方法选择状态::结果定义暂不支持:
        return true;
    default: return false;
    }
}

bool 条件筹办有结构错误(const 新任务治理条件筹办结果& 结果) noexcept {
    return 结果.状态 == 新任务治理条件筹办状态::结构不一致
        || std::any_of(结果.方法筹办组.begin(), 结果.方法筹办组.end(), [](const auto& 方法) {
            return 方法.状态 == 新任务候选方法筹办状态::结构缺口
                || std::any_of(方法.条件结果组.begin(), 方法.条件结果组.end(), [](const auto& 配对) {
                    return 配对.状态 == 新任务方法条件项核验状态::结构缺口;
                });
        });
}

// 比较选定方法的声明，不把其它参数补齐当作整个来源失效。
bool 条件提议来源一致(const 新任务条件子需求提议& 当前,
    const 新任务条件子需求提议& 原提议) noexcept {
    if(!当前.分解依据||!原提议.分解依据)return false;
    const auto& 今=*当前.分解依据;
    const auto& 原=*原提议.分解依据;
    return (当前.状态 == 新任务条件子需求提议状态::已形成
            || 当前.状态 == 新任务条件子需求提议状态::无需子需求
            || 当前.状态 == 新任务条件子需求提议状态::材料未到)
        && 当前.任务节点 == 原提议.任务节点
        && 当前.父需求节点 == 原提议.父需求节点
        && 当前.父需求目标存在节点 == 原提议.父需求目标存在节点
        && 当前.父需求目标 == 原提议.父需求目标
        && 当前.方法节点 == 原提议.方法节点
        && 当前.配对节点 == 原提议.配对节点
        && 今.来源方法节点==原.来源方法节点&&今.来源配对节点==原.来源配对节点
        && 今.来源条件节点==原.来源条件节点&&今.来源结果节点==原.来源结果节点
        && 今.参数要求组==原.参数要求组&&今.原始条件==原.原始条件
        && 今.结果==原.结果&&今.线性关系组==原.线性关系组
        && 今.执行分支节点==原.执行分支节点;
}

bool 条件提议等材料(const 新任务条件子需求提议& 当前) noexcept {
    return 当前.状态==新任务条件子需求提议状态::材料未到
        ||当前.状态==新任务条件子需求提议状态::需求信息待完善;
}

enum class 条件子项当前支持 : std::uint8_t { 仍支持,等材料,已变化 };

条件子项当前支持 读取条件子项当前支持(const 新任务条件子需求提议& 当前,
    const 新任务条件子需求提议& 原提议,const 新任务条件子需求项& 项) noexcept {
    using S=条件子项当前支持;
    if(条件提议等材料(当前)||!当前.分解依据)return S::等材料;
    if(std::find(当前.子需求项组.begin(),当前.子需求项组.end(),项)!=当前.子需求项组.end())return S::仍支持;
    const auto& 未解析=当前.分解依据->未解析条件序号组;
    const auto 未取得=[&](std::uint64_t 序号){
        return std::find(未解析.begin(),未解析.end(),序号)!=未解析.end();
    };
    if(!项.集合目标) {
        if(std::find(当前.已知条件项组.begin(),当前.已知条件项组.end(),项)!=当前.已知条件项组.end())return S::仍支持;
        return 未取得(项.条件项序号) ? S::等材料 : S::已变化;
    }
    const auto 同槽=[&](const auto& x){return x.目标存在节点==项.目标存在节点
        &&x.目标特征类型概念节点==项.目标特征类型概念节点;};
    if(std::any_of(原提议.已知条件项组.begin(),原提议.已知条件项组.end(),[&](const auto& x){
        return 同槽(x)&&未取得(x.条件项序号);
    }))return S::等材料;
    const auto 原数量=std::count_if(原提议.已知条件项组.begin(),原提议.已知条件项组.end(),同槽);
    if(!原数量||原数量!=std::count_if(当前.已知条件项组.begin(),当前.已知条件项组.end(),同槽))return S::已变化;
    return std::all_of(原提议.已知条件项组.begin(),原提议.已知条件项组.end(),[&](const auto& x){
        return !同槽(x)||std::count(原提议.已知条件项组.begin(),原提议.已知条件项组.end(),x)
            ==std::count(当前.已知条件项组.begin(),当前.已知条件项组.end(),x);
    }) ? S::仍支持 : S::已变化;
}

bool 扫描承接可资源重试(const 新任务承接处理结果& 结果) noexcept {
    if (结果.原消息.依据 != 新任务承接依据::自我全树扫描 || !结果.承接)
        return false;
    const auto& 承接 = *结果.承接;
    return 承接.状态 == 新任务治理承接状态::资源失败
        || (承接.状态 == 新任务治理承接状态::任务操作未完成
            && 承接.建立结果
            && 承接.建立结果->状态 == 新任务操作状态::资源失败);
}

void 记录需求任务承接观察(const 新任务承接处理结果& 结果) noexcept {
    try {
        if (!结果.承接) return;
        const auto& 承接 = *结果.承接;
        const auto* 建立 = 承接.建立结果 ? &*承接.建立结果 : nullptr;
        const bool 同步未完成 = 建立
            && 建立->状态 == 新任务操作状态::服务任务集合同步失败;
        const bool 结构异常 = 建立
            && (建立->状态 == 新任务操作状态::结构不一致
                || 建立->状态 == 新任务操作状态::需求已有多个任务);
        const bool 已建立 = 建立 && 建立->状态 == 新任务操作状态::已建立;
        const bool 资源未完成 = 承接.状态 == 新任务治理承接状态::资源失败
            || (建立 && 建立->状态 == 新任务操作状态::资源失败);
        if (!同步未完成 && !结构异常 && !已建立 && !资源未完成) return;
        std::wostringstream 文本;
        文本 << (同步未完成 ? L"任务集合未完成同步；返回值未区分来源读取、回调或写后失败"
            : 结构异常 ? L"需求任务结构异常"
            : 已建立 ? L"任务建立返回及承接摘要" : L"本次承接因资源问题未完成")
            << L"；消息=" << 结果.原消息.消息编号
            << L"；需求=" << 结果.原消息.需求节点.值
            << L"；消息依据=" << static_cast<int>(结果.原消息.依据)
            << L"；处理状态=" << static_cast<int>(结果.状态)
            << L"；承接状态=" << static_cast<int>(承接.状态)
            << L"；写入已尝试=" << 承接.写入已尝试 << L"；建立状态=";
        if (建立) 文本 << static_cast<int>(建立->状态); else 文本 << L"无";
        文本 << L"；任务=";
        if (建立 && 建立->任务节点) 文本 << 建立->任务节点->值; else 文本 << L"无";
        文本 << L"；任务虚拟存在=";
        if (建立 && 建立->任务虚拟存在节点) 文本 << 建立->任务虚拟存在节点->值;
        else 文本 << L"无";
        if (扫描承接可资源重试(结果))
            文本 << L"；扫描承接保留原消息及结果通道，延后重新读取并建立或复用任务";
        if (结构异常) {
            (void)记录逻辑错误日志(L"任务管理线程", L"需求任务承接", 文本.str());
        } else {
            (void)记录运行日志(L"任务管理线程", L"需求任务承接", 文本.str());
        }
    } catch (...) {
        // 日志不可用不改变已经形成的结构化结果。
    }
}
} // namespace

bool 新长期服务值结算配置::完整() const noexcept {
    return 世界树服务
        && 有效(自我所在场景)
        && 有效(自我存在)
        && 有效(服务值特征)
        && 有效(完成服务需求) && 有效(服务根需求)
        && 有效(自我概念节点) && 有效(服务值特征类型节点)
        && 能力.类别 == 新结算能力类别::自我服务值治理
        && 有效(能力.节点) && 工程诊断端口;
}

bool 新任务筹办反馈定位::完整() const noexcept {
    return 有效(任务节点)
        && 原工作.工作编号 != 0
        && 原工作.任务节点 == 任务节点
        && (原工作.种类 == 新任务工作种类::查找方法
            || 原工作.种类 == 新任务工作种类::检查条件);
}

bool 新任务学习复核定位::完整() const noexcept {
    return 复核编号 && 学习交接编号 && 有效(方法节点)
        && 有效(任务节点) && 有效(需求节点)
        && (!候选材料节点 || 有效(*候选材料节点))
        && (!方法实例节点 || 有效(*方法实例节点))
        && (!方法执行组织节点 || 有效(*方法执行组织节点))
        && 学习筹办安全状态(任务状态);
}

bool 新任务等待复核定位::完整() const noexcept {
    return 复核编号 && 定位.完整()
        && 原因 > 新任务等待原因::无 && 原因 < 新任务等待原因::需核查;
}

bool 新任务结果反馈定位::完整() const noexcept {
    if (!有效(任务节点) || !有效(执行实例节点) || !有效(影响评估节点)
        || (已发布需求节点 && !有效(*已发布需求节点))
        || !原工作.工作编号 || 原工作.任务节点 != 任务节点) return false;
    if (原工作.种类 == 新任务工作种类::检测本能结果)
        return 原工作.本能结果检测请求
            && 原工作.本能结果检测请求->任务节点 == 任务节点
            && 原工作.本能结果检测请求->执行实例节点 == 执行实例节点
            && !原工作.本能执行请求 && !原工作.本能结果获取请求;
    return 原工作.种类 == 新任务工作种类::获取本能结果
        && 原工作.本能结果获取请求
        && 原工作.本能结果获取请求->任务节点 == 任务节点
        && 原工作.本能结果获取请求->执行实例节点 == 执行实例节点
        && !原工作.本能执行请求 && !原工作.本能结果检测请求;
}

namespace {
std::mutex 管理所有者互斥;
const 新_任务管理线程类* 管理所有者 = nullptr;

std::chrono::milliseconds 等待时长(std::uint64_t 毫秒) noexcept {
    const auto 最大 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::duration::max() / 4).count();
    return std::chrono::milliseconds((std::min)(毫秒, static_cast<std::uint64_t>(最大)));
}

enum class 筹办派发状态 : std::uint8_t {
    已派发 = 1, 无可派发任务, 资源暂不可用, 已停止, 内部错误
};
}

struct 新_任务管理线程类::实现 final {
    explicit 实现(新_任务类& 任务) noexcept
        : 任务服务(&任务),
          工作线程(new (std::nothrow) 新_任务工作线程类(
              任务, [this] {
                  try {
                      std::lock_guard 锁(互斥);
                      变化.notify_all();
                  } catch (...) {}
              })) {}
    新_任务类* 任务服务;
    const 新_方法类* 学习方法读取 = nullptr;
    struct 学习任务项 final {
        稳定编码 任务节点;
        std::optional<std::uint64_t> 复核编号;
        bool 已终结 = false;
    };
    struct 学习事件 final {
        新学习结束通知 通知;
        bool 已展开 = false, 已终结 = false;
        std::vector<学习任务项> 任务组;
        std::chrono::steady_clock::time_point 可处理时点{};
    };
    struct 学习复核记录 final {
        新任务学习复核定位 定位;
        std::optional<新自我学习复核决议> 决议;
        bool 已反馈 = false, 已终结 = false, 准备已尝试 = false;
    };
    // 已终结项保留到实例销毁，仅用于同一进程交接去重，不是业务账。
    std::map<std::uint64_t, 学习事件> 学习事件组;
    std::set<稳定编码> 待退出适用方法;
    std::optional<std::uint64_t> 当前学习交接;
    std::map<std::uint64_t, 学习复核记录> 学习复核组;
    std::set<std::uint64_t> 学习复核占用;
    std::map<std::uint64_t, std::uint64_t> 学习失效筹办工作;
    std::uint64_t 下一学习复核编号 = 1;
    std::optional<新学习结束通知> 最近方法学习结束;
    std::optional<新学习运行异常记录> 最近学习运行异常;
    mutable std::mutex 互斥;
    std::mutex 加入互斥;
    std::condition_variable 变化;
    std::thread 线程;
    新任务管理运行配置 配置;
    新任务管理生命周期 生命周期 = 新任务管理生命周期::未启动;
    bool 创建已尝试 = false, 已进入 = false, 停止 = false, 已完成 = false, 已加入 = false;
    std::stop_source 停止源;
    bool 内部错误发生 = false;
    struct 消息项 final {
        新任务承接消息 消息;
        std::optional<新任务筹办反馈定位> 父筹办定位;
        std::promise<新任务承接处理结果> 承诺;
        std::shared_future<新任务承接处理结果> 结果 = 承诺.get_future().share();
        std::chrono::steady_clock::time_point 可处理时点;
        bool 扫描资源失败已记录 = false;
    };
    std::vector<std::shared_ptr<消息项>> 活动;
    std::size_t 普通消息数量已加锁() const noexcept {
        return 活动.size();
    }
    struct 已完成自动决议 final {
        新任务承接消息 消息;
        std::shared_future<新任务承接处理结果> 结果;
    };
    // 普通自动治理分别处理安全根和服务根；保留固定小容量槽位，仅保存最近
    // 已完成自动决议的共享结果，不建立历史消息账。
    std::array<std::optional<已完成自动决议>, 2> 最近自动决议;
    std::unique_ptr<新_任务工作线程类> 工作线程;
    struct 工作占用 final {
        新任务工作请求 请求;
        std::shared_future<新任务工作处理结果> 结果;
        std::optional<新任务工作处理结果> 前置已读回;
    };
    static_assert(std::is_nothrow_move_constructible_v<新任务工作请求>);
    static_assert(std::is_nothrow_move_constructible_v<工作占用>);
    static_assert(std::is_nothrow_copy_assignable_v<std::shared_future<新任务工作处理结果>>);
    std::vector<工作占用> 活动工作;
    enum class 最终工作领取 : std::uint8_t { 自我待取, 自我确认中, 停止收束中, 已确认 };
    struct 最终工作记录 final {
        工作占用 工作;
        const void* 自我所有者 = nullptr;
        最终工作领取 领取 = 最终工作领取::自我待取;
        bool 提交中 = false, 已通知 = false;
        bool 工作池已确认 = false;
        std::optional<新任务完成结算确认状态> 贡献确认;
        std::optional<新服务值最终消费结果> 最终交付结果;
    };
    // 当前K只指向一个在途请求；已确认编号保留原future，只用于技术交接幂等。
    std::map<std::uint64_t, 最终工作记录> 最终工作组;
    std::map<稳定编码, std::uint64_t> 当前最终工作;
    using 贡献工作键 = std::array<std::uint64_t, 3>;
    std::map<贡献工作键, std::uint64_t> 当前贡献工作;
    std::set<std::uint64_t> 普通工作提交中;
    static 贡献工作键 贡献键(const 新任务贡献反馈定位& 定位) noexcept {
        return {定位.任务节点.值, 定位.执行实例节点.值, 定位.影响评估节点.值};
    }
    bool 贡献占用任务已加锁(std::uint64_t 任务值) const noexcept {
        return std::any_of(当前贡献工作.begin(), 当前贡献工作.end(),
            [&](const auto& 项) { return 项.first[0] == 任务值; });
    }
    // 同T技术占用覆盖pool ACK之后、贡献确认之前的窗口。
    // 该helper只从无manager锁的派发路径进入。
    新任务工作接收结果 提交普通工作(const 新任务工作请求& 请求) {
        工作占用 待安装;
        try {
            std::lock_guard 锁(互斥);
            if (停止 || 已完成) return {新任务工作接收状态::已停止接收, {}};
            for(const auto& 项:活动工作)if(项.请求.工作编号==请求.工作编号) {
                if(项.请求!=请求)return {新任务工作接收状态::身份冲突,{}};
                if(!项.结果.valid())throw std::logic_error("accepted ordinary work future missing");
                return {新任务工作接收状态::精确重复,项.结果};
            }
            if(std::any_of(活动工作.begin(),活动工作.end(),[&](const auto& 项){return 项.请求.任务节点==请求.任务节点;})
                ||贡献占用任务已加锁(请求.任务节点.值)||普通工作提交中.contains(请求.任务节点.值)
                ||std::any_of(最终工作组.begin(),最终工作组.end(),[&](const auto& 项){
                    return 项.second.提交中&&项.second.工作.请求.任务节点==请求.任务节点;
                }))return {新任务工作接收状态::任务已占用,{}};
            if(请求.种类==新任务工作种类::筹办前置语言输入
                &&(任务有执行保护已加锁(请求.任务节点.值)||待反馈任务.contains(请求.任务节点.值)
                    ||学习复核占用.contains(请求.任务节点.值)||等待复核占用.contains(请求.任务节点.值)
                    ||有服务结算来源待处理已加锁(请求.任务节点.值)))
                return {新任务工作接收状态::任务已占用,{}};
            const auto 特殊提交数=static_cast<std::size_t>(std::count_if(最终工作组.begin(),最终工作组.end(),
                [](const auto& 项){return 项.second.提交中;}));
            const auto 占用数=活动工作.size()+普通工作提交中.size()+特殊提交数;
            if(占用数>=配置.邮箱容量)return {新任务工作接收状态::邮箱已满,{}};
            // 所有请求内分配及全部在途安装槽均在投池前完成。
            待安装=工作占用{请求,{},{}};
            活动工作.reserve(占用数+1);
            if (请求.种类 == 新任务工作种类::查找方法 || 请求.种类 == 新任务工作种类::检查条件) {
                const auto 原 = 筹办知识基线.find(请求.任务节点.值);
                if (原 == 筹办知识基线.end() || 原->second.原工作 != 请求) {
                    const auto 材料 = 调度材料.find(请求.任务节点.值);
                    if (材料 == 调度材料.end() || !材料->second.需求)
                        throw std::logic_error("planning knowledge baseline missing before dispatch");
                    筹办知识基线.insert_or_assign(请求.任务节点.值, 筹办知识记录{请求, 材料->second});
                }
            }
            if(!普通工作提交中.insert(请求.任务节点.值).second)
                throw std::logic_error("ordinary submission reservation changed under manager lock");
        }catch(const std::bad_alloc&){return {新任务工作接收状态::资源失败,{}};}
        catch(const std::length_error&){return {新任务工作接收状态::资源失败,{}};}
        const auto 接收 = 工作线程->提交工作(请求);
        try {
            std::lock_guard 锁(互斥);
            if (接收.状态 == 新任务工作接收状态::已接收 || 接收.状态 == 新任务工作接收状态::精确重复) {
                if (!接收.处理结果.valid()) throw std::logic_error("普通工作已接收但future缺失");
                待安装.结果=接收.处理结果;
                活动工作.push_back(std::move(待安装));
            }
            // 先安装活动项再释放提交占用，避免self确认窗口漏看已入池工作。
            普通工作提交中.erase(请求.任务节点.值);
            变化.notify_all();
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"普通工作交接", L"池返回后无法保存原future");
        }
        return 接收;
    }
    新服务值结算确认接口* 最终消费端口 = nullptr;
    std::set<std::uint64_t> 阻塞任务;
    // 只承载可以被新的筹办输入替代的W1/W2来源；其它保护仍在阻塞任务。
    std::set<std::uint64_t> 筹办阻塞任务;
    // W0是完整只读快照，不授予执行许可；接受新工作后移出本轮就绪集合。
    std::map<std::uint64_t, 新任务调度材料项> 调度材料;
    bool 已有完整调度材料 = false, 调度待触发 = true;
    std::optional<新任务工作请求> 待派调度工作, 待派筹办工作;
    std::set<std::uint64_t> 零动作失效调度工作;
    std::chrono::steady_clock::time_point 下一调度时点{}, 最早事件调度时点{};
    struct 等待来源记录 final {
        新任务信息 任务;
        新任务等待原因 原因 = 新任务等待原因::未分类;
        std::optional<新任务工作请求> 原工作;
    };
    std::map<std::uint64_t, 等待来源记录> 等待来源;
    std::set<std::uint64_t> 等待目标复核;
    std::map<std::uint64_t, 新任务工作请求> 筹办延后重入;
    struct 等待复核记录 final {
        新任务等待复核定位 定位;
        std::optional<新自我等待复核决议> 决议;
        bool 已反馈 = false, 已终结 = false, 准备已尝试 = false;
    };
    std::map<std::uint64_t, 等待复核记录> 等待复核组;
    std::set<std::uint64_t> 等待复核占用;
    std::map<std::uint64_t, 新任务调度材料项> 上次等待材料;
    struct 筹办知识记录 final { 新任务工作请求 原工作; 新任务调度材料项 材料; };
    std::map<std::uint64_t, 筹办知识记录> 筹办知识基线;
    std::uint64_t 下一等待复核编号 = 1;
    std::chrono::steady_clock::time_point 下一条件复核时点{};
    std::chrono::steady_clock::time_point 下一工作消费时点{};
    std::optional<新任务调度读取状态> 最近调度读取状态;
    std::vector<稳定编码> 最近调度异常任务;
    std::map<std::uint64_t, 新任务筹办处理结果> 最近筹办结果;
    std::set<std::uint64_t> 待反馈任务;
    struct 自我筹办裁决记录 final {
        新自我筹办裁决 裁决;
        bool 已完成接收 = false;
        bool 执行工作已派发 = false;
        std::optional<新任务工作请求> 执行工作;
        std::vector<新任务已确认条件子需求> 已确认子需求组;
        // 与成功D一一对应；收到回执即预分配，尚未送达也占等待位置。
        std::vector<std::shared_ptr<消息项>> 子需求承接组;
        std::vector<bool> 子需求承接完成组;
        std::vector<bool> 条件准备依据已交付组;
        std::set<std::uint64_t> 来源变化子需求;
        bool 条件复核待触发 = false;
        bool 条件交接失败 = false;
        bool 条件来源待重筹办 = false;
    };
    std::map<std::uint64_t, 自我筹办裁决记录> 最近自我筹办裁决;
    struct 条件准备待交付项 final {
        新任务筹办反馈定位 原定位;
        新任务条件子需求提议 原提议;
        新任务已确认条件子需求 已确认项;
        friend bool operator==(const 条件准备待交付项&, const 条件准备待交付项&) = default;
    };
    using 条件准备待交付指针 = std::shared_ptr<const 条件准备待交付项>;
    std::map<std::uint64_t, 条件准备待交付指针> 待交付条件准备组;
    struct 因果子需求承接进度 final {
        std::shared_ptr<const 新任务治理承接结果> 最近本次回执;
        std::shared_ptr<const 新任务治理承接结果> 写入尝试依据回执;
        std::optional<std::pair<稳定编码,稳定编码>> 已证任务与虚拟存在;
        std::shared_ptr<const 新任务治理承接结果> 身份依据回执;
        std::shared_ptr<const 新任务治理承接结果> 任务副本依据回执;
        std::shared_ptr<const 新任务治理承接结果> 虚拟存在副本依据回执;
        std::shared_ptr<const 新任务治理承接结果> 激活尝试依据回执;
        std::shared_ptr<const 新任务治理承接结果> 激活生效依据回执;
    };
    using 因果承接回执指针=std::shared_ptr<const 新任务治理承接结果>;
    using 因果承接身份=std::optional<std::pair<稳定编码,稳定编码>>;
    using 因果重筹办前缀=std::optional<std::pair<新任务调度定位,新任务操作状态>>;
    static_assert(std::is_nothrow_move_constructible_v<新任务治理承接结果>
        &&std::is_nothrow_move_assignable_v<新任务治理承接结果>
        &&std::is_nothrow_destructible_v<新任务治理承接结果>);
    static_assert(std::is_nothrow_move_constructible_v<因果承接回执指针>
        &&std::is_nothrow_move_assignable_v<因果承接回执指针>
        &&std::is_nothrow_copy_constructible_v<因果承接回执指针>
        &&std::is_nothrow_copy_assignable_v<因果承接回执指针>
        &&std::is_nothrow_constructible_v<因果承接回执指针,std::shared_ptr<新任务治理承接结果>&&>);
    static_assert(std::is_nothrow_move_constructible_v<因果承接身份>
        &&std::is_nothrow_move_assignable_v<因果承接身份>
        &&std::is_nothrow_copy_constructible_v<因果承接身份>
        &&std::is_nothrow_copy_assignable_v<因果承接身份>
        &&std::is_nothrow_move_constructible_v<因果重筹办前缀>
        &&std::is_nothrow_move_assignable_v<因果重筹办前缀>);
    struct 因果等待记录 final {
        新任务筹办反馈定位 定位;
        新自我筹办裁决::因果采用回执 回执;
        新任务因果等待依据 依据;
        std::vector<bool> 子需求已承接;
        bool 来源已变化 = false;
        std::vector<因果子需求承接进度> 子需求承接进度;
        因果重筹办前缀 重新筹办前缀;
    };
    std::map<std::uint64_t, 因果等待记录> 因果等待组;
    std::chrono::steady_clock::time_point 下一因果复核时点{};

    using 因果等待组织快照 = std::vector<std::pair<bool, std::pair<稳定编码, 稳定编码>>>;
    // 调用者持管理锁。false为D引用，true为直接T引用，不混淆两个身份空间。
    // 诊断责任：向上送出；调用者已取得本manager原锁。
    因果等待组织快照 读取因果等待组织已加锁(std::stop_token 停止 = {}) const {
        检查因果提交停止(停止);
        因果等待组织快照 结果;
        for (const auto& [_, 记录] : 因果等待组) {
            检查因果提交停止(停止);
            for (auto D : 记录.依据.子需求节点组) {
                检查因果提交停止(停止);
                结果.push_back({false, {记录.定位.任务节点, D}});
            }
            for (auto T : 记录.依据.待推进任务组) {
                检查因果提交停止(停止);
                结果.push_back({true, {记录.定位.任务节点, T}});
            }
        }
        for (const auto& [_, 记录] : 最近自我筹办裁决) {
            检查因果提交停止(停止);
            for (const auto& D : 记录.已确认子需求组) {
                检查因果提交停止(停止);
                结果.push_back({false, {记录.裁决.定位.任务节点, D.需求节点}});
            }
        }
        检查因果提交停止(停止);
        std::sort(结果.begin(), 结果.end());
        检查因果提交停止(停止);
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        检查因果提交停止(停止);
        return 结果;
    }

    // 只复制manager的等待组织；D→T在锁外由任务owner正式读取。
    std::vector<新因果任务等待边> 读取因果等待边内部(
        因果等待组织快照* 组织快照 = nullptr, std::stop_token 停止 = {}) const {
        检查因果提交停止(停止);
        std::vector<std::pair<稳定编码, 稳定编码>> 需求边;
        std::vector<新因果任务等待边> 结果;
        {
            auto 锁 = 获取因果提交锁(互斥, 停止);
            if (组织快照) *组织快照 = 读取因果等待组织已加锁(停止);
            for (const auto& [_, 记录] : 因果等待组) {
                检查因果提交停止(停止);
                const auto 父任务 = 记录.定位.任务节点;
                for (auto 子需求 : 记录.依据.子需求节点组) {
                    检查因果提交停止(停止);
                    需求边.emplace_back(父任务, 子需求);
                }
                for (auto 候选 : 记录.依据.待推进任务组) {
                    检查因果提交停止(停止);
                    结果.push_back({父任务, 候选});
                }
            }
            for (const auto& [_, 记录] : 最近自我筹办裁决) {
                检查因果提交停止(停止);
                for (const auto& 子需求 : 记录.已确认子需求组) {
                    检查因果提交停止(停止);
                    需求边.emplace_back(记录.裁决.定位.任务节点, 子需求.需求节点);
                }
            }
        }
        for (const auto& [父任务, 子需求] : 需求边) {
            检查因果提交停止(停止);
            const auto 读取 = 任务服务->查询需求任务(子需求, 停止);
            if (std::holds_alternative<新任务因果读取取消>(读取)) throw 新任务因果读取取消{};
            const auto& 子任务 = std::get<std::optional<稳定编码>>(读取);
            检查因果提交停止(停止);
            if (子任务) 结果.push_back({父任务, *子任务});
        }
        const auto 小于 = [](const auto& 左, const auto& 右) {
            return std::pair{左.等待任务节点, 左.被等待任务节点}
                < std::pair{右.等待任务节点, 右.被等待任务节点};
        };
        检查因果提交停止(停止);
        std::sort(结果.begin(), 结果.end(), 小于);
        检查因果提交停止(停止);
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        检查因果提交停止(停止);
        return 结果;
    }

    // 诊断责任：向上送出；只由两个筹办提交入口调用。
    新自我因果筹办提交结果 提交因果筹办裁决内部(
        const 新自我筹办裁决&, std::stop_token);
    新自我因果筹办提交结果 提交非执行筹办裁决内部(
        const 新自我筹办裁决&, const std::optional<新任务因果子需求提议>&,
        std::stop_token);
    std::map<std::uint64_t, 新任务执行处理结果> 最近执行结果;
    // 只有运行期定位；正式结果仍从同一T/X读取，不保存第二份状态材料。
    struct 结果等待记录 final {
        稳定编码 执行实例节点;
        std::chrono::steady_clock::time_point 下次检测时点{};
        bool 待检测 = true;
        bool 在途期间再次唤醒 = false;
        std::optional<新任务工作请求> 原检测工作;
        bool 已派发 = false;
    };
    std::map<std::uint64_t, 结果等待记录> 结果等待任务;
    bool 结果通知待处理 = false;
    bool 下次优先结果检测 = false;
    std::uint64_t 最近检测任务值 = 0;
    std::set<std::uint64_t> 待结果反馈任务;
    std::map<std::uint64_t, 新任务贡献反馈定位> 待完成贡献复核;
    std::map<std::uint64_t, 新任务贡献反馈定位> 贡献复核在途;
    std::map<std::uint64_t, 新任务贡献反馈定位> 贡献复核再次唤醒;
    std::map<std::uint64_t, 新任务贡献反馈定位> 贡献复核待重投;
    std::vector<新任务贡献反馈定位> 贡献复核批次;
    bool 贡献复核批次来自新进度 = false;
    std::size_t 贡献复核游标 = 0;
    bool 贡献复核待触发 = true;
    std::chrono::steady_clock::time_point 下次贡献投递{};
    std::set<稳定编码> 已通知贡献接收;
    std::set<稳定编码> 待最终贡献消费;
    std::map<std::uint64_t, 新自我任务后继决议> 最近任务后继决议;
    std::optional<新长期服务值结算配置> 服务值结算配置;
    std::optional<稳定编码> 服务值结算任务;
    // 仅为进程内交接队列；绑定及重新筹办只由管理循环推进。
    std::vector<稳定编码> 待服务结算来源;
    bool 服务结算来源排空中 = false;
    const void* 自我反馈所有者 = nullptr;
    新_任务管理线程类::筹办反馈接收函数 自我反馈接收;
    新_任务管理线程类::结果反馈接收函数 自我结果反馈接收;
    新_任务管理线程类::学习复核接收函数 自我学习反馈接收;
    新_任务管理线程类::等待复核接收函数 自我等待反馈接收;
    新_任务管理线程类::贡献反馈接收函数 自我贡献反馈接收;
    std::function<void()> 自我最终工作唤醒;
    bool 工程停止已报告 = false;
    std::uint64_t 下一工作编号 = 1;

    void 报告工程停止(std::string_view 原因) noexcept {
        try {
            新服务值结算确认接口* 诊断 = nullptr;
            {
                std::lock_guard 锁(互斥);
                if (工程停止已报告) return;
                内部错误发生 = true;
                停止源.request_stop(); 停止 = true;
                工程停止已报告 = true;
                if (服务值结算配置) 诊断 = 服务值结算配置->工程诊断端口;
                变化.notify_all();
            }
            if (诊断) 诊断->报告并停止(原因);
        } catch (...) {}
    }

    新任务管理操作结果 操作结果(新任务管理操作状态 状态) const noexcept {
        return {状态, 生命周期};
    }

    bool 有服务结算来源待处理已加锁(std::uint64_t 任务值) const noexcept {
        return 服务值结算任务 && 服务值结算任务->值 == 任务值
            && !待服务结算来源.empty();
    }

    bool 学习占用或旧筹办已失效已加锁(
        const 新任务筹办反馈定位& 定位) const noexcept {
        const auto 位置 = 学习失效筹办工作.find(定位.任务节点.值);
        return 学习复核占用.contains(定位.任务节点.值)
            || 等待复核占用.contains(定位.任务节点.值)
            || 有服务结算来源待处理已加锁(定位.任务节点.值)
            || (位置 != 学习失效筹办工作.end()
                && 位置->second == 定位.原工作.工作编号);
    }

    // 仅登记运行进度；调用者已持邮箱锁，不在这里调用领域服务。
    void 触发条件子需求复核已加锁() noexcept {
        调度待触发 = true;
        for (auto& [任务值, 记录] : 最近自我筹办裁决) {
            (void)任务值;
            if (记录.裁决.子需求回执 && 记录.已完成接收)
                记录.条件复核待触发 = true;
        }
        变化.notify_all();
    }

    // 仅置进度标记；调用者持邮箱锁，领域读取始终在锁外完成。
    void 标记贡献复核待触发() noexcept {
        贡献复核待触发 = true;
        变化.notify_all();
    }

    bool 子需求承接定位有效已加锁(const 消息项& 项) const noexcept {
        if (!项.父筹办定位
            || 学习占用或旧筹办已失效已加锁(*项.父筹办定位)) return false;
        const auto 任务值 = 项.父筹办定位->任务节点.值;
        const auto 筹办 = 最近筹办结果.find(任务值);
        const auto 裁决 = 最近自我筹办裁决.find(任务值);
        if (筹办 == 最近筹办结果.end()
            || 筹办->second.原工作 != 项.父筹办定位->原工作
            || 裁决 == 最近自我筹办裁决.end()
            || !裁决->second.已完成接收
            || 裁决->second.裁决.定位 != *项.父筹办定位
            || !裁决->second.裁决.子需求回执
            || 裁决->second.来源变化子需求.contains(项.消息.需求节点.值)) return false;
        return std::any_of(裁决->second.已确认子需求组.begin(),
            裁决->second.已确认子需求组.end(), [&](const auto& 子项) {
                return 子项.需求节点 == 项.消息.需求节点;
            });
    }

    // 诊断责任：向上送出。原正式接收的不可变投影只在task实际接收后去项。
    void 尝试交付条件准备依据(std::stop_token 停止令牌) {
        检查因果提交停止(停止令牌);
        std::vector<条件准备待交付指针> 待交付;
        {
            auto 锁 = 获取因果提交锁(互斥, 停止令牌);
            if (停止 || 已完成) return;
            待交付.reserve(待交付条件准备组.size());
            for (const auto& [子D, 项] : 待交付条件准备组) {
                检查因果提交停止(停止令牌);
                if (!项 || 子D != 项->已确认项.需求节点.值)
                    throw std::logic_error("条件准备待交付身份不符");
                待交付.push_back(项);
            }
        }
        for (const auto& 项 : 待交付) {
            检查因果提交停止(停止令牌);
            const auto 返回 = 任务服务->接收已确认方法条件准备依据(
                项->原提议, 项->已确认项, 停止令牌);
            if (std::holds_alternative<新任务因果读取取消>(返回))
                throw 新任务因果读取取消{};
            const auto* 正常 = std::get_if<新_任务类::条件准备交付状态>(&返回);
            if (!正常) throw std::logic_error("条件准备交付回执分支不符");
            switch (*正常) {
            case 新_任务类::条件准备交付状态::来源已变化:
            case 新_任务类::条件准备交付状态::依赖未就绪:
            case 新_任务类::条件准备交付状态::未支持:
                continue; // 保原待项，原调度节奏在后续材料到达后再试。
            case 新_任务类::条件准备交付状态::资源失败:
                return;
            case 新_任务类::条件准备交付状态::已交付:
            case 新_任务类::条件准备交付状态::已复用:
                break;
            default:
                throw std::logic_error("条件准备交付状态未知");
            }
            // task已经发表或复用。这里不响应stop、不分配，必须先收口真实交付。
            std::lock_guard 锁(互斥);
            const auto 当前待项 = 待交付条件准备组.find(项->已确认项.需求节点.值);
            if (当前待项 != 待交付条件准备组.end()) {
                if (!当前待项->second || 当前待项->second->原提议 != 项->原提议
                    || 当前待项->second->已确认项 != 项->已确认项)
                    throw std::logic_error("条件准备交付后同键原值改变");
                待交付条件准备组.erase(当前待项);
            }
            // 另一调用可已收口同待项；工程工作定位更新或record退出不撤销交付。
            const auto 当前记录 = 最近自我筹办裁决.find(项->原提议.任务节点.值);
            if (当前记录 != 最近自我筹办裁决.end()) {
                auto& 记录 = 当前记录->second;
                if (记录.已完成接收 && 记录.裁决.子需求回执
                    && 记录.裁决.定位.任务节点 == 项->原提议.任务节点
                    && 记录.裁决.子需求回执->原提议 == 项->原提议) {
                    if (记录.条件准备依据已交付组.size() != 记录.已确认子需求组.size())
                        throw std::logic_error("条件准备逐项交付进度不完整");
                    for (std::size_t i = 0; i < 记录.已确认子需求组.size(); ++i)
                        if (记录.已确认子需求组[i] == 项->已确认项)
                            记录.条件准备依据已交付组[i] = true;
                }
            }
            调度待触发 = true;
            变化.notify_all();
        }
    }

    void 尝试复核条件子需求() noexcept {
        struct 待复核 final {
            新任务筹办反馈定位 定位;
            std::vector<新任务已确认条件子需求> 子需求组;
            新任务条件子需求提议 原提议;
            bool 来源过期 = false;
        };
        try {
            std::vector<待复核> 待复核组;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                const auto 此刻 = std::chrono::steady_clock::now();
                if (此刻 < 下一条件复核时点) return;
                下一条件复核时点 = 此刻 + 等待时长(配置.调度复核间隔毫秒);
            }
            const auto 停止令牌 = 停止源.get_token();
            尝试交付条件准备依据(停止令牌);
            {
                auto 锁 = 获取因果提交锁(互斥, 停止令牌);
                if (停止 || 已完成) return;
                for (auto& [任务值, 记录] : 最近自我筹办裁决) {
                    (void)任务值;
                    if (学习占用或旧筹办已失效已加锁(记录.裁决.定位)
                        || 任务有执行保护已加锁(任务值)
                        || !记录.已完成接收 || !记录.裁决.子需求回执
                        || !记录.条件复核待触发 || 记录.条件交接失败
                        || 记录.条件准备依据已交付组.size() != 记录.已确认子需求组.size()
                        || !std::all_of(记录.条件准备依据已交付组.begin(),
                            记录.条件准备依据已交付组.end(), [](bool 完成) { return 完成; })
                        || !std::all_of(记录.子需求承接完成组.begin(),
                            记录.子需求承接完成组.end(), [](bool 完成) { return 完成; }))
                        continue;
                    待复核组.push_back({记录.裁决.定位, 记录.已确认子需求组,
                        记录.裁决.子需求回执->原提议, 记录.条件来源待重筹办});
                }
            }
            for (const auto& 项 : 待复核组) {
                const auto 就绪 = 任务服务->复核条件子需求就绪(
                    项.定位.任务节点, 项.子需求组);
                if (就绪.状态 == 新任务条件子需求就绪状态::结构不一致
                    || 就绪.状态 == 新任务条件子需求就绪状态::资源失败)
                    throw std::logic_error("条件子需求就绪读取失败");
                if (就绪.状态 == 新任务条件子需求就绪状态::任务状态已变化)
                    continue;
                const auto 当前 = 任务服务->提议任务条件子需求(项.定位.任务节点);
                if (当前.状态 == 新任务条件子需求提议状态::结构不一致
                    || 当前.状态 == 新任务条件子需求提议状态::资源失败)
                    throw std::logic_error("条件子需求重新分解失败");
                if (当前.状态 == 新任务条件子需求提议状态::任务状态已变化) continue;
                const bool 全部就绪 = 就绪.状态 == 新任务条件子需求就绪状态::已就绪
                    && 就绪.全部需求已满足 && 就绪.当前选择
                    && 就绪.当前选择->状态 == 新任务方法选择状态::已选定;
                if(!全部就绪&&!项.来源过期) {
                    if(条件提议等材料(当前))continue;
                    if(当前.状态!=新任务条件子需求提议状态::无需子需求&&条件提议来源一致(当前,项.原提议)) {
                        const bool 有未覆盖项=std::any_of(当前.子需求项组.begin(),当前.子需求项组.end(),[&](const auto& 待办){
                            return std::none_of(项.子需求组.begin(),项.子需求组.end(),[&](const auto& 已确认){
                                return 已确认.原提议==待办;
                            });
                        });
                        const bool 有变化项=std::any_of(项.子需求组.begin(),项.子需求组.end(),[&](const auto& 已确认){
                            return 读取条件子项当前支持(当前,项.原提议,已确认.原提议)==条件子项当前支持::已变化;
                        });
                        if(!有未覆盖项&&!有变化项)continue;
                    }
                }
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                const auto 任务值 = 项.定位.任务节点.值;
                const auto 筹办 = 最近筹办结果.find(任务值);
                const auto 裁决 = 最近自我筹办裁决.find(任务值);
                if (筹办 == 最近筹办结果.end()
                    || 任务有执行保护已加锁(任务值)
                    || 学习占用或旧筹办已失效已加锁(项.定位)
                    || 筹办->second.原工作 != 项.定位.原工作
                    || 裁决 == 最近自我筹办裁决.end()
                    || 裁决->second.裁决.定位 != 项.定位
                    || 裁决->second.已确认子需求组 != 项.子需求组
                    || !裁决->second.裁决.子需求回执
                    || 裁决->second.裁决.子需求回执->原提议 != 项.原提议
                    || 裁决->second.条件来源待重筹办 != 项.来源过期
                    || 裁决->second.条件交接失败
                    || 裁决->second.条件准备依据已交付组.size() != 裁决->second.已确认子需求组.size()
                    || !std::all_of(裁决->second.条件准备依据已交付组.begin(),
                        裁决->second.条件准备依据已交付组.end(), [](bool 完成) { return 完成; })
                    || !std::all_of(裁决->second.子需求承接完成组.begin(),
                        裁决->second.子需求承接完成组.end(), [](bool 完成) { return 完成; }))
                    continue;
                待反馈任务.erase(任务值);
                最近筹办结果.erase(筹办);
                最近自我筹办裁决.erase(裁决);
                // 只释放相同W2的软阻塞；独立读回/确认/执行保护绝不清空。
                筹办阻塞任务.erase(任务值);
                调度待触发 = true;
                // T仍是待检查条件，下一步只经普通调度形成新W2。
                变化.notify_all();
            }
        } catch (const 新任务因果读取取消&) {
            // 已交付事实、待项及原record均保留；不继续下一次跨owner调用。
        } catch (const std::bad_alloc&) {
            // 当前尚未完成的待项由原条件复核节奏继续，不重建D。
        } catch (const std::length_error&) {
            // 容器资源边界同上；不是业务否定或交付成功。
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"尝试复核条件子需求", L"条件分解重新筹办工程故障");
        }
    }

    void 终结待办(新任务承接处理状态 状态) noexcept {
        for (const auto& 项 : 活动) {
            try { 项->承诺.set_value({项->消息, 状态, std::nullopt}); }
            catch (...) {}
        }
        活动.clear();
        for (auto& [编号, 记录] : 学习复核组) {
            (void)编号;
            记录.已终结 = true;
        }
        学习复核占用.clear();
        for (auto& [编号, 记录] : 等待复核组) {
            (void)编号;
            记录.已终结 = true;
        }
        等待复核占用.clear();
        const auto 未交付 = std::find_if(学习事件组.begin(), 学习事件组.end(),
            [](const auto& 事件) { return !事件.second.已终结; });
        if (未交付 != 学习事件组.end())
            最近学习运行异常 = 新学习运行异常记录{
                未交付->second.通知.方法节点, 新学习运行异常原因::停止取消};
    }

    void 尝试反馈筹办结果() noexcept {
        新任务筹办反馈定位 定位;
        新_任务管理线程类::筹办反馈接收函数 接收;
        try {
            {
                std::lock_guard 锁(互斥);
                if (!自我反馈接收 || 待反馈任务.empty() || 停止 || 已完成) return;
                const auto 待反馈 = std::find_if(待反馈任务.begin(), 待反馈任务.end(),
                    [&](const auto 任务值) {
                        const auto 位置 = 最近筹办结果.find(任务值);
                        return 位置 == 最近筹办结果.end()
                            || !学习占用或旧筹办已失效已加锁(
                                {稳定编码{任务值}, 位置->second.原工作});
                    });
                if (待反馈 == 待反馈任务.end()) return;
                const auto 任务值 = *待反馈;
                const auto 结果位置 = 最近筹办结果.find(任务值);
                if (结果位置 == 最近筹办结果.end()) {
                    待反馈任务.erase(任务值);
                    return;
                }
                定位 = {稳定编码{任务值}, 结果位置->second.原工作};
                接收 = 自我反馈接收;
            }
            // 回调可能进入self邮箱，绝不持manager互斥调用。
            (void)接收(定位);
        } catch (...) {
            // 定位仍保留在待反馈集合；后续按具名重试间隔再次尝试。
        }
    }

    void 尝试反馈任务结果() noexcept {
        新任务结果反馈定位 定位;
        新_任务管理线程类::结果反馈接收函数 接收;
        try {
            {
                std::lock_guard 锁(互斥);
                if (!自我结果反馈接收 || 待结果反馈任务.empty()
                    || 停止 || 已完成) return;
                const auto 任务值 = *待结果反馈任务.begin();
                const auto 结果位置 = 最近执行结果.find(任务值);
                if (结果位置 == 最近执行结果.end()
                    || 结果位置->second.状态
                        != 新任务执行处理状态::已完成影响评估
                    || !结果位置->second.独立读回
                    || !结果位置->second.独立读回->执行
                    || !结果位置->second.独立读回->影响评估) {
                    待结果反馈任务.erase(任务值);
                    return;
                }
                定位 = {稳定编码{任务值}, 结果位置->second.原工作,
                    结果位置->second.独立读回->执行->节点,
                    结果位置->second.独立读回->影响评估->节点};
                接收 = 自我结果反馈接收;
            }
            // 跨task/factory/publisher的正式读取不持manager互斥。
            const auto 需求结果 = 任务服务->读取执行已发布需求(定位.任务节点, 定位.执行实例节点);
            switch (需求结果.状态) {
            case 新任务需求结果读取状态::已读取:
                if (!需求结果.结果)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"反馈已发布需求", L"正式需求结果成功但无材料。");
                定位.已发布需求节点 = 需求结果.结果->需求节点;
                break;
            case 新任务需求结果读取状态::无此结果:
                if (需求结果.结果)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"反馈已发布需求", L"无需求结果却携带材料。");
                break;
            case 新任务需求结果读取状态::资源失败:
            case 新任务需求结果读取状态::前提已变化:
            case 新任务需求结果读取状态::执行不存在:
                return; // 保留原待反馈定位，后续按现调度重读。
            default:
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"反馈已发布需求", L"正式需求结果依赖未装配或状态不合法。");
            }
            {
                std::lock_guard 锁(互斥);
                const auto 当前 = 最近执行结果.find(定位.任务节点.值);
                if (停止 || 已完成 || !待结果反馈任务.contains(定位.任务节点.值)
                    || 当前 == 最近执行结果.end()
                    || 当前->second.原工作 != 定位.原工作
                    || 当前->second.状态 != 新任务执行处理状态::已完成影响评估
                    || !当前->second.独立读回 || !当前->second.独立读回->执行
                    || !当前->second.独立读回->影响评估
                    || 当前->second.独立读回->执行->节点 != 定位.执行实例节点
                    || 当前->second.独立读回->影响评估->节点 != 定位.影响评估节点) return;
            }
            (void)接收(定位);
        } catch (const std::bad_alloc&) {
            // 保留正式定位，资源恢复后按现调度再次尝试。
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"反馈已发布需求", L"任务结果正式交付工程异常，保留原定位与已发表事实。");
        }
    }

    void 尝试反馈任务完成贡献() noexcept {
        try {
            bool 建批 = false;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成 || !自我贡献反馈接收) return;
                if (贡献复核游标 == 贡献复核批次.size() && !贡献复核待重投.empty()) {
                    贡献复核批次.clear();
                    for (const auto& [任务值, 定位] : 贡献复核待重投) {
                        (void)任务值;
                        贡献复核批次.push_back(定位);
                    }
                    贡献复核待重投.clear();
                    贡献复核游标 = 0;
                    贡献复核批次来自新进度 = false;
                } else if (贡献复核游标 == 贡献复核批次.size() && 贡献复核待触发) {
                    贡献复核待触发 = false;
                    建批 = true;
                }
            }
            if (建批) {
                const auto 全集 = 任务服务->查询全部任务();
                if (全集.状态 != 新任务操作状态::已找到)
                    throw std::logic_error("贡献复核任务全集读取失败");
                std::vector<新任务贡献反馈定位> 新批次;
                bool 需要重读 = false;
                for (const auto 编码 : 全集.任务节点组) {
                    const auto 任务 = 任务服务->获取任务(编码);
                    if (!任务) throw std::logic_error("贡献复核任务不可读");
                    if (任务->当前状态 != 新任务状态::已完成
                        && 任务->当前状态 != 新任务状态::已结算) continue;
                    const auto 读取 = 任务服务->读取任务完成贡献交接(编码);
                    if (读取.状态 == 新任务贡献交接读取状态::预期已变化) {
                        需要重读 = true; continue;
                    }
                    if (读取.状态 != 新任务贡献交接读取状态::已读取 || !读取.交接)
                        throw std::logic_error("贡献交接完整投影读取失败");
                    const auto& 交接 = *读取.交接;
                    // R已存在仍可能尚未收尾；只补普通9或明确待配置，不重复计值。
                    if (交接.完成结算节点 && (!交接.服务贡献值
                        || !交接.直接父需求节点)) continue;
                    if (交接.接收节点
                        && 交接.直属收尾.分类 != 新任务直属收尾分类::普通待收尾
                        && 交接.直属收尾.分类 != 新任务直属收尾分类::等待结算配置
                        && 交接.直属收尾.分类 != 新任务直属收尾分类::最终消费保留) continue;
                    const auto 执行码 = 任务->最近执行实例节点;
                    if (!执行码) throw std::logic_error("已完成任务缺少执行定位");
                    const auto 执行 = 任务服务->获取执行实例(*执行码);
                    if (!执行 || 执行->任务节点 != 编码 || !执行->影响评估节点)
                        throw std::logic_error("已完成任务缺少评估定位");
                    if (交接.完成结算节点) {
                        const auto 结算 = 任务服务->读取任务完成结算(编码);
                        if (!结算 || 结算->节点 != *交接.完成结算节点) {
                            需要重读 = true; continue;
                        }
                        // 旧K不授予新X/A贡献资格。来源不符是稳定等待，不是新进度。
                        if (结算->执行实例节点 != *执行码
                            || 结算->影响评估节点 != *执行->影响评估节点) continue;
                    }
                    新批次.push_back({编码, *执行码, *执行->影响评估节点});
                }
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                贡献复核批次.swap(新批次);
                贡献复核游标 = 0;
                贡献复核批次来自新进度 = true;
                贡献复核待触发 = 贡献复核待触发 || 需要重读;
            }
            新任务贡献反馈定位 定位;
            新_任务管理线程类::贡献反馈接收函数 接收;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成 || !自我贡献反馈接收
                    || std::chrono::steady_clock::now() < 下次贡献投递) return;
                while (贡献复核游标 < 贡献复核批次.size()
                    && 贡献复核在途.contains(贡献复核批次[贡献复核游标].任务节点.值)) {
                    if (贡献复核批次来自新进度) {
                        const auto& 待重读 = 贡献复核批次[贡献复核游标];
                        贡献复核再次唤醒.insert_or_assign(待重读.任务节点.值, 待重读);
                    }
                    ++贡献复核游标;
                }
                if (贡献复核游标 == 贡献复核批次.size()) return;
                定位 = 贡献复核批次[贡献复核游标];
                待完成贡献复核.insert_or_assign(定位.任务节点.值, 定位);
                // 回调可能很快完成确认，故先占位，回调成功后不能重新加回在途。
                贡献复核在途.emplace(定位.任务节点.值, 定位);
                接收 = 自我贡献反馈接收;
                下次贡献投递 = std::chrono::steady_clock::now() + 等待时长(配置.资源重试间隔毫秒);
            }
            const auto 接纳 = 接收(定位);
            {
                std::lock_guard 锁(互斥);
                if (接纳 == 新任务结果反馈接收状态::已接收
                    || 接纳 == 新任务结果反馈接收状态::精确重复) {
                    ++贡献复核游标;
                    return;
                }
                贡献复核在途.erase(定位.任务节点.值);
                if (接纳 == 新任务结果反馈接收状态::邮箱已满
                    || 接纳 == 新任务结果反馈接收状态::资源失败
                    || 接纳 == 新任务结果反馈接收状态::尚未启动
                    || 接纳 == 新任务结果反馈接收状态::已停止接收) return;
            }
            throw std::logic_error("贡献复核消息被拒绝或身份冲突");
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"尝试反馈任务完成贡献", L"贡献复核读取或消息交接异常");
        }
    }

    // 仅在manager锁内登记已独立读回的当前执行，不做领域读取。
    void 登记结果等待已加锁(稳定编码 任务节点, 稳定编码 执行实例节点) {
        auto [位置, 新建] = 结果等待任务.try_emplace(任务节点.值);
        auto& 记录 = 位置->second;
        if (!新建 && 记录.执行实例节点 != 执行实例节点)
            throw std::logic_error("same task has another pending execution");
        记录.执行实例节点 = 执行实例节点;
        if (记录.已派发) 记录.在途期间再次唤醒 = true;
        else 记录.待检测 = true;
    }

    void 更新结果检测唤醒已加锁() noexcept {
        const bool 收到通知 = 结果通知待处理;
        结果通知待处理 = false;
        const auto 当前 = std::chrono::steady_clock::now();
        for (auto& [任务值, 记录] : 结果等待任务) {
            (void)任务值;
            if (记录.已派发) {
                if (收到通知) 记录.在途期间再次唤醒 = true;
            } else if (收到通知 || 当前 >= 记录.下次检测时点) {
                记录.待检测 = true;
            }
        }
    }

    筹办派发状态 尝试派发结果检测() noexcept {
        try {
            std::unique_lock 锁(互斥);
            if (停止 || 已完成) return 筹办派发状态::已停止;
            更新结果检测唤醒已加锁();
            if (结果等待任务.empty()) return 筹办派发状态::无可派发任务;
            auto 位置 = 结果等待任务.upper_bound(最近检测任务值);
            for (std::size_t 剩余 = 结果等待任务.size(); 剩余; --剩余) {
                if (位置 == 结果等待任务.end()) 位置 = 结果等待任务.begin();
                const auto 当前位置 = 位置++;
                const auto 任务值 = 当前位置->first;
                auto& 记录 = 当前位置->second;
                if (!记录.待检测 || 记录.已派发 || 贡献占用任务已加锁(任务值)
                    || std::any_of(活动工作.begin(), 活动工作.end(),
                        [&](const auto& 项) { return 项.请求.任务节点.值 == 任务值; }))
                    continue;
                活动工作.reserve(活动工作.size() + 1);
                if (!记录.原检测工作) {
                    if (!下一工作编号 || 下一工作编号 == (std::numeric_limits<std::uint64_t>::max)())
                        return 筹办派发状态::内部错误;
                    新任务工作请求 请求;
                    请求.工作编号 = 下一工作编号++;
                    请求.种类 = 新任务工作种类::检测本能结果;
                    请求.任务节点 = 稳定编码{任务值};
                    请求.本能结果检测请求 = 新任务结果检测请求{请求.任务节点, 记录.执行实例节点};
                    记录.原检测工作 = std::move(请求);
                }
                工作占用 待接纳{*记录.原检测工作, {}};
                锁.unlock();
                const auto 接收 = 提交普通工作(待接纳.请求);
                锁.lock();
                if (接收.状态 == 新任务工作接收状态::已接收
                    || 接收.状态 == 新任务工作接收状态::精确重复) {
                    // 提交前已预留；即使停止恰好到达也必须保住future交给收束。
                    待接纳.结果 = 接收.处理结果;
                    const auto 当前 = 结果等待任务.find(任务值);
                    if (当前 == 结果等待任务.end()
                        || 当前->second.原检测工作 != 待接纳.请求)
                        return 筹办派发状态::内部错误;
                    当前->second.已派发 = true;
                    当前->second.待检测 = false;
                    最近检测任务值 = 任务值;
                    return 筹办派发状态::已派发;
                }
                // 提交期间已释放manager锁，贡献确认可能改动等待表；不再沿用旧迭代器。
                if (接收.状态 == 新任务工作接收状态::任务已占用)
                    return 筹办派发状态::无可派发任务;
                if (接收.状态 == 新任务工作接收状态::邮箱已满
                    || 接收.状态 == 新任务工作接收状态::资源失败)
                    return 筹办派发状态::资源暂不可用;
                return 接收.状态 == 新任务工作接收状态::已停止接收
                    ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
            }
            return 筹办派发状态::无可派发任务;
        } catch (const std::bad_alloc&) { return 筹办派发状态::资源暂不可用; }
        catch (...) { return 筹办派发状态::内部错误; }
    }

    bool 任务有执行保护已加锁(std::uint64_t 任务值) const noexcept {
        return 贡献占用任务已加锁(任务值) || 普通工作提交中.contains(任务值)
            || 阻塞任务.contains(任务值)
            || 结果等待任务.contains(任务值) || 待结果反馈任务.contains(任务值)
            || std::any_of(活动工作.begin(), 活动工作.end(), [&](const auto& 工作) {
                return 工作.请求.任务节点.值 == 任务值;
            });
    }

    void 登记等待来源已加锁(const 新任务信息& 任务, 新任务等待原因 原因,
        std::optional<新任务工作请求> 原工作 = {}) {
        等待来源.insert_or_assign(任务.节点.值, 等待来源记录{任务, 原因, std::move(原工作)});
        上次等待材料.erase(任务.节点.值);
        if (原因 == 新任务等待原因::尚无方法
            || 原因 == 新任务等待原因::候选条件未齐) {
            const auto& 来源 = 等待来源.at(任务.节点.值);
            const auto 原 = 筹办知识基线.find(任务.节点.值);
            if (!来源.原工作
                || (来源.原工作->种类 != 新任务工作种类::查找方法
                    && 来源.原工作->种类 != 新任务工作种类::检查条件)
                || 原 == 筹办知识基线.end() || 原->second.原工作 != *来源.原工作)
                throw std::logic_error("planning wait lacks original scheduling baseline");
            auto 基线 = 原->second.材料;
            // W1完整零召回已证明本次实际消费的是空候选，不沿用更早W0候选。
            if (原因 == 新任务等待原因::尚无方法
                && 来源.原工作->种类 == 新任务工作种类::查找方法) 基线.当前候选方法闭包.clear();
            // 条件等待也保留派发前的W0材料，不能拿W2选择结论与W0核验混比。
            // 使用原工作基线，仍能发现工作运行至等待登记期间发生的真实变化。
            上次等待材料.insert_or_assign(任务.节点.值, std::move(基线));
        }
        调度待触发 = true;
    }

    static bool 等待能力知识相同(新任务调度材料项 左, 新任务调度材料项 右) {
        for (auto* 项 : {&左, &右})
            for (auto& 方法 : 项->当前候选方法闭包)
                for (auto& 配对 : 方法.条件结果组) 配对.命中次数 = 0;
        return 左.定位.任务节点 == 右.定位.任务节点 && 左.定位.需求节点 == 右.定位.需求节点
            && 左.需求 && 右.需求 && 左.需求->目标存在节点 == 右.需求->目标存在节点
            && 左.需求->目标 == 右.需求->目标 && 左.当前候选方法闭包 == 右.当前候选方法闭包;
    }

    static bool 等待知识相同(新任务调度材料项 左, 新任务调度材料项 右) {
        for (auto* 项 : {&左, &右}) {
            // 权重不是候选条件知识；仅归一比较副本，不改真实调度材料。
            if (项->需求) 项->需求->权重 = 0;
            for (auto& 方法 : 项->当前候选方法闭包)
                for (auto& 配对 : 方法.条件结果组) 配对.命中次数 = 0;
        }
        return 左.定位 == 右.定位 && 左.需求 == 右.需求
            && 左.候选材料 == 右.候选材料 && 左.条件核验 == 右.条件核验
            && 左.当前候选方法闭包 == 右.当前候选方法闭包;
    }

    // 只消费完整W0。所有可能分配失败的副本先构造，最后在邮箱锁内交换。
    void 处理调度扫描结果(const 工作占用& 占用) noexcept {
        try {
            const auto& 返回 = 占用.结果.get();
            if (返回.原请求 != 占用.请求
                || (返回.状态 != 新任务工作处理状态::调度材料已返回
                    && 返回.状态 != 新任务工作处理状态::停止取消)
                || (返回.状态 == 新任务工作处理状态::调度材料已返回 && !返回.调度材料)) {
                报告工程停止("调度扫描工作返回结构不一致");
                return;
            }
            std::map<std::uint64_t, 新任务调度材料项> 新材料;
            std::vector<稳定编码> 异常任务;
            const bool 已读取 = 返回.状态 == 新任务工作处理状态::调度材料已返回
                && 返回.调度材料->状态 == 新任务调度读取状态::已读取;
            if (已读取) {
                for (const auto& 项 : 返回.调度材料->项组) {
                    if (!有效(项.定位.任务节点)
                        || !新材料.emplace(项.定位.任务节点.值, 项).second) {
                        报告工程停止("调度扫描返回重复或空任务身份");
                        return;
                    }
                    if (项.状态 == 新任务调度项状态::结构不一致)
                        异常任务.push_back(项.定位.任务节点);
                }
            }
            std::unique_lock 锁(互斥);
            if (零动作失效调度工作.contains(占用.请求.工作编号)) {
                if (工作线程->确认结果已接收(占用.请求.工作编号, {}) != 新任务工作操作状态::已完成) {
                    锁.unlock();
                    报告工程停止("条件失效后的陈旧调度结果无法确认回收");
                    return;
                }
                零动作失效调度工作.erase(占用.请求.工作编号);
                std::erase_if(活动工作, [&](const auto& 项) { return 项.请求.工作编号 == 占用.请求.工作编号; });
                调度材料.clear(); 已有完整调度材料 = false; 调度待触发 = true;
                变化.notify_all();
                return;
            }
            const bool 诊断变化 = 返回.调度材料
                && (最近调度读取状态 != 返回.调度材料->状态 || 最近调度异常任务 != 异常任务);
            auto 新上次 = 上次等待材料;
            auto 新记录 = 等待复核组;
            auto 新占用 = 等待复核占用;
            auto 新失效 = 学习失效筹办工作;
            auto 新编号 = 下一等待复核编号;
            if (已读取 && !停止) {
                for (const auto& [任务值, 项] : 新材料) {
                    if (项.需求治理 && !项.需求治理->完整目标) {
                        // 信息完善等待由R变化唤醒，不因定时扫描派发普通筹办。
                        新上次.insert_or_assign(任务值, 项);
                        continue;
                    }
                    const auto 状态 = 项.定位.任务状态;
                    if ((状态 != 新任务状态::等待条件 && 状态 != 新任务状态::待检查条件
                            && 状态 != 新任务状态::已建立)
                        || !项.任务 || !项.需求 || !项.定位.完整()
                        || 项.状态 == 新任务调度项状态::结构不一致
                        || 项.状态 == 新任务调度项状态::旧执行待结果
                        || 项.状态 == 新任务调度项状态::仅材料治理
                        || 任务有执行保护已加锁(任务值)
                        || 有服务结算来源待处理已加锁(任务值)
                        || 学习复核占用.contains(任务值) || 新占用.contains(任务值)) continue;
                    const auto 裁决 = 最近自我筹办裁决.find(任务值);
                    if (待反馈任务.contains(任务值)
                        || (裁决 != 最近自我筹办裁决.end()
                            && (!裁决->second.已完成接收 || 裁决->second.执行工作
                                || 裁决->second.裁决.子需求回执))) continue;
                    auto 原因 = 项.等待原因;
                    const auto 来源 = 等待来源.find(任务值);
                    if (来源 != 等待来源.end() && 来源->second.任务 == *项.任务)
                        原因 = 来源->second.原因;
                    if (原因 == 新任务等待原因::需核查 || 原因 == 新任务等待原因::未分类
                        || 原因 == 新任务等待原因::无) continue;
                    const auto 上次 = 新上次.find(任务值);
                    bool 需复核 = 等待目标复核.contains(任务值);
                    if (原因 == 新任务等待原因::首次关联待许可
                        || 原因 == 新任务等待原因::需求材料未到
                        || 原因 == 新任务等待原因::执行后等待
                        || 原因 == 新任务等待原因::同次执行比较材料不足) 需复核 = true;
                    else if (原因 == 新任务等待原因::尚无方法) {
                        // 完整空召回不会因周期经过而重复W1。
                        需复核 = 需复核 || (!项.当前候选方法闭包.empty()
                            && (上次 == 新上次.end() || !等待能力知识相同(上次->second, 项)));
                    } else if (原因 == 新任务等待原因::候选条件未齐) {
                        需复核 = 需复核 || (上次 != 新上次.end() && !等待知识相同(上次->second, 项));
                        // 无显式等待来源的首次W0只建立基线；已登记等待使用派发前W0。
                    }
                    新上次.insert_or_assign(任务值, 项);
                    if (!需复核) continue;
                    if (!新编号 || 新编号 == (std::numeric_limits<std::uint64_t>::max)()) {
                        内部错误发生 = true;
                        停止源.request_stop(); 停止 = true;
                        break;
                    }
                    等待复核记录 记录;
                    记录.定位 = {新编号++, 项.定位, 原因};
                    新记录.emplace(记录.定位.复核编号, 记录);
                    新占用.insert(任务值);
                    const auto 筹办 = 最近筹办结果.find(任务值);
                    if (筹办 != 最近筹办结果.end())
                        新失效.insert_or_assign(任务值, 筹办->second.原工作.工作编号);
                }
                for (auto 位置 = 新上次.begin(); 位置 != 新上次.end();) {
                    const auto 当前 = 新材料.find(位置->first);
                    if (当前 == 新材料.end() || !学习筹办安全状态(当前->second.定位.任务状态))
                        位置 = 新上次.erase(位置);
                    else ++位置;
                }
            }
            const auto 确认 = 工作线程->确认结果已接收(占用.请求.工作编号, {});
            if (确认 != 新任务工作操作状态::已完成) {
                内部错误发生 = true;
                停止源.request_stop(); 停止 = true;
                return; // future及容量保留，绝不伪装确认成功。
            }
            if (已读取 && !停止) {
                for (const auto& [任务值, 项] : 新材料) {
                    const auto 状态 = 项.定位.任务状态;
                    const auto 旧 = 调度材料.find(任务值);
                    if ((状态 == 新任务状态::已完成 || 状态 == 新任务状态::已结算
                            || 状态 == 新任务状态::已失败 || 状态 == 新任务状态::已取消)
                        && (旧 == 调度材料.end() || 旧->second.定位.任务状态 != 状态))
                        标记贡献复核待触发();
                }
                调度材料.swap(新材料);
                上次等待材料.swap(新上次);
                等待复核组.swap(新记录);
                等待复核占用.swap(新占用);
                学习失效筹办工作.swap(新失效);
                下一等待复核编号 = 新编号;
                已有完整调度材料 = true;
                触发条件子需求复核已加锁();
            } else if (!已读取 && 返回.调度材料
                && 返回.调度材料->状态 != 新任务调度读取状态::资源失败) {
                // 结构失败没有完整新视图，冻结旧普通候选；结果回流不受阻。
                调度材料.clear();
                已有完整调度材料 = false;
            }
            std::erase_if(活动工作, [&](const auto& 项) {
                return 项.请求.工作编号 == 占用.请求.工作编号;
            });
            const auto 此刻 = std::chrono::steady_clock::now();
            下一调度时点 = 此刻 + 等待时长(配置.调度复核间隔毫秒);
            最早事件调度时点 = 下一调度时点;
            if (返回.调度材料) {
                最近调度读取状态 = 返回.调度材料->状态;
                最近调度异常任务 = std::move(异常任务);
            }
            if (诊断变化) {
                try {
                    std::wostringstream 文本;
                    文本 << L"调度扫描读取状态=" << static_cast<int>(*最近调度读取状态)
                        << L"；完整视图=" << 已读取 << L"；工作并发上限=" << 新任务工作并发上限
                        << L"；结构异常任务=";
                    for (const auto 任务 : 最近调度异常任务) 文本 << 任务.值 << L",";
                    (void)记录运行日志(L"任务管理线程", L"任务调度扫描", 文本.str());
                } catch (...) {} // 诊断不能改变已ack工作或调度资格。
            }
            变化.notify_all();
#ifdef HY_EGO_DEBUG_LOG_TASK_SCHEDULING
            // 只投影已返回材料；不持邮箱/业务锁写日志，不追加领域查询。
            锁.unlock();
            if (返回.调度材料) {
                try {
                    static thread_local std::wstring 上次调度诊断;
                    std::wostringstream 文本;
                    for (const auto& 项 : 返回.调度材料->项组) {
                        文本 << L"任务=" << 项.定位.任务节点.值
                            << L" 需求=" << 项.定位.需求节点.值
                            << L" 任务状态=" << static_cast<int>(项.定位.任务状态)
                            << L" 调度状态=" << static_cast<int>(项.状态)
                            << L" 候选材料=" << (项.定位.候选材料节点 ? 项.定位.候选材料节点->值 : 0)
                            << L" 候选数量=" << (项.候选材料 ? 项.候选材料->候选方法节点组.size() : 0)
                            << L" 执行组织=" << (项.定位.方法执行组织节点 ? 项.定位.方法执行组织节点->值 : 0)
                            << L" 条件核验=" << (项.条件核验 ? static_cast<int>(项.条件核验->状态) : 0)
                            << L" 完整核验=" << (项.条件核验 && 项.条件核验->候选已完整核验)
                            << L"\n";
                    }
                    const auto 本次诊断 = 文本.str();
                    if (本次诊断 != 上次调度诊断) {
                        (void)记录调试日志(L"TASK_SCHEDULING", L"任务管理线程",
                            L"处理调度扫描结果", 本次诊断);
                        上次调度诊断 = 本次诊断;
                    }
                } catch (...) {} // 独立人读诊断，不改变已确认的工作结果。
            }
#endif
        } catch (const std::bad_alloc&) {
            std::lock_guard 锁(互斥);
            下一工作消费时点 = std::chrono::steady_clock::now() + 等待时长(配置.资源重试间隔毫秒);
        } catch (...) { 报告工程停止("调度扫描结果无法读取"); }
    }

    筹办派发状态 尝试派发调度扫描() noexcept {
        try {
            std::unique_lock 锁(互斥);
            if (停止 || 已完成) return 筹办派发状态::已停止;
            if (服务结算来源排空中) return 筹办派发状态::无可派发任务;
            const auto 当前 = std::chrono::steady_clock::now();
            if (std::any_of(活动工作.begin(), 活动工作.end(), [](const auto& 项) {
                    return 项.请求.种类 == 新任务工作种类::读取任务调度材料;
                }) || (!待派调度工作 && 当前 < 下一调度时点
                    && (!调度待触发 || 当前 < 最早事件调度时点)))
                return 筹办派发状态::无可派发任务;
            if (活动工作.size() >= 配置.邮箱容量) return 筹办派发状态::资源暂不可用;
            活动工作.reserve(活动工作.size() + 1);
            if (!待派调度工作) {
                if (!下一工作编号 || 下一工作编号 == (std::numeric_limits<std::uint64_t>::max)())
                    return 筹办派发状态::内部错误;
                新任务工作请求 请求;
                请求.工作编号 = 下一工作编号++;
                请求.种类 = 新任务工作种类::读取任务调度材料;
                待派调度工作 = 请求;
            }
            工作占用 待接纳{*待派调度工作, {}};
            调度待触发 = false; // 只消费提交前事件；解锁后新事件不能被覆盖。
            锁.unlock();
            const auto 接收 = 提交普通工作(待接纳.请求);
            锁.lock();
            if (接收.状态 == 新任务工作接收状态::已接收 || 接收.状态 == 新任务工作接收状态::精确重复) {
                待接纳.结果 = 接收.处理结果;
                待派调度工作.reset();
                return 筹办派发状态::已派发;
            }
            调度待触发 = true;
            if (接收.状态 == 新任务工作接收状态::邮箱已满
                || 接收.状态 == 新任务工作接收状态::资源失败
                || 接收.状态 == 新任务工作接收状态::任务已占用) return 筹办派发状态::资源暂不可用;
            return 接收.状态 == 新任务工作接收状态::已停止接收
                ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
        } catch (const std::bad_alloc&) { return 筹办派发状态::资源暂不可用; }
        catch (...) { return 筹办派发状态::内部错误; }
    }

    void 推进等待复核() noexcept {
        try {
            等待复核记录 记录;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                const auto 位置 = std::find_if(等待复核组.begin(), 等待复核组.end(),
                    [&](const auto& 项) {
                        const auto 任务值 = 项.second.定位.定位.任务节点.值;
                        return !项.second.已终结 && (!项.second.已反馈 || 项.second.决议)
                            && !学习复核占用.contains(任务值) && !任务有执行保护已加锁(任务值);
                    });
                if (位置 == 等待复核组.end()) return;
                记录 = 位置->second;
            }
            const auto 编号 = 记录.定位.复核编号;
            const auto 任务值 = 记录.定位.定位.任务节点.值;
            const auto 终结 = [&](bool 硬错误, [[maybe_unused]] const wchar_t* 出口 = L"", [[maybe_unused]] int 值 = -1) {
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                if (硬错误) try {
                    std::wostringstream 文本;
                    文本 << L"任务=" << 任务值 << L" 出口=" << 出口 << L" 值=" << 值;
                    (void)记录调试日志(L"PARAM_BINDING", L"等待复核", L"硬错误入阻塞", 文本.str());
                } catch (...) {}
#endif
                std::lock_guard 锁(互斥);
                if (硬错误) 阻塞任务.insert(任务值);
                等待复核组.at(编号).已终结 = true;
                等待复核占用.erase(任务值);
            };
            {
                std::lock_guard 锁(互斥);
                if (学习复核占用.contains(任务值) || 任务有执行保护已加锁(任务值)) return;
            }
            if (!记录.决议) {
                等待复核接收函数 接收;
                { std::lock_guard 锁(互斥); 接收 = 自我等待反馈接收; }
                const auto 状态 = 接收 ? 接收(记录.定位) : 新任务等待复核反馈接收状态::尚未配置;
                if (状态 == 新任务等待复核反馈接收状态::已接收
                    || 状态 == 新任务等待复核反馈接收状态::精确重复) {
                    std::lock_guard 锁(互斥);
                    等待复核组.at(编号).已反馈 = true;
                } else if (状态 != 新任务等待复核反馈接收状态::资源失败
                    && 状态 != 新任务等待复核反馈接收状态::邮箱已满
                    && 状态 != 新任务等待复核反馈接收状态::尚未启动) 终结(true, L"自我拒收", static_cast<int>(状态));
                return;
            }
            if (记录.决议->状态 != 新自我等待复核状态::允许重新筹办) {
                if (记录.决议->状态 == 新自我等待复核状态::等待材料
                    || 记录.决议->状态 == 新自我等待复核状态::当前已满足
                    || 记录.决议->状态 == 新自我等待复核状态::需求非活动
                    || 记录.决议->状态 == 新自我等待复核状态::等待同次执行比较) {
                    std::lock_guard 锁(互斥);
                    等待目标复核.insert(任务值);
                }
                终结(记录.决议->状态 == 新自我等待复核状态::需核查, L"自我决议需核查", static_cast<int>(记录.决议->状态));
                return;
            }
            { std::lock_guard 锁(互斥); 等待复核组.at(编号).准备已尝试 = true; }
            const auto 准备 = 任务服务->准备等待任务重新筹办(记录.定位.定位);
            if (准备 == 新任务操作状态::资源失败) return;
            if (准备 != 新任务操作状态::已修改 && 准备 != 新任务操作状态::无变化) {
                终结(准备 != 新任务操作状态::任务状态不允许 && 准备 != 新任务操作状态::任务不存在,
                    L"准备等待任务重新筹办", static_cast<int>(准备));
                return;
            }
            {
                std::lock_guard 锁(互斥);
                最近筹办结果.erase(任务值);
                待反馈任务.erase(任务值);
                最近自我筹办裁决.erase(任务值);
                筹办阻塞任务.erase(任务值);
                等待来源.erase(任务值);
                上次等待材料.erase(任务值);
                等待目标复核.erase(任务值);
                筹办延后重入.erase(任务值);
                调度待触发 = true;
            }
            终结(false);
        } catch (const std::bad_alloc&) {
            // 原定位和已经接受的决定保留，重试不得签新动作。
        } catch (...) { 报告工程停止("等待复核交接内部错误"); }
    }

    筹办派发状态 尝试派发执行工作() noexcept {
        try {
            工作占用 待接纳;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return 筹办派发状态::已停止;
                if (!已有完整调度材料) return 筹办派发状态::无可派发任务;
                活动工作.reserve(活动工作.size() + 1);
                for (const auto& [任务值, 记录] : 最近自我筹办裁决) {
                    if (!记录.已完成接收 || 记录.执行工作已派发 || !记录.执行工作
                        || 任务有执行保护已加锁(任务值)
                        || 学习占用或旧筹办已失效已加锁(记录.裁决.定位)) continue;
                    if (记录.执行工作->种类 != 新任务工作种类::执行本能动作)
                        return 筹办派发状态::内部错误;
                    待接纳.请求 = *记录.执行工作;
                    break;
                }
            }
            if (!待接纳.请求.工作编号) return 筹办派发状态::无可派发任务;
            const auto 接收 = 提交普通工作(待接纳.请求);
            if (接收.状态 == 新任务工作接收状态::已接收
                || 接收.状态 == 新任务工作接收状态::精确重复) {
                std::lock_guard 锁(互斥);
                待接纳.结果 = 接收.处理结果;
                const auto& 请求 = 待接纳.请求;
                调度材料.erase(请求.任务节点.值);
                const auto 位置 = 最近自我筹办裁决.find(请求.任务节点.值);
                if (位置 == 最近自我筹办裁决.end()
                    || !位置->second.执行工作 || *位置->second.执行工作 != 请求)
                    return 筹办派发状态::内部错误;
                位置->second.执行工作已派发 = true;
                最近执行结果.erase(请求.任务节点.值);
                变化.notify_all();
                return 筹办派发状态::已派发;
            }
            if (接收.状态 == 新任务工作接收状态::邮箱已满
                || 接收.状态 == 新任务工作接收状态::任务已占用
                || 接收.状态 == 新任务工作接收状态::资源失败)
                return 筹办派发状态::资源暂不可用;
            return 接收.状态 == 新任务工作接收状态::已停止接收
                ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
        } catch (const std::bad_alloc&) { return 筹办派发状态::资源暂不可用; }
        catch (...) { return 筹办派发状态::内部错误; }
    }

    筹办派发状态 尝试派发筹办工作() noexcept {
        try {
            std::vector<新任务调度定位> 就绪;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return 筹办派发状态::已停止;
                if (!已有完整调度材料) return 筹办派发状态::无可派发任务;
                if (活动工作.size() >= 配置.邮箱容量) return 筹办派发状态::资源暂不可用;
                活动工作.reserve(活动工作.size() + 1);
                for (const auto& [任务值, 项] : 调度材料) {
                    (void)任务值;
                    if (项.状态 == 新任务调度项状态::普通可筹办) 就绪.push_back(项.定位);
                }
            }
            for (const auto 工作种类 : {
                    新任务工作种类::检查条件,
                    新任务工作种类::查找方法}) {
                // 资源失败沿原工作号重入，不伪造新的筹办尝试或丢掉已形成材料。
                std::optional<工作占用> 重试;
                {
                    std::lock_guard 锁(互斥);
                    const auto 当前时点 = std::chrono::steady_clock::now();
                    if (当前时点 >= 下一工作消费时点) {
                        for (const auto& [任务值, 请求] : 筹办延后重入) {
                            if (请求.种类 != 工作种类 || 任务有执行保护已加锁(任务值)
                                || 有服务结算来源待处理已加锁(任务值)
                                || 学习复核占用.contains(任务值) || 等待复核占用.contains(任务值)) continue;
                            重试 = 工作占用{请求, {}};
                            break;
                        }
                    }
                }
                if (重试) {
                    const auto 信息准备 = 任务服务->复核任务需求准备(重试->请求.任务节点);
                    if(信息准备.状态==新任务需求准备状态::来源前提已变化
                        ||信息准备.状态==新任务需求准备状态::资源延后) {
                        std::lock_guard lock(互斥);
                        下一工作消费时点=std::chrono::steady_clock::now()+等待时长(配置.资源重试间隔毫秒);
                        return 筹办派发状态::资源暂不可用;
                    }
                    if (信息准备.状态 == 新任务需求准备状态::待完善
                        || 信息准备.状态 == 新任务需求准备状态::工程故障) continue;
                    if (信息准备.状态 != 新任务需求准备状态::可筹办)
                        throw std::logic_error("planning retry demand readiness unreadable");
                    const auto 接收 = 提交普通工作(重试->请求);
                    std::lock_guard 锁(互斥);
                    if (接收.状态 == 新任务工作接收状态::已接收 || 接收.状态 == 新任务工作接收状态::精确重复) {
                        const auto 任务值 = 重试->请求.任务节点.值;
                        重试->结果 = 接收.处理结果;
                        筹办延后重入.erase(任务值);
                        调度材料.erase(任务值);
                        return 筹办派发状态::已派发;
                    }
                    if (接收.状态 == 新任务工作接收状态::资源失败
                        || 接收.状态 == 新任务工作接收状态::邮箱已满
                        || 接收.状态 == 新任务工作接收状态::任务已占用) return 筹办派发状态::资源暂不可用;
                    return 接收.状态 == 新任务工作接收状态::已停止接收
                        ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
                }
                for (const auto& 定位 : 就绪) {
                    const auto 任务节点 = 定位.任务节点;
                    if (工作种类 == 新任务工作种类::检查条件
                        ? 定位.任务状态 != 新任务状态::待检查条件
                        : (定位.任务状态 != 新任务状态::已建立 && 定位.任务状态 != 新任务状态::待查找方法)) continue;
                    {
                        std::lock_guard 锁(互斥);
                        if (停止 || 已完成 || 贡献占用任务已加锁(任务节点.值) || 阻塞任务.contains(任务节点.值)
                            || 有服务结算来源待处理已加锁(任务节点.值)
                            || 筹办阻塞任务.contains(任务节点.值)
                            || 学习复核占用.contains(任务节点.值)
                            || 等待复核占用.contains(任务节点.值)
                            || 筹办延后重入.contains(任务节点.值)
                            || std::any_of(活动工作.begin(), 活动工作.end(),
                                [&](const auto& 占用) {
                                    return 占用.请求.任务节点 == 任务节点;
                                })) continue;
                    }
                    const auto 信息准备 = 任务服务->复核任务需求准备(任务节点);
                    if(信息准备.状态==新任务需求准备状态::来源前提已变化
                        ||信息准备.状态==新任务需求准备状态::资源延后) {
                        std::lock_guard lock(互斥);
                        下一工作消费时点=std::chrono::steady_clock::now()+等待时长(配置.资源重试间隔毫秒);
                        return 筹办派发状态::资源暂不可用;
                    }
                    if (信息准备.状态 == 新任务需求准备状态::待完善
                        || 信息准备.状态 == 新任务需求准备状态::工程故障) continue;
                    if (信息准备.状态 != 新任务需求准备状态::可筹办)
                        throw std::logic_error("planning demand readiness unreadable");
                    if (工作种类 == 新任务工作种类::查找方法) {
                        const auto 准备 = 任务服务->准备任务查找方法(任务节点);
                        if (准备 != 新任务操作状态::已修改
                            && 准备 != 新任务操作状态::无变化) {
                            if (准备 != 新任务操作状态::任务状态不允许
                                && 准备 != 新任务操作状态::资源失败) {
                                std::lock_guard 锁(互斥);
                                筹办阻塞任务.insert(任务节点.值);
                            }
                            continue;
                        }
                    }

                    工作占用 待接纳;
                    新任务工作请求 请求;
                    {
                        std::lock_guard 锁(互斥);
                        if (停止 || 已完成) return 筹办派发状态::已停止;
                        if (有服务结算来源待处理已加锁(任务节点.值)) continue;
                        if (!下一工作编号
                            || 下一工作编号
                                == (std::numeric_limits<std::uint64_t>::max)()) {
                            阻塞任务.insert(任务节点.值);
                            return 筹办派发状态::内部错误;
                        }
                        if (活动工作.size() >= 配置.邮箱容量) return 筹办派发状态::资源暂不可用;
                        if (待派筹办工作 && (待派筹办工作->任务节点 != 任务节点
                            || 待派筹办工作->种类 != 工作种类)) {
                            // 原未接纳请求所属任务已离开就绪集合，不能堵住其它T。
                            const auto 旧 = std::find_if(就绪.begin(), 就绪.end(), [&](const auto& 项) {
                                return 项.任务节点 == 待派筹办工作->任务节点;
                            });
                            if (旧 != 就绪.end() && !等待复核占用.contains(旧->任务节点.值)
                                && !有服务结算来源待处理已加锁(旧->任务节点.值)
                                && !学习复核占用.contains(旧->任务节点.值)
                                && !筹办阻塞任务.contains(旧->任务节点.值)
                                && !阻塞任务.contains(旧->任务节点.值)) continue;
                            待派筹办工作.reset();
                        }
                        if (!待派筹办工作) {
                            请求.工作编号 = 下一工作编号++;
                            请求.种类 = 工作种类;
                            请求.任务节点 = 任务节点;
                            待派筹办工作 = 请求;
                        } else 请求 = *待派筹办工作;
                        待接纳.请求 = 请求;
                    }
                    const auto 接收 = 提交普通工作(请求);
                    if (接收.状态 == 新任务工作接收状态::已接收
                        || 接收.状态 == 新任务工作接收状态::精确重复) {
                        std::lock_guard 锁(互斥);
                        待接纳.结果 = 接收.处理结果;
                        待派筹办工作.reset();
                        调度材料.erase(任务节点.值);
                        等待来源.erase(任务节点.值);
                        上次等待材料.erase(任务节点.值);
                        最近筹办结果.erase(任务节点.值);
                        待反馈任务.erase(任务节点.值);
                        待结果反馈任务.erase(任务节点.值);
                        最近自我筹办裁决.erase(任务节点.值);
                        变化.notify_all();
                        return 筹办派发状态::已派发;
                    }
                    if (接收.状态 != 新任务工作接收状态::邮箱已满
                        && 接收.状态 != 新任务工作接收状态::任务已占用
                        && 接收.状态 != 新任务工作接收状态::资源失败) {
                        std::lock_guard 锁(互斥);
                        待派筹办工作.reset();
                        if (接收.状态 == 新任务工作接收状态::已停止接收)
                            阻塞任务.insert(任务节点.值);
                        else 筹办阻塞任务.insert(任务节点.值);
                    }
                    if (接收.状态 == 新任务工作接收状态::已停止接收) {
                        return 筹办派发状态::已停止;
                    }
                    if (接收.状态 == 新任务工作接收状态::邮箱已满
                        || 接收.状态 == 新任务工作接收状态::任务已占用
                        || 接收.状态 == 新任务工作接收状态::资源失败) {
                        return 筹办派发状态::资源暂不可用;
                    }
                }
            }
            return 筹办派发状态::无可派发任务;
        } catch (const std::bad_alloc&) {
            return 筹办派发状态::资源暂不可用;
        } catch (...) {
            return 筹办派发状态::内部错误;
        }
    }

    void 处理已完成结果检测(const 工作占用& 占用) noexcept {
        try {
            const auto& 返回 = 占用.结果.get();
            if (返回.原请求 != 占用.请求 || !占用.请求.本能结果检测请求)
                throw std::logic_error("result detection reply identity mismatch");
            const auto 定位 = *占用.请求.本能结果检测请求;
            const auto t = 定位.任务节点.值;
            const bool 取消 = 返回.状态 == 新任务工作处理状态::停止取消;
            新任务执行处理结果 处理;
            处理.原工作 = 占用.请求;
            if (!取消) {
                if (返回.状态 != 新任务工作处理状态::本能结果检测已返回
                    || !返回.本能结果检测 || 返回.本能结果检测->原请求 != 定位
                    || 返回.本能执行 || 返回.本能结果获取 || 返回.条件子需求提议)
                    throw std::logic_error("result detection reply shape mismatch");
                const auto& 检测 = *返回.本能结果检测;
                const auto& 回读 = 检测.正式读回;
                switch (检测.状态) {
                case 新任务结果检测状态::等待结果材料:
                    if (回读.状态 != 新任务治理执行读回状态::已读回等待观察
                        && 回读.状态 != 新任务治理执行读回状态::已读回等待评估)
                        throw std::logic_error("waiting reply has no pending execution");
                    break;
                case 新任务结果检测状态::已完成影响评估:
                    if (回读.状态 != 新任务治理执行读回状态::已读回影响评估 || !回读.影响评估)
                        throw std::logic_error("completed detection has no assessment");
                    break;
                case 新任务结果检测状态::定位已变化:
                    if (回读.状态 != 新任务治理执行读回状态::执行不在当前位)
                        throw std::logic_error("stale detection readback mismatch");
                    break;
                case 新任务结果检测状态::正式记录不存在:
                    if (回读.状态 != 新任务治理执行读回状态::任务或执行不存在)
                        throw std::logic_error("missing detection readback mismatch");
                    break;
                default: throw std::logic_error("unknown detection status");
                }
                if (检测.状态 == 新任务结果检测状态::等待结果材料
                    || 检测.状态 == 新任务结果检测状态::已完成影响评估) {
                    if (!回读.任务 || !回读.执行
                        || 回读.任务->节点 != 定位.任务节点
                        || 回读.任务->最近执行实例节点 != 定位.执行实例节点
                        || 回读.执行->节点 != 定位.执行实例节点
                        || 回读.执行->任务节点 != 定位.任务节点)
                        throw std::logic_error("detection material belongs to another execution");
                }
                处理.工作结果 = 返回;
                处理.独立读回 = 任务服务->读回任务执行(定位.任务节点, 定位.执行实例节点);
                switch (处理.独立读回->状态) {
                case 新任务治理执行读回状态::已读回等待观察:
                case 新任务治理执行读回状态::已读回等待评估:
                    处理.状态 = 新任务执行处理状态::动作已发出等待结果;
                    break;
                case 新任务治理执行读回状态::已读回影响评估:
                    if (!处理.独立读回->影响评估)
                        throw std::logic_error("independent assessment missing");
                    处理.状态 = 新任务执行处理状态::已完成影响评估;
                    break;
                case 新任务治理执行读回状态::执行不在当前位:
                case 新任务治理执行读回状态::任务或执行不存在:
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                    break;
                default: throw std::logic_error("independent result readback failed");
                }
            }

            std::unique_lock 锁(互斥);
            const auto 匹配等待 = [&]() {
                const auto p = 结果等待任务.find(t);
                return p != 结果等待任务.end()
                    && p->second.执行实例节点 == 定位.执行实例节点
                    && p->second.原检测工作 == 占用.请求 && p->second.已派发;
            };
            const bool 当前交接 = 匹配等待();
            const bool 等待 = !取消 && 处理.状态 == 新任务执行处理状态::动作已发出等待结果;
            const bool 完成 = !取消 && 处理.状态 == 新任务执行处理状态::已完成影响评估;
            // 所有可能分配的交接位置先准备，ack之后只移动/清除现存位置。
            if (当前交接 && (等待 || 完成)) {
                最近执行结果.try_emplace(t);
                if (完成) 待结果反馈任务.insert(t);
            }
            锁.unlock();
            if (工作线程->确认结果已接收(占用.请求.工作编号, 定位.任务节点)
                != 新任务工作操作状态::已完成)
                throw std::logic_error("result detection acknowledgement failed");
            锁.lock();
            std::erase_if(活动工作, [&](const auto& 项) { return 项.请求 == 占用.请求; });
            if (!当前交接 || !匹配等待()) return; // 迟到回执不能清掉新的X。
            auto 位置 = 结果等待任务.find(t);
            if (取消) {
                位置->second.原检测工作.reset();
                位置->second.已派发 = false;
            } else if (等待) {
                auto& 记录 = 位置->second;
                记录.原检测工作.reset();
                记录.已派发 = false;
                记录.待检测 = 记录.在途期间再次唤醒;
                记录.在途期间再次唤醒 = false;
                记录.下次检测时点 = std::chrono::steady_clock::now()
                    + 等待时长(配置.资源重试间隔毫秒);
                最近执行结果.at(t) = std::move(处理);
            } else if (完成) {
                最近执行结果.at(t) = std::move(处理);
                结果等待任务.erase(位置);
            } else {
                结果等待任务.erase(位置);
                const auto 旧 = 最近执行结果.find(t);
                if (旧 != 最近执行结果.end()) {
                    const auto& 旧结果 = 旧->second;
                    const bool 同X = (旧结果.独立读回 && 旧结果.独立读回->执行
                            && 旧结果.独立读回->执行->节点 == 定位.执行实例节点)
                        || (旧结果.原工作.本能结果检测请求
                            && 旧结果.原工作.本能结果检测请求->执行实例节点 == 定位.执行实例节点);
                    if (同X) {
                        待结果反馈任务.erase(t);
                        const auto 后继 = 最近任务后继决议.find(t);
                        if (后继 != 最近任务后继决议.end()
                            && 后继->second.执行实例节点 == 定位.执行实例节点)
                            最近任务后继决议.erase(后继);
                        最近执行结果.erase(旧);
                    }
                }
            }
            变化.notify_all();
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成结果检测", L"结果检测交接、正式读回或确认发生工程错误");
        }
    }

    void 处理已完成执行工作(const 工作占用& 占用) noexcept {
        新任务执行处理结果 处理;
        bool 零动作已回筹办 = false;
        bool 旧执行回执已失效 = false;
        bool 零动作定位失效 = false;
        try {
            处理.原工作 = 占用.请求;
            处理.工作结果 = 占用.结果.get();
            if (处理.工作结果->原请求 != 占用.请求
                || 处理.工作结果->条件子需求提议) {
                处理.状态 = 新任务执行处理状态::内部错误;
            } else if (处理.工作结果->状态 == 新任务工作处理状态::停止取消) {
                处理.状态 = 新任务执行处理状态::停止取消;
            } else if (占用.请求.种类 == 新任务工作种类::执行本能动作
                && 处理.工作结果->本能执行) {
                const auto& 执行结果 = *处理.工作结果->本能执行;
                if (!占用.请求.本能执行请求
                    || 执行结果.原请求 != *占用.请求.本能执行请求
                    || 执行结果.原请求.任务节点 != 占用.请求.任务节点
                    || !执行结果.原请求.完整()
                    || (执行结果.零动作恢复.has_value()
                        != (执行结果.状态 == 新任务治理本能执行状态::零动作已回筹办)))
                    throw std::logic_error("action reply differs from its original request");
                if (执行结果.执行开始
                    && 执行结果.执行开始->执行实例节点) {
                    处理.独立读回 = 任务服务->读回任务执行(
                        占用.请求.任务节点,
                        *执行结果.执行开始->执行实例节点);
                }
                switch (执行结果.状态) {
                case 新任务治理本能执行状态::工程停止:
                    处理.状态 = 新任务执行处理状态::工程停止;
                    break;
                case 新任务治理本能执行状态::动作已发出且结果材料已具备:
                    if (!处理.独立读回
                        || 处理.独立读回->状态
                            != 新任务治理执行读回状态::已读回等待观察
                        || !执行结果.执行开始
                        || !执行结果.执行开始->执行实例节点
                        || !执行结果.方法调用
                        || !执行结果.方法调用->本次条件结果对
                        || 执行结果.方法调用->本次条件结果对->结果.
                            已形成状态节点组.empty()) {
                        处理.状态 = 新任务执行处理状态::独立读回失败;
                        break;
                    }
                    处理.状态 = 新任务执行处理状态::动作已发出等待结果;
                    break;
                case 新任务治理本能执行状态::动作已发出等待结果:
                    处理.状态 = 处理.独立读回
                            && 处理.独立读回->状态
                                == 新任务治理执行读回状态::已读回等待观察
                        ? 新任务执行处理状态::动作已发出等待结果
                        : 新任务执行处理状态::独立读回失败;
                    break;
                case 新任务治理本能执行状态::执行依赖未就绪:
                case 新任务治理本能执行状态::动作未进入:
                    // 没有正式恢复材料的旧拒绝不能解释为可重试的业务条件。
                    处理.状态 = 新任务执行处理状态::工程停止;
                    break;
                case 新任务治理本能执行状态::零动作已回筹办: {
                    const auto& 恢复 = *执行结果.零动作恢复;
                    if (恢复.原最近方法节点.has_value() != 恢复.原最近执行实例节点.has_value()
                        || (恢复.原最近方法节点 && !恢复.原最近方法节点->值)
                        || (恢复.原最近执行实例节点 && !恢复.原最近执行实例节点->值)
                        || 执行结果.动作输出记录状态)
                        throw std::logic_error("invalid zero-action recovery receipt");
                    std::optional<新任务信息> 当前任务;
                    if (恢复.已撤销执行实例节点) {
                        if (!恢复.已撤销执行实例节点->值
                            || 恢复.已撤销执行实例节点 == 恢复.原最近执行实例节点
                            || !执行结果.执行开始
                            || 执行结果.执行开始->状态 != 新任务操作状态::已开始执行
                            || 执行结果.执行开始->执行实例节点 != 恢复.已撤销执行实例节点
                            || (执行结果.未进入动作撤销状态 != 新任务操作状态::已修改
                                && 执行结果.未进入动作撤销状态 != 新任务操作状态::精确重复)
                            || !处理.独立读回
                            || 处理.独立读回->状态 != 新任务治理执行读回状态::任务或执行不存在
                            || 处理.独立读回->执行 || !处理.独立读回->任务)
                            throw std::logic_error("empty execution removal not confirmed");
                        当前任务 = 处理.独立读回->任务;
                        if (执行结果.方法调用) {
                            const auto& 调用 = *执行结果.方法调用;
                            if (调用.状态 == 新方法本能调用状态::已调用 && 调用.本次条件结果对) {
                                const auto& r = 调用.本次条件结果对->结果;
                                if (r.状态 != 新本能函数执行状态::参数不匹配
                                    || r.动作阶段 != 新本能函数动作阶段::未进入动作
                                    || r.结果取得状态 != 新本能函数结果取得状态::未取得
                                    || r.动作动态节点 || !r.已形成状态节点组.empty()
                                    || !r.已形成动态节点组.empty() || !r.已形成二次特征结果组.empty()
                                    || !r.建议观察特征节点组.empty()
                                    || r.服务需求信息包 || r.服务对象观察材料)
                                    throw std::logic_error("zero-action reply contains effects or engineering failure");
                            } else if (调用.状态 != 新方法本能调用状态::方法停用 || 调用.本次条件结果对) {
                                throw std::logic_error("invocation failure cannot prove zero action");
                            }
                        }
                    } else {
                        if ((执行结果.执行开始 && 执行结果.执行开始->执行实例节点)
                            || 执行结果.方法调用 || 执行结果.未进入动作撤销状态)
                            throw std::logic_error("pre-start recovery contains an execution");
                        当前任务 = 任务服务->获取任务(占用.请求.任务节点);
                    }
                    if (!当前任务)
                        throw std::logic_error("recovered task cannot be read");
                    零动作已回筹办 = 当前任务->当前状态 == 新任务状态::待查找方法
                        && 当前任务->最近方法节点 == 恢复.原最近方法节点
                        && 当前任务->最近执行实例节点 == 恢复.原最近执行实例节点;
                    // 正式任务已前进时，只接收旧工作，不能清理新执行的材料。
                    旧执行回执已失效 = !零动作已回筹办;
                    处理.状态 = 零动作已回筹办 ? 新任务执行处理状态::动作未进入
                        : 新任务执行处理状态::任务状态已变化;
                    break;
                }
                case 新任务治理本能执行状态::任务状态已变化:
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                    旧执行回执已失效 = true;
                    break;
                case 新任务治理本能执行状态::定位已变化: {
                    // 这是动作前复核不再成立，不是执行失败，也不能重放旧请求。
                    if ((执行结果.执行开始 && 执行结果.执行开始->执行实例节点)
                        || 执行结果.方法调用)
                        throw std::logic_error("changed pre-action locator contains an action");
                    const auto 当前任务 = 任务服务->获取任务(占用.请求.任务节点);
                    if (!当前任务)
                        throw std::logic_error("pre-action task cannot be read");
                    零动作定位失效 = true;
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                    break;
                }
                default:
                    处理.状态 = 新任务执行处理状态::工作未完成;
                    break;
                }
            } else if (占用.请求.种类 == 新任务工作种类::获取本能结果
                && 占用.请求.本能结果获取请求
                && 处理.工作结果->本能结果获取) {
                const auto& 获取结果 = *处理.工作结果->本能结果获取;
                处理.独立读回 = 任务服务->读回任务执行(
                    占用.请求.任务节点,
                    占用.请求.本能结果获取请求->执行实例节点);
                if (获取结果.状态
                        == 新任务治理结果获取状态::已完成影响评估) {
                    处理.状态 = 处理.独立读回
                            && 处理.独立读回->状态
                                == 新任务治理执行读回状态::已读回影响评估
                        ? 新任务执行处理状态::已完成影响评估
                        : 新任务执行处理状态::独立读回失败;
                } else if (获取结果.状态 == 新任务治理结果获取状态::等待结果材料
                    && 处理.独立读回
                    && (处理.独立读回->状态 == 新任务治理执行读回状态::已读回等待观察
                        || 处理.独立读回->状态 == 新任务治理执行读回状态::已读回等待评估)) {
                    处理.状态 = 新任务执行处理状态::动作已发出等待结果;
                } else if (获取结果.状态
                        == 新任务治理结果获取状态::任务状态已变化) {
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                } else {
                    处理.状态 = 新任务执行处理状态::工作未完成;
                }
            } else {
                处理.状态 = 新任务执行处理状态::内部错误;
            }
        } catch (...) {
            处理.状态 = 新任务执行处理状态::内部错误;
        }

        if (处理.状态 == 新任务执行处理状态::内部错误
            || 处理.状态 == 新任务执行处理状态::独立读回失败
            || 处理.状态 == 新任务执行处理状态::工作未完成
            || 处理.状态 == 新任务执行处理状态::工程停止)
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成执行工作", L"动作或结果工作工程失败，不转为业务等待");
        try {
        bool 是工程停止 = false;
        // 先准备可能分配的交接位置，再确认工作；不能丢掉已发生动作的future。
        try {
            std::lock_guard 锁(互斥);
            const auto t = 占用.请求.任务节点.值;
            if (!旧执行回执已失效) 最近执行结果.try_emplace(t);
            if (处理.状态 == 新任务执行处理状态::动作已发出等待结果) {
                if (!处理.独立读回 || !处理.独立读回->执行)
                    throw std::logic_error("pending action has no execution");
                登记结果等待已加锁(占用.请求.任务节点, 处理.独立读回->执行->节点);
            } else if (处理.状态 == 新任务执行处理状态::已完成影响评估) {
                待结果反馈任务.insert(t);
            }
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成执行工作", L"动作回执交接位置准备失败");
        }
        {
            std::lock_guard 锁(互斥);
            // 结算动作后的工程失败不得静默留在等待，也不得交给自我重筹办。
            // 原始工程停止不能被接收确认失败覆盖成普通非成功状态。
            const bool 是结算任务 = 服务值结算任务
                && *服务值结算任务 == 占用.请求.任务节点;
            if (是工程停止 || (是结算任务
                && (处理.状态 == 新任务执行处理状态::独立读回失败
                    || 处理.状态 == 新任务执行处理状态::工作未完成
                    || 处理.状态 == 新任务执行处理状态::内部错误
                    || 处理.状态 == 新任务执行处理状态::执行依赖未就绪))) {
                处理.状态 = 新任务执行处理状态::工程停止;
            }
            if (零动作定位失效 || 零动作已回筹办) {
                const auto t = 占用.请求.任务节点.值;
                const auto 裁决 = 最近自我筹办裁决.find(t);
                // 仅使本次零动作请求所属的筹办材料失效，不能清掉新裁决。
                if (裁决 != 最近自我筹办裁决.end()
                    && 裁决->second.执行工作 == 占用.请求) {
                    const auto 旧 = 最近筹办结果.find(t);
                    if (旧 != 最近筹办结果.end()
                        && 旧->second.原工作 == 裁决->second.裁决.定位.原工作) {
                        学习失效筹办工作.insert_or_assign(t, 旧->second.原工作.工作编号);
                        if (待派筹办工作 && *待派筹办工作 == 旧->second.原工作)
                            待派筹办工作.reset();
                        待反馈任务.erase(t);
                        最近筹办结果.erase(旧);
                        筹办阻塞任务.erase(t);
                    }
                    最近自我筹办裁决.erase(裁决);
                    零动作定位失效 = true;
                } else {
                    零动作定位失效 = false;
                }
                if (零动作已回筹办 || 零动作定位失效) {
                    for (const auto& w : 活动工作)
                        if (w.请求.种类 == 新任务工作种类::读取任务调度材料)
                            零动作失效调度工作.insert(w.请求.工作编号);
                    调度材料.clear(); 已有完整调度材料 = false;
                    待派调度工作.reset();
                    调度待触发 = true;
                }
            } else if (!旧执行回执已失效
                && 处理.状态 != 新任务执行处理状态::动作已发出等待结果
                && 处理.状态 != 新任务执行处理状态::已完成影响评估
                && 处理.状态 != 新任务执行处理状态::停止取消) {
                阻塞任务.insert(占用.请求.任务节点.值);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                try {
                    std::wostringstream 文本;
                    文本 << L"任务=" << 占用.请求.任务节点.值 << L" 执行处理状态=" << static_cast<int>(处理.状态);
                    (void)记录调试日志(L"PARAM_BINDING", L"执行结果", L"入阻塞", 文本.str());
                } catch (...) {}
#endif
            }
            // 后继工作构造也可能失败；保持同一结算 X，不发布业务反馈。
            if (是结算任务 && 处理.状态 == 新任务执行处理状态::内部错误) {
                处理.状态 = 新任务执行处理状态::工程停止;
            }
            是工程停止 = 处理.状态 == 新任务执行处理状态::工程停止;
            const bool 需要结果反馈 = 处理.状态
                == 新任务执行处理状态::已完成影响评估;
            if (处理.状态 == 新任务执行处理状态::工程停止) {
                内部错误发生 = true;
                停止源.request_stop(); 停止 = true;
            }
            const auto 最近 = 最近执行结果.find(占用.请求.任务节点.值);
            // 工作编号来自同一manager的单调分配器，迟到旧回执不能覆盖新回执。
            if (!旧执行回执已失效 && 最近 != 最近执行结果.end()
                && 最近->second.原工作.工作编号 <= 占用.请求.工作编号)
                最近->second = std::move(处理);
            if (需要结果反馈) {
                待结果反馈任务.insert(占用.请求.任务节点.值);
            }
            变化.notify_all();
        }
        if (是工程停止)
            throw std::logic_error("action handover requires engineering stop");
        if (工作线程->确认结果已接收(占用.请求.工作编号, 占用.请求.任务节点)
            != 新任务工作操作状态::已完成)
            throw std::logic_error("action reply acknowledgement failed");
        {
            std::lock_guard 锁(互斥);
            std::erase_if(活动工作, [&](const auto& 项) { return 项.请求 == 占用.请求; });
            变化.notify_all();
        }
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成执行工作", L"执行工作收束发生工程错误，保留既有事实");
        }
    }

    void 延后工作消费() noexcept {
        try {
            std::lock_guard 锁(互斥);
            下一工作消费时点 = std::chrono::steady_clock::now() + 等待时长(配置.资源重试间隔毫秒);
        } catch (...) { 报告工程停止("筹办结果资源延后失败"); }
    }

    // 来源重筹办不会取消worker。先取得旧future并确认回收，再丢弃其调度作用。
    bool 收回来源已替代工作(const 工作占用& 占用) noexcept {
        try {
            {
                std::lock_guard 锁(互斥);
                const bool 是旧扫描 = 服务结算来源排空中
                    && 占用.请求.种类 == 新任务工作种类::读取任务调度材料;
                const bool 是旧筹办 = 有服务结算来源待处理已加锁(占用.请求.任务节点.值)
                    && (占用.请求.种类 == 新任务工作种类::查找方法
                        || 占用.请求.种类 == 新任务工作种类::检查条件);
                if (!是旧扫描 && !是旧筹办) return false;
            }
            const auto& 返回 = 占用.结果.get();
            const bool 定义前提变化=是定义前提变化完整回执(返回,占用.请求);
            if((返回.状态==新任务工作处理状态::执行组织定义前提已变化
                ||(返回.执行组织交付&&返回.执行组织交付->状态==新任务执行组织状态::定义前提已变化))&&!定义前提变化) {
                报告工程停止("旧来源定义前提变化回执形状不完整");return true;
            }
            const bool 未发布资源失败 = 返回.方法召回 && !返回.方法召回->发布已尝试
                && (返回.方法召回->状态 == 新任务治理方法召回状态::资源失败
                    || (返回.方法召回->方法查询
                        && 返回.方法召回->方法查询->状态 == 新方法操作状态::资源失败));
            const bool 条件资源失败 = 返回.条件筹办
                && 返回.条件筹办->状态 == 新任务治理条件筹办状态::资源失败;
            const bool 缺少工作材料 = 返回.状态 != 新任务工作处理状态::停止取消
                && ((占用.请求.种类 == 新任务工作种类::查找方法 && !返回.方法召回)
                    || (占用.请求.种类 == 新任务工作种类::检查条件 && !返回.条件筹办 && !定义前提变化
                        && !(返回.状态 == 新任务工作处理状态::组织能力暂不支持
                            && 返回.执行组织交付
                            && 返回.执行组织交付->状态 == 新任务执行组织状态::增补结构暂不支持))
                    || (占用.请求.种类 == 新任务工作种类::读取任务调度材料 && !返回.调度材料));
            if (!定义前提变化 && (返回.原请求 != 占用.请求
                || 缺少工作材料
                || 返回.状态 == 新任务工作处理状态::内部错误
                || (返回.状态 == 新任务工作处理状态::需核查 && !未发布资源失败 && !条件资源失败)
                || (返回.状态 == 新任务工作处理状态::查找未完成 && !未发布资源失败)
                || 返回.状态 == 新任务工作处理状态::候选发布未完成
                || 返回.状态 == 新任务工作处理状态::服务未就绪
                || (返回.条件筹办 && 条件筹办有结构错误(*返回.条件筹办))
                || (返回.方法召回 && 返回.方法召回->状态 == 新任务治理方法召回状态::结构不一致)
                || (返回.条件子需求提议 && 返回.条件子需求提议->状态 == 新任务条件子需求提议状态::结构不一致)
                || (返回.调度材料 && 返回.调度材料->状态 != 新任务调度读取状态::已读取
                    && 返回.调度材料->状态 != 新任务调度读取状态::资源失败))) {
                报告工程停止("来源重新筹办排空时发现旧工作工程异常");
                return true;
            }
            // 明确的资源失败没有业务结果；旧请求回收后由新的普通筹办再读，不升级为进程故障。
            const auto 确认 = 工作线程->确认结果已接收(
                占用.请求.工作编号, 占用.请求.任务节点);
            if (确认 != 新任务工作操作状态::已完成) {
                报告工程停止("来源重新筹办未能收回旧工作");
                return true;
            }
            std::lock_guard 锁(互斥);
            std::erase_if(活动工作, [&](const auto& 项) {
                return 项.请求.工作编号 == 占用.请求.工作编号;
            });
            调度待触发 = true;
            return true;
        } catch (...) {
            报告工程停止("来源重新筹办读取旧工作失败");
            return true;
        }
    }

    bool 收回未执行已满足工作(const 工作占用& 占用) {
        const auto& 返回=占用.结果.get();
        if(返回.状态!=新任务工作处理状态::未执行目标已满足)return false;
        if(返回.原请求!=占用.请求||!返回.执行前目标收束
            ||(占用.请求.种类!=新任务工作种类::查找方法&&占用.请求.种类!=新任务工作种类::执行本能动作))
            throw std::logic_error("未执行收束工作回执身份不完整");
        const auto& 原=*返回.执行前目标收束;
        if(原.任务节点!=占用.请求.任务节点||!原.来源需求节点||!原.来源目标||!原.任务读回
            ||原.任务读回->当前状态!=新任务状态::未执行但目标已满足
            ||(原.状态!=新任务执行前目标收束状态::已标记未执行目标已满足
                &&原.状态!=新任务执行前目标收束状态::已处该状态))throw std::logic_error("未执行收束业务材料形状错误");
        const auto 今=任务服务->复核并收束执行前已满足目标(占用.请求.任务节点);
        if(今.状态==新任务执行前目标收束状态::资源失败){延后工作消费();return true;}
        // 目标可在发布点之后变化；只收回原工作，不把旧满足结论重发为完成。
        const auto T=任务服务->获取任务(占用.请求.任务节点);
        if(!T||T->需求节点!=*原.来源需求节点
            ||(今.状态==新任务执行前目标收束状态::已处该状态&&今.来源目标!=原.来源目标))
            throw std::logic_error("未执行收束独立读回任务来源不一致");
        std::lock_guard 锁(互斥);
        const auto p=std::find_if(活动工作.begin(),活动工作.end(),[&](const auto& x){return x.请求==占用.请求;});
        if(p==活动工作.end())throw std::logic_error("未执行收束原工作占用已经替换");
        if(工作线程->确认结果已接收(占用.请求.工作编号,占用.请求.任务节点)!=新任务工作操作状态::已完成)
            throw std::logic_error("未执行收束工作ACK失败");
        活动工作.erase(p);
        const auto t=占用.请求.任务节点.值;
        const auto 旧=最近筹办结果.find(t);
        const bool 同旧控制=旧!=最近筹办结果.end()&&旧->second.原工作==占用.请求;
        const bool 仍已收束=T->当前状态==新任务状态::未执行但目标已满足;
        if(同旧控制&&仍已收束){
            最近筹办结果.erase(旧);待反馈任务.erase(t);筹办阻塞任务.erase(t);
        }
        const auto retry=筹办延后重入.find(t);
        if(retry!=筹办延后重入.end()&&retry->second==占用.请求)筹办延后重入.erase(retry);
        const auto wait=等待来源.find(t);
        if(仍已收束&&wait!=等待来源.end()&&wait->second.原工作==占用.请求)等待来源.erase(wait);
        调度待触发=true;变化.notify_all();return true;
    }

    void 处理已完成工作(const 工作占用& 占用) noexcept {
        if(占用.请求.种类==新任务工作种类::筹办前置语言输入) {
            try {
                const auto r=占用.结果.get();
                if(r.原请求!=占用.请求||!占用.请求.前置语言输入||!r.前置语言输入
                    ||r.前置语言输入->原请求!=*占用.请求.前置语言输入
                    ||r.状态!=新任务工作处理状态::前置语言输入已返回
                    ||!任务服务->是前置语言材料父任务(占用.请求.任务节点))throw std::logic_error("material worker result identity corrupt");
                std::lock_guard 锁(互斥);
                const auto i=std::find_if(活动工作.begin(),活动工作.end(),[&](const auto& w){return w.请求==占用.请求;});
                if(i!=活动工作.end())i->前置已读回=r;
                变化.notify_all();return;
            }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"前置材料工作读回",L"原请求、父域或只读返回结构异常");}
        }
        if (占用.请求.种类 == 新任务工作种类::执行最终服务消费
            || 占用.请求.种类 == 新任务工作种类::执行任务贡献治理)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"普通任务工作回收", L"最终消费必须走专属领取和确认");
        if (收回来源已替代工作(占用)) return;
        try{if(收回未执行已满足工作(占用))return;}
        catch(const std::bad_alloc&){延后工作消费();return;}
        catch(...){报告工程停止("未执行已满足工作收束失败");return;}
        if (占用.请求.种类 == 新任务工作种类::读取任务调度材料) {
            处理调度扫描结果(占用);
            return;
        }
        if (占用.请求.种类 == 新任务工作种类::检测本能结果) {
            处理已完成结果检测(占用);
            return;
        }
        if (占用.请求.种类 == 新任务工作种类::执行本能动作
            || 占用.请求.种类 == 新任务工作种类::获取本能结果) {
            处理已完成执行工作(占用);
            return;
        }
        新任务筹办处理结果 处理;
        处理.原工作 = 占用.请求;
        try {
            处理.工作结果 = 占用.结果.get();
            const auto& 返回 = *处理.工作结果;
            const bool 定义前提变化=是定义前提变化完整回执(返回,占用.请求);
            if((返回.状态==新任务工作处理状态::执行组织定义前提已变化
                ||(返回.执行组织交付&&返回.执行组织交付->状态==新任务执行组织状态::定义前提已变化))&&!定义前提变化) {
                报告工程停止("定义前提变化工作回执形状不完整");return;
            }
            bool 资源重入 = !定义前提变化 && 返回.原请求 == 占用.请求 && (
                (返回.方法召回 && !返回.方法召回->发布已尝试
                    && (返回.方法召回->状态 == 新任务治理方法召回状态::资源失败
                        || (返回.方法召回->方法查询
                            && 返回.方法召回->方法查询->状态 == 新方法操作状态::资源失败)))
                || (返回.条件筹办 && (返回.条件筹办->状态 == 新任务治理条件筹办状态::资源失败
                    || 返回.条件筹办->方法选择状态 == 新任务方法选择状态::资源失败
                    || std::any_of(返回.条件筹办->方法筹办组.begin(), 返回.条件筹办->方法筹办组.end(),
                        [](const auto& 方法) { return 方法.状态 == 新任务候选方法筹办状态::资源失败
                            || std::any_of(方法.条件结果组.begin(), 方法.条件结果组.end(),
                                [](const auto& 配对) { return 配对.状态 == 新任务方法条件项核验状态::资源失败; }); })))
                || (返回.条件子需求提议 && 返回.条件子需求提议->状态 == 新任务条件子需求提议状态::资源失败));
            if (资源重入||定义前提变化) {
                std::lock_guard 锁(互斥);
                if (!停止) 筹办延后重入.insert_or_assign(占用.请求.任务节点.值, 占用.请求);
                const auto 确认 = 工作线程->确认结果已接收(占用.请求.工作编号, 占用.请求.任务节点);
                if (确认 != 新任务工作操作状态::已完成) {
                    阻塞任务.insert(占用.请求.任务节点.值);
                    内部错误发生 = true;
                    停止源.request_stop(); 停止 = true;
                    return;
                }
                std::erase_if(活动工作, [&](const auto& 项) { return 项.请求.工作编号 == 占用.请求.工作编号; });
                下一工作消费时点 = std::chrono::steady_clock::now() + 等待时长(配置.资源重试间隔毫秒);
                调度待触发 = true;
                return;
            }
            if (处理.工作结果->原请求 != 占用.请求) {
                处理.状态 = 新任务筹办处理状态::内部错误;
            } else if (返回.状态 == 新任务工作处理状态::复合目标需自我确认
                || 返回.状态 == 新任务工作处理状态::内容结果定位未支持) {
                if (占用.请求.种类 != 新任务工作种类::查找方法 || !返回.复合目标提议) {
                    处理.状态 = 新任务筹办处理状态::独立读回失败;
                } else {
                    处理.复合目标提议 = 任务服务->读取当前复合目标提议(占用.请求.任务节点).提议;
                    if (处理.复合目标提议->状态 == 新任务复合分解状态::资源失败
                        || 处理.复合目标提议->状态 == 新任务复合分解状态::依赖未就绪) {
                        延后工作消费(); return;
                    }
                    const auto& 当前提议 = *处理.复合目标提议;
                    const auto& 原提议 = *返回.复合目标提议;
                    if (当前提议.任务节点 != 占用.请求.任务节点
                        || 原提议.任务节点 != 占用.请求.任务节点) {
                        处理.状态 = 新任务筹办处理状态::独立读回失败;
                    } else if (当前提议.父需求节点 != 原提议.父需求节点
                        || 当前提议.父目标 != 原提议.父目标) {
                        处理.状态 = 新任务筹办处理状态::任务状态已变化;
                    } else if (返回.状态 == 新任务工作处理状态::复合目标需自我确认
                        && (当前提议.状态 == 新任务复合分解状态::已形成
                            || 当前提议.状态 == 新任务复合分解状态::分支待定)) {
                        处理.状态 = 新任务筹办处理状态::复合目标需自我确认;
                    } else if (返回.状态 == 新任务工作处理状态::内容结果定位未支持
                        && 当前提议.状态 == 新任务复合分解状态::叶子目标) {
                        处理.状态 = 新任务筹办处理状态::内容结果定位未支持;
                    } else {
                        处理.状态 = 新任务筹办处理状态::任务状态已变化;
                    }
                    if (处理.状态 == 新任务筹办处理状态::内容结果定位未支持) {
                        if (!返回.方法召回 || !返回.方法召回->内容学习发现) {
                            处理.状态 = 新任务筹办处理状态::独立读回失败;
                        } else {
                            const auto 当前 = 任务服务->查询内容叶方法学习发现(占用.请求.任务节点);
                            if (当前.状态 == 新任务内容学习发现状态::资源失败
                                || 当前.状态 == 新任务内容学习发现状态::查询未完成) {
                                延后工作消费(); return;
                            }
                            const auto& 原发现 = *返回.方法召回->内容学习发现;
                            if (当前.状态 != 新任务内容学习发现状态::已形成 || !当前.发现
                                || *当前.发现 != 原发现
                                || 原发现.任务节点 != 占用.请求.任务节点
                                || 原发现.来源需求节点 != 处理.复合目标提议->父需求节点
                                || 原发现.来源目标 != 处理.复合目标提议->父目标)
                                处理.状态 = 新任务筹办处理状态::任务状态已变化;
                        }
                    }
                }
            } else if (返回.状态 == 新任务工作处理状态::组织能力暂不支持) {
                处理.状态 = 占用.请求.种类 == 新任务工作种类::检查条件
                    && 返回.执行组织交付
                    && 返回.执行组织交付->状态 == 新任务执行组织状态::增补结构暂不支持
                    && !返回.条件筹办 && !返回.条件子需求提议
                    ? 新任务筹办处理状态::组织能力暂不支持
                    : 新任务筹办处理状态::独立读回失败;
            } else if (占用.请求.种类 != 新任务工作种类::检查条件
                && 处理.工作结果->条件子需求提议) {
                处理.状态 = 新任务筹办处理状态::独立读回失败;
            } else if (占用.请求.种类 == 新任务工作种类::检查条件
                && 处理.工作结果->状态
                    == 新任务工作处理状态::已完成条件筹办) {
                处理.条件筹办 = 处理.工作结果->条件筹办;
                if (处理.条件筹办 && 处理.条件筹办->任务节点 != 占用.请求.任务节点) {
                    处理.状态 = 新任务筹办处理状态::独立读回失败;
                } else if (!处理.条件筹办
                    || 条件筹办有结构错误(*处理.条件筹办)
                    || 处理.条件筹办->状态
                        != 新任务治理条件筹办状态::已完成
                    || (!处理.条件筹办->候选已完整核验
                        && !条件筹办为业务等待(*处理.条件筹办))) {
                    处理.状态 = 新任务筹办处理状态::独立读回失败;
                } else {
                    处理.独立条件筹办 = 任务服务->筹办任务条件和参数(
                        占用.请求.任务节点);
                    if (处理.独立条件筹办->状态 == 新任务治理条件筹办状态::资源失败) {
                        延后工作消费();
                        return;
                    }
                    处理.状态 = 条件筹办有结构错误(*处理.独立条件筹办)
                        || 处理.独立条件筹办->任务节点 != 占用.请求.任务节点
                        ? 新任务筹办处理状态::独立读回失败
                        : 处理.独立条件筹办->状态
                            == 新任务治理条件筹办状态::任务状态已变化
                        ? 新任务筹办处理状态::任务状态已变化
                        : (处理.独立条件筹办->状态
                                == 新任务治理条件筹办状态::已完成
                            && (处理.独立条件筹办->候选已完整核验
                                || 条件筹办为业务等待(*处理.独立条件筹办))
                            ? 新任务筹办处理状态::条件筹办已完成
                            : (处理.独立条件筹办->状态 == 新任务治理条件筹办状态::已完成
                                ? 新任务筹办处理状态::任务状态已变化
                                : 新任务筹办处理状态::独立读回失败));
                    if (处理.状态 == 新任务筹办处理状态::条件筹办已完成) {
                        处理.独立子需求提议 = 任务服务->提议任务条件子需求(
                            占用.请求.任务节点);
                        if (处理.独立子需求提议->状态 == 新任务条件子需求提议状态::资源失败) {
                            延后工作消费();
                            return;
                        }
                        if (!处理.工作结果->条件子需求提议
                            || 处理.工作结果->条件子需求提议->任务节点 != 占用.请求.任务节点
                            || 处理.工作结果->条件子需求提议->状态 == 新任务条件子需求提议状态::结构不一致
                            || 处理.独立子需求提议->任务节点 != 占用.请求.任务节点
                            || 处理.独立子需求提议->状态 == 新任务条件子需求提议状态::结构不一致) {
                            处理.状态 = 新任务筹办处理状态::独立读回失败;
                        } else if (处理.独立子需求提议->状态 == 新任务条件子需求提议状态::任务状态已变化
                            || 处理.独立子需求提议->状态 == 新任务条件子需求提议状态::来源已变化) {
                            处理.状态 = 新任务筹办处理状态::任务状态已变化;
                        } else {
                            const auto& 当前提议 = *处理.独立子需求提议;
                            const auto& 原提议 = *处理.工作结果->条件子需求提议;
                            // 尚未交给self的提议按当前缺项推进，只固定已经明确的父目标。
                            if (有效(原提议.父需求节点) && 有效(当前提议.父需求节点)
                                && (当前提议.父需求节点 != 原提议.父需求节点
                                    || 当前提议.父需求目标存在节点 != 原提议.父需求目标存在节点
                                    || 当前提议.父需求目标 != 原提议.父需求目标))
                                处理.状态 = 新任务筹办处理状态::任务状态已变化;
                        }
                    }
                }
            } else if (处理.工作结果->状态
                    == 新任务工作处理状态::已发布有候选
                || 处理.工作结果->状态
                    == 新任务工作处理状态::已发布完整无候选) {
                if (!处理.工作结果->方法召回
                    || !处理.工作结果->方法召回->候选材料节点) {
                    处理.状态 = 新任务筹办处理状态::独立读回失败;
                } else {
                    处理.独立消费 = 任务服务->消费任务方法候选(
                        占用.请求.任务节点,
                        *处理.工作结果->方法召回->候选材料节点);
                    if (处理.独立消费->状态 == 新任务治理候选消费状态::资源失败
                        || 处理.独立消费->读回.状态 == 新任务治理候选读回状态::资源失败) {
                        延后工作消费();
                        return;
                    }
                    switch (处理.独立消费->状态) {
                    case 新任务治理候选消费状态::已进入条件检查:
                        处理.状态 = 新任务筹办处理状态::有候选并进入条件检查;
                        break;
                    case 新任务治理候选消费状态::已进入等待条件:
                        处理.状态 = 新任务筹办处理状态::完整无候选并进入等待;
                        break;
                    case 新任务治理候选消费状态::任务状态已变化:
                        处理.状态 = 新任务筹办处理状态::任务状态已变化;
                        break;
                    case 新任务治理候选消费状态::读回失败:
                    case 新任务治理候选消费状态::尚未初始化:
                        处理.状态 = 新任务筹办处理状态::独立读回失败;
                        break;
                    case 新任务治理候选消费状态::状态写入失败:
                    case 新任务治理候选消费状态::资源失败:
                        处理.状态 = 新任务筹办处理状态::状态推进失败;
                        break;
                    }
                }
            } else if (处理.工作结果->状态
                == 新任务工作处理状态::任务状态已变化) {
                处理.状态 = 新任务筹办处理状态::任务状态已变化;
            } else if (处理.工作结果->状态
                == 新任务工作处理状态::停止取消) {
                处理.状态 = 新任务筹办处理状态::停止取消;
            } else {
                处理.状态 = 新任务筹办处理状态::工作未完成;
            }
        } catch (const std::bad_alloc&) {
            延后工作消费();
            return;
        } catch (...) {
            处理.状态 = 新任务筹办处理状态::内部错误;
        }

        try {
        if (处理.状态 == 新任务筹办处理状态::完整无候选并进入等待
            || (处理.状态 == 新任务筹办处理状态::条件筹办已完成
                && 处理.独立条件筹办
                && 处理.独立条件筹办->方法选择状态 == 新任务方法选择状态::无适用结果)) {
            // 方法优先；只有正式直接查找无适用结果后，才进入因果路径。
            处理.因果子需求提议 = 任务服务->提议任务因果子需求(
                占用.请求.任务节点, 读取因果等待边内部());
            if(处理.因果子需求提议->状态==新任务因果子需求提议状态::资源延后) {
                延后工作消费();return;
            }
        }
        const auto 等待任务 = 处理.状态 == 新任务筹办处理状态::完整无候选并进入等待
            ? 任务服务->获取任务(占用.请求.任务节点) : std::optional<新任务信息>{};
        if (处理.状态 == 新任务筹办处理状态::完整无候选并进入等待 && !等待任务) {
            延后工作消费();
            return;
        }
        const bool 旧筹办已变化 = 处理.状态 == 新任务筹办处理状态::任务状态已变化;
        const bool 需要反馈 = 处理.状态 != 新任务筹办处理状态::有候选并进入条件检查
            && 处理.状态 != 新任务筹办处理状态::停止取消 && !旧筹办已变化;
        const bool 业务等待 = 处理.状态 == 新任务筹办处理状态::工作未完成
            && 处理.条件筹办 && 条件筹办为业务等待(*处理.条件筹办);
        const bool 技术失败 = (处理.状态 == 新任务筹办处理状态::工作未完成 && !业务等待)
            || 处理.状态 == 新任务筹办处理状态::独立读回失败
            || 处理.状态 == 新任务筹办处理状态::状态推进失败
            || 处理.状态 == 新任务筹办处理状态::内部错误;
        // 所有可分配的记录在ack之前预留，已接受工作不能因分配失败丢失future。
        {
            std::lock_guard 锁(互斥);
            最近筹办结果.try_emplace(占用.请求.任务节点.值);
            if (需要反馈) 待反馈任务.insert(占用.请求.任务节点.值);
            if (技术失败) 阻塞任务.insert(占用.请求.任务节点.值);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            if (技术失败) try {
                std::wostringstream 文本;
                文本 << L"任务=" << 占用.请求.任务节点.值 << L" 工作种类=" << static_cast<int>(占用.请求.种类)
                     << L" 处理状态=" << static_cast<int>(处理.状态)
                     << L" 条件筹办=" << (处理.条件筹办 ? static_cast<int>(处理.条件筹办->状态) : -1)
                     << L" 方法选择=" << (处理.条件筹办 && 处理.条件筹办->方法选择状态 ? static_cast<int>(*处理.条件筹办->方法选择状态) : -1)
                     << L" 已完整核验=" << (处理.条件筹办 ? 处理.条件筹办->候选已完整核验 : false);
                (void)记录调试日志(L"PARAM_BINDING", L"筹办结果", L"技术失败入阻塞", 文本.str());
            } catch (...) {}
#endif
            if ((占用.请求.种类 == 新任务工作种类::检查条件
                    || 处理.复合目标提议) && !旧筹办已变化)
                筹办阻塞任务.insert(占用.请求.任务节点.值);
            if (等待任务) 登记等待来源已加锁(*等待任务, 新任务等待原因::尚无方法, 占用.请求);
        }
        const auto 确认 = 工作线程->确认结果已接收(
            占用.请求.工作编号, 占用.请求.任务节点);
        std::lock_guard 锁(互斥);
        const auto 位置 = std::find_if(活动工作.begin(), 活动工作.end(),
            [&](const auto& 当前) {
                return 当前.请求.工作编号 == 占用.请求.工作编号;
            });
        if (确认 != 新任务工作操作状态::已完成) {
            处理.状态 = 新任务筹办处理状态::内部错误;
            内部错误发生 = true;
            停止源.request_stop(); 停止 = true;
            return;
        }
        if (位置 != 活动工作.end()) 活动工作.erase(位置);
        最近筹办结果[占用.请求.任务节点.值] = std::move(处理);
        if (!需要反馈) 待反馈任务.erase(占用.请求.任务节点.值);
        调度待触发 = true;
        变化.notify_all();
        } catch (const std::bad_alloc&) { 延后工作消费(); }
        catch (...) { 报告工程停止("筹办工作结果交接失败"); }
    }

    void 尝试复核因果等待() noexcept {
        try {
            const auto 停止令牌=停止源.get_token();
            检查因果提交停止(停止令牌);
            std::vector<std::pair<std::uint64_t, 因果等待记录>> 批次;
            {
                auto 锁=获取因果提交锁(互斥,停止令牌);
                if (停止 || 已完成 || std::chrono::steady_clock::now() < 下一因果复核时点) return;
                下一因果复核时点 = std::chrono::steady_clock::now() + 等待时长(配置.调度复核间隔毫秒);
                for (const auto& [任务值,原记录] : 因果等待组) {
                    检查因果提交停止(停止令牌);
                    因果等待记录 复制;
                    复制.定位=原记录.定位;复制.回执=原记录.回执;复制.依据=原记录.依据;
                    复制.子需求已承接=原记录.子需求已承接;复制.来源已变化=原记录.来源已变化;
                    批次.emplace_back(任务值,std::move(复制));
                    检查因果提交停止(停止令牌);
                }
            }
            for (auto& [任务值, 记录] : 批次) {
                检查因果提交停止(停止令牌);
                // 调用者持manager锁；不跨owner。真实返回只能接回原始同一等待记录。
                const auto 定位原记录已加锁=[&]() -> 因果等待记录& {
                    const auto 位置=因果等待组.find(任务值);
                    if(位置==因果等待组.end()||任务值!=记录.定位.任务节点.值
                        ||位置->second.定位!=记录.定位||位置->second.回执!=记录.回执
                        ||位置->second.依据!=记录.依据
                        ||位置->second.子需求已承接.size()!=记录.依据.子需求节点组.size()
                        ||位置->second.子需求承接进度.size()!=记录.依据.子需求节点组.size())
                        throw std::logic_error("因果承接返回的等待记录身份已改变");
                    return 位置->second;
                };
                {
                    // 已发布D不能因自身被扫描建立T而跳过交接；合法变化由任务owner核验。
                    // 真正来源过期时仍保留已发布身份，随后转入原任务重新筹办。
                    for (std::size_t i = 0; i < 记录.依据.子需求节点组.size(); ++i) {
                        检查因果提交停止(停止令牌);
                        if (记录.子需求已承接[i]) continue;
                        if (记录.来源已变化 && 记录.依据.来源前端
                            && std::any_of(记录.回执.项组.begin(), 记录.回执.项组.end(), [&](const auto& 项) {
                                检查因果提交停止(停止令牌);
                                return 项.来源前驱模板 && 项.需求节点 == 记录.依据.子需求节点组[i];
                            }))
                            continue; // 过期来源只保存真实G/R，不能据此许可新T。
                        { auto 锁=获取因果提交锁(互斥,停止令牌); if (停止 || 已完成) return; }
                        auto 候选回执=std::make_shared<新任务治理承接结果>();
                        检查因果提交停止(停止令牌);
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                        记录管理因果阶段(L"已确认因果子需求承接前", 记录.定位.任务节点, 记录.依据.子需求节点组[i]);
#endif
                        auto 承接读取 = 任务服务->承接已确认因果子需求(
                            记录.依据.采用依据,记录.依据.子需求节点组[i],停止令牌);
                        if(std::holds_alternative<新任务因果读取取消>(承接读取))return;
                        auto* 正常承接=std::get_if<新任务治理承接结果>(&承接读取);
                        if(!正常承接)throw std::logic_error("因果承接variant无合法分支");
                        *候选回执=std::move(*正常承接);
                        因果承接回执指针 本次回执=std::move(候选回执);
                        const auto& 承接=*本次回执;
                        const bool 承接成功=承接.状态==新任务治理承接状态::已读回
                            ||承接.状态==新任务治理承接状态::已重新启用
                            ||承接.状态==新任务治理承接状态::既有终结任务
                            ||承接.状态==新任务治理承接状态::需求当前已满足;
                        {
                            // 此保存区不响应stop、不分配节点、不复制DTO。先保存本次，再核身份，最后增空历史槽。
                            std::lock_guard 锁(互斥);
                            auto& 正式=定位原记录已加锁();
                            if(i>=正式.子需求承接进度.size())throw std::logic_error("因果承接进度索引越界");
                            auto& 进度=正式.子需求承接进度[i];进度.最近本次回执=本次回执;
                            const auto D=记录.依据.子需求节点组[i];
                            if(承接.需求节点!=D)throw std::logic_error("因果承接回执需求身份不一致");
                            if(进度.已证任务与虚拟存在.has_value()!=static_cast<bool>(进度.身份依据回执)
                                ||(!进度.已证任务与虚拟存在&&(进度.任务副本依据回执||进度.虚拟存在副本依据回执
                                    ||进度.激活尝试依据回执||进度.激活生效依据回执)))
                                throw std::logic_error("因果承接历史身份与依据不闭合");
                            因果承接身份 本次身份;
                            const auto 核入本次身份=[&](稳定编码 T,稳定编码 Vt) {
                                if(!有效(T)||!有效(Vt)||(本次身份&&*本次身份!=std::pair{T,Vt}))
                                    throw std::logic_error("因果承接回执内部任务身份冲突");
                                本次身份=std::pair{T,Vt};
                            };
                            if(承接.建立结果&&(承接.建立结果->状态==新任务操作状态::已建立
                                ||承接.建立结果->状态==新任务操作状态::已复用)) {
                                if(!承接.建立结果->任务节点||!承接.建立结果->任务虚拟存在节点)
                                    throw std::logic_error("因果承接成功建立缺少任务身份");
                                核入本次身份(*承接.建立结果->任务节点,*承接.建立结果->任务虚拟存在节点);
                            }
                            if(承接.任务) {
                                if(承接.任务->需求节点!=D)throw std::logic_error("因果承接任务副本需求不一致");
                                核入本次身份(承接.任务->节点,承接.任务->任务虚拟存在节点);
                            }
                            if(进度.已证任务与虚拟存在&&本次身份&&*进度.已证任务与虚拟存在!=*本次身份)
                                throw std::logic_error("因果承接跨次任务身份冲突");
                            const auto 锚定=进度.已证任务与虚拟存在 ? 进度.已证任务与虚拟存在 : 本次身份;
                            if(承接.虚拟存在&&锚定&&(承接.虚拟存在->节点!=锚定->second
                                ||承接.虚拟存在->任务节点!=锚定->first))
                                throw std::logic_error("因果承接虚拟存在身份冲突");
                            if(承接成功&&(!承接.任务||!承接.虚拟存在||!锚定))
                                throw std::logic_error("因果承接成功缺少完整任务读回");
                            switch(承接.状态) {
                            case 新任务治理承接状态::已读回:case 新任务治理承接状态::已重新启用:
                            case 新任务治理承接状态::既有终结任务:case 新任务治理承接状态::需求当前已满足:
                            case 新任务治理承接状态::尚未初始化:case 新任务治理承接状态::需求不存在:
                            case 新任务治理承接状态::任务操作未完成:case 新任务治理承接状态::读回不一致:
                            case 新任务治理承接状态::资源失败:break;
                            default:throw std::logic_error("因果子需求承接状态未知");
                            }
                            if(承接.写入已尝试&&!进度.写入尝试依据回执)进度.写入尝试依据回执=本次回执;
                            if(!进度.已证任务与虚拟存在&&本次身份) {
                                进度.已证任务与虚拟存在=本次身份;进度.身份依据回执=本次回执;
                            }
                            if(锚定&&承接.任务&&!进度.任务副本依据回执)进度.任务副本依据回执=本次回执;
                            if(锚定&&承接.虚拟存在&&!进度.虚拟存在副本依据回执)进度.虚拟存在副本依据回执=本次回执;
                            if(锚定&&承接.任务&&承接.重新启用结果) {
                                if(!进度.激活尝试依据回执)进度.激活尝试依据回执=本次回执;
                                if((*承接.重新启用结果==新任务操作状态::已修改||*承接.重新启用结果==新任务操作状态::无变化)
                                    &&!进度.激活生效依据回执)进度.激活生效依据回执=本次回执;
                            }
                            if(承接成功) {
                                正式.子需求已承接[i]=true;记录.子需求已承接[i]=true;
                                调度待触发=true;变化.notify_all();
                            }
                        }
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                        记录管理因果阶段(L"已确认因果子需求承接后", 记录.定位.任务节点, 记录.依据.子需求节点组[i],
                            因果承接诊断状态名(承接.状态), 承接.任务 ? std::optional{承接.任务->节点} : std::nullopt);
#endif
                        if (承接.状态 == 新任务治理承接状态::读回不一致
                            || 承接.状态 == 新任务治理承接状态::需求不存在)
                            throw std::logic_error("因果子需求承接正式身份不一致");
                        检查因果提交停止(停止令牌);
                        if(!承接成功)continue;
                    }
                    if (!记录.来源已变化
                        && !std::all_of(记录.子需求已承接.begin(), 记录.子需求已承接.end(), [&](bool x) {
                            检查因果提交停止(停止令牌);return x;
                        }))
                        continue;
                }
                // 包含零子D等待；每次由正式事实判断，不依赖方法选择或单条唤醒消息。
                检查因果提交停止(停止令牌);
                const auto 边 = 读取因果等待边内部(nullptr,停止令牌);
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                记录管理因果阶段(L"因果等待复核前", 记录.定位.任务节点);
#endif
                auto 复核读取=任务服务->复核任务因果等待(记录.依据,边,停止令牌);
                if(std::holds_alternative<新任务因果读取取消>(复核读取))return;
                const auto* 正常复核=std::get_if<新任务因果复核结果>(&复核读取);
                if(!正常复核)throw std::logic_error("因果等待复核variant无合法分支");
                const auto& 复核=*正常复核;
                if(复核.fresh提议&&(复核.fresh提议->状态==新任务因果子需求提议状态::资源延后
                    ||复核.fresh提议->状态==新任务因果子需求提议状态::材料已变化))
                    continue; // 尚未取得本次可用材料，保留原等待和已承接进度。
                switch(复核.状态) {
                case 新任务因果复核状态::仍等条件:case 新任务因果复核状态::条件发生变化需重新筹办:
                case 新任务因果复核状态::原目标当前满足待完成核验:case 新任务因果复核状态::既有入账待收尾:
                case 新任务因果复核状态::原需求失活:break;
                default:throw std::logic_error("因果等待复核状态未知");
                }
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                记录管理因果阶段(L"因果等待复核后", 记录.定位.任务节点);
#endif
                检查因果提交停止(停止令牌);
                if (!记录.来源已变化 && (复核.状态 == 新任务因果复核状态::仍等条件
                        || 复核.状态 == 新任务因果复核状态::既有入账待收尾)) continue;
                const bool 当前满足 = 复核.状态 == 新任务因果复核状态::原目标当前满足待完成核验;
                const bool 已失活 = 复核.状态 == 新任务因果复核状态::原需求失活;
                if (!当前满足 && !已失活) {
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                    记录管理因果阶段(L"重新筹办调度读取前", 记录.定位.任务节点);
#endif
                    检查因果提交停止(停止令牌);
                    const auto 材料 = 任务服务->读取任务调度材料(记录.定位.任务节点);
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                    记录管理因果阶段(L"重新筹办调度读取后", 记录.定位.任务节点);
#endif
                    if (材料.状态 == 新任务调度读取状态::资源失败) {检查因果提交停止(停止令牌);continue;}
                    if (材料.状态 != 新任务调度读取状态::已读取 || 材料.项组.size() != 1)
                        throw std::logic_error("因果等待任务定位不可读");
                    检查因果提交停止(停止令牌);
                    const auto 准备 = 任务服务->准备等待任务重新筹办(材料.项组.front().定位);
                    {
                        std::lock_guard 锁(互斥);auto& 正式=定位原记录已加锁();
                        正式.重新筹办前缀=std::pair{材料.项组.front().定位,准备};
                    }
                    if (准备 == 新任务操作状态::资源失败) {检查因果提交停止(停止令牌);continue;}
                    if (准备 != 新任务操作状态::已修改 && 准备 != 新任务操作状态::无变化
                        && 准备 != 新任务操作状态::任务状态不允许)
                        throw std::logic_error("因果等待重新筹办失败");
                    检查因果提交停止(停止令牌);
                }
                std::set<std::uint64_t> 预备目标复核;
                if(当前满足)预备目标复核.insert(任务值);
                auto 锁=获取因果提交锁(互斥,停止令牌);
                const auto 位置 = 因果等待组.find(任务值);
                if (位置 == 因果等待组.end() || 位置->second.定位 != 记录.定位
                    || 位置->second.回执 != 记录.回执 || 任务有执行保护已加锁(任务值)) continue;
                const auto 筹办 = 最近筹办结果.find(任务值);
                检查因果提交停止(停止令牌);
                if (筹办 != 最近筹办结果.end() && 筹办->second.原工作 == 记录.定位.原工作) {
                    if(当前满足&&!等待目标复核.contains(任务值))等待目标复核.insert(预备目标复核.extract(任务值));
                    最近筹办结果.erase(筹办); 待反馈任务.erase(任务值);
                    最近自我筹办裁决.erase(任务值); 筹办阻塞任务.erase(任务值);
                }
                // 这里只退出等待/再次筹办；无X/A时不造完成结算或贡献。
                因果等待组.erase(位置);
                调度待触发 = true; 变化.notify_all();
            }
        } catch (const 新任务因果读取取消&) {
            // 仅取消本次后台工作；正式等待、已移交回执和历史前缀保持。
        } catch (const std::bad_alloc&) {
            // 尚未接收/承接的进度仍在等待组，下一真实管理周期继续。
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复核因果等待",
                L"正式因果条件或任务读取失败；保留已发布需求和任务。");
        }
    }

    新任务完成结算确认状态 确认任务贡献复核内部(
        const 新任务贡献反馈定位& 定位, const 新任务贡献推进结果& 推进, bool 停止收束) noexcept {
        if (!有效(定位.任务节点) || !有效(定位.执行实例节点) || !有效(定位.影响评估节点))
            return 新任务完成结算确认状态::入口拒绝;
        try {
            const auto 读取 = 任务服务->读取任务完成贡献交接(定位.任务节点);
            if (读取.状态 == 新任务贡献交接读取状态::预期已变化)
                return 新任务完成结算确认状态::资源失败; // 原消息保留，重读，不释放缓存。
            if (读取.状态 != 新任务贡献交接读取状态::已读取 || !读取.交接)
                throw std::logic_error("贡献确认投影读取失败");
            const auto& 交接 = *读取.交接;
            const auto 任务 = 任务服务->获取任务(定位.任务节点);
            const auto 执行 = 任务服务->获取执行实例(定位.执行实例节点);
            const auto 结算 = 任务服务->读取任务完成结算(定位.任务节点);
            if ((结算 ? std::optional{结算->节点} : std::nullopt) != 交接.完成结算节点)
                return 新任务完成结算确认状态::资源失败;
            if (!任务 || !执行 || 执行->任务节点 != 定位.任务节点
                || 执行->影响评估节点 != 定位.影响评估节点)
                throw std::logic_error("贡献确认执行评估身份损坏");
            if ((任务->当前状态 != 新任务状态::已完成 && 任务->当前状态 != 新任务状态::已结算)
                || 任务->最近执行实例节点 != 定位.执行实例节点) {
                std::lock_guard 锁(互斥);
                贡献复核在途.erase(定位.任务节点.值);
                待完成贡献复核.erase(定位.任务节点.值);
                if (auto 再次 = 贡献复核再次唤醒.extract(定位.任务节点.值); !再次.empty()) {
                    if (!停止收束) 贡献复核待重投.insert_or_assign(再次.key(), 再次.mapped());
                    变化.notify_all();
                }
                return 新任务完成结算确认状态::定位已变化;
            }
            using 状态 = 新任务贡献推进状态;
            using 收尾分类 = 新任务直属收尾分类;
            bool 等待 = true;
            bool 相符 = true;
            switch (推进.状态) {
            case 状态::已完成:
            case 状态::精确重复:
                相符 = 交接.完成结算节点 && 交接.服务贡献值
                    && (交接.接收节点 || !交接.直接父需求节点);
                if (交接.接收节点) {
                    const auto c = 交接.直属收尾.分类;
                    相符 = 相符 && ((((c == 收尾分类::普通已收尾 || c == 收尾分类::最终已收尾)
                        && 交接.直属收尾.阶段节点)
                        || c == 收尾分类::历史已收尾) ? 任务->当前状态 == 新任务状态::已结算
                        : c == 收尾分类::最终消费保留 || c == 收尾分类::结算任务排除);
                }
                等待 = false; break;
            case 状态::等待结算配置:
                相符 = 交接.完成结算节点 && 交接.服务贡献值 && 交接.接收节点
                    && 交接.直属收尾.分类 == 收尾分类::等待结算配置;
                break;
            case 状态::无直接父任务:
                相符 = 交接.完成结算节点 && 交接.服务贡献值 && !交接.直接父需求节点;
                等待 = false; break;
            case 状态::等待直接父任务:
                相符 = 交接.完成结算节点 && 交接.服务贡献值
                    && 交接.直接父需求节点 && !交接.直接父任务节点; break;
            case 状态::父完成结算已封口:
                相符 = 交接.完成结算节点 && 交接.服务贡献值
                    && 交接.直接父完成结算节点 && !交接.接收节点; break;
            case 状态::历史结算缺贡献:
                相符 = 交接.完成结算节点 && !交接.服务贡献值; break;
            case 状态::等待直接子结算:
            case 状态::等待目标分类:
                相符 = !交接.完成结算节点; break;
            case 状态::预期已变化: break;
            case 状态::工程故障:
            default: throw std::logic_error("贡献推进报告工程故障");
            }
            if (推进.状态 != 状态::预期已变化
                && (推进.完成结算节点 != 交接.完成结算节点
                    || (推进.接收节点 && 推进.接收节点 != 交接.接收节点)
                    || (推进.直接父任务节点 && 推进.直接父任务节点 != 交接.直接父任务节点)))
                相符 = false;
            std::lock_guard 锁(互斥);
            if (!停止收束 && (停止 || 已完成)) return 新任务完成结算确认状态::已停止接收;
            const auto 值 = 定位.任务节点.值;
            贡献复核在途.erase(值);
            if (auto 再次 = 贡献复核再次唤醒.extract(值); !再次.empty()) {
                if (!停止收束) 贡献复核待重投.insert_or_assign(再次.key(), 再次.mapped());
                变化.notify_all();
            }
            if (const auto 原结果 = 最近执行结果.find(值);
                原结果 != 最近执行结果.end()
                    && 原结果->second.独立读回 && 原结果->second.独立读回->执行
                    && 原结果->second.独立读回->执行->节点 == 定位.执行实例节点)
                待结果反馈任务.erase(值);
            const bool 结算来源不符 = 结算 && (结算->执行实例节点 != 定位.执行实例节点
                || 结算->影响评估节点 != 定位.影响评估节点);
            if (!相符 || 推进.状态 == 状态::预期已变化 || 结算来源不符) {
                待完成贡献复核.insert_or_assign(值, 定位);
                // 技术交接已回收；旧K与当前完成来源不符时只等真实变化，不能自激空转。
                if (!停止收束 && !结算来源不符) 标记贡献复核待触发();
                return 新任务完成结算确认状态::已确认;
            }
            if (等待) 待完成贡献复核.insert_or_assign(值, 定位);
            else 待完成贡献复核.erase(值);
            // 有正式完成身份后可丢弃工程运行缓存。无完成身份的归集等待保留X/A。
            if (交接.完成结算节点) {
                const auto 旧 = 最近执行结果.find(值);
                const auto 决议 = 最近任务后继决议.find(值);
                const auto 筹办 = 最近筹办结果.find(值);
                const auto 裁决 = 最近自我筹办裁决.find(值);
                // 缓存不存在不是清理许可。重启后新W1/W2没有X时，不能被旧贡献回执删除。
                const bool 同轮 = 旧 != 最近执行结果.end()
                    && 旧->second.独立读回 && 旧->second.独立读回->执行
                    && 旧->second.独立读回->执行->节点 == 定位.执行实例节点
                    && 决议 != 最近任务后继决议.end()
                    && 决议->second.执行实例节点 == 定位.执行实例节点
                    && 决议->second.类型 == 新自我任务后继类型::完成
                    && (筹办 == 最近筹办结果.end()
                        || 筹办->second.原工作.工作编号 <= 旧->second.原工作.工作编号)
                    && (裁决 == 最近自我筹办裁决.end()
                        || 裁决->second.裁决.定位.原工作.工作编号 <= 旧->second.原工作.工作编号)
                    && (!待派筹办工作 || 待派筹办工作->任务节点 != 定位.任务节点)
                    && !普通工作提交中.contains(值)
                    && std::none_of(活动工作.begin(), 活动工作.end(), [&](const auto& 工作) {
                        return 工作.请求.任务节点 == 定位.任务节点;
                    });
                if (同轮) {
                    if (const auto 等 = 结果等待任务.find(值);
                        等 != 结果等待任务.end() && !等->second.已派发
                            && 等->second.执行实例节点 == 定位.执行实例节点)
                        结果等待任务.erase(等);
                    阻塞任务.erase(值);
                    筹办阻塞任务.erase(值);
                    最近筹办结果.erase(值);
                    最近自我筹办裁决.erase(值);
                    最近执行结果.erase(值);
                    最近任务后继决议.erase(值);
                }
            }
            if (交接.接收节点 && 已通知贡献接收.insert(*交接.接收节点).second) {
                if (!停止收束) 标记贡献复核待触发();
                if (!停止收束) 触发条件子需求复核已加锁();
            }
            // 旧R本轮才补9→12时，R通知去重不能挡住新状态的条件复核。
            // 完整重复允许幂等唤醒，但不再置贡献全批次触发，避免空转。
            if (!等待 && 交接.直属收尾.分类 == 收尾分类::普通已收尾) {
                if (!停止收束) 触发条件子需求复核已加锁();
                变化.notify_all();
            }
            return 新任务完成结算确认状态::已确认;
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"确认任务贡献复核", L"贡献正式材料或交接一致性检查异常");
        }
    }

    新任务完成结算确认状态 收回贡献工作(std::uint64_t 编号,
        const 新任务贡献反馈定位& 来源, const void* 所有者, bool 停止收束) noexcept {
        try {
            工作占用 占用;
            bool 池已确认 = false;
            {
                std::lock_guard 锁(互斥);
                const auto i = 最终工作组.find(编号);
                if (i == 最终工作组.end() || i->second.工作.请求.贡献来源 != 来源
                    || (!停止收束 && i->second.自我所有者 != 所有者))
                    throw std::logic_error("贡献工作确认身份不一致");
                auto& 项 = i->second;
                if (项.领取 == 最终工作领取::已确认)
                    return 项.贡献确认.value_or(新任务完成结算确认状态::精确重复);
                if (项.领取 != 最终工作领取::自我待取 || 项.提交中 || !项.工作.结果.valid())
                    return 新任务完成结算确认状态::资源失败;
                占用 = 项.工作; 池已确认 = 项.工作池已确认;
                停止收束 = 停止收束 || 停止 || 已完成;
                项.领取 = 停止收束 ? 最终工作领取::停止收束中 : 最终工作领取::自我确认中;
            }
            const auto 延后 = [&]() {
                std::lock_guard 锁(互斥);
                最终工作组.at(编号).领取 = 最终工作领取::自我待取;
                变化.notify_all();
                return 新任务完成结算确认状态::资源失败;
            };
            if (占用.结果.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return 延后();
            const auto& 返回 = 占用.结果.get();
            if (返回.原请求 != 占用.请求 || 返回.方法召回 || 返回.条件筹办 || 返回.条件子需求提议
                || 返回.本能执行 || 返回.本能结果获取 || 返回.调度材料 || 返回.执行组织交付
                || 返回.本能结果检测 || 返回.最终消费)
                throw std::logic_error("贡献工作回执含异义负载");
            const 新任务贡献治理返回材料* 材料 = nullptr;
            if (返回.状态 == 新任务工作处理状态::任务贡献治理已返回 && 返回.贡献治理) {
                const auto& 对 = *返回.贡献治理;
                const auto& r = 对.结果;
                if (!r.任务贡献治理 || r.任务贡献治理->来源 != 来源
                    || r.状态 != 新本能函数执行状态::已按合同返回
                    || r.结果取得状态 != 新本能函数结果取得状态::已取得
                    || r.动作阶段 != (r.任务贡献治理->本次动作组.empty()
                        ? 新本能函数动作阶段::未进入动作 : 新本能函数动作阶段::已按函数合同返回)
                    || !对.条件.参数角色组.empty() || r.动作动态节点 || !r.已形成状态节点组.empty()
                    || !r.已形成动态节点组.empty() || !r.已形成二次特征结果组.empty()
                    || !r.建议观察特征节点组.empty() || r.服务需求信息包 || r.服务对象观察材料 || r.服务值消费节点 || r.已发布需求
                    || (对.条件.任务贡献治理参数 && 对.条件.任务贡献治理参数->完成来源 != 来源)
                    || (!对.条件.任务贡献治理参数 && (!对.条件.治理动作前特征组.empty()
                        || !r.任务贡献治理->本次动作组.empty()
                        || r.任务贡献治理->推进.状态 != 新任务贡献推进状态::预期已变化)))
                    throw std::logic_error("贡献本能材料违反治理隔离合同");
                材料 = &*r.任务贡献治理;
                任务服务->核验任务贡献治理返回材料(*材料);
            } else if (返回.状态 != 新任务工作处理状态::停止取消 || 返回.贡献治理) {
                throw std::logic_error("贡献工作不是正式返回或未开始取消");
            }
            // 尚未开始取消也独立核读已有正式交接，不把已有事实归给本次。
            const auto 正式 = 任务服务->读取任务完成贡献交接(来源.任务节点);
            if (正式.状态 == 新任务贡献交接读取状态::预期已变化) return 延后();
            if (正式.状态 != 新任务贡献交接读取状态::已读取 || !正式.交接)
                throw std::logic_error("贡献工作正式交接不可读");
            if (!池已确认) {
                const auto 接收 = 工作线程->确认结果已接收(编号, 来源.任务节点);
                if (接收 != 新任务工作操作状态::已完成 && 接收 != 新任务工作操作状态::无变化)
                    throw std::logic_error("贡献工作池确认失败");
                std::lock_guard 锁(互斥);
                最终工作组.at(编号).工作池已确认 = true;
                std::erase_if(活动工作, [&](const auto& 项) { return 项.请求.工作编号 == 编号; });
            }
            // 当前T/X/A映射仍保留，阻止其它工作趁pool已释放抢占同T。
            const auto 确认 = 材料 ? 确认任务贡献复核内部(来源, 材料->推进, 停止收束)
                : 新任务完成结算确认状态::已确认;
            if (确认 == 新任务完成结算确认状态::资源失败
                || 确认 == 新任务完成结算确认状态::已停止接收) return 延后();
            if (确认 != 新任务完成结算确认状态::已确认 && 确认 != 新任务完成结算确认状态::精确重复
                && 确认 != 新任务完成结算确认状态::定位已变化)
                throw std::logic_error("贡献工作正式确认失败");
            {
                std::lock_guard 锁(互斥);
                auto& 项 = 最终工作组.at(编号);
                项.贡献确认 = 确认; 项.领取 = 最终工作领取::已确认;
                const auto 当前 = 当前贡献工作.find(贡献键(来源));
                if (当前 != 当前贡献工作.end() && 当前->second == 编号) 当前贡献工作.erase(当前);
                变化.notify_all();
            }
            return 确认;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"收回贡献治理工作", L"贡献工作或正式材料确认失败");
        }
    }

    新任务工作操作状态 收回最终工作(std::uint64_t 编号,
        const 新服务值最终来源定位& 来源, const void* 所有者, bool 停止收束) noexcept {
        try {
            工作占用 占用;
            新服务值结算确认接口* 端口 = nullptr;
            {
                std::lock_guard 锁(互斥);
                const auto i = 最终工作组.find(编号);
                if (i == 最终工作组.end() || i->second.工作.请求.最终来源 != 来源
                    || (!停止收束 && i->second.自我所有者 != 所有者))
                    throw std::logic_error("最终服务工作确认身份不一致");
                auto& 项 = i->second;
                if (项.领取 == 最终工作领取::已确认) return 新任务工作操作状态::无变化;
                if (项.领取 != 最终工作领取::自我待取 || 项.提交中)
                    return 新任务工作操作状态::已在运行;
                if (!项.工作.结果.valid()) return 新任务工作操作状态::尚未启动;
                占用 = 项.工作;
                端口 = 最终消费端口;
                项.领取 = 停止收束 ? 最终工作领取::停止收束中 : 最终工作领取::自我确认中;
            }
            if (!端口) throw std::logic_error("最终服务消费端口提前释放");
            if (占用.结果.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                std::lock_guard 锁(互斥);
                最终工作组.at(编号).领取 = 最终工作领取::自我待取;
                变化.notify_all();
                return 新任务工作操作状态::已在运行;
            }
            const auto& 返回 = 占用.结果.get();
            if (返回.原请求 != 占用.请求 || 返回.方法召回 || 返回.条件筹办 || 返回.本能执行
                || 返回.本能结果获取 || 返回.本能结果检测 || 返回.调度材料
                || 返回.条件子需求提议 || 返回.执行组织交付 || 返回.贡献治理)
                throw std::logic_error("最终消费工作回执含异义材料");
            const auto 正式 = 端口->读取最终来源结果(来源);
            bool 完成 = false;
            if (返回.状态 == 新任务工作处理状态::最终服务消费已返回 && 返回.最终消费) {
                完成 = 核验最终消费正式结果(来源, *返回.最终消费, 正式);
                // 未动作回执不冻结以后资格；正式reader已核无坏半现场。
                // 后续合法进度可改变状态，不能强求与旧否定回执状态相等。
            } else {
                if (正式.来源 != 来源) throw std::logic_error("最终消费独立读回来源不一致");
                if (返回.状态 != 新任务工作处理状态::停止取消 || 返回.最终消费)
                    throw std::logic_error("最终消费工作不是完整结果或未开始取消");
            }
            const bool 技术回执 = 返回.最终消费
                && (返回.最终消费->状态 == 新服务值最终消费状态::资源延后
                    || 返回.最终消费->状态 == 新服务值最终消费状态::已准备待续);
            if (!完成 && !技术回执 && !停止收束
                && 正式.状态 == 新服务值最终消费状态::已入账待收尾)
                throw std::logic_error("未消费回执却发现U已入账而J5未收尾，保留现场");
            {
                std::lock_guard 锁(互斥);
                最终工作组.at(编号).最终交付结果 = 返回.最终消费;
            }
            // ready工作已经交付；ACK只回收transport，不声称消费完成。
            // 尤其停止阶段不能反复等待同一ready资源回执。
            const auto 接收 = 工作线程->确认结果已接收(编号, 来源.来源任务节点);
            if (接收 != 新任务工作操作状态::已完成 && 接收 != 新任务工作操作状态::无变化)
                throw std::logic_error("最终消费工作池确认失败");
            {
                std::lock_guard 锁(互斥);
                auto& 项 = 最终工作组.at(编号);
                项.工作池已确认 = true;
                项.领取 = 最终工作领取::已确认;
                std::erase_if(活动工作, [&](const auto& w) { return w.请求.工作编号 == 编号; });
                if (完成) {
                    待最终贡献消费.erase(来源.完成结算节点);
                    调度待触发 = true;
                    标记贡献复核待触发();
                    触发条件子需求复核已加锁();
                } else {
                    const auto 当前 = 当前最终工作.find(来源.完成结算节点);
                    if (当前 != 当前最终工作.end() && 当前->second == 编号) 当前最终工作.erase(当前);
                }
                变化.notify_all();
            }
            return 新任务工作操作状态::已完成;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"收回最终服务工作", L"最终工作身份、正式读回或交接失败");
        }
    }

    void 处理最终工作通知() noexcept {
        try {
            std::vector<工作占用> 待检测;
            std::vector<工作占用> 待收束;
            std::function<void()> 唤醒;
            {
                std::lock_guard 锁(互斥);
                for (const auto& [编号, 项] : 最终工作组) {
                    (void)编号;
                    if (项.领取 != 最终工作领取::自我待取 || 项.提交中 || !项.工作.结果.valid()) continue;
                    待检测.push_back(项.工作);
                }
            }
            for (const auto& 工作 : 待检测) {
                if (工作.结果.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) continue;
                {
                    std::lock_guard 锁(互斥);
                    auto& 项 = 最终工作组.at(工作.请求.工作编号);
                    if (项.领取 != 最终工作领取::自我待取) continue;
                    if (!自我反馈所有者 || !自我最终工作唤醒) 待收束.push_back(项.工作);
                    else if (!项.已通知) { 项.已通知 = true; 唤醒 = 自我最终工作唤醒; }
                }
            }
            if (唤醒) 唤醒();
            for (const auto& 项 : 待收束) {
                if (项.请求.种类 == 新任务工作种类::执行任务贡献治理)
                    (void)收回贡献工作(项.请求.工作编号, *项.请求.贡献来源, nullptr, true);
                else (void)收回最终工作(项.请求.工作编号, *项.请求.最终来源, nullptr, true);
            }
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"通知最终服务工作", L"完成唤醒或专属收束失败");
        }
    }

    bool 停止工作线程并收束() noexcept {
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录管理停止阶段(L"工作收束入口");
#endif
        停止源.request_stop();
        if (!工作线程) return false;
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录管理停止阶段(L"工作池请求前");
#endif
        const auto 停止结果 = 工作线程->请求停止();
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录管理停止阶段(L"工作池请求已返回_等待前");
#endif
        const auto 等待结果 = 工作线程->等待停止(
            (std::numeric_limits<std::uint64_t>::max)());
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录管理停止阶段(L"工作池等待已返回_提交收口等待前");
#endif
        {
            std::unique_lock 锁(互斥);
            变化.wait(锁, [&] {
                return 普通工作提交中.empty() && std::none_of(最终工作组.begin(), 最终工作组.end(),
                    [](const auto& 项) { return 项.second.提交中; });
            });
        }
        bool 完整 = (停止结果.状态 == 新任务工作操作状态::已完成
                || 停止结果.状态 == 新任务工作操作状态::无变化)
            && (等待结果.状态 == 新任务工作操作状态::已完成
                || 等待结果.状态 == 新任务工作操作状态::无变化);
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录管理停止阶段(L"提交收口已返回_结果排空前");
#endif
        for (;;) {
            工作占用 占用;
            {
                std::lock_guard 锁(互斥);
                if (活动工作.empty()) {
                    // pool已ACK而贡献尚在确认的记录不再在活动表，仍须drain。
                    const auto 剩余 = std::find_if(最终工作组.begin(), 最终工作组.end(), [](const auto& 项) {
                        return 项.second.领取 != 最终工作领取::已确认 && 项.second.工作.结果.valid();
                    });
                    if (剩余 == 最终工作组.end()) break;
                    占用 = 剩余->second.工作;
                } else 占用 = 活动工作.front();
            }
            if (!占用.结果.valid()
                || 占用.结果.wait_for(std::chrono::milliseconds(0))
                    != std::future_status::ready) {
                完整 = false;
                break;
            }
            if (占用.请求.种类 == 新任务工作种类::执行任务贡献治理) {
                const auto 收回 = 收回贡献工作(占用.请求.工作编号, *占用.请求.贡献来源, nullptr, true);
                if (收回 == 新任务完成结算确认状态::资源失败) {
                    std::unique_lock 锁(互斥);
                    // 同源确认或合法读回漂移只重读，不能重新执行工作。
                    变化.wait_for(锁, std::chrono::milliseconds(1));
                    continue;
                }
            } else if (占用.请求.种类 == 新任务工作种类::执行最终服务消费) {
                const auto 收回 = 收回最终工作(占用.请求.工作编号, *占用.请求.最终来源, nullptr, true);
                if (收回 == 新任务工作操作状态::已在运行) {
                    std::unique_lock 锁(互斥);
                    变化.wait(锁, [&] {
                        const auto& 项 = 最终工作组.at(占用.请求.工作编号);
                        return 项.领取 == 最终工作领取::已确认 || 项.领取 == 最终工作领取::自我待取;
                    });
                    continue;
                }
            } else if(占用.请求.种类==新任务工作种类::筹办前置语言输入) {
                处理已完成工作(占用);
                const auto 确认=工作线程->确认结果已接收(占用.请求.工作编号,占用.请求.任务节点);
                if(确认!=新任务工作操作状态::已完成&&确认!=新任务工作操作状态::无变化){完整=false;break;}
                std::lock_guard 锁(互斥);std::erase_if(活动工作,[&](const auto& w){return w.请求==占用.请求;});
            } else {
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                记录管理停止阶段(L"普通已完成工作处理前");
#endif
                处理已完成工作(占用);
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
                记录管理停止阶段(L"普通已完成工作处理后");
#endif
            }
            {
                std::lock_guard 锁(互斥);
                if (std::any_of(活动工作.begin(), 活动工作.end(), [&](const auto& 项) {
                        return 项.请求.工作编号 == 占用.请求.工作编号;
                    })) {
                    完整 = false; // 收束失败保留证据，不在停止中无限重复同一ready项。
                    break;
                }
            }
        }
        {
            std::lock_guard 锁(互斥);
            if (!活动工作.empty()) 完整 = false;
        }
        return 完整;
    }

    // 每次只推进一个事件的一项；所有领域读取和self回调都在manager锁外。
    void 推进学习事件() noexcept {
        std::uint64_t 交接 = 0;
        新学习结束通知 通知;
        try {
            学习任务项 项;
            bool 需展开 = false;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                // manager本线程收束退出通知，不让self并发改写当前学习事件。
                for (const auto 方法 : 待退出适用方法) {
                    for (auto& [编号, 事件] : 学习事件组) {
                        (void)编号;
                        if (事件.通知.方法节点 != 方法 || 事件.已终结) continue;
                        for (auto& 任务项 : 事件.任务组) {
                            学习复核占用.erase(任务项.任务节点.值);
                            if (任务项.复核编号) 学习复核组.at(*任务项.复核编号).已终结 = true;
                            任务项.已终结 = true;
                        }
                        事件.已终结 = true;
                    }
                    调度待触发 = true;
                }
                待退出适用方法.clear();
                if (当前学习交接 && 学习事件组.at(*当前学习交接).已终结)
                    当前学习交接.reset();
                const auto 位置 = 当前学习交接 ? 学习事件组.find(*当前学习交接)
                    : std::find_if(学习事件组.begin(), 学习事件组.end(),
                        [](const auto& 候选) { return !候选.second.已终结; });
                if (位置 == 学习事件组.end()) return;
                当前学习交接 = 位置->first; // 晚到的小编号不能抢占正在推进的批次。
                auto& 事件 = 位置->second;
                if (事件.可处理时点 > std::chrono::steady_clock::now()) return;
                交接 = 位置->first;
                通知 = 事件.通知;
                // 每次尝试先安排退避，异常和等待分支不形成紧循环。
                事件.可处理时点 = std::chrono::steady_clock::now()
                    + 等待时长(配置.资源重试间隔毫秒);
                需展开 = !事件.已展开;
                if (!需展开) {
                    const auto 待办 = std::find_if(事件.任务组.begin(), 事件.任务组.end(),
                        [](const auto& 当前) { return !当前.已终结; });
                    if (待办 == 事件.任务组.end()) {
                        事件.已终结 = true;
                        return;
                    }
                    项 = *待办;
                }
            }
            if (需展开) {
                const auto 方法 = 学习方法读取->获取方法(通知.方法节点);
                const bool 合法 = 方法
                    && !方法->来源方法节点 && !方法->实例根节点
                    && 方法->实现方式 == 新方法实现方式::本能
                    && 方法->本能函数登记名 && !方法->本能函数登记名->empty();
                新任务列表结果 相关;
                if (合法 && !方法->条件结果组.empty())
                    相关 = 任务服务->查询受方法学习影响任务(通知.方法节点);
                else if (合法) 相关.状态 = 新任务操作状态::已找到;
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                {
                    std::wostringstream 文本;
                    文本 << L"方法=" << 通知.方法节点.值 << L" 合法=" << 合法 << L" 配对数="
                        << (方法 ? 方法->条件结果组.size() : 0) << L" 查询状态=" << static_cast<int>(相关.状态) << L" 任务=";
                    for (const auto T : 相关.任务节点组) 文本 << T.值 << L",";
                    (void)记录调试日志(L"PARAM_BINDING", L"学习唤醒", L"受影响任务", 文本.str());
                }
#endif
                if (合法 && (相关.状态 == 新任务操作状态::资源失败||相关.状态==新任务操作状态::预期已变化)) return;
                std::vector<学习任务项> 新任务组;
                std::set<std::uint64_t> 新占用;
                if (合法 && 相关.状态 == 新任务操作状态::已找到) {
                    新任务组.reserve(相关.任务节点组.size());
                    for (const auto 任务 : 相关.任务节点组) {
                        if (新占用.insert(任务.值).second)
                            新任务组.push_back({任务, {}, false});
                    }
                }
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                auto& 事件 = 学习事件组.at(交接);
                if (!合法 || 相关.状态 != 新任务操作状态::已找到) {
                    最近学习运行异常 = 新学习运行异常记录{
                        通知.方法节点, 新学习运行异常原因::结构不一致};
                    事件.已终结 = true;
                    return;
                }
                auto 新失效 = 学习失效筹办工作;
                // 等待复核先完成交接，学习事件保留同一批次延后，不同时占用T。
                if (std::any_of(新任务组.begin(), 新任务组.end(), [&](const auto& 项) {
                        return 等待复核占用.contains(项.任务节点.值)
                            || 有服务结算来源待处理已加锁(项.任务节点.值);
                    })) return;
                for (const auto& 当前 : 新任务组) {
                    const auto 旧 = 最近筹办结果.find(当前.任务节点.值);
                    if (旧 != 最近筹办结果.end())
                        新失效.insert_or_assign(当前.任务节点.值, 旧->second.原工作.工作编号);
                }
                // 整批构造完成才发布；事件串行推进，前一批的占用已全部释放。
                事件.任务组.swap(新任务组);
                学习复核占用.swap(新占用);
                学习失效筹办工作.swap(新失效);
                事件.已展开 = true;
                事件.已终结 = 事件.任务组.empty();
                最近方法学习结束 = 通知;
                return;
            }

            const auto 终结 = [&](bool 异常) {
                std::lock_guard 锁(互斥);
                auto& 事件 = 学习事件组.at(交接);
                for (auto& 当前 : 事件.任务组) {
                    if (当前.任务节点 != 项.任务节点) continue;
                    当前.已终结 = true;
                    if (当前.复核编号) 学习复核组.at(*当前.复核编号).已终结 = true;
                    break;
                }
                学习复核占用.erase(项.任务节点.值);
                if (异常) 最近学习运行异常 = 新学习运行异常记录{
                    通知.方法节点, 新学习运行异常原因::结构不一致};
                事件.已终结 = std::all_of(事件.任务组.begin(), 事件.任务组.end(),
                    [](const auto& 当前) { return 当前.已终结; });
            };
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            const auto 学习诊断 = [&](const wchar_t* 出口) {
                try {
                    std::wostringstream 文本;
                    文本 << L"方法=" << 通知.方法节点.值 << L" 任务=" << 项.任务节点.值 << L" 出口=" << 出口;
                    (void)记录调试日志(L"PARAM_BINDING", L"学习唤醒", L"逐任务复核", 文本.str());
                } catch (...) {}
            };
#else
            const auto 学习诊断 = [](const wchar_t*) {};
#endif
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成
                    || std::any_of(活动工作.begin(), 活动工作.end(), [&](const auto& 工作) {
                        return 工作.请求.任务节点 == 项.任务节点;
                    })) { 学习诊断(L"任务有活动工作"); return; }
                // 活动W1/W2完成后才出现的定位同样失效，不能趁占用释放派旧W3。
                const auto 旧 = 最近筹办结果.find(项.任务节点.值);
                if (旧 != 最近筹办结果.end())
                    学习失效筹办工作.insert_or_assign(
                        项.任务节点.值, 旧->second.原工作.工作编号);
            }
            const auto 任务 = 任务服务->获取任务(项.任务节点);
            if (!任务) { 终结(true); return; }
            新任务学习复核定位 当前定位{0, 交接, 通知.方法节点,
                项.任务节点, 任务->需求节点, 任务->当前状态,
                任务->当前候选方法材料节点, 任务->方法实例节点, 任务->方法执行组织节点};
            if (!学习筹办安全状态(任务->当前状态)) { 学习诊断(L"任务状态不安全"); 终结(false); return; }
            if (!有效(任务->需求节点)) { 终结(true); return; }
            bool 有保护 = false;
            [[maybe_unused]] const wchar_t* 保护来源 = L"";
            {
                std::lock_guard 锁(互斥);
                有保护 = 阻塞任务.contains(项.任务节点.值)
                    || 结果等待任务.contains(项.任务节点.值)
                    || 待结果反馈任务.contains(项.任务节点.值);
                保护来源 = 阻塞任务.contains(项.任务节点.值) ? L"任务受保护(阻塞任务)"
                    : 结果等待任务.contains(项.任务节点.值) ? L"任务受保护(结果等待任务)"
                    : L"任务受保护(待结果反馈任务)";
            }
            if (有保护) { 学习诊断(保护来源); 终结(false); return; }

            学习复核记录 记录;
            if (!项.复核编号) {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                if (!下一学习复核编号
                    || 下一学习复核编号 == (std::numeric_limits<std::uint64_t>::max)()) {
                    最近学习运行异常 = 新学习运行异常记录{
                        通知.方法节点, 新学习运行异常原因::内部错误};
                    // 编号耗尽是硬停止，不换编号空间重试。
                    停止源.request_stop(); 停止 = true;
                    return;
                }
                当前定位.复核编号 = 下一学习复核编号++;
                记录.定位 = 当前定位;
                学习复核组.emplace(当前定位.复核编号, 记录);
                for (auto& 当前 : 学习事件组.at(交接).任务组)
                    if (当前.任务节点 == 项.任务节点) {
                        当前.复核编号 = 当前定位.复核编号;
                        break;
                    }
                项.复核编号 = 当前定位.复核编号;
            } else {
                std::lock_guard 锁(互斥);
                记录 = 学习复核组.at(*项.复核编号);
            }
            当前定位.复核编号 = 记录.定位.复核编号;
            auto 可比较定位 = 当前定位;
            if (记录.准备已尝试 && 当前定位.任务状态 == 新任务状态::待查找方法)
                可比较定位.任务状态 = 记录.定位.任务状态;
            if (可比较定位 != 记录.定位) {
                学习诊断(L"定位已变化");
                const auto 相关 = 任务服务->查询受方法学习影响任务(通知.方法节点);
                if (相关.状态 == 新任务操作状态::资源失败||相关.状态==新任务操作状态::预期已变化) return;
                if (相关.状态 != 新任务操作状态::已找到) { 终结(true); return; }
                if (std::find(相关.任务节点组.begin(), 相关.任务节点组.end(), 项.任务节点)
                    == 相关.任务节点组.end()) { 终结(false); return; }
                std::lock_guard 锁(互斥);
                学习复核组.at(*项.复核编号).已终结 = true;
                for (auto& 当前 : 学习事件组.at(交接).任务组)
                    if (当前.任务节点 == 项.任务节点) 当前.复核编号.reset();
                return; // 保留占用；下一次建立全新的fresh定位，不复用旧许可。
            }
            if (!记录.决议) {
                if (记录.已反馈) return;
                学习复核接收函数 接收;
                {
                    std::lock_guard 锁(互斥);
                    接收 = 自我学习反馈接收;
                }
                const auto 状态 = 接收 ? 接收(记录.定位)
                    : 新任务学习复核反馈接收状态::尚未配置;
                学习诊断(状态 == 新任务学习复核反馈接收状态::已接收 ? L"已发自我复核" : L"自我复核未接收");
                if (状态 == 新任务学习复核反馈接收状态::已接收
                    || 状态 == 新任务学习复核反馈接收状态::精确重复) {
                    std::lock_guard 锁(互斥);
                    学习复核组.at(*项.复核编号).已反馈 = true;
                } else if (状态 != 新任务学习复核反馈接收状态::邮箱已满
                    && 状态 != 新任务学习复核反馈接收状态::尚未启动
                    && 状态 != 新任务学习复核反馈接收状态::资源失败) 终结(true);
                return;
            }
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            {
                std::wostringstream 文本;
                文本 << L"方法=" << 通知.方法节点.值 << L" 任务=" << 项.任务节点.值
                    << L" 自我决议=" << static_cast<int>(记录.决议->状态);
                (void)记录调试日志(L"PARAM_BINDING", L"学习唤醒", L"自我决议", 文本.str());
            }
#endif
            if (记录.决议->状态 != 新自我学习复核状态::允许重新筹办) {
                终结(记录.决议->状态 == 新自我学习复核状态::需核查);
                return;
            }
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                学习复核组.at(*项.复核编号).准备已尝试 = true;
            }
            const auto 准备 = 任务服务->准备学习后重新筹办(项.任务节点, 通知.方法节点);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            {
                std::wostringstream 文本;
                文本 << L"方法=" << 通知.方法节点.值 << L" 任务=" << 项.任务节点.值 << L" 准备=" << static_cast<int>(准备);
                (void)记录调试日志(L"PARAM_BINDING", L"学习唤醒", L"准备重新筹办", 文本.str());
            }
#endif
            if (准备 == 新任务操作状态::资源失败) return;
            if (准备 != 新任务操作状态::已修改 && 准备 != 新任务操作状态::无变化) {
                终结(准备 != 新任务操作状态::任务状态不允许
                    && 准备 != 新任务操作状态::任务不存在);
                return;
            }
            const auto 读回 = 任务服务->获取任务(项.任务节点);
            if (!读回 || 读回->当前状态 != 新任务状态::待查找方法
                || 读回->需求节点 != 记录.定位.需求节点
                || 读回->方法实例节点 != 记录.定位.方法实例节点
                || 读回->方法执行组织节点 != 记录.定位.方法执行组织节点) {
                终结(true);
                return;
            }
            {
                std::lock_guard 锁(互斥);
                最近筹办结果.erase(项.任务节点.值);
                待反馈任务.erase(项.任务节点.值);
                最近自我筹办裁决.erase(项.任务节点.值);
                筹办阻塞任务.erase(项.任务节点.值);
                等待来源.erase(项.任务节点.值);
                等待目标复核.erase(项.任务节点.值);
                筹办延后重入.erase(项.任务节点.值);
                上次等待材料.erase(项.任务节点.值);
                调度待触发 = true;
                // 不清阻塞任务、执行/结果/恢复/结算材料。
            }
            终结(false);
        } catch (const std::bad_alloc&) {
            // 原事件、定位及已接纳决议仍在；下一次按同一交接重入。
        } catch (...) {
            std::lock_guard 锁(互斥);
            最近学习运行异常 = 新学习运行异常记录{
                通知.方法节点, 新学习运行异常原因::内部错误};
            if (交接) {
                auto& 事件 = 学习事件组.at(交接);
                事件.已终结 = true;
                for (const auto& 项 : 事件.任务组) {
                    学习复核占用.erase(项.任务节点.值);
                    if (项.复核编号) 学习复核组.at(*项.复核编号).已终结 = true;
                }
            }
        }
    }

    void 推进服务结算来源事件() noexcept {
        try {
            稳定编码 来源;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成 || 待服务结算来源.empty() || !服务值结算配置) return;
                来源 = 待服务结算来源.front();
            }
            // 新治理不再把完成来源绑定给结算方法，亦不重筹办原首子目标。
            // 此队列仅保留完成来源的进度通知；事实与最终入账由self直接治理。
            const auto 完成 = 任务服务->读取任务完成结算(来源);
            std::lock_guard 锁(互斥);
            if (待服务结算来源.empty() || 待服务结算来源.front() != 来源)
                throw std::logic_error("结算来源队列身份变化");
            if (完成 && 完成->服务贡献) 待最终贡献消费.insert(完成->节点);
            else 标记贡献复核待触发();
            待服务结算来源.erase(待服务结算来源.begin());
            服务结算来源排空中 = false;
            调度待触发 = true;
            变化.notify_all();
        } catch (const std::bad_alloc&) {
            // 先完成待办分配，再移除原消息；资源恢复后重试同一来源。
        } catch (...) { 报告工程停止("结算来源消息串行推进失败"); }
    }
    void 运行() noexcept {
        try {
            std::unique_lock 锁(互斥);
            已进入 = true;
            变化.notify_all();
            while (!停止) {
                // 先回收已就绪回执，避免它们占据邮箱和同T排他位置。
                while (!停止 && std::chrono::steady_clock::now() >= 下一工作消费时点) {
                    const auto 已完成工作 = std::find_if(
                        活动工作.begin(), 活动工作.end(), [](const auto& 占用) {
                            return 占用.请求.种类 != 新任务工作种类::执行最终服务消费
                                && 占用.请求.种类 != 新任务工作种类::执行任务贡献治理 && !占用.前置已读回 && 占用.结果.valid()
                                && 占用.结果.wait_for(std::chrono::milliseconds(0))
                                    == std::future_status::ready;
                        });
                    if (已完成工作 == 活动工作.end()) break;
                    const auto 占用 = *已完成工作;
                    生命周期 = 新任务管理生命周期::处理消息;
                    锁.unlock();
                    处理已完成工作(占用);
                    锁.lock();
                    调度待触发 = true;
                }
                if (停止) continue;

                锁.unlock();
                处理最终工作通知();
                推进服务结算来源事件();
                推进学习事件();
                推进等待复核();
                尝试反馈筹办结果();
                尝试反馈任务结果();
                尝试反馈任务完成贡献();
                尝试复核条件子需求();
                尝试复核因果等待();
                锁.lock();

                if (停止) continue;
                std::shared_ptr<消息项> 项;
                if (!活动.empty()) {
                    const auto 最早 = std::min_element(活动.begin(), 活动.end(),
                        [](const auto& 左, const auto& 右) {
                            return 左->可处理时点 < 右->可处理时点;
                        });
                    if ((*最早)->可处理时点 <= std::chrono::steady_clock::now()) {
                        项 = *最早;
                    }
                }
                if (项) {
                    生命周期 = 新任务管理生命周期::处理消息;
                    锁.unlock();
                    新任务承接处理结果 结果;
                    结果.原消息 = 项->消息;
                    const bool 是扫描补齐 = 项->消息.依据
                        == 新任务承接依据::自我全树扫描;
                    const bool 是条件子需求 = 项->消息.依据
                        == 新任务承接依据::自我条件子需求;
                    const bool 是外部服务要求 = 项->消息.依据
                        == 新任务承接依据::外部服务要求;
                    const bool 是行动请求 = 项->消息.依据==新任务承接依据::行动请求;
                    const bool 自动决议 = 项->消息.依据
                        != 新任务承接依据::显式请求 && !是扫描补齐
                        && !是条件子需求 && !是外部服务要求 && !是行动请求;
                    bool 条件暂缓 = false;
                    if (是条件子需求) {
                        结果.承接 = 新任务治理承接结果{};
                        try {
                            std::vector<新任务已确认条件子需求> 单项组;
                            新任务条件子需求提议 原提议;
                            bool 来源过期 = false;
                            bool 等待凭据交付 = false;
                            {
                                std::lock_guard 定位锁(互斥);
                                if (!停止 && 子需求承接定位有效已加锁(*项)) {
                                    const auto& 记录 = 最近自我筹办裁决.at(
                                        项->父筹办定位->任务节点.值);
                                    const auto 子项 = std::find_if(记录.已确认子需求组.begin(),
                                        记录.已确认子需求组.end(), [&](const auto& 候选) {
                                            return 候选.需求节点 == 项->消息.需求节点;
                                        });
                                    const auto 序号 = static_cast<std::size_t>(
                                        std::distance(记录.已确认子需求组.begin(), 子项));
                                    if (子项 == 记录.已确认子需求组.end()
                                        || 记录.条件准备依据已交付组.size() != 记录.已确认子需求组.size())
                                        throw std::logic_error("条件子需求后台交付进度不闭合");
                                    等待凭据交付 = !记录.条件准备依据已交付组[序号];
                                    if (!等待凭据交付) {
                                        单项组.push_back(*子项);
                                        原提议 = 记录.裁决.子需求回执->原提议;
                                        来源过期 = 记录.条件来源待重筹办;
                                    }
                                }
                            }
                            条件暂缓 = 等待凭据交付;
                            if (!单项组.empty()) {
                                // 历史确认已由静态核心及task凭据保留；这里只核当前必要性。
                                const auto 身份 = 任务服务->复核已发布条件子需求身份(
                                    项->父筹办定位->任务节点, 单项组.front());
                                if (身份 == 新任务已发布条件子需求核验状态::结构不一致)
                                    throw std::logic_error("条件子需求承接身份读取失败");
                                if (身份 == 新任务已发布条件子需求核验状态::资源失败)
                                    结果.承接->状态 = 新任务治理承接状态::资源失败;
                                else if (身份 == 新任务已发布条件子需求核验状态::身份一致
                                    || 身份 == 新任务已发布条件子需求核验状态::材料已变化
                                    || 身份 == 新任务已发布条件子需求核验状态::任务状态已变化) {
                                    来源过期 |= 身份 != 新任务已发布条件子需求核验状态::身份一致;
                                    const auto 当前 = 任务服务->提议任务条件子需求(
                                        项->父筹办定位->任务节点);
                                    if (当前.状态 == 新任务条件子需求提议状态::结构不一致)
                                        throw std::logic_error("条件子需求承接来源读取失败");
                                    const bool 读取资源失败 = 当前.状态 == 新任务条件子需求提议状态::资源失败;
                                    const bool 提议等材料=条件提议等材料(当前);
                                    const bool 同来源=!提议等材料&&条件提议来源一致(当前,原提议);
                                    const auto 支持=同来源
                                        ? 读取条件子项当前支持(当前,原提议,单项组.front().原提议)
                                        : 条件子项当前支持::已变化;
                                    const bool 读取暂缓=提议等材料||(同来源&&支持==条件子项当前支持::等材料);
                                    const bool 本项无需推进=同来源&&支持==条件子项当前支持::仍支持
                                        &&std::find(当前.子需求项组.begin(),当前.子需求项组.end(),单项组.front().原提议)==当前.子需求项组.end();
                                    if (读取资源失败) 结果.承接->状态 = 新任务治理承接状态::资源失败;
                                    else if(读取暂缓)条件暂缓=true;
                                    else 来源过期=来源过期||!同来源||支持==条件子项当前支持::已变化;
                                    bool 定位有效 = false;
                                    {
                                        std::lock_guard 定位锁(互斥);
                                        定位有效 = !停止 && 子需求承接定位有效已加锁(*项);
                                        if (定位有效 && !读取资源失败 && !读取暂缓) {
                                            auto& 记录 = 最近自我筹办裁决.at(
                                                项->父筹办定位->任务节点.值);
                                            定位有效 = 记录.裁决.子需求回执->原提议 == 原提议;
                                            const auto 确认项 = std::find(记录.已确认子需求组.begin(),
                                                记录.已确认子需求组.end(), 单项组.front());
                                            if (确认项 == 记录.已确认子需求组.end()
                                                || 记录.条件准备依据已交付组.size() != 记录.已确认子需求组.size())
                                                定位有效 = false;
                                            else 定位有效 = 定位有效 && 记录.条件准备依据已交付组[
                                                static_cast<std::size_t>(std::distance(记录.已确认子需求组.begin(), 确认项))];
                                            if (定位有效)
                                                记录.条件来源待重筹办 |= 来源过期;
                                        }
                                    }
                                    if (定位有效 && !读取资源失败 && !读取暂缓) {
                                        // 过期或当前无需推进的项只补齐D-T前缀，不重新启用。
                                        结果.承接 = 来源过期||本项无需推进
                                            ? 任务服务->建立需求任务关联(项->消息.需求节点)
                                            : 任务服务->承接已确认条件子需求(
                                                项->父筹办定位->任务节点, 单项组.front());
                                    }
                                } else throw std::logic_error("条件子需求当前必要性状态未知");
                            }
                        } catch (const std::bad_alloc&) {
                            结果.承接->状态 = 新任务治理承接状态::资源失败;
                        } catch (const std::length_error&) {
                            结果.承接->状态 = 新任务治理承接状态::资源失败;
                        } catch (...) {
                            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                                L"条件子需求承接", L"条件子需求承接工程故障");
                        }
                    } else if (是行动请求) {
                        if(!项->消息.行动请求来源)
                            throw std::logic_error("action request task source missing");
                        结果.承接=任务服务->承接行动请求需求(项->消息.需求节点,*项->消息.行动请求来源);
                    } else if (是外部服务要求) {
                        if (!项->消息.外部要求节点)
                            throw std::logic_error("external service request locator missing");
                        结果.承接 = 任务服务->承接外部服务要求(
                            项->消息.需求节点, *项->消息.外部要求节点);
                    } else {
                        结果.承接 = 是扫描补齐
                            ? 任务服务->建立需求任务关联(项->消息.需求节点)
                            : 任务服务->承接需求任务(
                                项->消息.需求节点, 自动决议);
                    }
                    if (条件暂缓) {
                        // 未执行承接写入，不制造失败回执；保留原消息等待原节奏补齐材料。
                        锁.lock();
                        if (!停止) 项->可处理时点 = std::chrono::steady_clock::now()
                            + 等待时长(配置.调度复核间隔毫秒);
                        continue;
                    }
                    switch (结果.承接->状态) {
                    case 新任务治理承接状态::已读回:
                        结果.状态 = 新任务承接处理状态::已承接; break;
                    case 新任务治理承接状态::已重新启用:
                        结果.状态 = 新任务承接处理状态::已重新启用; break;
                    case 新任务治理承接状态::既有终结任务:
                        结果.状态 = 新任务承接处理状态::既有终结; break;
                    case 新任务治理承接状态::需求当前已满足:
                        结果.状态 = 新任务承接处理状态::需求当前已满足; break;
                    case 新任务治理承接状态::需求不存在:
                        结果.状态 = 新任务承接处理状态::入口拒绝; break;
                    case 新任务治理承接状态::尚未初始化:
                        结果.状态 = 新任务承接处理状态::服务未就绪; break;
                    default:
                        结果.状态 = 新任务承接处理状态::需核查; break;
                    }
                    // 合法的被认知需求不是执行需求：终结本次误投，不配任务，
                    // 也不把这个零写拒绝当作资源失败重新派发。
                    if (结果.承接->建立结果
                        && 结果.承接->建立结果->状态 == 新任务操作状态::需求非执行域) {
                        结果.状态 = 新任务承接处理状态::入口拒绝;
                    }
                    // task调用及其递归锁已退出，manager锁尚未重新取得。
                    const bool 扫描资源重试 = 扫描承接可资源重试(结果);
                    if (!扫描资源重试 || !项->扫描资源失败已记录) {
                        记录需求任务承接观察(结果);
                        if (扫描资源重试) 项->扫描资源失败已记录 = true;
                    }
                    锁.lock();
                    // 普通承接只重试零写资源失败；受限条件承接则通过一D一T
                    // 读回收敛。扫描只对具名资源失败fresh重入，保留原消息、
                    // promise和容量；同步bool的未知失败不能走资源重试。
                    const bool 请求技术重试=(是行动请求||是外部服务要求)&&结果.承接
                        &&(结果.承接->状态==新任务治理承接状态::资源失败
                            ||(结果.承接->状态==新任务治理承接状态::任务操作未完成
                                &&结果.承接->建立结果
                                &&(结果.承接->建立结果->状态==新任务操作状态::预期已变化
                                    ||结果.承接->建立结果->状态==新任务操作状态::任务状态不允许
                                    ||结果.承接->建立结果->状态==新任务操作状态::资源失败)));
                    if (扫描资源重试 || 请求技术重试
                        || ((!结果.承接->写入已尝试 || 是条件子需求)
                            && 结果.承接->状态 == 新任务治理承接状态::资源失败)) {
                        if (!停止) {
                            项->可处理时点 = std::chrono::steady_clock::now()
                                + 等待时长(配置.资源重试间隔毫秒);
                            continue;
                        }
                        结果.状态 = 新任务承接处理状态::停止取消;
                    }
                    if (是条件子需求 && 项->父筹办定位) {
                        const auto 父记录 = 最近自我筹办裁决.find(
                            项->父筹办定位->任务节点.值);
                        if (父记录 != 最近自我筹办裁决.end()
                            && 父记录->second.裁决.定位 == *项->父筹办定位) {
                            auto& 记录 = 父记录->second;
                            for (std::size_t i = 0; i < 记录.子需求承接组.size(); ++i) {
                                if (记录.子需求承接组[i] != 项) continue;
                                记录.子需求承接完成组[i] = true;
                                if (结果.状态 != 新任务承接处理状态::已承接
                                    && 结果.状态 != 新任务承接处理状态::已重新启用
                                    && 结果.状态 != 新任务承接处理状态::既有终结
                                    && 结果.状态 != 新任务承接处理状态::需求当前已满足)
                                    记录.条件交接失败 = true;
                                break;
                            }
                        }
                        if (结果.承接->任务
                            && 结果.承接->状态 == 新任务治理承接状态::已重新启用) {
                            阻塞任务.erase(结果.承接->任务->节点.值);
                            筹办阻塞任务.erase(结果.承接->任务->节点.值);
                        }
                    }
                    if ((是外部服务要求||是行动请求) && 结果.承接 && 结果.承接->任务
                        && 结果.承接->任务->当前状态 == 新任务状态::待查找方法) {
                        const auto 编码 = 结果.承接->任务->节点.值;
                        阻塞任务.erase(编码);
                        筹办阻塞任务.erase(编码);
                        等待来源.erase(编码);
                    }
                    if (结果.承接 && 结果.承接->任务
                        && 结果.状态 == 新任务承接处理状态::已承接
                        && 结果.承接->任务->当前状态 == 新任务状态::等待条件
                        && !结果.承接->任务->当前候选方法材料节点
                        && !结果.承接->任务->最近执行实例节点)
                        登记等待来源已加锁(*结果.承接->任务, 新任务等待原因::首次关联待许可);
                    调度待触发 = true;
                    项->承诺.set_value(std::move(结果));
                    标记贡献复核待触发();
                    if (自动决议) {
                        auto 位置 = std::find_if(最近自动决议.begin(),
                            最近自动决议.end(), [&](const auto& 记录) {
                                return 记录
                                    && 记录->消息.需求节点 == 项->消息.需求节点;
                            });
                        if (位置 == 最近自动决议.end()) {
                            位置 = std::find_if(最近自动决议.begin(),
                                最近自动决议.end(), [](const auto& 记录) {
                                    return !记录;
                                });
                        }
                        if (位置 != 最近自动决议.end()) {
                            *位置 = 已完成自动决议{项->消息, 项->结果};
                        }
                    }
                    活动.erase(std::find(活动.begin(), 活动.end(), 项));
                }

                锁.unlock();
                const auto 普通派发 = [&]() {
                    auto 状态 = 尝试派发调度扫描();
                    if (状态 == 筹办派发状态::无可派发任务)
                        状态 = 尝试派发执行工作();
                    if (状态 == 筹办派发状态::无可派发任务)
                        状态 = 尝试派发筹办工作();
                    return 状态;
                };
                bool 本次为结果检测 = 下次优先结果检测;
                auto 派发 = 本次为结果检测 ? 尝试派发结果检测() : 普通派发();
                if (派发 == 筹办派发状态::无可派发任务) {
                    本次为结果检测 = !本次为结果检测;
                    派发 = 本次为结果检测 ? 尝试派发结果检测() : 普通派发();
                }
                锁.lock();
                if (派发 == 筹办派发状态::已派发) {
                    下次优先结果检测 = !本次为结果检测;
                    continue;
                }
                if (派发 == 筹办派发状态::内部错误) {
                    锁.unlock();
                    工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                        L"任务工作派发", L"工作接纳或调度结构发生工程错误");
                }
                if (派发 == 筹办派发状态::已停止) {
                    停止源.request_stop(); 停止 = true;
                    continue;
                }
                生命周期 = 新任务管理生命周期::等待消息;
                变化.wait_for(锁, (std::min)(等待时长(配置.资源重试间隔毫秒),
                    等待时长(配置.调度复核间隔毫秒)));
            }

            锁.unlock();
            const bool 工作已收束 = 停止工作线程并收束();
            锁.lock();
            终结待办(内部错误发生
                ? 新任务承接处理状态::内部错误
                : 新任务承接处理状态::停止取消);
            生命周期 = !内部错误发生 && 工作已收束
                ? 新任务管理生命周期::已停止
                : 新任务管理生命周期::内部错误;
            已完成 = true;
            变化.notify_all();
        } catch (...) {
            try {
                {
                    std::lock_guard 锁(互斥);
                    内部错误发生 = true;
                    停止源.request_stop(); 停止 = true;
                    生命周期 = 新任务管理生命周期::停止中;
                    变化.notify_all();
                }
                (void)停止工作线程并收束();
                {
                    std::lock_guard 锁(互斥);
                    终结待办(新任务承接处理状态::内部错误);
                    生命周期 = 新任务管理生命周期::内部错误;
                    已完成 = true;
                    变化.notify_all();
                }
            } catch (...) { std::terminate(); }
        }
    }
};

新_任务管理线程类::新_任务管理线程类(新_任务类& 任务服务) noexcept
    : 实现_(new (std::nothrow) 实现(任务服务)) {}

新服务值结算协作状态 新_任务管理线程类::接纳服务值结算来源(
    稳定编码 已完成任务节点) noexcept {
    if (!实现_) return 新服务值结算协作状态::工程停止;
    try {
        // 领域读取在manager锁外，避免与任务集合同步/通知链形成反向锁序。
        const auto 准入 = 实现_->任务服务->核验直接服务结算来源(已完成任务节点);
        switch (准入.状态) {
        case 新直接服务结算来源状态::最终贡献消费尚未装配: {
            const auto 结算 = 实现_->任务服务->读取任务完成结算(已完成任务节点);
            if (!结算 || !结算->服务贡献)
                throw std::logic_error("待最终消费来源缺少贡献身份");
            const auto 交接 = 实现_->任务服务->读取任务完成贡献交接(已完成任务节点);
            if (交接.状态 == 新任务贡献交接读取状态::预期已变化)
                return 新服务值结算协作状态::来源不具备;
            if (交接.状态 != 新任务贡献交接读取状态::已读取 || !交接.交接)
                throw std::logic_error("最终消费贡献交接不可读");
            std::lock_guard 锁(实现_->互斥);
            if (交接.交接->直属收尾.分类 == 新任务直属收尾分类::最终已收尾) {
                实现_->待最终贡献消费.erase(结算->节点);
                return 新服务值结算协作状态::精确重复;
            }
            实现_->待最终贡献消费.insert(结算->节点);
            return 新服务值结算协作状态::最终贡献消费尚未装配;
        }
        case 新直接服务结算来源状态::非直接服务:
            return 新服务值结算协作状态::非本方法;
        case 新直接服务结算来源状态::尚未完成:
        case 新直接服务结算来源状态::需重新确认:
            return 新服务值结算协作状态::来源不具备;
        case 新直接服务结算来源状态::允许结算: break;
        default: throw std::logic_error("direct service settlement source read failed");
        }
        std::lock_guard 锁(实现_->互斥);
        if (!有效(已完成任务节点) || !实现_->服务值结算配置 || !实现_->服务值结算任务
            || 实现_->停止 || 实现_->已完成) return 新服务值结算协作状态::工程停止;
        if (已完成任务节点 == *实现_->服务值结算任务) return 新服务值结算协作状态::非本方法;
        auto& 来源组 = 实现_->待服务结算来源;
        if (std::find(来源组.begin(), 来源组.end(), 已完成任务节点) == 来源组.end())
            来源组.push_back(已完成任务节点);
        实现_->变化.notify_all();
        return 新服务值结算协作状态::已接收待筹办;
    } catch (...) {
        实现_->报告工程停止("服务结算来源消息未能收存");
        return 新服务值结算协作状态::工程停止;
    }
}

bool 新_任务管理线程类::使用任务服务(const 新_任务类& 任务服务) const noexcept {
    return 实现_ && 实现_->任务服务 == &任务服务;
}

新贡献治理工作接收结果 新_任务管理线程类::提交任务贡献治理工作(
    const void* 所有者, const 新任务贡献反馈定位& 来源) noexcept {
    新贡献治理工作接收结果 结果;
    if (!实现_ || !所有者 || !有效(来源.任务节点)
        || !有效(来源.执行实例节点) || !有效(来源.影响评估节点)) return 结果;
    try {
        // 这里只核定位身份；完整父子参数采集留给worker，不能在self提交栈归集。
        const auto t = 实现_->任务服务->获取任务(来源.任务节点);
        const auto x = 实现_->任务服务->获取执行实例(来源.执行实例节点);
        const auto a = 实现_->任务服务->获取影响评估(来源.影响评估节点);
        if (!t || !x || !a || x->任务节点 != 来源.任务节点 || x->影响评估节点 != 来源.影响评估节点
            || a->任务节点 != 来源.任务节点 || a->执行实例节点 != 来源.执行实例节点)
            throw std::logic_error("贡献提交正式定位损坏");
        新任务工作请求 请求;
        实现::工作占用 待安装;
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->自我反馈所有者 != 所有者) return 结果;
            if (实现_->停止 || 实现_->已完成) {
                结果.状态 = 新任务工作接收状态::已停止接收; return 结果;
            }
            const auto 键 = 实现::贡献键(来源);
            auto 当前 = 实现_->当前贡献工作.find(键);
            if (当前 == 实现_->当前贡献工作.end()) {
                if (实现_->贡献占用任务已加锁(来源.任务节点.值)
                    || 实现_->普通工作提交中.contains(来源.任务节点.值)) {
                    结果.状态 = 新任务工作接收状态::任务已占用; return 结果;
                }
                if (!实现_->下一工作编号 || 实现_->下一工作编号 == (std::numeric_limits<std::uint64_t>::max)())
                    throw std::logic_error("统一工作编号耗尽");
                请求.工作编号 = 实现_->下一工作编号++;
                请求.种类 = 新任务工作种类::执行任务贡献治理;
                请求.任务节点 = 来源.任务节点; 请求.贡献来源 = 来源;
                实现::最终工作记录 记录;
                记录.工作.请求 = 请求; 记录.自我所有者 = 所有者;
                实现_->最终工作组.emplace(请求.工作编号, std::move(记录));
                当前 = 实现_->当前贡献工作.emplace(键, 请求.工作编号).first;
            }
            auto& 记录 = 实现_->最终工作组.at(当前->second);
            if (记录.工作.请求.贡献来源 != 来源 || 记录.自我所有者 != 所有者)
                throw std::logic_error("相同贡献来源存在异义工作");
            try {结果.工作=记录.工作.请求;}
            catch(const std::bad_alloc&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            catch(const std::length_error&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            if (记录.工作.结果.valid()) {
                结果.状态 = 新任务工作接收状态::精确重复;
                结果.处理结果 = 记录.工作.结果; return 结果;
            }
            if (记录.提交中) { 结果.状态 = 新任务工作接收状态::任务已占用; return 结果; }
            if(实现_->普通工作提交中.contains(来源.任务节点.值)
                ||std::any_of(实现_->活动工作.begin(),实现_->活动工作.end(),[&](const auto& 项){return 项.请求.任务节点==来源.任务节点;})
                ||std::any_of(实现_->最终工作组.begin(),实现_->最终工作组.end(),[&](const auto& 项){
                    return 项.first!=当前->second&&项.second.提交中&&项.second.工作.请求.任务节点==来源.任务节点;
                })){结果.状态=新任务工作接收状态::任务已占用;return 结果;}
            const auto 特殊提交数=static_cast<std::size_t>(std::count_if(实现_->最终工作组.begin(),实现_->最终工作组.end(),
                [](const auto& 项){return 项.second.提交中;}));
            const auto 占用数=实现_->活动工作.size()+实现_->普通工作提交中.size()+特殊提交数;
            if(占用数>=实现_->配置.邮箱容量){结果.状态=新任务工作接收状态::邮箱已满;return 结果;}
            try {
                待安装=实现::工作占用{记录.工作.请求,{},{}};
                请求=记录.工作.请求;
                实现_->活动工作.reserve(占用数+1);
            }catch(const std::bad_alloc&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            catch(const std::length_error&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            记录.提交中 = true;
        }
        const auto 接收 = 实现_->工作线程->提交工作(请求);
        {
            std::lock_guard 锁(实现_->互斥);
            auto& 记录 = 实现_->最终工作组.at(请求.工作编号);
            结果.状态 = 接收.状态;
            if (接收.状态 == 新任务工作接收状态::已接收 || 接收.状态 == 新任务工作接收状态::精确重复) {
                if (!接收.处理结果.valid()) throw std::logic_error("贡献工作已接收却没有future");
                记录.工作.结果 = 接收.处理结果; 结果.处理结果 = 接收.处理结果;
                待安装.结果=接收.处理结果;
                实现_->活动工作.push_back(std::move(待安装));
            } else if (接收.状态 != 新任务工作接收状态::任务已占用
                && 接收.状态 != 新任务工作接收状态::邮箱已满
                && 接收.状态 != 新任务工作接收状态::尚未启动
                && 接收.状态 != 新任务工作接收状态::已停止接收
                && 接收.状态 != 新任务工作接收状态::资源失败) {
                throw std::logic_error("贡献工作接收工程错误");
            }
            记录.提交中 = false;
            实现_->变化.notify_all();
        }
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交贡献治理工作", L"贡献来源或技术请求身份错误");
    }
}

新任务完成结算确认状态 新_任务管理线程类::确认任务贡献治理工作(
    const void* 所有者, std::uint64_t 编号, const 新任务贡献反馈定位& 来源) noexcept {
    if (!实现_ || !所有者 || !编号) return 新任务完成结算确认状态::入口拒绝;
    return 实现_->收回贡献工作(编号, 来源, 所有者, false);
}

新最终服务工作接收结果 新_任务管理线程类::提交最终服务消费工作(
    const void* 所有者, const 新服务值最终来源定位& 来源) noexcept {
    新最终服务工作接收结果 结果;
    if (!实现_ || !所有者 || !有效(来源.来源任务节点)
        || !有效(来源.完成结算节点) || !有效(来源.直接接收节点)) return 结果;
    try {
        // 正式K/R读回在manager锁外；不把内存待办集合当作贡献权威。
        const auto 正式 = 实现_->任务服务->读取任务完成贡献交接(来源.来源任务节点);
        if (正式.状态 != 新任务贡献交接读取状态::已读取 || !正式.交接
            || 正式.交接->完成结算节点 != 来源.完成结算节点
            || 正式.交接->接收节点 != 来源.直接接收节点
            || (正式.交接->直属收尾.分类 != 新任务直属收尾分类::最终消费保留
                && 正式.交接->直属收尾.分类 != 新任务直属收尾分类::最终已收尾))
            throw std::logic_error("最终消费提交不是正式最终K/R定位");
        新任务工作请求 请求;
        实现::工作占用 待安装;
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->自我反馈所有者 != 所有者) return 结果;
            if (实现_->停止 || 实现_->已完成) {
                结果.状态 = 新任务工作接收状态::已停止接收; return 结果;
            }
            if (实现_->贡献占用任务已加锁(来源.来源任务节点.值)
                || 实现_->普通工作提交中.contains(来源.来源任务节点.值)) {
                结果.状态 = 新任务工作接收状态::任务已占用; return 结果;
            }
            auto 当前 = 实现_->当前最终工作.find(来源.完成结算节点);
            if (当前 == 实现_->当前最终工作.end()) {
                if (!实现_->下一工作编号
                    || 实现_->下一工作编号 == (std::numeric_limits<std::uint64_t>::max)())
                    throw std::logic_error("统一工作编号耗尽");
                请求.工作编号 = 实现_->下一工作编号++;
                请求.种类 = 新任务工作种类::执行最终服务消费;
                请求.任务节点 = 来源.来源任务节点;
                请求.最终来源 = 来源;
                实现::最终工作记录 记录;
                记录.工作.请求 = 请求;
                记录.自我所有者 = 所有者;
                实现_->最终工作组.emplace(请求.工作编号, std::move(记录));
                当前 = 实现_->当前最终工作.emplace(来源.完成结算节点, 请求.工作编号).first;
                实现_->待最终贡献消费.insert(来源.完成结算节点);
            }
            auto& 记录 = 实现_->最终工作组.at(当前->second);
            if (记录.工作.请求.最终来源 != 来源 || 记录.自我所有者 != 所有者)
                throw std::logic_error("相同K存在异义最终消费请求");
            try {结果.工作=记录.工作.请求;}
            catch(const std::bad_alloc&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            catch(const std::length_error&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            if (记录.工作.结果.valid()) {
                结果.状态 = 新任务工作接收状态::精确重复;
                结果.处理结果 = 记录.工作.结果; return 结果;
            }
            if (记录.提交中) { 结果.状态 = 新任务工作接收状态::任务已占用; return 结果; }
            if(实现_->普通工作提交中.contains(来源.来源任务节点.值)
                ||std::any_of(实现_->活动工作.begin(),实现_->活动工作.end(),[&](const auto& 项){return 项.请求.任务节点==来源.来源任务节点;})
                ||std::any_of(实现_->最终工作组.begin(),实现_->最终工作组.end(),[&](const auto& 项){
                    return 项.first!=当前->second&&项.second.提交中&&项.second.工作.请求.任务节点==来源.来源任务节点;
                })){结果.状态=新任务工作接收状态::任务已占用;return 结果;}
            const auto 特殊提交数=static_cast<std::size_t>(std::count_if(实现_->最终工作组.begin(),实现_->最终工作组.end(),
                [](const auto& 项){return 项.second.提交中;}));
            const auto 占用数=实现_->活动工作.size()+实现_->普通工作提交中.size()+特殊提交数;
            if(占用数>=实现_->配置.邮箱容量){结果.状态=新任务工作接收状态::邮箱已满;return 结果;}
            try {
                待安装=实现::工作占用{记录.工作.请求,{},{}};
                请求=记录.工作.请求;
                实现_->活动工作.reserve(占用数+1);
            }catch(const std::bad_alloc&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            catch(const std::length_error&){结果.状态=新任务工作接收状态::资源失败;return 结果;}
            记录.提交中 = true;
        }
        const auto 接收 = 实现_->工作线程->提交工作(请求);
        {
            std::lock_guard 锁(实现_->互斥);
            auto& 记录 = 实现_->最终工作组.at(请求.工作编号);
            结果.状态 = 接收.状态;
            if (接收.状态 == 新任务工作接收状态::已接收
                || 接收.状态 == 新任务工作接收状态::精确重复) {
                if (!接收.处理结果.valid()) throw std::logic_error("池已接收最终工作却无future");
                记录.工作.结果 = 接收.处理结果;
                结果.处理结果 = 接收.处理结果;
                待安装.结果=接收.处理结果;
                实现_->活动工作.push_back(std::move(待安装));
            } else if (接收.状态 != 新任务工作接收状态::任务已占用
                && 接收.状态 != 新任务工作接收状态::邮箱已满
                && 接收.状态 != 新任务工作接收状态::尚未启动
                && 接收.状态 != 新任务工作接收状态::已停止接收
                && 接收.状态 != 新任务工作接收状态::资源失败) {
                throw std::logic_error("最终消费工作提交工程错误");
            }
            记录.提交中 = false;
            实现_->变化.notify_all();
        }
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交最终服务消费工作", L"正式定位、工作编号或池接收不一致");
    }
}

新任务工作操作状态 新_任务管理线程类::确认最终服务消费工作(
    const void* 所有者, std::uint64_t 编号, const 新服务值最终来源定位& 来源) noexcept {
    if (!实现_ || !所有者 || !编号) return 新任务工作操作状态::入口拒绝;
    return 实现_->收回最终工作(编号, 来源, 所有者, false);
}

新任务完成结算确认状态 新_任务管理线程类::确认直接最终服务消费(
    const 新服务值最终来源定位& 来源, const 新服务值最终消费结果& 结果) noexcept {
    using 确认 = 新任务完成结算确认状态;
    if (!实现_ || !有效(来源.来源任务节点) || !有效(来源.完成结算节点)
        || !有效(来源.直接接收节点)) return 确认::入口拒绝;
    try {
        if (结果.来源 != 来源) throw std::logic_error("直接最终消费回执来源不一致");
        新服务值结算确认接口* 端口 = nullptr;
        // 两次锁内核验同K旧工作；外部事实读取绝不跨manager锁。
        const auto 旧工作未收束 = [&]() {
            return std::any_of(实现_->最终工作组.begin(), 实现_->最终工作组.end(),
                [&](const auto& 项) {
                    const auto& 原 = 项.second.工作.请求.最终来源;
                    return 原 && 原->完成结算节点 == 来源.完成结算节点
                        && 项.second.领取 != 实现::最终工作领取::已确认;
                });
        };
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->停止 || 实现_->已完成) return 确认::已停止接收;
            if (旧工作未收束()) return 确认::资源失败;
            端口 = 实现_->最终消费端口;
        }
        if (!端口) throw std::logic_error("直接最终消费确认端口缺失");
        const auto 正式 = 端口->读取最终来源结果(来源);
        if (正式.状态 == 新服务值最终消费状态::资源延后
            || ((结果.状态 == 新服务值最终消费状态::资源延后
                    || 结果.状态 == 新服务值最终消费状态::已准备待续)
                && 正式.状态 != 新服务值最终消费状态::已完成)) return 确认::资源失败;
        const bool 完成 = 核验最终消费正式结果(来源, 结果, 正式);
        if (!完成 && 正式.状态 == 新服务值最终消费状态::已入账待收尾)
            throw std::logic_error("未消费回执却发现U已入账而J5未收尾，保留现场");
        if (!完成 && 正式.状态 == 新服务值最终消费状态::等待同源工作)
            return 确认::资源失败;
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return 确认::已停止接收;
        if (旧工作未收束()) return 确认::资源失败;
        if (完成) {
            实现_->待最终贡献消费.erase(来源.完成结算节点);
            实现_->调度待触发 = true;
            实现_->标记贡献复核待触发();
            实现_->触发条件子需求复核已加锁();
            实现_->变化.notify_all();
        }
        // 零动作等待不清待办、不自激重投；不创建工作号或调用工作池ACK。
        return 确认::已确认;
    } catch (const std::bad_alloc&) {
        return 确认::资源失败;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认直接最终服务消费",
            L"直接最终消费的来源、正式结果或交接不一致；保留已发生事实。");
    }
}

新任务管理操作状态 新_任务管理线程类::绑定自我筹办反馈(
    const void* 所有者,
    筹办反馈接收函数 筹办接收,
    结果反馈接收函数 结果接收,
    学习复核接收函数 学习接收,
    等待复核接收函数 等待接收,
    贡献反馈接收函数 贡献接收,
    std::function<void()> 最终服务工作唤醒,
    const 新长期服务值结算配置& 服务结算配置) noexcept {
    if (!实现_ || !所有者 || !筹办接收 || !结果接收 || !学习接收 || !等待接收 || !贡献接收
        || !最终服务工作唤醒 || !服务结算配置.完整()) {
        return 新任务管理操作状态::入口拒绝;
    }
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->创建已尝试 || 实现_->停止 || 实现_->已完成)
            return 新任务管理操作状态::服务未就绪;
        if (实现_->自我反馈所有者) {
            return 实现_->自我反馈所有者 == 所有者
                    && 实现_->服务值结算配置 == 服务结算配置
                ? 新任务管理操作状态::无变化
                : 新任务管理操作状态::所有者冲突;
        }
        实现_->自我反馈接收 = std::move(筹办接收);
        实现_->自我结果反馈接收 = std::move(结果接收);
        实现_->自我学习反馈接收 = std::move(学习接收);
        实现_->自我等待反馈接收 = std::move(等待接收);
        实现_->自我贡献反馈接收 = std::move(贡献接收);
        实现_->自我最终工作唤醒 = std::move(最终服务工作唤醒);
        实现_->最终消费端口 = 服务结算配置.工程诊断端口;
        实现_->标记贡献复核待触发();
        实现_->自我反馈所有者 = 所有者;
        实现_->服务值结算配置 = 服务结算配置;
        实现_->变化.notify_all();
        return 新任务管理操作状态::已完成;
    } catch (const std::bad_alloc&) {
        return 新任务管理操作状态::资源失败;
    } catch (...) {
        return 新任务管理操作状态::内部错误;
    }
}

void 新_任务管理线程类::解除自我筹办反馈(const void* 所有者) noexcept {
    if (!实现_ || !所有者) return;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->自我反馈所有者 != 所有者) return;
        实现_->自我反馈接收 = {};
        实现_->自我结果反馈接收 = {};
        实现_->自我学习反馈接收 = {};
        实现_->自我等待反馈接收 = {};
        实现_->自我贡献反馈接收 = {};
        实现_->自我最终工作唤醒 = {};
        实现_->自我反馈所有者 = nullptr;
        // 端口、L配置必须保留到pool join和专属最终结果收束结束。
        实现_->变化.notify_all();
    } catch (...) {}
}

// 诊断责任：向上送出。旧入口空token复用；所有跨owner读取都在管理锁外。
新自我因果筹办提交结果 新_任务管理线程类::实现::提交因果筹办裁决内部(
    const 新自我筹办裁决& 裁决, std::stop_token 停止) {
    检查因果提交停止(停止);
    const auto 任务值 = 裁决.定位.任务节点.值;

    if (!裁决.因果回执 || 裁决.子需求回执 || 裁决.本能采用 || 裁决.执行前判断
        || 裁决.唯一具备方法节点 || 裁决.唯一具备配对节点 || 裁决.唯一对应结果节点
        || !裁决.因果回执->已完成处理 || !裁决.因果回执->原提议.依据)
        return 新自我筹办裁决接收状态::入口拒绝;
    新任务因果子需求提议 原提议;
    {
        auto 锁 = 获取因果提交锁(互斥, 停止);
        if (this->停止 || 已完成) return 新自我筹办裁决接收状态::已停止接收;
        const auto 已有 = 因果等待组.find(任务值);
        if (已有 != 因果等待组.end())
            return 已有->second.定位 == 裁决.定位 && 已有->second.回执 == *裁决.因果回执
                ? 新自我筹办裁决接收状态::精确重复 : 新自我筹办裁决接收状态::身份冲突;
        const auto 当前 = 最近筹办结果.find(任务值);
        if (当前 == 最近筹办结果.end() || 当前->second.原工作 != 裁决.定位.原工作
            || !当前->second.因果子需求提议 || 学习占用或旧筹办已失效已加锁(裁决.定位))
            return 新自我筹办裁决接收状态::定位已变化;
        原提议 = *当前->second.因果子需求提议;
    }
    const auto& 回执 = *裁决.因果回执;
    if (回执.原提议 != 原提议
        || 回执.项组.size() != 原提议.子需求项组.size() + 原提议.来源前驱前端组.size())
        return 新自我筹办裁决接收状态::身份冲突;
    // 一个正式来源前端才可行动；多义OR不能由回执任选或全部采用。
    if (原提议.来源前驱前端组.size() > 1)
        throw std::logic_error("因果采用包含未选择的多个来源前端");
    因果等待组织快照 等待组织;
    const auto 边 = 读取因果等待边内部(&等待组织, 停止);
    检查因果提交停止(停止);
    因果等待记录 记录;
    记录.定位 = 裁决.定位; 记录.回执 = 回执;
    记录.依据.采用依据 = *原提议.依据;
    记录.依据.待推进任务组 = 原提议.继续推进任务组;
    // 本条来源是否变化由具体项的当前材料判断；新增子D/T或其它候选变化不撤销等待。
    检查因果提交停止(停止);
    if (回执.项组.empty()) {
        auto 当前读取 = 任务服务->提议任务因果子需求(裁决.定位.任务节点, 边, 停止);
        if (std::holds_alternative<新任务因果读取取消>(当前读取)) throw 新任务因果读取取消{};
        auto 当前 = std::get<新任务因果子需求提议>(std::move(当前读取));
        检查因果提交停止(停止);
        if (当前.状态 == 新任务因果子需求提议状态::资源延后
            || 当前.状态 == 新任务因果子需求提议状态::材料已变化)
            return 新自我筹办裁决接收状态::资源失败;
        const auto& 原请求 = 原提议.依据->请求;
        const bool 可等待 = 当前.依据 && !当前.路径选择未确定
            && (当前.状态 == 新任务因果子需求提议状态::已形成
                || 当前.状态 == 新任务因果子需求提议状态::无需子需求)
            && 当前.子需求项组.empty() && 当前.来源前驱前端组.empty()
            && !当前.依据->请求.来源前驱上下文
            && 当前.依据->请求.父任务节点 == 裁决.定位.任务节点
            && 当前.依据->请求.父任务节点 == 原请求.父任务节点
            && 当前.依据->请求.父需求节点 == 原请求.父需求节点
            && 当前.依据->请求.目标存在节点 == 原请求.目标存在节点
            && 当前.依据->请求.目标 == 原请求.目标;
        if (可等待 && 当前.继续推进任务组.empty() && 当前.依据->读回.派生关系读回
            && 当前.依据->读回.派生关系读回->状态 == 新派生源反推状态::材料已变化)
            return 新自我筹办裁决接收状态::资源失败;
        if (可等待 && (!当前.继续推进任务组.empty() || 新_任务类::支持当前被动条件等待(当前))) {
            记录.依据.采用依据 = std::move(*当前.依据);
            记录.依据.待推进任务组 = std::move(当前.继续推进任务组);
        } else {
            记录.来源已变化 = true;
            记录.依据.待推进任务组.clear();
        }
    }
    新需求任务对应结果 对应;
    if (!回执.项组.empty()) {
        auto 对应读取 = 任务服务->读取需求列表任务对应(停止);
        if (std::holds_alternative<新任务因果读取取消>(对应读取)) throw 新任务因果读取取消{};
        对应 = std::get<新需求任务对应结果>(std::move(对应读取));
        if (对应.状态 != 新任务操作状态::已找到)
            throw std::logic_error("因果回执正式需求列表不可读");
    }
    检查因果提交停止(停止);
    std::set<稳定编码> 已确认;
    for (std::size_t i = 0; i < 回执.项组.size(); ++i) {
        检查因果提交停止(停止);
        const auto& 项 = 回执.项组[i];
        const bool 来源分支 = i >= 原提议.子需求项组.size();
        if (!来源分支) {
            if (项.原提议 != 原提议.子需求项组[i] || 项.来源前驱模板
                || 项.来源前驱实例 || 项.来源前驱具体目标)
                return 新自我筹办裁决接收状态::身份冲突;
        } else {
            const auto& 模板 = 原提议.来源前驱前端组[i - 原提议.子需求项组.size()];
            const auto& 依据 = *原提议.依据;
            if (项.原提议 != 新任务条件子需求项{} || 项.来源前驱模板 != 模板
                || !依据.请求.来源前驱上下文 || !依据.读回.来源前驱读回)
                return 新自我筹办裁决接收状态::身份冲突;
            const auto& 上下文 = *依据.请求.来源前驱上下文;
            const auto& 正式前端 = 依据.读回.来源前驱读回->行动前端组;
            if (std::find(正式前端.begin(), 正式前端.end(), 模板) == 正式前端.end()
                || 模板.支持OR路径序号组.empty())
                throw std::logic_error("因果采用来源模板不属于正式规则前端");
            for (const auto 路径 : 模板.支持OR路径序号组) {
                检查因果提交停止(停止);
                if (路径 >= 依据.读回.来源前驱读回->OR路径组.size())
                    throw std::logic_error("因果采用来源模板的支持路径越界");
            }
            const bool 内容前端 = 模板.种类 == 新来源前驱前端种类::既有需求内容
                || 模板.种类 == 新来源前驱前端种类::待实例化需求内容;
            if (!内容前端 && 模板.种类 != 新来源前驱前端种类::取得服务对象)
                throw std::logic_error("因果采用来源前端种类未知");
            if (!模板.正式根路径.empty())
                throw std::logic_error("因果采用来源前端不是正式根槽");
            if (内容前端) {
                if (!模板.H || !有效(*模板.H) || 模板.目标槽类型 != 上下文.配置.对象需求类型
                    || 模板.要求概念 != 上下文.配置.完整被认知需求概念
                    || (模板.种类 == 新来源前驱前端种类::既有需求内容) != 模板.D.has_value())
                    throw std::logic_error("因果采用内容前端角色或正式概念不符");
                if (项.来源前驱实例) {
                    const auto& 实例 = *项.来源前驱实例;
                    if (!有效(实例.出生记录) || !有效(实例.需求节点)
                        || 实例.键 != 新服务取得目标实例键{裁决.定位.任务节点,
                            模板.目标槽类型, *模板.H, 模板.正式根路径}
                        || 实例.父需求 != 依据.请求.父需求节点
                        || 实例.原父目标 != 新需求完整目标{依据.请求.目标存在节点, 依据.请求.目标}
                        || 实例.人类概念 != 上下文.配置.人类概念
                        || 实例.需求身份概念 != 上下文.配置.需求概念
                        || 实例.完整内容概念 != 模板.要求概念
                        || 实例.复用指定已有需求 != 模板.D.has_value()
                        || (模板.D && 实例.需求节点 != *模板.D))
                        throw std::logic_error("因果采用内容实例与原模板或父目标不符");
                    检查因果提交停止(停止);
                    const auto 实例读取 = 任务服务->读取来源前驱目标实例(实例.键);
                    检查因果提交停止(停止);
                    const auto 核实例身份 = [&](bool 仅核固定身份) {
                        if (!实例读取.信息)
                            throw std::logic_error("因果采用内容实例正式读回缺少信息");
                        auto 当前身份 = *实例读取.信息;
                        if (仅核固定身份)
                            当前身份.骨架就绪 = 实例.骨架就绪;
                        if (当前身份 != 实例)
                            throw std::logic_error("因果采用内容实例正式身份不符");
                    };
                    switch (实例读取.状态) {
                    case 新服务取得目标实例状态::已复用:
                        核实例身份(项.状态 == 新子需求采用状态::来源过期);
                        if (!实例读取.信息->骨架就绪 || 实例读取.完整保护.empty())
                            throw std::logic_error("因果采用内容实例正式读回不符");
                        break;
                    case 新服务取得目标实例状态::已出生待补骨架:
                        if (项.状态 != 新子需求采用状态::来源过期
                            || 项.需求节点 || 项.来源前驱具体目标)
                            throw std::logic_error("因果采用未完成骨架被用作子需求发布许可");
                        核实例身份(true);
                        记录.来源已变化 = true;
                        break;
                    case 新服务取得目标实例状态::资源失败:
                    case 新服务取得目标实例状态::依赖未就绪:
                        return 新自我筹办裁决接收状态::资源失败;
                    case 新服务取得目标实例状态::前提已变化:
                        if (实例读取.信息)
                            核实例身份(true);
                        记录.来源已变化 = true;
                        break;
                    default:
                        throw std::logic_error("因果采用已有内容实例不能正式读回");
                    }
                }
                if (项.来源前驱具体目标) {
                    if (!项.来源前驱实例 || !项.来源前驱实例->骨架就绪
                        || *项.来源前驱具体目标 != 新需求完整目标{*模板.H,
                            {模板.目标槽类型, 新需求目标形式::值, 模板.要求概念,
                             std::nullopt, std::nullopt,
                             新需求引用内容定位{项.来源前驱实例->需求节点, 模板.正式根路径}, std::nullopt}})
                        throw std::logic_error("因果采用内容具体目标与同D绑定不符");
                }
            } else {
                if (模板.H || 模板.D || 项.来源前驱实例
                    || 模板.目标槽类型 != 上下文.配置.交互者类型
                    || 模板.要求概念 != 上下文.配置.交互者人类范围)
                    throw std::logic_error("因果采用H前提混入内容实例或其它范围");
                if (项.来源前驱具体目标) {
                    const auto& 目标 = *项.来源前驱具体目标;
                    if (目标.目标存在节点 != 上下文.交互者.自我存在节点
                        || 目标.目标.目标特征类型概念节点 != 模板.目标槽类型
                        || 目标.目标.目标形式 != 新需求目标形式::值
                        || 目标.目标.目标值域概念节点 != 模板.要求概念
                        || !目标.目标.目标比较关系概念节点
                        || 目标.目标.目标变化方向 || 目标.目标.引用内容定位 || 目标.目标.新增成员来源见证)
                        throw std::logic_error("因果采用H前提具体目标不符");
                }
            }
            记录.依据.来源前端 = 项.来源前驱模板;
            记录.依据.目标实例 = 项.来源前驱实例;
        }
        const bool 已发布来源前缀 = 来源分支 && 项.状态 == 新子需求采用状态::来源过期
            && 项.需求节点.has_value();
        if (项.状态 == 新子需求采用状态::已建立 || 项.状态 == 新子需求采用状态::已复用
            || 已发布来源前缀) {
            if (已发布来源前缀) 记录.来源已变化 = true;
            if (!项.需求节点 || !有效(*项.需求节点) || !已确认.insert(*项.需求节点).second
                || (来源分支 && !项.来源前驱具体目标))
                return 新自我筹办裁决接收状态::入口拒绝;
            const auto 找到 = std::find_if(对应.对应项组.begin(), 对应.对应项组.end(),
                [&](const auto& x) {
                    检查因果提交停止(停止);
                    return x.需求.节点 == *项.需求节点;
                });
            if (找到 == 对应.对应项组.end() || !找到->需求.完整目标
                || 找到->需求.父需求节点 != 原提议.依据->请求.父需求节点
                || 找到->需求.来源承接节点 != 裁决.定位.任务节点
                || 找到->需求.来源类型 != 新需求来源类型::任务子目标
                || (来源分支
                    ? (*找到->需求.完整目标 != *项.来源前驱具体目标
                        || 找到->需求.执行域 != 新需求执行域::自我执行
                        || 找到->需求.需求主体存在节点
                            != 原提议.依据->请求.来源前驱上下文->交互者.自我存在节点)
                    : (找到->需求.完整目标->目标存在节点 != 项.原提议.目标存在节点
                        || 找到->需求.完整目标->目标.目标特征类型概念节点 != 项.原提议.目标特征类型概念节点
                        || 找到->需求.完整目标->目标.目标值域概念节点 != 项.原提议.目标值域概念节点)))
                throw std::logic_error("因果回执子需求正式身份不符");
            const auto& witness=找到->需求.完整目标->目标.新增成员来源见证;
            if(!来源分支 && (项.原提议.新增成员见证要求
                ?(!witness||witness->要求!=*项.原提议.新增成员见证要求):witness.has_value())
                )
                throw std::logic_error("因果回执新增成员来源要求不符");
            检查因果提交停止(停止);
            const auto 身份读取 = 任务服务->核验因果子需求身份(
                *原提议.依据, *项.需求节点, 停止);
            if (std::holds_alternative<新任务因果读取取消>(身份读取)) throw 新任务因果读取取消{};
            if (!std::get<bool>(身份读取)) {
                if (!来源分支)
                    throw std::logic_error("因果回执子需求完整目标或来源不符");
                // 正式静态身份已逐值核同，false只撤销当前支持，不丢已发布G/R。
                记录.来源已变化 = true;
            }
            检查因果提交停止(停止);
            记录.依据.子需求节点组.push_back(*项.需求节点);
        } else {
            if (项.需求节点) return 新自我筹办裁决接收状态::入口拒绝;
            if (项.状态 == 新子需求采用状态::来源过期) 记录.来源已变化 = true;
            else if (项.状态 != 新子需求采用状态::已无需要)
                return 新自我筹办裁决接收状态::入口拒绝;
        }
    }
    记录.子需求已承接.resize(记录.依据.子需求节点组.size(), false);
    记录.子需求承接进度.resize(记录.依据.子需求节点组.size());
    检查因果提交停止(停止);
    const bool 含需求引用 = std::any_of(等待组织.begin(), 等待组织.end(), [&](const auto& 项) {
        检查因果提交停止(停止);
        return !项.first;
    });
    if (含需求引用 && 读取因果等待边内部(nullptr, 停止) != 边)
        return 新自我筹办裁决接收状态::资源失败;
    自我筹办裁决记录 裁决记录;
    裁决记录.裁决 = 裁决; 裁决记录.已完成接收 = true;
    // 先在私有容器分配完整节点；正式采用只转移相同allocator的node_handle。
    std::map<std::uint64_t, 自我筹办裁决记录> 预备裁决;
    std::map<std::uint64_t, 因果等待记录> 预备等待;
    std::set<std::uint64_t> 预备阻塞;
    预备裁决.emplace(任务值, std::move(裁决记录));
    预备等待.emplace(任务值, std::move(记录));
    预备阻塞.insert(任务值);
    {
        auto 锁 = 获取因果提交锁(互斥, 停止);
        if (this->停止 || 已完成) return 新自我筹办裁决接收状态::已停止接收;
        if (读取因果等待组织已加锁(停止) != 等待组织)
            return 新自我筹办裁决接收状态::资源失败;
        const auto 当前 = 最近筹办结果.find(任务值);
        if (当前 == 最近筹办结果.end() || 当前->second.原工作 != 裁决.定位.原工作
            || 当前->second.因果子需求提议 != 原提议
            || 学习占用或旧筹办已失效已加锁(裁决.定位))
            return 新自我筹办裁决接收状态::定位已变化;
        const auto 已有等待 = 因果等待组.find(任务值);
        if (已有等待 != 因果等待组.end()) return 已有等待->second.回执 == 回执
            ? 新自我筹办裁决接收状态::精确重复 : 新自我筹办裁决接收状态::身份冲突;
        const auto 已有裁决 = 最近自我筹办裁决.find(任务值);
        if (已有裁决 != 最近自我筹办裁决.end() && 已有裁决->second.裁决 != 裁决)
            return 新自我筹办裁决接收状态::身份冲突;
        // 最后控制检查。首个正式插入开始后完整完成原采用，不中途取消。
        检查因果提交停止(停止);
        if (已有裁决 == 最近自我筹办裁决.end())
            最近自我筹办裁决.insert(预备裁决.extract(任务值));
        if (!筹办阻塞任务.contains(任务值))
            筹办阻塞任务.insert(预备阻塞.extract(任务值));
        因果等待组.insert(预备等待.extract(任务值));
        待反馈任务.erase(任务值);
        变化.notify_all();
    }
    return 新自我筹办裁决接收状态::已接收;
}

// 诊断责任：向上送出。只抽取原非执行采用尾部，不创建执行工作。
新自我因果筹办提交结果 新_任务管理线程类::实现::提交非执行筹办裁决内部(
    const 新自我筹办裁决& 裁决,
    const std::optional<新任务因果子需求提议>& 因果来源, std::stop_token 停止) {
    检查因果提交停止(停止);
    const auto 读取 = 任务服务->获取任务(裁决.定位.任务节点, 停止);
    if (std::holds_alternative<新任务因果读取取消>(读取)) throw 新任务因果读取取消{};
    const auto& 等待任务 = std::get<std::optional<新任务信息>>(读取);
    if (!等待任务) return 新自我筹办裁决接收状态::资源失败;
    检查因果提交停止(停止);
    const auto 任务值 = 裁决.定位.任务节点.值;
    auto 锁 = 获取因果提交锁(互斥, 停止);
    if (this->停止 || 已完成) return 新自我筹办裁决接收状态::已停止接收;
    const auto 当前 = 最近筹办结果.find(任务值);
    if (学习占用或旧筹办已失效已加锁(裁决.定位)
        || 当前 == 最近筹办结果.end() || 当前->second.原工作 != 裁决.定位.原工作)
        return 新自我筹办裁决接收状态::定位已变化;
    if (因果来源 && (当前->second.因果子需求提议 != 因果来源
        || (裁决.状态 == 新自我筹办裁决状态::等待材料
            && 因果来源->状态 != 新任务因果子需求提议状态::路径待选择)))
        return 新自我筹办裁决接收状态::定位已变化;
    const auto 已有 = 最近自我筹办裁决.find(任务值);
    if (已有 != 最近自我筹办裁决.end()) {
        if (已有->second.裁决 != 裁决) return 新自我筹办裁决接收状态::身份冲突;
        if (!已有->second.已完成接收) return 新自我筹办裁决接收状态::资源失败;
        待反馈任务.erase(任务值);
        return 新自我筹办裁决接收状态::精确重复;
    }
    自我筹办裁决记录 记录;
    记录.裁决 = 裁决;
    记录.已完成接收 = true;
    const bool 登记等待 = 学习筹办安全状态(等待任务->当前状态);
    auto 原因 = 新任务等待原因::需核查;
    const bool 可进入 = 裁决.状态 == 新自我筹办裁决状态::可进入执行前判断;
    if (登记等待) {
        const auto& 当前条件筹办 = 当前->second.独立条件筹办
            ? 当前->second.独立条件筹办 : 当前->second.条件筹办;
        if (裁决.状态 == 新自我筹办裁决状态::当前无可用方法)
            原因 = 新任务等待原因::尚无方法;
        else if (裁决.状态 == 新自我筹办裁决状态::等待材料)
            原因 = 新任务等待原因::候选条件未齐;
        else if (可进入 && 裁决.执行前判断->状态 == 新自我执行前判断状态::暂缓执行)
            原因 = 新任务等待原因::需求材料未到;
        else if (当前条件筹办 && 条件筹办为业务等待(*当前条件筹办))
            原因 = 新任务等待原因::候选条件未齐;
    }
    // 首个正式修改开始后，不在原等待组织更新中途控制取消。
    检查因果提交停止(停止);
    if (登记等待) 登记等待来源已加锁(*等待任务, 原因, 裁决.定位.原工作);
    最近自我筹办裁决.emplace(任务值, std::move(记录));
    最近任务后继决议.erase(任务值);
    待反馈任务.erase(任务值);
    变化.notify_all();
    return 新自我筹办裁决接收状态::已接收;
}

// 诊断责任：本地记录。只接收self的三类因果来源裁决。
新自我因果筹办提交结果 新_任务管理线程类::提交自我因果筹办裁决(
    const 新自我筹办裁决& 裁决, std::stop_token 停止) noexcept {
    if (!实现_ || !裁决.定位.完整()) return 新自我筹办裁决接收状态::入口拒绝;
    try {
        检查因果提交停止(停止);
        if (裁决.状态 == 新自我筹办裁决状态::已处理因果子需求)
            return 实现_->提交因果筹办裁决内部(裁决, 停止);
        if ((裁决.状态 != 新自我筹办裁决状态::等待材料
                && 裁决.状态 != 新自我筹办裁决状态::任务状态变化)
            || 裁决.因果回执 || 裁决.子需求回执 || 裁决.本能采用 || 裁决.执行前判断
            || 裁决.唯一具备方法节点 || 裁决.唯一具备配对节点 || 裁决.唯一对应结果节点)
            return 新自我筹办裁决接收状态::入口拒绝;
        std::optional<新任务因果子需求提议> 因果来源;
        {
            auto 锁 = 获取因果提交锁(实现_->互斥, 停止);
            if (实现_->停止 || 实现_->已完成) return 新自我筹办裁决接收状态::已停止接收;
            const auto 当前 = 实现_->最近筹办结果.find(裁决.定位.任务节点.值);
            if (当前 == 实现_->最近筹办结果.end()
                || 当前->second.原工作 != 裁决.定位.原工作
                || !当前->second.因果子需求提议
                || 实现_->学习占用或旧筹办已失效已加锁(裁决.定位))
                return 新自我筹办裁决接收状态::定位已变化;
            if (裁决.状态 == 新自我筹办裁决状态::等待材料
                && 当前->second.因果子需求提议->状态 != 新任务因果子需求提议状态::路径待选择)
                return 新自我筹办裁决接收状态::入口拒绝;
            因果来源 = 当前->second.因果子需求提议;
        }
        检查因果提交停止(停止);
        return 实现_->提交非执行筹办裁决内部(裁决, 因果来源, 停止);
    } catch (const 新任务因果读取取消&) {
        return 新任务因果读取取消{};
    } catch (const std::bad_alloc&) {
        return 新自我筹办裁决接收状态::资源失败;
    } catch (const std::exception& error) {
        工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
            L"提交自我因果筹办裁决", L"因果来源裁决接纳发生工程错误",error.what());
    } catch (...) {
        工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
            L"提交自我因果筹办裁决", L"因果来源裁决接纳发生工程错误");
    }
}

新自我筹办裁决接收状态 新_任务管理线程类::提交自我筹办裁决(
    const 新自我筹办裁决& 裁决) noexcept {
    if (!实现_ || !裁决.定位.完整())
        return 新自我筹办裁决接收状态::入口拒绝;
    try {
        const auto 任务值 = 裁决.定位.任务节点.值;
        const bool 是因果回执 = 裁决.状态 == 新自我筹办裁决状态::已处理因果子需求;
        if (是因果回执 != 裁决.因果回执.has_value())
            return 新自我筹办裁决接收状态::入口拒绝;
        if (是因果回执) {
            const auto 提交 = 实现_->提交因果筹办裁决内部(裁决, {});
            return std::get<新自我筹办裁决接收状态>(提交);
        }
        const bool 是子需求回执 = 裁决.状态 == 新自我筹办裁决状态::已处理条件子需求;
        if (是子需求回执 != 裁决.子需求回执.has_value())
            return 新自我筹办裁决接收状态::入口拒绝;
        if (是子需求回执) {
            const auto 停止令牌 = 实现_->停止源.get_token();
            const auto 补交已接收项 = [&]() {
                try { 实现_->尝试交付条件准备依据(停止令牌); }
                catch (const 新任务因果读取取消&) {}
                catch (const std::bad_alloc&) {}
                catch (const std::length_error&) {}
                // 正式record/outbox已经接收；取消或资源只延后交付，不伪撤销接收。
            };
            检查因果提交停止(停止令牌);
            if (裁决.定位.原工作.种类 != 新任务工作种类::检查条件
                || 裁决.唯一具备方法节点 || 裁决.唯一具备配对节点
                || 裁决.唯一对应结果节点 || 裁决.执行前判断 || 裁决.本能采用
                || !裁决.子需求回执->已完成处理)
                return 新自我筹办裁决接收状态::入口拒绝;
            新任务条件子需求提议 原提议;
            bool 是精确重复 = false;
            {
                std::lock_guard 锁(实现_->互斥);
                if (实现_->停止 || 实现_->已完成)
                    return 新自我筹办裁决接收状态::已停止接收;
                const auto 当前 = 实现_->最近筹办结果.find(任务值);
                if (实现_->学习占用或旧筹办已失效已加锁(裁决.定位)
                    || 当前 == 实现_->最近筹办结果.end()
                    || 当前->second.原工作 != 裁决.定位.原工作
                    || 当前->second.状态 != 新任务筹办处理状态::条件筹办已完成
                    || !当前->second.独立子需求提议)
                    return 新自我筹办裁决接收状态::定位已变化;
                const auto 已有 = 实现_->最近自我筹办裁决.find(任务值);
                if (已有 != 实现_->最近自我筹办裁决.end()) {
                    if (已有->second.裁决 != 裁决)
                        return 新自我筹办裁决接收状态::身份冲突;
                    if (!已有->second.已完成接收)
                        return 新自我筹办裁决接收状态::资源失败;
                    是精确重复 = true;
                }
                if (!是精确重复) 原提议 = *当前->second.独立子需求提议;
            }
            if (是精确重复) {
                补交已接收项();
                return 新自我筹办裁决接收状态::精确重复;
            }
            const auto& 回执 = *裁决.子需求回执;
            if (回执.原提议 != 原提议
                || 回执.项组.size() != 原提议.子需求项组.size())
                return 新自我筹办裁决接收状态::身份冲突;
            实现::自我筹办裁决记录 记录;
            记录.裁决 = 裁决;
            std::set<std::uint64_t> 成功需求;
            for (std::size_t i = 0; i < 回执.项组.size(); ++i) {
                const auto& 项 = 回执.项组[i];
                if (项.原提议 != 原提议.子需求项组[i] || 项.来源前驱模板
                    || 项.来源前驱实例 || 项.来源前驱具体目标)
                    return 新自我筹办裁决接收状态::身份冲突;
                switch (项.状态) {
                case 新子需求采用状态::已建立:
                case 新子需求采用状态::已复用:
                    if (!项.需求节点 || !有效(*项.需求节点)
                        || !成功需求.insert(项.需求节点->值).second)
                        return 新自我筹办裁决接收状态::入口拒绝;
                    记录.已确认子需求组.push_back({项.原提议, *项.需求节点});
                    break;
                case 新子需求采用状态::已无需要:
                    if (项.需求节点) return 新自我筹办裁决接收状态::入口拒绝;
                    break;
                case 新子需求采用状态::来源过期:
                    if (项.需求节点) return 新自我筹办裁决接收状态::入口拒绝;
                    记录.条件来源待重筹办 = true;
                    break;
                case 新子需求采用状态::拒绝:
                case 新子需求采用状态::结构错误:
                    if (项.需求节点) return 新自我筹办裁决接收状态::入口拒绝;
                    记录.条件交接失败 = true;
                    break;
                default:
                    return 新自我筹办裁决接收状态::入口拒绝;
                }
            }
            for (const auto& 子项 : 记录.已确认子需求组) {
                检查因果提交停止(停止令牌);
                const 新_任务类::方法条件准备承接凭据 凭据{原提议, 子项};
                const auto 读取 = 实现_->任务服务->读取方法条件准备静态身份(凭据, 停止令牌);
                if (std::holds_alternative<新任务因果读取取消>(读取))
                    throw 新任务因果读取取消{};
                const auto* 读回 = std::get_if<新_任务类::方法条件准备身份结果>(&读取);
                if (!读回) throw std::logic_error("条件准备静态身份回执分支非法");
                检查因果提交停止(停止令牌);
                if (读回->状态 == 新_任务类::方法条件准备身份状态::来源已变化) {
                    if (读回->静态批) throw std::logic_error("条件准备源变仍携成功静态批");
                    记录.来源变化子需求.insert(子项.需求节点.值);
                    记录.条件交接失败 = true;
                    continue;
                }
                switch (读回->状态) {
                case 新_任务类::方法条件准备身份状态::依赖未就绪:
                case 新_任务类::方法条件准备身份状态::未支持:
                case 新_任务类::方法条件准备身份状态::资源失败:
                    if (读回->静态批) throw std::logic_error("条件准备非成功仍携静态批");
                    return 新自我筹办裁决接收状态::资源失败;
                case 新_任务类::方法条件准备身份状态::身份一致:
                    if (!读回->静态批 || 读回->静态批->原凭据 != 凭据
                        || 读回->静态批->完整保护.empty())
                        throw std::logic_error("条件准备成功静态身份缺少真实材料");
                    break;
                default:
                    throw std::logic_error("条件准备静态身份状态未知");
                }
            }
            检查因果提交停止(停止令牌);
            const auto 当前提议 = 实现_->任务服务->提议任务条件子需求(裁决.定位.任务节点);
            if (当前提议.状态 == 新任务条件子需求提议状态::结构不一致)
                工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                    L"提交自我筹办裁决", L"条件子需求来源重新读取失败");
            if (当前提议.状态 == 新任务条件子需求提议状态::资源失败)
                return 新自我筹办裁决接收状态::资源失败;
            检查因果提交停止(停止令牌);
            if(!条件提议等材料(当前提议)) {
                记录.条件来源待重筹办 |= !条件提议来源一致(当前提议,原提议)
                    ||std::any_of(记录.已确认子需求组.begin(),记录.已确认子需求组.end(),[&](const auto& 已确认){
                        return 读取条件子项当前支持(当前提议,原提议,已确认.原提议)==条件子项当前支持::已变化;
                    });
            }
            记录.子需求承接组.resize(记录.已确认子需求组.size());
            记录.子需求承接完成组.resize(记录.已确认子需求组.size(), false);
            记录.条件准备依据已交付组.resize(记录.已确认子需求组.size(), false);
            记录.条件复核待触发 = true; // 首次收执后的一次补查，承接未收口先合并。
            记录.已完成接收 = true;
            // record及其每个待交付项的唯一副本全部在正式接收前预分配。
            std::map<std::uint64_t, 实现::条件准备待交付指针> 预备待交付;
            for (const auto& 子项 : 记录.已确认子需求组) {
                检查因果提交停止(停止令牌);
                const auto 项 = std::make_shared<const 实现::条件准备待交付项>(
                    实现::条件准备待交付项{裁决.定位, 原提议, 子项});
                if (!预备待交付.emplace(子项.需求节点.值, 项).second)
                    throw std::logic_error("条件准备预备待交付出现重复子需求");
            }
            std::map<std::uint64_t, 实现::自我筹办裁决记录> 预备裁决;
            std::set<std::uint64_t> 预备阻塞;
            预备裁决.emplace(任务值, std::move(记录));
            预备阻塞.insert(任务值);
            {
                std::lock_guard 锁(实现_->互斥);
                if (实现_->停止 || 实现_->已完成)
                    return 新自我筹办裁决接收状态::已停止接收;
                const auto 当前 = 实现_->最近筹办结果.find(任务值);
                if (实现_->学习占用或旧筹办已失效已加锁(裁决.定位)
                    || 当前 == 实现_->最近筹办结果.end()
                    || 当前->second.原工作 != 裁决.定位.原工作
                    || 当前->second.独立子需求提议 != 原提议)
                    return 新自我筹办裁决接收状态::定位已变化;
                const auto 已有 = 实现_->最近自我筹办裁决.find(任务值);
                if (已有 != 实现_->最近自我筹办裁决.end()) {
                    if (已有->second.裁决 != 裁决)
                        return 新自我筹办裁决接收状态::身份冲突;
                    if (!已有->second.已完成接收)
                        return 新自我筹办裁决接收状态::资源失败;
                    是精确重复 = true;
                } else {
                    for (const auto& [子D, 项] : 预备待交付) {
                        const auto 旧 = 实现_->待交付条件准备组.find(子D);
                        if (旧 != 实现_->待交付条件准备组.end()
                            && (!旧->second || 旧->second->原提议 != 项->原提议
                                || 旧->second->已确认项 != 项->已确认项))
                            throw std::logic_error("条件准备同子需求待交付原值冲突");
                    }
                    // 最后取消门。以下整数key/node_handle转移不再分配或取消。
                    检查因果提交停止(停止令牌);
                    实现_->最近自我筹办裁决.insert(预备裁决.extract(任务值));
                    for (auto 位置 = 预备待交付.begin(); 位置 != 预备待交付.end();) {
                        const auto 子D = 位置->first;
                        ++位置;
                        if (!实现_->待交付条件准备组.contains(子D))
                            实现_->待交付条件准备组.insert(预备待交付.extract(子D));
                    }
                    if (!实现_->筹办阻塞任务.contains(任务值))
                        实现_->筹办阻塞任务.insert(预备阻塞.extract(任务值));
                    实现_->待反馈任务.erase(任务值);
                    实现_->变化.notify_all();
                }
            }
            补交已接收项();
            return 是精确重复 ? 新自我筹办裁决接收状态::精确重复
                : 新自我筹办裁决接收状态::已接收;
        }
        if (裁决.状态 == 新自我筹办裁决状态::任务状态变化
            && 裁决.定位.原工作.种类 == 新任务工作种类::查找方法) {
            if (裁决.唯一具备方法节点 || 裁决.唯一具备配对节点
                || 裁决.唯一对应结果节点 || 裁决.执行前判断
                || 裁决.本能采用)
                return 新自我筹办裁决接收状态::入口拒绝;
            std::optional<新任务内容方法学习发现> 原发现;
            {
                std::lock_guard 锁(实现_->互斥);
                if (实现_->停止 || 实现_->已完成) return 新自我筹办裁决接收状态::已停止接收;
                const auto 当前 = 实现_->最近筹办结果.find(任务值);
                if (当前 == 实现_->最近筹办结果.end()
                    || 当前->second.原工作 != 裁决.定位.原工作
                    || 实现_->学习占用或旧筹办已失效已加锁(裁决.定位))
                    return 新自我筹办裁决接收状态::定位已变化;
                if (当前->second.状态 == 新任务筹办处理状态::内容结果定位未支持) {
                    if (!当前->second.工作结果 || !当前->second.工作结果->方法召回
                        || !当前->second.工作结果->方法召回->内容学习发现)
                        throw std::logic_error("内容旧工作缺少完整学习发现");
                    原发现 = 当前->second.工作结果->方法召回->内容学习发现;
                }
            }
            if (原发现) {
                const auto fresh = 实现_->任务服务->查询内容叶方法学习发现(裁决.定位.任务节点);
                if (fresh.状态 == 新任务内容学习发现状态::资源失败
                    || fresh.状态 == 新任务内容学习发现状态::查询未完成)
                    return 新自我筹办裁决接收状态::资源失败;
                if (fresh.状态 == 新任务内容学习发现状态::已形成 && !fresh.发现)
                    throw std::logic_error("内容学习发现成功但未交完整材料");
                if (fresh.状态 == 新任务内容学习发现状态::已形成 && fresh.发现 == 原发现)
                    return 新自我筹办裁决接收状态::资源失败; // 已恢复，令self重新核验，不释放仍有效工作。
                std::lock_guard 锁(实现_->互斥);
                if (实现_->停止 || 实现_->已完成) return 新自我筹办裁决接收状态::已停止接收;
                const auto 当前 = 实现_->最近筹办结果.find(任务值);
                if (当前 == 实现_->最近筹办结果.end()
                    || 当前->second.原工作 != 裁决.定位.原工作
                    || 当前->second.状态 != 新任务筹办处理状态::内容结果定位未支持
                    || !当前->second.工作结果 || !当前->second.工作结果->方法召回
                    || 当前->second.工作结果->方法召回->内容学习发现 != 原发现
                    || 实现_->学习占用或旧筹办已失效已加锁(裁决.定位)
                    || std::any_of(实现_->活动工作.begin(), 实现_->活动工作.end(),
                        [&](const auto& 项) { return 项.请求.任务节点 == 裁决.定位.任务节点; }))
                    return 新自我筹办裁决接收状态::定位已变化;
                const auto 旧裁决 = 实现_->最近自我筹办裁决.find(任务值);
                const auto 等待 = 实现_->等待来源.find(任务值);
                const auto 延后 = 实现_->筹办延后重入.find(任务值);
                if ((旧裁决 != 实现_->最近自我筹办裁决.end() && 旧裁决->second.裁决.定位 != 裁决.定位)
                    || (等待 != 实现_->等待来源.end() && 等待->second.原工作 != std::optional{裁决.定位.原工作})
                    || (延后 != 实现_->筹办延后重入.end() && 延后->second != 裁决.定位.原工作))
                    return 新自我筹办裁决接收状态::定位已变化;
                // 只释放本T该原W1控制进度；不改Vt、不清工程保护或学习占用。
                实现_->最近筹办结果.erase(当前);
                实现_->待反馈任务.erase(任务值);
                实现_->最近自我筹办裁决.erase(任务值);
                实现_->筹办阻塞任务.erase(任务值);
                实现_->等待来源.erase(任务值);
                实现_->等待目标复核.erase(任务值);
                实现_->筹办延后重入.erase(任务值);
                实现_->上次等待材料.erase(任务值);
                实现_->调度待触发 = true;
                实现_->变化.notify_all();
                return 新自我筹办裁决接收状态::已接收;
            }
        }
        const bool 可进入 = 裁决.状态
            == 新自我筹办裁决状态::可进入执行前判断;
        if (可进入) {
            if (!裁决.唯一具备方法节点 || !裁决.唯一具备配对节点
                || !裁决.唯一对应结果节点 || !裁决.执行前判断 || !裁决.本能采用
                || 裁决.本能采用->来源方法节点 != *裁决.唯一具备方法节点
                || 裁决.本能采用->来源配对节点 != *裁决.唯一具备配对节点
                || 裁决.本能采用->来源结果节点 != *裁决.唯一对应结果节点
                || 裁决.执行前判断->任务节点 != 裁决.定位.任务节点
                || 裁决.执行前判断->方法节点
                    != *裁决.唯一具备方法节点
                || 裁决.执行前判断->配对节点
                    != *裁决.唯一具备配对节点
                || 裁决.执行前判断->结果节点
                    != *裁决.唯一对应结果节点
                || (裁决.执行前判断->状态
                        == 新自我执行前判断状态::允许执行
                    && (!裁决.执行前判断->本能执行请求
                        || !裁决.执行前判断->本能执行请求->完整()
                        || 裁决.执行前判断->本能执行请求->本能采用 != 裁决.本能采用
                        || 裁决.执行前判断->本能执行请求->任务节点
                            != 裁决.定位.任务节点
                        || 裁决.执行前判断->本能执行请求->方法节点
                            != *裁决.唯一具备方法节点
                        || 裁决.执行前判断->本能执行请求->配对节点
                            != *裁决.唯一具备配对节点
                        || 裁决.执行前判断->本能执行请求->结果节点
                            != *裁决.唯一对应结果节点))
                || (裁决.执行前判断->状态
                        != 新自我执行前判断状态::允许执行
                    && 裁决.执行前判断->本能执行请求)) {
                return 新自我筹办裁决接收状态::入口拒绝;
            }
        } else if (裁决.唯一具备方法节点 || 裁决.唯一具备配对节点
            || 裁决.唯一对应结果节点 || 裁决.执行前判断 || 裁决.本能采用) {
            return 新自我筹办裁决接收状态::入口拒绝;
        }
        const bool 需要执行 = 可进入
            && 裁决.执行前判断->状态
                == 新自我执行前判断状态::允许执行;
        if (!需要执行) {
            const auto 提交 = 实现_->提交非执行筹办裁决内部(裁决, {}, {});
            return std::get<新自我筹办裁决接收状态>(提交);
        }
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->停止 || 实现_->已完成)
                return 新自我筹办裁决接收状态::已停止接收;
            const auto 当前 = 实现_->最近筹办结果.find(任务值);
            if (实现_->学习占用或旧筹办已失效已加锁(裁决.定位)
                || 当前 == 实现_->最近筹办结果.end()
                || 当前->second.原工作 != 裁决.定位.原工作) {
                return 新自我筹办裁决接收状态::定位已变化;
            }
            const auto 已有 = 实现_->最近自我筹办裁决.find(任务值);
            if (已有 != 实现_->最近自我筹办裁决.end()) {
                if (已有->second.裁决 != 裁决)
                    return 新自我筹办裁决接收状态::身份冲突;
                if (!已有->second.已完成接收)
                    return 新自我筹办裁决接收状态::资源失败;
                实现_->待反馈任务.erase(任务值);
                return 新自我筹办裁决接收状态::精确重复;
            }
            std::optional<新任务工作请求> 执行工作;
            if (需要执行) {
                if (!实现_->下一工作编号
                    || 实现_->下一工作编号
                        == (std::numeric_limits<std::uint64_t>::max)()) {
                    return 新自我筹办裁决接收状态::资源失败;
                }
                新任务工作请求 请求;
                请求.工作编号 = 实现_->下一工作编号;
                请求.种类 = 新任务工作种类::执行本能动作;
                请求.任务节点 = 裁决.定位.任务节点;
                请求.本能执行请求 =
                    裁决.执行前判断->本能执行请求;
                执行工作 = std::move(请求);
            }
            实现::自我筹办裁决记录 记录;
            记录.裁决 = 裁决;
            记录.已完成接收 = true;
            记录.执行工作 = std::move(执行工作);
            实现_->最近自我筹办裁决.emplace(任务值, std::move(记录));
            实现_->最近任务后继决议.erase(任务值);
            if (需要执行) ++实现_->下一工作编号;
            实现_->待反馈任务.erase(任务值);
            实现_->变化.notify_all();
        }
        return 新自我筹办裁决接收状态::已接收;
    } catch (const 新任务因果读取取消&) {
        return 新自我筹办裁决接收状态::已停止接收;
    } catch (const std::bad_alloc&) {
        return 新自我筹办裁决接收状态::资源失败;
    } catch (const std::length_error&) {
        return 新自我筹办裁决接收状态::资源失败;
    } catch (...) {
        if (裁决.状态 == 新自我筹办裁决状态::已处理条件子需求)
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"提交自我筹办裁决", L"条件准备正式接收或待交付回执发生工程错误");
        return 新自我筹办裁决接收状态::内部错误;
    }
}

新任务结果状态接收状态 新_任务管理线程类::提交结果状态通知(
    稳定编码 状态节点) noexcept {
    if (!实现_ || !有效(状态节点)) return 新任务结果状态接收状态::内部错误;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成)
            return 新任务结果状态接收状态::已停止接收;
        // 通知只提示重读，不在调用者线程查询、核验或接纳S。
        实现_->结果通知待处理 = true;
        实现_->触发条件子需求复核已加锁();
        实现_->变化.notify_all();
        return 新任务结果状态接收状态::已接收待异步检测;
    } catch (...) {
        工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
            L"提交结果状态通知", L"结果检测通知接纳失败");
    }
}

新自我任务后继接收状态 新_任务管理线程类::提交自我任务后继决议(
    const 新任务结果反馈定位& 定位,
    const 新自我任务后继决议& 决议) noexcept {
    if (!实现_ || !定位.完整()
        || 决议.任务节点 != 定位.任务节点
        || 决议.执行实例节点 != 定位.执行实例节点
        || 决议.影响评估节点 != 定位.影响评估节点) {
        return 新自我任务后继接收状态::入口拒绝;
    }
    try {
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->停止 || 实现_->已完成) {
                return 新自我任务后继接收状态::已停止接收;
            }
            const auto 已有决议 = 实现_->最近任务后继决议.find(
                定位.任务节点.值);
            if (已有决议 != 实现_->最近任务后继决议.end()) {
                return 已有决议->second == 决议
                    ? 新自我任务后继接收状态::精确重复
                    : 新自我任务后继接收状态::入口拒绝;
            }
            const auto 当前 = 实现_->最近执行结果.find(定位.任务节点.值);
            if (当前 == 实现_->最近执行结果.end()
                || 当前->second.状态
                    != 新任务执行处理状态::已完成影响评估
                || 当前->second.原工作 != 定位.原工作
                || !当前->second.独立读回
                || !当前->second.独立读回->执行
                || !当前->second.独立读回->影响评估
                || 当前->second.独立读回->执行->节点
                    != 定位.执行实例节点
                || 当前->second.独立读回->影响评估->节点
                    != 定位.影响评估节点) {
                return 新自我任务后继接收状态::定位已变化;
            }
        }
        const auto 发布 = 实现_->任务服务->发布自我任务后继决议(决议);
        新自我任务后继接收状态 状态 =
            新自我任务后继接收状态::内部错误;
        switch (发布.状态) {
        case 新任务治理后继发布状态::已发布并读回:
            状态 = 新自我任务后继接收状态::已发布并读回; break;
        case 新任务治理后继发布状态::精确重复:
            状态 = 新自我任务后继接收状态::精确重复; break;
        case 新任务治理后继发布状态::定位已变化:
        case 新任务治理后继发布状态::需求信息待完善:
            // 完整性或确认依据已变化，旧完成决议失效；不发布完成，
            // 也不把业务等待升级为工程错误。
            状态 = 新自我任务后继接收状态::定位已变化; break;
        case 新任务治理后继发布状态::任务状态已变化:
            状态 = 新自我任务后继接收状态::任务状态已变化; break;
        case 新任务治理后继发布状态::状态发布失败:
            状态 = 新自我任务后继接收状态::状态发布失败; break;
        case 新任务治理后继发布状态::资源失败:
            状态 = 新自我任务后继接收状态::资源失败; break;
        case 新任务治理后继发布状态::结构不一致:
        default:
            状态 = 新自我任务后继接收状态::内部错误; break;
        }
        std::optional<新任务信息> 等待任务;
        if ((状态 == 新自我任务后继接收状态::已发布并读回
                || 状态 == 新自我任务后继接收状态::精确重复)
            && 决议.类型 == 新自我任务后继类型::等待)
            等待任务 = 实现_->任务服务->获取任务(定位.任务节点);
        if ((状态 == 新自我任务后继接收状态::已发布并读回
                || 状态 == 新自我任务后继接收状态::精确重复)
            && 决议.类型 == 新自我任务后继类型::等待 && !等待任务)
            return 新自我任务后继接收状态::资源失败;
        if (状态 == 新自我任务后继接收状态::已发布并读回
            || 状态 == 新自我任务后继接收状态::精确重复) {
            std::lock_guard 锁(实现_->互斥);
            if (等待任务 && 等待任务->当前状态 == 新任务状态::等待条件
                && 等待任务->最近执行实例节点 == 定位.执行实例节点)
                实现_->登记等待来源已加锁(*等待任务,
                    新任务等待原因::执行后等待, 定位.原工作);
            实现_->最近任务后继决议[定位.任务节点.值] = 决议;
            实现_->标记贡献复核待触发();
            实现_->调度待触发 = true;
            实现_->待结果反馈任务.erase(定位.任务节点.值);
            // 完成状态先于完成结算发布。完成分支必须保留原结果定位，直到
            // self确认结算已经由task owner正式建立；否则结算资源失败后
            // 无法按原请求恢复。其它后继已经没有待完成的self业务步骤。
            if (决议.类型 != 新自我任务后继类型::完成) {
                if (const auto 等待 = 实现_->结果等待任务.find(定位.任务节点.值);
                    等待 != 实现_->结果等待任务.end()
                    && 等待->second.执行实例节点 == 定位.执行实例节点
                    && !等待->second.已派发)
                    实现_->结果等待任务.erase(等待);
                实现_->阻塞任务.erase(定位.任务节点.值);
                实现_->筹办阻塞任务.erase(定位.任务节点.值);
                实现_->最近筹办结果.erase(定位.任务节点.值);
                实现_->最近自我筹办裁决.erase(定位.任务节点.值);
                实现_->最近执行结果.erase(定位.任务节点.值);
            }
            实现_->变化.notify_all();
        }
        return 状态;
    } catch (const std::bad_alloc&) {
        return 新自我任务后继接收状态::资源失败;
    } catch (...) {
        return 新自我任务后继接收状态::内部错误;
    }
}

新任务完成结算确认状态 新_任务管理线程类::确认任务完成结算(
    const 新任务结果反馈定位& 定位,
    稳定编码 结算节点) noexcept {
    if (!实现_ || !定位.完整() || !有效(结算节点)) {
        return 新任务完成结算确认状态::入口拒绝;
    }
    try {
        const auto 结算 = 实现_->任务服务->读取任务完成结算(定位.任务节点);
        if (!结算) return 新任务完成结算确认状态::尚未结算;
        if (结算->节点 != 结算节点
            || 结算->任务节点 != 定位.任务节点
            || 结算->执行实例节点 != 定位.执行实例节点
            || 结算->影响评估节点 != 定位.影响评估节点) {
            return 新任务完成结算确认状态::定位已变化;
        }

        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            return 新任务完成结算确认状态::已停止接收;
        }
        const auto 决议 = 实现_->最近任务后继决议.find(定位.任务节点.值);
        if (决议 == 实现_->最近任务后继决议.end()) {
            return 新任务完成结算确认状态::精确重复;
        }
        if (决议->second.任务节点 != 定位.任务节点
            || 决议->second.执行实例节点 != 定位.执行实例节点
            || 决议->second.影响评估节点 != 定位.影响评估节点
            || 决议->second.类型 != 新自我任务后继类型::完成) {
            return 新任务完成结算确认状态::定位已变化;
        }
        const auto 执行结果 = 实现_->最近执行结果.find(定位.任务节点.值);
        if (执行结果 == 实现_->最近执行结果.end()
            || 执行结果->second.原工作 != 定位.原工作) {
            return 新任务完成结算确认状态::定位已变化;
        }
        实现_->触发条件子需求复核已加锁();
        if (const auto 等待 = 实现_->结果等待任务.find(定位.任务节点.值);
                    等待 != 实现_->结果等待任务.end()
                    && 等待->second.执行实例节点 == 定位.执行实例节点
                    && !等待->second.已派发)
                    实现_->结果等待任务.erase(等待);
        实现_->阻塞任务.erase(定位.任务节点.值);
        实现_->筹办阻塞任务.erase(定位.任务节点.值);
        实现_->最近筹办结果.erase(定位.任务节点.值);
        实现_->最近自我筹办裁决.erase(定位.任务节点.值);
        实现_->最近执行结果.erase(定位.任务节点.值);
        实现_->最近任务后继决议.erase(决议);
        实现_->变化.notify_all();
        return 新任务完成结算确认状态::已确认;
    } catch (const std::bad_alloc&) {
        return 新任务完成结算确认状态::资源失败;
    } catch (...) {
        return 新任务完成结算确认状态::内部错误;
    }
}

新任务完成结算确认状态 新_任务管理线程类::确认任务贡献复核(
    const 新任务贡献反馈定位& 定位, const 新任务贡献推进结果& 推进) noexcept {
    if (!实现_) return 新任务完成结算确认状态::入口拒绝;
    return 实现_->确认任务贡献复核内部(定位, 推进, false);
}

新_任务管理线程类::~新_任务管理线程类() noexcept {
    if (!实现_) return;
    (void)请求停止();
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        if (实现_->线程.joinable()) {
            if (实现_->线程.get_id() == std::this_thread::get_id()) std::terminate();
            实现_->线程.join();
        }
        {
            std::lock_guard 锁(实现_->互斥);
            实现_->自我反馈接收 = {};
            实现_->自我结果反馈接收 = {};
            实现_->自我学习反馈接收 = {};
            实现_->自我等待反馈接收 = {};
            实现_->自我贡献反馈接收 = {};
            实现_->自我最终工作唤醒 = {};
            实现_->自我反馈所有者 = nullptr;
        }
        std::lock_guard 所有者锁(管理所有者互斥);
        if (管理所有者 == this) 管理所有者 = nullptr;
    } catch (...) { std::terminate(); }
}

新任务管理操作结果 新_任务管理线程类::启动(
    const 新任务管理运行配置& 配置, std::uint64_t 等待毫秒) noexcept {
    if (!实现_ || !实现_->工作线程) return {
        新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (实现_->停止源.stop_requested()) return 实现_->操作结果(新任务管理操作状态::入口拒绝);
        if (!配置.邮箱容量 || 配置.邮箱容量 > 实现_->活动.max_size()
            || !配置.资源重试间隔毫秒 || !等待毫秒 || !配置.调度复核间隔毫秒
            || static_cast<std::uint64_t>(等待时长(配置.资源重试间隔毫秒).count())
                != 配置.资源重试间隔毫秒
            || static_cast<std::uint64_t>(等待时长(配置.调度复核间隔毫秒).count())
                != 配置.调度复核间隔毫秒)
            return 实现_->操作结果(新任务管理操作状态::入口拒绝);
        if (实现_->创建已尝试) return 实现_->操作结果(
            实现_->已进入 && !实现_->停止 && !实现_->已完成 && 实现_->配置 == 配置
                ? 新任务管理操作状态::已在运行 : 新任务管理操作状态::入口拒绝);
        {
            std::lock_guard 所有者锁(管理所有者互斥);
            if (管理所有者 && 管理所有者 != this)
                return 实现_->操作结果(新任务管理操作状态::所有者冲突);
            管理所有者 = this;
        }
        实现_->创建已尝试 = true;
        实现_->配置 = 配置;
        实现_->生命周期 = 新任务管理生命周期::启动中;
        锁.unlock();
        const bool 就绪 = 实现_->任务服务->初始化治理();
        std::optional<新任务治理承接结果> 服务结算任务承接;
        bool 服务结算任务就绪 = 就绪;
        if (就绪 && 实现_->服务值结算配置) {
            服务结算任务承接 = 实现_->任务服务->承接需求任务(
                实现_->服务值结算配置->完成服务需求, false);
            服务结算任务就绪 = 服务结算任务承接->任务
                && 服务结算任务承接->虚拟存在
                && 服务结算任务承接->任务->需求节点
                    == 实现_->服务值结算配置->完成服务需求
                && 服务结算任务承接->虚拟存在->任务节点
                    == 服务结算任务承接->任务->节点
                && 服务结算任务承接->状态
                    == 新任务治理承接状态::已读回
                && (服务结算任务承接->任务->当前状态 == 新任务状态::已建立
                    || 服务结算任务承接->任务->当前状态 == 新任务状态::等待条件);
            if (服务结算任务就绪) {
                const auto& 配 = *实现_->服务值结算配置;
                const auto 配置状态 = 实现_->任务服务->配置自我服务结算({
                    服务结算任务承接->任务->节点, 配.完成服务需求, 配.服务根需求,
                    配.自我存在, 配.自我概念节点, 配.服务值特征,
                    配.服务值特征类型节点, 配.能力});
                服务结算任务就绪 = 配置状态 == 新任务操作状态::已修改
                    || 配置状态 == 新任务操作状态::精确重复;
            }
            if (服务结算任务就绪) {
                锁.lock();
                实现_->服务值结算任务 = 服务结算任务承接->任务->节点;
                实现_->调度待触发 = true;
                锁.unlock();
            }
        }
        const auto 恢复执行 = 就绪
            ? 实现_->任务服务->查询等待结果任务执行()
            : std::optional<std::vector<新任务治理执行读回结果>>{};
        bool 恢复完整 = 就绪 && 服务结算任务就绪
            && 恢复执行.has_value();
        if (就绪 && 恢复执行) {
            锁.lock();
            for (const auto& 读回 : *恢复执行) {
                if (!读回.任务 || !读回.执行
                    || 读回.任务->节点 != 读回.执行->任务节点
                    || 读回.任务->最近执行实例节点 != 读回.执行->节点
                    || (读回.状态 != 新任务治理执行读回状态::已读回等待观察
                        && 读回.状态 != 新任务治理执行读回状态::已读回等待评估
                        && 读回.状态 != 新任务治理执行读回状态::已读回影响评估)) {
                    恢复完整 = false;
                    break;
                }
                实现_->登记结果等待已加锁(读回.任务->节点, 读回.执行->节点);
            }
            锁.unlock();
        }
        if (恢复完整 && 实现_->最终消费端口) {
            const auto 绑定 = 实现_->工作线程->绑定最终服务消费端口(*实现_->最终消费端口);
            if (绑定 != 新任务工作操作状态::已完成 && 绑定 != 新任务工作操作状态::无变化)
                throw std::logic_error("最终消费工作池端口装配失败");
        }
        const auto 工作启动 = 恢复完整
            ? 实现_->工作线程->启动({配置.邮箱容量}, 等待毫秒)
            : 新任务工作操作结果{
                新任务工作操作状态::尚未启动, 新任务工作生命周期::未启动};
        锁.lock();
        if (!恢复完整 || 实现_->停止
            || (工作启动.状态 != 新任务工作操作状态::已完成
                && 工作启动.状态 != 新任务工作操作状态::已在运行)) {
            实现_->停止源.request_stop();
            实现_->生命周期 = 新任务管理生命周期::启动失败;
            实现_->已完成 = true;
            实现_->变化.notify_all();
            锁.unlock();
            (void)实现_->工作线程->请求停止();
            (void)实现_->工作线程->等待停止(等待毫秒);
            锁.lock();
            return 实现_->操作结果(新任务管理操作状态::服务未就绪);
        }
        try { 实现_->线程 = std::thread([p = 实现_.get()] { p->运行(); }); }
        catch (...) {
            实现_->停止源.request_stop();
            实现_->生命周期 = 新任务管理生命周期::启动失败;
            实现_->已完成 = true;
            锁.unlock();
            (void)实现_->工作线程->请求停止();
            (void)实现_->工作线程->等待停止(等待毫秒);
            锁.lock();
            return 实现_->操作结果(新任务管理操作状态::资源失败);
        }
        if (!实现_->变化.wait_for(锁, 等待时长(等待毫秒), [this] {
            return 实现_->已进入 || 实现_->已完成;
        })) return 实现_->操作结果(新任务管理操作状态::等待超时);
        return 实现_->操作结果(实现_->已完成
            ? 新任务管理操作状态::内部错误 : 新任务管理操作状态::已完成);
    } catch (...) {
        实现_->停止源.request_stop();
        return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误};
    }
}

新任务操作状态 新_任务管理线程类::提交已确认复合子需求(
    const 新任务筹办反馈定位& 定位, const 新任务复合采用请求& 请求) noexcept {
    if (!实现_ || !定位.完整() || 定位.原工作.种类 != 新任务工作种类::查找方法
        || 请求.父任务节点 != 定位.任务节点) return 新任务操作状态::入口拒绝;
    try {
        const auto 定位有效=[&]() {
            const auto 最近=实现_->最近筹办结果.find(定位.任务节点.值);
            return !实现_->停止&&!实现_->已完成&&实现_->已进入
                &&!实现_->学习占用或旧筹办已失效已加锁(定位)
                &&最近!=实现_->最近筹办结果.end()&&最近->second.原工作==定位.原工作
                &&最近->second.复合目标提议
                &&*最近->second.复合目标提议==请求.原提议;
        };
        {std::lock_guard 锁(实现_->互斥);if(!定位有效())return 新任务操作状态::任务状态不允许;}
        const auto 核验 = 实现_->任务服务->核验复合子目标采用(请求);
        if (核验 != 新任务操作状态::已找到) return 核验;
        if (请求.原提议.状态 == 新任务复合分解状态::叶子目标
            && 请求.采用组.empty() && 请求.原提议.子目标组.empty()
            && 请求.原提议.父目标.目标.引用内容定位
            && 请求.原提议.父目标.目标.引用内容定位->正式内容路径.size() == 1) {
            const auto 父 = 实现_->任务服务->获取任务(定位.任务节点);
            if (!父 || 父->需求节点 != 请求.原提议.父需求节点
                || 父->当前状态 != 新任务状态::待查找方法) return 新任务操作状态::任务状态不允许;
            // 仅manager控制面软阻塞；与学习失效占用共用同一互斥。
            std::lock_guard 锁(实现_->互斥);
            if (!定位有效()) return 新任务操作状态::任务状态不允许;
            实现_->筹办阻塞任务.insert(定位.任务节点.值);
            实现_->等待目标复核.insert(定位.任务节点.值);
            实现_->登记等待来源已加锁(*父, 新任务等待原因::需求材料未到, 定位.原工作);
            实现_->待反馈任务.erase(定位.任务节点.值);
            实现_->变化.notify_all();
            return 新任务操作状态::已修改;
        }
        // 逐项承接是可重入的正式D→T操作；失败保留已经建立的前缀。
        for (const auto& 项 : 请求.采用组) {
            const auto 承接 = 实现_->任务服务->承接需求任务(项.子需求节点);
            if (承接.状态 == 新任务治理承接状态::资源失败) return 新任务操作状态::资源失败;
            if (承接.状态 != 新任务治理承接状态::已读回
                && 承接.状态 != 新任务治理承接状态::已重新启用
                && 承接.状态 != 新任务治理承接状态::既有终结任务
                && 承接.状态 != 新任务治理承接状态::需求当前已满足)
                return 新任务操作状态::任务状态不允许;
            if (承接.任务 && 承接.任务->需求节点 != 项.子需求节点)
                throw std::logic_error("复合子需求承接返回与请求不一致");
        }
        const auto 父 = 实现_->任务服务->获取任务(定位.任务节点);
        if (!父) return 新任务操作状态::任务状态不允许;
        if(父->当前状态==新任务状态::待查找方法) {
            const auto 推进=实现_->任务服务->修改任务状态(定位.任务节点,新任务状态::等待条件);
            if(推进!=新任务操作状态::已修改&&推进!=新任务操作状态::无变化)return 推进;
        }
        const auto 等待父=实现_->任务服务->获取任务(定位.任务节点);
        if(!等待父)return 新任务操作状态::任务状态不允许;
        std::lock_guard 锁(实现_->互斥);
        if(!定位有效())return 新任务操作状态::任务状态不允许;
        实现_->筹办阻塞任务.insert(定位.任务节点.值);
        实现_->等待目标复核.insert(定位.任务节点.值);
        实现_->登记等待来源已加锁(*等待父, 新任务等待原因::需求材料未到, 定位.原工作);
        实现_->待反馈任务.erase(定位.任务节点.值);
        实现_->变化.notify_all();
        return 新任务操作状态::已修改;
    } catch (const std::bad_alloc&) { return 新任务操作状态::资源失败; }
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复合子需求承接", L"正式复合采用发生工程错误");
    }
}

新任务承接接收结果 新_任务管理线程类::提交已确认条件子需求(
    const 新任务筹办反馈定位& 父筹办定位,
    const 新任务承接消息& 消息) noexcept {
    新任务承接接收结果 结果;
    if (!实现_) { 结果.状态 = 新任务承接接收状态::资源失败; return 结果; }
    if (!父筹办定位.完整()
        || 父筹办定位.原工作.种类 != 新任务工作种类::检查条件
        || !消息.消息编号 || !有效(消息.需求节点)
        || 消息.依据 != 新任务承接依据::自我条件子需求
        || 消息.来源状态节点 || 消息.外部要求节点 || 消息.行动请求来源) return 结果;
    try {
        // 先分配整个消息，再取得邮箱锁；失败不占用子D交接槽。
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息 = 消息;
        项->父筹办定位 = 父筹办定位;
        项->可处理时点 = std::chrono::steady_clock::now();
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            结果.状态 = 新任务承接接收状态::已停止接收; return 结果;
        }
        if (!实现_->已进入) {
            结果.状态 = 新任务承接接收状态::尚未启动; return 结果;
        }
        if (!实现_->子需求承接定位有效已加锁(*项)) {
            结果.状态 = 新任务承接接收状态::过期决议; return 结果;
        }
        auto& 记录 = 实现_->最近自我筹办裁决.at(父筹办定位.任务节点.值);
        const auto 位置 = std::find_if(记录.已确认子需求组.begin(),
            记录.已确认子需求组.end(), [&](const auto& 子项) {
                return 子项.需求节点 == 消息.需求节点;
            });
        const auto 序号 = static_cast<std::size_t>(
            std::distance(记录.已确认子需求组.begin(), 位置));
        if (序号 >= 记录.子需求承接组.size()) return 结果;
        if (const auto& 已有 = 记录.子需求承接组[序号]) {
            if (已有->消息 == 消息 && 已有->父筹办定位 == 父筹办定位) {
                结果.状态 = 新任务承接接收状态::精确重复;
                结果.处理结果 = 已有->结果;
            } else {
                结果.状态 = 新任务承接接收状态::身份冲突;
            }
            return 结果;
        }
        // 同种消息号不可在另一父任务或另一子D下复义。
        for (const auto& [任务值, 当前] : 实现_->最近自我筹办裁决) {
            (void)任务值;
            for (const auto& 已有 : 当前.子需求承接组) {
                if (已有 && 已有->消息.消息编号 == 消息.消息编号) {
                    结果.状态 = 新任务承接接收状态::身份冲突;
                    return 结果;
                }
            }
        }
        if (实现_->普通消息数量已加锁() >= 实现_->配置.邮箱容量) {
            结果.状态 = 新任务承接接收状态::邮箱已满; return 结果;
        }
        实现_->活动.push_back(项);
        记录.子需求承接组[序号] = 项;
        结果.状态 = 新任务承接接收状态::已接收;
        结果.处理结果 = 项->结果;
        实现_->变化.notify_all();
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务承接接收状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务承接接收状态::内部错误;
    }
    return 结果;
}

新任务承接接收结果 新_任务管理线程类::提交消息(const 新任务承接消息& 消息) noexcept {
    新任务承接接收结果 结果;
    if (!实现_) { 结果.状态 = 新任务承接接收状态::资源失败; return 结果; }
    if (!消息.消息编号 || !有效(消息.需求节点)) return 结果;
    const bool 是首次自动 = 消息.依据 == 新任务承接依据::自我首次复核;
    const bool 是状态自动 = 消息.依据 == 新任务承接依据::自我状态变化复核;
    const bool 是扫描补齐 = 消息.依据 == 新任务承接依据::自我全树扫描;
    if (消息.依据 == 新任务承接依据::外部服务要求 || 消息.外部要求节点 || 消息.行动请求来源) return 结果;
    if ((消息.依据 == 新任务承接依据::显式请求 && 消息.来源状态节点)
        || (是首次自动 && 消息.来源状态节点)
        || (是状态自动 && (!消息.来源状态节点 || !有效(*消息.来源状态节点)))
        || (是扫描补齐 && 消息.来源状态节点)
        || (消息.依据 != 新任务承接依据::显式请求
            && !是首次自动 && !是状态自动 && !是扫描补齐)) return 结果;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            结果.状态 = 新任务承接接收状态::已停止接收; return 结果;
        }
        if (!实现_->已进入) { 结果.状态 = 新任务承接接收状态::尚未启动; return 结果; }
        for (const auto& 项 : 实现_->活动) {
            if (项->消息.依据 != 消息.依据
                || 项->消息.消息编号 != 消息.消息编号) continue;
            if (项->消息 == 消息) {
                结果.状态 = 新任务承接接收状态::精确重复;
                结果.处理结果 = 项->结果;
            } else 结果.状态 = 新任务承接接收状态::身份冲突;
            return 结果;
        }
        if (是首次自动 || 是状态自动) {
            for (const auto& 记录 : 实现_->最近自动决议) {
                if (!记录 || 记录->消息.需求节点 != 消息.需求节点) continue;
                if (记录->消息.依据 == 消息.依据
                    && 记录->消息.来源状态节点 == 消息.来源状态节点) {
                    结果.状态 = 新任务承接接收状态::精确重复;
                    结果.处理结果 = 记录->结果;
                    return 结果;
                }
                if (是首次自动
                    || (记录->消息.来源状态节点 && 消息.来源状态节点
                        && 消息.来源状态节点->值 <= 记录->消息.来源状态节点->值)) {
                    结果.状态 = 新任务承接接收状态::过期决议;
                    return 结果;
                }
                break;
            }
            const auto 已占根数 = static_cast<std::size_t>(std::count_if(
                实现_->最近自动决议.begin(), 实现_->最近自动决议.end(),
                [](const auto& 记录) { return 记录.has_value(); }));
            const bool 已有本根 = std::any_of(
                实现_->最近自动决议.begin(), 实现_->最近自动决议.end(),
                [&](const auto& 记录) {
                    return 记录 && 记录->消息.需求节点 == 消息.需求节点;
                });
            if (!已有本根 && 已占根数 >= 实现_->最近自动决议.size()) {
                return 结果;
            }
        }
        if (实现_->普通消息数量已加锁() >= 实现_->配置.邮箱容量) {
            结果.状态 = 新任务承接接收状态::邮箱已满; return 结果;
        }
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息 = 消息;
        项->可处理时点 = std::chrono::steady_clock::now();
        实现_->活动.push_back(项);
        结果.处理结果 = 项->结果;
        结果.状态 = 新任务承接接收状态::已接收;
        实现_->变化.notify_all();
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = 新任务承接接收状态::资源失败; }
    catch (...) { 结果.状态 = 新任务承接接收状态::内部错误; }
    return 结果;
}

新任务承接接收结果 新_任务管理线程类::提交外部服务需求承接(const 新任务承接消息& 消息) noexcept {
    if (!实现_) return {新任务承接接收状态::资源失败, {}};
    if (!消息.消息编号 || !有效(消息.需求节点)
        || 消息.依据 != 新任务承接依据::外部服务要求
        || !消息.外部要求节点 || !有效(*消息.外部要求节点)
        || 消息.来源状态节点 || 消息.行动请求来源) return {};
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return {新任务承接接收状态::已停止接收, {}};
        if (!实现_->已进入) return {新任务承接接收状态::尚未启动, {}};
        for (const auto& 项 : 实现_->活动) {
            if (项->消息.依据 != 消息.依据 || 项->消息.消息编号 != 消息.消息编号) continue;
            if (项->消息 != 消息) return {新任务承接接收状态::身份冲突, {}};
            return {新任务承接接收状态::精确重复, 项->结果};
        }
        if (实现_->普通消息数量已加锁() >= 实现_->配置.邮箱容量)
            return {新任务承接接收状态::邮箱已满, {}};
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息 = 消息;
        项->可处理时点 = std::chrono::steady_clock::now();
        实现_->活动.push_back(项);
        实现_->变化.notify_all();
        return {新任务承接接收状态::已接收, 项->结果};
    } catch (const std::bad_alloc&) { return {新任务承接接收状态::资源失败, {}};
    } catch (...) {
        工程异常终止(L"新_任务管理线程类.cpp", __LINE__, L"提交外部服务需求承接", L"外部需求交接异常");
    }
}

新任务承接接收结果 新_任务管理线程类::提交行动请求需求承接(
    const 新任务承接消息& message) noexcept {
    if(!实现_)return {新任务承接接收状态::资源失败,{}};
    if(!message.消息编号||!有效(message.需求节点)||message.依据!=新任务承接依据::行动请求
        ||!message.行动请求来源||message.外部要求节点||message.来源状态节点)return {};
    const auto& source=*message.行动请求来源;const auto& key=source.需求来源.键;
    if(!有效(key.接收消息节点)||!有效(source.需求来源.采用含义节点)
        ||!有效(source.被认知需求节点)||!有效(source.来源人类节点)||!有效(source.受请求者节点)
        ||key.原文起点>=key.原文终点||key.根出现路径.empty()
        ||std::any_of(key.根出现路径.begin(),key.根出现路径.end(),[](auto n){return n<0;}))return {};
    try {
        std::lock_guard lock(实现_->互斥);
        if(实现_->停止||实现_->已完成)return {新任务承接接收状态::已停止接收,{}};
        if(!实现_->已进入)return {新任务承接接收状态::尚未启动,{}};
        for(const auto& item:实现_->活动) {
            if(item->消息.依据!=message.依据||item->消息.消息编号!=message.消息编号)continue;
            if(item->消息!=message)return {新任务承接接收状态::身份冲突,{}};
            return {新任务承接接收状态::精确重复,item->结果};
        }
        if(实现_->普通消息数量已加锁()>=实现_->配置.邮箱容量)return {新任务承接接收状态::邮箱已满,{}};
        const auto item=std::make_shared<实现::消息项>();
        item->消息=message;item->可处理时点=std::chrono::steady_clock::now();
        实现_->活动.push_back(item);实现_->变化.notify_all();
        return {新任务承接接收状态::已接收,item->结果};
    }catch(const std::bad_alloc&){return {新任务承接接收状态::资源失败,{}};}
    catch(const std::length_error&){return {新任务承接接收状态::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"提交行动请求需求承接",L"正式请求任务邮箱接纳不变量异常。");}
}

std::stop_token 新_任务管理线程类::读取停止令牌() const noexcept {
    if (!实现_) 工程异常终止(L"新_任务管理线程类.cpp", __LINE__, L"读取停止令牌", L"管理线程实现未建立");
    return 实现_->停止源.get_token();
}

新任务管理操作结果 新_任务管理线程类::请求停止() noexcept {
    if (!实现_) return {新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    实现_->停止源.request_stop();
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!实现_->创建已尝试) return 实现_->操作结果(新任务管理操作状态::尚未启动);
        if (实现_->停止 || 实现_->已完成) return 实现_->操作结果(新任务管理操作状态::无变化);
        实现_->停止 = true;
        实现_->生命周期 = 新任务管理生命周期::停止中;
        实现_->变化.notify_all();
        return 实现_->操作结果(新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

新任务管理操作结果 新_任务管理线程类::等待停止(std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!实现_->线程.joinable()) return 实现_->操作结果(实现_->已加入
            ? 新任务管理操作状态::无变化 : 新任务管理操作状态::尚未启动);
        if (实现_->线程.get_id() == std::this_thread::get_id())
            return 实现_->操作结果(新任务管理操作状态::不能等待自身);
        if (!实现_->变化.wait_for(锁, 等待时长(等待毫秒), [this] { return 实现_->已完成; }))
            return 实现_->操作结果(新任务管理操作状态::等待超时);
        锁.unlock();
        实现_->线程.join();
        锁.lock();
        实现_->已加入 = true;
        return 实现_->操作结果(实现_->生命周期 == 新任务管理生命周期::内部错误
            ? 新任务管理操作状态::内部错误 : 新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

bool 新_任务管理线程类::绑定方法学习读取(const 新_方法类& 方法) noexcept {
    if (!实现_) return false;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->学习方法读取==&方法) return true;
        if (实现_->学习方法读取 || 实现_->创建已尝试) return false;
        实现_->学习方法读取=&方法;
        return true;
    } catch (...) { return false; }
}

新方法学习接收状态 新_任务管理线程类::提交方法学习结束(
    const 新学习结束通知& 结果, std::uint64_t 交接编号) noexcept {
    if (!实现_) return 新方法学习接收状态::资源失败;
    if (!有效(结果.方法节点) || !交接编号
        || 交接编号 == (std::numeric_limits<std::uint64_t>::max)())
        return 新方法学习接收状态::入口拒绝;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return 新方法学习接收状态::已停止接收;
        if (!实现_->学习方法读取) return 新方法学习接收状态::入口拒绝;
        if (!实现_->已进入) return 新方法学习接收状态::尚未启动;
        const auto 旧 = 实现_->学习事件组.find(交接编号);
        if (旧 != 实现_->学习事件组.end())
            return 旧->second.通知.方法节点 == 结果.方法节点
                ? 新方法学习接收状态::精确重复 : 新方法学习接收状态::身份冲突;
        实现::学习事件 事件;
        事件.通知 = 结果;
        实现_->学习事件组.emplace(交接编号, std::move(事件));
        实现_->调度待触发 = true;
        实现_->变化.notify_all();
        return 新方法学习接收状态::已接收;
    } catch (const std::bad_alloc&) { return 新方法学习接收状态::资源失败; }
    catch (...) { return 新方法学习接收状态::内部错误; }
}

std::optional<新任务学习复核定位> 新_任务管理线程类::读取学习复核定位(
    std::uint64_t 复核编号) const noexcept {
    if (!实现_ || !复核编号) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->学习复核组.find(复核编号);
        if (实现_->停止 || 实现_->已完成 || 位置 == 实现_->学习复核组.end()
            || 位置->second.已终结) return std::nullopt;
        return 位置->second.定位;
    } catch (...) { return std::nullopt; }
}

std::optional<新任务等待复核定位> 新_任务管理线程类::读取等待复核定位(
    std::uint64_t 复核编号) const noexcept {
    if (!实现_ || !复核编号) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->等待复核组.find(复核编号);
        if (实现_->停止 || 实现_->已完成 || 位置 == 实现_->等待复核组.end()
            || 位置->second.已终结) return std::nullopt;
        return 位置->second.定位;
    } catch (...) { return std::nullopt; }
}

新自我等待复核接收状态 新_任务管理线程类::提交等待复核决议(
    const 新自我等待复核决议& 决议) noexcept {
    if (!实现_ || !决议.定位.完整()
        || 决议.状态 < 新自我等待复核状态::允许重新筹办
        || 决议.状态 > 新自我等待复核状态::需核查)
        return 新自我等待复核接收状态::入口拒绝;
    try {
        if (决议.状态 == 新自我等待复核状态::允许重新筹办) {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->停止 || 实现_->已完成) return 新自我等待复核接收状态::已停止接收;
            const auto p = 实现_->等待复核组.find(决议.定位.复核编号);
            if (p == 实现_->等待复核组.end()) return 新自我等待复核接收状态::定位已变化;
            if (p->second.定位 != 决议.定位) return 新自我等待复核接收状态::身份冲突;
            if (p->second.决议) return *p->second.决议 == 决议
                ? 新自我等待复核接收状态::精确重复 : 新自我等待复核接收状态::身份冲突;
            if (p->second.已终结 || !实现_->等待复核占用.contains(决议.定位.定位.任务节点.值))
                return 新自我等待复核接收状态::定位已变化;
        }
        std::optional<新需求筹办资格结果> fresh;
        if (决议.状态 == 新自我等待复核状态::允许重新筹办) {
            if (!决议.筹办资格 || 决议.筹办资格->状态 != 新需求筹办资格状态::允许筹办
                || !决议.筹办资格->需求 || 决议.筹办资格->需求->节点 != 决议.定位.定位.需求节点
                || !决议.筹办资格->当前满足 || !决议.当前满足状态
                || 决议.筹办资格->当前满足->状态 != *决议.当前满足状态)
                return 新自我等待复核接收状态::入口拒绝;
            fresh = 实现_->任务服务->复核任务筹办资格(决议.定位.定位.任务节点);
        }
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return 新自我等待复核接收状态::已停止接收;
        const auto 位置 = 实现_->等待复核组.find(决议.定位.复核编号);
        if (位置 == 实现_->等待复核组.end()) return 新自我等待复核接收状态::定位已变化;
        auto& 记录 = 位置->second;
        if (记录.定位 != 决议.定位) return 新自我等待复核接收状态::身份冲突;
        if (记录.决议) return *记录.决议 == 决议
            ? 新自我等待复核接收状态::精确重复 : 新自我等待复核接收状态::身份冲突;
        if (记录.已终结 || !实现_->等待复核占用.contains(决议.定位.定位.任务节点.值))
            return 新自我等待复核接收状态::定位已变化;
        if (fresh) {
            if (fresh->状态 == 新需求筹办资格状态::资源失败) return 新自我等待复核接收状态::资源失败;
            if (fresh->状态 == 新需求筹办资格状态::结构不一致
                || fresh->状态 == 新需求筹办资格状态::目标定义不合法
                || fresh->状态 == 新需求筹办资格状态::需求不存在) return 新自我等待复核接收状态::内部错误;
            // 后继按实时资格重新筹办；这里只固定同一需求目标，不冻结权重或当前差距。
            if (fresh->状态 != 新需求筹办资格状态::允许筹办 || !fresh->需求
                || fresh->需求->节点 != 决议.筹办资格->需求->节点
                || fresh->需求->目标存在节点 != 决议.筹办资格->需求->目标存在节点
                || fresh->需求->目标 != 决议.筹办资格->需求->目标)
                return 新自我等待复核接收状态::筹办材料已变化;
        }
        记录.决议 = 决议;
        实现_->变化.notify_all();
        return 新自我等待复核接收状态::已接收;
    } catch (const std::bad_alloc&) { return 新自我等待复核接收状态::资源失败; }
    catch (...) { return 新自我等待复核接收状态::内部错误; }
}

新自我学习复核接收状态 新_任务管理线程类::提交学习复核决议(
    const 新自我学习复核决议& 决议) noexcept {
    if (!实现_ || !决议.定位.完整()) return 新自我学习复核接收状态::入口拒绝;
    if (决议.状态 < 新自我学习复核状态::允许重新筹办
        || 决议.状态 > 新自我学习复核状态::需核查)
        return 新自我学习复核接收状态::入口拒绝;
    try {
        if (决议.状态 == 新自我学习复核状态::允许重新筹办) {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->停止 || 实现_->已完成) return 新自我学习复核接收状态::已停止接收;
            const auto p = 实现_->学习复核组.find(决议.定位.复核编号);
            if (p == 实现_->学习复核组.end()) return 新自我学习复核接收状态::定位已变化;
            if (p->second.定位 != 决议.定位) return 新自我学习复核接收状态::身份冲突;
            if (p->second.决议) return *p->second.决议 == 决议
                ? 新自我学习复核接收状态::精确重复 : 新自我学习复核接收状态::身份冲突;
            if (p->second.已终结 || !实现_->学习复核占用.contains(决议.定位.任务节点.值))
                return 新自我学习复核接收状态::定位已变化;
        }
        std::optional<新需求筹办资格结果> fresh;
        if (决议.状态 == 新自我学习复核状态::允许重新筹办) {
            if (!决议.筹办资格 || 决议.筹办资格->状态 != 新需求筹办资格状态::允许筹办
                || !决议.筹办资格->需求 || 决议.筹办资格->需求->节点 != 决议.定位.需求节点
                || !决议.筹办资格->当前满足 || !决议.当前满足状态
                || 决议.筹办资格->当前满足->状态 != *决议.当前满足状态)
                return 新自我学习复核接收状态::入口拒绝;
            fresh = 实现_->任务服务->复核任务筹办资格(决议.定位.任务节点);
        }
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return 新自我学习复核接收状态::已停止接收;
        const auto 位置 = 实现_->学习复核组.find(决议.定位.复核编号);
        if (位置 == 实现_->学习复核组.end()) return 新自我学习复核接收状态::定位已变化;
        auto& 记录 = 位置->second;
        if (记录.定位 != 决议.定位) return 新自我学习复核接收状态::身份冲突;
        if (记录.决议) return *记录.决议 == 决议
            ? 新自我学习复核接收状态::精确重复 : 新自我学习复核接收状态::身份冲突;
        if (记录.已终结 || !实现_->学习复核占用.contains(决议.定位.任务节点.值))
            return 新自我学习复核接收状态::定位已变化;
        if (fresh) {
            if (fresh->状态 == 新需求筹办资格状态::资源失败) return 新自我学习复核接收状态::资源失败;
            if (fresh->状态 == 新需求筹办资格状态::结构不一致
                || fresh->状态 == 新需求筹办资格状态::目标定义不合法
                || fresh->状态 == 新需求筹办资格状态::需求不存在) return 新自我学习复核接收状态::内部错误;
            // 学习只唤醒重新筹办，实际资格由后继按当前材料读取。
            if (fresh->状态 != 新需求筹办资格状态::允许筹办 || !fresh->需求
                || fresh->需求->节点 != 决议.筹办资格->需求->节点
                || fresh->需求->目标存在节点 != 决议.筹办资格->需求->目标存在节点
                || fresh->需求->目标 != 决议.筹办资格->需求->目标)
                return 新自我学习复核接收状态::筹办材料已变化;
        }
        记录.决议 = 决议;
        实现_->变化.notify_all();
        return 新自我学习复核接收状态::已接收;
    } catch (const std::bad_alloc&) { return 新自我学习复核接收状态::资源失败; }
    catch (...) { return 新自我学习复核接收状态::内部错误; }
}

新任务管理快照 新_任务管理线程类::读取快照() const noexcept {
    新任务管理快照 结果;
    if (!实现_) { 结果.生命周期 = 新任务管理生命周期::内部错误; return 结果; }
    try {
        std::lock_guard 锁(实现_->互斥);
        结果.生命周期 = 实现_->生命周期;
        结果.最近方法学习结束=实现_->最近方法学习结束;
        结果.最近学习运行异常=实现_->最近学习运行异常;
        结果.等待方法实例消费 = !实现_->已完成 && std::any_of(
            实现_->学习事件组.begin(), 实现_->学习事件组.end(),
            [](const auto& 项) { return !项.second.已终结; });
        结果.活动消息数 = 实现_->活动.size();
        结果.活动工作数 = 实现_->活动工作.size();
        结果.等待复核任务数 = 实现_->等待复核占用.size();
        结果.调度扫描在途 = std::any_of(实现_->活动工作.begin(), 实现_->活动工作.end(),
            [](const auto& 项) { return 项.请求.种类 == 新任务工作种类::读取任务调度材料; });
        for (const auto& [任务值, 项] : 实现_->调度材料) {
            if (项.状态 == 新任务调度项状态::普通可筹办
                && !实现_->筹办阻塞任务.contains(任务值)
                && !实现_->任务有执行保护已加锁(任务值)
                && !实现_->学习复核占用.contains(任务值)
                && !实现_->等待复核占用.contains(任务值)) ++结果.就绪工作数;
        }
        for (const auto& [任务值, 项] : 实现_->最近自我筹办裁决) {
            if (项.已完成接收 && 项.执行工作 && !项.执行工作已派发
                && !实现_->学习占用或旧筹办已失效已加锁(项.裁决.定位)
                && !std::any_of(实现_->活动工作.begin(), 实现_->活动工作.end(),
                    [&](const auto& 工作) { return 工作.请求.任务节点.值 == 任务值; })) ++结果.就绪工作数;
        }
        for (const auto& [任务值, 项] : 实现_->结果等待任务) {
            if (!项.已派发 && (项.待检测 || 实现_->结果通知待处理
                    || std::chrono::steady_clock::now() >= 项.下次检测时点)
                && !std::any_of(实现_->活动工作.begin(), 实现_->活动工作.end(),
                    [&](const auto& 工作) { return 工作.请求.任务节点.值 == 任务值; })) ++结果.就绪工作数;
        }
        结果.待反馈任务数 = 实现_->待反馈任务.size();
        结果.自我筹办裁决数 = static_cast<std::uint64_t>(std::count_if(
            实现_->最近自我筹办裁决.begin(),
            实现_->最近自我筹办裁决.end(), [](const auto& 项) {
                return 项.second.已完成接收;
            }));
        结果.执行结果数 = 实现_->最近执行结果.size();
        结果.停止已请求 = 实现_->停止;
        结果.线程已完成 = 实现_->已完成;
        if (实现_->服务值结算配置) {
            结果.服务值结算需求 =
                实现_->服务值结算配置->完成服务需求;
        }
        结果.服务值结算任务 = 实现_->服务值结算任务;
        结果.服务值结算方法已配置 = false;
        结果.服务值结算治理已配置 = 实现_->服务值结算配置.has_value()
            && 实现_->服务值结算任务.has_value();
    } catch (...) { 结果.生命周期 = 新任务管理生命周期::内部错误; }
    return 结果;
}

std::vector<新因果任务等待边> 新_任务管理线程类::读取因果等待边() const {
    if (!实现_) throw std::logic_error("因果等待组织尚未建立");
    return 实现_->读取因果等待边内部();
}

// 诊断责任：向上送出。完整边与工程取消分别返回，不交付部分边。
新因果等待边可停止结果 新_任务管理线程类::读取因果等待边(std::stop_token 停止) const {
    try {
        检查因果提交停止(停止);
        if (!实现_) throw std::logic_error("因果等待组织尚未建立");
        return 实现_->读取因果等待边内部(nullptr, 停止);
    } catch (const 新任务因果读取取消&) {
        return 新任务因果读取取消{};
    }
}

std::optional<新任务筹办处理结果> 新_任务管理线程类::读取最近筹办结果(
    稳定编码 任务节点) const noexcept {
    if (!实现_ || !有效(任务节点)) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->最近筹办结果.find(任务节点.值);
        return 位置 == 实现_->最近筹办结果.end()
                || 实现_->学习占用或旧筹办已失效已加锁({任务节点, 位置->second.原工作})
            ? std::nullopt
            : std::optional<新任务筹办处理结果>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新自我筹办裁决> 新_任务管理线程类::读取最近自我筹办裁决(
    稳定编码 任务节点) const noexcept {
    if (!实现_ || !有效(任务节点)) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->最近自我筹办裁决.find(任务节点.值);
        return 位置 == 实现_->最近自我筹办裁决.end()
                || !位置->second.已完成接收
            ? std::nullopt
            : std::optional<新自我筹办裁决>{位置->second.裁决};
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新任务执行处理结果> 新_任务管理线程类::读取最近执行结果(
    稳定编码 任务节点) const noexcept {
    if (!实现_ || !有效(任务节点)) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->最近执行结果.find(任务节点.值);
        return 位置 == 实现_->最近执行结果.end()
            ? std::nullopt
            : std::optional<新任务执行处理结果>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

新前置语言筹办接收结果 新_任务管理线程类::提交前置语言输入筹办(const 新任务前置语言输入请求& q) noexcept {
    using S=新任务工作接收状态;
    新前置语言筹办接收结果 out;
    if(!实现_||!有效(q.父任务节点)||!有效(q.父需求节点))return out;
    try {
        if(!实现_->任务服务->是前置语言材料父任务(q.父任务节点))return out;
        const auto t=实现_->任务服务->获取任务(q.父任务节点);
        if(!t||t->需求节点!=q.父需求节点)throw std::logic_error("material parent request inconsistent");
        新任务工作请求 w;
        {
            std::lock_guard 锁(实现_->互斥);
            if(实现_->停止||实现_->已完成){out.状态=S::已停止接收;return out;}
            if(!实现_->已进入||!实现_->工作线程){out.状态=S::尚未启动;return out;}
            for(const auto& item:实现_->活动工作) {
                if(item.请求.前置语言输入&&item.请求.前置语言输入->输入.来源==q.输入.来源) {
                    if(*item.请求.前置语言输入!=q){out.状态=S::身份冲突;return out;}
                    out.状态=S::精确重复;out.工作=item.请求;out.处理结果=item.结果;return out;
                }
            }
            for(const auto& 项:实现_->活动工作) {
                if(项.请求.任务节点==q.父任务节点){out.状态=S::任务已占用;return out;}
            }
            if(实现_->普通工作提交中.contains(q.父任务节点.值)
                ||实现_->任务有执行保护已加锁(q.父任务节点.值)
                ||实现_->待反馈任务.contains(q.父任务节点.值)
                ||实现_->学习复核占用.contains(q.父任务节点.值)
                ||实现_->等待复核占用.contains(q.父任务节点.值)
                ||实现_->有服务结算来源待处理已加锁(q.父任务节点.值)
                ||std::any_of(实现_->最终工作组.begin(),实现_->最终工作组.end(),[&](const auto& 项){
                    return 项.second.提交中&&项.second.工作.请求.任务节点==q.父任务节点;
                })){out.状态=S::任务已占用;return out;}
            if(实现_->活动工作.size()>=实现_->配置.邮箱容量){out.状态=S::邮箱已满;return out;}
            if(!实现_->下一工作编号||实现_->下一工作编号==(std::numeric_limits<std::uint64_t>::max)())throw std::logic_error("material work identity exhausted");
            w.工作编号=实现_->下一工作编号++;w.种类=新任务工作种类::筹办前置语言输入;w.任务节点=q.父任务节点;w.前置语言输入=q;
            out.工作=w;
        }
        const auto r=实现_->提交普通工作(w);out.状态=r.状态;
        if(r.状态==S::已接收||r.状态==S::精确重复)out.处理结果=r.处理结果;
        else out.工作.reset();
        return out;
    }catch(const std::bad_alloc&){out.状态=S::资源失败;return out;}
    catch(const std::length_error&){out.状态=S::资源失败;return out;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"接纳前置输入筹办",L"同工作接纳或父域结构异常");}
}
std::optional<新任务工作处理结果> 新_任务管理线程类::读取前置语言输入筹办结果(const 新任务工作请求& q) const noexcept {
    try {
        if(!实现_||q.种类!=新任务工作种类::筹办前置语言输入||!q.前置语言输入)return {};
        std::optional<新任务工作处理结果> r;
        {
            std::lock_guard 锁(实现_->互斥);
            const auto i=std::find_if(实现_->活动工作.begin(),实现_->活动工作.end(),[&](const auto& w){return w.请求.工作编号==q.工作编号;});
            if(i==实现_->活动工作.end()) {
                if(实现_->停止||实现_->已完成)return {};
                throw std::logic_error("accepted material work disappeared before acknowledgement");
            }
            if(i->请求!=q)throw std::logic_error("material readback request conflict");
            r=i->前置已读回;
        }
        if(r&&(!r->前置语言输入||r->原请求!=q||r->前置语言输入->原请求!=*q.前置语言输入
            ||!实现_->任务服务->是前置语言材料父任务(q.任务节点)))throw std::logic_error("material result readback corrupt");
        return r;
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取前置输入筹办",L"完整原结果或父域不变量异常");}
}
新任务工作操作状态 新_任务管理线程类::确认前置语言输入筹办已读回(const 新任务工作请求& q) noexcept {
    using S=新任务工作操作状态;
    try {
        if(!实现_||q.种类!=新任务工作种类::筹办前置语言输入||!q.前置语言输入)return S::入口拒绝;
        {
            std::lock_guard 锁(实现_->互斥);
            const auto i=std::find_if(实现_->活动工作.begin(),实现_->活动工作.end(),[&](const auto& w){return w.请求.工作编号==q.工作编号;});
            if(i==实现_->活动工作.end())return S::无变化;
            if(i->请求!=q||!i->前置已读回)throw std::logic_error("unread material work acknowledgement");
        }
        const auto ack=实现_->工作线程->确认结果已接收(q.工作编号,q.任务节点);
        if(ack!=S::已完成&&ack!=S::无变化)throw std::logic_error("material worker acknowledgement failed");
        std::lock_guard 锁(实现_->互斥);
        std::erase_if(实现_->活动工作,[&](const auto& w){return w.请求==q;});
        实现_->调度待触发=true;实现_->变化.notify_all();return S::已完成;
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"确认前置输入筹办",L"只读工作资源释放不变量异常");}
}

} // namespace 海中鱼巣
