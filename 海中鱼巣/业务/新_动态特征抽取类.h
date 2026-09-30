#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../领域/新_动态类.h"
#include "../领域/新_二次特征类.h"

namespace 海中鱼巣 {

// 抽取结果可直接交给概念_动态类::保存动态特征，不复制第二套DTO。
using 新动态特征项 = 动态概念特征项;

struct 新动态特征抽取请求 final {
    稳定编码 动态节点;
    // 调用者明确本次需要的二次特征类型。抽取器不扫描概念树猜测算法。
    std::vector<稳定编码> 二次特征类型概念节点组;
    friend bool operator==(const 新动态特征抽取请求&,
        const 新动态特征抽取请求&) = default;
};

enum class 新动态特征抽取状态 : std::uint8_t {
    已完成 = 1,
    入口拒绝 = 2,
    动态不存在 = 3,
    状态不足 = 4,
    状态不存在 = 5,
    状态轨迹不一致 = 6,
    二次特征概念不存在 = 7,
    二次特征不适用 = 8,
    二次特征计算失败 = 9,
    结构不一致 = 10,
    资源失败 = 11
};

struct 新动态特征抽取结果 final {
    新动态特征抽取状态 状态 = 新动态特征抽取状态::入口拒绝;
    std::optional<稳定编码> 动态节点;
    std::optional<新二次特征计算状态> 二次特征失败状态;
    std::vector<新动态特征项> 特征组;
};

class 新_动态特征抽取类 final {
public:
    新_动态特征抽取类(
        const 新_动态类& 动态服务,
        const 新_状态类& 状态服务,
        const 新_二次特征类& 二次特征服务,
        const 概念_二次特征类& 二次特征概念服务) noexcept;

    // 只读取实例动态和来源状态并计算值式结果；不修改动态、状态或概念。
    新动态特征抽取结果 抽取(
        const 新动态特征抽取请求& 请求) const noexcept;

private:
    const 新_动态类& 动态服务_;
    const 新_状态类& 状态服务_;
    const 新_二次特征类& 二次特征服务_;
    const 概念_二次特征类& 二次特征概念服务_;
};

} // namespace 海中鱼巣
