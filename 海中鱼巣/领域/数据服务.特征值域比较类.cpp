#include "数据服务.特征值域比较类.h"

namespace 海中鱼巣 {
namespace {
bool 基础预算有效(const 概念树预算& b) noexcept {
    return b.最大概念数 && b.最大关系数 && b.最大特征属性数;
}
bool 读取请求有效(const 特征值域读取请求_v1& r) noexcept {
    return r.版本 == 1 && r.Gread && r.H && r.H <= r.Gread && 有效(r.FC.值)
        && 基础预算有效(r.预算.概念预算) && r.预算.最大域项数;
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
}

特征值域比较数据服务::特征值域比较数据服务(
    const 概念树类数据服务& concepts, const 特征类数据服务& features,
    const 特征值类数据服务& values) noexcept
    : concepts_(concepts), features_(features), values_(values) {}

bool 特征值域读取结果_v1::成功(const 特征值域读取请求_v1& r) const noexcept {
    return 版本 == 1 && 读取请求有效(r) && 状态 == 特征值域比较状态_v1::已读取
        && Gread == r.Gread && H == r.H && 域 && 域->FC == r.FC && 有效(域->FT)
        && 有效(域->规则) && 域->表示 == 特征值表示类型::I64 && !域->I64域.区间.empty()
        && 域->基础读回.FC == r.FC && 域->基础读回.Gread == r.Gread && 域->基础读回.H == r.H;
}

bool 特征值域关系结果_v1::成功(const 特征值域关系核验请求_v1& r) const noexcept {
    特征值域读取请求_v1 left{1, r.Gread, r.H, r.左FC, r.预算};
    特征值域读取请求_v1 right{1, r.Gread, r.H, r.右FC, r.预算};
    return 版本 == 1 && r.版本 == 1 && 读取请求有效(left) && 读取请求有效(right)
        && 状态 == 特征值域比较状态_v1::已核验 && Gread == r.Gread && H == r.H
        && 关系 && 左域 && 右域 && 左域->FC == r.左FC && 右域->FC == r.右FC
        && 左域->FT == 右域->FT && 左域->表示 == 右域->表示;
}

bool 实例值域命中结果_v1::成功(const 实例值域命中核验请求_v1& r) const noexcept {
    特征值域读取请求_v1 fc{1, r.Gread, r.H, r.FC, r.预算};
    return 版本 == 1 && r.版本 == 1 && r.F.编码.值 && 读取请求有效(fc)
        && 状态 == 特征值域比较状态_v1::已核验 && Gread == r.Gread && H == r.H
        && 关系 && (*关系 == 特征值域关系_v1::左包含右 || *关系 == 特征值域关系_v1::不包含)
        && 域 && 域->FC == r.FC;
}

特征值域读取结果_v1 特征值域比较数据服务::读取特征值域(
    const 特征值域读取请求_v1& r) const {
    特征值域读取结果_v1 out; out.Gread = r.Gread;
    try {
        if (!读取请求有效(r)) return out;
        const auto base = concepts_.读取特征概念值域基础({1, r.Gread, r.H, r.FC, r.预算.概念预算});
        if (!base.成功({1, r.Gread, r.H, r.FC, r.预算.概念预算}) || !base.事实) {
            out.状态 = 映射基础状态(base.状态); return out;
        }
        const auto& fact = *base.事实;
        if (fact.表示 == 特征值表示类型::I64组) return 读取I64组值域待实现(r, fact);
        if (fact.表示 == 特征值表示类型::U64组) return 读取U64组值域待实现(r, fact);
        if (fact.表示 == 特征值表示类型::独立材料引用) return 读取独立材料值域待实现(r, fact);
        if (fact.表示 != 特征值表示类型::I64 || !fact.I64域 || fact.I64域->区间.empty()
            || fact.I64域->区间.size() > r.预算.最大域项数) {
            out.状态 = 特征值域比较状态_v1::内部不一致; return out;
        }
        const auto type = features_.读取先天I64特征类型事实({1, r.Gread, r.H, fact.FT});
        const auto* typeFact = std::get_if<特征截止事实<先天I64特征类型信息>>(&type);
        if (!typeFact) { out.状态 = 映射特征错误(std::get<特征数据错误>(type)); return out; }
        if (typeFact->Gread != r.Gread || typeFact->H != r.H) {
            out.状态 = 特征值域比较状态_v1::事实代次漂移; return out;
        }
        if (!typeFact->数据.规则) { out.状态 = 特征值域比较状态_v1::规则缺失; return out; }
        const auto normalized = features_.规范化I64特征域({{1, r.Gread, r.H, fact.FT}, *fact.I64域});
        const auto* domain = std::get_if<特征截止事实<特征规范I64域>>(&normalized);
        if (!domain) { out.状态 = 映射特征错误(std::get<特征数据错误>(normalized)); return out; }
        if (domain->Gread != r.Gread || domain->H != r.H || domain->数据.区间.empty()
            || domain->数据.区间.size() > r.预算.最大域项数) {
            out.状态 = 特征值域比较状态_v1::事实代次漂移; return out;
        }
        out.H = r.H;
        out.域 = 特征值域事实_v1{r.FC, fact.FT, fact.表示, *typeFact->数据.规则,
            domain->数据, fact, {}};
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
    try {
        const 特征值域读取请求_v1 leftRequest{1, r.Gread, r.H, r.左FC, r.预算};
        const 特征值域读取请求_v1 rightRequest{1, r.Gread, r.H, r.右FC, r.预算};
        if (r.版本 != 1 || !读取请求有效(leftRequest) || !读取请求有效(rightRequest)) return out;
        const auto left = 读取特征值域(leftRequest);
        if (!left.成功(leftRequest)) { out.状态 = left.状态; return out; }
        const auto right = 读取特征值域(rightRequest);
        if (!right.成功(rightRequest)) { out.状态 = right.状态; return out; }
        if (left.域->FT != right.域->FT || left.域->表示 != right.域->表示) {
            out.状态 = 特征值域比较状态_v1::类型不相容; return out;
        }
        const auto containsLeft = features_.判定I64域包含({{1, r.Gread, r.H, left.域->FT}, left.域->I64域, right.域->I64域});
        const auto containsRight = features_.判定I64域包含({{1, r.Gread, r.H, left.域->FT}, right.域->I64域, left.域->I64域});
        const auto* l = std::get_if<特征截止事实<bool>>(&containsLeft);
        const auto* rr = std::get_if<特征截止事实<bool>>(&containsRight);
        if (!l || !rr) { out.状态 = 映射特征错误(l ? std::get<特征数据错误>(containsRight) : std::get<特征数据错误>(containsLeft)); return out; }
        if (l->Gread != r.Gread || l->H != r.H || rr->Gread != r.Gread || rr->H != r.H) {
            out.状态 = 特征值域比较状态_v1::事实代次漂移; return out;
        }
        out.H = r.H; out.左域 = left.域; out.右域 = right.域;
        out.关系 = l->数据 && rr->数据 ? 特征值域关系_v1::相等 : l->数据 ? 特征值域关系_v1::左包含右 :
            rr->数据 ? 特征值域关系_v1::右包含左 : 特征值域关系_v1::不包含;
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
    try {
        const 特征值域读取请求_v1 fcRequest{1, r.Gread, r.H, r.FC, r.预算};
        if (r.版本 != 1 || !有效(r.F) || !读取请求有效(fcRequest)) return out;
        const auto domain = 读取特征值域(fcRequest);
        if (!domain.成功(fcRequest)) { out.状态 = domain.状态; return out; }
        const auto feature = features_.读取准确特征(r.F);
        const auto* actual = std::get_if<特征信息>(&feature);
        if (!actual) { out.状态 = 映射特征错误(std::get<特征数据错误>(feature)); return out; }
        if (actual->类型 != domain.域->FT) { out.状态 = 特征值域比较状态_v1::类型不相容; return out; }
        const auto hit = features_.判定准确特征命中域({{1, r.Gread, r.H, r.F}, domain.域->I64域});
        const auto* matched = std::get_if<特征截止事实<bool>>(&hit);
        if (!matched) { out.状态 = 映射特征错误(std::get<特征数据错误>(hit)); return out; }
        if (matched->Gread != r.Gread || matched->H != r.H) { out.状态 = 特征值域比较状态_v1::事实代次漂移; return out; }
        out.H = r.H; out.域 = domain.域;
        out.关系 = matched->数据 ? 特征值域关系_v1::左包含右 : 特征值域关系_v1::不包含;
        out.状态 = 特征值域比较状态_v1::已核验;
    } catch (const std::bad_alloc&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (const std::length_error&) { out.状态 = 特征值域比较状态_v1::资源失败; }
      catch (...) { out.状态 = 特征值域比较状态_v1::内部不一致; }
    if (out.状态 != 特征值域比较状态_v1::已核验) { out.H = 0; out.关系.reset(); out.域.reset(); }
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
