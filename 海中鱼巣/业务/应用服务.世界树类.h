#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include "../领域/数据服务.绑定存在.h"
#include "../领域/数据服务.世界树根.h"
#include "../领域/数据服务.场景类.h"
#include "../领域/数据服务.存在类.h"
#include "../领域/数据服务.概念树类.h"

namespace 海中鱼巣 {

enum class 世界树操作阶段 : std::uint8_t {
    无 = 0,
    现实树预读 = 1,
    存在创建 = 2,
    场景包含发布 = 3,
    现实树最终确认 = 4
};

enum class 世界树操作状态 : std::uint8_t {
    已验证现实根 = 1,
    已读取存在位置 = 2,
    已移动 = 3,
    已创建场景并纳入 = 4,
    已创建存在并纳入 = 5,
    精确重复 = 6,
    入口拒绝 = 7,
    场景不在现实树 = 8,
    存在不在现实树 = 9,
    成员多重位置 = 10,
    现实根不可移动 = 11,
    原位置不匹配 = 12,
    目标位置相同 = 13,
    形成场景环 = 14,
    引用冲突 = 15,
    幂等冲突 = 17,
    资源失败 = 20,
    内部不一致 = 21,
    部分已发布 = 22,
    已发布待复核 = 23,
    结果未知 = 24,
    既有操作已被后继事实覆盖 = 25,
    未实现 = 26
};

struct 世界树结果头 final {
    世界树操作状态 状态 = 世界树操作状态::入口拒绝;
    世界树操作阶段 阶段 = 世界树操作阶段::无;
    std::optional<存在类数据状态> 存在原因;
    std::optional<场景直接包含状态> 场景原因;
    std::optional<绑定存在创建状态> 绑定原因;
};

struct 世界树根验证请求 final {
    std::optional<稳定编码> 期望根;
    friend bool operator==(const 世界树根验证请求&,
                           const 世界树根验证请求&) = default;
};

struct 世界树根验证结果 final {
    世界树结果头 结果头;
    std::optional<场景树当前事实> 树;
    bool 成功(const 世界树根验证请求&) const noexcept;
};

enum class 世界树节点视角 : std::uint8_t {
    世界根场景 = 1,
    场景 = 2,
    存在 = 3
};

struct 世界树层级位置读取请求 final {
    稳定编码 节点{};
    世界树节点视角 预期视角 = 世界树节点视角::存在;
};

struct 世界树层级位置 final {
    稳定编码 节点{}, 世界根{};
    世界树节点视角 视角 = 世界树节点视角::存在;
    std::optional<直接归属联合事实> 直接结构父;
    std::optional<场景父语境投影事实> 父场景语境;
    std::vector<直接归属联合事实> 上行路径;
};

struct 世界树层级位置结果 final {
    世界树结果头 结果头;
    std::optional<世界树层级位置> 位置;
    bool 成功(const 世界树层级位置读取请求&) const noexcept;
};

struct 世界树存在位置读取请求 final { 稳定编码 存在{}; };
struct 世界树存在位置 final {
    稳定编码 存在{}, 所在场景{};
    场景直接包含事实 成员关系;
};
struct 世界树存在位置结果 final {
    世界树结果头 结果头;
    std::optional<世界树存在位置> 位置;
    bool 成功(const 世界树存在位置读取请求&) const noexcept;
};

struct 世界树节点查询请求 final {
    稳定编码 节点{};
    世界树节点视角 视角 = 世界树节点视角::存在;
};
struct 世界树节点组查询结果 final {
    世界树结果头 结果头;
    std::vector<稳定编码> 节点组;
    bool 成功(const 世界树节点查询请求&) const noexcept;
};

struct 世界树成员移动请求 final {
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 原场景{}, 目标场景{}, 成员{};
    friend bool operator==(const 世界树成员移动请求&,
                           const 世界树成员移动请求&) = default;
};
struct 世界树移动投影 final {
    稳定编码 成员{}, 原场景{}, 目标场景{}, 新关系{};
};
struct 世界树移动结果 final {
    世界树结果头 结果头;
    std::optional<世界树成员移动请求> 原请求;
    std::optional<世界树移动投影> 投影;
    bool 成功(const 世界树成员移动请求&) const noexcept;
};

struct 世界树场景创建请求 final {
    稳定编码 父场景{};
    纯概念创建请求 概念;
    绑定存在创建请求 世界;
    friend bool operator==(const 世界树场景创建请求&,
                           const 世界树场景创建请求&) = default;
};
struct 世界树存在创建请求 final {
    存在初始绑定 绑定;
    纯概念创建请求 概念;
    绑定存在创建请求 世界;
    friend bool operator==(const 世界树存在创建请求&,
                           const 世界树存在创建请求&) = default;
};
using 世界树创建原请求 =
    std::variant<世界树场景创建请求, 世界树存在创建请求>;

enum class 世界树概念创建阶段 : std::uint8_t {
    无 = 0,
    输入定位 = 1,
    概念查询 = 2,
    概念发布 = 3,
    世界发布 = 4,
    最终读回 = 5
};
enum class 世界树概念创建状态 : std::uint8_t {
    已完成 = 1,
    精确重复 = 2,
    入口拒绝 = 3,
    位置失败 = 4,
    概念失败 = 5,
    世界失败 = 6,
    角色冲突 = 7,
    幂等冲突 = 9,
    资源失败 = 12,
    内部不一致 = 13,
    已发布待复核 = 14,
    已可能发布 = 15,
    后继覆盖 = 16,
    未实现 = 17
};
struct 世界树概念创建投影 final {
    稳定编码 E{}, 世界根{};
    世界树层级位置 位置;
    已发布概念绑定投影 内容;
};
struct 世界树概念创建结果 final {
    世界树概念创建状态 状态 = 世界树概念创建状态::入口拒绝;
    世界树概念创建阶段 阶段 = 世界树概念创建阶段::无;
    std::optional<纯概念查询结果> 概念查询;
    std::optional<纯概念写入结果> 概念写入;
    std::optional<已发布概念绑定创建结果> 世界写入;
    std::optional<世界树操作状态> 位置原因;
    std::optional<世界树创建原请求> 请求回显;
    std::optional<世界树概念创建投影> 投影;
    bool 成功(const 世界树场景创建请求&) const noexcept;
    bool 成功(const 世界树存在创建请求&) const noexcept;
};

struct 世界树存在信息读取请求 final {
    稳定编码 E{};
    世界树节点视角 视角 = 世界树节点视角::存在;
};
struct 世界树存在信息结果 final {
    世界树概念创建状态 状态 = 世界树概念创建状态::入口拒绝;
    std::optional<世界树概念创建投影> 投影;
    std::optional<世界树操作状态> 位置原因;
    std::optional<纯概念状态> 概念原因;
    bool 成功(const 世界树存在信息读取请求&) const noexcept;
};

class 世界树应用服务 final {
public:
    世界树应用服务() = delete;
    世界树应用服务(const 世界树应用服务&) = delete;
    世界树应用服务& operator=(const 世界树应用服务&) = delete;
    世界树应用服务(世界树应用服务&&) = delete;
    世界树应用服务& operator=(世界树应用服务&&) = delete;

    世界树根验证结果 验证现实世界根(
        const 世界树根验证请求&) const noexcept;
    世界树根验证结果 读取当前现实世界根() const noexcept;
    世界树层级位置结果 读取世界树层级位置(
        const 世界树层级位置读取请求&) const noexcept;
    世界树存在位置结果 读取现实世界存在位置(
        const 世界树存在位置读取请求&) const noexcept;
    世界树节点组查询结果 查询父节点(
        const 世界树节点查询请求&) const noexcept;
    世界树节点组查询结果 查询子节点(
        const 世界树节点查询请求&) const noexcept;
    世界树节点组查询结果 查找兄弟节点(
        const 世界树节点查询请求&) const noexcept;

    世界树移动结果 移动现实世界存在(
        const 世界树成员移动请求&) noexcept;
    世界树移动结果 移动现实世界场景(
        const 世界树成员移动请求&) noexcept;
    世界树概念创建结果 创建场景并纳入现实世界(
        const 世界树场景创建请求&) noexcept;
    世界树概念创建结果 创建存在并纳入现实世界(
        const 世界树存在创建请求&) noexcept;
    世界树存在信息结果 读取世界存在信息(
        const 世界树存在信息读取请求&) const noexcept;

    bool 使用存在提供者(const 存在类数据服务&) const noexcept;
    bool 使用场景提供者(const 场景类数据服务&) const noexcept;
    bool 使用概念提供者(const 概念树类数据服务&) const noexcept;
    稳定编码 世界根() const noexcept;

private:
    struct 已验证世界根令牌 final {};
    世界树应用服务(场景类数据服务&, 存在类数据服务&,
                   概念树类数据服务&, 稳定编码,
                   已验证世界根令牌);
    friend struct 世界树应用服务建立结果;
    friend 世界树应用服务建立结果 建立世界树应用服务(
        场景类数据服务&, 存在类数据服务&,
        概念树类数据服务&, 稳定编码) noexcept;

    世界树移动结果 移动(
        const 世界树成员移动请求&, bool 场景移动) noexcept;
    std::optional<场景树当前事实> 读取树() const;

    场景类数据服务& scene_;
    存在类数据服务& existence_;
    概念树类数据服务& concept_;
    直接归属联合只读组合器 joint_;
    稳定编码 root_{};
};

enum class 世界树应用服务建立状态 : std::uint8_t {
    已建立 = 1,
    入口拒绝 = 2,
    根未找到 = 3,
    提供者不一致 = 4,
    资源失败 = 5,
    内部不一致 = 6
};
struct 世界树应用服务建立结果 final {
    世界树应用服务建立状态 状态 = 世界树应用服务建立状态::入口拒绝;
    稳定编码 根{};
    std::unique_ptr<世界树应用服务> 服务;
    bool 成功() const noexcept;
};

世界树应用服务建立结果 建立世界树应用服务(
    场景类数据服务&, 存在类数据服务&,
    概念树类数据服务&, 稳定编码 世界根) noexcept;

} // namespace 海中鱼巣
