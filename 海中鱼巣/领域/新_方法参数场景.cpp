#include "新_方法参数场景.h"

#include <utility>

namespace 海中鱼巣 {

方法参数场景::方法参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 角色绑定组,
    const 新_特征类& 特征服务,
    const 新_特征值类& 特征值服务,
    新方法参数场景用途 用途,
    const 新内部治理对象读取接口* 治理对象读取,
    std::shared_ptr<新方法模拟运行接口> 模拟运行) noexcept
    : 方法节点_(方法节点),
      角色绑定组_(std::move(角色绑定组)),
      特征服务_(&特征服务),
      特征值服务_(&特征值服务),
      用途_(用途),
      治理对象读取_(治理对象读取),
      模拟运行_(std::move(模拟运行)) {}

稳定编码 方法参数场景::方法节点() const noexcept {
    return 方法节点_;
}

新方法参数场景用途 方法参数场景::用途() const noexcept {
    return 用途_;
}

新服务值结算本能端口* 方法参数场景::获取服务值结算模拟端口() const noexcept {
    if (用途_ != 新方法参数场景用途::模拟 || !模拟运行_
        || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_)) return nullptr;
    return 模拟运行_->获取服务值结算模拟端口();
}

const std::vector<新方法参数角色绑定>&
方法参数场景::全部角色绑定() const noexcept {
    return 角色绑定组_;
}

const 新方法参数角色绑定* 方法参数场景::查询角色(
    稳定编码 参数角色节点) const noexcept {
    for (const auto& 绑定 : 角色绑定组_) {
        if (绑定.参数角色节点 == 参数角色节点) return &绑定;
    }
    return nullptr;
}

std::optional<新任务虚拟存在读回> 方法参数场景::读取绑定任务虚拟存在(
    稳定编码 参数角色节点) const noexcept {
    const auto* 绑定 = 查询角色(参数角色节点);
    const 新内部治理对象读取接口* 读取 = 治理对象读取_;
    if (用途_ == 新方法参数场景用途::模拟) {
        if (!模拟运行_ || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_))
            return std::nullopt;
        读取 = 模拟运行_.get();
    }
    if ((用途_ != 新方法参数场景用途::执行 && 用途_ != 新方法参数场景用途::模拟) || !读取
        || !绑定 || !绑定->任务虚拟存在引用
        || 绑定->世界场景节点 || 绑定->实际存在节点
        || 绑定->实际特征组.size() != 1
        || 绑定->实际特征组.front().特征节点
            != 绑定->任务虚拟存在引用->状态特征节点) return std::nullopt;
    const auto 读回 = 读取->读取任务虚拟存在引用(
        *绑定->任务虚拟存在引用);
    return 读回 && 读回->引用 == *绑定->任务虚拟存在引用
        ? 读回 : std::nullopt;
}

std::optional<新特征信息> 方法参数场景::读取绑定特征(
    稳定编码 特征节点) const noexcept {
    if (!有效(特征节点)) return std::nullopt;
    if (用途_ == 新方法参数场景用途::模拟) {
        if (!模拟运行_ || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_))
            return std::nullopt;
    } else if (!特征服务_) return std::nullopt;
    for (const auto& 角色 : 角色绑定组_) {
        for (const auto& 特征 : 角色.实际特征组) {
            if (特征.特征节点 == 特征节点) {
                if (角色.任务虚拟存在引用
                    && !读取绑定任务虚拟存在(角色.参数角色节点)) {
                    return std::nullopt;
                }
                return 用途_ == 新方法参数场景用途::模拟
                    ? 模拟运行_->读取模拟特征(特征节点)
                    : 特征服务_->获取特征(特征节点);
            }
        }
    }
    return std::nullopt;
}

std::optional<新特征值信息> 方法参数场景::读取绑定特征值材料(
    稳定编码 特征值节点) const noexcept {
    if (!有效(特征值节点)) return std::nullopt;
    if (用途_ == 新方法参数场景用途::模拟) {
        if (!模拟运行_ || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_))
            return std::nullopt;
    } else if (!特征值服务_) return std::nullopt;
    for (const auto& 角色 : 角色绑定组_) {
        for (const auto& 特征绑定 : 角色.实际特征组) {
            const auto 特征 = 读取绑定特征(特征绑定.特征节点);
            if (!特征 || !特征->当前值) continue;
            const auto* 材料节点 = std::get_if<稳定编码>(&*特征->当前值);
            if (材料节点 && *材料节点 == 特征值节点) {
                return 用途_ == 新方法参数场景用途::模拟
                    ? 模拟运行_->读取模拟特征值材料(特征值节点)
                    : 特征值服务_->获取特征值(特征值节点);
            }
        }
    }
    return std::nullopt;
}

} // namespace 海中鱼巣
