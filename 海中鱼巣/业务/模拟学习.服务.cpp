#include "模拟学习.服务.h"
#include "模拟学习.算法.h"
#include "模拟任务需求来源.服务.h"
#include "../领域/新_模拟世界类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/概念_特征类.h"
#include "../领域/概念_存在类.h"
#include "../领域/新_存在类.h"
#include "../领域/动作概念.只读投影合同.h"
#include "../领域/概念_动态类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <memory>
#include <map>
#include <set>
#include <new>
#include <thread>
#include <tuple>
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
#include "../核心/日志系统.h"
#include <atomic>
#endif

namespace 海中鱼巣 {
extern thread_local int 模拟样例拒绝行;
namespace { struct 模拟输入采集漂移 final {}; }
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
namespace {
std::atomic<std::uint64_t> 模拟进度日志条数{0};
struct 模拟学习进度观察 final {
    const 新方法信息& 方法;
    std::optional<稳定编码> 分支;
    std::optional<std::uint32_t> 消息方案;
    std::uint64_t 采集次数=0, 样例次数=0, 外层漂移=0, 来源漂移=0;
    const wchar_t* 当前阶段=L"入口";
    void 记录(const wchar_t* 阶段, std::uint64_t 规模=0) noexcept {
        当前阶段=阶段;
        try {
            const auto 行=模拟进度日志条数.fetch_add(1, std::memory_order_relaxed);
            if(行>8192)return;
            if(行==8192) {
                (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"模拟学习",L"输出截断",
                    L"本进程已达到8192条详细诊断；仅停止日志，不限制学习或组合。");
                return;
            }
            std::wstring 登记名=L"未提供";
            if(方法.本能函数登记名) {
                const auto& 名=*方法.本能函数登记名;
                if(名.size()<=4096) {
                    const auto 长=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,名.data(),static_cast<int>(名.size()),nullptr,0);
                    if(长>0) {
                        登记名.resize(static_cast<std::size_t>(长));
                        if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,名.data(),static_cast<int>(名.size()),登记名.data(),长))登记名=L"转换失败";
                    }
                } else 登记名=L"登记名过长，省略";
            }
            std::wostringstream 文本;
            文本<<L"方法="<<方法.节点.值<<L"；登记名="<<登记名
                <<L"；线程="<<GetCurrentThreadId()<<L"；分支="<<(分支 ? 分支->值 : 0)
                <<L"；消息方案="<<(消息方案 ? std::to_wstring(*消息方案) : L"无")
                <<L"；采集="<<采集次数<<L"；样例="<<样例次数
                <<L"；外层漂移="<<外层漂移<<L"；来源漂移="<<来源漂移
                <<L"；规模或状态="<<规模;
            (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"模拟学习",阶段,文本.str());
        } catch(...) {} // 诊断失败不影响原返回、异常、采样或调用。
    }
};
}
#define HY_SIM_PROGRESS(阶段, 规模) 观察.记录(阶段, static_cast<std::uint64_t>(规模))
#else
#define HY_SIM_PROGRESS(阶段, 规模) ((void)0)
#endif
模拟学习服务::模拟学习服务(新_方法类& 方法, 新_特征类& 特征,概念_特征类& 概念,概念_二次特征类& 二次,
    const 模拟任务需求来源读取接口& 来源) noexcept
    : 方法_(方法), 概念_(概念), 二次概念_(二次),二次计算_(特征,二次),业务来源_(来源) {}
void 模拟学习服务::配置场景事实构成读取(const 新方法场景事实构成读取接口& 提供者) {
    if (场景构成_&&场景构成_!=&提供者) throw std::logic_error("scene construction provider conflict");
    场景构成_=&提供者;
}
void 模拟学习服务::配置隔离动作定义(const 动作方法定义只读提供者& 提供者,新_动态类& 定义,const 概念_动态类& 概念) {
    if ((动作定义_&&动作定义_!=&提供者)||(动态定义_&&动态定义_!=&定义)||(动态概念_&&动态概念_!=&概念))
        throw std::logic_error("isolated action definition provider conflict");
    动作定义_=&提供者;动态定义_=&定义;动态概念_=&概念;
}
void 模拟学习服务::配置需求执行来源(稳定编码 自我存在,const 新_存在类& 来源,const 概念_存在类& 存在) {
    if(!有效(自我存在)||(有效(需求执行来源_)&&需求执行来源_!=自我存在)
        ||(来源身份_&&来源身份_!=&来源)||(存在概念_&&存在概念_!=&存在))
        throw std::logic_error("simulation demand actor source conflict");
    需求执行来源_=自我存在;来源身份_=&来源;存在概念_=&存在;
}
void 模拟学习服务::配置字段询问语言(std::shared_ptr<const 新字段询问语言接口> 语言,
    std::optional<新字段询问登记配置> 配置) {
    if(static_cast<bool>(语言)!=配置.has_value())
        throw std::invalid_argument("simulation question dependencies must be paired");
    std::lock_guard 锁(字段询问装配锁_);
    if(活动学习数_)throw std::logic_error("simulation question dependencies are in use");
    if(语言&&字段询问语言_&&(语言!=字段询问语言_||配置!=字段询问配置_))
        throw std::logic_error("simulation question dependency conflict");
    字段询问语言_=std::move(语言);字段询问配置_=std::move(配置);
}

新学习运行异常::新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因) noexcept
    : 记录_{方法节点,原因} {}
新学习运行异常::新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因, std::string 诊断)
    : 记录_{方法节点, 原因, std::move(诊断)} {}
const 新学习运行异常记录& 新学习运行异常::记录() const noexcept { return 记录_; }
const char* 新学习运行异常::what() const noexcept {
    if (!记录_.诊断.empty()) return 记录_.诊断.c_str();
    switch (记录_.原因) {
    case 新学习运行异常原因::规格未支持: return "unsupported simulation specification";
    case 新学习运行异常原因::停止取消: return "learning operation cancelled";
    case 新学习运行异常原因::输入不合法: return "invalid learning input";
    case 新学习运行异常原因::资源不足: return "learning resource exhausted";
    case 新学习运行异常原因::结构不一致: return "learning structure inconsistent";
    case 新学习运行异常原因::本能运行异常: return "instinct execution abnormal";
    case 新学习运行异常原因::发布结果未知: return "learning publication indeterminate";
    default: return "learning internal error";
    }
}

新学习结束通知 模拟学习服务::学习(稳定编码 方法节点, const std::function<bool()>& 停止检查) {
    using 原因 = 新学习运行异常原因;
    try {
        struct 学习装配保护 final {
            std::mutex& 锁;
            std::size_t& 活动数;
            学习装配保护(std::mutex& m,std::size_t& n) : 锁(m),活动数(n) {
                std::lock_guard guard(锁);++活动数;
            }
            ~学习装配保护() { std::lock_guard guard(锁);--活动数; }
        } 装配保护{字段询问装配锁_,活动学习数_};
        if (停止检查 && 停止检查())
            throw 新学习运行异常(方法节点, 原因::停止取消);
        if (!有效(方法节点))
            throw 新学习运行异常(方法节点, 原因::输入不合法);
        const auto 协作停止 = [&] {
            return 停止检查 && 停止检查();
        };
        if (协作停止()) throw 新学习运行异常(方法节点, 原因::停止取消);
        const auto 方法 = 方法_.获取方法(方法节点);
        if (!方法)
            throw 新学习运行异常(方法节点, 原因::结构不一致);
        if (方法->来源方法节点 || 方法->实例根节点
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用)
            throw 新学习运行异常(方法节点, 原因::输入不合法);
        if (!方法->本能函数登记名 || 方法->本能函数登记名->empty())
            throw 新学习运行异常(方法节点, 原因::结构不一致);
        for (std::size_t i = 0; i < 方法->参数规格组.size(); ++i) {
            if (!方法->参数规格组[i].有序规格)
                throw 新学习运行异常(方法节点, 原因::规格未支持,
                    "simulation parameter " + std::to_string(i + 1)
                    + ": legacy role specification requires migration to ordered parameters");
        }
        // 只有通用模拟流程实际返回，才能交付正常学习结束通知。
        学习有序本能(*方法, 协作停止);
        if (协作停止()) throw 新学习运行异常(方法节点, 原因::停止取消);
        return {方法节点};
    } catch (const 新学习运行异常&) {
        throw;
    } catch (const std::bad_alloc&) {
        throw 新学习运行异常(方法节点, 原因::资源不足);
    } catch (const std::exception& e) {
        throw 新学习运行异常(方法节点, 原因::内部错误, e.what());
    } catch (...) {
        throw 新学习运行异常(方法节点, 原因::内部错误);
    }
}
} // namespace 海中鱼巣

namespace 海中鱼巣 {

void 模拟学习服务::学习有序本能(const 新方法信息& 原方法,const std::function<bool()>& 停止检查,
    const std::map<std::uint32_t,稳定编码>& 原型选择,std::optional<稳定编码> 执行分支,bool 已选路径,
    std::optional<std::uint32_t> 消息方案) {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
    模拟学习进度观察 观察{原方法,执行分支,消息方案};
#endif
    HY_SIM_PROGRESS(L"进入有序学习路径",原方法.参数规格组.size());
    if(!已选路径) {
        if(原方法.执行分支组.empty()) {
            学习有序本能(原方法,停止检查,原型选择,{},true);
            return;
        }
        std::exception_ptr 未完成;
        for(const auto& 分支:原方法.执行分支组) {
            try { 学习有序本能(原方法,停止检查,原型选择,分支.节点,true); }
            catch(const 新学习运行异常& e) {
                if(e.记录().原因!=新学习运行异常原因::规格未支持
                    &&e.记录().原因!=新学习运行异常原因::输入不合法)throw;
                if(!未完成)未完成=std::current_exception();
            }
        }
        if(未完成)std::rethrow_exception(未完成);
        return;
    }
    if(!消息方案&&!原方法.初始模拟输入方案组.empty()) {
        std::exception_ptr 未完成;
        for(const auto& plan:原方法.初始模拟输入方案组) {
            try {学习有序本能(原方法,停止检查,原型选择,执行分支,true,plan.顺序);}
            catch(const 新学习运行异常& e) {
                if(e.记录().原因!=新学习运行异常原因::规格未支持&&e.记录().原因!=新学习运行异常原因::输入不合法)throw;
                if(!未完成)未完成=std::current_exception();
            }
        }
        if(未完成)std::rethrow_exception(未完成);return;
    }
    for(;;)try {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
    ++观察.采集次数;
#endif
    HY_SIM_PROGRESS(L"开始采集定义及原型",原型选择.size());
    auto m=原方法;
    {
        const auto 有效声明=方法_.读取执行分支必要条件(m.节点,执行分支);
        if(有效声明.状态==新方法执行分支读取状态::前提已变化)
            throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"execution branch projection premise changed");
        if(有效声明.状态!=新方法执行分支读取状态::已读取)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"selected execution branch unavailable");
        m.初始必要条件=有效声明.有效必要条件;
    }
    const auto 停止=[&] { if(停止检查&&停止检查())throw 新学习运行异常(m.节点,新学习运行异常原因::停止取消); };
    const auto 不支持 = [&](std::uint32_t 位置, const std::string& 原因) -> void {
        throw 新学习运行异常(m.节点, 新学习运行异常原因::规格未支持,
            "simulation parameter " + std::to_string(位置) + ": " + 原因);
    };
    std::optional<稳定编码> 需求结果概念;
    for(const auto 概念:m.方法头.可能结果概念组) {
        if(方法_.读取模拟特征范围(概念))continue;
        const auto 结构=方法_.读取模拟正式结构(概念);
        if(结构.状态==正式结构操作状态::前提已变化)throw 模拟输入采集漂移{};
        if(!结构.信息||!std::holds_alternative<信息结构概念定义>(结构.信息->定义))
            不支持(0,"result is neither a feature nor a supported information structure");
        if(!存在概念_||!来源身份_||!有效(需求执行来源_))
            不支持(0,"formal demand execution actor source is not configured");
        const auto 定义=存在概念_->读取引用内容筹办概念();
        if(定义.状态==正式结构操作状态::前提已变化)throw 模拟输入采集漂移{};
        if(定义.状态!=正式结构操作状态::已读取||!定义.信息||定义.信息->完整被认知需求!=概念||需求结果概念)
            不支持(0,"information result is not the supported complete recognized demand");
        需求结果概念=概念;
    }
    const auto 核内容采样形状=[&](const 新方法条件项定义& c) {
        if(c.引用内容条件) {
            const auto& x=*c.引用内容条件;
            if(!需求结果概念||x.作用点!=新方法内容作用点::参数自身||!x.正式内容路径.empty()
                ||x.要求概念节点!=*需求结果概念)
                不支持(c.参数序号,"content sampling requires a complete formal demand parameter");
        }
        if(c.特征前态限定&&(c.特征前态限定->当前值状态!=新方法当前值状态::未知
            ||c.特征前态限定->历史不同值数量!=std::optional<std::uint64_t>{0}))
            不支持(c.参数序号,"only an unknown current value with empty history is sampled");
    };
    if(m.初始必要条件)for(const auto& c:m.初始必要条件->条件.条件项组)核内容采样形状(c);
    for(const auto& pair:m.条件结果组)for(const auto& c:pair.条件.条件项组)核内容采样形状(c);
    std::vector<新模拟有序参数输入> base;
    std::vector<std::optional<新特征匹配定义快照>> defs;
    std::set<std::size_t> 需求参数位置;
    for(const auto& spec:m.参数规格组) {
        if(!spec.有序规格) 不支持(0,"mixed ordered and role parameters");
        if(spec.有序规格->信息类型==新方法参数信息类型::存在) {
            const auto formal=方法_.读取模拟正式结构(spec.有序规格->范围概念节点);
            if(formal.状态==正式结构操作状态::前提已变化)throw 模拟输入采集漂移{};
            if(formal.状态==正式结构操作状态::已读取&&formal.信息
                &&std::holds_alternative<信息结构概念定义>(formal.信息->定义)) {
                const auto roots=方法_.读取模拟需求概念组();
                if(roots.状态==正式结构操作状态::前提已变化)throw 模拟输入采集漂移{};
                if(roots.状态!=正式结构操作状态::已读取||!roots.信息)
                    不支持(spec.有序规格->序号,"formal demand root unavailable");
                if(spec.有序规格->范围概念节点!=roots.信息->需求)
                    不支持(spec.有序规格->序号,"only the formal demand root is supported as an information parameter");
                需求参数位置.insert(base.size());
            }
        }
        base.push_back({*spec.有序规格});
        defs.push_back(spec.有序规格->信息类型==新方法参数信息类型::特征
            ?方法_.读取模拟特征范围(spec.有序规格->范围概念节点):std::nullopt);
        if(spec.有序规格->信息类型==新方法参数信息类型::特征&&!defs.back())
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "parameter feature range unavailable: position=" + std::to_string(spec.有序规格->序号)
                + " concept=" + std::to_string(spec.有序规格->范围概念节点.值));
    }
    const auto N=base.size();
    const auto 是需求参数=[&](std::size_t i){return 需求参数位置.contains(i);};
    const auto 是普通存在=[&](std::size_t i){return base[i].规格.信息类型==新方法参数信息类型::存在&&!是需求参数(i);};
    std::optional<稳定编码> 需求主体FT;
    if(!需求参数位置.empty()) {
        const auto slot=概念_.读取先天特征类型槽(先天特征类型槽::需求主体);
        if(slot.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
        if(slot.状态==特征概念规则状态::前提已变化)throw 模拟输入采集漂移{};
        if(!slot.类型根)不支持(0,"formal demand subject feature type unavailable");
        需求主体FT=slot.类型根;
    }
    auto 初始关系组=m.初始模拟关系组;
    std::vector<std::pair<std::size_t,稳定编码>> 初始存在条件;
    std::vector<新方法条件项定义> 初始数值条件;
    std::vector<新方法条件项定义> 初始附属条件;
    if(m.初始必要条件) {
        const auto& 必要=*m.初始必要条件;
        if(必要.条件.关系规则!=新方法项关系规则::全部成立)
            不支持(0,"初始必要条件不是完整合取声明");
        for(const auto& 条件:必要.条件.条件项组) {
            if(条件.引用内容条件) {
                if(!条件.参数序号||条件.参数序号>N||!是需求参数(条件.参数序号-1))
                    不支持(条件.参数序号,"content predicate is not bound to a formal demand parameter");
                continue; // 原函数在同一本地D上实际求值完整内容，不从声明预造成功。
            }
            if(条件.特征前态限定) {
                if(!条件.参数序号||条件.参数序号>N||!defs[条件.参数序号-1]
                    ||defs[条件.参数序号-1]->材料类型!=特征概念材料物理类型::需求节点引用)
                    不支持(条件.参数序号,"first-value predicate is not a demand reference feature");
                continue;
            }
            if(条件.参数序号&&条件.参数序号<=N&&是需求参数(条件.参数序号-1)) {
                新方法条件项定义 预期;预期.参数序号=条件.参数序号;
                预期.存在概念节点=base[条件.参数序号-1].规格.范围概念节点;
                if(条件!=预期)不支持(条件.参数序号,"formal demand attached/content conditions are not supported");
                continue;
            }
            if(条件.参数序号&&条件.参数序号<=N&&!defs[条件.参数序号-1]
                &&条件.特征类型概念节点) {
                auto shape=条件;shape.特征类型概念节点.reset();shape.特征值域概念节点.reset();shape.精确数值域.reset();
                新方法条件项定义 expected;expected.参数序号=条件.参数序号;expected.存在概念节点=条件.存在概念节点;
                if(shape!=expected)不支持(条件.参数序号,"unsupported attached feature condition shape");
                初始附属条件.push_back(条件);
                if(条件.存在概念节点)初始存在条件.push_back({条件.参数序号-1,*条件.存在概念节点});
                continue;
            }
            if(条件.参数序号&&条件.参数序号<=N&&defs[条件.参数序号-1]
                &&defs[条件.参数序号-1]->材料类型==特征概念材料物理类型::I64标量) {
                auto 形状=条件;
                形状.特征类型概念节点.reset();形状.特征值域概念节点.reset();形状.精确数值域.reset();
                新方法条件项定义 预期;预期.参数序号=条件.参数序号;
                if(形状!=预期||!条件.特征类型概念节点
                    ||*条件.特征类型概念节点!=defs[条件.参数序号-1]->特征类型根
                    ||(!条件.特征值域概念节点&&!条件.精确数值域))
                    不支持(条件.参数序号,"unsupported numeric branch condition shape");
                初始数值条件.push_back(条件);continue;
            }
            // 存在引用特征参数的值域条件（如“自我.交互者有值”）：模拟只记录条件实参、不构造值存在；
            // 值域作为分支必要条件由筹办按声明核验（2026-11-10 获取服务对象以交互者为条件）。
            if(条件.参数序号&&条件.参数序号<=N&&defs[条件.参数序号-1]
                &&defs[条件.参数序号-1]->材料类型==特征概念材料物理类型::存在节点引用) {
                auto 形状=条件;形状.特征类型概念节点.reset();形状.特征值域概念节点.reset();
                新方法条件项定义 预期;预期.参数序号=条件.参数序号;
                if(形状!=预期||!条件.特征类型概念节点||!条件.特征值域概念节点
                    ||*条件.特征类型概念节点!=defs[条件.参数序号-1]->特征类型根)
                    不支持(条件.参数序号,"unsupported existence-reference value condition shape");
                continue;
            }
            新方法条件项定义 可构造条件;
            可构造条件.参数序号=条件.参数序号;可构造条件.存在概念节点=条件.存在概念节点;
            if(!条件.参数序号||条件.参数序号>N||!条件.存在概念节点||条件!=可构造条件
                ||base[条件.参数序号-1].规格.信息类型!=新方法参数信息类型::存在)
                不支持(条件.参数序号,"初始条件尚无通用存在范围构造能力");
            初始存在条件.push_back({条件.参数序号-1,*条件.存在概念节点});
        }
        for(const auto& 关系:必要.条件.参数关系组)
            if(std::find(初始关系组.begin(),初始关系组.end(),关系)==初始关系组.end())初始关系组.push_back(关系);
    }
    const auto 有效存在概念=[&](std::size_t i) {
        return base.at(i).构造存在概念.value_or(base.at(i).规格.范围概念节点);
    };
    struct 存在归属读取依据 final {
        存在范围语义信息 概念;
        基础只读预期 只读预期;
    };
    std::map<稳定编码,存在归属读取依据> 存在归属保护;
    const auto 归属根相同=[](const 基础联合节点快照& a,const 基础联合节点快照& b) {
        return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组
            &&a.入向字段组==b.入向字段组&&a.入向外部关系组==b.入向外部关系组
            &&a.出向外部关系组==b.出向外部关系组;
    };
    const auto 读取归属语义=[&](稳定编码 C) {
        auto read=方法_.读取模拟存在范围语义保护(C);
        if(read.状态==正式概念读取状态::前提已变化)throw 模拟输入采集漂移{};
        if(read.状态!=正式概念读取状态::已读取||!read.信息)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"existence membership semantic definition unavailable");
        return read;
    };
    const auto 采集归属根=[&](稳定编码 C)->const 存在归属读取依据& {
        if(const auto it=存在归属保护.find(C);it!=存在归属保护.end())return it->second;
        auto read=读取归属语义(C);
        return 存在归属保护.emplace(C,存在归属读取依据{*read.信息,std::move(read.只读预期)}).first->second;
    };
    const auto 范围包含 = [&](稳定编码 child, 稳定编码 parent) {
        std::vector<稳定编码> active, done;
        std::function<bool(稳定编码)> visit = [&](稳定编码 c) {
            if(std::find(active.begin(),active.end(),c)!=active.end())
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"cyclic existence classification");
            if(std::find(done.begin(),done.end(),c)!=done.end())return false;
            const auto& captured=采集归属根(c);
            const auto& info=captured.概念;
            // owner守实际分类边及端点，不沿组织边/无关下位扩大读取范围。
            active.push_back(c);bool found=c==parent;
            for(const auto p:info.上位概念组)found=visit(p)||found;
            active.pop_back();done.push_back(c);return found;
        };
        return visit(child);
    };
    // 同一F的参数限定和存在原型均取正式值域交集，不丢弃任一限定。
    const auto 收窄特征范围 = [&](std::size_t k,const 新特征匹配定义快照& d) {
        if(defs[k]->特征类型根!=d.特征类型根||defs[k]->材料类型!=d.材料类型
            ||defs[k]->比较规则!=d.比较规则)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"aliased feature definitions disagree");
        if(std::holds_alternative<特征概念全材料值域>(defs[k]->值域)) {
                defs[k]->值域=d.值域;
            } else if(std::holds_alternative<特征概念全材料值域>(d.值域)) {
                // 另一侧为全集，保留本侧已有约束。
            } else if(const auto* a=std::get_if<std::vector<特征概念I64闭区间>>(&defs[k]->值域)) {
                const auto* b=std::get_if<std::vector<特征概念I64闭区间>>(&d.值域);
                if(!b) 不支持(static_cast<std::uint32_t>(k+1),"prototype range intersection shape");
                std::vector<特征概念I64闭区间> intersection;
                for(const auto& x:*a)for(const auto& y:*b) {
                    const auto l=(std::max)(x.下界,y.下界),h=(std::min)(x.上界,y.上界);
                    if(l<=h)intersection.push_back({l,h});
                }
                defs[k]->值域=std::move(intersection);
            } else if (auto* a = std::get_if<特征概念结构化I64值域>(&defs[k]->值域)) {
                const auto* b = std::get_if<特征概念结构化I64值域>(&d.值域);
                if (!b || a->分量.size() != b->分量.size())
                    不支持(static_cast<std::uint32_t>(k+1),"prototype structured range shape");
                for (std::size_t n=0;n<a->分量.size();++n) {
                    auto& 左=a->分量[n]; const auto& 右=b->分量[n];
                    if (左.角色!=右.角色 || 左.单位!=右.单位)
                        不支持(static_cast<std::uint32_t>(k+1),"prototype component roles or units differ");
                    std::vector<特征概念I64闭区间> 交集;
                    for (const auto& x:左.值域) for (const auto& y:右.值域) {
                        const auto l=(std::max)(x.下界,y.下界),h=(std::min)(x.上界,y.上界);
                        if(l<=h)交集.push_back({l,h});
                    }
                    左.值域=std::move(交集);
                }
            } else if (defs[k]->值域 != d.值域) {
                const auto 比较=概念_.比较值域(defs[k]->特征类型根,defs[k]->值域,d.值域);
                if(比较.状态==特征概念规则状态::资源失败)throw 新学习运行异常(m.节点,新学习运行异常原因::资源不足);
                if(比较.状态!=特征概念规则状态::已完成||!比较.关系)
                    不支持(static_cast<std::uint32_t>(k+1),"prototype domain comparison is unsupported");
                if(*比较.关系==特征概念值域关系::左包含右)defs[k]->值域=d.值域;
                else if(*比较.关系==特征概念值域关系::相等||*比较.关系==特征概念值域关系::右包含左){}
                else if(*比较.关系==特征概念值域关系::分离)defs[k]->值域=std::vector<稳定编码>{};
                else {
                    const auto* 有限=std::get_if<std::vector<稳定编码>>(&defs[k]->值域);
                    const auto* 另一有限=std::get_if<std::vector<稳定编码>>(&d.值域);
                    if(!有限&&!另一有限)不支持(static_cast<std::uint32_t>(k+1),"nonfinite intersecting prototype domains");
                    const auto& 取样=有限 ? *有限 : *另一有限;
                    const auto& 范围=有限 ? d.值域 : defs[k]->值域;
                    std::vector<稳定编码> 交集;
                    for(const auto v:取样) {
                        const auto r=概念_.比较值域(defs[k]->特征类型根,特征概念值域{std::vector<稳定编码>{v}},范围);
                        if(r.状态!=特征概念规则状态::已完成||!r.关系)
                            不支持(static_cast<std::uint32_t>(k+1),"finite prototype element comparison");
                        if(*r.关系==特征概念值域关系::相等||*r.关系==特征概念值域关系::右包含左)交集.push_back(v);
                    }
                    defs[k]->值域=std::move(交集);
                }
            }
    };
    for(const auto& 条件:初始数值条件) {
        const auto i=条件.参数序号-1;
        if(条件.特征值域概念节点) {
            const auto d=方法_.读取模拟特征范围(*条件.特征值域概念节点);
            if(!d)不支持(条件.参数序号,"numeric branch domain unavailable");
            收窄特征范围(i,*d);
        }
        if(条件.精确数值域) {
            auto d=*defs[i];d.值域=std::vector<特征概念I64闭区间>{*条件.精确数值域};
            收窄特征范围(i,d);
        }
    }
    // 参数各自的概念范围决定取样点；合并后的范围只负责合法性约束。
    auto 原始参数定义=defs;
    std::vector<std::size_t> 代表(N);
    for(std::size_t i=0;i<N;++i)代表[i]=i;
    const auto 根=[&](std::size_t i){while(代表[i]!=i)i=代表[i];return i;};
    for(const auto& r:初始关系组) {
        if(static_cast<std::int64_t>(r.种类)<1||static_cast<std::int64_t>(r.种类)>8)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"未知初始参数关系种类");
        if(!r.左参数序号||r.左参数序号>N||!r.右参数序号||r.右参数序号>N)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"relation parameter out of range");
        if(r.种类==新方法参数关系种类::内容引用参数) {
            if(!是需求参数(r.左参数序号-1)||!是普通存在(r.右参数序号-1)
                ||!需求主体FT||r.左正式内容路径!=std::vector{*需求主体FT})
                不支持(r.左参数序号,"only a formal demand subject-to-parameter relation is supported");
        } else if(!r.左正式内容路径.empty())
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"legacy relation carries a content path");
        if(r.种类==新方法参数关系种类::同一) {
            const auto a=根(r.左参数序号-1),b=根(r.右参数序号-1);
            if(base[a].规格.信息类型!=base[b].规格.信息类型)
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"alias information type mismatch");
            if(是需求参数(a)!=是需求参数(b))
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"ordinary existence and formal demand cannot alias");
            代表[(std::max)(a,b)]=(std::min)(a,b);
        }
        if((是需求参数(r.左参数序号-1)||是需求参数(r.右参数序号-1))
            &&r.种类!=新方法参数关系种类::同一&&r.种类!=新方法参数关系种类::不同
            &&r.种类!=新方法参数关系种类::内容引用参数
            &&!(r.种类==新方法参数关系种类::特征引用存在&&是需求参数(r.右参数序号-1)))
            不支持(r.左参数序号,"formal demand relation other than same/different is not supported");
    }
    for(std::size_t i=0;i<N;++i){代表[i]=根(i);if(代表[i]!=i)base[i].同一参数=static_cast<std::uint32_t>(代表[i]+1);}
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i&&defs[i])
        收窄特征范围(代表[i],*defs[i]);
    // E别名的固定范围和额外条件共同收窄，不改任何固定spec。
    const auto 合入存在范围=[&](std::size_t i,稳定编码 要求范围) {
        const auto 当前=有效存在概念(i);
        if(范围包含(要求范围,当前))base[i].构造存在概念=要求范围;
        else if(!范围包含(当前,要求范围))不支持(static_cast<std::uint32_t>(i+1),"初始存在条件与规格的共同范围不能证明，未虚构交集");
    };
    for(std::size_t i=0;i<N;++i)if(是普通存在(i)&&代表[i]!=i)
        合入存在范围(代表[i],base[i].规格.范围概念节点);
    for(const auto& [位置,范围]:初始存在条件)合入存在范围(代表[位置],范围);
    for(std::size_t i=0;i<N;++i)if(是普通存在(i))
        base[i].构造存在概念=有效存在概念(代表[i]);
    // 正式分类没有实例轮廓时，所有合法下位专属原型逐个求值；不任选首项。
    for (const auto& [位置,范围]:初始存在条件) {
        const auto i=代表[位置];
        const auto& C=采集归属根(有效存在概念(i)).概念;
        if (C.对应存在节点) continue;
        const auto 选择=原型选择.find(static_cast<std::uint32_t>(i+1));
        if (选择==原型选择.end()) {
            const auto 原型=方法_.读取模拟存在原型候选(有效存在概念(i));
            HY_SIM_PROGRESS(L"原型候选已读回",原型.候选组.size());
            if (原型.状态!=存在概念实例候选读取状态::已完成)
                不支持(static_cast<std::uint32_t>(i+1),"formal classification prototype enumeration unavailable");
            if(原型.候选组.empty())continue; // 完整抽象C可按必需字段构造，不要求现实样例。
            // 参数来源两类并列（7165§10）：复制合法专属原型，或按C正式定义在隔离世界构造具体实例。
            // 已有原型不关闭构造路径；构造仍走原必需字段路径，依据不足时由该路径具名报告缺项。
            // 某一组合规格未支持/输入不合法不阻断其它组合，全部尝试后再如实上报首个未完成原因。
            std::exception_ptr 未完成;
            const auto 尝试=[&](稳定编码 选择值) {
                停止();
                auto 后继=原型选择;
                后继.emplace(static_cast<std::uint32_t>(i+1),选择值);
                try { 学习有序本能(原方法,停止检查,后继,执行分支,true,消息方案); }
                catch(const 新学习运行异常& e) {
                    if(e.记录().原因!=新学习运行异常原因::规格未支持
                        &&e.记录().原因!=新学习运行异常原因::输入不合法)throw;
                    if(!未完成)未完成=std::current_exception();
                }
            };
            for (const auto& 候选:原型.候选组)尝试(候选.专属概念节点);
            尝试(稳定编码{}); // 空选择值表示“按概念正式定义构造”，不是概念节点充当实例。
            if(未完成)std::rethrow_exception(未完成);
            return;
        }
        if (!有效(选择->second)) continue; // 按概念构造：保留正式范围C，由原构造路径按必需字段生成实例。
        if (!范围包含(选择->second,范围))
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"prototype no longer belongs to required concept");
        base[i].构造存在概念=选择->second;
        for(std::size_t j=0;j<N;++j)if(代表[j]==i)base[j].构造存在概念=选择->second;
    }
    // 专属概念明确指向同一来源时是身份别名，不按普通同概念归并。
    std::map<稳定编码,std::size_t> 专属来源代表;
    for(std::size_t i=0;i<N;++i)if(是普通存在(i)) {
        const auto& ec=采集归属根(有效存在概念(i)).概念;
        if(!ec.对应存在节点)continue;
        const auto [it,added]=专属来源代表.emplace(*ec.对应存在节点,根(i));
        if(!added) {const auto a=根(i),b=根(it->second);代表[(std::max)(a,b)]=(std::min)(a,b);}
    }
    for(std::size_t i=0;i<N;++i) {
        代表[i]=根(i);
        if(代表[i]!=i)base[i].同一参数=static_cast<std::uint32_t>(代表[i]+1);
    }
    // 原型逐个求值时，不同位置可能选中同一专属原型而成为同一存在；若初始关系要求两端不同
    // （不同），该原型组合不构成合法样例，只跳过本组合，不终止其它组合的求值。
    if(!原型选择.empty()) {
        for(const auto& r:初始关系组)
            if(r.种类==新方法参数关系种类::不同&&代表[r.左参数序号-1]==代表[r.右参数序号-1])return;
        // 初始场景事实的场景、主体、对象是三个不同参与者。
        if(m.初始必要条件)for(const auto& 限定:m.初始必要条件->场景事实限定组) {
            const std::uint32_t 位置组[]={限定.场景参数序号,限定.主体参数序号,限定.对象参数序号};
            for(std::size_t a=0;a<3;++a)for(std::size_t b=a+1;b<3;++b)
                if(位置组[a]&&位置组[b]&&位置组[a]<=N&&位置组[b]<=N&&代表[位置组[a]-1]==代表[位置组[b]-1])return;
        }
        // 事件特征的数值变化须发生在其所属存在所在的场景内；所属存在与样例场景重合时，
        // 隔离世界没有可承载该变化的外层场景，本组合具名报告未支持，不当内部错误终止其它组合。
        std::set<std::size_t> 场景代表;
        if(m.初始必要条件)for(const auto& 限定:m.初始必要条件->场景事实限定组)
            if(限定.场景参数序号&&限定.场景参数序号<=N)场景代表.insert(代表[限定.场景参数序号-1]);
        // R97：事件特征的所属存在由规格的所属存在位置给出。
        for(const auto pos:m.初始模拟事件参数组)if(pos&&pos<=N&&m.参数规格组[pos-1].有序规格) {
            const auto owner=m.参数规格组[pos-1].有序规格->所属存在位置;
            if(owner&&owner<=N&&场景代表.contains(代表[owner-1]))
                不支持(pos,"event feature owner coincides with the sample scene under this prototype combination");
        }
    }
    for(std::size_t i=0;i<N;++i)if(是普通存在(i)&&代表[i]!=i)合入存在范围(代表[i],有效存在概念(i));
    for(std::size_t i=0;i<N;++i)if(是普通存在(i))base[i].构造存在概念=有效存在概念(代表[i]);
    for(const auto& r:初始关系组) {
        const auto a=代表[r.左参数序号-1],b=代表[r.右参数序号-1];
        if(r.种类==新方法参数关系种类::不同&&a==b)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"conflicting same/different relations");
        if(r.种类==新方法参数关系种类::集合包含存在)
            base[a].集合成员参数组.push_back(static_cast<std::uint32_t>(b+1));
    }
    // R97：特征所属来自规格的所属存在位置（不再来自“特征所属存在”关系）。
    for(std::size_t i=0;i<N&&i<m.参数规格组.size();++i) {
        const auto& 规格=m.参数规格组[i].有序规格;
        if(!规格||!规格->所属存在位置||规格->所属存在位置>N)continue;
        auto& p=base[代表[i]]; const auto owner=static_cast<std::uint32_t>(代表[规格->所属存在位置-1]+1);
        if(p.所属存在参数&&p.所属存在参数!=owner)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"conflicting feature owners");
        p.所属存在参数=owner;
    }
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i) {
        base[i].所属存在参数=base[代表[i]].所属存在参数;
        base[i].集合成员参数组=base[代表[i]].集合成员参数组;
    }
    std::vector<新模拟场景事实输入> 场景输入;
    // 正式关系提供完整输入构成，不读取现实连接实例或按方法名分派。
    if(m.初始必要条件)for(const auto& 限定:m.初始必要条件->场景事实限定组) {
        const auto 定位诊断=" concept="+std::to_string(限定.关系概念节点.值)+" S/A/B/F="
            +std::to_string(限定.场景参数序号)+"/"+std::to_string(限定.主体参数序号)+"/"
            +std::to_string(限定.对象参数序号)+"/"+std::to_string(限定.特征参数序号);
        const auto 保护=方法_.读取初始场景事实概念保护(限定.关系概念节点);
        if(保护.状态==正式结构操作状态::前提已变化)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"初始场景事实正式定义在读取期间变化");
        if(保护.状态!=正式结构操作状态::已读取||!保护.信息)
            throw 新学习运行异常(m.节点,新学习运行异常原因::规格未支持,
                "未知初始场景事实关系未取得完整正式概念证明"+定位诊断+"，缺少隔离输入提供者");
        const auto* 定义=std::get_if<基础范围概念定义>(&保护.信息->定义);
        const std::uint32_t 位置组[]={限定.场景参数序号,限定.主体参数序号,限定.对象参数序号,限定.特征参数序号};
        for(std::size_t i=0;i<4;++i)if(!位置组[i]||位置组[i]>N
            ||base[位置组[i]-1].规格.信息类型!=(i == 3 ? 新方法参数信息类型::特征 : 新方法参数信息类型::存在))
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"初始场景事实参数位置或类型损坏");
        for(std::size_t i=0;i<3;++i)if(是需求参数(位置组[i]-1))
            不支持(位置组[i],"formal demand is not an ordinary scene-fact participant");
        const bool 已确认连接=定义&&定义->原语==基础概念原语::具名领域身份
            &&定义->整数参数==std::vector<std::int64_t>{14}&&定义->引用参数.empty();
        if (!已确认连接||!场景构成_) 不支持(0,"formal scene relation construction provider unavailable");
        const auto 构成=场景构成_->读取场景事实构成(限定.关系概念节点);
        if (构成.状态==新交互求值状态::前提变化) throw 模拟输入采集漂移{}; // 并发前提变化：沿既有采集重读
        if (构成.状态!=新交互求值状态::已取得||!构成.构成||构成.完整保护.empty()
            ||构成.构成->关系概念!=限定.关系概念节点)
            不支持(0,"complete formal scene relation construction unavailable");
        场景输入.push_back({限定,*构成.构成});
        auto& F=base[代表[限定.特征参数序号-1]];
        F.历史引用存在参数组.push_back(static_cast<std::uint32_t>(代表[限定.对象参数序号-1]+1));
    }
    std::map<稳定编码,新存在信息> 原型存在快照;
    std::map<稳定编码,新特征匹配实例快照> 原型特征快照;
    std::map<std::size_t,稳定编码> 固定来源特征;
    const bool 需要需求构造=!需求参数位置.empty()||std::any_of(defs.begin(),defs.end(),[](const auto& d) {
        return d&&d->材料类型==特征概念材料物理类型::需求节点引用;
    });
    std::optional<新模拟需求来源结果<新模拟需求构造依据>> 需求构造依据;
    std::string 需求构造缺口;
    if(需要需求构造) {
        需求构造依据=业务来源_.读取模拟构造依据();
        if(需求构造依据->状态==新模拟需求状态::前提已变化)throw 模拟输入采集漂移{};
        if(需求构造依据->状态==新模拟需求状态::资源失败)throw std::bad_alloc{};
        if(需求构造依据->状态!=新模拟需求状态::已完成||!需求构造依据->信息) {
            需求构造缺口="demand construction human concept provider unavailable";需求构造依据.reset();
        }
    }
    if(需求构造依据) {
        const auto human=需求构造依据->信息->人类概念;
        if(std::none_of(base.begin(),base.end(),[&](const auto& p) {
            const auto i=static_cast<std::size_t>(&p-base.data());
            return 是普通存在(i)&&范围包含(p.构造存在概念.value_or(p.规格.范围概念节点),human);
        })) {base.push_back({{0,新方法参数信息类型::存在,human}});defs.push_back(std::nullopt);}
    }
    // 存在范围中的正式特征成为原型的一部分，不凭位置邻接给参数添关系。
    for(std::size_t i=0;i<base.size();++i) {
        if(defs[i])if(const auto* 引用=std::get_if<特征概念存在引用值域>(&defs[i]->值域)) {
            if(std::none_of(base.begin(),base.end(),[&](const auto& p) {
                const auto i=static_cast<std::size_t>(&p-base.data());
                return 是普通存在(i)&&p.构造存在概念.value_or(p.规格.范围概念节点)==引用->存在概念节点;
            })) {
                // 类型范围缺少原型时追加私有存在；已有同范围原型复用，递归引用由此闭环。
                base.push_back({{0,新方法参数信息类型::存在,引用->存在概念节点}});
                defs.push_back(std::nullopt);
            }
        }
        if(!是普通存在(i))continue;
        const auto owner=static_cast<std::uint32_t>((i<N ? 代表[i] : i)+1);
        const auto& ec=采集归属根(有效存在概念(i)).概念;
        for(const auto& required:ec.特征组) {
            const auto d=方法_.读取模拟特征范围(required.具体特征概念);
            if(!d)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "existence required feature range unavailable: concept=" + std::to_string(required.具体特征概念.值));
            auto k=base.size();
            for(std::size_t j=0;j<base.size();++j)
                if(base[j].所属存在参数==owner&&defs[j]&&defs[j]->特征类型根==d->特征类型根) {k=j;break;}
            if(k==base.size()) {
                base.push_back({{0,新方法参数信息类型::特征,required.具体特征概念},owner});
                defs.push_back(d);
                // 正式专属原型的非参数上下文沿当前实际材料固定，不额外扩成数值试验轴。
                if(ec.对应存在节点) {
                    const auto E=方法_.读取模拟来源存在(*ec.对应存在节点);
                    if(!E) throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"dedicated prototype source unreadable");
                    const auto [旧E,新增E]=原型存在快照.emplace(*ec.对应存在节点,*E);
                    if(!新增E&&旧E->second!=*E)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"dedicated prototype existence changed during capture");
                    for(const auto F:E->特征节点组) {
                        const auto 实际=方法_.读取模拟来源特征(F);
                        if(!实际||实际->定义.特征类型根!=d->特征类型根) continue;
                        const auto [旧F,新增F]=原型特征快照.emplace(F,*实际);
                        if(!新增F&&旧F->second!=*实际)
                            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"dedicated prototype feature changed during capture");
                        固定来源特征.emplace(k,F);base[k].复制上下文=true;
                        break;
                    }
                }
            } else { 收窄特征范围(k,*d); }
        }
    }
    for(const auto& c:初始附属条件) {
        const auto owner=static_cast<std::uint32_t>(代表[c.参数序号-1]+1);
        const auto 授权=方法_.读取存在参数特征声明(m.节点,执行分支,c.参数序号,*c.特征类型概念节点);
        if(授权.状态==新方法参数特征读取状态::定义前提已变化)
            throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"attached feature declaration changed");
        if(授权.状态!=新方法参数特征读取状态::已读取||!授权.特征定义)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"attached feature condition is not formally declared");
        auto k=base.size();
        for(std::size_t j=0;j<base.size();++j)
            if(base[j].所属存在参数==owner&&defs[j]&&defs[j]->特征类型根==*c.特征类型概念节点){k=j;break;}
        if(k==base.size()) {
            base.push_back({{0,新方法参数信息类型::特征,*c.特征类型概念节点},owner});
            defs.push_back(*授权.特征定义);
        }
        if(c.特征值域概念节点) {
            const auto d=方法_.读取模拟特征范围(*c.特征值域概念节点);
            if(!d)不支持(c.参数序号,"attached condition range unavailable");
            收窄特征范围(k,*d);
        }
        if(c.精确数值域) {
            if(defs[k]->材料类型!=特征概念材料物理类型::I64标量)不支持(c.参数序号,"attached precise range is not scalar integer");
            auto d=*defs[k];d.值域=std::vector<特征概念I64闭区间>{*c.精确数值域};收窄特征范围(k,d);
        }
        if(k>=N) {base[k].复制上下文=false;base[k].值.reset();base[k].材料.reset();固定来源特征.erase(k);}
    }
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i&&defs[i])defs[i]=defs[代表[i]];
    for(const auto& [E,快照]:原型存在快照)
        if(方法_.读取模拟来源存在(E)!=快照)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"dedicated prototype existence capture changed");
    for(const auto& [F,快照]:原型特征快照)
        if(方法_.读取模拟来源特征(F)!=快照)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"dedicated prototype feature capture changed");
    const auto 数值候选 = [&](std::size_t 位置, std::optional<std::size_t> 分量) {
        const auto 读取区间 = [&](const 新特征匹配定义快照& 定义)
            -> const std::vector<特征概念I64闭区间>* {
            if (!分量) return std::get_if<std::vector<特征概念I64闭区间>>(&定义.值域);
            const auto* 结构 = std::get_if<特征概念结构化I64值域>(&定义.值域);
            return 结构 && *分量 < 结构->分量.size() ? &结构->分量[*分量].值域 : nullptr;
        };
        const auto* 有效范围 = 读取区间(*defs[位置]);
        if (!有效范围) 不支持(static_cast<std::uint32_t>(位置 + 1), "numeric candidate range unavailable");
        // 正式条件域参与别名/原型合取后，直接在每个合法交集取三点。
        if(!初始数值条件.empty())return 生成通用模拟整数候选(*有效范围);
        // 私有原型没有独立调用参数，其正式范围直接作为取样依据。
        const auto* 初始范围 = 位置 < N && 原始参数定义[位置]
            ? 读取区间(*原始参数定义[位置]) : 有效范围;
        if (!初始范围) {
            if (位置 < N && std::holds_alternative<特征概念全材料值域>(原始参数定义[位置]->值域))
                初始范围 = 有效范围; // 全材料声明本身没有有限点，由正式原型限定提供。
            else 不支持(static_cast<std::uint32_t>(位置 + 1), "numeric parameter sample shape unavailable");
        }
        auto 候选 = 生成通用模拟整数候选(*初始范围);
        std::erase_if(候选, [&](const auto& 项) {
            const auto 值 = std::get<std::int64_t>(项);
            return std::none_of(有效范围->begin(), 有效范围->end(), [&](const auto& 区间) {
                return 区间.下界 <= 值 && 值 <= 区间.上界;
            });
        });
        for (std::size_t j = 0; j < N && 位置 < N; ++j) {
            if (j == 位置 || 代表[j] != 代表[位置] || !原始参数定义[j]) continue;
            if (std::holds_alternative<特征概念全材料值域>(原始参数定义[j]->值域)) continue;
            const auto* 范围 = 读取区间(*原始参数定义[j]);
            if (!范围) 不支持(static_cast<std::uint32_t>(j + 1), "aliased numeric sample shape unavailable");
            const auto 其它候选 = 生成通用模拟整数候选(*范围);
            std::erase_if(候选, [&](const auto& 值) {
                return std::find(其它候选.begin(), 其它候选.end(), 值) == 其它候选.end();
            });
        }
        return 候选;
    };
    // 来源只由正式有限材料明确给出，不从现实实例中任取首项。
    // 这里只投影执行所需的E/F/当前材料闭包；实例身份全部转换为私有输入位置。
    const auto 原型数量=base.size();
    新模拟需求输入图 需求图模板;
    std::set<稳定编码> 首批需求;
    std::map<std::size_t,std::vector<std::uint32_t>> 需求候选序号;
    // 正式任务/需求成员先交各自owner读取；可读值投影不冒充可执行本地图。
    std::map<稳定编码,新模拟任务来源投影> 任务来源;
    std::map<稳定编码,std::uint32_t> 任务位置;
    std::vector<稳定编码> 任务顺序;
    std::vector<新模拟任务治理输入> 任务治理组;
    const auto 加任务=[&](稳定编码 任务)->std::uint32_t {
        停止();
        if(!有效(任务))throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"typed task source identity is empty");
        if(const auto 已有位置=任务位置.find(任务);已有位置!=任务位置.end())return 已有位置->second;
        const auto 读取=业务来源_.读取任务来源(任务);
        if(读取.状态==新模拟业务来源状态::来源已变化)throw 模拟输入采集漂移{};
        if(读取.状态!=新模拟业务来源状态::已读取||!读取.信息)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"typed task source unavailable: "+读取.诊断);
        const auto& 来源=*读取.信息;
        if(来源.任务.节点!=任务||来源.虚拟存在.任务节点!=任务
            ||!有效(来源.虚拟存在.节点)||来源.任务.任务虚拟存在节点!=来源.虚拟存在.节点
            ||来源.治理特征.引用!=新任务虚拟存在引用{任务,来源.虚拟存在.节点,来源.虚拟存在.任务状态特征节点}
            ||!有效(来源.治理特征.状态.特征节点)||来源.治理特征.状态.特征节点!=来源.虚拟存在.任务状态特征节点)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"typed task source projections disagree");
        if(任务顺序.size()>=(std::numeric_limits<std::uint32_t>::max)())
            throw 新学习运行异常(m.节点,新学习运行异常原因::资源不足,"typed task position exceeds uint32");
        const auto 位置=static_cast<std::uint32_t>(任务顺序.size()+1);
        任务来源.emplace(任务,来源);任务位置.emplace(任务,位置);任务顺序.push_back(任务);return 位置;
    };
    struct 来源采集已漂移 final {};
    for (;;) {
        停止();
        HY_SIM_PROGRESS(L"开始采集业务来源",原型数量);
        任务来源.clear();
        任务位置.clear();任务顺序.clear();
        任务治理组.clear();
        try
        {
            for (std::size_t i = 0; i < 原型数量; ++i)
                if (defs[i] && (defs[i]->比较规则 == 特征概念比较规则::服务任务集合身份判等 ||
                                defs[i]->比较规则 == 特征概念比较规则::需求集合身份判等))
                {
                    停止();
                    if (std::holds_alternative<特征概念全材料值域>(defs[i]->值域)
                        &&defs[i]->材料类型==特征概念材料物理类型::U64数组) continue;
                    const auto *vs = std::get_if<std::vector<稳定编码>>(&defs[i]->值域);
                    if (!vs)
                        不支持(static_cast<std::uint32_t>(i + 1),
                               "task/demand collection has no explicitly known finite source material");
                    for (const auto V : *vs)
                    {
                        const auto material = 方法_.读取模拟候选材料(V);
                        const auto *members =
                            material ? std::get_if<std::vector<std::uint64_t>>(&material->材料) : nullptr;
                        if (!members)
                            throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致,
                                                 "typed source collection material unavailable");
                        for (const auto n : *members)
                        {
                            停止();
                            const auto prefix = " M=" + std::to_string(m.节点.值) +
                                                " parameter=" + std::to_string(i + 1) + " V=" + std::to_string(V.值) +
                                                " member=" + std::to_string(n);
                            const auto consume = [&](const auto &read, const char *kind) {
                                if (read.状态 == 新模拟业务来源状态::来源已变化)
                                    throw 来源采集已漂移{};
                                if (read.状态 == 新模拟业务来源状态::尚缺依赖提供者)
                                    不支持(static_cast<std::uint32_t>(i + 1), read.诊断);
                                if (read.状态 != 新模拟业务来源状态::已读取 || !read.信息)
                                    throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致,
                                                         prefix + " source=" + kind + " status=" +
                                                             std::to_string(static_cast<unsigned>(read.状态)) + " " +
                                                             read.诊断);
                                if(std::string_view{kind}=="demand")首批需求.insert(稳定编码{n});
                            };
                            if (defs[i]->比较规则 == 特征概念比较规则::服务任务集合身份判等)
                            {
                                (void)加任务(稳定编码{n});
                            }
                            else
                                consume(业务来源_.读取需求来源(稳定编码{n}), "demand");
                        }
                    }
                }
            break;
        }
        catch (const 来源采集已漂移 &)
        {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
            ++观察.来源漂移;
#endif
            HY_SIM_PROGRESS(L"来源采集漂移，重采整批",0);
            // 还未建立模拟对象或发表知识；丢弃整批纯值副本，不拼接两次来源。
            停止();
            std::this_thread::yield();
        }
    }
    std::map<稳定编码, std::size_t> 来源存在位置;
    HY_SIM_PROGRESS(L"开始构造引用闭包",首批需求.size());
    std::map<稳定编码, 新存在信息> 来源存在快照;
    std::map<稳定编码, 新特征匹配实例快照> 来源特征快照;
    std::map<稳定编码,新特征信息> 来源完整特征快照;
    std::vector<稳定编码> 待复制存在;
    const auto 加入来源存在 = [&](稳定编码 E) -> std::size_t {
        if(const auto it=来源存在位置.find(E);it!=来源存在位置.end())
            return it->second;
        const auto e=方法_.读取模拟来源存在(E);
        if(!e)
            throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致, "copy source existence unavailable");
        auto pos=base.size();
        for(std::size_t i=0;i<base.size();++i)if(是普通存在(i)&&(i>=N||代表[i]==i)) {
            const auto& ec=采集归属根(有效存在概念(i)).概念;
            if(ec.对应存在节点&&*ec.对应存在节点==E) {
                if(pos!=base.size())不支持(0,"one source has multiple unmerged dedicated private prototypes");
                pos=i;
            }
        }
        if(pos==base.size()) {
            新模拟有序参数输入 p{{0,新方法参数信息类型::存在,e->专属存在概念节点}};
            base.push_back(std::move(p));defs.push_back(std::nullopt);
        }
        base[pos].来源存在=E;
        来源存在位置.emplace(E,pos);
        来源存在快照.emplace(E, *e);
        待复制存在.push_back(E);
        return pos;
    };
    for(std::size_t i=0;i<原型数量;++i)if(defs[i]
        &&defs[i]->比较规则==特征概念比较规则::存在集合身份判等) {
        if(const auto* vs=std::get_if<std::vector<稳定编码>>(&defs[i]->值域)) {
            // 加入来源可能扩容defs，遍历材料编码的值副本，不保留容器内指针。
            const auto 材料组=*vs;
            for(const auto V:材料组) {
                const auto v=方法_.读取模拟候选材料(V);
                const auto* a=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
                if(!a)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source collection material unavailable");
                for(const auto E:*a)加入来源存在(稳定编码{E});
            }
        }
    }
    // 下方单一typed队列同时展开E/F/D/U/C/要求/语言场景，保持内容环与别名。
    std::map<稳定编码,基础联合节点快照> 定义保护;
    std::map<稳定编码,基础联合节点快照> 共享定义保护;
    const auto 加保护=[&](const auto& group) {
        for(const auto& snap:group) {
            const auto [it,added]=定义保护.emplace(snap.结构.节点.编码,snap);
            if(!added&&(it->second.结构.字段组!=snap.结构.字段组||it->second.入向字段组!=snap.入向字段组
                ||it->second.入向外部关系组!=snap.入向外部关系组||it->second.出向外部关系组!=snap.出向外部关系组))
                throw 模拟输入采集漂移{};
        }
    };
    const auto 加正式保护=[&](const auto& group) {
        for(const auto& n:group) {
            const auto [it,added]=共享定义保护.emplace(n.结构.节点.编码,n);
            if(!added&&!归属根相同(it->second,n))throw 模拟输入采集漂移{};
        }
    };
    const auto 保护引用定义=[&](const 新需求目标定义& target) {
        if(target.引用内容定位)不支持(0,"引用内容目标的模拟定义保护尚未支持");
        const auto feature=[&](稳定编码 C) {
            if(!有效(C))return;
            const auto r=概念_.读取特征概念定义保护(C);if(r.前提已变化)throw 模拟输入采集漂移{};
            if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(r.状态!=基础结构操作状态::已完成||!r.信息)throw std::runtime_error("copied semantic definition missing");加正式保护(r.预期节点组);
        };
        feature(target.目标特征类型概念节点);if(target.目标值域概念节点)feature(*target.目标值域概念节点);
        if(target.目标比较关系概念节点) {
            const auto b=二次概念_.读取二次特征定义保护(*target.目标比较关系概念节点);if(b.前提已变化)throw 模拟输入采集漂移{};
            if(b.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(b.状态!=基础结构操作状态::已完成||!b.信息)throw std::runtime_error("copied semantic comparison definition missing");加正式保护(b.预期节点组);
            for(const auto& in:b.信息->输入定义组)if(in.特征类型概念节点)feature(*in.特征类型概念节点);
            if(b.信息->参照特征概念节点)feature(*b.信息->参照特征概念节点);feature(b.信息->输出特征概念.节点);
        }
    };
    const auto 保护目标=[&](const 新需求目标定义& target) {
        if(target.引用内容定位)不支持(0,"引用内容目标的模拟参数生成尚未支持");
        const auto g=核验需求目标定义保护(target,概念_,二次概念_);
        if(g.状态==新需求目标定义核验状态::前提已变化)
            throw 模拟输入采集漂移{};
        if(g.状态==新需求目标定义核验状态::资源失败)throw std::bad_alloc{};
        if(g.状态!=新需求目标定义核验状态::已完成)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"typed demand target definition invalid");
        加正式保护(g.完整保护);
    };
    const auto concepts=方法_.读取模拟需求概念组();
    const auto 核构造读取=[&](正式结构操作状态 状态,bool 有载荷,const char* 诊断) {
        if(状态==正式结构操作状态::前提已变化)throw 模拟输入采集漂移{};
        if(状态==正式结构操作状态::未找到||状态==正式结构操作状态::依赖未就绪)不支持(0,诊断);
        if(状态!=正式结构操作状态::已读取||!有载荷)throw std::runtime_error(诊断);
    };
    const auto 内容概念=[&](std::uint32_t role) {
        核构造读取(concepts.状态,concepts.信息.has_value(),"formal demand concept group unavailable");
        const auto it=std::find_if(concepts.信息->子类与支持定义.begin(),concepts.信息->子类与支持定义.end(),[&](const auto& x){return x.first==role;});
        if(it==concepts.信息->子类与支持定义.end())不支持(0,"formal demand subclass definition unavailable");
        const auto read=方法_.读取模拟正式结构(it->second);
        核构造读取(read.状态,read.信息.has_value(),"formal demand subclass is unreadable");
        加正式保护(read.完整保护组);return it->second;
    };
    std::map<稳定编码,std::uint32_t> D位置,U位置,C位置,R位置,L位置;
    std::map<稳定编码,新模拟需求来源投影> D来源;
    std::map<稳定编码,服务要求理解信息> U来源;
    std::map<稳定编码,服务要求确认信息> C来源;
    std::map<稳定编码,新部分服务要求材料> R来源;
    std::map<稳定编码,新自然语言场景复制信息> L来源;
    std::vector<稳定编码> 待复制D,待复制U,待复制C,待复制R,待复制L;
    const auto 入列=[&](auto& positions,auto& queue,auto& inputs,稳定编码 id) {
        if(!有效(id))throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"typed source has invalid identity");
        if(const auto it=positions.find(id);it!=positions.end())return it->second;
        const auto pos=static_cast<std::uint32_t>(inputs.size()+1);positions.emplace(id,pos);queue.push_back(id);inputs.emplace_back();return pos;
    };
    const auto 加D=[&](稳定编码 id){return 入列(D位置,待复制D,需求图模板.需求组,id);};
    const auto 加U=[&](稳定编码 id){return 入列(U位置,待复制U,需求图模板.理解组,id);};
    const auto 加C=[&](稳定编码 id){return 入列(C位置,待复制C,需求图模板.确认组,id);};
    const auto 加R=[&](稳定编码 id){return 入列(R位置,待复制R,需求图模板.要求组,id);};
    const auto 加L=[&](稳定编码 id){return 入列(L位置,待复制L,需求图模板.语言场景组,id);};
    for(const auto D:首批需求)加D(D);
    for(std::size_t i=0;i<原型数量;++i)if(defs[i]&&defs[i]->材料类型==特征概念材料物理类型::需求节点引用) {
        if(defs[i]->比较规则!=特征概念比较规则::需求身份精确)不支持(static_cast<std::uint32_t>(i+1),"demand reference comparison is not typed identity");
        if(const auto* ids=std::get_if<std::vector<稳定编码>>(&defs[i]->值域))for(const auto D:*ids)需求候选序号[i].push_back(加D(D));
    }
    struct 复制失败诊断上下文 final {
        稳定编码 来源特征;
        const char* 复制入口;
        std::optional<std::size_t> 既有输入索引;
        std::uint32_t 所属存在输入;
        std::uint32_t 所属语言场景输入;
        const char* 值槽;
        std::optional<std::size_t> 槽内索引;
    };
    const auto 转准确=[&](const 新特征准确值& value,const 新特征匹配定义快照& def,
        const 复制失败诊断上下文& diagnostic)->新模拟复制准确值 {
        using K=新模拟复制值种类;
        if(const auto* x=std::get_if<std::int64_t>(&value)) {
            if(def.材料类型!=特征概念材料物理类型::I64标量)throw std::runtime_error("typed copied scalar disagrees with feature type");
            return {K::I64,*x};
        }
        const auto id=std::get<稳定编码>(value);
        if(def.材料类型==特征概念材料物理类型::存在节点引用)return {K::存在引用,static_cast<std::uint32_t>(加入来源存在(id)+1)};
        if(def.材料类型==特征概念材料物理类型::需求节点引用)return {K::需求引用,加D(id)};
        if(def.材料类型==特征概念材料物理类型::信息节点引用)不支持(0,"copied feature information-reference category has no typed closure");
        const auto material=方法_.读取模拟候选材料(id);if(!material)throw std::runtime_error("copied feature material unreadable");
        if(def.比较规则==特征概念比较规则::存在集合身份判等||def.比较规则==特征概念比较规则::需求集合身份判等) {
            const auto* ids=std::get_if<std::vector<std::uint64_t>>(&material->材料);if(!ids)throw std::runtime_error("typed collection payload invalid");
            std::vector<std::uint32_t> mapped;for(const auto source:*ids)mapped.push_back(def.比较规则==特征概念比较规则::存在集合身份判等
                ?static_cast<std::uint32_t>(加入来源存在(稳定编码{source})+1):加D(稳定编码{source}));
            if(def.比较规则==特征概念比较规则::存在集合身份判等)return {K::存在集合,std::move(mapped)};
            return {K::需求集合,std::move(mapped)};
        }
        if(def.比较规则==特征概念比较规则::服务任务集合身份判等){
            const auto* 成员组=std::get_if<std::vector<std::uint64_t>>(&material->材料);
            if(material->物理类型!=材料物理类型::U64数组||!成员组)
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"typed copied task collection payload invalid");
            std::vector<std::uint32_t> 映射组;
            for(const auto 任务:*成员组)映射组.push_back(加任务(稳定编码{任务}));
            return {K::任务集合,std::move(映射组)};
        }
        if(def.比较规则==特征概念比较规则::消息集合身份判等) {
            const auto* ids=std::get_if<std::vector<std::uint64_t>>(&material->材料);
            if(!ids||!ids->empty())不支持(0,
                std::string("copied history/current task or message collection lacks complete typed mapping")
                +"; methodId="+std::to_string(m.节点.值)
                +"; sourceFeatureId="+std::to_string(diagnostic.来源特征.值)
                +"; featureTypeRootId="+std::to_string(def.特征类型根.值)
                +"; featureConceptId="+std::to_string(def.概念节点.值)
                +"; copyEntry="+diagnostic.复制入口
                +"; existingInputIndex0="+(diagnostic.既有输入索引 ? std::to_string(*diagnostic.既有输入索引) : "null")
                +"; ownerExistenceInput="+std::to_string(diagnostic.所属存在输入)
                +"; languageSceneInput="+std::to_string(diagnostic.所属语言场景输入)
                +"; valueSlot="+diagnostic.值槽
                +"; slotIndex0="+(diagnostic.槽内索引 ? std::to_string(*diagnostic.槽内索引) : "null")
                +"; definitionMaterialTypeCode="+std::to_string(static_cast<std::int64_t>(def.材料类型))
                +"; comparisonRuleCode="+std::to_string(static_cast<std::int64_t>(def.比较规则))
                +"; materialId="+std::to_string(id.值)
                +"; materialPhysicalTypeCode="+std::to_string(static_cast<std::int64_t>(material->物理类型))
                +"; payloadIsU64Array="+(ids ? "true" : "false")
                +"; payloadEmpty="+(ids ? (ids->empty() ? "true" : "false") : "null")
                +"; memberCount="+(ids ? std::to_string(ids->size()) : "null"));
        }
        if(material->物理类型!=材料物理类型::UTF8字符串&&material->物理类型!=材料物理类型::I64数组&&material->物理类型!=材料物理类型::U64数组)
            不支持(0,"copied material physical category unsupported");
        return {K::普通材料,*material};
    };
    const auto 转域=[&](const 特征概念值域& domain,const 新特征匹配定义快照& def,
        const 复制失败诊断上下文& diagnostic)->新模拟复制值域 {
        if(const auto* ids=std::get_if<std::vector<稳定编码>>(&domain)) {
            std::vector<新模拟复制准确值> out;std::size_t 索引=0;
            for(const auto id:*ids){
                auto 当前诊断=diagnostic;当前诊断.槽内索引=索引++;
                out.push_back(转准确(新特征准确值{id},def,当前诊断));
            }
            return {std::move(out)};
        }
        if(const auto* d=std::get_if<特征概念存在集合值域>(&domain);d&&!d->必含存在节点组.empty())不支持(0,"copied domain existence-member constraints lack a local domain representation");
        if(const auto* d=std::get_if<特征概念需求集合值域>(&domain);d&&!d->必含需求节点组.empty())不支持(0,"copied domain demand-member constraints lack a local domain representation");
        if(const auto* d=std::get_if<特征概念任务集合值域>(&domain);d&&(!d->必含任务节点组.empty()||!d->必含需求任务组.empty()||!d->必含服务要求任务组.empty()))不支持(0,"copied domain task constraints unsupported");
        return {domain};
    };
    std::map<稳定编码,std::size_t> 来源F位置;
    const auto 复制F=[&](稳定编码 F,std::optional<std::size_t> existing,std::uint32_t owner,std::uint32_t language,const char* entry) {
        if(const auto it=来源F位置.find(F);it!=来源F位置.end()) {
            const auto& p=base.at(it->second);if(p.所属存在参数!=owner||p.所属语言场景输入!=language)throw std::runtime_error("source feature has conflicting owners");return it->second;
        }
        const auto shape=方法_.读取模拟来源特征(F);const auto full=方法_.读取模拟来源完整特征(F);
        if(!shape||!full)throw std::runtime_error("full source feature unavailable");
        if(full->上级特征节点)不支持(0,"source feature: non-top-level organization unsupported");
        if(shape->当前值!=full->当前值||shape->实例区间值!=full->实例区间值||shape->实例单位!=full->实例单位)
            throw 模拟输入采集漂移{};
        新模拟完整特征复制输入 copied;copied.实例单位=full->实例单位;
        if(full->当前值)copied.当前值=转准确(*full->当前值,shape->定义,{F,entry,existing,owner,language,"current",{}});
        if(full->标准值)copied.标准值=转准确(*full->标准值,shape->定义,{F,entry,existing,owner,language,"standard",{}});
        if(full->实例区间值)copied.实例区间=转域(*full->实例区间值,shape->定义,{F,entry,existing,owner,language,"interval-finite",{}});
        if(full->实例历史聚合值域)copied.历史聚合=转域(*full->实例历史聚合值域,shape->定义,{F,entry,existing,owner,language,"history-domain-finite",{}});
        std::size_t 历史不同值索引=0;
        for(const auto& v:full->历史不同值)
            copied.历史不同值.push_back(转准确(v,shape->定义,{F,entry,existing,owner,language,"history-distinct",历史不同值索引++}));
        std::size_t 命中记录索引=0;
        for(const auto& hit:full->历史值命中信息) {
            copied.历史值命中信息.push_back({转准确(hit.值,shape->定义,{F,entry,existing,owner,language,"history-hit",命中记录索引++}),hit.命中次数});
        }
        auto pos=existing.value_or(base.size());
        新模拟有序参数输入 input{{0,新方法参数信息类型::特征,shape->定义.概念节点},owner};
        if(existing)input.规格=base.at(pos).规格;
        input.所属语言场景输入=language;input.复制上下文=true;input.完整复制特征=std::move(copied);
        if(full->当前值)input.初值方式=新模拟特征初值方式::复制准确;
        else input.初值方式=新模拟特征初值方式::复制未知;
        if(existing){
            收窄特征范围(pos,shape->定义);
            const auto 约束诊断=[&](const std::string& status,const std::string& relation) {
                std::string current="unknown";
                if(full->当前值) {
                    if(const auto* x=std::get_if<std::int64_t>(&*full->当前值))current="I64:"+std::to_string(*x);
                    else current="node:"+std::to_string(std::get<稳定编码>(*full->当前值).值);
                }
                return " position="+std::to_string(pos+1)+" FT="+std::to_string(shape->定义.特征类型根.值)
                    +" material="+std::to_string(static_cast<int>(shape->定义.材料类型))+" current="+current
                    +" right-domain-variant="+std::to_string(defs[pos]->值域.index())
                    +" comparison-status="+status+" relation="+relation;
            };
            if(!full->当前值) {
                const bool direct=std::any_of(初始数值条件.begin(),初始数值条件.end(),[&](const auto& c) {
                    return c.参数序号&&c.参数序号<=N&&代表[c.参数序号-1]==pos
                        &&(c.特征值域概念节点||c.精确数值域);
                });
                const bool attached=std::any_of(初始附属条件.begin(),初始附属条件.end(),[&](const auto& c) {
                    return c.参数序号&&c.参数序号<=N&&代表[c.参数序号-1]+1==owner
                        &&c.特征类型概念节点==std::optional{shape->定义.特征类型根}
                        &&(c.特征值域概念节点||c.精确数值域);
                });
                if(direct||attached)不支持(static_cast<std::uint32_t>(pos+1),"unknown copied current cannot satisfy a necessary value constraint"+约束诊断("not-compared","unknown"));
            }
            if(full->当前值) {
                if(shape->定义.材料类型==特征概念材料物理类型::存在节点引用) {
                    const auto* E=std::get_if<稳定编码>(&*full->当前值);
                    const auto* domain=std::get_if<特征概念存在引用值域>(&defs[pos]->值域);
                    if(!E||!domain)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                        "copied existence reference or concept range malformed"+约束诊断("not-compared","invalid-shape"));
                    // 转准确已通过加入来源存在采集此E；沿同一份来源快照核概念归属，后续统一fresh核同。
                    const auto source=来源存在快照.find(*E);
                    if(source==来源存在快照.end())throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                        "copied existence reference lacks captured identity"+约束诊断("not-compared","missing-source"));
                    if(!范围包含(source->second.专属存在概念节点,domain->存在概念节点))
                        不支持(static_cast<std::uint32_t>(pos+1),"fixed source existence does not belong to required concept"
                            +约束诊断("existence-membership-completed","outside")
                            +" source-concept="+std::to_string(source->second.专属存在概念节点.值)
                            +" required-concept="+std::to_string(domain->存在概念节点.值));
                } else {
                    特征概念值域 singleton;
                    if(const auto* x=std::get_if<std::int64_t>(&*full->当前值))singleton=std::vector<特征概念I64闭区间>{{*x,*x}};
                    else singleton=std::vector<稳定编码>{std::get<稳定编码>(*full->当前值)};
                    const auto check=概念_.比较值域(shape->定义.特征类型根,singleton,defs[pos]->值域);
                    const auto detail=约束诊断(std::to_string(static_cast<int>(check.状态)),
                        check.关系 ? std::to_string(static_cast<int>(*check.关系)) : "absent");
                    if(check.状态==特征概念规则状态::资源失败)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::资源不足,"source constraint comparison resource failure"+detail);
                    if(check.状态==特征概念规则状态::前提已变化)throw 模拟输入采集漂移{};
                    if(check.状态==特征概念规则状态::规则不支持)
                        不支持(static_cast<std::uint32_t>(pos+1),"source constraint comparison rule unsupported"+detail);
                    if(check.状态!=特征概念规则状态::已完成||!check.关系)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"source constraint comparison did not produce a valid relation"+detail);
                    switch(*check.关系) {
                    case 特征概念值域关系::相等:case 特征概念值域关系::右包含左:break;
                    case 特征概念值域关系::左包含右:case 特征概念值域关系::相交:case 特征概念值域关系::分离:
                        不支持(static_cast<std::uint32_t>(pos+1),"fixed source feature conflicts with generated formal constraints"+detail);break;
                    default:throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"source constraint comparison returned invalid relation"+detail);
                    }
                }
            }
            // 条件约束可筛除来源，不能改写来源当前值、标准值或历史。
            for(const auto ref:base[pos].历史引用存在参数组) {
                const 新模拟复制准确值 v{新模拟复制值种类::存在引用,ref};
                if(std::find(input.完整复制特征->历史不同值.begin(),input.完整复制特征->历史不同值.end(),v)==input.完整复制特征->历史不同值.end())
                    不支持(static_cast<std::uint32_t>(pos+1),"fixed source history lacks required formal relation"+约束诊断("history-membership-completed","outside"));
            }
            base[pos]=std::move(input);
        }else {base.push_back(std::move(input));defs.push_back(shape->定义);}
        来源F位置.emplace(F,pos);来源特征快照.emplace(F,*shape);来源完整特征快照.emplace(F,*full);return pos;
    };
    for(const auto& [pos,F]:固定来源特征)复制F(F,pos,base.at(pos).所属存在参数,0,"fixed-source-feature");
    std::size_t nextE=0,nextD=0,nextU=0,nextC=0,nextR=0,nextL=0;
    const auto sourceRead=[&](const auto& read,const char* category)->const auto& {
        if(read.状态==新模拟需求状态::前提已变化)throw 模拟输入采集漂移{};
        if(read.状态==新模拟需求状态::资源失败)throw std::bad_alloc{};
        if(read.状态!=新模拟需求状态::已完成||!read.信息)throw std::runtime_error(std::string(category)+" source unavailable");
        加保护(read.完整保护);return *read.信息;
    };
    while(nextE<待复制存在.size()||nextD<待复制D.size()||nextU<待复制U.size()||nextC<待复制C.size()||nextR<待复制R.size()||nextL<待复制L.size()) {
        停止();
        if(nextE<待复制存在.size()&&nextD==待复制D.size()&&nextU==待复制U.size()
            &&nextC==待复制C.size()&&nextR==待复制R.size()&&nextL==待复制L.size()) {
            const auto E=待复制存在[nextE++];const auto pos=来源存在位置.at(E);const auto e=来源存在快照.at(E);
            if(base.at(pos).引用语言场景输入)continue;
            const auto language=业务来源_.读取语言场景来源(E);
            if(language.状态==新自然语言场景结构比较状态::资源失败)throw std::bad_alloc{};
            if(language.状态==新自然语言场景结构比较状态::前提已变化)throw 模拟输入采集漂移{};
            if(language.状态==新自然语言场景结构比较状态::复制内容未支持)不支持(0,"source existence is a language scene with unsupported content");
            if(language.状态==新自然语言场景结构比较状态::已完成) {
                if(!language.信息)throw std::runtime_error("successful language scene source projection missing");
                base.at(pos).引用语言场景输入=加L(E);continue;
            }
            if(language.状态!=新自然语言场景结构比较状态::场景不存在)
                throw std::runtime_error("source existence language-scene classification failed");
            if(!e.子存在节点组.empty())不支持(0,"source E child organization has no typed local input");
            for(const auto F:e.特征节点组) {
                const auto shape=方法_.读取模拟来源特征(F);if(!shape)throw 模拟输入采集漂移{};
                std::optional<std::size_t> existing;
                for(std::size_t k=0;k<base.size();++k)if(defs[k]&&(k>=N||代表[k]==k)&&base[k].所属存在参数==pos+1
                    &&defs[k]->特征类型根==shape->定义.特征类型根) {
                    if(existing)不支持(0,"source prototype contains ambiguous same-type feature aliases");existing=k;
                }
                复制F(F,existing,static_cast<std::uint32_t>(pos+1),0,"source-existence-all-features");
            }
            continue;
        }
        if(nextD<待复制D.size()) {
            const auto D=待复制D[nextD++];const auto read=业务来源_.读取需求来源(D);
            if(read.状态==新模拟业务来源状态::来源已变化)throw 模拟输入采集漂移{};
            if(read.状态==新模拟业务来源状态::尚缺依赖提供者)不支持(0,read.诊断);
            if(read.状态!=新模拟业务来源状态::已读取||!read.信息||!read.信息->完整需求)不支持(0,"source demand has no complete target");
            加保护(read.完整保护);const auto g=read.信息->治理;const auto d=*read.信息->完整需求;
            if(d.目标.引用内容定位)不支持(0,"引用内容目标不能映射为普通值或方向需求");
            for(const auto& edge:read.信息->实例引用组)switch(edge.类别) {
                case 新模拟业务引用类别::存在:case 新模拟业务引用类别::需求:
                case 新模拟业务引用类别::语言理解:case 新模拟业务引用类别::服务要求确认:case 新模拟业务引用类别::服务要求:break;
                default:throw 新学习运行异常(m.节点,新学习运行异常原因::规格未支持,"demand source unsupported field="+edge.字段角色+" category="+std::to_string(static_cast<int>(edge.类别)));
            }
            新模拟需求输入 input;input.序号=D位置.at(D);input.内容概念=concepts.信息->需求;
            input.主体存在输入=static_cast<std::uint32_t>(加入来源存在(d.需求主体存在节点)+1);
            input.目标存在输入=static_cast<std::uint32_t>(加入来源存在(d.目标存在节点)+1);
            input.目标=d.目标;保护目标(input.目标);input.执行域=d.执行域;input.权重=d.权重;input.活动状态=d.活动状态;
            input.附加内容概念组.push_back(内容概念(d.目标.目标形式 == 新需求目标形式::值 ? 204 : 205));
            if(d.执行域==新需求执行域::被认知)input.附加内容概念组.push_back(内容概念(208));
            input.根角色=d.根角色;input.来源类型=d.来源类型;if(d.父需求节点)input.父需求输入=加D(*d.父需求节点);
            if(d.认知来源){input.理解输入=加U(d.认知来源->理解节点);input.确认输入=加C(d.认知来源->内容确认节点);if(d.认知来源->来源要求节点)input.来源要求输入=加R(*d.认知来源->来源要求节点);}
            if(g.外部要求节点)input.外部要求输入=加R(*g.外部要求节点);
            for(const auto c:d.服务要求确认组)input.服务要求确认输入组.push_back(加C(c));
            input.来源=新模拟需求来源种类::单向复制;input.来源需求=D;需求图模板.需求组.at(input.序号-1)=std::move(input);D来源.emplace(D,*read.信息);continue;
        }
        if(nextU<待复制U.size()) {
            const auto id=待复制U[nextU++];const auto read=业务来源_.读取模拟理解来源(id);const auto u=sourceRead(read,"understanding");
            新模拟理解输入 input;input.序号=U位置.at(id);input.语言场景输入=加L(u.语言场景节点);
            input.来源存在输入=static_cast<std::uint32_t>(加入来源存在(u.来源人类节点)+1);input.目标存在输入=static_cast<std::uint32_t>(加入来源存在(u.目标存在节点)+1);
            input.公开会话标识=u.公开会话标识;input.目标=u.目标;input.固定语义=u.固定语义;input.来源身份=id;
            if(u.来源要求节点)input.来源要求输入=加R(*u.来源要求节点);保护引用定义(input.目标);
            需求图模板.理解组.at(input.序号-1)=std::move(input);U来源.emplace(id,u);continue;
        }
        if(nextC<待复制C.size()) {
            const auto id=待复制C[nextC++];const auto read=业务来源_.读取模拟确认来源(id);const auto c=sourceRead(read,"confirmation");
            新模拟确认输入 input{C位置.at(id),加U(c.理解.节点),c.需重新确认,id};
            需求图模板.确认组.at(input.序号-1)=input;C来源.emplace(id,c);continue;
        }
        if(nextR<待复制R.size()) {
            const auto id=待复制R[nextR++];const auto read=业务来源_.读取模拟要求来源(id);const auto r=sourceRead(read,"requirement");
            新模拟要求输入 input;input.序号=R位置.at(id);input.来源存在输入=static_cast<std::uint32_t>(加入来源存在(r.来源人类节点)+1);
            if(r.已知目标.目标存在节点)input.目标存在输入=static_cast<std::uint32_t>(加入来源存在(*r.已知目标.目标存在节点)+1);
            input.已知目标=r.已知目标;input.已知目标.目标存在节点.reset();input.公开会话标识=r.公开会话标识;input.来源要求标识=r.来源要求标识;
            if(r.理解节点)input.理解输入=加U(*r.理解节点);if(r.确认节点)input.确认输入=加C(*r.确认节点);
            input.来源身份=id;需求图模板.要求组.at(input.序号-1)=std::move(input);R来源.emplace(id,r);continue;
        }
        if(nextL<待复制L.size()) {
            const auto id=待复制L[nextL++];const auto read=业务来源_.读取语言场景来源(id);
            if(read.状态==新自然语言场景结构比较状态::资源失败)throw std::bad_alloc{};
            if(read.状态==新自然语言场景结构比较状态::前提已变化)throw 模拟输入采集漂移{};
            if(read.状态==新自然语言场景结构比较状态::复制内容未支持)不支持(0,"language source scene contains unsupported child/state/dynamic records");
            if(read.状态!=新自然语言场景结构比较状态::已完成||!read.信息)throw std::runtime_error("language scene source unreadable");
            加保护(read.完整保护);const auto l=*read.信息;新模拟语言场景输入 input;input.序号=L位置.at(id);input.专属存在概念=l.场景.专属存在概念节点;input.来源身份=id;
            if(const auto e=来源存在位置.find(id);e!=来源存在位置.end())base.at(e->second).引用语言场景输入=input.序号;
            for(const auto F:l.场景.特征节点组)input.特征输入组.push_back(static_cast<std::uint32_t>(复制F(F,{},0,input.序号,"language-scene-all-features")+1));
            for(const auto E:l.场景.直接成员存在组)input.成员存在输入组.push_back(static_cast<std::uint32_t>(加入来源存在(E)+1));
            for(const auto E:l.场景.边界存在组)input.边界存在输入组.push_back(static_cast<std::uint32_t>(加入来源存在(E)+1));
            for(const auto& e:l.概念实例组)input.实例组.push_back({static_cast<std::uint32_t>(加入来源存在(e.节点)+1),e.概念节点,e.种类});
            需求图模板.语言场景组.at(input.序号-1)=std::move(input);L来源.emplace(id,l);
        }
    }
    // 生成分支使用正式概念结构及原型FT闭包；不以来源D存在为条件。
    HY_SIM_PROGRESS(L"引用闭包构造完成",base.size());
    if(需求构造依据) {
      try {
        HY_SIM_PROGRESS(L"开始生成全部需求候选",原型数量);
        核构造读取(concepts.状态,concepts.信息.has_value(),"formal demand concept group unavailable");
        const auto root=方法_.读取模拟正式结构(concepts.信息->需求);
        核构造读取(root.状态,root.信息.has_value(),"formal demand root unavailable");加正式保护(root.完整保护组);
        加正式保护(需求构造依据->完整保护);
        const auto valueC=内容概念(204),directionC=内容概念(205),recognizedC=内容概念(208);
        const auto 正式整数范围=[&](std::uint32_t role) {
            const auto def=方法_.读取模拟正式结构(内容概念(role));
            核构造读取(def.状态,def.信息.has_value(),"demand primitive definition unavailable");
            const auto* range=std::get_if<基础范围概念定义>(&def.信息->定义);
            if(!range)不支持(0,"demand primitive is not an integer range");
            if(range->原语==基础概念原语::I64有限枚举)return range->整数参数;
            if(range->原语==基础概念原语::I64闭区间&&range->整数参数.size()==2) {
                std::vector<std::int64_t> values;
                for(const auto& x:生成通用模拟整数候选({{range->整数参数[0],range->整数参数[1]}}))values.push_back(std::get<std::int64_t>(x));return values;
            }不支持(0,"unsupported formal demand primitive range");return std::vector<std::int64_t>{};
        };
        const auto activities=正式整数范围(413),directions=正式整数范围(414),weights=正式整数范围(408),domains=正式整数范围(411);
        if(std::find(weights.begin(),weights.end(),0)==weights.end()||std::find(domains.begin(),domains.end(),2)==domains.end())
            不支持(0,"recognized demand weight/execution-domain formal constraints do not permit construction");
        std::vector<std::size_t> subjects,targets;
        for(std::size_t e=0;e<原型数量;++e)if(是普通存在(e)&&(e>=N||代表[e]==e)) {
            targets.push_back(e);if(范围包含(有效存在概念(e),需求构造依据->信息->人类概念))subjects.push_back(e);
        }
        if(subjects.empty())需求构造缺口="recognized demand has no constructible human subject";
        std::map<std::size_t,std::vector<新需求目标定义>> goals;
        bool 有数值FT=false;
        for(const auto e:targets) {
            std::set<稳定编码> types;
            for(std::size_t f=0;f<原型数量;++f)if(defs[f]&&base[f].所属存在参数==e+1)types.insert(defs[f]->特征类型根);
            for(const auto FT:types) {
                const auto d=方法_.读取模拟特征范围(FT);if(!d)throw std::runtime_error("demand target FT disappeared");
                for(const auto direction:directions) {
                    if(direction!=1&&direction!=2)不支持(0,"formal demand direction value has no domain meaning");
                    goals[e].push_back({FT,新需求目标形式::变化方向,{},{},static_cast<新需求变化方向>(direction)});
                }
                if(d->材料类型!=特征概念材料物理类型::I64标量||d->比较规则!=特征概念比较规则::I64数值)continue;
                const auto* ranges=std::get_if<std::vector<特征概念I64闭区间>>(&d->值域);if(!ranges||ranges->empty())continue;
                有数值FT=true;
                for(const auto& sample:生成通用模拟整数候选(*ranges)) {
                    const auto FC=方法_.建立模拟I64目标域(FT,std::get<std::int64_t>(sample));
                    const std::vector<二次特征概念输入定义> in{{0,二次特征输入角色::左输入,二次特征输入来源种类::特征准确值,FT},
                        {1,二次特征输入角色::右输入,二次特征输入来源种类::特征准确值,FT}};
                    const auto r=二次概念_.建立或取得默认二次特征概念(二次特征算法::有向特征差值,in,FC);
                    if(!r.概念节点)throw std::runtime_error("simulation goal comparison root construction failed");
                    const auto compare=二次概念_.建立或取得参照结果域概念(*r.概念节点,二次特征结果域种类::包含于参照值域);
                    if(!compare.概念节点)throw std::runtime_error("simulation goal comparison construction failed");
                    goals[e].push_back({FT,新需求目标形式::值,FC,*compare.概念节点,{}});
                }
            }
        }
        // 概念建立可能改变入向关系：全部定义建立结束后统一捕获，不拼旧保护。
        定义保护.clear();共享定义保护.clear();加正式保护(方法_.读取模拟正式结构(concepts.信息->需求).完整保护组);
        for(const auto C:{valueC,directionC,recognizedC})加正式保护(方法_.读取模拟正式结构(C).完整保护组);
        for(const auto role:{408u,411u,413u,414u})(void)内容概念(role);
        if(!有数值FT)需求构造缺口="value target lacks a legal I64 FT in the formal prototype closure";
        for(std::size_t f=0;f<原型数量;++f)if((defs[f]&&defs[f]->材料类型==特征概念材料物理类型::需求节点引用
            &&std::holds_alternative<特征概念全材料值域>(defs[f]->值域)&&!base[f].复制上下文)
            ||(!需求参数位置.empty()&&f==*需求参数位置.begin())) {
            if(!有数值FT)continue;
            for(const auto subject:subjects)for(const auto& [target,targetGoals]:goals)for(const auto& t:targetGoals) {
                保护目标(t);
                const auto ft=概念_.获取特征概念(t.目标特征类型概念节点);if(!ft)throw std::runtime_error("target feature semantic definition unavailable");
                服务要求目标语义快照 semantic{{ft->节点,ft->材料类型,ft->单位,ft->比较规则}};
                if(t.目标形式==新需求目标形式::值) {
                    const auto fc=概念_.获取特征概念(*t.目标值域概念节点);const auto rc=二次概念_.获取二次特征概念(*t.目标比较关系概念节点);
                    if(!fc||!rc)throw std::runtime_error("target semantic domain/comparison unavailable");
                    semantic.目标域=服务要求值域语义快照{{fc->节点,fc->材料类型,fc->单位,fc->比较规则},fc->值域};
                    semantic.比较=服务要求比较语义快照{rc->节点,rc->二次特征类型根节点,rc->算法,rc->输入定义组,rc->参照特征概念节点,
                        {{rc->输出特征概念.节点,rc->输出特征概念.材料类型,rc->输出特征概念.单位,rc->输出特征概念.比较规则},rc->输出特征概念.值域},rc->结果域种类组};
                }
                for(const auto activityValue:activities) {
                    if(activityValue<1||activityValue>4)不支持(0,"formal demand activity value has no supported domain meaning");
                    const auto activity=static_cast<新需求活动状态>(activityValue);
                    const auto D=static_cast<std::uint32_t>(需求图模板.需求组.size()+1),U=static_cast<std::uint32_t>(需求图模板.理解组.size()+1),
                        C=static_cast<std::uint32_t>(需求图模板.确认组.size()+1),R=static_cast<std::uint32_t>(需求图模板.要求组.size()+1),L=static_cast<std::uint32_t>(需求图模板.语言场景组.size()+1);
                    新特征匹配定义快照 textDef;textDef.材料类型=特征概念材料物理类型::UTF8字符串;
                    textDef.比较规则=特征概念比较规则::完整材料精确;textDef.值域=特征概念全材料值域{};
                    const auto text=生成通用模拟UTF8候选(textDef,true,{},m.节点.值+D);
                    if(text.状态!=通用UTF8取样状态::已生成||text.候选.empty())throw std::runtime_error("generic nonempty UTF8 construction failed");
                    const auto session=text.候选.front()+std::to_string(D);
                    新模拟需求输入 d;d.序号=D;d.主体存在输入=static_cast<std::uint32_t>(subject+1);d.目标存在输入=static_cast<std::uint32_t>(target+1);
                    d.内容概念=concepts.信息->需求;d.附加内容概念组={t.目标形式 == 新需求目标形式::值 ? valueC : directionC,recognizedC};d.目标=t;
                    d.执行域=新需求执行域::被认知;d.活动状态=activity;d.理解输入=U;d.确认输入=C;d.来源要求输入=R;
                    需求图模板.需求组.push_back(d);需求图模板.理解组.push_back({U,L,d.主体存在输入,d.目标存在输入,R,session,t,semantic,{}});
                    需求图模板.确认组.push_back({C,U,false,{}});
                    新模拟要求输入 r;r.序号=R;r.来源存在输入=d.主体存在输入;r.目标存在输入=d.目标存在输入;r.公开会话标识=session;r.来源要求标识=session;
                    r.已知目标={{},t.目标特征类型概念节点,t.目标形式,t.目标值域概念节点,t.目标比较关系概念节点,t.目标变化方向};r.理解输入=U;r.确认输入=C;
                    需求图模板.要求组.push_back(std::move(r));新模拟语言场景输入 l;l.序号=L;需求图模板.语言场景组.push_back(std::move(l));需求候选序号[f].push_back(D);
                }
            }
        }
      } catch(const 新学习运行异常& e) {
        if(e.记录().原因!=新学习运行异常原因::规格未支持)throw;
        // 备用构造未支持不抹掉已合法复制/构造的完整D；资源、漂移及工程错误不降格。
        需求构造缺口=e.记录().诊断;
      }
    }
    // 独立D参数可以选择任何已合法采集/构造的完整D，不从F.current猜选中身份。
    for(const auto i:需求参数位置) {
        auto& candidates=需求候选序号[i];
        candidates.clear();
        for(const auto& d:需求图模板.需求组)candidates.push_back(d.序号);
        std::sort(candidates.begin(),candidates.end());
        candidates.erase(std::unique(candidates.begin(),candidates.end()),candidates.end());
        if(candidates.empty())不支持(static_cast<std::uint32_t>(i+1),需求构造缺口.empty()
            ? "formal demand parameter has no complete source or constructible candidate" : 需求构造缺口);
    }
    // owner逐项采集不是跨世界原子快照。读回检测采集期间变化；之后只用值副本。
    HY_SIM_PROGRESS(L"需求图构造完成，开始核同来源",需求图模板.需求组.size());
    for(const auto& [E,e]:来源存在快照)if(方法_.读取模拟来源存在(E)!=e)
        throw 模拟输入采集漂移{};
    for(const auto& [F,f]:来源特征快照)if(方法_.读取模拟来源特征(F)!=f)
        throw 模拟输入采集漂移{};
    for(const auto& [F,f]:来源完整特征快照)if(方法_.读取模拟来源完整特征(F)!=f)
        throw 模拟输入采集漂移{};
    for(const auto& [id,old]:D来源) {
        const auto fresh=业务来源_.读取需求来源(id);
        if(fresh.状态==新模拟业务来源状态::来源已变化||fresh.状态==新模拟业务来源状态::来源不存在)throw 模拟输入采集漂移{};
        if(fresh.状态==新模拟业务来源状态::尚缺依赖提供者)不支持(0,fresh.诊断);
        if(fresh.状态!=新模拟业务来源状态::已读取||!fresh.信息)throw std::runtime_error("fresh typed demand owner unavailable");
        if(fresh.信息->治理!=old.治理||fresh.信息->完整需求!=old.完整需求)
            throw 模拟输入采集漂移{};
        加保护(fresh.完整保护);
        if(old.治理.外部要求材料) {
            const auto r=R来源.find(old.治理.外部要求材料->要求节点);
            if(r==R来源.end()||r->second!=*old.治理.外部要求材料)throw std::runtime_error("external requirement projection differs from typed owner");
        }
    }
    for(const auto& [id,old]:U来源){const auto read=业务来源_.读取模拟理解来源(id);if(sourceRead(read,"understanding")!=old)throw 模拟输入采集漂移{};}
    for(const auto& [id,old]:C来源){const auto read=业务来源_.读取模拟确认来源(id);if(sourceRead(read,"confirmation")!=old)throw 模拟输入采集漂移{};}
    for(const auto& [id,old]:R来源){const auto read=业务来源_.读取模拟要求来源(id);if(sourceRead(read,"requirement")!=old)throw 模拟输入采集漂移{};}
    for(const auto& [id,old]:L来源) {
        const auto read=业务来源_.读取语言场景来源(id);
        if(read.状态==新自然语言场景结构比较状态::资源失败)throw std::bad_alloc{};
        if(read.状态==新自然语言场景结构比较状态::前提已变化||read.状态==新自然语言场景结构比较状态::场景不存在)throw 模拟输入采集漂移{};
        if(read.状态==新自然语言场景结构比较状态::复制内容未支持)不支持(0,"fresh source language scene content became unsupported");
        if(read.状态!=新自然语言场景结构比较状态::已完成||!read.信息)throw std::runtime_error("fresh language scene source engineering read failed");
        if(read.信息->场景!=old.场景||read.信息->概念实例组!=old.概念实例组||read.信息->普通成员存在组!=old.普通成员存在组)
            throw 模拟输入采集漂移{};加保护(read.完整保护);
    }
    const auto 保护内容定义=[&](稳定编码 C) {
        const auto p=方法_.读取模拟正式结构(C);
        if(p.状态==正式结构操作状态::前提已变化)throw 模拟输入采集漂移{};
        if(p.状态!=正式结构操作状态::已读取||!p.信息)throw std::runtime_error("typed content formal definition unavailable");
        加正式保护(p.完整保护组);
    };
    if(需求构造依据) {
        const auto fresh=业务来源_.读取模拟构造依据();
        if(fresh.状态==新模拟需求状态::前提已变化)throw 模拟输入采集漂移{};
        if(fresh.状态==新模拟需求状态::资源失败)throw std::bad_alloc{};
        if(fresh.状态!=新模拟需求状态::已完成||!fresh.信息)throw std::runtime_error("human construction definition unavailable");
        if(fresh.信息->人类概念!=需求构造依据->信息->人类概念)throw 模拟输入采集漂移{};
        加正式保护(fresh.完整保护);
    }
    for(const auto& d:需求图模板.需求组){保护目标(d.目标);保护内容定义(d.内容概念);for(const auto C:d.附加内容概念组)保护内容定义(C);}
    for(const auto& u:需求图模板.理解组)保护引用定义(u.目标);
    for(const auto& r:需求图模板.要求组){新需求目标定义 t;t.目标特征类型概念节点=r.已知目标.目标特征类型概念节点.value_or(稳定编码{});t.目标值域概念节点=r.已知目标.目标值域概念节点;t.目标比较关系概念节点=r.已知目标.目标比较关系概念节点;保护引用定义(t);}
    if(!定义保护.empty()) {
        std::vector<稳定编码> ids;for(const auto& [id,p]:定义保护)ids.push_back(id);
        const auto fresh=全局基础数据集.查询节点组完整结构(ids);
        if(fresh.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(fresh.状态!=基础结构操作状态::已完成||fresh.节点组.size()!=ids.size())throw std::runtime_error("input graph final protection read failed");
        for(const auto& n:fresh.节点组) {
            const auto& old=定义保护.at(n.查询节点);if(!n.快照||old.结构.字段组!=n.快照->结构.字段组||old.入向字段组!=n.快照->入向字段组
                ||old.入向外部关系组!=n.快照->入向外部关系组||old.出向外部关系组!=n.快照->出向外部关系组)throw 模拟输入采集漂移{};
        }
    }
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i&&defs[i]) {
        const auto spec=base[i].规格;base[i]=base[代表[i]];base[i].规格=spec;
        base[i].同一参数=static_cast<std::uint32_t>(代表[i]+1);defs[i]=defs[代表[i]];
    }
    for(const auto 任务:任务顺序){
        停止();const auto 复读=业务来源_.读取任务来源(任务);
        if(复读.状态==新模拟业务来源状态::来源已变化)throw 模拟输入采集漂移{};
        if(复读.状态!=新模拟业务来源状态::已读取||!复读.信息)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"typed task final source unavailable: "+复读.诊断);
        const auto& 来源=任务来源.at(任务);
        if(复读.信息->任务!=来源.任务||复读.信息->虚拟存在!=来源.虚拟存在||复读.信息->治理特征!=来源.治理特征)
            throw 模拟输入采集漂移{};
    }
    for(const auto 任务:任务顺序)任务治理组.push_back({任务来源.at(任务).治理特征});
    const auto K=base.size(); // 到此闭包完成；轴开始后禁止扩容。
    std::vector<通用模拟候选轴> axes;
    const auto utf8=[&](const 新特征匹配定义快照& d,bool nonempty,std::uint64_t seed) {
        std::vector<新特征值信息> 材料;
        if(const auto* ids=std::get_if<std::vector<稳定编码>>(&d.值域))for(const auto id:*ids) {
            const auto v=方法_.读取模拟候选材料(id);
            if(!v)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"UTF8 finite material unavailable");
            材料.push_back(*v);
        }
        auto r=生成通用模拟UTF8候选(d,nonempty,材料,seed);
        if(r.状态!=通用UTF8取样状态::已生成)throw 新学习运行异常(m.节点,
            r.状态 == 通用UTF8取样状态::域未支持 ? 新学习运行异常原因::规格未支持 : 新学习运行异常原因::输入不合法,"UTF8 input domain has no supported legal candidate");
        return r.候选;
    };
    std::map<std::size_t,std::vector<std::string>> 文本候选;
    std::vector<std::vector<std::size_t>> axisFor(base.size());
    std::vector<bool> 集合选择(base.size(),false),引用选择(base.size(),false);
    for(const auto i:需求参数位置) {
        if(代表[i]!=i){axisFor[i]=axisFor[代表[i]];continue;}
        std::vector<通用模拟候选值> values;
        for(const auto id:需求候选序号[i])values.emplace_back(static_cast<std::int64_t>(id));
        axisFor[i].push_back(axes.size());
        axes.push_back({static_cast<std::uint32_t>(axes.size()+1),
            生成通用模拟枚举候选(std::move(values),m.节点.值+i)});
    }
    for(std::size_t i=0;i<K;++i) if(defs[i]) {
        if(base[i].复制上下文) continue;
        if(i<N&&代表[i]!=i){axisFor[i]=axisFor[代表[i]];集合选择[i]=集合选择[代表[i]];引用选择[i]=引用选择[代表[i]];continue;}
        if (const auto* 结构 = std::get_if<特征概念结构化I64值域>(&defs[i]->值域)) {
            if (defs[i]->材料类型 != 特征概念材料物理类型::I64数组)
                不支持(static_cast<std::uint32_t>(i+1),"structured non-array material needs payload shape constraints");
            if (结构->分量.empty())
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"empty structured integer domain");
            for (std::size_t 分量 = 0; 分量 < 结构->分量.size(); ++分量) {
                axisFor[i].push_back(axes.size());
                axes.push_back({static_cast<std::uint32_t>(axes.size()+1),数值候选(i, 分量)});
            }
            continue;
        }
        std::vector<通用模拟候选值> values;
        if(defs[i]->材料类型==特征概念材料物理类型::需求节点引用) {
            bool 允许未知=true;
            const auto 同簇=[&](std::uint32_t 位置){return 位置&&位置<=N&&代表[位置-1]==(i<N ? 代表[i] : i);};
            const auto 核条件=[&](const 新方法条件项定义& 条件) {
                if(同簇(条件.参数序号)) {
                    auto 仅类型=新方法条件项定义{};仅类型.参数序号=条件.参数序号;仅类型.特征类型概念节点=条件.特征类型概念节点;
                    if(条件.特征前态限定&&条件.特征前态限定->当前值状态==新方法当前值状态::未知
                        &&条件.特征前态限定->历史不同值数量==std::optional<std::uint64_t>{0})
                        仅类型.特征前态限定=条件.特征前态限定;
                    if(条件!=仅类型)允许未知=false;
                }
                if(条件.参数序号&&条件.参数序号<=N&&base[i].所属存在参数
                    &&代表[条件.参数序号-1]==代表[base[i].所属存在参数-1]
                    &&条件.特征类型概念节点==std::optional{defs[i]->特征类型根})允许未知=false;
            };
            if(m.初始必要条件)for(const auto& 条件:m.初始必要条件->条件.条件项组)核条件(条件);
            for(const auto& 配对:m.条件结果组)for(const auto& 条件:配对.条件.条件项组)核条件(条件);
            for(const auto& 关系:初始关系组)if((同簇(关系.左参数序号)||同簇(关系.右参数序号))
                &&关系.种类!=新方法参数关系种类::同一&&关系.种类!=新方法参数关系种类::不同)允许未知=false;
            if(允许未知)values.emplace_back(std::int64_t{0}); // 选择未知，不是节点或特征准确值。
            for(const auto id:需求候选序号[i])values.emplace_back(static_cast<std::int64_t>(id));
            if(values.empty())不支持(static_cast<std::uint32_t>(i+1),"single demand range has no legal constructed/copied candidate");
            values=生成通用模拟枚举候选(std::move(values),m.节点.值+i);
        } else if(defs[i]->材料类型==特征概念材料物理类型::UTF8字符串) {
            auto text=utf8(*defs[i],false,m.节点.值+i);
            for(std::size_t j=0;j<text.size();++j)values.emplace_back(static_cast<std::int64_t>(j));
            文本候选.emplace(i,std::move(text));
        } else if(const auto* 引用=std::get_if<特征概念存在引用值域>(&defs[i]->值域)) {
            引用选择[i]=true;
            for(std::size_t j=0;j<K;++j)
                if(是普通存在(j)
                    &&(j>=N||代表[j]==j)
                    &&范围包含(有效存在概念(j),引用->存在概念节点))
                    values.emplace_back(static_cast<std::int64_t>(j+1));
            if(values.empty())不支持(static_cast<std::uint32_t>(i+1),"existence reference has no expressible formal existence parameter");
            for(const auto& relation:初始关系组)
                if(relation.种类==新方法参数关系种类::特征引用存在&&代表[relation.左参数序号-1]==i) {
                    const auto required=static_cast<std::int64_t>(代表[relation.右参数序号-1]+1);
                    std::erase_if(values,[&](const auto& v){return std::get<std::int64_t>(v)!=required;});
                }
            if(values.empty())throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,
                "initial existence-reference relations have no legal candidate");
            values=生成通用模拟枚举候选(std::move(values),m.节点.值+i);
        }
        else if(std::holds_alternative<std::vector<特征概念I64闭区间>>(defs[i]->值域))
            values=数值候选(i, std::nullopt);
        else if(const auto* r=std::get_if<std::vector<稳定编码>>(&defs[i]->值域)) {
            for(auto v:*r)values.emplace_back(v);
            values=生成通用模拟枚举候选(std::move(values),m.节点.值+i);
        } else if(const auto* r=std::get_if<特征概念存在集合值域>(&defs[i]->值域)) {
            // R102：概念成员同样要求非空集合（规范化后“至少一个”已归零），不能用空集原型代替。
            if(r->至少一个存在||!r->必含存在节点组.empty()||!r->含概念成员组.empty()) 不支持(static_cast<std::uint32_t>(i+1),"referenced existence collection needs isolated binding");
            values.emplace_back(std::int64_t{0}); // 空集合原型的本地选择号，不是特征的数值。
        } else if (std::holds_alternative<特征概念全材料值域>(defs[i]->值域)
            &&(defs[i]->材料类型==特征概念材料物理类型::UTF8字符串
                ||defs[i]->材料类型==特征概念材料物理类型::U64数组)) {
            values.emplace_back(std::int64_t{0}); // 带下面物理分支的纯材料选择号，非节点编码。
        } else { 不支持(static_cast<std::uint32_t>(i+1),"range has no finite candidate declaration"); }
        axisFor[i].push_back(axes.size());
        axes.push_back({static_cast<std::uint32_t>(axes.size()+1),std::move(values)});
    }
    std::vector<std::size_t> eventAxes;
    for(const auto pos:m.初始模拟事件参数组) {
        if(!pos||pos>N||!defs[pos-1])throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "simulation event parameter position invalid");
        // 每个事件按其声明参数的原始范围取样；别名共享F身份，不要求先后事件值相同。
        const auto& 事件定义 = std::holds_alternative<特征概念全材料值域>(原始参数定义[pos-1]->值域)
            ? *defs[pos-1] : *原始参数定义[pos-1];
        const auto* domain=std::get_if<std::vector<特征概念I64闭区间>>(&事件定义.值域);
        if(!domain) 不支持(pos,"event parameter is not an integer domain");
        eventAxes.push_back(axes.size());
        axes.push_back({static_cast<std::uint32_t>(axes.size()+1),生成通用模拟整数候选(*domain)});
    }
    std::vector<std::vector<std::string>> 消息正文候选;
    std::vector<std::size_t> 消息正文轴;
    if(消息方案) {
        if(*消息方案>=m.初始模拟输入方案组.size())throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"message input plan absent");
        for(const auto& e:m.初始模拟输入方案组[*消息方案].消息组) {
            const auto d=方法_.读取模拟特征范围(e.正文范围概念);
            if(!d)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"message payload domain unavailable");
            auto text=utf8(*d,e.正文非空,m.节点.值+e.顺序);
            std::vector<通用模拟候选值> choices;
            for(std::size_t j=0;j<text.size();++j)choices.emplace_back(static_cast<std::int64_t>(j));
            消息正文轴.push_back(axes.size());消息正文候选.push_back(std::move(text));
            axes.push_back({static_cast<std::uint32_t>(axes.size()+1),std::move(choices)});
        }
    }
    // 轴生成也会查存在范围；待所有归属调用结束再统一核同并并入world的原子发表前提。
    for(const auto& [C,captured]:存在归属保护) {
        const auto fresh=读取归属语义(C);
        if(*fresh.信息!=captured.概念)throw 模拟输入采集漂移{};
        const auto status=全局基础数据集.核验只读预期(captured.只读预期);
        if(status==基础节点组追加状态::预期不符||status==基础节点组追加状态::节点不存在
            ||status==基础节点组追加状态::引用不存在)throw 模拟输入采集漂移{};
        if(status==基础节点组追加状态::资源失败)throw std::bad_alloc{};
        if(status!=基础节点组追加状态::已完成)throw std::runtime_error("existence semantic readset verification failed");
        auto& target=需求图模板.存在范围语义保护;
        target.节点组.insert(target.节点组.end(),captured.只读预期.节点组.begin(),captured.只读预期.节点组.end());
        target.字段集合组.insert(target.字段集合组.end(),captured.只读预期.字段集合组.begin(),captured.只读预期.字段集合组.end());
        target.外部关系集合组.insert(target.外部关系集合组.end(),captured.只读预期.外部关系集合组.begin(),captured.只读预期.外部关系集合组.end());
    }
    需求图模板.正式定义保护.clear();
    // 来源事实的完整读取依据必须一直送到world的同锁提交点，不能只在采集阶段核一次。
    for(const auto& [C,p]:定义保护)需求图模板.正式定义保护.push_back(p);
    // 只替换owner已授权的存在范围C本身；不能把readset中的schema/端点身份当作定义替代证明。
    // 来源事实的独立full仍留在定义保护中，绝不按编码全局删除。
    for(const auto& [C,p]:共享定义保护)if(!存在归属保护.contains(C)) {
        const auto fresh=全局基础数据集.查询节点组完整结构({C});
        if(fresh.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(fresh.状态!=基础结构操作状态::已完成||fresh.节点组.size()!=1)
            throw std::runtime_error("shared definition final protection read failed");
        if(!fresh.节点组.front().快照||!归属根相同(p,*fresh.节点组.front().快照))throw 模拟输入采集漂移{};
        需求图模板.正式定义保护.push_back(p);
    }
    const auto 选择需求图=[&](std::vector<新模拟有序参数输入>& input) {
        std::array<std::set<std::uint32_t>,5> keep;
        std::vector<std::pair<std::size_t,std::uint32_t>> todo;
        const auto add=[&](std::size_t type,std::uint32_t id){if(id&&keep[type].insert(id).second)todo.push_back({type,id});};
        const auto valueRefs=[&](const 新模拟复制准确值& v) {
            if(v.种类==新模拟复制值种类::需求引用)add(0,std::get<std::uint32_t>(v.内容));
            else if(v.种类==新模拟复制值种类::需求集合)for(const auto d:std::get<std::vector<std::uint32_t>>(v.内容))add(0,d);
        };
        const auto domainRefs=[&](const std::optional<新模拟复制值域>& d) {
            if(d)if(const auto* values=std::get_if<std::vector<新模拟复制准确值>>(&d->内容))for(const auto& v:*values)valueRefs(v);
        };
        for(const auto& p:input) {
            add(0,p.引用需求输入);add(0,p.需求参数输入);add(4,p.引用语言场景输入);add(4,p.所属语言场景输入);
            if(p.完整复制特征) {
                const auto& f=*p.完整复制特征;if(f.当前值)valueRefs(*f.当前值);if(f.标准值)valueRefs(*f.标准值);
                for(const auto& v:f.历史不同值)valueRefs(v);for(const auto& h:f.历史值命中信息)valueRefs(h.值);
                domainRefs(f.实例区间);domainRefs(f.历史聚合);
            }
        }
        for(std::size_t next=0;next<todo.size();++next) {
            const auto [kind,id]=todo[next];
            switch(kind) {
            case 0:{const auto& d=需求图模板.需求组.at(id-1);if(d.父需求输入)add(0,*d.父需求输入);add(1,d.理解输入);add(2,d.确认输入);add(3,d.来源要求输入);add(3,d.外部要求输入);for(const auto c:d.服务要求确认输入组)add(2,c);break;}
            case 1:{const auto& u=需求图模板.理解组.at(id-1);add(4,u.语言场景输入);add(3,u.来源要求输入);break;}
            case 2:add(1,需求图模板.确认组.at(id-1).理解输入);break;
            case 3:{const auto& r=需求图模板.要求组.at(id-1);add(1,r.理解输入);add(2,r.确认输入);break;}
            case 4:(void)需求图模板.语言场景组.at(id-1);break;
            }
        }
        std::array<std::map<std::uint32_t,std::uint32_t>,5> maps;
        for(std::size_t kind=0;kind<5;++kind){std::uint32_t pos=0;for(const auto id:keep[kind])maps[kind].emplace(id,++pos);}
        const auto remap=[&](std::size_t kind,std::uint32_t x){return x?maps[kind].at(x):0u;};
        新模拟需求输入图 graph;graph.正式定义保护=需求图模板.正式定义保护;
        graph.存在范围语义保护=需求图模板.存在范围语义保护;
        for(const auto id:keep[0]) {
            auto d=需求图模板.需求组.at(id-1);d.序号=remap(0,id);if(d.父需求输入)d.父需求输入=remap(0,*d.父需求输入);
            d.理解输入=remap(1,d.理解输入);d.确认输入=remap(2,d.确认输入);d.来源要求输入=remap(3,d.来源要求输入);d.外部要求输入=remap(3,d.外部要求输入);
            for(auto& c:d.服务要求确认输入组)c=remap(2,c);graph.需求组.push_back(std::move(d));
        }
        for(const auto id:keep[1]){auto u=需求图模板.理解组.at(id-1);u.序号=remap(1,id);u.语言场景输入=remap(4,u.语言场景输入);u.来源要求输入=remap(3,u.来源要求输入);graph.理解组.push_back(std::move(u));}
        for(const auto id:keep[2]){auto c=需求图模板.确认组.at(id-1);c.序号=remap(2,id);c.理解输入=remap(1,c.理解输入);graph.确认组.push_back(std::move(c));}
        for(const auto id:keep[3]){auto r=需求图模板.要求组.at(id-1);r.序号=remap(3,id);r.理解输入=remap(1,r.理解输入);r.确认输入=remap(2,r.确认输入);graph.要求组.push_back(std::move(r));}
        for(const auto id:keep[4]){auto l=需求图模板.语言场景组.at(id-1);l.序号=remap(4,id);graph.语言场景组.push_back(std::move(l));}
        const auto remapValue=[&](新模拟复制准确值& v) {
            if(v.种类==新模拟复制值种类::需求引用)v.内容=remap(0,std::get<std::uint32_t>(v.内容));
            else if(v.种类==新模拟复制值种类::需求集合){auto ids=std::get<std::vector<std::uint32_t>>(v.内容);for(auto& id:ids)id=remap(0,id);v.内容=std::move(ids);}
        };
        const auto remapDomain=[&](std::optional<新模拟复制值域>& domain) {
            if(domain)if(auto* values=std::get_if<std::vector<新模拟复制准确值>>(&domain->内容))for(auto& v:*values)remapValue(v);
        };
        for(auto& p:input) {
            p.引用需求输入=remap(0,p.引用需求输入);p.需求参数输入=remap(0,p.需求参数输入);p.引用语言场景输入=remap(4,p.引用语言场景输入);p.所属语言场景输入=remap(4,p.所属语言场景输入);
            if(p.完整复制特征){auto& f=*p.完整复制特征;if(f.当前值)remapValue(*f.当前值);if(f.标准值)remapValue(*f.标准值);for(auto& v:f.历史不同值)remapValue(v);for(auto& h:f.历史值命中信息)remapValue(h.值);remapDomain(f.实例区间);remapDomain(f.历史聚合);}
        }return graph;
    };
    通用模拟组合游标 cursor(std::move(axes));
    HY_SIM_PROGRESS(L"参数轴与组合游标准备完成",K);
    bool 本支有合法样例=false;
    bool 本支有正式结果=false;
    bool 本支有未完成样例=false;
    while(!cursor.已结束()) {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        ++观察.样例次数;
#endif
        HY_SIM_PROGRESS(L"开始准备当前组合样例",base.size());
        停止();
        const auto values=cursor.读取当前组合();
        auto input=base;
        for(const auto i:需求参数位置)
            input[i].需求参数输入=static_cast<std::uint32_t>(std::get<std::int64_t>(values[axisFor[i].front()]));
        bool 需求关系冲突=false;
        for(const auto& r:初始关系组)if(r.种类==新方法参数关系种类::不同
            &&是需求参数(r.左参数序号-1)&&是需求参数(r.右参数序号-1)
            &&input[r.左参数序号-1].需求参数输入==input[r.右参数序号-1].需求参数输入)需求关系冲突=true;
        if(需求关系冲突){cursor.推进();continue;}
        for(std::size_t i=0;i<K;++i) if(defs[i]) {
            if(base[i].复制上下文) continue;
            if (std::holds_alternative<特征概念结构化I64值域>(defs[i]->值域)) {
                std::vector<std::int64_t> 分量;
                分量.reserve(axisFor[i].size());
                for (const auto 轴 : axisFor[i]) 分量.push_back(std::get<std::int64_t>(values[轴]));
                input[i].材料 = 新特征值材料{std::move(分量)};
                continue;
            }
            const auto& x=values[axisFor[i].front()];
            if(defs[i]->材料类型==特征概念材料物理类型::需求节点引用) {
                input[i].引用需求输入=static_cast<std::uint32_t>(std::get<std::int64_t>(x));
                if(!input[i].引用需求输入)input[i].初值方式=新模拟特征初值方式::概念构造未知;
            }
            else if (defs[i]->材料类型==特征概念材料物理类型::UTF8字符串)
                input[i].材料=新特征值材料{文本候选.at(i < N ? 代表[i] : i).at(static_cast<std::size_t>(std::get<std::int64_t>(x)))};
            else if (std::holds_alternative<特征概念全材料值域>(defs[i]->值域)
                &&defs[i]->材料类型==特征概念材料物理类型::U64数组)
                input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
            else if(引用选择[i])input[i].引用存在参数=static_cast<std::uint32_t>(std::get<std::int64_t>(x));
            else if(defs[i]->材料类型==特征概念材料物理类型::I64标量)input[i].值=std::get<std::int64_t>(x);
            else if(集合选择[i]) {
                input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
                input[i].集合成员参数组.push_back(static_cast<std::uint32_t>(std::get<std::int64_t>(x)));
            }
            else if(const auto* v=std::get_if<稳定编码>(&x)) {
                const auto material=方法_.读取模拟候选材料(*v);
                if(!material)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                    "selected candidate material unavailable");
                // 源成员只通过本地参数位置进入样例；数组中的源编码不进入模拟图。
                if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等) {
                    const auto* a=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                    if(!a)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                        "selected existence collection material is not U64 array");
                    for(const auto E:*a)input[i].集合成员参数组.push_back(static_cast<std::uint32_t>(来源存在位置.at(稳定编码{E})+1));
                    input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
                    continue;
                }
                if(defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
                    const auto* a=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                    if(!a)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                        "selected task collection material is not U64 array");
                    for(const auto T:*a)input[i].来源任务成员组.push_back(稳定编码{T});
                    input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
                    continue;
                }
                if(defs[i]->比较规则==特征概念比较规则::需求集合身份判等) {
                    const auto* a=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                    if(!a)throw std::runtime_error("selected demand collection material is not U64 array");
                    std::vector<std::uint32_t> ids;for(const auto D:*a)ids.push_back(D位置.at(稳定编码{D}));
                    新模拟完整特征复制输入 copy;copy.当前值=新模拟复制准确值{新模拟复制值种类::需求集合,std::move(ids)};
                    input[i].完整复制特征=std::move(copy);input[i].初值方式=新模拟特征初值方式::复制准确;
                    input[i].复制上下文=true;input[i].值.reset();input[i].材料.reset();continue;
                }
                input[i].材料=material->材料;
            } else input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
        }
        std::vector<新模拟输入事件> events;
        for(std::size_t i=0;i<eventAxes.size();++i)
            events.push_back({m.初始模拟事件参数组[i],std::get<std::int64_t>(values[eventAxes[i]])});
        auto world=动作定义_&&动态定义_&&动态概念_&&场景构成_
            ? std::make_shared<新_模拟世界类>(方法_,*动作定义_,*动态定义_,*动态概念_,*场景构成_)
            : 动作定义_&&动态定义_&&动态概念_
                ? std::make_shared<新_模拟世界类>(方法_,*动作定义_,*动态定义_,*动态概念_)
                : std::make_shared<新_模拟世界类>(方法_);
        world->配置二次特征计算(二次计算_);
        world->设置字段询问语言(字段询问语言_,字段询问配置_);
        std::optional<新模拟消息方案输入> 消息输入;
        if(消息方案) {
            消息输入=新模拟消息方案输入{*消息方案,{}};
            for(std::size_t j=0;j<消息正文轴.size();++j)消息输入->消息组.push_back({static_cast<std::uint32_t>(j),
                消息正文候选[j].at(static_cast<std::size_t>(std::get<std::int64_t>(values[消息正文轴[j]])))});
        }
        for(std::size_t i=0;i<input.size();++i)if(defs[i]&&defs[i]->比较规则==特征概念比较规则::消息集合身份判等&&input[i].材料) {
            const auto* a=std::get_if<std::vector<std::uint64_t>>(&*input[i].材料);
            if(!a||!a->empty())不支持(static_cast<std::uint32_t>(i+1),"nonempty source message references have no isolated remapping provider");
        }
        const auto 本组合需求图=选择需求图(input);
        const bool 有内容续学=std::any_of(m.条件结果组.begin(),m.条件结果组.end(),[](const auto& p){
            return std::any_of(p.条件.条件项组.begin(),p.条件.条件项组.end(),[](const auto& c){return c.引用内容条件.has_value();});});
        if(有内容续学) {
            const bool 落入已有范围=std::any_of(m.条件结果组.begin(),m.条件结果组.end(),[&](const auto& pair){
                bool 有内容=false;
                for(const auto& c:pair.条件.条件项组) {
                    if(!c.参数序号||c.参数序号>N)return false;
                    const auto& p=input[c.参数序号-1];
                    if(c.引用内容条件) {
                        有内容=true;if(!p.需求参数输入)return false;
                        if(c.引用内容条件->要求认知来源当前有效) {
                            const auto D=std::find_if(本组合需求图.需求组.begin(),本组合需求图.需求组.end(),
                                [&](const auto& x){return x.序号==p.需求参数输入;});
                            if(D==本组合需求图.需求组.end()||!D->确认输入)return false;
                            const auto C=std::find_if(本组合需求图.确认组.begin(),本组合需求图.确认组.end(),
                                [&](const auto& x){return x.序号==D->确认输入;});
                            if(C==本组合需求图.确认组.end()||C->需重新确认)return false;
                        }
                    }
                    if(c.特征前态限定) {
                        if(p.初值方式!=新模拟特征初值方式::概念构造未知) {
                            if(!p.完整复制特征||p.完整复制特征->当前值||!p.完整复制特征->历史不同值.empty()
                                ||!p.完整复制特征->历史值命中信息.empty())return false;
                        }
                    }
                }
                for(const auto& r:pair.条件.参数关系组)if(r.种类==新方法参数关系种类::内容引用参数) {
                    if(!需求主体FT||r.左正式内容路径!=std::vector{*需求主体FT})return false;
                    const auto id=input[r.左参数序号-1].需求参数输入;
                    const auto d=std::find_if(本组合需求图.需求组.begin(),本组合需求图.需求组.end(),[&](const auto& x){return x.序号==id;});
                    if(d==本组合需求图.需求组.end()||!d->主体存在输入)return false;
                    const auto subject=d->主体存在输入-1;
                    if((subject<N ? 代表[subject] : subject)!=代表[r.右参数序号-1])return false;
                }
                return 有内容;
            });
            if(!落入已有范围){cursor.推进();continue;}
        }
        HY_SIM_PROGRESS(L"即将建立隔离样例",本组合需求图.需求组.size());
        std::optional<新模拟执行主体输入> 执行主体;
        if(需求结果概念) {
            world->配置需求执行读取(*来源身份_,*存在概念_,概念_,二次概念_);
            const auto 身份=来源身份_->读取存在身份保护(需求执行来源_);
            if(身份.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(身份.前提已变化||身份.状态==基础结构操作状态::节点不存在)throw 模拟输入采集漂移{};
            if(身份.状态!=基础结构操作状态::已完成||!身份.信息||身份.预期节点组.empty())
                throw std::logic_error("configured simulation actor identity unreadable");
            const auto 类别=存在概念_->读取普通存在概念定义保护(身份.信息->专属存在概念节点);
            if(类别.状态==正式概念读取状态::前提已变化)throw 模拟输入采集漂移{};
            if(类别.状态!=正式概念读取状态::已读取||!类别.信息||类别.信息->种类!=存在概念种类::专属
                ||类别.信息->对应存在节点!=std::optional{需求执行来源_}||类别.完整保护组.empty())
                throw std::logic_error("configured simulation actor concept identity unreadable");
            新模拟执行主体输入 主体{需求执行来源_,身份.信息->专属存在概念节点,身份.预期节点组};
            主体.来源身份保护.insert(主体.来源身份保护.end(),类别.完整保护组.begin(),类别.完整保护组.end());
            for(std::size_t 位置=0;位置<input.size();++位置)if(input[位置].来源存在==std::optional{需求执行来源_}
                &&input[位置].规格.信息类型==新方法参数信息类型::存在&&!input[位置].需求参数输入) {
                主体.同源存在输入=static_cast<std::uint32_t>(位置+1);break;
            }
            执行主体=std::move(主体);
        }
        const auto created=world->建立有序样例(m.节点,input,events,任务治理组,场景输入,执行分支,消息输入,本组合需求图,执行主体);
        HY_SIM_PROGRESS(L"隔离样例建立返回",created.状态);
        if(created.状态==新模拟世界状态::定义前提已变化)
            throw 模拟输入采集漂移{};
        if(created.状态!=新模拟世界状态::已完成)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "ordered sample construction rejected: state=" + std::to_string(static_cast<int>(created.状态))
            + " line=" + std::to_string(模拟样例拒绝行));
        本支有合法样例=true;
        const auto bindings=world->读取参数绑定();
        if(!bindings)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "ordered sample parameter bindings unavailable");
        for(std::size_t i=0;i<N;++i)if(input[i].引用存在参数) {
            const auto f=world->读取模拟特征((*bindings)[i].有序引用->信息节点);
            const auto* value=f&&f->当前值 ? std::get_if<稳定编码>(&*f->当前值) : nullptr;
            const auto pos=input[i].引用存在参数;
            // 候选位置按K枚举（含N之后私有追加存在）；N内用正式绑定，N外用世界记录的同次输入节点。
            const auto expected=!pos||pos>K ? std::optional<稳定编码>{}
                : pos<=N ? std::optional<稳定编码>{(*bindings)[pos-1].有序引用->信息节点}
                : world->读取有序输入节点(pos);
            if(!expected||!value||*value!=*expected)
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"local existence reference binding readback differs: i="
                    + std::to_string(i+1) + " pos=" + std::to_string(pos) + " N=" + std::to_string(N) + " K=" + std::to_string(K)
                    + " expected=" + std::to_string(expected ? expected->值 : 0) + " value=" + std::to_string(value ? value->值 : 0));
        }
        const auto 集合正式材料 = [&](const 新特征值信息& value,特征概念比较规则 rule) -> std::optional<新特征值信息> {
            const auto* members=std::get_if<std::vector<std::uint64_t>>(&value.材料);
            if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "knowledge collection material is not U64 array");
            std::vector<std::uint64_t> sources;
            bool 全为来源=true;
            for(const auto n:*members) {
                const auto source=rule==特征概念比较规则::服务任务集合身份判等
                    ?world->读取模拟任务来源(稳定编码{n}):world->读取模拟存在来源(稳定编码{n});
                if(source)sources.push_back(source->值);else 全为来源=false;
            }
            if(全为来源) {
                std::sort(sources.begin(),sources.end());
                sources.erase(std::unique(sources.begin(),sources.end()),sources.end());
                auto result=value;result.节点={};result.材料=std::move(sources);
                return result; // 只发表知识材料，不向任何源存在写入模拟结果。
            }
            if(rule==特征概念比较规则::服务任务集合身份判等)
                不支持(0,"task collection contains an unmapped member; existence-position relations cannot represent tasks");
            for(const auto n:*members) {
                const auto visible=std::any_of(bindings->begin(),bindings->end(),[&](const auto& b) {
                    return b.有序引用&&是普通存在(b.有序引用->序号-1)
                        &&b.有序引用->信息节点.值==n;
                });
                if(!visible)不支持(0,"mixed copied and synthetic collection cannot be fully expressed by current C/R positions");
            }
            return {}; // 每个成员均可通过正式有序参数关系表达。
        };
        auto context=方法_.建立模拟参数场景(m.节点,*bindings,world,{},执行分支);
        if(context.状态==新方法操作状态::定义前提已变化)
            throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"simulation parameter scene premise changed");
        if(!context.参数场景)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "simulation parameter context rejected: state=" + std::to_string(static_cast<int>(context.状态))
            + " builtin=" + m.本能函数登记名.value_or("")
            + " parameters=" + std::to_string(m.参数规格组.size()));
        for(std::size_t i=0;i<N;++i)if(defs[i]&&defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
            const auto F=(*bindings)[i].有序引用->信息节点;
            const auto f=context.参数场景->读取绑定特征(F);
            const auto* V = (f && f->当前值) ? std::get_if<稳定编码>(&*f->当前值) : nullptr;
            const auto v=V?world->读取模拟特征值材料(*V):std::nullopt;
            const auto* members=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
            if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "local task collection readback is not U64 array");
            for(const auto T:*members) {
                const auto source=world->读取模拟任务来源(稳定编码{T});
                const auto ref=world->查找任务虚拟存在引用(稳定编码{T});
                const auto state=ref?world->读取任务虚拟存在引用(*ref):std::nullopt;
                const auto local=context.参数场景->读取绑定集合任务特征组(static_cast<std::uint32_t>(i+1),稳定编码{T});
                if(!source||!local||!state||state->引用!=local->引用
                    ||state->当前状态!=local->状态.当前值)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"local task governance readback missing");
                const auto& original=任务来源.at(*source).治理特征;
                if(local->引用.任务节点.值!=T||local->引用.任务节点==*source
                    ||local->引用.虚拟存在节点==original.引用.虚拟存在节点
                    ||local->状态.当前值!=original.状态.当前值
                    ||local->服务值.has_value()!=original.服务值.has_value()
                    ||(local->服务值&&local->服务值->当前值!=original.服务值->当前值))
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"local task governance readback disagrees");
            }
        }
        struct 附属样例 final {
            std::uint32_t 位置=0;稳定编码 FT;
            新方法参数特征读取结果 前;
        };
        std::vector<附属样例> 附属组;
        for(std::size_t e=0;e<N;++e)if(是普通存在(e)) {
            std::set<稳定编码> 已查FT;
            for(std::size_t k=0;k<K;++k)if(defs[k]
                &&base[k].所属存在参数==代表[e]+1&&已查FT.insert(defs[k]->特征类型根).second) {
                const auto FT=defs[k]->特征类型根;
                const auto f=world->读取模拟存在参数特征(static_cast<std::uint32_t>(e+1),FT);
                if(f.状态==新方法参数特征读取状态::未声明)continue;
                if(f.状态==新方法参数特征读取状态::定义前提已变化)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"attached feature sample premise changed");
                if((f.状态!=新方法参数特征读取状态::已读取&&f.状态!=新方法参数特征读取状态::当前值未取得)||!f.特征)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"attached feature sample locator unavailable");
                bool 直接别名=false;
                for(std::size_t j=0;j<N;++j)if(defs[j]&&(*bindings)[j].有序引用->信息节点==f.特征->节点) {
                    // R97：直接特征位置须在规格中声明所属为本存在位置。
                    if(j>=m.参数规格组.size()||!m.参数规格组[j].有序规格
                        ||m.参数规格组[j].有序规格->所属存在位置!=e+1)
                        不支持(static_cast<std::uint32_t>(e+1),"attached/direct alias lacks formal ownership declaration");
                    直接别名=true;break;
                }
                if(!直接别名)附属组.push_back({static_cast<std::uint32_t>(e+1),FT,f});
            }
        }
        std::vector<std::optional<新特征信息>> before(N);
        std::vector<std::optional<新特征值信息>> beforeMaterial(N);
        for(std::size_t i=0;i<N;++i) if(defs[i]) {
            before[i]=context.参数场景->读取绑定特征((*bindings)[i].有序引用->信息节点);
            if(!before[i])throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "simulation before feature current value unavailable: position=" + std::to_string(i + 1));
            if(!before[i]->当前值)continue;
            if(defs[i]->比较规则!=特征概念比较规则::存在身份精确
                &&defs[i]->材料类型!=特征概念材料物理类型::需求节点引用)
              if(const auto* v=std::get_if<稳定编码>(&*before[i]->当前值))
                beforeMaterial[i]=world->读取模拟特征值材料(*v);
        }
        std::map<稳定编码,新模拟需求信息> 动作前需求;
        for(const auto i:需求参数位置) {
            const auto D=world->读取模拟需求((*bindings)[i].有序引用->信息节点);
            if(!D.信息)throw std::logic_error("bound formal demand is unreadable before invocation");
            动作前需求.emplace(D.信息->节点,*D.信息);
        }
        停止();
        const auto 结果声明闭包=方法_.读取方法执行结构闭包({m.节点});
        if(结果声明闭包.状态==新_方法类::执行结构读取状态::前提已变化)
            throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"result declaration premise changed");
        const auto 声明方法=std::find_if(结果声明闭包.方法组.begin(),结果声明闭包.方法组.end(),
            [&](const auto& x){return x.节点==m.节点;});
        if(结果声明闭包.状态!=新_方法类::执行结构读取状态::已读取
            ||声明方法==结果声明闭包.方法组.end()||结果声明闭包.完整保护组.empty()
            ||声明方法->方法头!=m.方法头||声明方法->参数规格组!=m.参数规格组)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"formal result declaration closure unavailable");
        std::set<稳定编码> 结果类型组;
        for(const auto ft:声明方法->方法头.可能结果概念组) {
            if(需求结果概念==std::optional{ft})continue;
            const auto def=方法_.读取模拟特征范围(ft);
            if(!def)不支持(0,"result declaration is not a supported formal feature concept");
            const auto root=方法_.读取模拟特征范围(def->特征类型根);
            if(!root||root->特征类型根!=def->特征类型根||root->材料类型!=def->材料类型||root->比较规则!=def->比较规则)
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"declared result concept root definition inconsistent");
            结果类型组.insert(def->特征类型根); // 原子域仍保留于声明方法快照，不当成实际结果。
        }
        if(需求结果概念) {
            // 本叶只学习首次落实；其它前态在动作前保留未支持，不先执行再抛弃结果。
            const bool 未覆盖前态=std::any_of(before.begin(),before.end(),[&](const auto& f){
                if(!f)return false;
                const auto d=方法_.读取模拟特征范围(f->特征概念节点);
                if(!d)throw std::logic_error("formal demand input feature definition unavailable");
                return d->材料类型==特征概念材料物理类型::需求节点引用
                    &&(f->当前值||!f->历史不同值.empty()||!f->历史值命中信息.empty());
            });
            if(未覆盖前态){context.参数场景.reset();world->释放有序样例();cursor.推进();continue;}
            const auto 预检=world->预检需求执行(*context.参数场景);
            if(预检.状态==新模拟需求执行预检状态::前提已变化)throw 模拟输入采集漂移{};
            if(预检.状态==新模拟需求执行预检状态::依赖未就绪)不支持(0,预检.诊断);
            if(预检.状态==新模拟需求执行预检状态::规格未支持) {
                本支有未完成样例=true;context.参数场景.reset();world->释放有序样例();cursor.推进();continue;
            }
        }
        world->开始记录复制上下文读取(停止检查);
        HY_SIM_PROGRESS(L"即将调用原本能函数",m.参数规格组.size());
        const auto call=方法_.调用本能函数(m.节点,*context.参数场景);
        HY_SIM_PROGRESS(L"原本能调用返回",call.状态);
        const auto 有隐藏依赖=world->结束记录复制上下文读取();
        const auto 调用证据=world->读取已关闭调用证据();
        if(!调用证据 || 调用证据->身份.方法节点!=m.节点)
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"closed isolated invocation evidence missing");
        if(调用证据->已请求取消)
            throw 新学习运行异常(m.节点,新学习运行异常原因::停止取消);
        const auto 成员读取组 = world->读取任务成员记录();
        const auto 方法写回组 = world->读取本次写回特征组();
        const auto 本次场景事实=world->读取本次场景事实组();
        const auto 附属实际读取=world->读取本次存在参数特征读取组();
        const auto 是本次方法写回 = [&](稳定编码 特征) {
            return std::find(方法写回组.begin(),方法写回组.end(),特征) != 方法写回组.end();
        };
        if (成员读取组.size() > 1) 不支持(0,"multiple task members or collection paths are not supported");
        const auto 特征可定位=[&](稳定编码 特征) {
            bool 可定位=false;
            for (std::size_t i=0;i<N;++i)
                if (defs[i] && (*bindings)[i].有序引用->信息节点 == 特征) 可定位 = true;
            for (const auto& 路径 : 成员读取组)
                if (路径.动作前.状态.特征节点 == 特征
                    || (路径.动作前.服务值 && 路径.动作前.服务值->特征节点 == 特征)) 可定位 = true;
            for(const auto& 项:附属组)if(项.前.特征->节点==特征)可定位=true;
            return 可定位;
        };
        for (const auto 特征 : 方法写回组) {
            const bool 可定位=特征可定位(特征);
            if (!可定位) 不支持(0,"method wrote a feature without a formal C/R parameter or member path");
        }
        for(const auto& 变化:调用证据->变化组) {
            if(变化.调用节点!=调用证据->身份.调用节点 || !特征可定位(变化.特征节点))
                不支持(0,"isolated operation evidence has no complete formal feature locator");
            const auto 后=world->读取本地状态(变化.后状态);
            const auto 动=world->读取本地变化(变化.变化动态);
            if(!后||后->来源实例特征节点!=变化.特征节点||!动
                ||std::find(动->状态节点组.begin(),动->状态节点组.end(),后->节点)==动->状态节点组.end())
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"isolated operation state/dynamic evidence incomplete");
            if(变化.前状态) {
                const auto 前=world->读取本地状态(*变化.前状态);
                if(!前||前->来源实例特征节点!=变化.特征节点
                    ||std::find(动->状态节点组.begin(),动->状态节点组.end(),前->节点)==动->状态节点组.end())
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"isolated operation previous state evidence incomplete");
            }
            if(变化.方法直接写入) {
                if(!变化.动作节点||std::find(调用证据->阶段动作组.begin(),调用证据->阶段动作组.end(),*变化.动作节点)==调用证据->阶段动作组.end())
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"isolated method operation lacks independent action");
                const auto a=world->读取本地动作(*变化.动作节点);
                if(!a||a->动作目标特征节点!=变化.特征节点)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"isolated method operation action target differs");
            } else if(变化.动作节点)
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"environment operation attributed to method action");
        }
        if(call.状态==新方法本能调用状态::定义前提已变化&&!call.本次条件结果对)
            throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"method invocation premise changed before function call");
        if(call.状态!=新方法本能调用状态::已调用||!call.本次条件结果对)
            throw 新学习运行异常(m.节点,新学习运行异常原因::本能运行异常);
        const auto& run=*call.本次条件结果对;
        HY_SIM_PROGRESS(L"本能执行状态读回",run.结果.状态);
        // 工程依赖或执行保障失败与输入取值无关，不能当未完成业务样例继续枚举。
        // 调用关闭证据已核验；异常展开仍由world既有析构路径释放完整本地样例。
        switch(run.结果.状态) {
        case 新本能函数执行状态::函数未登记:
            throw 新学习运行异常(m.节点,新学习运行异常原因::本能运行异常,
                "simulation intrinsic is not registered; parameter enumeration stopped");
        case 新本能函数执行状态::依赖未装配:
            throw 新学习运行异常(m.节点,新学习运行异常原因::本能运行异常,
                "simulation intrinsic dependency unavailable; parameter enumeration stopped");
        case 新本能函数执行状态::资源失败:
            throw 新学习运行异常(m.节点,新学习运行异常原因::资源不足,
                "simulation intrinsic resource failure; parameter enumeration stopped");
        case 新本能函数执行状态::内部错误:
        case 新本能函数执行状态::工程停止:
            throw 新学习运行异常(m.节点,新学习运行异常原因::内部错误,
                "simulation intrinsic internal error or engineering stop; parameter enumeration stopped");
        default: break;
        }
        std::optional<新本能已发布需求结果> 已核需求结果;
        std::set<稳定编码> 已核需求保护;
        if(需求结果概念&&run.结果.状态==新本能函数执行状态::已按合同返回
            &&run.结果.结果取得状态==新本能函数结果取得状态::已取得) {
            if(!run.结果.已发布需求||!调用证据->身份.主动作
                ||run.结果.动作动态节点!=调用证据->身份.主动作)
                throw std::logic_error("isolated demand result missing its actual main action");
            const auto 核验=world->核验本次需求执行结果(*context.参数场景,调用证据->身份.调用节点,*run.结果.已发布需求);
            if(核验.状态==新本能已发布需求核验状态::资源失败)throw std::bad_alloc{};
            if(核验.状态==新本能已发布需求核验状态::前提已变化)throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化);
            if(核验.状态!=新本能已发布需求核验状态::已核验||核验.结果!=run.结果.已发布需求||核验.完整保护.empty())
                throw std::logic_error("isolated demand actual result independent verification failed");
            已核需求结果=核验.结果;
            for(const auto& p:核验.完整保护)已核需求保护.insert(p.结构.节点.编码);
            HY_SIM_PROGRESS(L"本次需求实际隔离结果已独立读回",1);
        }
        std::vector<新方法初始场景事实限定> 已核结果事实;
        for(const auto& 事实:run.结果.已取得场景事实组) {
            if(!调用证据->身份.主动作 || run.结果.动作动态节点!=调用证据->身份.主动作)
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"scene result lacks independent main action");
            新交互连接过程 过程;
            const auto 核验=world->读取本次结果场景事实(事实,*bindings,
                调用证据->身份.调用节点,*调用证据->身份.主动作,过程);
            if(核验==新交互求值状态::不支持)不支持(0,"unhandled current scene result");
            if(核验==新交互求值状态::前提变化)
                throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"scene result premise changed");
            if(核验!=新交互求值状态::已取得)
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"current scene result verification rejected");
            if(std::find(已核结果事实.begin(),已核结果事实.end(),事实.限定)==已核结果事实.end())
                已核结果事实.push_back(事实.限定);
        }
        // 仅条件样例（5370 §5.3，R96）：本能按合同返回、未进入动作、未取得结果且无写回时，
        // 模拟不能取得结果（例如需要真实读取外设）；条件为真，结果为空，只发表条件节点。
        const bool 仅条件样例=run.结果.状态==新本能函数执行状态::已按合同返回
            &&run.结果.结果取得状态==新本能函数结果取得状态::未取得
            &&run.结果.动作阶段==新本能函数动作阶段::未进入动作
            &&!run.结果.动作动态节点&&方法写回组.empty()&&run.结果.已形成状态节点组.empty();
        if(!仅条件样例&&(run.结果.状态!=新本能函数执行状态::已按合同返回
            ||run.结果.结果取得状态!=新本能函数结果取得状态::已取得))
            本支有未完成样例=true;
        std::optional<新方法条件结果定义> 本组已发表;
        std::optional<稳定编码> 本组配对节点;
        if(run.结果.状态==新本能函数执行状态::已按合同返回
            &&(run.结果.结果取得状态==新本能函数结果取得状态::已取得||仅条件样例)) {
            // 在调用真实返回之后、任何永久材料/域写入之前做内容完整性门。
            std::optional<std::array<std::size_t,3>> 需求位置;
            std::set<std::pair<std::uint8_t,稳定编码>> 已覆盖来源;
            if(!需求参数位置.empty()) {
                if(!已核需求结果||!需求结果概念||!需求主体FT)
                    不支持(0,"formal demand content has no independently verified result");
                const auto& result=*已核需求结果;
                const auto 找位置=[&](稳定编码 id,新方法参数信息类型 type)->std::size_t {
                    for(std::size_t i=0;i<N;++i)if((*bindings)[i].有序引用->信息节点==id
                        &&(*bindings)[i].有序引用->信息类型==type)return 代表[i];
                    不支持(0,"actual demand result cannot be rebound to a formal parameter");return N;
                };
                需求位置=std::array{找位置(result.主体存在节点,新方法参数信息类型::存在),
                    找位置(result.需求特征节点,新方法参数信息类型::特征),找位置(result.需求节点,新方法参数信息类型::存在)};
                const auto [h,f,d]=*需求位置;
                if(!是普通存在(h)||!是需求参数(d)||!defs[f]||!before[f]||before[f]->当前值
                    ||!before[f]->历史不同值.empty()||!before[f]->历史值命中信息.empty())
                    不支持(0,"formal demand learning currently supports only unknown current value and empty history");
                for(const auto i:{h,f,d})if(std::none_of(run.条件.参数角色组.begin(),run.条件.参数角色组.end(),[&](const auto& p){
                    return p.有序引用==(*bindings)[i].有序引用;}))
                    不支持(static_cast<std::uint32_t>(i+1),"successful result did not consume its formal parameter");
                const auto old=动作前需求.find(result.需求节点);
                const auto now=world->读取模拟需求(result.需求节点);
                if(old==动作前需求.end()||!now.信息||old->second!=*now.信息
                    ||old->second.需求主体存在节点!=result.主体存在节点||!old->second.认知来源)
                    不支持(0,"demand content changed or has no complete same-subject source");
                if(!是本次方法写回(result.需求特征节点))
                    不支持(0,"first-value demand result has no actual feature write");
                const auto& source=*old->second.认知来源;
                const auto u=world->读取模拟理解(source.理解节点);
                const auto c=world->读取模拟确认(source.内容确认节点);
                if(!u.信息||!c.信息||c.信息->需重新确认||c.信息->理解!=*u.信息
                    ||u.信息->来源人类节点!=result.主体存在节点
                    ||u.信息->来源要求节点!=source.来源要求节点)
                    不支持(0,"recognized demand source closure does not match the actual demand");
                已覆盖来源={{1,result.需求节点},{2,source.理解节点},{3,source.内容确认节点},{5,u.信息->语言场景节点}};
                if(source.来源要求节点)已覆盖来源.emplace(4,*source.来源要求节点);
            }
            const auto 实际读取=world->读取本次内容读取组();
            bool 已读动作前需求=false,已读动作前特征=false,已读需求写回值=false;
            bool 已读动作前有效确认=false;
            for(const auto& trace:实际读取) {
                if(trace.类别!=6) {
                    if(!需求位置||!已覆盖来源.contains({trace.类别,trace.节点})||!已核需求保护.contains(trace.节点))
                        不支持(0,"content read is outside the independently verified same-demand source closure");
                    bool 相同=false;
                    switch(trace.类别) {
                    case 1:{const auto x=world->读取模拟需求(trace.节点);const auto* v=std::get_if<新模拟需求信息>(&trace.投影);
                        相同=x.信息&&v&&*x.信息==*v&&动作前需求.at(trace.节点)==*v;
                        已读动作前需求=已读动作前需求||(相同&&trace.动作前);break;}
                    case 2:{const auto x=world->读取模拟理解(trace.节点);const auto* v=std::get_if<服务要求理解信息>(&trace.投影);相同=x.信息&&v&&*x.信息==*v;break;}
                    case 3:{const auto x=world->读取模拟确认(trace.节点);const auto* v=std::get_if<服务要求确认信息>(&trace.投影);
                        相同=x.信息&&v&&*x.信息==*v;
                        已读动作前有效确认=已读动作前有效确认||(相同&&trace.动作前&&!v->需重新确认);
                        break;}
                    case 4:{const auto x=world->读取模拟要求(trace.节点);const auto* v=std::get_if<新部分服务要求材料>(&trace.投影);相同=x.信息&&v&&*x.信息==*v;break;}
                    case 5:{const auto x=world->读取模拟语言场景(trace.节点);const auto* v=std::get_if<新模拟语言场景信息>(&trace.投影);
                        相同=x.信息&&v&&x.信息->节点==v->节点&&x.信息->专属存在概念==v->专属存在概念
                            &&x.信息->特征节点组==v->特征节点组&&x.信息->直接成员存在组==v->直接成员存在组
                            &&x.信息->边界存在组==v->边界存在组&&x.信息->概念实例组==v->概念实例组;break;}
                    default:break;
                    }
                    if(!相同)不支持(0,"actual demand source content changed or cannot be covered by its formal predicate");
                    continue;
                }
                const auto* f=std::get_if<新特征信息>(&trace.投影);
                if(!f)throw std::logic_error("typed feature read trace projection invalid");
                const auto d=方法_.读取模拟特征范围(f->特征概念节点);if(!d)throw std::logic_error("typed feature read trace definition invalid");
                if(需求位置&&f->节点==已核需求结果->需求特征节点) {
                    const auto& 前=before[(*需求位置)[1]];
                    if(trace.动作前) {
                        if(!前||*前!=*f||已读需求写回值)
                            不支持(0,"pre-action demand feature trace differs from its fixed projection");
                    } else {
                        const auto 后=world->读取模拟特征(f->节点);
                        if(后&&*后==*f)已读需求写回值=true;
                        else if(!前||*前!=*f||已读需求写回值)
                            不支持(0,"demand feature read is outside its actual first-value write progression");
                        // 动作记录之后、写入之前的fresh复核仍可读取旧值，但它不是动作前C证据。
                    }
                    已读动作前特征=已读动作前特征||trace.动作前;
                    continue;
                }
                if(d->材料类型==特征概念材料物理类型::需求节点引用||d->材料类型==特征概念材料物理类型::信息节点引用
                    ||d->比较规则==特征概念比较规则::需求集合身份判等)
                    不支持(0,"single demand/provenance feature including history has no formal C/R content abstraction");
                if(trace.参数位置组.empty()&&std::none_of(附属组.begin(),附属组.end(),[&](const auto& x){
                    return x.前.特征->节点==f->节点&&std::any_of(附属实际读取.begin(),附属实际读取.end(),[&](const auto& role){
                        return role.有序引用&&role.有序引用->序号==x.位置&&std::any_of(role.实际特征组.begin(),role.实际特征组.end(),
                            [&](const auto& a){return a.特征节点==f->节点;});});}))
                    不支持(0,"actual feature read has no declared parameter or attached feature position");
                if(!f->当前值)不支持(0,"actual read of unknown feature current value has no formal condition abstraction");
            }
            if(需求位置&&(!已读动作前需求||!已读动作前特征||!已读动作前有效确认))
                不支持(0,"formal demand condition has no actual pre-action content/feature read");
            if(有隐藏依赖)不支持(0,"copied context was read but its conditions have no formal C/R parameter positions");
            for(const auto F:方法写回组) {
                const auto f=world->读取模拟特征(F);const auto d=f?方法_.读取模拟特征范围(f->特征概念节点):std::nullopt;
                if(!f||!d)throw std::logic_error("actual write feature projection unavailable");
                if(d->材料类型==特征概念材料物理类型::需求节点引用||d->材料类型==特征概念材料物理类型::信息节点引用
                    ||d->比较规则==特征概念比较规则::需求集合身份判等)
                    if(!需求位置||F!=已核需求结果->需求特征节点)
                        不支持(0,"actual demand/provenance write requires a formal rebindable result abstraction");
            }
            if(run.条件.执行分支节点!=执行分支)
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"successful call execution branch differs");
            const auto identities=world->读取本地实例身份组();
            const std::set<稳定编码> 本地身份(identities.begin(),identities.end());
            const auto 不得本地引用=[&](稳定编码 id) {
                if(本地身份.contains(id))不支持(0,"local sample identity cannot enter permanent knowledge");
            };
            const auto 保存材料域=[&](稳定编码 C,const 新特征值信息& value) {
                不得本地引用(C);
                const auto d=方法_.读取模拟特征范围(C);if(!d)throw std::logic_error("knowledge material feature definition unavailable");
                // U64数字材料不按编码猜测；只有正式引用集合语义才检查成员身份。
                if(d->比较规则==特征概念比较规则::存在集合身份判等
                    ||d->比较规则==特征概念比较规则::需求集合身份判等
                    ||d->比较规则==特征概念比较规则::服务任务集合身份判等
                    ||d->比较规则==特征概念比较规则::消息集合身份判等) {
                    const auto* ids=std::get_if<std::vector<std::uint64_t>>(&value.材料);
                    if(!ids)throw std::logic_error("typed knowledge collection has invalid material");
                    for(const auto id:*ids)不得本地引用(稳定编码{id});
                }
                return 方法_.保存模拟材料值域(C,value);
            };
            新方法条件结果定义 pair;
            if(执行分支)pair.来源执行分支节点组.push_back(*执行分支);
            pair.条件.关系规则=新方法项关系规则::全部成立;
            pair.条件.参数关系组=初始关系组;
            if (m.初始必要条件) {
                for(const auto& 限定:m.初始必要条件->场景事实限定组)
                    if(std::find(本次场景事实.begin(),本次场景事实.end(),限定)==本次场景事实.end())
                        不支持(0,"successful call did not read its declared complete scene fact");
            }
            pair.条件.场景事实限定组=本次场景事实;
            pair.结果.关系规则=新方法项关系规则::全部成立;
            pair.结果.场景事实限定组=已核结果事实;
            const auto 引用位置 = [&](const 新特征信息& f) -> std::uint32_t {
                const auto* e=f.当前值 ? std::get_if<稳定编码>(&*f.当前值) : nullptr;
                if(!e||!有效(*e))throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"invalid local existence reference");
                for(std::size_t j=0;j<N;++j)if(是普通存在(j)&&(*bindings)[j].有序引用->信息节点==*e)
                    return static_cast<std::uint32_t>(代表[j]+1);
                const auto local=world->读取模拟存在特征组(*e);
                if(!local)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"dangling local existence reference");
                // 引用值是样例本地的非参数存在（R96）：无法用参数位置表达，只省略这条引用关系；
                // 条件项仍记录“值属于参数范围”，条件保持为真。
                return 0;
            };
            const auto 加引用关系 = [&](auto& relations,std::uint32_t f,std::uint32_t e) {
                if(e==0)return;
                const 新方法参数关系 r{新方法参数关系种类::特征引用存在,f,e};
                if(std::find(relations.begin(),relations.end(),r)==relations.end())relations.push_back(r);
            };
            for(std::size_t i=0;i<N;++i) {
                if(defs[i]&&(defs[i]->材料类型==特征概念材料物理类型::需求节点引用
                    ||defs[i]->材料类型==特征概念材料物理类型::信息节点引用
                    ||defs[i]->比较规则==特征概念比较规则::需求集合身份判等))continue;
                if(defs[i]&&!before[i]->当前值)continue; // 未被实际读取的未知输入不补准确C。
                新方法条件项定义 c;c.参数序号=static_cast<std::uint32_t>(i+1);
                if(!defs[i]) {
                    c.存在概念节点=有效存在概念(i);
                    for(const auto& [位置,范围]:初始存在条件)
                        if(位置==i)c.存在概念节点=范围;
                }
                else {
                    c.特征类型概念节点=defs[i]->特征类型根;
                    if(defs[i]->比较规则==特征概念比较规则::存在身份精确) {
                        c.特征值域概念节点=base[i].规格.范围概念节点;
                        加引用关系(pair.条件.参数关系组,static_cast<std::uint32_t>(i+1),引用位置(*before[i]));
                    }
                    else if(const auto* v=std::get_if<std::int64_t>(&*before[i]->当前值))c.精确数值域=特征概念I64闭区间{*v,*v};
                    else {
                        if(!beforeMaterial[i])throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                        if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等
                            ||defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
                            const auto* members=std::get_if<std::vector<std::uint64_t>>(&beforeMaterial[i]->材料);
                            if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                            if (!成员读取组.empty() && 成员读取组.front().集合参数序号 == i + 1)
                                c.特征值域概念节点 = base[i].规格.范围概念节点;
                            else if(const auto formal=集合正式材料(*beforeMaterial[i],defs[i]->比较规则))
                                c.特征值域概念节点=保存材料域(base[i].规格.范围概念节点,*formal);
                            else {
                                c.特征值域概念节点=base[i].规格.范围概念节点;
                                for(std::size_t j=0;j<N;++j)if(是普通存在(j)
                                    &&std::find(members->begin(),members->end(),(*bindings)[j].有序引用->信息节点.值)!=members->end()) {
                                    const 新方法参数关系 relation{新方法参数关系种类::集合包含存在,
                                        static_cast<std::uint32_t>(i+1),static_cast<std::uint32_t>(j+1)};
                                    if(std::find(pair.条件.参数关系组.begin(),pair.条件.参数关系组.end(),relation)==pair.条件.参数关系组.end())
                                        pair.条件.参数关系组.push_back(relation);
                                }
                            }
                        } else c.特征值域概念节点=保存材料域(base[i].规格.范围概念节点,*beforeMaterial[i]);
                    }
                }
                pair.条件.条件项组.push_back(std::move(c));
                if(!defs[i])continue;
                const auto id=(*bindings)[i].有序引用->信息节点;
                const auto f=context.参数场景->读取绑定特征(id);
                if(!f||!f->当前值)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                if (!成员读取组.empty() && 成员读取组.front().集合参数序号 == i + 1) {
                    const auto* V = std::get_if<稳定编码>(&*f->当前值);
                    const auto material = V ? world->读取模拟特征值材料(*V) : std::nullopt;
                    if (!material || !beforeMaterial[i] || material->材料 != beforeMaterial[i]->材料)
                        不支持(static_cast<std::uint32_t>(i + 1),"changed member collection cannot be expressed without source identity");
                    continue; // 未改变集合，不把本轮来源T身份固化成结果。
                }
                if (!是本次方法写回(id)||!结果类型组.contains(defs[i]->特征类型根)) continue;
                新方法结果项定义 r;r.参数序号=static_cast<std::uint32_t>(i+1);r.特征类型概念节点=defs[i]->特征类型根;
                if(defs[i]->比较规则==特征概念比较规则::存在身份精确) {
                    加引用关系(pair.结果.参数关系组,static_cast<std::uint32_t>(i+1),引用位置(*f));
                }
                else if(const auto* v=std::get_if<std::int64_t>(&*f->当前值))r.精确数值域=特征概念I64闭区间{*v,*v};
                else {
                    const auto material=world->读取模拟特征值材料(std::get<稳定编码>(*f->当前值));
                    if(!material)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                    if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等
                        ||defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
                        const auto* members=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                        if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                        if(const auto formal=集合正式材料(*material,defs[i]->比较规则))
                            r.特征值域概念节点=保存材料域(base[i].规格.范围概念节点,*formal);
                        else for(std::size_t j=0;j<N;++j) if(是普通存在(j)
                            &&std::find(members->begin(),members->end(),(*bindings)[j].有序引用->信息节点.值)!=members->end())
                            pair.结果.参数关系组.push_back({新方法参数关系种类::集合包含存在,static_cast<std::uint32_t>(i+1),static_cast<std::uint32_t>(j+1)});
                    } else r.特征值域概念节点=保存材料域(base[i].规格.范围概念节点,*material);
                }
                pair.结果.结果项组.push_back(std::move(r));
            }
            if(需求位置) {
                const auto [h,f,d]=*需求位置;
                新方法条件项定义 前态;前态.参数序号=static_cast<std::uint32_t>(f+1);
                前态.特征类型概念节点=defs[f]->特征类型根;
                前态.特征前态限定=新方法特征前态限定{新方法当前值状态::未知,std::uint64_t{0}};
                pair.条件.条件项组.push_back(前态);
                新方法条件项定义 内容;内容.参数序号=static_cast<std::uint32_t>(d+1);
                // 同D来源闭包、动作前确认读迹以及原reader固定语义核验均已在上方通过。
                内容.引用内容条件=新方法引用内容条件{{},*需求结果概念,新方法内容作用点::参数自身,true};
                pair.条件.条件项组.push_back(内容);
                if(f>=m.参数规格组.size()||!m.参数规格组[f].有序规格
                    ||m.参数规格组[f].有序规格->所属存在位置!=h+1)
                    不支持(0,"actual demand feature ownership is not declared by the method");
                pair.条件.参数关系组.push_back({新方法参数关系种类::内容引用参数,
                    static_cast<std::uint32_t>(d+1),static_cast<std::uint32_t>(h+1),{*需求主体FT}});
                新方法结果项定义 r;r.参数序号=static_cast<std::uint32_t>(f+1);
                r.引用内容结果=新方法引用内容条件{{},*需求结果概念,新方法内容作用点::特征当前引用};
                pair.结果.结果项组.push_back(r);
                加引用关系(pair.结果.参数关系组,static_cast<std::uint32_t>(f+1),static_cast<std::uint32_t>(d+1));
                const auto 规范化=[](auto& 组) {
                    std::sort(组.begin(),组.end(),[](const auto& a,const auto& b){
                        return std::tie(a.种类,a.左参数序号,a.右参数序号,a.左正式内容路径)
                            <std::tie(b.种类,b.左参数序号,b.右参数序号,b.左正式内容路径);});
                    组.erase(std::unique(组.begin(),组.end()),组.end());
                };
                规范化(pair.条件.参数关系组);规范化(pair.结果.参数关系组);
            }
            for(const auto& 项:附属组) {
                const auto F=项.前.特征->节点;
                const auto d=方法_.读取模拟特征范围(项.FT);
                if(!d)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"attached feature result type missing");
                const auto role=std::find_if(附属实际读取.begin(),附属实际读取.end(),[&](const auto& x) {
                    return x.有序引用&&x.有序引用->序号==项.位置;
                });
                const 新本能函数条件特征材料* used=nullptr;
                if(role!=附属实际读取.end()) {
                    const auto it=std::find_if(role->实际特征组.begin(),role->实际特征组.end(),[&](const auto& x){return x.特征节点==F;});
                    if(it!=role->实际特征组.end())used=&*it;
                }
                const auto 填域=[&](auto& target,const 新方法参数特征读取结果& value) {
                    if(!value.特征||!value.特征->当前值)不支持(项.位置,"written attached feature current value missing");
                    if(const auto* v=std::get_if<std::int64_t>(&*value.特征->当前值))
                        target.精确数值域=特征概念I64闭区间{*v,*v};
                    else {
                        if(d->材料类型==特征概念材料物理类型::存在节点引用
                            ||d->材料类型==特征概念材料物理类型::需求节点引用
                            ||d->比较规则==特征概念比较规则::存在集合身份判等
                            ||d->比较规则==特征概念比较规则::服务任务集合身份判等
                            ||d->比较规则==特征概念比较规则::需求集合身份判等
                            ||d->比较规则==特征概念比较规则::消息集合身份判等)
                            不支持(项.位置,"attached identity value has no supported formal left-feature relation");
                        if(!value.当前完整材料)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"attached complete material missing");
                        target.特征值域概念节点=保存材料域(项.FT,*value.当前完整材料);
                    }
                };
                if(used) {
                    if(used->动作前准确值!=项.前.特征->当前值)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"attached feature changed before actual read");
                    新方法条件项定义 c;c.参数序号=项.位置;c.特征类型概念节点=项.FT;
                    const auto 已知存在=std::find_if(pair.条件.条件项组.begin(),pair.条件.条件项组.end(),[&](const auto& x) {
                        return x.参数序号==项.位置&&x.存在概念节点&&!x.特征类型概念节点;
                    });
                    if(已知存在!=pair.条件.条件项组.end())c.存在概念节点=已知存在->存在概念节点;
                    if(used->动作前准确值)填域(c,项.前);
                    pair.条件.条件项组.push_back(std::move(c));
                }
                if(是本次方法写回(F)&&结果类型组.contains(项.FT)) {
                    const auto 后=world->读取模拟存在参数特征(项.位置,项.FT);
                    if(后.状态==新方法参数特征读取状态::定义前提已变化)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"attached result premise changed");
                    if(后.状态!=新方法参数特征读取状态::已读取||!后.特征||后.特征->节点!=F)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"attached result locator differs");
                    新方法结果项定义 r;r.参数序号=项.位置;r.特征类型概念节点=项.FT;填域(r,后);
                    pair.结果.结果项组.push_back(std::move(r));
                }
            }
            if (!成员读取组.empty()) {
                const auto& 路径 = 成员读取组.front();
                const auto after = world->读取任务虚拟存在特征组(路径.动作前.引用);
                if (!after || after->引用 != 路径.动作前.引用
                    || after->服务值.has_value() != 路径.动作前.服务值.has_value())
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"member governance shape changed");
                const auto 增加项 = [&](const 新任务虚拟存在特征读回& 前, const 新任务虚拟存在特征读回& 后) {
                    if (前.特征节点 != 后.特征节点 || 前.特征类型概念节点 != 后.特征类型概念节点)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"member feature identity changed");
                    新方法条件项定义 c;
                    c.参数序号 = 路径.集合参数序号; c.任务成员变量序号 = 1;
                    c.特征类型概念节点 = 前.特征类型概念节点;
                    c.精确数值域 = 特征概念I64闭区间{前.当前值,前.当前值};
                    pair.条件.条件项组.push_back(c);
                    if (是本次方法写回(后.特征节点)&&结果类型组.contains(后.特征类型概念节点)) {
                        新方法结果项定义 r;
                        r.参数序号 = 路径.集合参数序号; r.任务成员变量序号 = 1;
                        r.特征类型概念节点 = 后.特征类型概念节点;
                        r.精确数值域 = 特征概念I64闭区间{后.当前值,后.当前值};
                        pair.结果.结果项组.push_back(r);
                    }
                };
                增加项(路径.动作前.状态,after->状态);
                if (after->服务值) 增加项(*路径.动作前.服务值,*after->服务值);
            }
            // 仅条件样例结果为空也发表条件节点（5370 §5.3，R96）。
            if(!pair.结果.结果项组.empty()||!pair.结果.参数关系组.empty()||!pair.结果.场景事实限定组.empty()||仅条件样例) {
                // 调用证据完整性：每个阶段动作须为本方法同次调用的动作；需求结果须有独立核同的首个后状态。
                for(const auto A:调用证据->阶段动作组) {
                    const auto 动作=world->读取本地动作(A);
                    const bool 主动作=调用证据->身份.主动作==std::optional{A};
                    if(!动作||!动作->动作来源节点||动作->动作定义节点!=m.节点
                        ||(主动作&&动作->动作来源节点!=调用证据->身份.调用节点))
                        throw std::logic_error("method definition reference has no same-call action");
                }
                if(已核需求结果) {
                    const auto& 结果=*已核需求结果;
                    const auto 后=world->读取最新特征状态(结果.结果场景节点,结果.主体存在节点,结果.需求特征节点);
                    if(!后||!已核需求保护.contains(后->节点)
                        ||后->被描述存在节点!=结果.主体存在节点||后->来源实例特征节点!=结果.需求特征节点
                        ||后->固定准确值!=新特征准确值{结果.需求节点}
                        ||std::find(run.结果.已形成状态节点组.begin(),run.结果.已形成状态节点组.end(),后->节点)
                            ==run.结果.已形成状态节点组.end())
                        throw std::logic_error("method definition reference has no independently verified first post-state");
                }
                // 发布核验（2026-11-09用户裁决）：概念只维护当前状态、认知允许漂移，结果按身份引用概念，概念内容变化与
                // 他处引用增删都不阻止发布。只核：①结果声明闭包读过的节点仍存在；②挂接的方法结构不变。此处提前核一次，
                // 发布器在方法锁内对同一基线再核（追加模拟条件结果的学习基线参数）。
                if(方法_.核对学习发布基线(结果声明闭包)!=新_方法类::学习发布基线状态::成立)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"result declaration closure changed before publication");
                const auto 核可选=[&](const std::optional<稳定编码>& id){if(id)不得本地引用(*id);};
                const auto 核域=[&](const std::optional<稳定编码>& C) {
                    核可选(C);if(!C)return;const auto d=方法_.读取模拟特征范围(*C);if(!d)throw std::logic_error("knowledge domain missing before publication");
                    if(const auto* refs=std::get_if<std::vector<稳定编码>>(&d->值域))for(const auto id:*refs)不得本地引用(id);
                };
                for(const auto& c:pair.条件.条件项组) {
                    不得本地引用(c.参数角色节点);核可选(c.存在概念节点);核可选(c.特征类型概念节点);核域(c.特征值域概念节点);
                    核可选(c.二次特征概念节点);核可选(c.状态或动态概念节点);
                    if(c.引用内容条件){不得本地引用(c.引用内容条件->要求概念节点);for(const auto FT:c.引用内容条件->正式内容路径)不得本地引用(FT);}
                }
                for(const auto& r:pair.结果.结果项组) {
                    不得本地引用(r.参数角色节点);核可选(r.特征类型概念节点);核域(r.特征值域概念节点);
                    核可选(r.变化方向概念节点);核可选(r.状态或动态概念节点);
                    if(r.引用内容结果){不得本地引用(r.引用内容结果->要求概念节点);for(const auto FT:r.引用内容结果->正式内容路径)不得本地引用(FT);}
                }
                for(const auto* group:{&pair.条件.参数关系组,&pair.结果.参数关系组})
                    for(const auto& rel:*group)for(const auto FT:rel.左正式内容路径)不得本地引用(FT);
                for(const auto& f:pair.条件.场景事实限定组)不得本地引用(f.关系概念节点);
                for(const auto& f:pair.结果.场景事实限定组)不得本地引用(f.关系概念节点);
                for(const auto id:pair.来源执行分支节点组)不得本地引用(id);
                // 取不到结果的样例（仅条件节点）：I64条件记录本次取样所用的合法值域区间（采样值所在的
                // 规范化闭区间），不记单个采样点（2026-11-06用户裁决，替代5370第108行“条件记录实际实参”）。
                // 有结果的样例仍记实际实参，供尝试学习补全后按方向判断融合。
                if(pair.结果.结果项组.empty()&&pair.结果.参数关系组.empty()&&pair.结果.场景事实限定组.empty())
                    for(auto& c:pair.条件.条件项组) {
                        if(!c.精确数值域||c.精确数值域->下界!=c.精确数值域->上界
                            ||c.参数序号==0||c.参数序号>N||!defs[c.参数序号-1])continue;
                        const auto* 采样域=std::get_if<std::vector<特征概念I64闭区间>>(&defs[c.参数序号-1]->值域);
                        if(!采样域)continue;
                        const auto v=c.精确数值域->下界;
                        const auto 所在=std::find_if(采样域->begin(),采样域->end(),[&](const auto& r){return r.下界<=v&&v<=r.上界;});
                        if(所在!=采样域->end())c.精确数值域=*所在;
                    }
                // R102（设计v0.2 §7）：无结果样例的存在集合条件，由“等于本次集合快照”改为概念成员约束：
                // 成员是有序普通存在参数j的实参时，取参数j的范围概念为概念成员；集合非空则至少一个存在。
                // “集合包含存在”参数关系照常保留；本次读过成员的集合参数（成员读取组）不改。
                if(pair.结果.结果项组.empty()&&pair.结果.参数关系组.empty()&&pair.结果.场景事实限定组.empty())
                    for(auto& c:pair.条件.条件项组) {
                        if(c.参数序号==0||c.参数序号>N)continue;
                        const auto i=static_cast<std::size_t>(c.参数序号-1);
                        if(!defs[i]||defs[i]->比较规则!=特征概念比较规则::存在集合身份判等||!beforeMaterial[i])continue;
                        if(!成员读取组.empty()&&成员读取组.front().集合参数序号==i+1)continue;
                        const auto* members=std::get_if<std::vector<std::uint64_t>>(&beforeMaterial[i]->材料);
                        if(!members)continue;
                        特征概念存在集合值域 约束;
                        for(std::size_t j=0;j<N;++j)
                            if(是普通存在(j)&&std::find(members->begin(),members->end(),(*bindings)[j].有序引用->信息节点.值)!=members->end())
                                约束.含概念成员组.push_back(base[j].规格.范围概念节点);
                        std::sort(约束.含概念成员组.begin(),约束.含概念成员组.end());
                        约束.含概念成员组.erase(std::unique(约束.含概念成员组.begin(),约束.含概念成员组.end()),约束.含概念成员组.end());
                        约束.至少一个存在=!members->empty()&&约束.含概念成员组.empty();
                        c.特征值域概念节点=方法_.保存模拟集合约束值域(base[i].规格.范围概念节点,特征概念值域{约束});
                    }
                const auto published=方法_.追加模拟条件结果(m.节点,{pair},&结果声明闭包);
                HY_SIM_PROGRESS(L"追加条件结果返回，尚待读回",published.状态);
                if(published.状态==新方法学习发布状态::前提已变化)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::定义前提已变化,"result declaration baseline changed at publication");
                if(published.状态==新方法学习发布状态::资源失败)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::资源不足,"learning publication resource failure");
                if(published.状态!=新方法学习发布状态::已发布&&published.状态!=新方法学习发布状态::精确重复
                    &&published.状态!=新方法学习发布状态::发布结果未知)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                const auto fresh=方法_.获取方法(m.节点);
                if(!fresh||!std::any_of(fresh->条件结果组.begin(),fresh->条件结果组.end(),[&](const auto& p) {
                    return p.条件==pair.条件&&p.结果==pair.结果&&p.命中次数==0
                        &&p.知识来源==新方法知识来源种类::模拟
                        &&(!执行分支||std::find(p.来源执行分支节点组.begin(),p.来源执行分支节点组.end(),*执行分支)!=p.来源执行分支节点组.end());
                }))工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"模拟学习发布读回",
                    L"完整条件结果发布无法读回确认；保留当前样例，禁止析构释放后继续业务。");
                本组已发表=pair;
                if(published.状态==新方法学习发布状态::已发布&&published.新配对节点组.size()==1)
                    本组配对节点=published.新配对节点组.front();
                else if(published.已有配对节点)本组配对节点=*published.已有配对节点;
                HY_SIM_PROGRESS(L"正式条件结果已核同读回",fresh->条件结果组.size());
                本支有正式结果=true;
            }
        }
        context.参数场景.reset();
        HY_SIM_PROGRESS(L"即将释放当前隔离样例",0);
        world->释放有序样例();
        HY_SIM_PROGRESS(L"隔离样例已释放",0);
        if (本组已发表) {
            const auto 释放后=方法_.获取方法(m.节点);
            if(!释放后||!std::any_of(释放后->条件结果组.begin(),释放后->条件结果组.end(),[&](const auto& p) {
                return p.条件==本组已发表->条件&&p.结果==本组已发表->结果&&p.命中次数==0
                    &&p.知识来源==新方法知识来源种类::模拟
                    &&(!执行分支||std::find(p.来源执行分支节点组.begin(),p.来源执行分支节点组.end(),*执行分支)!=p.来源执行分支节点组.end());
            }))throw 新学习运行异常(m.节点,新学习运行异常原因::发布结果未知,"knowledge invalid after sample release");
            // 每次发表后按条件/结果变化方向尝试融合（2026-11-06用户裁决）；无可融合或资源失败保留原配对，
            // 下一次发表再试；发布结果未知按工程问题上报。
            if (本组配对节点) {
                const auto 融合=方法_.融合模拟条件结果(m.节点,*本组配对节点);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
                try {
                    std::wostringstream 文本;
                    文本<<L"方法="<<m.节点.值<<L" 新配对="<<本组配对节点->值<<L" 融合状态="<<static_cast<int>(融合.状态)
                        <<L" 保留="<<(融合.新配对节点组.empty() ? 0 : 融合.新配对节点组.front().值);
                    (void)调试日志<调试切片::PARAM_BINDING>(L"模拟学习",L"配对融合",文本.str());
                } catch(...) {}
#endif
                if(融合.状态==新方法学习发布状态::发布结果未知||融合.状态==新方法学习发布状态::结构不一致)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"pair value-domain merge publication inconsistent");
            }
        }
        cursor.推进();
    }
    if(!本支有合法样例)throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"execution branch has no legal simulation sample");
    if(!本支有正式结果)
        不支持(0,"execution branch produced no formally published actual result");
    if(本支有未完成样例)
        不支持(0,"execution branch contains samples without a completed actual result");
    return;
    }catch(const 模拟输入采集漂移&) {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        ++观察.外层漂移;
        const auto 最后阶段=观察.当前阶段;
        HY_SIM_PROGRESS(最后阶段,0);
#endif
        HY_SIM_PROGRESS(L"外层采集漂移，整批重新开始",0);
        if(停止检查&&停止检查())throw 新学习运行异常(原方法.节点,新学习运行异常原因::停止取消);
        std::this_thread::yield(); // 尚未建样例/调用/发表：丢整批纯输入，重新采集，不拼两轮来源。
    }
}

}
