module;
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
export module 海中鱼巣.领域.数据服务.绑定存在;
export import 海中鱼巣.领域.合同.场景角色组织;
export namespace 海中鱼巣 {
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
  绑定已退出 = 6,
  绑定类型不符 = 7,
  绑定不在现实树 = 8,
  包含冲突 = 9,
  成环 = 10,
  事实代次漂移 = 11,
  幂等冲突 = 12,
  数量预算不足 = 13,
  历史材料已清理 = 14,
  资源失败 = 15,
  内部不一致 = 16,
  已可能发布 = 17,
  既有操作已被后继事实覆盖 = 18
};

struct 绑定存在事实 final {
  存在初始绑定种类 种类{};
  稳定编码 绑定节点{}, 新存在{}, 绑定关系{};
};

struct 绑定存在创建结果 final {
  std::uint32_t 版本 = 1;
  绑定存在创建状态 状态 = 绑定存在创建状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布H;
  std::optional<绑定存在创建请求> 原请求;
  std::optional<绑定存在事实> 事实;
  bool 成功(const 绑定存在创建请求 &) const noexcept;
};

template <class T> struct 绑定存在参与者结果 final {
  绑定存在创建状态 状态 = 绑定存在创建状态::内部不一致;
  std::uint64_t Gread = 0, H = 0;
  std::optional<T> 数据;
};

struct 存在绑定出生见证 final {
  std::uint64_t Gread = 0, H = 0;
  稳定编码 新存在{};
  存在身份来源历史见证 存在身份{};
  std::optional<存在组成关系事实> 组成绑定关系;
};

struct 场景绑定出生见证 final {
  std::uint64_t Gread = 0, H = 0;
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
  准备存在出生片段(const 绑定存在创建请求 &, std::uint64_t Gread) const = 0;
  virtual 绑定存在参与者结果<L1所有者范围首次写入读取结果>
      读取存在出生首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual 绑定存在参与者结果<存在绑定出生见证>
  读取存在绑定出生(std::uint64_t Gread, std::uint64_t H, 稳定编码 新存在,
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
  读取场景绑定出生(std::uint64_t Gread, std::uint64_t H, 稳定编码 新存在,
                   const 存在初始绑定 &) const = 0;

public:
  virtual ~绑定存在场景参与者() = default;
};

} // namespace 海中鱼巣

export namespace 海中鱼巣 {
class 绑定存在数据服务 final {
  using S = 绑定存在创建状态;
  using K = 存在初始绑定种类;
  using Map = std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>;
  using Maps = std::array<Map, 2>;
  绑定存在内容参与者 &content_;
  绑定存在场景参与者 &scene_;
  直接归属联合只读组合器 joint_;
  std::mutex mutex_;
  static void 要求(bool ok, S s = S::内部不一致) {
    if (!ok)
      throw s;
  }
  template <class T>
  static T 取(const 绑定存在参与者结果<T> &r, std::uint64_t g,
              std::uint64_t h) {
    要求(r.状态 == S::精确重复, r.状态);
    要求(r.Gread == g && r.H == h && r.数据);
    return *r.数据;
  }
  std::uint64_t 当前() const {
    auto r = content_.绑定存在底座().读取中性当前事实代次({L1中性CRUD合同版本});
    要求(r.状态 == L1中性读取状态::成功,
         r.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致);
    要求(r.事实代次 != 0);
    return r.事实代次;
  }
  static void 核验现实见证(const 绑定现实树见证 &v, const 绑定存在创建请求 &r,
                           std::uint64_t g) {
    const auto &root = v.根角色;
    要求(v.Gread == g && v.绑定节点 == r.绑定.绑定节点 &&
         v.期望现实树根 == r.期望现实树根 && root.Gread == g && root.H == g &&
         root.场景 == r.期望现实树根 &&
         root.位置 == 直接归属场景位置::场景树根 &&
         root.树根 == r.期望现实树根 && root.树证明关系 &&
         有效(*root.树证明关系));
    要求(v.上行路径.size() <= r.预算.最大祖先数量 &&
             v.上行路径.size() <= r.预算.最大关系数量,
         S::数量预算不足);
    auto cursor = r.绑定.绑定节点;
    std::unordered_set<std::uint64_t> seen{cursor.值};
    for (const auto &edge : v.上行路径) {
      要求(edge.Gread == g && edge.H == g && edge.成员 == cursor &&
           有效(edge.父) && 有效(edge.关系) && edge.创建事实代次 &&
           edge.创建事实代次 <= g && !edge.退出事实代次);
      要求(seen.insert(edge.父.值).second, S::成环);
      cursor = edge.父;
    }
    要求(cursor == r.期望现实树根, S::绑定不在现实树);
  }
  static 稳定编码 编码(const Maps &maps, std::size_t part,
                       L1所有者范围写集本地键 key) {
    要求(part < 2);
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
      要求(x.参与者.值 && x.参与者.值 <= 2);
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
  static void 核验映射(const Maps &maps, K k) {
    std::unordered_set<std::uint64_t> ids;
    const std::size_t count0 = k == K::父存在组成 ? 3 : 2;
    const std::size_t count1 = k == K::父存在组成 ? 0
                               : k == K::场景成员 ? 1
                                                  : 12;
    for (std::size_t i = 0; i < 2; ++i) {
      const auto count = i ? count1 : count0;
      要求(maps[i].size() == count);
      for (std::uint32_t j = 1; j <= count; ++j)
        要求(ids.insert(编码(maps, i, {j}).值).second);
    }
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
      auto r = content_.绑定存在底座().读取所有者范围历史事实(
          {L1所有者范围CRUD合同版本, id});
      if (r.物理清理墓碑 || r.物理清理事实代次)
        throw S::历史材料已清理;
      要求(r.状态 == L1所有者范围读取状态::成功,
           r.状态 == L1所有者范围读取状态::资源失败 ? S::资源失败
                                                    : S::内部不一致);
      要求(r.读取事实代次 == g, S::事实代次漂移);
      要求(r.查询编码 == id && r.合同版本 == L1所有者范围CRUD合同版本 &&
           r.事实);
      return *r.事实;
    };
    for (const auto &n : w.节点) {
      auto f = raw(编码(maps, part, n.本地键));
      auto *p = std::get_if<L1所有者范围节点事实>(&f);
      要求(p && p->种类 == n.种类 && p->属性类型表示 == n.属性类型表示 &&
           p->写入所有者 == owner && p->创建事实代次 == h &&
           (!p->退出事实代次 || *p->退出事实代次 > h));
    }
    for (const auto &e : w.关系) {
      auto f = raw(编码(maps, part, e.本地键));
      auto *p = std::get_if<L1所有者范围关系事实>(&f);
      要求(p && p->源节点 == resolve(e.源节点) &&
           p->目标节点 == resolve(e.目标节点) &&
           p->关系类型节点 == resolve(e.关系类型节点) &&
           p->角色或顺序 == e.角色或顺序 && p->写入所有者 == owner &&
           p->创建事实代次 == h && (!p->退出事实代次 || *p->退出事实代次 > h));
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
  绑定存在创建结果 创建绑定存在(const 绑定存在创建请求 &r) noexcept;
};
inline bool 绑定存在创建结果::成功(const 绑定存在创建请求 &r) const noexcept {
  using S = 绑定存在创建状态;
  return 版本 == 1 && r.版本 == 1 &&
         (状态 == S::已创建 || 状态 == S::精确重复) && Gread && 首次发布H &&
         r.G0 < *首次发布H && *首次发布H <= Gread && 原请求 && *原请求 == r &&
         事实 && 事实->种类 == r.绑定.种类 &&
         事实->绑定节点 == r.绑定.绑定节点 && 有效(事实->新存在) &&
         有效(事实->绑定关系) && 事实->新存在 != 事实->绑定节点;
}
inline 绑定存在创建结果
绑定存在数据服务::创建绑定存在(const 绑定存在创建请求 &r) noexcept {
  绑定存在创建结果 out;
  bool dispatched = false;
  auto fail = [&](S s) {
    out.状态 = dispatched ? S::已可能发布 : s;
    out.事实.reset();
    if (out.状态 != S::已可能发布 && out.状态 != S::既有操作已被后继事实覆盖)
      out.首次发布H.reset();
    try {
      追根因检查(
          false,
          L"普通存在绑定创建未完成：请核对绑定节点、现实树根及结构化原因");
    } catch (...) {
    };
  };
  try {
    std::lock_guard lock(mutex_);
    out.原请求 = r;
    要求(有效(r.绑定.绑定节点) &&
             (r.绑定.种类 == K::父存在组成 || r.绑定.种类 == K::场景成员 ||
              r.绑定.种类 == K::直接子场景),
         S::绑定未明确);
    要求(r.版本 == 1 && r.G0 && r.G0 < UINT64_MAX && 有效(r.期望现实树根) &&
             !r.幂等键.valueless_by_exception(),
         S::入口拒绝);
    要求(r.预算.最大关系数量 && r.预算.最大场景数量 && r.预算.最大祖先数量 &&
             r.预算.最大关系数量 <= 4096 && r.预算.最大场景数量 <= 4096 &&
             r.预算.最大祖先数量 <= 4096,
         S::入口拒绝);
    const bool single = r.绑定.种类 == K::父存在组成;
    要求(single == std::holds_alternative<存在组成绑定创建键>(r.幂等键),
         S::入口拒绝);
    std::array<L1所有者范围写入幂等身份, 3> keys{};
    if (single)
      keys[0] = std::get<存在组成绑定创建键>(r.幂等键).幂等身份;
    else {
      const auto &k = std::get<存在场景绑定创建键>(r.幂等键);
      keys = {k.存在幂等身份, k.场景幂等身份, k.组合幂等身份};
    }
    要求(content_.绑定存在幂等键可用(keys[0]), S::入口拒绝);
    if (!single) {
      要求(scene_.绑定存在幂等键可用(keys[1]) && 有效(keys[2]), S::入口拒绝);
      要求(keys[0] != keys[1] && keys[0] != keys[2] && keys[1] != keys[2],
           S::入口拒绝);
    }
    out.Gread = 当前();
    要求(r.G0 <= out.Gread, S::事实代次漂移);
    const std::array<L1结构所有者身份, 2> owners{
        content_.绑定存在端口().所有者身份(),
        scene_.绑定存在端口().所有者身份()};
    std::array<L1所有者范围首次写入读取结果, 2> first;
    Maps maps;
    std::size_t found = 0;
    const auto count = single ? 1U : 2U;
    first[0] = 取(content_.读取存在出生首次材料(keys[0]), out.Gread, out.Gread);
    if (!single)
      first[1] = 取(scene_.读取场景绑定首次材料(keys[1]), out.Gread, out.Gread);
    for (std::size_t i = 0; i < count; ++i) {
      const auto &f = first[i];
      要求(f.读取事实代次 == out.Gread, S::事实代次漂移);
      要求(f.合同版本 == L1所有者范围首次写入读取合同版本 &&
           f.所有者 == owners[i] && f.写入幂等身份 == keys[i]);
      if (f.状态 == L1所有者范围读取状态::未找到) {
        要求(!f.首次规范化写集 && !f.首次写入结果);
        continue;
      }
      要求(f.状态 == L1所有者范围读取状态::成功,
           f.状态 == L1所有者范围读取状态::资源失败 ? S::资源失败
                                                    : S::内部不一致);
      要求(f.首次规范化写集 && f.首次写入结果 &&
           f.首次写入结果->状态 == L1所有者范围写入状态::成功 &&
           f.首次写入结果->合同版本 == L1所有者范围CRUD合同版本 &&
           f.首次写入结果->是否形成内存权威发布 &&
           f.首次写入结果->所有者 == owners[i] &&
           f.首次写入结果->写入幂等身份 == keys[i]);
      maps[i] = f.首次写入结果->新编码映射;
      ++found;
    }
    要求(found == 0 || found == count);
    const bool replay = found == count;
    if (replay) {
      out.首次发布H = first[0].首次写入结果->事实代次;
      要求(*out.首次发布H == r.G0 + 1, S::幂等冲突);
      if (!single)
        要求(first[1].首次写入结果->事实代次 == *out.首次发布H);
      核验映射(maps, r.绑定.种类);
      const auto &recorded = first[single ? 0 : 1].首次规范化写集->关系;
      const L1所有者范围写集本地键 bindingKey{
          single ? 3U : (r.绑定.种类 == K::场景成员 ? 1U : 11U)};
      auto edge =
          std::find_if(recorded.begin(), recorded.end(),
                       [&](const auto &e) { return e.本地键 == bindingKey; });
      要求(edge != recorded.end() &&
               edge->源节点 == L1所有者范围事实引用{r.绑定.绑定节点},
           S::幂等冲突);
      if (r.绑定.种类 == K::直接子场景) {
        auto rootEdge =
            std::find_if(recorded.begin(), recorded.end(),
                         [](const auto &e) { return e.本地键.值 == 10; });
        要求(rootEdge != recorded.end() &&
                 rootEdge->目标节点 == L1所有者范围事实引用{r.期望现实树根},
             S::幂等冲突);
      }

    } else {
      要求(out.Gread == r.G0, S::事实代次漂移);
      核验现实见证(取(scene_.核验绑定现实树(out.Gread, r.绑定, r.期望现实树根,
                                            r.预算, joint_),
                      out.Gread, out.Gread),
                   r, out.Gread);
    }
    std::vector<L1有限N分区原子参与者写集_v3> parts;
    parts.push_back(
        取(content_.准备存在出生片段(r, out.Gread), out.Gread, r.G0));
    if (!single)
      parts.push_back(
          取(scene_.准备场景绑定片段(r, out.Gread), out.Gread, r.G0));
    for (std::size_t i = 0; i < count; ++i) {
      要求(parts[i].参与者.值 == i + 1 && parts[i].所有者 == owners[i] &&
           parts[i].写集.期望事实代次 == r.G0 &&
           parts[i].写集.写入幂等身份 == keys[i]);
      if (replay) {
        要求(规范(parts[i].写集, maps) == *first[i].首次规范化写集,
             S::幂等冲突);
        核验事实(*first[i].首次规范化写集, owners[i], i, maps, out.Gread,
                 *out.首次发布H);
      }
    }
    std::optional<L1所有者范围写集请求> singleRequest;
    std::optional<L1有限N分区原子事务请求_v3> multiRequest;
    std::vector<const L1所有者范围写端口 *> otherPorts;
    if (single)
      singleRequest = 规范(parts[0].写集, maps);
    else {
      multiRequest = L1有限N分区原子事务请求_v3{L1有限N分区原子事务合同版本_v3,
                                                r.G0, keys[2], parts};
      otherPorts.push_back(&scene_.绑定存在端口());
    }
    要求(当前() == out.Gread, S::事实代次漂移);
    dispatched = true;
    bool repeated = false;
    if (single) {
      const auto saved =
          content_.绑定存在端口().提交所有者范围中性写集(*singleRequest);
      using L = L1所有者范围写入状态;
      if (saved.状态 != L::成功 && saved.状态 != L::精确重复) {
        if (saved.是否形成内存权威发布 ||
            saved.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛)
          throw S::已可能发布;
        dispatched = false;
        switch (saved.状态) {
        case L::事实代次漂移:
          throw S::事实代次漂移;
        case L::幂等冲突:
          throw S::幂等冲突;
        case L::资源失败:
          throw S::资源失败;
        case L::入口拒绝:
          throw S::入口拒绝;
        default:
          throw S::内部不一致;
        }
      }
      要求(saved.合同版本 == L1所有者范围CRUD合同版本 &&
           (saved.状态 == L::精确重复 || saved.是否形成内存权威发布) &&
           saved.事实代次 == r.G0 + 1 && saved.所有者 == owners[0] &&
           saved.写入幂等身份 == keys[0]);
      if (replay)
        要求(saved.新编码映射 == maps[0]);
      maps[0] = saved.新编码映射;
      repeated = saved.状态 == L::精确重复;
    } else {
      const auto saved = content_.绑定存在端口().提交有限N分区原子事务_v3(
          *multiRequest, otherPorts);
      using L = L1有限N分区原子事务状态_v3;
      if (saved.状态 != L::已提交 && saved.状态 != L::精确重复) {
        if (saved.状态 == L::已可能发布 || saved.是否已确认形成内存权威发布 ||
            saved.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛)
          throw S::已可能发布;
        dispatched = false;
        switch (saved.状态) {
        case L::事实代次漂移:
          throw S::事实代次漂移;
        case L::幂等冲突:
          throw S::幂等冲突;
        case L::资源失败:
          throw S::资源失败;
        case L::入口拒绝:
          throw S::入口拒绝;
        default:
          throw S::内部不一致;
        }
      }
      要求(saved.合同版本 == 3 &&
           (saved.状态 == L::精确重复 || saved.是否已确认形成内存权威发布) &&
           saved.共同事实代次 == r.G0 + 1 &&
           saved.组合写入幂等身份 == keys[2] && saved.参与者结果组.size() == 2);
      Maps published;
      std::array<bool, 2> seen{};
      for (const auto &p : saved.参与者结果组) {
        要求(p.参与者.值 >= 1 && p.参与者.值 <= 2);
        auto i = p.参与者.值 - 1;
        要求(!seen[i] && p.所有者 == owners[i]);
        seen[i] = true;
        published[i] = p.新编码映射;
      }
      if (replay)
        要求(published == maps);
      maps = std::move(published);
      repeated = saved.状态 == L::精确重复;
    }
    核验映射(maps, r.绑定.种类);
    out.首次发布H = r.G0 + 1;
    dispatched = false;
    out.Gread = 当前();
    const auto E = 编码(maps, 0, {1});
    for (std::size_t i = 0; i < count; ++i)
      核验事实(规范(parts[i].写集, maps), owners[i], i, maps, out.Gread,
               *out.首次发布H);
    const auto relation =
        编码(maps, single ? 0 : 1,
             {single ? 3U : (r.绑定.种类 == K::场景成员 ? 1U : 11U)});
    const auto ev =
        取(content_.读取存在绑定出生(out.Gread, *out.首次发布H, E, r.绑定),
           out.Gread, *out.首次发布H);
    要求(ev.Gread == out.Gread && ev.H == *out.首次发布H && ev.新存在 == E &&
         存在身份来源历史见证完整(ev.存在身份, ev.H, E));
    要求(ev.存在身份.族归属关系 == 编码(maps, 0, {2}) &&
         ev.存在身份.节点生命周期.创建事实代次 == ev.H &&
         ev.组成绑定关系.has_value() == single);
    if (single) {
      const auto &e = *ev.组成绑定关系;
      要求(e.Gread == out.Gread && e.H == ev.H && e.父存在 == r.绑定.绑定节点 &&
           e.子存在 == E && e.关系 == relation && e.创建事实代次 == ev.H &&
           !e.退出事实代次);
    } else {
      const auto sv =
          取(scene_.读取场景绑定出生(out.Gread, *out.首次发布H, E, r.绑定),
             out.Gread, *out.首次发布H);
      const auto &e = sv.绑定关系;
      要求(sv.Gread == out.Gread && sv.H == ev.H && sv.种类 == r.绑定.种类 &&
           sv.新存在 == E && sv.绑定节点 == r.绑定.绑定节点 &&
           e.Gread == out.Gread && e.H == ev.H && e.父场景 == r.绑定.绑定节点 &&
           e.成员 == E && e.关系.编码 == relation &&
           e.关系.源 == r.绑定.绑定节点 && e.关系.目标 == E &&
           e.关系.角色或顺序 == 1 && e.关系.生命周期.创建事实代次 == ev.H &&
           !e.关系.生命周期.退出事实代次);
      要求(sv.子场景结构.has_value() == (r.绑定.种类 == K::直接子场景));
      if (sv.子场景结构) {
        const auto &n = *sv.子场景结构;
        要求(n.场景角色.Gread == out.Gread && n.场景角色.H == ev.H &&
             n.场景角色.场景 == E && n.树证明.树根 == r.期望现实树根 &&
             n.树证明.关系 == 编码(maps, 1, {10}) && n.直接父 &&
             n.直接父->关系 == relation &&
             n.从上游场景到本场景路径.size() == 1 &&
             n.从上游场景到本场景路径.front().关系 == relation &&
             n.父语境投影 && n.父语境投影->Gread == out.Gread &&
             n.父语境投影->H == ev.H && n.父语境投影->场景 == E &&
             n.父语境投影->父场景语境 == r.绑定.绑定节点 &&
             n.父语境投影->结构父.关系 == 编码(maps, 1, {11}) &&
             n.父语境投影->投影边.编码 == 编码(maps, 1, {12}) &&
             n.父语境投影->投影边.源 == E &&
             n.父语境投影->投影边.目标 == r.绑定.绑定节点 &&
             有效(n.父语境投影->投影边.关系类型) &&
             n.父语境投影->投影边.关系类型 !=
                 e.关系.关系类型 &&
             n.父语境投影->投影边.角色或顺序 == 1 &&
             n.父语境投影->投影边.生命周期.创建事实代次 == ev.H &&
             !n.父语境投影->投影边.生命周期.退出事实代次);
        for (std::uint32_t i = 0; i < 4; ++i)
          要求(n.场景角色.四根[i].根.编码 == 编码(maps, 1, {i + 1}) &&
               n.场景角色.四根[i].绑定.编码 == 编码(maps, 1, {i + 6}));
      }
    }
    const auto current =
        joint_.读取当前联合父({1, out.Gread, E, r.预算.最大关系数量});
    if (!current.父读取成功({1, out.Gread, E, r.预算.最大关系数量})) {
      using J = 直接归属联合只读状态;
      switch (current.状态) {
      case J::成员已退出:
        要求(current.Gread == out.Gread, S::事实代次漂移);
        throw out.Gread > *out.首次发布H ? S::既有操作已被后继事实覆盖
                                         : S::内部不一致;
      case J::事实代次漂移:
        throw S::事实代次漂移;
      case J::数量预算不足:
        throw S::数量预算不足;
      case J::历史材料已清理:
        throw S::历史材料已清理;
      case J::资源失败:
        throw S::资源失败;
      case J::成员未找到:
      case J::入口拒绝:
      case J::内部不一致:
      case J::已读取:
        throw S::内部不一致;
      }
      throw S::内部不一致;
    }
    if (!current.父 || current.父->父 != r.绑定.绑定节点 ||
        current.父->关系 != relation)
      throw out.Gread > *out.首次发布H ? S::既有操作已被后继事实覆盖
                                       : S::内部不一致;
    const auto reality = scene_.核验绑定现实树(out.Gread, r.绑定,
                                               r.期望现实树根, r.预算, joint_);
    if (reality.状态 == S::绑定不在现实树 || reality.状态 == S::绑定已退出) {
      要求(reality.Gread == out.Gread && reality.H == out.Gread);
      if (out.Gread > *out.首次发布H)
        throw S::既有操作已被后继事实覆盖;
    }
    核验现实见证(取(reality, out.Gread, out.Gread), r, out.Gread);
    要求(当前() == out.Gread, S::事实代次漂移);
    out.事实 = 绑定存在事实{r.绑定.种类, r.绑定.绑定节点, E, relation};
    out.状态 = repeated ? S::精确重复 : S::已创建;
    要求(out.成功(r));
  } catch (S s) {
    fail(s);
  } catch (const std::bad_alloc &) {
    fail(S::资源失败);
  } catch (const std::length_error &) {
    fail(S::资源失败);
  } catch (...) {
    fail(S::内部不一致);
  }
  return out;
}
} // namespace 海中鱼巣
