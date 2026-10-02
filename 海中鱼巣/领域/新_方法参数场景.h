#pragma once

#include <optional>
#include <vector>

#include "新_方法公共类型.h"

namespace 海中鱼巣 {

class 新_方法类;

struct 新方法实际特征绑定 final {
    稳定编码 特征节点;
    friend bool operator==(const 新方法实际特征绑定&,
        const 新方法实际特征绑定&) = default;
};

struct 新方法参数角色绑定 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 世界场景节点;
    std::optional<稳定编码> 实际存在节点;
    std::vector<新方法实际特征绑定> 实际特征组;
    friend bool operator==(const 新方法参数角色绑定&,
        const 新方法参数角色绑定&) = default;
};

class 方法参数场景 final {
public:
    稳定编码 方法节点() const noexcept;
    const std::vector<新方法参数角色绑定>& 全部角色绑定() const noexcept;
    const 新方法参数角色绑定* 查询角色(
        稳定编码 参数角色节点) const noexcept;

private:
    friend class 新_方法类;
    方法参数场景(
        稳定编码 方法节点,
        std::vector<新方法参数角色绑定> 角色绑定组) noexcept;

    稳定编码 方法节点_;
    std::vector<新方法参数角色绑定> 角色绑定组_;
};

} // namespace 海中鱼巣
