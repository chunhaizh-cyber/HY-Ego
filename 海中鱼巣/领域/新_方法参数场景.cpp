#include "新_方法参数场景.h"

#include <utility>

namespace 海中鱼巣 {

方法参数场景::方法参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 角色绑定组) noexcept
    : 方法节点_(方法节点),
      角色绑定组_(std::move(角色绑定组)) {}

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

} // namespace 海中鱼巣
