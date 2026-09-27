#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "合同.场景角色组织.h"

namespace 海中鱼巣 {

class 绑定存在数据服务;

class 已发布概念引用参与者 {
  friend class 绑定存在数据服务;

private:
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;

public:
  virtual ~已发布概念引用参与者() = default;
};

enum class 存在初始绑定种类 : std::uint8_t {
  父存在组成 = 1,
  场景成员 = 2,
  直接子场景 = 3
};

struct 存在初始绑定 final {
  存在初始绑定种类 种类 = 存在初始绑定种类::父存在组成;
  稳定编码 绑定节点{};
  friend bool operator==(const 存在初始绑定 &,
                         const 存在初始绑定 &) = default;
};

struct 存在组成绑定创建键 final {
  L1所有者范围写入幂等身份 幂等身份{};
  friend bool operator==(const 存在组成绑定创建键 &,
                         const 存在组成绑定创建键 &) = default;
};

struct 存在场景绑定创建键 final {
  L1所有者范围写入幂等身份 组合幂等身份{};
  L1所有者范围写入幂等身份 存在幂等身份{};
  L1所有者范围写入幂等身份 场景幂等身份{};
  friend bool operator==(const 存在场景绑定创建键 &,
                         const 存在场景绑定创建键 &) = default;
};

using 绑定存在创建键 =
    std::variant<存在组成绑定创建键, 存在场景绑定创建键>;

struct 存在单例角色身份 final {
  稳定编码 值{};
  friend bool operator==(const 存在单例角色身份 &,
                         const 存在单例角色身份 &) = default;
};

struct 已发布概念身份 final {
  稳定编码 编码{};
  friend bool operator==(const 已发布概念身份 &,
                         const 已发布概念身份 &) = default;
};

struct 绑定存在创建请求 final {
  存在初始绑定 绑定;
  稳定编码 期望现实树根{};
  绑定存在创建键 幂等键;
  已发布概念身份 概念;
  std::optional<存在单例角色身份> 初始角色;
  friend bool operator==(const 绑定存在创建请求 &,
                         const 绑定存在创建请求 &) = default;
};

using 已发布概念绑定创建请求 = 绑定存在创建请求;

enum class 绑定存在创建状态 : std::uint8_t {
  已创建 = 1,
  精确重复 = 2,
  入口拒绝 = 3,
  绑定未明确 = 4,
  绑定未找到 = 5,
  绑定类型不符 = 7,
  绑定不在现实树 = 8,
  包含冲突 = 9,
  成环 = 10,
  幂等冲突 = 12,
  资源失败 = 15,
  内部不一致 = 16,
  已可能发布 = 17,
  未实现 = 18
};

struct 绑定存在事实 final {
  存在初始绑定种类 种类 = 存在初始绑定种类::父存在组成;
  稳定编码 绑定节点{}, 新存在{}, 绑定关系{};
  friend bool operator==(const 绑定存在事实 &,
                         const 绑定存在事实 &) = default;
};

struct 存在单例角色结构交付 final {
  稳定编码 角色登记类型{}, 角色目标类型{};
  存在单例角色身份 项目角色;
  friend bool operator==(const 存在单例角色结构交付 &,
                         const 存在单例角色结构交付 &) = default;
};

enum class 存在单例角色状态 : std::uint8_t {
  已登记 = 1,
  已读取 = 2,
  未绑定 = 3,
  精确重复 = 4,
  入口拒绝 = 5,
  未找到 = 6,
  角色冲突 = 8,
  幂等冲突 = 10,
  资源失败 = 13,
  内部不一致 = 14,
  已可能发布 = 15,
  未实现 = 16
};

struct 存在单例角色结构登记请求 final {
  L1所有者范围写入幂等身份 幂等身份;
};

struct 存在单例角色结构登记结果 final {
  存在单例角色状态 状态 = 存在单例角色状态::入口拒绝;
  std::optional<存在单例角色结构交付> 交付;
  bool 成功(const 存在单例角色结构登记请求 &) const noexcept;
};

struct 存在单例角色读取请求 final { 存在单例角色身份 角色; };

struct 存在单例角色事实 final {
  存在单例角色身份 角色;
  稳定编码 登记关系{}, 目标关系{}, 存在{};
  存在身份来源当前见证 存在身份;
  friend bool operator==(const 存在单例角色事实 &,
                         const 存在单例角色事实 &) = default;
};

struct 存在单例角色读取结果 final {
  存在单例角色状态 状态 = 存在单例角色状态::入口拒绝;
  std::optional<存在单例角色事实> 事实;
  bool 成功(const 存在单例角色读取请求 &) const noexcept;
  bool 确认未绑定(const 存在单例角色读取请求 &) const noexcept;
};

struct 已发布概念绑定投影 final {
  绑定存在事实 绑定;
  存在身份来源当前见证 存在身份;
  已发布概念身份 概念;
  std::optional<场景树节点当前事实> 场景;
  std::optional<存在单例角色事实> 角色;
  friend bool operator==(const 已发布概念绑定投影 &,
                         const 已发布概念绑定投影 &) = default;
};

enum class 已发布概念绑定状态 : std::uint8_t {
  已创建 = 1,
  精确重复 = 2,
  已读取 = 3,
  未派发 = 4,
  入口拒绝 = 5,
  绑定失败 = 6,
  概念失败 = 7,
  角色冲突 = 8,
  幂等冲突 = 10,
  资源失败 = 13,
  内部不一致 = 14,
  已可能发布 = 15,
  当前事实不再匹配 = 16,
  未实现 = 17
};

struct 已发布概念绑定创建结果 final {
  已发布概念绑定状态 状态 = 已发布概念绑定状态::入口拒绝;
  std::optional<绑定存在创建状态> 绑定原因;
  std::optional<绑定存在创建请求> 原请求;
  std::optional<已发布概念绑定投影> 投影;
  bool 成功(const 绑定存在创建请求 &) const noexcept;
};

template <class T> struct 绑定存在参与者结果 final {
  绑定存在创建状态 状态 = 绑定存在创建状态::内部不一致;
  std::optional<T> 数据;
};

struct 存在绑定出生见证 final {
  稳定编码 新存在{};
  存在身份来源当前见证 存在身份;
  std::optional<存在组成关系事实> 组成绑定关系;
};

struct 场景绑定出生见证 final {
  存在初始绑定种类 种类 = 存在初始绑定种类::场景成员;
  稳定编码 绑定节点{}, 新存在{};
  场景直接包含事实 绑定关系;
  std::optional<场景树节点当前事实> 子场景结构;
};

struct 绑定现实树见证 final {
  稳定编码 绑定节点{}, 期望现实树根{};
  直接归属场景角色事实 根角色;
  std::vector<直接归属联合事实> 上行路径;
};

class 绑定存在内容参与者 {
  friend class 绑定存在数据服务;

private:
  virtual const L1事实基座服务 &绑定存在底座() const noexcept = 0;
  virtual L1所有者范围写端口 &绑定存在端口() noexcept = 0;
  virtual bool 绑定存在结构已就绪() const noexcept = 0;
  virtual bool 绑定存在幂等键可用(
      L1所有者范围写入幂等身份) const noexcept = 0;
  virtual const 存在组成结构只读提供者 &
  绑定存在组成提供者() const noexcept = 0;
  virtual 绑定存在参与者结果<L1有限N分区原子参与者写集>
  准备存在出生片段(const 绑定存在创建请求 &,
                   const std::optional<存在单例角色身份> &) const = 0;
  virtual 存在单例角色读取结果
  读取绑定单例角色(const 存在单例角色读取请求 &) const noexcept = 0;
  virtual 绑定存在参与者结果<L1所有者范围首次写入读取结果>
  读取存在出生首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual 绑定存在参与者结果<存在绑定出生见证>
  读取存在绑定出生(稳定编码 新存在,
                   const 存在初始绑定 &) const = 0;

public:
  virtual ~绑定存在内容参与者() = default;
};

class 绑定存在场景参与者 {
  friend class 绑定存在数据服务;

private:
  virtual const L1事实基座服务 &绑定存在底座() const noexcept = 0;
  virtual L1所有者范围写端口 &绑定存在端口() noexcept = 0;
  virtual bool 绑定存在结构已就绪() const noexcept = 0;
  virtual bool 绑定存在幂等键可用(
      L1所有者范围写入幂等身份) const noexcept = 0;
  virtual const 场景直接包含只读提供者 &
  绑定存在场景提供者() const noexcept = 0;
  virtual 绑定存在参与者结果<绑定现实树见证>
  核验绑定现实树(const 存在初始绑定 &, 稳定编码 期望现实树根,
                 const 直接归属联合只读提供者 &) const = 0;
  virtual 绑定存在参与者结果<L1有限N分区原子参与者写集>
  准备场景绑定片段(const 绑定存在创建请求 &) const = 0;
  virtual 绑定存在参与者结果<L1所有者范围首次写入读取结果>
  读取场景绑定首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual 绑定存在参与者结果<场景绑定出生见证>
  读取场景绑定出生(稳定编码 新存在,
                   const 存在初始绑定 &) const = 0;

public:
  virtual ~绑定存在场景参与者() = default;
};

class 绑定存在数据服务 final {
public:
  绑定存在数据服务(绑定存在内容参与者 &,
                   绑定存在场景参与者 &);

  bool 使用存在提供者(const 绑定存在内容参与者 &) const noexcept;
  bool 使用场景提供者(const 绑定存在场景参与者 &) const noexcept;

  已发布概念绑定创建结果
  创建绑定存在并引用概念(const 绑定存在创建请求 &,
                         已发布概念引用参与者 &) noexcept;

private:
  static bool 请求形状有效(const 绑定存在创建请求 &) noexcept;

  绑定存在内容参与者 &存在参与者_;
  绑定存在场景参与者 &场景参与者_;
  直接归属联合只读组合器 联合提供者_;
};

} // namespace 海中鱼巣
