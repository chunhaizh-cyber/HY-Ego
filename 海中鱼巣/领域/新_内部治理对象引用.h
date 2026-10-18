#pragma once

#include <cstdint>
#include <optional>
#include "../核心/基础数据集.h"
#include "新_状态类.h"

namespace 海中鱼巣 {

struct 新任务虚拟存在引用 final {
    稳定编码 任务节点;
    稳定编码 虚拟存在节点;
    稳定编码 状态特征节点;
    friend bool operator==(const 新任务虚拟存在引用&,
        const 新任务虚拟存在引用&) = default;
};

struct 新任务虚拟存在读回 final {
    新任务虚拟存在引用 引用;
    稳定编码 状态特征类型概念节点;
    std::int64_t 当前状态 = 0;
};

struct 新任务虚拟存在特征读回 final {
    稳定编码 特征节点, 特征类型概念节点, 当前特征概念节点;
    std::int64_t 当前值 = 0;
    friend bool operator==(const 新任务虚拟存在特征读回&, const 新任务虚拟存在特征读回&) = default;
};
struct 新任务虚拟存在特征组 final {
    新任务虚拟存在引用 引用;
    新任务虚拟存在特征读回 状态;
    std::optional<新任务虚拟存在特征读回> 服务值;
    friend bool operator==(const 新任务虚拟存在特征组&, const 新任务虚拟存在特征组&) = default;
};
struct 新任务虚拟存在特征引用 final {
    新任务虚拟存在引用 任务引用;
    稳定编码 特征节点;
    friend bool operator==(const 新任务虚拟存在特征引用&,
        const 新任务虚拟存在特征引用&) = default;
};
struct 新治理任务特征变化结果 final {
    新任务虚拟存在特征引用 目标;
    稳定编码 治理场景节点, 前状态节点, 后状态节点, 变化动态节点;
};

enum class 新任务贡献动作种类 : std::int64_t {
    来源计值 = 1, 直接接收 = 2, 权威迁移 = 3, 直属结算收尾 = 4, 最终结算收尾 = 5
};
struct 新任务结算边界证据 final {
    稳定编码 结算任务节点, 结算需求节点, 服务根需求节点, 登记方法节点;
    friend bool operator==(const 新任务结算边界证据&,
        const 新任务结算边界证据&) = default;
};
struct 新任务贡献动作定义 final {
    稳定编码 节点, 目标特征类型概念节点, 任务参数角色节点;
    新任务贡献动作种类 种类;
};
struct 新任务贡献动作阶段 final {
    稳定编码 节点;
    新任务贡献动作种类 种类;
    新任务虚拟存在特征引用 目标;
    稳定编码 动作定义节点, 执行主体节点, 现实来源场景节点;
    稳定编码 前状态节点, 来源任务节点;
    新状态强时间 动作时间;
    std::int64_t 原值 = 0, 新值 = 0;
    std::optional<稳定编码> 来源执行节点, 来源评估节点, 来源结算节点;
    std::vector<稳定编码> 直接子接收证据组;
    std::optional<std::int64_t> 原旧小计;
    std::optional<稳定编码> 发表结果节点;
    std::optional<稳定编码> 来源直接接收节点;
    std::optional<新任务结算边界证据> 结算边界;
    std::optional<稳定编码> 来源最终消费节点;
};
struct 新治理任务特征动作变化结果 final {
    新任务虚拟存在特征引用 目标;
    稳定编码 阶段节点, 治理场景节点, 动作节点;
    稳定编码 前状态节点, 后状态节点, 变化动态节点;
};

struct 新任务治理场景信息 final {
    稳定编码 根场景节点, 场景节点;
    新任务虚拟存在引用 任务引用;
    friend bool operator==(const 新任务治理场景信息&,
        const 新任务治理场景信息&) = default;
};
enum class 新任务治理场景发布工程状态 : std::uint8_t {
    已发布 = 1, 未发布已清理场景
};
struct 新任务治理场景发布结果 final {
    新任务治理场景发布工程状态 状态 = 新任务治理场景发布工程状态::未发布已清理场景;
    std::optional<新任务治理场景信息> 场景;
};

struct 新任务虚拟存在语义读回 final {
    新任务虚拟存在读回 状态;
    稳定编码 专属存在概念节点;
};

enum class 新治理参与者种类 : std::uint8_t {
    任务虚拟存在 = 1, 现实执行主体引用 = 2
};
struct 新治理场景参与者 final {
    稳定编码 场景节点, 参与节点, 专属存在概念节点;
    新治理参与者种类 种类 = 新治理参与者种类::任务虚拟存在;
    std::optional<新任务虚拟存在引用> 任务引用;
    std::optional<稳定编码> 现实来源场景节点;
    friend bool operator==(const 新治理场景参与者&, const 新治理场景参与者&) = default;
};
class 新治理参与者读取接口 {
public:
    virtual ~新治理参与者读取接口() = default;
    virtual std::optional<新治理场景参与者> 读取治理参与者(
        稳定编码 场景节点, 稳定编码 参与节点) const noexcept = 0;
};

struct 新治理任务动作请求 final {
    新任务虚拟存在引用 目标;
    稳定编码 执行主体节点, 现实来源场景节点;
    稳定编码 动作定义节点, 来源普通动作节点, 前状态节点;
    新状态强时间 动作时间;
};
struct 新治理任务变化结果 final {
    稳定编码 治理场景节点, 动作节点, 前状态节点, 后状态节点, 变化动态节点;
};
enum class 新治理动作登记状态 : std::uint8_t { 已建立 = 1, 精确重复, 入口拒绝 };
struct 新治理动作登记结果 final {
    新治理动作登记状态 状态 = 新治理动作登记状态::入口拒绝;
    std::optional<稳定编码> 动作节点;
};
class 新任务治理世界协作接口 {
public:
    virtual ~新任务治理世界协作接口() = default;
    virtual 新任务治理场景发布结果 发布新任务治理场景(
        const 新任务虚拟存在引用&, const 基础封存追加片段&) noexcept = 0;
    virtual void 迁移已有任务治理场景(
        const std::vector<新任务虚拟存在引用>&) noexcept = 0;
    virtual std::optional<新任务治理场景信息> 读取任务治理场景(
        const 新任务虚拟存在引用&) const noexcept = 0;
    virtual 新治理动作登记结果 建立任务状态动作(const 新治理任务动作请求&) noexcept = 0;
    virtual 新治理任务变化结果 完成任务状态变化(
        稳定编码 动作, 稳定编码 前状态, 稳定编码 后状态) noexcept = 0;
    virtual std::optional<新治理任务变化结果> 读取任务状态变化(
        const 新任务虚拟存在引用&, 稳定编码 来源普通动作) const noexcept = 0;
    virtual 新治理任务特征变化结果 完成任务特征变化(
        const 新任务虚拟存在特征引用&, 稳定编码 前状态, 稳定编码 后状态) noexcept = 0;
    virtual std::optional<新治理任务特征变化结果> 读取任务特征变化(
        const 新任务虚拟存在特征引用&, 稳定编码 前状态, 稳定编码 后状态) const noexcept = 0;
    virtual 新治理动作登记结果 建立任务特征动作(稳定编码 阶段节点) noexcept = 0;
    virtual 新治理任务特征动作变化结果 完成任务特征动作(
        稳定编码 阶段节点, 稳定编码 后状态节点) noexcept = 0;
    virtual std::optional<新治理任务特征动作变化结果> 读取任务特征动作(
        稳定编码 阶段节点) const noexcept = 0;
};

// 中立只读合同：目标结构由其唯一owner核验，动态/场景不依赖任务实现。
class 新内部治理对象读取接口 {
public:
    virtual ~新内部治理对象读取接口() = default;
    virtual std::optional<新任务虚拟存在读回> 读取任务虚拟存在引用(
        const 新任务虚拟存在引用&) const noexcept = 0;
    // 未提供语义投影的模拟读取器不能因此转而查询现实任务。
    virtual std::optional<新任务虚拟存在语义读回> 读取任务虚拟存在语义(
        const 新任务虚拟存在引用&) const noexcept { return std::nullopt; }
    // 模拟读取器未实现时仅返回无能力，不得回退到现实任务。
    virtual std::optional<新任务虚拟存在特征组> 读取任务虚拟存在特征组(
        const 新任务虚拟存在引用&) const noexcept { return std::nullopt; }
    virtual std::optional<稳定编码> 获取任务已结算动作定义() const noexcept { return std::nullopt; }
    virtual std::optional<新任务贡献动作定义> 读取任务贡献动作定义(
        稳定编码) const noexcept { return std::nullopt; }
    virtual std::optional<新任务贡献动作阶段> 读取任务贡献动作阶段(
        稳定编码) const noexcept { return std::nullopt; }
};

} // namespace 海中鱼巣
