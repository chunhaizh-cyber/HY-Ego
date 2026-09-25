#include "任务治理.本能根任务核心.h"

#include <algorithm>
#include <array>
#include <limits>
#include <new>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace 海中鱼巣 {
namespace {

using 阶段 = 本能根任务阶段状态_v1;
using 总状态 = 本能根任务承接总状态_v1;
using Key = L1所有者范围写集本地键;

constexpr L1所有者范围写入幂等身份 结构登记键{0x544F000000000001ULL};
constexpr L1所有者范围写入幂等身份 高水位首次键{0x544F000000000002ULL};
constexpr std::uint64_t 控制键基址 = 0x544F100000000000ULL;
constexpr std::uint64_t 阶段键基址 = 0x544F200000000000ULL;
constexpr std::uint64_t 最大预留序号 = 0x2AEC37FFFFFFFFFFULL;

bool 角色有效(本能根角色 r) noexcept {
  return r == 本能根角色::安全 || r == 本能根角色::服务;
}
bool 生命周期有效(本能根任务生命周期_v1 v) noexcept {
  return v >= 本能根任务生命周期_v1::当前可治理
      && v <= 本能根任务生命周期_v1::已退出当前资格;
}
bool 退出目标有效(本能根任务生命周期_v1 v) noexcept {
  return v == 本能根任务生命周期_v1::已完成
      || v == 本能根任务生命周期_v1::已失败
      || v == 本能根任务生命周期_v1::已取消
      || v == 本能根任务生命周期_v1::已退出当前资格;
}
bool 来源有效(const 本能根任务目标来源定位_v1& s) noexcept {
  return 角色有效(s.根角色) && 有效(s.D.值) && 有效(s.L)
      && s.来源截止H != 0 && 有效(s.根形成F.编码);
}
bool 初始化请求有效(const 本能根任务初始化语义请求_v1& r) noexcept {
  return r.合同版本 == 本能根任务核心合同版本_v1 && 有效(r.意图.值)
      && r.期望事实代次 != 0 && 来源有效(r.来源)
      && r.来源.来源截止H == r.期望事实代次
      && (!r.前序任务 || 有效(r.前序任务->值));
}
bool 初始化包有效(const 不可变本能根任务初始化包_v1& p) noexcept {
  if (!初始化请求有效(p.原请求) || !有效(p.预留记录.值)
      || p.预留序号 == 0 || p.预留序号 > 最大预留序号
      || !有效(p.控制幂等身份) || !有效(p.任务核心建立幂等身份)
      || !有效(p.P1建立幂等身份) || !有效(p.Vt首迁移幂等身份)
      || p.预留形成事实代次 == 0) return false;
  const auto control = 控制键基址 + p.预留序号;
  const auto base = 阶段键基址 + 4 * p.预留序号;
  return p.控制幂等身份.值 == control
      && p.任务核心建立幂等身份.值 == base
      && p.P1建立幂等身份.值 == base + 1
      && p.Vt首迁移幂等身份.值 == base + 2;
}

std::optional<std::uint64_t> 当前G(const L1事实基座服务& l1) noexcept {
  try {
    const auto q = l1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (q.状态 != L1中性读取状态::成功
        || q.合同版本 != L1中性CRUD合同版本 || q.事实代次 == 0) return std::nullopt;
    return q.事实代次;
  } catch (...) { return std::nullopt; }
}

阶段 映射写入状态(L1所有者范围写入状态 s) noexcept {
  switch (s) {
  case L1所有者范围写入状态::成功: return 阶段::已发布;
  case L1所有者范围写入状态::精确重复: return 阶段::精确重复;
  case L1所有者范围写入状态::入口拒绝: return 阶段::入口拒绝;
  case L1所有者范围写入状态::未找到: return 阶段::未找到;
  case L1所有者范围写入状态::事实代次漂移: return 阶段::事实代次漂移;
  case L1所有者范围写入状态::幂等冲突: return 阶段::幂等冲突;
  case L1所有者范围写入状态::引用冲突: return 阶段::引用冲突;
  case L1所有者范围写入状态::资源失败: return 阶段::资源失败;
  default: return 阶段::内部不一致;
  }
}
阶段 映射读取状态(L1所有者范围读取状态 s) noexcept {
  switch (s) {
  case L1所有者范围读取状态::入口拒绝: return 阶段::入口拒绝;
  case L1所有者范围读取状态::未找到: return 阶段::未找到;
  case L1所有者范围读取状态::已退出:
  case L1所有者范围读取状态::历史材料已清理: return 阶段::历史材料不可用;
  case L1所有者范围读取状态::事实代次漂移: return 阶段::事实代次漂移;
  case L1所有者范围读取状态::资源失败: return 阶段::资源失败;
  default: return 阶段::内部不一致;
  }
}
阶段 映射根历史状态(本能根历史材料状态_v1 s) noexcept {
  switch (s) {
  case 本能根历史材料状态_v1::入口拒绝: return 阶段::入口拒绝;
  case 本能根历史材料状态_v1::未找到: return 阶段::未找到;
  case 本能根历史材料状态_v1::已退出:
  case 本能根历史材料状态_v1::历史材料不可用: return 阶段::历史材料不可用;
  case 本能根历史材料状态_v1::事实代次漂移: return 阶段::事实代次漂移;
  case 本能根历史材料状态_v1::幂等冲突: return 阶段::幂等冲突;
  case 本能根历史材料状态_v1::引用冲突: return 阶段::引用冲突;
  case 本能根历史材料状态_v1::资源失败: return 阶段::资源失败;
  default: return 阶段::内部不一致;
  }
}
总状态 映射总状态(阶段 s) noexcept {
  switch (s) {
  case 阶段::入口拒绝: return 总状态::入口拒绝;
  case 阶段::未找到: return 总状态::未找到;
  case 阶段::当前任务不可复用: return 总状态::当前任务不可复用;
  case 阶段::目标不一致: return 总状态::目标不一致;
  case 阶段::事实代次漂移: return 总状态::事实代次漂移;
  case 阶段::幂等冲突: return 总状态::幂等冲突;
  case 阶段::引用冲突: return 总状态::引用冲突;
  case 阶段::历史材料不可用: return 总状态::历史材料不可用;
  case 阶段::已可能发布: return 总状态::已可能发布;
  case 阶段::号段耗尽: return 总状态::号段耗尽;
  case 阶段::待迁移: return 总状态::待迁移;
  case 阶段::资源失败: return 总状态::资源失败;
  default: return 总状态::内部不一致;
  }
}

bool 写入头完整(const L1所有者范围写入结果& w,
    L1结构所有者身份 owner, L1所有者范围写入幂等身份 key) noexcept {
  const bool first = w.状态 == L1所有者范围写入状态::成功
      && w.是否形成内存权威发布
      && w.重试边界 == L1所有者范围重试边界::不适用;
  const bool replay = w.状态 == L1所有者范围写入状态::精确重复
      && !w.是否形成内存权威发布
      && w.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
  return w.合同版本 == L1所有者范围CRUD合同版本 && w.所有者 == owner
      && w.写入幂等身份 == key && w.事实代次 != 0 && (first || replay);
}
bool 写入声称已发布(const L1所有者范围写入结果& w) noexcept {
  return w.是否形成内存权威发布
      ||w.状态==L1所有者范围写入状态::成功
      ||w.状态==L1所有者范围写入状态::精确重复;
}
阶段 核验首次读取头(const L1所有者范围首次写入读取结果& first,
    L1结构所有者身份 owner,L1所有者范围写入幂等身份 key,
    std::uint64_t Gread) noexcept {
  if(first.状态!=L1所有者范围读取状态::成功)
    return 映射读取状态(first.状态);
  if(first.读取事实代次!=Gread)return 阶段::事实代次漂移;
  if(first.合同版本!=L1所有者范围首次写入读取合同版本
      ||first.所有者!=owner||first.写入幂等身份!=key
      ||!first.首次规范化写集||!first.首次写入结果
      ||!写入头完整(*first.首次写入结果,owner,key))
    return 阶段::内部不一致;
  return 阶段::已读取;
}
bool 忽略G写集相同(const L1所有者范围写集请求& a,
    const L1所有者范围写集请求& b) {
  auto expected=b;
  std::sort(expected.节点.begin(),expected.节点.end(),[](const auto& x,const auto& y){
    return x.本地键<y.本地键;});
  std::sort(expected.关系.begin(),expected.关系.end(),[](const auto& x,const auto& y){
    return x.本地键<y.本地键;});
  std::sort(expected.值.begin(),expected.值.end(),[](const auto& x,const auto& y){
    return x.本地键<y.本地键;});
  const auto refKey=[](const L1所有者范围事实引用& ref){
    return std::visit([](const auto& value)->std::uint64_t{
      using T=std::decay_t<decltype(value)>;
      if constexpr(std::is_same_v<T,稳定编码>)return value.值;
      else return (1ULL<<63)|value.值;
    },ref);
  };
  std::sort(expected.属性槽变更.begin(),expected.属性槽变更.end(),
      [&](const auto& x,const auto& y){
        const auto xo=refKey(x.所属节点),yo=refKey(y.所属节点);
        if(xo!=yo)return xo<yo;
        const auto xt=refKey(x.属性类型节点),yt=refKey(y.属性类型节点);
        if(xt!=yt)return xt<yt;
        return x.新当前值<y.新当前值;
      });
  std::sort(expected.退出事实.begin(),expected.退出事实.end());
  return a.合同版本 == expected.合同版本
      && a.写入幂等身份 == expected.写入幂等身份
      && a.节点 == expected.节点 && a.关系 == expected.关系
      && a.值 == expected.值 && a.属性槽变更 == expected.属性槽变更
      && a.退出事实 == expected.退出事实;
}
std::optional<稳定编码> 映射编码(const L1所有者范围写入结果& w,
    std::uint32_t key) noexcept {
  std::optional<稳定编码> out;
  for (const auto& [k, id] : w.新编码映射) {
    if (k.值 != key) continue;
    if (out || !有效(id)) return std::nullopt;
    out = id;
  }
  return out;
}

template<class T>
bool 全部唯一有效(const T& values) {
  std::vector<std::uint64_t> ids;
  for (const auto id : values) {
    if (!有效(id)) return false;
    ids.push_back(id.值);
  }
  std::sort(ids.begin(), ids.end());
  return std::adjacent_find(ids.begin(), ids.end()) == ids.end();
}

struct 材料读取结果 final {
  阶段 状态 = 阶段::内部不一致;
  std::uint64_t Gread = 0;
  std::uint64_t 创建G = 0;
  std::optional<std::vector<std::uint64_t>> U64;
  std::optional<std::int64_t> I64;
  std::optional<稳定编码> 值身份;
};

材料读取结果 读取属性(const L1事实基座服务& l1, L1结构所有者身份 owner,
    稳定编码 node, 稳定编码 type, std::uint64_t Gread, std::uint64_t H,
    L1所有者范围值表示种类 kind) noexcept {
  材料读取结果 out; out.Gread = Gread;
  try {
    const L1所有者范围历史完整属性值组读取请求_v2 req{
        L1所有者范围历史完整属性值组读取合同版本, node, Gread, H};
    const auto q = l1.读取所有者范围历史完整属性值组(req);
    if (q.读取事实代次 != Gread) { out.状态 = 阶段::事实代次漂移; out.Gread=q.读取事实代次; return out; }
    if (!q.成功(req)) {
      using S = L1所有者范围历史完整属性值组读取状态_v2;
      switch (q.状态) {
      case S::入口拒绝: out.状态=阶段::入口拒绝; break;
      case S::未找到: out.状态=阶段::未找到; break;
      case S::已退出: case S::历史材料已清理: out.状态=阶段::历史材料不可用; break;
      case S::事实代次漂移: out.状态=阶段::事实代次漂移; break;
      case S::资源失败: out.状态=阶段::资源失败; break;
      default: out.状态=阶段::内部不一致; break;
      }
      return out;
    }
    const L1所有者范围值事实* found = nullptr;
    for (const auto& v : q.属性值组) {
      if (v.属性类型节点 != type) continue;
      if (found) return out;
      found = &v;
    }
    if (!found || found->所属节点 != node || found->属性类型节点 != type
        || found->写入所有者 != owner || found->创建事实代次 > H
        || (found->退出事实代次 && *found->退出事实代次 <= H)
        || !有效(found->编码)) return out;
    if (kind == L1所有者范围值表示种类::I64) {
      const auto* p = std::get_if<std::int64_t>(&found->材料);
      if (!p) return out;
      out.I64 = *p;
    } else if (kind == L1所有者范围值表示种类::U64组) {
      const auto* p = std::get_if<std::vector<std::uint64_t>>(&found->材料);
      if (!p) return out;
      out.U64 = *p;
    } else return out;
    out.值身份 = found->编码; out.创建G = found->创建事实代次;
    out.状态 = 阶段::已读取; return out;
  } catch (const std::bad_alloc&) { out.状态=阶段::资源失败; }
    catch (const std::length_error&) { out.状态=阶段::资源失败; }
    catch (...) { out.状态=阶段::内部不一致; }
  return out;
}

struct 关系读取结果 final {
  阶段 状态 = 阶段::内部不一致;
  std::uint64_t Gread = 0;
  std::vector<L1所有者范围关系事实> 关系;
};
关系读取结果 读取关系(const L1事实基座服务& l1, L1结构所有者身份 owner,
    L1所有者范围关系端点方向 direction, 稳定编码 endpoint, 稳定编码 type,
    std::uint64_t Gread, std::uint64_t H) noexcept {
  关系读取结果 out; out.Gread=Gread;
  try {
    const L1所有者范围历史完整关系组读取请求_v2 req{
        L1所有者范围历史完整关系组读取合同版本, direction,
        endpoint, type, Gread, H};
    const auto q=l1.读取所有者范围历史完整关系组(req);
    if (q.读取事实代次 != Gread) {out.状态=阶段::事实代次漂移;out.Gread=q.读取事实代次;return out;}
    if (!q.成功(req)) {
      using S=L1所有者范围历史完整关系组读取状态_v2;
      switch(q.状态) {
      case S::入口拒绝:out.状态=阶段::入口拒绝;break;
      case S::未找到:out.状态=阶段::未找到;break;
      case S::已退出:case S::历史材料已清理:out.状态=阶段::历史材料不可用;break;
      case S::事实代次漂移:out.状态=阶段::事实代次漂移;break;
      case S::资源失败:out.状态=阶段::资源失败;break;
      default:out.状态=阶段::内部不一致;break;
      }
      return out;
    }
    for(const auto& e:q.关系组) {
      if(e.写入所有者!=owner||e.关系类型节点!=type||e.角色或顺序!=0
          ||e.创建事实代次>H||(e.退出事实代次&&*e.退出事实代次<=H)
          || (direction == L1所有者范围关系端点方向::源
              ? e.源节点 != endpoint : e.目标节点 != endpoint))
        return out;
    }
    out.关系=q.关系组;out.状态=阶段::已读取;return out;
  } catch(const std::bad_alloc&){out.状态=阶段::资源失败;}
    catch(const std::length_error&){out.状态=阶段::资源失败;}
    catch(...){out.状态=阶段::内部不一致;}
  return out;
}

bool 结构字段完整(const 本能根任务核心结构交付_v1& s) {
  if(s.合同版本!=1||!有效(s.task_owner))return false;
  const std::array<稳定编码,18> ids{s.任务族根,s.预留记录族根,s.初始化回执族根,
    s.任务族成员关系类型,s.任务查询锚点关系类型,s.L当前任务关系类型,
    s.任务来源D关系类型,s.任务Vt关系类型,s.任务R1关系类型,s.R1的P1关系类型,
    s.预留记录成员关系类型,s.初始化回执成员关系类型,s.私有高水位属性类型,
    s.任务生命周期属性类型,s.Vt状态属性类型,s.预留材料属性类型,
    s.根来源定位属性类型,s.初始化回执材料属性类型};
  return 全部唯一有效(ids);
}

bool 核心投影完整(const 本能根任务核心投影_v1& c) noexcept {
  return 有效(c.T.值) && 有效(c.L) && 有效(c.D.值)
      && 有效(c.T到D关系) && 有效(c.Vt) && 有效(c.R1.值)
      && (!c.P1 || 有效(c.P1->值)) && 生命周期有效(c.生命周期)
      && 来源有效(c.首次来源) && c.D == c.首次来源.D
      && c.L == c.首次来源.L && 有效(c.核心键) && 有效(c.P1键)
      && 有效(c.首迁移键) && c.核心形成截止 != 0
      && (!c.P1形成截止 || (c.P1 && c.P1形成截止 >= c.核心形成截止))
      && (!c.首迁移形成截止 || c.首迁移形成截止 >= c.核心形成截止);
}

bool 目标投影完整(const 本能根任务目标投影_v1& p) noexcept {
  return 角色有效(p.根角色) && 有效(p.D.值) && 有效(p.L)
      && 有效(p.目标宿主E) && 有效(p.目标FT.编码)
      && 有效(p.目标状态合同) && p.H != 0;
}

} // namespace

bool 本能根任务核心结构登记结果_v1::成功(
    const 本能根任务核心结构登记请求_v1& r) const noexcept {
  return r.合同版本==1&&r.期望事实代次!=0&&有效(r.幂等身份)
      &&(状态==阶段::已发布||状态==阶段::精确重复)&&合同版本==r.合同版本
      &&事实代次>=r.期望事实代次&&交付&&结构字段完整(*交付);
}
bool 本能根任务初始化包结果_v1::成功(
    const 本能根任务初始化语义请求_v1& r) const noexcept {
  return 初始化请求有效(r)&&(状态==阶段::已发布||状态==阶段::精确重复)
      &&合同版本==r.合同版本&&事实代次!=0&&原请求==r&&包&&包->原请求==r
      &&包->预留形成事实代次<=事实代次&&初始化包有效(*包);
}
bool 本能根任务承接结果_v1::成功(
    const 不可变本能根任务初始化包_v1& p) const noexcept {
  if(!初始化包有效(p)||合同版本!=1||原包!=p||事实代次==0||!核心
      ||!核心投影完整(*核心))return false;
  if(状态==总状态::已承接到当前任务)
    return 核心阶段==阶段::已发布&&P1阶段==阶段::不适用&&首迁移阶段==阶段::不适用;
  if(状态!=总状态::已完成&&状态!=总状态::精确重复)return false;
  if(状态==总状态::精确重复&&核心阶段==阶段::精确重复
      &&P1阶段==阶段::不适用&&首迁移阶段==阶段::不适用)
    return true;
  return (核心阶段==阶段::已发布||核心阶段==阶段::精确重复)
      &&(P1阶段==阶段::已发布||P1阶段==阶段::精确重复)
      &&(首迁移阶段==阶段::已发布||首迁移阶段==阶段::精确重复)
      &&核心->P1.has_value()&&核心->Vt状态==本能根任务Vt状态_v1::待找方法;
}
bool 本能根任务核心读取结果_v1::成功(
    const 本能根任务身份读取请求_v1& r) const noexcept {
  return r.合同版本==1&&r.Gread!=0&&有效(r.T.值)&&状态==阶段::已读取
      &&合同版本==r.合同版本&&Gread==r.Gread&&核心&&核心->T==r.T
      &&核心投影完整(*核心);
}
bool 本能根任务核心读取结果_v1::成功(
    const 本能根任务锚点读取请求_v1& r) const noexcept {
  return r.合同版本==1&&r.Gread!=0&&有效(r.L)&&状态==阶段::已读取
      &&合同版本==r.合同版本&&Gread==r.Gread&&核心&&核心->L==r.L
      &&核心投影完整(*核心);
}
bool 本能根任务目标投影结果_v1::成功(
    const 本能根任务目标投影读取请求_v1& r) const noexcept {
  return r.合同版本==1&&r.Gread!=0&&r.H!=0&&r.H<=r.Gread&&有效(r.T.值)
      &&((r.读取种类==本能根任务读取种类_v1::当前&&r.H==r.Gread)
          ||r.读取种类==本能根任务读取种类_v1::历史)
      &&状态==阶段::已读取&&合同版本==r.合同版本&&Gread==r.Gread&&H==r.H
      &&读取种类==r.读取种类&&投影&&投影->H==r.H
      &&投影->H<=Gread&&目标投影完整(*投影);
}
bool 本能根任务当前资格退出结果_v1::成功(
    const 本能根任务当前资格退出请求_v1& r) const noexcept {
  return r.合同版本==1&&r.期望事实代次!=0&&有效(r.T.值)&&有效(r.L)
      &&r.期望前生命周期==本能根任务生命周期_v1::当前可治理
      &&退出目标有效(r.目标生命周期)&&有效(r.幂等身份)
      &&(状态==阶段::已发布||状态==阶段::精确重复)&&合同版本==r.合同版本
      &&事实代次>=r.期望事实代次&&T==r.T&&L==r.L&&目标生命周期==r.目标生命周期
      &&已退出L当前任务关系&&已退出旧生命周期值&&新生命周期值&&退出回执
      &&有效(*已退出L当前任务关系)&&有效(*已退出旧生命周期值)
      &&有效(*新生命周期值)&&有效(退出回执->值);
}

本能根任务核心结构登记结果_v1 本能根任务核心服务_v1::登记结构(
    const L1事实基座服务& l1,L1所有者范围写端口& port,
    const 本能根任务核心结构登记请求_v1& r) noexcept {
  本能根任务核心结构登记结果_v1 out;out.事实代次=r.期望事实代次;
  if(r.合同版本!=1||r.期望事实代次==0||r.幂等身份!=结构登记键
      ||!port.有效()||!port.绑定于(l1))return out;
  try {
    const auto g=当前G(l1);if(!g){out.状态=阶段::内部不一致;return out;}
    if(*g!=r.期望事实代次){out.状态=阶段::事实代次漂移;out.事实代次=*g;return out;}
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,*g,r.幂等身份};
    for(std::uint32_t i=1;i<=3;++i)ws.节点.push_back({Key{i},节点种类::普通,std::nullopt});
    for(std::uint32_t i=10;i<=18;++i)ws.节点.push_back({Key{i},节点种类::普通,std::nullopt});
    for(std::uint32_t i=20;i<=25;++i)ws.节点.push_back({Key{i},节点种类::属性类型,
        i <= 22 ? L1所有者范围值表示种类::I64 : L1所有者范围值表示种类::U64组});
    const auto first=port.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
    const L1所有者范围写入结果* saved=nullptr;L1所有者范围写入结果 fresh;
    if(first.状态==L1所有者范围读取状态::成功){
      if(first.读取事实代次!=*g||first.合同版本!=L1所有者范围首次写入读取合同版本
          ||first.所有者!=port.所有者身份()||first.写入幂等身份!=r.幂等身份
          ||!first.首次规范化写集||!first.首次写入结果
          ||!忽略G写集相同(*first.首次规范化写集,ws)){out.状态=阶段::幂等冲突;return out;}
      saved=&*first.首次写入结果;out.状态=阶段::精确重复;
    } else if(first.状态==L1所有者范围读取状态::未找到){
      fresh=port.提交所有者范围中性写集(ws);out.状态=映射写入状态(fresh.状态);saved=&fresh;
    } else {out.状态=映射读取状态(first.状态);out.事实代次=first.读取事实代次;return out;}
    if(!saved||!写入头完整(*saved,port.所有者身份(),r.幂等身份)
        || saved->新编码映射.size() != 18) {
      out.状态 = saved && saved->是否形成内存权威发布
          ? 阶段::已可能发布 : 阶段::内部不一致;
      return out;
    }
    std::array<稳定编码,26> id{};
    for(const auto& [k,v]:saved->新编码映射){if(k.值>=id.size()||有效(id[k.值])||!有效(v)){out.状态=阶段::内部不一致;return out;}id[k.值]=v;}
    本能根任务核心结构交付_v1 layout{1,port.所有者身份(),id[1],id[2],id[3],
      id[10],id[11],id[12],id[13],id[14],id[15],id[16],id[17],id[18],
      id[20],id[21],id[22],id[23],id[24],id[25]};
    if(!结构字段完整(layout)){out.状态=阶段::内部不一致;return out;}
    auto after=当前G(l1);if(!after){out.状态=阶段::内部不一致;return out;}
    out.事实代次=*after;

    L1所有者范围写集请求 hws{L1所有者范围CRUD合同版本,*after,高水位首次键};
    hws.值={{Key{1},layout.任务族根,layout.私有高水位属性类型,std::int64_t{0},layout.任务族根}};
    hws.属性槽变更={{layout.任务族根,layout.私有高水位属性类型,Key{1}}};
    const auto hf=port.读取首次写入材料({L1所有者范围首次写入读取合同版本,高水位首次键});
    if(hf.状态==L1所有者范围读取状态::成功){
      if(hf.读取事实代次!=*after||hf.合同版本!=L1所有者范围首次写入读取合同版本
          ||hf.所有者!=port.所有者身份()||!hf.首次规范化写集||!hf.首次写入结果
          ||!忽略G写集相同(*hf.首次规范化写集,hws)||!写入头完整(*hf.首次写入结果,port.所有者身份(),高水位首次键)){
        out.状态=阶段::内部不一致;out.交付.reset();return out;}
    } else if(hf.状态==L1所有者范围读取状态::未找到){
      const auto hw=port.提交所有者范围中性写集(hws);
      if(!写入头完整(hw,port.所有者身份(),高水位首次键)){
        out.状态 = hw.是否形成内存权威发布
            ? 阶段::已可能发布 : 映射写入状态(hw.状态);
        out.事实代次=hw.事实代次;return out;}
      out.状态=阶段::已发布;out.事实代次=hw.事实代次;
    } else {out.状态=映射读取状态(hf.状态);out.事实代次=hf.读取事实代次;return out;}
    out.交付=layout;return out;
  } catch(const std::bad_alloc&){out.状态=阶段::资源失败;}
    catch(const std::length_error&){out.状态=阶段::资源失败;}
    catch(...){out.状态=阶段::内部不一致;}
  out.交付.reset();return out;
}

本能根任务核心服务_v1::本能根任务核心服务_v1(
    const L1事实基座服务& l1,const 需求类数据服务& demand,
    const 存在类数据服务& existence,const 特征类数据服务& feature,
    L1所有者范围写端口&& port,const 本能根任务核心结构交付_v1& layout,
    存在单例角色身份 selfRole)
  :l1_(l1),需求_(demand),存在_(existence),特征_(feature),写端口_(std::move(port)),
   owner_(写端口_.所有者身份()),结构_(layout),self角色_(selfRole){
  if(!写端口_.有效()||!写端口_.绑定于(l1_)||!需求_.绑定于(l1_)
      ||!存在_.绑定于(l1_)||!特征_.绑定于(l1_)||!有效(owner_)
      ||owner_!=结构_.task_owner||!结构字段完整(结构_)||!有效(self角色_.值))
    throw std::invalid_argument("invalid instinct-root task owner configuration");
  const auto q=l1_.读取当前结构所有者({L1所有者范围CRUD合同版本,owner_});
  if(q.状态!=L1所有者范围读取状态::成功||q.合同版本!=L1所有者范围CRUD合同版本
      ||!q.所有者事实||q.所有者事实->所有者!=owner_
      ||q.所有者事实->范围种类!=L1所有者范围种类::独占结构范围
      ||q.所有者事实->退出事实代次)
    throw std::invalid_argument("invalid instinct-root task owner");
}
bool 本能根任务核心服务_v1::绑定于(const L1事实基座服务& l1) const noexcept{return &l1==&l1_;}

namespace {

std::vector<std::uint64_t> 编码预留材料(const 本能根任务初始化语义请求_v1& r,
    std::uint64_t n) {
  const auto control=控制键基址+n,base=阶段键基址+4*n;
  return {1,r.意图.值.值,r.期望事实代次,static_cast<std::uint64_t>(r.来源.根角色),
    r.来源.D.值.值,r.来源.L.值,r.来源.来源截止H,r.来源.根形成F.编码.值,
    r.前序任务 ? r.前序任务->值.值 : 0,n,control,base,base+1,base+2};
}

std::optional<不可变本能根任务初始化包_v1> 解析预留材料(
    const std::vector<std::uint64_t>& v, 稳定编码 record, std::uint64_t created) noexcept {
  if(v.size()!=14||v[0]!=1||v[1]==0||v[2]==0||(v[3]!=1&&v[3]!=2)
      ||v[4]==0||v[5]==0||v[6]==0||v[7]==0||v[9]==0||v[9]>最大预留序号
      ||v[10]!=控制键基址+v[9]||v[11]!=阶段键基址+4*v[9]
      ||v[12]!=v[11]+1||v[13]!=v[11]+2||!有效(record)||created==0)return std::nullopt;
  本能根任务初始化语义请求_v1 r{1,{{v[1]}},v[2],
    {static_cast<本能根角色>(v[3]),{{v[4]}},{v[5]},v[6],{{v[7]}}},std::nullopt};
  if(v[8])r.前序任务=本能根任务身份_v1{{v[8]}};
  不可变本能根任务初始化包_v1 p{r,{record},v[9],{v[10]},{v[11]},{v[12]},{v[13]},created};
  return 初始化包有效(p) ? std::optional{p} : std::nullopt;
}

struct 预留扫描结果 final {
  阶段 状态=阶段::未找到;
  std::uint64_t G=0;
  std::optional<不可变本能根任务初始化包_v1> 包;
};
预留扫描结果 扫描预留(const L1事实基座服务& l1,L1结构所有者身份 owner,
    const 本能根任务核心结构交付_v1& s,const 本能根任务初始化语义请求_v1& request,
    std::uint64_t g) noexcept {
  预留扫描结果 out;out.G=g;
  const auto rows=读取关系(l1,owner,L1所有者范围关系端点方向::源,
      s.预留记录族根,s.预留记录成员关系类型,g,g);
  if(rows.状态!=阶段::已读取){out.状态=rows.状态;out.G=rows.Gread;return out;}
  std::optional<不可变本能根任务初始化包_v1> found;
  bool sameIntentDifferent=false;
  for(const auto& edge:rows.关系){
    if(edge.源节点!=s.预留记录族根){out.状态=阶段::内部不一致;return out;}
    const auto material=读取属性(l1,owner,edge.目标节点,s.预留材料属性类型,g,g,
        L1所有者范围值表示种类::U64组);
    if(material.状态!=阶段::已读取||!material.U64){out.状态=material.状态;out.G=material.Gread;return out;}
    const auto parsed=解析预留材料(*material.U64,edge.目标节点,edge.创建事实代次);
    if(!parsed){out.状态=阶段::内部不一致;return out;}
    if(parsed->原请求.意图!=request.意图)continue;
    if(parsed->原请求!=request){sameIntentDifferent=true;continue;}
    if(found){out.状态=阶段::内部不一致;return out;}
    found=*parsed;
  }
  if(found&&sameIntentDifferent){out.状态=阶段::内部不一致;return out;}
  if(found){out.状态=阶段::精确重复;out.包=*found;return out;}
  if(sameIntentDifferent){out.状态=阶段::幂等冲突;return out;}
  out.状态=阶段::未找到;return out;
}

} // namespace

本能根任务初始化包结果_v1 本能根任务核心服务_v1::签发或恢复不可变初始化包(
    const 本能根任务初始化语义请求_v1& r) noexcept {
  本能根任务初始化包结果_v1 out;out.原请求=r;
  if(!初始化请求有效(r))return out;
  try {
    const auto g=当前G(l1_);if(!g){out.状态=阶段::内部不一致;return out;}out.事实代次=*g;
    const auto scan=扫描预留(l1_,owner_,结构_,r,*g);
    if(scan.状态==阶段::精确重复&&scan.包){out.状态=阶段::精确重复;out.包=scan.包;return out;}
    if(scan.状态!=阶段::未找到){out.状态=scan.状态;out.事实代次=scan.G;return out;}
    if(*g!=r.期望事实代次){out.状态=阶段::事实代次漂移;return out;}

    const auto high=读取属性(l1_,owner_,结构_.任务族根,结构_.私有高水位属性类型,*g,*g,
        L1所有者范围值表示种类::I64);
    if(high.状态!=阶段::已读取||!high.I64||!high.值身份||*high.I64<0){out.状态=high.状态;out.事实代次=high.Gread;return out;}
    const auto old=static_cast<std::uint64_t>(*high.I64);
    if(old>=最大预留序号){out.状态=阶段::号段耗尽;return out;}
    const auto n=old+1,control=控制键基址+n,base=阶段键基址+4*n;
    if(control<控制键基址||base<阶段键基址||base>std::numeric_limits<std::uint64_t>::max()-3
        ||(control&0xFFFF000000000000ULL)==0x4E43000000000000ULL
        ||(base&0xFFFF000000000000ULL)==0x4E43000000000000ULL){out.状态=阶段::号段耗尽;return out;}
    const L1所有者范围写入幂等身份 key{control};
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,*g,key};
    ws.节点={{Key{1},节点种类::普通,std::nullopt}};
    ws.值={{Key{2},Key{1},结构_.预留材料属性类型,编码预留材料(r,n),Key{1}},
      {Key{3},结构_.任务族根,结构_.私有高水位属性类型,static_cast<std::int64_t>(n),结构_.任务族根}};
    ws.关系={{Key{20},结构_.预留记录族根,Key{1},结构_.预留记录成员关系类型,0}};
    ws.属性槽变更={{Key{1},结构_.预留材料属性类型,Key{2}},
      {结构_.任务族根,结构_.私有高水位属性类型,Key{3}}};
    ws.退出事实={*high.值身份};
    const auto first=写端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本,key});
    const L1所有者范围写入结果* saved=nullptr;L1所有者范围写入结果 fresh;
    if(first.状态==L1所有者范围读取状态::成功){
      if(first.读取事实代次!=*g||first.合同版本!=L1所有者范围首次写入读取合同版本
          ||first.所有者!=owner_||!first.首次规范化写集||!first.首次写入结果
          ||!忽略G写集相同(*first.首次规范化写集,ws)){out.状态=阶段::幂等冲突;return out;}
      saved=&*first.首次写入结果;out.状态=阶段::精确重复;
    } else if(first.状态==L1所有者范围读取状态::未找到){
      fresh=写端口_.提交所有者范围中性写集(ws);saved=&fresh;out.状态=映射写入状态(fresh.状态);
    } else {out.状态=映射读取状态(first.状态);out.事实代次=first.读取事实代次;return out;}
    if(!saved||!写入头完整(*saved,owner_,key)){
      out.状态 = saved && saved->是否形成内存权威发布
          ? 阶段::已可能发布
          : 映射写入状态(saved ? saved->状态 : L1所有者范围写入状态::内部不一致);
      if(saved)out.事实代次=saved->事实代次;return out;
    }
    const auto record=映射编码(*saved,1),material=映射编码(*saved,2),newHigh=映射编码(*saved,3),edge=映射编码(*saved,20);
    if(saved->新编码映射.size()!=4||!record||!material||!newHigh||!edge){out.状态=阶段::内部不一致;return out;}
    out.事实代次=saved->事实代次;
    const auto check=扫描预留(l1_,owner_,结构_,r,out.事实代次);
    if(check.状态!=阶段::精确重复||!check.包||check.包->预留记录.值!=*record
        ||check.包->预留序号!=n||check.包->预留形成事实代次!=out.事实代次){
      out.状态 = saved->是否形成内存权威发布
          ? 阶段::已可能发布 : 阶段::内部不一致;
      out.包.reset();return out;}
    out.包=check.包;return out;
  } catch(const std::bad_alloc&){out.状态=阶段::资源失败;}
    catch(const std::length_error&){out.状态=阶段::资源失败;}
    catch(...){out.状态=阶段::内部不一致;}
  out.包.reset();return out;
}

namespace {

std::vector<std::uint64_t> 编码核心回执(
    const 不可变本能根任务初始化包_v1& p, std::uint64_t 分支,
    本能根任务身份_v1 既有T = {}) {
  return {1,1,p.预留记录.值.值,p.原请求.意图.值.值,分支,既有T.值.值};
}
std::vector<std::uint64_t> 编码P1回执(
    const 不可变本能根任务初始化包_v1& p,
    本能根任务身份_v1 T, 本能根任务轮次身份_v1 R1) {
  return {1,2,p.预留记录.值.值,T.值.值,R1.值.值};
}
std::vector<std::uint64_t> 编码首迁移回执(
    const 不可变本能根任务初始化包_v1& p,
    本能根任务身份_v1 T, 稳定编码 Vt, 稳定编码 旧状态值) {
  return {1,3,p.预留记录.值.值,T.值.值,Vt.值,旧状态值.值};
}
std::vector<std::uint64_t> 编码退出回执(
    const 本能根任务当前资格退出请求_v1& r,
    稳定编码 旧当前关系, 稳定编码 旧生命周期值) {
  return {2,r.T.值.值,r.L.值,static_cast<std::uint64_t>(r.期望前生命周期),
    static_cast<std::uint64_t>(r.目标生命周期),r.幂等身份.值,
    旧当前关系.值,旧生命周期值.值};
}

L1所有者范围写集请求 构造核心新建写集(
    const 本能根任务核心结构交付_v1& s,
    const 不可变本能根任务初始化包_v1& p,std::uint64_t G) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,G,p.任务核心建立幂等身份};
  ws.节点={{Key{1},节点种类::普通,std::nullopt},{Key{2},节点种类::普通,std::nullopt},
    {Key{3},节点种类::普通,std::nullopt},{Key{4},节点种类::普通,std::nullopt}};
  const auto base=p.任务核心建立幂等身份.值;
  ws.值={{Key{10},Key{1},s.根来源定位属性类型,
      std::vector<std::uint64_t>{1,static_cast<std::uint64_t>(p.原请求.来源.根角色),
        p.原请求.来源.D.值.值,p.原请求.来源.L.值,p.原请求.来源.来源截止H,
        p.原请求.来源.根形成F.编码.值,p.预留记录.值.值,base},Key{1}},
    {Key{11},Key{1},s.任务生命周期属性类型,
      static_cast<std::int64_t>(本能根任务生命周期_v1::当前可治理),Key{1}},
    {Key{12},Key{2},s.Vt状态属性类型,
      static_cast<std::int64_t>(本能根任务Vt状态_v1::已建立待首轮准备),Key{2}},
    {Key{13},Key{4},s.初始化回执材料属性类型,编码核心回执(p,1),Key{4}}};
  ws.关系={{Key{20},s.任务族根,Key{1},s.任务族成员关系类型,0},
    {Key{21},Key{1},p.原请求.来源.L,s.任务查询锚点关系类型,0},
    {Key{22},p.原请求.来源.L,Key{1},s.L当前任务关系类型,0},
    {Key{23},Key{1},p.原请求.来源.D.值,s.任务来源D关系类型,0},
    {Key{24},Key{1},Key{2},s.任务Vt关系类型,0},
    {Key{25},Key{1},Key{3},s.任务R1关系类型,0},
    {Key{26},s.初始化回执族根,Key{4},s.初始化回执成员关系类型,0}};
  ws.属性槽变更={{Key{1},s.根来源定位属性类型,Key{10}},
    {Key{1},s.任务生命周期属性类型,Key{11}},
    {Key{2},s.Vt状态属性类型,Key{12}},
    {Key{4},s.初始化回执材料属性类型,Key{13}}};
  return ws;
}

L1所有者范围写集请求 构造核心既有回执写集(
    const 本能根任务核心结构交付_v1& s,
    const 不可变本能根任务初始化包_v1& p,std::uint64_t G,
    本能根任务身份_v1 existing,std::uint64_t branch) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,G,p.任务核心建立幂等身份};
  ws.节点={{Key{1},节点种类::普通,std::nullopt}};
  ws.值={{Key{10},Key{1},s.初始化回执材料属性类型,
    编码核心回执(p,branch,existing),Key{1}}};
  ws.关系={{Key{20},s.初始化回执族根,Key{1},s.初始化回执成员关系类型,0}};
  ws.属性槽变更={{Key{1},s.初始化回执材料属性类型,Key{10}}};
  return ws;
}

L1所有者范围写集请求 构造P1写集(
    const 本能根任务核心结构交付_v1& s,
    const 不可变本能根任务初始化包_v1& p,std::uint64_t G,
    本能根任务身份_v1 T,本能根任务轮次身份_v1 R1) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,G,p.P1建立幂等身份};
  ws.节点={{Key{1},节点种类::普通,std::nullopt},{Key{2},节点种类::普通,std::nullopt}};
  ws.值={{Key{10},Key{2},s.初始化回执材料属性类型,编码P1回执(p,T,R1),Key{2}}};
  ws.关系={{Key{20},R1.值,Key{1},s.R1的P1关系类型,0},
    {Key{21},s.初始化回执族根,Key{2},s.初始化回执成员关系类型,0}};
  ws.属性槽变更={{Key{2},s.初始化回执材料属性类型,Key{10}}};
  return ws;
}

L1所有者范围写集请求 构造首迁移写集(
    const 本能根任务核心结构交付_v1& s,
    const 不可变本能根任务初始化包_v1& p,std::uint64_t G,
    本能根任务身份_v1 T,稳定编码 Vt,稳定编码 oldState) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,G,p.Vt首迁移幂等身份};
  ws.节点={{Key{1},节点种类::普通,std::nullopt}};
  ws.值={{Key{10},Vt,s.Vt状态属性类型,
      static_cast<std::int64_t>(本能根任务Vt状态_v1::待找方法),Vt},
    {Key{11},Key{1},s.初始化回执材料属性类型,
      编码首迁移回执(p,T,Vt,oldState),Key{1}}};
  ws.关系={{Key{20},s.初始化回执族根,Key{1},s.初始化回执成员关系类型,0}};
  ws.属性槽变更={{Vt,s.Vt状态属性类型,Key{10}},
    {Key{1},s.初始化回执材料属性类型,Key{11}}};
  ws.退出事实={oldState};
  return ws;
}

L1所有者范围写集请求 构造退出写集(
    const 本能根任务核心结构交付_v1& s,
    const 本能根任务当前资格退出请求_v1& r,
    稳定编码 oldCurrent,稳定编码 oldLife) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.期望事实代次,r.幂等身份};
  ws.节点={{Key{1},节点种类::普通,std::nullopt}};
  ws.值={{Key{10},r.T.值,s.任务生命周期属性类型,
      static_cast<std::int64_t>(r.目标生命周期),r.T.值},
    {Key{11},Key{1},s.初始化回执材料属性类型,
      编码退出回执(r,oldCurrent,oldLife),Key{1}}};
  ws.关系={{Key{20},s.初始化回执族根,Key{1},s.初始化回执成员关系类型,0}};
  ws.属性槽变更={{r.T.值,s.任务生命周期属性类型,Key{10}},
    {Key{1},s.初始化回执材料属性类型,Key{11}}};
  ws.退出事实={oldCurrent,oldLife};
  return ws;
}

const L1所有者范围值新建项* 查找本地值(
    const L1所有者范围写集请求& ws, std::uint32_t key) noexcept {
  const L1所有者范围值新建项* found=nullptr;
  for(const auto& v:ws.值)if(v.本地键.值==key){if(found)return nullptr;found=&v;}
  return found;
}
const std::vector<std::uint64_t>* 查找U64材料(
    const L1所有者范围写集请求& ws, std::uint32_t key) noexcept {
  const auto* v=查找本地值(ws,key);
  return v ? std::get_if<std::vector<std::uint64_t>>(&v->材料) : nullptr;
}

bool 映射键精确(const L1所有者范围写入结果&,
    std::initializer_list<std::uint32_t>) noexcept;

bool 核验回执闭包(const L1事实基座服务& l1,L1结构所有者身份 owner,
    const 本能根任务核心结构交付_v1& s,
    const L1所有者范围写入结果& written,std::uint64_t G,std::uint64_t H,
    std::uint32_t nodeKey,std::uint32_t valueKey,std::uint32_t edgeKey,
    const std::vector<std::uint64_t>& expected) noexcept {
  const auto nodeId=映射编码(written,nodeKey),valueId=映射编码(written,valueKey),
      edgeId=映射编码(written,edgeKey);
  if(!nodeId||!valueId||!edgeId)return false;
  const auto node=l1.读取所有者范围历史事实(
      {L1所有者范围CRUD合同版本,*nodeId});
  const auto* fact=node.事实
      ?std::get_if<L1所有者范围节点事实>(&*node.事实):nullptr;
  const auto material=读取属性(l1,owner,*nodeId,s.初始化回执材料属性类型,
      G,H,L1所有者范围值表示种类::U64组);
  const auto member=读取关系(l1,owner,L1所有者范围关系端点方向::目标,
      *nodeId,s.初始化回执成员关系类型,G,H);
  return node.状态==L1所有者范围读取状态::成功
      &&node.合同版本==L1所有者范围CRUD合同版本&&node.读取事实代次==G
      &&fact&&fact->编码==*nodeId&&fact->写入所有者==owner
      &&fact->种类==节点种类::普通&&!fact->属性类型表示
      &&fact->创建事实代次==written.事实代次
      &&fact->创建事实代次<=H&&(!fact->退出事实代次||*fact->退出事实代次>H)
      &&material.状态==阶段::已读取&&material.值身份==valueId
      &&material.创建G==written.事实代次&&material.U64
      &&*material.U64==expected&&member.状态==阶段::已读取
      &&member.关系.size()==1&&member.关系.front().编码==*edgeId
      &&member.关系.front().源节点==s.初始化回执族根
      &&member.关系.front().目标节点==*nodeId
      &&member.关系.front().创建事实代次==written.事实代次;
}


bool 核验退出闭包(const L1事实基座服务& l1,L1结构所有者身份 owner,
    const 本能根任务核心结构交付_v1& s,
    const 本能根任务当前资格退出请求_v1& request,
    const L1所有者范围写集请求& firstWrite,
    const L1所有者范围写入结果& written,std::uint64_t G,
    稳定编码 oldCurrent,稳定编码 oldLife) {
  const auto expected=构造退出写集(s,request,oldCurrent,oldLife);
  if(!忽略G写集相同(firstWrite,expected)
      ||!映射键精确(written,{1,10,11,20})
      ||!核验回执闭包(l1,owner,s,written,G,G,1,11,20,
          编码退出回执(request,oldCurrent,oldLife)))return false;
  const auto newLife=映射编码(written,10);
  const auto oldRelation=l1.读取所有者范围历史事实(
      {L1所有者范围CRUD合同版本,oldCurrent});
  const auto oldValue=l1.读取所有者范围历史事实(
      {L1所有者范围CRUD合同版本,oldLife});
  const auto* relation=oldRelation.事实
      ?std::get_if<L1所有者范围关系事实>(&*oldRelation.事实):nullptr;
  const auto* value=oldValue.事实
      ?std::get_if<L1所有者范围值事实>(&*oldValue.事实):nullptr;
  const auto* oldI64=value?std::get_if<std::int64_t>(&value->材料):nullptr;
  const auto current=读取关系(l1,owner,L1所有者范围关系端点方向::源,
      request.L,s.L当前任务关系类型,G,G);
  const auto lifecycle=读取属性(l1,owner,request.T.值,
      s.任务生命周期属性类型,G,G,L1所有者范围值表示种类::I64);
  return newLife&&oldRelation.状态==L1所有者范围读取状态::成功
      &&oldRelation.合同版本==L1所有者范围CRUD合同版本
      &&oldRelation.查询编码==oldCurrent&&oldRelation.读取事实代次==G
      &&relation&&relation->编码==oldCurrent
      &&relation->源节点==request.L&&relation->目标节点==request.T.值
      &&relation->关系类型节点==s.L当前任务关系类型
      &&relation->角色或顺序==0&&relation->写入所有者==owner
      &&relation->退出事实代次==written.事实代次
      &&oldValue.状态==L1所有者范围读取状态::成功
      &&oldValue.合同版本==L1所有者范围CRUD合同版本
      &&oldValue.查询编码==oldLife&&oldValue.读取事实代次==G
      &&value&&value->编码==oldLife
      &&value->所属节点==request.T.值
      &&value->属性类型节点==s.任务生命周期属性类型
      &&value->来源节点==request.T.值&&value->写入所有者==owner&&oldI64
      &&*oldI64==static_cast<std::int64_t>(request.期望前生命周期)
      &&value->退出事实代次==written.事实代次
      &&current.状态==阶段::已读取&&current.关系.empty()
      &&lifecycle.状态==阶段::已读取&&lifecycle.值身份==newLife
      &&lifecycle.创建G==written.事实代次&&lifecycle.I64
      &&*lifecycle.I64==static_cast<std::int64_t>(request.目标生命周期);
}

bool 核验核心首次闭包(const L1事实基座服务& l1,L1结构所有者身份 owner,
    const 本能根任务核心结构交付_v1& s,
    const 不可变本能根任务初始化包_v1& package,
    const L1所有者范围写入结果& written,std::uint64_t G) {
  if(!映射键精确(written,{1,2,3,4,10,11,12,13,20,21,22,23,24,25,26}))
    return false;
  const auto T=映射编码(written,1),Vt=映射编码(written,2),R1=映射编码(written,3);
  const auto originId=映射编码(written,10),lifeId=映射编码(written,11),
      vtStateId=映射编码(written,12);
  if(!T||!Vt||!R1||!originId||!lifeId||!vtStateId)return false;
  const auto read=[&](稳定编码 id)->std::optional<L1所有者范围事实副本>{
    const auto q=l1.读取所有者范围历史事实({L1所有者范围CRUD合同版本,id});
    if(q.状态!=L1所有者范围读取状态::成功
        ||q.合同版本!=L1所有者范围CRUD合同版本||q.查询编码!=id
        ||q.读取事实代次!=G||!q.事实)
      return std::nullopt;
    return q.事实;
  };
  const auto nodeVt=read(*Vt),nodeR1=read(*R1),origin=read(*originId),
      life=read(*lifeId),vtState=read(*vtStateId);
  const auto* nVt=nodeVt?std::get_if<L1所有者范围节点事实>(&*nodeVt):nullptr;
  const auto* nR1=nodeR1?std::get_if<L1所有者范围节点事实>(&*nodeR1):nullptr;
  const auto* originFact=origin?std::get_if<L1所有者范围值事实>(&*origin):nullptr;
  const auto* lifeFact=life?std::get_if<L1所有者范围值事实>(&*life):nullptr;
  const auto* stateFact=vtState?std::get_if<L1所有者范围值事实>(&*vtState):nullptr;
  const auto* originU64=originFact
      ?std::get_if<std::vector<std::uint64_t>>(&originFact->材料):nullptr;
  const auto* lifeI64=lifeFact?std::get_if<std::int64_t>(&lifeFact->材料):nullptr;
  const auto* stateI64=stateFact?std::get_if<std::int64_t>(&stateFact->材料):nullptr;
  const auto expectedWrite=构造核心新建写集(s,package,1);
  const auto* expectedOrigin=std::get_if<std::vector<std::uint64_t>>(
      &expectedWrite.值.front().材料);
  if(!nVt||!nR1||nVt->编码!=*Vt||nR1->编码!=*R1
      ||nVt->写入所有者!=owner||nR1->写入所有者!=owner
      ||nVt->种类!=节点种类::普通||nR1->种类!=节点种类::普通
      ||nVt->属性类型表示||nR1->属性类型表示
      ||nVt->创建事实代次!=written.事实代次
      ||nR1->创建事实代次!=written.事实代次
      ||!originFact||!lifeFact||!stateFact||!originU64||!lifeI64||!stateI64
      ||!expectedOrigin
      ||originFact->编码!=*originId||lifeFact->编码!=*lifeId
      ||stateFact->编码!=*vtStateId
      ||originFact->所属节点!=*T||originFact->属性类型节点!=s.根来源定位属性类型
      ||originFact->来源节点!=*T||originFact->写入所有者!=owner
      ||originFact->创建事实代次!=written.事实代次
      ||*originU64!=*expectedOrigin
      ||lifeFact->所属节点!=*T||lifeFact->属性类型节点!=s.任务生命周期属性类型
      ||lifeFact->来源节点!=*T||lifeFact->写入所有者!=owner
      ||lifeFact->创建事实代次!=written.事实代次
      ||*lifeI64!=static_cast<std::int64_t>(本能根任务生命周期_v1::当前可治理)
      ||stateFact->所属节点!=*Vt||stateFact->属性类型节点!=s.Vt状态属性类型
      ||stateFact->来源节点!=*Vt||stateFact->写入所有者!=owner
      ||stateFact->创建事实代次!=written.事实代次
      ||*stateI64!=static_cast<std::int64_t>(本能根任务Vt状态_v1::已建立待首轮准备))
    return false;
  const std::array<std::tuple<std::uint32_t,稳定编码,稳定编码,稳定编码>,6> expected{{
    {20,s.任务族根,*T,s.任务族成员关系类型},
    {21,*T,package.原请求.来源.L,s.任务查询锚点关系类型},
    {22,package.原请求.来源.L,*T,s.L当前任务关系类型},
    {23,*T,package.原请求.来源.D.值,s.任务来源D关系类型},
    {24,*T,*Vt,s.任务Vt关系类型},
    {25,*T,*R1,s.任务R1关系类型}}};
  for(const auto& [key,source,target,type]:expected){
    const auto id=映射编码(written,key);if(!id)return false;
    const auto fact=read(*id);
    const auto* relation=fact?std::get_if<L1所有者范围关系事实>(&*fact):nullptr;
    if(!relation||relation->编码!=*id||relation->源节点!=source
        ||relation->目标节点!=target||relation->关系类型节点!=type
        ||relation->角色或顺序!=0||relation->写入所有者!=owner
        ||relation->创建事实代次!=written.事实代次)return false;
  }
  return 核验回执闭包(l1,owner,s,written,G,G,4,13,26,
      编码核心回执(package,1));
}

struct 核心内部结果 final{阶段 状态=阶段::入口拒绝;std::uint64_t G=0;std::optional<本能根任务核心投影_v1> 核心;};
核心内部结果 读取核心内部(const L1事实基座服务& l1,
    L1所有者范围写端口& port,L1结构所有者身份 owner,
    const 本能根任务核心结构交付_v1& s,本能根任务身份_v1 T,
    std::uint64_t G,std::uint64_t H) noexcept{
  核心内部结果 out;out.G=G;
  if(!有效(T.值)||!G||!H||H>G)return out;
  try{
    const auto node=l1.读取所有者范围历史事实({L1所有者范围CRUD合同版本,T.值});
    if(node.读取事实代次!=G){out.状态=阶段::事实代次漂移;out.G=node.读取事实代次;return out;}
    if(node.状态!=L1所有者范围读取状态::成功){out.状态=映射读取状态(node.状态);return out;}
    const auto* tn = node.事实
        ? std::get_if<L1所有者范围节点事实>(&*node.事实) : nullptr;
    if(!tn||tn->编码!=T.值||tn->写入所有者!=owner||tn->种类!=节点种类::普通
        ||tn->属性类型表示){out.状态=阶段::内部不一致;return out;}
    if(tn->创建事实代次>H){out.状态=阶段::未找到;return out;}
    if(tn->退出事实代次&&*tn->退出事实代次<=H){out.状态=阶段::历史材料不可用;return out;}
    const auto one=[&](L1所有者范围关系端点方向 dir,稳定编码 endpoint,稳定编码 type)
        ->std::pair<阶段,std::optional<L1所有者范围关系事实>>{
      auto q=读取关系(l1,owner,dir,endpoint,type,G,H);
      if(q.状态!=阶段::已读取)return {q.状态,std::nullopt};
      if(q.关系.size()!=1)return {q.关系.empty() ? 阶段::未找到 : 阶段::内部不一致,std::nullopt};
      return {阶段::已读取,q.关系.front()};
    };
    const auto member=one(L1所有者范围关系端点方向::目标,T.值,s.任务族成员关系类型);
    const auto anchor=one(L1所有者范围关系端点方向::源,T.值,s.任务查询锚点关系类型);
    const auto source=one(L1所有者范围关系端点方向::源,T.值,s.任务来源D关系类型);
    const auto vt=one(L1所有者范围关系端点方向::源,T.值,s.任务Vt关系类型);
    const auto r1=one(L1所有者范围关系端点方向::源,T.值,s.任务R1关系类型);
    for(const auto* x:{&member,&anchor,&source,&vt,&r1})if(x->first!=阶段::已读取){out.状态=x->first;return out;}
    if(member.second->源节点!=s.任务族根||member.second->目标节点!=T.值
        ||anchor.second->源节点!=T.值||source.second->源节点!=T.值
        ||vt.second->源节点!=T.值||r1.second->源节点!=T.值){out.状态=阶段::内部不一致;return out;}
    const auto lifecycle=读取属性(l1,owner,T.值,s.任务生命周期属性类型,G,H,L1所有者范围值表示种类::I64);
    const auto origin=读取属性(l1,owner,T.值,s.根来源定位属性类型,G,H,L1所有者范围值表示种类::U64组);
    const auto vtstate=读取属性(l1,owner,vt.second->目标节点,s.Vt状态属性类型,G,H,L1所有者范围值表示种类::I64);
    if(lifecycle.状态!=阶段::已读取||origin.状态!=阶段::已读取||vtstate.状态!=阶段::已读取
        ||!lifecycle.I64||!origin.U64||!vtstate.I64){
      out.状态 = lifecycle.状态 != 阶段::已读取 ? lifecycle.状态
          : origin.状态 != 阶段::已读取 ? origin.状态 : vtstate.状态;
      return out;
    }
    if(*lifecycle.I64<1||*lifecycle.I64>5||*vtstate.I64<1||*vtstate.I64>2||origin.U64->size()!=8
        ||(*origin.U64)[0]!=1||((*origin.U64)[1]!=1&&(*origin.U64)[1]!=2)
        ||(*origin.U64)[2]!=source.second->目标节点.值||(*origin.U64)[3]!=anchor.second->目标节点.值
        ||(*origin.U64)[4]==0||(*origin.U64)[5]==0||(*origin.U64)[6]==0
        ||(*origin.U64)[7]<阶段键基址||(*origin.U64)[7]>std::numeric_limits<std::uint64_t>::max()-2){
      out.状态=阶段::内部不一致;return out;}
    const auto base=(*origin.U64)[7];
    const L1所有者范围写入幂等身份 coreKey{base},p1Key{base+1},migrationKey{base+2};
    const 稳定编码 reserveRecord{(*origin.U64)[6]};
    const auto reserveMember=one(L1所有者范围关系端点方向::目标,
        reserveRecord,s.预留记录成员关系类型);
    const auto reserveMaterial=读取属性(l1,owner,reserveRecord,
        s.预留材料属性类型,G,H,L1所有者范围值表示种类::U64组);
    if(reserveMember.first!=阶段::已读取||!reserveMember.second
        ||reserveMember.second->源节点!=s.预留记录族根
        ||reserveMember.second->目标节点!=reserveRecord
        ||reserveMaterial.状态!=阶段::已读取||!reserveMaterial.U64){
      out.状态=reserveMember.first!=阶段::已读取
          ? reserveMember.first:reserveMaterial.状态;
      return out;
    }
    const auto package=解析预留材料(*reserveMaterial.U64,reserveRecord,
        reserveMember.second->创建事实代次);
    if(!package||package->预留记录.值!=reserveRecord
        ||package->原请求.来源.根角色!=static_cast<本能根角色>((*origin.U64)[1])
        ||package->原请求.来源.D.值.值!=(*origin.U64)[2]
        ||package->原请求.来源.L.值!=(*origin.U64)[3]
        ||package->原请求.来源.来源截止H!=(*origin.U64)[4]
        ||package->原请求.来源.根形成F.编码.值!=(*origin.U64)[5]
        ||package->任务核心建立幂等身份!=coreKey){
      out.状态=阶段::内部不一致;return out;
    }
    const auto coreFirst=port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,coreKey});
    const auto coreHead=核验首次读取头(coreFirst,owner,coreKey,G);
    if(coreHead!=阶段::已读取){
      out.状态=coreFirst.状态==L1所有者范围读取状态::未找到
          ? 阶段::内部不一致:coreHead;return out;}
    const auto mappedT=映射编码(*coreFirst.首次写入结果,1);
    const auto* coreReceipt=查找U64材料(*coreFirst.首次规范化写集,13);
    const auto expectedCore=构造核心新建写集(s,*package,1);
    if(!忽略G写集相同(*coreFirst.首次规范化写集,expectedCore)
        ||!核验核心首次闭包(l1,owner,s,*package,*coreFirst.首次写入结果,G)
        ||!mappedT||*mappedT!=T.值||!coreReceipt
        ||*coreReceipt!=编码核心回执(*package,1)
        ||tn->创建事实代次!=coreFirst.首次写入结果->事实代次){
      out.状态=阶段::内部不一致;return out;
    }
    const auto p1rows=读取关系(l1,owner,L1所有者范围关系端点方向::源,
        r1.second->目标节点,s.R1的P1关系类型,G,H);
    if(p1rows.状态!=阶段::已读取||p1rows.关系.size()>1){
      out.状态 = p1rows.状态 == 阶段::已读取 ? 阶段::内部不一致 : p1rows.状态;
      return out;
    }
    std::optional<本能根任务准备身份_v1> P1;
    std::uint64_t Gp1=0,Gmigration=0;
    const auto p1First=port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,p1Key});
    if(p1First.状态==L1所有者范围读取状态::成功){
      if(核验首次读取头(p1First,owner,p1Key,G)!=阶段::已读取){
        out.状态=阶段::内部不一致;return out;}
      if(p1First.首次写入结果->事实代次<=H){
        if(p1rows.关系.size()!=1){out.状态=阶段::内部不一致;return out;}
        const auto mappedP1=映射编码(*p1First.首次写入结果,1);
        const auto mappedP1Relation=映射编码(*p1First.首次写入结果,20);
        const auto p1Node=mappedP1?l1.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本,*mappedP1}):L1所有者范围历史读取结果{};
        const auto* p1Fact=p1Node.事实
            ?std::get_if<L1所有者范围节点事实>(&*p1Node.事实):nullptr;
        const auto expectedReceipt=编码P1回执(*package,T,{r1.second->目标节点});
        const auto expectedWrite=构造P1写集(s,*package,1,T,{r1.second->目标节点});
        if(!映射键精确(*p1First.首次写入结果,{1,2,10,20,21})
            ||!忽略G写集相同(*p1First.首次规范化写集,expectedWrite)
            ||!mappedP1||!mappedP1Relation||*mappedP1!=p1rows.关系.front().目标节点
            ||p1Node.状态!=L1所有者范围读取状态::成功
            ||p1Node.合同版本!=L1所有者范围CRUD合同版本
            ||p1Node.查询编码!=*mappedP1||p1Node.读取事实代次!=G
            ||!p1Fact||p1Fact->编码!=*mappedP1||p1Fact->写入所有者!=owner
            ||p1Fact->种类!=节点种类::普通||p1Fact->属性类型表示
            ||p1Fact->创建事实代次!=p1First.首次写入结果->事实代次
            ||p1rows.关系.front().编码!=*mappedP1Relation
            ||p1rows.关系.front().创建事实代次!=p1First.首次写入结果->事实代次
            ||!核验回执闭包(l1,owner,s,*p1First.首次写入结果,G,H,
                2,10,21,expectedReceipt)){
          out.状态=阶段::内部不一致;return out;}
        P1=本能根任务准备身份_v1{*mappedP1};Gp1=p1First.首次写入结果->事实代次;
      }else if(!p1rows.关系.empty()){
        out.状态=阶段::内部不一致;return out;
      }
    }else if(p1First.状态==L1所有者范围读取状态::未找到){
      if(!p1rows.关系.empty()){out.状态=阶段::内部不一致;return out;}
    }else{out.状态=映射读取状态(p1First.状态);return out;}
    const auto migrationFirst=port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,migrationKey});
    if(migrationFirst.状态==L1所有者范围读取状态::成功){
      if(核验首次读取头(migrationFirst,owner,migrationKey,G)!=阶段::已读取){
        out.状态=阶段::内部不一致;return out;}
      if(migrationFirst.首次写入结果->事实代次<=H){
        const auto* receipt=查找U64材料(*migrationFirst.首次规范化写集,11);
        if(!receipt||receipt->size()!=6||(*receipt)[0]!=1||(*receipt)[1]!=3
            ||(*receipt)[2]!=package->预留记录.值.值||(*receipt)[3]!=T.值.值
            ||(*receipt)[4]!=vt.second->目标节点.值||(*receipt)[5]==0){
          out.状态=阶段::内部不一致;return out;}
        const 稳定编码 oldState{(*receipt)[5]};
        const auto expectedWrite=构造首迁移写集(s,*package,1,T,
            vt.second->目标节点,oldState);
        const auto old=l1.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本,oldState});
        const auto* oldFact=old.事实
            ?std::get_if<L1所有者范围值事实>(&*old.事实):nullptr;
        const auto* oldI64=oldFact?std::get_if<std::int64_t>(&oldFact->材料):nullptr;
        if(!映射键精确(*migrationFirst.首次写入结果,{1,10,11,20})
            ||!忽略G写集相同(*migrationFirst.首次规范化写集,expectedWrite)
            ||!核验回执闭包(l1,owner,s,*migrationFirst.首次写入结果,G,H,
                1,11,20,*receipt)
            ||*vtstate.I64!=static_cast<std::int64_t>(本能根任务Vt状态_v1::待找方法)
            ||vtstate.值身份!=映射编码(*migrationFirst.首次写入结果,10)
            ||vtstate.创建G!=migrationFirst.首次写入结果->事实代次
            ||old.状态!=L1所有者范围读取状态::成功||old.读取事实代次!=G
            ||!oldFact||oldFact->写入所有者!=owner
            ||oldFact->所属节点!=vt.second->目标节点
            ||oldFact->属性类型节点!=s.Vt状态属性类型
            ||oldFact->来源节点!=vt.second->目标节点||!oldI64
            ||*oldI64!=static_cast<std::int64_t>(本能根任务Vt状态_v1::已建立待首轮准备)
            ||oldFact->退出事实代次!=migrationFirst.首次写入结果->事实代次){
          out.状态=阶段::内部不一致;return out;}
        Gmigration=migrationFirst.首次写入结果->事实代次;
      }else if(*vtstate.I64!=static_cast<std::int64_t>(
                   本能根任务Vt状态_v1::已建立待首轮准备)){
        out.状态=阶段::内部不一致;return out;
      }
    }else if(migrationFirst.状态==L1所有者范围读取状态::未找到){
      if(*vtstate.I64!=static_cast<std::int64_t>(本能根任务Vt状态_v1::已建立待首轮准备)){
        out.状态=阶段::内部不一致;return out;}
    }else{out.状态=映射读取状态(migrationFirst.状态);return out;}
    本能根任务核心投影_v1 core{T,anchor.second->目标节点,{source.second->目标节点},
      source.second->编码,vt.second->目标节点,{r1.second->目标节点},P1,
      static_cast<本能根任务生命周期_v1>(*lifecycle.I64),
      static_cast<本能根任务Vt状态_v1>(*vtstate.I64),
      {static_cast<本能根角色>((*origin.U64)[1]),{{(*origin.U64)[2]}},{(*origin.U64)[3]},
        (*origin.U64)[4],{{(*origin.U64)[5]}}},coreKey,p1Key,migrationKey,
      coreFirst.首次写入结果->事实代次,Gp1,Gmigration};
    if(!来源有效(core.首次来源)||core.D!=core.首次来源.D||core.L!=core.首次来源.L
        ||!生命周期有效(core.生命周期)||!有效(core.Vt)||!有效(core.R1.值)
        ||!有效(core.T到D关系)||!有效(core.核心键)||!有效(core.P1键)||!有效(core.首迁移键)){
      out.状态=阶段::内部不一致;return out;}
    out.状态=阶段::已读取;out.核心=std::move(core);return out;
  }catch(const std::bad_alloc&){out.状态=阶段::资源失败;}
   catch(const std::length_error&){out.状态=阶段::资源失败;}
   catch(...){out.状态=阶段::内部不一致;}
  out.核心.reset();return out;
}

阶段 映射角色状态(存在单例角色状态 s)noexcept{
  switch(s){case 存在单例角色状态::入口拒绝:return 阶段::入口拒绝;
    case 存在单例角色状态::未绑定:case 存在单例角色状态::未找到:return 阶段::未找到;
    case 存在单例角色状态::目标已退出:case 存在单例角色状态::历史材料不可用:return 阶段::历史材料不可用;
    case 存在单例角色状态::事实代次漂移:return 阶段::事实代次漂移;
    case 存在单例角色状态::资源失败:return 阶段::资源失败;default:return 阶段::内部不一致;}
}
阶段 映射采用状态(存在当前采用完整读取状态_v2 s)noexcept{
  switch(s){case 存在当前采用完整读取状态_v2::入口拒绝:return 阶段::入口拒绝;
    case 存在当前采用完整读取状态_v2::未找到:return 阶段::未找到;
    case 存在当前采用完整读取状态_v2::目标已退出:case 存在当前采用完整读取状态_v2::历史材料不可用:return 阶段::历史材料不可用;
    case 存在当前采用完整读取状态_v2::事实代次漂移:return 阶段::事实代次漂移;
    case 存在当前采用完整读取状态_v2::资源失败:return 阶段::资源失败;default:return 阶段::内部不一致;}
}
阶段 映射特征错误(特征数据错误 s)noexcept{
  switch(s){case 特征数据错误::入口拒绝:return 阶段::入口拒绝;
    case 特征数据错误::未找到:case 特征数据错误::未设置:return 阶段::未找到;
    case 特征数据错误::已退出:case 特征数据错误::历史材料不可用:case 特征数据错误::旧格式不支持:return 阶段::历史材料不可用;
    case 特征数据错误::并发变化:return 阶段::事实代次漂移;
    case 特征数据错误::引用冲突:case 特征数据错误::类型不相容:return 阶段::引用冲突;
    case 特征数据错误::资源失败:return 阶段::资源失败;
    case 特征数据错误::幂等冲突:return 阶段::幂等冲突;default:return 阶段::内部不一致;}
}

bool 映射键精确(const L1所有者范围写入结果& w,
    std::initializer_list<std::uint32_t> keys) noexcept {
  if(w.新编码映射.size()!=keys.size())return false;
  for(const auto key:keys)if(!映射编码(w,key))return false;
  return true;
}

阶段 验证根来源(const L1事实基座服务& l1,const 需求类数据服务& demandService,
    const 存在类数据服务& existenceService,const 特征类数据服务& featureService,
    存在单例角色身份 selfRole,const 本能根任务目标来源定位_v1& source,
    std::uint64_t Gread) noexcept {
  try{
    const 本能根历史材料请求_v1 dr{1,Gread,source.来源截止H,source.根角色,source.根形成F};
    const auto demand=demandService.读取本能根历史材料(dr);
    if(!demand.成功(dr)||!demand.材料)return 映射根历史状态(demand.状态);
    if(demand.材料->根需求!=source.D.值||demand.材料->根列表项!=source.L
        ||demand.材料->实际特征!=source.根形成F.编码)return 阶段::目标不一致;
    const 存在单例角色读取请求 er{1,Gread,source.来源截止H,selfRole,1};
    const auto self=existenceService.读取单例角色(er);
    if(!self.成功(er)||!self.事实)return 映射角色状态(self.状态);
    const 准确特征读取请求 fr{1,Gread,demand.材料->创建事实代次,source.根形成F};
    const auto feature=featureService.读取准确特征事实(fr);
    if(const auto* e=std::get_if<特征数据错误>(&feature))return 映射特征错误(*e);
    const auto* ff=std::get_if<准确特征读取事实>(&feature);
    if(!ff||ff->Gread!=Gread||ff->H!=demand.材料->创建事实代次
        ||ff->信息.身份!=source.根形成F||!有效(ff->信息.类型)||!有效(ff->类型关系))
      return 阶段::内部不一致;
    const 存在当前采用完整读取请求_v2 ar{2,Gread,demand.材料->创建事实代次,
      self.事实->E,ff->信息.类型};
    const auto adopted=existenceService.读取当前采用完整_v2(ar);
    if(!adopted.成功(ar)||!adopted.采用)return 映射采用状态(adopted.状态);
    if(adopted.采用->F!=source.根形成F||adopted.采用->E!=self.事实->E
        ||adopted.采用->FT!=ff->信息.类型||!有效(adopted.采用->关系))
      return 阶段::内部不一致;
    return 阶段::已读取;
  }catch(const std::bad_alloc&){return 阶段::资源失败;}
   catch(const std::length_error&){return 阶段::资源失败;}
   catch(...){return 阶段::内部不一致;}
}

} // namespace

本能根任务核心读取结果_v1 本能根任务核心服务_v1::按任务读取核心(
    const 本能根任务身份读取请求_v1& r) const noexcept{
  本能根任务核心读取结果_v1 out;out.Gread=r.Gread;
  if(r.合同版本!=1||!r.Gread||!有效(r.T.值))return out;
  const auto q=读取核心内部(l1_,写端口_,owner_,结构_,r.T,r.Gread,r.Gread);
  out.状态=q.状态;out.Gread=q.G;out.核心=q.核心;return out;
}
本能根任务核心读取结果_v1 本能根任务核心服务_v1::按查询锚点读取当前任务(
    const 本能根任务锚点读取请求_v1& r) const noexcept{
  本能根任务核心读取结果_v1 out;out.Gread=r.Gread;
  if(r.合同版本!=1||!r.Gread||!有效(r.L))return out;
  const auto rows=读取关系(l1_,owner_,L1所有者范围关系端点方向::源,r.L,
      结构_.L当前任务关系类型,r.Gread,r.Gread);
  if(rows.状态!=阶段::已读取){out.状态=rows.状态;out.Gread=rows.Gread;return out;}
  if(rows.关系.empty()){out.状态=阶段::未找到;return out;}
  if(rows.关系.size()!=1){out.状态=阶段::内部不一致;return out;}
  const auto q=读取核心内部(l1_,写端口_,owner_,结构_,{rows.关系.front().目标节点},r.Gread,r.Gread);
  out.状态=q.状态;out.Gread=q.G;out.核心=q.核心;
  if(out.状态==阶段::已读取&&(!out.核心||out.核心->L!=r.L)){out.状态=阶段::内部不一致;out.核心.reset();}
  return out;
}
本能根任务目标投影结果_v1 本能根任务核心服务_v1::按任务与显式截止读取目标投影(
    const 本能根任务目标投影读取请求_v1& r) const noexcept{
  本能根任务目标投影结果_v1 out;out.Gread=r.Gread;out.H=r.H;out.读取种类=r.读取种类;
  if(r.合同版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.T.值)
      ||(r.读取种类==本能根任务读取种类_v1::当前&&r.H!=r.Gread)
      ||(r.读取种类!=本能根任务读取种类_v1::当前&&r.读取种类!=本能根任务读取种类_v1::历史))return out;
  try{
    const auto core=读取核心内部(l1_,写端口_,owner_,结构_,r.T,r.Gread,r.H);
    if(core.状态!=阶段::已读取||!core.核心){out.状态=core.状态;out.Gread=core.G;return out;}
    const 本能根历史材料请求_v1 dr{1,r.Gread,r.H,core.核心->首次来源.根角色,core.核心->首次来源.根形成F};
    const auto demand=需求_.读取本能根历史材料(dr);
    if(!demand.成功(dr)){out.状态=映射根历史状态(demand.状态);return out;}
    if(!demand.材料||demand.材料->根需求!=core.核心->D.值
        ||demand.材料->根列表项!=core.核心->L
        ||demand.材料->实际特征!=core.核心->首次来源.根形成F.编码){out.状态=阶段::内部不一致;return out;}
    const 存在单例角色读取请求 er{1,r.Gread,r.H,self角色_,1};
    const auto self=存在_.读取单例角色(er);
    if(!self.成功(er)||!self.事实){out.状态=映射角色状态(self.状态);return out;}
    const 准确特征读取请求 fr{1,r.Gread,demand.材料->创建事实代次,core.核心->首次来源.根形成F};
    const auto feature=特征_.读取准确特征事实(fr);
    if(const auto* e=std::get_if<特征数据错误>(&feature)){out.状态=映射特征错误(*e);return out;}
    const auto* ff=std::get_if<准确特征读取事实>(&feature);
    if(!ff||ff->Gread!=r.Gread||ff->H!=demand.材料->创建事实代次
        ||ff->信息.身份!=core.核心->首次来源.根形成F||!有效(ff->信息.类型)
        ||ff->创建G>demand.材料->创建事实代次
        ||(ff->退出G&&*ff->退出G<=demand.材料->创建事实代次)||!有效(ff->类型关系)){
      out.状态=阶段::内部不一致;return out;}
    const 存在当前采用完整读取请求_v2 ar{2,r.Gread,demand.材料->创建事实代次,
      self.事实->E,ff->信息.类型};
    const auto adopted=存在_.读取当前采用完整_v2(ar);
    if(!adopted.成功(ar)||!adopted.采用){out.状态=映射采用状态(adopted.状态);return out;}
    if(adopted.采用->E!=self.事实->E||adopted.采用->FT!=ff->信息.类型
        ||adopted.采用->F!=core.核心->首次来源.根形成F||!有效(adopted.采用->关系)){
      out.状态=阶段::内部不一致;return out;}
    out.投影=本能根任务目标投影_v1{core.核心->首次来源.根角色,core.核心->D,
      core.核心->L,self.事实->E,ff->信息.类型,demand.材料->根目标合同,
      demand.材料->目标I64值,r.H};out.状态=阶段::已读取;return out;
  }catch(const std::bad_alloc&){out.状态=阶段::资源失败;}
   catch(const std::length_error&){out.状态=阶段::资源失败;}
   catch(...){out.状态=阶段::内部不一致;}
  out.投影.reset();return out;
}

本能根任务承接结果_v1 本能根任务核心服务_v1::承接或建立任务(
    const 不可变本能根任务初始化包_v1& p) noexcept {
  本能根任务承接结果_v1 out; out.原包 = p;
  bool 提交调用中=false;
  const auto 失败=[&](总状态 status){out.状态=status;out.核心.reset();return out;};
  if(!初始化包有效(p)){out.状态=总状态::入口拒绝;return out;}
  try{
    auto g=当前G(l1_);if(!g){out.状态=总状态::内部不一致;return out;}out.事实代次=*g;
    const auto reservation=扫描预留(l1_,owner_,结构_,p.原请求,*g);
    if(reservation.状态!=阶段::精确重复||!reservation.包){
      out.状态=映射总状态(reservation.状态);out.事实代次=reservation.G;return out;}
    if(*reservation.包!=p){out.状态=总状态::幂等冲突;return out;}

    bool newBranch=false;
    auto coreFirst=写端口_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,p.任务核心建立幂等身份});
    if(coreFirst.状态==L1所有者范围读取状态::成功){
      const auto head=核验首次读取头(coreFirst,owner_,p.任务核心建立幂等身份,*g);
      if(head!=阶段::已读取){out.状态=映射总状态(head);return out;}
      const auto* receipt=查找U64材料(*coreFirst.首次规范化写集,
          coreFirst.首次规范化写集->节点.size()==4?13:10);
      if(!receipt||receipt->size()!=6||(*receipt)[0]!=1||(*receipt)[1]!=1){
        out.状态=总状态::内部不一致;return out;}
      if((*receipt)[2]!=p.预留记录.值.值||(*receipt)[3]!=p.原请求.意图.值.值){
        out.状态=总状态::幂等冲突;return out;}
      const auto branch=(*receipt)[4];
      if(branch==1&&(*receipt)[5]==0){
        const auto expected=构造核心新建写集(结构_,p,1);
        if(!忽略G写集相同(*coreFirst.首次规范化写集,expected)
            ||!核验核心首次闭包(
                l1_,owner_,结构_,p,*coreFirst.首次写入结果,*g)){
          out.状态=总状态::内部不一致;return out;}
        const auto tid=映射编码(*coreFirst.首次写入结果,1);
        if(!tid){out.状态=总状态::内部不一致;return out;}
        const auto core=读取核心内部(l1_,写端口_,owner_,结构_,{*tid},*g,*g);
        if(core.状态!=阶段::已读取||!core.核心){out.状态=映射总状态(core.状态);return out;}
        out.核心=core.核心;newBranch=true;
      }else if(branch==2&&(*receipt)[5]!=0){
        const 本能根任务身份_v1 existing{{(*receipt)[5]}};
        const auto expected=构造核心既有回执写集(结构_,p,1,existing,2);
        if(!映射键精确(*coreFirst.首次写入结果,{1,10,20})
            ||!忽略G写集相同(*coreFirst.首次规范化写集,expected)
            ||!核验回执闭包(l1_,owner_,结构_,*coreFirst.首次写入结果,*g,*g,
                1,10,20,编码核心回执(p,2,existing))){
          out.状态=总状态::内部不一致;return out;}
        const auto core=读取核心内部(l1_,写端口_,owner_,结构_,existing,*g,*g);
        if(core.状态!=阶段::已读取||!core.核心){out.状态=映射总状态(core.状态);return out;}
        out.核心=core.核心;out.核心阶段=阶段::精确重复;
        out.P1阶段=阶段::不适用;out.首迁移阶段=阶段::不适用;
        out.事实代次=coreFirst.首次写入结果->事实代次;
        out.状态=总状态::精确重复;return out;
      }else{out.状态=总状态::内部不一致;return out;}
      out.核心阶段=阶段::精确重复;
    }else if(coreFirst.状态==L1所有者范围读取状态::未找到){
      const auto sourceStatus=验证根来源(l1_,需求_,存在_,特征_,self角色_,p.原请求.来源,*g);
      if(sourceStatus!=阶段::已读取){out.状态=映射总状态(sourceStatus);return out;}
      const auto currentRows=读取关系(l1_,owner_,L1所有者范围关系端点方向::源,
          p.原请求.来源.L,结构_.L当前任务关系类型,*g,*g);
      if(currentRows.状态!=阶段::已读取){out.状态=映射总状态(currentRows.状态);return out;}
      L1所有者范围写集请求 ws;
      if(currentRows.关系.empty()){
        newBranch=true;
        ws=构造核心新建写集(结构_,p,*g);
      }else{
        if(currentRows.关系.size()!=1){out.状态=总状态::内部不一致;return out;}
        const 本能根任务身份_v1 existing{currentRows.关系.front().目标节点};
        const auto core=读取核心内部(l1_,写端口_,owner_,结构_,existing,*g,*g);
        if(core.状态!=阶段::已读取||!core.核心){out.状态=映射总状态(core.状态);return out;}
        if(core.核心->D!=p.原请求.来源.D||core.核心->L!=p.原请求.来源.L
            ||core.核心->首次来源.根角色!=p.原请求.来源.根角色
            ||core.核心->首次来源.根形成F!=p.原请求.来源.根形成F){
          out.状态=总状态::目标不一致;return out;}
        if(core.核心->生命周期!=本能根任务生命周期_v1::当前可治理){
          out.状态=总状态::当前任务不可复用;return out;}
        ws=构造核心既有回执写集(结构_,p,*g,existing,2);
      }
      提交调用中=true;
      auto written=写端口_.提交所有者范围中性写集(ws);
      提交调用中=false;
      out.事实代次=written.事实代次;
      if(!写入头完整(written,owner_,p.任务核心建立幂等身份)){
        out.状态 = 写入声称已发布(written)
            ? 总状态::已可能发布 : 映射总状态(映射写入状态(written.状态));
        return out;}
      out.核心阶段=映射写入状态(written.状态);
      const auto coreReadG=当前G(l1_);
      if(!coreReadG){out.状态=总状态::已可能发布;return out;}
      if(newBranch){
        if(!映射键精确(written,{1,2,3,4,10,11,12,13,20,21,22,23,24,25,26})){
          out.状态=总状态::已可能发布;return out;}
        const auto tid=映射编码(written,1);if(!tid){out.状态=总状态::已可能发布;return out;}
        const auto core=读取核心内部(l1_,写端口_,owner_,结构_,{*tid},*coreReadG,*coreReadG);
        if(core.状态!=阶段::已读取||!core.核心){out.状态=总状态::已可能发布;return out;}
        out.核心=core.核心;*g=*coreReadG;
      }else{
        if(!映射键精确(written,{1,10,20})){out.状态=总状态::已可能发布;return out;}
        const auto existing=currentRows.关系.front().目标节点;
        const auto core=读取核心内部(l1_,写端口_,owner_,结构_,{existing},*coreReadG,*coreReadG);
        if(core.状态!=阶段::已读取||!core.核心){out.状态=总状态::已可能发布;return out;}
        out.核心=core.核心;out.P1阶段=阶段::不适用;out.首迁移阶段=阶段::不适用;
        out.状态=out.核心阶段==阶段::精确重复
            ?总状态::精确重复:总状态::已承接到当前任务;
        return out;
      }
    }else{out.状态=映射总状态(映射读取状态(coreFirst.状态));out.事实代次=coreFirst.读取事实代次;return out;}

    if(!out.核心||!newBranch)return 失败(总状态::内部不一致);
    g=当前G(l1_);if(!g)return 失败(总状态::内部不一致);
    auto p1First=写端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本,p.P1建立幂等身份});
    if(p1First.状态==L1所有者范围读取状态::未找到){
      if(out.核心->P1)return 失败(总状态::内部不一致);
      auto ws=构造P1写集(结构_,p,*g,out.核心->T,out.核心->R1);
      提交调用中=true;
      auto w=写端口_.提交所有者范围中性写集(ws);
      提交调用中=false;
      out.事实代次=w.事实代次;
      if(!写入头完整(w,owner_,p.P1建立幂等身份)||!映射键精确(w,{1,2,10,20,21})){
        out.P1阶段=映射写入状态(w.状态);
        out.状态 = 写入声称已发布(w)
            ? 总状态::已可能发布 : 映射总状态(out.P1阶段);
        return 失败(out.状态);}
      out.P1阶段=映射写入状态(w.状态);*g=w.事实代次;
      const auto afterP1G=当前G(l1_);if(!afterP1G)return 失败(总状态::已可能发布);
      *g=*afterP1G;
    }else if(p1First.状态==L1所有者范围读取状态::成功){
      const auto head=核验首次读取头(p1First,owner_,p.P1建立幂等身份,*g);
      if(head!=阶段::已读取)return 失败(映射总状态(head));
      const auto* receipt=查找U64材料(*p1First.首次规范化写集,10);
      const auto expected=编码P1回执(p,out.核心->T,out.核心->R1);
      if(!receipt||receipt->size()!=5||(*receipt)[0]!=1||(*receipt)[1]!=2){
        return 失败(总状态::内部不一致);}
      if((*receipt)[2]!=p.预留记录.值.值||(*receipt)[3]!=out.核心->T.值.值
          ||(*receipt)[4]!=out.核心->R1.值.值){
        return 失败(总状态::幂等冲突);}
      const auto expectedWrite=构造P1写集(结构_,p,1,out.核心->T,out.核心->R1);
      if(*receipt!=expected
          ||!忽略G写集相同(*p1First.首次规范化写集,expectedWrite)
          ||!映射键精确(*p1First.首次写入结果,{1,2,10,20,21})
          ||!核验回执闭包(l1_,owner_,结构_,*p1First.首次写入结果,*g,*g,
              2,10,21,expected)){
        return 失败(总状态::内部不一致);}
      out.P1阶段=阶段::精确重复;
    }else{out.P1阶段=映射读取状态(p1First.状态);return 失败(映射总状态(out.P1阶段));}

    auto coreAfterP1=读取核心内部(l1_,写端口_,owner_,结构_,out.核心->T,*g,*g);
    if(coreAfterP1.状态!=阶段::已读取||!coreAfterP1.核心||!coreAfterP1.核心->P1){
      return 失败(总状态::已可能发布);}out.核心=coreAfterP1.核心;
    auto migrationFirst=写端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本,p.Vt首迁移幂等身份});
    if(migrationFirst.状态==L1所有者范围读取状态::未找到){
      const auto oldState=读取属性(l1_,owner_,out.核心->Vt,结构_.Vt状态属性类型,*g,*g,L1所有者范围值表示种类::I64);
      if(oldState.状态!=阶段::已读取||!oldState.I64||!oldState.值身份
          ||*oldState.I64!=static_cast<std::int64_t>(本能根任务Vt状态_v1::已建立待首轮准备)){
        return 失败(总状态::内部不一致);}
      auto ws=构造首迁移写集(结构_,p,*g,out.核心->T,
          out.核心->Vt,*oldState.值身份);
      提交调用中=true;
      auto w=写端口_.提交所有者范围中性写集(ws);
      提交调用中=false;
      out.事实代次=w.事实代次;
      if(!写入头完整(w,owner_,p.Vt首迁移幂等身份)||!映射键精确(w,{1,10,11,20})){
        out.首迁移阶段=映射写入状态(w.状态);
        out.状态 = 写入声称已发布(w)
            ? 总状态::已可能发布 : 映射总状态(out.首迁移阶段);
        return 失败(out.状态);}
      out.首迁移阶段=映射写入状态(w.状态);*g=w.事实代次;
      const auto afterMigrationG=当前G(l1_);
      if(!afterMigrationG)return 失败(总状态::已可能发布);
      *g=*afterMigrationG;
    }else if(migrationFirst.状态==L1所有者范围读取状态::成功){
      const auto head=核验首次读取头(
          migrationFirst,owner_,p.Vt首迁移幂等身份,*g);
      if(head!=阶段::已读取)return 失败(映射总状态(head));
      const auto* receipt=查找U64材料(*migrationFirst.首次规范化写集,11);
      if(!receipt||receipt->size()!=6||(*receipt)[0]!=1||(*receipt)[1]!=3
          ||(*receipt)[5]==0)return 失败(总状态::内部不一致);
      if((*receipt)[2]!=p.预留记录.值.值||(*receipt)[3]!=out.核心->T.值.值
          ||(*receipt)[4]!=out.核心->Vt.值)return 失败(总状态::幂等冲突);
      const 稳定编码 oldState{(*receipt)[5]};
      const auto expected=编码首迁移回执(p,out.核心->T,out.核心->Vt,oldState);
      const auto expectedWrite=构造首迁移写集(
          结构_,p,1,out.核心->T,out.核心->Vt,oldState);
      const auto old=l1_.读取所有者范围历史事实(
          {L1所有者范围CRUD合同版本,oldState});
      const auto* oldFact=old.事实
          ?std::get_if<L1所有者范围值事实>(&*old.事实):nullptr;
      const auto* oldI64=oldFact?std::get_if<std::int64_t>(&oldFact->材料):nullptr;
      if(*receipt!=expected
          ||!忽略G写集相同(*migrationFirst.首次规范化写集,expectedWrite)
          ||!映射键精确(*migrationFirst.首次写入结果,{1,10,11,20})
          ||!核验回执闭包(l1_,owner_,结构_,*migrationFirst.首次写入结果,*g,*g,
              1,11,20,expected)
          ||old.状态!=L1所有者范围读取状态::成功||old.读取事实代次!=*g
          ||!oldFact||oldFact->写入所有者!=owner_
          ||oldFact->所属节点!=out.核心->Vt
          ||oldFact->属性类型节点!=结构_.Vt状态属性类型
          ||oldFact->来源节点!=out.核心->Vt||!oldI64
          ||*oldI64!=static_cast<std::int64_t>(本能根任务Vt状态_v1::已建立待首轮准备)
          ||oldFact->退出事实代次!=migrationFirst.首次写入结果->事实代次){
        return 失败(总状态::内部不一致);}
      out.首迁移阶段=阶段::精确重复;
    }else{out.首迁移阶段=映射读取状态(migrationFirst.状态);return 失败(映射总状态(out.首迁移阶段));}
    const auto finalCore=读取核心内部(l1_,写端口_,owner_,结构_,out.核心->T,*g,*g);
    if(finalCore.状态!=阶段::已读取||!finalCore.核心
        ||finalCore.核心->Vt状态!=本能根任务Vt状态_v1::待找方法){
      return 失败(总状态::已可能发布);}
    out.核心=finalCore.核心;out.事实代次=*g;
    out.状态 = out.核心阶段==阶段::精确重复&&out.P1阶段==阶段::精确重复
        &&out.首迁移阶段==阶段::精确重复
        ? 总状态::精确重复 : 总状态::已完成;
    return out;
  }catch(const std::bad_alloc&){out.状态=提交调用中 ? 总状态::已可能发布 : 总状态::资源失败;}
   catch(const std::length_error&){out.状态=提交调用中 ? 总状态::已可能发布 : 总状态::资源失败;}
   catch(...){out.状态=提交调用中 ? 总状态::已可能发布 : 总状态::内部不一致;}
  out.核心.reset();return out;
}

本能根任务承接结果_v1 本能根任务核心服务_v1::恢复任务初始化(
    const 不可变本能根任务初始化包_v1& p) noexcept {
  return 承接或建立任务(p);
}

本能根任务当前资格退出结果_v1 本能根任务核心服务_v1::退出任务当前资格(
    const 本能根任务当前资格退出请求_v1& r) noexcept {
  本能根任务当前资格退出结果_v1 out;
  bool 提交调用中=false;
  const auto 失败=[&](阶段 status){
    out.状态=status;out.已退出L当前任务关系.reset();
    out.已退出旧生命周期值.reset();out.新生命周期值.reset();
    out.退出回执.reset();return out;
  };
  out.T = r.T; out.L = r.L; out.目标生命周期 = r.目标生命周期;
  if(r.合同版本!=1||!r.期望事实代次||!有效(r.T.值)||!有效(r.L)
      ||r.期望前生命周期!=本能根任务生命周期_v1::当前可治理
      ||!退出目标有效(r.目标生命周期)||!有效(r.幂等身份))return out;
  try{
    const auto g=当前G(l1_);if(!g){out.状态=阶段::内部不一致;return out;}
    out.事实代次=*g;
    const auto first=写端口_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,r.幂等身份});
    if(first.状态==L1所有者范围读取状态::成功){
      const auto head=核验首次读取头(first,owner_,r.幂等身份,*g);
      if(head!=阶段::已读取){out.状态=head;return out;}
      const auto* receipt=查找U64材料(*first.首次规范化写集,11);
      if(!receipt||receipt->size()!=8||(*receipt)[0]!=2
          ||(*receipt)[6]==0||(*receipt)[7]==0){
        out.状态=阶段::内部不一致;return out;}
      if((*receipt)[1]!=r.T.值.值||(*receipt)[2]!=r.L.值
          ||(*receipt)[3]!=static_cast<std::uint64_t>(r.期望前生命周期)
          ||(*receipt)[4]!=static_cast<std::uint64_t>(r.目标生命周期)
          ||(*receipt)[5]!=r.幂等身份.值){out.状态=阶段::幂等冲突;return out;}
      const 稳定编码 oldCurrent{(*receipt)[6]},oldLife{(*receipt)[7]};
      if(!核验退出闭包(l1_,owner_,结构_,r,*first.首次规范化写集,
          *first.首次写入结果,*g,oldCurrent,oldLife)){
        out.状态=阶段::内部不一致;return out;}
      out.状态=阶段::精确重复;out.事实代次=first.首次写入结果->事实代次;
      out.已退出L当前任务关系=oldCurrent;
      out.已退出旧生命周期值=oldLife;
      out.新生命周期值=映射编码(*first.首次写入结果,10);
      const auto rid=映射编码(*first.首次写入结果,1);
      if(rid)out.退出回执=本能根任务退出回执身份_v1{*rid};
      if(!out.成功(r))return 失败(阶段::内部不一致);return out;
    }
    if(first.状态!=L1所有者范围读取状态::未找到){out.状态=映射读取状态(first.状态);out.事实代次=first.读取事实代次;return out;}
    if(*g!=r.期望事实代次){out.状态=阶段::事实代次漂移;return out;}
    const auto core=读取核心内部(l1_,写端口_,owner_,结构_,r.T,*g,*g);
    if(core.状态!=阶段::已读取||!core.核心){out.状态=core.状态;return out;}
    if(core.核心->L!=r.L||core.核心->生命周期!=r.期望前生命周期){out.状态=阶段::当前任务不可复用;return out;}
    const auto current=读取关系(l1_,owner_,L1所有者范围关系端点方向::源,
        r.L,结构_.L当前任务关系类型,*g,*g);
    const auto oldLife=读取属性(l1_,owner_,r.T.值,结构_.任务生命周期属性类型,*g,*g,L1所有者范围值表示种类::I64);
    if(current.状态!=阶段::已读取||current.关系.size()!=1||current.关系.front().目标节点!=r.T.值
        ||oldLife.状态!=阶段::已读取||!oldLife.值身份||!oldLife.I64
        ||*oldLife.I64!=static_cast<std::int64_t>(r.期望前生命周期)){
      out.状态=阶段::内部不一致;return out;}
    auto ws=构造退出写集(结构_,r,current.关系.front().编码,*oldLife.值身份);
    提交调用中=true;
    const auto w=写端口_.提交所有者范围中性写集(ws);
    out.事实代次=w.事实代次;
    if(!写入头完整(w,owner_,r.幂等身份)||!映射键精确(w,{1,10,11,20})){
      out.状态 = 写入声称已发布(w)
          ? 阶段::已可能发布 : 映射写入状态(w.状态);
      return 失败(out.状态);}
    const auto exitReadG=当前G(l1_);
    if(!exitReadG)return 失败(阶段::已可能发布);
    if(!核验退出闭包(l1_,owner_,结构_,r,ws,w,*exitReadG,
        current.关系.front().编码,*oldLife.值身份)){
      out.状态=阶段::已可能发布;out.事实代次=w.事实代次;return out;}
    提交调用中=false;
    out.状态=映射写入状态(w.状态);out.事实代次=w.事实代次;
    out.已退出L当前任务关系=current.关系.front().编码;
    out.已退出旧生命周期值=*oldLife.值身份;out.新生命周期值=映射编码(w,10);
    const auto rid=映射编码(w,1);if(rid)out.退出回执=本能根任务退出回执身份_v1{*rid};
    if(!out.成功(r))return 失败(阶段::已可能发布);return out;
  }catch(const std::bad_alloc&){return 失败(提交调用中 ? 阶段::已可能发布 : 阶段::资源失败);}
   catch(const std::length_error&){return 失败(提交调用中 ? 阶段::已可能发布 : 阶段::资源失败);}
   catch(...){return 失败(提交调用中 ? 阶段::已可能发布 : 阶段::内部不一致);}
}

} // namespace 海中鱼巣
