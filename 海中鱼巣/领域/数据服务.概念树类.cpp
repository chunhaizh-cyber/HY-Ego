#include "数据服务.概念树类.h"
#include "数据服务.特征值域比较类.h"

#include <functional>
#include <map>
#include <tuple>

namespace {
海中鱼巣::L1所有者范围当前读取结果 读取任一当前事实(
    const 海中鱼巣::L1事实基座服务& l1,海中鱼巣::稳定编码 id) {
    using namespace 海中鱼巣;
    const L1所有者范围事实读取请求 request{L1所有者范围CRUD合同版本,id};
    const std::array reads{l1.读取所有者范围当前节点(request),
        l1.读取所有者范围当前关系(request),l1.读取所有者范围当前值(request)};
    std::optional<L1所有者范围当前读取结果> hit;
    for(const auto&read:reads){
        if(read.状态==L1所有者范围读取状态::成功){if(hit)return {};
            hit=read;
        }else if(read.状态!=L1所有者范围读取状态::未找到)return read;
    }
    return hit?*hit:reads.front();
}
struct 当前关系组读取 final {
    海中鱼巣::L1所有者范围读取状态 状态;
    std::uint64_t 读取事实代次;
    std::vector<海中鱼巣::L1所有者范围关系事实> 关系组;
};
当前关系组读取 读取当前关系组(const 海中鱼巣::L1事实基座服务& l1,
                              bool incoming, 海中鱼巣::稳定编码 id,
                              海中鱼巣::稳定编码 type) {
    using namespace 海中鱼巣;
    if (incoming) {
        const auto result=l1.读取所有者范围当前目标关系组(
            {L1所有者范围CRUD合同版本,id,type});
        return {result.状态,result.读取事实代次,result.关系组};
    }
    const auto result=l1.读取所有者范围当前源关系组(
        {L1所有者范围CRUD合同版本,id,type});
    return {result.状态,result.读取事实代次,result.关系组};
}
bool 纯概念结构相同(const 海中鱼巣::纯概念结构交付_v2& a,
                 const 海中鱼巣::纯概念结构交付_v2& b) noexcept {
    const auto& x=a.类型; const auto& y=b.类型;
    return a.版本==b.版本 && a.格式锚点==b.格式锚点 && a.概念族锚点==b.概念族锚点 &&
        x.类型登记==y.类型登记 && x.概念族成员==y.概念族成员 && x.格式版本==y.格式版本 &&
        x.概念类别==y.概念类别 && x.定义成员==y.定义成员 && x.定义种类==y.定义种类 &&
        x.定义特征类型==y.定义特征类型 && x.定义模板==y.定义模板 && x.I64域==y.I64域 &&
        x.通用规则==y.通用规则 && x.直接上位==y.直接上位 && x.生命周期==y.生命周期 &&
        x.存在概念使用==y.存在概念使用;
}
bool 出生结构身份独立(const 海中鱼巣::特征概念出生使用结构交付& b,
                    const 海中鱼巣::纯概念结构交付_v2& p) noexcept {
    if (!海中鱼巣::有效(b.锚点) || !海中鱼巣::有效(b.F到FCv出生使用关系类型) ||
        b.锚点==b.F到FCv出生使用关系类型) return false;
    const auto& t=p.类型;
    const std::array<海中鱼巣::稳定编码,15> ids{p.格式锚点,p.概念族锚点,
        t.类型登记,t.概念族成员,t.格式版本,t.概念类别,t.定义成员,t.定义种类,
        t.定义特征类型,t.定义模板,t.I64域,t.通用规则,t.直接上位,t.生命周期,t.存在概念使用};
    return std::none_of(ids.begin(),ids.end(),[&](auto id) { return id==b.锚点 || id==b.F到FCv出生使用关系类型; });
}

bool 已发布纯概念结构_v2(const 海中鱼巣::L1事实基座服务& l1,
                         const 海中鱼巣::L1所有者范围写端口& port,
                         const 海中鱼巣::纯概念结构交付_v2& layout,
                         std::uint64_t g) noexcept {
    using namespace 海中鱼巣;
    try {
        if (layout.版本 != 2 || !g || !port.有效() || !port.绑定于(l1)) return false;
        const auto& t=layout.类型;
        const std::array<稳定编码,15> ids{layout.格式锚点,layout.概念族锚点,
            t.类型登记,t.概念族成员,t.格式版本,t.概念类别,t.定义成员,t.定义种类,
            t.定义特征类型,t.定义模板,t.I64域,t.通用规则,t.直接上位,t.生命周期,t.存在概念使用};
        std::set<std::uint64_t> unique;
        for (std::size_t i=0;i<ids.size();++i) {
            if (!有效(ids[i]) || !unique.insert(ids[i].值).second) return false;
            const auto read=读取任一当前事实(l1,ids[i]);
            if (read.状态!=L1所有者范围读取状态::成功 || read.读取事实代次!=g ||
                !read.事实) return false;
            const auto* node=std::get_if<L1所有者范围节点事实>(&*read.事实);
            std::optional<L1所有者范围值表示种类> representation;
            const auto key=i+1;
            if (key==5||key==6||key==8||key==12||key==14) representation=L1所有者范围值表示种类::I64;
            else if (key==11) representation=L1所有者范围值表示种类::U64组;
            if (!node || node->写入所有者!=port.所有者身份() ||
                node->种类!=(representation ? 节点种类::属性类型 : 节点种类::普通) ||
                node->属性类型表示!=representation) return false;
        }
        const auto edges=l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本,ids[0],ids[2]});
        if (edges.状态!=L1所有者范围读取状态::成功 || edges.读取事实代次!=g ||
            edges.关系组.size()<13) return false;
        std::array<bool,13> seen{};
        for (const auto& edge : edges.关系组) {
            // pure v2 的类型登记关系允许后继扩展使用其它具名角色；
            // 基础交付只核验并唯一覆盖角色 1..13。
            if (edge.角色或顺序 < 1 || edge.角色或顺序 > 13) continue;
            const auto slot=static_cast<std::size_t>(edge.角色或顺序-1);
            if (edge.写入所有者!=port.所有者身份() || edge.源节点!=ids[0] ||
                edge.目标节点!=ids[slot+2] || edge.关系类型节点!=ids[2] ||
                seen[slot]) return false;
            seen[slot]=true;
        }
        if (std::ranges::any_of(seen,[](bool value){return !value;})) return false;
        const L1所有者范围所属节点当前完整值组读取请求_v2 valuesRequest{
            L1所有者范围所属节点当前完整值组读取合同版本_v2,
            port.所有者身份(),ids[0],g};
        const auto values=l1.读取所有者范围所属节点当前完整值组(valuesRequest);
        if (values.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功 ||
            values.读取事实代次!=g || values.载荷.size()!=1) return false;
        const auto& value=values.载荷.front();
        if (value.写入所有者!=port.所有者身份() || value.所属节点!=ids[0] ||
            value.来源节点!=ids[0] || value.属性类型节点!=ids[4] ||
            !std::holds_alternative<std::int64_t>(value.材料) ||
            std::get<std::int64_t>(value.材料)!=2) return false;
        return true;
    } catch (...) { return false; }
}

bool 已发布出生使用结构(const 海中鱼巣::L1事实基座服务& l1,
                        const 海中鱼巣::L1所有者范围写端口& port,
                        const 海中鱼巣::纯概念结构交付_v2& pure,
                        const 海中鱼巣::特征概念出生使用结构交付& delivery,
                        std::uint64_t g) noexcept {
    using namespace 海中鱼巣;
    try {
        if (delivery.格式!=1 || !出生结构身份独立(delivery,pure)) return false;
        const std::array<稳定编码,2> nodes{delivery.锚点,delivery.F到FCv出生使用关系类型};
        for (const auto id:nodes) {
            const auto read=读取任一当前事实(l1,id);
            const auto* node=read.事实 ? std::get_if<L1所有者范围节点事实>(&*read.事实) : nullptr;
            if(read.状态!=L1所有者范围读取状态::成功||read.读取事实代次!=g||!node||
               node->写入所有者!=port.所有者身份()||node->种类!=节点种类::普通||
               node->属性类型表示) return false;
        }
        const auto edges=l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本,delivery.锚点,pure.类型.类型登记});
        if(edges.状态!=L1所有者范围读取状态::成功||edges.读取事实代次!=g||
           edges.关系组.size()!=1) return false;
        const auto& edge=edges.关系组.front();
        if(edge.写入所有者!=port.所有者身份()||edge.源节点!=delivery.锚点||
           edge.目标节点!=delivery.F到FCv出生使用关系类型||
           edge.关系类型节点!=pure.类型.类型登记||edge.角色或顺序!=1||
           edge.创建事实代次>g) return false;
        const L1所有者范围所属节点当前完整值组读取请求_v2 valuesRequest{
            L1所有者范围所属节点当前完整值组读取合同版本_v2,
            port.所有者身份(),delivery.锚点,g};
        const auto values=l1.读取所有者范围所属节点当前完整值组(valuesRequest);
        if(values.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功||
           values.读取事实代次!=g) return false;
        std::size_t count=0;
        for(const auto& value:values.载荷) if(value.属性类型节点==pure.类型.格式版本) {
            ++count;
            if(value.写入所有者!=port.所有者身份()||value.所属节点!=delivery.锚点||
               value.来源节点!=delivery.锚点||
               !std::holds_alternative<std::int64_t>(value.材料)||
               std::get<std::int64_t>(value.材料)!=1) return false;
        }
        return count==1;
    } catch(...) { return false; }
}

bool 已发布存在两组结构(const 海中鱼巣::L1事实基座服务& l1,
                        const 海中鱼巣::L1所有者范围写端口& port,
                        const 海中鱼巣::纯概念结构交付_v2& pure,
                        const 海中鱼巣::存在概念两组结构交付_v1& delivery,
                        std::uint64_t g) noexcept {
    using namespace 海中鱼巣;
    try {
        const auto& t=delivery.类型;
        const std::array<稳定编码,6> ids{delivery.格式锚点,t.两组定义成员,
            t.自身特征值域项,t.自身项特征类型,t.自身项值域概念,t.已知子存在概念};
        if(delivery.版本!=1) return false;
        std::set<std::uint64_t> unique;
        for(const auto id:ids) {
            if(!有效(id)||!unique.insert(id.值).second) return false;
            const auto read=读取任一当前事实(l1,id);
            const auto* node=read.事实 ? std::get_if<L1所有者范围节点事实>(&*read.事实) : nullptr;
            if(read.状态!=L1所有者范围读取状态::成功||read.读取事实代次!=g||!node||
               node->写入所有者!=port.所有者身份()||node->种类!=节点种类::普通||
               node->属性类型表示) return false;
        }
        const auto edges=l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本,delivery.格式锚点,pure.类型.类型登记});
        if(edges.状态!=L1所有者范围读取状态::成功||edges.读取事实代次!=g||
           edges.关系组.size()!=5) return false;
        std::array<bool,5> seen{};
        for(const auto& edge:edges.关系组) {
            if(edge.角色或顺序<1||edge.角色或顺序>5) return false;
            const auto slot=static_cast<std::size_t>(edge.角色或顺序-1);
            if(seen[slot]||edge.写入所有者!=port.所有者身份()||
               edge.源节点!=delivery.格式锚点||edge.目标节点!=ids[slot+1]||
               edge.关系类型节点!=pure.类型.类型登记) return false;
            seen[slot]=true;
        }
        const L1所有者范围所属节点当前完整值组读取请求_v2 valuesRequest{
            L1所有者范围所属节点当前完整值组读取合同版本_v2,
            port.所有者身份(),delivery.格式锚点,g};
        const auto values=l1.读取所有者范围所属节点当前完整值组(valuesRequest);
        if(values.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功||
           values.读取事实代次!=g) return false;
        std::size_t count=0;
        for(const auto& value:values.载荷) if(value.属性类型节点==pure.类型.格式版本) {
            ++count;
            if(value.写入所有者!=port.所有者身份()||value.所属节点!=delivery.格式锚点||
               value.来源节点!=delivery.格式锚点||
               !std::holds_alternative<std::int64_t>(value.材料)||
               std::get<std::int64_t>(value.材料)!=1) return false;
        }
        return count==1;
    } catch(...) { return false; }
}

bool 已发布二次关系结构(const 海中鱼巣::L1事实基座服务& l1,
                        const 海中鱼巣::L1所有者范围写端口& port,
                        const 海中鱼巣::纯概念结构交付_v2& pure,
                        const 海中鱼巣::二次关系结构交付& delivery,
                        std::uint64_t g) noexcept {
    using namespace 海中鱼巣;
    try {
        const auto& t=delivery.类型;
        const std::array<稳定编码,22> types{
            t.规范化规则归属,t.规则版本,t.定义种类,t.定义格式,t.域掩码,
            t.输出角色,t.固定K,t.约束成员,t.约束FC,t.约束EC,t.合取成员,
            t.子RC,t.来源成员,t.来源F,t.来源B,t.来源概念,
            t.用途成员,t.用途目标,t.用途业务依据,t.用途业务标识,t.用途角色,
            t.用途时间};
        if(delivery.版本!=1||!有效(delivery.锚点)||
           !有效(delivery.规范化规则.值)) return false;
        std::set<std::uint64_t> unique{delivery.锚点.值};
        if(!unique.insert(delivery.规范化规则.值.值).second) return false;
        for(std::size_t i=0;i<types.size();++i) {
            const auto id=types[i];
            if(!有效(id)||!unique.insert(id.值).second) return false;
            const auto read=读取任一当前事实(l1,id);
            const auto* node=read.事实 ? std::get_if<L1所有者范围节点事实>(&*read.事实) : nullptr;
            std::optional<L1所有者范围值表示种类> representation;
            const auto position=i+1;
            if(position==2||position==3||position==4||position==5||position==6||
               position==21||position==22)
                representation=L1所有者范围值表示种类::I64;
            else if(position==20)
                representation=L1所有者范围值表示种类::U64组;
            if(read.状态!=L1所有者范围读取状态::成功||read.读取事实代次!=g||!node||
               node->写入所有者!=port.所有者身份()||
               node->种类!=(representation ? 节点种类::属性类型 : 节点种类::普通)||
               node->属性类型表示!=representation) return false;
        }
        for(const auto id:{delivery.锚点,delivery.规范化规则.值}) {
            const auto read=读取任一当前事实(l1,id);
            const auto* node=read.事实 ? std::get_if<L1所有者范围节点事实>(&*read.事实) : nullptr;
            if(read.状态!=L1所有者范围读取状态::成功||read.读取事实代次!=g||!node||
               node->写入所有者!=port.所有者身份()||node->种类!=节点种类::普通||
               node->属性类型表示) return false;
        }
        const auto locating=l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本,pure.格式锚点,pure.类型.类型登记});
        if(locating.状态!=L1所有者范围读取状态::成功||locating.读取事实代次!=g||
           locating.关系组.empty()) return false;
        std::size_t locatingCount=0;
        for(const auto& edge:locating.关系组) if(edge.角色或顺序==4242) {
            ++locatingCount;
            if(edge.写入所有者!=port.所有者身份()||edge.源节点!=pure.格式锚点||
               edge.目标节点!=delivery.锚点||
               edge.关系类型节点!=pure.类型.类型登记) return false;
        }
        if(locatingCount!=1) return false;
        const auto registrations=l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本,delivery.锚点,pure.类型.类型登记});
        if(registrations.状态!=L1所有者范围读取状态::成功||
           registrations.读取事实代次!=g||
           registrations.关系组.size()!=22) return false;
        std::array<bool,22> seen{};
        for(const auto& edge:registrations.关系组) {
            if(edge.角色或顺序<1||edge.角色或顺序>22) return false;
            const auto slot=static_cast<std::size_t>(edge.角色或顺序-1);
            if(seen[slot]||edge.写入所有者!=port.所有者身份()||
               edge.源节点!=delivery.锚点||edge.目标节点!=types[slot]||
               edge.关系类型节点!=pure.类型.类型登记) return false;
            seen[slot]=true;
        }
        const auto rules=l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本,delivery.锚点,t.规范化规则归属});
        if(rules.状态!=L1所有者范围读取状态::成功||rules.读取事实代次!=g||
           rules.关系组.size()!=1) return false;
        const auto& rule=rules.关系组.front();
        if(rule.写入所有者!=port.所有者身份()||rule.源节点!=delivery.锚点||
           rule.目标节点!=delivery.规范化规则.值||
           rule.关系类型节点!=t.规范化规则归属||rule.角色或顺序!=1||
           rule.创建事实代次>g) return false;
        const auto hasI64=[&](稳定编码 node,稳定编码 type) {
            const L1所有者范围所属节点当前完整值组读取请求_v2 request{
                L1所有者范围所属节点当前完整值组读取合同版本_v2,
                port.所有者身份(),node,g};
            const auto values=l1.读取所有者范围所属节点当前完整值组(request);
            if(values.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功||
               values.读取事实代次!=g) return false;
            std::size_t count=0;
            for(const auto& value:values.载荷) if(value.属性类型节点==type) {
                ++count;
                if(value.写入所有者!=port.所有者身份()||value.所属节点!=node||
                   value.来源节点!=node||
                   !std::holds_alternative<std::int64_t>(value.材料)||
                   std::get<std::int64_t>(value.材料)!=1) return false;
            }
            return count==1;
        };
        return hasI64(delivery.锚点,pure.类型.格式版本)&&
               hasI64(delivery.规范化规则.值,pure.类型.格式版本)&&
               hasI64(delivery.规范化规则.值,t.规则版本);
    } catch(...) { return false; }
}
// 出生使用扩展必须由纯概念交付定位其登记类型；不得扫描其它扩展。
bool 可登记出生使用(const 海中鱼巣::特征概念出生使用结构登记请求& r) noexcept {
    return r.版本==1 && r.G0!=0 && 海中鱼巣::有效(r.幂等键)
        && r.最大首次材料项数>=4 && r.纯概念结构.版本==2
        && 海中鱼巣::有效(r.纯概念结构.格式锚点)
        && 海中鱼巣::有效(r.纯概念结构.类型.类型登记)
        && 海中鱼巣::有效(r.纯概念结构.类型.格式版本);
}
}

namespace 海中鱼巣 {

// 诊断责任：低层状态保留错误种类并向公开边界送出。
概念树数据状态 概念树类数据服务::映射(L1所有者范围读取状态 s) {
    switch (s) {
    case L1所有者范围读取状态::入口拒绝:
        return S::入口拒绝;
    case L1所有者范围读取状态::未找到:
        return S::未找到;
    case L1所有者范围读取状态::事实代次漂移:
        return S::事实代次漂移;
    case L1所有者范围读取状态::资源失败:
        return S::资源失败;
    default:
        return S::内部不一致;
    }
}

void 概念树类数据服务::预算有效(const 概念树预算 &b) {
    if (!b.最大概念数 || !b.最大关系数 || !b.最大来源数 || !b.最大支持数 || !b.最大世界成员数 ||
        !b.最大特征属性数)
        throw 失败{S::入口拒绝};
}
void 概念树类数据服务::检查数量(std::size_t n, std::uint64_t max) {
    if (n > max)
        throw 失败{S::数量预算不足};
}
std::uint64_t 概念树类数据服务::当前代次() const {
    const auto r = l1_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (r.状态 != L1中性读取状态::成功)
        throw 失败{r.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致};
    if (r.合同版本 != L1中性CRUD合同版本 || !r.事实代次)
        throw 失败{S::内部不一致};
    return r.事实代次;
}
void 概念树类数据服务::守卫代次(std::uint64_t g) const {
    if (当前代次() != g)
        throw 失败{S::事实代次漂移};
}
L1所有者范围事实副本 概念树类数据服务::原始事实(稳定编码 id, std::uint64_t g) const {
    if (!有效(id))
        throw 失败{S::入口拒绝};
    const auto r = 读取任一当前事实(l1_,id);
    if (r.状态 != L1所有者范围读取状态::成功)
        throw 失败{映射(r.状态)};
    if (r.合同版本 != L1所有者范围CRUD合同版本 || r.查询编码 != id || !r.事实)
        throw 失败{S::内部不一致};
    if (r.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    std::visit(
        [&](const auto &v) {
            if (v.编码 != id || v.写入所有者 != port_.所有者身份())
                throw 失败{S::引用冲突};
            if (!v.创建事实代次 || v.创建事实代次 > g)
                throw 失败{S::内部不一致};
        },
        *r.事实);
    return *r.事实;
}
概念树类数据服务::N 概念树类数据服务::节点(
    稳定编码 id, std::uint64_t g,
    概念事实读取会话_v1* session) const {
    if (session) {
        if (session->所有者_ != port_.所有者身份() || session->Gread_ != g ||
            !session->预算_.最大节点数 || !session->预算_.最大关系数 ||
            !session->预算_.最大值数 || !session->预算_.最大材料总数)
            throw 失败{S::入口拒绝};
        if (const auto it=session->节点缓存_.find(id.值); it!=session->节点缓存_.end())
            return it->second;
        if (session->用量_.节点数 >= session->预算_.最大节点数 ||
            session->用量_.材料总数 >= session->预算_.最大材料总数)
            throw 失败{S::数量预算不足};
        auto f=原始事实(id,g);
        auto* n=std::get_if<N>(&f);
        if(!n) throw 失败{S::引用冲突};
        if(n->创建事实代次>g) throw 失败{S::未找到};
        ++session->用量_.节点数;
        ++session->用量_.材料总数;
        session->节点缓存_.emplace(id.值,*n);
        return *n;
    }
    auto f = 原始事实(id, g);
    auto *n = std::get_if<N>(&f);
    if (!n)
        throw 失败{S::引用冲突};
    if (n->创建事实代次 > g)
        throw 失败{S::未找到};
    return *n;
}
std::vector<概念树类数据服务::E> 概念树类数据服务::关系(稳定编码 id, 稳定编码 type, bool incoming,
                                                         std::uint64_t g,
                                                         std::uint64_t budget,
                                                         概念事实读取会话_v1* session) const {
    std::vector<E> loaded;
    if(session) {
        if(session->所有者_!=port_.所有者身份()||session->Gread_!=g||
           !session->预算_.最大节点数||!session->预算_.最大关系数||
           !session->预算_.最大值数||!session->预算_.最大材料总数||!budget)
            throw 失败{S::入口拒绝};
        const 概念事实读取会话_v1::关系键 key{id.值,type.值,incoming};
        if(const auto it=session->关系组缓存_.find(key);it!=session->关系组缓存_.end()) {
            loaded=it->second;
        } else {
            const auto remainingRelations=session->预算_.最大关系数-session->用量_.关系数;
            const auto remainingTotal=session->预算_.最大材料总数-session->用量_.材料总数;
            const auto maximum=std::min({budget,remainingRelations,remainingTotal});
            if(!maximum) throw 失败{S::数量预算不足};
            const auto r=读取当前关系组(l1_,incoming,id,type);
            if(r.状态!=L1所有者范围读取状态::成功) throw 失败{映射(r.状态)};
            if(r.读取事实代次!=g||r.关系组.size()>maximum) throw 失败{r.读取事实代次!=g ? S::事实代次漂移 : S::数量预算不足};
            for(const auto& e:r.关系组)
                if(e.写入所有者!=port_.所有者身份()) throw 失败{S::引用冲突};
            session->用量_.关系数+=static_cast<std::uint64_t>(r.关系组.size());
            session->用量_.材料总数+=static_cast<std::uint64_t>(r.关系组.size());
            loaded=r.关系组;
            session->关系组缓存_.emplace(key,loaded);
        }
        检查数量(loaded.size(),budget);
        std::sort(loaded.begin(),loaded.end(),[](const auto&a,const auto&b){
            return a.角色或顺序!=b.角色或顺序 ? a.角色或顺序<b.角色或顺序 : a.编码.值<b.编码.值;
        });
        return loaded;
    }
    const auto r = 读取当前关系组(l1_,incoming,id,type);
    if (r.状态 != L1所有者范围读取状态::成功)
        throw 失败{映射(r.状态)};
    if (r.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    检查数量(r.关系组.size(), budget);
    auto out = r.关系组;
    std::set<std::uint64_t> ids;
    for (const auto &e : out) {
        if (!有效(e.编码) || !有效(e.源节点) || !有效(e.目标节点) || e.关系类型节点 != type ||
            (incoming ? e.目标节点 : e.源节点) != id || e.写入所有者 != port_.所有者身份() ||
            !ids.insert(e.编码.值).second)
            throw 失败{S::内部不一致};
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) {
        return a.角色或顺序 != b.角色或顺序 ? a.角色或顺序 < b.角色或顺序 : a.编码.值 < b.编码.值;
    });
    return out;
}
std::vector<概念树类数据服务::V> 概念树类数据服务::属性(
    稳定编码 id, std::uint64_t g,
    概念事实读取会话_v1* session) const {
    if(session) {
        if(session->所有者_!=port_.所有者身份()||session->Gread_!=g||
           !session->预算_.最大节点数||!session->预算_.最大关系数||
           !session->预算_.最大值数||!session->预算_.最大材料总数)
            throw 失败{S::入口拒绝};
        if(const auto it=session->属性值组缓存_.find(id.值);it!=session->属性值组缓存_.end())
            return it->second;
        const auto remainingValues=session->预算_.最大值数-session->用量_.值数;
        const auto remainingTotal=session->预算_.最大材料总数-session->用量_.材料总数;
        const auto maximum=std::min(remainingValues,remainingTotal);
        if(!maximum) throw 失败{S::数量预算不足};
        const L1所有者范围所属节点当前完整值组读取请求_v2 request{
            L1所有者范围所属节点当前完整值组读取合同版本_v2,
            port_.所有者身份(),id,g};
        const auto r=l1_.读取所有者范围所属节点当前完整值组(request);
        if(r.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功)
            throw 失败{r.状态==L1所有者范围所属节点当前完整值组读取状态_v2::事实代次漂移 ? S::事实代次漂移 :
                       r.状态==L1所有者范围所属节点当前完整值组读取状态_v2::资源失败 ? S::资源失败 : S::内部不一致};
        if(r.读取事实代次!=g||r.载荷.size()>maximum) throw 失败{r.读取事实代次!=g ? S::事实代次漂移 : S::数量预算不足};
        std::set<std::uint64_t> slots;
        for(const auto& v:r.载荷)
            if(!有效(v.属性类型节点)||v.来源节点!=id||v.写入所有者!=port_.所有者身份()||
               !slots.insert(v.属性类型节点.值).second)
                throw 失败{S::内部不一致};
        session->用量_.值数+=static_cast<std::uint64_t>(r.载荷.size());
        session->用量_.材料总数+=static_cast<std::uint64_t>(r.载荷.size());
        session->属性值组缓存_.emplace(id.值,r.载荷);
        return r.载荷;
    }
    const L1所有者范围所属节点当前完整值组读取请求_v2 request{
        L1所有者范围所属节点当前完整值组读取合同版本_v2,
        port_.所有者身份(),id,g};
    const auto r = l1_.读取所有者范围所属节点当前完整值组(request);
    if (r.状态 != L1所有者范围所属节点当前完整值组读取状态_v2::成功)
        throw 失败{r.状态==L1所有者范围所属节点当前完整值组读取状态_v2::事实代次漂移 ? S::事实代次漂移 :
                   r.状态==L1所有者范围所属节点当前完整值组读取状态_v2::资源失败 ? S::资源失败 : S::内部不一致};
    if (r.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    if (r.合同版本 != request.合同版本 || r.所有者 != request.所有者 ||
        r.所属节点 != id || r.期望事实代次 != g)
        throw 失败{S::内部不一致};
    std::set<std::uint64_t> slots;
    for (const auto &v : r.载荷)
        if (!有效(v.编码) || v.所属节点 != id || v.来源节点 != id || v.写入所有者 != port_.所有者身份() ||
            !slots.insert(v.属性类型节点.值).second)
            throw 失败{S::内部不一致};
    return r.载荷;
}
std::vector<概念树类数据服务::E> 概念树类数据服务::完整关系组(
    稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t g) const {
    const auto result = 读取当前关系组(l1_,incoming,id,type);
    if (result.状态 != L1所有者范围读取状态::成功)
        throw 失败{映射(result.状态)};
    if (result.读取事实代次 != g) throw 失败{S::事实代次漂移};
    auto out = result.关系组;
    std::set<std::uint64_t> ids;
    for (const auto &edge : out) {
        if (!有效(edge.编码) || !有效(edge.源节点) || !有效(edge.目标节点) ||
            edge.关系类型节点 != type ||
            (incoming ? edge.目标节点 : edge.源节点) != id ||
            edge.写入所有者 != port_.所有者身份() ||
            !ids.insert(edge.编码.值).second)
            throw 失败{S::内部不一致};
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) {
        return a.角色或顺序 != b.角色或顺序
                   ? a.角色或顺序 < b.角色或顺序
                   : a.编码.值 < b.编码.值;
    });
    return out;
}
std::vector<概念树类数据服务::V> 概念树类数据服务::完整属性值组(
    稳定编码 id, std::uint64_t g) const {
    const L1所有者范围所属节点当前完整值组读取请求_v2 request{
        L1所有者范围所属节点当前完整值组读取合同版本_v2,
        port_.所有者身份(),id,g};
    const auto result = l1_.读取所有者范围所属节点当前完整值组(request);
    if (result.状态 != L1所有者范围所属节点当前完整值组读取状态_v2::成功)
        throw 失败{result.状态==L1所有者范围所属节点当前完整值组读取状态_v2::事实代次漂移 ? S::事实代次漂移 :
                   result.状态==L1所有者范围所属节点当前完整值组读取状态_v2::资源失败 ? S::资源失败 : S::内部不一致};
    if (result.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    std::set<std::uint64_t> slots;
    for (const auto &value : result.载荷)
        if (!有效(value.编码) || value.所属节点 != id || value.来源节点 != id ||
            value.写入所有者 != port_.所有者身份() ||
            !slots.insert(value.属性类型节点.值).second)
            throw 失败{S::内部不一致};
    return result.载荷;
}
const 概念树类数据服务::V &概念树类数据服务::唯一属性(const std::vector<V> &values, 稳定编码 type) {
    const V *found = nullptr;
    for (const auto &v : values)
        if (v.属性类型节点 == type) {
            if (found)
                throw 失败{S::内部不一致};
            found = &v;
        }
    if (!found)
        throw 失败{S::内部不一致};
    return *found;
}
概念树截止结果 概念树类数据服务::读取当前事实代次() const {
    try {
        return {1, S::已读取, 当前代次()};
    } catch (const 失败 &x) {
        return {1, x.状态, 0};
    } catch (const std::bad_alloc &) {
        return {1, S::资源失败, 0};
    } catch (const std::length_error &) {
        return {1, S::资源失败, 0};
    } catch (...) {
        return {1, S::内部不一致, 0};
    }
}
概念树定义 概念树类数据服务::规范化定义(const 概念树定义 &input) const {
    auto out = input;
    std::visit(
        [&](auto &d) {
            using T = std::decay_t<decltype(d)>;
            if constexpr (std::is_same_v<T, 概念树特征定义>) {
                if (!有效(d.形成宿主.值) || !有效(d.特征类型.值))
                    throw 失败{S::入口拒绝};
                if (auto *interval = std::get_if<概念树I64区间>(&d.值域)) {
                    if (interval->下界 > interval->上界)
                        throw 失败{S::入口拒绝};
                    if (interval->下界 == interval->上界)
                        d.值域 = 概念树精确值{interval->下界};
                }
                if (auto *exact = std::get_if<概念树精确值>(&d.值域))
                    std::visit(
                        [](const auto &v) {
                            if constexpr (!std::is_same_v<std::decay_t<decltype(v)>, std::int64_t>)
                                if (v.empty())
                                    throw 失败{S::入口拒绝};
                        },
                        *exact);
            } else if constexpr (std::is_same_v<T, 概念树存在定义>) {
                if (d.特征模板组.empty())
                    throw 失败{S::入口拒绝};
                std::sort(d.特征模板组.begin(), d.特征模板组.end(), 身份小于);
                for (std::size_t i = 0; i < d.特征模板组.size(); ++i)
                    if (!有效(d.特征模板组[i].值) || (i && d.特征模板组[i] == d.特征模板组[i - 1]))
                        throw 失败{S::入口拒绝};
            }
        },
        out);
    return out;
}
void 概念树类数据服务::验证世界(const 概念树形成世界引用 &w, std::uint64_t g,
                                 const 概念树预算 &b) const {
    if (!g || !有效(世界编码(w)))
        throw 失败{S::入口拒绝};
    std::visit(
        [&](const auto &x) {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, 概念树存在引用>) {
                const auto r = existences_.确认当前存在结构身份(g, x.值);
                if (!r.成功(g)) {
                    if (r.状态 == 存在结构身份只读状态::未找到)
                        throw 失败{S::未找到};
                    if (r.状态 == 存在结构身份只读状态::事实代次漂移)
                        throw 失败{S::事实代次漂移};
                    if (r.状态 == 存在结构身份只读状态::资源失败)
                        throw 失败{S::资源失败};
                    throw 失败{S::内部不一致};
                }
                if (r.Gread != g)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树特征引用>) {
                const auto r = 特征结果(features_.读取准确特征事实({1,g,{x.值}}));
                if (r.Gread != g || r.信息.身份.编码 != x.值)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树特征类型引用>) {
                const auto r = 特征结果(features_.读取I64基础特征类型事实({1,g,{x.值}}));
                if (r.Gread != g || r.数据.身份.编码 != x.值)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树场景引用>) {
                守卫代次(g);
                throw 失败{S::不支持};
            }
        },
        w);
}

概念树数据状态 概念树类数据服务::共享映射(特征数据错误 s) {
    switch (s) {
    case 特征数据错误::未找到: return S::未找到;
    case 特征数据错误::未设置: case 特征数据错误::规则缺失: return S::规则缺失;
    case 特征数据错误::入口拒绝: return S::入口拒绝;
    case 特征数据错误::类型不相容: return S::类型不相容;
    case 特征数据错误::能力未提供: return S::不支持;
    case 特征数据错误::旧格式不支持: return S::旧格式不支持;
    case 特征数据错误::引用冲突: return S::引用冲突;
    case 特征数据错误::算术不可表示: return S::差异不可表示;
    case 特征数据错误::并发变化: return S::事实代次漂移;
    case 特征数据错误::资源失败: return S::资源失败;
    case 特征数据错误::幂等冲突: return S::幂等冲突;
    case 特征数据错误::数量预算不足: return S::数量预算不足;
    case 特征数据错误::前次写入待收敛: return S::前次写入待收敛;
    default: return S::内部不一致;
    }
}

void 概念树类数据服务::核验外部节点(稳定编码 id,std::uint64_t g) const {
    const auto r = 读取任一当前事实(l1_,id);
    if (r.读取事实代次 != g) throw 失败{S::事实代次漂移};
    if (r.状态 != L1所有者范围读取状态::成功) throw 失败{映射(r.状态)};
    const auto* n = r.事实 ? std::get_if<N>(&*r.事实) : nullptr;
    if (r.合同版本 != L1所有者范围CRUD合同版本 || r.查询编码 != id || !n || n->编码 != id)
        throw 失败{S::引用冲突};
    if (!n->创建事实代次 || n->创建事实代次 > g) throw 失败{S::内部不一致};
}

概念树类数据服务::概念树类数据服务(
    const L1事实基座服务& l1, const 特征类数据服务& features,
    const 存在类数据服务& existences, const 特征值类数据服务& values,
    const 场景类数据服务& scenes, L1所有者范围写端口&& port,
    const 纯概念结构交付_v2& layout,
    const 特征概念出生使用结构交付& birthLayout,
    const 存在概念两组结构交付_v1& twoGroupLayout)
    : l1_(l1), features_(features), existences_(existences), values_(values),
      port_(std::move(port)), scenes_(&scenes), pure_layout_(layout),
      feature_birth_layout_(birthLayout), two_group_definition_layout_(twoGroupLayout) {
    if (!port_.有效() || !port_.绑定于(l1_) || !features_.绑定于(l1_) ||
        !existences_.绑定于(l1_) || !values_.绑定于(l1_) || !scenes.绑定于(l1_) ||
        layout.版本 != 2 || twoGroupLayout.版本 != 1)
        throw std::invalid_argument("concept two-group services");
}

概念树类数据服务::概念树类数据服务(
    const L1事实基座服务& l1, const 特征类数据服务& features,
    const 存在类数据服务& existences, const 特征值类数据服务& values,
    const 场景类数据服务& scenes, L1所有者范围写端口&& port,
    const 纯概念结构交付_v2& layout,
    const 特征概念出生使用结构交付& birthLayout,
    const 存在概念两组结构交付_v1& twoGroupLayout,
    const 二次关系结构交付& secondaryRelationLayout,
    const 状态使用绑定只读提供者& stateUseBindings)
    : l1_(l1), features_(features), existences_(existences), values_(values),
      port_(std::move(port)), scenes_(&scenes), pure_layout_(layout),
      feature_birth_layout_(birthLayout), two_group_definition_layout_(twoGroupLayout),
      secondary_relation_layout_(secondaryRelationLayout),
      state_use_bindings_(&stateUseBindings) {
    const auto types = std::array<稳定编码, 22>{
        secondaryRelationLayout.类型.规范化规则归属,
        secondaryRelationLayout.类型.规则版本,
        secondaryRelationLayout.类型.定义种类,
        secondaryRelationLayout.类型.定义格式,
        secondaryRelationLayout.类型.域掩码,
        secondaryRelationLayout.类型.输出角色,
        secondaryRelationLayout.类型.固定K,
        secondaryRelationLayout.类型.约束成员,
        secondaryRelationLayout.类型.约束FC,
        secondaryRelationLayout.类型.约束EC,
        secondaryRelationLayout.类型.合取成员,
        secondaryRelationLayout.类型.子RC,
        secondaryRelationLayout.类型.来源成员,
        secondaryRelationLayout.类型.来源F,
        secondaryRelationLayout.类型.来源B,
        secondaryRelationLayout.类型.来源概念,
        secondaryRelationLayout.类型.用途成员,
        secondaryRelationLayout.类型.用途目标,
        secondaryRelationLayout.类型.用途业务依据,
        secondaryRelationLayout.类型.用途业务标识,
        secondaryRelationLayout.类型.用途角色,
        secondaryRelationLayout.类型.用途时间};
    std::set<std::uint64_t> unique;
    unique.insert(secondaryRelationLayout.锚点.值);
    unique.insert(secondaryRelationLayout.规范化规则.值.值);
    for (const auto id : types)
        if (!有效(id) || !unique.insert(id.值).second)
            throw std::invalid_argument("secondary relation layout");
    if (!port_.有效() || !port_.绑定于(l1_) || !features_.绑定于(l1_) ||
        !existences_.绑定于(l1_) || !values_.绑定于(l1_) ||
        !scenes.绑定于(l1_) || !stateUseBindings.绑定于(l1_) ||
        &stateUseBindings !=
            static_cast<const 状态使用绑定只读提供者 *>(&scenes) ||
        layout.版本 != 2 || twoGroupLayout.版本 != 1 ||
        secondaryRelationLayout.版本 != 1 ||
        !有效(secondaryRelationLayout.锚点) ||
        !有效(secondaryRelationLayout.规范化规则.值))
        throw std::invalid_argument("secondary relation services");
    const auto current=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
    if(current.状态!=L1中性读取状态::成功||!current.事实代次||
       !已发布纯概念结构_v2(l1_,port_,layout,current.事实代次)||
       !已发布出生使用结构(l1_,port_,layout,birthLayout,current.事实代次)||
       !已发布存在两组结构(l1_,port_,layout,twoGroupLayout,current.事实代次)||
       !已发布二次关系结构(l1_,port_,layout,secondaryRelationLayout,
                              current.事实代次))
        throw std::invalid_argument("secondary relation published layouts");
}

namespace {
存在概念两组状态_v3 映射两组状态(纯概念状态 state) noexcept {
    switch (state) {
    case 纯概念状态::入口拒绝: return 存在概念两组状态_v3::入口拒绝;
    case 纯概念状态::未找到: return 存在概念两组状态_v3::未找到;
    case 纯概念状态::概念已退役: return 存在概念两组状态_v3::概念已退役;
    case 纯概念状态::类别冲突: return 存在概念两组状态_v3::类别冲突;
    case 纯概念状态::定义不相容: return 存在概念两组状态_v3::定义不相容;
    case 纯概念状态::定义不支持: return 存在概念两组状态_v3::规则缺失;
    case 纯概念状态::事实代次漂移: return 存在概念两组状态_v3::事实代次漂移;
    case 纯概念状态::幂等冲突: return 存在概念两组状态_v3::幂等冲突;
    case 纯概念状态::数量预算不足: return 存在概念两组状态_v3::数量预算不足;
    case 纯概念状态::资源失败: return 存在概念两组状态_v3::资源失败;
    case 纯概念状态::已可能发布: return 存在概念两组状态_v3::已可能发布;
    case 纯概念状态::旧格式不支持: return 存在概念两组状态_v3::旧格式不支持;
    default: return 存在概念两组状态_v3::内部不一致;
    }
}

纯概念状态 映射特征值域比较状态(特征值域比较状态_v1 state) noexcept {
    switch (state) {
    case 特征值域比较状态_v1::入口拒绝: return 纯概念状态::入口拒绝;
    case 特征值域比较状态_v1::未找到:
    case 特征值域比较状态_v1::类别冲突:
    case 特征值域比较状态_v1::类型不相容: return 纯概念状态::定义不相容;
    case 特征值域比较状态_v1::规则缺失:
    case 特征值域比较状态_v1::未实现: return 纯概念状态::定义不支持;
    case 特征值域比较状态_v1::事实代次漂移: return 纯概念状态::事实代次漂移;
    case 特征值域比较状态_v1::数量预算不足: return 纯概念状态::数量预算不足;
    case 特征值域比较状态_v1::资源失败: return 纯概念状态::资源失败;
    default: return 纯概念状态::内部不一致;
    }
}
存在概念两组完整读取状态_v2 映射两组完整状态(纯概念状态 state) noexcept {
    using R = 存在概念两组完整读取状态_v2;
    switch (state) {
    case 纯概念状态::入口拒绝: return R::入口拒绝;
    case 纯概念状态::未找到: return R::未找到;
    case 纯概念状态::概念已退役: return R::概念已退役;
    case 纯概念状态::类别冲突: return R::类别冲突;
    case 纯概念状态::定义不相容: return R::定义不相容;
    case 纯概念状态::定义不支持: return R::规则缺失;
    case 纯概念状态::事实代次漂移: return R::事实代次漂移;
    case 纯概念状态::资源失败: return R::资源失败;
    case 纯概念状态::旧格式不支持: return R::旧格式不支持;
    default: return R::内部不一致;
    }
}
纯概念状态 映射特征值域比较状态_v2(特征值域比较状态_v2 state) noexcept {
    switch (state) {
    case 特征值域比较状态_v2::入口拒绝: return 纯概念状态::入口拒绝;
    case 特征值域比较状态_v2::未找到:
    case 特征值域比较状态_v2::类别冲突:
    case 特征值域比较状态_v2::类型不相容: return 纯概念状态::定义不相容;
    case 特征值域比较状态_v2::规则缺失:
    case 特征值域比较状态_v2::未实现: return 纯概念状态::定义不支持;
    case 特征值域比较状态_v2::事实代次漂移: return 纯概念状态::事实代次漂移;
    case 特征值域比较状态_v2::资源失败: return 纯概念状态::资源失败;
    default: return 纯概念状态::内部不一致;
    }
}
}

存在概念两组定义_v3 概念树类数据服务::规范化两组定义内部(
    const 存在概念两组定义_v3& input, std::uint64_t g,
    const 存在概念两组预算_v3& budget, const 特征值域比较数据服务& comparison,
    const 特征值域比较预算_v1& comparisonBudget,
    概念事实读取会话_v1* session,
    特征值域事实读取会话_v1* featureSession) const {
    if (!two_group_definition_layout_ || !pure_layout_ || !comparison.绑定于(l1_) || !g ||
        !input.自身特征组已完整声明 || !input.子存在概念组已完整声明 ||
        !budget.最大自身特征项 || !budget.最大子概念项 || !budget.最大定义成员数 ||
        !budget.基础.最大概念数 || !budget.基础.最大关系数 || !budget.基础.最大特征属性数)
        throw 纯失败{纯概念状态::入口拒绝};
    if (input.自身特征值域组.size() > budget.最大自身特征项 ||
        input.已知子存在概念组.size() > budget.最大子概念项 ||
        input.自身特征值域组.size() + input.已知子存在概念组.size() > budget.最大定义成员数)
        throw 纯失败{纯概念状态::数量预算不足};
    auto out=input;
    std::sort(out.自身特征值域组.begin(),out.自身特征值域组.end(),[](const auto&a,const auto&b) {
        return std::tie(a.FT.编码.值,a.FC.值.值) < std::tie(b.FT.编码.值,b.FC.值.值);
    });
    std::set<std::uint64_t> seenFeatureConcepts;
    for (std::size_t i=0;i<out.自身特征值域组.size();++i) {
        const auto& item=out.自身特征值域组[i];
        if (!有效(item.FT) || !有效(item.FC.值) ||
            (i && out.自身特征值域组[i-1].FT == item.FT) ||
            !seenFeatureConcepts.insert(item.FC.值.值).second)
            throw 纯失败{纯概念状态::定义不相容};
        const 特征值域读取请求_v1 request{1,g,item.FC,comparisonBudget};
        if(session&&featureSession) {
            if(session->用量_.节点数>session->预算_.最大节点数||
               session->用量_.关系数>session->预算_.最大关系数||
               session->用量_.值数>session->预算_.最大值数||
               session->用量_.材料总数>session->预算_.最大材料总数)
                throw 纯失败{纯概念状态::数量预算不足};
            featureSession->计量_.上限={
                featureSession->计量_.用量.节点数+
                    (session->预算_.最大节点数-session->用量_.节点数),
                featureSession->计量_.用量.关系数+
                    (session->预算_.最大关系数-session->用量_.关系数),
                featureSession->计量_.用量.属性值数+
                    (session->预算_.最大值数-session->用量_.值数),
                featureSession->计量_.用量.材料总数+
                    (session->预算_.最大材料总数-session->用量_.材料总数)};
        }
        const auto value=session&&featureSession ?
            comparison.读取特征值域共享(request,*session,*featureSession) :
            session ? comparison.读取特征值域共享(request,*session) :
                      comparison.读取特征值域(request);
        if (!value.成功(request) || !value.域 || value.域->FT != item.FT)
            throw 纯失败{value.成功(request) ? 纯概念状态::定义不相容 : 映射特征值域比较状态(value.状态)};
    }
    std::sort(out.已知子存在概念组.begin(),out.已知子存在概念组.end(),[](auto a,auto b){return a.值.值<b.值.值;});
    if (std::adjacent_find(out.已知子存在概念组.begin(),out.已知子存在概念组.end()) != out.已知子存在概念组.end())
        throw 纯失败{纯概念状态::定义不相容};
    for (const auto child:out.已知子存在概念组) {
        if (!有效(child.值)) throw 纯失败{纯概念状态::定义不相容};
        if(session&&featureSession) {
            if(featureSession->计量_.用量.节点数>featureSession->计量_.上限.最大节点数||
               featureSession->计量_.用量.关系数>featureSession->计量_.上限.最大关系数||
               featureSession->计量_.用量.属性值数>featureSession->计量_.上限.最大属性值数||
               featureSession->计量_.用量.材料总数>featureSession->计量_.上限.最大材料总数)
                throw 纯失败{纯概念状态::数量预算不足};
            session->预算_={session->用量_.节点数+
                    (featureSession->计量_.上限.最大节点数-featureSession->计量_.用量.节点数),
                session->用量_.关系数+
                    (featureSession->计量_.上限.最大关系数-featureSession->计量_.用量.关系数),
                session->用量_.值数+
                    (featureSession->计量_.上限.最大属性值数-featureSession->计量_.用量.属性值数),
                session->用量_.材料总数+
                    (featureSession->计量_.上限.最大材料总数-featureSession->计量_.用量.材料总数)};
        }
        const auto fact=读取纯概念内部(child,g,budget.基础,session,featureSession);
        if (fact.类别 != 相关概念类别::存在) throw 纯失败{纯概念状态::类别冲突};
        if (fact.治理状态 == 概念树生命周期状态::退役) throw 纯失败{纯概念状态::概念已退役};
    }
    return out;
}

存在概念两组规范化结果_v1 概念树类数据服务::规范化存在概念两组定义(
    const 存在概念两组规范化请求_v1& r, const 特征值域比较数据服务& comparison,
    const 特征值域比较预算_v1& comparisonBudget) const noexcept {
    存在概念两组规范化结果_v1 out; out.Gread=r.Gread;
    try {
        if(r.版本!=1 || !r.Gread) throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread); out.定义=规范化两组定义内部(r.定义,r.Gread,r.预算,comparison,comparisonBudget);
        out.状态=存在概念两组状态_v3::已规范化; 守卫代次(r.Gread);
    } catch(const 纯失败& e) { out.状态=映射两组状态(e.状态); out.定义.reset(); }
      catch(const std::bad_alloc&) { out.状态=存在概念两组状态_v3::资源失败; }
      catch(...) { out.状态=存在概念两组状态_v3::内部不一致; }
    return out;
}

存在概念两组事实_v3 概念树类数据服务::读取两组定义内部(
    概念树概念身份 id, std::uint64_t g,
    const 存在概念两组预算_v3& budget, const 特征值域比较数据服务& comparison,
    const 特征值域比较预算_v1& comparisonBudget,
    概念事实读取会话_v1* session,
    特征值域事实读取会话_v1* featureSession) const {
    if (!two_group_definition_layout_ || !pure_layout_ || !有效(id.值)) throw 纯失败{纯概念状态::旧格式不支持};
    if (!comparison.绑定于(l1_)) throw 纯失败{纯概念状态::入口拒绝};
    const auto base=读取纯概念内部(id,g,budget.基础,session,featureSession);
    if(base.类别 != 相关概念类别::存在) throw 纯失败{纯概念状态::类别冲突};
    if(base.治理状态 == 概念树生命周期状态::退役) throw 纯失败{纯概念状态::概念已退役};
    const auto& t=two_group_definition_layout_->类型;
    const auto definitions=关系(id.值,t.两组定义成员,false,g,2,session);
    if(definitions.size()!=1 || definitions.front().角色或顺序!=1) throw 纯失败{纯概念状态::内部不一致};
    const auto d=definitions.front().目标节点;
    const auto dNode=节点(d,g,session);
    if(dNode.写入所有者!=port_.所有者身份() || dNode.种类!=节点种类::普通 || dNode.属性类型表示)
        throw 纯失败{纯概念状态::内部不一致};
    存在概念两组事实_v3 out;
    out.概念=id; out.定义记录=d; out.定义记录生命周期=投影纯生命(dNode);
    out.状态生命周期=base.生命周期值生命周期; out.概念生命周期=base.生命周期;
    out.治理状态=base.治理状态; out.直接上位=base.直接上位;
    out.定义.自身特征组已完整声明=true; out.定义.子存在概念组已完整声明=true;
    const auto own=关系(d,t.自身特征值域项,false,g,budget.最大自身特征项+1,session);
    if(own.size()>budget.最大自身特征项) throw 纯失败{纯概念状态::数量预算不足};
    for(std::size_t i=0;i<own.size();++i) {
        const auto& edge=own[i];
        if(edge.角色或顺序!=static_cast<std::int64_t>(i+1)) throw 纯失败{纯概念状态::内部不一致};
        const auto itemNode=节点(edge.目标节点,g,session);
        if(itemNode.写入所有者!=port_.所有者身份() || itemNode.种类!=节点种类::普通 || itemNode.属性类型表示 || !属性(edge.目标节点,g,session).empty())
            throw 纯失败{纯概念状态::内部不一致};
        const auto ft=关系(edge.目标节点,t.自身项特征类型,false,g,2,session);
        const auto fc=关系(edge.目标节点,t.自身项值域概念,false,g,2,session);
        if(ft.size()!=1 || fc.size()!=1 || ft.front().角色或顺序!=1 || fc.front().角色或顺序!=1)
            throw 纯失败{纯概念状态::内部不一致};
        存在概念特征值域项_v3 item{特征类型身份{ft.front().目标节点},概念树概念身份{fc.front().目标节点}};
        out.定义.自身特征值域组.push_back(item);
        out.自身特征项关系组.push_back({edge.编码,edge.源节点,edge.目标节点,edge.关系类型节点,
            static_cast<std::uint64_t>(edge.角色或顺序),投影纯生命(edge)});
    }
    const auto children=关系(d,t.已知子存在概念,false,g,budget.最大子概念项+1,session);
    if(children.size()>budget.最大子概念项) throw 纯失败{纯概念状态::数量预算不足};
    for(std::size_t i=0;i<children.size();++i) {
        const auto& edge=children[i];
        if(edge.角色或顺序!=static_cast<std::int64_t>(i+1)) throw 纯失败{纯概念状态::内部不一致};
        out.定义.已知子存在概念组.push_back(概念树概念身份{edge.目标节点});
        out.子概念关系组.push_back({edge.编码,edge.源节点,edge.目标节点,edge.关系类型节点,
            static_cast<std::uint64_t>(edge.角色或顺序),投影纯生命(edge)});
    }
    const auto normalized=规范化两组定义内部(out.定义,g,budget,comparison,
        comparisonBudget,session,featureSession);
    if(normalized != out.定义) throw 纯失败{纯概念状态::内部不一致};
    return out;
}

存在概念两组读取结果_v1 概念树类数据服务::读取存在概念两组定义(
    const 存在概念两组读取请求_v1& r, const 特征值域比较数据服务& comparison,
    const 特征值域比较预算_v1& comparisonBudget) const noexcept {
    存在概念两组读取结果_v1 out; out.Gread=r.Gread;
    try {
        if(r.版本!=1 || !r.Gread || !有效(r.EC.值)) throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread); out.事实=读取两组定义内部(r.EC,r.Gread,r.预算,comparison,comparisonBudget);
        out.状态=存在概念两组状态_v3::已读取; 守卫代次(r.Gread);
    } catch(const 纯失败& e) { out.状态=映射两组状态(e.状态); out.事实.reset(); }
      catch(const std::bad_alloc&) { out.状态=存在概念两组状态_v3::资源失败; }
      catch(...) { out.状态=存在概念两组状态_v3::内部不一致; }
    return out;
}

存在概念两组事实_v3 概念树类数据服务::读取两组完整内部(
    概念树概念身份 id, std::uint64_t g,
    const 特征值域比较数据服务 &comparison,
    std::set<std::uint64_t> &active) const {
    if (!two_group_definition_layout_ || !pure_layout_ || !有效(id.值))
        throw 纯失败{纯概念状态::旧格式不支持};
    if (!comparison.绑定于(l1_) || !g)
        throw 纯失败{纯概念状态::入口拒绝};
    if (!active.insert(id.值.值).second)
        throw 纯失败{纯概念状态::内部不一致};
    struct ActiveGuard final {
        std::set<std::uint64_t> &set;
        std::uint64_t value;
        ~ActiveGuard() { set.erase(value); }
    } activeGuard{active, id.值.值};
    const auto base = 读取纯概念完整内部(id, g);
    if (base.类别 != 相关概念类别::存在)
        throw 纯失败{纯概念状态::类别冲突};
    if (base.治理状态 == 概念树生命周期状态::退役)
        throw 纯失败{纯概念状态::概念已退役};
    const auto &t = two_group_definition_layout_->类型;
    const auto definitions = 完整关系组(id.值, t.两组定义成员, false, g);
    if (definitions.size() != 1 || definitions.front().角色或顺序 != 1)
        throw 纯失败{纯概念状态::内部不一致};
    const auto definitionId = definitions.front().目标节点;
    const auto definitionNode = 节点(definitionId, g);
    if (definitionNode.写入所有者 != port_.所有者身份() ||
        definitionNode.种类 != 节点种类::普通 || definitionNode.属性类型表示)
        throw 纯失败{纯概念状态::内部不一致};

    存在概念两组事实_v3 out;
    out.概念 = id;
    out.定义记录 = definitionId;
    out.定义记录生命周期 = 投影纯生命(definitionNode);
    out.状态生命周期 = base.生命周期值生命周期;
    out.概念生命周期 = base.生命周期;
    out.治理状态 = base.治理状态;
    out.直接上位 = base.直接上位;
    out.定义.自身特征组已完整声明 = true;
    out.定义.子存在概念组已完整声明 = true;

    const auto own = 完整关系组(definitionId, t.自身特征值域项, false, g);
    std::set<std::uint64_t> seenFeatureConcepts;
    std::uint64_t previousFeatureType = 0;
    for (std::size_t i = 0; i < own.size(); ++i) {
        const auto &edge = own[i];
        if (edge.角色或顺序 != static_cast<std::int64_t>(i + 1))
            throw 纯失败{纯概念状态::内部不一致};
        const auto itemNode = 节点(edge.目标节点, g);
        if (itemNode.写入所有者 != port_.所有者身份() ||
            itemNode.种类 != 节点种类::普通 || itemNode.属性类型表示 ||
            !完整属性值组(edge.目标节点, g).empty())
            throw 纯失败{纯概念状态::内部不一致};
        const auto featureTypes =
            完整关系组(edge.目标节点, t.自身项特征类型, false, g);
        const auto featureConcepts =
            完整关系组(edge.目标节点, t.自身项值域概念, false, g);
        if (featureTypes.size() != 1 || featureConcepts.size() != 1 ||
            featureTypes.front().角色或顺序 != 1 ||
            featureConcepts.front().角色或顺序 != 1)
            throw 纯失败{纯概念状态::内部不一致};
        const 存在概念特征值域项_v3 item{
            特征类型身份{featureTypes.front().目标节点},
            概念树概念身份{featureConcepts.front().目标节点}};
        if (!有效(item.FT) || !有效(item.FC.值) ||
            item.FT.编码.值 <= previousFeatureType ||
            !seenFeatureConcepts.insert(item.FC.值.值).second)
            throw 纯失败{纯概念状态::内部不一致};
        const 特征值域读取请求_v2 domainRequest{2, g, item.FC};
        const auto domain = comparison.读取特征值域_v2(domainRequest);
        if (!domain.成功(domainRequest) || !domain.域 || domain.域->FT != item.FT)
            throw 纯失败{domain.成功(domainRequest)
                              ? 纯概念状态::定义不相容
                              : 映射特征值域比较状态_v2(domain.状态)};
        previousFeatureType = item.FT.编码.值;
        out.定义.自身特征值域组.push_back(item);
        out.自身特征项关系组.push_back(
            {edge.编码, edge.源节点, edge.目标节点, edge.关系类型节点,
             static_cast<std::uint64_t>(edge.角色或顺序), 投影纯生命(edge)});
    }

    const auto children = 完整关系组(definitionId, t.已知子存在概念, false, g);
    std::uint64_t previousChild = 0;
    for (std::size_t i = 0; i < children.size(); ++i) {
        const auto &edge = children[i];
        const 概念树概念身份 child{edge.目标节点};
        if (edge.角色或顺序 != static_cast<std::int64_t>(i + 1) ||
            !有效(child.值) || child.值.值 <= previousChild)
            throw 纯失败{纯概念状态::内部不一致};
        (void)读取两组完整内部(child, g, comparison, active);
        previousChild = child.值.值;
        out.定义.已知子存在概念组.push_back(child);
        out.子概念关系组.push_back(
            {edge.编码, edge.源节点, edge.目标节点, edge.关系类型节点,
             static_cast<std::uint64_t>(edge.角色或顺序), 投影纯生命(edge)});
    }
    return out;
}

存在概念两组完整读取结果_v2 概念树类数据服务::读取存在概念两组完整定义(
    const 存在概念两组完整读取请求_v2 &r,
    const 特征值域比较数据服务 &comparison) const noexcept {
    using RS = 存在概念两组完整读取状态_v2;
    存在概念两组完整读取结果_v2 out;
    out.Gread = r.Gread;
    const auto fail = [&](RS state) {
        out.状态 = state;
        out.事实.reset();
    };
    try {
        if (r.版本 != 2 || !r.Gread ||
            !有效(r.EC.值) || !comparison.绑定于(l1_)) {
            fail(RS::入口拒绝);
            return out;
        }
        守卫代次(r.Gread);
        std::set<std::uint64_t> active;
        out.事实 = 读取两组完整内部(r.EC, r.Gread, comparison, active);
        out.状态 = RS::已读取;
        守卫代次(r.Gread);
        if (!out.成功(r)) fail(RS::内部不一致);
    } catch (const 纯失败 &e) {
        fail(映射两组完整状态(e.状态));
    } catch (const 失败 &e) {
        fail(e.状态 == S::入口拒绝 ? RS::入口拒绝
             : e.状态 == S::未找到 ? RS::未找到
             : e.状态 == S::事实代次漂移 ? RS::事实代次漂移
             : e.状态 == S::资源失败 ? RS::资源失败
             : RS::内部不一致);
    } catch (const std::bad_alloc &) {
        fail(RS::资源失败);
    } catch (const std::length_error &) {
        fail(RS::资源失败);
    } catch (...) {
        fail(RS::内部不一致);
    }
    return out;
}

存在概念两组枚举结果_v1 概念树类数据服务::枚举存在概念候选(
    const 存在概念两组枚举请求_v1& r, const 特征值域比较数据服务& comparison,
    const 特征值域比较预算_v1& comparisonBudget) const noexcept {
    存在概念两组枚举结果_v1 out; out.Gread=r.Gread;
    try {
        if(r.版本!=1 || !r.Gread || !r.预算.最大候选数 || !pure_layout_ || !two_group_definition_layout_ || !comparison.绑定于(l1_))
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        const auto members=关系(pure_layout_->概念族锚点,pure_layout_->类型.概念族成员,false,r.Gread,r.预算.最大候选数+1);
        if(members.size()>r.预算.最大候选数) throw 纯失败{纯概念状态::数量预算不足};
        for(const auto& member:members) {
            if(member.角色或顺序!=1) throw 纯失败{纯概念状态::内部不一致};
            const auto definition=关系(member.目标节点,two_group_definition_layout_->类型.两组定义成员,
                                      false,r.Gread,2);
            if(definition.empty()) continue;
            if(definition.size()!=1 || definition.front().角色或顺序!=1)
                throw 纯失败{纯概念状态::内部不一致};
            const auto base=读取纯概念内部(概念树概念身份{member.目标节点},r.Gread,r.预算.基础);
            if(base.类别 != 相关概念类别::存在) continue;
            const auto fact=读取两组定义内部(概念树概念身份{member.目标节点},r.Gread,r.预算,comparison,comparisonBudget);
            out.候选.push_back(fact);
        }
        out.状态=存在概念两组状态_v3::已枚举; 守卫代次(r.Gread);
    } catch(const 纯失败& e) { out.状态=映射两组状态(e.状态); out.候选.clear(); }
      catch(const std::bad_alloc&) { out.状态=存在概念两组状态_v3::资源失败; out.候选.clear(); }
      catch(...) { out.状态=存在概念两组状态_v3::内部不一致; out.候选.clear(); }
    return out;
}

存在概念两组查询结果_v1 概念树类数据服务::精确查询存在概念(
    const 存在概念两组查询请求_v1& r, const 特征值域比较数据服务& comparison,
    const 特征值域比较预算_v1& comparisonBudget) const noexcept {
    存在概念两组查询结果_v1 out; out.Gread=r.Gread;
    try {
        if(r.版本!=1 || !r.Gread) throw 纯失败{纯概念状态::入口拒绝};
        const auto normalized=规范化两组定义内部(r.定义,r.Gread,r.预算,comparison,comparisonBudget);
        const auto candidates=枚举存在概念候选({1,r.Gread,r.预算},comparison,comparisonBudget);
        if(!candidates.成功({1,r.Gread,r.预算}))
            throw 纯失败{candidates.状态 == 存在概念两组状态_v3::数量预算不足 ? 纯概念状态::数量预算不足 :
                         (candidates.状态 == 存在概念两组状态_v3::事实代次漂移 ? 纯概念状态::事实代次漂移 :
                          (candidates.状态 == 存在概念两组状态_v3::资源失败 ? 纯概念状态::资源失败 : 纯概念状态::内部不一致))};
        for(const auto& fact:candidates.候选) if(fact.定义==normalized) {
            if(out.事实) throw 纯失败{纯概念状态::内部不一致};
            out.事实=fact;
        }
        out.状态=out.事实 ? 存在概念两组状态_v3::已读取 : 存在概念两组状态_v3::未找到;
    } catch(const 纯失败& e) { out.状态=映射两组状态(e.状态); out.事实.reset(); }
      catch(const std::bad_alloc&) { out.状态=存在概念两组状态_v3::资源失败; }
      catch(...) { out.状态=存在概念两组状态_v3::内部不一致; }
    return out;
}

存在概念两组写入结果_v1 概念树类数据服务::创建或复用存在概念(
    const 存在概念两组创建请求_v1& r, const 特征值域比较数据服务& comparison,
    const 特征值域比较预算_v1& comparisonBudget) noexcept {
    存在概念两组写入结果_v1 out; out.原请求=r;
    bool entered=false;
    try {
        std::scoped_lock lock(mutex_);
        if(r.版本!=1 || !r.G0 || r.G0==UINT64_MAX || !有效(r.幂等身份) || !pure_layout_ || !two_group_definition_layout_ || !comparison.绑定于(l1_))
            throw 纯失败{纯概念状态::入口拒绝};
        const auto current=读取当前事实代次();
        if(!current.成功()) throw 纯失败{纯概念状态::资源失败};
        out.Gread=current.Gread;
        const auto definition=规范化两组定义内部(r.定义,current.Gread,r.预算,comparison,comparisonBudget);
        auto parents=r.直接上位;
        std::sort(parents.begin(),parents.end(),[](auto a,auto b){return a.值.值<b.值.值;});
        if(std::adjacent_find(parents.begin(),parents.end())!=parents.end()) throw 纯失败{纯概念状态::定义不相容};
        for(const auto parent:parents) {
            const auto parentFact=读取两组定义内部(parent,current.Gread,r.预算,comparison,comparisonBudget);
            if(parentFact.治理状态==概念树生命周期状态::退役)
                throw 纯失败{纯概念状态::概念已退役};
        }
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本 || first.所有者!=port_.所有者身份() || first.写入幂等身份!=r.幂等身份)
            throw 纯失败{纯概念状态::内部不一致};
        if(first.状态==L1所有者范围读取状态::成功) {
            if(!first.首次写入结果 || !first.首次规范化写集 || first.首次写入结果->状态!=L1所有者范围写入状态::成功 ||
               !first.首次写入结果->是否形成内存权威发布) throw 纯失败{纯概念状态::已可能发布};
            稳定编码 id{}; for(const auto& [key,value]:first.首次写入结果->新编码映射) if(key.值==1) id=value;
            if(!有效(id)) throw 纯失败{纯概念状态::内部不一致};
            const auto fact=读取两组定义内部(概念树概念身份{id},current.Gread,r.预算,comparison,comparisonBudget);
            if(fact.定义!=definition) throw 纯失败{纯概念状态::幂等冲突};
            out.首次发布H=first.首次写入结果->事实代次; out.事实=fact;
            out.状态=存在概念两组状态_v3::精确重复; out.发布=纯概念发布状态::确认发布; return out;
        }
        if(first.状态!=L1所有者范围读取状态::未找到 || first.读取事实代次!=r.G0 || current.Gread!=r.G0)
            throw 纯失败{first.状态==L1所有者范围读取状态::资源失败 ? 纯概念状态::资源失败 : 纯概念状态::事实代次漂移};
        const auto found=精确查询存在概念({1,r.G0,definition,r.预算},comparison,comparisonBudget);
        if(found.成功({1,r.G0,definition,r.预算})) {
            out.事实=found.事实; out.状态=存在概念两组状态_v3::精确重复; out.发布=纯概念发布状态::确认未发布; return out;
        }
        if(!found.确认未找到({1,r.G0,definition,r.预算})) throw 纯失败{纯概念状态::内部不一致};
        const auto& pure=pure_layout_->类型; const auto& two=two_group_definition_layout_->类型;
        L1所有者范围写集请求 write{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
        write.节点.push_back({{1},节点种类::普通,std::nullopt});
        write.节点.push_back({{2},节点种类::普通,std::nullopt});
        write.节点.push_back({{3},节点种类::普通,std::nullopt});
        std::uint32_t nodeKey=4,edgeKey=0x41001,valueKey=0x42001;
        auto edge=[&](Ref source,Ref target,稳定编码 type,std::int64_t role=1){write.关系.push_back({{edgeKey++},source,target,Ref{type},role});};
        auto value=[&](Ref node,稳定编码 type,L1所有者范围原始值材料 material){const Key key{valueKey++};write.值.push_back({key,node,Ref{type},std::move(material),node});write.属性槽变更.push_back({node,Ref{type},key});};
        edge(Ref{pure_layout_->概念族锚点},Ref{Key{1}},pure.概念族成员);
        edge(Ref{Key{1}},Ref{Key{2}},pure.定义成员);
        edge(Ref{Key{1}},Ref{Key{3}},two.两组定义成员);
        value(Ref{Key{1}},pure.概念类别,std::int64_t{1});
        value(Ref{Key{1}},pure.生命周期,std::int64_t{static_cast<std::uint8_t>(概念树生命周期状态::活跃)});
        value(Ref{Key{2}},pure.定义种类,std::int64_t{3});
        value(Ref{Key{2}},pure.通用规则,std::int64_t{1});
        for(std::size_t i=0;i<definition.自身特征值域组.size();++i) {
            const Key item{nodeKey++}; write.节点.push_back({item,节点种类::普通,std::nullopt});
            edge(Ref{Key{3}},Ref{item},two.自身特征值域项,static_cast<std::int64_t>(i+1));
            edge(Ref{item},Ref{definition.自身特征值域组[i].FT.编码},two.自身项特征类型);
            edge(Ref{item},Ref{definition.自身特征值域组[i].FC.值},two.自身项值域概念);
        }
        for(std::size_t i=0;i<definition.已知子存在概念组.size();++i)
            edge(Ref{Key{3}},Ref{definition.已知子存在概念组[i].值},two.已知子存在概念,static_cast<std::int64_t>(i+1));
        for(const auto parent:parents) edge(Ref{parent.值},Ref{Key{1}},pure.直接上位);
        entered=true;
        const auto saved=port_.提交所有者范围中性写集(write); out.Gread=saved.事实代次;
        if(saved.状态!=L1所有者范围写入状态::成功 || saved.事实代次!=r.G0+1) {
            out.状态=saved.状态==L1所有者范围写入状态::事实代次漂移 ? 存在概念两组状态_v3::事实代次漂移 :
                (saved.状态==L1所有者范围写入状态::幂等冲突 ? 存在概念两组状态_v3::幂等冲突 : 存在概念两组状态_v3::已可能发布);
            out.发布=saved.是否形成内存权威发布 ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布; return out;
        }
        稳定编码 id{}; for(const auto& [key,candidate]:saved.新编码映射) if(key.值==1) id=candidate;
        if(!有效(id)) throw 纯失败{纯概念状态::已可能发布};
        const auto tail=读取当前事实代次(); if(!tail.成功() || tail.Gread<saved.事实代次) throw 纯失败{纯概念状态::已可能发布};
        out.Gread=tail.Gread; out.首次发布H=saved.事实代次;
        out.事实=读取两组定义内部(概念树概念身份{id},out.Gread,r.预算,comparison,comparisonBudget);
        if(out.事实->定义!=definition) throw 纯失败{纯概念状态::内部不一致};
        out.状态=存在概念两组状态_v3::已创建; out.发布=纯概念发布状态::确认发布;
    } catch(const 纯失败& e) {
        out.状态=entered ? 存在概念两组状态_v3::已可能发布 : 映射两组状态(e.状态);
        out.发布=entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
        if(out.状态!=存在概念两组状态_v3::已可能发布) {out.首次发布H.reset();out.事实.reset();}
    } catch(const std::bad_alloc&) {out.状态=entered?存在概念两组状态_v3::已可能发布:存在概念两组状态_v3::资源失败;out.发布=entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布;}
      catch(...) {out.状态=entered?存在概念两组状态_v3::已可能发布:存在概念两组状态_v3::内部不一致;out.发布=entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布;}
    return out;
}

存在概念两组结构登记结果_v1 概念树类数据服务::登记存在概念两组结构_v1(
    const L1事实基座服务& l1, L1所有者范围写端口& port,
    const 存在概念两组结构登记请求_v1& r) noexcept {
    存在概念两组结构登记结果_v1 out;
    out.原请求 = r;
    bool entered = false;
    const auto fail = [&](存在概念两组状态_v3 state) {
        out.状态 = state;
        out.发布 = entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
        if (state != 存在概念两组状态_v3::已可能发布) out.首次发布H.reset();
        out.交付.reset(); out.首次材料.reset();
    };
    try {
        if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX || !有效(r.幂等身份) ||
            !port.有效() || !port.绑定于(l1) || r.纯概念结构.版本 != 2 ||
            !有效(r.纯概念结构.格式锚点) || !有效(r.纯概念结构.类型.类型登记) ||
            !有效(r.纯概念结构.类型.格式版本) || r.最大首次材料项数 < 12) {
            fail(存在概念两组状态_v3::入口拒绝); return out;
        }
        const auto& pure = r.纯概念结构;
        const auto& type = pure.类型;
        L1所有者范围写集请求 write{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
        for (std::uint32_t key=1; key<=6; ++key)
            write.节点.push_back({{key},节点种类::普通,std::nullopt});
        for (std::uint32_t i=0; i<5; ++i)
            write.关系.push_back({{0x31001U+i},Key{1},Key{2U+i},type.类型登记,
                                  static_cast<std::int64_t>(i+1)});
        write.值.push_back({{0x32001},Key{1},type.格式版本,std::int64_t{1},Key{1}});
        write.属性槽变更.push_back({Key{1},type.格式版本,Key{0x32001}});

        const auto first=port.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.幂等身份});
        out.Gread=first.读取事实代次;
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
            first.所有者 != port.所有者身份() || first.写入幂等身份 != r.幂等身份) {
            fail(存在概念两组状态_v3::内部不一致); return out;
        }
        bool replay=false;
        if (first.状态 == L1所有者范围读取状态::成功) {
            replay=true;
            if (!first.首次规范化写集 || *first.首次规范化写集 != write) {
                fail(存在概念两组状态_v3::幂等冲突); return out;
            }
            if (!first.首次写入结果 || first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
                first.首次写入结果->事实代次 != r.G0+1 || !first.首次写入结果->是否形成内存权威发布) {
                entered=true; fail(存在概念两组状态_v3::已可能发布); return out;
            }
        } else if (first.状态 == L1所有者范围读取状态::未找到) {
            if (first.读取事实代次 != r.G0 || first.首次规范化写集 || first.首次写入结果) {
                fail(first.读取事实代次 != r.G0 ? 存在概念两组状态_v3::事实代次漂移 :
                     存在概念两组状态_v3::内部不一致); return out;
            }
        } else {
            fail(first.状态 == L1所有者范围读取状态::资源失败 ?
                 存在概念两组状态_v3::资源失败 : 存在概念两组状态_v3::内部不一致); return out;
        }
        const auto pureReadGeneration=replay ? first.读取事实代次 : r.G0;
        if (!已发布纯概念结构_v2(l1,port,r.纯概念结构,pureReadGeneration)) {
            fail(存在概念两组状态_v3::旧格式不支持); return out;
        }
        entered=true;
        const auto saved=port.提交所有者范围中性写集(write);
        out.Gread=saved.事实代次;
        if (saved.状态 != (replay ? L1所有者范围写入状态::精确重复 : L1所有者范围写入状态::成功) ||
            saved.合同版本 != L1所有者范围CRUD合同版本 || saved.所有者 != port.所有者身份() ||
            saved.写入幂等身份 != r.幂等身份 || saved.事实代次 != r.G0+1 || saved.新编码映射.size()!=12) {
            fail(saved.状态 == L1所有者范围写入状态::事实代次漂移 ? 存在概念两组状态_v3::事实代次漂移 :
                 (saved.状态 == L1所有者范围写入状态::幂等冲突 ? 存在概念两组状态_v3::幂等冲突 :
                  存在概念两组状态_v3::已可能发布)); return out;
        }
        const auto mapped=[&](std::uint64_t key) {
            std::optional<稳定编码> id;
            for(const auto& [k,v]:saved.新编码映射) if(k.值==key) { if(id||!有效(v)) throw 纯失败{纯概念状态::内部不一致}; id=v; }
            if(!id) throw 纯失败{纯概念状态::内部不一致}; return *id;
        };
        std::array<稳定编码,6> nodes{};
        for(std::uint64_t i=0;i<nodes.size();++i) nodes[i]=mapped(i+1);
        存在概念两组结构交付_v1 delivery{1,nodes[0],{nodes[1],nodes[2],nodes[3],nodes[4],nodes[5]}};
        const auto tail=l1.读取中性当前事实代次({L1中性CRUD合同版本});
        if(tail.状态!=L1中性读取状态::成功 || !tail.事实代次 || tail.事实代次<saved.事实代次) {
            fail(存在概念两组状态_v3::已可能发布); return out;
        }
        out.Gread=tail.事实代次; out.首次发布H=saved.事实代次;
        const auto raw=[&](稳定编码 id) {
            const auto read=读取任一当前事实(l1,id);
            if(read.状态!=L1所有者范围读取状态::成功 || read.读取事实代次!=out.Gread || !read.事实)
                throw 纯失败{纯概念状态::已可能发布};
            return *read.事实;
        };
        存在概念两组结构首次材料_v1 material;
        for(std::size_t i=0;i<nodes.size();++i) {
            const auto rawFact=raw(nodes[i]);
            const auto* fact=std::get_if<L1所有者范围节点事实>(&rawFact);
            if(!fact || fact->写入所有者!=port.所有者身份() || fact->种类!=节点种类::普通 ||
               fact->属性类型表示 || fact->创建事实代次!=saved.事实代次)
                throw 纯失败{纯概念状态::内部不一致};
            material.节点[i]=*fact;
        }
        for(std::uint64_t i=0;i<5;++i) {
            const auto rawFact=raw(mapped(0x31001U+i));
            const auto* fact=std::get_if<L1所有者范围关系事实>(&rawFact);
            if(!fact || fact->写入所有者!=port.所有者身份() || fact->源节点!=nodes[0] ||
               fact->目标节点!=nodes[i+1] || fact->关系类型节点!=type.类型登记 ||
               fact->角色或顺序!=static_cast<std::int64_t>(i+1) || fact->创建事实代次!=saved.事实代次)
                throw 纯失败{纯概念状态::内部不一致};
            material.类型登记关系[i]=*fact;
        }
        const auto rawValue=raw(mapped(0x32001));
        const auto* value=std::get_if<L1所有者范围值事实>(&rawValue);
        if(!value || value->写入所有者!=port.所有者身份() || value->所属节点!=nodes[0] ||
           value->来源节点!=nodes[0] || value->属性类型节点!=type.格式版本 ||
           !std::holds_alternative<std::int64_t>(value->材料) || std::get<std::int64_t>(value->材料)!=1 ||
           value->创建事实代次!=saved.事实代次)
            throw 纯失败{纯概念状态::内部不一致};
        material.格式值=*value; out.交付=delivery; out.首次材料=material;
        out.状态=replay ? 存在概念两组状态_v3::精确重复 : 存在概念两组状态_v3::已创建;
        out.发布=纯概念发布状态::确认发布;
        if(!out.成功(r)) throw 纯失败{纯概念状态::内部不一致};
    } catch(const 纯失败&) { fail(entered ? 存在概念两组状态_v3::已可能发布 : 存在概念两组状态_v3::内部不一致); }
      catch(const std::bad_alloc&) { fail(entered ? 存在概念两组状态_v3::已可能发布 : 存在概念两组状态_v3::资源失败); }
      catch(...) { fail(entered ? 存在概念两组状态_v3::已可能发布 : 存在概念两组状态_v3::内部不一致); }
    return out;
}



概念树类数据服务::概念树类数据服务(
    const L1事实基座服务 &l1, const 特征类数据服务 &features,
    const 存在类数据服务 &existences, const 特征值类数据服务 &values,
    const 场景类数据服务 &scenes, L1所有者范围写端口 &&port,
    const 相关概念结构交付 &layout)
    : l1_(l1), features_(features), existences_(existences), values_(values),
      port_(std::move(port)), scenes_(&scenes), related_layout_(layout) {
    if (!port_.有效() || !port_.绑定于(l1_) || !features_.绑定于(l1_) ||
        !existences_.绑定于(l1_) || !values_.绑定于(l1_) || !scenes.绑定于(l1_))
        throw std::invalid_argument("related concept provider binding mismatch");
    try { 校验相关概念布局(当前代次()); }
    catch (const 失败 &e) {
        if (e.状态 == S::资源失败) throw std::bad_alloc{};
        throw std::invalid_argument("related concept structure delivery is invalid");
    }
}


void 概念树类数据服务::校验相关概念布局(std::uint64_t g) const {
    if (!related_layout_ || related_layout_->版本 != 1 || related_layout_->格式 != 1)
        throw 失败{S::旧格式不支持};
    const auto owner = l1_.读取当前结构所有者(
        {L1所有者范围CRUD合同版本, port_.所有者身份()});
    if (owner.状态 != L1所有者范围读取状态::成功) throw 失败{映射(owner.状态)};
    if (owner.读取事实代次 != g) throw 失败{S::事实代次漂移};
    if (!owner.所有者事实 || owner.所有者事实->所有者 != port_.所有者身份() ||
        owner.所有者事实->范围种类 != L1所有者范围种类::独占结构范围)
        throw 失败{S::引用冲突};
    const auto &x = *related_layout_; const auto &t = x.类型;
    const std::array<稳定编码, 20> types{
        t.类型登记,t.概念族成员,t.概念类别,t.定义成员,t.定义种类,t.定义宿主,
        t.定义特征类型,t.定义模板,t.精确I64,t.精确I64组,t.精确U64组,
        t.区间下界,t.区间上界,t.直接上位,t.形成成员,t.形成存在,t.形成特征,
        t.形成特征类型,t.形成场景,t.生命周期};
    std::set<std::uint64_t> ids;
    auto unique=[&](稳定编码 id){if(!有效(id)||!ids.insert(id.值).second)throw 失败{S::引用冲突};};
    unique(x.格式锚点); unique(x.概念族锚点); for(auto id:types)unique(id);
    for(auto id:{x.格式锚点,x.概念族锚点}){
        const auto n=节点(id,g);
        if(n.种类!=节点种类::普通||n.属性类型表示||!属性(id,g).empty())throw 失败{S::引用冲突};
    }
    using R=L1所有者范围值表示种类;
    for(std::size_t i=0;i<types.size();++i){
        std::optional<R> rep;
        switch(i+1){case 3:case 5:case 9:case 12:case 13:case 20:rep=R::I64;break;
        case 10:rep=R::I64组;break;case 11:rep=R::U64组;break;default:break;}
        const auto n=节点(types[i],g);
        if(n.种类!=(rep?节点种类::属性类型:节点种类::普通)||n.属性类型表示!=rep||!属性(types[i],g).empty())
            throw 失败{S::引用冲突};
    }
    const auto registrations=关系(x.格式锚点,t.类型登记,false,g,20);
    if(registrations.size()!=20)throw 失败{S::引用冲突};
    for(std::size_t i=0;i<20;++i)
        if(registrations[i].源节点!=x.格式锚点||registrations[i].目标节点!=types[i]||
           registrations[i].角色或顺序!=static_cast<std::int64_t>(i+1))throw 失败{S::引用冲突};
}

相关概念参与状态 概念树类数据服务::映射相关状态(S s) const noexcept {
    using R=相关概念参与状态;
    switch(s){
    case S::精确重复:return R::精确重复;case S::已读取:return R::已读取;
    case S::入口拒绝:return R::入口拒绝;case S::未找到:return R::概念未找到;
    case S::形成环:return R::上位成环;
    case S::事实代次漂移:return R::事实代次漂移;case S::幂等冲突:return R::幂等冲突;
    case S::数量预算不足:return R::数量预算不足;
    case S::资源失败:return R::资源失败;case S::已可能发布:return R::已可能发布;
    case S::旧格式不支持:return R::旧格式不支持;case S::引用冲突:return R::挂靠无效;
    case S::内部不一致:return R::内部不一致;
    case S::已创建:case S::已添加支持:case S::已删除支持:case S::已添加上位:
    case S::已替换上位:case S::已删除上位:case S::已迁移生命周期:case S::已删除概念:
    case S::已释放引用:case S::无须变更:case S::根不可退出:case S::已创建共享定义:
    case S::已绑定共享名称:case S::已删除共享名称:case S::已记录共享用途:case S::不支持:
    case S::差异不可表示:case S::已登记类型观察:case S::已登记特征概念命中:
    case S::已变更特征概念组织:case S::规则缺失:case S::类型不相容:case S::前次写入待收敛:
        return R::内部不一致;
    }
    return R::内部不一致;
}

void 概念树类数据服务::验证相关世界(const 相关概念世界挂靠 &binding,std::uint64_t g,
                                         const 相关概念读取预算 &budget) const {
    if(binding.世界事实.valueless_by_exception()||
        !有效(世界编码(binding.世界事实)))throw 相关失败{相关概念参与状态::挂靠无效};
    try{
        if(const auto *scene=std::get_if<概念树场景引用>(&binding.世界事实)){
            if(!scenes_)throw 相关失败{相关概念参与状态::旧格式不支持};
            const 场景当前身份请求 request{2,g,scene->值};
            const auto result=scenes_->确认当前场景角色(request);
            if(!result.成功(request)){
                if(result.状态==场景角色数据状态::资源失败)throw 相关失败{相关概念参与状态::资源失败};
                if(result.状态==场景角色数据状态::事实代次漂移)throw 相关失败{相关概念参与状态::事实代次漂移};
                throw 相关失败{相关概念参与状态::挂靠无效};
            }
            return;
        }
        验证世界(binding.世界事实,g,budget);
    }catch(const 相关失败&){throw;}catch(const 失败 &e){
        const auto mapped=映射相关状态(e.状态);
        if(mapped==相关概念参与状态::概念未找到||
           mapped==相关概念参与状态::入口拒绝)throw 相关失败{相关概念参与状态::挂靠无效};
        throw 相关失败{mapped};
    }
}

概念树类数据服务::相关概念展开 概念树类数据服务::展开相关概念(
    概念树概念身份 id, std::uint64_t g,
    const 相关概念读取预算 &budget) const {
    if (!related_layout_) throw 相关失败{相关概念参与状态::旧格式不支持};
    if (!有效(id.值) || !g) throw 相关失败{相关概念参与状态::入口拒绝};
    try {
        const auto &t = related_layout_->类型;
        相关概念展开 out;
        out.概念 = id;
        const auto conceptNodeFact = 节点(id.值, g);
        if (conceptNodeFact.种类 != 节点种类::普通 || conceptNodeFact.属性类型表示)
            throw 相关失败{相关概念参与状态::签名冲突};
        out.生命周期 = 投影生命(conceptNodeFact);

        const auto members = 关系(id.值, t.概念族成员, true, g, 1);
        if (members.size() != 1 || members.front().源节点 != related_layout_->概念族锚点 ||
            members.front().角色或顺序 != 1)
            throw 相关失败{相关概念参与状态::签名冲突};
        const auto conceptValues = 属性(id.值, g);
        if (conceptValues.size() != 2) throw 相关失败{相关概念参与状态::签名冲突};
        const auto category = std::get<std::int64_t>(唯一属性(conceptValues, t.概念类别).材料);
        const auto lifecycle = std::get<std::int64_t>(唯一属性(conceptValues, t.生命周期).材料);
        if ((category != 1 && category != 2) || lifecycle != 1)
            throw 相关失败{相关概念参与状态::签名冲突};
        out.类别 = static_cast<相关概念类别>(category);

        const auto definitionMembers = 关系(id.值, t.定义成员, false, g, 1);
        if (definitionMembers.size() != 1 || definitionMembers.front().角色或顺序 != 1)
            throw 相关失败{相关概念参与状态::签名冲突};
        const auto definitionNode = 节点(definitionMembers.front().目标节点, g);
        if (definitionNode.种类 != 节点种类::普通 || definitionNode.属性类型表示 ||
            definitionNode.创建事实代次 != conceptNodeFact.创建事实代次)
            throw 相关失败{相关概念参与状态::签名冲突};
        const auto definitionValues = 属性(definitionNode.编码, g);
        const auto kind = std::get<std::int64_t>(唯一属性(definitionValues, t.定义种类).材料);
        if (kind != category) throw 相关失败{相关概念参与状态::类别冲突};
        if (kind == 1) {
            概念树存在定义 definition;
            const auto templates = 关系(definitionNode.编码, t.定义模板, false, g,
                                         budget.最大关系数);
            if (definitionValues.size() != 1 || templates.empty())
                throw 相关失败{相关概念参与状态::签名冲突};
            for (std::size_t i = 0; i < templates.size(); ++i) {
                if (templates[i].角色或顺序 != static_cast<std::int64_t>(i + 1))
                    throw 相关失败{相关概念参与状态::签名冲突};
                definition.特征模板组.emplace_back(templates[i].目标节点);
            }
            out.定义 = 规范化定义(definition);
        } else {
            概念树特征定义 definition;
            const auto hosts = 关系(definitionNode.编码, t.定义宿主, false, g, 1);
            const auto types = 关系(definitionNode.编码, t.定义特征类型, false, g, 1);
            if (hosts.size() != 1 || types.size() != 1 || hosts.front().角色或顺序 != 1 ||
                types.front().角色或顺序 != 1)
                throw 相关失败{相关概念参与状态::签名冲突};
            definition.形成宿主 = 概念树存在引用{hosts.front().目标节点};
            definition.特征类型 = 概念树特征类型引用{types.front().目标节点};
            if (definitionValues.size() == 2) {
                const auto &value = definitionValues[0].属性类型节点 == t.定义种类
                    ? definitionValues[1] : definitionValues[0];
                if (value.属性类型节点 == t.精确I64)
                    definition.值域 = 概念树精确值{std::get<std::int64_t>(value.材料)};
                else if (value.属性类型节点 == t.精确I64组)
                    definition.值域 = 概念树精确值{std::get<std::vector<std::int64_t>>(value.材料)};
                else if (value.属性类型节点 == t.精确U64组)
                    definition.值域 = 概念树精确值{std::get<std::vector<std::uint64_t>>(value.材料)};
                else throw 相关失败{相关概念参与状态::签名冲突};
            } else if (definitionValues.size() == 3) {
                definition.值域 = 概念树I64区间{
                    std::get<std::int64_t>(唯一属性(definitionValues, t.区间下界).材料),
                    std::get<std::int64_t>(唯一属性(definitionValues, t.区间上界).材料)};
            } else throw 相关失败{相关概念参与状态::签名冲突};
            out.定义 = 规范化定义(definition);
        }

        std::set<std::uint64_t> parentIds;
        for (const auto &edge : 关系(id.值, t.直接上位, true, g, budget.最大关系数)) {
            if (edge.角色或顺序 != 1 || edge.源节点 == id.值 ||
                !parentIds.insert(edge.源节点.值).second)
                throw 相关失败{相关概念参与状态::上位成环};
            const auto parentValues = 属性(edge.源节点, g);
            const auto parentCategory = std::get<std::int64_t>(
                唯一属性(parentValues, t.概念类别).材料);
            if (parentCategory != category) throw 相关失败{相关概念参与状态::类别冲突};
            out.直接上位.push_back({edge.编码, 概念树概念身份{edge.源节点}, id,
                                    投影生命(edge)});
        }
        std::sort(out.直接上位.begin(), out.直接上位.end(), [](const auto &a, const auto &b) {
            return a.上位.值.值 < b.上位.值.值;
        });

        for (const auto &member : 关系(id.值, t.形成成员, false, g, budget.最大来源数)) {
            if (member.角色或顺序 != 1) throw 相关失败{相关概念参与状态::内部不一致};
            const auto record = 节点(member.目标节点, g);
            const auto recordValues = 属性(record.编码, g);
            if (record.种类 != 节点种类::普通 || record.属性类型表示 || !recordValues.empty())
                throw 相关失败{相关概念参与状态::内部不一致};
            std::optional<E> endpoint;
            std::size_t branch = 0;
            for (const auto type : {t.形成存在, t.形成特征, t.形成特征类型, t.形成场景}) {
                const auto found = 关系(record.编码, type, false, g, 1);
                if (!found.empty()) { endpoint = found.front(); ++branch; }
            }
            if (branch != 1 || !endpoint || endpoint->角色或顺序 != 1)
                throw 相关失败{相关概念参与状态::内部不一致};
            概念树形成世界引用 world;
            if (endpoint->关系类型节点 == t.形成存在) world = 概念树存在引用{endpoint->目标节点};
            else if (endpoint->关系类型节点 == t.形成特征) world = 概念树特征引用{endpoint->目标节点};
            else if (endpoint->关系类型节点 == t.形成特征类型) world = 概念树特征类型引用{endpoint->目标节点};
            else world = 概念树场景引用{endpoint->目标节点};
            out.挂接.push_back({endpoint->编码, record.编码, id, world, 投影生命(*endpoint)});
        }
        return out;
    } catch (const 相关失败 &) { throw; }
      catch (const 失败 &e) { throw 相关失败{映射相关状态(e.状态)}; }
}

相关概念完整事实 概念树类数据服务::读取相关概念内部(
    const 相关概念参与请求 &request, 概念树概念身份 id,
    std::uint64_t g) const {
    const auto expanded = 展开相关概念(id, g, request.预算);
    const auto expectedCategory = std::visit([](const auto &v) { return v.类别; }, request.方案);
    const auto expectedDefinition = 规范化定义(std::visit([](const auto &v) -> 概念树定义 {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, 相关概念精确复用方案>) return v.预期定义;
        else return v.定义;
    }, request.方案));
    if (expanded.类别 != expectedCategory) throw 相关失败{相关概念参与状态::类别冲突};
    if (expanded.定义 != expectedDefinition) throw 相关失败{相关概念参与状态::签名冲突};
    const auto target = 世界编码(request.挂靠.世界事实);
    const 概念树形成引用事实 *attachment = nullptr;
    for (const auto &value : expanded.挂接) {
        if (value.世界引用.index() == request.挂靠.世界事实.index() &&
            世界编码(value.世界引用) == target) {
            if (attachment) throw 相关失败{相关概念参与状态::内部不一致};
            attachment = &value;
        }
    }
    if (!attachment) throw 相关失败{相关概念参与状态::概念未找到};
    return {expanded.概念, expanded.类别, expanded.定义, expanded.直接上位,
            *attachment, expanded.生命周期};
}

L1有限N分区原子参与者写集_v3 概念树类数据服务::形成相关概念写集(
    const 相关概念参与请求 &request, std::uint64_t g,
    L1有限N分区原子参与者身份_v3 participant,
    const std::optional<相关概念完整事实> &existing) const {
    if (!related_layout_) throw 相关失败{相关概念参与状态::旧格式不支持};
    L1有限N分区原子参与者写集_v3 out;
    out.参与者 = participant;
    out.所有者 = port_.所有者身份();
    out.写集.合同版本 = L1所有者范围CRUD合同版本;
    out.写集.期望事实代次 = g;
    out.写集.写入幂等身份 = request.幂等身份;
    std::uint32_t nodeKey = 0, edgeKey = 0xFFFF, valueKey = 0x1FFFF;
    const auto newNode = [&]() {
        Key key{++nodeKey};
        out.写集.节点.push_back({key, 节点种类::普通, std::nullopt});
        return key;
    };
    const auto newEdge = [&](L1有限N分区原子事实引用值_v3 source,
                             L1有限N分区原子事实引用值_v3 target,
                             稳定编码 type, std::int64_t order = 1) {
        Key key{++edgeKey};
        out.写集.关系.push_back({key, std::move(source), std::move(target), type, order});
        return key;
    };
    const auto newValue = [&](L1有限N分区原子事实引用值_v3 node,
                              稳定编码 type, L1所有者范围原始值材料 material) {
        Key key{++valueKey};
        out.写集.值.push_back({key, node, type, std::move(material), node});
        out.写集.属性槽变更.push_back({node, type, key});
        return key;
    };
    const auto &t = related_layout_->类型;
    L1有限N分区原子事实引用值_v3 conceptRef;
    if (existing) conceptRef = existing->概念.值;
    else {
        const auto conceptNode = newNode();
        const auto definitionNode = newNode();
        conceptRef = conceptNode;
        const auto category = std::visit([](const auto &v) { return v.类别; }, request.方案);
        const auto definition = std::visit([](const auto &v) -> 概念树定义 {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, 相关概念精确复用方案>) return v.预期定义;
            else return v.定义;
        }, request.方案);
        newEdge(related_layout_->概念族锚点, conceptNode, t.概念族成员);
        newValue(conceptNode, t.概念类别, static_cast<std::int64_t>(category));
        newValue(conceptNode, t.生命周期, std::int64_t{1});
        newEdge(conceptNode, definitionNode, t.定义成员);
        newValue(definitionNode, t.定义种类, static_cast<std::int64_t>(category));
        if (const auto *feature = std::get_if<概念树特征定义>(&definition)) {
            newEdge(definitionNode, feature->形成宿主.值, t.定义宿主);
            newEdge(definitionNode, feature->特征类型.值, t.定义特征类型);
            if (const auto *exact = std::get_if<概念树精确值>(&feature->值域)) {
                const auto type = exact->index() == 0 ? t.精确I64
                    : exact->index() == 1 ? t.精确I64组 : t.精确U64组;
                std::visit([&](const auto &value) { newValue(definitionNode, type, value); }, *exact);
            } else {
                const auto range = std::get<概念树I64区间>(feature->值域);
                newValue(definitionNode, t.区间下界, range.下界);
                newValue(definitionNode, t.区间上界, range.上界);
            }
        } else {
            const auto &definitionValue = std::get<概念树存在定义>(definition);
            for (std::size_t i = 0; i < definitionValue.特征模板组.size(); ++i)
                newEdge(definitionNode, definitionValue.特征模板组[i].值, t.定义模板,
                        static_cast<std::int64_t>(i + 1));
        }
        if (const auto *create = std::get_if<相关概念创建方案>(&request.方案))
            for (const auto parent : create->直接上位)
                newEdge(parent.值, conceptNode, t.直接上位);
    }
    const auto formationNode = newNode();
    newEdge(conceptRef, formationNode, t.形成成员);
    const auto endpointType = std::visit([&](const auto &world) {
        using T = std::decay_t<decltype(world)>;
        if constexpr (std::is_same_v<T, 概念树存在引用>) return t.形成存在;
        else if constexpr (std::is_same_v<T, 概念树特征引用>) return t.形成特征;
        else if constexpr (std::is_same_v<T, 概念树特征类型引用>) return t.形成特征类型;
        else return t.形成场景;
    }, request.挂靠.世界事实);
    newEdge(formationNode, 世界编码(request.挂靠.世界事实), endpointType);
    if (out.写集.关系.size() > request.预算.最大关系数 ||
        out.写集.值.size() > request.预算.最大特征属性数)
        throw 相关失败{相关概念参与状态::数量预算不足};
    return out;
}


相关概念参与片段 概念树类数据服务::准备相关概念片段(
    const 相关概念参与请求 &request, std::uint64_t g,
    L1有限N分区原子参与者身份_v3 participant) const noexcept {
    相关概念参与片段 out;
    out.Gread = g;
    try {
        if (!related_layout_) throw 相关失败{相关概念参与状态::旧格式不支持};
        if (request.版本 != 1 || !g || request.G0 != g || !有效(request.幂等身份) ||
            participant.值 < 2 ||
            request.方案.valueless_by_exception() || request.挂靠.世界事实.valueless_by_exception())
            throw 相关失败{相关概念参与状态::入口拒绝};
        try { 预算有效(request.预算); }
        catch (const 失败 &e) { throw 相关失败{映射相关状态(e.状态)}; }
        守卫代次(g);
        校验相关概念布局(g);
        验证相关世界(request.挂靠, g, request.预算);
        const auto category = std::visit([](const auto &v) { return v.类别; }, request.方案);
        auto definition = 规范化定义(std::visit([](const auto &v) -> 概念树定义 {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, 相关概念精确复用方案>) return v.预期定义;
            else return v.定义;
        }, request.方案));
        if ((category != 相关概念类别::存在 && category != 相关概念类别::特征) ||
            (category == 相关概念类别::存在) != std::holds_alternative<概念树存在定义>(definition))
            throw 相关失败{相关概念参与状态::类别冲突};
        if (const auto *feature = std::get_if<概念树特征定义>(&definition)) {
            验证相关世界({feature->形成宿主}, g, request.预算);
            验证相关世界({feature->特征类型}, g, request.预算);
        } else {
            for (const auto templateId : std::get<概念树存在定义>(definition).特征模板组) {
                const auto feature = 展开相关概念(templateId, g, request.预算);
                if (feature.类别 != 相关概念类别::特征)
                    throw 相关失败{相关概念参与状态::类别冲突};
            }
        }

        std::optional<相关概念完整事实> existing;
        if (const auto *reuse = std::get_if<相关概念精确复用方案>(&request.方案)) {
            const auto found = 展开相关概念(reuse->概念, g, request.预算);
            if (found.类别 != category) throw 相关失败{相关概念参与状态::类别冲突};
            if (found.定义 != definition) throw 相关失败{相关概念参与状态::签名冲突};
            existing = {found.概念, found.类别, found.定义, found.直接上位,
                        {}, found.生命周期};
        } else {
            const auto family = 关系(related_layout_->概念族锚点,
                                     related_layout_->类型.概念族成员, false,
                                     g, request.预算.最大概念数);
            for (const auto &member : family) {
                const auto found = 展开相关概念(概念树概念身份{member.目标节点}, g, request.预算);
                if (found.定义 == definition) {
                    if (existing) throw 相关失败{相关概念参与状态::内部不一致};
                    if (found.类别 != category) throw 相关失败{相关概念参与状态::类别冲突};
                    existing = {found.概念, found.类别, found.定义, found.直接上位,
                                {}, found.生命周期};
                }
            }
        }

        if (const auto *create = std::get_if<相关概念创建方案>(&request.方案)) {
            auto parents = create->直接上位;
            std::sort(parents.begin(), parents.end(), 身份小于);
            if (std::adjacent_find(parents.begin(), parents.end()) != parents.end())
                throw 相关失败{相关概念参与状态::上位成环};
            for (const auto parent : parents) {
                const auto value = 展开相关概念(parent, g, request.预算);
                if (value.类别 != category) throw 相关失败{相关概念参与状态::类别冲突};
                if (existing && parent == existing->概念)
                    throw 相关失败{相关概念参与状态::上位成环};
            }
            if (existing) {
                std::vector<概念树概念身份> actual;
                for (const auto &edge : existing->直接上位) actual.push_back(edge.上位);
                std::sort(actual.begin(), actual.end(), 身份小于);
                if (actual != parents) throw 相关失败{相关概念参与状态::签名冲突};
            }
        }

        if (existing) {
            const auto expanded = 展开相关概念(existing->概念, g, request.预算);
            const auto target = 世界编码(request.挂靠.世界事实);
            for (const auto &attachment : expanded.挂接) {
                if (attachment.世界引用.index() != request.挂靠.世界事实.index() ||
                    世界编码(attachment.世界引用) != target) continue;
                out.状态 = 相关概念参与状态::精确重复;
                守卫代次(g);
                return out;
            }
        }
        out.写集 = 形成相关概念写集(request, g, participant, existing);
        if (!out.写集 || out.写集->写集.节点.empty())
            throw 相关失败{相关概念参与状态::内部不一致};
        out.状态 = 相关概念参与状态::已准备;
        守卫代次(g);
        return out;
    } catch (const 相关失败 &e) { out.状态 = e.状态; }
      catch (const 失败 &e) { out.状态 = 映射相关状态(e.状态); }
      catch (const std::bad_alloc &) { out.状态 = 相关概念参与状态::资源失败; }
      catch (const std::length_error &) { out.状态 = 相关概念参与状态::资源失败; }
      catch (...) { out.状态 = 相关概念参与状态::内部不一致; }
    out.写集.reset();
    return out;
}


相关概念组合提交结果 概念树类数据服务::提交相关概念组合事务(
    const 相关概念组合提交请求 &request,
    std::span<L1所有者范围写端口 *const> externalPorts) noexcept {
    相关概念组合提交结果 out;
    out.Gread = request.Gread;
    out.事务结果.组合写入幂等身份 = request.组合幂等身份;
    try {
        std::scoped_lock lock(mutex_);
        const auto count = request.前序参与者写集组.size();
        if (request.版本 != 1 || !request.Gread || !有效(request.组合幂等身份) ||
            count < 1 || count > 254 || externalPorts.size() != count ||
            request.概念请求.G0 != request.Gread || !有效(request.概念请求.幂等身份))
            throw 相关失败{相关概念参与状态::入口拒绝};
        std::set<std::uint64_t> owners;
        std::set<std::uint64_t> keys;
        keys.insert(request.组合幂等身份.值);
        for (std::size_t i = 0; i < count; ++i) {
            const auto &part = request.前序参与者写集组[i];
            auto *port = externalPorts[i];
            if (part.参与者.值 != i + 1 || !有效(part.所有者) ||
                part.写集.合同版本 != L1所有者范围CRUD合同版本 ||
                part.写集.期望事实代次 != request.Gread ||
                !有效(part.写集.写入幂等身份) || !port || !port->有效() ||
                !port->绑定于(l1_) || port->所有者身份() != part.所有者 ||
                !owners.insert(part.所有者.编码.值).second ||
                !keys.insert(part.写集.写入幂等身份.值).second)
                throw 相关失败{相关概念参与状态::入口拒绝};
        }
        if (!owners.insert(port_.所有者身份().编码.值).second ||
            !keys.insert(request.概念请求.幂等身份.值).second)
            throw 相关失败{相关概念参与状态::入口拒绝};

        const L1有限N分区原子参与者身份_v3 conceptParticipant{
            static_cast<std::uint8_t>(count + 1)};
        L1有限N分区原子参与者写集_v3 conceptPart;
        const auto first = port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, request.概念请求.幂等身份});
        if (first.状态 == L1所有者范围读取状态::未找到) {
            const auto prepared = 准备相关概念片段(
                request.概念请求, request.Gread, conceptParticipant);
            out.概念状态 = prepared.状态;
            if (prepared.状态 != 相关概念参与状态::已准备 || !prepared.写集)
                return out;
            conceptPart = *prepared.写集;
            if (conceptPart.参与者 != conceptParticipant ||
                conceptPart.所有者 != port_.所有者身份() ||
                conceptPart.写集.期望事实代次 != request.Gread ||
                conceptPart.写集.写入幂等身份 != request.概念请求.幂等身份)
                throw 相关失败{相关概念参与状态::内部不一致};
        } else if (first.状态 == L1所有者范围读取状态::成功) {
            if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
                first.所有者 != port_.所有者身份() ||
                first.写入幂等身份 != request.概念请求.幂等身份 ||
                !first.首次规范化写集 || !first.首次写入结果 ||
                first.首次写入结果->所有者 != port_.所有者身份() ||
                first.首次写入结果->写入幂等身份 != request.概念请求.幂等身份 ||
                !first.首次写入结果->事实代次)
                throw 相关失败{相关概念参与状态::内部不一致};
            const auto semantic = 读取相关概念结果(
                request.概念请求, first.读取事实代次);
            if (semantic.状态 != 相关概念参与状态::已读取 || !semantic.概念)
                throw 相关失败{相关概念参与状态::内部不一致};
            conceptPart.参与者 = conceptParticipant;
            conceptPart.所有者 = port_.所有者身份();
            const auto &source = *first.首次规范化写集;
            conceptPart.写集.合同版本 = source.合同版本;
            conceptPart.写集.期望事实代次 = source.期望事实代次;
            conceptPart.写集.写入幂等身份 = source.写入幂等身份;
            for (const auto &node : source.节点)
                conceptPart.写集.节点.push_back(
                    {node.本地键, node.种类, node.属性类型表示});
            conceptPart.写集.退出事实 = source.退出事实;
            const auto convertRef = [](const L1所有者范围事实引用 &ref)
                -> L1有限N分区原子事实引用值_v3 {
                return std::visit([](const auto &value) -> L1有限N分区原子事实引用值_v3 {
                    return value;
                }, ref);
            };
            for (const auto &edge : source.关系)
                conceptPart.写集.关系.push_back({edge.本地键, convertRef(edge.源节点),
                    convertRef(edge.目标节点), convertRef(edge.关系类型节点), edge.角色或顺序});
            for (const auto &value : source.值)
                conceptPart.写集.值.push_back({value.本地键, convertRef(value.所属节点),
                    convertRef(value.属性类型节点), value.材料, convertRef(value.来源节点)});
            for (const auto &slot : source.属性槽变更)
                conceptPart.写集.属性槽变更.push_back({convertRef(slot.所属节点),
                    convertRef(slot.属性类型节点), slot.新当前值});
            const auto restoreRef = [](const L1有限N分区原子事实引用值_v3 &ref)
                -> std::optional<L1所有者范围事实引用> {
                if (const auto *stable = std::get_if<稳定编码>(&ref)) return *stable;
                if (const auto *local = std::get_if<Key>(&ref)) return *local;
                return std::nullopt;
            };
            L1所有者范围写集请求 restored;
            restored.合同版本 = conceptPart.写集.合同版本;
            restored.期望事实代次 = conceptPart.写集.期望事实代次;
            restored.写入幂等身份 = conceptPart.写集.写入幂等身份;
            for (const auto &node : conceptPart.写集.节点)
                restored.节点.push_back(
                    {node.本地键, node.种类, node.属性类型表示});
            restored.退出事实 = conceptPart.写集.退出事实;
            for (const auto &edge : conceptPart.写集.关系) {
                const auto a = restoreRef(edge.源节点), b = restoreRef(edge.目标节点),
                           c = restoreRef(edge.关系类型节点);
                if (!a || !b || !c) throw 相关失败{相关概念参与状态::内部不一致};
                restored.关系.push_back({edge.本地键, *a, *b, *c, edge.角色或顺序});
            }
            for (const auto &value : conceptPart.写集.值) {
                const auto a = restoreRef(value.所属节点), b = restoreRef(value.属性类型节点),
                           c = restoreRef(value.来源节点);
                if (!a || !b || !c) throw 相关失败{相关概念参与状态::内部不一致};
                restored.值.push_back({value.本地键, *a, *b, value.材料, *c});
            }
            for (const auto &slot : conceptPart.写集.属性槽变更) {
                const auto a = restoreRef(slot.所属节点), b = restoreRef(slot.属性类型节点);
                if (!a || !b) throw 相关失败{相关概念参与状态::内部不一致};
                restored.属性槽变更.push_back({*a, *b, slot.新当前值});
            }
            if (restored != source) throw 相关失败{相关概念参与状态::内部不一致};
            out.概念状态 = 相关概念参与状态::已准备;
        } else {
            throw 相关失败{映射相关状态(映射(first.状态))};
        }

        L1有限N分区原子事务请求_v3 transaction;
        transaction.共同期望事实代次 = request.Gread;
        transaction.组合写入幂等身份 = request.组合幂等身份;
        transaction.参与者写集组 = request.前序参与者写集组;
        transaction.参与者写集组.push_back(std::move(conceptPart));
        std::vector<const L1所有者范围写端口 *> remaining;
        for (std::size_t i = 1; i < externalPorts.size(); ++i) remaining.push_back(externalPorts[i]);
        remaining.push_back(&port_);
        out.已进入L1 = true;
        out.事务结果 = externalPorts.front()->提交有限N分区原子事务_v3(transaction, remaining);
        if (out.事务结果.状态 == L1有限N分区原子事务状态_v3::精确重复)
            out.概念状态 = 相关概念参与状态::精确重复;
        return out;
    } catch (const 相关失败 &e) { out.概念状态 = e.状态; }
      catch (const 失败 &e) { out.概念状态 = 映射相关状态(e.状态); }
      catch (const std::bad_alloc &) { out.概念状态 = 相关概念参与状态::资源失败; }
      catch (const std::length_error &) { out.概念状态 = 相关概念参与状态::资源失败; }
      catch (...) { out.概念状态 = 相关概念参与状态::内部不一致; }
    out.已进入L1 = false;
    return out;
}


相关概念参与读回 概念树类数据服务::读取相关概念结果(
    const 相关概念参与请求 &request, std::uint64_t g) const noexcept {
    相关概念参与读回 out;
    out.Gread = g;
    try {
        if (!related_layout_) throw 相关失败{相关概念参与状态::旧格式不支持};
        if (request.版本 != 1 || !g || request.G0 > g ||
            !有效(request.幂等身份) || request.方案.valueless_by_exception())
            throw 相关失败{相关概念参与状态::入口拒绝};
        try { 预算有效(request.预算); }
        catch (const 失败 &e) { throw 相关失败{映射相关状态(e.状态)}; }
        守卫代次(g);
        概念树概念身份 id;
        if (const auto *reuse = std::get_if<相关概念精确复用方案>(&request.方案)) id = reuse->概念;
        else {
            const auto expected = 规范化定义(std::get<相关概念创建方案>(request.方案).定义);
            const auto family = 关系(related_layout_->概念族锚点,
                                     related_layout_->类型.概念族成员, false,
                                     g, request.预算.最大概念数);
            for (const auto &member : family) {
                const auto found = 展开相关概念(概念树概念身份{member.目标节点}, g, request.预算);
                if (found.定义 == expected) {
                    if (有效(id.值)) throw 相关失败{相关概念参与状态::内部不一致};
                    id = found.概念;
                }
            }
            if (!有效(id.值)) throw 相关失败{相关概念参与状态::概念未找到};
        }
        out.概念 = 读取相关概念内部(request, id, g);
        out.状态 = 相关概念参与状态::已读取;
        守卫代次(g);
        return out;
    } catch (const 相关失败 &e) { out.状态 = e.状态; }
      catch (const 失败 &e) { out.状态 = 映射相关状态(e.状态); }
      catch (const std::bad_alloc &) { out.状态 = 相关概念参与状态::资源失败; }
      catch (const std::length_error &) { out.状态 = 相关概念参与状态::资源失败; }
      catch (...) { out.状态 = 相关概念参与状态::内部不一致; }
    out.概念.reset();
    return out;
}


纯概念结构登记结果_v2 概念树类数据服务::登记纯概念结构_v2(
    const L1事实基座服务 &l1, L1所有者范围写端口 &port,
    const 纯概念结构登记请求_v2 &r) noexcept {
    纯概念结构登记结果_v2 out;
    out.原请求 = r;
    bool entered = false;
    try {
        if (r.版本 != 2 || !r.G0 || r.G0 == UINT64_MAX || !有效(r.幂等身份) ||
            !r.最大空域核验事实数 ||
            !port.有效() || !port.绑定于(l1))
            return out;
        L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
        for (std::uint32_t i = 1; i <= 15; ++i) {
            std::optional<L1所有者范围值表示种类> representation;
            if (i == 5 || i == 6 || i == 8 || i == 12 || i == 14)
                representation = L1所有者范围值表示种类::I64;
            else if (i == 11)
                representation = L1所有者范围值表示种类::U64组;
            ws.节点.push_back({{i}, representation ? 节点种类::属性类型
                                                    : 节点种类::普通,
                               representation});
        }
        for (std::uint32_t i = 0; i < 13; ++i)
            ws.关系.push_back({{0x10001U + i}, Key{1}, Key{3U + i}, Key{3},
                               static_cast<std::int64_t>(i + 1)});
        ws.值.push_back({{0x20001}, Key{1}, Key{5}, std::int64_t{2}, Key{1}});
        ws.属性槽变更.push_back({Key{1}, Key{5}, Key{0x20001}});
        const auto first = port.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, r.幂等身份});
        bool replay = false;
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
            first.所有者 != port.所有者身份() || first.写入幂等身份 != r.幂等身份) {
            out.状态 = 纯概念状态::内部不一致; out.Gread = first.读取事实代次; return out;
        }
        if (first.状态 == L1所有者范围读取状态::成功) {
            replay = true;
            if (!first.首次规范化写集 || *first.首次规范化写集 != ws) {
                if (first.首次规范化写集) {
                    auto 本次v2写集按原G0 = ws;
                    本次v2写集按原G0.期望事实代次 = first.首次规范化写集->期望事实代次;
                    out.状态 = 本次v2写集按原G0 == *first.首次规范化写集
                                   ? 纯概念状态::幂等冲突
                                   : 纯概念状态::旧格式不支持;
                } else {
                    out.状态 = 纯概念状态::旧格式不支持;
                }
                out.Gread = first.读取事实代次;
                return out;
            }
            if (!first.首次写入结果 ||
                first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
                first.首次写入结果->事实代次 != r.G0 + 1 ||
                !first.首次写入结果->是否形成内存权威发布) {
                out.状态 = 纯概念状态::已可能发布;
                out.发布 = 纯概念发布状态::可能发布;
                out.Gread = first.读取事实代次; return out;
            }
        } else if (first.状态 == L1所有者范围读取状态::未找到) {
            if (first.读取事实代次 != r.G0 || first.首次规范化写集 || first.首次写入结果) {
                out.状态 = first.读取事实代次 != r.G0 ? 纯概念状态::事实代次漂移
                                                        : 纯概念状态::内部不一致;
                out.Gread = first.读取事实代次; return out;
            }
            const L1所有者范围空域完整读取请求_v2 emptyRequest{
                L1所有者范围空域完整读取合同版本,port.所有者身份(),r.G0};
            const auto empty=l1.读取所有者范围完整空域(emptyRequest);
            out.Gread=empty.读取事实代次;
            const bool emptyOk=empty.状态==L1所有者范围空域完整读取状态_v2::成功
                &&empty.合同版本==emptyRequest.合同版本
                &&empty.所有者==emptyRequest.所有者
                &&empty.期望事实代次==emptyRequest.期望事实代次
                &&empty.读取事实代次==emptyRequest.期望事实代次
                &&empty.空域&&*empty.空域;
            if(!emptyOk) {
                switch(empty.状态) {
                case L1所有者范围空域完整读取状态_v2::入口拒绝:
                    out.状态=纯概念状态::入口拒绝;break;
                case L1所有者范围空域完整读取状态_v2::事实代次漂移:
                    out.状态=纯概念状态::事实代次漂移;break;
                case L1所有者范围空域完整读取状态_v2::资源失败:
                    out.状态=纯概念状态::资源失败;break;
                case L1所有者范围空域完整读取状态_v2::范围不支持:
                    out.状态=纯概念状态::旧格式不支持;break;
                default:out.状态=纯概念状态::内部不一致;break;
                }
                return out;
            }
            if(empty.状态!=L1所有者范围空域完整读取状态_v2::成功 ||
               empty.合同版本!=L1所有者范围空域完整读取合同版本 ||
               empty.所有者!=emptyRequest.所有者 ||
               empty.期望事实代次!=emptyRequest.期望事实代次 ||
               empty.读取事实代次!=emptyRequest.期望事实代次 ||
               empty.空域!=true) {
                out.状态=纯概念状态::旧格式不支持;
                return out;
            }
        } else {
            out.状态 = first.状态 == L1所有者范围读取状态::资源失败
                           ? 纯概念状态::资源失败 : 纯概念状态::内部不一致;
            out.Gread = first.读取事实代次; return out;
        }
        entered = true;
        const auto saved = port.提交所有者范围中性写集(ws);
        out.Gread = saved.事实代次;
        if (saved.状态 != (replay ? L1所有者范围写入状态::精确重复
                                  : L1所有者范围写入状态::成功) ||
            saved.合同版本 != L1所有者范围CRUD合同版本 ||
            saved.所有者 != port.所有者身份() || saved.写入幂等身份 != r.幂等身份 ||
            saved.事实代次 != r.G0 + 1 || saved.新编码映射.size() != 29) {
            out.状态 = saved.状态 == L1所有者范围写入状态::事实代次漂移
                           ? 纯概念状态::事实代次漂移
                           : (saved.状态 == L1所有者范围写入状态::幂等冲突
                                  ? 纯概念状态::幂等冲突
                                  : 纯概念状态::已可能发布);
            out.发布 = saved.是否形成内存权威发布 || replay
                           ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
            return out;
        }
        std::array<稳定编码, 15> ids{};
        for (const auto &[key, id] : saved.新编码映射)
            if (key.值 >= 1 && key.值 <= 15) ids[key.值 - 1] = id;
        if (std::ranges::any_of(ids, [](auto x) { return !有效(x); })) {
            out.状态 = 纯概念状态::已可能发布;
            out.发布 = 纯概念发布状态::可能发布; return out;
        }
        out.交付 = 纯概念结构交付_v2{
            2, ids[0], ids[1],
            {ids[2],ids[3],ids[4],ids[5],ids[6],ids[7],ids[8],ids[9],ids[10],ids[11],ids[12],ids[13],ids[14]}};
        out.首次发布H = saved.事实代次;
        const auto tail = l1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (tail.状态 != L1中性读取状态::成功 || !tail.事实代次 ||
            tail.事实代次 < saved.事实代次) {
            out.状态 = 纯概念状态::已可能发布;
            out.发布 = 纯概念发布状态::可能发布;
            out.Gread = tail.事实代次; return out;
        }
        out.Gread = tail.事实代次;
        auto mapped=[&](std::uint64_t key) {
            std::optional<稳定编码> found;
            for(const auto &[k,id]:saved.新编码映射) if(k.值==key) {
                if(found||!有效(id)) throw 纯失败{纯概念状态::内部不一致};
                found=id;
            }
            if(!found) throw 纯失败{纯概念状态::内部不一致};
            return *found;
        };
        auto raw=[&](稳定编码 id) {
            const auto read=读取任一当前事实(l1,id);
            if(read.状态!=L1所有者范围读取状态::成功||read.读取事实代次!=out.Gread||
               !read.事实) {
                if(read.状态==L1所有者范围读取状态::资源失败)
                    throw 纯失败{纯概念状态::资源失败};
                throw 纯失败{纯概念状态::内部不一致};
            }
            return *read.事实;
        };
        for(std::size_t i=0;i<ids.size();++i) {
            const auto fact=raw(ids[i]);
            const auto *node=std::get_if<L1所有者范围节点事实>(&fact);
            std::optional<L1所有者范围值表示种类> representation;
            const auto key = i + 1;
            if (key == 5 || key == 6 || key == 8 || key == 12 || key == 14)
                representation = L1所有者范围值表示种类::I64;
            else if (key == 11)
                representation = L1所有者范围值表示种类::U64组;
            if(!node||node->写入所有者!=port.所有者身份()||
               node->种类!=(representation ? 节点种类::属性类型 : 节点种类::普通)||
               node->属性类型表示!=representation||
               node->创建事实代次!=saved.事实代次)
                throw 纯失败{纯概念状态::内部不一致};
        }
        for(std::uint64_t i=0;i<13;++i) {
            const auto fact=raw(mapped(0x10001U+i));
            const auto *edge=std::get_if<L1所有者范围关系事实>(&fact);
            if(!edge||edge->写入所有者!=port.所有者身份()||
               edge->源节点!=ids[0]||edge->目标节点!=ids[2+i]||
               edge->关系类型节点!=ids[2]||edge->角色或顺序!=static_cast<std::int64_t>(i+1)||
               edge->创建事实代次!=saved.事实代次)
                throw 纯失败{纯概念状态::内部不一致};
        }
        const auto valueFact=raw(mapped(0x20001));
        const auto *value=std::get_if<L1所有者范围值事实>(&valueFact);
        if(!value||value->写入所有者!=port.所有者身份()||value->所属节点!=ids[0]||
           value->属性类型节点!=ids[4]||value->来源节点!=ids[0]||
           std::get<std::int64_t>(value->材料)!=2||
           value->创建事实代次!=saved.事实代次)
            throw 纯失败{纯概念状态::内部不一致};
        out.状态 = replay ? 纯概念状态::精确重复 : 纯概念状态::已创建;
        out.发布 = 纯概念发布状态::确认发布;
    } catch (const std::bad_alloc &) {
        out.状态 = entered ? 纯概念状态::已可能发布 : 纯概念状态::资源失败;
        out.发布 = entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
    } catch (...) {
        out.状态 = entered ? 纯概念状态::已可能发布 : 纯概念状态::内部不一致;
        out.发布 = entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
    }
    return out;
}

bool 特征概念出生使用结构登记结果::成功(
    const 特征概念出生使用结构登记请求& r) const noexcept {
    if (版本 != 1 || r.版本 != 1 ||
        (状态 != 纯概念状态::已创建 && 状态 != 纯概念状态::精确重复) ||
        发布 != 纯概念发布状态::确认发布 || !Gread || !首次发布H ||
        *首次发布H != r.G0 + 1 || *首次发布H > Gread || !原请求 ||
        原请求->版本 != r.版本 || 原请求->G0 != r.G0 ||
        原请求->幂等键 != r.幂等键 || !纯概念结构相同(原请求->纯概念结构, r.纯概念结构) ||
        原请求->最大首次材料项数 != r.最大首次材料项数 || !交付 || !首次材料 ||
        交付->格式 != 1 || !出生结构身份独立(*交付,r.纯概念结构)) return false;
    const auto& x = *首次材料;
    return x.锚点.编码 == 交付->锚点 && x.F到FCv出生使用关系类型.编码 == 交付->F到FCv出生使用关系类型 &&
        x.锚点.写入所有者 == x.F到FCv出生使用关系类型.写入所有者 &&
        x.锚点.写入所有者 == x.类型登记关系.写入所有者 &&
        x.锚点.写入所有者 == x.格式值.写入所有者 &&
        x.锚点.种类 == 节点种类::普通 && !x.锚点.属性类型表示 &&
        x.F到FCv出生使用关系类型.种类 == 节点种类::普通 && !x.F到FCv出生使用关系类型.属性类型表示 &&
        x.类型登记关系.源节点 == 交付->锚点 &&
        x.类型登记关系.目标节点 == 交付->F到FCv出生使用关系类型 &&
        x.类型登记关系.关系类型节点 == r.纯概念结构.类型.类型登记 &&
        x.类型登记关系.角色或顺序 == 1 &&
        x.格式值.所属节点 == 交付->锚点 && x.格式值.来源节点 == 交付->锚点 &&
        x.格式值.属性类型节点 == r.纯概念结构.类型.格式版本 &&
        std::holds_alternative<std::int64_t>(x.格式值.材料) &&
        std::get<std::int64_t>(x.格式值.材料) == 1 &&
        x.锚点.创建事实代次 == *首次发布H &&
        x.F到FCv出生使用关系类型.创建事实代次 == *首次发布H &&
        x.类型登记关系.创建事实代次 == *首次发布H && x.格式值.创建事实代次 == *首次发布H;
}

特征概念出生使用结构登记结果 概念树类数据服务::登记特征概念出生使用结构(
    const L1事实基座服务& l1, L1所有者范围写端口& port,
    const 特征概念出生使用结构登记请求& r) noexcept {
    特征概念出生使用结构登记结果 out;
    out.原请求 = r;
    bool entered = false;
    auto clear_material = [&] { out.交付.reset(); out.首次材料.reset(); };
    try {
        if (!可登记出生使用(r) || r.G0 == UINT64_MAX || !port.有效() || !port.绑定于(l1))
            return out;
        const auto& t = r.纯概念结构.类型;
        const std::array<稳定编码, 15> pure{r.纯概念结构.格式锚点, r.纯概念结构.概念族锚点,
            t.类型登记,t.概念族成员,t.格式版本,t.概念类别,t.定义成员,t.定义种类,
            t.定义特征类型,t.定义模板,t.I64域,t.通用规则,t.直接上位,t.生命周期,t.存在概念使用};
        std::set<std::uint64_t> distinct;
        for (const auto id : pure) if (!有效(id) || !distinct.insert(id.值).second) return out;
        const auto existing=port.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等键});
        out.Gread=existing.读取事实代次;
        if (existing.合同版本 != L1所有者范围首次写入读取合同版本 ||
            existing.所有者 != port.所有者身份() || existing.写入幂等身份 != r.幂等键) {
            out.状态=纯概念状态::内部不一致; return out;
        }
        const bool hasExisting=existing.状态==L1所有者范围读取状态::成功;
        if (!hasExisting && existing.状态 != L1所有者范围读取状态::未找到) {
            out.状态=existing.状态 == L1所有者范围读取状态::资源失败 ?
                纯概念状态::资源失败 : 纯概念状态::内部不一致;
            return out;
        }
        if (!hasExisting && (existing.读取事实代次 != r.G0 ||
            existing.首次规范化写集 || existing.首次写入结果)) {
            out.状态=existing.读取事实代次 != r.G0 ?
                纯概念状态::事实代次漂移 : 纯概念状态::内部不一致;
            return out;
        }
        const auto pureReadGeneration=hasExisting ? existing.读取事实代次 : r.G0;
        if (!已发布纯概念结构_v2(l1,port,r.纯概念结构,pureReadGeneration)) {
            out.状态=纯概念状态::旧格式不支持; return out;
        }
        if (!hasExisting) {
        // The pure delivery is the only structure input.  Its fifteen named facts are
        // read at G0; no owner-domain enumeration or legacy-extension lookup occurs.
        for (std::size_t index=0; index<pure.size(); ++index) {
            const auto id=pure[index];
            const auto read = l1.读取所有者范围当前节点({L1所有者范围CRUD合同版本,id});
            out.Gread = read.读取事实代次;
            const auto* node = read.事实 ? std::get_if<L1所有者范围节点事实>(&*read.事实) : nullptr;
            std::optional<L1所有者范围值表示种类> representation;
            if (index==4 || index==5 || index==7 || index==11 || index==13)
                representation=L1所有者范围值表示种类::I64;
            else if (index==10) representation=L1所有者范围值表示种类::U64组;
            if (read.状态 != L1所有者范围读取状态::成功 || read.读取事实代次 != r.G0 ||
                !node || node->写入所有者 != port.所有者身份() ||
                node->种类 != (representation ? 节点种类::属性类型 : 节点种类::普通) ||
                node->属性类型表示 != representation) {
                out.状态 = read.状态 == L1所有者范围读取状态::事实代次漂移 ||
                    read.读取事实代次 != r.G0 ? 纯概念状态::事实代次漂移 :
                    (read.状态 == L1所有者范围读取状态::资源失败 ? 纯概念状态::资源失败 : 纯概念状态::旧格式不支持);
                return out;
            }
        }
        const auto registrations = l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本, r.纯概念结构.格式锚点, t.类型登记});
        out.Gread = registrations.读取事实代次;
        if (registrations.状态 != L1所有者范围读取状态::成功 || registrations.读取事实代次 != r.G0 ||
            registrations.关系组.size() != 13) {
            out.状态 = registrations.状态 == L1所有者范围读取状态::事实代次漂移 ||
                registrations.读取事实代次 != r.G0 ? 纯概念状态::事实代次漂移 :
                (registrations.状态 == L1所有者范围读取状态::资源失败 ? 纯概念状态::资源失败 : 纯概念状态::旧格式不支持);
            return out;
        }
        std::array<bool,13> seen{};
        for (const auto& e : registrations.关系组) {
            if (e.源节点 != r.纯概念结构.格式锚点 || e.关系类型节点 != t.类型登记 ||
                e.写入所有者 != port.所有者身份() || e.角色或顺序 < 1 || e.角色或顺序 > 13 ||
                seen[static_cast<std::size_t>(e.角色或顺序-1)] ||
                e.目标节点 != pure[static_cast<std::size_t>(e.角色或顺序+1)]) {
                out.状态 = 纯概念状态::旧格式不支持; return out;
            }
            seen[static_cast<std::size_t>(e.角色或顺序-1)]=true;
        }
        }
        L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.幂等键};
        ws.节点.push_back({{1},节点种类::普通,{}});
        ws.节点.push_back({{2},节点种类::普通,{}});
        ws.关系.push_back({{3},Key{1},Key{2},t.类型登记,1});
        ws.值.push_back({{4},Key{1},t.格式版本,std::int64_t{1},Key{1}});
        ws.属性槽变更.push_back({Key{1},t.格式版本,Key{4}});
        const auto& first=existing;
        bool replay=hasExisting;
        if (first.状态 == L1所有者范围读取状态::成功) {
            if (!first.首次规范化写集 || *first.首次规范化写集 != ws) {
                out.状态=纯概念状态::幂等冲突; return out;
            }
            if (!first.首次写入结果 || first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
                first.首次写入结果->事实代次 != r.G0+1 || !first.首次写入结果->是否形成内存权威发布) {
                out.状态=纯概念状态::已可能发布; out.发布=纯概念发布状态::可能发布; return out;
            }
        } else if (first.状态 == L1所有者范围读取状态::未找到) {
            if (first.读取事实代次 != r.G0 || first.首次规范化写集 || first.首次写入结果) {
                out.状态=first.读取事实代次 != r.G0 ? 纯概念状态::事实代次漂移 : 纯概念状态::内部不一致; return out;
            }
        } else { out.状态=first.状态 == L1所有者范围读取状态::资源失败 ? 纯概念状态::资源失败 : 纯概念状态::内部不一致; return out; }
        entered=true;
        const auto saved=port.提交所有者范围中性写集(ws);
        out.Gread=saved.事实代次;
        if (saved.状态 != (replay ? L1所有者范围写入状态::精确重复 : L1所有者范围写入状态::成功) ||
            saved.合同版本 != L1所有者范围CRUD合同版本 || saved.所有者 != port.所有者身份() ||
            saved.写入幂等身份 != r.幂等键 || saved.事实代次 != r.G0+1 || saved.新编码映射.size()!=4) {
            out.状态=saved.状态 == L1所有者范围写入状态::事实代次漂移 ? 纯概念状态::事实代次漂移 :
                (saved.状态 == L1所有者范围写入状态::幂等冲突 ? 纯概念状态::幂等冲突 : 纯概念状态::已可能发布);
            out.发布=saved.是否形成内存权威发布 || replay ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
            clear_material(); return out;
        }
        auto mapped=[&](std::uint64_t key) {
            std::optional<稳定编码> id;
            for (const auto& [k,v] : saved.新编码映射) if (k.值 == key) { if (id || !有效(v)) throw 纯失败{纯概念状态::内部不一致}; id=v; }
            if (!id) throw 纯失败{纯概念状态::内部不一致}; return *id;
        };
        const auto anchor=mapped(1), relationType=mapped(2), registration=mapped(3), format=mapped(4);
        if (!出生结构身份独立({anchor,1,relationType},r.纯概念结构))
            throw 纯失败{纯概念状态::内部不一致};
        const auto tail=l1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (tail.状态 != L1中性读取状态::成功 || !tail.事实代次 || tail.事实代次 < saved.事实代次) throw 纯失败{纯概念状态::已可能发布};
        out.Gread=tail.事实代次; out.首次发布H=saved.事实代次;
        auto rawNode=[&](稳定编码 id) {
            const auto read=读取任一当前事实(l1,id);
            if (read.状态 != L1所有者范围读取状态::成功 || read.读取事实代次 != out.Gread || !read.事实)
                throw 纯失败{read.状态 == L1所有者范围读取状态::资源失败 ? 纯概念状态::资源失败 : 纯概念状态::已可能发布};
            const auto* node=std::get_if<L1所有者范围节点事实>(&*read.事实);
            if (!node || node->创建事实代次 != *out.首次发布H)
                throw 纯失败{纯概念状态::已可能发布};
            return *node;
        };
        const auto an=rawNode(anchor), rtn=rawNode(relationType);
        const auto edgeRead=l1.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本,anchor,t.类型登记});
        const L1所有者范围所属节点当前完整值组读取请求_v2 valueRequest{
            L1所有者范围所属节点当前完整值组读取合同版本_v2,
            port.所有者身份(),anchor,out.Gread};
        const auto valueRead=l1.读取所有者范围所属节点当前完整值组(valueRequest);
        if (edgeRead.状态 != L1所有者范围读取状态::成功 || edgeRead.读取事实代次 != out.Gread ||
            edgeRead.关系组.size() != 1 ||
            valueRead.状态 != L1所有者范围所属节点当前完整值组读取状态_v2::成功 ||
            valueRead.读取事实代次 != out.Gread || valueRead.载荷.size() != 1)
            throw 纯失败{纯概念状态::已可能发布};
        const auto& edge=edgeRead.关系组.front(); const auto& value=valueRead.载荷.front();
        if (edge.编码 != registration || value.编码 != format || an.写入所有者 != port.所有者身份() || rtn.写入所有者 != port.所有者身份() ||
            an.种类 != 节点种类::普通 || an.属性类型表示 ||
            rtn.种类 != 节点种类::普通 || rtn.属性类型表示 ||
            edge.写入所有者 != port.所有者身份() || edge.源节点 != anchor || edge.目标节点 != relationType || edge.关系类型节点 != t.类型登记 || edge.角色或顺序 != 1 || edge.创建事实代次 != *out.首次发布H ||
            value.写入所有者 != port.所有者身份() || value.所属节点 != anchor || value.来源节点 != anchor || value.属性类型节点 != t.格式版本 || value.创建事实代次 != *out.首次发布H ||
            !std::holds_alternative<std::int64_t>(value.材料) || std::get<std::int64_t>(value.材料) != 1)
            throw 纯失败{纯概念状态::内部不一致};
        out.交付=特征概念出生使用结构交付{anchor,1,relationType};
        out.首次材料=特征概念出生使用结构首次材料读回{an,rtn,edge,value};
        out.状态=replay ? 纯概念状态::精确重复 : 纯概念状态::已创建;
        out.发布=纯概念发布状态::确认发布;
        if (!out.成功(r)) throw 纯失败{纯概念状态::内部不一致};
    } catch (const 纯失败& failure) {
        out.状态=failure.状态;
        out.发布=entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
        if (entered) { out.状态=纯概念状态::已可能发布; out.首次发布H.reset(); clear_material(); }
        else { out.首次发布H.reset(); clear_material(); }
    } catch (const std::bad_alloc&) {
        out.状态=entered ? 纯概念状态::已可能发布 : 纯概念状态::资源失败;
        out.发布=entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
        clear_material(); out.首次发布H.reset();
    } catch (...) {
        out.状态=entered ? 纯概念状态::已可能发布 : 纯概念状态::内部不一致;
        out.发布=entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
        clear_material(); out.首次发布H.reset();
    }
    return out;
}


纯概念定义 概念树类数据服务::规范化纯概念定义(
    const 纯概念定义 &input, std::uint64_t g,
    const 概念树预算 &budget, 概念事实读取会话_v1* session,
    特征值域事实读取会话_v1* featureSession) const {
    if (!pure_layout_ || !g || !budget.最大概念数 ||
        !budget.最大关系数 || !budget.最大特征属性数) {
        if(!pure_layout_) throw 纯失败{纯概念状态::旧格式不支持};
        throw 纯失败{纯概念状态::入口拒绝};
    }
    if (const auto *fc = std::get_if<纯I64特征概念定义>(&input)) {
        if (!有效(fc->特征类型.值) || fc->规范域.empty() ||
            fc->规范域.size() > budget.最大特征属性数)
            throw 纯失败{纯概念状态::定义不相容};
        特征规范I64域 raw;
        for (const auto &x : fc->规范域) raw.区间.push_back({x.下界,x.上界});
        if(session&&featureSession) {
            if(session->用量_.节点数>session->预算_.最大节点数||
               session->用量_.关系数>session->预算_.最大关系数||
               session->用量_.值数>session->预算_.最大值数||
               session->用量_.材料总数>session->预算_.最大材料总数)
                throw 纯失败{纯概念状态::数量预算不足};
            featureSession->计量_.上限={featureSession->计量_.用量.节点数+
                    (session->预算_.最大节点数-session->用量_.节点数),
                featureSession->计量_.用量.关系数+
                    (session->预算_.最大关系数-session->用量_.关系数),
                featureSession->计量_.用量.属性值数+
                    (session->预算_.最大值数-session->用量_.值数),
                featureSession->计量_.用量.材料总数+
                    (session->预算_.最大材料总数-session->用量_.材料总数)};
        }
        const auto normalized = 特征结果(featureSession ?
            features_.保护<特征截止事实<特征规范I64域>>([&] {
                features_.守卫(g);
                auto full=features_.读完整域({fc->特征类型.值},g,
                    &featureSession->计量_);
                auto domain=features_.规范域(raw);
                features_.要求(features_.包含(full,domain),特征数据错误::类型不相容);
                features_.守卫(g);
                return 特征截止事实<特征规范I64域>{g,std::move(domain)};
            }) : features_.规范化I64特征域({{1,g,{fc->特征类型.值}},raw}));
        if (normalized.Gread != g || normalized.数据.区间.empty())
            throw 纯失败{纯概念状态::定义不相容};
        纯I64特征概念定义 out; out.特征类型 = fc->特征类型;
        for (const auto &x : normalized.数据.区间) out.规范域.push_back({x.下界,x.上界});
        return out;
    }
    if (const auto *ec = std::get_if<纯合取存在概念定义>(&input)) {
        auto out = *ec;
        std::sort(out.特征模板组.begin(), out.特征模板组.end(),
                  [](const auto &a,const auto &b){return a.值.值<b.值.值;});
        out.特征模板组.erase(std::unique(out.特征模板组.begin(),out.特征模板组.end()),
                             out.特征模板组.end());
        if (out.特征模板组.empty() || out.特征模板组.size() > budget.最大概念数)
            throw 纯失败{纯概念状态::定义不相容};
        for (const auto id : out.特征模板组) {
            if(session&&featureSession) {
                session->预算_={session->用量_.节点数+
                        (featureSession->计量_.上限.最大节点数-featureSession->计量_.用量.节点数),
                    session->用量_.关系数+
                        (featureSession->计量_.上限.最大关系数-featureSession->计量_.用量.关系数),
                    session->用量_.值数+
                        (featureSession->计量_.上限.最大属性值数-featureSession->计量_.用量.属性值数),
                    session->用量_.材料总数+
                        (featureSession->计量_.上限.最大材料总数-featureSession->计量_.用量.材料总数)};
            }
            const auto fact = 读取纯概念内部(id,g,budget,session,featureSession);
            if (fact.类别 != 相关概念类别::特征 ||
                (fact.治理状态 != 概念树生命周期状态::活跃 &&
                 fact.治理状态 != 概念树生命周期状态::冷却))
                throw 纯失败{纯概念状态::定义不相容};
        }
        return out;
    }
    const auto &generic = std::get<通用存在概念定义>(input);
    if (generic.规则版本 != 1 || generic.规则 != 通用存在定义规则::不预设特征)
        throw 纯失败{纯概念状态::定义不支持};
    return generic;
}

纯概念定义 概念树类数据服务::规范化纯概念定义完整(
    const 纯概念定义 &input, std::uint64_t g) const {
    if (!pure_layout_) throw 纯失败{纯概念状态::旧格式不支持};
    if (!g) throw 纯失败{纯概念状态::入口拒绝};
    if (const auto *fc = std::get_if<纯I64特征概念定义>(&input)) {
        if (!有效(fc->特征类型.值) || fc->规范域.empty())
            throw 纯失败{纯概念状态::定义不相容};
        特征规范I64域 raw;
        raw.区间.reserve(fc->规范域.size());
        for (const auto &item : fc->规范域)
            raw.区间.push_back({item.下界, item.上界});
        const auto normalized = 特征结果(features_.规范化I64特征域(
            {{1, g, {fc->特征类型.值}}, std::move(raw)}));
        if (normalized.Gread != g ||
            normalized.数据.区间.empty())
            throw 纯失败{纯概念状态::定义不相容};
        纯I64特征概念定义 out;
        out.特征类型 = fc->特征类型;
        out.规范域.reserve(normalized.数据.区间.size());
        for (const auto &item : normalized.数据.区间)
            out.规范域.push_back({item.下界, item.上界});
        return out;
    }
    if (const auto *ec = std::get_if<纯合取存在概念定义>(&input)) {
        auto out = *ec;
        std::sort(out.特征模板组.begin(), out.特征模板组.end(),
                  [](auto a, auto b) { return a.值.值 < b.值.值; });
        out.特征模板组.erase(
            std::unique(out.特征模板组.begin(), out.特征模板组.end()),
            out.特征模板组.end());
        if (out.特征模板组.empty())
            throw 纯失败{纯概念状态::定义不相容};
        for (const auto child : out.特征模板组) {
            const auto fact = 读取纯概念完整内部(child, g);
            if (fact.类别 != 相关概念类别::特征 ||
                (fact.治理状态 != 概念树生命周期状态::活跃 &&
                 fact.治理状态 != 概念树生命周期状态::冷却))
                throw 纯失败{纯概念状态::定义不相容};
        }
        return out;
    }
    const auto &generic = std::get<通用存在概念定义>(input);
    if (generic.规则版本 != 1 ||
        generic.规则 != 通用存在定义规则::不预设特征)
        throw 纯失败{纯概念状态::定义不支持};
    return generic;
}

纯概念事实 概念树类数据服务::读取纯概念内部(
    概念树概念身份 id, std::uint64_t g,
    const 概念树预算 &budget, 概念事实读取会话_v1* session,
    特征值域事实读取会话_v1* featureSession) const {
    if (!pure_layout_) throw 纯失败{纯概念状态::旧格式不支持};
    if (!有效(id.值) || !g || !budget.最大关系数 ||
        !budget.最大概念数 || !budget.最大特征属性数)
        throw 纯失败{纯概念状态::入口拒绝};
    if(session && !session->已完整概念_.contains(id.值.值) &&
       session->已完整概念_.size()>=session->最大概念数_)
        throw 纯失败{纯概念状态::数量预算不足};
    const auto &t = pure_layout_->类型;
    纯概念事实 out; out.概念=id;
    const auto conceptNode=节点(id.值,g,session);
    out.生命周期=投影纯生命(conceptNode);
    const auto members=关系(id.值,t.概念族成员,true,g,1,session);
    if(members.size()!=1||members.front().源节点!=pure_layout_->概念族锚点||
       members.front().目标节点!=id.值||members.front().角色或顺序!=1||
       members.front().创建事实代次!=out.生命周期.创建事实代次)
        throw 纯失败{纯概念状态::内部不一致};
    const auto cv=属性(id.值,g,session);
    if(cv.size()!=2) throw 纯失败{纯概念状态::内部不一致};
    const auto category=std::get<std::int64_t>(唯一属性(cv,t.概念类别).材料);
    const auto lifecycle=std::get<std::int64_t>(唯一属性(cv,t.生命周期).材料);
    if((category!=1&&category!=2)||(lifecycle<1||lifecycle>3))
        throw 纯失败{纯概念状态::内部不一致};
    out.类别=static_cast<相关概念类别>(category);
    out.治理状态=static_cast<概念树生命周期状态>(lifecycle);
    const auto &lifeValue=唯一属性(cv,t.生命周期);
    out.生命周期值事实=lifeValue.编码;
    out.生命周期值生命周期=投影纯生命(lifeValue);
    const auto defs=关系(id.值,t.定义成员,false,g,1,session);
    if(defs.size()!=1||defs.front().角色或顺序!=1)
        throw 纯失败{纯概念状态::内部不一致};
    out.定义记录=defs.front().目标节点;
    out.定义记录生命周期=投影纯生命(节点(out.定义记录,g,session));
    out.定义关系组.push_back({defs.front().编码,defs.front().源节点,
        defs.front().目标节点,defs.front().关系类型节点,
        纯概念定义关系种类::定义成员,1,投影纯生命(defs.front())});
    const auto dv=属性(out.定义记录,g,session);
    const auto kind=std::get<std::int64_t>(唯一属性(dv,t.定义种类).材料);
    if(kind==1) {
        if(out.类别!=相关概念类别::特征||dv.size()!=2)
            throw 纯失败{纯概念状态::类别冲突};
        const auto ft=关系(out.定义记录,t.定义特征类型,false,g,1,session);
        if(ft.size()!=1||ft.front().角色或顺序!=1)
            throw 纯失败{纯概念状态::内部不一致};
        const auto &domain=唯一属性(dv,t.I64域);
        const auto &raw=std::get<std::vector<std::uint64_t>>(domain.材料);
        if(raw.empty()||raw.size()%2) throw 纯失败{纯概念状态::定义不相容};
        纯I64特征概念定义 def;
        def.特征类型=概念树特征类型引用{ft.front().目标节点};
        for(std::size_t i=0;i<raw.size();i+=2)
            def.规范域.push_back({std::bit_cast<std::int64_t>(raw[i]),
                                  std::bit_cast<std::int64_t>(raw[i+1])});
        const auto normalized=规范化纯概念定义(def,g,budget,session,featureSession);
        if(normalized!=纯概念定义{def})
            throw 纯失败{纯概念状态::内部不一致};
        out.定义=std::move(def);
        out.定义关系组.push_back({ft.front().编码,ft.front().源节点,
            ft.front().目标节点,ft.front().关系类型节点,
            纯概念定义关系种类::定义特征类型,1,投影纯生命(ft.front())});
    } else if(kind==2) {
        if(out.类别!=相关概念类别::存在||dv.size()!=1)
            throw 纯失败{纯概念状态::类别冲突};
        const auto edges=关系(out.定义记录,t.定义模板,false,g,budget.最大关系数,session);
        if(edges.empty()) throw 纯失败{纯概念状态::定义不相容};
        纯合取存在概念定义 def;
        for(std::size_t i=0;i<edges.size();++i) {
            if(edges[i].角色或顺序!=static_cast<std::int64_t>(i+1))
                throw 纯失败{纯概念状态::内部不一致};
            def.特征模板组.emplace_back(edges[i].目标节点);
            out.定义关系组.push_back({edges[i].编码,edges[i].源节点,
                edges[i].目标节点,edges[i].关系类型节点,
                纯概念定义关系种类::定义模板,i+1,投影纯生命(edges[i])});
        }
        for(std::size_t i=1;i<def.特征模板组.size();++i)
            if(def.特征模板组[i-1].值.值>=def.特征模板组[i].值.值)
                throw 纯失败{纯概念状态::内部不一致};
        const auto normalized=规范化纯概念定义(def,g,budget,session,featureSession);
        if(normalized!=纯概念定义{def})
            throw 纯失败{纯概念状态::内部不一致};
        out.定义=std::move(def);
    } else if(kind==3) {
        if(out.类别!=相关概念类别::存在||dv.size()!=2||
           std::get<std::int64_t>(唯一属性(dv,t.通用规则).材料)!=1)
            throw 纯失败{纯概念状态::定义不支持};
        out.定义=通用存在概念定义{1,通用存在定义规则::不预设特征};
    } else throw 纯失败{纯概念状态::定义不支持};
    const auto parents=关系(id.值,t.直接上位,true,g,budget.最大关系数,session);
    std::set<std::uint64_t> seen;
    for(const auto &e:parents) {
        if(e.角色或顺序!=1||e.源节点==id.值||!seen.insert(e.源节点.值).second)
            throw 纯失败{纯概念状态::上位成环};
        out.直接上位.push_back({e.编码,概念树概念身份{e.源节点},id,投影纯生命(e)});
    }
    if(session) session->已完整概念_.insert(id.值.值);
    return out;
}

纯概念事实 概念树类数据服务::读取纯概念完整内部(
    概念树概念身份 id, std::uint64_t g) const {
    std::set<std::uint64_t> active;
    return 读取纯概念完整内部(id, g, active);
}

纯概念事实 概念树类数据服务::读取纯概念完整内部(
    概念树概念身份 id, std::uint64_t g,
    std::set<std::uint64_t> &active) const {
    if (!pure_layout_) throw 纯失败{纯概念状态::旧格式不支持};
    if (!有效(id.值) || !g)
        throw 纯失败{纯概念状态::入口拒绝};
    if (!active.insert(id.值.值).second)
        throw 纯失败{纯概念状态::内部不一致};
    struct ActiveGuard final {
        std::set<std::uint64_t> &set;
        std::uint64_t value;
        ~ActiveGuard() { set.erase(value); }
    } activeGuard{active, id.值.值};

    const auto &t = pure_layout_->类型;
    纯概念事实 out;
    out.概念 = id;
    const auto conceptNode = 节点(id.值, g);
    out.生命周期 = 投影纯生命(conceptNode);
    const auto members = 完整关系组(id.值, t.概念族成员, true, g);
    if (members.size() != 1 || members.front().源节点 != pure_layout_->概念族锚点 ||
        members.front().目标节点 != id.值 || members.front().角色或顺序 != 1 ||
        members.front().创建事实代次 != out.生命周期.创建事实代次)
        throw 纯失败{纯概念状态::内部不一致};

    const auto conceptValues = 完整属性值组(id.值, g);
    if (conceptValues.size() != 2)
        throw 纯失败{纯概念状态::内部不一致};
    const auto category = std::get<std::int64_t>(唯一属性(conceptValues, t.概念类别).材料);
    const auto lifecycle = std::get<std::int64_t>(唯一属性(conceptValues, t.生命周期).材料);
    if ((category != 1 && category != 2) || lifecycle < 1 || lifecycle > 3)
        throw 纯失败{纯概念状态::内部不一致};
    out.类别 = static_cast<相关概念类别>(category);
    out.治理状态 = static_cast<概念树生命周期状态>(lifecycle);
    const auto &lifeValue = 唯一属性(conceptValues, t.生命周期);
    out.生命周期值事实 = lifeValue.编码;
    out.生命周期值生命周期 = 投影纯生命(lifeValue);

    const auto definitions = 完整关系组(id.值, t.定义成员, false, g);
    if (definitions.size() != 1 || definitions.front().角色或顺序 != 1)
        throw 纯失败{纯概念状态::内部不一致};
    out.定义记录 = definitions.front().目标节点;
    out.定义记录生命周期 = 投影纯生命(节点(out.定义记录, g));
    out.定义关系组.push_back(
        {definitions.front().编码, definitions.front().源节点,
         definitions.front().目标节点, definitions.front().关系类型节点,
         纯概念定义关系种类::定义成员, 1, 投影纯生命(definitions.front())});

    const auto definitionValues = 完整属性值组(out.定义记录, g);
    const auto kind = std::get<std::int64_t>(唯一属性(definitionValues, t.定义种类).材料);
    if (kind == 1) {
        if (out.类别 != 相关概念类别::特征 || definitionValues.size() != 2)
            throw 纯失败{纯概念状态::类别冲突};
        const auto featureTypes = 完整关系组(out.定义记录, t.定义特征类型, false, g);
        if (featureTypes.size() != 1 || featureTypes.front().角色或顺序 != 1)
            throw 纯失败{纯概念状态::内部不一致};
        const auto &domain = 唯一属性(definitionValues, t.I64域);
        const auto &raw = std::get<std::vector<std::uint64_t>>(domain.材料);
        if (raw.empty() || raw.size() % 2)
            throw 纯失败{纯概念状态::定义不相容};
        纯I64特征概念定义 definition;
        definition.特征类型 = 概念树特征类型引用{featureTypes.front().目标节点};
        特征规范I64域 input;
        input.区间.reserve(raw.size() / 2);
        for (std::size_t i = 0; i < raw.size(); i += 2) {
            definition.规范域.push_back(
                {std::bit_cast<std::int64_t>(raw[i]),
                 std::bit_cast<std::int64_t>(raw[i + 1])});
            input.区间.push_back(
                {std::bit_cast<std::int64_t>(raw[i]),
                 std::bit_cast<std::int64_t>(raw[i + 1])});
        }
        const auto normalized = 特征结果(features_.规范化I64特征域(
            {{1, g, 特征类型身份{definition.特征类型.值}}, std::move(input)}));
        if (normalized.Gread != g ||
            normalized.数据.区间.size() != definition.规范域.size())
            throw 纯失败{纯概念状态::内部不一致};
        for (std::size_t i = 0; i < definition.规范域.size(); ++i)
            if (definition.规范域[i].下界 != normalized.数据.区间[i].下界 ||
                definition.规范域[i].上界 != normalized.数据.区间[i].上界)
                throw 纯失败{纯概念状态::内部不一致};
        out.定义 = std::move(definition);
        out.定义关系组.push_back(
            {featureTypes.front().编码, featureTypes.front().源节点,
             featureTypes.front().目标节点, featureTypes.front().关系类型节点,
             纯概念定义关系种类::定义特征类型, 1,
             投影纯生命(featureTypes.front())});
    } else if (kind == 2) {
        if (out.类别 != 相关概念类别::存在 || definitionValues.size() != 1)
            throw 纯失败{纯概念状态::类别冲突};
        const auto templates = 完整关系组(out.定义记录, t.定义模板, false, g);
        if (templates.empty()) throw 纯失败{纯概念状态::定义不相容};
        纯合取存在概念定义 definition;
        for (std::size_t i = 0; i < templates.size(); ++i) {
            if (templates[i].角色或顺序 != static_cast<std::int64_t>(i + 1))
                throw 纯失败{纯概念状态::内部不一致};
            const 概念树概念身份 child{templates[i].目标节点};
            if (!definition.特征模板组.empty() &&
                definition.特征模板组.back().值.值 >= child.值.值)
                throw 纯失败{纯概念状态::内部不一致};
            const auto childFact = 读取纯概念完整内部(child, g, active);
            if (childFact.类别 != 相关概念类别::特征 ||
                (childFact.治理状态 != 概念树生命周期状态::活跃 &&
                 childFact.治理状态 != 概念树生命周期状态::冷却))
                throw 纯失败{纯概念状态::定义不相容};
            definition.特征模板组.push_back(child);
            out.定义关系组.push_back(
                {templates[i].编码, templates[i].源节点, templates[i].目标节点,
                 templates[i].关系类型节点, 纯概念定义关系种类::定义模板,
                 i + 1, 投影纯生命(templates[i])});
        }
        out.定义 = std::move(definition);
    } else if (kind == 3) {
        if (out.类别 != 相关概念类别::存在 || definitionValues.size() != 2 ||
            std::get<std::int64_t>(唯一属性(definitionValues, t.通用规则).材料) != 1)
            throw 纯失败{纯概念状态::定义不支持};
        out.定义 = 通用存在概念定义{1, 通用存在定义规则::不预设特征};
    } else {
        throw 纯失败{纯概念状态::定义不支持};
    }

    const auto parents = 完整关系组(id.值, t.直接上位, true, g);
    std::set<std::uint64_t> seenParents;
    for (const auto &edge : parents) {
        if (edge.角色或顺序 != 1 || edge.源节点 == id.值 ||
            !seenParents.insert(edge.源节点.值).second)
            throw 纯失败{纯概念状态::上位成环};
        out.直接上位.push_back(
            {edge.编码, 概念树概念身份{edge.源节点}, id, 投影纯生命(edge)});
    }
    if (!纯概念结果内部::事实完整(out, g))
        throw 纯失败{纯概念状态::内部不一致};
    return out;
}

纯概念读取结果 概念树类数据服务::读取纯概念(
    const 纯概念读取请求 &r) const noexcept {
    纯概念读取结果 out; out.Gread=r.Gread;
    try {
        if(r.版本!=2||!r.Gread||!有效(r.概念.值))
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        out.事实=读取纯概念内部(r.概念,r.Gread,r.预算);
        out.状态=纯概念状态::已读取;
        守卫代次(r.Gread);
    } catch(const 纯失败&e){out.状态=e.状态;out.事实.reset();}
      catch(const 失败&e){
        switch(e.状态) {
        case S::入口拒绝:out.状态=纯概念状态::入口拒绝;break;
        case S::未找到:out.状态=纯概念状态::未找到;break;
        case S::引用冲突:out.状态=纯概念状态::引用冲突;break;
        case S::事实代次漂移:out.状态=纯概念状态::事实代次漂移;break;
        case S::数量预算不足:out.状态=纯概念状态::数量预算不足;break;
        case S::资源失败:out.状态=纯概念状态::资源失败;break;
        case S::旧格式不支持:out.状态=纯概念状态::旧格式不支持;break;
        default:out.状态=纯概念状态::内部不一致;break;
        }
        out.事实.reset();
      } catch(const std::bad_alloc&){out.状态=纯概念状态::资源失败;out.事实.reset();}
      catch(const std::length_error&){out.状态=纯概念状态::资源失败;out.事实.reset();}
      catch(...){out.状态=纯概念状态::内部不一致;out.事实.reset();}
    return out;
}

纯概念完整读取结果_v3 概念树类数据服务::读取纯概念完整(
    const 纯概念完整读取请求_v3 &r) const noexcept {
    纯概念完整读取结果_v3 out;
    out.Gread = r.Gread;
    try {
        if (r.版本 != 3 || !r.Gread || !有效(r.概念.值))
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        out.事实 = 读取纯概念完整内部(r.概念, r.Gread);
        out.状态 = 纯概念状态::已读取;
        守卫代次(r.Gread);
        if (!out.成功(r)) throw 纯失败{纯概念状态::内部不一致};
    } catch (const 纯失败 &e) {
        out.状态 = e.状态;
        out.事实.reset();
    } catch (const 失败 &e) {
        switch (e.状态) {
        case S::入口拒绝: out.状态 = 纯概念状态::入口拒绝; break;
        case S::未找到: out.状态 = 纯概念状态::未找到; break;
        case S::引用冲突: out.状态 = 纯概念状态::引用冲突; break;
        case S::事实代次漂移: out.状态 = 纯概念状态::事实代次漂移; break;
        case S::资源失败: out.状态 = 纯概念状态::资源失败; break;
        case S::旧格式不支持: out.状态 = 纯概念状态::旧格式不支持; break;
        default: out.状态 = 纯概念状态::内部不一致; break;
        }
        out.事实.reset();
    } catch (const std::bad_alloc &) {
        out.状态 = 纯概念状态::资源失败;
        out.事实.reset();
    } catch (const std::length_error &) {
        out.状态 = 纯概念状态::资源失败;
        out.事实.reset();
    } catch (...) {
        out.状态 = 纯概念状态::内部不一致;
        out.事实.reset();
    }
    return out;
}

bool 特征概念值域基础读取结果_v1::成功(
    const 特征概念值域基础读取请求_v1& r) const noexcept {
    return 版本 == 1 && r.版本 == 1 && 状态 == 特征概念值域基础读取状态_v1::已读取
        && Gread == r.Gread && 事实
        && 事实->Gread == r.Gread && 事实->FC == r.FC
        && 有效(事实->FC.值) && 有效(事实->FT) && 事实->表示 == 特征值表示类型::I64
        && 事实->I64域.has_value() && !事实->I64域->区间.empty()
        && 读取用量.节点数 <= r.读取预算.最大节点数
        && 读取用量.关系数 <= r.读取预算.最大关系数
        && 读取用量.值数 <= r.读取预算.最大值数
        && 读取用量.材料总数 <= r.读取预算.最大材料总数
        && 读取用量.材料总数 == 读取用量.节点数 + 读取用量.关系数 + 读取用量.值数;
}

bool 特征概念值域基础读取结果_v2::成功(
    const 特征概念值域基础读取请求_v2 &r) const noexcept {
    if (版本 != 2 || r.版本 != 2 || !r.Gread ||
        !有效(r.FC.值) || 状态 != 特征概念值域基础读取状态_v2::已读取 ||
        Gread != r.Gread || !事实 || 事实->FC != r.FC ||
        !有效(事实->FT) || 事实->原始表示 != 特征值表示类型::I64 ||
        !有效(事实->规则身份) || 事实->规则版本 != 1 ||
        事实->完整纯概念事实.概念 != r.FC ||
        事实->完整纯概念事实.类别 != 相关概念类别::特征 ||
        !纯概念结果内部::事实完整(事实->完整纯概念事实, Gread))
        return false;
    const auto *definition =
        std::get_if<纯I64特征概念定义>(&事实->完整纯概念事实.定义);
    return definition && definition->特征类型.值 == 事实->FT.编码;
}

特征概念值域基础读取结果_v1 概念树类数据服务::读取特征概念值域基础(
    const 特征概念值域基础读取请求_v1& r) const noexcept {
    概念事实读取会话_v1 session(port_.所有者身份(),r.Gread,r.读取预算,
                                  r.预算.最大概念数);
    return 读取特征概念值域基础共享(r,session);
}

特征概念值域基础读取结果_v1 概念树类数据服务::读取特征概念值域基础共享(
    const 特征概念值域基础读取请求_v1& r,
    概念事实读取会话_v1& session) const noexcept {
    特征概念值域基础读取结果_v1 out; out.Gread = r.Gread;
    struct 用量回填 final {
        特征概念值域基础读取结果_v1& 输出;
        const 有界事实读取用量_B1& 用量;
        ~用量回填() { 输出.读取用量=用量; }
    } guard{out,session.用量_};
    try {
        if (r.版本 != 1 || !r.Gread || !有效(r.FC.值)
            || !r.预算.最大概念数 || !r.预算.最大关系数 || !r.预算.最大特征属性数
            || !r.读取预算.最大节点数 || !r.读取预算.最大关系数
            || !r.读取预算.最大值数 || !r.读取预算.最大材料总数
            || session.所有者_ != port_.所有者身份() || session.Gread_ != r.Gread
            || session.预算_ != r.读取预算
            || session.最大概念数_ != r.预算.最大概念数) {
            out.状态 = 特征概念值域基础读取状态_v1::入口拒绝;
            return out;
        }
        守卫代次(r.Gread);
        const auto fact = 读取纯概念内部(r.FC,r.Gread,r.预算,&session);
        if (fact.类别 != 相关概念类别::特征) {
            out.状态 = 特征概念值域基础读取状态_v1::类别冲突;
            return out;
        }
        if (fact.治理状态 == 概念树生命周期状态::退役) {
            out.状态 = 特征概念值域基础读取状态_v1::未找到;
            return out;
        }
        const auto* definition = std::get_if<纯I64特征概念定义>(&fact.定义);
        // 待实现：非 I64 特征概念的格式、规则和完整域适配器尚未交付。
        if (!definition) {
            out.状态 = 特征概念值域基础读取状态_v1::未实现;
            return out;
        }
        特征规范I64域 raw;
        raw.区间.reserve(definition->规范域.size());
        for (const auto& interval : definition->规范域)
            raw.区间.push_back({interval.下界, interval.上界});
        const auto normalized = features_.规范化I64特征域(
            {{1, r.Gread, 特征类型身份{definition->特征类型.值}}, std::move(raw)});
        if (const auto* value = std::get_if<特征截止事实<特征规范I64域>>(&normalized)) {
            if (value->Gread != r.Gread) {
                out.状态 = 特征概念值域基础读取状态_v1::事实代次漂移;
                return out;
            }
            out.事实 = 特征概念值域基础事实_v1{r.FC, 特征类型身份{definition->特征类型.值},
                特征值表示类型::I64, value->数据, fact.生命周期, r.Gread};
            out.状态 = 特征概念值域基础读取状态_v1::已读取;
            守卫代次(r.Gread);
            return out;
        }
        const auto error = std::get<特征数据错误>(normalized);
        out.状态 = error == 特征数据错误::规则缺失 ? 特征概念值域基础读取状态_v1::规则缺失 :
            error == 特征数据错误::并发变化 ? 特征概念值域基础读取状态_v1::事实代次漂移 :
            error == 特征数据错误::数量预算不足 ? 特征概念值域基础读取状态_v1::数量预算不足 :
            error == 特征数据错误::资源失败 ? 特征概念值域基础读取状态_v1::资源失败 :
            error == 特征数据错误::未找到 ? 特征概念值域基础读取状态_v1::未找到 :
            特征概念值域基础读取状态_v1::内部不一致;
    } catch (const 纯失败& e) {
        switch(e.状态) {
        case 纯概念状态::未找到: out.状态=特征概念值域基础读取状态_v1::未找到; break;
        case 纯概念状态::概念已退役:
            out.状态=特征概念值域基础读取状态_v1::未找到; break;
        case 纯概念状态::事实代次漂移: out.状态=特征概念值域基础读取状态_v1::事实代次漂移; break;
        case 纯概念状态::数量预算不足: out.状态=特征概念值域基础读取状态_v1::数量预算不足; break;
        case 纯概念状态::资源失败: out.状态=特征概念值域基础读取状态_v1::资源失败; break;
        case 纯概念状态::入口拒绝: out.状态=特征概念值域基础读取状态_v1::入口拒绝; break;
        default: out.状态=特征概念值域基础读取状态_v1::内部不一致; break;
        }
      } catch (const 失败& e) {
        switch(e.状态) {
        case S::未找到: out.状态=特征概念值域基础读取状态_v1::未找到; break;
        case S::事实代次漂移: out.状态=特征概念值域基础读取状态_v1::事实代次漂移; break;
        case S::数量预算不足: out.状态=特征概念值域基础读取状态_v1::数量预算不足; break;
        case S::资源失败: out.状态=特征概念值域基础读取状态_v1::资源失败; break;
        case S::入口拒绝: out.状态=特征概念值域基础读取状态_v1::入口拒绝; break;
        default: out.状态=特征概念值域基础读取状态_v1::内部不一致; break;
        }
      } catch (const std::bad_alloc&) { out.状态 = 特征概念值域基础读取状态_v1::资源失败; }
      catch (const std::length_error&) { out.状态 = 特征概念值域基础读取状态_v1::资源失败; }
      catch (...) { out.状态 = 特征概念值域基础读取状态_v1::内部不一致; }
    out.事实.reset();
    return out;
}

特征概念值域基础读取结果_v2 概念树类数据服务::读取特征概念值域基础_v2(
    const 特征概念值域基础读取请求_v2 &r) const noexcept {
    using RS = 特征概念值域基础读取状态_v2;
    特征概念值域基础读取结果_v2 out;
    out.Gread = r.Gread;
    const auto fail = [&](RS state) {
        out.状态 = state;
        out.事实.reset();
    };
    try {
        if (r.版本 != 2 || !r.Gread || !有效(r.FC.值)) {
            fail(RS::入口拒绝);
            return out;
        }
        守卫代次(r.Gread);
        const auto conceptResult = 读取纯概念完整({3, r.Gread, r.FC});
        if (!conceptResult.成功({3, r.Gread, r.FC})) {
            fail(conceptResult.状态 == 纯概念状态::未找到 ? RS::未找到
                 : conceptResult.状态 == 纯概念状态::概念已退役
                     ? RS::未找到
                 : conceptResult.状态 == 纯概念状态::类别冲突 ? RS::类别冲突
                 : conceptResult.状态 == 纯概念状态::事实代次漂移 ? RS::事实代次漂移
                 : conceptResult.状态 == 纯概念状态::资源失败 ? RS::资源失败
                 : conceptResult.状态 == 纯概念状态::入口拒绝 ? RS::入口拒绝
                 : RS::内部不一致);
            return out;
        }
        if (conceptResult.事实->类别 != 相关概念类别::特征) {
            fail(RS::类别冲突);
            return out;
        }
        if (conceptResult.事实->治理状态 == 概念树生命周期状态::退役) {
            fail(RS::未找到);
            return out;
        }
        const auto *definition =
            std::get_if<纯I64特征概念定义>(&conceptResult.事实->定义);
        // 待实现：非 I64 特征概念的完整域格式与规则适配由对应表示提供者补齐。
        if (!definition) {
            fail(RS::未实现);
            return out;
        }
        const 特征类型身份 ft{definition->特征类型.值};
        const auto type = features_.读取I64基础特征类型事实({1, r.Gread, ft});
        if (const auto *error = std::get_if<特征数据错误>(&type)) {
            fail(*error == 特征数据错误::规则缺失 ? RS::规则缺失
                 : *error == 特征数据错误::并发变化 ? RS::事实代次漂移
                 : *error == 特征数据错误::资源失败 ? RS::资源失败
                 : *error == 特征数据错误::未找到 ? RS::未找到
                 : RS::内部不一致);
            return out;
        }
        const auto &typeFact = std::get<特征截止事实<I64基础特征类型信息>>(type);
        if (typeFact.Gread != r.Gread ||
            typeFact.数据.身份 != ft || !typeFact.数据.规则 ||
            !有效(*typeFact.数据.规则)) {
            fail(typeFact.Gread != r.Gread ? RS::事实代次漂移 : RS::规则缺失);
            return out;
        }
        out.事实 = 特征概念值域基础事实_v2{
            r.FC, ft, 特征值表示类型::I64, *typeFact.数据.规则, 1,
            std::move(*conceptResult.事实)};
        out.状态 = RS::已读取;
        守卫代次(r.Gread);
        if (!out.成功(r)) fail(RS::内部不一致);
    } catch (const 失败 &e) {
        fail(e.状态 == S::入口拒绝 ? RS::入口拒绝
             : e.状态 == S::未找到 ? RS::未找到
             : e.状态 == S::事实代次漂移 ? RS::事实代次漂移
             : e.状态 == S::资源失败 ? RS::资源失败
             : RS::内部不一致);
    } catch (const std::bad_alloc &) {
        fail(RS::资源失败);
    } catch (const std::length_error &) {
        fail(RS::资源失败);
    } catch (...) {
        fail(RS::内部不一致);
    }
    return out;
}

纯概念查询结果 概念树类数据服务::精确查询纯概念(
    const 纯概念查询请求 &r) const noexcept {
    纯概念查询结果 out; out.Gread=r.Gread;
    try {
        if(r.版本!=2||!r.Gread)
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        const auto expected=规范化纯概念定义(r.定义,r.Gread,r.预算);
        const auto members=关系(pure_layout_->概念族锚点,pure_layout_->类型.概念族成员,
                                false,r.Gread,r.预算.最大概念数);
        std::optional<纯概念事实> found;
        for(const auto &m:members) {
            if(m.角色或顺序!=1) throw 纯失败{纯概念状态::内部不一致};
            const auto conceptNode=节点(m.目标节点,r.Gread);
            if(conceptNode.种类!=节点种类::普通||conceptNode.属性类型表示)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitions=关系(m.目标节点,pure_layout_->类型.定义成员,
                                        false,r.Gread,r.预算.最大关系数);
            if(definitions.size()!=1||definitions.front().角色或顺序!=1)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitionNode=节点(definitions.front().目标节点,r.Gread);
            if(definitionNode.种类!=节点种类::普通||definitionNode.属性类型表示)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitionValues=属性(definitionNode.编码,r.Gread);
            if(definitionValues.size()>r.预算.最大特征属性数)
                throw 纯失败{纯概念状态::数量预算不足};
            const auto pureKinds=std::count_if(definitionValues.begin(),definitionValues.end(),
                [&](const auto& value) {
                    return value.属性类型节点==pure_layout_->类型.定义种类;
                });
            if(pureKinds>1) throw 纯失败{纯概念状态::内部不一致};
            if(!pureKinds) {
                if(!secondary_relation_layout_)
                    throw 纯失败{纯概念状态::内部不一致};
                const auto rules=关系(m.目标节点,
                    secondary_relation_layout_->类型.规范化规则归属,false,
                    r.Gread,r.预算.最大关系数);
                if(rules.size()!=1||rules.front().角色或顺序!=1||
                   rules.front().目标节点!=secondary_relation_layout_->规范化规则.值)
                    throw 纯失败{纯概念状态::内部不一致};
                continue;
            }
            const auto f=读取纯概念内部(概念树概念身份{m.目标节点},r.Gread,r.预算);
            if(f.定义==expected) {
                if(found) throw 纯失败{纯概念状态::内部不一致};
                found=f;
            }
        }
        if(!found) {
            out.状态=纯概念状态::未找到;
            守卫代次(r.Gread); return out;
        }
        if(found->治理状态==概念树生命周期状态::退役)
            throw 纯失败{纯概念状态::概念已退役};
        out.事实=std::move(found);out.状态=纯概念状态::已读取;
        守卫代次(r.Gread);
    } catch(const 纯失败&e){out.状态=e.状态;out.事实.reset();}
      catch(const 失败&e){
        switch(e.状态) {
        case S::入口拒绝:out.状态=纯概念状态::入口拒绝;break;
        case S::未找到:out.状态=纯概念状态::未找到;break;
        case S::引用冲突:out.状态=纯概念状态::引用冲突;break;
        case S::事实代次漂移:out.状态=纯概念状态::事实代次漂移;break;
        case S::数量预算不足:out.状态=纯概念状态::数量预算不足;break;
        case S::资源失败:out.状态=纯概念状态::资源失败;break;
        case S::旧格式不支持:out.状态=纯概念状态::旧格式不支持;break;
        default:out.状态=纯概念状态::内部不一致;break;
        }
        out.事实.reset();
      } catch(const std::bad_alloc&){out.状态=纯概念状态::资源失败;out.事实.reset();}
      catch(const std::length_error&){out.状态=纯概念状态::资源失败;out.事实.reset();}
      catch(...){out.状态=纯概念状态::内部不一致;out.事实.reset();}
    return out;
}

纯概念查询结果_v3 概念树类数据服务::精确查询纯概念_v3(
    const 纯概念查询请求_v3 &r) const noexcept {
    纯概念查询结果_v3 out;
    out.Gread = r.Gread;
    try {
        if (r.版本 != 3 || !r.Gread)
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        const auto expected = 规范化纯概念定义完整(r.定义, r.Gread);
        const auto members = 完整关系组(
            pure_layout_->概念族锚点, pure_layout_->类型.概念族成员,
            false, r.Gread);
        std::optional<纯概念事实> found;
        for (const auto &member : members) {
            if (member.角色或顺序 != 1)
                throw 纯失败{纯概念状态::内部不一致};
            const auto conceptNode = 节点(member.目标节点, r.Gread);
            if (conceptNode.种类 != 节点种类::普通 ||
                conceptNode.属性类型表示)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitions = 完整关系组(
                member.目标节点, pure_layout_->类型.定义成员,
                false, r.Gread);
            if (definitions.size() != 1 || definitions.front().角色或顺序 != 1)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitionNode = 节点(
                definitions.front().目标节点, r.Gread);
            if (definitionNode.种类 != 节点种类::普通 ||
                definitionNode.属性类型表示)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitionValues = 完整属性值组(
                definitionNode.编码, r.Gread);
            const auto pureKinds = std::count_if(
                definitionValues.begin(), definitionValues.end(),
                [&](const auto &value) {
                    return value.属性类型节点 == pure_layout_->类型.定义种类;
                });
            if (pureKinds > 1)
                throw 纯失败{纯概念状态::内部不一致};
            if (!pureKinds) {
                if (!secondary_relation_layout_)
                    throw 纯失败{纯概念状态::内部不一致};
                const auto rules = 完整关系组(
                    member.目标节点,
                    secondary_relation_layout_->类型.规范化规则归属,
                    false, r.Gread);
                if (rules.size() != 1 || rules.front().角色或顺序 != 1 ||
                    rules.front().目标节点 !=
                        secondary_relation_layout_->规范化规则.值)
                    throw 纯失败{纯概念状态::内部不一致};
                continue;
            }
            if (two_group_definition_layout_) {
                const auto twoGroup = 完整关系组(
                    member.目标节点,
                    two_group_definition_layout_->类型.两组定义成员,
                    false, r.Gread);
                if (!twoGroup.empty()) {
                    if (twoGroup.size() != 1 ||
                        twoGroup.front().角色或顺序 != 1 ||
                        twoGroup.front().源节点 != member.目标节点)
                        throw 纯失败{纯概念状态::内部不一致};
                    continue;
                }
            }
            auto fact = 读取纯概念完整内部(
                概念树概念身份{member.目标节点}, r.Gread);
            if (fact.定义 == expected) {
                if (found) throw 纯失败{纯概念状态::内部不一致};
                found = std::move(fact);
            }
        }
        if (!found) {
            out.状态 = 纯概念状态::未找到;
            守卫代次(r.Gread);
            return out;
        }
        if (found->治理状态 == 概念树生命周期状态::退役)
            throw 纯失败{纯概念状态::概念已退役};
        out.事实 = std::move(found);
        out.状态 = 纯概念状态::已读取;
        守卫代次(r.Gread);
    } catch (const 纯失败 &failure) {
        out.状态 = failure.状态;
        out.事实.reset();
    } catch (const 失败 &failure) {
        switch (failure.状态) {
        case S::入口拒绝: out.状态 = 纯概念状态::入口拒绝; break;
        case S::未找到: out.状态 = 纯概念状态::未找到; break;
        case S::引用冲突: out.状态 = 纯概念状态::引用冲突; break;
        case S::事实代次漂移: out.状态 = 纯概念状态::事实代次漂移; break;
        case S::资源失败: out.状态 = 纯概念状态::资源失败; break;
        case S::旧格式不支持: out.状态 = 纯概念状态::旧格式不支持; break;
        default: out.状态 = 纯概念状态::内部不一致; break;
        }
        out.事实.reset();
    } catch (const std::bad_alloc &) {
        out.状态 = 纯概念状态::资源失败;
        out.事实.reset();
    } catch (const std::length_error &) {
        out.状态 = 纯概念状态::资源失败;
        out.事实.reset();
    } catch (...) {
        out.状态 = 纯概念状态::内部不一致;
        out.事实.reset();
    }
    return out;
}

I64特征概念组织读取结果 概念树类数据服务::读取当前I64特征概念(
    const I64特征概念组织读取请求 &r) const noexcept {
    I64特征概念组织读取结果 out;out.Gread=r.Gread;
    try {
        if(r.版本!=2||!r.Gread||!有效(r.FT.编码)||
           !r.预算.最大概念数||!r.预算.最大关系数||!r.预算.最大特征属性数)
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        const auto members=关系(pure_layout_->概念族锚点,pure_layout_->类型.概念族成员,
                                false,r.Gread,r.预算.最大概念数);
        std::uint64_t completeConcepts=0, completeRelations=0,
                      completeFeatureAttributes=0;
        const auto addWithin=[](std::uint64_t &total, std::uint64_t increment,
                                std::uint64_t limit) {
            if(increment>limit-total) throw 纯失败{纯概念状态::数量预算不足};
            total+=increment;
        };
        for(const auto& member:members) {
            if(member.角色或顺序!=1)throw 纯失败{纯概念状态::内部不一致};
            const auto conceptNode=节点(member.目标节点,r.Gread);
            if(conceptNode.种类!=节点种类::普通||conceptNode.属性类型表示)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitions=关系(member.目标节点,pure_layout_->类型.定义成员,
                                        false,r.Gread,r.预算.最大关系数);
            if(definitions.size()!=1||definitions.front().角色或顺序!=1)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitionNode=节点(definitions.front().目标节点,r.Gread);
            if(definitionNode.种类!=节点种类::普通||definitionNode.属性类型表示)
                throw 纯失败{纯概念状态::内部不一致};
            const auto definitionValues=属性(definitionNode.编码,r.Gread);
            if(definitionValues.size()>r.预算.最大特征属性数)
                throw 纯失败{纯概念状态::数量预算不足};
            const auto pureKinds=std::count_if(definitionValues.begin(),definitionValues.end(),
                [&](const auto& value) {
                    return value.属性类型节点==pure_layout_->类型.定义种类;
                });
            if(pureKinds>1)throw 纯失败{纯概念状态::内部不一致};
            if(!pureKinds) {
                if(!secondary_relation_layout_)
                    throw 纯失败{纯概念状态::内部不一致};
                const auto rules=关系(member.目标节点,
                    secondary_relation_layout_->类型.规范化规则归属,false,
                    r.Gread,r.预算.最大关系数);
                if(rules.size()!=1||rules.front().角色或顺序!=1||
                   rules.front().目标节点!=secondary_relation_layout_->规范化规则.值)
                    throw 纯失败{纯概念状态::内部不一致};
                continue;
            }
            auto fact=读取纯概念内部(概念树概念身份{member.目标节点},r.Gread,r.预算);
            const auto* definition=std::get_if<纯I64特征概念定义>(&fact.定义);
            if(fact.治理状态==概念树生命周期状态::活跃&&definition&&
               definition->特征类型.值==r.FT.编码) {
                // 完整事实包含概念族成员、定义关系和全部直接上位；纯 I64
                // 定义固定读取概念类别/生命周期及定义种类/I64 域四个属性。
                addWithin(completeConcepts,1,r.预算.最大概念数);
                addWithin(completeRelations,1+static_cast<std::uint64_t>(fact.定义关系组.size())+
                          static_cast<std::uint64_t>(fact.直接上位.size()),r.预算.最大关系数);
                addWithin(completeFeatureAttributes,4,r.预算.最大特征属性数);
                out.概念组.push_back(std::move(fact));
            }
        }
        std::sort(out.概念组.begin(),out.概念组.end(),[](const auto&a,const auto&b){return a.概念.值.值<b.概念.值.值;});
        if(std::adjacent_find(out.概念组.begin(),out.概念组.end(),[](const auto&a,const auto&b){return a.概念==b.概念;})!=out.概念组.end())
            throw 纯失败{纯概念状态::内部不一致};
        out.状态=纯概念状态::已读取;守卫代次(r.Gread);
    }catch(const 纯失败&e){out.状态=e.状态;out.概念组.clear();}
     catch(const 失败&e){
        switch(e.状态){case S::未找到:out.状态=纯概念状态::未找到;break;
        case S::事实代次漂移:out.状态=纯概念状态::事实代次漂移;break;case S::数量预算不足:out.状态=纯概念状态::数量预算不足;break;
        case S::资源失败:out.状态=纯概念状态::资源失败;break;
        default:out.状态=纯概念状态::内部不一致;break;}out.概念组.clear();}
     catch(const std::bad_alloc&){out.状态=纯概念状态::资源失败;out.概念组.clear();}
     catch(...){out.状态=纯概念状态::内部不一致;out.概念组.clear();}
    return out;
}

L1所有者范围写集请求 概念树类数据服务::形成纯概念写集(
    const 纯概念创建请求 &r, const 纯概念定义 &definition) const {
    return 形成纯概念写集(
        r.G0, r.幂等身份, r.直接上位, definition);
}

L1所有者范围写集请求 概念树类数据服务::形成纯概念写集(
    std::uint64_t g0, L1所有者范围写入幂等身份 idempotency,
    const std::vector<概念树概念身份> &directParents,
    const 纯概念定义 &definition) const {
    const auto &t=pure_layout_->类型;
    L1所有者范围写集请求 ws{
        L1所有者范围CRUD合同版本, g0, idempotency};
    ws.节点.push_back({{1},节点种类::普通,std::nullopt});
    ws.节点.push_back({{2},节点种类::普通,std::nullopt});
    std::uint32_t edgeKey=0x10001, valueKey=0x20001;
    auto edge=[&](Ref source,Ref target,稳定编码 type,std::int64_t role=1){
        ws.关系.push_back({{edgeKey++},source,target,Ref{type},role});
    };
    auto value=[&](Ref node,稳定编码 type,L1所有者范围原始值材料 material){
        const Key key{valueKey++};
        ws.值.push_back({key,node,Ref{type},std::move(material),node});
        ws.属性槽变更.push_back({node,Ref{type},key});
    };
    edge(Ref{pure_layout_->概念族锚点},Ref{Key{1}},t.概念族成员);
    edge(Ref{Key{1}},Ref{Key{2}},t.定义成员);
    const bool fc=std::holds_alternative<纯I64特征概念定义>(definition);
    value(Ref{Key{1}},t.概念类别,std::int64_t{fc?2:1});
    value(Ref{Key{1}},t.生命周期,
          std::int64_t{static_cast<std::uint8_t>(概念树生命周期状态::活跃)});
    if(const auto *d=std::get_if<纯I64特征概念定义>(&definition)) {
        value(Ref{Key{2}},t.定义种类,std::int64_t{1});
        edge(Ref{Key{2}},Ref{d->特征类型.值},t.定义特征类型);
        std::vector<std::uint64_t> raw;
        for(const auto &x:d->规范域){raw.push_back(std::bit_cast<std::uint64_t>(x.下界));
                                      raw.push_back(std::bit_cast<std::uint64_t>(x.上界));}
        value(Ref{Key{2}},t.I64域,std::move(raw));
    } else if(const auto *d=std::get_if<纯合取存在概念定义>(&definition)) {
        value(Ref{Key{2}},t.定义种类,std::int64_t{2});
        for(std::size_t i=0;i<d->特征模板组.size();++i)
            edge(Ref{Key{2}},Ref{d->特征模板组[i].值},t.定义模板,
                 static_cast<std::int64_t>(i+1));
    } else {
        value(Ref{Key{2}},t.定义种类,std::int64_t{3});
        value(Ref{Key{2}},t.通用规则,std::int64_t{1});
    }
    auto parents=directParents;
    std::sort(parents.begin(),parents.end(),[](auto a,auto b){return a.值.值<b.值.值;});
    for(const auto parent:parents)
        edge(Ref{parent.值},Ref{Key{1}},t.直接上位);
    return ws;
}

纯概念写入结果 概念树类数据服务::创建或复用纯概念(
    const 纯概念创建请求 &r) noexcept {
    纯概念写入结果 out;
    bool entered=false;
    try {
        out.原请求=r;
        std::scoped_lock lock(mutex_);
        if(r.版本!=2||!r.G0||r.G0==UINT64_MAX||!有效(r.幂等身份)||
           (r.组织!=概念初始组织指定::显式顶层&&
            r.组织!=概念初始组织指定::具名上位))
            throw 纯失败{纯概念状态::入口拒绝};
        const auto current=读取当前事实代次();
        if(!current.成功()) throw 纯失败{纯概念状态::资源失败};
        out.Gread=current.Gread;
        const auto definition=规范化纯概念定义(r.定义,current.Gread,r.预算);
        auto parents=r.直接上位;
        std::sort(parents.begin(),parents.end(),[](auto a,auto b){return a.值.值<b.值.值;});
        if(std::adjacent_find(parents.begin(),parents.end())!=parents.end()||
           (r.组织==概念初始组织指定::显式顶层&&!parents.empty())||
           (r.组织==概念初始组织指定::具名上位&&parents.empty()))
            throw 纯失败{纯概念状态::组织冲突};
        if(std::holds_alternative<通用存在概念定义>(definition)&&
           r.组织!=概念初始组织指定::显式顶层)
            throw 纯失败{纯概念状态::组织冲突};
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||
           first.所有者!=port_.所有者身份()||first.写入幂等身份!=r.幂等身份)
            throw 纯失败{纯概念状态::内部不一致};
        if(first.状态==L1所有者范围读取状态::成功) {
            if(!first.首次规范化写集)
                throw 纯失败{纯概念状态::内部不一致};
            if(first.首次规范化写集->期望事实代次!=r.G0)
                throw 纯失败{纯概念状态::幂等冲突};
            纯概念创建恢复请求 rr{2,first.读取事实代次,r.幂等身份,
                                     r.定义,r.组织,parents,r.预算};
            const auto restored=读取纯概念创建首次结果(rr);
            out.Gread=restored.Gread;out.首次发布H=restored.首次发布H;
            if(restored.成功(rr)) {
                out.事实=restored.事实;
                out.状态=纯概念状态::精确重复;out.发布=纯概念发布状态::确认发布;
            } else {
                out.状态=restored.状态==纯概念恢复状态::幂等冲突 ? 纯概念状态::幂等冲突 :
                    (restored.状态==纯概念恢复状态::已可能发布 ? 纯概念状态::已可能发布 :
                     (restored.状态==纯概念恢复状态::资源失败 ? 纯概念状态::资源失败 :
                      (restored.状态==纯概念恢复状态::未找到 ? 纯概念状态::未找到 :
                       (restored.状态==纯概念恢复状态::事实代次漂移 ? 纯概念状态::事实代次漂移 :
                        (restored.状态==纯概念恢复状态::数量预算不足 ? 纯概念状态::数量预算不足 :
                         纯概念状态::内部不一致)))));
                out.发布=restored.状态==纯概念恢复状态::已可能发布 ?
                    纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
            }
            return out;
        }
        if(first.状态!=L1所有者范围读取状态::未找到||first.读取事实代次!=r.G0) {
            if(first.状态==L1所有者范围读取状态::资源失败)
                throw 纯失败{纯概念状态::资源失败};
            throw 纯失败{纯概念状态::事实代次漂移};
        }
        if(out.Gread!=r.G0) throw 纯失败{纯概念状态::事实代次漂移};
        const auto found=精确查询纯概念({2,r.G0,definition,r.预算});
        if(found.成功({2,r.G0,definition,r.预算})) {
            auto actual=found.事实->直接上位;
            std::vector<概念树概念身份> actualIds;
            for(const auto &x:actual) actualIds.push_back(x.上位);
            std::sort(actualIds.begin(),actualIds.end(),[](auto a,auto b){return a.值.值<b.值.值;});
            if(actualIds!=parents) throw 纯失败{纯概念状态::组织冲突};
            out.状态=纯概念状态::精确重复;out.发布=纯概念发布状态::确认未发布;
            out.Gread=r.G0;out.事实=found.事实;return out;
        }
        if(!found.确认未找到({2,r.G0,definition,r.预算})) throw 纯失败{found.状态};
        const auto category=std::holds_alternative<纯I64特征概念定义>(definition) ?
            相关概念类别::特征 : 相关概念类别::存在;
        for(const auto parent:parents) {
            const auto pf=读取纯概念内部(parent,r.G0,r.预算);
            if(pf.类别!=category||pf.治理状态==概念树生命周期状态::退役)
                throw 纯失败{纯概念状态::类别冲突};
            if(const auto *child=std::get_if<纯I64特征概念定义>(&definition)) {
                const auto *up=std::get_if<纯I64特征概念定义>(&pf.定义);
                if(!up||up->特征类型!=child->特征类型)
                    throw 纯失败{纯概念状态::定义不相容};
                特征规范I64域 a,b;for(auto x:up->规范域)a.区间.push_back({x.下界,x.上界});
                for(auto x:child->规范域)b.区间.push_back({x.下界,x.上界});
                const auto contains=特征结果(features_.判定I64域包含(
                    {{1,r.G0,{child->特征类型.值}},a,b}));
                if(!contains.数据||a==b) throw 纯失败{纯概念状态::定义不相容};
            } else if(const auto *child=std::get_if<纯合取存在概念定义>(&definition)) {
                if(const auto *up=std::get_if<纯合取存在概念定义>(&pf.定义)) {
                    if(!std::includes(child->特征模板组.begin(),child->特征模板组.end(),
                                      up->特征模板组.begin(),up->特征模板组.end(),
                                      [](auto a,auto b){return a.值.值<b.值.值;})||*up==*child)
                        throw 纯失败{纯概念状态::定义不相容};
                } else if(!std::holds_alternative<通用存在概念定义>(pf.定义))
                    throw 纯失败{纯概念状态::定义不相容};
            }
        }
        auto ws=形成纯概念写集(r,definition);
        entered=true;
        const auto saved=port_.提交所有者范围中性写集(ws);
        out.Gread=saved.事实代次;
        if(saved.状态!=L1所有者范围写入状态::成功||
           saved.事实代次!=r.G0+1||saved.新编码映射.empty()) {
            out.状态=saved.状态==L1所有者范围写入状态::事实代次漂移 ?
                纯概念状态::事实代次漂移 : (saved.状态==L1所有者范围写入状态::幂等冲突 ?
                纯概念状态::幂等冲突 : 纯概念状态::已可能发布);
            out.发布=saved.是否形成内存权威发布 ? 纯概念发布状态::可能发布 :
                                                    纯概念发布状态::确认未发布;
            return out;
        }
        稳定编码 concept_id{};
        for(const auto &[key,id]:saved.新编码映射)if(key.值==1)concept_id=id;
        if(!有效(concept_id)) throw 纯失败{纯概念状态::已可能发布};
        out.首次发布H=saved.事实代次;
        const auto tail=读取当前事实代次();
        if(!tail.成功()||tail.Gread<saved.事实代次)throw 纯失败{纯概念状态::已可能发布};
        out.Gread=tail.Gread;
        out.事实=读取纯概念内部(概念树概念身份{concept_id},out.Gread,r.预算);
        out.状态=纯概念状态::已创建;out.发布=纯概念发布状态::确认发布;
    } catch(const 纯失败&e){
        out.状态=entered ? 纯概念状态::已可能发布 : e.状态;
        out.发布=entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;
        if(out.状态!=纯概念状态::已可能发布){out.首次发布H.reset();out.事实.reset();}
    } catch(const 失败&e) {
        switch(e.状态) {
        case S::入口拒绝:out.状态=纯概念状态::入口拒绝;break;
        case S::未找到:out.状态=纯概念状态::未找到;break;
        case S::引用冲突:out.状态=纯概念状态::引用冲突;break;
        case S::事实代次漂移:out.状态=纯概念状态::事实代次漂移;break;
        case S::幂等冲突:out.状态=纯概念状态::幂等冲突;break;
        case S::数量预算不足:out.状态=纯概念状态::数量预算不足;break;
        case S::资源失败:out.状态=纯概念状态::资源失败;break;
        case S::旧格式不支持:out.状态=纯概念状态::旧格式不支持;break;
        default:out.状态=纯概念状态::内部不一致;break;
        }
        if(entered)out.状态=纯概念状态::已可能发布;
        out.发布=entered ? 纯概念发布状态::可能发布
                          : 纯概念发布状态::确认未发布;
        if(out.状态!=纯概念状态::已可能发布) {
            out.首次发布H.reset();out.事实.reset();
        }
    } catch(const std::bad_alloc&){out.状态=entered ? 纯概念状态::已可能发布 : 纯概念状态::资源失败;
        out.发布=entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;out.事实.reset();}
      catch(...){out.状态=entered ? 纯概念状态::已可能发布 : 纯概念状态::内部不一致;
        out.发布=entered ? 纯概念发布状态::可能发布 : 纯概念发布状态::确认未发布;out.事实.reset();}
    return out;
}

纯概念创建恢复结果 概念树类数据服务::读取纯概念创建首次结果(
    const 纯概念创建恢复请求 &r) const noexcept {
    纯概念创建恢复结果 out;out.Gread=r.Gread;
    try {
        if(r.版本!=2||!r.Gread||!有效(r.幂等身份)||
           (r.组织!=概念初始组织指定::显式顶层&&
            r.组织!=概念初始组织指定::具名上位))
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.幂等身份});
        out.Gread=first.读取事实代次;
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||
           first.所有者!=port_.所有者身份()||first.写入幂等身份!=r.幂等身份)
            throw 纯失败{纯概念状态::内部不一致};
        if(first.状态==L1所有者范围读取状态::未找到) {
            if(first.读取事实代次!=r.Gread||first.首次规范化写集||first.首次写入结果)
                throw 纯失败{纯概念状态::内部不一致};
            守卫代次(r.Gread);
            out.首次实际G0.reset();out.首次发布H.reset();out.事实.reset();
            out.状态=纯概念恢复状态::未派发;return out;
        }
        if(first.状态==L1所有者范围读取状态::资源失败)
            throw 纯失败{纯概念状态::资源失败};
        if(first.状态!=L1所有者范围读取状态::成功||!first.首次规范化写集||
           !first.首次写入结果||first.首次写入结果->状态!=L1所有者范围写入状态::成功||
           !first.首次写入结果->是否形成内存权威发布)
            throw 纯失败{纯概念状态::已可能发布};
        const auto g0=first.首次规范化写集->期望事实代次;
        const auto firstH=first.首次写入结果->事实代次;
        if(first.读取事实代次!=r.Gread)
            throw 纯失败{纯概念状态::事实代次漂移};
        if(!g0||firstH!=g0+1||firstH>r.Gread)
            throw 纯失败{纯概念状态::内部不一致};
        auto parents=r.直接上位;
        std::sort(parents.begin(),parents.end(),[](auto a,auto b){return a.值.值<b.值.值;});
        if(std::adjacent_find(parents.begin(),parents.end())!=parents.end()||
           (r.组织==概念初始组织指定::显式顶层&&!parents.empty())||
           (r.组织==概念初始组织指定::具名上位&&parents.empty()))
            throw 纯失败{纯概念状态::组织冲突};
        const auto definition=规范化纯概念定义(r.定义,r.Gread,r.预算);
        纯概念创建请求 original{2,g0,r.幂等身份,definition,r.组织,parents,r.预算};
        const auto expected=形成纯概念写集(original,definition);
        if(expected!=*first.首次规范化写集)
            throw 纯失败{纯概念状态::幂等冲突};
        稳定编码 concept_id{};
        for(const auto &[key,id]:first.首次写入结果->新编码映射)
            if(key.值==1)concept_id=id;
        if(!有效(concept_id)) throw 纯失败{纯概念状态::内部不一致};
        out.事实=读取纯概念内部(概念树概念身份{concept_id},r.Gread,r.预算);
        if(out.事实->定义!=definition) throw 纯失败{纯概念状态::幂等冲突};
        out.首次实际G0=g0;out.首次发布H=firstH;out.状态=纯概念恢复状态::已读回;
        守卫代次(r.Gread);
    } catch(const 纯失败&e) {
        switch(e.状态){
        case 纯概念状态::入口拒绝:out.状态=纯概念恢复状态::入口拒绝;break;
        case 纯概念状态::幂等冲突:out.状态=纯概念恢复状态::幂等冲突;break;
        case 纯概念状态::事实代次漂移:out.状态=纯概念恢复状态::事实代次漂移;break;
        case 纯概念状态::数量预算不足:out.状态=纯概念恢复状态::数量预算不足;break;
        case 纯概念状态::已可能发布:out.状态=纯概念恢复状态::已可能发布;break;
        case 纯概念状态::资源失败:out.状态=纯概念恢复状态::资源失败;break;
        default:out.状态=纯概念恢复状态::内部不一致;break;}
        out.事实.reset();
    } catch(const 失败&e) {
        out.状态=纯概念恢复状态::内部不一致;
        if(e.状态==S::事实代次漂移) out.状态=纯概念恢复状态::事实代次漂移;
        else if(e.状态==S::数量预算不足) out.状态=纯概念恢复状态::数量预算不足;
        else if(e.状态==S::未找到) out.状态=纯概念恢复状态::未找到;
        else if(e.状态==S::资源失败) out.状态=纯概念恢复状态::资源失败;
        out.事实.reset();
    } catch(const std::bad_alloc&){out.状态=纯概念恢复状态::资源失败;out.事实.reset();}
      catch(...){out.状态=纯概念恢复状态::内部不一致;out.事实.reset();}
    return out;
}

纯概念写入结果_v3 概念树类数据服务::创建或复用纯概念_v3(
    const 纯概念创建请求_v3 &r) noexcept {
    纯概念写入结果_v3 out;
    bool entered = false;
    try {
        out.原请求 = r;
        std::scoped_lock lock(mutex_);
        if (r.版本 != 3 || !r.G0 || r.G0 == UINT64_MAX ||
            !有效(r.幂等身份) ||
            (r.组织 != 概念初始组织指定::显式顶层 &&
             r.组织 != 概念初始组织指定::具名上位))
            throw 纯失败{纯概念状态::入口拒绝};
        const auto current = 读取当前事实代次();
        if (!current.成功()) throw 纯失败{纯概念状态::资源失败};
        out.Gread = current.Gread;
        const auto definition =
            规范化纯概念定义完整(r.定义, current.Gread);
        auto parents = r.直接上位;
        std::sort(parents.begin(), parents.end(),
                  [](auto a, auto b) { return a.值.值 < b.值.值; });
        if (std::adjacent_find(parents.begin(), parents.end()) != parents.end() ||
            (r.组织 == 概念初始组织指定::显式顶层 && !parents.empty()) ||
            (r.组织 == 概念初始组织指定::具名上位 && parents.empty()))
            throw 纯失败{纯概念状态::组织冲突};
        if (std::holds_alternative<通用存在概念定义>(definition) &&
            r.组织 != 概念初始组织指定::显式顶层)
            throw 纯失败{纯概念状态::组织冲突};

        const auto first = port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, r.幂等身份});
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
            first.所有者 != port_.所有者身份() ||
            first.写入幂等身份 != r.幂等身份)
            throw 纯失败{纯概念状态::内部不一致};
        if (first.状态 == L1所有者范围读取状态::成功) {
            if (!first.首次规范化写集)
                throw 纯失败{纯概念状态::内部不一致};
            if (first.首次规范化写集->期望事实代次 != r.G0)
                throw 纯失败{纯概念状态::幂等冲突};
            const 纯概念创建恢复请求_v3 recover{
                3, first.读取事实代次, r.幂等身份,
                r.定义, r.组织, parents};
            const auto restored = 读取纯概念创建首次结果_v3(recover);
            out.Gread = restored.Gread;
            out.首次发布H = restored.首次发布H;
            if (restored.成功(recover)) {
                out.事实 = restored.事实;
                out.状态 = 纯概念状态::精确重复;
                out.发布 = 纯概念发布状态::确认发布;
            } else {
                switch (restored.状态) {
                case 纯概念恢复状态::幂等冲突:
                    out.状态 = 纯概念状态::幂等冲突; break;
                case 纯概念恢复状态::已可能发布:
                    out.状态 = 纯概念状态::已可能发布; break;
                case 纯概念恢复状态::资源失败:
                    out.状态 = 纯概念状态::资源失败; break;
                case 纯概念恢复状态::未找到:
                    out.状态 = 纯概念状态::未找到; break;
                case 纯概念恢复状态::事实代次漂移:
                    out.状态 = 纯概念状态::事实代次漂移; break;
                default:
                    out.状态 = 纯概念状态::内部不一致; break;
                }
                out.发布 = restored.状态 == 纯概念恢复状态::已可能发布
                                  ? 纯概念发布状态::可能发布
                                  : 纯概念发布状态::确认未发布;
                out.事实.reset();
            }
            return out;
        }
        if (first.状态 != L1所有者范围读取状态::未找到 ||
            first.读取事实代次 != r.G0) {
            if (first.状态 == L1所有者范围读取状态::资源失败)
                throw 纯失败{纯概念状态::资源失败};
            throw 纯失败{纯概念状态::事实代次漂移};
        }
        if (out.Gread != r.G0)
            throw 纯失败{纯概念状态::事实代次漂移};

        const 纯概念查询请求_v3 query{3, r.G0, definition};
        const auto found = 精确查询纯概念_v3(query);
        if (found.成功(query)) {
            std::vector<概念树概念身份> actualParents;
            actualParents.reserve(found.事实->直接上位.size());
            for (const auto &item : found.事实->直接上位)
                actualParents.push_back(item.上位);
            std::sort(actualParents.begin(), actualParents.end(),
                      [](auto a, auto b) { return a.值.值 < b.值.值; });
            if (actualParents != parents)
                throw 纯失败{纯概念状态::组织冲突};
            out.状态 = 纯概念状态::精确重复;
            out.发布 = 纯概念发布状态::确认未发布;
            out.Gread = r.G0;
            out.事实 = found.事实;
            return out;
        }
        if (!found.确认未找到(query))
            throw 纯失败{found.状态};

        const auto category =
            std::holds_alternative<纯I64特征概念定义>(definition)
                ? 相关概念类别::特征
                : 相关概念类别::存在;
        for (const auto parent : parents) {
            const auto parentFact = 读取纯概念完整内部(parent, r.G0);
            if (parentFact.类别 != category ||
                parentFact.治理状态 == 概念树生命周期状态::退役)
                throw 纯失败{纯概念状态::类别冲突};
            if (const auto *child =
                    std::get_if<纯I64特征概念定义>(&definition)) {
                const auto *upper =
                    std::get_if<纯I64特征概念定义>(&parentFact.定义);
                if (!upper || upper->特征类型 != child->特征类型)
                    throw 纯失败{纯概念状态::定义不相容};
                特征规范I64域 upperDomain, childDomain;
                for (const auto item : upper->规范域)
                    upperDomain.区间.push_back({item.下界, item.上界});
                for (const auto item : child->规范域)
                    childDomain.区间.push_back({item.下界, item.上界});
                const auto contains = 特征结果(features_.判定I64域包含(
                    {{1, r.G0, {child->特征类型.值}},
                     upperDomain, childDomain}));
                if (!contains.数据 || upperDomain == childDomain)
                    throw 纯失败{纯概念状态::定义不相容};
            } else if (const auto *child =
                           std::get_if<纯合取存在概念定义>(&definition)) {
                if (const auto *upper =
                        std::get_if<纯合取存在概念定义>(&parentFact.定义)) {
                    if (!std::includes(
                            child->特征模板组.begin(), child->特征模板组.end(),
                            upper->特征模板组.begin(), upper->特征模板组.end(),
                            [](auto a, auto b) { return a.值.值 < b.值.值; }) ||
                        *upper == *child)
                        throw 纯失败{纯概念状态::定义不相容};
                } else if (!std::holds_alternative<通用存在概念定义>(
                               parentFact.定义)) {
                    throw 纯失败{纯概念状态::定义不相容};
                }
            }
        }

        auto writeSet = 形成纯概念写集(
            r.G0, r.幂等身份, parents, definition);
        entered = true;
        const auto saved = port_.提交所有者范围中性写集(writeSet);
        out.Gread = saved.事实代次;
        if (saved.状态 != L1所有者范围写入状态::成功 ||
            saved.事实代次 != r.G0 + 1 || saved.新编码映射.empty()) {
            out.状态 = saved.状态 == L1所有者范围写入状态::事实代次漂移
                             ? 纯概念状态::事实代次漂移
                             : (saved.状态 == L1所有者范围写入状态::幂等冲突
                                    ? 纯概念状态::幂等冲突
                                    : 纯概念状态::已可能发布);
            out.发布 = saved.是否形成内存权威发布
                           ? 纯概念发布状态::可能发布
                           : 纯概念发布状态::确认未发布;
            return out;
        }
        稳定编码 conceptId{};
        for (const auto &[key, id] : saved.新编码映射)
            if (key.值 == 1) conceptId = id;
        if (!有效(conceptId))
            throw 纯失败{纯概念状态::已可能发布};
        out.首次发布H = saved.事实代次;
        const auto tail = 读取当前事实代次();
        if (!tail.成功() || tail.Gread < saved.事实代次)
            throw 纯失败{纯概念状态::已可能发布};
        out.Gread = tail.Gread;
        out.事实 = 读取纯概念完整内部(
            概念树概念身份{conceptId}, out.Gread);
        out.状态 = 纯概念状态::已创建;
        out.发布 = 纯概念发布状态::确认发布;
    } catch (const 纯失败 &failure) {
        out.状态 = entered ? 纯概念状态::已可能发布 : failure.状态;
        out.发布 = entered ? 纯概念发布状态::可能发布
                           : 纯概念发布状态::确认未发布;
        if (out.状态 != 纯概念状态::已可能发布) {
            out.首次发布H.reset();
        }
        out.事实.reset();
    } catch (const 失败 &failure) {
        switch (failure.状态) {
        case S::入口拒绝: out.状态 = 纯概念状态::入口拒绝; break;
        case S::未找到: out.状态 = 纯概念状态::未找到; break;
        case S::引用冲突: out.状态 = 纯概念状态::引用冲突; break;
        case S::事实代次漂移: out.状态 = 纯概念状态::事实代次漂移; break;
        case S::幂等冲突: out.状态 = 纯概念状态::幂等冲突; break;
        case S::资源失败: out.状态 = 纯概念状态::资源失败; break;
        case S::旧格式不支持: out.状态 = 纯概念状态::旧格式不支持; break;
        default: out.状态 = 纯概念状态::内部不一致; break;
        }
        if (entered) out.状态 = 纯概念状态::已可能发布;
        out.发布 = entered ? 纯概念发布状态::可能发布
                           : 纯概念发布状态::确认未发布;
        if (out.状态 != 纯概念状态::已可能发布) {
            out.首次发布H.reset();
        }
        out.事实.reset();
    } catch (const std::bad_alloc &) {
        out.状态 = entered ? 纯概念状态::已可能发布
                           : 纯概念状态::资源失败;
        out.发布 = entered ? 纯概念发布状态::可能发布
                           : 纯概念发布状态::确认未发布;
        out.事实.reset();
    } catch (const std::length_error &) {
        out.状态 = entered ? 纯概念状态::已可能发布
                           : 纯概念状态::资源失败;
        out.发布 = entered ? 纯概念发布状态::可能发布
                           : 纯概念发布状态::确认未发布;
        out.事实.reset();
    } catch (...) {
        out.状态 = entered ? 纯概念状态::已可能发布
                           : 纯概念状态::内部不一致;
        out.发布 = entered ? 纯概念发布状态::可能发布
                           : 纯概念发布状态::确认未发布;
        out.事实.reset();
    }
    return out;
}

纯概念创建恢复结果_v3
概念树类数据服务::读取纯概念创建首次结果_v3(
    const 纯概念创建恢复请求_v3 &r) const noexcept {
    纯概念创建恢复结果_v3 out;
    out.Gread = r.Gread;
    try {
        if (r.版本 != 3 || !r.Gread || !有效(r.幂等身份) ||
            (r.组织 != 概念初始组织指定::显式顶层 &&
             r.组织 != 概念初始组织指定::具名上位))
            throw 纯失败{纯概念状态::入口拒绝};
        守卫代次(r.Gread);
        const auto first = port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, r.幂等身份});
        out.Gread = first.读取事实代次;
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
            first.所有者 != port_.所有者身份() ||
            first.写入幂等身份 != r.幂等身份)
            throw 纯失败{纯概念状态::内部不一致};
        if (first.状态 == L1所有者范围读取状态::未找到) {
            if (first.读取事实代次 != r.Gread || first.首次规范化写集 ||
                first.首次写入结果)
                throw 纯失败{纯概念状态::内部不一致};
            守卫代次(r.Gread);
            out.状态 = 纯概念恢复状态::未派发;
            return out;
        }
        if (first.状态 == L1所有者范围读取状态::资源失败)
            throw 纯失败{纯概念状态::资源失败};
        if (first.状态 != L1所有者范围读取状态::成功 ||
            !first.首次规范化写集 || !first.首次写入结果 ||
            first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
            !first.首次写入结果->是否形成内存权威发布)
            throw 纯失败{纯概念状态::已可能发布};
        const auto g0 = first.首次规范化写集->期望事实代次;
        const auto firstH = first.首次写入结果->事实代次;
        if (first.读取事实代次 != r.Gread)
            throw 纯失败{纯概念状态::事实代次漂移};
        if (!g0 || firstH != g0 + 1 || firstH > r.Gread)
            throw 纯失败{纯概念状态::内部不一致};
        auto parents = r.直接上位;
        std::sort(parents.begin(), parents.end(),
                  [](auto a, auto b) { return a.值.值 < b.值.值; });
        if (std::adjacent_find(parents.begin(), parents.end()) != parents.end() ||
            (r.组织 == 概念初始组织指定::显式顶层 && !parents.empty()) ||
            (r.组织 == 概念初始组织指定::具名上位 && parents.empty()))
            throw 纯失败{纯概念状态::组织冲突};
        const auto definition =
            规范化纯概念定义完整(r.定义, r.Gread);
        const auto expected = 形成纯概念写集(
            g0, r.幂等身份, parents, definition);
        if (expected != *first.首次规范化写集)
            throw 纯失败{纯概念状态::幂等冲突};
        稳定编码 conceptId{};
        for (const auto &[key, id] : first.首次写入结果->新编码映射)
            if (key.值 == 1) conceptId = id;
        if (!有效(conceptId))
            throw 纯失败{纯概念状态::内部不一致};
        out.事实 = 读取纯概念完整内部(
            概念树概念身份{conceptId}, r.Gread);
        if (out.事实->定义 != definition ||
            !纯概念结果内部::父组匹配(*out.事实, parents))
            throw 纯失败{纯概念状态::幂等冲突};
        out.首次实际G0 = g0;
        out.首次发布H = firstH;
        out.状态 = 纯概念恢复状态::已读回;
        守卫代次(r.Gread);
    } catch (const 纯失败 &failure) {
        switch (failure.状态) {
        case 纯概念状态::入口拒绝:
            out.状态 = 纯概念恢复状态::入口拒绝; break;
        case 纯概念状态::幂等冲突:
        case 纯概念状态::组织冲突:
            out.状态 = 纯概念恢复状态::幂等冲突; break;
        case 纯概念状态::事实代次漂移:
            out.状态 = 纯概念恢复状态::事实代次漂移; break;
        case 纯概念状态::已可能发布:
            out.状态 = 纯概念恢复状态::已可能发布; break;
        case 纯概念状态::资源失败:
            out.状态 = 纯概念恢复状态::资源失败; break;
        default:
            out.状态 = 纯概念恢复状态::内部不一致; break;
        }
        out.事实.reset();
    } catch (const 失败 &failure) {
        out.状态 = 纯概念恢复状态::内部不一致;
        if (failure.状态 == S::事实代次漂移)
            out.状态 = 纯概念恢复状态::事实代次漂移;
        else if (failure.状态 == S::未找到)
            out.状态 = 纯概念恢复状态::未找到;
        else if (failure.状态 == S::资源失败)
            out.状态 = 纯概念恢复状态::资源失败;
        out.事实.reset();
    } catch (const std::bad_alloc &) {
        out.状态 = 纯概念恢复状态::资源失败;
        out.事实.reset();
    } catch (const std::length_error &) {
        out.状态 = 纯概念恢复状态::资源失败;
        out.事实.reset();
    } catch (...) {
        out.状态 = 纯概念恢复状态::内部不一致;
        out.事实.reset();
    }
    return out;
}

void 概念树类数据服务::核验纯概念无保护引用(
    const 纯概念事实&fact,std::uint64_t g,const 概念树预算&budget) const {
    if(!pure_layout_||!g||!budget.最大关系数||!budget.最大特征属性数||
       budget.最大关系数>UINT64_MAX-budget.最大特征属性数)
        throw 纯失败{纯概念状态::入口拒绝};
    const auto limit=budget.最大关系数+budget.最大特征属性数;
    std::vector<L1节点当前引用事实> allowed;
    auto add=[&](稳定编码 id) {
        const auto raw=原始事实(id,g);
        if(const auto*e=std::get_if<E>(&raw)) {
            if(e->写入所有者!=port_.所有者身份())throw 纯失败{纯概念状态::内部不一致};
            allowed.emplace_back(*e);
        } else if(const auto*v=std::get_if<V>(&raw)) {
            if(v->写入所有者!=port_.所有者身份())throw 纯失败{纯概念状态::内部不一致};
            allowed.emplace_back(*v);
        } else throw 纯失败{纯概念状态::内部不一致};
    };
    const auto member=关系(fact.概念.值,pure_layout_->类型.概念族成员,true,g,1);
    if(member.size()!=1||member.front().源节点!=pure_layout_->概念族锚点||
       member.front().目标节点!=fact.概念.值||member.front().角色或顺序!=1||
       member.front().创建事实代次!=fact.生命周期.创建事实代次)
        throw 纯失败{纯概念状态::内部不一致};
    add(member.front().编码);
    for(const auto&e:fact.定义关系组)add(e.关系);
    for(const auto&e:fact.直接上位)add(e.关系);
    for(const auto&v:属性(fact.概念.值,g))add(v.编码);
    for(const auto&v:属性(fact.定义记录,g))add(v.编码);

    auto mapFailure=[](L1节点当前完整引用读取状态_v2 s) {
        switch(s) {
        case L1节点当前完整引用读取状态_v2::入口拒绝:return 纯概念状态::入口拒绝;
        case L1节点当前完整引用读取状态_v2::未找到:return 纯概念状态::未找到;
        case L1节点当前完整引用读取状态_v2::事实代次漂移:return 纯概念状态::事实代次漂移;
        case L1节点当前完整引用读取状态_v2::资源失败:return 纯概念状态::资源失败;
        default:return 纯概念状态::内部不一致;
        }
    };
    std::vector<L1节点当前引用事实> actual;
    for(const auto node:{fact.概念.值,fact.定义记录}) {
        const L1节点当前完整引用读取请求_v2 request{
            L1节点当前完整引用读取合同版本,node,g};
        const auto read=l1_.读取节点全部当前引用_v2(request);
        if(read.状态!=L1节点当前完整引用读取状态_v2::成功
            ||read.合同版本!=request.合同版本||read.节点!=request.节点
            ||read.期望事实代次!=request.期望事实代次
            ||read.读取事实代次!=request.期望事实代次)
            throw 纯失败{mapFailure(read.状态)};
        for(const auto&item:read.引用) {
            auto code=[](const L1节点当前引用事实&x) {
                return std::visit([](const auto&v){return v.编码;},x);
            };
            const auto id=code(item);
            const auto found=std::find_if(actual.begin(),actual.end(),
                [&](const auto&x){return code(x)==id;});
            if(found!=actual.end()) {
                if(*found!=item)throw 纯失败{纯概念状态::内部不一致};
            } else actual.push_back(item);
        }
    }
    std::uint64_t relationCount=0,valueCount=0;
    for(const auto&item:actual) {
        if(std::holds_alternative<E>(item))++relationCount;else ++valueCount;
        if(relationCount>budget.最大关系数||valueCount>budget.最大特征属性数)
            throw 纯失败{纯概念状态::数量预算不足};
        if(std::find(allowed.begin(),allowed.end(),item)==allowed.end())
            throw 纯失败{纯概念状态::引用冲突};
    }
}

纯概念生命周期结果 概念树类数据服务::迁移纯概念生命周期(
    const 纯概念生命周期请求 &r) noexcept {
    纯概念生命周期结果 out; out.原请求=r;
    bool entered=false,knownPublished=false;
    try {
        if(!pure_layout_||r.版本!=2||!r.G0||r.G0==UINT64_MAX||
           !有效(r.幂等身份)||!有效(r.概念.值)||!有效(r.预期生命周期值事实)||
           !r.预算.最大概念数||!r.预算.最大关系数||!r.预算.最大特征属性数||
           r.预期治理状态==r.目标治理状态||
           !((r.预期治理状态==概念树生命周期状态::活跃&&
              r.目标治理状态==概念树生命周期状态::冷却)||
             (r.预期治理状态==概念树生命周期状态::冷却&&
              (r.目标治理状态==概念树生命周期状态::活跃||
               r.目标治理状态==概念树生命周期状态::退役)))) {
            if(!pure_layout_) throw 纯失败{纯概念状态::旧格式不支持};
            throw 纯失败{纯概念状态::入口拒绝};
        }

        const auto current=读取当前事实代次();
        if(!current.成功()) throw 纯失败{纯概念状态::资源失败};
        out.Gread=current.Gread;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||
           first.所有者!=port_.所有者身份()||first.写入幂等身份!=r.幂等身份)
            throw 纯失败{纯概念状态::内部不一致};
        bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay) {
            if(!first.首次规范化写集||!first.首次写入结果||
               first.首次写入结果->状态!=L1所有者范围写入状态::成功||
               !first.首次写入结果->是否形成内存权威发布||
               first.首次写入结果->事实代次!=r.G0+1)
                throw 纯失败{纯概念状态::已可能发布};
            knownPublished=true;out.首次发布H=first.首次写入结果->事实代次;
        } else if(first.状态==L1所有者范围读取状态::未找到) {
            if(first.首次规范化写集||first.首次写入结果)
                throw 纯失败{纯概念状态::内部不一致};
        } else if(first.状态==L1所有者范围读取状态::资源失败)
            throw 纯失败{纯概念状态::资源失败};
        else throw 纯失败{纯概念状态::内部不一致};
        const auto before=读取纯概念内部(r.概念,current.Gread,r.预算);
        if(!replay&&(before.治理状态!=r.预期治理状态||
           before.生命周期值事实!=r.预期生命周期值事实))
            throw 纯失败{纯概念状态::引用冲突};
        if(!replay&&r.目标治理状态==概念树生命周期状态::退役) {
            核验纯概念无保护引用(before,current.Gread,r.预算);
        }

        L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
        ws.退出事实.push_back(r.预期生命周期值事实);
        ws.值.push_back({{0x20001},Ref{r.概念.值},Ref{pure_layout_->类型.生命周期},
                         std::int64_t{static_cast<std::uint8_t>(r.目标治理状态)},Ref{r.概念.值}});
        ws.属性槽变更.push_back({Ref{r.概念.值},Ref{pure_layout_->类型.生命周期},Key{0x20001}});

        if(replay) {
            if(!first.首次规范化写集||*first.首次规范化写集!=ws)
                throw 纯失败{纯概念状态::幂等冲突};
        } else {
            if(current.Gread!=r.G0||first.读取事实代次!=r.G0)
                throw 纯失败{纯概念状态::事实代次漂移};
        }

        entered=true;
        const auto saved=port_.提交所有者范围中性写集(ws);
        out.Gread=saved.事实代次;
        if(saved.是否形成内存权威发布&&saved.事实代次) {
            knownPublished=true;
            out.首次发布H=saved.事实代次;
        }
        if(saved.状态!=(replay?L1所有者范围写入状态::精确重复:
                                 L1所有者范围写入状态::成功)||
           saved.事实代次!=r.G0+1) {
            out.状态=纯概念状态::已可能发布;
            if(saved.状态==L1所有者范围写入状态::事实代次漂移)
                out.状态=纯概念状态::事实代次漂移;
            else if(saved.状态==L1所有者范围写入状态::幂等冲突)
                out.状态=纯概念状态::幂等冲突;
            else if(saved.状态==L1所有者范围写入状态::引用冲突)
                out.状态=纯概念状态::引用冲突;
            out.发布=knownPublished?纯概念发布状态::确认发布:
                (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
            return out;
        }
        const auto tail=读取当前事实代次();
        if(!tail.成功()||tail.Gread<saved.事实代次)
            throw 纯失败{纯概念状态::已可能发布};
        knownPublished=true;
        out.Gread=tail.Gread;out.首次发布H=saved.事实代次;
        out.事实=读取纯概念内部(r.概念,out.Gread,r.预算);
        if(out.事实->治理状态!=r.目标治理状态||
           out.事实->生命周期值事实==r.预期生命周期值事实||
           out.事实->生命周期值生命周期.创建事实代次!=saved.事实代次||
           out.事实->定义!=before.定义||out.事实->生命周期.创建事实代次!=before.生命周期.创建事实代次)
            throw 纯失败{纯概念状态::内部不一致};
        out.状态=replay?纯概念状态::精确重复:纯概念状态::已迁移生命周期;
        out.发布=纯概念发布状态::确认发布;
    } catch(const 纯失败&e) {
        out.状态=entered?纯概念状态::已可能发布:e.状态;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
        out.事实.reset();
    } catch(const 失败&e) {
        switch(e.状态) {
        case S::入口拒绝:out.状态=纯概念状态::入口拒绝;break;
        case S::未找到:out.状态=纯概念状态::未找到;break;
        case S::引用冲突:out.状态=纯概念状态::引用冲突;break;
        case S::事实代次漂移:out.状态=纯概念状态::事实代次漂移;break;
        case S::幂等冲突:out.状态=纯概念状态::幂等冲突;break;
        case S::数量预算不足:out.状态=纯概念状态::数量预算不足;break;
        case S::资源失败:out.状态=纯概念状态::资源失败;break;
        case S::旧格式不支持:out.状态=纯概念状态::旧格式不支持;break;
        default:out.状态=纯概念状态::内部不一致;break;
        }
        if(entered&&!knownPublished)out.状态=纯概念状态::已可能发布;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
        out.事实.reset();
    } catch(const std::bad_alloc&) {
        out.状态=entered?纯概念状态::已可能发布:纯概念状态::资源失败;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
        out.事实.reset();
    } catch(...) {
        out.状态=entered?纯概念状态::已可能发布:纯概念状态::内部不一致;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
        out.事实.reset();
    }
    return out;
}

纯概念退出结果 概念树类数据服务::退出无引用叶概念(
    const 纯概念退出请求 &r) noexcept {
    纯概念退出结果 out;out.原请求=r;
    bool entered=false,knownPublished=false;
    try {
        if(!pure_layout_||r.版本!=2||!r.G0||r.G0==UINT64_MAX||
           !有效(r.幂等身份)||!有效(r.概念.值)||!有效(r.预期生命周期值事实)||
           !r.预算.最大概念数||!r.预算.最大关系数||!r.预算.最大特征属性数) {
            if(!pure_layout_) throw 纯失败{纯概念状态::旧格式不支持};
            throw 纯失败{纯概念状态::入口拒绝};
        }
        const auto current=读取当前事实代次();
        if(!current.成功())throw 纯失败{纯概念状态::资源失败};
        out.Gread=current.Gread;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||
           first.所有者!=port_.所有者身份()||first.写入幂等身份!=r.幂等身份)
            throw 纯失败{纯概念状态::内部不一致};
        bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay) {
            if(!first.首次规范化写集||!first.首次写入结果||
               first.首次写入结果->状态!=L1所有者范围写入状态::成功||
               !first.首次写入结果->是否形成内存权威发布||
               first.首次写入结果->事实代次!=r.G0+1)
                throw 纯失败{纯概念状态::已可能发布};
            knownPublished=true;
            out.首次发布H=first.首次写入结果->事实代次;
        } else if(first.状态==L1所有者范围读取状态::未找到) {
            if(first.首次规范化写集||first.首次写入结果)
                throw 纯失败{纯概念状态::内部不一致};
        } else if(first.状态==L1所有者范围读取状态::资源失败)
            throw 纯失败{纯概念状态::资源失败};
        else throw 纯失败{纯概念状态::内部不一致};

        L1所有者范围写集请求 ws;
        if(replay) {
            ws=*first.首次规范化写集;
            if(ws.合同版本!=L1所有者范围CRUD合同版本||
               ws.期望事实代次!=r.G0||ws.写入幂等身份!=r.幂等身份||
               !std::binary_search(ws.退出事实.begin(),ws.退出事实.end(),r.概念.值,编码小于)||
               !std::binary_search(ws.退出事实.begin(),ws.退出事实.end(),r.预期生命周期值事实,编码小于))
                throw 纯失败{纯概念状态::幂等冲突};
            const auto currentConcept=读取任一当前事实(l1_,r.概念.值);
            if(currentConcept.读取事实代次!=current.Gread)
                throw 纯失败{纯概念状态::事实代次漂移};
            if(currentConcept.状态!=L1所有者范围读取状态::未找到)
                throw 纯失败{纯概念状态::内部不一致};
        } else {
            if(current.Gread!=r.G0||first.读取事实代次!=r.G0)
                throw 纯失败{纯概念状态::事实代次漂移};
            const auto before=读取纯概念内部(r.概念,current.Gread,r.预算);
            if(before.治理状态!=概念树生命周期状态::退役||
               before.生命周期值事实!=r.预期生命周期值事实)
                throw 纯失败{纯概念状态::引用冲突};
            核验纯概念无保护引用(before,current.Gread,r.预算);
            ws={L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
            ws.退出事实.push_back(r.概念.值);
            ws.退出事实.push_back(before.定义记录);
            for(const auto &x:before.定义关系组)ws.退出事实.push_back(x.关系);
            for(const auto &x:before.直接上位)ws.退出事实.push_back(x.关系);
            for(const auto &x:属性(r.概念.值,current.Gread))ws.退出事实.push_back(x.编码);
            for(const auto &x:属性(before.定义记录,current.Gread))ws.退出事实.push_back(x.编码);
            const auto member=关系(r.概念.值,pure_layout_->类型.概念族成员,
                                   true,current.Gread,1);
            if(member.size()!=1)throw 纯失败{纯概念状态::内部不一致};
            ws.退出事实.push_back(member.front().编码);
            std::sort(ws.退出事实.begin(),ws.退出事实.end(),编码小于);
            ws.退出事实.erase(std::unique(ws.退出事实.begin(),ws.退出事实.end()),ws.退出事实.end());
        }

        entered=true;
        const auto saved=port_.提交所有者范围中性写集(ws);
        out.Gread=saved.事实代次;
        if(saved.是否形成内存权威发布&&saved.事实代次) {
            knownPublished=true;
            out.首次发布H=saved.事实代次;
        }
        if(saved.状态!=(replay?L1所有者范围写入状态::精确重复:
                                 L1所有者范围写入状态::成功)||saved.事实代次!=r.G0+1) {
            out.状态=纯概念状态::已可能发布;
            if(saved.状态==L1所有者范围写入状态::事实代次漂移)
                out.状态=纯概念状态::事实代次漂移;
            else if(saved.状态==L1所有者范围写入状态::幂等冲突)
                out.状态=纯概念状态::幂等冲突;
            else if(saved.状态==L1所有者范围写入状态::引用冲突)
                out.状态=纯概念状态::引用冲突;
            out.发布=knownPublished?纯概念发布状态::确认发布:
                (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
            return out;
        }
        const auto tail=读取当前事实代次();
        if(!tail.成功()||tail.Gread<saved.事实代次)
            throw 纯失败{纯概念状态::已可能发布};
        knownPublished=true;
        out.Gread=tail.Gread;out.首次发布H=saved.事实代次;
        const auto currentConcept=读取任一当前事实(l1_,r.概念.值);
        if(currentConcept.读取事实代次!=out.Gread||
           currentConcept.状态!=L1所有者范围读取状态::未找到)
            throw 纯失败{纯概念状态::内部不一致};
        out.状态=replay?纯概念状态::精确重复:纯概念状态::已删除;
        out.发布=纯概念发布状态::确认发布;
    } catch(const 纯失败&e) {
        out.状态=entered?纯概念状态::已可能发布:e.状态;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
    } catch(const 失败&e) {
        switch(e.状态) {
        case S::入口拒绝:out.状态=纯概念状态::入口拒绝;break;
        case S::未找到:out.状态=纯概念状态::未找到;break;
        case S::引用冲突:out.状态=纯概念状态::引用冲突;break;
        case S::事实代次漂移:out.状态=纯概念状态::事实代次漂移;break;
        case S::幂等冲突:out.状态=纯概念状态::幂等冲突;break;
        case S::数量预算不足:out.状态=纯概念状态::数量预算不足;break;
        case S::资源失败:out.状态=纯概念状态::资源失败;break;
        case S::旧格式不支持:out.状态=纯概念状态::旧格式不支持;break;
        default:out.状态=纯概念状态::内部不一致;break;
        }
        if(entered&&!knownPublished)out.状态=纯概念状态::已可能发布;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
    } catch(const std::bad_alloc&) {
        out.状态=entered?纯概念状态::已可能发布:纯概念状态::资源失败;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
    } catch(...) {
        out.状态=entered?纯概念状态::已可能发布:纯概念状态::内部不一致;
        out.发布=knownPublished?纯概念发布状态::确认发布:
            (entered?纯概念发布状态::可能发布:纯概念发布状态::确认未发布);
    }
    return out;
}

存在概念使用读取结果 概念树类数据服务::读取存在概念使用(
    const 存在概念使用读取请求 &r) const noexcept {
    存在概念使用读取结果 out; out.Gread=r.Gread;
    try {
        if(!pure_layout_||r.版本!=2||!r.Gread||
           !有效(r.E.值)||!r.预算.最大关系数||!r.预算.最大概念数)
            throw 纯失败{pure_layout_ ? 纯概念状态::入口拒绝 : 纯概念状态::旧格式不支持};
        核验外部节点(r.E.值,r.Gread);
        const auto edges=关系(r.E.值,pure_layout_->类型.存在概念使用,false,
                              r.Gread,r.预算.最大关系数);
        if(edges.empty()) throw 纯失败{纯概念状态::未找到};
        if(edges.size()!=1||edges.front().角色或顺序!=1)
            throw 纯失败{纯概念状态::引用冲突};
        const auto &edge=edges.front();
        const auto fact=读取纯概念内部(概念树概念身份{edge.目标节点},r.Gread,r.预算);
        if(fact.类别!=相关概念类别::存在)
            throw 纯失败{纯概念状态::类别冲突};
        存在概念使用事实 use;
        use.关系=edge.编码; use.E=r.E.值;
        use.EC=概念树概念身份{edge.目标节点};
        use.生命周期={edge.创建事实代次};
        out.使用=use;
        out.概念=fact; out.状态=纯概念状态::已读取;
        守卫代次(r.Gread);
    } catch(const 纯失败&e){out.状态=e.状态;out.使用.reset();out.概念.reset();}
      catch(const 失败&e){out.状态=e.状态==S::事实代次漂移 ? 纯概念状态::事实代次漂移 :
          e.状态==S::数量预算不足 ? 纯概念状态::数量预算不足 :
          e.状态==S::未找到 ? 纯概念状态::未找到 : 纯概念状态::内部不一致;
          out.使用.reset();out.概念.reset();}
      catch(const std::bad_alloc&){out.状态=纯概念状态::资源失败;out.使用.reset();out.概念.reset();}
      catch(...){out.状态=纯概念状态::内部不一致;out.使用.reset();out.概念.reset();}
    return out;
}

已发布概念引用片段结果 概念树类数据服务::准备存在概念引用片段(
    const 已发布存在概念引用准备请求 &r,std::uint64_t g) const noexcept {
    已发布概念引用片段结果 out; out.Gread=g;
    try {
        if(!pure_layout_||r.版本!=2||!g||r.G0!=g||!有效(r.幂等身份)||
           r.参与者.值<=1||r.新存在.参与者.值!=1||r.新存在.本地键.值!=1||
           !有效(r.EC.值)||!r.预算.最大关系数)
            throw 纯失败{pure_layout_ ? 纯概念状态::入口拒绝 : 纯概念状态::旧格式不支持};
        const auto definition=规范化纯概念定义(r.预期定义,g,r.预算);
        const auto fact=读取纯概念内部(r.EC,g,r.预算);
        if(fact.类别!=相关概念类别::存在)throw 纯失败{纯概念状态::类别冲突};
        if(fact.定义!=definition)throw 纯失败{纯概念状态::定义不相容};
        if(fact.治理状态!=概念树生命周期状态::活跃&&
           fact.治理状态!=概念树生命周期状态::冷却)
            throw 纯失败{纯概念状态::概念已退役};
        L1有限N分区原子参与者写集_v3 p;
        p.参与者=r.参与者;p.所有者=port_.所有者身份();
        p.写集.合同版本=L1所有者范围CRUD合同版本;
        p.写集.期望事实代次=r.G0;p.写集.写入幂等身份=r.幂等身份;
        p.写集.关系.push_back({{1},r.新存在,r.EC.值,pure_layout_->类型.存在概念使用,1});
        out.写集=std::move(p);out.状态=已发布概念引用参与状态::已准备;
        守卫代次(g);
    } catch(const 纯失败&e){
        switch(e.状态){
        case 纯概念状态::未找到:out.状态=已发布概念引用参与状态::概念未找到;break;
        case 纯概念状态::概念已退役:out.状态=已发布概念引用参与状态::概念已退役;break;
        case 纯概念状态::类别冲突:out.状态=已发布概念引用参与状态::类别冲突;break;
        case 纯概念状态::定义不相容:out.状态=已发布概念引用参与状态::定义不相容;break;
        case 纯概念状态::事实代次漂移:out.状态=已发布概念引用参与状态::事实代次漂移;break;
        case 纯概念状态::数量预算不足:out.状态=已发布概念引用参与状态::数量预算不足;break;
        case 纯概念状态::资源失败:out.状态=已发布概念引用参与状态::资源失败;break;
        case 纯概念状态::旧格式不支持:out.状态=已发布概念引用参与状态::旧格式不支持;break;
        default:out.状态=已发布概念引用参与状态::入口拒绝;break;}
        out.写集.reset();
    } catch(const 失败&e){out.状态=e.状态==S::事实代次漂移 ? 已发布概念引用参与状态::事实代次漂移 :
        e.状态==S::数量预算不足 ? 已发布概念引用参与状态::数量预算不足 :
        已发布概念引用参与状态::内部不一致;out.写集.reset();}
      catch(const std::bad_alloc&){out.状态=已发布概念引用参与状态::资源失败;out.写集.reset();}
      catch(...){out.状态=已发布概念引用参与状态::内部不一致;out.写集.reset();}
    return out;
}

已发布概念引用首次材料结果 概念树类数据服务::读取存在概念引用首次材料(
    L1所有者范围写入幂等身份 key) const noexcept {
    已发布概念引用首次材料结果 out;
    try {
        if(!pure_layout_||!有效(key)){out.状态=pure_layout_ ? 已发布概念引用参与状态::入口拒绝 :
            已发布概念引用参与状态::旧格式不支持;return out;}
        auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,key});
        out.Gread=first.读取事实代次;
        if(first.状态==L1所有者范围读取状态::未找到){out.状态=已发布概念引用参与状态::未派发;return out;}
        if(first.状态==L1所有者范围读取状态::资源失败){out.状态=已发布概念引用参与状态::资源失败;return out;}
        if(first.状态!=L1所有者范围读取状态::成功||!first.首次规范化写集||!first.首次写入结果){out.状态=已发布概念引用参与状态::内部不一致;return out;}
        out.首次材料=std::move(first);out.状态=已发布概念引用参与状态::已读取;
    } catch(const std::bad_alloc&){out.状态=已发布概念引用参与状态::资源失败;out.首次材料.reset();}
      catch(...){out.状态=已发布概念引用参与状态::内部不一致;out.首次材料.reset();}
    return out;
}

const L1事实基座服务& 概念树类数据服务::原子I64底座() const noexcept { return l1_; }
L1所有者范围写端口& 概念树类数据服务::原子I64端口() noexcept { return port_; }
bool 概念树类数据服务::原子I64结构已就绪() const noexcept {
    return pure_layout_.has_value() && feature_birth_layout_.has_value() && port_.有效() && port_.绑定于(l1_);
}

原子I64特征出生使用读取结果 概念树类数据服务::读取原子I64出生使用(
    const 原子I64特征出生使用读取请求& r) const {
    原子I64特征出生使用读取结果 out;out.Gread=r.Gread;
    auto map=[&](纯概念状态 s) {
        using A=原子I64特征出生使用读取状态;
        switch(s) { case 纯概念状态::未找到:
        case 纯概念状态::概念已退役:return A::未找到;case 纯概念状态::事实代次漂移:return A::事实代次漂移;
        case 纯概念状态::数量预算不足:return A::预算不足;
        case 纯概念状态::资源失败:return A::资源失败;default:return A::内部不一致; }
    };
    try {
        if(!原子I64结构已就绪()||r.版本!=1||!r.Gread||!有效(r.F)||
           !r.概念读取预算.最大概念数||!r.概念读取预算.最大关系数||!r.概念读取预算.最大特征属性数) {
            out.状态=原子I64特征出生使用读取状态::入口拒绝;return out;
        }
        const auto guard=[&] {
            const auto current=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
            if(current.状态!=L1中性读取状态::成功||current.事实代次!=r.Gread)
                throw 原子I64特征出生使用读取状态::事实代次漂移;
        };
        guard();
        const auto edges=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,
            r.F,feature_birth_layout_->F到FCv出生使用关系类型});
        if(edges.状态!=L1所有者范围读取状态::成功||edges.读取事实代次!=r.Gread) {
            out.状态=edges.状态==L1所有者范围读取状态::事实代次漂移 ? 原子I64特征出生使用读取状态::事实代次漂移 :
                (edges.状态==L1所有者范围读取状态::资源失败 ? 原子I64特征出生使用读取状态::资源失败 :
                 原子I64特征出生使用读取状态::内部不一致);return out;
        }
        if(edges.关系组.empty()) {out.状态=原子I64特征出生使用读取状态::未找到;return out;}
        if(edges.关系组.size()!=1) {out.状态=原子I64特征出生使用读取状态::内部不一致;return out;}
        const auto& edge=edges.关系组.front();
        if(edge.写入所有者!=port_.所有者身份()||edge.源节点!=r.F||edge.关系类型节点!=feature_birth_layout_->F到FCv出生使用关系类型||edge.角色或顺序!=1||edge.创建事实代次>r.Gread) {
            out.状态=原子I64特征出生使用读取状态::内部不一致;return out;
        }
        const auto fc=读取纯概念({2,r.Gread,概念树概念身份{edge.目标节点},
            {r.概念读取预算.最大概念数,r.概念读取预算.最大关系数,0,0,0,r.概念读取预算.最大特征属性数,0,0}});
        if(fc.状态!=纯概念状态::已读取||!fc.事实) {out.状态=map(fc.状态);return out;}
        const auto* definition=std::get_if<纯I64特征概念定义>(&fc.事实->定义);
        if(fc.Gread!=r.Gread||fc.事实->类别!=相关概念类别::特征||!definition||
           fc.事实->治理状态!=概念树生命周期状态::活跃||definition->规范域.size()!=1) {
            out.状态=原子I64特征出生使用读取状态::概念不适配;return out;
        }
        guard();
        out.事实=特征概念出生使用事实{edge.编码,特征信息身份{r.F},概念树概念身份{edge.目标节点},
            {edge.创建事实代次}};
        out.状态=原子I64特征出生使用读取状态::已读取;
    } catch(const std::bad_alloc&) {out.状态=原子I64特征出生使用读取状态::资源失败;out.事实.reset();}
      catch(...) {out.状态=原子I64特征出生使用读取状态::内部不一致;out.事实.reset();}
    return out;
}

原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
概念树类数据服务::准备原子I64出生片段(const 原子I64特征出生请求& r,std::uint64_t g) const {
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3> out;out.Gread=g;
    try {
        if(!原子I64结构已就绪()||r.版本!=1||!r.G0||g!=r.G0||!有效(r.FCv.值)||!有效(r.正式特征类型)||
           !有效(r.键.概念使用)||!r.概念读取预算.最大概念数||!r.概念读取预算.最大关系数||!r.概念读取预算.最大特征属性数)
            throw 原子I64特征出生状态::入口拒绝;
        const auto fc=读取纯概念({2,g,r.FCv,{r.概念读取预算.最大概念数,r.概念读取预算.最大关系数,0,0,0,r.概念读取预算.最大特征属性数,0,0}});
        if(fc.状态!=纯概念状态::已读取||!fc.事实) {
            switch(fc.状态) {case 纯概念状态::未找到:case 纯概念状态::概念已退役:throw 原子I64特征出生状态::未找到;case 纯概念状态::事实代次漂移:throw 原子I64特征出生状态::事实代次漂移;case 纯概念状态::数量预算不足:throw 原子I64特征出生状态::预算不足;case 纯概念状态::资源失败:throw 原子I64特征出生状态::资源失败;default:throw 原子I64特征出生状态::内部不一致;}
        }
        const auto* d=std::get_if<纯I64特征概念定义>(&fc.事实->定义);
        if(fc.Gread!=g||fc.事实->类别!=相关概念类别::特征||fc.事实->治理状态!=概念树生命周期状态::活跃||!d||d->特征类型.值!=r.正式特征类型||d->规范域.size()!=1||d->规范域[0].下界!=r.准确I64||d->规范域[0].上界!=r.准确I64)
            throw 原子I64特征出生状态::概念不适配;
        L1有限N分区原子参与者写集_v3 part;part.参与者={4};part.所有者=port_.所有者身份();part.写集={L1所有者范围CRUD合同版本,r.G0,r.键.概念使用};
        part.写集.关系.push_back({{1},L1有限N分区原子事实引用_v3{{1},{1}},r.FCv.值,feature_birth_layout_->F到FCv出生使用关系类型,1});
        out.数据=std::move(part);out.状态=原子I64特征出生状态::已创建;
    } catch(原子I64特征出生状态 s) {out.状态=s;out.数据.reset();}
      catch(const std::bad_alloc&) {out.状态=原子I64特征出生状态::资源失败;out.数据.reset();}
      catch(...) {out.状态=原子I64特征出生状态::内部不一致;out.数据.reset();}
    return out;
}

原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
概念树类数据服务::准备原子I64出生使用退出片段(const 原子I64特征出生使用退出片段请求& r,std::uint64_t g) const {
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3> out;out.Gread=g;
    if(r.版本!=1||!r.G0||g!=r.G0||!有效(r.F)||!有效(r.概念使用退出键)) return out;
    const auto use=读取原子I64出生使用({1,g,r.F,r.概念读取预算});
    if(use.状态!=原子I64特征出生使用读取状态::已读取||!use.事实) return out;
    L1有限N分区原子参与者写集_v3 part;part.参与者={4};part.所有者=port_.所有者身份();part.写集={L1所有者范围CRUD合同版本,r.G0,r.概念使用退出键};part.写集.退出事实.push_back(use.事实->关系);
    out.数据=std::move(part);out.状态=原子I64特征出生状态::已创建;return out;
}

bool 原子I64特征出生使用读取结果::成功(const 原子I64特征出生使用读取请求& r) const noexcept {
    return 版本==1&&r.版本==1&&状态==原子I64特征出生使用读取状态::已读取&&Gread==r.Gread&&事实.has_value()&&事实->F.编码==r.F;
}

namespace {
bool 约束小于(const 二次关系概念约束&a,const 二次关系概念约束&b) noexcept {
    if(a.角色!=b.角色)return static_cast<unsigned>(a.角色)<static_cast<unsigned>(b.角色);
    return a.概念.值.值<b.概念.值.值;
}
void 二次规范化写集(L1所有者范围写集请求& writeSet) {
    const auto referenceKey=[](const L1所有者范围事实引用& reference) {
        return std::visit([](const auto& value)->std::uint64_t {
            using T=std::decay_t<decltype(value)>;
            if constexpr(std::is_same_v<T,稳定编码>)return value.值;
            else return (1ULL<<63)|value.值;
        },reference);
    };
    std::sort(writeSet.节点.begin(),writeSet.节点.end(),
        [](const auto&a,const auto&b){return a.本地键<b.本地键;});
    std::sort(writeSet.关系.begin(),writeSet.关系.end(),
        [](const auto&a,const auto&b){return a.本地键<b.本地键;});
    std::sort(writeSet.值.begin(),writeSet.值.end(),
        [](const auto&a,const auto&b){return a.本地键<b.本地键;});
    std::sort(writeSet.属性槽变更.begin(),writeSet.属性槽变更.end(),
        [&](const auto&a,const auto&b) {
            const auto ao=referenceKey(a.所属节点),bo=referenceKey(b.所属节点);
            if(ao!=bo)return ao<bo;
            const auto at=referenceKey(a.属性类型节点),bt=referenceKey(b.属性类型节点);
            if(at!=bt)return at<bt;
            return a.新当前值<b.新当前值;
        });
    std::sort(writeSet.退出事实.begin(),writeSet.退出事实.end());
}
bool 约束组小于(const std::vector<二次关系概念约束>&a,
                const std::vector<二次关系概念约束>&b) noexcept {
    if(a.size()!=b.size())return a.size()<b.size();
    return std::lexicographical_compare(a.begin(),a.end(),b.begin(),b.end(),约束小于);
}
void 规范约束组(二次关系约束组& q) {
    const auto one=[](std::vector<二次关系概念约束>& v) {
        for(const auto&x:v)if(!有效(x.概念.值)||static_cast<unsigned>(x.角色)<1||static_cast<unsigned>(x.角色)>3)
            throw 二次关系数据状态::入口拒绝;
        std::sort(v.begin(),v.end(),约束小于);v.erase(std::unique(v.begin(),v.end()),v.end());
        std::vector<概念树概念身份> common,a,b;
        for(const auto&x:v) {
            if(x.角色==二次关系约束角色::共同){common.push_back(x.概念);a.push_back(x.概念);b.push_back(x.概念);}
            else if(x.角色==二次关系约束角色::A)a.push_back(x.概念);else b.push_back(x.概念);
        }
        const auto sort=[](auto&x){std::sort(x.begin(),x.end(),[](auto p,auto q){return p.值.值<q.值.值;});x.erase(std::unique(x.begin(),x.end()),x.end());};
        sort(a);sort(b);std::vector<概念树概念身份> both;std::set_intersection(a.begin(),a.end(),b.begin(),b.end(),std::back_inserter(both),[](auto p,auto q){return p.值.值<q.值.值;});
        v.clear();for(auto x:both)v.push_back({二次关系约束角色::共同,x});
        for(auto x:a)if(std::find(both.begin(),both.end(),x)==both.end())v.push_back({二次关系约束角色::A,x});
        for(auto x:b)if(std::find(both.begin(),both.end(),x)==both.end())v.push_back({二次关系约束角色::B,x});
        std::sort(v.begin(),v.end(),约束小于);
    }; one(q.FC);one(q.EC);
}
bool 原子同量(const 二次关系原子定义&a,const 二次关系原子定义&b) noexcept {
    return a.K==b.K&&a.输出==b.输出&&a.约束==b.约束;
}
bool 原子小于(const 二次关系原子定义&a,const 二次关系原子定义&b) noexcept {
    if(a.K.编码.值!=b.K.编码.值)return a.K.编码.值<b.K.编码.值;
    if(a.输出!=b.输出)return static_cast<unsigned>(a.输出)<static_cast<unsigned>(b.输出);
    if(a.约束.FC!=b.约束.FC)return 约束组小于(a.约束.FC,b.约束.FC);
    if(a.约束.EC!=b.约束.EC)return 约束组小于(a.约束.EC,b.约束.EC);
    return a.D.掩码<b.D.掩码;
}
二次关系数据状态 映射纯二次失败(纯概念状态 s) noexcept {
    switch(s){
    case 纯概念状态::入口拒绝:return 二次关系数据状态::入口拒绝;
    case 纯概念状态::未找到:return 二次关系数据状态::未找到;
    case 纯概念状态::概念已退役:return 二次关系数据状态::退役命中;
    case 纯概念状态::定义不支持:case 纯概念状态::旧格式不支持:return 二次关系数据状态::格式不支持;
    case 纯概念状态::定义不相容:case 纯概念状态::类别冲突:return 二次关系数据状态::规则不支持;
    case 纯概念状态::组织冲突:case 纯概念状态::引用冲突:return 二次关系数据状态::引用冲突;
    case 纯概念状态::上位成环:return 二次关系数据状态::形成环;
    case 纯概念状态::事实代次漂移:return 二次关系数据状态::事实代次漂移;
    case 纯概念状态::幂等冲突:return 二次关系数据状态::幂等冲突;
    case 纯概念状态::数量预算不足:return 二次关系数据状态::旧预算不足;
    case 纯概念状态::资源失败:return 二次关系数据状态::资源失败;
    case 纯概念状态::已可能发布:return 二次关系数据状态::发布未知;
    default:return 二次关系数据状态::内部不一致;
    }
}
二次关系数据状态 映射二次失败(概念树数据状态 s) noexcept {
    switch(s){case 概念树数据状态::入口拒绝:return 二次关系数据状态::入口拒绝;case 概念树数据状态::未找到:return 二次关系数据状态::未找到;case 概念树数据状态::事实代次漂移:return 二次关系数据状态::事实代次漂移;case 概念树数据状态::数量预算不足:return 二次关系数据状态::旧预算不足;case 概念树数据状态::资源失败:return 二次关系数据状态::资源失败;default:return 二次关系数据状态::内部不一致;}
}
二次关系发布见证 二次见证(const L1所有者范围写入结果&r,bool replay){二次关系发布见证 x;x.幂等身份=r.写入幂等身份.值;x.发布H=r.事实代次;x.已确认发布=r.是否形成内存权威发布;x.精确重放=replay;for(const auto&[k,v]:r.新编码映射)x.新编码映射.push_back({k.值,v});std::sort(x.新编码映射.begin(),x.新编码映射.end(),[](auto&a,auto&b){return a.first<b.first;});return x;}
}

bool 二次关系约束读取结果::成功() const noexcept {
    return 版本==2&&状态==二次关系数据状态::已读取&&Gread&&定义.has_value();
}

二次关系规范形 概念树类数据服务::规范化二次关系内部(
    const 二次关系定义& input,std::uint64_t g,
    std::vector<概念树概念身份>& stack) const {
    if(!g)throw 二次关系失败{二次关系数据状态::入口拒绝};
    std::vector<二次关系原子定义> atoms;
    const auto validateAtom=[&](二次关系原子定义 a) {
        if(a.D.掩码<1||a.D.掩码>7||!有效(a.K)||a.输出!=特征类标量结果角色::差异)throw 二次关系失败{二次关系数据状态::规则不支持};
        try{规范约束组(a.约束);}catch(二次关系数据状态 s){throw 二次关系失败{s};}
        const auto mapK=[](特征I64比较绑定读取状态_v2 s) {
            switch(s) {
            case 特征I64比较绑定读取状态_v2::事实代次漂移:return 二次关系数据状态::事实代次漂移;
            case 特征I64比较绑定读取状态_v2::资源失败:return 二次关系数据状态::资源失败;
            case 特征I64比较绑定读取状态_v2::内部不一致:return 二次关系数据状态::内部不一致;
            default:return 二次关系数据状态::规则不支持;
            }
        };
        const auto k=features_.读取I64比较绑定_v2({2,g,a.K});
        if(!k.成功()||!k.事实)throw 二次关系失败{mapK(k.状态)};
        const auto current=features_.读取当前I64比较绑定_v2(
            {2,g,k.事实->定义.输入FT,k.事实->定义.用途});
        if(!current.成功()||!current.事实||current.事实->身份!=a.K)
            throw 二次关系失败{mapK(current.状态)};
        bool difference=false;for(const auto&o:k.事实->定义.输出组)if(o.输出.角色==特征类标量结果角色::差异)difference=true;if(!difference)throw 二次关系失败{二次关系数据状态::规则不支持};
        bool af=false,bf=false,ae=false,be=false;
        std::array<std::vector<概念树I64区间>,2> domains;
        const auto check=[&](const auto& group,bool fc){for(const auto&q:group){
            const auto rr=读取二次关系约束定义({2,{1,g},q.概念});
            if(!rr.成功())throw 二次关系失败{rr.状态};
            if(fc) {
                const auto* pure=std::get_if<纯概念事实>(&*rr.定义);
                const auto* definition=pure?std::get_if<纯I64特征概念定义>(&pure->定义):nullptr;
                if(!definition||definition->特征类型.值!=k.事实->定义.输入FT.编码)
                    throw 二次关系失败{二次关系数据状态::规则不支持};
                const auto addDomain=[&](std::size_t side){
                    if(domains[side].empty())domains[side]=definition->规范域;
                    else {
                        std::vector<概念树I64区间> next;
                        for(const auto&x:domains[side])for(const auto&y:definition->规范域){
                            const auto lo=std::max(x.下界,y.下界),hi=std::min(x.上界,y.上界);
                            if(lo<=hi)next.push_back({lo,hi});
                        }
                        domains[side]=std::move(next);
                        if(domains[side].empty())throw 二次关系失败{二次关系数据状态::定义矛盾};
                    }
                };
                if(q.角色==二次关系约束角色::共同||q.角色==二次关系约束角色::A){af=true;addDomain(0);}
                if(q.角色==二次关系约束角色::共同||q.角色==二次关系约束角色::B){bf=true;addDomain(1);}
            } else {
                bool ec=false;
                if(const auto*p=std::get_if<纯概念事实>(&*rr.定义))
                    ec=p->类别==相关概念类别::存在&&std::holds_alternative<通用存在概念定义>(p->定义);
                else ec=std::holds_alternative<存在概念两组事实_v3>(*rr.定义);
                if(!ec)throw 二次关系失败{二次关系数据状态::规则不支持};
                if(q.角色==二次关系约束角色::共同||q.角色==二次关系约束角色::A)ae=true;
                if(q.角色==二次关系约束角色::共同||q.角色==二次关系约束角色::B)be=true;
            }
        }};
        check(a.约束.FC,true);check(a.约束.EC,false);if(!af||!bf||!ae||!be)throw 二次关系失败{二次关系数据状态::来源不足};
        atoms.push_back(std::move(a));
    };
    if(const auto*a=std::get_if<二次关系原子定义>(&input))validateAtom(*a);else {
        const auto&andd=std::get<二次关系合取定义>(input);if(andd.条件组.empty())throw 二次关系失败{二次关系数据状态::入口拒绝};
        for(const auto&item:andd.条件组){if(!有效(item.子RC.值)||std::find(stack.begin(),stack.end(),item.子RC)!=stack.end())throw 二次关系失败{二次关系数据状态::形成环};stack.push_back(item.子RC);auto child=读取二次关系内部(item.子RC,g,stack);stack.pop_back();for(auto a:child.规范形.原子组){a.约束.FC.insert(a.约束.FC.end(),andd.共同约束.FC.begin(),andd.共同约束.FC.end());a.约束.FC.insert(a.约束.FC.end(),item.附加约束.FC.begin(),item.附加约束.FC.end());a.约束.EC.insert(a.约束.EC.end(),andd.共同约束.EC.begin(),andd.共同约束.EC.end());a.约束.EC.insert(a.约束.EC.end(),item.附加约束.EC.begin(),item.附加约束.EC.end());validateAtom(std::move(a));}}
    }
    std::sort(atoms.begin(),atoms.end(),原子小于);std::vector<二次关系原子定义> merged;for(auto&a:atoms){if(!merged.empty()&&原子同量(merged.back(),a)){merged.back().D.掩码&=a.D.掩码;if(!merged.back().D.掩码)throw 二次关系失败{二次关系数据状态::定义矛盾};}else merged.push_back(a);}return {1,std::move(merged)};
}

二次关系概念事实 概念树类数据服务::读取二次关系内部(
    概念树概念身份 id,std::uint64_t g,
    std::vector<概念树概念身份>& stack) const {
    if(!secondary_relation_layout_||!pure_layout_)throw 二次关系失败{二次关系数据状态::格式不支持};
    if(!有效(id.值)||!g)throw 二次关系失败{二次关系数据状态::入口拒绝};
    const auto& p=pure_layout_->类型;const auto&t=secondary_relation_layout_->类型;
    二次关系概念事实 out;out.Gread=g;out.身份=id;out.类别=相关概念类别::特征;out.规则=secondary_relation_layout_->规范化规则;
    const auto readNode=[&](稳定编码 code){return 节点(code,g);};
    const auto readEdges=[&](稳定编码 endpoint,稳定编码 type,bool incoming,std::uint64_t wanted){
        auto values=完整关系组(endpoint,type,incoming,g);
        if(wanted&&values.size()>wanted)throw 二次关系失败{二次关系数据状态::内部不一致};
        return values;
    };
    const auto readValues=[&](稳定编码 code){
        return 完整属性值组(code,g);
    };
    const auto addNode=[&](const N&n){out.自有节点.push_back({n.编码,投影纯生命(n)});};
    const auto addEdge=[&](const E&e){out.自有关系.push_back({e.编码,e.源节点,e.目标节点,e.关系类型节点,e.角色或顺序,投影纯生命(e)});};
    const auto addValue=[&](const V&v){二次关系值见证 x;x.编码=v.编码;x.所属节点=v.所属节点;x.属性类型=v.属性类型节点;x.来源=v.来源节点;if(const auto*i=std::get_if<std::int64_t>(&v.材料))x.材料=*i;else if(const auto*q=std::get_if<std::vector<std::uint64_t>>(&v.材料))x.材料=*q;else throw 二次关系失败{二次关系数据状态::内部不一致};x.生命周期=投影纯生命(v);out.自有值.push_back(std::move(x));};

    const auto conceptNode=readNode(id.值);if(conceptNode.种类!=节点种类::普通||conceptNode.属性类型表示)throw 二次关系失败{二次关系数据状态::内部不一致};
    addNode(conceptNode);out.生命周期=投影纯生命(conceptNode);
    const auto member=readEdges(id.值,p.概念族成员,true,2);if(member.size()!=1||member.front().源节点!=pure_layout_->概念族锚点||member.front().角色或顺序!=1)throw 二次关系失败{二次关系数据状态::格式不支持};addEdge(member.front());
    const auto cv=readValues(id.值);for(const auto&v:cv)addValue(v);
    const auto category=std::get_if<std::int64_t>(&唯一属性(cv,p.概念类别).材料);
    const auto lifecycle=std::get_if<std::int64_t>(&唯一属性(cv,p.生命周期).材料);
    if(!category||*category!=2||!lifecycle||*lifecycle<1||*lifecycle>3)throw 二次关系失败{二次关系数据状态::规则不支持};
    out.治理状态=static_cast<概念树生命周期状态>(*lifecycle);
    const auto defEdge=readEdges(id.值,p.定义成员,false,2);if(defEdge.size()!=1||defEdge.front().角色或顺序!=1)throw 二次关系失败{二次关系数据状态::格式不支持};addEdge(defEdge.front());
    const auto defNode=readNode(defEdge.front().目标节点);if(defNode.种类!=节点种类::普通||defNode.属性类型表示)throw 二次关系失败{二次关系数据状态::内部不一致};addNode(defNode);
    const auto dv=readValues(defNode.编码);for(const auto&v:dv)addValue(v);
    const auto format=std::get_if<std::int64_t>(&唯一属性(dv,t.定义格式).材料);
    const auto kind=std::get_if<std::int64_t>(&唯一属性(dv,t.定义种类).材料);
    if(!format||*format!=1||!kind||(*kind!=1&&*kind!=2)||dv.size()!=2)throw 二次关系失败{二次关系数据状态::格式不支持};
    const auto rule=readEdges(id.值,t.规范化规则归属,false,2);if(rule.size()!=1||rule.front().角色或顺序!=1||rule.front().目标节点!=secondary_relation_layout_->规范化规则.值)throw 二次关系失败{二次关系数据状态::规则不支持};addEdge(rule.front());
    const auto parents=readEdges(id.值,p.直接上位,true,0);std::set<std::uint64_t> parentIds;for(const auto&e:parents){if(e.角色或顺序!=1||e.目标节点!=id.值||e.源节点==id.值||!parentIds.insert(e.源节点.值).second)throw 二次关系失败{二次关系数据状态::形成环};addEdge(e);}

    const auto readConstraints=[&](稳定编码 holder,std::uint64_t itemOrder){
        二次关系约束组 group;const auto rows=readEdges(holder,t.约束成员,false,0);
        for(std::size_t i=0;i<rows.size();++i){const auto&e=rows[i];if(e.角色或顺序!=static_cast<std::int64_t>(i+1))throw 二次关系失败{二次关系数据状态::内部不一致};addEdge(e);const auto record=readNode(e.目标节点);if(record.种类!=节点种类::普通||record.属性类型表示||!readValues(record.编码).empty())throw 二次关系失败{二次关系数据状态::内部不一致};addNode(record);const auto fc=readEdges(record.编码,t.约束FC,false,2);const auto ec=readEdges(record.编码,t.约束EC,false,2);if((fc.size()+ec.size())!=1)throw 二次关系失败{二次关系数据状态::内部不一致};const auto&target=fc.empty()?ec.front():fc.front();if(target.角色或顺序<1||target.角色或顺序>3)throw 二次关系失败{二次关系数据状态::内部不一致};addEdge(target);二次关系概念约束 q{static_cast<二次关系约束角色>(target.角色或顺序),概念树概念身份{target.目标节点}};(fc.empty()?group.EC:group.FC).push_back(q);out.定义引用.push_back({fc.empty()?二次关系引用种类::EC:二次关系引用种类::FC,record.编码,e.编码,target.编码,itemOrder,i+1,q.角色,q.概念});}
        return group;
    };

    if(*kind==1){
        if(cv.size()!=4)throw 二次关系失败{二次关系数据状态::内部不一致};
        const auto*d=std::get_if<std::int64_t>(&唯一属性(cv,t.域掩码).材料);const auto*output=std::get_if<std::int64_t>(&唯一属性(cv,t.输出角色).材料);
        if(!d||*d<1||*d>7||!output||*output!=static_cast<std::int64_t>(特征类标量结果角色::差异))throw 二次关系失败{二次关系数据状态::规则不支持};
        const auto k=readEdges(id.值,t.固定K,false,2);if(k.size()!=1||k.front().角色或顺序!=1)throw 二次关系失败{二次关系数据状态::内部不一致};addEdge(k.front());
        if(!readEdges(id.值,t.合取成员,false,1).empty())throw 二次关系失败{二次关系数据状态::内部不一致};
        二次关系原子定义 atom{{static_cast<std::uint8_t>(*d)},readConstraints(id.值,0),特征I64比较绑定身份{k.front().目标节点},static_cast<特征类标量结果角色>(*output)};out.定义=atom;out.定义引用.push_back({二次关系引用种类::K,id.值,k.front().编码,k.front().编码,0,0,二次关系约束角色::共同,atom.K});
    }else{
        if(cv.size()!=2||!readEdges(id.值,t.固定K,false,1).empty())throw 二次关系失败{二次关系数据状态::内部不一致};
        二次关系合取定义 definition;definition.共同约束=readConstraints(id.值,0);const auto items=readEdges(id.值,t.合取成员,false,0);if(items.empty())throw 二次关系失败{二次关系数据状态::内部不一致};
        for(std::size_t i=0;i<items.size();++i){const auto&e=items[i];if(e.角色或顺序!=static_cast<std::int64_t>(i+1))throw 二次关系失败{二次关系数据状态::内部不一致};addEdge(e);const auto item=readNode(e.目标节点);if(item.种类!=节点种类::普通||item.属性类型表示||!readValues(item.编码).empty())throw 二次关系失败{二次关系数据状态::内部不一致};addNode(item);const auto child=readEdges(item.编码,t.子RC,false,2);if(child.size()!=1||child.front().角色或顺序!=1)throw 二次关系失败{二次关系数据状态::内部不一致};addEdge(child.front());definition.条件组.push_back({概念树概念身份{child.front().目标节点},readConstraints(item.编码,i+1)});out.定义引用.push_back({二次关系引用种类::子RC,item.编码,e.编码,child.front().编码,i+1,0,二次关系约束角色::共同,概念树概念身份{child.front().目标节点}});}
        out.定义=std::move(definition);
    }
    out.规范形=规范化二次关系内部(out.定义,g,stack);
    std::sort(out.定义引用.begin(),out.定义引用.end(),[](const auto&a,const auto&b){return a.记录.值<b.记录.值;});
    const auto sortId=[](const auto&a,const auto&b){return a.编码.值<b.编码.值;};std::sort(out.自有节点.begin(),out.自有节点.end(),sortId);std::sort(out.自有关系.begin(),out.自有关系.end(),sortId);std::sort(out.自有值.begin(),out.自有值.end(),sortId);
    return out;
}

二次关系定义核验结果 概念树类数据服务::规范化二次关系定义(const 二次关系定义查找请求&r) const {
    二次关系定义核验结果 out;out.Gread=r.读取头.Gread;
    try{if(!secondary_relation_layout_)throw 二次关系失败{二次关系数据状态::格式不支持};if(r.版本!=2||r.读取头.合同版本!=1||!r.读取头.Gread)throw 二次关系失败{二次关系数据状态::入口拒绝};守卫代次(r.读取头.Gread);std::vector<概念树概念身份> stack;out.规范形=规范化二次关系内部(r.定义,r.读取头.Gread,stack);out.状态=二次关系数据状态::已读取;守卫代次(r.读取头.Gread);}catch(const 二次关系失败&e){out.状态=e.状态;out.规范形.reset();}catch(const 失败&e){out.状态=映射二次失败(e.状态);out.规范形.reset();}catch(const std::bad_alloc&){out.状态=二次关系数据状态::资源失败;out.规范形.reset();}catch(...){out.状态=二次关系数据状态::内部不一致;out.规范形.reset();}return out;
}

二次关系概念读取结果 概念树类数据服务::读取二次关系概念(const 二次关系概念读取请求&r) const {
    二次关系概念读取结果 out;out.Gread=r.读取头.Gread;
    try {
        if(!secondary_relation_layout_)
            throw 二次关系失败{二次关系数据状态::格式不支持};
        if(r.版本!=2||r.读取头.合同版本!=1||!r.读取头.Gread||!有效(r.RC.值))
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        守卫代次(r.读取头.Gread);
        std::vector<概念树概念身份> stack{r.RC};
        out.事实=读取二次关系内部(r.RC,r.读取头.Gread,stack);
        out.状态=out.事实->治理状态==概念树生命周期状态::活跃
            ? 二次关系数据状态::已读取
            : (out.事实->治理状态==概念树生命周期状态::冷却
                ? 二次关系数据状态::冷却命中
                : 二次关系数据状态::退役命中);
        守卫代次(r.读取头.Gread);
    } catch(const 二次关系失败&e) {
        out.状态=e.状态;out.事实.reset();
    } catch(const 失败&e) {
        out.状态=映射二次失败(e.状态);out.事实.reset();
    } catch(const std::bad_alloc&) {
        out.状态=二次关系数据状态::资源失败;out.事实.reset();
    } catch(...) {
        out.状态=二次关系数据状态::内部不一致;out.事实.reset();
    }
    return out;
}

std::vector<概念树概念身份> 概念树类数据服务::枚举二次关系身份(
    std::uint64_t g) const {
    if(!secondary_relation_layout_||!pure_layout_||!g)
        throw 二次关系失败{二次关系数据状态::格式不支持};
    std::vector<概念树概念身份> out;
    const auto members=完整关系组(pure_layout_->概念族锚点,
        pure_layout_->类型.概念族成员,false,g);
    for(const auto& member:members) {
        if(member.角色或顺序!=1)
            throw 二次关系失败{二次关系数据状态::内部不一致};
        const auto rules=完整关系组(member.目标节点,
            secondary_relation_layout_->类型.规范化规则归属,false,g);
        if(rules.empty())continue;
        if(rules.size()!=1||rules.front().角色或顺序!=1||
           rules.front().目标节点!=secondary_relation_layout_->规范化规则.值)
            throw 二次关系失败{二次关系数据状态::内部不一致};
        out.emplace_back(member.目标节点);
    }
    std::sort(out.begin(),out.end(),身份小于);
    if(std::adjacent_find(out.begin(),out.end())!=out.end())
        throw 二次关系失败{二次关系数据状态::内部不一致};
    return out;
}

二次关系概念读取结果 概念树类数据服务::查找二次关系完整定义(const 二次关系定义查找请求&r) const {
    二次关系概念读取结果 out;out.Gread=r.读取头.Gread;
    try{if(r.版本!=2||r.读取头.合同版本!=1||!r.读取头.Gread)throw 二次关系失败{二次关系数据状态::入口拒绝};守卫代次(r.读取头.Gread);std::vector<概念树概念身份> stack;const auto expected=规范化二次关系内部(r.定义,r.读取头.Gread,stack);std::optional<二次关系概念事实> found;for(const auto id:枚举二次关系身份(r.读取头.Gread)){std::vector<概念树概念身份> path{id};auto fact=读取二次关系内部(id,r.读取头.Gread,path);if(fact.规范形==expected){if(found)throw 二次关系失败{二次关系数据状态::内部不一致};found=std::move(fact);}}if(!found)out.状态=二次关系数据状态::同义缺失;else{out.状态=found->治理状态==概念树生命周期状态::活跃 ? 二次关系数据状态::已读取 : (found->治理状态==概念树生命周期状态::冷却 ? 二次关系数据状态::冷却命中 : 二次关系数据状态::退役命中);out.事实=std::move(found);}守卫代次(r.读取头.Gread);}catch(const 二次关系失败&e){out.状态=e.状态;out.事实.reset();}catch(const 失败&e){out.状态=映射二次失败(e.状态);out.事实.reset();}catch(const std::bad_alloc&){out.状态=二次关系数据状态::资源失败;out.事实.reset();}catch(...){out.状态=二次关系数据状态::内部不一致;out.事实.reset();}return out;
}

L1所有者范围写集请求 概念树类数据服务::形成二次关系建立写集(
    const 二次关系概念建立请求&r,const 二次关系规范形& normalized) const {
    const auto&p=pure_layout_->类型;const auto&t=secondary_relation_layout_->类型;
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.写入头.期望事实代次,r.写入头.幂等身份};
    std::uint32_t nodeKey=1,edgeKey=0x51001,valueKey=0x52001;
    const Key conceptKey{nodeKey++},definitionKey{nodeKey++};ws.节点.push_back({conceptKey,节点种类::普通,std::nullopt});ws.节点.push_back({definitionKey,节点种类::普通,std::nullopt});
    const auto edge=[&](Ref source,Ref target,稳定编码 type,std::int64_t role=1){ws.关系.push_back({Key{edgeKey++},source,target,Ref{type},role});};
    const auto value=[&](Ref node,稳定编码 type,L1所有者范围原始值材料 material){const Key key{valueKey++};ws.值.push_back({key,node,Ref{type},std::move(material),node});ws.属性槽变更.push_back({node,Ref{type},key});};
    edge(Ref{pure_layout_->概念族锚点},Ref{conceptKey},p.概念族成员);edge(Ref{conceptKey},Ref{definitionKey},p.定义成员);edge(Ref{conceptKey},Ref{secondary_relation_layout_->规范化规则.值},t.规范化规则归属);
    value(Ref{conceptKey},p.概念类别,std::int64_t{2});value(Ref{conceptKey},p.生命周期,std::int64_t{static_cast<std::uint8_t>(概念树生命周期状态::活跃)});value(Ref{definitionKey},t.定义格式,std::int64_t{1});
    auto parents=r.初始父组;std::sort(parents.begin(),parents.end(),身份小于);for(const auto parent:parents)edge(Ref{parent.值},Ref{conceptKey},p.直接上位);
    const auto writeConstraints=[&](Ref holder,二次关系约束组 q){规范约束组(q);std::vector<std::pair<bool,二次关系概念约束>> all;for(const auto&x:q.FC)all.emplace_back(true,x);for(const auto&x:q.EC)all.emplace_back(false,x);std::sort(all.begin(),all.end(),[](const auto&a,const auto&b){if(a.first!=b.first)return a.first>b.first;return 约束小于(a.second,b.second);});for(std::size_t i=0;i<all.size();++i){const Key record{nodeKey++};ws.节点.push_back({record,节点种类::普通,std::nullopt});edge(holder,Ref{record},t.约束成员,static_cast<std::int64_t>(i+1));edge(Ref{record},Ref{all[i].second.概念.值},all[i].first?t.约束FC:t.约束EC,static_cast<std::int64_t>(all[i].second.角色));}};
    if(normalized.原子组.size()==1){const auto&a=normalized.原子组.front();value(Ref{definitionKey},t.定义种类,std::int64_t{1});value(Ref{conceptKey},t.域掩码,std::int64_t{a.D.掩码});value(Ref{conceptKey},t.输出角色,std::int64_t{static_cast<std::uint8_t>(a.输出)});edge(Ref{conceptKey},Ref{a.K.编码},t.固定K);writeConstraints(Ref{conceptKey},a.约束);}else{value(Ref{definitionKey},t.定义种类,std::int64_t{2});const auto*andd=std::get_if<二次关系合取定义>(&r.定义);if(!andd)throw 二次关系失败{二次关系数据状态::规则不支持};auto items=andd->条件组;for(auto&i:items)规范约束组(i.附加约束);std::sort(items.begin(),items.end(),[](const auto&a,const auto&b){if(a.子RC.值.值!=b.子RC.值.值)return a.子RC.值.值<b.子RC.值.值;if(a.附加约束.FC!=b.附加约束.FC)return 约束组小于(a.附加约束.FC,b.附加约束.FC);return 约束组小于(a.附加约束.EC,b.附加约束.EC);});if(std::adjacent_find(items.begin(),items.end())!=items.end())items.erase(std::unique(items.begin(),items.end()),items.end());auto common=andd->共同约束;规范约束组(common);writeConstraints(Ref{conceptKey},common);for(std::size_t i=0;i<items.size();++i){const Key item{nodeKey++};ws.节点.push_back({item,节点种类::普通,std::nullopt});edge(Ref{conceptKey},Ref{item},t.合取成员,static_cast<std::int64_t>(i+1));edge(Ref{item},Ref{items[i].子RC.值},t.子RC);writeConstraints(Ref{item},items[i].附加约束);}}
    auto sources=r.形成来源;const auto sourceCode=[](const 二次关系形成来源&s){return std::visit([](const auto&v){if constexpr(std::is_same_v<std::decay_t<decltype(v)>,特征信息身份>)return std::pair{1u,v.编码.值};else if constexpr(std::is_same_v<std::decay_t<decltype(v)>,状态使用绑定身份>)return std::pair{2u,v.编码.值};else return std::pair{3u,v.值.值};},s.来源);};std::sort(sources.begin(),sources.end(),[&](const auto&a,const auto&b){return sourceCode(a)<sourceCode(b);});
    for(const auto&s:sources){const Key record{nodeKey++};ws.节点.push_back({record,节点种类::普通,std::nullopt});edge(Ref{conceptKey},Ref{record},t.来源成员);std::visit([&](const auto&v){if constexpr(std::is_same_v<std::decay_t<decltype(v)>,特征信息身份>)edge(Ref{record},Ref{v.编码},t.来源F);else if constexpr(std::is_same_v<std::decay_t<decltype(v)>,状态使用绑定身份>)edge(Ref{record},Ref{v.编码},t.来源B);else edge(Ref{record},Ref{v.值},t.来源概念);},s.来源);}
    二次规范化写集(ws);
    return ws;
}

namespace {
bool 二次约束包含(const std::vector<二次关系概念约束>& child,const std::vector<二次关系概念约束>& parent){return std::includes(child.begin(),child.end(),parent.begin(),parent.end(),约束小于);}
bool 二次原子收窄(const 二次关系原子定义&child,const 二次关系原子定义&parent){return child.K==parent.K&&child.输出==parent.输出&&(child.D.掩码&parent.D.掩码)==child.D.掩码&&二次约束包含(child.约束.FC,parent.约束.FC)&&二次约束包含(child.约束.EC,parent.约束.EC);}
二次关系数据状态 映射二次写入(L1所有者范围写入状态 s) noexcept{switch(s){case L1所有者范围写入状态::事实代次漂移:return 二次关系数据状态::事实代次漂移;case L1所有者范围写入状态::幂等冲突:return 二次关系数据状态::幂等冲突;case L1所有者范围写入状态::引用冲突:case L1所有者范围写入状态::未找到:return 二次关系数据状态::引用冲突;case L1所有者范围写入状态::资源失败:return 二次关系数据状态::资源失败;case L1所有者范围写入状态::入口拒绝:return 二次关系数据状态::入口拒绝;default:return 二次关系数据状态::发布未知;}}

std::pair<unsigned,std::uint64_t> 二次来源键(const 二次关系形成来源& source) {
    return std::visit([](const auto& value) {
        if constexpr(std::is_same_v<std::decay_t<decltype(value)>,特征信息身份>)
            return std::pair{1u,value.编码.值};
        else if constexpr(std::is_same_v<std::decay_t<decltype(value)>,状态使用绑定身份>)
            return std::pair{2u,value.编码.值};
        else
            return std::pair{3u,value.值.值};
    },source.来源);
}

bool 二次首次发布已确认(const L1所有者范围首次写入读取结果& first) noexcept {
    return first.状态==L1所有者范围读取状态::成功&&first.首次规范化写集&&
        first.首次写入结果&&first.首次写入结果->状态==L1所有者范围写入状态::成功&&
        first.首次写入结果->是否形成内存权威发布&&first.首次写入结果->事实代次;
}

bool 二次提交状态符合(const L1所有者范围写入结果& saved,bool replay) noexcept {
    if(replay)return saved.状态==L1所有者范围写入状态::精确重复;
    return saved.状态==L1所有者范围写入状态::成功&&
        saved.是否形成内存权威发布&&saved.事实代次;
}
}

二次关系概念写入结果 概念树类数据服务::建立二次关系概念(const 二次关系概念建立请求&r){
    二次关系概念写入结果 out;out.建立原请求=r;bool dispatched=false;
    try {
        std::scoped_lock lock(mutex_);
        if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||
           !r.写入头.期望事实代次||r.写入头.期望事实代次==UINT64_MAX||
           !有效(r.写入头.幂等身份)||
           (r.组织!=概念初始组织指定::显式顶层&&
            r.组织!=概念初始组织指定::具名上位))
            throw 二次关系失败{二次关系数据状态::入口拒绝};

        const auto current=当前代次();
        out.Gread=current;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||
           first.所有者!=port_.所有者身份()||
           first.写入幂等身份!=r.写入头.幂等身份)
            throw 二次关系失败{二次关系数据状态::内部不一致};
        const bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay&&!二次首次发布已确认(first))
            throw 二次关系失败{二次关系数据状态::发布未知};
        if(!replay&&first.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ?
                二次关系数据状态::资源失败 : 二次关系数据状态::内部不一致};
        if(!replay&&current!=r.写入头.期望事实代次)
            throw 二次关系失败{二次关系数据状态::事实代次漂移};
        守卫代次(current);

        auto parents=r.初始父组;
        std::sort(parents.begin(),parents.end(),身份小于);
        if(std::adjacent_find(parents.begin(),parents.end())!=parents.end()||
           (r.组织==概念初始组织指定::显式顶层&&!parents.empty())||
           (r.组织==概念初始组织指定::具名上位&&parents.empty()))
            throw 二次关系失败{二次关系数据状态::引用冲突};

        std::vector<概念树概念身份> path;
        const auto norm=规范化二次关系内部(r.定义,current,path);
        for(const auto parent:parents) {
            std::vector<概念树概念身份> parentPath{parent};
            const auto parentFact=读取二次关系内部(parent,current,parentPath);
            if(parentFact.治理状态==概念树生命周期状态::退役)
                throw 二次关系失败{二次关系数据状态::退役命中};
            for(const auto& parentAtom:parentFact.规范形.原子组) {
                const bool matched=std::any_of(norm.原子组.begin(),norm.原子组.end(),
                    [&](const auto& childAtom){return 二次原子收窄(childAtom,parentAtom);});
                if(!matched)throw 二次关系失败{二次关系数据状态::引用冲突};
            }
        }

        std::set<std::pair<unsigned,std::uint64_t>> sourceSet;
        for(const auto& source:r.形成来源) {
            if(!sourceSet.insert(二次来源键(source)).second)
                throw 二次关系失败{二次关系数据状态::引用冲突};
            std::visit([&](const auto& value) {
                if constexpr(std::is_same_v<std::decay_t<decltype(value)>,特征信息身份>) {
                    if(std::holds_alternative<特征数据错误>(
                        features_.读取准确特征事实({1,current,value})))
                        throw 二次关系失败{二次关系数据状态::来源不足};
                } else if constexpr(std::is_same_v<std::decay_t<decltype(value)>,状态使用绑定身份>) {
                    if(!state_use_bindings_||
                       !state_use_bindings_->读取当前状态使用绑定({1,current,value}).成功())
                        throw 二次关系失败{二次关系数据状态::来源不足};
                } else {
                    const auto constraint=读取二次关系约束定义(
                        {2,{1,current},value});
                    if(!constraint.成功()) {
                        std::vector<概念树概念身份> conceptPath{value};
                        try {
                            (void)读取二次关系内部(value,current,conceptPath);
                        } catch(...) {
                            throw 二次关系失败{二次关系数据状态::来源不足};
                        }
                    }
                }
            },source.来源);
        }

        const auto writeSet=形成二次关系建立写集(r,norm);
        if(replay) {
            if(*first.首次规范化写集!=writeSet)
                throw 二次关系失败{二次关系数据状态::幂等冲突};
        } else {
            const auto found=查找二次关系完整定义(
                {2,{1,current},r.定义});
            if(found.事实) {
                out.事实=found.事实;
                out.状态=found.状态==二次关系数据状态::冷却命中 ?
                    二次关系数据状态::冷却命中 :
                    (found.状态==二次关系数据状态::退役命中 ?
                        二次关系数据状态::退役命中 : 二次关系数据状态::已复用);
                out.建立原请求.reset();
                return out;
            }
            if(found.状态!=二次关系数据状态::同义缺失)
                throw 二次关系失败{found.状态};
        }

        dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(writeSet);
        if(!二次提交状态符合(saved,replay))
            throw 二次关系失败{映射二次写入(saved.状态)};
        const auto& published=replay ? *first.首次写入结果 : saved;
        if(published.事实代次!=r.写入头.期望事实代次+1)
            throw 二次关系失败{二次关系数据状态::发布未知};
        稳定编码 id{};
        for(const auto&[key,value]:published.新编码映射)
            if(key.值==1)id=value;
        if(!有效(id))throw 二次关系失败{二次关系数据状态::发布未知};
        out.首次H=published.事实代次;
        out.正式回执=二次见证(published,replay);
        const auto tail=当前代次();
        out.Gread=tail;
        std::vector<概念树概念身份> readPath{概念树概念身份{id}};
        out.事实=读取二次关系内部(
            概念树概念身份{id},tail,readPath);
        out.状态=replay ? 二次关系数据状态::精确重放 : 二次关系数据状态::已创建;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.事实.reset();if(out.状态!=二次关系数据状态::发布未知){out.首次H.reset();out.正式回执.reset();out.建立原请求.reset();}}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.事实.reset();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.事实.reset();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.事实.reset();}return out;
}

二次关系概念写入结果 概念树类数据服务::收敛二次关系概念建立(const 二次关系概念建立请求&r){
    二次关系概念写入结果 out;out.建立原请求=r;bool dispatched=false;
    try{std::scoped_lock lock(mutex_);if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||!r.写入头.期望事实代次||!有效(r.写入头.幂等身份))throw 二次关系失败{二次关系数据状态::入口拒绝};const auto current=当前代次();out.Gread=current;const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});if(first.状态==L1所有者范围读取状态::未找到){out.状态=二次关系数据状态::未找到;out.建立原请求.reset();return out;}if(!二次首次发布已确认(first))throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ? 二次关系数据状态::资源失败 : 二次关系数据状态::发布未知};std::vector<概念树概念身份> path;const auto norm=规范化二次关系内部(r.定义,current,path);const auto ws=形成二次关系建立写集(r,norm);if(ws!=*first.首次规范化写集)throw 二次关系失败{二次关系数据状态::幂等冲突};dispatched=true;const auto saved=port_.提交所有者范围中性写集(ws);if(!二次提交状态符合(saved,true))throw 二次关系失败{映射二次写入(saved.状态)};稳定编码 id{};for(const auto&[key,value]:first.首次写入结果->新编码映射)if(key.值==1)id=value;if(!有效(id))throw 二次关系失败{二次关系数据状态::发布未知};out.首次H=first.首次写入结果->事实代次;out.正式回执=二次见证(*first.首次写入结果,true);const auto tail=当前代次();out.Gread=tail;std::vector<概念树概念身份> rp{概念树概念身份{id}};out.事实=读取二次关系内部(概念树概念身份{id},tail,rp);out.状态=二次关系数据状态::精确重放;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.事实.reset();if(out.状态!=二次关系数据状态::发布未知){out.首次H.reset();out.正式回执.reset();out.建立原请求.reset();}}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.事实.reset();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.事实.reset();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.事实.reset();}return out;
}

二次关系关联结果 概念树类数据服务::读取二次关系来源(const 二次关系关联读取请求&r) const {
    二次关系关联结果 out;out.Gread=r.读取头.Gread;
    try {
        if(!secondary_relation_layout_||r.版本!=2||r.读取头.合同版本!=1||
           !r.读取头.Gread||
           !有效(r.RC.值))
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        守卫代次(r.读取头.Gread);
        std::vector<概念树概念身份> path{r.RC};
        (void)读取二次关系内部(r.RC,r.读取头.Gread,path);
        const auto&t=secondary_relation_layout_->类型;
        const auto rows=完整关系组(r.RC.值,t.来源成员,false,r.读取头.Gread);
        std::set<std::pair<unsigned,std::uint64_t>> sourceKeys;
        for(const auto& member:rows) {
            if(member.角色或顺序!=1)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            const auto node=节点(member.目标节点,r.读取头.Gread);
            if(node.种类!=节点种类::普通||node.属性类型表示)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            const auto f=完整关系组(node.编码,t.来源F,false,r.读取头.Gread);
            const auto b=完整关系组(node.编码,t.来源B,false,r.读取头.Gread);
            const auto c=完整关系组(node.编码,t.来源概念,false,r.读取头.Gread);
            const auto targetCount=f.size()+b.size()+c.size();
            if(targetCount!=1)throw 二次关系失败{二次关系数据状态::内部不一致};
            const E& target=!f.empty()?f.front():(!b.empty()?b.front():c.front());
            if(target.角色或顺序!=1)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            const auto values=完整属性值组(node.编码,r.读取头.Gread);
            if(!values.empty())
                throw 二次关系失败{二次关系数据状态::内部不一致};
            二次关系形成来源 source;
            if(!f.empty())source.来源=特征信息身份{target.目标节点};
            else if(!b.empty())source.来源=状态使用绑定身份{target.目标节点};
            else source.来源=概念树概念身份{target.目标节点};
            if(!sourceKeys.insert(二次来源键(source)).second)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            out.来源组.push_back({node.编码,member.编码,target.编码,
                r.RC,source,投影纯生命(node)});
        }
        std::sort(out.来源组.begin(),out.来源组.end(),
            [](const auto&a,const auto&b){return a.记录.值<b.记录.值;});
        out.状态=二次关系数据状态::已读取;
        守卫代次(r.读取头.Gread);
    }catch(const 二次关系失败&e){out.状态=e.状态;out.来源组.clear();}catch(const 失败&e){out.状态=映射二次失败(e.状态);out.来源组.clear();}catch(const std::bad_alloc&){out.状态=二次关系数据状态::资源失败;out.来源组.clear();}catch(...){out.状态=二次关系数据状态::内部不一致;out.来源组.clear();}return out;
}

二次关系关联结果 概念树类数据服务::读取二次关系用途(const 二次关系关联读取请求&r) const {
    二次关系关联结果 out;out.Gread=r.读取头.Gread;
    try {
        if(!secondary_relation_layout_||r.版本!=2||r.读取头.合同版本!=1||
           !r.读取头.Gread||
           !有效(r.RC.值))
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        守卫代次(r.读取头.Gread);
        std::vector<概念树概念身份> path{r.RC};
        (void)读取二次关系内部(r.RC,r.读取头.Gread,path);
        const auto&t=secondary_relation_layout_->类型;
        const auto rows=完整关系组(r.RC.值,t.用途成员,false,r.读取头.Gread);
        std::set<std::tuple<std::uint64_t,std::uint64_t,std::uint32_t>> useKeys;
        for(const auto& member:rows) {
            if(member.角色或顺序!=1)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            const auto node=节点(member.目标节点,r.读取头.Gread);
            const auto target=完整关系组(node.编码,t.用途目标,false,r.读取头.Gread);
            const auto basis=完整关系组(node.编码,t.用途业务依据,false,r.读取头.Gread);
            if(target.size()!=1||basis.size()!=1)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            if(target.front().目标节点!=r.RC.值||target.front().角色或顺序!=1||
               basis.front().角色或顺序!=1)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            const auto values=完整属性值组(node.编码,r.读取头.Gread);
            if(values.size()!=3)throw 二次关系失败{二次关系数据状态::内部不一致};
            const auto* business=std::get_if<std::vector<std::uint64_t>>(
                &唯一属性(values,t.用途业务标识).材料);
            const auto* role=std::get_if<std::int64_t>(&唯一属性(values,t.用途角色).材料);
            const auto* time=std::get_if<std::int64_t>(&唯一属性(values,t.用途时间).材料);
            if(!business||business->size()!=1||!business->front()||!role||*role<1||
               *role>4||!time||*time<0)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            const auto roleValue=static_cast<std::uint32_t>(*role);
            if(!useKeys.emplace(business->front(),r.RC.值.值,roleValue).second)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            out.用途组.push_back({node.编码,member.编码,target.front().编码,
                basis.front().编码,r.RC,business->front(),roleValue,*time,
                basis.front().目标节点,
                投影纯生命(node)});
        }
        std::sort(out.用途组.begin(),out.用途组.end(),
            [](const auto&a,const auto&b){return a.记录.值<b.记录.值;});
        out.状态=二次关系数据状态::已读取;
        守卫代次(r.读取头.Gread);
    }catch(const 二次关系失败&e){out.状态=e.状态;out.用途组.clear();}catch(const 失败&e){out.状态=映射二次失败(e.状态);out.用途组.clear();}catch(const std::bad_alloc&){out.状态=二次关系数据状态::资源失败;out.用途组.clear();}catch(...){out.状态=二次关系数据状态::内部不一致;out.用途组.clear();}return out;
}

二次关系关联结果 概念树类数据服务::添加二次关系形成来源(const 二次关系来源写入请求&r){
    二次关系关联结果 out;bool dispatched=false;
    try {
        std::scoped_lock lock(mutex_);
        if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||
           !r.写入头.期望事实代次||!有效(r.写入头.幂等身份)||
           !有效(r.RC.值)||r.来源.empty())
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        const auto g=当前代次();
        out.Gread=g;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});
        const bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay&&!二次首次发布已确认(first))
            throw 二次关系失败{二次关系数据状态::发布未知};
        if(!replay&&first.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ?
                二次关系数据状态::资源失败 : 二次关系数据状态::内部不一致};
        if(!replay&&g!=r.写入头.期望事实代次)
            throw 二次关系失败{二次关系数据状态::事实代次漂移};
        const auto expectedGeneration=g;
        const auto existing=读取二次关系来源({2,{1,g},r.RC});
        if(!existing.成功())throw 二次关系失败{existing.状态};

        auto requested=r.来源;
        std::sort(requested.begin(),requested.end(),[](const auto&a,const auto&b){
            const auto ka=二次来源键(a),kb=二次来源键(b);
            return ka<kb;
        });
        for(std::size_t i=0;i<requested.size();++i) {
            if(i&&二次来源键(requested[i-1])==二次来源键(requested[i]))
                throw 二次关系失败{二次关系数据状态::引用冲突};
        }

        std::vector<二次关系形成来源> missing;
        for(const auto& source:requested) {
            const auto same=std::find_if(existing.来源组.begin(),existing.来源组.end(),
                [&](const auto& item){return 二次来源键(item.来源)==二次来源键(source);});
            if(same!=existing.来源组.end()) {
                continue;
            }
            std::visit([&](const auto& value) {
                if constexpr(std::is_same_v<std::decay_t<decltype(value)>,特征信息身份>) {
                    if(std::holds_alternative<特征数据错误>(
                        features_.读取准确特征事实({1,g,value})))
                        throw 二次关系失败{二次关系数据状态::来源不足};
                } else if constexpr(std::is_same_v<std::decay_t<decltype(value)>,状态使用绑定身份>) {
                    if(!state_use_bindings_||
                       !state_use_bindings_->读取当前状态使用绑定({1,g,value}).成功())
                        throw 二次关系失败{二次关系数据状态::来源不足};
                } else {
                    const auto constraint=读取二次关系约束定义(
                        {2,{1,g},value});
                    if(!constraint.成功()) {
                        std::vector<概念树概念身份> path{value};
                        (void)读取二次关系内部(value,g,path);
                    }
                }
            },source.来源);
            missing.push_back(source);
        }
        if(replay) missing=requested;

        if(!replay&&missing.empty()) {
            out.Gread=g;out.状态=二次关系数据状态::已复用;
            for(const auto& source:requested) {
                const auto found=std::find_if(existing.来源组.begin(),existing.来源组.end(),
                    [&](const auto& item){return item.来源==source;});
                if(found==existing.来源组.end())
                    throw 二次关系失败{二次关系数据状态::内部不一致};
                out.来源组.push_back(*found);
            }
            return out;
        }

        L1所有者范围写集请求 writeSet{
            L1所有者范围CRUD合同版本,expectedGeneration,r.写入头.幂等身份};
        const auto&t=secondary_relation_layout_->类型;
        std::uint32_t nodeKey=1,edgeKey=0x61001;
        const auto edge=[&](Ref source,Ref target,稳定编码 type) {
            writeSet.关系.push_back({Key{edgeKey++},source,target,Ref{type},1});
        };
        for(const auto& source:missing) {
            const Key record{nodeKey++};
            writeSet.节点.push_back({record,节点种类::普通,std::nullopt});
            edge(Ref{r.RC.值},Ref{record},t.来源成员);
            std::visit([&](const auto& value) {
                if constexpr(std::is_same_v<std::decay_t<decltype(value)>,特征信息身份>)
                    edge(Ref{record},Ref{value.编码},t.来源F);
                else if constexpr(std::is_same_v<std::decay_t<decltype(value)>,状态使用绑定身份>)
                    edge(Ref{record},Ref{value.编码},t.来源B);
                else edge(Ref{record},Ref{value.值},t.来源概念);
            },source.来源);
        }
        二次规范化写集(writeSet);
        if(replay&&*first.首次规范化写集!=writeSet)
            throw 二次关系失败{二次关系数据状态::幂等冲突};

        dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(writeSet);
        if(!二次提交状态符合(saved,replay))
            throw 二次关系失败{映射二次写入(saved.状态)};
        const auto& published=replay ? *first.首次写入结果 : saved;
        out.首次H=published.事实代次;
        out.正式回执=二次见证(published,replay);
        const auto tail=当前代次();
        const auto readback=读取二次关系来源(
            {2,{1,tail},r.RC});
        if(!readback.成功())throw 二次关系失败{readback.状态};
        for(const auto& source:requested) {
            const auto found=std::find_if(readback.来源组.begin(),readback.来源组.end(),
                [&](const auto& item){return item.来源==source;});
            if(found==readback.来源组.end())
                throw 二次关系失败{二次关系数据状态::内部不一致};
            out.来源组.push_back(*found);
        }
        out.Gread=tail;
        out.状态=replay ? 二次关系数据状态::精确重放 : 二次关系数据状态::已变更;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.来源组.clear();out.用途组.clear();}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.来源组.clear();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.来源组.clear();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.来源组.clear();}return out;
}

二次关系关联结果 概念树类数据服务::记录二次关系实际采用(const 二次关系用途请求&r){
    二次关系关联结果 out;bool dispatched=false;
    try {
        std::scoped_lock lock(mutex_);
        if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||
           !r.写入头.期望事实代次||!有效(r.写入头.幂等身份)||
           !有效(r.RC.值)||!r.业务标识||r.用途角色<1||r.用途角色>4||
           r.时间<0||!有效(r.业务依据))
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        const auto g=当前代次();out.Gread=g;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});
        const bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay&&!二次首次发布已确认(first))
            throw 二次关系失败{二次关系数据状态::发布未知};
        if(!replay&&first.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ?
                二次关系数据状态::资源失败 : 二次关系数据状态::内部不一致};
        if(!replay&&g!=r.写入头.期望事实代次)
            throw 二次关系失败{二次关系数据状态::事实代次漂移};
        const auto expectedGeneration=r.写入头.期望事实代次;
        核验外部节点(r.业务依据,g);
        const auto existing=读取二次关系用途({2,{1,g},r.RC});
        if(!existing.成功())throw 二次关系失败{existing.状态};
        const auto same=std::find_if(existing.用途组.begin(),existing.用途组.end(),
            [&](const auto& item){return item.业务标识==r.业务标识&&
                item.RC==r.RC&&item.用途角色==r.用途角色;});
        if(same!=existing.用途组.end()) {
            if(same->时间!=r.时间||same->业务依据!=r.业务依据)
                throw 二次关系失败{二次关系数据状态::引用冲突};
            if(!replay) {
                out.Gread=g;out.用途组={*same};
                out.状态=二次关系数据状态::已复用;
                return out;
            }
        }

        const auto&t=secondary_relation_layout_->类型;
        L1所有者范围写集请求 writeSet{
            L1所有者范围CRUD合同版本,expectedGeneration,r.写入头.幂等身份};
        const Key record{1};
        writeSet.节点.push_back({record,节点种类::普通,std::nullopt});
        std::uint32_t edgeKey=0x71001,valueKey=0x72001;
        const auto edge=[&](Ref source,Ref target,稳定编码 type) {
            writeSet.关系.push_back({Key{edgeKey++},source,target,Ref{type},1});
        };
        const auto value=[&](稳定编码 type,L1所有者范围原始值材料 material) {
            const Key key{valueKey++};
            writeSet.值.push_back({key,Ref{record},Ref{type},std::move(material),Ref{record}});
            writeSet.属性槽变更.push_back({Ref{record},Ref{type},key});
        };
        edge(Ref{r.RC.值},Ref{record},t.用途成员);
        edge(Ref{record},Ref{r.RC.值},t.用途目标);
        edge(Ref{record},Ref{r.业务依据},t.用途业务依据);
        value(t.用途业务标识,std::vector<std::uint64_t>{r.业务标识});
        value(t.用途角色,std::int64_t{r.用途角色});
        value(t.用途时间,r.时间);
        二次规范化写集(writeSet);
        if(replay&&*first.首次规范化写集!=writeSet)
            throw 二次关系失败{二次关系数据状态::幂等冲突};

        dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(writeSet);
        if(!二次提交状态符合(saved,replay))
            throw 二次关系失败{映射二次写入(saved.状态)};
        const auto& published=replay ? *first.首次写入结果 : saved;
        out.首次H=published.事实代次;
        out.正式回执=二次见证(published,replay);
        const auto tail=当前代次();
        const auto readback=读取二次关系用途(
            {2,{1,tail},r.RC});
        if(!readback.成功())throw 二次关系失败{readback.状态};
        const auto written=std::find_if(readback.用途组.begin(),readback.用途组.end(),
            [&](const auto& item){return item.业务标识==r.业务标识&&
                item.RC==r.RC&&item.用途角色==r.用途角色&&item.时间==r.时间&&
                item.业务依据==r.业务依据;});
        if(written==readback.用途组.end())
            throw 二次关系失败{二次关系数据状态::内部不一致};
        out.用途组={*written};out.Gread=tail;
        out.状态=replay ? 二次关系数据状态::精确重放 : 二次关系数据状态::已变更;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.用途组.clear();out.来源组.clear();}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.用途组.clear();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.用途组.clear();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.用途组.clear();}return out;
}

二次关系图结果 概念树类数据服务::读取二次关系概念图(const 二次关系图读取请求&r) const {
    二次关系图结果 out;out.Gread=r.读取头.Gread;
    try{if(r.版本!=2||r.读取头.合同版本!=1||!r.读取头.Gread)throw 二次关系失败{二次关系数据状态::入口拒绝};守卫代次(r.读取头.Gread);const auto ids=枚举二次关系身份(r.读取头.Gread);std::set<std::uint64_t> known;for(const auto id:ids)known.insert(id.值.值);for(const auto id:ids){std::vector<概念树概念身份> path{id};auto f=读取二次关系内部(id,r.读取头.Gread,path);for(const auto&e:f.自有关系)if(e.类型==pure_layout_->类型.直接上位&&known.contains(e.源.值)&&known.contains(e.目标.值))out.直接边.push_back(e);out.RC组.push_back(std::move(f));}std::sort(out.直接边.begin(),out.直接边.end(),[](const auto&a,const auto&b){return a.编码.值<b.编码.值;});std::map<std::uint64_t,std::vector<std::uint64_t>> children;for(const auto&e:out.直接边)children[e.源.值].push_back(e.目标.值);std::set<std::uint64_t> gray,black;std::function<void(std::uint64_t)> dfs=[&](auto x){if(gray.contains(x))throw 二次关系失败{二次关系数据状态::形成环};if(black.contains(x))return;gray.insert(x);for(auto y:children[x])dfs(y);gray.erase(x);black.insert(x);};for(auto id:known)dfs(id);out.状态=二次关系数据状态::已读取;守卫代次(r.读取头.Gread);}catch(const 二次关系失败&e){out.状态=e.状态;out.RC组.clear();out.直接边.clear();}catch(const 失败&e){out.状态=映射二次失败(e.状态);out.RC组.clear();out.直接边.clear();}catch(const std::bad_alloc&){out.状态=二次关系数据状态::资源失败;out.RC组.clear();out.直接边.clear();}catch(...){out.状态=二次关系数据状态::内部不一致;out.RC组.clear();out.直接边.clear();}return out;
}

二次关系治理结果 概念树类数据服务::替换二次关系父组(const 二次关系父组请求&r){
    二次关系治理结果 out;bool dispatched=false;
    try {
        std::scoped_lock lock(mutex_);
        if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||
           !r.写入头.期望事实代次||!有效(r.写入头.幂等身份)||
           !有效(r.RC.值))
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        const auto g=当前代次();out.Gread=g;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});
        const bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay&&!二次首次发布已确认(first))
            throw 二次关系失败{二次关系数据状态::发布未知};
        if(!replay&&first.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ?
                二次关系数据状态::资源失败 : 二次关系数据状态::内部不一致};
        if(!replay&&g!=r.写入头.期望事实代次)
            throw 二次关系失败{二次关系数据状态::事实代次漂移};
        std::vector<概念树概念身份> path{r.RC};
        const auto before=读取二次关系内部(r.RC,g,path);
        std::vector<二次关系关系见证> actual;
        for(const auto& edge:before.自有关系)
            if(edge.类型==pure_layout_->类型.直接上位&&edge.目标==r.RC.值)
                actual.push_back(edge);
        auto expected=r.预期直接父边;
        const auto byId=[](const auto&a,const auto&b){return a.编码.值<b.编码.值;};
        std::sort(actual.begin(),actual.end(),byId);
        std::sort(expected.begin(),expected.end(),byId);
        if(!replay&&actual!=expected)throw 二次关系失败{二次关系数据状态::引用冲突};

        auto parents=r.新父组;
        std::sort(parents.begin(),parents.end(),身份小于);
        if(std::adjacent_find(parents.begin(),parents.end())!=parents.end()||
           std::find(parents.begin(),parents.end(),r.RC)!=parents.end())
            throw 二次关系失败{二次关系数据状态::形成环};
        const auto graph=读取二次关系概念图({2,{1,g}});
        if(!graph.成功())throw 二次关系失败{graph.状态};
        std::map<std::uint64_t,std::vector<std::uint64_t>> children;
        for(const auto& edge:graph.直接边)children[edge.源.值].push_back(edge.目标.值);
        std::set<std::uint64_t> descendants;
        std::function<void(std::uint64_t)> visit=[&](std::uint64_t id) {
            if(!descendants.insert(id).second)return;
            for(const auto child:children[id])visit(child);
        };
        visit(r.RC.值.值);
        for(const auto parent:parents) {
            if(descendants.contains(parent.值.值))
                throw 二次关系失败{二次关系数据状态::形成环};
            std::vector<概念树概念身份> parentPath{parent};
            const auto parentFact=读取二次关系内部(parent,g,parentPath);
            for(const auto& parentAtom:parentFact.规范形.原子组) {
                const bool contained=std::any_of(before.规范形.原子组.begin(),
                    before.规范形.原子组.end(),[&](const auto& childAtom) {
                        return 二次原子收窄(childAtom,parentAtom);
                    });
                if(!contained)throw 二次关系失败{二次关系数据状态::引用冲突};
            }
        }

        L1所有者范围写集请求 writeSet;
        if(replay) writeSet=*first.首次规范化写集;
        else {
            writeSet={L1所有者范围CRUD合同版本,r.写入头.期望事实代次,r.写入头.幂等身份};
            for(const auto& edge:actual)writeSet.退出事实.push_back(edge.编码);
            std::uint32_t edgeKey=0x81001;
            for(const auto parent:parents)
                writeSet.关系.push_back({Key{edgeKey++},Ref{parent.值},Ref{r.RC.值},
                    Ref{pure_layout_->类型.直接上位},1});
            二次规范化写集(writeSet);
        }
        dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(writeSet);
        if(!二次提交状态符合(saved,replay))
            throw 二次关系失败{映射二次写入(saved.状态)};
        const auto& published=replay ? *first.首次写入结果 : saved;
        out.首次H=published.事实代次;
        out.正式回执=二次见证(published,replay);
        out.Gread=当前代次();
        std::vector<概念树概念身份> readPath{r.RC};
        out.事实=读取二次关系内部(
            r.RC,out.Gread,readPath);
        for(const auto& edge:out.事实->自有关系)
            if(edge.类型==pure_layout_->类型.直接上位&&edge.目标==r.RC.值)
                out.新直接边.push_back(edge);
        out.状态=replay ? 二次关系数据状态::精确重放 : 二次关系数据状态::已变更;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.事实.reset();out.新直接边.clear();}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.事实.reset();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.事实.reset();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.事实.reset();}return out;
}

二次关系治理结果 概念树类数据服务::迁移二次关系生命周期(const 二次关系生命周期请求&r){
    二次关系治理结果 out;bool dispatched=false;
    try {
        std::scoped_lock lock(mutex_);
        const bool transition=
            (r.预期==概念树生命周期状态::活跃&&r.目标==概念树生命周期状态::冷却)||
            (r.预期==概念树生命周期状态::冷却&&
                (r.目标==概念树生命周期状态::活跃||r.目标==概念树生命周期状态::退役))||
            (r.预期==概念树生命周期状态::退役&&r.目标==概念树生命周期状态::活跃);
        if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||
           !r.写入头.期望事实代次||!有效(r.写入头.幂等身份)||
           !有效(r.RC.值)||!有效(r.治理依据)||!transition)
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        const auto g=当前代次();out.Gread=g;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});
        const bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay&&!二次首次发布已确认(first))
            throw 二次关系失败{二次关系数据状态::发布未知};
        if(!replay&&first.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ?
                二次关系数据状态::资源失败 : 二次关系数据状态::内部不一致};
        if(!replay&&g!=r.写入头.期望事实代次)
            throw 二次关系失败{二次关系数据状态::事实代次漂移};
        核验外部节点(r.治理依据,g);
        std::vector<概念树概念身份> path{r.RC};
        const auto before=读取二次关系内部(r.RC,g,path);
        if(!replay&&before.治理状态!=r.预期)
            throw 二次关系失败{二次关系数据状态::引用冲突};
        const auto life=std::find_if(before.自有值.begin(),before.自有值.end(),
            [&](const auto& value){return value.所属节点==r.RC.值&&
                value.属性类型==pure_layout_->类型.生命周期;});
        if(life==before.自有值.end())
            throw 二次关系失败{二次关系数据状态::内部不一致};
        L1所有者范围写集请求 writeSet;
        if(replay) writeSet=*first.首次规范化写集;
        else {
            writeSet={L1所有者范围CRUD合同版本,r.写入头.期望事实代次,r.写入头.幂等身份};
            writeSet.退出事实.push_back(life->编码);
            const Key value{0x82001};
            writeSet.值.push_back({value,Ref{r.RC.值},Ref{pure_layout_->类型.生命周期},
                std::int64_t{static_cast<std::uint8_t>(r.目标)},Ref{r.RC.值}});
            writeSet.属性槽变更.push_back(
                {Ref{r.RC.值},Ref{pure_layout_->类型.生命周期},value});
            二次规范化写集(writeSet);
        }
        dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(writeSet);
        if(!二次提交状态符合(saved,replay))
            throw 二次关系失败{映射二次写入(saved.状态)};
        const auto& published=replay ? *first.首次写入结果 : saved;
        out.首次H=published.事实代次;
        out.正式回执=二次见证(published,replay);
        out.Gread=当前代次();
        std::vector<概念树概念身份> readPath{r.RC};
        out.事实=读取二次关系内部(
            r.RC,out.Gread,readPath);
        if(out.事实->治理状态!=r.目标)
            throw 二次关系失败{二次关系数据状态::内部不一致};
        out.状态=replay ? 二次关系数据状态::精确重放 : 二次关系数据状态::已变更;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.事实.reset();}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.事实.reset();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.事实.reset();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.事实.reset();}return out;
}

二次关系治理结果 概念树类数据服务::释放二次关系形成来源(const 二次关系来源释放请求&r){
    二次关系治理结果 out;bool dispatched=false;
    try {
        std::scoped_lock lock(mutex_);
        if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||
           !r.写入头.期望事实代次||!有效(r.写入头.幂等身份)||
           !有效(r.RC.值)||r.预期来源.empty())
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        const auto g=当前代次();out.Gread=g;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});
        const bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay&&!二次首次发布已确认(first))
            throw 二次关系失败{二次关系数据状态::发布未知};
        if(!replay&&first.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ?
                二次关系数据状态::资源失败 : 二次关系数据状态::内部不一致};
        if(!replay&&g!=r.写入头.期望事实代次)
            throw 二次关系失败{二次关系数据状态::事实代次漂移};
        const auto expectedGeneration=r.写入头.期望事实代次;
        auto expected=r.预期来源;
        const auto byId=[](const auto&a,const auto&b){return a.记录.值<b.记录.值;};
        std::sort(expected.begin(),expected.end(),byId);
        if(std::adjacent_find(expected.begin(),expected.end(),
            [](const auto&a,const auto&b){return a.记录==b.记录;})!=expected.end())
            throw 二次关系失败{二次关系数据状态::引用冲突};
        L1所有者范围写集请求 writeSet;
        if(replay) writeSet=*first.首次规范化写集;
        else {
            const auto current=读取二次关系来源({2,{1,g},r.RC});
            if(!current.成功())throw 二次关系失败{current.状态};
            for(const auto& item:expected) {
                const auto found=std::find(current.来源组.begin(),current.来源组.end(),item);
                if(found==current.来源组.end())
                    throw 二次关系失败{二次关系数据状态::引用冲突};
            }
            writeSet={L1所有者范围CRUD合同版本,expectedGeneration,r.写入头.幂等身份};
            for(const auto& item:expected)
                writeSet.退出事实.insert(writeSet.退出事实.end(),
                    {item.记录,item.成员关系,item.目标关系});
            std::sort(writeSet.退出事实.begin(),writeSet.退出事实.end(),编码小于);
            writeSet.退出事实.erase(
                std::unique(writeSet.退出事实.begin(),writeSet.退出事实.end()),
                writeSet.退出事实.end());
            二次规范化写集(writeSet);
        }
        dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(writeSet);
        if(!二次提交状态符合(saved,replay))
            throw 二次关系失败{映射二次写入(saved.状态)};
        const auto& published=replay ? *first.首次写入结果 : saved;
        out.首次H=published.事实代次;
        out.正式回执=二次见证(published,replay);
        out.Gread=当前代次();
        std::vector<概念树概念身份> readPath{r.RC};
        out.事实=读取二次关系内部(
            r.RC,out.Gread,readPath);
        out.状态=replay ? 二次关系数据状态::精确重放 : 二次关系数据状态::已变更;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.事实.reset();}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.事实.reset();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.事实.reset();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.事实.reset();}return out;
}

二次关系治理结果 概念树类数据服务::退出二次关系概念(const 二次关系退出请求&r){
    二次关系治理结果 out;bool dispatched=false;
    try {
        std::scoped_lock lock(mutex_);
        if(!secondary_relation_layout_||r.版本!=2||r.写入头.合同版本!=1||
           !r.写入头.期望事实代次||!有效(r.写入头.幂等身份)||
           !有效(r.RC.值))
            throw 二次关系失败{二次关系数据状态::入口拒绝};
        const auto g=当前代次();out.Gread=g;
        const auto first=port_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本,r.写入头.幂等身份});
        const bool replay=first.状态==L1所有者范围读取状态::成功;
        if(replay&&!二次首次发布已确认(first))
            throw 二次关系失败{二次关系数据状态::发布未知};
        if(!replay&&first.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{first.状态==L1所有者范围读取状态::资源失败 ?
                二次关系数据状态::资源失败 : 二次关系数据状态::内部不一致};
        if(!replay&&g!=r.写入头.期望事实代次)
            throw 二次关系失败{二次关系数据状态::事实代次漂移};
        if(replay) {
            const auto& writeSet=*first.首次规范化写集;
            if(writeSet.合同版本!=L1所有者范围CRUD合同版本||
               writeSet.期望事实代次!=r.写入头.期望事实代次||
               writeSet.写入幂等身份!=r.写入头.幂等身份||
               !std::binary_search(writeSet.退出事实.begin(),writeSet.退出事实.end(),
                                   r.RC.值,编码小于))
                throw 二次关系失败{二次关系数据状态::幂等冲突};
            dispatched=true;
            const auto saved=port_.提交所有者范围中性写集(writeSet);
            if(!二次提交状态符合(saved,true))
                throw 二次关系失败{映射二次写入(saved.状态)};
            const auto& published=*first.首次写入结果;
            out.首次H=published.事实代次;
            out.正式回执=二次见证(published,true);
            out.Gread=当前代次();
            const auto current=读取任一当前事实(l1_,r.RC.值);
            if(current.读取事实代次!=out.Gread||
               current.状态!=L1所有者范围读取状态::未找到)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            for(const auto&[key,id]:published.新编码映射) {
                const auto raw=原始事实(id,out.Gread);
                if(const auto* edge=std::get_if<E>(&raw))
                    out.新直接边.push_back({edge->编码,edge->源节点,edge->目标节点,
                        edge->关系类型节点,edge->角色或顺序,投影纯生命(*edge)});
            }
            std::sort(out.新直接边.begin(),out.新直接边.end(),
                [](const auto&a,const auto&b){return a.编码.值<b.编码.值;});
            out.状态=二次关系数据状态::精确重放;
            return out;
        }
        const auto expectedGeneration=r.写入头.期望事实代次;
        std::vector<概念树概念身份> path{r.RC};
        auto before=读取二次关系内部(r.RC,g,path);
        if(before.治理状态!=概念树生命周期状态::退役)
            throw 二次关系失败{二次关系数据状态::引用冲突};
        const auto sources=读取二次关系来源({2,{1,g},r.RC});
        if(!sources.成功())throw 二次关系失败{sources.状态};
        if(!sources.来源组.empty())throw 二次关系失败{二次关系数据状态::引用冲突};
        const auto uses=读取二次关系用途({2,{1,g},r.RC});
        if(!uses.成功())throw 二次关系失败{uses.状态};
        const auto graph=读取二次关系概念图({2,{1,g}});
        if(!graph.成功())throw 二次关系失败{graph.状态};
        for(const auto& fact:graph.RC组)
            if(fact.身份!=r.RC)
                for(const auto& reference:fact.定义引用)
                    if(reference.种类==二次关系引用种类::子RC&&
                       std::get<概念树概念身份>(reference.目标)==r.RC)
                        throw 二次关系失败{二次关系数据状态::引用冲突};

        L1所有者范围写集请求 writeSet{
            L1所有者范围CRUD合同版本,expectedGeneration,r.写入头.幂等身份};
        std::vector<稳定编码> protectedNodes;
        for(const auto& node:before.自有节点) {
            writeSet.退出事实.push_back(node.编码);
            protectedNodes.push_back(node.编码);
        }
        for(const auto& edge:before.自有关系)writeSet.退出事实.push_back(edge.编码);
        for(const auto& value:before.自有值)writeSet.退出事实.push_back(value.编码);
        for(const auto& use:uses.用途组) {
            protectedNodes.push_back(use.记录);
            writeSet.退出事实.insert(writeSet.退出事实.end(),
                {use.记录,use.成员关系,use.概念关系,use.依据关系});
            const auto values=完整属性值组(use.记录,g);
            for(const auto& value:values)writeSet.退出事实.push_back(value.编码);
        }

        std::vector<概念树概念身份> parents;
        for(const auto& edge:before.自有关系)
            if(edge.类型==pure_layout_->类型.直接上位&&edge.目标==r.RC.值)
                parents.emplace_back(edge.源);
        std::sort(parents.begin(),parents.end(),身份小于);
        std::set<std::pair<std::uint64_t,std::uint64_t>> existingEdges;
        for(const auto& edge:graph.直接边)
            existingEdges.emplace(edge.源.值,edge.目标.值);
        std::set<std::pair<std::uint64_t,std::uint64_t>> replacements;
        for(const auto& edge:graph.直接边) {
            if(edge.源!=r.RC.值)continue;
            writeSet.退出事实.push_back(edge.编码);
            for(const auto parent:parents) {
                const auto pair=std::pair{parent.值.值,edge.目标.值};
                if(parent.值!=edge.目标&&!existingEdges.contains(pair))
                    replacements.insert(pair);
            }
        }
        std::uint32_t edgeKey=0x91001;
        for(const auto& [parent,child]:replacements)
            writeSet.关系.push_back({Key{edgeKey++},Ref{稳定编码{parent}},
                Ref{稳定编码{child}},Ref{pure_layout_->类型.直接上位},1});
        std::sort(writeSet.退出事实.begin(),writeSet.退出事实.end(),编码小于);
        writeSet.退出事实.erase(
            std::unique(writeSet.退出事实.begin(),writeSet.退出事实.end()),
            writeSet.退出事实.end());

        if(!replay) {
            std::sort(protectedNodes.begin(),protectedNodes.end(),编码小于);
            protectedNodes.erase(std::unique(protectedNodes.begin(),protectedNodes.end()),
                protectedNodes.end());
            std::set<std::uint64_t> allowed;
            for(const auto id:writeSet.退出事实)allowed.insert(id.值);
            const auto mapReferenceFailure=[](L1节点当前完整引用读取状态_v2 state) {
                switch(state) {
                case L1节点当前完整引用读取状态_v2::事实代次漂移:
                    return 二次关系数据状态::事实代次漂移;
                case L1节点当前完整引用读取状态_v2::资源失败:
                    return 二次关系数据状态::资源失败;
                case L1节点当前完整引用读取状态_v2::未找到:
                    return 二次关系数据状态::引用冲突;
                default:return 二次关系数据状态::内部不一致;
                }
            };
            std::set<std::uint64_t> seenReferences;
            for(const auto node:protectedNodes) {
                const L1节点当前完整引用读取请求_v2 request{
                    L1节点当前完整引用读取合同版本,node,g};
                const auto read=l1_.读取节点全部当前引用_v2(request);
                if(read.状态 != L1节点当前完整引用读取状态_v2::成功
                    || read.合同版本 != L1节点当前完整引用读取合同版本
                    || read.节点 != request.节点
                    || read.期望事实代次 != request.期望事实代次
                    || read.读取事实代次 != request.期望事实代次)
                    throw 二次关系失败{mapReferenceFailure(read.状态)};
                for(const auto& reference:read.引用) {
                    const auto id=std::visit([](const auto& value){return value.编码;},reference);
                    if(!seenReferences.insert(id.值).second)continue;
                    if(!allowed.contains(id.值))
                        throw 二次关系失败{二次关系数据状态::引用冲突};
                }
            }
        }

        二次规范化写集(writeSet);
        if(replay&&*first.首次规范化写集!=writeSet)
            throw 二次关系失败{二次关系数据状态::幂等冲突};
        dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(writeSet);
        if(!二次提交状态符合(saved,replay))
            throw 二次关系失败{映射二次写入(saved.状态)};
        const auto& published=replay ? *first.首次写入结果 : saved;
        out.首次H=published.事实代次;
        out.正式回执=二次见证(published,replay);
        out.Gread=当前代次();
        const auto current=读取任一当前事实(l1_,r.RC.值);
        if(current.读取事实代次!=out.Gread||
           current.状态!=L1所有者范围读取状态::未找到)
            throw 二次关系失败{二次关系数据状态::内部不一致};
        for(const auto&[key,id]:published.新编码映射) {
            const auto raw=原始事实(id,out.Gread);
            if(const auto* edge=std::get_if<E>(&raw)) {
                out.新直接边.push_back({edge->编码,edge->源节点,edge->目标节点,
                    edge->关系类型节点,edge->角色或顺序,投影纯生命(*edge)});
            }
        }
        std::sort(out.新直接边.begin(),out.新直接边.end(),
            [](const auto&a,const auto&b){return a.编码.值<b.编码.值;});
        out.状态=replay ? 二次关系数据状态::精确重放 : 二次关系数据状态::已变更;
    }catch(const 二次关系失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : e.状态;out.事实.reset();out.新直接边.clear();}catch(const 失败&e){out.状态=dispatched ? 二次关系数据状态::发布未知 : 映射二次失败(e.状态);out.事实.reset();}catch(const std::bad_alloc&){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::资源失败;out.事实.reset();}catch(...){out.状态=dispatched ? 二次关系数据状态::发布未知 : 二次关系数据状态::内部不一致;out.事实.reset();}return out;
}

二次关系约束读取结果 概念树类数据服务::读取二次关系约束定义(
    const 二次关系约束读取请求& r) const {
    二次关系约束读取结果 out;out.Gread=r.读取头.Gread;
    if(!secondary_relation_layout_){out.状态=二次关系数据状态::格式不支持;return out;}
    if(r.版本!=2||r.读取头.合同版本!=1||!r.读取头.Gread||!有效(r.概念.值))return out;
    try {
        守卫代次(r.读取头.Gread);
        if(!two_group_definition_layout_)
            throw 二次关系失败{二次关系数据状态::格式不支持};
        const auto& two=*two_group_definition_layout_;
        const auto twoEdges=完整关系组(r.概念.值,two.类型.两组定义成员,
            false,r.读取头.Gread);
        if(!twoEdges.empty()) {
            if(twoEdges.size()!=1||twoEdges.front().角色或顺序!=1)
                throw 二次关系失败{二次关系数据状态::内部不一致};
            特征值域比较数据服务 comparison(*this,features_,values_);
            const 存在概念两组完整读取请求_v2 request{
                2,r.读取头.Gread,r.概念};
            const auto ec=读取存在概念两组完整定义(request,comparison);
            if(!ec.成功(request)) {
                switch(ec.状态) {
                case 存在概念两组完整读取状态_v2::未找到:
                    throw 二次关系失败{二次关系数据状态::未找到};
                case 存在概念两组完整读取状态_v2::概念已退役:
                    throw 二次关系失败{二次关系数据状态::退役命中};
                case 存在概念两组完整读取状态_v2::事实代次漂移:
                    throw 二次关系失败{二次关系数据状态::事实代次漂移};
                case 存在概念两组完整读取状态_v2::资源失败:
                    throw 二次关系失败{二次关系数据状态::资源失败};
                case 存在概念两组完整读取状态_v2::内部不一致:
                    throw 二次关系失败{二次关系数据状态::内部不一致};
                default:throw 二次关系失败{二次关系数据状态::规则不支持};
                }
            }
            out.定义=*ec.事实;out.状态=二次关系数据状态::已读取;
            守卫代次(r.读取头.Gread);return out;
        }
        const 纯概念完整读取请求_v3 request{
            3,r.读取头.Gread,r.概念};
        const auto read=读取纯概念完整(request);
        if(!read.成功(request))throw 二次关系失败{映射纯二次失败(read.状态)};
        const auto& pure=*read.事实;
        const bool fc=std::holds_alternative<纯I64特征概念定义>(pure.定义);
        const auto* ec=std::get_if<通用存在概念定义>(&pure.定义);
        if(fc||(ec&&ec->规则版本==1&&ec->规则==通用存在定义规则::不预设特征&&
                pure.类别==相关概念类别::存在&&pure.直接上位.empty())) {
            if(pure.治理状态==概念树生命周期状态::退役){out.状态=二次关系数据状态::退役命中;return out;}
            out.定义=pure;out.状态=二次关系数据状态::已读取;
            守卫代次(r.读取头.Gread);return out;
        }
        out.状态=二次关系数据状态::规则不支持;return out;
    } catch(const 二次关系失败&e){out.状态=e.状态;}
      catch(const 失败&e){out.状态=映射二次失败(e.状态);}
      catch(const std::bad_alloc&){out.状态=二次关系数据状态::资源失败;}
      catch(...){out.状态=二次关系数据状态::内部不一致;}
    out.定义.reset();return out;
}

} // namespace 海中鱼巣
