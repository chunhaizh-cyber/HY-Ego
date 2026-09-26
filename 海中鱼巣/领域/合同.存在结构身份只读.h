#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/服务.L1事实基座.h"


namespace 海中鱼巣 {

enum class 存在结构身份只读状态 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  事实代次漂移 = 5,
  资源失败 = 7,
  内部不一致 = 8,
  未实现 = 9,
  数量预算不足 = 10
};

inline constexpr std::uint32_t 存在结构身份只读合同版本 = 2;

struct 存在当前身份确认结果 final {
  存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
  std::uint32_t 合同版本 = 存在结构身份只读合同版本;
  std::uint64_t Gread = 0;

  bool 成功(std::uint64_t 请求G0) const noexcept {
    return 状态 == 存在结构身份只读状态::已读取 &&
           合同版本 == 存在结构身份只读合同版本 && 请求G0 != 0 &&
           Gread == 请求G0;
  }
};

struct 存在身份来源当前见证 final {
  稳定编码 身份{};
  稳定编码 族锚点{};
  稳定编码 族归属关系类型{};
  稳定编码 族归属关系{};
  std::uint64_t 角色 = 0;
  std::uint64_t 节点创建事实代次 = 0;
  std::uint64_t 族锚点创建事实代次 = 0;
  std::uint64_t 关系类型创建事实代次 = 0;
  std::uint64_t 归属关系创建事实代次 = 0;
};

inline bool 存在身份来源当前见证完整(const 存在身份来源当前见证 &v,
                                     std::uint64_t Gread,
                                     稳定编码 请求身份) noexcept {
  if (Gread == 0 || !有效(请求身份) || v.身份 != 请求身份 || v.角色 != 1)
    return false;

  const 稳定编码 编码组[]{v.身份, v.族锚点, v.族归属关系类型, v.族归属关系};
  for (std::size_t i = 0; i < 4; ++i) {
    if (!有效(编码组[i]))
      return false;
    for (std::size_t j = i + 1; j < 4; ++j)
      if (编码组[i] == 编码组[j])
        return false;
  }

  const std::uint64_t 创建事实代次组[]{v.节点创建事实代次,
      v.族锚点创建事实代次, v.关系类型创建事实代次,
      v.归属关系创建事实代次};
  for (const auto 创建事实代次 : 创建事实代次组)
    if (创建事实代次 == 0 || 创建事实代次 > Gread)
      return false;

  return v.节点创建事实代次 == v.归属关系创建事实代次 &&
         v.族锚点创建事实代次 <= v.节点创建事实代次 &&
         v.关系类型创建事实代次 <= v.节点创建事实代次;
}

struct 存在身份来源当前见证读取结果 final {
  存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
  std::uint32_t 合同版本 = 存在结构身份只读合同版本;
  std::uint64_t Gread = 0;
  std::optional<存在身份来源当前见证> 见证;

  bool 成功(std::uint64_t 请求Gread,
            稳定编码 请求身份) const noexcept {
    return 状态 == 存在结构身份只读状态::已读取 &&
           合同版本 == 存在结构身份只读合同版本 && 请求Gread != 0 &&
           有效(请求身份) && Gread == 请求Gread && 见证 &&
           存在身份来源当前见证完整(*见证, 请求Gread, 请求身份);
  }
};

enum class 存在已知准确特征只读状态 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  事实代次漂移 = 5,
  资源失败 = 7,
  内部不一致 = 8,
  未实现 = 9,
  数量预算不足 = 10
};

inline constexpr std::uint32_t 存在已知准确特征只读合同版本 = 1;

struct 存在已知准确特征当前请求 final {
  std::uint32_t 版本 = 存在已知准确特征只读合同版本;
  std::uint64_t G0 = 0;
  稳定编码 存在{}, 特征{};
};

struct 存在已知准确特征见证 final {
  稳定编码 已知关系{}, 特征{};
  std::uint64_t 特征创建事实代次 = 0;
};

inline bool 存在已知准确特征见证完整(const 存在已知准确特征见证 &v,
                                     std::uint64_t Gread,
                                     稳定编码 请求特征) noexcept {
  return Gread != 0 && 有效(请求特征) && 有效(v.已知关系) && 有效(v.特征) &&
         v.已知关系 != v.特征 && v.特征 == 请求特征 &&
         v.特征创建事实代次 != 0 && v.特征创建事实代次 <= Gread;
}

struct 存在已知准确特征读取结果 final {
  存在已知准确特征只读状态 状态 = 存在已知准确特征只读状态::入口拒绝;
  std::uint32_t 合同版本 = 存在已知准确特征只读合同版本;
  std::uint64_t Gread = 0;
  稳定编码 存在{}, 特征{};
  std::optional<存在已知准确特征见证> 见证;

  bool 当前成功(const 存在已知准确特征当前请求 &r) const noexcept {
    return r.版本 == 存在已知准确特征只读合同版本 && r.G0 != 0 &&
           有效(r.存在) && 有效(r.特征) && r.存在 != r.特征 &&
           合同版本 == 存在已知准确特征只读合同版本 &&
           状态 == 存在已知准确特征只读状态::已读取 && Gread == r.G0 &&
           存在 == r.存在 && 特征 == r.特征 && 见证 &&
           存在已知准确特征见证完整(*见证, Gread, r.特征);
  }
};

struct 存在组成关系事实 final {
  std::uint64_t Gread = 0;
  稳定编码 关系{}, 父存在{}, 子存在{};
  std::uint64_t 创建事实代次 = 0;
};

struct 存在组成父读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  稳定编码 子存在{};
  std::uint64_t 最大关系数量 = 0;
};

struct 存在组成子组读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  稳定编码 父存在{};
  std::uint64_t 最大关系数量 = 0;
};

struct 存在组成读取结果 final {
  存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  std::optional<存在组成关系事实> 父;
  std::vector<存在组成关系事实> 子组;
  bool 父读取成功(const 存在组成父读取请求 &) const noexcept;
  bool 子组读取成功(const 存在组成子组读取请求 &) const noexcept;
};

inline constexpr std::uint32_t 存在组成当前完整读取合同版本_v2 = 2;

enum class 存在组成当前完整读取状态_v2 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  事实代次漂移 = 5,
  资源失败 = 6,
  内部不一致 = 7
};

struct 存在组成父当前完整读取请求_v2 final {
  std::uint32_t 版本 = 存在组成当前完整读取合同版本_v2;
  std::uint64_t G0 = 0;
  稳定编码 子存在{};
};

struct 存在组成子组当前完整读取请求_v2 final {
  std::uint32_t 版本 = 存在组成当前完整读取合同版本_v2;
  std::uint64_t G0 = 0;
  稳定编码 父存在{};
};

struct 存在组成当前完整读取结果_v2 final {
  存在组成当前完整读取状态_v2 状态 = 存在组成当前完整读取状态_v2::入口拒绝;
  std::uint32_t 版本 = 存在组成当前完整读取合同版本_v2;
  std::uint64_t Gread = 0;
  std::optional<存在组成关系事实> 父;
  std::vector<存在组成关系事实> 子组;
  bool 父读取成功(const 存在组成父当前完整读取请求_v2 &) const noexcept;
  bool 子组读取成功(const 存在组成子组当前完整读取请求_v2 &) const noexcept;
};

class 存在组成结构只读提供者 {
public:
  virtual ~存在组成结构只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 存在组成读取结果 读取当前组成父(const 存在组成父读取请求 &) const = 0;
  virtual 存在组成读取结果
  读取当前组成子组(const 存在组成子组读取请求 &) const = 0;
  virtual 存在组成当前完整读取结果_v2
  读取当前组成父_v2(const 存在组成父当前完整读取请求_v2 &) const = 0;
  virtual 存在组成当前完整读取结果_v2
  读取当前组成子组_v2(const 存在组成子组当前完整读取请求_v2 &) const = 0;
};

inline constexpr std::uint32_t 直接归属联合只读合同版本 = 1;
enum class 直接归属来源 : std::uint8_t {
  存在组成 = 1,
  场景成员 = 2,
  直接子场景 = 3
};
enum class 直接归属联合只读状态 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  成员未找到 = 3,
  事实代次漂移 = 5,
  数量预算不足 = 6,
  资源失败 = 8,
  内部不一致 = 9
};
struct 直接归属联合事实 final {
  std::uint64_t Gread = 0;
  直接归属来源 来源 = 直接归属来源::存在组成;
  稳定编码 关系{}, 父{}, 成员{};
  std::uint64_t 创建事实代次 = 0;
};
struct 直接归属联合父读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  稳定编码 成员{};
  std::uint64_t 最大关系数量 = 0;
};
struct 直接归属联合子组读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  稳定编码 父{};
  std::uint64_t 最大关系数量 = 0;
};
struct 直接归属联合读取结果 final {
  直接归属联合只读状态 状态 = 直接归属联合只读状态::入口拒绝;
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  std::optional<直接归属联合事实> 父;
  std::vector<直接归属联合事实> 子组;
  bool 父读取成功(const 直接归属联合父读取请求 &) const noexcept;
  bool 子组读取成功(const 直接归属联合子组读取请求 &) const noexcept;
};

inline constexpr std::uint32_t 直接归属联合当前完整读取合同版本_v2 = 2;

enum class 直接归属联合当前完整读取状态_v2 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  事实代次漂移 = 5,
  资源失败 = 6,
  内部不一致 = 7
};

struct 直接归属联合父当前完整读取请求_v2 final {
  std::uint32_t 版本 = 直接归属联合当前完整读取合同版本_v2;
  std::uint64_t G0 = 0;
  稳定编码 成员{};
};

struct 直接归属联合子组当前完整读取请求_v2 final {
  std::uint32_t 版本 = 直接归属联合当前完整读取合同版本_v2;
  std::uint64_t G0 = 0;
  稳定编码 父{};
};

struct 直接归属联合当前完整读取结果_v2 final {
  直接归属联合当前完整读取状态_v2 状态 =
      直接归属联合当前完整读取状态_v2::入口拒绝;
  std::uint32_t 版本 = 直接归属联合当前完整读取合同版本_v2;
  std::uint64_t Gread = 0;
  std::optional<直接归属联合事实> 父;
  std::vector<直接归属联合事实> 子组;
  bool 父读取成功(const 直接归属联合父当前完整读取请求_v2 &) const noexcept;
  bool 子组读取成功(const 直接归属联合子组当前完整读取请求_v2 &) const noexcept;
};
enum class 直接归属场景位置 : std::uint8_t {
  未纳入场景树 = 1,
  场景树根 = 2,
  场景树非根 = 3
};
struct 直接归属场景角色事实 final {
  std::uint64_t Gread = 0;
  稳定编码 场景{};
  直接归属场景位置 位置 = 直接归属场景位置::未纳入场景树;
  std::optional<稳定编码> 树根, 树证明关系;
};
struct 直接归属场景角色读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  稳定编码 对象{};
};
struct 直接归属场景角色读取结果 final {
  直接归属联合只读状态 状态 = 直接归属联合只读状态::入口拒绝;
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  std::optional<直接归属场景角色事实> 角色;
  bool 成功(const 直接归属场景角色读取请求 &) const noexcept;
};

inline constexpr std::uint32_t 直接归属场景角色当前完整读取合同版本_v2 = 2;

enum class 直接归属场景角色当前完整读取状态_v2 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  事实代次漂移 = 5,
  资源失败 = 7,
  内部不一致 = 8
};

struct 直接归属场景角色当前完整读取请求_v2 final {
  std::uint32_t 版本 = 直接归属场景角色当前完整读取合同版本_v2;
  std::uint64_t G0 = 0;
  稳定编码 对象{};
};

struct 直接归属场景角色当前完整读取结果_v2 final {
  直接归属场景角色当前完整读取状态_v2 状态 =
      直接归属场景角色当前完整读取状态_v2::入口拒绝;
  std::uint32_t 版本 = 直接归属场景角色当前完整读取合同版本_v2;
  std::uint64_t Gread = 0;
  std::optional<直接归属场景角色事实> 角色;
  bool 成功(const 直接归属场景角色当前完整读取请求_v2 &) const noexcept;
};
class 直接归属联合只读提供者 {
public:
  virtual ~直接归属联合只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 直接归属联合读取结果
  读取当前联合父(const 直接归属联合父读取请求 &) const = 0;
  virtual 直接归属联合读取结果
  读取当前联合子组(const 直接归属联合子组读取请求 &) const = 0;
  virtual 直接归属联合当前完整读取结果_v2
  读取当前联合父_v2(const 直接归属联合父当前完整读取请求_v2 &) const = 0;
  virtual 直接归属联合当前完整读取结果_v2
  读取当前联合子组_v2(const 直接归属联合子组当前完整读取请求_v2 &) const = 0;
  virtual 直接归属场景角色读取结果
  读取当前场景角色位置(const 直接归属场景角色读取请求 &) const = 0;
  virtual 直接归属场景角色当前完整读取结果_v2
  读取当前场景角色位置_v2(
      const 直接归属场景角色当前完整读取请求_v2 &) const = 0;
};

class 存在结构身份只读提供者 {
public:
  virtual ~存在结构身份只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 存在当前身份确认结果 确认当前存在结构身份(std::uint64_t G0,
                                                     稳定编码 身份) const = 0;
  virtual 存在身份来源当前见证读取结果
  读取当前存在身份来源见证(std::uint64_t G0, 稳定编码 身份) const = 0;
  virtual 存在已知准确特征读取结果
  确认当前已知准确特征(const 存在已知准确特征当前请求 &) const = 0;
};

inline bool
存在组成读取结果::父读取成功(const 存在组成父读取请求 &r) const noexcept {
  return r.版本 == 1 && r.G0 && 有效(r.子存在) && r.最大关系数量 >= 1 &&
         r.最大关系数量 <= 4096 && 状态 == 存在结构身份只读状态::已读取 &&
         版本 == 1 && Gread == r.G0 && 子组.empty() &&
         (!父 || (父->Gread == Gread &&
                   父->子存在 == r.子存在 && 有效(父->关系) &&
                   有效(父->父存在) && 父->创建事实代次));
}
inline bool 存在组成当前完整读取结果_v2::父读取成功(
    const 存在组成父当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 存在组成当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.子存在) || 版本 != 存在组成当前完整读取合同版本_v2 ||
      状态 != 存在组成当前完整读取状态_v2::已读取 || Gread != r.G0 ||
      !子组.empty())
    return false;
  if (!父)
    return true;
  return 父->Gread == Gread && 父->子存在 == r.子存在 &&
         有效(父->关系) && 有效(父->父存在) && 父->父存在 != 父->子存在 &&
         父->创建事实代次 &&
         父->创建事实代次 <= Gread;
}
inline bool 存在组成当前完整读取结果_v2::子组读取成功(
    const 存在组成子组当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 存在组成当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.父存在) || 版本 != 存在组成当前完整读取合同版本_v2 ||
      状态 != 存在组成当前完整读取状态_v2::已读取 || Gread != r.G0 || 父)
    return false;
  for (std::size_t i = 0; i < 子组.size(); ++i) {
    const auto &x = 子组[i];
    if (x.Gread != Gread || x.父存在 != r.父存在 ||
        !有效(x.关系) || !有效(x.子存在) || x.父存在 == x.子存在 ||
        !x.创建事实代次 ||
        x.创建事实代次 > Gread ||
        (i && 子组[i - 1].关系.值 >= x.关系.值))
      return false;
  }
  return true;
}
inline bool
存在组成读取结果::子组读取成功(const 存在组成子组读取请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || !有效(r.父存在) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096 || 状态 != 存在结构身份只读状态::已读取 ||
      版本 != 1 || Gread != r.G0 || 父 || 子组.size() > r.最大关系数量)
    return false;
  for (std::size_t i = 0; i < 子组.size(); ++i) {
    const auto &x = 子组[i];
    if (x.Gread != Gread || x.父存在 != r.父存在 ||
        !有效(x.关系) || !有效(x.子存在) || !x.创建事实代次)
      return false;
    if (i && 子组[i - 1].关系.值 >= x.关系.值)
      return false;
  }
  return true;
}
inline bool 直接归属联合读取结果::父读取成功(
    const 直接归属联合父读取请求 &r) const noexcept {
  return r.版本 == 1 && r.G0 && 有效(r.成员) && r.最大关系数量 >= 1 &&
         r.最大关系数量 <= 4096 && 版本 == 1 &&
         状态 == 直接归属联合只读状态::已读取 && Gread == r.G0 &&
         子组.empty() &&
         (!父 || (父->Gread == Gread && 父->成员 == r.成员 &&
                   有效(父->关系) && 有效(父->父) && 父->创建事实代次 &&
                   父->创建事实代次 <= Gread));
}
inline bool 直接归属联合读取结果::子组读取成功(
    const 直接归属联合子组读取请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || !有效(r.父) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096 || 版本 != 1 ||
      状态 != 直接归属联合只读状态::已读取 || Gread != r.G0 || 父 ||
      子组.size() > r.最大关系数量)
    return false;
  for (std::size_t i = 0; i < 子组.size(); ++i) {
    const auto &x = 子组[i];
    if (x.Gread != Gread || x.父 != r.父 || !有效(x.关系) ||
        !有效(x.成员) || !x.创建事实代次)
      return false;
    if (i) {
      const auto &p = 子组[i - 1];
      if (p.成员.值 > x.成员.值 ||
          (p.成员 == x.成员 &&
           (static_cast<unsigned>(p.来源) > static_cast<unsigned>(x.来源) ||
            (p.来源 == x.来源 && p.关系.值 >= x.关系.值))))
        return false;
    }
  }
  return true;
}
inline bool 直接归属联合当前完整读取结果_v2::父读取成功(
    const 直接归属联合父当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 直接归属联合当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.成员) || 版本 != 直接归属联合当前完整读取合同版本_v2 ||
      状态 != 直接归属联合当前完整读取状态_v2::已读取 || Gread != r.G0 ||
      !子组.empty())
    return false;
  if (!父)
    return true;
  return 父->Gread == Gread && 父->成员 == r.成员 &&
         有效(父->关系) && 有效(父->父) && 父->父 != 父->成员 &&
         父->创建事实代次 &&
         父->创建事实代次 <= Gread &&
         (父->来源 == 直接归属来源::存在组成 ||
          父->来源 == 直接归属来源::场景成员 ||
          父->来源 == 直接归属来源::直接子场景);
}
inline bool 直接归属联合当前完整读取结果_v2::子组读取成功(
    const 直接归属联合子组当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 直接归属联合当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.父) || 版本 != 直接归属联合当前完整读取合同版本_v2 ||
      状态 != 直接归属联合当前完整读取状态_v2::已读取 || Gread != r.G0 || 父)
    return false;
  for (std::size_t i = 0; i < 子组.size(); ++i) {
    const auto &x = 子组[i];
    if (x.Gread != Gread || x.父 != r.父 || !有效(x.关系) ||
        !有效(x.成员) || x.父 == x.成员 || !x.创建事实代次 ||
        x.创建事实代次 > Gread ||
        (x.来源 != 直接归属来源::存在组成 &&
         x.来源 != 直接归属来源::场景成员 &&
         x.来源 != 直接归属来源::直接子场景))
      return false;
    if (i) {
      const auto &p = 子组[i - 1];
      if (p.成员.值 > x.成员.值 ||
          (p.成员 == x.成员 &&
           (static_cast<unsigned>(p.来源) > static_cast<unsigned>(x.来源) ||
            (p.来源 == x.来源 && p.关系.值 >= x.关系.值))))
        return false;
      if (p.成员 == x.成员)
        return false;
    }
  }
  return true;
}
inline bool 直接归属场景角色读取结果::成功(
    const 直接归属场景角色读取请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || !有效(r.对象) || 版本 != 1 ||
      状态 != 直接归属联合只读状态::已读取 || Gread != r.G0)
    return false;
  if (!角色)
    return true;
  if (角色->Gread != Gread || 角色->场景 != r.对象)
    return false;
  if (角色->位置 == 直接归属场景位置::未纳入场景树)
    return !角色->树根 && !角色->树证明关系;
  if (角色->位置 != 直接归属场景位置::场景树根 &&
      角色->位置 != 直接归属场景位置::场景树非根)
    return false;
  return 角色->树根 && 角色->树证明关系 && 有效(*角色->树根) &&
         有效(*角色->树证明关系) &&
         (角色->位置 != 直接归属场景位置::场景树根 || *角色->树根 == r.对象);
}
inline bool 直接归属场景角色当前完整读取结果_v2::成功(
    const 直接归属场景角色当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 直接归属场景角色当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.对象) || 版本 != 直接归属场景角色当前完整读取合同版本_v2 ||
      状态 != 直接归属场景角色当前完整读取状态_v2::已读取 ||
      Gread != r.G0)
    return false;
  if (!角色)
    return true;
  if (角色->Gread != Gread || 角色->场景 != r.对象)
    return false;
  switch (角色->位置) {
  case 直接归属场景位置::未纳入场景树:
    return !角色->树根 && !角色->树证明关系;
  case 直接归属场景位置::场景树根:
    return 角色->树根 && 角色->树证明关系 && 有效(*角色->树根) &&
           有效(*角色->树证明关系) && *角色->树根 == r.对象;
  case 直接归属场景位置::场景树非根:
    return 角色->树根 && 角色->树证明关系 && 有效(*角色->树根) &&
           有效(*角色->树证明关系) && *角色->树根 != r.对象;
  default:
    return false;
  }
}

} // namespace 海中鱼巣
