#include "表达.组织.h"
#include "表达.基础语言.h"
#include <algorithm>
#include "../领域/概念_存在类.h"
#include "../领域/概念_特征类.h"

namespace 海中鱼巣 {
namespace {
// 招呼（CONV-EXPR-TEXT §3.5）：无条件、无取材；模板顶层招呼单元的种类与请求种类相同即成立。
// 同一种类多条成立返回多规则待选：选用哪种说法属于自我意识，本函数不选。
新表达结构组织结果 组织招呼(const 正式表达需求& 请求,const 正式表达招呼& g,const 新_语言类& 语言) {
    using S=新表达结构组织状态;
    const auto 失败=[](S s){新表达结构组织结果 r;r.状态=s;return r;};
    if(请求.表达用途!=新表达用途::招呼)return 失败(S::范围未支持);
    const auto 候选=语言.查询适用表达组织知识({新表达用途::招呼,static_cast<std::int64_t>(请求.表达对象.index())});
    std::vector<const 新表达组织知识信息*> 成立;
    for(const auto& k:候选.候选组){
        const auto& 网=k.定义.含义网模板;
        if(网.顶层项组.size()!=1)throw std::logic_error("招呼组织知识模板顶层不唯一");
        const auto 根=std::find_if(网.内容项组.begin(),网.内容项组.end(),[&](const auto& i){return i.编号==网.顶层项组.front().内容引用;});
        if(根==网.内容项组.end())throw std::logic_error("招呼组织知识模板根项缺失");
        const auto* u=std::get_if<新招呼含义单元>(&根->内容);
        if(!u)throw std::logic_error("招呼组织知识模板根项不是招呼单元");
        if(u->种类==g.种类)成立.push_back(&k);
    }
    if(成立.size()>1)return 失败(S::多规则待选);
    if(成立.empty())return 失败(S::缺组织知识);
    新表达结构组织结果 out;
    out.含义网=成立.front()->定义.含义网模板;
    核验含义网结构(out.含义网);
    out.完整保护=候选.完整保护;
    std::vector<稳定编码> ids;for(const auto& v:out.完整保护)ids.push_back(v.结构.节点.编码);
    if(!ids.empty()){
        const auto fresh=全局基础数据集.查询节点组完整结构(ids);
        if(fresh.状态!=基础结构操作状态::已完成)throw std::logic_error("招呼组织材料完整复读工程失败");
        for(std::size_t i=0;i<fresh.节点组.size();++i){
            const auto& v=fresh.节点组[i];
            const auto o=std::find_if(out.完整保护.begin(),out.完整保护.end(),[&](const auto& x){return x.结构.节点.编码==v.查询节点;});
            if(!v.快照||o==out.完整保护.end()||!(v.快照->结构.节点==o->结构.节点&&v.快照->结构.字段组==o->结构.字段组
                &&v.快照->入向字段组==o->入向字段组&&v.快照->入向外部关系组==o->入向外部关系组&&v.快照->出向外部关系组==o->出向外部关系组))
                return 失败(S::前提已变化);
        }
    }
    out.状态=S::已组织;out.所用组织知识节点=成立.front()->节点;return out;
}
} // namespace

新表达结构组织结果 组织具体表达结构(const 正式表达需求& 请求,const 新_语言类& 语言,
    const 正式信息参数读取接口& 读取,const 概念_存在类& 存在概念,const 概念_特征类& 特征概念) {
    using S=新表达结构组织状态;新表达结构组织结果 out;
    const auto 失败=[&](S s){新表达结构组织结果 r;r.状态=s;return r;};
    if(const auto* g=std::get_if<正式表达招呼>(&请求.表达对象))return 组织招呼(请求,*g,语言);
    // 载荷栏问题与目标栏问题（R120）共用需求D、字段与范围；目标栏问题另带问题主体（D的需求主体）。
    std::optional<正式表达需求字段> 字段对象;std::optional<稳定编码> 问题主体;
    if(const auto* p=std::get_if<正式表达需求字段>(&请求.表达对象))字段对象=*p;
    else if(const auto* t=std::get_if<正式表达需求目标栏>(&请求.表达对象)){
        字段对象=正式表达需求字段{t->需求D,t->字段特征类型,t->需求范围概念};
        if(!有效(t->问题主体))return 失败(S::缺材);问题主体=t->问题主体;
    }
    if(!字段对象)return 失败(S::范围未支持);
    const auto* d=&*字段对象;
    if(!有效(d->需求D)||!有效(d->字段特征类型)||!有效(d->需求范围概念))return 失败(S::缺材);
    const auto 候选=语言.查询适用表达组织知识({请求.表达用途,static_cast<std::int64_t>(请求.表达对象.index())});
    std::map<稳定编码,基础联合节点快照> 保护;
    const auto 同=[&](const auto& a,const auto& b){return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组
        &&a.入向字段组==b.入向字段组&&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;};
    const auto 并保护=[&](const auto& 组){for(const auto& s:组){const auto [i,added]=保护.emplace(s.结构.节点.编码,s);if(!added&&!同(i->second,s))return false;}return true;};
    if(!并保护(候选.完整保护))return 失败(S::前提已变化);
    std::optional<正式信息参数读取结果> 材料;
    const auto 取需求=[&]()->S {
        if(材料)return S::已组织;
        材料=读取.读取正式信息参数保护(d->需求D,d->需求范围概念);
        if(材料->状态==正式信息参数读取状态::前提已变化)return S::前提已变化;
        if(材料->状态==正式信息参数读取状态::范围未支持)return S::范围未支持;
        if(材料->状态!=正式信息参数读取状态::已读取||!材料->信息||材料->完整保护组.empty())
            throw std::logic_error("表达组织正式需求保护读取未完成");
        return 并保护(材料->完整保护组) ? S::已组织 : S::前提已变化;
    };
    std::vector<const 新表达组织知识信息*> 成立;
    bool 未知=false,不成立=false;
    for(const auto& k:候选.候选组){
        bool 条件成立=true;
        // 完整求值每项，不用AND短路掩盖工程故障或未覆盖范围。
        for(auto 条件:k.定义.适用条件组){
            if(条件!=新组织取材算子::需求字段内容状态)throw std::logic_error("未登记组织条件算子");
            const auto s=取需求();
            if(s==S::前提已变化)return 失败(s);
            if(s==S::范围未支持){未知=true;条件成立=false;continue;}
            const auto& 组=材料->信息->直接内容组;
            const auto i=std::find_if(组.begin(),组.end(),[&](const auto& x){return x.特征类型==d->字段特征类型;});
            if(i==组.end()){未知=true;条件成立=false;continue;}
            switch(i->内容.状态){
            case 正式内容值状态::当前未知:break;
            case 正式内容值状态::已知:
            case 正式内容值状态::不适用:条件成立=false;不成立=true;break;
            default:throw std::logic_error("组织条件字段状态未定义");
            }
        }
        if(条件成立)成立.push_back(&k);
    }
    if(未知)return 失败(S::范围未支持);
    if(成立.size()>1)return 失败(S::多规则待选);
    if(成立.empty())return 失败(不成立 ? S::条件不成立 : S::缺组织知识);
    const auto& k=*成立.front();
    std::map<新组织绑定角色,新绑定指称> 角色{
        {新组织绑定角色::说话者,请求.说话者},{新组织绑定角色::受话者,请求.受话者}};
    std::map<std::int64_t,新绑定指称> 步骤值;
    for(const auto& t:k.定义.取材步骤组){
        新绑定指称 v;
        if(t.算子==新组织取材算子::绑定角色){
            switch(t.输出角色){
            case 新组织绑定角色::需求D:v={新指称类别::需求,d->需求D,d->需求范围概念};break;
            case 新组织绑定角色::字段特征类型:v={新指称类别::特征类型,d->字段特征类型,d->字段特征类型};break;
            case 新组织绑定角色::说话者:v=请求.说话者;break;
            case 新组织绑定角色::受话者:v=请求.受话者;break;
            case 新组织绑定角色::问题主体:
                if(!问题主体)return 失败(S::范围未支持);
                v={新指称类别::存在,*问题主体,{}};break;
            default:throw std::logic_error("绑定角色算子签名不合法");
            }
        } else if(t.算子==新组织取材算子::存在根概念){
            const auto r=存在概念.读取存在根概念保护();
            if(r.状态==正式概念读取状态::前提已变化)return 失败(S::前提已变化);
            if(r.状态!=正式概念读取状态::已读取||!r.信息||r.完整保护组.empty())return 失败(S::范围未支持);
            if(!并保护(r.完整保护组))return 失败(S::前提已变化);
            v={新指称类别::概念,r.信息->节点,r.信息->节点};
        } else if(t.算子==新组织取材算子::需求字段值域概念){
            const auto s=取需求();if(s!=S::已组织)return 失败(s);
            const auto r=读取正式字段询问范围(*材料,d->字段特征类型,存在概念,特征概念);
            if(r.状态==结构项概念保护状态::前提已变化)return 失败(S::前提已变化);
            if(r.状态==结构项概念保护状态::未取得正式概念证明||r.状态==结构项概念保护状态::输入拒绝)return 失败(S::范围未支持);
            if(r.状态!=结构项概念保护状态::已读取||!r.内容概念||r.完整保护组.empty())throw std::logic_error("组织取材值域概念读取未完成");
            if(!并保护(r.完整保护组))return 失败(S::前提已变化);
            v={新指称类别::概念,*r.内容概念,*r.内容概念};
        } else throw std::logic_error("组织取材算子未登记");
        if(!有效(v.节点))return 失败(S::缺材);
        角色[t.输出角色]=v;步骤值.emplace(t.编号,v);out.取材材料组.push_back({t.编号,t.输出角色,v});
    }
    out.含义网=k.定义.含义网模板;
    const auto 绑定=[&](新表达指称& v)->bool {
        if(const auto* r=std::get_if<新规则角色变量>(&v)){
            const auto i=角色.find(r->角色);if(i==角色.end()||!有效(i->second.节点))return false;v=i->second;
        } else if(auto* q=std::get_if<新疑问代词实例>(&v)){
            if(!q->类型取材步骤)throw std::logic_error("组织模板疑问类型没有取材引用");
            const auto i=步骤值.find(*q->类型取材步骤);if(i==步骤值.end())throw std::logic_error("组织模板引用未定义取材步骤");
            q->待求类型概念=i->second.节点;q->类型取材步骤.reset();
        }
        return true;
    };
    bool 完备=true;
    const auto 绑值=[&](新表达特征值& v){if(auto* p=std::get_if<新表达指称>(&v))if(!绑定(*p))完备=false;};
    for(auto& i:out.含义网.内容项组){
        for(auto& r:i.角色映射)if(!绑定(r.指称))完备=false;
        std::visit([&](auto& x){using T=std::decay_t<decltype(x)>;
            if constexpr(std::is_same_v<T,新静态含义单元>){
                if(!绑定(x.主体))完备=false;
                if(x.时间&&!绑定(*x.时间))完备=false;if(x.范围&&!绑定(*x.范围))完备=false;
                std::visit([&](auto& p){using P=std::decay_t<decltype(p)>;
                    if constexpr(std::is_same_v<P,新特征值谓项>){if(!绑定(p.特征类型))完备=false;绑值(p.值);}
                    else if constexpr(std::is_same_v<P,新概念归属谓项>){if(!绑定(p.概念))完备=false;}
                    else if(!绑定(p.另一指称))完备=false;
                },x.谓项);
            } else if constexpr(std::is_same_v<T,新内容算子>){for(auto& r:x.角色组)if(!绑定(r.指称))完备=false;}
            else if constexpr(std::is_same_v<T,新事件含义单元>){
                if(x.动作发生){if(x.动作发生->主体&&!绑定(*x.动作发生->主体))完备=false;for(auto& r:x.动作发生->对象角色组)if(!绑定(r.指称))完备=false;}
                if(x.状态变化){auto& s=*x.状态变化;if(!绑定(s.对象))完备=false;if(s.前值)绑值(*s.前值);if(s.后值或方向)绑值(*s.后值或方向);if(s.时间&&!绑定(*s.时间))完备=false;}
            }
        },i.内容);
    }
    if(!完备)return 失败(S::缺材);
    核验含义网结构(out.含义网);
    std::vector<稳定编码> ids;for(const auto& [id,v]:保护){ids.push_back(id);out.完整保护.push_back(v);}
    const auto fresh=全局基础数据集.查询节点组完整结构(ids);
    if(fresh.状态!=基础结构操作状态::已完成)throw std::logic_error("组织材料完整复读工程失败");
    for(const auto& v:fresh.节点组)if(!v.快照||!同(保护.at(v.查询节点),*v.快照))return 失败(S::前提已变化);
    out.状态=S::已组织;out.所用组织知识节点=k.节点;return out;
}
} // namespace 海中鱼巣
