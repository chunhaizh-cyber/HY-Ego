#include "应用服务.特征概念类.h"

#include <algorithm>
#include <array>
#include <limits>
#include <new>
#include <variant>

namespace 海中鱼巣 {
namespace {

bool 键有效且互异(const L1所有者范围写入幂等身份& fc,
                 const 原子I64特征出生键& keys) noexcept {
    const std::array<L1所有者范围写入幂等身份,6> all{
        fc,keys.组合,keys.内容,keys.已知,keys.组织,keys.概念使用};
    for(std::size_t i=0;i<all.size();++i) {
        if(!有效(all[i])) return false;
        for(std::size_t j=0;j<i;++j) if(all[i]==all[j]) return false;
    }
    return true;
}

bool 域包含(const std::vector<概念树I64区间>& outer,
            const std::vector<概念树I64区间>& inner) noexcept {
    for(const auto& target:inner) {
        bool covered=false;
        for(const auto& candidate:outer) {
            if(candidate.下界<=target.下界&&candidate.上界>=target.上界) {
                covered=true;
                break;
            }
        }
        if(!covered) return false;
    }
    return true;
}

bool 域相同(const std::vector<概念树I64区间>& a,
            const std::vector<概念树I64区间>& b) noexcept {
    return a==b;
}

std::vector<概念树I64区间> 转为概念域(const 特征规范I64域& domain) {
    std::vector<概念树I64区间> out;
    out.reserve(domain.区间.size());
    for(const auto& x:domain.区间) out.push_back({x.下界,x.上界});
    return out;
}

bool 单点属于完整域(const 特征规范I64域& domain,std::int64_t value) noexcept {
    return std::any_of(domain.区间.begin(),domain.区间.end(),[&](const auto& x) {
        return x.下界<=value&&value<=x.上界;
    });
}

} // namespace

bool I64单值出生概念确保结果::成功() const noexcept {
    if(版本!=1||原请求.版本!=1||!Gread||!FCv||!有效(FCv->概念.值)||
       (状态!=I64单值出生概念确保状态::已建立&&
        状态!=I64单值出生概念确保状态::已复用&&
        状态!=I64单值出生概念确保状态::精确重复)) return false;
    const auto* definition=std::get_if<纯I64特征概念定义>(&FCv->定义);
    if(!definition||FCv->类别!=相关概念类别::特征||
       FCv->治理状态!=概念树生命周期状态::活跃||
       definition->特征类型.值!=原请求.FT.编码||definition->规范域.size()!=1||
       definition->规范域.front()!=概念树I64区间{原请求.准确I64,原请求.准确I64}) return false;
    if(原请求.初始组织==概念初始组织指定::显式顶层)
        return 原请求.直接上位.empty()&&FCv->直接上位.empty();
    if(原请求.初始组织!=概念初始组织指定::具名上位||原请求.直接上位.empty()||
       FCv->直接上位.size()!=原请求.直接上位.size()) return false;
    auto actual=FCv->直接上位;
    std::sort(actual.begin(),actual.end(),[](const auto&a,const auto&b) {
        return a.上位.值.值<b.上位.值.值;
    });
    auto expected=原请求.直接上位;
    std::sort(expected.begin(),expected.end(),[](const auto&a,const auto&b) {
        return a.值.值<b.值.值;
    });
    for(std::size_t i=0;i<actual.size();++i) if(actual[i].上位!=expected[i]) return false;
    return true;
}

bool I64原子准确特征出生应用结果::成功() const noexcept {
    return 版本==1&&原请求.版本==1&&
        (状态==I64原子准确特征出生应用状态::已创建||
         状态==I64原子准确特征出生应用状态::已复用||
         状态==I64原子准确特征出生应用状态::精确重复)&&
        FCv确保&&FCv确保->成功()&&事实&&有效(事实->F)&&有效(事实->FCv)&&
        事实->正式特征类型==原请求.FT.编码&&事实->准确I64==原请求.准确I64&&
        事实->位置==原请求.位置;
}

bool 特征概念应用服务::概念请求有效(const I64单值出生概念确保请求& r) noexcept {
    if(r.版本!=1||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()||
       !有效(r.FT)||!有效(r.概念键)||!r.概念预算.最大概念数||
       !r.概念预算.最大关系数||!r.概念预算.最大特征属性数) return false;
    if(r.初始组织==概念初始组织指定::显式顶层) return r.直接上位.empty();
    if(r.初始组织!=概念初始组织指定::具名上位||r.直接上位.empty()) return false;
    for(std::size_t i=0;i<r.直接上位.size();++i) {
        if(!有效(r.直接上位[i].值)) return false;
        for(std::size_t j=0;j<i;++j) if(r.直接上位[i]==r.直接上位[j]) return false;
    }
    return true;
}

bool 特征概念应用服务::请求有效(const I64原子准确特征出生应用请求& r) noexcept {
    return r.版本==1&&r.G0&&r.G0!=std::numeric_limits<std::uint64_t>::max()&&
        有效(r.位置.场景)&&有效(r.位置.组织父)&&有效(r.FT)&&
        键有效且互异(r.FCv概念键,r.F出生键)&&r.候选读取预算.最大候选数&&
        r.组织读取预算.最大路径长度&&r.出生概念读取预算.最大概念数&&
        r.出生概念读取预算.最大关系数&&r.出生概念读取预算.最大特征属性数&&
        r.FCv概念预算.最大概念数&&r.FCv概念预算.最大关系数&&
        r.FCv概念预算.最大特征属性数;
}

I64单值出生概念确保状态 特征概念应用服务::映射概念状态(纯概念状态 s) noexcept {
    switch(s) {
    case 纯概念状态::事实代次漂移: return I64单值出生概念确保状态::事实代次漂移;
    case 纯概念状态::幂等冲突: return I64单值出生概念确保状态::幂等冲突;
    case 纯概念状态::历史材料不可用: return I64单值出生概念确保状态::历史材料不可用;
    case 纯概念状态::资源失败: return I64单值出生概念确保状态::资源失败;
    case 纯概念状态::已可能发布: return I64单值出生概念确保状态::已可能发布;
    case 纯概念状态::入口拒绝: return I64单值出生概念确保状态::入口拒绝;
    case 纯概念状态::定义不相容:
    case 纯概念状态::类别冲突: return I64单值出生概念确保状态::类型不相容;
    default: return I64单值出生概念确保状态::内部不一致;
    }
}

I64原子准确特征出生应用状态 特征概念应用服务::映射出生状态(
    原子I64特征出生状态 s) noexcept {
    return s==原子I64特征出生状态::入口拒绝 ?
        I64原子准确特征出生应用状态::入口拒绝 :
        I64原子准确特征出生应用状态::出生失败;
}

std::vector<概念树概念身份> 特征概念应用服务::计算直接上位(
    const 特征规范I64域& complete,const I64特征概念组织读取结果& concepts,
    const 特征规范I64域& child) {
    const auto childDomain=转为概念域(child);
    std::vector<const 纯概念事实*> candidates;
    for(const auto& fact:concepts.概念组) {
        const auto* definition=std::get_if<纯I64特征概念定义>(&fact.定义);
        if(!definition||!域包含(definition->规范域,childDomain)||
           域相同(definition->规范域,childDomain)) continue;
        candidates.push_back(&fact);
    }
    std::vector<概念树概念身份> result;
    for(const auto* candidate:candidates) {
        const auto* candidateDefinition=std::get_if<纯I64特征概念定义>(&candidate->定义);
        bool farther=false;
        for(const auto* middle:candidates) {
            if(middle==candidate) continue;
            const auto* middleDefinition=std::get_if<纯I64特征概念定义>(&middle->定义);
            if(域包含(candidateDefinition->规范域,middleDefinition->规范域)&&
               !域相同(candidateDefinition->规范域,middleDefinition->规范域)) {
                farther=true;
                break;
            }
        }
        if(!farther) result.push_back(candidate->概念);
    }
    std::sort(result.begin(),result.end(),[](auto a,auto b) { return a.值.值<b.值.值; });
    result.erase(std::unique(result.begin(),result.end()),result.end());
    return result;
}

I64单值出生概念确保结果 特征概念应用服务::确保FCv(
    const I64原子准确特征出生应用请求& source) {
    I64单值出生概念确保结果 out;
    const 纯I64特征概念定义 definition{
        概念树特征类型引用{source.FT.编码},{{source.准确I64,source.准确I64}}};
    const 纯概念查询请求 exact{2,source.G0,source.G0,definition,source.FCv概念预算};
    const auto found=概念服务_.精确查询纯概念(exact);
    if(found.成功(exact)) {
        out.状态=I64单值出生概念确保状态::已复用;
        out.Gread=found.Gread;
        out.FCv=found.事实;
        out.原请求={1,source.G0,source.FT,source.准确I64,source.FCv概念键,
                    found.事实->直接上位.empty()?概念初始组织指定::显式顶层:
                    概念初始组织指定::具名上位,{},source.FCv概念预算};
        for(const auto& edge:found.事实->直接上位) out.原请求.直接上位.push_back(edge.上位);
        if(!out.成功()) { out.状态=I64单值出生概念确保状态::内部不一致; out.FCv.reset(); }
        return out;
    }
    if(!found.确认未找到(exact)) {
        out.状态=映射概念状态(found.状态); out.Gread=found.Gread; return out;
    }

    const auto current=概念服务_.读取当前事实代次();
    if(!current.成功()||!current.Gread) {
        out.状态=I64单值出生概念确保状态::资源失败;
        return out;
    }
    const 特征类型截止请求 typeRead{1,current.Gread,current.Gread,source.FT};
    const I64特征概念组织读取请求 conceptRead{2,current.Gread,current.Gread,source.FT,source.FCv概念预算};
    const auto complete=特征服务_.读取I64类型完整域(typeRead);
    const auto allConcepts=概念服务_.读取当前I64特征概念(conceptRead);
    const auto* completeFact=std::get_if<特征截止事实<特征规范I64域>>(&complete);
    if(!completeFact||completeFact->Gread!=current.Gread||completeFact->H!=current.Gread) {
        out.状态=I64单值出生概念确保状态::规则缺失;
        return out;
    }
    if(!allConcepts.成功(conceptRead)) {
        out.状态=映射概念状态(allConcepts.状态); out.Gread=allConcepts.Gread; return out;
    }
    if(!单点属于完整域(completeFact->数据,source.准确I64)) {
        out.状态=I64单值出生概念确保状态::类型不相容; out.Gread=current.Gread; return out;
    }

    const 特征规范I64域 child{{{source.准确I64,source.准确I64}}};
    I64单值出生概念确保请求 request;
    request.G0=current.Gread; request.FT=source.FT; request.准确I64=source.准确I64;
    request.概念键=source.FCv概念键; request.概念预算=source.FCv概念预算;
    if(completeFact->数据==child) request.初始组织=概念初始组织指定::显式顶层;
    else {
        request.初始组织=概念初始组织指定::具名上位;
        request.直接上位=计算直接上位(completeFact->数据,allConcepts,child);
        if(request.直接上位.empty()) {
            out.状态=I64单值出生概念确保状态::规则缺失; out.Gread=current.Gread; return out;
        }
    }
    if(!概念请求有效(request)) { out.状态=I64单值出生概念确保状态::入口拒绝; return out; }
    const 纯概念创建请求 create{2,request.G0,request.概念键,definition,
                                  request.初始组织,request.直接上位,request.概念预算};
    const auto saved=概念服务_.创建或复用纯概念(create);
    out.Gread=saved.Gread; out.原请求=request; out.首次H=saved.首次发布H; out.FCv=saved.事实;
    if(!saved.成功(create)) {
        out.状态=映射概念状态(saved.状态);
        out.FCv.reset(); out.首次H.reset();
        return out;
    }
    out.状态=saved.状态==纯概念状态::已创建 ? I64单值出生概念确保状态::已建立 :
        I64单值出生概念确保状态::精确重复;
    if(!out.成功()) { out.状态=I64单值出生概念确保状态::内部不一致; out.FCv.reset(); }
    return out;
}

特征概念应用服务::特征概念应用服务(特征类数据服务& features,
    概念树类数据服务& concepts,原子I64特征出生数据服务& atoms)
    : 特征服务_(features),概念服务_(concepts),原子服务_(atoms) {}

I64原子准确特征出生应用结果 特征概念应用服务::处理I64原子准确特征出生(
    const I64原子准确特征出生应用请求& r) {
    I64原子准确特征出生应用结果 out; out.原请求=r;
    if(!请求有效(r)) return out;
    try {
        out.FCv确保=确保FCv(r);
        if(!out.FCv确保->成功()) {
            out.状态=I64原子准确特征出生应用状态::FCv确保失败;
            out.Gread=out.FCv确保->Gread;
            return out;
        }
        const auto current=概念服务_.读取当前事实代次();
        if(!current.成功()||!current.Gread) {
            out.状态=I64原子准确特征出生应用状态::候选查询失败;
            return out;
        }
        const 原子I64特征候选查询请求 query{1,current.Gread,r.位置,r.FT.编码,r.准确I64,
                                                r.候选读取预算,r.组织读取预算};
        out.候选查询=原子服务_.查询精确候选(query);
        if(out.候选查询->状态!=原子I64特征候选查询状态::已读取||
           out.候选查询->Gread!=query.Gread||out.候选查询->原请求!=query||
           out.候选查询->候选.size()>r.候选读取预算.最大候选数) {
            out.状态=I64原子准确特征出生应用状态::候选查询失败;
            out.Gread=out.候选查询->Gread;
            return out;
        }
        for(const auto& candidate:out.候选查询->候选) {
            if(!有效(candidate.F)||!candidate.出生H||candidate.出生H>query.Gread) {
                out.状态=I64原子准确特征出生应用状态::候选读取失败; out.Gread=query.Gread; return out;
            }
            const 原子I64特征出生读取请求 read{1,query.Gread,candidate.出生H,candidate.F,r.位置,
                                                  r.组织读取预算,r.出生概念读取预算};
            auto material=原子服务_.读取(read);
            out.候选读取组.push_back(material);
            if(!material.成功(read)) {
                out.状态=I64原子准确特征出生应用状态::候选读取失败; out.Gread=material.Gread; return out;
            }
        }
        out.Gread=query.Gread;
        if(out.候选读取组.size()>1) {
            out.状态=I64原子准确特征出生应用状态::需选择F;
            return out;
        }
        if(out.候选读取组.size()==1) {
            out.事实=out.候选读取组.front().事实;
            out.状态=I64原子准确特征出生应用状态::已复用;
            return out;
        }
        const 原子I64特征出生请求 create{1,current.Gread,r.位置,r.FT.编码,r.准确I64,
            out.FCv确保->FCv->概念,r.F出生键,r.候选读取预算,r.组织读取预算,r.出生概念读取预算};
        out.出生=原子服务_.创建(create);
        out.Gread=out.出生->Gread;
        if(!out.出生->成功()) {
            out.状态=映射出生状态(out.出生->状态);
            return out;
        }
        out.事实=out.出生->事实;
        out.状态=out.出生->状态==原子I64特征出生状态::已创建 ?
            I64原子准确特征出生应用状态::已创建 : I64原子准确特征出生应用状态::精确重复;
    } catch(const std::bad_alloc&) {
        out.状态=out.FCv确保&&out.FCv确保->成功() ?
            I64原子准确特征出生应用状态::出生失败 : I64原子准确特征出生应用状态::FCv确保失败;
    } catch(...) {
        out.状态=out.FCv确保&&out.FCv确保->成功() ?
            I64原子准确特征出生应用状态::出生失败 : I64原子准确特征出生应用状态::FCv确保失败;
    }
    return out;
}

} // namespace 海中鱼巣
