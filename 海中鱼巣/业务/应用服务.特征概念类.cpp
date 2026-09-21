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

bool 本能先天特征概念初始化结果::成功() const noexcept {
    if (版本 != 1 || 原请求.版本 != 1 || !Gread ||
        (状态 != 本能先天特征概念初始化状态::已形成
            && 状态 != 本能先天特征概念初始化状态::已恢复)
        || !安全值交付 || !服务值交付) return false;
    const auto valid = [&](const 先天I64特征概念交付& delivery, 本能值角色 role) {
        const auto& type = delivery.类型;
        const auto& conceptFact = delivery.完整域概念;
        const auto* definition = std::get_if<纯I64特征概念定义>(&conceptFact.定义);
        const auto maximum = std::numeric_limits<std::int64_t>::max();
        return delivery.角色 == role && delivery.Gread == Gread
            && delivery.FT首次H && delivery.概念首次H
            && delivery.FT首次H <= Gread && delivery.概念首次H <= Gread
            && 有效(type.身份) && type.规格.来源 == 特征类型来源::先天定义
            && !type.规格.外设提供者 && type.规格.单位 == type.身份.编码
            && type.规格.缩放分子 == 1 && type.规格.缩放分母 == 1
            && type.规格.允许集合 == std::vector<特征I64闭区间>{{0, maximum}}
            && !type.规格.域形成 && !type.规则
            && conceptFact.类别 == 相关概念类别::特征
            && conceptFact.治理状态 == 概念树生命周期状态::活跃
            && definition && definition->特征类型.值 == type.身份.编码
            && definition->规范域 == std::vector<概念树I64区间>{{0, maximum}}
            && conceptFact.直接上位.empty() && conceptFact.概念.值 != type.身份.编码;
    };
    return valid(*安全值交付, 本能值角色::安全值)
        && valid(*服务值交付, 本能值角色::服务值)
        && 安全值交付->类型.身份 != 服务值交付->类型.身份
        && 安全值交付->完整域概念.概念 != 服务值交付->完整域概念.概念;
}

本能先天特征概念初始化结果 本能先天特征概念初始化提供者::初始化(
    const 本能先天特征概念初始化请求& request) noexcept {
    本能先天特征概念初始化结果 out; out.原请求 = request;
    using V = 本能先天特征概念初始化状态;
    constexpr L1所有者范围写入幂等身份 安全FT键{0x494E'4654'5341'4631ULL};
    constexpr L1所有者范围写入幂等身份 服务FT键{0x494E'4654'5352'5631ULL};
    constexpr L1所有者范围写入幂等身份 安全概念键{0x494E'4346'5341'4631ULL};
    constexpr L1所有者范围写入幂等身份 服务概念键{0x494E'4346'5352'5631ULL};
    struct 前缀 final {
        本能值角色 角色;
        I64基础特征类型定义结果 FT;
        纯概念写入结果 概念;
    };
    try {
        if (request.版本 != 1 || !request.G0 || !request.概念预算.最大概念数
            || !request.概念预算.最大关系数 || !request.概念预算.最大特征属性数)
            return out;
        const auto initialCurrent = 概念服务_.读取当前事实代次();
        if (!initialCurrent.成功() || initialCurrent.Gread != request.G0) {
            out.状态 = V::当前性漂移; return out;
        }
        bool formed = false;
        const auto form = [&](本能值角色 role, L1所有者范围写入幂等身份 ftKey,
                              L1所有者范围写入幂等身份 conceptKey,
                              std::uint64_t g0) -> 前缀 {
            const auto maximum = std::numeric_limits<std::int64_t>::max();
            I64基础特征类型定义请求 ftRequest;
            ftRequest.G0 = g0; ftRequest.幂等身份 = ftKey;
            ftRequest.规格.来源 = 特征类型来源::先天定义;
            ftRequest.规格.单位绑定 = I64基础特征单位绑定::新FT自身;
            ftRequest.规格.缩放分子 = 1; ftRequest.规格.缩放分母 = 1;
            ftRequest.规格.允许集合 = {{0, maximum}};
            auto ft = 特征服务_.形成或读取I64基础特征类型(ftRequest);
            if (!ft.成功()) {
                if (ft.状态 == I64基础特征类型定义状态::当前性漂移) out.状态 = V::当前性漂移;
                else if (ft.状态 == I64基础特征类型定义状态::幂等冲突) out.状态 = V::幂等冲突;
                else if (ft.状态 == I64基础特征类型定义状态::已可能发布) out.状态 = V::已可能发布;
                else if (ft.状态 == I64基础特征类型定义状态::资源失败) out.状态 = V::资源失败;
                else out.状态 = V::FT失败;
                return {role, std::move(ft), {}};
            }
            formed = formed || ft.状态 == I64基础特征类型定义状态::已形成;
            const 纯I64特征概念定义 definition{
                概念树特征类型引用{ft.事实->数据.身份.编码}, {{0, maximum}}};
            const 纯概念创建恢复请求 recoveryRequest{2, ft.事实->Gread,
                conceptKey, definition, 概念初始组织指定::显式顶层, {}, request.概念预算};
            const auto recovered = 概念服务_.读取纯概念创建首次结果(recoveryRequest);
            纯概念写入结果 conceptResult;
            if (recovered.成功(recoveryRequest)) {
                const 纯概念创建请求 original{2, *recovered.首次实际G0, conceptKey,
                    definition, 概念初始组织指定::显式顶层, {}, request.概念预算};
                conceptResult.状态 = 纯概念状态::精确重复;
                conceptResult.发布 = 纯概念发布状态::确认发布;
                conceptResult.Gread = recovered.Gread;
                conceptResult.H = *recovered.首次发布H;
                conceptResult.首次发布H = recovered.首次发布H;
                conceptResult.原请求 = original;
                conceptResult.事实 = recovered.事实;
            } else if (recovered.状态 == 纯概念恢复状态::未派发) {
                const 纯概念创建请求 conceptRequest{2, recovered.Gread, conceptKey, definition,
                    概念初始组织指定::显式顶层, {}, request.概念预算};
                conceptResult = 概念服务_.创建或复用纯概念(conceptRequest);
                if (!conceptResult.成功(conceptRequest) || !conceptResult.首次发布H) {
                    if (conceptResult.状态 == 纯概念状态::事实代次漂移) out.状态 = V::当前性漂移;
                    else if (conceptResult.状态 == 纯概念状态::幂等冲突) out.状态 = V::幂等冲突;
                    else if (conceptResult.状态 == 纯概念状态::已可能发布) out.状态 = V::已可能发布;
                    else if (conceptResult.状态 == 纯概念状态::资源失败) out.状态 = V::资源失败;
                    else if (conceptResult.成功(conceptRequest)) out.状态 = V::首次材料不一致;
                    else out.状态 = V::概念失败;
                    return {role, std::move(ft), std::move(conceptResult)};
                }
            } else {
                if (recovered.状态 == 纯概念恢复状态::事实代次漂移) out.状态 = V::当前性漂移;
                else if (recovered.状态 == 纯概念恢复状态::幂等冲突) out.状态 = V::幂等冲突;
                else if (recovered.状态 == 纯概念恢复状态::目标已退出) out.状态 = V::类型或概念已退出;
                else if (recovered.状态 == 纯概念恢复状态::已可能发布) out.状态 = V::已可能发布;
                else if (recovered.状态 == 纯概念恢复状态::资源失败) out.状态 = V::资源失败;
                else out.状态 = V::概念失败;
                return {role, std::move(ft), std::move(conceptResult)};
            }
            formed = formed || conceptResult.状态 == 纯概念状态::已创建;
            return {role, std::move(ft), std::move(conceptResult)};
        };

        auto safety = form(本能值角色::安全值, 安全FT键, 安全概念键, request.G0);
        if (!safety.FT.成功() || !safety.概念.原请求 || !safety.概念.首次发布H) return out;
        auto service = form(本能值角色::服务值, 服务FT键, 服务概念键, safety.概念.Gread);
        if (!service.FT.成功() || !service.概念.原请求 || !service.概念.首次发布H) return out;

        const auto current = 概念服务_.读取当前事实代次();
        if (!current.成功() || current.Gread != service.概念.Gread) {
            out.状态 = V::当前性漂移; return out;
        }
        const auto finish = [&](const 前缀& prefix) -> std::optional<先天I64特征概念交付> {
            const auto type = 特征服务_.读取I64基础特征类型事实(
                {1, current.Gread, prefix.FT.事实->H, prefix.FT.事实->数据.身份});
            const auto* typeFact = std::get_if<特征截止事实<I64基础特征类型信息>>(&type);
            if (!typeFact || typeFact->Gread != current.Gread) return std::nullopt;
            const 纯概念读取请求 conceptRequest{2, current.Gread,
                *prefix.概念.首次发布H, prefix.概念.事实->概念, request.概念预算};
            const auto conceptResult = 概念服务_.读取纯概念(conceptRequest);
            if (!conceptResult.成功(conceptRequest) || !conceptResult.事实) return std::nullopt;
            return 先天I64特征概念交付{prefix.角色, typeFact->数据, *conceptResult.事实,
                typeFact->H, *prefix.概念.首次发布H, current.Gread};
        };
        out.安全值交付 = finish(safety); out.服务值交付 = finish(service);
        out.Gread = current.Gread;
        out.状态 = formed ? V::已形成 : V::已恢复;
        if (!out.成功()) {
            out.状态 = V::引用冲突; out.安全值交付.reset(); out.服务值交付.reset(); out.Gread = 0;
        }
    } catch (const std::bad_alloc&) { out.状态 = V::资源失败; }
      catch (const std::length_error&) { out.状态 = V::资源失败; }
      catch (...) { out.状态 = V::内部不一致; }
    return out;
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
    存在类数据服务& existences, const 本能先天特征概念初始化结果& initialization,
    实例特征结构诊断 diagnosis) noexcept
    : 特征服务_(features),概念服务_(concepts),原子服务_(atoms),
      存在服务_(existences),初始化交付_(initialization),结构诊断_(diagnosis) {}

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
        auto grouped = 归组(out.IFR->Gread, first ? out.IFR->Gread : out.IFR->H,
                            std::move(currentItems), first);
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
        grouped = 归组(out.IFR->Gread,
                       afterBirthFirst ? out.IFR->Gread : out.IFR->H,
                       std::move(currentItems), afterBirthFirst);
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

本能根I64实际F结果 特征概念应用服务::形成或读取本能根I64实际F(
    const 本能根I64实际F请求& r) noexcept {
    本能根I64实际F结果 out; out.原请求 = r;
    try {
        if (r.版本 != 1 || !r.G0 || !有效(r.E.编码) || !有效(r.位置.场景) || !有效(r.位置.组织父)
            || !有效(r.IFR键) || !有效(r.FCv概念键) || !有效(r.当前采用键)
            || !键有效且互异(r.FCv概念键, r.F出生键)
            || r.IFR键 == r.FCv概念键 || r.IFR键 == r.当前采用键
            || r.FCv概念键 == r.当前采用键 || !r.当前采用关系预算
            || !IFR预算有效(r.IFR预算) || !R规则预算有效(r.R规则预算)
            || (r.角色 != 本能值角色::安全值 && r.角色 != 本能值角色::服务值)) return out;
        const std::array<L1所有者范围写入幂等身份,5> birthKeys{
            r.F出生键.组合,r.F出生键.内容,r.F出生键.已知,r.F出生键.组织,r.F出生键.概念使用};
        for (const auto key : birthKeys)
            if (key == r.IFR键 || key == r.FCv概念键 || key == r.当前采用键) return out;
        if (!初始化交付_.成功()) {
            out.状态 = 本能根I64实际F状态::初始化交付不完整; return out;
        }
        const auto& delivered = r.角色 == 本能值角色::安全值
            ? *初始化交付_.安全值交付 : *初始化交付_.服务值交付;
        auto current = 概念服务_.读取当前事实代次();
        if (!current.成功() || current.Gread != r.G0) {
            out.状态 = 本能根I64实际F状态::事实代次漂移; return out;
        }
        const auto typeRead = 特征服务_.读取I64基础特征类型事实(
            {1,current.Gread,delivered.FT首次H,delivered.类型.身份});
        const auto* typeFact = std::get_if<特征截止事实<I64基础特征类型信息>>(&typeRead);
        const 纯概念读取请求 fullRequest{2,current.Gread,delivered.概念首次H,
            delivered.完整域概念.概念,r.概念预算};
        const auto full = 概念服务_.读取纯概念(fullRequest);
        const auto maximum = std::numeric_limits<std::int64_t>::max();
        if (!typeFact || typeFact->Gread != current.Gread || !full.成功(fullRequest) || !full.事实
            || typeFact->数据.规格.来源 != 特征类型来源::先天定义
            || typeFact->数据.规格.外设提供者 || typeFact->数据.规格.单位 != typeFact->数据.身份.编码
            || typeFact->数据.规格.缩放分子 != 1 || typeFact->数据.规格.缩放分母 != 1
            || typeFact->数据.规格.允许集合 != std::vector<特征I64闭区间>{{0,maximum}}
            || full.事实->概念 != delivered.完整域概念.概念 || !full.事实->直接上位.empty()) {
            out.状态 = 本能根I64实际F状态::初始化交付不完整; return out;
        }
        const auto* fullDefinition = std::get_if<纯I64特征概念定义>(&full.事实->定义);
        if (!fullDefinition || fullDefinition->特征类型.值 != typeFact->数据.身份.编码
            || fullDefinition->规范域 != std::vector<概念树I64区间>{{0,maximum}}) {
            out.状态 = 本能根I64实际F状态::初始化交付不完整; return out;
        }
        out.初始化交付 = 先天I64特征概念交付{r.角色,typeFact->数据,*full.事实,
            typeFact->H,delivered.概念首次H,current.Gread};
        const auto ft = typeFact->数据.身份;
        const auto initial = maximum / 2;
        auto adopted = 存在服务_.读取当前采用({1, current.Gread, current.Gread, r.E.编码, ft, r.当前采用关系预算});
        if (adopted.状态 != 存在类数据状态::已读取) { out.状态 = 本能根I64实际F状态::当前采用读取失败; return out; }
        const bool formed = !adopted.采用;
        特征信息身份 f{};
        if (adopted.采用) {
            f = adopted.采用->F;
        } else {
            const 实例特征IFR完整投影* projection = nullptr;
            out.IFR读回 = 存在服务_.读取实例特征IFR(
                {1,r.E,ft,current.Gread,r.IFR预算});
            if (out.IFR读回->状态 == 实例特征IFR状态::已读取 &&
                out.IFR读回->投影) {
                projection = &*out.IFR读回->投影;
            } else if (out.IFR读回->状态 == 实例特征IFR状态::未找到) {
                const 实例特征R观察请求 observe{1, r.E, r.位置, ft,
                    特征准确值{initial}, current.Gread, r.IFR键, r.F出生键,
                    r.FCv概念键, r.IFR预算, r.R规则预算, r.候选预算,
                    r.组织预算, r.概念读取预算, r.概念预算};
                out.IFR = 处理实例特征R观察(observe);
                if (!out.IFR ||
                    (out.IFR->状态 != 实例特征R观察状态::已发布IFR &&
                     out.IFR->状态 != 实例特征R观察状态::IFR未变更) ||
                    !out.IFR->IFR || !out.IFR->IFR->投影) {
                    out.状态 = 本能根I64实际F状态::IFR失败; return out;
                }
                current = 概念服务_.读取当前事实代次();
                if (!current.成功() || current.Gread != out.IFR->Gread) {
                    out.状态 = 本能根I64实际F状态::事实代次漂移; return out;
                }
                projection = &*out.IFR->IFR->投影;
            } else {
                out.状态 = 本能根I64实际F状态::IFR失败; return out;
            }
            std::vector<特征信息身份> matches;
            for (const auto& x : projection->F成员) {
                const auto read = 特征服务_.读取准确特征事实({1, current.Gread, current.Gread, x});
                if (const auto* fact = std::get_if<准确特征读取事实>(&read)) {
                    if (fact->信息.类型 == ft && std::get_if<std::int64_t>(&fact->信息.准确值)
                        && *std::get_if<std::int64_t>(&fact->信息.准确值) == initial) matches.push_back(x);
                }
            }
            if (matches.size() != 1) { out.状态 = 本能根I64实际F状态::引用冲突; return out; }
            f = matches.front();
            out.当前采用 = 存在服务_.变更当前采用({1, current.Gread, r.当前采用键, r.E.编码, ft,
                存在关联已知并采用{f, std::nullopt}, r.当前采用关系预算});
            if (!out.当前采用->成功() || !out.当前采用->采用) { out.状态 = 本能根I64实际F状态::当前采用写入失败; return out; }
            current = 概念服务_.读取当前事实代次();
            if (!current.成功() || current.Gread != out.当前采用->Gread) { out.状态 = 本能根I64实际F状态::事实代次漂移; return out; }
        }
        adopted = 存在服务_.读取当前采用(
            {1,current.Gread,current.Gread,r.E.编码,ft,r.当前采用关系预算});
        if (adopted.状态 != 存在类数据状态::已读取 || !adopted.采用 || adopted.采用->F != f) {
            out.状态 = 本能根I64实际F状态::当前采用读取失败; return out;
        }
        out.IFR读回 = 存在服务_.读取实例特征IFR({1,r.E,ft,current.Gread,r.IFR预算});
        if (out.IFR读回->状态 != 实例特征IFR状态::已读取 || !out.IFR读回->投影
            || out.IFR读回->Gread != current.Gread
            || std::count(out.IFR读回->投影->F成员.begin(),out.IFR读回->投影->F成员.end(),f) != 1) {
            out.状态 = 本能根I64实际F状态::IFR失败; return out;
        }
        const auto exact = 特征服务_.读取准确特征事实({1, current.Gread, current.Gread, f});
        const auto* fact = std::get_if<准确特征读取事实>(&exact);
        if (!fact || fact->信息.类型 != ft || !std::holds_alternative<std::int64_t>(fact->信息.准确值)
            || std::get<std::int64_t>(fact->信息.准确值) != initial) { out.状态 = 本能根I64实际F状态::引用冲突; return out; }
        const 原子I64特征出生读取请求 atomRequest{1,current.Gread,fact->创建G,f.编码,r.位置,
            r.组织预算,r.概念读取预算};
        out.原子读回 = 原子服务_.读取(atomRequest);
        if (!out.原子读回->成功(atomRequest) || !out.原子读回->事实
            || out.原子读回->事实->F != f.编码 || out.原子读回->事实->正式特征类型 != ft.编码
            || out.原子读回->事实->准确I64 != initial) {
            out.状态 = 本能根I64实际F状态::引用冲突; return out;
        }
        const auto singleConcept = 概念树概念身份{out.原子读回->事实->FCv};
        const 纯概念读取请求 singleRequest{2,current.Gread,current.Gread,singleConcept,r.概念预算};
        out.单值概念读回 = 概念服务_.读取纯概念(singleRequest);
        if (!out.单值概念读回->成功(singleRequest) || !out.单值概念读回->事实) {
            out.状态 = 本能根I64实际F状态::引用冲突; return out;
        }
        const auto* singleDefinition = std::get_if<纯I64特征概念定义>(&out.单值概念读回->事实->定义);
        const bool hasFullParent = std::any_of(out.单值概念读回->事实->直接上位.begin(),
            out.单值概念读回->事实->直接上位.end(),[&](const auto& edge) {
                return edge.上位 == full.事实->概念;
            });
        if (!singleDefinition || singleDefinition->特征类型.值 != ft.编码
            || singleDefinition->规范域 != std::vector<概念树I64区间>{{initial,initial}}
            || !hasFullParent || singleConcept == full.事实->概念) {
            out.状态 = 本能根I64实际F状态::引用冲突; return out;
        }
        out.当前采用 = adopted; out.准确F = *fact; out.实际F = f;
        out.状态 = formed ? 本能根I64实际F状态::已形成 : 本能根I64实际F状态::已读取;
    } catch (const std::bad_alloc&) { out.状态 = 本能根I64实际F状态::资源失败; }
      catch (const std::length_error&) { out.状态 = 本能根I64实际F状态::资源失败; }
      catch (...) { out.状态 = 本能根I64实际F状态::内部不一致; }
    if (!out.成功()) {
        out.准确F.reset(); out.实际F.reset(); out.原子读回.reset();
        out.单值概念读回.reset(); out.IFR读回.reset();
    }
    return out;
}

} // namespace 海中鱼巣
