#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "新_方法公共类型.h"
#include "新_内部治理对象引用.h"
#include "新_特征类.h"
#include "新_特征值类.h"

namespace 海中鱼巣 {

class 新_方法类;
class 新服务值结算本能端口;

enum class 新方法参数场景用途 : std::uint8_t {
    登记 = 1,
    执行 = 2,
    模拟 = 3
};

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
    std::optional<新任务虚拟存在引用> 任务虚拟存在引用;
    friend bool operator==(const 新方法参数角色绑定&,
        const 新方法参数角色绑定&) = default;
};

// 一个调用期所有者同时提供只读投影与隔离写端口，不能拼接不同样例。
class 新方法模拟运行接口 : public 新内部治理对象读取接口 {
public:
    virtual 稳定编码 模拟根节点() const noexcept = 0;
    virtual bool 核验模拟参数(稳定编码 方法节点,
        const std::vector<新方法参数角色绑定>&) const noexcept = 0;
    virtual std::optional<新特征信息> 读取模拟特征(稳定编码) const noexcept = 0;
    virtual std::optional<新特征值信息> 读取模拟特征值材料(稳定编码) const noexcept = 0;
    virtual 新服务值结算本能端口* 获取服务值结算模拟端口() noexcept = 0;
};

class 方法参数场景 final {
public:
    稳定编码 方法节点() const noexcept;
    新方法参数场景用途 用途() const noexcept;
    新服务值结算本能端口* 获取服务值结算模拟端口() const noexcept;
    const std::vector<新方法参数角色绑定>& 全部角色绑定() const noexcept;
    const 新方法参数角色绑定* 查询角色(
        稳定编码 参数角色节点) const noexcept;
    std::optional<新任务虚拟存在读回> 读取绑定任务虚拟存在(
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
        const 新_特征值类& 特征值服务,
        新方法参数场景用途 用途,
        const 新内部治理对象读取接口* 治理对象读取,
        std::shared_ptr<新方法模拟运行接口> 模拟运行 = {}) noexcept;

    稳定编码 方法节点_;
    std::vector<新方法参数角色绑定> 角色绑定组_;
    const 新_特征类* 特征服务_;
    const 新_特征值类* 特征值服务_;
    新方法参数场景用途 用途_;
    // 非拥有引用；调用方保证读取owner覆盖整个参数场景寿命。
    const 新内部治理对象读取接口* 治理对象读取_;
    std::shared_ptr<新方法模拟运行接口> 模拟运行_;
};

} // namespace 海中鱼巣
