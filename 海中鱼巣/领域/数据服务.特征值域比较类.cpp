#include "数据服务.特征值域比较类.h"

#include <algorithm>
#include <new>
#include <stdexcept>
#include <type_traits>

namespace 海中鱼巣 {
namespace {

bool 规范值域完整(const 特征规范值域& domain) noexcept {
    return std::visit([](const auto& value) noexcept {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, 特征规范I64域>) {
            return !value.区间.empty();
        } else if constexpr (std::is_same_v<T, 特征I64组有限域> ||
                             std::is_same_v<T, 特征U64组有限域>) {
            return !value.点组.empty();
        } else {
            return !value.材料组.empty() &&
                std::all_of(value.材料组.begin(), value.材料组.end(),
                    [](const 特征独立材料域项& item) noexcept {
                        return 有效(item.格式) && !item.完整载荷.empty();
                    });
        }
    }, domain);
}

特征值域比较状态 映射基础读取状态(
    特征概念值域基础读取状态 state) noexcept {
    switch (state) {
    case 特征概念值域基础读取状态::未找到:
        return 特征值域比较状态::未找到;
    case 特征概念值域基础读取状态::类别冲突:
        return 特征值域比较状态::类别冲突;
    case 特征概念值域基础读取状态::规则缺失:
        return 特征值域比较状态::规则缺失;
    case 特征概念值域基础读取状态::未实现:
        return 特征值域比较状态::未实现;
    case 特征概念值域基础读取状态::资源失败:
        return 特征值域比较状态::资源失败;
    case 特征概念值域基础读取状态::入口拒绝:
        return 特征值域比较状态::入口拒绝;
    default:
        return 特征值域比较状态::内部不一致;
    }
}

} // namespace

特征值域比较数据服务::特征值域比较数据服务(
    const 概念树类数据服务& concepts,
    const 特征类数据服务& features,
    const 特征值类数据服务& values) noexcept
    : concepts_(concepts), features_(features), values_(values) {}

bool 特征值域比较数据服务::绑定于(
    const L1事实基座服务& l1) const noexcept {
    return concepts_.绑定于(l1) && features_.绑定于(l1) && values_.绑定于(l1);
}

bool 特征值域读取结果::成功(
    const 特征值域读取请求& request) const noexcept {
    return 有效(request.FC.值) && 状态 == 特征值域比较状态::已读取 &&
        域 && 域->FC == request.FC && 有效(域->FT) &&
        有效(域->规则身份) && 域->规则版本 != 0 &&
        域->基础读回.FC == request.FC &&
        域->基础读回.FT == 域->FT.编码 &&
        规范值域完整(域->规范化值域);
}

bool 特征值域关系结果::成功(
    const 特征值域关系核验请求& request) const noexcept {
    return 有效(request.左FC.值) && 有效(request.右FC.值) &&
        状态 == 特征值域比较状态::已核验 && 关系 && 左域 && 右域 &&
        左域->FC == request.左FC && 右域->FC == request.右FC &&
        左域->FT == 右域->FT && 左域->原始表示 == 右域->原始表示 &&
        左域->规则身份 == 右域->规则身份 &&
        左域->规则版本 == 右域->规则版本 &&
        规范值域完整(左域->规范化值域) &&
        规范值域完整(右域->规范化值域);
}

bool 实例值域命中结果::成功(
    const 实例值域命中核验请求& request) const noexcept {
    return 有效(request.F) && 有效(request.FC.值) &&
        状态 == 特征值域比较状态::已核验 && 关系 && 域 &&
        域->FC == request.FC &&
        (*关系 == 特征值域关系::相等 ||
         *关系 == 特征值域关系::左包含右 ||
         *关系 == 特征值域关系::不包含) &&
        规范值域完整(域->规范化值域);
}

特征值域读取结果 特征值域比较数据服务::读取特征值域(
    const 特征值域读取请求& request) const noexcept {
    特征值域读取结果 result;
    if (!有效(request.FC.值)) return result;

    try {
        const 特征概念值域基础读取请求 baseRequest{request.FC};
        const auto base = concepts_.读取特征概念值域基础(baseRequest);
        if (!base.成功(baseRequest)) {
            result.状态 = 映射基础读取状态(base.状态);
            return result;
        }

        // 待实现：概念、特征类型和完整值域尚无一次 current-only 组合读取。
        // 不能把多次独立读取伪装为共同快照，因此不构造成功值域。
        result.状态 = 特征值域比较状态::未实现;
    } catch (const std::bad_alloc&) {
        result.状态 = 特征值域比较状态::资源失败;
    } catch (const std::length_error&) {
        result.状态 = 特征值域比较状态::资源失败;
    } catch (...) {
        result.状态 = 特征值域比较状态::内部不一致;
    }
    return result;
}

特征值域关系结果 特征值域比较数据服务::核验特征值域关系(
    const 特征值域关系核验请求& request) const noexcept {
    特征值域关系结果 result;
    if (!有效(request.左FC.值) || !有效(request.右FC.值)) return result;

    // 待实现：需要能一次读回两个完整值域及共同规则的 current-only 组合入口。
    result.状态 = 特征值域比较状态::未实现;
    return result;
}

实例值域命中结果 特征值域比较数据服务::核验实例值域命中(
    const 实例值域命中核验请求& request) const noexcept {
    实例值域命中结果 result;
    if (!有效(request.F) || !有效(request.FC.值)) return result;

    // 待实现：需要能一次读回准确特征、完整概念值域及规则的 current-only 组合入口。
    result.状态 = 特征值域比较状态::未实现;
    return result;
}

} // namespace 海中鱼巣
