#pragma once

#include <optional>
#include <vector>

#include "新_方法公共类型.h"
#include "新_特征类.h"
#include "新_特征值类.h"

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
    // 只允许读取本参数场景已经绑定的特征；每次调用都从特征owner
    // 取得当前信息，不保存调用前值的副本。
    std::optional<新特征信息> 读取绑定特征(
        稳定编码 特征节点) const noexcept;
    // 只允许读取被某个绑定特征当前值引用的非标量材料节点。
    std::optional<新特征值信息> 读取绑定特征值材料(
        稳定编码 特征值节点) const noexcept;

private:
    friend class 新_方法类;
    方法参数场景(
        稳定编码 方法节点,
        std::vector<新方法参数角色绑定> 角色绑定组,
        const 新_特征类& 特征服务,
        const 新_特征值类& 特征值服务) noexcept;

    稳定编码 方法节点_;
    std::vector<新方法参数角色绑定> 角色绑定组_;
    const 新_特征类* 特征服务_;
    const 新_特征值类* 特征值服务_;
};

} // namespace 海中鱼巣
