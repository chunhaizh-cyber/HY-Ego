#include "转换.语言场景共同.h"

#include <algorithm>
#include <array>
#include <new>
#include <set>
#include <stdexcept>
#include <tuple>

namespace 海中鱼巣::语言场景转换内部 {
namespace {
读取状态 名称状态(概念名称关系查询状态 状态) {
    switch (状态) {
    case 概念名称关系查询状态::已找到: return 读取状态::已完成;
    case 概念名称关系查询状态::未找到: return 读取状态::缺名称;
    case 概念名称关系查询状态::输入不合法: return 读取状态::输入拒绝;
    case 概念名称关系查询状态::结构不一致: return 读取状态::内部不一致;
    case 概念名称关系查询状态::读取漂移:
    case 概念名称关系查询状态::依赖已变化: return 读取状态::读取漂移;
    case 概念名称关系查询状态::资源失败:
    case 概念名称关系查询状态::工程未完成: return 读取状态::工程未完成;
    default: return 读取状态::引用不可读;
    }
}
读取状态 指代状态(指代名称读取状态 状态) {
    if (状态 == 指代名称读取状态::资源失败 || 状态 == 指代名称读取状态::工程未完成)
        return 读取状态::工程未完成;
    if (状态 == 指代名称读取状态::结构不一致) return 读取状态::内部不一致;
    if (状态 == 指代名称读取状态::读取漂移 || 状态 == 指代名称读取状态::依赖已变化)
        return 读取状态::读取漂移;
    return 读取状态::引用不可读;
}
读取状态 知识状态(新语言知识状态 状态) {
    switch (状态) {
    case 新语言知识状态::已完成: return 读取状态::已完成;
    case 新语言知识状态::未找到: return 读取状态::缺规则;
    case 新语言知识状态::额度不足: return 读取状态::额度不足;
    case 新语言知识状态::输入拒绝: return 读取状态::输入拒绝;
    case 新语言知识状态::读取漂移: return 读取状态::读取漂移;
    case 新语言知识状态::内部不一致: return 读取状态::内部不一致;
    case 新语言知识状态::工程未完成: return 读取状态::工程未完成;
    default: return 读取状态::引用不可读;
    }
}
名称读取结果 读取名称组(const std::vector<稳定编码>& 节点组,
    std::optional<稳定编码> 词条限定, std::optional<稳定编码> 概念限定,
    稳定编码 语言节点, const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在服务, const 概念_特征类& 特征服务) {
    名称读取结果 结果{读取状态::已完成, {}};
    std::set<稳定编码> 已见;
    for (const auto 节点 : 节点组) {
        if (!有效(节点) || !已见.insert(节点).second) return {读取状态::内部不一致, {}};
        const auto 指代 = 名称服务.获取指代名称关系(节点);
        if (指代.状态 == 指代名称读取状态::已读取) {
            if (!指代.信息
                || (词条限定 && 指代.信息->词条.节点 != *词条限定)
                || (概念限定 && 指代.信息->概念.节点 != *概念限定))
                return {读取状态::内部不一致, {}};
            continue;
        }
        if (指代.状态 != 指代名称读取状态::未找到) return {指代状态(指代.状态), {}};
        const auto 名称 = 名称服务.获取名称关系(节点);
        if (名称.状态 == 概念名称关系查询状态::未找到) return {读取状态::读取漂移, {}};
        if (名称.状态 != 概念名称关系查询状态::已找到) return {名称状态(名称.状态), {}};
        if (!名称.信息
            || (词条限定 && 名称.信息->词条节点 != *词条限定)
            || (概念限定 && 名称.信息->概念节点 != *概念限定))
            return {读取状态::内部不一致, {}};
        const auto 词 = 词服务.获取词条(名称.信息->词条节点);
        if (词.状态 != 新词条查询状态::已找到 || !词.信息)
            return {词.状态 == 新词条查询状态::资源失败 ? 读取状态::工程未完成 : 读取状态::引用不可读, {}};
        const auto E = 存在服务.获取存在概念(名称.信息->概念节点);
        const auto F = 特征服务.获取特征概念(名称.信息->概念节点);
        if (E.has_value() == F.has_value()) return {读取状态::引用不可读, {}};
        if (词.信息->语言节点 != 语言节点) continue;
        if (E) 结果.完整普通名称材料组.push_back({*词.信息, *名称.信息, *E});
        else 结果.完整普通名称材料组.push_back({*词.信息, *名称.信息, *F});
    }
    std::sort(结果.完整普通名称材料组.begin(), 结果.完整普通名称材料组.end(),
        [](const auto& a, const auto& b) {
            return std::tie(a.名称关系.概念节点, a.名称关系.节点, a.词条.节点)
                < std::tie(b.名称关系.概念节点, b.名称关系.节点, b.词条.节点);
        });
    return 结果;
}
} // namespace

namespace {
bool 依据快照相同(const 基础联合节点快照& a,const 基础联合节点快照& b) {
    return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组
        &&a.入向字段组==b.入向字段组&&a.入向外部关系组==b.入向外部关系组
        &&a.出向外部关系组==b.出向外部关系组;
}
bool 正式候选相同(const 概念名称候选正式信息& a,const 概念名称候选正式信息& b) {
    if(a.index()!=b.index())return false;
    if(const auto*x=std::get_if<概念名称关系信息>(&a))return *x==std::get<概念名称关系信息>(b);
    const auto&x=std::get<指代名称关系信息>(a);const auto&y=std::get<指代名称关系信息>(b);
    return x.节点==y.节点&&x.概念==y.概念&&x.词条==y.词条&&x.语言==y.语言&&x.角色==y.角色
        &&x.来源.节点==y.来源.节点&&x.来源.种类==y.来源.种类
        &&x.来源.语言节点==y.来源.语言节点&&x.来源.固定原文==y.来源.固定原文;
}
bool 正式候选组相同(const std::vector<概念名称候选正式信息>& a,
    const std::vector<概念名称候选正式信息>& b) {
    return a.size()==b.size()&&std::equal(a.begin(),a.end(),b.begin(),正式候选相同);
}
}
bool 特征定义语义相同(const 特征概念信息& a,const 特征概念信息& b) {
    auto left=a,right=b;
    left.当前引用实例数量=0;right.当前引用实例数量=0;
    return left==right;
}
bool 特征依据语义相同(const 语言特征概念读取依据& a,const 语言特征概念读取依据& b) {
    if(a.概念节点!=b.概念节点||a.信息.类型根!=b.信息.类型根
        ||!特征定义语义相同(a.信息.定义,b.信息.定义)
        ||!有效(a.分类根节点)||a.分类根节点!=b.分类根节点
        ||a.根链组.empty()||a.根链组.size()!=b.根链组.size())return false;
    return std::equal(a.根链组.begin(),a.根链组.end(),b.根链组.begin(),[](const auto& x,const auto& y) {
        return x.信息.定义==y.信息.定义&&x.信息.类型根==y.信息.类型根
            &&x.直接上位概念节点==y.直接上位概念节点;
    });
}
void 解释读取依据::并保护(const std::vector<基础联合节点快照>& 输入) {
    if(输入.empty()){失败=读取状态::内部不一致;return;}
    for(const auto& s:输入) {
        const auto i=std::find_if(完整保护.begin(),完整保护.end(),[&](const auto&x){return x.结构.节点.编码==s.结构.节点.编码;});
        if(i==完整保护.end())完整保护.push_back(s);
        else if(!依据快照相同(*i,s))失败=读取状态::读取漂移;
    }
}
新词条查询结果 解释读取依据::查词(稳定编码 lang,稳定编码 contract,const std::string& text) {
    const auto p=词.按规范化词面查询保护(lang,contract,text);
    if(p.状态!=新词条保护状态::已读取&&p.状态!=新词条保护状态::未找到) {
        if(p.状态==新词条保护状态::前提已变化)失败=读取状态::读取漂移;
        else if(p.状态==新词条保护状态::资源失败)失败=读取状态::工程未完成;
        else 失败=读取状态::输入拒绝;
        return {p.状态==新词条保护状态::资源失败 ? 新词条查询状态::资源失败:新词条查询状态::结构不一致,{}};
    }
    并保护(p.完整保护);
    const auto i=std::find_if(词面查询依据组.begin(),词面查询依据组.end(),[&](const auto&x){return x.语言节点==lang&&x.规范化合同节点==contract&&x.规范化词面==text;});
    if(i==词面查询依据组.end())词面查询依据组.push_back({lang,contract,text,p.信息});
    else if(i->命中词条!=p.信息)失败=读取状态::读取漂移;
    return {p.状态==新词条保护状态::已读取 ? 新词条查询状态::已找到:新词条查询状态::未找到,p.信息};
}
新词条查询结果 解释读取依据::读词(稳定编码 id) {
    const auto p=词.读取词条保护(id);
    if(p.状态==新词条保护状态::已读取&&p.信息) {
        并保护(p.完整保护);
        const auto r=查词(p.信息->语言节点,p.信息->规范化合同节点,p.信息->规范化词面);
        if(r.信息!=p.信息)失败=读取状态::读取漂移;
        return {新词条查询状态::已找到,p.信息};
    }
    if(p.状态==新词条保护状态::资源失败){失败=读取状态::工程未完成;return {新词条查询状态::资源失败,{}};}
    if(p.状态==新词条保护状态::前提已变化)失败=读取状态::读取漂移;
    else 失败=读取状态::引用不可读;
    return {新词条查询状态::结构不一致,{}};
}
概念名称关系列表查询结果 解释读取依据::读名称(稳定编码 term) {
    const auto p=名称.读取词条名称候选保护(term);
    if(p.状态!=概念名称关系查询状态::已找到&&p.状态!=概念名称关系查询状态::未找到) {
        失败=名称状态(p.状态);return {p.状态,{}};
    }
    并保护(p.完整保护);
    const auto i=std::find_if(名称候选依据组.begin(),名称候选依据组.end(),[&](const auto&x){return x.词条节点==term;});
    if(i==名称候选依据组.end())名称候选依据组.push_back({term,p.候选组});
    else if(!正式候选组相同(i->完整候选组,p.候选组))失败=读取状态::读取漂移;
    std::vector<稳定编码> ids;
    for(const auto& item:p.候选组)ids.push_back(std::visit([](const auto&v){return v.节点;},item));
    return {p.状态,std::move(ids)};
}
void 解释读取依据::读特征(稳定编码 id) {
    if(失败!=读取状态::已完成)return;
    if(!有效(id))throw std::logic_error("feature evidence requires a valid located concept");
    try {
        语言特征概念读取依据 result;result.概念节点=id;
        const auto merge=[&](const std::vector<基础联合节点快照>& nodes) {
            if(nodes.empty())throw std::logic_error("feature chain reader returned no protection");
            并保护(nodes);
            if(失败!=读取状态::已完成)throw 失败;
            for(const auto& n:nodes) {
                const auto i=std::find_if(result.完整保护.begin(),result.完整保护.end(),
                    [&](const auto& x){return x.结构.节点.编码==n.结构.节点.编码;});
                if(i==result.完整保护.end())result.完整保护.push_back(n);
            }
        };
        const auto category=特征.读取特征类别根概念保护();
        if(category.状态==正式概念读取状态::前提已变化)throw 读取状态::读取漂移;
        if(category.状态==正式概念读取状态::依赖未就绪)throw 读取状态::引用不可读;
        if(category.状态!=正式概念读取状态::已读取||!category.信息||!有效(category.信息->节点))
            throw std::logic_error("feature category root evidence is corrupt");
        result.分类根节点=category.信息->节点;merge(category.完整保护组);
        const auto readDefinition=[&](稳定编码 current) {
            const auto p=特征.读取特征概念定义保护(current);
            if(p.前提已变化)throw 读取状态::读取漂移;
            if(p.状态==基础结构操作状态::资源失败)throw 读取状态::工程未完成;
            if(p.状态==基础结构操作状态::节点不存在||p.状态==基础结构操作状态::引用不存在)
                throw 读取状态::读取漂移;
            if(p.状态!=基础结构操作状态::已完成||!p.信息
                ||p.信息->定义.节点!=current||!有效(p.信息->类型根))
                throw std::logic_error("feature chain definition evidence is corrupt");
            merge(p.预期节点组);return *p.信息;
        };
        result.信息=readDefinition(id);
        std::set<稳定编码> seen;
        auto current=id;
        for(;;) {
            if(current==result.分类根节点||!seen.insert(current).second)
                throw std::logic_error("feature chain has a cycle or a category definition");
            auto definition=readDefinition(current);
            if(definition.类型根!=result.信息.类型根)
                throw std::logic_error("feature chain changes its type root");
            if(current==id&&!特征定义语义相同(definition.定义,result.信息.定义))throw 读取状态::读取漂移;
            const auto relation=特征.读取特征分类关系保护(current);
            if(relation.状态==特征概念规则状态::前提已变化)throw 读取状态::读取漂移;
            if(relation.状态==特征概念规则状态::资源失败)throw 读取状态::工程未完成;
            if(relation.状态==特征概念规则状态::读取接口未装配)throw 读取状态::引用不可读;
            if(relation.状态==特征概念规则状态::入口拒绝)throw 读取状态::读取漂移;
            if(relation.状态!=特征概念规则状态::已完成||!relation.信息
                ||relation.信息->概念节点!=current||relation.信息->是类别根
                ||relation.信息->直接上位概念组.size()!=1||!有效(relation.信息->直接上位概念组.front()))
                throw std::logic_error("feature chain direct parent evidence is corrupt");
            merge(relation.完整保护组);
            const auto parent=relation.信息->直接上位概念组.front();
            definition.定义.当前引用实例数量=0;
            result.根链组.push_back({std::move(definition),parent});
            if(current==result.信息.类型根) {
                if(parent!=result.分类根节点)throw std::logic_error("feature type root has a non-category parent");
                break;
            }
            if(parent==result.分类根节点)throw std::logic_error("feature chain ended before its type root");
            current=parent;
        }
        const auto i=std::find_if(特征概念依据组.begin(),特征概念依据组.end(),[&](const auto& x){return x.概念节点==id;});
        if(i==特征概念依据组.end())特征概念依据组.push_back(std::move(result));
        else if(!特征依据语义相同(*i,result))失败=读取状态::读取漂移;
    }catch(读取状态 state){失败=state;}
    catch(const std::bad_alloc&){失败=读取状态::工程未完成;}
    catch(const std::length_error&){失败=读取状态::工程未完成;}
}
void 解释读取依据::核特征读取(稳定编码 id,const 特征概念信息& 原读) {
    读特征(id);
    if(失败!=读取状态::已完成)return;
    const auto i=std::find_if(特征概念依据组.begin(),特征概念依据组.end(),[&](const auto&x){return x.概念节点==id;});
    if(i==特征概念依据组.end()||!特征定义语义相同(i->信息.定义,原读))失败=读取状态::读取漂移;
}
新语言词条匹配结果 解释读取依据::匹配词条(稳定编码 lang,const std::string& text) {
    const auto r=语言.按已登记词条最长匹配(lang,text,词);
    if(r.状态!=新语言查询状态::已找到)return r;
    // 最长匹配实际读取本语言全词典；保留其成员/负查询依据，而非只保留选中的词。
    const auto all=词.查询语言词条(lang);
    if(all.状态!=新词条查询状态::已找到&&all.状态!=新词条查询状态::未找到){失败=all.状态==新词条查询状态::资源失败 ? 读取状态::工程未完成:读取状态::引用不可读;return r;}
    for(auto id:all.词条节点组)(void)读词(id);
    for(const auto& p:r.片段组)if(!p.词条节点&&!p.原文.empty())
        (void)查词(lang,严格UTF8字节精确规范化合同节点,p.原文);
    const auto 再读=语言.按已登记词条最长匹配(lang,text,词);
    if(再读.状态!=新语言查询状态::已找到||再读.片段组!=r.片段组)
        失败=再读.状态==新语言查询状态::资源失败 ? 读取状态::工程未完成:读取状态::读取漂移;
    return r;
}
void 解释读取依据::合并依据(const std::vector<语言特征概念读取依据>& fs,
    const std::vector<语言词面查询依据>& ws,const std::vector<语言名称候选读取依据>& ns) {
    for(const auto& p:ws) {
        const auto r=查词(p.语言节点,p.规范化合同节点,p.规范化词面);
        if(失败!=读取状态::已完成)return;
        if(r.信息!=p.命中词条){失败=读取状态::读取漂移;return;}
    }
    for(const auto& p:ns) {
        (void)读名称(p.词条节点);
        if(失败!=读取状态::已完成)return;
        const auto i=std::find_if(名称候选依据组.begin(),名称候选依据组.end(),
            [&](const auto&x){return x.词条节点==p.词条节点;});
        if(i==名称候选依据组.end()||!正式候选组相同(i->完整候选组,p.完整候选组)) {
            失败=读取状态::读取漂移;return;
        }
    }
    for(const auto& p:fs) {
        读特征(p.概念节点);
        if(失败!=读取状态::已完成)return;
        const auto i=std::find_if(特征概念依据组.begin(),特征概念依据组.end(),
            [&](const auto&x){return x.概念节点==p.概念节点;});
        if(i==特征概念依据组.end()||!特征依据语义相同(*i,p)) {
            失败=读取状态::读取漂移;return;
        }
    }
}

std::optional<原文映射> 解码语言场景原文(const std::string& 原文) {
    if (!新_语言类::严格UTF8字节串有效(原文)) return std::nullopt;
    原文映射 结果{原文, {0}};
    for (std::size_t i = 0; i < 原文.size();) {
        const auto c = static_cast<unsigned char>(原文[i]);
        i += c < 0x80 ? 1 : c < 0xE0 ? 2 : c < 0xF0 ? 3 : 4;
        结果.标量字节边界.push_back(i);
    }
    return 结果;
}
std::string 读取语言场景原文范围(const 原文映射& 原文, 解释原文范围 范围) {
    if (范围.起始标量 >= 范围.结束标量 || 范围.结束标量 >= 原文.标量字节边界.size())
        throw std::logic_error("language scene invalid scalar range");
    const auto a = 原文.标量字节边界[范围.起始标量], b = 原文.标量字节边界[范围.结束标量];
    return 原文.原文.substr(a, b-a);
}
名称读取结果 读取词条普通名称完整(稳定编码 词条节点, 稳定编码 语言节点,
    const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在服务, const 概念_特征类& 特征服务,解释读取依据* 依据) {
    if(依据) {
        const auto term=依据->读词(词条节点);if(!term.信息||term.信息->语言节点!=语言节点)return {读取状态::引用不可读,{}};
        const auto all=依据->读名称(词条节点);if(all.状态!=概念名称关系查询状态::已找到)return {名称状态(all.状态),{}};
        名称读取结果 r{读取状态::已完成,{}};
        const auto stored=std::find_if(依据->名称候选依据组.begin(),依据->名称候选依据组.end(),[&](const auto&x){return x.词条节点==词条节点;});
        for(const auto& v:stored->完整候选组)if(const auto* name=std::get_if<概念名称关系信息>(&v)) {
            const auto e=存在服务.获取存在概念(name->概念节点);const auto f=特征服务.获取特征概念(name->概念节点);
            if(f)依据->核特征读取(name->概念节点,*f);
            if(e)r.完整普通名称材料组.push_back({*term.信息,*name,*e});
            else if(f)r.完整普通名称材料组.push_back({*term.信息,*name,*f});
            else return {读取状态::引用不可读,{}};
        }
        return r;
    }
    const auto 词 = 词服务.获取词条(词条节点);
    if (词.状态 != 新词条查询状态::已找到 || !词.信息)
        return {词.状态 == 新词条查询状态::资源失败 ? 读取状态::工程未完成 : 读取状态::引用不可读, {}};
    if (词.信息->语言节点 != 语言节点)
        return {读取状态::读取漂移, {}};
    const auto 名称 = 名称服务.查询词条候选概念(词条节点);
    if (名称.状态 != 概念名称关系查询状态::已找到) return {名称状态(名称.状态), {}};
    return 读取名称组(名称.名称关系节点组, 词条节点, {}, 语言节点, 词服务, 名称服务, 存在服务, 特征服务);
}
名称读取结果 读取概念当前名称完整(稳定编码 概念节点, 稳定编码 语言节点,
    const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在服务, const 概念_特征类& 特征服务) {
    const auto 名称 = 名称服务.查询概念名称(概念节点);
    if (名称.状态 != 概念名称关系查询状态::已找到) return {名称状态(名称.状态), {}};
    auto 结果 = 读取名称组(名称.名称关系节点组, {}, 概念节点, 语言节点, 词服务, 名称服务, 存在服务, 特征服务);
    if (结果.状态 == 读取状态::已完成 && 结果.完整普通名称材料组.empty()) 结果.状态 = 读取状态::缺名称;
    return 结果;
}
知识读取结果 读取语言场景基础知识完整(稳定编码 语言节点, std::size_t 最大条目数,
    const 新_语言类& 语言服务, const 新_词类& 词服务, const 概念_存在类& 存在服务,语言解释读取模式 模式) {
    if(模式==语言解释读取模式::本次正式有限集完整读取) {
        const auto p=语言服务.读取解释资源保护(语言节点,词服务,存在服务);
        if(p.状态==新语言解释资源保护状态::已读取)return {读取状态::已完成,p.候选知识组};
        return {p.状态==新语言解释资源保护状态::前提已变化 ? 读取状态::读取漂移:p.状态==新语言解释资源保护状态::资源失败 ? 读取状态::工程未完成:读取状态::引用不可读,{}};
    }
    const auto 全部 = 语言服务.枚举语言知识候选(语言节点, 最大条目数, 词服务, 存在服务);
    if (全部.状态 != 新语言知识状态::已完成) return {知识状态(全部.状态), {}};
    std::set<稳定编码> 已见;
    for (const auto& 项 : 全部.信息组) {
        if (!已见.insert(项.节点).second) return {读取状态::内部不一致, {}};
        const auto 回读 = 语言服务.获取语言知识(项.节点, 词服务, 存在服务);
        if (回读.状态 == 新语言知识状态::未找到) return {读取状态::读取漂移, {}};
        if (回读.状态 != 新语言知识状态::已完成) return {知识状态(回读.状态), {}};
        if (!回读.信息 || 回读.信息->节点 != 项.节点 || 回读.信息->目标节点 != 项.目标节点
            || 回读.信息->结构节点 != 项.结构节点 || 回读.信息->定义 != 项.定义
            || 回读.信息->来源组 != 项.来源组) return {读取状态::读取漂移, {}};
    }
    return {读取状态::已完成, 全部.信息组};
}
读取状态 核验语言场景命名来源(const 原文映射& 原文, const std::vector<命名来源项>& 来源,
    稳定编码 语言节点, const 新_词类& 词服务, const 概念_名称关系类& 名称服务) {
    for (const auto& 项 : 来源) {
        if (项.范围.起始标量 >= 项.范围.结束标量 || 项.范围.结束标量 >= 原文.标量字节边界.size()
            || !有效(项.词条节点) || !有效(项.名称关系节点) || !有效(项.概念节点)) return 读取状态::输入拒绝;
        const auto 词 = 词服务.获取词条(项.词条节点);
        if (词.状态 != 新词条查询状态::已找到 || !词.信息)
            return 词.状态 == 新词条查询状态::资源失败 ? 读取状态::工程未完成 : 读取状态::引用不可读;
        if (词.信息->语言节点 != 语言节点
            || 词.信息->规范化词面 != 读取语言场景原文范围(原文, 项.范围)) return 读取状态::输入拒绝;
        const auto 名称 = 名称服务.获取名称关系(项.名称关系节点);
        if (名称.状态 == 概念名称关系查询状态::未找到) continue;
        if (名称.状态 != 概念名称关系查询状态::已找到) return 名称状态(名称.状态);
        if (!名称.信息
            || 名称.信息->词条节点 != 项.词条节点 || 名称.信息->概念节点 != 项.概念节点)
            return 读取状态::输入拒绝;
    }
    return 读取状态::已完成;
}
bool 是S01知识(const 新语言知识信息& 知识, 稳定编码 语言节点) {
    const auto& 目标 = 知识.定义.目标;
    const auto& 结构 = 知识.定义.结构;
    if (目标.目的 != 新语言知识表达目的::陈述
        || !目标.适用条件组.empty() || 目标.角色组.size() != 2
        || 目标.含义组.size() != 1
        || 结构.类别 != 新语言知识结构类别::基础表达
        || 结构.语言节点 != 语言节点 || 结构.有序项组.size() != 3)
        return false;
    for (std::int64_t 编号 = 0; 编号 != 2; ++编号) {
        const auto 角色 = std::find_if(目标.角色组.begin(), 目标.角色组.end(),
            [编号](const auto& 项) { return 项.局部编号 == 编号; });
        if (角色 == 目标.角色组.end()
            || 角色->种类 != (编号 == 0
                ? 新语言知识角色种类::领域对象引用
                : 新语言知识角色种类::概念引用)
            || 角色->数量 != 新语言知识角色数量::单项
            || 角色->必要约束组.size() != 1
            || 角色->必要约束组.front().被约束角色 != 编号
            || 角色->必要约束组.front().允许类别组
                != std::vector<新语言知识角色类别>{编号 == 0
                    ? 新语言知识角色类别::存在
                    : 新语言知识角色类别::存在概念}) return false;
    }
    const auto& 含义 = 目标.含义组.front();
    if (含义.操作 != 新语言知识含义操作::概念归类
        || 含义.作用域 != 新语言知识作用域::普通陈述
        || 含义.特征值分支 || 含义.被询含义 || 含义.确认范围
        || 含义.参数组.size() != 2) return false;
    const auto 有参数 = [&](新语言知识参数角色 参数, std::int64_t 角色) {
        return std::count_if(含义.参数组.begin(), 含义.参数组.end(),
            [&](const auto& 项) {
                return 项.参数 == 参数 && 项.规则角色 == 角色;
            }) == 1;
    };
    if (!有参数(新语言知识参数角色::主体, 0)
        || !有参数(新语言知识参数角色::归类概念, 1)) return false;
    const auto& 项 = 结构.有序项组;
    if (项[0].局部编号 != 0 || 项[1].局部编号 != 1
        || 项[2].局部编号 != 2) return false;
    const auto* 左 = std::get_if<新语言知识角色表达项>(&项[0].载荷);
    const auto* 中 = std::get_if<新语言知识固定词项>(&项[1].载荷);
    const auto* 右 = std::get_if<新语言知识角色表达项>(&项[2].载荷);
    return 左 && 中 && 右 && 左->规则角色 == 0
        && 左->方式 == 新语言知识表达方式::明确对象描述
        && !左->指代概念节点 && 有效(中->词条节点)
        && 右->规则角色 == 1
        && 右->方式 == 新语言知识表达方式::名称表达
        && !右->指代概念节点;
}

namespace {
using 输入状态 = 语言场景归类输入状态;
struct 名称候选 final { 稳定编码 概念节点, 名称关系节点, 词条节点; };
输入状态 映射读取失败(读取状态 状态) {
    switch (状态) {
    case 读取状态::已完成: return 输入状态::已形成且读回;
    case 读取状态::输入拒绝: return 输入状态::输入拒绝;
    case 读取状态::格式失败: return 输入状态::格式失败;
    case 读取状态::未知词: return 输入状态::未知词;
    case 读取状态::缺名称: return 输入状态::缺名称;
    case 读取状态::缺规则: return 输入状态::缺规则;
    case 读取状态::额度不足: return 输入状态::额度不足;
    case 读取状态::读取漂移: return 输入状态::读取漂移;
    case 读取状态::内部不一致: return 输入状态::内部不一致;
    case 读取状态::工程未完成: return 输入状态::工程未完成;
    default: return 输入状态::引用不可读;
    }
}
输入状态 映射语言失败(新语言查询状态 状态) {
    return 状态 == 新语言查询状态::资源失败 ? 输入状态::工程未完成
        : 状态 == 新语言查询状态::结构不一致 ? 输入状态::内部不一致
        : 状态 == 新语言查询状态::输入不合法 ? 输入状态::输入拒绝
        : 输入状态::引用不可读;
}
输入状态 读取类名候选(稳定编码 词条节点, 稳定编码 语言节点,
    const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务, const 概念_特征类& 特征概念服务,
    std::vector<名称候选>& 结果,解释读取依据* 依据=nullptr) {
    const auto 全部 = 读取词条普通名称完整(词条节点, 语言节点,
        词服务, 名称服务, 存在概念服务, 特征概念服务,依据);
    if (全部.状态 != 读取状态::已完成) return 映射读取失败(全部.状态);
    for (const auto& 名称 : 全部.完整普通名称材料组) {
        const auto* 概念 = std::get_if<存在概念信息>(&名称.目标概念);
        if (概念 && 概念->种类 == 存在概念种类::抽象)
            结果.push_back({概念->节点, 名称.名称关系.节点, 名称.词条.节点});
    }
    return 结果.empty() ? 输入状态::角色不相容 : 输入状态::已形成且读回;
}
// 本叶只有完整单词条槽。空格若出现，只能恰好占两个槽间的一码点。
std::optional<std::array<新语言词条匹配片段, 3>> 读取三词布局(
    const std::vector<新语言词条匹配片段>& 片段组,
    const std::string& 原文) {
    if (原文.empty() || 原文.front() == ' ' || 原文.back() == ' ')
        return std::nullopt;
    std::array<新语言词条匹配片段, 3> 词;
    if (片段组.size() == 3) {
        for (std::size_t i = 0; i != 3; ++i) 词[i] = 片段组[i];
    } else if (片段组.size() == 5
        && 片段组[1].原文 == " " && 片段组[3].原文 == " "
        && 片段组[1].原文码点数量 == 1
        && 片段组[3].原文码点数量 == 1) {
        词 = {片段组[0], 片段组[2], 片段组[4]};
    } else return std::nullopt;
    if (!词[0].词条节点 || !词[1].词条节点 || !词[2].词条节点
        || !词[0].原文码点数量 || !词[1].原文码点数量
        || !词[2].原文码点数量 || 词[0].原文码点起点 != 0)
        return std::nullopt;
    std::size_t 字节位置 = 0, 标量位置 = 0;
    for (const auto& 片 : 片段组) {
        if (片.原文字节起点 != 字节位置
            || 片.原文码点起点 != 标量位置
            || 片.原文字节长度 != 片.原文.size()) return std::nullopt;
        字节位置 += 片.原文字节长度;
        标量位置 += 片.原文码点数量;
    }
    if (字节位置 != 原文.size()) return std::nullopt;
    return 词;
}

bool 候选顺序(const 语言场景归类候选& 左,
    const 语言场景归类候选& 右) {
    return std::tie(左.所用知识节点, 左.主体概念节点,
        左.归类概念节点, 左.出现组[0].名称关系节点,
        左.出现组[1].名称关系节点)
        < std::tie(右.所用知识节点, 右.主体概念节点,
            右.归类概念节点, 右.出现组[0].名称关系节点,
            右.出现组[1].名称关系节点);
}

} // namespace
static S01只读结果 只读识别语言场景S01实现(
    const 语言场景归类输入请求& 请求,
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务,
    const 概念_特征类& 特征概念服务,语言解释读取模式 模式,解释读取依据* 依据) {
    S01只读结果 结果{输入状态::输入拒绝, {}};
    if (!有效(请求.语言节点) || 请求.固定原文.empty()
        || (模式==语言解释读取模式::显式额度&&请求.最大知识条目数 == 0)) return 结果;
    if (!新_语言类::严格UTF8字节串有效(请求.固定原文)) {
        结果.状态 = 输入状态::格式失败; return 结果;
    }
    const auto 语言 = 语言服务.获取语言(请求.语言节点);
    if (语言.状态 != 新语言查询状态::已找到 || !语言.信息) {
        结果.状态 = 映射语言失败(语言.状态); return 结果;
    }
    const auto 匹配 = 依据 ? 依据->匹配词条(请求.语言节点,请求.固定原文)
        :语言服务.按已登记词条最长匹配(请求.语言节点, 请求.固定原文, 词服务);
    if (匹配.状态 != 新语言查询状态::已找到) {
        结果.状态 = 映射语言失败(匹配.状态); return 结果;
    }
    const auto 布局 = 读取三词布局(匹配.片段组, 请求.固定原文);
    if (!布局) {
        结果.状态 = std::any_of(匹配.片段组.begin(), 匹配.片段组.end(),
            [](const auto& 项) { return !项.词条节点 && 项.原文 != " "; })
            ? 输入状态::未知词 : 输入状态::结构未支持;
        return 结果;
    }
    std::array<新词条信息, 3> 词;
    for (std::size_t i = 0; i != 3; ++i) {
        const auto 读回 = 依据 ? 依据->读词(*(*布局)[i].词条节点):词服务.获取词条(*(*布局)[i].词条节点);
        if (读回.状态 != 新词条查询状态::已找到 || !读回.信息) {
            结果.状态 = 读回.状态 == 新词条查询状态::资源失败
                ? 输入状态::工程未完成 : 输入状态::引用不可读;
            return 结果;
        }
        if (读回.信息->语言节点 != 请求.语言节点
            || 读回.信息->规范化词面 != (*布局)[i].原文) {
            结果.状态 = 输入状态::读取漂移; return 结果;
        }
        词[i] = *读回.信息;
    }
    if (词[1].规范化词面 != "是") {
        结果.状态 = 输入状态::结构未支持; return 结果;
    }
    const auto 知识组 = 读取语言场景基础知识完整(
        请求.语言节点, 请求.最大知识条目数, 语言服务, 词服务, 存在概念服务,模式);
    if (知识组.状态 != 读取状态::已完成) {
        结果.状态 = 映射读取失败(知识组.状态); return 结果;
    }
    std::vector<稳定编码> 规则组;
    for (const auto& 知识 : 知识组.完整知识组) {
        if (!是S01知识(知识, 请求.语言节点)) continue;
        const auto 固定 = std::get<新语言知识固定词项>(
            知识.定义.结构.有序项组[1].载荷).词条节点;
        const auto 固定词 = 依据 ? 依据->读词(固定):词服务.获取词条(固定);
        if (固定词.状态 != 新词条查询状态::已找到 || !固定词.信息) {
            结果.状态 = 输入状态::引用不可读; return 结果;
        }
        if (固定词.信息->语言节点 != 请求.语言节点) continue;
        if (固定词.信息->节点 == 词[1].节点) 规则组.push_back(知识.节点);
    }
    if (规则组.empty()) { 结果.状态 = 输入状态::缺规则; return 结果; }
    std::vector<名称候选> 左, 右;
    结果.状态 = 读取类名候选(词[0].节点, 请求.语言节点, 词服务, 名称服务,
        存在概念服务, 特征概念服务, 左,依据);
    if (结果.状态 != 输入状态::已形成且读回) return 结果;
    结果.状态 = 读取类名候选(词[2].节点, 请求.语言节点, 词服务, 名称服务,
        存在概念服务, 特征概念服务, 右,依据);
    if (结果.状态 != 输入状态::已形成且读回) return 结果;
    const auto 左范围 = 解释原文范围{(*布局)[0].原文码点起点,
        (*布局)[0].原文码点起点 + (*布局)[0].原文码点数量};
    const auto 右范围 = 解释原文范围{(*布局)[2].原文码点起点,
        (*布局)[2].原文码点起点 + (*布局)[2].原文码点数量};
    for (auto 规则 : 规则组) for (const auto& a : 左) for (const auto& b : 右) {
        结果.完整候选组.push_back({a.概念节点, b.概念节点, 规则, {
            {1, 语言场景归类角色::主体, 左范围,
                a.词条节点, a.名称关系节点, a.概念节点},
            {2, 语言场景归类角色::归类概念, 右范围,
                b.词条节点, b.名称关系节点, b.概念节点}}});
    }
    std::sort(结果.完整候选组.begin(), 结果.完整候选组.end(), 候选顺序);
    if (规则组.size() != 1) 结果.状态 = 输入状态::多规则待解;
    else if (结果.完整候选组.size() != 1)
        结果.状态 = 输入状态::概念多义待解;
    return 结果;
}

S01只读结果 只读识别语言场景S01(const 语言场景归类输入请求& 请求,
    const 新_语言类& 语言服务,const 新_词类& 词服务,const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在服务,const 概念_特征类& 特征服务,语言解释读取模式 模式) {
    解释读取依据 依据{语言服务,词服务,名称服务,特征服务};
    auto r=只读识别语言场景S01实现(请求,语言服务,词服务,名称服务,存在服务,特征服务,模式,
        模式==语言解释读取模式::本次正式有限集完整读取 ? &依据:nullptr);
    if(依据.失败!=读取状态::已完成){r.状态=映射读取失败(依据.失败);r.完整候选组.clear();return r;}
    r.特征概念依据组=std::move(依据.特征概念依据组);r.词面查询依据组=std::move(依据.词面查询依据组);
    r.名称候选依据组=std::move(依据.名称候选依据组);return r;
}


} // namespace 海中鱼巣::语言场景转换内部
