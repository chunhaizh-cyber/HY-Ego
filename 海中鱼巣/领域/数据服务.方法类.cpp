#include "数据服务.方法类.h"

#include <algorithm>
#include <array>
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

bool 当前普通节点(const L1所有者范围一致节点读取结果项 *item,
                  稳定编码 id, L1结构所有者身份 owner) noexcept {
  return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
      && item->事实 && item->事实->编码 == id
      && item->事实->写入所有者 == owner
      && item->事实->种类 == 节点种类::普通
      && !item->事实->属性类型表示;
}

bool 参数类别有效(方法参数类别 value) noexcept {
  return value == 方法参数类别::场景 || value == 方法参数类别::存在
      || value == 方法参数类别::特征;
}

} // namespace

bool 方法类记录结果::成功() const noexcept {
  if (状态 != 方法类数据状态::已创建
      && 状态 != 方法类数据状态::精确重复
      && 状态 != 方法类数据状态::已读取)
    return false;
  if (!记录 || !有效(记录->身份.值) || !有效(记录->来源节点)
      || !有效(记录->来源关系))
    return false;
  if (记录->虚拟存在
      && (记录->虚拟存在->方法节点 != 记录->身份.值
          || !有效(记录->虚拟存在->虚拟存在节点)
          || !有效(记录->虚拟存在->方法虚拟存在关系)))
    return false;
  return std::all_of(记录->条件组.begin(), 记录->条件组.end(),
             [&](const auto &item) {
               return item.方法 == 记录->身份 && 方法类条件项完整(item);
             })
      && std::all_of(记录->结果方向组.begin(), 记录->结果方向组.end(),
             [&](const auto &item) {
               return item.方法 == 记录->身份
                   && 方法类结果方向项完整(item);
             })
      && std::all_of(记录->参数规格组.begin(), 记录->参数规格组.end(),
             [&](const auto &item) {
               return item.方法 == 记录->身份
                   && 方法类参数规格项完整(item);
             });
}

bool 方法类虚拟存在结果::成功() const noexcept {
  return (状态 == 方法类数据状态::已初始化方法虚拟存在
          || 状态 == 方法类数据状态::精确重复
          || 状态 == 方法类数据状态::方法虚拟存在已初始化)
      && 提供者状态 == 方法虚拟存在专用状态::已读取
      && 有效(方法.值) && 虚拟存在
      && 虚拟存在->方法节点 == 方法.值
      && 有效(虚拟存在->虚拟存在节点)
      && 有效(虚拟存在->方法虚拟存在关系);
}

bool 方法类条件结果::成功() const noexcept {
  return (状态 == 方法类数据状态::已添加条件项
          || 状态 == 方法类数据状态::精确重复)
      && 条件项 && 方法类条件项完整(*条件项);
}

bool 方法类条件组结果::成功() const noexcept {
  if (状态 != 方法类数据状态::已读取条件组 || !有效(方法.值))
    return false;
  for (std::size_t i = 0; i < 条件组.size(); ++i) {
    if (条件组[i].方法 != 方法 || !方法类条件项完整(条件组[i]))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (条件组[j].条件特征 == 条件组[i].条件特征) return false;
  }
  return true;
}

bool 方法类结果方向结果::成功() const noexcept {
  return (状态 == 方法类数据状态::已添加结果方向项
          || 状态 == 方法类数据状态::精确重复)
      && 结果方向项 && 方法类结果方向项完整(*结果方向项);
}

bool 方法类结果方向组结果::成功() const noexcept {
  if (状态 != 方法类数据状态::已读取结果方向组 || !有效(方法.值))
    return false;
  for (std::size_t i = 0; i < 结果方向组.size(); ++i) {
    if (结果方向组[i].方法 != 方法
        || !方法类结果方向项完整(结果方向组[i]))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (结果方向组[j].行动变化方向 == 结果方向组[i].行动变化方向
          && 结果方向组[j].结果方向 == 结果方向组[i].结果方向)
        return false;
  }
  return true;
}

bool 方法类参数规格结果::成功() const noexcept {
  return (状态 == 方法类数据状态::已添加参数规格
          || 状态 == 方法类数据状态::精确重复)
      && 参数规格 && 方法类参数规格项完整(*参数规格);
}

bool 方法类参数规格组结果::成功() const noexcept {
  if (状态 != 方法类数据状态::已读取参数规格组
      || !有效(方法.值) || !有效(方法虚拟存在节点))
    return false;
  for (std::size_t i = 0; i < 参数规格组.size(); ++i) {
    const auto &item = 参数规格组[i];
    if (!方法类参数规格项完整(item) || item.方法 != 方法
        || item.方法虚拟存在节点 != 方法虚拟存在节点
        || item.顺序 != i + 1)
      return false;
  }
  return true;
}

方法类数据服务::方法类数据服务(
    const L1事实基座服务 &l1, const 特征类数据服务 &features,
    const 存在类数据服务 &existence, L1所有者范围写端口 &&port,
    方法类结构类型 layout)
    : 第一层服务_(l1), 特征服务_(features), 存在服务_(existence),
      写入端口_(std::move(port)), 所有者_(写入端口_.所有者身份()),
      结构类型_(layout) {
  if (!绑定于(l1) || !有效(所有者_) || !结构类型有效() || !当前结构有效())
    throw std::invalid_argument("invalid method data configuration");
}

bool 方法类数据服务::绑定于(const L1事实基座服务 &l1) const noexcept {
  return &l1 == &第一层服务_ && 写入端口_.有效() && 写入端口_.绑定于(l1)
      && 特征服务_.绑定于(l1) && 存在服务_.绑定于(l1);
}

bool 方法类数据服务::结构类型有效() const noexcept {
  const std::array ids{
      结构类型_.方法来源关系类型, 结构类型_.方法虚拟存在关系类型,
      结构类型_.方法条件关系类型, 结构类型_.条件特征关系类型,
      结构类型_.方法结果方向关系类型, 结构类型_.结果行动方向关系类型,
      结构类型_.结果结果方向关系类型,
      结构类型_.场景参数特征类型关系类型,
      结构类型_.存在参数特征类型关系类型,
      结构类型_.特征参数特征类型关系类型};
  for (std::size_t i = 0; i < ids.size(); ++i) {
    if (!有效(ids[i])) return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j]) return false;
  }
  return true;
}

bool 方法类数据服务::当前结构有效() const {
  const std::array ids{
      结构类型_.方法来源关系类型, 结构类型_.方法虚拟存在关系类型,
      结构类型_.方法条件关系类型, 结构类型_.条件特征关系类型,
      结构类型_.方法结果方向关系类型, 结构类型_.结果行动方向关系类型,
      结构类型_.结果结果方向关系类型,
      结构类型_.场景参数特征类型关系类型,
      结构类型_.存在参数特征类型关系类型,
      结构类型_.特征参数特征类型关系类型};
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
  return std::all_of(ids.begin(), ids.end(), [&](稳定编码 id) {
    return 当前普通节点(节点(r, id), id, 所有者_);
  });
}

方法类记录结果 方法类数据服务::记录失败(方法类数据状态 s) noexcept {
  return {s, std::nullopt};
}

方法类虚拟存在结果 方法类数据服务::虚拟存在失败(
    方法类数据状态 s, 方法虚拟存在专用状态 provider,
    方法类记录身份 method) noexcept {
  return {s, provider, method, std::nullopt};
}

方法类条件结果 方法类数据服务::条件失败(方法类数据状态 s) noexcept {
  return {s, std::nullopt};
}

方法类条件组结果 方法类数据服务::条件组失败(
    方法类数据状态 s, 方法类记录身份 method) noexcept {
  return {s, method, {}};
}

方法类结果方向结果 方法类数据服务::结果失败(方法类数据状态 s) noexcept {
  return {s, std::nullopt};
}

方法类结果方向组结果 方法类数据服务::结果组失败(
    方法类数据状态 s, 方法类记录身份 method) noexcept {
  return {s, method, {}};
}

方法类参数规格结果 方法类数据服务::参数失败(方法类数据状态 s) noexcept {
  return {s, std::nullopt};
}

方法类参数规格组结果 方法类数据服务::参数组失败(
    方法类数据状态 s, 方法类记录身份 method) noexcept {
  return {s, method, {}, {}};
}

方法类记录结果 方法类数据服务::新增方法记录(const 方法类新增请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.来源节点))
    return 记录失败(方法类数据状态::入口拒绝);
  return 记录失败(方法类数据状态::未实现);
}

方法类记录结果 方法类数据服务::查询方法记录(
    const 方法类身份查询请求 &q) const {
  if (!有效(q.方法.值)) return 记录失败(方法类数据状态::入口拒绝);
  // 条件、结果方向和虚拟存在需要从首层关系发现后继续展开，当前缺少一次投影闭包合同。
  return 记录失败(方法类数据状态::未实现);
}

方法类记录结果 方法类数据服务::按来源节点精确查询(
    const 方法类精确查询请求 &q) const {
  if (!有效(q.来源节点)) return 记录失败(方法类数据状态::入口拒绝);
  return 记录失败(方法类数据状态::未实现);
}

方法类虚拟存在结果 方法类数据服务::初始化方法虚拟存在(
    const 方法类虚拟存在初始化请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.方法.值))
    return 虚拟存在失败(方法类数据状态::入口拒绝,
                         方法虚拟存在专用状态::入口拒绝, q.方法);
  try {
    L1所有者范围一致当前读取请求 request;
    request.所有者 = {所有者_};
    request.节点 = {q.方法.值};
    request.源关系组 = {
        {q.方法.值, 结构类型_.方法虚拟存在关系类型}};
    const auto r = 第一层服务_.尝试读取所有者范围一致当前投影(request);
    if (r.状态 == L1所有者范围一致当前读取状态::资源失败)
      return 虚拟存在失败(方法类数据状态::资源失败,
                           方法虚拟存在专用状态::资源失败, q.方法);
    if (r.状态 != L1所有者范围一致当前读取状态::成功)
      return 虚拟存在失败(方法类数据状态::内部不一致,
                           方法虚拟存在专用状态::内部不一致, q.方法);
    if (!当前普通节点(节点(r, q.方法.值), q.方法.值, 所有者_))
      return 虚拟存在失败(方法类数据状态::未找到,
                           方法虚拟存在专用状态::方法节点未找到, q.方法);
    const auto *group = 源关系(
        r, q.方法.值, 结构类型_.方法虚拟存在关系类型);
    if (!group)
      return 虚拟存在失败(方法类数据状态::内部不一致,
                           方法虚拟存在专用状态::内部不一致, q.方法);
    if (group->成员.empty()) {
      // 物理归属仍为“方法节点 -> 方法虚拟存在节点”；仅旧存在类专用写入口已退役。
      return 虚拟存在失败(方法类数据状态::未实现,
                           方法虚拟存在专用状态::未实现, q.方法);
    }
    if (group->成员.size() != 1)
      return 虚拟存在失败(方法类数据状态::内部不一致,
                           方法虚拟存在专用状态::结构冲突, q.方法);
    const auto &member = group->成员.front();
    const auto &edge = member.关系;
    if (edge.写入所有者 != 所有者_ || edge.源节点 != q.方法.值
        || edge.关系类型节点 != 结构类型_.方法虚拟存在关系类型
        || edge.目标节点 != member.对端节点.编码 || edge.角色或顺序 != 0
        || member.对端节点.写入所有者 != 所有者_
        || member.对端节点.种类 != 节点种类::普通
        || member.对端节点.属性类型表示)
      return 虚拟存在失败(方法类数据状态::内部不一致,
                           方法虚拟存在专用状态::结构冲突, q.方法);
    return {方法类数据状态::方法虚拟存在已初始化,
            方法虚拟存在专用状态::已读取,
            q.方法,
            方法虚拟存在投影{
                q.方法.值, edge.目标节点, edge.编码}};
  } catch (...) {
    return 虚拟存在失败(方法类数据状态::资源失败,
                         方法虚拟存在专用状态::资源失败, q.方法);
  }
}

方法类条件结果 方法类数据服务::添加条件项(
    const 方法类条件添加请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.方法.值) || !有效(q.条件特征))
    return 条件失败(方法类数据状态::入口拒绝);
  return 条件失败(方法类数据状态::未实现);
}

方法类条件组结果 方法类数据服务::读取条件项组(
    const 方法类条件组读取请求 &q) const {
  if (!有效(q.方法.值))
    return 条件组失败(方法类数据状态::入口拒绝, q.方法);
  return 条件组失败(方法类数据状态::未实现, q.方法);
}

方法类结果方向结果 方法类数据服务::添加结果方向项(
    const 方法类结果方向添加请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.方法.值)
      || !有效(q.行动变化方向.结点) || !有效(q.结果方向.结点))
    return 结果失败(方法类数据状态::入口拒绝);
  return 结果失败(方法类数据状态::未实现);
}

方法类结果方向组结果 方法类数据服务::读取结果方向项组(
    const 方法类结果方向组读取请求 &q) const {
  if (!有效(q.方法.值))
    return 结果组失败(方法类数据状态::入口拒绝, q.方法);
  return 结果组失败(方法类数据状态::未实现, q.方法);
}

方法类参数规格结果 方法类数据服务::添加方法参数规格(
    const 方法类参数规格添加请求 &q) {
  if (!有效(q.幂等身份) || !有效(q.方法.值)
      || !参数类别有效(q.类别) || !有效(q.参数特征类型) || !q.顺序)
    return 参数失败(!参数类别有效(q.类别)
        ? 方法类数据状态::参数类别无效
        : 方法类数据状态::入口拒绝);
  return 参数失败(方法类数据状态::未实现);
}

方法类参数规格组结果 方法类数据服务::读取方法参数规格组(
    const 方法类参数规格组读取请求 &q) const {
  if (!有效(q.方法.值))
    return 参数组失败(方法类数据状态::入口拒绝, q.方法);
  // 参数关系源仍是方法虚拟存在；发现该节点后还需第二层读取，不能连续读取伪造共同快照。
  return 参数组失败(方法类数据状态::未实现, q.方法);
}

} // namespace 海中鱼巣
