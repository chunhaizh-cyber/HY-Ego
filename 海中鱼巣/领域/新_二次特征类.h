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
    // 类型节点为空时：优先按此完整定义建立；未提供时根据输入和实际
    // 比较算法推导最小定义。每次成功计算都会确保结果域概念存在。
    std::optional<二次特征概念定义> 概念定义;
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

struct 二次特征类型变化输入 final {
    std::optional<稳定编码> 来源动态节点;
    稳定编码 特征类型概念节点;
    新特征准确值 开始值;
    新特征准确值 结束值;
    friend bool operator==(const 二次特征类型变化输入&,
        const 二次特征类型变化输入&) = default;
};

struct 二次特征时间变化输入 final {
    std::optional<稳定编码> 来源动态节点;
    std::int64_t 开始纳秒 = 0;
    std::int64_t 结束纳秒 = 0;
    friend bool operator==(const 二次特征时间变化输入&,
        const 二次特征时间变化输入&) = default;
};

using 二次特征变化输入 = std::variant<
    二次特征类型变化输入,
    二次特征时间变化输入>;

struct 新二次跨类型变化比较请求 final {
    // 参照变化被归一化为一个单位；目标变化始终使用目标特征类型
    // 自己的比较规则取得，不直接比较两个不同单位的原始值。
    二次特征变化输入 参照变化;
    二次特征变化输入 目标变化;
    // 可选参照只用于划分计算结果域；不改变两侧变化量的计算方式。
    std::optional<稳定编码> 参照特征概念节点;
    friend bool operator==(const 新二次跨类型变化比较请求&,
        const 新二次跨类型变化比较请求&) = default;
};

enum class 新二次跨类型变化比较状态 : std::uint8_t {
    已完成 = 1,
    入口拒绝 = 2,
    不是跨类型比较 = 3,
    概念不存在 = 4,
    材料类型不相容 = 5,
    单位不相容 = 6,
    规则不支持 = 7,
    参照无变化 = 8,
    结构不一致 = 9,
    资源失败 = 10
};

struct 新二次跨类型变化比较结果 final {
    新二次跨类型变化比较状态 状态 =
        新二次跨类型变化比较状态::入口拒绝;
    std::optional<std::int64_t> 参照原始变化量;
    std::optional<std::int64_t> 目标原始变化量;
    // 目标原始变化量 / 参照原始变化量。C++整数除法向零截断；余数
    // 与原始两侧变化量共同保留完整关系。唯一溢出MIN/-1饱和为MAX。
    std::optional<std::int64_t> 单位参照目标变化量;
    std::optional<std::int64_t> 归一化余数;
    bool 结果已饱和 = false;
    std::optional<稳定编码> 二次特征类型概念节点;
    std::optional<稳定编码> 结果值域概念节点;
    std::optional<二次特征变化输入> 参照来源;
    std::optional<二次特征变化输入> 目标来源;
};

class 新_二次特征类 final {
public:
    新_二次特征类(
        新_特征类& 特征服务,
        概念_二次特征类& 二次特征概念服务) noexcept;

    // 每次只计算一次并返回值式材料；不建立实例特征或修改任何来源节点。
    新二次特征计算结果 计算(
        const 新二次特征计算请求& 请求) const noexcept;

    // 比较两种不同变化：先分别按各自特征类型取得“结束值-开始值”，
    // 再返回参照每变化一个单位期间目标特征的变化量。
    新二次跨类型变化比较结果 比较跨特征类型变化(
        const 新二次跨类型变化比较请求& 请求) const noexcept;

private:
    新_特征类& 特征服务_;
    概念_二次特征类& 二次特征概念服务_;
};

} // namespace 海中鱼巣
