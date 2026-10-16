#include "转换.语言场景基础.h"
#include "转换.语言场景共同.h"
#include "../领域/新_自然语言世界树类.h"

#include <algorithm>
#include <array>
#include <functional>
#include <set>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace 海中鱼巣 {
namespace {
using namespace 语言场景转换内部;
using I = 语言场景基础输入状态;
using O = 语言场景基础输出状态;
using F = 语言场景基础构式;
using R = 语言场景基础角色;
using U = 语言基础统一构式;
using K = 新语言知识角色类别;
using M = 新语言知识含义操作;
using P = 新语言知识参数角色;
using W = 新语言知识表达方式;

I 输入映射(读取状态 s) {
    switch (s) {
    case 读取状态::已完成: return I::已形成且读回;
    case 读取状态::输入拒绝: return I::输入拒绝;
    case 读取状态::格式失败: return I::格式失败;
    case 读取状态::未知词: return I::未知词;
    case 读取状态::缺名称: return I::缺名称;
    case 读取状态::缺规则: return I::缺规则;
    case 读取状态::额度不足: return I::额度不足;
    case 读取状态::读取漂移: return I::读取漂移;
    case 读取状态::内部不一致: return I::内部不一致;
    case 读取状态::工程未完成: return I::工程未完成;
    default: return I::引用不可读;
    }
}
O 输出映射(I s) {
    switch (s) {
    case I::已形成且读回: return O::已生成且唯一回读;
    case I::输入拒绝: case I::格式失败: return O::输入拒绝;
    case I::缺名称: return O::缺名称;
    case I::缺规则: return O::缺规则;
    case I::概念多义待解: case I::多规则待解: case I::构式多义待解: return O::回读多义;
    case I::额度不足: return O::额度不足;
    case I::引用不可读: return O::引用不可读;
    case I::读取漂移: return O::读取漂移;
    case I::内部不一致: return O::内部不一致;
    case I::工程未完成: return O::工程未完成;
    default: return O::回读含义不一致;
    }
}
bool 技术未完成(I s) {
    return s == I::额度不足 || s == I::引用不可读 || s == I::读取漂移
        || s == I::内部不一致 || s == I::工程未完成 || s == I::输入拒绝 || s == I::格式失败;
}
bool 范围相同(解释原文范围 a, 解释原文范围 b) {
    return a.起始标量 == b.起始标量 && a.结束标量 == b.结束标量;
}
bool 知识相同(const 新语言知识信息& a, const 新语言知识信息& b) {
    return a.节点 == b.节点 && a.目标节点 == b.目标节点 && a.结构节点 == b.结构节点
        && a.定义 == b.定义 && a.来源组 == b.来源组;
}
std::optional<std::string> 规范数字(const std::string& 原文) {
    std::size_t p = 0;
    bool 负 = false;
    if (!原文.empty() && (原文[0] == '+' || 原文[0] == '-')) { 负 = 原文[0] == '-'; ++p; }
    if (p == 原文.size()) return {};
    for (auto i = p; i < 原文.size(); ++i) if (原文[i] < '0' || 原文[i] > '9') return {};
    while (p < 原文.size() && 原文[p] == '0') ++p;
    if (p == 原文.size()) return std::string{"0"};
    return (负  ?  "-" : "") + 原文.substr(p);
}

bool 角色定义相同(const 新语言知识目标描述& d, std::int64_t n,
    新语言知识角色种类 kind, const std::vector<K>& cats) {
    const auto it = std::find_if(d.角色组.begin(), d.角色组.end(), [n](const auto& r) { return r.局部编号 == n; });
    return it != d.角色组.end() && it->种类 == kind && it->数量 == 新语言知识角色数量::单项
        && it->必要约束组.size() == 1 && it->必要约束组[0].被约束角色 == n
        && it->必要约束组[0].允许类别组 == cats;
}
// Only the published, unmodified leaf shapes enter this conversion family.
std::optional<U> 知识构式(const 新语言知识信息& k, 稳定编码 lang) {
    const auto& d = k.定义; const auto& t = d.目标; const auto& items = d.结构.有序项组;
    if (d.结构.语言节点 != lang || d.结构.类别 != 新语言知识结构类别::基础表达
        || !t.适用条件组.empty() || t.含义组.size() != 1) return {};
    for (std::size_t i = 0; i < items.size(); ++i) if (items[i].局部编号 != static_cast<std::int64_t>(i)) return {};
    const auto& m = t.含义组[0];
    if (m.局部编号 != 0) return {};
    const auto 参数 = [&](P p, std::int64_t r) { return std::count(m.参数组.begin(), m.参数组.end(), 新语言知识角色参数{p,r}) == 1; };
    const auto 角色项 = [&](std::size_t i, std::int64_t r, W w) {
        const auto* p = std::get_if<新语言知识角色表达项>(&items[i].载荷);
        return p && p->规则角色 == r && p->方式 == w && !p->指代概念节点;
    };
    const auto 固定项 = [&](std::size_t i) {
        const auto* p = std::get_if<新语言知识固定词项>(&items[i].载荷);
        return p && 有效(p->词条节点);
    };
    const auto 对象 = [&](std::int64_t i) { return 角色定义相同(t,i,新语言知识角色种类::领域对象引用,{K::存在}); };
    const auto 概念 = [&](std::int64_t i) { return 角色定义相同(t,i,新语言知识角色种类::概念引用,{K::存在概念,K::特征概念}); };
    if (t.目的 == 新语言知识表达目的::提问 && items.size() == 2 && t.角色组.size() == 2
        && m.操作 == M::信息请求 && m.作用域 == 新语言知识作用域::提问范围
        && m.参数组.empty() && !m.特征值分支 && m.确认范围 == 新语言知识确认范围::主张成立与否
        && m.被询含义 && m.被询含义->局部编号 == 1 && m.被询含义->操作 == M::概念归类
        && m.被询含义->作用域 == 新语言知识作用域::提问范围
        && m.被询含义->参数组.size() == 2 && 概念(0) && 概念(1) && 固定项(1)) {
        const auto* sub = std::get_if<新语言知识子规则项>(&items[0].载荷);
        const auto& ps = m.被询含义->参数组;
        if (sub && 有效(sub->子知识节点) && sub->映射组 == std::vector<新语言知识子规则映射>{{0,0},{1,1}}
            && std::count(ps.begin(),ps.end(),新语言知识角色参数{P::主体,0}) == 1
            && std::count(ps.begin(),ps.end(),新语言知识角色参数{P::归类概念,1}) == 1) return U::S09归类确认;
    }
    if (t.目的 != 新语言知识表达目的::陈述 || m.作用域 != 新语言知识作用域::普通陈述
        || m.被询含义 || m.确认范围) return {};
    if (m.操作 == M::概念归类 && !m.特征值分支 && t.角色组.size() == 2
        && m.参数组.size() == 2 && 参数(P::主体,0) && 参数(P::归类概念,1)) {
        if (items.size() == 3 && 对象(0)
            && 角色定义相同(t,1,新语言知识角色种类::概念引用,{K::存在概念})
            && 角色项(0,0,W::明确对象描述) && 固定项(1) && 角色项(2,1,W::名称表达)) return U::S01归类;
        if (items.size() == 4 && 概念(0) && 概念(1) && 角色项(0,0,W::名称表达)
            && 固定项(1) && 固定项(2) && 角色项(3,1,W::名称表达)) return U::S02上下位;
    }
    if (m.操作 == M::组成描述 && !m.特征值分支 && t.角色组.size() == 2
        && m.参数组.size() == 2 && 参数(P::部分,0) && 参数(P::整体,1) && 对象(0) && 对象(1)
        && items.size() == 5 && 角色项(0,0,W::明确对象描述) && 固定项(1)
        && 角色项(2,1,W::明确对象描述) && 固定项(3) && 固定项(4)) return U::S05组成;
    if (m.操作 != M::特征描述 || items.size() != 6 || t.角色组.size() != 3
        || m.参数组.size() != 3 || !参数(P::主体,0) || !参数(P::特征类型,1) || !参数(P::特征值,2)
        || !对象(0) || !角色项(0,0,W::明确对象描述) || !角色项(2,1,W::名称表达)
        || !固定项(1) || !固定项(3) || !固定项(4)) return {};
    if (m.特征值分支 == 新语言知识特征值分支::概念条件
        && 角色定义相同(t,1,新语言知识角色种类::特征类型引用,{K::正式特征类型})
        && 角色定义相同(t,2,新语言知识角色种类::概念引用,{K::特征概念}) && 角色项(5,2,W::名称表达)) return U::S03特征概念条件;
    if (m.特征值分支 == 新语言知识特征值分支::准确值
        && 角色定义相同(t,1,新语言知识角色种类::特征类型引用,{K::I64标量特征类型})
        && 角色定义相同(t,2,新语言知识角色种类::准确值,{K::I64标量}) && 角色项(5,2,W::类型化字面值)) return U::S03整数;
    return {};
}
struct 叶项 final {
    std::vector<std::int64_t> 路径;
    std::optional<稳定编码> 固定词;
    std::optional<std::int64_t> 角色;
    bool 数字 = false;
};
struct 只读上下文 final {
    const 新_语言类& 语言服务; const 新_词类& 词服务;
    const 概念_名称关系类& 名称服务; const 概念_存在类& 存在概念服务;
    const 概念_特征类& 特征概念服务;
};
struct 叶布局 final { 解释原文范围 范围; std::string 文本; std::optional<稳定编码> 词条; };
struct 布局结果 final { I 状态 = I::结构未支持; std::vector<叶布局> 项组; };
const 新语言知识信息* 找知识(const std::vector<新语言知识信息>& ks, 稳定编码 id) {
    const auto it=std::find_if(ks.begin(),ks.end(),[id](const auto& k){return k.节点==id;});
    return it == ks.end()  ?  nullptr : &*it;
}
std::vector<叶项> 展开知识(const 新语言知识信息& k, const std::vector<新语言知识信息>& ks, bool 外层=false) {
    std::vector<叶项> result;
    for (const auto& item : k.定义.结构.有序项组) {
        if (const auto* sub=std::get_if<新语言知识子规则项>(&item.载荷)) {
            if (外层) throw std::logic_error("recursive basic language knowledge");
            const auto* child=找知识(ks,sub->子知识节点);
            if (!child) return {};
            const auto cf=知识构式(*child,k.定义.结构.语言节点);
            if (cf != U::S02上下位) return {};
            auto leaves=展开知识(*child,ks,true);
            for(auto& leaf:leaves){leaf.路径.insert(leaf.路径.begin(),item.局部编号);result.push_back(std::move(leaf));}
        } else if (const auto* fixed=std::get_if<新语言知识固定词项>(&item.载荷))
            result.push_back({{item.局部编号},fixed->词条节点,{},false});
        else {
            const auto& role=std::get<新语言知识角色表达项>(item.载荷);
            result.push_back({{item.局部编号},{},role.规则角色,role.方式==W::类型化字面值});
        }
    }
    return result;
}
// Token boundaries come from registered longest matching, with one optional global spacing layout.
布局结果 匹配叶布局(const std::string& text, 稳定编码 lang, const std::vector<叶项>& leaves,
    const 只读上下文& c) {
    布局结果 failure;
    const auto map=解码语言场景原文(text);
    if(!map||text.empty()||leaves.empty()){failure.状态=I::格式失败;return failure;}
    if(text.front()==' '||text.back()==' ')return failure;
    const auto match=c.语言服务.按已登记词条最长匹配(lang,text,c.词服务);
    if(match.状态!=新语言查询状态::已找到){failure.状态=match.状态==新语言查询状态::资源失败 ? I::工程未完成:I::引用不可读;return failure;}
    std::vector<新词条信息> fixed;
    for(const auto&leaf:leaves)if(leaf.固定词){
        const auto word=c.词服务.获取词条(*leaf.固定词);
        if(word.状态!=新词条查询状态::已找到||!word.信息){failure.状态=word.状态==新词条查询状态::资源失败 ? I::工程未完成:I::引用不可读;return failure;}
        if(word.信息->节点!=*leaf.固定词||word.信息->语言节点!=lang){failure.状态=I::读取漂移;return failure;}
        fixed.push_back(*word.信息);
    }
    // Registered words may themselves contain spaces. Only separators outside matched words count.
    for(const bool spaced:{false,true}){
        布局结果 r;std::size_t byte=0,fi=0;bool unknown=false,valid=true;
        for(std::size_t i=0;i<leaves.size();++i){
            if(i&&spaced){if(byte>=text.size()||text[byte]!=' '){valid=false;break;}++byte;}
            if(byte>=text.size()){valid=false;break;}
            const auto start=byte;std::optional<稳定编码> term;
            if(leaves[i].数字){byte=text.size();if(!规范数字(text.substr(start))){valid=false;break;}}
            else{
                const auto it=std::find_if(match.片段组.begin(),match.片段组.end(),[byte](const auto&p){return p.原文字节起点==byte;});
                if(it==match.片段组.end()||!it->原文字节长度){valid=false;break;}
                term=it->词条节点;byte+=it->原文字节长度;unknown|=!term.has_value();
            }
            const auto a=std::lower_bound(map->标量字节边界.begin(),map->标量字节边界.end(),start);
            const auto b=std::lower_bound(map->标量字节边界.begin(),map->标量字节边界.end(),byte);
            if(a==map->标量字节边界.end()||b==map->标量字节边界.end()||*a!=start||*b!=byte)throw std::logic_error("broken language token boundary");
            叶布局 p{{static_cast<std::size_t>(a-map->标量字节边界.begin()),static_cast<std::size_t>(b-map->标量字节边界.begin())},text.substr(start,byte-start),term};
            if(leaves[i].固定词){const auto&word=fixed[fi++];if(term!=leaves[i].固定词||word.规范化词面!=p.文本){valid=false;break;}}
            r.项组.push_back(std::move(p));
        }
        if(valid&&byte==text.size()){r.状态=unknown ? I::未知词:I::已形成且读回;return r;}
    }
    return failure;
}
struct 根快照 final { 稳定编码 左,右; 单特征概念关系结果 结果; };
struct 读取快照 final {
    std::optional<新语言信息> 语言;
    std::vector<新语言知识信息> 知识;
    std::vector<普通名称材料> 名称;
    std::vector<新词条信息> 固定词;
    std::vector<根快照> 根;
};
struct 只读结果 final { I 状态=I::输入拒绝; std::vector<语言场景基础候选> 完整候选组; 读取快照 快照; };
using 角色键=std::tuple<std::int64_t,int,稳定编码,std::string,稳定编码,std::size_t,std::size_t>;
auto 候选键(const 语言场景基础候选& x, bool 证据=true){
    std::vector<角色键> rs;
    for(const auto& v:x.出现组)std::visit([&](const auto& o){
        using T=std::decay_t<decltype(o)>;
        if constexpr(std::is_same_v<T,语言场景基础命名出现>)rs.emplace_back(o.局部出现编号,static_cast<int>(o.角色),o.概念节点,"",证据 ? o.名称关系节点:稳定编码{},证据 ? o.原文范围.起始标量:0,证据 ? o.原文范围.结束标量:0);
        else rs.emplace_back(o.局部出现编号,static_cast<int>(R::整数值),稳定编码{},o.规范十进制,稳定编码{},证据 ? o.原文范围.起始标量:0,证据 ? o.原文范围.结束标量:0);
    },v);
    return std::tuple{x.构式,x.所用知识节点,rs};
}
bool 候选相同(const 语言场景基础候选& a,const 语言场景基础候选& b){
    if(候选键(a)!=候选键(b)||a.固定词组.size()!=b.固定词组.size()||a.主张.index()!=b.主张.index())return false;
    for(std::size_t i=0;i<a.固定词组.size();++i){const auto&x=a.固定词组[i];const auto&y=b.固定词组[i];if(x.结构项局部编号!=y.结构项局部编号||x.词条节点!=y.词条节点||!范围相同(x.原文范围,y.原文范围))return false;}
    for(std::size_t i=0;i<a.出现组.size();++i){
        if(const auto*x=std::get_if<语言场景基础整数出现>(&a.出现组[i])){const auto*y=std::get_if<语言场景基础整数出现>(&b.出现组[i]);if(!y||x->原始数字词面!=y->原始数字词面)return false;}
        else if(std::get<语言场景基础命名出现>(a.出现组[i]).词条节点!=std::get<语言场景基础命名出现>(b.出现组[i]).词条节点)return false;
    }
    return true;
}
bool 快照相同(const 读取快照& a,const 读取快照& b){
    if(a.语言!=b.语言||a.知识.size()!=b.知识.size()||a.名称.size()!=b.名称.size()||a.固定词!=b.固定词||a.根.size()!=b.根.size())return false;
    for(std::size_t i=0;i<a.知识.size();++i)if(!知识相同(a.知识[i],b.知识[i]))return false;
    for(std::size_t i=0;i<a.名称.size();++i){const auto&x=a.名称[i];const auto&y=b.名称[i];if(x.词条!=y.词条||x.名称关系!=y.名称关系||x.目标概念!=y.目标概念)return false;}
    for(std::size_t i=0;i<a.根.size();++i){const auto&x=a.根[i];const auto&y=b.根[i];if(x.左!=y.左||x.右!=y.右||x.结果.状态!=y.结果.状态||x.结果.关系!=y.结果.关系||x.结果.特征类型根!=y.结果.特征类型根||x.结果.最近共同上位!=y.结果.最近共同上位)return false;}
    return true;
}
I 根状态(特征概念规则状态 s){
    if(s==特征概念规则状态::资源失败||s==特征概念规则状态::读取接口未装配)return I::工程未完成;
    if(s==特征概念规则状态::值不存在)return I::引用不可读;
    return I::内部不一致;
}
只读结果 解释一个构式(const 语言场景基础输入请求& q,const 只读上下文& c,U form){
    只读结果 r;
    if(!有效(q.语言节点)||q.固定原文.empty()||!q.最大知识条目数)return r;
    if(!解码语言场景原文(q.固定原文)){r.状态=I::格式失败;return r;}
    const auto lang=c.语言服务.获取语言(q.语言节点);
    if(lang.状态!=新语言查询状态::已找到||!lang.信息||lang.信息->节点!=q.语言节点){r.状态=lang.状态==新语言查询状态::资源失败 ? I::工程未完成:I::引用不可读;return r;}
    r.快照.语言=lang.信息;
    const auto ks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,c.语言服务,c.词服务,c.存在概念服务);
    if(ks.状态!=读取状态::已完成){r.状态=输入映射(ks.状态);return r;}
    r.快照.知识=ks.完整知识组; r.状态=I::结构未支持;
    bool 有规则=false;
    for(const auto& k:ks.完整知识组){
        if(知识构式(k,q.语言节点)!=form)continue;
        有规则=true;
        const auto leaves=展开知识(k,ks.完整知识组);
        const auto layout=匹配叶布局(q.固定原文,q.语言节点,leaves,c);
        if(技术未完成(layout.状态)){r.状态=layout.状态;r.完整候选组.clear();return r;}
        if(layout.状态!=I::已形成且读回){if(layout.状态==I::未知词)r.状态=I::未知词;continue;}
        const auto& ps=layout.项组;
        bool 固定语义=true;
        const std::vector<std::pair<std::size_t,std::string>> expected=form==U::S05组成
             ? std::vector<std::pair<std::size_t,std::string>>{{1,"是"},{3,"的"},{4,"一部分"}}
            :std::vector<std::pair<std::size_t,std::string>>{{1,"的"},{3,"是"},{4,form==U::S03整数 ? "整数":"概念"}};
        for(const auto&[i,word]:expected){
            const auto term=c.词服务.获取词条(*leaves[i].固定词);
            if(term.状态!=新词条查询状态::已找到||!term.信息){r.状态=I::引用不可读;r.完整候选组.clear();return r;}
            r.快照.固定词.push_back(*term.信息);
            if(ps[i].文本!=word)固定语义=false;
        }
        if(!固定语义)continue;
        const auto number=form==U::S03整数 ? 规范数字(ps[5].文本):std::optional<std::string>{};
        if(form==U::S03整数&&!number)continue;
        const std::vector<std::size_t> named=form==U::S05组成 ? std::vector<std::size_t>{0,2}
            :form==U::S03整数 ? std::vector<std::size_t>{0,2}:std::vector<std::size_t>{0,2,5};
        std::vector<std::vector<普通名称材料>> choices;
        for(const auto i:named){
            const auto all=读取词条普通名称完整(*ps[i].词条,q.语言节点,c.词服务,c.名称服务,c.存在概念服务,c.特征概念服务);
            if(all.状态!=读取状态::已完成){r.状态=输入映射(all.状态);r.完整候选组.clear();return r;}
            r.快照.名称.insert(r.快照.名称.end(),all.完整普通名称材料组.begin(),all.完整普通名称材料组.end());
            std::vector<普通名称材料> filtered;
            for(const auto& x:all.完整普通名称材料组){
                const auto* e=std::get_if<存在概念信息>(&x.目标概念);
                if(form==U::S05组成||i==0){if(e&&e->种类==存在概念种类::抽象)filtered.push_back(x);}
                else if(std::holds_alternative<特征概念信息>(x.目标概念))filtered.push_back(x);
            }
            choices.push_back(std::move(filtered));
        }
        if(std::any_of(choices.begin(),choices.end(),[](const auto& x){return x.empty();})){r.状态=I::角色不相容;continue;}
        const auto make=[&](const std::vector<普通名称材料>& selected){
            语言场景基础候选 candidate;
            candidate.构式=form==U::S05组成 ? F::S05组成:form==U::S03整数 ? F::S03整数:F::S03特征概念条件;
            candidate.所用知识节点=k.节点;
            for(std::size_t j=0;j<selected.size();++j){const auto& x=selected[j];
                const R role=form==U::S05组成 ? (j==0 ? R::部分:R::整体):(j==0 ? R::主体:j==1 ? R::特征类型:R::特征概念);
                candidate.出现组.push_back(语言场景基础命名出现{static_cast<std::int64_t>(j+1),role,ps[named[j]].范围,x.词条.节点,x.名称关系.节点,x.名称关系.概念节点});}
            for(std::size_t i=0;i<leaves.size();++i)if(leaves[i].固定词)candidate.固定词组.push_back({k.定义.结构.有序项组[i].局部编号,ps[i].范围,*leaves[i].固定词});
            if(form==U::S05组成)candidate.主张=语言场景组成主张{1,2};
            else if(form==U::S03整数){candidate.出现组.push_back(语言场景基础整数出现{3,ps[5].范围,ps[5].文本,*number});candidate.主张=语言场景整数特征主张{1,2,3};}
            else candidate.主张=语言场景特征概念主张{1,2,3};
            r.完整候选组.push_back(std::move(candidate));
        };
        I failure=I::已形成且读回;
        std::vector<普通名称材料> selected;
        std::function<void(std::size_t)> walk=[&](std::size_t i){
            if(failure!=I::已形成且读回)return;
            if(i!=choices.size()){for(const auto& x:choices[i]){selected.push_back(x);walk(i+1);selected.pop_back();}return;}
            if(form!=U::S05组成){
                const auto ft=selected[1].名称关系.概念节点;
                const auto root=c.特征概念服务.比较单特征概念(ft,ft);
                r.快照.根.push_back({ft,ft,root});
                if(root.状态!=特征概念规则状态::已完成){failure=根状态(root.状态);return;}
                if(!root.关系||!root.特征类型根){failure=I::内部不一致;return;}
                if(root.特征类型根!=ft)return;
                if(form==U::S03整数){const auto& info=std::get<特征概念信息>(selected[1].目标概念);
                    if(info.材料类型!=特征概念材料物理类型::I64标量||!std::holds_alternative<std::vector<特征概念I64闭区间>>(info.值域))return;
                }else{
                    const auto target=selected[2].名称关系.概念节点;
                    const auto relation=c.特征概念服务.比较单特征概念(ft,target);
                    r.快照.根.push_back({ft,target,relation});
                    if(relation.状态!=特征概念规则状态::已完成){failure=根状态(relation.状态);return;}
                    if(!relation.关系){failure=I::内部不一致;return;}
                    if(relation.特征类型根!=ft)return;
                }
            }
            make(selected);
        };
        const auto before=r.完整候选组.size();walk(0);
        if(failure!=I::已形成且读回){r.状态=failure;r.完整候选组.clear();return r;}
        if(r.完整候选组.size()==before)r.状态=I::角色不相容;
    }
    if(!有规则){r.状态=I::缺规则;return r;}
    if(r.完整候选组.empty())return r;
    std::sort(r.完整候选组.begin(),r.完整候选组.end(),[](const auto&a,const auto&b){return 候选键(a)<候选键(b);});
    // Same semantic choice with duplicate name evidence has one stable execution representative.
    r.完整候选组.erase(std::unique(r.完整候选组.begin(),r.完整候选组.end(),[](const auto&a,const auto&b){return 候选键(a,false)==候选键(b,false);}),r.完整候选组.end());
    std::set<稳定编码> rules;std::set<decltype(候选键(r.完整候选组[0],false))> meanings;
    for(const auto& x:r.完整候选组){rules.insert(x.所用知识节点);auto key=候选键(x,false);std::get<1>(key)=稳定编码{};meanings.insert(key);}
    r.状态=meanings.size()>1 ? I::概念多义待解:rules.size()>1 ? I::多规则待解:I::已形成且读回;
    return r;
}
只读结果 只读解释语言场景S03概念(const 语言场景基础输入请求& q,const 只读上下文& c){return 解释一个构式(q,c,U::S03特征概念条件);}
只读结果 只读解释语言场景S03整数(const 语言场景基础输入请求& q,const 只读上下文& c){return 解释一个构式(q,c,U::S03整数);}
只读结果 只读解释语言场景S05(const 语言场景基础输入请求& q,const 只读上下文& c){return 解释一个构式(q,c,U::S05组成);}
只读结果 只读分派语言场景基础结构(const 语言场景基础输入请求& q,const 只读上下文& c){
    auto a=只读解释语言场景S03概念(q,c);auto b=只读解释语言场景S03整数(q,c);auto d=只读解释语言场景S05(q,c);
    只读结果 r;r.状态=I::结构未支持;
    for(auto* x:{&a,&b,&d}){
        if(技术未完成(x->状态)){r.状态=x->状态;return r;}
        if(x->状态!=I::结构未支持&&x->状态!=I::缺规则)r.状态=x->状态;
        r.完整候选组.insert(r.完整候选组.end(),x->完整候选组.begin(),x->完整候选组.end());
        if(!r.快照.语言)r.快照.语言=x->快照.语言;
        if(r.快照.知识.empty())r.快照.知识=x->快照.知识;
        else if(x->快照.知识.size()!=r.快照.知识.size()||!std::equal(r.快照.知识.begin(),r.快照.知识.end(),x->快照.知识.begin(),知识相同)){r.状态=I::读取漂移;r.完整候选组.clear();return r;}
        r.快照.名称.insert(r.快照.名称.end(),x->快照.名称.begin(),x->快照.名称.end());
        r.快照.固定词.insert(r.快照.固定词.end(),x->快照.固定词.begin(),x->快照.固定词.end());
        r.快照.根.insert(r.快照.根.end(),x->快照.根.begin(),x->快照.根.end());
    }
    std::set<F> forms;for(const auto&x:r.完整候选组)forms.insert(x.构式);
    if(forms.size()>1)r.状态=I::构式多义待解;
    else if(!r.完整候选组.empty())r.状态=r.完整候选组[0].构式==F::S03特征概念条件 ? a.状态:r.完整候选组[0].构式==F::S03整数 ? b.状态:d.状态;
    return r;
}

const 语言场景基础命名出现& 命名(const 语言场景基础候选& x,std::size_t i){return std::get<语言场景基础命名出现>(x.出现组.at(i));}
U 统一构式(F f){return f==F::S03整数 ? U::S03整数:f==F::S03特征概念条件 ? U::S03特征概念条件:U::S05组成;}
bool DTO形状有效(const 语言场景基础候选& x){
    if(!有效(x.所用知识节点))return false;
    const bool composition=x.构式==F::S05组成;
    const bool integer=x.构式==F::S03整数;
    if(!composition&&!integer&&x.构式!=F::S03特征概念条件)return false;
    if(x.出现组.size()!=(composition ? 2u:3u)||x.固定词组.size()!=3)return false;
    for(std::size_t i=0;i<x.出现组.size();++i){
        if(integer&&i==2){const auto* n=std::get_if<语言场景基础整数出现>(&x.出现组[i]);if(!n||n->局部出现编号!=3||规范数字(n->规范十进制)!=n->规范十进制||规范数字(n->原始数字词面)!=n->规范十进制)return false;}
        else{const auto* n=std::get_if<语言场景基础命名出现>(&x.出现组[i]);
            const auto role=composition ? (i==0 ? R::部分:R::整体):(i==0 ? R::主体:i==1 ? R::特征类型:R::特征概念);
            if(!n||n->局部出现编号!=static_cast<std::int64_t>(i+1)||n->角色!=role||!有效(n->词条节点)||!有效(n->名称关系节点)||!有效(n->概念节点))return false;}
    }
    if(composition){const auto*m=std::get_if<语言场景组成主张>(&x.主张);return m&&m->部分出现编号==1&&m->整体出现编号==2;}
    if(integer){const auto*m=std::get_if<语言场景整数特征主张>(&x.主张);return m&&m->主体出现编号==1&&m->特征类型出现编号==2&&m->整数出现编号==3;}
    const auto*m=std::get_if<语言场景特征概念主张>(&x.主张);return m&&m->主体出现编号==1&&m->特征类型出现编号==2&&m->特征概念出现编号==3;
}
I 核验叶覆盖(const 原文映射& map,const std::vector<语言基础统一对齐>& align,const std::vector<叶项>& leaves,
    稳定编码 lang,const 新_词类& words,const 概念_名称关系类& names){
    if(align.size()!=leaves.size()||align.empty()||map.原文.empty()||map.原文.front()==' '||map.原文.back()==' ')return I::输入拒绝;
    const auto total=map.标量字节边界.size()-1;
    std::vector<命名来源项> named;
    bool spaced=false;
    for(std::size_t i=0;i<align.size();++i){const auto&a=align[i];const auto&l=leaves[i];
        if(a.结构项路径!=l.路径||a.原文范围.起始标量>=a.原文范围.结束标量||a.原文范围.结束标量>total)return I::输入拒绝;
        if(i==0){if(a.原文范围.起始标量!=0)return I::输入拒绝;}
        else{const auto end=align[i-1].原文范围.结束标量;
            if(i==1)spaced=a.原文范围.起始标量==end+1;
            if(a.原文范围.起始标量!=end+(spaced ? 1u:0u))return I::输入拒绝;
            if(spaced&&读取语言场景原文范围(map,{end,end+1})!=" ")return I::输入拒绝;}
        if(l.固定词){
            if(a.词条节点!=l.固定词||a.名称关系节点||a.概念节点||a.局部出现编号||a.规范十进制)return I::输入拒绝;
            const auto t=words.获取词条(*l.固定词);
            if(t.状态!=新词条查询状态::已找到||!t.信息)return t.状态==新词条查询状态::资源失败 ? I::工程未完成:I::引用不可读;
            if(t.信息->语言节点!=lang||t.信息->规范化词面!=读取语言场景原文范围(map,a.原文范围))return I::输入拒绝;
        }else if(l.数字){
            if(a.词条节点||a.名称关系节点||a.概念节点||!a.局部出现编号||!a.规范十进制
                ||a.局部出现编号!=*l.角色+1||规范数字(读取语言场景原文范围(map,a.原文范围))!=a.规范十进制)return I::输入拒绝;
        }else{
            if(!a.词条节点||!a.名称关系节点||!a.概念节点||a.规范十进制||a.局部出现编号!=*l.角色+1)return I::输入拒绝;
            named.push_back({a.原文范围,*a.词条节点,*a.名称关系节点,*a.概念节点});
        }
    }
    if(align.back().原文范围.结束标量!=total)return I::输入拒绝;
    return 输入映射(核验语言场景命名来源(map,named,lang,words,names));
}
std::vector<语言基础统一对齐> 基础对齐(const 语言场景基础候选& x,const 新语言知识信息& k){
    std::vector<语言基础统一对齐> result;
    for(const auto& item:k.定义.结构.有序项组){
        if(const auto*f=std::get_if<新语言知识固定词项>(&item.载荷)){
            const auto it=std::find_if(x.固定词组.begin(),x.固定词组.end(),[&](const auto&o){return o.结构项局部编号==item.局部编号;});
            if(it==x.固定词组.end()||it->词条节点!=f->词条节点)return {};
            result.push_back({it->原文范围,{item.局部编号},it->词条节点,{},{},{},{}});
        }else{
            const auto*r=std::get_if<新语言知识角色表达项>(&item.载荷);if(!r)return {};
            const auto n=static_cast<std::size_t>(r->规则角色);
            if(n>=x.出现组.size())return {};
            if(const auto*o=std::get_if<语言场景基础命名出现>(&x.出现组[n]))result.push_back({o->原文范围,{item.局部编号},o->词条节点,o->名称关系节点,o->概念节点,o->局部出现编号,{}});
            else{const auto&v=std::get<语言场景基础整数出现>(x.出现组[n]);result.push_back({v.原文范围,{item.局部编号},{},{},{},v.局部出现编号,v.规范十进制});}
        }
    }
    return result;
}
I 核验基础来源(const 语言场景基础含义& meaning,稳定编码 lang,const 只读上下文& c,
    const std::vector<新语言知识信息>& ks){
    if(!DTO形状有效(meaning.解释)||meaning.作用域!=新语言知识作用域::普通陈述)return I::输入拒绝;
    const auto*k=找知识(ks,meaning.解释.所用知识节点);
    if(!k||知识构式(*k,lang)!=统一构式(meaning.解释.构式))return I::缺规则;
    const auto map=解码语言场景原文(meaning.固定原文);if(!map)return I::输入拒绝;
    auto status=核验叶覆盖(*map,基础对齐(meaning.解释,*k),展开知识(*k,ks),lang,c.词服务,c.名称服务);
    if(status!=I::已形成且读回)return status;
    const auto&x=meaning.解释;
    if(x.构式==F::S03整数){const auto&v=std::get<语言场景基础整数出现>(x.出现组[2]);
        if(读取语言场景原文范围(*map,v.原文范围)!=v.原始数字词面)return I::输入拒绝;}
    for(std::size_t i=0;i<x.出现组.size();++i){
        const auto*n=std::get_if<语言场景基础命名出现>(&x.出现组[i]);if(!n)continue;
        const auto E=c.存在概念服务.获取存在概念(n->概念节点);const auto Finfo=c.特征概念服务.获取特征概念(n->概念节点);
        if(E.has_value()==Finfo.has_value())return I::引用不可读;
        if(i==0||x.构式==F::S05组成){if(!E||E->种类!=存在概念种类::抽象)return I::角色不相容;}
        else if(!Finfo)return I::角色不相容;
    }
    if(x.构式!=F::S05组成){
        const auto ft=命名(x,1).概念节点;const auto root=c.特征概念服务.比较单特征概念(ft,ft);
        if(root.状态!=特征概念规则状态::已完成)return 根状态(root.状态);
        if(root.特征类型根!=ft)return I::角色不相容;
        if(x.构式==F::S03整数){const auto info=c.特征概念服务.获取特征概念(ft);
            if(!info)return I::引用不可读;
            if(info->材料类型!=特征概念材料物理类型::I64标量||!std::holds_alternative<std::vector<特征概念I64闭区间>>(info->值域))return I::角色不相容;
        }else{const auto relation=c.特征概念服务.比较单特征概念(ft,命名(x,2).概念节点);
            if(relation.状态!=特征概念规则状态::已完成)return 根状态(relation.状态);
            if(relation.特征类型根!=ft)return I::角色不相容;}
    }
    return I::已形成且读回;
}
I 核验实例绑定(const 语言场景基础含义& m,const 新_自然语言世界树类& world){
    const auto scene=world.获取场景(m.场景节点);
    if(!scene||scene->节点!=m.场景节点)return I::引用不可读;
    const std::size_t count=m.解释.构式==F::S05组成 ? 2:1;
    if(m.对象绑定组.size()!=count)return I::输入拒绝;
    std::set<稳定编码> seen;
    for(std::size_t i=0;i<count;++i){const auto&b=m.对象绑定组[i];
        if(b.局部出现编号!=static_cast<std::int64_t>(i+1)||b.来源概念节点!=命名(m.解释,i).概念节点||!seen.insert(b.实例节点).second)return I::输入拒绝;
        const auto instance=world.获取概念实例(b.实例节点);
        if(!instance||instance->节点!=b.实例节点||instance->所属场景节点!=m.场景节点
            ||instance->概念节点!=b.来源概念节点||instance->种类!=新自然语言概念实例种类::存在概念)return I::引用不可读;
    }
    return I::已形成且读回;
}
struct 生成项选择 final { std::optional<新词条信息> 词条; std::optional<概念名称关系信息> 名称; std::optional<std::string> 数字; };
struct 生成文字候选 final { std::string 文本; std::vector<语言场景基础出现> 出现组; std::vector<语言场景基础固定词出现> 固定词组; };
生成文字候选 按基础知识生成文字(const 语言场景基础候选& x,const 新语言知识信息& k,
    const std::vector<生成项选择>& choices,bool spaced){
    生成文字候选 result;std::size_t scalar=0;
    const auto&items=k.定义.结构.有序项组;
    if(choices.size()!=items.size())throw std::logic_error("incomplete generation selection");
    for(std::size_t i=0;i<items.size();++i){
        const auto&s=choices[i];
        if(i&&spaced){result.文本+=' ';++scalar;}
        const auto text=s.数字 ? *s.数字:s.词条 ? s.词条->规范化词面:throw std::logic_error("missing generation word");
        const auto map=解码语言场景原文(text);if(!map)throw std::logic_error("invalid generation UTF8");
        const 解释原文范围 range{scalar,scalar+map->标量字节边界.size()-1};scalar=range.结束标量;result.文本+=text;
        if(std::holds_alternative<新语言知识固定词项>(items[i].载荷))result.固定词组.push_back({items[i].局部编号,range,s.词条->节点});
        else{const auto&role=std::get<新语言知识角色表达项>(items[i].载荷);const auto n=static_cast<std::size_t>(role.规则角色);
            if(s.数字)result.出现组.push_back(语言场景基础整数出现{static_cast<std::int64_t>(n+1),range,text,text});
            else{if(!s.名称)throw std::logic_error("missing name evidence");
                auto o=命名(x,n);o.原文范围=range;o.词条节点=s.词条->节点;o.名称关系节点=s.名称->节点;
                result.出现组.push_back(o);}
        }
    }
    return result;
}
生成文字候选 生成语言场景S03概念文字(const 语言场景基础候选& x,const 新语言知识信息& k,const std::vector<生成项选择>& s,bool space){return 按基础知识生成文字(x,k,s,space);}
生成文字候选 生成语言场景S03整数文字(const 语言场景基础候选& x,const 新语言知识信息& k,const std::vector<生成项选择>& s,bool space){return 按基础知识生成文字(x,k,s,space);}
生成文字候选 生成语言场景S05文字(const 语言场景基础候选& x,const 新语言知识信息& k,const std::vector<生成项选择>& s,bool space){return 按基础知识生成文字(x,k,s,space);}
} // namespace

语言场景基础输入结果 转换单句基础结构到空白语言场景(const 语言场景基础输入请求& q,
    新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,
    const 概念_名称关系类& names,const 概念_存在类& es,const 概念_特征类& fs){
    const 只读上下文 c{lang,words,names,es,fs};语言场景基础输入结果 result;
    const auto read=只读分派语言场景基础结构(q,c);result.状态=read.状态;result.完整候选组=read.完整候选组;
    if(read.状态!=I::已形成且读回)return result;
    const auto again=只读分派语言场景基础结构(q,c);
    if(again.状态!=I::已形成且读回||read.完整候选组.size()!=1||again.完整候选组.size()!=1
        ||!候选相同(read.完整候选组[0],again.完整候选组[0])||!快照相同(read.快照,again.快照)){
        result.状态=I::读取漂移;result.完整候选组.clear();return result;}
    const auto&candidate=read.完整候选组[0];const auto scene=world.建立空场景();result.已取得场景节点=scene.场景节点;
    if(scene.状态!=新自然语言场景操作状态::已建立||!scene.场景节点){result.状态=I::场景建立未完成;return result;}
    const std::size_t count=candidate.构式==F::S05组成 ? 2:1;
    for(std::size_t i=0;i<count;++i){const auto&occ=命名(candidate,i);const auto instance=world.实例化概念(*scene.场景节点,occ.概念节点);
        if(instance.实例)result.已取得对象绑定组.push_back({occ.局部出现编号,instance.实例->节点,instance.实例->概念节点});
        if(instance.状态!=新自然语言概念实例操作状态::已建立||!instance.实例){result.状态=I::实例建立未完成;return result;}}
    语言场景基础含义 meaning{*scene.场景节点,新语言知识作用域::普通陈述,q.固定原文,q.未核验来源语言记录节点,candidate,result.已取得对象绑定组};
    const auto after=只读分派语言场景基础结构(q,c);
    if(核验实例绑定(meaning,world)!=I::已形成且读回||after.状态!=I::已形成且读回||after.完整候选组.size()!=1
        ||!候选相同(candidate,after.完整候选组[0])||!快照相同(read.快照,after.快照)||核验基础来源(meaning,q.语言节点,c,after.快照.知识)!=I::已形成且读回){result.状态=I::写后读回未完成;result.完整候选组.clear();return result;}
    result.含义=std::move(meaning);result.状态=I::已形成且读回;return result;
}

语言场景基础输出结果 从语言场景基础含义生成可回读短句(const 语言场景基础输出请求& q,
    const 新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,
    const 概念_名称关系类& names,const 概念_存在类& es,const 概念_特征类& fs){
    语言场景基础输出结果 result;const 只读上下文 c{lang,words,names,es,fs};
    if(!有效(q.语言节点)||!q.最大知识条目数||!DTO形状有效(q.含义.解释))return result;
    const auto ks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(ks.状态!=读取状态::已完成){result.状态=输出映射(输入映射(ks.状态));return result;}
    auto status=核验基础来源(q.含义,q.语言节点,c,ks.完整知识组);
    if(status==I::已形成且读回)status=核验实例绑定(q.含义,world);
    if(status!=I::已形成且读回){result.状态=输出映射(status);return result;}
    const auto&x=q.含义.解释;const auto*k=找知识(ks.完整知识组,x.所用知识节点);
    std::vector<std::vector<普通名称材料>> options;
    for(const auto&v:x.出现组){if(const auto*o=std::get_if<语言场景基础命名出现>(&v)){
        const auto all=读取概念当前名称完整(o->概念节点,q.语言节点,words,names,es,fs);
        if(all.状态!=读取状态::已完成){result.状态=输出映射(输入映射(all.状态));return result;}options.push_back(all.完整普通名称材料组);}}
    struct Usable{bool spaced;生成文字候选 output;std::vector<稳定编码> names;};std::vector<Usable> usable;
    bool ambiguous=false,mismatch=false;I fatal=I::已形成且读回;
    std::vector<普通名称材料> selected;
    std::function<void(std::size_t)> visit=[&](std::size_t n){
        if(fatal!=I::已形成且读回)return;
        if(n<options.size()){for(const auto&o:options[n]){selected.push_back(o);visit(n+1);selected.pop_back();}return;}
        std::vector<生成项选择> choices;
        for(const auto&item:k->定义.结构.有序项组){
            if(const auto*f=std::get_if<新语言知识固定词项>(&item.载荷)){
                const auto word=words.获取词条(f->词条节点);if(word.状态!=新词条查询状态::已找到||!word.信息){fatal=I::引用不可读;return;}
                choices.push_back({*word.信息,{},{}});
            }else{const auto&r=std::get<新语言知识角色表达项>(item.载荷);
                if(r.方式==W::类型化字面值)choices.push_back({{},{},std::get<语言场景基础整数出现>(x.出现组[2]).规范十进制});
                else{const auto&o=selected.at(static_cast<std::size_t>(r.规则角色));choices.push_back({o.词条,o.名称关系,{}});}}
        }
        for(const bool spaced:{false,true}){
            auto text=x.构式==F::S05组成 ? 生成语言场景S05文字(x,*k,choices,spaced)
                :x.构式==F::S03整数 ? 生成语言场景S03整数文字(x,*k,choices,spaced):生成语言场景S03概念文字(x,*k,choices,spaced);
            const auto read=只读分派语言场景基础结构({q.语言节点,text.文本,{},q.最大知识条目数},c);
            if(技术未完成(read.状态)){fatal=read.状态;return;}
            if(read.状态==I::概念多义待解||read.状态==I::多规则待解||read.状态==I::构式多义待解){ambiguous=true;continue;}
            if(read.状态!=I::已形成且读回||read.完整候选组.size()!=1||候选键(read.完整候选组[0],false)!=候选键(x,false)){mismatch=true;continue;}
            auto expected=x;expected.出现组=text.出现组;expected.固定词组=text.固定词组;
            if(!候选相同(expected,read.完整候选组[0])){mismatch=true;continue;}
            std::vector<稳定编码> ids;for(const auto&o:selected)ids.push_back(o.名称关系.节点);
            usable.push_back({spaced,std::move(text),std::move(ids)});
        }
    };visit(0);
    if(fatal!=I::已形成且读回){result.状态=输出映射(fatal);return result;}
    const auto endks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(endks.状态!=读取状态::已完成){result.状态=输出映射(输入映射(endks.状态));return result;}
    if(endks.完整知识组.size()!=ks.完整知识组.size()||!std::equal(ks.完整知识组.begin(),ks.完整知识组.end(),endks.完整知识组.begin(),知识相同)){result.状态=O::读取漂移;return result;}
    std::size_t index=0;
    for(const auto&v:x.出现组)if(const auto*o=std::get_if<语言场景基础命名出现>(&v)){
        const auto all=读取概念当前名称完整(o->概念节点,q.语言节点,words,names,es,fs);
        if(all.状态!=读取状态::已完成){result.状态=输出映射(输入映射(all.状态));return result;}
        读取快照 a,b;a.名称=options[index++];b.名称=all.完整普通名称材料组;if(!快照相同(a,b)){result.状态=O::读取漂移;return result;}}
    if(usable.empty()){result.状态=ambiguous ? O::回读多义:mismatch ? O::回读含义不一致:O::名称待选;return result;}
    std::sort(usable.begin(),usable.end(),[](const auto&a,const auto&b){return std::tie(a.spaced,a.output.文本,a.names)<std::tie(b.spaced,b.output.文本,b.names);});
    result.文本=usable[0].output.文本;result.输出对齐组=usable[0].output.出现组;result.输出固定词组=usable[0].output.固定词组;result.所用知识节点=x.所用知识节点;result.状态=O::已生成且唯一回读;return result;
}

namespace {

I S01状态(语言场景归类输入状态 s){
    using S=语言场景归类输入状态;
    switch(s){
#define MAP01(v) case S::v:return I::v
    MAP01(已形成且读回);MAP01(输入拒绝);MAP01(格式失败);MAP01(结构未支持);MAP01(未知词);MAP01(缺名称);
    MAP01(角色不相容);MAP01(缺规则);MAP01(概念多义待解);MAP01(多规则待解);MAP01(额度不足);
    MAP01(场景建立未完成);MAP01(实例建立未完成);MAP01(写后读回未完成);MAP01(引用不可读);MAP01(读取漂移);MAP01(内部不一致);MAP01(工程未完成);
#undef MAP01
    }throw std::logic_error("unknown S01 state");
}
O S01输出状态(语言场景归类输出状态 s){
    using S=语言场景归类输出状态;
    switch(s){
#define MAP01OUT(v) case S::v: return O::v
    MAP01OUT(已生成且唯一回读); MAP01OUT(输入拒绝); MAP01OUT(缺名称); MAP01OUT(名称待选);
    MAP01OUT(缺规则); MAP01OUT(回读多义); MAP01OUT(回读含义不一致); MAP01OUT(引用不可读);
    MAP01OUT(读取漂移); MAP01OUT(额度不足); MAP01OUT(内部不一致); MAP01OUT(工程未完成);
#undef MAP01OUT
    }
    throw std::logic_error("unknown S01 output state");
}
I S02状态(基础概念上下位输入状态 s){
    using S=基础概念上下位输入状态;
    switch(s){
    case S::已形成唯一含义:return I::已形成且读回;
#define MAP02(v) case S::v:return I::v
    MAP02(概念多义待解);MAP02(多规则待解);MAP02(未知词);MAP02(缺名称);MAP02(缺规则);MAP02(角色不相容);
    MAP02(结构未支持);MAP02(输入拒绝);MAP02(格式失败);MAP02(引用不可读);MAP02(额度不足);MAP02(内部不一致);MAP02(工程未完成);
#undef MAP02
    case S::能力未支持:return I::结构未支持;
    }throw std::logic_error("unknown S02 state");
}
I S09状态(基础概念归类确认输入状态 s){
    using S=基础概念归类确认输入状态;
    switch(s){
    case S::已形成唯一信息请求:case S::受询者待绑定:return I::已形成且读回;
#define MAP09(v) case S::v:return I::v
    MAP09(概念多义待解);MAP09(多规则待解);MAP09(未知词);MAP09(缺名称);MAP09(缺规则);MAP09(角色不相容);
    MAP09(结构未支持);MAP09(输入拒绝);MAP09(格式失败);MAP09(引用不可读);MAP09(额度不足);MAP09(内部不一致);MAP09(工程未完成);
#undef MAP09
    case S::能力未支持:return I::结构未支持;
    case S::受询者不可读:return I::内部不一致;
    }throw std::logic_error("unknown S09 state");
}
U 候选构式(const 语言基础统一候选& v){return std::visit([](const auto&x)->U{
    using T=std::decay_t<decltype(x)>;
    if constexpr(std::is_same_v<T,语言场景归类候选>)return U::S01归类;
    else if constexpr(std::is_same_v<T,基础概念上下位候选>)return U::S02上下位;
    else if constexpr(std::is_same_v<T,语言场景基础候选>)return 统一构式(x.构式);
    else return U::S09归类确认;
},v);}
struct 统一键 final {
    U 构式; K 类别=K::存在;std::vector<稳定编码> 身份;std::vector<std::string> 数字;
    friend bool operator==(const 统一键&,const 统一键&)=default;
    friend bool operator<(const 统一键&a,const 统一键&b){return std::tie(a.构式,a.类别,a.身份,a.数字)<std::tie(b.构式,b.类别,b.身份,b.数字);}
};
统一键 统一候选键(const 语言基础统一候选& v,bool names=false){
    统一键 key{候选构式(v)};
    const auto conceptkey=[&](const 基础概念上下位候选&x){key.类别=x.类别;key.身份={x.所用知识节点,x.主体概念节点,x.归类概念节点};if(names){key.身份.push_back(x.主体名称关系节点);key.身份.push_back(x.归类名称关系节点);}};
    std::visit([&](const auto&x){using T=std::decay_t<decltype(x)>;
        if constexpr(std::is_same_v<T,语言场景归类候选>){key.身份={x.所用知识节点,x.主体概念节点,x.归类概念节点};if(names)for(const auto&o:x.出现组){key.身份.push_back(o.名称关系节点);key.身份.push_back(o.词条节点);}}
        else if constexpr(std::is_same_v<T,基础概念上下位候选>)conceptkey(x);
        else if constexpr(std::is_same_v<T,语言场景基础候选>){key.身份={x.所用知识节点};for(const auto&o:x.出现组){if(const auto*n=std::get_if<语言场景基础命名出现>(&o)){key.身份.push_back(n->概念节点);if(names){key.身份.push_back(n->名称关系节点);key.身份.push_back(n->词条节点);}}else key.数字.push_back(std::get<语言场景基础整数出现>(o).规范十进制);}}
        else{conceptkey(x.被询归类);key.身份.insert(key.身份.begin(),x.外层知识节点);key.身份.push_back(x.子知识节点);if(x.受询者存在节点)key.身份.push_back(*x.受询者存在节点);}
    },v);return key;
}
bool S02完全相同(const 基础概念上下位候选&a,const 基础概念上下位候选&b){return a.类别==b.类别&&a.主体概念节点==b.主体概念节点&&a.归类概念节点==b.归类概念节点&&a.主体名称关系节点==b.主体名称关系节点&&a.归类名称关系节点==b.归类名称关系节点&&a.所用知识节点==b.所用知识节点&&范围相同(a.主体范围,b.主体范围)&&范围相同(a.归类范围,b.归类范围);}
bool 统一候选相同(const 语言基础统一候选&a,const 语言基础统一候选&b){
    if(a.index()!=b.index()||统一候选键(a,true)!=统一候选键(b,true))return false;
    if(const auto*x=std::get_if<语言场景基础候选>(&a))return 候选相同(*x,std::get<语言场景基础候选>(b));
    if(const auto*x=std::get_if<基础概念上下位候选>(&a))return S02完全相同(*x,std::get<基础概念上下位候选>(b));
    if(const auto*x=std::get_if<基础概念归类确认候选>(&a)){const auto&y=std::get<基础概念归类确认候选>(b);return !x->受询者存在节点&&!y.受询者存在节点&&S02完全相同(x->被询归类,y.被询归类)&&范围相同(x->子句范围,y.子句范围)&&范围相同(x->确认词范围,y.确认词范围);}
    const auto&x=std::get<语言场景归类候选>(a);const auto&y=std::get<语言场景归类候选>(b);
    if(x.出现组.size()!=y.出现组.size())return false;
    for(std::size_t i=0;i<x.出现组.size();++i){const auto&p=x.出现组[i];const auto&q=y.出现组[i];if(p.局部出现编号!=q.局部出现编号||p.角色!=q.角色||!范围相同(p.原文范围,q.原文范围))return false;}
    return true;
}
语言基础统一输入结果 只读识别六类基础句型(const 语言基础统一输入请求& q,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances){
    语言基础统一输入结果 result;
    if(!有效(q.语言节点)||q.固定原文.empty()||!q.最大原文标量数||!q.最大知识条目数||!q.最大名称候选数||!q.最大组合候选数||!q.最大确认候选数)return result;
    const auto map=解码语言场景原文(q.固定原文);if(!map){result.状态=I::格式失败;return result;}
    if(map->标量字节边界.size()-1>q.最大原文标量数){result.状态=I::额度不足;return result;}
    const auto language=lang.获取语言(q.语言节点);if(language.状态!=新语言查询状态::已找到||!language.信息||language.信息->节点!=q.语言节点){result.状态=language.状态==新语言查询状态::资源失败 ? I::工程未完成:I::引用不可读;return result;}
    const auto ks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(ks.状态!=读取状态::已完成){result.状态=输入映射(ks.状态);return result;}
    const 只读上下文 c{lang,words,names,es,fs};std::set<U> applicable;
    for(const auto&k:ks.完整知识组){const auto form=知识构式(k,q.语言节点);if(!form)continue;
        const auto leaves=展开知识(k,ks.完整知识组);if(leaves.empty())continue;
        const auto layout=匹配叶布局(q.固定原文,q.语言节点,leaves,c);
        if(技术未完成(layout.状态)){result.状态=layout.状态;return result;}
        if(layout.状态==I::已形成且读回||layout.状态==I::未知词)applicable.insert(*form);
    }
    result.状态=ks.完整知识组.empty() ? I::缺规则:I::结构未支持;
    std::vector<I> branchstates;
    const auto collect=[&](I status,const auto&candidates){
        branchstates.push_back(status);
        if(技术未完成(status)){result.状态=status;result.完整候选组.clear();return false;}
        if(status!=I::结构未支持&&status!=I::缺规则)result.状态=status;
        for(const auto&x:candidates)result.完整候选组.emplace_back(x);
        return true;
    };
    if(applicable.contains(U::S01归类)){
        const auto r=只读识别语言场景S01({q.语言节点,q.固定原文,q.未核验来源语言记录节点,q.最大知识条目数},lang,words,names,es,fs);
        if(!collect(S01状态(r.状态),r.完整候选组))return result;
    }
    if(applicable.contains(U::S02上下位)){
        const auto r=解释单句概念上下位({q.固定原文,q.语言节点,q.未核验来源语言记录节点,q.最大原文标量数,q.最大知识条目数,q.最大名称候选数,q.最大组合候选数},lang,words,names,es,fs);
        if(!collect(S02状态(r.状态),r.候选组))return result;
    }
    if(applicable.contains(U::S03整数)||applicable.contains(U::S03特征概念条件)||applicable.contains(U::S05组成)){
        const auto r=只读分派语言场景基础结构({q.语言节点,q.固定原文,q.未核验来源语言记录节点,q.最大知识条目数},c);
        if(!collect(r.状态,r.完整候选组))return result;
    }
    if(applicable.contains(U::S09归类确认)){
        const auto r=解释单句概念归类确认({q.固定原文,q.语言节点,q.未核验来源语言记录节点,{},q.最大原文标量数,q.最大知识条目数,q.最大名称候选数,q.最大组合候选数,q.最大确认候选数},lang,words,names,es,fs,instances);
        if(std::any_of(r.候选组.begin(),r.候选组.end(),[](const auto&x){return x.受询者存在节点.has_value();}))throw std::logic_error("unexpected S09 addressee");
        if(!collect(S09状态(r.状态),r.候选组))return result;
    }
    std::sort(result.完整候选组.begin(),result.完整候选组.end(),[](const auto&a,const auto&b){return 统一候选键(a,true)<统一候选键(b,true);});
    if(result.完整候选组.empty())return result;
    std::set<U> forms;std::set<统一键> semantics;std::set<std::vector<稳定编码>> rules;
    for(const auto&x:result.完整候选组){forms.insert(候选构式(x));auto key=统一候选键(x);auto semantic=key;
        if(key.构式==U::S09归类确认){rules.insert({key.身份[0],key.身份[1],key.身份.back()});semantic.身份.erase(semantic.身份.begin());semantic.身份.erase(semantic.身份.begin());semantic.身份.pop_back();}
        else{rules.insert({key.身份[0]});semantic.身份.erase(semantic.身份.begin());}
        semantics.insert(semantic);
    }
    if(forms.size()>1)result.状态=I::构式多义待解;
    else if(semantics.size()>1)result.状态=I::概念多义待解;
    else if(rules.size()>1)result.状态=I::多规则待解;
    else if(std::any_of(branchstates.begin(),branchstates.end(),[](I s){return s==I::概念多义待解||s==I::多规则待解||s==I::构式多义待解;}))
        result.状态=*std::find_if(branchstates.begin(),branchstates.end(),[](I s){return s==I::概念多义待解||s==I::多规则待解||s==I::构式多义待解;});
    else result.状态=I::已形成且读回;
    return result;
}
const 基础概念上下位候选& 纯概念主体(const 语言基础纯概念含义& p){
    if(const auto*c=std::get_if<基础概念上下位候选>(&p.解释))return *c;
    return std::get<基础概念归类确认候选>(p.解释).被询归类;
}
稳定编码 纯概念知识(const 语言基础纯概念含义& p){
    if(const auto*c=std::get_if<基础概念上下位候选>(&p.解释))return c->所用知识节点;
    return std::get<基础概念归类确认候选>(p.解释).外层知识节点;
}
struct 对齐读取结果 final { I 状态; std::vector<语言基础统一对齐> 对齐组; };
对齐读取结果 构造纯概念对齐(const std::string& text,稳定编码 lang,
    const std::variant<基础概念上下位候选,基础概念归类确认候选>& v,const 新语言知识信息& k,
    const std::vector<新语言知识信息>& ks,const 只读上下文& c){
    const auto leaves=展开知识(k,ks);const auto layout=匹配叶布局(text,lang,leaves,c);
    if(layout.状态!=I::已形成且读回)return {layout.状态,{}};
    const auto& body=std::holds_alternative<基础概念上下位候选>(v) ? std::get<基础概念上下位候选>(v):std::get<基础概念归类确认候选>(v).被询归类;
    std::vector<语言基础统一对齐> result;
    for(std::size_t i=0;i<leaves.size();++i){const auto&l=leaves[i];const auto&p=layout.项组[i];
        if(l.固定词)result.push_back({p.范围,l.路径,l.固定词,{},{},{},{}});
        else{const bool left=*l.角色==0;
            if(!范围相同(p.范围,left ? body.主体范围:body.归类范围))return {I::输入拒绝,{}};
            result.push_back({p.范围,l.路径,p.词条,left ? body.主体名称关系节点:body.归类名称关系节点,left ? body.主体概念节点:body.归类概念节点,*l.角色+1,{}});}
    }
    return {I::已形成且读回,std::move(result)};
}
I 核验纯概念来源(const 语言基础纯概念含义& p,稳定编码 lang,
    const std::vector<新语言知识信息>& ks,const 只读上下文& c){
    const auto*k=找知识(ks,纯概念知识(p));if(!k)return I::缺规则;
    const bool question=std::holds_alternative<基础概念归类确认候选>(p.解释);
    if(知识构式(*k,lang)!=(question ? U::S09归类确认:U::S02上下位))return I::输入拒绝;
    const auto&b=纯概念主体(p);
    if(!有效(b.主体概念节点)||!有效(b.归类概念节点)||!有效(b.主体名称关系节点)||!有效(b.归类名称关系节点)
        ||(b.类别!=K::存在概念&&b.类别!=K::特征概念))return I::输入拒绝;
    for(const auto id:{b.主体概念节点,b.归类概念节点}){
        const auto E=c.存在概念服务.获取存在概念(id);const auto Finfo=c.特征概念服务.获取特征概念(id);
        if(E.has_value()==Finfo.has_value())return I::引用不可读;
        if((b.类别==K::存在概念)!=E.has_value())return I::角色不相容;
    }
    if(question){const auto&q=std::get<基础概念归类确认候选>(p.解释);
        if(q.受询者存在节点||q.子知识节点!=b.所用知识节点||q.外层知识节点!=k->节点)return I::输入拒绝;
        const auto*sub=std::get_if<新语言知识子规则项>(&k->定义.结构.有序项组[0].载荷);
        if(!sub||sub->子知识节点!=q.子知识节点)return I::输入拒绝;
        const auto*child=找知识(ks,q.子知识节点);if(!child||知识构式(*child,lang)!=U::S02上下位)return I::缺规则;
    }else if(b.所用知识节点!=k->节点)return I::输入拒绝;
    const auto map=解码语言场景原文(p.固定原文);if(!map)return I::输入拒绝;
    const auto leaves=展开知识(*k,ks);
    const auto status=核验叶覆盖(*map,p.来源对齐组,leaves,lang,c.词服务,c.名称服务);
    if(status!=I::已形成且读回)return status;
    for(const auto&a:p.来源对齐组)if(a.局部出现编号){const bool left=*a.局部出现编号==1;
        if(a.概念节点!=(left ? b.主体概念节点:b.归类概念节点)||a.名称关系节点!=(left ? b.主体名称关系节点:b.归类名称关系节点)||!范围相同(a.原文范围,left ? b.主体范围:b.归类范围))return I::输入拒绝;}
    if(question){const auto&q=std::get<基础概念归类确认候选>(p.解释);
        if(p.来源对齐组.size()!=5||!范围相同(q.确认词范围,p.来源对齐组.back().原文范围)
            ||q.子句范围.起始标量!=0||q.子句范围.结束标量!=p.来源对齐组[3].原文范围.结束标量)return I::输入拒绝;}
    return I::已形成且读回;
}
语言基础统一候选 含义候选(const 语言基础统一含义& m){return std::visit([](const auto&x)->语言基础统一候选{
    using T=std::decay_t<decltype(x)>;
    if constexpr(std::is_same_v<T,语言场景归类含义>)return 语言场景归类候选{x.主体来源概念节点,x.归类概念节点,x.所用知识节点,x.出现组};
    else if constexpr(std::is_same_v<T,语言场景基础含义>)return x.解释;
    else return std::visit([](const auto&v)->语言基础统一候选{return v;},x.解释);
},m);}
} // namespace

语言基础统一输入结果 转换单句基础语言到信息(const 语言基础统一输入请求& q,
    新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances){
    auto result=只读识别六类基础句型(q,lang,words,names,es,fs,instances);
    if(result.状态!=I::已形成且读回)return result;
    const auto again=只读识别六类基础句型(q,lang,words,names,es,fs,instances);
    if(again.状态!=I::已形成且读回||result.完整候选组.size()!=1||again.完整候选组.size()!=1
        ||!统一候选相同(result.完整候选组[0],again.完整候选组[0])){result.状态=I::读取漂移;result.完整候选组.clear();return result;}
    const auto selected=result.完整候选组[0];result.构式=候选构式(selected);
    if(std::holds_alternative<语言场景归类候选>(selected)){
        auto r=转换单句归类到空白语言场景({q.语言节点,q.固定原文,q.未核验来源语言记录节点,q.最大知识条目数},world,lang,words,names,es,fs);
        result.状态=S01状态(r.状态);result.已取得场景节点=r.已取得场景节点;
        result.完整候选组.clear();if(!技术未完成(result.状态))for(const auto&candidate:r.完整候选组)result.完整候选组.emplace_back(candidate);
        if(r.已取得实例节点){
            if(r.完整候选组.size()!=1)throw std::logic_error("S01 acquired instance without chosen source");
            result.已取得对象绑定组.push_back({1,*r.已取得实例节点,r.完整候选组[0].主体概念节点});
        }
        if(result.状态==I::写后读回未完成)result.完整候选组.clear();
        if(r.含义){语言基础统一含义 m=*r.含义;
            if(!统一候选相同(selected,含义候选(m))){result.状态=I::写后读回未完成;result.完整候选组.clear();return result;}result.含义=std::move(m);}
        return result;
    }
    if(std::holds_alternative<语言场景基础候选>(selected)){
        auto r=转换单句基础结构到空白语言场景({q.语言节点,q.固定原文,q.未核验来源语言记录节点,q.最大知识条目数},world,lang,words,names,es,fs);
        result.状态=r.状态;result.已取得场景节点=r.已取得场景节点;result.已取得对象绑定组=r.已取得对象绑定组;
        result.完整候选组.clear();if(!技术未完成(result.状态))for(const auto&candidate:r.完整候选组)result.完整候选组.emplace_back(candidate);
        if(result.状态==I::写后读回未完成)result.完整候选组.clear();
        if(r.含义){if(!统一候选相同(selected,r.含义->解释)){result.状态=I::写后读回未完成;result.完整候选组.clear();return result;}result.含义=*r.含义;}
        return result;
    }
    语言基础纯概念含义 meaning;meaning.固定原文=q.固定原文;meaning.未核验来源语言记录节点=q.未核验来源语言记录节点;
    if(const auto*b=std::get_if<基础概念上下位候选>(&selected))meaning.解释=*b;
    else meaning.解释=std::get<基础概念归类确认候选>(selected);
    const auto ks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(ks.状态!=读取状态::已完成){result.状态=输入映射(ks.状态);result.完整候选组.clear();return result;}
    const auto*k=找知识(ks.完整知识组,纯概念知识(meaning));if(!k){result.状态=I::读取漂移;result.完整候选组.clear();return result;}
    const 只读上下文 c{lang,words,names,es,fs};
    auto alignment=构造纯概念对齐(q.固定原文,q.语言节点,meaning.解释,*k,ks.完整知识组,c);
    if(alignment.状态!=I::已形成且读回){result.状态=alignment.状态;result.完整候选组.clear();return result;}
    meaning.来源对齐组=std::move(alignment.对齐组);
    result.状态=核验纯概念来源(meaning,q.语言节点,ks.完整知识组,c);
    if(result.状态==I::已形成且读回)result.含义=std::move(meaning);
    else result.完整候选组.clear();
    return result;
}

namespace {
struct 纯概念生成结果 final { O 状态=O::名称待选; std::vector<基础表达结果> 候选组; };
纯概念生成结果 生成纯概念子句(const 语言基础统一输出请求& q,const 基础概念上下位候选& b,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances){
    纯概念生成结果 result;
    const auto left=读取概念当前名称完整(b.主体概念节点,q.语言节点,words,names,es,fs);
    const auto right=读取概念当前名称完整(b.归类概念节点,q.语言节点,words,names,es,fs);
    if(left.状态!=读取状态::已完成||right.状态!=读取状态::已完成){result.状态=输出映射(输入映射(left.状态!=读取状态::已完成 ? left.状态:right.状态));return result;}
    if(left.完整普通名称材料组.size()>q.最大名称候选数||right.完整普通名称材料组.size()>q.最大名称候选数){result.状态=O::额度不足;return result;}
    for(const auto&a:left.完整普通名称材料组)for(const auto&z:right.完整普通名称材料组){
        基础表达请求 expr{};expr.语言节点=q.语言节点;expr.目的=新语言知识表达目的::陈述;expr.操作=M::概念归类;expr.作用域=新语言知识作用域::普通陈述;
        expr.参数组={{P::主体,b.类别,基础表达参数种类::概念,b.主体概念节点,a.名称关系.节点},{P::归类概念,b.类别,基础表达参数种类::概念,b.归类概念节点,z.名称关系.节点}};
        expr.最大知识条目数=q.最大知识条目数;expr.最大名称条目数=q.最大名称候选数;expr.最大输出标量数=q.最大输出标量数;
        const auto r=生成可唯一回读的概念上下位表达({expr,q.最大回读原文标量数,q.最大知识条目数,q.最大名称候选数,q.最大组合候选数},lang,words,names,es,fs,instances);
        if(r.状态==基础概念上下位可逆表达状态::已生成且可唯一回读&&r.可用表达){
            if(r.可用表达->知识节点==b.所用知识节点)result.候选组.push_back(*r.可用表达);
            else result.状态=O::回读含义不一致;
        }else{
            if(r.原始表达状态==基础表达状态::额度不足||r.回读状态==基础概念上下位输入状态::额度不足){result.状态=O::额度不足;result.候选组.clear();return result;}
            if(r.状态==基础概念上下位可逆表达状态::回读技术未完成
                ||r.原始表达状态==基础表达状态::工程未完成||r.原始表达状态==基础表达状态::内部不一致||r.原始表达状态==基础表达状态::读取漂移||r.原始表达状态==基础表达状态::引用不可读){
                result.状态=r.原始表达状态==基础表达状态::内部不一致 ? O::内部不一致:r.原始表达状态==基础表达状态::读取漂移 ? O::读取漂移:O::工程未完成;result.候选组.clear();return result;}
            result.状态=r.状态==基础概念上下位可逆表达状态::回读语义未闭合 ? O::回读多义:O::回读含义不一致;
        }
    }
    for(const auto&[id,previous]:std::array<std::pair<稳定编码,const 名称读取结果*>,2>{{{b.主体概念节点,&left},{b.归类概念节点,&right}}}){
        const auto now=读取概念当前名称完整(id,q.语言节点,words,names,es,fs);
        if(now.状态!=读取状态::已完成){result.状态=输出映射(输入映射(now.状态));result.候选组.clear();return result;}
        读取快照 x,y;x.名称=previous->完整普通名称材料组;y.名称=now.完整普通名称材料组;
        if(!快照相同(x,y)){result.状态=O::读取漂移;result.候选组.clear();return result;}
    }
    if(!result.候选组.empty())result.状态=O::已生成且唯一回读;
    return result;
}
语言基础统一输入请求 回读请求(const 语言基础统一输出请求&q,const std::string&text){return {q.语言节点,text,{},q.最大回读原文标量数,q.最大知识条目数,q.最大名称候选数,q.最大组合候选数,q.最大确认候选数};}
O 统一输出核验(const 语言基础统一输出请求&q,const std::string&text,const 语言基础统一候选& expected,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances,
    std::optional<语言基础统一候选>& actual){
    const auto map=解码语言场景原文(text);if(!map)return O::内部不一致;
    const auto n=map->标量字节边界.size()-1;
    if(n>q.最大输出标量数||n>q.最大回读原文标量数)return O::额度不足;
    const auto read=只读识别六类基础句型(回读请求(q,text),lang,words,names,es,fs,instances);
    if(read.状态!=I::已形成且读回)return 输出映射(read.状态);
    if(read.完整候选组.size()!=1)return O::回读多义;
    if(统一候选键(expected)!=统一候选键(read.完整候选组[0]))return O::回读含义不一致;
    actual=read.完整候选组[0];return O::已生成且唯一回读;
}
语言基础统一输出结果 生成无受询者概念归类确认文字(const 语言基础统一输出请求&q,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances){
    语言基础统一输出结果 result;const auto&p=std::get<语言基础纯概念含义>(q.含义);const auto&question=std::get<基础概念归类确认候选>(p.解释);
    if(question.受询者存在节点)return result;
    const auto ks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(ks.状态!=读取状态::已完成){result.状态=输出映射(输入映射(ks.状态));return result;}
    const auto*k=找知识(ks.完整知识组,question.外层知识节点);if(!k||知识构式(*k,q.语言节点)!=U::S09归类确认){result.状态=O::缺规则;return result;}
    const auto*sub=std::get_if<新语言知识子规则项>(&k->定义.结构.有序项组[0].载荷);
    if(!sub||sub->子知识节点!=question.子知识节点||question.子知识节点!=question.被询归类.所用知识节点)return result;
    const auto&fixed=std::get<新语言知识固定词项>(k->定义.结构.有序项组[1].载荷);
    const auto term=words.获取词条(fixed.词条节点);
    if(term.状态!=新词条查询状态::已找到||!term.信息||term.信息->语言节点!=q.语言节点||term.信息->规范化词面!="吗"){result.状态=O::引用不可读;return result;}
    const auto children=生成纯概念子句(q,question.被询归类,lang,words,names,es,fs,instances);
    if(children.状态!=O::已生成且唯一回读){result.状态=children.状态;return result;}
    const 只读上下文 c{lang,words,names,es,fs};
    struct Good{bool space;std::string text;std::vector<语言基础统一对齐> align;};std::vector<Good> good;
    O last=O::回读含义不一致;
    for(const auto&child:children.候选组)for(const bool space:{false,true}){
        const auto text=child.文本+(space ? " ":"")+term.信息->规范化词面;
        std::optional<语言基础统一候选> actual;
        const auto state=统一输出核验(q,text,question,lang,words,names,es,fs,instances,actual);
        if(state==O::额度不足||state==O::工程未完成||state==O::内部不一致||state==O::引用不可读||state==O::读取漂移){result.状态=state;return result;}
        if(state!=O::已生成且唯一回读){last=state;continue;}
        const auto&read=std::get<基础概念归类确认候选>(*actual);
        bool selected=true;
        for(const auto&a:child.已用绑定){if(a.参数==P::主体&&a.选用名称关系!=read.被询归类.主体名称关系节点)selected=false;
            if(a.参数==P::归类概念&&a.选用名称关系!=read.被询归类.归类名称关系节点)selected=false;}
        if(!selected){last=O::回读含义不一致;continue;}
        auto align=构造纯概念对齐(text,q.语言节点,read,*k,ks.完整知识组,c);
        if(align.状态!=I::已形成且读回){
            if(技术未完成(align.状态)){result.状态=输出映射(align.状态);return result;}
            last=输出映射(align.状态);continue;
        }
        good.push_back({space,text,std::move(align.对齐组)});
    }
    const auto finalks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(finalks.状态!=读取状态::已完成){result.状态=输出映射(输入映射(finalks.状态));return result;}
    if(finalks.完整知识组.size()!=ks.完整知识组.size()||!std::equal(ks.完整知识组.begin(),ks.完整知识组.end(),finalks.完整知识组.begin(),知识相同)){result.状态=O::读取漂移;return result;}
    if(good.empty()){result.状态=last;return result;}
    std::sort(good.begin(),good.end(),[](const auto&a,const auto&b){return std::tie(a.space,a.text)<std::tie(b.space,b.text);});
    result.文本=good[0].text;result.输出对齐组=good[0].align;result.构式=U::S09归类确认;result.所用知识节点组={question.外层知识节点,question.子知识节点};result.状态=O::已生成且唯一回读;return result;
}
} // namespace

语言基础统一输出结果 从基础信息生成可回读单句(const 语言基础统一输出请求&q,
    const 新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances){
    语言基础统一输出结果 result;
    if(!有效(q.语言节点)||!q.最大知识条目数||!q.最大名称候选数||!q.最大输出标量数||!q.最大回读原文标量数||!q.最大组合候选数||!q.最大确认候选数)return result;
    const auto ks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(ks.状态!=读取状态::已完成){result.状态=输出映射(输入映射(ks.状态));return result;}
    const auto expected=含义候选(q.含义);const auto form=候选构式(expected);const 只读上下文 c{lang,words,names,es,fs};
    std::string text;
    if(const auto*p=std::get_if<语言基础纯概念含义>(&q.含义)){
        const auto status=核验纯概念来源(*p,q.语言节点,ks.完整知识组,c);
        if(status!=I::已形成且读回){result.状态=输出映射(status);return result;}
        if(form==U::S09归类确认)return 生成无受询者概念归类确认文字(q,lang,words,names,es,fs,instances);
        const auto generated=生成纯概念子句(q,std::get<基础概念上下位候选>(p->解释),lang,words,names,es,fs,instances);
        if(generated.状态!=O::已生成且唯一回读){result.状态=generated.状态;return result;}
        std::vector<std::string> usable;O last=O::回读含义不一致;
        for(const auto&g:generated.候选组){std::optional<语言基础统一候选> actual;
            const auto state=统一输出核验(q,g.文本,expected,lang,words,names,es,fs,instances,actual);
            if(state==O::额度不足||state==O::工程未完成||state==O::内部不一致||state==O::引用不可读||state==O::读取漂移){result.状态=state;return result;}
            if(state==O::已生成且唯一回读)usable.push_back(g.文本);else last=state;}
        if(usable.empty()){result.状态=last;return result;}std::sort(usable.begin(),usable.end());text=usable[0];
    }else if(const auto*p=std::get_if<语言场景归类含义>(&q.含义)){
        const auto r=从语言场景归类含义生成可回读短句({q.语言节点,*p,q.最大知识条目数},world,lang,words,names,es,fs);
        if(r.状态!=语言场景归类输出状态::已生成且唯一回读){result.状态=S01输出状态(r.状态);return result;}text=r.文本;
    }else{
        const auto r=从语言场景基础含义生成可回读短句({q.语言节点,std::get<语言场景基础含义>(q.含义),q.最大知识条目数},world,lang,words,names,es,fs);
        if(r.状态!=O::已生成且唯一回读){result.状态=r.状态;return result;}text=r.文本;
    }
    std::optional<语言基础统一候选> actual;result.状态=统一输出核验(q,text,expected,lang,words,names,es,fs,instances,actual);
    if(result.状态!=O::已生成且唯一回读)return result;
    const auto finalks=读取语言场景基础知识完整(q.语言节点,q.最大知识条目数,lang,words,es);
    if(finalks.状态!=读取状态::已完成){result.状态=输出映射(输入映射(finalks.状态));return result;}
    if(finalks.完整知识组.size()!=ks.完整知识组.size()||!std::equal(ks.完整知识组.begin(),ks.完整知识组.end(),finalks.完整知识组.begin(),知识相同)){result.状态=O::读取漂移;return result;}
    const auto*k=找知识(ks.完整知识组,统一候选键(*actual).身份[0]);
    if(!k){result.状态=O::读取漂移;return result;}
    if(const auto*p=std::get_if<语言场景基础候选>(&*actual))result.输出对齐组=基础对齐(*p,*k);
    else if(const auto*p=std::get_if<语言场景归类候选>(&*actual)){
        const auto leaves=展开知识(*k,ks.完整知识组);const auto layout=匹配叶布局(text,q.语言节点,leaves,c);
        if(layout.状态!=I::已形成且读回){result.状态=输出映射(layout.状态);return result;}
        for(std::size_t i=0;i<leaves.size();++i){const auto&l=leaves[i];const auto&a=layout.项组[i];
            if(l.固定词)result.输出对齐组.push_back({a.范围,l.路径,l.固定词,{},{},{},{}});
            else{const auto&o=p->出现组.at(static_cast<std::size_t>(*l.角色));result.输出对齐组.push_back({a.范围,l.路径,o.词条节点,o.名称关系节点,o.概念节点,o.局部出现编号,{}});}}
    }else {
        auto alignment=构造纯概念对齐(text,q.语言节点,std::get<基础概念上下位候选>(*actual),*k,ks.完整知识组,c);
        if(alignment.状态!=I::已形成且读回){result.状态=输出映射(alignment.状态);return result;}
        result.输出对齐组=std::move(alignment.对齐组);
    }
    if(result.输出对齐组.empty()){result.状态=O::读取漂移;return result;}
    result.文本=text;result.构式=form;result.所用知识节点组={k->节点};return result;
}
} // namespace 海中鱼巣
