#include "接收需求认知.服务.h"
#include "新_外设世界树类.h"
#include "转换.语言场景基础.h"
#include "转换.语言场景共同.h"
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
    const auto r=d.特征概念.读取特征概念定义保护(p.目标特征类型);
    if(r.前提已变化)漂移();if(r.状态==基础结构操作状态::资源失败)资源();
    if(r.状态!=基础结构操作状态::已完成||!r.信息)throw std::logic_error("adopted target FT unreadable");
    const auto& def=r.信息->定义;
    if(r.信息->类型根!=p.目标特征类型||def.材料类型!=特征概念材料物理类型::I64标量||(!p.方向&&def.单位!=p.单位))
        throw 推进中断{阶段::D映射受阻,"目标FT类型或单位暂不能精确映射"};
    新部分服务要求目标 out;out.目标存在节点=p.目标存在;out.目标特征类型概念节点=p.目标特征类型;
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
} // namespace

struct 接收需求认知服务::实现 final {
    认知依赖 依赖;
    struct 会话 final {接收需求认知会话语境 语境;bool 封存=false;};
    enum class 前置阶段 {无,待接纳,待筹办,具名等待,待重入,已停用};
    struct 调度 final {
        接收需求认知观察 观察;bool 终结=false;std::chrono::steady_clock::time_point 下次{};bool 行动请求已交接=false;
        前置阶段 前置=前置阶段::无;
        std::optional<新前置语言输入定位> 定位;
        std::optional<新任务前置语言输入结果> 已确认结果;
    };
    mutable std::mutex 锁;
    std::map<std::string,会话,std::less<>> 会话组;
    std::map<稳定编码,调度> 进度;
    std::optional<接收需求认知观察> 最近;
    std::optional<稳定编码> 上次;
    explicit 实现(认知依赖 d):依赖(d){}
    新前置语言输入读取结果 读取前置材料内部(const 新前置语言输入定位&,std::stop_token);
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
    try {std::lock_guard g(实现_->锁);const auto it=实现_->会话组.find(c);if(it!=实现_->会话组.end())it->second.封存=true;
        for(auto& [M,项]:实现_->进度)if(项.定位&&项.定位->公开宿主会话==c) {
            项.前置=实现::前置阶段::已停用;项.观察.待交接前置输入.reset();
        }}
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
        if(c->second.语境.已绑定人类!=std::optional{p.来源人类节点}
            ||c->second.语境.已绑定自我!=std::optional{p.自我存在节点}
            ||c->second.语境.语言!=p.语言节点||c->second.语境.现实根!=p.现实根场景)return S::来源冲突;
        if(w.前置==P::待筹办||w.前置==P::具名等待||w.前置==P::待重入)return S::精确重复;
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
            w.前置=r.材料->可重入原认知 ? P::待重入 : P::具名等待;
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
新前置语言输入读取结果 接收需求认知服务::实现::读取前置材料内部(const 新前置语言输入定位& p,std::stop_token stop) {
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
    q.交互上下文=语言图交互上下文{p.来源.接收消息节点,p.来源人类节点,p.自我存在节点,p.现实根场景};
    const auto parsed=只读解释单句基础语言(q,d.语言,d.词,d.名称,d.存在概念,d.特征概念,d.存在);
    if(parsed.状态==语言场景基础输入状态::读取漂移)漂移();
    if(parsed.状态==语言场景基础输入状态::工程未完成||parsed.状态==语言场景基础输入状态::额度不足)资源();
    if(parsed.状态==语言场景基础输入状态::内部不一致)throw std::logic_error("input material parse corrupt");
    新前置语言输入材料 out{p};
    if(parsed.状态==语言场景基础输入状态::已形成值式图&&parsed.含义&&std::holds_alternative<语言图含义材料>(*parsed.含义)
        &&(parsed.构式==语言基础统一构式::需求陈述||parsed.构式==语言基础统一构式::行动请求)) {
        const auto roles=读取认知图角色(std::get<语言图含义材料>(*parsed.含义));
        out.可重入原认知=roles.主体==p.来源人类节点&&(!roles.受请求者||roles.受请求者==std::optional{p.自我存在节点});
    }
    if(!out.可重入原认知) {
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
    检查认知停止(stop);return {R::已读取,std::move(out)};
}
新前置语言输入读取结果 接收需求认知服务::读取前置语言输入材料(const 新前置语言输入定位& p,std::stop_token stop) const {
    using R=新前置语言读取状态;using P=实现::前置阶段;
    try {
        const auto 核工作=[&]() -> R {
            std::lock_guard g(实现_->锁);const auto c=实现_->会话组.find(p.公开宿主会话);
            if(c==实现_->会话组.end()||c->second.封存)return R::来源停用;
            if(c->second.语境.已绑定人类!=std::optional{p.来源人类节点}
                ||c->second.语境.已绑定自我!=std::optional{p.自我存在节点}
                ||c->second.语境.语言!=p.语言节点||c->second.语境.现实根!=p.现实根场景)return R::输入不适用;
            const auto i=实现_->进度.find(p.来源.接收消息节点);
            if(i==实现_->进度.end()||i->second.定位!=std::optional{p}||i->second.观察.U||i->second.观察.D)return R::输入不适用;
            if(i->second.前置==P::已停用)return R::来源停用;
            return i->second.前置==P::待筹办 ? R::已读取 : R::前提已变化;
        };
        if(stop.stop_requested())return {R::停止取消,{}};
        const auto before=核工作();if(before!=R::已读取)return {before,{}};
        auto out=实现_->读取前置材料内部(p,stop);
        const auto after=核工作();if(after!=R::已读取)return {after,{}};
        return out;
    }catch(const 推进中断& e){return {e.状态==阶段::停止未完成 ? R::停止取消 : e.状态==阶段::资源延后 ? R::资源失败 : R::前提已变化,{}};}
    catch(const std::bad_alloc&){return {R::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取前置输入材料",L"只读材料来源或内部结构不变量异常");}
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
    const auto eligible=[&](const auto& m){std::lock_guard g(impl.锁);const auto it=impl.进度.find(m.消息节点);
        if(it!=impl.进度.end()&&(it->second.前置==实现::前置阶段::待筹办||it->second.前置==实现::前置阶段::具名等待
            ||it->second.前置==实现::前置阶段::已停用))return false;
        return it==impl.进度.end()
        ||((!it->second.终结||(it->second.观察.阶段==阶段::已读回行动请求&&!it->second.行动请求已交接))&&it->second.下次<=now);};
    const auto 会话仍活跃=[&](const auto& m){
        std::lock_guard g(impl.锁);const auto it=impl.会话组.find(m.来源.公开宿主会话);
        return it!=impl.会话组.end()&&!it->second.封存;};
    std::size_t start=0;
    std::optional<稳定编码> 上次;{std::lock_guard g(impl.锁);上次=impl.上次;}
    if(上次){const auto prev=std::find_if(records.消息组.begin(),records.消息组.end(),[&](const auto& m){return m.消息节点==*上次;});
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
            (void)调试日志<调试切片::RECEIVED_DEMAND_SCHED>(L"接收需求认知",L"推进一次",t.str());}catch(...){}
        return {};}
    const auto& m=*chosen;本次消息=m.消息节点;
    实现::调度 work;
    {std::lock_guard g(impl.锁);impl.上次=m.消息节点;work=impl.进度[m.消息节点];}
    const auto 原阶段=work.前置;const auto 原定位=work.定位;const auto 原交接=work.行动请求已交接;
    const auto 提交进度=[&]() {
        std::lock_guard g(impl.锁);auto& actual=impl.进度.at(m.消息节点);
        if(actual.前置!=原阶段||actual.定位!=原定位||actual.行动请求已交接!=原交接)return false;
        const auto c=impl.会话组.find(m.来源.公开宿主会话);
        if(c!=impl.会话组.end()&&c->second.封存&&work.定位){actual.前置=实现::前置阶段::已停用;actual.观察.待交接前置输入.reset();return false;}
        actual=work;return true;
    };
    auto out=work.观察;out.M=m.消息节点;
    if(work.前置==实现::前置阶段::待接纳) {
        out.待交接前置输入=work.定位;work.观察=out;
        work.下次=std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
        if(!提交进度())return {};保存观察(out);return out;
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
    bool 招呼不交接=false; // 招呼只保存为场景信息（7240 §2.1），不交前置输入治理、不形成需求。
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
            if(it==impl.会话组.end()||it->second.封存)throw 推进中断{阶段::非本次人类需求,"消息未接入活动正式会话语境，保留原始接收事实"};context=it->second.语境;}
        out.H=context.已绑定人类;
        if(!context.已绑定人类)throw 推进中断{阶段::非本次人类需求,"匿名会话不推定人类主体"};
        const 新接收认知来源键 请求键{m.消息节点,0,标量数(m.UTF8正文),{}};
        进入步骤("建立或取得正式原文来源");
        const auto pre=d.语言.建立或取得前置来源({请求键},d.外设);
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
        default:来源工程错误("正式保留消息的前置来源返回不符合接线前置的状态");
        }
        if(!pre.信息)来源工程错误("前置来源成功却缺少正式载荷");
        const auto& 固定来源=*pre.信息;
        if(!有效(固定来源.节点)||固定来源.来源!=请求键
            ||固定来源.固定文本材料!=m.正文材料节点||固定来源.固定UTF8原文!=m.UTF8正文)
            来源工程错误("正式前置来源与选定消息不一致");
        const auto& k=固定来源.来源;
        if(!out.U&&context.已绑定自我)
            work.定位=新前置语言输入定位{k,固定来源.节点,固定来源.固定文本材料,context.语言,*context.已绑定人类,*context.已绑定自我,context.现实根,context.公开宿主会话};
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
            if(known.状态==新需求操作状态::目标内容依赖未就绪)throw 推进中断{阶段::资源延后,"需求目标内容依赖未就绪，保留原认知进度等待重试"};
            if(known.状态==新需求操作状态::已找到){
                if(!known.信息||!known.信息->首次接收认知来源)throw std::logic_error("same-source demand read returned no source");
                out.D=known.信息->节点;
            } else if(known.状态!=新需求操作状态::需求不存在)throw std::logic_error("same-source demand lookup failed");
            // CONV-INPUT 最小切片：本消息尚无同源D时，若自我曾就某个待完善被认知需求D向该说话者发出字段询问，
            // 且回复的需求目标与D已知字段相容，则视为对该询问的回应，经需求owner补齐D缺失字段，不新建D（7240 §5.4 回应交被回应项提出方）。
            bool 已回答写回=false;
            if(known.状态==新需求操作状态::需求不存在) {
                进入步骤("核对回应出站字段询问");
                const auto asked=d.语言.查询出站字段询问(*context.已绑定人类);
                // 被询字段FT → 部分目标字段：只在该字段仍未知且回复确实给出它时才算回应。
                std::map<稳定编码,std::uint32_t> 字段槽;
                if(!asked.empty())for(const auto 槽:{先天特征类型槽::需求目标存在,先天特征类型槽::需求目标FT,
                        先天特征类型槽::需求目标形式,先天特征类型槽::目标域,先天特征类型槽::目标比较,先天特征类型槽::目标方向}) {
                    const auto r=d.特征概念.读取先天特征类型槽(槽);
                    if(r.状态==特征概念规则状态::资源失败)资源();if(r.状态==特征概念规则状态::前提已变化)漂移();
                    if(r.状态!=特征概念规则状态::已完成||!r.类型根)throw std::logic_error("demand field innate FT slot unavailable");
                    字段槽.emplace(*r.类型根,槽);
                }
                const auto 有字段=[](const 新部分服务要求目标& t,std::uint32_t 槽)->bool{
                    using namespace 先天特征类型槽;
                    return 槽==需求目标存在 ? t.目标存在节点.has_value() : 槽==需求目标FT ? t.目标特征类型概念节点.has_value()
                        : 槽==需求目标形式 ? t.目标形式.has_value() : 槽==目标域 ? t.目标值域概念节点.has_value()
                        : 槽==目标比较 ? t.目标比较关系概念节点.has_value() : t.目标变化方向.has_value();
                };
                std::optional<新部分服务要求目标> target;
                for(auto it=asked.rbegin();it!=asked.rend()&&!已回答写回;++it) {
                    const auto 槽=字段槽.find(it->字段特征类型);
                    if(槽==字段槽.end())continue;
                    const auto D=d.需求.读取需求治理信息(it->需求D);
                    if(!D||D->执行域!=新需求执行域::被认知||D->需求主体存在节点!=*context.已绑定人类
                        ||D->首次接收认知来源||D->信息完善阶段!=std::optional{新需求信息完善阶段::待完善})continue;
                    auto merged=D->已知目标.value_or(新部分服务要求目标{});
                    if(有字段(merged,槽->second))continue; // 被询字段已知：该询问已不再待回应
                    if(!target)target=映射目标(d,projected,stop);
                    if(!有字段(*target,槽->second))continue;
                    bool conflict=false;
                    const auto fill=[&](auto& dst,const auto& src){if(!src)return;if(dst&&*dst!=*src)conflict=true;else if(!dst)dst=src;};
                    fill(merged.目标存在节点,target->目标存在节点);fill(merged.目标特征类型概念节点,target->目标特征类型概念节点);
                    fill(merged.目标形式,target->目标形式);fill(merged.目标值域概念节点,target->目标值域概念节点);
                    fill(merged.目标比较关系概念节点,target->目标比较关系概念节点);fill(merged.目标变化方向,target->目标变化方向);
                    // 值目标与方向目标互斥：合并后混合即不相容，按原路径处理。
                    const bool 有值=merged.目标值域概念节点||merged.目标比较关系概念节点;
                    if(conflict||(merged.目标变化方向&&有值)
                        ||(merged.目标形式==std::optional{新需求目标形式::值}&&merged.目标变化方向)
                        ||(merged.目标形式==std::optional{新需求目标形式::变化方向}&&有值))continue;
                    if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入回答写回"};
                    进入步骤("经需求owner补齐被询需求字段");
                    const auto upd=d.需求.更新被认知需求信息({D->节点,*D,merged,std::nullopt});
                    if(upd.状态==新需求操作状态::资源失败)资源();if(upd.状态==新需求操作状态::前提已变化)漂移();
                    // 只有实际补齐才消费本消息；owner拒绝的合法不相容目标退回原路径。
                    if(upd.状态!=新需求操作状态::已修改)continue;
                    out.D=D->节点;out.阶段=阶段::已回答写回;
                    out.原因="回应对既有被认知需求的字段询问，已补齐需求字段；未新建需求";
                    本次终结=true;已回答写回=true;
                }
            }
            if(!已回答写回) {
                进入步骤("映射正式需求目标");
                const auto target=映射目标(d,projected,stop);
                if(stop.stop_requested())throw 推进中断{阶段::停止未完成,"停止后不进入需求发表"};
                进入步骤("发表同源被认知需求");
                const auto created=d.需求.建立或取得接收认知需求({source,target},d.语言,d.特征);
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
                        if(qualification.状态==新被认知需求资格状态::内容待完善)
                            throw 推进中断{阶段::D映射受阻,"已保存需求尚不符合完整内容概念，未承接"};
                        if(qualification.状态!=新被认知需求资格状态::已读取||!qualification.信息)
                            throw std::logic_error("request child demand complete qualification unavailable");
                        out.阶段=阶段::已读回行动请求;out.原因="明确行动请求及同源需求子内容正式读回；未承接";
                        out.待交接请求=新行动请求承接来源{source,*out.D,*context.已绑定人类,*out.受请求者};
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
            语言基础统一输入请求 req{context.语言,固定来源.固定UTF8原文,{},0,0,0,0,0,&names,语言解释读取模式::本次正式有限集完整读取};
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
            if(parsed.状态==语言场景基础输入状态::已形成值式图&&parsed.含义
                &&std::holds_alternative<语言图含义材料>(*parsed.含义)&&parsed.构式==语言基础统一构式::招呼) {
                招呼不交接=true;
                throw 推进中断{阶段::非支持输入,"收到招呼：只保存为场景信息，不形成需求"};
            }
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
            const auto u=d.语言.建立或取得接收采用含义({k,*out.S,*context.已绑定人类,context.现实根,固定来源.固定文本材料,graph,parsed.图角色原文定位组},d.特征,d.特征概念,reader);
            if(u.状态==新语言含义采用状态::资源失败)资源();if(u.状态==新语言含义采用状态::前提已变化)漂移();
            if(u.状态==新语言含义采用状态::输入拒绝)
                throw 推进中断{阶段::非支持输入,"语言角色的正式特征引用材料尚不受支持，保留原消息及来源场景"};
            if((u.状态!=新语言含义采用状态::已采用&&u.状态!=新语言含义采用状态::精确重复)||!u.信息)
                throw std::logic_error("same-source meaning adoption failed");
            out.U=u.信息->节点;out.阶段=阶段::已采用U;
            out.受请求者=roles.受请求者;out.需求来源.reset();
            out.原因=roles.受请求者 ? "已采用明确请求及本句需求内容，尚未建立需求D" : "已采用真实消息的唯一需求含义，尚未建立需求D";
        } else throw std::logic_error("same-source meaning lookup failed");
    }catch(const 推进中断& e){out.阶段=e.状态;out.原因=std::string(步骤)+"："+e.原因;本次终结=e.状态!=阶段::资源延后&&e.状态!=阶段::前提变化&&e.状态!=阶段::停止未完成;
        if(!out.U&&!out.D&&work.定位&&!招呼不交接&&(e.状态==阶段::对象未决||e.状态==阶段::非支持输入)) {
            work.前置=实现::前置阶段::待接纳;out.待交接前置输入=work.定位;
            out.阶段=阶段::前置输入待治理;本次终结=false;
        }
        if(e.状态==阶段::停止未完成&&work.定位){work.前置=实现::前置阶段::已停用;out.待交接前置输入.reset();}
    }
    catch(const std::bad_alloc&){out.阶段=阶段::资源延后;out.待交接请求.reset();本次终结=false;out.原因="认知推进分配资源未完成";}
    catch(const std::length_error&){out.阶段=阶段::资源延后;out.待交接请求.reset();本次终结=false;out.原因="认知推进材料资源未完成";}
    work.观察=out;work.终结=本次终结;work.下次=std::chrono::steady_clock::now()+std::chrono::milliseconds(500);
    if(!提交进度())return {};保存观察(out);return out;
    }catch(const std::bad_alloc&){
        接收需求认知观察 out{本次消息.value_or(稳定编码{}),{},{},{},{},阶段::资源延后,"认知调度分配资源未完成"};
        保存观察(out);return out;
    }catch(const std::length_error&){
        接收需求认知观察 out{本次消息.value_or(稳定编码{}),{},{},{},{},阶段::资源延后,"认知调度材料资源未完成"};
        保存观察(out);return out;
    }
}
} // namespace 海中鱼巣
