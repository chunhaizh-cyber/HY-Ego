#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_状态类.h"
#include "新_内部治理对象引用.h"

namespace 海中鱼巣
{

enum class 新因果类型 : std::int64_t
{
    未知动作 = 1,
    已知动作 = 2
};

enum class 新因果动作触发方式 : std::int64_t
{
    尚未确定 = 0,
    主动调用 = 1,
    被动触发 = 2
};

enum class 新因果形成来源 : std::int64_t
{
    结果状态筛选 = 1,
    结果动态反向 = 2,
    动作事实反向 = 3,
    外部动作动态反向 = 4
};

enum class 新因果操作状态 : std::uint8_t
{
    已记录首例 = 1,
    已建立 = 2,
    已更新 = 3,
    已找到 = 4,
    无匹配 = 5,
    重复发生 = 6,
    入口拒绝 = 7,
    尚未初始化 = 8,
    状态不存在 = 9,
    动态不存在 = 10,
    动作证据不存在 = 11,
    依赖未配置 = 12,
    无共同条件 = 13,
    因果不存在 = 14,
    计数溢出 = 15,
    结构不一致 = 16,
    资源失败 = 17
};

// 状态模式保留实际参与者、特征类型和当时准确值。后续抽象可以把它
// 提升为存在概念或特征值域，但不能丢失本次形成因果的原始含义。
struct 新因果状态模式 final
{
    稳定编码 被描述存在节点;
    稳定编码 特征类型概念节点;
    新特征准确值 准确值;
    friend bool operator==(const 新因果状态模式 &, const 新因果状态模式 &) = default;
};

struct 新因果动态模式 final
{
    稳定编码 主题存在节点;
    稳定编码 动态概念节点;
    friend bool operator==(const 新因果动态模式 &, const 新因果动态模式 &) = default;
};

// 动作必须有执行主体。作用部位、作用对象和本能方法只在已知时保存。
struct 新因果动作定义 final
{
    稳定编码 动作概念节点;
    稳定编码 执行主体存在节点;
    std::optional<稳定编码> 作用部位存在节点;
    std::optional<稳定编码> 作用对象存在节点;
    std::optional<稳定编码> 本能方法节点;
    std::optional<新任务虚拟存在引用> 作用对象任务虚拟存在;
    新因果动作触发方式 触发方式 = 新因果动作触发方式::尚未确定;
    friend bool operator==(const 新因果动作定义 &, const 新因果动作定义 &) = default;
};

struct 新因果定义 final
{
    新因果类型 类型 = 新因果类型::未知动作;
    std::vector<新因果状态模式> 条件状态组;
    std::vector<新因果动态模式> 条件动态组;
    std::optional<新因果动作定义> 动作;
    新因果状态模式 结果状态;
    friend bool operator==(const 新因果定义 &, const 新因果定义 &) = default;
};

// 发生身份由上层为同一次现实发生统一提供。不同发现路径使用相同身份，
// 从而保证一次发生最多计数一次。
struct 新因果发生记录 final
{
    稳定编码 发生身份;
    稳定编码 场景节点;
    std::vector<稳定编码> 前置状态节点组;
    std::vector<稳定编码> 前置动态节点组;
    稳定编码 结果状态节点;
    std::optional<稳定编码> 结果动态节点;
    std::optional<稳定编码> 动作证据节点;
};

struct 新因果统计 final
{
    std::int64_t 符合出现次数 = 0;
    std::int64_t 不符合出现次数 = 0;
    std::int64_t 自我尝试次数 = 0;
    std::int64_t 自我尝试符合次数 = 0;
    friend bool operator==(const 新因果统计 &, const 新因果统计 &) = default;
};

struct 新因果信息 final
{
    稳定编码 节点;
    新因果定义 定义;
    std::vector<新因果形成来源> 形成来源组;
    新因果统计 统计;
    std::vector<稳定编码> 已计数发生身份组;
    std::optional<稳定编码> 发生场景节点;
    friend bool operator==(const 新因果信息 &, const 新因果信息 &) = default;
};

struct 新因果概念角色
{
    std::int64_t 角色 = 0;
    稳定编码 存在概念节点;
    std::optional<新治理参与者种类> 治理类别;
    friend bool operator==(const 新因果概念角色 &, const 新因果概念角色 &) = default;
};
struct 新因果概念状态项
{
    std::int64_t 角色 = 0;
    稳定编码 特征类型概念节点;
    稳定编码 值域概念节点;
    friend bool operator==(const 新因果概念状态项 &, const 新因果概念状态项 &) = default;
};
struct 新因果概念动态项
{
    std::int64_t 角色 = 0;
    稳定编码 动态概念节点;
    friend bool operator==(const 新因果概念动态项 &, const 新因果概念动态项 &) = default;
};
struct 新因果概念动作项
{
    稳定编码 动作概念节点;
    std::int64_t 执行主体角色 = 0;
    std::optional<std::int64_t> 作用部位角色, 作用对象角色;
    新因果动作触发方式 触发方式 = 新因果动作触发方式::尚未确定;
    friend bool operator==(const 新因果概念动作项 &, const 新因果概念动作项 &) = default;
};
struct 新因果概念分支
{
    稳定编码 来源具体因果节点;
    std::vector<新因果概念状态项> 条件状态组;
    std::vector<新因果概念动态项> 条件动态组;
    新因果概念状态项 结果状态;
    friend bool operator==(const 新因果概念分支 &, const 新因果概念分支 &) = default;
};
struct 新概念因果定义
{
    新因果类型 类型 = 新因果类型::未知动作;
    稳定编码 场景概念节点;
    std::vector<新因果概念角色> 角色组;
    std::optional<新因果概念动作项> 动作;
    std::vector<新因果概念分支> 分支组;
    friend bool operator==(const 新概念因果定义 &, const 新概念因果定义 &) = default;
};
enum class 新因果共同抽象状态 : std::uint8_t
{
    已建立 = 1,
    已复用,
    来源不足,
    场景来源不足,
    无结构共性,
    角色对齐不唯一,
    无共同存在概念,
    精确值域尚不可表达,
    值不在合法类型域内,
    已读取,
    概念已不相容
};
struct 新概念因果信息
{
    稳定编码 节点;
    新概念因果定义 定义;
    std::vector<稳定编码> 来源发生身份组;
    std::int64_t 覆盖发生次数 = 0;
    friend bool operator==(const 新概念因果信息 &, const 新概念因果信息 &) = default;
};
struct 新因果共同抽象结果
{
    新因果共同抽象状态 状态 = 新因果共同抽象状态::来源不足;
    std::optional<新概念因果信息> 信息;
    friend bool operator==(const 新因果共同抽象结果 &, const 新因果共同抽象结果 &) = default;
};

struct 新因果处理结果 final
{
    新因果操作状态 状态 = 新因果操作状态::入口拒绝;
    std::optional<稳定编码> 因果节点;
    std::optional<新因果信息> 因果信息;
};

struct 新因果查询结果 final
{
    新因果操作状态 状态 = 新因果操作状态::入口拒绝;
    std::vector<稳定编码> 因果节点组;
};

struct 新因果世界范围 final
{
    稳定编码 世界根节点;
    std::vector<稳定编码> 具体因果节点组, 概念因果节点组;
    friend bool operator==(const 新因果世界范围 &, const 新因果世界范围 &) = default;
};

struct 新因果方法参考 final
{
    稳定编码 因果节点;
    稳定编码 本能方法节点;
    新因果动作定义 动作;
    新因果统计 统计;
    friend bool operator==(const 新因果方法参考 &, const 新因果方法参考 &) = default;
};

// 动作事实和外部动作识别尚未进入新版数据类，因此以显式依赖端口接入。
// 未装配端口时，动作路径必须返回“依赖未配置”。
class 新因果动作证据提供者
{
  public:
    virtual ~新因果动作证据提供者() = default;
    virtual std::optional<新因果动作定义> 读取自我动作事实(
        稳定编码 动作事实节点) const noexcept = 0;
    virtual std::optional<新因果动作定义> 推断外部动作(稳定编码 动作动态节点) const noexcept = 0;
};

} // namespace 海中鱼巣
