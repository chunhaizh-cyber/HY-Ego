#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <unordered_set>
#include <vector>
#include "../领域/数据服务.存在类.h"
#include "../领域/数据服务.场景类.h"
#include "../领域/数据服务.绑定存在.h"
#include "../领域/数据服务.概念树类.h"
#include "../领域/数据服务.世界树根.h"
#include "../领域/合同.场景角色组织.h"

namespace 海中鱼巣 {
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
  稳定编码 成员{}, 原场景{}, 目标场景{}, 新关系{};
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
                     概念树类数据服务 &, const 世界树根验证请求 &) noexcept;
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
  bool 成功(const 世界树读取预算 &b) const noexcept {
    return 结果头.版本==2&&结果头.状态==世界树操作状态::已验证现实根&&
           结果头.阶段==世界树操作阶段::现实树预读&&结果头.Gread&&树&&
           树->Gread==结果头.Gread&&!树->场景组.empty()&&
           树->场景组.size()<=b.最大场景数量&&b.最大场景数量&&b.最大关系数量;
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
           位置->成员关系.Gread == r.G0 &&
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
           有效(投影->新关系) &&
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
      return life.创建事实代次 && life.创建事实代次 <= g;
    };
    if (!有效(r.父场景) || !有效(投影->E) || 投影->E == r.父场景 ||
        role.Gread != g ||
        !存在身份来源当前见证完整(role.对象存在来源, g, 投影->E) ||
        role.对象存在来源.节点创建事实代次 != h ||
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
    return n.树证明.Gread == g &&
           n.树证明.种类 == 场景树证明种类::树归属 &&
           n.树证明.场景 == 投影->E && 有效(n.树证明.树根) &&
           n.树证明.树根 != 投影->E && 有效(n.树证明.关系) &&
           n.树证明.关系 == n.树证明.见证.编码 && n.树证明.见证.源 == 投影->E &&
           n.树证明.见证.目标 == n.树证明.树根 &&
           有效(n.树证明.见证.关系类型) && n.树证明.见证.角色或顺序 == 1 &&
           n.树证明.见证.生命周期.创建事实代次 == h &&
           current(n.树证明.见证.生命周期) && n.直接父 &&
           n.直接父->Gread == g &&
           n.直接父->父 == r.父场景 && n.直接父->成员 == 投影->E &&
           n.直接父->来源 == 直接归属来源::直接子场景 && 有效(n.直接父->关系) &&
           n.直接父->创建事实代次 == h &&
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

struct 世界树场景创建请求_v4 final {
  std::uint32_t 版本=4; std::uint64_t G0=0; 稳定编码 父场景{}; 纯概念定义 定义;
  概念初始组织指定 组织=概念初始组织指定::未指定;
  std::vector<概念树概念身份> 直接上位; L1所有者范围写入幂等身份 概念定义键;
  存在场景概念创建键_v2 世界键; std::optional<存在单例角色身份> 初始角色;
  世界树绑定创建预算 预算; 概念树预算 概念预算;
  friend bool operator==(const 世界树场景创建请求_v4&,const 世界树场景创建请求_v4&)=default;
};
struct 世界树存在创建请求_v4 final {
  std::uint32_t 版本=4; std::uint64_t G0=0; 存在初始绑定 绑定; 纯概念定义 定义;
  概念初始组织指定 组织=概念初始组织指定::未指定;
  std::vector<概念树概念身份> 直接上位; L1所有者范围写入幂等身份 概念定义键;
  已发布概念绑定键_v2 世界键; std::optional<存在单例角色身份> 初始角色;
  世界树绑定创建预算 预算; 概念树预算 概念预算;
  friend bool operator==(const 世界树存在创建请求_v4&,const 世界树存在创建请求_v4&)=default;
};
using 世界树创建原请求_v4=std::variant<世界树场景创建请求_v4,世界树存在创建请求_v4>;
enum class 世界树概念创建阶段_v4:std::uint8_t{无=0,输入定位=1,概念查询=2,概念发布=3,世界发布=4,最终读回=5};
enum class 世界树概念创建状态_v4:std::uint8_t{已完成=1,精确重复=2,入口拒绝=3,位置失败=4,概念失败=5,世界失败=6,角色冲突=7,事实代次漂移=8,幂等冲突=9,数量预算不足=10,资源失败=12,内部不一致=13,已发布待复核=14,已可能发布=15,后继覆盖=16};
struct 世界树概念创建投影_v4 final {
  稳定编码 E{},世界根{}; 世界树层级位置 位置; 已发布概念绑定投影 内容;
};
struct 世界树概念创建结果_v4 final {
  std::uint32_t 版本=4; 世界树概念创建状态_v4 状态=世界树概念创建状态_v4::入口拒绝;
  世界树概念创建阶段_v4 阶段=世界树概念创建阶段_v4::无; std::uint64_t Gread=0;
  std::optional<std::uint64_t> 概念首次H,世界首次H; std::optional<纯概念查询结果> 概念查询;
  std::optional<纯概念创建恢复结果> 概念恢复; std::optional<纯概念写入结果> 概念写入;
  std::optional<已发布概念绑定恢复结果> 恢复读取; std::optional<已发布概念绑定创建结果> 世界写入;
  std::optional<世界树操作状态> 位置原因; std::optional<世界树创建原请求_v4> 请求回显;
  std::optional<世界树概念创建投影_v4> 投影;
  bool 成功(const 世界树场景创建请求_v4&) const noexcept;
  bool 成功(const 世界树存在创建请求_v4&) const noexcept;
};
struct 世界树存在信息读取请求_v4 final {
  std::uint32_t 版本=4;std::uint64_t G0=0;稳定编码 E{};
  世界树节点视角 视角=世界树节点视角::存在;世界树绑定创建预算 预算;概念树预算 概念预算;
  friend bool operator==(const 世界树存在信息读取请求_v4&,const 世界树存在信息读取请求_v4&)=default;
};
struct 世界树存在信息结果_v4 final {
  std::uint32_t 版本=4;世界树概念创建状态_v4 状态=世界树概念创建状态_v4::入口拒绝;
  std::uint64_t Gread=0;std::optional<世界树概念创建投影_v4> 投影;
  std::optional<世界树操作状态> 位置原因;std::optional<纯概念状态> 概念原因;
  bool 成功(const 世界树存在信息读取请求_v4&) const noexcept;
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
  世界树概念创建结果_v4 创建场景并纳入现实世界(
      const 世界树场景创建请求_v4 &r) noexcept;
  世界树概念创建结果_v4 创建存在并纳入现实世界(
      const 世界树存在创建请求_v4 &r) noexcept;
  世界树存在信息结果_v4 读取世界存在信息(
      const 世界树存在信息读取请求_v4 &r) const noexcept;
  世界树根验证结果 读取当前现实世界根(const 世界树读取预算&) const noexcept;
  bool 使用存在提供者(const 存在类数据服务 &p) const noexcept {
    return &p==&existence_;
  }

private:
  template<class R>
  世界树概念创建结果_v4 创建概念世界(const R &, bool) noexcept;
  struct 已验证世界根令牌 final {};
  世界树应用服务(场景类数据服务 &s, 存在类数据服务 &e,
                 概念树类数据服务 &c, 已验证世界根令牌)
      : scene_(s), existence_(e), concept_(c), joint_(e, s), binding_(e, s) {
    if (!scene_.使用存在提供者(e)||!concept_.绑定于(e.世界树根底座()))
      throw std::invalid_argument("invalid world root");
  }
  friend 世界树应用服务建立结果
  建立世界树应用服务(场景类数据服务 &, 存在类数据服务 &,
                       概念树类数据服务 &, const 世界树根验证请求 &) noexcept;
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
  场景类数据服务 &scene_;
  存在类数据服务 &existence_;
  概念树类数据服务 &concept_;
  直接归属联合只读组合器 joint_;
  绑定存在数据服务 binding_;
};

inline 世界树根验证结果 世界树应用服务::读取当前现实世界根(
    const 世界树读取预算 &budget) const noexcept {
  世界树根验证结果 out{{2,世界树操作状态::入口拒绝,
      世界树操作阶段::现实树预读,0},{}};
  if(!budget.最大场景数量||budget.最大场景数量>4096||
     !budget.最大关系数量||budget.最大关系数量>4096) return out;
  const auto mapCut=[&](概念树数据状态 s) {
    if(s==概念树数据状态::事实代次漂移)return 世界树操作状态::事实代次漂移;
    if(s==概念树数据状态::资源失败)return 世界树操作状态::资源失败;
    return 世界树操作状态::内部不一致;
  };
  const auto first=concept_.读取当前事实代次();
  if(!first.成功()){out.结果头.状态=mapCut(first.状态);return out;}
  out=验证现实世界根({2,first.Gread,budget});
  if(!out.成功({2,first.Gread,budget})) return out;
  const auto tail=concept_.读取当前事实代次();
  if(!tail.成功()){out.结果头.状态=mapCut(tail.状态);out.树.reset();return out;}
  if(tail.Gread!=first.Gread){out.结果头.状态=世界树操作状态::事实代次漂移;
    out.结果头.Gread=tail.Gread;out.树.reset();}
  return out;
}

inline 世界树存在信息结果_v4 世界树应用服务::读取世界存在信息(
    const 世界树存在信息读取请求_v4 &r) const noexcept {
  世界树存在信息结果_v4 out;out.Gread=r.G0;
  try {
    if(r.版本!=4||!r.G0||!有效(r.E)||r.视角==世界树节点视角::世界根场景||
       !r.预算.最大场景数量||!r.预算.最大关系数量||!r.预算.最大祖先数量||
       !r.概念预算.最大概念数||!r.概念预算.最大关系数||!r.概念预算.最大特征属性数)
      return out;
    const 世界树层级位置读取请求 pq{世界树应用合同版本,r.G0,r.E,r.视角,
        {r.预算.最大场景数量,r.预算.最大关系数量}};
    const auto position=读取世界树层级位置(pq);
    if(!position.成功(pq)||!position.位置||!position.位置->直接结构父) {
      out.位置原因=position.结果头.状态;
      out.状态=世界树概念创建状态_v4::位置失败;
      if(position.结果头.状态==世界树操作状态::事实代次漂移)
        out.状态=世界树概念创建状态_v4::事实代次漂移;
      else if(position.结果头.状态==世界树操作状态::数量预算不足)
        out.状态=世界树概念创建状态_v4::数量预算不足;
      return out;
    }
    const auto identity=existence_.读取当前存在身份来源见证(r.G0,r.E);
    if(!identity.成功(r.G0,r.E)||!identity.见证) {
      out.状态=世界树概念创建状态_v4::世界失败;return out;
    }
    const 存在概念使用读取请求 uq{2,r.G0,概念树存在引用{r.E},r.概念预算};
    const auto use=concept_.读取存在概念使用(uq);
    if(!use.成功(uq)||!use.使用||!use.概念) {
      out.概念原因=use.状态;
      out.状态=世界树概念创建状态_v4::概念失败;
      if(use.状态==纯概念状态::事实代次漂移)
        out.状态=世界树概念创建状态_v4::事实代次漂移;
      else if(use.状态==纯概念状态::数量预算不足)
        out.状态=世界树概念创建状态_v4::数量预算不足;
      else if(use.状态==纯概念状态::资源失败)
        out.状态=世界树概念创建状态_v4::资源失败;
      return out;
    }
    const auto &parent=*position.位置->直接结构父;
    存在初始绑定种类 kind=存在初始绑定种类::父存在组成;
    if(parent.来源==直接归属来源::场景成员)
      kind=存在初始绑定种类::场景成员;
    else if(parent.来源==直接归属来源::直接子场景)
      kind=存在初始绑定种类::直接子场景;
    std::optional<场景树节点当前事实> sceneFact;
    if(r.视角==世界树节点视角::场景) {
      const auto root=验证现实世界根({2,r.G0,
          {r.预算.最大场景数量,r.预算.最大关系数量}});
      if(!root.成功({2,r.G0,{r.预算.最大场景数量,r.预算.最大关系数量}})) {
        out.位置原因=root.结果头.状态;out.状态=世界树概念创建状态_v4::位置失败;return out;
      }
      for(const auto &node:root.树->场景组)if(node.场景角色.场景==r.E) {
        if(sceneFact) {out.状态=世界树概念创建状态_v4::内部不一致;return out;}
        sceneFact=node;
      }
      if(!sceneFact) {out.状态=世界树概念创建状态_v4::位置失败;return out;}
    } else {
      if(kind==存在初始绑定种类::父存在组成) {
        const auto parentIdentity=existence_.读取当前存在身份来源见证(r.G0,parent.父);
        if(!parentIdentity.成功(r.G0,parent.父)) {
          out.状态=世界树概念创建状态_v4::内部不一致;return out;
        }
      }
    }
    已发布概念绑定投影 content;
    content.绑定={kind,parent.父,r.E,parent.关系};
    content.存在身份=*identity.见证;content.概念=*use.概念;content.使用=*use.使用;
    content.场景=sceneFact;
    out.投影=世界树概念创建投影_v4{r.E,position.位置->世界根,*position.位置,std::move(content)};
    out.状态=世界树概念创建状态_v4::已完成;
    if(!out.成功(r)) {
      out.状态=世界树概念创建状态_v4::内部不一致;
      out.投影.reset();
    }
  } catch(const std::bad_alloc&){out.状态=世界树概念创建状态_v4::资源失败;out.投影.reset();}
    catch(const std::length_error&){out.状态=世界树概念创建状态_v4::资源失败;out.投影.reset();}
    catch(...){out.状态=世界树概念创建状态_v4::内部不一致;out.投影.reset();}
  return out;
}

namespace 世界树存在信息结果内部 {
template<class T>
inline bool 生命周期有效(const T &v, std::uint64_t h) noexcept {
  return v.创建事实代次 && v.创建事实代次 <= h;
}
inline bool 场景角色完整(const 场景角色当前事实 &v, std::uint64_t g,
                         稳定编码 scene) noexcept {
  if (v.Gread != g || v.场景 != scene ||
      !存在身份来源当前见证完整(v.对象存在来源, g, scene)) return false;
  const 场景节点见证 *meta[]{&v.场景族锚点,&v.场景族归属类型,&v.根绑定类型};
  for(std::size_t i=0;i<3;++i) {
    if(!有效(meta[i]->编码)||!生命周期有效(meta[i]->生命周期,g))return false;
    for(std::size_t j=0;j<i;++j)if(meta[i]->编码==meta[j]->编码)return false;
  }
  if(!有效(v.场景角色登记边.编码)||v.场景角色登记边.源!=scene||
     v.场景角色登记边.目标!=v.场景族锚点.编码||
     v.场景角色登记边.关系类型!=v.场景族归属类型.编码||
     v.场景角色登记边.角色或顺序!=1||
     !生命周期有效(v.场景角色登记边.生命周期,g))return false;
  std::array<稳定编码,8> own{};
  for(std::size_t i=0;i<4;++i) {
    const auto&x=v.四根[i];
    if(static_cast<unsigned>(x.角色)!=i+1||!有效(x.根.编码)||!有效(x.绑定.编码)||
       x.绑定.源!=scene||x.绑定.目标!=x.根.编码||
       x.绑定.关系类型!=v.根绑定类型.编码||x.绑定.角色或顺序!=i+1||
       !生命周期有效(x.根.生命周期,g)||!生命周期有效(x.绑定.生命周期,g)||
       x.根.生命周期.创建事实代次!=x.绑定.生命周期.创建事实代次)return false;
    own[i*2]=x.根.编码;own[i*2+1]=x.绑定.编码;
  }
  for(std::size_t i=0;i<own.size();++i) {
    if(own[i]==scene||own[i]==v.场景族锚点.编码||
       own[i]==v.场景族归属类型.编码||own[i]==v.根绑定类型.编码||
       own[i]==v.场景角色登记边.编码)return false;
    for(std::size_t j=0;j<i;++j)if(own[i]==own[j])return false;
  }
  return true;
}
inline bool 位置完整(const 世界树层级位置 &p,
                     const 世界树存在信息读取请求_v4&r) {
  if(p.节点!=r.E||p.视角!=r.视角||!有效(p.世界根)||!p.直接结构父||
     p.上行路径.empty()||p.上行路径.size()>r.预算.最大关系数量||
     p.上行路径.size()>r.预算.最大祖先数量)return false;
  const auto&direct=*p.直接结构父;
  const auto&first=p.上行路径.front();
  if(direct.Gread!=first.Gread||direct.来源!=first.来源||
     direct.关系!=first.关系||direct.父!=first.父||direct.成员!=first.成员||
     direct.创建事实代次!=first.创建事实代次)return false;
  std::unordered_set<std::uint64_t> seen{r.E.值};
  auto cursor=r.E;
  for(const auto&e:p.上行路径) {
    if((e.来源!=直接归属来源::存在组成&&e.来源!=直接归属来源::场景成员&&
        e.来源!=直接归属来源::直接子场景)||
       !联合父载荷完整(e,r.G0,e.父,cursor)||!seen.insert(e.父.值).second)
      return false;
    cursor=e.父;
  }
  if(cursor!=p.世界根)return false;
  if(r.视角==世界树节点视角::场景)
    return p.父场景语境&&p.父场景语境->Gread==r.G0&&
      p.父场景语境->场景==r.E&&
      p.父场景语境->结构父.关系==p.直接结构父->关系&&
      p.父场景语境->结构父.父==p.直接结构父->父&&
      p.父场景语境->结构父.成员==r.E;
  return !p.父场景语境;
}
}

inline bool 世界树存在信息结果_v4::成功(const 世界树存在信息读取请求_v4&r) const noexcept {
  try {
    if(版本!=4||r.版本!=4||!r.G0||!有效(r.E)||
       (r.视角!=世界树节点视角::场景&&r.视角!=世界树节点视角::存在)||
       !r.预算.最大场景数量||r.预算.最大场景数量>4096||
       !r.预算.最大关系数量||r.预算.最大关系数量>4096||
       !r.预算.最大祖先数量||r.预算.最大祖先数量>4096||
       !r.概念预算.最大概念数||!r.概念预算.最大关系数||
       !r.概念预算.最大特征属性数||状态!=世界树概念创建状态_v4::已完成||
       Gread!=r.G0||位置原因||概念原因||!投影)return false;
    const auto&p=*投影;const auto&c=p.内容;
    if(p.E!=r.E||p.世界根!=p.位置.世界根||
       !世界树存在信息结果内部::位置完整(p.位置,r)||
       c.绑定.新存在!=r.E||c.绑定.绑定节点!=p.位置.直接结构父->父||
       c.绑定.绑定关系!=p.位置.直接结构父->关系||
       c.存在身份.身份!=r.E||!存在身份来源当前见证完整(c.存在身份,r.G0,r.E)||
       c.角色)return false;
    const auto source=p.位置.直接结构父->来源;
    if(source!=直接归属来源::存在组成&&source!=直接归属来源::场景成员&&
       source!=直接归属来源::直接子场景)return false;
    存在初始绑定种类 kind{};
    if(source==直接归属来源::存在组成)kind=存在初始绑定种类::父存在组成;
    else if(source==直接归属来源::场景成员)kind=存在初始绑定种类::场景成员;
    else if(source==直接归属来源::直接子场景)kind=存在初始绑定种类::直接子场景;
    if(c.绑定.种类!=kind)return false;
    const 纯概念读取请求 conceptRequest{2,r.G0,c.概念.概念,r.概念预算};
    const 纯概念读取结果 conceptResult{2,纯概念状态::已读取,r.G0,c.概念};
    if(!conceptResult.成功(conceptRequest)||c.概念.类别!=相关概念类别::存在||
       c.使用.E!=r.E||c.使用.EC!=c.概念.概念||!有效(c.使用.关系)||
       !世界树存在信息结果内部::生命周期有效(c.使用.生命周期,r.G0))return false;
    if(r.视角==世界树节点视角::场景)
      return kind==存在初始绑定种类::直接子场景&&c.场景&&c.场景->直接父&&
        c.场景->直接父->Gread==p.位置.直接结构父->Gread&&
        c.场景->直接父->来源==p.位置.直接结构父->来源&&
        c.场景->直接父->关系==p.位置.直接结构父->关系&&
        c.场景->直接父->父==p.位置.直接结构父->父&&
        c.场景->直接父->成员==p.位置.直接结构父->成员&&
        世界树存在信息结果内部::场景角色完整(c.场景->场景角色,r.G0,r.E)&&
        c.场景->树证明.Gread==r.G0&&
        c.场景->树证明.种类==场景树证明种类::树归属&&
        c.场景->树证明.场景==r.E&&c.场景->树证明.树根==p.世界根&&
        有效(c.场景->树证明.关系)&&
        c.场景->树证明.见证.编码==c.场景->树证明.关系&&
        c.场景->树证明.见证.源==r.E&&
        c.场景->树证明.见证.目标==p.世界根&&
        有效(c.场景->树证明.见证.关系类型)&&
        c.场景->树证明.见证.角色或顺序==1&&
        世界树存在信息结果内部::生命周期有效(
          c.场景->树证明.见证.生命周期,r.G0);
    return !c.场景;
  } catch(...) {return false;}
}

template<class R>
inline 世界树概念创建结果_v4 世界树应用服务::创建概念世界(
    const R &r,bool sceneNode) noexcept {
  世界树概念创建结果_v4 out;out.Gread=r.G0;
  auto mapConcept=[](纯概念状态 s){
    switch(s){
    case 纯概念状态::事实代次漂移:return 世界树概念创建状态_v4::事实代次漂移;
    case 纯概念状态::幂等冲突:return 世界树概念创建状态_v4::幂等冲突;
    case 纯概念状态::数量预算不足:return 世界树概念创建状态_v4::数量预算不足;
    case 纯概念状态::资源失败:return 世界树概念创建状态_v4::资源失败;
    case 纯概念状态::内部不一致:return 世界树概念创建状态_v4::内部不一致;
    case 纯概念状态::已可能发布:return 世界树概念创建状态_v4::已可能发布;
    default:return 世界树概念创建状态_v4::概念失败;
    }
  };
  auto mapWorld=[](已发布概念绑定状态 s){
    switch(s){
    case 已发布概念绑定状态::角色冲突:return 世界树概念创建状态_v4::角色冲突;
    case 已发布概念绑定状态::事实代次漂移:return 世界树概念创建状态_v4::事实代次漂移;
    case 已发布概念绑定状态::幂等冲突:return 世界树概念创建状态_v4::幂等冲突;
    case 已发布概念绑定状态::数量预算不足:return 世界树概念创建状态_v4::数量预算不足;
    case 已发布概念绑定状态::资源失败:return 世界树概念创建状态_v4::资源失败;
    case 已发布概念绑定状态::内部不一致:return 世界树概念创建状态_v4::内部不一致;
    case 已发布概念绑定状态::已可能发布:return 世界树概念创建状态_v4::已可能发布;
    case 已发布概念绑定状态::当前事实不再匹配:return 世界树概念创建状态_v4::后继覆盖;
    case 已发布概念绑定状态::入口拒绝:return 世界树概念创建状态_v4::入口拒绝;
    default:return 世界树概念创建状态_v4::世界失败;
    }
  };
  try {
    out.请求回显=世界树创建原请求_v4{r};
    out.阶段=世界树概念创建阶段_v4::输入定位;
    if(r.版本!=4||!r.G0||!有效(r.概念定义键)||
       !r.预算.最大关系数量||!r.预算.最大场景数量||!r.预算.最大祖先数量||
       !r.概念预算.最大概念数||!r.概念预算.最大关系数||!r.概念预算.最大特征属性数||
       std::holds_alternative<纯I64特征概念定义>(r.定义)) return out;
    纯概念定义 normalizedDefinition=r.定义;
    if(auto*ec=std::get_if<纯合取存在概念定义>(&normalizedDefinition)) {
      if(ec->特征模板组.empty())return out;
      std::sort(ec->特征模板组.begin(),ec->特征模板组.end(),
          [](auto a,auto b){return a.值.值<b.值.值;});
      ec->特征模板组.erase(std::unique(ec->特征模板组.begin(),ec->特征模板组.end()),
                            ec->特征模板组.end());
    } else {
      const auto&universal=std::get<通用存在概念定义>(normalizedDefinition);
      if(universal.规则版本!=1||universal.规则!=通用存在定义规则::不预设特征)return out;
    }
    auto normalizedParents=r.直接上位;
    std::sort(normalizedParents.begin(),normalizedParents.end(),
        [](auto a,auto b){return a.值.值<b.值.值;});
    if(std::any_of(normalizedParents.begin(),normalizedParents.end(),
          [](auto x){return !有效(x.值);})||
       std::adjacent_find(normalizedParents.begin(),normalizedParents.end())!=normalizedParents.end()||
       !((r.组织==概念初始组织指定::显式顶层&&normalizedParents.empty())||
         (r.组织==概念初始组织指定::具名上位&&!normalizedParents.empty()))||
       (std::holds_alternative<通用存在概念定义>(normalizedDefinition)&&
        r.组织!=概念初始组织指定::显式顶层))return out;
    存在初始绑定 bindingSpec;
    已发布概念绑定键_v2 worldKey=r.世界键;
    if constexpr(std::is_same_v<R,世界树场景创建请求_v4>) {
      if(!有效(r.父场景))return out;
      bindingSpec={存在初始绑定种类::直接子场景,r.父场景};
    } else {
      if(!有效(r.绑定.绑定节点)||r.绑定.种类==存在初始绑定种类::直接子场景)return out;
      bindingSpec=r.绑定;
    }
    std::vector<std::uint64_t> keys{r.概念定义键.值};
    std::visit([&](const auto &k){
      keys.push_back(k.组合.值);keys.push_back(k.存在.值);
      if constexpr(requires{k.场景;})keys.push_back(k.场景.值);
      keys.push_back(k.概念引用.值);
    },worldKey);
    std::sort(keys.begin(),keys.end());
    if(keys.front()==0||std::adjacent_find(keys.begin(),keys.end())!=keys.end())return out;

    const auto current=concept_.读取当前事实代次();
    if(!current.成功()){out.状态=世界树概念创建状态_v4::资源失败;return out;}
    out.Gread=current.Gread;
    const 世界树读取预算 rootBudget{r.预算.最大场景数量,r.预算.最大关系数量};
    const auto root=读取当前现实世界根(rootBudget);
    if(!root.成功({2,current.Gread,rootBudget})) {
      out.Gread=root.结果头.Gread;
      if(root.结果头.状态==世界树操作状态::事实代次漂移)
        out.状态=世界树概念创建状态_v4::事实代次漂移;
      else if(root.结果头.状态==世界树操作状态::资源失败)
        out.状态=世界树概念创建状态_v4::资源失败;
      else if(root.结果头.状态==世界树操作状态::数量预算不足)
        out.状态=世界树概念创建状态_v4::数量预算不足;
      else out.状态=世界树概念创建状态_v4::位置失败;
      out.位置原因=root.结果头.状态;
      return out;
    }
    std::optional<纯概念事实> conceptFact;
    const 已发布概念绑定恢复请求 recoverReq{
        .版本=2,
        .Gread=current.Gread,
        .绑定=bindingSpec,
        .期望现实树根=root.树->根场景,
        .键=worldKey,
        .预算={r.预算.最大关系数量,r.预算.最大场景数量,r.预算.最大祖先数量},
        .预期定义=normalizedDefinition,
        .概念预算=r.概念预算,
        .初始角色=r.初始角色};
    out.恢复读取=binding_.读取已发布概念绑定创建(recoverReq,concept_);
    out.世界首次H=out.恢复读取->首次发布代次;
    out.Gread=out.恢复读取->Gread;
    if(out.世界首次H)out.阶段=世界树概念创建阶段_v4::世界发布;
    if(out.恢复读取->成功(recoverReq)) {
      out.阶段=世界树概念创建阶段_v4::最终读回;
      const auto &p=*out.恢复读取->投影;
      const auto read=读取世界存在信息({4,out.恢复读取->Gread,p.绑定.新存在,
          sceneNode?世界树节点视角::场景:世界树节点视角::存在,r.预算,r.概念预算});
      out.Gread=read.Gread;
      if(!read.成功({4,out.恢复读取->Gread,p.绑定.新存在,
          sceneNode?世界树节点视角::场景:世界树节点视角::存在,r.预算,r.概念预算})) {
        out.状态=世界树概念创建状态_v4::已发布待复核;out.位置原因=read.位置原因;return out;
      }
      out.投影=read.投影;out.投影->内容.角色=p.角色;
      out.状态=世界树概念创建状态_v4::精确重复;
      bool complete=false;
      if constexpr(std::is_same_v<R,世界树场景创建请求_v4>) complete=out.成功(r);
      else complete=out.成功(r);
      if(!complete){out.状态=世界树概念创建状态_v4::内部不一致;out.投影.reset();}
      return out;
    }
    if(out.恢复读取->状态!=已发布概念绑定状态::未派发) {
      out.状态=mapWorld(out.恢复读取->状态);return out;
    }
    out.恢复读取.reset();

    const 纯概念创建恢复请求 conceptRecoverReq{2,current.Gread,r.概念定义键,
        normalizedDefinition,r.组织,normalizedParents,r.概念预算};
    out.概念恢复=concept_.读取纯概念创建首次结果(conceptRecoverReq);
    if(out.概念恢复->Gread)out.Gread=out.概念恢复->Gread;
    if(out.概念恢复->首次发布H)out.概念首次H=out.概念恢复->首次发布H;
    if(out.概念恢复->成功(conceptRecoverReq)) {
      conceptFact=out.概念恢复->事实;
    }

    if(!out.概念恢复->成功(conceptRecoverReq)&&
       out.概念恢复->状态!=纯概念恢复状态::未派发) {
      out.状态=世界树概念创建状态_v4::概念失败;
      if(out.概念恢复->状态==纯概念恢复状态::事实代次漂移)
        out.状态=世界树概念创建状态_v4::事实代次漂移;
      else if(out.概念恢复->状态==纯概念恢复状态::幂等冲突)
        out.状态=世界树概念创建状态_v4::幂等冲突;
      else if(out.概念恢复->状态==纯概念恢复状态::数量预算不足)
        out.状态=世界树概念创建状态_v4::数量预算不足;
      else if(out.概念恢复->状态==纯概念恢复状态::资源失败)
        out.状态=世界树概念创建状态_v4::资源失败;
      return out;
    }
    if(!conceptFact)out.概念恢复.reset();

    const auto refresh=concept_.读取当前事实代次();
    if(!refresh.成功()){out.状态=世界树概念创建状态_v4::资源失败;return out;}
    out.Gread=refresh.Gread;
    if(!conceptFact&&refresh.Gread!=r.G0){out.状态=世界树概念创建状态_v4::事实代次漂移;return out;}
    const auto guardG=refresh.Gread;
    auto parentView=世界树节点视角::场景;
    if(bindingSpec.种类==存在初始绑定种类::父存在组成)
      parentView=世界树节点视角::存在;
    else if(bindingSpec.绑定节点==root.树->根场景)
      parentView=世界树节点视角::世界根场景;
    const 世界树层级位置读取请求 posReq{世界树应用合同版本,guardG,
        bindingSpec.绑定节点,parentView,{r.预算.最大场景数量,r.预算.最大关系数量}};
    const auto pos=读取世界树层级位置(posReq);
    if(!pos.成功(posReq)) {
      out.位置原因=pos.结果头.状态;
      out.状态=世界树概念创建状态_v4::位置失败;
      if(pos.结果头.状态==世界树操作状态::事实代次漂移)
        out.状态=世界树概念创建状态_v4::事实代次漂移;
      return out;
    }
    if(pos.位置->上行路径.size()>r.预算.最大祖先数量) {
      out.状态=世界树概念创建状态_v4::数量预算不足;
      return out;
    }

    out.阶段=世界树概念创建阶段_v4::概念查询;
    if(!conceptFact) {
      const 纯概念查询请求 qq{2,guardG,normalizedDefinition,r.概念预算};
      if(!out.概念查询||out.概念查询->Gread!=guardG)
        out.概念查询=concept_.精确查询纯概念(qq);
      if(out.概念查询->成功(qq))conceptFact=out.概念查询->事实;
      else if(out.概念查询->确认未找到(qq)) {
        out.阶段=世界树概念创建阶段_v4::概念发布;
        const 纯概念创建请求 cq{2,guardG,r.概念定义键,normalizedDefinition,
            r.组织,normalizedParents,r.概念预算};
        out.概念写入=concept_.创建或复用纯概念(cq);
        if(out.概念写入->Gread)out.Gread=out.概念写入->Gread;
        if(out.概念写入->首次发布H)out.概念首次H=out.概念写入->首次发布H;
        if(!out.概念写入->成功(cq)){out.状态=mapConcept(out.概念写入->状态);return out;}
        conceptFact=out.概念写入->事实;
      } else {out.状态=mapConcept(out.概念查询->状态);return out;}
    }
    const auto&requestedParents=normalizedParents;
    std::vector<概念树概念身份> actualParents;
    for(const auto&edge:conceptFact->直接上位)actualParents.push_back(edge.上位);
    std::sort(actualParents.begin(),actualParents.end(),
        [](auto a,auto b){return a.值.值<b.值.值;});
    if(actualParents!=requestedParents||
       (r.组织==概念初始组织指定::显式顶层&&!actualParents.empty())||
       (r.组织==概念初始组织指定::具名上位&&actualParents.empty())||
       (std::holds_alternative<通用存在概念定义>(conceptFact->定义)&&
        r.组织!=概念初始组织指定::显式顶层)) {
      out.状态=世界树概念创建状态_v4::概念失败;
      return out;
    }
    const auto worldCurrent=concept_.读取当前事实代次();
    if(!worldCurrent.成功()){out.状态=世界树概念创建状态_v4::资源失败;return out;}
    out.Gread=worldCurrent.Gread;
    const 世界树层级位置读取请求 worldPosReq{世界树应用合同版本,worldCurrent.Gread,
        bindingSpec.绑定节点,parentView,{r.预算.最大场景数量,r.预算.最大关系数量}};
    const auto worldPos=读取世界树层级位置(worldPosReq);
    if(!worldPos.成功(worldPosReq)){out.位置原因=worldPos.结果头.状态;out.状态=世界树概念创建状态_v4::位置失败;return out;}
    if(worldPos.位置->上行路径.size()>r.预算.最大祖先数量) {
      out.状态=世界树概念创建状态_v4::数量预算不足;
      return out;
    }
    const 已发布概念绑定创建请求 child{
        .版本=2,
        .G0=worldCurrent.Gread,
        .绑定=bindingSpec,
        .期望现实树根=root.树->根场景,
        .键=worldKey,
        .预算={r.预算.最大关系数量,r.预算.最大场景数量,r.预算.最大祖先数量},
        .EC=conceptFact->概念,
        .预期定义=conceptFact->定义,
        .概念预算=r.概念预算,
        .初始角色=r.初始角色};
    out.阶段=世界树概念创建阶段_v4::世界发布;
    if(sceneNode) out.世界写入=scene_.添加场景节点(child,binding_,concept_);
    else out.世界写入=existence_.添加存在节点(child,binding_,concept_);
    out.世界首次H=out.世界写入->首次发布代次;
    out.Gread=out.世界写入->Gread;
    if(!out.世界写入->成功(child)){out.状态=mapWorld(out.世界写入->状态);return out;}
    const auto E=out.世界写入->投影->绑定.新存在;
    out.阶段=世界树概念创建阶段_v4::最终读回;
    const auto finalView=sceneNode ? 世界树节点视角::场景 : 世界树节点视角::存在;
    const 世界树存在信息读取请求_v4 finalReq{4,out.Gread,E,finalView,r.预算,r.概念预算};
    const auto final=读取世界存在信息(finalReq);
    if(!final.成功(finalReq)){out.状态=世界树概念创建状态_v4::已发布待复核;
      out.位置原因=final.位置原因;return out;}
    out.投影=final.投影;out.投影->内容.角色=out.世界写入->投影->角色;
    out.状态=世界树概念创建状态_v4::已完成;
    if(out.世界写入->状态==已发布概念绑定状态::精确重复)
      out.状态=世界树概念创建状态_v4::精确重复;
    bool complete=false;
    if constexpr(std::is_same_v<R,世界树场景创建请求_v4>) complete=out.成功(r);
    else complete=out.成功(r);
    if(!complete){out.状态=世界树概念创建状态_v4::内部不一致;out.投影.reset();}
  } catch(const std::bad_alloc&){out.状态=世界树概念创建状态_v4::资源失败;if(out.世界首次H)out.状态=世界树概念创建状态_v4::已发布待复核;out.投影.reset();}
    catch(const std::length_error&){out.状态=世界树概念创建状态_v4::资源失败;if(out.世界首次H)out.状态=世界树概念创建状态_v4::已发布待复核;out.投影.reset();}
    catch(...){out.状态=世界树概念创建状态_v4::内部不一致;if(out.世界首次H)out.状态=世界树概念创建状态_v4::已发布待复核;out.投影.reset();}
  return out;
}

inline 世界树概念创建结果_v4 世界树应用服务::创建场景并纳入现实世界(
    const 世界树场景创建请求_v4 &r) noexcept {return 创建概念世界(r,true);}
inline 世界树概念创建结果_v4 世界树应用服务::创建存在并纳入现实世界(
    const 世界树存在创建请求_v4 &r) noexcept {return 创建概念世界(r,false);}

namespace 世界树应用内部 {
inline bool 相同上位(const std::vector<概念树概念身份>&a,
                    const std::vector<概念树概念身份>&b) noexcept {
  if(a.size()!=b.size()) return false;
  for(std::size_t i=0;i<a.size();++i) if(a[i].值!=b[i].值) return false;
  return true;
}
inline bool 相同请求(const 世界树场景创建请求_v4&a,
                    const 世界树场景创建请求_v4&b) noexcept {
  return a.版本==b.版本&&a.G0==b.G0&&a.父场景==b.父场景&&a.定义==b.定义&&
      a.组织==b.组织&&相同上位(a.直接上位,b.直接上位)&&a.概念定义键==b.概念定义键&&
      a.世界键==b.世界键&&a.初始角色==b.初始角色&&a.预算==b.预算&&a.概念预算==b.概念预算;
}
inline bool 相同请求(const 世界树存在创建请求_v4&a,
                    const 世界树存在创建请求_v4&b) noexcept {
  return a.版本==b.版本&&a.G0==b.G0&&a.绑定==b.绑定&&a.定义==b.定义&&
      a.组织==b.组织&&相同上位(a.直接上位,b.直接上位)&&a.概念定义键==b.概念定义键&&
      a.世界键==b.世界键&&a.初始角色==b.初始角色&&a.预算==b.预算&&a.概念预算==b.概念预算;
}
inline bool 相同概念事实(const 纯概念事实&a,const 纯概念事实&b) noexcept {
  if(a.概念!=b.概念||a.类别!=b.类别||a.定义!=b.定义||a.定义记录!=b.定义记录||
     a.定义记录生命周期!=b.定义记录生命周期||a.直接上位!=b.直接上位||
     a.治理状态!=b.治理状态||a.生命周期值事实!=b.生命周期值事实||
     a.生命周期值生命周期!=b.生命周期值生命周期||a.生命周期!=b.生命周期||
     a.定义关系组.size()!=b.定义关系组.size())return false;
  for(std::size_t i=0;i<a.定义关系组.size();++i) {
    const auto&x=a.定义关系组[i];const auto&y=b.定义关系组[i];
    if(x.关系!=y.关系||x.源!=y.源||x.目标!=y.目标||x.关系类型!=y.关系类型||
       x.种类!=y.种类||x.顺序!=y.顺序||x.生命周期!=y.生命周期)return false;
  }
  return true;
}
inline bool 相同存在身份(const 存在身份来源当前见证&a,
                         const 存在身份来源当前见证&b) noexcept {
  return a.身份==b.身份&&a.族锚点==b.族锚点&&
      a.族归属关系类型==b.族归属关系类型&&a.族归属关系==b.族归属关系&&
      a.角色==b.角色&&a.节点创建事实代次==b.节点创建事实代次&&
      a.族锚点创建事实代次==b.族锚点创建事实代次&&
      a.关系类型创建事实代次==b.关系类型创建事实代次&&
      a.归属关系创建事实代次==b.归属关系创建事实代次;
}
inline bool 相同绑定内容(const 已发布概念绑定投影&a,
                         const 已发布概念绑定投影&b) noexcept {
  if(a.绑定!=b.绑定||!相同存在身份(a.存在身份,b.存在身份)||
     !相同概念事实(a.概念,b.概念)||a.使用!=b.使用||
     a.场景.has_value()!=b.场景.has_value()||a.角色.has_value()!=b.角色.has_value())
    return false;
  if(a.场景) {
    if(a.场景->场景角色.场景!=b.场景->场景角色.场景||
       a.场景->树证明.树根!=b.场景->树证明.树根||
       a.场景->树证明.关系!=b.场景->树证明.关系||
       a.场景->直接父.has_value()!=b.场景->直接父.has_value())return false;
    if(a.场景->直接父&&
       (a.场景->直接父->关系!=b.场景->直接父->关系||
        a.场景->直接父->父!=b.场景->直接父->父||
        a.场景->直接父->成员!=b.场景->直接父->成员||
        a.场景->直接父->来源!=b.场景->直接父->来源))return false;
  }
  if(a.角色&&(a.角色->角色!=b.角色->角色||a.角色->E!=b.角色->E||
      a.角色->登记关系!=b.角色->登记关系||a.角色->目标关系!=b.角色->目标关系))
    return false;
  return true;
}
template<class R>
inline bool 概念事实匹配(const 世界树概念创建结果_v4&out,const R&r) noexcept {
  if(!out.投影)return false;
  const auto&fact=out.投影->内容.概念;
  try {
    auto parents=r.直接上位;
    std::sort(parents.begin(),parents.end(),[](auto a,auto b){return a.值.值<b.值.值;});
    if(std::adjacent_find(parents.begin(),parents.end())!=parents.end()||
       fact.直接上位.size()!=parents.size())return false;
    std::vector<概念树概念身份> actual;
    for(const auto&e:fact.直接上位)actual.push_back(e.上位);
    std::sort(actual.begin(),actual.end(),[](auto a,auto b){return a.值.值<b.值.值;});
    if(actual!=parents||
       (r.组织==概念初始组织指定::显式顶层&&!actual.empty())||
       (r.组织==概念初始组织指定::具名上位&&actual.empty()))return false;
    纯概念定义 expected=r.定义;
    if(auto*ec=std::get_if<纯合取存在概念定义>(&expected)) {
      if(ec->特征模板组.empty())return false;
      std::sort(ec->特征模板组.begin(),ec->特征模板组.end(),
          [](auto a,auto b){return a.值.值<b.值.值;});
      ec->特征模板组.erase(std::unique(ec->特征模板组.begin(),ec->特征模板组.end()),
                            ec->特征模板组.end());
    } else if(const auto*ec=std::get_if<通用存在概念定义>(&expected)) {
      if(ec->规则版本!=1||ec->规则!=通用存在定义规则::不预设特征)return false;
    } else return false;
    if(fact.定义!=expected||
       (std::holds_alternative<通用存在概念定义>(expected)&&
        r.组织!=概念初始组织指定::显式顶层))return false;
    if(out.概念恢复) {
      const 纯概念创建恢复请求 q{2,out.概念恢复->Gread,r.概念定义键,
          expected,r.组织,parents,r.概念预算};
      if(out.概念恢复->成功(q)&&out.概念恢复->事实&&
         相同概念事实(*out.概念恢复->事实,fact))return true;
    }
    if(out.概念查询) {
      const 纯概念查询请求 q{2,out.概念查询->Gread,
          expected,r.概念预算};
      if(out.概念查询->成功(q)&&out.概念查询->事实&&
         相同概念事实(*out.概念查询->事实,fact))return true;
    }
    if(out.概念写入&&out.概念写入->原请求) {
      const auto&q=*out.概念写入->原请求;
      if(q.幂等身份==r.概念定义键&&q.定义==expected&&q.组织==r.组织&&
         q.直接上位==parents&&q.预算==r.概念预算&&
         out.概念写入->成功(q)&&out.概念写入->事实&&
         相同概念事实(*out.概念写入->事实,fact))return true;
    }
    if(out.恢复读取&&out.恢复读取->投影&&
       相同概念事实(out.恢复读取->投影->概念,fact))return true;
    return false;
  } catch(...) {return false;}
}
template<class R>
inline bool 绑定结果匹配(const 世界树概念创建结果_v4&out,const R&r,
                         const 存在初始绑定&binding) noexcept {
  if(!out.投影||!out.世界首次H)return false;
  const auto&final=*out.投影;
  const 已发布概念绑定恢复请求 rr{.版本=2,.Gread=out.Gread,.绑定=binding,
      .期望现实树根=final.世界根,.键=已发布概念绑定键_v2{r.世界键},
      .预算={r.预算.最大关系数量,r.预算.最大场景数量,r.预算.最大祖先数量},
      .预期定义=final.内容.概念.定义,.概念预算=r.概念预算,.初始角色=r.初始角色};
  if(out.世界写入) {
    if(!out.世界写入->原实际子请求)return false;
    const auto&q=*out.世界写入->原实际子请求;
    return out.世界写入->成功(q)&&out.世界写入->Gread==out.Gread&&
        out.世界写入->首次发布代次==out.世界首次H&&q.G0<*out.世界首次H&&
        q.绑定==binding&&q.期望现实树根==final.世界根&&q.键==rr.键&&
        q.预算==rr.预算&&q.EC==final.内容.概念.概念&&
        q.预期定义==final.内容.概念.定义&&q.概念预算==r.概念预算&&
        q.初始角色==r.初始角色&&out.世界写入->投影&&
        相同绑定内容(*out.世界写入->投影,final.内容);
  }
  return out.恢复读取&&out.恢复读取->首次发布代次==out.世界首次H&&
      out.恢复读取->Gread==out.Gread&&out.恢复读取->成功(rr)&&
      out.恢复读取->投影&&相同绑定内容(*out.恢复读取->投影,final.内容);
}
}

inline bool 世界树概念创建结果_v4::成功(const 世界树场景创建请求_v4&r) const noexcept {
  if(!(版本==4&&r.版本==4&&(状态==世界树概念创建状态_v4::已完成||
      状态==世界树概念创建状态_v4::精确重复)&&阶段==世界树概念创建阶段_v4::最终读回&&
      Gread&&世界首次H&&*世界首次H<=Gread&&请求回显&&
      std::holds_alternative<世界树场景创建请求_v4>(*请求回显)&&
      世界树应用内部::相同请求(std::get<世界树场景创建请求_v4>(*请求回显),r)&&投影&&
      投影->位置.视角==世界树节点视角::场景&&投影->位置.直接结构父&&
      投影->位置.直接结构父->父==r.父场景&&投影->内容.绑定.种类==存在初始绑定种类::直接子场景&&
      投影->内容.概念.类别==相关概念类别::存在&&投影->内容.使用.EC==投影->内容.概念.概念&&
      投影->内容.角色.has_value()==r.初始角色.has_value()&&
      (!投影->内容.角色||投影->内容.角色->角色==*r.初始角色)&&
      世界树应用内部::概念事实匹配(*this,r)&&
      bool(世界写入)!=bool(恢复读取))) return false;
  return 世界树应用内部::绑定结果匹配(*this,r,
      {存在初始绑定种类::直接子场景,r.父场景});
}
inline bool 世界树概念创建结果_v4::成功(const 世界树存在创建请求_v4&r) const noexcept {
  if(!(版本==4&&r.版本==4&&(状态==世界树概念创建状态_v4::已完成||
      状态==世界树概念创建状态_v4::精确重复)&&阶段==世界树概念创建阶段_v4::最终读回&&
      Gread&&世界首次H&&*世界首次H<=Gread&&请求回显&&
      std::holds_alternative<世界树存在创建请求_v4>(*请求回显)&&
      世界树应用内部::相同请求(std::get<世界树存在创建请求_v4>(*请求回显),r)&&投影&&
      投影->位置.视角==世界树节点视角::存在&&投影->位置.直接结构父&&
      投影->位置.直接结构父->父==r.绑定.绑定节点&&
      投影->内容.绑定.种类==r.绑定.种类&&投影->内容.概念.类别==相关概念类别::存在&&
      投影->内容.使用.EC==投影->内容.概念.概念&&
      投影->内容.角色.has_value()==r.初始角色.has_value()&&
      (!投影->内容.角色||投影->内容.角色->角色==*r.初始角色)&&
      世界树应用内部::概念事实匹配(*this,r)&&
      bool(世界写入)!=bool(恢复读取))) return false;
  return 世界树应用内部::绑定结果匹配(*this,r,r.绑定);
}

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
                     概念树类数据服务 &concepts, const 世界树根验证请求 &r) noexcept {
  世界树应用服务建立结果 out;
  out.Gread = r.G0;
  if (r.版本 != 2 || !r.G0 || r.预算.最大场景数量 < 1 ||
       r.预算.最大关系数量 < 1 || !scene.使用存在提供者(existence)||
       !concepts.绑定于(existence.世界树根底座()))
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
    const auto firstH = rootNode.场景角色.对象存在来源.节点创建事实代次;
    世界树全局根投影 projection{root, firstH, r.G0};
    auto candidate=std::unique_ptr<世界树应用服务>(new 世界树应用服务(
        scene, existence, concepts, 世界树应用服务::已验证世界根令牌{}));
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
    out.服务=std::move(candidate);
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
