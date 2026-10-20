#pragma once
#include <memory>

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "新_存在类.h"
#include "新_特征类.h"
#include "概念_二次特征类.h"

namespace 海中鱼巣 {

class 新_自我线程类;

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

enum class 新需求执行域 : std::int64_t { 自我执行 = 1, 被认知 = 2 };
struct 新需求认知来源 final {
    稳定编码 理解节点;
    稳定编码 内容确认节点;
    std::optional<稳定编码> 来源要求节点;
    friend bool operator==(const 新需求认知来源&, const 新需求认知来源&) = default;
};

enum class 新需求来源类型 : std::int64_t {
    本能定义 = 1,
    任务子目标 = 2,
    外部服务要求 = 3
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

enum class 新服务要求确认状态 : std::uint8_t {
    已读取 = 1, 当前有效, 需重新确认, 确认不存在,
    未装配, 结构不一致, 资源失败
};
struct 新服务要求确认目标 final {
    稳定编码 确认节点, 理解节点, 来源人类节点, 目标存在节点;
    新需求目标定义 目标;
    std::optional<稳定编码> 来源要求节点;
    friend bool operator==(const 新服务要求确认目标&,
        const 新服务要求确认目标&) = default;
};

struct 新部分服务要求目标 final {
    std::optional<稳定编码> 目标存在节点, 目标特征类型概念节点;
    std::optional<新需求目标形式> 目标形式;
    std::optional<稳定编码> 目标值域概念节点, 目标比较关系概念节点;
    std::optional<新需求变化方向> 目标变化方向;
    friend bool operator==(const 新部分服务要求目标&, const 新部分服务要求目标&) = default;
};
enum class 新服务要求缺项 : std::uint8_t {
    目标存在=1, 目标特征类型, 目标形式, 目标值域, 目标比较关系, 目标变化方向, 明确确认
};
struct 新部分服务要求材料 final {
    稳定编码 要求节点, 来源人类节点;
    std::string 公开会话标识, 来源要求标识;
    新部分服务要求目标 已知目标;
    std::optional<稳定编码> 理解节点, 确认节点;
    friend bool operator==(const 新部分服务要求材料&, const 新部分服务要求材料&) = default;
};
// 仅定位已发布材料；接收方必须重新读取，消息本身不构成需求确认。
struct 新完整服务需求材料包 final {
    稳定编码 要求节点, 理解节点, 确认节点;
    friend bool operator==(const 新完整服务需求材料包&,
        const 新完整服务需求材料包&) = default;
};
enum class 新部分服务要求读取状态 : std::uint8_t { 已读取=1, 材料不存在, 工程故障 };
struct 新部分服务要求读取结果 final {
    新部分服务要求读取状态 状态 = 新部分服务要求读取状态::工程故障;
    std::optional<新部分服务要求材料> 材料;
    std::vector<基础联合节点快照> 发布预期节点组;
};
class 新部分服务要求读取接口 {
public:
    virtual ~新部分服务要求读取接口() = default;
    virtual 新部分服务要求读取结果 读取部分服务要求(稳定编码 要求节点) const noexcept = 0;
};

// 仅计算本次材料缺项，不产生完整度特征或业务事实。
std::vector<新服务要求缺项> 查询服务要求缺项(const 新部分服务要求材料&);
struct 新服务要求确认读取结果 final {
    新服务要求确认状态 状态 = 新服务要求确认状态::未装配;
    std::optional<新服务要求确认目标> 确认;
};
struct 新服务要求确认同义结果 final {
    新服务要求确认状态 状态 = 新服务要求确认状态::未装配;
    std::optional<bool> 同义;
};
enum class 新服务要求确认保护状态 : std::uint8_t {
    已准备 = 1, 确认不存在, 需重新确认, 前提已变化, 未装配, 结构不一致, 资源失败
};
class 新服务要求确认语义保护 final {
public:
    新服务要求确认语义保护(新服务要求确认语义保护&&) noexcept;
    新服务要求确认语义保护& operator=(新服务要求确认语义保护&&) noexcept;
    ~新服务要求确认语义保护();
    新服务要求确认语义保护(const 新服务要求确认语义保护&) = delete;
    新服务要求确认语义保护& operator=(const 新服务要求确认语义保护&) = delete;
    const 新服务要求确认目标& 确认() const noexcept;
    const std::vector<基础联合节点快照>& 预期节点组() const noexcept;
private:
    struct 实现;
    explicit 新服务要求确认语义保护(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 结构化服务要求服务;
};
struct 新服务要求确认保护结果 final {
    新服务要求确认保护状态 状态 = 新服务要求确认保护状态::未装配;
    std::optional<新服务要求确认语义保护> 保护;
};
class 新服务要求确认读取接口 {
public:
    virtual ~新服务要求确认读取接口() = default;
    virtual 新服务要求确认保护结果 准备服务要求确认语义保护(稳定编码) const noexcept = 0;
    virtual 新服务要求确认读取结果 读取服务要求确认身份(稳定编码) const noexcept = 0;
    virtual 新服务要求确认读取结果 复核服务要求确认语义(稳定编码) const noexcept = 0;
    virtual 新服务要求确认同义结果 比较服务要求确认固定语义(
        稳定编码, 稳定编码) const noexcept = 0;
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
    // 本能定义来源不携带承接节点；任务子目标来源必须携带父任务节点，
    // 并且只能经self确认后的需求owner受限入口建立。
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
    std::vector<稳定编码> 服务要求确认组;
    // 仅新服务要求D出生时记录，不给既有/同义复用D补写取得来源。
    std::optional<稳定编码> 服务需求取得执行节点;
    稳定编码 需求主体存在节点;
    新需求执行域 执行域{};
    std::optional<新需求认知来源> 认知来源;
    friend bool operator==(const 新需求信息&, const 新需求信息&) = default;
};

struct 新需求完整目标 final {
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    friend bool operator==(const 新需求完整目标&, const 新需求完整目标&) = default;
};
struct 新需求治理信息 final {
    稳定编码 节点;
    std::optional<新需求根角色> 根角色;
    std::optional<稳定编码> 父需求节点;
    std::optional<新需求来源类型> 来源类型;
    std::optional<稳定编码> 来源承接节点;
    std::int64_t 权重 = 0;
    新需求活动状态 活动状态 = 新需求活动状态::活动;
    std::vector<稳定编码> 服务要求确认组;
    std::optional<稳定编码> 服务需求取得执行节点;
    std::optional<新需求完整目标> 完整目标;
    std::optional<稳定编码> 外部要求节点;
    std::optional<新部分服务要求材料> 外部要求材料;
    稳定编码 需求主体存在节点;
    新需求执行域 执行域{};
    std::optional<新需求认知来源> 认知来源;
    friend bool operator==(const 新需求治理信息&, const 新需求治理信息&) = default;
};
enum class 新外部服务需求采用状态 : std::uint8_t {
    已建立=1, 已复用, 材料待完善, 需重新确认, 来源不符, 要求不存在, 工程故障
};
struct 新外部服务需求采用结果 final {
    新外部服务需求采用状态 状态 = 新外部服务需求采用状态::工程故障;
    std::optional<稳定编码> 需求节点, 需求列表项节点;
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
    权重不合法 = 21,
    执行域不适用, 认知来源不合法, 同来源目标冲突, 需重新确认, 前提已变化
};

struct 新被认知需求建立请求 final {
    稳定编码 需求主体存在节点;
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    新需求认知来源 认知来源;
};
struct 新需求记录全集结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::vector<新需求信息> 需求组;
};

struct 新需求建立结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<稳定编码> 需求节点;
    std::optional<稳定编码> 需求列表项节点;
};

struct 新需求集合发布关联 final {
    稳定编码 需求节点, 主体存在节点, 场景节点, 动作节点;
    friend bool operator==(const 新需求集合发布关联&, const 新需求集合发布关联&) = default;
};
struct 新需求集合发布关联结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<新需求集合发布关联> 关联;
};
class 新需求集合发布准备 final {
public:
    新需求集合发布准备(新需求集合发布准备&&) noexcept;
    新需求集合发布准备& operator=(新需求集合发布准备&&) noexcept;
    ~新需求集合发布准备() noexcept;
    新需求集合发布准备(const 新需求集合发布准备&) = delete;
    新需求集合发布准备& operator=(const 新需求集合发布准备&) = delete;
    const 基础封存追加片段& 片段() const noexcept;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
    explicit 新需求集合发布准备(std::unique_ptr<实现>) noexcept;
    friend class 新_需求类;
};
struct 新需求集合发布准备结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<新需求集合发布准备> 准备;
};

enum class 新服务要求采用关系状态 : std::uint8_t {
    已读取 = 1, 未装配, 确认不存在, 结构不一致, 资源失败
};
struct 新服务要求采用关系结果 final {
    新服务要求采用关系状态 状态 = 新服务要求采用关系状态::未装配;
    std::vector<稳定编码> 需求节点组;
};
enum class 新服务要求执行守卫状态 : std::uint8_t {
    不涉及 = 1, 当前有效, 需重新确认, 未装配,
    需求不存在, 结构不一致, 资源失败, 需求信息待完善
};
struct 新服务要求执行守卫结果 final {
    新服务要求执行守卫状态 状态 = 新服务要求执行守卫状态::未装配;
    std::optional<稳定编码> 阻塞需求节点;
    std::vector<稳定编码> 确认节点组;
};
enum class 新服务要求需求建立状态 : std::uint8_t {
    已建立 = 1, 已复用, 未装配, 来源不符, 确认不合法,
    需重新确认, 既有需求来源冲突, 结构不一致, 资源失败
};
struct 新服务要求需求建立结果 final {
    新服务要求需求建立状态 状态 = 新服务要求需求建立状态::未装配;
    std::optional<新需求建立结果> 建立结果;
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

// 需求owner对独立需求树的完整、可判错读取。调用方不得用空vector
// 区分“当前没有普通子需求”和“结构或资源读取失败”。
struct 新需求全集结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::vector<稳定编码> 需求节点组;
};

struct 新需求列表全集结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::vector<新需求列表项信息> 列表项组;
    std::vector<新需求治理信息> 需求组;
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
    需要执行前后比较 = 13,
    读取接口未装配 = 14,
    需求信息待完善 = 15
};

struct 新需求当前满足结果 final {
    新需求当前满足状态 状态 = 新需求当前满足状态::入口拒绝;
    std::optional<稳定编码> 实际特征节点;
    // I64目标返回“目标边界 - 当前值”；正数表示需增加，负数表示需减少。
    std::optional<std::int64_t> 准确差异;
};

// ARCH-L4 需求组织服务。它只在同一个基础数据集中建立独立需求树，
// 世界树、特征概念树和二次特征概念只作为只读引用来源。
class 新_需求类 final : public 特征需求成员读取接口 {
public:
    新_需求类(
        新_特征值类& 特征值服务,
        新_存在类& 存在服务,
        新_特征类& 特征服务,
        概念_特征类& 特征概念服务,
        概念_二次特征类& 二次特征概念服务) noexcept;

    // 只建立需求树自身的根、类型节点和字段节点。
    bool 初始化() noexcept;

    // 通知只表示关联结构可能变化；在owner锁内调用，不得反入owner或读写领域。
    bool 绑定关联检查通知(const void* 所有者, std::function<void()> 通知) noexcept;
    void 解除关联检查通知(const void* 所有者) noexcept;

    新需求双根初始化结果 建立或取得本能双根(
        const 新需求双根初始化请求& 请求) noexcept;

    // 固定定义代码只能建立本能定义子需求；任务子目标必须经self确认并
    // 核验父需求与唯一来源任务后进入受限入口，任意调用者不能伪造来源。
    新需求建立结果 建立或取得普通需求(
        const 新普通需求建立请求& 请求) noexcept;

    std::optional<新需求信息> 获取需求(
        稳定编码 需求节点) const noexcept;
    std::optional<新需求治理信息> 读取需求治理信息(稳定编码) const noexcept;
    void 装配部分服务要求读取接口(const 新部分服务要求读取接口*) noexcept;
    std::optional<新需求列表项信息> 获取需求列表项(
        稳定编码 列表项节点) const noexcept;
    std::optional<稳定编码> 查询根需求(
        新需求根角色 角色) const noexcept;
    新需求全集结果 查询全部需求() const noexcept;
    // 全部需求/列表及目标查询仅覆盖自我执行域；记录查询覆盖两域。
    新需求建立结果 建立或取得被认知需求(const 新被认知需求建立请求&) noexcept;
    特征需求成员读取结果 读取需求集合成员身份(稳定编码) const noexcept override;
    新需求集合发布准备结果 准备主体需求集合发布(
        稳定编码 需求节点, 稳定编码 主体存在, 稳定编码 特征节点,
        稳定编码 场景节点, 稳定编码 动作节点, const 基础联合节点快照& 特征保护) noexcept;
    新需求集合发布关联结果 读取主体需求集合发布关联(稳定编码 需求节点) const noexcept;
    void 完成主体需求集合发布(
        const 新需求集合发布准备&, const 基础片段发布回执&) noexcept;
    新需求记录全集结果 查询全部需求记录() const noexcept;
    新需求记录全集结果 查询主体需求(稳定编码 主体存在, 新需求执行域 域) const noexcept;
    新需求记录全集结果 查询被认知需求来源(稳定编码 理解节点) const noexcept;
    // 完整核验列表登记、原始成员及需求树覆盖；非成功不返回部分材料。
    新需求列表全集结果 查询全部需求列表() const noexcept;

    std::vector<稳定编码> 查询直接子需求(
        稳定编码 需求节点) const noexcept;
    std::vector<稳定编码> 查询目标存在需求(
        稳定编码 目标存在节点) const noexcept;
    新需求精确查询结果 按完整目标查询(
        稳定编码 目标存在节点,
        const 新需求目标定义& 目标) const noexcept;

    新需求当前满足结果 复核当前满足(
        稳定编码 需求节点) const noexcept;
    // 仅在线程启动前装配，全部线程join后解绑；接口所有者寿命覆盖使用期。
    void 装配服务要求确认读取接口(const 新服务要求确认读取接口*) noexcept;
    // 概念比较回调可调用：只读L1和冻结schema，不取得需求/业务锁。
    新服务要求采用关系结果 查询服务要求采用关系(稳定编码 确认节点) const noexcept;
    新服务要求采用关系结果 查询服务需求取得来源(稳定编码 取得执行节点) const noexcept;
    新服务要求确认读取结果 读取服务要求确认身份(稳定编码 确认节点) const noexcept;
    新服务要求确认读取结果 复核服务要求确认语义(稳定编码 确认节点) const noexcept;
    新服务要求执行守卫结果 复核需求服务要求语义(稳定编码 需求节点) const noexcept;
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
    // 初始化后schema冻结；仅用一次L1结构快照核验身份和字段形状，不取得需求锁。
    特征概念规则状态 校验集合成员需求身份(稳定编码 需求节点) const noexcept;

private:
    新需求操作状态 迁移既有自我需求归属(稳定编码 唯一自我) noexcept;
    friend class 新_自我线程类;
    新外部服务需求采用结果 建立或取得外部服务需求已核验(
        const 新完整服务需求材料包&) noexcept;
    // 只供self在核验父需求与来源任务一一对应后采用任务条件子目标。
    新需求建立结果 建立或取得普通需求已核验(
        const 新普通需求建立请求& 请求,
        bool 允许任务子目标) noexcept;
    新需求操作状态 配置服务要求采用来源已核验(
        稳定编码 结算需求, 稳定编码 结算任务,
        const 特征任务成员读取接口&) noexcept;
    新服务要求需求建立结果 建立或取得服务要求需求已核验(
        稳定编码 确认节点) noexcept;
    // self已独立核验X真实返回、动作和目标；需求owner不反向引用任务类。
    新服务要求需求建立结果 建立或取得服务要求需求已核验(
        稳定编码 确认节点, 稳定编码 取得执行节点) noexcept;
    新服务要求需求建立结果 建立或取得服务要求需求内部(
        稳定编码 确认节点, std::optional<稳定编码> 取得执行节点) noexcept;
    新需求建立结果 建立或取得普通需求内部(
        const 新普通需求建立请求&, bool 允许任务子目标,
        std::optional<稳定编码> 初始服务要求确认,
        std::optional<稳定编码> 初始服务需求取得执行节点 = std::nullopt) noexcept;

    新_特征值类& 特征值服务_;
    新_存在类& 存在服务_;
    新_特征类& 特征服务_;
    概念_特征类& 特征概念服务_;
    概念_二次特征类& 二次特征概念服务_;
    const void* 关联检查通知所有者_ = nullptr;
    std::function<void()> 关联检查通知_;
    const 新服务要求确认读取接口* 服务要求确认读取接口_ = nullptr;
    const 新部分服务要求读取接口* 部分服务要求读取接口_ = nullptr;
    const 特征任务成员读取接口* 服务要求任务读取接口_ = nullptr;
    稳定编码 服务要求结算需求_;
    稳定编码 服务要求结算任务_;
};

} // namespace 海中鱼巣
