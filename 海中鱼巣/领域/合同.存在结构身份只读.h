#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/服务.L1事实基座.h"

namespace 海中鱼巣 {

enum class 存在结构身份只读状态 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  资源失败 = 7,
  内部不一致 = 8,
  未实现 = 9
};

struct 存在当前身份确认结果 final {
  存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
  bool 成功() const noexcept;
};

struct 存在身份来源当前见证 final {
  稳定编码 身份{};
  稳定编码 族锚点{};
  稳定编码 族归属关系类型{};
  稳定编码 族归属关系{};
  std::uint64_t 角色 = 0;
};

bool 存在身份来源当前见证完整(
    const 存在身份来源当前见证 &, 稳定编码 请求身份) noexcept;

struct 存在身份来源当前见证读取结果 final {
  存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
  std::optional<存在身份来源当前见证> 见证;
  bool 成功(稳定编码 请求身份) const noexcept;
};

enum class 存在已知准确特征只读状态 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  资源失败 = 7,
  内部不一致 = 8,
  未实现 = 9
};

struct 存在已知准确特征当前请求 final { 稳定编码 存在{}, 特征{}; };

struct 存在已知准确特征见证 final { 稳定编码 已知关系{}, 特征{}; };

bool 存在已知准确特征见证完整(
    const 存在已知准确特征见证 &, 稳定编码 请求特征) noexcept;

struct 存在已知准确特征读取结果 final {
  存在已知准确特征只读状态 状态 = 存在已知准确特征只读状态::入口拒绝;
  稳定编码 存在{}, 特征{};
  std::optional<存在已知准确特征见证> 见证;
  bool 当前成功(const 存在已知准确特征当前请求 &) const noexcept;
};

struct 存在组成关系事实 final { 稳定编码 关系{}, 父存在{}, 子存在{}; };
struct 存在组成父读取请求 final { 稳定编码 子存在{}; };
struct 存在组成子组读取请求 final { 稳定编码 父存在{}; };

struct 存在组成读取结果 final {
  存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
  std::optional<存在组成关系事实> 父;
  std::vector<存在组成关系事实> 子组;
  bool 父读取成功(const 存在组成父读取请求 &) const noexcept;
  bool 子组读取成功(const 存在组成子组读取请求 &) const noexcept;
};

class 存在组成结构只读提供者 {
public:
  virtual ~存在组成结构只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 存在组成读取结果
  读取当前组成父(const 存在组成父读取请求 &) const = 0;
  virtual 存在组成读取结果
  读取当前组成子组(const 存在组成子组读取请求 &) const = 0;
};

enum class 直接归属来源 : std::uint8_t {
  存在组成 = 1,
  场景成员 = 2,
  直接子场景 = 3
};

enum class 直接归属联合只读状态 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  成员未找到 = 3,
  资源失败 = 8,
  内部不一致 = 9
};

struct 直接归属联合事实 final {
  直接归属来源 来源 = 直接归属来源::存在组成;
  稳定编码 关系{}, 父{}, 成员{};
};

struct 直接归属联合父读取请求 final { 稳定编码 成员{}; };
struct 直接归属联合子组读取请求 final { 稳定编码 父{}; };

struct 直接归属联合读取结果 final {
  直接归属联合只读状态 状态 = 直接归属联合只读状态::入口拒绝;
  std::optional<直接归属联合事实> 父;
  std::vector<直接归属联合事实> 子组;
  bool 父读取成功(const 直接归属联合父读取请求 &) const noexcept;
  bool 子组读取成功(const 直接归属联合子组读取请求 &) const noexcept;
};

enum class 直接归属场景位置 : std::uint8_t {
  未纳入场景树 = 1,
  场景树根 = 2,
  场景树非根 = 3
};

struct 直接归属场景角色事实 final {
  稳定编码 场景{};
  直接归属场景位置 位置 = 直接归属场景位置::未纳入场景树;
  std::optional<稳定编码> 树根, 树证明关系;
};

struct 直接归属场景角色读取请求 final { 稳定编码 对象{}; };

struct 直接归属场景角色读取结果 final {
  直接归属联合只读状态 状态 = 直接归属联合只读状态::入口拒绝;
  std::optional<直接归属场景角色事实> 角色;
  bool 成功(const 直接归属场景角色读取请求 &) const noexcept;
};

class 直接归属联合只读提供者 {
public:
  virtual ~直接归属联合只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 直接归属联合读取结果
  读取当前联合父(const 直接归属联合父读取请求 &) const = 0;
  virtual 直接归属联合读取结果
  读取当前联合子组(const 直接归属联合子组读取请求 &) const = 0;
  virtual 直接归属场景角色读取结果
  读取当前场景角色位置(const 直接归属场景角色读取请求 &) const = 0;
};

class 存在结构身份只读提供者 {
public:
  virtual ~存在结构身份只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 存在当前身份确认结果
  确认当前存在结构身份(稳定编码 身份) const = 0;
  virtual 存在身份来源当前见证读取结果
  读取当前存在身份来源见证(稳定编码 身份) const = 0;
  virtual 存在已知准确特征读取结果
  确认当前已知准确特征(const 存在已知准确特征当前请求 &) const = 0;
};

} // namespace 海中鱼巣
