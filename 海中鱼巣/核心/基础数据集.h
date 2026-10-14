#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <shared_mutex>
#include <string>
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

struct 基础节点结构结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::optional<基础节点结构> 结构;
};

struct 基础引用节点结构结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::vector<基础字段关系> 引用字段组;
    std::vector<基础节点结构> 来源节点组;
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
    基础节点结构结果 查询完整节点结构(稳定编码 节点) const noexcept;
    基础引用节点结构结果 查询字段引用节点结构(
        稳定编码 目标节点, 稳定编码 字段节点) const noexcept;

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

    mutable std::shared_mutex 互斥_;
    std::uint64_t 下一个编码_ = 1;
    std::map<std::uint64_t, 基础节点> 节点_;
    std::map<std::uint64_t, 基础外部关系> 外部关系_;
    std::map<std::uint64_t, 基础字段关系> 字段关系_;
};

// 新数据架构共用唯一基础数据集；各类数据树以其中的根节点区分。
extern 基础数据集 全局基础数据集;

} // namespace 海中鱼巣
