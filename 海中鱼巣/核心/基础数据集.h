#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <shared_mutex>
#include <string>
#include <variant>
#include <vector>

#include "L1公共事实.数据.h"

namespace 海中鱼巣 {

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

class 基础数据集 final {
public:
    基础数据集() = default;
    基础数据集(const 基础数据集&) = delete;
    基础数据集& operator=(const 基础数据集&) = delete;

    // 诊断责任：向上送出。空编码表示参数无效、引用不存在或资源失败。
    稳定编码 新建节点() noexcept;
    稳定编码 新建节点(稳定编码 上级节点) noexcept;
    bool 删除节点(稳定编码 节点) noexcept;
    std::optional<基础节点> 查询节点(稳定编码 节点) const noexcept;

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

private:
    稳定编码 分配编码() noexcept;
    bool 节点存在(稳定编码 节点) const noexcept;
    bool 节点被引用(稳定编码 节点) const noexcept;

    mutable std::shared_mutex 互斥_;
    std::uint64_t 下一个编码_ = 1;
    std::map<std::uint64_t, 基础节点> 节点_;
    std::map<std::uint64_t, 基础外部关系> 外部关系_;
    std::map<std::uint64_t, 基础字段关系> 字段关系_;
};

} // namespace 海中鱼巣
