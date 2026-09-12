module;
#include <algorithm>
#include <cstdint>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <unordered_set>
export module 海中鱼巣.业务.应用服务.世界树类;
export import 海中鱼巣.领域.数据服务.存在类;
export import 海中鱼巣.领域.数据服务.场景类;
export import 海中鱼巣.领域.数据服务.绑定存在;
export import 海中鱼巣.领域.数据服务.世界树根;
export namespace 海中鱼巣 {
inline constexpr std::uint32_t 世界树应用合同版本 = 3;
enum class 世界树操作阶段 : std::uint8_t {
  无 = 0,
  现实树预读 = 1,
  存在创建 = 2,
  场景包含发布 = 3,
  现实树最终确认 = 4
};
enum class 世界树操作状态 : std::uint8_t {
  已验证现实根 = 1,
  已读取存在位置 = 2,
  已移动 = 3,
  已创建场景并纳入 = 4,
  已创建存在并纳入 = 5,
  精确重复 = 6,
  入口拒绝 = 7,
  场景不在现实树 = 8,
  存在不在现实树 = 9,
  成员多重位置 = 10,
  现实根不可移动 = 11,
  原位置不匹配 = 12,
  目标位置相同 = 13,
  形成场景环 = 14,
  引用冲突 = 15,
  事实代次漂移 = 16,
  幂等冲突 = 17,
  数量预算不足 = 18,
  历史材料已清理 = 19,
  资源失败 = 20,
  内部不一致 = 21,
  部分已发布 = 22,
  已发布待复核 = 23,
  结果未知 = 24,
  既有操作已被后继事实覆盖 = 25
};
struct 世界树读取预算 final {
  std::uint64_t 最大场景数量 = 0, 最大关系数量 = 0;
  friend bool operator==(const 世界树读取预算 &,
                         const 世界树读取预算 &) = default;
};
struct 世界树根验证请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t G0 = 0;
  世界树读取预算 预算;
  friend bool operator==(const 世界树根验证请求 &,
                         const 世界树根验证请求 &) = default;
};
struct 世界树存在位置读取请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t G0 = 0;
  稳定编码 存在{};
  世界树读取预算 预算;
  friend bool operator==(const 世界树存在位置读取请求 &,
                         const 世界树存在位置读取请求 &) = default;
};
struct 世界树成员移动请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 原场景{}, 目标场景{}, 成员{};
  世界树读取预算 预算;
  friend bool operator==(const 世界树成员移动请求 &,
                         const 世界树成员移动请求 &) = default;
};
struct 世界树绑定创建预算 final {
  std::uint64_t 最大场景数量 = 0, 最大关系数量 = 0, 最大祖先数量 = 0;
  friend bool operator==(const 世界树绑定创建预算 &, const 世界树绑定创建预算 &) = default;
};
struct 世界树场景创建请求 final {
  std::uint32_t 版本 = 3;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 组合幂等身份{}, 存在幂等身份{}, 场景幂等身份{};
  稳定编码 父场景{};
  世界树绑定创建预算 预算{};
  friend bool operator==(const 世界树场景创建请求 &, const 世界树场景创建请求 &) = default;
};
struct 世界树存在创建请求 final {
  std::uint32_t 版本 = 3;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 组合幂等身份{}, 存在幂等身份{}, 场景幂等身份{};
  稳定编码 目标场景{};
  世界树绑定创建预算 预算{};
  friend bool operator==(const 世界树存在创建请求 &, const 世界树存在创建请求 &) = default;
};
struct 世界树结果头 final {
  std::uint32_t 版本 = 2;
  世界树操作状态 状态 = 世界树操作状态::入口拒绝;
  世界树操作阶段 阶段 = 世界树操作阶段::无;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布H;
  std::optional<存在类数据状态> 存在原因;
  std::optional<场景直接包含状态> 场景原因;
  std::optional<绑定存在创建状态> 绑定原因;
};
struct 世界树存在位置 final {
  稳定编码 存在{}, 所在场景{};
  场景直接包含事实 成员关系;
};
struct 世界树移动投影 final {
  稳定编码 成员{}, 原场景{}, 目标场景{}, 旧关系{}, 新关系{};
  std::uint64_t 首次发布H = 0;
};
struct 世界树移动进度 final {
  场景直接包含迁移请求 已交换请求;
  std::optional<std::uint64_t> 首次发布H;
};

struct 世界树场景创建投影 final {
  稳定编码 E{}, 父场景{};
  场景树节点当前事实 场景;
};

struct 世界树存在创建投影 final {
  稳定编码 存在{};
  世界树存在位置 位置;
};

enum class 世界树节点视角 : std::uint8_t { 世界根场景 = 1, 场景 = 2, 存在 = 3 };
struct 世界树层级位置读取请求 final {
  std::uint32_t 版本 = 世界树应用合同版本;
  std::uint64_t G0 = 0;
  稳定编码 节点{};
  世界树节点视角 预期视角 = 世界树节点视角::存在;
  世界树读取预算 预算{};
};
struct 世界树层级位置 final {
  稳定编码 节点{}, 世界根{};
  世界树节点视角 视角 = 世界树节点视角::存在;
  std::optional<直接归属联合事实> 直接结构父;
  std::optional<场景父语境投影事实> 父场景语境;
  std::vector<直接归属联合事实> 上行路径;
};
struct 世界树层级位置结果 final {
  世界树结果头 结果头;
  std::optional<世界树层级位置> 位置;
  bool 成功(const 世界树层级位置读取请求 &r) const noexcept {
    return r.版本 == 世界树应用合同版本 &&
           结果头.版本 == 世界树应用合同版本 &&
           结果头.状态 == 世界树操作状态::已读取存在位置 &&
           结果头.Gread == r.G0 && 位置 && 位置->节点 == r.节点 &&
           位置->视角 == r.预期视角 && 有效(位置->世界根);
  }
};
struct 世界树全局根投影 final {
  稳定编码 根{};
  std::uint64_t 首次H = 0, 最近验证G = 0;
};
enum class 世界树全局根读取状态 : std::uint8_t { 已读取 = 1, 未绑定 = 2 };
struct 世界树全局根读取结果 final {
  世界树全局根读取状态 状态 = 世界树全局根读取状态::未绑定;
  std::optional<世界树全局根投影> 投影;
  bool 成功() const noexcept {
    return 状态 == 世界树全局根读取状态::已读取 && 投影 &&
           有效(投影->根) && 投影->首次H &&
           投影->首次H <= 投影->最近验证G;
  }
};
class 世界树应用服务;
struct 世界树应用服务建立结果;
世界树全局根读取结果 读取世界树根节点() noexcept;
世界树应用服务建立结果
建立世界树应用服务(场景类数据服务 &, 存在类数据服务 &,
                     const 世界树根验证请求 &) noexcept;
struct 世界树根验证结果 final {
  世界树结果头 结果头;
  std::optional<场景树当前事实> 树;
  bool 成功(const 世界树根验证请求 &r) const noexcept {
    return r.版本 == 2 && r.G0 && 结果头.版本 == 2 &&
           结果头.状态 == 世界树操作状态::已验证现实根 &&
           结果头.阶段 == 世界树操作阶段::现实树预读 && 结果头.Gread == r.G0 &&
           !结果头.首次发布H && !结果头.存在原因 && !结果头.场景原因 && 树 &&
           树->Gread == r.G0 && 有效(树->根场景) && !树->场景组.empty() &&
           树->场景组.size() <= r.预算.最大场景数量;
  }
};
struct 世界树存在位置结果 final {
  世界树结果头 结果头;
  std::optional<世界树存在位置> 位置;
  bool 成功(const 世界树存在位置读取请求 &r) const noexcept {
    return r.版本 == 2 && r.G0 && 有效(r.存在) &&
           结果头.版本 == 2 && 结果头.状态 == 世界树操作状态::已读取存在位置 &&
           结果头.阶段 == 世界树操作阶段::现实树预读 && 结果头.Gread == r.G0 &&
           !结果头.首次发布H && !结果头.存在原因 && !结果头.场景原因 && 位置 &&
           位置->存在 == r.存在 && 有效(位置->所在场景) &&
           位置->成员关系.Gread == r.G0 && 位置->成员关系.H == r.G0 &&
           位置->成员关系.种类 == 场景直接包含种类::存在成员 &&
           位置->成员关系.父场景 == 位置->所在场景 &&
           位置->成员关系.成员 == r.存在;
  }
};
struct 世界树移动结果 final {
  世界树结果头 结果头;
  std::optional<世界树成员移动请求> 原请求;
  std::optional<世界树移动进度> 进度;
  std::optional<世界树移动投影> 投影;
  bool 成功(const 世界树成员移动请求 &r) const noexcept {
    return r.版本 == 2 && r.G0 && 结果头.版本 == 2 &&
           (结果头.状态 == 世界树操作状态::已移动 ||
            结果头.状态 == 世界树操作状态::精确重复) &&
           结果头.阶段 == 世界树操作阶段::现实树最终确认 && 结果头.Gread &&
           结果头.首次发布H && *结果头.首次发布H <= 结果头.Gread && 原请求 &&
           *原请求 == r && 投影 && !进度 && !结果头.存在原因 &&
           !结果头.场景原因 && 投影->成员 == r.成员 &&
           投影->原场景 == r.原场景 && 投影->目标场景 == r.目标场景 &&
           有效(投影->旧关系) && 有效(投影->新关系) &&
           投影->首次发布H == *结果头.首次发布H;
  }
};
struct 世界树场景创建结果 final {
  世界树结果头 结果头;
  std::optional<世界树场景创建请求> 原请求;
  std::optional<世界树场景创建投影> 投影;
  bool 成功(const 世界树场景创建请求 &r) const noexcept {
    if (!(r.版本 == 3 && 结果头.版本 == 3 &&
          (结果头.状态 == 世界树操作状态::已创建场景并纳入 ||
           结果头.状态 == 世界树操作状态::精确重复) &&
          结果头.阶段 == 世界树操作阶段::现实树最终确认 && 结果头.Gread &&
          结果头.首次发布H && r.G0 < *结果头.首次发布H &&
          *结果头.首次发布H <= 结果头.Gread && 原请求 && *原请求 == r && 投影 &&
          投影->父场景 == r.父场景 && 投影->E == 投影->场景.场景角色.场景 &&
          !结果头.存在原因 && !结果头.场景原因 &&
          (结果头.绑定原因 == 绑定存在创建状态::已创建 ||
           结果头.绑定原因 == 绑定存在创建状态::精确重复)))
      return false;
    const auto g = 结果头.Gread, h = *结果头.首次发布H;
    const auto &n = 投影->场景;
    const auto &role = n.场景角色;
    const auto current = [&](const 场景事实生命周期 &life) {
      return life.创建事实代次 && life.创建事实代次 <= g && !life.退出事实代次;
    };
    if (!有效(r.父场景) || !有效(投影->E) || 投影->E == r.父场景 ||
        role.Gread != g || role.H != g ||
        !存在身份来源历史见证完整(role.对象存在来源, g, 投影->E) ||
        role.对象存在来源.节点生命周期.创建事实代次 != h ||
        !有效(role.场景族锚点.编码) || !有效(role.场景族归属类型.编码) ||
        !有效(role.根绑定类型.编码) || !current(role.场景族锚点.生命周期) ||
        !current(role.场景族归属类型.生命周期) ||
        !current(role.根绑定类型.生命周期) || !有效(role.场景角色登记边.编码) ||
        role.场景角色登记边.源 != 投影->E ||
        role.场景角色登记边.目标 != role.场景族锚点.编码 ||
        role.场景角色登记边.关系类型 != role.场景族归属类型.编码 ||
        role.场景角色登记边.角色或顺序 != 1 ||
        role.场景角色登记边.生命周期.创建事实代次 != h ||
        !current(role.场景角色登记边.生命周期))
      return false;
    for (std::size_t i = 0; i < 4; ++i) {
      const auto &root = role.四根[i];
      if (root.角色 != static_cast<场景根角色>(i + 1) || !有效(root.根.编码) ||
          !有效(root.绑定.编码) || root.绑定.源 != 投影->E ||
          root.绑定.目标 != root.根.编码 ||
          root.绑定.关系类型 != role.根绑定类型.编码 ||
          root.绑定.角色或顺序 != i + 1 || root.根.生命周期.创建事实代次 != h ||
          root.绑定.生命周期.创建事实代次 != h || !current(root.根.生命周期) ||
          !current(root.绑定.生命周期))
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (root.根.编码 == role.四根[j].根.编码 ||
            root.绑定.编码 == role.四根[j].绑定.编码)
          return false;
    }
    return n.树证明.Gread == g && n.树证明.H == g &&
           n.树证明.种类 == 场景树证明种类::树归属 &&
           n.树证明.场景 == 投影->E && 有效(n.树证明.树根) &&
           n.树证明.树根 != 投影->E && 有效(n.树证明.关系) &&
           n.树证明.关系 == n.树证明.见证.编码 && n.树证明.见证.源 == 投影->E &&
           n.树证明.见证.目标 == n.树证明.树根 &&
           有效(n.树证明.见证.关系类型) && n.树证明.见证.角色或顺序 == 1 &&
           n.树证明.见证.生命周期.创建事实代次 == h &&
           current(n.树证明.见证.生命周期) && n.直接父 &&
           n.直接父->Gread == g && n.直接父->H == g &&
           n.直接父->父 == r.父场景 && n.直接父->成员 == 投影->E &&
           n.直接父->来源 == 直接归属来源::直接子场景 && 有效(n.直接父->关系) &&
           n.直接父->创建事实代次 == h && !n.直接父->退出事实代次 &&
           n.从上游场景到本场景路径.size() == 1 &&
           n.从上游场景到本场景路径.front().关系 == n.直接父->关系;
  }
};
struct 世界树存在创建结果 final {
  世界树结果头 结果头;
  std::optional<世界树存在创建请求> 原请求;
  std::optional<世界树存在创建投影> 投影;
  bool 成功(const 世界树存在创建请求 &r) const noexcept {
    return r.版本 == 3 && 结果头.版本 == 3 &&
           (结果头.状态 == 世界树操作状态::已创建存在并纳入 ||
            结果头.状态 == 世界树操作状态::精确重复) &&
           结果头.阶段 == 世界树操作阶段::现实树最终确认 && 结果头.Gread &&
           结果头.首次发布H && r.G0 < *结果头.首次发布H &&
           *结果头.首次发布H <= 结果头.Gread && 原请求 && *原请求 == r &&
           投影 && 投影->存在 == 投影->位置.存在 && 有效(r.目标场景) &&
           有效(投影->存在) && 投影->存在 != r.目标场景 &&
           投影->位置.所在场景 == r.目标场景 &&
           场景直接包含事实完整(投影->位置.成员关系, 结果头.Gread) &&
           投影->位置.成员关系.种类 == 场景直接包含种类::存在成员 &&
           投影->位置.成员关系.父场景 == r.目标场景 &&
           投影->位置.成员关系.成员 == 投影->存在 &&
           投影->位置.成员关系.关系.生命周期.创建事实代次 ==
               *结果头.首次发布H &&
           !结果头.存在原因 && !结果头.场景原因 &&
           (结果头.绑定原因 == 绑定存在创建状态::已创建 ||
            结果头.绑定原因 == 绑定存在创建状态::精确重复);
  }
};

class 世界树应用服务 final {
public:
  世界树应用服务() = delete;
  世界树应用服务(const 世界树应用服务 &) = delete;
  世界树应用服务 &operator=(const 世界树应用服务 &) = delete;
  世界树应用服务(世界树应用服务 &&) = delete;
  世界树应用服务 &operator=(世界树应用服务 &&) = delete;

  世界树层级位置结果
  读取世界树层级位置(const 世界树层级位置读取请求 &r) const noexcept {
    世界树层级位置结果 out{
        {世界树应用合同版本, 世界树操作状态::入口拒绝,
         世界树操作阶段::现实树预读, r.G0},
        {}};
    if (r.版本 != 世界树应用合同版本 || !r.G0 || !有效(r.节点) ||
        r.预算.最大场景数量 < 1 || r.预算.最大场景数量 > 4096 ||
        r.预算.最大关系数量 < 1 || r.预算.最大关系数量 > 4096)
      return out;
    try {
      const auto global = 读取世界树根节点();
      if (!global.成功()) {
        out.结果头.状态 = 世界树操作状态::内部不一致;
        return out;
      }
      世界树层级位置 position{r.节点, global.投影->根, r.预期视角};
      if (r.节点 == global.投影->根) {
        if (r.预期视角 != 世界树节点视角::世界根场景) {
          out.结果头.状态 = 世界树操作状态::场景不在现实树;
          return out;
        }
        const auto role = scene_.读取当前场景角色位置({1, r.G0, r.节点});
        const 直接归属联合父读取请求 pq{1, r.G0, r.节点,
                                            r.预算.最大关系数量};
        const auto parent = joint_.读取当前联合父(pq);
        if (!role.成功({1, r.G0, r.节点}) || !role.角色 ||
            role.角色->位置 != 直接归属场景位置::场景树根 ||
            !parent.父读取成功(pq) || parent.父) {
          out.结果头.状态 = 世界树操作状态::内部不一致;
          return out;
        }
        out.结果头.状态 = 世界树操作状态::已读取存在位置;
        out.位置 = std::move(position);
        return out;
      }
      std::unordered_set<std::uint64_t> seen;
      auto cursor = r.节点;
      while (cursor != global.投影->根) {
        if (!seen.insert(cursor.值).second) {
          out.结果头.状态 = 世界树操作状态::形成场景环;
          return out;
        }
        if (position.上行路径.size() >= r.预算.最大关系数量) {
          out.结果头.状态 = 世界树操作状态::数量预算不足;
          return out;
        }
        const 直接归属联合父读取请求 pq{
            1, r.G0, cursor, r.预算.最大关系数量};
        const auto parent = joint_.读取当前联合父(pq);
        if (!parent.父读取成功(pq)) {
          out.结果头.状态 =
              parent.状态 == 直接归属联合只读状态::数量预算不足
                  ? 世界树操作状态::数量预算不足
                  : 世界树操作状态::内部不一致;
          return out;
        }
        if (!parent.父) {
          out.结果头.状态 =
              r.预期视角 == 世界树节点视角::存在
                  ? 世界树操作状态::存在不在现实树
                  : 世界树操作状态::场景不在现实树;
          return out;
        }
        if (!position.直接结构父)
          position.直接结构父 = parent.父;
        position.上行路径.push_back(*parent.父);
        cursor = parent.父->父;
      }
      if (r.预期视角 == 世界树节点视角::场景) {
        const auto role = scene_.读取当前场景角色位置({1, r.G0, r.节点});
        if (!role.成功({1, r.G0, r.节点}) || !role.角色 ||
            role.角色->位置 == 直接归属场景位置::场景树根) {
          out.结果头.状态 = 世界树操作状态::场景不在现实树;
          return out;
        }
        const 场景父语境读取请求 contextRequest{
            1, r.G0, r.节点, r.预算.最大关系数量};
        const auto context = scene_.读取当前父场景语境(contextRequest);
        if (!context.成功(contextRequest) || !context.投影 ||
            context.投影->结构父.关系 != position.直接结构父->关系 ||
            context.投影->结构父.父 != position.直接结构父->父 ||
            context.投影->结构父.成员 != position.直接结构父->成员 ||
            context.投影->结构父.来源 != position.直接结构父->来源) {
          out.结果头.状态 = 世界树操作状态::内部不一致;
          return out;
        }
        position.父场景语境 = context.投影;
      }
      out.结果头.状态 = 世界树操作状态::已读取存在位置;
      out.位置 = std::move(position);
    } catch (const std::bad_alloc &) {
      out.结果头.状态 = 世界树操作状态::资源失败;
    } catch (const std::length_error &) {
      out.结果头.状态 = 世界树操作状态::资源失败;
    } catch (...) {
      out.结果头.状态 = 世界树操作状态::内部不一致;
    }
    return out;
  }

  世界树根验证结果 验证现实世界根(const 世界树根验证请求 &r) const noexcept {
    世界树根验证结果 o{
        {2, 世界树操作状态::入口拒绝, 世界树操作阶段::现实树预读, r.G0}, {}};
    if (!有效读取(r.版本, r.G0, r.预算))
      return o;
    try {
      const 场景树当前读取请求 q{1, r.G0, 读取根(), r.预算.最大场景数量,
                                 r.预算.最大关系数量};
      auto t = scene_.读取当前场景树(q, joint_);
      if (!t.成功(q)) {
        o.结果头.状态 = 映射场景(t.结果头.状态);
        o.结果头.Gread = t.结果头.Gread;
        o.结果头.场景原因 = t.结果头.状态;
        return o;
      }
      const 直接归属联合父读取请求 p{1, r.G0, 读取根(), r.预算.最大关系数量};
      auto parent = joint_.读取当前联合父(p);
      if (!parent.父读取成功(p) || parent.父) {
        o.结果头.状态 = 世界树操作状态::内部不一致;
        return o;
      }
      o.结果头.状态 = 世界树操作状态::已验证现实根;
      o.树 = std::move(t.树);
    } catch (const std::bad_alloc &) {
      o.结果头.状态 = 世界树操作状态::资源失败;
    } catch (const std::length_error &) {
      o.结果头.状态 = 世界树操作状态::资源失败;
    } catch (...) {
      o.结果头.状态 = 世界树操作状态::内部不一致;
    }
    return o;
  }
  世界树存在位置结果
  读取现实世界存在位置(const 世界树存在位置读取请求 &r) const noexcept {
    世界树存在位置结果 o{
        {2, 世界树操作状态::入口拒绝, 世界树操作阶段::现实树预读, r.G0}, {}};
    if (r.版本 != 2 || !有效(r.存在) || !有效读取(r.版本, r.G0, r.预算))
      return o;
    try {
      auto root = 验证现实世界根({2, r.G0, r.预算});
      if (!root.成功({2, r.G0, r.预算})) {
        o.结果头 = root.结果头;
        return o;
      }
      for (const auto &s : root.树->场景组)
        for (const auto &m : s.直接存在成员组)
          if (m.成员 == r.存在) {
            if (o.位置) {
              o.位置.reset();
              o.结果头.状态 = 世界树操作状态::成员多重位置;
              return o;
            }
            o.位置 = 世界树存在位置{r.存在, s.场景角色.场景, m};
          }
      o.结果头.状态 = o.位置 ? 世界树操作状态::已读取存在位置
                             : 世界树操作状态::存在不在现实树;
    } catch (const std::bad_alloc &) {
      o.结果头.状态 = 世界树操作状态::资源失败;
    } catch (const std::length_error &) {
      o.结果头.状态 = 世界树操作状态::资源失败;
    } catch (...) {
      o.结果头.状态 = 世界树操作状态::内部不一致;
    }
    return o;
  }
  世界树移动结果 移动现实世界存在(const 世界树成员移动请求 &r) noexcept {
    return 移动(r, false);
  }
  世界树移动结果 移动现实世界场景(const 世界树成员移动请求 &r) noexcept {
    return 移动(r, true);
  }
  世界树场景创建结果
  创建场景并纳入现实世界(const 世界树场景创建请求 &r) noexcept {
    return 创建场景(r);
  }
  世界树存在创建结果
  创建存在并纳入现实世界(const 世界树存在创建请求 &r) noexcept {
    return 创建存在(r);
  }

private:
  struct 已验证世界根令牌 final {};
  世界树应用服务(场景类数据服务 &s, 存在类数据服务 &e, 已验证世界根令牌)
      : scene_(s), existence_(e), joint_(e, s), binding_(e, s) {
    if (!scene_.使用存在提供者(e) || !读取世界树根节点().成功())
      throw std::invalid_argument("invalid world root");
  }
  friend 世界树应用服务建立结果
  建立世界树应用服务(场景类数据服务 &, 存在类数据服务 &,
                       const 世界树根验证请求 &) noexcept;
  static 稳定编码 读取根() noexcept {
    const auto r = 读取世界树根节点();
    return r.成功() ? r.投影->根 : 稳定编码{};
  }
  static bool 有效读取(std::uint32_t v, std::uint64_t g,
                       世界树读取预算 b) noexcept {
    return v == 2 && g && b.最大场景数量 >= 1 && b.最大场景数量 <= 4096 &&
           b.最大关系数量 >= 1 && b.最大关系数量 <= 4096;
  }

  static bool 树含场景(const 场景树当前事实 &t, 稳定编码 c) noexcept {
    return std::any_of(t.场景组.begin(), t.场景组.end(),
                       [&](const auto &x) { return x.场景角色.场景 == c; });
  }
  static bool 相同(const 场景直接子场景启用请求 &a,
                   const 场景直接子场景启用请求 &b) noexcept {
    return a.版本 == b.版本 && a.G0 == b.G0 && a.幂等身份 == b.幂等身份 &&
           a.对象存在 == b.对象存在 && a.父场景 == b.父场景 &&
           a.最大关系数量 == b.最大关系数量 && a.最大祖先数量 == b.最大祖先数量;
  }
  static bool 相同(const 场景直接包含写请求 &a,
                   const 场景直接包含写请求 &b) noexcept {
    return a.版本 == b.版本 && a.G0 == b.G0 && a.幂等身份 == b.幂等身份 &&
           a.父场景 == b.父场景 && a.成员 == b.成员 &&
           a.最大关系数量 == b.最大关系数量 && a.最大场景数量 == b.最大场景数量;
  }
  static 世界树操作状态 映射场景(场景直接包含状态 s) noexcept {
    switch (s) {
    case 场景直接包含状态::事实代次漂移:
      return 世界树操作状态::事实代次漂移;
    case 场景直接包含状态::幂等冲突:
      return 世界树操作状态::幂等冲突;
    case 场景直接包含状态::数量预算不足:
      return 世界树操作状态::数量预算不足;
    case 场景直接包含状态::历史材料已清理:
      return 世界树操作状态::历史材料已清理;
    case 场景直接包含状态::资源失败:
      return 世界树操作状态::资源失败;
    case 场景直接包含状态::形成场景环:
      return 世界树操作状态::形成场景环;
    case 场景直接包含状态::引用冲突:
      return 世界树操作状态::引用冲突;
    case 场景直接包含状态::原父不匹配:
      return 世界树操作状态::原位置不匹配;
    case 场景直接包含状态::目标父相同:
      return 世界树操作状态::目标位置相同;
    case 场景直接包含状态::场景未找到:
    case 场景直接包含状态::场景已退出:
      return 世界树操作状态::场景不在现实树;
    default:
      return 世界树操作状态::内部不一致;
    }
  }

  世界树移动结果 移动(const 世界树成员移动请求 &r, bool sceneMove) noexcept {
    世界树移动结果 o{
        {2, 世界树操作状态::入口拒绝, 世界树操作阶段::现实树预读, r.G0}, r};
    if (!有效读取(r.版本, r.G0, r.预算) || r.幂等身份.值 <= 1 ||
        !有效(r.原场景) || !有效(r.目标场景) || !有效(r.成员))
      return o;
    if (r.原场景 == r.目标场景) {
      o.结果头.状态 = 世界树操作状态::目标位置相同;
      return o;
    }
    if (sceneMove && r.成员 == 读取根()) {
      o.结果头.状态 = 世界树操作状态::现实根不可移动;
      return o;
    }
    try {
      auto tree = 验证现实世界根({2, r.G0, r.预算});
      const bool treeRead = tree.成功({2, r.G0, r.预算});
      if (!treeRead && tree.结果头.状态 != 世界树操作状态::事实代次漂移) {
        o.结果头 = tree.结果头;
        return o;
      }
      if (treeRead &&
          (!树含场景(*tree.树, r.原场景) || !树含场景(*tree.树, r.目标场景))) {
        o.结果头 = tree.结果头;
        o.结果头.状态 = 世界树操作状态::场景不在现实树;
        return o;
      }
      场景直接包含迁移请求 q{
          1,          r.G0,   r.幂等身份,          r.原场景,
          r.目标场景, r.成员, r.预算.最大关系数量, r.预算.最大场景数量};
      auto moved = sceneMove ? scene_.迁移直接子场景(q, joint_)
                             : scene_.迁移直接存在成员(q, joint_);
      o.结果头.Gread = moved.结果头.Gread;
      if (!moved.成功(q)) {
        o.结果头.状态 = moved.结果头.状态 == 场景直接包含状态::已可能发布
                            ? 世界树操作状态::结果未知
                            : 映射场景(moved.结果头.状态);
        o.结果头.场景原因 = moved.结果头.状态;
        if (moved.结果头.状态 == 场景直接包含状态::已可能发布) {
          o.进度 = 世界树移动进度{q, moved.结果头.首次发布H};
          o.结果头.首次发布H = moved.结果头.首次发布H;
        }
        return o;
      }
      o.进度 = 世界树移动进度{q, moved.结果头.首次发布H};
      bool confirmed = false;
      bool readable = false;
      bool successorObserved = false;
      if (sceneMove) {
        const 场景树当前读取请求 read{1, moved.结果头.Gread, 读取根(),
                                      r.预算.最大场景数量, r.预算.最大关系数量};
        const auto treeAfter = scene_.读取当前场景树(read, joint_);
        if (treeAfter.成功(read)) {
          readable = true;
          const auto it = std::find_if(
              treeAfter.树->场景组.begin(), treeAfter.树->场景组.end(),
              [&](const auto &x) { return x.场景角色.场景 == r.成员; });
          if (it != treeAfter.树->场景组.end() && it->直接父) {
            confirmed = it->直接父->父 == r.目标场景;
            successorObserved = it->直接父->父 != r.目标场景;
          }
        }
      } else {
        const auto position =
            读取现实世界存在位置({2, moved.结果头.Gread, r.成员, r.预算});
        readable = position.成功({2, moved.结果头.Gread, r.成员, r.预算});
        if (readable) {
          confirmed = position.位置->所在场景 == r.目标场景;
          successorObserved = position.位置->所在场景 != r.目标场景;
        }
      }
      if (!confirmed) {
        o.结果头.状态 = moved.结果头.状态 == 场景直接包含状态::精确重复 &&
                                readable && successorObserved
                             ? 世界树操作状态::既有操作已被后继事实覆盖
                             : 世界树操作状态::已发布待复核;
        o.结果头.阶段 = 世界树操作阶段::现实树最终确认;
        o.结果头.首次发布H = moved.结果头.首次发布H;
        return o;
      }
      o.结果头.阶段 = 世界树操作阶段::现实树最终确认;
      o.结果头.首次发布H = moved.结果头.首次发布H;
      o.投影 =
          世界树移动投影{r.成员,
                         r.原场景,
                         r.目标场景,
                         moved.已退出原包含->关系.编码,
                         moved.已建立新包含->关系.编码,
                         moved.结果头.首次发布H.value_or(moved.结果头.Gread)};
      o.结果头.状态 = moved.结果头.状态 == 场景直接包含状态::精确重复
                          ? 世界树操作状态::精确重复
                          : 世界树操作状态::已移动;
      o.进度.reset();
    } catch (const std::bad_alloc &) {
      o.结果头.状态 = 世界树操作状态::资源失败;
    } catch (const std::length_error &) {
      o.结果头.状态 = 世界树操作状态::资源失败;
    } catch (...) {
      o.结果头.状态 = 世界树操作状态::内部不一致;
    }
    return o;
  }
  static 世界树操作状态 映射绑定状态(绑定存在创建状态 x, bool scene) noexcept {
    using S = 绑定存在创建状态;
    using W = 世界树操作状态;
    switch (x) {
    case S::已创建:
      return scene ? W::已创建场景并纳入 : W::已创建存在并纳入;
    case S::精确重复:
      return W::精确重复;
    case S::绑定不在现实树:
      return W::场景不在现实树;
    case S::包含冲突:
      return W::成员多重位置;
    case S::成环:
      return W::形成场景环;
    case S::事实代次漂移:
      return W::事实代次漂移;
    case S::幂等冲突:
      return W::幂等冲突;
    case S::数量预算不足:
      return W::数量预算不足;
    case S::历史材料已清理:
      return W::历史材料已清理;
    case S::资源失败:
      return W::资源失败;
    case S::内部不一致:
      return W::内部不一致;
    case S::已可能发布:
      return W::已发布待复核;
    case S::既有操作已被后继事实覆盖:
      return W::既有操作已被后继事实覆盖;
    default:
      return W::入口拒绝;
    }
  }
  template <class R>
  绑定存在创建请求 绑定请求(const R &r, 稳定编码 parent,
                            存在初始绑定种类 kind) const {
    return {r.版本 == 3 ? 1u : 0u,
            r.G0,
            {kind, parent},
            读取根(),
            存在场景绑定创建键{r.组合幂等身份, r.存在幂等身份, r.场景幂等身份},
            {r.预算.最大关系数量, r.预算.最大场景数量, r.预算.最大祖先数量}};
  }
  世界树场景创建结果 创建场景(const 世界树场景创建请求 &r) noexcept {
    世界树场景创建结果 o{};
    o.结果头.版本 = 3;
    try {
      o.原请求 = r;
      const auto q = 绑定请求(r, r.父场景, 存在初始绑定种类::直接子场景);
      const auto a = binding_.创建绑定存在(q);
      o.结果头.状态 = 映射绑定状态(a.状态, true);
      o.结果头.绑定原因 = a.状态;
      o.结果头.Gread = a.Gread;
      o.结果头.首次发布H = a.首次发布H;
      o.结果头.阶段 = 世界树操作阶段::存在创建;
      if (!a.成功(q))
        return o;
      o.结果头.阶段 = 世界树操作阶段::现实树最终确认;
      const 世界树根验证请求 read{
          2, a.Gread, {r.预算.最大场景数量, r.预算.最大关系数量}};
      const auto tree = 验证现实世界根(read);
      if (!tree.成功(read)) {
        o.结果头.状态 = 世界树操作状态::已发布待复核;
        return o;
      }
      const auto it = std::find_if(
          tree.树->场景组.begin(), tree.树->场景组.end(), [&](const auto &n) {
            return n.场景角色.场景 == a.事实->新存在 && n.直接父 &&
                   n.直接父->父 == r.父场景;
          });
      if (it == tree.树->场景组.end()) {
        o.结果头.状态 = 世界树操作状态::已发布待复核;
        return o;
      }
      o.投影 = 世界树场景创建投影{a.事实->新存在, r.父场景, *it};
      return o;
    } catch (const std::bad_alloc &) {
      o.投影.reset();
      o.结果头.状态 = o.结果头.首次发布H ? 世界树操作状态::已发布待复核
                                         : 世界树操作状态::资源失败;
      return o;
    } catch (const std::length_error &) {
      o.投影.reset();
      o.结果头.状态 = o.结果头.首次发布H ? 世界树操作状态::已发布待复核
                                         : 世界树操作状态::资源失败;
      return o;
    } catch (...) {
      o.投影.reset();
      o.结果头.状态 = o.结果头.首次发布H ? 世界树操作状态::已发布待复核
                                         : 世界树操作状态::内部不一致;
      return o;
    }
  }
  世界树存在创建结果 创建存在(const 世界树存在创建请求 &r) noexcept {
    世界树存在创建结果 o{};
    o.结果头.版本 = 3;
    try {
      o.原请求 = r;
      const auto q = 绑定请求(r, r.目标场景, 存在初始绑定种类::场景成员);
      const auto a = binding_.创建绑定存在(q);
      o.结果头.状态 = 映射绑定状态(a.状态, false);
      o.结果头.绑定原因 = a.状态;
      o.结果头.Gread = a.Gread;
      o.结果头.首次发布H = a.首次发布H;
      o.结果头.阶段 = 世界树操作阶段::存在创建;
      if (!a.成功(q))
        return o;
      o.结果头.阶段 = 世界树操作阶段::现实树最终确认;
      const 世界树存在位置读取请求 read{
          2,
          a.Gread,
          a.事实->新存在,
          {r.预算.最大场景数量, r.预算.最大关系数量}};
      const auto pos = 读取现实世界存在位置(read);
      if (!pos.成功(read) || pos.位置->所在场景 != r.目标场景) {
        o.结果头.状态 = 世界树操作状态::已发布待复核;
        return o;
      }
      o.投影 = 世界树存在创建投影{a.事实->新存在, *pos.位置};
      return o;
    } catch (const std::bad_alloc &) {
      o.投影.reset();
      o.结果头.状态 = o.结果头.首次发布H ? 世界树操作状态::已发布待复核
                                         : 世界树操作状态::资源失败;
      return o;
    } catch (const std::length_error &) {
      o.投影.reset();
      o.结果头.状态 = o.结果头.首次发布H ? 世界树操作状态::已发布待复核
                                         : 世界树操作状态::资源失败;
      return o;
    } catch (...) {
      o.投影.reset();
      o.结果头.状态 = o.结果头.首次发布H ? 世界树操作状态::已发布待复核
                                         : 世界树操作状态::内部不一致;
      return o;
    }
  }
  场景类数据服务 &scene_;
  存在类数据服务 &existence_;
  直接归属联合只读组合器 joint_;
  绑定存在数据服务 binding_;
};

namespace 世界树应用内部 {
struct 全局根绑定 final {
  const L1事实基座服务 *底座 = nullptr;
  世界树全局根投影 投影;
};
inline std::mutex 根锁;
inline std::optional<全局根绑定> 根;
}

inline 世界树全局根读取结果 读取世界树根节点() noexcept {
  try {
    std::lock_guard lock(世界树应用内部::根锁);
    if (!世界树应用内部::根)
      return {};
    return {世界树全局根读取状态::已读取, 世界树应用内部::根->投影};
  } catch (...) {
    return {};
  }
}

struct 世界树应用服务建立结果 final {
  世界树操作状态 状态 = 世界树操作状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<世界树全局根投影> 投影;
  std::unique_ptr<世界树应用服务> 服务;
  bool 成功() const noexcept {
    return 状态 == 世界树操作状态::已验证现实根 && 投影 && 服务 &&
           有效(投影->根) && 投影->首次H &&
           投影->首次H <= 投影->最近验证G && Gread == 投影->最近验证G;
  }
};

inline 世界树应用服务建立结果
建立世界树应用服务(场景类数据服务 &scene, 存在类数据服务 &existence,
                     const 世界树根验证请求 &r) noexcept {
  世界树应用服务建立结果 out;
  out.Gread = r.G0;
  if (r.版本 != 2 || !r.G0 || r.预算.最大场景数量 < 1 ||
      r.预算.最大关系数量 < 1 || !scene.使用存在提供者(existence))
    return out;
  try {
    const auto roots = scene.读取当前世界树根组(
        {世界树根合同版本, r.G0, r.预算.最大关系数量});
    if (!roots.成功({世界树根合同版本, r.G0, r.预算.最大关系数量}) ||
        roots.根组.size() != 1) {
      out.状态 = roots.根组.size() > 1 ? 世界树操作状态::内部不一致
                                      : 世界树操作状态::场景不在现实树;
      return out;
    }
    const auto root = roots.根组.front();
    直接归属联合只读组合器 joint(existence, scene);
    const 场景树当前读取请求 treeRequest{
        1, r.G0, root, r.预算.最大场景数量, r.预算.最大关系数量};
    const auto tree = scene.读取当前场景树(treeRequest, joint);
    const 直接归属联合父读取请求 parentRequest{
        1, r.G0, root, r.预算.最大关系数量};
    const auto parent = joint.读取当前联合父(parentRequest);
    if (!tree.成功(treeRequest) || !tree.树 || tree.树->场景组.empty() ||
        !parent.父读取成功(parentRequest) || parent.父) {
      out.状态 = 世界树操作状态::内部不一致;
      return out;
    }
    const auto rootPosition = std::find_if(
        tree.树->场景组.begin(), tree.树->场景组.end(),
        [&](const auto &x) { return x.场景角色.场景 == root; });
    if (rootPosition == tree.树->场景组.end()) {
      out.状态 = 世界树操作状态::内部不一致;
      return out;
    }
    const auto &rootNode = *rootPosition;
    const auto firstH = rootNode.场景角色.对象存在来源.节点生命周期.创建事实代次;
    世界树全局根投影 projection{root, firstH, r.G0};
    {
      std::lock_guard lock(世界树应用内部::根锁);
      auto &global = 世界树应用内部::根;
      if (global &&
          (global->底座 != &existence.世界树根底座() ||
           global->投影.根 != root || global->投影.首次H != firstH)) {
        out.状态 = 世界树操作状态::内部不一致;
        return out;
      }
      if (!global)
        global = 世界树应用内部::全局根绑定{
            &existence.世界树根底座(), projection};
      else
        global->投影.最近验证G = r.G0;
      projection = global->投影;
    }
    out.服务.reset(new 世界树应用服务(
        scene, existence, 世界树应用服务::已验证世界根令牌{}));
    out.投影 = projection;
    out.状态 = 世界树操作状态::已验证现实根;
  } catch (const std::bad_alloc &) {
    out.状态 = 世界树操作状态::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 世界树操作状态::资源失败;
  } catch (...) {
    out.状态 = 世界树操作状态::内部不一致;
  }
  return out;
}

static_assert(世界树应用合同版本 == 3);
} // namespace 海中鱼巣
