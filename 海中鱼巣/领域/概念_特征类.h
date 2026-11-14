#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <memory>
#include <variant>
#include <vector>
#include <span>

#include "../核心/基础数据集.h"
#include "新_特征值类.h"

namespace 海中鱼巣 {

class 概念_名称关系类;
class 概念_特征类;
class 新消息待发表凭据;

// I64标量直接保存在特征节点中，其余材料由新_特征值类保存。
enum class 特征概念材料物理类型 : std::int64_t {
    I64标量 = 1,
    U64数组 = 2,
    I64数组 = 3,
    UTF8字符串 = 4,
    二维二值格 = 5,
    三维二值格 = 6,
    // 值由稳定编码直接引用一个存在实例；值域由存在概念限定。
    存在节点引用 = 7,
    需求节点引用 = 8,
    信息节点引用 = 9
};

struct 特征概念I64闭区间 final {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const 特征概念I64闭区间&,
        const 特征概念I64闭区间&) = default;
};

// 表示该材料物理类型下的全部合法元素。它不是空集合。
struct 特征概念全材料值域 final {
    friend bool operator==(const 特征概念全材料值域&,
        const 特征概念全材料值域&) = default;
};

struct 特征概念I64分量定义 final {
    稳定编码 角色;
    std::optional<稳定编码> 单位;
    std::vector<特征概念I64闭区间> 值域;
    friend bool operator==(const 特征概念I64分量定义&,
        const 特征概念I64分量定义&) = default;
};

// 维度数量由分量.size()唯一得出，不另存第二份数量事实。
struct 特征概念结构化I64值域 final {
    std::vector<特征概念I64分量定义> 分量;
    std::size_t 维度数量() const noexcept { return 分量.size(); }
    friend bool operator==(const 特征概念结构化I64值域&,
        const 特征概念结构化I64值域&) = default;
};

struct 特征概念存在引用值域 final {
    稳定编码 存在概念节点;
    friend bool operator==(const 特征概念存在引用值域&,
        const 特征概念存在引用值域&) = default;
};

// 无序任务集合的正向成员约束；不是完整特征值材料节点的有限枚举。
struct 特征概念任务集合值域 final {
    std::vector<稳定编码> 必含任务节点组;
    bool 至少一个任务 = false;
    std::vector<稳定编码> 必含需求任务组;
    // 正式确认身份；不以任意治理任务非空代替该来源要求。
    std::vector<稳定编码> 必含服务要求任务组;
    // R102：概念成员（至少一个成员属于该概念）与概念子集（全部成员属于该概念）。
    std::vector<稳定编码> 含概念成员组;
    std::vector<稳定编码> 全部属于概念组;
    friend bool operator==(const 特征概念任务集合值域&,
        const 特征概念任务集合值域&) = default;
};

struct 特征概念需求集合值域 final {
    std::vector<稳定编码> 必含需求节点组;
    bool 至少一个需求 = false;
    // R102：概念成员（至少一个成员属于该概念）与概念子集（全部成员属于该概念）。
    std::vector<稳定编码> 含概念成员组;
    std::vector<稳定编码> 全部属于概念组;
    friend bool operator==(const 特征概念需求集合值域&,
        const 特征概念需求集合值域&) = default;
};

struct 特征概念存在集合值域 final {
    std::vector<稳定编码> 必含存在节点组;
    bool 至少一个存在 = false;
    // R102：概念成员（至少一个成员属于该概念）与概念子集（全部成员属于该概念）。
    std::vector<稳定编码> 含概念成员组;
    std::vector<稳定编码> 全部属于概念组;
    friend bool operator==(const 特征概念存在集合值域&,
        const 特征概念存在集合值域&) = default;
};
enum class 信息引用语义 : std::uint8_t { 信息对象=1, 概念定义=2 };
struct 特征概念信息引用值域 final {
    信息引用语义 语义;
    稳定编码 内容概念;
    friend bool operator==(const 特征概念信息引用值域&, const 特征概念信息引用值域&) = default;
};

struct 特征概念概念集合值域 final {
    std::vector<稳定编码> 必含概念节点组;
    bool 至少一个概念 = false;
    friend bool operator==(const 特征概念概念集合值域&, const 特征概念概念集合值域&) = default;
};

// I64标量使用规范化闭区间；结构化I64材料使用有序分量值域；
// 其它材料可使用全材料值域或完整特征值节点身份集合。
using 特征概念值域 = std::variant<
    std::vector<特征概念I64闭区间>,
    特征概念全材料值域,
    std::vector<稳定编码>,
    特征概念结构化I64值域,
    特征概念存在引用值域,
    特征概念任务集合值域,
    特征概念需求集合值域,
    特征概念存在集合值域,
    特征概念信息引用值域,
    特征概念概念集合值域>;

using 特征概念准确值 = std::variant<std::int64_t, 稳定编码>;

enum class 特征概念比较规则 : std::int64_t {
    I64数值 = 1,
    完整材料精确 = 2,
    存在身份精确 = 3,
    服务任务集合身份判等 = 4,
    需求集合身份判等 = 5,
    存在集合身份判等 = 6,
    消息集合身份判等 = 7,
    需求身份精确 = 8,
    信息引用身份精确 = 9,
    概念集合身份判等 = 10
};

enum class 特征概念聚合规则 : std::int64_t {
    不自动聚合 = 1,
    I64连续值归组 = 2,
    完整材料精确集合 = 3
};

struct 特征概念定义 final {
    特征概念材料物理类型 材料类型 = 特征概念材料物理类型::I64标量;
    特征概念值域 值域;
    std::optional<稳定编码> 单位;
    特征概念比较规则 比较规则 = 特征概念比较规则::I64数值;
    特征概念聚合规则 聚合规则 = 特征概念聚合规则::不自动聚合;
    std::vector<稳定编码> 名称关系;
    friend bool operator==(const 特征概念定义&, const 特征概念定义&) = default;
};

struct 特征概念信息 final {
    稳定编码 节点;
    特征概念材料物理类型 材料类型 = 特征概念材料物理类型::I64标量;
    特征概念值域 值域;
    std::optional<稳定编码> 单位;
    特征概念比较规则 比较规则 = 特征概念比较规则::I64数值;
    特征概念聚合规则 聚合规则 = 特征概念聚合规则::不自动聚合;

    // 保存由自然语言类建立的名称关系身份；词条和关系操作稍后实现。
    std::vector<稳定编码> 名称关系;

    // 当前通过正式特征概念字段直接引用本概念的实例特征节点数量。
    std::int64_t 当前引用实例数量 = 0;

    friend bool operator==(const 特征概念信息&, const 特征概念信息&) = default;
};

enum class 特征概念直接子查询状态 : std::uint8_t {
    已完成, 输入拒绝, 内部不一致, 工程未完成, 前提已变化
};

struct 特征概念直接子查询结果 final {
    特征概念直接子查询状态 状态 = 特征概念直接子查询状态::输入拒绝;
    std::vector<稳定编码> 子概念节点组;
};

enum class 特征概念规则状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    材料类型不相容 = 4,
    值不存在 = 5,
    规则不支持 = 6,
    资源失败 = 7,
    内部不一致 = 8,
    读取接口未装配 = 9,
    前提已变化 = 10,
    // R102：集合成员概念归属无法确定（不等于不满足）。
    成员归属未知 = 11
};
// F准备之前取得，同线程一次纯构造，不持跨调用锁。
class 新消息集合保持材料 final {
public:
    ~新消息集合保持材料();
    新消息集合保持材料(新消息集合保持材料&&) noexcept;
    新消息集合保持材料& operator=(新消息集合保持材料&&) noexcept;
    新消息集合保持材料(const 新消息集合保持材料&) = delete;
    新消息集合保持材料& operator=(const 新消息集合保持材料&) = delete;
private:
    struct 实现;
    explicit 新消息集合保持材料(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 概念_特征类;
};
struct 新消息集合保持读取结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<新消息集合保持材料> 材料;
};
enum class 正式概念读取状态 : std::uint8_t { 已读取=1, 非本提供者概念, 依赖未就绪, 前提已变化 };
struct 正式概念身份保护 final {
    稳定编码 概念节点;
    std::vector<基础联合节点快照> 完整保护组;
};
struct 正式概念身份读取结果 final {
    正式概念读取状态 状态 = 正式概念读取状态::非本提供者概念;
    std::optional<正式概念身份保护> 身份;
};
class 正式概念身份读取接口 {
public:
    virtual ~正式概念身份读取接口() = default;
    virtual 正式概念身份读取结果 读取正式概念身份保护(稳定编码) const noexcept = 0;
};
struct 先天特征类型槽请求 final { std::uint32_t 语义序号; 特征概念定义 定义; };
struct 先天特征类型槽结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<稳定编码> 类型根;
    std::vector<基础联合节点快照> 完整保护组;
};
namespace 先天特征类型槽 {
inline constexpr std::uint32_t 需求主体=1001, 需求执行域=1002, 需求目标存在=1003, 需求目标FT=1004,
    需求目标形式=1005, 需求权重=1006, 需求活动=1007, 目标域=1008, 目标比较=1009, 目标方向=1010,
    父需求=1011, 需求来源=1012, 需求根角色=1013, 理解来源=1014, 内容确认来源=1015,
    引用内容需求=1016, 引用内容项=1017;
inline constexpr std::uint32_t 对应需求=1101, 任务虚拟存在=1102, 采用方法=1103, 执行组织=1104,
    执行实例=1105, 评估依据=1106, 结算依据=1107;
inline constexpr std::uint32_t 方法头=1201, 方法作用域=1202, 方法实现方式=1203, 方法行为=1204,
    动作定义=1205, 实现绑定=1206, 方法虚拟存在=1207, 共同约束=1208, 条件结果知识=1209,
    本能登记=1210, 作用存在概念=1211, 涉及FT概念=1212, 可能结果概念=1213;
inline constexpr std::uint32_t 有序参数规格=1301, 方法运行次数=1302, 方法成功次数=1303, 方法可靠度=1304,
    参数序号=1311, 参数信息类型=1312, 参数范围=1313, 所属方法=1401, 条件项=1402,
    父条件=1403, 配对结果=1404, 结果项=1405;
inline constexpr std::uint32_t 概念名称挂接=1501, 词典材料=1502, 词典来源批次=1503;
}

struct 特征概念编码操作结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
};
struct 特征概念查询操作结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::vector<稳定编码> 概念节点组;
};
class 存在集合保护准备凭据 final {
public:
    ~存在集合保护准备凭据();
    存在集合保护准备凭据(存在集合保护准备凭据&&) noexcept;
    存在集合保护准备凭据& operator=(存在集合保护准备凭据&&) noexcept;
    存在集合保护准备凭据(const 存在集合保护准备凭据&) = delete;
    存在集合保护准备凭据& operator=(const 存在集合保护准备凭据&) = delete;
private:
    struct 实现;
    explicit 存在集合保护准备凭据(std::unique_ptr<实现>);
    std::unique_ptr<实现> 实现_;
    friend class 概念_特征类;
};
struct 存在集合保护准备结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<存在集合保护准备凭据> 凭据;
};
struct 存在集合依据推进结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::vector<基础联合节点快照> 推进后依据;
};

struct 特征存在成员读取结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<稳定编码> 存在节点;
    std::vector<基础联合节点快照> 身份保护;
};
// R102：集合成员是否属于概念，由成员原领域owner判定；判定不得取业务锁。
enum class 特征成员概念归属 : std::uint8_t { 属于 = 1, 不属于, 未知 };
struct 特征成员概念归属结果 final {
    // 已完成时归属有效；读取接口未装配/资源失败/前提已变化/内部不一致按工程或重读路径处理。
    特征概念规则状态 状态 = 特征概念规则状态::读取接口未装配;
    特征成员概念归属 归属 = 特征成员概念归属::未知;
};
class 特征存在成员读取接口 {
public:
    virtual ~特征存在成员读取接口() = default;
    virtual 特征存在成员读取结果 读取存在集合成员身份(稳定编码) const noexcept = 0;
    virtual 特征成员概念归属结果 判定存在集合成员概念(稳定编码, 稳定编码) const noexcept { return {}; }
};
struct 特征消息成员读取结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<稳定编码> 消息节点;
    std::vector<基础联合节点快照> 身份保护;
};
class 特征消息成员读取接口 {
public:
    virtual ~特征消息成员读取接口() = default;
    virtual 特征消息成员读取结果 读取消息集合成员身份(稳定编码) const noexcept = 0;
};
struct 特征存在集合条件比较结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::int64_t 满足 = 0;
    std::vector<稳定编码> 缺少存在节点组;
    bool 缺少任意存在 = false;
    std::vector<稳定编码> 缺少概念成员组, 不属于概念组, 未知概念组;
};

enum class 特征任务成员读取状态 : std::uint8_t {
    已读取 = 1, 未装配, 节点不存在, 结构不一致, 资源失败
};

struct 特征需求成员身份 final {
    稳定编码 需求节点, 主体存在节点;
    bool 被认知 = false;
};

struct 特征需求成员读取结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<特征需求成员身份> 身份;
};

struct 特征需求引用读取结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<特征需求成员身份> 身份;
    std::vector<基础联合节点快照> 身份保护;
};

class 特征需求成员读取接口 {
public:
    virtual ~特征需求成员读取接口() = default;
    virtual 特征需求成员读取结果 读取需求集合成员身份(稳定编码) const noexcept = 0;
    virtual 特征需求引用读取结果 读取需求节点引用身份(稳定编码) const noexcept;
    virtual 特征成员概念归属结果 判定需求集合成员概念(稳定编码, 稳定编码) const noexcept { return {}; }
};

struct 特征需求集合条件比较结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::int64_t 满足 = 0;
    std::vector<稳定编码> 缺少需求节点组;
    bool 缺少任意需求 = false;
    std::vector<稳定编码> 缺少概念成员组, 不属于概念组, 未知概念组;
};

struct 特征任务成员读取结果 final {
    特征任务成员读取状态 状态 = 特征任务成员读取状态::未装配;
    std::optional<稳定编码> 需求节点;
};

class 特征任务成员读取接口 {
public:
    virtual ~特征任务成员读取接口() = default;
    virtual 特征任务成员读取结果 读取任务所属需求(稳定编码) const noexcept = 0;
    virtual 特征概念规则状态 验证正式需求(稳定编码) const noexcept = 0;
    virtual 特征概念规则状态 验证正式服务要求(稳定编码) const noexcept = 0;
    // 已读取+空需求表示确认合法但尚未采用；与读取失败分账。
    virtual 特征任务成员读取结果 读取服务要求对应需求(稳定编码) const noexcept = 0;
    virtual 特征成员概念归属结果 判定任务集合成员概念(稳定编码, 稳定编码) const noexcept { return {}; }
    // R108：任务服务需求集S(T)（建立需求及同列表项无直接任务的成员，升序）；读取失败返回空。
    // 只读基础数据，不取任务锁；需求owner持需求锁调用安全。
    virtual std::vector<稳定编码> 读取任务服务需求组(稳定编码) const noexcept { return {}; }
    virtual bool 需求属于任务服务需求集(稳定编码, 稳定编码) const noexcept { return false; }
};

struct 特征任务集合条件比较结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<std::int64_t> 比较值;
    std::vector<稳定编码> 命中任务节点组;
    std::vector<稳定编码> 缺少任务节点组;
    std::vector<稳定编码> 缺少需求节点组;
    bool 缺少任意任务 = false;
    std::vector<稳定编码> 缺少服务要求节点组;
    std::vector<稳定编码> 缺少概念成员组, 不属于概念组, 未知概念组;
};

enum class 特征概念比较次序 : std::int8_t {
    小于 = -1,
    相等 = 0,
    大于 = 1
};

struct 特征概念比较结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<bool> 相等;
    std::optional<特征概念比较次序> 次序;
    std::optional<std::uint64_t> I64绝对差;
    bool 成功() const noexcept;
};

enum class 特征概念值域关系 : std::uint8_t {
    相等 = 1,
    左包含右 = 2,
    右包含左 = 3,
    相交 = 4,
    分离 = 5
};

struct 特征概念值域比较结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<特征概念值域关系> 关系;
};

enum class 单特征概念关系 : std::uint8_t {
    同一 = 1,
    左为上位 = 2,
    右为上位 = 3,
    具有共同上位 = 4,
    无可比关系 = 5
};

struct 单特征概念关系结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<单特征概念关系> 关系;
    std::optional<稳定编码> 特征类型根;
    std::optional<稳定编码> 最近共同上位;
};

struct 特征概念聚合结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<特征概念值域> 值域;
    bool 成功() const noexcept;
};

enum class 特征概念查找状态 : std::uint8_t {
    已找到 = 1,
    未找到 = 2,
    入口拒绝 = 3,
    材料类型不相容 = 4,
    单位不相容 = 5,
    匹配冲突 = 6,
    结构不一致 = 7,
    资源失败 = 8,
    前提已变化 = 9
};

struct 特征概念按值查找结果 final {
    特征概念查找状态 状态 = 特征概念查找状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
    std::vector<稳定编码> 冲突候选;
};

struct 先天特征概念集合 final {
    稳定编码 毫米单位;
    稳定编码 三维空间坐标;
    稳定编码 三维空间尺寸;
    稳定编码 RGB颜色;
    稳定编码 二维轮廓;
    稳定编码 三维体素;
    稳定编码 服务任务集合;
    稳定编码 服务对象需求集合;
    稳定编码 交互对象集合;
    稳定编码 需求;
    bool 完整() const noexcept;
    friend bool operator==(const 先天特征概念集合&,
        const 先天特征概念集合&) = default;
};

// 全局基础数据集中的特征概念树根节点。
extern 稳定编码 特征概念树;

// 特征层只读使用存在身份与概念归属，不取得存在事实写权。
class 特征存在引用读取接口 {
public:
    virtual ~特征存在引用读取接口() = default;
    virtual bool 是存在概念节点(稳定编码 概念节点) const noexcept = 0;
    virtual bool 存在节点属于概念(
        稳定编码 存在节点, 稳定编码 概念节点) const noexcept = 0;
    virtual bool 存在概念包含(
        稳定编码 上位概念, 稳定编码 下位概念) const noexcept = 0;
    virtual bool 存在概念相交(
        稳定编码 左概念, 稳定编码 右概念) const noexcept = 0;
};

struct 特征概念定义保护信息 final {
    特征概念信息 定义;
    稳定编码 类型根;
};
struct 特征概念定义保护结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::optional<特征概念定义保护信息> 信息;
    std::vector<基础联合节点快照> 预期节点组;
    bool 前提已变化 = false;
};
// 正式类别根不是有材料和值域的FT，不进入FT单父链或实例引用计数。
struct 特征类别根信息 final {
    稳定编码 节点;
    std::vector<稳定编码> 名称关系;
};
struct 特征类别根读取结果 final {
    正式概念读取状态 状态 = 正式概念读取状态::非本提供者概念;
    std::optional<特征类别根信息> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};
struct 特征分类关系信息 final {
    稳定编码 概念节点;
    bool 是类别根 = false;
    std::vector<稳定编码> 直接上位概念组, 直接下位概念组;
};
struct 特征分类关系保护结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<特征分类关系信息> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};
class 概念_特征类 final : public 正式概念身份读取接口 {
public:
    稳定编码 概念集合成员保护字段() const noexcept;
    特征概念规则状态 读取概念集合成员(const 特征概念准确值&, const 新_特征值类&,
        std::vector<稳定编码>&) const noexcept;
    特征概念规则状态 确保概念集合材料保护(稳定编码 类型根, 稳定编码 材料, const 新_特征值类&) noexcept;
    特征概念编码操作结果 建立或取得特征类别根概念() noexcept;
    特征类别根读取结果 读取特征类别根概念保护() const noexcept;
    特征分类关系保护结果 读取特征分类关系保护(稳定编码) const noexcept;
    正式概念身份读取结果 读取正式概念身份保护(稳定编码) const noexcept override;
    先天特征类型槽结果 建立或取得先天特征类型槽(const 先天特征类型槽请求&, const 新_特征值类&,
        std::span<const 正式概念身份读取接口* const>) noexcept;
    先天特征类型槽结果 读取先天特征类型槽(std::uint32_t 语义序号) const noexcept;
    // 当前定位不展开FT定义，不收集或复核旧保护全集。
    先天特征类型槽结果 读取当前先天特征类型槽(std::uint32_t 语义序号) const noexcept;
    特征概念编码操作结果 建立或取得特征概念强类型(
        const 特征概念定义&, const 新_特征值类&,
        std::optional<稳定编码> = std::nullopt) noexcept;
    特征概念规则状态 更新特征概念值域强类型(
        稳定编码, const 特征概念值域&, const 新_特征值类&) noexcept;
    特征概念查询操作结果 查询特征概念强类型(const 特征概念定义&) const noexcept;
    存在集合保护准备结果 准备存在集合材料保护(
        稳定编码, 稳定编码, const 新_特征值类&,
        const std::vector<基础联合节点快照>&) noexcept;
    存在集合依据推进结果 消费存在集合保护依据(存在集合保护准备凭据&&) const noexcept;
    特征概念定义保护结果 读取特征概念定义保护(稳定编码) const noexcept;
    // 解析器由上层装配，必须比本对象存活更久；材料特征不依赖该接口。
    void 装配存在引用读取接口(
        const 特征存在引用读取接口& 读取接口) noexcept;

    // 借用正式任务owner；其读取不得取得任务锁或进入需求/特征服务锁。
    // 上层在线程全部停止后解除，且接口提供者必须存活至解除完成。
    void 装配任务成员读取接口(const 特征任务成员读取接口*) noexcept;
    void 装配需求成员读取接口(const 特征需求成员读取接口*) noexcept;
    void 装配存在成员读取接口(const 特征存在成员读取接口*) noexcept;
    void 装配消息成员读取接口(const 特征消息成员读取接口*) noexcept;
    稳定编码 消息集合成员保护字段() const noexcept;
    新消息集合保持读取结果 读取消息集合保持材料(
        稳定编码, const 新_特征值类&) const noexcept;
    基础节点组追加请求 构造消息集合成员保持(
        新消息集合保持材料&&, const 新消息待发表凭据&) const;
    特征概念规则状态 读取消息成员身份(稳定编码,
        std::vector<基础联合节点快照>* 身份保护 = nullptr) const noexcept;
    特征概念规则状态 读取消息集合成员(
        const 特征概念准确值&, const 新_特征值类&, std::vector<稳定编码>&,
        std::vector<基础联合节点快照>* 身份保护 = nullptr) const noexcept;
    特征存在集合条件比较结果 比较存在集合值域(
        稳定编码, const 特征概念准确值&, const 特征概念存在集合值域&,
        const 新_特征值类&) const noexcept;
    稳定编码 存在集合成员保护字段() const noexcept;
    稳定编码 需求引用保护字段() const noexcept;
    稳定编码 需求域引用保护字段() const noexcept;
    特征概念规则状态 确保存在集合材料保护(
        稳定编码, 稳定编码, const 新_特征值类&) noexcept;
    特征概念规则状态 核验存在集合材料保护(
        稳定编码, 稳定编码, const 新_特征值类&) const noexcept;

    特征需求集合条件比较结果 比较需求集合值域(
        稳定编码 类型根, const 特征概念准确值& 当前,
        const 特征概念需求集合值域& 目标,
        const 新_特征值类&) const noexcept;

    特征任务集合条件比较结果 比较任务集合值域(
        const 特征概念准确值& 当前值,
        const 特征概念任务集合值域& 目标,
        const 新_特征值类& 特征值服务) const noexcept;

    bool 初始化(const 新_特征值类& 特征值服务) noexcept;

    std::optional<先天特征概念集合> 获取先天特征概念() const noexcept;

    // 未给上位概念时建立新的根链特征类型概念；根链节点自身就是类型身份，
    // 不按相同材料和值域合并。给出上位概念时，在其直接子链复用或建立
    // 严格子值域概念。
    稳定编码 建立或取得特征概念(
        const 特征概念定义& 定义,
        const 新_特征值类& 特征值服务,
        std::optional<稳定编码> 上位概念 = std::nullopt) noexcept;

    std::optional<特征概念信息> 获取特征概念(
        稳定编码 特征概念节点) const noexcept;

    // 直接修改既有概念的值域；实例只引用概念并独立保存自身历史聚合值域，
    // 不保存或同步第二份概念准入值域。
    bool 更新特征概念值域(
        稳定编码 特征概念节点,
        const 特征概念值域& 新值域,
        const 新_特征值类& 特征值服务) noexcept;

    // 只维护实例特征对概念的直接引用数量；概念父子关系不计数。
    // 减少后若概念已零引用且无子概念，则删除该概念节点。
    bool 增加当前实例引用(稳定编码 特征概念节点) noexcept;
    // 调用前必须已经解除对应实例的正式特征概念字段引用。
    bool 减少当前实例引用(稳定编码 特征概念节点) noexcept;

    std::vector<稳定编码> 查询特征概念(
        const 特征概念定义& 定义) const noexcept;

    // 完整核验直接子组织后按规范化定义匹配，名称不参与身份；
    // 已完成可以为空组，非完成不返回部分命中，工程异常向调用方送出。
    特征概念直接子查询结果 查询直接子特征概念(
        稳定编码 直接父概念, const 特征概念定义& 完整定义) const;

    // 从根链中的特征类型概念开始，沿外部父子关系查找包含该值的最具体概念。
    特征概念按值查找结果 查找值对应的最具体概念(
        稳定编码 特征类型概念,
        const 特征概念准确值& 特征值,
        std::optional<稳定编码> 单位,
        const 新_特征值类& 特征值服务) const noexcept;

    bool 是特征概念节点(稳定编码 节点) const noexcept;
    // 三态：是/否特征概念；结构或字段读取未完成返回空（不与“不是”混同）。
    std::optional<bool> 判定特征概念节点(稳定编码 节点) const noexcept;

    特征概念比较结果 执行比较规则(
        特征概念材料物理类型 材料类型,
        特征概念比较规则 规则,
        const 特征概念准确值& 左值,
        const 特征概念准确值& 右值,
        const 新_特征值类& 特征值服务) const noexcept;

    // 只比较同一材料类型下的值域，不把值域关系换算为相似度。
    特征概念值域比较结果 比较值域(
        稳定编码 特征类型根, const 特征概念值域&, const 特征概念值域&) const noexcept;
    特征概念值域比较结果 比较值域(
        特征概念材料物理类型 材料类型,
        const 特征概念值域& 左值域,
        const 特征概念值域& 右值域) const noexcept;

    // 只处理一个根链特征类型内部的概念层级，不处理多特征类型组合概念。
    单特征概念关系结果 比较单特征概念(
        稳定编码 左概念节点,
        稳定编码 右概念节点) const noexcept;

    // 只计算规范化值域，不在本函数中建立新概念节点。
    特征概念聚合结果 执行聚合规则(
        稳定编码 特征类型根, const std::vector<特征概念准确值>&,
        const 新_特征值类&) const noexcept;
    特征概念聚合结果 执行聚合规则(
        特征概念材料物理类型 材料类型,
        特征概念聚合规则 规则,
        const std::vector<特征概念准确值>& 历史不同值,
        const 新_特征值类& 特征值服务) const noexcept;

private:
    friend class 新_特征类;
    // 只供信息引用型空F出生：本owner计数与调用者封存的空实例共同发布。
    基础片段组合结果 联合发布新增空实例引用(
        稳定编码 特征概念,
        const 基础封存追加片段& 空实例片段) noexcept;

    稳定编码 建立特征概念内部(const 特征概念定义&, const 新_特征值类&,
        std::optional<稳定编码>);
    bool 更新特征概念值域内部(稳定编码, const 特征概念值域&, const 新_特征值类&);
    std::vector<稳定编码> 查询特征概念内部(const 特征概念定义&) const;
    friend class 概念_名称关系类;
    基础片段封存结果 封存建立名称参与(
        const 基础节点组追加请求&, std::size_t 名称槽, 稳定编码 概念) const;
    基础片段封存结果 封存删除名称参与(稳定编码 概念, 稳定编码 名称) const;


    const 特征存在引用读取接口* 存在引用读取接口_ = nullptr;
    const 特征任务成员读取接口* 任务成员读取接口_ = nullptr;
    const 特征需求成员读取接口* 需求成员读取接口_ = nullptr;
    const 特征存在成员读取接口* 存在成员读取接口_ = nullptr;
    const 特征消息成员读取接口* 消息成员读取接口_ = nullptr;
    // 初始化时借用，不拥有；服务须覆盖本对象的初始化后使用期。
    // 仅供无材料参数的值域比较入口读取有限材料，不另建材料owner。
    const 新_特征值类* 特征值读取服务_ = nullptr;
};

} // namespace 海中鱼巣
