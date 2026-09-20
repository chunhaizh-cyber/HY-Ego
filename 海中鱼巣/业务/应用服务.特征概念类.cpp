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

bool IFR预算有效(const 实例特征IFR读取预算& b) noexcept {
    return b.最大F成员数 && b.最大R项数 && b.最大关系数 &&
        b.最大材料值数 && b.最大单材料U64元素数;
}

bool R规则预算有效(const 特征R规则读取预算& b) noexcept {
    return b.最大规则节点数 && b.最大规则关系数 && b.最大规则值数 &&
        b.最大R项数 && b.最大R成员数 && b.最大材料U64项数 &&
        b.最大候选值元素数;
}

void 发出结构诊断(实例特征结构诊断 callback, 存在信息身份 e,
    特征类型身份 ft, 实例特征结构异常 issue, std::uint64_t g,
    std::uint64_t h) noexcept {
    if (!callback) return;
    try { callback(e, ft, issue, g, h); } catch (...) {}
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

bool 特征概念应用服务::请求有效(const 实例特征R观察请求& r) noexcept {
    if (r.版本 != 1 || !有效(r.E.编码) || !有效(r.FT) || !有效(r.IFR键) ||
        !IFR预算有效(r.IFR预算) || !R规则预算有效(r.R规则预算)) return false;
    const auto* value = std::get_if<std::int64_t>(&r.候选值);
    if (!value) return true;
    const I64原子准确特征出生应用请求 birth{
        1, r.G0, r.位置, r.FT, *value, r.FCv概念键,
        r.F出生键, r.候选预算, r.组织预算, r.概念读取预算, r.概念预算};
    return 请求有效(birth);
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
    概念树类数据服务& concepts, 原子I64特征出生数据服务& atoms,
    存在类数据服务& existences, 实例特征结构诊断 diagnosis) noexcept
    : 特征服务_(features),概念服务_(concepts),原子服务_(atoms),
      存在服务_(existences),结构诊断_(diagnosis) {}

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

实例特征R观察结果 特征概念应用服务::处理实例特征R观察(
    const 实例特征R观察请求& r) noexcept {
    using IFR状态 = 实例特征IFR状态;
    using 观察状态 = 实例特征R观察状态;

    实例特征R观察结果 out;
    out.Gread = r.G0;
    std::optional<实例特征结构异常> diagnostic;
    auto finish = [&]() noexcept -> 实例特征R观察结果 {
        if (diagnostic)
            发出结构诊断(结构诊断_, r.E, r.FT, *diagnostic, out.Gread, r.G0);
        return out;
    };
    if (!请求有效(r)) return finish();

    const auto* candidate = std::get_if<std::int64_t>(&r.候选值);
    if (!candidate) {
        out.状态 = 观察状态::比较未启用;
        return finish();
    }

    auto readIFR = [&](std::uint64_t g) {
        return 存在服务_.读取实例特征IFR({1, r.E, r.FT, g, r.IFR预算});
    };

    auto 归组输入 = [&](const 实例特征IFR完整投影& projection,
                       std::uint64_t g, std::uint64_t h,
                       std::vector<特征R规则项投影>& items) noexcept -> bool {
        try {
            if (projection.E != r.E || projection.FT != r.FT || projection.R项.empty()) {
                diagnostic = projection.R项.empty() ? 实例特征结构异常::空R项 :
                    实例特征结构异常::半结构;
                return false;
            }
            std::vector<稳定编码> allMembers;
            std::vector<稳定编码> expectedMembers;
            expectedMembers.reserve(projection.F成员.size());
            for (const auto& member : projection.F成员) {
                if (!有效(member)) {
                    diagnostic = 实例特征结构异常::半结构;
                    return false;
                }
                expectedMembers.push_back(member.编码);
            }
            std::sort(expectedMembers.begin(), expectedMembers.end());
            if (std::adjacent_find(expectedMembers.begin(), expectedMembers.end()) != expectedMembers.end()) {
                diagnostic = 实例特征结构异常::半结构;
                return false;
            }

            items.clear();
            items.reserve(projection.R项.size());
            稳定编码 previousR{};
            for (const auto& source : projection.R项) {
                if (!有效(source.R项.编码) || (有效(previousR) && !(previousR < source.R项.编码)) ||
                    source.形成成员.empty()) {
                    diagnostic = !有效(source.R项.编码) || source.形成成员.empty() ?
                        实例特征结构异常::半结构 : 实例特征结构异常::重复R项;
                    return false;
                }
                if (source.材料.规范化U64组.empty()) {
                    diagnostic = 实例特征结构异常::缺材料;
                    return false;
                }
                previousR = source.R项.编码;
                特征R规则项投影 item;
                item.R = source.R项.编码;
                item.材料 = source.材料;
                item.形成成员.reserve(source.形成成员.size());
                稳定编码 previousF{};
                for (const auto& member : source.形成成员) {
                    if (!有效(member) || (有效(previousF) && !(previousF < member.编码))) {
                        diagnostic = 实例特征结构异常::半结构;
                        return false;
                    }
                    previousF = member.编码;
                    allMembers.push_back(member.编码);
                    const 原子I64特征出生使用读取请求 useRequest{
                        1, g, h, member.编码, r.概念读取预算};
                    const auto use = 概念服务_.读取原子I64出生使用(useRequest);
                    if (!use.成功(useRequest) || !use.事实 || use.事实->F != member ||
                        !有效(use.事实->FCv.值)) {
                        switch (use.状态) {
                        case 原子I64特征出生使用读取状态::未找到:
                        case 原子I64特征出生使用读取状态::目标已退出:
                        case 原子I64特征出生使用读取状态::概念不适配:
                        case 原子I64特征出生使用读取状态::内部不一致:
                            diagnostic = 实例特征结构异常::半结构;
                            break;
                        default:
                            break;
                        }
                        return false;
                    }
                    item.形成成员.push_back({member, use.事实->FCv.值});
                }
                items.push_back(std::move(item));
            }
            std::sort(allMembers.begin(), allMembers.end());
            if (std::adjacent_find(allMembers.begin(), allMembers.end()) != allMembers.end() ||
                allMembers != expectedMembers) {
                diagnostic = 实例特征结构异常::半结构;
                return false;
            }
            return true;
        } catch (...) {
            diagnostic = 实例特征结构异常::半结构;
            return false;
        }
    };

    auto 归组 = [&](std::uint64_t g, std::uint64_t h,
                    std::vector<特征R规则项投影> current,
                    bool first) noexcept {
        auto budget = r.R规则预算;
        if (first) {
            budget.最大R项数 = 0;
            budget.最大R成员数 = 0;
            budget.最大材料U64项数 = 0;
        }
        return 特征服务_.归组特征R({1, g, h, r.FT, r.候选值,
                                       std::move(current), std::move(budget)});
    };

    auto 构造目标 = [&](const std::optional<实例特征IFR完整投影>& current,
                       std::optional<稳定编码> updatedR,
                       const 特征R区间材料& updatedMaterial,
                       std::optional<特征信息身份> additionalF) {
        std::vector<实例特征IFR目标项> target;
        if (current) {
            target.reserve(current->R项.size() + (additionalF ? 1U : 0U));
            for (const auto& source : current->R项) {
                实例特征IFR目标项 item{source.材料, source.形成成员};
                if (updatedR && source.R项.编码 == *updatedR) item.材料 = updatedMaterial;
                target.push_back(std::move(item));
            }
        }
        if (additionalF) target.push_back({updatedMaterial, {*additionalF}});
        std::sort(target.begin(), target.end(), [](const auto& left, const auto& right) {
            return left.形成成员.front().编码 < right.形成成员.front().编码;
        });
        return target;
    };

    auto 提交 = [&](std::uint64_t g, std::optional<实例特征IFR完整投影> expected,
                    std::vector<实例特征IFR目标项> target) {
        out.IFR = 存在服务_.提交实例特征IFR目标结构(
            {1, r.E, r.FT, g, std::move(expected), std::move(target), r.IFR键, r.IFR预算});
        out.Gread = out.IFR->Gread;
        switch (out.IFR->状态) {
        case IFR状态::已发布:
        case IFR状态::精确重复:
            out.状态 = 观察状态::已发布IFR;
            break;
        case IFR状态::无须变更:
            out.状态 = 观察状态::IFR未变更;
            break;
        default:
            out.状态 = 观察状态::IFR写入失败;
            break;
        }
    };

    try {
        out.IFR = readIFR(r.G0);
        out.Gread = out.IFR->Gread;
        const bool first = out.IFR->状态 == IFR状态::未找到;
        if (!first && (out.IFR->状态 != IFR状态::已读取 || !out.IFR->投影)) {
            if (out.IFR->状态 == IFR状态::内部不一致) {
                diagnostic = 实例特征结构异常::半结构;
                out.状态 = 观察状态::结构异常;
            } else {
                out.状态 = 观察状态::读取失败;
            }
            return finish();
        }

        std::vector<特征R规则项投影> currentItems;
        if (!first && !归组输入(*out.IFR->投影, out.IFR->Gread, out.IFR->H, currentItems)) {
            out.状态 = diagnostic ? 观察状态::结构异常 : 观察状态::归组失败;
            return finish();
        }
        auto grouped = 归组(out.IFR->Gread, out.IFR->H, std::move(currentItems), first);
        out.Gread = grouped.Gread;
        if (grouped.状态 == 特征R规则状态::规则未启用) {
            out.状态 = 观察状态::比较未启用;
            return finish();
        }
        if (!grouped.规范化材料 ||
            (grouped.状态 != 特征R规则状态::唯一命中 &&
             grouped.状态 != 特征R规则状态::形成新R)) {
            out.状态 = 观察状态::归组失败;
            return finish();
        }

        if (grouped.状态 == 特征R规则状态::唯一命中) {
            if (first || !grouped.命中R) {
                diagnostic = 实例特征结构异常::多重命中;
                out.状态 = 观察状态::结构异常;
                return finish();
            }
            提交(out.IFR->Gread, out.IFR->投影,
                构造目标(out.IFR->投影, grouped.命中R, *grouped.规范化材料, std::nullopt));
            return finish();
        }

        const I64原子准确特征出生应用请求 birthRequest{
            1, out.IFR->Gread, r.位置, r.FT, *candidate, r.FCv概念键,
            r.F出生键, r.候选预算, r.组织预算, r.概念读取预算, r.概念预算};
        out.F出生 = 处理I64原子准确特征出生(birthRequest);
        out.Gread = out.F出生->Gread;
        if (!out.F出生->成功() || !out.F出生->事实) {
            out.状态 = 观察状态::F出生失败;
            return finish();
        }

        out.IFR = readIFR(out.F出生->Gread);
        out.Gread = out.IFR->Gread;
        const bool afterBirthFirst = out.IFR->状态 == IFR状态::未找到;
        if (!afterBirthFirst && (out.IFR->状态 != IFR状态::已读取 || !out.IFR->投影)) {
            if (out.IFR->状态 == IFR状态::内部不一致) {
                diagnostic = 实例特征结构异常::半结构;
                out.状态 = 观察状态::结构异常;
            } else {
                out.状态 = 观察状态::读取失败;
            }
            return finish();
        }

        currentItems.clear();
        if (!afterBirthFirst && !归组输入(*out.IFR->投影, out.IFR->Gread,
                                               out.IFR->H, currentItems)) {
            out.状态 = diagnostic ? 观察状态::结构异常 : 观察状态::归组失败;
            return finish();
        }
        grouped = 归组(out.IFR->Gread, out.IFR->H, std::move(currentItems), afterBirthFirst);
        out.Gread = grouped.Gread;
        if (grouped.状态 == 特征R规则状态::规则未启用) {
            out.状态 = 观察状态::比较未启用;
            return finish();
        }
        if (!grouped.规范化材料 ||
            (grouped.状态 != 特征R规则状态::唯一命中 &&
             grouped.状态 != 特征R规则状态::形成新R)) {
            out.状态 = 观察状态::归组失败;
            return finish();
        }
        if (grouped.状态 == 特征R规则状态::唯一命中) {
            if (afterBirthFirst || !grouped.命中R) {
                diagnostic = 实例特征结构异常::多重命中;
                out.状态 = 观察状态::结构异常;
                return finish();
            }
            提交(out.IFR->Gread, out.IFR->投影,
                构造目标(out.IFR->投影, grouped.命中R, *grouped.规范化材料, std::nullopt));
        } else {
            提交(out.IFR->Gread, afterBirthFirst ? std::nullopt : out.IFR->投影,
                构造目标(afterBirthFirst ? std::optional<实例特征IFR完整投影>{} : out.IFR->投影,
                           std::nullopt, *grouped.规范化材料, 特征信息身份{out.F出生->事实->F}));
        }
    } catch (...) {
        out.状态 = 观察状态::归组失败;
    }
    return finish();
}

} // namespace 海中鱼巣
