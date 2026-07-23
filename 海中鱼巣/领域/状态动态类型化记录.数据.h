// 文件规则：本文件只定义状态 / 动态领域的强类型记录、请求、规格和值式 DTO；不得保存拓扑副本、仓库、令牌、锁或可写引用。
#pragma once

#include "../核心/句柄.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

// 下列公共合同类型由 NT-C1 唯一提供；本文件只消费其预冻结声明，不重复定义。
enum class 合同结果状态 : std::uint32_t;
enum class 合同结果原因 : std::uint32_t;
struct 正式关系值式材料;
template <typename 值类型> struct 合同单项结果;
template <typename 值类型> struct 合同具名组结果;
template <typename 值类型> struct 带值结构写入结果;
template <typename 值类型> struct 带值读取结果;
template <typename 值类型> struct 带值读取结果;

enum class 状态记录模式 : std::uint32_t {
    观察 = 1,
    动作结果 = 2,
    治理事实 = 3
};

enum class 动态种类 : std::uint32_t {
    状态迁移动能 = 1,
    动作致变 = 2,
    观察动态 = 3,
    外部事件动态 = 4,
    聚合动态 = 5
};

struct 状态域记录 {
    节点句柄 所属状态;
    std::uint64_t 格式版本 = 1;
    std::uint64_t 记录版本 = 1;
    记录状态 状态 = static_cast<记录状态>(0);
    std::uint64_t 发生时间 = 0;
    std::optional<std::uint64_t> 接收时间;
    状态记录模式 记录模式 = static_cast<状态记录模式>(0);

    bool 值域有效() const noexcept {
        return 句柄有效(所属状态) && 格式版本 == 1 && 记录版本 != 0
            && 发生时间 != 0 && (!接收时间 || *接收时间 >= 发生时间)
            && static_cast<std::uint32_t>(记录模式) >= 1
            && static_cast<std::uint32_t>(记录模式) <= 3;
    }
};

struct 动态域记录 {
    节点句柄 所属动态;
    std::uint64_t 格式版本 = 1;
    std::uint64_t 记录版本 = 1;
    记录状态 状态 = static_cast<记录状态>(0);
    std::uint64_t 发生时间 = 0;
    动态种类 种类 = static_cast<动态种类>(0);
    std::uint64_t 聚合规则版本 = 0;

    bool 值域有效() const noexcept {
        const auto 种类值 = static_cast<std::uint32_t>(种类);
        return 句柄有效(所属动态) && 格式版本 == 1 && 记录版本 != 0
            && 发生时间 != 0 && 种类值 >= 1 && 种类值 <= 5
            && ((种类 == 动态种类::聚合动态 && 聚合规则版本 != 0)
                || (种类 != 动态种类::聚合动态 && 聚合规则版本 == 0));
    }
};

struct 完整实例状态请求 {
    节点稳定主键 状态稳定主键;
    节点句柄 主体;
    节点句柄 场景;
    节点句柄 特征;
    节点句柄 特征值;
    节点句柄 来源存在;
    std::uint64_t 发生时间 = 0;
    std::optional<std::uint64_t> 接收时间;
    状态记录模式 记录模式 = static_cast<状态记录模式>(0);
};

struct 状态事实值式材料 {
    节点句柄 状态;
    节点稳定主键 稳定主键;
    std::uint64_t 节点版本 = 0;
    std::uint64_t 记录版本 = 0;
    std::uint64_t 发生时间 = 0;
    std::optional<std::uint64_t> 接收时间;
    状态记录模式 记录模式 = static_cast<状态记录模式>(0);
    std::vector<正式关系值式材料> 角色证据;

    bool 角色证据完整() const noexcept { return 角色证据.size() == 5; }
};

struct 主体状态事实组值式材料 {
    节点句柄 主体;
    std::vector<状态事实值式材料> 项目组;
    std::uint64_t 读取结构版本 = 0;
};

struct 完整动态请求 {
    节点稳定主键 动态稳定主键;
    动态种类 种类 = static_cast<动态种类>(0);
    节点句柄 主体;
    节点句柄 场景;
    节点句柄 被改变目标;
    节点句柄 前状态;
    节点句柄 后状态;
    节点句柄 来源存在;
    std::optional<节点句柄> 来源动作;
    std::vector<节点句柄> 来源低层动态组;
    std::optional<节点句柄> 同源状态迁移动能;
    std::uint64_t 发生时间 = 0;
    std::uint64_t 聚合规则版本 = 0;
};

struct 动态事实值式材料 {
    节点句柄 动态;
    节点稳定主键 稳定主键;
    std::uint64_t 节点版本 = 0;
    std::uint64_t 记录版本 = 0;
    std::uint64_t 发生时间 = 0;
    std::uint64_t 聚合规则版本 = 0;
    动态种类 种类 = static_cast<动态种类>(0);
    std::vector<正式关系值式材料> 角色证据;

    bool 角色证据完整() const noexcept { return 角色证据.size() == 9; }
};

struct 主体动态事实组值式材料 {
    节点句柄 主体;
    std::vector<动态事实值式材料> 项目组;
    std::uint64_t 读取结构版本 = 0;
};

struct 状态退役规格 {
    节点句柄 目标;
    std::uint64_t 预期版本 = 0;
};

struct 动态退役规格 {
    节点句柄 目标;
    std::uint64_t 预期版本 = 0;
};

struct 状态退役材料 {
    节点句柄 状态;
    std::uint64_t 节点版本 = 0;
    std::uint64_t 记录版本 = 0;
    std::vector<正式关系值式材料> 关系19组;
    std::vector<正式关系值式材料> 被动态引用组;
};

struct 动态退役材料 {
    节点句柄 动态;
    std::uint64_t 节点版本 = 0;
    std::uint64_t 记录版本 = 0;
    std::vector<正式关系值式材料> 关系20组;
    std::vector<正式关系值式材料> 被聚合或同源引用组;
};

using 完整状态写入结果 = 带值结构写入结果<状态事实值式材料>;
using 完整状态读取结果 = 合同单项结果<状态事实值式材料>;
using 主体状态事实组读取结果 = 合同具名组结果<主体状态事实组值式材料>;
using 完整动态写入结果 = 带值结构写入结果<动态事实值式材料>;
using 完整动态读取结果 = 合同单项结果<动态事实值式材料>;
using 主体动态事实组读取结果 = 合同具名组结果<主体动态事实组值式材料>;
using 状态退役规格结果 = 合同单项结果<状态退役规格>;
using 动态退役规格结果 = 合同单项结果<动态退役规格>;

}
