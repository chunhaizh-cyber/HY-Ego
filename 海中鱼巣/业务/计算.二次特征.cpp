#include "计算.二次特征.h"

namespace 海中鱼巣::二次计算内部 {

节点键 键(const 二次计算节点身份& id) {
    return std::visit([](const auto& x)->节点键{
        if constexpr(std::is_same_v<std::decay_t<decltype(x)>,二次本次节点身份>)return {true,x.局部编号};
        else return {false,x.定义.结点.值};
    },id);
}
二次计算节点身份 身份(节点键 k) {
    if(k.first)return 二次本次节点身份{k.second};
    return 二次已保存定义节点身份{{稳定编码{k.second}}};
}
二次计算来源 来源(const 二次根输出& r) {
    return std::visit([](const auto& s)->二次计算来源{return s;},r.来源);
}
节点键 输出键(const 二次计算来源& s) {
    if(const auto* x=std::get_if<二次本次输出来源>(&s))return {true,x->局部编号};
    return {false,std::get<二次已保存定义输出来源>(s).定义.结点.值};
}
特征类标量结果角色 输出角色(const 二次计算来源& s) {
    if(const auto* x=std::get_if<二次本次输出来源>(&s))return x->输出角色;
    return std::get<二次已保存定义输出来源>(s).输出角色;
}
unsigned 角色位(特征类标量结果角色 r) noexcept {
    const auto n=static_cast<unsigned>(r);return n>=1&&n<=3 ? 1U<<(n-1):0;
}
unsigned 上下文位(const 二次计算上下文& c) noexcept {
    return (c.参与者A ? 1U:0U)|(c.参与者B ? 2U:0U)|(c.左时间 ? 4U:0U)|(c.右时间 ? 8U:0U)|(c.参照 ? 16U:0U);
}
bool 上下文有效(const 二次计算上下文& c) noexcept {
    return (!c.参与者A||有效(*c.参与者A))&&(!c.参与者B||有效(*c.参与者B))&&(!c.参照||有效(*c.参照));
}
bool 来源有效(const 二次计算来源& s) noexcept {
    if(s.valueless_by_exception())return false;
    if(const auto* x=std::get_if<二次准确F来源>(&s))return 有效(x->F);
    if(const auto* x=std::get_if<二次本次输出来源>(&s))return x->局部编号&&角色位(x->输出角色);
    const auto& x=std::get<二次已保存定义输出来源>(s);return 有效(x.定义.结点)&&角色位(x.输出角色);
}
std::optional<std::int64_t> 整数(const 准确特征读取事实& f) noexcept {
    if(const auto* x=std::get_if<std::int64_t>(&f.完整值))return *x;
    if(const auto* m=std::get_if<特征值信息>(&f.完整值))
        if(const auto* x=std::get_if<std::int64_t>(&m->值内容))return *x;
    return {};
}
bool 准确来源完整(const 准确特征读取事实& f) noexcept {
    if(!浅层结构有效(f.信息)||!有效(f.类型关系)
        ||(f.准确值事实&&!有效(*f.准确值事实)))return false;
    if(const auto* direct=std::get_if<std::int64_t>(&f.信息.准确值)) {
        const auto* value=std::get_if<std::int64_t>(&f.完整值);
        return value&&*value==*direct;
    }
    const auto* identity=std::get_if<特征值身份>(&f.信息.准确值);
    const auto* material=std::get_if<特征值信息>(&f.完整值);
    return identity&&material&&f.准确值事实&&material->值身份==*identity
        &&material->值身份.编码==*f.准确值事实&&std::holds_alternative<std::int64_t>(material->值内容);
}
bool 快照完整(const 有序I64比较合同快照& k) {
    if(!有效(k.K)||!k.H||k.H>k.Gread
        ||(k.来源!=特征比较合同来源::当前独立绑定&&k.来源!=特征比较合同来源::已保存定义固定K))return false;
    特征I64比较绑定定义 definition{k.输入FT,k.用途,k.算法族,k.算法版本,k.左角色,k.右角色,
        k.上下文要求位,k.输入量化,k.误差合同版本,k.误差预算,k.相等容差,k.关系编码,{}};
    for(const auto& output:k.输出组){
        if(!有效(output.输出关系))return false;
        definition.输出组.push_back({output.输出,output.输出FT});
    }
    return I64绑定定义完整(definition);
}
bool 同量化(const 特征类标量量化合同& a,const 特征类标量量化合同& b) noexcept {
    return a.单位==b.单位&&a.维度==b.维度&&a.分量角色==b.分量角色&&a.缩放分子==b.缩放分子
        &&a.缩放分母==b.缩放分母&&a.量纲类别==b.量纲类别&&a.舍入==b.舍入&&a.溢出==b.溢出;
}
有序I64比较合同快照 快照(const 特征I64比较绑定事实& f,std::uint64_t g) {
    const auto& d=f.定义;
    return {特征比较合同来源::当前独立绑定,f.身份,d.输入FT,d.用途,d.算法族,d.算法版本,d.左角色,d.右角色,
        d.上下文要求位,d.输入量化,d.误差合同版本,d.误差预算,d.相等容差,d.关系编码,f.输出组,g,g};
}
bool 固定相容(const 有序I64比较合同快照& a,const 有序I64比较合同快照& b) noexcept {
    if(a.输入FT!=b.输入FT||a.用途!=b.用途||a.算法族!=b.算法族||a.算法版本!=b.算法版本
        ||a.左角色!=b.左角色||a.右角色!=b.右角色||a.上下文要求位!=b.上下文要求位
        ||a.输入量化!=b.输入量化||a.误差合同版本!=b.误差合同版本||a.误差预算!=b.误差预算
        ||a.相等容差!=b.相等容差||a.关系编码!=b.关系编码||a.输出组.size()!=b.输出组.size())return false;
    for(std::size_t i=0;i<a.输出组.size();++i)
        if(a.输出组[i].输出!=b.输出组[i].输出||a.输出组[i].输出FT!=b.输出组[i].输出FT)return false;
    return true;
}
}

namespace 海中鱼巣 {
bool 二次准确计算结果::成功() const noexcept {
    using namespace 二次计算内部;
    try {
        if(版本!=1||状态!=二次计算状态::已计算||!G||!H||H>G||!请求身份||!上下文有效(上下文)
            ||根输出组.empty()||结果组.size()!=根输出组.size()||基础叶组.empty()||计算项组.empty())return false;
        std::map<std::uint64_t,const 准确特征读取事实*> leaves;
        for(const auto& leaf:基础叶组) {
            const auto& f=leaf.事实;
            if(f.Gread!=G||f.H!=H||!准确来源完整(f)||!f.创建G||f.创建G>H
                ||(f.退出G&&*f.退出G<=H)||!整数(f)||!leaves.emplace(f.信息.身份.编码.值,&f).second)return false;
        }
        std::map<节点键,const 二次计算项回执*> nodes;unsigned contexts=0;
        for(const auto& node:计算项组) {
            const auto id=键(node.节点);const auto& k=node.K;
            if(!id.second||nodes.contains(id)||!快照完整(k)||k.Gread!=G
                ||k.H!=(k.来源==特征比较合同来源::当前独立绑定 ? G:H)||k.上下文要求位>31
                ||node.输出组.empty()||node.真实阶次<=1
                ||(id.first!=(k.来源==特征比较合同来源::当前独立绑定)))return false;
            contexts|=k.上下文要求位;
            std::uint32_t maximum=0;
            for(std::size_t i=0;i<2;++i) {
                const auto& x=node.输入[i];
                if(x.顺序!=i+1||x.角色!=(i ? k.右角色:k.左角色)||x.实际FT!=k.输入FT||!来源有效(x.来源))return false;
                if(x.值<k.输入量化.下界||x.值>k.输入量化.上界)return false;
                if(const auto* s=std::get_if<二次准确F来源>(&x.来源)) {
                    const auto it=leaves.find(s->F.编码.值);
                    if(it==leaves.end()||it->second->信息.类型!=x.实际FT||整数(*it->second)!=x.值||x.实际阶次!=1)return false;
                } else {
                    const auto it=nodes.find(输出键(x.来源));
                    if(it==nodes.end())return false;
                    const auto& outputs=it->second->输出组;
                    const auto o=std::find_if(outputs.begin(),outputs.end(),[&](const auto& v){return v.输出角色==输出角色(x.来源);});
                    if(o==outputs.end()||o->值!=x.值||o->输出FT!=x.实际FT||o->真实阶次!=x.实际阶次
                        ||!同量化(o->量化,k.输入量化))return false;
                }
                maximum=std::max(maximum,x.实际阶次);
            }
            if(maximum==UINT32_MAX||maximum+1!=node.真实阶次)return false;
            unsigned prior=0;
            for(const auto& o:node.输出组) {
                const auto role=static_cast<unsigned>(o.输出角色);
                const auto spec=std::find_if(k.输出组.begin(),k.输出组.end(),[&](const auto& v){return v.输出.角色==o.输出角色;});
                if(!角色位(o.输出角色)||role<=prior||键(o.节点)!=id||o.K!=k.K||o.真实阶次!=node.真实阶次
                    ||spec==k.输出组.end()||o.输出FT!=spec->输出FT||o.量化!=spec->输出.量化
                    ||o.值<o.量化.下界||o.值>o.量化.上界)return false;
                prior=role;
            }
            nodes.emplace(id,&node);
        }
        if(contexts!=上下文位(上下文))return false;
        std::set<std::pair<节点键,unsigned>> roots;
        for(std::size_t i=0;i<根输出组.size();++i) {
            const auto s=来源(根输出组[i]);
            if(!来源有效(s))return false;
            const auto id=输出键(s);const auto role=输出角色(s);
            const auto it=nodes.find(id);
            if(it==nodes.end()||!roots.emplace(id,static_cast<unsigned>(role)).second)return false;
            const auto o=std::find_if(it->second->输出组.begin(),it->second->输出组.end(),[&](const auto& v){return v.输出角色==role;});
            if(o==it->second->输出组.end()||*o!=结果组[i])return false;
        }
        return true;
    } catch(...) {return false;}
}
}
