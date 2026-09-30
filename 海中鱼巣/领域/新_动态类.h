#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_状态类.h"
#include "概念_动态类.h"

namespace 海中鱼巣 {

class 新_场景类;

enum class 新动态种类 : std::uint8_t {
    原子 = 1,
    组合 = 2
};

struct 新动态信息 final {
    稳定编码 节点;
    新动态种类 种类 = 新动态种类::原子;
    稳定编码 主题存在节点;
    // 原子动态直接保存两个状态；组合动态从有序原子成员推导状态序列。
    std::vector<稳定编码> 状态节点组;
    std::vector<稳定编码> 原子动态节点组;
    // 只有原子动态立即形成实例动态概念；组合动态不复制第二套概念事实。
    std::optional<稳定编码> 动态概念节点;
    std::int64_t 被引用次数 = 0;
    friend bool operator==(const 新动态信息&, const 新动态信息&) = default;
};

enum class 新动态操作状态 : std::uint8_t {
    已完成 = 1,
    入口拒绝 = 2,
    状态不足 = 3,
    状态不存在 = 4,
    主题不一致 = 5,
    特征类型不一致 = 6,
    时间不合法 = 7,
    状态重复 = 8,
    动态不存在 = 9,
    实例概念失败 = 10,
    动态仍被引用 = 11,
    引用计数溢出 = 12,
    结构不一致 = 13,
    资源失败 = 14,
    概念不合法 = 15,
    状态数量不合法 = 16,
    原子动态不足 = 17,
    不是原子动态 = 18,
    端点不连续 = 19
};

struct 新动态建立结果 final {
    新动态操作状态 状态 = 新动态操作状态::入口拒绝;
    std::optional<稳定编码> 动态节点;
    std::optional<稳定编码> 动态概念节点;
};

class 新_动态类 final {
public:
    新_动态类(
        新_状态类& 状态服务,
        概念_动态类& 动态概念服务) noexcept;

    bool 初始化() noexcept;

    // 建立恰好由前、后两个状态组成的原子动态，并立即建立实例动态概念。
    新动态建立结果 建立原子动态(
        稳定编码 主题存在节点,
        稳定编码 前状态节点,
        稳定编码 后状态节点) noexcept;

    // 兼容调用形状，但不再允许任意多状态动态；状态组必须恰好为两个。
    新动态建立结果 建立动态(
        稳定编码 主题存在节点,
        const std::vector<稳定编码>& 状态节点组) noexcept;

    // 组合动态只保存有序原子动态引用。相邻成员必须共享端点，并且主题、
    // 基础特征类型一致、时间严格递增。
    新动态建立结果 连接原子动态(
        const std::vector<稳定编码>& 有序原子动态组) noexcept;

    std::optional<新动态信息> 获取动态(
        稳定编码 动态节点) const noexcept;
    bool 是动态节点(稳定编码 节点) const noexcept;

    std::vector<稳定编码> 按主题查询动态(
        稳定编码 主题存在节点) const noexcept;
    std::vector<稳定编码> 按状态查询动态(
        稳定编码 状态节点) const noexcept;
    std::vector<稳定编码> 按概念查询动态(
        稳定编码 动态概念节点) const noexcept;

    // 只在上层已经确认聚合结果稳定后调用。成功后动态改为引用共享聚合
    // 概念，并删除原来的实例动态概念节点。
    新动态操作状态 采用聚合动态概念(
        稳定编码 动态节点,
        稳定编码 聚合动态概念节点) noexcept;

    新动态操作状态 删除动态(稳定编码 动态节点) noexcept;

private:
    friend class 新_场景类;

    新动态操作状态 增加引用(稳定编码 动态节点) noexcept;
    新动态操作状态 撤销增加引用(稳定编码 动态节点) noexcept;
    新动态操作状态 减少引用(稳定编码 动态节点) noexcept;

    新_状态类& 状态服务_;
    概念_动态类& 动态概念服务_;
};

} // namespace 海中鱼巣
