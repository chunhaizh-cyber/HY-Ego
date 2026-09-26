#include "应用服务.自我形成.h"
#include <array>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

namespace 海中鱼巣::自我形成内部 {

bool 预算有效(const 世界树绑定创建预算 &w,
              const 概念树预算 &c) noexcept {
  return w.最大场景数量 > 0 && w.最大场景数量 <= 4096 &&
         w.最大关系数量 > 0 && w.最大关系数量 <= 4096 &&
         w.最大祖先数量 > 0 && w.最大祖先数量 <= 4096 &&
         c.最大概念数 > 0 && c.最大关系数 > 0 &&
         c.最大特征属性数 > 0 && c.最大世界成员数 > 0;
}

bool 请求有效(const 真实自我形成请求 &r) noexcept {
  if (r.版本 != 1 || !有效(r.期望世界根) || !有效(r.角色.值) ||
      !预算有效(r.预算, r.概念预算))
    return false;
  const std::array keys{r.概念定义键, r.世界键.组合, r.世界键.存在,
                        r.世界键.场景, r.世界键.概念引用};
  for (std::size_t i = 0; i < keys.size(); ++i) {
    if (!有效(keys[i]))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (keys[i] == keys[j])
        return false;
  }
  return true;
}

bool 请求有效(const 真实自我读取请求 &r) noexcept {
  return r.版本 == 1 && r.Gread && 有效(r.期望世界根) &&
         有效(r.角色.值) && 预算有效(r.预算, r.概念预算);
}

bool 生命周期有效(const 概念树生命周期 &v, std::uint64_t h) noexcept {
  return v.创建事实代次 && v.创建事实代次 <= h;
}

bool 生命周期有效(const 场景事实生命周期 &v, std::uint64_t h) noexcept {
  return v.创建事实代次 && v.创建事实代次 <= h;
}

bool 同一父边(const 直接归属联合事实 &a,
               const 直接归属联合事实 &b) noexcept {
  return a.Gread == b.Gread && a.来源 == b.来源 &&
         a.关系 == b.关系 && a.父 == b.父 && a.成员 == b.成员 &&
         a.创建事实代次 == b.创建事实代次;
}

bool 同一存在身份见证(const 存在身份来源当前见证 &a,
                       const 存在身份来源当前见证 &b) noexcept {
  return a.身份 == b.身份 && a.族锚点 == b.族锚点 &&
         a.族归属关系类型 == b.族归属关系类型 &&
         a.族归属关系 == b.族归属关系 && a.角色 == b.角色 &&
         a.节点创建事实代次 == b.节点创建事实代次 &&
         a.族锚点创建事实代次 == b.族锚点创建事实代次 &&
         a.关系类型创建事实代次 == b.关系类型创建事实代次 &&
         a.归属关系创建事实代次 == b.归属关系创建事实代次;
}

bool 父语境完整(const 场景父语境投影事实 &v,
                const 直接归属联合事实 &p,
                std::uint64_t g) noexcept {
  return v.Gread == g && v.场景 == p.成员 &&
         v.父场景语境 == p.父 && 同一父边(v.结构父, p) &&
         有效(v.投影边.编码) && v.投影边.源 == p.成员 &&
         v.投影边.目标 == p.父 && 有效(v.投影边.关系类型) &&
         v.投影边.角色或顺序 == 1 &&
         生命周期有效(v.投影边.生命周期, g) &&
         v.投影边.生命周期.创建事实代次 == p.创建事实代次;
}

bool 场景角色完整(const 场景角色当前事实 &v,
                  std::uint64_t g) noexcept {
  if (v.Gread != g || !有效(v.场景) ||
      !存在身份来源当前见证完整(v.对象存在来源, g, v.场景))
    return false;

  const 场景节点见证 *meta[]{&v.场景族锚点, &v.场景族归属类型,
                             &v.根绑定类型};
  const 稳定编码 m[]{meta[0]->编码, meta[1]->编码, meta[2]->编码};
  for (std::size_t i = 0; i < 3; ++i) {
    if (!有效(m[i]) || !生命周期有效(meta[i]->生命周期, g))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (m[i] == m[j])
        return false;
  }

  const auto &registration = v.场景角色登记边;
  if (!有效(registration.编码) || registration.源 != v.场景 ||
      registration.目标 != m[0] || registration.关系类型 != m[1] ||
      registration.角色或顺序 != 1 ||
      !生命周期有效(registration.生命周期, g))
    return false;
  const auto birth = registration.生命周期.创建事实代次;
  if (v.对象存在来源.节点创建事实代次 > birth)
    return false;

  std::array<稳定编码, 8> own{};
  for (std::size_t i = 0; i < v.四根.size(); ++i) {
    const auto &root = v.四根[i];
    if (static_cast<unsigned>(root.角色) != i + 1 ||
        !有效(root.根.编码) || !有效(root.绑定.编码) ||
        root.绑定.源 != v.场景 || root.绑定.目标 != root.根.编码 ||
        root.绑定.关系类型 != m[2] || root.绑定.角色或顺序 != i + 1 ||
        root.根.生命周期.创建事实代次 != birth ||
        root.绑定.生命周期.创建事实代次 != birth ||
        !生命周期有效(root.根.生命周期, g) ||
        !生命周期有效(root.绑定.生命周期, g))
      return false;
    own[i * 2] = root.根.编码;
    own[i * 2 + 1] = root.绑定.编码;
  }

  for (std::size_t i = 0; i < own.size(); ++i) {
    if (own[i] == v.场景 || own[i] == m[0] || own[i] == m[1] ||
        own[i] == m[2] || own[i] == registration.编码)
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (own[i] == own[j])
        return false;
  }
  return registration.编码 != v.场景 && registration.编码 != m[0] &&
         registration.编码 != m[1] && registration.编码 != m[2];
}

bool 首次场景完整(const 场景树节点当前事实 &v,
                   const 直接归属联合事实 &parent,
                   std::uint64_t g) noexcept {
  const auto &role = v.场景角色;
  const auto &proof = v.树证明;
  const auto birth = role.对象存在来源.节点创建事实代次;
  const 场景角色当前读取请求 request{2, g, parent.成员};
  const 场景角色当前读取结果 result{
      2, 场景角色数据状态::已读取, g, role};
  if (!result.成功(request) || !场景角色完整(role, g) ||
      role.Gread != g ||
      role.场景 != parent.成员 || birth != parent.创建事实代次 ||
      role.场景角色登记边.生命周期.创建事实代次 != birth ||
      !v.直接父 || !同一父边(*v.直接父, parent) ||
      v.从上游场景到本场景路径.size() != 1 ||
      !同一父边(v.从上游场景到本场景路径.front(), parent) ||
      !v.父语境投影 || !父语境完整(*v.父语境投影, parent, g) ||
      proof.Gread != g ||
      proof.种类 != 场景树证明种类::树归属 ||
      proof.场景 != parent.成员 || proof.树根 != parent.父 ||
      !有效(proof.关系) || proof.关系 != proof.见证.编码 ||
      proof.见证.源 != parent.成员 || proof.见证.目标 != parent.父 ||
      !有效(proof.见证.关系类型) || proof.见证.角色或顺序 != 1 ||
      !生命周期有效(proof.见证.生命周期, g) ||
      proof.见证.生命周期.创建事实代次 != birth)
    return false;
  for (const auto &root : role.四根)
    if (root.根.生命周期.创建事实代次 != birth ||
        root.绑定.生命周期.创建事实代次 != birth)
      return false;
  for (const auto &member : v.直接存在成员组)
    if (!场景直接包含事实完整(member, g) ||
        member.种类 != 场景直接包含种类::存在成员 ||
        member.父场景 != parent.成员)
      return false;
  for (const auto &child : v.直接子场景组)
    if (!场景直接包含事实完整(child, g) ||
        child.种类 != 场景直接包含种类::子场景 ||
        child.父场景 != parent.成员)
      return false;
  return true;
}

bool 通用概念完整(const 纯概念事实 &c, std::uint64_t g) noexcept {
  const auto *definition = std::get_if<通用存在概念定义>(&c.定义);
  if (c.类别 != 相关概念类别::存在 || !definition ||
      definition->规则版本 != 1 ||
      definition->规则 != 通用存在定义规则::不预设特征 ||
      !有效(c.概念.值) || !有效(c.定义记录) ||
      c.概念.值 == c.定义记录 || !c.直接上位.empty() ||
      !生命周期有效(c.生命周期, g) ||
      !生命周期有效(c.定义记录生命周期, g) ||
      c.定义记录生命周期.创建事实代次 != c.生命周期.创建事实代次 ||
      (c.治理状态 != 概念树生命周期状态::活跃 &&
       c.治理状态 != 概念树生命周期状态::冷却) ||
      !有效(c.生命周期值事实) ||
      !生命周期有效(c.生命周期值生命周期, g) ||
      c.定义关系组.size() != 1)
    return false;
  const auto &edge = c.定义关系组.front();
  return 有效(edge.关系) && 有效(edge.关系类型) &&
         edge.种类 == 纯概念定义关系种类::定义成员 && edge.顺序 == 1 &&
         edge.源 == c.概念.值 && edge.目标 == c.定义记录 &&
         生命周期有效(edge.生命周期, g) &&
         edge.生命周期.创建事实代次 == c.生命周期.创建事实代次;
}

bool 首次投影完整(const 真实自我投影 &v, 稳定编码 root,
                   存在单例角色身份 role,
                   const 世界树绑定创建预算 &budget) noexcept {
  const auto g = v.Gread;
  if (!g || !有效(v.E) || v.E == root ||
      v.世界根 != root || v.角色.角色 != role || v.角色.E != v.E ||
      v.位置.节点 != v.E || v.位置.世界根 != root ||
      v.位置.视角 != 世界树节点视角::场景 ||
      !v.位置.直接结构父 || v.位置.上行路径.size() != 1 ||
      !v.位置.父场景语境)
    return false;
  const auto &parent = *v.位置.直接结构父;
  if (!联合父载荷完整(parent, g, root, v.E) ||
      parent.来源 != 直接归属来源::直接子场景 ||
      !同一父边(parent, v.位置.上行路径.front()) ||
      !父语境完整(*v.位置.父场景语境, parent, g) ||
      !首次场景完整(v.场景, parent, g) ||
      v.位置.父场景语境->投影边.编码 !=
          v.场景.父语境投影->投影边.编码)
    return false;
  const 存在单例角色读取请求 roleRequest{1, g, role,
                                          budget.最大关系数量};
  const 存在单例角色读取结果 roleResult{
      1, 存在单例角色状态::已读取, g, v.角色};
  const auto birth = parent.创建事实代次;
  return roleResult.成功(roleRequest) &&
         存在身份来源当前见证完整(v.角色.存在身份, g, v.E) &&
         v.角色.存在身份.节点创建事实代次 == birth &&
         v.角色.目标创建事实代次 == birth &&
         通用概念完整(v.概念, g) && 有效(v.使用.关系) &&
         v.使用.E == v.E && v.使用.EC == v.概念.概念 &&
         生命周期有效(v.使用.生命周期, g) &&
         v.使用.生命周期.创建事实代次 == birth &&
          v.概念.生命周期.创建事实代次 < birth;
}

bool 当前上行路径完整(const 世界树层级位置 &v, 稳定编码 root,
                       std::uint64_t g,
                       const 世界树绑定创建预算 &budget) noexcept {
  if (!v.直接结构父 || v.上行路径.empty() ||
      v.上行路径.size() > budget.最大关系数量 ||
      v.上行路径.size() > budget.最大祖先数量 ||
      !同一父边(*v.直接结构父, v.上行路径.front()))
    return false;

  auto cursor = v.节点;
  for (std::size_t i = 0; i < v.上行路径.size(); ++i) {
    const auto &edge = v.上行路径[i];
    if ((edge.来源 != 直接归属来源::存在组成 &&
         edge.来源 != 直接归属来源::场景成员 &&
         edge.来源 != 直接归属来源::直接子场景) ||
        !联合父载荷完整(edge, g, edge.父, cursor) ||
        edge.父 == v.节点)
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (v.上行路径[j].父 == edge.父)
        return false;
    cursor = edge.父;
  }
  return cursor == root;
}

bool 当前场景完整(const 场景树节点当前事实 &v,
                   const 世界树层级位置 &position,
                   稳定编码 root, std::uint64_t g,
                   const 世界树绑定创建预算 &budget,
                   const 存在身份来源当前见证 &identity) noexcept {
  if (!position.直接结构父 || !position.父场景语境 ||
      !当前上行路径完整(position, root, g, budget))
    return false;
  const auto &parent = *position.直接结构父;
  const auto &role = v.场景角色;
  const auto &proof = v.树证明;
  const auto birth = identity.节点创建事实代次;
  const 场景角色当前读取请求 request{2, g, position.节点};
  const 场景角色当前读取结果 result{
      2, 场景角色数据状态::已读取, g, role};
  if (!result.成功(request) || !场景角色完整(role, g) ||
      role.Gread != g || role.场景 != position.节点 ||
      !同一存在身份见证(role.对象存在来源, identity) ||
      role.场景角色登记边.生命周期.创建事实代次 != birth ||
      !v.直接父 || !同一父边(*v.直接父, parent) ||
      v.从上游场景到本场景路径.empty() ||
      v.从上游场景到本场景路径.size() > position.上行路径.size() ||
      v.从上游场景到本场景路径.size() > budget.最大关系数量 ||
      v.从上游场景到本场景路径.size() > budget.最大祖先数量 ||
      !v.父语境投影 ||
      !父语境完整(*v.父语境投影, parent, g) ||
      !父语境完整(*position.父场景语境, parent, g) ||
      v.父语境投影->投影边.编码 !=
          position.父场景语境->投影边.编码 ||
      proof.Gread != g ||
      proof.种类 != 场景树证明种类::树归属 ||
      proof.场景 != position.节点 || proof.树根 != root ||
      !有效(proof.关系) || proof.关系 != proof.见证.编码 ||
      proof.见证.源 != position.节点 || proof.见证.目标 != root ||
      !有效(proof.见证.关系类型) || proof.见证.角色或顺序 != 1 ||
      !生命周期有效(proof.见证.生命周期, g) ||
      proof.见证.生命周期.创建事实代次 != birth)
    return false;

  const auto &scenePath = v.从上游场景到本场景路径;
  for (std::size_t i = 0; i < scenePath.size(); ++i) {
    const auto &edge = scenePath[i];
    if ((edge.来源 != 直接归属来源::存在组成 &&
         edge.来源 != 直接归属来源::场景成员 &&
         edge.来源 != 直接归属来源::直接子场景) ||
        !联合父载荷完整(edge, g, edge.父, edge.成员) ||
        (i && scenePath[i - 1].成员 != edge.父) ||
        edge.成员 == scenePath.front().父)
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (scenePath[j].成员 == edge.成员)
        return false;
  }
  if (scenePath.back().成员 != position.节点 ||
      !同一父边(scenePath.back(), parent))
    return false;
  for (std::size_t i = 0; i < scenePath.size(); ++i)
    if (!同一父边(scenePath[scenePath.size() - 1 - i],
                   position.上行路径[i]))
      return false;
  for (const auto &rootFact : role.四根)
    if (rootFact.根.生命周期.创建事实代次 != birth ||
        rootFact.绑定.生命周期.创建事实代次 != birth)
      return false;
  for (const auto &member : v.直接存在成员组)
    if (!场景直接包含事实完整(member, g) ||
        member.种类 != 场景直接包含种类::存在成员 ||
        member.父场景 != position.节点)
      return false;
  for (const auto &child : v.直接子场景组)
    if (!场景直接包含事实完整(child, g) ||
        child.种类 != 场景直接包含种类::子场景 ||
        child.父场景 != position.节点)
      return false;
  return true;
}

bool 当前投影完整(const 真实自我投影 &v, 稳定编码 root,
                   存在单例角色身份 role,
                   const 世界树绑定创建预算 &budget) noexcept {
  const auto g = v.Gread;
  if (!g || !有效(v.E) || v.E == root ||
      v.世界根 != root || v.角色.角色 != role || v.角色.E != v.E ||
      v.位置.节点 != v.E || v.位置.世界根 != root ||
      v.位置.视角 != 世界树节点视角::场景 ||
      !v.位置.直接结构父 || !v.位置.父场景语境 ||
      !当前上行路径完整(v.位置, root, g, budget))
    return false;
  const auto &parent = *v.位置.直接结构父;
  if (!联合父载荷完整(parent, g, parent.父, v.E) ||
      parent.来源 != 直接归属来源::直接子场景 ||
      !父语境完整(*v.位置.父场景语境, parent, g) ||
      !当前场景完整(v.场景, v.位置, root, g, budget,
                      v.角色.存在身份))
    return false;

  const 存在单例角色读取请求 roleRequest{
      1, g, role, budget.最大关系数量};
  const 存在单例角色读取结果 roleResult{
      1, 存在单例角色状态::已读取, g, v.角色};
  const auto birth = v.角色.存在身份.节点创建事实代次;
  return roleResult.成功(roleRequest) &&
         存在身份来源当前见证完整(v.角色.存在身份, g, v.E) &&
         v.角色.目标创建事实代次 == birth &&
         通用概念完整(v.概念, g) && 有效(v.使用.关系) &&
         v.使用.E == v.E && v.使用.EC == v.概念.概念 &&
         生命周期有效(v.使用.生命周期, g) &&
         v.使用.生命周期.创建事实代次 == birth &&
         v.概念.生命周期.创建事实代次 < birth;
}

世界树场景创建请求_v4 创建请求(const 真实自我形成请求 &r,
                                 std::uint64_t g) {
  世界树场景创建请求_v4 out;
  out.G0 = g;
  out.父场景 = r.期望世界根;
  out.定义 = 通用存在概念定义{1, 通用存在定义规则::不预设特征};
  out.组织 = 概念初始组织指定::显式顶层;
  out.概念定义键 = r.概念定义键;
  out.世界键 = r.世界键;
  out.初始角色 = r.角色;
  out.预算 = r.预算;
  out.概念预算 = r.概念预算;
  return out;
}

bool 世界请求相同(const 世界树场景创建请求_v4 &w,
                  const 真实自我形成请求 &r) noexcept {
  const auto *definition = std::get_if<通用存在概念定义>(&w.定义);
  return w.版本 == 4 && w.G0 && w.父场景 == r.期望世界根 &&
         definition && definition->规则版本 == 1 &&
         definition->规则 == 通用存在定义规则::不预设特征 &&
         w.组织 == 概念初始组织指定::显式顶层 && w.直接上位.empty() &&
         w.概念定义键 == r.概念定义键 && w.世界键 == r.世界键 &&
         w.初始角色 && *w.初始角色 == r.角色 &&
         w.预算 == r.预算 && w.概念预算 == r.概念预算;
}

真实自我形成状态 映射世界(世界树概念创建状态_v4 s) noexcept {
  using W = 世界树概念创建状态_v4;
  using S = 真实自我形成状态;
  switch (s) {
  case W::入口拒绝: return S::入口拒绝;
  case W::幂等冲突: return S::请求冲突;
  case W::位置失败: return S::根无效;
  case W::角色冲突: return S::角色冲突;
  case W::概念失败: return S::概念阶段失败;
  case W::世界失败:
  case W::后继覆盖: return S::世界阶段失败;
  case W::已可能发布: return S::已可能发布;
  case W::已发布待复核:
  case W::事实代次漂移:
  case W::数量预算不足:
  case W::资源失败: return S::资源失败;
  case W::内部不一致:
  case W::已完成:
  case W::精确重复: return S::内部不一致;
  }
  return S::内部不一致;
}

真实自我形成状态 映射角色(存在单例角色状态 s) noexcept {
  using R = 存在单例角色状态;
  using S = 真实自我形成状态;
  switch (s) {
  case R::入口拒绝: return S::入口拒绝;
  case R::角色冲突:
  case R::未找到: return S::角色冲突;
  case R::幂等冲突: return S::请求冲突;
  case R::未绑定:
  case R::事实代次漂移:
  case R::数量预算不足: return S::读取未完成;
  case R::已可能发布: return S::已可能发布;
  case R::资源失败: return S::资源失败;
  case R::已登记:
  case R::精确重复:
  case R::已读取:
  case R::内部不一致: return S::内部不一致;
  }
  return S::内部不一致;
}

真实自我形成状态 映射根(世界树操作状态 s) noexcept {
  using W = 世界树操作状态;
  using S = 真实自我形成状态;
  switch (s) {
  case W::入口拒绝: return S::入口拒绝;
  case W::资源失败: return S::资源失败;
  case W::事实代次漂移:
  case W::数量预算不足:
  case W::场景不在现实树:
  case W::引用冲突:
  case W::形成场景环: return S::根无效;
  default: return S::内部不一致;
  }
}

真实自我形成结果 读取当前自我实现(
    const 真实自我读取请求 &, 世界树应用服务 &,
    const 存在类数据服务 &, 存在单例角色身份) noexcept;

} // namespace 海中鱼巣::自我形成内部

namespace 海中鱼巣 {

bool 真实自我形成结果::成功(const 真实自我形成请求 &r) const noexcept {
  if (版本 != 1 || !自我形成内部::请求有效(r) ||
      (状态 != 真实自我形成状态::已形成 &&
       状态 != 真实自我形成状态::精确重复) ||
      !请求回显 || *请求回显 != r || !世界结果 || !投影 ||
      !世界结果->请求回显)
    return false;
  const auto *worldRequest = std::get_if<世界树场景创建请求_v4>(
      &*世界结果->请求回显);
  if (!worldRequest || !自我形成内部::世界请求相同(*worldRequest, r) ||
      !世界结果->成功(*worldRequest) || !世界结果->投影 ||
      世界结果->Gread != 投影->Gread ||
      世界结果->投影->E != 投影->E ||
      世界结果->投影->世界根 != 投影->世界根 ||
      世界结果->投影->内容.概念.概念 != 投影->概念.概念 ||
      世界结果->投影->内容.使用 != 投影->使用 ||
      !世界结果->投影->内容.角色 ||
      世界结果->投影->内容.角色->目标关系 != 投影->角色.目标关系 ||
      !世界结果->世界首次H ||
      *世界结果->世界首次H != 投影->角色.目标创建事实代次)
    return false;
  return 自我形成内部::首次投影完整(*投影, r.期望世界根, r.角色,
                                        r.预算);
}

bool 真实自我形成结果::成功(const 真实自我读取请求 &r) const noexcept {
  return 版本 == 1 && 自我形成内部::请求有效(r) &&
         状态 == 真实自我形成状态::已读取 && !请求回显 && !世界结果 &&
         投影 && 投影->Gread == r.Gread &&
          自我形成内部::当前投影完整(*投影, r.期望世界根, r.角色,
                                        r.预算);
}

真实自我形成服务::真实自我形成服务(
    世界树应用服务 &world, const 存在类数据服务 &existence,
    存在单例角色身份 role)
    : 世界_(world), 存在_(existence), 角色_(role) {
  if (!有效(role.值) || !world.使用存在提供者(existence))
    throw std::invalid_argument("invalid self formation dependencies");
}

真实自我形成结果
自我形成内部::读取当前自我实现(const 真实自我读取请求 &r,
                                  世界树应用服务 &world,
                                  const 存在类数据服务 &existence,
                                  存在单例角色身份 roleIdentity) noexcept {
  using S = 真实自我形成状态;
  真实自我形成结果 out;
  if (!自我形成内部::请求有效(r) || r.角色 != roleIdentity)
    return out;
  try {
    const 世界树读取预算 rootBudget{r.预算.最大场景数量,
                                      r.预算.最大关系数量};
    const auto root = world.读取当前现实世界根(rootBudget);
    if (!root.成功(rootBudget)) {
      out.状态 = 自我形成内部::映射根(root.结果头.状态);
      return out;
    }
    if (root.树->根场景 != r.期望世界根) {
      out.状态 = S::根无效;
      return out;
    }
    // 本入口不把显式读取截止改成后来取得的代次。
    if (root.结果头.Gread != r.Gread) {
      out.状态 = S::读取未完成;
      return out;
    }
    const 存在单例角色读取请求 roleRequest{
        1, r.Gread, r.角色, r.预算.最大关系数量};
    auto role = existence.读取单例角色(roleRequest);
    if (!role.成功(roleRequest)) {
      out.状态 = 自我形成内部::映射角色(role.状态);
      if(role.状态 == 存在单例角色状态::未绑定 &&
         !role.确认未绑定(roleRequest))
        out.状态 = S::内部不一致;
      return out;
    }
    const 世界树存在信息读取请求_v4 worldRequest{
        4, r.Gread, role.事实->E, 世界树节点视角::场景,
        r.预算, r.概念预算};
    auto worldResult = world.读取世界存在信息(worldRequest);
    if (!worldResult.成功(worldRequest)) {
      out.状态 = 自我形成内部::映射世界(worldResult.状态);
      return out;
    }
    if (!worldResult.投影 || !worldResult.投影->内容.场景 ||
        worldResult.投影->内容.角色 || worldResult.投影->E != role.事实->E ||
        worldResult.投影->世界根 != r.期望世界根 ||
        worldResult.Gread != r.Gread) {
      out.状态 = S::内部不一致;
      return out;
    }

    真实自我投影 projection;
    projection.Gread = r.Gread;
    projection.E = role.事实->E;
    projection.世界根 = r.期望世界根;
    projection.角色 = std::move(*role.事实);
    projection.位置 = std::move(worldResult.投影->位置);
    projection.场景 = std::move(*worldResult.投影->内容.场景);
    projection.概念 = std::move(worldResult.投影->内容.概念);
    projection.使用 = std::move(worldResult.投影->内容.使用);
    out.投影 = std::move(projection);
    out.状态 = S::已读取;
    if (!out.成功(r)) {
      out.状态 = S::内部不一致;
      out.投影.reset();
    }
  } catch (const std::bad_alloc &) {
    out.状态 = S::资源失败;
    out.投影.reset();
  } catch (const std::length_error &) {
    out.状态 = S::资源失败;
    out.投影.reset();
  } catch (...) {
    out.状态 = S::内部不一致;
    out.投影.reset();
  }
  return out;
}

真实自我形成结果
真实自我形成服务::读取当前自我(const 真实自我读取请求 &r) const noexcept {
  return 自我形成内部::读取当前自我实现(r,世界_,存在_,角色_);
}

真实自我当前完整读取结果_v2
真实自我形成服务::读取当前自我_v2(
    const 真实自我当前完整读取请求_v2 &r) const noexcept {
  真实自我当前完整读取结果_v2 out;out.Gread=r.Gread;
  if(r.版本!=真实自我当前完整读取合同版本_v2||!r.Gread||
     !有效(r.期望世界根)||!有效(r.角色.值)||r.角色!=角色_)return out;
  try {
    const auto root=世界_.读取当前现实世界根_v3(
        {世界树现实根当前完整读取合同版本_v3});
    if(root.版本!=世界树现实根当前完整读取合同版本_v3||
       root.状态!=世界树现实根当前完整读取状态_v3::已读取||!root.树){
      out.Gread=root.Gread;
      if(root.状态==世界树现实根当前完整读取状态_v3::资源失败)
        out.状态=真实自我形成状态::资源失败;
      else if(root.状态==世界树现实根当前完整读取状态_v3::根未找到)
        out.状态=真实自我形成状态::根无效;
      else if(root.状态==世界树现实根当前完整读取状态_v3::事实代次漂移)
        out.状态=真实自我形成状态::读取未完成;
      else out.状态=真实自我形成状态::内部不一致;
      return out;
    }
    if(root.Gread!=r.Gread){out.Gread=root.Gread;out.状态=真实自我形成状态::读取未完成;return out;}
    if(root.树->根场景!=r.期望世界根){out.状态=真实自我形成状态::根无效;return out;}
    const auto role=存在_.读取单例角色当前完整_v2(
        {存在单例角色当前完整读取合同版本_v2,r.Gread,r.角色});
    if(role.版本!=存在单例角色当前完整读取合同版本_v2||
       role.Gread!=r.Gread||
       role.状态!=存在单例角色当前完整读取状态_v2::已读取||!role.事实){
      if(role.状态==存在单例角色当前完整读取状态_v2::未绑定)
        out.状态=真实自我形成状态::读取未完成;
      else if(role.状态==存在单例角色当前完整读取状态_v2::角色冲突)
        out.状态=真实自我形成状态::角色冲突;
      else if(role.状态==存在单例角色当前完整读取状态_v2::事实代次漂移)
        out.状态=真实自我形成状态::读取未完成;
      else if(role.状态==存在单例角色当前完整读取状态_v2::资源失败)
        out.状态=真实自我形成状态::资源失败;
      else out.状态=真实自我形成状态::内部不一致;
      return out;
    }
    const auto world=世界_.读取世界存在信息当前完整_v5(
        {世界树存在信息当前完整读取合同版本_v5,r.Gread,role.事实->E,
         世界树节点视角::场景});
    if(world.版本!=世界树存在信息当前完整读取合同版本_v5||
       world.Gread!=r.Gread||
       world.状态!=世界树存在信息当前完整读取状态_v5::已读取||!world.投影||
       !world.投影->内容.场景){
      if(world.状态==世界树存在信息当前完整读取状态_v5::事实代次漂移)
        out.状态=真实自我形成状态::读取未完成;
      else if(world.状态==世界树存在信息当前完整读取状态_v5::资源失败)
        out.状态=真实自我形成状态::资源失败;
      else if(world.状态==世界树存在信息当前完整读取状态_v5::位置未找到)
        out.状态=真实自我形成状态::读取未完成;
      else out.状态=真实自我形成状态::内部不一致;
      return out;
    }
    const auto &w=*world.投影;
    if(w.E!=role.事实->E||w.世界根!=r.期望世界根||w.内容.角色||
       w.位置.视角!=世界树节点视角::场景||!w.位置.直接结构父||
       w.位置.上行路径.empty()||
       w.位置.上行路径.back().父!=r.期望世界根||
       w.内容.使用.E!=role.事实->E||w.内容.使用.EC!=w.内容.概念.概念){
      out.状态=真实自我形成状态::内部不一致;return out;
    }
    真实自我投影 projection;
    projection.Gread=r.Gread;projection.E=role.事实->E;
    projection.世界根=r.期望世界根;projection.角色=*role.事实;
    projection.位置=w.位置;projection.场景=*w.内容.场景;
    projection.概念=w.内容.概念;projection.使用=w.内容.使用;
    out.投影=std::move(projection);out.状态=真实自我形成状态::已读取;
  } catch(const std::bad_alloc&){out.状态=真实自我形成状态::资源失败;out.投影.reset();}
    catch(const std::length_error&){out.状态=真实自我形成状态::资源失败;out.投影.reset();}
    catch(...){out.状态=真实自我形成状态::内部不一致;out.投影.reset();}
  return out;
}

真实自我形成结果
真实自我形成服务::形成(const 真实自我形成请求 &r) noexcept {
  using S = 真实自我形成状态;
  真实自我形成结果 out;
  out.请求回显 = r;
  if (!自我形成内部::请求有效(r) || r.角色 != 角色_)
    return out;
  try {
    const 世界树读取预算 rootBudget{r.预算.最大场景数量,
                                     r.预算.最大关系数量};
    const auto root = 世界_.读取当前现实世界根(rootBudget);
    if (!root.成功(rootBudget)) {
      out.状态 = 自我形成内部::映射根(root.结果头.状态);
      return out;
    }
    if (root.树->根场景 != r.期望世界根) {
      out.状态 = S::根无效;
      return out;
    }
    const auto g = root.结果头.Gread;
    const 存在单例角色读取请求 roleRequest{
        1, g, r.角色, r.预算.最大关系数量};
    const auto role = 存在_.读取单例角色(roleRequest);
    const bool alreadyBound = role.成功(roleRequest);
    if (!alreadyBound && !role.确认未绑定(roleRequest)) {
      out.状态 = role.状态 == 存在单例角色状态::未绑定
                     ? S::内部不一致
                     : 自我形成内部::映射角色(role.状态);
      return out;
    }

    // 已有角色也经同一个世界入口核验原键；不凭同一通用EC复用他请求。
    const auto worldRequest = 自我形成内部::创建请求(r, g);
    out.世界结果 = 世界_.创建场景并纳入现实世界(worldRequest);
    const auto &world = *out.世界结果;
    if (!world.成功(worldRequest)) {
      out.状态 = alreadyBound &&
                         world.状态 == 世界树概念创建状态_v4::角色冲突
                     ? S::请求冲突
                     : 自我形成内部::映射世界(world.状态);
      return out;
    }
    if (!world.投影 ||
        (alreadyBound && world.投影->E != role.事实->E)) {
      out.状态 = S::内部不一致;
      return out;
    }

    // 出生返回不是独立读回的输入；按该次共同截止重新读取真实角色和世界。
    const 真实自我读取请求 readRequest{
        1, world.Gread, r.期望世界根, r.角色, r.预算, r.概念预算};
    auto read = 读取当前自我(readRequest);
    if (!read.成功(readRequest)) {
      out.状态 = read.状态;
      return out;
    }
    if (read.投影->E != world.投影->E) {
      out.状态 = S::内部不一致;
      return out;
    }
    out.投影 = std::move(read.投影);
    out.状态 = world.状态 == 世界树概念创建状态_v4::精确重复
                   ? S::精确重复
                   : S::已形成;
    if (!out.成功(r)) {
      out.状态 = S::内部不一致;
      out.投影.reset();
    }
  } catch (const std::bad_alloc &) {
    out.状态 = S::资源失败;
    out.投影.reset();
  } catch (const std::length_error &) {
    out.状态 = S::资源失败;
    out.投影.reset();
  } catch (...) {
    out.状态 = S::内部不一致;
    out.投影.reset();
  }
  return out;
}

} // namespace 海中鱼巣
