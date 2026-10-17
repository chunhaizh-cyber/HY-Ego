#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <shared_mutex>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace 海中鱼巣 {

struct 稳定编码 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 稳定编码&, const 稳定编码&) = default;
    friend bool operator<(const 稳定编码& 左, const 稳定编码& 右) noexcept {
        return 左.值 < 右.值;
    }
};

inline bool 有效(稳定编码 编码) noexcept { return 编码.值 != 0; }

// 直接值只是字段末端的原始材料，不取得独立稳定编码或业务语义。
using 基础原始值 = std::variant<std::int64_t,
    std::vector<std::int64_t>, std::vector<std::uint64_t>, std::string>;

struct 基础值 final {
    基础原始值 材料;
    friend bool operator==(const 基础值&, const 基础值&) = default;
};

struct 基础节点 final {
    稳定编码 编码;
    friend bool operator==(const 基础节点&, const 基础节点&) = default;
};

enum class 基础外部关系类型 : std::uint8_t {
    父子 = 1,
    兄弟 = 2
};

struct 基础外部关系 final {
    稳定编码 编码;
    稳定编码 源节点;
    稳定编码 目标节点;
    基础外部关系类型 类型 = 基础外部关系类型::父子;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const 基础外部关系&, const 基础外部关系&) = default;
};

// 稳定编码内容表示字段指向另一个节点；基础值内容表示字段直接保存原始材料。
using 基础字段内容 = std::variant<基础值, 稳定编码>;

struct 基础字段关系 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 字段节点;
    基础字段内容 内容;
    friend bool operator==(const 基础字段关系&, const 基础字段关系&) = default;
};

enum class 基础结构操作状态 : std::uint8_t {
    已完成 = 1, 入口拒绝, 节点不存在, 引用不存在, 资源失败, 内部不一致,
    引用仍存在
};

struct 基础节点初始字段 final {
    稳定编码 字段节点;
    基础字段内容 内容;
};

struct 基础节点建立请求 final {
    std::optional<稳定编码> 上级节点;
    std::vector<基础节点初始字段> 字段组;
};

struct 基础节点建立结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::optional<稳定编码> 节点;
};

// 组内索引仅用于本次调用，发布时转换为正式节点引用。
struct 基础组内节点引用 final {
    std::size_t 索引 = 0;
};

using 基础组内字段内容 = std::variant<基础值, 稳定编码, 基础组内节点引用>;

struct 基础组内初始字段 final {
    稳定编码 字段节点;
    基础组内字段内容 内容;
};

struct 基础组内新节点 final {
    std::vector<基础组内初始字段> 字段组;
};

struct 基础节点组建立结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::vector<稳定编码> 节点组;
};

struct 基础节点结构 final {
    基础节点 节点;
    std::vector<基础字段关系> 字段组;
};

struct 基础联合节点快照 final {
    基础节点结构 结构;
    std::vector<基础外部关系> 入向外部关系组, 出向外部关系组;
    std::vector<基础字段关系> 入向字段组;
};
struct 基础字段组合替换 final {
    稳定编码 字段关系;
    基础字段内容 新内容;
};
struct 基础组内外部关系 final {
    std::variant<稳定编码, 基础组内节点引用> 源节点, 目标节点;
    基础外部关系类型 类型 = 基础外部关系类型::父子;
    std::int64_t 角色或顺序 = 0;
};

// 追加可与具备完整联合预期的字段替换/删除、外部边共同发布。
struct 基础既有节点追加字段 final {
    稳定编码 所属节点, 字段节点;
    基础组内字段内容 内容;
};
struct 基础节点组追加请求 final {
    std::vector<基础组内新节点> 新节点组;
    std::vector<基础既有节点追加字段> 追加字段组;
    std::vector<基础节点结构> 预期既有节点组;
    std::vector<基础联合节点快照> 预期联合节点组;
    std::vector<基础字段组合替换> 替换字段组;
    std::vector<基础组内外部关系> 追加外部关系组;
    std::vector<稳定编码> 删除字段关系组;
};
enum class 基础节点组追加状态 : std::uint8_t {
    已完成 = 1, 入口拒绝, 节点不存在, 引用不存在,
    预期不符, 资源失败, 内部不一致
};
struct 基础节点组追加结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::vector<稳定编码> 新节点组;
    std::vector<稳定编码> 追加字段关系组;
    std::vector<稳定编码> 追加外部关系组;
};

// 同一数据集内的不可变技术材料；不开放owner的原始写集。
class 基础封存追加片段 final {
public:
    基础封存追加片段(const 基础封存追加片段&) noexcept = default;
    基础封存追加片段(基础封存追加片段&&) noexcept = default;
    基础封存追加片段& operator=(const 基础封存追加片段&) noexcept = default;
    基础封存追加片段& operator=(基础封存追加片段&&) noexcept = default;
private:
    struct 内容;
    explicit 基础封存追加片段(std::shared_ptr<const 内容>) noexcept;
    std::shared_ptr<const 内容> 内容_;
    friend class 基础数据集;
    friend class 基础片段发布回执;
};

struct 基础片段封存结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::optional<基础封存追加片段> 片段;
};

// 回执与封存内容身份绑定；复制只共享已发布的只读结果，不复制编码数组。
class 基础片段发布回执 final {
public:
    基础片段发布回执(const 基础片段发布回执&) noexcept = default;
    基础片段发布回执(基础片段发布回执&&) noexcept = default;
    基础片段发布回执& operator=(const 基础片段发布回执&) noexcept = default;
    基础片段发布回执& operator=(基础片段发布回执&&) noexcept = default;
    bool 对应片段(const 基础封存追加片段&) const noexcept;
    const std::vector<稳定编码>& 新节点组() const noexcept;
    const std::vector<稳定编码>& 追加字段关系组() const noexcept;
    const std::vector<稳定编码>& 追加外部关系组() const noexcept;
private:
    struct 内容;
    explicit 基础片段发布回执(std::shared_ptr<const 内容>) noexcept;
    std::shared_ptr<const 内容> 内容_;
    friend class 基础数据集;
};

struct 基础片段组合结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::vector<基础片段发布回执> 片段回执组;
};

struct 基础节点结构结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::optional<基础节点结构> 结构;
};

struct 基础引用节点结构结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::vector<基础字段关系> 引用字段组;
    std::vector<基础节点结构> 来源节点组;
};

struct 基础联合节点读取项 final {
    稳定编码 查询节点;
    std::optional<基础联合节点快照> 快照;
};
struct 基础联合节点查询结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::vector<基础联合节点读取项> 节点组;
};
struct 基础字段组合追加 final {
    稳定编码 所属节点, 字段节点;
    基础字段内容 内容;
};
struct 基础字段组合变化请求 final {
    std::vector<基础联合节点快照> 预期节点组;
    std::vector<稳定编码> 删除字段关系组;
    std::vector<基础字段组合替换> 替换字段组;
    std::vector<基础字段组合追加> 追加字段组;
};
enum class 基础字段组合变化状态 : std::uint8_t {
    已完成 = 1, 入口拒绝, 节点不存在, 引用不存在,
    预期不符, 编码耗尽, 资源失败
};
struct 基础字段组合变化结果 final {
    基础字段组合变化状态 状态 = 基础字段组合变化状态::入口拒绝;
    std::vector<稳定编码> 追加字段关系组;
};
enum class 基础互引节点操作状态 : std::uint8_t {
    已完成 = 1, 入口拒绝, 节点不存在, 引用不存在,
    资源失败, 内部不一致, 引用仍存在, 预期不符
};
struct 基础互引节点建立请求 final {
    基础节点建立请求 新节点;
    稳定编码 反向引用所属节点, 反向引用字段节点;
    std::vector<基础联合节点快照> 预期节点组;
};
struct 基础互引节点建立结果 final {
    基础互引节点操作状态 状态 = 基础互引节点操作状态::入口拒绝;
    std::optional<稳定编码> 节点, 反向字段关系, 组织关系;
};
struct 基础互引节点删除请求 final {
    稳定编码 节点, 上级节点, 反向字段关系;
    std::vector<基础联合节点快照> 预期节点组;
};

class 基础数据集 final {
public:
    基础数据集() = default;
    基础数据集(const 基础数据集&) = delete;
    基础数据集& operator=(const 基础数据集&) = delete;

    // 诊断责任：向上送出。空编码表示参数无效、引用不存在或资源失败。
    稳定编码 新建节点() noexcept;
    稳定编码 新建节点(稳定编码 上级节点) noexcept;
    bool 删除节点(稳定编码 节点) noexcept;
    bool 有入向字段引用(稳定编码 节点) const noexcept;
    bool 有入向字段引用(稳定编码 节点, 稳定编码 忽略所属节点) const noexcept;
    // 显式完整删除集一次校验、一次移除；集合外引用阻止全部拆除。
    基础结构操作状态 原子删除节点结构(
        const std::vector<稳定编码>& 节点组,
        const std::vector<稳定编码>& 字段关系组,
        const std::vector<稳定编码>& 外部关系组) noexcept;
    std::optional<基础节点> 查询节点(稳定编码 节点) const noexcept;

    // 新节点及初始结构一次发布；失败无公开半结构，已分配编码不回退。
    基础节点建立结果 原子建立节点(const 基础节点建立请求& 请求) noexcept;
    基础节点组建立结果 原子建立节点组(
        const std::vector<基础组内新节点>& 请求) noexcept;
    // 诊断责任：结构/资源状态向上送出；内部不变量异常进入工程终止链。
    基础节点组追加结果 原子追加节点组(
        const 基础节点组追加请求& 请求) noexcept;
    // 封存零事实写入；组合只调用一次既有原子追加，回执在发布前预分配。
    基础片段封存结果 封存原子追加片段(
        const 基础节点组追加请求& 请求) noexcept;
    基础片段组合结果 原子追加封存片段组(
        const std::vector<基础封存追加片段>& 片段组) noexcept;
    基础节点结构结果 查询完整节点结构(稳定编码 节点) const noexcept;
    基础引用节点结构结果 查询字段引用节点结构(
        稳定编码 目标节点, 稳定编码 字段节点) const noexcept;
    基础联合节点查询结果 查询节点组完整结构(
        const std::vector<稳定编码>& 节点组) const noexcept;
    基础字段组合变化结果 原子比较变更字段组(
        const 基础字段组合变化请求& 请求) noexcept;
    基础互引节点建立结果 原子建立互引节点(
        const 基础互引节点建立请求& 请求) noexcept;
    基础互引节点操作状态 原子删除互引节点(
        const 基础互引节点删除请求& 请求) noexcept;

    // 诊断责任：向上送出。空编码或 false 表示参数无效、引用不存在或资源失败。
    稳定编码 添加关系(稳定编码 源节点, 稳定编码 目标节点,
        基础外部关系类型 类型, std::int64_t 角色或顺序 = 0) noexcept;
    bool 修改关系(稳定编码 关系, 稳定编码 源节点, 稳定编码 目标节点,
        基础外部关系类型 类型, std::int64_t 角色或顺序 = 0) noexcept;
    bool 删除关系(稳定编码 关系) noexcept;
    std::optional<基础外部关系> 查询关系(稳定编码 关系) const noexcept;
    std::vector<基础外部关系> 查询源关系(
        稳定编码 源节点, 基础外部关系类型 类型) const;
    std::vector<基础外部关系> 查询目标关系(
        稳定编码 目标节点, 基础外部关系类型 类型) const;

    // 字段允许同一“所属节点 + 字段节点”存在多项；数量约束由上层数据类决定。
    稳定编码 添加字段值(稳定编码 节点, 稳定编码 字段节点,
        基础值 值) noexcept;
    稳定编码 添加字段节点(稳定编码 节点, 稳定编码 字段节点,
        稳定编码 目标节点) noexcept;
    bool 修改字段值(稳定编码 字段关系, 基础值 值) noexcept;
    bool 修改字段节点(稳定编码 字段关系, 稳定编码 目标节点) noexcept;
    bool 删除字段(稳定编码 字段关系) noexcept;
    std::optional<基础字段关系> 查询字段关系(稳定编码 字段关系) const noexcept;
    std::vector<基础字段关系> 查询字段(
        稳定编码 节点, 稳定编码 字段节点) const;
    // 反向查询某个字段节点中指向目标节点的全部字段关系。
    std::vector<基础字段关系> 查询目标字段(
        稳定编码 目标节点, 稳定编码 字段节点) const;

private:
    // 仅由持有本对象独占锁的原子建立入口调用，不重入公开函数。
    基础节点组建立结果 原子建立节点组已加锁(
        const std::vector<基础组内新节点>& 请求,
        std::optional<稳定编码> 上级节点);
    稳定编码 分配编码() noexcept;
    bool 节点存在(稳定编码 节点) const noexcept;
    bool 节点被引用(稳定编码 节点) const noexcept;
    // 只在本对象锁内使用；索引存编码而不是字段副本或跨发布迭代器。
    using 字段索引键 = std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>;
    void 插入字段已加锁(基础字段关系 字段);
    void 核验字段索引已加锁(const 基础字段关系& 字段) const;
    void 删除字段已加锁(const 基础字段关系& 字段) noexcept;
    void 读取节点字段已加锁(稳定编码 节点, std::vector<基础字段关系>& 结果) const;
    基础联合节点快照 读取联合快照已加锁(稳定编码 节点) const;
    基础互引节点操作状态 复核联合预期已加锁(
        const std::vector<基础联合节点快照>& 预期,
        const std::set<std::uint64_t>& 必需) const;

    mutable std::shared_mutex 互斥_;
    // 首次封存时在锁内生成，之后不变；旧句柄不能用于地址复用后的新数据集。
    std::shared_ptr<const int> 封存实例身份_;
    std::uint64_t 下一个编码_ = 1;
    std::map<std::uint64_t, 基础节点> 节点_;
    std::map<std::uint64_t, 基础外部关系> 外部关系_;
    std::map<std::uint64_t, 基础字段关系> 字段关系_;
    std::set<字段索引键> 所属字段索引_; // 所属节点、字段节点、字段关系编码
    std::set<字段索引键> 目标字段索引_; // 字段节点、引用目标、字段关系编码
};

// 新数据架构共用唯一基础数据集；各类数据树以其中的根节点区分。
extern 基础数据集 全局基础数据集;

} // namespace 海中鱼巣
