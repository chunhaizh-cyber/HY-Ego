#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>
#include "新_需求类.h"
#include "新_方法公共类型.h"
#include "概念_二次特征类.h"

namespace 海中鱼巣 {

struct 服务要求特征语义快照 {
    稳定编码 概念节点;
    特征概念材料物理类型 材料类型 = 特征概念材料物理类型::I64标量;
    std::optional<稳定编码> 单位;
    特征概念比较规则 比较规则 = 特征概念比较规则::I64数值;
    friend bool operator==(const 服务要求特征语义快照&, const 服务要求特征语义快照&) = default;
};
struct 服务要求值域语义快照 {
    服务要求特征语义快照 特征语义;
    特征概念值域 值域;
    friend bool operator==(const 服务要求值域语义快照&, const 服务要求值域语义快照&) = default;
};
struct 服务要求比较语义快照 {
    稳定编码 比较概念节点, 类型根节点;
    二次特征算法 算法 = 二次特征算法::有向特征差值;
    std::vector<二次特征概念输入定义> 输入定义组;
    std::optional<稳定编码> 参照概念节点;
    服务要求值域语义快照 输出语义;
    std::vector<二次特征结果域种类> 结果域种类组;
    friend bool operator==(const 服务要求比较语义快照&, const 服务要求比较语义快照&) = default;
};
struct 服务要求目标语义快照 {
    服务要求特征语义快照 目标类型;
    std::optional<服务要求值域语义快照> 目标域;
    std::optional<服务要求比较语义快照> 比较;
    friend bool operator==(const 服务要求目标语义快照&, const 服务要求目标语义快照&) = default;
};
struct 服务要求理解信息 {
    稳定编码 节点, 语言场景节点;
    std::string 公开会话标识;
    稳定编码 来源人类节点, 目标存在节点;
    新需求目标定义 目标;
    服务要求目标语义快照 固定语义;
    std::optional<稳定编码> 来源要求节点;
    friend bool operator==(const 服务要求理解信息&, const 服务要求理解信息&) = default;
};
struct 服务要求确认信息 {
    稳定编码 节点;
    服务要求理解信息 理解;
    bool 需重新确认 = false;
    friend bool operator==(const 服务要求确认信息&, const 服务要求确认信息&) = default;
};

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

// 仅保留格式2历史归档的编解码形状。新本能执行不得构造或返回此包。
struct 新服务需求信息包 final {
    稳定编码 来源人类节点, 目标存在节点;
    新需求目标定义 目标;
    std::array<新服务要求分项引用, 4> 分项来源;
    新服务要求确认引用 确认;
    friend bool operator==(const 新服务需求信息包&, const 新服务需求信息包&) = default;
};

class 方法参数场景;
struct 新方法知识建立请求;
struct 新方法建立结果;
struct 新本能函数本次条件结果对;

struct 新服务对象新需求配置 final {
    稳定编码 自我存在节点, 人类概念节点, 通用存在概念节点;
    稳定编码 对象需求类型, 完整被认知需求概念;
    稳定编码 需求身份概念;
    friend bool operator==(const 新服务对象新需求配置&,
        const 新服务对象新需求配置&) = default;
};
// 获取服务对象新需求唯一有序规格（R97）：1人类{对象需求}、2对象需求特征/1、3需求身份{}。
inline std::vector<新方法有序参数规格> 服务对象新需求有序规格(const 新服务对象新需求配置& c) {
    using T = 新方法参数信息类型;
    return {{1, T::存在, c.人类概念节点, {c.对象需求类型}, 0},
        {2, T::特征, c.对象需求类型, {}, 1},
        {3, T::存在, c.需求身份概念, {}, 0}};
}
// 新需求的材料来自显式绑定的场景存在；本端口不筹办、不承接为自我需求。
class 新服务对象新需求本能端口 {
public:
    virtual ~新服务对象新需求本能端口() = default;
    virtual std::optional<新服务对象新需求配置> 读取登记材料() const noexcept = 0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept = 0;
    virtual 新本能函数本次条件结果对 执行(const 方法参数场景&) noexcept = 0;
};
} // namespace 海中鱼巣
