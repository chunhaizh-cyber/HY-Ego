#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "../核心/基础数据集.h"
#include "概念_二次特征类.h"

namespace 海中鱼巣 {

// 当次只读内容的共同表达；不建立特征实例，不授予被引用对象的写权。
enum class 正式内容值种类 : std::uint8_t {
    I64数值=1, 存在引用, 概念引用, 需求引用, 语言理解引用, 内容确认引用,
    // R102（SET-DOMAIN-MEMBER-CONCEPT §4）：任务领域节点引用（任务、任务虚拟存在及其执行/评估/结算节点）与方法引用。
    任务引用, 方法引用,
    词条引用 = 9
};
enum class 正式内容值状态 : std::uint8_t { 已知=1, 当前未知, 不适用 };
using 正式内容准确值=std::variant<std::int64_t,稳定编码>;
struct 正式内容值 final {
    正式内容值种类 种类{};
    正式内容值状态 状态{};
    std::optional<正式内容准确值> 值;
};
struct 正式直接内容项 final {
    稳定编码 特征类型;
    正式内容值 内容;
};

enum class 正式信息参数读取状态 : std::uint8_t {
    已读取=1, 信息不存在, 范围未支持, 依赖未就绪, 前提已变化, 资源失败
};
// 当次引用定义的完备程度，不表示需求已满足、已确认或语言能够表达。
enum class 正式引用概念语义状态 : std::uint8_t {
    完整=1, 部分=2, 引用内容未展开=3
};
struct 正式内容引用定位 final {
    稳定编码 信息节点;
    std::vector<稳定编码> 正式内容路径;
};
struct 正式引用概念语义 final {
    正式引用概念语义状态 状态=正式引用概念语义状态::部分;
    std::vector<特征概念定义保护信息> 特征概念定义组;
    std::vector<二次特征概念信息> 二次特征定义组;
    std::optional<正式内容引用定位> 引用内容定位;
};
struct 正式信息参数投影 final {
    稳定编码 信息节点, 类别概念;
    std::vector<正式直接内容项> 直接内容组;
    // 未提供语义的既有reader保持nullopt；不允许消费者据此推定语义完整。
    std::optional<正式引用概念语义> 引用概念语义;
};
struct 正式信息参数读取结果 final {
    正式信息参数读取状态 状态=正式信息参数读取状态::范围未支持;
    std::optional<正式信息参数投影> 信息;
    std::vector<基础联合节点快照> 完整保护组;
};
class 正式信息参数读取接口 {
public:
    virtual ~正式信息参数读取接口()=default;
    virtual 正式信息参数读取结果 读取正式信息参数保护(
        稳定编码 信息节点, 稳定编码 范围概念) const noexcept=0;
};

} // namespace 海中鱼巣
