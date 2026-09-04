module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.服务.L2任务结果消费结构;

export import 海中鱼巣.领域.合同.任务结果消费共同;
import 海中鱼巣.核心.服务.L1事实基座;

#define L2_TASK_RESULT_CONSUMPTION_NO_INCLUDES
export {
#include "L2任务结果消费结构.数据.h"
}

export namespace 海中鱼巣 {

inline constexpr L1所有者范围建立幂等身份 任务结果消费所有者建立身份{
    0x4C34'0305'0000'0001ULL};

class L2任务结果消费结构服务;

class L2任务结果消费所有者交付 final {
public:
    L2任务结果消费所有者交付() = delete;
    L2任务结果消费所有者交付(const L2任务结果消费所有者交付&) = delete;
    L2任务结果消费所有者交付(L2任务结果消费所有者交付&&) noexcept = default;
private:
    explicit L2任务结果消费所有者交付(L1所有者范围写端口&& 端口) noexcept
        : 写入端口_(std::move(端口)) {}
    friend std::optional<L2任务结果消费所有者交付>
        尝试形成L2任务结果消费所有者交付(
            const L1事实基座服务&, L1所有者范围交付&&) noexcept;
    friend class L2任务结果消费结构服务;
    L1所有者范围写端口 写入端口_;
};

std::optional<L2任务结果消费所有者交付>
尝试形成L2任务结果消费所有者交付(
    const L1事实基座服务& 第一层服务, L1所有者范围交付&& 原始交付) noexcept {
    try {
        const auto& 建立 = 原始交付.建立结果;
        const bool 成功 = 建立.状态 == L1所有者范围管理状态::成功
            || 建立.状态 == L1所有者范围管理状态::精确重复;
        if (!成功 || 建立.建立幂等身份 != 任务结果消费所有者建立身份
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
        return L2任务结果消费所有者交付{std::move(*原始交付.写入端口)};
    } catch (...) { return std::nullopt; }
}

}

namespace 海中鱼巣::L2任务结果消费结构内部 {

inline constexpr L1所有者范围写入幂等身份 登记幂等身份{
    0x4C32'0305'5245'4731ULL};
inline constexpr std::uint32_t 载荷键 = 0x0305'2000U;
inline constexpr std::uint32_t 任务关系键 = 0x0305'2001U;
inline constexpr std::uint32_t 轮次关系键 = 0x0305'2002U;
inline constexpr std::uint32_t 结果关系键 = 0x0305'2003U;
inline constexpr std::uint32_t 成员需求关系键 = 0x0305'2004U;
inline constexpr std::uint32_t 优先级批次关系键 = 0x0305'2005U;
inline constexpr std::uint32_t 优先级证据关系键 = 0x0305'2006U;

struct 登记定位 final {
    稳定编码 载荷, 任务关系, 轮次关系, 结果关系;
    稳定编码 成员需求关系, 优先级批次关系, 优先级证据关系;
};

inline L1所有者范围写集本地键 键(std::uint32_t 值) noexcept { return {值}; }
inline std::optional<稳定编码> 取映射(const L1所有者范围写入结果& 结果,
    std::uint32_t 本地键) noexcept {
    for (const auto& [键值, 编码] : 结果.新编码映射)
        if (键值.值 == 本地键) return 编码;
    return std::nullopt;
}

inline 登记定位 初始化登记(const L1事实基座服务& L1,
    L1所有者范围写端口& 端口) {
    L1所有者范围写集请求 写集;
    const auto 首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功 && 首次.首次规范化写集)
        写集 = *首次.首次规范化写集;
    else {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("L2 result registration cutoff unavailable");
        写集.期望事实代次 = 当前.事实代次;
        写集.写入幂等身份 = 登记幂等身份;
        写集.节点.push_back({键(载荷键), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组});
        for (const auto k : {任务关系键, 轮次关系键, 结果关系键,
                成员需求关系键, 优先级批次关系键, 优先级证据关系键})
            写集.节点.push_back({键(k), 节点种类::普通, std::nullopt});
    }
    const auto 写入 = 端口.提交所有者范围中性写集(写集);
    if (写入.状态 != L1所有者范围写入状态::成功
        && 写入.状态 != L1所有者范围写入状态::精确重复)
        throw std::runtime_error("L2 result registration failed");
    const auto 取 = [&](std::uint32_t k) {
        const auto v = 取映射(写入, k);
        if (!v) throw std::runtime_error("L2 result registration mapping missing");
        return *v;
    };
    return {取(载荷键), 取(任务关系键), 取(轮次关系键), 取(结果关系键),
        取(成员需求关系键), 取(优先级批次关系键), 取(优先级证据关系键)};
}

inline bool 活跃(const L1所有者范围事实副本& 事实, std::uint64_t 截止) noexcept {
    return std::visit([&](const auto& f) {
        return f.创建事实代次 != 0 && f.创建事实代次 <= 截止
            && (!f.退出事实代次 || *f.退出事实代次 > 截止);
    }, 事实);
}
inline bool 外部事实存在(const L1事实基座服务& L1, 稳定编码 id,
    std::uint64_t 截止) noexcept {
    const auto r = L1.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
    return 有效(id) && r.状态 == L1所有者范围读取状态::成功 && r.事实
        && 活跃(*r.事实, 截止);
}
inline bool 成员关系存在(const L1事实基座服务& L1,
    const L2成员消费资格材料& q, std::uint64_t 截止) noexcept {
    const auto r = L1.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, q.成员.关系稳定编码});
    if (r.状态 != L1所有者范围读取状态::成功 || !r.事实 || !活跃(*r.事实, 截止))
        return false;
    const auto* rel = std::get_if<L1所有者范围关系事实>(&*r.事实);
    return rel && rel->源节点 == q.成员.任务.值
        && rel->目标节点 == q.成员.来源需求.值;
}

inline bool 查成员(const std::vector<L2成员消费资格材料>& group,
    const L2成员消费资格材料& value) noexcept {
    return std::count(group.begin(), group.end(), value) == 1;
}

inline bool 记录有效(const L2任务结果消费分配记录事实& f,
    bool 持久, std::uint64_t 截止 = 0) noexcept {
    const bool 身份 = 持久 ? f.身份.有效() && L2生命周期完整(f.生命周期)
        && f.生命周期.创建事实代次 <= 截止
        && (!f.生命周期.退出事实代次 || *f.生命周期.退出事实代次 > 截止)
        : !f.身份.有效() && f.生命周期.创建事实代次 == 0
            && !f.生命周期.退出事实代次;
    if (!身份 || !有效(f.任务.值) || !有效(f.任务轮次.值) || !有效(f.正式结果.值)
        || f.冻结成员集合版本 == 0 || f.冻结成员.empty() || f.冻结成员.size() > 10000U
        || !L2任务结果消费容量合同有效(f.容量合同)
        || f.逐成员结果.size() != f.冻结成员.size()
        || f.来源共同事实截止 == 0 || f.关系规则版本 != 1
        || f.优先级规则版本 != 1 || f.同级规则版本 != 1) return false;
    for (std::size_t i = 0; i < f.冻结成员.size(); ++i) {
        const auto& q = f.冻结成员[i];
        if (!L2成员消费资格材料有效(q) || q.成员.任务 != f.任务
            || (i && f.冻结成员[i - 1].成员.关系稳定编码.值
                >= q.成员.关系稳定编码.值)) return false;
    }
    const bool 共享 = f.消费形态 == L2任务结果消费形态::可重复引用;
    const bool 可分 = f.消费形态 == L2任务结果消费形态::可分割单位;
    const bool 不可分 = f.消费形态 == L2任务结果消费形态::不可分割资格;
    if ((!共享 && !可分 && !不可分) || f.容量合同.消费形态 != f.消费形态
        || f.初始剩余 != f.容量合同.初始可用单位) return false;
    if (共享) {
        if (f.优先级批次 || !f.正式优先级组.empty() || f.初始剩余 || f.最终剩余)
            return false;
        for (std::size_t i = 0; i < f.逐成员结果.size(); ++i) {
            const auto& x = f.逐成员结果[i];
            if (x.消费资格 != f.冻结成员[i] || x.处理顺序 != i + 1 || x.正式优先级
                || x.成员状态 != L2成员消费分配状态::共同引用
                || x.消费资格.最低合法分配单位 || x.消费资格.期望分配单位
                || x.分配前剩余 || x.实际分配单位 || x.分配后剩余) return false;
        }
        return true;
    }
    if (!f.优先级批次 || !f.优先级批次->值.值
        || f.正式优先级组.size() != f.冻结成员.size()
        || f.容量合同.扣减维度组.size() != 1 || f.初始剩余 == 0
        || (不可分 && f.初始剩余 != 1)) return false;
    for (std::size_t i = 0; i < f.正式优先级组.size(); ++i) {
        const auto& p = f.正式优先级组[i];
        if (!p.身份.值.值 || !查成员(f.冻结成员, p.消费资格)
            || !p.优先级序号 || !p.同级顺序序号) return false;
        if (i) {
            const auto& prev = f.正式优先级组[i - 1];
            if (prev.优先级序号 > p.优先级序号
                || (prev.优先级序号 == p.优先级序号
                    && prev.同级顺序序号 >= p.同级顺序序号)) return false;
        }
    }
    std::uint64_t remaining = f.初始剩余, allocated = 0;
    for (std::size_t i = 0; i < f.逐成员结果.size(); ++i) {
        const auto& x = f.逐成员结果[i];
        const auto& p = f.正式优先级组[i];
        const auto& q = x.消费资格;
        if (x.处理顺序 != i + 1 || !x.正式优先级 || *x.正式优先级 != p
            || q != p.消费资格 || !q.最低合法分配单位
            || q.最低合法分配单位 > q.期望分配单位 || x.分配前剩余 != remaining
            || (不可分 && (q.最低合法分配单位 != 1 || q.期望分配单位 != 1))) return false;
        if (x.成员状态 == L2成员消费分配状态::已分配) {
            if (x.实际分配单位 < q.最低合法分配单位 || x.实际分配单位 > q.期望分配单位
                || x.实际分配单位 > x.分配前剩余
                || x.分配后剩余 != x.分配前剩余 - x.实际分配单位) return false;
        } else if (x.成员状态 == L2成员消费分配状态::未获分配) {
            if (!可分 || !x.分配前剩余 || x.分配前剩余 >= q.最低合法分配单位
                || x.实际分配单位 || x.分配后剩余 != x.分配前剩余) return false;
        } else if (x.成员状态 == L2成员消费分配状态::结果已耗尽) {
            if (x.分配前剩余 || x.实际分配单位 || x.分配后剩余) return false;
        } else return false;
        if (allocated > std::numeric_limits<std::uint64_t>::max() - x.实际分配单位)
            return false;
        allocated += x.实际分配单位;
        remaining = x.分配后剩余;
    }
    return remaining == f.最终剩余 && allocated == f.初始剩余 - f.最终剩余;
}

inline void E(std::vector<std::uint64_t>& o, 稳定编码 v) { o.push_back(v.值); }
inline void L(std::vector<std::uint64_t>& o, const L2生命周期& x) {
    o.push_back(x.创建事实代次); o.push_back(x.退出事实代次.value_or(0));
}
inline void Q(std::vector<std::uint64_t>& o, const L2成员消费资格材料& q) {
    E(o,q.成员.关系稳定编码); E(o,q.成员.任务.值); E(o,q.成员.来源需求.值);
    o.push_back(q.成员.来源顺序); L(o,q.成员.生命周期); E(o,q.目标合同.值);
    E(o,q.资格证据身份); o.push_back(q.资格规则版本); E(o,q.范围合同身份);
    E(o,q.所有权合同身份); E(o,q.有效性合同身份);
    o.push_back(q.最低合法分配单位); o.push_back(q.期望分配单位);
}
inline void P(std::vector<std::uint64_t>& o, const L2正式成员分配优先级引用& p) {
    E(o,p.身份.值); Q(o,p.消费资格); o.push_back(p.优先级序号); o.push_back(p.同级顺序序号);
}
inline void C(std::vector<std::uint64_t>& o, const L2任务结果消费容量合同& c) {
    E(o,c.合同身份); o.push_back(c.合同版本); o.push_back(static_cast<std::uint64_t>(c.消费形态));
    o.push_back(c.初始可用单位); o.push_back(c.扣减维度组.size());
    for(auto x:c.扣减维度组)o.push_back(static_cast<std::uint64_t>(x));
    E(o,c.范围合同身份); E(o,c.所有权合同身份); E(o,c.有效性合同身份);
}
inline std::vector<std::uint64_t> 编码(const L2任务结果消费分配记录事实& f) {
    std::vector<std::uint64_t> o; E(o,f.任务.值); E(o,f.任务轮次.值); E(o,f.正式结果.值);
    o.push_back(static_cast<std::uint64_t>(f.消费形态)); o.push_back(f.冻结成员集合版本); C(o,f.容量合同);
    o.push_back(f.冻结成员.size()); for(const auto& q:f.冻结成员)Q(o,q);
    o.push_back(f.优先级批次 ? 1 : 0);
    o.push_back(f.优先级批次 ? f.优先级批次->值.值 : 0);
    o.push_back(f.正式优先级组.size()); for(const auto& p:f.正式优先级组)P(o,p);
    o.push_back(f.逐成员结果.size()); for(const auto& x:f.逐成员结果){Q(o,x.消费资格);o.push_back(x.处理顺序);
        o.push_back(x.正式优先级 ? 1 : 0);
        if (x.正式优先级) P(o, *x.正式优先级);
        o.push_back(static_cast<std::uint64_t>(x.成员状态));o.push_back(x.分配前剩余);
        o.push_back(x.实际分配单位);o.push_back(x.分配后剩余);}
    o.push_back(f.初始剩余);o.push_back(f.最终剩余);o.push_back(f.来源共同事实截止);
    o.push_back(f.关系规则版本);o.push_back(f.优先级规则版本);o.push_back(f.同级规则版本);return o;
}
inline bool R(const std::vector<std::uint64_t>& a,std::size_t& i,std::uint64_t& v){if(i>=a.size())return false;v=a[i++];return true;}
inline bool RE(const std::vector<std::uint64_t>& a,std::size_t& i,稳定编码& v){return R(a,i,v.值);}
inline bool RL(const std::vector<std::uint64_t>& a,std::size_t& i,L2生命周期& x){std::uint64_t c=0,e=0;if(!R(a,i,c)||!R(a,i,e))return false;x={c,e?std::optional<std::uint64_t>{e}:std::nullopt};return true;}
inline bool RQ(const std::vector<std::uint64_t>& a,std::size_t& i,L2成员消费资格材料& q){std::uint64_t v=0;return RE(a,i,q.成员.关系稳定编码)&&RE(a,i,q.成员.任务.值)&&RE(a,i,q.成员.来源需求.值)&&R(a,i,q.成员.来源顺序)&&RL(a,i,q.成员.生命周期)&&RE(a,i,q.目标合同.值)&&RE(a,i,q.资格证据身份)&&R(a,i,v)&&(q.资格规则版本=static_cast<std::uint32_t>(v),true)&&RE(a,i,q.范围合同身份)&&RE(a,i,q.所有权合同身份)&&RE(a,i,q.有效性合同身份)&&R(a,i,q.最低合法分配单位)&&R(a,i,q.期望分配单位);}
inline bool RP(const std::vector<std::uint64_t>& a,std::size_t& i,L2正式成员分配优先级引用& p){return RE(a,i,p.身份.值)&&RQ(a,i,p.消费资格)&&R(a,i,p.优先级序号)&&R(a,i,p.同级顺序序号);}
inline bool RC(const std::vector<std::uint64_t>& a,std::size_t& i,L2任务结果消费容量合同& c){std::uint64_t v=0,t=0,n=0;if(!RE(a,i,c.合同身份)||!R(a,i,v)||!R(a,i,t)||!R(a,i,c.初始可用单位)||!R(a,i,n)||n>a.size()-i)return false;c.合同版本=static_cast<std::uint32_t>(v);c.消费形态=static_cast<L2任务结果消费形态>(t);for(std::uint64_t k=0;k<n;++k){if(!R(a,i,v))return false;c.扣减维度组.push_back(static_cast<L2结果消费扣减维度>(v));}return RE(a,i,c.范围合同身份)&&RE(a,i,c.所有权合同身份)&&RE(a,i,c.有效性合同身份);}
inline bool 解码(const std::vector<std::uint64_t>& a,L2任务结果消费分配记录事实& f){std::size_t i=0;std::uint64_t v=0,n=0,flag=0,id=0;if(!RE(a,i,f.任务.值)||!RE(a,i,f.任务轮次.值)||!RE(a,i,f.正式结果.值)||!R(a,i,v))return false;f.消费形态=static_cast<L2任务结果消费形态>(v);if(!R(a,i,f.冻结成员集合版本)||!RC(a,i,f.容量合同)||!R(a,i,n)||n>10000U)return false;for(std::uint64_t k=0;k<n;++k){L2成员消费资格材料 q;if(!RQ(a,i,q))return false;f.冻结成员.push_back(std::move(q));}if(!R(a,i,flag)||!R(a,i,id)||flag>1||(flag==0&&id))return false;if(flag)f.优先级批次=L2自我结果争用优先级批次身份{稳定编码{id}};if(!R(a,i,n)||n>10000U)return false;for(std::uint64_t k=0;k<n;++k){L2正式成员分配优先级引用 p;if(!RP(a,i,p))return false;f.正式优先级组.push_back(std::move(p));}if(!R(a,i,n)||n>10000U)return false;for(std::uint64_t k=0;k<n;++k){L2成员消费分配结果事实 x;if(!RQ(a,i,x.消费资格)||!R(a,i,x.处理顺序)||!R(a,i,flag)||flag>1)return false;if(flag){L2正式成员分配优先级引用 p;if(!RP(a,i,p))return false;x.正式优先级=std::move(p);}if(!R(a,i,v))return false;x.成员状态=static_cast<L2成员消费分配状态>(v);if(!R(a,i,x.分配前剩余)||!R(a,i,x.实际分配单位)||!R(a,i,x.分配后剩余))return false;f.逐成员结果.push_back(std::move(x));}std::uint64_t r=0,p=0,s=0;return R(a,i,f.初始剩余)&&R(a,i,f.最终剩余)&&R(a,i,f.来源共同事实截止)&&R(a,i,r)&&R(a,i,p)&&R(a,i,s)&&(f.关系规则版本=static_cast<std::uint32_t>(r),f.优先级规则版本=static_cast<std::uint32_t>(p),f.同级规则版本=static_cast<std::uint32_t>(s),true)&&i==a.size();}

inline L2任务结果消费分配记录状态 映射(L1所有者范围写入状态 s) noexcept {
    switch(s){case L1所有者范围写入状态::成功:return L2任务结果消费分配记录状态::已形成;
    case L1所有者范围写入状态::精确重复:return L2任务结果消费分配记录状态::精确重复;
    case L1所有者范围写入状态::入口拒绝:return L2任务结果消费分配记录状态::入口拒绝;
    case L1所有者范围写入状态::许可拒绝:return L2任务结果消费分配记录状态::许可拒绝;
    case L1所有者范围写入状态::事实代次漂移:case L1所有者范围写入状态::引用冲突:
    case L1所有者范围写入状态::未找到:case L1所有者范围写入状态::已退出:return L2任务结果消费分配记录状态::当前性漂移;
    case L1所有者范围写入状态::幂等冲突:return L2任务结果消费分配记录状态::幂等冲突;
    case L1所有者范围写入状态::资源失败:return L2任务结果消费分配记录状态::资源失败;
    default:return L2任务结果消费分配记录状态::内部错误;}
}

} // namespace 海中鱼巣::L2任务结果消费结构内部

export namespace 海中鱼巣 {

class L2任务结果消费结构服务 final {
public:
    L2任务结果消费结构服务(const L1事实基座服务& 第一层服务,
        L2任务结果消费所有者交付&& 交付)
        : 第一层服务_(第一层服务), 第一层写入端口_(验证并移动交付(第一层服务, 交付)),
          登记_(L2任务结果消费结构内部::初始化登记(第一层服务_, 第一层写入端口_)) {}

    L2提交任务结果消费分配记录结果 发布或读取任务结果消费分配记录(
        const L2提交任务结果消费分配记录请求& 请求) noexcept {
        std::lock_guard<std::mutex> lock(互斥体_);
        const auto fail=[](auto s){L2提交任务结果消费分配记录结果 r;r.状态=s;return r;};
        try {
            if(请求.请求头.合同版本!=L2结构合同版本||!请求.请求头.期望事实代次
                ||!L2结构幂等身份有效(请求.幂等身份)
                ||请求.幂等身份.值==L2任务结果消费结构内部::登记幂等身份.值)
                return fail(L2任务结果消费分配记录状态::入口拒绝);
            const auto& f=请求.候选记录;
            if(!L2任务结果消费结构内部::记录有效(f,false)){
                if(!L2任务结果消费容量合同有效(f.容量合同))return fail(L2任务结果消费分配记录状态::容量合同缺失);
                if((f.消费形态==L2任务结果消费形态::可分割单位||f.消费形态==L2任务结果消费形态::不可分割资格)&&(!f.优先级批次||f.正式优先级组.empty()))return fail(L2任务结果消费分配记录状态::优先级合同缺失);
                return fail(L2任务结果消费分配记录状态::成员漂移);
            }
            const auto g=请求.请求头.期望事实代次;
            if(!L2任务结果消费结构内部::外部事实存在(第一层服务_,f.任务.值,g)
                ||!L2任务结果消费结构内部::外部事实存在(第一层服务_,f.任务轮次.值,g)
                ||!L2任务结果消费结构内部::外部事实存在(第一层服务_,f.正式结果.值,g))return fail(L2任务结果消费分配记录状态::当前性漂移);
            for(const auto& q:f.冻结成员)if(!L2任务结果消费结构内部::成员关系存在(第一层服务_,q,g)||!L2任务结果消费结构内部::外部事实存在(第一层服务_,q.目标合同.值,g))return fail(L2任务结果消费分配记录状态::成员漂移);
            if(f.优先级批次&&!L2任务结果消费结构内部::外部事实存在(第一层服务_,f.优先级批次->值,g))return fail(L2任务结果消费分配记录状态::优先级合同缺失);
            for(const auto& p:f.正式优先级组)if(!L2任务结果消费结构内部::外部事实存在(第一层服务_,p.身份.值,g))return fail(L2任务结果消费分配记录状态::优先级合同缺失);
            L1所有者范围写集请求 w;w.期望事实代次=g;w.写入幂等身份={请求.幂等身份.值};
            w.节点.push_back({L1所有者范围写集本地键{1}, 节点种类::普通,
                std::nullopt});
            w.值.push_back({L1所有者范围写集本地键{2},
                L1所有者范围写集本地键{1}, 登记_.载荷,
                L2任务结果消费结构内部::编码(f),
                L1所有者范围写集本地键{1}});
            w.属性槽变更.push_back({L1所有者范围写集本地键{1},
                登记_.载荷, L1所有者范围写集本地键{2}});
            const auto rel = [&](std::uint32_t k, 稳定编码 target,
                稳定编码 type, std::int64_t role = 1) {
                w.关系.push_back({L1所有者范围写集本地键{k},
                    L1所有者范围写集本地键{1}, target, type, role});
            };
            rel(10,f.任务.值,登记_.任务关系);rel(11,f.任务轮次.值,登记_.轮次关系);rel(12,f.正式结果.值,登记_.结果关系);
            std::uint32_t k=100;for(std::size_t i=0;i<f.冻结成员.size();++i)rel(k++,f.冻结成员[i].成员.来源需求.值,登记_.成员需求关系,static_cast<std::int64_t>(i+1));
            if(f.优先级批次)rel(k++,f.优先级批次->值,登记_.优先级批次关系);
            for(std::size_t i=0;i<f.正式优先级组.size();++i)rel(k++,f.正式优先级组[i].身份.值,登记_.优先级证据关系,static_cast<std::int64_t>(i+1));
            const auto wr=第一层写入端口_.提交所有者范围中性写集(w);const auto s=L2任务结果消费结构内部::映射(wr.状态);
            if(s!=L2任务结果消费分配记录状态::已形成&&s!=L2任务结果消费分配记录状态::精确重复)return fail(s);
            const auto id=L2任务结果消费结构内部::取映射(wr,1);
            if(id){auto rr=读取_已锁定({1,L2任务结果消费分配记录身份{*id},f.正式结果,wr.事实代次});if(rr.成功()){auto expected=f;expected.身份=L2任务结果消费分配记录身份{*id};expected.生命周期={wr.事实代次,std::nullopt};if(rr.记录&&*rr.记录==expected)return {s,std::move(rr.记录),std::nullopt,rr.本次正式读回截止};}}
            L2提交任务结果消费分配记录结果 r;r.状态=L2任务结果消费分配记录状态::已可能发布;r.提交见证=L2任务结果消费分配提交见证{第一层写入端口_.所有者身份(),请求.幂等身份,wr.事实代次,1};return r;
        }catch(const std::bad_alloc&){return fail(L2任务结果消费分配记录状态::资源失败);}catch(const std::length_error&){return fail(L2任务结果消费分配记录状态::资源失败);}catch(...){return fail(L2任务结果消费分配记录状态::内部错误);}
    }

    L2任务结果消费分配记录读取结果 读取任务结果消费分配记录(
        const L2任务结果消费分配记录读取请求& 请求) const noexcept {
        std::lock_guard<std::mutex> lock(互斥体_);return 读取_已锁定(请求);
    }
private:
    std::vector<L1所有者范围关系事实> 关系(稳定编码 src,稳定编码 type,std::uint64_t g)const{
        const auto x=第一层服务_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,src,type,g});
        std::vector<L1所有者范围关系事实> o;if(x.状态!=L1所有者范围读取状态::成功)return o;for(const auto& r:x.关系组)if(r.写入所有者==第一层写入端口_.所有者身份()&&r.创建事实代次<=g&&(!r.退出事实代次||*r.退出事实代次>g))o.push_back(r);return o;
    }
    bool 唯一(稳定编码 src,稳定编码 type,稳定编码 dst,std::uint64_t g)const{const auto x=关系(src,type,g);return x.size()==1&&x[0].目标节点==dst&&x[0].角色或顺序==1;}
    L2任务结果消费分配记录读取结果 读取_已锁定(const L2任务结果消费分配记录读取请求& q)const noexcept{
        const auto fail=[](auto s){L2任务结果消费分配记录读取结果 r;r.状态=s;return r;};
        try{if(q.合同版本!=1||!q.记录.有效()||!有效(q.正式结果.值)||!q.非零读取截止)return fail(L2任务结果消费分配记录状态::入口拒绝);
            const auto nr=第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,q.记录.值});
            if(nr.状态==L1所有者范围读取状态::资源失败)return fail(L2任务结果消费分配记录状态::资源失败);
            if(nr.状态!=L1所有者范围读取状态::成功||!nr.事实)return fail(L2任务结果消费分配记录状态::未找到);
            const auto* n=std::get_if<L1所有者范围节点事实>(&*nr.事实);if(!n||n->写入所有者!=第一层写入端口_.所有者身份()||n->创建事实代次>q.非零读取截止||(n->退出事实代次&&*n->退出事实代次<=q.非零读取截止))return fail(L2任务结果消费分配记录状态::许可拒绝);
            const auto vr=第一层服务_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本,q.记录.值,q.非零读取截止});if(vr.状态!=L1所有者范围读取状态::成功)return fail(vr.状态==L1所有者范围读取状态::资源失败 ? L2任务结果消费分配记录状态::资源失败 : L2任务结果消费分配记录状态::内部错误);
            const std::vector<std::uint64_t>* payload=nullptr;for(const auto& v:vr.属性值组)if(v.属性类型节点==登记_.载荷&&v.创建事实代次<=q.非零读取截止&&(!v.退出事实代次||*v.退出事实代次>q.非零读取截止)){if(payload)return fail(L2任务结果消费分配记录状态::内部错误);payload=std::get_if<std::vector<std::uint64_t>>(&v.材料);}if(!payload)return fail(L2任务结果消费分配记录状态::内部错误);
            L2任务结果消费分配记录事实 f;if(!L2任务结果消费结构内部::解码(*payload,f)||f.正式结果!=q.正式结果)return fail(L2任务结果消费分配记录状态::内部错误);f.身份=q.记录;f.生命周期={n->创建事实代次,n->退出事实代次};
            if(!唯一(q.记录.值,登记_.任务关系,f.任务.值,q.非零读取截止)||!唯一(q.记录.值,登记_.轮次关系,f.任务轮次.值,q.非零读取截止)||!唯一(q.记录.值,登记_.结果关系,f.正式结果.值,q.非零读取截止))return fail(L2任务结果消费分配记录状态::内部错误);
            auto members=关系(q.记录.值,登记_.成员需求关系,q.非零读取截止);if(members.size()!=f.冻结成员.size())return fail(L2任务结果消费分配记录状态::内部错误);std::sort(members.begin(),members.end(),[](const auto&a,const auto&b){return a.角色或顺序<b.角色或顺序;});for(std::size_t i=0;i<members.size();++i)if(members[i].角色或顺序!=static_cast<std::int64_t>(i+1)||members[i].目标节点!=f.冻结成员[i].成员.来源需求.值)return fail(L2任务结果消费分配记录状态::内部错误);
            const auto batch=关系(q.记录.值,登记_.优先级批次关系,q.非零读取截止);if((f.优先级批次&&(batch.size()!=1||batch[0].目标节点!=f.优先级批次->值))||(!f.优先级批次&&!batch.empty()))return fail(L2任务结果消费分配记录状态::内部错误);
            auto ps=关系(q.记录.值,登记_.优先级证据关系,q.非零读取截止);if(ps.size()!=f.正式优先级组.size())return fail(L2任务结果消费分配记录状态::内部错误);std::sort(ps.begin(),ps.end(),[](const auto&a,const auto&b){return a.角色或顺序<b.角色或顺序;});for(std::size_t i=0;i<ps.size();++i)if(ps[i].角色或顺序!=static_cast<std::int64_t>(i+1)||ps[i].目标节点!=f.正式优先级组[i].身份.值)return fail(L2任务结果消费分配记录状态::内部错误);
            if(!L2任务结果消费结构内部::记录有效(f,true,q.非零读取截止))return fail(L2任务结果消费分配记录状态::内部错误);return {L2任务结果消费分配记录状态::已形成,std::move(f),q.非零读取截止};
        }catch(const std::bad_alloc&){return fail(L2任务结果消费分配记录状态::资源失败);}catch(const std::length_error&){return fail(L2任务结果消费分配记录状态::资源失败);}catch(...){return fail(L2任务结果消费分配记录状态::内部错误);}}
    static L1所有者范围写端口 验证并移动交付(const L1事实基座服务& s,L2任务结果消费所有者交付& d){if(!d.写入端口_.有效()||!d.写入端口_.绑定于(s))throw std::runtime_error("L2 result consumption owner delivery invalid");return std::move(d.写入端口_);}
    const L1事实基座服务& 第一层服务_;L1所有者范围写端口 第一层写入端口_;
    L2任务结果消费结构内部::登记定位 登记_;mutable std::mutex 互斥体_;
};

}
