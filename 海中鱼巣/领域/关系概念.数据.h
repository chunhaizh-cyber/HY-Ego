#pragma once
// 世界树组织架构的关系概念（非 7270 语言构式）：端点角色约束 + 已有边/字段连接描述。
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "../核心/基础数据集.h"
#include "新_语言类.h"

namespace 海中鱼巣 {

enum class 关系连接种类 : std::uint8_t { 外部边 = 1, 字段引用 = 2 };

enum class 关系端点对象种类 : std::uint8_t {
    存在 = 1,
    需求 = 2
};

struct 关系端点约束 final {
    std::string 角色键; // 如 整体/部分、父/子
    关系端点对象种类 对象种类 = 关系端点对象种类::存在;
    friend bool operator==(const 关系端点约束&, const 关系端点约束&) = default;
};

// 外部边：业务语义 = owner 标识 + 边类型 + 角色或顺序 + 方向（源/目标角色键）。
struct 关系外部边连接 final {
    std::string owner标识; // 如 新_存在类 / 新_需求类
    基础外部关系类型 边类型 = 基础外部关系类型::父子;
    std::int64_t 角色或顺序 = 0;
    std::string 源角色键;
    std::string 目标角色键;
    friend bool operator==(const 关系外部边连接&, const 关系外部边连接&) = default;
};

// 字段引用连接：统一可扩展槽；本切片生成路径可返回未接线。
struct 关系字段引用连接 final {
    std::string owner标识;
    std::string 字段键名;
    std::string 宿主角色键;
    std::string 目标角色键;
    friend bool operator==(const 关系字段引用连接&, const 关系字段引用连接&) = default;
};

using 关系连接描述 = std::variant<关系外部边连接, 关系字段引用连接>;

struct 关系概念语言名称 final {
    稳定编码 语言节点{};
    稳定编码 词条节点{}; // 词组织根下词条；词面不在本登记项上存字符串
    bool 预置 = true;
    friend bool operator==(const 关系概念语言名称&, const 关系概念语言名称&) = default;
};

// 成文取材：把关系端点节点投影为语言图「存在」槽材料。
enum class 关系表达取材投影 : std::uint8_t {
    端点身份 = 1,     // 端点本身须为存在节点
    需求目标存在 = 2, // 端点为需求，取正式 目标存在节点
    需求主体存在 = 3  // 端点为需求，取正式 需求主体存在节点
};

// 表达槽：关系端点角色 → 语言含义参数；差异由知识数据提供，非每关系专用构图函数。
struct 关系表达角色槽 final {
    std::string 端点角色键;
    // 与 新语言知识参数角色 对齐（整体/部分/上级/下级等）。
    std::int64_t 参数角色码 = 0;
    关系表达取材投影 投影 = 关系表达取材投影::端点身份;
    friend bool operator==(const 关系表达角色槽&, const 关系表达角色槽&) = default;
};

// 组合陈述中的一段：各自绑定语言知识与槽；由统一入口逐段成文再连接，无关系专用构图。
struct 关系表达陈述段 final {
    std::int64_t 含义操作码 = 0;
    std::string 预置知识尾词;
    std::vector<稳定编码> 表达知识节点组;
    std::vector<关系表达角色槽> 角色槽组;
    friend bool operator==(const 关系表达陈述段&, const 关系表达陈述段&) = default;
};

struct 关系概念表达关联 final {
    稳定编码 语言节点{};
    // 单段快捷：含义操作；组合时可为 0，改由 陈述段组 提供。
    std::int64_t 含义操作码 = 0;
    bool 预置 = true;
    // 单段快捷槽/尾词/知识；当 陈述段组 非空时忽略（改用各段）。
    std::vector<关系表达角色槽> 角色槽组;
    std::string 预置知识尾词;
    std::vector<稳定编码> 表达知识节点组;
    // 组合陈述：每段是需表达的关系内容（非子适用条件）；空=单段快捷。
    std::vector<关系表达陈述段> 陈述段组;
    friend bool operator==(const 关系概念表达关联&, const 关系概念表达关联&) = default;
};

struct 关系概念定义 final {
    std::string 稳定种子键;
    std::vector<关系端点约束> 端点组;
    // 主连接：边或字段引用；世界事实成立判定。
    关系连接描述 连接;
    // 附加连接：适用条件（须成立，默认不单独成句）；组合成句内容由 陈述段组 承担。
    std::vector<关系连接描述> 附加连接组;
    std::vector<关系概念语言名称> 名称组;
    std::vector<关系概念表达关联> 表达关联组;
    friend bool operator==(const 关系概念定义&, const 关系概念定义&) = default;
};

struct 关系概念信息 final {
    稳定编码 节点{};
    关系概念定义 定义;
    friend bool operator==(const 关系概念信息&, const 关系概念信息&) = default;
};

enum class 关系概念操作状态 : std::uint8_t {
    已建立 = 1,
    已复用,
    已读取,
    未找到,
    输入不合法,
    依赖未就绪,
    结构不一致,
    定义冲突,
    资源失败
};

struct 关系概念建立结果 final {
    关系概念操作状态 状态 = 关系概念操作状态::输入不合法;
    std::optional<关系概念信息> 信息;
};

struct 关系概念读取结果 final {
    关系概念操作状态 状态 = 关系概念操作状态::输入不合法;
    std::optional<关系概念信息> 信息;
};

struct 关系概念列表结果 final {
    关系概念操作状态 状态 = 关系概念操作状态::输入不合法;
    std::vector<关系概念信息> 信息组;
};

// 入站材料：解析结果写入自然语言场景侧材料（角色/来源/未确定），不写业务边/需求事实。
// 自我经交互场景查询取用；与出站共享关系种子键、角色槽与表达知识。
struct 关系陈述入站角色材料 final {
    std::string 端点角色键;
    std::optional<稳定编码> 所指存在;   // 已绑定存在；空=未确定
    std::optional<稳定编码> 场景实例;   // 自然语言场景内实例
    std::optional<稳定编码> 来源概念;
    bool 未确定 = false;
    friend bool operator==(const 关系陈述入站角色材料&, const 关系陈述入站角色材料&) = default;
};

struct 关系陈述入站材料 final {
    稳定编码 材料节点{};
    稳定编码 自然语言场景{};
    std::optional<稳定编码> 交互场景; // 供自我衔接；空=仅语言场景材料
    std::string 关系种子键;
    std::string 固定原文;
    std::vector<关系陈述入站角色材料> 角色组;
    std::vector<稳定编码> 所用知识节点组;
    friend bool operator==(const 关系陈述入站材料&, const 关系陈述入站材料&) = default;
};

struct 关系陈述入站材料结果 final {
    关系概念操作状态 状态 = 关系概念操作状态::输入不合法;
    std::optional<关系陈述入站材料> 信息;
};

} // namespace 海中鱼巣
