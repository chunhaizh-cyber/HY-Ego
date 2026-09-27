#include "任务治理.本能根任务核心.h"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {
namespace {

bool 根角色有效(本能根角色 role) noexcept {
  return role == 本能根角色::安全 || role == 本能根角色::服务;
}

bool 生命周期有效(本能根任务生命周期_v1 value) noexcept {
  return value >= 本能根任务生命周期_v1::当前可治理
      && value <= 本能根任务生命周期_v1::已退出当前资格;
}

bool 退出目标有效(本能根任务生命周期_v1 value) noexcept {
  return value == 本能根任务生命周期_v1::已完成
      || value == 本能根任务生命周期_v1::已失败
      || value == 本能根任务生命周期_v1::已取消
      || value == 本能根任务生命周期_v1::已退出当前资格;
}

bool 来源有效(const 本能根任务目标来源定位_v1 &source) noexcept {
  return 根角色有效(source.根角色) && 有效(source.D.值) && 有效(source.L)
      && 有效(source.根形成F);
}

bool 初始化请求有效(const 本能根任务初始化语义请求_v1 &q) noexcept {
  return 有效(q.意图.值) && 来源有效(q.来源)
      && (!q.前序任务 || 有效(q.前序任务->值));
}

bool 初始化包完整(const 不可变本能根任务初始化包_v1 &p) noexcept {
  return 初始化请求有效(p.原请求) && 有效(p.预留记录.值)
      && p.预留序号 != 0 && 有效(p.控制幂等身份)
      && 有效(p.任务核心建立幂等身份) && 有效(p.P1建立幂等身份)
      && 有效(p.Vt首迁移幂等身份);
}

bool 核心投影完整(const 本能根任务核心投影_v1 &core) noexcept {
  if (!有效(core.T.值) || !有效(core.L) || !有效(core.D.值)
      || !有效(core.T到D关系) || !有效(core.Vt) || !有效(core.R1.值)
      || !来源有效(core.首次来源) || !生命周期有效(core.生命周期)
      || (core.Vt状态 != 本能根任务Vt状态_v1::已建立待首轮准备
          && core.Vt状态 != 本能根任务Vt状态_v1::待找方法)
      || !有效(core.核心键) || !有效(core.P1键) || !有效(core.首迁移键))
    return false;
  if (core.T.值 == core.Vt || core.T.值 == core.R1.值 || core.Vt == core.R1.值)
    return false;
  return !core.P1 || (有效(core.P1->值) && core.P1->值 != core.T.值
      && core.P1->值 != core.Vt && core.P1->值 != core.R1.值);
}

const L1所有者范围一致节点读取结果项 *节点(
    const L1所有者范围一致当前读取结果 &r, 稳定编码 id) noexcept {
  const auto it = std::find_if(r.节点.begin(), r.节点.end(),
      [&](const auto &x) { return x.查询编码 == id; });
  return it == r.节点.end() ? nullptr : &*it;
}

bool 当前普通节点(const L1所有者范围一致节点读取结果项 *item,
                  稳定编码 id, L1结构所有者身份 owner) noexcept {
  return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
      && item->事实 && item->事实->编码 == id
      && item->事实->写入所有者 == owner
      && item->事实->种类 == 节点种类::普通
      && !item->事实->属性类型表示;
}

bool 当前属性节点(const L1所有者范围一致节点读取结果项 *item,
                  稳定编码 id, L1结构所有者身份 owner,
                  L1所有者范围值表示种类 repr) noexcept {
  return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
      && item->事实 && item->事实->编码 == id
      && item->事实->写入所有者 == owner
      && item->事实->种类 == 节点种类::属性类型
      && item->事实->属性类型表示 == repr;
}

} // namespace

bool 本能根任务核心结构登记结果_v1::成功(
    const 本能根任务核心结构登记请求_v1 &q) const noexcept {
  return 有效(q.幂等身份)
      && (状态 == 本能根任务阶段状态_v1::已发布
          || 状态 == 本能根任务阶段状态_v1::精确重复)
      && 交付.has_value();
}

bool 本能根任务初始化包结果_v1::成功(
    const 本能根任务初始化语义请求_v1 &q) const noexcept {
  return 原请求 == q
      && (状态 == 本能根任务阶段状态_v1::已发布
          || 状态 == 本能根任务阶段状态_v1::精确重复
          || 状态 == 本能根任务阶段状态_v1::已读取)
      && 包 && 包->原请求 == q && 初始化包完整(*包);
}

bool 本能根任务承接结果_v1::成功(
    const 不可变本能根任务初始化包_v1 &p) const noexcept {
  if (原包 != p || !初始化包完整(p) || !核心 || !核心投影完整(*核心))
    return false;
  const bool stages = (核心阶段 == 本能根任务阶段状态_v1::已发布
                       || 核心阶段 == 本能根任务阶段状态_v1::精确重复)
      && (P1阶段 == 本能根任务阶段状态_v1::已发布
          || P1阶段 == 本能根任务阶段状态_v1::精确重复)
      && (首迁移阶段 == 本能根任务阶段状态_v1::已发布
          || 首迁移阶段 == 本能根任务阶段状态_v1::精确重复);
  return stages && 核心->P1 && 核心->Vt状态 == 本能根任务Vt状态_v1::待找方法
      && (状态 == 本能根任务承接总状态_v1::已完成
          || 状态 == 本能根任务承接总状态_v1::精确重复
          || 状态 == 本能根任务承接总状态_v1::已承接到当前任务);
}

bool 本能根任务核心读取结果_v1::成功(
    const 本能根任务身份读取请求_v1 &q) const noexcept {
  return 有效(q.T.值) && 状态 == 本能根任务阶段状态_v1::已读取
      && 核心 && 核心->T == q.T && 核心投影完整(*核心);
}

bool 本能根任务核心读取结果_v1::成功(
    const 本能根任务锚点读取请求_v1 &q) const noexcept {
  return 有效(q.L) && 状态 == 本能根任务阶段状态_v1::已读取
      && 核心 && 核心->L == q.L && 核心投影完整(*核心);
}

bool 本能根任务目标投影结果_v1::成功(
    const 本能根任务目标投影读取请求_v1 &q) const noexcept {
  return 有效(q.T.值) && 状态 == 本能根任务阶段状态_v1::已读取
      && 投影 && 根角色有效(投影->根角色) && 有效(投影->D.值)
      && 有效(投影->L) && 有效(投影->目标宿主E)
      && 有效(投影->目标FT) && 有效(投影->目标状态合同);
}

bool 本能根任务当前资格退出结果_v1::成功(
    const 本能根任务当前资格退出请求_v1 &q) const noexcept {
  return 有效(q.T.值) && 有效(q.L) && 有效(q.幂等身份)
      && q.期望前生命周期 == 本能根任务生命周期_v1::当前可治理
      && 退出目标有效(q.目标生命周期)
      && (状态 == 本能根任务阶段状态_v1::已发布
          || 状态 == 本能根任务阶段状态_v1::精确重复)
      && T == q.T && L == q.L && 目标生命周期 == q.目标生命周期
      && 新生命周期值 && 有效(*新生命周期值)
      && 退出回执 && 有效(退出回执->值);
}

本能根任务核心结构登记结果_v1 本能根任务核心服务_v1::登记结构(
    const L1事实基座服务 &l1, L1所有者范围写端口 &port,
    const 本能根任务核心结构登记请求_v1 &q) noexcept {
  if (!port.有效() || !port.绑定于(l1) || !有效(q.幂等身份))
    return {本能根任务阶段状态_v1::入口拒绝, std::nullopt};
  return {本能根任务阶段状态_v1::未实现, std::nullopt};
}

本能根任务核心服务_v1::本能根任务核心服务_v1(
    const L1事实基座服务 &l1, const 需求类数据服务 &demand,
    const 存在类数据服务 &existence, const 特征类数据服务 &feature,
    L1所有者范围写端口 &&port,
    const 本能根任务核心结构交付_v1 &layout,
    存在单例角色身份 selfRole)
    : l1_(l1), 需求_(demand), 存在_(existence), 特征_(feature),
      写端口_(std::move(port)), owner_(写端口_.所有者身份()),
      结构_(layout), self角色_(selfRole) {
  if (!绑定于(l1_) || !有效(owner_) || owner_ != 结构_.task_owner
      || !有效(self角色_.值) || !结构浅层有效() || !当前结构有效())
    throw std::invalid_argument("invalid instinct-root task core configuration");
}

bool 本能根任务核心服务_v1::绑定于(
    const L1事实基座服务 &l1) const noexcept {
  return &l1 == &l1_ && 写端口_.有效() && 写端口_.绑定于(l1)
      && 需求_.绑定于(l1) && 存在_.绑定于(l1) && 特征_.绑定于(l1);
}

bool 本能根任务核心服务_v1::结构浅层有效() const noexcept {
  const std::array ids{结构_.任务族根, 结构_.预留记录族根,
      结构_.初始化回执族根, 结构_.任务族成员关系类型,
      结构_.任务查询锚点关系类型, 结构_.L当前任务关系类型,
      结构_.任务来源D关系类型, 结构_.任务Vt关系类型,
      结构_.任务R1关系类型, 结构_.R1的P1关系类型,
      结构_.预留记录成员关系类型, 结构_.初始化回执成员关系类型,
      结构_.私有高水位属性类型, 结构_.任务生命周期属性类型,
      结构_.Vt状态属性类型, 结构_.预留材料属性类型,
      结构_.根来源定位属性类型, 结构_.初始化回执材料属性类型};
  for (std::size_t i = 0; i < ids.size(); ++i) {
    if (!有效(ids[i])) return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j]) return false;
  }
  return true;
}

bool 本能根任务核心服务_v1::当前结构有效() const {
  const std::array ids{结构_.任务族根, 结构_.预留记录族根,
      结构_.初始化回执族根, 结构_.任务族成员关系类型,
      结构_.任务查询锚点关系类型, 结构_.L当前任务关系类型,
      结构_.任务来源D关系类型, 结构_.任务Vt关系类型,
      结构_.任务R1关系类型, 结构_.R1的P1关系类型,
      结构_.预留记录成员关系类型, 结构_.初始化回执成员关系类型,
      结构_.私有高水位属性类型, 结构_.任务生命周期属性类型,
      结构_.Vt状态属性类型, 结构_.预留材料属性类型,
      结构_.根来源定位属性类型, 结构_.初始化回执材料属性类型};
  L1所有者范围一致当前读取请求 q;
  q.所有者 = {owner_};
  q.节点.assign(ids.begin(), ids.end());
  const auto r = l1_.尝试读取所有者范围一致当前投影(q);
  if (r.状态 != L1所有者范围一致当前读取状态::成功
      || r.所有者.size() != 1 || r.所有者.front().查询所有者 != owner_
      || r.所有者.front().状态 != L1所有者范围一致当前读取项目状态::成功
      || !r.所有者.front().所有者事实
      || r.所有者.front().所有者事实->范围种类
          != L1所有者范围种类::独占结构范围)
    return false;
  for (std::size_t i = 0; i < ids.size(); ++i) {
    const auto *item = 节点(r, ids[i]);
    if (i < 12) {
      if (!当前普通节点(item, ids[i], owner_)) return false;
    } else {
      const auto repr = (i <= 14)
          ? L1所有者范围值表示种类::I64
          : L1所有者范围值表示种类::U64组;
      if (!当前属性节点(item, ids[i], owner_, repr)) return false;
    }
  }
  return true;
}

本能根任务初始化包结果_v1
本能根任务核心服务_v1::签发或恢复不可变初始化包(
    const 本能根任务初始化语义请求_v1 &q) noexcept {
  本能根任务初始化包结果_v1 out;
  out.原请求 = q;
  if (!初始化请求有效(q)) return out;
  // 预留号段、高水位和不可变包发布尚缺 current-only 原子合同。
  out.状态 = 本能根任务阶段状态_v1::未实现;
  return out;
}

本能根任务初始化包按意图读取结果_v1
本能根任务核心服务_v1::按初始化意图读取不可变包(
    const 本能根任务初始化包按意图读取请求_v1 &q) const noexcept {
  本能根任务初始化包按意图读取结果_v1 out;
  out.意图 = q.意图;
  out.状态 = 有效(q.意图.值)
      ? 本能根任务阶段状态_v1::未实现
      : 本能根任务阶段状态_v1::入口拒绝;
  return out;
}

本能根任务承接结果_v1 本能根任务核心服务_v1::承接或建立任务(
    const 不可变本能根任务初始化包_v1 &p) noexcept {
  本能根任务承接结果_v1 out;
  out.原包 = p;
  out.状态 = 初始化包完整(p)
      ? 本能根任务承接总状态_v1::未实现
      : 本能根任务承接总状态_v1::入口拒绝;
  return out;
}

本能根任务承接结果_v1 本能根任务核心服务_v1::恢复任务初始化(
    const 不可变本能根任务初始化包_v1 &p) noexcept {
  本能根任务承接结果_v1 out;
  out.原包 = p;
  out.状态 = 初始化包完整(p)
      ? 本能根任务承接总状态_v1::未实现
      : 本能根任务承接总状态_v1::入口拒绝;
  return out;
}

本能根任务核心读取结果_v1 本能根任务核心服务_v1::按任务读取核心(
    const 本能根任务身份读取请求_v1 &q) const noexcept {
  return {有效(q.T.值) ? 本能根任务阶段状态_v1::未实现
                       : 本能根任务阶段状态_v1::入口拒绝,
          std::nullopt};
}

本能根任务核心读取结果_v1
本能根任务核心服务_v1::按查询锚点读取当前任务(
    const 本能根任务锚点读取请求_v1 &q) const noexcept {
  return {有效(q.L) ? 本能根任务阶段状态_v1::未实现
                    : 本能根任务阶段状态_v1::入口拒绝,
          std::nullopt};
}

本能根任务目标投影结果_v1 本能根任务核心服务_v1::按任务读取目标投影(
    const 本能根任务目标投影读取请求_v1 &q) const noexcept {
  return {有效(q.T.值) ? 本能根任务阶段状态_v1::未实现
                       : 本能根任务阶段状态_v1::入口拒绝,
          std::nullopt};
}

本能根任务当前资格退出结果_v1 本能根任务核心服务_v1::退出任务当前资格(
    const 本能根任务当前资格退出请求_v1 &q) noexcept {
  本能根任务当前资格退出结果_v1 out;
  out.T = q.T;
  out.L = q.L;
  out.目标生命周期 = q.目标生命周期;
  if (!有效(q.T.值) || !有效(q.L) || !有效(q.幂等身份)
      || q.期望前生命周期 != 本能根任务生命周期_v1::当前可治理
      || !退出目标有效(q.目标生命周期))
    return out;
  out.状态 = 本能根任务阶段状态_v1::未实现;
  return out;
}

} // namespace 海中鱼巣
