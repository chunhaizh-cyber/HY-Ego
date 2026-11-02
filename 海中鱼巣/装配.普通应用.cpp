#include "装配.普通应用.h"
#include "装配.网页外设.h"
#include "业务/内部治理.世界.h"
#include "业务/内部治理.先天因果初始化服务.h"
#include "业务/新_外设世界树类.h"
#include "业务/接收需求认知.服务.h"
#include "领域/语言含义图.数据.h"

#include "领域/新_本能函数管理类.h"
#include "领域/新_方法类.h"
#include "领域/新_自然语言世界树类.h"
#include "业务/服务值结算.本能运行服务.h"
#include "业务/服务对象.本能运行服务.h"
#include "业务/服务需求.本能运行服务.h"
#include "业务/字段询问.语言服务.h"
#include "业务/词条归类.事实服务.h"
#include "业务/语言补全.运行服务.h"
#include "业务/字段询问.本能运行服务.h"
#include "业务/语言场景写回.本能运行服务.h"
#include "业务/模拟学习.服务.h"
#include "业务/模拟任务需求来源.服务.h"
#include "业务/执行场景采集.服务.h"
#include "业务/方法问题学习.服务.h"
#include "业务/服务要求.结构化.h"
#include "业务/服务对象需求.发布服务.h"
#include "业务/动作概念.方法投影服务.h"
#include "业务/因果.任务筹办服务.h"
#include "领域/新_模拟世界类.h"
#include "核心/日志系统.h"
#include "核心/容错检查.h"
#include "界面/宿主.网页对话.h"

#include <chrono>

#ifndef HY_EGO_STARTUP_TIMING
#define HY_EGO_STARTUP_TIMING 0
#endif
#include <condition_variable>
#include <ctime>
#include <algorithm>
#include <array>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stop_token>
#include <string>
#include <thread>
#include <utility>
#include <variant>
#include <vector>

namespace 海中鱼巣 {
namespace 普通应用装配内部 {

#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
static void 记录装配停止阶段(const wchar_t* 阶段) noexcept {
    try {
        (void)记录调试日志(L"MAINLINE_PHASE", L"普通应用停止", 阶段,
            L"仅观察原停止调用前后，不读取快照或改变停止合同。");
    } catch (...) {}
}
#endif

static const wchar_t* 启动观察状态名(新任务管理操作状态 状态) noexcept {
    switch (状态) {
    case 新任务管理操作状态::已完成: return L"已完成";
    case 新任务管理操作状态::无变化: return L"无变化";
    case 新任务管理操作状态::入口拒绝: return L"入口拒绝";
    case 新任务管理操作状态::已在运行: return L"已在运行";
    case 新任务管理操作状态::尚未启动: return L"尚未启动";
    case 新任务管理操作状态::等待超时: return L"等待超时";
    case 新任务管理操作状态::不能等待自身: return L"不能等待自身";
    case 新任务管理操作状态::服务未就绪: return L"服务未就绪";
    case 新任务管理操作状态::所有者冲突: return L"所有者冲突";
    case 新任务管理操作状态::资源失败: return L"资源失败";
    case 新任务管理操作状态::内部错误: return L"内部错误";
    }
    return L"未知枚举值";
}

static const wchar_t* 启动观察状态名(新自我线程操作状态 状态) noexcept {
    switch (状态) {
    case 新自我线程操作状态::已完成: return L"已完成";
    case 新自我线程操作状态::无变化: return L"无变化";
    case 新自我线程操作状态::入口拒绝: return L"入口拒绝";
    case 新自我线程操作状态::初始化请求不完整: return L"初始化请求不完整";
    case 新自我线程操作状态::世界树初始化失败: return L"世界树初始化失败";
    case 新自我线程操作状态::自我所在场景失败: return L"自我所在场景失败";
    case 新自我线程操作状态::自我建立失败: return L"自我建立失败";
    case 新自我线程操作状态::唯一自我冲突: return L"唯一自我冲突";
    case 新自我线程操作状态::尚未初始化: return L"尚未初始化";
    case 新自我线程操作状态::尚未启动: return L"尚未启动";
    case 新自我线程操作状态::已在运行: return L"已在运行";
    case 新自我线程操作状态::等待超时: return L"等待超时";
    case 新自我线程操作状态::不能等待自身: return L"不能等待自身";
    case 新自我线程操作状态::结构不一致: return L"结构不一致";
    case 新自我线程操作状态::资源失败: return L"资源失败";
    case 新自我线程操作状态::内部错误: return L"内部错误";
    case 新自我线程操作状态::本能双根初始化失败: return L"本能双根初始化失败";
    case 新自我线程操作状态::需求树初始化失败: return L"需求树初始化失败";
    case 新自我线程操作状态::方法根初始化失败: return L"方法根初始化失败";
    case 新自我线程操作状态::消息配置不合法: return L"消息配置不合法";
    case 新自我线程操作状态::服务一级需求初始化失败: return L"服务一级需求初始化失败";
    case 新自我线程操作状态::服务对象需求初始化失败: return L"服务对象需求初始化失败";
    }
    return L"未知枚举值";
}

template<class 线程结果>
static void 记录治理线程启动返回(const wchar_t* 线程名, const 线程结果& 结果,
    std::uint64_t 等待毫秒, std::chrono::steady_clock::time_point 起点) noexcept {
    try {
        std::wostringstream 文本;
        文本 << L"线程=" << 线程名 << L"；启动操作状态=" << 启动观察状态名(结果.状态)
            << L"（" << static_cast<int>(结果.状态) << L"）；返回生命周期="
            << static_cast<int>(结果.生命周期) << L"；配置进入等待毫秒=" << 等待毫秒
            << L"；调用耗时毫秒=" << std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - 起点).count();
        (void)记录运行日志(L"普通应用", L"治理线程启动返回", 文本.str());
    } catch (...) { /* 不让观察改变原启动判断和失败回收。 */ }
}

struct 普通控制快照缓存 final {
    std::mutex 互斥;
    std::condition_variable_any 唤醒;
    网页控制快照状态 状态 = 网页控制快照状态::采集中;
    std::shared_ptr<const std::string> 正文;
    std::uint64_t 序号 = 0;
    std::chrono::steady_clock::time_point 完成时间{};
    bool 正在采集 = false;
    // 首条正式入站后再放行只读采集，避免与 /api/messages 争用共享结构锁饿死单线程 accept。
    std::atomic<bool> 入站后允许采集{false};
};
struct 普通控制采集结果 final {
    std::string JSON;
    bool 完整 = false;
};
struct 普通控制采集取消 final {};
using 普通控制采集可停止结果 = std::variant<普通控制采集结果, 普通控制采集取消>;

void 检查普通控制采集停止(std::stop_token 停止) {
    if (停止.stop_requested()) throw 普通控制采集取消{};
}
struct 普通应用上下文;
void 停止普通控制采集(普通应用上下文& 当前);


void 记录普通装配观察(const 普通应用装配结果& 结果) noexcept {
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
    输出新需求启动观察();
#endif
    try {
        std::wostringstream 文本;
        文本 << L"普通应用装配返回；状态=" << static_cast<int>(结果.状态)
            << L"；自我=";
        if (结果.自我 && 结果.自我->自我存在) 文本 << 结果.自我->自我存在->值;
        else 文本 << L"无";
        文本 << L"；结算需求=";
        if (结果.自我 && 结果.自我->完成服务需求) 文本 << 结果.自我->完成服务需求->值;
        else 文本 << L"无";
        文本 << L"；服务任务类型=";
        if (结果.服务任务特征概念节点) 文本 << 结果.服务任务特征概念节点->值;
        else 文本 << L"无";
        文本 << L"；服务任务特征=";
        if (结果.服务任务特征节点) 文本 << 结果.服务任务特征节点->值;
        else 文本 << L"无";
        文本 << L"；任务管理绑定状态=";
        if (结果.任务管理绑定) {
            文本 << static_cast<int>(结果.任务管理绑定->状态)
                << L"；自我线程生命周期=" << static_cast<int>(结果.任务管理绑定->生命周期);
        } else 文本 << L"未调用";
        (void)记录运行日志(L"普通应用", L"构造普通应用上下文", 文本.str());
    } catch (...) {
        // 仅为人读投影，不改变装配返回值。
    }
}

void 报告结算工程异常(std::string_view 错误) noexcept {
    // 工程诊断只复制现有材料；独立诊断线程记录、弹窗并终止，不恢复治理。
    try {
        const std::string 副本(错误);
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务值结算",
            L"结算执行链发生工程异常，请修复后重新启动。", 副本.c_str());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务值结算",
            L"结算工程诊断副本建立失败。");
    }
}

struct 普通应用上下文 final {
    struct 服务任务同步清理 final {
        新_任务类& 任务服务;
        const void* 所有者;
        ~服务任务同步清理() {
            (void)任务服务.解除服务任务集合同步(所有者);
        }
    };
    struct 任务成员端口清理 final {
        概念_特征类& 特征概念服务;
        新_特征类& 特征服务;
        新_需求类& 需求服务;
        服务对象需求发布服务& 需求发布;
        新_方法类& 方法服务;
        ~任务成员端口清理() {
            if (!特征服务.装配正式概念引用读取接口(nullptr, {}))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收正式概念引用端口", L"线程join后仍有在途特征更新，不能释放概念读取端口。");
            方法服务.装配正式信息参数读取接口(nullptr);
            需求服务.装配引用内容目标读取接口(nullptr);
            if(!需求服务.装配新增成员读取接口(nullptr))
                工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"回收新增成员读取端口",L"join后端口解绑失败。");
            需求发布.关闭();
            特征概念服务.装配需求成员读取接口(nullptr);
            特征服务.装配需求引用读取接口(nullptr);
            特征概念服务.装配任务成员读取接口(nullptr);
            需求服务.装配服务要求确认读取接口(nullptr);
            需求服务.装配部分服务要求读取接口(nullptr);
            需求服务.装配接收认知语言读取接口(nullptr);
        }
    };
    struct 需求任务工厂清理 final {
        新_任务类& 任务服务;
        std::shared_ptr<服务对象新需求本能运行服务>& 运行服务;
        bool 已绑定 = false;
        ~需求任务工厂清理() {
            if (!运行服务) return;
            if (已绑定 && !任务服务.卸载本能需求任务参数工厂(*运行服务))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收需求任务参数工厂", L"join后工厂所有者不一致。");
            // 初始化失败也只关闭本app对象；注册表的精确所有者检查保护其它端口。
            (void)运行服务->关闭();
        }
    };
    struct 语言写回端口清理 final {
        std::shared_ptr<语言场景写回本能运行服务>& 运行;
        bool 已绑定=false;
        ~语言写回端口清理(){if(!运行)return;运行->关闭();
            if(已绑定&&新_本能函数管理类::关闭语言场景写回端口(*运行)!=新本能函数端口装配状态::已关闭)
                工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"回收语言场景写回端口",L"join后端口所有者不一致。");
            运行.reset();}
    };
    struct 字段询问端口清理 final {
        新_方法类& 方法;
        模拟学习服务& 学习;
        新_模拟世界类& 模拟;
        std::shared_ptr<字段询问本能运行服务>& 运行;
        std::shared_ptr<字段询问语言服务>& 语言;
        bool 方法已绑定=false, 登记已绑定=false;
        ~字段询问端口清理() {
            // 声明顺序保证self/manager及worker的最终join已经完成。
            if (运行) {
                运行->关闭();
                if (方法已绑定 && !方法.解除字段询问运行接口(*运行))
                    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"回收字段询问运行接口",L"方法owner与运行接口所有者不一致。");
                if (登记已绑定 && 新_本能函数管理类::关闭字段询问端口(*运行)!=新本能函数端口装配状态::已关闭)
                    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"回收字段询问登记端口",L"登记端口所有者不一致。");
            }
            try {
                学习.配置字段询问语言({},{});
                模拟.释放有序样例();
                模拟.设置字段询问语言({},{});
                运行.reset();语言.reset();
            } catch (const std::exception& e) {
                工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"回收字段询问依赖",L"线程join后字段询问依赖未能回收。",e.what());
            } catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"回收字段询问依赖",L"线程join后字段询问依赖回收发生未知错误。");
            }
        }
    };
    explicit 普通应用上下文(const 普通应用配置& 运行配置)
        : 配置(运行配置), 世界事实(世界树.获取事实服务引用()), 外设世界树(世界树),
          网页外设模块(外设世界树),
          治理二次特征(世界事实.二次特征概念服务,
              世界事实.词服务, 世界事实.名称关系服务),
          自然语言世界树(
              世界事实.特征概念服务, 世界事实.存在概念服务,
              世界事实.特征服务, 世界事实.存在服务,
              世界事实.状态服务, 世界事实.动态服务),
          服务要求(自然语言世界树, 世界事实.存在服务, 世界事实.特征值服务,
              世界事实.存在概念服务, 世界事实.特征概念服务,
              世界事实.二次特征概念服务),
          需求服务(世界事实.特征值服务, 世界事实.存在服务,
              世界事实.特征服务, 世界事实.特征概念服务,
              世界事实.二次特征概念服务, 世界事实.存在概念服务),
          接收认知(外设世界树,自然语言世界树,世界事实.语言服务,世界事实.词服务,
              世界事实.名称关系服务,世界事实.存在概念服务,世界事实.特征概念服务,
              世界事实.二次特征概念服务,世界事实.存在服务,世界事实.特征值服务,
              世界事实.特征服务,世界事实.场景服务,需求服务),
          服务对象需求发布(世界树, 需求服务),
          方法服务(世界事实.存在服务, 世界事实.场景服务,
              世界事实.特征值服务, 世界事实.特征服务,
              世界事实.存在概念服务, 世界事实.特征概念服务, 世界事实.状态服务),
          问题模拟世界(方法服务),
          执行场景采集(世界事实.场景服务, 世界事实.存在服务,
              世界事实.特征服务, 世界事实.特征值服务, 世界事实.状态服务,
              世界事实.动态服务, 世界事实.特征概念服务, 世界事实.存在概念服务),
          任务服务(需求服务, 方法服务, 世界事实.特征值服务,
              世界事实.特征服务, 世界事实.特征概念服务,
              世界事实.状态服务, 世界事实.场景服务,
              世界事实.存在服务, 世界事实.二次特征概念服务,
              世界事实.存在概念服务),
          治理先天初始化(世界事实.因果服务, 世界事实.动态服务,
              世界事实.存在概念服务, 世界事实.特征概念服务),
          动作投影(方法服务, 任务服务, 世界事实.存在概念服务, 世界事实.特征概念服务,
              &治理先天初始化),
          内部治理世界(世界事实.特征值服务, 世界事实.特征概念服务,
              世界事实.特征服务, 世界事实.存在概念服务, 世界事实.动态概念服务,
              世界事实.状态服务, 世界事实.存在服务, 世界事实.动态服务, 世界事实.场景服务),
          因果反查(世界事实.因果服务, 世界事实.特征服务, 世界事实.特征概念服务,
              世界事实.存在概念服务, 世界事实.动态概念服务, 世界事实.动态服务,
              世界事实.存在服务, 世界事实.场景服务),
          因果筹办(因果反查, 世界事实.因果服务, 方法服务, 世界事实.动态概念服务,
              世界事实.二次特征概念服务, 世界事实.特征概念服务, 世界事实.特征值服务),
          服务结算运行(世界树, 方法服务, 任务服务, 需求服务,
              世界事实.场景服务, 世界事实.动态服务, 世界事实.特征服务,
              世界事实.特征概念服务, 世界事实.二次特征概念服务,
              内部治理世界, &报告结算工程异常),
          模拟任务需求来源(任务服务,需求服务,服务要求,自然语言世界树),
          模拟学习(方法服务, 世界事实.特征服务, 世界事实.特征概念服务, 世界事实.二次特征概念服务,模拟任务需求来源),
          问题学习(问题模拟世界),
          同步清理{任务服务, this},
          成员端口清理{世界事实.特征概念服务, 世界事实.特征服务, 需求服务, 服务对象需求发布, 方法服务},
          需求工厂清理{任务服务, 服务对象新需求运行},
          字段询问清理{方法服务, 模拟学习, 问题模拟世界, 字段询问运行, 字段询问语言},
          语言写回清理{语言写回运行},
          任务管理线程(任务服务),
          自我线程(世界树, 需求服务, 方法服务, 任务服务) {
        // 构造只注入窄读取端口；真实动作时才读取方法声明并形成动作C。
        世界事实.动态服务.设置动作方法定义提供者(动作投影, 世界事实.特征服务);
        if (!内部治理世界.装配任务读取(任务服务) || !任务服务.装配内部治理世界(内部治理世界)
            || !内部治理世界.装配方法读取(方法服务) || !方法服务.装配内部治理世界(内部治理世界))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"装配治理世界", L"治理世界与任务/方法owner唯一接口绑定失败。");
        任务服务.配置因果筹办读取(因果筹办);
    }

    ~普通应用上下文() {
        停止普通控制采集(*this);
        (void)自我线程.请求停止();
        (void)任务管理线程.请求停止();
        (void)自我线程.等待停止(配置.线程停止回收等待毫秒);
        (void)任务管理线程.等待停止(配置.线程停止回收等待毫秒);
        if (服务对象运行) (void)服务对象运行->关闭();
        if (连接运行) 连接运行->关闭();
        网页外设模块.关闭();
        // 有界等待可能超时；成员逆序析构完成最终join后才清理回调及外部绑定。
    }

    普通应用配置 配置;
    新_世界树类 世界树;
    新世界事实服务引用 世界事实;
    新_外设世界树类 外设世界树;
    网页外设运行模块 网页外设模块;
    治理二次特征应用服务 治理二次特征;
    新_自然语言世界树类 自然语言世界树;
    结构化服务要求服务 服务要求;
    新_需求类 需求服务;
    接收需求认知服务 接收认知;
    服务对象需求发布服务 服务对象需求发布;
    新_方法类 方法服务;
    新_模拟世界类 问题模拟世界;
    执行场景采集服务 执行场景采集;
    新_任务类 任务服务;
    内部治理先天因果初始化服务 治理先天初始化;
    动作方法投影服务 动作投影;
    新_内部治理世界类 内部治理世界;
    因果反向查询服务 因果反查;
    因果任务筹办服务 因果筹办;
    服务值结算本能运行服务 服务结算运行;
    std::shared_ptr<连接本能运行服务> 连接运行;
    std::shared_ptr<服务对象本能运行服务> 服务对象运行;
    std::shared_ptr<服务对象新需求本能运行服务> 服务对象新需求运行;
    std::shared_ptr<词条归类事实服务> 词条归类事实;
    std::shared_ptr<语言补全运行服务> 语言补全运行;
    std::shared_ptr<字段询问语言服务> 字段询问语言;
    std::shared_ptr<字段询问本能运行服务> 字段询问运行;
    std::shared_ptr<语言场景写回本能运行服务> 语言写回运行;
    模拟任务需求来源服务 模拟任务需求来源;
    模拟学习服务 模拟学习;
    方法问题学习服务 问题学习;
    std::optional<新自我线程初始化请求> 自我初始化请求;
    std::optional<普通应用装配结果> 装配结果;
    std::optional<初始化概念名称结果> 初始概念名称;
    std::optional<初始化结构化子概念结果> 结构化子概念;
    std::optional<初始化语言知识结果> 语言知识;
    std::optional<初始化指代种子结果> 指代种子;
    bool 治理线程已启动 = false;
    std::optional<稳定编码> 服务任务自我存在节点;
    std::optional<稳定编码> 服务任务特征概念节点;
    std::optional<稳定编码> 服务任务特征节点;
    bool 服务任务集合写后失败 = false;

    // 第一权限人是世界树根场景中的实际存在。以下可选身份只用于在一次
    // 装配失败后从已完成步骤继续，不构成第二份事实来源。
    std::optional<稳定编码> 中文语言节点;
    std::optional<稳定编码> 登录名词条;
    std::optional<稳定编码> 姓名词条;
    std::optional<稳定编码> 权限角色词条;
    std::optional<稳定编码> 登录名值词条;
    std::optional<稳定编码> 姓名值词条;
    std::optional<稳定编码> 第一权限人值词条;
    std::optional<稳定编码> 登录名值节点;
    std::optional<稳定编码> 姓名值节点;
    std::optional<稳定编码> 第一权限人值节点;
    std::optional<稳定编码> 登录名特征概念节点;
    std::optional<稳定编码> 姓名特征概念节点;
    std::optional<稳定编码> 性别特征概念节点;
    std::optional<稳定编码> 第一权限人性别特征节点;
    std::optional<稳定编码> 权限角色特征概念节点;
    std::optional<稳定编码> 登录名精确概念节点;
    std::optional<稳定编码> 姓名精确概念节点;
    std::optional<稳定编码> 第一权限人精确概念节点;
    std::vector<稳定编码> 第一权限人名称关系;
    std::optional<稳定编码> 第一权限人存在节点;
    std::optional<稳定编码> 第一权限人专属存在概念节点;
    std::optional<稳定编码> 人类词条;
    std::optional<稳定编码> 人类名称关系;
    std::optional<稳定编码> 人类存在概念节点;
    std::optional<稳定编码> 语言语义差异二次特征类型根;
    std::optional<稳定编码> 无法确认结果概念;
    std::optional<稳定编码> 无法确认词条;
    std::optional<稳定编码> 无法确认名称关系;
    std::optional<稳定编码> 确认结果概念;
    std::optional<稳定编码> 确认词条;
    std::optional<稳定编码> 确认名称关系;

    // 公开会话及其可信绑定仍由宿主生命周期管理，不由外设原文推断身份。
    std::optional<std::string> 活跃网页会话标识;
    std::optional<稳定编码> 网页会话绑定人类;
    // 旧现实窗口依赖的语言入口保留显式未绑定，不以外设E冒充现实身份。

    // self/manager及其worker最终join → 解绑 → 捕获状态 → 领域owner。
    // 所有回调使用的状态必须声明在线程和清理器之前。
    服务任务同步清理 同步清理;
    任务成员端口清理 成员端口清理;
    需求任务工厂清理 需求工厂清理;
    字段询问端口清理 字段询问清理;
    语言写回端口清理 语言写回清理;
    新_任务管理线程类 任务管理线程;
    新_自我线程类 自我线程;
    // 必须先join只读采集线程，再销毁它借用的所有领域owner。
    std::shared_ptr<普通控制快照缓存> 控制缓存;
    std::jthread 控制采集线程;
};

std::mutex 上下文锁;
std::mutex 治理启停锁;
std::unique_ptr<普通应用上下文> 上下文;

// 此处只序列化公开读取的值式副本，不把显示材料用于治理。
std::string 控制JSON字符串(std::string_view 文本, std::stop_token 停止) {
    检查普通控制采集停止(停止);
    constexpr char 十六进制[] = "0123456789abcdef";
    std::string 结果 = "\"";
    for (const unsigned char 字符 : 文本) {
        检查普通控制采集停止(停止);
        if (字符 == '"' || 字符 == '\\') {
            结果 += '\\'; 结果 += static_cast<char>(字符);
        } else if (字符 < 0x20) {
            结果 += "\\u00"; 结果 += 十六进制[字符 >> 4]; 结果 += 十六进制[字符 & 15];
        } else 结果 += static_cast<char>(字符);
    }
    结果 += '"';
    检查普通控制采集停止(停止);
    return 结果;
}
std::string 控制JSON字符串(std::string_view 文本) {
    return 控制JSON字符串(文本, {});
}
std::string 控制编码(std::optional<稳定编码> 节点) {
    return 节点 ? 控制JSON字符串(std::to_string(节点->值)) : "null";
}
std::string 控制计数(std::uint64_t 值) { return 控制JSON字符串(std::to_string(值)); }
const char* 控制布尔(bool 值) noexcept { return 值 ? "true" : "false"; }
std::string 控制UTC时间() {
    const auto 时间 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm UTC{};
    if (gmtime_s(&UTC, &时间) != 0) throw std::runtime_error("control UTC conversion failed");
    char 文本[32]{};
    if (std::strftime(文本, sizeof(文本), "%Y-%m-%dT%H:%M:%SZ", &UTC) == 0)
        throw std::runtime_error("control UTC formatting failed");
    return 控制JSON字符串(文本);
}

#define 控制名称项(项) case E::项: return #项
const char* 控制名称(新任务状态 值) noexcept {
    using E = 新任务状态; switch (值) {
        控制名称项(已建立); 控制名称项(待查找方法); 控制名称项(待检查条件);
        控制名称项(待执行); 控制名称项(执行中); 控制名称项(待影响评估);
        控制名称项(等待自我决议); 控制名称项(等待条件); 控制名称项(已完成);
        控制名称项(已失败); 控制名称项(已取消); 控制名称项(已结算);
        控制名称项(未执行但目标已满足);
    } return "未知任务状态";
}
const char* 控制名称(新任务等待原因 值) noexcept {
    using E = 新任务等待原因; switch (值) {
        控制名称项(无); 控制名称项(首次关联待许可); 控制名称项(尚无方法);
        控制名称项(候选条件未齐); 控制名称项(需求材料未到); 控制名称项(执行后等待);
        控制名称项(同次执行比较材料不足); 控制名称项(需求信息待完善);
        控制名称项(未分类); 控制名称项(需核查);
    } return "未知等待原因";
}
const char* 控制名称(新任务调度项状态 值) noexcept {
    using E = 新任务调度项状态; switch (值) {
        控制名称项(普通可筹办); 控制名称项(业务等待); 控制名称项(旧执行待结果);
        控制名称项(后继待决议); 控制名称项(完成待结算); 控制名称项(终结); 控制名称项(结构不一致);
    } return "未知调度状态";
}
const char* 控制名称(新自我线程生命周期 值) noexcept {
    using E = 新自我线程生命周期; switch (值) {
        控制名称项(未初始化); 控制名称项(已初始化); 控制名称项(启动中);
        控制名称项(等待治理消息); 控制名称项(正在停止); 控制名称项(已停止);
        控制名称项(启动失败); 控制名称项(内部错误); 控制名称项(正在处理消息);
    } return "未知自我线程状态";
}
const char* 控制名称(新任务管理生命周期 值) noexcept {
    using E = 新任务管理生命周期; switch (值) {
        控制名称项(未启动); 控制名称项(启动中); 控制名称项(等待消息); 控制名称项(处理消息);
        控制名称项(停止中); 控制名称项(已停止); 控制名称项(启动失败); 控制名称项(内部错误);
    } return "未知任务管理状态";
}
const char* 控制名称(新需求活动状态 值) noexcept {
    using E = 新需求活动状态; switch (值) {
        控制名称项(活动); 控制名称项(等待); 控制名称项(已满足); 控制名称项(失活);
    } return "未知需求活动状态";
}
const char* 控制名称(新需求执行域 值) noexcept {
    using E = 新需求执行域; switch (值) { 控制名称项(自我执行); 控制名称项(被认知); }
    return "未知执行域";
}
const char* 控制名称(新需求来源类型 值) noexcept {
    using E = 新需求来源类型; switch (值) {
        控制名称项(本能定义); 控制名称项(任务子目标); 控制名称项(外部服务要求); 控制名称项(行动请求);
    } return "未知需求来源";
}
const char* 控制名称(新任务方法选择状态 值) noexcept {
    using E = 新任务方法选择状态; switch (值) {
        控制名称项(已选定); 控制名称项(等待条件); 控制名称项(匹配不唯一);
        控制名称项(目标已满足); 控制名称项(无适用结果); 控制名称项(幅度材料不足);
        控制名称项(结果定义暂不支持); 控制名称项(任务状态已变化); 控制名称项(实例未建立);
        控制名称项(结构不一致); 控制名称项(资源失败);
    } return "未知方法选择状态";
}
const char* 控制名称(新任务治理条件筹办状态 值) noexcept {
    using E = 新任务治理条件筹办状态; switch (值) {
        控制名称项(已完成); 控制名称项(任务状态已变化); 控制名称项(尚未初始化);
        控制名称项(任务不存在); 控制名称项(候选材料不存在); 控制名称项(结构不一致); 控制名称项(资源失败);
    } return "未知条件筹办状态";
}
const char* 控制名称(新任务候选方法筹办状态 值) noexcept {
    using E = 新任务候选方法筹办状态; switch (值) {
        控制名称项(条件具备); 控制名称项(条件不具备); 控制名称项(材料未到);
        控制名称项(参数缺失); 控制名称项(结构缺口); 控制名称项(资源失败);
    } return "未知候选筹办状态";
}
const char* 控制名称(新任务方法条件缺口种类 值) noexcept {
    using E = 新任务方法条件缺口种类; switch (值) {
        控制名称项(参数对象未取得); 控制名称项(参数匹配不唯一); 控制名称项(参数特征值未取得);
        控制名称项(条件值不满足); 控制名称项(参数特征类型未取得); 控制名称项(参数概念未确认);
    } return "未知条件缺口";
}
const char* 控制名称(新任务筹办处理状态 值) noexcept {
    using E = 新任务筹办处理状态; switch (值) {
        控制名称项(有候选并进入条件检查); 控制名称项(完整无候选并进入等待);
        控制名称项(条件筹办已完成); 控制名称项(任务状态已变化); 控制名称项(工作未完成);
        控制名称项(独立读回失败); 控制名称项(状态推进失败); 控制名称项(停止取消);
        控制名称项(内部错误); 控制名称项(组织能力暂不支持); 控制名称项(复合目标需自我确认);
        控制名称项(内容结果定位未支持); 控制名称项(未执行目标已满足);
    } return "未知筹办状态";
}
const char* 控制名称(新学习运行异常原因 值) noexcept {
    using E = 新学习运行异常原因; switch (值) {
        控制名称项(停止取消); 控制名称项(输入不合法); 控制名称项(资源不足);
        控制名称项(结构不一致); 控制名称项(本能运行异常); 控制名称项(发布结果未知);
        控制名称项(内部错误); 控制名称项(规格未支持); 控制名称项(定义前提已变化);
    } return "未知学习异常";
}
const char* 控制名称(新任务执行状态 值) noexcept {
    using E = 新任务执行状态; switch (值) {
        控制名称项(执行中); 控制名称项(已结束待评估); 控制名称项(已评估);
    } return "未知执行状态";
}
const char* 控制名称(新任务调度读取状态 值) noexcept {
    using E = 新任务调度读取状态; switch (值) {
        控制名称项(已读取); 控制名称项(尚未初始化); 控制名称项(任务不存在);
        控制名称项(结构不一致); 控制名称项(资源失败);
    } return "未知任务读取状态";
}
const char* 控制名称(新需求操作状态 值) noexcept {
    using E = 新需求操作状态; switch (值) {
        控制名称项(已建立); 控制名称项(已复用); 控制名称项(已找到); 控制名称项(已修改);
        控制名称项(无变化); 控制名称项(入口拒绝); 控制名称项(尚未初始化); 控制名称项(需求不存在);
        控制名称项(父需求不合法); 控制名称项(目标存在不合法); 控制名称项(目标形式不合法);
        控制名称项(目标特征类型不合法); 控制名称项(目标值域不合法); 控制名称项(比较关系不合法);
        控制名称项(目标方向不合法); 控制名称项(双根定义冲突); 控制名称项(任务来源未接通);
        控制名称项(会形成环); 控制名称项(结构不一致); 控制名称项(资源失败); 控制名称项(权重不合法);
        控制名称项(执行域不适用); 控制名称项(认知来源不合法); 控制名称项(同来源目标冲突);
        控制名称项(需重新确认); 控制名称项(前提已变化); 控制名称项(目标内容依赖未就绪); 控制名称项(目标内容未支持);
    } return "未知需求读取状态";
}
const char* 控制名称(新学习线程状态 值) noexcept {
    using E = 新学习线程状态; switch (值) {
        控制名称项(未启动); 控制名称项(等待工作); 控制名称项(正在学习);
        控制名称项(停止中); 控制名称项(已停止); 控制名称项(内部错误);
    } return "未知学习线程状态";
}
const char* 控制名称(新学习工作快照读取状态 值) noexcept {
    using E = 新学习工作快照读取状态; switch (值) {
        控制名称项(已取得); 控制名称项(尚未建立);
        控制名称项(资源失败); 控制名称项(内部错误);
    } return "未知学习读取状态";
}
const char* 控制名称(新学习工作阶段 值) noexcept {
    using E = 新学习工作阶段; switch (值) {
        控制名称项(排队中); 控制名称项(执行中);
        case E::完成待确认: return "已结束待确认";
    } return "未知学习工作阶段";
}
const char* 控制名称(新方法目录读取状态 值) noexcept {
    using E = 新方法目录读取状态; switch (值) {
        控制名称项(已读取); 控制名称项(尚未初始化); 控制名称项(结构不一致);
        控制名称项(资源失败); 控制名称项(内部错误);
    } return "未知方法目录读取状态";
}
const char* 控制名称(新方法作用域 值) noexcept {
    using E = 新方法作用域; switch (值) { 控制名称项(内部治理); 控制名称项(普通世界); }
    return "未知方法作用域";
}
const char* 控制名称(新方法实现方式 值) noexcept {
    using E = 新方法实现方式; switch (值) { 控制名称项(本能); 控制名称项(组合); }
    return "未知方法实现方式";
}
const char* 控制名称(新方法行为 值) noexcept {
    using E = 新方法行为; switch (值) { 控制名称项(读取); 控制名称项(改变); }
    return "未知方法行为";
}
const char* 控制名称(新方法参数信息类型 值) noexcept {
    using E = 新方法参数信息类型; switch (值) { 控制名称项(存在); 控制名称项(特征); }
    return "未知参数信息类型";
}
const char* 控制名称(新方法参数对象种类 值) noexcept {
    using E = 新方法参数对象种类; switch (值) {
        控制名称项(普通世界); 控制名称项(任务虚拟存在); 控制名称项(服务要求来源人类);
        控制名称项(服务要求目标存在); 控制名称项(服务要求目标特征类型);
        控制名称项(服务要求目标定义); 控制名称项(服务要求确认);
    } return "未知旧规格对象种类";
}
#undef 控制名称项

class 普通控制投影 final {
public:
    explicit 普通控制投影(普通应用上下文& 当前, std::stop_token 停止)
        : 当前_(当前), 停止_(停止) {}
    std::string 概念引用(std::optional<稳定编码> 节点) {
        检查停止();
        if (!节点) return "null";
        auto 位置 = 名称_.find(*节点);
        if (位置 == 名称_.end()) {
            检查停止();
            const auto 列表 = 当前_.世界事实.名称关系服务.查询概念名称(*节点);
            检查停止();
            std::set<std::string> 词面;
            bool 完整 = 列表.状态 == 概念名称关系查询状态::已找到
                || 列表.状态 == 概念名称关系查询状态::未找到;
            if (列表.状态 == 概念名称关系查询状态::已找到) {
                for (const auto 关系节点 : 列表.名称关系节点组) {
                    检查停止();
                    const auto 关系 = 当前_.世界事实.名称关系服务.获取名称关系(关系节点);
                    检查停止();
                    if (关系.状态 != 概念名称关系查询状态::已找到 || !关系.信息) { 完整 = false; break; }
                    检查停止();
                    const auto 词 = 当前_.世界事实.词服务.获取词条(关系.信息->词条节点);
                    检查停止();
                    if (词.状态 != 新词条查询状态::已找到 || !词.信息) { 完整 = false; break; }
                    词面.insert(词.信息->规范化词面);
                }
            }
            std::string 名称;
            if (完整) for (const auto& 词 : 词面) {
                检查停止();
                if (!名称.empty()) 名称 += "／";
                名称 += 词;
            }
            if (名称.empty()) 名称 = 完整 ? "概念（未命名）" : "概念（名称暂不可用）";
            位置 = 名称_.emplace(*节点, std::move(名称)).first;
        }
        auto JSON = "{\"id\":" + 控制编码(节点) + ",\"label\":" + 字符串(位置->second) + "}";
        检查停止();
        return JSON;
    }
    std::string 存在引用(std::optional<稳定编码> 节点) {
        检查停止();
        if (!节点) return "null";
        // 名称读取独立发生，不宣称与需求/任务属于同一原子快照。
        检查停止();
        const auto 存在 = 当前_.世界事实.存在服务.获取存在(*节点);
        检查停止();
        std::string 名称 = "存在（名称暂不可用）";
        if (存在) {
            const auto 引用 = 概念引用(存在->专属存在概念节点);
            const auto& 已有名称 = 名称_.at(存在->专属存在概念节点);
            auto JSON = "{\"id\":" + 控制编码(节点) + ",\"label\":"
                + 字符串(已有名称 == "概念（未命名）" ? "存在（未命名）" : 已有名称)
                + ",\"concept\":" + 引用 + "}";
            检查停止();
            return JSON;
        }
        auto JSON = "{\"id\":" + 控制编码(节点) + ",\"label\":" + 字符串(名称) + "}";
        检查停止();
        return JSON;
    }
    std::string 目标(std::optional<稳定编码> 存在, const 新需求目标定义& 定义) {
        检查停止();
        std::string 形式 = "未知目标形式";
        if (定义.目标形式 == 新需求目标形式::值) 形式 = "值";
        else if (定义.目标形式 == 新需求目标形式::变化方向) 形式 = "变化方向";
        std::string 方向 = "null";
        if (定义.目标变化方向) {
            方向 = 字符串(*定义.目标变化方向 == 新需求变化方向::增加 ? "增加"
                : *定义.目标变化方向 == 新需求变化方向::减少 ? "减少" : "未知方向");
        }
        std::string 路径 = "[";
        if (定义.引用内容定位) for (const auto 项 : 定义.引用内容定位->正式内容路径) {
            检查停止();
            if (路径.size() != 1) 路径 += ','; 路径 += 概念引用(项);
        }
        路径 += ']';
        std::string 见证="null";
        if(定义.新增成员来源见证){const auto& c=*定义.新增成员来源见证;
            见证="{\"sourceFeatureType\":"+概念引用(c.要求.源特征类型概念节点)
                +",\"baselineMaterial\":"+控制编码(c.基准材料节点)
                +",\"specifiedHuman\":"+存在引用(c.要求.指定来源主体)
                +",\"specifiedDemand\":"+(c.要求.指定被认知需求 ? 控制编码(*c.要求.指定被认知需求) : "null")+"}";}
        检查停止();
        auto JSON = "{\"existence\":" + 存在引用(存在) + ",\"featureType\":" + 概念引用(定义.目标特征类型概念节点)
            + ",\"form\":" + 字符串(形式) + ",\"valueDomain\":" + 概念引用(定义.目标值域概念节点)
            + ",\"comparison\":" + 概念引用(定义.目标比较关系概念节点) + ",\"direction\":" + 方向
            + ",\"referencedDemand\":" + (定义.引用内容定位 ?
                "{\"id\":" + 控制编码(定义.引用内容定位->引用需求节点) + ",\"label\":\"需求\"}" : "null")
            + ",\"contentPath\":" + 路径 + ",\"newMemberSourceWitness\":"+见证+"}";
        检查停止();
        return JSON;
    }
    std::string 需求(const 新需求治理信息& D) {
        检查停止();
        std::string 来源 = D.来源类型 ? 字符串(控制名称(*D.来源类型)) : "null";
        std::string 请求来源 = "null";
        if (D.行动请求来源) {
            const auto& s = *D.行动请求来源;
            std::string 路径 = "[";
            for (const auto x : s.需求来源.键.根出现路径) {
                检查停止();
                if (路径.size() != 1) 路径 += ',';
                路径 += std::to_string(x);
            }
            请求来源 = "{\"messageId\":" + 控制编码(s.需求来源.键.接收消息节点)
                + ",\"meaningId\":" + 控制编码(s.需求来源.采用含义节点)
                + ",\"demandId\":" + 控制编码(s.被认知需求节点)
                + ",\"humanId\":" + 控制编码(s.来源人类节点)
                + ",\"addresseeId\":" + 控制编码(s.受请求者节点)
                + ",\"begin\":" + 字符串(std::to_string(s.需求来源.键.原文起点))
                + ",\"end\":" + 字符串(std::to_string(s.需求来源.键.原文终点))
                + ",\"path\":" + 路径 + "]}";
        }
        std::string 目标正文 = "null";
        if (D.完整目标) 目标正文 = 目标(D.完整目标->目标存在节点, D.完整目标->目标);
        else if (D.已知目标) {
            const auto& P = *D.已知目标;
            目标正文 = "{\"existence\":" + 存在引用(P.目标存在节点)
                + ",\"featureType\":" + 概念引用(P.目标特征类型概念节点)
                + ",\"form\":" + (P.目标形式 ? 字符串(*P.目标形式 == 新需求目标形式::值 ? "值" : "变化方向") : "null")
                + ",\"valueDomain\":" + 概念引用(P.目标值域概念节点)
                + ",\"comparison\":" + 概念引用(P.目标比较关系概念节点)
                + ",\"direction\":" + (P.目标变化方向 ? 字符串(*P.目标变化方向 == 新需求变化方向::增加 ? "增加" : "减少") : "null") + "}";
        }
        检查停止();
        auto JSON = "{\"id\":" + 控制编码(D.节点) + ",\"parentId\":" + 控制编码(D.父需求节点)
            + ",\"subject\":" + 存在引用(D.需求主体存在节点)
            + ",\"executionDomain\":" + 字符串(控制名称(D.执行域))
            + ",\"activity\":" + 字符串(控制名称(D.活动状态))
            + ",\"weight\":" + 字符串(std::to_string(D.权重))
            + ",\"sourceType\":" + 来源 + ",\"sourceTaskId\":" + 控制编码(D.来源承接节点)
            + ",\"decompositionSource\":" + (D.复合来源 ? "\"复合目标分解\"" : 来源)
            + ",\"childRelation\":\"关系未接入\",\"complete\":" + 控制布尔(D.完整目标.has_value())
            + ",\"target\":" + 目标正文 + ",\"actionRequestSource\":" + 请求来源 + "}";
        检查停止();
        return JSON;
    }
    std::string 条件(const std::optional<新任务治理条件筹办结果>& 结果) {
        检查停止();
        if (!结果) return "null";
        std::string 组 = "[";
        for (const auto& M : 结果->方法筹办组) {
            检查停止();
            if (组.size() != 1) 组 += ',';
            组 += "{\"methodId\":" + 控制编码(M.方法节点) + ",\"status\":" + 字符串(控制名称(M.状态)) + ",\"gaps\":[";
            bool 首项 = true;
            for (const auto& 缺口 : M.条件缺口组) {
                检查停止();
                if (!首项) 组 += ','; 首项 = false;
                组 += "{\"kind\":" + 字符串(控制名称(缺口.种类))
                    + ",\"position\":" + (缺口.参数规格.有序规格 ? 控制计数(缺口.参数规格.有序规格->序号) : "null")
                    + ",\"roleId\":" + (缺口.参数规格.有序规格 ? "null" : 控制编码(缺口.参数规格.参数角色节点))
                    + ",\"sourcePairId\":" + 控制编码(缺口.来源配对节点)
                    + ",\"conditionIndex\":" + (缺口.条件项序号 ? 控制计数(*缺口.条件项序号) : "null") + "}";
            }
            组 += "]}";
        }
        检查停止();
        auto JSON = "{\"status\":" + 字符串(控制名称(结果->状态))
            + ",\"complete\":" + 控制布尔(结果->候选已完整核验)
            + ",\"selection\":" + (结果->方法选择状态 ? 字符串(控制名称(*结果->方法选择状态)) : "null")
            + ",\"methods\":" + 组 + "]}";
        检查停止();
        return JSON;
    }
    std::string 参数规格(const std::vector<新方法参数规格>& 规格组) {
        检查停止();
        std::string 参数 = "[";
        for (const auto& P : 规格组) {
            检查停止();
            if (参数.size() != 1) 参数 += ',';
            if (P.有序规格) {
                参数 += "{\"format\":\"有序参数\",\"position\":" + 控制计数(P.有序规格->序号)
                    + ",\"informationType\":" + 字符串(控制名称(P.有序规格->信息类型))
                    + ",\"range\":" + 概念引用(P.有序规格->范围概念节点) + "}";
            } else {
                std::string 特征组 = "[";
                for (const auto& F : P.特征规格组) {
                    检查停止();
                    if (特征组.size() != 1) 特征组 += ',';
                    特征组 += "{\"featureType\":" + 概念引用(F.特征类型概念节点)
                        + ",\"valueDomain\":" + 概念引用(F.值域概念节点)
                        + ",\"secondaryConcepts\":" + 概念引用组(F.二次特征概念组) + "}";
                }
                const auto 集合 = P.集合成员约束
                    ? "{\"collectionRoleId\":" + 控制编码(P.集合成员约束->集合参数角色节点)
                        + ",\"featureType\":" + 概念引用(P.集合成员约束->集合特征类型概念节点) + "}"
                    : std::string("null");
                参数 += "{\"format\":\"兼容旧角色规格\",\"roleId\":" + 控制编码(P.参数角色节点)
                    + ",\"range\":" + 概念引用(P.存在概念节点) + ",\"requiresScene\":" + 控制布尔(P.要求场景)
                    + ",\"objectKind\":" + 字符串(控制名称(P.对象种类))
                    + ",\"features\":" + 特征组 + "],\"collectionConstraint\":" + 集合 + "}";
            }
        }
        参数 += ']';
        检查停止();
        return 参数;
    }
    std::string 概念引用组(const std::vector<稳定编码>& 组) {
        检查停止();
        std::string JSON = "[";
        for (const auto C : 组) {
            检查停止();
            if (JSON.size() != 1) JSON += ',';
            JSON += 概念引用(C);
        }
        JSON += ']';
        检查停止();
        return JSON;
    }
    std::string 方法头(const 新方法头定义& 头) {
        检查停止();
        auto JSON = "{\"existenceConcepts\":" + 概念引用组(头.作用存在概念组)
            + ",\"featureTypes\":" + 概念引用组(头.涉及特征类型概念组)
            + ",\"possibleResults\":" + 概念引用组(头.可能结果概念组) + "}";
        检查停止();
        return JSON;
    }
    std::string 方法(const 新方法信息& M) {
        检查停止();
        auto JSON = "{\"id\":" + 控制编码(M.节点) + ",\"label\":" + 字符串(M.本能函数登记名.value_or("方法（无登记名）"))
            + ",\"parameterCount\":" + 控制计数(M.参数规格组.size()) + ",\"pairCount\":" + 控制计数(M.条件结果组.size())
            + ",\"enabled\":" + 控制布尔(M.启用状态 == 新方法启用状态::启用) + ",\"parameters\":" + 参数规格(M.参数规格组) + "}";
        检查停止();
        return JSON;
    }
    普通控制采集结果 方法目录(const 新方法目录读取结果& 目录) {
        检查停止();
        const std::string 范围 = "正式方法根全部直接M成员，含停用/副本；配对仅计引用，未逐对核验；不是执行资格";
        if (目录.状态 != 新方法目录读取状态::已读取) {
            普通控制采集结果 结果{"{\"status\":\"暂不可用\",\"source\":\"方法owner轻量目录\",\"scope\":" + 字符串(范围)
                + ",\"reason\":" + 字符串(控制名称(目录.状态)) + ",\"root\":null,\"items\":null}", false};
            检查停止();
            return 结果;
        }
        std::string 项组 = "[";
        for (const auto& M : 目录.项组) {
            检查停止();
            if (项组.size() != 1) 项组 += ',';
            项组 += "{\"id\":" + 控制编码(M.节点) + ",\"label\":" + 字符串(M.本能函数登记名.value_or("方法（无登记名）"))
                + ",\"headId\":" + 控制编码(M.方法头节点) + ",\"head\":" + 方法头(M.方法头)
                + ",\"virtualExistence\":" + 控制编码(M.方法虚拟存在节点)
                + ",\"scope\":" + 字符串(控制名称(M.作用域))
                + ",\"implementation\":" + 字符串(控制名称(M.实现方式))
                + ",\"enabled\":" + 控制布尔(M.启用状态 == 新方法启用状态::启用)
                + ",\"behavior\":" + (M.行为 ? 字符串(控制名称(*M.行为)) : "null")
                + ",\"registrationName\":" + (M.本能函数登记名 ? 字符串(*M.本能函数登记名) : "null")
                + ",\"sourceMethodId\":" + 控制编码(M.来源方法节点) + ",\"instanceRootId\":" + 控制编码(M.实例根节点)
                + ",\"parentMethodId\":" + 控制编码(M.父代方法节点) + ",\"parameters\":" + 参数规格(M.参数规格组)
                + ",\"pairCount\":" + 控制计数(M.条件结果配对数量) + "}";
        }
        检查停止();
        普通控制采集结果 结果{"{\"status\":\"已取得\",\"source\":\"方法owner轻量目录\",\"scope\":" + 字符串(范围)
            + ",\"root\":" + 控制编码(目录.方法组织根) + ",\"items\":" + 项组 + "]}", true};
        检查停止();
        return 结果;
    }
    std::string 缺失方法目录(const std::vector<新缺失方法信息>& 目录) {
        检查停止();
        std::string 项组 = "[";
        for (const auto& M : 目录) {
            检查停止();
            if (项组.size() != 1) 项组 += ',';
            项组 += "{\"id\":" + 控制编码(M.方法头节点) + ",\"head\":" + 方法头(M.定义.方法头)
                + ",\"valueDomain\":" + 概念引用(M.定义.目标值域概念节点)
                + ",\"comparison\":" + 概念引用(M.定义.目标比较关系概念节点)
                + ",\"direction\":" + 概念引用(M.定义.目标变化方向概念节点)
                + ",\"sourceDemandId\":" + 控制编码(M.定义.来源需求节点)
                + ",\"sourceTaskId\":" + 控制编码(M.定义.来源任务节点) + "}";
        }
        检查停止();
        auto JSON = "{\"status\":\"已取得\",\"source\":\"正式缺失方法列表；仅能力缺口，不在方法树中\",\"items\":" + 项组 + "]}";
        检查停止();
        return JSON;
    }
    std::string 任务(const 新任务调度材料项& T) {
        检查停止();
        std::string 新增判定="null";
        if(T.新增成员目标判定){const auto& r=*T.新增成员目标判定;
            (void)映射新增成员目标满足(r); // 纯载荷核验，不查询、不据此授予业务资格。
            static constexpr std::array<const char*,5> 状态{"已读取","前提已变化","依赖未就绪","未支持","资源失败"};
            const auto index=static_cast<std::size_t>(r.状态);if(index<1||index>状态.size())throw std::logic_error("新增成员控制投影状态不合法");
            std::string witnesses="[";
            for(const auto& w:r.见证组){
                检查停止();
                if(witnesses.size()!=1)witnesses+=',';
                witnesses+="{\"taskId\":"+控制编码(w.任务节点)+",\"selfDemandId\":"+控制编码(w.自我需求节点)
                    +",\"meaningId\":"+控制编码(w.来源.需求来源.采用含义节点)+",\"messageId\":"+控制编码(w.来源.需求来源.键.接收消息节点)
                    +",\"humanId\":"+控制编码(w.来源.来源人类节点)+",\"cognizedDemandId\":"+控制编码(w.来源.被认知需求节点)+"}";}
            witnesses+=']';std::string current="null";
            if(r.当前集合){const auto& s=*r.当前集合;std::string members="[";
                for(const auto t:s.成员组){
                    检查停止();
                    if(members.size()!=1)members+=',';
                    members+=控制编码(t);
                }
                members+=']';
                current="{\"existenceId\":"+控制编码(s.目标存在)+",\"featureId\":"+控制编码(s.源特征节点)
                    +",\"featureTypeId\":"+控制编码(s.源特征类型)+",\"materialId\":"+控制编码(s.材料节点)+",\"members\":"+members+"}";}
            新增判定="{\"status\":"+字符串(状态[index-1])+",\"rawAddedCount\":"
                +(r.原始新增数量 ? 字符串(std::to_string(*r.原始新增数量)) : "null")+",\"currentSet\":"+current
                +",\"witnesses\":"+witnesses+",\"source\":\"同次任务调度材料；非跨服务原子快照\"}";
        }
        std::string 候选 = "null", 方法组 = "[";
        if (T.候选材料) {
            候选 = "[";
            for (const auto M : T.候选材料->候选方法节点组) {
                检查停止();
                if (候选.size() != 1) 候选 += ',';
                const auto 位置 = std::find_if(T.当前候选方法闭包.begin(), T.当前候选方法闭包.end(),
                    [&](const auto& x) { return x.节点 == M; });
                检查停止();
                候选 += "{\"id\":" + 控制编码(M) + ",\"label\":" + 字符串(
                    位置 != T.当前候选方法闭包.end() ? 位置->本能函数登记名.value_or("方法（无登记名）") : "方法（本轮无详情）") + "}";
            }
            候选 += ']';
        }
        for (const auto& M : T.当前候选方法闭包) {
            检查停止();
            if (方法组.size() != 1) 方法组 += ',';
            方法组 += 方法(M);
        }
        方法组 += ']';
        std::string 执行 = "null";
        if (T.执行) {
            const auto& X = *T.执行;
            std::string 状态组 = "[";
            for (const auto S : X.实际状态节点组) {
                检查停止();
                if (状态组.size() != 1) 状态组 += ',';
                状态组 += 控制编码(S);
            }
            执行 = "{\"id\":" + 控制编码(X.节点) + ",\"state\":" + 字符串(控制名称(X.状态))
                + ",\"actionId\":" + 控制编码(X.普通动作节点) + ",\"actualStateIds\":" + 状态组
                + "],\"assessmentId\":" + 控制编码(X.影响评估节点) + "}";
        }
        检查停止();
        const auto 筹办 = 当前_.任务管理线程.读取最近筹办结果(T.定位.任务节点);
        检查停止();
        std::string 筹办正文 = "null";
        std::string 派生读回 = "null";
        if (筹办 && 筹办->因果子需求提议) {
            const auto& p = *筹办->因果子需求提议;
            const auto 添加 = [&](const 新任务因果筹办依据& b) {
                检查停止();
                if (!b.读回.派生关系读回) return;
                if (派生读回 == "null") 派生读回 = "[";
                if (派生读回.size() != 1) 派生读回 += ',';
                派生读回 += 派生因果(*b.读回.派生关系读回);
                检查停止();
            };
            if (!p.候选依据组.empty()) for (const auto& b : p.候选依据组) {
                检查停止();
                添加(b);
            }
            else if (p.依据) 添加(*p.依据);
            if (派生读回 != "null") 派生读回 += ']';
        }
        if (筹办) 筹办正文 = "{\"status\":" + 字符串(控制名称(筹办->状态))
            + ",\"conditionProposalCount\":" + (筹办->独立子需求提议 ? 控制计数(筹办->独立子需求提议->子需求项组.size()) : "null")
            + ",\"causalProposalCount\":" + (筹办->因果子需求提议 ? 控制计数(筹办->因果子需求提议->子需求项组.size()) : "null")
            + ",\"compositeProposalCount\":" + (筹办->复合目标提议 ? 控制计数(筹办->复合目标提议->子目标组.size()) : "null")
            + ",\"derivedCausal\":" + 派生读回 + "}";
        检查停止();
        auto JSON = "{\"id\":" + 控制编码(T.定位.任务节点) + ",\"demandId\":" + 控制编码(T.定位.需求节点)
            + ",\"state\":" + 字符串(控制名称(T.定位.任务状态))
            + ",\"waitReason\":" + 字符串(控制名称(T.等待原因))
            + ",\"schedulingStatus\":" + 字符串(控制名称(T.状态))
            + ",\"newMemberSourceAssessment\":"+新增判定
            + ",\"lastMethodId\":" + (T.任务 ? 控制编码(T.任务->最近方法节点) : "null")
            + ",\"executionId\":" + 控制编码(T.定位.最近执行实例节点)
            + ",\"executionOrganizationId\":" + 控制编码(T.定位.方法执行组织节点)
            + ",\"queryComplete\":" + (T.候选材料 ? 控制布尔(T.候选材料->查询完整) : "null")
            + ",\"candidates\":" + 候选 + ",\"methods\":" + 方法组
            + ",\"conditionReview\":" + 条件(T.条件核验) + ",\"execution\":" + 执行
            + ",\"preparation\":" + 筹办正文 + "}";
        检查停止();
        return JSON;
    }
    普通控制采集结果 服务值() {
        检查停止();
        const auto 不可用 = [this](const char* 原因) -> 普通控制采集结果 {
            检查停止();
            普通控制采集结果 结果{"{\"status\":\"暂不可用\",\"reason\":" + 字符串(原因)
                + ",\"value\":null}", false};
            检查停止();
            return 结果;
        };
        if (!当前_.装配结果 || !当前_.装配结果->自我
            || !当前_.装配结果->自我->本能双根结果
            || !当前_.装配结果->自我->本能双根结果->服务实际特征节点)
            return 不可用("尚无自我实际服务特征身份");
        const auto F = *当前_.装配结果->自我->本能双根结果->服务实际特征节点;
        检查停止();
        const auto 特征 = 当前_.世界树.查询特征(F);
        检查停止();
        if (!特征) return 不可用("自我实际服务特征本轮不可读");
        if (!特征->当前值) return 不可用("自我实际服务特征尚无当前值");
        const auto* 值 = std::get_if<std::int64_t>(&*特征->当前值);
        if (!值) return 不可用("自我服务特征当前值不是I64");
        普通控制采集结果 结果{"{\"status\":\"已取得\",\"feature\":" + 控制编码(F)
            + ",\"value\":" + 字符串(std::to_string(*值))
            + ",\"source\":\"自我实际服务特征当前值，非初值或目标值\"}", true};
        检查停止();
        return 结果;
    }
    普通控制采集结果 学习工作(const 新学习工作快照& 快照,
        const std::map<稳定编码, std::string>& 方法名称) {
        检查停止();
        const std::string 范围 = "全部排队、执行中、已结束待确认工作；排除已确认去重缓存，不是学习历史";
        if (快照.状态 != 新学习工作快照读取状态::已取得 || !快照.线程快照) {
            普通控制采集结果 结果{"{\"status\":\"暂不可用\",\"reason\":" + 字符串(控制名称(快照.状态))
                + ",\"scope\":" + 字符串(范围) + ",\"thread\":null,\"items\":null}", false};
            检查停止();
            return 结果;
        }
        std::uint64_t 排队 = 0, 执行中 = 0, 待确认 = 0;
        std::string 项组 = "[";
        for (const auto& 项 : 快照.项组) {
            检查停止();
            switch (项.阶段) {
                case 新学习工作阶段::排队中: ++排队; break;
                case 新学习工作阶段::执行中: ++执行中; break;
                case 新学习工作阶段::完成待确认: ++待确认; break;
            }
            const auto 名称 = 方法名称.find(项.方法节点);
            const auto 标签 = 名称 == 方法名称.end() ? "方法（本轮无名称材料）" : 名称->second;
            if (项组.size() != 1) 项组 += ',';
            项组 += "{\"kind\":" + 字符串(项.种类 == 新学习工作种类::方法模拟 ? "方法模拟" : "问题材料")
                + ",\"state\":" + 字符串(控制名称(项.阶段))
                + ",\"methodId\":{\"id\":" + 控制编码(项.方法节点) + ",\"label\":" + 字符串(标签) + "}"
                + ",\"archiveId\":" + 控制编码(项.问题归档节点) + ",\"taskId\":" + 控制编码(项.任务节点)
                + ",\"executionId\":" + 控制编码(项.执行实例节点)
                + ",\"queuePosition\":" + (项.排队序号 ? 控制计数(*项.排队序号) : "null") + "}";
        }
        const auto& 线程 = *快照.线程快照;
        检查停止();
        普通控制采集结果 结果{"{\"status\":\"已取得\",\"scope\":" + 字符串(范围)
            + ",\"thread\":{\"state\":" + 字符串(控制名称(线程.状态))
            + ",\"currentMethodId\":" + 控制编码(线程.当前方法)
            + ",\"stopRequested\":" + 控制布尔(线程.停止已请求) + "}"
            + ",\"queuedCount\":" + 控制计数(排队) + ",\"runningCount\":" + 控制计数(执行中)
            + ",\"awaitingAcknowledgementCount\":" + 控制计数(待确认) + ",\"items\":" + 项组 + "]}", true};
        检查停止();
        return 结果;
    }
    普通控制采集结果 执行证据(const 新任务调度材料读取结果& 任务组) {
        检查停止();
        const std::string 范围 = "当前调度材料最近执行引用的动作、实际状态及显式动作结果；非全业务历史";
        const auto 不可用 = [&](const std::string& 原因) -> 普通控制采集结果 {
            检查停止();
            普通控制采集结果 结果{"{\"status\":\"暂不可用\",\"source\":\"本轮任务执行正式引用\",\"scope\":"
                + 字符串(范围) + ",\"reason\":" + 字符串(原因)
                + ",\"latestEvidenceAt\":null,\"items\":null}", false};
            检查停止();
            return 结果;
        };
        if (任务组.状态 != 新任务调度读取状态::已读取)
            return 不可用(控制名称(任务组.状态));
        using 来源 = std::set<std::pair<稳定编码, 稳定编码>>;
        std::map<稳定编码, 来源> 状态来源, 动态来源;
        std::map<稳定编码, std::optional<新动态信息>> 动态缓存;
        std::map<稳定编码, 新状态信息> 状态缓存;
        const auto 读取动态 = [&](稳定编码 D) -> const std::optional<新动态信息>& {
            检查停止();
            auto [位置, 新增] = 动态缓存.try_emplace(D);
            if (新增) {
                检查停止();
                位置->second = 当前_.世界事实.动态服务.获取动态(D);
                检查停止();
            }
            检查停止();
            return 位置->second;
        };
        for (const auto& T : 任务组.项组) {
            检查停止();
            if (!T.执行) continue;
            const auto& X = *T.执行;
            const auto 源 = std::make_pair(T.定位.任务节点, X.节点);
            for (const auto S : X.实际状态节点组) {
                检查停止();
                状态来源[S].insert(源);
            }
            if (!X.普通动作节点) continue;
            const auto A = *X.普通动作节点;
            动态来源[A].insert(源);
            const auto& 动作 = 读取动态(A);
            if (!动作 || 动作->种类 != 新动态种类::动作)
                return 不可用("任务所引用动作本轮不可读：" + std::to_string(A.值));
            for (const auto& 关联 : 动作->动作结果组) {
                检查停止();
                const auto D = 关联.结果动态节点;
                动态来源[D].insert(源);
                const auto& 结果 = 读取动态(D);
                if (!结果) return 不可用("动作所引用结果动态本轮不可读：" + std::to_string(D.值));
                for (const auto S : 结果->状态节点组) {
                    检查停止();
                    状态来源[S].insert(源);
                }
            }
        }
        for (const auto& [S, 源] : 状态来源) {
            检查停止();
            const auto 状态 = 当前_.世界事实.状态服务.获取状态(S);
            检查停止();
            if (!状态) return 不可用("执行所引用状态本轮不可读：" + std::to_string(S.值));
            状态缓存.emplace(S, *状态);
        }
        const auto 引用组 = [this](const auto& 组) {
            检查停止();
            std::string JSON = "[";
            for (const auto 节点 : 组) {
                检查停止();
                if (JSON.size() != 1) JSON += ',';
                JSON += 控制编码(节点);
            }
            JSON += ']';
            检查停止();
            return JSON;
        };
        const auto 来源正文 = [&](const 来源& 组) {
            检查停止();
            std::set<稳定编码> T组, X组;
            std::string 配对 = "[";
            for (const auto& [T, X] : 组) {
                检查停止();
                T组.insert(T); X组.insert(X);
                if (配对.size() != 1) 配对 += ',';
                配对 += "{\"taskId\":" + 控制编码(T) + ",\"executionId\":" + 控制编码(X) + "}";
            }
            auto JSON = ",\"taskIds\":" + 引用组(T组) + ",\"executionIds\":" + 引用组(X组)
                + ",\"sources\":" + 配对 + ']';
            检查停止();
            return JSON;
        };
        const auto 时间名 = [](const 新状态强时间& 时间) {
            return 时间.语义 == 新状态时间语义::实例绝对UTC ? "实例绝对UTC" : "抽象相对";
        };
        struct 显示项 final {
            std::optional<std::int64_t> 时间;
            int 类别;
            稳定编码 节点;
            std::string JSON;
        };
        std::vector<显示项> 项组;
        for (const auto& [S, 源] : 状态来源) {
            检查停止();
            const auto& 状态 = 状态缓存.at(S);
            const auto* I64 = std::get_if<std::int64_t>(&状态.固定准确值);
            const auto 值 = I64 ? 字符串(std::to_string(*I64)) : 控制编码(std::get<稳定编码>(状态.固定准确值));
            const auto 实际时间 = 状态.强时间.语义 == 新状态时间语义::实例绝对UTC
                ? std::optional<std::int64_t>(状态.强时间.纳秒) : std::nullopt;
            项组.push_back({实际时间, 0, S, "{\"kind\":\"状态\",\"id\":" + 控制编码(S)
                + ",\"timeSemantic\":" + 字符串(时间名(状态.强时间))
                + ",\"timeNs\":" + 字符串(std::to_string(状态.强时间.纳秒))
                + ",\"subjectId\":" + 控制编码(状态.被描述存在节点)
                + ",\"feature\":" + 控制编码(状态.来源实例特征节点)
                + ",\"featureType\":" + 概念引用(状态.特征类型概念节点)
                + ",\"fixedValue\":{\"kind\":" + 字符串(I64 ? "I64" : "节点引用")
                + ",\"value\":" + 值 + "}" + 来源正文(源) + "}"});
        }
        for (const auto& [D, 源] : 动态来源) {
            检查停止();
            const auto& 动态 = *动态缓存.at(D);
            std::optional<std::int64_t> 开始, 结束;
            bool UTC完整 = !动态.状态节点组.empty();
            for (const auto S : 动态.状态节点组) {
                检查停止();
                const auto 位置 = 状态缓存.find(S);
                if (位置 == 状态缓存.end() || 位置->second.强时间.语义 != 新状态时间语义::实例绝对UTC) {
                    UTC完整 = false; continue;
                }
                const auto 时刻 = 位置->second.强时间.纳秒;
                开始 = 开始 ? (std::min)(*开始, 时刻) : 时刻;
                结束 = 结束 ? (std::max)(*结束, 时刻) : 时刻;
            }
            if (!UTC完整) { 开始.reset(); 结束.reset(); }
            auto 排序时刻 = 结束;
            std::string 种类 = 动态.种类 == 新动态种类::动作 ? "动作" : 动态.种类 == 新动态种类::原子 ? "变化动态" : "复合动态";
            std::string 时间语义 = UTC完整 ? "实例绝对UTC" : "无可比较实际时刻";
            std::string 时刻 = "null";
            if (动态.动作时间) {
                时间语义 = 时间名(*动态.动作时间);
                时刻 = 字符串(std::to_string(动态.动作时间->纳秒));
                if (动态.动作时间->语义 == 新状态时间语义::实例绝对UTC) 排序时刻 = 动态.动作时间->纳秒;
            } else if (排序时刻) 时刻 = 字符串(std::to_string(*排序时刻));
            std::string 关联组 = "[";
            for (const auto& 关联 : 动态.动作结果组) {
                检查停止();
                if (关联组.size() != 1) 关联组 += ',';
                关联组 += "{\"relation\":" + 字符串(关联.类型 == 新动作结果关系类型::导致变化 ? "导致变化" : "观察取得")
                    + ",\"resultDynamicId\":" + 控制编码(关联.结果动态节点) + "}";
            }
            const auto 目标 = 动态.动作目标任务虚拟存在
                ? std::optional<稳定编码>(动态.动作目标任务虚拟存在->虚拟存在节点) : 动态.动作目标存在节点;
            项组.push_back({排序时刻, 1, D, "{\"kind\":" + 字符串(种类) + ",\"id\":" + 控制编码(D)
                + ",\"timeSemantic\":" + 字符串(时间语义) + ",\"timeNs\":" + 时刻
                + ",\"startTimeNs\":" + (开始 ? 字符串(std::to_string(*开始)) : "null")
                + ",\"endTimeNs\":" + (结束 ? 字符串(std::to_string(*结束)) : "null")
                + ",\"subjectId\":" + 控制编码(动态.主题存在节点)
                + ",\"actionDefinitionId\":" + 控制编码(动态.动作定义节点)
                + ",\"actionSourceId\":" + 控制编码(动态.动作来源节点)
                + ",\"targetKind\":" + (动态.种类 == 新动态种类::动作
                    ? 字符串(动态.动作目标任务虚拟存在 ? "任务虚拟存在" : "普通存在") : "null")
                + ",\"targetId\":" + 控制编码(目标) + ",\"targetFeatureId\":" + 控制编码(动态.动作目标特征节点)
                + ",\"stateIds\":" + 引用组(动态.状态节点组) + ",\"memberDynamicIds\":" + 引用组(动态.成员动态节点组)
                + ",\"actionResults\":" + 关联组 + ']' + 来源正文(源) + "}"});
        }
        检查停止();
        std::sort(项组.begin(), 项组.end(), [](const auto& 左, const auto& 右) {
            if (左.时间 != 右.时间) return 左.时间 > 右.时间;
            if (左.类别 != 右.类别) return 左.类别 < 右.类别;
            return 左.节点 < 右.节点;
        });
        检查停止();
        std::string JSON = "[";
        for (const auto& 项 : 项组) {
            检查停止();
            if (JSON.size() != 1) JSON += ',';
            JSON += 项.JSON;
        }
        const auto 最近 = !项组.empty() ? 项组.front().时间 : std::nullopt;
        检查停止();
        普通控制采集结果 结果{"{\"status\":\"已取得\",\"source\":\"本轮任务执行正式引用\",\"scope\":" + 字符串(范围)
            + ",\"latestEvidenceAt\":" + (最近 ? 字符串(std::to_string(*最近)) : "null")
            + ",\"items\":" + JSON + "]}", true};
        检查停止();
        return 结果;
    }
    std::string 接收认知材料() {
        检查停止();
        const auto last=当前_.接收认知.读取最近观察();
        if(!last) {
            检查停止();
            return "{\"stage\":\"未处理\",\"reason\":\"尚无接收认知观察\",\"message\":null,\"speaker\":null,\"scene\":null,\"meaning\":null,\"demand\":null}";
        }
        auto item=*last;
        static constexpr std::array<const char*,14> names{"未处理","已采用U","已建立D","已读回","非支持输入",
            "对象未决","非本次人类需求","D映射拒绝","D映射受阻","前提变化","资源延后","停止未完成","已读回行动请求","已读回待完善请求"};
        const auto n=static_cast<std::size_t>(item.阶段);
        if(n<1||n>names.size())throw std::logic_error("received cognition observation stage invalid");
        检查停止();
        if(有效(item.M)) {
            检查停止();
            const auto m=当前_.外设世界树.读取消息保护(item.M);
            检查停止();
            if(m.状态!=新外设事实状态::已完成||!m.消息) {
                item.M={};item.H.reset();item.S.reset();item.U.reset();item.D.reset();
                item.受请求者.reset();item.需求来源.reset();item.待交接请求.reset();
                item.原因+="；当前正式消息读回暂不可用";
            }
        }
        if(item.U) {
            检查停止();
            const auto u=当前_.世界事实.语言服务.读取采用含义(*item.U,当前_.世界事实.特征服务);
            检查停止();
            if(u.状态!=新语言含义采用状态::已采用||!u.信息||u.信息->来源.接收消息节点!=item.M) {
                item.U.reset();item.D.reset();item.受请求者.reset();item.需求来源.reset();item.待交接请求.reset();
                item.原因+="；当前正式含义读回暂不可用";
            } else {item.H=u.信息->说话者节点;item.S=u.信息->场景节点;}
        }
        if(item.U&&item.受请求者) {
            检查停止();
            const auto r=当前_.世界事实.语言服务.读取采用行动请求保护(*item.U,*item.受请求者,
                当前_.世界事实.特征服务,当前_.世界事实.存在服务);
            检查停止();
            if(r.状态!=新语言含义采用状态::已采用||!r.信息
                ||r.信息->请求来源.接收消息节点!=item.M||r.信息->说话者H!=item.H
                ||r.信息->需求来源!=item.需求来源) {
                item.受请求者.reset();item.需求来源.reset();item.D.reset();item.待交接请求.reset();
                item.原因+="；当前正式行动请求读回暂不可用";
            }
        }
        if(item.D) {
            检查停止();
            const auto d=当前_.需求服务.读取需求治理信息(*item.D);
            检查停止();
            if(!d||!d->首次接收认知来源||d->首次接收认知来源->键.接收消息节点!=item.M
                ||d->首次接收认知来源->采用含义节点!=item.U
                ||!item.需求来源||d->首次接收认知来源->键!=*item.需求来源) {
                item.D.reset();item.待交接请求.reset();item.原因+="；当前正式需求来源读回暂不可用";
            }
        }
        std::string 来源="null";
        if(item.需求来源) {
            std::string 路径="[";
            for(const auto x:item.需求来源->根出现路径) {
                检查停止();
                if(路径.size()!=1)路径+=',';
                路径+=std::to_string(x);
            }
            来源="{\"message\":"+控制编码(item.需求来源->接收消息节点)
                +",\"start\":"+字符串(std::to_string(item.需求来源->原文起点))
                +",\"end\":"+字符串(std::to_string(item.需求来源->原文终点))
                +",\"path\":"+路径+"]}";
        }
        检查停止();
        auto JSON = "{\"stage\":"+字符串(names[n-1])+",\"reason\":"+字符串(item.原因)
            +",\"message\":"+控制编码(有效(item.M)?std::optional<稳定编码>{item.M}:std::nullopt)
            +",\"speaker\":"+控制编码(item.H)+",\"scene\":"+控制编码(item.S)
            +",\"meaning\":"+控制编码(item.U)+",\"demand\":"+控制编码(item.D)
            +",\"recipient\":"+控制编码(item.受请求者)+",\"demandSource\":"+来源
            +",\"handoffPending\":"+控制布尔(item.待交接请求.has_value())+"}";
        检查停止();
        return JSON;
    }
    普通控制采集可停止结果 采集() {
        try {
            检查停止();
            auto 结果 = 采集完整材料();
            检查停止();
            return 结果;
        } catch (const 普通控制采集取消&) {
            return 普通控制采集取消{};
        }
    }
private:
    void 检查停止() const { 检查普通控制采集停止(停止_); }
    std::string 字符串(std::string_view 文本) {
        return 控制JSON字符串(文本, 停止_);
    }
    普通控制采集结果 采集完整材料() {
        检查停止();
        const auto 开始时间 = 控制UTC时间();
        检查停止();
        const auto 自我 = 当前_.自我线程.读取快照();
        检查停止();
        const auto 管理 = 当前_.任务管理线程.读取快照();
        检查停止();
        const auto 需求组 = 当前_.需求服务.查询全部需求记录();
        检查停止();
        const auto 任务组 = 当前_.任务服务.读取任务调度材料();
        检查停止();
        const auto 服务材料 = 服务值();
        检查停止();
        const auto 时间线 = 执行证据(任务组);
        检查停止();
        const auto 目录 = 当前_.方法服务.读取方法目录();
        检查停止();
        const auto 目录材料 = 方法目录(目录);
        检查停止();
        const auto 缺失列表 = 当前_.方法服务.查询缺失方法列表();
        检查停止();
        const auto 缺失目录 = 缺失方法目录(缺失列表);
        检查停止();
        std::map<稳定编码, std::string> 方法名称;
        if (目录.状态 == 新方法目录读取状态::已读取)
            for (const auto& M : 目录.项组) {
                检查停止();
                方法名称.emplace(M.节点, M.本能函数登记名.value_or("方法（无登记名）"));
            }
        if (任务组.状态 == 新任务调度读取状态::已读取)
            for (const auto& T : 任务组.项组) {
                检查停止();
                for (const auto& M : T.当前候选方法闭包) {
                    检查停止();
                    方法名称.emplace(M.节点, M.本能函数登记名.value_or("方法（无登记名）"));
                }
            }
        检查停止();
        const auto 学习快照 = 当前_.自我线程.读取学习工作快照();
        检查停止();
        const auto 学习材料 = 学习工作(学习快照, 方法名称);
        检查停止();
        const bool 需求已读取 = 需求组.状态 == 新需求操作状态::已找到;
        const bool 任务已读取 = 任务组.状态 == 新任务调度读取状态::已读取;
        std::string D组 = "[", T组 = "[";
        if (需求已读取) for (const auto& D : 需求组.需求组) {
            检查停止();
            新需求治理信息 信息;
            信息.节点 = D.节点; 信息.父需求节点 = D.父需求节点; 信息.来源类型 = D.来源类型;
            信息.来源承接节点 = D.来源承接节点; 信息.权重 = D.权重; 信息.活动状态 = D.活动状态;
            信息.需求主体存在节点 = D.需求主体存在节点; 信息.执行域 = D.执行域; 信息.复合来源 = D.复合来源;
            信息.行动请求来源 = D.行动请求来源;
            信息.完整目标 = 新需求完整目标{D.目标存在节点, D.目标};
            if (D组.size() != 1) D组 += ','; D组 += 需求(信息);
        }
        if (需求已读取) for (const auto& D : 需求组.待完善需求组) {
            检查停止();
            if (D组.size() != 1) D组 += ',';
            D组 += 需求(D);
        }
        if (任务已读取) for (const auto& T : 任务组.项组) {
            检查停止();
            if (T组.size() != 1) T组 += ',';
            T组 += 任务(T);
        }
        检查停止();
        const auto 需求正文 = 需求已读取 ? "{\"status\":\"已取得\",\"items\":" + D组 + "]}"
            : "{\"status\":\"暂不可用\",\"reason\":" + 字符串(控制名称(需求组.状态)) + ",\"items\":null}";
        检查停止();
        const auto 任务正文 = 任务已读取 ? "{\"status\":\"已取得\",\"items\":" + T组 + "]}"
            : "{\"status\":\"暂不可用\",\"reason\":" + 字符串(控制名称(任务组.状态)) + ",\"items\":null}";
        检查停止();
        const auto 异常正文 = [&](const std::optional<新学习运行异常记录>& 异常) {
            检查停止();
            auto JSON = 异常 ? "{\"methodId\":" + 控制编码(异常->方法节点) + ",\"reason\":"
                + 字符串(控制名称(异常->原因)) + ",\"diagnostic\":" + 字符串(异常->诊断) + "}" : std::string("null");
            检查停止();
            return JSON;
        };
        const auto 接收认知 = 接收认知材料();
        检查停止();
        const auto 完成时间 = 控制UTC时间();
        检查停止();
        auto 正文 = "{\"readStartedAt\":" + 开始时间 + ",\"readCompletedAt\":" + 完成时间
            + ",\"consistency\":\"分项只读，非跨服务原子快照\",\"overview\":{"
            "\"startupStage\":\"已完成装配；启动期间页面未接入\",\"self\":{\"state\":" + 字符串(控制名称(自我.生命周期))
            + ",\"existence\":" + 存在引用(自我.自我存在) + ",\"scene\":" + 存在引用(自我.自我所在场景)
            + ",\"activeMessages\":" + 控制计数(自我.活动消息数)
            + ",\"stopRequested\":" + 控制布尔(自我.停止已请求) + ",\"finished\":" + 控制布尔(自我.线程已完成)
            + "},\"manager\":{\"state\":" + 字符串(控制名称(管理.生命周期))
            + ",\"activeMessages\":" + 控制计数(管理.活动消息数) + ",\"activeWork\":" + 控制计数(管理.活动工作数)
            + ",\"pendingFeedback\":" + 控制计数(管理.待反馈任务数) + ",\"pendingReview\":" + 控制计数(管理.等待复核任务数)
            + ",\"schedulingInFlight\":" + 控制布尔(管理.调度扫描在途)
            + "},\"serviceValue\":" + 服务材料.JSON + ",\"lastBusinessProgressAt\":{\"status\":\"未接入\",\"reason\":\"尚无全系统业务推进时间源；局部执行证据不代替它\"}},"
            "\"demands\":" + 需求正文 + ",\"tasks\":" + 任务正文
            + ",\"methodCatalog\":" + 目录材料.JSON + ",\"missingMethods\":" + 缺失目录
            + ",\"learning\":{\"source\":\"学习线程活动工作快照；自我和管理线程反馈另列\","
            "\"workSnapshot\":" + 学习材料.JSON + ",\"pendingMethods\":" + 控制计数(自我.待学习方法数)
            + ",\"lastSelfCompletionMethodId\":" + (自我.最近学习结束 ? 控制编码(自我.最近学习结束->方法节点) : "null")
            + ",\"completionDeliveredToManager\":" + 控制布尔(自我.学习结束已交接管理)
            + ",\"lastManagerCompletionMethodId\":" + (管理.最近方法学习结束 ? 控制编码(管理.最近方法学习结束->方法节点) : "null")
            + ",\"waitingMethodInstance\":" + 控制布尔(管理.等待方法实例消费)
            + ",\"lastSelfError\":" + 异常正文(自我.最近学习运行异常)
            + ",\"lastManagerError\":" + 异常正文(管理.最近学习运行异常)
            + "},\"timeline\":" + 时间线.JSON + ",\"receivedDemandCognition\":"+接收认知+"}";
        检查停止();
        return {std::move(正文), 需求已读取 && 任务已读取 && 服务材料.完整 && 学习材料.完整 && 时间线.完整 && 目录材料.完整};
    }
    std::string 派生因果(const 新派生关系筹办读回& x) {
        检查停止();
        const auto 枚举 = [this](auto e, std::initializer_list<const char*> 名称) {
            const auto n = static_cast<std::int64_t>(e);
            if (n < 1 || static_cast<std::size_t>(n) > 名称.size())
                throw std::logic_error("control derived causal unknown enum");
            检查停止();
            return 字符串(*(名称.begin() + n - 1));
        };
        const auto 节点组 = [this](const std::vector<稳定编码>& ns) {
            检查停止();
            std::string s = "[";
            for (const auto n : ns) {
                检查停止();
                if (s.size() != 1) s += ',';
                s += 控制编码(n);
            }
            s += ']';
            检查停止();
            return s;
        };
        const auto 缺口 = [&](const std::vector<新关系筹办缺口>& gs) {
            检查停止();
            std::string s = "[";
            for (const auto& g : gs) {
                检查停止();
                if (s.size() != 1) s += ',';
                s += "{\"kind\":" + 枚举(g.种类, {"前提尚未具备", "后继投影成员目标未具备", "结果与源约束无交集",
                    "本叶不支持关系族", "绑定缺失", "绑定不唯一", "没有匹配关系规则"})
                    + ",\"ruleId\":" + 控制编码(g.规则节点) + ",\"conditionPosition\":"
                    + (g.条件序号 ? std::to_string(*g.条件序号) : "null") + "}";
            }
            s += ']';
            检查停止();
            return s;
        };
        std::string 约束 = "null";
        if (x.源约束) {
            const auto& s = *x.源约束;
            std::string 域 = "[";
            for (const auto& r : s.允许新增区间组) {
                检查停止();
                if (域.size() != 1) 域 += ',';
                域 += "{\"lower\":" + 字符串(std::to_string(r.下界))
                    + ",\"upper\":" + 字符串(std::to_string(r.上界)) + "}";
            }
            约束 = "{\"derivedType\":" + 控制编码(s.原派生类型) + ",\"targetDomain\":" + 控制编码(s.原目标域)
                + ",\"targetExistence\":" + 控制编码(s.目标存在节点) + ",\"sourceFeature\":" + 控制编码(s.源特征节点)
                + ",\"sourceType\":" + 控制编码(s.源特征类型) + ",\"baselineMaterial\":" + 控制编码(s.基准材料节点)
                + ",\"baselineMembers\":" + 节点组(s.基准成员组)
                + ",\"algorithm\":" + std::to_string(static_cast<std::int64_t>(s.派生定义.算法))
                + ",\"allowedAddedRanges\":" + 域 + "],\"baselineKind\":"
                + 字符串(s.新增成员见证要求 ? "目标出生固定基准，非执行前基线" : "本次筹办解释基准，非执行前基线")
                + ",\"requiresFormalRequestWitness\":"+控制布尔(s.新增成员见证要求.has_value())+"}";
        }
        std::string 规则 = "[";
        for (const auto& r : x.关系分支组) {
            检查停止();
            if (规则.size() != 1) 规则 += ',';
            const auto& d = r.正式定义;
            std::string 角色 = "[", 条件 = "[";
            for (const auto& b : r.角色绑定组) {
                检查停止();
                if (角色.size() != 1) 角色 += ',';
                std::string actual = "null";
                if (const auto* e = std::get_if<新因果普通存在绑定>(&b.实际绑定)) actual = 控制编码(e->存在节点);
                if (const auto* n = std::get_if<新因果需求信息绑定>(&b.实际绑定)) actual = 控制编码(n->需求节点);
                角色 += "{\"position\":" + std::to_string(b.角色序号)
                    + ",\"category\":" + 枚举(b.类别, {"普通存在", "需求信息", "任务信息", "执行实例", "影响评估"})
                    + ",\"range\":" + 控制编码(b.范围概念) + ",\"actual\":" + actual + "}";
            }
            for (const auto& c : r.全部条件) {
                检查停止();
                if (条件.size() != 1) 条件 += ',';
                std::string 位置 = "[";
                for (const auto n : c.正式条件.角色组) {
                    检查停止();
                    if (位置.size() != 1) 位置 += ',';
                    位置 += std::to_string(n);
                }
                条件 += "{\"position\":" + std::to_string(c.正式条件.序号)
                    + ",\"kind\":" + 枚举(c.正式条件.种类, {"角色有效", "自我服务投影绑定", "需求父链归根", "排除需求有父",
                        "投影源对应完整", "当前引用集合合法", "当前集合等于投影", "投影待写入", "需求准备非待完善", "任务关联需求",
                        "任务最近执行", "执行关联任务", "执行关联评估", "评估关联任务", "评估关联执行", "评估允许完成",
                        "自我接受完成决议", "任务状态属于", "执行状态属于"})
                    + ",\"relation\":" + 控制编码(c.正式条件.关系概念) + ",\"rolePositions\":" + 位置
                    + "],\"conceptArguments\":" + 节点组(c.正式条件.概念参数组)
                    + ",\"status\":" + 枚举(c.状态, {"满足", "尚未具备", "角色未绑定", "本叶未支持"})
                    + ",\"evidence\":" + 节点组(c.正式依据节点组) + "}";
            }
            规则 += "{\"id\":" + 控制编码(r.规则节点) + ",\"ruleConcept\":" + 控制编码(d.规则概念)
                + ",\"governanceDefinition\":" + 控制编码(d.治理定义) + ",\"sceneScope\":" + 控制编码(d.作用场景范围概念)
                + ",\"actionConcept\":" + 控制编码(d.动作概念)
                + ",\"trigger\":" + 枚举(static_cast<std::int64_t>(d.触发方式) + 1, {"尚未确定", "主动调用", "被动触发"})
                + ",\"resultKind\":" + 枚举(d.唯一结果.种类, {"正式服务任务投影", "特征值域"})
                + ",\"resultChange\":" + 枚举(d.唯一结果.变化, {"改变", "保持"})
                + ",\"resultFeature\":" + 控制编码(d.唯一结果.结果特征类型)
                + ",\"resultRange\":" + 控制编码(d.唯一结果.结果范围概念)
                + ",\"roles\":" + 角色 + "],\"conditions\":" + 条件 + ']'
                + ",\"currentProjectionMembers\":" + (r.当前正式投影成员组 ? 节点组(*r.当前正式投影成员组) : "null")
                + ",\"currentProjectionAddedCount\":" + (r.当前正式投影新增数 ? 字符串(std::to_string(*r.当前正式投影新增数)) : "null")
                + ",\"currentProjectionSatisfies\":" + (r.当前正式投影满足源约束 ? 控制布尔(*r.当前正式投影满足源约束) : "null")
                + ",\"gaps\":" + 缺口(r.缺口组) + "}";
        }
        检查停止();
        auto JSON = "{\"status\":" + 枚举(x.状态, {"已形成", "不是派生目标", "算法未支持", "输入关系未支持",
                "缺少源基准", "数学无解", "材料已变化", "源配置未装配"})
            + ",\"sourceConstructionStatus\":" + (x.来源构造状态 ? 枚举(*x.来源构造状态,
                {"已读取", "算法未支持", "输入关系未支持", "源配置未装配", "源特征尚未形成", "源当前值未形成", "材料已变化"}) : "null")
            + ",\"sourceConstraint\":" + 约束 + ",\"rules\":" + 规则 + "],\"gaps\":" + 缺口(x.缺口组) + "}";
        检查停止();
        return JSON;
    }
    普通应用上下文& 当前_;
    std::map<稳定编码, std::string> 名称_;
    std::stop_token 停止_;
};

void 停止普通控制采集(普通应用上下文& 当前) {
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
    记录装配停止阶段(L"采集停止入口");
#endif
    if (!当前.控制缓存) return;
    if (当前.控制采集线程.joinable()) {
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"采集request_stop前");
#endif
        当前.控制采集线程.request_stop();
        当前.控制缓存->唤醒.notify_all();
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"采集join前");
#endif
        当前.控制采集线程.join();
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"采集join后");
#endif
    }
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
    记录装配停止阶段(L"采集缓存锁前");
#endif
    std::lock_guard 锁(当前.控制缓存->互斥);
    当前.控制缓存->正在采集 = false;
    当前.控制缓存->状态 = 网页控制快照状态::已停止;
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
    记录装配停止阶段(L"采集停止返回前");
#endif
}

void 采集普通控制面板(普通应用上下文& 当前,
    const std::shared_ptr<普通控制快照缓存>& 缓存, std::stop_token 停止) noexcept {
    try {
        bool 已报告资源异常 = false;
        // 等到首条正式入站（或停止）再采集；无入站时最多等 30 分钟，避免只读观察永久饿死。
        {
            std::unique_lock 锁(缓存->互斥);
            (void)缓存->唤醒.wait_for(锁, 停止, std::chrono::seconds(1800), [&] {
                return 缓存->入站后允许采集.load(std::memory_order_acquire);
            });
        }
        if (停止.stop_requested()) {
            std::lock_guard 锁(缓存->互斥);
            缓存->正在采集 = false;
            缓存->状态 = 网页控制快照状态::已停止;
            return;
        }
        while (!停止.stop_requested()) {
            { std::lock_guard 锁(缓存->互斥); 缓存->正在采集 = true; }
            std::shared_ptr<const std::string> 新正文;
            bool 完整 = false;
            bool 本轮取消 = false;
            try {
                auto 结果 = 普通控制投影(当前, 停止).采集();
                if (const auto* 正常 = std::get_if<普通控制采集结果>(&结果)) {
                    完整 = 正常->完整;
                    if (!停止.stop_requested()) {
                        新正文 = std::make_shared<const std::string>(
                            std::move(std::get<普通控制采集结果>(结果).JSON));
                    }
                } else if (std::holds_alternative<普通控制采集取消>(结果)) {
                    if (!停止.stop_requested())
                        throw std::logic_error("control collection cancelled without stop request");
                    本轮取消 = true;
                } else {
                    throw std::logic_error("control collection result has no valid alternative");
                }
            } catch (const std::bad_alloc&) {
                if (!已报告资源异常) {
                    (void)记录运行日志(L"控制界面", L"采集只读快照", L"控制面板快照资源不足，保留上一份显示；不改变业务状态。");
                    已报告资源异常 = true;
                }
            }
            {
                std::lock_guard 锁(缓存->互斥);
                缓存->正在采集 = false;
                if (本轮取消 || 停止.stop_requested()) { 缓存->状态 = 网页控制快照状态::已停止; break; }
                if (新正文) {
                    if (缓存->序号 == (std::numeric_limits<std::uint64_t>::max)()) throw std::overflow_error("control sequence overflow");
                    缓存->正文 = std::move(新正文); ++缓存->序号;
                    缓存->完成时间 = std::chrono::steady_clock::now();
                    缓存->状态 = 完整 ? 网页控制快照状态::已取得 : 网页控制快照状态::采集失败;
                } else 缓存->状态 = 网页控制快照状态::采集失败;
            }
            std::unique_lock 锁(缓存->互斥);
            const auto 间隔 = 完整 ? std::chrono::milliseconds(2000) : std::chrono::milliseconds(15000);
            (void)缓存->唤醒.wait_for(锁, 停止, 间隔, [] { return false; });
        }
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"控制面板只读采集", L"只读快照工程异常，不将其解释为业务失败或空数据。",e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"控制面板只读采集", L"只读快照工程异常，不将其解释为业务失败或空数据。");
    }
}

普通应用上下文* 读取治理启停上下文(bool 要求成功装配) noexcept {
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文 || (要求成功装配 && (!上下文->装配结果 || !上下文->装配结果->成功()))) return nullptr;
        return 上下文.get(); // 上下文只首次建立，启停期间不替换或销毁。
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取治理启停上下文", L"启停引用读取异常。"); }
}

bool 配置有效(const 普通应用配置& 配置) noexcept {
    return 配置.自我线程邮箱容量 != 0
        && 配置.任务管理线程邮箱容量 != 0
        && 配置.资源重试间隔毫秒 != 0
        && 配置.线程进入等待毫秒 != 0
        && 配置.线程停止回收等待毫秒 != 0
        && 配置.安全根初始权重 >= 0
        && 配置.服务根初始权重 >= 0
        && 新_语言类::严格UTF8字节串有效(配置.第一权限人登录名)
        && 新_语言类::严格UTF8字节串有效(配置.第一权限人姓名)
        && (配置.本次网页会话服务对象登录名.empty()
            || 新_语言类::严格UTF8字节串有效(配置.本次网页会话服务对象登录名));
}

bool 世界初始化成功(新世界初始化状态 状态) noexcept {
    return 状态 == 新世界初始化状态::已完成
        || 状态 == 新世界初始化状态::无变化;
}

bool 管理启动成功(新任务管理操作状态 状态) noexcept {
    return 状态 == 新任务管理操作状态::已完成
        || 状态 == 新任务管理操作状态::已在运行;
}

bool 管理停止成功(新任务管理操作状态 状态) noexcept {
    return 状态 == 新任务管理操作状态::已完成
        || 状态 == 新任务管理操作状态::无变化
        || 状态 == 新任务管理操作状态::尚未启动;
}

bool 自我启动成功(新自我线程操作状态 状态) noexcept {
    return 状态 == 新自我线程操作状态::已完成
        || 状态 == 新自我线程操作状态::已在运行;
}

bool 自我停止成功(新自我线程操作状态 状态) noexcept {
    return 状态 == 新自我线程操作状态::已完成
        || 状态 == 新自我线程操作状态::无变化
        || 状态 == 新自我线程操作状态::尚未启动;
}

std::optional<std::int64_t> 当前UTC纳秒() noexcept {
    try {
        const auto 计数 = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        if (计数 < 0
            || static_cast<std::uintmax_t>(计数)
                > static_cast<std::uintmax_t>(
                    (std::numeric_limits<std::int64_t>::max)())) {
            return std::nullopt;
        }
        return static_cast<std::int64_t>(计数);
    } catch (...) {
        return std::nullopt;
    }
}

bool 确保服务任务集合特征(
    普通应用上下文& 当前, 稳定编码 自我存在节点) noexcept {
    try {
        if (!有效(自我存在节点) || 当前.服务任务集合写后失败
            || (当前.服务任务自我存在节点
                && *当前.服务任务自我存在节点 != 自我存在节点)) return false;
        if (!当前.服务任务自我存在节点) 当前.服务任务自我存在节点 = 自我存在节点;
        特征概念定义 定义;
        定义.材料类型 = 特征概念材料物理类型::U64数组;
        定义.值域 = 特征概念全材料值域{};
        定义.比较规则 = 特征概念比较规则::服务任务集合身份判等;
        定义.聚合规则 = 特征概念聚合规则::不自动聚合;
        const auto 先天 = 当前.世界事实.特征概念服务.获取先天特征概念();
        if (!先天 || (当前.服务任务特征概念节点
                && *当前.服务任务特征概念节点 != 先天->服务任务集合)) return false;
        if (!当前.服务任务特征概念节点) {
            当前.服务任务特征概念节点 = 先天->服务任务集合;
        }
        const auto 概念 = 当前.世界事实.特征概念服务.获取特征概念(
            *当前.服务任务特征概念节点);
        if (!概念 || 概念->材料类型 != 定义.材料类型
            || 概念->值域 != 定义.值域 || 概念->单位
            || 概念->比较规则 != 定义.比较规则
            || 概念->聚合规则 != 定义.聚合规则) return false;
        if (!当前.服务任务特征节点) {
            const auto 添加 = 当前.世界事实.存在服务.添加特征区间值(
                自我存在节点, *当前.服务任务特征概念节点, 定义.值域);
            if ((添加.状态 != 新存在操作状态::已完成
                    && 添加.状态 != 新存在操作状态::无变化)
                || !添加.特征节点) return false;
            当前.服务任务特征节点 = 添加.特征节点;
        }
        const auto 存在 = 当前.世界事实.存在服务.获取存在(自我存在节点);
        const auto 特征 = 当前.世界事实.特征服务.获取特征(*当前.服务任务特征节点);
        if (!存在 || !特征
            || std::count(存在->特征节点组.begin(), 存在->特征节点组.end(),
                *当前.服务任务特征节点) != 1) return false;
        const auto 类型关系 = 当前.世界事实.特征概念服务.比较单特征概念(
            *当前.服务任务特征概念节点, 特征->特征概念节点);
        return 类型关系.状态 == 特征概念规则状态::已完成
            && 类型关系.特征类型根 == 当前.服务任务特征概念节点;
    } catch (...) {
        return false;
    }
}

bool 确保交互对象集合特征(
    普通应用上下文& 当前, 稳定编码 自我存在节点) noexcept {
    std::wstring_view 阶段 = L"入口参数与先天概念读取";
    const auto 失败 = [&]() noexcept {
        try { (void)记录运行日志(L"普通应用", L"交互对象集合特征失败阶段", 阶段); }
        catch (...) {}
        return false;
    };
    try {
        auto& fc = 当前.世界事实.特征概念服务;
        const auto 先天 = fc.获取先天特征概念();
        const auto e = 当前.世界事实.存在服务.获取存在(自我存在节点);
        if (!先天 || !e || !当前.中文语言节点) return 失败();
        阶段 = L"交互对象集合类型定义核验";
        const auto 根 = fc.获取特征概念(先天->交互对象集合);
        if (!根 || 根->比较规则 != 特征概念比较规则::存在集合身份判等
            || 根->材料类型 != 特征概念材料物理类型::U64数组 || 根->单位
            || 根->聚合规则 != 特征概念聚合规则::完整材料精确集合) return 失败();
        阶段 = L"交互对象词条建立";
        const auto 词 = 当前.世界事实.词服务.建立或取得词条({*当前.中文语言节点,
            严格UTF8字节精确规范化合同节点, "交互对象", std::nullopt});
        if (!词.词条节点) return 失败();
        阶段 = L"交互对象名称关系建立";
        const auto 名 = 当前.世界事实.名称关系服务.建立或取得名称关系(根->节点, *词.词条节点);
        if (!名.名称关系节点) return 失败();
        std::optional<稳定编码> f;
        阶段 = L"自我既有特征枚举与类型核验";
        for (const auto n : e->特征节点组) {
            const auto v = 当前.世界事实.特征服务.获取特征(n);
            if (!v) return 失败();
            const auto r = fc.比较单特征概念(v->特征概念节点, v->特征概念节点);
            if (r.状态 != 特征概念规则状态::已完成 || !r.特征类型根) return 失败();
            if (*r.特征类型根 != 根->节点) continue;
            if (f || v->上级特征节点) return 失败();
            f = n;
        }
        if (!f) {
            阶段 = L"空集合材料、时间及所属场景读取";
            const auto 空 = 当前.世界事实.特征值服务.保存或取得特征值(
                材料物理类型::U64数组, std::vector<std::uint64_t>{});
            const auto 时间 = 当前UTC纳秒();
            const auto 场景 = 当前.世界事实.场景服务.查询所属场景(自我存在节点);
            if (!有效(空) || !时间 || !场景) return 失败();
            阶段 = L"场景中添加交互对象集合特征";
            const auto a = 当前.世界树.给存在添加特征(*场景, 自我存在节点,
                根->节点, 新特征准确值{空}, {新状态时间语义::实例绝对UTC, *时间});
            if (a.状态 != 新场景操作状态::已完成 || !a.存在结果
                || !a.存在结果->特征节点) {
                std::wostringstream 诊断;
                诊断 << L"场景添加状态=" << static_cast<int>(a.状态);
                if (a.存在结果) {
                    诊断 << L"；存在特征添加状态=" << static_cast<int>(a.存在结果->状态);
                    if (a.存在结果->状态结果)
                        诊断 << L"；状态建立状态=" << static_cast<int>(a.存在结果->状态结果->状态);
                } else {
                    诊断 << L"；未返回存在特征添加结果";
                }
                (void)记录运行日志(L"普通应用", L"交互对象集合特征添加返回", 诊断.str());
                return 失败();
            }
            f = *a.存在结果->特征节点;
        }
        阶段 = L"自我存在与当前集合特征读回";
        const auto 后E = 当前.世界事实.存在服务.获取存在(自我存在节点);
        const auto 后F = 当前.世界事实.特征服务.获取特征(*f);
        if (!后E || !后F || !后F->当前值
            || std::count(后E->特征节点组.begin(), 后E->特征节点组.end(), *f) != 1) return 失败();
        阶段 = L"交互对象集合材料保护核验";
        const auto* v = std::get_if<稳定编码>(&*后F->当前值);
        if (!v || fc.核验存在集合材料保护(根->节点, *v, 当前.世界事实.特征值服务)
            != 特征概念规则状态::已完成) return 失败();
        阶段 = L"交互对象集合自身比较读回";
        const auto 读 = 当前.世界事实.特征服务.按概念比较特征值(
            根->节点, *后F->当前值, *后F->当前值);
        if (读.状态 != 新特征比较状态::已完成 || 读.比较值 != 10000) return 失败();
        return true;
    } catch (...) { return 失败(); }
}

bool 同步服务任务集合特征(
    普通应用上下文& 当前,
    const std::vector<稳定编码>& 服务任务节点组) noexcept {
    bool 世界写已调用 = false;
    try {
        if (当前.服务任务集合写后失败 || !当前.服务任务自我存在节点
            || !当前.服务任务特征概念节点 || !当前.服务任务特征节点) return false;
        if (!确保服务任务集合特征(当前, *当前.服务任务自我存在节点)) return false;
        const auto 存在 = 当前.世界事实.存在服务.获取存在(*当前.服务任务自我存在节点);
        const auto 特征 = 当前.世界事实.特征服务.获取特征(*当前.服务任务特征节点);
        if (!存在 || !特征
            || std::count(存在->特征节点组.begin(), 存在->特征节点组.end(),
                *当前.服务任务特征节点) != 1) return false;
        const auto 类型关系 = 当前.世界事实.特征概念服务.比较单特征概念(
            *当前.服务任务特征概念节点, 特征->特征概念节点);
        if (类型关系.状态 != 特征概念规则状态::已完成
            || 类型关系.特征类型根 != 当前.服务任务特征概念节点) return false;
        std::vector<std::uint64_t> 编码组;
        编码组.reserve(服务任务节点组.size());
        for (const auto 编码 : 服务任务节点组) 编码组.push_back(编码.值);
        if (特征->当前值) {
            const auto* 值节点 = std::get_if<稳定编码>(&*特征->当前值);
            if (!值节点) return false;
            const auto 值 = 当前.世界事实.特征值服务.获取特征值(*值节点);
            if (!值 || 值->物理类型 != 材料物理类型::U64数组) return false;
            const auto* 已存编码 = std::get_if<std::vector<std::uint64_t>>(&值->材料);
            if (!已存编码) return false;
            if (*已存编码 == 编码组) return true;
        }
        const auto 材料节点 = 当前.世界事实.特征值服务.保存或取得特征值(
            材料物理类型::U64数组, 新特征值材料{编码组});
        const auto 时间 = 当前UTC纳秒();
        const auto 场景 = 当前.世界事实.场景服务.查询所属场景(*当前.服务任务自我存在节点);
        if (!有效(材料节点) || !时间 || !场景) return false;
        世界写已调用 = true;
        const auto 更新 = 当前.世界树.更新存在特征并形成动态(
            *场景, *当前.服务任务自我存在节点, *当前.服务任务特征节点,
            新特征准确值{材料节点}, {新状态时间语义::实例绝对UTC, *时间});
        if (更新.状态 != 新世界特征变化状态::已完成
            && 更新.状态 != 新世界特征变化状态::无变化
            && 更新.状态 != 新世界特征变化状态::已生成状态但无可连接前状态) {
            当前.服务任务集合写后失败 = true;
            return false;
        }
        const auto 读回 = 当前.世界事实.特征服务.获取特征(*当前.服务任务特征节点);
        const auto* 值节点 = 读回 && 读回->当前值
            ? std::get_if<稳定编码>(&*读回->当前值) : nullptr;
        const auto 值 = 值节点 ? 当前.世界事实.特征值服务.获取特征值(*值节点)
            : std::optional<新特征值信息>{};
        const auto* 实际编码 = 值 && 值->物理类型 == 材料物理类型::U64数组
            ? std::get_if<std::vector<std::uint64_t>>(&值->材料) : nullptr;
        if (!实际编码 || *实际编码 != 编码组
            || !确保服务任务集合特征(当前, *当前.服务任务自我存在节点)) {
            当前.服务任务集合写后失败 = true;
            return false;
        }
        return true;
    } catch (...) {
        if (世界写已调用) 当前.服务任务集合写后失败 = true;
        return false;
    }
}

bool 是有效UTF8(std::string_view 文本) noexcept {
    if (文本.empty()) return false;
    std::size_t 位置 = 0;
    while (位置 < 文本.size()) {
        const auto 首字节 = static_cast<unsigned char>(文本[位置]);
        std::size_t 后续数量 = 0;
        std::uint32_t 码点 = 0;
        if (首字节 <= 0x7F) {
            ++位置;
            continue;
        }
        if ((首字节 & 0xE0) == 0xC0) {
            后续数量 = 1;
            码点 = 首字节 & 0x1F;
        } else if ((首字节 & 0xF0) == 0xE0) {
            后续数量 = 2;
            码点 = 首字节 & 0x0F;
        } else if ((首字节 & 0xF8) == 0xF0) {
            后续数量 = 3;
            码点 = 首字节 & 0x07;
        } else {
            return false;
        }
        if (位置 + 后续数量 >= 文本.size()) return false;
        for (std::size_t i = 1; i <= 后续数量; ++i) {
            const auto 字节 = static_cast<unsigned char>(文本[位置 + i]);
            if ((字节 & 0xC0) != 0x80) return false;
            码点 = (码点 << 6) | (字节 & 0x3F);
        }
        if ((后续数量 == 1 && 码点 < 0x80)
            || (后续数量 == 2 && 码点 < 0x800)
            || (后续数量 == 3 && 码点 < 0x10000)
            || (码点 >= 0xD800 && 码点 <= 0xDFFF)
            || 码点 > 0x10FFFF) {
            return false;
        }
        位置 += 后续数量 + 1;
    }
    return true;
}

bool 二次概念建立成功(二次特征概念操作状态 状态) noexcept {
    return 状态 == 二次特征概念操作状态::已建立
        || 状态 == 二次特征概念操作状态::已复用;
}

bool 特征属于类型根(
    const 普通应用上下文& 当前,
    稳定编码 特征概念节点,
    稳定编码 特征类型根) noexcept {
    const auto 关系 = 当前.世界事实.特征概念服务.比较单特征概念(
        特征概念节点, 特征类型根);
    return 关系.状态 == 特征概念规则状态::已完成
        && 关系.特征类型根 == 特征类型根
        && 关系.关系
        && (*关系.关系 == 单特征概念关系::同一
            || *关系.关系 == 单特征概念关系::右为上位);
}

// 绑定只接受实际存在的登录名当前值；启动配置不是世界事实的替代物。
网页服务输入状态 可信服务对象可绑定(普通应用上下文& 当前) {
    using 状态 = 网页服务输入状态;
    if (!当前.第一权限人存在节点 || !当前.人类存在概念节点
        || !当前.登录名特征概念节点) return 状态::结构不一致;
    const auto 人 = 当前.世界事实.存在服务.获取存在(*当前.第一权限人存在节点);
    if (!人 || 人->节点 != *当前.第一权限人存在节点) return 状态::结构不一致;
    std::size_t 登录名数量 = 0;
    bool 登录名匹配 = false;
    for (const auto 身份 : 人->特征节点组) {
        const auto 特征 = 当前.世界事实.特征服务.获取特征(身份);
        if (!特征 || 特征->节点 != 身份) return 状态::结构不一致;
        const auto 关系 = 当前.世界事实.特征概念服务.比较单特征概念(
            特征->特征概念节点, *当前.登录名特征概念节点);
        if (关系.状态 == 特征概念规则状态::资源失败) return 状态::资源失败;
        if (关系.状态 != 特征概念规则状态::已完成) return 状态::结构不一致;
        if (关系.特征类型根 != 当前.登录名特征概念节点) continue;
        ++登录名数量;
        const auto* 值节点 = 特征->当前值
            ? std::get_if<稳定编码>(&*特征->当前值) : nullptr;
        if (!值节点) return 状态::结构不一致;
        const auto 值 = 当前.世界事实.特征值服务.获取特征值(*值节点);
        const auto* 文本 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
        if (!文本 || 值->节点 != *值节点 || 值->物理类型 != 材料物理类型::UTF8字符串)
            return 状态::结构不一致;
        登录名匹配 = *文本 == 当前.配置.本次网页会话服务对象登录名;
    }
    if (登录名数量 != 1) return 状态::结构不一致;
    bool 已见人类 = false;
    std::set<稳定编码> 路径, 已读;
    std::vector<std::pair<稳定编码, bool>> 栈{{人->专属存在概念节点, false}};
    while (!栈.empty()) {
        const auto [节点, 退出] = 栈.back(); 栈.pop_back();
        if (退出) { 路径.erase(节点); 已读.insert(节点); continue; }
        if (路径.contains(节点)) return 状态::结构不一致;
        if (已读.contains(节点)) continue;
        const auto 概念 = 当前.世界事实.存在概念服务.获取存在概念(节点);
        if (!概念 || 概念->节点 != 节点) return 状态::结构不一致;
        if (节点 == *当前.人类存在概念节点) 已见人类 = true;
        路径.insert(节点); 栈.emplace_back(节点, true);
        for (auto i = 概念->上位概念组.rbegin(); i != 概念->上位概念组.rend(); ++i)
            栈.emplace_back(*i, false);
    }
    return 登录名匹配 && 已见人类 ? 状态::已完成 : 状态::对象不匹配;
}

网页服务输入状态 核验服务会话(普通应用上下文& 当前, std::string_view 会话) {
    if (!当前.装配结果 || !当前.装配结果->成功()) return 网页服务输入状态::结构不一致;
    if (!当前.活跃网页会话标识 || *当前.活跃网页会话标识 != 会话)
        return 网页服务输入状态::会话失效;
    if (!当前.网页会话绑定人类) return 网页服务输入状态::未绑定;
    if (当前.网页会话绑定人类 != 当前.第一权限人存在节点)
        return 网页服务输入状态::对象不匹配;
    return 可信服务对象可绑定(当前);
}

网页服务输入状态 映射服务要求状态(结构化服务要求状态 状态) noexcept {
    switch (状态) {
    case 结构化服务要求状态::已建立:
    case 结构化服务要求状态::已读取: return 网页服务输入状态::已完成;
    case 结构化服务要求状态::已复用: return 网页服务输入状态::已复用;
    case 结构化服务要求状态::会话未绑定: return 网页服务输入状态::未绑定;
    case 结构化服务要求状态::会话已失效: return 网页服务输入状态::会话失效;
    case 结构化服务要求状态::目标不完整: return 网页服务输入状态::输入不完整;
    case 结构化服务要求状态::目标不合法: return 网页服务输入状态::输入不合法;
    case 结构化服务要求状态::来源不合法:
    case 结构化服务要求状态::确认对象不匹配: return 网页服务输入状态::对象不匹配;
    case 结构化服务要求状态::理解不存在: return 网页服务输入状态::理解不存在;
    case 结构化服务要求状态::资源失败: return 网页服务输入状态::资源失败;
    case 结构化服务要求状态::需重新确认: return 网页服务输入状态::需重新确认;
    case 结构化服务要求状态::语义快照暂不支持: return 网页服务输入状态::语义快照暂不支持;
    default: return 网页服务输入状态::结构不一致;
    }
}

bool 服务输入成功(网页服务输入状态 状态) noexcept {
    return 状态 == 网页服务输入状态::已完成 || 状态 == 网页服务输入状态::已复用;
}

网页服务输入状态 核验服务要求现实目标(普通应用上下文& 当前, 稳定编码 目标) {
    if (!目标.值) return 网页服务输入状态::输入不合法;
    const auto 范围 = 当前.世界事实.场景服务.读取本树范围成员完整();
    if (范围.状态 == 新场景范围成员读取状态::读取已变化)
        return 网页服务输入状态::目标范围已变化;
    if (范围.状态 != 新场景范围成员读取状态::已完成 || !范围.根场景节点)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验服务要求现实目标", L"现实场景完整范围读取发生工程故障。");
    if (目标 == *范围.根场景节点
        || std::any_of(范围.场景成员组.begin(), 范围.场景成员组.end(),
            [&](const auto& 项) { return 项.场景节点 == 目标; })
        || std::find(范围.范围成员存在组.begin(), 范围.范围成员存在组.end(), 目标)
            != 范围.范围成员存在组.end()) return 网页服务输入状态::已完成;
    return 网页服务输入状态::输入不合法;
}

void 重述特征语义(std::ostringstream& 文本, const 服务要求特征语义快照& 语义) {
    文本 << "概念#" << 语义.概念节点.值 << "，材料型=" << static_cast<std::int64_t>(语义.材料类型)
        << "，单位=";
    if (语义.单位) 文本 << '#' << 语义.单位->值;
    else 文本 << "无";
    文本 << "，比较规则=" << static_cast<std::int64_t>(语义.比较规则);
}

void 重述数值区间(std::ostringstream& 文本, const std::vector<特征概念I64闭区间>& 区间组) {
    文本 << '{';
    for (std::size_t i = 0; i < 区间组.size(); ++i) {
        if (i) 文本 << ',';
        文本 << '[' << 区间组[i].下界 << ',' << 区间组[i].上界 << ']';
    }
    文本 << '}';
}

bool 重述固定值域(普通应用上下文& 当前, std::ostringstream& 文本,
    const 服务要求值域语义快照& 快照) {
    重述特征语义(文本, 快照.特征语义);
    文本 << "，值域=";
    if (const auto* 区间 = std::get_if<std::vector<特征概念I64闭区间>>(&快照.值域)) {
        重述数值区间(文本, *区间);
    } else if (std::holds_alternative<特征概念全材料值域>(快照.值域)) {
        文本 << "该材料型的全部合法材料";
    } else if (const auto* 材料组 = std::get_if<std::vector<稳定编码>>(&快照.值域)) {
        文本 << '{';
        for (std::size_t i = 0; i < 材料组->size(); ++i) {
            const auto 值 = 当前.世界事实.特征值服务.获取特征值((*材料组)[i]);
            if (!值 || 值->节点 != (*材料组)[i]) return false;
            if (i) 文本 << ';';
            文本 << "材料#" << 值->节点.值 << "(物理型=" << static_cast<std::int64_t>(值->物理类型) << ")=";
            if (const auto* 字符串 = std::get_if<std::string>(&值->材料)) {
                // 长度限定边界，即使原文包含分隔符，也不改变重述结构。
                文本 << "UTF8字节数" << 字符串->size() << ':' << *字符串;
            } else if (const auto* 数组 = std::get_if<std::vector<std::uint64_t>>(&值->材料)) {
                文本 << '[';
                for (std::size_t j = 0; j < 数组->size(); ++j) { if (j) 文本 << ','; 文本 << (*数组)[j]; }
                文本 << ']';
            } else if (const auto* 数组 = std::get_if<std::vector<std::int64_t>>(&值->材料)) {
                文本 << '[';
                for (std::size_t j = 0; j < 数组->size(); ++j) { if (j) 文本 << ','; 文本 << (*数组)[j]; }
                文本 << ']';
            } else return false;
        }
        文本 << '}';
    } else if (const auto* 结构 = std::get_if<特征概念结构化I64值域>(&快照.值域)) {
        文本 << "有序分量{";
        for (std::size_t i = 0; i < 结构->分量.size(); ++i) {
            const auto& 分量 = 结构->分量[i];
            if (i) 文本 << ';';
            文本 << "序号=" << i << "，角色#" << 分量.角色.值 << "，单位=";
            if (分量.单位) 文本 << '#' << 分量.单位->值; else 文本 << "无";
            文本 << "，值域=";
            重述数值区间(文本, 分量.值域);
        }
        文本 << '}';
    } else return false;
    return true;
}

std::optional<网页服务要求显示信息> 形成服务要求显示(普通应用上下文& 当前,
    const 服务要求理解信息& 理解, std::optional<稳定编码> 确认 = std::nullopt) {
    网页服务要求显示信息 显示;
    显示.理解编码 = 理解.节点.值;
    显示.服务对象编码 = 理解.来源人类节点.值;
    显示.目标.目标存在编码 = 理解.目标存在节点.值;
    显示.目标.特征类型编码 = 理解.目标.目标特征类型概念节点.值;
    显示.目标.目标形式 = static_cast<std::int64_t>(理解.目标.目标形式);
    if (理解.目标.目标值域概念节点) 显示.目标.目标域编码 = 理解.目标.目标值域概念节点->值;
    if (理解.目标.目标比较关系概念节点) 显示.目标.比较概念编码 = 理解.目标.目标比较关系概念节点->值;
    if (理解.目标.目标变化方向) 显示.目标.变化方向 = static_cast<std::int64_t>(*理解.目标.目标变化方向);
    if (确认) 显示.确认编码 = 确认->值;
    std::ostringstream 文本;
    文本 << (确认 ? "已明确确认" : "尚未确认") << "；理解#" << 理解.节点.值
        << "；需求主体#" << 理解.来源人类节点.值
        << "；目标存在#" << 理解.目标存在节点.值 << "；目标特征：";
    重述特征语义(文本, 理解.固定语义.目标类型);
    if (理解.目标.目标形式 == 新需求目标形式::值) {
        if (!理解.固定语义.目标域 || !理解.固定语义.比较) return std::nullopt;
        文本 << "；希望达到目标域：";
        if (!重述固定值域(当前, 文本, *理解.固定语义.目标域)) return std::nullopt;
        const auto& 比较 = *理解.固定语义.比较;
        文本 << "；采用比较概念#" << 比较.比较概念节点.值 << "，类型根#" << 比较.类型根节点.值
            << "，算法=" << static_cast<std::int64_t>(比较.算法) << "，有序输入={";
        for (std::size_t i = 0; i < 比较.输入定义组.size(); ++i) {
            const auto& 输入 = 比较.输入定义组[i];
            if (i) 文本 << ';';
            文本 << "序号=" << 输入.顺序 << "，角色=" << static_cast<std::int64_t>(输入.角色)
                << "，来源=" << static_cast<std::int64_t>(输入.来源种类) << "，特征类型=";
            if (输入.特征类型概念节点) 文本 << '#' << 输入.特征类型概念节点->值; else 文本 << "无";
        }
        文本 << "}，参照=";
        if (比较.参照概念节点) 文本 << '#' << 比较.参照概念节点->值; else 文本 << "默认0";
        文本 << "；比较输出：";
        if (!重述固定值域(当前, 文本, 比较.输出语义)) return std::nullopt;
        文本 << "，结果域种类={";
        for (std::size_t i = 0; i < 比较.结果域种类组.size(); ++i) {
            if (i) 文本 << ',';
            文本 << static_cast<std::int64_t>(比较.结果域种类组[i]);
        }
        文本 << '}';
    } else if (理解.目标.目标形式 == 新需求目标形式::变化方向 && 理解.目标.目标变化方向) {
        文本 << "；希望该特征沿" << (*理解.目标.目标变化方向 == 新需求变化方向::增加 ? "增加" : "减少") << "方向变化";
    } else return std::nullopt;
    显示.UTF8重述 = 文本.str();
    return 显示;
}

bool 建立第一权限人(普通应用上下文& 当前, 稳定编码 根场景) noexcept {
    try {
        auto& 语言服务 = 当前.世界事实.语言服务;
        auto& 词服务 = 当前.世界事实.词服务;
        auto& 名称服务 = 当前.世界事实.名称关系服务;
        auto& 特征值服务 = 当前.世界事实.特征值服务;
        auto& 特征概念服务 = 当前.世界事实.特征概念服务;

        if (!当前.中文语言节点) {
            const auto 建立 = 语言服务.建立或取得语言({"zh-CN", std::nullopt});
            if ((建立.状态 != 新语言建立状态::已建立
                    && 建立.状态 != 新语言建立状态::已复用)
                || !建立.语言节点) {
                return false;
            }
            当前.中文语言节点 = 建立.语言节点;
        }

        const auto 建立词条 = [&](std::optional<稳定编码>& 保存位置,
            const std::string& 词面) {
            if (保存位置) {
                const auto 读回 = 词服务.获取词条(*保存位置);
                return 读回.状态 == 新词条查询状态::已找到
                    && 读回.信息 && 读回.信息->规范化词面 == 词面;
            }
            const auto 建立 = 词服务.建立或取得词条({
                *当前.中文语言节点,
                严格UTF8字节精确规范化合同节点,
                词面,
                std::nullopt});
            if ((建立.状态 != 新词条建立状态::已建立
                    && 建立.状态 != 新词条建立状态::已复用)
                || !建立.词条节点) {
                return false;
            }
            保存位置 = 建立.词条节点;
            return true;
        };
        if (!建立词条(当前.登录名词条, "登录名")
            || !建立词条(当前.姓名词条, "姓名")
            || !建立词条(当前.权限角色词条, "权限角色")
            || !建立词条(当前.登录名值词条, 当前.配置.第一权限人登录名)
            || !建立词条(当前.姓名值词条, 当前.配置.第一权限人姓名)
            || !建立词条(当前.第一权限人值词条, "第一权限人")) {
            return false;
        }

        const auto 建立值 = [&](std::optional<稳定编码>& 保存位置,
            const std::string& 值) {
            if (保存位置) {
                const auto 读回 = 特征值服务.获取特征值(*保存位置);
                return 读回 && 读回->物理类型 == 材料物理类型::UTF8字符串
                    && 读回->材料 == 新特征值材料{值};
            }
            const auto 节点 = 特征值服务.保存或取得特征值(
                材料物理类型::UTF8字符串, 新特征值材料{值});
            if (!有效(节点)) return false;
            保存位置 = 节点;
            return true;
        };
        if (!建立值(当前.登录名值节点, 当前.配置.第一权限人登录名)
            || !建立值(当前.姓名值节点, 当前.配置.第一权限人姓名)
            || !建立值(当前.第一权限人值节点, "第一权限人")) {
            return false;
        }

        const auto 建立类型概念 = [&](std::optional<稳定编码>& 保存位置) {
            if (保存位置) return 特征概念服务.是特征概念节点(*保存位置);
            特征概念定义 定义;
            定义.材料类型 = 特征概念材料物理类型::UTF8字符串;
            定义.值域 = 特征概念全材料值域{};
            定义.比较规则 = 特征概念比较规则::完整材料精确;
            定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
            const auto 节点 = 特征概念服务.建立或取得特征概念(
                定义, 特征值服务);
            if (!有效(节点)) return false;
            保存位置 = 节点;
            return true;
        };
        if (!建立类型概念(当前.登录名特征概念节点)
            || !建立类型概念(当前.姓名特征概念节点)
            || !建立类型概念(当前.权限角色特征概念节点)) {
            return false;
        }

        const auto 建立精确概念 = [&](std::optional<稳定编码>& 保存位置,
            稳定编码 类型概念, 稳定编码 值节点) {
            if (保存位置) return 特征概念服务.是特征概念节点(*保存位置);
            特征概念定义 定义;
            定义.材料类型 = 特征概念材料物理类型::UTF8字符串;
            定义.值域 = std::vector<稳定编码>{值节点};
            定义.比较规则 = 特征概念比较规则::完整材料精确;
            定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
            const auto 节点 = 特征概念服务.建立或取得特征概念(
                定义, 特征值服务, 类型概念);
            if (!有效(节点)) return false;
            保存位置 = 节点;
            return true;
        };
        if (!建立精确概念(当前.登录名精确概念节点,
                *当前.登录名特征概念节点, *当前.登录名值节点)
            || !建立精确概念(当前.姓名精确概念节点,
                *当前.姓名特征概念节点, *当前.姓名值节点)
            || !建立精确概念(当前.第一权限人精确概念节点,
                *当前.权限角色特征概念节点, *当前.第一权限人值节点)) {
            return false;
        }

        const auto 建立名称 = [&](稳定编码 概念, 稳定编码 词条) {
            const auto 建立 = 名称服务.建立或取得名称关系(概念, 词条);
            if ((建立.状态 != 概念名称关系建立状态::已建立
                    && 建立.状态 != 概念名称关系建立状态::已复用)
                || !建立.名称关系节点) {
                return false;
            }
            if (std::find(当前.第一权限人名称关系.begin(),
                    当前.第一权限人名称关系.end(),
                    *建立.名称关系节点)
                == 当前.第一权限人名称关系.end()) {
                当前.第一权限人名称关系.push_back(*建立.名称关系节点);
            }
            return true;
        };
        if (!建立名称(*当前.登录名特征概念节点, *当前.登录名词条)
            || !建立名称(*当前.姓名特征概念节点, *当前.姓名词条)
            || !建立名称(*当前.权限角色特征概念节点, *当前.权限角色词条)
            || !建立名称(*当前.登录名精确概念节点, *当前.登录名值词条)
            || !建立名称(*当前.姓名精确概念节点, *当前.姓名值词条)
            || !建立名称(*当前.第一权限人精确概念节点,
                *当前.第一权限人值词条)) {
            return false;
        }

        if (!当前.第一权限人存在节点) {
            const auto 时间 = 当前UTC纳秒();
            if (!时间) return false;
            新存在建立请求 请求;
            请求.初始特征组 = {
                {*当前.登录名精确概念节点,
                    新特征准确值{*当前.登录名值节点}},
                {*当前.姓名精确概念节点,
                    新特征准确值{*当前.姓名值节点}},
                {*当前.第一权限人精确概念节点,
                    新特征准确值{*当前.第一权限人值节点}}};
            const auto 建立 = 当前.世界树.建立场景内存在(
                根场景, 请求,
                {新状态时间语义::实例绝对UTC, *时间});
            if (建立.状态 != 新场景操作状态::已完成
                || !建立.存在节点 || !建立.专属存在概念节点) {
                return false;
            }
            当前.第一权限人存在节点 = 建立.存在节点;
            当前.第一权限人专属存在概念节点 = 建立.专属存在概念节点;
        }
        if (!当前.第一权限人专属存在概念节点) {
            当前.第一权限人专属存在概念节点 =
                当前.世界事实.存在服务.查询专属存在概念(
                    *当前.第一权限人存在节点);
        }
        if (!当前.第一权限人专属存在概念节点
            || !建立名称(*当前.第一权限人专属存在概念节点,
                *当前.姓名值词条)) {
            return false;
        }

        const auto 存在读回 = 当前.世界树.查询存在(
            *当前.第一权限人存在节点);
        const auto 概念读回 = 当前.世界事实.存在概念服务.获取存在概念(
            *当前.第一权限人专属存在概念节点);
        return 存在读回 && 概念读回
            && 存在读回->专属存在概念节点
                == *当前.第一权限人专属存在概念节点
            && 概念读回->对应存在节点
            && *概念读回->对应存在节点 == *当前.第一权限人存在节点
            && 概念读回->特征组.size() == (当前.第一权限人性别特征节点 ? 4u : 3u);
    } catch (...) {
        return false;
    }
}

bool 建立人类描述特征(普通应用上下文& 当前) noexcept {
    try {
        const auto 名字 = 当前.世界事实.词服务.建立或取得词条({*当前.中文语言节点,
            严格UTF8字节精确规范化合同节点, "名字", std::nullopt});
        const auto 性别 = 当前.世界事实.词服务.建立或取得词条({*当前.中文语言节点,
            严格UTF8字节精确规范化合同节点, "性别", std::nullopt});
        if (!名字.词条节点 || !性别.词条节点) return false;
        if (!当前.性别特征概念节点) {
            特征概念定义 定义;
            定义.材料类型 = 特征概念材料物理类型::UTF8字符串;
            定义.值域 = 特征概念全材料值域{};
            定义.比较规则 = 特征概念比较规则::完整材料精确;
            定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
            const auto 节点 = 当前.世界事实.特征概念服务.建立或取得特征概念(定义, 当前.世界事实.特征值服务);
            if (!有效(节点)) return false;
            当前.性别特征概念节点 = 节点;
        }
        const auto 名字关系 = 当前.世界事实.名称关系服务.建立或取得名称关系(*当前.姓名特征概念节点, *名字.词条节点);
        const auto 性别关系 = 当前.世界事实.名称关系服务.建立或取得名称关系(*当前.性别特征概念节点, *性别.词条节点);
        if (!名字关系.名称关系节点 || !性别关系.名称关系节点) return false;
        if (!当前.第一权限人性别特征节点) {
            const auto 添加 = 当前.世界事实.存在服务.添加特征区间值(*当前.第一权限人存在节点,
                *当前.性别特征概念节点, 特征概念全材料值域{});
            if (添加.状态 != 新存在操作状态::已完成 || !添加.特征节点) return false;
            当前.第一权限人性别特征节点 = 添加.特征节点;
        }
        const auto 特征 = 当前.世界事实.特征服务.获取特征(*当前.第一权限人性别特征节点);
        return 特征.has_value();
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立人类描述特征", L"描述初始化异常。", 异常.what());
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立人类描述特征", L"描述初始化未知异常。"); }
}

bool 建立人类存在概念(普通应用上下文& 当前) noexcept {
    try {
        if (!当前.中文语言节点 || !当前.第一权限人专属存在概念节点) {
            return false;
        }
        auto& 词服务 = 当前.世界事实.词服务;
        auto& 名称服务 = 当前.世界事实.名称关系服务;
        auto& 存在概念服务 = 当前.世界事实.存在概念服务;

        if (!当前.人类词条) {
            const auto 建立 = 词服务.建立或取得词条({
                *当前.中文语言节点,
                严格UTF8字节精确规范化合同节点,
                "人类", std::nullopt});
            if ((建立.状态 != 新词条建立状态::已建立
                    && 建立.状态 != 新词条建立状态::已复用)
                || !建立.词条节点) {
                return false;
            }
            当前.人类词条 = 建立.词条节点;
        }

        if (!当前.人类存在概念节点) {
            const auto 候选关系 = 名称服务.查询词条候选概念(*当前.人类词条);
            if (候选关系.状态 != 概念名称关系查询状态::已找到
                && 候选关系.状态 != 概念名称关系查询状态::未找到) {
                return false;
            }
            for (const auto 关系节点 : 候选关系.名称关系节点组) {
                const auto 关系 = 名称服务.获取名称关系(关系节点);
                if (关系.状态 != 概念名称关系查询状态::已找到
                    || !关系.信息) {
                    return false;
                }
                if (!存在概念服务.是抽象存在概念节点(
                        关系.信息->概念节点)) {
                    continue;
                }
                if (当前.人类存在概念节点
                    && *当前.人类存在概念节点 != 关系.信息->概念节点) {
                    return false;
                }
                当前.人类存在概念节点 = 关系.信息->概念节点;
                当前.人类名称关系 = 关系节点;
            }
        }

        if (!当前.人类存在概念节点) {
            const auto 建立 = 存在概念服务.建立人工抽象存在概念(
                {*当前.第一权限人专属存在概念节点});
            if (建立.状态 != 存在概念操作状态::已建立
                || !建立.概念节点) {
                return false;
            }
            当前.人类存在概念节点 = 建立.概念节点;
        }

        if (!当前.人类名称关系) {
            const auto 建立 = 名称服务.建立或取得名称关系(
                *当前.人类存在概念节点, *当前.人类词条);
            if ((建立.状态 != 概念名称关系建立状态::已建立
                    && 建立.状态 != 概念名称关系建立状态::已复用)
                || !建立.名称关系节点) {
                return false;
            }
            当前.人类名称关系 = 建立.名称关系节点;
        }

        const auto 人类 = 存在概念服务.获取存在概念(
            *当前.人类存在概念节点);
        const auto 名称 = 名称服务.获取名称关系(*当前.人类名称关系);
        const auto 周春海上位 = 存在概念服务.查询上位存在概念(
            *当前.第一权限人专属存在概念节点);
        return 人类 && 人类->种类 == 存在概念种类::抽象
            && std::find(人类->下位概念组.begin(), 人类->下位概念组.end(),
                *当前.第一权限人专属存在概念节点)
                != 人类->下位概念组.end()
            && std::find(周春海上位.begin(), 周春海上位.end(),
                *当前.人类存在概念节点) != 周春海上位.end()
            && 名称.状态 == 概念名称关系查询状态::已找到
            && 名称.信息
            && 名称.信息->概念节点 == *当前.人类存在概念节点
            && 名称.信息->词条节点 == *当前.人类词条;
    } catch (...) {
        return false;
    }
}

bool 建立确认二次特征概念(普通应用上下文& 当前) noexcept {
    try {
        if (!当前.中文语言节点) return false;
        auto& 二次概念服务 = 当前.世界事实.二次特征概念服务;
        auto& 词服务 = 当前.世界事实.词服务;
        auto& 名称服务 = 当前.世界事实.名称关系服务;

        const std::vector<二次特征概念输入定义> 输入定义组{
            {0, 二次特征输入角色::左输入,
                二次特征输入来源种类::自然语言场景信息节点,
                std::nullopt},
            {1, 二次特征输入角色::右输入,
                二次特征输入来源种类::自然语言场景信息节点,
                std::nullopt}};
        if (!当前.语言语义差异二次特征类型根) {
            const auto 建立 = 二次概念服务.建立或取得默认二次特征概念(
                二次特征算法::语言语义结构差异, 输入定义组);
            if (!二次概念建立成功(建立.状态) || !建立.概念节点) {
                return false;
            }
            当前.语言语义差异二次特征类型根 = 建立.概念节点;
        }
        if (!当前.无法确认结果概念) {
            const auto 建立 = 二次概念服务.建立或取得参照结果域概念(
                *当前.语言语义差异二次特征类型根,
                二次特征结果域种类::小于);
            if (!二次概念建立成功(建立.状态) || !建立.概念节点) {
                return false;
            }
            当前.无法确认结果概念 = 建立.概念节点;
        }
        if (!当前.无法确认词条) {
            const auto 建立 = 词服务.建立或取得词条({
                *当前.中文语言节点,
                严格UTF8字节精确规范化合同节点,
                "无法确认", std::nullopt});
            if ((建立.状态 != 新词条建立状态::已建立
                    && 建立.状态 != 新词条建立状态::已复用)
                || !建立.词条节点) {
                return false;
            }
            当前.无法确认词条 = 建立.词条节点;
        }
        if (!当前.无法确认名称关系) {
            const auto 建立 = 名称服务.建立或取得名称关系(
                *当前.无法确认结果概念, *当前.无法确认词条);
            if ((建立.状态 != 概念名称关系建立状态::已建立
                    && 建立.状态 != 概念名称关系建立状态::已复用)
                || !建立.名称关系节点) {
                return false;
            }
            当前.无法确认名称关系 = 建立.名称关系节点;
        }
        if (!当前.确认结果概念) {
            const auto 建立 = 二次概念服务.建立或取得参照结果域概念(
                *当前.语言语义差异二次特征类型根,
                二次特征结果域种类::等于);
            if (!二次概念建立成功(建立.状态) || !建立.概念节点) {
                return false;
            }
            当前.确认结果概念 = 建立.概念节点;
        }
        if (!当前.确认词条) {
            const auto 建立 = 词服务.建立或取得词条({
                *当前.中文语言节点,
                严格UTF8字节精确规范化合同节点,
                "确认", std::nullopt});
            if ((建立.状态 != 新词条建立状态::已建立
                    && 建立.状态 != 新词条建立状态::已复用)
                || !建立.词条节点) {
                return false;
            }
            当前.确认词条 = 建立.词条节点;
        }
        if (!当前.确认名称关系) {
            const auto 建立 = 名称服务.建立或取得名称关系(
                *当前.确认结果概念, *当前.确认词条);
            if ((建立.状态 != 概念名称关系建立状态::已建立
                    && 建立.状态 != 概念名称关系建立状态::已复用)
                || !建立.名称关系节点) {
                return false;
            }
            当前.确认名称关系 = 建立.名称关系节点;
        }

        const auto 类型根 = 二次概念服务.获取二次特征概念(
            *当前.语言语义差异二次特征类型根);
        const auto 无法确认 = 二次概念服务.获取二次特征概念(
            *当前.无法确认结果概念);
        const auto 确认 = 二次概念服务.获取二次特征概念(
            *当前.确认结果概念);
        const auto 无法确认词 = 词服务.获取词条(*当前.无法确认词条);
        const auto 词条 = 词服务.获取词条(*当前.确认词条);
        const auto 无法确认名称 = 名称服务.获取名称关系(
            *当前.无法确认名称关系);
        const auto 名称 = 名称服务.获取名称关系(*当前.确认名称关系);
        if (!类型根 || !无法确认 || !确认
            || 类型根->二次特征类型根节点
                != *当前.语言语义差异二次特征类型根
            || 类型根->算法 != 二次特征算法::语言语义结构差异
            || 类型根->输入定义组 != 输入定义组
            || 无法确认->二次特征类型根节点
                != *当前.语言语义差异二次特征类型根
            || 确认->二次特征类型根节点
                != *当前.语言语义差异二次特征类型根
            || 无法确认词.状态 != 新词条查询状态::已找到
            || !无法确认词.信息
            || 无法确认词.信息->规范化词面 != "无法确认"
            || 词条.状态 != 新词条查询状态::已找到
            || !词条.信息 || 词条.信息->规范化词面 != "确认"
            || 无法确认名称.状态 != 概念名称关系查询状态::已找到
            || !无法确认名称.信息
            || 无法确认名称.信息->概念节点
                != *当前.无法确认结果概念
            || 无法确认名称.信息->词条节点 != *当前.无法确认词条
            || 名称.状态 != 概念名称关系查询状态::已找到
            || !名称.信息
            || 名称.信息->概念节点 != *当前.确认结果概念
            || 名称.信息->词条节点 != *当前.确认词条) {
            return false;
        }
        const auto* 类型值域 = std::get_if<
            std::vector<特征概念I64闭区间>>(&类型根->输出特征概念.值域);
        const auto* 无法确认值域 = std::get_if<
            std::vector<特征概念I64闭区间>>(
                &无法确认->输出特征概念.值域);
        const auto* 值域 = std::get_if<std::vector<特征概念I64闭区间>>(
            &确认->输出特征概念.值域);
        return 类型值域 && 类型值域->size() == 1
            && 类型值域->front().下界
                == (std::numeric_limits<std::int64_t>::min)()
            && 类型值域->front().上界
                == (std::numeric_limits<std::int64_t>::max)()
            && 无法确认值域 && 无法确认值域->size() == 1
            && 无法确认值域->front().下界
                == (std::numeric_limits<std::int64_t>::min)()
            && 无法确认值域->front().上界 == -1
            && std::find(无法确认->结果域种类组.begin(),
                无法确认->结果域种类组.end(),
                二次特征结果域种类::小于)
                != 无法确认->结果域种类组.end()
            && 值域 && 值域->size() == 1
            && 值域->front().下界 == 0
            && 值域->front().上界 == 0
            && std::find(确认->结果域种类组.begin(),
                确认->结果域种类组.end(),
                二次特征结果域种类::等于)
                != 确认->结果域种类组.end();
    } catch (...) {
        return false;
    }
}


std::optional<新自我线程初始化请求> 形成自我初始化请求(
    const 普通应用配置& 配置,
    const 新世界初始化结果& 世界,
    const 新自我治理基础结果& 基础,
    稳定编码 人类存在概念节点,
    稳定编码 默认服务对象存在节点) noexcept {
    if (!世界.世界树根场景 || !基础.安全值特征类型根
        || !基础.服务值特征类型根 || !基础.安全值目标概念
        || !基础.服务值目标概念 || !基础.安全值目标比较关系
        || !基础.服务值目标比较关系
        || !有效(人类存在概念节点) || !有效(默认服务对象存在节点)) {
        return std::nullopt;
    }
    const auto 时间 = 当前UTC纳秒();
    if (!时间) return std::nullopt;
    constexpr auto 冷启动初值 =
        (std::numeric_limits<std::int64_t>::max)() / 2;

    新自我线程初始化请求 请求;
    请求.新场景父节点 = *世界.世界树根场景;
    请求.自我建立请求.初始特征组 = {
        {*基础.安全值特征类型根, 新特征准确值{冷启动初值}},
        {*基础.服务值特征类型根, 新特征准确值{冷启动初值}}};
    请求.初始状态时间 = {
        新状态时间语义::实例绝对UTC, *时间};
    请求.人类存在概念节点 = 人类存在概念节点;
    请求.默认服务对象存在节点 = 默认服务对象存在节点;
    请求.本能双根 = {
        {*基础.安全值特征类型根, *基础.安全值目标概念,
            *基础.安全值目标比较关系, 配置.安全根初始权重},
        {*基础.服务值特征类型根, *基础.服务值目标概念,
            *基础.服务值目标比较关系, 配置.服务根初始权重}};
    return 请求.完整()
        ? std::optional<新自我线程初始化请求>{std::move(请求)}
        : std::nullopt;
}

std::string 读取初始化标准文本(const wchar_t* 固定文件名) {
    std::vector<wchar_t> 路径缓冲(260);
    std::filesystem::path 模块路径;
    for (;;) {
        const auto 长度 = GetModuleFileNameW(nullptr, 路径缓冲.data(),
            static_cast<DWORD>(路径缓冲.size()));
        if (!长度) throw std::runtime_error("无法读取当前程序模块绝对路径");
        if (长度 < 路径缓冲.size()) {
            模块路径 = std::wstring{路径缓冲.data(), 长度};
            break;
        }
        if (路径缓冲.size() > (std::numeric_limits<DWORD>::max)() / 2)
            throw std::runtime_error("当前程序模块路径无法完整读取");
        路径缓冲.resize(路径缓冲.size() * 2);
    }
    const auto 路径 = 模块路径.parent_path() / L"初始化材料" / 固定文件名;
    std::ifstream 文件(路径, std::ios::binary);
    if (!文件.is_open()) throw std::runtime_error("无法打开程序旁的固定初始化资源");
    std::string 原文;
    std::array<char, 8192> 字节缓冲{};
    while (文件) {
        文件.read(字节缓冲.data(), static_cast<std::streamsize>(字节缓冲.size()));
        const auto 数量 = 文件.gcount();
        if (数量 > 0) 原文.append(字节缓冲.data(), static_cast<std::size_t>(数量));
    }
    if (文件.bad() || !文件.eof()) throw std::runtime_error("初始化标准文本未完整读取到EOF");
    return 原文;
}

void 初始化普通应用概念名称(普通应用上下文& 当前, 普通应用装配结果& 结果,
    const std::string& 标准文本, bool 只读复核) noexcept {
    try {
        if (!当前.中文语言节点)
            throw std::logic_error("概念名称初始化缺少正式中文身份");
        初始化概念名称请求 请求{
            标准文本, *当前.中文语言节点, 严格UTF8字节精确规范化合同节点,
            读取先天概念名称环境(当前.世界事实.特征概念服务), {}};
        auto 材料 = 只读复核
            ? 读取已有概念名称(请求, 当前.世界事实.语言服务, 当前.世界事实.词服务,
                当前.世界事实.名称关系服务, 当前.世界事实.特征概念服务)
            : 初始化已有概念名称(请求, 当前.世界事实.语言服务, 当前.世界事实.词服务,
                当前.世界事实.名称关系服务, 当前.世界事实.特征概念服务);
        当前.初始概念名称 = std::move(材料);
        结果.初始概念名称 = 当前.初始概念名称;
        if (当前.初始概念名称->状态 != 初始化概念名称状态::已完成) {
            const auto& 未完成材料 = *当前.初始概念名称;
            std::ostringstream 原因;
            原因 << "状态=" << static_cast<int>(未完成材料.状态)
                << "，阶段=" << static_cast<int>(未完成材料.阶段);
            if (未完成材料.当前名称项) {
                const auto& 项 = 未完成材料.名称项组[*未完成材料.当前名称项];
                原因 << "，名称=" << 项.词面 << "，概念=" << 项.概念节点.值;
            }
            原因 << "，原因=" << 未完成材料.诊断说明;
            const auto 说明 = 原因.str();
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用概念名称",
                L"固定初始化名称材料未闭合；已发生步骤保留。", 说明.c_str());
        }
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用概念名称",
            L"概念名称解释、发布或结果保存发生工程异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用概念名称",
            L"概念名称解释、发布或结果保存发生未知工程异常。");
    }
}

void 初始化普通应用结构化子概念(普通应用上下文& 当前, 普通应用装配结果& 结果,
    const std::string& 标准文本, bool 只读) noexcept {
    try {
        if (!当前.中文语言节点)
            throw std::logic_error("结构化子概念初始化缺少正式中文身份");
        初始化结构化子概念请求 请求{
            标准文本, *当前.中文语言节点, 严格UTF8字节精确规范化合同节点, {}};
        auto 材料 = 只读
            ? 读取结构化子概念(请求, 当前.世界事实.语言服务, 当前.世界事实.特征概念服务,
                当前.世界事实.词服务, 当前.世界事实.名称关系服务)
            : 初始化结构化子概念(请求, 当前.世界事实.语言服务, 当前.世界事实.特征概念服务,
                当前.世界事实.特征值服务, 当前.世界事实.词服务, 当前.世界事实.名称关系服务);
        当前.结构化子概念 = std::move(材料);
        结果.结构化子概念 = 当前.结构化子概念;
        if (当前.结构化子概念->状态 != 初始化结构化子概念状态::已完成) {
            const auto& 未完成 = *当前.结构化子概念;
            std::ostringstream 原因;
            原因 << "状态=" << static_cast<int>(未完成.状态)
                << "，阶段=" << static_cast<int>(未完成.阶段);
            if (未完成.当前声明编号) 原因 << "，声明=" << *未完成.当前声明编号;
            原因 << "，原因=" << 未完成.诊断说明;
            if (未完成.名称结果) 原因 << "，名称原因=" << 未完成.名称结果->诊断说明;
            const auto 说明 = 原因.str();
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用结构化子概念",
                L"固定子概念材料未闭合；已发生步骤保留。", 说明.c_str());
        }
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用结构化子概念",
            L"子概念解释、预检、构造、命名或结果保存发生工程异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用结构化子概念",
            L"子概念初始化发生未知工程异常。");
    }
}

稳定编码 读取字段询问先天类型(普通应用上下文& 当前,std::uint32_t 槽) {
    const auto t=当前.世界事实.特征概念服务.读取先天特征类型槽(槽);
    if ((t.状态!=特征概念规则状态::已完成 && t.状态!=特征概念规则状态::无变化)
        || !t.类型根 || t.完整保护组.empty())
        throw std::logic_error("字段询问先天FT未就绪");
    return *t.类型根;
}
稳定编码 读取字段询问第三方概念(普通应用上下文& 当前) {
    if (!当前.指代种子) throw std::logic_error("字段询问缺少指代初始化结果");
    std::optional<稳定编码> found;
    for (const auto& item:当前.指代种子->项组) {
        if (!item.定义读回 || !item.定义读回->信息) throw std::logic_error("字段询问指代定义读回未完成");
        const auto& info=*item.定义读回->信息;
        if (info.定义.规则!=指代声明规则::作用域第三方) continue;
        const auto live=当前.世界事实.存在概念服务.获取指代概念(info.节点);
        if (!live.信息 || live.信息->定义!=info.定义 || found)
            throw std::logic_error("字段询问第三方指代不唯一或定义已变化");
        found=info.节点;
    }
    if (!found) throw std::logic_error("字段询问第三方指代缺失");
    return *found;
}
稳定编码 读取行动请求作用域指代概念(普通应用上下文& 当前,指代声明规则 规则,
    const char* 不唯一说明,const char* 缺失说明) {
    if (!当前.指代种子) throw std::logic_error("行动请求缺少指代初始化结果");
    std::optional<稳定编码> found;
    for (const auto& item:当前.指代种子->项组) {
        if (!item.定义读回 || !item.定义读回->信息) throw std::logic_error("行动请求指代定义读回未完成");
        const auto& info=*item.定义读回->信息;
        if (info.定义.规则!=规则) continue;
        const auto live=当前.世界事实.存在概念服务.获取指代概念(info.节点);
        if (!live.信息 || live.信息->定义!=info.定义 || found)
            throw std::logic_error(不唯一说明);
        found=info.节点;
    }
    if (!found) throw std::logic_error(缺失说明);
    return *found;
}
稳定编码 读取行动请求受话者概念(普通应用上下文& 当前) {
    return 读取行动请求作用域指代概念(当前,指代声明规则::作用域受话者,
        "行动请求受话者指代不唯一或定义已变化","行动请求受话者指代缺失");
}
稳定编码 读取行动请求说话者概念(普通应用上下文& 当前) {
    return 读取行动请求作用域指代概念(当前,指代声明规则::作用域说话者,
        "行动请求说话者指代不唯一或定义已变化","行动请求说话者指代缺失");
}
std::vector<初始化语言概念名称绑定> 读取字段询问名称绑定(普通应用上下文& 当前) {
    const auto d=当前.世界事实.存在概念服务.读取先天需求任务方法概念();
    if (!d.信息) throw std::logic_error("字段询问正式需求根未就绪");
    return {{"DEMAND",d.信息->需求},
        {"DEMAND-TARGET-EXISTENCE",读取字段询问先天类型(当前,1003)},
        {"DEMAND-TARGET-FEATURE",读取字段询问先天类型(当前,1004)},
        {"DEMAND-TARGET-FORM",读取字段询问先天类型(当前,1005)},
        {"DEMAND-TARGET-RANGE",读取字段询问先天类型(当前,1008)},
        {"DEMAND-TARGET-COMPARISON",读取字段询问先天类型(当前,1009)},
        {"DEMAND-TARGET-DIRECTION",读取字段询问先天类型(当前,1010)},
        {"THIRD-PARTY",读取字段询问第三方概念(当前)},
        {"ADDRESSEE",读取行动请求受话者概念(当前)},
        {"SPEAKER",读取行动请求说话者概念(当前)}};
}
void 初始化普通应用语言知识(普通应用上下文& 当前, 普通应用装配结果& 结果,
    const std::string& 标准文本, bool 只读) noexcept {
    try {
        if (!当前.中文语言节点)
            throw std::logic_error("语言知识初始化缺少正式中文身份");
        const 初始化语言知识请求 请求{
            标准文本, *当前.中文语言节点, 严格UTF8字节精确规范化合同节点,
            读取字段询问名称绑定(当前)};
        auto 材料 = 只读
            ? 读取最小语言知识(请求, 当前.世界事实.语言服务, 当前.世界事实.词服务,
                当前.世界事实.存在概念服务, 当前.世界事实.名称关系服务)
            : 初始化最小语言知识(请求, 当前.世界事实.语言服务, 当前.世界事实.词服务,
                当前.世界事实.存在概念服务, 当前.世界事实.名称关系服务);
        当前.语言知识 = std::move(材料);
        结果.语言知识 = 当前.语言知识;
        if (当前.语言知识->工程控制 != 初始化语言知识工程控制::已完成
            || 当前.语言知识->状态 != 初始化语言知识状态::已完成) {
            const auto& 未完成 = *当前.语言知识;
            std::ostringstream 原因;
            原因 << "工程控制=" << static_cast<int>(未完成.工程控制)
                << "，阶段=" << static_cast<int>(未完成.阶段);
            if (未完成.状态) 原因 << "，状态=" << static_cast<int>(*未完成.状态);
            if (未完成.到限额度) 原因 << "，到限额度=" << static_cast<int>(*未完成.到限额度);
            if (未完成.当前项) 原因 << "，项=" << *未完成.当前项;
            原因 << "，原因=" << 未完成.诊断说明;
            const auto 说明 = 原因.str();
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用语言知识",
                L"固定语言知识材料未闭合；已发生步骤保留。", 说明.c_str());
        }
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用语言知识",
            L"知识解码、发布、完整读取或结果保存发生工程异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用语言知识",
            L"语言知识初始化发生未知工程异常。");
    }
}

void 初始化普通应用指代种子(普通应用上下文& 当前, 普通应用装配结果& 结果,
    const std::string& 标准文本, bool 只读) noexcept {
    try {
        if (!当前.中文语言节点)
            throw std::logic_error("指代种子初始化缺少正式中文身份");
        const 初始化指代种子请求 请求{
            标准文本, *当前.中文语言节点, 严格UTF8字节精确规范化合同节点};
        auto 材料 = 只读
            ? 读取初始化指代概念名称(请求, 当前.世界事实.语言服务,
                当前.世界事实.词服务, 当前.世界事实.存在概念服务,
                当前.世界事实.名称关系服务)
            : 初始化指代概念名称(请求, 当前.世界事实.语言服务,
                当前.世界事实.词服务, 当前.世界事实.存在概念服务,
                当前.世界事实.名称关系服务);
        当前.指代种子 = std::move(材料);
        结果.指代种子 = 当前.指代种子;
        const auto& m = *当前.指代种子;
        if (m.工程异常) std::rethrow_exception(m.工程异常);
        const bool 六项完成 = std::all_of(m.项组.begin(), m.项组.end(),
            [](const 初始化指代种子项& 项) {
                return 项.逐项核验完成 && 项.全部复核完成
                    && 项.定义读回 && 项.定义读回->信息
                    && 项.词条读回 && 项.词条读回->信息
                    && 项.关系读回 && 项.关系读回->信息
                    && 项.概念名称读回 && 项.词条候选读回;
            });
        if (m.工程控制 != 初始化指代种子工程控制::已完成
            || m.状态 != 初始化指代种子状态::已完成
            || m.阶段 != 初始化指代种子阶段::完成 || m.当前项
            || !m.来源读回 || !m.来源读回->信息 || !六项完成) {
            std::ostringstream 原因;
            原因 << "工程控制=" << static_cast<int>(m.工程控制)
                << "，阶段=" << static_cast<int>(m.阶段);
            if (m.状态) 原因 << "，状态=" << static_cast<int>(*m.状态);
            if (m.解释结果) {
                原因 << "，解释工程控制=" << static_cast<int>(m.解释结果->工程控制);
                if (m.解释结果->解释状态)
                    原因 << "，解释状态=" << static_cast<int>(*m.解释结果->解释状态);
                if (m.解释结果->诊断)
                    原因 << "，解释诊断=" << static_cast<int>(m.解释结果->诊断->原因);
            }
            if (m.语言读回) 原因 << "，语言读取=" << static_cast<int>(m.语言读回->状态);
            if (m.定义结构处理) 原因 << "，定义结构=" << static_cast<int>(*m.定义结构处理);
            if (m.名称结构处理) 原因 << "，名称结构=" << static_cast<int>(*m.名称结构处理);
            if (m.来源发布) 原因 << "，来源发布=" << static_cast<int>(m.来源发布->状态);
            if (m.来源读回) 原因 << "，来源读取=" << static_cast<int>(m.来源读回->状态);
            if (m.当前项 && *m.当前项 < m.项组.size()) {
                const auto& 项 = m.项组[*m.当前项];
                原因 << "，项=" << *m.当前项;
                if (项.结构前定义查询) 原因 << "，定义前查=" << static_cast<int>(项.结构前定义查询->状态);
                if (项.定义查询) 原因 << "，定义查询=" << static_cast<int>(项.定义查询->状态);
                if (项.定义读回) 原因 << "，定义读回=" << static_cast<int>(项.定义读回->状态);
                if (项.词条查询) 原因 << "，词条查询=" << static_cast<int>(项.词条查询->状态);
                if (项.词条读回) 原因 << "，词条读回=" << static_cast<int>(项.词条读回->状态);
                if (项.定义写入状态) 原因 << "，定义=" << static_cast<int>(*项.定义写入状态);
                if (项.词条写入状态) 原因 << "，词条=" << static_cast<int>(*项.词条写入状态);
                if (项.名称写入状态) 原因 << "，名称=" << static_cast<int>(*项.名称写入状态);
                if (项.名称发布) 原因 << "，名称发布阶段=" << static_cast<int>(项.名称发布->阶段);
                if (项.关系读回) 原因 << "，名称读取=" << static_cast<int>(项.关系读回->状态);
                if (项.概念名称读回) 原因 << "，概念名称=" << static_cast<int>(项.概念名称读回->状态);
                if (项.词条候选读回) 原因 << "，词条候选=" << static_cast<int>(项.词条候选读回->状态);
            }
            原因 << "，原因=" << m.诊断说明;
            const auto 说明 = 原因.str();
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用指代种子",
                L"固定指代材料未闭合；已发生步骤保留。", 说明.c_str());
        }
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用指代种子",
            L"指代解释、发布、读回或结果保存发生工程异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用指代种子",
            L"指代初始化发生未知工程异常。");
    }
}

std::optional<稳定编码> 读取连接具名概念(
    普通应用上下文& 当前, std::string_view 名称, bool 存在概念) {
    if (!当前.中文语言节点) return std::nullopt;
    const auto 词 = 当前.世界事实.词服务.建立或取得词条({
        *当前.中文语言节点, 严格UTF8字节精确规范化合同节点,
        std::string(名称), std::nullopt});
    if (!词.词条节点) return std::nullopt;
    const auto 候选 = 当前.世界事实.名称关系服务.查询词条候选概念(*词.词条节点);
    if (候选.状态 != 概念名称关系查询状态::已找到
        && 候选.状态 != 概念名称关系查询状态::未找到) return std::nullopt;
    std::optional<稳定编码> 唯一;
    for (const auto 关系节点 : 候选.名称关系节点组) {
        const auto 关系 = 当前.世界事实.名称关系服务.获取名称关系(关系节点);
        if (关系.状态 != 概念名称关系查询状态::已找到 || !关系.信息)
            return std::nullopt;
        const auto n = 关系.信息->概念节点;
        if (存在概念 ? !当前.世界事实.存在概念服务.获取存在概念(n)
            : !当前.世界事实.特征概念服务.获取特征概念(n)) return std::nullopt;
        if (唯一 && *唯一 != n) return std::nullopt;
        唯一 = n;
    }
    return 唯一;
}

struct 存在根名称定位 final {
    稳定编码 词条节点;
    std::optional<稳定编码> 概念节点;
};

存在根名称定位 读取存在根名称候选(普通应用上下文& 当前) {
    if(!当前.中文语言节点)throw std::logic_error("存在根命名缺少正式中文身份");
    const auto 词=当前.世界事实.词服务.建立或取得词条({
        *当前.中文语言节点,严格UTF8字节精确规范化合同节点,"存在",std::nullopt});
    if(!词.词条节点)throw std::runtime_error("存在根词条无法完整建立或读取");
    const auto 候选=当前.世界事实.名称关系服务.查询词条候选概念(*词.词条节点);
    if(候选.状态==概念名称关系查询状态::未找到) {
        if(!候选.名称关系节点组.empty())throw std::logic_error("存在根名称未找到但返回非空关系");
        return {*词.词条节点,{}};
    }
    if(候选.状态!=概念名称关系查询状态::已找到||候选.名称关系节点组.empty())
        throw std::runtime_error("存在根名称候选读取未完成，禁止作为未找到重新创建");
    std::optional<稳定编码> 唯一;
    for(const auto n:候选.名称关系节点组) {
        const auto r=当前.世界事实.名称关系服务.获取名称关系(n);
        if(r.状态!=概念名称关系查询状态::已找到||!r.信息||r.信息->词条节点!=*词.词条节点
            ||!当前.世界事实.存在概念服务.获取存在概念(r.信息->概念节点))
            throw std::logic_error("存在根名称关系不是完整普通存在概念候选");
        if(唯一&&*唯一!=r.信息->概念节点)
            throw std::logic_error("存在根名称对应多个不同概念，保留材料并停止初始化");
        唯一=r.信息->概念节点;
    }
    return {*词.词条节点,唯一};
}

稳定编码 初始化存在语义根(普通应用上下文& 当前) try {
    if(!当前.人类存在概念节点)throw std::logic_error("存在语义根初始化缺少人类概念");
    const auto 候选=读取存在根名称候选(当前);
    const auto 建立=当前.世界事实.存在概念服务.建立或取得存在根概念(
        {候选.概念节点,{*当前.人类存在概念节点}});
    if(!建立.概念节点||(建立.状态!=存在概念操作状态::已建立
        &&建立.状态!=存在概念操作状态::已复用&&建立.状态!=存在概念操作状态::已更新))
        throw std::logic_error("存在语义根或人类分类未完整发布，保留既有事实");
    const auto 名=当前.世界事实.名称关系服务.建立或取得名称关系(*建立.概念节点,候选.词条节点);
    if(!名.名称关系节点)throw std::runtime_error("存在语义根已发布但名称关联未完成，禁止另建根");
    const auto 名读回=读取存在根名称候选(当前);
    const auto 根=当前.世界事实.存在概念服务.读取存在根概念保护();
    if(名读回.概念节点!=建立.概念节点||根.状态!=正式概念读取状态::已读取
        ||!根.信息||根.信息->节点!=*建立.概念节点||根.完整保护组.empty())
        throw std::logic_error("存在语义根命名与owner登记读回不一致");
    return *建立.概念节点;
} catch(const std::exception& 异常) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"初始化存在语义根",
        L"根定位、登记或命名未完整完成；保留已发布事实，不另建替代根。",异常.what());
} catch(...) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"初始化存在语义根",
        L"根初始化发生未知工程故障；保留已发布事实。");
}

稳定编码 初始化特征类别根(普通应用上下文& 当前) try {
    auto& owner=当前.世界事实.特征概念服务;
    const auto 建立=owner.建立或取得特征类别根概念();
    if(!建立.概念节点||(建立.状态!=特征概念规则状态::已完成
        &&建立.状态!=特征概念规则状态::无变化)||!当前.中文语言节点)
        throw std::logic_error("正式特征类别根未完整建立或复用");
    const auto 词=当前.世界事实.词服务.建立或取得词条({
        *当前.中文语言节点,严格UTF8字节精确规范化合同节点,"特征",std::nullopt});
    if(!词.词条节点)throw std::runtime_error("特征类别根命名词条未就绪");
    const auto 名=当前.世界事实.名称关系服务.建立或取得名称关系(*建立.概念节点,*词.词条节点);
    if(!名.名称关系节点)throw std::runtime_error("特征类别根已发布但名称关联未完成");
    const auto 名读回=当前.世界事实.名称关系服务.获取名称关系(*名.名称关系节点);
    const auto 根=owner.读取特征类别根概念保护();
    const auto 身份=owner.读取正式概念身份保护(*建立.概念节点);
    if(名读回.状态!=概念名称关系查询状态::已找到||!名读回.信息
        ||名读回.信息->概念节点!=*建立.概念节点||名读回.信息->词条节点!=*词.词条节点
        ||根.状态!=正式概念读取状态::已读取||!根.信息||根.信息->节点!=*建立.概念节点
        ||根.完整保护组.empty()||身份.状态!=正式概念读取状态::已读取||!身份.身份
        ||身份.身份->概念节点!=*建立.概念节点||身份.身份->完整保护组.empty()
        ||std::count(根.信息->名称关系.begin(),根.信息->名称关系.end(),*名.名称关系节点)!=1
        ||owner.是特征概念节点(*建立.概念节点))
        throw std::logic_error("特征类别根正式身份或名称未一致读回，不能充作值域FT");
    // 分类列表仅在此阶段按需读取，不进入每次参数类别身份解码。
    const auto 分类=owner.读取特征分类关系保护(*建立.概念节点);
    const auto 先天=owner.获取先天特征概念();
    if(分类.状态!=特征概念规则状态::已完成||!分类.信息||!分类.信息->是类别根
        ||分类.信息->概念节点!=*建立.概念节点||!分类.信息->直接上位概念组.empty()
        ||分类.完整保护组.empty()||!先天
        ||std::count(分类.信息->直接下位概念组.begin(),分类.信息->直接下位概念组.end(),先天->需求)!=1)
        throw std::logic_error("特征类别根与先天需求特征类型的分类未完整读回");
    const auto 子=owner.读取特征分类关系保护(先天->需求);
    if(子.状态!=特征概念规则状态::已完成||!子.信息||子.信息->是类别根
        ||子.信息->直接上位概念组!=std::vector<稳定编码>{*建立.概念节点}||子.完整保护组.empty())
        throw std::logic_error("先天特征类型未投影到同一正式特征类别根");
    return *建立.概念节点;
} catch(const std::exception& 异常) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"初始化特征类别根",
        L"特征类别根、命名或分类未完整完成；保留已发布事实。",异常.what());
} catch(...) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"初始化特征类别根",
        L"特征类别根初始化发生未知工程故障。");
}

void 核验存在分类根(普通应用上下文& 当前,稳定编码 R,const 先天信息概念组& 三类) try {
    auto& owner=当前.世界事实.存在概念服务;
    const auto root=owner.读取存在分类关系保护(R);
    if(root.状态!=正式结构操作状态::已读取||!root.信息
        ||root.信息->形状!=存在分类概念形状::普通抽象||!root.信息->直接上位概念组.empty()
        ||root.完整保护组.empty()||!当前.人类存在概念节点)
        throw std::logic_error("正式存在根分类未完整读回");
    const std::array 必需下位{*当前.人类存在概念节点,三类.需求,三类.任务,三类.方法};
    for(const auto child:必需下位)
        if(std::count(root.信息->直接下位概念组.begin(),root.信息->直接下位概念组.end(),child)!=1)
            throw std::logic_error("正式存在根缺少唯一的人类或需求任务方法直接下位");
    for(const auto child:{三类.需求,三类.任务,三类.方法}) {
        const auto c=owner.读取存在分类关系保护(child);
        if(c.状态!=正式结构操作状态::已读取||!c.信息
            ||c.信息->形状!=存在分类概念形状::信息结构
            ||c.信息->直接上位概念组!=std::vector<稳定编码>{R}||c.完整保护组.empty())
            throw std::logic_error("需求任务方法未直接且唯一归属同一存在语义根");
    }
} catch(const std::exception& 异常) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"核验存在分类根",
        L"存在根与人类、需求、任务、方法分类未完整读回；保留已有信息。",异常.what());
} catch(...) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"核验存在分类根",
        L"共同分类读取发生未知工程故障；保留已有信息。");
}

std::optional<稳定编码> 确保连接通用存在概念(普通应用上下文& 当前) try {
    const auto 根=当前.世界事实.存在概念服务.读取存在根概念保护();
    if(根.状态!=正式概念读取状态::已读取||!根.信息||根.完整保护组.empty())
        throw std::logic_error("连接参数初始化缺少已登记存在语义根");
    const auto 名=读取存在根名称候选(当前);
    if(名.概念节点!=std::optional{根.信息->节点})
        throw std::logic_error("连接通用存在名称与正式根身份不一致");
    return 根.信息->节点;
} catch(const std::exception& 异常) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复用连接通用存在概念",
        L"连接参数引用的正式存在根未完整读取；不改写已固定规格。",异常.what());
} catch(...) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复用连接通用存在概念",
        L"正式存在根复用发生未知工程故障。");
}

std::optional<稳定编码> 确保单引用交互者特征(
    普通应用上下文& 当前, 稳定编码 自我, 稳定编码 通用存在概念) {
    auto& 服务 = 当前.世界事实;
    服务.特征服务.装配存在成员身份读取接口(服务.存在服务);
    特征概念定义 定义;
    定义.材料类型 = 特征概念材料物理类型::存在节点引用;
    定义.值域 = 特征概念存在引用值域{通用存在概念};
    定义.比较规则 = 特征概念比较规则::存在身份精确;
    定义.聚合规则 = 特征概念聚合规则::不自动聚合;
    auto 类型 = 读取连接具名概念(当前, "交互者", false);
    if (!类型) {
        const auto 新类型 = 服务.特征概念服务.建立或取得特征概念(定义, 服务.特征值服务);
        if (!有效(新类型) || !当前.中文语言节点) return std::nullopt;
        const auto 词 = 服务.词服务.建立或取得词条({*当前.中文语言节点,
            严格UTF8字节精确规范化合同节点, "交互者", std::nullopt});
        if (!词.词条节点) return std::nullopt;
        const auto 名 = 服务.名称关系服务.建立或取得名称关系(新类型, *词.词条节点);
        if (!名.名称关系节点) return std::nullopt;
        类型 = 新类型;
    }
    const auto 概念 = 服务.特征概念服务.获取特征概念(*类型);
    if (!概念 || 概念->材料类型 != 定义.材料类型 || 概念->值域 != 定义.值域
        || 概念->比较规则 != 定义.比较规则 || 概念->聚合规则 != 定义.聚合规则
        || 概念->单位 || 服务.特征概念服务.比较单特征概念(*类型, *类型).特征类型根 != 类型)
        return std::nullopt;
    const auto E = 服务.存在服务.获取存在(自我);
    const auto S = 服务.场景服务.查询所属场景(自我);
    if (!E || !S) return std::nullopt;
    std::optional<稳定编码> 已有;
    for (const auto f : E->特征节点组) {
        const auto F = 服务.特征服务.获取特征(f);
        const auto 根 = F ? 服务.特征概念服务.比较单特征概念(
            F->特征概念节点, F->特征概念节点).特征类型根 : std::nullopt;
        if (!根) return std::nullopt;
        if (*根 != *类型) continue;
        if (已有) return std::nullopt;
        已有 = f;
    }
    if (!已有) {
        const auto 添加 = 服务.存在服务.添加无当前值特征(自我, *类型);
        if (添加.状态 != 新存在操作状态::已完成 || !添加.特征节点) return std::nullopt;
        已有 = 添加.特征节点;
    }
    const auto F = 服务.特征服务.获取特征(*已有);
    if (!F || F->上级特征节点) return std::nullopt;
    // 尚未连接时保持无当前值；旧集合和匿名网页登录信息不能成为初值。
    return 类型;
}

void 命名并读回先天信息概念(普通应用上下文& 当前, const 先天信息概念组& 组) {
    if (!当前.中文语言节点)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念命名", L"正式中文身份未就绪。");
    const auto 命名 = [&](稳定编码 节点, std::string_view 名, bool 材料FT) {
        const auto 词 = 当前.世界事实.词服务.建立或取得词条({*当前.中文语言节点,
            严格UTF8字节精确规范化合同节点, std::string(名), std::nullopt});
        if (!词.词条节点)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念命名", L"词条建立失败，已发表概念保留。");
        const auto 名称 = 当前.世界事实.名称关系服务.建立或取得名称关系(节点, *词.词条节点);
        if (!名称.名称关系节点)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念命名", L"名称关联建立失败，已发表概念保留。");
        std::vector<稳定编码> 关联;
        if (材料FT) {
            const auto 读回 = 当前.世界事实.特征概念服务.读取特征概念定义保护(节点);
            if (读回.前提已变化 || 读回.状态 != 基础结构操作状态::已完成 || !读回.信息)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念命名", L"FT定义或保护未完整读回。");
            关联 = 读回.信息->定义.名称关系;
        } else {
            const auto 读回 = 当前.世界事实.存在概念服务.读取正式结构概念保护(节点);
            if (读回.状态 != 正式结构操作状态::已读取 || !读回.信息 || 读回.完整保护组.empty())
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念命名", L"结构定义及保护未完整读回。");
            关联 = 读回.信息->名称关系;
        }
        const auto 名称读回 = 当前.世界事实.名称关系服务.获取名称关系(*名称.名称关系节点);
        if (std::count(关联.begin(), 关联.end(), *名称.名称关系节点) != 1
            || 名称读回.状态 != 概念名称关系查询状态::已找到 || !名称读回.信息
            || 名称读回.信息->概念节点 != 节点 || 名称读回.信息->词条节点 != *词.词条节点)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念命名", L"名称及正式反向关联不一致。");
    };
    命名(组.需求, "需求", false); 命名(组.任务, "任务", false); 命名(组.方法, "方法", false);
    const std::pair<std::uint32_t, std::string_view> 定义名称[] = {
        {201,"自我需求"},{202,"自我执行域需求"},{203,"自我执行需求"},{204,"值目标需求"},
        {205,"方向目标需求"},{206,"普通自我执行需求"},{207,"根需求"},{208,"被认知需求"},
        {209,"完整被认知需求"},{231,"自我引用内容值需求"},{232,"自我引用内容单项值需求"},
        {211,"已采用方法任务"},{212,"有执行组织任务"},{213,"已开始执行任务"},{214,"已评估任务"},
        {215,"已结算任务"},{221,"本能方法"},{222,"组合方法"},{301,"任务虚拟存在"},{302,"方法虚拟存在"},
        {303,"方法头"},{304,"参数规格"},{305,"条件"},{306,"子条件"},{307,"结果"},
        {308,"正常条件结果知识"},{309,"异常条件结果知识"},{401,"合法存在"},{402,"合法FT根"},
        {403,"合法特征概念"},{404,"合法比较关系"},{405,"正式信息身份"},{406,"正式概念身份"},
        {407,"全I64"},{408,"非负I64"},{409,"万分整域"},{410,"序号域"},{411,"执行域枚举"},
        {412,"目标形式枚举"},{413,"活动枚举"},{414,"方向枚举"},{415,"根角色枚举"},{416,"任务状态域"},
        {417,"方法作用域域"},{418,"实现方式域"},{419,"行为域"},{420,"启用域"},
        {501,"需求来源"},{502,"理解来源"},{503,"内容确认来源"},{504,"执行组织"},{505,"执行实例X"},
        {506,"评估依据"},{507,"结算依据"},{508,"动作定义"},{509,"实现绑定"},{510,"本能登记"},
        {511,"共同约束项"},{512,"条件项"},{513,"结果项"},{514,"已确认消息连接"},
        {515,"需求身份"},{601,"唯一自我存在"},
        {701,"共同约束组"},{702,"条件知识组"},{703,"作用存在概念组"},{704,"涉及FT组"},
        {705,"可能结果组"},{706,"规格有序组"},{707,"条件项组"},{708,"配对结果组"},{709,"非空结果项组"},
        {801,"目标域属于FT"},{802,"比较适用"},{803,"根禁普通来源"},{804,"成功不大于运行"},
        {805,"规格连续"},{806,"条件同方法父链"},{807,"正常配对"},{808,"异常配对"},
        {809,"可靠度允许未知"},{810,"已结算状态域"},{811,"需求父链合法"},
        {812,"同一对象恰一结构分支"},{821,"单点一"},{822,"单点二"}};
    for (const auto& [槽, 节点] : 组.子类与支持定义) {
        const auto 找到 = std::find_if(std::begin(定义名称), std::end(定义名称),
            [&](const auto& 项) { return 项.first == 槽; });
        if (找到 == std::end(定义名称))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念命名", L"读回包含未声明的定义槽。");
        命名(节点, 找到->second, false);
    }
    const std::pair<std::uint32_t, std::string_view> 字段名称[] = {
        {1001,"需求主体"},{1002,"需求执行域"},{1003,"需求目标存在"},{1004,"需求目标FT"},
        {1005,"需求目标形式"},{1006,"需求权重"},{1007,"需求活动"},{1008,"目标域"},{1009,"目标比较"},
        {1010,"目标方向"},{1011,"父需求"},{1012,"需求来源"},{1013,"需求根角色"},{1014,"理解来源"},
        {1015,"内容确认来源"},{1016,"引用内容需求"},{1017,"引用内容项"},
        {1101,"对应需求"},{1102,"任务虚拟存在"},{1103,"采用方法"},
        {1104,"执行组织"},{1105,"执行实例"},{1106,"评估依据"},{1107,"结算依据"},{1201,"方法头"},
        {1202,"方法作用域"},{1203,"方法实现方式"},{1204,"方法行为"},{1205,"动作定义"},{1206,"实现绑定"},
        {1207,"方法虚拟存在"},{1208,"共同约束"},{1209,"条件结果知识"},{1210,"本能登记"},
        {1211,"作用存在概念"},{1212,"涉及FT概念"},{1213,"可能结果概念"},{1301,"有序参数规格"},
        {1302,"方法运行次数"},{1303,"方法成功次数"},{1304,"方法可靠度"},{1311,"参数序号"},
        {1312,"参数信息类型"},{1313,"参数范围"},{1401,"所属方法"},{1402,"条件项"},
        {1403,"父条件"},{1404,"配对结果"},{1405,"结果项"}};
    const bool 引用内容结构已建立 = std::any_of(组.子类与支持定义.begin(), 组.子类与支持定义.end(),
        [](const auto& 项) { return 项.first == 231 || 项.first == 232; });
    for (const auto& [槽, 名称] : 字段名称) {
        // 首次命名早于引用内容定义批；仅在其结构已发表后要求配套FT。
        if (!引用内容结构已建立 && (槽 == 1016 || 槽 == 1017)) continue;
        const auto 类型 = 当前.世界事实.特征概念服务.读取先天特征类型槽(槽);
        if ((类型.状态 != 特征概念规则状态::已完成 && 类型.状态 != 特征概念规则状态::无变化)
            || !类型.类型根 || 类型.完整保护组.empty())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息字段命名", L"固定FT槽未完整读回。");
        命名(*类型.类型根, 名称, true);
    }
}

普通应用装配结果 完成装配(普通应用上下文& 当前,
    const std::string& 标准文本, const std::string& 子概念文本,
    const std::string& 语言知识文本, const std::string& 指代文本) noexcept {
#if HY_EGO_STARTUP_TIMING
    const auto 观察起点 = std::chrono::steady_clock::now();
    auto 上一步观察 = 观察起点;
    const auto 观察阶段 = [&](const wchar_t* 步骤, std::int64_t 返回值) noexcept {
        const auto now = std::chrono::steady_clock::now();
        const auto elapsed = std::chrono::duration<double, std::milli>(now - 上一步观察).count();
        const auto total = std::chrono::duration<double, std::milli>(now - 观察起点).count();
        上一步观察 = now;
        try {
            std::wostringstream text;
            text << L"阶段=结束；耗时毫秒=" << elapsed << L"；装配累计毫秒=" << total << L"；返回值=" << 返回值;
            (void)记录调试日志(L"STARTUP_TIMING", L"普通应用装配", 步骤, text.str());
        } catch (...) {}
    };
#endif
    普通应用装配结果 结果;
    try {
        结果.世界树 = 当前.世界树.初始化();
        if (!世界初始化成功(结果.世界树->状态)
            || !结果.世界树->世界树根场景) {
            结果.状态 = 普通应用装配状态::世界树初始化失败;
            return 结果;
        }

        结果.自然语言世界树 = 当前.自然语言世界树.初始化();
        if (!结果.自然语言世界树->成功()) {
            结果.状态 = 普通应用装配状态::自然语言世界初始化失败;
            return 结果;
        }
        结果.自然语言世界树根节点 = 结果.自然语言世界树->根节点;
        if (!当前.内部治理世界.初始化())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化治理世界", L"治理世界未装配任务owner。");
        结果.内部治理世界树根节点 = 当前.内部治理世界.获取根场景();

        if (!建立第一权限人(
            当前, *结果.世界树->世界树根场景)) {
            结果.状态 = 普通应用装配状态::第一权限人初始化失败;
            return 结果;
        }
        结果.第一权限人存在节点 = 当前.第一权限人存在节点;
        结果.第一权限人专属存在概念节点 =
            当前.第一权限人专属存在概念节点;
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"世界与第一权限人", 1);
#endif
        初始化普通应用概念名称(当前, 结果, 标准文本, false);
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"基础概念名称", 1);
#endif
        初始化普通应用结构化子概念(当前, 结果, 子概念文本, false);
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"结构化子概念", 1);
#endif
        const auto 语言结构 = 当前.世界事实.语言服务.初始化语言知识结构();
        if (语言结构 != 新语言知识状态::已完成)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化普通应用语言知识结构",
                L"指代种子前语言知识结构未就绪；保留已发生结构身份。");
        初始化普通应用指代种子(当前, 结果, 指代文本, false);
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"语言结构与指代种子", 1);
#endif
        if (!建立人类描述特征(当前))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"人类描述初始化", L"名字或性别描述结构无法完整建立。");
        if (!建立人类存在概念(当前)) {
            结果.状态 = 普通应用装配状态::人类概念初始化失败;
            return 结果;
        }
        结果.人类存在概念节点 = 当前.人类存在概念节点;
        const auto 存在语义根=初始化存在语义根(当前);
        (void)初始化特征类别根(当前);
        结果.登录名特征概念节点 = 当前.登录名特征概念节点;
        结果.姓名特征概念节点 = 当前.姓名特征概念节点;
        结果.权限角色特征概念节点 = 当前.权限角色特征概念节点;

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"人类描述及分类根", 1);
#endif
        const auto 服务要求初始化 = 当前.服务要求.初始化(*当前.人类存在概念节点);
        if (服务要求初始化 != 结构化服务要求状态::已建立
            && 服务要求初始化 != 结构化服务要求状态::已复用) {
            结果.状态 = 普通应用装配状态::服务要求初始化失败;
            return 结果;
        }
        if (!当前.配置.本次网页会话服务对象登录名.empty()) {
            const auto 绑定 = 可信服务对象可绑定(当前);
            if (绑定 != 网页服务输入状态::已完成) {
                结果.状态 = 绑定 == 网页服务输入状态::资源失败 ? 普通应用装配状态::资源失败
                    : 绑定 == 网页服务输入状态::对象不匹配 ? 普通应用装配状态::服务对象绑定失败
                    : 普通应用装配状态::内部不一致;
                return 结果;
            }
        }

        if (!建立确认二次特征概念(当前)) {
            结果.状态 = 普通应用装配状态::确认概念初始化失败;
            return 结果;
        }
        结果.语言语义差异二次特征类型根 =
            当前.语言语义差异二次特征类型根;
        结果.无法确认结果概念 = 当前.无法确认结果概念;
        结果.无法确认词条 = 当前.无法确认词条;
        结果.无法确认名称关系 = 当前.无法确认名称关系;
        结果.确认结果概念 = 当前.确认结果概念;
        结果.确认词条 = 当前.确认词条;
        结果.确认名称关系 = 当前.确认名称关系;

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"服务要求与确认概念", 1);
#endif
        const auto 治理概念 = 当前.治理二次特征.初始化(*当前.中文语言节点);
        if (治理概念.状态 != 治理二次特征状态::已完成 || !治理概念.概念) {
            结果.状态 = 普通应用装配状态::治理二次特征初始化失败;
            return 结果;
        }
        结果.治理时间概念 = 治理概念.概念;
        结果.自我治理基础 = 当前.自我线程.建立或取得治理基础();
        if (!结果.自我治理基础->成功()) {
            结果.状态 = 普通应用装配状态::自我治理基础初始化失败;
            return 结果;
        }
        if (!当前.自我初始化请求) {
            当前.自我初始化请求 = 形成自我初始化请求(
                当前.配置, *结果.世界树, *结果.自我治理基础,
                *当前.人类存在概念节点, *当前.第一权限人存在节点);
        }
        if (!当前.自我初始化请求) {
            结果.状态 = 普通应用装配状态::内部不一致;
            return 结果;
        }

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"治理基础", 1);
#endif
        const auto 本能登记 = 新_本能函数管理类::初始化();
        if (本能登记 != 新本能函数登记状态::已登记
            && 本能登记 != 新本能函数登记状态::精确重复) {
            结果.状态 = 普通应用装配状态::本能函数登记失败;
            return 结果;
        }

        const auto 端口步骤已完成 = [](bool 已完成, const wchar_t* 步骤) {
            if (!已完成)
                (void)记录运行日志(L"普通应用", L"端口装配停止步骤", 步骤);
            return 已完成;
        };
        if (!端口步骤已完成(当前.需求服务.初始化(), L"需求服务初始化")
            || !端口步骤已完成(当前.方法服务.初始化(), L"方法服务初始化")
            || !端口步骤已完成(当前.任务服务.初始化治理(), L"任务治理初始化")
            || !端口步骤已完成(当前.任务服务.装配本能动作读取(当前.世界事实.动态服务), L"本能动作读取装配")
            || !端口步骤已完成(当前.任务服务.装配方法问题反馈(当前.执行场景采集, 当前.问题模拟世界), L"方法问题反馈装配")) {
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
        当前.方法服务.初始化治理场景组织();
        // 只注入既有task owner的身份读取能力；不建任务、不改变集合。
        // 清理器在self/manager及其worker全部join后、task owner销毁前解绑。
        当前.世界事实.特征概念服务.装配任务成员读取接口(&当前.任务服务);
        当前.世界事实.特征概念服务.装配需求成员读取接口(&当前.需求服务);
        当前.世界事实.特征服务.装配需求引用读取接口(&当前.需求服务);
        当前.世界事实.特征概念服务.装配存在成员读取接口(&当前.世界事实.存在服务);
        if (!端口步骤已完成(当前.世界事实.存在服务.装配存在集合成员保护字段(
            当前.世界事实.特征概念服务.存在集合成员保护字段()), L"存在集合成员保护字段装配")) {
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
        当前.需求服务.装配服务要求确认读取接口(&当前.服务要求);
        当前.需求服务.装配部分服务要求读取接口(&当前.服务要求);
        当前.需求服务.装配接收认知语言读取接口(&当前.世界事实.语言服务);
        const std::array<const 正式概念身份读取接口*, 3> 概念提供者{
            &当前.世界事实.特征概念服务, &当前.世界事实.二次特征概念服务, &当前.世界事实.存在概念服务};
        if (!端口步骤已完成(当前.世界事实.特征服务.装配正式概念引用读取接口(
            &当前.世界事实.存在概念服务, 概念提供者), L"正式概念引用特征读取装配")) {
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
        const auto 状态FT = 当前.任务服务.获取任务状态特征类型();
        const auto 贡献FT = 当前.任务服务.获取任务服务值特征类型();
        const auto 启用FT = 当前.方法服务.获取方法启用特征类型();
        const auto 统计FT = 当前.方法服务.初始化并读取方法统计特征类型();
        if (!状态FT || !贡献FT || !启用FT || !统计FT)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念装配", L"正式任务/方法字段类型未就绪。");
        const auto 先天定义 = 当前.世界事实.存在概念服务.建立或取得先天需求任务方法概念(
            {*状态FT, *贡献FT, *启用FT, 统计FT->运行次数, 统计FT->成功次数, 统计FT->可靠度},
            当前.世界事实.特征值服务, 概念提供者);
        const auto 先天读回 = 当前.世界事实.存在概念服务.读取先天需求任务方法概念();
        if ((先天定义.状态 != 正式结构操作状态::已建立 && 先天定义.状态 != 正式结构操作状态::已复用)
            || !先天定义.信息 || 先天读回.状态 != 正式结构操作状态::已读取
            || !先天读回.信息 || *先天读回.信息 != *先天定义.信息)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天信息概念装配", L"三根及支持定义未完整读回。");
        命名并读回先天信息概念(当前, *先天读回.信息);
        核验存在分类根(当前,存在语义根,*先天读回.信息);
        const auto 需求集合先天 = 当前.世界树.获取先天特征概念();
        if (!需求集合先天 || !当前.服务对象需求发布.初始化(需求集合先天->需求, 需求集合先天->服务对象需求集合)) {
            (void)端口步骤已完成(false, L"服务对象需求集合发布初始化");
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"端口及先天概念", 1);
#endif
        const auto 内容概念 = 当前.世界事实.存在概念服务.建立或取得引用内容筹办概念(
            当前.世界事实.特征值服务, 概念提供者);
        const auto 内容概念读回 = 当前.世界事实.存在概念服务.读取引用内容筹办概念();
        if ((内容概念.状态 != 正式结构操作状态::已建立 && 内容概念.状态 != 正式结构操作状态::已复用)
            || !内容概念.信息 || 内容概念读回.状态 != 正式结构操作状态::已读取
            || !内容概念读回.信息
            || 内容概念读回.信息->完整被认知需求 != 内容概念.信息->完整被认知需求
            || 内容概念读回.信息->自我引用内容值需求 != 内容概念.信息->自我引用内容值需求
            || 内容概念读回.信息->自我引用内容单项值需求 != 内容概念.信息->自我引用内容单项值需求
            || 内容概念读回.信息->恰一分支约束 != 内容概念.信息->恰一分支约束)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"引用内容筹办装配", L"内容筹办概念未完整读回");
        const auto 内容端口 = 当前.需求服务.装配引用内容目标读取接口(&当前.服务对象需求发布);
        if(!当前.需求服务.装配新增成员读取接口(&当前.任务服务))
            throw std::logic_error("新增成员目标读取端口装配失败");
        if (!当前.方法服务.装配正式信息参数读取接口(&当前.需求服务))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"正式信息参数装配", L"需求参数读取端口绑定失败");
        if (内容端口 != 新需求操作状态::已修改 && 内容端口 != 新需求操作状态::无变化)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"引用内容筹办装配", L"内容读取端口绑定失败");
        // 本批一次完整发布需要正式D及槽FT；仍早于自我初始化和工作线程启动。
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"引用内容筹办概念", 1);
#endif
        初始化普通应用语言知识(当前, 结果, 语言知识文本, false);
        // 名称仅关联既有正式FT，语言不产生另一份需求类型或需求内容。
        if (!当前.中文语言节点)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求类型装配", L"缺少正式中文身份。");
        const auto 需求词条 = 当前.世界事实.词服务.建立或取得词条({*当前.中文语言节点,
            严格UTF8字节精确规范化合同节点, "需求", std::nullopt});
        if (!需求词条.词条节点)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求类型装配", L"需求词条建立失败。");
        const auto 需求名称 = 当前.世界事实.名称关系服务.建立或取得名称关系(
            需求集合先天->需求, *需求词条.词条节点);
        const auto 需求定义读回 = 当前.世界事实.特征概念服务.获取特征概念(需求集合先天->需求);
        if (!需求名称.名称关系节点 || !需求定义读回
            || std::count(需求定义读回->名称关系.begin(), 需求定义读回->名称关系.end(), *需求名称.名称关系节点) != 1)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求类型装配", L"需求名称关联未完成。");
        if (!当前.自我线程.绑定服务值结算确认端口(当前.服务结算运行).成功()) {
            (void)端口步骤已完成(false, L"结算端口及自我确认端口装配");
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
        if (!当前.自我线程.绑定模拟学习(当前.模拟学习)
            || !当前.自我线程.绑定方法问题学习(当前.问题学习)) {
            结果.状态=普通应用装配状态::线程绑定失败;
            return 结果;
        }
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"语言知识与需求命名", 1);
#endif
        结果.自我 = 当前.自我线程.初始化(*当前.自我初始化请求);
        if (!结果.自我->成功()) {
            结果.状态 = 普通应用装配状态::自我初始化失败;
            return 结果;
        }

        if (!结果.自我->自我存在 || !结果.自我->完成服务需求) {
            结果.状态 = 普通应用装配状态::内部不一致;
            return 结果;
        }
        if (!结果.自我->自我建立结果 || 结果.自我->自我建立结果->存在节点 != 结果.自我->自我存在)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天自我需求子类装配", L"初始化正式回执的唯一自我身份不一致。");
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"自我初始化", 1);
#endif
        const auto 自我子类 = 当前.世界事实.存在概念服务.建立或取得自我需求子概念(
            *结果.自我->自我存在, 当前.世界事实.存在服务, 概念提供者);
        const auto 自我子类读回 = 当前.世界事实.存在概念服务.读取先天需求任务方法概念();
        if ((自我子类.状态 != 正式结构操作状态::已建立 && 自我子类.状态 != 正式结构操作状态::已复用)
            || !自我子类.信息 || 自我子类读回.状态 != 正式结构操作状态::已读取
            || !自我子类读回.信息 || *自我子类读回.信息 != *自我子类.信息)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"先天自我需求子类装配", L"真实自我限定子类未完整读回。");
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"自我需求子概念", 1);
#endif
        命名并读回先天信息概念(当前, *自我子类读回.信息);
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"先天信息概念命名", 1);
#endif
        // legacy引导先建立正式双根及首子需求，全部迁移后才开放完整任务读取。
        const auto 目标承载 = 当前.需求服务.启用目标特征实例承载();
        if (目标承载 != 新需求操作状态::已修改 && 目标承载 != 新需求操作状态::无变化) {
            (void)端口步骤已完成(false, L"需求目标特征实例迁移");
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
        if (!当前.任务服务.初始化()) {
            (void)记录运行日志(L"普通应用", L"服务任务初始化诊断",
                L"任务服务初始化返回未就绪；后续集合特征建立未执行。");
            结果.状态 = 普通应用装配状态::内部不一致;
            return 结果;
        }
        if (!确保服务任务集合特征(当前, *结果.自我->自我存在)) {
            (void)记录运行日志(L"普通应用", L"服务任务集合特征诊断",
                L"自我服务任务集合特征核验或建立未完成；交互对象集合建立未执行。");
            结果.状态 = 普通应用装配状态::内部不一致;
            return 结果;
        }
        if (!确保交互对象集合特征(当前, *结果.自我->自我存在)) {
            (void)记录运行日志(L"普通应用", L"交互对象集合特征诊断",
                L"自我交互对象集合特征核验或建立未完成。");
            结果.状态 = 普通应用装配状态::内部不一致;
            return 结果;
        }
        // 全量任务对应读取依赖自我初始化建立的正式双根需求。
        // 治理场景仍须在线程启动和任务集合同步交付之前就绪。
        当前.任务服务.初始化任务治理场景组织();
        const auto 同步绑定 = 当前.任务服务.绑定服务任务集合同步(
            &当前, *结果.自我->完成服务需求,
            [&当前](const std::vector<稳定编码>& 成员) {
                return 同步服务任务集合特征(当前, 成员);
            });
        if (同步绑定 != 新任务操作状态::已修改
            && 同步绑定 != 新任务操作状态::精确重复) {
            结果.状态 = 普通应用装配状态::内部不一致;
            return 结果;
        }
        结果.服务任务特征概念节点 = 当前.服务任务特征概念节点;
        结果.服务任务特征节点 = 当前.服务任务特征节点;

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"任务及集合初始化", 1);
#endif
        // 只初始化派生定义并绑定真实集合定位，不增加需求、任务或执行事实。
        const auto 取得概念 = 当前.治理二次特征.初始化服务取得概念(
            *当前.中文语言节点, *当前.服务任务特征概念节点);
        if (取得概念.状态 != 治理二次特征状态::已完成 || !取得概念.概念) {
            结果.状态 = 普通应用装配状态::治理二次特征初始化失败;
            return 结果;
        }
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"服务取得概念", 1);
#endif
        const auto 取得配置 = 当前.任务服务.配置服务需求取得合同({
            *结果.自我->自我存在, *当前.服务任务特征节点,
            *当前.服务任务特征概念节点, 取得概念.概念->数量变化量,
            取得概念.概念->已新增});
        if (取得配置 != 新任务操作状态::已修改 && 取得配置 != 新任务操作状态::精确重复) {
            结果.状态 = 普通应用装配状态::内部不一致;
            return 结果;
        }

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"取得合同配置", 1);
#endif
        结果.外设世界树 = 当前.网页外设模块.初始化();
        if (!世界初始化成功(结果.外设世界树->状态)
            || !结果.外设世界树->世界树根场景) {
            结果.状态 = 普通应用装配状态::交互窗口初始化失败;
            return 结果;
        }
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"网页外设初始化", 1);
#endif
        const auto 自我材料 = 当前.世界事实.存在服务.获取存在(*结果.自我->自我存在);
        const auto 先天特征 = 当前.世界事实.特征概念服务.获取先天特征概念();
        const auto 通用存在 = 确保连接通用存在概念(当前);
        const auto 接收消息类型 = 读取连接具名概念(当前, "外设对话框接收消息", false);
        const auto 接收数量类型 = 读取连接具名概念(当前, "外设对话框接收数量", false);
        const auto 发送消息类型 = 读取连接具名概念(当前, "外设对话框发送消息", false);
        const auto 消息正文范围 = 读取连接具名概念(当前, "外设对话框接收内容", false);
        const auto 输出队列类型 = 当前.外设世界树.获取输出队列特征类型();
        const auto 交互者类型 = 通用存在
            ? 确保单引用交互者特征(当前, *结果.自我->自我存在, *通用存在) : std::nullopt;
        if (!自我材料 || !先天特征 || !通用存在
            || !接收消息类型 || !发送消息类型 || !交互者类型 || !消息正文范围 || !输出队列类型 || !接收数量类型) {
            结果.状态 = 普通应用装配状态::连接本能装配失败;
            return 结果;
        }
        try {
            if (!当前.人类存在概念节点 || !当前.中文语言节点)
                throw std::logic_error("字段询问装配缺少正式人类或语言概念");
            // F提供真实注册、可租用的typed写回端口；普通接收链尚不派发S03/S05。
            if(!当前.语言写回运行)当前.语言写回运行=std::make_shared<语言场景写回本能运行服务>(
                当前.世界树,当前.外设世界树,当前.自然语言世界树,当前.方法服务,*通用存在);
            const auto 写回端口=新_本能函数管理类::装配语言场景写回端口(当前.语言写回运行);
            if(写回端口!=新本能函数端口装配状态::已装配&&写回端口!=新本能函数端口装配状态::精确重复)
                throw std::logic_error("language claim writeback owner conflict");
            当前.语言写回清理.已绑定=true;
            当前.语言写回运行->初始化();
            新字段询问登记配置 询问配置;
            询问配置.需求根概念=先天读回.信息->需求;
            询问配置.自我范围概念=自我材料->专属存在概念节点;
            询问配置.人类概念=*当前.人类存在概念节点;
            询问配置.通用存在概念=*通用存在;
            询问配置.输出队列类型=*输出队列类型;
            询问配置.语言节点=*当前.中文语言节点;
            询问配置.第三方指代概念=读取字段询问第三方概念(当前);
            const auto 指代名称=当前.世界事实.名称关系服务.查询指代概念名称(询问配置.第三方指代概念);
            if (指代名称.状态!=指代名称读取状态::已读取)
                throw std::logic_error("字段询问第三方名称未完整读取");
            for (const auto& n:指代名称.信息组) {
                if (n.角色!=指代名称角色::别名 || n.词条.语言节点!=询问配置.语言节点
                    || n.词条.规范化词面!="它") continue;
                if (有效(询问配置.第三方别名关系)) throw std::logic_error("字段询问第三方别名不唯一");
                询问配置.第三方别名关系=n.节点;
            }
            if (!有效(询问配置.第三方别名关系)) throw std::logic_error("字段询问第三方别名缺失");
            for (const auto slot:{1003u,1004u,1005u,1008u,1009u,1010u})
                询问配置.支持字段类型组.push_back(读取字段询问先天类型(当前,slot));
            询问配置.需求主体类型=读取字段询问先天类型(当前,1001);
            if (!当前.字段询问语言) 当前.字段询问语言=std::make_shared<字段询问语言服务>(当前.世界事实.语言服务,
                当前.世界事实.词服务,当前.世界事实.名称关系服务,当前.世界事实.存在概念服务,
                当前.世界事实.特征概念服务,当前.世界事实.存在服务,当前.自然语言世界树);
            if (!当前.字段询问运行) 当前.字段询问运行=std::make_shared<字段询问本能运行服务>(当前.世界树,当前.外设世界树,
                当前.方法服务,当前.需求服务,当前.世界事实.存在服务,当前.世界事实.特征服务,
                当前.世界事实.动态服务,当前.世界事实.存在概念服务,当前.世界事实.特征概念服务,当前.字段询问语言);
            if (!当前.字段询问运行->初始化(询问配置)) throw std::logic_error("字段询问运行初始化失败");
            const auto 询问端口=新_本能函数管理类::装配字段询问端口(当前.字段询问运行);
            if (询问端口!=新本能函数端口装配状态::已装配 && 询问端口!=新本能函数端口装配状态::精确重复)
                throw std::logic_error("字段询问登记端口所有者冲突");
            当前.字段询问清理.登记已绑定=true;
            if (!当前.方法服务.装配字段询问运行接口(当前.字段询问运行))
                throw std::logic_error("字段询问方法运行接口装配失败");
            当前.字段询问清理.方法已绑定=true;
            当前.模拟学习.配置字段询问语言(当前.字段询问语言,询问配置);
            当前.问题模拟世界.设置字段询问语言(当前.字段询问语言,询问配置);
            const auto 询问空场景=当前.方法服务.建立服务需求登记参数场景();
            if (!询问空场景.参数场景) throw std::logic_error("字段询问登记空场景未建立");
            const auto 询问登记调用=新_本能函数管理类::执行(新本能函数登记名_询问指定需求字段,*询问空场景.参数场景);
            if (询问登记调用.结果.状态!=新本能函数执行状态::已按合同返回
                || !当前.方法服务.查询本能登记(std::string(新本能函数登记名_询问指定需求字段)).方法节点)
                throw std::logic_error("字段询问头及参数规格登记失败");
            if(!当前.词条归类事实)当前.词条归类事实=std::make_shared<词条归类事实服务>(当前.世界事实.词服务,
                当前.世界事实.名称关系服务,当前.世界事实.存在概念服务,当前.世界事实.特征概念服务,当前.世界树.获取词条归类动态概念服务());
            当前.需求服务.装配词条归类读取接口(当前.词条归类事实.get());
            if(!当前.语言补全运行)当前.语言补全运行=std::make_shared<语言补全运行服务>(当前.外设世界树,当前.自然语言世界树,
                当前.世界事实.语言服务,当前.世界事实.词服务,当前.世界事实.名称关系服务,当前.世界事实.存在概念服务,
                当前.世界事实.特征概念服务,当前.世界树.获取词条归类动态概念服务(),当前.世界事实.二次特征概念服务,
                当前.世界事实.存在服务,当前.世界事实.特征值服务,当前.世界事实.特征服务,当前.世界事实.场景服务,
                当前.需求服务,当前.方法服务,当前.世界事实.动态服务,当前.接收认知);
            if(!当前.语言补全运行->初始化(询问配置))throw std::logic_error("language completion runtime initialization failed");
            当前.接收认知.装配字段答案执行(当前.方法服务,当前.世界事实.动态服务,询问配置);
            当前.接收认知.装配语言补全(当前.语言补全运行);
            当前.任务服务.装配语言补全运行接口(当前.语言补全运行);

        } catch (const std::exception& e) {
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"装配字段询问本能",L"字段询问运行依赖或方法声明未完整装配；保留已发生步骤。",e.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"装配字段询问本能",L"字段询问装配发生未知工程异常。");
        }
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"连接概念与参数准备", 1);
#endif
        当前.连接运行 = std::make_shared<连接本能运行服务>(
            当前.世界树, 当前.外设世界树, 当前.方法服务,
            当前.世界事实.存在服务, 当前.世界事实.特征服务,
            当前.世界事实.特征值服务, 当前.世界事实.状态服务,
            当前.世界事实.动态服务, 当前.世界事实.存在概念服务,
            当前.世界事实.特征概念服务, 当前.任务服务);
        const auto 连接端口 = 新_本能函数管理类::装配连接端口(当前.连接运行);
        if ((连接端口 != 新本能函数端口装配状态::已装配
                && 连接端口 != 新本能函数端口装配状态::精确重复)
            || !当前.连接运行->初始化({*结果.自我->自我存在,
                自我材料->专属存在概念节点, *通用存在,
                *交互者类型, *接收消息类型, *发送消息类型,*消息正文范围,*输出队列类型,*接收数量类型})) {
            结果.状态 = 普通应用装配状态::连接本能装配失败;
            return 结果;
        }
        const auto 连接方法 = 当前.方法服务.查询本能登记(
            std::string(新本能函数登记名_建立连接));
        if (!连接方法.方法节点) {
            结果.状态 = 普通应用装配状态::连接本能装配失败;
            return 结果;
        }
        结果.建立连接方法节点 = *连接方法.方法节点;
        当前.模拟学习.配置场景事实构成读取(*当前.连接运行);
        当前.模拟学习.配置隔离动作定义(当前.动作投影,当前.世界事实.动态服务,当前.世界事实.动态概念服务);
        当前.任务服务.配置场景事实读取(*当前.连接运行, *当前.连接运行);
        if (!当前.人类存在概念节点) {
            结果.状态 = 普通应用装配状态::人类概念初始化失败;
            return 结果;
        }
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"连接与字段询问本能初始化", 1);
#endif
        当前.服务对象运行 = std::make_shared<服务对象本能运行服务>(
            当前.世界树, 当前.方法服务, 当前.任务服务,
            当前.世界事实.存在服务, 当前.世界事实.特征服务,
            当前.世界事实.动态服务, *当前.连接运行);
        if (!当前.服务对象运行->初始化({*结果.自我->自我存在,
                自我材料->专属存在概念节点, *通用存在,
                *当前.人类存在概念节点, *交互者类型})) {
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"服务对象本能初始化", 1);
#endif
        结果.获取服务对象方法节点 = 当前.服务对象运行->已登记方法节点();
        if (!结果.获取服务对象方法节点) {
            结果.状态 = 普通应用装配状态::本能函数登记失败;
            return 结果;
        }
        结果.旧网页读取退役 = 当前.方法服务.退役旧网页本能登记(
            std::string(新本能函数登记名_读取网页对话消息));
        结果.旧服务对象退役 = 当前.方法服务.退役旧网页本能登记(
            std::string(新本能函数登记名_旧两参数获取服务对象));

        结果.旧网页设置核验 = 当前.方法服务.退役旧网页本能登记(
            std::string(新本能函数登记名_设置网页对话输入文本));
        结果.旧网页提交核验 = 当前.方法服务.退役旧网页本能登记(
            std::string(新本能函数登记名_提交网页对话消息));

        结果.任务管理绑定 =
            当前.自我线程.绑定任务管理(当前.任务管理线程);
        if (!结果.任务管理绑定->成功()) {
            结果.状态 = 普通应用装配状态::线程绑定失败;
            return 结果;
        }
        if(!当前.自我线程.绑定接收需求认知(当前.接收认知).成功()) {
            结果.状态=普通应用装配状态::线程绑定失败;return 结果;
        }
        当前.接收认知.装配任务读取接口(当前.任务服务);
        当前.任务服务.装配字段答案候选读取([&provider=当前.接收认知](const 新任务字段追问记录& r) {
            return provider.读取字段答案候选(r);
        });
        当前.任务服务.装配字段答案执行([&provider=当前.接收认知](const 新需求字段答案写回请求& q,
            新_需求类& owner,const 方法参数场景& scene) {
            return provider.执行字段答案写回本能(q,owner,scene);
        });

        结果.消息配置 = 当前.自我线程.配置消息处理({
            当前.配置.自我线程邮箱容量,
            当前.配置.资源重试间隔毫秒});
        if (!结果.消息配置->成功()) {
            结果.状态 = 普通应用装配状态::消息配置失败;
            return 结果;
        }

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"其余本能与线程绑定", 1);
#endif
        当前.服务对象新需求运行 = std::make_shared<服务对象新需求本能运行服务>(
            当前.世界树, 当前.方法服务, 当前.需求服务,
            当前.世界事实.动态服务, 当前.服务对象需求发布, 当前.服务要求, 当前.任务服务);
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"构造新需求运行服务", 1);
#endif
        新服务对象新需求配置 新需求配置;
        新需求配置.自我存在节点 = *结果.自我->自我存在;
        当前.模拟学习.配置需求执行来源(*结果.自我->自我存在,
            当前.世界事实.存在服务,当前.世界事实.存在概念服务);
        新需求配置.人类概念节点 = *当前.人类存在概念节点;
        新需求配置.通用存在概念节点 = *通用存在;
        新需求配置.对象需求类型 = 需求集合先天->需求;
        新需求配置.完整被认知需求概念 = 内容概念.信息->完整被认知需求;
        新需求配置.需求身份概念 = 先天读回.信息->需求;
        // 初始化内部仅传登记用途的空参数场景，不生成业务实参或执行动作。
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"填充已有概念登记配置", 1);
#endif
        const auto 新需求初始化成功 = 当前.服务对象新需求运行->初始化(新需求配置);
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"新需求运行服务初始化返回", 新需求初始化成功);
#endif
        if (!新需求初始化成功) {
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
        const auto 新需求登记 = 当前.方法服务.查询本能登记(
            std::string(新本能函数登记名_获取服务对象需求信息));
#if HY_EGO_STARTUP_TIMING
        观察阶段(L"装配层查询新方法登记", static_cast<std::int64_t>(新需求登记.状态));
#endif
        if (!新需求登记.方法节点) {
            结果.状态 = 普通应用装配状态::本能函数登记失败;
            return 结果;
        }
        if (!当前.任务服务.配置本能需求任务参数工厂(*当前.服务对象新需求运行)) {
            结果.状态 = 普通应用装配状态::本能函数端口装配失败;
            return 结果;
        }
        当前.需求工厂清理.已绑定 = true;

        // 只登记已实现的治理规律；不得由此补造任务或完成事实。
        const auto 自我场景=当前.世界事实.场景服务.查询所属场景(*结果.自我->自我存在);
        const auto 投影场景 = 自我场景 ? 当前.世界事实.场景服务.获取场景(*自我场景) : std::nullopt;
        const auto 治理根=当前.内部治理世界.获取根场景();
        const auto 治理范围 = 治理根 ? 当前.世界事实.存在服务.查询专属存在概念(*治理根) : std::nullopt;
        const auto 自我概念=当前.世界事实.存在服务.查询专属存在概念(*结果.自我->自我存在);
        const auto 信息概念=当前.世界事实.存在概念服务.读取先天需求任务方法概念();
        const auto 状态类型=当前.任务服务.获取任务状态特征类型();
        const auto 状态根 = 状态类型 ? 当前.世界事实.特征概念服务.获取特征概念(*状态类型) : std::nullopt;
        if (!投影场景 || !治理范围 || !自我概念 || !信息概念.信息 || !状态根
            || !当前.服务任务特征概念节点)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"初始化治理先天因果",L"正式概念或两类实际场景范围未就绪。");
        const auto 状态范围=[&](新任务状态 状态){
            特征概念定义 定义;
            定义.材料类型=状态根->材料类型;
            定义.单位=状态根->单位;
            定义.比较规则=状态根->比较规则;
            定义.聚合规则=状态根->聚合规则;
            const auto 值=static_cast<std::int64_t>(状态);
            定义.值域=std::vector<特征概念I64闭区间>{{值,值}};
            const auto 域=当前.世界树.建立特征概念(定义,*状态类型);
            if (!有效(域)) 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,
                L"初始化治理先天因果",L"正式任务状态子域未读回。");
            return 域;
        };
        const auto 先天回执=当前.治理先天初始化.初始化({*自我概念,
            信息概念.信息->需求,信息概念.信息->任务,投影场景->专属存在概念节点,*治理范围,
            *当前.服务任务特征概念节点,*状态类型,
            状态范围(新任务状态::等待自我决议),状态范围(新任务状态::已完成)});
        if (先天回执.规则组.size()!=4 || 先天回执.动作概念组.size()!=2)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"初始化治理先天因果",L"完整定义初始化回执不一致。");

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"治理先天因果初始化", 1);
#endif
        // 来源规则只描述治理关系；启动装配不采用请求、不建立D或T。
        const auto 交互者定义 = 当前.世界事实.特征概念服务.读取特征概念定义保护(*交互者类型);
        if (交互者定义.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
        if (交互者定义.前提已变化 || 交互者定义.状态 != 基础结构操作状态::已完成
            || !交互者定义.信息 || 交互者定义.信息->类型根 != *交互者类型
            || 交互者定义.信息->定义.材料类型 != 特征概念材料物理类型::存在节点引用)
            throw std::logic_error("来源前驱装配缺少正式单引用交互者定义");
        const auto& 交互根 = 交互者定义.信息->定义;
        特征概念定义 人类引用定义;
        人类引用定义.材料类型 = 交互根.材料类型;
        人类引用定义.单位 = 交互根.单位;
        人类引用定义.比较规则 = 交互根.比较规则;
        人类引用定义.聚合规则 = 交互根.聚合规则;
        人类引用定义.值域 = 特征概念存在引用值域{*当前.人类存在概念节点};
        const auto 人类交互者范围 = 当前.世界事实.特征概念服务.建立或取得特征概念(
            人类引用定义, 当前.世界事实.特征值服务, *交互者类型);
        if (!有效(人类交互者范围)) throw std::logic_error("来源前驱人类交互者范围建立未完成");
        const auto 直接子 = 当前.世界事实.特征概念服务.查询直接子特征概念(*交互者类型, 人类引用定义);
        const auto 人类范围读回 = 当前.世界事实.特征概念服务.读取特征概念定义保护(人类交互者范围);
        if (人类范围读回.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
        if (直接子.状态 != 特征概念直接子查询状态::已完成
            || 直接子.子概念节点组 != std::vector<稳定编码>{人类交互者范围}
            || 人类范围读回.前提已变化 || 人类范围读回.状态 != 基础结构操作状态::已完成
            || !人类范围读回.信息 || 人类范围读回.信息->类型根 != *交互者类型
            || 人类范围读回.信息->定义.值域 != 人类引用定义.值域)
            throw std::logic_error("来源前驱人类交互者范围正式读回不一致");
        const 新服务取得来源前驱配置 来源配置{*自我概念, *当前.人类存在概念节点,
            信息概念.信息->需求, 信息概念.信息->任务, *交互者类型, 人类交互者范围,
            新需求配置.对象需求类型, 新需求配置.完整被认知需求概念,
            *当前.服务任务特征概念节点, 投影场景->专属存在概念节点, *治理范围};
        const auto 来源回执 = 当前.治理先天初始化.初始化来源前驱(来源配置);
        if (来源回执.元数据.配置 != 来源配置)
            throw std::logic_error("来源前驱元数据配置读回不一致");
        const auto 来源配置状态 = 当前.任务服务.配置服务取得来源前驱(来源配置);
        if (来源配置状态 == 新任务操作状态::资源失败) throw std::bad_alloc{};
        if (来源配置状态 != 新任务操作状态::已修改 && 来源配置状态 != 新任务操作状态::已复用)
            throw std::logic_error("来源前驱任务配置未完成");

#if HY_EGO_STARTUP_TIMING
        观察阶段(L"来源前驱初始化与装配结束", 1);
#endif
        结果.状态 = 普通应用装配状态::已装配;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 普通应用装配状态::资源失败;
    } catch (...) {
        结果.状态 = 普通应用装配状态::内部不一致;
    }
    return 结果;
}

} // namespace 普通应用装配内部

bool 普通应用装配结果::成功() const noexcept {
    if (!治理时间概念) return false;
    const 稳定编码 治理节点[] = {治理时间概念->时间窗口余量,
        治理时间概念->时间不足, 治理时间概念->恰好足够, 治理时间概念->时间有余量};
    for (std::size_t i = 0; i < 4; ++i) {
        if (!有效(治理节点[i])) return false;
        for (std::size_t j = 0; j < i; ++j) if (治理节点[i] == 治理节点[j]) return false;
    }
    return 状态 == 普通应用装配状态::已装配
        && 初始概念名称 && 初始概念名称->状态 == 初始化概念名称状态::已完成
        && 结构化子概念 && 结构化子概念->状态 == 初始化结构化子概念状态::已完成
        && 语言知识 && 语言知识->工程控制 == 初始化语言知识工程控制::已完成
        && 语言知识->状态 == 初始化语言知识状态::已完成
        && 指代种子 && 指代种子->工程控制 == 初始化指代种子工程控制::已完成
        && 指代种子->状态 == 初始化指代种子状态::已完成
        && 指代种子->阶段 == 初始化指代种子阶段::完成
        && !指代种子->当前项
        && 指代种子->来源读回 && 指代种子->来源读回->信息
        && std::all_of(指代种子->项组.begin(), 指代种子->项组.end(),
            [](const 初始化指代种子项& 项) {
                return 项.逐项核验完成 && 项.全部复核完成
                    && 项.定义读回 && 项.定义读回->信息
                    && 项.词条读回 && 项.词条读回->信息
                    && 项.关系读回 && 项.关系读回->信息;
            })
        && 外设世界树 && 普通应用装配内部::世界初始化成功(外设世界树->状态)
        && 外设世界树->世界树根场景 && 有效(*外设世界树->世界树根场景)
        && 旧网页读取退役 && 旧服务对象退役
        && (旧网页读取退役->状态 == 新方法操作状态::方法不存在
            || 旧网页读取退役->状态 == 新方法操作状态::登记已退役)
        && (旧服务对象退役->状态 == 新方法操作状态::方法不存在
            || 旧服务对象退役->状态 == 新方法操作状态::登记已退役)
        && 旧网页设置核验 && 旧网页设置核验->状态 == 新方法操作状态::方法不存在
        && 旧网页提交核验 && 旧网页提交核验->状态 == 新方法操作状态::方法不存在
        && 世界树 && 普通应用装配内部::世界初始化成功(世界树->状态)
        && 自然语言世界树 && 自然语言世界树->成功()
        && 自然语言世界树根节点 && 有效(*自然语言世界树根节点)
        && 内部治理世界树根节点 && 有效(*内部治理世界树根节点)
        && 内部治理世界树根节点 != 自然语言世界树根节点
        && 内部治理世界树根节点 != 世界树->世界树根场景
        && 第一权限人存在节点 && 有效(*第一权限人存在节点)
        && 第一权限人专属存在概念节点
        && 有效(*第一权限人专属存在概念节点)
        && 人类存在概念节点 && 有效(*人类存在概念节点)
        && 登录名特征概念节点 && 有效(*登录名特征概念节点)
        && 姓名特征概念节点 && 有效(*姓名特征概念节点)
        && 权限角色特征概念节点 && 有效(*权限角色特征概念节点)
        && 自我治理基础 && 自我治理基础->成功()
        && 自我 && 自我->成功()
        && 任务管理绑定 && 任务管理绑定->成功()
        && 消息配置 && 消息配置->成功()
        && 获取服务对象方法节点 && 有效(*获取服务对象方法节点)
        && 语言语义差异二次特征类型根
        && 有效(*语言语义差异二次特征类型根)
        && 无法确认结果概念 && 有效(*无法确认结果概念)
        && 无法确认词条 && 有效(*无法确认词条)
        && 无法确认名称关系 && 有效(*无法确认名称关系)
        && 确认结果概念 && 有效(*确认结果概念)
        && 确认词条 && 有效(*确认词条)
        && 确认名称关系 && 有效(*确认名称关系)
        && 服务任务特征概念节点 && 有效(*服务任务特征概念节点)
        && 服务任务特征节点 && 有效(*服务任务特征节点)
        && 建立连接方法节点 && 有效(*建立连接方法节点);
}

bool 普通应用治理操作结果::成功() const noexcept {
    return 状态 == 普通应用治理操作状态::已完成
        || 状态 == 普通应用治理操作状态::无变化;
}

普通应用装配结果 构造普通应用上下文(
    const 普通应用配置& 配置) noexcept {
    using namespace 普通应用装配内部;
    if (!配置有效(配置)) return {};
    std::string 标准文本;
    std::string 子概念文本;
    std::string 语言知识文本;
    std::string 指代文本;
    try {
        标准文本 = 读取初始化标准文本(L"最小结构定义种子.txt");
        子概念文本 = 读取初始化标准文本(L"结构化子概念种子.txt");
        语言知识文本 = 读取初始化标准文本(L"最小语言知识种子.txt");
        指代文本 = 读取初始化标准文本(L"指代定义种子.txt");
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取初始化标准文本",
            L"程序旁四份固定初始化资源无法完整读取。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取初始化标准文本",
            L"程序旁四份固定初始化资源读取发生未知工程异常。");
    }
    std::unique_lock 锁(上下文锁);
    const auto 固定装配结果 = [&]() noexcept -> 普通应用装配结果 {
        try {
            return *上下文->装配结果;
        } catch (const std::exception& 异常) {
            锁.unlock();
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"固定普通应用装配结果",
                L"装配结果及名称、子概念、语言知识、指代材料无法完整复制。", 异常.what());
        } catch (...) {
            锁.unlock();
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"固定普通应用装配结果",
                L"装配结果及名称、子概念、语言知识、指代材料复制发生未知工程异常。");
        }
    };
    try {
        if (!上下文) {
            上下文 = std::make_unique<普通应用上下文>(配置);
        } else if (上下文->配置 != 配置) {
            return {};
        }
        if (上下文->装配结果 && 上下文->装配结果->成功()) {
            auto 结果 = 固定装配结果();
            auto* const 当前 = 上下文.get();
            if (当前->内部治理世界.获取根场景() != 结果.内部治理世界树根节点)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复核治理世界装配", L"治理根身份变化或不可读。");
            初始化普通应用概念名称(*当前, 结果, 标准文本, true);
            初始化普通应用结构化子概念(*当前, 结果, 子概念文本, true);
            初始化普通应用指代种子(*当前, 结果, 指代文本, true);
            初始化普通应用语言知识(*当前, 结果, 语言知识文本, true);
            const auto 同步绑定 = 当前->任务服务.绑定服务任务集合同步(
                当前, *结果.自我->完成服务需求,
                [当前](const std::vector<稳定编码>& 成员) {
                    return 同步服务任务集合特征(*当前, 成员);
                });
            if (同步绑定 != 新任务操作状态::已修改
                && 同步绑定 != 新任务操作状态::精确重复) {
                结果.状态 = 普通应用装配状态::内部不一致;
            }
            锁.unlock();
            if (结果.状态 != 普通应用装配状态::已装配) 记录普通装配观察(结果);
            return 结果;
        }
        上下文->装配结果 = 完成装配(*上下文, 标准文本, 子概念文本,
            语言知识文本, 指代文本);
        auto 结果 = 固定装配结果();
        auto* const 报告者 = &上下文->服务结算运行;
        锁.unlock();
        报告者->报告待处理工程异常();
        记录普通装配观察(结果);
        return 结果;
    } catch (const std::bad_alloc&) {
        普通应用装配结果 结果;
        结果.状态 = 普通应用装配状态::资源失败;
        if (锁.owns_lock()) 锁.unlock();
        记录普通装配观察(结果);
        return 结果;
    } catch (...) {
        普通应用装配结果 结果;
        结果.状态 = 普通应用装配状态::内部不一致;
        if (锁.owns_lock()) 锁.unlock();
        记录普通装配观察(结果);
        return 结果;
    }
}

普通应用装配结果 构造普通应用上下文() noexcept {
    return 构造普通应用上下文(普通应用配置{});
}

普通应用网页自我归类表达结果 生成普通应用网页自我归类表达(
    const 普通应用网页自我归类表达请求& 请求) noexcept {
    using namespace 普通应用装配内部;
    普通应用网页自我归类表达结果 结果;
    if (请求.公开会话标识.empty()
        || !新_语言类::严格UTF8字节串有效(请求.公开会话标识)
        || !有效(请求.归类概念节点) || 请求.最大知识条目数 == 0
        || 请求.最大名称条目数 == 0 || 请求.最大输出标量数 == 0
        || (请求.选用归类名称关系 && !有效(*请求.选用归类名称关系)))
        return 结果;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文 || !上下文->装配结果 || !上下文->装配结果->成功()) {
            结果.状态 = 普通应用网页自我归类表达状态::上下文不存在;
            return 结果;
        }
        auto& 当前 = *上下文;
        if (!当前.活跃网页会话标识
            || *当前.活跃网页会话标识 != 请求.公开会话标识) {
            结果.状态 = 普通应用网页自我归类表达状态::会话失效;
            return 结果;
        }
        const auto 宿主 = 核验网页对话当前会话(请求.公开会话标识);
        const auto 宿主状态 = [&](网页对话会话核验状态 状态) {
            switch (状态) {
            case 网页对话会话核验状态::宿主未运行:
                return 普通应用网页自我归类表达状态::来源不可用;
            case 网页对话会话核验状态::会话不匹配:
                return 普通应用网页自我归类表达状态::会话失效;
            case 网页对话会话核验状态::输入拒绝:
                return 普通应用网页自我归类表达状态::输入拒绝;
            case 网页对话会话核验状态::资源失败:
                return 普通应用网页自我归类表达状态::工程未完成;
            default: return 普通应用网页自我归类表达状态::内部不一致;
            }
        };
        if (宿主.状态 != 网页对话会话核验状态::已匹配) {
            结果.状态 = 宿主状态(宿主.状态);
            return 结果;
        }
        if (!当前.装配结果->自我 || !当前.装配结果->自我->自我存在
            || !当前.指代种子) {
            结果.状态 = 普通应用网页自我归类表达状态::内部不一致;
            return 结果;
        }
        // 旧现实窗口来源及其全局游标已退出；不得以残留句柄重走旧表达路径。
        结果.状态 = 普通应用网页自我归类表达状态::来源不可用;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 普通应用网页自我归类表达状态::工程未完成;
        结果.表达.reset(); 结果.已核验来源.reset();
        return 结果;
    } catch (const std::length_error&) {
        结果.状态 = 普通应用网页自我归类表达状态::工程未完成;
        结果.表达.reset(); 结果.已核验来源.reset();
        return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"网页自我归类表达",
            L"已核验来源的语言表达发生工程异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"网页自我归类表达",
            L"已核验来源的语言表达发生未知工程异常。");
    }
}

治理时序解释结果 计算普通应用时序语义(
    const 需求时序可达性裁决请求& 请求) noexcept {
    try {
        using namespace 普通应用装配内部;
        std::lock_guard 锁(上下文锁);
        if (!上下文 || !上下文->装配结果 || !上下文->装配结果->成功()) return {};
        return 上下文->治理二次特征.裁决时序(请求);
    } catch (const std::bad_alloc&) {
        治理时序解释结果 结果;
        结果.状态 = 治理二次特征状态::资源失败;
        return 结果;
    } catch (...) {
        治理时序解释结果 结果;
        结果.状态 = 治理二次特征状态::结构不一致;
        return 结果;
    }
}

普通应用治理操作结果 启动普通应用治理线程() noexcept {
    using namespace 普通应用装配内部;
    std::lock_guard 启停守卫(治理启停锁);
    auto* const 当前 = 读取治理启停上下文(true);
    普通应用治理操作结果 结果;
    if (!当前) {
        return 结果;
    }
    try {
        const auto 管理启动起点 = std::chrono::steady_clock::now();
        结果.任务管理 = 当前->任务管理线程.启动({
            当前->配置.任务管理线程邮箱容量,
            当前->配置.资源重试间隔毫秒}, 当前->配置.线程进入等待毫秒);
        记录治理线程启动返回(L"任务管理", *结果.任务管理,
            当前->配置.线程进入等待毫秒, 管理启动起点);
        if (!管理启动成功(结果.任务管理->状态)) {
            (void)当前->任务管理线程.请求停止();
            (void)当前->任务管理线程.等待停止(当前->配置.线程停止回收等待毫秒);
            结果.状态 = 普通应用治理操作状态::任务管理启动失败;
            return 结果;
        }

        const auto 自我启动起点 = std::chrono::steady_clock::now();
        结果.自我线程 = 当前->自我线程.启动(当前->配置.线程进入等待毫秒);
        记录治理线程启动返回(L"自我", *结果.自我线程,
            当前->配置.线程进入等待毫秒, 自我启动起点);
        if (!自我启动成功(结果.自我线程->状态)) {
            (void)当前->自我线程.请求停止();
            (void)当前->自我线程.等待停止(当前->配置.线程停止回收等待毫秒);
            (void)当前->任务管理线程.请求停止();
            (void)当前->任务管理线程.等待停止(当前->配置.线程停止回收等待毫秒);
            结果.状态 = 普通应用治理操作状态::自我线程启动失败;
            return 结果;
        }
        bool 原已启动;
        { std::lock_guard 锁(上下文锁); 原已启动 = 当前->治理线程已启动; 当前->治理线程已启动 = true; }
        结果.状态 = 原已启动
            ? 普通应用治理操作状态::无变化
            : 普通应用治理操作状态::已完成;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"启动普通应用治理线程", L"启动控制发生工程异常。");
    }
}

普通应用治理操作结果 停止并回收普通应用治理线程() noexcept {
    using namespace 普通应用装配内部;
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
    记录装配停止阶段(L"正常停止入口_启停锁前");
#endif
    std::lock_guard 启停守卫(治理启停锁);
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
    记录装配停止阶段(L"已取得启停锁_上下文读取前");
#endif
    auto* const 当前 = 读取治理启停上下文(false);
    普通应用治理操作结果 结果;
    if (!当前) return 结果;
    try {
        const auto 自我停止 = 当前->自我线程.请求停止();
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"self请求已返回_manager请求前");
#endif
        const auto 管理停止 = 当前->任务管理线程.请求停止();
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"manager请求已返回_采集停止前");
#endif
        停止普通控制采集(*当前);
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"采集停止已返回_self等待前");
#endif
        const auto 自我等待 = 当前->自我线程.等待停止(当前->配置.线程停止回收等待毫秒);
        结果.自我线程 = 自我等待;
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"self等待已返回_manager等待前");
#endif

        const auto 管理等待 = 当前->任务管理线程.等待停止(当前->配置.线程停止回收等待毫秒);
        结果.任务管理 = 管理等待;
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
        记录装配停止阶段(L"manager等待已返回");
#endif

        if (!自我停止成功(自我停止.状态)
            || !自我停止成功(自我等待.状态)) {
            结果.状态 = 普通应用治理操作状态::自我线程停止失败;
            return 结果;
        }
        if (!管理停止成功(管理停止.状态)
            || !管理停止成功(管理等待.状态)) {
            结果.状态 = 普通应用治理操作状态::任务管理停止失败;
            return 结果;
        }
        bool 原已启动;
        { std::lock_guard 锁(上下文锁); 原已启动 = 当前->治理线程已启动; 当前->治理线程已启动 = false; }
        结果.状态 = 原已启动
            ? 普通应用治理操作状态::已完成
            : 普通应用治理操作状态::无变化;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"停止并回收普通应用治理线程", L"停止控制发生工程异常。");
    }
}

void 回收普通应用上下文() noexcept {
    using namespace 普通应用装配内部;
    // 有界停止即便未完成，原上下文析构仍保留最终join，不提前销毁依赖。
    (void)停止并回收普通应用治理线程();
    try {
        std::lock_guard 启停守卫(治理启停锁);
        std::unique_ptr<普通应用上下文> 待回收;
        {
            std::lock_guard 锁(上下文锁);
            待回收 = std::move(上下文);
        }
        // 不持上下文锁进入析构，避免最后的解绑回调重入同一把锁。
        待回收.reset();
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收普通应用上下文",
            L"正常程序退出清理发生工程异常，不绕过最终资源回收。");
    }
}

网页服务输入状态 开始普通应用网页会话(std::string_view 公开会话标识) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文 || !上下文->装配结果 || !上下文->装配结果->成功())
            return 网页服务输入状态::结构不一致;
        auto& 当前 = *上下文;
        if (!是有效UTF8(公开会话标识)) return 网页服务输入状态::输入不合法;
        if (当前.活跃网页会话标识) {
            if (*当前.活跃网页会话标识 != 公开会话标识) return 网页服务输入状态::会话失效;
            return 当前.网页会话绑定人类 ? 核验服务会话(当前, 公开会话标识) : 网页服务输入状态::匿名会话;
        }
        if (!当前.配置.本次网页会话服务对象登录名.empty()) {
            const auto 绑定 = 可信服务对象可绑定(当前);
            if (绑定 != 网页服务输入状态::已完成) return 绑定;
        }
        // 先完成可能分配的字符串，再发布进程绑定。
        std::string 会话(公开会话标识);
        const auto root=当前.世界树.获取世界树根场景();
        if(!root||!当前.中文语言节点)return 网页服务输入状态::结构不一致;
        if(!当前.装配结果->自我||!当前.装配结果->自我->自我存在)
            return 网页服务输入状态::结构不一致;
        const auto self=*当前.装配结果->自我->自我存在;
        const auto identity=当前.世界事实.存在服务.读取存在身份保护(self);
        if(identity.状态==基础结构操作状态::资源失败)return 网页服务输入状态::资源失败;
        if(identity.状态!=基础结构操作状态::已完成||identity.前提已变化||!identity.信息)
            return 网页服务输入状态::结构不一致;
        const auto human=当前.配置.本次网页会话服务对象登录名.empty()?std::nullopt:当前.第一权限人存在节点;
        const auto registered=当前.接收认知.登记会话语境({会话,human,*root,*当前.中文语言节点,self});
        if(registered==接收认知语境接纳状态::资源失败)return 网页服务输入状态::资源失败;
        if(registered!=接收认知语境接纳状态::已登记&&registered!=接收认知语境接纳状态::精确重复)
            return 网页服务输入状态::会话失效;
        当前.活跃网页会话标识 = std::move(会话);
        if (!当前.配置.本次网页会话服务对象登录名.empty()) {
            当前.网页会话绑定人类 = 当前.第一权限人存在节点;
            return 网页服务输入状态::已完成;
        }
        return 网页服务输入状态::匿名会话;
    } catch (const std::bad_alloc&) { return 网页服务输入状态::资源失败; }
    catch (...) { return 网页服务输入状态::结构不一致; }
}

void 结束普通应用网页会话(std::string_view 公开会话标识) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文 || !上下文->活跃网页会话标识
            || *上下文->活跃网页会话标识 != 公开会话标识) return;
        auto& 当前 = *上下文;
        当前.接收认知.封存会话(公开会话标识);
        当前.活跃网页会话标识.reset();
        当前.网页会话绑定人类.reset();
        // 只结束连接授权，不删除外设事实和历史语言确认。
    } catch (...) {
        // noexcept宿主回收边界；正常清理仅销毁缓存，不分配或写领域事实。
    }
}

void 通知普通应用接收事实已记录(std::string_view 会话,std::string_view 页面,std::uint64_t 序号) noexcept {
    (void)会话;(void)页面;(void)序号;
    try {
        std::shared_ptr<普通应用装配内部::普通控制快照缓存> 缓存;
        {
            std::lock_guard lock(普通应用装配内部::上下文锁);
            if (普通应用装配内部::上下文) {
                普通应用装配内部::上下文->自我线程.通知接收认知变化();
                缓存 = 普通应用装配内部::上下文->控制缓存;
            }
        }
        if (缓存) {
            缓存->入站后允许采集.store(true, std::memory_order_release);
            缓存->唤醒.notify_all();
        }
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"通知普通应用接收认知", L"正式接收事实唤醒异常。");
    }
}

网页服务目标选项结果 查询普通应用服务目标选项(std::string_view 公开会话标识) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文) return {网页服务输入状态::结构不一致};
        auto& 当前 = *上下文;
        const auto 会话状态 = 核验服务会话(当前, 公开会话标识);
        if (会话状态 != 网页服务输入状态::已完成) return {会话状态};
        const auto 人 = 当前.世界事实.存在服务.获取存在(*当前.网页会话绑定人类);
        if (!人 || 人->节点 != *当前.网页会话绑定人类) return {网页服务输入状态::结构不一致};
        std::set<std::pair<std::uint64_t, std::uint64_t>> 唯一提示;
        for (const auto 节点 : 人->特征节点组) {
            const auto 特征 = 当前.世界事实.特征服务.获取特征(节点);
            if (!特征 || 特征->节点 != 节点) return {网页服务输入状态::结构不一致};
            const auto 概念 = 当前.世界事实.特征概念服务.获取特征概念(特征->特征概念节点);
            const auto 关系 = 当前.世界事实.特征概念服务.比较单特征概念(
                特征->特征概念节点, 特征->特征概念节点);
            if (关系.状态 == 特征概念规则状态::资源失败) return {网页服务输入状态::资源失败};
            if (!概念 || 概念->节点 != 特征->特征概念节点
                || 关系.状态 != 特征概念规则状态::已完成 || !关系.特征类型根)
                return {网页服务输入状态::结构不一致};
            唯一提示.emplace(关系.特征类型根->值, 特征->特征概念节点.值);
        }
        网页服务目标选项结果 结果;
        结果.状态 = 网页服务输入状态::已完成;
        结果.服务对象编码 = 人->节点.值;
        for (const auto& [类型, 概念] : 唯一提示) 结果.当前特征提示组.push_back({类型, 概念});
        return 结果;
    } catch (const std::bad_alloc&) { return {网页服务输入状态::资源失败}; }
    catch (...) { return {网页服务输入状态::结构不一致}; }
}

网页服务要求结果 预览普通应用服务要求(std::string_view 公开会话标识,
    const 网页服务目标提交& 目标) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文) return {网页服务输入状态::结构不一致};
        auto& 当前 = *上下文;
        const auto 会话状态 = 核验服务会话(当前, 公开会话标识);
        if (会话状态 != 网页服务输入状态::已完成) return {会话状态};
        if (!目标.目标存在编码 || !目标.特征类型编码 || !目标.目标形式) return {网页服务输入状态::输入不完整};
        const auto 目标状态 = 核验服务要求现实目标(当前, 稳定编码{目标.目标存在编码});
        if (目标状态 != 网页服务输入状态::已完成) return {目标状态};
        服务要求理解请求 请求;
        请求.公开会话标识 = 公开会话标识;
        请求.来源人类节点 = *当前.网页会话绑定人类;
        请求.目标存在节点 = 稳定编码{目标.目标存在编码};
        请求.目标.目标特征类型概念节点 = 稳定编码{目标.特征类型编码};
        if (目标.目标形式 == 1) {
            if (目标.变化方向) return {网页服务输入状态::输入不合法};
            if (!目标.目标域编码 || !目标.比较概念编码) return {网页服务输入状态::输入不完整};
            if (!*目标.目标域编码 || !*目标.比较概念编码) return {网页服务输入状态::输入不合法};
            请求.目标.目标形式 = 新需求目标形式::值;
            请求.目标.目标值域概念节点 = 稳定编码{*目标.目标域编码};
            请求.目标.目标比较关系概念节点 = 稳定编码{*目标.比较概念编码};
        } else if (目标.目标形式 == 2) {
            if (目标.目标域编码 || 目标.比较概念编码) return {网页服务输入状态::输入不合法};
            if (!目标.变化方向) return {网页服务输入状态::输入不完整};
            if (*目标.变化方向 != 1 && *目标.变化方向 != 2) return {网页服务输入状态::输入不合法};
            请求.目标.目标形式 = 新需求目标形式::变化方向;
            请求.目标.目标变化方向 = *目标.变化方向 == 1 ? 新需求变化方向::增加 : 新需求变化方向::减少;
        } else return {网页服务输入状态::输入不合法};
        const auto 理解 = 当前.服务要求.建立或取得理解(请求);
        const auto 状态 = 映射服务要求状态(理解.状态);
        if (!服务输入成功(状态)) return {状态};
        if (!理解.理解) return {网页服务输入状态::结构不一致};
        auto 显示 = 形成服务要求显示(当前, *理解.理解);
        if (!显示) return {网页服务输入状态::结构不一致};
        return {状态, std::move(显示)};
    } catch (const std::bad_alloc&) { return {网页服务输入状态::资源失败}; }
    catch (...) { return {网页服务输入状态::结构不一致}; }
}

网页服务要求结果 读取普通应用服务要求预览(std::string_view 公开会话标识,
    std::uint64_t 理解编码) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文) return {网页服务输入状态::结构不一致};
        auto& 当前 = *上下文;
        const auto 会话状态 = 核验服务会话(当前, 公开会话标识);
        if (会话状态 != 网页服务输入状态::已完成) return {会话状态};
        if (!理解编码) return {网页服务输入状态::输入不合法};
        const auto 理解 = 当前.服务要求.读取理解(稳定编码{理解编码});
        const auto 状态 = 映射服务要求状态(理解.状态);
        if (!服务输入成功(状态)) return {状态};
        if (!理解.理解) return {网页服务输入状态::结构不一致};
        if (理解.理解->公开会话标识 != 公开会话标识
            || 理解.理解->来源人类节点 != *当前.网页会话绑定人类)
            return {网页服务输入状态::对象不匹配};
        auto 显示 = 形成服务要求显示(当前, *理解.理解);
        if (!显示) return {网页服务输入状态::结构不一致};
        return {状态, std::move(显示)};
    } catch (const std::bad_alloc&) { return {网页服务输入状态::资源失败}; }
    catch (...) { return {网页服务输入状态::结构不一致}; }
}

namespace 普通应用装配内部 {
网页服务材料投递状态 投递完整服务需求材料(普通应用上下文& 当前, 稳定编码 要求) {
    // 网页请求先保存于材料owner；只有最新R完整且已有U/C才交self。
    const auto 读取 = 当前.服务要求.读取部分服务要求(要求);
    if (读取.状态 != 新部分服务要求读取状态::已读取 || !读取.材料)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"投递完整服务需求材料",
            L"已发布服务要求无法完整读回。");
    const auto& 材料 = *读取.材料;
    if (!查询服务要求缺项(材料).empty() || !材料.理解节点 || !材料.确认节点)
        return 网页服务材料投递状态::未尝试;
    if (材料.已知目标.目标存在节点 != 材料.来源人类节点)
        return 网页服务材料投递状态::等待后继消费;
    const 新完整服务需求材料包 包{材料.要求节点, *材料.理解节点, *材料.确认节点};
    const auto 回执 = 当前.自我线程.提交完整服务需求材料(包);
    switch (回执.状态) {
    case 新自我消息接收状态::已接收:
    case 新自我消息接收状态::精确重复: return 网页服务材料投递状态::消息已接纳;
    case 新自我消息接收状态::邮箱已满:
    case 新自我消息接收状态::尚未启动:
    case 新自我消息接收状态::已停止接收: return 网页服务材料投递状态::暂未接纳;
    case 新自我消息接收状态::资源失败:
    default:
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"投递完整服务需求材料",
            L"完整服务需求材料未能按已装配合同交给自我线程。");
    }
}
std::string 服务要求缺项名称(新服务要求缺项 项) {
    switch (项) {
    case 新服务要求缺项::目标存在: return "目标存在";
    case 新服务要求缺项::目标特征类型: return "目标特征类型";
    case 新服务要求缺项::目标形式: return "目标形式";
    case 新服务要求缺项::目标值域: return "目标值域";
    case 新服务要求缺项::目标比较关系: return "目标比较关系";
    case 新服务要求缺项::目标变化方向: return "目标变化方向";
    case 新服务要求缺项::明确确认: return "明确确认";
    }
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务要求缺项名称", L"缺项枚举不合法。");
}
网页部分服务要求结果 形成部分要求网页结果(普通应用上下文& 当前,
    const 部分服务要求结果& r) {
    网页部分服务要求结果 out;
    switch (r.状态) {
    case 部分服务要求状态::已建立:
    case 部分服务要求状态::已补充:
    case 部分服务要求状态::已读取: out.状态 = 网页服务输入状态::已完成; break;
    case 部分服务要求状态::精确重复: out.状态 = 网页服务输入状态::已复用; break;
    case 部分服务要求状态::来源不匹配: out.状态 = 网页服务输入状态::对象不匹配; break;
    case 部分服务要求状态::已知值冲突: out.状态 = 网页服务输入状态::已知值冲突; break;
    case 部分服务要求状态::输入不合法: out.状态 = 网页服务输入状态::输入不合法; break;
    case 部分服务要求状态::要求不存在: out.状态 = 网页服务输入状态::要求不存在; break;
    case 部分服务要求状态::需重新确认: out.状态 = 网页服务输入状态::需重新确认; break;
    default: 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"形成部分要求网页结果", L"服务要求owner发生工程故障。");
    }
    for (const auto x : r.缺项) out.缺项.push_back(服务要求缺项名称(x));
    for (const auto x : r.冲突项) out.冲突项.push_back(服务要求缺项名称(x));
    if (!r.材料) {
        if (服务输入成功(out.状态)) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"形成部分要求网页结果", L"成功要求结果缺少正式R材料。");
        return out;
    }
    const auto& v = *r.材料; const auto& t = v.已知目标;
    out.要求编码 = v.要求节点.值; out.来源要求标识 = v.来源要求标识;
    out.服务对象编码 = v.来源人类节点.值;
    if (t.目标存在节点) out.已知目标.目标存在编码 = t.目标存在节点->值;
    if (t.目标特征类型概念节点) out.已知目标.特征类型编码 = t.目标特征类型概念节点->值;
    if (t.目标形式) out.已知目标.目标形式 = static_cast<std::int64_t>(*t.目标形式);
    if (t.目标值域概念节点) out.已知目标.目标域编码 = t.目标值域概念节点->值;
    if (t.目标比较关系概念节点) out.已知目标.比较概念编码 = t.目标比较关系概念节点->值;
    if (t.目标变化方向) out.已知目标.变化方向 = static_cast<std::int64_t>(*t.目标变化方向);
    if (r.理解 && out.状态 != 网页服务输入状态::需重新确认) {
        out.完整预览 = 形成服务要求显示(当前, *r.理解, v.确认节点);
        if (!out.完整预览) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"形成部分要求网页结果", L"完整理解无法生成正式重述。");
    }
    return out;
}
} // namespace 普通应用装配内部

网页部分服务要求结果 提交普通应用部分服务要求(std::string_view 会话,
    const 网页部分服务要求提交& 输入) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文) return {网页服务输入状态::未绑定};
        auto& 当前 = *上下文;
        const auto 状态 = 核验服务会话(当前, 会话);
        if (状态 != 网页服务输入状态::已完成) return {状态};
        部分服务要求提交 请求;
        请求.公开会话标识 = 会话; 请求.来源要求标识 = 输入.来源要求标识;
        请求.来源人类节点 = *当前.网页会话绑定人类;
        if (输入.要求编码) 请求.要求节点 = 稳定编码{*输入.要求编码};
        auto& t = 请求.补充目标;
        if (输入.目标存在编码) t.目标存在节点 = 稳定编码{*输入.目标存在编码};
        if (输入.特征类型编码) t.目标特征类型概念节点 = 稳定编码{*输入.特征类型编码};
        if (输入.目标域编码) t.目标值域概念节点 = 稳定编码{*输入.目标域编码};
        if (输入.比较概念编码) t.目标比较关系概念节点 = 稳定编码{*输入.比较概念编码};
        if (输入.目标形式) t.目标形式 = static_cast<新需求目标形式>(*输入.目标形式);
        if (输入.变化方向) t.目标变化方向 = static_cast<新需求变化方向>(*输入.变化方向);
        const auto 已有 = 当前.服务要求.查询部分要求(会话, 请求.来源人类节点,
            std::string_view{请求.来源要求标识});
        if (已有.状态 == 部分服务要求状态::输入不合法) return {网页服务输入状态::输入不合法};
        const bool 有旧 = 已有.状态 == 部分服务要求状态::已读取
            || 已有.状态 == 部分服务要求状态::需重新确认;
        if ((有旧 && !已有.材料) || (!有旧 && 已有.状态 != 部分服务要求状态::要求不存在))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交普通应用部分服务要求", L"来源键查询发生工程故障。");
        if (请求.要求节点) {
            const auto 显式 = 当前.服务要求.查询部分要求(会话, 请求.来源人类节点, *请求.要求节点);
            if (显式.状态 == 部分服务要求状态::来源不匹配) return {网页服务输入状态::对象不匹配};
            if (显式.状态 == 部分服务要求状态::要求不存在) return {网页服务输入状态::要求不存在};
            if ((显式.状态 != 部分服务要求状态::已读取
                    && 显式.状态 != 部分服务要求状态::需重新确认) || !显式.材料)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交普通应用部分服务要求", L"显式要求查询发生工程故障。");
            if (显式.材料->来源要求标识 != 请求.来源要求标识)
                return {网页服务输入状态::对象不匹配};
            if (!有旧 || 已有.材料->要求节点 != 显式.材料->要求节点)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交普通应用部分服务要求", L"来源键与显式要求定位矛盾。");
        }
        std::set<稳定编码> 目标组;
        if (t.目标存在节点) 目标组.insert(*t.目标存在节点);
        if (有旧 && 已有.材料->已知目标.目标存在节点)
            目标组.insert(*已有.材料->已知目标.目标存在节点);
        for (const auto 目标 : 目标组) {
            const auto 范围状态 = 核验服务要求现实目标(当前, 目标);
            if (范围状态 != 网页服务输入状态::已完成) return {范围状态};
        }
        auto 结果 = 形成部分要求网页结果(当前, 当前.服务要求.提交或补充要求(请求));
        if (服务输入成功(结果.状态)) {
            if (!结果.要求编码) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交普通应用部分服务要求", L"成功要求缺少R编码。");
            结果.材料投递状态 = 投递完整服务需求材料(当前, 稳定编码{*结果.要求编码});
            结果.消息已接纳 = 结果.材料投递状态 == 网页服务材料投递状态::消息已接纳;
        }
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交普通应用部分服务要求", L"部分要求输入处理异常。");
    }
}

网页部分服务要求结果 读取普通应用部分服务要求(std::string_view 会话,
    std::uint64_t 要求) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文) return {网页服务输入状态::未绑定};
        auto& 当前 = *上下文;
        const auto 状态 = 核验服务会话(当前, 会话);
        if (状态 != 网页服务输入状态::已完成) return {状态};
        return 形成部分要求网页结果(当前, 当前.服务要求.查询部分要求(
            会话, *当前.网页会话绑定人类, 稳定编码{要求}));
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取普通应用部分服务要求", L"部分要求查询处理异常。");
    }
}

网页服务要求结果 确认普通应用服务要求(std::string_view 公开会话标识,
    std::uint64_t 理解编码) noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文) return {网页服务输入状态::结构不一致};
        auto& 当前 = *上下文;
        const auto 会话状态 = 核验服务会话(当前, 公开会话标识);
        if (会话状态 != 网页服务输入状态::已完成) return {会话状态};
        if (!理解编码) return {网页服务输入状态::输入不合法};
        const auto 读取 = 当前.服务要求.读取理解(稳定编码{理解编码});
        const auto 读取状态 = 映射服务要求状态(读取.状态);
        if (!服务输入成功(读取状态)) return {读取状态};
        if (!读取.理解) return {网页服务输入状态::结构不一致};
        if (读取.理解->公开会话标识 != 公开会话标识
            || 读取.理解->来源人类节点 != *当前.网页会话绑定人类)
            return {网页服务输入状态::对象不匹配};
        const auto 目标状态 = 核验服务要求现实目标(当前, 读取.理解->目标存在节点);
        if (目标状态 != 网页服务输入状态::已完成) return {目标状态};
        const auto 确认 = 当前.服务要求.确认理解(公开会话标识, *当前.网页会话绑定人类, 稳定编码{理解编码});
        if (确认.状态 == 结构化服务要求状态::结构不一致
            || 确认.状态 == 结构化服务要求状态::资源失败)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认普通应用服务要求", L"确认发布或正式材料读取发生工程故障。");
        const auto 状态 = 映射服务要求状态(确认.状态);
        if (!服务输入成功(状态)) return {状态};
        if (!确认.确认) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认普通应用服务要求", L"成功确认缺少正式材料。");
        if (确认.确认->需重新确认) return {网页服务输入状态::需重新确认};
        auto 投递 = 网页服务材料投递状态::未尝试;
        if (确认.确认->理解.来源要求节点)
            投递 = 投递完整服务需求材料(当前, *确认.确认->理解.来源要求节点);
        auto 显示 = 形成服务要求显示(当前, 确认.确认->理解, 确认.确认->节点);
        if (!显示) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认普通应用服务要求", L"正式确认无法形成重述。");
        return {状态, std::move(显示), 投递};
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认普通应用服务要求", L"确认交接发生工程异常。"); }
}

namespace {
bool 网页目标格式有效(const 网页对话框目标& 目标) noexcept {
    const auto& 标识 = 目标.对话框标识;
    if (目标.公开会话标识.empty() || !新_语言类::严格UTF8字节串有效(目标.公开会话标识)
        || 标识.size() != 36 || 标识[14] != '4'
        || (标识[19] != '8' && 标识[19] != '9' && 标识[19] != 'a' && 标识[19] != 'b')) return false;
    for (std::size_t 序号 = 0; 序号 < 标识.size(); ++序号) {
        if (序号 == 8 || 序号 == 13 || 序号 == 18 || 序号 == 23) {
            if (标识[序号] != '-') return false;
        } else if (!((标识[序号] >= '0' && 标识[序号] <= '9')
            || (标识[序号] >= 'a' && 标识[序号] <= 'f'))) return false;
    }
    return true;
}
}

新连接返回 开始普通应用连接(const 新连接请求& 请求) noexcept {
    std::lock_guard 锁(普通应用装配内部::上下文锁);
    const auto& 当前 = 普通应用装配内部::上下文;
    return 当前 && 当前->装配结果 && 当前->装配结果->成功() && 当前->连接运行
        ? 当前->连接运行->开始(请求) : 新连接返回{};
}

新连接返回 推进普通应用连接(稳定编码 调用身份) noexcept {
    std::lock_guard 锁(普通应用装配内部::上下文锁);
    const auto& 当前 = 普通应用装配内部::上下文;
    return 当前 && 当前->装配结果 && 当前->装配结果->成功() && 当前->连接运行
        ? 当前->连接运行->推进(调用身份) : 新连接返回{新连接状态::来源失效};
}

新连接返回 取消普通应用连接(稳定编码 调用身份) noexcept {
    std::lock_guard 锁(普通应用装配内部::上下文锁);
    const auto& 当前 = 普通应用装配内部::上下文;
    return 当前 && 当前->装配结果 && 当前->装配结果->成功() && 当前->连接运行
        ? 当前->连接运行->取消(调用身份) : 新连接返回{新连接状态::来源失效};
}

网页控制面板回调组 取得普通应用控制面板回调() noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 启停守卫(治理启停锁);
        auto* const 当前 = 读取治理启停上下文(true);
        if (!当前) return {};
        if (!当前->控制缓存) 当前->控制缓存 = std::make_shared<普通控制快照缓存>();
        const auto 缓存 = 当前->控制缓存;
        bool 可采集;
        { std::lock_guard 锁(上下文锁); 可采集 = 当前->治理线程已启动; }
        if (可采集 && !当前->控制采集线程.joinable()) {
            try {
                当前->控制采集线程 = std::jthread([当前, 缓存](std::stop_token 停止) {
                    采集普通控制面板(*当前, 缓存, 停止);
                });
            } catch (const std::system_error&) {
                { std::lock_guard 锁(缓存->互斥); 缓存->状态 = 网页控制快照状态::采集失败; }
                (void)记录运行日志(L"控制界面", L"启动只读采集", L"只读采集线程资源不可用，业务线程保持原状态。");
            }
        } else if (!可采集) {
            std::lock_guard 锁(缓存->互斥); 缓存->状态 = 网页控制快照状态::已停止;
        }
        return {[缓存]() -> 网页控制快照读取结果 {
            std::lock_guard 锁(缓存->互斥);
            std::uint64_t 年龄 = 0;
            if (缓存->正文) 年龄 = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - 缓存->完成时间).count());
            return {缓存->状态, 缓存->正文, 缓存->序号, 年龄, 缓存->正在采集};
        }};
    } catch (const std::bad_alloc&) {
        (void)记录运行日志(L"控制界面", L"建立只读采集回调", L"控制面板缓存资源不足，未取得快照，不改变业务状态。");
        return {[] { return 网页控制快照读取结果{网页控制快照状态::采集失败}; }};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立控制面板只读采集", L"采集装配工程异常，不能伪装成已取得数据。");
    }
}

网页外设运行回调组 取得普通应用网页外设运行回调() noexcept {
    using namespace 普通应用装配内部;
    try {
        std::lock_guard 锁(上下文锁);
        if (!上下文)
            throw std::runtime_error("peripheral module context unavailable");
        // 只在组合时取得闭包；闭包独立保活适配状态，不借用普通上下文。
        return 上下文->网页外设模块.取得运行回调();
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"取得网页外设模块回调",
            L"外设模块未就绪或回调分配失败，不能伪装空外设。");
    }
}

} // namespace 海中鱼巣
