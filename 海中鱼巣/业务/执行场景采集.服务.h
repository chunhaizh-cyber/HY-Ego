#pragma once

#include "../领域/方法问题反馈.数据.h"

namespace 海中鱼巣 {

struct 新执行场景采集请求 final {
    新场景快照阶段 阶段 = 新场景快照阶段::动作前;
    std::vector<稳定编码> 入口场景组, 额外存在组, 额外特征组,
        额外状态组, 额外动态组, 额外特征概念组;
};
enum class 新执行场景采集状态 : std::uint8_t { 已完成 = 1, 材料不完整, 资源失败 };
struct 新执行场景采集结果 final {
    新执行场景采集状态 状态 = 新执行场景采集状态::材料不完整;
    新执行场景快照 快照;
};

// 只组合公开读取，不写现实结构；全部依赖须比本服务存活更久。
class 执行场景采集服务 final {
public:
    执行场景采集服务(const 新_场景类&, const 新_存在类&,
        const 新_特征类&, const 新_特征值类&, const 新_状态类&,
        const 新_动态类&, const 概念_特征类&, const 概念_存在类&) noexcept;
    新执行场景采集结果 采集(const 新执行场景采集请求&) const noexcept;

private:
    const 新_场景类& 场景_;
    const 新_存在类& 存在_;
    const 新_特征类& 特征_;
    const 新_特征值类& 值_;
    const 新_状态类& 状态_;
    const 新_动态类& 动态_;
    const 概念_特征类& 特征概念_;
    const 概念_存在类& 存在概念_;
};

} // namespace 海中鱼巣
