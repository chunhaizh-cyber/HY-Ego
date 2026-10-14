#pragma once
#include "../领域/新_因果公共类型.h"
#include "../领域/概念_特征类.h"
#include <cstddef>
#include <optional>
#include <variant>
#include <vector>

namespace 海中鱼巣
{
class 新_因果类;
class 新_特征类;
class 概念_存在类;
class 概念_动态类;
class 新_动态类;
class 新_存在类;
class 新_场景类;

struct 反向因果参与者
{
    std::int64_t 标识 = 0;
    std::optional<稳定编码> 实际存在节点, 存在概念节点;
    std::optional<新任务虚拟存在引用> 任务虚拟存在引用;
    friend bool operator==(const 反向因果参与者 &, const 反向因果参与者 &) = default;
};
struct 反向因果准确约束
{
    新特征准确值 值;
    friend bool operator==(const 反向因果准确约束 &, const 反向因果准确约束 &) = default;
};
struct 反向因果概念域约束
{
    稳定编码 值域概念节点;
    friend bool operator==(const 反向因果概念域约束 &, const 反向因果概念域约束 &) = default;
};
struct 反向因果直接I64区间约束
{
    特征概念I64闭区间 值域;
    friend bool operator==(const 反向因果直接I64区间约束 &,
                           const 反向因果直接I64区间约束 &) = default;
};
using 反向因果状态约束 =
    std::variant<反向因果准确约束, 反向因果概念域约束, 反向因果直接I64区间约束>;
struct 反向因果状态条件
{
    std::int64_t 参与者标识 = 0;
    稳定编码 特征类型概念节点;
    反向因果状态约束 约束;
    friend bool operator==(const 反向因果状态条件 &, const 反向因果状态条件 &) = default;
};
struct 反向因果动态条件
{
    std::int64_t 参与者标识 = 0;
    稳定编码 动态概念节点;
    friend bool operator==(const 反向因果动态条件 &, const 反向因果动态条件 &) = default;
};
struct 反向因果动作条件
{
    稳定编码 动作动态节点;
    friend bool operator==(const 反向因果动作条件 &, const 反向因果动作条件 &) = default;
};
using 反向因果起点条件 = std::variant<反向因果状态条件, 反向因果动态条件, 反向因果动作条件>;
struct 反向因果显式角色绑定
{
    稳定编码 因果节点;
    std::int64_t 分支序号 = 0, 局部角色 = 0, 参与者标识 = 0;
    friend bool operator==(const 反向因果显式角色绑定 &, const 反向因果显式角色绑定 &) = default;
};
struct 反向因果查询请求
{
    std::vector<反向因果参与者> 参与者组;
    反向因果状态条件 目标结果;
    std::optional<std::vector<反向因果起点条件>> 起点条件组;
    std::vector<反向因果显式角色绑定> 角色绑定组;
    std::optional<稳定编码> 共同场景节点;
    friend bool operator==(const 反向因果查询请求 &, const 反向因果查询请求 &) = default;
};
enum class 反向因果覆盖 : std::uint8_t
{
    未指定起点 = 1,
    完整,
    部分,
    无已知路径
};
enum class 反向因果端类型 : std::uint8_t
{
    状态 = 1,
    动态,
    动作,
    场景,
    参与者绑定
};
enum class 反向因果未解释原因 : std::uint8_t
{
    无更前规则 = 1,
    未绑定递归角色边界,
    比较材料尚不支持,
    动作参数未确认,
    具体实体待绑定
};
enum class 反向因果参与者键类 : std::uint8_t
{
    输入符号 = 1,
    实际存在,
    规则局部符号,
    任务虚拟存在 = 4
};
struct 反向因果参与者键
{
    反向因果参与者键类 类别 = 反向因果参与者键类::输入符号;
    std::int64_t 输入标识 = 0;
    稳定编码 实际存在节点, 规则因果节点;
    std::int64_t 规则分支 = 0, 局部角色 = 0;
    std::optional<新任务虚拟存在引用> 任务虚拟存在引用;
    friend bool operator==(const 反向因果参与者键 &, const 反向因果参与者键 &) = default;
};
struct 反向因果图角色绑定
{
    std::int64_t 局部角色 = 0;
    反向因果参与者键 参与者;
    friend bool operator==(const 反向因果图角色绑定 &, const 反向因果图角色绑定 &) = default;
};
struct 反向因果图节点
{
    std::size_t 编号 = 0;
    反向因果端类型 类型 = 反向因果端类型::状态;
    std::optional<反向因果参与者键> 参与者;
    std::optional<反向因果状态条件> 状态;
    std::optional<稳定编码> 动态概念节点, 动作概念节点, 场景节点;
    std::optional<稳定编码> 待绑定实际存在节点;
    bool 起点已覆盖 = false, 完整覆盖 = false, 角色尚未实际绑定 = false;
    std::optional<反向因果未解释原因> 未解释原因;
    std::optional<新任务虚拟存在引用> 待绑定任务虚拟存在;
    friend bool operator==(const 反向因果图节点 &, const 反向因果图节点 &) = default;
};
struct 反向因果图分支
{
    std::size_t 编号 = 0, 结果节点 = 0;
    稳定编码 因果节点;
    std::int64_t 来源分支 = 0;
    bool 概念因果 = false;
    std::vector<反向因果图角色绑定> 角色映射;
    std::vector<std::size_t> 全部前提节点;
    bool 完整覆盖 = false;
    friend bool operator==(const 反向因果图分支 &, const 反向因果图分支 &) = default;
};
struct 反向因果环边
{
    std::size_t 前提节点 = 0, 返回节点 = 0;
    friend bool operator==(const 反向因果环边 &, const 反向因果环边 &) = default;
};
struct 反向因果未绑定角色边界
{
    std::size_t 前提节点 = 0;
    稳定编码 因果节点;
    std::int64_t 来源分支 = 0, 局部角色 = 0;
    std::vector<反向因果图角色绑定> 已有角色映射;
    friend bool operator==(const 反向因果未绑定角色边界 &,
                           const 反向因果未绑定角色边界 &) = default;
};
struct 反向因果查询结果
{
    反向因果覆盖 覆盖 = 反向因果覆盖::无已知路径;
    std::size_t 目标节点 = 0;
    std::vector<反向因果图节点> 节点组;
    std::vector<反向因果图分支> 分支组;
    std::vector<std::size_t> 已知最前端节点组, 未覆盖节点组;
    std::vector<反向因果环边> 环边组;
    std::vector<反向因果未绑定角色边界> 未绑定递归角色边界组;
    std::vector<稳定编码> 已不相容概念因果组;
    bool 材料已变化 = false;
    friend bool operator==(const 反向因果查询结果 &, const 反向因果查询结果 &) = default;
};
class 因果反向查询服务 final
{
  public:
    因果反向查询服务(const 新_因果类 &, const 新_特征类 &, const 概念_特征类 &, const 概念_存在类 &,
                     const 概念_动态类 &, const 新_动态类 &, const 新_存在类 &,
                     const 新_场景类 &, const 新治理参与者读取接口 * = nullptr) noexcept;
    反向因果查询结果 查询(const 反向因果查询请求 &) const noexcept;

  private:
    const 新_因果类 &因果服务_;
    const 新_特征类 &特征服务_;
    const 概念_特征类 &特征概念服务_;
    const 概念_存在类 &存在概念服务_;
    const 概念_动态类 &动态概念服务_;
    const 新_动态类 &动态服务_;
    const 新_存在类 &存在服务_;
    const 新_场景类 &场景服务_;
    const 新治理参与者读取接口 *治理参与者_ = nullptr;
};
} // namespace 海中鱼巣
