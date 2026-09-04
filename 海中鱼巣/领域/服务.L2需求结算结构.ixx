module;

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.服务.L2需求结算结构;

export import 海中鱼巣.领域.服务.L2任务结果消费结构;
import 海中鱼巣.核心.服务.L1事实基座;

#define L2_DEMAND_SETTLEMENT_NO_INCLUDES
export {
#include "L2需求结算结构.数据.h"
}

export namespace 海中鱼巣 {

inline constexpr L1所有者范围建立幂等身份 需求结算所有者建立身份{
    0x4C34'0306'0000'0001ULL};

class L2需求结算结构服务;

class L2需求结算所有者交付 final {
public:
    L2需求结算所有者交付() = delete;
    L2需求结算所有者交付(const L2需求结算所有者交付&) = delete;
    L2需求结算所有者交付(L2需求结算所有者交付&&) noexcept = default;
private:
    explicit L2需求结算所有者交付(L1所有者范围写端口&& 端口) noexcept
        : 写入端口_(std::move(端口)) {}
    friend std::optional<L2需求结算所有者交付> 尝试形成L2需求结算所有者交付(
        const L1事实基座服务&, L1所有者范围交付&&) noexcept;
    friend class L2需求结算结构服务;
    L1所有者范围写端口 写入端口_;
};

std::optional<L2需求结算所有者交付> 尝试形成L2需求结算所有者交付(
    const L1事实基座服务& 第一层服务, L1所有者范围交付&& 原始交付) noexcept {
    try {
        const auto& 建立 = 原始交付.建立结果;
        const bool 成功 = 建立.状态 == L1所有者范围管理状态::成功
            || 建立.状态 == L1所有者范围管理状态::精确重复;
        if (!成功 || 建立.建立幂等身份 != 需求结算所有者建立身份
            || 建立.事实代次 == 0 || !建立.所有者事实 || !原始交付.写入端口
            || 建立.所有者事实->范围种类 != L1所有者范围种类::独占结构范围
            || 建立.所有者事实->退出事实代次
            || 原始交付.写入端口->所有者身份() != 建立.所有者事实->所有者
            || !原始交付.写入端口->有效()
            || !原始交付.写入端口->绑定于(第一层服务)) return std::nullopt;
        const auto 当前 = 第一层服务.读取当前结构所有者(
            {L1所有者范围CRUD合同版本, 建立.所有者事实->所有者});
        if (当前.状态 != L1所有者范围读取状态::成功 || !当前.所有者事实
            || *当前.所有者事实 != *建立.所有者事实) return std::nullopt;
        return L2需求结算所有者交付{std::move(*原始交付.写入端口)};
    } catch (...) { return std::nullopt; }
}

}

namespace 海中鱼巣::L2需求结算结构内部 {

inline constexpr L1所有者范围写入幂等身份 登记幂等身份{
    0x4C32'0306'5245'4731ULL};
inline constexpr std::uint32_t 载荷键=0x0306'2000U,需求关系键=0x0306'2001U;
inline constexpr std::uint32_t 任务关系键=0x0306'2002U,轮次关系键=0x0306'2003U;
inline constexpr std::uint32_t 结果关系键=0x0306'2004U,分配记录关系键=0x0306'2005U;
inline constexpr std::uint32_t 实际状态关系键=0x0306'2006U,目标合同关系键=0x0306'2007U;
inline constexpr std::uint32_t 比较注册关系键=0x0306'2008U,满足记录关系键=0x0306'2009U;
struct 登记定位 final {稳定编码 载荷,需求关系,任务关系,轮次关系,结果关系,分配记录关系;
    稳定编码 实际状态关系,目标合同关系,比较注册关系,满足记录关系;};
inline L1所有者范围写集本地键 键(std::uint32_t v)noexcept{return{v};}
inline std::optional<稳定编码> 取映射(const L1所有者范围写入结果&r,std::uint32_t k)noexcept{for(const auto&[x,v]:r.新编码映射)if(x.值==k)return v;return std::nullopt;}
inline 登记定位 初始化登记(const L1事实基座服务&L1,L1所有者范围写端口&端口){L1所有者范围写集请求 w;const auto first=端口.读取首次写入材料({L1所有者范围首次写入读取合同版本,登记幂等身份});if(first.状态==L1所有者范围读取状态::成功&&first.首次规范化写集)w=*first.首次规范化写集;else{const auto g=L1.读取中性当前事实代次({L1中性CRUD合同版本});if(g.状态!=L1中性读取状态::成功||!g.事实代次)throw std::runtime_error("L2 settlement registration cutoff unavailable");w.期望事实代次=g.事实代次;w.写入幂等身份=登记幂等身份;w.节点.push_back({键(载荷键),节点种类::属性类型,L1所有者范围值表示种类::U64组});for(auto k:{需求关系键,任务关系键,轮次关系键,结果关系键,分配记录关系键,实际状态关系键,目标合同关系键,比较注册关系键,满足记录关系键})w.节点.push_back({键(k),节点种类::普通,std::nullopt});}const auto r=端口.提交所有者范围中性写集(w);if(r.状态!=L1所有者范围写入状态::成功&&r.状态!=L1所有者范围写入状态::精确重复)throw std::runtime_error("L2 settlement registration failed");const auto get=[&](auto k){const auto v=取映射(r,k);if(!v)throw std::runtime_error("L2 settlement registration mapping missing");return*v;};return{get(载荷键),get(需求关系键),get(任务关系键),get(轮次关系键),get(结果关系键),get(分配记录关系键),get(实际状态关系键),get(目标合同关系键),get(比较注册关系键),get(满足记录关系键)};}

inline bool 活跃(const L1所有者范围事实副本&f,std::uint64_t g)noexcept{return std::visit([&](const auto&x){return x.创建事实代次&&x.创建事实代次<=g&&(!x.退出事实代次||*x.退出事实代次>g);},f);}
inline bool 外部事实存在(const L1事实基座服务&L1,稳定编码 id,std::uint64_t g)noexcept{const auto r=L1.读取所有者范围历史事实({L1所有者范围CRUD合同版本,id});return 有效(id)&&r.状态==L1所有者范围读取状态::成功&&r.事实&&活跃(*r.事实,g);}
inline bool 成员关系存在(const L1事实基座服务&L1,const L2成员消费资格材料&q,std::uint64_t g)noexcept{const auto r=L1.读取所有者范围历史事实({L1所有者范围CRUD合同版本,q.成员.关系稳定编码});if(r.状态!=L1所有者范围读取状态::成功||!r.事实||!活跃(*r.事实,g))return false;const auto*x=std::get_if<L1所有者范围关系事实>(&*r.事实);return x&&x->源节点==q.成员.任务.值&&x->目标节点==q.成员.来源需求.值;}

inline bool 事实有效(const L2需求本轮核算事实& f, bool 持久,
    std::uint64_t g = 0) noexcept {
    const bool identity = 持久
        ? f.身份.有效() && L2生命周期完整(f.生命周期)
            && f.生命周期.创建事实代次 <= g
            && (!f.生命周期.退出事实代次 || *f.生命周期.退出事实代次 > g)
        : !f.身份.有效() && !f.生命周期.创建事实代次
            && !f.生命周期.退出事实代次;
    if (!identity || !有效(f.需求.值) || !有效(f.来源任务.值)
        || !有效(f.来源任务轮次.值) || !有效(f.来源正式结果.值)
        || !f.消费分配记录.有效() || !L2成员消费资格材料有效(f.消费资格)
        || f.消费资格.成员.任务 != f.来源任务
        || f.消费资格.成员.来源需求 != f.需求
        || !f.来源共同事实截止 || f.核算规则版本 != 1) return false;
    const bool assigned = f.分配状态 == L2成员消费分配状态::共同引用
        || f.分配状态 == L2成员消费分配状态::已分配;
    const bool denied = f.分配状态 == L2成员消费分配状态::未获分配
        || f.分配状态 == L2成员消费分配状态::结果已耗尽;
    if (!assigned && !denied) return false;
    if (有效(f.目标状态合同.值) && f.目标状态合同 != f.消费资格.目标合同)
        return false;
    if (denied)
        return f.结论 == L2需求本轮核算结论::未获分配 && !f.正式满足记录;
    const bool complete = 有效(f.比较时实际状态.值)
        && 有效(f.目标状态合同.值) && 有效(f.比较注册)
        && f.目标状态合同 == f.消费资格.目标合同;
    if (f.结论 == L2需求本轮核算结论::已满足)
        return complete && f.正式满足记录 && f.正式满足记录->有效();
    if (f.正式满足记录) return false;
    if (f.结论 == L2需求本轮核算结论::未满足
        || f.结论 == L2需求本轮核算结论::不可比较) return complete;
    if (f.结论 == L2需求本轮核算结论::材料不足) return !complete;
    return false;
}

inline void E(std::vector<std::uint64_t>&o,稳定编码 v){o.push_back(v.值);}inline void L(std::vector<std::uint64_t>&o,const L2生命周期&x){o.push_back(x.创建事实代次);o.push_back(x.退出事实代次.value_or(0));}
inline void Q(std::vector<std::uint64_t>&o,const L2成员消费资格材料&q){E(o,q.成员.关系稳定编码);E(o,q.成员.任务.值);E(o,q.成员.来源需求.值);o.push_back(q.成员.来源顺序);L(o,q.成员.生命周期);E(o,q.目标合同.值);E(o,q.资格证据身份);o.push_back(q.资格规则版本);E(o,q.范围合同身份);E(o,q.所有权合同身份);E(o,q.有效性合同身份);o.push_back(q.最低合法分配单位);o.push_back(q.期望分配单位);}
inline std::vector<std::uint64_t> 编码(const L2需求本轮核算事实& f) {
    std::vector<std::uint64_t> o;
    E(o, f.需求.值); E(o, f.来源任务.值); E(o, f.来源任务轮次.值);
    E(o, f.来源正式结果.值); E(o, f.消费分配记录.值); Q(o, f.消费资格);
    o.push_back(static_cast<std::uint64_t>(f.分配状态));
    o.push_back(static_cast<std::uint64_t>(f.结论));
    o.push_back(f.正式满足记录 ? 1 : 0);
    o.push_back(f.正式满足记录 ? f.正式满足记录->值.值 : 0);
    E(o, f.比较时实际状态.值); E(o, f.目标状态合同.值);
    E(o, f.比较注册); o.push_back(f.来源共同事实截止);
    o.push_back(f.核算规则版本);
    return o;
}
inline bool R(const std::vector<std::uint64_t>&a,std::size_t&i,std::uint64_t&v){if(i>=a.size())return false;v=a[i++];return true;}inline bool RE(const std::vector<std::uint64_t>&a,std::size_t&i,稳定编码&v){return R(a,i,v.值);}inline bool RL(const std::vector<std::uint64_t>&a,std::size_t&i,L2生命周期&x){std::uint64_t c=0,e=0;if(!R(a,i,c)||!R(a,i,e))return false;x={c,e?std::optional<std::uint64_t>{e}:std::nullopt};return true;}inline bool RQ(const std::vector<std::uint64_t>&a,std::size_t&i,L2成员消费资格材料&q){std::uint64_t v=0;return RE(a,i,q.成员.关系稳定编码)&&RE(a,i,q.成员.任务.值)&&RE(a,i,q.成员.来源需求.值)&&R(a,i,q.成员.来源顺序)&&RL(a,i,q.成员.生命周期)&&RE(a,i,q.目标合同.值)&&RE(a,i,q.资格证据身份)&&R(a,i,v)&&(q.资格规则版本=static_cast<std::uint32_t>(v),true)&&RE(a,i,q.范围合同身份)&&RE(a,i,q.所有权合同身份)&&RE(a,i,q.有效性合同身份)&&R(a,i,q.最低合法分配单位)&&R(a,i,q.期望分配单位);}
inline bool 解码(const std::vector<std::uint64_t>&a,L2需求本轮核算事实&f){std::size_t i=0;std::uint64_t v=0,c=0,flag=0,id=0;if(!RE(a,i,f.需求.值)||!RE(a,i,f.来源任务.值)||!RE(a,i,f.来源任务轮次.值)||!RE(a,i,f.来源正式结果.值)||!RE(a,i,f.消费分配记录.值)||!RQ(a,i,f.消费资格)||!R(a,i,v))return false;f.分配状态=static_cast<L2成员消费分配状态>(v);if(!R(a,i,c)||!R(a,i,flag)||!R(a,i,id)||flag>1||(flag==0&&id))return false;f.结论=static_cast<L2需求本轮核算结论>(c);if(flag)f.正式满足记录=L2需求满足记录身份{稳定编码{id}};if(!RE(a,i,f.比较时实际状态.值)||!RE(a,i,f.目标状态合同.值)||!RE(a,i,f.比较注册)||!R(a,i,f.来源共同事实截止)||!R(a,i,v))return false;f.核算规则版本=static_cast<std::uint32_t>(v);return i==a.size();}
inline L2需求本轮核算写入状态 映射(L1所有者范围写入状态 s)noexcept{switch(s){case L1所有者范围写入状态::成功:return L2需求本轮核算写入状态::已形成;case L1所有者范围写入状态::精确重复:return L2需求本轮核算写入状态::精确重复;case L1所有者范围写入状态::入口拒绝:return L2需求本轮核算写入状态::入口拒绝;case L1所有者范围写入状态::许可拒绝:return L2需求本轮核算写入状态::许可拒绝;case L1所有者范围写入状态::事实代次漂移:case L1所有者范围写入状态::未找到:case L1所有者范围写入状态::已退出:return L2需求本轮核算写入状态::当前性漂移;case L1所有者范围写入状态::引用冲突:return L2需求本轮核算写入状态::分配记录漂移;case L1所有者范围写入状态::幂等冲突:return L2需求本轮核算写入状态::幂等冲突;case L1所有者范围写入状态::资源失败:return L2需求本轮核算写入状态::资源失败;default:return L2需求本轮核算写入状态::内部错误;}}

} // namespace 海中鱼巣::L2需求结算结构内部

export namespace 海中鱼巣 {

class L2需求结算结构服务 final {
public:
    L2需求结算结构服务(const L1事实基座服务& 第一层服务,L2需求结算所有者交付&& 交付)
        :第一层服务_(第一层服务),第一层写入端口_(验证并移动交付(第一层服务,交付)),登记_(L2需求结算结构内部::初始化登记(第一层服务_,第一层写入端口_)){}

    L2需求本轮核算写入结果 写入或读取需求本轮核算(const L2需求本轮核算写入请求&请求)noexcept{
        std::lock_guard<std::mutex>lock(互斥体_);const auto fail=[](auto s){L2需求本轮核算写入结果 r;r.状态=s;return r;};try{if(请求.请求头.合同版本!=L2结构合同版本||!请求.请求头.期望事实代次||!L2结构幂等身份有效(请求.幂等身份)||请求.幂等身份.值==L2需求结算结构内部::登记幂等身份.值)return fail(L2需求本轮核算写入状态::入口拒绝);const auto&f=请求.候选事实;if(!L2需求结算结构内部::事实有效(f,false))return fail(L2需求本轮核算写入状态::入口拒绝);const auto g=请求.请求头.期望事实代次;if(!L2需求结算结构内部::外部事实存在(第一层服务_,f.消费分配记录.值,g))return fail(L2需求本轮核算写入状态::分配记录漂移);if(!L2需求结算结构内部::成员关系存在(第一层服务_,f.消费资格,g)||!L2需求结算结构内部::外部事实存在(第一层服务_,f.需求.值,g)||!L2需求结算结构内部::外部事实存在(第一层服务_,f.来源任务.值,g)||!L2需求结算结构内部::外部事实存在(第一层服务_,f.来源任务轮次.值,g)||!L2需求结算结构内部::外部事实存在(第一层服务_,f.来源正式结果.值,g))return fail(L2需求本轮核算写入状态::当前性漂移);for(const auto id:{f.比较时实际状态.值,f.目标状态合同.值,f.比较注册})if(有效(id)&&!L2需求结算结构内部::外部事实存在(第一层服务_,id,g))return fail(L2需求本轮核算写入状态::当前性漂移);if(f.正式满足记录&&!L2需求结算结构内部::外部事实存在(第一层服务_,f.正式满足记录->值,g))return fail(L2需求本轮核算写入状态::当前性漂移);
            L1所有者范围写集请求 w;
            w.期望事实代次 = g;
            w.写入幂等身份 = {请求.幂等身份.值};
            w.节点.push_back({L1所有者范围写集本地键{1}, 节点种类::普通,
                std::nullopt});
            w.值.push_back({L1所有者范围写集本地键{2},
                L1所有者范围写集本地键{1}, 登记_.载荷,
                L2需求结算结构内部::编码(f), L1所有者范围写集本地键{1}});
            w.属性槽变更.push_back({L1所有者范围写集本地键{1},
                登记_.载荷, L1所有者范围写集本地键{2}});
            std::uint32_t k = 10;
            const auto rel = [&](稳定编码 target, 稳定编码 type) {
                w.关系.push_back({L1所有者范围写集本地键{k++},
                    L1所有者范围写集本地键{1}, target, type, 1});
            };
            rel(f.需求.值, 登记_.需求关系); rel(f.来源任务.值, 登记_.任务关系);
            rel(f.来源任务轮次.值, 登记_.轮次关系);
            rel(f.来源正式结果.值, 登记_.结果关系);
            rel(f.消费分配记录.值, 登记_.分配记录关系);
            if (有效(f.比较时实际状态.值)) rel(f.比较时实际状态.值, 登记_.实际状态关系);
            if (有效(f.目标状态合同.值)) rel(f.目标状态合同.值, 登记_.目标合同关系);
            if (有效(f.比较注册)) rel(f.比较注册, 登记_.比较注册关系);
            if (f.正式满足记录) rel(f.正式满足记录->值, 登记_.满足记录关系);
            const auto wr = 第一层写入端口_.提交所有者范围中性写集(w);
            const auto s = L2需求结算结构内部::映射(wr.状态);
            if (s != L2需求本轮核算写入状态::已形成
                && s != L2需求本轮核算写入状态::精确重复) return fail(s);
            const auto id = L2需求结算结构内部::取映射(wr, 1);
            if (id) {
                auto rr = 读取_已锁定({1, L2需求本轮核算身份{*id}, f.需求,
                    f.来源任务, f.来源任务轮次, wr.事实代次});
                if (rr.成功()) {
                    auto expected = f;
                    expected.身份 = L2需求本轮核算身份{*id};
                    expected.生命周期 = {wr.事实代次, std::nullopt};
                    if (rr.核算事实 && *rr.核算事实 == expected)
                        return {s, std::move(rr.核算事实), std::nullopt,
                            rr.本次正式读回截止};
                }
            }
            L2需求本轮核算写入结果 r;
            r.状态 = L2需求本轮核算写入状态::已可能发布;
            r.提交见证 = L2需求本轮核算提交见证{第一层写入端口_.所有者身份(),
                请求.幂等身份, wr.事实代次, 1};
            return r;
        } catch (const std::bad_alloc&) { return fail(L2需求本轮核算写入状态::资源失败); }
        catch (const std::length_error&) { return fail(L2需求本轮核算写入状态::资源失败); }
        catch (...) { return fail(L2需求本轮核算写入状态::内部错误); }
    }

    L2需求本轮核算读取结果 读取需求本轮核算(const L2需求本轮核算读取请求&请求)const noexcept{std::lock_guard<std::mutex>lock(互斥体_);return 读取_已锁定(请求);}
private:
    std::vector<L1所有者范围关系事实> 关系(稳定编码 src,稳定编码 type,std::uint64_t g)const{const auto x=第一层服务_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,src,type,g});std::vector<L1所有者范围关系事实>o;if(x.状态!=L1所有者范围读取状态::成功)return o;for(const auto&r:x.关系组)if(r.写入所有者==第一层写入端口_.所有者身份()&&r.创建事实代次<=g&&(!r.退出事实代次||*r.退出事实代次>g))o.push_back(r);return o;}
    bool 对应(稳定编码 src,稳定编码 type,稳定编码 dst,std::uint64_t g,bool required=true)const{const auto x=关系(src,type,g);return required ? x.size()==1&&x[0].目标节点==dst&&x[0].角色或顺序==1 : x.empty();}
    L2需求本轮核算读取结果 读取_已锁定(const L2需求本轮核算读取请求&q)const noexcept{const auto fail=[](auto s){L2需求本轮核算读取结果 r;r.状态=s;return r;};try{if(q.合同版本!=1||!q.核算.有效()||!有效(q.需求.值)||!有效(q.来源任务.值)||!有效(q.来源任务轮次.值)||!q.非零读取截止)return fail(L2需求本轮核算读取状态::入口拒绝);const auto nr=第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,q.核算.值});if(nr.状态==L1所有者范围读取状态::资源失败)return fail(L2需求本轮核算读取状态::资源失败);if(nr.状态!=L1所有者范围读取状态::成功||!nr.事实)return fail(L2需求本轮核算读取状态::未找到);const auto*n=std::get_if<L1所有者范围节点事实>(&*nr.事实);if(!n||n->写入所有者!=第一层写入端口_.所有者身份()||n->创建事实代次>q.非零读取截止||(n->退出事实代次&&*n->退出事实代次<=q.非零读取截止))return fail(L2需求本轮核算读取状态::许可拒绝);const auto vr=第一层服务_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本,q.核算.值,q.非零读取截止});if(vr.状态!=L1所有者范围读取状态::成功)return fail(vr.状态==L1所有者范围读取状态::资源失败 ? L2需求本轮核算读取状态::资源失败 : L2需求本轮核算读取状态::内部错误);const std::vector<std::uint64_t>*p=nullptr;for(const auto&v:vr.属性值组)if(v.属性类型节点==登记_.载荷&&v.创建事实代次<=q.非零读取截止&&(!v.退出事实代次||*v.退出事实代次>q.非零读取截止)){if(p)return fail(L2需求本轮核算读取状态::内部错误);p=std::get_if<std::vector<std::uint64_t>>(&v.材料);}if(!p)return fail(L2需求本轮核算读取状态::内部错误);L2需求本轮核算事实 f;if(!L2需求结算结构内部::解码(*p,f)||f.需求!=q.需求||f.来源任务!=q.来源任务||f.来源任务轮次!=q.来源任务轮次)return fail(L2需求本轮核算读取状态::内部错误);f.身份=q.核算;f.生命周期={n->创建事实代次,n->退出事实代次};const auto rel=[&](稳定编码 t,稳定编码 d){return 对应(q.核算.值,t,d,q.非零读取截止);};if(!rel(登记_.需求关系,f.需求.值)||!rel(登记_.任务关系,f.来源任务.值)||!rel(登记_.轮次关系,f.来源任务轮次.值)||!rel(登记_.结果关系,f.来源正式结果.值)||!rel(登记_.分配记录关系,f.消费分配记录.值))return fail(L2需求本轮核算读取状态::内部错误);if(!对应(q.核算.值,登记_.实际状态关系,f.比较时实际状态.值,q.非零读取截止,有效(f.比较时实际状态.值))||!对应(q.核算.值,登记_.目标合同关系,f.目标状态合同.值,q.非零读取截止,有效(f.目标状态合同.值))||!对应(q.核算.值,登记_.比较注册关系,f.比较注册,q.非零读取截止,有效(f.比较注册))||!对应(q.核算.值,登记_.满足记录关系,f.正式满足记录 ? f.正式满足记录->值 : 稳定编码{},q.非零读取截止,f.正式满足记录.has_value()))return fail(L2需求本轮核算读取状态::内部错误);if(!L2需求结算结构内部::事实有效(f,true,q.非零读取截止))return fail(L2需求本轮核算读取状态::内部错误);return{L2需求本轮核算读取状态::已读取,std::move(f),q.非零读取截止};}catch(const std::bad_alloc&){return fail(L2需求本轮核算读取状态::资源失败);}catch(const std::length_error&){return fail(L2需求本轮核算读取状态::资源失败);}catch(...){return fail(L2需求本轮核算读取状态::内部错误);}}
    static L1所有者范围写端口 验证并移动交付(const L1事实基座服务&s,L2需求结算所有者交付&d){if(!d.写入端口_.有效()||!d.写入端口_.绑定于(s))throw std::runtime_error("L2 demand settlement owner delivery invalid");return std::move(d.写入端口_);}
    const L1事实基座服务&第一层服务_;L1所有者范围写端口 第一层写入端口_;
    L2需求结算结构内部::登记定位 登记_;mutable std::mutex 互斥体_;
};

}
