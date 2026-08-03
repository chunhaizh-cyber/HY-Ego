// 文件规则：本头只承载跨两个以上世界树领域服务共享的纯值事实。
#pragma once

#include "../核心/节点直接结构合同.数据.h"

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 世界树服务合同版本 = 1;
inline constexpr std::uint32_t 世界树规则版本 = 1;
inline constexpr std::uint32_t 世界树稳定排序规则版本 = 1;

using 世界树幂等身份 = 节点直接事务幂等身份;
using 世界树节点身份见证 = 节点稳定身份见证;
using 世界树关系身份见证 = 关系稳定身份见证;

inline constexpr 服务稳定身份 世界结构服务身份{
    static_cast<std::uint64_t>(服务稳定身份命名域::L2领域服务), 1};
inline constexpr 服务稳定身份 状态服务身份{
    static_cast<std::uint64_t>(服务稳定身份命名域::L2领域服务), 2};
inline constexpr 服务稳定身份 动态服务身份{
    static_cast<std::uint64_t>(服务稳定身份命名域::L2领域服务), 3};
inline constexpr 服务稳定身份 存在服务身份{
    static_cast<std::uint64_t>(服务稳定身份命名域::L2领域服务), 4};

inline constexpr 类型合同稳定身份 世界场景资格类型合同{
    static_cast<std::uint64_t>(类型合同稳定身份命名域::L2领域合同), 1};
inline constexpr 类型合同稳定身份 世界存在资格类型合同{
    static_cast<std::uint64_t>(类型合同稳定身份命名域::L2领域合同), 2};

enum class 世界场景资格值 : std::int64_t {
    现实世界根 = 1, 初始场景 = 2, 普通场景 = 3
};
enum class 世界存在资格值 : std::int64_t {
    实际存在 = 1, 治理虚拟存在 = 2
};

struct 世界树请求头 {
    std::uint32_t 合同版本 = 世界树服务合同版本;
    std::uint32_t 世界规则版本 = 世界树规则版本;
    世界树节点身份见证 世界根;
};

enum class 世界树缺项种类 : std::uint8_t {
    根 = 1, 场景父关系 = 2, 场景成员关系 = 3, 存在归属 = 4,
    特征槽 = 5, 特征定义 = 6, 特征当前值 = 7,
    状态角色 = 8, 动态角色 = 9, 端点 = 10
};

struct 世界树缺项 {
    世界树缺项种类 种类 = 世界树缺项种类::根;
    std::optional<世界树节点身份见证> 对象;
    std::optional<世界树关系身份见证> 关系;
};

struct 世界树存在事实 {
    世界树节点身份见证 存在;
    std::optional<世界树关系身份见证> 当前场景归属;
};

struct 世界树场景事实 {
    世界树节点身份见证 场景;
    std::optional<世界树关系身份见证> 直接父关系;
    std::vector<世界树关系身份见证> 直接子场景关系组;
    std::vector<世界树关系身份见证> 直接存在成员关系组;
};

struct 世界树创建存在读回 {
    世界树存在事实 存在;
    世界树关系身份见证 场景归属;
};
struct 世界树移动读回 {
    世界树节点身份见证 成员;
    世界树关系身份见证 旧关系;
    世界树关系身份见证 新关系;
};
struct 世界树移除读回 {
    世界树节点身份见证 存在;
    世界树关系身份见证 已失效归属;
};

} // namespace 海中鱼巣
