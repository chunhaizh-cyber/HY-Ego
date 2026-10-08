#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_存在类.h"
#include "新_特征类.h"
#include "概念_二次特征类.h"

namespace 海中鱼巣 {

// 三个身份都位于全局基础数据集中。需求树根不属于世界树；安全根和
// 服务根仍是独立需求节点，但它们的目标存在都是唯一自我。
extern 稳定编码 需求树根节点;
extern 稳定编码 安全根需求节点;
extern 稳定编码 服务根需求节点;

enum class 新需求根角色 : std::int64_t {
    安全 = 1,
    服务 = 2
};

enum class 新需求活动状态 : std::int64_t {
    活动 = 1,
    等待 = 2,
    已满足 = 3,
    失活 = 4
};

enum class 新需求来源类型 : std::int64_t {
    本能定义 = 1,
    任务子目标 = 2
};

enum class 新需求目标形式 : std::int64_t {
    值 = 1,
    变化方向 = 2
};

enum class 新需求变化方向 : std::int64_t {
    增加 = 1,
    减少 = 2
};

struct 新需求目标定义 final {
    稳定编码 目标特征类型概念节点;
    新需求目标形式 目标形式 = 新需求目标形式::值;
    std::optional<稳定编码> 目标值域概念节点;
    std::optional<稳定编码> 目标比较关系概念节点;
    std::optional<新需求变化方向> 目标变化方向;
    friend bool operator==(const 新需求目标定义&,
        const 新需求目标定义&) = default;
};

struct 新根需求定义 final {
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    // 根权重由具名运行配置明确提供；普通子需求不能自行指定权重。
    std::int64_t 初始权重;
    friend bool operator==(const 新根需求定义&,
        const 新根需求定义&) = default;
};

struct 新需求双根初始化请求 final {
    新根需求定义 安全根;
    新根需求定义 服务根;
};

struct 新普通需求建立请求 final {
    稳定编码 父需求节点;
    // 场景本身也具有存在身份，因此目标存在统一使用世界存在身份。
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    // 本能定义来源不携带承接节点；任务子目标来源必须携带由task owner
    // 发布的承接节点。当前只开放本能定义来源的正式建立路径。
    新需求来源类型 来源类型 = 新需求来源类型::任务子目标;
    std::optional<稳定编码> 来源承接节点;
};

struct 新需求信息 final {
    稳定编码 节点;
    std::optional<新需求根角色> 根角色;
    std::optional<稳定编码> 父需求节点;
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    std::optional<新需求来源类型> 来源类型;
    std::optional<稳定编码> 来源承接节点;
    std::int64_t 权重 = 0;
    新需求活动状态 活动状态 = 新需求活动状态::活动;
    friend bool operator==(const 新需求信息&, const 新需求信息&) = default;
};

struct 新需求列表项信息 final {
    稳定编码 节点;
    std::optional<新需求根角色> 根角色;
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    std::vector<稳定编码> 需求节点组;
    friend bool operator==(const 新需求列表项信息&,
        const 新需求列表项信息&) = default;
};

enum class 新需求操作状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    已找到 = 3,
    已修改 = 4,
    无变化 = 5,
    入口拒绝 = 6,
    尚未初始化 = 7,
    需求不存在 = 8,
    父需求不合法 = 9,
    目标存在不合法 = 10,
    目标形式不合法 = 11,
    目标特征类型不合法 = 12,
    目标值域不合法 = 13,
    比较关系不合法 = 14,
    目标方向不合法 = 15,
    双根定义冲突 = 16,
    任务来源未接通 = 17,
    会形成环 = 18,
    结构不一致 = 19,
    资源失败 = 20,
    权重不合法 = 21
};

struct 新需求建立结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<稳定编码> 需求节点;
    std::optional<稳定编码> 需求列表项节点;
};

struct 新需求双根初始化结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<稳定编码> 安全根;
    std::optional<稳定编码> 服务根;
    std::optional<稳定编码> 安全根列表项;
    std::optional<稳定编码> 服务根列表项;
};

struct 新需求精确查询结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<稳定编码> 需求列表项节点;
    std::vector<稳定编码> 需求节点组;
};

enum class 新需求当前满足状态 : std::uint8_t {
    已满足 = 1,
    需增加 = 2,
    需减少 = 3,
    未满足但无有向差值 = 4,
    目标特征尚未形成 = 5,
    当前值不存在 = 6,
    比较关系不适用 = 7,
    双根负差非法 = 8,
    入口拒绝 = 9,
    需求不存在 = 10,
    结构不一致 = 11,
    资源失败 = 12,
    // 方向目标不能由一个current-only当前值判定；任务执行实例必须比较
    // 动作前值A与结果观察值B。
    需要执行前后比较 = 13
};

struct 新需求当前满足结果 final {
    新需求当前满足状态 状态 = 新需求当前满足状态::入口拒绝;
    std::optional<稳定编码> 实际特征节点;
    // I64目标返回“目标边界 - 当前值”；正数表示需增加，负数表示需减少。
    std::optional<std::int64_t> 准确差异;
};

// ARCH-L4 需求组织服务。它只在同一个基础数据集中建立独立需求树，
// 世界树、特征概念树和二次特征概念只作为只读引用来源。
class 新_需求类 final {
public:
    新_需求类(
        新_特征值类& 特征值服务,
        新_存在类& 存在服务,
        新_特征类& 特征服务,
        概念_特征类& 特征概念服务,
        概念_二次特征类& 二次特征概念服务) noexcept;

    // 只建立需求树自身的根、类型节点和字段节点。
    bool 初始化() noexcept;

    新需求双根初始化结果 建立或取得本能双根(
        const 新需求双根初始化请求& 请求) noexcept;

    // 固定定义代码可以建立本能定义子需求；任务子目标仍须等待task owner
    // 提供正式强类型承接读取能力，不能用任意节点代替。
    新需求建立结果 建立或取得普通需求(
        const 新普通需求建立请求& 请求) noexcept;

    std::optional<新需求信息> 获取需求(
        稳定编码 需求节点) const noexcept;
    std::optional<新需求列表项信息> 获取需求列表项(
        稳定编码 列表项节点) const noexcept;
    std::optional<稳定编码> 查询根需求(
        新需求根角色 角色) const noexcept;

    std::vector<稳定编码> 查询直接子需求(
        稳定编码 需求节点) const noexcept;
    std::vector<稳定编码> 查询目标存在需求(
        稳定编码 目标存在节点) const noexcept;
    新需求精确查询结果 按完整目标查询(
        稳定编码 目标存在节点,
        const 新需求目标定义& 目标) const noexcept;

    新需求当前满足结果 复核当前满足(
        稳定编码 需求节点) const noexcept;
    std::optional<std::int64_t> 查询需求权重(
        稳定编码 需求节点) const noexcept;
    // 只有双根权重可以由配置修改；普通子需求权重始终由父需求平分。
    新需求操作状态 修改根需求权重(
        稳定编码 根需求节点,
        std::int64_t 新权重) noexcept;
    新需求操作状态 修改活动状态(
        稳定编码 需求节点,
        新需求活动状态 新状态) noexcept;
    bool 是需求节点(稳定编码 节点) const noexcept;

private:
    新_特征值类& 特征值服务_;
    新_存在类& 存在服务_;
    新_特征类& 特征服务_;
    概念_特征类& 特征概念服务_;
    概念_二次特征类& 二次特征概念服务_;
};

} // namespace 海中鱼巣
