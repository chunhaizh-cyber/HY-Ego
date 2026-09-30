#pragma once

#include <cstdint>
#include <optional>
#include <variant>

#include "新_特征类.h"
#include "概念_二次特征类.h"

namespace 海中鱼巣 {

struct 二次特征准确特征输入 final {
    std::optional<稳定编码> 来源节点;
    稳定编码 特征类型概念节点;
    新特征准确值 值;
    friend bool operator==(const 二次特征准确特征输入&,
        const 二次特征准确特征输入&) = default;
};

struct 二次特征强时间输入 final {
    std::optional<稳定编码> 来源状态节点;
    std::int64_t 纳秒 = 0;
    friend bool operator==(const 二次特征强时间输入&,
        const 二次特征强时间输入&) = default;
};

using 二次特征准确输入 = std::variant<
    二次特征准确特征输入,
    二次特征强时间输入>;

struct 新二次特征计算请求 final {
    稳定编码 二次特征类型概念节点;
    二次特征准确输入 左输入;
    二次特征准确输入 右输入;
    friend bool operator==(const 新二次特征计算请求&,
        const 新二次特征计算请求&) = default;
};

enum class 新二次特征计算状态 : std::uint8_t {
    已完成 = 1,
    入口拒绝 = 2,
    概念不存在 = 3,
    派生定义不完整 = 4,
    输入种类不匹配 = 5,
    特征类型不匹配 = 6,
    材料类型不相容 = 7,
    单位不相容 = 8,
    规则不支持 = 9,
    结果不在值域 = 10,
    结构不一致 = 11,
    资源失败 = 12
};

struct 新二次特征计算结果 final {
    新二次特征计算状态 状态 = 新二次特征计算状态::入口拒绝;
    std::optional<稳定编码> 二次特征类型概念节点;
    std::optional<稳定编码> 结果值域概念节点;
    std::optional<std::int64_t> 结果值;
    std::optional<二次特征准确输入> 左来源;
    std::optional<二次特征准确输入> 右来源;
};

class 新_二次特征类 final {
public:
    新_二次特征类(
        新_特征类& 特征服务,
        概念_二次特征类& 二次特征概念服务) noexcept;

    // 每次只计算一次并返回值式材料；不建立实例特征或修改任何来源节点。
    新二次特征计算结果 计算(
        const 新二次特征计算请求& 请求) const noexcept;

private:
    新_特征类& 特征服务_;
    概念_二次特征类& 二次特征概念服务_;
};

} // namespace 海中鱼巣
