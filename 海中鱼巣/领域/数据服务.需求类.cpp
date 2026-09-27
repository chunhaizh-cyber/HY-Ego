#include "数据服务.需求类.h"

#include <algorithm>
#include <array>
#include <set>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {
namespace {

const L1所有者范围一致节点读取结果项 *节点(
    const L1所有者范围一致当前读取结果 &r, 稳定编码 id) noexcept {
  const auto it = std::find_if(r.节点.begin(), r.节点.end(),
      [&](const auto &x) { return x.查询编码 == id; });
  return it == r.节点.end() ? nullptr : &*it;
}

const L1所有者范围一致源关系组读取结果项 *源关系(
    const L1所有者范围一致当前读取结果 &r,
    稳定编码 source, 稳定编码 type) noexcept {
  const auto it = std::find_if(r.源关系组.begin(), r.源关系组.end(),
      [&](const auto &x) {
        return x.源节点 == source && x.关系类型节点 == type;
      });
  return it == r.源关系组.end() ? nullptr : &*it;
}

const L1所有者范围一致目标关系组读取结果项 *目标关系(
    const L1所有者范围一致当前读取结果 &r,
    稳定编码 target, 稳定编码 type) noexcept {
  const auto it = std::find_if(r.目标关系组.begin(), r.目标关系组.end(),
      [&](const auto &x) {
        return x.目标节点 == target && x.关系类型节点 == type;
      });
  return it == r.目标关系组.end() ? nullptr : &*it;
}

bool 当前普通节点(const L1所有者范围一致节点读取结果项 *item,
                  稳定编码 id, L1结构所有者身份 owner) noexcept {
  return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
      && item->事实 && item->事实->编码 == id
      && item->事实->写入所有者 == owner
      && item->事实->种类 == 节点种类::普通
      && !item->事实->属性类型表示;
}

bool 当前属性类型节点(const L1所有者范围一致节点读取结果项 *item,
                      稳定编码 id, L1结构所有者身份 owner) noexcept {
  return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
      && item->事实 && item->事实->编码 == id
      && item->事实->写入所有者 == owner
      && item->事实->种类 == 节点种类::属性类型
      && item->事实->属性类型表示 == L1所有者范围值表示种类::I64;
}

需求类数据状态 映射一致读取(L1所有者范围一致当前读取状态 s) noexcept {
  if (s == L1所有者范围一致当前读取状态::资源失败)
    return 需求类数据状态::资源失败;
  if (s == L1所有者范围一致当前读取状态::入口拒绝)
    return 需求类数据状态::入口拒绝;
  return 需求类数据状态::内部不一致;
}

bool 角色有效(本能根角色 role) noexcept {
  return role == 本能根角色::安全 || role == 本能根角色::服务;
}

} // namespace

bool 需求结构登记结果::成功(const 需求结构登记请求 &) const noexcept {
  return (状态 == 需求类数据状态::已创建
          || 状态 == 需求类数据状态::精确重复)
      && 交付.has_value();
}

bool 本能根材料::完整() const noexcept {
  const std::array ids{根目标合同, 根需求, 根列表项, 实际特征,
      实际特征关系, 目标合同关系, 列表成员关系, 目标值};
  if (!角色有效(角色)
      || 目标I64值 != std::numeric_limits<std::int64_t>::max()
      || std::any_of(ids.begin(), ids.end(),
          [](稳定编码 id) { return !有效(id); }))
    return false;
  auto sorted = ids;
  std::sort(sorted.begin(), sorted.end());
  return std::adjacent_find(sorted.begin(), sorted.end()) == sorted.end();
}

bool 本能根材料结果::成功(const 本能根材料请求 &q) const noexcept {
  return (状态 == 本能根材料状态::已形成
          || 状态 == 本能根材料状态::已恢复
          || 状态 == 本能根材料状态::已读取)
      && 材料 && 材料->完整() && 材料->角色 == q.角色
      && 材料->实际特征 == q.实际特征;
}

bool 需求类记录结果::成功() const noexcept {
  return (状态 == 需求类数据状态::已创建
          || 状态 == 需求类数据状态::精确重复
          || 状态 == 需求类数据状态::已读取)
      && 记录 && 需求类记录闭合(*记录);
}

需求结构登记结果 需求类数据服务::登记需求结构(
    const L1事实基座服务 &l1, L1所有者范围写端口 &port,
    const 需求结构登记请求 &) {
  if (!port.有效() || !port.绑定于(l1))
    return {需求类数据状态::入口拒绝, std::nullopt};
  // 固定幂等登记的写后交付需要由当前 L1 提供“本地键映射一次读回”合同。
  return {需求类数据状态::未实现, std::nullopt};
}

需求类数据服务::需求类数据服务(
    const L1事实基座服务 &l1, const 特征类数据服务 &features,
    const 存在类数据服务 &existence, L1所有者范围写端口 &&port,
    需求结构交付 layout)
    : 第一层服务_(l1), 特征服务_(features), 存在服务_(existence),
      写入端口_(std::move(port)), 所有者_(写入端口_.所有者身份()),
      结构类型_(layout.普通结构), 根结构类型_(layout.根结构) {
  if (!绑定于(l1) || !有效(所有者_) || !结构类型有效() || !当前结构有效())
    throw std::invalid_argument("invalid demand data configuration");
}

bool 需求类数据服务::绑定于(const L1事实基座服务 &l1) const noexcept {
  return &l1 == &第一层服务_ && 写入端口_.有效() && 写入端口_.绑定于(l1)
      && 特征服务_.绑定于(l1) && 存在服务_.绑定于(l1);
}

bool 需求类数据服务::与需求服务同底座(
    const 需求类数据服务 &other) const noexcept {
  return 绑定于(第一层服务_) && other.绑定于(第一层服务_);
}

bool 需求类数据服务::与存在服务同底座(
    const 存在类数据服务 &other) const noexcept {
  return 绑定于(第一层服务_) && other.绑定于(第一层服务_);
}

bool 需求类数据服务::与特征服务同底座(
    const 特征类数据服务 &other) const noexcept {
  return 绑定于(第一层服务_) && other.绑定于(第一层服务_);
}

bool 需求类数据服务::结构类型有效() const noexcept {
  const std::array ids{结构类型_.所属存在关系类型,
      结构类型_.目标宿主关系类型, 结构类型_.静态目标特征关系类型,
      结构类型_.方向二次特征关系类型, 根结构类型_.根实际特征关系类型,
      根结构类型_.根目标合同关系类型, 根结构类型_.根列表成员关系类型,
      根结构类型_.根目标值属性类型};
  for (std::size_t i = 0; i < ids.size(); ++i) {
    if (!有效(ids[i])) return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j]) return false;
  }
  return true;
}

bool 需求类数据服务::当前结构有效() const {
  const std::array ids{结构类型_.所属存在关系类型,
      结构类型_.目标宿主关系类型, 结构类型_.静态目标特征关系类型,
      结构类型_.方向二次特征关系类型, 根结构类型_.根实际特征关系类型,
      根结构类型_.根目标合同关系类型, 根结构类型_.根列表成员关系类型,
      根结构类型_.根目标值属性类型};
  L1所有者范围一致当前读取请求 q;
  q.所有者 = {所有者_};
  q.节点.assign(ids.begin(), ids.end());
  const auto r = 第一层服务_.尝试读取所有者范围一致当前投影(q);
  if (r.状态 != L1所有者范围一致当前读取状态::成功
      || r.所有者.size() != 1 || r.所有者.front().查询所有者 != 所有者_
      || r.所有者.front().状态 != L1所有者范围一致当前读取项目状态::成功
      || !r.所有者.front().所有者事实
      || r.所有者.front().所有者事实->范围种类
          != L1所有者范围种类::独占结构范围)
    return false;
  for (std::size_t i = 0; i < ids.size(); ++i) {
    if (i + 1 == ids.size()) {
      if (!当前属性类型节点(节点(r, ids[i]), ids[i], 所有者_)) return false;
    } else if (!当前普通节点(节点(r, ids[i]), ids[i], 所有者_)) return false;
  }
  return true;
}

需求类记录结果 需求类数据服务::记录失败(需求类数据状态 s) noexcept {
  return {s, std::nullopt};
}

本能根材料结果 需求类数据服务::根失败(本能根材料状态 s) noexcept {
  return {s, std::nullopt};
}

L1所有者范围写入幂等身份 需求类数据服务::根写入身份(
    本能根角色 role) noexcept {
  return {role == 本能根角色::安全
      ? 0x415243484C345341ULL : 0x415243484C345356ULL};
}

需求类记录结果 需求类数据服务::新增需求记录(const 需求类新增请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.所属存在) || !有效(q.目标宿主)
      || !有效(q.静态目标特征) || !有效(q.方向二次特征.结点))
    return 记录失败(需求类数据状态::入口拒绝);
  // 端点跨 owner 核验、精确唯一性与创建必须在一个原子 current-only 合同内完成。
  return 记录失败(需求类数据状态::未实现);
}

需求类记录结果 需求类数据服务::从当前投影读取记录(
    需求类记录身份 id, const L1所有者范围一致当前读取结果 &r) const {
  const auto *n = 节点(r, id.值);
  if (n && n->状态 == L1所有者范围一致当前读取项目状态::未找到)
    return 记录失败(需求类数据状态::未找到);
  if (!当前普通节点(n, id.值, 所有者_))
    return 记录失败(需求类数据状态::内部不一致);

  需求类记录 out;
  out.身份 = id;
  const std::array types{结构类型_.所属存在关系类型,
      结构类型_.目标宿主关系类型, 结构类型_.静态目标特征关系类型,
      结构类型_.方向二次特征关系类型};
  std::array<稳定编码 *, 4> targets{&out.所属存在, &out.目标宿主,
      &out.静态目标特征, &out.方向二次特征.结点};
  std::array<稳定编码 *, 4> edges{&out.所属存在关系, &out.目标宿主关系,
      &out.静态目标特征关系, &out.方向二次特征关系};
  for (std::size_t i = 0; i < types.size(); ++i) {
    const auto *group = 源关系(r, id.值, types[i]);
    if (!group || group->成员.size() != 1)
      return 记录失败(需求类数据状态::内部不一致);
    const auto &edge = group->成员.front().关系;
    if (edge.写入所有者 != 所有者_ || edge.源节点 != id.值
        || edge.关系类型节点 != types[i] || edge.角色或顺序 != 0
        || edge.目标节点 != group->成员.front().对端节点.编码)
      return 记录失败(需求类数据状态::内部不一致);
    *targets[i] = edge.目标节点;
    *edges[i] = edge.编码;
  }
  return 需求类记录闭合(out)
      ? 需求类记录结果{需求类数据状态::已读取, std::move(out)}
      : 记录失败(需求类数据状态::内部不一致);
}

需求类记录结果 需求类数据服务::查询需求记录(
    const 需求类身份查询请求 &q) const {
  if (!有效(q.需求.值)) return 记录失败(需求类数据状态::入口拒绝);
  try {
    L1所有者范围一致当前读取请求 request;
    request.所有者 = {所有者_};
    request.节点 = {q.需求.值};
    request.源关系组 = {{q.需求.值, 结构类型_.所属存在关系类型},
        {q.需求.值, 结构类型_.目标宿主关系类型},
        {q.需求.值, 结构类型_.静态目标特征关系类型},
        {q.需求.值, 结构类型_.方向二次特征关系类型}};
    const auto r = 第一层服务_.尝试读取所有者范围一致当前投影(request);
    if (r.状态 != L1所有者范围一致当前读取状态::成功)
      return 记录失败(映射一致读取(r.状态));
    return 从当前投影读取记录(q.需求, r);
  } catch (...) {
    return 记录失败(需求类数据状态::资源失败);
  }
}

需求类记录结果 需求类数据服务::按所属存在和目标精确查询(
    const 需求类精确查询请求 &q) const {
  if (!有效(q.所属存在) || !有效(q.目标宿主)
      || !有效(q.静态目标特征) || !有效(q.方向二次特征.结点))
    return 记录失败(需求类数据状态::入口拒绝);
  try {
    L1所有者范围一致当前读取请求 request;
    request.所有者 = {所有者_};
    request.目标关系组 = {{q.所属存在, 结构类型_.所属存在关系类型},
        {q.目标宿主, 结构类型_.目标宿主关系类型},
        {q.静态目标特征, 结构类型_.静态目标特征关系类型},
        {q.方向二次特征.结点, 结构类型_.方向二次特征关系类型}};
    const auto r = 第一层服务_.尝试读取所有者范围一致当前投影(request);
    if (r.状态 != L1所有者范围一致当前读取状态::成功)
      return 记录失败(映射一致读取(r.状态));

    const std::array targets{q.所属存在, q.目标宿主,
        q.静态目标特征, q.方向二次特征.结点};
    const std::array types{结构类型_.所属存在关系类型,
        结构类型_.目标宿主关系类型, 结构类型_.静态目标特征关系类型,
        结构类型_.方向二次特征关系类型};
    std::optional<std::set<std::uint64_t>> intersection;
    for (std::size_t i = 0; i < types.size(); ++i) {
      const auto *group = 目标关系(r, targets[i], types[i]);
      if (!group) return 记录失败(需求类数据状态::内部不一致);
      std::set<std::uint64_t> current;
      for (const auto &member : group->成员) {
        const auto &edge = member.关系;
        if (edge.写入所有者 != 所有者_ || edge.目标节点 != targets[i]
            || edge.关系类型节点 != types[i] || edge.角色或顺序 != 0
            || edge.源节点 != member.对端节点.编码
            || member.对端节点.写入所有者 != 所有者_
            || member.对端节点.种类 != 节点种类::普通
            || member.对端节点.属性类型表示)
          return 记录失败(需求类数据状态::内部不一致);
        if (!current.insert(edge.源节点.值).second)
          return 记录失败(需求类数据状态::内部不一致);
      }
      if (!intersection) intersection = std::move(current);
      else {
        std::set<std::uint64_t> next;
        std::set_intersection(intersection->begin(), intersection->end(),
            current.begin(), current.end(), std::inserter(next, next.end()));
        intersection = std::move(next);
      }
    }
    if (!intersection || intersection->empty())
      return 记录失败(需求类数据状态::未找到);
    if (intersection->size() != 1)
      return 记录失败(需求类数据状态::内部不一致);

    const 稳定编码 demand{*intersection->begin()};
    需求类记录 out{{demand}, q.所属存在, q.目标宿主,
        q.静态目标特征, q.方向二次特征};
    std::array<稳定编码 *, 4> edges{&out.所属存在关系, &out.目标宿主关系,
        &out.静态目标特征关系, &out.方向二次特征关系};
    for (std::size_t i = 0; i < types.size(); ++i) {
      const auto *group = 目标关系(r, targets[i], types[i]);
      const auto it = std::find_if(group->成员.begin(), group->成员.end(),
          [&](const auto &member) { return member.关系.源节点 == demand; });
      if (it == group->成员.end())
        return 记录失败(需求类数据状态::内部不一致);
      *edges[i] = it->关系.编码;
    }
    return 需求类记录闭合(out)
        ? 需求类记录结果{需求类数据状态::已读取, std::move(out)}
        : 记录失败(需求类数据状态::内部不一致);
  } catch (...) {
    return 记录失败(需求类数据状态::资源失败);
  }
}

本能根材料结果 需求类数据服务::建立或读取本能根材料(
    const 本能根材料请求 &q) {
  if (!角色有效(q.角色) || !有效(q.实际特征))
    return 根失败(本能根材料状态::入口拒绝);
  // 根创建需要固定幂等写入与完整 current-only 读回；旧恢复路径不再使用。
  return 根失败(本能根材料状态::未实现);
}

本能根材料结果 需求类数据服务::读取本能根材料(
    const 本能根材料请求 &q) const {
  if (!角色有效(q.角色) || !有效(q.实际特征))
    return 根失败(本能根材料状态::入口拒绝);
  return 根失败(本能根材料状态::未实现);
}

} // namespace 海中鱼巣
