#include "数据服务.特征值域比较类.h"

namespace 海中鱼巣 {
namespace {
bool 概念读取预算有效(const 有界事实读取预算_B1& b) noexcept {
    return b.最大节点数 && b.最大关系数 && b.最大值数 && b.最大材料总数;
}
bool 概念读取用量有效(const 有界事实读取用量_B1& u,
                       const 有界事实读取预算_B1& b) noexcept {
    return 概念读取预算有效(b) && u.节点数<=b.最大节点数 &&
        u.关系数<=b.最大关系数 && u.值数<=b.最大值数 &&
        u.材料总数<=b.最大材料总数 &&
        u.材料总数==u.节点数+u.关系数+u.值数;
}
bool 值与材料预算有效(const 世界结构预算_B1& b) noexcept {
    return 世界结构预算有效(b) && b.最大节点数 && b.最大关系数 && b.最大值数;
}
bool 值与材料用量有效(const 世界结构用量_B1& u,
                       const 世界结构预算_B1& b) noexcept {
    return 值与材料预算有效(b) && u.最大节点数<=b.最大节点数 &&
        u.最大关系数<=b.最大关系数 && u.最大值数<=b.最大值数 &&
        u.最大祖先数==0 && u.最大后代数==0 && u.最大候选数==0 &&
        u.最大值元素数==0 && u.最大材料字节数==0 && u.最大域原子数==0;
}
bool 值与材料合计有效(const 世界结构用量_B1& a,const 世界结构用量_B1& b,
                       const 世界结构预算_B1& limit) noexcept {
    if(!值与材料用量有效(a,limit)||!值与材料用量有效(b,limit)||
       a.最大节点数>UINT64_MAX-b.最大节点数||
       a.最大关系数>UINT64_MAX-b.最大关系数||
       a.最大值数>UINT64_MAX-b.最大值数)return false;
    return a.最大节点数+b.最大节点数<=limit.最大节点数 &&
        a.最大关系数+b.最大关系数<=limit.最大关系数 &&
        a.最大值数+b.最大值数<=limit.最大值数;
}
template<class Result>
struct 概念用量回填 final {
    Result& 结果;
    const 有界事实读取用量_B1& 用量;
    ~概念用量回填() { 结果.概念读取用量=用量; }
};
bool 基础预算有效(const 概念树预算& b) noexcept {
    return b.最大概念数 && b.最大关系数 && b.最大特征属性数;
}
bool 读取请求有效(const 特征值域读取请求_v1& r) noexcept {
    return r.版本 == 1 && r.Gread && r.H && r.H <= r.Gread && 有效(r.FC.值)
        && 值与材料预算有效(r.预算.值与材料预算) && 基础预算有效(r.预算.概念预算)
        && 概念读取预算有效(r.预算.概念读取预算) && r.预算.最大域项数;
}
特征值域比较状态_v1 映射基础状态(特征概念值域基础读取状态_v1 s) noexcept {
    switch (s) {
    case 特征概念值域基础读取状态_v1::未找到: return 特征值域比较状态_v1::未找到;
    case 特征概念值域基础读取状态_v1::目标已退出: return 特征值域比较状态_v1::目标已退出;
    case 特征概念值域基础读取状态_v1::类别冲突: return 特征值域比较状态_v1::类别冲突;
    case 特征概念值域基础读取状态_v1::规则缺失: return 特征值域比较状态_v1::规则缺失;
    case 特征概念值域基础读取状态_v1::未实现: return 特征值域比较状态_v1::未实现;
    case 特征概念值域基础读取状态_v1::事实代次漂移: return 特征值域比较状态_v1::事实代次漂移;
    case 特征概念值域基础读取状态_v1::数量预算不足: return 特征值域比较状态_v1::数量预算不足;
    case 特征概念值域基础读取状态_v1::历史材料不可用: return 特征值域比较状态_v1::历史材料不可用;
    case 特征概念值域基础读取状态_v1::资源失败: return 特征值域比较状态_v1::资源失败;
    case 特征概念值域基础读取状态_v1::入口拒绝: return 特征值域比较状态_v1::入口拒绝;
    default: return 特征值域比较状态_v1::内部不一致;
    }
}
特征值域比较状态_v1 映射特征错误(特征数据错误 e) noexcept {
    switch (e) {
    case 特征数据错误::未找到: return 特征值域比较状态_v1::未找到;
    case 特征数据错误::已退出: return 特征值域比较状态_v1::目标已退出;
    case 特征数据错误::类型不相容: return 特征值域比较状态_v1::类型不相容;
    case 特征数据错误::规则缺失: return 特征值域比较状态_v1::规则缺失;
    case 特征数据错误::并发变化: return 特征值域比较状态_v1::事实代次漂移;
    case 特征数据错误::数量预算不足: return 特征值域比较状态_v1::数量预算不足;
    case 特征数据错误::历史材料不可用: return 特征值域比较状态_v1::历史材料不可用;
    case 特征数据错误::资源失败: return 特征值域比较状态_v1::资源失败;
    case 特征数据错误::入口拒绝: return 特征值域比较状态_v1::入口拒绝;
    default: return 特征值域比较状态_v1::内部不一致;
    }
}
特征值域比较状态_v2 映射基础状态_v2(特征概念值域基础读取状态_v2 s) noexcept {
    switch (s) {
    case 特征概念值域基础读取状态_v2::未找到: return 特征值域比较状态_v2::未找到;
    case 特征概念值域基础读取状态_v2::目标已退出: return 特征值域比较状态_v2::目标已退出;
    case 特征概念值域基础读取状态_v2::类别冲突: return 特征值域比较状态_v2::类别冲突;
    case 特征概念值域基础读取状态_v2::规则缺失: return 特征值域比较状态_v2::规则缺失;
    case 特征概念值域基础读取状态_v2::未实现: return 特征值域比较状态_v2::未实现;
    case 特征概念值域基础读取状态_v2::事实代次漂移: return 特征值域比较状态_v2::事实代次漂移;
    case 特征概念值域基础读取状态_v2::历史材料不可用: return 特征值域比较状态_v2::历史材料不可用;
    case 特征概念值域基础读取状态_v2::资源失败: return 特征值域比较状态_v2::资源失败;
    case 特征概念值域基础读取状态_v2::入口拒绝: return 特征值域比较状态_v2::入口拒绝;
    default: return 特征值域比较状态_v2::内部不一致;
    }
}
特征值域比较状态_v2 映射特征错误_v2(特征数据错误 e) noexcept {
    switch (e) {
    case 特征数据错误::未找到: return 特征值域比较状态_v2::未找到;
    case 特征数据错误::已退出: return 特征值域比较状态_v2::目标已退出;
    case 特征数据错误::类型不相容: return 特征值域比较状态_v2::类型不相容;
    case 特征数据错误::规则缺失: return 特征值域比较状态_v2::规则缺失;
    case 特征数据错误::能力未提供: return 特征值域比较状态_v2::未实现;
    case 特征数据错误::并发变化: return 特征值域比较状态_v2::事实代次漂移;
    case 特征数据错误::历史材料不可用: return 特征值域比较状态_v2::历史材料不可用;
    case 特征数据错误::资源失败: return 特征值域比较状态_v2::资源失败;
    case 特征数据错误::入口拒绝: return 特征值域比较状态_v2::入口拒绝;
    default: return 特征值域比较状态_v2::内部不一致;
    }
}
bool 规范值域完整(const 特征规范值域_v2 &domain) noexcept {
    return std::visit([](const auto &value) noexcept {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, 特征规范I64域>)
            return !value.区间.empty();
        else if constexpr (std::is_same_v<T, 特征I64组有限域_v2> ||
                           std::is_same_v<T, 特征U64组有限域_v2>)
            return !value.点组.empty();
        else
            return !value.材料组.empty() &&
                   std::all_of(value.材料组.begin(), value.材料组.end(), [](const auto &item) {
                       return 有效(item.格式) && !item.完整载荷.empty();
                   });
    }, domain);
}
}

特征值域比较数据服务::特征值域比较数据服务(
    const 概念树类数据服务& concepts, const 特征类数据服务& features,
    const 特征值类数据服务& values) noexcept
    : concepts_(concepts), features_(features), values_(values) {}

bool 特征值域比较数据服务::绑定于(const L1事实基座服务& l1) const noexcept {
    return concepts_.绑定于(l1) && features_.绑定于(l1) && values_.绑定于(l1);
}

bool 特征值域读取结果_v1::成功(const 特征值域读取请求_v1& r) const noexcept {
    return 版本 == 1 && 读取请求有效(r) && 状态 == 特征值域比较状态_v1::已读取
        && Gread == r.Gread && H == r.H && 域 && 域->FC == r.FC && 有效(域->FT)
        && 有效(域->规则) && 域->表示 == 特征值表示类型::I64 && !域->I64域.区间.empty()
        && 域->基础读回.FC == r.FC && 域->基础读回.Gread == r.Gread && 域->基础读回.H == r.H
        && 值与材料用量有效(域->值与材料用量,r.预算.值与材料预算)
        && 概念读取用量有效(概念读取用量,r.预算.概念读取预算);
}

bool 特征值域关系结果_v1::成功(const 特征值域关系核验请求_v1& r) const noexcept {
    特征值域读取请求_v1 left{1, r.Gread, r.H, r.左FC, r.预算};
    特征值域读取请求_v1 right{1, r.Gread, r.H, r.右FC, r.预算};
    return 版本 == 1 && r.版本 == 1 && 读取请求有效(left) && 读取请求有效(right)
        && 状态 == 特征值域比较状态_v1::已核验 && Gread == r.Gread && H == r.H
        && 关系 && 左域 && 右域 && 左域->FC == r.左FC && 右域->FC == r.右FC
        && 左域->FT == 右域->FT && 左域->表示 == 右域->表示
        && 值与材料合计有效(左域->值与材料用量,右域->值与材料用量,
            r.预算.值与材料预算)
        && 概念读取用量有效(概念读取用量,r.预算.概念读取预算);
}

bool 实例值域命中结果_v1::成功(const 实例值域命中核验请求_v1& r) const noexcept {
    特征值域读取请求_v1 fc{1, r.Gread, r.H, r.FC, r.预算};
    return 版本 == 1 && r.版本 == 1 && r.F.编码.值 && 读取请求有效(fc)
        && 状态 == 特征值域比较状态_v1::已核验 && Gread == r.Gread && H == r.H
        && 关系 && (*关系 == 特征值域关系_v1::左包含右 || *关系 == 特征值域关系_v1::不包含)
        && 域 && 域->FC == r.FC
        && 值与材料用量有效(域->值与材料用量,r.预算.值与材料预算)
        && 概念读取用量有效(概念读取用量,r.预算.概念读取预算);
}

bool 特征值域读取结果_v2::成功(const 特征值域读取请求_v2 &r) const noexcept {
    return 版本 == 2 && r.版本 == 2 && r.Gread && r.H && r.H <= r.Gread &&
        有效(r.FC.值) && 状态 == 特征值域比较状态_v2::已读取 &&
        Gread == r.Gread && H == r.H && 域 && 域->FC == r.FC &&
        有效(域->FT) && 有效(域->规则身份) && 域->规则版本 == 1 &&
        域->基础读回.FC == r.FC && 域->基础读回.FT == 域->FT &&
        域->基础读回.原始表示 == 域->原始表示 &&
        域->基础读回.规则身份 == 域->规则身份 &&
        域->基础读回.规则版本 == 域->规则版本 && 规范值域完整(域->规范化值域);
}

bool 特征值域关系结果_v2::成功(const 特征值域关系核验请求_v2 &r) const noexcept {
    return 版本 == 2 && r.版本 == 2 && r.Gread && r.H && r.H <= r.Gread &&
        有效(r.左FC.值) && 有效(r.右FC.值) &&
        状态 == 特征值域比较状态_v2::已核验 && Gread == r.Gread && H == r.H &&
        关系 && 左域 && 右域 && 左域->FC == r.左FC && 右域->FC == r.右FC &&
        左域->FT == 右域->FT && 左域->原始表示 == 右域->原始表示 &&
        左域->规则身份 == 右域->规则身份 && 左域->规则版本 == 右域->规则版本 &&
        规范值域完整(左域->规范化值域) && 规范值域完整(右域->规范化值域);
}

bool 实例值域命中结果_v2::成功(const 实例值域命中核验请求_v2 &r) const noexcept {
    return 版本 == 2 && r.版本 == 2 && r.Gread && r.H && r.H <= r.Gread &&
        有效(r.F) && 有效(r.FC.值) && 状态 == 特征值域比较状态_v2::已核验 &&
        Gread == r.Gread && H == r.H && 关系 && 域 && 域->FC == r.FC &&
        (*关系 == 特征值域关系_v2::左包含右 ||
         *关系 == 特征值域关系_v2::不包含) && 规范值域完整(域->规范化值域);
}

特征值域读取结果_v1 特征值域比较数据服务::读取特征值域(
    const 特征值域读取请求_v1& r) const {
    概念事实读取会话_v1 session(concepts_.port_.所有者身份(),r.Gread,r.H,
                                  r.预算.概念读取预算,r.预算.概念预算.最大概念数);
    特征值域事实读取会话_v1 featureSession(r.预算.值与材料预算);
    return 读取特征值域共享(r,session,featureSession);
}

特征值域读取结果_v1 特征值域比较数据服务::读取特征值域共享(
    const 特征值域读取请求_v1& r, 概念事实读取会话_v1& session) const {
    特征值域事实读取会话_v1 featureSession(r.预算.值与材料预算);
    return 读取特征值域共享(r,session,featureSession);
}

特征值域读取结果_v1 特征值域比较数据服务::读取特征值域共享(
    const 特征值域读取请求_v1& r, 概念事实读取会话_v1& session,
    特征值域事实读取会话_v1& featureSession) const {
    特征值域读取结果_v1 out; out.Gread = r.Gread;
    概念用量回填 guard{out,session.用量_};
    const auto featureBefore=featureSession.计量_.用量;
    const auto featureDelta=[&]() {
        const auto& now=featureSession.计量_.用量;
        if(now.节点数<featureBefore.节点数||now.关系数<featureBefore.关系数||
           now.属性值数<featureBefore.属性值数)
            throw 特征数据错误::内部不一致;
        return 世界结构用量_B1{now.节点数-featureBefore.节点数,
            now.关系数-featureBefore.关系数,now.属性值数-featureBefore.属性值数};
    };
    try {
        if (!读取请求有效(r) || session.所有者_!=concepts_.port_.所有者身份() ||
            session.Gread_!=r.Gread || session.H_!=r.H ||
            session.预算_!=r.预算.概念读取预算 ||
            session.最大概念数_!=r.预算.概念预算.最大概念数) return out;
        const 特征概念值域基础读取请求_v1 baseRequest{
            1,r.Gread,r.H,r.FC,r.预算.概念预算,r.预算.概念读取预算};
        const auto base = concepts_.读取特征概念值域基础共享(baseRequest,session);
        if (!base.成功(baseRequest) || !base.事实) {
            out.状态 = 映射基础状态(base.状态); return out;
        }
        const auto& fact = *base.事实;
        if (fact.表示 == 特征值表示类型::I64组) { out.状态=读取I64组值域待实现(r,fact).状态; return out; }
        if (fact.表示 == 特征值表示类型::U64组) { out.状态=读取U64组值域待实现(r,fact).状态; return out; }
        if (fact.表示 == 特征值表示类型::独立材料引用) { out.状态=读取独立材料值域待实现(r,fact).状态; return out; }
        if (fact.表示 != 特征值表示类型::I64 || !fact.I64域 || fact.I64域->区间.empty()
            || fact.I64域->区间.size() > r.预算.最大域项数) {
            out.状态 = 特征值域比较状态_v1::内部不一致; return out;
        }
        const auto provider=features_.保护<std::pair<I64基础特征类型信息,特征规范I64域>>([&] {
            features_.截止有效(1,r.Gread,r.H);features_.守卫(r.Gread);
            auto type=features_.读类型(fact.FT,r.Gread,r.H,nullptr,&featureSession.计量_);
            if(!type.规则)throw 特征数据错误::规则缺失;
            auto full=features_.读完整域(fact.FT,r.Gread,r.H,&featureSession.计量_);
            auto domain=features_.规范域(*fact.I64域);
            features_.要求(features_.包含(full,domain),特征数据错误::类型不相容);
            features_.守卫(r.Gread);return std::pair{std::move(type),std::move(domain)};
        });
        const auto* provided=std::get_if<std::pair<I64基础特征类型信息,特征规范I64域>>(&provider);
        if(!provided){out.状态=映射特征错误(std::get<特征数据错误>(provider));return out;}
        if(provided->second.区间.empty()||provided->second.区间.size()>r.预算.最大域项数) {
            out.状态 = 特征值域比较状态_v1::事实代次漂移; return out;
        }
        out.H = r.H;
        out.域 = 特征值域事实_v1{r.FC, fact.FT, fact.表示, *provided->first.规则,
            provided->second, fact, featureDelta()};
        out.状态 = 特征值域比较状态_v1::已读取;
    } catch (const std::bad_alloc&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (const std::length_error&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (...) { out.状态 = 特征值域比较状态_v1::内部不一致; }
    if (out.状态 != 特征值域比较状态_v1::已读取) { out.H = 0; out.域.reset(); }
    return out;
}

特征值域关系结果_v1 特征值域比较数据服务::核验特征值域关系(
    const 特征值域关系核验请求_v1& r) const {
    特征值域关系结果_v1 out; out.Gread = r.Gread;
    概念事实读取会话_v1 session(concepts_.port_.所有者身份(),r.Gread,r.H,
                                  r.预算.概念读取预算,r.预算.概念预算.最大概念数);
    特征值域事实读取会话_v1 featureSession(r.预算.值与材料预算);
    概念用量回填 guard{out,session.用量_};
    try {
        const 特征值域读取请求_v1 leftRequest{1, r.Gread, r.H, r.左FC, r.预算};
        const 特征值域读取请求_v1 rightRequest{1, r.Gread, r.H, r.右FC, r.预算};
        if (r.版本 != 1 || !读取请求有效(leftRequest) || !读取请求有效(rightRequest)) return out;
        const auto left = 读取特征值域共享(leftRequest,session,featureSession);
        if (!left.成功(leftRequest)) { out.状态 = left.状态; return out; }
        const auto right = 读取特征值域共享(rightRequest,session,featureSession);
        if (!right.成功(rightRequest)) { out.状态 = right.状态; return out; }
        if (left.域->FT != right.域->FT || left.域->表示 != right.域->表示) {
            out.状态 = 特征值域比较状态_v1::类型不相容; return out;
        }
        const bool l=features_.包含(left.域->I64域,right.域->I64域);
        const bool rr=features_.包含(right.域->I64域,left.域->I64域);
        out.H = r.H; out.左域 = left.域; out.右域 = right.域;
        out.关系 = l && rr ? 特征值域关系_v1::相等 : l ? 特征值域关系_v1::左包含右 :
            rr ? 特征值域关系_v1::右包含左 : 特征值域关系_v1::不包含;
        out.状态 = 特征值域比较状态_v1::已核验;
    } catch (const std::bad_alloc&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (const std::length_error&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (...) { out.状态 = 特征值域比较状态_v1::内部不一致; }
    if (out.状态 != 特征值域比较状态_v1::已核验) { out.H = 0; out.关系.reset(); out.左域.reset(); out.右域.reset(); }
    return out;
}

实例值域命中结果_v1 特征值域比较数据服务::核验实例值域命中(
    const 实例值域命中核验请求_v1& r) const {
    实例值域命中结果_v1 out; out.Gread = r.Gread;
    概念事实读取会话_v1 session(concepts_.port_.所有者身份(),r.Gread,r.H,
                                  r.预算.概念读取预算,r.预算.概念预算.最大概念数);
    特征值域事实读取会话_v1 featureSession(r.预算.值与材料预算);
    概念用量回填 guard{out,session.用量_};
    try {
        const 特征值域读取请求_v1 fcRequest{1, r.Gread, r.H, r.FC, r.预算};
        if (r.版本 != 1 || !有效(r.F) || !读取请求有效(fcRequest)) return out;
        const auto domain = 读取特征值域共享(fcRequest,session,featureSession);
        if (!domain.成功(fcRequest)) { out.状态 = domain.状态; return out; }
        const auto matched=features_.保护<bool>([&] {
            features_.截止有效(1,r.Gread,r.H);features_.守卫(r.Gread);
            const auto actual=features_.读准确(r.F,r.Gread,r.H,nullptr,&featureSession.计量_);
            features_.要求(actual.信息.类型==domain.域->FT,特征数据错误::类型不相容);
            const auto value=features_.完整整数(actual);
            const auto hit=features_.包含(domain.域->I64域,特征规范I64域{{{value,value}}});
            features_.守卫(r.Gread);return hit;
        });
        const auto* hit=std::get_if<bool>(&matched);
        if(!hit){out.状态=映射特征错误(std::get<特征数据错误>(matched));return out;}
        out.H = r.H; out.域 = domain.域;
        const auto& usage=featureSession.计量_.用量;
        out.域->值与材料用量={usage.节点数,usage.关系数,usage.属性值数};
        out.关系 = *hit ? 特征值域关系_v1::左包含右 : 特征值域关系_v1::不包含;
        out.状态 = 特征值域比较状态_v1::已核验;
    } catch (const std::bad_alloc&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (const std::length_error&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (...) { out.状态 = 特征值域比较状态_v1::内部不一致; }
    if (out.状态 != 特征值域比较状态_v1::已核验) { out.H = 0; out.关系.reset(); out.域.reset(); }
    return out;
}

特征值域读取结果_v2 特征值域比较数据服务::读取特征值域_v2(
    const 特征值域读取请求_v2 &r) const noexcept {
    特征值域读取结果_v2 out;
    out.Gread = r.Gread;
    const auto fail = [&](特征值域比较状态_v2 state) {
        out.状态 = state;
        out.H = 0;
        out.域.reset();
    };
    try {
        if (r.版本 != 2 || !r.Gread || !r.H || r.H > r.Gread || !有效(r.FC.值)) {
            fail(特征值域比较状态_v2::入口拒绝);
            return out;
        }
        const 特征概念值域基础读取请求_v2 baseRequest{2, r.Gread, r.H, r.FC};
        const auto base = concepts_.读取特征概念值域基础_v2(baseRequest);
        if (!base.成功(baseRequest) || !base.事实) {
            fail(映射基础状态_v2(base.状态));
            return out;
        }
        const auto *definition =
            std::get_if<纯I64特征概念定义>(&base.事实->完整纯概念事实.定义);
        // 待实现：I64组、U64组和独立材料的完整域适配器尚未交付。
        if (!definition) {
            fail(特征值域比较状态_v2::未实现);
            return out;
        }
        特征规范I64域 raw;
        raw.区间.reserve(definition->规范域.size());
        for (const auto &interval : definition->规范域)
            raw.区间.push_back({interval.下界, interval.上界});
        const auto provider = features_.保护<std::pair<I64基础特征类型信息,
                                                       特征规范I64域>>([&] {
            features_.截止有效(1, r.Gread, r.H);
            features_.守卫(r.Gread);
            auto type = features_.读类型(base.事实->FT, r.Gread, r.H);
            if (!type.规则) throw 特征数据错误::规则缺失;
            auto full = features_.读完整域(base.事实->FT, r.Gread, r.H);
            auto domain = features_.规范域(raw);
            features_.要求(features_.包含(full, domain), 特征数据错误::类型不相容);
            features_.守卫(r.Gread);
            return std::pair{std::move(type), std::move(domain)};
        });
        const auto *provided =
            std::get_if<std::pair<I64基础特征类型信息, 特征规范I64域>>(&provider);
        if (!provided) {
            fail(映射特征错误_v2(std::get<特征数据错误>(provider)));
            return out;
        }
        if (!provided->first.规则 || *provided->first.规则 != base.事实->规则身份 ||
            base.事实->规则版本 != 1 || provided->second.区间.empty()) {
            fail(特征值域比较状态_v2::内部不一致);
            return out;
        }
        out.H = r.H;
        out.域 = 特征值域事实_v2{
            r.FC, base.事实->FT, base.事实->原始表示, *provided->first.规则, 1,
            特征规范值域_v2{provided->second}, *base.事实};
        out.状态 = 特征值域比较状态_v2::已读取;
        if (!out.成功(r)) fail(特征值域比较状态_v2::内部不一致);
    } catch (const std::bad_alloc &) {
        fail(特征值域比较状态_v2::资源失败);
    } catch (const std::length_error &) {
        fail(特征值域比较状态_v2::资源失败);
    } catch (...) {
        fail(特征值域比较状态_v2::内部不一致);
    }
    return out;
}

特征值域关系结果_v2 特征值域比较数据服务::核验特征值域关系_v2(
    const 特征值域关系核验请求_v2 &r) const noexcept {
    特征值域关系结果_v2 out;
    out.Gread = r.Gread;
    const auto fail = [&](特征值域比较状态_v2 state) {
        out.状态 = state;
        out.H = 0;
        out.关系.reset();
        out.左域.reset();
        out.右域.reset();
    };
    try {
        const 特征值域读取请求_v2 leftRequest{2, r.Gread, r.H, r.左FC};
        const 特征值域读取请求_v2 rightRequest{2, r.Gread, r.H, r.右FC};
        if (r.版本 != 2 || !r.Gread || !r.H || r.H > r.Gread ||
            !有效(r.左FC.值) || !有效(r.右FC.值)) {
            fail(特征值域比较状态_v2::入口拒绝);
            return out;
        }
        const auto left = 读取特征值域_v2(leftRequest);
        if (!left.成功(leftRequest)) {
            fail(left.状态);
            return out;
        }
        const auto right = 读取特征值域_v2(rightRequest);
        if (!right.成功(rightRequest)) {
            fail(right.状态);
            return out;
        }
        if (left.域->FT != right.域->FT ||
            left.域->原始表示 != right.域->原始表示 ||
            left.域->规则身份 != right.域->规则身份 ||
            left.域->规则版本 != right.域->规则版本) {
            fail(特征值域比较状态_v2::类型不相容);
            return out;
        }
        const auto *leftI64 = std::get_if<特征规范I64域>(&left.域->规范化值域);
        const auto *rightI64 = std::get_if<特征规范I64域>(&right.域->规范化值域);
        if (!leftI64 || !rightI64) {
            fail(特征值域比较状态_v2::未实现);
            return out;
        }
        const bool leftContains = features_.包含(*leftI64, *rightI64);
        const bool rightContains = features_.包含(*rightI64, *leftI64);
        out.H = r.H;
        out.左域 = left.域;
        out.右域 = right.域;
        out.关系 = leftContains && rightContains ? 特征值域关系_v2::相等
                   : leftContains ? 特征值域关系_v2::左包含右
                   : rightContains ? 特征值域关系_v2::右包含左
                                   : 特征值域关系_v2::不包含;
        out.状态 = 特征值域比较状态_v2::已核验;
        if (!out.成功(r)) fail(特征值域比较状态_v2::内部不一致);
    } catch (const std::bad_alloc &) {
        fail(特征值域比较状态_v2::资源失败);
    } catch (const std::length_error &) {
        fail(特征值域比较状态_v2::资源失败);
    } catch (...) {
        fail(特征值域比较状态_v2::内部不一致);
    }
    return out;
}

实例值域命中结果_v2 特征值域比较数据服务::核验实例值域命中_v2(
    const 实例值域命中核验请求_v2 &r) const noexcept {
    实例值域命中结果_v2 out;
    out.Gread = r.Gread;
    const auto fail = [&](特征值域比较状态_v2 state) {
        out.状态 = state;
        out.H = 0;
        out.关系.reset();
        out.域.reset();
    };
    try {
        const 特征值域读取请求_v2 domainRequest{2, r.Gread, r.H, r.FC};
        if (r.版本 != 2 || !r.Gread || !r.H || r.H > r.Gread ||
            !有效(r.F) || !有效(r.FC.值)) {
            fail(特征值域比较状态_v2::入口拒绝);
            return out;
        }
        const auto domain = 读取特征值域_v2(domainRequest);
        if (!domain.成功(domainRequest)) {
            fail(domain.状态);
            return out;
        }
        const auto *i64 = std::get_if<特征规范I64域>(&domain.域->规范化值域);
        if (!i64) {
            fail(特征值域比较状态_v2::未实现);
            return out;
        }
        const auto matched = features_.保护<bool>([&] {
            features_.截止有效(1, r.Gread, r.H);
            features_.守卫(r.Gread);
            const auto actual = features_.读准确(r.F, r.Gread, r.H);
            features_.要求(actual.信息.类型 == domain.域->FT,
                           特征数据错误::类型不相容);
            const auto value = features_.完整整数(actual);
            const auto hit = features_.包含(*i64, 特征规范I64域{{{value, value}}});
            features_.守卫(r.Gread);
            return hit;
        });
        const auto *hit = std::get_if<bool>(&matched);
        if (!hit) {
            fail(映射特征错误_v2(std::get<特征数据错误>(matched)));
            return out;
        }
        out.H = r.H;
        out.域 = domain.域;
        out.关系 = *hit ? 特征值域关系_v2::左包含右
                        : 特征值域关系_v2::不包含;
        out.状态 = 特征值域比较状态_v2::已核验;
        if (!out.成功(r)) fail(特征值域比较状态_v2::内部不一致);
    } catch (const std::bad_alloc &) {
        fail(特征值域比较状态_v2::资源失败);
    } catch (const std::length_error &) {
        fail(特征值域比较状态_v2::资源失败);
    } catch (...) {
        fail(特征值域比较状态_v2::内部不一致);
    }
    return out;
}

// 待实现：I64组值域专用适配器。无任何写端口、写集或成功载荷。
特征值域读取结果_v1 特征值域比较数据服务::读取I64组值域待实现(
    const 特征值域读取请求_v1& r, const 特征概念值域基础事实_v1&) const {
    特征值域读取结果_v1 out; out.Gread = r.Gread;
    out.状态 = 特征值域比较状态_v1::未实现;
    return out;
}
// 待实现：U64组值域专用适配器。无任何写端口、写集或成功载荷。
特征值域读取结果_v1 特征值域比较数据服务::读取U64组值域待实现(
    const 特征值域读取请求_v1& r, const 特征概念值域基础事实_v1&) const {
    特征值域读取结果_v1 out; out.Gread = r.Gread;
    out.状态 = 特征值域比较状态_v1::未实现;
    return out;
}
// 待实现：独立材料值域专用适配器。无任何写端口、写集或成功载荷。
特征值域读取结果_v1 特征值域比较数据服务::读取独立材料值域待实现(
    const 特征值域读取请求_v1& r, const 特征概念值域基础事实_v1&) const {
    特征值域读取结果_v1 out; out.Gread = r.Gread;
    out.状态 = 特征值域比较状态_v1::未实现;
    return out;
}
} // namespace 海中鱼巣
