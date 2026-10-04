#include "新_方法参数场景.h"

#include <utility>

namespace 海中鱼巣 {

方法参数场景::方法参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 角色绑定组,
    const 新_特征类& 特征服务,
    const 新_特征值类& 特征值服务) noexcept
    : 方法节点_(方法节点),
      角色绑定组_(std::move(角色绑定组)),
      特征服务_(&特征服务),
      特征值服务_(&特征值服务) {}

稳定编码 方法参数场景::方法节点() const noexcept {
    return 方法节点_;
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

std::optional<新特征信息> 方法参数场景::读取绑定特征(
    稳定编码 特征节点) const noexcept {
    if (!特征服务_ || !有效(特征节点)) return std::nullopt;
    for (const auto& 角色 : 角色绑定组_) {
        for (const auto& 特征 : 角色.实际特征组) {
            if (特征.特征节点 == 特征节点) {
                return 特征服务_->获取特征(特征节点);
            }
        }
    }
    return std::nullopt;
}

std::optional<新特征值信息> 方法参数场景::读取绑定特征值材料(
    稳定编码 特征值节点) const noexcept {
    if (!特征值服务_ || !有效(特征值节点)) return std::nullopt;
    for (const auto& 角色 : 角色绑定组_) {
        for (const auto& 特征绑定 : 角色.实际特征组) {
            const auto 特征 = 读取绑定特征(特征绑定.特征节点);
            if (!特征 || !特征->当前值) continue;
            const auto* 材料节点 = std::get_if<稳定编码>(&*特征->当前值);
            if (材料节点 && *材料节点 == 特征值节点) {
                return 特征值服务_->获取特征值(特征值节点);
            }
        }
    }
    return std::nullopt;
}

} // namespace 海中鱼巣
