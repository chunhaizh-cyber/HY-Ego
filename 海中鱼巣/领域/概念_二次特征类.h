#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "概念_特征类.h"

namespace 海中鱼巣 {

enum class 二次特征算法 : std::int64_t {
    有向特征差值 = 1,
    特征材料相似度 = 2,
    强时间间隔 = 3
};

enum class 二次特征输入来源种类 : std::int64_t {
    特征准确值 = 1,
    强时间 = 2
};

enum class 二次特征输入角色 : std::int64_t {
    左输入 = 1,
    右输入 = 2
};

struct 二次特征概念输入定义 final {
    std::int64_t 顺序 = 0;
    二次特征输入角色 角色 = 二次特征输入角色::左输入;
    二次特征输入来源种类 来源种类 = 二次特征输入来源种类::特征准确值;
    std::optional<稳定编码> 特征类型概念节点;
    friend bool operator==(const 二次特征概念输入定义&,
        const 二次特征概念输入定义&) = default;
};

// 输出特征定义承载二次特征类型的材料、单位、总值域和名称。
// 输入组合与算法决定该结果是如何形成的。
struct 二次特征概念定义 final {
    特征概念定义 输出特征定义;
    二次特征算法 算法 = 二次特征算法::有向特征差值;
    std::vector<二次特征概念输入定义> 输入定义组;
    friend bool operator==(const 二次特征概念定义&,
        const 二次特征概念定义&) = default;
};

struct 二次特征概念信息 final {
    // 节点可以是类型根本身，也可以是其下进一步细分的结果值域概念。
    稳定编码 节点;
    稳定编码 二次特征类型根节点;
    特征概念信息 输出特征概念;
    二次特征算法 算法 = 二次特征算法::有向特征差值;
    std::vector<二次特征概念输入定义> 输入定义组;
    friend bool operator==(const 二次特征概念信息&,
        const 二次特征概念信息&) = default;
};

enum class 二次特征概念操作状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    已找到 = 3,
    未找到 = 4,
    入口拒绝 = 5,
    输出定义不合法 = 6,
    输入定义不合法 = 7,
    特征概念不存在 = 8,
    算法不适用 = 9,
    概念不存在 = 10,
    值域不合法 = 11,
    结构不一致 = 12,
    资源失败 = 13
};

struct 二次特征概念建立结果 final {
    二次特征概念操作状态 状态 = 二次特征概念操作状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
};

struct 二次特征概念查找结果 final {
    二次特征概念操作状态 状态 = 二次特征概念操作状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
};

class 概念_二次特征类 final {
public:
    概念_二次特征类(
        概念_特征类& 特征概念服务,
        const 新_特征值类& 特征值服务) noexcept;

    // 只建立派生定义所需的共享字段和辅助节点类型，不建立第二概念树。
    bool 初始化() noexcept;
    std::optional<稳定编码> 获取纳秒单位节点() const noexcept;

    // 相同输出定义、相同有序输入组合和相同算法只建立一个类型根。
    二次特征概念建立结果 建立或取得二次特征概念(
        const 二次特征概念定义& 定义) noexcept;

    // 在既有二次特征类型内建立更小的结果值域概念。
    二次特征概念建立结果 建立或取得结果值域概念(
        稳定编码 上位二次特征概念节点,
        const 特征概念值域& 结果值域,
        const std::vector<稳定编码>& 名称关系 = {}) noexcept;

    std::optional<二次特征概念信息> 获取二次特征概念(
        稳定编码 概念节点) const noexcept;
    bool 是二次特征概念(稳定编码 概念节点) const noexcept;

    // 从类型根向下返回包含准确结果的最具体值域概念；不自动建立新区间。
    二次特征概念查找结果 查找结果对应最具体概念(
        稳定编码 二次特征类型根节点,
        std::int64_t 准确结果) const noexcept;

private:
    概念_特征类& 特征概念服务_;
    const 新_特征值类& 特征值服务_;
};

} // namespace 海中鱼巣
