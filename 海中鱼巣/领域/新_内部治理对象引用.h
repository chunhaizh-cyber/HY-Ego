#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <vector>
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
// TASK-STATE-CAUSAL 切片2（5260 §4.2第5项）：任务虚拟存在的因素特征。只在末尾追加，不重排。
enum class 新任务因素特征种类 : std::uint8_t {
    承接依据 = 1, 候选结论, 条件结论, 执行进入结论, 影响评估结论, 后继决议, 不合法子需求
};
inline constexpr std::size_t 新任务因素特征种类数 = 7;
// 各I64因素特征的取值（与5260 §4.2因素特征表逐项对应）；“影响评估结论”取影响评估的需求结果
// （新需求当前满足状态）原值；“不合法子需求”以U64数组材料保存子需求节点编码组（升序去重）。
enum class 新任务承接依据值 : std::int64_t {
    自我首次复核 = 1, 状态变化复核, 条件子需求, 因果子需求, 复合子需求, 外部服务要求, 行动请求, 扫描补齐
};
enum class 新任务候选结论值 : std::int64_t { 有候选 = 1, 无候选 };
enum class 新任务条件结论值 : std::int64_t { 具备 = 1, 等待条件, 需子需求 };
enum class 新任务执行进入结论值 : std::int64_t { 已进入动作 = 1, 零动作 };
enum class 新任务后继决议值 : std::int64_t { 继续查找 = 1, 等待条件, 完成, 失败, 取消 };
struct 新任务因素特征读回 final {
    新任务因素特征种类 种类 = 新任务因素特征种类::承接依据;
    稳定编码 特征节点, 特征类型概念节点;
    friend bool operator==(const 新任务因素特征读回&, const 新任务因素特征读回&) = default;
};
struct 新任务虚拟存在特征组 final {
    新任务虚拟存在引用 引用;
    新任务虚拟存在特征读回 状态;
    std::optional<新任务虚拟存在特征读回> 服务值;
    // 已登记的因素特征（旧任务补齐前可缺）；只引用任务自身材料，不为需求生成状态。
    std::vector<新任务因素特征读回> 因素组;
    const 新任务因素特征读回* 查找因素(稳定编码 F) const noexcept {
        for (const auto& 项 : 因素组) if (项.特征节点 == F) return &项;
        return nullptr;
    }
    // 状态、服务值或因素特征之一：任务治理场景状态的来源实例特征只能是这些。
    bool 含任务特征(稳定编码 F) const noexcept {
        return F == 状态.特征节点 || (服务值 && F == 服务值->特征节点) || 查找因素(F) != nullptr;
    }
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
enum class 新结算能力类别 : std::uint8_t { 自我服务值治理 = 2 };
struct 新结算能力引用 final {
    新结算能力类别 类别;
    稳定编码 节点;
    friend bool operator==(const 新结算能力引用&, const 新结算能力引用&) = default;
};
struct 新服务值治理定义 final {
    稳定编码 节点, 自我概念节点, 服务值特征类型节点;
    friend bool operator==(const 新服务值治理定义&, const 新服务值治理定义&) = default;
};
struct 新任务结算边界证据 final {
    稳定编码 结算任务节点, 结算需求节点, 服务根需求节点;
    新结算能力引用 能力;
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

// TASK-STATE-CAUSAL（5260 §4.2、4211 §4.1）：触发一次任务状态迁移的治理操作。只在末尾追加，不重排。
enum class 新任务状态迁移原因种类 : std::int64_t {
    前置材料父任务建立 = 1,   // 建立前置语言材料父任务：已建立→等待条件
    准备查找方法 = 2,         // 已建立→待查找方法
    自我承接 = 3,             // 承接需求任务（自我新鲜决议）
    扫描补齐 = 4,             // 建立需求任务关联：已建立→等待条件
    承接行动请求 = 5,
    承接外部服务要求 = 6,
    承接条件子需求 = 7,
    承接因果子需求 = 8,
    承接来源前端子需求 = 9,
    等待复核放行 = 10,        // 准备等待任务重新筹办
    学习后重新筹办 = 11,
    消费方法候选 = 12,
    确认进入待执行 = 13,
    开始执行 = 14,
    撤销未进入动作执行 = 15,
    零动作恢复 = 16,
    执行结束 = 17,
    影响评估完成 = 18,
    自我后继决议 = 19,
    执行前目标收束 = 20,
    重新启用 = 21,
    放弃结算 = 22,
    直属结算收尾 = 23,
    最终结算收尾 = 24,
    复合子需求提交 = 25,
    子需求不合法通知 = 26,
    子需求不合法重筹办无可用候选 = 27,
    子需求不合法重新筹办 = 28,
    子需求恢复合法 = 29
};
// 修订后迁移表中一对“前状态→后状态”的迁移动作定义；挂在任务集合根下，由动作方法投影形成动作概念。
struct 新任务状态迁移定义 final {
    稳定编码 节点, 目标特征类型概念节点, 任务参数角色节点;
    std::int64_t 前状态值 = 0, 后状态值 = 0;
};
// 治理迁移记录（4211 §4.1）：治理动作的唯一来源，持久结构。原因材料引用以编码值保存，
// 只供追溯，不阻止所引用材料（候选材料、撤销的空执行等）按各自生命周期物理删除。
struct 新任务状态迁移记录 final {
    稳定编码 节点;
    新任务虚拟存在引用 任务引用;
    std::int64_t 前状态值 = 0, 后状态值 = 0;
    稳定编码 前状态节点, 动作定义节点, 执行主体节点, 现实来源场景节点;
    新状态强时间 动作时间;
    新任务状态迁移原因种类 原因种类 = 新任务状态迁移原因种类::前置材料父任务建立;
    std::vector<稳定编码> 原因材料引用组;
};
struct 新治理任务状态迁移结果 final {
    稳定编码 记录节点, 治理场景节点, 动作节点;
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

struct 新方法虚拟存在引用 final {
    稳定编码 方法节点, 虚拟存在节点, 启用特征节点;
    friend bool operator==(const 新方法虚拟存在引用&,
        const 新方法虚拟存在引用&) = default;
};
struct 新方法虚拟存在读回 final {
    新方法虚拟存在引用 引用;
    稳定编码 专属存在概念节点;
    稳定编码 启用特征类型概念节点, 当前特征概念节点;
    std::int64_t 当前启用 = 0;
};
struct 新方法治理场景信息 final {
    稳定编码 根场景节点, 场景节点;
    新方法虚拟存在引用 方法引用;
    // 已有C(M)所属启用F字段关系的只读投影，不增加事实。
    稳定编码 启用特征归属字段关系;
    friend bool operator==(const 新方法治理场景信息&,
        const 新方法治理场景信息&) = default;
};
struct 新治理方法特征变化结果 final {
    新方法虚拟存在引用 目标;
    稳定编码 治理场景节点, 前状态节点, 后状态节点, 变化动态节点;
};
enum class 新方法治理场景发布工程状态 : std::uint8_t {
    已发布 = 1, 未发布已清理场景
};
struct 新方法治理场景发布结果 final {
    新方法治理场景发布工程状态 状态 = 新方法治理场景发布工程状态::未发布已清理场景;
    std::vector<新方法治理场景信息> 场景组;
};
class 新方法治理对象读取接口 {
public:
    virtual ~新方法治理对象读取接口() = default;
    virtual std::optional<新方法虚拟存在读回> 读取方法虚拟存在引用(
        const 新方法虚拟存在引用&) const noexcept = 0;
};
class 新特征I64更新准备;
using 新方法治理片段工厂 = std::function<基础封存追加片段()>;
class 新方法治理世界协作接口 {
public:
    virtual ~新方法治理世界协作接口() = default;
    virtual 新方法治理场景发布结果 发布新方法治理场景组(
        const std::vector<新方法虚拟存在引用>&, const 基础封存追加片段&,
        新特征I64更新准备* 联动特征准备 = nullptr) noexcept = 0;
    virtual 新方法治理场景发布结果 发布新方法治理场景组(
        const std::vector<新方法虚拟存在引用>&, const 新方法治理片段工厂&,
        新特征I64更新准备* 联动特征准备 = nullptr) noexcept = 0;
    virtual void 迁移已有方法治理场景(
        const std::vector<新方法虚拟存在引用>&) noexcept = 0;
    virtual std::optional<新方法治理场景信息> 读取方法治理场景(
        const 新方法虚拟存在引用&) const noexcept = 0;
    virtual 新治理方法特征变化结果 完成方法特征变化(
        const 新方法虚拟存在引用&, 稳定编码 前状态, 稳定编码 后状态) noexcept = 0;
    virtual std::optional<新治理方法特征变化结果> 读取方法特征变化(
        const 新方法虚拟存在引用&, 稳定编码 前状态, 稳定编码 后状态) const noexcept = 0;
    virtual void 回收未消费方法治理场景组(
        const std::vector<新方法虚拟存在引用>&) noexcept = 0;
};

enum class 新治理参与者种类 : std::uint8_t {
    任务虚拟存在 = 1, 现实执行主体引用 = 2, 方法虚拟存在 = 3
};
struct 新治理场景参与者 final {
    稳定编码 场景节点, 参与节点, 专属存在概念节点;
    新治理参与者种类 种类 = 新治理参与者种类::任务虚拟存在;
    std::optional<新任务虚拟存在引用> 任务引用;
    std::optional<稳定编码> 现实来源场景节点;
    std::optional<新方法虚拟存在引用> 方法引用;
    friend bool operator==(const 新治理场景参与者&, const 新治理场景参与者&) = default;
};
class 新治理参与者读取接口 {
public:
    virtual ~新治理参与者读取接口() = default;
    virtual std::optional<新治理场景参与者> 读取治理参与者(
        稳定编码 场景节点, 稳定编码 参与节点) const noexcept = 0;
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
    // TASK-STATE-CAUSAL：以已发布的治理迁移记录为来源建立治理动作（前状态先挂入任务治理场景）。
    virtual 新治理动作登记结果 建立任务状态迁移动作(稳定编码 记录节点) noexcept = 0;
    // 任务状态特征已提交后：挂后状态、建原子动态、关联“导致变化”，并同步交付实际发生。
    virtual 新治理任务状态迁移结果 完成任务状态迁移(
        稳定编码 记录节点, 稳定编码 后状态节点) noexcept = 0;
    virtual std::optional<新治理任务状态迁移结果> 读取任务状态迁移(
        稳定编码 记录节点) const noexcept = 0;
    // TASK-STATE-CAUSAL 切片2：把任务owner为因素特征新值建立的单个状态挂入该任务治理场景。
    // 只认可本任务因素特征、值类型合法的状态；挂接失败为工程故障。
    virtual void 登记任务因素状态(const 新任务虚拟存在引用&, 稳定编码 状态节点) noexcept = 0;
};

// 中立只读合同：目标结构由其唯一owner核验，动态/场景不依赖任务实现。
class 新内部治理对象读取接口 {
public:
    virtual ~新内部治理对象读取接口() = default;
    virtual std::optional<新任务虚拟存在引用> 查找任务虚拟存在引用(
        稳定编码) const noexcept { return std::nullopt; }
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
    virtual std::optional<新任务状态迁移定义> 读取任务状态迁移定义(
        稳定编码) const noexcept { return std::nullopt; }
    virtual std::optional<新任务状态迁移记录> 读取任务状态迁移记录(
        稳定编码) const noexcept { return std::nullopt; }
};

} // namespace 海中鱼巣
