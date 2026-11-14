#pragma once

#include <cstdint>
#include <array>
#include <exception>
#include <optional>
#include <vector>
#include <variant>
#include <span>
#include <functional>
#include <set>
#include <utility>

#include "../核心/基础数据集.h"
#include "信息内容.数据.h"
#include "概念_特征类.h"

namespace 海中鱼巣 {

class 概念_名称关系类;

enum class 正式结构操作状态 : std::uint8_t {
    已建立=1, 已复用, 已读取, 未找到, 输入拒绝, 依赖未就绪, 前提已变化, 定义冲突
};
struct 结构概念内容项 final {
    std::uint32_t 序号;
    稳定编码 特征类型, 内容概念;
    std::vector<稳定编码> 读取路径;
    friend bool operator==(const 结构概念内容项&, const 结构概念内容项&) = default;
};
struct 成员组织概念定义 final {
    稳定编码 成员概念;
    std::uint32_t 最少成员=0;
    std::optional<std::uint32_t> 最多成员;
    bool 有序=false, 全部成立=false;
    friend bool operator==(const 成员组织概念定义&, const 成员组织概念定义&) = default;
};
enum class 基础概念原语 : std::uint8_t {
    合法存在=1,合法FT根,合法特征概念,合法比较关系,正式信息身份,正式概念身份,
    I64闭区间,I64有限枚举,具名领域身份,唯一存在
};
struct 基础范围概念定义 final {
    基础概念原语 原语;
    std::vector<std::int64_t> 整数参数;
    std::vector<稳定编码> 引用参数;
    friend bool operator==(const 基础范围概念定义&, const 基础范围概念定义&) = default;
};
enum class 结构约束原语 : std::uint8_t {
    禁止项目=1,域属于FT,比较适用于目标,数值不大于,连续有序,同所属父链无环,
    固定配对基数,允许未知当前值,需求父链合法,同一对象恰一结构分支
};
struct 结构约束定义 final {
    结构约束原语 原语;
    std::vector<稳定编码> 项目FT组;
    std::vector<std::int64_t> 整数参数;
    std::vector<稳定编码> 引用参数;
    friend bool operator==(const 结构约束定义&, const 结构约束定义&) = default;
};
struct 信息结构概念定义 final {
    std::vector<稳定编码> 上位结构概念组;
    std::vector<结构概念内容项> 新增项组;
    std::vector<std::pair<稳定编码,稳定编码>> 收窄组;
    std::vector<稳定编码> 约束概念组;
    friend bool operator==(const 信息结构概念定义&, const 信息结构概念定义&) = default;
};
using 正式结构定义=std::variant<基础范围概念定义,结构约束定义,成员组织概念定义,信息结构概念定义>;
struct 正式结构概念信息 final {
    稳定编码 节点;
    正式结构定义 定义;
    std::vector<稳定编码> 名称关系;
    friend bool operator==(const 正式结构概念信息&, const 正式结构概念信息&) = default;
};
struct 正式结构概念结果 final {
    正式结构操作状态 状态=正式结构操作状态::输入拒绝;
    std::optional<正式结构概念信息> 信息;
};
struct 正式结构概念保护结果 final {
    正式结构操作状态 状态=正式结构操作状态::输入拒绝;
    std::optional<正式结构概念信息> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};
enum class 正式结构有效内容读取状态 : std::uint8_t {
    已读取=1, 输入不合法, 未找到, 非信息结构,
    依赖未就绪, 前提已变化, 资源失败
};
struct 正式结构有效内容项 final {
    稳定编码 特征类型, 内容概念;
    std::vector<稳定编码> 读取路径;
    friend bool operator==(const 正式结构有效内容项&,
        const 正式结构有效内容项&) = default;
};
struct 正式结构有效约束 final {
    稳定编码 约束概念节点;
    结构约束定义 定义;
    std::vector<稳定编码> 声明结构概念组;
    friend bool operator==(const 正式结构有效约束&,
        const 正式结构有效约束&) = default;
};
struct 正式结构有效内容 final {
    稳定编码 概念节点;
    std::vector<正式结构有效内容项> 内容项组;
    std::vector<正式结构有效约束> 约束组;
    friend bool operator==(const 正式结构有效内容&,
        const 正式结构有效内容&) = default;
};
struct 正式结构有效内容读取结果 final {
    正式结构有效内容读取状态 状态=正式结构有效内容读取状态::输入不合法;
    std::optional<正式结构有效内容> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};
struct 引用内容筹办概念组 final {
    稳定编码 完整被认知需求, 自我引用内容值需求, 自我引用内容单项值需求, 恰一分支约束;
};
struct 引用内容筹办概念结果 final {
    正式结构操作状态 状态=正式结构操作状态::输入拒绝;
    std::optional<引用内容筹办概念组> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};
enum class 正式内容符合状态 : std::uint8_t {
    符合=1, 不符合, 当前未知, 不适用, 未支持, 前提已变化, 资源失败
};
struct 正式内容符合结果 final {
    正式内容符合状态 状态=正式内容符合状态::未支持;
    std::vector<基础联合节点快照> 完整保护组;
};
struct 正式结构项目判定 final {
    稳定编码 特征类型, 要求概念;
    正式内容符合状态 状态{};
    friend bool operator==(const 正式结构项目判定&,const 正式结构项目判定&)=default;
};
struct 正式结构分支判定 final {
    稳定编码 约束概念;
    std::optional<稳定编码> 所选分支;
    正式内容符合状态 状态{};
    friend bool operator==(const 正式结构分支判定&,const 正式结构分支判定&)=default;
};
struct 正式结构内容判定结果 final {
    正式内容符合状态 状态=正式内容符合状态::未支持;
    std::vector<正式结构项目判定> 项目组;
    std::vector<正式结构分支判定> 分支组;
    std::vector<基础联合节点快照> 完整保护组;
};
// R102（SET-DOMAIN-MEMBER-CONCEPT §4.1）：调用方已解析的读取路径值，按（实例, 完整路径）精确定位。
enum class 正式路径值状态 : std::uint8_t { 已读取 = 1, 无值, 未知, 不适用, 未支持 };
struct 正式已解析路径值 final {
    稳定编码 实例;
    std::vector<稳定编码> 完整路径;
    正式路径值状态 状态 = 正式路径值状态::未支持;
    std::optional<正式内容值> 值;
    std::vector<基础联合节点快照> 保护;
};
// R102（§4.2）：引用型内容值遇结构型范围概念时，交对应领域继续判定。
using 正式结构范围判定器=std::function<特征成员概念归属结果(const 正式内容值&,稳定编码)>;
// 嵌套判定防护：同线程重复（对象,概念）或深度超过8时不允许继续，调用方返回未知。
class 正式嵌套判定防护 final {
public:
    正式嵌套判定防护(稳定编码 对象, 稳定编码 概念) noexcept {
        try {
            auto& 集 = 访问集();
            if (深度() >= 8) return;
            键_ = {对象.值, 概念.值};
            if (!集.insert(键_).second) return;
            ++深度(); 允许_ = true;
        } catch (...) { 允许_ = false; }
    }
    ~正式嵌套判定防护() { if (允许_) { 访问集().erase(键_); --深度(); } }
    正式嵌套判定防护(const 正式嵌套判定防护&) = delete;
    正式嵌套判定防护& operator=(const 正式嵌套判定防护&) = delete;
    bool 允许() const noexcept { return 允许_; }
private:
    static std::set<std::pair<std::uint64_t, std::uint64_t>>& 访问集() noexcept {
        thread_local std::set<std::pair<std::uint64_t, std::uint64_t>> 集; return 集;
    }
    static int& 深度() noexcept { thread_local int 值 = 0; return 值; }
    std::pair<std::uint64_t, std::uint64_t> 键_{};
    bool 允许_ = false;
};
using 正式直接内容读取器=std::function<正式内容值(稳定编码,稳定编码)>;
using 正式引用身份核验器=std::function<正式内容符合结果(
    基础概念原语,const std::vector<std::int64_t>&,const std::vector<稳定编码>&,const 正式内容值&)>;
using 正式关联约束核验器=std::function<正式内容符合结果(稳定编码,const 结构约束定义&)>;
enum class 结构项概念保护状态 : std::uint8_t { 已读取=1,输入拒绝,依赖未就绪,未取得正式概念证明,前提已变化 };
struct 结构项概念保护结果 final {
    结构项概念保护状态 状态=结构项概念保护状态::输入拒绝;
    std::optional<稳定编码> 特征类型根,内容概念;
    std::vector<基础联合节点快照> 完整保护组;
};
struct 先天信息概念组 final {
    稳定编码 需求,任务,方法;
    std::vector<std::pair<std::uint32_t,稳定编码>> 子类与支持定义;
    friend bool operator==(const 先天信息概念组&, const 先天信息概念组&) = default;
};
struct 先天信息概念结果 final {
    正式结构操作状态 状态=正式结构操作状态::输入拒绝;
    std::optional<先天信息概念组> 信息;
};
struct 先天信息类型依赖 final {
    稳定编码 任务状态FT,任务服务值FT,方法启用FT;
    稳定编码 方法运行次数FT,方法成功次数FT,方法可靠度FT;
    // TASK-STATE-CAUSAL 切片2：任务虚拟存在的I64因素特征类型根（承接依据、候选结论、条件结论、
    // 执行进入结论、影响评估结论、后继决议），镜像为结构301条目与基础枚举槽421..426。
    std::array<稳定编码,6> 任务因素FT{};
};

enum class 专属存在概念身份读取状态 : std::uint8_t {
    已完成 = 1, 输入拒绝, 提供者未就绪, 节点不存在,
    前提已变化, 结构不一致, 资源失败
};
struct 专属存在概念身份保护结果 final {
    专属存在概念身份读取状态 状态 = 专属存在概念身份读取状态::输入拒绝;
    std::optional<稳定编码> 专属概念节点, 对应存在节点;
    std::vector<基础联合节点快照> 身份保护;
};

enum class 存在概念种类 : std::uint8_t {
    专属 = 1,
    抽象 = 2
};

struct 存在概念特征项 final {
    稳定编码 特征类型概念;
    稳定编码 具体特征概念;
    friend bool operator==(const 存在概念特征项&,
        const 存在概念特征项&) = default;
};

struct 存在概念信息 final {
    稳定编码 节点;
    存在概念种类 种类 = 存在概念种类::专属;
    std::optional<稳定编码> 对应存在节点;
    std::vector<存在概念特征项> 特征组;
    std::vector<稳定编码> 名称关系;
    std::vector<稳定编码> 上位概念组;
    std::vector<稳定编码> 下位概念组;
    friend bool operator==(const 存在概念信息&,
        const 存在概念信息&) = default;
};

struct 普通存在概念定义保护结果 final {
    正式概念读取状态 状态=正式概念读取状态::非本提供者概念;
    std::optional<存在概念信息> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};

// 范围语义不包含名称、无关下位或使用本概念的入向引用。
struct 存在范围语义信息 final {
    稳定编码 节点;
    存在概念种类 种类;
    std::optional<稳定编码> 对应存在节点;
    std::vector<存在概念特征项> 特征组;
    std::vector<稳定编码> 上位概念组;
    friend bool operator==(const 存在范围语义信息&, const 存在范围语义信息&) = default;
};
struct 存在范围语义保护结果 final {
    正式概念读取状态 状态=正式概念读取状态::非本提供者概念;
    std::optional<存在范围语义信息> 信息;
    基础只读预期 只读预期;
};

enum class 存在概念操作状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    已更新 = 3,
    无变化 = 4,
    无共同点 = 5,
    入口拒绝 = 6,
    来源不足 = 7,
    概念不存在 = 8,
    特征概念不一致 = 9,
    层级不成立 = 10,
    会形成环 = 11,
    结构不一致 = 12,
    资源失败 = 13
};

struct 存在概念建立结果 final {
    存在概念操作状态 状态 = 存在概念操作状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
};

struct 存在根建立请求 final {
    std::optional<稳定编码> 既有根候选;
    std::vector<稳定编码> 必要直接下位普通概念组;
};

enum class 存在分类概念形状 : std::uint8_t {
    普通专属=1, 普通抽象, 信息结构
};
struct 存在分类关系信息 final {
    稳定编码 概念节点;
    存在分类概念形状 形状;
    std::vector<稳定编码> 直接上位概念组, 直接下位概念组;
};
struct 存在分类关系保护结果 final {
    正式结构操作状态 状态=正式结构操作状态::输入拒绝;
    std::optional<存在分类关系信息> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};

struct 存在概念抽象结果 final {
    存在概念操作状态 状态 = 存在概念操作状态::入口拒绝;
    std::optional<稳定编码> 抽象概念节点;
    std::vector<存在概念特征项> 共同特征组;
};

enum class 存在概念实例候选读取状态 : std::uint8_t {
    已完成 = 1, 输入拒绝, 概念不可读, 读取已变化, 结构不一致, 资源失败
};
struct 存在概念专属实例候选 final {
    稳定编码 专属概念节点, 对应存在节点;
};
struct 存在概念实例候选读取结果 final {
    存在概念实例候选读取状态 状态 = 存在概念实例候选读取状态::输入拒绝;
    std::optional<稳定编码> 起始概念节点;
    std::vector<存在概念专属实例候选> 候选组;
};

enum class 指代目标范围 : std::uint8_t { 单个已登记强类型信息节点 = 1 };
enum class 指代参数角色 : std::uint8_t {
    目标 = 1, 作用域 = 2, 参照中心 = 3, 参照维度规则 = 4, 主体类型限制 = 5
};
enum class 指代参数类型 : std::uint8_t {
    强类型信息节点 = 1, 身份变量 = 2, 交互作用域 = 3,
    参照信息节点 = 4, 正式参照规则 = 5, 正式主体类型限制 = 6
};
struct 指代参数定义 final {
    指代参数角色 角色;
    指代参数类型 类型;
    friend bool operator==(const 指代参数定义&, const 指代参数定义&) = default;
};
enum class 指代声明规则 : std::uint8_t {
    作用域说话者 = 1, 作用域受话者 = 2, 作用域第三方 = 3,
    相对近指 = 4, 相对远指 = 5, 受限身份变量 = 6
};
struct 指代I64特征约束 final {
    指代参数角色 被约束角色;
    稳定编码 特征类型根;
    std::vector<特征概念I64闭区间> 允许值域;
    特征概念定义 特征类型快照;
    friend bool operator==(const 指代I64特征约束&,
        const 指代I64特征约束&) = default;
};
struct 指代概念定义 final {
    指代目标范围 目标范围 = 指代目标范围::单个已登记强类型信息节点;
    std::vector<指代参数定义> 参数组;
    指代声明规则 规则;
    std::vector<指代I64特征约束> 特征约束组;
    friend bool operator==(const 指代概念定义&,
        const 指代概念定义&) = default;
};
struct 指代概念信息 final {
    稳定编码 节点;
    指代概念定义 定义;
    std::vector<稳定编码> 名称关系;
    friend bool operator==(const 指代概念信息&,
        const 指代概念信息&) = default;
};
enum class 指代定义结构状态 : std::uint8_t {
    已就绪 = 1, 入口拒绝, 内部不一致, 资源失败
};
enum class 指代概念发布阶段 : std::uint8_t {
    未发布 = 1, 已发布待核验, 已完整读回
};
enum class 指代概念写入状态 : std::uint8_t {
    已建立 = 1, 已复用, 输入拒绝, 依赖缺失,
    依赖已变化, 结构不一致, 资源失败, 读取未完成, 读取漂移
};
struct 指代概念写入结果 final {
    指代概念写入状态 状态 = 指代概念写入状态::输入拒绝;
    指代概念发布阶段 阶段 = 指代概念发布阶段::未发布;
    std::optional<稳定编码> 已发生身份;
    std::optional<指代概念信息> 信息;
    std::exception_ptr 发布后异常;
};
enum class 指代概念查询状态 : std::uint8_t {
    已找到 = 1, 未找到, 输入拒绝, 依赖缺失,
    依赖已变化, 结构不一致, 资源失败, 读取未完成, 读取漂移
};
struct 指代概念查询结果 final {
    指代概念查询状态 状态 = 指代概念查询状态::输入拒绝;
    std::optional<指代概念信息> 信息;
};
struct 指代概念保护结果 final {
    指代概念查询状态 状态 = 指代概念查询状态::输入拒绝;
    std::optional<指代概念信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};



// 全局基础数据集中的物理组织根。它不是存在概念，不参与抽象和匹配。
extern 稳定编码 存在概念图根节点;

struct 存在概念上位保护信息 final {
    稳定编码 概念节点;
    std::vector<稳定编码> 上位概念组;
};
struct 存在概念上位保护结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::optional<存在概念上位保护信息> 信息;
    std::vector<基础联合节点快照> 预期节点组;
    bool 前提已变化 = false;
};
class 概念_存在类 final : public 正式概念身份读取接口 {
public:
    // owner登记的语义根不同于物理组织根；名称仅供语言引用。
    存在概念建立结果 建立或取得存在根概念(const 存在根建立请求&) noexcept;
    普通存在概念定义保护结果 读取存在根概念保护() const noexcept;
    // 共同分类视图不接管D/T/M实例内容或写入权限。
    存在分类关系保护结果 读取存在分类关系保护(稳定编码) const noexcept;
    正式概念身份读取结果 读取正式概念身份保护(稳定编码) const noexcept override;
    普通存在概念定义保护结果 读取普通存在概念定义保护(稳定编码) const noexcept;
    存在范围语义保护结果 读取存在范围语义保护(稳定编码) const noexcept;
    结构项概念保护结果 读取结构项概念保护(稳定编码 特征类型, 稳定编码 内容概念,
        std::span<const 正式概念身份读取接口* const> 提供者组) const noexcept;
    正式结构概念结果 读取正式结构概念(稳定编码) const noexcept;
    正式结构概念保护结果 读取正式结构概念保护(稳定编码) const noexcept;
    // 只读定义展开、按值返回；不执行实例读取路径或实例符合性判定。
    正式结构有效内容读取结果 读取正式结构有效内容保护(稳定编码 概念节点) const noexcept;
    引用内容筹办概念结果 建立或取得引用内容筹办概念(const 新_特征值类&,
        std::span<const 正式概念身份读取接口* const>) noexcept;
    引用内容筹办概念结果 读取引用内容筹办概念() const noexcept;
    // 方案D（2026-11-12用户裁决）：先天“补问需求”（槽233，上位为自我引用内容单项值需求232）。
    // 由建立或取得引用内容筹办概念同次确保；具体需求是否属于它由需求owner按事实判定，本owner只记录定义。
    正式结构概念保护结果 读取补问需求概念保护() const noexcept;
    正式内容符合结果 比较正式内容范围(稳定编码,const 正式内容值&,
        const 正式引用身份核验器&,const 正式结构范围判定器& 结构判定={}) const noexcept;
    // R102：信息结构概念 下 是否沿上位结构概念组到达 上（相等也算）；读取失败或前提变化返回空。
    std::optional<bool> 正式结构概念属于(稳定编码 下, 稳定编码 上) const noexcept;
    // 路径值与结构判定器默认空，旧调用行为不变（SET-DOMAIN-MEMBER-CONCEPT §4.1、§4.2）。
    正式结构内容判定结果 判定正式结构内容(稳定编码,稳定编码,
        const 正式直接内容读取器&,const 正式引用身份核验器&,
        const 正式关联约束核验器&,std::span<const 正式已解析路径值> 路径值={},
        const 正式结构范围判定器& 结构判定={}) const noexcept;
    正式结构概念结果 建立或取得已确认消息连接概念() noexcept;
    正式结构概念保护结果 读取已确认消息连接概念保护() const noexcept;
    正式结构概念结果 建立或取得词典材料身份范围概念() noexcept;
    正式结构概念结果 读取词典材料身份范围概念() const noexcept;
    正式结构概念结果 建立或取得词条身份范围概念() noexcept;
    正式结构概念保护结果 读取词条身份范围概念保护() const noexcept;
    先天信息概念结果 建立或取得先天需求任务方法概念(const 先天信息类型依赖&,
        const 新_特征值类&, std::span<const 正式概念身份读取接口* const>) noexcept;
    先天信息概念结果 建立或取得自我需求子概念(稳定编码 唯一自我存在,
        const 特征存在成员读取接口& 实际存在读取,
        std::span<const 正式概念身份读取接口* const>) noexcept;
    先天信息概念结果 读取先天需求任务方法概念() const noexcept;
    专属存在概念身份保护结果 读取专属存在概念身份保护(
        稳定编码 专属概念节点, 稳定编码 预期存在节点,
        const 基础联合节点快照& 已读存在快照) const noexcept;
    存在概念上位保护结果 读取存在概念上位保护(稳定编码) const noexcept;
    explicit 概念_存在类(概念_特征类& 特征概念服务) noexcept;

    bool 初始化() noexcept;

    指代定义结构状态 初始化指代定义结构();
    指代概念写入结果 建立或取得指代概念(const 指代概念定义& 请求);
    指代概念查询结果 查询指代概念(const 指代概念定义& 请求) const;
    指代概念查询结果 获取指代概念(稳定编码 概念节点) const;
    指代概念保护结果 读取指代概念保护(稳定编码 概念节点) const;

    // 每个实际存在总是建立一个新的专属概念，不按特征相同复用。
    // 专属概念允许暂时没有特征项；空专属概念只作为该存在的概念
    // 承载节点，不参与匹配，也不能形成空抽象概念。
    存在概念建立结果 建立专属存在概念(
        稳定编码 实际存在节点,
        const std::vector<稳定编码>& 具体特征概念组) noexcept;

    // 只允许更新专属概念的完整特征投影；不自动产生抽象概念。
    存在概念操作状态 更新专属存在概念(
        稳定编码 专属概念节点,
        const std::vector<稳定编码>& 具体特征概念组) noexcept;

    // 仅供存在建立失败回滚：概念尚未进入上下位图、且存在节点尚未写入
    // 专属概念字段时才允许撤销。
    bool 撤销未绑定专属存在概念(
        稳定编码 专属概念节点,
        稳定编码 实际存在节点) noexcept;

    // 只读核验初建单项专属概念及其私有特征项；不符时工程终止。
    // 仅供同次私有创建/明确未采用回收，不提供任意概念删除权限。
    void 核验初建单特征专属概念可回收(
        稳定编码 专属概念节点, 稳定编码 对应存在节点,
        const 存在概念特征项& 预期特征项,
        const std::vector<稳定编码>& 允许外部入向字段关系组) const noexcept;

    // 至少两个不同来源概念才可执行。默认提取全部来源的最大共同特征集；
    // 没有共同点时正常返回“无共同点”，不会建立空抽象概念。
    存在概念抽象结果 抽象共同存在概念(
        const std::vector<稳定编码>& 来源概念组) noexcept;

    // 为定义级分类建立新的抽象概念。调用者负责用名称关系赋予其语义身份；
    // 本入口至少要求一个正式下位概念，不参与“两个以上实例求共同点”的
    // 自动抽象，也不会按空特征组复用其它人工分类概念。
    存在概念建立结果 建立人工抽象存在概念(
        const std::vector<稳定编码>& 直接下位概念组) noexcept;

    // 指定实例归入（2026-11-10用户裁决）：已明确告知的实例优先级最高，先把其概念登记为
    // 上位抽象概念的直接下位；直接下位达到两个以上后，以全部直接下位的共同特征
    // 补全或优化上位概念的特征（可为空，随实例增加逐步完善）。
    // 返回已更新（新建归入或特征变化）、无变化、会形成环、概念不存在、入口拒绝（上位非抽象）。
    存在概念操作状态 归入上位存在概念(
        稳定编码 下位概念, 稳定编码 上位抽象概念) noexcept;

    // 先天类别概念（0050 §1.4）：以必需特征直接定义，挂在存在根下，不要求先有实例。
    // 调用者负责名称关系与按名称复用；本入口每次建立一个新的抽象概念。
    存在概念建立结果 建立先天类别存在概念(
        const std::vector<稳定编码>& 具体特征概念组) noexcept;

    std::optional<存在概念信息> 获取存在概念(
        稳定编码 概念节点) const noexcept;

    bool 是存在概念节点(稳定编码 节点) const noexcept;
    bool 是专属存在概念节点(稳定编码 节点) const noexcept;
    bool 是抽象存在概念节点(稳定编码 节点) const noexcept;

    std::vector<稳定编码> 查询上位存在概念(
        稳定编码 概念节点) const noexcept;
    std::vector<稳定编码> 查询下位存在概念(
        稳定编码 概念节点) const noexcept;
    // 正式下位链接的完整候选，不替代场景/存在owner对实例资格的复核。
    存在概念实例候选读取结果 读取下位专属实例候选完整(稳定编码 概念节点) const noexcept;

private:
    friend class 概念_名称关系类;
    基础片段封存结果 封存建立名称参与(
        const 基础节点组追加请求&, std::size_t 名称槽, 稳定编码 概念) const;
    基础片段封存结果 封存删除名称参与(稳定编码 概念, 稳定编码 名称) const;
    正式结构操作状态 发布先天定义批(const 基础节点组追加请求&,
        const std::vector<std::pair<std::uint32_t,std::size_t>>&) noexcept;


    概念_特征类& 特征概念服务_;
};

} // namespace 海中鱼巣
