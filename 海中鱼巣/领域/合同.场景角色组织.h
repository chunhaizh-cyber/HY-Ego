#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

#include "合同.场景状态使用绑定.h"
#include "合同.动态结构.h"

namespace 海中鱼巣 {

enum class 场景根角色 : std::uint8_t { 特征 = 1, 状态 = 2, 动态 = 3, 关系 = 4 };

enum class 场景角色数据状态 : std::uint8_t {
  已登记 = 1, 已启用 = 2, 已组织 = 3, 已读取 = 4, 已删除 = 5,
  精确重复 = 6, 入口拒绝 = 7, 未找到 = 8, 场景角色未启用 = 10,
  场景角色已存在 = 11, 实例未组织 = 12, 实例已组织 = 13,
  引用冲突 = 14, 幂等冲突 = 16, 资源失败 = 20, 内部不一致 = 21,
  已可能发布 = 22, 未实现 = 23
};

struct 场景节点见证 final { 稳定编码 编码{}; };
struct 场景组织边见证 final {
  稳定编码 编码{}, 源{}, 目标{}, 关系类型{};
  std::uint64_t 角色或顺序 = 0;
};
struct 场景根事实 final {
  场景根角色 角色 = 场景根角色::特征;
  场景节点见证 根;
  场景组织边见证 绑定;
};
struct 场景角色当前事实 final {
  稳定编码 场景{};
  存在身份来源当前见证 对象存在来源;
  场景节点见证 场景族锚点, 场景族归属类型, 根绑定类型;
  场景组织边见证 场景角色登记边;
  std::array<场景根事实, 4> 四根;
};
struct 场景父语境投影事实 final {
  稳定编码 场景{}, 父场景语境{};
  直接归属联合事实 结构父;
  场景组织边见证 投影边;
};
struct 场景父语境读取请求 final { 稳定编码 场景{}; };
struct 场景父语境读取结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景父语境投影事实> 投影;
  bool 成功(const 场景父语境读取请求 &) const noexcept;
};

struct 场景当前身份请求 final { 稳定编码 场景{}; };
struct 场景角色当前读取请求 final { 稳定编码 场景{}; };
struct 场景当前身份结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  bool 成功(const 场景当前身份请求 &) const noexcept;
};
struct 场景角色当前读取结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景角色当前事实> 角色;
  bool 成功(const 场景角色当前读取请求 &) const noexcept;
};
struct 场景组织当前读取请求 final {
  场景根角色 角色 = 场景根角色::状态;
  稳定编码 实例{};
};
struct 场景实例组织事实 final {
  场景角色当前事实 场景角色;
  场景根角色 角色 = 场景根角色::状态;
  稳定编码 根{}, 实例{};
  std::vector<场景节点见证> 路径节点;
  std::vector<场景组织边见证> 路径边;
};
struct 场景组织当前读取结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景实例组织事实> 组织;
  bool 成功(const 场景组织当前读取请求 &) const noexcept;
};
struct 场景动态组织当前读取请求 final { 动态信息身份 动态; };
using 场景动态组织事实 = 场景实例组织事实;
struct 场景动态组织当前读取结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景动态组织事实> 组织;
  bool 成功(const 场景动态组织当前读取请求 &) const noexcept;
};

class 场景动态组织只读提供者 {
public:
  virtual ~场景动态组织只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 场景当前身份结果 确认当前场景角色(const 场景当前身份请求 &) const = 0;
  virtual 场景角色当前读取结果 读取当前场景角色(const 场景角色当前读取请求 &) const = 0;
  virtual 场景父语境读取结果 读取当前父场景语境(const 场景父语境读取请求 &) const = 0;
  virtual 场景动态组织当前读取结果 读取当前动态场景组织(const 场景动态组织当前读取请求 &) const = 0;
};

struct 场景角色结构交付 final {
  状态使用绑定结构交付 绑定结构;
  稳定编码 根绑定关系类型{}, 状态组织关系类型{}, 动态组织关系类型{}, 父场景语境关系类型{};
};
struct 场景结构登记请求 final { L1所有者范围写入幂等身份 幂等身份; };
struct 场景结构登记结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景角色结构交付> 交付;
  bool 成功(const 场景结构登记请求 &) const noexcept;
};
struct 场景角色启用请求 final {
  L1所有者范围写入幂等身份 幂等身份;
  稳定编码 对象存在{}, 父场景语境{}, 预期结构父关系{};
};
struct 场景角色退出请求 final {
  L1所有者范围写入幂等身份 幂等身份;
  稳定编码 场景{}, 父场景语境{}, 预期结构父关系{};
};
struct 场景角色写结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景角色当前事实> 角色;
  std::optional<直接归属联合事实> 结构父;
  std::optional<场景父语境投影事实> 父语境投影;
  bool 启用成功(const 场景角色启用请求 &) const noexcept;
  bool 退出成功(const 场景角色退出请求 &) const noexcept;
};
struct 场景状态组织请求 final {
  L1所有者范围写入幂等身份 幂等身份;
  稳定编码 场景{}, 组织父{};
  状态信息身份 状态;
};
struct 场景动态组织请求 final {
  L1所有者范围写入幂等身份 幂等身份;
  稳定编码 场景{}, 组织父{};
  动态信息身份 动态;
};
struct 场景实例组织回执 final {
  稳定编码 场景{}, 根{}, 实例{};
  场景根角色 角色 = 场景根角色::状态;
  场景组织边见证 直接组织边;
};
struct 场景组织写结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景实例组织回执> 组织;
  bool 状态组织成功(const 场景状态组织请求 &) const noexcept;
  bool 动态组织成功(const 场景动态组织请求 &) const noexcept;
};

inline constexpr L1所有者范围写入幂等身份 场景特征组织扩展登记固定幂等身份{0x5343'454E'4546'4F52ULL};
struct 场景特征组织扩展结构交付 final { 稳定编码 特征组织关系类型{}; };
struct 场景特征组织扩展登记请求 final {};
struct 场景特征组织扩展登记结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景特征组织扩展结构交付> 交付;
  bool 成功(const 场景特征组织扩展登记请求 &) const noexcept;
};
struct 场景特征组织请求 final {
  L1所有者范围写入幂等身份 幂等身份;
  稳定编码 场景{}, 组织父{};
  特征信息身份 特征;
};
struct 场景特征组织当前读取请求 final { 特征信息身份 特征; };
using 场景特征组织事实 = 场景实例组织事实;
struct 场景特征组织当前读取结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景特征组织事实> 组织;
  bool 成功(const 场景特征组织当前读取请求 &) const noexcept;
};
struct 场景特征组织写结果 final {
  场景角色数据状态 状态 = 场景角色数据状态::入口拒绝;
  std::optional<场景实例组织回执> 组织;
  bool 成功(const 场景特征组织请求 &) const noexcept;
};

inline constexpr L1所有者范围写入幂等身份 场景直接包含扩展登记固定幂等身份{0x5343'454E'434F'4E54ULL};
enum class 场景直接包含状态 : std::uint8_t {
  已登记 = 1, 已读取 = 2, 已新增 = 3, 已删除 = 4, 已迁移 = 5,
  精确重复 = 6, 入口拒绝 = 7, 场景未找到 = 8, 成员未找到 = 10,
  成员未归属 = 12, 成员已归属 = 13, 成员多重归属 = 14,
  原父不匹配 = 15, 目标父相同 = 16, 形成场景环 = 17,
  引用冲突 = 18, 幂等冲突 = 20, 资源失败 = 23, 内部不一致 = 24,
  已可能发布 = 25, 已建立场景树根 = 26, 已启用并纳入场景树 = 27,
  未实现 = 28
};
struct 场景直接包含扩展结构交付 final {
  稳定编码 树登记锚点{}, 根标记关系类型{}, 树归属关系类型{},
      直接存在成员关系类型{}, 直接子场景关系类型{};
};
struct 场景直接包含扩展登记请求 final {};
struct 场景直接包含扩展登记结果 final {
  场景直接包含状态 状态 = 场景直接包含状态::入口拒绝;
  std::optional<场景直接包含扩展结构交付> 交付;
  bool 成功(const 场景直接包含扩展登记请求 &) const noexcept;
};
enum class 场景直接包含种类 : std::uint8_t { 存在成员 = 1, 子场景 = 2 };
struct 场景直接包含事实 final {
  场景直接包含种类 种类 = 场景直接包含种类::存在成员;
  稳定编码 父场景{}, 成员{};
  场景组织边见证 关系;
};
enum class 场景树证明种类 : std::uint8_t { 根标记 = 1, 树归属 = 2 };
struct 场景树证明事实 final {
  场景树证明种类 种类 = 场景树证明种类::根标记;
  稳定编码 关系{}, 场景{}, 树根{};
  场景组织边见证 见证;
};
struct 场景直接包含写请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 父场景{}, 成员{};
  friend bool operator==(const 场景直接包含写请求 &, const 场景直接包含写请求 &) noexcept = default;
};
struct 场景直接包含迁移请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 原父场景{}, 目标父场景{}, 成员{};
  friend bool operator==(const 场景直接包含迁移请求 &, const 场景直接包含迁移请求 &) noexcept = default;
};
struct 场景树根启用请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 对象存在{};
  friend bool operator==(const 场景树根启用请求 &, const 场景树根启用请求 &) noexcept = default;
};
struct 场景直接子场景启用请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 对象存在{}, 父场景{};
  friend bool operator==(const 场景直接子场景启用请求 &, const 场景直接子场景启用请求 &) noexcept = default;
};
struct 场景直接包含组读取请求 final { 稳定编码 父场景{}; };
struct 场景直接包含反向读取请求 final { 稳定编码 成员{}; };
struct 场景直接包含当前读取请求 final { 稳定编码 关系{}; };
struct 场景直接包含结果头 final { 场景直接包含状态 状态 = 场景直接包含状态::入口拒绝; };
struct 场景直接包含单项结果 final {
  场景直接包含结果头 结果头;
  std::optional<场景直接包含事实> 包含;
  bool 成功() const noexcept;
};
struct 场景直接包含迁移结果 final {
  场景直接包含结果头 结果头;
  std::optional<场景直接包含事实> 已建立新包含;
  std::optional<场景父语境投影事实> 已建立新父语境;
  bool 成功(const 场景直接包含迁移请求 &) const noexcept;
};
struct 场景树角色写结果 final {
  场景直接包含结果头 结果头;
  std::optional<场景角色当前事实> 场景角色;
  std::optional<场景树证明事实> 树证明;
  std::optional<稳定编码> 最近上游场景;
  std::optional<直接归属联合事实> 既有直接父;
  std::optional<场景直接包含事实> 新直接包含;
  std::optional<场景父语境投影事实> 父语境投影;
  bool 建根成功(const 场景树根启用请求 &) const noexcept;
  bool 子场景纳入成功(const 场景直接子场景启用请求 &) const noexcept;
};
struct 场景直接包含组结果 final {
  场景直接包含结果头 结果头;
  std::vector<场景直接包含事实> 包含组;
  bool 父组读取成功(const 场景直接包含反向读取请求 &) const noexcept;
  bool 子组读取成功(const 场景直接包含组读取请求 &) const noexcept;
};
struct 场景树节点当前事实 final {
  场景角色当前事实 场景角色;
  场景树证明事实 树证明;
  std::optional<直接归属联合事实> 直接父;
  std::vector<场景直接包含事实> 直接存在成员组, 直接子场景组;
  std::vector<直接归属联合事实> 从上游场景到本场景路径;
  std::optional<场景父语境投影事实> 父语境投影;
};
struct 场景树当前读取请求 final { 稳定编码 根场景{}; };
struct 场景树当前事实 final {
  稳定编码 根场景{};
  std::vector<场景树节点当前事实> 场景组;
};
struct 场景树当前结果 final {
  场景直接包含结果头 结果头;
  std::optional<场景树当前事实> 树;
  bool 成功(const 场景树当前读取请求 &) const noexcept;
};

class 场景直接包含只读提供者 {
public:
  virtual ~场景直接包含只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 场景直接包含组结果 读取当前场景包含父组(const 场景直接包含反向读取请求 &) const = 0;
  virtual 场景直接包含组结果 读取当前场景包含子组(const 场景直接包含组读取请求 &) const = 0;
  virtual 直接归属场景角色读取结果 读取当前场景角色位置(const 直接归属场景角色读取请求 &) const = 0;
};

class 直接归属联合只读组合器 final : public 直接归属联合只读提供者 {
public:
  直接归属联合只读组合器(const 存在组成结构只读提供者 &, const 场景直接包含只读提供者 &) noexcept;
  bool 绑定于(const L1事实基座服务 &) const noexcept override;
  直接归属联合读取结果 读取当前联合父(const 直接归属联合父读取请求 &) const override;
  直接归属联合读取结果 读取当前联合子组(const 直接归属联合子组读取请求 &) const override;
  直接归属场景角色读取结果 读取当前场景角色位置(const 直接归属场景角色读取请求 &) const override;

private:
  const 存在组成结构只读提供者 &存在提供者_;
  const 场景直接包含只读提供者 &场景提供者_;
};

bool 联合父载荷完整(const 直接归属联合事实 &, 稳定编码 父, 稳定编码 成员) noexcept;
bool 场景直接包含事实完整(const 场景直接包含事实 &) noexcept;

static_assert(static_cast<std::uint8_t>(场景根角色::特征) == 1);
static_assert(static_cast<std::uint8_t>(场景根角色::状态) == 2);
static_assert(static_cast<std::uint8_t>(场景根角色::动态) == 3);
static_assert(static_cast<std::uint8_t>(场景根角色::关系) == 4);

} // namespace 海中鱼巣
