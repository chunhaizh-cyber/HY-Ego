#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_状态类.h"
#include "概念_动态类.h"

namespace 海中鱼巣 {

class 新_场景类;

struct 新动态信息 final {
    稳定编码 节点;
    稳定编码 主题存在节点;
    std::vector<稳定编码> 状态节点组;
    稳定编码 动态概念节点;
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
    概念不合法 = 15
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

    // 状态组是同一主题、同一特征类型的有序实例状态轨迹。
    // 实例状态必须使用绝对时间且严格递增。
    新动态建立结果 建立动态(
        稳定编码 主题存在节点,
        const std::vector<稳定编码>& 状态节点组) noexcept;

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
    新动态操作状态 减少引用(稳定编码 动态节点) noexcept;

    新_状态类& 状态服务_;
    概念_动态类& 动态概念服务_;
};

} // namespace 海中鱼巣
