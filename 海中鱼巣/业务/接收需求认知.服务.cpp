#include "接收需求认知.服务.h"
#include "新_外设世界树类.h"
#include "字段询问.语言服务.h"
#include "关系陈述.语言服务.h"
#include "新_世界树类.h"
#include "../适配/适配器.UTC时钟.h"
#include "转换.语言场景基础.h"
#include "表达.含义网投影.h"
#include "表达.基础语言.h"
#include "转换.语言场景共同.h"
#include "查询.词典候选.h"
#include "../领域/新_自然语言世界树类.h"
#include "../领域/新_场景类.h"
#include "../领域/新_需求类.h"
#include "../领域/新_任务类.h"
#include "../领域/语言分界.数据.h"
#include "../领域/概念_二次特征类.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
#include <atomic>
#endif
#include <sstream>
#include <algorithm>
#include <charconv>
#include <chrono>
#include <functional>
#include <iterator>
#include <map>
#include <mutex>
#include <set>
#include <stdexcept>
#include <tuple>

namespace 海中鱼巣 {
namespace {
using 阶段=接收需求认知阶段;
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
std::atomic_uint64_t 接收解释诊断序号{0};
void 记录接收解释调用(std::uint64_t 序号, 稳定编码 消息,
    std::chrono::steady_clock::time_point 开始, const 语言基础统一输入结果* 结果,
    const 解释原文范围* 句段 = nullptr) noexcept {
    if (序号 > 64) return;
    try {
        std::wostringstream 文本;
        文本 << L"调用序号=" << 序号 << L"；消息=" << 消息.值
            << L"；环节=" << (结果 ? L"返回" : L"开始")
            << L"；耗时毫秒=" << std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - 开始).count();
        if (句段) 文本 << L"；原文起点=" << 句段->起始标量 << L"；原文终点=" << 句段->结束标量;
        if (结果) {
            文本 << L"；统一状态=" << static_cast<int>(结果->状态)
                << L"；图解释状态=" << (结果->图解释状态 ? static_cast<int>(*结果->图解释状态) : -1)
                << L"；完整候选数=" << 结果->完整候选组.size()
                << L"；受阻节点数=" << 结果->受阻节点组.size();
            const auto 偏移 = 句段 ? 句段->起始标量 : 0;
            for (const auto& 缺口 : 结果->静态角色缺口组) {
                文本 << L"；角色缺口=" << 缺口.知识节点.值
                    << L"," << static_cast<int>(缺口.参数)
                    << L"," << static_cast<int>(缺口.种类)
                    << L"," << static_cast<int>(缺口.类别)
                    << L"," << 偏移 + 缺口.起始标量
                    << L"," << 偏移 + 缺口.结束标量
                    << L"," << (缺口.词条 ? 缺口.词条->值 : 0);
            }
        }
        (void)调试日志<调试切片::RECEIVED_DEMAND_PARSE>(L"接收需求认知", L"只读解释单句基础语言", 文本.str());
    } catch (...) {} // 仅人读观察，日志分配或写入失败不改变原返回和重试。
}
#endif
struct 推进中断 final {阶段 状态;std::string 原因;};
void 检查认知停止(std::stop_token stop){
    if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止前保留已接纳消息及已发表材料"};
}
[[noreturn]] void 漂移(){throw 推进中断{阶段::前提变化,"正式来源或解释依据已变化"};}
[[noreturn]] void 资源(){throw 推进中断{阶段::资源延后,"正式读取或分配资源未完成"};}
void 追加句段缺口(const 语言基础统一输入结果& 解释, 解释原文范围 句段,
    std::vector<新前置语言缺口>& 缺口组) {
    using I = 语言场景基础输入状态;
    using K = 新前置语言缺口种类;
    if (!解释.静态角色缺口组.empty()) {
        for (const auto& 角色缺口 : 解释.静态角色缺口组) {
            const auto 种类 = 角色缺口.种类 == 新语言知识角色种类::领域对象引用 ? K::对象未唯一绑定
                : 角色缺口.词条 ? K::名称材料不足 : K::角色原文待确认;
            std::vector<稳定编码> 来源{角色缺口.知识节点};
            if (角色缺口.词条) 来源.push_back(*角色缺口.词条);
            缺口组.push_back({种类, 句段.起始标量 + 角色缺口.起始标量,
                句段.起始标量 + 角色缺口.结束标量, std::move(来源)});
        }
        return;
    }
    K 种类 = K::语言规则不足;
    switch (解释.状态) {
    case I::已形成且读回:
    case I::已形成值式图:
    case I::已形成只读候选:
        return;
    case I::缺名称:
        种类 = K::名称材料不足;
        break;
    case I::对象待绑定:
        种类 = K::对象未唯一绑定;
        break;
    case I::概念多义待解:
    case I::多规则待解:
    case I::构式多义待解:
        种类 = K::解释多义;
        break;
    default:
        break;
    }

    bool 已列范围 = false;
    const auto 长度 = 句段.结束标量 - 句段.起始标量;
    for (const auto& 未覆盖 : 解释.未覆盖范围组) {
        if (未覆盖.起始标量 >= 未覆盖.结束标量 || 未覆盖.结束标量 > 长度) continue;
        缺口组.push_back({种类, 句段.起始标量 + 未覆盖.起始标量,
            句段.起始标量 + 未覆盖.结束标量, 解释.受阻节点组});
        已列范围 = true;
    }
    if (!已列范围) 缺口组.push_back({种类, 句段.起始标量, 句段.结束标量, 解释.受阻节点组});
}
接收语言句段解释 记录句段解释(const 语言基础统一输入结果& 结果, 解释原文范围 范围,
    const 关系陈述语言服务* 关系陈述 = nullptr) {
    接收语言句段解释 证据{范围.起始标量, 范围.结束标量, 结果.状态, 结果.图解释状态, {}, 结果.构式};
    追加句段缺口(结果, 范围, 证据.缺口组);
    证据.词条类型依据组 = 结果.词条类型依据组;
    for (auto& 依据 : 证据.词条类型依据组) {
        依据.起始标量 += 范围.起始标量;
        依据.结束标量 += 范围.起始标量;
    }
    证据.静态结构 = 结果.静态结构;
    if (证据.静态结构) {
        for (auto& 角色 : 证据.静态结构->角色组) {
            角色.起始标量 += 范围.起始标量;
            角色.结束标量 += 范围.起始标量;
        }
    }
    if (关系陈述) {
        接收语言关系观察 观察;
        for (const auto& 图 : 结果.陈述候选原文组) {
            auto 投影 = 关系陈述->投影接收陈述(图.含义, 图.角色定位组, 范围);
            观察.读取未完成 |= 投影.读取未完成;
            for (auto& 候选 : 投影.候选组)
                if (std::ranges::find(观察.候选组, 候选) == 观察.候选组.end())
                    观察.候选组.push_back(std::move(候选));
        }
        if (观察.读取未完成 || !观察.候选组.empty()) 证据.关系观察 = std::move(观察);
    }
    return 证据;
}
class 接收篇章主体观察 final {
    enum class 称名状态 { 未决, 候选, 已识别 };

public:
    void 记录未绑定候选(const 语言基础统一输入结果& 解释, 稳定编码 语言,
        const 概念_名称关系类& 名称, 接收语言句段解释& 句) {
        using O = 新语言知识含义操作;
        using P = 新语言知识参数角色;
        using K = 新语言知识角色种类;
        using C = 新语言知识角色类别;
        if (!句.静态结构 || 解释.图解释状态 != 基础语言输入状态::静态角色待取材) {
            前文可判_ = false;
            return;
        }
        const auto& 结构 = *句.静态结构;
        const auto 是当前主体缺口 = [&](const auto& 项) {
            return 项.知识节点 == 结构.所用构式 && 项.操作 == 结构.操作
                && 项.参数 == P::主体 && 项.种类 == K::领域对象引用 && 项.类别 == C::存在;
        };
        const auto& 缺口 = 解释.静态角色缺口组;
        const auto 主体 = std::ranges::find_if(缺口, 是当前主体缺口);
        const auto 静态主体 = std::ranges::find(结构.角色组, P::主体, &接收语言已解释角色::参数);
        if (主体 == 缺口.end() || std::ranges::count_if(缺口, 是当前主体缺口) != 1
            || 静态主体 == 结构.角色组.end() || 静态主体->值
            || 主体->角色.角色编号 != 静态主体->角色编号
            || 主体->起始标量 >= 主体->结束标量 || 主体->结束标量 > 句.结束标量 - 句.起始标量
            || 句.起始标量 + 主体->起始标量 != 静态主体->起始标量
            || 句.起始标量 + 主体->结束标量 != 静态主体->结束标量) {
            前文可判_ = false;
            return;
        }
        const auto 起 = 静态主体->起始标量;
        const auto 末 = 静态主体->结束标量;
        if (结构.操作 == O::概念归类) {
            const auto [状态, 名称关系] = 识别称名(主体->词条, 名称);
            if (状态 == 称名状态::未决) {
                前文可判_ = false;
                return;
            }
            句.主体关系 = 接收语言主体关系{起,末,起,末,结构.所用构式,{},false};
            if (主体->词条)
                句.主体关系->来源 = 接收语言主体来源{主体->词条,名称关系,{}};
            记入前文(*句.主体关系);
            return;
        }
        const auto 指代 = 结构.操作 == O::特征描述 ? 第三方单指(主体->词条, 语言, 名称) : std::nullopt;
        if (!指代) {
            前文可判_ = false;
            return;
        }
        const bool 唯一 = 前文可判_ && 首个称名_ && !多个前文对象_;
        句.主体关系 = 接收语言主体关系{起,末,唯一 ? 首个称名_->对象起始标量 : 0,
            唯一 ? 首个称名_->对象结束标量 : 0,结构.所用构式,指代->概念.节点,false,
            接收语言主体来源{指代->词条.节点,指代->节点,{}}};
    }

    void 记录(const 语言基础统一输入结果& 解释, 稳定编码 语言, const 概念_名称关系类& 名称,
        新_自然语言世界树类& 世界, const 新语言前置来源信息& 来源, 接收语言句段解释& 句) {
        if (句.静态结构) {
            using C=新语言知识角色类别;
            using P=新语言知识参数角色;
            const auto& 结构=*句.静态结构;
            if (结构.操作 == 新语言知识含义操作::特征子类型定义
                || 结构.操作 == 新语言知识含义操作::特征有序大于) return;
            const auto 项=std::ranges::find(结构.角色组,P::主体,&接收语言已解释角色::参数);
            const auto* 引用=项!=结构.角色组.end() && 项->值 ? std::get_if<语言图引用值>(&*项->值) : nullptr;
            if (引用 && 引用->类别==C::存在) {
                const auto 起=项->起始标量;
                const auto 末=项->结束标量;
                接收语言主体来源 来源{{},{},引用->节点};
                if (引用->选用名称关系) {
                    const auto 已选=名称.获取名称关系(*引用->选用名称关系);
                    核名称读取状态(已选.状态);
                    if (!已选.信息 || 已选.信息->节点!=*引用->选用名称关系
                        || (引用->来源概念 && 已选.信息->概念节点!=*引用->来源概念)) 漂移();
                    来源.词条=已选.信息->词条节点;
                    来源.名称关系=已选.信息->节点;
                }
                句.主体关系=接收语言主体关系{起,末,起,末,结构.所用构式,{},true,std::move(来源)};
                记入前文(*句.主体关系);
                return;
            }
        }
        const auto 主体 = std::find_if(解释.静态角色缺口组.begin(), 解释.静态角色缺口组.end(),
            [](const auto& 项) {
                return 项.参数 == 新语言知识参数角色::主体
                    && 项.种类 == 新语言知识角色种类::领域对象引用
                    && 项.类别 == 新语言知识角色类别::存在;
            });
        using O = 新语言知识含义操作;
        if (解释.图解释状态 != 基础语言输入状态::静态角色待取材
            || 主体 == 解释.静态角色缺口组.end()
            || (主体->操作 != O::概念归类 && 主体->操作 != O::特征描述)) {
            前文可判_ = false;
            return;
        }

        const auto 起 = 句.起始标量 + 主体->起始标量;
        const auto 末 = 句.起始标量 + 主体->结束标量;
        if (主体->操作 == O::概念归类) {
            const auto [称名,名称关系] = 识别称名(主体->词条, 名称);
            if (称名 == 称名状态::未决) {
                前文可判_ = false;
                return;
            }
            using C = 新语言知识角色类别;
            using P = 新语言知识参数角色;
            using S = 新来源子场景状态;
            std::optional<稳定编码> 局部对象;
            if (称名 == 称名状态::候选 && 句.静态结构 && 句.静态结构->所用构式 == 主体->知识节点) {
                auto& 角色组 = 句.静态结构->角色组;
                const auto 角色 = std::ranges::find(角色组, P::主体, &接收语言已解释角色::参数);
                if (角色 != 角色组.end() && !角色->值 && 角色->起始标量 == 起 && 角色->结束标量 == 末) {
                    const auto 对象 = 世界.建立或取得接收局部对象(来源, 起, 末, 主体->知识节点);
                    if (对象.状态 == S::资源失败) 资源();
                    if (对象.状态 == S::前提已变化 || 对象.状态 == S::未建立) 漂移();
                    if (对象.状态 == S::已建立 || 对象.状态 == S::已存在) {
                        if (!对象.信息) throw std::logic_error("received local object has no formal readback");
                        局部对象 = 对象.信息->存在;
                        角色->值 = 语言图引用值{C::存在, *局部对象, {}, {}};
                        移除主体缺口(句, 起, 末);
                    }
                }
            }
            句.主体关系 = 接收语言主体关系{起, 末, 起, 末, 主体->知识节点, {},
                称名 == 称名状态::已识别 || 局部对象.has_value()};
            if (主体->词条 || 局部对象)
                句.主体关系->来源=接收语言主体来源{主体->词条,名称关系,局部对象};
            记入前文(*句.主体关系);
            return;
        }

        const auto 指代 = 第三方单指(主体->词条, 语言, 名称);
        if (!指代) {
            前文可判_ = false;
            return;
        }
        if (!前文可判_ || !首个称名_ || 多个前文对象_) {
            句.主体关系=接收语言主体关系{起,末,0,0,主体->知识节点,指代->概念.节点,false,
                接收语言主体来源{指代->词条.节点,指代->节点,{},正式前文候选_.size() == 2
                    ? 正式前文候选_ : std::vector<稳定编码>{}}};
            前文可判_=false;
            return;
        }
        const auto 对象=首个称名_->来源 ? 首个称名_->来源->已绑定存在 : std::nullopt;
        句.主体关系 = 接收语言主体关系{起, 末, 首个称名_->对象起始标量,
            首个称名_->对象结束标量, 主体->知识节点, 指代->概念.节点, 首个称名_->已确定,
            接收语言主体来源{指代->词条.节点,指代->节点,对象}};
        if (句.主体关系->已确定) 移除主体缺口(句, 起, 末);
    }

private:
    static void 移除主体缺口(接收语言句段解释& 句, std::uint64_t 起, std::uint64_t 末) {
        std::erase_if(句.缺口组, [起, 末](const auto& 缺口) {
            return 缺口.种类 == 新前置语言缺口种类::对象未唯一绑定
                && 缺口.起始标量 == 起 && 缺口.结束标量 == 末;
        });
    }

    void 记入前文(const 接收语言主体关系& 出现) {
        if (出现.来源 && 出现.来源->已绑定存在 && 正式前文候选_.size() < 2) {
            const auto E = *出现.来源->已绑定存在;
            if (std::ranges::find(正式前文候选_, E) == 正式前文候选_.end())
                正式前文候选_.push_back(E);
        }
        if (!首个称名_) {
            首个称名_=出现;
            return;
        }
        if (出现.来源 && 首个称名_->来源 && 出现.来源->已绑定存在
            && 出现.来源->已绑定存在==首个称名_->来源->已绑定存在) return;
        多个前文对象_=true;
    }

    static void 核名称读取状态(概念名称关系查询状态 状态) {
        using S = 概念名称关系查询状态;
        switch (状态) {
        case S::已找到:
        case S::未找到:
        case S::依赖不存在:
            return;
        case S::依赖已变化:
        case S::读取漂移:
            漂移();
        case S::读取未完成:
        case S::资源失败:
        case S::工程未完成:
            资源();
        case S::输入不合法:
        case S::结构不一致:
            throw std::logic_error("received subject name relation inconsistent");
        }
        throw std::logic_error("unknown subject name relation status");
    }

    static std::pair<称名状态,std::optional<稳定编码>> 识别称名(const std::optional<稳定编码>& 词,
        const 概念_名称关系类& 名称) {
        // 未登记的整段也可能是复合指称，只保留其候选范围。
        if (!词) return {称名状态::候选,{}};
        const auto 结果=名称.读取词条名称候选保护(*词);
        核名称读取状态(结果.状态);
        if (结果.状态==概念名称关系查询状态::未找到) return {称名状态::候选,{}};
        if (结果.状态!=概念名称关系查询状态::已找到 || 结果.候选组.size()!=1)
            return {称名状态::未决,{}};
        const auto* 普通=std::get_if<概念名称关系信息>(&结果.候选组.front());
        if (!普通) return {称名状态::未决,{}};
        return {称名状态::已识别,普通->节点};
    }

    static std::optional<指代名称关系信息> 第三方单指(const std::optional<稳定编码>& 词,
        稳定编码 语言, const 概念_名称关系类& 名称) {
        if (!词) return {};
        const auto 结果=名称.读取词条名称候选保护(*词);
        核名称读取状态(结果.状态);
        if (结果.状态 != 概念名称关系查询状态::已找到 || 结果.候选组.size() != 1) return {};
        const auto* 指代 = std::get_if<指代名称关系信息>(&结果.候选组.front());
        if (!指代 || 指代->语言.节点 != 语言) return {};
        const auto& 定义 = 指代->概念.定义;
        if (定义.规则 != 指代声明规则::作用域第三方
            || 定义.目标范围 != 指代目标范围::单个已登记强类型信息节点
            || !定义.特征约束组.empty()
            || 定义.参数组 != std::vector<指代参数定义>{
                {指代参数角色::目标, 指代参数类型::强类型信息节点},
                {指代参数角色::作用域, 指代参数类型::交互作用域}})
            return {};
        return *指代;
    }

    bool 前文可判_ = true;
    bool 多个前文对象_ = false;
    std::vector<稳定编码> 正式前文候选_;
    std::optional<接收语言主体关系> 首个称名_;
};

bool 同结构(const 基础节点结构& a,const 基础节点结构& b){return a.节点==b.节点&&a.字段组==b.字段组;}
bool 同联合(const 基础联合节点快照& a,const 基础联合节点快照& b){
    return 同结构(a.结构,b.结构)&&a.入向字段组==b.入向字段组
        &&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;
}
void 并保护(std::vector<基础联合节点快照>& out,const std::vector<基础联合节点快照>& in){
    for(const auto& p:in){const auto it=std::find_if(out.begin(),out.end(),[&](const auto& x){return x.结构.节点.编码==p.结构.节点.编码;});
        if(it==out.end())out.push_back(p);else if(!同联合(*it,p))漂移();}
}
bool 同结构闭包(const std::vector<基础联合节点快照>& a,const std::vector<基础联合节点快照>& b){
    return a.size()==b.size()&&std::all_of(a.begin(),a.end(),[&](const auto& x){
        const auto it=std::find_if(b.begin(),b.end(),[&](const auto& y){return y.结构.节点.编码==x.结构.节点.编码;});
        return it!=b.end()&&同结构(x.结构,it->结构);
    });
}
bool 同范围(const 新指定场景范围读取结果& a,const 新指定场景范围读取结果& b){
    return a.根场景节点==b.根场景节点&&a.起始场景节点==b.起始场景节点
        &&a.直接父场景节点==b.直接父场景节点&&a.范围成员存在组==b.范围成员存在组
        &&a.场景成员组.size()==b.场景成员组.size()
        &&std::equal(a.场景成员组.begin(),a.场景成员组.end(),b.场景成员组.begin(),[](const auto& x,const auto& y){
            return x.场景节点==y.场景节点&&x.直接成员存在组==y.直接成员存在组;});
}
bool 同名称(const 概念名称候选正式信息& a,const 概念名称候选正式信息& b){
    if(a.index()!=b.index())return false;
    if(const auto* x=std::get_if<概念名称关系信息>(&a))return *x==std::get<概念名称关系信息>(b);
    const auto& x=std::get<指代名称关系信息>(a);const auto& y=std::get<指代名称关系信息>(b);
    return x.节点==y.节点&&x.概念==y.概念&&x.词条==y.词条&&x.语言==y.语言&&x.角色==y.角色
        &&x.来源.节点==y.来源.节点&&x.来源.种类==y.来源.种类
        &&x.来源.语言节点==y.来源.语言节点&&x.来源.固定原文==y.来源.固定原文;
}
bool 同消息(const 新外设消息信息& a,const 新外设消息信息& b){
    return a.消息节点==b.消息节点&&a.场景节点==b.场景节点&&a.窗口节点==b.窗口节点
        &&a.正文材料节点==b.正文材料节点&&a.来源==b.来源&&a.UTF8正文==b.UTF8正文
        &&a.回应消息节点==b.回应消息节点&&a.对应输出消息节点==b.对应输出消息节点;
}
std::uint64_t 标量数(const std::string& s){
    if(!新_语言类::严格UTF8字节串有效(s))throw std::logic_error("received UTF8 fact corrupt");
    return std::count_if(s.begin(),s.end(),[](unsigned char c){return (c&0xC0)!=0x80;});
}
struct 认知依赖 final {
    新_外设世界树类& 外设;新_自然语言世界树类& 语言世界;
    新_语言类& 语言;新_词类& 词;概念_名称关系类& 名称;概念_存在类& 存在概念;
    概念_特征类& 特征概念;概念_二次特征类& 二次概念;新_存在类& 存在;
    新_特征值类& 特征值;新_特征类& 特征;新_场景类& 现实场景;新_需求类& 需求;
    const 关系陈述语言服务* 关系陈述 = nullptr;
};
void 记录待完善谓项概念(认知依赖& d, const 新语言前置来源信息& 来源,
    const 语言基础统一输入结果& 解释, 接收语言句段解释& 句, std::stop_token stop) {
    using O = 新语言知识含义操作;
    using P = 新语言知识参数角色;
    using K = 新语言知识角色种类;
    using C = 新语言知识角色类别;
    using N = 概念名称关系查询状态;
    using S = 待完善存在概念状态;
    if (!句.静态结构) return;
    auto& 结构 = *句.静态结构;

    for (const auto& 角色 : 解释.静态角色缺口组) {
        if (角色.知识节点 != 结构.所用构式 || 角色.操作 != 结构.操作 || !角色.词条) continue;
        const bool 存在归类 = 角色.操作 == O::概念归类 && 角色.参数 == P::归类概念
            && 角色.种类 == K::概念引用 && 角色.类别 == C::存在概念;
        const bool 特征描述 = 角色.操作 == O::特征描述
            && ((角色.参数 == P::特征类型 && 角色.种类 == K::特征类型引用 && 角色.类别 == C::正式特征类型)
                || (角色.参数 == P::特征值 && 角色.种类 == K::概念引用 && 角色.类别 == C::特征概念));
        const bool 子类型 = 角色.操作 == O::特征子类型定义 && 角色.参数 == P::主体
            && 角色.种类 == K::概念引用 && 角色.类别 == C::特征概念;
        const bool 子类型适用对象 = 角色.操作 == O::特征子类型定义 && 角色.参数 == P::适用对象
            && 角色.种类 == K::概念引用 && 角色.类别 == C::存在概念;
        if (!存在归类 && !特征描述 && !子类型 && !子类型适用对象) continue;
        const auto 开始 = 句.起始标量 + 角色.起始标量;
        const auto 结束 = 句.起始标量 + 角色.结束标量;
        const auto 缺口 = std::ranges::find_if(句.缺口组, [&](const auto& 项) {
            return 项.种类 == 新前置语言缺口种类::名称材料不足
                && 项.起始标量 == 开始 && 项.结束标量 == 结束
                && std::ranges::find(项.已取得相关节点, 角色.知识节点) != 项.已取得相关节点.end()
                && std::ranges::find(项.已取得相关节点, *角色.词条) != 项.已取得相关节点.end();
        });
        if (缺口 == 句.缺口组.end()) continue;
        const auto 谓项 = std::ranges::find(结构.角色组, 角色.参数, &接收语言已解释角色::参数);
        if (谓项 == 结构.角色组.end() || 谓项->值 || 谓项->角色编号 != 角色.角色.角色编号
            || 谓项->起始标量 != 开始 || 谓项->结束标量 != 结束) continue;

        检查认知停止(stop);
        const auto 名称 = d.名称.读取词条名称候选保护(*角色.词条);
        switch (名称.状态) {
        case N::已找到: continue;
        case N::未找到: break;
        case N::依赖不存在: case N::依赖已变化: case N::读取漂移: 漂移();
        case N::资源失败: case N::读取未完成: case N::工程未完成: 资源();
        default: throw std::logic_error("pending concept source name lookup inconsistent");
        }
        if (特征描述 || 子类型) {
            using F = 特征概念规则状态;
            const auto 身份 = d.特征概念.建立或取得待完善特征概念(来源.节点, *角色.词条,
                待完善特征概念原文用法{角色.知识节点, 开始, 结束});
            switch (身份.状态) {
            case F::已完成: case F::无变化:
                if (!身份.信息) throw std::logic_error("pending feature concept receipt missing");
                缺口->已取得相关节点.push_back(身份.信息->节点);
                if (子类型) 谓项->值 = 语言图引用值{C::特征概念, 身份.信息->节点, {}, {}};
                break;
            case F::入口拒绝: break;
            case F::前提已变化: 漂移();
            case F::资源失败: case F::读取接口未装配: 资源();
            default: throw std::logic_error("pending feature concept publication inconsistent");
            }
            continue;
        }
        const auto 身份 = d.存在概念.建立或取得待完善存在概念(
            {来源.节点, *角色.词条, 角色.知识节点, 开始, 结束});
        switch (身份.状态) {
        case S::已建立: case S::已复用:
            if (!身份.信息) throw std::logic_error("pending existence concept receipt missing");
            缺口->已取得相关节点.push_back(身份.信息->节点);
            谓项->值 = 语言图引用值{C::存在概念, 身份.信息->节点, {}, {}};
            break;
        case S::入口拒绝: break;
        case S::前提已变化: 漂移();
        case S::资源失败: case S::依赖未就绪: 资源();
        default: throw std::logic_error("pending existence concept publication inconsistent");
        }
    }
}

bool 取得欠缺的可信词头(认知依赖& d, const std::shared_ptr<const 离线词典数据源>& 词典,
    稳定编码 语言, const std::string& 原句, const 语言基础统一输入结果& 解释, std::stop_token stop) {
    if (!词典 || !解释.静态结构 || 解释.静态角色缺口组.empty()) return false;
    using K = 新语言知识角色种类;
    const auto 可取词头 = [](const 语言图静态角色缺口& 缺口) {
        return !缺口.词条 && (缺口.种类 == K::概念引用 || 缺口.种类 == K::特征类型引用);
    };
    if (!std::ranges::any_of(解释.静态角色缺口组, 可取词头)) return false;

    const auto 语言信息 = d.语言.获取语言(语言);
    if (语言信息.状态 == 新语言查询状态::资源失败) 资源();
    if (语言信息.状态 != 新语言查询状态::已找到 || !语言信息.信息) 漂移();
    if (语言信息.信息->登记键 != "zh-CN") return false;
    const auto 批次 = 词典->读取可信批次标识();
    if (!批次 || 批次->empty()) return false;
    const auto 原文 = 语言场景转换内部::解码语言场景原文(原句);
    if (!原文) throw std::logic_error("headword source has invalid UTF8");

    using W = 新词条查询状态;
    using B = 新词条建立状态;
    using S = 新语言分界工程状态;
    bool 应重读 = false;
    std::set<std::string> 已处理词面;
    const 词典查询服务 查询{词典};
    for (const auto& 缺口 : 解释.静态角色缺口组) {
        检查认知停止(stop);
        if (!可取词头(缺口)) continue;
        const auto 词面 = 语言场景转换内部::读取语言场景原文范围(
            *原文, {缺口.起始标量, 缺口.结束标量});
        if (!已处理词面.insert(词面).second) continue;
        const auto 已有 = d.词.按规范化词面查询(语言, 严格UTF8字节精确规范化合同节点, 词面);
        if (已有.状态 == W::资源失败) 资源();
        if (已有.状态 == W::已找到 && 已有.信息) { 应重读 = true; continue; }
        if (已有.状态 != W::未找到) throw std::logic_error("headword vocabulary lookup failed");

        const auto 规范 = 词典->繁简规范化(词面);
        if (!规范 || *规范 != 词面) continue;
        const auto 命中 = 查询.查询词义({"zh", 词面});
        if (命中.状态 == 词典操作状态::未命中 || 命中.状态 == 词典操作状态::未配置) continue;
        if (命中.状态 == 词典操作状态::资源失败) 资源();
        if (命中.状态 == 词典操作状态::读取失败)
            throw 推进中断{阶段::资源延后, "可信词典词头读取失败"};
        if (命中.状态 != 词典操作状态::已完成 || 命中.义项组.empty() || 命中.包标识.empty())
            throw std::logic_error("trusted headword evidence unreadable");

        新可信词头来源定义 依据{语言, 严格UTF8字节精确规范化合同节点, *批次,
            命中.包标识, "zh", 词面, {}};
        for (const auto& 行 : 命中.义项组) {
            if (行.词头语种 != 依据.语种 || 行.词面 != 词面 || 行.记录键.empty())
                throw std::logic_error("trusted headword record differs from exact lookup");
            依据.记录键组.push_back(行.记录键);
        }
        检查认知停止(stop);
        const auto 来源 = d.语言.建立或取得可信词头来源(std::move(依据));
        if (来源.状态 == S::资源失败) 资源();
        if (来源.状态 == S::前提已变化) 漂移();
        if (来源.状态 == S::输入拒绝) continue;
        if (来源.状态 != S::已完成 || !来源.信息) throw std::logic_error("trusted headword source missing");
        const auto 登记 = d.词.建立或取得词条({语言, 严格UTF8字节精确规范化合同节点, 词面, 来源.信息->节点});
        if (登记.状态 == B::资源失败) 资源();
        if (登记.状态 == B::依赖已变化 || 登记.状态 == B::依赖不存在) 漂移();
        if ((登记.状态 != B::已建立 && 登记.状态 != B::已复用) || !登记.词条节点)
            throw std::logic_error("trusted headword admission failed");
        const auto 读回 = d.词.按规范化词面查询(语言, 严格UTF8字节精确规范化合同节点, 词面);
        if (读回.状态 == W::资源失败) 资源();
        if (读回.状态 == W::未找到) 漂移();
        if (读回.状态 != W::已找到 || !读回.信息 || 读回.信息->节点 != *登记.词条节点
            || (登记.状态 == B::已建立 && 读回.信息->创建来源节点 != 来源.信息->节点))
            throw std::logic_error("trusted headword admission readback differs");
        应重读 = true;
    }
    return 应重读;
}

struct 专名现场项 final {
    稳定编码 C;解释原文范围 范围;
    std::optional<存在概念信息> 概念;
    std::optional<新存在身份保护信息> 身份;
    语言图专名绑定状态 状态=语言图专名绑定状态::非实例专名;
    std::vector<基础联合节点快照> 保护;
};
// 仅本次解释使用。每个实际专名查询均记录，包括被筛除的候选。
class 接收专名读取器 final : public 语言图专名绑定读取接口 {
public:
    接收专名读取器(认知依赖& d,稳定编码 root,std::stop_token stop):d_(d),root_(root),stop_(stop){}
    语言图专名绑定结果 读取实例专名(稳定编码 c,const 解释原文范围& range) const override {
        auto r=读(c,range);项目.push_back(r);
        return {r.状态, r.身份 ? std::optional<稳定编码>{r.身份->存在节点} : std::nullopt, std::move(r.保护)};
    }
    专名现场项 读(稳定编码 c,const 解释原文范围& range) const {
        专名现场项 out{c,range};
        检查认知停止(stop_);
        const auto cp=d_.存在概念.读取普通存在概念定义保护(c);
        if(cp.状态==正式概念读取状态::前提已变化){out.状态=语言图专名绑定状态::前提已变化;return out;}
        if(cp.状态==正式概念读取状态::依赖未就绪){out.状态=语言图专名绑定状态::资源失败;return out;}
        if(cp.状态==正式概念读取状态::非本提供者概念)return out;
        if(cp.状态!=正式概念读取状态::已读取||!cp.信息||cp.完整保护组.empty())throw std::logic_error("proper name concept read corrupt");
        out.概念=cp.信息;out.保护=cp.完整保护组;
        if(cp.信息->种类!=存在概念种类::专属||!cp.信息->对应存在节点)return out;
        检查认知停止(stop_);
        const auto ep=d_.存在.读取存在身份保护(*cp.信息->对应存在节点);
        if(ep.前提已变化){out.状态=语言图专名绑定状态::前提已变化;return out;}
        if(ep.状态==基础结构操作状态::资源失败){out.状态=语言图专名绑定状态::资源失败;return out;}
        if(ep.状态!=基础结构操作状态::已完成||!ep.信息) {out.状态=语言图专名绑定状态::前提已变化;return out;}
        if(ep.信息->专属存在概念节点!=c||ep.信息->存在节点!=*cp.信息->对应存在节点)
            throw std::logic_error("proper name concept and existence identity differ");
        const auto e=std::find_if(ep.预期节点组.begin(),ep.预期节点组.end(),[&](const auto& x){return x.结构.节点.编码==ep.信息->存在节点;});
        if(e==ep.预期节点组.end())throw std::logic_error("proper name existence identity protection missing");
        检查认知停止(stop_);
        const auto back=d_.存在概念.读取专属存在概念身份保护(c,ep.信息->存在节点,*e);
        if(back.状态==专属存在概念身份读取状态::资源失败){out.状态=语言图专名绑定状态::资源失败;return out;}
        if(back.状态==专属存在概念身份读取状态::前提已变化){out.状态=语言图专名绑定状态::前提已变化;return out;}
        if(back.状态!=专属存在概念身份读取状态::已完成)throw std::logic_error("proper name identity reverse read failed");
        并保护(out.保护,ep.预期节点组);并保护(out.保护,back.身份保护);out.身份=ep.信息;
        检查认知停止(stop_);
        const auto scope=d_.现实场景.读取指定场景范围保护(root_);
        if(scope.范围.状态==新场景范围成员读取状态::资源失败){out.状态=语言图专名绑定状态::资源失败;return out;}
        if(scope.范围.状态==新场景范围成员读取状态::读取已变化){out.状态=语言图专名绑定状态::前提已变化;return out;}
        if(scope.范围.状态!=新场景范围成员读取状态::已完成)throw std::logic_error("proper name reality scope unavailable");
        并保护(out.保护,scope.完整保护);
        out.状态=std::find(scope.范围.范围成员存在组.begin(),scope.范围.范围成员存在组.end(),ep.信息->存在节点)
            ==scope.范围.范围成员存在组.end() ? 语言图专名绑定状态::不在范围 : 语言图专名绑定状态::已绑定;
        检查认知停止(stop_);
        return out;
    }
    mutable std::vector<专名现场项> 项目;
private:认知依赖& d_;稳定编码 root_;std::stop_token stop_;
};
std::vector<语言场景局部称名> 读取同源局部称名(认知依赖& d,
    const 新接收认知来源键& 来源, std::stop_token 停止,
    std::optional<std::uint64_t> 截止标量 = {}) {
    using L = 新语言分界工程状态;
    using S = 新来源子场景状态;
    检查认知停止(停止);
    const auto 成员 = d.语言世界.读取接收局部对象组(来源.接收消息节点);
    if (成员.状态 == S::资源失败) 资源();
    if (成员.状态 == S::前提已变化 || 成员.状态 == S::未建立) 漂移();
    if (成员.状态 != S::已存在) throw std::logic_error("adopted statement scene unreadable");
    if (成员.对象组.empty()) return {};
    const auto 固定 = d.语言.读取前置来源(来源, d.外设);
    if (固定.状态 == L::资源失败) 资源();
    if (固定.状态 == L::前提已变化 || 固定.状态 == L::未找到 || 固定.状态 == L::材料未就绪) 漂移();
    if (固定.状态 != L::已完成 || !固定.信息) throw std::logic_error("adopted statement text unreadable");

    using namespace 语言场景转换内部;
    const auto 原文 = 解码语言场景原文(固定.信息->固定UTF8原文);
    if (!原文) throw std::logic_error("adopted statement source is not UTF-8");
    std::vector<语言场景局部称名> 结果;
    for (const auto& 对象 : 成员.对象组) {
        检查认知停止(停止);
        if (对象.声明.前置来源 != 固定.信息->节点
            || (截止标量 && 对象.声明.结束标量 > *截止标量)) continue;
        结果.push_back({对象.存在, 读取语言场景原文范围(*原文,
            {对象.声明.起始标量, 对象.声明.结束标量})});
    }
    return 结果;
}

// A local object stays in its original language scene. This view reads only the
// confirmed window's earlier, already saved declarations.
bool 仅含类型词面引号(const std::string& 正文, const 接收语言句段解释& 句,
    bool 允许后续句 = false) {
    using P = 新语言知识参数角色;
    using O = 新语言知识含义操作;
    if (句.起始标量 != 0 || !句.静态结构) return false;
    const auto 操作 = 允许后续句 ? O::任务用途描述 : O::特征新增请求;
    if (句.静态结构->操作 != 操作) return false;
    const auto& 角色组 = 句.静态结构->角色组;
    const auto 类型 = std::ranges::find(角色组, P::特征类型, &接收语言已解释角色::参数);
    if (类型 == 角色组.end() || 类型->起始标量 == 0 || 类型->起始标量 >= 类型->结束标量
        || 类型->结束标量 >= 句.结束标量) return false;
    using namespace 语言场景转换内部;
    const auto 原文 = 解码语言场景原文(正文);
    if (!原文 || 原文->标量字节边界.size() - 1 < 句.结束标量
        || (!允许后续句 && 原文->标量字节边界.size() - 1 != 句.结束标量)) return false;
    const auto 开引 = static_cast<std::size_t>(类型->起始标量 - 1);
    const auto 闭引 = static_cast<std::size_t>(类型->结束标量);
    if (读取语言场景原文范围(*原文, {开引, 开引 + 1}) != "「"
        || 读取语言场景原文范围(*原文, {闭引, 闭引 + 1}) != "」") return false;
    // 只屏蔽已由构式确定的一对名称边界；额外的引号或转述仍由统一门禁拒绝。
    const auto& 边界 = 原文->标量字节边界;
    auto 外层 = 正文;
    外层.replace(边界[闭引], 边界[闭引 + 1] - 边界[闭引], " ");
    外层.replace(边界[开引], 边界[开引 + 1] - 边界[开引], " ");
    return 可读取无上下文外层共指(外层);
}

std::vector<语言场景局部称名> 读取同窗已声明局部称名(
    认知依赖& d, const 新正式消息角色材料& 本次, std::stop_token 停止,
    std::map<稳定编码, 稳定编码>* 声明消息组 = nullptr,
    const 接收语言句段解释* 新增请求引号依据 = nullptr) {
    using L = 新语言分界工程状态;
    using R = 新接收指代调用状态;
    using O = 新语言知识含义操作;
    using C = 新语言知识角色类别;
    using P = 新语言知识参数角色;
    std::vector<语言场景局部称名> 结果;
    const auto& 当前消息 = 本次.消息;
    if (!本次.事实 || !本次.内部交互场景 || !本次.说话者 || !本次.受话者
        || (!可读取无上下文外层共指(当前消息.UTF8正文)
            && (!新增请求引号依据 || !仅含类型词面引号(当前消息.UTF8正文, *新增请求引号依据)))) return 结果;

    const auto 保留消息 = d.外设.读取当前消息(当前消息.窗口节点, 新外设消息方向::接收);
    for (const auto& 前文 : 保留消息) {
        检查认知停止(停止);
        if (前文.来源.来源序号 >= 当前消息.来源.来源序号) continue;
        if (前文.场景节点 != 当前消息.场景节点 || 前文.窗口节点 != 当前消息.窗口节点
            || 前文.来源.公开宿主会话 != 当前消息.来源.公开宿主会话
            || 前文.来源.对话框标识 != 当前消息.来源.对话框标识)
            throw std::logic_error("local description source is outside its confirmed window");
        if (!可读取无上下文外层共指(前文.UTF8正文)) continue;

        const auto 角色 = d.语言.读取正式消息角色(前文.消息节点, 新外设消息方向::接收);
        if (角色.状态 == R::资源失败) 资源();
        if (角色.状态 == R::前提已变化) 漂移();
        if (角色.状态 != R::已完成 || !角色.信息)
            throw std::logic_error("local description source roles unreadable");
        if (!角色.信息->事实 || 角色.信息->内部交互场景 != 本次.内部交互场景
            || 角色.信息->说话者 != 本次.说话者 || 角色.信息->受话者 != 本次.受话者) continue;

        const 新接收认知来源键 来源{前文.消息节点, 0, 标量数(前文.UTF8正文), {}};
        const auto 正式 = d.语言.读取接收语言解释(来源, d.语言世界, d.外设);
        if (正式.状态 == L::资源失败) 资源();
        if (正式.状态 == L::前提已变化) 漂移();
        if (正式.状态 == L::未找到 || 正式.状态 == L::材料未就绪) continue;
        if (正式.状态 != L::已完成 || !正式.信息)
            throw std::logic_error("local description source explanation unreadable");

        const auto 声明 = d.语言世界.读取接收局部对象组(前文.消息节点);
        using S = 新来源子场景状态;
        if (声明.状态 == S::资源失败) 资源();
        if (声明.状态 == S::前提已变化 || 声明.状态 == S::未建立) 漂移();
        if (声明.状态 != S::已存在)
            throw std::logic_error("local description source objects unreadable");
        if (声明.对象组.empty()) continue;
        using namespace 语言场景转换内部;
        const auto 原文 = 解码语言场景原文(前文.UTF8正文);
        if (!原文) throw std::logic_error("local description source is not UTF-8");

        for (const auto& 对象 : 声明.对象组) {
            const auto& 锚 = 对象.声明;
            if (对象.场景 != 正式.信息->内容.来源场景
                || 锚.前置来源 != 正式.信息->内容.前置来源) continue;
            const auto 是该声明 = [&](const 接收语言句段解释& 句) {
                if (!句.静态结构 || 句.静态结构->操作 != O::概念归类
                    || 句.静态结构->所用构式 != 锚.构式
                    || !句.主体关系 || !句.主体关系->已确定 || 句.主体关系->指代概念) return false;
                const auto& 组 = 句.静态结构->角色组;
                const auto 主体 = std::ranges::find(组, P::主体, &接收语言已解释角色::参数);
                const auto* 值 = 主体 != 组.end() && 主体->值
                    ? std::get_if<语言图引用值>(&*主体->值) : nullptr;
                return 值 && 值->类别 == C::存在 && 值->节点 == 对象.存在
                    && 主体->起始标量 == 锚.起始标量 && 主体->结束标量 == 锚.结束标量;
            };
            if (std::ranges::count_if(正式.信息->内容.句段组, 是该声明) != 1) continue;
            auto 词面 = 读取语言场景原文范围(*原文, {锚.起始标量, 锚.结束标量});
            if (词面.empty()) throw std::logic_error("local description source has an empty name range");
            if (std::ranges::none_of(结果, [&](const auto& 项) {
                return 项.存在节点 == 对象.存在 && 项.词面 == 词面;
            })) {
                结果.push_back({对象.存在, std::move(词面)});
                if (声明消息组) 声明消息组->emplace(对象.存在, 前文.消息节点);
            }
        }
    }
    return 结果;
}

bool 处理局部具名特征材料(认知依赖& d, 接收需求认知观察& out,
    const 新正式消息角色材料& 角色, const 新接收语言解释信息& 解释, std::stop_token stop) {
    using O = 新语言知识含义操作;
    using P = 新语言知识参数角色;
    using C = 新语言知识角色类别;
    using N = 概念名称关系查询状态;
    using T = 新词条查询状态;
    using W = 新特征写入状态;
    const auto& 内容 = 解释.内容;
    if (内容.句段组.size() != 1 || 内容.原文起点 != 0
        || !角色.事实 || !角色.内部交互场景 || !角色.说话者 || !角色.受话者) return false;
    const auto& 句 = 内容.句段组.front();
    if (!句.静态结构 || 句.起始标量 != 内容.原文起点 || 句.结束标量 != 内容.原文终点) return false;
    const auto& 结构 = *句.静态结构;
    const bool 发表 = 结构.操作 == O::未分解具名特征声明;
    if (!发表 && 结构.操作 != O::信息请求) return false;
    if (!可读取无上下文外层共指(角色.消息.UTF8正文)
        || 标量数(角色.消息.UTF8正文) != 内容.原文终点) return false;
    const auto 取角色 = [&](P 参数) -> const 语言图引用值* {
        const auto it = std::ranges::find(结构.角色组, 参数, &接收语言已解释角色::参数);
        return it == 结构.角色组.end() || !it->值 ? nullptr : std::get_if<语言图引用值>(&*it->值);
    };
    const auto* E = 取角色(P::主体);
    const auto* FT = 取角色(P::特征类型);
    if (!E || E->类别 != C::存在 || !FT || FT->类别 != C::特征概念
        || FT->来源概念 != FT->节点 || !FT->选用名称关系) return false;
    if (发表 && 结构.角色组.size() != 2) return false;
    const auto 名称 = d.名称.获取名称关系(*FT->选用名称关系);
    if (名称.状态 == N::工程未完成 || 名称.状态 == N::读取未完成 || 名称.状态 == N::资源失败) 资源();
    if (名称.状态 == N::结构不一致 || 名称.状态 == N::输入不合法)
        throw std::logic_error("local named feature name unreadable");
    if (名称.状态 != N::已找到 || !名称.信息 || 名称.信息->概念节点 != FT->节点) 漂移();
    const auto 词 = d.词.获取词条(名称.信息->词条节点);
    if (词.状态 == T::资源失败) 资源();
    if (词.状态 == T::结构不一致) throw std::logic_error("local named feature word unreadable");
    if (词.状态 != T::已找到 || !词.信息) 漂移();
    if (词.信息->语言节点 != 内容.语言节点 || 词.信息->规范化词面 != "水温") return false;
    std::map<稳定编码, 稳定编码> 声明消息;
    const auto 同窗对象 = 读取同窗已声明局部称名(d, 角色, stop, &声明消息);
    const auto 对象角色 = std::ranges::find(结构.角色组, P::主体, &接收语言已解释角色::参数);
    const auto 原文 = 语言场景转换内部::解码语言场景原文(角色.消息.UTF8正文);
    if (!原文) throw std::logic_error("local named feature source text invalid");
    const auto 词面 = 语言场景转换内部::读取语言场景原文范围(*原文,
        {static_cast<std::size_t>(对象角色->起始标量), static_cast<std::size_t>(对象角色->结束标量)});
    if (std::ranges::count(同窗对象, 词面, &语言场景局部称名::词面) != 1
        || std::ranges::none_of(同窗对象, [&](const auto& 项) { return 项.存在节点 == E->节点 && 项.词面 == 词面; })
        || !声明消息.contains(E->节点)) return false;

    if (!发表) {
        const auto 读回 = d.语言世界.读取接收局部具名特征(E->节点);
        if (读回 && 读回->概念 == FT->节点) out.局部具名特征对象 = E->节点;
        return false;
    }
    const auto 候选保护 = d.名称.读取词条名称候选保护(名称.信息->词条节点);
    if (候选保护.状态 == N::工程未完成 || 候选保护.状态 == N::读取未完成
        || 候选保护.状态 == N::资源失败) 资源();
    if (候选保护.状态 == N::结构不一致 || 候选保护.状态 == N::输入不合法)
        throw std::logic_error("local named feature candidate protection invalid");
    if (候选保护.状态 != N::已找到 || 候选保护.候选组.size() != 1) return false;
    const auto* 关系 = std::get_if<概念名称关系信息>(&候选保护.候选组.front());
    if (!关系 || 关系->节点 != *FT->选用名称关系 || 关系->概念节点 != FT->节点) return false;
    auto 预期 = 角色.证据预期;
    for (const auto& 项 : 候选保护.完整保护) {
        const auto& 节点 = 项.结构.节点;
        if (std::ranges::none_of(预期.节点组, [&](const auto& 已有) { return 已有.编码 == 节点.编码; }))
            预期.节点组.push_back(节点);
        预期.字段集合组.push_back({节点.编码, {}, {}, 项.结构.字段组});
    }
    检查认知停止(stop);
    const auto 已写 = d.语言世界.采用接收局部具名特征(声明消息.at(E->节点), E->节点, FT->节点,
        {角色.消息.消息节点, 解释.解释定义节点, 句.起始标量, 句.结束标量}, std::move(预期));
    if (已写.状态 == W::资源失败) 资源();
    if (已写.状态 == W::前提已变化) 漂移();
    if (已写.状态 == W::入口拒绝 || 已写.状态 == W::概念不存在) return false;
    if ((已写.状态 != W::已完成 && 已写.状态 != W::无变化) || !已写.特征节点)
        throw std::logic_error("local named feature publication failed");
    const auto 读回 = d.语言世界.读取接收局部具名特征(E->节点);
    if (!读回 || 读回->特征 != 已写.特征节点 || 读回->概念 != FT->节点)
        throw std::logic_error("local named feature publication not readable");
    out.局部具名特征对象 = E->节点;
    return true;
}

struct 接收待完善谓项 final {
    const 新前置语言缺口* 缺口;
    语言场景待完善概念词用 词用;
};
std::optional<接收待完善谓项> 读取接收待完善谓项(认知依赖& d, 稳定编码 前置来源,
    const 接收语言句段解释& 句) {
    using C = 新语言知识角色类别;
    using P = 新语言知识参数角色;
    using S = 待完善存在概念状态;
    if (!句.静态结构 || 句.静态结构->操作 != 新语言知识含义操作::概念归类) return {};
    const auto& 结构 = *句.静态结构;
    const auto 归类 = std::ranges::find(结构.角色组, P::归类概念, &接收语言已解释角色::参数);
    const auto* C引用 = 归类 != 结构.角色组.end() && 归类->值
        ? std::get_if<语言图引用值>(&*归类->值) : nullptr;
    if (!C引用 || C引用->类别 != C::存在概念 || C引用->选用名称关系) return {};
    const auto 缺口 = std::ranges::find_if(句.缺口组, [&](const auto& 项) {
        return 项.种类 == 新前置语言缺口种类::名称材料不足
            && 项.起始标量 == 归类->起始标量 && 项.结束标量 == 归类->结束标量
            && std::ranges::find(项.已取得相关节点, C引用->节点) != 项.已取得相关节点.end();
    });
    if (缺口 == 句.缺口组.end()) return {};

    const auto 身份 = d.存在概念.读取待完善存在概念(C引用->节点);
    if (身份.状态 == S::资源失败 || 身份.状态 == S::依赖未就绪) 资源();
    if (身份.状态 == S::未找到 || 身份.状态 == S::前提已变化) 漂移();
    if (身份.状态 != S::已读取 || !身份.信息)
        throw std::logic_error("saved pending predicate identity unavailable");
    const auto& 来源 = 身份.信息->来源;
    if (来源.前置来源 != 前置来源 || 来源.构式 != 结构.所用构式
        || 来源.起始标量 != 缺口->起始标量 || 来源.结束标量 != 缺口->结束标量
        || std::ranges::find(缺口->已取得相关节点, 来源.词条) == 缺口->已取得相关节点.end())
        throw std::logic_error("saved pending predicate source inconsistent");
    return 接收待完善谓项{&*缺口, {身份.信息->节点, 来源.词条, 来源.构式, 归类->角色编号}};
}

std::vector<语言场景待完善概念词用> 读取接收待完善概念词用组(认知依赖& d,
    const 新接收语言解释内容& 内容, std::stop_token stop) {
    std::vector<语言场景待完善概念词用> 词用组;
    for (const auto& 句 : 内容.句段组) {
        检查认知停止(stop);
        if (const auto 谓项 = 读取接收待完善谓项(d, 内容.前置来源, 句))
            词用组.push_back(谓项->词用);
    }
    return 词用组;
}

struct 接收待完善特征角色 final {
    新语言知识参数角色 参数;
    const 新前置语言缺口* 缺口;
    语言场景待完善概念词用 词用;
};

std::vector<接收待完善特征角色> 读取接收待完善特征角色(认知依赖& d,
    稳定编码 前置来源, const 接收语言句段解释& 句) {
    using P = 新语言知识参数角色;
    using C = 新语言知识角色类别;
    using S = 特征概念规则状态;
    std::vector<接收待完善特征角色> 结果;
    if (!句.静态结构 || 句.静态结构->操作 != 新语言知识含义操作::特征描述) return 结果;
    const auto& 结构 = *句.静态结构;
    for (const auto& 角色 : 结构.角色组) {
        if (角色.值 || (角色.参数 != P::特征类型 && 角色.参数 != P::特征值)) continue;
        const auto 类别 = 角色.参数 == P::特征类型 ? C::正式特征类型 : C::特征概念;
        const auto 同范围 = [&](const auto& 项) {
            return 项.起始标量 == 角色.起始标量 && 项.结束标量 == 角色.结束标量;
        };
        const auto 依据 = std::ranges::find_if(句.词条类型依据组, [&](const auto& 项) {
            return 同范围(项) && 项.所需类别 == 类别 && 项.构式依据组 == std::vector{结构.所用构式};
        });
        if (依据 == 句.词条类型依据组.end()) continue;
        const auto 缺口 = std::ranges::find_if(句.缺口组, [&](const auto& 项) {
            return 同范围(项) && 项.种类 == 新前置语言缺口种类::名称材料不足
                && std::ranges::find(项.已取得相关节点, 依据->词条) != 项.已取得相关节点.end();
        });
        if (缺口 == 句.缺口组.end()) continue;
        std::optional<稳定编码> 概念;
        for (const auto 节点 : 缺口->已取得相关节点) {
            if (节点 == 结构.所用构式 || 节点 == 依据->词条) continue;
            const auto 读取 = d.特征概念.按节点读取待完善特征概念(节点);
            if (读取.状态 == S::资源失败) 资源();
            if (读取.状态 == S::前提已变化) 漂移();
            if (读取.状态 == S::值不存在) continue;
            if (读取.状态 != S::已完成 || !读取.信息)
                throw std::logic_error("saved pending feature role unavailable");
            const auto& 信息 = *读取.信息;
            if (信息.采用来源 != 前置来源 || 信息.来源词条 != 依据->词条 || !信息.原文用法
                || 信息.原文用法->构式 != 结构.所用构式
                || 信息.原文用法->起始标量 != 角色.起始标量 || 信息.原文用法->结束标量 != 角色.结束标量) continue;
            if (信息.已完善) 漂移();
            if (概念 && *概念 != 节点) return {};
            概念 = 节点;
        }
        if (概念) 结果.push_back({角色.参数, &*缺口,
            {*概念, 依据->词条, 结构.所用构式, 角色.角色编号}});
    }
    return 结果;
}

bool 核待完善特征陈述回读(const 接收语言句段解释& 原句, const 新静态含义单元& 原含义,
    const std::vector<接收待完善特征角色>& 同源角色组, 接收语言句段解释& 回读) {
    using P = 新语言知识参数角色;
    using C = 新语言知识角色类别;
    if (!原句.静态结构 || !回读.静态结构 || 回读.静态结构->所用构式 != 原句.静态结构->所用构式
        || 回读.静态结构->操作 != 原句.静态结构->操作
        || 回读.静态结构->角色组.size() != 原句.静态结构->角色组.size()
        || 回读.缺口组.size() != 同源角色组.size()) return false;
    const auto* 对象 = std::get_if<新绑定指称>(&原含义.主体);
    if (!对象) return false;
    for (const auto& 原角色 : 原句.静态结构->角色组) {
        const auto 现角色 = std::ranges::find(回读.静态结构->角色组, 原角色.参数, &接收语言已解释角色::参数);
        if (现角色 == 回读.静态结构->角色组.end() || 现角色->角色编号 != 原角色.角色编号) return false;
        if (原角色.参数 == P::主体) {
            const auto* 引用 = 现角色->值 ? std::get_if<语言图引用值>(&*现角色->值) : nullptr;
            if (!引用 || 引用->类别 != C::存在 || 引用->节点 != 对象->节点) return false;
            continue;
        }
        if (原角色.值) {
            const auto* 原引用 = std::get_if<语言图引用值>(&*原角色.值);
            const auto* 现引用 = 现角色->值 ? std::get_if<语言图引用值>(&*现角色->值) : nullptr;
            if (!原引用 || !现引用 || 原引用->类别 != 现引用->类别 || 原引用->节点 != 现引用->节点) return false;
            continue;
        }
        const auto 同源 = std::ranges::find(同源角色组, 原角色.参数, &接收待完善特征角色::参数);
        if (同源 == 同源角色组.end() || 现角色->值) return false;
        const auto 类别 = 原角色.参数 == P::特征类型 ? C::正式特征类型 : C::特征概念;
        const auto 同范围 = [&](const auto& 项) {
            return 项.起始标量 == 现角色->起始标量 && 项.结束标量 == 现角色->结束标量;
        };
        const auto 依据 = std::ranges::find_if(回读.词条类型依据组, [&](const auto& 项) {
            return 同范围(项) && 项.所需类别 == 类别 && 项.构式依据组 == std::vector{同源->词用.构式};
        });
        if (依据 == 回读.词条类型依据组.end() || 依据->词条 != 同源->词用.词条) return false;
        const auto 缺口 = std::ranges::find_if(回读.缺口组, [&](const auto& 项) {
            return 同范围(项) && 项.种类 == 新前置语言缺口种类::名称材料不足;
        });
        if (缺口 == 回读.缺口组.end()) return false;
        缺口->已取得相关节点.push_back(同源->词用.概念节点);
    }
    return true;
}

const 接收语言关系候选* 读取可成文关系(const 接收语言句段解释& 句) {
    using C=新语言知识角色类别;
    if (句.状态!=语言场景基础输入状态::已形成值式图
        || !句.关系观察 || 句.关系观察->读取未完成 || 句.关系观察->候选组.size()!=1)
        return nullptr;
    const auto& 候选=句.关系观察->候选组.front();
    const bool 端点已定=std::ranges::all_of(候选.端点组,[](const auto& 端点) {
        return 端点.指称 && 端点.指称->类别==C::存在 && 端点.起始标量<端点.结束标量;
    });
    return 端点已定 ? &候选 : nullptr;
}

bool 接收关系语义一致(const 接收语言关系候选& 原, const 接收语言关系候选& 实际) {
    return 原.关系概念==实际.关系概念 && 原.种子键==实际.种子键 && 原.所用构式==实际.所用构式
        && 原.源角色键==实际.源角色键 && 原.目标角色键==实际.目标角色键
        && std::ranges::equal(原.端点组,实际.端点组,[](const auto& 左,const auto& 右) {
            return 左.角色键==右.角色键 && 左.参数==右.参数 && 左.指称->类别==右.指称->类别
                && 左.指称->节点==右.指称->节点 && 左.指称->来源概念==右.指称->来源概念;
        });
}

bool 处理局部组成材料(认知依赖& d, 接收需求认知观察& out,
    const 新正式消息角色材料& 角色, const 新接收语言解释信息& 解释, std::stop_token stop) {
    using P = 新语言知识参数角色;
    using A = 基础节点组追加状态;
    const auto& 内容 = 解释.内容;
    if (!角色.事实 || !角色.内部交互场景 || !角色.说话者 || !角色.受话者
        || 内容.原文起点 != 0 || 标量数(角色.消息.UTF8正文) != 内容.原文终点
        || !可读取无上下文外层共指(角色.消息.UTF8正文)) return false;
    const 接收语言句段解释* 来源句 = nullptr;
    const 接收语言关系候选* 关系 = nullptr;
    for (const auto& 句 : 内容.句段组) {
        const auto* 候选 = 读取可成文关系(句);
        if (!候选 || 候选->种子键 != "existence.composition") continue;
        if (关系) return false;
        关系 = 候选;
        来源句 = &句;
    }
    if (!关系 || 关系->端点组.size() != 2 || 关系->源角色键 != "整体" || 关系->目标角色键 != "部分")
        return false;
    const auto 整体 = std::ranges::find(关系->端点组, P::整体, &接收语言关系端点::参数);
    const auto 部分 = std::ranges::find(关系->端点组, P::部分, &接收语言关系端点::参数);
    if (整体 == 关系->端点组.end() || 部分 == 关系->端点组.end()
        || 整体->角色键 != "整体" || 部分->角色键 != "部分"
        || 整体->指称->节点 == 部分->指称->节点) return false;
    const auto M = 角色.消息.消息节点;
    std::map<稳定编码, 稳定编码> 声明消息;
    auto 称名 = 读取同窗已声明局部称名(d, 角色, stop, &声明消息);
    auto 同文称名 = 读取同源局部称名(d, {M, 内容.原文起点, 内容.原文终点, {}}, stop, 来源句->起始标量);
    for (const auto& 名 : 同文称名) {
        称名.push_back(名);
        声明消息.emplace(名.存在节点, M);
    }
    const auto 原文 = 语言场景转换内部::解码语言场景原文(角色.消息.UTF8正文);
    if (!原文) throw std::logic_error("local composition source is not UTF-8");
    const auto 验端点 = [&](const 接收语言关系端点& 端) {
        if (端.起始标量 < 来源句->起始标量 || 端.结束标量 > 来源句->结束标量
            || !声明消息.contains(端.指称->节点)) return false;
        const auto 词面 = 语言场景转换内部::读取语言场景原文范围(*原文,
            {static_cast<std::size_t>(端.起始标量), static_cast<std::size_t>(端.结束标量)});
        return !词面.empty() && std::ranges::count(称名, 词面, &语言场景局部称名::词面) == 1
            && std::ranges::any_of(称名, [&](const auto& 项) {
                return 项.词面 == 词面 && 项.存在节点 == 端.指称->节点;
            });
    };
    if (!验端点(*整体) || !验端点(*部分)) return false;
    const auto 父 = 整体->指称->节点;
    const auto 子 = 部分->指称->节点;
    if (声明消息.at(父) != 声明消息.at(子)) return false;
    检查认知停止(stop);
    const auto 时间 = UTC时钟适配器{}.读取当前UTC时间证据_v1({});
    if (!时间.成功()) throw std::logic_error("local composition lacks adoption UTC");
    const auto 已写 = d.语言世界.采用接收局部组成(声明消息.at(子), 父, 子,
        {M, 解释.解释定义节点, 来源句->起始标量, 来源句->结束标量},
        角色.证据预期, {新状态时间语义::实例绝对UTC, 时间.证据->UTC纳秒});
    if (已写.状态 == A::入口拒绝) return false;
    if (已写.状态 == A::资源失败) 资源();
    if (已写.状态 == A::预期不符 || 已写.状态 == A::节点不存在 || 已写.状态 == A::引用不存在) 漂移();
    if (已写.状态 != A::已完成 || !已写.信息 || 已写.信息->子 != 子 || 已写.信息->父 != 父)
        throw std::logic_error("local composition atomic publication failed");
    out.局部组成子 = 子;
    return true;
}

void 追加正式关系成文观察(认知依赖& d, const 新接收语言解释信息& 正式,
    const 新接收认知来源键& 来源, const 语言图交互上下文& 交互,
    接收部分成文观察& 观察, std::stop_token stop) {
    using S=接收篇章成文状态;
    using B=基础表达状态;
    using I=语言场景基础输入状态;
    using L=新语言分界工程状态;
    if (!d.关系陈述) return;
    for (const auto& 句 : 正式.内容.句段组) {
        检查认知停止(stop);
        const auto* 原=读取可成文关系(句);
        if (!原) continue;
        auto& 输出=观察.句段组.emplace_back(句.起始标量,句.结束标量);
        auto 称名=读取同源局部称名(d,来源,stop);
        if (原->种子键 == "existence.composition") {
            using R = 新接收指代调用状态;
            const auto 角色 = d.语言.读取正式消息角色(来源.接收消息节点, 新外设消息方向::接收);
            if (角色.状态 == R::资源失败) 资源();
            if (角色.状态 == R::前提已变化) 漂移();
            if (角色.状态 == R::已完成 && 角色.信息) {
                auto 前文 = 读取同窗已声明局部称名(d, *角色.信息, stop);
                for (const auto& 名 : 前文) {
                    const auto 相同 = [&](const auto& 项) { return 项.存在节点 == 名.存在节点 && 项.词面 == 名.词面; };
                    if (std::ranges::none_of(称名, 相同)) 称名.push_back(名);
                }
            }
        }
        const auto 成文=d.关系陈述->生成接收关系陈述(正式.内容.语言节点,*原,交互,称名);
        if (成文.状态==B::读取漂移 || 成文.状态==B::引用不可读) 漂移();
        if (成文.状态==B::工程未完成 || 成文.状态==B::额度不足) 资源();
        if (成文.状态==B::内部不一致) throw std::logic_error("received relation generation inconsistent");
        输出.成文状态=成文.状态;
        if (成文.状态!=B::已生成 || 成文.文本.empty()) {
            输出.状态=S::成文受阻;
            continue;
        }
        输出.UTF8正文=成文.文本;

        const auto 当前=d.语言.读取接收语言解释(来源,d.语言世界,d.外设);
        if (当前.状态==L::资源失败) 资源();
        if (当前.状态==L::未找到 || 当前.状态==L::前提已变化) 漂移();
        if (当前.状态!=L::已完成 || !当前.信息)
            throw std::logic_error("received relation source unavailable");
        if (当前.信息->记录节点!=正式.记录节点 || 当前.信息->内容!=正式.内容) 漂移();
        const auto& 回读称名=称名;
        接收专名读取器 专名(d,交互.现实范围根,stop);
        语言基础统一输入请求 回读{正式.内容.语言节点,输出.UTF8正文,{},0,0,0,0,0,
            &专名,语言解释读取模式::本次正式有限集完整读取};
        回读.交互上下文=交互;
        回读.读取静态角色缺口=true;
        回读.局部称名组=回读称名;
        const auto 实际=只读解释单句基础语言(回读,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
        if (实际.状态==I::读取漂移 || 实际.状态==I::引用不可读) 漂移();
        if (实际.状态==I::工程未完成 || 实际.状态==I::额度不足) 资源();
        if (实际.状态==I::内部不一致) throw std::logic_error("received relation independent readback inconsistent");
        auto& 实际句=输出.独立回读.emplace_back(记录句段解释(实际,
            {0,static_cast<std::size_t>(标量数(输出.UTF8正文))},d.关系陈述));
        const auto* 已回读=读取可成文关系(实际句);
        输出.状态=!已回读 ? S::回读未唯一
            : 接收关系语义一致(*原,*已回读) ? S::已成文且独立回读 : S::回读含义不一致;
    }
}

void 转接静态成文已选名称(const 接收语言句段解释& 句, 语言图含义材料& 图) {
    using P = 新语言知识参数角色;
    if (!句.静态结构 || 图.定义图.节点组.size() != 1 || 图.出现组.size() != 1) return;
    const auto& 定义 = 图.定义图.节点组.front();
    const auto& 源角色组 = 句.静态结构->角色组;
    auto& 绑定组 = 图.出现组.front().角色组;
    for (const auto 参数 : {P::归类概念, P::特征类型, P::特征值}) {
        const auto 源角色 = std::ranges::find(源角色组, 参数, &接收语言已解释角色::参数);
        if (源角色 == 源角色组.end() || !源角色->值
            || 源角色->起始标量 < 句.起始标量 || 源角色->结束标量 > 句.结束标量
            || 源角色->起始标量 >= 源角色->结束标量
            || std::ranges::count(源角色组, 参数, &接收语言已解释角色::参数) != 1) continue;
        const auto* 源引用 = std::get_if<语言图引用值>(&*源角色->值);
        if (!源引用 || !源引用->选用名称关系) continue;
        const auto 定义角色 = std::ranges::find(定义.参数组, 参数, &新语言知识角色参数::参数);
        if (定义角色 == 定义.参数组.end()) continue;
        const auto 绑定 = std::ranges::find(绑定组, 定义角色->规则角色, &语言图角色绑定::角色编号);
        auto* 投影引用 = 绑定 == 绑定组.end() ? nullptr : std::get_if<语言图引用值>(&绑定->值);
        if (投影引用 && 投影引用->节点 == 源引用->节点)
            投影引用->选用名称关系 = 源引用->选用名称关系;
    }
}

std::optional<接收部分成文句段> 从接收具名部分句段成文(认知依赖& d,
    const 新接收语言解释信息& 正式, const 新接收认知来源键& 来源,
    const std::optional<语言图交互上下文>& 交互, std::stop_token stop, const char*& 步骤) {
    using P = 新语言知识参数角色;
    using O = 新语言知识含义操作;
    using W = 新语言知识表达方式;
    using K = 新语言知识状态;
    using L = 新语言分界工程状态;
    using G = 新前置语言缺口种类;
    using C = 新语言知识角色类别;
    using S = 接收篇章成文状态;
    const auto& 内容 = 正式.内容;
    if (内容.句段组.size() != 1 || 内容.原文起点 != 0 || !来源.根出现路径.empty()) return {};
    const auto& 句 = 内容.句段组.front();
    if (!句.静态结构 || 句.起始标量 != 0 || 句.结束标量 != 内容.原文终点) return {};
    const auto& 结构 = *句.静态结构;
    const bool 新增 = 结构.操作 == O::特征新增请求;
    const bool 上升 = 结构.操作 == O::近期完成上升描述;
    const bool 需求 = 结构.操作 == O::需求描述;
    const bool 当前值询问 = 结构.操作 == O::信息请求;
    if ((!新增 && !上升 && !需求 && !当前值询问)
        || (!需求 && 结构.角色组.size() != (新增 ? 2 : 上升 ? 4 : 3))) return {};
    const auto 取角色 = [&](P 参数) {
        return std::ranges::find(结构.角色组, 参数, &接收语言已解释角色::参数);
    };
    const auto 主体 = 取角色(需求 ? P::目标存在 : P::主体);
    const auto 类型 = 取角色(P::特征类型), 载荷 = 取角色(P::特征值);
    const auto 需求主体 = 取角色(P::主体);
    if (主体 == 结构.角色组.end() || 类型 == 结构.角色组.end()) return {};
    const auto* 已绑定主体 = 主体->值 ? std::get_if<语言图引用值>(&*主体->值) : nullptr;
    if (主体->值 && ((!当前值询问 && !上升 && !新增) || !交互 || !已绑定主体 || 已绑定主体->类别 != C::存在)) return {};
    if (需求 && (需求主体 == 结构.角色组.end() || 需求主体->值
        || 结构.角色组.size() != (载荷 == 结构.角色组.end() ? 3 : 4))) return {};
    if ((新增 && 类型->值) || (!新增 && !类型->值)) return {};
    if (新增 || 上升) {
        if (载荷 != 结构.角色组.end()) return {};
    } else if (!需求 && (载荷 == 结构.角色组.end() || 载荷->值)) return {};
    const auto 取数 = [&](P 参数) -> const 语言图整数值* {
        const auto 角色 = 取角色(参数);
        return 角色 == 结构.角色组.end() || !角色->值 ? nullptr
            : std::get_if<语言图整数值>(&*角色->值);
    };
    if (上升) {
        const auto* 前 = 取数(P::前值);
        const auto* 后 = 取数(P::后值);
        if (!前 || !后 || !前->单位 || 前->单位 != 后->单位) return {};
    }
    const auto* 类型值 = 类型->值 ? std::get_if<语言图引用值>(&*类型->值) : nullptr;
    if (!新增 && (!类型值 || !类型值->选用名称关系
        || (类型值->类别 != C::特征概念 && 类型值->类别 != C::正式特征类型
            && !(需求 && 类型值->类别 == C::I64标量特征类型)))) return {};
    const auto 同范围 = [](const auto& 缺口, const auto& 角色) {
        return 缺口.起始标量 == 角色.起始标量 && 缺口.结束标量 == 角色.结束标量;
    };
    const auto 主体缺口 = [&](const auto& 缺口) {
        return 缺口.种类 == G::对象未唯一绑定 && 同范围(缺口,*主体);
    };
    const auto 类型缺口 = [&](const auto& 缺口) {
        return 同范围(缺口,*类型) && (缺口.种类 == G::名称材料不足
            || (新增 && 缺口.种类 == G::角色原文待确认));
    };
    const auto 说话者缺口 = [&](const auto& 缺口) {
        return 需求 && 缺口.种类 == G::对象未唯一绑定 && 同范围(缺口,*需求主体);
    };
    if (std::ranges::count_if(句.缺口组,主体缺口) != (已绑定主体 ? 0 : 1)
        || (需求 && std::ranges::count_if(句.缺口组,说话者缺口) != 1)
        || (新增 && std::ranges::count_if(句.缺口组,类型缺口) != 1)
        || std::ranges::any_of(句.缺口组,[&](const auto& 缺口) {
            return !主体缺口(缺口) && !类型缺口(缺口) && !说话者缺口(缺口);
        })) return {};

    步骤 = "读取部分句段构式和正式词用";
    const auto 知识 = d.语言.获取语言知识(结构.所用构式, d.词, d.存在概念);
    if (知识.状态 == K::读取漂移 || 知识.状态 == K::未找到 || 知识.状态 == K::引用不可读) 漂移();
    if (知识.状态 == K::工程未完成 || 知识.状态 == K::额度不足) 资源();
    if (知识.状态 != K::已完成 || !知识.信息) throw std::logic_error("saved question knowledge unavailable");
    const auto& 定义 = 知识.信息->定义;
    const auto& 图 = 定义.目标.含义图;
    const auto* 目标图 = 需求 ? 部分特征需求目标图(定义) : nullptr;
    if (需求 && (!目标图 || 目标图->特征值分支.has_value() != (载荷 != 结构.角色组.end()))) return {};
    const auto* 角色图 = 新增 || 上升
        ? (图 && 图->节点组.size() == 1 && 图->节点组.front().操作 == 结构.操作
            ? &图->节点组.front() : nullptr)
        : 当前特征值问图子节点(定义);
    if (!需求 && !角色图) return {};
    const auto 固定 = d.语言.读取前置来源(来源,d.外设);
    if (固定.状态 == L::资源失败) 资源();
    if (固定.状态 == L::前提已变化 || 固定.状态 == L::未找到 || 固定.状态 == L::材料未就绪) 漂移();
    if (固定.状态 != L::已完成 || !固定.信息 || 固定.信息->节点 != 内容.前置来源)
        throw std::logic_error("saved question source unavailable");
    const auto& 原文 = 固定.信息->固定UTF8原文;
    if ((!新增 && !可读取无上下文外层共指(原文)) || 标量数(原文) != 句.结束标量) return {};
    const auto 解码 = 语言场景转换内部::解码语言场景原文(原文);
    if (!解码) throw std::logic_error("saved question source is not UTF-8");
    const auto 未决词面 = [&](const 接收语言已解释角色& 角色) {
        return 语言场景转换内部::读取语言场景原文范围(*解码,
            {static_cast<std::size_t>(角色.起始标量),static_cast<std::size_t>(角色.结束标量)});
    };
    const auto 取同窗称名 = [&]() {
        using R = 新接收指代调用状态;
        const auto 角色 = d.语言.读取正式消息角色(来源.接收消息节点, 新外设消息方向::接收);
        if (角色.状态 == R::资源失败) 资源();
        if (角色.状态 == R::前提已变化 || 角色.状态 == R::未找到) 漂移();
        if (角色.状态 != R::已完成 || !角色.信息)
            throw std::logic_error("saved bound question roles unavailable");
        if (角色.信息->说话者 != 交互->说话者H || 角色.信息->受话者 != 交互->受话者E) 漂移();
        return 读取同窗已声明局部称名(d, *角色.信息, stop, nullptr, 新增 ? &句 : nullptr);
    };
    const auto 主体原词面 = 未决词面(*主体);
    const auto 取唯一称名 = [&](const std::vector<语言场景局部称名>& 名称组)
        -> std::optional<std::string> {
        const auto 位置 = std::ranges::find(名称组, 主体原词面, &语言场景局部称名::词面);
        if (位置 == 名称组.end() || 位置->存在节点 != 已绑定主体->节点
            || std::ranges::count(名称组, 主体原词面, &语言场景局部称名::词面) != 1) return {};
        return 位置->词面;
    };
    const auto 正式主体称名 = 已绑定主体 ? 取唯一称名(取同窗称名()) : std::optional<std::string>{};
    if (已绑定主体 && !正式主体称名) return {};

    std::optional<稳定编码> 类型词条;
    if (!新增) {
        using N = 概念名称关系查询状态;
        const auto 名称 = d.名称.获取名称关系(*类型值->选用名称关系);
        if (名称.状态 == N::工程未完成 || 名称.状态 == N::读取未完成 || 名称.状态 == N::资源失败) 资源();
        if (名称.状态 == N::结构不一致 || 名称.状态 == N::输入不合法)
            throw std::logic_error("saved question feature name inconsistent");
        if (名称.状态 != N::已找到 || !名称.信息 || 名称.信息->概念节点 != 类型值->节点) 漂移();
        类型词条 = 名称.信息->词条节点;
    }

    接收部分成文句段 输出(句.起始标量,句.结束标量);
    const auto 加词 = [&](稳定编码 节点) {
        using T = 新词条查询状态;
        const auto 词条 = d.词.获取词条(节点);
        if (词条.状态 == T::资源失败) 资源();
        if (词条.状态 == T::结构不一致) throw std::logic_error("saved question word inconsistent");
        if (词条.状态 != T::已找到 || !词条.信息 || 词条.信息->语言节点 != 内容.语言节点) 漂移();
        输出.UTF8正文 += 词条.信息->规范化词面;
    };
    for (const auto& 项 : 定义.结构.有序项组) {
        检查认知停止(stop);
        if (const auto* 固定词 = std::get_if<新语言知识固定词项>(&项.载荷)) {
            加词(固定词->词条节点);
            continue;
        }
        const auto* 角色 = std::get_if<新语言知识图角色表达项>(&项.载荷);
        if (!角色 || (!需求 && 角色->角色.含义编号 != 角色图->局部编号)) return {};
        const auto 角色号 = 需求 ? 项.局部编号 : 角色->角色.角色编号;
        if (需求 && 角色号 == 需求主体->角色编号 && 角色->方式 == W::已消解指代
            && 角色->指代名称关系节点) {
            const auto 指代 = d.名称.获取指代名称关系(*角色->指代名称关系节点);
            using N = 指代名称读取状态;
            if (指代.状态 == N::资源失败) 资源();
            if (指代.状态 != N::已读取 || !指代.信息) 漂移();
            if (指代.信息->概念.定义.规则 != 指代声明规则::作用域说话者
                || 指代.信息->语言.节点 != 内容.语言节点) return {};
            加词(指代.信息->词条.节点);
        } else if (角色号 == 主体->角色编号 && 角色->方式 == W::明确对象描述)
            输出.UTF8正文 += 正式主体称名 ? *正式主体称名 : 主体原词面;
        else if (角色号 == 类型->角色编号 && 角色->方式 == W::名称表达) {
            if (类型词条) 加词(*类型词条);
            else 输出.UTF8正文 += 未决词面(*类型);
        } else if (!新增 && !上升 && !需求 && 角色号 == 载荷->角色编号
            && 角色->方式 == W::查询变量词面 && 角色->查询词条节点) {
            加词(*角色->查询词条节点);
        } else if ((上升 || 需求) && 角色->方式 == W::类型化字面值) {
            const auto 实参 = std::ranges::find(结构.角色组,角色号,&接收语言已解释角色::角色编号);
            if (实参 == 结构.角色组.end() || (需求 ? 实参->参数 != P::特征值
                : (实参->参数 != P::前值 && 实参->参数 != P::后值))) return {};
            const auto* 数 = 取数(实参->参数);
            if (!数 || !数->单位) return {};
            const auto 单位词 = d.语言.按计量单位读取词条(内容.语言节点,*数->单位);
            if (单位词.状态 == K::工程未完成 || 单位词.状态 == K::额度不足) 资源();
            if (单位词.状态 == K::内部不一致) throw std::logic_error("saved rise unit inconsistent");
            if (单位词.状态 != K::已完成 || !单位词.匹配节点) 漂移();
            输出.UTF8正文 += ' ';
            输出.UTF8正文 += 数->规范十进制;
            输出.UTF8正文 += ' ';
            加词(*单位词.匹配节点);
        } else return {};
    }
    if (输出.UTF8正文.empty()) return {};
    输出.成文状态 = 基础表达状态::已生成;

    步骤 = "独立读回正式部分句段";
    std::optional<接收专名读取器> 专名;
    if (交互) 专名.emplace(d,交互->现实范围根,stop);
    语言基础统一输入请求 回读{内容.语言节点,输出.UTF8正文,{},0,0,0,0,0,
        专名 ? &*专名 : nullptr,语言解释读取模式::本次正式有限集完整读取};
    回读.交互上下文 = 交互;
    回读.读取静态角色缺口 = true;
    const auto 回读称名 = 已绑定主体 ? 取同窗称名() : std::vector<语言场景局部称名>{};
    if (已绑定主体 && 取唯一称名(回读称名) != 正式主体称名) 漂移();
    回读.局部称名组 = 回读称名;
    if (需求) {
        using R = 新接收指代调用状态;
        const auto 角色 = d.语言.读取正式消息角色(来源.接收消息节点,新外设消息方向::接收);
        if (角色.状态 == R::资源失败) 资源();
        if (角色.状态 == R::前提已变化 || 角色.状态 == R::未找到) 漂移();
        if (角色.状态 != R::已完成 || !角色.信息) throw std::logic_error("saved demand person roles unavailable");
        if (角色.信息->说话者) 漂移();
        回读.正式人称 = 语言图原消息人称{角色.信息->说话者,角色.信息->受话者};
    }
    const auto 实际 = 只读解释单句基础语言(回读,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
    using I = 语言场景基础输入状态;
    if (实际.状态 == I::读取漂移 || 实际.状态 == I::引用不可读) 漂移();
    if (实际.状态 == I::工程未完成 || 实际.状态 == I::额度不足) 资源();
    if (实际.状态 == I::内部不一致) throw std::logic_error("partial request independent readback inconsistent");
    const auto& 回读证据 = 输出.独立回读.emplace_back(记录句段解释(实际,
        {0,static_cast<std::size_t>(标量数(输出.UTF8正文))}));
    const auto 当前 = d.语言.读取接收语言解释(来源,d.语言世界,d.外设);
    if (当前.状态 == L::资源失败) 资源();
    if (当前.状态 == L::未找到 || 当前.状态 == L::前提已变化) 漂移();
    if (当前.状态 != L::已完成 || !当前.信息) throw std::logic_error("partial request context unavailable");
    if (当前.信息->记录节点 != 正式.记录节点 || 当前.信息->解释定义节点 != 正式.解释定义节点
        || 当前.信息->内容 != 内容 || 当前.信息->篇章采用) 漂移();
    输出.状态 = !回读证据.静态结构 ? S::回读未唯一
        : 回读证据.静态结构 == 句.静态结构 && 回读证据.缺口组 == 句.缺口组
            && 回读证据.结束标量 == 句.结束标量 && !实际.含义
        ? S::已成文且独立回读 : S::回读含义不一致;
    return 输出;
}

void 关联同文待绑定任务(std::vector<接收语言句段解释>& 句段, const std::string& 原文) {
    using O = 新语言知识含义操作;
    using P = 新语言知识参数角色;
    if (句段.size() != 2 || (!可读取无上下文外层共指(原文)
        && !仅含类型词面引号(原文,句段.front(),true))) return;
    const auto& 首句 = 句段.front();
    auto& 末句 = 句段.back();
    if (!首句.静态结构 || 首句.静态结构->操作!=O::任务用途描述
        || !末句.静态结构 || 末句.静态结构->操作!=O::任务当前等待描述
        || 首句.主体关系 || 末句.主体关系) return;
    const auto 解码 = 语言场景转换内部::解码语言场景原文(原文);
    if (!解码) return;
    const auto& 首角色 = 首句.静态结构->角色组;
    const auto& 末角色 = 末句.静态结构->角色组;
    const auto 首 = std::ranges::find(首角色,P::主体,&接收语言已解释角色::参数);
    const auto 末 = std::ranges::find(末角色,P::主体,&接收语言已解释角色::参数);
    if (首 == 首角色.end() || 末 == 末角色.end() || 首->值 || 末->值) return;
    const auto 词面 = [&](const auto& 角色) {
        return 语言场景转换内部::读取语言场景原文范围(*解码,
            {static_cast<std::size_t>(角色.起始标量),static_cast<std::size_t>(角色.结束标量)});
    };
    if (词面(*首) != "这项任务" || 词面(*末) != "任务") return;
    末句.主体关系 = 接收语言主体关系{末->起始标量,末->结束标量,首->起始标量,首->结束标量,
        末句.静态结构->所用构式,{},false};
}

std::optional<接收部分成文句段> 从接收任务两句成文(认知依赖& d,
    const 新接收语言解释信息& 正式, const 新接收认知来源键& 来源, std::stop_token stop, const char*& 步骤) {
    using P = 新语言知识参数角色;
    using O = 新语言知识含义操作;
    using W = 新语言知识表达方式;
    using K = 新语言知识状态;
    using L = 新语言分界工程状态;
    using S = 接收篇章成文状态;
    const auto& 内容 = 正式.内容;
    if (内容.句段组.size()!=2 || 内容.原文起点!=0 || !来源.根出现路径.empty()
        || !内容.句段组.back().主体关系 || !内容.句段组.front().静态结构 || !内容.句段组.back().静态结构
        || 内容.句段组.front().静态结构->操作!=O::任务用途描述
        || 内容.句段组.back().静态结构->操作!=O::任务当前等待描述) return {};
    步骤 = "读取任务两句的正式来源";
    const auto 固定 = d.语言.读取前置来源(来源,d.外设);
    if (固定.状态 == L::资源失败) 资源();
    if (固定.状态 == L::前提已变化 || 固定.状态 == L::未找到 || 固定.状态 == L::材料未就绪) 漂移();
    if (固定.状态 != L::已完成 || !固定.信息 || 固定.信息->节点 != 内容.前置来源)
        throw std::logic_error("saved task description source unavailable");
    const auto& 原文 = 固定.信息->固定UTF8原文;
    const auto 解码 = 语言场景转换内部::解码语言场景原文(原文);
    if (!解码 || (!可读取无上下文外层共指(原文)
        && !仅含类型词面引号(原文,内容.句段组.front(),true))) return {};
    接收部分成文句段 输出(内容.原文起点,内容.原文终点);
    const auto 加词 = [&](稳定编码 节点) {
        using T = 新词条查询状态;
        const auto 词条 = d.词.获取词条(节点);
        if (词条.状态 == T::资源失败) 资源();
        if (词条.状态 == T::结构不一致) throw std::logic_error("saved task word inconsistent");
        if (词条.状态 != T::已找到 || !词条.信息 || 词条.信息->语言节点 != 内容.语言节点) 漂移();
        输出.UTF8正文 += 词条.信息->规范化词面;
    };
    步骤 = "从正式任务用途与状态结构成文";
    for (const auto& 句 : 内容.句段组) {
        const auto& 结构 = *句.静态结构;
        const auto 知识 = d.语言.获取语言知识(结构.所用构式,d.词,d.存在概念);
        if (知识.状态 == K::读取漂移 || 知识.状态 == K::未找到 || 知识.状态 == K::引用不可读) 漂移();
        if (知识.状态 == K::工程未完成 || 知识.状态 == K::额度不足) 资源();
        if (知识.状态 != K::已完成 || !知识.信息) throw std::logic_error("saved task knowledge unavailable");
        for (const auto& 项 : 知识.信息->定义.结构.有序项组) {
            检查认知停止(stop);
            if (const auto* 词 = std::get_if<新语言知识固定词项>(&项.载荷)) {
                加词(词->词条节点);
                continue;
            }
            const auto* 表达 = std::get_if<新语言知识图角色表达项>(&项.载荷);
            const auto 角色 = std::ranges::find(结构.角色组,项.局部编号,&接收语言已解释角色::角色编号);
            if (!表达 || 角色 == 结构.角色组.end()) return {};
            if (表达->方式 == W::明确对象描述 && !角色->值
                && (角色->参数 == P::主体 || 角色->参数 == P::目标存在 || 角色->参数 == P::需求主体)) {
                输出.UTF8正文 += 语言场景转换内部::读取语言场景原文范围(*解码,
                    {static_cast<std::size_t>(角色->起始标量),static_cast<std::size_t>(角色->结束标量)});
            } else if (表达->方式 == W::已消解指代 && 角色->参数 == P::受请求者 && 表达->指代名称关系节点) {
                using N = 指代名称读取状态;
                const auto 指代 = d.名称.获取指代名称关系(*表达->指代名称关系节点);
                if (指代.状态 == N::资源失败) 资源();
                if (指代.状态 != N::已读取 || !指代.信息) 漂移();
                if (指代.信息->概念.定义.规则 != 指代声明规则::作用域受话者
                    || 指代.信息->语言.节点 != 内容.语言节点) return {};
                加词(指代.信息->词条.节点);
            } else if (表达->方式 == W::名称表达 && 角色->参数 == P::特征类型 && 角色->值) {
                using N = 概念名称关系查询状态;
                const auto* 型 = std::get_if<语言图引用值>(&*角色->值);
                if (!型 || !型->选用名称关系) return {};
                const auto 名称 = d.名称.获取名称关系(*型->选用名称关系);
                if (名称.状态 == N::工程未完成 || 名称.状态 == N::读取未完成 || 名称.状态 == N::资源失败) 资源();
                if (名称.状态 == N::结构不一致 || 名称.状态 == N::输入不合法)
                    throw std::logic_error("saved task feature name inconsistent");
                if (名称.状态 != N::已找到 || !名称.信息 || 名称.信息->概念节点 != 型->节点) 漂移();
                加词(名称.信息->词条节点);
            } else if (表达->方式 == W::类型化字面值 && 角色->参数 == P::特征值 && 角色->值) {
                const auto* 值 = std::get_if<语言图整数值>(&*角色->值);
                if (!值 || !值->单位) return {};
                const auto 单位 = d.语言.按计量单位读取词条(内容.语言节点,*值->单位);
                if (单位.状态 == K::工程未完成 || 单位.状态 == K::额度不足) 资源();
                if (单位.状态 == K::内部不一致) throw std::logic_error("saved task unit inconsistent");
                if (单位.状态 != K::已完成 || !单位.匹配节点) 漂移();
                输出.UTF8正文 += ' ';
                输出.UTF8正文 += 值->规范十进制;
                输出.UTF8正文 += ' ';
                加词(*单位.匹配节点);
            } else return {};
        }
    }
    输出.成文状态 = 基础表达状态::已生成;
    步骤 = "独立解释实际生成的任务两句";
    using R = 新接收指代调用状态;
    const auto 角色 = d.语言.读取正式消息角色(来源.接收消息节点,新外设消息方向::接收);
    if (角色.状态 == R::资源失败) 资源();
    if (角色.状态 == R::前提已变化 || 角色.状态 == R::未找到) 漂移();
    if (角色.状态 != R::已完成 || !角色.信息) throw std::logic_error("saved task person roles unavailable");
    for (const auto& 句 : 投影接收篇章句界(输出.UTF8正文)) {
        检查认知停止(stop);
        语言基础统一输入请求 回读{内容.语言节点,句.句面原文,{},0,0,0,0,0,
            nullptr,语言解释读取模式::本次正式有限集完整读取};
        回读.读取静态角色缺口 = true;
        回读.正式人称 = 语言图原消息人称{角色.信息->说话者,角色.信息->受话者};
        const auto 实际 = 只读解释单句基础语言(回读,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
        using I = 语言场景基础输入状态;
        if (实际.状态 == I::读取漂移 || 实际.状态 == I::引用不可读) 漂移();
        if (实际.状态 == I::工程未完成 || 实际.状态 == I::额度不足) 资源();
        if (实际.状态 == I::内部不一致) throw std::logic_error("partial task independent readback inconsistent");
        输出.独立回读.push_back(记录句段解释(实际,句.全文范围));
    }
    关联同文待绑定任务(输出.独立回读,输出.UTF8正文);
    const auto 当前 = d.语言.读取接收语言解释(来源,d.语言世界,d.外设);
    if (当前.状态 == L::资源失败) 资源();
    if (当前.状态 == L::未找到 || 当前.状态 == L::前提已变化) 漂移();
    if (当前.状态 != L::已完成 || !当前.信息) throw std::logic_error("partial task context unavailable");
    if (当前.信息->记录节点 != 正式.记录节点 || 当前.信息->解释定义节点 != 正式.解释定义节点
        || 当前.信息->内容 != 内容 || 当前.信息->篇章采用) 漂移();
    输出.状态 = 输出.独立回读 == 内容.句段组 ? S::已成文且独立回读 : S::回读含义不一致;
    return 输出;
}

std::optional<接收部分成文观察> 从接收部分含义成文并独立回读(认知依赖& d,
    const 新接收语言解释信息& 正式, const 新接收认知来源键& 来源,
    const std::optional<语言图交互上下文>& 交互, std::stop_token stop, const char*& 步骤) {
    using S = 接收篇章成文状态;
    using B = 基础表达状态;
    using I = 语言场景基础输入状态;
    using L = 新语言分界工程状态;
    const auto& 内容 = 正式.内容;
    if (正式.篇章采用) return {};
    步骤 = "核对部分成文正式材料";
    接收部分成文观察 结果{正式.记录节点};
    if (交互) 追加正式关系成文观察(d,正式,来源,*交互,结果,stop);
    const auto 完成本次观察 = [&]() -> std::optional<接收部分成文观察> {
        if (结果.句段组.empty()) return {};
        std::ranges::sort(结果.句段组,{},&接收部分成文句段::原文起点);
        return std::move(结果);
    };
    if (!内容.含义候选) {
        if (auto 任务两句 = 从接收任务两句成文(d,正式,来源,stop,步骤))
            结果.句段组.push_back(std::move(*任务两句));
        if (auto 请求句段 = 从接收具名部分句段成文(d,正式,来源,交互,stop,步骤))
            结果.句段组.push_back(std::move(*请求句段));
        return 完成本次观察();
    }
    const auto 成文词用 = 读取接收待完善概念词用组(d, 内容, stop);
    const auto 核同源指代 = [&](const 接收语言句段解释& 句, 稳定编码 主体节点) {
        using R = 新接收指代调用状态;
        const auto& 关系 = *句.主体关系;
        const auto 读回 = d.语言.读取当前接收指代关系组(来源, d.语言世界, d.外设);
        if (读回.状态 == R::资源失败) 资源();
        if (读回.状态 == R::前提已变化) 漂移();
        if (读回.状态 == R::未找到) return false;
        if (读回.状态 != R::已完成 || !读回.信息)
            throw std::logic_error("partial description reference unreadable");
        const 新接收指代出现键 出现{来源, {关系.起始标量, 关系.结束标量}, {}};
        const 新接收指代作用域段 句范围{句.起始标量, 句.结束标量};
        const 新接收指代关系值 目标{新接收指代直接目标{
            新绑定指称{新指称类别::存在, 主体节点}}};
        return std::ranges::count_if(*读回.信息, [&](const auto& 项) {
            return 项.出现 == 出现 && 项.句段范围 == 句范围 && 项.角色 == "主体"
                && 项.解释定义 == 正式.解释定义节点 && 项.值 == 目标;
        }) == 1;
    };

    const auto 成文称名 = 交互 ? 读取同源局部称名(d, 来源, stop) : std::vector<语言场景局部称名>{};
    for (const auto& 句 : 内容.句段组) {
        检查认知停止(stop);
        const auto 谓项 = 读取接收待完善谓项(d, 内容.前置来源, 句);
        const bool 存在归类 = 谓项 && 句.缺口组.size() == 1 && 谓项->缺口 == &句.缺口组.front();
        if (!交互 && !存在归类) continue;
        const auto 特征角色组 = 交互 ? 读取接收待完善特征角色(d, 内容.前置来源, 句)
            : std::vector<接收待完善特征角色>{};
        const bool 特征陈述 = !特征角色组.empty() && 句.缺口组.size() == 特征角色组.size()
            && std::ranges::all_of(句.缺口组, [&](const auto& 缺口) {
                return std::ranges::find(特征角色组, &缺口, &接收待完善特征角色::缺口) != 特征角色组.end();
            });
        const auto& 同文主体 = 句.主体关系;
        const bool 同文特征分类 = 交互 && 句.缺口组.empty() && 句.静态结构
            && 句.静态结构->操作 == 新语言知识含义操作::特征描述
            && 内容.含义候选->顶层项组.size() == 1
            && 内容.含义候选->顶层项组.front().表达用途 == 新表达用途::陈述
            && 同文主体 && 同文主体->已确定 && 同文主体->指代概念 && 同文主体->来源
            && 同文主体->来源->已绑定存在 && 同文主体->来源->已证冲突候选.empty()
            && 内容.原文起点 <= 同文主体->对象起始标量
            && 同文主体->对象起始标量 < 同文主体->对象结束标量
            && 同文主体->对象结束标量 <= 句.起始标量
            && 句.起始标量 <= 同文主体->起始标量
            && 同文主体->起始标量 < 同文主体->结束标量
            && 同文主体->结束标量 <= 句.结束标量;
        if (!存在归类 && !特征陈述 && !同文特征分类) continue;
        const 新含义原文范围 范围{static_cast<std::int64_t>(句.起始标量),
            static_cast<std::int64_t>(句.结束标量 - 句.起始标量)};
        const auto& 原网 = *内容.含义候选;
        const auto 项 = std::ranges::find_if(原网.内容项组, [&](const auto& 单元) {
            return 单元.原文范围组.size() == 1 && 单元.原文范围组.front() == 范围;
        });
        if (项 == 原网.内容项组.end()) continue;
        const auto* 单元 = std::get_if<新静态含义单元>(&项->内容);
        const auto* 主体 = 单元 ? std::get_if<新绑定指称>(&单元->主体) : nullptr;
        if (!主体) continue;
        步骤 = "读取部分成文同源称名";
        const auto 句内称名 = 同文特征分类 ? 读取同源局部称名(d, 来源, stop, 句.起始标量)
            : 交互 ? 成文称名 : 读取同源局部称名(d, 来源, stop, 句.结束标量);
        if (!交互 && std::ranges::none_of(句内称名, [&](const auto& 称名) {
                return 称名.存在节点 == 主体->节点;
            })) continue;
        新语言含义网 单句;
        单句.顶层项组.push_back({{}, 新表达用途::陈述, 项->编号});
        单句.内容项组.push_back(*项);
        std::vector<语言场景待完善概念词用> 特征词用组;
        if (特征陈述) {
            using P = 新语言知识参数角色;
            auto& 临时单元 = std::get<新静态含义单元>(单句.内容项组.front().内容);
            auto* 特征 = std::get_if<新特征值谓项>(&临时单元.谓项);
            if (!特征) continue;
            for (const auto& 角色 : 特征角色组) {
                const auto& 词用 = 角色.词用;
                if (角色.参数 == P::特征类型)
                    特征->特征类型 = 新绑定指称{新指称类别::特征类型, 词用.概念节点, {}};
                else
                    特征->值 = 新表达概念条件{词用.概念节点};
                特征词用组.push_back(词用);
            }
            if (!std::holds_alternative<新表达概念条件>(特征->值)) continue;
        } else if (同文特征分类) {
            const auto* 特征 = std::get_if<新特征值谓项>(&单元->谓项);
            const auto* 类型 = 特征 ? std::get_if<新绑定指称>(&特征->特征类型) : nullptr;
            if (主体->类别 != 新指称类别::存在 || 主体->节点 != *同文主体->来源->已绑定存在
                || !类型 || 类型->类别 != 新指称类别::特征类型
                || !std::holds_alternative<新表达概念条件>(特征->值)
                || std::ranges::count(句内称名, 主体->节点, &语言场景局部称名::存在节点) != 1
                || !核同源指代(句, 主体->节点)) continue;
        } else if (!std::holds_alternative<新概念归属谓项>(单元->谓项)) {
            continue;
        }

        auto& 输出 = 结果.句段组.emplace_back(句.起始标量, 句.结束标量);
        auto 投影 = 含义网投影旧图(单句, {内容.语言节点});
        if (!投影.图 || 投影.图->定义图.节点组.size() != 1) continue;
        const auto& 定义 = 投影.图->定义图.节点组.front();
        if (同文特征分类) 转接静态成文已选名称(句, *投影.图);
        基础表达请求 请求{};
        请求.语言节点 = 内容.语言节点;
        请求.目的 = 新语言知识表达目的::陈述;
        请求.操作 = 定义.操作;
        请求.作用域 = 定义.作用域;
        请求.含义图 = std::move(投影.图);
        if (交互) {
            请求.成文说话者 = 交互->说话者H;
            请求.成文受话者 = 交互->受话者E;
        }
        请求.允许根知识节点组 = 项->所用知识组;
        请求.局部称名组 = 句内称名;
        if (!同文特征分类) 请求.待完善概念词用组 = 特征陈述 ? 特征词用组 : 成文词用;
        步骤 = "生成部分成文";
        const auto 成文 = 生成基础语言表达(请求,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
        if (成文.状态 == B::读取漂移) 漂移();
        if (成文.状态 == B::工程未完成 || 成文.状态 == B::额度不足) 资源();
        if (成文.状态 == B::内部不一致) throw std::logic_error("partial statement generation inconsistent");
        输出.成文状态 = 成文.状态;
        if (成文.状态 != B::已生成 || 成文.文本.empty()) { 输出.状态 = S::成文受阻; continue; }
        输出.UTF8正文 = 成文.文本;

        步骤 = "复核部分成文解释版本";
        const auto 当前 = d.语言.读取接收语言解释(来源, d.语言世界, d.外设);
        if (当前.状态 == L::资源失败) 资源();
        if (当前.状态 == L::未找到 || 当前.状态 == L::前提已变化) 漂移();
        if (当前.状态 != L::已完成 || !当前.信息)
            throw std::logic_error("partial statement context unavailable");
        if (当前.信息->记录节点 != 正式.记录节点 || 当前.信息->内容 != 内容) 漂移();
        const auto 回读称名 = 读取同源局部称名(d, 来源, stop,
            同文特征分类 ? std::optional{句.起始标量}
                : 交互 ? std::nullopt : std::optional{句.结束标量});
        if (同文特征分类 && (!std::ranges::equal(回读称名, 句内称名, [](const auto& 左, const auto& 右) {
                return 左.存在节点 == 右.存在节点 && 左.词面 == 右.词面;
            }) || !核同源指代(句, 主体->节点))) 漂移();
        const auto 回读词用 = 特征陈述 || 同文特征分类 ? std::vector<语言场景待完善概念词用>{}
            : 读取接收待完善概念词用组(d, 当前.信息->内容, stop);
        std::optional<接收专名读取器> 专名;
        if (交互) 专名.emplace(d,交互->现实范围根,stop);
        语言基础统一输入请求 回读{内容.语言节点, 输出.UTF8正文, {},0,0,0,0,0,
            专名 ? &*专名 : nullptr, 语言解释读取模式::本次正式有限集完整读取};
        回读.交互上下文 = 交互;
        回读.读取静态角色缺口 = true;
        回读.局部称名组 = 回读称名;
        回读.待完善概念词用组 = 回读词用;
        步骤 = "独立解释部分成文";
        const auto 实际 = 只读解释单句基础语言(回读,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
        if (实际.状态 == I::读取漂移 || 实际.状态 == I::引用不可读) 漂移();
        if (实际.状态 == I::工程未完成 || 实际.状态 == I::额度不足) 资源();
        if (实际.状态 == I::内部不一致) throw std::logic_error("partial statement independent readback inconsistent");
        输出.独立回读.push_back(记录句段解释(实际,{0,static_cast<std::size_t>(标量数(输出.UTF8正文))}));
        if (特征陈述) {
            const auto 同源句 = std::ranges::find(当前.信息->内容.句段组, 句.起始标量,
                &接收语言句段解释::起始标量);
            if (同源句 == 当前.信息->内容.句段组.end()) 漂移();
            const auto 同源词用 = 读取接收待完善特征角色(d, 当前.信息->内容.前置来源, *同源句);
            if (!实际.静态结构) 输出.状态 = S::回读未唯一;
            else if (std::ranges::equal(同源词用, 特征角色组, {}, &接收待完善特征角色::词用,
                    &接收待完善特征角色::词用)
                && 核待完善特征陈述回读(*同源句, *单元, 同源词用, 输出.独立回读.back()))
                输出.状态 = S::已成文且独立回读;
            else 输出.状态 = S::回读含义不一致;
            continue;
        }
        const auto* 实际图 = 实际.含义 ? std::get_if<语言图含义材料>(&*实际.含义) : nullptr;
        if (实际.状态 != I::已形成值式图 || !实际图 || !实际.静态结构) {
            输出.状态 = S::回读未唯一;
            continue;
        }
        if (同文特征分类) {
            输出.状态 = 语言图结构语义一致(*请求.含义图, *实际图)
                ? S::已成文且独立回读 : S::回读含义不一致;
            continue;
        }
        const auto& 词用 = 谓项->词用;
        const bool 同词用 = 实际.静态结构->所用构式 == 词用.构式
            && std::ranges::any_of(实际.词条类型依据组, [&](const auto& 证据) {
                return 证据.词条 == 词用.词条 && 证据.构式依据组 == std::vector{词用.构式};
            });
        输出.状态 = 同词用 && 语言图结构语义一致(*请求.含义图, *实际图)
            ? S::已成文且独立回读 : S::回读含义不一致;
    }
    return 完成本次观察();
}

接收篇章成文观察 从采用篇章成文并独立回读(认知依赖& d, const 新接收篇章采用信息& 采用,
    const 新接收认知来源键& 来源, const 语言图交互上下文& 交互, std::stop_token 停止) {
    using S=接收篇章成文状态;
    using B=基础表达状态;
    using I=语言场景基础输入状态;
    接收篇章成文观察 结果{采用.节点};
    const auto& 采用内容=采用.内容;
    const auto& 网=采用内容.含义候选;
    if (!网 || 网->顶层项组.size()!=1) return 结果;
    const auto& 顶=网->顶层项组.front();
    if (顶.表达用途!=新表达用途::陈述) return 结果;
    const auto 取项=[&](std::int64_t 编号) {
        return std::ranges::find(网->内容项组,编号,&新含义内容项::编号);
    };
    const auto 根=取项(顶.内容引用);
    if (根==网->内容项组.end()) return 结果;
    std::vector<std::int64_t> 内容顺序{根->编号};
    if (const auto* 组合=std::get_if<新篇章组合>(&根->内容)) {
        if (组合->种类!=新篇章组合种类::文本顺序) return 结果;
        内容顺序=组合->有序子项组;
    }

    const auto 成文称名 = 读取同源局部称名(d, 来源, 停止);
    std::vector<语言图含义材料> 原含义;
    std::vector<解释原文范围> 成文范围;
    std::string 全文;
    std::size_t 标量=0;
    for (const auto 编号 : 内容顺序) {
        检查认知停止(停止);
        const auto 项=取项(编号);
        if (项==网->内容项组.end() || !std::holds_alternative<新静态含义单元>(项->内容)) return 结果;
        新语言含义网 单元网;
        单元网.顶层项组.push_back({{},顶.表达用途,编号});
        单元网.内容项组.push_back(*项);
        auto& 主体 = std::get<新静态含义单元>(单元网.内容项组.front().内容).主体;
        if (const auto* 指代 = std::get_if<新指代实例引用>(&主体)) {
            const auto 固定 = std::ranges::find(采用.指代目标组, 编号, &新接收篇章指代目标::内容项);
            if (!指代->节点 || 固定 == 采用.指代目标组.end() || 固定->实例 != *指代->节点) return 结果;
            主体 = 固定->采用时目标;
        }
        auto 投影=含义网投影旧图(单元网,{采用内容.语言节点});
        if (!投影.图 || 投影.图->定义图.节点组.size()!=1) return 结果;

        if (项->原文范围组.size() == 1 && 项->所用知识组.size() == 1) {
            const auto 范围 = 项->原文范围组.front();
            const auto 源句 = std::ranges::find_if(采用内容.句段组, [&](const auto& 句) {
                return 句.静态结构 && 句.静态结构->所用构式 == 项->所用知识组.front()
                    && 范围 == 新含义原文范围{static_cast<std::int64_t>(句.起始标量),
                        static_cast<std::int64_t>(句.结束标量 - 句.起始标量)};
            });
            if (源句 != 采用内容.句段组.end()) 转接静态成文已选名称(*源句, *投影.图);
        }

        const auto& 定义=投影.图->定义图.节点组.front();
        基础表达请求 请求{};
        请求.语言节点=采用内容.语言节点;
        请求.目的=新语言知识表达目的::陈述;
        请求.操作=定义.操作;
        请求.作用域=定义.作用域;
        请求.含义图=std::move(投影.图);
        请求.成文说话者=采用.说话者;
        请求.成文受话者=交互.受话者E;
        请求.允许根知识节点组=项->所用知识组;
        请求.局部称名组 = 成文称名;
        const auto 成文=生成基础语言表达(请求,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
        if (成文.状态==B::读取漂移 || 成文.状态==B::引用不可读) 漂移();
        if (成文.状态==B::工程未完成 || 成文.状态==B::额度不足) 资源();
        if (成文.状态==B::内部不一致) throw std::logic_error("adopted statement generation inconsistent");
        if (成文.状态!=B::已生成 || 成文.文本.empty()) {
            结果.状态=S::成文受阻;
            结果.成文状态=成文.状态;
            return 结果;
        }
        if (!全文.empty()) {
            全文+='\n';
            ++标量;
        }
        const auto 末=标量+static_cast<std::size_t>(标量数(成文.文本));
        成文范围.push_back({标量,末});
        标量=末;
        全文+=成文.文本;
        原含义.push_back(std::move(*请求.含义图));
    }
    结果.UTF8正文=std::move(全文);

    auto 句段组=投影接收篇章句界(结果.UTF8正文);
    if (原含义.size()==1 && 句段组.empty()) 句段组.push_back({{0,标量},结果.UTF8正文});
    if (句段组.size()!=成文范围.size()) {
        结果.状态=S::回读句界不一致;
        return 结果;
    }
    接收专名读取器 专名(d,交互.现实范围根,停止);
    语言基础统一输入请求 回读{采用内容.语言节点,{}, {},0,0,0,0,0,&专名,语言解释读取模式::本次正式有限集完整读取};
    回读.交互上下文=语言图交互上下文{交互.接收消息M,采用.说话者,交互.受话者E,交互.现实范围根};
    回读.读取静态角色缺口=true;
    const auto 回读称名 = 读取同源局部称名(d, 来源, 停止);
    回读.局部称名组 = 回读称名;
    bool 唯一=true;
    bool 一致=true;
    for (std::size_t i=0;i<句段组.size();++i) {
        检查认知停止(停止);
        const auto& 句=句段组[i];
        if (句.全文范围.起始标量!=成文范围[i].起始标量 || 句.全文范围.结束标量!=成文范围[i].结束标量) {
            结果.状态=S::回读句界不一致;
            return 结果;
        }
        回读.固定原文=句.句面原文;
        const auto 实际=只读解释单句基础语言(回读,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
        if (实际.状态==I::读取漂移 || 实际.状态==I::引用不可读) 漂移();
        if (实际.状态==I::工程未完成 || 实际.状态==I::额度不足) 资源();
        if (实际.状态==I::内部不一致) throw std::logic_error("adopted statement independent readback inconsistent");
        结果.独立回读句段.push_back(记录句段解释(实际,句.全文范围));
        const auto* 实际图=实际.含义 ? std::get_if<语言图含义材料>(&*实际.含义) : nullptr;
        if (实际.状态!=I::已形成值式图 || !实际图 || !实际.静态结构) 唯一=false;
        else if (!语言图结构语义一致(原含义[i],*实际图)) 一致=false;
    }
    结果.状态=!唯一 ? S::回读未唯一 : 一致 ? S::已成文且独立回读 : S::回读含义不一致;
    return 结果;
}

新语言解释资源保护结果 读资源(认知依赖& d,稳定编码 l,std::stop_token stop){
    检查认知停止(stop);
    auto r=d.语言.读取解释资源保护(l,d.词,d.存在概念);
    if(r.状态==新语言解释资源保护状态::资源失败)资源();
    if(r.状态==新语言解释资源保护状态::前提已变化)漂移();
    if(r.状态!=新语言解释资源保护状态::已读取||r.完整保护.empty())throw std::logic_error("complete language resources unavailable");
    检查认知停止(stop);
    return r;
}
新指定场景范围保护结果 读范围(认知依赖& d,稳定编码 root,std::stop_token stop){
    检查认知停止(stop);
    auto r=d.现实场景.读取指定场景范围保护(root);
    if(r.范围.状态==新场景范围成员读取状态::资源失败)资源();
    if(r.范围.状态==新场景范围成员读取状态::读取已变化)漂移();
    if(r.范围.状态!=新场景范围成员读取状态::已完成)throw std::logic_error("received cognition reality scope unavailable");
    检查认知停止(stop);
    return r;
}
const 语言图含义出现& 找认知图出现(const 语言图含义材料& 图,std::int64_t id) {
    const auto it=std::find_if(图.出现组.begin(),图.出现组.end(),[&](const auto& x){return x.出现编号==id;});
    if(it==图.出现组.end())throw std::logic_error("received meaning occurrence missing");
    return *it;
}
const auto& 找认知图定义(const 语言图含义材料& 图,const 语言图含义出现& o) {
    const auto it=std::find_if(图.定义图.节点组.begin(),图.定义图.节点组.end(),[&](const auto& x){return x.局部编号==o.定义含义编号;});
    if(it==图.定义图.节点组.end())throw std::logic_error("received meaning definition missing");
    return *it;
}
const 语言图绑定值& 找认知图参数(const 语言图含义材料& 图,const 语言图含义出现& o,新语言知识参数角色 p) {
    const auto& def=找认知图定义(图,o);
    const auto arg=std::find_if(def.参数组.begin(),def.参数组.end(),[&](const auto& x){return x.参数==p;});
    if(arg==def.参数组.end())throw std::logic_error("received meaning parameter missing");
    const auto role=std::find_if(o.角色组.begin(),o.角色组.end(),[&](const auto& x){return x.角色编号==arg->规则角色;});
    if(role==o.角色组.end())throw std::logic_error("received meaning bound role missing");
    return role->值;
}
struct 认知图角色 final {稳定编码 主体;std::optional<稳定编码> 受请求者;};
认知图角色 读取认知图角色(const 语言图含义材料& 图) {
    const auto& root=找认知图出现(图,图.根出现编号);
    const auto& def=找认知图定义(图,root);
    const 语言图含义出现* demand=&root;
    认知图角色 out;
    if(def.操作==新语言知识含义操作::行动请求) {
        if(root.有效作用域!=新语言知识作用域::请求范围)throw std::logic_error("received request scope invalid");
        const auto* e=std::get_if<语言图引用值>(&找认知图参数(图,root,新语言知识参数角色::受请求者));
        const auto* q=std::get_if<语言图子出现引用>(&找认知图参数(图,root,新语言知识参数角色::目标内容));
        if(!e||e->类别!=新语言知识角色类别::存在||!有效(e->节点)||!q)
            throw std::logic_error("received request recipient or expected demand missing");
        out.受请求者=e->节点;demand=&找认知图出现(图,q->出现编号);
    }
    const auto& d=找认知图定义(图,*demand);
    if(d.操作!=新语言知识含义操作::需求描述||demand->有效作用域!=(out.受请求者
        ? 新语言知识作用域::需求目标范围 : 新语言知识作用域::普通陈述))
        throw std::logic_error("received demand occurrence or scope invalid");
    const auto* h=std::get_if<语言图引用值>(&找认知图参数(图,*demand,新语言知识参数角色::主体));
    if(!h||h->类别!=新语言知识角色类别::存在||!有效(h->节点))
        throw std::logic_error("received demand subject not bound");
    out.主体=h->节点;return out;
}
// 由外层先裁定唯一需求含义；本reader只机械重读这些实际定位。
class 接收采用读取器 final : public 新语言采用来源读取接口 {
public:
    接收采用读取器(认知依赖& d,const 接收需求认知会话语境& context,
        const 新外设消息信息& m,const 新语言解释资源保护结果& resources,
        const 新指定场景范围保护结果& scope,const 语言基础统一输入结果& parsed,
        const std::vector<专名现场项>& names,稳定编码 speaker,std::stop_token stop,std::function<bool()> 语境仍有效)
        :d_(d),context_(context),m_(m),resources_(resources),scope_(scope),parsed_(parsed),names_(names),speaker_(speaker),stop_(stop),语境仍有效_(std::move(语境仍有效)){}
    新语言采用来源保护 读取采用来源保护(const 新接收认知来源键& k,稳定编码 scene) const override {
        新语言采用来源保护 out;
        try {
            检查认知停止(stop_);
            if(!语境仍有效_())漂移();
            const auto m=d_.外设.读取消息保护(m_.消息节点);
            if(m.状态==新外设事实状态::资源失败)资源();
            if(m.状态==新外设事实状态::未找到)漂移();
            if(m.状态!=新外设事实状态::已完成||!m.消息||m.完整保护.empty())throw std::logic_error("received message protection corrupt");
            if(!同消息(*m.消息,m_))漂移();
            if(k.接收消息节点!=m_.消息节点||k.原文起点!=0||k.原文终点!=标量数(m_.UTF8正文)||!k.根出现路径.empty())漂移();
            检查认知停止(stop_);
            const auto s=d_.语言世界.读取接收来源场景(m_.消息节点);
            if(s.状态==新来源子场景状态::资源失败)资源();
            if(s.状态==新来源子场景状态::前提已变化)漂移();
            if(s.状态==新来源子场景状态::未建立)漂移();
            if(s.状态!=新来源子场景状态::已存在||!s.信息||s.完整保护.empty())throw std::logic_error("received source scene protection corrupt");
            if(s.信息->场景节点!=scene)漂移();
            auto resources=读资源(d_,context_.语言,stop_);
            // 原owner结构/定义/候选全集不变；入向计数不是句子含义。
            if(resources.当前采用!=resources_.当前采用||!同结构闭包(resources.完整保护,resources_.完整保护))漂移();
            并保护(out.完整保护,m.完整保护);并保护(out.完整保护,s.完整保护);并保护(out.完整保护,resources.完整保护);
            for(const auto& basis:parsed_.词面查询依据组){
                检查认知停止(stop_);
                const auto r=d_.词.按规范化词面查询保护(basis.语言节点,basis.规范化合同节点,basis.规范化词面);
                if(r.状态==新词条保护状态::资源失败)资源();
                if(r.状态==新词条保护状态::前提已变化)漂移();
                if((r.状态!=新词条保护状态::已读取&&r.状态!=新词条保护状态::未找到)||r.信息!=basis.命中词条)漂移();
                并保护(out.完整保护,r.完整保护);
            }
            for(const auto& basis:parsed_.名称候选依据组){
                检查认知停止(stop_);
                const auto r=d_.名称.读取词条名称候选保护(basis.词条节点);
                if(r.状态==概念名称关系查询状态::资源失败)资源();
                if(r.状态!=概念名称关系查询状态::已找到&&r.状态!=概念名称关系查询状态::未找到)漂移();
                if(r.候选组.size()!=basis.完整候选组.size()||!std::equal(r.候选组.begin(),r.候选组.end(),basis.完整候选组.begin(),同名称))漂移();
                并保护(out.完整保护,r.完整保护);
            }
            语言场景转换内部::解释读取依据 ft读取{d_.语言,d_.词,d_.名称,d_.特征概念};
            for(const auto& basis:parsed_.特征概念依据组){
                检查认知停止(stop_);
                ft读取.读特征(basis.概念节点);
                using R=语言场景转换内部::读取状态;
                if(ft读取.失败==R::工程未完成)资源();
                if(ft读取.失败==R::读取漂移||ft读取.失败==R::引用不可读)漂移();
                if(ft读取.失败!=R::已完成)throw std::logic_error("actual feature definition collector failed");
                const auto it=std::find_if(ft读取.特征概念依据组.begin(),ft读取.特征概念依据组.end(),[&](const auto& x){return x.概念节点==basis.概念节点;});
                if(it==ft读取.特征概念依据组.end()||it->完整保护.empty()||it->根链组.empty()||basis.根链组.empty())
                    throw std::logic_error("actual feature complete chain missing");
                auto a=it->信息.定义,b=basis.信息.定义;
                a.当前引用实例数量=0;b.当前引用实例数量=0;
                if(a!=b||it->信息.类型根!=basis.信息.类型根||it->分类根节点!=basis.分类根节点
                    ||it->根链组.size()!=basis.根链组.size()
                    ||!std::equal(it->根链组.begin(),it->根链组.end(),basis.根链组.begin(),[](const auto& x,const auto& y){
                        return x.信息.定义==y.信息.定义&&x.信息.类型根==y.信息.类型根&&x.直接上位概念节点==y.直接上位概念节点;}))漂移();
                out.特征概念依据组.push_back(*it);并保护(out.完整保护,it->完整保护);
            }
            const auto scope=读范围(d_,context_.现实根,stop_);
            if(!同范围(scope.范围,scope_.范围))漂移();并保护(out.完整保护,scope.完整保护);
            const auto& 图=std::get<语言图含义材料>(*parsed_.含义);
            for(const auto& 绑定:parsed_.限定主体绑定组) {
                检查认知停止(stop_);
                const auto& 出现=找认知图出现(图,绑定.出现编号);
                const auto 角色=std::ranges::find(出现.角色组,绑定.主体角色编号,
                    [](const auto& 项){return 项.角色编号;});
                const auto* 主体=角色==出现.角色组.end() ? nullptr : std::get_if<语言图引用值>(&角色->值);
                if(!主体||主体->类别!=新语言知识角色类别::存在||主体->节点!=绑定.实际存在)
                    throw std::logic_error("qualified feature subject lost its adopted graph role");
                const auto 当前=定位语言图限定主体(绑定.原称名存在,绑定.特征类型,
                    d_.存在,d_.特征概念,true);
                using R=基础语言输入状态;
                if(当前.状态==R::资源失败)资源();
                if(当前.状态==R::内部不一致)
                    throw std::logic_error("qualified feature subject has inconsistent source");
                if(当前.状态!=R::已形成唯一含义 || 当前.适用概念!=绑定.适用存在概念
                    || 当前.实际主体!=绑定.实际存在)漂移();
                并保护(out.完整保护,当前.保护);
            }
            const auto roles=读取认知图角色(图);
            if(roles.主体!=speaker_)漂移();
            if(roles.受请求者) {
                if(roles.受请求者!=context_.已绑定自我)漂移();
                for(const auto E:{roles.主体,*roles.受请求者}) {
                    if(std::count(scope.范围.范围成员存在组.begin(),scope.范围.范围成员存在组.end(),E)!=1)漂移();
                    检查认知停止(stop_);
                    const auto identity=d_.存在.读取存在身份保护(E);
                    if(identity.状态==基础结构操作状态::资源失败)资源();
                    if(identity.前提已变化||identity.状态==基础结构操作状态::节点不存在)漂移();
                    if(identity.状态!=基础结构操作状态::已完成||!identity.信息||identity.预期节点组.empty())
                        throw std::logic_error("request interaction identity protection unavailable");
                    并保护(out.完整保护,identity.预期节点组);
                }
                out.受话者节点=roles.受请求者;
            }
            接收专名读取器 names(d_,context_.现实根,stop_);
            for(const auto& basis:names_){
                const auto r=names.读(basis.C,basis.范围);
                if(r.状态==语言图专名绑定状态::资源失败)资源();
                if(r.状态==语言图专名绑定状态::前提已变化)漂移();
                if(r.状态!=basis.状态||r.概念!=basis.概念||r.身份.has_value()!=basis.身份.has_value()
                    ||(r.身份&&(r.身份->存在节点!=basis.身份->存在节点||r.身份->专属存在概念节点!=basis.身份->专属存在概念节点)))漂移();
                并保护(out.完整保护,r.保护);
            }
            out.状态=新语言含义采用状态::已采用;out.来源=k;out.场景节点=scene;
            out.说话者节点=speaker_;out.现实范围根=context_.现实根;out.原文材料节点=m_.正文材料节点;
            out.公开宿主会话=m_.来源.公开宿主会话;out.对话框标识=m_.来源.对话框标识;
            out.上行来源序号=m_.来源.来源序号;out.固定原文=m_.UTF8正文;
            out.唯一解释=std::get<语言图含义材料>(*parsed_.含义);out.角色原文定位组=parsed_.图角色原文定位组;
            检查认知停止(stop_);
            if(!语境仍有效_())漂移();
            return out;
        } catch(const 推进中断& e){
            // 私有停止穿过原组合操作的清理边界，不伪装成来源漂移。
            if(e.状态==阶段::停止未完成)throw;
            out={};out.状态=e.状态==阶段::资源延后 ? 新语言含义采用状态::资源失败 : 新语言含义采用状态::前提已变化;return out;
        }
        catch(const std::bad_alloc&){return {新语言含义采用状态::资源失败};}
        catch(const std::length_error&){return {新语言含义采用状态::资源失败};}
    }
private:
    认知依赖& d_;const 接收需求认知会话语境& context_;const 新外设消息信息& m_;
    const 新语言解释资源保护结果& resources_;const 新指定场景范围保护结果& scope_;
    const 语言基础统一输入结果& parsed_;const std::vector<专名现场项>& names_;
    稳定编码 speaker_;
    std::stop_token stop_;
    std::function<bool()> 语境仍有效_;
};
std::int64_t 读I64(const std::string& s){
    std::int64_t v=0;const auto r=std::from_chars(s.data(),s.data()+s.size(),v);
    if(r.ec!=std::errc{}||r.ptr!=s.data()+s.size())throw 推进中断{阶段::D映射拒绝,"语言整数超出正式I64目标范围"};
    return v;
}
新部分服务要求目标 映射目标(认知依赖& d,const 新语言需求目标投影& p,std::stop_token stop){
    检查认知停止(stop);
    const auto r=d.特征概念.读取特征概念定义保护(p.目标特征类型);
    if(r.前提已变化)漂移();if(r.状态==基础结构操作状态::资源失败)资源();
    if(r.状态!=基础结构操作状态::已完成||!r.信息)throw std::logic_error("adopted target FT unreadable");
    const auto& def=r.信息->定义;
    if (r.信息->实例所属特征类型 != p.目标特征类型)
        throw 推进中断{阶段::D映射受阻,"目标不是正式实例特征类型"};
    新部分服务要求目标 out;
    out.目标存在节点 = p.目标存在;
    out.目标特征类型概念节点 = p.目标特征类型;
    if (!p.方向 && !p.准确整数 && !p.区间下界 && !p.区间上界) return out;
    if (def.材料类型 != 特征概念材料物理类型::I64标量 || (!p.方向 && def.单位 != p.单位))
        throw 推进中断{阶段::D映射受阻,"目标FT类型或单位暂不能精确映射"};
    if(p.方向){out.目标形式=新需求目标形式::变化方向;out.目标变化方向=*p.方向==新语言知识特征方向::增加 ? 新需求变化方向::增加 : 新需求变化方向::减少;return out;}
    const auto lo=读I64(p.准确整数 ? *p.准确整数 : *p.区间下界),hi=p.准确整数 ? lo : 读I64(*p.区间上界);
    const auto* ranges=std::get_if<std::vector<特征概念I64闭区间>>(&def.值域);
    if(!ranges)throw 推进中断{阶段::D映射受阻,"目标FT不是已支持的I64区间值域"};
    if(lo>hi||std::none_of(ranges->begin(),ranges->end(),[&](const auto& x){return x.下界<=lo&&x.上界>=hi;}))
        throw 推进中断{阶段::D映射拒绝,"语言目标不包含于目标FT准入值域"};
    特征概念定义 target{def.材料类型,std::vector<特征概念I64闭区间>{{lo,hi}},def.单位,def.比较规则,def.聚合规则,{}};
    检查认知停止(stop);
    const auto tc=d.特征概念.建立或取得特征概念强类型(target,d.特征值,p.目标特征类型);
    if(tc.状态==特征概念规则状态::资源失败)资源();if(tc.状态==特征概念规则状态::前提已变化)漂移();
    if(tc.状态!=特征概念规则状态::已完成&&tc.状态!=特征概念规则状态::无变化)
        throw 推进中断{阶段::D映射受阻,"目标域概念尚未建立"};
    if(!tc.概念节点)throw std::logic_error("target domain creation returned no node");
    检查认知停止(stop);
    const auto cmp=d.二次概念.建立或取得默认二次特征概念(二次特征算法::有向特征差值,
        {{0,二次特征输入角色::左输入,二次特征输入来源种类::特征准确值,p.目标特征类型},
         {1,二次特征输入角色::右输入,二次特征输入来源种类::特征准确值,p.目标特征类型}},*tc.概念节点);
    if(cmp.状态==二次特征概念操作状态::资源失败)资源();
    if(!cmp.概念节点)throw 推进中断{阶段::D映射受阻,"目标位置比较根暂不支持"};
    检查认知停止(stop);
    const auto profile=d.二次概念.建立或取得参照结果域概念(*cmp.概念节点,二次特征结果域种类::包含于参照值域);
    if(profile.状态==二次特征概念操作状态::资源失败)资源();
    if(!profile.概念节点)throw 推进中断{阶段::D映射受阻,"目标位置比较关系暂不支持"};
    out.目标形式=新需求目标形式::值;out.目标值域概念节点=tc.概念节点;out.目标比较关系概念节点=profile.概念节点;
    检查认知停止(stop);
    const auto verified=核验需求目标定义保护({p.目标特征类型,新需求目标形式::值,tc.概念节点,profile.概念节点,{},{}},d.特征概念,d.二次概念);
    if(verified.状态==新需求目标定义核验状态::资源失败)资源();if(verified.状态==新需求目标定义核验状态::前提已变化)漂移();
    if(verified.状态!=新需求目标定义核验状态::已完成)throw 推进中断{阶段::D映射受阻,"正式目标定义尚未通过位置比较合同"};
    检查认知停止(stop);
    return out;
}
// 来源、场景、说话者和时刻由调用方填写。不是值类构式或招呼时返回空，由原路径处理。
// R121（7240 §5.4“需求信息节点”）：需求陈述/请求达成在语言转换阶段即从唯一图含义取出需求信息——需求主体、
// 目标存在、目标特征类型，话中带出的值或方向；能确定的记、不能确定的记未知。不依赖被认知需求能否建立、
// 也不依赖需求主体是否为本次说话者。不是需求陈述或行动请求、或图含义不唯一时返回空。
// 最小需求句式“Y要X”（需求描述直接带目标存在）：X为专名时目标存在为已绑定存在；X为普通名词时返回其概念，
// 由调用方在接收来源语言场景中按该概念实例化一个存在作为目标存在（7240 §4.3，用户裁决2026-11-12）。
struct 提取的需求信息 final {
    新接收场景信息 信息;
    bool 目标存在需求=false;
    std::optional<稳定编码> 待实例化目标概念;
};
std::optional<提取的需求信息> 提取需求信息(const 语言基础统一输入结果& parsed,std::uint64_t 全文终点){
    using U=语言基础统一构式;using P=新语言知识参数角色;using K=新值陈述值种类;using M=新语言知识含义操作;
    if(!parsed.构式||(*parsed.构式!=U::需求陈述&&*parsed.构式!=U::行动请求))return std::nullopt;
    const 语言图含义材料* g=nullptr;
    if(parsed.状态==语言场景基础输入状态::已形成值式图&&parsed.含义&&std::holds_alternative<语言图含义材料>(*parsed.含义))
        g=&std::get<语言图含义材料>(*parsed.含义);
    else if(parsed.状态==语言场景基础输入状态::对象待绑定&&parsed.完整候选组.size()==1
        &&std::holds_alternative<语言图含义材料>(parsed.完整候选组.front()))
        g=&std::get<语言图含义材料>(parsed.完整候选组.front());
    if(!g)return std::nullopt;
    新接收场景信息 out;out.种类=新接收场景信息种类::需求信息;
    out.原文起点=0;out.原文终点=static_cast<std::int64_t>(全文终点);
    const auto 已绑定存在=[](const 语言图绑定值& v)->std::optional<稳定编码> {
        const auto* r=std::get_if<语言图引用值>(&v);
        return r&&r->类别==新语言知识角色类别::存在&&有效(r->节点) ? std::optional{r->节点} : std::nullopt;
    };
    const auto* demand=&找认知图出现(*g,g->根出现编号);
    if(找认知图定义(*g,*demand).操作==M::行动请求) {
        const auto* q=std::get_if<语言图子出现引用>(&找认知图参数(*g,*demand,P::目标内容));
        if(!q)throw std::logic_error("received request expected demand is not a child occurrence");
        demand=&找认知图出现(*g,q->出现编号);
    }
    const auto& demandDef=找认知图定义(*g,*demand);
    if(demandDef.操作!=M::需求描述)throw std::logic_error("received demand occurrence operation invalid");
    out.主体存在=已绑定存在(找认知图参数(*g,*demand,P::主体));
    const auto& root=找认知图出现(*g,g->根出现编号).原文范围;
    if(root&&root->first<root->second&&root->second<=全文终点){out.原文起点=static_cast<std::int64_t>(root->first);out.原文终点=static_cast<std::int64_t>(root->second);}
    if(std::any_of(demandDef.参数组.begin(),demandDef.参数组.end(),[](auto p){return p.参数==P::目标存在;})) {
        提取的需求信息 r{out,true,std::nullopt};
        const auto& x=找认知图参数(*g,*demand,P::目标存在);
        r.信息.目标存在=已绑定存在(x);
        if(const auto* c=std::get_if<语言图引用值>(&x);c&&c->类别==新语言知识角色类别::存在概念&&有效(c->节点))r.待实例化目标概念=c->节点;
        return r;
    }
    const auto* h=std::get_if<语言图子出现引用>(&找认知图参数(*g,*demand,P::目标内容));
    if(!h)throw std::logic_error("received demand target content is not a child occurrence");
    const auto& o=找认知图出现(*g,h->出现编号);const auto& def=找认知图定义(*g,o);
    out.目标存在=已绑定存在(找认知图参数(*g,o,P::主体));
    if(const auto* ft=std::get_if<语言图引用值>(&找认知图参数(*g,o,P::特征类型));ft&&有效(ft->节点))out.特征类型=ft->节点;
    switch(def.操作) {
    case M::特征描述:
        if (!def.特征值分支) break;
        if(const auto* n=std::get_if<语言图整数值>(&找认知图参数(*g,o,P::特征值))) {
            out.值种类=K::准确整数;out.准确整数=n->规范十进制;out.单位=n->单位;
        }
        break;
    case M::特征区间描述: {
        const auto* lo=std::get_if<语言图整数值>(&找认知图参数(*g,o,P::下界));
        const auto* hi=std::get_if<语言图整数值>(&找认知图参数(*g,o,P::上界));
        if(lo&&hi&&lo->单位==hi->单位){out.值种类=K::整数闭区间;out.区间下界=lo->规范十进制;out.区间上界=hi->规范十进制;out.单位=lo->单位;}
        break;
    }
    case M::特征方向描述:
        if(def.方向){out.值种类=K::变化方向;out.方向=def.方向;}
        break;
    default:throw std::logic_error("received demand target operation unsupported");
    }
    return 提取的需求信息{out,false,std::nullopt};
}
std::optional<新接收场景信息> 提取接收场景信息(const 语言基础统一输入结果& parsed,std::uint64_t 全文终点){
    using U=语言基础统一构式;using P=新语言知识参数角色;using K=新值陈述值种类;
    if(!parsed.构式)return std::nullopt;
    const auto 值类=[](U c){return c==U::S03整数||c==U::S03特征概念条件||c==U::目标单值
        ||c==U::目标闭区间||c==U::目标方向||c==U::S10字段填项;};
    新接收场景信息 out;out.原文起点=0;out.原文终点=static_cast<std::int64_t>(全文终点);
    const auto 定范围=[&](const std::optional<std::pair<std::size_t,std::size_t>>& r){
        if(r&&r->first<r->second&&r->second<=全文终点){
            out.原文起点=static_cast<std::int64_t>(r->first);out.原文终点=static_cast<std::int64_t>(r->second);}
    };
    const 语言图含义材料* g=nullptr;
    if(parsed.状态==语言场景基础输入状态::已形成值式图&&parsed.含义&&std::holds_alternative<语言图含义材料>(*parsed.含义))
        g=&std::get<语言图含义材料>(*parsed.含义);
    else if(parsed.状态==语言场景基础输入状态::对象待绑定&&值类(*parsed.构式)&&parsed.完整候选组.size()==1
        &&std::holds_alternative<语言图含义材料>(parsed.完整候选组.front()))
        g=&std::get<语言图含义材料>(parsed.完整候选组.front());
    if(g) {
        const auto& root=找认知图出现(*g,g->根出现编号);
        if(*parsed.构式==U::招呼) {
            out.种类=新接收场景信息种类::招呼;out.招呼操作=找认知图定义(*g,root).操作;定范围(root.原文范围);return out;
        }
        if(!值类(*parsed.构式))return std::nullopt;
        const auto* o=&root;
        if(*parsed.构式==U::S10字段填项) {
            const auto* q=std::get_if<语言图子出现引用>(&找认知图参数(*g,root,P::被询内容));
            if(!q)throw std::logic_error("received fill-in question content is not a child occurrence");
            o=&找认知图出现(*g,q->出现编号);
        }
        const auto& def=找认知图定义(*g,*o);
        out.种类=新接收场景信息种类::值陈述;定范围(root.原文范围);
        const auto& subject=找认知图参数(*g,*o,P::主体);
        if(const auto* r=std::get_if<语言图引用值>(&subject)) {
            if(r->类别==新语言知识角色类别::存在)out.主体存在=r->节点;
            else if(r->类别==新语言知识角色类别::存在概念)out.主体概念=r->节点;
            else throw std::logic_error("received value statement subject category invalid");
        } else if(!std::holds_alternative<语言图待求值>(subject))
            throw std::logic_error("received value statement subject binding invalid");
        const auto* ft=std::get_if<语言图引用值>(&找认知图参数(*g,*o,P::特征类型));
        if(!ft||!有效(ft->节点))throw std::logic_error("received value statement feature type not bound");
        out.特征类型=ft->节点;
        switch(def.操作) {
        case 新语言知识含义操作::特征描述: {
            const auto& v=找认知图参数(*g,*o,P::特征值);
            if(const auto* n=std::get_if<语言图整数值>(&v)){out.值种类=K::准确整数;out.准确整数=n->规范十进制;out.单位=n->单位;}
            else if(const auto* c=std::get_if<语言图引用值>(&v)) {
                if(c->类别!=新语言知识角色类别::特征概念||!有效(c->节点))throw std::logic_error("received value statement concept condition invalid");
                out.值种类=K::特征概念条件;out.特征概念=c->节点;
            } else if(std::holds_alternative<语言图待求值>(v))out.值种类=K::待求;
            else throw std::logic_error("received value statement value binding invalid");
            break;
        }
        case 新语言知识含义操作::特征区间描述: {
            const auto* lo=std::get_if<语言图整数值>(&找认知图参数(*g,*o,P::下界));
            const auto* hi=std::get_if<语言图整数值>(&找认知图参数(*g,*o,P::上界));
            if(!lo||!hi||lo->单位!=hi->单位)throw std::logic_error("received value statement interval invalid");
            out.值种类=K::整数闭区间;out.区间下界=lo->规范十进制;out.区间上界=hi->规范十进制;out.单位=lo->单位;break;
        }
        case 新语言知识含义操作::特征方向描述:
            if(!def.方向)throw std::logic_error("received value statement direction missing");
            out.值种类=K::变化方向;out.方向=def.方向;break;
        default:throw std::logic_error("received value statement operation unsupported");
        }
        return out;
    }
    // 旧六类只读候选（S03整数、S03特征概念条件）：主体只有指称概念，记为待定。
    if(parsed.状态!=语言场景基础输入状态::已形成只读候选||parsed.完整候选组.size()!=1
        ||!std::holds_alternative<语言场景基础候选>(parsed.完整候选组.front()))return std::nullopt;
    const auto& c=std::get<语言场景基础候选>(parsed.完整候选组.front());
    if(c.构式!=语言场景基础构式::S03整数&&c.构式!=语言场景基础构式::S03特征概念条件)return std::nullopt;
    const auto 命名=[&](std::int64_t id)->const 语言场景基础命名出现& {
        for(const auto& x:c.出现组)if(const auto* n=std::get_if<语言场景基础命名出现>(&x);n&&n->局部出现编号==id)return *n;
        throw std::logic_error("received legacy value occurrence missing");
    };
    out.种类=新接收场景信息种类::值陈述;
    if(const auto* a=std::get_if<语言场景整数特征主张>(&c.主张)) {
        out.主体概念=命名(a->主体出现编号).概念节点;out.特征类型=命名(a->特征类型出现编号).概念节点;
        const auto it=std::find_if(c.出现组.begin(),c.出现组.end(),[&](const auto& x){
            const auto* n=std::get_if<语言场景基础整数出现>(&x);return n&&n->局部出现编号==a->整数出现编号;});
        if(it==c.出现组.end())throw std::logic_error("received legacy integer occurrence missing");
        out.值种类=K::准确整数;out.准确整数=std::get<语言场景基础整数出现>(*it).规范十进制;
    } else if(const auto* a=std::get_if<语言场景特征概念主张>(&c.主张)) {
        out.主体概念=命名(a->主体出现编号).概念节点;out.特征类型=命名(a->特征类型出现编号).概念节点;
        out.值种类=K::特征概念条件;out.特征概念=命名(a->特征概念出现编号).概念节点;
    } else throw std::logic_error("received legacy value claim inconsistent");
    if(!有效(*out.主体概念)||!有效(*out.特征类型)||(out.特征概念&&!有效(*out.特征概念)))
        throw std::logic_error("received legacy value concept invalid");
    return out;
}
std::optional<稳定编码> 输出接收分界询问(认知依赖& d, const 接收分界询问配置& 配置,
    const 新前置语言输入定位& p, 稳定编码 R, const 新外设消息信息& 消息,
    std::stop_token 停止, const std::function<bool()>& 会话活动, std::string& 等待原因) {
    等待原因 = "等待原文分界询问的外设窗口定位";
    检查认知停止(停止);
    if (!会话活动()) return {};
    const auto 外设 = d.外设.按场景读取交互场景(消息.场景节点);
    if (!外设 || !外设->来源.定位.输出队列特征) return {};
    const auto& 来源窗口 = 外设->来源;
    if (来源窗口.定位.窗口节点 != 消息.窗口节点
        || 来源窗口.公开宿主会话 != p.公开宿主会话
        || 来源窗口.对话框标识 != 消息.来源.对话框标识) return {};

    auto 出站 = d.语言.读取出站具体表达(R);
    if (!出站) {
        等待原因 = "等待原文分界询问的现实交互场景";
        const auto 交互场景 = 配置.世界->查找现实交互场景(消息.场景节点, 配置.外设场景标识类型);
        using C = 新场景操作状态;
        if (交互场景.状态 == C::资源失败) 资源();
        if (交互场景.状态 == C::存在不存在 || 交互场景.状态 == C::入口拒绝) return {};
        if (交互场景.状态 != C::无变化 || !交互场景.存在节点)
            throw std::logic_error("分界询问的现实交互窗口定位不一致");
        const auto 作用域 = d.现实场景.查询所属场景(*交互场景.存在节点);
        if (!作用域) return {};
        正式表达需求 表达;
        表达.表达对象 = 正式表达信息字段{R, 配置.有效分界类型, 配置.材料范围概念};
        表达.说话者 = {新指称类别::存在, p.自我存在节点, {}};
        表达.受话者 = {新指称类别::存在, p.来源人类节点, {}};
        表达.成文参数 = {p.语言节点, *交互场景.存在节点, *作用域,
            配置.第三方指代概念, 配置.第三方别名关系, 100};
        const auto 问句 = 配置.表达->生成候选话语(表达, *配置.取材);
        if (问句.状态 != 新字段询问文字状态::已生成且唯一回读) {
            等待原因 = "原文分界询问尚未生成，语言状态=" + std::to_string(static_cast<int>(问句.状态));
            return {};
        }
        等待原因 = "等待原文分界询问的正式出站表达";
        检查认知停止(停止);
        if (!会话活动()) return {};
        const auto 场景 = d.语言世界.建立或取得出站来源场景(R);
        using L = 新来源子场景状态;
        if (场景.状态 == L::资源失败) 资源();
        if (场景.状态 == L::前提已变化 || 场景.状态 == L::未建立) return {};
        if ((场景.状态 != L::已建立 && 场景.状态 != L::已存在) || !场景.信息)
            throw std::logic_error("分界询问的出站来源场景不一致");
        出站 = d.语言.建立或取得出站具体表达(场景.信息->场景节点, 问句.含义网,
            问句.所用组织知识节点, 问句.UTF8正文, R, 新出站正式被询定位{R, 配置.有效分界类型});
        if (!出站) return {};
    }

    等待原因 = "等待原文分界询问的正式问题";
    const 新语言原文分界被询目标 目标{R, 配置.有效分界类型, 消息.场景节点, 消息.窗口节点,
        来源窗口.公开宿主会话, 来源窗口.对话框标识};
    稳定编码 Q;
    if (const auto 已有 = d.语言.按出站来源读取问题(R)) {
        if (已有->定义.被问种类 != 新问题被问种类::原文分界
            || 已有->定义.分界目标 != 目标 || 已有->定义.交互者 != p.来源人类节点)
            throw std::logic_error("同源分界问题定位冲突");
        Q = 已有->问题节点;
    } else {
        检查认知停止(停止);
        if (!会话活动()) return {};
        const auto 时间 = UTC时钟适配器{}.读取当前UTC时间证据_v1({});
        if (!时间.成功()) throw std::logic_error("无法取得正式分界提问的UTC时刻");
        新语言问题定义 定义;
        定义.出站来源场景 = 出站->场景;
        定义.出站来源 = R;
        定义.交互场景 = 消息.场景节点;
        定义.交互者 = p.来源人类节点;
        定义.被问种类 = 新问题被问种类::原文分界;
        定义.提问时刻 = 时间.证据->UTC纳秒;
        定义.分界目标 = 目标;
        const auto 建立 = d.语言.建立或取得出站问题(定义);
        using S = 新语言问题建立状态;
        if (建立.状态 == S::前提已变化) return {};
        if ((建立.状态 != S::已建立 && 建立.状态 != S::已存在) || !建立.问题节点)
            throw std::logic_error("正式分界问题建立失败");
        Q = *建立.问题节点;
    }
    检查认知停止(停止);
    if (!会话活动()) return {};
    const auto 入列 = d.外设.提交输出({来源窗口.定位, 出站->候选话语原文, {}, {}, Q});
    using S = 新外设输出队列状态;
    if (入列.状态 == S::资源失败) 资源();
    if (入列.状态 == S::目标不符) throw std::logic_error("同问题的外设输出来源冲突");
    等待原因 = "等待原文分界询问的外设入列，状态=" + std::to_string(static_cast<int>(入列.状态));
    if (入列.状态 != S::已入列 && 入列.状态 != S::已复用) return {};
    if (!入列.消息 || 入列.消息->关联问题 != Q) throw std::logic_error("正式分界询问出站回执不一致");
    try {
        std::wostringstream 记录;
        记录 << L"M=" << 消息.消息节点.值 << L" R=" << R.值 << L" Q=" << Q.值 << L" O=" << 入列.消息->消息节点.值;
        (void)记录运行日志(L"接收需求认知", L"分界问题出站", 记录.str());
    } catch (...) {}
    return 入列.消息->消息节点;
}

class 分界回答取材 final : public 新语言正式分界取材接口 {
public:
    explicit 分界回答取材(const 新语言分界回答信息& 回答) : 回答_(回答) {}
    新语言分界调用结果<新语言正式分界取材> 读取已确认分界(
        稳定编码 范围,稳定编码) const override {
        using S=新语言分界工程状态;
        if (范围!=回答_.范围) return {S::输入拒绝,{},{}};
        return {S::已完成,新语言正式分界取材{回答_.词段组,
            新语言回答分界来源{回答_.问题,回答_.节点,回答_.对齐}},{}};
    }
private:
    const 新语言分界回答信息& 回答_;
};

void 核对分界答复状态(新语言分界工程状态 状态) {
    using S=新语言分界工程状态;
    switch (状态) {
    case S::已完成: return;
    case S::资源失败: 资源();
    case S::输入拒绝:
        throw 推进中断{阶段::分界答复不适用,"答复与正式问题、原文分段或当前依据不相符"};
    case S::未找到:
    case S::材料未就绪:
    case S::前提已变化: 漂移();
    default: throw std::logic_error("原文分界答复所需服务无法读取");
    }
}

std::optional<新语言明确回复事实> 读取明确回复事实(认知依赖& d,
    const 新外设消息信息& 回复,稳定编码 说话者) {
    if(!回复.回应消息节点) return {};
    using M=新外设事实状态;
    const auto 取得=[&](稳定编码 节点) {
        auto r=d.外设.读取消息保护(节点);
        if (r.状态==M::资源失败) 资源();
        if (r.状态==M::未找到 || r.状态==M::未就绪) 漂移();
        if (r.状态!=M::已完成 || !r.消息)
            throw std::logic_error("明确回复消息读取失败");
        return r;
    };
    auto O=取得(*回复.回应消息节点);
    if(!O.消息->关联问题) return {};
    auto 发送键=O.消息->来源;
    发送键.方向=新外设消息方向::发送;
    const auto 已发送=d.外设.查找消息(发送键);
    if(!已发送) 漂移();
    auto Send=取得(已发送->消息节点);
    auto M保护=取得(回复.消息节点);
    新语言明确回复事实 事实{std::move(*M保护.消息),std::move(*O.消息),std::move(*Send.消息),说话者,{}};
    for(auto* 来源 : {&M保护,&O,&Send})
        for(auto& 项 : 来源->完整保护) 事实.正式消息保护.push_back(std::move(项));
    return 事实;
}

bool 处理明确整数短答(认知依赖& d,const 新语言明确回复事实& 事实,稳定编码 语言节点,
    接收需求认知观察& 观察,std::stop_token 停止,const std::function<bool()>& 会话活动) {
    using I=基础语言输入状态;
    const auto& M=事实.回复;
    const auto 解析=解释明确整数短答(M.UTF8正文,语言节点,d.语言,d.词);
    if(解析.状态==I::资源失败) 资源();
    if(解析.状态==I::引用不可读 || 解析.状态==I::读取漂移) 漂移();
    if(解析.状态==I::内部不一致) throw std::logic_error("明确整数短答单位读取不一致");
    if(!解析.值) return false;
    检查认知停止(停止);
    if(!会话活动()) throw 推进中断{阶段::停止未完成,"当前会话已封存"};
    const auto 场景=d.语言世界.建立或取得接收来源场景(M.消息节点);
    if(场景.状态==新来源子场景状态::资源失败) 资源();
    if(场景.状态==新来源子场景状态::前提已变化) 漂移();
    if(!场景.信息) throw std::logic_error("明确整数短答缺少来源场景");
    const auto 源场景=场景.信息->场景节点;
    检查认知停止(停止);
    if(!会话活动()) throw 推进中断{阶段::停止未完成,"当前会话已封存"};
    const auto 保存=d.语言.建立或取得明确整数短答(源场景,
        static_cast<std::int64_t>(标量数(M.UTF8正文)),*解析.值,事实);
    using S=新接收场景信息保存状态;
    if(保存.状态==S::资源失败) 资源();
    if(保存.状态==S::前提已变化) 漂移();
    if(保存.状态==S::输入拒绝 || 保存.状态==S::来源冲突) return false;
    if((保存.状态!=S::已保存 && 保存.状态!=S::精确重复) || !保存.陈述节点)
        throw std::logic_error("明确整数短答没有正式保存回执");
    观察.S=源场景;
    return true;
}

bool 处理接收分界答复(认知依赖& d,const 接收分界询问配置& 配置,
    const 新语言明确回复事实* 事实,接收需求认知观察& 观察,std::stop_token 停止,
    const std::function<bool()>& 会话活动) {
    const auto 确认会话可推进=[&]{
        检查认知停止(停止);
        if (!会话活动()) throw 推进中断{阶段::停止未完成,"当前会话已封存"};
    };

    if (!观察.分界答复) {
        if(!事实) return false;
        const auto Q=d.语言.读取问题(*事实->输出.关联问题);
        if (!Q) 漂移();
        if (Q->定义.被问种类!=新问题被问种类::原文分界) return false;
        确认会话可推进();
        const auto 回答=d.语言.接纳原文分界回答(*事实,d.特征值);
        核对分界答复状态(回答.状态);
        if (!回答.信息) throw std::logic_error("分界答复成功但没有正式回答");
        const auto R=回答.信息->范围;

        const auto 现况=配置.取材->读取正式信息参数保护(R,配置.材料范围概念);
        using P=正式信息参数读取状态;
        if (现况.状态==P::资源失败) 资源();
        if (现况.状态==P::信息不存在 || 现况.状态==P::依赖未就绪 || 现况.状态==P::前提已变化) 漂移();
        if (现况.状态!=P::已读取 || !现况.信息)
            throw std::logic_error("正式分界参数范围未读取");
        const auto& 内容=现况.信息->直接内容组;
        const auto 候选=std::find_if(内容.begin(),内容.end(),
            [&](const auto& 项){return 项.特征类型==配置.候选类型;});
        if (候选==内容.end()) throw std::logic_error("正式分界参数缺少候选字段");
        if (候选->内容.状态==正式内容值状态::当前未知) 漂移();
        const auto* C=候选->内容.值 ? std::get_if<稳定编码>(&*候选->内容.值) : nullptr;
        if (!C || !有效(*C)) throw std::logic_error("正式分界候选值缺少材料引用");

        const 分界回答取材 取材(*回答.信息);
        确认会话可推进();
        const auto 依据=d.语言.形成分界依据(R,*C,取材);
        核对分界答复状态(依据.状态);
        if (!依据.信息) throw std::logic_error("分界依据成功但没有正式材料");
        确认会话可推进();
        const auto 采用=d.语言世界.采用已形成分界依据(d.语言,{R,依据.信息->节点,{}},停止);
        核对分界答复状态(采用.状态);
        if (!采用.信息) throw std::logic_error("分界依据采用成功但没有正式回执");
        观察.分界答复=接收分界答复证据{Q->问题节点,R,采用.信息->实际依据};
    }

    const auto& 证据=*观察.分界答复;
    const auto 回执=d.语言.读取分界依据采用(证据.范围,证据.依据);
    核对分界答复状态(回执.状态);
    if (!回执.信息) throw std::logic_error("分界依据采用读回没有正式回执");
    if (const auto 状态=回执.信息->新状态) {
        确认会话可推进();
        if (!配置.依据状态投递.提交(*配置.依据状态投递.自我,*状态)) 资源();
    }
    return true;
}

} // namespace

struct 接收需求认知服务::实现 final {
    认知依赖 依赖;
    using 接收来源顺序 = std::tuple<std::string,std::string,std::uint64_t,稳定编码>;
    struct 会话 final {接收需求认知会话语境 语境;bool 封存=false;};
    enum class 前置阶段 {无,待接纳,待筹办,具名等待,待正式词段解释,待重入,已停用};
    struct 调度 final {
        接收需求认知观察 观察;bool 终结=false;std::chrono::steady_clock::time_point 下次{};bool 行动请求已交接=false;
        std::string 公开宿主会话;
        前置阶段 前置=前置阶段::无;
        std::optional<新前置语言输入定位> 定位;
        std::optional<新任务前置语言输入结果> 已确认结果;
        bool 分界询问已收束=false;
        std::optional<稳定编码> 分界询问输出;
        // 本消息首次确定的说话者（当前交互者）；重入沿用，不随交互者后续变化改写。
        std::optional<稳定编码> 说话者;
    };
    mutable std::mutex 锁;
    std::map<std::string,会话,std::less<>> 会话组;
    std::map<稳定编码,调度> 进度;
    std::optional<接收需求认知观察> 最近;
    std::optional<接收来源顺序> 上次;
    std::optional<稳定编码> 交互者特征类型;
    std::optional<接收分界询问配置> 分界询问配置;
    std::shared_ptr<const 离线词典数据源> 可信词头来源;
    explicit 实现(认知依赖 d):依赖(d){}
    新前置语言输入读取结果 读取前置材料内部(const 新前置语言输入定位&,bool,std::stop_token);
    std::optional<新前置有效分界目标> 读取当前分界缺口(const 新前置语言输入材料&) const;
};
接收需求认知服务::接收需求认知服务(新_外设世界树类& a,新_自然语言世界树类& b,新_语言类& c,新_词类& e,
    概念_名称关系类& n,概念_存在类& ce,概念_特征类& cf,概念_二次特征类& c2,新_存在类& ex,
    新_特征值类& v,新_特征类& f,新_场景类& s,新_需求类& d)
    :实现_(std::make_unique<实现>(认知依赖{a,b,c,e,n,ce,cf,c2,ex,v,f,s,d})){}
接收需求认知服务::~接收需求认知服务()=default;
void 接收需求认知服务::装配关系陈述(const 关系陈述语言服务& 服务) noexcept {
    实现_->依赖.关系陈述 = &服务;
}
bool 接收需求认知服务::装配被动分界询问(接收分界询问配置 配置) {
    if(!配置.世界||!配置.表达||!配置.取材||!有效(配置.外设场景标识类型)||!有效(配置.材料范围概念)
        ||!有效(配置.候选类型)||!有效(配置.依据类型)||!有效(配置.有效分界类型)
        ||!有效(配置.第三方指代概念)||!有效(配置.第三方别名关系)
        ||!配置.依据状态投递.自我||!配置.依据状态投递.提交)return false;
    std::lock_guard 锁(实现_->锁);
    if(实现_->分界询问配置)return *实现_->分界询问配置==配置;
    实现_->分界询问配置=std::move(配置);
    return true;
}
bool 接收需求认知服务::装配可信词头来源(std::shared_ptr<const 离线词典数据源> 来源) {
    if (!来源 || !来源->读取可信批次标识()) return false;
    std::lock_guard 锁(实现_->锁);
    if (实现_->可信词头来源) return 实现_->可信词头来源 == 来源;
    实现_->可信词头来源 = std::move(来源);
    return true;
}

接收认知语境接纳状态 接收需求认知服务::登记会话语境(const 接收需求认知会话语境& c){
    using S=接收认知语境接纳状态;
    if(c.公开宿主会话.empty()||!有效(c.现实根)||!有效(c.语言)||(c.已绑定人类&&!有效(*c.已绑定人类))
        ||(c.已绑定自我&&!有效(*c.已绑定自我)))return S::输入拒绝;
    try {std::lock_guard g(实现_->锁);const auto it=实现_->会话组.find(c.公开宿主会话);
        if(it!=实现_->会话组.end())return it->second.封存 ? S::停止接收 :
            (it->second.语境==c ? S::精确重复 : S::来源冲突);
        实现_->会话组.emplace(c.公开宿主会话,实现::会话{c,false});return S::已登记;
    }catch(const std::bad_alloc&){return S::资源失败;}catch(const std::length_error&){return S::资源失败;}
}
void 接收需求认知服务::封存会话(std::string_view c) noexcept {
    try {std::lock_guard g(实现_->锁);const auto it=实现_->会话组.find(c);if(it!=实现_->会话组.end())it->second.封存=true;
        for(auto& [M,项]:实现_->进度)if(项.公开宿主会话==c) {
            项.前置=实现::前置阶段::已停用;项.观察.待交接前置输入.reset();
        }}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"封存接收认知会话",L"接收认知会话封存发生工程异常。");}
}
std::optional<接收需求认知观察> 接收需求认知服务::读取最近观察() const {std::lock_guard g(实现_->锁);return 实现_->最近;}
新语言分界调用结果<现实交互指代材料> 接收需求认知服务::读取现实交互场景指代(
    const 新接收认知来源键& 来源, const 新_世界树类& 世界, 稳定编码 外设场景标识类型,
    std::optional<稳定编码> 预期现实交互场景, std::optional<稳定编码> 篇章采用) const {
    using S = 新语言分界工程状态;
    using C = 新场景操作状态;
    if (!有效(来源.接收消息节点) || !有效(外设场景标识类型)
        || 来源.原文起点 != 0 || !来源.原文终点 || !来源.根出现路径.empty()
        || (预期现实交互场景 && !有效(*预期现实交互场景)) || (篇章采用 && !有效(*篇章采用)))
        return {S::输入拒绝, {}, {}};

    const auto& d = 实现_->依赖;
    try {
        const auto 消息 = d.外设.读取分界消息正文(来源.接收消息节点);
        if (消息.状态 == S::未支持) return {S::输入拒绝, {}, {}};
        if (消息.状态 != S::已完成) return {消息.状态, {}, {}};
        if (!消息.消息) throw std::logic_error("interaction reference source missing");
        const auto 副本 = 世界.查找现实交互场景(消息.消息->场景节点, 外设场景标识类型);
        if (副本.状态 == C::资源失败) return {S::资源失败, {}, {}};
        if (副本.状态 == C::存在不存在) return {S::材料未就绪, {}, {}};
        if (副本.状态 == C::入口拒绝) return {S::未初始化, {}, {}};
        if (副本.状态 != C::无变化 || !副本.存在节点)
            throw std::logic_error("interaction reference window mapping inconsistent");
        const auto 现实场景 = d.现实场景.查询所属场景(*副本.存在节点);
        if (!现实场景) return {S::材料未就绪, {}, {}};
        if (预期现实交互场景 && 预期现实交互场景 != 现实场景)
            return {S::输入拒绝, {}, {}};

        const auto 解释 = d.语言.读取接收语言解释(来源, d.语言世界, d.外设);
        if (解释.状态 == S::未找到) return {S::材料未就绪, {}, {}};
        if (解释.状态 != S::已完成) return {解释.状态, {}, {}};
        if (!解释.信息) throw std::logic_error("interaction reference explanation missing");
        const auto 现实根 = 世界.获取世界树根场景();
        if (!现实根) return {S::未初始化, {}, {}};
        const auto 属于现实世界 = [&](const 新绑定指称& 目标) {
            return 目标.类别 == 新指称类别::存在
                && d.现实场景.存在属于场景范围(*现实根, 目标.节点);
        };

        现实交互指代材料 材料{*现实场景, 来源.接收消息节点, 解释.信息->内容.来源场景, 篇章采用, {}};
        using T = 新接收指代调用状态;
        const auto 类型化 = 篇章采用
            ? d.语言.读取指定接收指代采用(来源, *篇章采用, d.语言世界, d.外设)
            : d.语言.读取当前接收指代关系组(来源, d.语言世界, d.外设);
        if (类型化.状态 != T::未找到) {
            材料.类型化读回状态 = 类型化.状态;
            if (类型化.状态 != T::已完成) return {S::未支持, std::move(材料), {}};
            if (!类型化.信息) throw std::logic_error("typed interaction reference missing");
            for (const auto& 项 : *类型化.信息) {
                现实交互指代项 指代{项.特征, {},
                    std::pair{项.出现.范围.起始标量, 项.出现.范围.结束标量},
                    项.解释定义, 项.采用内容项, false, 项.值, 项.出现.作用域, 项.原词面};
                if (项.值) {
                    if (const auto* 直接 = std::get_if<新接收指代直接目标>(&*项.值)) {
                        if (const auto* 目标 = std::get_if<新绑定指称>(直接)) {
                            指代.目标 = *目标;
                            指代.已确认现实目标 = 属于现实世界(*目标);
                        }
                    }
                }
                材料.指代组.push_back(std::move(指代));
            }
            return {S::已完成, std::move(材料), {}};
        }
        if (篇章采用) {
            const auto& 已采用 = 解释.信息->篇章采用;
            if (!已采用 || 已采用->节点 != *篇章采用) return {S::未找到, {}, {}};
            材料.指代组.reserve(已采用->指代目标组.size());
            for (const auto& 项 : 已采用->指代目标组) {
                材料.指代组.push_back({项.实例, 项.采用时目标, {}, {}, 项.内容项,
                    属于现实世界(项.采用时目标)});
            }
        } else {
            const auto 实例组 = d.语言.读取接收指代实例组(来源, d.语言世界, d.外设);
            if (实例组.状态 != S::已完成) return {实例组.状态, {}, {}};
            if (!实例组.信息) throw std::logic_error("interaction reference instances missing");
            材料.指代组.reserve(实例组.信息->size());
            for (const auto& 项 : *实例组.信息) {
                if (!项.当前适用) continue;
                现实交互指代项 指代{项.节点, {},
                    std::pair{项.出现.起始标量, 项.出现.结束标量},
                    项.当前依据.value_or(项.解释定义)};
                if (项.当前目标) {
                    指代.目标 = 新绑定指称{新指称类别::存在, *项.当前目标};
                    指代.已确认现实目标 = 属于现实世界(*指代.目标);
                }
                材料.指代组.push_back(std::move(指代));
            }
        }
        return {S::已完成, std::move(材料), {}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, {}}; }
      catch (const std::length_error&) { return {S::资源失败, {}, {}}; }
}

void 接收需求认知服务::设置交互者特征类型(稳定编码 t) noexcept {
    if(!有效(t))工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"设置交互者特征类型",L"装配给出的交互者特征类型无效。");
    std::lock_guard g(实现_->锁);实现_->交互者特征类型=t;
}

接收行动请求交接确认状态 接收需求认知服务::确认行动请求已交接(
    const 新行动请求承接来源& source) noexcept {
    using S=接收行动请求交接确认状态;
    if(!有效(source.需求来源.键.接收消息节点))return S::输入拒绝;
    try {
        std::lock_guard g(实现_->锁);
        const auto it=实现_->进度.find(source.需求来源.键.接收消息节点);
        if(it==实现_->进度.end()||it->second.观察.阶段!=阶段::已读回行动请求)return S::输入拒绝;
        auto& work=it->second;const auto& out=work.观察;
        if(out.M!=source.需求来源.键.接收消息节点||out.U!=source.需求来源.采用含义节点
            ||out.D!=source.被认知需求节点||out.H!=source.来源人类节点
            ||out.受请求者!=source.受请求者节点||out.需求来源!=std::optional{source.需求来源.键})
            return S::来源冲突;
        if(work.行动请求已交接)return S::精确重复;
        work.行动请求已交接=true;work.观察.待交接请求.reset();
        if(实现_->最近&&实现_->最近->M==out.M)实现_->最近->待交接请求.reset();
        return S::已确认;
    }catch(const std::bad_alloc&){return S::资源失败;}
    catch(const std::length_error&){return S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"确认行动请求交接",L"认知请求交接进度不变量异常。");}
}

接收行动请求交接确认状态 接收需求认知服务::确认前置输入已接纳(const 新前置语言输入定位& p) noexcept {
    using S=接收行动请求交接确认状态;using P=实现::前置阶段;
    try {
        std::lock_guard g(实现_->锁);
        const auto i=实现_->进度.find(p.来源.接收消息节点);
        if(i==实现_->进度.end()||!i->second.定位)return S::输入拒绝;
        auto& w=i->second;
        if(w.定位!=std::optional{p})return S::来源冲突;
        const auto c=实现_->会话组.find(p.公开宿主会话);
        if(c==实现_->会话组.end()||c->second.封存||w.前置==P::已停用)return S::输入拒绝;
        // 来源人类为该消息的说话者（当前交互者），已随定位相等核对。
        if(c->second.语境.已绑定自我!=std::optional{p.自我存在节点}
            ||c->second.语境.语言!=p.语言节点||c->second.语境.现实根!=p.现实根场景)return S::来源冲突;
        if(w.前置==P::待筹办||w.前置==P::具名等待||w.前置==P::待正式词段解释||w.前置==P::待重入)return S::精确重复;
        if(w.前置!=P::待接纳||w.观察.U||w.观察.D)return S::输入拒绝;
        w.已确认结果.reset();
        w.前置=P::待筹办;w.观察.待交接前置输入.reset();
        if(实现_->最近&&实现_->最近->M==w.观察.M)实现_->最近=w.观察;
        return S::已确认;
    }catch(const std::bad_alloc&){return S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"确认前置输入接纳",L"原M输入接纳不变量异常");}
}
接收行动请求交接确认状态 接收需求认知服务::确认前置输入已筹办(const 新任务前置语言输入结果& r) noexcept {
    using S=接收行动请求交接确认状态;using P=实现::前置阶段;using R=新前置语言读取状态;
    try {
        if((r.状态==R::已读取)!=r.材料.has_value()
            ||(r.材料&&(r.材料->定位!=r.原请求.输入||(!r.材料->可重入原认知&&r.材料->缺口组.empty()))))
            throw std::logic_error("preunderstanding result payload invalid");
        std::lock_guard g(实现_->锁);const auto& p=r.原请求.输入;
        const auto i=实现_->进度.find(p.来源.接收消息节点);
        if(i==实现_->进度.end()||!i->second.定位)return S::输入拒绝;
        auto& w=i->second;if(w.定位!=std::optional{p})return S::来源冲突;
        if(w.已确认结果&&*w.已确认结果==r&&w.前置!=P::待筹办)return S::精确重复;
        if(w.已确认结果&&w.前置!=P::待筹办)return S::来源冲突;
        const auto c=实现_->会话组.find(p.公开宿主会话);
        if(c==实现_->会话组.end()||c->second.封存||w.前置==P::已停用){w.前置=P::已停用;return S::输入拒绝;}
        if(w.前置!=P::待筹办||w.观察.U||w.观察.D)return S::输入拒绝;
        w.已确认结果=r;w.观察.待交接前置输入.reset();
        switch(r.状态) {
        case R::已读取:
            w.观察.前置输入等待材料=r.材料;
            w.前置=P::具名等待;
            if(r.材料->可重入原认知)
                w.前置=r.材料->正式词条准入 ? P::待正式词段解释 : P::待重入;
            w.观察.阶段=阶段::前置输入治理等待;w.终结=false;break;
        case R::前提已变化:case R::停止取消:
            w.前置=P::待接纳;w.观察.待交接前置输入=p;w.观察.阶段=阶段::前置输入待治理;break;
        case R::来源停用:case R::输入不适用:w.前置=P::已停用;break;
        case R::资源失败:throw std::logic_error("preunderstanding resource failure requires engineering handling");
        default:throw std::logic_error("unknown preunderstanding result state");
        }
        w.下次=std::chrono::steady_clock::now();实现_->最近=w.观察;return S::已确认;
    }catch(const std::bad_alloc&){return S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"确认前置输入筹办",L"实际筹办结果或进度不变量异常");}
}
std::optional<新前置有效分界目标> 接收需求认知服务::实现::读取当前分界缺口(
    const 新前置语言输入材料& 材料) const {
    using R=新语言分界工程状态;
    using F=新特征完整读取状态;
    const auto& 定位=材料.定位;
    if(材料.可重入原认知 || !定位.正式原文范围 ||
        std::none_of(材料.缺口组.begin(),材料.缺口组.end(),[](const auto& 项){
            return 项.种类==新前置语言缺口种类::角色原文待确认;
        })) return {};
    const auto 范围=依赖.语言.读取材料范围特征(*定位.正式原文范围);
    if(范围.状态==R::资源失败)资源();
    if(范围.状态==R::前提已变化 || 范围.状态==R::材料未就绪 || 范围.状态==R::未找到)漂移();
    if(范围.状态!=R::已完成 || !范围.信息)throw std::logic_error("formal segmentation range unavailable");
    const auto& 正式=范围.信息->范围;
    if(正式.定位.前置来源节点!=定位.前置来源节点 || 正式.定位.语言节点!=定位.语言节点
        || 正式.定位.起始标量!=定位.来源.原文起点 || 正式.定位.结束标量!=定位.来源.原文终点)
        throw std::logic_error("segmentation range does not belong to the original input");
    constexpr auto 槽=static_cast<std::size_t>(新语言范围特征种类::有效分界结果);
    const auto 特征=依赖.特征.读取特征完整保护(正式.特征节点组[槽]);
    if(特征.状态==F::资源失败)资源();
    if(特征.状态==F::前提已变化 || 特征.状态==F::特征不存在)漂移();
    if(特征.状态!=F::已读取 || !特征.信息 || 特征.信息->特征类型根!=范围.信息->特征类型组[槽])
        throw std::logic_error("formal segmentation result feature unreadable");
    if(特征.信息->特征.当前值)return {};
    return 新前置有效分界目标{正式.节点,特征.信息->特征类型根,正式.特征节点组[槽]};
}

新前置语言输入读取结果 接收需求认知服务::实现::读取前置材料内部(
    const 新前置语言输入定位& p,bool 使用正式准入,std::stop_token stop) {
    using R=新前置语言读取状态;
    auto& d=依赖;检查认知停止(stop);
    const auto pre=d.语言.读取前置来源(p.来源,d.外设);
    if(pre.状态==新语言分界工程状态::资源失败)资源();
    if(pre.状态==新语言分界工程状态::前提已变化)漂移();
    if(pre.状态!=新语言分界工程状态::已完成||!pre.信息)throw std::logic_error("retained input source unreadable");
    if(pre.信息->节点!=p.前置来源节点||pre.信息->固定文本材料!=p.固定文本材料||pre.信息->来源!=p.来源)
        return {R::输入不适用,{}};
    const auto m=d.外设.读取消息保护(p.来源.接收消息节点);
    if(m.状态==新外设事实状态::资源失败)资源();
    if(m.状态!=新外设事实状态::已完成||!m.消息)漂移();
    if(m.消息->来源.公开宿主会话!=p.公开宿主会话||m.消息->正文材料节点!=p.固定文本材料
        ||m.消息->UTF8正文!=pre.信息->固定UTF8原文)return {R::输入不适用,{}};
    接收专名读取器 names(d,p.现实根场景,stop);
    语言基础统一输入请求 q{p.语言节点,pre.信息->固定UTF8原文,{},0,0,0,0,0,&names,语言解释读取模式::本次正式有限集完整读取};
    using S = 新接收指代调用状态;
    const auto 角色 = d.语言.读取正式消息角色(p.来源.接收消息节点,新外设消息方向::接收);
    if (角色.状态 == S::资源失败) 资源();
    if (角色.状态 == S::前提已变化) 漂移();
    if (角色.状态 != S::已完成 || !角色.信息) throw std::logic_error("retained formal role unavailable");
    if (角色.信息->说话者 != p.来源人类节点 || 角色.信息->受话者 != p.自我存在节点)
        return {R::输入不适用,{}};
    q.正式人称 = 语言图原消息人称{角色.信息->说话者,角色.信息->受话者};
    q.交互上下文=语言图交互上下文{p.来源.接收消息节点,p.来源人类节点,p.自我存在节点,p.现实根场景};
    q.读取静态角色缺口 = true;
    新前置语言输入材料 out{p};
    新语言分界调用结果<新语言分界材料信息> 准入读回;
    const 新语言词条准入定义* 准入定义 = nullptr;
    if (使用正式准入) {
        if (!p.正式原文范围) throw std::logic_error("admitted input missing formal range");
        准入读回 = d.语言.读取当前接收词条准入(*p.正式原文范围, p.前置来源节点);
        using A = 新语言分界工程状态;
        switch (准入读回.状态) {
        case A::已完成: break;
        case A::资源失败: 资源();
        case A::未找到:
        case A::未初始化:
        case A::材料未就绪:
        case A::前提已变化: 漂移();
        default: throw std::logic_error("admitted input read rejected");
        }
        if (准入读回.信息)
            准入定义 = std::get_if<新语言词条准入定义>(&准入读回.信息->定义);
        if (!准入定义) throw std::logic_error("admitted input missing word attachment");
        out.正式词条准入 = 准入读回.信息->节点;
    }
    bool 句界冲突 = false;
    const auto 解释当前句 = [&](const 解释原文范围* 句段 = nullptr) {
        const auto 范围 = 句段 ? *句段 : 解释原文范围{
            static_cast<std::size_t>(p.来源.原文起点), static_cast<std::size_t>(p.来源.原文终点)};
        if (准入定义) q.正式词段 = 语言图正式词段视图{准入定义, 范围};
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
        const auto 解释诊断号 = 接收解释诊断序号.fetch_add(1, std::memory_order_relaxed) + 1;
        const auto 解释诊断开始 = std::chrono::steady_clock::now();
        记录接收解释调用(解释诊断号, p.来源.接收消息节点, 解释诊断开始, nullptr, 句段);
#endif
        auto 结果 = 只读解释单句基础语言(q, d.语言, d.词, d.名称, d.存在概念, d.特征概念, d.存在);
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
        记录接收解释调用(解释诊断号, p.来源.接收消息节点, 解释诊断开始, &结果, 句段);
#else
        (void)句段;
#endif
        if (结果.状态 == 语言场景基础输入状态::读取漂移
            || ((句段 || 准入定义) && 结果.状态 == 语言场景基础输入状态::引用不可读)) 漂移();
        if (结果.状态 == 语言场景基础输入状态::工程未完成 || 结果.状态 == 语言场景基础输入状态::额度不足) 资源();
        if (结果.状态 == 语言场景基础输入状态::内部不一致) throw std::logic_error("input material parse corrupt");
        if (结果.图解释状态 == 基础语言输入状态::正式词段句界冲突) 句界冲突 = true;
        for (const auto& 缺口 : 结果.静态角色缺口组) {
            if (!缺口.正式普通名称未登记 || !缺口.词条
                || 缺口.操作 == 新语言知识含义操作::特征子类型定义) continue;
            out.普通词条名称缺口组.push_back({*缺口.词条, 缺口.知识节点, 缺口.类别,
                范围.起始标量 + 缺口.起始标量, 范围.起始标量 + 缺口.结束标量});
        }
        return 结果;
    };

    const auto 候选句段 = 投影接收篇章句界(q.固定原文);
    if (!候选句段.empty()) {
        for (const auto& 句段 : 候选句段) {
            检查认知停止(stop);
            q.固定原文 = 句段.句面原文;
            追加句段缺口(解释当前句(&句段.全文范围), 句段.全文范围, out.缺口组);
        }
        if (out.缺口组.empty()) out.缺口组.push_back({新前置语言缺口种类::非本入口构式,
            p.来源.原文起点, p.来源.原文终点, {}});
        if (!准入定义) out.有效分界缺口=读取当前分界缺口(out);
        if (准入定义 && (句界冲突
            || !核对接收篇章句间分隔(pre.信息->固定UTF8原文, 候选句段, *准入定义).empty()))
            out.普通词条名称缺口组.clear();
        检查认知停止(stop);
        return {R::已读取, std::move(out)};
    }

    const auto parsed = 解释当前句();
    if(parsed.状态==语言场景基础输入状态::已形成值式图&&parsed.含义&&std::holds_alternative<语言图含义材料>(*parsed.含义)
        &&(parsed.构式==语言基础统一构式::需求陈述||parsed.构式==语言基础统一构式::行动请求)) {
        const auto roles=读取认知图角色(std::get<语言图含义材料>(*parsed.含义));
        out.可重入原认知=roles.主体==p.来源人类节点&&(!roles.受请求者||roles.受请求者==std::optional{p.自我存在节点});
    }
    if (!out.可重入原认知 && (!parsed.静态角色缺口组.empty()
        || parsed.图解释状态 == 基础语言输入状态::多规则待解)) {
        追加句段缺口(parsed, {0, 标量数(q.固定原文)}, out.缺口组);
    } else if (!out.可重入原认知 && 准入定义) {
        追加句段缺口(parsed, {0, 标量数(q.固定原文)}, out.缺口组);
        if (out.缺口组.empty()) out.缺口组.push_back({新前置语言缺口种类::非本入口构式,
            p.来源.原文起点, p.来源.原文终点, {}});
    } else if (!out.可重入原认知) {
        const auto matched=d.语言.按已登记词条最长匹配(p.语言节点,pre.信息->固定UTF8原文,d.词);
        if(matched.状态==新语言查询状态::资源失败)资源();
        if(matched.状态!=新语言查询状态::已找到)throw std::logic_error("input lexical material unreadable");
        for(const auto& x:matched.片段组)if(!x.词条节点)
            out.缺口组.push_back({新前置语言缺口种类::词面未登记,x.原文码点起点,x.原文码点起点+x.原文码点数量,{}});
        新前置语言缺口种类 kind=新前置语言缺口种类::语言规则不足;
        switch(parsed.状态) {
        case 语言场景基础输入状态::缺名称:kind=新前置语言缺口种类::名称材料不足;break;
        case 语言场景基础输入状态::对象待绑定:kind=新前置语言缺口种类::对象未唯一绑定;break;
        case 语言场景基础输入状态::概念多义待解:case 语言场景基础输入状态::多规则待解:case 语言场景基础输入状态::构式多义待解:
            kind=新前置语言缺口种类::解释多义;break;
        case 语言场景基础输入状态::已形成值式图:kind=新前置语言缺口种类::非本入口构式;break;
        default:break;
        }
        out.缺口组.push_back({kind,p.来源.原文起点,p.来源.原文终点,parsed.受阻节点组});
    }
    if (!准入定义) out.有效分界缺口=读取当前分界缺口(out);
    检查认知停止(stop);return {R::已读取,std::move(out)};
}
新前置语言输入读取结果 接收需求认知服务::读取前置语言输入材料(const 新前置语言输入定位& p,std::stop_token stop) const {
    using R=新前置语言读取状态;using P=实现::前置阶段;
    try {
        bool 使用正式准入 = false;
        const auto 核工作=[&]() -> R {
            std::lock_guard g(实现_->锁);const auto c=实现_->会话组.find(p.公开宿主会话);
            if(c==实现_->会话组.end()||c->second.封存)return R::来源停用;
            if(c->second.语境.已绑定自我!=std::optional{p.自我存在节点}
                ||c->second.语境.语言!=p.语言节点||c->second.语境.现实根!=p.现实根场景)return R::输入不适用;
            const auto i=实现_->进度.find(p.来源.接收消息节点);
            if(i==实现_->进度.end()||i->second.定位!=std::optional{p}||i->second.观察.U||i->second.观察.D)return R::输入不适用;
            if(i->second.前置==P::已停用)return R::来源停用;
            使用正式准入 = i->second.观察.词条准入.has_value();
            return i->second.前置==P::待筹办 ? R::已读取 : R::前提已变化;
        };
        if(stop.stop_requested())return {R::停止取消,{}};
        const auto before=核工作();if(before!=R::已读取)return {before,{}};
        auto out=实现_->读取前置材料内部(p,使用正式准入,stop);
        const auto after=核工作();if(after!=R::已读取)return {after,{}};
        return out;
    }catch(const 推进中断& e){return {e.状态==阶段::停止未完成 ? R::停止取消 : e.状态==阶段::资源延后 ? R::资源失败 : R::前提已变化,{}};}
    catch(const std::bad_alloc&){return {R::资源失败,{}};}
    catch(const std::length_error&){return {R::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取前置输入材料",L"只读材料来源或内部结构不变量异常");}
}

新前置语言输入读取结果 接收需求认知服务::复读已筹办分界缺口(
    const 新前置语言输入材料& 材料,std::stop_token stop) const {
    using R=新前置语言读取状态;
    try {
        if(stop.stop_requested())return {R::停止取消,{}};
        const auto& p=材料.定位;
        const auto 读取工作状态=[&] {
            std::lock_guard 锁(实现_->锁);
            const auto c=实现_->会话组.find(p.公开宿主会话);
            if(c==实现_->会话组.end() || c->second.封存)return R::来源停用;
            const auto w=实现_->进度.find(p.来源.接收消息节点);
            if(!材料.有效分界缺口 || w==实现_->进度.end() || !w->second.已确认结果
                || !w->second.已确认结果->材料 || *w->second.已确认结果->材料!=材料)return R::输入不适用;
            if(w->second.前置==实现::前置阶段::已停用)return R::来源停用;
            if(w->second.前置!=实现::前置阶段::具名等待)return R::输入不适用;
            return R::已读取;
        };
        const auto 前=读取工作状态();
        if(前!=R::已读取)return {前,{}};
        auto 当前=实现_->读取前置材料内部(p,false,stop);
        const auto 后=读取工作状态();
        return 后==R::已读取 ? std::move(当前) : 新前置语言输入读取结果{后,{}};
    }catch(const 推进中断& e){return {e.状态==阶段::资源延后 ? R::资源失败 : R::前提已变化,{}};}
    catch(const std::bad_alloc&){return {R::资源失败,{}};}
    catch(const std::length_error&){return {R::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复读已筹办分界缺口",L"已确认的原M与分界范围无法读回");}
}

std::optional<接收需求认知观察> 接收需求认知服务::推进一次(std::stop_token stop){
    auto& impl=*实现_;auto& d=impl.依赖;
    std::optional<稳定编码> 本次消息;
    const char* 步骤="读取正式接收记录";
    const auto 保存观察=[&](const 接收需求认知观察& out){
        std::lock_guard g(impl.锁);impl.最近=out;
        try{
            std::wostringstream t;
            t<<L"M="<<out.M.值<<L"；阶段="<<static_cast<unsigned>(out.阶段)
             <<L"；U="<<(out.U?out.U->值:0)<<L"；D="<<(out.D?out.D->值:0)
             <<L"；原因字节="<<out.原因.size();
            (void)调试日志<调试切片::RECEIVED_DEMAND_SCHED>(L"接收需求认知",L"推进一次观察",t.str());
        }catch(...){}
    };
    try {
    if(stop.stop_requested()) {
        std::lock_guard g(impl.锁);
        for(auto& [M,项]:impl.进度)if(项.定位) {
            项.前置=实现::前置阶段::已停用;项.观察.待交接前置输入.reset();
        }
        return {};
    }
    const auto now=std::chrono::steady_clock::now();
    const auto 已到处理时间 = [&](const 实现::调度& 工作) {
        if (工作.前置 == 实现::前置阶段::待筹办 || 工作.前置 == 实现::前置阶段::已停用 || 工作.下次 > now)
            return false;
        if (工作.前置 == 实现::前置阶段::具名等待)
            return impl.分界询问配置 && !工作.观察.词条准入 && 工作.观察.前置输入等待材料
                && 工作.观察.前置输入等待材料->有效分界缺口;
        return !工作.终结 || (工作.观察.阶段 == 阶段::已读回行动请求 && !工作.行动请求已交接);
    };
    const auto 会话仍活跃_已加锁 = [&](std::string_view 会话) {
        const auto it=impl.会话组.find(会话);
        return it!=impl.会话组.end()&&!it->second.封存;
    };
    std::vector<std::string> 活跃会话;
    std::vector<稳定编码> 续办消息;
    std::optional<实现::接收来源顺序> 上次;
    {
        std::lock_guard 锁(impl.锁);
        for (const auto& [键, 会话] : impl.会话组)
            if (!会话.封存) 活跃会话.push_back(键);
        for (const auto& [M, 工作] : impl.进度)
            if (已到处理时间(工作) && 会话仍活跃_已加锁(工作.公开宿主会话)) 续办消息.push_back(M);
        上次=impl.上次;
    }
    if (活跃会话.empty()) return {};
    const auto records=d.外设.读取场景当前及指定接收记录(活跃会话, 续办消息);
    if(records.状态==新外设事实状态::资源失败){
        接收需求认知观察 out{{},{},{},{},{},阶段::资源延后,"接收记录完整读取资源未完成"};
        保存观察(out);return out;
    }
    if(records.状态!=新外设事实状态::已完成)throw std::logic_error("scene inbound record enumeration failed");
    if(stop.stop_requested())return {};
    const 新外设消息信息* chosen=nullptr;
    // 公平轮转，不让单条反复前提变化阻挡后续真实M。
    const auto 会话仍活跃=[&](const auto& m){
        std::lock_guard g(impl.锁);return 会话仍活跃_已加锁(m.来源.公开宿主会话);};
    const auto 来源顺序 = [](const 新外设消息信息& m) {
        return std::tie(m.来源.公开宿主会话,m.来源.对话框标识,m.来源.来源序号,m.消息节点);
    };
    std::size_t start=0;
    if (上次) {
        const auto 下一=std::upper_bound(records.消息组.begin(),records.消息组.end(),*上次,
            [&](const auto& 上次顺序,const auto& m){return 上次顺序<来源顺序(m);});
        start=static_cast<std::size_t>(下一-records.消息组.begin());
    }
    实现::调度 work;
    std::optional<接收分界询问配置> 分界配置;
    {
        std::lock_guard 锁(impl.锁);
        for (std::size_t i=0; i<records.消息组.size(); ++i) {
            const auto& m=records.消息组[(start+i)%records.消息组.size()];
            if (!会话仍活跃_已加锁(m.来源.公开宿主会话)) continue;
            auto it=impl.进度.find(m.消息节点);
            if (it!=impl.进度.end() && !已到处理时间(it->second)) continue;
            if (it==impl.进度.end()) {
                实现::调度 初始;
                初始.公开宿主会话=m.来源.公开宿主会话;
                it=impl.进度.emplace(m.消息节点,std::move(初始)).first;
            }
            chosen=&m;
            impl.上次=实现::接收来源顺序(来源顺序(m));
            work=it->second;
            分界配置=impl.分界询问配置;
            break;
        }
    }
    if(!chosen){
        try{std::wostringstream t;t<<L"接收候选条数="<<records.消息组.size()<<L"；本拍无合格M";
            (void)调试日志<调试切片::RECEIVED_DEMAND_SCHED>(L"接收需求认知",L"推进一次",t.str());}catch(...){}
        return {};}
    const auto& m=*chosen;本次消息=m.消息节点;
    const auto 原阶段=work.前置;const auto 原定位=work.定位;const auto 原交接=work.行动请求已交接;
    const auto 提交进度=[&]() {
        std::lock_guard g(impl.锁);auto& actual=impl.进度.at(m.消息节点);
        if(actual.前置!=原阶段||actual.定位!=原定位||actual.行动请求已交接!=原交接)return false;
        const auto c=impl.会话组.find(m.来源.公开宿主会话);
        if(c!=impl.会话组.end()&&c->second.封存){actual.前置=实现::前置阶段::已停用;actual.观察.待交接前置输入.reset();return false;}
        actual=work;return true;
    };
    auto out=work.观察;out.M=m.消息节点;
    if (work.前置 == 实现::前置阶段::待接纳) {
        using S = 新语言分界工程状态;
        using P = 实现::前置阶段;
        if (!work.定位) throw std::logic_error("preunderstanding handoff lacks source");
        if (stop.stop_requested() || !会话仍活跃(m)) return {};
        if (!work.定位->正式原文范围) {
            步骤 = "建立或取得接收原文范围";
            const auto& p = *work.定位;
            const auto 范围 = d.语言世界.建立或取得接收原文范围(d.语言, p.来源, p.前置来源节点, p.语言节点);
            out.阶段 = 阶段::前置输入治理等待;
            out.原因 = "等待原文范围与实际特征就绪";
            switch (范围.状态) {
            case S::已完成:
                if (!范围.信息) throw std::logic_error("received range success without information");
                work.定位->正式原文范围 = 范围.信息->节点;
                break;
            case S::未找到:
                work.前置 = P::待重入;
                out.原因 = "原文范围来源未找到，重新读取原消息";
                break;
            case S::资源失败:
                out.阶段 = 阶段::资源延后;
                out.原因 = "原文范围建立资源未完成";
                break;
            case S::未初始化:
            case S::材料未就绪:
            case S::前提已变化:
                break;
            case S::输入拒绝:
            case S::未支持:
                (void)记录逻辑错误日志(L"接收需求认知", L"建立或取得接收原文范围", L"正式前置输入的范围定位被拒绝。");
                work.前置 = P::已停用;
                work.终结 = true;
                out.阶段 = 阶段::非支持输入;
                out.原因 = "正式前置输入的范围定位被拒绝";
                break;
            }
        }
        out.待交接前置输入.reset();
        if (out.词条准入) {
            out.阶段 = 阶段::前置输入待治理;
            out.原因 = "按同源正式词段提交当前普通内容词名称缺口";
            out.待交接前置输入 = work.定位;
        } else if (work.定位->正式原文范围) {
            步骤 = "采用接收分词候选";
            const auto& p = *work.定位;
            const auto 候选 = d.语言世界.采用接收分词候选(
                d.语言, *p.正式原文范围, p.前置来源节点, d.特征值, d.词, stop);
            out.阶段 = 阶段::前置输入治理等待;
            out.原因 = "等待同源分词候选材料";
            switch (候选.状态) {
            case S::已完成:
                if (!候选.信息) throw std::logic_error("received lexical candidate adoption missing");
                out.阶段 = 阶段::前置输入待治理;
                out.原因 = "已按原文来源采用分词候选，等待前置材料交接";
                out.待交接前置输入 = work.定位;
                break;
            case S::未找到:
                work.定位->正式原文范围.reset();
                break;
            case S::资源失败:
                out.阶段 = 阶段::资源延后;
                out.原因 = "原文分词候选资源未完成";
                break;
            case S::未初始化:
            case S::材料未就绪:
            case S::前提已变化:
                break;
            case S::输入拒绝:
            case S::未支持:
                work.前置 = P::已停用;
                work.终结 = true;
                out.阶段 = 阶段::非支持输入;
                out.原因 = "同源分词候选材料无法采用";
                break;
            }
        }
        work.观察 = out;
        work.下次 = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        if (stop.stop_requested() || !提交进度()) return {};
        保存观察(out);
        return out;
    }
    if(work.前置==实现::前置阶段::具名等待 && 分界配置 && out.前置输入等待材料
        && out.前置输入等待材料->有效分界缺口) {
        const auto& 材料=*out.前置输入等待材料;
        using S=正式信息参数读取状态;
        try {
            if(stop.stop_requested()||!会话仍活跃(m))return {};
            using 准入状态 = 新语言分界工程状态;
            const auto& 定位 = 材料.定位;
            const auto 准入 = d.语言世界.采用接收词条准入(d.语言, 材料.有效分界缺口->范围,
                定位.前置来源节点, d.特征值, d.词, stop);
            if (准入.状态 == 准入状态::已完成) {
                if (!准入.信息) throw std::logic_error("source admission readback missing");
                work.分界询问已收束 = true;
                work.前置 = 实现::前置阶段::待正式词段解释;
                work.终结 = false;
                out.词条准入 = 接收词条准入证据{定位.来源, 定位.前置来源节点,
                    准入.信息->所属范围, 准入.信息->节点};
                out.原因 = "原消息已读回同范围的正式词条准入，待按正式词段解释";
            } else if (准入.状态 == 准入状态::资源失败) {
                资源();
            } else if (准入.状态 == 准入状态::输入拒绝 || 准入.状态 == 准入状态::未支持) {
                throw std::logic_error("formal reception admission source rejected");
            }
            if (!work.分界询问已收束 && 准入.状态 == 准入状态::未找到) {
                if(const auto 缺口=impl.读取当前分界缺口(材料)) {
                    const auto 取材=分界配置->取材->读取正式信息参数保护(缺口->范围,分界配置->材料范围概念);
                    if(取材.状态==S::资源失败)资源();
                    if(取材.状态==S::已读取&&取材.信息) {
                        const auto& 字段=取材.信息->直接内容组;
                        const auto 已知=[&](稳定编码 类型){
                            return std::any_of(字段.begin(),字段.end(),[&](const auto& 项){
                                return 项.特征类型==类型&&项.内容.状态==正式内容值状态::已知;
                            });
                        };
                        if(已知(分界配置->依据类型)||已知(分界配置->有效分界类型)) {
                            work.分界询问已收束=true;
                        } else if(已知(分界配置->候选类型)) {
                            work.分界询问输出=输出接收分界询问(d,*分界配置,材料.定位,缺口->范围,m,stop,
                                [&]{return 会话仍活跃(m);},out.原因);
                            if(work.分界询问输出){
                                work.分界询问已收束=true;
                                out.原因="正式分界问题已入列，等待明确关联回答";
                            }
                        }
                    }
                } else work.分界询问已收束=true;
            }
        } catch(const 推进中断& 中断) {
            if(stop.stop_requested())return {};
            out.原因=中断.原因;
        } catch(const std::bad_alloc&) {
            out.原因="正式分界询问资源延后";
        } catch(const std::length_error&) {
            out.原因="正式分界询问材料资源延后";
        }
        const bool 有新观察 = out.词条准入.has_value() || out.原因 != work.观察.原因;
        work.观察=out;
        work.下次=std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
        if(!提交进度() || !有新观察)return {};
        保存观察(out);return out;
    }
    if(work.前置==实现::前置阶段::待重入) {
        work.前置=实现::前置阶段::无;work.观察.前置输入等待材料.reset();out.前置输入等待材料.reset();
    }
    if(work.终结&&out.阶段==阶段::已读回行动请求&&!work.行动请求已交接) {
        if(!out.U||!out.D||!out.H||!out.受请求者||!out.需求来源)
            throw std::logic_error("completed request handoff source missing");
        const 新行动请求承接来源 source{{*out.需求来源,*out.U},*out.D,*out.H,*out.受请求者};
        const auto fresh=d.需求.读取行动请求承接来源保护(source);
        if(stop.stop_requested())return {};
        out.待交接请求.reset();
        switch(fresh.状态) {
        case 新行动请求来源读取状态::已读取:
            if(!fresh.信息||fresh.信息->来源!=source||fresh.完整保护.empty())
                throw std::logic_error("request handoff readback payload missing");
            out.待交接请求=source;out.原因="正式行动请求待自我邮箱接纳";break;
        case 新行动请求来源读取状态::前提已变化:
            out.原因="请求来源前提变化，保留原认知进度等待重新读取";break;
        case 新行动请求来源读取状态::依赖未就绪:
        case 新行动请求来源读取状态::资源失败:
            out.原因="请求来源读取暂未完成，保留原认知进度等待重试";break;
        default:throw std::logic_error("completed request handoff source inconsistent");
        }
        // 认知阶段13保持已发生事实；本次未取得来源时不交付待交接值。
        work.观察=out;work.下次=std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
        if(!提交进度())return {};保存观察(out);return out;
    }
    bool 本次终结=work.终结;
    const bool 正式词段重入 = work.前置 == 实现::前置阶段::待正式词段解释;
    bool 本次取得正式解释 = false;
    bool 需要普通词条筹办 = false;
    std::optional<语言图交互上下文> 篇章成文语境;
    const auto 进入步骤=[&](const char* name){
        步骤=name;
        if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止前保留已接纳消息"};
        // 只更新工程观察，不提前改变已完成阶段或预造U/D。
        if (!正式词段重入 && work.观察.阶段 != 阶段::语言解释待完善) {
            auto current=out;current.原因=std::string("正在")+name;保存观察(current);
        }
    };
    try {
        进入步骤("读取正式会话语境");
        接收需求认知会话语境 context;
        {std::lock_guard g(impl.锁);const auto it=impl.会话组.find(m.来源.公开宿主会话);
            if(it==impl.会话组.end()||it->second.封存)throw 推进中断{阶段::非本次人类需求,"消息未接入活动正式会话语境，保留原始接收事实"};context=it->second.语境;}
        const 新接收认知来源键 请求键{m.消息节点,0,标量数(m.UTF8正文),{}};
        进入步骤("建立或取得正式原文来源");
        const auto pre = 正式词段重入 ? d.语言.读取前置来源(请求键,d.外设)
            : d.语言.建立或取得前置来源({请求键},d.外设);
        const auto 来源工程错误=[&](const char* 原因) {
            throw std::logic_error(std::string(原因)+"；M="+std::to_string(m.消息节点.值)
                +"；来源状态="+std::to_string(static_cast<unsigned>(pre.状态))
                +"；本次已发表来源="+std::to_string(pre.本次已发表身份 ? pre.本次已发表身份->值 : 0));
        };
        if(pre.状态!=新语言分界工程状态::已完成&&pre.信息)
            来源工程错误("前置来源非成功携带业务信息");
        switch(pre.状态) {
        case 新语言分界工程状态::已完成:break;
        case 新语言分界工程状态::资源失败:资源();
        case 新语言分界工程状态::前提已变化:漂移();
        case 新语言分界工程状态::未找到:
            throw 推进中断{阶段::非本次人类需求, "当前消息已无正式前置来源，结束本次交接"};
        default:来源工程错误("正式保留消息的前置来源返回不符合接线前置的状态");
        }
        if(!pre.信息)来源工程错误("前置来源成功却缺少正式载荷");
        const auto& 固定来源=*pre.信息;
        if(!有效(固定来源.节点)||固定来源.来源!=请求键
            ||固定来源.固定文本材料!=m.正文材料节点||固定来源.固定UTF8原文!=m.UTF8正文)
            来源工程错误("正式前置来源与选定消息不一致");
        const auto& k=固定来源.来源;
        if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止前保留已接纳消息"};
        进入步骤("读取同源语言采用");
        auto adopted=d.语言.读取接收采用含义(k,d.特征);
        if(adopted.状态==新语言含义采用状态::资源失败)资源();if(adopted.状态==新语言含义采用状态::前提已变化)漂移();
        using 角色状态 = 新接收指代调用状态;
        const auto 角色 = d.语言.读取正式消息角色(m.消息节点,新外设消息方向::接收);
        if (角色.状态 == 角色状态::资源失败) 资源();
        if (角色.状态 == 角色状态::前提已变化) 漂移();
        if (角色.状态 != 角色状态::已完成 || !角色.信息)
            throw std::logic_error("formal receive person roles unreadable");
        const auto 本次人称 = 语言图原消息人称{角色.信息->说话者,角色.信息->受话者};
        const bool 历史采用 = adopted.状态==新语言含义采用状态::已采用 && adopted.信息;
        if (!历史采用 && !out.分界答复) {
            using I=基础语言输入状态;
            const auto 解释带单位I64=[&](const std::string& 文本)
                -> std::optional<std::pair<std::int64_t,稳定编码>> {
                const auto 材料=解释明确整数短答(文本,context.语言,d.语言,d.词);
                if(材料.状态==I::资源失败)资源();
                if(材料.状态==I::引用不可读||材料.状态==I::读取漂移)漂移();
                if(材料.状态==I::内部不一致)throw std::logic_error("正式整数答复单位读取不一致");
                if(!材料.值||!材料.值->单位)return {};
                const auto& 十进制=材料.值->规范十进制;
                std::int64_t 值=0;
                const auto 转换=std::from_chars(十进制.data(),十进制.data()+十进制.size(),值);
                if(转换.ec!=std::errc{}||转换.ptr!=十进制.data()+十进制.size())return {};
                return std::pair{值,*材料.值->单位};
            };
            constexpr std::string_view 中文引导="更正答案：", 西文引导="更正答案:", 旧新分隔="改为";
            const std::string_view 原文=m.UTF8正文;
            const auto 引导长度=原文.starts_with(中文引导) ? 中文引导.size()
                : 原文.starts_with(西文引导) ? 西文引导.size() : 0;
            std::optional<std::pair<std::int64_t,稳定编码>> 短答;
            std::optional<std::int64_t> 指明原整数;
            if(引导长度) {
                const auto 分隔=原文.find(旧新分隔,引导长度);
                if(分隔!=std::string_view::npos&&原文.find(旧新分隔,分隔+旧新分隔.size())==std::string_view::npos) {
                    const auto 旧=解释带单位I64(std::string{原文.substr(引导长度,分隔-引导长度)});
                    const auto 新=解释带单位I64(std::string{原文.substr(分隔+旧新分隔.size())});
                    if(旧&&新&&旧->second==新->second) {
                        短答=新;
                        指明原整数=旧->first;
                    }
                }
            } else 短答=解释带单位I64(m.UTF8正文);
            if(短答) {
                bool 正式分界回复=false;
                if(m.回应消息节点) {
                    const auto 运输=d.外设.读取消息保护(*m.回应消息节点);
                    if(运输.状态==新外设事实状态::资源失败)资源();
                    if(运输.消息 && 运输.消息->关联问题)
                        正式分界回复=d.语言.是登记的原文分界问题(*运输.消息->关联问题);
                }
                if(!正式分界回复) {
                    进入步骤("保存带单位的原消息整数答复");
                    if(stop.stop_requested()||!会话仍活跃(m))
                        throw 推进中断{阶段::停止未完成,"停止前保留已接纳消息"};
                    const auto 场景=d.语言世界.建立或取得接收来源场景(m.消息节点);
                    if(场景.状态==新来源子场景状态::资源失败)资源();
                    if(!场景.信息)漂移();
                    const auto 答复=d.语言.保存原消息整数答复(m.消息节点,场景.信息->场景节点,
                        短答->first,短答->second,d.语言世界,{},指明原整数);
                    using A=新原所求项答复状态;
                    if(答复.状态==A::资源失败)资源();
                    if(答复.状态==A::前提已变化||答复.状态==A::未找到)漂移();
                    if((答复.状态!=A::已关联&&答复.状态!=A::待确认)||!答复.信息)
                        throw std::logic_error("正式原消息整数答复未保存");
                    out.S=场景.信息->场景节点;
                    out.被动解释来源=k;
                    out.H=本次人称.说话者;
                    throw 推进中断{阶段::已保存场景信息,答复.状态==A::已关联
                        ? "原所求项已保存正式整数答复" : "正式整数解释已保存，原所求项待确认"};
                }
            }
        }
        if (!历史采用 && (!本次人称.说话者 || !本次人称.受话者 || 本次人称.受话者 != context.已绑定自我)) {
            进入步骤("被动保存单端或未决角色");
            const auto 场景 = d.语言世界.建立或取得接收来源场景(m.消息节点);
            if (场景.状态 == 新来源子场景状态::资源失败) 资源();
            if (!场景.信息) 漂移();
            out.S = 场景.信息->场景节点;
            out.被动解释来源 = k;
            out.H = 本次人称.说话者;
            接收专名读取器 names(d,context.现实根,stop);
            语言基础统一输入请求 q{context.语言,固定来源.固定UTF8原文,{},0,0,0,0,0,&names,
                语言解释读取模式::本次正式有限集完整读取};
            q.正式人称 = 本次人称;
            q.读取静态角色缺口 = true;
            auto 句段 = 投影接收篇章句界(q.固定原文);
            const bool 已证普通外层 = 可读取无上下文外层共指(q.固定原文);
            const bool 读取同文候选 = !句段.empty() && 已证普通外层;
            const auto 词头来源 = [&] {
                std::lock_guard 锁(impl.锁);
                return impl.可信词头来源;
            }();
            const auto 读当前句 = [&] {
                auto 图 = 只读解释单句基础语言(q,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
                using I = 语言场景基础输入状态;
                if (图.状态 == I::读取漂移 || 图.状态 == I::引用不可读) 漂移();
                if (图.状态 == I::工程未完成 || 图.状态 == I::额度不足) 资源();
                if (图.状态 == I::内部不一致) throw std::logic_error("passive receive person interpretation inconsistent");
                return 图;
            };
            const auto 可取存在归类 = [&](const 语言基础统一输入结果& 图) {
                using O = 新语言知识含义操作;
                using P = 新语言知识参数角色;
                using K = 新语言知识角色种类;
                using C = 新语言知识角色类别;
                if (!已证普通外层 || 图.图解释状态 != 基础语言输入状态::静态角色待取材
                    || !图.静态结构 || 图.静态结构->操作 != O::概念归类)
                    return false;
                const auto& 结构 = *图.静态结构;
                const auto 主体 = std::ranges::find(结构.角色组, P::主体, &接收语言已解释角色::参数);
                if (主体 == 结构.角色组.end() || 主体->值) return false;
                return std::ranges::count_if(图.静态角色缺口组, [&](const auto& 项) {
                    return 项.知识节点 == 结构.所用构式 && 项.操作 == 结构.操作
                        && 项.参数 == P::归类概念 && 项.种类 == K::概念引用 && 项.类别 == C::存在概念;
                }) == 1;
            };
            const auto 是正式局部声明 = [](const 语言基础统一输入结果& 图) {
                using O = 新语言知识含义操作;
                using P = 新语言知识参数角色;
                using C = 新语言知识角色类别;
                if (图.图解释状态 != 基础语言输入状态::静态角色待取材
                    || !图.静态结构 || 图.静态结构->操作 != O::概念归类
                    || 图.静态角色缺口组.size() != 1
                    || 图.静态角色缺口组.front().参数 != P::主体)
                    return false;
                const auto& 角色组 = 图.静态结构->角色组;
                const auto 谓项 = std::ranges::find(角色组, P::归类概念, &接收语言已解释角色::参数);
                const auto* 概念 = 谓项 != 角色组.end() && 谓项->值
                    ? std::get_if<语言图引用值>(&*谓项->值) : nullptr;
                return 概念 && 概念->类别 == C::存在概念
                    && 概念->来源概念 == 概念->节点 && 概念->选用名称关系.has_value();
            };
            接收篇章主体观察 主体观察;
            if (句段.empty()) 句段.push_back({{static_cast<std::size_t>(k.原文起点),
                static_cast<std::size_t>(k.原文终点)},q.固定原文});
            std::vector<新接收指代材料> 指代;
            std::set<稳定编码> 本文已声明;
            out.句段解释.clear();
            for (const auto& 句 : 句段) {
                检查认知停止(stop);
                q.固定原文 = 句.句面原文;
                auto 图 = 读当前句();
                if (可取存在归类(图)
                    && 取得欠缺的可信词头(d, 词头来源, q.语言节点, q.固定原文, 图, stop))
                    图 = 读当前句();
                if (读取同文候选 && 图.静态结构
                    && 图.静态结构->操作 == 新语言知识含义操作::特征有序大于) {
                    const auto 局部称名 = 读取同源局部称名(d, k, stop, 句.全文范围.起始标量);
                    if (!局部称名.empty()) {
                        q.局部称名组 = 局部称名;
                        图 = 读当前句();
                        q.局部称名组 = {};
                    }
                }
                auto& 证据 = out.句段解释.emplace_back(记录句段解释(图,句.全文范围,d.关系陈述));
                const bool 待取归类 = 可取存在归类(图);
                if (待取归类)
                    记录待完善谓项概念(d, 固定来源, 图, 证据, stop);
                if (读取同文候选) {
                    using O = 新语言知识含义操作;
                    using P = 新语言知识参数角色;
                    using C = 新语言知识角色类别;
                    bool 已取待完善归类 = false;
                    if (待取归类 && 证据.静态结构) {
                        const auto& 角色组 = 证据.静态结构->角色组;
                        const auto 分类 = std::ranges::find(角色组, P::归类概念, &接收语言已解释角色::参数);
                        const auto* 值 = 分类 != 角色组.end() && 分类->值
                            ? std::get_if<语言图引用值>(&*分类->值) : nullptr;
                        已取待完善归类 = 值 && 值->类别 == C::存在概念 && !值->选用名称关系;
                    }
                    const bool 局部声明 = 是正式局部声明(图) || 已取待完善归类;
                    const bool 取同文指代 = !本文已声明.empty() && 证据.静态结构
                        && 证据.静态结构->操作 == O::特征描述;
                    if (局部声明 || 取同文指代)
                        主体观察.记录(图, q.语言节点, d.名称, d.语言世界, 固定来源, 证据);
                    else
                        主体观察.记录未绑定候选(图, q.语言节点, d.名称, 证据);

                    const auto& 关系 = 证据.主体关系;
                    if (关系 && 关系->已确定 && 关系->来源 && 关系->来源->已绑定存在) {
                        const auto E = *关系->来源->已绑定存在;
                        if (局部声明 && !关系->指代概念) 本文已声明.insert(E);
                        if (取同文指代 && 关系->指代概念 && 本文已声明.contains(E)) {
                            指代.push_back({{k, {关系->起始标量, 关系->结束标量}, {}},
                                {证据.起始标量, 证据.结束标量}, "主体", {}, 新接收指代关系值{
                                    新接收指代直接目标{新绑定指称{新指称类别::存在, E}}}});
                        }
                    }
                }
                auto 项 = 投影原消息人称(图,k,句.全文范围);
                指代.insert(指代.end(),std::make_move_iterator(项.begin()),std::make_move_iterator(项.end()));
            }
            const auto 原指代 = d.语言.读取当前接收指代关系组(k,d.语言世界,d.外设);
            if (原指代.状态 == 角色状态::资源失败) 资源();
            if (原指代.状态 == 角色状态::前提已变化) 漂移();
            if (原指代.状态 != 角色状态::已完成 && 原指代.状态 != 角色状态::未找到)
                throw std::logic_error("passive receive existing references unreadable");
            bool 已有明确非人称 = false;
            if (原指代.信息) for (const auto& 既有 : *原指代.信息) {
                if (既有.原词面 == "我" || 既有.原词面 == "你" || 既有.原词面 == "我们") continue;
                const auto 本次 = std::ranges::find(指代, 既有.出现, &新接收指代材料::出现);
                if (本次 != 指代.end()) {
                    if (!既有.值 || 既有.值 == 本次->值) continue;
                    指代.erase(本次);
                }
                已有明确非人称 |= 既有.值.has_value();
                if (std::ranges::none_of(out.句段解释,[&](const auto& 句) {
                    return 句.起始标量 == 既有.句段范围.起始标量 && 句.结束标量 == 既有.句段范围.结束标量;
                })) continue;
                指代.push_back({既有.出现,既有.句段范围,既有.角色,{},既有.值});
            }
            if (已有明确非人称)
                for (auto& 证据 : out.句段解释) 证据.主体关系.reset();
            if (!已有明确非人称)
                关联同文待绑定任务(out.句段解释,固定来源.固定UTF8原文);
            步骤 = "发表被动解释及指代";
            auto 保存 = d.语言.保存接收语言解释及指代关系(k,{*out.S,固定来源.节点,context.语言,
                k.原文起点,k.原文终点,out.句段解释},指代,本次人称.说话者.value_or(稳定编码{}),d.语言世界,d.外设);
            if (保存.状态 == 角色状态::资源失败) 资源();
            if (保存.状态 == 角色状态::前提已变化 || 保存.状态 == 角色状态::未找到) 漂移();
            if (保存.状态 != 角色状态::已完成 || !保存.信息)
                throw std::logic_error("passive receive person interpretation unpublished");
            if (读取同文候选) {
                步骤 = "检查被动解释的场景比较方向";
                const auto 方向 = d.语言世界.采用接收语言场景方向(k, *保存.信息, d.语言, d.外设);
                using A = 基础节点组追加状态;
                if (方向 == A::资源失败) 资源();
                if (方向 == A::预期不符 || 方向 == A::节点不存在 || 方向 == A::引用不存在) 漂移();
                if (方向 != A::已完成)
                    throw std::logic_error("received comparison scene direction unpublished");
            }
            const auto& 正式句段 = 保存.信息->内容.句段组;
            using O = 新语言知识含义操作;
            const bool 外层单句部分语义 = 已证普通外层 && 正式句段.size() == 1 && 正式句段.front().静态结构
                && (正式句段.front().静态结构->操作 == O::信息请求
                    || 正式句段.front().静态结构->操作 == O::近期完成上升描述
                    || 正式句段.front().静态结构->操作 == O::需求描述);
            const bool 单句特征新增 = 正式句段.size() == 1 && 正式句段.front().静态结构
                && 正式句段.front().静态结构->操作 == 新语言知识含义操作::特征新增请求;
            const bool 有同文任务 = 正式句段.size()==2 && 正式句段.back().主体关系
                && 正式句段.back().静态结构 && 正式句段.back().静态结构->操作==O::任务当前等待描述;
            if (读取同文候选 || 外层单句部分语义 || 单句特征新增 || 有同文任务)
                out.部分成文 = 从接收部分含义成文并独立回读(d, *保存.信息, k, std::nullopt, stop, 步骤);
            throw 推进中断{阶段::语言解释待完善,"已保存同源被动解释及确定的一端，角色不足时保留原消息"};
        }
        if (!context.已绑定自我)
            throw 推进中断{阶段::语言解释待完善,"已有采用保留，当前缺少正式接收自我"};
        work.说话者 = 历史采用 ? std::optional{adopted.信息->说话者节点} : 本次人称.说话者;
        const auto speaker=*work.说话者;
        out.H=speaker;
        if (本次人称.说话者 == speaker && 本次人称.受话者 == context.已绑定自我)
            篇章成文语境=语言图交互上下文{m.消息节点,speaker,*本次人称.受话者,context.现实根};
        if (out.阶段==阶段::已保存场景信息 && out.篇章成文
            && out.篇章成文->状态==接收篇章成文状态::待成文)
            throw 推进中断{阶段::已保存场景信息,"固定篇章已采用，继续反向成文"};
        if(m.回应消息节点) {
            进入步骤("读取正式回复传输");
            const auto 事实=out.分界答复 ? std::nullopt : 读取明确回复事实(d,m,speaker);
            const auto 活动=[&]{return 会话仍活跃(m);};
            if(分界配置 && (事实 || out.分界答复)) {
                进入步骤("接收正式分界答复");
                if(处理接收分界答复(d,*分界配置,事实 ? &*事实 : nullptr,out,stop,活动))
                    throw 推进中断{阶段::已采用分界答复,"已读回同源分界依据，并按实际状态交接自我"};
            }
            if(事实) {
                进入步骤("保存正式明确整数短答");
                if(处理明确整数短答(d,*事实,context.语言,out,stop,活动))
                    throw 推进中断{阶段::已保存场景信息,"已保存明确回复的整数与正式单位"};
            }
        }
        if (!out.U) {
            新前置语言输入定位 定位{k, 固定来源.节点, 固定来源.固定文本材料, context.语言, speaker,
                *context.已绑定自我, context.现实根, context.公开宿主会话};
            if (work.定位 && work.定位->正式原文范围) {
                定位.正式原文范围 = work.定位->正式原文范围;
                if (定位 != *work.定位) {
                    work.前置 = 实现::前置阶段::已停用;
                    out.待交接前置输入.reset();
                    throw 推进中断{阶段::非本次人类需求, "当前会话与原前置交接定位不一致，结束本次交接"};
                }
            }
            work.定位 = std::move(定位);
        }
        if(adopted.状态==新语言含义采用状态::已采用){
            if(!adopted.信息)throw std::logic_error("adopted meaning source returned no payload");
            out.U=adopted.信息->节点;out.S=adopted.信息->场景节点;
            const auto roles=读取认知图角色(adopted.信息->含义);
            if(roles.主体!=speaker)throw std::logic_error("saved demand speaker conflicts with source context");
            新语言需求目标投影 projected;
            if(roles.受请求者) {
                if(!context.已绑定自我)throw 推进中断{阶段::对象未决,"本次会话缺少正式受请求者"};
                进入步骤("读取同源行动请求");
                const auto request=d.语言.读取采用行动请求保护(*out.U,*context.已绑定自我,d.特征,d.存在);
                if(request.状态==新语言含义采用状态::资源失败)资源();
                if(request.状态==新语言含义采用状态::前提已变化)漂移();
                if(request.状态!=新语言含义采用状态::已采用||!request.信息)
                    throw std::logic_error("saved request projection conflicts with adopted graph");
                if(request.信息->请求来源!=k||request.信息->说话者H!=speaker
                    ||request.信息->受请求者E!=*context.已绑定自我)漂移();
                projected=request.信息->期望需求;
                out.受请求者=request.信息->受请求者E;
                out.需求来源=request.信息->需求来源;
            } else {
                进入步骤("读取采用需求目标");
                const auto value=d.语言.读取采用需求目标(*out.U,d.特征);
                if(value.状态==新语言含义采用状态::资源失败)资源();
                if(value.状态==新语言含义采用状态::前提已变化)漂移();
                if(value.状态!=新语言含义采用状态::已采用||!value.信息)
                    throw std::logic_error("adopted demand target projection missing");
                projected=*value.信息;out.受请求者.reset();out.需求来源=projected.需求来源;
            }
            if(!out.需求来源||projected.需求来源!=*out.需求来源
                ||projected.需求来源.接收消息节点!=m.消息节点
                ||projected.需求主体!=speaker)
                throw std::logic_error("adopted demand projection source identity missing");
            // R121（7240 §5.4“需求信息节点”）：需求信息节点已在语言转换阶段于接收来源语言场景形成（见下方“未找到”分支）；
            // 这里只按来源消息引用该节点作为D'的需求信息来源。转换阶段未形成（本改动前已采用的消息）时按采用投影补形成。
            进入步骤("引用需求信息节点");
            if(!out.S)throw std::logic_error("adopted meaning has no received language scene");
            std::optional<稳定编码> 需求信息节点;
            if(const auto 已有=d.语言.按来源消息读取接收场景信息(m.消息节点)) {
                if(已有->内容.种类!=新接收场景信息种类::需求信息)throw std::logic_error("adopted demand source message holds non-demand scene information");
                需求信息节点=已有->陈述节点;
            } else {
                新接收场景信息 需求信息;
                需求信息.种类=新接收场景信息种类::需求信息;需求信息.来源消息=m.消息节点;需求信息.来源场景=*out.S;
                需求信息.交互场景=m.场景节点;需求信息.说话者=speaker;需求信息.主体存在=projected.需求主体;
                需求信息.目标存在=projected.目标存在;需求信息.特征类型=projected.目标特征类型;
                if(projected.方向){需求信息.值种类=新值陈述值种类::变化方向;需求信息.方向=projected.方向;}
                else if(projected.准确整数){需求信息.值种类=新值陈述值种类::准确整数;需求信息.准确整数=projected.准确整数;需求信息.单位=projected.单位;}
                else if(projected.区间下界&&projected.区间上界){
                    需求信息.值种类=新值陈述值种类::整数闭区间;需求信息.区间下界=projected.区间下界;需求信息.区间上界=projected.区间上界;需求信息.单位=projected.单位;
                }
                需求信息.原文起点=0;需求信息.原文终点=static_cast<std::int64_t>(标量数(m.UTF8正文));需求信息.时刻=m.正式记录时间.纳秒;
                if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入需求信息保存"};
                const auto 需求信息保存=d.语言.建立或取得接收场景信息(需求信息);
                if(需求信息保存.状态==新接收场景信息保存状态::前提已变化)漂移();
                if((需求信息保存.状态!=新接收场景信息保存状态::已保存&&需求信息保存.状态!=新接收场景信息保存状态::精确重复)
                    ||!需求信息保存.陈述节点)
                    throw std::logic_error("received demand information conflicts with saved record");
                需求信息节点=需求信息保存.陈述节点;
            }
            const 新接收认知来源 source{*out.需求来源,*out.U};
            进入步骤("读取同源被认知需求");
            const auto known=d.需求.查询接收认知来源需求(source,d.语言,d.特征);
            if(known.状态==新需求操作状态::资源失败)资源();if(known.状态==新需求操作状态::前提已变化)漂移();
            if(known.状态==新需求操作状态::目标内容依赖未就绪)throw 推进中断{阶段::资源延后,"需求目标内容依赖未就绪，保留原认知进度等待重试"};
            if(known.状态==新需求操作状态::已找到){
                if(!known.信息||!known.信息->首次接收认知来源)throw std::logic_error("same-source demand read returned no source");
                out.D=known.信息->节点;
            } else if(known.状态!=新需求操作状态::需求不存在)throw std::logic_error("same-source demand lookup failed");
            {
                进入步骤("映射正式需求目标");
                const auto target=映射目标(d,projected,stop);
                if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入需求发表"};
                进入步骤("发表同源被认知需求");
                const auto created=d.需求.建立或取得接收认知需求({source,target,需求信息节点},d.语言,d.特征);
                if(created.状态==新需求操作状态::资源失败)资源();if(created.状态==新需求操作状态::前提已变化)漂移();
                if(created.状态==新需求操作状态::目标内容依赖未就绪)throw 推进中断{阶段::资源延后,"需求目标内容依赖未就绪，保留原认知进度等待重试"};
                if((created.状态!=新需求操作状态::已建立&&created.状态!=新需求操作状态::已复用)||!created.需求节点)
                    throw std::logic_error("same-source demand publication failed");
                // 同源D即使上次已发表，也必须经原owner补齐五F再报告阶段完成。
                if(out.D&&out.D!=created.需求节点)throw std::logic_error("same-source demand identity changed during reentry");
                out.D=created.需求节点;out.阶段=阶段::已建立D;out.原因="已建立待完善被认知需求，未入自我需求树";
                if(work.观察.阶段==阶段::已建立D&&known.状态==新需求操作状态::已找到) {
                    进入步骤("正式读回同源被认知需求");
                    const auto verified=d.需求.查询接收认知来源需求(source,d.语言,d.特征);
                    if(verified.状态==新需求操作状态::资源失败)资源();if(verified.状态==新需求操作状态::前提已变化)漂移();
                    if(verified.状态==新需求操作状态::目标内容依赖未就绪)throw 推进中断{阶段::资源延后,"需求目标内容依赖未就绪，保留原认知进度等待重试"};
                    if(verified.状态!=新需求操作状态::已找到||!verified.信息||verified.信息->节点!=*out.D
                        ||verified.信息->首次接收认知来源!=std::optional{source})
                        throw std::logic_error("same-source demand final readback missing");
                    if(out.受请求者) {
                        进入步骤("复核正式行动请求与需求来源");
                        const auto fresh=d.语言.读取采用行动请求保护(*out.U,*out.受请求者,d.特征,d.存在);
                        if(fresh.状态==新语言含义采用状态::资源失败)资源();
                        if(fresh.状态==新语言含义采用状态::前提已变化)漂移();
                        if(fresh.状态!=新语言含义采用状态::已采用||!fresh.信息
                            ||fresh.信息->需求来源!=source.键||fresh.信息->请求来源!=k
                            ||fresh.信息->说话者H!=speaker)
                            throw std::logic_error("request and child demand final source inconsistent");
                        const auto qualification=d.需求.读取被认知需求完整资格保护(*out.D,speaker);
                        if(qualification.状态==新被认知需求资格状态::资源失败)资源();
                        if(qualification.状态==新被认知需求资格状态::前提已变化)漂移();
                        if(qualification.状态==新被认知需求资格状态::内容待完善)
                            throw 推进中断{阶段::D映射受阻,"已保存需求尚不符合完整内容概念，未承接"};
                        if(qualification.状态!=新被认知需求资格状态::已读取||!qualification.信息)
                            throw std::logic_error("request child demand complete qualification unavailable");
                        out.阶段=阶段::已读回行动请求;out.原因="明确行动请求及同源需求子内容正式读回；未承接";
                        out.待交接请求=新行动请求承接来源{source,*out.D,speaker,*out.受请求者};
                    } else {out.阶段=阶段::已读回;out.原因="同源语言采用与被认知需求正式读回；未自动承接";}
                    本次终结=true;
                }
            }
        } else if(adopted.状态==新语言含义采用状态::未找到){
            接收专名读取器 names(d,context.现实根,stop);
            语言基础统一输入请求 req{context.语言,固定来源.固定UTF8原文,{},0,0,0,0,0,&names,语言解释读取模式::本次正式有限集完整读取};
            req.交互上下文=语言图交互上下文{m.消息节点,speaker,*context.已绑定自我,context.现实根};
            req.正式人称 = 本次人称;
            std::vector<新接收指代材料> 本次人称材料;
            const auto 收集人称 = [&](const 语言基础统一输入结果& 图, 解释原文范围 句) {
                auto 项 = 投影原消息人称(图,k,句);
                本次人称材料.insert(本次人称材料.end(),
                    std::make_move_iterator(项.begin()),std::make_move_iterator(项.end()));
            };
            const auto 句段组 = 投影接收篇章句界(req.固定原文);
            const auto 同窗前文 = 正式词段重入 ? std::vector<语言场景局部称名>{}
                : 读取同窗已声明局部称名(d, *角色.信息, stop);
            req.局部称名组 = 同窗前文;
            req.读取静态角色缺口 = true;
            const auto 词头来源 = [&] {
                std::lock_guard 锁(impl.锁);
                return impl.可信词头来源;
            }();
            const auto 读当前句 = [&] {
                auto 结果 = 只读解释单句基础语言(req,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
                using I = 语言场景基础输入状态;
                if (结果.状态 == I::读取漂移 || 结果.状态 == I::引用不可读) 漂移();
                if (结果.状态 == I::工程未完成 || 结果.状态 == I::额度不足) 资源();
                if (结果.状态 == I::内部不一致) throw std::logic_error("received sentence interpretation inconsistent");
                return 结果;
            };
            const auto 解释当前句 = [&] {
                auto 初读 = 读当前句();
                if (!正式词段重入 && 取得欠缺的可信词头(d, 词头来源, req.语言节点, req.固定原文, 初读, stop))
                    初读 = 读当前句();
                if (!正式词段重入 && req.局部称名组.empty() && req.固定原文 == m.UTF8正文
                    && 初读.静态结构
                    && 初读.静态结构->操作 == 新语言知识含义操作::特征新增请求) {
                    const auto 新增句 = 记录句段解释(初读, {0, 标量数(req.固定原文)});
                    const auto 称名 = 读取同窗已声明局部称名(d, *角色.信息, stop, nullptr, &新增句);
                    if (!称名.empty()) {
                        req.局部称名组 = 称名;
                        auto 已补主体 = 读当前句();
                        req.局部称名组 = {};
                        return 已补主体;
                    }
                }
                return 初读;
            };
            const auto 确认语言场景 = [&] {
                检查认知停止(stop);
                const auto s = d.语言世界.建立或取得接收来源场景(m.消息节点);
                if (s.状态 == 新来源子场景状态::资源失败) 资源();
                if (s.状态 == 新来源子场景状态::前提已变化) 漂移();
                if (!s.信息) throw std::logic_error("received language source scene missing");
                out.S = s.信息->场景节点;
                out.被动解释来源 = k;
            };
            const auto 保存解释材料 = [&](std::vector<接收语言句段解释> 解释组) {
                检查认知停止(stop);
                关联同文待绑定任务(解释组,固定来源.固定UTF8原文);
                using S = 新接收指代调用状态;
                auto 指代材料 = 本次人称材料;
                for (const auto& 句 : 解释组) {
                    if (!句.主体关系 || !句.主体关系->指代概念 || !句.主体关系->来源) continue;
                    const auto& 关系 = *句.主体关系;
                    新接收指代材料 出现{{k, {关系.起始标量, 关系.结束标量}, {}},
                        {句.起始标量, 句.结束标量}, "主体", {}, {}};
                    if (关系.来源->已绑定存在)
                        出现.值 = 新接收指代关系值{新接收指代直接目标{
                            新绑定指称{新指称类别::存在, *关系.来源->已绑定存在}}};
                    if (!std::ranges::any_of(指代材料, [&](const auto& 项) { return 项.出现 == 出现.出现; }))
                        指代材料.push_back(std::move(出现));
                }
                auto 保存 = d.语言.保存接收语言解释及指代关系(k,
                    {*out.S,固定来源.节点,context.语言,k.原文起点,k.原文终点,解释组}, 指代材料,
                    speaker,d.语言世界,d.外设);
                if (保存.状态 == S::资源失败) 资源();
                if (保存.状态 == S::前提已变化 || 保存.状态 == S::未找到) 漂移();
                if (保存.状态 != S::已完成 || !保存.信息)
                    throw std::logic_error("received typed language scene material was not published");
                const auto& 正式句段 = 保存.信息->内容.句段组;
                if (正式句段.size() == 2 && 正式句段.back().主体关系 && 正式句段.back().静态结构
                    && 正式句段.back().静态结构->操作 == 新语言知识含义操作::任务当前等待描述) {
                    // 保存时副本补了同文未决任务，控制读取须用成功保存的同一份材料。
                    out.句段解释 = 正式句段;
                }
                if (处理局部组成材料(d, out, *角色.信息, *保存.信息, stop)) {
                    out.部分成文 = 从接收部分含义成文并独立回读(d, *保存.信息, k, req.交互上下文, stop, 步骤);
                    throw 推进中断{阶段::已保存场景信息, "已发表并读回同一直接场景的局部组成"};
                }
                if (处理局部具名特征材料(d, out, *角色.信息, *保存.信息, stop))
                    throw 推进中断{阶段::已保存场景信息, "已保存有来源的局部空特征，当前值未知"};
                if (!保存.信息->篇章采用) {
                    auto 采用 = d.语言.采用当前接收指代关系篇章(k, speaker,
                        保存.信息->解释定义节点, d.语言世界, d.外设);
                    if (采用.状态 == S::资源失败) 资源();
                    if (采用.状态 == S::前提已变化 || 采用.状态 == S::未找到) 漂移();
                    if (采用.状态 == S::已完成 && 采用.信息) 保存 = std::move(采用);
                    else if (采用.状态 != S::材料未就绪)
                        throw std::logic_error("received typed reference adoption could not be published");
                }
                if (const auto& 采用=保存.信息->篇章采用) {
                    out.部分成文.reset();
                    if (!out.篇章成文 || out.篇章成文->采用!=采用->节点)
                        out.篇章成文=接收篇章成文观察{采用->节点,接收篇章成文状态::待成文};
                    return true;
                }
                out.部分成文 = 从接收部分含义成文并独立回读(d, *保存.信息, k, req.交互上下文, stop, 步骤);
                return false;
            };
            if (!正式词段重入 && !句段组.empty()) {
                进入步骤("自然语言场景逐句解释");
                确认语言场景();
                std::vector<接收语言句段解释> 解释组;
                解释组.reserve(句段组.size());
                接收篇章主体观察 主体观察;
                std::vector<语言场景局部称名> 称名;
                for (const auto& 句段 : 句段组) {
                    检查认知停止(stop);
                    称名=读取同源局部称名(d,k,stop,句段.全文范围.起始标量);
                    称名.insert(称名.end(), 同窗前文.begin(), 同窗前文.end());
                    req.局部称名组=称名;
                    req.固定原文 = 句段.句面原文;
                    const auto 解释 = 解释当前句();
                    收集人称(解释,句段.全文范围);
                    auto& 句 = 解释组.emplace_back(记录句段解释(解释, 句段.全文范围, d.关系陈述));
                    记录待完善谓项概念(d, 固定来源, 解释, 句, stop);
                    主体观察.记录(解释, req.语言节点, d.名称, d.语言世界, 固定来源, 句);
                }
                out.句段解释 = std::move(解释组);
                const bool 已采用=保存解释材料(out.句段解释);
                if (已采用) throw 推进中断{阶段::已保存场景信息,"已读回同源正式篇章采用"};
                throw 推进中断{阶段::语言解释待完善, "已保存原文句段解释材料，等待篇章含义"};
            }
            新语言分界调用结果<新语言分界材料信息> 准入读回;
            const 新语言词条准入定义* 准入定义 = nullptr;
            if (正式词段重入) {
                using 准入状态 = 新语言分界工程状态;
                进入步骤("读取同源正式词段");
                if (!out.词条准入 || out.词条准入->来源 != k || out.词条准入->前置来源 != 固定来源.节点)
                    throw std::logic_error("formal word admission lacks original source");
                const auto 范围 = out.词条准入->范围;
                准入读回 = d.语言.读取当前接收词条准入(范围, 固定来源.节点);
                switch (准入读回.状态) {
                case 准入状态::已完成: break;
                case 准入状态::资源失败: 资源();
                case 准入状态::未找到:
                case 准入状态::未初始化:
                case 准入状态::材料未就绪:
                case 准入状态::前提已变化: 漂移();
                default: throw std::logic_error("formal word admission read rejected");
                }
                if (准入读回.信息 && 准入读回.信息->所属范围 == 范围)
                    准入定义 = std::get_if<新语言词条准入定义>(&准入读回.信息->定义);
                if (!准入定义) throw std::logic_error("formal word admission payload missing");
                req.读取静态角色缺口 = true;
            }
            const auto 解释正式句段 = [&](解释原文范围 范围) {
                req.正式词段 = 语言图正式词段视图{准入定义, 范围};
                return 解释当前句();
            };
            const auto 是子类型定义 = [](const 语言基础统一输入结果& 结果) {
                using S = 基础语言输入状态;
                return 结果.静态结构
                    && 结果.静态结构->操作 == 新语言知识含义操作::特征子类型定义
                    && (结果.图解释状态 == S::已形成唯一含义 || 结果.图解释状态 == S::静态角色待取材);
            };
            const auto 有普通词条名称缺口 = [&](const 语言基础统一输入结果& 结果) {
                return !是子类型定义(结果)
                    && std::ranges::any_of(结果.静态角色缺口组,
                        [](const auto& 缺口) {return 缺口.正式普通名称未登记 && 缺口.词条;});
            };
            const auto 保存正式词段子类型解释 = [&] {
                确认语言场景();
                auto 待保存句段 = out.句段解释;
                if (待保存句段.size() > 1) {
                    using S = 新语言分界工程状态;
                    const auto 目前 = d.语言.读取接收语言解释(k, d.语言世界, d.外设);
                    if (目前.状态 == S::资源失败) 资源();
                    if (目前.状态 == S::前提已变化) 漂移();
                    if (目前.状态 != S::已完成 && 目前.状态 != S::未找到)
                        throw std::logic_error("saved mixed language interpretation could not be read");
                    if (目前.信息) for (auto& 句 : 待保存句段) {
                        if (句.静态结构 && 句.静态结构->操作 == 新语言知识含义操作::特征子类型定义) continue;
                        const auto& 已保存 = 目前.信息->内容.句段组;
                        const auto 同源句 = std::ranges::find_if(已保存, [&](const auto& 原句) {
                            return 原句.起始标量 == 句.起始标量 && 原句.结束标量 == 句.结束标量;
                        });
                        if (同源句 != 已保存.end()) 句 = *同源句;
                    }
                }
                (void)保存解释材料(待保存句段);
                throw 推进中断{阶段::语言解释待完善, "已在同源语言场景保存正式词段的子类型定义解释"};
            };
            if (!句段组.empty()) {
                进入步骤("按正式词段逐句解释");
                const auto 冲突组 = 核对接收篇章句间分隔(固定来源.固定UTF8原文, 句段组, *准入定义);
                std::vector<接收语言句段解释> 解释组;
                std::vector<std::pair<std::size_t, 语言基础统一输入结果>> 子类型解释组;
                for (const auto& 句段 : 句段组) {
                    检查认知停止(stop);
                    req.固定原文 = 句段.句面原文;
                    auto 结果 = 解释正式句段(句段.全文范围);
                    收集人称(结果,句段.全文范围);
                    需要普通词条筹办 |= 有普通词条名称缺口(结果);
                    解释组.push_back(记录句段解释(结果, 句段.全文范围));
                    if (冲突组.empty() && 是子类型定义(结果))
                        子类型解释组.emplace_back(解释组.size() - 1, std::move(结果));
                }
                for (const auto& 冲突 : 冲突组) {
                    解释组.push_back({冲突.起始标量, 冲突.结束标量, 语言场景基础输入状态::结构未支持,
                        基础语言输入状态::正式词段句界冲突,
                        {{新前置语言缺口种类::语言规则不足, 冲突.起始标量, 冲突.结束标量, {}}}});
                }
                const bool 有句界冲突 = std::ranges::any_of(解释组, [](const auto& 证据) {
                    return 证据.图状态 == 基础语言输入状态::正式词段句界冲突;
                });
                if (有句界冲突) {
                    需要普通词条筹办 = false;
                } else {
                    for (const auto& [位置, 结果] : 子类型解释组)
                        记录待完善谓项概念(d, 固定来源, 结果, 解释组[位置], stop);
                }
                std::ranges::sort(解释组, {}, &接收语言句段解释::起始标量);
                out.句段解释 = std::move(解释组);
                out.词条准入->准入材料 = 准入读回.信息->节点;
                本次取得正式解释 = true;
                if (!有句界冲突 && !子类型解释组.empty()) 保存正式词段子类型解释();
                if (!有句界冲突 && !本次人称材料.empty()) {
                    确认语言场景();
                    if (保存解释材料(out.句段解释))
                        throw 推进中断{阶段::已保存场景信息,"已读回同源正式篇章采用"};
                }
                throw 推进中断{阶段::前置输入治理等待, "已按同源正式词段逐句解释，等待篇章对象绑定"};
            }
            进入步骤("读取语言解释资源");
            const auto resources=读资源(d,context.语言,stop);
            进入步骤("读取现实对象范围");
            const auto scope=读范围(d,context.现实根,stop);
            进入步骤("纯统一解释");
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
            const auto 解释诊断号 = 接收解释诊断序号.fetch_add(1, std::memory_order_relaxed) + 1;
            const auto 解释诊断开始 = std::chrono::steady_clock::now();
            记录接收解释调用(解释诊断号, m.消息节点, 解释诊断开始, nullptr);
#endif
            const 解释原文范围 整句{static_cast<std::size_t>(k.原文起点), static_cast<std::size_t>(k.原文终点)};
            auto parsed = 正式词段重入 ? 解释正式句段(整句) : 解释当前句();
            收集人称(parsed,整句);
            bool 已采用静态篇章=false;
            if (正式词段重入) {
                out.句段解释 = {记录句段解释(parsed, 整句)};
                out.词条准入->准入材料 = 准入读回.信息->节点;
                本次取得正式解释 = true;
                需要普通词条筹办 = 有普通词条名称缺口(parsed);
                if (是子类型定义(parsed)) {
                    记录待完善谓项概念(d, 固定来源, parsed, out.句段解释.front(), stop);
                    保存正式词段子类型解释();
                }
                if (!本次人称材料.empty()) {
                    确认语言场景();
                    已采用静态篇章 = 保存解释材料(out.句段解释);
                }
            } else if (parsed.状态 != 语言场景基础输入状态::输入拒绝) {
                确认语言场景();
                out.句段解释 = {记录句段解释(parsed, 整句, d.关系陈述)};
                记录待完善谓项概念(d, 固定来源, parsed, out.句段解释.front(), stop);
                接收篇章主体观察 主体观察;
                主体观察.记录(parsed, req.语言节点, d.名称, d.语言世界, 固定来源, out.句段解释.front());
                if (!本次人称材料.empty() || parsed.静态结构 || out.句段解释.front().关系观察
                    || (!parsed.含义 && parsed.完整候选组.empty()))
                    已采用静态篇章=保存解释材料(out.句段解释);
            }
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
            记录接收解释调用(解释诊断号, m.消息节点, 解释诊断开始, &parsed);
#endif
            if(parsed.状态==语言场景基础输入状态::读取漂移)漂移();
            if(parsed.状态==语言场景基础输入状态::工程未完成||parsed.状态==语言场景基础输入状态::额度不足)资源();
            if(parsed.状态==语言场景基础输入状态::内部不一致)throw std::logic_error("pure language explanation reported internal inconsistency");
            // R121（7240 §5.4“需求信息节点”）：需求陈述/请求达成在语言转换阶段就在接收来源语言场景形成需求信息节点，
            // 不依赖下方能否建成被认知需求，也不依赖需求主体是否为本次说话者；同一来源消息只一个节点，已有则不改。
            const auto 提取=提取需求信息(parsed,标量数(m.UTF8正文));
            if(提取) {
                auto 需求信息=std::optional<新接收场景信息>{提取->信息};
                if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入需求信息保存"};
                进入步骤("取得同源语言场景");
                const auto s=d.语言世界.建立或取得接收来源场景(m.消息节点);
                if(s.状态==新来源子场景状态::资源失败)资源();if(s.状态==新来源子场景状态::前提已变化)漂移();
                if(!s.信息)throw std::logic_error("received language source scene missing");out.S=s.信息->场景节点;
                进入步骤("保存需求信息节点");
                if(const auto 已有=d.语言.按来源消息读取接收场景信息(m.消息节点)) {
                    if(已有->内容.种类!=新接收场景信息种类::需求信息)throw std::logic_error("demand source message holds non-demand scene information");
                } else {
                    // X为普通名词：在接收来源语言场景中按概念X实例化一个存在，记为需求信息节点的目标存在（7240 §4.3）。
                    if(提取->待实例化目标概念) {
                        进入步骤("按概念实例化目标存在");
                        const auto 实例=d.语言世界.实例化概念(s.信息->场景节点,*提取->待实例化目标概念);
                        using IS=新自然语言概念实例操作状态;
                        if(实例.状态==IS::资源失败)资源();
                        if(实例.状态==IS::场景不存在||实例.状态==IS::概念不存在)漂移();
                        if(!实例.成功()||!实例.实例||!有效(实例.实例->节点))throw std::logic_error("received demand target concept instantiation failed");
                        需求信息->目标存在=实例.实例->节点;
                    }
                    需求信息->来源消息=m.消息节点;需求信息->来源场景=s.信息->场景节点;需求信息->交互场景=m.场景节点;
                    需求信息->说话者=speaker;需求信息->时刻=m.正式记录时间.纳秒;
                    const auto saved=d.语言.建立或取得接收场景信息(*需求信息);
                    if(saved.状态==新接收场景信息保存状态::前提已变化)漂移();
                    if((saved.状态!=新接收场景信息保存状态::已保存&&saved.状态!=新接收场景信息保存状态::精确重复)||!saved.陈述节点)
                        throw std::logic_error("received demand information conflicts with saved record");
                }
            }
            if(提取&&提取->目标存在需求) {
                // 最小需求句式“Y要X”只形成需求信息节点（等待自我取用，7240 §5.4）；本切片不据它建立被认知需求，
                // 需求主体非本次说话者时同样到此结束。
                out.阶段=阶段::已保存场景信息;本次终结=true;
                out.原因="收到最小需求陈述：已在接收来源场景形成需求信息节点，未建立被认知需求";
            } else {
            // §3.3 第1–3项：值类构式与招呼保存为场景信息，不建立需求；已经过前置输入治理的消息仍走原路径。
            auto 场景信息=(原阶段==实现::前置阶段::无 || 正式词段重入)
                ? 提取接收场景信息(parsed,标量数(m.UTF8正文)) : std::nullopt;
            if(场景信息) {
                if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入场景信息保存"};
                进入步骤("取得同源语言场景");
                const auto s=d.语言世界.建立或取得接收来源场景(m.消息节点);
                if(s.状态==新来源子场景状态::资源失败)资源();if(s.状态==新来源子场景状态::前提已变化)漂移();
                if(!s.信息)throw std::logic_error("received language source scene missing");out.S=s.信息->场景节点;
                场景信息->来源消息=m.消息节点;场景信息->来源场景=s.信息->场景节点;场景信息->交互场景=m.场景节点;
                场景信息->说话者=speaker;场景信息->时刻=m.正式记录时间.纳秒;
                if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"来源场景已保留，停止后不进入场景信息保存"};
                进入步骤("保存接收场景信息");
                const auto saved=d.语言.建立或取得接收场景信息(*场景信息);
                if(saved.状态==新接收场景信息保存状态::前提已变化)漂移();
                if((saved.状态!=新接收场景信息保存状态::已保存&&saved.状态!=新接收场景信息保存状态::精确重复)||!saved.陈述节点)
                    throw std::logic_error("received scene information conflicts with saved record");
                out.阶段=阶段::已保存场景信息;本次终结=true;
                out.原因=场景信息->种类==新接收场景信息种类::招呼 ? "收到招呼：已保存为场景信息，不形成需求"
                    : "收到值陈述：已保存为场景信息，不建立需求";
            } else if (已采用静态篇章) {
                out.阶段=阶段::已保存场景信息;
                本次终结=true;
                out.原因="已读回同源正式静态陈述采用";
            } else if (!正式词段重入 && out.句段解释.size()==1 && out.句段解释.front().关系观察) {
                if (!读取可成文关系(out.句段解释.front()))
                    throw 推进中断{阶段::语言解释待完善, "已保存关系候选，等待关系或指称材料"};
                out.阶段=阶段::已保存场景信息;
                本次终结=true;
                out.原因="已保存同源语言关系陈述";
            } else {
            if(parsed.状态!=语言场景基础输入状态::已形成值式图||!parsed.含义
                ||!std::holds_alternative<语言图含义材料>(*parsed.含义)
                ||(parsed.构式!=语言基础统一构式::需求陈述&&parsed.构式!=语言基础统一构式::行动请求))
                throw 推进中断{parsed.状态==语言场景基础输入状态::对象待绑定 ? 阶段::对象未决 : 阶段::非支持输入,"本条消息未形成唯一受支持的需求或行动请求"};
            const auto& graph=std::get<语言图含义材料>(*parsed.含义);
            const auto roles=读取认知图角色(graph);
            if(roles.主体!=speaker)
                throw 推进中断{阶段::非本次人类需求,"需求主体不是本消息说话者（当前交互者）"};
            if(roles.受请求者&&roles.受请求者!=context.已绑定自我)
                throw 推进中断{阶段::对象未决,"行动请求受话者不是本次正式自我"};
            if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入语言采用"};
            进入步骤("取得同源语言场景");
            const auto s=d.语言世界.建立或取得接收来源场景(m.消息节点);
            if(s.状态==新来源子场景状态::资源失败)资源();if(s.状态==新来源子场景状态::前提已变化)漂移();
            if(!s.信息)throw std::logic_error("received language source scene missing");out.S=s.信息->场景节点;
            if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"来源场景已保留，停止后不进入语言采用"};
            const auto contextStill=[&impl,context]() {
                std::lock_guard g(impl.锁);
                const auto it=impl.会话组.find(context.公开宿主会话);
                return it!=impl.会话组.end()&&!it->second.封存&&it->second.语境==context;
            };
            接收采用读取器 reader(d,context,m,resources,scope,parsed,names.项目,speaker,stop,contextStill);
            进入步骤("发表同源语言采用");
            const auto u=d.语言.建立或取得接收采用含义({k,*out.S,speaker,context.现实根,固定来源.固定文本材料,graph,parsed.图角色原文定位组},d.特征,d.特征概念,reader);
            if(u.状态==新语言含义采用状态::资源失败)资源();if(u.状态==新语言含义采用状态::前提已变化)漂移();
            if(u.状态==新语言含义采用状态::输入拒绝)
                throw 推进中断{阶段::非支持输入,"语言角色的正式特征引用材料尚不受支持，保留原消息及来源场景"};
            if((u.状态!=新语言含义采用状态::已采用&&u.状态!=新语言含义采用状态::精确重复)||!u.信息)
                throw std::logic_error("same-source meaning adoption failed");
            out.U=u.信息->节点;out.阶段=阶段::已采用U;
            out.受请求者=roles.受请求者;out.需求来源.reset();
            out.原因=roles.受请求者 ? "已采用明确请求及本句需求内容，尚未建立需求D" : "已采用真实消息的唯一需求含义，尚未建立需求D";
            }
            }
        } else throw std::logic_error("same-source meaning lookup failed");
    }catch(const 推进中断& e){out.阶段=e.状态;out.原因=std::string(步骤)+"："+e.原因;本次终结=e.状态!=阶段::资源延后&&e.状态!=阶段::前提变化&&e.状态!=阶段::停止未完成;
        if (e.状态 == 阶段::语言解释待完善
            || (!正式词段重入 && out.被动解释来源 && !out.U && !out.D
                && (e.状态 == 阶段::对象未决 || e.状态 == 阶段::非支持输入))) {
            out.阶段 = 阶段::语言解释待完善;
            本次终结 = false;
        }
        if(e.状态==阶段::停止未完成&&work.定位){work.前置=实现::前置阶段::已停用;out.待交接前置输入.reset();}
    }
    catch(const std::bad_alloc&){out.阶段=阶段::资源延后;out.待交接请求.reset();本次终结=false;out.原因="认知推进分配资源未完成";}
    catch(const std::length_error&){out.阶段=阶段::资源延后;out.待交接请求.reset();本次终结=false;out.原因="认知推进材料资源未完成";}
    if (本次终结 && out.阶段==阶段::已保存场景信息 && out.被动解释来源 && 篇章成文语境
        && out.篇章成文 && out.篇章成文->状态==接收篇章成文状态::待成文) {
        const auto 等待成文=[&](const char* 原因) {
            本次终结=false;
            out.原因=原因;
        };
        try {
            检查认知停止(stop);
            using L=新语言分界工程状态;
            const auto 正式=d.语言.读取接收语言解释(*out.被动解释来源,d.语言世界,d.外设);
            if (正式.状态==L::资源失败) 资源();
            if (正式.状态==L::前提已变化 || 正式.状态==L::未找到) 漂移();
            if (正式.状态!=L::已完成 || !正式.信息 || !正式.信息->篇章采用
                || 正式.信息->篇章采用->节点!=out.篇章成文->采用)
                throw std::logic_error("adopted statement source changed during generation");
            out.篇章成文=从采用篇章成文并独立回读(d,*正式.信息->篇章采用,
                *out.被动解释来源,*篇章成文语境,stop);
            if (out.篇章成文->状态==接收篇章成文状态::已成文且独立回读)
                out.原因="固定篇章已成文并独立回读一致";
        } catch (const 推进中断& e) {
            等待成文(e.状态==阶段::停止未完成 ? "已保留正式篇章，停止本次成文"
                : "已保留正式篇章，等待重新读取成文材料");
        } catch (const std::bad_alloc&) {
            等待成文("已保留正式篇章，等待成文资源");
        } catch (const std::length_error&) {
            等待成文("已保留正式篇章，等待成文资源");
        }
    }
    if (本次取得正式解释 && 需要普通词条筹办 && !out.U && !out.D && work.定位 && !stop.stop_requested()) {
        work.前置 = 实现::前置阶段::待接纳;
        out.阶段 = 阶段::前置输入待治理;
        out.原因 = "同源正式词段解释取得普通内容词名称缺口，等待筹办确认";
        out.待交接前置输入 = work.定位;
        本次终结 = false;
    }
    const bool 有新正式观察 = 本次取得正式解释
        || (out.U && out.U != work.观察.U) || (out.D && out.D != work.观察.D)
        || (本次终结 && out.D && out.阶段 != work.观察.阶段);
    const bool 待语言重读 = out.阶段 == 阶段::语言解释待完善;
    const bool 观察未变 = 待语言重读 && work.观察.阶段 == out.阶段
        && work.观察.S == out.S && work.观察.被动解释来源 == out.被动解释来源
        && work.观察.原因 == out.原因 && work.观察.句段解释 == out.句段解释
        && work.观察.部分成文 == out.部分成文;
    work.观察 = out;
    work.终结 = 本次终结;
    work.下次 = std::chrono::steady_clock::now()
        + (待语言重读 ? std::chrono::milliseconds(2000) : std::chrono::milliseconds(500));
    if (!提交进度() || 观察未变 || (正式词段重入 && !有新正式观察)) return {};
    保存观察(out);return out;
    }catch(const std::bad_alloc&){
        接收需求认知观察 out{本次消息.value_or(稳定编码{}),{},{},{},{},阶段::资源延后,"认知调度分配资源未完成"};
        保存观察(out);return out;
    }catch(const std::length_error&){
        接收需求认知观察 out{本次消息.value_or(稳定编码{}),{},{},{},{},阶段::资源延后,"认知调度材料资源未完成"};
        保存观察(out);return out;
    }
}
} // namespace 海中鱼巣
