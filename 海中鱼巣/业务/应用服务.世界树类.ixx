module;
#include <algorithm>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
export module 海中鱼巣.业务.应用服务.世界树类;
export import 海中鱼巣.领域.数据服务.存在类;
export import 海中鱼巣.领域.数据服务.场景类;
export namespace 海中鱼巣 {
inline constexpr std::uint32_t 世界树应用合同版本 = 2;
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
struct 世界树场景创建请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t 观察G0 = 0, 存在创建期望G0 = 0;
  L1所有者范围写入幂等身份 存在幂等身份{}, 场景幂等身份{};
  稳定编码 父场景{};
  std::optional<场景直接子场景启用请求> 已固定第二阶段请求;
  世界树读取预算 预算;
  friend bool operator==(const 世界树场景创建请求 &a,
                         const 世界树场景创建请求 &b) noexcept {
    return a.版本 == b.版本 && a.观察G0 == b.观察G0 &&
           a.存在创建期望G0 == b.存在创建期望G0 &&
           a.存在幂等身份 == b.存在幂等身份 && a.场景幂等身份 == b.场景幂等身份 &&
           a.父场景 == b.父场景 && a.已固定第二阶段请求 == b.已固定第二阶段请求 &&
           a.预算 == b.预算;
  }
};
struct 世界树存在创建请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t 观察G0 = 0, 存在创建期望G0 = 0;
  L1所有者范围写入幂等身份 存在幂等身份{}, 接纳幂等身份{};
  稳定编码 目标场景{};
  std::optional<场景直接包含写请求> 已固定第二阶段请求;
  世界树读取预算 预算;
  friend bool operator==(const 世界树存在创建请求 &a,
                         const 世界树存在创建请求 &b) noexcept {
    return a.版本 == b.版本 && a.观察G0 == b.观察G0 &&
           a.存在创建期望G0 == b.存在创建期望G0 &&
           a.存在幂等身份 == b.存在幂等身份 && a.接纳幂等身份 == b.接纳幂等身份 &&
           a.目标场景 == b.目标场景 && a.已固定第二阶段请求 == b.已固定第二阶段请求 &&
           a.预算 == b.预算;
  }
};
struct 世界树结果头 final {
  std::uint32_t 版本 = 2;
  世界树操作状态 状态 = 世界树操作状态::入口拒绝;
  世界树操作阶段 阶段 = 世界树操作阶段::无;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布H;
  std::optional<存在类数据状态> 存在原因;
  std::optional<场景直接包含状态> 场景原因;
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
struct 世界树场景创建进度 final {
  稳定编码 E{};
  std::uint64_t 存在首次发布H = 0;
  std::optional<场景直接子场景启用请求> 第二阶段请求;
};
struct 世界树场景创建投影 final {
  稳定编码 E{}, 父场景{};
  场景树节点当前事实 场景;
};
struct 世界树存在创建进度 final {
  稳定编码 E{};
  std::uint64_t 存在首次发布H = 0;
  std::optional<场景直接包含写请求> 第二阶段请求;
};
struct 世界树存在创建投影 final {
  稳定编码 存在{};
  世界树存在位置 位置;
};
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
  std::optional<世界树场景创建请求> 原请求, 续跑请求;
  std::optional<世界树场景创建进度> 进度;
  std::optional<世界树场景创建投影> 投影;
  bool 成功(const 世界树场景创建请求 &r) const noexcept {
    return r.版本 == 2 && 结果头.版本 == 2 &&
           (结果头.状态 == 世界树操作状态::已创建场景并纳入 ||
            结果头.状态 == 世界树操作状态::精确重复) &&
           结果头.阶段 == 世界树操作阶段::现实树最终确认 && 结果头.Gread &&
           结果头.首次发布H && *结果头.首次发布H <= 结果头.Gread && 原请求 &&
           *原请求 == r && 投影 && 投影->父场景 == r.父场景 &&
           投影->E == 投影->场景.场景角色.场景 &&
           !续跑请求 && !进度 && !结果头.存在原因 && !结果头.场景原因;
  }
};
struct 世界树存在创建结果 final {
  世界树结果头 结果头;
  std::optional<世界树存在创建请求> 原请求, 续跑请求;
  std::optional<世界树存在创建进度> 进度;
  std::optional<世界树存在创建投影> 投影;
  bool 成功(const 世界树存在创建请求 &r) const noexcept {
    return r.版本 == 2 && 结果头.版本 == 2 &&
           (结果头.状态 == 世界树操作状态::已创建存在并纳入 ||
            结果头.状态 == 世界树操作状态::精确重复) &&
           结果头.阶段 == 世界树操作阶段::现实树最终确认 && 结果头.Gread &&
           结果头.首次发布H && *结果头.首次发布H <= 结果头.Gread && 原请求 &&
           *原请求 == r && 投影 && 投影->存在 == 投影->位置.存在 &&
           投影->位置.所在场景 == r.目标场景 &&
           !续跑请求 && !进度 && !结果头.存在原因 && !结果头.场景原因;
  }
};

class 世界树应用服务 final {
public:
  世界树应用服务() = delete;
  世界树应用服务(const 世界树应用服务 &) = delete;
  世界树应用服务 &operator=(const 世界树应用服务 &) = delete;
  世界树应用服务(世界树应用服务 &&) = delete;
  世界树应用服务 &operator=(世界树应用服务 &&) = delete;
  世界树应用服务(场景类数据服务 &s, 存在类数据服务 &e, 稳定编码 root)
      : scene_(s), existence_(e), root_(root), joint_(e, s) {
    if (!有效(root_) || !scene_.使用存在提供者(e))
      throw std::invalid_argument("invalid world root");
  }
  世界树根验证结果 验证现实世界根(const 世界树根验证请求 &r) const noexcept {
    世界树根验证结果 o{
        {2, 世界树操作状态::入口拒绝, 世界树操作阶段::现实树预读, r.G0}, {}};
    if (!有效读取(r.版本, r.G0, r.预算))
      return o;
    try {
      const 场景树当前读取请求 q{1, r.G0, root_, r.预算.最大场景数量,
                                 r.预算.最大关系数量};
      auto t = scene_.读取当前场景树(q, joint_);
      if (!t.成功(q)) {
        o.结果头.状态 = 映射场景(t.结果头.状态);
        o.结果头.Gread = t.结果头.Gread;
        o.结果头.场景原因 = t.结果头.状态;
        return o;
      }
      const 直接归属联合父读取请求 p{1, r.G0, root_, r.预算.最大关系数量};
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
  static bool 有效读取(std::uint32_t v, std::uint64_t g,
                       世界树读取预算 b) noexcept {
    return v == 2 && g && b.最大场景数量 >= 1 && b.最大场景数量 <= 4096 &&
           b.最大关系数量 >= 1 && b.最大关系数量 <= 4096;
  }
  static bool 有效创建(std::uint32_t v, std::uint64_t observe,
                       std::uint64_t create, L1所有者范围写入幂等身份 a,
                       L1所有者范围写入幂等身份 b, 稳定编码 target,
                       世界树读取预算 budget) noexcept {
    return 有效读取(v, observe, budget) && create && create != UINT64_MAX &&
           a.值 > 1 && b.值 > 1 && a != b && 有效(target);
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
  static 世界树操作状态 映射存在(存在类数据状态 s) noexcept {
    switch (s) {
    case 存在类数据状态::事实代次漂移:
      return 世界树操作状态::事实代次漂移;
    case 存在类数据状态::幂等冲突:
      return 世界树操作状态::幂等冲突;
    case 存在类数据状态::数量预算不足:
      return 世界树操作状态::数量预算不足;
    case 存在类数据状态::历史材料已清理:
      return 世界树操作状态::历史材料已清理;
    case 存在类数据状态::资源失败:
      return 世界树操作状态::资源失败;
    case 存在类数据状态::已可能发布:
      return 世界树操作状态::结果未知;
    case 存在类数据状态::引用冲突:
      return 世界树操作状态::引用冲突;
    case 存在类数据状态::未找到:
    case 存在类数据状态::目标已退出:
      return 世界树操作状态::存在不在现实树;
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
    if (sceneMove && r.成员 == root_) {
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
        const 场景树当前读取请求 read{1, moved.结果头.Gread, root_,
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
  世界树场景创建结果 创建场景(const 世界树场景创建请求 &r) noexcept {
    世界树场景创建结果 o{
        {2, 世界树操作状态::入口拒绝, 世界树操作阶段::现实树预读, r.观察G0}, r};
    if (!有效创建(r.版本, r.观察G0, r.存在创建期望G0, r.存在幂等身份,
                  r.场景幂等身份, r.父场景, r.预算))
      return o;
    if (r.已固定第二阶段请求) {
      const auto &fixed = *r.已固定第二阶段请求;
      if (fixed.版本 != 1 || !fixed.G0 || fixed.幂等身份 != r.场景幂等身份 ||
          fixed.父场景 != r.父场景 || !有效(fixed.对象存在) ||
          fixed.最大关系数量 != r.预算.最大关系数量 ||
          fixed.最大祖先数量 != r.预算.最大场景数量)
        return o;
    }
    try {
      auto tree = 验证现实世界根({2, r.观察G0, r.预算});
      const bool treeRead = tree.成功({2, r.观察G0, r.预算});
      if (!treeRead && tree.结果头.状态 != 世界树操作状态::事实代次漂移) {
        o.结果头 = tree.结果头;
        return o;
      }
      if (treeRead && !树含场景(*tree.树, r.父场景)) {
        o.结果头 = tree.结果头;
        o.结果头.状态 = 世界树操作状态::场景不在现实树;
        return o;
      }
      o.结果头.阶段 = 世界树操作阶段::存在创建;
      auto made = existence_.新增存在(
          {存在类数据合同版本, r.存在创建期望G0, r.存在幂等身份});
      if (!made.成功() || !made.存在结点) {
        o.结果头.状态 = 映射存在(made.状态);
        o.结果头.Gread = made.事实代次;
        o.结果头.存在原因 = made.状态;
        if (made.状态 == 存在类数据状态::已可能发布) {
          o.结果头.阶段 = 世界树操作阶段::存在创建;
          o.原请求 = r;
        }
        return o;
      }
      auto E = made.存在结点->结点;
      const 场景直接子场景启用请求 derived{
          1,        made.事实代次,       r.场景幂等身份,     E,
          r.父场景, r.预算.最大关系数量, r.预算.最大场景数量};
      if (r.已固定第二阶段请求 && r.已固定第二阶段请求->对象存在 != E) {
        o.结果头.状态 = 世界树操作状态::部分已发布;
        o.结果头.Gread = made.事实代次;
        o.进度 = 世界树场景创建进度{E, made.存在结点->创建事实代次, derived};
        return o;
      }
      const auto second = r.已固定第二阶段请求.value_or(derived);
      o.进度 = 世界树场景创建进度{E, made.存在结点->创建事实代次, second};
      o.结果头.阶段 = 世界树操作阶段::场景包含发布;
      auto accepted = scene_.启用并接纳直接子场景(second, joint_);
      o.结果头.Gread = accepted.结果头.Gread;
      if (!accepted.子场景纳入成功(second)) {
        o.结果头.场景原因 = accepted.结果头.状态;
        o.结果头.状态 = accepted.结果头.状态 == 场景直接包含状态::已可能发布
                            ? 世界树操作状态::结果未知
                            : 世界树操作状态::部分已发布;
        if (accepted.结果头.状态 == 场景直接包含状态::事实代次漂移) {
          const auto fresh = 验证现实世界根({2, accepted.结果头.Gread, r.预算});
          if (fresh.成功({2, accepted.结果头.Gread, r.预算}) &&
              树含场景(*fresh.树, r.父场景)) {
            auto retrySecond = second;
            retrySecond.G0 = accepted.结果头.Gread;
            auto retry = r;
            retry.观察G0 = accepted.结果头.Gread;
            retry.已固定第二阶段请求 = retrySecond;
            o.续跑请求 = std::move(retry);
          }
        } else if (accepted.结果头.状态 == 场景直接包含状态::已可能发布) {
          o.结果头.首次发布H = accepted.结果头.首次发布H;
        }
        return o;
      }
      const 场景树当前读取请求 finalRequest{1, accepted.结果头.Gread, root_,
                                            r.预算.最大场景数量,
                                            r.预算.最大关系数量};
      const auto finalTree = scene_.读取当前场景树(finalRequest, joint_);
      if (!finalTree.成功(finalRequest)) {
        o.结果头.状态 = 世界树操作状态::已发布待复核;
        o.结果头.阶段 = 世界树操作阶段::现实树最终确认;
        o.结果头.首次发布H = accepted.结果头.首次发布H;
        return o;
      }
      const auto matches = std::count_if(
          finalTree.树->场景组.begin(), finalTree.树->场景组.end(),
          [&](const auto &x) {
            return x.场景角色.场景 == E && x.直接父 && x.直接父->父 == r.父场景;
          });
      const auto sceneIt = std::find_if(finalTree.树->场景组.begin(),
                                        finalTree.树->场景组.end(),
                                        [&](const auto &x) {
                                          return x.场景角色.场景 == E;
                                        });
      if (matches != 1 || sceneIt == finalTree.树->场景组.end()) {
        const bool successorObserved =
            sceneIt != finalTree.树->场景组.end() && sceneIt->直接父 &&
            sceneIt->直接父->父 != r.父场景;
        o.结果头.状态 = accepted.结果头.状态 == 场景直接包含状态::精确重复 &&
                                successorObserved
                            ? 世界树操作状态::既有操作已被后继事实覆盖
                            : 世界树操作状态::已发布待复核;
        o.结果头.阶段 = 世界树操作阶段::现实树最终确认;
        o.结果头.首次发布H = accepted.结果头.首次发布H;
        return o;
      }
      o.结果头 = {2,
                  accepted.结果头.状态 == 场景直接包含状态::精确重复
                      ? 世界树操作状态::精确重复
                      : 世界树操作状态::已创建场景并纳入,
                  世界树操作阶段::现实树最终确认, accepted.结果头.Gread,
                  accepted.结果头.首次发布H};
      o.投影 = 世界树场景创建投影{E, r.父场景, *sceneIt};
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
  世界树存在创建结果 创建存在(const 世界树存在创建请求 &r) noexcept {
    世界树存在创建结果 o{
        {2, 世界树操作状态::入口拒绝, 世界树操作阶段::现实树预读, r.观察G0}, r};
    if (!有效创建(r.版本, r.观察G0, r.存在创建期望G0, r.存在幂等身份,
                  r.接纳幂等身份, r.目标场景, r.预算))
      return o;
    if (r.已固定第二阶段请求) {
      const auto &fixed = *r.已固定第二阶段请求;
      if (fixed.版本 != 1 || !fixed.G0 || fixed.幂等身份 != r.接纳幂等身份 ||
          fixed.父场景 != r.目标场景 || !有效(fixed.成员) ||
          fixed.最大关系数量 != r.预算.最大关系数量 ||
          fixed.最大场景数量 != r.预算.最大场景数量)
        return o;
    }
    try {
      auto tree = 验证现实世界根({2, r.观察G0, r.预算});
      const bool treeRead = tree.成功({2, r.观察G0, r.预算});
      if (!treeRead && tree.结果头.状态 != 世界树操作状态::事实代次漂移) {
        o.结果头 = tree.结果头;
        return o;
      }
      if (treeRead && !树含场景(*tree.树, r.目标场景)) {
        o.结果头 = tree.结果头;
        o.结果头.状态 = 世界树操作状态::场景不在现实树;
        return o;
      }
      o.结果头.阶段 = 世界树操作阶段::存在创建;
      auto made = existence_.新增存在(
          {存在类数据合同版本, r.存在创建期望G0, r.存在幂等身份});
      if (!made.成功() || !made.存在结点) {
        o.结果头.状态 = 映射存在(made.状态);
        o.结果头.Gread = made.事实代次;
        o.结果头.存在原因 = made.状态;
        if (made.状态 == 存在类数据状态::已可能发布) {
          o.结果头.阶段 = 世界树操作阶段::存在创建;
          o.原请求 = r;
        }
        return o;
      }
      auto E = made.存在结点->结点;
      const 场景直接包含写请求 derived{
          1, made.事实代次,       r.接纳幂等身份,     r.目标场景,
          E, r.预算.最大关系数量, r.预算.最大场景数量};
      if (r.已固定第二阶段请求 && r.已固定第二阶段请求->成员 != E) {
        o.结果头.状态 = 世界树操作状态::部分已发布;
        o.结果头.Gread = made.事实代次;
        o.进度 = 世界树存在创建进度{E, made.存在结点->创建事实代次, derived};
        return o;
      }
      const auto second = r.已固定第二阶段请求.value_or(derived);
      o.进度 = 世界树存在创建进度{E, made.存在结点->创建事实代次, second};
      o.结果头.阶段 = 世界树操作阶段::场景包含发布;
      auto accepted = scene_.新增直接存在成员(second, joint_);
      o.结果头.Gread = accepted.结果头.Gread;
      if (!accepted.成功()) {
        o.结果头.场景原因 = accepted.结果头.状态;
        o.结果头.状态 = accepted.结果头.状态 == 场景直接包含状态::已可能发布
                            ? 世界树操作状态::结果未知
                            : 世界树操作状态::部分已发布;
        if (accepted.结果头.状态 == 场景直接包含状态::事实代次漂移) {
          const auto fresh = 验证现实世界根({2, accepted.结果头.Gread, r.预算});
          if (fresh.成功({2, accepted.结果头.Gread, r.预算}) &&
              树含场景(*fresh.树, r.目标场景)) {
            auto retrySecond = second;
            retrySecond.G0 = accepted.结果头.Gread;
            auto retry = r;
            retry.观察G0 = accepted.结果头.Gread;
            retry.已固定第二阶段请求 = retrySecond;
            o.续跑请求 = std::move(retry);
          }
        } else if (accepted.结果头.状态 == 场景直接包含状态::已可能发布) {
          o.结果头.首次发布H = accepted.结果头.首次发布H;
        }
        return o;
      }
      const auto finalPosition = 读取现实世界存在位置(
          {2, accepted.结果头.Gread, E, r.预算});
      if (!finalPosition.成功({2, accepted.结果头.Gread, E, r.预算}) ||
          finalPosition.位置->所在场景 != r.目标场景) {
        o.结果头.状态 = accepted.结果头.状态 == 场景直接包含状态::精确重复 &&
                                finalPosition.成功(
                                    {2, accepted.结果头.Gread, E, r.预算})
                            ? 世界树操作状态::既有操作已被后继事实覆盖
                            : 世界树操作状态::已发布待复核;
        o.结果头.阶段 = 世界树操作阶段::现实树最终确认;
        o.结果头.首次发布H = accepted.结果头.首次发布H;
        return o;
      }
      o.结果头 = {2,
                  accepted.结果头.状态 == 场景直接包含状态::精确重复
                      ? 世界树操作状态::精确重复
                      : 世界树操作状态::已创建存在并纳入,
                  世界树操作阶段::现实树最终确认, accepted.结果头.Gread,
                  accepted.结果头.首次发布H};
      o.投影 = 世界树存在创建投影{E, *finalPosition.位置};
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
  场景类数据服务 &scene_;
  存在类数据服务 &existence_;
  稳定编码 root_;
  直接归属联合只读组合器 joint_;
};
static_assert(世界树应用合同版本 == 2);
} // namespace 海中鱼巣
