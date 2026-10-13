#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>
#include "新_需求类.h"

namespace 海中鱼巣 {

enum class 新服务要求分项种类 : std::uint8_t {
    来源人类 = 1, 目标存在, 目标特征类型, 目标定义
};

// 字段直接值没有独立节点；引用明确的理解及其中一项，不复制第二份事实。
struct 新服务要求分项引用 final {
    稳定编码 理解节点;
    新服务要求分项种类 分项 = 新服务要求分项种类::来源人类;
    friend bool operator==(const 新服务要求分项引用&, const 新服务要求分项引用&) = default;
};
struct 新服务要求确认引用 final {
    稳定编码 确认节点;
    friend bool operator==(const 新服务要求确认引用&, const 新服务要求确认引用&) = default;
};
struct 新服务要求目标分项 final {
    新需求目标形式 目标形式 = 新需求目标形式::值;
    std::optional<稳定编码> 目标值域概念节点;
    std::optional<稳定编码> 目标比较关系概念节点;
    std::optional<新需求变化方向> 目标变化方向;
    friend bool operator==(const 新服务要求目标分项&, const 新服务要求目标分项&) = default;
};
struct 新服务要求分项材料 final {
    新服务要求分项引用 引用;
    std::variant<稳定编码, 新服务要求目标分项> 值;
    friend bool operator==(const 新服务要求分项材料&, const 新服务要求分项材料&) = default;
};
enum class 新服务要求材料读取状态 : std::uint8_t {
    已读取 = 1, 材料不存在, 工程故障
};
struct 新服务要求分项读取结果 final {
    新服务要求材料读取状态 状态 = 新服务要求材料读取状态::工程故障;
    std::optional<新服务要求分项材料> 材料;
};
class 新服务要求材料读取接口 : public 新服务要求确认读取接口 {
public:
    virtual 新服务要求分项读取结果 读取服务要求分项(
        const 新服务要求分项引用&) const noexcept = 0;
};

// 调用期值式交付，不是正式需求节点，也不是持久化的第二材料仓库。
struct 新服务需求信息包 final {
    稳定编码 来源人类节点, 目标存在节点;
    新需求目标定义 目标;
    std::array<新服务要求分项引用, 4> 分项来源;
    新服务要求确认引用 确认;
    friend bool operator==(const 新服务需求信息包&, const 新服务需求信息包&) = default;
};

enum class 新服务需求获取方式 : std::uint8_t { 主动 = 1, 被动 = 2 };
struct 新服务需求获取配置 final {
    稳定编码 自我存在节点, 所在场景节点, 信息包存在概念节点;
    // 顺序：来源人类、目标存在、目标特征类型、目标定义、确认。
    std::array<稳定编码, 5> 参数角色概念组;
    friend bool operator==(const 新服务需求获取配置&, const 新服务需求获取配置&) = default;
};
struct 新服务需求获取动作回执 final {
    bool 已新建 = false;
    std::optional<稳定编码> 动作节点;
};
class 方法参数场景;
struct 新方法知识建立请求;
struct 新方法建立结果;

// 动作身份保留在外层执行租约中，不作为函数的第二个上下文参数。
class 新服务需求获取本能端口 {
public:
    virtual ~新服务需求获取本能端口() = default;
    virtual std::optional<新服务需求获取配置> 读取登记材料() const noexcept = 0;
    virtual 新方法建立结果 确保方法登记(新服务需求获取方式,
        const 新方法知识建立请求&) noexcept = 0;
    virtual bool 核验执行(新服务需求获取方式, const 方法参数场景&) const noexcept = 0;
    virtual 新服务需求获取动作回执 建立动作(新服务需求获取方式,
        const 方法参数场景&, 稳定编码 来源人类节点) noexcept = 0;
    virtual void 报告工程故障(std::string_view) noexcept = 0;
};
using 新服务需求工程报告函数 = void (*)(std::string_view) noexcept;
} // namespace 海中鱼巣
