#include "词条归类.事实服务.h"
#include "../领域/新_词类.h"
#include "../领域/概念_名称关系类.h"
#include "../领域/概念_存在类.h"
#include "../领域/概念_特征类.h"
#include "../领域/概念_动态类.h"
#include <algorithm>
#include <array>
#include <new>
#include <stdexcept>
namespace 海中鱼巣 {
namespace {
bool 相同(const 基础联合节点快照&a,const 基础联合节点快照&b){return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组&&a.入向字段组==b.入向字段组&&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;}
bool 合并(std::vector<基础联合节点快照>&p,const std::vector<基础联合节点快照>&more){for(const auto&n:more){const auto it=std::find_if(p.begin(),p.end(),[&](const auto&x){return x.结构.节点.编码==n.结构.节点.编码;});if(it==p.end())p.push_back(n);else if(!相同(*it,n))return false;}return true;}
std::optional<基础联合节点快照> 取快照(稳定编码 n){const auto r=全局基础数据集.查询节点组完整结构({n});if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=1)throw std::logic_error("lemma source snapshot failed");return r.节点组.front().快照;}
bool 未变(const std::vector<基础联合节点快照>&p){for(const auto&n:p){const auto r=取快照(n.结构.节点.编码);if(!r||!相同(n,*r))return false;}return true;}
// 唯一可放宽的是本次刚发表节点引起的引用/登记增量，源内容和原关系仍完整核验。
std::optional<std::vector<基础联合节点快照>> 刷新自身前缀(const std::vector<基础联合节点快照>&source,const std::vector<稳定编码>&own){
    std::vector<基础联合节点快照> result;
    const auto 是自身=[&](稳定编码 n){return std::find(own.begin(),own.end(),n)!=own.end();};
    for(const auto&old:source){const auto fresh=取快照(old.结构.节点.编码);if(!fresh)return {};auto normalized=*fresh;
        std::erase_if(normalized.结构.字段组,[&](const auto&f){const auto*n=std::get_if<稳定编码>(&f.内容);return n&&是自身(*n)&&std::find(old.结构.字段组.begin(),old.结构.字段组.end(),f)==old.结构.字段组.end();});
        std::erase_if(normalized.入向字段组,[&](const auto&f){return 是自身(f.所属节点)&&std::find(old.入向字段组.begin(),old.入向字段组.end(),f)==old.入向字段组.end();});
        const auto 边=[&](const auto&e,const auto&prior){return (是自身(e.源节点)||是自身(e.目标节点))&&std::find(prior.begin(),prior.end(),e)==prior.end();};
        std::erase_if(normalized.入向外部关系组,[&](const auto&e){return 边(e,old.入向外部关系组);});
        std::erase_if(normalized.出向外部关系组,[&](const auto&e){return 边(e,old.出向外部关系组);});
        if(!相同(old,normalized))return {};result.push_back(*fresh);
    }return result;
}
新词条归类概念写回状态 映射(归类概念读取状态 s){using S=归类概念读取状态;using R=新词条归类概念写回状态;switch(s){case S::已读取:return R::已写入且读回;case S::非本提供者:return R::未满足前提;case S::前提已变化:return R::前提变化;case S::依赖未就绪:return R::依赖未就绪;case S::资源失败:return R::资源失败;default:return R::结构不一致;}}
}
词条归类事实服务::词条归类事实服务(新_词类&w,概念_名称关系类&n,概念_存在类&e,概念_特征类&f,概念_动态类&d) noexcept:词_(w),名称_(n),存在_(e),特征_(f),动态_(d){名称_.装配归类动态概念读取(&动态_);}
归类概念身份读取结果 词条归类事实服务::读取概念(稳定编码 n) const {
    const std::array all{存在_.读取语言归类概念身份保护(n),特征_.读取语言归类概念身份保护(n),动态_.读取语言归类概念身份保护(n)};
    std::optional<归类概念身份读取结果> found;
    for(const auto&r:all){if(r.状态==归类概念读取状态::资源失败||r.状态==归类概念读取状态::前提已变化||r.状态==归类概念读取状态::结构不一致)return r;if(r.状态==归类概念读取状态::已读取){if(found)return {归类概念读取状态::结构不一致,{},{}};found=r;}}
    if(found)return *found;
    if(std::any_of(all.begin(),all.end(),[](const auto&r){return r.状态==归类概念读取状态::依赖未就绪;}))return {归类概念读取状态::依赖未就绪,{},{}};
    return {};
}
新词条归类读取结果 词条归类事实服务::读取词条归类目标(新词条归类目标 target) const {
    using S=新词条归类读取状态;
    try{
        const auto word=词_.读取词条保护(target.目标词条);
        if(word.状态==新词条保护状态::资源失败)return {S::资源失败,{}};
        if(word.状态==新词条保护状态::前提已变化)return {S::前提已变化,{}};
        if(word.状态==新词条保护状态::未找到)return {S::词条不存在,word.完整保护};
        if(word.状态!=新词条保护状态::已读取||!word.信息)return {S::结构不一致,{}};
        auto protect=word.完整保护;
        const auto names=名称_.读取词条名称候选保护(target.目标词条);
        if(names.状态==概念名称关系查询状态::资源失败)return {S::资源失败,{}};
        if(names.状态==概念名称关系查询状态::读取漂移||names.状态==概念名称关系查询状态::依赖已变化)return {S::前提已变化,{}};
        if(names.状态!=概念名称关系查询状态::已找到&&names.状态!=概念名称关系查询状态::未找到)return {S::结构不一致,{}};
        if(!合并(protect,names.完整保护))return {S::前提已变化,{}};
        bool matched=false;
        for(const auto&candidate:names.候选组){
            const auto* name=std::get_if<概念名称关系信息>(&candidate);
            if(!name)continue; // 指代名称属于出现消解，不是本种类的三根归类事实。
            const auto endpoint=读取概念(name->概念节点);
            if(endpoint.状态==归类概念读取状态::前提已变化)return {S::前提已变化,{}};
            if(endpoint.状态==归类概念读取状态::资源失败)return {S::资源失败,{}};
            if(endpoint.状态==归类概念读取状态::依赖未就绪)return {S::依赖未就绪,{}};
            if(endpoint.状态!=归类概念读取状态::已读取||!endpoint.信息)return {S::结构不一致,{}};
            if(!合并(protect,endpoint.完整保护))return {S::前提已变化,{}};
            if(name->概念节点==endpoint.信息->所属根)return {S::结构不一致,{}};
            if(有效(endpoint.信息->所属根))matched=true;
        }
        if(!未变(protect))return {S::前提已变化,{}};
        return {matched ? S::已满足 : S::未满足,std::move(protect)};
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(...){return {S::结构不一致,{}};}
}
新词条归类概念写回结果 词条归类事实服务::写回词条归类(const 新词条归类概念写回请求&q){
    using R=新词条归类概念写回状态;新词条归类概念写回结果 out;
    try{
        if(!有效(q.目标词条)||!有效(q.上位概念)||!有效(q.归类需求)||!有效(q.回答消息)||q.来源保护.empty())return out;
        for(const auto id:{q.归类需求,q.回答消息})if(std::none_of(q.来源保护.begin(),q.来源保护.end(),[&](const auto&x){return x.结构.节点.编码==id;}))return out;
        if(!未变(q.来源保护)){out.状态=R::前提变化;return out;}
        const auto word=词_.读取词条保护(q.目标词条);if(word.状态!=新词条保护状态::已读取||!word.信息){out.状态=word.状态==新词条保护状态::资源失败 ? R::资源失败 : R::未满足前提;return out;}
        const auto currentNames=名称_.读取词条指代候选保护(q.目标词条);
        if(currentNames.状态==概念名称关系查询状态::资源失败){out.状态=R::资源失败;return out;}
        if(currentNames.状态!=概念名称关系查询状态::已找到&&currentNames.状态!=概念名称关系查询状态::未找到){out.状态=R::前提变化;return out;}
        if(std::any_of(currentNames.候选组.begin(),currentNames.候选组.end(),[](const auto&v){return std::holds_alternative<指代名称关系信息>(v);}))return out;
        const auto Y=读取概念(q.上位概念);if(Y.状态!=归类概念读取状态::已读取||!Y.信息){out.状态=映射(Y.状态);return out;}
        auto source=q.来源保护;if(!合并(source,word.完整保护)||!合并(source,Y.完整保护)||!合并(source,currentNames.完整保护)){out.状态=R::前提变化;return out;}
        归类概念建立请求 create{q.目标词条,q.上位概念,q.归类需求,q.回答消息,source};
        const auto E=存在_.读取语言归类概念身份保护(q.上位概念),F=特征_.读取语言归类概念身份保护(q.上位概念);
        const auto made=E.信息 ? 存在_.建立或取得语言归类概念(create) : F.信息 ? 特征_.建立或取得语言归类概念(create) : 动态_.建立或取得语言归类概念(create);
        out.概念节点=made.概念节点;if(made.状态!=归类概念读取状态::已读取||!made.概念节点){out.状态=映射(made.状态);return out;}
        auto refreshed=刷新自身前缀(source,{*made.概念节点});if(!refreshed){out.状态=R::前提变化;return out;}
        const auto named=名称_.建立或取得归类名称关系(*made.概念节点,q.目标词条,*refreshed);out.名称关系节点=named.名称关系节点;
        if(named.状态!=概念名称关系建立状态::已建立&&named.状态!=概念名称关系建立状态::已复用){out.状态=named.状态==概念名称关系建立状态::资源失败 ? R::资源失败 : named.状态==概念名称关系建立状态::依赖已变化 ? R::前提变化 : R::结构不一致;return out;}
        const auto back=读取概念(*made.概念节点);if(back.状态!=归类概念读取状态::已读取||!back.信息){out.状态=映射(back.状态);return out;}
        const auto relation=out.名称关系节点 ? 名称_.获取名称关系(*out.名称关系节点) : 概念名称关系查询结果{};
        if(!back.信息->定义未知||back.信息->直接上位!=q.上位概念||back.信息->目标词条!=q.目标词条||!relation.信息||relation.信息->概念节点!=*made.概念节点||relation.信息->词条节点!=q.目标词条){out.状态=R::结构不一致;return out;}
        const auto current=读取词条归类目标({q.目标词条});if(current.状态!=新词条归类读取状态::已满足){out.状态=current.状态==新词条归类读取状态::资源失败 ? R::资源失败 : R::前提变化;return out;}
        const auto final=刷新自身前缀(source,{*made.概念节点,*out.名称关系节点});if(!final){out.状态=R::前提变化;return out;}
        out.完整保护=current.完整保护;if(!合并(out.完整保护,back.完整保护)||!合并(out.完整保护,*final)){out.状态=R::前提变化;return out;}
        out.状态=R::已写入且读回;return out;
    }catch(const std::bad_alloc&){out.状态=R::资源失败;return out;}
    catch(...){out.状态=R::结构不一致;return out;}
}
}
