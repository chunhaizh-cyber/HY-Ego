#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "../核心/容错检查.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_set>
#include <variant>
#include <vector>
#include "合同.场景角色组织.h"
#include "合同.相关概念添加参与.h"
namespace 海中鱼巣 {
class 绑定存在数据服务;
class 已发布概念引用参与者 {
  friend class 绑定存在数据服务;
private:
  virtual bool 绑定于(const L1事实基座服务&) const noexcept=0;
  virtual L1所有者范围写端口& 借用存在概念引用写端口() noexcept=0;
  virtual 已发布概念引用片段结果 准备存在概念引用片段(
    const 已发布存在概念引用准备请求&, std::uint64_t Gread) const noexcept=0;
  virtual 已发布概念引用首次材料结果 读取存在概念引用首次材料(
    L1所有者范围写入幂等身份) const noexcept=0;
  virtual 存在概念使用读取结果 读取存在概念使用(
    const 存在概念使用读取请求&) const noexcept=0;
public:
  virtual ~已发布概念引用参与者()=default;
};

enum class 存在初始绑定种类 : std::uint8_t {
  父存在组成 = 1,
  场景成员 = 2,
  直接子场景 = 3
};

struct 存在初始绑定 final {
  存在初始绑定种类 种类{};
  稳定编码 绑定节点{};
  friend bool operator==(const 存在初始绑定 &, const 存在初始绑定 &) = default;
};

struct 绑定存在创建预算 final {
  std::uint64_t 最大关系数量 = 0;
  std::uint64_t 最大场景数量 = 0;
  std::uint64_t 最大祖先数量 = 0;
  friend bool operator==(const 绑定存在创建预算 &,
                         const 绑定存在创建预算 &) = default;
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

struct 绑定存在创建请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  存在初始绑定 绑定{};
  稳定编码 期望现实树根{};
  std::variant<存在组成绑定创建键, 存在场景绑定创建键> 幂等键{};
  绑定存在创建预算 预算{};
  friend bool operator==(const 绑定存在创建请求 &,
                         const 绑定存在创建请求 &) = default;
};

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
  事实代次漂移 = 11,
  幂等冲突 = 12,
  数量预算不足 = 13,
  资源失败 = 15,
  内部不一致 = 16,
  已可能发布 = 17
};

struct 绑定存在事实 final {
  存在初始绑定种类 种类{};
  稳定编码 绑定节点{}, 新存在{}, 绑定关系{};
  friend bool operator==(const 绑定存在事实 &, const 绑定存在事实 &) = default;
};

struct 存在单例角色身份 final {
  稳定编码 值{};
  friend bool operator==(const 存在单例角色身份 &, const 存在单例角色身份 &) = default;
};
struct 存在单例角色结构交付 final {
  std::uint32_t 版本 = 1;
  稳定编码 角色登记类型{}, 角色目标类型{};
  存在单例角色身份 项目角色;
  friend bool operator==(const 存在单例角色结构交付 &, const 存在单例角色结构交付 &) = default;
};
enum class 存在单例角色状态 : std::uint8_t {
  已登记=1, 已读取=2, 未绑定=3, 精确重复=4, 入口拒绝=5,
  未找到=6, 角色冲突=8, 事实代次漂移=9,
  幂等冲突=10, 数量预算不足=11,
  资源失败=13, 内部不一致=14, 已可能发布=15
};
struct 存在单例角色结构登记请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  friend bool operator==(const 存在单例角色结构登记请求 &, const 存在单例角色结构登记请求 &) = default;
};
struct 存在单例角色结构登记结果 final {
  std::uint32_t 版本 = 1;
  存在单例角色状态 状态 = 存在单例角色状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布代次;
  std::optional<存在单例角色结构交付> 交付;
  bool 成功(const 存在单例角色结构登记请求 &) const noexcept;
};
struct 存在单例角色读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  存在单例角色身份 角色;
  std::uint64_t 最大关系数 = 0;
  friend bool operator==(const 存在单例角色读取请求 &, const 存在单例角色读取请求 &) = default;
};
struct 存在单例角色事实 final {
  存在单例角色身份 角色;
  稳定编码 登记关系{}, 目标关系{}, E{};
  std::uint64_t 登记创建事实代次 = 0, 目标创建事实代次 = 0;
  存在身份来源当前见证 存在身份;
  friend bool operator==(const 存在单例角色事实 &, const 存在单例角色事实 &) = default;
};
struct 存在单例角色读取结果 final {
  std::uint32_t 版本 = 1;
  存在单例角色状态 状态 = 存在单例角色状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<存在单例角色事实> 事实;
  bool 成功(const 存在单例角色读取请求 &) const noexcept;
  bool 确认未绑定(const 存在单例角色读取请求 &) const noexcept;
};
struct 存在组成概念创建键_v2 final {
  L1所有者范围写入幂等身份 组合{}, 存在{}, 概念引用{};
  friend bool operator==(const 存在组成概念创建键_v2 &, const 存在组成概念创建键_v2 &) = default;
};
struct 存在场景概念创建键_v2 final {
  L1所有者范围写入幂等身份 组合{}, 存在{}, 场景{}, 概念引用{};
  friend bool operator==(const 存在场景概念创建键_v2 &, const 存在场景概念创建键_v2 &) = default;
};
using 已发布概念绑定键_v2 = std::variant<存在组成概念创建键_v2, 存在场景概念创建键_v2>;
struct 已发布概念绑定创建请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t G0 = 0;
  存在初始绑定 绑定;
  稳定编码 期望现实树根{};
  已发布概念绑定键_v2 键;
  绑定存在创建预算 预算;
  概念树概念身份 EC;
  纯概念定义 预期定义;
  概念树预算 概念预算;
  std::optional<存在单例角色身份> 初始角色;
  friend bool operator==(const 已发布概念绑定创建请求 &, const 已发布概念绑定创建请求 &) = default;
};
enum class 已发布概念绑定状态 : std::uint8_t {
  已创建=1, 精确重复=2, 已读取=3, 未派发=4, 入口拒绝=5,
  绑定失败=6, 概念失败=7, 角色冲突=8, 事实代次漂移=9,
  幂等冲突=10, 数量预算不足=11,
  资源失败=13, 内部不一致=14, 已可能发布=15,
  当前事实不再匹配=16
};
struct 已发布概念绑定投影 final {
  绑定存在事实 绑定;
  存在身份来源当前见证 存在身份;
  纯概念事实 概念;
  存在概念使用事实 使用;
  std::optional<场景树节点当前事实> 场景;
  std::optional<存在单例角色事实> 角色;
  friend bool operator==(const 已发布概念绑定投影 &, const 已发布概念绑定投影 &) = default;
};
struct 已发布概念绑定创建结果 final {
  std::uint32_t 版本 = 2;
  已发布概念绑定状态 状态 = 已发布概念绑定状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布代次;
  std::optional<绑定存在创建状态> 绑定原因;
  std::optional<纯概念状态> 概念原因;
  std::optional<已发布概念引用参与状态> 概念引用原因;
  std::optional<已发布概念绑定创建请求> 原实际子请求;
  std::optional<已发布概念绑定投影> 投影;
  bool 成功(const 已发布概念绑定创建请求 &) const noexcept;
};
struct 已发布概念绑定恢复请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0;
  存在初始绑定 绑定;
  稳定编码 期望现实树根{};
  已发布概念绑定键_v2 键;
  绑定存在创建预算 预算;
  纯概念定义 预期定义;
  概念树预算 概念预算;
  std::optional<存在单例角色身份> 初始角色;
  friend bool operator==(const 已发布概念绑定恢复请求 &, const 已发布概念绑定恢复请求 &) = default;
};
struct 已发布概念绑定恢复结果 final {
  std::uint32_t 版本 = 2;
  已发布概念绑定状态 状态 = 已发布概念绑定状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布代次;
  std::optional<绑定存在创建状态> 绑定原因;
  std::optional<纯概念状态> 概念原因;
  std::optional<已发布概念引用参与状态> 概念引用原因;
  std::optional<已发布概念绑定创建请求> 原实际子请求;
  std::optional<已发布概念绑定投影> 投影;
  bool 成功(const 已发布概念绑定恢复请求 &) const noexcept;
};

template <class T> struct 绑定存在参与者结果 final {
  绑定存在创建状态 状态 = 绑定存在创建状态::内部不一致;
  std::uint64_t Gread = 0;
  std::optional<T> 数据;
};

struct 存在绑定出生见证 final {
  std::uint64_t Gread = 0;
  稳定编码 新存在{};
  存在身份来源当前见证 存在身份{};
  std::optional<存在组成关系事实> 组成绑定关系;
};

struct 场景绑定出生见证 final {
  std::uint64_t Gread = 0;
  存在初始绑定种类 种类{};
  稳定编码 绑定节点{}, 新存在{};
  场景直接包含事实 绑定关系{};
  std::optional<场景树节点当前事实> 子场景结构;
};

struct 绑定现实树见证 final {
  std::uint64_t Gread = 0;
  稳定编码 绑定节点{}, 期望现实树根{};
  直接归属场景角色事实 根角色{};
  std::vector<直接归属联合事实> 上行路径;
};

class 绑定存在内容参与者 {
  friend class 绑定存在数据服务;

private:
  virtual const L1事实基座服务 &绑定存在底座() const noexcept = 0;
  virtual L1所有者范围写端口 &绑定存在端口() noexcept = 0;
  virtual bool 绑定存在结构已就绪() const noexcept = 0;
  virtual bool 绑定存在幂等键可用(L1所有者范围写入幂等身份) const noexcept = 0;
  virtual const 存在组成结构只读提供者 &绑定存在组成提供者() const noexcept = 0;
  virtual 绑定存在参与者结果<L1有限N分区原子参与者写集_v3>
  准备存在出生片段(const 绑定存在创建请求 &, std::uint64_t Gread,
                   const std::optional<存在单例角色身份> &) const = 0;
  virtual 存在单例角色读取结果 读取绑定单例角色(
      const 存在单例角色读取请求 &) const noexcept = 0;
  virtual 绑定存在参与者结果<L1所有者范围首次写入读取结果>
      读取存在出生首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual 绑定存在参与者结果<存在绑定出生见证>
  读取存在绑定出生(std::uint64_t Gread, 稳定编码 新存在,
                   const 存在初始绑定 &) const = 0;

public:
  virtual ~绑定存在内容参与者() = default;
};

class 绑定存在场景参与者 {
  friend class 绑定存在数据服务;

private:
  virtual const L1事实基座服务 &绑定存在底座() const noexcept = 0;
  virtual const L1所有者范围写端口 &绑定存在端口() const noexcept = 0;
  virtual bool 绑定存在结构已就绪() const noexcept = 0;
  virtual bool 绑定存在幂等键可用(L1所有者范围写入幂等身份) const noexcept = 0;
  virtual const 场景直接包含只读提供者 &绑定存在场景提供者() const noexcept = 0;
  virtual 绑定存在参与者结果<绑定现实树见证>
  核验绑定现实树(std::uint64_t Gread, const 存在初始绑定 &,
                 稳定编码 期望现实树根, const 绑定存在创建预算 &,
                 const 直接归属联合只读提供者 &) const = 0;
  virtual 绑定存在参与者结果<L1有限N分区原子参与者写集_v3>
  准备场景绑定片段(const 绑定存在创建请求 &, std::uint64_t Gread) const = 0;
  virtual 绑定存在参与者结果<L1所有者范围首次写入读取结果>
      读取场景绑定首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual 绑定存在参与者结果<场景绑定出生见证>
  读取场景绑定出生(std::uint64_t Gread, 稳定编码 新存在,
                   const 存在初始绑定 &) const = 0;

public:
  virtual ~绑定存在场景参与者() = default;
};

} // namespace 海中鱼巣

namespace 海中鱼巣 {
class 绑定存在数据服务 final {
  friend struct 已发布概念绑定创建结果;
  friend struct 已发布概念绑定恢复结果;
  using S = 绑定存在创建状态;
  using K = 存在初始绑定种类;
  using Map = std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>;
  using Maps = std::array<Map, 3>;
  绑定存在内容参与者 &content_;
  绑定存在场景参与者 &scene_;
  直接归属联合只读组合器 joint_;
  mutable std::mutex mutex_;
  static void 要求(bool ok, S s = S::内部不一致);
  template <class T>
  static T 取(const 绑定存在参与者结果<T> &r, std::uint64_t g) {
    要求(r.状态 == S::精确重复, r.状态);
    要求(r.Gread == g && r.数据);
    return *r.数据;
  }
  std::uint64_t 当前() const {
    auto r = content_.绑定存在底座().读取中性当前事实代次({L1中性CRUD合同版本});
    要求(r.状态 == L1中性读取状态::成功,
         r.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致);
    要求(r.事实代次 != 0);
    return r.事实代次;
  }
  static void 核验现实见证(const 绑定现实树见证 &, const 绑定存在创建请求 &,
                           std::uint64_t);
  static 稳定编码 编码(const Maps &maps, std::size_t part,
                       L1所有者范围写集本地键 key) {
    要求(part < maps.size());
    稳定编码 out{};
    for (const auto &[k, id] : maps[part])
      if (k == key) {
        要求(!有效(out));
        out = id;
      }
    要求(有效(out));
    return out;
  }
  static L1所有者范围写集请求 规范(const L1有限N分区原子写集请求_v3 &w,
                                   const Maps &maps) {
    auto ref =
        [&](const L1有限N分区原子事实引用值_v3 &v) -> L1所有者范围事实引用 {
      if (auto *id = std::get_if<稳定编码>(&v))
        return *id;
      if (auto *key = std::get_if<L1所有者范围写集本地键>(&v))
        return *key;
      const auto &x = std::get<L1有限N分区原子事实引用_v3>(v);
      要求(x.参与者.值 && x.参与者.值 <= maps.size());
      return 编码(maps, x.参与者.值 - 1, x.本地键);
    };
    要求(w.值.empty() && w.属性槽变更.empty() && w.退出事实.empty());
    L1所有者范围写集请求 out{w.合同版本, w.期望事实代次, w.写入幂等身份};
    for (const auto &n : w.节点)
      out.节点.push_back({n.本地键, n.种类, n.属性类型表示});
    for (const auto &e : w.关系)
      out.关系.push_back({e.本地键, ref(e.源节点), ref(e.目标节点),
                          ref(e.关系类型节点), e.角色或顺序});
    return out;
  }
  void 核验事实(const L1所有者范围写集请求 &w, L1结构所有者身份 owner,
                std::size_t part, const Maps &maps, std::uint64_t g,
                std::uint64_t h) const {
    auto resolve = [&](const L1所有者范围事实引用 &x) {
      if (auto *id = std::get_if<稳定编码>(&x))
        return *id;
      return 编码(maps, part, std::get<L1所有者范围写集本地键>(x));
    };
    auto raw = [&](稳定编码 id) {
      const L1所有者范围当前事实读取请求_v2 request{
          L1所有者范围当前事实读取合同版本_v2, owner, id, g};
      auto r = content_.绑定存在底座().读取所有者范围当前事实(request);
      要求(r.状态 == L1所有者范围当前事实读取状态_v2::成功,
           r.状态 == L1所有者范围当前事实读取状态_v2::资源失败
               ? S::资源失败
               : S::内部不一致);
      要求(r.读取事实代次 == g, S::事实代次漂移);
      要求(r.事实编码 == id && r.所有者 == owner &&
           r.合同版本 == L1所有者范围当前事实读取合同版本_v2 && r.载荷);
      return *r.载荷;
    };
    for (const auto &n : w.节点) {
      auto f = raw(编码(maps, part, n.本地键));
      auto *p = std::get_if<L1所有者范围节点事实>(&f);
      要求(p && p->种类 == n.种类 && p->属性类型表示 == n.属性类型表示 &&
           p->写入所有者 == owner && p->创建事实代次 == h);
    }
    for (const auto &e : w.关系) {
      auto f = raw(编码(maps, part, e.本地键));
      auto *p = std::get_if<L1所有者范围关系事实>(&f);
      要求(p && p->源节点 == resolve(e.源节点) &&
           p->目标节点 == resolve(e.目标节点) &&
           p->关系类型节点 == resolve(e.关系类型节点) &&
           p->角色或顺序 == e.角色或顺序 && p->写入所有者 == owner &&
           p->创建事实代次 == h);
    }
  }

public:
  绑定存在数据服务(绑定存在内容参与者 &c, 绑定存在场景参与者 &s)
      : content_(c), scene_(s),
        joint_(c.绑定存在组成提供者(), s.绑定存在场景提供者()) {
    const auto &b = c.绑定存在底座();
    if (&b != &s.绑定存在底座() || !c.绑定存在端口().有效() ||
        !s.绑定存在端口().有效() || !c.绑定存在端口().绑定于(b) ||
        !s.绑定存在端口().绑定于(b) || !joint_.绑定于(b) ||
        c.绑定存在端口().所有者身份() == s.绑定存在端口().所有者身份() ||
        !c.绑定存在结构已就绪() || !s.绑定存在结构已就绪())
      throw std::invalid_argument("bound existence participants");
  }
  bool 使用存在提供者(const 绑定存在内容参与者 &) const noexcept;
  bool 使用场景提供者(const 绑定存在场景参与者 &) const noexcept;
  已发布概念绑定创建结果 创建绑定存在并引用概念(
      const 已发布概念绑定创建请求 &, 已发布概念引用参与者 &) noexcept;
  已发布概念绑定恢复结果 读取已发布概念绑定创建(
      const 已发布概念绑定恢复请求 &, 已发布概念引用参与者 &) const noexcept;

private:
  using W = 已发布概念绑定状态;
  using C = 已发布概念引用参与状态;
  using P = 纯概念状态;
  struct 恢复材料 final {
    std::array<L1所有者范围首次写入读取结果, 3> 首次;
    Maps 映射;
    std::optional<已发布概念绑定创建请求> 请求;
    std::optional<已发布概念绑定投影> 投影;
    std::uint64_t 发布代次 = 0;
  };
  static void 完整要求(bool ok, W s = W::内部不一致);
  static bool 投影完整(const 已发布概念绑定投影 &, const 已发布概念绑定创建请求 &,
                       std::uint64_t Gread, std::uint64_t 发布代次) noexcept;
  static W 映射状态(S) noexcept;
  static W 映射状态(P) noexcept;
  static W 映射状态(C) noexcept;
  static W 映射角色(存在单例角色状态) noexcept;
  // 顺序固定为存在、可选场景、概念；末项为组合键。
  static std::array<L1所有者范围写入幂等身份, 4>
  取键(const 已发布概念绑定键_v2 &, K);
  static 绑定存在创建请求 旧片段请求(const 已发布概念绑定创建请求 &);
  static 已发布概念绑定恢复请求 恢复请求(
      const 已发布概念绑定创建请求 &, std::uint64_t);
  void 核验输入(const 已发布概念绑定恢复请求 &,
                const 已发布概念引用参与者 &) const;
  static void 核验映射(const Maps &, K, bool);
  已发布概念绑定投影 读取出生投影(const 已发布概念绑定创建请求 &,
      std::uint64_t, std::uint64_t, const Maps &, const 已发布概念引用参与者 &,
      bool 核验当前 = true) const;
  恢复材料 读取恢复材料(const 已发布概念绑定恢复请求 &,
                         const 已发布概念引用参与者 &,
                         std::optional<std::uint64_t>* = nullptr) const;
  static std::vector<L1有限N分区原子参与者写集_v3> 反建原组合片段(const 恢复材料 &);
  static void 核验提交映射(const L1有限N分区原子事务结果_v3 &,
      const 已发布概念绑定创建请求 &,
      const std::vector<L1有限N分区原子参与者写集_v3> &, Maps &);
};

inline void 绑定存在数据服务::要求(bool ok, S s) {
  if (!ok) throw s;
}

inline void 绑定存在数据服务::核验现实见证(
    const 绑定现实树见证 &v, const 绑定存在创建请求 &r, std::uint64_t g) {
  const auto &root = v.根角色;
  要求(v.Gread == g && v.绑定节点 == r.绑定.绑定节点 &&
       v.期望现实树根 == r.期望现实树根 && root.Gread == g &&
       root.场景 == r.期望现实树根 &&
       root.位置 == 直接归属场景位置::场景树根 &&
       root.树根 == r.期望现实树根 && root.树证明关系 && 有效(*root.树证明关系));
  要求(v.上行路径.size() <= r.预算.最大祖先数量 &&
           v.上行路径.size() <= r.预算.最大关系数量,
       S::数量预算不足);
  auto cursor = r.绑定.绑定节点;
  std::unordered_set<std::uint64_t> seen{cursor.值};
  for (const auto &edge : v.上行路径) {
    要求(edge.Gread == g && edge.成员 == cursor && 有效(edge.父) &&
         有效(edge.关系) && edge.创建事实代次 && edge.创建事实代次 <= g);
    要求(seen.insert(edge.父.值).second, S::成环);
    cursor = edge.父;
  }
  要求(cursor == r.期望现实树根, S::绑定不在现实树);
}

inline bool 绑定存在数据服务::使用存在提供者(const 绑定存在内容参与者 &p) const noexcept {
  return &p == &content_;
}

inline bool 绑定存在数据服务::使用场景提供者(const 绑定存在场景参与者 &p) const noexcept {
  return &p == &scene_;
}

inline void 绑定存在数据服务::完整要求(bool ok, W s) {
  if (!ok) throw s;
}

inline 绑定存在数据服务::W 绑定存在数据服务::映射状态(S s) noexcept {
  switch (s) {
  case S::入口拒绝: case S::绑定未明确: return W::入口拒绝;
  case S::绑定未找到: case S::绑定类型不符:
  case S::绑定不在现实树: case S::包含冲突: case S::成环: return W::绑定失败;
  case S::事实代次漂移: return W::事实代次漂移;
  case S::幂等冲突: return W::幂等冲突;
  case S::数量预算不足: return W::数量预算不足;
  case S::资源失败: return W::资源失败;
  case S::已可能发布: return W::已可能发布;
  default: return W::内部不一致;
  }
}

inline 绑定存在数据服务::W 绑定存在数据服务::映射状态(P s) noexcept {
  switch (s) {
  case P::入口拒绝: case P::未找到: case P::类别冲突:
  case P::定义不支持: case P::定义不相容: case P::组织冲突:
  case P::上位成环: case P::概念已退役:
  case P::引用冲突: case P::旧格式不支持: return W::概念失败;
  case P::事实代次漂移: return W::事实代次漂移;
  case P::幂等冲突: return W::幂等冲突;
  case P::数量预算不足: return W::数量预算不足;
  case P::资源失败: return W::资源失败;
  case P::已可能发布: return W::已可能发布;
  default: return W::内部不一致;
  }
}

inline 绑定存在数据服务::W 绑定存在数据服务::映射状态(C s) noexcept {
  switch (s) {
  case C::入口拒绝: return W::入口拒绝;
  case C::概念未找到: case C::概念已退役:
  case C::类别冲突: case C::定义不相容: case C::引用冲突:
  case C::旧格式不支持: return W::概念失败;
  case C::事实代次漂移: return W::事实代次漂移;
  case C::幂等冲突: return W::幂等冲突;
  case C::数量预算不足: return W::数量预算不足;
  case C::资源失败: return W::资源失败;
  case C::已可能发布: return W::已可能发布;
  default: return W::内部不一致;
  }
}

inline 绑定存在数据服务::W 绑定存在数据服务::映射角色(存在单例角色状态 s) noexcept {
  using R = 存在单例角色状态;
  switch (s) {
  case R::入口拒绝: return W::入口拒绝;
  case R::角色冲突: case R::未找到: return W::角色冲突;
  case R::事实代次漂移: return W::事实代次漂移;
  case R::幂等冲突: return W::幂等冲突;
  case R::数量预算不足: return W::数量预算不足;
  case R::资源失败: return W::资源失败;
  case R::已可能发布: return W::已可能发布;
  default: return W::内部不一致;
  }
}

inline std::array<L1所有者范围写入幂等身份, 4>
绑定存在数据服务::取键(const 已发布概念绑定键_v2 &v, K k) {
  完整要求(!v.valueless_by_exception(), W::入口拒绝);
  if (k == K::父存在组成) {
    完整要求(std::holds_alternative<存在组成概念创建键_v2>(v), W::入口拒绝);
    const auto &x = std::get<存在组成概念创建键_v2>(v);
    return {x.存在, x.概念引用, {}, x.组合};
  }
  完整要求(std::holds_alternative<存在场景概念创建键_v2>(v), W::入口拒绝);
  const auto &x = std::get<存在场景概念创建键_v2>(v);
  return {x.存在, x.场景, x.概念引用, x.组合};
}

inline 绑定存在创建请求 绑定存在数据服务::旧片段请求(const 已发布概念绑定创建请求 &r) {
  const auto keys = 取键(r.键, r.绑定.种类);
  绑定存在创建请求 q{1, r.G0, r.绑定, r.期望现实树根, {}, r.预算};
  if (r.绑定.种类 == K::父存在组成)
    q.幂等键 = 存在组成绑定创建键{keys[0]};
  else
    q.幂等键 = 存在场景绑定创建键{keys[3], keys[0], keys[1]};
  return q;
}

inline 已发布概念绑定恢复请求 绑定存在数据服务::恢复请求(
    const 已发布概念绑定创建请求 &r, std::uint64_t g) {
  return {2, g, r.绑定, r.期望现实树根, r.键, r.预算,
          r.预期定义, r.概念预算, r.初始角色};
}

inline void 绑定存在数据服务::核验输入(const 已发布概念绑定恢复请求 &r,
                                   const 已发布概念引用参与者 &c) const {
  要求(有效(r.绑定.绑定节点) &&
       (r.绑定.种类 == K::父存在组成 || r.绑定.种类 == K::场景成员 ||
        r.绑定.种类 == K::直接子场景), S::绑定未明确);
  完整要求(r.版本 == 2 && r.Gread && 有效(r.期望现实树根) &&
           c.绑定于(content_.绑定存在底座()) &&
           (!r.初始角色 || 有效(r.初始角色->值)), W::入口拒绝);
  完整要求(r.预算.最大关系数量 && r.预算.最大关系数量 <= 4096 &&
           r.预算.最大场景数量 && r.预算.最大场景数量 <= 4096 &&
           r.预算.最大祖先数量 && r.预算.最大祖先数量 <= 4096 &&
           !r.预期定义.valueless_by_exception() &&
           !std::holds_alternative<纯I64特征概念定义>(r.预期定义), W::入口拒绝);
  const auto keys = 取键(r.键, r.绑定.种类);
  const auto count = r.绑定.种类 == K::父存在组成 ? 2U : 3U;
  完整要求(content_.绑定存在幂等键可用(keys[0]) && 有效(keys[3]), W::入口拒绝);
  if (count == 3) 完整要求(scene_.绑定存在幂等键可用(keys[1]), W::入口拒绝);
  for (std::size_t i = 0; i < count; ++i) {
    完整要求(有效(keys[i]) && keys[i] != keys[3], W::入口拒绝);
    for (std::size_t j = 0; j < i; ++j)
      完整要求(keys[i] != keys[j], W::入口拒绝);
  }
}

inline void 绑定存在数据服务::核验映射(const Maps &maps, K k, bool role) {
  const auto count = k == K::父存在组成 ? 2U : 3U;
  std::array<std::size_t, 3> sizes{
      (k == K::父存在组成 ? 3U : 2U) + (role ? 1U : 0U),
      k == K::父存在组成 || k == K::场景成员 ? 1U : 12U,
      k == K::父存在组成 ? 0U : 1U};
  std::unordered_set<std::uint64_t> ids;
  for (std::size_t i = 0; i < maps.size(); ++i) {
    完整要求(maps[i].size() == sizes[i]);
    for (std::uint32_t j = 1; j <= sizes[i]; ++j)
      完整要求(i < count && ids.insert(编码(maps, i, {j}).值).second);
  }
}

inline void 绑定存在数据服务::核验提交映射(
    const L1有限N分区原子事务结果_v3 &saved,
    const 已发布概念绑定创建请求 &r,
    const std::vector<L1有限N分区原子参与者写集_v3> &parts, Maps &maps) {
  const auto keys = 取键(r.键, r.绑定.种类);
  要求(saved.合同版本 == L1有限N分区原子事务合同版本_v3 &&
       saved.共同事实代次 == r.G0 + 1 && saved.组合写入幂等身份 == keys[3] &&
       saved.参与者结果组.size() == parts.size());
  Maps actual;
  std::array<bool, 3> seen{};
  for (const auto &p : saved.参与者结果组) {
    要求(p.参与者.值 && p.参与者.值 <= parts.size());
    const auto i = p.参与者.值 - 1;
    要求(!seen[i] && p.所有者 == parts[i].所有者);
    seen[i] = true;
    actual[i] = p.新编码映射;
  }
  核验映射(actual, r.绑定.种类, r.初始角色.has_value());
  maps = std::move(actual);
}

inline bool 存在单例角色结构登记结果::成功(
    const 存在单例角色结构登记请求 &r) const noexcept {
  if (版本 != 1 || r.版本 != 1 || !r.G0 || r.幂等身份.值!=0x1202U ||
      (状态 != 存在单例角色状态::已登记 && 状态 != 存在单例角色状态::精确重复) ||
      !首次发布代次 || r.G0 >= *首次发布代次 || *首次发布代次 > Gread ||
      !交付 || 交付->版本 != 1)
    return false;
  const auto &x = *交付;
  return 有效(x.角色登记类型) && 有效(x.角色目标类型) && 有效(x.项目角色.值) &&
         x.角色登记类型 != x.角色目标类型 && x.角色登记类型 != x.项目角色.值 &&
         x.角色目标类型 != x.项目角色.值;
}
inline bool 存在单例角色读取结果::成功(
    const 存在单例角色读取请求 &r) const noexcept {
  if (版本 != 1 || r.版本 != 1 || !r.Gread ||
      !r.最大关系数 || !有效(r.角色.值) || Gread != r.Gread ||
      状态 != 存在单例角色状态::已读取 || !事实)
    return false;
  const auto &f = *事实;
  return f.角色 == r.角色 && 有效(f.登记关系) && 有效(f.目标关系) &&
         f.登记关系 != f.目标关系 && 有效(f.E) && f.E != f.角色.值 &&
         f.登记创建事实代次 && f.登记创建事实代次 <= Gread &&
         f.目标创建事实代次 && f.目标创建事实代次 <= Gread &&
         存在身份来源当前见证完整(f.存在身份, Gread, f.E);
}
inline bool 存在单例角色读取结果::确认未绑定(
    const 存在单例角色读取请求 &r) const noexcept {
  return 版本 == 1 && r.版本 == 1 && r.Gread &&
         r.最大关系数 && 有效(r.角色.值) && Gread == r.Gread &&
         状态 == 存在单例角色状态::未绑定 && !事实;
}

inline bool 已发布概念绑定创建结果::成功(
    const 已发布概念绑定创建请求 &r) const noexcept {
  const auto input=[&]() noexcept {
    if(r.版本!=2||!r.G0||r.G0==UINT64_MAX||!有效(r.绑定.绑定节点)||
       (r.绑定.种类!=存在初始绑定种类::父存在组成&&
        r.绑定.种类!=存在初始绑定种类::场景成员&&
        r.绑定.种类!=存在初始绑定种类::直接子场景)||
       !有效(r.期望现实树根)||!有效(r.EC.值)||
       !r.预算.最大关系数量||!r.预算.最大场景数量||!r.预算.最大祖先数量||
       !r.概念预算.最大概念数||!r.概念预算.最大关系数||!r.概念预算.最大特征属性数||
       std::holds_alternative<纯I64特征概念定义>(r.预期定义)||
       (r.初始角色&&!有效(r.初始角色->值))) return false;
    std::array<L1所有者范围写入幂等身份,4> keys{};
    std::size_t count=0;
    if(r.绑定.种类==存在初始绑定种类::父存在组成) {
      const auto *k=std::get_if<存在组成概念创建键_v2>(&r.键);
      if(!k)return false;keys={k->组合,k->存在,k->概念引用,{}};count=3;
    } else {
      const auto *k=std::get_if<存在场景概念创建键_v2>(&r.键);
      if(!k)return false;keys={k->组合,k->存在,k->场景,k->概念引用};count=4;
    }
    for(std::size_t i=0;i<count;++i){if(!有效(keys[i]))return false;
      for(std::size_t j=0;j<i;++j)if(keys[i]==keys[j])return false;}
    return true;
  };
  if (版本 != 2 || !input() ||
      (状态 != 已发布概念绑定状态::已创建 && 状态 != 已发布概念绑定状态::精确重复) ||
      !首次发布代次 || r.G0 == UINT64_MAX || r.G0 + 1 != *首次发布代次 ||
      *首次发布代次 > Gread || !原实际子请求 ||
      *原实际子请求 != r || !投影 || 绑定原因 || 概念原因 || 概念引用原因)
    return false;
  return 绑定存在数据服务::投影完整(*投影, r, Gread, *首次发布代次);
}
inline bool 绑定存在数据服务::投影完整(const 已发布概念绑定投影 &p,
    const 已发布概念绑定创建请求 &r, std::uint64_t Gread, std::uint64_t h) noexcept {

  const auto &b = p.绑定;
  if (b.种类 != r.绑定.种类 || b.绑定节点 != r.绑定.绑定节点 ||
      !有效(b.新存在) || b.新存在 == b.绑定节点 || !有效(b.绑定关系) ||
      !存在身份来源当前见证完整(p.存在身份, h, b.新存在) ||
      p.存在身份.节点创建事实代次 != h ||
      p.概念.概念 != r.EC || p.概念.类别 != 相关概念类别::存在 ||
      p.概念.定义 != r.预期定义 || !p.概念.生命周期.创建事实代次 ||
      p.概念.生命周期.创建事实代次 >= h ||
      p.使用.E != b.新存在 || p.使用.EC != r.EC || !有效(p.使用.关系) ||
      p.使用.生命周期.创建事实代次 != h ||
      p.角色.has_value() != r.初始角色.has_value() ||
      p.场景.has_value() != (r.绑定.种类 == 存在初始绑定种类::直接子场景))
    return false;
  if (p.角色 && (p.角色->角色 != *r.初始角色 || p.角色->E != b.新存在 ||
      !有效(p.角色->目标关系) || p.角色->目标创建事实代次 != h))
    return false;
  if (p.场景) {
    const auto &s = *p.场景;
    if (s.场景角色.场景 != b.新存在 || s.场景角色.Gread != Gread ||
        s.树证明.树根 != r.期望现实树根 ||
        !s.直接父 || s.直接父->关系 != b.绑定关系 || !s.父语境投影 ||
        s.父语境投影->父场景语境 != r.绑定.绑定节点)
      return false;
    for (const auto &root : s.场景角色.四根)
      if (!有效(root.根.编码) || !有效(root.绑定.编码) ||
          root.根.生命周期.创建事实代次 != h ||
          root.绑定.生命周期.创建事实代次 != h) return false;
  }
  return true;
}
inline bool 已发布概念绑定恢复结果::成功(
    const 已发布概念绑定恢复请求 &r) const noexcept {
  const auto input=[&]() noexcept {
    if(r.版本!=2||!r.Gread||!有效(r.绑定.绑定节点)||!有效(r.期望现实树根)||
       (r.绑定.种类!=存在初始绑定种类::父存在组成&&
        r.绑定.种类!=存在初始绑定种类::场景成员&&
        r.绑定.种类!=存在初始绑定种类::直接子场景)||
       !r.预算.最大关系数量||!r.预算.最大场景数量||!r.预算.最大祖先数量||
       !r.概念预算.最大概念数||!r.概念预算.最大关系数||!r.概念预算.最大特征属性数||
       std::holds_alternative<纯I64特征概念定义>(r.预期定义)||
       (r.初始角色&&!有效(r.初始角色->值))) return false;
    std::array<L1所有者范围写入幂等身份,4> keys{};
    std::size_t count=0;
    if(r.绑定.种类==存在初始绑定种类::父存在组成) {
      const auto *k=std::get_if<存在组成概念创建键_v2>(&r.键);
      if(!k)return false;keys={k->组合,k->存在,k->概念引用,{}};count=3;
    } else {
      const auto *k=std::get_if<存在场景概念创建键_v2>(&r.键);
      if(!k)return false;keys={k->组合,k->存在,k->场景,k->概念引用};count=4;
    }
    for(std::size_t i=0;i<count;++i){if(!有效(keys[i]))return false;
      for(std::size_t j=0;j<i;++j)if(keys[i]==keys[j])return false;}
    return true;
  };
  if (版本 != 2 || !input() || 状态 != 已发布概念绑定状态::已读取 ||
      Gread != r.Gread || !原实际子请求 || !投影 || !首次发布代次)
    return false;
  const auto &q = *原实际子请求;
  if (q.版本 != 2 || q.绑定 != r.绑定 || q.期望现实树根 != r.期望现实树根 || q.键 != r.键 ||
      q.预算 != r.预算 || q.预期定义 != r.预期定义 ||
      q.概念预算 != r.概念预算 || q.初始角色 != r.初始角色) return false;
  return q.G0 && q.G0 < UINT64_MAX && q.G0 + 1 == *首次发布代次 &&
         *首次发布代次 <= Gread &&
         !绑定原因 && !概念原因 && !概念引用原因 &&
         绑定存在数据服务::投影完整(*投影, q, Gread, *首次发布代次);
}

inline 已发布概念绑定投影 绑定存在数据服务::读取出生投影(
    const 已发布概念绑定创建请求 &r, std::uint64_t g, std::uint64_t h,
    const Maps &maps, const 已发布概念引用参与者 &c, bool checkCurrent) const {
  const bool single = r.绑定.种类 == K::父存在组成;
  const auto last = single ? 1U : 2U;
  const auto E = 编码(maps, 0, {1});
  const auto relation = 编码(maps, single ? 0 : 1,
      {single ? 3U : (r.绑定.种类 == K::场景成员 ? 1U : 11U)});
  已发布概念绑定投影 out;
  out.绑定 = {r.绑定.种类, r.绑定.绑定节点, E, relation};
  const auto ev = 取(content_.读取存在绑定出生(g, E, r.绑定), g);
  要求(ev.Gread == g && ev.新存在 == E &&
       存在身份来源当前见证完整(ev.存在身份, h, E) &&
       ev.存在身份.族归属关系 == 编码(maps, 0, {2}) &&
       ev.存在身份.节点创建事实代次 == h &&
       ev.组成绑定关系.has_value() == single);
  out.存在身份 = ev.存在身份;
  if (single) {
    const auto &e = *ev.组成绑定关系;
    要求(e.Gread == g && e.父存在 == r.绑定.绑定节点 &&
         e.子存在 == E && e.关系 == relation && e.创建事实代次 == h);
  } else {
    const auto sv = 取(scene_.读取场景绑定出生(g, E, r.绑定), g);
    const auto &e = sv.绑定关系;
    要求(sv.Gread == g && sv.种类 == r.绑定.种类 &&
         sv.新存在 == E && sv.绑定节点 == r.绑定.绑定节点 &&
         e.Gread == g && e.父场景 == r.绑定.绑定节点 &&
         e.成员 == E && e.关系.编码 == relation &&
         e.关系.源 == r.绑定.绑定节点 && e.关系.目标 == E &&
         e.关系.角色或顺序 == 1 && e.关系.生命周期.创建事实代次 == h &&
         sv.子场景结构.has_value() == (r.绑定.种类 == K::直接子场景));
    out.场景 = sv.子场景结构;
    if (out.场景) {
      const auto &n = *out.场景;
      要求(n.场景角色.Gread == g &&
           n.场景角色.场景 == E && n.树证明.树根 == r.期望现实树根 &&
           n.树证明.关系 == 编码(maps, 1, {10}) && n.直接父 &&
           n.直接父->关系 == relation &&
           n.从上游场景到本场景路径.size() == 1 &&
           n.从上游场景到本场景路径.front().关系 == relation &&
           n.父语境投影 && n.父语境投影->Gread == g &&
           n.父语境投影->场景 == E && n.父语境投影->父场景语境 == r.绑定.绑定节点 &&
           n.父语境投影->结构父.关系 == 编码(maps, 1, {11}) &&
           n.父语境投影->投影边.编码 == 编码(maps, 1, {12}) &&
           n.父语境投影->投影边.源 == E &&
           n.父语境投影->投影边.目标 == r.绑定.绑定节点 &&
           有效(n.父语境投影->投影边.关系类型) &&
           n.父语境投影->投影边.关系类型 != e.关系.关系类型 &&
           n.父语境投影->投影边.角色或顺序 == 1 &&
           n.父语境投影->投影边.生命周期.创建事实代次 == h);
      for (std::uint32_t i = 0; i < 4; ++i)
        要求(n.场景角色.四根[i].根.编码 == 编码(maps, 1, {i + 1}) &&
             n.场景角色.四根[i].绑定.编码 == 编码(maps, 1, {i + 6}));
    }
  }
  const 存在概念使用读取请求 usageRequest{2, g, 概念树存在引用{E}, r.概念预算};
  const auto usage = c.读取存在概念使用(usageRequest);
  if (!usage.成功(usageRequest)) throw usage.状态;
  完整要求(usage.使用 && usage.概念 && usage.使用->E == E &&
           usage.使用->EC == r.EC && usage.使用->关系 == 编码(maps, last, {1}) &&
           usage.使用->生命周期.创建事实代次 == h &&
           usage.概念->概念 == r.EC && usage.概念->类别 == 相关概念类别::存在 &&
           usage.概念->定义 == r.预期定义 &&
           usage.概念->生命周期.创建事实代次 &&
           usage.概念->生命周期.创建事实代次 < h, W::幂等冲突);
  out.概念 = *usage.概念;
  out.使用 = *usage.使用;
  if (r.初始角色) {
    const 存在单例角色读取请求 request{1, g, *r.初始角色, r.预算.最大关系数量};
    const auto role = content_.读取绑定单例角色(request);
    if (!role.成功(request)) throw 映射角色(role.状态);
    完整要求(role.事实->E == E &&
             role.事实->目标关系 == 编码(maps, 0, {single ? 4U : 3U}) &&
             role.事实->目标创建事实代次 == h, W::幂等冲突);
    out.角色 = role.事实;
  }
  if (!checkCurrent) {
    要求(当前() == g, S::事实代次漂移);
    return out;
  }
  // 首次发布材料完整并不意味着当前仍采用初始父和引用。
  const 直接归属联合父读取请求 parentRequest{1, g, E, r.预算.最大关系数量};
  const auto current = joint_.读取当前联合父(parentRequest);
  if (!current.父读取成功(parentRequest)) {
    using J = 直接归属联合只读状态;
    switch (current.状态) {
    case J::成员未找到:
      要求(current.Gread == g, S::事实代次漂移);
      throw g > h ? W::当前事实不再匹配 : W::内部不一致;
    case J::事实代次漂移: throw W::事实代次漂移;
    case J::数量预算不足: throw W::数量预算不足;
    case J::资源失败: throw W::资源失败;
    default: throw W::内部不一致;
    }
  }
  if (!current.父 || current.父->父 != r.绑定.绑定节点 || current.父->关系 != relation)
    throw g > h ? W::当前事实不再匹配 : W::内部不一致;
  const auto reality = scene_.核验绑定现实树(g, r.绑定, r.期望现实树根, r.预算, joint_);
  if (reality.状态 == S::绑定不在现实树 && g > h) {
    要求(reality.Gread == g, S::事实代次漂移);
    throw W::当前事实不再匹配;
  }
  核验现实见证(取(reality, g), 旧片段请求(r), g);
  if (g > h) {
    const 存在概念使用读取请求 nowRequest{2, g, 概念树存在引用{E}, r.概念预算};
    const auto now = c.读取存在概念使用(nowRequest);
    if (!now.成功(nowRequest)) {
      if (now.状态 == P::未找到) {
        完整要求(now.Gread == g, W::事实代次漂移);
        throw W::当前事实不再匹配;
      }
      throw now.状态;
    }
    if (now.使用->关系 != out.使用.关系 || now.使用->EC != r.EC)
      throw W::当前事实不再匹配;
    if (r.初始角色) {
      const 存在单例角色读取请求 request{1, g, *r.初始角色, r.预算.最大关系数量};
      const auto role = content_.读取绑定单例角色(request);
      if (role.确认未绑定(request)) throw W::当前事实不再匹配;
      if (!role.成功(request)) throw 映射角色(role.状态);
      if (role.事实->E != E || role.事实->目标关系 != out.角色->目标关系)
        throw W::当前事实不再匹配;
    }
  }
  要求(当前() == g, S::事实代次漂移);
  return out;
}

inline 绑定存在数据服务::恢复材料 绑定存在数据服务::读取恢复材料(
    const 已发布概念绑定恢复请求 &r, const 已发布概念引用参与者 &c,
    std::optional<std::uint64_t>* observedPublishGeneration) const {
  核验输入(r, c);
  要求(当前() == r.Gread, S::事实代次漂移);
  const bool single = r.绑定.种类 == K::父存在组成;
  const auto count = single ? 2U : 3U;
  const auto last = count - 1;
  const auto keys = 取键(r.键, r.绑定.种类);
  恢复材料 out;
  out.首次[0] = 取(content_.读取存在出生首次材料(keys[0]), r.Gread);
  if (!single)
    out.首次[1] = 取(scene_.读取场景绑定首次材料(keys[1]), r.Gread);
  const auto cf = c.读取存在概念引用首次材料(keys[last]);
  if (cf.状态 != C::未派发 && cf.状态 != C::已读取) throw cf.状态;
  完整要求(cf.Gread == r.Gread, W::事实代次漂移);
  if (cf.状态 == C::未派发) {
    完整要求(!cf.首次材料);
    out.首次[last].状态 = L1所有者范围读取状态::未找到;
    out.首次[last].写入幂等身份 = keys[last];
    out.首次[last].读取事实代次 = r.Gread;
  } else {
    完整要求(cf.首次材料.has_value());
    out.首次[last] = *cf.首次材料;
  }
  std::size_t found = 0;
  for (std::size_t i = 0; i < count; ++i) {
    const auto &f = out.首次[i];
    要求(f.读取事实代次 == r.Gread, S::事实代次漂移);
    要求(f.合同版本 == L1所有者范围首次写入读取合同版本 && f.写入幂等身份 == keys[i]);
    if (i == 0) 要求(f.所有者 == content_.绑定存在端口().所有者身份());
    if (!single && i == 1) 要求(f.所有者 == scene_.绑定存在端口().所有者身份());
    if (f.状态 == L1所有者范围读取状态::未找到) {
      要求(!f.首次规范化写集 && !f.首次写入结果);
      continue;
    }
    要求(f.状态 == L1所有者范围读取状态::成功,
         f.状态 == L1所有者范围读取状态::资源失败 ? S::资源失败 : S::内部不一致);
    要求(f.首次规范化写集 && f.首次写入结果 && 有效(f.所有者));
    const auto &saved = *f.首次写入结果;
    const auto &w = *f.首次规范化写集;
    要求(saved.状态 == L1所有者范围写入状态::成功 &&
         saved.合同版本 == L1所有者范围CRUD合同版本 && saved.是否形成内存权威发布 &&
         saved.所有者 == f.所有者 && saved.写入幂等身份 == keys[i] &&
         saved.事实代次 > 1 && saved.事实代次 <= r.Gread &&
         w.合同版本 == L1所有者范围CRUD合同版本 && w.写入幂等身份 == keys[i] &&
         w.期望事实代次 == saved.事实代次 - 1);
    if (out.发布代次) 要求(out.发布代次 == saved.事实代次);
    out.发布代次 = saved.事实代次;
    if(observedPublishGeneration)*observedPublishGeneration=out.发布代次;
    out.映射[i] = saved.新编码映射;
    ++found;
  }
  if (found == 0) {
    要求(当前() == r.Gread, S::事实代次漂移);
    return out;
  }
  完整要求(found == count, W::已可能发布);
  for (std::size_t i = 1; i < count; ++i)
    for (std::size_t j = 0; j < i; ++j)
      要求(out.首次[i].所有者 != out.首次[j].所有者);
  核验映射(out.映射, r.绑定.种类, r.初始角色.has_value());
  const auto E = 编码(out.映射, 0, {1});
  if(single) {
    const auto &w=*out.首次[0].首次规范化写集;
    const auto it=std::find_if(w.关系.begin(),w.关系.end(),
        [](const auto &x){return x.本地键.值==3;});
    完整要求(it!=w.关系.end()&&
             it->源节点==L1所有者范围事实引用{r.绑定.绑定节点}&&
             it->目标节点==L1所有者范围事实引用{L1所有者范围写集本地键{1}},
             W::幂等冲突);
  } else {
    const auto &w=*out.首次[1].首次规范化写集;
    std::uint32_t expected=11U;
    if(r.绑定.种类==K::场景成员)expected=1U;
    const auto it=std::find_if(w.关系.begin(),w.关系.end(),
        [&](const auto &x){return x.本地键.值==expected;});
    完整要求(it!=w.关系.end()&&
             it->源节点==L1所有者范围事实引用{r.绑定.绑定节点}&&
             it->目标节点==L1所有者范围事实引用{E},W::幂等冲突);
  }
  const auto &cw = *out.首次[last].首次规范化写集;
  要求(cw.节点.empty() && cw.关系.size() == 1 && cw.值.empty() &&
       cw.属性槽变更.empty() && cw.退出事实.empty());
  const auto &edge = cw.关系.front();
  const auto *ec = std::get_if<稳定编码>(&edge.目标节点);
  完整要求(edge.本地键.值 == 1 && edge.源节点 == L1所有者范围事实引用{E} &&
           ec && 有效(*ec) && edge.角色或顺序 == 1, W::幂等冲突);
  const 存在概念使用读取请求 usageRequest{
      2, r.Gread, 概念树存在引用{E}, r.概念预算};
  const auto usage = c.读取存在概念使用(usageRequest);
  if (!usage.成功(usageRequest)) throw usage.状态;
  完整要求(usage.使用 && usage.概念 && usage.使用->EC.值 == *ec &&
           usage.使用->关系 == 编码(out.映射, last, {1}) &&
           usage.概念->定义 == r.预期定义 && usage.概念->类别 == 相关概念类别::存在,
           W::幂等冲突);
  out.请求 = 已发布概念绑定创建请求{2, out.发布代次 - 1, r.绑定, r.期望现实树根,
      r.键, r.预算, usage.使用->EC, r.预期定义, r.概念预算, r.初始角色};
  for (std::size_t i = 0; i < count; ++i) {
    const auto &w = *out.首次[i].首次规范化写集;
    const auto nodes = i == 0 ? 1U : (!single && i == 1 && r.绑定.种类 == K::直接子场景 ? 4U : 0U);
    完整要求(w.节点.size() == nodes && w.节点.size() + w.关系.size() == out.映射[i].size() &&
             w.值.empty() && w.属性槽变更.empty() && w.退出事实.empty(), W::幂等冲突);
    for (const auto &n : w.节点)
      完整要求(n.本地键.值 >= 1 && n.本地键.值 <= nodes &&
               n.种类 == 节点种类::普通 && !n.属性类型表示, W::幂等冲突);
  }
  for (std::size_t i = 0; i < count; ++i)
    核验事实(*out.首次[i].首次规范化写集, out.首次[i].所有者, i,
             out.映射, r.Gread, out.发布代次);
  out.投影 = 读取出生投影(*out.请求, r.Gread, out.发布代次, out.映射, c, false);
  return out;
}

inline 已发布概念绑定创建结果 绑定存在数据服务::创建绑定存在并引用概念(
    const 已发布概念绑定创建请求 &r, 已发布概念引用参与者 &c) noexcept {
  已发布概念绑定创建结果 out;
  bool dispatched = false;
  auto fail = [&](W state) {
    out.状态 = dispatched ? W::已可能发布 : state;
    out.投影.reset();
    try { 追根因检查(false, L"存在绑定与已发布概念引用未完成：请核对结构化原因和原键材料"); }
    catch (...) {}
  };
  try {
    std::unique_lock lock(mutex_);
    out.原实际子请求 = r;
    完整要求(r.版本 == 2 && r.G0 && r.G0 < UINT64_MAX && 有效(r.EC.值), W::入口拒绝);
    out.Gread = 当前();
    要求(r.G0 <= out.Gread, S::事实代次漂移);
    const auto query = 恢复请求(r, out.Gread);
    auto prior = 读取恢复材料(query,c,&out.首次发布代次);
    if (prior.请求) {
      // 恢复入口独占自己的协调锁；当前读取代次仍由请求固定。
      lock.unlock();
      const auto recovered = 读取已发布概念绑定创建(query, c);
      out.Gread = recovered.Gread;
      out.首次发布代次 = recovered.首次发布代次;
      out.绑定原因 = recovered.绑定原因;
      out.概念原因 = recovered.概念原因;
      out.概念引用原因 = recovered.概念引用原因;
      out.原实际子请求 = recovered.原实际子请求;
      if (!recovered.成功(query)) {
        fail(recovered.状态 == W::未派发 ? W::已可能发布 : recovered.状态);
        return out;
      }
      完整要求(*recovered.原实际子请求 == r, W::幂等冲突);
      out.投影 = recovered.投影;
      out.状态 = W::精确重复;
      完整要求(out.成功(r));
      return out;
    }
    要求(out.Gread == r.G0, S::事实代次漂移);
    const auto old = 旧片段请求(r);
    核验现实见证(取(scene_.核验绑定现实树(out.Gread, r.绑定, r.期望现实树根,
        r.预算, joint_), out.Gread), old, out.Gread);
    if (r.初始角色) {
      const 存在单例角色读取请求 roleRequest{1, out.Gread,
          *r.初始角色, r.预算.最大关系数量};
      const auto role = content_.读取绑定单例角色(roleRequest);
      if (role.成功(roleRequest)) throw W::角色冲突;
      if (!role.确认未绑定(roleRequest)) throw 映射角色(role.状态);
    }
    const bool single = r.绑定.种类 == K::父存在组成;
    const auto count = single ? 2U : 3U;
    const auto last = count - 1;
    const auto keys = 取键(r.键, r.绑定.种类);
    auto &conceptPort = c.借用存在概念引用写端口();
    完整要求(conceptPort.有效() && conceptPort.绑定于(content_.绑定存在底座()) &&
             conceptPort.所有者身份() != content_.绑定存在端口().所有者身份() &&
             conceptPort.所有者身份() != scene_.绑定存在端口().所有者身份(), W::入口拒绝);
    std::vector<L1有限N分区原子参与者写集_v3> parts;
    parts.reserve(count);
    parts.push_back(取(content_.准备存在出生片段(old, out.Gread, r.初始角色), out.Gread));
    if (!single)
      parts.push_back(取(scene_.准备场景绑定片段(old, out.Gread), out.Gread));
    const 已发布存在概念引用准备请求 conceptRequest{2, r.G0, keys[last],
        {static_cast<std::uint8_t>(count)}, {{1}, {1}}, r.EC, r.预期定义, r.概念预算};
    const auto prepared = c.准备存在概念引用片段(conceptRequest, out.Gread);
    if (prepared.状态 != C::已准备) throw prepared.状态;
    完整要求(prepared.Gread == out.Gread, W::事实代次漂移);
    完整要求(prepared.写集.has_value());
    parts.push_back(*prepared.写集);
    for (std::size_t i = 0; i < count; ++i) {
      const auto &p = parts[i];
      完整要求(p.参与者.值 == i + 1 && p.写集.合同版本 == L1所有者范围CRUD合同版本 &&
               p.写集.期望事实代次 == r.G0 && p.写集.写入幂等身份 == keys[i]);
      const auto owner = i == 0 ? content_.绑定存在端口().所有者身份() :
          (i == last ? conceptPort.所有者身份() : scene_.绑定存在端口().所有者身份());
      完整要求(p.所有者 == owner);
    }
    const auto &cw = parts.back().写集;
    完整要求(cw.节点.empty() && cw.关系.size() == 1 && cw.值.empty() &&
             cw.属性槽变更.empty() && cw.退出事实.empty());
    const auto &ce = cw.关系.front();
    完整要求(ce.本地键.值 == 1 && ce.角色或顺序 == 1 &&
             ce.源节点 == L1有限N分区原子事实引用值_v3{L1有限N分区原子事实引用_v3{{1}, {1}}} &&
             ce.目标节点 == L1有限N分区原子事实引用值_v3{r.EC.值} &&
             std::holds_alternative<稳定编码>(ce.关系类型节点) &&
             有效(std::get<稳定编码>(ce.关系类型节点)));
    std::vector<const L1所有者范围写端口 *> ports;
    if (!single) ports.push_back(&scene_.绑定存在端口());
    ports.push_back(&conceptPort);
    const L1有限N分区原子事务请求_v3 transaction{
        L1有限N分区原子事务合同版本_v3, r.G0, keys[3], parts};
    要求(当前() == out.Gread, S::事实代次漂移);
    dispatched = true;
    const auto saved = content_.绑定存在端口().提交有限N分区原子事务_v3(transaction, ports);
    using L = L1有限N分区原子事务状态_v3;
    if (saved.状态 != L::已提交 && saved.状态 != L::精确重复) {
      if (saved.状态 == L::已可能发布 || saved.是否已确认形成内存权威发布 ||
          saved.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛)
        throw W::已可能发布;
      dispatched = false;
      switch (saved.状态) {
      case L::事实代次漂移: throw S::事实代次漂移;
      case L::幂等冲突: throw S::幂等冲突;
      case L::资源失败: throw S::资源失败;
      case L::入口拒绝: throw S::入口拒绝;
      default: throw S::内部不一致;
      }
    }
    完整要求(saved.状态 == L::精确重复 || saved.是否已确认形成内存权威发布);
    Maps maps;
    核验提交映射(saved, r, parts, maps);
    out.首次发布代次 = r.G0 + 1;
    dispatched = false;
    out.Gread = 当前();
    for (std::size_t i = 0; i < parts.size(); ++i)
      核验事实(规范(parts[i].写集, maps), parts[i].所有者, i, maps,
               out.Gread, *out.首次发布代次);
    const auto first = 读取恢复材料(恢复请求(r, out.Gread), c);
    完整要求(first.请求 && *first.请求 == r &&
             first.发布代次 == *out.首次发布代次 && first.映射 == maps);
    out.投影 = 读取出生投影(r, out.Gread, *out.首次发布代次, maps, c);
    out.状态 = saved.状态 == L::精确重复 ? W::精确重复 : W::已创建;
    完整要求(out.成功(r));
  } catch (S s) { out.绑定原因 = s; fail(映射状态(s)); }
    catch (P s) { out.概念原因 = s; fail(映射状态(s)); }
    catch (C s) { out.概念引用原因 = s; fail(映射状态(s)); }
    catch (W s) { fail(s); }
    catch (const std::bad_alloc &) { fail(W::资源失败); }
    catch (const std::length_error &) { fail(W::资源失败); }
    catch (...) { fail(W::内部不一致); }
  return out;
}

std::vector<L1有限N分区原子参与者写集_v3>
inline 绑定存在数据服务::反建原组合片段(const 恢复材料 &m) {
  完整要求(m.请求 && m.投影 && m.发布代次 > 1);
  const auto &r = *m.请求;
  const auto &projection = *m.投影;
  const bool single = r.绑定.种类 == K::父存在组成;
  const auto count = single ? 2U : 3U;
  const auto last = count - 1;
  const auto E = 编码(m.映射, 0, {1});
  const L1所有者范围事实引用 ownE{L1所有者范围写集本地键{1}};
  const L1所有者范围事实引用 stableE{E};
  auto unchanged = [](const L1所有者范围事实引用 &v) -> L1有限N分区原子事实引用值_v3 {
    if (const auto *s = std::get_if<稳定编码>(&v)) return *s;
    return std::get<L1所有者范围写集本地键>(v);
  };
  std::vector<L1有限N分区原子参与者写集_v3> parts;
  parts.reserve(count);
  for (std::uint32_t i = 0; i < count; ++i) {
    完整要求(m.首次[i].首次规范化写集.has_value());
    const auto &w = *m.首次[i].首次规范化写集;
    L1有限N分区原子参与者写集_v3 part{{static_cast<std::uint8_t>(i + 1)}, m.首次[i].所有者,
        {w.合同版本, w.期望事实代次, w.写入幂等身份}};
    for (const auto &n : w.节点)
      part.写集.节点.push_back({n.本地键, n.种类, n.属性类型表示});
    for (const auto &e : w.关系) {
      const auto key = e.本地键.值;
      bool sourceE = false, targetE = false;
      完整要求(std::holds_alternative<稳定编码>(e.关系类型节点) &&
               有效(std::get<稳定编码>(e.关系类型节点)), W::幂等冲突);
      if (i == 0) {
        完整要求(e.角色或顺序 == 1, W::幂等冲突);
        if (key == 2) {
          完整要求(e.源节点 == ownE &&
                   e.目标节点 == L1所有者范围事实引用{projection.存在身份.族锚点} &&
                   e.关系类型节点 == L1所有者范围事实引用{projection.存在身份.族归属关系类型},
                   W::幂等冲突);
        } else if (single && key == 3) {
          完整要求(e.源节点 == L1所有者范围事实引用{r.绑定.绑定节点} &&
                   e.目标节点 == ownE, W::幂等冲突);
        } else {
          完整要求(r.初始角色 && key == (single ? 4U : 3U) &&
                   e.源节点 == L1所有者范围事实引用{r.初始角色->值} &&
                   e.目标节点 == ownE, W::幂等冲突);
        }
      } else if (i == last) {
        完整要求(key == 1 && e.角色或顺序 == 1 && e.源节点 == stableE &&
                 e.目标节点 == L1所有者范围事实引用{r.EC.值}, W::幂等冲突);
        sourceE = true;
      } else if (r.绑定.种类 == K::场景成员) {
        完整要求(key == 1 && e.角色或顺序 == 1 &&
                 e.源节点 == L1所有者范围事实引用{r.绑定.绑定节点} &&
                 e.目标节点 == stableE, W::幂等冲突);
        targetE = true;
      } else {
        完整要求(projection.场景 && key >= 5 && key <= 12, W::幂等冲突);
        const auto &scene = *projection.场景;
        if (key == 11) {
          完整要求(e.源节点 == L1所有者范围事实引用{r.绑定.绑定节点} &&
                   e.目标节点 == stableE && e.角色或顺序 == 1, W::幂等冲突);
          targetE = true;
        } else {
          完整要求(e.源节点 == stableE, W::幂等冲突);
          sourceE = true;
          if (key == 5)
            完整要求(e.目标节点 == L1所有者范围事实引用{scene.场景角色.场景族锚点.编码} &&
                     e.关系类型节点 == L1所有者范围事实引用{scene.场景角色.场景族归属类型.编码} &&
                     e.角色或顺序 == 1, W::幂等冲突);
          else if (key <= 9)
            完整要求(e.目标节点 == L1所有者范围事实引用{L1所有者范围写集本地键{key - 5}} &&
                     e.关系类型节点 == L1所有者范围事实引用{scene.场景角色.根绑定类型.编码} &&
                     e.角色或顺序 == key - 5, W::幂等冲突);
          else if (key == 10)
            完整要求(e.目标节点 == L1所有者范围事实引用{r.期望现实树根} &&
                     e.角色或顺序 == 1, W::幂等冲突);
          else
            完整要求(e.目标节点 == L1所有者范围事实引用{r.绑定.绑定节点} &&
                     scene.父语境投影 &&
                     e.关系类型节点 == L1所有者范围事实引用{scene.父语境投影->投影边.关系类型} &&
                     e.角色或顺序 == 1, W::幂等冲突);
        }
      }
      const L1有限N分区原子事实引用值_v3 newborn{L1有限N分区原子事实引用_v3{{1}, {1}}};
      part.写集.关系.push_back({e.本地键,
          sourceE ? newborn : unchanged(e.源节点),
          targetE ? newborn : unchanged(e.目标节点),
          unchanged(e.关系类型节点), e.角色或顺序});
    }
    完整要求(规范(part.写集, m.映射) == w, W::幂等冲突);
    parts.push_back(std::move(part));
  }
  return parts;
}

inline 已发布概念绑定恢复结果 绑定存在数据服务::读取已发布概念绑定创建(
    const 已发布概念绑定恢复请求 &r, 已发布概念引用参与者 &c) const noexcept {
  已发布概念绑定恢复结果 out;
  bool confirmed = false;
  auto fail = [&](W state) {
    out.状态 = state;
    out.投影.reset();
    if (!confirmed) out.原实际子请求.reset();
  };
  try {
    std::lock_guard lock(mutex_);
    out.Gread = 当前();
    要求(out.Gread == r.Gread, S::事实代次漂移);
    const auto prior = 读取恢复材料(r,c,&out.首次发布代次);
    if (!prior.请求) {
      完整要求(!prior.发布代次 && !prior.投影);
      out.状态 = W::未派发;
      return out;
    }
    out.首次发布代次 = prior.发布代次;
    const auto &q = *prior.请求;
    完整要求(q.G0 && q.G0 + 1 == prior.发布代次 &&
             prior.发布代次 <= r.Gread && q.G0 < r.Gread);
    const auto parts = 反建原组合片段(prior);
    const auto keys = 取键(q.键, q.绑定.种类);
    const auto last = parts.size() - 1;
    auto &conceptPort = c.借用存在概念引用写端口();
    完整要求(c.绑定于(content_.绑定存在底座()) && conceptPort.有效() &&
             conceptPort.绑定于(content_.绑定存在底座()) &&
             conceptPort.所有者身份() == parts[last].所有者);
    完整要求(content_.绑定存在端口().所有者身份() == parts[0].所有者);
    std::vector<const L1所有者范围写端口 *> ports;
    if (parts.size() == 3) {
      完整要求(scene_.绑定存在端口().所有者身份() == parts[1].所有者);
      ports.push_back(&scene_.绑定存在端口());
    }
    ports.push_back(&conceptPort);
    要求(当前() == r.Gread, S::事实代次漂移);
    // 只有真实首次账证明的旧 G 才能进入：未命中组合账也不能形成新写。
    const L1有限N分区原子事务请求_v3 transaction{
        L1有限N分区原子事务合同版本_v3, q.G0, keys[3], parts};
    const auto saved = content_.绑定存在端口().提交有限N分区原子事务_v3(transaction, ports);
    using L = L1有限N分区原子事务状态_v3;
    if (saved.是否已确认形成内存权威发布 || saved.状态 == L::已可能发布 || saved.状态 == L::已提交)
      throw W::已可能发布;
    if (saved.状态 == L::事实代次漂移 || saved.状态 == L::幂等冲突) throw W::幂等冲突;
    if (saved.状态 == L::资源失败) throw W::资源失败;
    完整要求(saved.状态 == L::精确重复 &&
             saved.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛);
    Maps maps;
    核验提交映射(saved, q, parts, maps);
    完整要求(maps == prior.映射);
    confirmed = true;
    out.原实际子请求 = q;
    // 两组预算回显本次读取限额；首次账不含原预算。
    out.投影 = 读取出生投影(q, r.Gread, prior.发布代次, maps, c);
    out.状态 = W::已读取;
    完整要求(out.成功(r));
  } catch (S s) { out.绑定原因 = s; fail(映射状态(s)); }
    catch (P s) { out.概念原因 = s; fail(映射状态(s)); }
    catch (C s) { out.概念引用原因 = s; fail(映射状态(s)); }
    catch (W s) { fail(s); }
    catch (const std::bad_alloc &) { fail(W::资源失败); }
    catch (const std::length_error &) { fail(W::资源失败); }
    catch (...) { fail(W::内部不一致); }
  return out;
}
} // namespace 海中鱼巣
