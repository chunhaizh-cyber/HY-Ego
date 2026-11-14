#include "新_自我线程类.h"
#include "../业务/接收需求认知.服务.h"
#include "../业务/新_外设世界树类.h"
#include "../领域/新_方法类.h"
#include "../核心/日志系统.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <condition_variable>
#include <limits>
#include <map>
#include <set>
#include <mutex>
#include <new>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
namespace 海中鱼巣 { extern thread_local int 引用内容建立失败行; }
#endif
namespace 海中鱼巣 {

稳定编码 新版自我所在场景节点;
稳定编码 新版自我节点;

namespace {

std::mutex 唯一自我互斥;
新_自我线程类* 唯一自我线程所有者 = nullptr;
std::optional<新自我线程初始化请求> 唯一自我初始化请求;
std::atomic_uint64_t 下一自动决议消息编号{1};

#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
// 本切片只投影当前调用已经持有的材料；默认关闭，不参与任何业务裁决。
std::atomic_uint64_t 主线因果阶段日志条数{0};
#define HY_MAINLINE_STATE(项) case 状态::项: return HY_EGO_WIDEN(#项);
const wchar_t* 主线因果状态文字(新任务因果子需求提议状态 值) noexcept {
    using 状态 = 新任务因果子需求提议状态;
    switch (值) { HY_MAINLINE_STATE(已形成) HY_MAINLINE_STATE(无需子需求)
        HY_MAINLINE_STATE(材料已变化) HY_MAINLINE_STATE(无路径) HY_MAINLINE_STATE(路径待选择)
        HY_MAINLINE_STATE(资源延后)
        case 状态::能力未支持: return L"词条参与者尚未支持"; }
    return L"未知因果提议状态";
}
const wchar_t* 主线因果状态文字(二次特征概念操作状态 值) noexcept {
    using 状态 = 二次特征概念操作状态;
    switch (值) { HY_MAINLINE_STATE(已建立) HY_MAINLINE_STATE(已复用) HY_MAINLINE_STATE(已找到)
        HY_MAINLINE_STATE(未找到) HY_MAINLINE_STATE(入口拒绝) HY_MAINLINE_STATE(输出定义不合法)
        HY_MAINLINE_STATE(输入定义不合法) HY_MAINLINE_STATE(特征概念不存在) HY_MAINLINE_STATE(算法不适用)
        HY_MAINLINE_STATE(概念不存在) HY_MAINLINE_STATE(值域不合法) HY_MAINLINE_STATE(结构不一致)
        HY_MAINLINE_STATE(资源失败) }
    return L"未知二次特征概念操作状态";
}
const wchar_t* 主线因果状态文字(新需求操作状态 值) noexcept {
    using 状态 = 新需求操作状态;
    switch (值) { HY_MAINLINE_STATE(已建立) HY_MAINLINE_STATE(已复用) HY_MAINLINE_STATE(已找到)
        HY_MAINLINE_STATE(已修改) HY_MAINLINE_STATE(无变化) HY_MAINLINE_STATE(入口拒绝)
        HY_MAINLINE_STATE(尚未初始化) HY_MAINLINE_STATE(需求不存在) HY_MAINLINE_STATE(父需求不合法)
        HY_MAINLINE_STATE(目标存在不合法) HY_MAINLINE_STATE(目标形式不合法) HY_MAINLINE_STATE(目标特征类型不合法)
        HY_MAINLINE_STATE(目标值域不合法) HY_MAINLINE_STATE(比较关系不合法) HY_MAINLINE_STATE(目标方向不合法)
        HY_MAINLINE_STATE(双根定义冲突) HY_MAINLINE_STATE(任务来源未接通) HY_MAINLINE_STATE(会形成环)
        HY_MAINLINE_STATE(结构不一致) HY_MAINLINE_STATE(资源失败) HY_MAINLINE_STATE(权重不合法)
        HY_MAINLINE_STATE(执行域不适用) HY_MAINLINE_STATE(认知来源不合法) HY_MAINLINE_STATE(同来源目标冲突)
        HY_MAINLINE_STATE(需重新确认) HY_MAINLINE_STATE(前提已变化) HY_MAINLINE_STATE(目标内容依赖未就绪)
        HY_MAINLINE_STATE(目标内容未支持) }
    return L"未知需求操作状态";
}
const wchar_t* 主线因果状态文字(新自我筹办裁决接收状态 值) noexcept {
    using 状态 = 新自我筹办裁决接收状态;
    switch (值) { HY_MAINLINE_STATE(已接收) HY_MAINLINE_STATE(精确重复) HY_MAINLINE_STATE(定位已变化)
        HY_MAINLINE_STATE(身份冲突) HY_MAINLINE_STATE(已停止接收) HY_MAINLINE_STATE(入口拒绝)
        HY_MAINLINE_STATE(资源失败) HY_MAINLINE_STATE(内部错误) }
    return L"未知筹办裁决接收状态";
}
const wchar_t* 主线因果状态文字(新自我消息处理状态 值) noexcept {
    using 状态 = 新自我消息处理状态;
    switch (值) { HY_MAINLINE_STATE(已复核) HY_MAINLINE_STATE(入口拒绝) HY_MAINLINE_STATE(等待材料)
        HY_MAINLINE_STATE(比较失败) HY_MAINLINE_STATE(内部不一致) HY_MAINLINE_STATE(停止取消)
        HY_MAINLINE_STATE(内部错误) HY_MAINLINE_STATE(任务已承接) HY_MAINLINE_STATE(既有终结任务)
        HY_MAINLINE_STATE(任务管理未就绪) HY_MAINLINE_STATE(任务承接需核查) HY_MAINLINE_STATE(已交接未收回)
        HY_MAINLINE_STATE(任务筹办已裁决) HY_MAINLINE_STATE(任务筹办定位已变化) HY_MAINLINE_STATE(任务筹办需核查)
        HY_MAINLINE_STATE(任务结果已裁决) HY_MAINLINE_STATE(任务结果定位已变化) HY_MAINLINE_STATE(任务结果需核查)
        HY_MAINLINE_STATE(工程停止) HY_MAINLINE_STATE(学习复核已裁决) HY_MAINLINE_STATE(学习复核定位已变化)
        HY_MAINLINE_STATE(学习复核需核查) HY_MAINLINE_STATE(等待复核已裁决) HY_MAINLINE_STATE(等待复核定位已变化)
        HY_MAINLINE_STATE(等待复核需核查) HY_MAINLINE_STATE(任务完成贡献待归集)
        HY_MAINLINE_STATE(需求不合法未承接) }
    return L"未知自我消息处理状态";
}
#undef HY_MAINLINE_STATE
struct 主线因果阶段观察 final {
    const 新自我治理消息& 消息;
    const 新自我消息处理结果& 结果;
    const wchar_t* 最近阶段 = L"处理入口";
    std::optional<稳定编码> 父需求;
    void 记录(const wchar_t* 阶段, const wchar_t* 状态 = nullptr,
        std::optional<稳定编码> 子需求 = {}) noexcept {
        最近阶段 = 阶段;
        try {
            const auto 行 = 主线因果阶段日志条数.fetch_add(1, std::memory_order_relaxed);
            if (行 > 256) return;
            if (行 == 256) {
                (void)调试日志<调试切片::MAINLINE_PHASE>(L"自我线程", L"输出截断",
                    L"本进程自我因果阶段诊断已达256条；仅停止诊断，不限制业务推进。");
                return;
            }
            std::wostringstream 文本;
            文本 << L"阶段=" << 阶段 << L"；消息编号=" << 消息.消息编号;
            if (消息.筹办反馈) 文本 << L"；任务引用=" << 消息.筹办反馈->任务节点.值
                << L"；工作编号=" << 消息.筹办反馈->原工作.工作编号;
            if (父需求) 文本 << L"；父需求引用=" << 父需求->值;
            if (状态) 文本 << L"；返回状态=" << 状态;
            if (子需求) 文本 << L"；子需求引用=" << 子需求->值;
            文本 << L"；自我处理状态=" << 主线因果状态文字(结果.状态);
            if (结果.筹办裁决接收状态) 文本 << L"；裁决接收=" << 主线因果状态文字(*结果.筹办裁决接收状态);
            (void)调试日志<调试切片::MAINLINE_PHASE>(L"自我线程", L"处理因果子需求提议", 文本.str());
        } catch (...) {} // 诊断失败不能修改原分支、回执或返回结果。
    }
    ~主线因果阶段观察() noexcept {
        记录(L"函数返回", 主线因果状态文字(结果.状态));
    }
};
#define HY_SELF_CAUSAL_PHASE(阶段, 状态, 子需求) 观察.记录(阶段, 状态, 子需求)
#else
#define HY_SELF_CAUSAL_PHASE(阶段, 状态, 子需求) ((void)0)
#endif

#ifdef HY_EGO_DEBUG_LOG_CAUSAL_PROPOSAL_DIFF
// 仅比较本次已经持有的两份值式材料；不查名称/事实，不改变正式比较或分支。
void 记录因果提议差异(稳定编码 父任务, const 新任务因果子需求提议& 原,
    const 新任务因果子需求提议& 当前) noexcept {
    try {
        static std::atomic_uint64_t 条数{0};
        const auto 行 = 条数.fetch_add(1, std::memory_order_relaxed);
        if (行 > 32) return;
        if (行 == 32) {
            (void)调试日志<调试切片::CAUSAL_PROPOSAL_DIFF>(L"自我线程", L"输出截断",
                L"本进程仅保留32次提议差异观察；不限制实际筹办。");
            return;
        }
        std::wostringstream 文本;
        文本 << L"父任务引用=" << 父任务.值 << L"；整体相等=" << (原 == 当前)
            << L"；原状态=" << static_cast<int>(原.状态)
            << L"；新状态=" << static_cast<int>(当前.状态)
            << L"；子目标数量=" << 原.子需求项组.size() << L"->" << 当前.子需求项组.size();
        const auto 差异 = [&](const wchar_t* 字段, const auto& 左, const auto& 右) {
            if (左 != 右) 文本 << L"；差异=" << 字段;
        };
        差异(L"依据", 原.依据, 当前.依据);
        差异(L"候选依据组", 原.候选依据组, 当前.候选依据组);
        差异(L"路径选择", 原.路径选择未确定, 当前.路径选择未确定);
        差异(L"子目标组", 原.子需求项组, 当前.子需求项组);
        差异(L"继续推进任务组", 原.继续推进任务组, 当前.继续推进任务组);
        差异(L"未满足关系组", 原.未满足关系组, 当前.未满足关系组);
        if (原.依据 && 当前.依据) {
            const auto& 左 = *原.依据;
            const auto& 右 = *当前.依据;
            差异(L"请求父任务", 左.请求.父任务节点, 右.请求.父任务节点);
            差异(L"请求父需求", 左.请求.父需求节点, 右.请求.父需求节点);
            差异(L"请求目标存在", 左.请求.目标存在节点, 右.请求.目标存在节点);
            差异(L"请求目标", 左.请求.目标, 右.请求.目标);
            差异(L"请求通用查询", 左.请求.通用查询, 右.请求.通用查询);
            差异(L"原始等待边_不直接参与依据相等", 左.请求.当前等待边组, 右.请求.当前等待边组);
            差异(L"结算上下文有无", 左.请求.结算上下文.has_value(), 右.请求.结算上下文.has_value());
            差异(L"读回状态", 左.读回.状态, 右.读回.状态);
            差异(L"读回通用图", 左.读回.通用反查图, 右.读回.通用反查图);
            差异(L"读回主动方法", 左.读回.主动方法参考组, 右.读回.主动方法参考组);
            差异(L"读回静态缺口", 左.读回.静态状态缺口组, 右.读回.静态状态缺口组);
            差异(L"读回能力缺口", 左.读回.能力缺口组, 右.读回.能力缺口组);
            差异(L"读回分支", 左.读回.分支组, 右.读回.分支组);
            if (左.请求.结算上下文 && 右.请求.结算上下文) {
                const auto& a = *左.请求.结算上下文;
                const auto& b = *右.请求.结算上下文;
                差异(L"规则节点", a.所依据先天规则.节点, b.所依据先天规则.节点);
                差异(L"规则定义", a.所依据先天规则.定义, b.所依据先天规则.定义);
                差异(L"规则统计_不参与依据相等", a.所依据先天规则.统计, b.所依据先天规则.统计);
                差异(L"发生身份_不参与依据相等", a.所依据先天规则.发生身份组, b.所依据先天规则.发生身份组);
                差异(L"上下文世界", a.世界根节点, b.世界根节点);
                差异(L"上下文场景", a.目标场景节点, b.目标场景节点);
                差异(L"上下文自我", a.自我存在节点, b.自我存在节点);
                差异(L"上下文目标特征", a.目标特征节点, b.目标特征节点);
                差异(L"上下文目标值", a.当前目标值, b.当前目标值);
                差异(L"集合特征", a.服务任务集合特征节点, b.服务任务集合特征节点);
                差异(L"集合类型", a.服务任务集合类型, b.服务任务集合类型);
                差异(L"集合材料", a.集合材料节点, b.集合材料节点);
                差异(L"集合当前值", a.正式集合当前值, b.正式集合当前值);
                差异(L"任务成员组", a.正式任务成员组, b.正式任务成员组);
                差异(L"枚举完整", a.成员枚举完整, b.成员枚举完整);
                差异(L"任务范围", a.任务范围概念, b.任务范围概念);
                差异(L"候选事实组", a.候选事实组, b.候选事实组);
                文本 << L"；集合材料引用=" << a.集合材料节点.值 << L"->" << b.集合材料节点.值
                    << L"；成员数量=" << a.正式任务成员组.size() << L"->" << b.正式任务成员组.size();
                for (const auto& 旧 : a.候选事实组) {
                    const auto 位置 = std::find_if(b.候选事实组.begin(), b.候选事实组.end(),
                        [&](const auto& 新) { return 新.任务节点 == 旧.任务节点; });
                    if (位置 == b.候选事实组.end()) {
                        文本 << L"；首个已移除候选任务=" << 旧.任务节点.值;
                        break;
                    }
                    const auto& 新 = *位置;
                    if (旧 == 新) continue;
                    文本 << L"；首个变化候选任务=" << 旧.任务节点.值;
                    差异(L"候选读取状态", 旧.状态, 新.状态);
                    差异(L"候选任务引用", 旧.任务引用, 新.任务引用);
                    差异(L"候选需求", 旧.原需求节点, 新.原需求节点);
                    差异(L"候选目标存在", 旧.原目标存在节点, 新.原目标存在节点);
                    差异(L"候选目标", 旧.原目标, 新.原目标);
                    差异(L"候选需求活动", 旧.原需求活动状态, 新.原需求活动状态);
                    差异(L"候选任务状态", 旧.当前任务状态, 新.当前任务状态);
                    差异(L"候选服务值", 旧.当前服务值, 新.当前服务值);
                    差异(L"候选执行", 旧.执行实例节点, 新.执行实例节点);
                    差异(L"候选评估", 旧.影响评估节点, 新.影响评估节点);
                    差异(L"候选结算", 旧.完成结算节点, 新.完成结算节点);
                    差异(L"候选子接收", 旧.直接子接收节点组, 新.直接子接收节点组);
                    差异(L"候选人类锚点", 旧.人类确认锚点组, 新.人类确认锚点组);
                    差异(L"候选贡献", 旧.下层贡献证明组, 新.下层贡献证明组);
                    差异(L"候选治理边界", 旧.治理边界, 新.治理边界);
                    差异(L"候选消费", 旧.既有消费, 新.既有消费);
                    差异(L"候选世界", 旧.治理世界根节点, 新.治理世界根节点);
                    差异(L"候选场景", 旧.治理场景节点, 新.治理场景节点);
                    差异(L"候选条件", 旧.条件组, 新.条件组);
                    break;
                }
                for (const auto& 新 : b.候选事实组) {
                    if (std::none_of(a.候选事实组.begin(), a.候选事实组.end(),
                        [&](const auto& 旧) { return 旧.任务节点 == 新.任务节点; })) {
                        文本 << L"；首个新增候选任务=" << 新.任务节点.值;
                        break;
                    }
                }
            }
        }
        (void)调试日志<调试切片::CAUSAL_PROPOSAL_DIFF>(L"自我线程", L"fresh提议与原提议比较", 文本.str());
    } catch (...) {} // 包括相等运算中的材料复制失败；只丢诊断，不影响原筹办。
}
#endif

void 记录治理绑定拒绝(const wchar_t* 入口, const wchar_t* 阶段,
    const 新自我线程操作结果& 结果, int 上游状态 = -1) noexcept {
    try {
        std::wostringstream 文本;
        文本 << L"阶段=" << 阶段 << L"；状态=" << static_cast<int>(结果.状态)
            << L"；生命周期=" << static_cast<int>(结果.生命周期);
        if (上游状态 >= 0) 文本 << L"；上游状态=" << 上游状态;
        (void)记录运行日志(L"自我线程", 入口, 文本.str());
    } catch (...) {} // 观察输出失败不能改变已经确定的拒绝结果。
}

[[noreturn]] void 报告服务要求采用启动异常(const wchar_t* 阶段, int 状态,
    稳定编码 结算需求, 稳定编码 结算任务) noexcept {
    wchar_t 原因[256]{};
    (void)_snwprintf_s(原因, _TRUNCATE,
        L"阶段=%ls；状态=%d；结算需求=%llu；结算任务=%llu。来源初始化合同未完成，禁止开启治理。",
        阶段, 状态, static_cast<unsigned long long>(结算需求.值),
        static_cast<unsigned long long>(结算任务.值));
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"自我启动采用来源核验", 原因);
}

void 记录自我学习回流(const 新学习结束通知& 结果) noexcept {
    try {
        std::wostringstream 文本;
        // METHOD-LEARN-STATE：学习结束不再交接管理线程；等待方法知识的任务由方法状态唤醒。
        文本 << L"方法=" << 结果.方法节点.值;
        (void)记录运行日志(L"自我线程",L"模拟学习结束接收",文本.str());
    } catch (...) {}
}

void 记录自我学习运行异常(const 新学习运行异常记录& 异常) noexcept {
    try {
        std::wostringstream 文本;
        文本 << L"方法=" << 异常.方法节点.值 << L"；运行原因=" << static_cast<int>(异常.原因);
        if (!异常.诊断.empty()) {
            const auto 字节数 = static_cast<int>((std::min)(异常.诊断.size(), std::size_t{2048}));
            const auto 长度 = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                异常.诊断.data(), 字节数, nullptr, 0);
            if (长度 > 0) {
                std::wstring 说明(static_cast<std::size_t>(长度), L'\0');
                if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                    异常.诊断.data(), 字节数, 说明.data(), 长度) == 长度)
                    文本 << L"；诊断=" << 说明;
            }
        }
        if (异常.原因==新学习运行异常原因::停止取消)
            (void)记录运行日志(L"自我线程",L"学习停止取消",文本.str());
        else (void)记录逻辑错误日志(L"自我线程",L"学习运行异常",文本.str());
    } catch (...) {}
}

void 记录方法问题接收结束(const 新方法问题学习消息& 消息) noexcept {
    try {
        std::wostringstream 文本;
        文本<<L"方法="<<消息.方法节点.值<<L" 任务="<<消息.任务节点.值
            <<L" 执行="<<消息.执行实例节点.值;
        (void)记录运行日志(L"自我线程",L"方法问题材料接收结束",文本.str());
    } catch (...) {}
}

void 记录方法问题交付边界(稳定编码 任务,稳定编码 执行,
    新任务问题学习交付状态 状态) noexcept {
    try {
        std::wostringstream 文本;
        文本<<L"任务="<<任务.值<<L" 执行="<<执行.值<<L" 学习交付状态="<<static_cast<int>(状态)
            <<L"；不改变任务结论，未投递学习";
        if (状态==新任务问题学习交付状态::身份冲突 || 状态==新任务问题学习交付状态::结构不一致)
            (void)记录逻辑错误日志(L"自我线程",L"方法问题交付身份结构异常",文本.str());
        else (void)记录运行日志(L"自我线程",L"方法问题尚不可交付",文本.str());
    } catch (...) {}
}

struct 未完成自我建立前缀 final {
    新自我线程初始化请求 请求;
    稳定编码 场景节点;
    新场景成员建立结果 建立结果;
};

// 场景服务可能已经建立存在、但未能挂接全部初始状态。当前没有合法的
// 续挂入口，因此保留这一进程内前缀以阻止同一请求盲目再建第二个存在。
std::optional<未完成自我建立前缀> 唯一自我未完成前缀;

std::chrono::milliseconds 有界等待时长(std::uint64_t 等待毫秒) noexcept {
    using 表示类型 = std::chrono::milliseconds::rep;
    constexpr auto 最大等待 = (std::numeric_limits<表示类型>::max)();
    const auto 上限 = static_cast<std::uint64_t>(最大等待);
    return std::chrono::milliseconds{
        static_cast<表示类型>((std::min)(等待毫秒, 上限))};
}

bool 世界初始化成功(新世界初始化状态 状态) noexcept {
    return 状态 == 新世界初始化状态::已完成
        || 状态 == 新世界初始化状态::无变化;
}

bool 场景建立成功(新场景操作状态 状态) noexcept {
    return 状态 == 新场景操作状态::已完成
        || 状态 == 新场景操作状态::无变化;
}

bool 是I64最大值单点概念(
    const 概念_特征类& 特征概念服务,
    稳定编码 概念节点) noexcept {
    const auto 概念 = 特征概念服务.获取特征概念(概念节点);
    if (!概念 || 概念->材料类型 != 特征概念材料物理类型::I64标量) {
        return false;
    }
    const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(
        &概念->值域);
    constexpr auto 最大值 = (std::numeric_limits<std::int64_t>::max)();
    return 区间组 && 区间组->size() == 1
        && 区间组->front().下界 == 最大值
        && 区间组->front().上界 == 最大值;
}

std::optional<std::int64_t> 读取非负I64当前值(
    const 新_特征类& 特征服务,
    稳定编码 特征节点) noexcept {
    const auto 特征 = 特征服务.获取特征(特征节点);
    if (!特征 || !特征->当前值) return std::nullopt;
    const auto* 值 = std::get_if<std::int64_t>(&*特征->当前值);
    return 值 && *值 >= 0 ? std::optional<std::int64_t>{*值} : std::nullopt;
}

std::optional<std::int64_t> 读取服务I64当前值(
    const 新_特征类& 特征服务, 稳定编码 特征节点) noexcept {
    const auto 特征 = 特征服务.获取特征(特征节点);
    if (!特征 || !特征->当前值) return std::nullopt;
    const auto* 值 = std::get_if<std::int64_t>(&*特征->当前值);
    return 值 ? std::optional<std::int64_t>{*值} : std::nullopt;
}

新自我本能需求复核结果 复核自我本能需求(
    新_世界树类& 世界树服务,
    新_需求类& 需求服务,
    稳定编码 自我存在,
    稳定编码 安全需求,
    稳定编码 服务需求,
    std::optional<稳定编码> 来源状态) noexcept {
    新自我本能需求复核结果 结果;
    结果.自我存在 = 自我存在;
    结果.安全需求 = 安全需求;
    结果.服务需求 = 服务需求;
    结果.来源状态 = 来源状态;
    try {
        if (!世界树服务.已初始化()) {
            结果.状态 = 新自我需求复核状态::世界树未初始化;
            return 结果;
        }
        const auto 事实 = 世界树服务.获取事实服务引用();
        const auto 自我 = 事实.存在服务.获取存在(自我存在);
        const auto 安全 = 需求服务.获取需求(安全需求);
        const auto 服务 = 需求服务.获取需求(服务需求);
        if (!自我 || !安全 || !服务 || 安全需求 == 服务需求
            || 需求服务.查询根需求(新需求根角色::安全) != 安全需求
            || 需求服务.查询根需求(新需求根角色::服务) != 服务需求
            || 安全->根角色 != 新需求根角色::安全
            || 服务->根角色 != 新需求根角色::服务
            || 安全->目标存在节点 != 自我存在
            || 服务->目标存在节点 != 自我存在
            || 安全->目标.目标形式 != 新需求目标形式::值
            || 服务->目标.目标形式 != 新需求目标形式::值
            || 安全->目标.目标变化方向 || 服务->目标.目标变化方向
            || 安全->目标.目标特征类型概念节点 == 服务->目标.目标特征类型概念节点
            || !服务->目标.目标值域概念节点
            || !服务->目标.目标比较关系概念节点
            || !安全->目标.目标值域概念节点
            || !安全->目标.目标比较关系概念节点
            ) {
            return 结果;
        }
        if (来源状态) {
            const auto 来源 = 事实.状态服务.获取状态(*来源状态);
            if (!来源 || 来源->被描述存在节点 != 自我存在
                || 来源->强时间.语义 != 新状态时间语义::实例绝对UTC
                || 来源->强时间.纳秒 < 0) {
                结果.状态 = 新自我需求复核状态::来源状态不存在;
                return 结果;
            }
        }
        结果.安全结果 = 需求服务.复核当前满足(安全需求);
        结果.服务结果 = 需求服务.复核当前满足(服务需求);
        if (!结果.安全结果->实际特征节点
            || !结果.服务结果->实际特征节点
            || 结果.安全结果->实际特征节点
                == 结果.服务结果->实际特征节点) {
            结果.状态 = 新自我需求复核状态::需求比较失败;
            return 结果;
        }
        const auto 安全当前 = 读取非负I64当前值(
            事实.特征服务, *结果.安全结果->实际特征节点);
        const auto 服务当前 = 读取服务I64当前值(
            事实.特征服务, *结果.服务结果->实际特征节点);
        if (!安全当前 || !服务当前
            || !是I64最大值单点概念(
                事实.特征概念服务, *安全->目标.目标值域概念节点)
            || !是I64最大值单点概念(
                事实.特征概念服务, *服务->目标.目标值域概念节点)) {
            结果.状态 = 新自我需求复核状态::需求比较失败;
            return 结果;
        }
        constexpr auto 最大值 = (std::numeric_limits<std::int64_t>::max)();
        const auto 根结果一致 = [最大值](
            std::int64_t 当前,
            const 新需求当前满足结果& 根结果) {
            if (当前 < 0) throw std::overflow_error("root M-current cannot be represented by I64");
            if (当前 == 最大值) {
                return 根结果.状态 == 新需求当前满足状态::已满足
                    && 根结果.准确差异 == 0;
            }
            return 根结果.状态 == 新需求当前满足状态::需增加
                && 根结果.准确差异 == 最大值 - 当前;
        };
        if (!根结果一致(*安全当前, *结果.安全结果)
            || !根结果一致(*服务当前, *结果.服务结果)) {
            结果.状态 = 新自我需求复核状态::需求比较失败;
            return 结果;
        }
        结果.状态 = 新自我需求复核状态::已复核;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新自我需求复核状态::资源失败;
        return 结果;
    } catch (const std::length_error&) {结果.状态=新自我需求复核状态::资源失败;return 结果;}
    catch(const std::exception& 异常){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核自我本能需求",L"双根M目标或准确差复核发生工程异常",异常.what());}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核自我本能需求",L"双根M目标复核发生未知工程异常");}
}

新自我本能双根初始化结果 建立或取得自我本能双根(
    新_世界树类& 世界树服务,
    新_需求类& 需求服务,
    稳定编码 自我存在节点,
    const 新自我本能双根定义& 定义) noexcept {
    新自我本能双根初始化结果 结果;
    try {
        if (!世界树服务.已初始化()) {
            结果.状态 = 新自我本能双根初始化状态::世界树未初始化;
            return 结果;
        }
        const auto 事实 = 世界树服务.获取事实服务引用();
        if (!有效(自我存在节点)
            || !有效(定义.安全根.特征类型根概念节点)
            || !有效(定义.服务根.特征类型根概念节点)
            || !有效(定义.安全根.目标值域概念节点)
            || !有效(定义.安全根.目标比较关系概念节点)
            || !有效(定义.服务根.目标值域概念节点)
            || !有效(定义.服务根.目标比较关系概念节点)
            || 定义.安全根.初始权重 < 0
            || 定义.服务根.初始权重 < 0) {
            return 结果;
        }
        if (定义.安全根.特征类型根概念节点
                == 定义.服务根.特征类型根概念节点) {
            结果.状态 = 新自我本能双根初始化状态::双根定义冲突;
            return 结果;
        }
        const auto 自我 = 事实.存在服务.获取存在(自我存在节点);
        if (!自我) {
            结果.状态 = 新自我本能双根初始化状态::自我不存在;
            return 结果;
        }

        const auto 读取类型根 = [&](稳定编码 概念节点)
            -> std::optional<稳定编码> {
            const auto 关系 = 事实.特征概念服务.比较单特征概念(
                概念节点, 概念节点);
            if (关系.状态 != 特征概念规则状态::已完成
                || !关系.特征类型根) {
                return std::nullopt;
            }
            return 关系.特征类型根;
        };
        const auto 安全根 = 读取类型根(
            定义.安全根.特征类型根概念节点);
        const auto 服务根 = 读取类型根(
            定义.服务根.特征类型根概念节点);
        if (!安全根 || !服务根
            || *安全根 != 定义.安全根.特征类型根概念节点
            || *服务根 != 定义.服务根.特征类型根概念节点) {
            结果.状态 = 新自我本能双根初始化状态::特征类型根不合法;
            return 结果;
        }

        std::vector<稳定编码> 安全候选;
        std::vector<稳定编码> 服务候选;
        for (const auto 特征节点 : 自我->特征节点组) {
            const auto 特征 = 事实.特征服务.获取特征(特征节点);
            if (!特征) {
                结果.状态 = 新自我本能双根初始化状态::结构不一致;
                return 结果;
            }
            const auto 类型根 = 读取类型根(特征->特征概念节点);
            if (!类型根) {
                结果.状态 = 新自我本能双根初始化状态::结构不一致;
                return 结果;
            }
            if (*类型根 == *安全根) 安全候选.push_back(特征节点);
            if (*类型根 == *服务根) 服务候选.push_back(特征节点);
        }
        if (安全候选.empty()) {
            结果.状态 = 新自我本能双根初始化状态::安全特征不存在;
            return 结果;
        }
        if (服务候选.empty()) {
            结果.状态 = 新自我本能双根初始化状态::服务特征不存在;
            return 结果;
        }
        if (安全候选.size() != 1) {
            结果.状态 = 新自我本能双根初始化状态::安全特征不唯一;
            return 结果;
        }
        if (服务候选.size() != 1) {
            结果.状态 = 新自我本能双根初始化状态::服务特征不唯一;
            return 结果;
        }
        if (安全候选.front() == 服务候选.front()) {
            结果.状态 = 新自我本能双根初始化状态::双根定义冲突;
            return 结果;
        }
        if (!是I64最大值单点概念(
                事实.特征概念服务, 定义.安全根.目标值域概念节点)
            || !是I64最大值单点概念(
                事实.特征概念服务, 定义.服务根.目标值域概念节点)) {
            结果.状态 = 新自我本能双根初始化状态::根目标不是I64最大值;
            return 结果;
        }
        if (!读取非负I64当前值(事实.特征服务, 安全候选.front())
            || !读取服务I64当前值(事实.特征服务, 服务候选.front())) {
            结果.状态 = 新自我本能双根初始化状态::根当前值不合法;
            return 结果;
        }
        结果.安全实际特征节点 = 安全候选.front();
        结果.服务实际特征节点 = 服务候选.front();
        结果.需求双根结果 = 需求服务.建立或取得本能双根({
            {自我存在节点,
                {定义.安全根.特征类型根概念节点,
                    新需求目标形式::值,
                    定义.安全根.目标值域概念节点,
                    定义.安全根.目标比较关系概念节点,
                    std::nullopt},
                定义.安全根.初始权重},
            {自我存在节点,
                {定义.服务根.特征类型根概念节点,
                    新需求目标形式::值,
                    定义.服务根.目标值域概念节点,
                    定义.服务根.目标比较关系概念节点,
                    std::nullopt},
                定义.服务根.初始权重}});
        switch (结果.需求双根结果->状态) {
        case 新需求操作状态::已建立:
            结果.状态 = 新自我本能双根初始化状态::已建立;
            break;
        case 新需求操作状态::已复用:
            结果.状态 = 新自我本能双根初始化状态::已复用;
            break;
        case 新需求操作状态::双根定义冲突:
        case 新需求操作状态::权重不合法:
            结果.状态 = 新自我本能双根初始化状态::双根定义冲突;
            break;
        case 新需求操作状态::结构不一致:
        case 新需求操作状态::尚未初始化:
            结果.状态 = 新自我本能双根初始化状态::结构不一致;
            break;
        case 新需求操作状态::资源失败:
            结果.状态 = 新自我本能双根初始化状态::资源失败;
            break;
        default:
            结果.状态 = 新自我本能双根初始化状态::需求双根建立失败;
            break;
        }
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新自我本能双根初始化状态::资源失败;
        return 结果;
    } catch (const std::length_error&) {结果.状态=新自我本能双根初始化状态::资源失败;return 结果;}
    catch(const std::exception& 异常){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立或取得自我本能双根",L"双根M目标初始化发生工程异常，保留已发表事实",异常.what());}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立或取得自我本能双根",L"双根M目标初始化发生未知工程异常，保留已发表事实");}
}

} // namespace

bool 新自我本能双根初始化结果::成功() const noexcept {
    return (状态 == 新自我本能双根初始化状态::已建立
            || 状态 == 新自我本能双根初始化状态::已复用)
        && 安全实际特征节点.has_value()
        && 服务实际特征节点.has_value()
        && 需求双根结果.has_value()
        && (需求双根结果->状态 == 新需求操作状态::已建立
            || 需求双根结果->状态 == 新需求操作状态::已复用)
        && 需求双根结果->安全根.has_value()
        && 需求双根结果->服务根.has_value()
        && 需求双根结果->安全根列表项.has_value()
        && 需求双根结果->服务根列表项.has_value()
        && 有效(*安全实际特征节点)
        && 有效(*服务实际特征节点)
        && *安全实际特征节点 != *服务实际特征节点
        && 有效(*需求双根结果->安全根)
        && 有效(*需求双根结果->服务根)
        && *需求双根结果->安全根 != *需求双根结果->服务根
        && 有效(*需求双根结果->安全根列表项)
        && 有效(*需求双根结果->服务根列表项)
        && *需求双根结果->安全根列表项
            != *需求双根结果->服务根列表项;
}

bool 新自我治理基础结果::成功() const noexcept {
    return (状态 == 新自我治理基础状态::已建立
            || 状态 == 新自我治理基础状态::已复用)
        && 安全值特征类型根 && 服务值特征类型根
        && 安全值目标概念 && 服务值目标概念
        && 安全值比较类型根 && 服务值比较类型根
        && 安全值目标比较关系 && 服务值目标比较关系
        && 有效(*安全值特征类型根) && 有效(*服务值特征类型根)
        && 有效(*安全值目标概念) && 有效(*服务值目标概念)
        && 有效(*安全值比较类型根) && 有效(*服务值比较类型根)
        && 有效(*安全值目标比较关系) && 有效(*服务值目标比较关系)
        && *安全值特征类型根 != *服务值特征类型根
        && *安全值目标概念 != *服务值目标概念
        && *安全值比较类型根 != *服务值比较类型根
        && *安全值目标比较关系 != *服务值目标比较关系;
}

bool 新自我线程初始化请求::完整() const noexcept {
    if (既有自我所在场景 && 新场景父节点) return false;
    if (自我建立请求.父存在节点
        || 自我建立请求.初始特征组.empty()
        || 初始状态时间.语义 != 新状态时间语义::实例绝对UTC
        || 初始状态时间.纳秒 < 0
        || !有效(本能双根.安全根.特征类型根概念节点)
        || !有效(本能双根.服务根.特征类型根概念节点)
        || !有效(本能双根.安全根.目标值域概念节点)
        || !有效(本能双根.安全根.目标比较关系概念节点)
        || !有效(人类存在概念节点)
        || 本能双根.安全根.初始权重 < 0
        || 本能双根.服务根.初始权重 < 0
        || 本能双根.安全根.特征类型根概念节点
            == 本能双根.服务根.特征类型根概念节点) {
        return false;
    }
    if ((既有自我所在场景 && !有效(*既有自我所在场景))
        || (新场景父节点 && !有效(*新场景父节点))) {
        return false;
    }
    for (const auto& 特征 : 自我建立请求.初始特征组) {
        if (!有效(特征.特征概念节点)) return false;
    }
    return true;
}

bool operator==(
    const 新自我线程初始化请求& 左,
    const 新自我线程初始化请求& 右) noexcept {
    return 左.既有自我所在场景 == 右.既有自我所在场景
        && 左.新场景父节点 == 右.新场景父节点
        && 左.自我建立请求.父存在节点
            == 右.自我建立请求.父存在节点
        && 左.自我建立请求.初始特征组
            == 右.自我建立请求.初始特征组
        && 左.初始状态时间 == 右.初始状态时间
        && 左.本能双根 == 右.本能双根
        && 左.人类存在概念节点 == 右.人类存在概念节点;
}

bool 新自我线程初始化结果::成功() const noexcept {
    return (状态 == 新自我线程操作状态::已完成
            || 状态 == 新自我线程操作状态::无变化)
        && 世界树结果.has_value()
        && 世界初始化成功(世界树结果->状态)
        && 世界树结果->世界树根场景.has_value()
        && 世界树结果->根场景专属存在概念.has_value()
        && 有效(*世界树结果->世界树根场景)
        && 有效(*世界树结果->根场景专属存在概念)
        && 自我所在场景.has_value()
        && 自我存在.has_value()
        && 需求树根.has_value()
        && 安全根需求.has_value()
        && 服务根需求.has_value()
        && 完成服务需求.has_value()
        && 服务对象特征类型根.has_value()
        && 服务对象特征节点.has_value()
        && 服务对象比较类型根.has_value()
        && 服务对象目标比较关系.has_value()
        && 服务结算治理.has_value()
        && (服务结算治理->状态 == 新服务值结算协作状态::已完成
            || 服务结算治理->状态 == 新服务值结算协作状态::精确重复)
        && 服务结算治理->治理定义
        && 有效(服务结算治理->治理定义->节点)
        && 方法组织根.has_value()
        && 本能双根结果.has_value()
        && 本能双根结果->成功()
        && 有效(*自我所在场景)
        && 有效(*自我存在)
        && 有效(*需求树根)
        && 有效(*安全根需求)
        && 有效(*服务根需求)
        && 有效(*完成服务需求)
        && 有效(*服务对象特征类型根)
        && 有效(*服务对象特征节点)
        && 有效(*服务对象比较类型根)
        && 有效(*服务对象目标比较关系)
        && 有效(*方法组织根)
        && *需求树根 == 需求树根节点
        && *方法组织根 == 新方法组织根节点
        && *安全根需求
            == *本能双根结果->需求双根结果->安全根
        && *服务根需求
            == *本能双根结果->需求双根结果->服务根
        && 完成服务需求建立结果.has_value()
        && (完成服务需求建立结果->状态 == 新需求操作状态::已建立
            || 完成服务需求建立结果->状态 == 新需求操作状态::已复用)
        && 完成服务需求建立结果->需求节点 == 完成服务需求
        && 服务结算治理->结算需求节点 == 完成服务需求;
}

bool 新自我线程操作结果::成功() const noexcept {
    return 状态 == 新自我线程操作状态::已完成
        || 状态 == 新自我线程操作状态::无变化;
}

struct 接收认知让出最终 final {};

// R111：自我需求合法性判定的结果。判定依据属于自我意识，现阶段不规定。
enum class 自我需求合法性判定 : std::uint8_t { 合法 = 1, 不合法 = 2 };
// 发送点的承接依据：标明本次为哪一种需求向任务管理线程请求建立任务。
enum class 自我需求承接依据 : std::uint8_t {
    全树扫描 = 1, 本能根复核, 显式请求, 外部服务要求, 行动请求,
    条件子需求, 复合子需求, 因果子需求
};

struct 新_自我线程类::实现 final {
    实现(
        新_世界树类& 世界树,
        新_需求类& 需求,
        新_方法类& 方法,
        新_任务类& 任务) noexcept
        : 世界树服务(&世界树), 需求服务(&需求),
          方法服务(&方法), 任务服务(&任务) {}

    新_世界树类* 世界树服务 = nullptr;
    新_需求类* 需求服务 = nullptr;
    新_方法类* 方法服务 = nullptr;
    新_任务类* 任务服务 = nullptr;
    新服务值结算确认接口* 服务结算确认端口 = nullptr;
    接收需求认知服务* 接收认知 = nullptr;
    bool 接收认知待唤醒 = false;
    const 新_外设世界树类* 外设世界 = nullptr;
    稳定编码 外设场景标识类型{};
    稳定编码 外设存在标识类型{};
    bool 外设交互场景待补建 = false;
    std::chrono::steady_clock::time_point 最近接收认知推进{};
    std::mutex 治理基础互斥;
    新自我治理基础结果 治理基础缓存;
    mutable std::mutex 状态互斥;
    std::condition_variable 状态变化;
    std::atomic_bool 最终服务工作待唤醒{false};
    std::mutex 加入互斥;
    std::optional<新自我线程初始化请求> 锁定初始化请求;
    std::optional<新自我线程初始化结果> 初始化结果;
    std::thread 工作线程;
    新自我线程生命周期 生命周期 = 新自我线程生命周期::未初始化;
    bool 线程已进入 = false;
    bool 停止已请求 = false;
    std::stop_source 停止源;
    bool 线程已完成 = false;
    bool 已加入 = false;

    // DEP-READY-WAKE A：结构依赖未就绪（读取接口未装配、先天槽值不存在等）在运行期出现即装配缺陷，
    // 不重试、不轮询。本线程运行中进工程诊断链；已请求停止返回true，调用方按停止取消返回，不弹窗。
    bool 结构依赖未就绪已停止(const wchar_t* 入口, int 状态值, std::uint64_t 关联值 = 0) const noexcept {
        if (停止源.stop_requested()) return true;
        char 说明[128] = {};
        (void)std::snprintf(说明, sizeof(说明), "入口状态值=%d；关联=%llu",
            状态值, static_cast<unsigned long long>(关联值));
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, 入口, L"运行期结构依赖未就绪", 说明);
    }

    std::optional<新自我消息运行配置> 消息配置;
    std::optional<新自我本能需求复核结果> 首次需求复核;
    新_任务管理线程类* 任务管理 = nullptr;
    std::unique_ptr<新_自我工作线程类> 自我工作线程;
    模拟学习服务* 学习服务 = nullptr;
    方法问题学习服务* 问题学习服务 = nullptr;
    std::unique_ptr<新_学习线程类> 学习线程;
    enum class 学习交接阶段 : std::uint8_t { 等待结果, 待确认学习, 硬失败, 待重学, 待提交 };
    struct 词典学习请求者 final {
        稳定编码 任务, 需求, 候选方法;
        新需求完整目标 目标;
        std::vector<新任务依赖键> 等待键;
    };
    struct 词典学习待项 final {
        新方法学习请求 请求;
        std::vector<词典学习请求者> 请求者组;
    };
    struct 词典学习未命中 final {
        新方法学习请求 请求;
        词典学习请求者 请求者;
    };
    struct 学习占用 final {
        std::shared_future<新学习结束通知> 处理结果;
        学习交接阶段 阶段 = 学习交接阶段::等待结果;
        // 学习中途定义前提变化：已发表的部分配对不算方法更新，确认收回后退避重学（2026-11-06用户裁决）。
        bool 需重学 = false;
        std::optional<新学习结束通知> 通知;
        std::optional<新学习运行异常记录> 异常;
        std::chrono::steady_clock::time_point 可处理时点{};
        std::optional<新方法学习请求> 当前请求;
        std::vector<词典学习请求者> 请求者组;
        std::vector<词典学习待项> 待处理请求组;
        bool 定向结束已消费 = false;
    };
    std::map<稳定编码, 学习占用> 在途学习;
    std::vector<词典学习未命中> 词典未命中组;
    std::set<稳定编码> 已处理学习方法;
    std::optional<新学习结束通知> 最近学习结束;
    std::optional<新学习运行异常记录> 最近学习运行异常;
    struct 问题学习占用 final {
        新方法问题学习消息 消息;
        std::shared_future<新学习结束通知> 处理结果;
        std::chrono::steady_clock::time_point 可投递时点;
        bool 已消费=false;
    };
    // 键为执行实例节点；消息携带不可变执行材料共享指针。
    std::map<稳定编码,问题学习占用> 问题学习组;
    std::set<std::pair<稳定编码,稳定编码>> 已报告问题交付边界;
    新自我需求扫描阶段 需求扫描阶段 = 新自我需求扫描阶段::未启动;
    std::shared_future<新自我需求树扫描结果> 扫描工作结果;
    std::shared_future<新自我需求树扫描结果> 最近已消费检查结果;
    std::optional<新自我需求树扫描请求> 当前扫描请求;
    std::optional<std::uint64_t> 最近需求数量;
    std::optional<std::uint64_t> 最近任务数量;
    bool 关联复核待触发 = false;
    bool 扫描承接失败 = false;
    enum class 扫描恢复方式 : std::uint8_t { 无, 资源延后, 等待事件, 硬失败 };
    扫描恢复方式 扫描恢复 = 扫描恢复方式::无;
    std::optional<新自我工作种类> 待恢复工作种类;
    std::optional<std::chrono::steady_clock::time_point> 扫描重试时点;
    struct 扫描诊断 final {
        const wchar_t* 来源 = nullptr;
        int 状态 = 0;
        扫描恢复方式 恢复 = 扫描恢复方式::无;
        新自我需求树扫描请求 请求;
        std::uint64_t 消息编号 = 0;
        稳定编码 需求节点;
    };
    std::optional<扫描诊断> 最近扫描诊断;
    std::optional<扫描诊断> 待记录扫描诊断;

    struct 筹办反馈桥 final {
        std::mutex 互斥;
        实现* 目标 = nullptr;
    };
    std::shared_ptr<筹办反馈桥> 反馈桥;

    struct 关联检查通知桥 final {
        std::mutex 互斥;
        实现* 目标 = nullptr;
        std::atomic_bool 待复核{false};
    };
    std::shared_ptr<关联检查通知桥> 关联检查桥;

    bool 有关联变化通知() const noexcept {
        return 关联检查桥 && 关联检查桥->待复核.load();
    }

    // 调用方不能持有状态互斥；owner通知的锁序为owner→桥→self。
    void 解除关联检查通知() noexcept {
        const auto 桥 = 关联检查桥;
        if (!桥) return;
        {
            std::lock_guard 锁(桥->互斥);
            桥->目标 = nullptr;
        }
        需求服务->解除关联检查通知(桥.get());
        任务服务->解除关联检查通知(桥.get());
        关联检查桥.reset();
    }

    新自我线程操作状态 绑定关联检查通知() noexcept {
        try {
            if (关联检查桥) return 新自我线程操作状态::已完成;
            关联检查桥 = std::make_shared<关联检查通知桥>();
            关联检查桥->目标 = this;
            const std::weak_ptr<关联检查通知桥> 弱桥 = 关联检查桥;
            const auto 通知 = [弱桥] {
                if (const auto 桥 = 弱桥.lock()) {
                    // 必须先置位：即使等待self锁，正在消费的结果也能察觉变化。
                    桥->待复核.store(true);
                    std::lock_guard 桥锁(桥->互斥);
                    if (桥->目标) {
                        std::lock_guard 锁(桥->目标->状态互斥);
                        桥->目标->唤醒因果能力等待已加锁();
                        桥->目标->状态变化.notify_all();
                    }
                }
            };
            if (需求服务->绑定关联检查通知(关联检查桥.get(), 通知)
                && 任务服务->绑定关联检查通知(关联检查桥.get(), 通知))
                return 新自我线程操作状态::已完成;
            解除关联检查通知();
            return 新自我线程操作状态::入口拒绝;
        } catch (const std::bad_alloc&) {
            解除关联检查通知();
            return 新自我线程操作状态::资源失败;
        } catch (...) {
            解除关联检查通知();
            return 新自我线程操作状态::内部错误;
        }
    }

    struct 消息项 final {
        新自我治理消息 消息;
        std::promise<新自我消息处理结果> 承诺;
        std::shared_future<新自我消息处理结果> 结果 = 承诺.get_future().share();
        std::chrono::steady_clock::time_point 可处理时点{};
        std::shared_future<新任务承接处理结果> 管理结果;
        std::optional<新任务承接消息> 外部服务承接消息;
        std::optional<稳定编码> 行动请求自我需求;
        std::optional<新行动请求采用结果> 行动请求采用材料;
        std::optional<新外部服务需求采用结果> 外部服务采用材料;
        std::optional<新子需求采用回执> 条件子需求回执;
        bool 条件子需求回执已接收 = false;
        std::optional<新自我筹办裁决::因果采用回执> 因果子需求回执;
        bool 因果能力复核登记 = false, 因果能力期间事件 = false, 因果能力已停放 = false;
        std::vector<新因果能力缺口> 因果能力缺口组;
        std::vector<新任务承接消息> 条件子需求承接消息组;
        std::vector<std::shared_future<新任务承接处理结果>> 条件子需求承接结果组;
        std::vector<bool> 条件子需求承接完成组;
        std::optional<新自我等待复核决议> 等待决议;
        bool 贡献复核再次唤醒 = false;
        std::atomic_bool 贡献工作已交接阶段{false};
        std::optional<新任务工作请求> 贡献治理工作;
        std::shared_future<新任务工作处理结果> 贡献治理结果;
        std::optional<新本能函数本次条件结果对> 直接贡献结果;
        bool 直接贡献已确认 = false;
        std::atomic_bool 最终工作已交接阶段{false};
        std::optional<新自我消息处理结果> 贡献前置结果;
        bool 需求认知已读取 = false;
        std::optional<新本能已发布需求结果> 需求认知材料;
        std::optional<新服务值最终来源定位> 最终消费来源;
        std::optional<新任务工作请求> 最终消费工作;
        std::shared_future<新任务工作处理结果> 最终消费结果;
        std::optional<新服务值最终消费结果> 直接最终消费结果;
        bool 直接最终消费已确认 = false;
        std::optional<新任务复合采用请求> 复合子需求采用;
        // R111：复合合法项已被任务管理线程接纳；重投不合法通知时不再重复提交。
        bool 复合合法项已提交 = false;
        std::optional<新任务前置语言输入请求> 前置请求;
        std::optional<新任务工作请求> 前置工作;
        std::shared_future<新任务工作处理结果> 前置结果;
        std::set<稳定编码> 已处理内容学习方法;
    };
    // 处理中项仍留在活动组，接收方查重与容量计算不会漏算。
    std::vector<std::shared_ptr<消息项>> 活动消息;
    std::size_t 普通消息数量已加锁() const noexcept {
        return 活动消息.size();
    }
    // DEP-READY-WAKE B：外部输入投递的事实变化先入此转交队列，邮箱有容量时转为事实变化消息；
    // 邮箱暂满不丢弃，待已处理消息释放容量后转入。
    std::vector<稳定编码> 待转交事实变化;
    // 调用者持状态互斥。只做内存交接，不调用领域服务。
    void 转入待转交事实变化已加锁() {
        if (!消息配置) return;
        std::size_t 已转 = 0;
        while (已转 < 待转交事实变化.size() && 普通消息数量已加锁() < 消息配置->邮箱容量) {
            const auto 编号 = 取得自动消息编号();
            if (!编号) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"转交事实变化", L"自我消息编号已耗尽。");
            const auto 项 = std::make_shared<消息项>();
            项->消息.消息编号 = 编号;
            项->消息.类型 = 新自我消息类型::事实变化;
            项->消息.来源状态节点 = 待转交事实变化[已转];
            项->可处理时点 = std::chrono::steady_clock::now();
            活动消息.push_back(项);
            ++已转;
        }
        待转交事实变化.erase(待转交事实变化.begin(), 待转交事实变化.begin() + static_cast<std::ptrdiff_t>(已转));
    }

    struct 自动承接项 final {
        新任务承接消息 消息;
        std::chrono::steady_clock::time_point 可处理时点{};
        std::shared_future<新任务承接处理结果> 管理结果;
        bool 等待恢复事件 = false;
        const wchar_t* 最近扫描失败来源 = nullptr;
        int 最近扫描失败状态 = 0;
        std::uint8_t 最近扫描失败推进 = 0;
    };
    // 这是self内部治理进度，不是外部邮箱消息；每个本能根至多一项。
    std::array<std::optional<自动承接项>, 2> 自动承接;
    std::vector<自动承接项> 扫描承接;

    新自我线程操作结果 操作结果(
        新自我线程操作状态 状态) const noexcept {
        return {状态, 生命周期};
    }

    // R111：需求合法性判定。函数只读，不修改需求、任务或世界事实，不发消息；以后的
    // 判定规则只加在这里。诊断责任：无适用错误分支（现阶段常量返回“合法”）。
    自我需求合法性判定 判定需求合法性(
        稳定编码 需求节点, 自我需求承接依据 承接依据) const noexcept {
        (void)需求节点;
        (void)承接依据;
        return 自我需求合法性判定::合法;
    }

    struct 子需求合法性分组 final {
        std::vector<稳定编码> 不合法;
        std::vector<稳定编码> 恢复合法;
    };
    // 逐项判定；合法且仍记在父任务“不合法子需求”持久特征中的进入恢复组（R112，5260 §5.5第6、7项：
    // 以特征为准，重启后仍可改判恢复）。读取特征不持状态互斥。
    子需求合法性分组 判定子需求组(稳定编码 父任务,
        const std::vector<稳定编码>& 子需求组, 自我需求承接依据 依据) {
        子需求合法性分组 分组;
        std::vector<稳定编码> 合法;
        for (const auto 节点 : 子需求组) {
            if (判定需求合法性(节点, 依据) == 自我需求合法性判定::不合法)
                分组.不合法.push_back(节点);
            else 合法.push_back(节点);
        }
        if (合法.empty()) return 分组;
        const auto 记录 = 任务服务->读取任务不合法子需求(父任务);
        if (!记录) return 分组; // 父任务已不存在：无可恢复的挂起依据。
        for (const auto 节点 : 合法)
            if (std::find(记录->begin(), 记录->end(), 节点) != 记录->end()) 分组.恢复合法.push_back(节点);
        return 分组;
    }

    enum class 子需求通知推进 : std::uint8_t { 已送达, 重投, 已停止 };
    // 接纳失败（资源、尚未启动）由调用方按既有重投间隔重投。挂起依据由任务管理线程经任务owner
    // 写入父任务“不合法子需求”特征，本线程不另存记录。入口拒绝或内部错误属工程故障。
    子需求通知推进 发送子需求合法性通知(
        bool 不合法, const 新子需求合法性通知& 通知) noexcept {
        try {
            const auto 接收 = 不合法
                ? 任务管理->提交子需求不合法通知(通知)
                : 任务管理->提交子需求恢复合法通知(通知);
            switch (接收) {
            case 新子需求合法性通知接收状态::已接收:
            case 新子需求合法性通知接收状态::精确重复:
            case 新子需求合法性通知接收状态::任务状态已变化: break;
            case 新子需求合法性通知接收状态::资源失败: return 子需求通知推进::重投;
            case 新子需求合法性通知接收状态::已停止接收: return 子需求通知推进::已停止;
            default:
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"发送子需求合法性通知",
                    L"任务管理线程拒收子需求合法性通知。");
            }
            return 子需求通知推进::已送达;
        } catch (const std::bad_alloc&) {
            return 子需求通知推进::重投; // 重投由接收方幂等吸收。
        }
    }

    // 有父任务的发送点共用：先补发恢复合法，再对不合法项发不合法通知。
    // 返回空表示全部合法且无待发通知，调用方照常提交；否则为本次推进结论。
    std::optional<子需求通知推进> 通知子需求合法性(const 新任务筹办反馈定位& 定位,
        const 子需求合法性分组& 分组, const 新子需求不合法候选& 候选) noexcept {
        try {
            新子需求合法性通知 通知;
            通知.父任务定位 = 定位;
            通知.候选 = 候选;
            if (!分组.恢复合法.empty()) {
                通知.子需求节点组 = 分组.恢复合法;
                const auto 推进 = 发送子需求合法性通知(false, 通知);
                if (推进 != 子需求通知推进::已送达) return 推进;
            }
            if (分组.不合法.empty()) return std::nullopt;
            通知.子需求节点组 = 分组.不合法;
            return 发送子需求合法性通知(true, 通知);
        } catch (const std::bad_alloc&) { return 子需求通知推进::重投; }
    }

    新自我本能需求复核结果 复核需求(
        std::optional<稳定编码> 来源 = std::nullopt) noexcept {
        // 初始化结果在启动后不再修改；绝不持状态互斥调用领域服务。
        return 复核自我本能需求(*世界树服务, *需求服务,
            *初始化结果->自我存在, *初始化结果->安全根需求,
            *初始化结果->服务根需求, 来源);
    }

    enum class 方法学习发现状态 : std::uint8_t {
        已接纳 = 1, 已在途, 已处理仍无配对, 已有配对, 无需学习, 暂缓, 停止取消
    };
    static void 合并词典请求者(std::vector<词典学习请求者>& 组, 词典学习请求者 请求者) {
        const auto 已有 = std::find_if(组.begin(), 组.end(), [&](const auto& 项) {
            return 项.任务 == 请求者.任务 && 项.候选方法 == 请求者.候选方法;
        });
        if (已有 == 组.end()) 组.push_back(std::move(请求者));
        else *已有 = std::move(请求者);
    }

    static void 合并词典待项(std::vector<词典学习待项>& 组,
        const 新方法学习请求& 请求, 词典学习请求者 请求者) {
        const auto 已有 = std::find_if(组.begin(), 组.end(), [&](const auto& 项) { return 项.请求 == 请求; });
        if (已有 != 组.end()) 合并词典请求者(已有->请求者组, std::move(请求者));
        else 组.push_back({请求, {std::move(请求者)}});
    }

    新词典学习发现结果 读取当前词典学习发现(稳定编码 T, 稳定编码 M) const {
        auto 结果 = 任务服务->查询词条材料定向学习发现(T, M);
        if (结果.状态 == 新词典学习发现状态::资源失败) throw std::bad_alloc{};
        if (结果.状态 == 新词典学习发现状态::结构依赖未就绪) {
            (void)结构依赖未就绪已停止(L"词典学习发现", static_cast<int>(结果.状态), T.值);
        }
        if (结果.状态 == 新词典学习发现状态::已读取 && !结果.发现)
            throw std::logic_error("dictionary discovery missing current material");
        return 结果;
    }

    bool 词典输入已未命中(const 新方法学习请求& 请求, const 词典学习请求者& 请求者) {
        std::lock_guard 锁(状态互斥);
        // 只保留该请求者当前目标及实际联合输入的未命中记忆。
        std::erase_if(词典未命中组, [&](const auto& 项) {
            return 项.请求者.任务 == 请求者.任务 && 项.请求者.候选方法 == 请求者.候选方法
                && (项.请求 != 请求 || 项.请求者.需求 != 请求者.需求 || 项.请求者.目标 != 请求者.目标);
        });
        return std::any_of(词典未命中组.begin(), 词典未命中组.end(), [&](const auto& 项) {
            return 项.请求 == 请求 && 项.请求者.任务 == 请求者.任务
                && 项.请求者.候选方法 == 请求者.候选方法 && 项.请求者.需求 == 请求者.需求
                && 项.请求者.目标 == 请求者.目标;
        });
    }

    void 刷新词典请求者等待(const 词典学习请求者& 原请求者, const 新词典学习发现* 当前) {
        if (当前 && 原请求者.等待键 == 当前->等待依赖键组) return;
        const auto 结果 = 任务管理->请求词典等待依赖刷新(原请求者.任务);
        if (结果.状态 == 新任务管理操作状态::资源失败) throw std::bad_alloc{};
        if (结果.状态 != 新任务管理操作状态::已完成 && 结果.状态 != 新任务管理操作状态::无变化
            && !停止源.stop_requested())
            throw std::logic_error("dictionary dependency refresh rejected");
    }

    方法学习发现状态 接纳词典学习发现(稳定编码 候选, const 新词典学习发现& 发现) {
        using S = 方法学习发现状态;
        if (发现.处置 != 新词典学习处置::可提交学习) return S::无需学习;
        if (!发现.学习请求 || !发现.学习请求->定向输入)
            throw std::logic_error("dictionary discovery missing full learning request");
        词典学习请求者 请求者{发现.任务节点, 发现.需求节点, 候选, 发现.当前目标, 发现.等待依赖键组};
        if (词典输入已未命中(*发现.学习请求, 请求者)) return S::已处理仍无配对;
        std::lock_guard 锁(状态互斥);
        if (停止已请求) return S::停止取消;
        if (!学习线程) return S::暂缓;
        const auto [位置, 新建] = 在途学习.try_emplace(发现.方法节点);
        auto& 槽 = 位置->second;
        if (新建) 槽.阶段 = 学习交接阶段::待提交;
        if (槽.当前请求 == 发现.学习请求) {
            合并词典请求者(槽.请求者组, std::move(请求者));
            槽.定向结束已消费 = false;
        } else 合并词典待项(槽.待处理请求组, *发现.学习请求, std::move(请求者));
        状态变化.notify_all();
        return 新建 ? S::已接纳 : S::已在途;
    }

    bool 准备下一词典学习(稳定编码 方法, 学习占用& 槽) {
        // 本函数只更新主循环持有的局部槽，不向map旧槽重新挂接后继。
        std::vector<词典学习待项> 当前组;
        for (const auto& 待项 : 槽.待处理请求组) {
            for (const auto& 请求者 : 待项.请求者组) {
                const auto 读 = 读取当前词典学习发现(请求者.任务, 请求者.候选方法);
                const auto* 当前 = 读.发现 ? &*读.发现 : nullptr;
                刷新词典请求者等待(请求者, 当前);
                if (!当前 || 当前->处置 != 新词典学习处置::可提交学习
                    || 当前->方法节点 != 方法) continue;
                if (!当前->学习请求 || !当前->学习请求->定向输入)
                    throw std::logic_error("dictionary pending discovery missing input");
                词典学习请求者 新请求者{当前->任务节点, 当前->需求节点, 请求者.候选方法,
                    当前->当前目标, 当前->等待依赖键组};
                if (!词典输入已未命中(*当前->学习请求, 新请求者))
                    合并词典待项(当前组, *当前->学习请求, std::move(新请求者));
            }
        }
        槽.待处理请求组 = std::move(当前组);
        if (槽.待处理请求组.empty()) return true;
        const auto& 下项 = 槽.待处理请求组.front();
        // 全部分配在提交前完成，取得承诺后只移动或写入无抛字段。
        槽.当前请求 = 下项.请求;
        槽.请求者组 = 下项.请求者组;
        const auto 接收 = 学习线程->提交学习(*槽.当前请求);
        if (接收.状态 == 新学习操作状态::已完成 || 接收.状态 == 新学习操作状态::精确重复) {
            if (!接收.处理结果.valid()) throw std::logic_error("dictionary accepted without future");
            槽.处理结果 = 接收.处理结果;
            槽.待处理请求组.erase(槽.待处理请求组.begin());
            槽.定向结束已消费 = false;
            槽.阶段 = 学习交接阶段::等待结果;
            return false;
        }
        槽.当前请求.reset();
        槽.请求者组.clear();
        if (接收.处理结果.valid()) throw std::logic_error("dictionary deferred with unrelated future");
        if (接收.状态 == 新学习操作状态::同方法在途 || 接收.状态 == 新学习操作状态::资源失败
            || 接收.状态 == 新学习操作状态::尚未启动) return false;
        if (接收.状态 == 新学习操作状态::已停止接收 && 停止源.stop_requested()) return false;
        throw std::logic_error("dictionary learning request rejected");
    }

    void 消费词典学习结束(稳定编码 方法, 学习占用& 槽) {
        if (!槽.当前请求 || !槽.当前请求->定向输入 || 槽.定向结束已消费) return;
        if (槽.通知 && !槽.通知->定向结束)
            throw std::logic_error("dictionary completion missing directed outcome");
        const auto 结局 = 槽.通知 ? 槽.通知->定向结束->结局 : 新定向学习结局::需重新组织输入;
        bool 允许当前输入续办 = false;
        if (槽.通知) {
            switch (结局) {
            case 新定向学习结局::需重新组织输入:
            case 新定向学习结局::条件未具备:
            case 新定向学习结局::请求已无需要:
                允许当前输入续办 = true;
                break;
            case 新定向学习结局::已完成学习:
            case 新定向学习结局::已被现有知识覆盖:
            case 新定向学习结局::已有空结果配对:
            case 新定向学习结局::正式未命中:
                break;
            default: throw std::logic_error("dictionary learning outcome unsupported");
            }
        } else {
            允许当前输入续办 = 槽.需重学 || (槽.异常 && 槽.异常->原因 == 新学习运行异常原因::资源不足);
        }
        for (const auto& 请求者 : 槽.请求者组) {
            if (结局 == 新定向学习结局::正式未命中
                && !词典输入已未命中(*槽.当前请求, 请求者)) {
                std::lock_guard 锁(状态互斥);
                词典未命中组.push_back({*槽.当前请求, 请求者});
            }
            const auto 读 = 读取当前词典学习发现(请求者.任务, 请求者.候选方法);
            const auto* 当前 = 读.发现 ? &*读.发现 : nullptr;
            刷新词典请求者等待(请求者, 当前);
            if (!当前 || 当前->处置 != 新词典学习处置::可提交学习 || 当前->方法节点 != 方法) continue;
            if (!当前->学习请求 || !当前->学习请求->定向输入)
                throw std::logic_error("dictionary completion discovery missing input");
            词典学习请求者 新请求者{当前->任务节点, 当前->需求节点, 请求者.候选方法,
                当前->当前目标, 当前->等待依赖键组};
            // 重组须重读完整F/Q等实时材料，联合引用未变也可能需要重新准备。
            // 当前仍缺条件时不排队；覆盖、空R及同输入未命中沿原等待/选择收束。
            if ((允许当前输入续办 || *当前->学习请求 != *槽.当前请求 || 当前->当前目标 != 请求者.目标)
                && !词典输入已未命中(*当前->学习请求, 新请求者))
                合并词典待项(槽.待处理请求组, *当前->学习请求, std::move(新请求者));
        }
        槽.定向结束已消费 = true;
        槽.需重学 = false;
    }

    方法学习发现状态 发现方法学习(稳定编码 方法节点) noexcept {
        using S = 方法学习发现状态;
        bool 本次预留 = false;
        bool 本次标记 = false;
        bool 已接纳 = false;
        try {
            auto 方法=方法服务->获取方法(方法节点);
            // 筹办反馈可能定位任务副本；学习只补抽象方法，不改冻结副本。
            if (方法 && 方法->来源方法节点) 方法=方法服务->获取方法(*方法->来源方法节点);
            if (!方法) return S::无需学习;
            if (方法->来源方法节点 || 方法->实例根节点
                || 方法->启用状态 != 新方法启用状态::启用
                || 方法->实现方式 != 新方法实现方式::本能) return S::无需学习;
            if (!方法->本能函数登记名 || 方法->本能函数登记名->empty())
                throw std::logic_error("正式本能方法缺少登记名");
            const auto 事实 = 世界树服务->获取事实服务引用();
            const auto 词典类型 = 事实.特征概念服务.读取当前先天特征类型槽(先天特征类型槽::词典材料);
            if (词典类型.状态 == 特征概念规则状态::资源失败) return S::暂缓;
            if (词典类型.类型根 && std::any_of(方法->登记结果组.begin(), 方法->登记结果组.end(),
                [&](const auto& 结果) { return 结果.参数序号 == 1 && 结果.特征类型概念节点 == *词典类型.类型根; }))
                return S::无需学习;
            if (!方法->条件结果组.empty()) return S::已有配对;
            方法节点=方法->节点;
            std::unique_lock 锁(状态互斥);
            if (停止已请求) return S::停止取消;
            if (在途学习.contains(方法节点)) return S::已在途;
            if (已处理学习方法.contains(方法节点)) return S::已处理仍无配对;
            if (!学习线程) return S::暂缓;
            // 先留接收槽再提交，避免工作已经开始而self分配槽失败丢结果。
            const auto 插入=在途学习.try_emplace(方法节点).second;
            if (!插入) return S::已在途;
            本次预留 = true;
            本次标记 = 已处理学习方法.insert(方法节点).second;
            if (!本次标记)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"预留方法学习", L"同锁检查后学习去重身份意外已存在。");
            锁.unlock();
            const auto 接收=学习线程->提交学习({方法节点,{}});
            已接纳 = 接收.状态==新学习操作状态::已完成 || 接收.状态==新学习操作状态::精确重复;
            if (已接纳 && !接收.处理结果.valid())
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交方法学习", L"学习线程已接纳但未交付有效future。");
            锁.lock();
            const auto 位置 = 在途学习.find(方法节点);
            if (位置 == 在途学习.end())
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交方法学习", L"本次学习接收槽在交接期间丢失。");
            if (已接纳) {
                位置->second.处理结果=接收.处理结果;
                return S::已接纳;
            } else {
                if (接收.处理结果.valid())
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交方法学习", L"未接纳回执意外交付有效future。");
                在途学习.erase(方法节点);
                已处理学习方法.erase(方法节点);
                本次预留 = false;
                本次标记 = false;
                const 新学习运行异常记录 异常{方法节点,
                    接收.状态==新学习操作状态::资源失败 ? 新学习运行异常原因::资源不足
                    : 接收.状态==新学习操作状态::入口拒绝 ? 新学习运行异常原因::输入不合法
                    : 新学习运行异常原因::内部错误};
                最近学习运行异常=异常;
                锁.unlock();
                记录自我学习运行异常(异常);
                if (接收.状态 == 新学习操作状态::入口拒绝) {
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交方法学习", L"合法抽象方法被学习入口意外拒绝。");
                }
                if (接收.状态 != 新学习操作状态::已停止接收
                    && 接收.状态 != 新学习操作状态::尚未启动
                    && 接收.状态 != 新学习操作状态::资源失败)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交方法学习", L"学习接收返回非预期工程状态。");
                return 接收.状态 == 新学习操作状态::已停止接收 ? S::停止取消 : S::暂缓;
            }
        } catch (const std::bad_alloc&) {
            if (已接纳)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"接纳方法学习", L"有效学习承诺交接发生工程异常，禁止撤销接纳。");
            新学习运行异常记录 异常{方法节点,新学习运行异常原因::内部错误};
            异常.原因=新学习运行异常原因::资源不足;
            try {
                std::lock_guard 锁(状态互斥);
                if (本次预留) 在途学习.erase(方法节点);
                if (本次标记) 已处理学习方法.erase(方法节点);
                最近学习运行异常=异常;
            } catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"撤销学习预留", L"未接纳学习的本次预留无法可靠撤销。");
            }
            记录自我学习运行异常(异常);
            return S::暂缓;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"发现方法学习", L"正式方法或学习承诺发生工程错误，禁止作为业务否定处理。");
        }
    }

    方法学习发现状态 发现候选方法学习(稳定编码 T, 稳定编码 M,
        std::vector<新任务依赖键>& 等待键, std::optional<新词典学习处置>& 词典处置) noexcept {
        try {
            const auto 读 = 读取当前词典学习发现(T, M);
            if (!读.发现) return 读.状态 == 新词典学习发现状态::不涉及
                ? 发现方法学习(M) : 方法学习发现状态::无需学习;
            const auto& 当前 = *读.发现;
            词典处置 = 当前.处置;
            等待键.insert(等待键.end(), 当前.等待依赖键组.begin(), 当前.等待依赖键组.end());
            return 接纳词典学习发现(M, 当前);
        } catch (const std::bad_alloc&) { return 方法学习发现状态::暂缓; }
        catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"发现候选词典学习", L"正式发现或接纳材料发生工程错误。");
        }
    }

    bool 有学习结果已就绪() const noexcept {
        return std::any_of(在途学习.begin(),在途学习.end(),[](const auto& 项) {
            if (项.second.阶段 == 学习交接阶段::待提交)
                return 项.second.可处理时点 <= std::chrono::steady_clock::now();
            return 项.second.阶段 == 学习交接阶段::等待结果 && 项.second.处理结果.valid()
                && 项.second.处理结果.wait_for(std::chrono::milliseconds(0))
                ==std::future_status::ready;
        }) || std::any_of(问题学习组.begin(),问题学习组.end(),[](const auto& 项) {
            return !项.second.已消费 && 项.second.处理结果.valid()
                && 项.second.处理结果.wait_for(std::chrono::milliseconds(0))==std::future_status::ready;
        });
    }

    // 操作局部副本，调用者不持self状态锁；接纳和worker确认是独立阶段。
    bool 推进学习交接(稳定编码 方法, 学习占用& 项) noexcept {
        const auto 记异常 = [&](新学习运行异常原因 原因, const std::string& 诊断 = {}) {
            项.异常 = 新学习运行异常记录{方法, 原因, 诊断};
            记录自我学习运行异常(*项.异常);
        };
        if (项.阶段 == 学习交接阶段::待提交) {
            项.可处理时点 = std::chrono::steady_clock::now()
                + 有界等待时长(消息配置->资源重试间隔毫秒);
            try { return 准备下一词典学习(方法, 项); }
            catch (const std::bad_alloc&) { return false; }
            catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"领取定向词典学习", L"当前输入组织或承诺交接发生工程错误。");
            }
        }
        if (项.阶段 == 学习交接阶段::等待结果) {
            try {
                const auto 通知 = 项.处理结果.get();
                if (通知.方法节点 != 方法) {
                    记异常(新学习运行异常原因::结构不一致);
                    项.阶段 = 学习交接阶段::待确认学习;
                } else {
                    // METHOD-LEARN-STATE：学习结果由方法owner形成方法状态并唤醒等待任务；这里只确认学习收回。
                    项.通知 = 通知;
                    记录自我学习回流(通知);
                    项.阶段 = 学习交接阶段::待确认学习;
                }
            } catch (const 新学习运行异常& 异常) {
                记异常(异常.记录().方法节点 == 方法
                    ? 异常.记录().原因 : 新学习运行异常原因::结构不一致, 异常.记录().诊断);
                项.需重学 = 异常.记录().方法节点 == 方法
                    && 异常.记录().原因 == 新学习运行异常原因::定义前提已变化;
                项.阶段 = 学习交接阶段::待确认学习;
            } catch (const std::bad_alloc&) {
                记异常(新学习运行异常原因::资源不足);
                项.阶段 = 学习交接阶段::待确认学习;
            } catch (...) {
                记异常(新学习运行异常原因::内部错误);
                项.阶段 = 学习交接阶段::待确认学习;
            }
        }
        项.可处理时点 = std::chrono::steady_clock::now()
            + 有界等待时长(消息配置->资源重试间隔毫秒);
        if (项.阶段 == 学习交接阶段::待重学) {
            const auto 接收 = 学习线程->提交学习({方法,{}});
            if (接收.状态 == 新学习操作状态::已完成 || 接收.状态 == 新学习操作状态::精确重复) {
                if (!接收.处理结果.valid()) {
                    记异常(新学习运行异常原因::内部错误);
                    项.阶段 = 学习交接阶段::硬失败;
                    return false;
                }
                项.处理结果 = 接收.处理结果;
                项.需重学 = false;
                项.阶段 = 学习交接阶段::等待结果;
                return false;
            }
            if (接收.状态 == 新学习操作状态::资源失败 || 接收.状态 == 新学习操作状态::尚未启动)
                return false;
            记异常(接收.状态 == 新学习操作状态::已停止接收
                ? 新学习运行异常原因::停止取消 : 新学习运行异常原因::内部错误);
            项.阶段 = 学习交接阶段::硬失败;
            return false;
        }
        if (项.阶段 == 学习交接阶段::待确认学习) {
            try { 消费词典学习结束(方法, 项); }
            catch (const std::bad_alloc&) { return false; }
            catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"收回定向词典学习", L"当前请求者读取或等待刷新发生工程错误。");
            }
            const auto 确认 = 学习线程->确认结果已接收(方法);
            if ((确认 == 新学习操作状态::已完成 || 确认 == 新学习操作状态::无变化) && 项.需重学) {
                // 原工作已收回；保留在途槽，按资源重试间隔退避后重新提交同一方法。
                项.阶段 = 学习交接阶段::待重学;
                return false;
            }
            if (确认 == 新学习操作状态::已完成 || 确认 == 新学习操作状态::无变化)
                return true;
            if (确认 == 新学习操作状态::资源失败 || 确认 == 新学习操作状态::尚未启动)
                return false;
            记异常(确认 == 新学习操作状态::已停止接收
                ? 新学习运行异常原因::停止取消 : 新学习运行异常原因::内部错误);
            项.阶段 = 学习交接阶段::硬失败;
        }
        return false;
    }

    void 发现方法问题学习(稳定编码 任务,稳定编码 执行) noexcept {
        if (!问题学习服务 || !学习线程) return;
        try {
            const auto 回执=任务服务->读取执行问题学习回执(任务,执行);
            if (回执.状态!=新任务问题学习交付状态::已交付 || !回执.消息) {
                if (回执.状态==新任务问题学习交付状态::无问题
                    || 回执.状态==新任务问题学习交付状态::尚未评估) return;
                bool 首次=false;
                {
                    std::lock_guard 锁(状态互斥);
                    首次=已报告问题交付边界.emplace(任务,执行).second;
                }
                if (首次) 记录方法问题交付边界(任务,执行,
                    回执.状态==新任务问题学习交付状态::已交付
                        ? 新任务问题学习交付状态::结构不一致 : 回执.状态);
                return;
            }
            const auto& 消息=*回执.消息;
            if (消息.任务节点!=任务 || 消息.执行实例节点!=执行
                || !有效(消息.方法节点) || !消息.材料) {
                记录自我学习运行异常({消息.方法节点,新学习运行异常原因::结构不一致});
                return;
            }
            std::lock_guard 锁(状态互斥);
            if (停止已请求) return;
            const auto [位置,插入]=问题学习组.try_emplace(消息.执行实例节点,
                问题学习占用{消息,{},std::chrono::steady_clock::now(),false});
            if (!插入 && 位置->second.消息!=消息)
                记录自我学习运行异常({消息.方法节点,新学习运行异常原因::结构不一致});
            状态变化.notify_all();
        } catch (...) {
            // 旁路收集设施异常不能改变普通任务的满足、完成或结算。
            (void)记录逻辑错误日志(L"自我线程",L"方法问题消息暂存失败",L"本次执行材料未投递学习");
        }
    }

    std::optional<std::chrono::steady_clock::time_point> 最近问题投递时点() const noexcept {
        std::optional<std::chrono::steady_clock::time_point> 时点;
        for (const auto& [执行,项]:问题学习组) {
            (void)执行;
            if (项.已消费 || 项.处理结果.valid()) continue;
            if (!时点 || 项.可投递时点<*时点) 时点=项.可投递时点;
        }
        return 时点;
    }

    std::optional<std::chrono::steady_clock::time_point> 合并后台推进时点已加锁(
        std::optional<std::chrono::steady_clock::time_point> 时点) const noexcept {
        const auto 待投 = 最近问题投递时点();
        if (待投 && (!时点 || *待投 < *时点)) 时点 = 待投;
        const auto 扫描时点 = 最近扫描推进时点已加锁();
        if (扫描时点 && (!时点 || *扫描时点 < *时点)) 时点 = 扫描时点;
        for (const auto& [方法, 项] : 在途学习) {
            (void)方法;
            if (项.阶段 == 学习交接阶段::待确认学习
                || 项.阶段 == 学习交接阶段::待重学 || 项.阶段 == 学习交接阶段::待提交)
                if (!时点 || 项.可处理时点 < *时点) 时点 = 项.可处理时点;
        }
        return 时点;
    }

    static std::uint64_t 取得自动消息编号() noexcept {
        auto 编号 = 下一自动决议消息编号.load(std::memory_order_relaxed);
        while (编号 && 编号 != (std::numeric_limits<std::uint64_t>::max)()) {
            if (下一自动决议消息编号.compare_exchange_weak(
                    编号, 编号 + 1, std::memory_order_relaxed)) return 编号;
        }
        return 0; // 永久耗尽，不回绕、不换键重试。
    }

    // 2026-11-11 用户裁决：暂停需求树全树扫描（兜底补齐与正式条件子需求承接争抢同一需求）。
    // 恢复时改回 false；暂停期间不提交扫描工作、不发“自我全树扫描”承接消息。
    static constexpr bool 全树扫描已暂停 = true;

    bool 提交需求工作已加锁(新自我工作种类 种类) noexcept {
        if constexpr (全树扫描已暂停) { (void)种类; return false; }
        if (当前扫描请求 || 扫描工作结果.valid()) return false;
        关联复核待触发 = true;
        新自我需求树扫描请求 请求;
        请求.工作编号 = 取得自动消息编号();
        请求.种类 = 种类;
        if (!自我工作线程) {
            安排扫描恢复已加锁(扫描恢复方式::硬失败, 种类);
            登记扫描诊断已加锁(L"工作线程缺失", 0, 扫描恢复, 请求);
            return false;
        }
        // 只在提交前清先前信号；提交期间及之后到来的变化必须留给下一次。
        if (关联检查桥) (void)关联检查桥->待复核.exchange(false);
        const auto 接收 = 自我工作线程->提交工作(请求);
        if ((接收.状态 != 新自我工作接收状态::已接收
                && 接收.状态 != 新自我工作接收状态::精确重复)
            || !接收.处理结果.valid()) {
            const auto 恢复 = 接收.状态 == 新自我工作接收状态::资源失败
                ? 扫描恢复方式::资源延后
                : 接收.状态 == 新自我工作接收状态::尚未启动
                    ? 扫描恢复方式::等待事件 : 扫描恢复方式::硬失败;
            安排扫描恢复已加锁(恢复, 种类);
            登记扫描诊断已加锁(L"提交扫描工作", static_cast<int>(接收.状态), 恢复, 请求);
            return false;
        }
        当前扫描请求 = 请求;
        关联复核待触发 = false;
        扫描工作结果 = 接收.处理结果;
        扫描恢复 = 扫描恢复方式::无;
        待恢复工作种类.reset();
        扫描重试时点.reset();
        需求扫描阶段 = 新自我需求扫描阶段::等待扫描结果;
        return true;
    }

    void 登记扫描诊断已加锁(const wchar_t* 来源, int 状态, 扫描恢复方式 恢复,
        const 新自我需求树扫描请求& 请求, const 新任务承接消息* 消息 = nullptr) noexcept {
        const 扫描诊断 诊断{来源, 状态, 恢复, 请求,
            消息 ? 消息->消息编号 : 0, 消息 ? 消息->需求节点 : 稳定编码{}};
        // 同一失败反复资源重试不刷日志；新工作编号并不是新的失败种类。
        if (最近扫描诊断 && 最近扫描诊断->来源 == 来源
            && 最近扫描诊断->状态 == 状态 && 最近扫描诊断->恢复 == 恢复
            && 最近扫描诊断->请求.种类 == 请求.种类
            && 最近扫描诊断->消息编号 == 诊断.消息编号) return;
        最近扫描诊断 = 诊断;
        待记录扫描诊断 = 诊断;
    }

    static void 记录扫描诊断(const 扫描诊断& 诊断,
        const std::shared_future<新自我需求树扫描结果>& 报告) noexcept {
        try {
            std::wostringstream 文本;
            文本 << L"工作=" << 诊断.请求.工作编号
                << L" 种类=" << static_cast<int>(诊断.请求.种类)
                << L" 消息=" << 诊断.消息编号 << L" 需求=" << 诊断.需求节点.值
                << L" 来源=" << 诊断.来源 << L" 状态=" << 诊断.状态
                << L" 处理=" << (诊断.恢复 == 扫描恢复方式::资源延后 ? L"资源延后重试"
                    : 诊断.恢复 == 扫描恢复方式::等待事件 ? L"保留原请求等待有效治理事件"
                    : 诊断.恢复 == 扫描恢复方式::硬失败 ? L"硬失败，不自动恢复"
                    : L"保留关联异常报告，等待新变化");
            if (报告.valid() && 报告.get().原请求 == 诊断.请求) {
                const auto& 结果 = 报告.get();
                文本 << L" 需求数=" << 结果.需求数量 << L" 任务数=" << 结果.任务数量
                    << L" 缺任务数=" << 结果.缺少任务的需求节点组.size()
                    << L" 多任务需求数=" << 结果.多任务需求节点组.size()
                    << L" 孤立任务数=" << 结果.无需求任务节点组.size();
                if (!结果.多任务需求节点组.empty())
                    文本 << L" 首个多任务需求=" << 结果.多任务需求节点组.front().值;
                if (!结果.无需求任务节点组.empty())
                    文本 << L" 首个孤立任务=" << 结果.无需求任务节点组.front().值;
            }
            if (诊断.恢复 == 扫描恢复方式::硬失败)
                (void)记录逻辑错误日志(L"自我线程", L"需求扫描恢复边界", 文本.str());
            else (void)记录运行日志(L"自我线程", L"需求扫描恢复边界", 文本.str());
        } catch (...) {}
    }

    void 安排扫描恢复已加锁(扫描恢复方式 恢复, 新自我工作种类 种类) noexcept {
        扫描恢复 = 恢复;
        待恢复工作种类 = 种类;
        扫描重试时点.reset();
        需求扫描阶段 = 新自我需求扫描阶段::失败;
        if (恢复 == 扫描恢复方式::资源延后)
            扫描重试时点 = std::chrono::steady_clock::now()
                + 有界等待时长(消息配置->资源重试间隔毫秒);
    }

    std::optional<std::chrono::steady_clock::time_point> 最近扫描推进时点已加锁() const noexcept {
        auto 时点 = 扫描重试时点;
        if (需求扫描阶段 == 新自我需求扫描阶段::等待任务承接) {
            for (const auto& 项 : 扫描承接) {
                if (!项.等待恢复事件 && (!时点 || 项.可处理时点 < *时点))
                    时点 = 项.可处理时点;
            }
        }
        return 时点;
    }

    bool 有未停放消息已加锁() const noexcept {
        return std::any_of(活动消息.begin(), 活动消息.end(),
            [](const auto& 项) { return !项->因果能力已停放; });
    }

    // 只由真实关联通知或学习完成通知调用，不能由周期扫描/消息收尾触发。
    void 唤醒因果能力等待已加锁() noexcept {
        const auto 当前 = std::chrono::steady_clock::now();
        for (auto& 项 : 活动消息) {
            if (!项->因果能力复核登记) continue;
            项->因果能力期间事件 = true;
            if (项->因果能力已停放) {
                项->因果能力已停放 = false;
                项->可处理时点 = 当前;
            }
        }
    }

    void 请求关联复核已加锁(bool 允许恢复 = false) noexcept {
        关联复核待触发 = true;
        if (停止已请求 || 扫描恢复 == 扫描恢复方式::硬失败) return;
        if (允许恢复) {
            for (auto& 项 : 扫描承接) {
                if (!项.等待恢复事件) continue;
                项.等待恢复事件 = false;
                项.可处理时点 = std::chrono::steady_clock::now();
            }
            if (扫描恢复 == 扫描恢复方式::等待事件 && 待恢复工作种类
                && !当前扫描请求 && !扫描工作结果.valid()) {
                (void)提交需求工作已加锁(*待恢复工作种类);
                return;
            }
        }
        if (需求扫描阶段 == 新自我需求扫描阶段::空闲
            && !当前扫描请求 && !扫描工作结果.valid()
            && 扫描承接.empty()) {
            (void)提交需求工作已加锁(新自我工作种类::需求树全局扫描);
        }
    }

    bool 确认并释放扫描结果已加锁() noexcept {
        if (!自我工作线程 || !当前扫描请求 || !扫描工作结果.valid()) {
            const auto 请求 = 当前扫描请求.value_or(新自我需求树扫描请求{});
            安排扫描恢复已加锁(扫描恢复方式::硬失败, 请求.种类);
            登记扫描诊断已加锁(L"扫描交接身份缺失", 0, 扫描恢复, 请求);
            return false;
        }
        const auto 确认 = 自我工作线程->确认结果已接收(
            当前扫描请求->工作编号);
        if (确认 != 新自我工作操作状态::已完成) {
            安排扫描恢复已加锁(扫描恢复方式::硬失败, 当前扫描请求->种类);
            登记扫描诊断已加锁(L"确认扫描结果", static_cast<int>(确认), 扫描恢复, *当前扫描请求);
            return false; // 未确认时保留句柄，不允许开始下一次扫描。
        }
        扫描工作结果 = {};
        当前扫描请求.reset();
        return true;
    }

    // 需求不合法未承接：自我判定不合法，不发送、不重试，本项按终结移出（不计扫描失败）。
    enum class 扫描承接推进 : std::uint8_t { 等待, 等待事件, 成功, 失败, 需求不合法未承接 };
    struct 扫描承接进度 final {
        扫描承接推进 推进 = 扫描承接推进::失败;
        const wchar_t* 来源 = nullptr;
        int 状态 = 0;
    };

    扫描承接进度 处理扫描承接(自动承接项& 项) noexcept {
        try {
            if (!任务管理) return {扫描承接推进::失败, L"任务管理缺失"};
            if (!项.管理结果.valid()) {
                if (判定需求合法性(项.消息.需求节点, 自我需求承接依据::全树扫描)
                    == 自我需求合法性判定::不合法) return {扫描承接推进::需求不合法未承接};
                const auto 接收 = 任务管理->提交消息(项.消息);
                switch (接收.状态) {
                case 新任务承接接收状态::已接收:
                case 新任务承接接收状态::精确重复:
                    项.管理结果 = 接收.处理结果;
                    if (!项.管理结果.valid()) return {扫描承接推进::失败, L"承接结果句柄无效"};
                    break;
                case 新任务承接接收状态::邮箱已满:
                case 新任务承接接收状态::资源失败:
                    return {扫描承接推进::等待, L"提交扫描承接", static_cast<int>(接收.状态)};
                case 新任务承接接收状态::尚未启动:
                    return {扫描承接推进::等待事件, L"提交扫描承接", static_cast<int>(接收.状态)};
                default:
                    return {扫描承接推进::失败, L"提交扫描承接", static_cast<int>(接收.状态)};
                }
            }
            if (项.管理结果.wait_for(std::chrono::milliseconds(0))
                != std::future_status::ready) return {扫描承接推进::等待};
            const auto& 结果 = 项.管理结果.get();
            if (结果.原消息 != 项.消息) return {扫描承接推进::失败, L"承接回执身份冲突"};
            if (结果.状态 == 新任务承接处理状态::已承接
                    || 结果.状态 == 新任务承接处理状态::已重新启用
                    || 结果.状态 == 新任务承接处理状态::既有终结) return {扫描承接推进::成功};
            if (结果.状态 == 新任务承接处理状态::服务未就绪)
                return {扫描承接推进::等待事件, L"承接服务未就绪", static_cast<int>(结果.状态)};
            if (结果.状态 == 新任务承接处理状态::需核查 && 结果.承接
                && 结果.承接->建立结果
                && 结果.承接->建立结果->状态 == 新任务操作状态::服务任务集合同步失败)
                return {扫描承接推进::等待事件, L"服务任务集合同步原因未知",
                    static_cast<int>(结果.承接->建立结果->状态)};
            return {扫描承接推进::失败, L"承接回执不可恢复", static_cast<int>(结果.状态)};
        } catch (...) {
            // broken_promise等不证明资源短缺；保留硬失败诊断，不推断写入结果。
            return {扫描承接推进::失败, L"读取承接回执异常"};
        }
    }

    void 结束扫描失败已加锁(扫描恢复方式 恢复, const wchar_t* 来源, int 状态) noexcept {
        const auto 请求 = 当前扫描请求.value_or(新自我需求树扫描请求{});
        if (!确认并释放扫描结果已加锁()) return;
        安排扫描恢复已加锁(恢复, 请求.种类);
        登记扫描诊断已加锁(来源, 状态, 恢复, 请求);
    }

    void 完成扫描承接已加锁() noexcept {
        if (!扫描承接.empty()) return; // 包括等待事件的行，未真正结束前不能ack。
        const auto 请求 = 当前扫描请求.value_or(新自我需求树扫描请求{});
        if (!确认并释放扫描结果已加锁()) return;
        if (扫描承接失败) {
            安排扫描恢复已加锁(扫描恢复方式::硬失败, 请求.种类);
            return;
        }
        需求扫描阶段 = 新自我需求扫描阶段::空闲;
        最近扫描诊断.reset();
        if (关联复核待触发 || 有关联变化通知()) 请求关联复核已加锁();
    }

    void 消费扫描结果已加锁() noexcept {
        需求扫描阶段 = 新自我需求扫描阶段::处理扫描结果;
        if (!当前扫描请求 || !扫描工作结果.valid()) {
            (void)确认并释放扫描结果已加锁();
            return;
        }
        const auto 请求 = *当前扫描请求;
        // 确认会释放成员句柄；本地共享句柄保证下面的const引用仍有效。
        const auto 结果句柄 = 扫描工作结果;
        try {
            const auto& 扫描 = 结果句柄.get();
            if (扫描.原请求 != 请求) {
                结束扫描失败已加锁(扫描恢复方式::硬失败, L"扫描回执身份冲突", 0);
                return;
            }
            const bool 可因变化作废 = 扫描.状态 == 新自我需求树扫描状态::已完成
                || 扫描.状态 == 新自我需求树扫描状态::已完成但关联异常
                || 扫描.状态 == 新自我需求树扫描状态::需求树结构不一致
                || 扫描.状态 == 新自我需求树扫描状态::任务结构不一致;
            if (可因变化作废 && (关联复核待触发 || 有关联变化通知())) {
                if (!确认并释放扫描结果已加锁()) return;
                需求扫描阶段 = 新自我需求扫描阶段::空闲;
                请求关联复核已加锁();
                return;
            }
            最近已消费检查结果 = 结果句柄;
            if (扫描.状态 == 新自我需求树扫描状态::已完成但关联异常) {
                最近需求数量 = 扫描.需求数量;
                最近任务数量 = 扫描.任务数量;
                if (!确认并释放扫描结果已加锁()) return;
                需求扫描阶段 = 新自我需求扫描阶段::空闲;
                登记扫描诊断已加锁(L"完整关联检查异常", static_cast<int>(扫描.状态),
                    扫描恢复方式::无, 请求);
                return; // 整批零补建；不擅自选取或删除异常任务。
            }
            if (扫描.状态 != 新自我需求树扫描状态::已完成) {
                const auto 恢复 = 扫描.状态 == 新自我需求树扫描状态::资源失败
                    ? 扫描恢复方式::资源延后
                    : 扫描.状态 == 新自我需求树扫描状态::需求服务未就绪
                        || 扫描.状态 == 新自我需求树扫描状态::任务服务未就绪
                        ? 扫描恢复方式::等待事件 : 扫描恢复方式::硬失败;
                结束扫描失败已加锁(恢复, L"扫描工作回执", static_cast<int>(扫描.状态));
                return;
            }
            最近需求数量 = 扫描.需求数量;
            最近任务数量 = 扫描.任务数量;
            if (!扫描承接.empty()) {
                结束扫描失败已加锁(扫描恢复方式::硬失败, L"扫描承接批次重叠", 0);
                return;
            }
            // 先完整构造再发布，bad_alloc不能让已构造前缀进入派发路径。
            std::vector<自动承接项> 新批次;
            新批次.reserve(扫描.缺少任务的需求节点组.size());
            for (const auto 需求节点 : 扫描.缺少任务的需求节点组) {
                新任务承接消息 消息;
                消息.消息编号 = 取得自动消息编号();
                消息.需求节点 = 需求节点;
                消息.依据 = 新任务承接依据::自我全树扫描;
                新批次.push_back({std::move(消息), std::chrono::steady_clock::now(), {}});
            }
            扫描承接.swap(新批次);
            扫描承接失败 = false;
            需求扫描阶段 = 新自我需求扫描阶段::等待任务承接;
            if (扫描承接.empty()) 完成扫描承接已加锁();
        } catch (const std::bad_alloc&) {
            结束扫描失败已加锁(扫描恢复方式::资源延后, L"读取或构造扫描批次资源不足", 0);
        } catch (...) {
            结束扫描失败已加锁(扫描恢复方式::硬失败, L"读取或构造扫描批次异常", 0);
        }
    }

    新任务等待复核反馈接收状态 接收等待复核(
        const 新任务等待复核定位& 定位) noexcept {
        if (!定位.完整()) return 新任务等待复核反馈接收状态::入口拒绝;
        try {
            std::lock_guard 锁(状态互斥);
            if (停止已请求 || 线程已完成)
                return 新任务等待复核反馈接收状态::已停止接收;
            if (!线程已进入) return 新任务等待复核反馈接收状态::尚未启动;
            if (!消息配置) return 新任务等待复核反馈接收状态::尚未配置;
            for (const auto& 项 : 活动消息) {
                if (项->消息.类型 != 新自我消息类型::等待任务复核
                    || !项->消息.等待复核) continue;
                if (*项->消息.等待复核 == 定位)
                    return 新任务等待复核反馈接收状态::精确重复;
                if (项->消息.等待复核->复核编号 == 定位.复核编号)
                    return 新任务等待复核反馈接收状态::身份冲突;
            }
            if (普通消息数量已加锁() >= 消息配置->邮箱容量)
                return 新任务等待复核反馈接收状态::邮箱已满;
            const auto 项 = std::make_shared<消息项>();
            项->消息.消息编号 = 取得自动消息编号();
            if (!项->消息.消息编号) return 新任务等待复核反馈接收状态::内部错误;
            项->消息.类型 = 新自我消息类型::等待任务复核;
            项->消息.等待复核 = 定位;
            项->可处理时点 = std::chrono::steady_clock::now();
            活动消息.push_back(项);
            状态变化.notify_all();
            return 新任务等待复核反馈接收状态::已接收;
        } catch (const std::bad_alloc&) { return 新任务等待复核反馈接收状态::资源失败; }
        catch (...) { return 新任务等待复核反馈接收状态::内部错误; }
    }

    // 这里只给重新筹办资格，不调用方法选择器、不写T/M/X/A，也不签动作许可。
    std::optional<新自我等待复核决议> 复核等待任务(
        const 新任务等待复核定位& 定位) {
        新自我等待复核决议 决议{定位, 新自我等待复核状态::需核查, {}};
        const auto 出口 = [&]([[maybe_unused]] int 行) -> const 新自我等待复核决议& {
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            try {
                std::wostringstream 文本;
                文本 << L"任务=" << 定位.定位.任务节点.值 << L" 等待原因=" << static_cast<int>(定位.原因)
                     << L" 任务状态=" << static_cast<int>(定位.定位.任务状态) << L" 出口行=" << 行
                     << L" 决议=" << static_cast<int>(决议.状态)
                     << L" 当前满足=" << (决议.当前满足状态 ? static_cast<int>(*决议.当前满足状态) : -1);
                (void)调试日志<调试切片::PARAM_BINDING>(L"自我等待复核", L"需核查出口", 文本.str());
            } catch (...) {}
#endif
            return 决议;
        };
        // 轻量核对（2026-11-06用户裁决）：只读任务、执行、评估与需求核对定位，不重读整份调度材料、
        // 不重算条件核验；读取期间场景变化不再被核同报成结构不一致。资格与迁移仍由后续fresh入口核验。
        const auto 任务读 = 任务服务->获取任务(定位.定位.任务节点);
        if (!任务读) return 出口(__LINE__);
        const auto& 任务 = *任务读;
        std::optional<新任务执行信息> 执行读;
        std::optional<新任务影响评估信息> 评估读;
        if (任务.最近执行实例节点) {
            执行读 = 任务服务->获取执行实例(*任务.最近执行实例节点);
            if (!执行读 || 执行读->任务节点 != 任务.节点) return 出口(__LINE__);
            if (执行读->影响评估节点) {
                评估读 = 任务服务->获取影响评估(*执行读->影响评估节点);
                if (!评估读 || 评估读->任务节点 != 任务.节点
                    || 评估读->执行实例节点 != 执行读->节点) return 出口(__LINE__);
            }
        }
        新任务调度定位 当前定位;
        当前定位.任务节点 = 任务.节点;
        当前定位.需求节点 = 任务.需求节点;
        当前定位.任务状态 = 任务.当前状态;
        当前定位.候选材料节点 = 任务.当前候选方法材料节点;
        当前定位.方法实例节点 = 任务.方法实例节点;
        当前定位.最近执行实例节点 = 任务.最近执行实例节点;
        当前定位.影响评估节点 = 执行读 ? 执行读->影响评估节点 : std::nullopt;
        当前定位.方法执行组织节点 = 任务.方法执行组织节点;
        if (当前定位 != 定位.定位) {
            决议.状态 = 新自我等待复核状态::定位已变化;
            return 出口(__LINE__);
        }
        const auto 治理 = 需求服务->读取需求治理信息(任务.需求节点);
        if (治理 && !治理->完整目标) {
            决议.状态 = 新自我等待复核状态::等待材料;
            return 出口(__LINE__);
        }
        const auto 需求 = 需求服务->获取需求(定位.定位.需求节点);
        if (!治理 || !需求) return 出口(__LINE__);
        if (需求->活动状态 != 新需求活动状态::活动) {
            决议.状态 = 新自我等待复核状态::需求非活动;
            return 出口(__LINE__);
        }
        const auto 资格 = 任务服务->复核任务筹办资格(任务.节点);
        // 结构依赖未就绪：运行中工程异常终止；停机期间不形成决议。
        if (资格.状态 == 新需求筹办资格状态::结构依赖未就绪) {
            (void)结构依赖未就绪已停止(L"复核等待任务/复核任务筹办资格",static_cast<int>(资格.状态),任务.节点.值);
            return std::nullopt;
        }
        if (资格.状态 == 新需求筹办资格状态::资源失败
            || 资格.状态 == 新需求筹办资格状态::材料已变化) return std::nullopt;
        const auto 当前 = 资格.当前满足.value_or(新需求当前满足结果{});
        if (当前.状态 == 新需求当前满足状态::资源失败) return std::nullopt;
        决议.当前满足状态 = 当前.状态;
        if (当前.状态 == 新需求当前满足状态::已满足) {
            决议.状态 = 新自我等待复核状态::当前已满足;
            return 出口(__LINE__);
        }
        if (任务.当前状态 != 新任务状态::已建立
            && 任务.当前状态 != 新任务状态::等待条件
            && 任务.当前状态 != 新任务状态::待检查条件) return 出口(__LINE__);
        // W0不能证明等待来源；只接纳manager在实际等待入口保存的明确原因。
        if (定位.原因 == 新任务等待原因::无
            || 定位.原因 == 新任务等待原因::未分类
            || 定位.原因 == 新任务等待原因::需核查) return 出口(__LINE__);
        if (定位.定位.最近执行实例节点) {
            if (!执行读) return 出口(__LINE__);
            const auto& 执行 = *执行读;
            if (执行.状态 != 新任务执行状态::已评估 || !执行.结束时间) {
                决议.状态 = 新自我等待复核状态::等待同次执行比较;
                return 出口(__LINE__);
            }
            if (!评估读 || !定位.定位.影响评估节点) return 出口(__LINE__);
            const auto& 评估 = *评估读;
            if (需求->目标.目标形式 == 新需求目标形式::变化方向) {
                // A.方向验证是方法预测验证，不能替代目标D的同次A/B比较。
                const auto 满足 = 评估.目标需求结果.状态;
                if (满足 == 新需求当前满足状态::已满足) {
                    决议.状态 = 新自我等待复核状态::当前已满足;
                    return 出口(__LINE__);
                }
                if (满足 != 新需求当前满足状态::需增加
                    && 满足 != 新需求当前满足状态::需减少) {
                    决议.状态 = 新自我等待复核状态::等待同次执行比较;
                    return 出口(__LINE__);
                }
            }
        } else if (定位.原因 == 新任务等待原因::执行后等待
            || 定位.原因 == 新任务等待原因::同次执行比较材料不足) return 出口(__LINE__);
        if (资格.状态 == 新需求筹办资格状态::允许筹办
            || 资格.状态 == 新需求筹办资格状态::等待普通材料) {
            bool 词典仍需材料 = false;
            bool 词典已有知识 = false;
            bool 有其它候选 = false;
            if (const auto 候选 = 任务服务->获取当前候选方法材料(任务.节点)) {
                for (const auto M : 候选->候选方法节点组) {
                    const auto 读 = 读取当前词典学习发现(任务.节点, M);
                    if (!读.发现) { 有其它候选 = true; continue; }
                    const auto& 当前发现 = *读.发现;
                    决议.附加等待依赖键组.insert(决议.附加等待依赖键组.end(),
                        当前发现.等待依赖键组.begin(), 当前发现.等待依赖键组.end());
                    const auto 接纳 = 接纳词典学习发现(M, 当前发现);
                    if (接纳 == 方法学习发现状态::暂缓) return std::nullopt;
                    词典仍需材料 |= 当前发现.处置 == 新词典学习处置::条件未具备
                        || 当前发现.处置 == 新词典学习处置::可提交学习;
                    词典已有知识 |= 当前发现.处置 == 新词典学习处置::已有覆盖知识
                        || 当前发现.处置 == 新词典学习处置::已有空结果配对;
                }
            }
            if (词典仍需材料 && !词典已有知识 && !有其它候选) {
                决议.状态 = 新自我等待复核状态::等待材料;
                return 出口(__LINE__);
            }
            if (资格.状态 == 新需求筹办资格状态::等待普通材料) {
                决议.状态 = 新自我等待复核状态::等待材料;
                return 出口(__LINE__);
            }
            决议.筹办资格 = 资格;
            决议.状态 = 新自我等待复核状态::允许重新筹办;
        }
        return 出口(__LINE__);
    }

    std::optional<新自我消息处理结果> 处理等待复核(消息项& 项) noexcept {
        新自我消息处理结果 结果;
        结果.原消息 = 项.消息;
        结果.状态 = 新自我消息处理状态::等待复核需核查;
        if (!任务管理 || !任务服务 || !需求服务 || !项.消息.等待复核
            || !项.消息.等待复核->完整()) return 结果;
        try {
            if (!项.等待决议) {
                const auto& 定位 = *项.消息.等待复核;
                const auto 管理定位 = 任务管理->读取等待复核定位(定位.复核编号);
                if (!管理定位 || *管理定位 != 定位) {
                    结果.状态 = 新自我消息处理状态::等待复核定位已变化;
                    return 结果;
                }
                项.等待决议 = 复核等待任务(定位);
                if (!项.等待决议) return std::nullopt;
            }
        } catch (const std::bad_alloc&) { return std::nullopt; }
        catch (...) {
            // 业务硬失败也必须交回具名决议，不能把manager留在永久占用中。
            项.等待决议 = 新自我等待复核决议{
                *项.消息.等待复核, 新自我等待复核状态::需核查, {}};
        }
        结果.等待复核决议 = 项.等待决议;
        const auto 接收 = 任务管理->提交等待复核决议(*项.等待决议);
        结果.等待复核接收状态 = 接收;
        if (接收 == 新自我等待复核接收状态::筹办材料已变化) {
            项.等待决议.reset(); return std::nullopt;
        }
        if (接收 == 新自我等待复核接收状态::内部错误)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"等待复核资格交接", L"manager资格复核或交接发生工程错误。");
        // 交接资源失败仅重试同一决定，不重算业务资格。
        if (接收 == 新自我等待复核接收状态::资源失败) return std::nullopt;
        if (接收 == 新自我等待复核接收状态::已接收
            || 接收 == 新自我等待复核接收状态::精确重复)
            结果.状态 = 新自我消息处理状态::等待复核已裁决;
        else if (接收 == 新自我等待复核接收状态::定位已变化)
            结果.状态 = 新自我消息处理状态::等待复核定位已变化;
        else if (接收 == 新自我等待复核接收状态::已停止接收)
            结果.状态 = 新自我消息处理状态::停止取消;
        return 结果;
    }

    新任务筹办反馈接收状态 接收筹办反馈(
        const 新任务筹办反馈定位& 定位) noexcept {
        if (!定位.完整()) return 新任务筹办反馈接收状态::入口拒绝;
        try {
            std::lock_guard 锁(状态互斥);
            if (停止已请求 || 线程已完成)
                return 新任务筹办反馈接收状态::已停止接收;
            if (!线程已进入)
                return 新任务筹办反馈接收状态::尚未启动;
            if (!消息配置)
                return 新任务筹办反馈接收状态::尚未配置;
            for (const auto& 项 : 活动消息) {
                if (项->消息.类型 != 新自我消息类型::任务筹办反馈
                    || !项->消息.筹办反馈) continue;
                if (*项->消息.筹办反馈 == 定位)
                    return 新任务筹办反馈接收状态::精确重复;
                if (项->消息.筹办反馈->任务节点 == 定位.任务节点)
                    return 新任务筹办反馈接收状态::身份冲突;
            }
            if (普通消息数量已加锁() >= 消息配置->邮箱容量)
                return 新任务筹办反馈接收状态::邮箱已满;
            const auto 项 = std::make_shared<消息项>();
            项->消息.消息编号 = 取得自动消息编号();
            项->消息.类型 = 新自我消息类型::任务筹办反馈;
            项->消息.筹办反馈 = 定位;
            项->可处理时点 = std::chrono::steady_clock::now();
            活动消息.push_back(项);
            状态变化.notify_all();
            return 新任务筹办反馈接收状态::已接收;
        } catch (const std::bad_alloc&) {
            return 新任务筹办反馈接收状态::资源失败;
        } catch (...) {
            return 新任务筹办反馈接收状态::内部错误;
        }
    }

    新任务结果反馈接收状态 接收任务结果反馈(
        const 新任务结果反馈定位& 定位) noexcept {
        if (!定位.完整()) return 新任务结果反馈接收状态::入口拒绝;
        try {
            std::lock_guard 锁(状态互斥);
            if (停止已请求 || 线程已完成)
                return 新任务结果反馈接收状态::已停止接收;
            if (!线程已进入)
                return 新任务结果反馈接收状态::尚未启动;
            if (!消息配置)
                return 新任务结果反馈接收状态::尚未配置;
            for (const auto& 项 : 活动消息) {
                if (项->消息.类型 != 新自我消息类型::任务结果反馈
                    || !项->消息.结果反馈) continue;
                if (*项->消息.结果反馈 == 定位)
                    return 新任务结果反馈接收状态::精确重复;
                if (项->消息.结果反馈->任务节点 == 定位.任务节点)
                    return 新任务结果反馈接收状态::身份冲突;
            }
            if (普通消息数量已加锁() >= 消息配置->邮箱容量)
                return 新任务结果反馈接收状态::邮箱已满;
            const auto 项 = std::make_shared<消息项>();
            项->消息.消息编号 = 取得自动消息编号();
            项->消息.类型 = 新自我消息类型::任务结果反馈;
            项->消息.结果反馈 = 定位;
            项->可处理时点 = std::chrono::steady_clock::now();
            活动消息.push_back(项);
            状态变化.notify_all();
            return 新任务结果反馈接收状态::已接收;
        } catch (const std::bad_alloc&) {
            return 新任务结果反馈接收状态::资源失败;
        } catch (...) {
            return 新任务结果反馈接收状态::内部错误;
        }
    }

    新任务结果反馈接收状态 接收任务完成贡献复核(
        const 新任务贡献反馈定位& 定位) noexcept {
        if (!有效(定位.任务节点) || !有效(定位.执行实例节点) || !有效(定位.影响评估节点))
            return 新任务结果反馈接收状态::入口拒绝;
        try {
            std::lock_guard 锁(状态互斥);
            if (停止已请求 || 线程已完成) return 新任务结果反馈接收状态::已停止接收;
            if (!线程已进入) return 新任务结果反馈接收状态::尚未启动;
            if (!消息配置) return 新任务结果反馈接收状态::尚未配置;
            for (const auto& 项 : 活动消息) {
                if (!项->消息.完成贡献复核) continue;
                if (*项->消息.完成贡献复核 == 定位) {
                    // 原贡献确认已交出而最终future尚未消费时，不把新进度合并进旧
                    // 否定future。manager保留本次新事件，待本消息出队后新建消息。
                    if (项->最终工作已交接阶段.load() || 项->贡献工作已交接阶段.load())
                        return 新任务结果反馈接收状态::邮箱已满;
                    // manager可在本消息确认后、出队前收到新进度；合并唤醒而非丢掉再投。
                    项->贡献复核再次唤醒 = true;
                    return 新任务结果反馈接收状态::精确重复;
                }
                if (项->消息.完成贡献复核->任务节点 == 定位.任务节点) return 新任务结果反馈接收状态::身份冲突;
            }
            if (普通消息数量已加锁() >= 消息配置->邮箱容量) return 新任务结果反馈接收状态::邮箱已满;
            const auto 项 = std::make_shared<消息项>();
            项->消息.消息编号 = 取得自动消息编号(); 项->消息.类型 = 新自我消息类型::任务完成贡献复核;
            项->消息.完成贡献复核 = 定位; 项->可处理时点 = std::chrono::steady_clock::now();
            活动消息.push_back(项); 状态变化.notify_all();
            return 新任务结果反馈接收状态::已接收;
        } catch (const std::bad_alloc&) { return 新任务结果反馈接收状态::资源失败; }
        catch (...) { return 新任务结果反馈接收状态::内部错误; }
    }

    // 承接回执的非成功业务映射；普通承接、完整服务需求和行动请求承接共用。
    static std::optional<新自我消息处理状态> 承接回执非成功状态(
        新任务承接处理状态 状态) noexcept {
        switch (状态) {
        case 新任务承接处理状态::入口拒绝: return 新自我消息处理状态::入口拒绝;
        case 新任务承接处理状态::服务未就绪: return 新自我消息处理状态::任务管理未就绪;
        case 新任务承接处理状态::需核查: return 新自我消息处理状态::任务承接需核查;
        default: return std::nullopt;
        }
    }

    std::optional<新自我消息处理结果> 处理完整服务需求材料(消息项& 项) noexcept {
        try {
            if (!任务管理 || !任务服务 || !需求服务 || !项.消息.完整服务需求材料)
                throw std::logic_error("complete service packet dependencies missing");
            新自我消息处理结果 结果;
            结果.原消息 = 项.消息;
            结果.外部服务需求采用=项.外部服务采用材料;
            const auto& 材料包 = *项.消息.完整服务需求材料;
            // 已提交manager的请求先收回同一future，不能因材料后变而丢弃承接事实。
            if (项.管理结果.valid()) {
                if (项.管理结果.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
                    return std::nullopt;
                结果.任务承接 = 项.管理结果.get();
                const auto& 承接 = *结果.任务承接;
                if (!项.外部服务承接消息 || 承接.原消息 != *项.外部服务承接消息)
                    throw std::logic_error("complete service acceptance receipt mismatch");
                if (承接.状态 == 新任务承接处理状态::停止取消) {
                    结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                }
                if (const auto 映射 = 承接回执非成功状态(承接.状态)) {
                    结果.状态 = *映射; return 结果;
                }
                if (承接.状态 != 新任务承接处理状态::已承接
                    && 承接.状态 != 新任务承接处理状态::已重新启用
                    && 承接.状态 != 新任务承接处理状态::既有终结
                    && 承接.状态 != 新任务承接处理状态::需求当前已满足)
                    throw std::logic_error("complete service task acceptance failed");
                if (!承接.承接 || !承接.承接->任务)
                    throw std::logic_error("complete service acceptance task missing");
                const auto 当前任务 = 任务服务->获取任务(承接.承接->任务->节点);
                if (!当前任务 || !任务服务->需求属于任务服务集(当前任务->节点, 项.外部服务承接消息->需求节点))
                    throw std::logic_error("complete service receipt formal identity mismatch");
            }
            const 新外部服务需求采用结果* 已知前缀=nullptr;
            if(项.外部服务采用材料)已知前缀=&*项.外部服务采用材料;
            const auto 采用 = 需求服务->建立或取得外部服务需求已核验(材料包,std::nullopt,已知前缀);
            if(!项.外部服务采用材料)项.外部服务采用材料.emplace();
            auto& 已知=*项.外部服务采用材料;
            if(采用.需求节点) {
                if(已知.需求节点&&已知.需求节点!=采用.需求节点)
                    throw std::logic_error("external adoption demand prefix changed");
                已知.需求节点=采用.需求节点;
            }
            if(采用.需求列表项节点) {
                if(已知.需求列表项节点&&已知.需求列表项节点!=采用.需求列表项节点)
                    throw std::logic_error("external adoption list prefix changed");
                已知.需求列表项节点=采用.需求列表项节点;
            }
            已知.状态=采用.状态;
            结果.外部服务需求采用 = 已知;
            // 采用状态依赖未就绪属结构类（需求类来源：读取接口未装配、结构未初始化、核验依赖未就绪），不再按邮箱间隔重试。
            if(采用.状态==新外部服务需求采用状态::依赖未就绪){
                (void)结构依赖未就绪已停止(L"建立或取得外部服务需求已核验",static_cast<int>(采用.状态),项.消息.消息编号);
                结果.状态=新自我消息处理状态::停止取消;return 结果;
            }
            if(采用.状态==新外部服务需求采用状态::前提已变化
                ||采用.状态==新外部服务需求采用状态::资源失败)return std::nullopt;
            if (采用.状态 == 新外部服务需求采用状态::材料待完善
                || 采用.状态 == 新外部服务需求采用状态::需重新确认) {
                结果.状态 = 新自我消息处理状态::等待材料;
                return 结果;
            }
            if (采用.状态 == 新外部服务需求采用状态::来源不符
                || 采用.状态 == 新外部服务需求采用状态::要求不存在) {
                // 承接后外部要求变化属内部世界正常变化：已建任务与需求由其owner处理，本消息以入口拒绝终结。
                结果.状态 = 新自我消息处理状态::入口拒绝; return 结果;
            }
            if ((采用.状态 != 新外部服务需求采用状态::已建立
                    && 采用.状态 != 新外部服务需求采用状态::已复用)
                || !采用.需求节点||!采用.需求列表项节点)
                throw std::logic_error("complete service demand adoption failed");
            if (结果.任务承接) {
                const auto& 承接 = *结果.任务承接;
                // D/R/确认/完整目标/list由需求owner的可抛链统一正式核验。
                if (!项.外部服务承接消息||项.外部服务承接消息->需求节点!=采用.需求节点
                    || !承接.承接 || !承接.承接->任务)
                    throw std::logic_error("complete service demand task receipt unreadable");
                const auto 当前任务 = 任务服务->获取任务(承接.承接->任务->节点);
                if (!当前任务 || !任务服务->需求属于任务服务集(当前任务->节点, *采用.需求节点))
                    throw std::logic_error("complete service demand task identity changed");
                结果.状态 = 新自我消息处理状态::任务已承接;
                return 结果;
            }
            if (!项.外部服务承接消息) {
                新任务承接消息 消息;
                消息.消息编号 = 项.消息.消息编号;
                消息.依据 = 新任务承接依据::外部服务要求;
                消息.需求节点 = *采用.需求节点;
                消息.外部要求节点 = 材料包.要求节点;
                项.外部服务承接消息 = 消息;
            } else if (项.外部服务承接消息->需求节点 != 采用.需求节点) {
                throw std::logic_error("same external request adopted another demand");
            }
            if (判定需求合法性(项.外部服务承接消息->需求节点, 自我需求承接依据::外部服务要求)
                == 自我需求合法性判定::不合法) {
                结果.状态 = 新自我消息处理状态::需求不合法未承接; return 结果;
            }
            const auto 接收 = 任务管理->提交外部服务需求承接(*项.外部服务承接消息);
            结果.管理接收状态 = 接收.状态;
            switch (接收.状态) {
            case 新任务承接接收状态::已接收:
            case 新任务承接接收状态::精确重复:
                if (!接收.处理结果.valid()) throw std::logic_error("external acceptance future missing");
                项.管理结果 = 接收.处理结果;
                return std::nullopt;
            case 新任务承接接收状态::邮箱已满:
            case 新任务承接接收状态::资源失败:
            case 新任务承接接收状态::尚未启动:
                return std::nullopt;
            case 新任务承接接收状态::已停止接收:
                结果.状态 = 新自我消息处理状态::任务管理未就绪; return 结果;
            default: throw std::logic_error("external service acceptance rejected formal demand");
            }
        } catch (const std::bad_alloc&) { return std::nullopt;
        } catch (const std::length_error&) { return std::nullopt;
        } catch (...) {
            工程异常终止(L"新_自我线程类.cpp", __LINE__, L"处理完整服务需求材料", L"完整需求信息包交接不变量异常");
        }
    }

    新自我消息接收结果 接纳行动请求(const 新行动请求承接来源& source) noexcept {
        const auto& key=source.需求来源.键;
        if(!有效(key.接收消息节点)||!有效(source.需求来源.采用含义节点)
            ||!有效(source.被认知需求节点)||!有效(source.来源人类节点)||!有效(source.受请求者节点)
            ||key.原文起点>=key.原文终点||key.根出现路径.empty()
            ||std::any_of(key.根出现路径.begin(),key.根出现路径.end(),[](auto n){return n<0;}))return {};
        try {
            std::lock_guard lock(状态互斥);
            if(停止已请求||线程已完成)return {新自我消息接收状态::已停止接收,{}};
            if(!线程已进入)return {新自我消息接收状态::尚未启动,{}};
            if(!消息配置)return {新自我消息接收状态::尚未配置,{}};
            for(const auto& item:活动消息) {
                if(!item->消息.行动请求来源
                    ||item->消息.行动请求来源->需求来源.采用含义节点!=source.需求来源.采用含义节点)continue;
                if(*item->消息.行动请求来源!=source)return {新自我消息接收状态::身份冲突,{}};
                return {新自我消息接收状态::精确重复,item->结果};
            }
            if(普通消息数量已加锁()>=消息配置->邮箱容量)return {新自我消息接收状态::邮箱已满,{}};
            const auto item=std::make_shared<消息项>();
            item->消息.消息编号=取得自动消息编号();
            if(!item->消息.消息编号)throw std::logic_error("action request self message identity exhausted");
            item->消息.类型=新自我消息类型::行动请求承接;item->消息.行动请求来源=source;
            item->可处理时点=std::chrono::steady_clock::now();
            活动消息.push_back(item);状态变化.notify_all();
            return {新自我消息接收状态::已接收,item->结果};
        }catch(const std::bad_alloc&){return {新自我消息接收状态::资源失败,{}};}
        catch(const std::length_error&){return {新自我消息接收状态::资源失败,{}};}
        catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"接纳行动请求",L"自我请求邮箱接纳不变量异常。");}
    }

    新自我消息接收结果 接纳前置输入(const 新前置语言输入定位& p) noexcept {
        using S=新自我消息接收状态;
        try {
            if(!有效(p.来源.接收消息节点)||!有效(p.前置来源节点)||!有效(p.固定文本材料)
                ||!有效(p.语言节点)||!有效(p.来源人类节点)||!有效(p.自我存在节点)||!有效(p.现实根场景)
                ||p.公开宿主会话.empty()||p.来源.原文起点>=p.来源.原文终点)return {S::入口拒绝,{}};
            std::lock_guard 锁(状态互斥);
            if(停止已请求||线程已完成)return {S::已停止接收,{}};
            if(!线程已进入)return {S::尚未启动,{}};
            if(!消息配置)return {S::尚未配置,{}};
            for(const auto& item:活动消息)if(item->消息.前置语言输入&&item->消息.前置语言输入->来源==p.来源) {
                if(*item->消息.前置语言输入!=p)return {S::身份冲突,{}};
                return {S::精确重复,item->结果};
            }
            if(普通消息数量已加锁()>=消息配置->邮箱容量)return {S::邮箱已满,{}};
            const auto item=std::make_shared<消息项>();
            item->消息.消息编号=取得自动消息编号();
            if(!item->消息.消息编号)throw std::logic_error("preunderstanding self mailbox identity exhausted");
            item->消息.类型=新自我消息类型::前置语言输入材料;item->消息.前置语言输入=p;
            item->可处理时点=std::chrono::steady_clock::now();活动消息.push_back(item);状态变化.notify_all();
            return {S::已接收,item->结果};
        }catch(const std::bad_alloc&){return {S::资源失败,{}};}
        catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"接纳前置输入",L"同M输入邮箱定位不变量异常");}
    }
    std::optional<新自我消息处理结果> 处理前置语言输入材料(消息项& 项) {
        using R=新前置语言读取状态;
        try {
            if(!接收认知||!任务服务||!任务管理||!需求服务||!初始化结果||!初始化结果->自我存在||!项.消息.前置语言输入)
                throw std::logic_error("preunderstanding self dependencies missing");
            const auto& p=*项.消息.前置语言输入;
            新自我消息处理结果 out;out.原消息=项.消息;
            if(p.自我存在节点!=*初始化结果->自我存在)throw std::logic_error("preunderstanding self identity conflict");
            if(!项.前置工作) {
                if(!项.前置请求) {
                if(停止源.stop_requested()){out.状态=新自我消息处理状态::停止取消;return out;}
                const auto fresh=接收认知->确认前置输入已接纳(p);
                if(fresh==接收行动请求交接确认状态::输入拒绝){out.状态=新自我消息处理状态::停止取消;return out;}
                if(fresh!=接收行动请求交接确认状态::已确认&&fresh!=接收行动请求交接确认状态::精确重复)
                    throw std::logic_error("fresh preunderstanding source or context rejected");
                const auto D=需求服务->查询根需求(新需求根角色::服务);
                if(!D)throw std::logic_error("service parent root missing");
                const auto parent=任务服务->建立前置语言材料父任务(p.自我存在节点,*D);
                if(parent.状态==新任务治理承接状态::尚未初始化)return {};
                if((parent.状态!=新任务治理承接状态::已读回&&parent.状态!=新任务治理承接状态::既有终结任务)||!parent.任务)
                    throw std::logic_error("material parent association unreadable");
                项.前置请求=新任务前置语言输入请求{p,*D,parent.任务->节点};
                }
                const auto accepted=任务管理->提交前置语言输入筹办(*项.前置请求);
                switch(accepted.状态) {
                case 新任务工作接收状态::已接收:case 新任务工作接收状态::精确重复:
                    if(!accepted.工作||!accepted.处理结果.valid())throw std::logic_error("material acceptance future missing");
                    项.前置工作=accepted.工作;项.前置结果=accepted.处理结果;break;
                case 新任务工作接收状态::任务已占用:case 新任务工作接收状态::邮箱已满:case 新任务工作接收状态::尚未启动:return {};
                case 新任务工作接收状态::已停止接收:out.状态=新自我消息处理状态::停止取消;return out;
                default:throw std::logic_error("material manager acceptance rejected");
                }
            }
            if(项.前置结果.wait_for(std::chrono::milliseconds(0))!=std::future_status::ready)return {};
            const auto& 原=项.前置结果.get();
            const auto read=任务管理->读取前置语言输入筹办结果(*项.前置工作);
            if(!read)return {};
            if(read->原请求!=*项.前置工作||原.原请求!=read->原请求||!read->前置语言输入||!原.前置语言输入
                ||*原.前置语言输入!=*read->前置语言输入||read->前置语言输入->原请求!=*项.前置请求)
                throw std::logic_error("material future and independent result mismatch");
            const auto& r=*read->前置语言输入;
            if(r.状态==R::资源失败) {
                (void)任务管理->确认前置语言输入筹办已读回(*项.前置工作);
                throw std::logic_error("material resource failure is not a language gap");
            }
            const auto ack=接收认知->确认前置输入已筹办(r);
            // 无论正常、晚到、停用或非成功，已完成future的transport均须释放。
            const auto released=任务管理->确认前置语言输入筹办已读回(*项.前置工作);
            if(released!=新任务工作操作状态::已完成&&released!=新任务工作操作状态::无变化)throw std::logic_error("material transport release failed");
            if(ack==接收行动请求交接确认状态::资源失败||ack==接收行动请求交接确认状态::来源冲突)
                throw std::logic_error("material cognition result acknowledgement failed");
            out.前置语言输入=r;
            switch(r.状态) {
            case R::已读取:
                if(!r.材料)throw std::logic_error("read material absent");
                out.状态=r.材料->可重入原认知 ? 新自我消息处理状态::已复核 : 新自我消息处理状态::等待材料;break;
            case R::前提已变化:out.状态=新自我消息处理状态::等待材料;break;
            case R::来源停用:case R::停止取消:out.状态=新自我消息处理状态::停止取消;break;
            case R::输入不适用:out.状态=新自我消息处理状态::入口拒绝;break;
            case R::资源失败:throw std::logic_error("material resource failure is not a language gap");
            default:throw std::logic_error("material result state unknown");
            }
            if(ack==接收行动请求交接确认状态::输入拒绝)out.状态=新自我消息处理状态::停止取消;
            return out;
        }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"处理前置输入材料",L"输入父域、原工作读回或结果确认异常");}
    }
    std::optional<新自我消息处理结果> 处理行动请求承接(消息项& 项) {
        try {
        if(!任务管理||!任务服务||!需求服务||!初始化结果
            ||!初始化结果->自我存在||!初始化结果->完成服务需求||!项.消息.行动请求来源)
            throw std::logic_error("action request adoption dependencies missing");
        新自我消息处理结果 out;out.原消息=项.消息;
        const auto& source=*项.消息.行动请求来源;
        if(source.受请求者节点!=*初始化结果->自我存在)
            throw std::logic_error("action request addressee differs from self");
        if(项.管理结果.valid()) {
            if(项.管理结果.wait_for(std::chrono::milliseconds(0))!=std::future_status::ready)return {};
            const auto receipt=项.管理结果.get();
            if(!项.外部服务承接消息||receipt.原消息!=*项.外部服务承接消息)
                throw std::logic_error("action request acceptance receipt mismatch");
            out.任务承接=receipt;out.行动请求采用=项.行动请求采用材料;
            if(receipt.状态==新任务承接处理状态::停止取消) {
                out.状态=新自我消息处理状态::停止取消;return out;
            }
            if(const auto 映射=承接回执非成功状态(receipt.状态)) {
                out.状态=*映射;return out;
            }
            if(receipt.状态!=新任务承接处理状态::已承接
                &&receipt.状态!=新任务承接处理状态::已重新启用
                &&receipt.状态!=新任务承接处理状态::既有终结
                &&receipt.状态!=新任务承接处理状态::需求当前已满足)
                throw std::logic_error("action request task acceptance failed");
            if(!项.行动请求自我需求||!receipt.承接||!receipt.承接->任务)
                throw std::logic_error("action request accepted task payload missing");
            const auto demand=需求服务->读取需求治理信息(*项.行动请求自我需求);
            const auto task=任务服务->获取任务(receipt.承接->任务->节点);
            if(!demand||!demand->完整目标||demand->执行域!=新需求执行域::自我执行
                ||demand->需求主体存在节点!=source.受请求者节点
                ||demand->父需求节点!=初始化结果->完成服务需求
                ||demand->来源类型!=新需求来源类型::行动请求
                ||demand->行动请求来源!=std::optional{source}
                ||!task||task->需求节点!=demand->节点)
                throw std::logic_error("action request formal demand task readback mismatch");
            out.状态=新自我消息处理状态::任务已承接;return out;
        }
        if(停止源.stop_requested()) {
            out.状态=新自我消息处理状态::停止取消;
            out.行动请求采用=项.行动请求采用材料;return out;
        }
        // 首版正式请求全部承接：不读取权限、安全、合理性或方法可用性。
        const auto adopted=需求服务->建立或取得行动请求需求已核验(source,*初始化结果->完成服务需求);
        switch(adopted.状态) {
        case 新行动请求采用状态::资源失败:
        case 新行动请求采用状态::前提已变化:return {};
        case 新行动请求采用状态::依赖未就绪:
            // 结构类（需求类来源：语言或结构未初始化、来源读取未装配、核验依赖未就绪），不再按邮箱间隔重试。
            (void)结构依赖未就绪已停止(L"建立或取得行动请求需求已核验",static_cast<int>(adopted.状态),项.消息.消息编号);
            out.状态=新自我消息处理状态::停止取消;out.行动请求采用=项.行动请求采用材料;return out;
        case 新行动请求采用状态::来源不适用:
        case 新行动请求采用状态::父域不符:
        case 新行动请求采用状态::同源冲突:
            // 合同拒绝是外部请求不被采用的业务结果，与前置输入“输入不适用”同口径。
            out.状态=新自我消息处理状态::入口拒绝;out.行动请求采用=adopted;return out;
        case 新行动请求采用状态::已建立:
        case 新行动请求采用状态::已复用:break;
        default:throw std::logic_error("formal action request demand adoption inconsistent");
        }
        if(!adopted.需求节点||!adopted.需求列表项节点)
            throw std::logic_error("action request adoption success payload missing");
        if(项.行动请求自我需求&&项.行动请求自我需求!=adopted.需求节点)
            throw std::logic_error("same action request adopted another self demand");
        项.行动请求自我需求=adopted.需求节点;项.行动请求采用材料=adopted;
        if(停止源.stop_requested()) {
            out.状态=新自我消息处理状态::停止取消;out.行动请求采用=adopted;return out;
        }
        if(!项.外部服务承接消息) {
            新任务承接消息 message;
            message.消息编号=项.消息.消息编号;message.需求节点=*adopted.需求节点;
            message.依据=新任务承接依据::行动请求;message.行动请求来源=source;
            项.外部服务承接消息=std::move(message);
        }
        if(判定需求合法性(项.外部服务承接消息->需求节点,自我需求承接依据::行动请求)
            ==自我需求合法性判定::不合法) {
            out.状态=新自我消息处理状态::需求不合法未承接;out.行动请求采用=adopted;return out;
        }
        const auto accepted=任务管理->提交行动请求需求承接(*项.外部服务承接消息);
        switch(accepted.状态) {
        case 新任务承接接收状态::已接收:
        case 新任务承接接收状态::精确重复:
            if(!accepted.处理结果.valid())throw std::logic_error("action request manager future missing");
            项.管理结果=accepted.处理结果;return {};
        case 新任务承接接收状态::邮箱已满:
        case 新任务承接接收状态::资源失败:
        case 新任务承接接收状态::尚未启动:return {};
        case 新任务承接接收状态::已停止接收:
            out.状态=新自我消息处理状态::任务管理未就绪;out.行动请求采用=adopted;return out;
        default:throw std::logic_error("action request manager rejected formal self demand");
        }
        }catch(const std::bad_alloc&){return {};}
        catch(const std::length_error&){return {};}
        catch(const std::exception& e){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"处理行动请求承接",L"正式请求承接或任务回执不变量异常。",e.what());}
        catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"处理行动请求承接",L"正式请求承接发生未知工程异常。");}
    }

    // 调用者持有状态互斥。双根各自明确“需增加”时才形成普通自动承接；
    // 前置输入工种只借用服务根唯一T，不取代普通目标筹办或授予执行许可。
    void 安排自动承接(const 新自我本能需求复核结果& 复核) noexcept {
        if (!消息配置 || !任务管理
            || 复核.状态 != 新自我需求复核状态::已复核) return;
        const auto 安排一个 = [&](稳定编码 需求,
            const std::optional<新需求当前满足结果>& 根结果) noexcept {
            if (!根结果 || 根结果->状态 != 新需求当前满足状态::需增加) return;
            if (std::any_of(自动承接.begin(), 自动承接.end(),
                    [&](const auto& 项) {
                        return 项 && 项->消息.需求节点 == 需求;
                    })) return;
            auto 空位 = std::find_if(自动承接.begin(), 自动承接.end(),
                [](const auto& 项) { return !项; });
            if (空位 == 自动承接.end()) return;
            新任务承接消息 消息;
            消息.消息编号 = 取得自动消息编号();
            消息.需求节点 = 需求;
            if (复核.来源状态) {
                消息.依据 = 新任务承接依据::自我状态变化复核;
                消息.来源状态节点 = 复核.来源状态;
            } else {
                消息.依据 = 新任务承接依据::自我首次复核;
            }
            空位->emplace(自动承接项{
                std::move(消息), std::chrono::steady_clock::now(), {}});
        };
        安排一个(复核.安全需求, 复核.安全结果);
        安排一个(复核.服务需求, 复核.服务结果);
    }

    // true表示本自动项已经得到终结反馈；false表示保留原决议后重试。
    bool 处理自动承接(自动承接项& 项) noexcept {
        if (!任务管理) return false;
        if (!项.管理结果.valid()) {
            // 不合法：不发送，本自动项按终结结束，不重试。
            if (判定需求合法性(项.消息.需求节点, 自我需求承接依据::本能根复核)
                == 自我需求合法性判定::不合法) return true;
            const auto 接收 = 任务管理->提交消息(项.消息);
            switch (接收.状态) {
            case 新任务承接接收状态::已接收:
            case 新任务承接接收状态::精确重复:
                项.管理结果 = 接收.处理结果;
                if (!项.管理结果.valid()) return true;
                break;
            case 新任务承接接收状态::邮箱已满:
            case 新任务承接接收状态::资源失败:
            case 新任务承接接收状态::尚未启动:
                return false;
            case 新任务承接接收状态::过期决议:
            case 新任务承接接收状态::已停止接收:
            case 新任务承接接收状态::入口拒绝:
            case 新任务承接接收状态::身份冲突:
            case 新任务承接接收状态::内部错误:
            default:
                return true;
            }
        }
        if (项.管理结果.wait_for(std::chrono::milliseconds(0))
            != std::future_status::ready) return false;
        (void)项.管理结果.get();
        return true;
    }

    新自我消息处理结果 处理消息(const 新自我治理消息& 消息) noexcept {
        新自我消息处理结果 结果;
        结果.原消息 = 消息;
        switch (消息.类型) {
        case 新自我消息类型::事实变化:
            if (任务管理) {
                // 回执仅确认异步唤醒已接纳，不证明该状态已匹配某个执行。
                结果.结果状态接收状态 =
                    任务管理->提交结果状态通知(消息.来源状态节点);
            }
            结果.需求复核 = 复核需求(消息.来源状态节点);
            break;
        default:
            结果.状态 = 新自我消息处理状态::入口拒绝;
            return 结果;
        }
        switch (结果.需求复核->状态) {
        case 新自我需求复核状态::已复核:
            结果.状态 = 新自我消息处理状态::已复核;
            break;
        case 新自我需求复核状态::来源状态不存在:
            结果.状态 = 新自我消息处理状态::入口拒绝;
            break;
        case 新自我需求复核状态::需求材料未就绪:
        case 新自我需求复核状态::世界树未初始化:
            结果.状态 = 新自我消息处理状态::等待材料;
            break;
        case 新自我需求复核状态::需求比较失败:
            结果.状态 = 新自我消息处理状态::比较失败;
            break;
        case 新自我需求复核状态::自我或根关系不一致:
            结果.状态 = 新自我消息处理状态::内部不一致;
            break;
        default:
            结果.状态 = 新自我消息处理状态::内部错误;
            break;
        }
        return 结果;
    }

    // 空结果表示本次已尝试、尚待manager结果或未接纳的资源重试。
    std::optional<新自我消息处理结果> 处理任务承接(消息项& 项) {
        新自我消息处理结果 结果;
        结果.原消息 = 项.消息;
        if (!任务管理) {
            结果.状态 = 新自我消息处理状态::任务管理未就绪;
            return 结果;
        }
        if (!项.管理结果.valid()) {
            if (判定需求合法性(项.消息.需求节点, 自我需求承接依据::显式请求)
                == 自我需求合法性判定::不合法) {
                结果.状态 = 新自我消息处理状态::需求不合法未承接;
                return 结果;
            }
            const auto 接收 = 任务管理->提交消息({项.消息.消息编号, 项.消息.需求节点});
            结果.管理接收状态 = 接收.状态;
            switch (接收.状态) {
            case 新任务承接接收状态::已接收:
            case 新任务承接接收状态::精确重复:
                项.管理结果 = 接收.处理结果;
                if (!项.管理结果.valid()) {
                    结果.状态 = 新自我消息处理状态::内部不一致;
                    return 结果;
                }
                break;
            case 新任务承接接收状态::邮箱已满:
            case 新任务承接接收状态::资源失败:
                return std::nullopt;
            case 新任务承接接收状态::过期决议:
                结果.状态 = 新自我消息处理状态::入口拒绝;
                return 结果;
            case 新任务承接接收状态::尚未启动:
            case 新任务承接接收状态::已停止接收:
                结果.状态 = 新自我消息处理状态::任务管理未就绪;
                return 结果;
            case 新任务承接接收状态::内部错误:
                结果.状态 = 新自我消息处理状态::内部错误;
                return 结果;
            default:
                结果.状态 = 新自我消息处理状态::入口拒绝;
                return 结果;
            }
        }
        if (项.管理结果.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
            return std::nullopt;
        结果.管理接收状态 = 新任务承接接收状态::已接收;
        结果.任务承接 = 项.管理结果.get();
        const auto& 承接 = *结果.任务承接;
        if (承接.原消息 != 新任务承接消息{项.消息.消息编号, 项.消息.需求节点}) {
            结果.状态 = 新自我消息处理状态::内部不一致;
            return 结果;
        }
        if (承接.状态 == 新任务承接处理状态::已承接
            || 承接.状态 == 新任务承接处理状态::已重新启用
            || 承接.状态 == 新任务承接处理状态::既有终结) {
            if (!承接.承接 || !承接.承接->任务) {
                结果.状态 = 新自我消息处理状态::内部不一致;
                return 结果;
            }
            // manager回传只作定位；self独立核验当前任务和Vt，不推进状态。
            结果.任务独立读回 = 任务服务->读回需求任务(
                项.消息.需求节点, 承接.承接->任务->节点);
            switch (结果.任务独立读回->状态) {
            case 新任务治理承接状态::已读回:
        case 新任务治理承接状态::已重新启用:
                结果.状态 = 新自我消息处理状态::任务已承接; break;
            case 新任务治理承接状态::既有终结任务:
                结果.状态 = 新自我消息处理状态::既有终结任务; break;
            default:
                结果.状态 = 新自我消息处理状态::任务承接需核查; break;
            }
        } else if (const auto 映射 = 承接回执非成功状态(承接.状态)) {
            结果.状态 = *映射;
        } else {
            switch (承接.状态) {
            case 新任务承接处理状态::停止取消:
                结果.状态 = 新自我消息处理状态::停止取消; break;
            case 新任务承接处理状态::内部错误:
                结果.状态 = 新自我消息处理状态::内部错误; break;
            case 新任务承接处理状态::需求当前已满足:
                结果.状态 = 新自我消息处理状态::已复核; break;
            default:
                结果.状态 = 新自我消息处理状态::任务承接需核查; break;
            }
        }
        return 结果;
    }

    // 消息项持有逐项发布和交接进度；领域调用期间不持self邮箱锁。
    static std::vector<二次特征概念输入定义> 子需求比较输入(稳定编码 类型) {
        return {{0, 二次特征输入角色::左输入, 二次特征输入来源种类::特征准确值, 类型},
            {1, 二次特征输入角色::右输入, 二次特征输入来源种类::特征准确值, 类型}};
    }

    std::optional<二次特征算法> 子需求比较算法(const 新任务条件子需求项& 目标) const {
        const auto 事实 = 世界树服务->获取事实服务引用();
        const auto 域 = 事实.特征概念服务.获取特征概念(目标.目标值域概念节点);
        if (!域) return std::nullopt;
        const bool 概念集合 = std::holds_alternative<特征概念概念集合值域>(域->值域);
        return 目标.集合目标 || 目标.存在引用目标 || 概念集合
            ? 二次特征算法::特征材料相似度 : 二次特征算法::有向特征差值;
    }

    新引用内容目标状态 复核内容子需求投影(const 新任务条件子需求项& 目标,
        std::optional<稳定编码> 已发布需求={}) const {
            if(!目标.引用内容定位||!目标.目标区间组.empty()||目标.单位||目标.集合目标)
                return 新引用内容目标状态::定义不合法;
            if(已发布需求){
                const auto D=需求服务->获取需求(*已发布需求);
                if(!D)throw std::logic_error("已发布内容子需求不可读");
                if(D->活动状态!=新需求活动状态::活动)return 新引用内容目标状态::引用前提已变化;
            }
            新需求目标定义 定义;定义.目标特征类型概念节点=目标.目标特征类型概念节点;
            定义.目标形式=新需求目标形式::值;定义.目标值域概念节点=目标.目标值域概念节点;定义.引用内容定位=目标.引用内容定位;
            const auto 读=需求服务->求值引用内容条件目标({目标.目标存在节点,定义});
            if(读.状态==新引用内容目标状态::已满足||读.状态==新引用内容目标状态::未满足||读.状态==新引用内容目标状态::分支待定)
                return 读.实际特征节点==std::optional<稳定编码>{目标.实际特征节点} ? 读.状态 : 新引用内容目标状态::引用前提已变化;
            return 读.状态;
    }
    bool 子需求目标投影一致(const 新任务条件子需求项& 目标) const {
        if(目标.引用内容定位) {
            const auto s=复核内容子需求投影(目标);
            if(s==新引用内容目标状态::资源失败)throw std::bad_alloc{};
            if(s==新引用内容目标状态::定义不合法)throw std::logic_error("正式内容子目标定义损坏");
            return s==新引用内容目标状态::已满足||s==新引用内容目标状态::未满足||s==新引用内容目标状态::分支待定;
        }
        const auto 事实 = 世界树服务->获取事实服务引用();
        const auto 域 = 事实.特征概念服务.获取特征概念(目标.目标值域概念节点);
        if (!域 || 域->单位 != 目标.单位) return false;
        // 存在引用目标（R96 切片4；2026-11-10 无关系的存在引用值域条件）：目标值域须是存在引用值域概念。
        if (目标.存在引用目标)
            return 目标.目标区间组.empty() && !目标.集合目标
                && 域->材料类型 == 特征概念材料物理类型::存在节点引用
                && std::holds_alternative<特征概念存在引用值域>(域->值域);
        if (目标.集合目标) {
            const auto* 集合 = std::get_if<特征概念任务集合值域>(&域->值域);
            return 目标.目标区间组.empty() && !目标.单位
                && 域->材料类型 == 特征概念材料物理类型::U64数组
                && 域->比较规则 == 特征概念比较规则::服务任务集合身份判等
                && 集合 && *集合 == *目标.集合目标;
        }
        if (std::holds_alternative<特征概念概念集合值域>(域->值域))
            return 目标.目标区间组.empty() && !目标.单位 && !目标.新增成员见证要求
                && 域->材料类型 == 特征概念材料物理类型::U64数组
                && 域->比较规则 == 特征概念比较规则::概念集合身份判等;
        const auto* 区间 = std::get_if<std::vector<特征概念I64闭区间>>(&域->值域);
        return 域->材料类型 == 特征概念材料物理类型::I64标量
            && 区间 && !区间->empty() && *区间 == 目标.目标区间组;
    }

    bool 正式条件子需求一致(const 新子需求采用项& 采用, 稳定编码 父需求, 稳定编码 父任务) const {
        if (!采用.需求节点) return false;
        const auto d = 需求服务->获取需求(*采用.需求节点);
        if(采用.来源前驱模板) {
            if(采用.原提议!=新任务条件子需求项{}||!采用.来源前驱具体目标||!d
                ||d->父需求节点!=父需求||d->来源类型!=新需求来源类型::任务子目标
                ||d->来源承接节点!=父任务||d->执行域!=新需求执行域::自我执行
                ||d->复合来源||新需求完整目标{d->目标存在节点,d->目标}!=*采用.来源前驱具体目标) {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
                try{
                    std::wstring t=L"需求="+std::to_wstring(采用.需求节点->值)+L"；原提议空="+std::to_wstring(采用.原提议==新任务条件子需求项{} ? 1 : 0)
                        +L"；有具体目标="+std::to_wstring(采用.来源前驱具体目标 ? 1 : 0)+L"；有D="+std::to_wstring(d ? 1 : 0);
                    if(d){
                        t+=L"；父需求="+std::to_wstring(d->父需求节点 ? d->父需求节点->值 : 0)+L"/期望"+std::to_wstring(父需求.值)
                          +L"；来源类型="+std::to_wstring(d->来源类型 ? static_cast<int>(*d->来源类型) : -1)
                          +L"；承接="+std::to_wstring(d->来源承接节点 ? d->来源承接节点->值 : 0)+L"/期望"+std::to_wstring(父任务.值)
                          +L"；执行域="+std::to_wstring(static_cast<int>(d->执行域))+L"；复合="+std::to_wstring(d->复合来源 ? 1 : 0);
                        if(采用.来源前驱具体目标){
                            const auto& g=*采用.来源前驱具体目标;
                            t+=L"；目标存在="+std::to_wstring(d->目标存在节点.值)+L"/"+std::to_wstring(g.目标存在节点.值)
                              +L"；FT="+std::to_wstring(d->目标.目标特征类型概念节点.值)+L"/"+std::to_wstring(g.目标.目标特征类型概念节点.值)
                              +L"；域同="+std::to_wstring(d->目标.目标值域概念节点==g.目标.目标值域概念节点 ? 1 : 0)
                              +L"；比较同="+std::to_wstring(d->目标.目标比较关系概念节点==g.目标.目标比较关系概念节点 ? 1 : 0)
                              +L"；形式同="+std::to_wstring(d->目标.目标形式==g.目标.目标形式 ? 1 : 0)
                              +L"；内容同="+std::to_wstring(d->目标.引用内容定位==g.目标.引用内容定位 ? 1 : 0)
                              +L"；方向同="+std::to_wstring(d->目标.目标变化方向==g.目标.目标变化方向 ? 1 : 0)
                              +L"；见证同="+std::to_wstring(d->目标.新增成员来源见证==g.目标.新增成员来源见证 ? 1 : 0);
                        }
                    }
                    (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"自我因果采用",L"来源子需求不一致",t);
                }catch(...){}
#endif
                throw std::logic_error("来源子需求固定身份或具体目标损坏");
            }
            const bool 仍活动=d->活动状态==新需求活动状态::活动;
            const auto& 模板=*采用.来源前驱模板;
            const auto& 目标=*采用.来源前驱具体目标;
            if(目标.目标.目标形式!=新需求目标形式::值
                ||目标.目标.目标特征类型概念节点!=模板.目标槽类型
                ||目标.目标.目标值域概念节点!=模板.要求概念
                ||目标.目标.目标变化方向||目标.目标.新增成员来源见证)
                throw std::logic_error("来源子需求具体目标与模板不符");
            if(模板.种类==新来源前驱前端种类::取得服务对象) {
                if(采用.来源前驱实例||模板.H||模板.D||!模板.正式根路径.empty()
                    ||目标.目标.引用内容定位||!目标.目标.目标比较关系概念节点)
                    throw std::logic_error("来源H前提混入内容实例或非法比较");
                const auto 事实=世界树服务->获取事实服务引用();
                const auto 域=事实.特征概念服务.获取特征概念(模板.要求概念);
                const auto 比较=事实.二次特征概念服务.获取二次特征概念(*目标.目标.目标比较关系概念节点);
                const bool 比较合法=域&&域->材料类型==特征概念材料物理类型::存在节点引用
                    &&std::holds_alternative<特征概念存在引用值域>(域->值域)
                    &&比较&&比较->算法==二次特征算法::特征材料相似度
                    &&比较->输入定义组==子需求比较输入(模板.目标槽类型)
                    &&比较->参照特征概念节点==模板.要求概念
                    &&std::find(比较->结果域种类组.begin(),比较->结果域种类组.end(),
                        二次特征结果域种类::包含于参照值域)!=比较->结果域种类组.end();
                if(!比较合法)throw std::logic_error("来源H前提正式引用域或比较定义损坏");
                return 仍活动;
            }
            if((模板.种类!=新来源前驱前端种类::既有需求内容
                    &&模板.种类!=新来源前驱前端种类::待实例化需求内容)
                ||!采用.来源前驱实例||!模板.H||目标.目标.目标比较关系概念节点)
                throw std::logic_error("来源内容子需求缺少固定实例或混入比较");
            const auto& 实例=*采用.来源前驱实例;
            if(!实例.骨架就绪||实例.键.父取得任务!=父任务||实例.父需求!=父需求
                ||实例.键.服务对象!=*模板.H||目标.目标存在节点!=*模板.H
                ||实例.键.目标槽类型!=模板.目标槽类型||实例.键.正式根路径!=模板.正式根路径
                ||实例.完整内容概念!=模板.要求概念
                ||实例.复用指定已有需求!=(模板.种类==新来源前驱前端种类::既有需求内容)
                ||(实例.复用指定已有需求 ? 模板.D!=实例.需求节点 : 模板.D.has_value())
                ||目标.目标.引用内容定位!=std::optional<新需求引用内容定位>{新需求引用内容定位{实例.需求节点,模板.正式根路径}})
                throw std::logic_error("来源内容子需求固定实例与模板不符");
            const auto 读回=任务服务->读取来源前驱目标实例(实例.键);
            switch(读回.状态) {
            case 新服务取得目标实例状态::资源失败:throw std::bad_alloc{};
            case 新服务取得目标实例状态::前提已变化:case 新服务取得目标实例状态::依赖未就绪:
            case 新服务取得目标实例状态::已出生待补骨架:return false;
            case 新服务取得目标实例状态::已复用:
                if(读回.信息!=采用.来源前驱实例||读回.完整保护.empty())
                    throw std::logic_error("来源内容实例成功读回与固定身份不符");
                return 仍活动;
            default:throw std::logic_error("来源内容固定实例正式读取状态不合法");
            }
        }
        if(采用.来源前驱实例||采用.来源前驱具体目标)return false;
        const auto& 目标 = 采用.原提议;
        if(目标.引用内容定位) {
            return d&&d->父需求节点==父需求&&d->来源类型==新需求来源类型::任务子目标
                &&d->来源承接节点==父任务&&d->目标存在节点==目标.目标存在节点
                &&!d->复合来源
                &&d->目标.目标形式==新需求目标形式::值&&!d->目标.目标比较关系概念节点&&!d->目标.目标变化方向
                &&d->目标.目标特征类型概念节点==目标.目标特征类型概念节点&&d->目标.目标值域概念节点==目标.目标值域概念节点
                &&d->目标.引用内容定位==目标.引用内容定位&&目标.目标区间组.empty()&&!目标.单位&&!目标.集合目标;
        }
        if (!d || d->父需求节点 != 父需求 || d->来源类型 != 新需求来源类型::任务子目标
            || d->来源承接节点 != 父任务 || d->目标存在节点 != 目标.目标存在节点
            || d->活动状态 == 新需求活动状态::失活 || d->目标.目标形式 != 新需求目标形式::值
            || d->目标.目标特征类型概念节点 != 目标.目标特征类型概念节点
            || d->目标.目标值域概念节点 != 目标.目标值域概念节点
            || d->目标.目标变化方向 || !d->目标.目标比较关系概念节点 || !子需求目标投影一致(目标)) return false;
        if(目标.新增成员见证要求) {
            if(!d->目标.新增成员来源见证||d->目标.新增成员来源见证->要求!=*目标.新增成员见证要求)return false;
        } else if(d->目标.新增成员来源见证)return false;
        const auto 事实 = 世界树服务->获取事实服务引用();
        const auto 比较 = 事实.二次特征概念服务.获取二次特征概念(*d->目标.目标比较关系概念节点);
        return 比较 && 比较->算法 == 子需求比较算法(目标)
            && 比较->输入定义组 == 子需求比较输入(目标.目标特征类型概念节点)
            && 比较->参照特征概念节点 == 目标.目标值域概念节点
            && std::find(比较->结果域种类组.begin(), 比较->结果域种类组.end(),
                二次特征结果域种类::包含于参照值域) != 比较->结果域种类组.end();
    }

    新自我消息处理结果 处理条件子需求提议(
        消息项& 项, const 新任务筹办处理结果& 筹办) noexcept {
        新自我消息处理结果 结果;
        结果.原消息 = 项.消息;
        结果.状态 = 新自我消息处理状态::任务筹办需核查;
        try {
            if (!任务管理 || !项.消息.筹办反馈) return 结果;
            const auto& 定位 = *项.消息.筹办反馈;
            const auto 等待 = [&]() {
                结果.状态 = 新自我消息处理状态::已交接未收回;
                return 结果;
            };
            if (!项.条件子需求回执) {
                if (!筹办.独立子需求提议
                    || 筹办.独立子需求提议->状态
                        != 新任务条件子需求提议状态::已形成
                    || 筹办.独立子需求提议->子需求项组.empty()) return 结果;
                新子需求采用回执 回执;
                回执.原提议 = *筹办.独立子需求提议;
                for (const auto& 子项 : 回执.原提议.子需求项组)
                    回执.项组.push_back({子项});
                const auto 数量 = 回执.项组.size();
                // 首次领域写入前准备所有槽位。分配失败可从无发布状态重试。
                项.条件子需求承接消息组.resize(数量);
                项.条件子需求承接结果组.resize(数量);
                项.条件子需求承接完成组.resize(数量, false);
                for (auto& 消息 : 项.条件子需求承接消息组) {
                    消息.消息编号 = 取得自动消息编号();
                    消息.依据 = 新任务承接依据::自我条件子需求;
                }
                项.条件子需求回执 = std::move(回执);
            }
            auto& 回执 = *项.条件子需求回执;
            const auto& 原提议 = 回执.原提议;
            const auto 事实 = 世界树服务->获取事实服务引用();
            auto& 概念服务 = 事实.二次特征概念服务;
            const auto 来源一致 = [&](const 新任务条件子需求提议& 当前) {
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
            };
            const auto 本项等材料 = [&](const 新任务条件子需求提议& 当前,
                                       const 新任务条件子需求项& 目标) {
                if(!当前.分解依据)return true;
                const auto& 未解析=当前.分解依据->未解析条件序号组;
                const auto 未取得=[&](std::uint64_t 序号){
                    return std::find(未解析.begin(),未解析.end(),序号)!=未解析.end();
                };
                if(!目标.集合目标)return 未取得(目标.条件项序号);
                return std::any_of(原提议.已知条件项组.begin(),原提议.已知条件项组.end(),[&](const auto& x){
                    return x.目标存在节点==目标.目标存在节点
                        &&x.目标特征类型概念节点==目标.目标特征类型概念节点&&未取得(x.条件项序号);
                });
            };
            const auto 输入定义 = [](稳定编码 类型) {
                return 子需求比较输入(类型);
            };
            const auto 目标投影一致 = [&](const 新任务条件子需求项& 目标) {
                return 子需求目标投影一致(目标);
            };
            const auto 目标比较算法 = [&](const 新任务条件子需求项& 目标) {
                return 子需求比较算法(目标);
            };
            const auto 正式需求一致 = [&](const 新子需求采用项& 采用) {
                return 正式条件子需求一致(采用, 原提议.父需求节点, 原提议.任务节点);
            };
            const auto 停止剩余 = [&](新子需求采用状态 状态) {
                for (auto& 采用 : 回执.项组) {
                    if (采用.状态 == 新子需求采用状态::未处理
                        || 采用.状态 == 新子需求采用状态::资源失败)
                        采用.状态 = 状态;
                }
            };
            if (!回执.已完成处理) {
                for (auto& 采用 : 回执.项组) {
                    if (采用.状态 == 新子需求采用状态::已建立
                        || 采用.状态 == 新子需求采用状态::已复用) {
                        // 资源异常可能发生在发布后读回途中；重试只重读，
                        // 不重新发布，也不把首次“已建立”改成“已复用”。
                        if (!正式需求一致(采用)) {
                            停止剩余(新子需求采用状态::结构错误);
                            break;
                        }
                        if(采用.原提议.引用内容定位){
                            const auto s=复核内容子需求投影(采用.原提议,采用.需求节点);
                            if(s==新引用内容目标状态::资源失败||s==新引用内容目标状态::依赖未就绪||s==新引用内容目标状态::未支持)return 等待();
                            if(s==新引用内容目标状态::引用前提已变化){停止剩余(新子需求采用状态::来源过期);break;}
                            if(s==新引用内容目标状态::定义不合法){停止剩余(新子需求采用状态::结构错误);break;}
                        }
                        continue;
                    }
                    if (采用.状态 != 新子需求采用状态::未处理
                        && 采用.状态 != 新子需求采用状态::资源失败) continue;
                    {
                        std::lock_guard 锁(状态互斥);
                        if (停止已请求) {
                            结果.状态 = 新自我消息处理状态::停止取消;
                            return 结果;
                        }
                    }
                    const auto 当前 = 任务服务->提议任务条件子需求(定位.任务节点);
                    if (当前.状态 == 新任务条件子需求提议状态::结构依赖未就绪) {
                        (void)结构依赖未就绪已停止(L"处理条件子需求提议/提议任务条件子需求",static_cast<int>(当前.状态),定位.任务节点.值);
                        结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                    }
                    if (当前.状态 == 新任务条件子需求提议状态::资源失败) return 等待();
                    if(当前.状态==新任务条件子需求提议状态::材料未到
                        ||当前.状态==新任务条件子需求提议状态::需求信息待完善)return 等待();
                    if(采用.原提议.引用内容定位){
                        const auto s=复核内容子需求投影(采用.原提议);
                        if(s==新引用内容目标状态::资源失败||s==新引用内容目标状态::依赖未就绪||s==新引用内容目标状态::未支持)return 等待();
                        if(s==新引用内容目标状态::引用前提已变化){停止剩余(新子需求采用状态::来源过期);break;}
                        if(s==新引用内容目标状态::定义不合法){停止剩余(新子需求采用状态::结构错误);break;}
                        if(当前.状态==新任务条件子需求提议状态::材料未到||当前.状态==新任务条件子需求提议状态::暂不支持)return 等待();
                    }
                    if (!来源一致(当前)) {
                        停止剩余(新子需求采用状态::来源过期);
                        break;
                    }
                    if (std::find(当前.子需求项组.begin(), 当前.子需求项组.end(),
                            采用.原提议) == 当前.子需求项组.end()) {
                        if(本项等材料(当前,采用.原提议))return 等待();
                        采用.状态 = 新子需求采用状态::已无需要;
                        continue;
                    }
                    const auto& 目标 = 采用.原提议;
                    if (!目标.引用内容定位&&!目标投影一致(目标)) {
                        停止剩余(新子需求采用状态::来源过期);
                        break;
                    }
                    if(目标.引用内容定位) {
                        新普通需求建立请求 请求;
                        请求.父需求节点=原提议.父需求节点;请求.目标存在节点=目标.目标存在节点;
                        请求.目标.目标特征类型概念节点=目标.目标特征类型概念节点;
                        请求.目标.目标形式=新需求目标形式::值;请求.目标.目标值域概念节点=目标.目标值域概念节点;
                        请求.目标.引用内容定位=目标.引用内容定位;
                        请求.来源类型=新需求来源类型::任务子目标;请求.来源承接节点=原提议.任务节点;
                        const auto 建立=需求服务->建立或取得普通需求已核验(请求,true);
                        if(建立.状态==新需求操作状态::资源失败||建立.状态==新需求操作状态::目标内容依赖未就绪
                            ||建立.状态==新需求操作状态::目标内容未支持)return 等待();
                        if(建立.状态==新需求操作状态::前提已变化){停止剩余(新子需求采用状态::来源过期);break;}
                        if(!建立.需求节点||(建立.状态!=新需求操作状态::已建立&&建立.状态!=新需求操作状态::已复用)){
                            停止剩余(新子需求采用状态::结构错误);break;
                        }
                        采用.需求节点=建立.需求节点;采用.状态=建立.状态==新需求操作状态::已建立 ? 新子需求采用状态::已建立 : 新子需求采用状态::已复用;
                        if(!正式需求一致(采用)){停止剩余(新子需求采用状态::结构错误);break;}
                        const auto 当前内容=复核内容子需求投影(目标,采用.需求节点);
                        if(当前内容==新引用内容目标状态::资源失败||当前内容==新引用内容目标状态::依赖未就绪
                            ||当前内容==新引用内容目标状态::未支持)return 等待();
                        if(当前内容==新引用内容目标状态::引用前提已变化){停止剩余(新子需求采用状态::来源过期);break;}
                        if(当前内容==新引用内容目标状态::定义不合法){停止剩余(新子需求采用状态::结构错误);break;}
                        {std::lock_guard 锁(状态互斥);请求关联复核已加锁(建立.状态==新需求操作状态::已建立);}
                        continue;
                    }
                    const auto 输入 = 输入定义(目标.目标特征类型概念节点);
                    const auto 算法 = 目标比较算法(目标);
                    if (!算法) return 等待();
                    const auto 类型 = 概念服务.建立或取得默认二次特征概念(
                        *算法, 输入, 目标.目标值域概念节点);
                    if (类型.状态 == 二次特征概念操作状态::资源失败) return 等待();
                    if (!类型.概念节点 || (类型.状态 != 二次特征概念操作状态::已建立
                            && 类型.状态 != 二次特征概念操作状态::已复用)) {
                        停止剩余(新子需求采用状态::结构错误);
                        break;
                    }
                    const auto 关系 = 概念服务.建立或取得参照结果域概念(
                        *类型.概念节点, 二次特征结果域种类::包含于参照值域);
                    if (关系.状态 == 二次特征概念操作状态::资源失败) return 等待();
                    if (!关系.概念节点 || (关系.状态 != 二次特征概念操作状态::已建立
                            && 关系.状态 != 二次特征概念操作状态::已复用)) {
                        停止剩余(新子需求采用状态::结构错误);
                        break;
                    }
                    // 概念建立与需求发布不是跨owner事务，发布前再次检查来源。
                    const auto 发布前 = 任务服务->提议任务条件子需求(定位.任务节点);
                    if (发布前.状态 == 新任务条件子需求提议状态::结构依赖未就绪) {
                        (void)结构依赖未就绪已停止(L"处理条件子需求提议/提议任务条件子需求",static_cast<int>(发布前.状态),定位.任务节点.值);
                        结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                    }
                    if (发布前.状态 == 新任务条件子需求提议状态::资源失败) return 等待();
                    if(发布前.状态==新任务条件子需求提议状态::材料未到
                        ||发布前.状态==新任务条件子需求提议状态::需求信息待完善)return 等待();
                    if (!来源一致(发布前)) {
                        停止剩余(新子需求采用状态::来源过期);
                        break;
                    }
                    if (std::find(发布前.子需求项组.begin(), 发布前.子需求项组.end(),
                            目标) == 发布前.子需求项组.end()) {
                        if(本项等材料(发布前,目标))return 等待();
                        采用.状态 = 新子需求采用状态::已无需要;
                        continue;
                    }
                    新普通需求建立请求 请求;
                    请求.父需求节点 = 原提议.父需求节点;
                    请求.目标存在节点 = 目标.目标存在节点;
                    请求.目标.目标特征类型概念节点 = 目标.目标特征类型概念节点;
                    请求.目标.目标形式 = 新需求目标形式::值;
                    请求.目标.目标值域概念节点 = 目标.目标值域概念节点;
                    请求.目标.目标比较关系概念节点 = 关系.概念节点;
                    请求.来源类型 = 新需求来源类型::任务子目标;
                    请求.来源承接节点 = 原提议.任务节点;
                    const auto 建立 = 需求服务->建立或取得普通需求已核验(请求, true);
                    // 与引用内容分支同口径：依赖未就绪等待，前提已变化按来源过期。
                    if (建立.状态 == 新需求操作状态::资源失败
                        || 建立.状态 == 新需求操作状态::目标内容依赖未就绪) return 等待();
                    if (建立.状态 == 新需求操作状态::前提已变化) {
                        停止剩余(新子需求采用状态::来源过期);
                        break;
                    }
                    if (!建立.需求节点 || (建立.状态 != 新需求操作状态::已建立
                            && 建立.状态 != 新需求操作状态::已复用)) {
                        停止剩余(新子需求采用状态::结构错误);
                        break;
                    }
                    采用.需求节点 = 建立.需求节点;
                    采用.状态 = 建立.状态 == 新需求操作状态::已建立
                        ? 新子需求采用状态::已建立 : 新子需求采用状态::已复用;
                    if (!正式需求一致(采用)) {
                        // 已发布的需求保留，不撤销、不把未读回对象当采用成功。
                        // 把前缀及剩余项的结论交回；manager仍须独立读回，
                        // 不能因为领域写入返回了编码就解开父任务阻塞。
                        停止剩余(新子需求采用状态::结构错误);
                        break;
                    }
                    {
                        std::lock_guard 锁(状态互斥);
                        请求关联复核已加锁(建立.状态 == 新需求操作状态::已建立);
                    }
                }
                回执.已完成处理 = true;
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                try {
                    std::wstring 文 = L"任务=" + std::to_wstring(定位.任务节点.值);
                    for (const auto& 采用 : 回执.项组)
                        文 += L" 项状态=" + std::to_wstring(static_cast<int>(采用.状态))
                            + L" 需求=" + std::to_wstring(采用.需求节点 ? 采用.需求节点->值 : 0)
                            + L" 目标存在=" + std::to_wstring(采用.原提议.目标存在节点.值)
                            + L" 目标FT=" + std::to_wstring(采用.原提议.目标特征类型概念节点.值)
                            + L" 目标域=" + std::to_wstring(采用.原提议.目标值域概念节点.值);
                    (void)调试日志<调试切片::PARAM_BINDING>(L"自我筹办反馈", L"条件子需求逐项", 文);
                } catch (...) {}
#endif
            }
            新自我筹办裁决 裁决;
            裁决.定位 = 定位;
            裁决.状态 = 新自我筹办裁决状态::已处理条件子需求;
            裁决.子需求回执 = 回执;
            结果.筹办裁决 = 裁决;
            if (!项.条件子需求回执已接收) {
                // R111：发送前逐项判定合法性。S0：任务管理线程要求条件回执与提议逐项齐全
                // （项数相等、每项状态在封闭集合内），不能只提交合法项；有不合法项时整组
                // 不提交，只发“子需求不合法”通知（施工记录偏差）。
                std::vector<稳定编码> 已采用子需求;
                for (const auto& 采用 : 回执.项组)
                    if (采用.需求节点 && (采用.状态 == 新子需求采用状态::已建立
                            || 采用.状态 == 新子需求采用状态::已复用))
                        已采用子需求.push_back(*采用.需求节点);
                const auto 分组 = 判定子需求组(
                    定位.任务节点, 已采用子需求, 自我需求承接依据::条件子需求);
                新子需求不合法候选 候选;
                候选.种类 = 新子需求不合法候选种类::条件方法;
                候选.方法节点 = 回执.原提议.方法节点;
                候选.配对节点 = 回执.原提议.配对节点;
                if (回执.原提议.分解依据) 候选.执行分支节点 = 回执.原提议.分解依据->执行分支节点;
                if (const auto 推进 = 通知子需求合法性(定位, 分组, 候选)) {
                    if (*推进 == 子需求通知推进::重投) return 等待();
                    if (*推进 == 子需求通知推进::已停止) {
                        结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                    }
                    if (!分组.不合法.empty()) {
                        结果.状态 = 新自我消息处理状态::需求不合法未承接; return 结果;
                    }
                }
                const auto 接收 = 任务管理->提交自我筹办裁决(裁决);
                结果.筹办裁决接收状态 = 接收;
                if (接收 == 新自我筹办裁决接收状态::资源失败) return 等待();
                if (接收 != 新自我筹办裁决接收状态::已接收
                    && 接收 != 新自我筹办裁决接收状态::精确重复) {
                    结果.状态 = 接收 == 新自我筹办裁决接收状态::定位已变化
                        ? 新自我消息处理状态::任务筹办定位已变化
                        : 新自我消息处理状态::任务筹办需核查;
                    return 结果;
                }
                项.条件子需求回执已接收 = true;
            }
            bool 尚有交接 = false;
            for (std::size_t i = 0; i < 回执.项组.size(); ++i) {
                if (项.条件子需求承接完成组[i]) continue;
                const auto& 采用 = 回执.项组[i];
                if (采用.状态 != 新子需求采用状态::已建立
                    && 采用.状态 != 新子需求采用状态::已复用) {
                    项.条件子需求承接完成组[i] = true;
                    continue;
                }
                auto& 异步 = 项.条件子需求承接结果组[i];
                auto& 消息 = 项.条件子需求承接消息组[i];
                if (!异步.valid()) {
                    if (!正式需求一致(采用)) {
                        结果.状态 = 新自我消息处理状态::内部不一致;
                        return 结果;
                    }
                    const auto 身份 = 任务服务->复核已发布条件子需求身份(
                        定位.任务节点, {采用.原提议, *采用.需求节点});
                    if(采用.原提议.引用内容定位&&身份==新任务已发布条件子需求核验状态::资源失败)return 等待();
                    if (身份 == 新任务已发布条件子需求核验状态::资源失败
                        || 身份 == 新任务已发布条件子需求核验状态::结构不一致)
                        工程异常终止(L"新_自我线程类.cpp", __LINE__,
                            L"处理条件子需求提议", L"已发布子需求身份读取失败");
                    if (身份 != 新任务已发布条件子需求核验状态::身份一致) {
                        结果.状态 = 新自我消息处理状态::任务筹办需核查;
                        return 结果;
                    }
                    消息.需求节点 = *采用.需求节点;
                    const auto 接收 = 任务管理->提交已确认条件子需求(定位, 消息);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                    try { (void)调试日志<调试切片::PARAM_BINDING>(L"消息收发", L"自我发条件子需求承接",
                        L"任务=" + std::to_wstring(定位.任务节点.值) + L" 子需求=" + std::to_wstring(消息.需求节点.值)
                        + L" 消息=" + std::to_wstring(消息.消息编号) + L" 接收=" + std::to_wstring(static_cast<int>(接收.状态))); } catch (...) {}
#endif
                    if (接收.状态 == 新任务承接接收状态::邮箱已满
                        || 接收.状态 == 新任务承接接收状态::资源失败) {
                        尚有交接 = true;
                        continue;
                    }
                    if ((接收.状态 != 新任务承接接收状态::已接收
                            && 接收.状态 != 新任务承接接收状态::精确重复)
                        || !接收.处理结果.valid()) return 结果;
                    异步 = 接收.处理结果;
                }
                if (异步.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                    尚有交接 = true;
                    continue;
                }
                const auto 承接 = 异步.get();
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                try { (void)调试日志<调试切片::PARAM_BINDING>(L"消息收发", L"自我收条件子需求承接",
                    L"任务=" + std::to_wstring(定位.任务节点.值) + L" 子需求=" + std::to_wstring(消息.需求节点.值)
                    + L" 处理=" + std::to_wstring(static_cast<int>(承接.状态))
                    + L" 承接=" + std::to_wstring(承接.承接 ? static_cast<int>(承接.承接->状态) : -1)
                    + L" 有任务=" + std::to_wstring(承接.承接 && 承接.承接->任务 ? 1 : 0)); } catch (...) {}
#endif
                if (承接.原消息 != 消息 || !承接.承接 || !承接.承接->任务
                    || (承接.状态 != 新任务承接处理状态::已承接
                        && 承接.状态 != 新任务承接处理状态::已重新启用
                        && 承接.状态 != 新任务承接处理状态::需求当前已满足
                        && 承接.状态 != 新任务承接处理状态::既有终结)) return 结果;
                const auto 读回 = 任务服务->读回需求任务(
                    消息.需求节点, 承接.承接->任务->节点);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                try { (void)调试日志<调试切片::PARAM_BINDING>(L"消息收发", L"自我读回条件子需求任务",
                    L"任务=" + std::to_wstring(定位.任务节点.值) + L" 子需求=" + std::to_wstring(消息.需求节点.值)
                    + L" 读回=" + std::to_wstring(static_cast<int>(读回.状态))); } catch (...) {}
#endif
                if (读回.状态 == 新任务治理承接状态::资源失败) return 等待();
                if (读回.状态 != 新任务治理承接状态::已读回
                    && 读回.状态 != 新任务治理承接状态::既有终结任务) return 结果;
                项.条件子需求承接完成组[i] = true;
            }
            if (尚有交接) return 等待();
            结果.状态 = 新自我消息处理状态::任务筹办已裁决;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = 新自我消息处理状态::已交接未收回;
            return 结果;
        } catch (...) {
            // ready future的永久交接错误不能伪装成可重试资源失败。
            // 已发布前缀保存在返回回执中，不撤销需求，也不解除父阻塞。
            try {
                if (项.条件子需求回执 && 项.消息.筹办反馈) {
                    新自我筹办裁决 裁决;
                    裁决.定位 = *项.消息.筹办反馈;
                    裁决.状态 = 新自我筹办裁决状态::已处理条件子需求;
                    裁决.子需求回执 = *项.条件子需求回执;
                    结果.筹办裁决 = std::move(裁决);
                }
            } catch (...) {}
            结果.状态 = 新自我消息处理状态::内部错误;
            return 结果;
        }
    }

    新自我筹办裁决 分类筹办结果(
        const 新任务筹办反馈定位& 定位,
        const 新任务筹办处理结果& 筹办) noexcept {
        新自我筹办裁决 裁决;
        裁决.定位 = 定位;
        switch (筹办.状态) {
        case 新任务筹办处理状态::完整无候选并进入等待:
            裁决.状态 = 新自我筹办裁决状态::当前无可用方法;
            return 裁决;
        case 新任务筹办处理状态::任务状态已变化:
            裁决.状态 = 新自我筹办裁决状态::任务状态变化;
            return 裁决;
        case 新任务筹办处理状态::条件筹办已完成:
            break;
        case 新任务筹办处理状态::组织能力暂不支持:
            裁决.状态 = 新自我筹办裁决状态::结构缺口;
            return 裁决;
        case 新任务筹办处理状态::有候选并进入条件检查:
        case 新任务筹办处理状态::工作未完成:
        case 新任务筹办处理状态::独立读回失败:
        case 新任务筹办处理状态::状态推进失败:
        case 新任务筹办处理状态::停止取消:
        case 新任务筹办处理状态::内部错误:
        default:
            return 裁决;
        }
        if (!筹办.条件筹办 || !筹办.独立条件筹办
            || 筹办.条件筹办->任务节点 != 定位.任务节点
            || 筹办.独立条件筹办->任务节点 != 定位.任务节点
            || 筹办.独立条件筹办->状态
                != 新任务治理条件筹办状态::已完成
            || 筹办.独立条件筹办->方法筹办组.empty()) {
            return 裁决;
        }
        if (!筹办.独立条件筹办->候选已完整核验) {
            // 未核验完不能证明能力缺失；已有登记头待学习也走材料等待。
            // 缺口组为空不等于所有候选已经被证明不适用。
            裁决.状态 = 新自我筹办裁决状态::等待材料;
            return 裁决;
        }
        const auto 选择 = 任务服务->选择任务方法结果(定位.任务节点);
        if (选择.状态 == 新任务方法选择状态::结构依赖未就绪) {
            // 运行中工程异常终止；仅停机期间可达此后，调用方随即按停止取消返回。
            (void)结构依赖未就绪已停止(L"分类筹办结果/选择任务方法结果",static_cast<int>(选择.状态),定位.任务节点.值);
            裁决.状态 = 新自我筹办裁决状态::等待材料;
            return 裁决;
        }
        if (选择.状态 == 新任务方法选择状态::已选定 && 选择.本能采用) {
            裁决.状态 = 新自我筹办裁决状态::可进入执行前判断;
            裁决.唯一具备方法节点 = 选择.方法节点;
            裁决.唯一具备配对节点 = 选择.配对节点;
            裁决.唯一对应结果节点 = 选择.结果节点;
            裁决.本能采用 = 选择.本能采用;
            return 裁决;
        }
        if (选择.状态 == 新任务方法选择状态::等待条件) {
            裁决.状态 = 新自我筹办裁决状态::等待材料;
        } else if (选择.状态 == 新任务方法选择状态::无适用结果) {
            裁决.状态 = 新自我筹办裁决状态::当前无可用方法;
        } else if (选择.状态 == 新任务方法选择状态::任务状态已变化
            || 选择.状态 == 新任务方法选择状态::目标已满足) {
            裁决.状态 = 新自我筹办裁决状态::任务状态变化;
        } else if (选择.状态 == 新任务方法选择状态::结构不一致
            || 选择.状态 == 新任务方法选择状态::实例未建立
            || 选择.状态 == 新任务方法选择状态::结果定义暂不支持) {
            裁决.状态 = 新自我筹办裁决状态::结构缺口;
        } else {
            裁决.状态 = 新自我筹办裁决状态::需核查;
        }
        return 裁决;
    }

    bool 登记当前缺失方法(const 新任务筹办反馈定位& 定位,
        const std::optional<新任务候选方法材料>& 分类候选) noexcept {
        try {
            const auto 原筹办 = 任务管理->读取最近筹办结果(定位.任务节点);
            if (!原筹办 || 原筹办->原工作 != 定位.原工作) return false;
            const auto t = 任务服务->获取任务(定位.任务节点);
            if (!t) throw std::logic_error("missing-method source task unreadable");
            const auto d = 需求服务->获取需求(t->需求节点);
            const auto c = 任务服务->获取当前候选方法材料(t->节点);
            if (!d || 任务服务->查询需求任务(d->节点) != t->节点)
                throw std::logic_error("missing-method source T-D mismatch");
            if (!c || c != 分类候选 || !c->查询完整 || c->需求节点 != d->节点 || c->需求目标 != d->目标
                || (t->当前状态 != 新任务状态::等待条件 && t->当前状态 != 新任务状态::待检查条件))
                return false; // 学习/重新筹办已经替换来源，不登记陈旧结论。
            // 引用内容目标（本叶、补问叶子）没有比较关系等普通方法头材料，缺失方法头无法按
            // “存在概念＋特征类型＋值域”登记；其取得方法由 ASK-READ-SCENE 另行定义。这里不登记
            // 缺失方法，仍按“当前无可用方法”等待，由方法知识变化唤醒。
            if (d->目标.引用内容定位) return true;
            if (原筹办->状态 == 新任务筹办处理状态::完整无候选并进入等待) {
                if (!原筹办->独立消费 || 原筹办->独立消费->读回.材料 != c
                    || !c->候选方法节点组.empty()) return false;
            } else if (原筹办->状态 != 新任务筹办处理状态::条件筹办已完成
                || !原筹办->独立条件筹办 || !原筹办->独立条件筹办->候选已完整核验
                || 原筹办->独立条件筹办->候选材料节点 != c->节点) return false;
            const auto f = 世界树服务->获取事实服务引用();
            std::optional<稳定编码> ec;
            if (const auto e = f.存在服务.获取存在(d->目标存在节点)) ec = e->专属存在概念节点;
            else {
                // Vt不属于现实世界存在，必须经任务owner取得正式语义。
                const auto 全部 = 任务服务->查询全部任务();
                if (全部.状态 != 新任务操作状态::已找到)
                    throw std::logic_error("missing-method virtual target lookup failed");
                for (auto n : 全部.任务节点组) {
                    const auto v = 任务服务->获取任务虚拟存在(n);
                    if (!v) throw std::logic_error("missing-method virtual target unreadable");
                    if (v->节点 != d->目标存在节点) continue;
                    const auto s = 任务服务->读取任务虚拟存在语义({n, v->节点, v->任务状态特征节点});
                    if (!s || ec) throw std::logic_error("missing-method virtual target ambiguous");
                    ec = s->专属存在概念节点;
                }
            }
            if (!ec) throw std::logic_error("missing-method demand target lacks formal existence concept");
            新缺失方法登记请求 请求;
            请求.来源需求节点 = d->节点;
            请求.来源任务节点 = t->节点;
            请求.方法头.作用存在概念组 = {*ec};
            请求.方法头.涉及特征类型概念组 = {d->目标.目标特征类型概念节点};
            if (d->目标.目标形式 == 新需求目标形式::值) {
                if (!d->目标.目标值域概念节点 || !d->目标.目标比较关系概念节点)
                    throw std::logic_error("missing-method value target incomplete");
                请求.目标值域概念节点 = d->目标.目标值域概念节点;
                请求.目标比较关系概念节点 = d->目标.目标比较关系概念节点;
                请求.方法头.可能结果概念组 = {*请求.目标值域概念节点, *请求.目标比较关系概念节点};
            } else {
                if (d->目标.目标形式 != 新需求目标形式::变化方向 || !d->目标.目标变化方向)
                    throw std::logic_error("missing-method direction target incomplete");
                const auto ft = d->目标.目标特征类型概念节点;
                const auto 根 = f.二次特征概念服务.建立或取得默认二次特征概念(二次特征算法::有向特征差值,
                    {{0, 二次特征输入角色::左输入, 二次特征输入来源种类::特征准确值, ft},
                     {1, 二次特征输入角色::右输入, 二次特征输入来源种类::特征准确值, ft}});
                if (!根.概念节点) throw std::logic_error("missing-method direction definition unavailable");
                const auto 方向 = f.二次特征概念服务.建立或取得参照结果域概念(*根.概念节点,
                    *d->目标.目标变化方向 == 新需求变化方向::增加 ? 二次特征结果域种类::大于 : 二次特征结果域种类::小于);
                if (!方向.概念节点) throw std::logic_error("missing-method direction domain unavailable");
                请求.目标变化方向概念节点 = 方向.概念节点;
                请求.方法头.可能结果概念组 = {*方向.概念节点};
            }
            const auto 当前需求 = 需求服务->获取需求(d->节点);
            if (任务服务->获取任务(t->节点) != t || !当前需求
                || 当前需求->目标存在节点 != d->目标存在节点 || 当前需求->目标 != d->目标
                || 当前需求->需求主体存在节点 != d->需求主体存在节点
                || 当前需求->执行域 != d->执行域 || 当前需求->活动状态 != d->活动状态
                || 任务服务->获取当前候选方法材料(t->节点) != c) return false;
            const auto 当前筹办 = 任务管理->读取最近筹办结果(定位.任务节点);
            if (!当前筹办 || 当前筹办->原工作 != 定位.原工作) return false;
            const auto r = 方法服务->登记缺失方法(请求);
            // 来源需求/任务节点被并发删除：来源已变化，交调用方按任务状态变化处理。
            if (r.状态 == 新方法操作状态::定义前提已变化) return false;
            if ((r.状态 != 新方法操作状态::已建立 && r.状态 != 新方法操作状态::已找到) || !r.信息)
                throw std::logic_error("confirmed missing-method registration rejected, 新方法操作状态="
                    + std::to_string(static_cast<unsigned>(r.状态))
                    + (r.状态 == 新方法操作状态::方法头不合法 ? " (method header invalid)" : ""));
            return true;
        } catch (const std::exception& e) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"自我登记缺失方法", L"已确认缺口的来源或登记结构不完整，不能当作能力不足继续等待。", e.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"自我登记缺失方法", L"已确认缺口的来源或登记结构不完整，不能当作能力不足继续等待。");
        }
    }

    // 无适用错误分支：只移交本消息已有的进度，不改变终结原因或重读事实。
    void 移交因果采用进度(消息项& 项, 新自我消息处理结果& 结果) noexcept {
        static_assert(std::is_nothrow_move_constructible_v<新自我筹办裁决::因果采用回执>
            && std::is_nothrow_move_assignable_v<新自我筹办裁决::因果采用回执>);
        if (项.因果子需求回执) {
            结果.停止时因果采用进度 = std::move(项.因果子需求回执);
            项.因果子需求回执.reset();
        }
    }

    // 无适用错误分支：只供确已观察到停止的因果分支调用。
    void 标记因果停止并移交进度(消息项& 项, 新自我消息处理结果& 结果) noexcept {
        结果.状态 = 新自我消息处理状态::停止取消;
        移交因果采用进度(项, 结果);
    }

    // 因果子需求不持有M/C/R或实际F；目标由正式因果提议及原D限定。
    新自我消息处理结果 处理因果子需求提议(
        消息项& 项, const 新任务筹办处理结果& 筹办) noexcept {
        新自我消息处理结果 结果;
        结果.原消息 = 项.消息;
        结果.状态 = 新自我消息处理状态::任务筹办需核查;
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        主线因果阶段观察 观察{项.消息, 结果};
#endif
        HY_SELF_CAUSAL_PHASE(L"处理入口", nullptr, std::nullopt);
        const auto 停止 = 停止源.get_token();
        // §8.1/8.2：收件通知置位后，长筹办须有界让回 while 入口，否则 500ms 补扫无法兑现。
        try {
            const auto 检查停止 = [&] {
                if (停止.stop_requested()) throw 新任务因果读取取消{};
                bool 让出 = false;
                {
                    std::lock_guard 锁(状态互斥);
                    让出 = 接收认知 && 接收认知待唤醒;
                }
                if (让出) throw 接收认知让出最终{};
            };
            const auto 完整提议 = [](新任务因果提议可停止结果 读取) {
                if (std::holds_alternative<新任务因果读取取消>(读取)) throw 新任务因果读取取消{};
                if (std::get<新任务因果子需求提议>(读取).状态 == 新任务因果子需求提议状态::资源延后)
                    throw std::bad_alloc{};
                return std::get<新任务因果子需求提议>(std::move(读取));
            };
            const auto 完整接收 = [](新自我因果筹办提交结果 读取) {
                if (std::holds_alternative<新任务因果读取取消>(读取)) throw 新任务因果读取取消{};
                return std::get<新自我筹办裁决接收状态>(读取);
            };
            const auto 读取当前提议 = [&]() -> 新任务因果提议可停止结果 {
                if (停止.stop_requested()) return 新任务因果读取取消{};
                auto 边 = 任务管理->读取因果等待边(停止);
                if (std::holds_alternative<新任务因果读取取消>(边)) return 新任务因果读取取消{};
                检查停止();
                // R111：与任务管理线程使用同一份筹办排除集重读。
                const auto 排除 = 任务管理->读取筹办排除集(项.消息.筹办反馈->任务节点);
                if (!排除) throw std::bad_alloc{};
                auto 当前 = 任务服务->提议任务因果子需求(项.消息.筹办反馈->任务节点,
                    std::get<std::vector<新因果任务等待边>>(边), 停止, *排除);
                检查停止();
                return 当前;
            };
            检查停止();
            if (!任务管理 || !项.消息.筹办反馈) return 结果;
            const auto& 定位 = *项.消息.筹办反馈;
            const auto 等待能力 = [&](const 新任务因果子需求提议& 当前) {
                结果.筹办独立读回 = 筹办;
                结果.筹办独立读回->因果子需求提议 = 当前;
                新自我筹办裁决 裁决;
                裁决.定位 = 定位;
                裁决.状态 = 新自我筹办裁决状态::等待材料;
                结果.筹办裁决 = 裁决;
                检查停止();
                const auto 接收 = 完整接收(任务管理->提交自我因果筹办裁决(裁决, 停止));
                结果.筹办裁决接收状态 = 接收;
                if (接收 == 新自我筹办裁决接收状态::已接收
                    || 接收 == 新自我筹办裁决接收状态::精确重复)
                    结果.状态 = 新自我消息处理状态::任务筹办已裁决;
                else if (接收 == 新自我筹办裁决接收状态::定位已变化)
                    结果.状态 = 新自我消息处理状态::任务筹办定位已变化;
                else if (接收 == 新自我筹办裁决接收状态::资源失败)
                    结果.状态 = 新自我消息处理状态::已交接未收回;
            };
            if (!项.因果子需求回执 && 筹办.因果子需求提议
                && 筹办.因果子需求提议->状态 == 新任务因果子需求提议状态::能力未支持) {
                等待能力(*筹办.因果子需求提议);
                return 结果;
            }
            if (!项.因果子需求回执 && 筹办.因果子需求提议
                && 筹办.因果子需求提议->状态 == 新任务因果子需求提议状态::路径待选择) {
                const auto& 待选 = *筹办.因果子需求提议;
                if (!待选.路径选择未确定 || 待选.候选依据组.empty() || 待选.依据
                    || !待选.子需求项组.empty() || !待选.继续推进任务组.empty()
                    || !待选.来源前驱前端组.empty())
                    throw std::logic_error("待选因果路径伪装已采用依据或子需求");
                HY_SELF_CAUSAL_PHASE(L"待选路径逆推前", nullptr, std::nullopt);
                const auto 当前 = 完整提议(读取当前提议());
                检查停止();
                HY_SELF_CAUSAL_PHASE(L"待选路径逆推后", 主线因果状态文字(当前.状态), std::nullopt);
                if (当前.状态 == 新任务因果子需求提议状态::能力未支持) {
                    等待能力(当前);
                    return 结果;
                }
                if (当前.状态 == 新任务因果子需求提议状态::材料已变化) {
                    结果.状态 = 新自我消息处理状态::已交接未收回;
                    return 结果;
                }
                新自我筹办裁决 裁决;
                裁决.定位 = 定位;
                const auto& 原目标 = 待选.候选依据组.front().请求;
                const bool 同一目标 = !当前.候选依据组.empty()
                    && std::all_of(当前.候选依据组.begin(), 当前.候选依据组.end(), [&](const auto& 依据) {
                        const auto& 请求 = 依据.请求;
                        return 请求.父任务节点 == 定位.任务节点
                            && 请求.父任务节点 == 原目标.父任务节点 && 请求.父需求节点 == 原目标.父需求节点
                            && 请求.目标存在节点 == 原目标.目标存在节点 && 请求.目标 == 原目标.目标;
                    });
                裁决.状态 = 当前.状态 == 新任务因果子需求提议状态::路径待选择
                    && 同一目标
                    ? 新自我筹办裁决状态::等待材料
                    : 新自我筹办裁决状态::任务状态变化;
                // 候选仍保存在manager原筹办结果中；本回执不是路径采用。
                结果.筹办裁决 = 裁决;
                HY_SELF_CAUSAL_PHASE(L"待选路径裁决前", nullptr, std::nullopt);
                检查停止();
                const auto 接收 = 完整接收(任务管理->提交自我因果筹办裁决(裁决, 停止));
                HY_SELF_CAUSAL_PHASE(L"待选路径裁决后", 主线因果状态文字(接收), std::nullopt);
                结果.筹办裁决接收状态 = 接收;
                检查停止();
                if (接收 == 新自我筹办裁决接收状态::资源失败)
                    结果.状态 = 新自我消息处理状态::已交接未收回;
                else if (接收 == 新自我筹办裁决接收状态::已接收
                    || 接收 == 新自我筹办裁决接收状态::精确重复)
                    结果.状态 = 新自我消息处理状态::任务筹办已裁决;
                else if (接收 == 新自我筹办裁决接收状态::定位已变化)
                    结果.状态 = 新自我消息处理状态::任务筹办定位已变化;
                return 结果;
            }
            if (!项.因果子需求回执) {
                if (!筹办.因果子需求提议 || !筹办.因果子需求提议->依据
                    || (筹办.因果子需求提议->状态 != 新任务因果子需求提议状态::已形成
                        && 筹办.因果子需求提议->状态 != 新任务因果子需求提议状态::无需子需求)) return 结果;
                新自我筹办裁决::因果采用回执 回执;
                回执.原提议 = *筹办.因果子需求提议;
                回执.项组.reserve(回执.原提议.子需求项组.size()+回执.原提议.来源前驱前端组.size());
                for (const auto& 子项 : 回执.原提议.子需求项组) {
                    检查停止();
                    回执.项组.push_back({子项});
                }
                for(const auto& 模板:回执.原提议.来源前驱前端组) {
                    检查停止();
                    新子需求采用项 采用;
                    采用.来源前驱模板=模板;
                    回执.项组.push_back(std::move(采用));
                }
                项.因果子需求回执 = std::move(回执);
            }
            auto& 回执 = *项.因果子需求回执;
            const auto& 原 = 回执.原提议;
            if (!原.依据) throw std::logic_error("因果采用缺少正式依据");
            const auto& 请求 = 原.依据->请求;
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
            观察.父需求 = 请求.父需求节点;
#endif
            if (请求.父任务节点 != 定位.任务节点) throw std::logic_error("因果父任务定位不一致");
            检查停止();
            const auto 事实 = 世界树服务->获取事实服务引用();
            检查停止();
            auto& 概念服务 = 事实.二次特征概念服务;
            const auto 输入定义 = [](稳定编码 类型) {
                return 子需求比较输入(类型);
            };
            const auto 投影一致 = [&](const 新任务条件子需求项& 目标) {
                return 子需求目标投影一致(目标);
            };
            const auto 算法 = [&](const 新任务条件子需求项& 目标) {
                return 子需求比较算法(目标);
            };
            const auto 正式一致 = [&](const 新子需求采用项& 采用) {
                return 正式条件子需求一致(采用, 请求.父需求节点, 请求.父任务节点);
            };
            const auto fresh = [&]() -> 新任务因果提议可停止结果 {
                // 不持self锁进入manager/task；等待边只取manager正式投影。
                HY_SELF_CAUSAL_PHASE(L"采用fresh逆推前", nullptr, std::nullopt);
                {
                    std::lock_guard 锁(状态互斥);
                    项.因果能力复核登记 = true;
                    项.因果能力期间事件 = false;
                    项.因果能力已停放 = false;
                }
                新任务因果提议可停止结果 当前;
                try {
                    当前 = 读取当前提议();
                    const auto* 正常 = std::get_if<新任务因果子需求提议>(&当前);
                    auto 缺口 = 正常 && 正常->状态 == 新任务因果子需求提议状态::能力未支持
                        ? 正常->能力缺口组 : std::vector<新因果能力缺口>{};
                    std::lock_guard 锁(状态互斥);
                    if (正常 && 正常->状态 == 新任务因果子需求提议状态::能力未支持) {
                        项.因果能力缺口组 = std::move(缺口);
                        项.因果能力已停放 = !项.因果能力期间事件;
                        if (!项.因果能力已停放) 项.可处理时点 = std::chrono::steady_clock::now();
                    } else {
                        项.因果能力复核登记 = false;
                        项.因果能力期间事件 = false;
                        项.因果能力已停放 = false;
                        项.因果能力缺口组.clear();
                    }
                } catch (...) {
                    std::lock_guard 锁(状态互斥);
                    项.因果能力复核登记 = false;
                    项.因果能力已停放 = false;
                    throw;
                }
                if (std::holds_alternative<新任务因果读取取消>(当前)) return 新任务因果读取取消{};
                [[maybe_unused]] const auto& 正常 = std::get<新任务因果子需求提议>(当前);
                HY_SELF_CAUSAL_PHASE(L"采用fresh逆推后", 主线因果状态文字(正常.状态), std::nullopt);
#ifdef HY_EGO_DEBUG_LOG_CAUSAL_PROPOSAL_DIFF
                记录因果提议差异(定位.任务节点, 原, 正常);
#endif
                return 当前;
            };
            enum class 本次因果采用判断 { 仍需采用, 已无需要, 重新读取, 等待能力 };
            const auto 当前仍需采用 = [&](const 新子需求采用项& 采用) -> 本次因果采用判断 {
                const auto 当前 = 完整提议(fresh());
                if (当前.状态 == 新任务因果子需求提议状态::能力未支持) {
                    const auto 解除能力停放 = [&] {
                        std::lock_guard 锁(状态互斥);
                        项.因果能力复核登记 = false;
                        项.因果能力期间事件 = false;
                        项.因果能力已停放 = false;
                        项.因果能力缺口组.clear();
                    };
                    try {
                        // 只有实际D/目标变化才收束旧来源，能力缺口本身不能证明已无需要。
                        const auto 任务读取 = 任务服务->获取任务(请求.父任务节点, 停止);
                        if (std::holds_alternative<新任务因果读取取消>(任务读取)) throw 新任务因果读取取消{};
                        const auto& T = std::get<std::optional<新任务信息>>(任务读取);
                        std::optional<稳定编码> 当前需求节点;
                        if (T) {
                            const auto 当前满足 = 任务服务->读取当前满足需求(T->节点);
                            switch (当前满足.状态) {
                            case 新当前满足需求状态::已选定: 当前需求节点 = 当前满足.需求节点; break;
                            case 新当前满足需求状态::无可服务需求: 当前需求节点 = T->需求节点; break;
                            case 新当前满足需求状态::资源失败: throw std::bad_alloc{};
                            case 新当前满足需求状态::前提已变化: break;
                            case 新当前满足需求状态::结构损坏:
                                throw std::logic_error("因果采用当前满足需求结构损坏");
                            }
                        }
                        const auto D = 当前需求节点 ? 需求服务->获取需求(*当前需求节点) : std::nullopt;
                        if (!D) {
                            // 空读取不能证明来源已经消失，保留已发布前缀并解除能力停放后重读。
                            解除能力停放();
                            return 本次因果采用判断::重新读取;
                        }
                        if (D->节点 != 请求.父需求节点
                            || D->目标存在节点 != 请求.目标存在节点 || D->目标 != 请求.目标) {
                            解除能力停放();
                            return 本次因果采用判断::已无需要;
                        }
                        return 本次因果采用判断::等待能力;
                    } catch (...) {
                        解除能力停放();
                        throw;
                    }
                }
                if (当前.状态 == 新任务因果子需求提议状态::材料已变化) return 本次因果采用判断::重新读取;
                if (!当前.依据 || (当前.状态 != 新任务因果子需求提议状态::已形成
                    && 当前.状态 != 新任务因果子需求提议状态::无需子需求)) return 本次因果采用判断::已无需要;
                const auto& 当前请求 = 当前.依据->请求;
                if (当前请求.父任务节点 != 请求.父任务节点 || 当前请求.父需求节点 != 请求.父需求节点
                    || 当前请求.目标存在节点 != 请求.目标存在节点 || 当前请求.目标 != 请求.目标)
                    return 本次因果采用判断::已无需要;
                // 只消费当前仍需的这一项目标；其它候选和因果图变化不撤销它。
                if (采用.来源前驱模板) {
                    const auto& 目标 = *采用.来源前驱模板;
                    const bool 仍需 = std::any_of(当前.来源前驱前端组.begin(), 当前.来源前驱前端组.end(),
                        [&](const auto& 项) {
                            return 项.种类 == 目标.种类 && 项.目标槽类型 == 目标.目标槽类型
                                && 项.要求概念 == 目标.要求概念 && 项.H == 目标.H && 项.D == 目标.D
                                && 项.正式根路径 == 目标.正式根路径;
                        });
                    return 仍需 ? 本次因果采用判断::仍需采用 : 本次因果采用判断::已无需要;
                }
                return std::find(当前.子需求项组.begin(), 当前.子需求项组.end(), 采用.原提议)
                    != 当前.子需求项组.end() ? 本次因果采用判断::仍需采用 : 本次因果采用判断::已无需要;
            };
            const auto 停止剩余 = [&](新子需求采用状态 状态,std::source_location 位置=std::source_location::current()) {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
                try{(void)调试日志<调试切片::SIMULATION_PROGRESS>(L"自我因果采用",L"停止剩余",
                    L"状态="+std::to_wstring(static_cast<int>(状态))+L"；行="+std::to_wstring(位置.line()));}catch(...){}
#else
                (void)位置;
#endif
                for (auto& 采用 : 回执.项组)
                    if (采用.状态 == 新子需求采用状态::未处理 || 采用.状态 == 新子需求采用状态::资源失败)
                        采用.状态 = 状态;
            };
            if (!回执.已完成处理) {
                for (auto& 采用 : 回执.项组) {
                    检查停止();
                    if(采用.来源前驱模板) {
                        const auto& 模板=*采用.来源前驱模板;
                        if(采用.原提议!=新任务条件子需求项{}||原.来源前驱前端组.size()!=1
                            ||原.来源前驱前端组.front()!=模板||!请求.来源前驱上下文
                            ||!原.依据->读回.来源前驱读回)
                            throw std::logic_error("来源前端采用缺少唯一正式模板");
                        const auto& 配置=请求.来源前驱上下文->配置;
                        const auto 核正式身份=[&]() {
                            const auto 读=任务服务->核验因果子需求身份(*原.依据,*采用.需求节点,停止);
                            if(std::holds_alternative<新任务因果读取取消>(读))throw 新任务因果读取取消{};
                            return std::get<bool>(读);
                        };
                        if(采用.需求节点) {
                            if(!采用.来源前驱具体目标)
                                throw std::logic_error("来源子需求前缀缺少具体目标");
                            // 保留已发布G/R；真实前提变化交manager重新筹办，不回滚或再生D。
                            if(!正式一致(采用)||!核正式身份()) {
                                采用.状态=新子需求采用状态::来源过期;
                                停止剩余(新子需求采用状态::来源过期);break;
                            }
                            if(采用.状态==新子需求采用状态::资源失败)
                                采用.状态=新子需求采用状态::已复用;
                            continue;
                        }
                        if(采用.状态!=新子需求采用状态::未处理&&采用.状态!=新子需求采用状态::资源失败)continue;
                        新需求完整目标 具体目标;
                        if(模板.种类==新来源前驱前端种类::取得服务对象) {
                            if(采用.来源前驱实例||模板.H||模板.D||!模板.正式根路径.empty()
                                ||模板.目标槽类型!=配置.交互者类型||模板.要求概念!=配置.交互者人类范围)
                                throw std::logic_error("服务对象前提模板或实例不合法");
                            const auto 仍需采用=当前仍需采用(采用);
                            if(仍需采用==本次因果采用判断::重新读取 || 仍需采用==本次因果采用判断::等待能力) {
                                结果.状态=新自我消息处理状态::已交接未收回;return 结果;
                            }
                            if(仍需采用==本次因果采用判断::已无需要){停止剩余(新子需求采用状态::来源过期);break;}
                            const auto 域=事实.特征概念服务.获取特征概念(模板.要求概念);
                            const auto* 引用域=域 ? std::get_if<特征概念存在引用值域>(&域->值域) : nullptr;
                            if(!引用域||引用域->存在概念节点!=配置.人类概念
                                ||域->材料类型!=特征概念材料物理类型::存在节点引用)
                                throw std::logic_error("服务对象前提缺少正式人类引用域");
                            检查停止();
                            const auto 类型=概念服务.建立或取得默认二次特征概念(
                                二次特征算法::特征材料相似度,子需求比较输入(模板.目标槽类型),模板.要求概念);
                            if(类型.状态==二次特征概念操作状态::资源失败)throw std::bad_alloc{};
                            if(!类型.概念节点||(类型.状态!=二次特征概念操作状态::已建立
                                &&类型.状态!=二次特征概念操作状态::已复用))
                                throw std::logic_error("服务对象前提比较类型建立失败");
                            检查停止();
                            const auto 关系=概念服务.建立或取得参照结果域概念(
                                *类型.概念节点,二次特征结果域种类::包含于参照值域);
                            if(关系.状态==二次特征概念操作状态::资源失败)throw std::bad_alloc{};
                            if(!关系.概念节点||(关系.状态!=二次特征概念操作状态::已建立
                                &&关系.状态!=二次特征概念操作状态::已复用))
                                throw std::logic_error("服务对象前提比较关系建立失败");
                            检查停止();
                            具体目标={请求.目标存在节点,{模板.目标槽类型,新需求目标形式::值,
                                模板.要求概念,关系.概念节点,{}}};
                        } else {
                            if((模板.种类!=新来源前驱前端种类::既有需求内容
                                    &&模板.种类!=新来源前驱前端种类::待实例化需求内容)
                                ||!模板.H||模板.目标槽类型!=配置.对象需求类型
                                ||模板.要求概念!=配置.完整被认知需求概念||!模板.正式根路径.empty())
                                throw std::logic_error("需求内容前端模板不合法");
                            auto 实例读取=任务服务->建立或取得来源前驱目标实例(*原.依据,模板,停止);
                            if(std::holds_alternative<新任务因果读取取消>(实例读取))throw 新任务因果读取取消{};
                            const auto& 实例结果=std::get<新服务取得目标实例结果>(实例读取);
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
                            try{(void)调试日志<调试切片::SIMULATION_PROGRESS>(L"自我因果采用",L"来源前驱实例",
                                L"状态="+std::to_wstring(static_cast<int>(实例结果.状态))+L"；有信息="+std::to_wstring(实例结果.信息 ? 1 : 0)
                                +L"；父任务="+std::to_wstring(原.依据->请求.父任务节点.值));}catch(...){}
#endif
                            if(实例结果.信息) {
                                auto 本次前缀=*实例结果.信息;
                                if(采用.来源前驱实例) {
                                    auto 固定身份=本次前缀;
                                    固定身份.骨架就绪=采用.来源前驱实例->骨架就绪;
                                    if(固定身份!=*采用.来源前驱实例)
                                        throw std::logic_error("来源前端续接改变固定实例身份");
                                    // 非成功reader的未确认位不能抹掉此前已确认进度；
                                    // 是否可继续仍只取本次状态及本次成功材料，不能借旧true放行。
                                    if(采用.来源前驱实例->骨架就绪
                                        &&实例结果.状态!=新服务取得目标实例状态::已建立
                                        &&实例结果.状态!=新服务取得目标实例状态::已复用)
                                        本次前缀.骨架就绪=true;
                                }
                                采用.来源前驱实例=std::move(本次前缀);
                            }
                            // 先保存真实出生前缀，再响应停止；不把骨架/绑定半程当成功。
                            检查停止();
                            switch(实例结果.状态) {
                            case 新服务取得目标实例状态::已建立:case 新服务取得目标实例状态::已复用:break;
                            case 新服务取得目标实例状态::资源失败:case 新服务取得目标实例状态::依赖未就绪:
                            case 新服务取得目标实例状态::已出生待补骨架:
                                采用.状态=新子需求采用状态::资源失败;
                                结果.状态=新自我消息处理状态::已交接未收回;return 结果;
                            case 新服务取得目标实例状态::前提已变化:case 新服务取得目标实例状态::绑定冲突:
                            case 新服务取得目标实例状态::输入不适用:
                                停止剩余(新子需求采用状态::来源过期);break;
                            default:throw std::logic_error("来源目标实例入口返回非法状态");
                            }
                            if(采用.状态==新子需求采用状态::来源过期)break;
                            if(!实例结果.信息||!实例结果.信息->骨架就绪||实例结果.完整保护.empty())
                                throw std::logic_error("来源目标实例成功缺少绑定或骨架材料");
                            const auto& 实例=*实例结果.信息;
                            if(实例.键!=新服务取得目标实例键{请求.父任务节点,模板.目标槽类型,*模板.H,模板.正式根路径}
                                ||实例.父需求!=请求.父需求节点||实例.原父目标!=新需求完整目标{请求.目标存在节点,请求.目标}
                                ||实例.人类概念!=配置.人类概念||实例.需求身份概念!=配置.需求概念
                                ||实例.完整内容概念!=模板.要求概念
                                ||实例.复用指定已有需求!=(模板.种类==新来源前驱前端种类::既有需求内容)
                                ||(实例.复用指定已有需求 ? 模板.D!=实例.需求节点 : 模板.D.has_value()))
                                throw std::logic_error("来源目标实例完整含义不符");
                            具体目标.目标存在节点=*模板.H;
                            具体目标.目标.目标特征类型概念节点=模板.目标槽类型;
                            具体目标.目标.目标值域概念节点=模板.要求概念;
                            具体目标.目标.引用内容定位=新需求引用内容定位{实例.需求节点,模板.正式根路径};
                        }
                        采用.来源前驱具体目标=具体目标;
                        新普通需求建立请求 建立请求;
                        建立请求.父需求节点=请求.父需求节点;
                        建立请求.目标存在节点=具体目标.目标存在节点;
                        建立请求.目标=具体目标.目标;
                        建立请求.来源类型=新需求来源类型::任务子目标;
                        建立请求.来源承接节点=请求.父任务节点;
                        检查停止();
                        const auto 建立=需求服务->建立或取得普通需求已核验(建立请求,true);
                        if(建立.需求节点)采用.需求节点=建立.需求节点;
                        if(建立.状态==新需求操作状态::资源失败||建立.状态==新需求操作状态::目标内容依赖未就绪
                            ||建立.状态==新需求操作状态::目标内容未支持) {
                            采用.状态=新子需求采用状态::资源失败;
                            结果.状态=新自我消息处理状态::已交接未收回;return 结果;
                        }
                        if(建立.状态==新需求操作状态::前提已变化) {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
                            try{(void)调试日志<调试切片::SIMULATION_PROGRESS>(L"自我因果采用",L"内容子需求前提已变化",
                                L"需求owner行="+std::to_wstring(引用内容建立失败行));}catch(...){}
#endif
                            停止剩余(新子需求采用状态::来源过期);break;
                        }
                        if(!采用.需求节点||(建立.状态!=新需求操作状态::已建立&&建立.状态!=新需求操作状态::已复用))
                            throw std::logic_error("来源内容子需求发布失败");
                        采用.状态=建立.状态==新需求操作状态::已建立 ? 新子需求采用状态::已建立 : 新子需求采用状态::已复用;
                        检查停止();
                        if(!正式一致(采用)||!核正式身份()) {
                            采用.状态=新子需求采用状态::来源过期;
                            停止剩余(新子需求采用状态::来源过期);break;
                        }
                        {std::lock_guard 锁(状态互斥);请求关联复核已加锁(建立.状态==新需求操作状态::已建立);}
                        检查停止();
                        continue;
                    }
                    const bool 续接已成立前缀=采用.需求节点&&采用.状态==新子需求采用状态::资源失败
                        &&采用.原提议.新增成员见证要求.has_value();
                    if (采用.需求节点&&!续接已成立前缀) {
                        if (!正式一致(采用)) throw std::logic_error("因果已发布子需求读回不一致");
                        检查停止();
                        continue; // 发布后重试只读；不覆盖首次发布结论。
                    }
                    if (采用.状态 != 新子需求采用状态::未处理 && 采用.状态 != 新子需求采用状态::资源失败) continue;
                    if (!续接已成立前缀) {
                        const auto 仍需采用=当前仍需采用(采用);
                        if(仍需采用==本次因果采用判断::重新读取 || 仍需采用==本次因果采用判断::等待能力) {
                            结果.状态=新自我消息处理状态::已交接未收回;return 结果;
                        }
                        if(仍需采用==本次因果采用判断::已无需要) { 停止剩余(新子需求采用状态::来源过期); break; }
                    }
                    检查停止();
                    const auto& 目标 = 采用.原提议;
                    if (!投影一致(目标)) throw std::logic_error("因果目标投影与正式域不一致");
                    HY_SELF_CAUSAL_PHASE(L"比较类型概念建立前", nullptr, std::nullopt);
                    检查停止();
                    const auto 当前算法 = 算法(目标);
                    if (!当前算法) { 结果.状态 = 新自我消息处理状态::已交接未收回; return 结果; }
                    const auto 类型 = 概念服务.建立或取得默认二次特征概念(
                        *当前算法, 输入定义(目标.目标特征类型概念节点), 目标.目标值域概念节点);
                    HY_SELF_CAUSAL_PHASE(L"比较类型概念建立后", 主线因果状态文字(类型.状态), std::nullopt);
                    if (类型.状态 == 二次特征概念操作状态::资源失败) {
                        检查停止();
                        结果.状态 = 新自我消息处理状态::已交接未收回; return 结果;
                    }
                    if (!类型.概念节点 || (类型.状态 != 二次特征概念操作状态::已建立
                        && 类型.状态 != 二次特征概念操作状态::已复用)) throw std::logic_error("因果目标比较类型建立失败");
                    检查停止();
                    HY_SELF_CAUSAL_PHASE(L"比较关系概念建立前", nullptr, std::nullopt);
                    const auto 关系 = 概念服务.建立或取得参照结果域概念(
                        *类型.概念节点, 二次特征结果域种类::包含于参照值域);
                    HY_SELF_CAUSAL_PHASE(L"比较关系概念建立后", 主线因果状态文字(关系.状态), std::nullopt);
                    if (关系.状态 == 二次特征概念操作状态::资源失败) {
                        检查停止();
                        结果.状态 = 新自我消息处理状态::已交接未收回; return 结果;
                    }
                    if (!关系.概念节点 || (关系.状态 != 二次特征概念操作状态::已建立
                        && 关系.状态 != 二次特征概念操作状态::已复用)) throw std::logic_error("因果目标比较关系建立失败");
                    检查停止();
                    新普通需求建立请求 建立请求;
                    建立请求.父需求节点 = 请求.父需求节点;
                    建立请求.目标存在节点 = 目标.目标存在节点;
                    建立请求.目标 = {目标.目标特征类型概念节点, 新需求目标形式::值,
                        目标.目标值域概念节点, 关系.概念节点, {}};
                    建立请求.来源类型 = 新需求来源类型::任务子目标;
                    建立请求.来源承接节点 = 请求.父任务节点;
                    HY_SELF_CAUSAL_PHASE(L"子需求建立复用前", nullptr, std::nullopt);
                    检查停止();
                    const auto 建立 = 目标.新增成员见证要求
                        ?需求服务->建立或取得新增成员见证需求已核验(建立请求,*目标.新增成员见证要求)
                        :需求服务->建立或取得普通需求已核验(建立请求, true);
                    HY_SELF_CAUSAL_PHASE(L"子需求建立复用后", 主线因果状态文字(建立.状态), 建立.需求节点);
                    if (建立.需求节点) {
                        if(采用.需求节点&&采用.需求节点!=建立.需求节点)
                            throw std::logic_error("因果子需求续接改变已成立需求身份");
                        采用.需求节点 = 建立.需求节点;
                    }
                    if(建立.状态==新需求操作状态::前提已变化&&!采用.需求节点){
                        停止剩余(新子需求采用状态::来源过期);break;
                    }
                    // 目标内容依赖未就绪属合法等待，与资源失败同按私有pending续接。
                    if (建立.状态 == 新需求操作状态::资源失败
                        ||建立.状态==新需求操作状态::前提已变化
                        ||建立.状态==新需求操作状态::目标内容依赖未就绪) {
                        采用.状态=新子需求采用状态::资源失败; // 私有pending；保留D/K/B并续接唯一出生核心。
                        检查停止();
                        结果.状态 = 新自我消息处理状态::已交接未收回; return 结果;
                    }
                    if (!采用.需求节点 || (建立.状态 != 新需求操作状态::已建立
                        && 建立.状态 != 新需求操作状态::已复用))
                        throw std::logic_error("因果子需求发布失败");
                    检查停止();
                    if (!正式一致(采用))
                        throw std::logic_error("因果子需求发布或独立读回失败");
                    采用.状态=建立.状态==新需求操作状态::已建立
                        ? 新子需求采用状态::已建立 : 新子需求采用状态::已复用;
                    检查停止();
                    {
                        // 已持状态互斥：不可再调检查停止()（其内部会再 lock → resource_deadlock_would_occur）。
                        std::lock_guard 锁(状态互斥);
                        if (停止.stop_requested()) throw 新任务因果读取取消{};
                        if (接收认知 && 接收认知待唤醒) throw 接收认知让出最终{};
                        请求关联复核已加锁(建立.状态 == 新需求操作状态::已建立||续接已成立前缀);
                    }
                    检查停止();
                }
                检查停止();
                回执.已完成处理 = true;
            }
            检查停止();
            {
                // R111：发送前逐项判定合法性。S0：因果回执须与提议逐项齐全（项数等于子需求项
                // 与来源前端之和），不能只提交合法项；有不合法项时整组不提交，只发通知（偏差）。
                std::vector<稳定编码> 已采用子需求;
                for (const auto& 采用 : 回执.项组)
                    if (采用.需求节点 && (采用.状态 == 新子需求采用状态::已建立
                            || 采用.状态 == 新子需求采用状态::已复用))
                        已采用子需求.push_back(*采用.需求节点);
                const auto 分组 = 判定子需求组(
                    定位.任务节点, 已采用子需求, 自我需求承接依据::因果子需求);
                新子需求不合法候选 候选;
                候选.种类 = 新子需求不合法候选种类::因果路径;
                候选.因果依据 = 原.依据;
                if (const auto 推进 = 通知子需求合法性(定位, 分组, 候选)) {
                    if (*推进 == 子需求通知推进::重投) {
                        结果.状态 = 新自我消息处理状态::已交接未收回; return 结果;
                    }
                    if (*推进 == 子需求通知推进::已停止) {
                        标记因果停止并移交进度(项, 结果); return 结果;
                    }
                    if (!分组.不合法.empty()) {
                        结果.状态 = 新自我消息处理状态::需求不合法未承接; return 结果;
                    }
                }
            }
            检查停止();
            新自我筹办裁决 裁决;
            裁决.定位 = 定位;
            裁决.状态 = 新自我筹办裁决状态::已处理因果子需求;
            裁决.因果回执 = 回执;
            结果.筹办裁决 = 裁决;
            HY_SELF_CAUSAL_PHASE(L"因果采用裁决前", nullptr, std::nullopt);
            检查停止();
            const auto 接收 = 完整接收(任务管理->提交自我因果筹办裁决(裁决, 停止));
            HY_SELF_CAUSAL_PHASE(L"因果采用裁决后", 主线因果状态文字(接收), std::nullopt);
            结果.筹办裁决接收状态 = 接收;
            检查停止();
            if (接收 == 新自我筹办裁决接收状态::资源失败) {
                结果.状态 = 新自我消息处理状态::已交接未收回;
            } else if (接收 == 新自我筹办裁决接收状态::已接收 || 接收 == 新自我筹办裁决接收状态::精确重复) {
                结果.状态 = 新自我消息处理状态::任务筹办已裁决;
            } else if (接收 == 新自我筹办裁决接收状态::定位已变化) {
                结果.状态 = 新自我消息处理状态::任务筹办定位已变化;
            }
            return 结果;
        } catch (const 新任务因果读取取消&) {
            标记因果停止并移交进度(项, 结果);
            return 结果;
        } catch (const 接收认知让出最终&) {
            try { (void)调试日志<调试切片::RECEIVED_DEMAND_SCHED>(L"自我线程", L"筹办让出接收认知",
                L"因果筹办检查点见接收认知待唤醒，交回运行循环入口。"); } catch (...) {}
            结果.状态 = 新自我消息处理状态::已交接未收回;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = 新自我消息处理状态::已交接未收回;
            return 结果;
        } catch (const std::length_error&) {
            结果.状态 = 新自我消息处理状态::已交接未收回;
            return 结果;
        } catch (const std::exception& error) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"处理因果子需求提议", L"正式因果依据、目标或已发布子需求不一致",error.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"处理因果子需求提议", L"正式因果依据、目标或已发布子需求不一致");
        }
    }

    std::optional<新自我消息处理结果> 处理复合目标提议(
        消息项& 项, const 新任务筹办处理结果& 筹办) noexcept {
        新自我消息处理结果 结果;
        结果.原消息 = 项.消息;
        结果.筹办独立读回 = 筹办;
        const auto& 定位 = *项.消息.筹办反馈;
        try {
            if (!项.复合子需求采用) {
                if (!筹办.复合目标提议) {
                    结果.状态 = 新自我消息处理状态::任务筹办需核查; return 结果;
                }
                新任务复合采用请求 请求;
                请求.父任务节点 = 定位.任务节点;
                请求.原提议 = *筹办.复合目标提议;
                请求.采用组.reserve(请求.原提议.子目标组.size());
                项.复合子需求采用 = std::move(请求);
            }
            auto& 请求 = *项.复合子需求采用;
            for (const auto& 子 : 请求.原提议.子目标组) {
                if (std::any_of(请求.采用组.begin(), 请求.采用组.end(),
                    [&](const auto& 已有) { return 已有.原提议 == 子; })) continue;
                // 发表D会改变需求树权重与保护版本；只用fresh事实重新核必要性，
                // 不用发表前的整批快照把本消息已经发表的前缀判为失效。
                const auto 本次 = 任务服务->读取当前复合目标提议(定位.任务节点);
                const auto& 当前 = 本次.提议;
                if (当前.状态 == 新任务复合分解状态::结构依赖未就绪) {
                    (void)结构依赖未就绪已停止(L"处理复合目标提议/读取当前复合目标提议",static_cast<int>(当前.状态),定位.任务节点.值);
                    结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                }
                if (当前.状态 == 新任务复合分解状态::资源失败
                    || 当前.状态 == 新任务复合分解状态::依赖未就绪) return std::nullopt;
                if (当前.父需求节点 != 请求.原提议.父需求节点
                    || 当前.父目标 != 请求.原提议.父目标
                    || 当前.判定.分支组 != 请求.原提议.判定.分支组) {
                    结果.状态 = 新自我消息处理状态::任务筹办定位已变化; return 结果;
                }
                if (std::find(当前.子目标组.begin(), 当前.子目标组.end(), 子) == 当前.子目标组.end())
                    continue; // 已满足项目不新建；已经发表的身份始终保留。
                新普通需求建立请求 建立请求;
                建立请求.父需求节点 = 当前.父需求节点;
                建立请求.目标存在节点 = 子.目标.目标存在节点;
                建立请求.目标 = 子.目标.目标;
                建立请求.来源类型 = 新需求来源类型::任务子目标;
                建立请求.来源承接节点 = 定位.任务节点;
                建立请求.复合来源 = 子.来源;
                // CONCEPT-INFO-MEMBERSHIP §4.4：存在归属分解子目标是普通值目标，比较关系按条件子需求同一约定补齐。
                const bool 归属分解 = 是存在归属分解(子.来源);
                if (归属分解) {
                    if (!子.目标.目标.目标值域概念节点 || 子.目标.目标.目标比较关系概念节点)
                        throw std::logic_error("存在归属分解子目标形状不合法");
                    const auto 事实 = 世界树服务->获取事实服务引用();
                    const auto 域 = 事实.特征概念服务.获取特征概念(*子.目标.目标.目标值域概念节点);
                    if (!域) throw std::logic_error("存在归属分解子目标值域不可读");
                    const auto 算法 = 域->材料类型 == 特征概念材料物理类型::I64标量
                        ? 二次特征算法::有向特征差值 : 二次特征算法::特征材料相似度;
                    const auto 类型 = 事实.二次特征概念服务.建立或取得默认二次特征概念(
                        算法, 子需求比较输入(子.目标.目标.目标特征类型概念节点), *子.目标.目标.目标值域概念节点);
                    if (类型.状态 == 二次特征概念操作状态::资源失败) return std::nullopt;
                    if (!类型.概念节点 || (类型.状态 != 二次特征概念操作状态::已建立
                            && 类型.状态 != 二次特征概念操作状态::已复用))
                        throw std::logic_error("存在归属分解子目标比较类型建立失败");
                    const auto 关系 = 事实.二次特征概念服务.建立或取得参照结果域概念(
                        *类型.概念节点, 二次特征结果域种类::包含于参照值域);
                    if (关系.状态 == 二次特征概念操作状态::资源失败) return std::nullopt;
                    if (!关系.概念节点 || (关系.状态 != 二次特征概念操作状态::已建立
                            && 关系.状态 != 二次特征概念操作状态::已复用))
                        throw std::logic_error("存在归属分解子目标比较关系建立失败");
                    建立请求.目标.目标比较关系概念节点 = *关系.概念节点;
                }
                const auto 建立 = 归属分解 ? 需求服务->建立或取得归属分解需求已核验(建立请求)
                    : 需求服务->建立或取得复合提议需求已核验(建立请求, 本次.内容);
                if (建立.状态 == 新需求操作状态::资源失败
                    || 建立.状态 == 新需求操作状态::目标内容依赖未就绪
                    || 建立.状态 == 新需求操作状态::目标内容未支持) return std::nullopt;
                if (建立.状态 == 新需求操作状态::前提已变化) {
                    结果.状态 = 新自我消息处理状态::任务筹办定位已变化; return 结果;
                }
                if (!建立.需求节点 || (建立.状态 != 新需求操作状态::已建立
                    && 建立.状态 != 新需求操作状态::已复用))
                    throw std::logic_error("复合子需求正式建立被拒绝");
                // reserve在首次写之前完成，新增前缀不再分配。
                请求.采用组.push_back({子, *建立.需求节点});
                const auto D = 需求服务->获取需求(*建立.需求节点);
                if (!D || D->父需求节点 != 当前.父需求节点
                    || D->复合来源 != std::optional{子.来源}
                    || 新需求完整目标{D->目标存在节点,D->目标}
                        != 新需求完整目标{建立请求.目标存在节点, 建立请求.目标})
                    throw std::logic_error("复合子需求正式独立读回不一致");
            }
            // R111：发送前逐项判定合法性。S0：复合采用核验只要求所交各项属于当前分解，
            // 允许缺项；不合法项不放入采用组，其余合法项照常提交，之后再发不合法通知
            // （父任务由提交转入等待条件后，通知再把它写为“子需求不合法”）。
            std::vector<稳定编码> 已采用子需求;
            for (const auto& 采用 : 请求.采用组) 已采用子需求.push_back(采用.子需求节点);
            const auto 分组 = 判定子需求组(
                定位.任务节点, 已采用子需求, 自我需求承接依据::复合子需求);
            新子需求不合法候选 候选;
            候选.种类 = 新子需求不合法候选种类::复合分解;
            候选.复合分解 = 请求.原提议;
            if (!分组.恢复合法.empty()) {
                const auto 推进 = 通知子需求合法性(定位, {{}, 分组.恢复合法}, 候选);
                if (推进 == 子需求通知推进::重投) return std::nullopt;
                if (推进 == 子需求通知推进::已停止) {
                    结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                }
            }
            auto 提交请求 = 请求;
            std::erase_if(提交请求.采用组, [&](const auto& 采用) {
                return std::find(分组.不合法.begin(), 分组.不合法.end(), 采用.子需求节点)
                    != 分组.不合法.end();
            });
            if (!项.复合合法项已提交
                && (分组.不合法.empty() || !提交请求.采用组.empty())) {
                const auto 接收 = 任务管理->提交已确认复合子需求(定位, 提交请求);
                if (接收 == 新任务操作状态::资源失败) return std::nullopt;
                if (接收 != 新任务操作状态::已修改) {
                    结果.状态 = 新自我消息处理状态::任务筹办定位已变化; return 结果;
                }
                项.复合合法项已提交 = true;
            }
            if (分组.不合法.empty()) {
                结果.状态 = 新自我消息处理状态::任务筹办已裁决; return 结果;
            }
            const auto 推进 = 通知子需求合法性(定位, {分组.不合法, {}}, 候选);
            if (推进 == 子需求通知推进::重投) return std::nullopt;
            if (推进 == 子需求通知推进::已停止) {
                结果.状态 = 新自我消息处理状态::停止取消; return 结果;
            }
            结果.状态 = 新自我消息处理状态::需求不合法未承接;
            return 结果;
        } catch (const std::bad_alloc&) { return std::nullopt; }
        catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复合目标自我采用", L"正式复合目标采用发生工程错误");
        }
    }

    // 空结果只表示manager暂时不能接纳裁决；保留原反馈定位后重试。
    std::optional<新自我消息处理结果> 处理筹办反馈(消息项& 项) noexcept {
        新自我消息处理结果 结果;
        结果.原消息 = 项.消息;
        if (停止源.stop_requested()) {
            标记因果停止并移交进度(项, 结果);
            return 结果;
        }
        {
            std::lock_guard 锁(状态互斥);
            if (接收认知 && 接收认知待唤醒) {
                try { (void)调试日志<调试切片::RECEIVED_DEMAND_SCHED>(L"自我线程", L"筹办入口让出",
                    L"处理筹办反馈入口见接收认知待唤醒，先回运行循环。"); } catch (...) {}
                return std::nullopt;
            }
        }
        if (!任务管理 || !项.消息.筹办反馈) {
            结果.状态 = 新自我消息处理状态::任务管理未就绪;
            return 结果;
        }
        const auto& 定位 = *项.消息.筹办反馈;
        if (项.因果子需求回执) {
            // 即使manager最近定位已变，已发布前缀仍须带原回执收口，
            // 不能先按旧定位退出而丢失本消息已发生的发布记录。
            auto 采用 = 处理因果子需求提议(项, {});
            if (采用.状态 == 新自我消息处理状态::已交接未收回) return std::nullopt;
            return 采用;
        }
        // manager在全部承接收口后可以释放旧定位；已接收回执的future仍
        // 必须由本消息收尾，不能因最近筹办已变化而丢失交接结果。
        if (项.条件子需求回执已接收) {
            auto 采用 = 处理条件子需求提议(项, {});
            if (采用.状态 == 新自我消息处理状态::已交接未收回)
                return std::nullopt;
            return 采用;
        }
        结果.筹办独立读回 = 任务管理->读取最近筹办结果(定位.任务节点);
        if (停止源.stop_requested()) {
            标记因果停止并移交进度(项, 结果);
            return 结果;
        }
        if (!结果.筹办独立读回
            || 结果.筹办独立读回->原工作 != 定位.原工作) {
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            try { (void)调试日志<调试切片::PARAM_BINDING>(L"自我筹办反馈", L"定位已变化",
                L"任务=" + std::to_wstring(定位.任务节点.值) + L" 有读回=" + std::to_wstring(结果.筹办独立读回.has_value())); } catch (...) {}
#endif
            结果.状态 = 新自我消息处理状态::任务筹办定位已变化;
            return 结果;
        }
        if ((!项.复合子需求采用 || 项.复合子需求采用->采用组.empty())
            && 结果.筹办独立读回->状态 == 新任务筹办处理状态::内容结果定位未支持) {
            const auto& 工作 = 结果.筹办独立读回->工作结果;
            if (!工作 || !工作->方法召回 || !工作->方法召回->内容学习发现)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"消费内容学习发现", L"原内容叶反馈缺少完整发现材料。");
            const auto& 原发现 = *工作->方法召回->内容学习发现;
            const auto 当前 = 任务服务->查询内容叶方法学习发现(定位.任务节点);
            if (当前.状态 == 新任务内容学习发现状态::结构依赖未就绪) {
                (void)结构依赖未就绪已停止(L"处理筹办反馈/查询内容叶方法学习发现",static_cast<int>(当前.状态),定位.任务节点.值);
                结果.状态 = 新自我消息处理状态::停止取消; return 结果;
            }
            if (当前.状态 == 新任务内容学习发现状态::资源失败
                || 当前.状态 == 新任务内容学习发现状态::查询未完成) return std::nullopt;
            if (当前.状态 != 新任务内容学习发现状态::已形成 || !当前.发现
                || *当前.发现 != 原发现 || 原发现.任务节点 != 定位.任务节点) {
                if (当前.状态 == 新任务内容学习发现状态::已形成 && !当前.发现)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"消费内容学习发现", L"成功发现缺少完整材料。");
                新自我筹办裁决 变化裁决;
                变化裁决.定位 = 定位;
                变化裁决.状态 = 新自我筹办裁决状态::任务状态变化;
                结果.筹办裁决 = 变化裁决;
                const auto 接收 = 任务管理->提交自我筹办裁决(变化裁决);
                结果.筹办裁决接收状态 = 接收;
                switch (接收) {
                case 新自我筹办裁决接收状态::已接收:
                case 新自我筹办裁决接收状态::精确重复:
                    结果.状态 = 新自我消息处理状态::任务筹办已裁决; return 结果;
                case 新自我筹办裁决接收状态::定位已变化:
                    结果.状态 = 新自我消息处理状态::任务筹办定位已变化; return 结果;
                case 新自我筹办裁决接收状态::资源失败: return std::nullopt;
                case 新自我筹办裁决接收状态::已停止接收:
                    结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                default:
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"收束内容旧工作", L"内容发现变化裁决未被可靠接收。");
                }
            }
            for (const auto M : 原发现.可学习方法节点组) {
                if (项.已处理内容学习方法.contains(M)) continue;
                const auto 学习 = 发现方法学习(M);
                if (学习 == 方法学习发现状态::暂缓) return std::nullopt;
                if (学习 == 方法学习发现状态::停止取消) {
                    结果.状态 = 新自我消息处理状态::停止取消;
                    return 结果;
                }
                try { 项.已处理内容学习方法.insert(M); }
                catch (const std::bad_alloc&) { return std::nullopt; }
            }
        }
        if (项.复合子需求采用
            || 结果.筹办独立读回->状态 == 新任务筹办处理状态::复合目标需自我确认
            || 结果.筹办独立读回->状态 == 新任务筹办处理状态::内容结果定位未支持)
            return 处理复合目标提议(项, *结果.筹办独立读回);
        if (停止源.stop_requested()) {
            标记因果停止并移交进度(项, 结果);
            return 结果;
        }
        std::vector<新任务依赖键> 词典等待键;
        bool 词典条件暂缺 = false;
        bool 词典需要学习 = false;
        bool 词典已有知识 = false;
        if (const auto 候选=任务服务->获取当前候选方法材料(定位.任务节点)) {
            for (const auto 方法节点:候选->候选方法节点组) {
                if (停止源.stop_requested()) {
                    标记因果停止并移交进度(项, 结果);
                    return 结果;
                }
                std::optional<新词典学习处置> 处置;
                const auto 学习 = 发现候选方法学习(定位.任务节点, 方法节点, 词典等待键, 处置);
                if (学习 == 方法学习发现状态::暂缓) return std::nullopt;
                if (学习 == 方法学习发现状态::停止取消) {
                    结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                }
                词典条件暂缺 |= 处置 == 新词典学习处置::条件未具备;
                词典需要学习 |= 处置 == 新词典学习处置::可提交学习;
                词典已有知识 |= 处置 == 新词典学习处置::已有覆盖知识
                    || 处置 == 新词典学习处置::已有空结果配对;
            }
        }
        if (停止源.stop_requested()) {
            标记因果停止并移交进度(项, 结果);
            return 结果;
        }
        if (项.条件子需求回执
            || (结果.筹办独立读回->状态 == 新任务筹办处理状态::条件筹办已完成
                && 结果.筹办独立读回->独立子需求提议
                && 结果.筹办独立读回->独立子需求提议->状态
                    == 新任务条件子需求提议状态::已形成)) {
            auto 采用 = 处理条件子需求提议(项, *结果.筹办独立读回);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            try { (void)调试日志<调试切片::PARAM_BINDING>(L"自我筹办反馈", L"条件子需求采用",
                L"任务=" + std::to_wstring(定位.任务节点.值) + L" 处理状态=" + std::to_wstring(static_cast<int>(采用.状态))); } catch (...) {}
#endif
            if (采用.状态 == 新自我消息处理状态::已交接未收回)
                return std::nullopt;
            return 采用;
        }
        if (项.因果子需求回执 || (结果.筹办独立读回->因果子需求提议
            && (结果.筹办独立读回->因果子需求提议->状态 == 新任务因果子需求提议状态::已形成
                || 结果.筹办独立读回->因果子需求提议->状态 == 新任务因果子需求提议状态::无需子需求
                || 结果.筹办独立读回->因果子需求提议->状态 == 新任务因果子需求提议状态::路径待选择
                || 结果.筹办独立读回->因果子需求提议->状态 == 新任务因果子需求提议状态::能力未支持))) {
            auto 采用 = 处理因果子需求提议(项, *结果.筹办独立读回);
            if (采用.状态 == 新自我消息处理状态::已交接未收回) return std::nullopt;
            return 采用;
        }
        const auto 分类候选 = 任务服务->获取当前候选方法材料(定位.任务节点);
        结果.筹办裁决 = 分类筹办结果(
            定位, *结果.筹办独立读回);
        结果.筹办裁决->附加等待依赖键组 = std::move(词典等待键);
        if (!词典已有知识 && (词典条件暂缺 || 词典需要学习)
            && (结果.筹办裁决->状态 == 新自我筹办裁决状态::等待材料
                || 结果.筹办裁决->状态 == 新自我筹办裁决状态::当前无可用方法)) {
            结果.筹办裁决->状态 = 词典条件暂缺 ? 新自我筹办裁决状态::等待材料
                : 新自我筹办裁决状态::当前无可用方法;
        }
        if (停止源.stop_requested()) { 结果.状态 = 新自我消息处理状态::停止取消; return 结果; }
        if (结果.筹办裁决->状态 == 新自我筹办裁决状态::当前无可用方法
            && !词典需要学习 && !词典条件暂缺
            && !登记当前缺失方法(定位, 分类候选)) {
            // 仍将失效决定交回manager收口旧反馈；不遗留永远待接收的定位。
            结果.筹办裁决->状态 = 新自我筹办裁决状态::任务状态变化;
        }
        if (结果.筹办裁决->状态
                == 新自我筹办裁决状态::可进入执行前判断
            && 结果.筹办裁决->唯一具备方法节点
            && 结果.筹办裁决->唯一具备配对节点
            && 结果.筹办裁决->唯一对应结果节点
            && 结果.筹办裁决->本能采用) {
            结果.执行方向核验 = 任务服务->核验任务执行方向(
                定位.任务节点,
                *结果.筹办裁决->本能采用,
                *初始化结果->自我存在,
                *初始化结果->安全根需求,
                *初始化结果->服务根需求);
            if (结果.执行方向核验->状态
                == 新任务治理执行方向核验状态::资源失败) {
                return std::nullopt;
            }
            if (结果.执行方向核验->状态 == 新任务治理执行方向核验状态::结构依赖未就绪) {
                (void)结构依赖未就绪已停止(L"处理筹办反馈/核验任务执行方向",static_cast<int>(结果.执行方向核验->状态),定位.任务节点.值);
                结果.状态 = 新自我消息处理状态::停止取消; return 结果;
            }
            新自我执行前判断 判断;
            判断.任务节点 = 定位.任务节点;
            判断.方法节点 = *结果.筹办裁决->唯一具备方法节点;
            判断.配对节点 = *结果.筹办裁决->唯一具备配对节点;
            判断.结果节点 = *结果.筹办裁决->唯一对应结果节点;
            switch (结果.执行方向核验->状态) {
            case 新任务治理执行方向核验状态::已核验可执行:
                判断.状态 = 新自我执行前判断状态::允许执行;
                break;
            case 新任务治理执行方向核验状态::等待材料:
            case 新任务治理执行方向核验状态::需求信息待完善:
                判断.状态 = 新自我执行前判断状态::暂缓执行;
                break;
            case 新任务治理执行方向核验状态::任务目标方向不一致:
            case 新任务治理执行方向核验状态::存在冲突需求:
            case 新任务治理执行方向核验状态::安全方向不一致:
            case 新任务治理执行方向核验状态::服务方向不一致:
                判断.状态 = 新自我执行前判断状态::拒绝执行;
                break;
            case 新任务治理执行方向核验状态::任务目标已满足:
            case 新任务治理执行方向核验状态::任务状态已变化:
            case 新任务治理执行方向核验状态::定位已变化:
                判断.状态 = 新自我执行前判断状态::重新判断;
                break;
            case 新任务治理执行方向核验状态::结果定义暂不支持:
                判断.状态 = 新自我执行前判断状态::缺少执行依赖;
                break;
            case 新任务治理执行方向核验状态::结构不一致:
            case 新任务治理执行方向核验状态::资源失败:
            default:
                判断.状态 = 新自我执行前判断状态::需核查;
                break;
            }
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
            try {
                const auto& 核 = *结果.执行方向核验;
                std::wostringstream 文本;
                文本 << L"任务=" << 定位.任务节点.值 << L" 方向核验=" << static_cast<int>(核.状态)
                     << L" 阶段数=" << 核.预计变化阶段组.size()
                     << L" 变化数=" << (核.预计变化阶段组.empty() ? 0 : 核.预计变化阶段组.front().变化组.size())
                     << L" 获取方案=" << (核.结果获取方案 ? (核.结果获取方案->完整() ? 2 : 1) : 0)
                     << L" 目标数=" << (核.结果获取方案 ? 核.结果获取方案->目标组.size() : 0)
                     << L" 采用同=" << (核.本能采用 == 结果.筹办裁决->本能采用);
                (void)调试日志<调试切片::PARAM_BINDING>(L"执行前判断", L"方向核验", 文本.str());
            } catch (...) {}
#endif
            // 2026-11-07 用户裁决：执行前判断暂时全部判通过。任务状态/定位已变化（事实变化）
            // 与结构不一致/资源失败（工程故障）不是判断结论，仍按原口径处理。
            if (判断.状态 == 新自我执行前判断状态::暂缓执行
                || 判断.状态 == 新自我执行前判断状态::拒绝执行
                || 判断.状态 == 新自我执行前判断状态::缺少执行依赖)
                判断.状态 = 新自我执行前判断状态::允许执行;
            if (判断.状态 == 新自我执行前判断状态::允许执行) {
                // 方向许可绑定完整采用；后续只复核它，不因新候选出现重新排名。
                if (结果.执行方向核验->本能采用 != 结果.筹办裁决->本能采用) {
                    判断.状态 = 新自我执行前判断状态::重新判断;
                } else {
                    新任务治理本能执行请求 请求;
                    请求.任务节点 = 判断.任务节点;
                    请求.方法节点 = 判断.方法节点;
                    请求.配对节点 = 判断.配对节点;
                    请求.结果节点 = 判断.结果节点;
                    请求.自我存在节点 = *初始化结果->自我存在;
                    请求.安全需求节点 = *初始化结果->安全根需求;
                    请求.服务需求节点 = *初始化结果->服务根需求;
                    请求.本能采用 = 结果.筹办裁决->本能采用;
                    请求.参数绑定组 = 请求.本能采用->参数绑定组;
                    请求.预计变化阶段组 =
                        结果.执行方向核验->预计变化阶段组;
                    if (结果.执行方向核验->结果获取方案) {
                        请求.结果获取方案 =
                            *结果.执行方向核验->结果获取方案;
                    }
                    if (请求.完整()) {
                        判断.本能执行请求 = std::move(请求);
                    } else {
                        判断.状态 = 新自我执行前判断状态::需核查;
                    }
                }
            }
            结果.筹办裁决->执行前判断 = 判断;
        }
        结果.筹办裁决接收状态 =
            任务管理->提交自我筹办裁决(*结果.筹办裁决);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
        if (结果.筹办裁决->执行前判断) try {
            const auto& 判 = *结果.筹办裁决->执行前判断;
            std::wostringstream 文本;
            文本 << L"任务=" << 定位.任务节点.值 << L" 判断=" << static_cast<int>(判.状态)
                 << L" 有请求=" << 判.本能执行请求.has_value()
                 << L" 请求完整=" << (判.本能执行请求 && 判.本能执行请求->完整())
                 << L" 接收=" << static_cast<int>(*结果.筹办裁决接收状态);
            (void)调试日志<调试切片::PARAM_BINDING>(L"执行前判断", L"提交", 文本.str());
        } catch (...) {}
#endif
        switch (*结果.筹办裁决接收状态) {
        case 新自我筹办裁决接收状态::已接收:
        case 新自我筹办裁决接收状态::精确重复:
            结果.状态 = 新自我消息处理状态::任务筹办已裁决;
            return 结果;
        case 新自我筹办裁决接收状态::资源失败:
            return std::nullopt;
        case 新自我筹办裁决接收状态::定位已变化:
            结果.状态 = 新自我消息处理状态::任务筹办定位已变化;
            return 结果;
        case 新自我筹办裁决接收状态::已停止接收:
            结果.状态 = 新自我消息处理状态::任务管理未就绪;
            return 结果;
        case 新自我筹办裁决接收状态::身份冲突:
            结果.状态 = 新自我消息处理状态::内部不一致;
            return 结果;
        case 新自我筹办裁决接收状态::入口拒绝:
            结果.状态 = 新自我消息处理状态::任务筹办需核查;
            return 结果;
        case 新自我筹办裁决接收状态::内部错误:
        default:
            结果.状态 = 新自我消息处理状态::内部错误;
            return 结果;
        }
    }

    enum class 明确完成来源接纳 { 已接纳, 已停止, 工程停止 };
    明确完成来源接纳 接纳明确完成来源(稳定编码 任务节点) noexcept {
        if (!任务管理) return 明确完成来源接纳::工程停止;
        const auto 来源 = 任务服务->核验直接服务结算来源(任务节点);
        switch (来源.状态) {
        case 新直接服务结算来源状态::非直接服务:
        case 新直接服务结算来源状态::尚未完成:
        case 新直接服务结算来源状态::需重新确认:
            return 明确完成来源接纳::已接纳; // 不进入加值队列；保留已形成的任务完成事实。
        case 新直接服务结算来源状态::最终贡献消费尚未装配:
        case 新直接服务结算来源状态::允许结算: break;
        default:
            工程异常终止(L"新_自我线程类.cpp", __LINE__,
                L"核验服务结算来源", L"正式服务需求及完成闭包读取失败");
        }
        const auto 状态 = 任务管理->接纳服务值结算来源(任务节点);
        const bool 已接纳 = 状态 == 新服务值结算协作状态::已完成
            || 状态 == 新服务值结算协作状态::已接收待筹办
            || 状态 == 新服务值结算协作状态::精确重复
            || 状态 == 新服务值结算协作状态::非本方法
            || 状态 == 新服务值结算协作状态::已有其他来源
            || 状态 == 新服务值结算协作状态::来源不具备
            || 状态 == 新服务值结算协作状态::目标已满足
            || 状态 == 新服务值结算协作状态::最终贡献消费尚未装配;
        if (已接纳) return 明确完成来源接纳::已接纳;
        // 正常停机：manager已停止接收，不是工程停止，不报告。
        if (状态 == 新服务值结算协作状态::已停止接收) return 明确完成来源接纳::已停止;
        if (服务结算确认端口)
            服务结算确认端口->报告并停止("完成任务来源接纳或结算筹办失败");
        return 明确完成来源接纳::工程停止;
    }

    enum class 需求认知读取进度 { 可继续, 资源重试, 定位变化, 工程停止 };
    需求认知读取进度 读取本次需求认知(消息项& 项, 新自我消息处理结果& 结果,
        const 新任务结果反馈定位& 定位) noexcept {
        try {
            if (!任务服务) throw std::logic_error("需求认知缺少任务owner");
            const auto 读回 = 任务服务->读取执行已发布需求(定位.任务节点, 定位.执行实例节点);
            switch (读回.状态) {
            case 新任务需求结果读取状态::资源失败:
                return 需求认知读取进度::资源重试;
            case 新任务需求结果读取状态::前提已变化:
            case 新任务需求结果读取状态::执行不存在:
                结果.已认知需求.reset();
                结果.状态 = 新自我消息处理状态::任务结果定位已变化;
                return 需求认知读取进度::定位变化;
            case 新任务需求结果读取状态::已读取:
                if (!读回.结果 || 定位.已发布需求节点 != std::optional{读回.结果->需求节点})
                    throw std::logic_error("需求认知正式结果与消息索引不符");
                break;
            case 新任务需求结果读取状态::无此结果:
                if (读回.结果 || 定位.已发布需求节点)
                    throw std::logic_error("需求认知空结果与正式消息矛盾");
                break;
            default:
                throw std::logic_error("需求认知依赖未装配或状态异常");
            }
            if (项.需求认知已读取 && 项.需求认知材料 != 读回.结果)
                throw std::logic_error("同消息的正式需求结果身份改变");
            项.需求认知材料 = 读回.结果;
            项.需求认知已读取 = true;
            结果.已认知需求 = 读回.结果;
            return 需求认知读取进度::可继续;
        } catch (const std::bad_alloc&) {
            return 需求认知读取进度::资源重试;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取本次需求认知", L"同次X正式需求认知核验失败，不自动承接。");
        }
    }

    std::optional<新自我消息处理结果> 推进最终消费消息(消息项& 项) noexcept {
        try {
            if (!项.贡献前置结果 || !项.最终消费来源 || !任务管理 || !反馈桥 || !服务结算确认端口)
                throw std::logic_error("最终消费消息前置定位或端口缺失");
            if (项.消息.结果反馈) {
                if (!项.需求认知已读取)
                    throw std::logic_error("结果反馈进入最终消费前缺少正式需求认知读回");
                auto 认知结果 = *项.贡献前置结果;
                const auto 进度 = 读取本次需求认知(项, 认知结果, *项.消息.结果反馈);
                if (进度 == 需求认知读取进度::资源重试) return std::nullopt;
                if (进度 != 需求认知读取进度::可继续) return 认知结果;
                项.贡献前置结果->已认知需求 = 认知结果.已认知需求;
            }
            const auto 核验最终返回 = [&](const 新服务值最终消费结果& 消费,
                const 新服务值最终消费结果& 正式) {
                if (消费.来源 != *项.最终消费来源 || 正式.来源 != *项.最终消费来源)
                    throw std::logic_error("最终消费来源不一致");
                // 技术回执不证明零写；完成只能由同源正式U/J5独立读回确认。
                if (消费.状态 == 新服务值最终消费状态::资源延后
                    || 消费.状态 == 新服务值最终消费状态::已准备待续) {
                    if (正式.状态 != 新服务值最终消费状态::已完成) return false;
                    if (!正式.消费节点 || !正式.收尾阶段节点)
                        throw std::logic_error("资源延后后正式完成缺少U/J5");
                    return true;
                }
                const bool 完成 = 消费.状态 == 新服务值最终消费状态::已完成
                    || 消费.状态 == 新服务值最终消费状态::精确重复;
                if (完成 && (正式.状态 != 新服务值最终消费状态::已完成
                    || !消费.消费节点 || !消费.收尾阶段节点
                    || 正式.消费节点 != 消费.消费节点 || 正式.收尾阶段节点 != 消费.收尾阶段节点
                    || 正式.普通动作节点 != 消费.普通动作节点))
                    throw std::logic_error("self独立读回U/J5与实际回执不符");
                if (!完成 && (消费.消费节点 || 消费.普通动作节点 || 消费.收尾阶段节点
                    || (消费.状态 != 新服务值最终消费状态::来源条件不具备
                        && 消费.状态 != 新服务值最终消费状态::来源已变化
                        && 消费.状态 != 新服务值最终消费状态::等待配置
                        && 消费.状态 != 新服务值最终消费状态::等待同源工作)))
                    throw std::logic_error("未消费回执伪装完整工作");
                if (!完成 && 正式.状态 == 新服务值最终消费状态::已入账待收尾)
                    throw std::logic_error("未消费回执与已入账待收尾事实不一致");
                return 完成;
            };
            if (!项.最终消费工作 && !项.最终消费结果.valid()) {
                if (!项.直接最终消费结果) {
                    if (停止源.stop_requested() || 任务管理->读取停止令牌().stop_requested()) {
                        auto 结果 = *项.贡献前置结果;
                        结果.状态 = 新自我消息处理状态::停止取消;
                        return 结果;
                    }
                    项.直接最终消费结果.emplace(服务结算确认端口->执行最终来源工作(*项.最终消费来源));
                }
                const auto& 消费 = *项.直接最终消费结果;
                const auto 正式 = 服务结算确认端口->读取最终来源结果(*项.最终消费来源);
                if (正式.状态 == 新服务值最终消费状态::资源延后) return std::nullopt;
                if ((消费.状态 == 新服务值最终消费状态::资源延后
                        || 消费.状态 == 新服务值最终消费状态::已准备待续)
                    && 正式.状态 != 新服务值最终消费状态::已完成) {
                    if (正式.状态 != 新服务值最终消费状态::等待同源工作)
                        项.直接最终消费结果.reset(); // 下一轮只续原source；执行器先核U/P并收尾。
                    return std::nullopt;
                }
                const bool 完成 = 核验最终返回(消费, 正式);
                const auto 确认 = 项.直接最终消费已确认 ? 新任务完成结算确认状态::精确重复
                    : 任务管理->确认直接最终服务消费(*项.最终消费来源, 消费);
                if (确认 == 新任务完成结算确认状态::资源失败) return std::nullopt;
                auto 结果 = *项.贡献前置结果;
                if (确认 == 新任务完成结算确认状态::已停止接收) {
                    结果.状态 = 新自我消息处理状态::已交接未收回;
                    结果.未确认直接最终消费 = 消费;
                    return 结果;
                }
                if (确认 != 新任务完成结算确认状态::已确认 && 确认 != 新任务完成结算确认状态::精确重复)
                    throw std::logic_error("直接最终消费正式确认失败");
                项.直接最终消费已确认 = true;
                结果.状态 = 完成 ? 新自我消息处理状态::任务结果已裁决
                    : 新自我消息处理状态::任务完成贡献待归集;
                return 结果;
            }
            if (!项.最终消费结果.valid()) {
                const auto 接收 = 任务管理->提交最终服务消费工作(反馈桥.get(), *项.最终消费来源);
                if (接收.工作) {
                    if (项.最终消费工作 && 项.最终消费工作 != 接收.工作)
                        throw std::logic_error("最终消费资源重试改变工作编号");
                    项.最终消费工作 = 接收.工作;
                }
                switch (接收.状态) {
                case 新任务工作接收状态::已接收:
                case 新任务工作接收状态::精确重复:
                    if (!接收.工作 || !接收.处理结果.valid())
                        throw std::logic_error("已接收最终消费缺少请求或future");
                    项.最终消费结果 = 接收.处理结果;
                    {
                        // 同一工作可能先被另一消息拿到pending编号，再由本消息
                        // 入池。负回执ACK会释放当前K映射；须先把原future交给所有
                        // 已持同一编号的槽，避免其重试时误领下一代工作。
                        std::lock_guard 锁(状态互斥);
                        for (const auto& 等待项 : 活动消息) {
                            if (等待项->最终消费工作 == 项.最终消费工作
                                && 等待项->最终消费来源 == 项.最终消费来源
                                && !等待项->最终消费结果.valid()) {
                                等待项->最终消费结果 = 接收.处理结果;
                                等待项->可处理时点 = std::chrono::steady_clock::now();
                            }
                        }
                    }
                    break;
                case 新任务工作接收状态::任务已占用:
                case 新任务工作接收状态::邮箱已满:
                case 新任务工作接收状态::尚未启动:
                    return std::nullopt;
                case 新任务工作接收状态::已停止接收: {
                    auto 结果 = *项.贡献前置结果;
                    结果.状态 = 新自我消息处理状态::停止取消;
                    return 结果;
                }
                default:
                    throw std::logic_error("最终消费工作提交被拒绝");
                }
            }
            // 消息循环只作零时探测，绝不占住self等待worker。
            if (项.最终消费结果.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
                return std::nullopt;
            const auto& 回执 = 项.最终消费结果.get();
            if (!项.最终消费工作 || 回执.原请求 != *项.最终消费工作
                || 回执.原请求.最终来源 != 项.最终消费来源)
                throw std::logic_error("最终消费回执与原工作不一致");
            // 先回收已交付的transport；资源读回不再占住ready worker。
            const auto 确认 = 任务管理->确认最终服务消费工作(反馈桥.get(),
                项.最终消费工作->工作编号, *项.最终消费来源);
            if (确认 == 新任务工作操作状态::已在运行) return std::nullopt;
            if (确认 != 新任务工作操作状态::已完成 && 确认 != 新任务工作操作状态::无变化)
                throw std::logic_error("最终消费工作确认失败");
            auto 正式 = 服务结算确认端口->读取最终来源结果(*项.最终消费来源);
            if (正式.状态 == 新服务值最终消费状态::资源延后) return std::nullopt;
            const auto 技术待续 = [](const 新服务值最终消费结果& r) {
                return r.状态 == 新服务值最终消费状态::资源延后
                    || r.状态 == 新服务值最终消费状态::已准备待续;
            };
            if (回执.最终消费 && 技术待续(*回执.最终消费)
                && 正式.状态 != 新服务值最终消费状态::已完成) {
                if (停止源.stop_requested() || 任务管理->读取停止令牌().stop_requested()) {
                    auto 结果 = *项.贡献前置结果;
                    结果.状态 = 新自我消息处理状态::停止取消;
                    结果.未确认直接最终消费 = 项.直接最终消费结果
                        ? 项.直接最终消费结果 : 回执.最终消费;
                    return 结果;
                }
                if (正式.状态 == 新服务值最终消费状态::等待同源工作) return std::nullopt;
                if (项.直接最终消费结果 && 技术待续(*项.直接最终消费结果))
                    项.直接最终消费结果.reset();
                if (!项.直接最终消费结果) {
                    // 同源结果阶段续行，保原工作号/future；执行器负责原P/U防重。
                    项.直接最终消费结果.emplace(服务结算确认端口->执行最终来源工作(*项.最终消费来源));
                    if (项.直接最终消费结果->来源 != *项.最终消费来源)
                        throw std::logic_error("最终消费续接改变来源");
                }
                正式 = 服务结算确认端口->读取最终来源结果(*项.最终消费来源);
                if (正式.状态 == 新服务值最终消费状态::资源延后
                    || 技术待续(*项.直接最终消费结果)) return std::nullopt;
            }
            bool 完成 = false;
            if (回执.状态 == 新任务工作处理状态::最终服务消费已返回 && 回执.最终消费) {
                const auto& 消费 = 技术待续(*回执.最终消费)
                    && 正式.状态 == 新服务值最终消费状态::已完成 ? *回执.最终消费
                    : 项.直接最终消费结果 ? *项.直接最终消费结果 : *回执.最终消费;
                完成 = 核验最终返回(消费, 正式);
                // transport首次核验可能只因资源延后未清业务待办；正式完成统一补确认。
                if (完成 || 项.直接最终消费结果 || 技术待续(*回执.最终消费)) {
                    const auto 业务确认 = 任务管理->确认直接最终服务消费(*项.最终消费来源, 消费);
                    if (业务确认 == 新任务完成结算确认状态::资源失败) return std::nullopt;
                    if (业务确认 != 新任务完成结算确认状态::已确认
                        && 业务确认 != 新任务完成结算确认状态::精确重复) {
                        if (业务确认 == 新任务完成结算确认状态::已停止接收) return std::nullopt;
                        throw std::logic_error("最终消费同源续接确认失败");
                    }
                    项.直接最终消费已确认 = true;
                }
            } else if (回执.状态 != 新任务工作处理状态::停止取消 || 回执.最终消费) {
                throw std::logic_error("最终消费工作回执不是合法业务等待或完成");
            }
            auto 结果 = *项.贡献前置结果;
            结果.状态 = 完成 ? 新自我消息处理状态::任务结果已裁决
                : 回执.状态 == 新任务工作处理状态::停止取消 ? 新自我消息处理状态::停止取消
                : 新自我消息处理状态::任务完成贡献待归集;
            // 未消费也结束本条消息；后续具名进度事件才能生成下一次工作，不能忙重投。
            return 结果;
        } catch (const std::bad_alloc&) {
            return std::nullopt;
        } catch (const std::length_error&) {
            return std::nullopt;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"推进最终消费消息", L"最终消费交接或正式结果核验失败");
        }
    }

    std::optional<新自我消息处理结果> 处理贡献定位(
        消息项& 项, const 新任务贡献反馈定位& 定位, 新自我消息处理结果 结果) noexcept {
        if (项.贡献前置结果) return 推进最终消费消息(项);
        if (!任务管理) { 结果.状态 = 新自我消息处理状态::任务管理未就绪; return 结果; }
        try {
            项.贡献工作已交接阶段.store(true);
            const auto 核验贡献返回 = [&](const 新本能函数本次条件结果对& 对)
                -> const 新任务贡献治理返回材料* {
                const auto& r = 对.结果;
                if (!r.任务贡献治理 || r.任务贡献治理->来源 != 定位
                    || r.状态 != 新本能函数执行状态::已按合同返回
                    || r.结果取得状态 != 新本能函数结果取得状态::已取得
                    || r.动作阶段 != (r.任务贡献治理->本次动作组.empty()
                        ? 新本能函数动作阶段::未进入动作 : 新本能函数动作阶段::已按函数合同返回)
                    || !对.条件.参数角色组.empty() || r.动作动态节点 || !r.已形成状态节点组.empty()
                    || !r.已形成动态节点组.empty() || !r.已形成二次特征结果组.empty()
                    || !r.建议观察特征节点组.empty() || r.服务需求信息包 || r.服务对象观察材料 || r.服务值消费节点 || r.已发布需求
                    || (对.条件.任务贡献治理参数 && 对.条件.任务贡献治理参数->完成来源 != 定位)
                    || (!对.条件.任务贡献治理参数 && (!对.条件.治理动作前特征组.empty()
                        || !r.任务贡献治理->本次动作组.empty()
                        || r.任务贡献治理->推进.状态 != 新任务贡献推进状态::预期已变化)))
                    throw std::logic_error("贡献本能材料违反专用合同");
                任务服务->核验任务贡献治理返回材料(*r.任务贡献治理);
                return &*r.任务贡献治理;
            };
            const 新任务贡献治理返回材料* 材料 = nullptr;
            新任务完成结算确认状态 确认;
            const bool 直接调用 = !项.贡献治理工作 && !项.贡献治理结果.valid();
            if (直接调用) {
                if (!项.直接贡献结果) {
                    if (停止源.stop_requested() || 任务管理->读取停止令牌().stop_requested()) {
                        结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                    }
                    const auto 管理状态 = 任务管理->读取快照().生命周期;
                    if (管理状态 == 新任务管理生命周期::未启动 || 管理状态 == 新任务管理生命周期::启动中)
                        return std::nullopt;
                    if (管理状态 == 新任务管理生命周期::停止中 || 管理状态 == 新任务管理生命周期::已停止) {
                        结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                    }
                    if (管理状态 != 新任务管理生命周期::等待消息 && 管理状态 != 新任务管理生命周期::处理消息)
                        throw std::logic_error("直接贡献治理的任务管理生命周期异常");
                    if (!服务结算确认端口) throw std::logic_error("直接贡献治理缺少正式端口");
                    auto 返回 = 服务结算确认端口->执行贡献治理工作(定位);
                    (void)核验贡献返回(返回);
                    项.直接贡献结果.emplace(std::move(返回));
                }
                材料 = &*项.直接贡献结果->结果.任务贡献治理;
                确认 = 项.直接贡献已确认 ? 新任务完成结算确认状态::精确重复
                    : 任务管理->确认任务贡献复核(定位, 材料->推进);
                if (确认 == 新任务完成结算确认状态::已停止接收) {
                    结果.任务完成结算确认状态 = 确认;
                    结果.未确认直接贡献治理 = *材料;
                    结果.状态 = 新自我消息处理状态::已交接未收回;
                    return 结果;
                }
                if (确认 == 新任务完成结算确认状态::已确认 || 确认 == 新任务完成结算确认状态::精确重复)
                    项.直接贡献已确认 = true;
            } else {
            if (!项.贡献治理结果.valid()) {
                const auto 接收 = 任务管理->提交任务贡献治理工作(反馈桥.get(), 定位);
                if (接收.工作) {
                    if (项.贡献治理工作 && 项.贡献治理工作 != 接收.工作)
                        throw std::logic_error("贡献治理资源重试改变工作编号");
                    项.贡献治理工作 = 接收.工作;
                }
                switch (接收.状态) {
                case 新任务工作接收状态::已接收:
                case 新任务工作接收状态::精确重复:
                    if (!接收.工作 || !接收.处理结果.valid())
                        throw std::logic_error("贡献工作已接收但没有future");
                    项.贡献治理结果 = 接收.处理结果;
                    {
                        // 必须在首个槽ACK前交付原future，避免同号pending槽误领下一代。
                        std::lock_guard 锁(状态互斥);
                        for (const auto& 等待项 : 活动消息) {
                            if (等待项->贡献治理工作 == 项.贡献治理工作 && !等待项->贡献治理结果.valid()) {
                                等待项->贡献治理结果 = 接收.处理结果;
                                等待项->可处理时点 = std::chrono::steady_clock::now();
                            }
                        }
                    }
                    break;
                case 新任务工作接收状态::任务已占用:
                case 新任务工作接收状态::邮箱已满:
                case 新任务工作接收状态::尚未启动:
                case 新任务工作接收状态::资源失败:
                    return std::nullopt;
                case 新任务工作接收状态::已停止接收:
                    结果.状态 = 新自我消息处理状态::停止取消; return 结果;
                default: throw std::logic_error("贡献工作提交被拒绝");
                }
            }
            if (项.贡献治理结果.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
                return std::nullopt;
            const auto& 回执 = 项.贡献治理结果.get();
            if (!项.贡献治理工作 || 回执.原请求 != *项.贡献治理工作
                || 回执.原请求.贡献来源 != 定位 || 回执.方法召回 || 回执.条件筹办
                || 回执.条件子需求提议 || 回执.本能执行 || 回执.本能结果获取
                || 回执.调度材料 || 回执.执行组织交付 || 回执.本能结果检测 || 回执.最终消费)
                throw std::logic_error("贡献工作回执身份或负载不一致");
            if (回执.状态 == 新任务工作处理状态::任务贡献治理已返回 && 回执.贡献治理) {
                材料 = 核验贡献返回(*回执.贡献治理);
            } else if (回执.状态 != 新任务工作处理状态::停止取消 || 回执.贡献治理) {
                throw std::logic_error("贡献回执不是正式结果或未开始取消");
            }
            确认 = 任务管理->确认任务贡献治理工作(反馈桥.get(),
                项.贡献治理工作->工作编号, 定位);
            }
            结果.任务完成结算确认状态 = 确认;
            if (确认 == 新任务完成结算确认状态::资源失败) return std::nullopt;
            if (确认 == 新任务完成结算确认状态::定位已变化) {
                结果.状态 = 新自我消息处理状态::任务结果定位已变化; return 结果;
            }
            if (确认 != 新任务完成结算确认状态::已确认 && 确认 != 新任务完成结算确认状态::精确重复)
                throw std::logic_error("贡献工作确认失败");
            if (!材料) { 结果.状态 = 新自我消息处理状态::停止取消; return 结果; }
            if (材料->推进.状态 == 新任务贡献推进状态::预期已变化) {
                // 本次返回只确认定位漂移；manager已保留复核，不能借旧K继续最终消费。
                结果.状态 = 新自我消息处理状态::任务完成贡献待归集;
                return 结果;
            }
            // ACK后fresh读取，不能把本次返回的旧分类或标量当作当前最终消费资格。
            const auto 最终 = 任务服务->读取任务完成贡献交接(定位.任务节点);
            if (最终.状态 == 新任务贡献交接读取状态::预期已变化) return std::nullopt;
            if (最终.状态 != 新任务贡献交接读取状态::已读取 || !最终.交接)
                throw std::logic_error("贡献正式交接不可读");
            const auto 当前任务 = 任务服务->获取任务(定位.任务节点);
            if (!当前任务) throw std::logic_error("贡献来源任务不可读");
            if (当前任务->最近执行实例节点 != 定位.执行实例节点
                || (当前任务->当前状态 != 新任务状态::已完成 && 当前任务->当前状态 != 新任务状态::已结算)) {
                结果.状态 = 新自我消息处理状态::任务结果定位已变化; return 结果;
            }
            const auto& 推进 = 材料->推进;
            // R108：最新K属于更早轮（新一轮尚未结算）时不按旧K裁决，也不进入最终消费。
            bool 本轮有K = false;
            if (最终.交接->完成结算节点) {
                const auto 完成 = 任务服务->读取任务完成结算(定位.任务节点);
                if (!完成 || 完成->节点 != 最终.交接->完成结算节点)
                    return std::nullopt;
                本轮有K = 完成->执行实例节点 == 定位.执行实例节点;
            }
            if (本轮有K) {
                const auto 完成 = 任务服务->读取任务完成结算(定位.任务节点);
                if (!完成) return std::nullopt;
                结果.任务完成结算 = 新任务完成结算结果{新任务操作状态::精确重复, *完成};
                if (const auto 接纳 = 接纳明确完成来源(定位.任务节点);
                    接纳 != 明确完成来源接纳::已接纳) {
                    结果.状态 = 接纳 == 明确完成来源接纳::已停止
                        ? 新自我消息处理状态::停止取消 : 新自我消息处理状态::工程停止;
                    return 结果;
                }
            }
            结果.状态 = 本轮有K && (推进.状态 == 新任务贡献推进状态::已完成
                || 推进.状态 == 新任务贡献推进状态::精确重复 || 推进.状态 == 新任务贡献推进状态::无直接父任务)
                ? 新自我消息处理状态::任务结果已裁决 : 新自我消息处理状态::任务完成贡献待归集;
            const bool 是最终来源 = 本轮有K && (最终.交接->直属收尾.分类 == 新任务直属收尾分类::最终消费保留
                || 最终.交接->直属收尾.分类 == 新任务直属收尾分类::最终已收尾);
            if (是最终来源) {
                if (!最终.交接->完成结算节点 || !最终.交接->接收节点)
                    throw std::logic_error("最终来源缺少正式K/R");
                项.最终工作已交接阶段.store(true);
                项.贡献前置结果 = 结果;
                项.最终消费来源 = 新服务值最终来源定位{定位.任务节点,
                    *最终.交接->完成结算节点, *最终.交接->接收节点};
                return 推进最终消费消息(项);
            }
            return 结果;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"贡献治理消息", L"贡献交接或正式结果核验失败");
        }
    }

    std::optional<新自我消息处理结果> 处理任务完成贡献复核(消息项& 项) noexcept {
        新自我消息处理结果 结果; 结果.原消息 = 项.消息;
        if (!项.消息.完成贡献复核) { 结果.状态 = 新自我消息处理状态::入口拒绝; return 结果; }
        return 处理贡献定位(项, *项.消息.完成贡献复核, std::move(结果));
    }

    std::optional<新自我消息处理结果> 处理任务结果反馈(
        消息项& 项) noexcept {
        if (项.贡献前置结果) return 推进最终消费消息(项);
        新自我消息处理结果 结果;
        结果.原消息 = 项.消息;
        if (!任务管理 || !项.消息.结果反馈) {
            结果.状态 = 新自我消息处理状态::任务管理未就绪;
            return 结果;
        }
        const auto& 定位 = *项.消息.结果反馈;
        if (项.贡献工作已交接阶段.load()) {
            if (!项.需求认知已读取)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"反馈需求认知", L"贡献阶段丢失本次正式认知定位。");
            const auto 进度 = 读取本次需求认知(项, 结果, 定位);
            if (进度 == 需求认知读取进度::资源重试) return std::nullopt;
            if (进度 != 需求认知读取进度::可继续) return 结果;
            return 处理贡献定位(项, {定位.任务节点, 定位.执行实例节点, 定位.影响评估节点}, std::move(结果));
        }
        const auto 管理结果 = 任务管理->读取最近执行结果(定位.任务节点);
        if (!管理结果 || 管理结果->原工作 != 定位.原工作
            || 管理结果->状态
                != 新任务执行处理状态::已完成影响评估) {
            // 完成状态已经发布且结算已经建立时，manager可能已在本线程
            // 重启后释放运行期定位。正式结算足以收束原反馈，不要求恢复
            // 第二份消息账或重跑方法。
            const auto 已有结算 = 任务服务->读取任务完成结算(
                定位.任务节点);
            if (已有结算
                && 已有结算->执行实例节点 == 定位.执行实例节点
                && 已有结算->影响评估节点 == 定位.影响评估节点) {
                结果.任务完成结算 = 新任务完成结算结果{
                    新任务操作状态::精确重复, *已有结算};
                const auto 进度 = 读取本次需求认知(项, 结果, 定位);
                if (进度 == 需求认知读取进度::资源重试) return std::nullopt;
                if (进度 != 需求认知读取进度::可继续) return 结果;
                if (const auto 接纳 = 接纳明确完成来源(定位.任务节点);
                    接纳 != 明确完成来源接纳::已接纳) {
                    结果.状态 = 接纳 == 明确完成来源接纳::已停止
                        ? 新自我消息处理状态::停止取消 : 新自我消息处理状态::工程停止;
                    return 结果;
                }
                return 处理贡献定位(项, {定位.任务节点, 定位.执行实例节点, 定位.影响评估节点}, std::move(结果));
            }
            结果.状态 = 新自我消息处理状态::任务结果定位已变化;
            return 结果;
        }
        结果.执行独立读回 = 任务服务->读回任务执行(
            定位.任务节点, 定位.执行实例节点);
        if (结果.执行独立读回->状态
                == 新任务治理执行读回状态::资源失败) {
            return std::nullopt;
        }
        if (结果.执行独立读回->状态
                != 新任务治理执行读回状态::已读回影响评估
            || !结果.执行独立读回->影响评估
            || 结果.执行独立读回->影响评估->节点
                != 定位.影响评估节点) {
            结果.状态 = 新自我消息处理状态::任务结果定位已变化;
            return 结果;
        }
        const auto 认知进度 = 读取本次需求认知(项, 结果, 定位);
        if (认知进度 == 需求认知读取进度::资源重试) return std::nullopt;
        if (认知进度 != 需求认知读取进度::可继续) return 结果;
        发现方法问题学习(定位.任务节点,定位.执行实例节点);
        结果.任务需求当前复核 = 任务服务->复核任务当前需求(
            定位.任务节点);
        if (结果.任务需求当前复核->状态
                == 新需求当前满足状态::资源失败) {
            return std::nullopt;
        }

        新自我任务后继决议 决议;
        决议.任务节点 = 定位.任务节点;
        决议.执行实例节点 = 定位.执行实例节点;
        决议.影响评估节点 = 定位.影响评估节点;
        const auto& 评估 = *结果.执行独立读回->影响评估;
        switch (结果.任务需求当前复核->状态) {
        case 新需求当前满足状态::已满足:
            if (评估.允许形成完成决议()) {
                决议.类型 = 新自我任务后继类型::完成;
            } else {
                const auto 不通过 = 新任务影响验证状态::已验证不通过;
                const auto 冲突 = 新任务影响验证状态::冲突;
                const bool 已知异常 = !评估.计划外状态组.empty()
                    || 评估.多渠道验证 == 不通过
                    || 评估.多渠道验证 == 冲突
                    || 评估.禁止结果验证 == 不通过
                    || 评估.禁止结果验证 == 冲突
                    || 评估.副作用验证 == 不通过
                    || 评估.副作用验证 == 冲突
                    || 评估.归因验证 == 不通过
                    || 评估.归因验证 == 冲突;
                // 目标当前满足并不足以证明本次执行闭包。已有异常时失败；
                // 仅缺比较/归因提供者时重新筹办，不重复消费旧W3-A。
                决议.类型 = 已知异常
                    ? 新自我任务后继类型::失败
                    : 新自我任务后继类型::重新筹办;
            }
            break;
        case 新需求当前满足状态::需增加:
        case 新需求当前满足状态::需减少:
            // 目标仍未满足时必须重新筹办，与方法预测是否准确无关；
            // 不能因为“命中同类型状态”就重复原动作。
            决议.类型 = 新自我任务后继类型::重新筹办;
            break;
        case 新需求当前满足状态::目标特征尚未形成:
        case 新需求当前满足状态::当前值不存在:
        case 新需求当前满足状态::需求信息待完善:
        case 新需求当前满足状态::目标引用前提已变化:
        case 新需求当前满足状态::目标内容未支持:
        case 新需求当前满足状态::目标内容分支待定:
        case 新需求当前满足状态::目标内容依赖未就绪:
        case 新需求当前满足状态::目标内容定义不合法:
            决议.类型 = 新自我任务后继类型::等待;
            break;
        case 新需求当前满足状态::未满足但无有向差值:
        // CONCEPT-INFO-MEMBERSHIP R-1：认知不足不等待、不结束，重新筹办以按目标分解形成认知子需求。
        case 新需求当前满足状态::当前值认知不足:
            决议.类型 = 新自我任务后继类型::重新筹办;
            break;
        case 新需求当前满足状态::需要执行前后比较:
            // 方向目标没有可永久保存的单点满足状态。本次执行是否达成目标
            // 只由同一执行实例中冻结的动作前值A和观察后值B决定。
            if (评估.目标需求结果.状态 == 新需求当前满足状态::已满足
                && 评估.允许形成完成决议()) {
                决议.类型 = 新自我任务后继类型::完成;
            } else if (评估.目标需求结果.状态
                    == 新需求当前满足状态::需要执行前后比较) {
                决议.类型 = 新自我任务后继类型::等待;
            } else {
                决议.类型 = 新自我任务后继类型::重新筹办;
            }
            break;
        case 新需求当前满足状态::比较关系不适用:
        case 新需求当前满足状态::双根负差非法:
        case 新需求当前满足状态::入口拒绝:
        case 新需求当前满足状态::需求不存在:
        case 新需求当前满足状态::结构不一致:
        default:
            决议.类型 = 新自我任务后继类型::失败;
            break;
        }
        结果.任务后继决议 = 决议;
        结果.任务后继接收状态 =
            任务管理->提交自我任务后继决议(定位, 决议);
        switch (*结果.任务后继接收状态) {
        case 新自我任务后继接收状态::已发布并读回:
        case 新自我任务后继接收状态::精确重复:
            if (决议.类型 == 新自我任务后继类型::完成) {
                return 处理贡献定位(项, {定位.任务节点, 定位.执行实例节点, 定位.影响评估节点}, std::move(结果));
            }
            结果.状态 = 新自我消息处理状态::任务结果已裁决;
            return 结果;
        case 新自我任务后继接收状态::资源失败:
            return std::nullopt;
        case 新自我任务后继接收状态::定位已变化:
        case 新自我任务后继接收状态::任务状态已变化:
            结果.状态 = 新自我消息处理状态::任务结果定位已变化;
            return 结果;
        case 新自我任务后继接收状态::已停止接收:
            结果.状态 = 新自我消息处理状态::任务管理未就绪;
            return 结果;
        case 新自我任务后继接收状态::状态发布失败:
        case 新自我任务后继接收状态::入口拒绝:
            结果.状态 = 新自我消息处理状态::任务结果需核查;
            return 结果;
        case 新自我任务后继接收状态::内部错误:
        default:
            结果.状态 = 新自我消息处理状态::内部错误;
            return 结果;
        }
    }

    void 终结剩余消息(新自我消息处理状态 状态) {
        for (auto& [方法, 项] : 在途学习) {
            if (项.阶段 == 学习交接阶段::硬失败) continue;
            项.阶段 = 学习交接阶段::硬失败;
            项.异常 = 新学习运行异常记录{方法, 新学习运行异常原因::停止取消};
            最近学习运行异常 = 项.异常;
            // 保留原future；停止不伪造交付/确认成功。
        }
        for (const auto& 项 : 活动消息) {
            新自我消息处理结果 结果;
            结果.原消息 = 项->消息;
            结果.状态 = 状态;
            结果.行动请求采用=项->行动请求采用材料;
            结果.外部服务需求采用=项->外部服务采用材料;
            if (项->管理结果.valid()) {
                结果.状态 = 新自我消息处理状态::已交接未收回;
                结果.未收回任务结果 = 项->管理结果;
            }
            if (项->最终消费结果.valid()) {
                结果.状态 = 新自我消息处理状态::已交接未收回;
                结果.未收回最终消费工作 = 项->最终消费工作;
                结果.未收回最终消费结果 = 项->最终消费结果;
            }
            if (项->贡献治理结果.valid()) {
                结果.状态 = 新自我消息处理状态::已交接未收回;
                结果.未收回贡献治理工作 = 项->贡献治理工作;
                结果.未收回贡献治理结果 = 项->贡献治理结果;
            }
            if (项->直接贡献结果 && !项->直接贡献已确认) {
                if (!项->直接贡献结果->结果.任务贡献治理)
                    throw std::logic_error("停止交接的直接贡献材料缺失");
                结果.状态 = 新自我消息处理状态::已交接未收回;
                结果.未确认直接贡献治理 = 项->直接贡献结果->结果.任务贡献治理;
            }
            if (项->直接最终消费结果 && !项->直接最终消费已确认) {
                结果.状态 = 新自我消息处理状态::已交接未收回;
                结果.未确认直接最终消费 = 项->直接最终消费结果;
            }
            移交因果采用进度(*项, 结果);
            项->承诺.set_value(std::move(结果));
        }
        活动消息.clear();
    }

    void 运行() noexcept {
        try {
            std::unique_lock 锁(状态互斥);
            // 入口就绪不等待首次业务工作；消息可入队，但仍在首次准备后处理。
            线程已进入 = true;
            生命周期 = 停止已请求
                ? 新自我线程生命周期::正在停止
                : 新自我线程生命周期::启动中;
            状态变化.notify_all();
            std::optional<std::chrono::steady_clock::time_point> 首次重试;
            if (!停止已请求) {
                // 跨owner的复核不能持状态互斥，启动方须能及时取得入口见证。
                锁.unlock();
                const auto 首次结果 = 复核需求();
                锁.lock();
                首次需求复核 = 首次结果;
                if (!停止已请求) {
                    安排自动承接(首次结果);
                    (void)提交需求工作已加锁(新自我工作种类::需求树全局扫描);
                    if (消息配置 && 首次结果.状态 == 新自我需求复核状态::资源失败) {
                        首次重试 = std::chrono::steady_clock::now()
                            + 有界等待时长(消息配置->资源重试间隔毫秒);
                    }
                }
            }
            生命周期 = 停止已请求
                ? 新自我线程生命周期::正在停止
                : 新自我线程生命周期::等待治理消息;
            状态变化.notify_all();
            const auto 等待推进 = [&](std::optional<std::chrono::steady_clock::time_point> 时点 = std::nullopt) {
                if (有关联变化通知() || 最终服务工作待唤醒.load() || 接收认知待唤醒 || 外设交互场景待补建) return;
                if (需求扫描阶段 == 新自我需求扫描阶段::等待扫描结果
                    && 扫描工作结果.valid()
                    && 扫描工作结果.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) return;
                if (首次重试 && (!时点 || *首次重试 < *时点)) 时点 = 首次重试;
                if(接收认知) {
                    const auto next=最近接收认知推进+std::chrono::milliseconds(500);
                    if(!时点||next<*时点)时点=next;
                }
                时点 = 合并后台推进时点已加锁(时点);
                生命周期 = 新自我线程生命周期::等待治理消息;
                if (时点) 状态变化.wait_until(锁, *时点);
                else 状态变化.wait(锁, [this] {
                    return 停止已请求 || 有关联变化通知() || 最终服务工作待唤醒.load() || 接收认知待唤醒
                        || 外设交互场景待补建
                        || 有未停放消息已加锁()
                        || (!待转交事实变化.empty() && 普通消息数量已加锁() < 消息配置->邮箱容量)
                        || 有学习结果已就绪()
                        || (需求扫描阶段 == 新自我需求扫描阶段::等待扫描结果
                            && 扫描工作结果.valid()
                            && 扫描工作结果.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
                        || std::any_of(自动承接.begin(), 自动承接.end(),
                            [](const auto& 项) { return 项.has_value(); });
                });
            };
            bool 扫描需要让出 = false;
            while (!停止已请求 && !停止源.stop_requested()) {
                if (外设交互场景待补建) {
                    // INTERACT-SCENE-INSTINCT（2026-11-09用户裁决）：现实交互场景与交互者对应存在改由获取服务对象
                    // 本能（内置监听）按需求建立；自我线程不再自动补建，外设通知只作唤醒。
                    外设交互场景待补建 = false;
                    continue;
                }
                if(接收认知&&(接收认知待唤醒||std::chrono::steady_clock::now()>=最近接收认知推进+std::chrono::milliseconds(500))) {
                    // 清标志在扫描前，锁外调用后不覆盖其间新通知。
                    const bool 由唤醒=接收认知待唤醒;
                    接收认知待唤醒=false;
                    auto* provider=接收认知;const auto token=停止源.get_token();
                    bool 行动请求已入邮箱=false;
                    锁.unlock();
                    try {
                        try {
                            std::wostringstream 诊断;
                            诊断<<L"由唤醒="<<(由唤醒 ? 1 : 0);
                            (void)调试日志<调试切片::RECEIVED_DEMAND_SCHED>(L"自我线程",L"推进接收需求认知入口",诊断.str());
                        } catch (...) {}
                        const auto observed=provider->推进一次(token);
                        if(!token.stop_requested()&&observed&&observed->待交接前置输入) {
                            const auto accepted=接纳前置输入(*observed->待交接前置输入);
                            if(accepted.状态==新自我消息接收状态::已接收||accepted.状态==新自我消息接收状态::精确重复) {
                                if(!accepted.处理结果.valid())throw std::logic_error("input handoff future missing");
                                const auto ack=provider->确认前置输入已接纳(*observed->待交接前置输入);
                                if(ack==接收行动请求交接确认状态::已确认||ack==接收行动请求交接确认状态::精确重复)行动请求已入邮箱=true;
                                // 来源冲突=同一采用含义来源已变而旧消息仍在邮箱：不确认交接，下轮重交。
                                else if(ack!=接收行动请求交接确认状态::资源失败&&ack!=接收行动请求交接确认状态::输入拒绝
                                    &&ack!=接收行动请求交接确认状态::来源冲突)
                                    throw std::logic_error("input handoff acknowledgement conflict");
                            } else if(accepted.状态!=新自我消息接收状态::邮箱已满&&accepted.状态!=新自我消息接收状态::身份冲突
                                &&accepted.状态!=新自我消息接收状态::资源失败
                                &&accepted.状态!=新自我消息接收状态::尚未启动&&accepted.状态!=新自我消息接收状态::尚未配置
                                &&accepted.状态!=新自我消息接收状态::已停止接收)throw std::logic_error("input handoff mailbox conflict");
                        }
                        if(!token.stop_requested()&&observed
                            &&observed->阶段==接收需求认知阶段::已读回行动请求&&observed->待交接请求) {
                            const auto accepted=接纳行动请求(*observed->待交接请求);
                            if(accepted.状态==新自我消息接收状态::已接收
                                ||accepted.状态==新自我消息接收状态::精确重复) {
                                if(!accepted.处理结果.valid())throw std::logic_error("action request self mailbox future missing");
                                if(!token.stop_requested()) {
                                    const auto ack=provider->确认行动请求已交接(*observed->待交接请求);
                                    // 输入拒绝/来源冲突：同源旧消息仍在邮箱，不确认交接，provider下轮重交。
                                    if(ack!=接收行动请求交接确认状态::已确认
                                        &&ack!=接收行动请求交接确认状态::精确重复
                                        &&ack!=接收行动请求交接确认状态::资源失败
                                        &&ack!=接收行动请求交接确认状态::输入拒绝
                                        &&ack!=接收行动请求交接确认状态::来源冲突)
                                        throw std::logic_error("action request cognition handoff acknowledgement mismatch");
                                    // 确认只证明邮箱接纳；回入口优先排干行动请求承接，再建Ds/T。
                                    if(ack==接收行动请求交接确认状态::已确认
                                        ||ack==接收行动请求交接确认状态::精确重复)
                                        行动请求已入邮箱=true;
                                }
                            } else if(accepted.状态!=新自我消息接收状态::邮箱已满
                                &&accepted.状态!=新自我消息接收状态::身份冲突
                                &&accepted.状态!=新自我消息接收状态::资源失败
                                &&accepted.状态!=新自我消息接收状态::尚未启动
                                &&accepted.状态!=新自我消息接收状态::尚未配置
                                &&accepted.状态!=新自我消息接收状态::已停止接收)
                                throw std::logic_error("formal action request mailbox rejected source");
                        }
                    }
                    catch(const std::exception& e){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"推进接收需求认知",L"真实消息认知结构或内部不变量错误，不当作业务缺项重试。",e.what());}
                    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"推进接收需求认知",L"真实消息认知发生未知工程异常。");}
                    锁.lock();
                    最近接收认知推进=std::chrono::steady_clock::now();
                    if(停止已请求||停止源.stop_requested())break;
                    // 普通认知拍仍继续原治理；前U材料或行动请求入邮箱后优先排干。
                    if(行动请求已入邮箱)continue;
                }
                if (最终服务工作待唤醒.exchange(false)) {
                    const auto 当前 = std::chrono::steady_clock::now();
                    for (const auto& 项 : 活动消息)
                        if (项->最终消费来源 || 项->贡献工作已交接阶段.load()) 项->可处理时点 = 当前;
                }
                if (关联检查桥 && 关联检查桥->待复核.exchange(false))
                    请求关联复核已加锁(true);
                if (待记录扫描诊断) {
                    const auto 诊断 = *待记录扫描诊断;
                    const auto 报告 = 最近已消费检查结果;
                    待记录扫描诊断.reset();
                    锁.unlock();
                    记录扫描诊断(诊断, 报告);
                    锁.lock();
                    continue;
                }
                if (!扫描需要让出 && 扫描恢复 == 扫描恢复方式::资源延后 && 扫描重试时点
                    && 待恢复工作种类 && *扫描重试时点 <= std::chrono::steady_clock::now()) {
                    const auto 种类 = *待恢复工作种类;
                    (void)提交需求工作已加锁(种类);
                    扫描需要让出 = true;
                    continue;
                }
                const auto 学习位置=std::find_if(在途学习.begin(),在途学习.end(),[](const auto& 项) {
                    const auto& 占用 = 项.second;
                    if (占用.阶段 == 学习交接阶段::硬失败) return false;
                    if (占用.阶段 != 学习交接阶段::等待结果)
                        return 占用.可处理时点 <= std::chrono::steady_clock::now();
                    return 占用.处理结果.valid()
                        && 占用.处理结果.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
                });
                if (学习位置!=在途学习.end()) {
                    const auto 方法节点=学习位置->first;
                    auto 占用 = 学习位置->second;
                    锁.unlock();
                    const bool 已回收 = 推进学习交接(方法节点, 占用);
                    锁.lock();
                    if (占用.通知) 最近学习结束 = 占用.通知;
                    if (占用.异常) 最近学习运行异常 = 占用.异常;
                    if (已回收 && 占用.待处理请求组.empty()) 在途学习.erase(学习位置);
                    else {
                        if (已回收) {
                            占用.处理结果 = {};
                            占用.当前请求.reset();
                            占用.请求者组.clear();
                            占用.通知.reset();
                            占用.异常.reset();
                            占用.需重学 = false;
                            占用.定向结束已消费 = false;
                            占用.阶段 = 学习交接阶段::待提交;
                        }
                        学习位置->second = std::move(占用);
                    }
                    continue;
                }
                const auto 问题位置=std::find_if(问题学习组.begin(),问题学习组.end(),[](const auto& 项) {
                    if (项.second.已消费) return false;
                    return 项.second.处理结果.valid()
                        ? 项.second.处理结果.wait_for(std::chrono::milliseconds(0))==std::future_status::ready
                        : 项.second.可投递时点<=std::chrono::steady_clock::now();
                });
                if (问题位置!=问题学习组.end()) {
                    auto& 项=问题位置->second;
                    const auto 消息=项.消息;
                    if (!项.处理结果.valid()) {
                        锁.unlock();
                        const auto 接收=学习线程->提交方法问题学习(消息);
                        锁.lock();
                        if ((接收.状态==新学习操作状态::已完成 || 接收.状态==新学习操作状态::精确重复)
                            && 接收.处理结果.valid()) 项.处理结果=接收.处理结果;
                        else if (接收.状态==新学习操作状态::资源失败)
                            项.可投递时点=std::chrono::steady_clock::now()+有界等待时长(消息配置->资源重试间隔毫秒);
                        else {
                            项.已消费=true;
                            const 新学习运行异常记录 异常{消息.方法节点,新学习运行异常原因::内部错误};
                            最近学习运行异常=异常;
                            锁.unlock(); 记录自我学习运行异常(异常); 锁.lock();
                        }
                    } else {
                        const auto future=项.处理结果;
                        项.已消费=true;
                        锁.unlock();
                        std::optional<新学习运行异常记录> 异常;
                        try {
                            if (future.get().方法节点!=消息.方法节点)
                                异常=新学习运行异常记录{消息.方法节点,新学习运行异常原因::结构不一致};
                        } catch (const 新学习运行异常& e) {
                            异常=e.记录();
                            if (异常->方法节点!=消息.方法节点)
                                异常=新学习运行异常记录{消息.方法节点,新学习运行异常原因::结构不一致};
                        }
                        catch (const std::bad_alloc&) { 异常=新学习运行异常记录{消息.方法节点,新学习运行异常原因::资源不足}; }
                        catch (...) { 异常=新学习运行异常记录{消息.方法节点,新学习运行异常原因::内部错误}; }
                        const auto 确认=学习线程->确认方法问题结果已接收(消息.执行实例节点);
                        if (确认!=新学习操作状态::已完成 && 确认!=新学习操作状态::无变化)
                            异常=新学习运行异常记录{消息.方法节点,新学习运行异常原因::内部错误};
                        if (异常) 记录自我学习运行异常(*异常);
                        else 记录方法问题接收结束(消息);
                        // METHOD-LEARN-STATE：尝试配对已发布时由方法owner形成方法状态并唤醒等待任务，不再交接manager。
                        const bool 待继续 = 异常 && (异常->原因==新学习运行异常原因::资源不足
                            || 异常->原因==新学习运行异常原因::发布结果未知);
                        const auto 收集 = 异常 ? std::nullopt : 学习线程->读取方法问题收集结果(消息.执行实例节点);
                        const bool 已发布 = 收集 && 收集->处理状态==新方法问题处理状态::已发布;
                        if (已发布) 记录自我学习回流(新学习结束通知{消息.方法节点});
                        锁.lock();
                        if (异常) 最近学习运行异常=异常;
                        if (已发布) 最近学习结束=新学习结束通知{消息.方法节点};
                        项.处理结果={};
                        if (待继续) {
                            项.已消费=false;
                            项.可投递时点=std::chrono::steady_clock::now()+有界等待时长(消息配置->资源重试间隔毫秒);
                        }
                    }
                    continue;
                }
                if (!扫描需要让出 && 需求扫描阶段 == 新自我需求扫描阶段::等待扫描结果
                    && 扫描工作结果.valid()
                    && 扫描工作结果.wait_for(std::chrono::milliseconds(0))
                        == std::future_status::ready) {
                    消费扫描结果已加锁();
                    扫描需要让出 = true;
                    continue;
                }
                if (!扫描需要让出 && 需求扫描阶段 == 新自我需求扫描阶段::等待任务承接
                    && !扫描承接.empty()) {
                    const auto 位置 = std::min_element(扫描承接.begin(), 扫描承接.end(), [](const auto& 左, const auto& 右) {
                        if (左.等待恢复事件 != 右.等待恢复事件) return !左.等待恢复事件;
                        return 左.可处理时点 < 右.可处理时点;
                    });
                    if (位置 != 扫描承接.end() && !位置->等待恢复事件
                        && 位置->可处理时点 <= std::chrono::steady_clock::now()) {
                        auto& 项 = *位置;
                        生命周期 = 新自我线程生命周期::正在处理消息;
                        锁.unlock();
                        const auto 推进 = 处理扫描承接(项);
                        锁.lock();
                        if (推进.来源 && (项.最近扫描失败来源 != 推进.来源
                                || 项.最近扫描失败状态 != 推进.状态
                                || 项.最近扫描失败推进 != static_cast<std::uint8_t>(推进.推进))) {
                            项.最近扫描失败来源 = 推进.来源;
                            项.最近扫描失败状态 = 推进.状态;
                            项.最近扫描失败推进 = static_cast<std::uint8_t>(推进.推进);
                            登记扫描诊断已加锁(推进.来源, 推进.状态,
                                推进.推进 == 扫描承接推进::等待 ? 扫描恢复方式::资源延后
                                    : 推进.推进 == 扫描承接推进::等待事件 ? 扫描恢复方式::等待事件
                                    : 扫描恢复方式::硬失败,
                                *当前扫描请求, &项.消息);
                        }
                        if (推进.推进 == 扫描承接推进::等待) {
                            项.可处理时点 = std::chrono::steady_clock::now()
                                + 有界等待时长(消息配置->资源重试间隔毫秒);
                        } else if (推进.推进 == 扫描承接推进::等待事件) {
                            项.管理结果 = {};
                            项.等待恢复事件 = true;
                        } else {
                            if (推进.推进 == 扫描承接推进::失败) 扫描承接失败 = true;
                            扫描承接.erase(位置);
                        }
                        if (扫描承接.empty()) 完成扫描承接已加锁();
                        扫描需要让出 = true;
                        continue;
                    }
                }
                // 即使扫描重试间隔很短，每次推进后也给治理消息一次处理机会。
                扫描需要让出 = false;
                转入待转交事实变化已加锁();
                if (首次重试 && *首次重试 <= std::chrono::steady_clock::now()) {
                    锁.unlock();
                    const auto 重试结果 = 复核需求();
                    锁.lock();
                    首次需求复核 = 重试结果;
                    安排自动承接(重试结果);
                    if (重试结果.状态 == 新自我需求复核状态::资源失败) {
                        首次重试 = std::chrono::steady_clock::now()
                            + 有界等待时长(消息配置->资源重试间隔毫秒);
                    } else {
                        首次重试.reset();
                    }
                    continue;
                }
                const bool 有自动承接 = std::any_of(
                    自动承接.begin(), 自动承接.end(),
                    [](const auto& 项) { return 项.has_value(); });
                if (!有未停放消息已加锁() && !有自动承接) {
                    等待推进();
                    continue;
                }
                auto 自动位置 = std::min_element(
                    自动承接.begin(), 自动承接.end(),
                    [](const auto& 左, const auto& 右) {
                        if (!左) return false;
                        if (!右) return true;
                        return 左->可处理时点 < 右->可处理时点;
                    });
                if (自动位置 != 自动承接.end() && *自动位置) {
                    auto& 自动项 = **自动位置;
                    // 邮箱中已就绪的行动请求承接优先于自动承接扫描，避免交接后被长自动路径饿死。
                    const bool 有就绪行动请求 = std::any_of(活动消息.begin(), 活动消息.end(),
                        [](const auto& m) {
                            return (m->消息.类型 == 新自我消息类型::行动请求承接||m->消息.类型==新自我消息类型::前置语言输入材料)
                                && m->可处理时点 <= std::chrono::steady_clock::now();
                        });
                    if (有就绪行动请求) {
                        // fall through to 活动消息 prefer path
                    } else if (自动项.可处理时点 <= std::chrono::steady_clock::now()) {
                        生命周期 = 新自我线程生命周期::正在处理消息;
                        锁.unlock();
                        const bool 已终结 = 处理自动承接(自动项);
                        锁.lock();
                        if (已终结) {
                            自动位置->reset();
                        } else {
                            自动项.可处理时点 = std::chrono::steady_clock::now()
                                + 有界等待时长(消息配置->资源重试间隔毫秒);
                        }
                        continue;
                    }
                    if (!有未停放消息已加锁()) {
                        等待推进(自动项.可处理时点);
                        continue;
                    }
                }
                // 已就绪行动请求承接优先于更早入队的筹办反馈：阶段13交接后先建Ds/T（§5 确认≠已建Ds/T）。
                const auto 选信时点 = std::chrono::steady_clock::now();
                const auto 最早 = std::min_element(活动消息.begin(), 活动消息.end(),
                    [&](const auto& 左, const auto& 右) {
                        if (左->因果能力已停放 != 右->因果能力已停放) return !左->因果能力已停放;
                        const bool 左就绪 = 左->可处理时点 <= 选信时点;
                        const bool 右就绪 = 右->可处理时点 <= 选信时点;
                        const bool 左请求 = 左就绪
                            && (左->消息.类型 == 新自我消息类型::行动请求承接||左->消息.类型==新自我消息类型::前置语言输入材料);
                        const bool 右请求 = 右就绪
                            && (右->消息.类型 == 新自我消息类型::行动请求承接||右->消息.类型==新自我消息类型::前置语言输入材料);
                        if (左请求 != 右请求) return 左请求;
                        return 左->可处理时点 < 右->可处理时点;
                    });
                if (最早 == 活动消息.end() || (*最早)->因果能力已停放) {
                    等待推进();
                    continue;
                }
                const auto 项 = *最早;
                if (项->可处理时点 > std::chrono::steady_clock::now()) {
                    生命周期 = 新自我线程生命周期::等待治理消息;
                    // 新消息、停止或时点到达都重新检查条件；通知不代表依赖成功。
                    auto 唤醒时点 = 首次重试
                        ? (std::min)(*首次重试, 项->可处理时点)
                        : 项->可处理时点;
                    if (自动位置 != 自动承接.end() && *自动位置) {
                        唤醒时点 = (std::min)(唤醒时点, (*自动位置)->可处理时点);
                    }
                    等待推进(唤醒时点);
                    continue;
                }
                生命周期 = 新自我线程生命周期::正在处理消息;
                锁.unlock();
                std::optional<新自我消息处理结果> 单次结果;
                if (项->消息.类型 == 新自我消息类型::需求承接)
                    单次结果 = 处理任务承接(*项);
                else if (项->消息.类型 == 新自我消息类型::任务筹办反馈)
                    单次结果 = 处理筹办反馈(*项);
                else if (项->消息.类型 == 新自我消息类型::任务结果反馈)
                    单次结果 = 处理任务结果反馈(*项);
                else if (项->消息.类型 == 新自我消息类型::任务完成贡献复核)
                    单次结果 = 处理任务完成贡献复核(*项);
                else if (项->消息.类型 == 新自我消息类型::完整服务需求材料)
                    单次结果 = 处理完整服务需求材料(*项);
                else if (项->消息.类型 == 新自我消息类型::行动请求承接)
                    单次结果 = 处理行动请求承接(*项);
                else if(项->消息.类型==新自我消息类型::前置语言输入材料)
                    单次结果=处理前置语言输入材料(*项);
                else if (项->消息.类型 == 新自我消息类型::等待任务复核)
                    单次结果 = 处理等待复核(*项);
                else {
                    单次结果 = 处理消息(项->消息);
                    // DEP-READY-WAKE B：就绪通知接纳资源失败不丢弃，按资源重试间隔重投。
                    if (单次结果->结果状态接收状态 == 新任务结果状态接收状态::资源失败) 单次结果.reset();
                }
                if (服务结算确认端口) 服务结算确认端口->报告待处理工程异常();
                锁.lock();
                if (!单次结果) {
                    if (项->因果能力已停放) continue;
                    if (项->因果能力复核登记 && 项->因果能力期间事件) {
                        项->可处理时点 = std::chrono::steady_clock::now();
                        continue;
                    }
                    项->可处理时点 = std::chrono::steady_clock::now()
                        + 有界等待时长(消息配置->资源重试间隔毫秒);
                    continue;
                }
                auto& 结果 = *单次结果;
                if (结果.状态 == 新自我消息处理状态::工程停止) {
                    停止源.request_stop(); 停止已请求 = true;
                }
                // request_stop先于请求方取得状态锁；本线程重新持锁后也须锁存
                // 已观察到的停止，不能趁请求方尚未写bool继续调度下一消息。
                if (停止源.stop_requested()) 停止已请求 = true;
                if (结果.需求复核) 安排自动承接(*结果.需求复核);
                // 只有已核验并处理的真实治理消息允许重新探测依赖；学习回流、
                // 轮询、拒绝/结构异常和停止不解除扫描等待，更不解除硬失败。
                const bool 有效治理事件 = 结果.原消息 == 项->消息
                    && 结果.管理接收状态 != 新任务承接接收状态::已停止接收
                    && (结果.状态 == 新自我消息处理状态::已复核
                        || 结果.状态 == 新自我消息处理状态::等待材料
                        || 结果.状态 == 新自我消息处理状态::任务已承接
                        || 结果.状态 == 新自我消息处理状态::既有终结任务
                        || 结果.状态 == 新自我消息处理状态::任务管理未就绪
                        || 结果.状态 == 新自我消息处理状态::任务筹办已裁决
                        || 结果.状态 == 新自我消息处理状态::任务结果已裁决);
                if (结果.需求复核
                    && 结果.需求复核->状态 == 新自我需求复核状态::资源失败
                    && !停止已请求) {
                    项->可处理时点 = std::chrono::steady_clock::now()
                        + 有界等待时长(消息配置->资源重试间隔毫秒);
                    continue;
                }
                if (!停止源.stop_requested() && 结果.状态 != 新自我消息处理状态::停止取消)
                    请求关联复核已加锁(有效治理事件);
                if (停止已请求 && 结果.需求复核
                    && 结果.需求复核->状态 == 新自我需求复核状态::资源失败) {
                    结果.状态 = 新自我消息处理状态::停止取消;
                }
                if (项->贡献复核再次唤醒 && !停止已请求) {
                    项->贡献复核再次唤醒 = false;
                    项->可处理时点 = std::chrono::steady_clock::now();
                    continue; // 最后一次处理出队时才兑现promise，绝不重复set_value。
                }
                项->承诺.set_value(std::move(结果));
                活动消息.erase(std::find(活动消息.begin(), 活动消息.end(), 项));
                生命周期 = 新自我线程生命周期::等待治理消息;
            }
            生命周期 = 新自我线程生命周期::正在停止;
            停止源.request_stop();
            if(接收认知) {
                auto* provider=接收认知;const auto token=停止源.get_token();
                锁.unlock();(void)provider->推进一次(token);锁.lock();
            }
            if (学习线程) (void)学习线程->请求停止();
            扫描承接.clear();
            扫描工作结果 = {};
            当前扫描请求.reset();
            扫描重试时点.reset();
            待恢复工作种类.reset();
            需求扫描阶段 = 新自我需求扫描阶段::已停止;
            for (auto& 项 : 自动承接) 项.reset();
            终结剩余消息(新自我消息处理状态::停止取消);
            生命周期 = 新自我线程生命周期::已停止;
            线程已完成 = true;
            状态变化.notify_all();
        } catch (...) {
            停止源.request_stop();
            try {
                std::lock_guard 锁(状态互斥);
                生命周期 = 新自我线程生命周期::内部错误;
                if (自我工作线程) (void)自我工作线程->请求停止();
                if (学习线程) (void)学习线程->请求停止();
                // 若交付本身异常，清除promise会唤醒接收者并报告broken_promise，
                // 不能将失败留成永远等待，也不伪造成功。
                try { 终结剩余消息(新自我消息处理状态::内部错误); }
                catch (...) { 活动消息.clear(); }
                线程已完成 = true;
                状态变化.notify_all();
            } catch (...) {
            }
        }
    }
};

新_自我线程类::新_自我线程类(
    新_世界树类& 世界树服务,
    新_需求类& 需求服务,
    新_方法类& 方法服务,
    新_任务类& 任务服务) noexcept
    : 实现_(new (std::nothrow) 实现(
          世界树服务, 需求服务, 方法服务, 任务服务)) {}

新_自我线程类::~新_自我线程类() noexcept {
    if (!实现_) return;
    (void)请求停止();
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        if (实现_->工作线程.joinable()) {
            if (实现_->工作线程.get_id() == std::this_thread::get_id()) {
                std::terminate();
            }
            实现_->工作线程.join();
        }
    } catch (...) {
        std::terminate();
    }
    实现_->解除关联检查通知();
    // 工作完成回调直接引用self；必须在self互斥和实现销毁前join。
    实现_->自我工作线程.reset();
    // 学习回调可能仍在使用反馈桥；先停止并join，才拆桥和依赖。
    std::unique_ptr<新_学习线程类> 待回收学习线程;
    try {
        std::lock_guard 锁(实现_->状态互斥);
        待回收学习线程 = std::move(实现_->学习线程);
    } catch (...) { std::terminate(); }
    // 学习完成通知会取得self锁，故成员移出后必须在锁外join。
    待回收学习线程.reset();
    try {
        const auto 桥 = 实现_->反馈桥;
        const auto 管理 = 实现_->任务管理;
        if (桥) {
            {
                std::lock_guard 锁(桥->互斥);
                桥->目标 = nullptr;
            }
            if (管理) 管理->解除自我筹办反馈(桥.get());
        }
    } catch (...) {
    }
    try {
        std::lock_guard 所有者锁(唯一自我互斥);
        if (唯一自我线程所有者 == this) 唯一自我线程所有者 = nullptr;
    } catch (...) {
    }
}

新自我治理基础结果 新_自我线程类::建立或取得治理基础() noexcept {
    if (!实现_) {
        新自我治理基础结果 结果;
        结果.状态 = 新自我治理基础状态::资源失败;
        return 结果;
    }

    std::lock_guard 锁(实现_->治理基础互斥);
    auto& 结果 = 实现_->治理基础缓存;
    try {
        if (!实现_->世界树服务->已初始化()) {
            结果.状态 = 新自我治理基础状态::世界树未初始化;
            return 结果;
        }
        const auto 世界事实 = 实现_->世界树服务->获取事实服务引用();
        auto& 特征值服务 = 世界事实.特征值服务;
        auto& 特征概念服务 = 世界事实.特征概念服务;
        auto& 二次特征概念服务 = 世界事实.二次特征概念服务;

        constexpr auto 最大值 = (std::numeric_limits<std::int64_t>::max)();
        const 特征概念定义 完整域定义{
            特征概念材料物理类型::I64标量,
            std::vector<特征概念I64闭区间>{{0, 最大值}},
            std::nullopt,
            特征概念比较规则::I64数值,
            特征概念聚合规则::I64连续值归组,
            {}};
        const 特征概念定义 目标定义{
            特征概念材料物理类型::I64标量,
            std::vector<特征概念I64闭区间>{{最大值, 最大值}},
            std::nullopt,
            特征概念比较规则::I64数值,
            特征概念聚合规则::I64连续值归组,
            {}};

        bool 本次建立 = false;
        const auto 定义一致 = [](const 特征概念信息& 信息,
            const 特征概念定义& 定义) noexcept {
            return 信息.材料类型 == 定义.材料类型
                && 信息.值域 == 定义.值域
                && 信息.单位 == 定义.单位
                && 信息.比较规则 == 定义.比较规则
                && 信息.聚合规则 == 定义.聚合规则
                && 信息.名称关系 == 定义.名称关系;
        };
        auto 服务完整域定义 = 完整域定义;
        服务完整域定义.值域 = std::vector<特征概念I64闭区间>{{
            (std::numeric_limits<std::int64_t>::min)(), 最大值}};
        const auto 确保根 = [&](std::optional<稳定编码>& 位置,
            const 特征概念定义& 预期定义, bool 允许扩展) {
            if (位置) {
                auto 信息 = 特征概念服务.获取特征概念(*位置);
                const auto 根核验 = 特征概念服务.比较单特征概念(*位置, *位置);
                if (根核验.状态 != 特征概念规则状态::已完成
                    || 根核验.特征类型根 != *位置) return false;
                if (允许扩展 && 信息 && 定义一致(*信息, 完整域定义)) {
                    if (!特征概念服务.更新特征概念值域(*位置, 预期定义.值域, 特征值服务))
                        return false;
                    信息 = 特征概念服务.获取特征概念(*位置);
                }
                const auto 关系 = 特征概念服务.比较单特征概念(*位置, *位置);
                return 信息 && 定义一致(*信息, 预期定义)
                    && 关系.状态 == 特征概念规则状态::已完成
                    && 关系.特征类型根 && *关系.特征类型根 == *位置;
            }
            const auto 节点 = 特征概念服务.建立或取得特征概念(
                预期定义, 特征值服务);
            if (!有效(节点)) return false;
            位置 = 节点;
            本次建立 = true;
            return true;
        };
        if (!确保根(结果.安全值特征类型根, 完整域定义, false)
            || !确保根(结果.服务值特征类型根, 服务完整域定义, true)
            || *结果.安全值特征类型根 == *结果.服务值特征类型根) {
            结果.状态 = 新自我治理基础状态::特征类型建立失败;
            return 结果;
        }

        const auto 确保目标 = [&](std::optional<稳定编码>& 位置,
            稳定编码 类型根) {
            if (位置) {
                const auto 信息 = 特征概念服务.获取特征概念(*位置);
                const auto 关系 = 特征概念服务.比较单特征概念(*位置, 类型根);
                return 信息 && 定义一致(*信息, 目标定义)
                    && 关系.状态 == 特征概念规则状态::已完成
                    && 关系.关系 == 单特征概念关系::右为上位
                    && 关系.特征类型根 && *关系.特征类型根 == 类型根;
            }
            const auto 节点 = 特征概念服务.建立或取得特征概念(
                目标定义, 特征值服务, 类型根);
            if (!有效(节点)) return false;
            位置 = 节点;
            本次建立 = true;
            return true;
        };
        if (!确保目标(结果.安全值目标概念,
                *结果.安全值特征类型根)
            || !确保目标(结果.服务值目标概念,
                *结果.服务值特征类型根)) {
            结果.状态 = 新自我治理基础状态::目标值域建立失败;
            return 结果;
        }

        const auto 确保比较 = [&](std::optional<稳定编码>& 类型位置,
            std::optional<稳定编码>& 关系位置,
            稳定编码 特征类型根, 稳定编码 目标概念) {
            const std::vector<二次特征概念输入定义> 输入组{
                {0, 二次特征输入角色::左输入,
                    二次特征输入来源种类::特征准确值, 特征类型根},
                {1, 二次特征输入角色::右输入,
                    二次特征输入来源种类::特征准确值, 特征类型根}};
            if (类型位置) {
                const auto 信息 = 二次特征概念服务.获取二次特征概念(
                    *类型位置);
                if (!信息 || 信息->二次特征类型根节点 != *类型位置
                    || 信息->算法 != 二次特征算法::有向特征差值
                    || 信息->输入定义组 != 输入组
                    || 信息->参照特征概念节点 != 目标概念) {
                    return false;
                }
            } else {
                const auto 建立 =
                    二次特征概念服务.建立或取得默认二次特征概念(
                        二次特征算法::有向特征差值, 输入组, 目标概念);
                if ((建立.状态 != 二次特征概念操作状态::已建立
                        && 建立.状态 != 二次特征概念操作状态::已复用)
                    || !建立.概念节点) {
                    return false;
                }
                类型位置 = 建立.概念节点;
                本次建立 = 本次建立
                    || 建立.状态 == 二次特征概念操作状态::已建立;
            }

            if (关系位置) {
                const auto 信息 = 二次特征概念服务.获取二次特征概念(
                    *关系位置);
                return 信息
                    && 信息->二次特征类型根节点 == *类型位置
                    && 信息->算法 == 二次特征算法::有向特征差值
                    && 信息->输入定义组 == 输入组
                    && 信息->参照特征概念节点 == 目标概念
                    && std::find(信息->结果域种类组.begin(),
                        信息->结果域种类组.end(),
                        二次特征结果域种类::包含于参照值域)
                        != 信息->结果域种类组.end();
            }
            const auto 建立 =
                二次特征概念服务.建立或取得参照结果域概念(
                    *类型位置, 二次特征结果域种类::包含于参照值域);
            if ((建立.状态 != 二次特征概念操作状态::已建立
                    && 建立.状态 != 二次特征概念操作状态::已复用)
                || !建立.概念节点) {
                return false;
            }
            关系位置 = 建立.概念节点;
            本次建立 = 本次建立
                || 建立.状态 == 二次特征概念操作状态::已建立;
            return true;
        };
        if (!确保比较(结果.安全值比较类型根,
                结果.安全值目标比较关系,
                *结果.安全值特征类型根, *结果.安全值目标概念)
            || !确保比较(结果.服务值比较类型根,
                结果.服务值目标比较关系,
                *结果.服务值特征类型根, *结果.服务值目标概念)) {
            结果.状态 = 新自我治理基础状态::比较关系建立失败;
            return 结果;
        }
        if (结果.安全值目标比较关系
                == 结果.服务值目标比较关系
            || 结果.安全值比较类型根 == 结果.服务值比较类型根) {
            结果.状态 = 新自我治理基础状态::结构不一致;
            return 结果;
        }
        结果.状态 = 本次建立
            ? 新自我治理基础状态::已建立
            : 新自我治理基础状态::已复用;
        return 结果;
    } catch (...) {
        结果.状态 = 新自我治理基础状态::资源失败;
        return 结果;
    }
}

新自我线程初始化结果 新_自我线程类::初始化(
    const 新自我线程初始化请求& 请求) noexcept {
    新自我线程初始化结果 结果;
    if (!实现_) {
        结果.状态 = 新自我线程操作状态::资源失败;
        return 结果;
    }
    if (!请求.完整()) {
        结果.状态 = 新自我线程操作状态::初始化请求不完整;
        return 结果;
    }

    try {
        std::lock_guard 所有者锁(唯一自我互斥);
        if (唯一自我线程所有者 && 唯一自我线程所有者 != this) {
            结果.状态 = 新自我线程操作状态::唯一自我冲突;
            return 结果;
        }
        {
            std::lock_guard 状态锁(实现_->状态互斥);
            if (实现_->线程已进入 || 实现_->工作线程.joinable()) {
                结果.状态 = 新自我线程操作状态::已在运行;
                return 结果;
            }
            if (实现_->锁定初始化请求
                && *实现_->锁定初始化请求 != 请求) {
                结果.状态 = 新自我线程操作状态::唯一自我冲突;
                return 结果;
            }
            if (实现_->初始化结果 && 实现_->初始化结果->成功()) {
                结果 = *实现_->初始化结果;
                结果.状态 = 新自我线程操作状态::无变化;
                return 结果;
            }
            实现_->锁定初始化请求 = 请求;
        }

        结果.世界树结果 = 实现_->世界树服务->初始化();
        if (!世界初始化成功(结果.世界树结果->状态)
            || !结果.世界树结果->世界树根场景) {
            结果.状态 = 新自我线程操作状态::世界树初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }

        const auto 事实 = 实现_->世界树服务->获取事实服务引用();
        if (!事实.存在概念服务.是抽象存在概念节点(
                请求.人类存在概念节点)) {
            结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }

        特征概念定义 服务对象定义;
        服务对象定义.材料类型 = 特征概念材料物理类型::存在节点引用;
        服务对象定义.值域 = 特征概念存在引用值域{
            请求.人类存在概念节点};
        服务对象定义.比较规则 = 特征概念比较规则::存在身份精确;
        服务对象定义.聚合规则 = 特征概念聚合规则::不自动聚合;
        const auto 服务对象特征类型根 =
            事实.特征概念服务.建立或取得特征概念(
                服务对象定义, 事实.特征值服务);
        const auto 服务对象类型读回 =
            事实.特征概念服务.比较单特征概念(
                服务对象特征类型根, 服务对象特征类型根);
        if (!有效(服务对象特征类型根)
            || 服务对象类型读回.状态 != 特征概念规则状态::已完成
            || 服务对象类型读回.特征类型根 != 服务对象特征类型根) {
            结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }
        结果.服务对象特征类型根 = 服务对象特征类型根;

        const std::vector<二次特征概念输入定义> 服务对象比较输入{
            {0, 二次特征输入角色::左输入,
                二次特征输入来源种类::特征准确值,
                服务对象特征类型根},
            {1, 二次特征输入角色::右输入,
                二次特征输入来源种类::特征准确值,
                服务对象特征类型根}};
        const auto 服务对象比较类型 =
            事实.二次特征概念服务.建立或取得默认二次特征概念(
                二次特征算法::特征材料相似度,
                服务对象比较输入, 服务对象特征类型根);
        if ((服务对象比较类型.状态 != 二次特征概念操作状态::已建立
                && 服务对象比较类型.状态 != 二次特征概念操作状态::已复用)
            || !服务对象比较类型.概念节点) {
            结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }
        结果.服务对象比较类型根 = 服务对象比较类型.概念节点;
        const auto 服务对象比较关系 =
            事实.二次特征概念服务.建立或取得参照结果域概念(
                *服务对象比较类型.概念节点,
                二次特征结果域种类::包含于参照值域);
        if ((服务对象比较关系.状态 != 二次特征概念操作状态::已建立
                && 服务对象比较关系.状态 != 二次特征概念操作状态::已复用)
            || !服务对象比较关系.概念节点) {
            结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }
        结果.服务对象目标比较关系 = 服务对象比较关系.概念节点;

        新存在建立请求 有效自我建立请求 = 请求.自我建立请求;
        // 冷启动不预置服务对象：初始特征组不得携带服务对象特征，自我建立后补无当前值特征。
        for (const auto& 初始特征 : 有效自我建立请求.初始特征组) {
            const auto 类型关系 = 事实.特征概念服务.比较单特征概念(
                初始特征.特征概念节点, 初始特征.特征概念节点);
            if (类型关系.状态 != 特征概念规则状态::已完成
                || !类型关系.特征类型根) {
                结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
                std::lock_guard 状态锁(实现_->状态互斥);
                实现_->初始化结果 = 结果;
                return 结果;
            }
            if (*类型关系.特征类型根 == 服务对象特征类型根) {
                结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
                std::lock_guard 状态锁(实现_->状态互斥);
                实现_->初始化结果 = 结果;
                return 结果;
            }
        }

        if (唯一自我未完成前缀) {
            if (唯一自我未完成前缀->请求 != 请求) {
                结果.状态 = 新自我线程操作状态::唯一自我冲突;
                return 结果;
            }
            const auto 场景 = 实现_->世界树服务->查询场景(
                唯一自我未完成前缀->场景节点);
            const auto 自我 = 唯一自我未完成前缀->建立结果.存在节点
                ? 实现_->世界树服务->查询存在(
                    *唯一自我未完成前缀->建立结果.存在节点)
                : std::nullopt;
            if (!场景 || !自我) {
                结果.状态 = 新自我线程操作状态::结构不一致;
            } else {
                结果.状态 = 新自我线程操作状态::自我建立失败;
                结果.自我建立结果 = 唯一自我未完成前缀->建立结果;
            }
            唯一自我线程所有者 = this;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }

        const bool 已有场景 = 有效(新版自我所在场景节点);
        const bool 已有自我 = 有效(新版自我节点);
        if (已有场景 != 已有自我) {
            结果.状态 = 新自我线程操作状态::结构不一致;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }

        稳定编码 场景节点;
        稳定编码 自我节点;
        bool 本次建立自我 = false;
        if (已有场景) {
            if ((唯一自我初始化请求 && *唯一自我初始化请求 != 请求)
                || (请求.既有自我所在场景
                    && *请求.既有自我所在场景
                        != 新版自我所在场景节点)) {
                结果.状态 = 新自我线程操作状态::唯一自我冲突;
                return 结果;
            }
            const auto 场景 = 实现_->世界树服务->查询场景(
                新版自我所在场景节点);
            const auto 自我 = 实现_->世界树服务->查询存在(新版自我节点);
            if (!场景 || !自我
                || std::find(场景->直接成员存在组.begin(),
                    场景->直接成员存在组.end(), 新版自我节点)
                    == 场景->直接成员存在组.end()
                || (请求.新场景父节点
                    && 场景->父场景节点 != 请求.新场景父节点)) {
                结果.状态 = 新自我线程操作状态::结构不一致;
                return 结果;
            }
            场景节点 = 新版自我所在场景节点;
            自我节点 = 新版自我节点;
        } else {
            bool 本次建立场景 = false;
            if (请求.既有自我所在场景) {
                if (!实现_->世界树服务->查询场景(
                        *请求.既有自我所在场景)) {
                    结果.状态 = 新自我线程操作状态::自我所在场景失败;
                    return 结果;
                }
                场景节点 = *请求.既有自我所在场景;
            } else {
                const auto 父场景 = 请求.新场景父节点
                    ? 请求.新场景父节点
                    : 结果.世界树结果->世界树根场景;
                if (!父场景 || !实现_->世界树服务->查询场景(*父场景)) {
                    结果.状态 = 新自我线程操作状态::自我所在场景失败;
                    return 结果;
                }
                结果.场景建立结果 =
                    实现_->世界树服务->建立子场景(*父场景);
                if (!场景建立成功(结果.场景建立结果->状态)
                    || !结果.场景建立结果->场景节点) {
                    结果.状态 = 新自我线程操作状态::自我所在场景失败;
                    return 结果;
                }
                场景节点 = *结果.场景建立结果->场景节点;
                本次建立场景 = true;
            }

            结果.自我建立结果 = 实现_->世界树服务->建立场景内存在(
                场景节点, 有效自我建立请求, 请求.初始状态时间);
            if (结果.自我建立结果->状态 != 新场景操作状态::已完成
                || !结果.自我建立结果->存在节点) {
                结果.状态 = 新自我线程操作状态::自我建立失败;
                if (结果.自我建立结果->状态
                        == 新场景操作状态::存在已建立但状态挂接未完成
                    && 结果.自我建立结果->存在节点) {
                    唯一自我未完成前缀 = 未完成自我建立前缀{
                        请求, 场景节点, *结果.自我建立结果};
                    唯一自我线程所有者 = this;
                    std::lock_guard 状态锁(实现_->状态互斥);
                    实现_->初始化结果 = 结果;
                    return 结果;
                }
                if (本次建立场景 && !结果.自我建立结果->存在节点) {
                    const auto 回滚 =
                        实现_->世界树服务->删除空场景(场景节点);
                    if (回滚 != 新场景操作状态::已完成) {
                        结果.状态 = 新自我线程操作状态::结构不一致;
                    }
                }
                return 结果;
            }
            自我节点 = *结果.自我建立结果->存在节点;
            本次建立自我 = true;
        }

        const auto 场景 = 实现_->世界树服务->查询场景(场景节点);
        const auto 自我 = 实现_->世界树服务->查询存在(自我节点);
        if (!场景 || !自我
            || std::find(场景->直接成员存在组.begin(),
                场景->直接成员存在组.end(), 自我节点)
                == 场景->直接成员存在组.end()) {
            结果.状态 = 新自我线程操作状态::结构不一致;
            return 结果;
        }

        std::vector<稳定编码> 服务对象特征候选;
        for (const auto 特征节点 : 自我->特征节点组) {
            const auto 特征 = 事实.特征服务.获取特征(特征节点);
            if (!特征) {
                结果.状态 = 新自我线程操作状态::结构不一致;
                return 结果;
            }
            const auto 类型关系 = 事实.特征概念服务.比较单特征概念(
                特征->特征概念节点, 特征->特征概念节点);
            if (类型关系.状态 != 特征概念规则状态::已完成
                || !类型关系.特征类型根) {
                结果.状态 = 新自我线程操作状态::结构不一致;
                return 结果;
            }
            if (*类型关系.特征类型根 == 服务对象特征类型根) {
                服务对象特征候选.push_back(特征节点);
            }
        }
        if (服务对象特征候选.empty()) {
            // 冷启动不预置服务对象：只建立概念约束的空特征，由获取服务对象本能写入当前值。
            const auto 添加 = 事实.存在服务.添加无当前值特征(自我节点, 服务对象特征类型根);
            if (添加.状态 != 新存在操作状态::已完成 || !添加.特征节点) {
                结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
                return 结果;
            }
            服务对象特征候选.push_back(*添加.特征节点);
        }
        if (服务对象特征候选.size() != 1) {
            结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
            return 结果;
        }
        const auto 服务对象特征 = 事实.特征服务.获取特征(
            服务对象特征候选.front());
        if (!服务对象特征 || (本次建立自我 && 服务对象特征->当前值)) {
            结果.状态 = 新自我线程操作状态::服务对象需求初始化失败;
            return 结果;
        }
        结果.服务对象特征节点 = 服务对象特征候选.front();

        // 自我所在场景由世界树持有当前值并随迁移更新（6220 §2）；登记失败不进入后续初始化。
        const auto 自我登记 = 实现_->世界树服务->登记自我(自我节点, 场景节点);
        if (自我登记 != 新场景操作状态::已完成 && 自我登记 != 新场景操作状态::无变化) {
            结果.状态 = 新自我线程操作状态::自我所在场景失败;
            return 结果;
        }
        新版自我所在场景节点 = 场景节点;
        新版自我节点 = 自我节点;
        唯一自我初始化请求 = 请求;
        唯一自我线程所有者 = this;
        结果.自我所在场景 = 场景节点;
        结果.自我存在 = 自我节点;
        const bool 原需求树有效 = 有效(需求树根节点);
        const bool 需求树就绪 = 实现_->需求服务->初始化();
        const bool 本次建立需求树 = !原需求树有效
            && 需求树就绪 && 有效(需求树根节点);
        结果.需求树根 = 需求树就绪 && 有效(需求树根节点)
            ? std::optional<稳定编码>{需求树根节点} : std::nullopt;
        if (!结果.需求树根) {
            结果.状态 = 新自我线程操作状态::需求树初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }
        结果.本能双根结果 =
            建立或取得自我本能双根(
                *实现_->世界树服务, *实现_->需求服务,
                自我节点, 请求.本能双根);
        if (!结果.本能双根结果->成功()) {
            结果.状态 = 新自我线程操作状态::本能双根初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }
        结果.安全根需求 =
            结果.本能双根结果->需求双根结果->安全根;
        结果.服务根需求 =
            结果.本能双根结果->需求双根结果->服务根;
        if (!实现_->服务结算确认端口) {
            结果.状态 = 新自我线程操作状态::服务一级需求初始化失败;
            return 结果;
        }
        结果.服务结算治理 = 实现_->服务结算确认端口->初始化治理(
            场景节点, 自我节点, *结果.本能双根结果->服务实际特征节点,
            *结果.服务根需求);
        const auto& 登记 = *结果.服务结算治理;
        const bool 本次建立服务一级需求 =
            登记.状态 == 新服务值结算协作状态::已完成;
        if ((!本次建立服务一级需求
                && 登记.状态 != 新服务值结算协作状态::精确重复)
            || !登记.治理定义 || !登记.结算需求节点) {
            结果.状态 = 新自我线程操作状态::服务一级需求初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }
        结果.完成服务需求 = 登记.结算需求节点;
        结果.完成服务需求建立结果 = 新需求建立结果{
            本次建立服务一级需求 ? 新需求操作状态::已建立 : 新需求操作状态::已复用,
            登记.结算需求节点};
        const bool 原方法根有效 = 有效(新方法组织根节点);
        const bool 方法根就绪 = 实现_->方法服务->初始化();
        const bool 本次建立方法根 = !原方法根有效
            && 方法根就绪 && 有效(新方法组织根节点);
        结果.方法组织根 = 方法根就绪 && 有效(新方法组织根节点)
            ? std::optional<稳定编码>{新方法组织根节点} : std::nullopt;
        if (!结果.方法组织根) {
            结果.状态 = 新自我线程操作状态::方法根初始化失败;
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            return 结果;
        }
        结果.状态 = !本次建立自我
                && !本次建立需求树
                && !本次建立服务一级需求
                && !本次建立方法根
                && 结果.本能双根结果->状态
                    == 新自我本能双根初始化状态::已复用
            ? 新自我线程操作状态::无变化
            : 新自我线程操作状态::已完成;
        {
            std::lock_guard 状态锁(实现_->状态互斥);
            实现_->初始化结果 = 结果;
            实现_->生命周期 = 新自我线程生命周期::已初始化;
        }
        return 结果;
    } catch (...) {
        结果.状态 = 新自我线程操作状态::内部错误;
        return 结果;
    }
}

bool 新_自我线程类::绑定方法问题学习(方法问题学习服务& 服务) noexcept {
    if (!实现_) return false;
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::lock_guard 锁(实现_->状态互斥);
        if (实现_->问题学习服务==&服务) return true;
        if (实现_->问题学习服务 || 实现_->工作线程.joinable() || 实现_->线程已完成) return false;
        实现_->问题学习服务=&服务;
        return true;
    } catch (...) { return false; }
}
bool 新_自我线程类::绑定模拟学习(模拟学习服务& 服务) noexcept {
    if (!实现_) return false;
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::lock_guard 锁(实现_->状态互斥);
        if (实现_->学习服务==&服务) return true;
        if (实现_->学习服务 || 实现_->工作线程.joinable() || 实现_->线程已完成) return false;
        实现_->学习服务=&服务;
        return true;
    } catch (...) { return false; }
}

新自我线程操作结果 新_自我线程类::启动(
    std::uint64_t 等待毫秒) noexcept {
    if (!实现_) {
        return {新自我线程操作状态::资源失败,
            新自我线程生命周期::内部错误};
    }
    if (!等待毫秒) {
        return {新自我线程操作状态::入口拒绝,
            新自我线程生命周期::未初始化};
    }
    try {
        std::unique_lock 加入锁(实现_->加入互斥);
        std::unique_lock 状态锁(实现_->状态互斥);
        if (!实现_->初始化结果 || !实现_->初始化结果->成功()) {
            return 实现_->操作结果(新自我线程操作状态::尚未初始化);
        }
        if (实现_->线程已进入 && !实现_->线程已完成) {
            return 实现_->操作结果(新自我线程操作状态::已在运行);
        }
        if (实现_->停止源.stop_requested() || 实现_->线程已完成 || 实现_->工作线程.joinable()) {
            return 实现_->操作结果(新自我线程操作状态::入口拒绝);
        }
        if (!实现_->消息配置 || !实现_->任务管理) {
            return 实现_->操作结果(新自我线程操作状态::尚未初始化);
        }
        if (!实现_->自我工作线程) {
            实现_->自我工作线程.reset(new (std::nothrow) 新_自我工作线程类(
                *实现_->需求服务, *实现_->任务服务, [p = 实现_.get()] {
                    try {
                        std::lock_guard 锁(p->状态互斥);
                        p->状态变化.notify_all();
                    } catch (...) {}
                }));
            if (!实现_->自我工作线程) {
                return 实现_->操作结果(新自我线程操作状态::资源失败);
            }
        }
        实现_->生命周期 = 新自我线程生命周期::启动中;
        if (实现_->学习服务 && !实现_->学习线程) {
            std::weak_ptr<实现::筹办反馈桥> 弱桥=实现_->反馈桥;
            auto 完成通知=[弱桥] {
                if (const auto 桥=弱桥.lock()) {
                    std::lock_guard 桥锁(桥->互斥);
                    if (桥->目标) {
                        std::lock_guard 锁(桥->目标->状态互斥);
                        桥->目标->唤醒因果能力等待已加锁();
                        桥->目标->状态变化.notify_all();
                    }
                }
            };
            if (实现_->问题学习服务)
                实现_->学习线程=std::make_unique<新_学习线程类>(
                    *实现_->学习服务,*实现_->问题学习服务,完成通知);
            else 实现_->学习线程=std::make_unique<新_学习线程类>(*实现_->学习服务,完成通知);
        }
        状态锁.unlock();
        // manager先启动并建立结算T，self尚未开启消息、扫描或学习线程。
        // 加入互斥仍固定绑定；跨owner调用不持self状态锁。
        const auto 结算需求 = 实现_->初始化结果->完成服务需求.value_or(稳定编码{});
        const auto 管理快照 = 实现_->任务管理->读取快照();
        if (管理快照.生命周期 == 新任务管理生命周期::内部错误) {
            加入锁.unlock();
            报告服务要求采用启动异常(L"读取任务管理快照",
                static_cast<int>(管理快照.生命周期), 结算需求,
                管理快照.服务值结算任务.value_or(稳定编码{}));
        }
        // 已进入manager线程后，快照短暂仍可能是“启动中”；不误拒该过渡。
        const bool 管理尚未就绪 = 管理快照.生命周期 == 新任务管理生命周期::未启动
            || (管理快照.生命周期 == 新任务管理生命周期::启动中
                && !管理快照.服务值结算任务);
        if (管理尚未就绪 || 管理快照.停止已请求 || 管理快照.线程已完成
            || 管理快照.生命周期 == 新任务管理生命周期::停止中
            || 管理快照.生命周期 == 新任务管理生命周期::已停止
            || 管理快照.生命周期 == 新任务管理生命周期::启动失败) {
            状态锁.lock();
            实现_->生命周期 = 实现_->停止已请求
                ? 新自我线程生命周期::已停止 : 新自我线程生命周期::已初始化;
            实现_->线程已完成 = 实现_->停止已请求;
            实现_->状态变化.notify_all();
            const auto 拒绝 = 实现_->操作结果(管理尚未就绪
                ? 新自我线程操作状态::尚未启动 : 新自我线程操作状态::入口拒绝);
            状态锁.unlock();
            加入锁.unlock();
            记录治理绑定拒绝(L"启动", L"任务管理尚未就绪或已停止", 拒绝,
                static_cast<int>(管理快照.生命周期));
            return 拒绝;
        }
        const auto 治理定义 = 实现_->初始化结果->服务结算治理
                && 实现_->初始化结果->服务结算治理->治理定义
            ? 实现_->任务服务->读取服务值治理定义(
                实现_->初始化结果->服务结算治理->治理定义->节点) : std::nullopt;
        if (!治理定义 || 治理定义 != 实现_->初始化结果->服务结算治理->治理定义
            || !有效(结算需求) || !管理快照.服务值结算治理已配置
            || !管理快照.服务值结算任务 || !有效(*管理快照.服务值结算任务)
            || 管理快照.服务值结算需求 != 实现_->初始化结果->完成服务需求) {
            加入锁.unlock();
            报告服务要求采用启动异常(L"结算任务及需求身份核验",
                static_cast<int>(管理快照.生命周期), 结算需求,
                管理快照.服务值结算任务.value_or(稳定编码{}));
        }
        const auto 采用来源 = 实现_->需求服务->配置服务要求采用来源已核验(
            结算需求, *管理快照.服务值结算任务, *实现_->任务服务);
        if (采用来源 != 新需求操作状态::已修改 && 采用来源 != 新需求操作状态::无变化) {
            加入锁.unlock();
            报告服务要求采用启动异常(L"配置服务要求采用来源已核验",
                static_cast<int>(采用来源), 结算需求, *管理快照.服务值结算任务);
        }
        状态锁.lock();
        if (实现_->停止已请求) {
            实现_->生命周期 = 新自我线程生命周期::已停止;
            实现_->线程已完成 = true;
            实现_->状态变化.notify_all();
            return 实现_->操作结果(新自我线程操作状态::入口拒绝);
        }
        状态锁.unlock();
        const auto 绑定 = 实现_->绑定关联检查通知();
        if (绑定 != 新自我线程操作状态::已完成) {
            实现_->停止源.request_stop();
            状态锁.lock();
            实现_->生命周期 = 新自我线程生命周期::启动失败;
            实现_->线程已完成 = true;
            实现_->状态变化.notify_all();
            return 实现_->操作结果(绑定);
        }
        const auto 工作启动 = 实现_->自我工作线程->启动(等待毫秒);
        if (工作启动.状态 != 新自我工作操作状态::已完成
            && 工作启动.状态 != 新自我工作操作状态::已在运行) {
            实现_->停止源.request_stop();
            (void)实现_->自我工作线程->请求停止();
            const auto 工作停止 = 实现_->自我工作线程->等待停止(等待毫秒);
            实现_->解除关联检查通知();
            状态锁.lock();
            实现_->生命周期 = 新自我线程生命周期::启动失败;
            实现_->线程已完成 = true;
            实现_->状态变化.notify_all();
            const bool 已回收 = 工作停止.状态 == 新自我工作操作状态::已完成
                || 工作停止.状态 == 新自我工作操作状态::无变化;
            return 实现_->操作结果(
                工作启动.状态 == 新自我工作操作状态::资源失败 && 已回收
                    ? 新自我线程操作状态::资源失败
                    : 新自我线程操作状态::内部错误);
        }
        if (实现_->学习线程) {
            const auto 学习启动=实现_->学习线程->启动(等待毫秒);
            if (学习启动!=新学习操作状态::已完成 && 学习启动!=新学习操作状态::无变化) {
                实现_->停止源.request_stop();
                (void)实现_->学习线程->请求停止();
                (void)实现_->学习线程->等待停止(等待毫秒);
                (void)实现_->自我工作线程->请求停止();
                (void)实现_->自我工作线程->等待停止(等待毫秒);
                实现_->解除关联检查通知();
                状态锁.lock();
                实现_->生命周期=新自我线程生命周期::启动失败;
                实现_->线程已完成=true;
                return 实现_->操作结果(新自我线程操作状态::内部错误);
            }
        }
        try {
            实现_->工作线程 = std::thread([p = 实现_.get()] { p->运行(); });
        } catch (...) {
            实现_->停止源.request_stop();
            if (实现_->学习线程) {
                (void)实现_->学习线程->请求停止();
                (void)实现_->学习线程->等待停止(等待毫秒);
            }
            (void)实现_->自我工作线程->请求停止();
            (void)实现_->自我工作线程->等待停止(等待毫秒);
            实现_->解除关联检查通知();
            状态锁.lock();
            实现_->生命周期 = 新自我线程生命周期::启动失败;
            实现_->线程已完成 = true;
            实现_->状态变化.notify_all();
            return 实现_->操作结果(新自我线程操作状态::资源失败);
        }
        状态锁.lock();
        if (!实现_->状态变化.wait_for(
            状态锁, 有界等待时长(等待毫秒), [this] {
                return 实现_->线程已进入 || 实现_->线程已完成;
            })) {
            return 实现_->操作结果(新自我线程操作状态::等待超时);
        }
        if (实现_->生命周期 == 新自我线程生命周期::内部错误) {
            return 实现_->操作结果(新自我线程操作状态::内部错误);
        }
        return 实现_->操作结果(新自我线程操作状态::已完成);
    } catch (const std::exception& 异常) {
        实现_->停止源.request_stop();
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"自我线程启动", L"启动初始化或线程控制异常。", 异常.what());
    } catch (...) {
        实现_->停止源.request_stop();
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"自我线程启动", L"启动初始化或线程控制未知异常。");
    }
}

新自我线程操作结果 新_自我线程类::配置消息处理(
    const 新自我消息运行配置& 配置) noexcept {
    if (!实现_) return {新自我线程操作状态::资源失败,
        新自我线程生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::lock_guard 锁(实现_->状态互斥);
        const auto 最大间隔 = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::duration::max() / 4).count();
        if (!配置.邮箱容量 || !配置.资源重试间隔毫秒
            || 配置.邮箱容量 > 实现_->活动消息.max_size()
            || 配置.资源重试间隔毫秒 > static_cast<std::uint64_t>(最大间隔)) {
            return 实现_->操作结果(新自我线程操作状态::消息配置不合法);
        }
        if (实现_->消息配置 == 配置) {
            return 实现_->操作结果(新自我线程操作状态::无变化);
        }
        if (实现_->工作线程.joinable() || 实现_->线程已完成) {
            return 实现_->操作结果(新自我线程操作状态::已在运行);
        }
        实现_->消息配置 = 配置;
        return 实现_->操作结果(新自我线程操作状态::已完成);
    } catch (...) {
        return {新自我线程操作状态::内部错误, 新自我线程生命周期::内部错误};
    }
}

新自我线程操作结果 新_自我线程类::绑定接收需求认知(接收需求认知服务& provider) noexcept {
    if(!实现_)return {新自我线程操作状态::资源失败,新自我线程生命周期::内部错误};
    try {
        std::lock_guard join(实现_->加入互斥);
        接收需求认知服务* 原=nullptr;新_任务类* task=nullptr;
        {
            std::lock_guard lock(实现_->状态互斥);
            if(实现_->工作线程.joinable()||实现_->线程已完成)return 实现_->操作结果(新自我线程操作状态::入口拒绝);
            原=实现_->接收认知;task=实现_->任务服务;
            if(原&&原!=&provider)return 实现_->操作结果(新自我线程操作状态::入口拒绝);
        }
        if(!task)throw std::logic_error("self has no task owner for reader binding");
        task->装配前置语言输入读取接口(&provider);
        std::lock_guard lock(实现_->状态互斥);
        if(实现_->接收认知!=原||实现_->工作线程.joinable()||实现_->线程已完成)
            throw std::logic_error("self input binding changed inside exclusive startup sequence");
        实现_->接收认知=&provider;实现_->接收认知待唤醒=true;
        if(原==&provider)return 实现_->操作结果(新自我线程操作状态::无变化);
        return 实现_->操作结果(新自我线程操作状态::已完成);
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"绑定接收认知",L"启动前只读端口装配或绑定一致性异常");}
}
新自我线程操作结果 新_自我线程类::绑定外设世界树(
    const 新_外设世界树类& 外设世界, 稳定编码 外设场景标识类型,
    稳定编码 外设存在标识类型) noexcept {
    if (!实现_) return {新自我线程操作状态::资源失败, 新自我线程生命周期::内部错误};
    try {
        std::lock_guard join(实现_->加入互斥);
        std::lock_guard lock(实现_->状态互斥);
        if (实现_->工作线程.joinable() || 实现_->线程已完成 || !有效(外设场景标识类型)
            || !有效(外设存在标识类型) || 外设场景标识类型 == 外设存在标识类型)
            return 实现_->操作结果(新自我线程操作状态::入口拒绝);
        if (实现_->外设世界)
            return 实现_->外设世界 == &外设世界 && 实现_->外设场景标识类型 == 外设场景标识类型
                    && 实现_->外设存在标识类型 == 外设存在标识类型
                ? 实现_->操作结果(新自我线程操作状态::无变化)
                : 实现_->操作结果(新自我线程操作状态::入口拒绝);
        实现_->外设世界 = &外设世界;
        实现_->外设场景标识类型 = 外设场景标识类型;
        实现_->外设存在标识类型 = 外设存在标识类型;
        实现_->外设交互场景待补建 = true; // 启动后先补建一次，不依赖通知必然送达。
        return 实现_->操作结果(新自我线程操作状态::已完成);
    } catch (...) { return {新自我线程操作状态::内部错误, 新自我线程生命周期::内部错误}; }
}

void 新_自我线程类::通知外设交互场景变化() noexcept {
    if (!实现_) return;
    try {
        std::lock_guard lock(实现_->状态互斥);
        if (实现_->停止已请求 || 实现_->线程已完成 || !实现_->外设世界) return;
        实现_->外设交互场景待补建 = true;
        实现_->状态变化.notify_all();
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"通知外设交互场景变化", L"外设交互场景轻通知异常。");
    }
}

void 新_自我线程类::通知接收认知变化() noexcept {
    if(!实现_)return;
    try {std::lock_guard lock(实现_->状态互斥);
        if(实现_->停止已请求||实现_->线程已完成||!实现_->接收认知)return;
        实现_->接收认知待唤醒=true;实现_->状态变化.notify_all();
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"通知接收认知变化",L"接收认知轻通知异常。");}
}

新自我线程操作结果 新_自我线程类::绑定服务值结算确认端口(
    新服务值结算确认接口& 端口) noexcept {
    if (!实现_) return {新自我线程操作状态::资源失败, 新自我线程生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::lock_guard 锁(实现_->状态互斥);
        if (实现_->服务结算确认端口 == &端口)
            return 实现_->操作结果(新自我线程操作状态::无变化);
        if (实现_->服务结算确认端口 || 实现_->初始化结果
            || 实现_->工作线程.joinable() || 实现_->线程已完成)
            return 实现_->操作结果(新自我线程操作状态::入口拒绝);
        实现_->服务结算确认端口 = &端口;
        return 实现_->操作结果(新自我线程操作状态::已完成);
    } catch (...) { return {新自我线程操作状态::内部错误, 新自我线程生命周期::内部错误}; }
}

新自我线程操作结果 新_自我线程类::绑定任务管理(
    新_任务管理线程类& 任务管理) noexcept {
    if (!实现_) return {新自我线程操作状态::资源失败, 新自我线程生命周期::内部错误};
    try {
        std::unique_lock 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->状态互斥);
        const auto 拒绝绑定 = [&](新自我线程操作状态 状态, const wchar_t* 阶段,
            int 上游状态 = -1) {
            const auto 结果 = 实现_->操作结果(状态);
            锁.unlock();
            加入锁.unlock();
            记录治理绑定拒绝(L"绑定任务管理", 阶段, 结果, 上游状态);
            return 结果;
        };
        if (!任务管理.使用任务服务(*实现_->任务服务))
            return 拒绝绑定(新自我线程操作状态::入口拒绝, L"任务服务不一致");
        if (实现_->任务管理 == &任务管理)
            return 实现_->操作结果(新自我线程操作状态::无变化);
        if (实现_->任务管理)
            return 拒绝绑定(新自我线程操作状态::唯一自我冲突, L"已有不同任务管理绑定");
        if (实现_->工作线程.joinable() || 实现_->线程已完成)
            return 拒绝绑定(新自我线程操作状态::已在运行, L"线程已经创建或已完成");
        const auto 桥 = std::make_shared<实现::筹办反馈桥>();
        桥->目标 = 实现_.get();
        std::weak_ptr<实现::筹办反馈桥> 弱桥 = 桥;
        if (!实现_->初始化结果 || !实现_->初始化结果->成功()) {
            桥->目标 = nullptr;
            return 拒绝绑定(新自我线程操作状态::尚未初始化, L"初始化结果缺少结算需求或服务实际特征");
        }
        const auto 绑定 = 任务管理.绑定自我筹办反馈(
            桥.get(), [弱桥](const 新任务筹办反馈定位& 定位) noexcept {
                const auto 已锁定 = 弱桥.lock();
                if (!已锁定) return 新任务筹办反馈接收状态::已停止接收;
                std::lock_guard 桥锁(已锁定->互斥);
                return 已锁定->目标
                    ? 已锁定->目标->接收筹办反馈(定位)
                    : 新任务筹办反馈接收状态::已停止接收;
            }, [弱桥](const 新任务结果反馈定位& 定位) noexcept {
                const auto 已锁定 = 弱桥.lock();
                if (!已锁定) return 新任务结果反馈接收状态::已停止接收;
                std::lock_guard 桥锁(已锁定->互斥);
                return 已锁定->目标
                    ? 已锁定->目标->接收任务结果反馈(定位)
                    : 新任务结果反馈接收状态::已停止接收;
            }, [弱桥](const 新任务等待复核定位& 定位) noexcept {
                const auto 已锁定 = 弱桥.lock();
                if (!已锁定) return 新任务等待复核反馈接收状态::已停止接收;
                std::lock_guard 桥锁(已锁定->互斥);
                return 已锁定->目标
                    ? 已锁定->目标->接收等待复核(定位)
                    : 新任务等待复核反馈接收状态::已停止接收;
            }, [弱桥](const 新任务贡献反馈定位& 定位) noexcept {
                const auto 已锁定 = 弱桥.lock();
                if (!已锁定) return 新任务结果反馈接收状态::已停止接收;
                std::lock_guard 桥锁(已锁定->互斥);
                return 已锁定->目标 ? 已锁定->目标->接收任务完成贡献复核(定位)
                    : 新任务结果反馈接收状态::已停止接收;
            }, [弱桥]() noexcept {
                const auto 已锁定 = 弱桥.lock();
                if (!已锁定) return;
                std::lock_guard 桥锁(已锁定->互斥);
                if (已锁定->目标) {
                    已锁定->目标->最终服务工作待唤醒.store(true);
                    已锁定->目标->状态变化.notify_all();
                }
            }, 新长期服务值结算配置{
                实现_->世界树服务,
                *实现_->初始化结果->自我所在场景,
                *实现_->初始化结果->自我存在,
                *实现_->初始化结果->本能双根结果->服务实际特征节点,
                *实现_->初始化结果->完成服务需求,
                *实现_->初始化结果->服务根需求,
                实现_->初始化结果->服务结算治理->治理定义->自我概念节点,
                实现_->初始化结果->服务结算治理->治理定义->服务值特征类型节点,
                {新结算能力类别::自我服务值治理, 实现_->初始化结果->服务结算治理->治理定义->节点},
                实现_->服务结算确认端口
            });
        if (绑定 != 新任务管理操作状态::已完成
            && 绑定 != 新任务管理操作状态::无变化) {
            桥->目标 = nullptr;
            if (绑定 == 新任务管理操作状态::内部错误) {
                锁.unlock();
                加入锁.unlock();
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"绑定任务管理", L"绑定自我筹办反馈返回内部错误。");
            }
            return 拒绝绑定(绑定 == 新任务管理操作状态::资源失败
                ? 新自我线程操作状态::资源失败
                : 新自我线程操作状态::入口拒绝, L"绑定自我筹办反馈被拒", static_cast<int>(绑定));
        }
        // 此阶段manager尚未启动，只冻结反馈与结算配置；实际T在其启动时建立。
        实现_->任务管理 = &任务管理;
        实现_->反馈桥 = 桥;
        return 实现_->操作结果(新自我线程操作状态::已完成);
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"绑定任务管理", L"任务管理接口绑定异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"绑定任务管理", L"任务管理接口绑定未知异常。");
    }
}

新自我消息接收结果 新_自我线程类::提交消息(
    const 新自我治理消息& 消息) noexcept {
    新自我消息接收结果 结果;
    if (!实现_) {
        结果.状态 = 新自我消息接收状态::资源失败;
        return 结果;
    }
    if (!消息.消息编号 || 消息.完整服务需求材料 || 消息.完成贡献复核 || 消息.行动请求来源) return 结果;
    if (消息.类型 == 新自我消息类型::事实变化) {
        if (!有效(消息.来源状态节点) || 有效(消息.需求节点)
            || 消息.筹办反馈 || 消息.结果反馈
            || 消息.等待复核) return 结果;
    } else if (消息.类型 == 新自我消息类型::需求承接) {
        if (!有效(消息.需求节点) || 有效(消息.来源状态节点)
            || 消息.筹办反馈 || 消息.结果反馈
            || 消息.等待复核) return 结果;
    } else if (消息.类型 == 新自我消息类型::任务筹办反馈) {
        // 此类型只能由绑定manager的窄回调提交，外部调用者不得伪造。
        return 结果;
    } else return 结果;
    try {
        std::lock_guard 锁(实现_->状态互斥);
        if (实现_->停止已请求 || 实现_->线程已完成) {
            结果.状态 = 新自我消息接收状态::已停止接收;
            return 结果;
        }
        if (!实现_->线程已进入) {
            结果.状态 = 新自我消息接收状态::尚未启动;
            return 结果;
        }
        if (!实现_->消息配置) {
            结果.状态 = 新自我消息接收状态::尚未配置;
            return 结果;
        }
        if (消息.类型 == 新自我消息类型::需求承接
            && 消息.需求节点 != *实现_->初始化结果->安全根需求
            && 消息.需求节点 != *实现_->初始化结果->服务根需求) {
            return 结果;
        }
        for (const auto& 项 : 实现_->活动消息) {
            if (项->消息.消息编号 != 消息.消息编号) continue;
            if (项->消息 != 消息) {
                结果.状态 = 新自我消息接收状态::身份冲突;
            } else {
                结果.状态 = 新自我消息接收状态::精确重复;
                结果.处理结果 = 项->结果;
            }
            return 结果;
        }
        if (实现_->普通消息数量已加锁() >= 实现_->消息配置->邮箱容量) {
            结果.状态 = 新自我消息接收状态::邮箱已满;
            return 结果;
        }
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息 = 消息;
        项->可处理时点 = std::chrono::steady_clock::now();
        实现_->活动消息.push_back(项);
        结果.处理结果 = 项->结果;
        结果.状态 = 新自我消息接收状态::已接收;
        实现_->状态变化.notify_all();
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新自我消息接收状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = 新自我消息接收状态::内部错误;
        return 结果;
    }
}

新自我消息接收结果 新_自我线程类::提交完整服务需求材料(
    const 新完整服务需求材料包& 材料包) noexcept {
    if (!有效(材料包.要求节点) || !有效(材料包.理解节点) || !有效(材料包.确认节点)) return {};
    if (!实现_) return {新自我消息接收状态::资源失败, {}};
    try {
        std::lock_guard 锁(实现_->状态互斥);
        if (实现_->停止已请求 || 实现_->线程已完成)
            return {新自我消息接收状态::已停止接收, {}};
        if (!实现_->线程已进入) return {新自我消息接收状态::尚未启动, {}};
        if (!实现_->消息配置) return {新自我消息接收状态::尚未配置, {}};
        if (实现_->普通消息数量已加锁() >= 实现_->消息配置->邮箱容量)
            return {新自我消息接收状态::邮箱已满, {}};
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息.消息编号 = 实现::取得自动消息编号();
        if (!项->消息.消息编号) throw std::logic_error("external service message identity exhausted");
        项->消息.类型 = 新自我消息类型::完整服务需求材料;
        项->消息.完整服务需求材料 = 材料包;
        项->可处理时点 = std::chrono::steady_clock::now();
        // 不按R合并在途包：后来的U/C必须独立读回，不能沿用旧包的结论。
        实现_->活动消息.push_back(项);
        实现_->状态变化.notify_all();
        return {新自我消息接收状态::已接收, 项->结果};
    } catch (const std::bad_alloc&) { return {新自我消息接收状态::资源失败, {}};
    } catch (...) {
        工程异常终止(L"新_自我线程类.cpp", __LINE__, L"提交完整服务需求材料", L"完整需求信息包接收异常");
    }
}

新自我消息接收结果 新_自我线程类::提交行动请求承接(
    const 新行动请求承接来源& source) noexcept {
    if(!实现_)return {新自我消息接收状态::资源失败,{}};
    return 实现_->接纳行动请求(source);
}
新自我消息接收结果 新_自我线程类::提交前置语言输入材料(const 新前置语言输入定位& p) noexcept {
    if(!实现_)return {新自我消息接收状态::资源失败,{}};
    return 实现_->接纳前置输入(p);
}

新自我消息接收结果 新_自我线程类::提交事实变化(稳定编码 来源状态节点) noexcept {
    新自我消息接收结果 结果;
    if (!实现_) { 结果.状态 = 新自我消息接收状态::资源失败; return 结果; }
    if (!有效(来源状态节点)) return 结果;
    try {
        std::lock_guard 锁(实现_->状态互斥);
        if (实现_->停止已请求 || 实现_->线程已完成) {
            结果.状态 = 新自我消息接收状态::已停止接收; return 结果;
        }
        if (!实现_->线程已进入) { 结果.状态 = 新自我消息接收状态::尚未启动; return 结果; }
        if (!实现_->消息配置) { 结果.状态 = 新自我消息接收状态::尚未配置; return 结果; }
        实现_->待转交事实变化.push_back(来源状态节点);
        实现_->状态变化.notify_all();
        结果.状态 = 新自我消息接收状态::已接收;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新自我消息接收状态::资源失败;
    } catch (...) {
        结果.状态 = 新自我消息接收状态::内部错误;
    }
    return 结果;
}

std::stop_token 新_自我线程类::读取停止令牌() const noexcept {
    if (!实现_) 工程异常终止(L"新_自我线程类.cpp", __LINE__, L"读取停止令牌", L"自我线程实现未建立");
    return 实现_->停止源.get_token();
}

新自我线程操作结果 新_自我线程类::请求停止() noexcept {
    if (!实现_) {
        return {新自我线程操作状态::资源失败,
            新自我线程生命周期::内部错误};
    }
    实现_->停止源.request_stop();
    try {
        std::lock_guard 状态锁(实现_->状态互斥);
        if (!实现_->初始化结果 || !实现_->初始化结果->成功()) {
            return 实现_->操作结果(新自我线程操作状态::尚未初始化);
        }
        if (!实现_->线程已进入
            && 实现_->生命周期 == 新自我线程生命周期::已初始化) {
            return 实现_->操作结果(新自我线程操作状态::尚未启动);
        }
        if (实现_->停止已请求 || 实现_->线程已完成) {
            return 实现_->操作结果(新自我线程操作状态::无变化);
        }
        实现_->停止已请求 = true;
        实现_->生命周期 = 新自我线程生命周期::正在停止;
        if (实现_->自我工作线程) {
            (void)实现_->自我工作线程->请求停止();
        }
        if (实现_->学习线程) (void)实现_->学习线程->请求停止();
        实现_->状态变化.notify_all();
        return 实现_->操作结果(新自我线程操作状态::已完成);
    } catch (...) {
        return {新自我线程操作状态::内部错误,
            新自我线程生命周期::内部错误};
    }
}

新自我线程操作结果 新_自我线程类::等待停止(
    std::uint64_t 等待毫秒) noexcept {
    if (!实现_) {
        return {新自我线程操作状态::资源失败,
            新自我线程生命周期::内部错误};
    }
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        if (实现_->工作线程.joinable()
            && 实现_->工作线程.get_id() == std::this_thread::get_id()) {
            std::lock_guard 状态锁(实现_->状态互斥);
            return 实现_->操作结果(新自我线程操作状态::不能等待自身);
        }
        std::unique_lock 状态锁(实现_->状态互斥);
        if (!实现_->初始化结果 || !实现_->初始化结果->成功()) {
            return 实现_->操作结果(新自我线程操作状态::尚未初始化);
        }
        if (!实现_->工作线程.joinable() && !实现_->线程已进入) {
            return 实现_->操作结果(实现_->已加入
                ? 新自我线程操作状态::无变化
                : 新自我线程操作状态::尚未启动);
        }
        if (!实现_->状态变化.wait_for(
            状态锁, 有界等待时长(等待毫秒), [this] {
                return 实现_->线程已完成;
            })) {
            return 实现_->操作结果(新自我线程操作状态::等待超时);
        }
        状态锁.unlock();
        if (实现_->工作线程.joinable()) 实现_->工作线程.join();
        实现_->解除关联检查通知();
        if (实现_->学习线程) {
            (void)实现_->学习线程->请求停止();
            const auto 学习停止=实现_->学习线程->等待停止(等待毫秒);
            if (学习停止!=新学习操作状态::已完成 && 学习停止!=新学习操作状态::无变化) {
                状态锁.lock();
                实现_->生命周期=新自我线程生命周期::内部错误;
                return 实现_->操作结果(新自我线程操作状态::内部错误);
            }
        }
        if (实现_->自我工作线程) {
            const auto 工作停止 = 实现_->自我工作线程->等待停止(等待毫秒);
            if (工作停止.状态 != 新自我工作操作状态::已完成
                && 工作停止.状态 != 新自我工作操作状态::无变化) {
                状态锁.lock();
                实现_->生命周期 = 新自我线程生命周期::内部错误;
                return 实现_->操作结果(新自我线程操作状态::内部错误);
            }
        }
        状态锁.lock();
        实现_->已加入 = true;
        实现_->生命周期 = 新自我线程生命周期::已停止;
        return 实现_->操作结果(新自我线程操作状态::已完成);
    } catch (...) {
        return {新自我线程操作状态::内部错误,
            新自我线程生命周期::内部错误};
    }
}

新学习工作快照 新_自我线程类::读取学习工作快照() const noexcept {
    if (!实现_) return {};
    try {
        std::lock_guard 锁(实现_->状态互斥);
        if (实现_->学习线程) {
            auto 结果 = 实现_->学习线程->读取工作快照();
            if (结果.状态 != 新学习工作快照读取状态::已取得) return 结果;
            for (const auto& [M, 槽] : 实现_->在途学习) {
                if (槽.阶段 == 实现::学习交接阶段::硬失败) continue;
                for (auto& 项 : 结果.项组) {
                    if (项.学习请求 && 项.学习请求 == 槽.当前请求)
                        项.请求者数量 = static_cast<std::uint64_t>(槽.请求者组.size());
                }
                for (const auto& 待项 : 槽.待处理请求组) {
                    新学习工作快照项 项;
                    项.方法节点 = M;
                    项.学习请求 = 待项.请求;
                    项.请求者数量 = static_cast<std::uint64_t>(待项.请求者组.size());
                    项.自我待提交 = true;
                    结果.项组.push_back(std::move(项));
                    ++结果.自我待提交请求数;
                }
            }
            return 结果;
        }
        新学习工作快照 结果;
        结果.状态 = 新学习工作快照读取状态::尚未建立;
        return 结果;
    } catch (const std::bad_alloc&) {
        新学习工作快照 结果;
        结果.状态 = 新学习工作快照读取状态::资源失败;
        return 结果;
    } catch (...) { return {}; }
}

新自我线程快照 新_自我线程类::读取快照() const noexcept {
    新自我线程快照 快照;
    if (!实现_) {
        快照.生命周期 = 新自我线程生命周期::内部错误;
        return 快照;
    }
    try {
        std::lock_guard 状态锁(实现_->状态互斥);
        快照.生命周期 = 实现_->生命周期;
        if (实现_->初始化结果 && 实现_->初始化结果->成功()) {
            // 快照显示自我当前所在场景；世界树未登记时退回初始化场景。
            const auto 当前场景 = 实现_->世界树服务 ? 实现_->世界树服务->读取自我所在场景() : std::nullopt;
            快照.自我所在场景 = 当前场景 ? 当前场景 : 实现_->初始化结果->自我所在场景;
            快照.自我存在 = 实现_->初始化结果->自我存在;
            快照.需求树根 = 实现_->初始化结果->需求树根;
            快照.安全根需求 = 实现_->初始化结果->安全根需求;
            快照.服务根需求 = 实现_->初始化结果->服务根需求;
            快照.完成服务需求 = 实现_->初始化结果->完成服务需求;
            快照.服务对象特征节点 = 实现_->初始化结果->服务对象特征节点;
            快照.需要服务对象需求 = 实现_->初始化结果->需要服务对象需求;
            快照.方法组织根 = 实现_->初始化结果->方法组织根;
        }
        快照.线程已进入 = 实现_->线程已进入;
        快照.停止已请求 = 实现_->停止已请求;
        快照.线程已完成 = 实现_->线程已完成;
        快照.活动消息数 = 实现_->活动消息.size();
        快照.首次需求复核 = 实现_->首次需求复核;
        快照.需求扫描阶段 = 实现_->需求扫描阶段;
        快照.最近需求数量 = 实现_->最近需求数量;
        快照.最近任务数量 = 实现_->最近任务数量;
        快照.最近已消费检查结果 = 实现_->最近已消费检查结果;
        快照.待承接需求数 = static_cast<std::uint64_t>(实现_->扫描承接.size());
        快照.待学习方法数=static_cast<std::uint64_t>(std::count_if(
            实现_->在途学习.begin(),实现_->在途学习.end(),[](const auto& 项) {
                return 项.second.阶段 != 实现::学习交接阶段::硬失败;
            }));
        快照.最近学习结束=实现_->最近学习结束;
        快照.最近学习运行异常=实现_->最近学习运行异常;
        return 快照;
    } catch (...) {
        快照.生命周期 = 新自我线程生命周期::内部错误;
        return 快照;
    }
}

} // namespace 海中鱼巣
