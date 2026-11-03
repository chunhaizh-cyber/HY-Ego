#include "模拟任务需求来源.服务.h"
#include "服务要求.结构化.h"
#include <algorithm>
#include <stdexcept>
#include <tuple>

namespace 海中鱼巣 {
namespace {
void 规范引用组(std::vector<新模拟业务待展开引用>& edges) {
    std::sort(edges.begin(),edges.end(),[](const auto& a,const auto& b) {
        return std::tie(a.类别,a.来源身份,a.字段角色)<std::tie(b.类别,b.来源身份,b.字段角色);
    });
    edges.erase(std::unique(edges.begin(),edges.end(),[](const auto& a,const auto& b) {
        return a.类别==b.类别&&a.来源身份==b.来源身份&&a.字段角色==b.字段角色;
    }),edges.end());
}
}
模拟任务需求来源服务::模拟任务需求来源服务(const 新_任务类& t,const 新_需求类& d,
    const 结构化服务要求服务& r,const 新_自然语言世界树类& l) noexcept
    :任务_(t),需求_(d),服务要求_(r),语言_(l) {}
新模拟需求来源结果<服务要求理解信息> 模拟任务需求来源服务::读取模拟理解来源(稳定编码 n) const {return 服务要求_.读取模拟理解来源(n);}
新模拟需求来源结果<服务要求确认信息> 模拟任务需求来源服务::读取模拟确认来源(稳定编码 n) const {return 服务要求_.读取模拟确认来源(n);}
新模拟需求来源结果<新部分服务要求材料> 模拟任务需求来源服务::读取模拟要求来源(稳定编码 n) const {return 服务要求_.读取模拟要求来源(n);}
新模拟需求来源结果<新模拟需求构造依据> 模拟任务需求来源服务::读取模拟构造依据() const {return 服务要求_.读取模拟构造依据();}
新自然语言场景复制结果 模拟任务需求来源服务::读取语言场景来源(稳定编码 n) const {return 语言_.读取复制场景(n);}

新模拟业务来源读回<新模拟任务来源投影> 模拟任务需求来源服务::读取任务来源(稳定编码 T) const {
    using S=新模拟业务来源状态;
    using K=新模拟业务引用类别;
    const auto t=任务_.获取任务(T);
    if(!t)return {S::来源不存在,{},"task source unavailable"};
    const auto v=任务_.获取任务虚拟存在(T);
    if(!v) {
        if(任务_.获取任务(T)!=t)return {S::来源已变化,{},"task changed before virtual existence capture"};
        throw std::logic_error("stable task has no readable virtual existence");
    }
    const 新任务虚拟存在引用 ref{T,v->节点,v->任务状态特征节点};
    const auto f=任务_.读取任务虚拟存在特征组(ref);
    if(!f) {
        if(任务_.获取任务(T)!=t||任务_.获取任务虚拟存在(T)!=v)
            return {S::来源已变化,{},"task changed before governance feature capture"};
        throw std::logic_error("stable task virtual existence has no readable governance features");
    }
    // 顺序采集不能假称原子快照；前后复核只检测本次可见变化。
    const auto t2=任务_.获取任务(T);
    const auto v2=任务_.获取任务虚拟存在(T);
    const auto f2=任务_.读取任务虚拟存在特征组(ref);
    if(t2!=t||v2!=v||f2!=f) {
        std::string 诊断{"task source changed during capture"};
        const auto 记变化 = [&](const char* 字段, const auto& 前, const auto& 后) {
            if (前 != 后) 诊断 += std::string{" "} + 字段;
        };
        if (t && t2) {
            记变化("T.identity", t->节点, t2->节点);
            记变化("T.demand", t->需求节点, t2->需求节点);
            记变化("T.virtual", t->任务虚拟存在节点, t2->任务虚拟存在节点);
            if (t->当前状态 != t2->当前状态)
                诊断 += " T.state=" + std::to_string(static_cast<int>(t->当前状态))
                    + "->" + std::to_string(static_cast<int>(t2->当前状态));
            记变化("T.candidate", t->当前候选方法材料节点, t2->当前候选方法材料节点);
            记变化("T.method", t->最近方法节点, t2->最近方法节点);
            记变化("T.execution", t->最近执行实例节点, t2->最近执行实例节点);
            记变化("T.evaluations", t->影响评估节点组, t2->影响评估节点组);
            记变化("T.settlement", t->完成结算节点, t2->完成结算节点);
            记变化("T.method_instance", t->方法实例节点, t2->方法实例节点);
            记变化("T.organization", t->方法执行组织节点, t2->方法执行组织节点);
        } else if (t != t2) 诊断 += " T.presence";
        if (v && v2) {
            记变化("V.identity", v->节点, v2->节点);
            记变化("V.task", v->任务节点, v2->任务节点);
            记变化("V.state_feature", v->任务状态特征节点, v2->任务状态特征节点);
            记变化("V.state", v->当前状态, v2->当前状态);
            记变化("V.organization", v->方法执行组织节点, v2->方法执行组织节点);
        } else if (v != v2) 诊断 += " V.presence";
        if (f && f2) {
            记变化("F.reference", f->引用, f2->引用);
            记变化("F.state", f->状态, f2->状态);
            记变化("F.service_value", f->服务值, f2->服务值);
        } else if (f != f2) 诊断 += " F.presence";
        return {S::来源已变化,{},std::move(诊断)};
    }
    if(t->节点!=T||v->任务节点!=T||t->任务虚拟存在节点!=v->节点
        ||f->引用!=ref||f->状态.特征节点!=v->任务状态特征节点
        ||static_cast<std::int64_t>(t->当前状态)!=f->状态.当前值
        ||static_cast<std::int64_t>(v->当前状态)!=f->状态.当前值)
        throw std::logic_error("task source owner projections disagree");
    新模拟任务来源投影 p{*t,*v,*f,{}};
    const auto edge=[&](K k,std::optional<稳定编码> n,const char* role) {
        if(n){if(!有效(*n))throw std::logic_error("invalid declared task reference");p.实例引用组.push_back({k,*n,role});}
    };
    edge(K::需求,t->需求节点,"T.需求节点");
    edge(K::任务虚拟存在,v->节点,"T.任务虚拟存在节点");
    edge(K::任务,T,"Vt.任务节点");
    edge(K::特征,f->状态.特征节点,"Vt.任务状态特征节点");
    if(f->服务值)edge(K::特征,f->服务值->特征节点,"Vt.服务值特征节点");
    edge(K::候选方法材料,t->当前候选方法材料节点,"T.当前候选方法材料节点");
    edge(K::方法,t->最近方法节点,"T.最近方法节点");
    edge(K::执行实例,t->最近执行实例节点,"T.最近执行实例节点");
    for(const auto n:t->影响评估节点组)edge(K::影响评估,n,"T.影响评估节点组");
    edge(K::完成结算,t->完成结算节点,"T.完成结算节点");
    edge(K::方法实例,t->方法实例节点,"T.方法实例节点");
    edge(K::执行组织,t->方法执行组织节点,"T.方法执行组织节点");
    edge(K::执行组织,v->方法执行组织节点,"Vt.方法执行组织节点");
    规范引用组(p.实例引用组);
    return {S::已读取,std::move(p),{}};
}

新模拟业务来源读回<新模拟需求来源投影> 模拟任务需求来源服务::读取需求来源(稳定编码 D) const {
    using S=新模拟业务来源状态;
    using K=新模拟业务引用类别;
    const auto identity=需求_.读取需求节点引用身份(D);
    if(identity.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
    if(identity.状态!=特征概念规则状态::已完成) {
        if(identity.状态==特征概念规则状态::值不存在||identity.状态==特征概念规则状态::入口拒绝)return {S::来源不存在};
        throw std::logic_error("demand identity source engineering read failed");
    }
    if(!identity.身份||identity.身份保护.empty())throw std::logic_error("successful demand identity projection incomplete");
    const auto g=需求_.读取需求治理信息(D);
    if(!g)throw std::logic_error("valid demand identity has no governance projection");
    const auto d=需求_.获取需求(D);
    if(需求_.读取需求治理信息(D)!=g||需求_.获取需求(D)!=d)
        return {S::来源已变化,{},"demand source changed during capture"};
    if(g->节点!=D||d.has_value()!=g->完整目标.has_value()
        ||(d&&(d->节点!=D||d->需求主体存在节点!=g->需求主体存在节点
        ||d->目标存在节点!=g->完整目标->目标存在节点||d->目标!=g->完整目标->目标)))
        throw std::logic_error("demand source owner projections disagree");
    if(g->完整目标 && g->完整目标->目标.新增成员来源见证)
        return {S::尚缺依赖提供者,{},"新增成员来源见证目标的隔离模拟来源尚缺依赖提供者",{}};
    if(g->完整目标 && g->完整目标->目标.引用内容定位)
        return {S::尚缺依赖提供者,{},"引用内容目标的隔离模拟来源尚未支持"};
    新模拟需求来源投影 p{*g,d,{}};
    const auto edge=[&](K k,std::optional<稳定编码> n,const char* role) {
        if(n){if(!有效(*n))throw std::logic_error("invalid declared demand reference");p.实例引用组.push_back({k,*n,role});}
    };
    edge(K::需求,g->父需求节点,"D.父需求节点");
    edge(K::存在,g->需求主体存在节点,"D.需求主体存在节点");
    if(g->完整目标)edge(K::存在,g->完整目标->目标存在节点,"D.目标存在节点");
    edge(K::需求承接,g->来源承接节点,"D.来源承接节点");
    for(const auto n:g->服务要求确认组)edge(K::服务要求确认,n,"D.服务要求确认组");
    edge(K::执行实例,g->服务需求取得执行节点,"D.服务需求取得执行节点");
    edge(K::服务要求,g->外部要求节点,"D.外部要求节点");
    if(g->认知来源) {
        edge(K::语言理解,g->认知来源->理解节点,"D.认知来源.理解节点");
        edge(K::服务要求确认,g->认知来源->内容确认节点,"D.认知来源.内容确认节点");
        edge(K::服务要求,g->认知来源->来源要求节点,"D.认知来源.来源要求节点");
    }
    if(g->外部要求材料) {
        const auto& a=*g->外部要求材料;
        edge(K::服务要求,a.要求节点,"D.外部要求材料.要求节点");
        edge(K::存在,a.来源人类节点,"D.外部要求材料.来源人类节点");
        edge(K::存在,a.已知目标.目标存在节点,"D.外部要求材料.目标存在节点");
        edge(K::语言理解,a.理解节点,"D.外部要求材料.理解节点");
        edge(K::服务要求确认,a.确认节点,"D.外部要求材料.确认节点");
    }
    规范引用组(p.实例引用组);
    const auto freshIdentity=需求_.读取需求节点引用身份(D);
    if(freshIdentity.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
    if(freshIdentity.状态==特征概念规则状态::值不存在)return {S::来源已变化,{},"demand identity disappeared during capture"};
    if(freshIdentity.状态!=特征概念规则状态::已完成||!freshIdentity.身份||freshIdentity.身份保护.empty())
        throw std::logic_error("fresh demand identity source engineering read failed");
    if(identity.身份保护.size()!=freshIdentity.身份保护.size())return {S::来源已变化};
    for(std::size_t i=0;i<identity.身份保护.size();++i) {
        const auto& a=identity.身份保护[i];const auto& b=freshIdentity.身份保护[i];
        if(a.结构.字段组!=b.结构.字段组||a.入向字段组!=b.入向字段组
            ||a.入向外部关系组!=b.入向外部关系组||a.出向外部关系组!=b.出向外部关系组)return {S::来源已变化};
    }
    return {S::已读取,std::move(p),{},freshIdentity.身份保护};
}
}
