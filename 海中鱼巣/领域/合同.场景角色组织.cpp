#include "合同.场景角色组织.h"
#include <system_error>
#include <unordered_set>
namespace 海中鱼巣 {

namespace 场景合同细节 {
using namespace 海中鱼巣;
inline bool 合法角色(场景根角色 v) noexcept {
  return v == 场景根角色::特征 || v == 场景根角色::状态 ||
         v == 场景根角色::动态 || v == 场景根角色::关系;
}
bool 生命周期完整(const 场景事实生命周期 &v, std::uint64_t g) noexcept {
  return v.创建事实代次 && v.创建事实代次 <= g;
}
inline bool 角色完整(const 场景角色当前事实 &v) noexcept {
  if (!v.Gread || !有效(v.场景) ||
      !存在身份来源当前见证完整(v.对象存在来源, v.Gread, v.场景) ||
      v.对象存在来源.身份 != v.场景)
    return false;
  const 场景节点见证 *meta[]{&v.场景族锚点, &v.场景族归属类型, &v.根绑定类型};
  const 稳定编码 m[]{meta[0]->编码, meta[1]->编码, meta[2]->编码};
  for (std::size_t i = 0; i < 3; ++i) {
    if (!有效(m[i]) || !生命周期完整(meta[i]->生命周期, v.Gread))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (m[i] == m[j])
        return false;
  }
  if (!有效(v.场景角色登记边.编码) || v.场景角色登记边.源 != v.场景 ||
      v.场景角色登记边.目标 != m[0] || v.场景角色登记边.关系类型 != m[1] ||
      v.场景角色登记边.角色或顺序 != 1 ||
      !生命周期完整(v.场景角色登记边.生命周期, v.Gread))
    return false;
  const auto gc = v.场景角色登记边.生命周期.创建事实代次;
  if (v.对象存在来源.节点创建事实代次 > gc)
    return false;
  std::array<稳定编码, 8> own{};
  for (std::size_t i = 0; i < 4; ++i) {
    const auto &r = v.四根[i];
    if (static_cast<unsigned>(r.角色) != i + 1 || !有效(r.根.编码) ||
        !有效(r.绑定.编码) || r.绑定.源 != v.场景 || r.绑定.目标 != r.根.编码 ||
        r.绑定.关系类型 != m[2] || r.绑定.角色或顺序 != i + 1)
      return false;
    if (r.根.生命周期.创建事实代次 != gc ||
        r.绑定.生命周期.创建事实代次 != gc)
      return false;
    if (gc > v.Gread)
      return false;
    own[i * 2] = r.根.编码;
    own[i * 2 + 1] = r.绑定.编码;
  }
  for (std::size_t i = 0; i < own.size(); ++i) {
    if (own[i] == v.场景 || own[i] == m[0] || own[i] == m[1] ||
        own[i] == m[2] || own[i] == v.场景角色登记边.编码)
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (own[i] == own[j])
        return false;
  }
  if (v.场景角色登记边.编码 == v.场景 || v.场景角色登记边.编码 == m[0] ||
      v.场景角色登记边.编码 == m[1] || v.场景角色登记边.编码 == m[2])
    return false;
  return true;
}
bool 路径完整(const 场景实例组织事实 &v, std::uint64_t 预算) noexcept {
  if (!角色完整(v.场景角色) || !v.Gread || v.Gread != v.场景角色.Gread ||
      !合法角色(v.角色) || !有效(v.根) ||
      !有效(v.实例) || v.根 == v.实例 || v.路径边.empty() ||
      v.路径边.size() > 预算 || v.路径节点.size() != v.路径边.size() + 1 ||
      v.路径节点.front().编码 != v.根 || v.路径节点.back().编码 != v.实例)
    return false;
  const auto roleIndex =
      static_cast<std::size_t>(static_cast<unsigned>(v.角色) - 1);
  if (roleIndex >= v.场景角色.四根.size() ||
      v.场景角色.四根[roleIndex].根.编码 != v.根)
    return false;
  std::optional<稳定编码> type;
  for (std::size_t i = 0; i < v.路径节点.size(); ++i) {
    const auto &n = v.路径节点[i];
    if (!有效(n.编码) || !生命周期完整(n.生命周期, v.Gread))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (v.路径节点[j].编码 == n.编码)
        return false;
  }
  for (std::size_t i = 0; i < v.路径边.size(); ++i) {
    const auto &e = v.路径边[i];
    if (!有效(e.编码) || !有效(e.关系类型) || e.源 != v.路径节点[i].编码 ||
        e.目标 != v.路径节点[i + 1].编码 || e.角色或顺序 != 1 ||
        !生命周期完整(e.生命周期, v.Gread))
      return false;
    if (!type)
      type = e.关系类型;
    else if (*type != e.关系类型)
      return false;
    for (const auto &n : v.路径节点)
      if (n.编码 == e.编码)
        return false;
    for (std::size_t j = 0; j < i; ++j)
      if (v.路径边[j].编码 == e.编码)
        return false;
  }
  return true;
}
} // namespace 场景合同细节

bool 场景当前身份结果::成功(const 场景当前身份请求 &r) const noexcept {
  return r.版本 == 2 && r.G0 && 有效(r.场景) && 版本 == 2 &&
         状态 == 场景角色数据状态::已读取 && Gread == r.G0;
}
bool 场景角色当前读取结果::成功(const 场景角色当前读取请求 &r) const noexcept {
  return r.版本 == 2 && r.G0 && 有效(r.场景) && 版本 == 2 &&
         状态 == 场景角色数据状态::已读取 && Gread == r.G0 && 角色 &&
         角色->Gread == Gread && 角色->场景 == r.场景 &&
         场景合同细节::角色完整(*角色);
}
bool 场景组织当前读取结果::成功(const 场景组织当前读取请求 &r) const noexcept {
  if (r.版本 != 2 || !r.G0 ||
      (r.角色 != 场景根角色::状态 && r.角色 != 场景根角色::动态) ||
      !有效(r.实例) || r.最大路径长度 < 1 || r.最大路径长度 > 4096 ||
      版本 != 2 || 状态 != 场景角色数据状态::已读取 || Gread != r.G0 ||
      !组织 || 组织->Gread != Gread ||
      !有效(组织->场景角色.场景) || 组织->场景角色.Gread != Gread ||
      组织->角色 != r.角色 || 组织->实例 != r.实例 ||
      !有效(组织->根) || 组织->路径边.empty() ||
      组织->路径边.size() > r.最大路径长度 ||
      组织->路径节点.size() != 组织->路径边.size() + 1 ||
      组织->路径节点.front().编码 != 组织->根 ||
      组织->路径节点.back().编码 != r.实例)
    return false;
  const auto i = static_cast<std::size_t>(static_cast<unsigned>(r.角色) - 1);
  return i < 组织->场景角色.四根.size() &&
         组织->场景角色.四根[i].根.编码 == 组织->根;
}
bool
场景动态组织当前读取结果::成功(const 场景动态组织当前读取请求 &r) const noexcept {
  return r.版本 == 2 && r.G0 && 有效(r.动态) &&
         r.最大路径长度 >= 1 && r.最大路径长度 <= 4096 && 版本 == 2 &&
         状态 == 场景角色数据状态::已读取 && Gread == r.G0 &&
         组织 && 组织->Gread == Gread &&
         组织->场景角色.Gread == Gread &&
         有效(组织->场景角色.场景) && 组织->角色 == 场景根角色::动态 &&
         组织->实例 == r.动态.编码 && 有效(组织->根) && !组织->路径边.empty() &&
         组织->路径边.size() <= r.最大路径长度 &&
         组织->路径节点.size() == 组织->路径边.size() + 1 &&
         组织->路径节点.front().编码 == 组织->根 &&
         组织->路径节点.back().编码 == r.动态.编码 &&
         组织->场景角色.四根[2].根.编码 == 组织->根;
}
bool 场景结构登记结果::成功(const 场景结构登记请求 &r) const noexcept {
  if (r.版本 != 2 || !r.G0 || r.G0 == UINT64_MAX || r.幂等身份.值 != 1 ||
      版本 != 2 ||
      (状态 != 场景角色数据状态::已登记 &&
       状态 != 场景角色数据状态::精确重复) ||
      首次发布代次 != r.G0 + 1 || Gread < 首次发布代次 || !交付)
    return false;
  const 稳定编码 ids[]{交付->绑定结构.场景族锚点,
                       交付->绑定结构.场景族归属关系类型,
                       交付->绑定结构.状态使用绑定成员关系类型,
                       交付->根绑定关系类型,
                       交付->状态组织关系类型,
                       交付->动态组织关系类型,
                       交付->父场景语境关系类型};
  for (std::size_t i = 0; i < 7; ++i) {
    if (!有效(ids[i]))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j])
        return false;
  }
  return true;
}
bool 场景角色写结果::启用成功(const 场景角色启用请求 &r) const noexcept {
  return r.版本 == 4 && r.G0 && r.G0 != UINT64_MAX && r.幂等身份.值 > 1 &&
         有效(r.对象存在) && 有效(r.父场景语境) &&
         有效(r.预期结构父关系) && r.最大关系数量 >= 1 &&
         r.最大关系数量 <= 4096 && r.最大祖先数量 >= 1 &&
         r.最大祖先数量 <= 4096 && 版本 == 2 &&
         (状态 == 场景角色数据状态::已启用 ||
          状态 == 场景角色数据状态::精确重复) &&
         首次发布代次 == r.G0 + 1 && Gread >= 首次发布代次 && 角色 &&
         角色->Gread == Gread &&
         角色->场景 == r.对象存在 && 结构父 &&
         结构父->Gread == Gread &&
         结构父->成员 == r.对象存在 && 结构父->父 == r.父场景语境 &&
         结构父->关系 == r.预期结构父关系 && 父语境投影 &&
         父语境投影->Gread == Gread &&
         父语境投影->场景 == r.对象存在 &&
         父语境投影->父场景语境 == r.父场景语境 &&
         父语境投影->结构父.关系 == r.预期结构父关系;
}
bool 场景角色写结果::退出成功(const 场景角色退出请求 &r) const noexcept {
  return r.版本 == 4 && r.G0 && r.G0 != UINT64_MAX && r.幂等身份.值 > 1 &&
         有效(r.场景) && 有效(r.父场景语境) &&
         有效(r.预期结构父关系) && r.最大关系数量 >= 1 &&
         r.最大关系数量 <= 4096 && 版本 == 2 &&
         (状态 == 场景角色数据状态::已删除 ||
          状态 == 场景角色数据状态::精确重复) &&
         首次发布代次 == r.G0 + 1 && Gread >= 首次发布代次 &&
         ((状态 == 场景角色数据状态::精确重复 && !角色 && !结构父 &&
           !父语境投影) ||
          (状态 == 场景角色数据状态::已删除 && 角色 &&
         角色->Gread == r.G0 &&
         角色->场景 == r.场景 && 结构父 &&
         结构父->Gread == r.G0 &&
         结构父->成员 == r.场景 && 结构父->父 == r.父场景语境 &&
         结构父->关系 == r.预期结构父关系 && 父语境投影 &&
         父语境投影->Gread == r.G0 &&
         父语境投影->场景 == r.场景 &&
         父语境投影->父场景语境 == r.父场景语境 &&
         父语境投影->结构父.关系 == r.预期结构父关系));
}
bool
场景父语境读取结果::成功(const 场景父语境读取请求 &r) const noexcept {
  return r.版本 == 1 && r.G0 && 有效(r.场景) &&
         r.最大关系数量 >= 1 && r.最大关系数量 <= 4096 && 版本 == 1 &&
         状态 == 场景角色数据状态::已读取 && Gread == r.G0 && 投影 &&
         投影->Gread == Gread &&
         投影->场景 == r.场景 && 有效(投影->父场景语境) &&
         投影->结构父.Gread == Gread &&
         投影->结构父.成员 == r.场景 &&
         投影->结构父.父 == 投影->父场景语境 &&
         投影->投影边.源 == r.场景 &&
         投影->投影边.目标 == 投影->父场景语境 &&
         投影->投影边.角色或顺序 == 1;
}
bool
场景组织写结果_v2::状态组织成功(const 场景状态组织请求 &r) const noexcept {
  return r.版本 == 2 && r.G0 && r.G0 != UINT64_MAX && r.幂等身份.值 > 1 &&
         有效(r.场景) && 有效(r.组织父) && 有效(r.状态) &&
         r.最大路径长度 >= 1 && r.最大路径长度 <= 4096 && 版本 == 2 &&
         (状态 == 场景角色数据状态::已组织 ||
          状态 == 场景角色数据状态::精确重复) &&
         首次发布代次 == r.G0 + 1 && Gread >= 首次发布代次 && 组织 &&
         组织->Gread == Gread &&
         组织->场景 == r.场景 && 组织->角色 == 场景根角色::状态 &&
         组织->实例 == r.状态.编码 && 有效(组织->根) &&
         有效(组织->直接组织边.编码) && 有效(组织->直接组织边.关系类型) &&
         组织->直接组织边.源 == r.组织父 &&
         组织->直接组织边.目标 == r.状态.编码 &&
         组织->直接组织边.角色或顺序 == 1 &&
         组织->直接组织边.生命周期.创建事实代次 == 首次发布代次;
}
inline bool
场景组织写结果_v2::动态组织成功(const 场景动态组织请求 &r) const noexcept {
  return r.版本 == 2 && r.G0 && r.G0 != UINT64_MAX && r.幂等身份.值 > 1 &&
         有效(r.场景) && 有效(r.组织父) && 有效(r.动态) &&
         r.最大路径长度 >= 1 && r.最大路径长度 <= 4096 && 版本 == 2 &&
         (状态 == 场景角色数据状态::已组织 ||
          状态 == 场景角色数据状态::精确重复) &&
         首次发布代次 == r.G0 + 1 && Gread >= 首次发布代次 && 组织 &&
         组织->Gread == Gread &&
         组织->场景 == r.场景 && 组织->角色 == 场景根角色::动态 &&
         组织->实例 == r.动态.编码 && 有效(组织->根) &&
         有效(组织->直接组织边.编码) && 有效(组织->直接组织边.关系类型) &&
         组织->直接组织边.源 == r.组织父 &&
         组织->直接组织边.目标 == r.动态.编码 &&
         组织->直接组织边.角色或顺序 == 1 &&
         组织->直接组织边.生命周期.创建事实代次 == 首次发布代次;
}
bool 场景特征组织扩展登记结果::成功(
    const 场景特征组织扩展登记请求 &r) const noexcept {
  return r.版本 == 1 && r.G0 && r.G0 != UINT64_MAX && 版本 == 1 &&
         (状态 == 场景角色数据状态::已登记 ||
          状态 == 场景角色数据状态::精确重复) &&
         首次发布代次 == r.G0 + 1 && Gread >= 首次发布代次 && 交付 &&
         有效(交付->特征组织关系类型);
}
bool
场景特征组织当前读取结果::成功(const 场景特征组织当前读取请求 &r) const noexcept {
  return r.版本 == 1 && r.G0 && 有效(r.特征) &&
         r.最大路径长度 >= 1 && r.最大路径长度 <= 4096 && 版本 == 1 &&
         状态 == 场景角色数据状态::已读取 && Gread == r.G0 &&
         组织 && 组织->Gread == Gread &&
         组织->角色 == 场景根角色::特征 && 组织->实例 == r.特征.编码 &&
         场景合同细节::路径完整(*组织, r.最大路径长度);
}
bool
场景特征组织写结果_v1::成功(const 场景特征组织请求 &r) const noexcept {
  return r.版本 == 1 && r.G0 && r.G0 != UINT64_MAX && r.幂等身份.值 > 1 &&
         r.幂等身份 != 场景特征组织扩展登记固定幂等身份 && 有效(r.场景) &&
         有效(r.组织父) && 有效(r.特征) && r.最大路径长度 >= 1 &&
         r.最大路径长度 <= 4096 && 版本 == 1 &&
         (状态 == 场景角色数据状态::已组织 ||
          状态 == 场景角色数据状态::精确重复) &&
         首次发布代次 == r.G0 + 1 && Gread >= 首次发布代次 && 组织 &&
         组织->Gread == Gread &&
         组织->场景 == r.场景 && 组织->角色 == 场景根角色::特征 &&
         组织->实例 == r.特征.编码 && 有效(组织->根) &&
         组织->直接组织边.源 == r.组织父 &&
         组织->直接组织边.目标 == r.特征.编码 && 有效(组织->直接组织边.编码) &&
         有效(组织->直接组织边.关系类型) && 组织->直接组织边.角色或顺序 == 1 &&
         组织->直接组织边.生命周期.创建事实代次 == 首次发布代次;
}
bool 场景直接包含扩展登记结果::成功(
    const 场景直接包含扩展登记请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX || 版本 != 1 ||
      (状态 != 场景直接包含状态::已登记 &&
       状态 != 场景直接包含状态::精确重复) ||
      首次发布H != r.G0 + 1 || Gread < 首次发布H || !交付)
    return false;
  const 稳定编码 ids[]{交付->树登记锚点, 交付->根标记关系类型,
                       交付->树归属关系类型, 交付->直接存在成员关系类型,
                       交付->直接子场景关系类型};
  for (std::size_t i = 0; i < 5; ++i) {
    if (!有效(ids[i]))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j])
        return false;
  }
  return true;
}
bool 场景直接包含单项结果::成功() const noexcept {
  if (结果头.版本 != 1 || !结果头.Gread)
    return false;
  if (结果头.状态 == 场景直接包含状态::已删除)
    return 结果头.首次发布H && !包含;
  if (结果头.状态 == 场景直接包含状态::精确重复 && !包含)
    return 结果头.首次发布H && *结果头.首次发布H <= 结果头.Gread;
  if (!包含 || 包含->Gread != 结果头.Gread ||
      !有效(包含->父场景) || !有效(包含->成员) || !有效(包含->关系.编码) ||
      包含->关系.源 != 包含->父场景 || 包含->关系.目标 != 包含->成员 ||
      !有效(包含->关系.关系类型) || 包含->关系.角色或顺序 != 1 ||
      !包含->关系.生命周期.创建事实代次 ||
      包含->关系.生命周期.创建事实代次 > 结果头.Gread ||
      (包含->种类 != 场景直接包含种类::存在成员 &&
       包含->种类 != 场景直接包含种类::子场景))
    return false;
  switch (结果头.状态) {
  case 场景直接包含状态::已读取:
    return !结果头.首次发布H;
  case 场景直接包含状态::已新增:
    return 结果头.首次发布H && *结果头.首次发布H <= 结果头.Gread;
  case 场景直接包含状态::精确重复:
    return 结果头.首次发布H && *结果头.首次发布H <= 结果头.Gread;
  default:
    return false;
  }
}
bool
场景直接包含迁移结果::成功(const 场景直接包含迁移请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX || r.幂等身份.值 <= 1 ||
      !有效(r.原父场景) || !有效(r.目标父场景) || r.原父场景 == r.目标父场景 ||
      !有效(r.成员) || r.最大关系数量 < 1 || r.最大关系数量 > 4096 ||
      r.最大场景数量 < 1 || r.最大场景数量 > 4096 || 结果头.版本 != 1 ||
      !结果头.Gread || !结果头.首次发布H ||
      *结果头.首次发布H != r.G0 + 1 || 结果头.Gread < *结果头.首次发布H ||
      (结果头.状态 != 场景直接包含状态::已迁移 &&
       结果头.状态 != 场景直接包含状态::精确重复) || !已建立新包含)
    return false;
  const auto &fresh = *已建立新包含;
  const bool freshBase = fresh.Gread == 结果头.Gread &&
         (fresh.种类 == 场景直接包含种类::存在成员 ||
          fresh.种类 == 场景直接包含种类::子场景) &&
         fresh.父场景 == r.目标父场景 && fresh.成员 == r.成员 &&
         有效(fresh.关系.编码) && fresh.关系.源 == r.目标父场景 &&
         fresh.关系.目标 == r.成员 && 有效(fresh.关系.关系类型) &&
         fresh.关系.角色或顺序 == 1 &&
         fresh.关系.生命周期.创建事实代次 == *结果头.首次发布H;
  if (!freshBase)
    return false;
  if (fresh.种类 == 场景直接包含种类::存在成员)
    return !已建立新父语境;
  if (!已建立新父语境)
    return false;
  const auto &newContext = *已建立新父语境;
  return newContext.Gread == 结果头.Gread && newContext.场景 == r.成员 &&
         newContext.父场景语境 == r.目标父场景 &&
         联合父载荷完整(newContext.结构父, 结果头.Gread,
                        r.目标父场景, r.成员) &&
         newContext.结构父.来源 == 直接归属来源::直接子场景 &&
         newContext.结构父.关系 == fresh.关系.编码 &&
         有效(newContext.投影边.编码) &&
         newContext.投影边.源 == r.成员 &&
         newContext.投影边.目标 == r.目标父场景 &&
         有效(newContext.投影边.关系类型) &&
         newContext.投影边.角色或顺序 == 1 &&
         newContext.投影边.生命周期.创建事实代次 == *结果头.首次发布H;
}
inline bool 场景树写头与角色完整(const 场景直接包含结果头 &h,
                                  const 场景角色当前事实 &role,
                                  const 场景树证明事实 &proof,
                                  std::uint64_t requestG0,
                                  稳定编码 object) noexcept {
  return requestG0 && requestG0 != UINT64_MAX && h.版本 == 1 && h.Gread &&
         h.首次发布H && *h.首次发布H == requestG0 + 1 &&
         *h.首次发布H <= h.Gread && role.Gread == h.Gread &&
         role.场景 == object && 场景合同细节::角色完整(role) &&
         proof.Gread == h.Gread && proof.场景 == object &&
         有效(proof.关系) && 有效(proof.树根) && proof.见证.编码 == proof.关系 &&
         proof.见证.源 == object && 有效(proof.见证.目标) &&
         有效(proof.见证.关系类型) && proof.见证.角色或顺序 == 1 &&
         proof.见证.生命周期.创建事实代次 == *h.首次发布H;
}

bool 新子场景载荷完整(const 场景直接包含事实 &x, std::uint64_t g,
                              std::uint64_t h, 稳定编码 parent,
                              稳定编码 member) noexcept {
  return x.Gread == g && x.种类 == 场景直接包含种类::子场景 &&
         x.父场景 == parent && x.成员 == member && 有效(x.关系.编码) &&
         x.关系.源 == parent && x.关系.目标 == member &&
         有效(x.关系.关系类型) && x.关系.角色或顺序 == 1 &&
         x.关系.生命周期.创建事实代次 == h;
}

bool 场景直接包含组结果::父组读取成功(
    const 场景直接包含反向读取请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || !有效(r.成员) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096 || 结果头.版本 != 1 ||
      结果头.状态 != 场景直接包含状态::已读取 || 结果头.Gread != r.G0 ||
      包含组.size() > r.最大关系数量)
    return false;
  for (std::size_t i = 0; i < 包含组.size(); ++i) {
    const auto &x = 包含组[i];
    if (!场景直接包含事实完整(x, r.G0) || x.成员 != r.成员)
      return false;
    if (i && 包含组[i - 1].关系.编码.值 >= x.关系.编码.值)
      return false;
  }
  return true;
}
bool 场景直接包含组结果::子组读取成功(
    const 场景直接包含组读取请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || !有效(r.父场景) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096 || 结果头.版本 != 1 ||
      结果头.状态 != 场景直接包含状态::已读取 || 结果头.Gread != r.G0 ||
      包含组.size() > r.最大关系数量)
    return false;
  for (std::size_t i = 0; i < 包含组.size(); ++i) {
    const auto &x = 包含组[i];
    if (!场景直接包含事实完整(x, r.G0) || x.父场景 != r.父场景)
      return false;
    if (i && 包含组[i - 1].关系.编码.值 >= x.关系.编码.值)
      return false;
  }
  return true;
}
bool
场景树角色写结果::建根成功(const 场景树根启用请求 &r) const noexcept {
  return r.版本 == 1 && r.幂等身份.值 > 1 && 有效(r.对象存在) &&
         r.最大关系数量 >= 1 && r.最大关系数量 <= 4096 &&
         r.最大祖先数量 >= 1 && r.最大祖先数量 <= 4096 &&
         (结果头.状态 == 场景直接包含状态::已建立场景树根 ||
          结果头.状态 == 场景直接包含状态::精确重复) &&
         场景角色 && 树证明 &&
         场景树写头与角色完整(结果头, *场景角色, *树证明, r.G0,
                                  r.对象存在) &&
         树证明->种类 == 场景树证明种类::根标记 &&
          树证明->树根 == r.对象存在 && !最近上游场景 && !既有直接父 &&
          !新直接包含 && !父语境投影;
}
bool 场景树角色写结果::子场景纳入成功(
    const 场景直接子场景启用请求 &r) const noexcept {
  return r.版本 == 1 && r.幂等身份.值 > 1 && 有效(r.对象存在) &&
         有效(r.父场景) && r.对象存在 != r.父场景 &&
         r.最大关系数量 >= 1 && r.最大关系数量 <= 4096 &&
         r.最大祖先数量 >= 1 && r.最大祖先数量 <= 4096 &&
         (结果头.状态 == 场景直接包含状态::已启用并纳入场景树 ||
          结果头.状态 == 场景直接包含状态::精确重复) &&
         场景角色 && 树证明 && 最近上游场景 && *最近上游场景 == r.父场景 &&
          既有直接父 && 新直接包含 && 父语境投影 &&
         场景树写头与角色完整(结果头, *场景角色, *树证明, r.G0,
                                  r.对象存在) &&
         树证明->种类 == 场景树证明种类::树归属 &&
         联合父载荷完整(*既有直接父, 结果头.Gread, r.父场景,
                          r.对象存在) &&
         既有直接父->来源 == 直接归属来源::直接子场景 &&
         新子场景载荷完整(*新直接包含, 结果头.Gread, *结果头.首次发布H,
                            r.父场景, r.对象存在) &&
          既有直接父->关系 == 新直接包含->关系.编码 &&
          父语境投影->场景 == r.对象存在 &&
          父语境投影->父场景语境 == r.父场景 &&
          父语境投影->结构父.关系 == 既有直接父->关系;
}
bool 场景树当前结果::成功(const 场景树当前读取请求 &r) const noexcept {
  if (r.版本 != 1 || !r.G0 || !有效(r.根场景) || r.最大场景数量 < 1 ||
      r.最大场景数量 > 4096 || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096 || 结果头.版本 != 1 ||
      结果头.状态 != 场景直接包含状态::已读取 || 结果头.Gread != r.G0 ||
      结果头.首次发布H || !树 || 树->Gread != r.G0 ||
      树->根场景 != r.根场景 || 树->场景组.empty() ||
      树->场景组.size() > r.最大场景数量)
    return false;
  std::unordered_set<std::uint64_t> relationIds;
  std::size_t rootCount = 0;
  for (std::size_t i = 0; i < 树->场景组.size(); ++i) {
    const auto &node = 树->场景组[i];
    const auto scene = node.场景角色.场景;
    if (!有效(scene) || node.场景角色.Gread != r.G0 ||
        node.树证明.Gread != r.G0 || node.树证明.场景 != scene ||
        node.树证明.树根 != r.根场景 || !有效(node.树证明.关系) ||
        node.树证明.见证.编码 != node.树证明.关系 ||
        node.树证明.见证.源 != scene ||
        node.树证明.见证.角色或顺序 != 1 ||
        (i && 树->场景组[i - 1].场景角色.场景.值 >= scene.值))
      return false;
    if (!relationIds.insert(node.树证明.关系.值).second)
      return false;
    if (scene == r.根场景) {
      ++rootCount;
      if (node.树证明.种类 != 场景树证明种类::根标记 || node.直接父 ||
          node.父语境投影 ||
          !node.从上游场景到本场景路径.empty())
        return false;
    } else if (node.树证明.种类 != 场景树证明种类::树归属 ||
               !node.直接父 || !node.父语境投影 ||
               node.父语境投影->场景 != scene ||
               node.父语境投影->结构父.关系 != node.直接父->关系 ||
               node.从上游场景到本场景路径.empty()) {
      return false;
    }
    if (node.父语境投影 &&
        !relationIds.insert(node.父语境投影->投影边.编码.值).second)
      return false;
    auto validGroup = [&](const std::vector<场景直接包含事实> &group,
                           场景直接包含种类 kind) noexcept {
      for (std::size_t j = 0; j < group.size(); ++j) {
        if (!场景直接包含事实完整(group[j], r.G0) ||
            group[j].种类 != kind || group[j].父场景 != scene ||
            (j && group[j - 1].关系.编码.值 >= group[j].关系.编码.值))
          return false;
        relationIds.insert(group[j].关系.编码.值);
      }
      return true;
    };
    if (!validGroup(node.直接存在成员组, 场景直接包含种类::存在成员) ||
        !validGroup(node.直接子场景组, 场景直接包含种类::子场景))
      return false;
    const auto &path = node.从上游场景到本场景路径;
    for (std::size_t j = 0; j < path.size(); ++j) {
      const auto &edge = path[j];
      if (edge.Gread != r.G0 || !有效(edge.关系) ||
          !有效(edge.父) || !有效(edge.成员) ||
          edge.创建事实代次 == 0 || edge.创建事实代次 > r.G0 ||
          (edge.来源 != 直接归属来源::存在组成 &&
           edge.来源 != 直接归属来源::场景成员 &&
           edge.来源 != 直接归属来源::直接子场景) ||
          (j && path[j - 1].成员 != edge.父))
        return false;
      relationIds.insert(edge.关系.值);
      if (j + 1 < path.size())
        for (const auto &projected : 树->场景组)
          if (projected.场景角色.场景 == edge.成员)
            return false;
    }
    if (!path.empty()) {
      const auto &tail = path.back();
      if (tail.成员 != scene || !node.直接父 ||
          node.直接父->Gread != tail.Gread ||
          node.直接父->来源 != tail.来源 || node.直接父->关系 != tail.关系 ||
          node.直接父->父 != tail.父 || node.直接父->成员 != tail.成员 ||
          node.直接父->创建事实代次 != tail.创建事实代次)
        return false;
      const auto upstream = std::find_if(
          树->场景组.begin(), 树->场景组.end(), [&](const auto &candidate) {
            return candidate.场景角色.场景 == path.front().父;
          });
      if (upstream == 树->场景组.end())
        return false;
    }
    if (relationIds.size() > r.最大关系数量)
      return false;
  }
  return rootCount == 1 && relationIds.size() <= r.最大关系数量;
}

bool 场景树当前完整读取结果_v2::成功(
    const 场景树当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 场景树当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.根场景) || 版本 != 场景树当前完整读取合同版本_v2 ||
      状态 != 场景树当前完整读取状态_v2::已读取 || Gread != r.G0 ||
      !树 || 树->Gread != r.G0 || 树->根场景 != r.根场景 ||
      树->场景组.empty())
    return false;

  auto 联合相同 = [](const 直接归属联合事实 &a,
                     const 直接归属联合事实 &b) noexcept {
    return a.Gread == b.Gread && a.来源 == b.来源 && a.关系 == b.关系 &&
           a.父 == b.父 && a.成员 == b.成员 &&
           a.创建事实代次 == b.创建事实代次;
  };
  std::vector<直接归属联合事实> 结构边;
  std::unordered_set<std::uint64_t> 证明边, 语境边;
  std::size_t 根数量 = 0;
  auto 登记结构边 = [&](const 直接归属联合事实 &e) {
    if (!联合父载荷完整(e, r.G0, e.父, e.成员) || e.父 == e.成员)
      return false;
    const auto it = std::find_if(结构边.begin(), 结构边.end(),
                                 [&](const auto &x) { return x.关系 == e.关系; });
    if (it != 结构边.end())
      return 联合相同(*it, e);
    结构边.push_back(e);
    return true;
  };
  auto 登记包含 = [&](const 场景直接包含事实 &x) {
    return 登记结构边({r.G0,
        x.种类 == 场景直接包含种类::存在成员
            ? 直接归属来源::场景成员
            : 直接归属来源::直接子场景,
        x.关系.编码, x.父场景, x.成员,
        x.关系.生命周期.创建事实代次});
  };

  for (std::size_t i = 0; i < 树->场景组.size(); ++i) {
    const auto &node = 树->场景组[i];
    const auto scene = node.场景角色.场景;
    if (!有效(scene) || (i && 树->场景组[i - 1].场景角色.场景.值 >= scene.值) ||
        node.场景角色.Gread != r.G0 ||
        !场景合同细节::角色完整(node.场景角色) ||
        node.树证明.Gread != r.G0 || node.树证明.场景 != scene ||
        node.树证明.树根 != r.根场景 || !有效(node.树证明.关系) ||
        node.树证明.见证.编码 != node.树证明.关系 ||
        node.树证明.见证.源 != scene ||
        node.树证明.见证.角色或顺序 != 1 ||
        !node.树证明.见证.生命周期.创建事实代次 ||
        node.树证明.见证.生命周期.创建事实代次 > r.G0 ||
        !证明边.insert(node.树证明.关系.值).second)
      return false;

    if (scene == r.根场景) {
      ++根数量;
      if (node.树证明.种类 != 场景树证明种类::根标记 || node.直接父 ||
          node.父语境投影 || !node.从上游场景到本场景路径.empty())
        return false;
    } else {
      if (node.树证明.种类 != 场景树证明种类::树归属 || !node.直接父 ||
          !node.父语境投影 || node.从上游场景到本场景路径.empty() ||
          node.父语境投影->Gread != r.G0 ||
          node.父语境投影->场景 != scene ||
          node.父语境投影->父场景语境 != node.直接父->父 ||
          !联合相同(node.父语境投影->结构父, *node.直接父) ||
          !有效(node.父语境投影->投影边.编码) ||
          node.父语境投影->投影边.源 != scene ||
          node.父语境投影->投影边.目标 != node.直接父->父 ||
          node.父语境投影->投影边.角色或顺序 != 1 ||
          !node.父语境投影->投影边.生命周期.创建事实代次 ||
          node.父语境投影->投影边.生命周期.创建事实代次 > r.G0 ||
          !语境边.insert(node.父语境投影->投影边.编码.值).second ||
          !登记结构边(*node.直接父))
        return false;
    }

    auto 核验组 = [&](const std::vector<场景直接包含事实> &group,
                      场景直接包含种类 kind) {
      for (std::size_t j = 0; j < group.size(); ++j) {
        if (!场景直接包含事实完整(group[j], r.G0) ||
            group[j].种类 != kind || group[j].父场景 != scene ||
            (j && group[j - 1].关系.编码.值 >= group[j].关系.编码.值) ||
            !登记包含(group[j]))
          return false;
        if (kind == 场景直接包含种类::子场景) {
          const auto child = std::find_if(
              树->场景组.begin(), 树->场景组.end(), [&](const auto &candidate) {
                return candidate.场景角色.场景 == group[j].成员;
              });
          if (child == 树->场景组.end() || !child->直接父 ||
              child->直接父->来源 != 直接归属来源::直接子场景 ||
              child->直接父->关系 != group[j].关系.编码 ||
              child->直接父->父 != scene || child->直接父->成员 != group[j].成员 ||
              child->直接父->创建事实代次 !=
                  group[j].关系.生命周期.创建事实代次)
            return false;
        }
      }
      return true;
    };
    if (!核验组(node.直接存在成员组, 场景直接包含种类::存在成员) ||
        !核验组(node.直接子场景组, 场景直接包含种类::子场景))
      return false;

    const auto &path = node.从上游场景到本场景路径;
    for (std::size_t j = 0; j < path.size(); ++j) {
      if (!登记结构边(path[j]) || (j && path[j - 1].成员 != path[j].父))
        return false;
      if (j + 1 < path.size() && std::ranges::any_of(
              树->场景组, [&](const auto &candidate) {
                return candidate.场景角色.场景 == path[j].成员;
              }))
        return false;
    }
    if (!path.empty()) {
      if (!node.直接父 || !联合相同(path.back(), *node.直接父))
        return false;
      if (std::ranges::none_of(树->场景组, [&](const auto &candidate) {
            return candidate.场景角色.场景 == path.front().父;
          }))
        return false;
    }
  }
  for (const auto id : 证明边)
    if (语境边.contains(id) || std::ranges::any_of(
            结构边, [&](const auto &e) { return e.关系.值 == id; }))
      return false;
  for (const auto id : 语境边)
    if (std::ranges::any_of(结构边,
                           [&](const auto &e) { return e.关系.值 == id; }))
      return false;
  return 根数量 == 1;
}

inline 直接归属联合只读状态 联合映射(存在结构身份只读状态 s) noexcept {
  using A = 存在结构身份只读状态;
  using B = 直接归属联合只读状态;
  switch (s) {
  case A::已读取:
    return B::已读取;
  case A::入口拒绝:
    return B::入口拒绝;
  case A::未找到:
    return B::成员未找到;
  case A::事实代次漂移:
    return B::事实代次漂移;
  case A::数量预算不足:
    return B::数量预算不足;
  case A::资源失败:
    return B::资源失败;
  default:
    return B::内部不一致;
  }
}
inline 直接归属联合只读状态 联合映射(场景直接包含状态 s) noexcept {
  using A = 场景直接包含状态;
  using B = 直接归属联合只读状态;
  switch (s) {
  case A::已读取:
    return B::已读取;
  case A::入口拒绝:
    return B::入口拒绝;
  case A::成员未找到:
    return B::成员未找到;
  case A::事实代次漂移:
    return B::事实代次漂移;
  case A::数量预算不足:
    return B::数量预算不足;
  case A::资源失败:
    return B::资源失败;
  default:
    return B::内部不一致;
  }
}

bool 场景直接包含当前完整读取结果_v2::父组读取成功(
    const 场景直接包含父组当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 场景直接包含当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.成员) || 版本 != 场景直接包含当前完整读取合同版本_v2 ||
      状态 != 场景直接包含当前完整读取状态_v2::已读取 || Gread != r.G0 ||
      包含组.size() > 1)
    return false;
  for (std::size_t i = 0; i < 包含组.size(); ++i) {
    const auto &x = 包含组[i];
    if (!场景直接包含事实完整(x, Gread) || x.成员 != r.成员 ||
        x.父场景 == x.成员 ||
        (i && 包含组[i - 1].关系.编码.值 >= x.关系.编码.值))
      return false;
  }
  return true;
}

bool 场景直接包含当前完整读取结果_v2::子组读取成功(
    const 场景直接包含子组当前完整读取请求_v2 &r) const noexcept {
  if (r.版本 != 场景直接包含当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.父) || 版本 != 场景直接包含当前完整读取合同版本_v2 ||
      状态 != 场景直接包含当前完整读取状态_v2::已读取 || Gread != r.G0)
    return false;
  for (std::size_t i = 0; i < 包含组.size(); ++i) {
    const auto &x = 包含组[i];
    if (!场景直接包含事实完整(x, Gread) || x.父场景 != r.父 ||
        x.父场景 == x.成员 ||
        (i && 包含组[i - 1].关系.编码.值 >= x.关系.编码.值))
      return false;
  }
  return true;
}

namespace {
直接归属联合当前完整读取状态_v2 联合映射_v2(
    存在组成当前完整读取状态_v2 状态) noexcept {
  using A = 存在组成当前完整读取状态_v2;
  using B = 直接归属联合当前完整读取状态_v2;
  switch (状态) {
  case A::已读取: return B::已读取;
  case A::入口拒绝: return B::内部不一致;
  case A::未找到: return B::未找到;
  case A::事实代次漂移: return B::事实代次漂移;
  case A::资源失败: return B::资源失败;
  default: return B::内部不一致;
  }
}

直接归属联合当前完整读取状态_v2 联合映射_v2(
    场景直接包含当前完整读取状态_v2 状态) noexcept {
  using A = 场景直接包含当前完整读取状态_v2;
  using B = 直接归属联合当前完整读取状态_v2;
  switch (状态) {
  case A::已读取: return B::已读取;
  case A::入口拒绝: return B::内部不一致;
  case A::未找到: return B::未找到;
  case A::事实代次漂移: return B::事实代次漂移;
  case A::资源失败: return B::资源失败;
  default: return B::内部不一致;
  }
}
} // namespace
直接归属联合读取结果
直接归属联合只读组合器::读取当前联合父(const 直接归属联合父读取请求 &r) const {
  直接归属联合读取结果 o;
  o.Gread = r.G0;
  if (r.版本 != 1 || !r.G0 || !有效(r.成员) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096)
    return o;
  try {
    const 存在组成父读取请求 erq{1, r.G0, r.成员, r.最大关系数量};
    auto er = e_.读取当前组成父(erq);
    if (!er.父读取成功(erq)) {
      o.状态 = er.状态 == 存在结构身份只读状态::已读取
                   ? 直接归属联合只读状态::内部不一致
                   : 联合映射(er.状态);
      o.Gread = er.Gread;
      return o;
    }
    const 场景直接包含反向读取请求 srq{1, r.G0, r.成员, r.最大关系数量};
    auto sr = s_.读取当前场景包含父组(srq);
    if (!sr.父组读取成功(srq)) {
      o.状态 = sr.结果头.状态 == 场景直接包含状态::已读取
                   ? 直接归属联合只读状态::内部不一致
                   : 联合映射(sr.结果头.状态);
      o.Gread = sr.结果头.Gread;
      return o;
    }
    std::vector<直接归属联合事实> xs;
    if (er.父)
      xs.push_back({r.G0, 直接归属来源::存在组成, er.父->关系,
                    er.父->父存在, er.父->子存在, er.父->创建事实代次});
    for (const auto &x : sr.包含组)
      xs.push_back({r.G0,
                    x.种类 == 场景直接包含种类::存在成员
                        ? 直接归属来源::场景成员
                        : 直接归属来源::直接子场景,
                    x.关系.编码, x.父场景, x.成员,
                    x.关系.生命周期.创建事实代次});
    if (xs.size() > 1) {
      o.状态 = 直接归属联合只读状态::内部不一致;
      return o;
    }
    o.状态 = 直接归属联合只读状态::已读取;
    if (!xs.empty())
      o.父 = xs.front();
  } catch (const std::bad_alloc &) {
    o.状态 = 直接归属联合只读状态::资源失败;
  } catch (...) {
    o.状态 = 直接归属联合只读状态::内部不一致;
  }
  return o;
}

直接归属联合读取结果 直接归属联合只读组合器::读取当前联合子组(
    const 直接归属联合子组读取请求 &r) const {
  直接归属联合读取结果 o;
  o.Gread = r.G0;
  if (r.版本 != 1 || !r.G0 || !有效(r.父) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096)
    return o;
  try {
    const 存在组成子组读取请求 erq{1, r.G0, r.父, r.最大关系数量};
    auto er = e_.读取当前组成子组(erq);
    if (!er.子组读取成功(erq)) {
      o.状态 = er.状态 == 存在结构身份只读状态::已读取
                   ? 直接归属联合只读状态::内部不一致
                   : 联合映射(er.状态);
      o.Gread = er.Gread;
      return o;
    }
    const 场景直接包含组读取请求 srq{1, r.G0, r.父, r.最大关系数量};
    auto sr = s_.读取当前场景包含子组(srq);
    if (!sr.子组读取成功(srq)) {
      o.状态 = sr.结果头.状态 == 场景直接包含状态::已读取
                   ? 直接归属联合只读状态::内部不一致
                   : 联合映射(sr.结果头.状态);
      o.Gread = sr.结果头.Gread;
      return o;
    }
    for (const auto &x : er.子组)
      o.子组.push_back({r.G0, 直接归属来源::存在组成, x.关系, x.父存在,
                        x.子存在, x.创建事实代次});
    for (const auto &x : sr.包含组)
      o.子组.push_back(
          {r.G0,
           x.种类 == 场景直接包含种类::存在成员 ? 直接归属来源::场景成员
                                                : 直接归属来源::直接子场景,
           x.关系.编码, x.父场景, x.成员,
           x.关系.生命周期.创建事实代次});
    if (o.子组.size() > r.最大关系数量) {
      o.状态 = 直接归属联合只读状态::数量预算不足;
      o.子组.clear();
      return o;
    }
    std::sort(o.子组.begin(), o.子组.end(), [](const auto &a, const auto &b) {
      if (a.成员 != b.成员)
        return a.成员.值 < b.成员.值;
      if (a.来源 != b.来源)
        return static_cast<unsigned>(a.来源) < static_cast<unsigned>(b.来源);
      return a.关系.值 < b.关系.值;
    });
    o.状态 = 直接归属联合只读状态::已读取;
  } catch (const std::bad_alloc &) {
    o.状态 = 直接归属联合只读状态::资源失败;
  } catch (...) {
    o.状态 = 直接归属联合只读状态::内部不一致;
  }
  if (o.状态 != 直接归属联合只读状态::已读取)
    o.子组.clear();
  return o;
}

直接归属联合当前完整读取结果_v2
直接归属联合只读组合器::读取当前联合父_v2(
    const 直接归属联合父当前完整读取请求_v2 &r) const {
  直接归属联合当前完整读取结果_v2 out;
  out.Gread = r.G0;
  if (r.版本 != 直接归属联合当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.成员))
    return out;
  try {
    const 存在组成父当前完整读取请求_v2 组成请求{
        存在组成当前完整读取合同版本_v2, r.G0, r.成员};
    const auto 组成 = e_.读取当前组成父_v2(组成请求);
    if (!组成.父读取成功(组成请求)) {
      out.状态 = 组成.状态 == 存在组成当前完整读取状态_v2::已读取
                       ? 直接归属联合当前完整读取状态_v2::内部不一致
                       : 联合映射_v2(组成.状态);
      out.Gread = 组成.Gread;
      return out;
    }
    const 场景直接包含父组当前完整读取请求_v2 场景请求{
        场景直接包含当前完整读取合同版本_v2, r.G0, r.成员};
    const auto 场景 = s_.读取当前场景包含父组_v2(场景请求);
    if (!场景.父组读取成功(场景请求)) {
      out.状态 = 场景.状态 == 场景直接包含当前完整读取状态_v2::已读取
                       ? 直接归属联合当前完整读取状态_v2::内部不一致
                       : 联合映射_v2(场景.状态);
      out.Gread = 场景.Gread;
      return out;
    }
    std::vector<直接归属联合事实> 候选;
    if (组成.父) {
      const auto &x = *组成.父;
      候选.push_back({r.G0, 直接归属来源::存在组成, x.关系,
                    x.父存在, x.子存在, x.创建事实代次});
    }
    for (const auto &x : 场景.包含组) {
      候选.push_back({r.G0,
                    x.种类 == 场景直接包含种类::存在成员
                        ? 直接归属来源::场景成员
                        : 直接归属来源::直接子场景,
                    x.关系.编码, x.父场景, x.成员,
                    x.关系.生命周期.创建事实代次});
    }
    if (候选.size() > 1) {
      out.状态 = 直接归属联合当前完整读取状态_v2::内部不一致;
      return out;
    }
    if (!候选.empty())
      out.父 = 候选.front();
    out.状态 = 直接归属联合当前完整读取状态_v2::已读取;
  } catch (const std::bad_alloc &) {
    out.状态 = 直接归属联合当前完整读取状态_v2::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 直接归属联合当前完整读取状态_v2::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 直接归属联合当前完整读取状态_v2::资源失败;
  } catch (...) {
    out.状态 = 直接归属联合当前完整读取状态_v2::内部不一致;
  }
  if (out.状态 != 直接归属联合当前完整读取状态_v2::已读取)
    out.父.reset();
  return out;
}

直接归属联合当前完整读取结果_v2
直接归属联合只读组合器::读取当前联合子组_v2(
    const 直接归属联合子组当前完整读取请求_v2 &r) const {
  直接归属联合当前完整读取结果_v2 out;
  out.Gread = r.G0;
  if (r.版本 != 直接归属联合当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.父))
    return out;
  try {
    const 存在组成子组当前完整读取请求_v2 组成请求{
        存在组成当前完整读取合同版本_v2, r.G0, r.父};
    const auto 组成 = e_.读取当前组成子组_v2(组成请求);
    if (!组成.子组读取成功(组成请求)) {
      out.状态 = 组成.状态 == 存在组成当前完整读取状态_v2::已读取
                       ? 直接归属联合当前完整读取状态_v2::内部不一致
                       : 联合映射_v2(组成.状态);
      out.Gread = 组成.Gread;
      return out;
    }
    const 场景直接包含子组当前完整读取请求_v2 场景请求{
        场景直接包含当前完整读取合同版本_v2, r.G0, r.父};
    const auto 场景 = s_.读取当前场景包含子组_v2(场景请求);
    if (!场景.子组读取成功(场景请求)) {
      out.状态 = 场景.状态 == 场景直接包含当前完整读取状态_v2::已读取
                       ? 直接归属联合当前完整读取状态_v2::内部不一致
                       : 联合映射_v2(场景.状态);
      out.Gread = 场景.Gread;
      return out;
    }
    for (const auto &x : 组成.子组)
      out.子组.push_back({r.G0, 直接归属来源::存在组成, x.关系,
                         x.父存在, x.子存在, x.创建事实代次});
    for (const auto &x : 场景.包含组)
      out.子组.push_back(
          {r.G0,
           x.种类 == 场景直接包含种类::存在成员 ? 直接归属来源::场景成员
                                                : 直接归属来源::直接子场景,
           x.关系.编码, x.父场景, x.成员,
           x.关系.生命周期.创建事实代次});
    std::sort(out.子组.begin(), out.子组.end(), [](const auto &a, const auto &b) {
      if (a.成员 != b.成员)
        return a.成员.值 < b.成员.值;
      if (a.来源 != b.来源)
        return static_cast<unsigned>(a.来源) < static_cast<unsigned>(b.来源);
      return a.关系.值 < b.关系.值;
    });
    std::unordered_set<std::uint64_t> 关系组;
    for (std::size_t i = 0; i < out.子组.size(); ++i) {
      if (!关系组.insert(out.子组[i].关系.值).second ||
          (i && out.子组[i - 1].成员 == out.子组[i].成员)) {
        out.状态 = 直接归属联合当前完整读取状态_v2::内部不一致;
        out.子组.clear();
        return out;
      }
    }
    out.状态 = 直接归属联合当前完整读取状态_v2::已读取;
  } catch (const std::bad_alloc &) {
    out.状态 = 直接归属联合当前完整读取状态_v2::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 直接归属联合当前完整读取状态_v2::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 直接归属联合当前完整读取状态_v2::资源失败;
  } catch (...) {
    out.状态 = 直接归属联合当前完整读取状态_v2::内部不一致;
  }
  if (out.状态 != 直接归属联合当前完整读取状态_v2::已读取)
    out.子组.clear();
  return out;
}
} // namespace 海中鱼巣
