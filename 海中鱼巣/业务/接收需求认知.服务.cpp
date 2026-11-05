#include "接收需求认知.服务.h"
#include "新_外设世界树类.h"
#include "转换.语言场景基础.h"
#include "转换.语言场景共同.h"
#include "../领域/新_自然语言世界树类.h"
#include "../领域/新_场景类.h"
#include "../领域/新_需求类.h"
#include "../领域/新_任务类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/新_动态类.h"
#include "../适配/适配器.UTC时钟.h"
#include "../领域/概念_二次特征类.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
#include <atomic>
#endif
#include <sstream>
#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <stdexcept>
#include <tuple>

namespace 海中鱼巣 {
namespace {
using 阶段=接收需求认知阶段;
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
std::atomic_uint64_t 接收解释诊断序号{0};
void 记录接收解释调用(std::uint64_t 序号, 稳定编码 消息,
    std::chrono::steady_clock::time_point 开始, const 语言基础统一输入结果* 结果) noexcept {
    if (序号 > 64) return;
    try {
        std::wostringstream 文本;
        文本 << L"调用序号=" << 序号 << L"；消息=" << 消息.值
            << L"；环节=" << (结果 ? L"返回" : L"开始")
            << L"；耗时毫秒=" << std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - 开始).count();
        if (结果) {
            文本 << L"；统一状态=" << static_cast<int>(结果->状态)
                << L"；图解释状态=" << (结果->图解释状态 ? static_cast<int>(*结果->图解释状态) : -1)
                << L"；完整候选数=" << 结果->完整候选组.size()
                << L"；受阻节点数=" << 结果->受阻节点组.size();
        }
        (void)记录调试日志(L"RECEIVED_DEMAND_PARSE", L"接收需求认知", L"只读解释单句基础语言", 文本.str());
    } catch (...) {} // 仅人读观察，日志分配或写入失败不改变原返回和重试。
}
#endif
struct 推进中断 final {阶段 状态;std::string 原因;};
void 检查认知停止(std::stop_token stop){
    if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止前保留已接纳消息及已发表材料"};
}
[[noreturn]] void 漂移(){throw 推进中断{阶段::前提变化,"正式来源或解释依据已变化"};}
[[noreturn]] void 资源(){throw 推进中断{阶段::资源延后,"正式读取或分配资源未完成"};}
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
        &&a.正文材料节点==b.正文材料节点&&a.来源==b.来源&&a.UTF8正文==b.UTF8正文;
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
};
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
        const std::vector<专名现场项>& names,std::stop_token stop,std::function<bool()> 语境仍有效)
        :d_(d),context_(context),m_(m),resources_(resources),scope_(scope),parsed_(parsed),names_(names),stop_(stop),语境仍有效_(std::move(语境仍有效)){}
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
            const auto roles=读取认知图角色(std::get<语言图含义材料>(*parsed_.含义));
            if(roles.主体!=context_.已绑定人类)漂移();
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
            out.说话者节点=*context_.已绑定人类;out.现实范围根=context_.现实根;out.原文材料节点=m_.正文材料节点;
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
    if(p.待澄清动作)return {}; // 明确没有目标投影，原动作只保存在语言来源中。
    新部分服务要求目标 out;out.目标存在节点=p.目标存在;out.目标特征类型概念节点=p.目标特征类型;
    if(!p.准确整数&&!p.区间下界&&!p.区间上界&&!p.方向)return out;
    const auto r=d.特征概念.读取特征概念定义保护(p.目标特征类型);
    if(r.前提已变化)漂移();if(r.状态==基础结构操作状态::资源失败)资源();
    if(r.状态!=基础结构操作状态::已完成||!r.信息)throw std::logic_error("adopted target FT unreadable");
    const auto& def=r.信息->定义;
    if(r.信息->类型根!=p.目标特征类型||def.材料类型!=特征概念材料物理类型::I64标量||(!p.方向&&def.单位!=p.单位))
        throw 推进中断{阶段::D映射受阻,"目标FT类型或单位暂不能精确映射"};
    if(p.方向){out.目标形式=新需求目标形式::变化方向;out.目标变化方向=*p.方向==新语言知识特征方向::增加 ? 新需求变化方向::增加 : 新需求变化方向::减少;return out;}
    const auto lo=读I64(p.准确整数 ? *p.准确整数 : *p.区间下界),hi=读I64(p.准确整数 ? *p.准确整数 : *p.区间上界);
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
} // namespace

struct 接收需求认知服务::实现 final {
    认知依赖 依赖;
    新_任务类* 任务=nullptr;新_方法类* 方法=nullptr;新_动态类* 动态=nullptr;稳定编码 答案方法{};
    struct 会话 final {接收需求认知会话语境 语境;bool 封存=false;};
    struct 调度 final {接收需求认知观察 观察;bool 终结=false;std::chrono::steady_clock::time_point 下次{};bool 行动请求已交接=false;};
    mutable std::mutex 锁;
    std::map<std::string,会话,std::less<>> 会话组;
    std::map<稳定编码,调度> 进度;
    std::optional<接收需求认知观察> 最近;
    std::optional<稳定编码> 上次;
    explicit 实现(认知依赖 d):依赖(d){}
};
接收需求认知服务::接收需求认知服务(新_外设世界树类& a,新_自然语言世界树类& b,新_语言类& c,新_词类& e,
    概念_名称关系类& n,概念_存在类& ce,概念_特征类& cf,概念_二次特征类& c2,新_存在类& ex,
    新_特征值类& v,新_特征类& f,新_场景类& s,新_需求类& d)
    :实现_(std::make_unique<实现>(认知依赖{a,b,c,e,n,ce,cf,c2,ex,v,f,s,d})){}
接收需求认知服务::~接收需求认知服务()=default;
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
    try {std::lock_guard g(实现_->锁);const auto it=实现_->会话组.find(c);if(it!=实现_->会话组.end())it->second.封存=true;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"封存接收认知会话",L"接收认知会话封存发生工程异常。");}
}
std::optional<接收需求认知观察> 接收需求认知服务::读取最近观察() const {std::lock_guard g(实现_->锁);return 实现_->最近;}

接收行动请求交接确认状态 接收需求认知服务::确认行动请求已交接(
    const 新行动请求承接来源& source) noexcept {
    using S=接收行动请求交接确认状态;
    if(!有效(source.需求来源.键.接收消息节点))return S::输入拒绝;
    try {
        std::lock_guard g(实现_->锁);
        const auto it=实现_->进度.find(source.需求来源.键.接收消息节点);
        if(it==实现_->进度.end()||(it->second.观察.阶段!=阶段::已读回行动请求&&it->second.观察.阶段!=阶段::已读回待完善请求))return S::输入拒绝;
        auto& work=it->second;const auto& out=work.观察;
        if(out.M!=source.需求来源.键.接收消息节点||out.U!=source.需求来源.采用含义节点
            ||out.D!=source.被认知需求节点||out.H!=source.来源人类节点
            ||out.受请求者!=source.受请求者节点||out.需求来源!=std::optional{source.需求来源.键})
            return S::来源冲突;
        if(work.行动请求已交接)return S::精确重复;
        work.行动请求已交接=true;work.观察.待交接请求.reset();work.观察.待完善交接.reset();
        if(实现_->最近&&实现_->最近->M==out.M){实现_->最近->待交接请求.reset();实现_->最近->待完善交接.reset();}
        return S::已确认;
    }catch(const std::bad_alloc&){return S::资源失败;}
    catch(const std::length_error&){return S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"确认行动请求交接",L"认知请求交接进度不变量异常。");}
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
            (void)记录调试日志(L"RECEIVED_DEMAND_SCHED",L"接收需求认知",L"推进一次观察",t.str());
        }catch(...){}
    };
    try {
    if(stop.stop_requested())return {};
    const auto records=d.外设.完整读取仍保留的接收记录();
    if(records.状态==新外设事实状态::资源失败){
        接收需求认知观察 out{{},{},{},{},{},阶段::资源延后,"接收记录完整读取资源未完成"};
        保存观察(out);return out;
    }
    if(records.状态!=新外设事实状态::已完成)throw std::logic_error("retained receive record enumeration failed");
    if(stop.stop_requested())return {};
    const 新外设消息信息* chosen=nullptr;
    const auto now=std::chrono::steady_clock::now();
    // 公平轮转，不让单条反复前提变化阻挡后续真实M。
    // 已登记且未封存会话上的M优先：旧会话残留不得长期挡住当前入站认知（§8.2 真实M公平）。
    const auto eligible=[&](const auto& m){const auto it=impl.进度.find(m.消息节点);return it==impl.进度.end()
        ||((!it->second.终结||((it->second.观察.阶段==阶段::已读回行动请求||it->second.观察.阶段==阶段::已读回待完善请求)&&!it->second.行动请求已交接))&&it->second.下次<=now);};
    const auto 会话仍活跃=[&](const auto& m){
        std::lock_guard g(impl.锁);const auto it=impl.会话组.find(m.来源.公开宿主会话);
        return it!=impl.会话组.end()&&!it->second.封存;};
    std::size_t start=0;
    if(impl.上次){const auto prev=std::find_if(records.消息组.begin(),records.消息组.end(),[&](const auto& m){return m.消息节点==*impl.上次;});
        if(prev!=records.消息组.end())start=static_cast<std::size_t>(prev-records.消息组.begin())+1;}
    const auto 轮转选取=[&](bool 仅活跃会话){
        for(std::size_t i=0;i<records.消息组.size();++i){
            const auto& m=records.消息组[(start+i)%records.消息组.size()];
            if(!eligible(m))continue;
            if(仅活跃会话&&!会话仍活跃(m))continue;
            return &m;}
        return static_cast<const 新外设消息信息*>(nullptr);};
    chosen=轮转选取(true);
    if(!chosen)chosen=轮转选取(false);
    if(!chosen){
        try{std::wostringstream t;t<<L"保留接收条数="<<records.消息组.size()<<L"；本拍无合格M";
            (void)记录调试日志(L"RECEIVED_DEMAND_SCHED",L"接收需求认知",L"推进一次",t.str());}catch(...){}
        return {};}
    const auto& m=*chosen;impl.上次=m.消息节点;本次消息=m.消息节点;
    auto& work=impl.进度[m.消息节点];auto out=work.观察;out.M=m.消息节点;
    if(work.终结&&(out.阶段==阶段::已读回行动请求||out.阶段==阶段::已读回待完善请求)&&!work.行动请求已交接) {
        if(!out.U||!out.D||!out.H||!out.受请求者||!out.需求来源)
            throw std::logic_error("completed request handoff source missing");
        const 新行动请求承接来源 source{{*out.需求来源,*out.U},*out.D,*out.H,*out.受请求者};
        if(out.阶段==阶段::已读回待完善请求) {
            const auto window=d.外设.读取对话框来源(m.窗口节点);
            if(!window||window->公开宿主会话!=m.来源.公开宿主会话||window->对话框标识!=m.来源.对话框标识
                ||window->定位.场景节点!=m.场景节点) {
                out.待完善交接.reset();out.待交接请求.reset();out.原因="待完善请求原会话前提已变化";
            }
            work.下次=std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
            保存观察(out);return out;
        }
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
        保存观察(out);return out;
    }
    bool 本次终结=work.终结;
    const auto 进入步骤=[&](const char* name){
        步骤=name;
        if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止前保留已接纳消息"};
        // 只更新工程观察，不提前改变已完成阶段或预造U/D。
        auto current=out;current.原因=std::string("正在")+name;保存观察(current);
    };
    try {
        进入步骤("读取正式会话语境");
        接收需求认知会话语境 context;
        {std::lock_guard g(impl.锁);const auto it=impl.会话组.find(m.来源.公开宿主会话);
            if(it==impl.会话组.end())throw 推进中断{阶段::非本次人类需求,"消息未接入正式会话语境，保留原始接收事实"};context=it->second.语境;}
        out.H=context.已绑定人类;
        if(!context.已绑定人类)throw 推进中断{阶段::非本次人类需求,"匿名会话不推定人类主体"};
        const 新接收认知来源键 k{m.消息节点,0,标量数(m.UTF8正文),{}};
        if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止前保留已接纳消息"};
        进入步骤("读取同源语言采用");
        auto adopted=d.语言.读取接收采用含义(k,d.特征);
        if(adopted.状态==新语言含义采用状态::资源失败)资源();if(adopted.状态==新语言含义采用状态::前提已变化)漂移();
        if(adopted.状态==新语言含义采用状态::已采用){
            if(!adopted.信息)throw std::logic_error("adopted meaning source returned no payload");
            out.U=adopted.信息->节点;out.S=adopted.信息->场景节点;
            const auto roles=读取认知图角色(adopted.信息->含义);
            if(roles.主体!=*context.已绑定人类)throw std::logic_error("saved demand speaker conflicts with source context");
            新语言需求目标投影 projected;
            if(roles.受请求者) {
                if(!context.已绑定自我)throw 推进中断{阶段::对象未决,"本次会话缺少正式受请求者"};
                进入步骤("读取同源行动请求");
                const auto request=d.语言.读取采用行动请求保护(*out.U,*context.已绑定自我,d.特征,d.存在);
                if(request.状态==新语言含义采用状态::资源失败)资源();
                if(request.状态==新语言含义采用状态::前提已变化)漂移();
                if(request.状态!=新语言含义采用状态::已采用||!request.信息)
                    throw std::logic_error("saved request projection conflicts with adopted graph");
                if(request.信息->请求来源!=k||request.信息->说话者H!=*context.已绑定人类
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
                ||projected.需求主体!=*context.已绑定人类)
                throw std::logic_error("adopted demand projection source identity missing");
            const 新接收认知来源 source{*out.需求来源,*out.U};
            进入步骤("读取同源被认知需求");
            const auto known=d.需求.查询接收认知来源需求(source,d.语言,d.特征);
            if(known.状态==新需求操作状态::资源失败)资源();if(known.状态==新需求操作状态::前提已变化)漂移();
            if(known.状态==新需求操作状态::已找到){
                if(!known.信息||!known.信息->首次接收认知来源)throw std::logic_error("same-source demand read returned no source");
                out.D=known.信息->节点;
            } else if(known.状态!=新需求操作状态::需求不存在)throw std::logic_error("same-source demand lookup failed");
            {
                进入步骤("映射正式需求目标");
                const auto target=映射目标(d,projected,stop);
                if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入需求发表"};
                进入步骤("发表同源被认知需求");
                const auto created=d.需求.建立或取得接收认知需求({source,target},d.语言,d.特征);
                if(created.状态==新需求操作状态::资源失败)资源();if(created.状态==新需求操作状态::前提已变化)漂移();
                if((created.状态!=新需求操作状态::已建立&&created.状态!=新需求操作状态::已复用)||!created.需求节点)
                    throw std::logic_error("same-source demand publication failed");
                // 同源D即使上次已发表，也必须经原owner补齐五F再报告阶段完成。
                if(out.D&&out.D!=created.需求节点)throw std::logic_error("same-source demand identity changed during reentry");
                out.D=created.需求节点;out.阶段=阶段::已建立D;out.原因="已建立待完善被认知需求，未入自我需求树";
                if(work.观察.阶段==阶段::已建立D&&known.状态==新需求操作状态::已找到) {
                    进入步骤("正式读回同源被认知需求");
                    const auto verified=d.需求.查询接收认知来源需求(source,d.语言,d.特征);
                    if(verified.状态==新需求操作状态::资源失败)资源();if(verified.状态==新需求操作状态::前提已变化)漂移();
                    if(verified.状态!=新需求操作状态::已找到||!verified.信息||verified.信息->节点!=*out.D
                        ||verified.信息->首次接收认知来源!=std::optional{source}
                        ||verified.信息->已知目标!=std::optional{target})
                        throw std::logic_error("same-source demand final readback missing");
                    if(out.受请求者) {
                        进入步骤("复核正式行动请求与需求来源");
                        const auto fresh=d.语言.读取采用行动请求保护(*out.U,*out.受请求者,d.特征,d.存在);
                        if(fresh.状态==新语言含义采用状态::资源失败)资源();
                        if(fresh.状态==新语言含义采用状态::前提已变化)漂移();
                        if(fresh.状态!=新语言含义采用状态::已采用||!fresh.信息
                            ||fresh.信息->需求来源!=source.键||fresh.信息->请求来源!=k
                            ||fresh.信息->说话者H!=*context.已绑定人类)
                            throw std::logic_error("request and child demand final source inconsistent");
                        const auto qualification=d.需求.读取被认知需求完整资格保护(*out.D,*context.已绑定人类);
                        if(qualification.状态==新被认知需求资格状态::资源失败)资源();
                        if(qualification.状态==新被认知需求资格状态::前提已变化)漂移();
                        out.待交接请求=新行动请求承接来源{source,*out.D,*context.已绑定人类,*out.受请求者};
                        if(qualification.状态==新被认知需求资格状态::内容待完善) {
                            const auto window=d.外设.读取对话框来源(m.窗口节点);
                            if(!window||window->公开宿主会话!=m.来源.公开宿主会话||window->对话框标识!=m.来源.对话框标识
                                ||window->定位.场景节点!=m.场景节点||!window->定位.输出队列特征)漂移();
                            新需求字段追问来源 pending;pending.请求=*out.待交接请求;
                            pending.窗口=m.窗口节点;pending.外设场景=m.场景节点;pending.输出特征=*window->定位.输出队列特征;
                            pending.公开宿主会话=m.来源.公开宿主会话;pending.对话框标识=m.来源.对话框标识;
                            out.待完善交接=std::move(pending);out.阶段=阶段::已读回待完善请求;
                            out.原因="同源请求为待完善材料；确认完整获取信息目标后由通用筹办提出补问子需求";
                        } else {
                            if(qualification.状态!=新被认知需求资格状态::已读取||!qualification.信息)
                                throw std::logic_error("request child demand complete qualification unavailable");
                            out.阶段=阶段::已读回行动请求;out.原因="明确行动请求及同源需求子内容正式读回；未承接";
                        }
                    } else {out.阶段=阶段::已读回;out.原因="同源语言采用与被认知需求正式读回；未自动承接";}
                    本次终结=true;
                }
            }
        } else if(adopted.状态==新语言含义采用状态::未找到){
            进入步骤("读取语言解释资源");
            const auto resources=读资源(d,context.语言,stop);
            进入步骤("读取现实对象范围");
            const auto scope=读范围(d,context.现实根,stop);
            接收专名读取器 names(d,context.现实根,stop);
            语言基础统一输入请求 req{context.语言,m.UTF8正文,{},0,0,0,0,0,&names,语言解释读取模式::本次正式有限集完整读取};
            if(context.已绑定自我)
                req.交互上下文=语言图交互上下文{m.消息节点,*context.已绑定人类,*context.已绑定自我,context.现实根};
            进入步骤("纯统一解释");
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
            const auto 解释诊断号 = 接收解释诊断序号.fetch_add(1, std::memory_order_relaxed) + 1;
            const auto 解释诊断开始 = std::chrono::steady_clock::now();
            记录接收解释调用(解释诊断号, m.消息节点, 解释诊断开始, nullptr);
#endif
            auto parsed=只读解释单句基础语言(req,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
            记录接收解释调用(解释诊断号, m.消息节点, 解释诊断开始, &parsed);
#endif
            if(parsed.状态==语言场景基础输入状态::读取漂移)漂移();
            if(parsed.状态==语言场景基础输入状态::工程未完成||parsed.状态==语言场景基础输入状态::额度不足)资源();
            if(parsed.状态==语言场景基础输入状态::内部不一致)throw std::logic_error("pure language explanation reported internal inconsistency");
            if(parsed.状态!=语言场景基础输入状态::已形成值式图||!parsed.含义
                ||!std::holds_alternative<语言图含义材料>(*parsed.含义)
                ||(parsed.构式!=语言基础统一构式::需求陈述&&parsed.构式!=语言基础统一构式::行动请求))
                throw 推进中断{parsed.状态==语言场景基础输入状态::对象待绑定 ? 阶段::对象未决 : 阶段::非支持输入,"本条消息未形成唯一受支持的需求或行动请求"};
            const auto& graph=std::get<语言图含义材料>(*parsed.含义);
            const auto roles=读取认知图角色(graph);
            if(roles.主体!=*context.已绑定人类)
                throw 推进中断{阶段::非本次人类需求,"需求主体不是本次正式会话人类"};
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
            接收采用读取器 reader(d,context,m,resources,scope,parsed,names.项目,stop,contextStill);
            进入步骤("发表同源语言采用");
            const auto u=d.语言.建立或取得接收采用含义({k,*out.S,*context.已绑定人类,context.现实根,m.正文材料节点,graph,parsed.图角色原文定位组},d.特征,d.特征概念,reader);
            if(u.状态==新语言含义采用状态::资源失败)资源();if(u.状态==新语言含义采用状态::前提已变化)漂移();
            if(u.状态==新语言含义采用状态::输入拒绝)
                throw 推进中断{阶段::非支持输入,"语言角色的正式特征引用材料尚不受支持，保留原消息及来源场景"};
            if((u.状态!=新语言含义采用状态::已采用&&u.状态!=新语言含义采用状态::精确重复)||!u.信息)
                throw std::logic_error("same-source meaning adoption failed");
            out.U=u.信息->节点;out.阶段=阶段::已采用U;
            out.受请求者=roles.受请求者;out.需求来源.reset();
            out.原因=roles.受请求者 ? "已采用明确请求及本句需求内容，尚未建立需求D" : "已采用真实消息的唯一需求含义，尚未建立需求D";
        } else throw std::logic_error("same-source meaning lookup failed");
    }catch(const 推进中断& e){out.阶段=e.状态;out.原因=std::string(步骤)+"："+e.原因;本次终结=e.状态!=阶段::资源延后&&e.状态!=阶段::前提变化&&e.状态!=阶段::停止未完成;}
    catch(const std::bad_alloc&){out.阶段=阶段::资源延后;out.待交接请求.reset();本次终结=false;out.原因="认知推进分配资源未完成";}
    catch(const std::length_error&){out.阶段=阶段::资源延后;out.待交接请求.reset();本次终结=false;out.原因="认知推进材料资源未完成";}
    work.观察=out;work.终结=本次终结;work.下次=std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
    保存观察(out);return out;
    }catch(const std::bad_alloc&){
        接收需求认知观察 out{本次消息.value_or(稳定编码{}),{},{},{},{},阶段::资源延后,"认知调度分配资源未完成"};
        保存观察(out);return out;
    }catch(const std::length_error&){
        接收需求认知观察 out{本次消息.value_or(稳定编码{}),{},{},{},{},阶段::资源延后,"认知调度材料资源未完成"};
        保存观察(out);return out;
    }
}

std::optional<新需求字段追问来源> 接收需求认知服务::读取字段追问来源(
    稳定编码 D,稳定编码 self,稳定编码 selfScene,稳定编码 parent) const {
    auto& impl=*实现_;auto& d=impl.依赖;
    const auto demand=d.需求.读取需求治理信息(D);
    if(!demand||demand->执行域!=新需求执行域::被认知||!demand->首次接收认知来源)return {};
    const auto& origin=*demand->首次接收认知来源;
    const auto records=d.外设.完整读取仍保留的接收记录();
    if(records.状态==新外设事实状态::资源失败)return {};
    if(records.状态!=新外设事实状态::已完成)throw std::logic_error("field question original receive records unavailable");
    const auto message=std::find_if(records.消息组.begin(),records.消息组.end(),[&](const auto& m){return m.消息节点==origin.键.接收消息节点;});
    if(message==records.消息组.end())return {}; // 原M未保留，不替换成最近窗口。
    {
        std::lock_guard lock(impl.锁);const auto session=impl.会话组.find(message->来源.公开宿主会话);
        if(session==impl.会话组.end()||session->second.封存
            ||session->second.语境.已绑定人类!=std::optional{demand->需求主体存在节点}
            ||session->second.语境.已绑定自我!=std::optional{self})return {};
    }
    const auto request=d.语言.读取采用行动请求保护(origin.采用含义节点,self,d.特征,d.存在);
    if((request.状态!=新语言含义采用状态::已采用&&request.状态!=新语言含义采用状态::精确重复)||!request.信息)return {};
    if(request.信息->需求来源!=origin.键||request.信息->说话者H!=demand->需求主体存在节点||request.信息->受请求者E!=self)return {};
    const auto window=d.外设.读取对话框来源(message->窗口节点);
    if(!window||window->公开宿主会话!=message->来源.公开宿主会话||window->对话框标识!=message->来源.对话框标识
        ||window->定位.场景节点!=message->场景节点||!window->定位.输出队列特征)return {};
    新需求字段追问来源 result;
    result.请求={origin,D,demand->需求主体存在节点,self};result.自我场景=selfScene;result.父需求=parent;
    result.窗口=message->窗口节点;result.外设场景=message->场景节点;result.输出特征=*window->定位.输出队列特征;
    result.公开宿主会话=message->来源.公开宿主会话;result.对话框标识=message->来源.对话框标识;
    if(d.需求.复核字段追问来源(result)!=新需求操作状态::已找到)return {};
    return result;
}

void 接收需求认知服务::装配任务读取接口(新_任务类& tasks) {
    std::lock_guard lock(实现_->锁);
    if(实现_->任务&&实现_->任务!=&tasks)throw std::logic_error("answer task reader owner differs");
    实现_->任务=&tasks;
}
void 接收需求认知服务::装配字段答案执行(新_方法类& methods,新_动态类& dynamics,稳定编码 common) {
    if(!有效(common))throw std::logic_error("answer builtin general concept unavailable");
    新方法知识建立请求 q;q.作用域=新方法作用域::普通世界;q.实现方式=新方法实现方式::本能;q.行为=新方法行为::读取;
    q.方法头.作用存在概念组={common};q.方法头.可能结果概念组={common};
    for(std::uint32_t i=1;i<=3;++i){新方法参数规格 p;p.有序规格={i,新方法参数信息类型::存在,common};q.参数规格组.push_back(std::move(p));}
    const auto registered=methods.确保本能方法登记(std::string(新本能函数登记名_写回服务对象需求字段),q);
    if(!registered.方法节点)throw std::logic_error("answer builtin independent method registration failed");
    std::lock_guard lock(实现_->锁);
    if((实现_->方法&&实现_->方法!=&methods)||(实现_->动态&&实现_->动态!=&dynamics))throw std::logic_error("answer builtin runtime owner differs");
    实现_->方法=&methods;实现_->动态=&dynamics;实现_->答案方法=*registered.方法节点;
}
std::optional<新需求字段答案写回请求> 接收需求认知服务::读取字段答案候选(const 新任务字段追问记录& question) const {
    auto& s=*实现_;auto& d=s.依赖;新_任务类* tasks;接收需求认知会话语境 context;
    {std::lock_guard lock(s.锁);tasks=s.任务;const auto session=s.会话组.find(question.来源.公开宿主会话);
        if(!tasks||session==s.会话组.end()||session->second.封存)return {};context=session->second.语境;}
    if(question.阶段!=新任务字段追问阶段::已发送待字段满足||!question.来源已具备||!question.候选||!question.已提交输出
        ||question.候选->表达目标.被询内容.正式内容路径.size()!=1
        ||context.已绑定人类!=std::optional{question.来源.请求.来源人类节点}
        ||context.已绑定自我!=std::optional{question.来源.请求.受请求者节点})return {};
    std::size_t pending=0;
    for(const auto T:tasks->查询字段追问任务()){const auto r=tasks->读取字段追问记录(T);
        if(r&&r->阶段==新任务字段追问阶段::已发送待字段满足&&r->来源.公开宿主会话==question.来源.公开宿主会话
            &&r->来源.对话框标识==question.来源.对话框标识)++pending;}
    if(pending!=1)return {}; // 多个未答字段需要显式回指，不按时间或遍历顺序绑定。
    const auto current=tasks->读取字段追问记录(question.任务节点);
    if(!current||current->阶段!=question.阶段||current->来源!=question.来源||current->候选!=question.候选||current->已提交输出!=question.已提交输出)return {};
    const auto original=d.外设.读取消息保护(question.来源.请求.需求来源.键.接收消息节点);
    const auto outgoing=d.外设.读取消息保护(question.已提交输出->消息节点);
    const auto all=d.外设.完整读取仍保留的接收记录();
    if(original.状态!=新外设事实状态::已完成||!original.消息||outgoing.状态!=新外设事实状态::已完成||!outgoing.消息
        ||all.状态!=新外设事实状态::已完成)return {};
    const auto old=d.需求.读取需求治理信息(question.来源.请求.被认知需求节点);
    if(!old||!old->已知目标)return {};
    const auto field=question.候选->表达目标.被询内容.正式内容路径.front();
    const auto EType=d.特征概念.读取先天特征类型槽(先天特征类型槽::需求目标存在);
    const auto FTType=d.特征概念.读取先天特征类型槽(先天特征类型槽::需求目标FT);
    const auto FormType=d.特征概念.读取先天特征类型槽(先天特征类型槽::需求目标形式);
    const auto DomainType=d.特征概念.读取先天特征类型槽(先天特征类型槽::目标域);
    const auto ComparisonType=d.特征概念.读取先天特征类型槽(先天特征类型槽::目标比较);
    const auto DirectionType=d.特征概念.读取先天特征类型槽(先天特征类型槽::目标方向);
    if(!EType.类型根||!FTType.类型根||!FormType.类型根||!DomainType.类型根||!ComparisonType.类型根||!DirectionType.类型根)return {};
    if(field!=*EType.类型根&&field!=*FTType.类型根&&field!=*FormType.类型根&&field!=*DomainType.类型根&&field!=*ComparisonType.类型根&&field!=*DirectionType.类型根)return {};
    std::optional<新需求字段答案写回请求> answer;
    for(const auto& m:all.消息组) {
        if(d.需求.接收答案消息已采用(m.消息节点))continue;
        if(m.来源.方向!=新外设消息方向::接收||m.窗口节点!=question.来源.窗口||m.场景节点!=question.来源.外设场景
            ||m.来源.公开宿主会话!=question.来源.公开宿主会话||m.来源.对话框标识!=question.来源.对话框标识
            ||m.来源.来源序号<=original.消息->来源.来源序号)continue;
        auto text=m.UTF8正文;
        while(!text.empty()&&(text.back()==' '||text.back()=='\r'||text.back()=='\n'||text.back()=='\t'))text.pop_back();
        for(const std::string_view terminal:{std::string_view{"。"},std::string_view{"."}})if(text.ends_with(terminal))text.resize(text.size()-terminal.size());
        std::optional<std::int64_t> enumValue;
        if(field==*FormType.类型根){if(text=="值")enumValue=static_cast<std::int64_t>(新需求目标形式::值);else if(text=="变化方向")enumValue=static_cast<std::int64_t>(新需求目标形式::变化方向);else continue;}
        if(field==*DirectionType.类型根){if(text=="增加")enumValue=static_cast<std::int64_t>(新需求变化方向::增加);else if(text=="减少")enumValue=static_cast<std::int64_t>(新需求变化方向::减少);else continue;}
        std::optional<新词条信息> answerWord;概念名称候选保护结果 names;
        if(!enumValue) {
            const auto word=d.词.按规范化词面查询(context.语言,严格UTF8字节精确规范化合同节点,text);
            if(word.状态==新词条查询状态::未找到)continue;
            if(word.状态==新词条查询状态::资源失败)return {};
            if(word.状态!=新词条查询状态::已找到||!word.信息)throw std::logic_error("answer word lookup failed");
            answerWord=*word.信息;names=d.名称.读取词条名称候选保护(word.信息->节点);
            if(names.状态==概念名称关系查询状态::未找到)continue;
            if(names.状态!=概念名称关系查询状态::已找到)return {};
        }
        std::vector<std::pair<稳定编码,std::vector<基础联合节点快照>>> values;
        for(const auto& named:names.候选组) {
            if(field==*EType.类型根) {
                if(const auto* pronoun=std::get_if<指代名称关系信息>(&named)) {
                    std::optional<稳定编码> E;
                    if(pronoun->概念.定义.规则==指代声明规则::作用域说话者)E=context.已绑定人类;
                    else if(pronoun->概念.定义.规则==指代声明规则::作用域受话者)E=context.已绑定自我;
                    if(!E)continue;const auto id=d.存在.读取存在身份保护(*E);
                    if(id.状态==基础结构操作状态::已完成&&!id.前提已变化&&id.信息)values.push_back({*E,id.预期节点组});
                } else {
                    const auto C=std::get<概念名称关系信息>(named).概念节点;
                    接收专名读取器 proper(d,context.现实根,{});const auto id=proper.读取实例专名(C,{0,标量数(text)});
                    if(id.状态==语言图专名绑定状态::已绑定&&id.存在节点)values.push_back({*id.存在节点,id.完整保护});
                }
            } else if(const auto* name=std::get_if<概念名称关系信息>(&named)) {
                if(field==*ComparisonType.类型根) {
                    const auto comparison=d.二次概念.读取二次特征定义保护(name->概念节点);
                    if(comparison.状态==基础结构操作状态::已完成&&!comparison.前提已变化&&comparison.信息&&old->已知目标->目标特征类型概念节点
                        &&std::any_of(comparison.信息->输入定义组.begin(),comparison.信息->输入定义组.end(),[&](const auto& input){return input.特征类型概念节点==*old->已知目标->目标特征类型概念节点;}))
                        values.push_back({name->概念节点,comparison.预期节点组});
                } else {
                    const auto ft=d.特征概念.读取特征概念定义保护(name->概念节点);
                    if(ft.状态==基础结构操作状态::已完成&&!ft.前提已变化&&ft.信息
                        &&(field==*FTType.类型根 ? ft.信息->类型根==name->概念节点
                            : field==*DomainType.类型根&&old->已知目标->目标特征类型概念节点==std::optional{ft.信息->类型根}))
                        values.push_back({name->概念节点,ft.预期节点组});
                }
            }
        }
        if(!enumValue&&values.empty())continue;
        if(!values.empty()&&std::any_of(values.begin()+1,values.end(),[&](const auto& v){return v.first!=values.front().first;}))return {};
        if(answer)return {}; // 两条可能回答不任选其一。
        const auto message=d.外设.读取消息保护(m.消息节点);if(message.状态!=新外设事实状态::已完成||!message.消息||!同消息(*message.消息,m))return {};
        新需求字段答案写回请求 q;q.来源=question.来源;q.字段任务=question.任务节点;q.答案消息=m.消息节点;q.字段类型=field;q.预期信息=*old;q.已知目标=*old->已知目标;
        if(field==*FormType.类型根)q.已知目标.目标形式=static_cast<新需求目标形式>(*enumValue);
        else if(field==*DirectionType.类型根)q.已知目标.目标变化方向=static_cast<新需求变化方向>(*enumValue);
        else if(field==*EType.类型根)q.已知目标.目标存在节点=values.front().first;
        else if(field==*FTType.类型根)q.已知目标.目标特征类型概念节点=values.front().first;
        else if(field==*DomainType.类型根)q.已知目标.目标值域概念节点=values.front().first;
        else q.已知目标.目标比较关系概念节点=values.front().first;
        q.答案保护=message.完整保护;并保护(q.答案保护,original.完整保护);并保护(q.答案保护,outgoing.完整保护);并保护(q.答案保护,names.完整保护);
        for(const auto& value:values)并保护(q.答案保护,value.second);
        std::vector<稳定编码> nodes{question.节点,question.任务节点};if(answerWord)nodes.push_back(answerWord->节点);
        const auto taskProof=全局基础数据集.查询节点组完整结构(nodes);
        if(taskProof.状态!=基础结构操作状态::已完成)return {};
        for(const auto& item:taskProof.节点组){if(!item.快照)return {};并保护(q.答案保护,{*item.快照});}
        answer=std::move(q);
    }
    return answer;
}
新本能函数本次条件结果对 接收需求认知服务::执行字段答案写回本能(
    const 新需求字段答案写回请求& request,新_需求类& owner,const 方法参数场景& originalScene) const {
    (void)originalScene;auto& s=*实现_;新_任务类* tasks;新_方法类* methods;新_动态类* dynamics;稳定编码 method;
    {std::lock_guard lock(s.锁);tasks=s.任务;methods=s.方法;dynamics=s.动态;method=s.答案方法;}
    if(!tasks||!methods||!dynamics||!有效(method)||&owner!=&s.依赖.需求)throw std::logic_error("answer builtin runtime is not assembled");
    std::vector<新方法参数角色绑定> bindings;
    const std::array ids{request.来源.请求.受请求者节点,request.来源.窗口,request.来源.外设场景};
    for(std::uint32_t i=0;i<ids.size();++i){新方法参数角色绑定 b;b.有序引用=新方法有序参数引用{i+1,新方法参数信息类型::存在,ids[i]};bindings.push_back(std::move(b));}
    const auto scene=methods->建立参数场景(method,bindings);if(!scene.参数场景){新本能函数本次条件结果对 r;r.结果.状态=新本能函数执行状态::参数不匹配;return r;}
    return 调用需求字段答案写回本能(*scene.参数场景,[&](const 方法参数场景& actual){
        新本能函数本次条件结果对 r;r.结果.状态=新本能函数执行状态::入口拒绝;
        const auto record=tasks->读取字段追问记录(request.字段任务);if(!record)return r;
        auto fresh=读取字段答案候选(*record);
        const auto same=[&](const auto& x){return x.答案消息==request.答案消息&&x.字段类型==request.字段类型&&x.来源==request.来源&&x.预期信息==request.预期信息&&x.已知目标==request.已知目标;};
        if(!fresh||!same(*fresh))return r;
        基础节点组追加请求 proof;proof.预期联合节点组=fresh->答案保护;
        const auto verified=全局基础数据集.原子追加节点组(proof);if(verified.状态==基础节点组追加状态::预期不符)return r;
        if(verified.状态!=基础节点组追加状态::已完成)throw std::logic_error("answer builtin precondition proof failed");
        const auto call=methods->建立直接调用身份(actual.方法节点(),actual.全部角色绑定(),request.来源.自我场景,request.来源.外设场景);
        const auto time=UTC时钟适配器{}.读取当前UTC时间证据_v1({});if(!time.成功())throw std::logic_error("answer observation UTC unavailable");
        const auto action=dynamics->建立动作动态({request.来源.请求.受请求者节点,actual.方法节点(),call.节点,request.来源.外设场景,{},
            {新状态时间语义::实例绝对UTC,time.证据->UTC纳秒}});
        if(action.状态!=新动态操作状态::已完成||!action.动态节点)throw std::logic_error("answer observation action publication failed");
        methods->绑定直接调用动作(call.节点,*action.动态节点);
        r.结果.动作动态节点=*action.动态节点;r.结果.动作阶段=新本能函数动作阶段::已进入动作;
        fresh=读取字段答案候选(*record);
        if(!fresh||!same(*fresh)){methods->收束直接调用身份(call.节点);return r;}
        fresh->动作节点=*action.动态节点;
        const auto written=owner.写回接收认知需求字段(*fresh);
        methods->收束直接调用身份(call.节点);
        if(written.状态!=新需求操作状态::已修改&&written.状态!=新需求操作状态::无变化)return r;
        if(!written.信息)throw std::logic_error("answer field owner returned no formal readback");
        r.结果.状态=新本能函数执行状态::已按合同返回;r.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;r.结果.结果取得状态=新本能函数结果取得状态::已取得;return r;
    });
}

} // namespace 海中鱼巣
