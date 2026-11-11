#pragma once

// 调试切片清单与开关（日志系统规范 §2.3、§4）。
// 每个调试切片在这里登记唯一枚举值、启用宏和人读登记；调用统一写作
//     调试日志<调试切片::名>(模块, 入口, 内容)
// 宏关闭时该切片的输出在编译期被丢弃。未在此登记的切片名不能编译；
// 直接写调试日志文件的底层入口不对外暴露（见 核心/日志系统.h）。
// 输出位置统一为 日志/调试_<切片名>.log。

#include <cstdint>
#include <string_view>

namespace 海中鱼巣 {

enum class 调试切片 : std::uint8_t {
    // 目的：参数绑定、方法选择、条件子需求、派发与线程间消息收发排查。
    // 启用宏：HY_EGO_DEBUG_LOG_PARAM_BINDING。影响文件：领域/新_任务类.cpp、领域/新_方法类.cpp、
    // 线程/新_任务管理线程类.cpp、线程/新_自我线程类.cpp、线程/新_任务工作线程类.cpp、业务/模拟学习.服务.cpp、
    // 业务/方法问题学习.服务.cpp、业务/服务对象.本能运行服务.cpp。关闭条件：INSTINCT-CONTRACT 阶段排查结束。
    // 验证方式：诊断版运行读 调试_PARAM_BINDING.log。
    PARAM_BINDING,
    // 目的：参数绑定逐参数、逐条件明细（宏 海中鱼巣参数绑定诊断；量大，每轮调度扫描上万条）。
    // 启用宏：HY_EGO_DEBUG_LOG_PARAM_BINDING_DETAIL。影响文件：领域/新_任务类.cpp。输出位置：调试_PARAM_BINDING_DETAIL.log。
    // 关闭条件：参数绑定明细排查结束。验证方式：诊断版运行读 调试_PARAM_BINDING_DETAIL.log。
    PARAM_BINDING_DETAIL,
    // 目的：模拟学习进度（分支、采集、样例、漂移）。启用宏：HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS。
    // 影响文件：业务/模拟学习.服务.cpp、业务/因果.任务筹办服务.cpp、业务/服务对象需求.发布服务.cpp、
    // 领域/新_任务类.cpp、领域/新_方法类.cpp、领域/新_模拟世界类.cpp、领域/新_需求类.cpp、线程/新_自我线程类.cpp。
    // 关闭条件：模拟学习排查结束。验证方式：诊断版运行读 调试_SIMULATION_PROGRESS.log。
    SIMULATION_PROGRESS,
    // 目的：接收需求认知推进节奏。启用宏：HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_SCHED（原无开关，2026-11-11 补登记）。
    // 影响文件：业务/接收需求认知.服务.cpp、线程/新_自我线程类.cpp。关闭条件：接收认知节奏排查结束。
    // 验证方式：诊断版运行读 调试_RECEIVED_DEMAND_SCHED.log。
    RECEIVED_DEMAND_SCHED,
    // 目的：接收需求解析过程。启用宏：HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE。影响文件：业务/接收需求认知.服务.cpp。
    // 关闭条件：解析排查结束。验证方式：诊断版运行读 调试_RECEIVED_DEMAND_PARSE.log。
    RECEIVED_DEMAND_PARSE,
    // 目的：主线阶段推进（自我、任务管理、装配）。启用宏：HY_EGO_DEBUG_LOG_MAINLINE_PHASE。
    // 影响文件：线程/新_任务管理线程类.cpp、线程/新_自我线程类.cpp、装配.普通应用.cpp。
    // 关闭条件：主线阶段排查结束。验证方式：诊断版运行读 调试_MAINLINE_PHASE.log。
    MAINLINE_PHASE,
    // 目的：语言输入解释与场景转换阶段。启用宏：HY_EGO_DEBUG_LOG_LANGUAGE_INPUT_PHASE。
    // 影响文件：业务/解释.基础语言.cpp、业务/转换.语言场景基础.cpp。关闭条件：语言输入排查结束。
    // 验证方式：诊断版运行读 调试_LANGUAGE_INPUT_PHASE.log。
    LANGUAGE_INPUT_PHASE,
    // 目的：因果采用。启用宏：HY_EGO_DEBUG_LOG_CAUSAL_ADOPTION。影响文件：领域/新_任务类.cpp。
    // 关闭条件：因果采用排查结束。验证方式：诊断版运行读 调试_CAUSAL_ADOPTION.log。
    CAUSAL_ADOPTION,
    // 目的：因果筹办阶段监控。启用宏：HY_EGO_DEBUG_LOG_CAUSAL_PREPARATION。影响文件：领域/新_任务类.cpp。
    // 关闭条件：因果筹办排查结束。验证方式：诊断版运行读 调试_CAUSAL_PREPARATION.log。
    CAUSAL_PREPARATION,
    // 目的：因果提议差异。启用宏：HY_EGO_DEBUG_LOG_CAUSAL_PROPOSAL_DIFF。影响文件：线程/新_自我线程类.cpp。
    // 关闭条件：因果提议排查结束。验证方式：诊断版运行读 调试_CAUSAL_PROPOSAL_DIFF.log。
    CAUSAL_PROPOSAL_DIFF,
    // 目的：方法查找。启用宏：HY_EGO_DEBUG_LOG_METHOD_LOOKUP。影响文件：领域/新_方法类.cpp。
    // 关闭条件：方法查找排查结束。验证方式：诊断版运行读 调试_METHOD_LOOKUP.log。
    METHOD_LOOKUP,
    // 目的：最小闭环。启用宏：HY_EGO_DEBUG_LOG_MIN_LOOP。影响文件：核心/日志系统.h（宏 海中鱼巣记录最小闭环调试日志）。
    // 关闭条件：最小闭环排查结束。验证方式：诊断版运行读 调试_MIN_LOOP.log。
    MIN_LOOP,
    // 目的：新需求启动链。启用宏：HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP。影响文件：业务/服务需求.本能运行服务.cpp。
    // 关闭条件：新需求启动排查结束。验证方式：诊断版运行读 调试_NEW_DEMAND_STARTUP.log。
    NEW_DEMAND_STARTUP,
    // 目的：普通应用装配各步耗时。启用宏：HY_EGO_DEBUG_LOG_STARTUP_TIMING，或沿用 HY_EGO_STARTUP_TIMING 非零。
    // 影响文件：装配.普通应用.cpp。关闭条件：启动耗时排查结束。验证方式：读 调试_STARTUP_TIMING.log。
    STARTUP_TIMING,
    // 目的：任务方法召回。启用宏：HY_EGO_DEBUG_LOG_TASK_METHOD_RECALL。影响文件：线程/新_任务工作线程类.cpp。
    // 关闭条件：方法召回排查结束。验证方式：诊断版运行读 调试_TASK_METHOD_RECALL.log。
    TASK_METHOD_RECALL,
    // 目的：调度扫描结果投影。启用宏：HY_EGO_DEBUG_LOG_TASK_SCHEDULING。影响文件：线程/新_任务管理线程类.cpp。
    // 关闭条件：调度排查结束。验证方式：诊断版运行读 调试_TASK_SCHEDULING.log。
    TASK_SCHEDULING,
    // 目的：任务状态前提。启用宏：HY_EGO_DEBUG_LOG_TASK_STATE_PREMISE。影响文件：领域/新_任务类.cpp。
    // 关闭条件：状态前提排查结束。验证方式：诊断版运行读 调试_TASK_STATE_PREMISE.log。
    TASK_STATE_PREMISE,
    // 目的：任务工作线程每项工作起止与耗时。启用宏：HY_EGO_DEBUG_LOG_WORKER_EXECUTION。
    // 影响文件：线程/新_任务工作线程类.cpp。关闭条件：工作耗时排查结束。验证方式：诊断版运行读 调试_WORKER_EXECUTION.log。
    WORKER_EXECUTION,
};

template <调试切片 切片> struct 调试切片信息;

#define HY_调试切片宽字面(文字) L##文字
#define HY_调试切片宽名(名) HY_调试切片宽字面(#名)
#define HY_调试切片登记(名, 开启值) \
    template <> struct 调试切片信息<调试切片::名> { \
        static constexpr std::wstring_view 名称 = HY_调试切片宽名(名); \
        static constexpr bool 开启 = (开启值); \
    }

#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
HY_调试切片登记(PARAM_BINDING, true);
#else
HY_调试切片登记(PARAM_BINDING, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING_DETAIL
HY_调试切片登记(PARAM_BINDING_DETAIL, true);
#else
HY_调试切片登记(PARAM_BINDING_DETAIL, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
HY_调试切片登记(SIMULATION_PROGRESS, true);
#else
HY_调试切片登记(SIMULATION_PROGRESS, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_SCHED
HY_调试切片登记(RECEIVED_DEMAND_SCHED, true);
#else
HY_调试切片登记(RECEIVED_DEMAND_SCHED, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
HY_调试切片登记(RECEIVED_DEMAND_PARSE, true);
#else
HY_调试切片登记(RECEIVED_DEMAND_PARSE, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_MAINLINE_PHASE
HY_调试切片登记(MAINLINE_PHASE, true);
#else
HY_调试切片登记(MAINLINE_PHASE, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_LANGUAGE_INPUT_PHASE
HY_调试切片登记(LANGUAGE_INPUT_PHASE, true);
#else
HY_调试切片登记(LANGUAGE_INPUT_PHASE, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_CAUSAL_ADOPTION
HY_调试切片登记(CAUSAL_ADOPTION, true);
#else
HY_调试切片登记(CAUSAL_ADOPTION, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_CAUSAL_PREPARATION
HY_调试切片登记(CAUSAL_PREPARATION, true);
#else
HY_调试切片登记(CAUSAL_PREPARATION, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_CAUSAL_PROPOSAL_DIFF
HY_调试切片登记(CAUSAL_PROPOSAL_DIFF, true);
#else
HY_调试切片登记(CAUSAL_PROPOSAL_DIFF, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
HY_调试切片登记(METHOD_LOOKUP, true);
#else
HY_调试切片登记(METHOD_LOOKUP, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_MIN_LOOP
HY_调试切片登记(MIN_LOOP, true);
#else
HY_调试切片登记(MIN_LOOP, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
HY_调试切片登记(NEW_DEMAND_STARTUP, true);
#else
HY_调试切片登记(NEW_DEMAND_STARTUP, false);
#endif
#if defined(HY_EGO_DEBUG_LOG_STARTUP_TIMING) || (defined(HY_EGO_STARTUP_TIMING) && HY_EGO_STARTUP_TIMING)
HY_调试切片登记(STARTUP_TIMING, true);
#else
HY_调试切片登记(STARTUP_TIMING, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_TASK_METHOD_RECALL
HY_调试切片登记(TASK_METHOD_RECALL, true);
#else
HY_调试切片登记(TASK_METHOD_RECALL, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_TASK_SCHEDULING
HY_调试切片登记(TASK_SCHEDULING, true);
#else
HY_调试切片登记(TASK_SCHEDULING, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_TASK_STATE_PREMISE
HY_调试切片登记(TASK_STATE_PREMISE, true);
#else
HY_调试切片登记(TASK_STATE_PREMISE, false);
#endif
#ifdef HY_EGO_DEBUG_LOG_WORKER_EXECUTION
HY_调试切片登记(WORKER_EXECUTION, true);
#else
HY_调试切片登记(WORKER_EXECUTION, false);
#endif

#undef HY_调试切片登记
#undef HY_调试切片宽名
#undef HY_调试切片宽字面

} // namespace 海中鱼巣
