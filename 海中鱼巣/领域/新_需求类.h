#pragma once
#include <memory>

#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <stop_token>
#include <string>
#include <vector>
#include <variant>

#include "新_存在类.h"
#include "信息内容.数据.h"
#include "新_特征类.h"
#include "概念_二次特征类.h"
#include "接收认知来源.数据.h"

namespace 海中鱼巣 {

class 新_自我线程类;
class 新_任务类;
class 新_世界树类;
class 概念_存在类;
class 新_语言类;

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
    外部服务要求 = 3,
    行动请求 = 4
};

enum class 新需求目标形式 : std::int64_t {
    值 = 1,
    变化方向 = 2
};

enum class 新需求变化方向 : std::int64_t {
    增加 = 1,
    减少 = 2
};

struct 新需求引用内容定位 final {
    稳定编码 引用需求节点;
    std::vector<稳定编码> 正式内容路径;
    friend bool operator==(const 新需求引用内容定位&,const 新需求引用内容定位&)=default;
};
struct 新需求新增成员见证要求 final {
    稳定编码 源特征类型概念节点;
    std::optional<稳定编码> 指定来源主体, 指定被认知需求;
    friend bool operator==(const 新需求新增成员见证要求&, const 新需求新增成员见证要求&) = default;
};
struct 新需求新增成员见证约束 final {
    新需求新增成员见证要求 要求;
    稳定编码 基准材料节点;
    friend bool operator==(const 新需求新增成员见证约束&, const 新需求新增成员见证约束&) = default;
};
struct 新需求目标定义 final {
    稳定编码 目标特征类型概念节点;
    新需求目标形式 目标形式 = 新需求目标形式::值;
    std::optional<稳定编码> 目标值域概念节点;
    std::optional<稳定编码> 目标比较关系概念节点;
    std::optional<新需求变化方向> 目标变化方向;
    std::optional<新需求引用内容定位> 引用内容定位;
    std::optional<新需求新增成员见证约束> 新增成员来源见证;
    friend bool operator==(const 新需求目标定义&,
        const 新需求目标定义&) = default;
};
enum class 新需求目标定义核验状态 : std::uint8_t {
    已完成=1, 输入不合法, 前提已变化, 资源失败, 内部不一致, 依赖未就绪, 未支持
};
struct 新需求目标定义核验结果 final {
    新需求目标定义核验状态 状态=新需求目标定义核验状态::输入不合法;
    std::vector<基础联合节点快照> 完整保护;
};
新需求目标定义核验结果 核验需求目标定义保护(
    const 新需求目标定义&,const 概念_特征类&,const 概念_二次特征类&);

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

struct 新需求复合分解来源 final {
    稳定编码 父任务节点, 父需求节点, 父目标存在节点;
    新需求目标定义 父目标;
    稳定编码 结构概念节点;
    std::optional<稳定编码> 择一约束节点, 所选分支概念;
    std::vector<稳定编码> 子目标内容路径;
    稳定编码 子目标要求概念;
    friend bool operator==(const 新需求复合分解来源&,const 新需求复合分解来源&)=default;
};
// CONCEPT-INFO-MEMBERSHIP §4.4：父目标不带引用内容定位的复合来源是“存在归属”分解——父目标为“值属于概念C”，
// 结构概念为C，子目标为当前值所指存在（或递归引用存在）Y 的“FT 属于 K”；子目标内容路径为引用路径加FT。
inline bool 是存在归属分解(const 新需求复合分解来源& s) noexcept { return !s.父目标.引用内容定位.has_value(); }
struct 新普通需求建立请求 final {
    稳定编码 父需求节点;
    // 场景本身也具有存在身份，因此目标存在统一使用世界存在身份。
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    // 本能定义来源不携带承接节点；任务子目标来源必须携带父任务节点，
    // 并且只能经self确认后的需求owner受限入口建立。
    新需求来源类型 来源类型 = 新需求来源类型::任务子目标;
    std::optional<稳定编码> 来源承接节点;
    std::optional<新需求复合分解来源> 复合来源;
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
    std::optional<新需求复合分解来源> 复合来源;
    std::optional<新接收认知来源> 首次接收认知来源;
    std::optional<新行动请求承接来源> 行动请求来源{};
    friend bool operator==(const 新需求信息&, const 新需求信息&) = default;
};

struct 新需求完整目标 final {
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    friend bool operator==(const 新需求完整目标&, const 新需求完整目标&) = default;
};

// 由任务owner冻结的正式取得槽；记录固定同一D，字段子目标复用它。
struct 新服务取得目标实例键 final {
    稳定编码 父取得任务,目标槽类型,服务对象;
    std::vector<稳定编码> 正式根路径;
    friend bool operator==(const 新服务取得目标实例键&,const 新服务取得目标实例键&)=default;
};
struct 新服务取得目标实例请求 final {
    新服务取得目标实例键 键;
    稳定编码 父需求;
    新需求完整目标 原父目标;
    稳定编码 人类概念,需求身份概念,完整内容概念;
    std::optional<稳定编码> 指定已有需求;
    // 本步依据（2026-11-08用户裁决：内部世界允许随时变化；采用只复核要采用的这一步）：使本前端成立的正式配置与
    // 来源前驱规则定义、交互者特征（当前值为H）、H属于人类的依据、本槽实例。非空。新出生、复用与补骨架各分支均以
    // 容许新增入向方式预期：其它节点新增指向这些节点不算变化；其本体改变或原引用删除为前提已变化。
    std::vector<基础联合节点快照> 正式前驱保护;
};
enum class 新服务取得目标实例状态 : std::uint8_t {
    已建立=1,已复用,已出生待补骨架,前提已变化,绑定冲突,
    输入不适用,资源失败,依赖未就绪,未找到
};
struct 新服务取得目标实例信息 final {
    稳定编码 出生记录,需求节点;
    新服务取得目标实例键 键;
    稳定编码 父需求;
    新需求完整目标 原父目标;
    稳定编码 人类概念,需求身份概念,完整内容概念;
    bool 骨架就绪=false;
    bool 复用指定已有需求=false;
    friend bool operator==(const 新服务取得目标实例信息&,const 新服务取得目标实例信息&)=default;
};
struct 新服务取得目标实例结果 final {
    新服务取得目标实例状态 状态{};
    std::optional<新服务取得目标实例信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};

enum class 新需求新增成员读取状态 : std::uint8_t {
    已读取=1, 前提已变化, 依赖未就绪, 未支持, 资源失败
};
struct 新需求新增成员基准 final {
    稳定编码 目标存在, 源特征节点, 源特征类型, 材料节点;
    std::vector<稳定编码> 成员组;
    friend bool operator==(const 新需求新增成员基准&, const 新需求新增成员基准&) = default;
};
struct 新需求新增成员基准结果 final {
    新需求新增成员读取状态 状态=新需求新增成员读取状态::未支持;
    std::optional<新需求新增成员基准> 基准;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新需求新增成员基准结果&, const 新需求新增成员基准结果&);
};
struct 新需求新增成员来源见证 final {
    稳定编码 任务节点, 自我需求节点;
    新行动请求承接来源 来源;
    friend bool operator==(const 新需求新增成员来源见证&, const 新需求新增成员来源见证&) = default;
};
struct 新需求新增成员判定结果 final {
    新需求新增成员读取状态 状态=新需求新增成员读取状态::未支持;
    std::optional<新需求新增成员基准> 当前集合;
    std::optional<std::int64_t> 原始新增数量;
    std::vector<新需求新增成员来源见证> 见证组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新需求新增成员判定结果&, const 新需求新增成员判定结果&);
};
class 新需求新增成员读取接口 {
public:
    virtual ~新需求新增成员读取接口()=default;
    virtual 新需求新增成员基准结果 读取新增成员目标基准(稳定编码, 稳定编码) const noexcept=0;
    virtual 新需求新增成员判定结果 判定新增成员来源(const 新需求完整目标&) const noexcept=0;
};

enum class 新行动请求来源读取状态 : std::uint8_t {
    已读取=1, 输入不适用, 前提已变化, 依赖未就绪, 资源失败
};
struct 新行动请求来源材料 final {
    新行动请求承接来源 来源;
    新需求完整目标 目标;
};
struct 新行动请求来源读取结果 final {
    新行动请求来源读取状态 状态=新行动请求来源读取状态::输入不适用;
    std::optional<新行动请求来源材料> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
enum class 新行动请求采用状态 : std::uint8_t {
    已建立=1, 已复用, 来源不适用, 前提已变化, 依赖未就绪, 父域不符, 同源冲突, 资源失败
};
struct 新行动请求采用结果 final {
    新行动请求采用状态 状态=新行动请求采用状态::来源不适用;
    std::optional<稳定编码> 需求节点, 需求列表项节点;
};
enum class 新引用内容目标状态 : std::uint8_t {
    已满足=1, 未满足, 分支待定, 未支持, 依赖未就绪,
    引用前提已变化, 定义不合法, 资源失败
};
struct 新引用内容目标判定 final {
    新引用内容目标状态 状态=新引用内容目标状态::依赖未就绪;
    std::optional<稳定编码> 场景节点, 实际特征节点;
    std::vector<正式结构项目判定> 必要项目组;
    std::vector<正式结构分支判定> 分支组;
    std::vector<基础联合节点快照> 完整保护组;
    // 读批保护不是静态提议身份；真正使用前仍独立fresh求值并联合复核。
    friend bool operator==(const 新引用内容目标判定& a,const 新引用内容目标判定& b) {
        return a.状态==b.状态&&a.场景节点==b.场景节点&&a.实际特征节点==b.实际特征节点
            &&a.必要项目组==b.必要项目组&&a.分支组==b.分支组;
    }
};
class 新引用内容目标读取接口 {
public:
    virtual ~新引用内容目标读取接口()=default;
    virtual 新需求目标定义核验结果 核验引用内容目标定义保护(const 新需求完整目标&) const noexcept=0;
    virtual 新引用内容目标判定 求值引用内容目标(const 新需求完整目标&) const noexcept=0;
    // 内容符合不证明主体的需求特征已经引用该节点。
    virtual 新引用内容目标判定 求值指定需求内容(const 新需求完整目标&) const noexcept=0;
};
enum class 新需求信息完善阶段 : std::int64_t { 待完善=1, 已完善=2 };
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
    std::optional<新需求信息完善阶段> 信息完善阶段;
    std::optional<新部分服务要求目标> 已知目标;
    std::optional<新需求复合分解来源> 复合来源;
    std::optional<新接收认知来源> 首次接收认知来源;
    std::optional<新行动请求承接来源> 行动请求来源{};
    friend bool operator==(const 新需求治理信息&, const 新需求治理信息&) = default;
};
struct 新接收认知需求建立请求 final {
    新接收认知来源 来源;
    新部分服务要求目标 已知目标;
};
struct 新被认知需求直接内容 final {
    稳定编码 需求节点, 主体存在节点;
    新需求信息完善阶段 信息完善阶段{};
    std::vector<正式直接内容项> 内容项组;
};
enum class 新需求内容读取状态 : std::uint8_t {
    已读取=1, 输入不合法, 需求不存在, 主体不符, 非被认知需求,
    内容项未支持, 依赖未就绪, 前提已变化, 资源失败
};
struct 新需求内容读取结果 final {
    新需求内容读取状态 状态=新需求内容读取状态::输入不合法;
    std::optional<新被认知需求直接内容> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};
enum class 新被认知需求来源种类 : std::uint8_t { 旧理解内容确认=1, 首次接收认知 };
enum class 新被认知需求资格状态 : std::uint8_t {
    已读取=1, 需求不存在, 主体不符, 非被认知需求, 内容待完善, 来源不符,
    需重新确认, 依赖未就绪, 前提已变化, 资源失败
};
struct 新被认知需求完整资格 final {
    新需求治理信息 需求;
    新被认知需求来源种类 来源种类{};
};
struct 新被认知需求资格读取结果 final {
    新被认知需求资格状态 状态=新被认知需求资格状态::依赖未就绪;
    std::optional<新被认知需求完整资格> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新需求内容判定结果 final {
    新引用内容目标状态 状态=新引用内容目标状态::依赖未就绪;
    std::vector<正式结构项目判定> 必要项目组;
    std::vector<正式结构分支判定> 分支组;
    std::vector<基础联合节点快照> 完整保护组;
};
enum class 新外部服务需求采用状态 : std::uint8_t {
    已建立=1, 已复用, 材料待完善, 需重新确认, 来源不符, 要求不存在, 工程故障,
    前提已变化=8, 资源失败=9,
    // FATAL-POPUP-FIX：目标内容依赖未就绪属合法等待（2026-11-11用户裁决）。
    依赖未就绪=10
};
struct 新外部服务需求采用结果 final {
    新外部服务需求采用状态 状态 = 新外部服务需求采用状态::工程故障;
    std::optional<稳定编码> 需求节点, 需求列表项节点;
};

// R108：需求所在列表项（完整目标相同的需求组）。只读基础数据、不取需求锁，
// 任务owner持任务锁时可调用；成员按节点编码升序。
enum class 新需求列表项读取状态 : std::uint8_t {
    已读取 = 1, 不在列表项, 资源失败, 结构不一致
};
struct 新需求列表项读取结果 final {
    新需求列表项读取状态 状态 = 新需求列表项读取状态::结构不一致;
    稳定编码 列表项节点;
    std::vector<稳定编码> 成员组;
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
    执行域不适用, 认知来源不合法, 同来源目标冲突, 需重新确认, 前提已变化,
    目标内容依赖未就绪=27, 目标内容未支持=28, 需求未满足
};
struct 新接收认知需求读取结果 final {
    新需求操作状态 状态=新需求操作状态::入口拒绝;
    std::optional<新需求治理信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};

struct 新被认知需求建立请求 final {
    稳定编码 需求主体存在节点;
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    新需求认知来源 认知来源;
};
struct 新待完善被认知需求建立请求 final {
    稳定编码 需求主体存在节点;
    新部分服务要求目标 已知目标;
};
struct 新被认知需求信息更新请求 final {
    稳定编码 需求节点;
    新需求治理信息 预期信息;
    新部分服务要求目标 已知目标;
    std::optional<新需求认知来源> 认知来源;
};
struct 新被认知需求信息更新结果 final {
    新需求操作状态 状态=新需求操作状态::入口拒绝;
    std::optional<新需求治理信息> 信息;
};
struct 新需求记录全集结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::vector<新需求信息> 需求组;
    std::vector<新需求治理信息> 待完善需求组;
};
enum class 新来源前驱候选枚举状态 : std::uint8_t {
    已读取=1, 输入不适用, 依赖未就绪, 前提已变化, 资源失败
};
struct 新来源前驱候选枚举结果 final {
    新来源前驱候选枚举状态 状态=新来源前驱候选枚举状态::依赖未就绪;
    稳定编码 主体存在节点;
    std::vector<新需求治理信息> 需求组;
    std::vector<新需求治理信息> 待完善需求组;
    std::vector<基础联合节点快照> 完整保护;
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
enum class 新需求发生方式 : std::uint8_t { 新需求=1, 已满足再次需要 };
struct 新需求发生核心定位 final {
    稳定编码 场景节点, 主体存在节点, 特征节点, 需求节点, 动作节点;
    新需求发生方式 方式=新需求发生方式::新需求;
    friend bool operator==(const 新需求发生核心定位&,const 新需求发生核心定位&)=default;
};
struct 新需求发生关联 final {
    新需求发生核心定位 定位;
    稳定编码 动作来源节点, 状态节点;
    friend bool operator==(const 新需求发生关联&,const 新需求发生关联&)=default;
};
enum class 新需求发生关联读取状态 : std::uint8_t { 已读取=1, 未找到 };
struct 新需求发生关联读取结果 final {
    新需求发生关联读取状态 状态=新需求发生关联读取状态::未找到;
    std::optional<新需求发生关联> 关联;
};
struct 新需求特征发布关联 final {
    稳定编码 需求节点, 主体存在节点, 场景节点, 动作节点;
    std::optional<稳定编码> 首状态节点;
    friend bool operator==(const 新需求特征发布关联&, const 新需求特征发布关联&) = default;
};
struct 新需求特征发布关联结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<新需求特征发布关联> 关联;
};
struct 新需求特征迁移来源项 final {
    稳定编码 需求节点, 动作节点, 前状态节点, 后状态节点;
    friend bool operator==(const 新需求特征迁移来源项&, const 新需求特征迁移来源项&) = default;
};
enum class 新需求特征事实用途 : std::uint8_t { 首次状态, 旧集合迁移, 迁移当前状态 };
struct 新需求特征事实核验输入 final {
    新需求特征事实用途 用途;
    稳定编码 场景节点, 主体存在节点, 原特征节点, 新特征节点, 当前需求节点;
    std::optional<稳定编码> 动作节点, 实际状态节点;
    std::vector<新需求特征迁移来源项> 有序来源组;
    friend bool operator==(const 新需求特征事实核验输入&, const 新需求特征事实核验输入&) = default;
};
struct 新需求特征事实依据 final {
    新需求特征事实核验输入 已核验输入;
    std::vector<基础联合节点快照> 完整保护组;
    基础封存追加片段 保护片段;
};
class 新需求特征事实凭据 final {
public:
    新需求特征事实凭据(新需求特征事实凭据&&) noexcept;
    新需求特征事实凭据& operator=(新需求特征事实凭据&&) noexcept;
    ~新需求特征事实凭据() noexcept;
    新需求特征事实凭据(const 新需求特征事实凭据&) = delete;
    新需求特征事实凭据& operator=(const 新需求特征事实凭据&) = delete;
    const 新需求特征事实依据& 依据() const noexcept;
private:
    struct 实现;
    explicit 新需求特征事实凭据(新需求特征事实依据, const 新_世界树类*) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 新_世界树类;
};
enum class 新需求特征事实核验状态 : std::uint8_t { 已核验, 前提已变化, 目标不合法 };
struct 新需求特征事实核验结果 final {
    新需求特征事实核验状态 状态 = 新需求特征事实核验状态::目标不合法;
    std::optional<新需求特征事实凭据> 凭据;
};
struct 新需求特征迁移信息 final {
    稳定编码 迁移记录节点, 场景节点, 主体存在节点, 原特征节点, 新特征节点, 当前需求节点;
    std::vector<新需求特征迁移来源项> 有序来源组;
    std::optional<稳定编码> 迁移当前状态节点;
};
struct 新需求特征迁移读取结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<新需求特征迁移信息> 信息;
};
class 新需求特征迁移准备 final {
public:
    新需求特征迁移准备(新需求特征迁移准备&&) noexcept;
    新需求特征迁移准备& operator=(新需求特征迁移准备&&) noexcept;
    ~新需求特征迁移准备() noexcept;
    新需求特征迁移准备(const 新需求特征迁移准备&) = delete;
    新需求特征迁移准备& operator=(const 新需求特征迁移准备&) = delete;
    const 基础封存追加片段& 片段() const noexcept;
private:
    struct 实现;
    explicit 新需求特征迁移准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 新_需求类;
};
struct 新需求特征迁移准备结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<新需求特征迁移准备> 准备;
};
struct 新需求特征迁移派生依据 final {
    稳定编码 场景节点, 主体存在节点, 原特征节点, 新特征节点, 需求类型根;
    稳定编码 当前需求节点, 原末状态节点;
    基础联合节点快照 新特征保护;
    std::vector<基础联合节点快照> 完整来源保护组;
    基础封存追加片段 来源保护片段;
};
class 新需求特征迁移派生凭据 final {
public:
    新需求特征迁移派生凭据(新需求特征迁移派生凭据&&) noexcept;
    新需求特征迁移派生凭据& operator=(新需求特征迁移派生凭据&&) noexcept;
    ~新需求特征迁移派生凭据() noexcept;
    新需求特征迁移派生凭据(const 新需求特征迁移派生凭据&) = delete;
    新需求特征迁移派生凭据& operator=(const 新需求特征迁移派生凭据&) = delete;
    const 新需求特征迁移派生依据& 依据() const noexcept;
private:
    struct 实现;
    explicit 新需求特征迁移派生凭据(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 新_需求类;
};
struct 新需求特征迁移派生准备结果 final {
    新需求操作状态 状态 = 新需求操作状态::入口拒绝;
    std::optional<新需求特征迁移派生凭据> 凭据;
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
    const 新需求发生核心定位& 发生定位() const noexcept;
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
    需求不存在, 结构不一致, 资源失败, 需求信息待完善, 前提已变化=9
};
struct 新服务要求执行守卫结果 final {
    新服务要求执行守卫状态 状态 = 新服务要求执行守卫状态::未装配;
    std::optional<稳定编码> 阻塞需求节点;
    std::vector<稳定编码> 确认节点组;
    std::vector<新行动请求承接来源> 行动请求来源组;
    std::vector<基础联合节点快照> 完整保护;
};
enum class 新服务要求需求建立状态 : std::uint8_t {
    已建立 = 1, 已复用, 未装配, 来源不符, 确认不合法,
    需重新确认, 既有需求来源冲突, 结构不一致, 资源失败, 前提已变化=10
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

struct 新需求读取取消 final {
    friend bool operator==(const 新需求读取取消&, const 新需求读取取消&) = default;
};
using 新需求全集可停止结果 = std::variant<新需求全集结果, 新需求读取取消>;
using 新需求列表全集可停止结果 = std::variant<新需求列表全集结果, 新需求读取取消>;

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
    需求信息待完善 = 15,
    目标引用前提已变化=16, 目标内容未支持=17, 目标内容分支待定=18,
    目标内容依赖未就绪=19, 目标内容定义不合法=20,
    // CONCEPT-INFO-MEMBERSHIP R-1：需求明确，但系统对当前值所指存在的认知不足以判断是否达到目标；
    // 不是已满足，也不是未满足。结果携带认知缺口组，由任务按目标分解形成认知子需求。
    当前值认知不足=21
};

struct 新需求当前满足结果 final {
    新需求当前满足状态 状态 = 新需求当前满足状态::入口拒绝;
    std::optional<稳定编码> 实际特征节点;
    // I64目标返回“目标边界 - 当前值”；正数表示需增加，负数表示需减少。
    std::optional<std::int64_t> 准确差异;
    // 仅“当前值认知不足”时非空：当前值所指存在相对目标值域概念尚未知道的项。
    std::vector<存在概念认知缺口> 认知缺口组;
    friend bool operator==(const 新需求当前满足结果&, const 新需求当前满足结果&) = default;
};
新需求当前满足结果 映射新增成员目标满足(const 新需求新增成员判定结果&);

enum class 新需求筹办资格状态 : std::uint8_t {
    允许筹办 = 1, 目标已满足, 等待普通材料, 需求非活动,
    需求待完善, 需求不存在, 目标定义不合法, 结构不一致,
    材料已变化, 资源失败,
    // FATAL-POPUP-FIX：依赖未就绪属合法等待（2026-11-11用户裁决）。
    依赖未就绪,
    // DEP-READY-WAKE A：任务场景核验批的运行期结构依赖未就绪（装配缺陷）；独立工程状态，由消费边界判定。
    // 只由任务owner复核任务筹办资格返回；需求owner的“依赖未就绪”来源混有业务材料未形成，不并入本值。
    结构依赖未就绪
};
struct 新需求筹办资格结果 final {
    新需求筹办资格状态 状态 = 新需求筹办资格状态::目标定义不合法;
    std::optional<新需求信息> 需求;
    std::optional<新需求当前满足结果> 当前满足;
    std::optional<二次特征概念信息> 派生定义;
    friend bool operator==(const 新需求筹办资格结果&, const 新需求筹办资格结果&) = default;
};

enum class 新行动请求筹办资格读取状态 : std::uint8_t {
    已读取=1,输入不适用,前提已变化,依赖未就绪,资源失败
};
struct 新行动请求筹办资格保护结果 final {
    新行动请求筹办资格读取状态 状态=新行动请求筹办资格读取状态::依赖未就绪;
    稳定编码 需求节点;
    std::optional<新需求筹办资格结果> 资格;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新行动请求筹办资格保护结果& a,const 新行动请求筹办资格保护结果& b) {
        return a.状态==b.状态&&a.需求节点==b.需求节点&&a.资格==b.资格;
    }
};

// ARCH-L4 需求组织服务。它只在同一个基础数据集中建立独立需求树，
// 世界树、特征概念树和二次特征概念只作为只读引用来源。
class 新_需求类 final : public 特征需求成员读取接口, public 正式信息参数读取接口 {
public:
    新服务取得目标实例结果 读取服务取得目标实例(const 新服务取得目标实例键&) const noexcept;
    新服务取得目标实例结果 读取服务取得目标槽实例(稳定编码 父取得任务,稳定编码 目标槽类型,
        const std::vector<稳定编码>& 正式根路径) const noexcept;
    bool 装配新增成员读取接口(const 新需求新增成员读取接口*) noexcept;
    新需求目标定义核验结果 核验新增成员目标定义保护(const 新需求完整目标&) const noexcept;
    新需求新增成员判定结果 读取新增成员来源见证(稳定编码) const noexcept;
    新需求操作状态 装配引用内容目标读取接口(const 新引用内容目标读取接口*) noexcept;
    新引用内容目标判定 复核引用内容目标(稳定编码 自我需求节点) const noexcept;
    新引用内容目标判定 求值引用内容条件目标(const 新需求完整目标&) const noexcept;
    新引用内容目标判定 求值指定需求内容目标(const 新需求完整目标&) const noexcept;
    新需求筹办资格结果 复核筹办资格(稳定编码 需求节点) const noexcept;
    // 仅来源前驱task上下文/fresh使用；无T仍可读取，不赋予执行许可。
    新行动请求筹办资格保护结果 读取行动请求自我需求筹办资格保护(
        const 新行动请求承接来源&,稳定编码 Ds) const noexcept;
    新_需求类(
        新_特征值类& 特征值服务,
        新_存在类& 存在服务,
        新_特征类& 特征服务,
        概念_特征类& 特征概念服务,
        概念_二次特征类& 二次特征概念服务,
        概念_存在类& 存在概念服务) noexcept;

    // 只建立需求树自身的根、类型节点和字段节点。
    bool 初始化() noexcept;
    // 先天目标FT及正式概念读取端口就绪后、开放线程前逐D迁移。
    新需求操作状态 启用目标特征实例承载() noexcept;

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
    正式信息参数读取结果 读取正式信息参数保护(
        稳定编码 信息节点, 稳定编码 范围概念) const noexcept override;
    // 同一D的直接内容投影，不执行概念符合性判定或fresh确认。
    新需求内容读取结果 读取被认知需求直接内容保护(
        稳定编码 需求节点, 稳定编码 预期主体,
        const std::vector<稳定编码>& 正式内容FT组) const noexcept;
    void 装配接收认知语言读取接口(const 新_语言类*) noexcept;
    新被认知需求资格读取结果 读取被认知需求完整资格保护(稳定编码,稳定编码) const noexcept;
    新需求内容判定结果 判定被认知需求内容保护(稳定编码,稳定编码,稳定编码,
        const std::vector<稳定编码>& 正式内容路径 = {}) const noexcept;
    新被认知需求信息更新结果 完善接收认知需求信息(稳定编码,const 新需求治理信息&) noexcept;
    void 装配部分服务要求读取接口(const 新部分服务要求读取接口*) noexcept;
    std::optional<新需求列表项信息> 获取需求列表项(
        稳定编码 列表项节点) const noexcept;
    // R108：不取需求锁；列表项反查多于一项或成员非需求为结构不一致。
    新需求列表项读取结果 读取需求所在列表项(稳定编码 需求节点) const noexcept;
    std::optional<稳定编码> 查询根需求(
        新需求根角色 角色) const noexcept;
    新需求全集结果 查询全部需求() const noexcept;
    新需求全集可停止结果 查询全部需求(std::stop_token) const noexcept;
    // 全部需求/列表及目标查询仅覆盖自我执行域；记录查询覆盖两域。
    新需求建立结果 建立或取得被认知需求(const 新被认知需求建立请求&) noexcept;
    新需求建立结果 建立待完善被认知需求(const 新待完善被认知需求建立请求&) noexcept;
    新需求建立结果 建立或取得接收认知需求(
        const 新接收认知需求建立请求&, const 新_语言类&, const 新_特征类&) noexcept;
    新接收认知需求读取结果 查询接收认知来源需求(const 新接收认知来源键&) const noexcept;
    新接收认知需求读取结果 查询接收认知来源需求(const 新接收认知来源&,
        const 新_语言类&, const 新_特征类&) const noexcept;
    新被认知需求信息更新结果 更新被认知需求信息(const 新被认知需求信息更新请求&) noexcept;
    特征需求成员读取结果 读取需求集合成员身份(稳定编码) const noexcept override;
    特征需求引用读取结果 读取需求节点引用身份(稳定编码) const noexcept override;
    // R102（SET-DOMAIN-MEMBER-CONCEPT §4）：需求成员是否属于需求类别概念（槽1或其下位）；不取需求锁。
    特征成员概念归属结果 判定需求集合成员概念(稳定编码 需求节点, 稳定编码 概念) const noexcept override;
    // 无锁直接内容：冻结schema+L1读取，全部需求（不限被认知），按先天需求槽FT投影；保护含D快照与槽保护。
    特征概念规则状态 读取需求直接内容无锁(稳定编码 需求节点,
        std::vector<std::pair<稳定编码, 正式内容值>>& 内容组,
        std::vector<基础联合节点快照>& 保护) const noexcept;
    新需求集合发布关联结果 读取主体需求集合发布关联(稳定编码 需求节点) const noexcept;
    新需求集合发布准备结果 准备主体需求特征发布(
        稳定编码, 稳定编码, 稳定编码, 稳定编码, 稳定编码, const 基础联合节点快照&) noexcept;
    新需求集合发布准备结果 准备主体需求再次需要(
        稳定编码,稳定编码,稳定编码,稳定编码,稳定编码,const 基础联合节点快照&) noexcept;
    void 完成主体需求再次需要(const 新需求集合发布准备&,const 基础片段发布回执&) noexcept;
    新需求发生关联读取结果 读取主体需求发生关联按动作(稳定编码) const noexcept;
    新需求发生关联读取结果 读取主体需求发生关联按来源(稳定编码) const noexcept;
    新需求发生关联读取结果 读取主体需求发生关联按状态(稳定编码) const noexcept;
    新需求操作状态 记录主体需求发生关联(const 新需求发生关联&) noexcept;
    bool 是需求迁移恢复状态(稳定编码) const noexcept;
    新需求特征发布关联结果 读取主体需求特征发布关联(稳定编码) const noexcept;
    void 完成主体需求特征发布(const 新需求集合发布准备&, const 基础片段发布回执&) noexcept;
    新需求操作状态 完成主体需求首状态关联(稳定编码, const 新需求特征事实凭据&) noexcept;
    新需求特征迁移准备结果 准备需求特征迁移(const 新需求特征事实凭据&) noexcept;
    void 完成需求特征迁移核心(const 新需求特征迁移准备&, const 基础片段发布回执&) noexcept;
    新需求操作状态 完成需求特征迁移状态(稳定编码, const 新需求特征事实凭据&) noexcept;
    新需求特征迁移读取结果 读取需求特征迁移(稳定编码) const noexcept;
    新需求特征迁移派生准备结果 准备需求特征迁移派生(稳定编码, const 新需求特征事实凭据&) const noexcept;
    新需求记录全集结果 查询全部需求记录() const noexcept;
    新需求记录全集结果 查询主体需求(稳定编码 主体存在, 新需求执行域 域) const noexcept;
    // 本族task在最外任务锁外读取；成功空域同样携带完整范围保护，不核资格。
    新来源前驱候选枚举结果 读取服务取得主体被认知需求候选保护(稳定编码 H) const noexcept;
    新需求记录全集结果 查询被认知需求来源(稳定编码 理解节点) const noexcept;
    // 完整核验列表登记、原始成员及需求树覆盖；非成功不返回部分材料。
    新需求列表全集结果 查询全部需求列表() const noexcept;
    新需求列表全集可停止结果 查询全部需求列表(std::stop_token) const noexcept;

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
    // 调用方须在任务锁外使用；确认owner提供语义及完整保护，本类不另行解释。
    新服务要求确认保护结果 准备服务要求确认语义保护(稳定编码 确认节点) const noexcept;
    新部分服务要求读取结果 读取部分服务要求(稳定编码 要求节点) const noexcept;
    新服务要求确认读取结果 复核服务要求确认语义(稳定编码 确认节点) const noexcept;
    新服务要求执行守卫结果 复核需求服务要求语义(稳定编码 需求节点) const noexcept;
    新行动请求来源读取结果 读取行动请求承接来源保护(const 新行动请求承接来源&) const noexcept;
    新接收认知需求读取结果 查询行动请求自我需求(const 新行动请求承接来源&) const noexcept;
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
    // 初始化后schema冻结；不取需求锁。新格式经F公开口取得完整保护，使用共享领域锁。
    特征概念规则状态 校验集合成员需求身份(稳定编码 需求节点) const noexcept;

private:
    struct 提议源收集批 final {
        基础只读预期 前提;
        std::set<稳定编码> 需求节点组;
    };
    static void 合并提议源前提(基础只读预期&,const 基础只读预期&);
    bool 核同提议源(const 提议源收集批*) const;
    static bool 提议需求值相同(const std::optional<新需求信息>&,
        const std::optional<新需求信息>&,const 提议源收集批*) noexcept;
    void 采集提议源需求本体保护(稳定编码,提议源收集批&,
        std::vector<基础联合节点快照>&) const;
    std::optional<新需求信息> 读取提议源需求(稳定编码,提议源收集批&,
        std::vector<基础联合节点快照>&,std::optional<新需求治理信息>* = nullptr) const;
    新服务要求执行守卫结果 复核需求服务要求语义内部(稳定编码,提议源收集批*) const noexcept;
    新接收认知需求读取结果 查询行动请求自我需求内部(
        const 新行动请求承接来源&,提议源收集批*) const noexcept;
    新需求新增成员判定结果 读取新增成员来源见证内部(稳定编码,提议源收集批*) const noexcept;
    struct 来源候选枚举保护批;
    void 追加来源候选枚举固定保护(来源候选枚举保护批&) const;
    void 追加来源候选枚举原始闭包保护(稳定编码,来源候选枚举保护批&) const;
    void 追加来源候选枚举本域保护(稳定编码,来源候选枚举保护批&) const;
    struct 筹办保护收集批;
    新需求筹办资格结果 复核筹办资格可抛内部(稳定编码,筹办保护收集批*,
        提议源收集批* = nullptr,std::vector<基础联合节点快照>* = nullptr) const;
    新需求当前满足结果 复核当前满足可抛内部(稳定编码,筹办保护收集批*,
        提议源收集批* = nullptr,std::vector<基础联合节点快照>* = nullptr) const;
    void 追加筹办本域保护(稳定编码,筹办保护收集批&,bool 来源枚举读取=false) const;
    void 追加筹办存在值域归属保护(稳定编码,稳定编码,筹办保护收集批&) const;
    // 仅正式任务绑定入口调用；不开放任意快照出生能力。
    friend class 新_任务类;
    // 提议读取不以旧权重限制采用；外部材料仍是完整快照，不按节点编码豁免。
    struct 引用内容提议读取 final {
        新引用内容目标判定 判定;
        std::optional<新需求信息> 需求;
        基础只读预期 源前提;
    };
    引用内容提议读取 读取引用内容提议(稳定编码, bool 完整源 = false) const noexcept;
    bool 核同引用内容提议源(const 引用内容提议读取&) const;
    新服务取得目标实例结果 建立或取得服务取得目标实例(const 新服务取得目标实例请求&) noexcept;
    新服务取得目标实例结果 读取服务取得目标实例可抛内部(const 新服务取得目标实例键&,
        新服务取得目标实例结果& 已成立前缀) const;
    新服务取得目标实例结果 读取服务取得目标实例核心可抛内部(稳定编码 父取得任务,稳定编码 目标槽类型,
        const std::vector<稳定编码>& 正式根路径,std::optional<稳定编码> 限定H,
        新服务取得目标实例结果& 前缀) const;
    struct 服务取得角色核验批;
    服务取得角色核验批 读取服务取得角色核验批(稳定编码,稳定编码,稳定编码,稳定编码) const;
    新行动请求采用结果 建立或取得行动请求需求已核验(
        const 新行动请求承接来源&, 稳定编码 完成服务需求) noexcept;
    新接收认知需求读取结果 查询行动请求自我需求已加锁(
        const 新行动请求承接来源&, const std::vector<基础联合节点快照>& 来源保护,
        提议源收集批* = nullptr) const;
    struct 接收认知来源保护结果;
    新接收认知需求读取结果 查询接收认知来源需求已加锁(const 新接收认知来源键&,
        const std::vector<基础联合节点快照>& 来源保护) const;
    接收认知来源保护结果 读取接收认知需求来源保护(const 新_语言类&,const 新_特征类&,
        const 新接收认知来源&,稳定编码,const 新部分服务要求目标&) const;
    新需求建立结果 建立待完善被认知需求内部(const 新待完善被认知需求建立请求&,
        const std::optional<新接收认知来源>&,
        const std::vector<基础联合节点快照>&) noexcept;
    新需求操作状态 确保目标特征实例承载内部(稳定编码) noexcept;
    // 向上送出：共享技术读取及旧采用链保留明确漂移/资源异常，不经旧工程包装。
    std::optional<新需求治理信息> 读取需求治理信息可抛内部(稳定编码,
        来源候选枚举保护批* 保护=nullptr,const 基础只读预期* = nullptr,
        std::vector<基础联合节点快照>* = nullptr) const;
    // friend任务读取本节点当前内容；不展开全树保护，不吞读取故障。
    std::optional<新需求治理信息> 读取当前需求治理信息(稳定编码) const;
    // 仅friend任务的来源准备证明使用；不核当前必要性，也不授予执行许可。
    enum class 来源准备治理读取状态 : std::uint8_t {
        已读取=1,需求不存在,尚未初始化,依赖未就绪,前提已变化,资源失败
    };
    struct 来源准备单需求治理保护结果 final {
        来源准备治理读取状态 状态=来源准备治理读取状态::依赖未就绪;
        std::optional<新需求治理信息> 信息;
        std::vector<基础联合节点快照> 完整保护;
    };
    using 来源准备单需求治理可停止结果=
        std::variant<来源准备单需求治理保护结果,新需求读取取消>;
    struct 来源准备自我列表域保护结果 final {
        来源准备治理读取状态 状态=来源准备治理读取状态::依赖未就绪;
        std::vector<新需求列表项信息> 列表项组;
        std::vector<新需求治理信息> 需求组;
        std::vector<基础联合节点快照> 完整保护;
    };
    using 来源准备自我列表域可停止结果=
        std::variant<来源准备自我列表域保护结果,新需求读取取消>;
    来源准备单需求治理可停止结果 读取来源准备单需求治理保护(稳定编码,std::stop_token) const;
    来源准备自我列表域可停止结果 读取来源准备自我列表域保护(std::stop_token) const;
    新需求全集可停止结果 查询全部需求可抛内部(std::stop_token,
        来源候选枚举保护批* 保护=nullptr) const;
    新需求记录全集结果 查询全部需求记录可抛内部(来源候选枚举保护批* 保护=nullptr,
        std::vector<新需求治理信息>* 同次完整治理组=nullptr) const;
    新需求列表全集可停止结果 查询全部需求列表可抛内部(std::stop_token,
        来源候选枚举保护批* 保护=nullptr) const;
    新需求精确查询结果 按完整目标查询可抛内部(稳定编码,const 新需求目标定义&) const;
    新服务要求采用关系结果 查询服务要求采用关系可抛内部(稳定编码) const;
    新服务要求采用关系结果 查询服务需求取得来源可抛内部(稳定编码) const;
    新需求操作状态 配置服务要求采用来源可抛内部(
        稳定编码,稳定编码,const 特征任务成员读取接口&);
    新需求操作状态 确保目标特征实例承载可抛内部(稳定编码);
    新需求建立结果 建立或取得普通需求来源可抛内部(
        const 新普通需求建立请求&,bool,std::optional<稳定编码>,
        std::optional<稳定编码>,新需求建立结果& 已成立前缀,
        const std::vector<基础联合节点快照>& 新增目标保护 = {});
    新需求操作状态 要求目标特征实例承载(稳定编码) noexcept;
    bool 目标特征实例承载已启用_ = false;
    新需求操作状态 迁移既有自我需求归属(稳定编码 唯一自我) noexcept;
    friend class 新_自我线程类;
    新外部服务需求采用结果 建立或取得外部服务需求已核验(
        const 新完整服务需求材料包&,
        std::optional<稳定编码> 取得执行节点 = std::nullopt,
        const 新外部服务需求采用结果* 已知前缀 = nullptr) noexcept;
    // 只供self在核验父需求与来源任务一一对应后采用任务条件子目标。
    新需求建立结果 建立或取得普通需求已核验(
        const 新普通需求建立请求& 请求,
        bool 允许任务子目标,
        const std::vector<基础联合节点快照>& 复合采用保护 = {}) noexcept;
    新需求建立结果 建立或取得复合提议需求已核验(
        const 新普通需求建立请求&, const 引用内容提议读取&) noexcept;
    // CONCEPT-INFO-MEMBERSHIP §4.4：建立或复用存在归属分解子需求（自我确认后调用）。
    新需求建立结果 建立或取得归属分解需求已核验(const 新普通需求建立请求&) noexcept;
    新需求建立结果 建立或取得新增成员见证需求已核验(
        const 新普通需求建立请求&, const 新需求新增成员见证要求&);
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
    新需求建立结果 建立或取得引用内容需求内部(const 新普通需求建立请求&,bool,
        const std::vector<基础联合节点快照>& 复合采用保护 = {},
        const 基础只读预期* 源前提 = nullptr) noexcept;

    新_特征值类& 特征值服务_;
    新_存在类& 存在服务_;
    新_特征类& 特征服务_;
    概念_特征类& 特征概念服务_;
    概念_二次特征类& 二次特征概念服务_;
    概念_存在类& 存在概念服务_;
    const void* 关联检查通知所有者_ = nullptr;
    std::function<void()> 关联检查通知_;
    const 新服务要求确认读取接口* 服务要求确认读取接口_ = nullptr;
    const 新部分服务要求读取接口* 部分服务要求读取接口_ = nullptr;
    const 新_语言类* 接收认知语言读取接口_ = nullptr;
    const 新引用内容目标读取接口* 引用内容目标读取接口_ = nullptr;
    const 新需求新增成员读取接口* 新增成员读取_ = nullptr;
    const 特征任务成员读取接口* 服务要求任务读取接口_ = nullptr;
    稳定编码 服务要求结算需求_;
    稳定编码 服务要求结算任务_;
};

} // namespace 海中鱼巣
