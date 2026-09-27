#include "数据服务.因果类.h"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <utility>

#include "数据服务.状态类.h"

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

bool 当前普通节点(const L1所有者范围一致节点读取结果项 *item,
                  稳定编码 id, L1结构所有者身份 owner) noexcept {
  return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
      && item->事实 && item->事实->编码 == id
      && item->事实->写入所有者 == owner
      && item->事实->种类 == 节点种类::普通
      && !item->事实->属性类型表示;
}

bool 当前属性类型节点(const L1所有者范围一致节点读取结果项 *item,
                      稳定编码 id, L1结构所有者身份 owner,
                      L1所有者范围值表示种类 repr) noexcept {
  return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
      && item->事实 && item->事实->编码 == id
      && item->事实->写入所有者 == owner
      && item->事实->种类 == 节点种类::属性类型
      && item->事实->属性类型表示 == repr;
}

因果数据状态 映射一致读取(L1所有者范围一致当前读取状态 s) noexcept {
  if (s == L1所有者范围一致当前读取状态::资源失败)
    return 因果数据状态::资源失败;
  if (s == L1所有者范围一致当前读取状态::入口拒绝)
    return 因果数据状态::入口拒绝;
  return 因果数据状态::内部不一致;
}

bool 浅层定义有效(const 因果定义 &d) noexcept {
  const auto type = static_cast<std::uint8_t>(d.类型);
  if ((type != 1 && type != 2) || d.参与者.角色组.empty()
      || d.条件组.empty() || d.结果组.empty())
    return false;
  if ((d.类型 == 因果类型::主动) != d.动作.has_value()) return false;
  for (const auto &role : d.参与者.角色组)
    if (!role.角色 || !有效(role.存在概念)) return false;
  for (const auto &condition : d.条件组)
    if (!condition.序号 || !condition.参与者角色
        || !有效(condition.特征概念)
        || condition.约束.valueless_by_exception())
      return false;
  for (const auto &result : d.结果组)
    if (!result.序号 || !result.承担者角色
        || !有效(result.特征概念)
        || result.约束.valueless_by_exception())
      return false;
  return true;
}

} // namespace

因果类数据服务::因果类数据服务(
    L1事实基座服务 &l1, L1所有者范围写端口 &&port,
    const 因果结构交付 &layout, const 因果概念核验提供者 &concepts,
    const 动态结构只读提供者 &dynamics,
    const 状态使用绑定只读提供者 &bindings,
    const 状态类数据服务 &states)
    : l1_(l1), port_(std::move(port)), owner_(port_.所有者身份()),
      layout_(layout), concepts_(concepts), dynamics_(dynamics),
      bindings_(bindings), states_(states) {
  if (!绑定于(l1_) || !有效(owner_) || !布局浅层有效() || !当前布局有效())
    throw std::invalid_argument("invalid causal configuration");
}

bool 因果类数据服务::绑定于(const L1事实基座服务 &s) const noexcept {
  return &s == &l1_ && port_.有效() && port_.绑定于(s)
      && concepts_.绑定于(s) && dynamics_.绑定于(s)
      && bindings_.绑定于(s) && states_.绑定于(s);
}

bool 因果类数据服务::布局浅层有效() const noexcept {
  const std::array ids{
      layout_.格式锚点, layout_.字段登记关系类型,
      layout_.因果族锚点, layout_.族归属关系类型,
      layout_.定义属性类型, layout_.参与者EC关系类型,
      layout_.条件FC关系类型, layout_.结果FC关系类型,
      layout_.参数FC关系类型, layout_.约束RC关系类型,
      layout_.动作DC关系类型, layout_.原因果关系类型,
      layout_.来源链关系类型, layout_.证据索引属性类型,
      layout_.证据锚点关系类型, layout_.证据存在关系类型,
      layout_.证据绑定关系类型};
  for (std::size_t i = 0; i < ids.size(); ++i) {
    if (!有效(ids[i])) return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j]) return false;
  }
  return true;
}

bool 因果类数据服务::当前布局有效() const {
  const std::array ids{
      layout_.格式锚点, layout_.字段登记关系类型,
      layout_.因果族锚点, layout_.族归属关系类型,
      layout_.定义属性类型, layout_.参与者EC关系类型,
      layout_.条件FC关系类型, layout_.结果FC关系类型,
      layout_.参数FC关系类型, layout_.约束RC关系类型,
      layout_.动作DC关系类型, layout_.原因果关系类型,
      layout_.来源链关系类型, layout_.证据索引属性类型,
      layout_.证据锚点关系类型, layout_.证据存在关系类型,
      layout_.证据绑定关系类型};
  L1所有者范围一致当前读取请求 q;
  q.所有者 = {owner_};
  q.节点.assign(ids.begin(), ids.end());
  q.源关系组 = {{layout_.格式锚点, layout_.字段登记关系类型}};
  const auto r = l1_.尝试读取所有者范围一致当前投影(q);
  if (r.状态 != L1所有者范围一致当前读取状态::成功
      || r.所有者.size() != 1 || r.所有者.front().查询所有者 != owner_
      || r.所有者.front().状态 != L1所有者范围一致当前读取项目状态::成功
      || !r.所有者.front().所有者事实
      || r.所有者.front().所有者事实->范围种类
          != L1所有者范围种类::独占结构范围)
    return false;

  for (const auto id : ids) {
    if (id == layout_.定义属性类型 || id == layout_.证据索引属性类型) {
      if (!当前属性类型节点(节点(r, id), id, owner_,
              L1所有者范围值表示种类::U64组))
        return false;
    } else if (!当前普通节点(节点(r, id), id, owner_)) {
      return false;
    }
  }

  const auto *fields = 源关系(r, layout_.格式锚点,
                              layout_.字段登记关系类型);
  if (!fields || fields->成员.size() != 15) return false;
  const std::array expected{
      layout_.因果族锚点, layout_.族归属关系类型,
      layout_.定义属性类型, layout_.参与者EC关系类型,
      layout_.条件FC关系类型, layout_.结果FC关系类型,
      layout_.参数FC关系类型, layout_.约束RC关系类型,
      layout_.动作DC关系类型, layout_.原因果关系类型,
      layout_.来源链关系类型, layout_.证据索引属性类型,
      layout_.证据锚点关系类型, layout_.证据存在关系类型,
      layout_.证据绑定关系类型};
  for (std::size_t i = 0; i < expected.size(); ++i) {
    const auto count = std::count_if(fields->成员.begin(), fields->成员.end(),
        [&](const auto &item) {
          const auto &edge = item.关系;
          return edge.写入所有者 == owner_
              && edge.源节点 == layout_.格式锚点
              && edge.关系类型节点 == layout_.字段登记关系类型
              && edge.目标节点 == expected[i]
              && edge.角色或顺序 == static_cast<std::int64_t>(i + 1);
        });
    if (count != 1) return false;
  }
  return true;
}

因果操作结果 因果类数据服务::失败头(因果数据状态 s) noexcept {
  return {s, 因果发布阶段::无写入};
}

因果单项结果 因果类数据服务::单项失败(因果数据状态 s) noexcept {
  return {失败头(s), std::nullopt};
}

因果组结果 因果类数据服务::组失败(因果数据状态 s) noexcept {
  return {失败头(s), {}};
}

因果证据结果 因果类数据服务::证据失败(因果数据状态 s) noexcept {
  return {失败头(s), {}};
}

因果单项结果 因果类数据服务::发布或复用因果(const 因果发布请求 &q) {
  if (!有效(q.幂等身份) || !浅层定义有效(q.定义))
    return 单项失败(因果数据状态::入口拒绝);
  // 定义核验、精确复用、写入与读回必须在同一 current-only 写入合同内闭合。
  return 单项失败(因果数据状态::依赖未实现);
}

因果单项结果 因果类数据服务::读取当前因果(
    const 因果当前读取请求 &q) const {
  if (!有效(q.身份)) return 单项失败(因果数据状态::入口拒绝);
  // 旧定义值编码尚未迁移为 current-only 的一次投影解码合同。
  return 单项失败(因果数据状态::依赖未实现);
}

因果组结果 因果类数据服务::查询精确定义(
    const 因果精确定义查询请求 &q) const {
  if (!浅层定义有效(q.定义)) return 组失败(因果数据状态::入口拒绝);
  return 组失败(因果数据状态::依赖未实现);
}

因果组结果 因果类数据服务::按特征概念查询因果(
    const 因果按特征查询请求 &q) const {
  const auto direction = static_cast<std::uint8_t>(q.方向);
  if (!有效(q.特征概念) || (direction != 1 && direction != 2))
    return 组失败(因果数据状态::入口拒绝);
  return 组失败(因果数据状态::依赖未实现);
}

因果证据结果 因果类数据服务::关联因果证据(
    const 因果证据关联请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.因果)
      || !有效(q.证据.发生锚点))
    return 证据失败(因果数据状态::入口拒绝);
  return 证据失败(因果数据状态::依赖未实现);
}

因果证据结果 因果类数据服务::读取因果证据(
    const 因果证据读取请求 &q) const {
  if (!有效(q.因果) || (q.发生锚点 && !有效(*q.发生锚点)))
    return 证据失败(因果数据状态::入口拒绝);
  return 证据失败(因果数据状态::依赖未实现);
}

因果证据结果 因果类数据服务::退出因果证据(
    const 因果证据退出请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.定位.因果)
      || !有效(q.定位.发生锚点))
    return 证据失败(因果数据状态::入口拒绝);
  return 证据失败(因果数据状态::依赖未实现);
}

因果单项结果 因果类数据服务::退出因果(const 因果退出请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.身份))
    return 单项失败(因果数据状态::入口拒绝);
  return 单项失败(因果数据状态::依赖未实现);
}

因果操作结果 因果类数据服务::确认当前因果结构身份(
    const 因果当前身份请求 &q) const {
  if (!有效(q.身份)) return 失败头(因果数据状态::入口拒绝);
  try {
    L1所有者范围一致当前读取请求 request;
    request.所有者 = {owner_};
    request.节点 = {q.身份.编码};
    request.源关系组 = {
        {q.身份.编码, layout_.族归属关系类型}};
    const auto r = l1_.尝试读取所有者范围一致当前投影(request);
    if (r.状态 != L1所有者范围一致当前读取状态::成功)
      return 失败头(映射一致读取(r.状态));
    const auto *n = 节点(r, q.身份.编码);
    if (n && n->状态 == L1所有者范围一致当前读取项目状态::未找到)
      return 失败头(因果数据状态::未找到);
    if (!当前普通节点(n, q.身份.编码, owner_))
      return 失败头(因果数据状态::内部不一致);
    const auto *family = 源关系(r, q.身份.编码, layout_.族归属关系类型);
    if (!family || family->成员.size() != 1)
      return 失败头(因果数据状态::内部不一致);
    const auto &edge = family->成员.front().关系;
    if (edge.写入所有者 != owner_ || edge.源节点 != q.身份.编码
        || edge.关系类型节点 != layout_.族归属关系类型
        || edge.目标节点 != layout_.因果族锚点
        || edge.角色或顺序 != 1)
      return 失败头(因果数据状态::内部不一致);
    return {因果数据状态::已读取, 因果发布阶段::无写入};
  } catch (...) {
    return 失败头(因果数据状态::资源失败);
  }
}

} // namespace 海中鱼巣
