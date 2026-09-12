module;

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.存在类;
import 海中鱼巣.领域.数据服务.绑定存在;

export import 海中鱼巣.领域.数据服务.特征类;
export import 海中鱼巣.领域.合同.存在结构身份只读;
import 海中鱼巣.领域.合同.世界树根;

namespace 海中鱼巣::存在类数据内部 {

inline constexpr L1所有者范围写集本地键 存在结点本地键{1};
inline constexpr L1所有者范围写集本地键 成员关系本地键{2};
inline constexpr L1所有者范围写集本地键 存在族归属关系本地键{2};
inline constexpr L1所有者范围写集本地键 存在族锚点本地键{1};
inline constexpr L1所有者范围写集本地键 存在族归属类型本地键{2};
inline constexpr L1所有者范围写集本地键 任务虚拟存在结点本地键{1};
inline constexpr L1所有者范围写集本地键 任务虚拟存在关系本地键{2};
inline constexpr L1所有者范围写集本地键 方法虚拟存在结点本地键{1};
inline constexpr L1所有者范围写集本地键 方法虚拟存在关系本地键{2};
inline constexpr L1所有者范围写集本地键 任务方法实例节点本地键{1};
inline constexpr L1所有者范围写集本地键 任务方法实例宿主关系本地键{2};
inline constexpr L1所有者范围写集本地键 任务方法实例来源方法关系本地键{3};
inline constexpr L1所有者范围写集本地键 任务方法实例来源Vm关系本地键{4};
inline constexpr L1所有者范围写集本地键 任务方法实例参数首关系本地键{5};
inline constexpr L1所有者范围写集本地键 任务实例参数数据关系本地键{1};

std::optional<稳定编码> 查找唯一编码(const L1所有者范围写入结果 &结果,
                                     L1所有者范围写集本地键 本地键) noexcept {
  std::optional<稳定编码> 找到;
  for (const auto &[键, 编码] : 结果.新编码映射) {
    if (键 != 本地键)
      continue;
    if (找到 || !有效(编码))
      return std::nullopt;
    找到 = 编码;
  }
  return 找到;
}

} // namespace 海中鱼巣::存在类数据内部

import 海中鱼巣.领域.数据服务.定位特征;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 存在类数据合同版本 = 1;

enum class 存在类成员种类 : std::uint8_t { 子存在 = 1, 特征 = 2 };

enum class 存在类数据状态 : std::uint8_t {
  已创建 = 1,
  精确重复 = 2,
  已读取 = 3,
  已添加成员 = 4,
  已移除成员 = 5,
  已退出 = 6,
  入口拒绝 = 7,
  未找到 = 8,
  目标已退出 = 9,
  成员已存在 = 10,
  成员未找到 = 11,
  子存在读取失败 = 12,
  特征读取失败 = 13,
  结构类型未找到 = 16,
  结构类型已退出 = 17,
  事实代次漂移 = 18,
  幂等冲突 = 19,
  引用冲突 = 20,
  历史材料已清理 = 21,
  资源失败 = 22,
  内部不一致 = 23,
  已可能发布 = 24,
  已建立当前采用 = 25,
  已替换当前采用 = 26,
  已解除当前采用 = 27,
  格式不支持 = 28,
  数量预算不足 = 29
};

enum class 存在历史读取状态 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  目标已退出 = 4,
  事实代次漂移 = 5,
  引用冲突 = 6,
  历史材料不可用 = 7,
  资源失败 = 8,
  内部不一致 = 9,
  数量预算不足 = 10
};
struct 存在当前采用结构交付 final {
  稳定编码 当前采用关系类型;
};
struct 存在当前采用事实 final {
  稳定编码 关系, E;
  特征类型身份 FT;
  特征信息身份 F;
  friend bool operator==(const 存在当前采用事实 &,
                         const 存在当前采用事实 &) = default;
};
struct 存在当前采用读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  稳定编码 E;
  特征类型身份 FT;
  std::uint64_t 关系预算 = 0;
};
struct 存在当前采用建立 final {
  特征信息身份 F;
  friend bool operator==(const 存在当前采用建立 &,
                         const 存在当前采用建立 &) = default;
};
struct 存在当前采用替换 final {
  存在当前采用事实 预期;
  特征信息身份 新F;
  friend bool operator==(const 存在当前采用替换 &,
                         const 存在当前采用替换 &) = default;
};
struct 存在当前采用解除 final {
  存在当前采用事实 预期;
  friend bool operator==(const 存在当前采用解除 &,
                         const 存在当前采用解除 &) = default;
};
struct 存在关联已知并采用 final {
  特征信息身份 F;
  std::optional<存在当前采用事实> 预期;
  friend bool operator==(const 存在关联已知并采用 &,
                         const 存在关联已知并采用 &) = default;
};
using 存在当前采用操作 = std::variant<存在当前采用建立, 存在当前采用替换,
                                      存在当前采用解除, 存在关联已知并采用>;
struct 存在当前采用写请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G = 0;
  L1所有者范围写入幂等身份 幂等身份;
  稳定编码 E;
  特征类型身份 FT;
  存在当前采用操作 操作;
  std::uint64_t 关系预算 = 0;
  friend bool operator==(const 存在当前采用写请求 &,
                         const 存在当前采用写请求 &) = default;
};
struct 存在当前采用结果 final {
  std::uint32_t 版本 = 1;
  存在类数据状态 状态 = 存在类数据状态::入口拒绝;
  std::uint64_t Gread = 0, H = 0;
  std::optional<std::uint64_t> 首次H;
  std::optional<存在当前采用事实> 采用;
  std::optional<存在当前采用写请求> 原请求;
  bool 成功() const noexcept;
};
struct 存在类成员引用 final {
  稳定编码 成员关系{};
  稳定编码 目标结点{};
  friend bool operator==(const 存在类成员引用 &,
                         const 存在类成员引用 &) = default;
};

// 本类只承载实例存在的结构数据；需求、任务、场景和业务身份暂不进入本服务。
struct 存在类结点 final {
  稳定编码 结点{};
  std::vector<存在类成员引用> 子存在组;
  std::vector<存在类成员引用> 特征组;
  std::uint64_t 创建事实代次 = 0;
  std::optional<std::uint64_t> 退出事实代次;
  friend bool operator==(const 存在类结点 &, const 存在类结点 &) = default;
  std::vector<存在当前采用事实> 当前采用组;
};



struct 存在类查询请求 final {
  std::uint32_t 合同版本 = 存在类数据合同版本;
  std::uint64_t 期望事实代次 = 0;
  稳定编码 存在结点{};
  friend bool operator==(const 存在类查询请求 &,
                         const 存在类查询请求 &) = default;
};

struct 存在类成员新增请求 final {
  std::uint32_t 合同版本 = 存在类数据合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 存在结点{};
  存在类成员种类 成员种类 = 存在类成员种类::子存在;
  稳定编码 目标结点{};
  friend bool operator==(const 存在类成员新增请求 &,
                         const 存在类成员新增请求 &) = default;
};

struct 存在类成员删除请求 final {
  std::uint32_t 合同版本 = 存在类数据合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 存在结点{};
  存在类成员种类 成员种类 = 存在类成员种类::子存在;
  稳定编码 目标结点{};
  friend bool operator==(const 存在类成员删除请求 &,
                         const 存在类成员删除请求 &) = default;
};

struct 存在类删除请求 final {
  std::uint32_t 合同版本 = 存在类数据合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 存在结点{};
  friend bool operator==(const 存在类删除请求 &,
                         const 存在类删除请求 &) = default;
};

struct 存在类结点结果 final {
  存在类数据状态 状态 = 存在类数据状态::入口拒绝;
  std::uint32_t 合同版本 = 存在类数据合同版本;
  std::uint64_t 事实代次 = 0;
  std::optional<存在类结点> 存在结点;

  bool 成功() const noexcept {
    return (状态 == 存在类数据状态::已创建 ||
            状态 == 存在类数据状态::精确重复 ||
            状态 == 存在类数据状态::已读取 ||
            状态 == 存在类数据状态::已添加成员 ||
            状态 == 存在类数据状态::已移除成员 ||
            状态 == 存在类数据状态::已退出) &&
           合同版本 == 存在类数据合同版本 && 事实代次 != 0 &&
           存在结点.has_value();
  }
};

inline constexpr std::uint32_t 任务虚拟存在专用合同版本 = 1;

enum class 任务虚拟存在专用状态 : std::uint8_t {
  已形成写集 = 1,
  已读取 = 2,
  未初始化 = 3,
  已初始化 = 4,
  入口拒绝 = 5,
  任务所有者未找到 = 6,
  任务所有者已退出 = 7,
  任务节点未找到 = 8,
  任务节点已退出 = 9,
  关系类型未找到 = 10,
  关系类型已退出 = 11,
  所有者不匹配 = 12,
  结构冲突 = 13,
  事实代次漂移 = 14,
  资源失败 = 15,
  内部不一致 = 16
};

struct 任务虚拟存在写集规格请求 final {
  std::uint32_t 合同版本 = 任务虚拟存在专用合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1结构所有者身份 任务所有者;
  L1所有者范围写入幂等身份 写入幂等身份;
  稳定编码 任务节点{};
  稳定编码 任务虚拟存在关系类型{};
  friend bool operator==(const 任务虚拟存在写集规格请求 &,
                         const 任务虚拟存在写集规格请求 &) = default;
};

struct 任务虚拟存在查询请求 final {
  std::uint32_t 合同版本 = 任务虚拟存在专用合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1结构所有者身份 任务所有者;
  稳定编码 任务节点{};
  稳定编码 任务虚拟存在关系类型{};
  friend bool operator==(const 任务虚拟存在查询请求 &,
                         const 任务虚拟存在查询请求 &) = default;
};

struct 任务虚拟存在投影 final {
  稳定编码 任务节点{};
  稳定编码 虚拟存在节点{};
  稳定编码 任务虚拟存在关系{};
  std::uint64_t 创建事实代次 = 0;
  friend bool operator==(const 任务虚拟存在投影 &,
                         const 任务虚拟存在投影 &) = default;
};

inline bool 任务虚拟存在写集完整(const L1所有者范围写集请求 &写集) noexcept {
  using namespace 存在类数据内部;
  if (写集.合同版本 != L1所有者范围CRUD合同版本 || 写集.期望事实代次 == 0 ||
      !有效(写集.写入幂等身份) || 写集.节点.size() != 1 ||
      写集.关系.size() != 1 || !写集.值.empty() || !写集.属性槽变更.empty() ||
      !写集.退出事实.empty())
    return false;
  const auto &节点 = 写集.节点.front();
  const auto &关系 = 写集.关系.front();
  const auto *源 = std::get_if<稳定编码>(&关系.源节点);
  const auto *目标 = std::get_if<L1所有者范围写集本地键>(&关系.目标节点);
  const auto *类型 = std::get_if<稳定编码>(&关系.关系类型节点);
  return 节点.本地键 == 任务虚拟存在结点本地键 && 节点.种类 == 节点种类::普通 &&
         !节点.属性类型表示 && 关系.本地键 == 任务虚拟存在关系本地键 && 源 &&
         有效(*源) && 目标 && *目标 == 任务虚拟存在结点本地键 && 类型 &&
         有效(*类型) && 关系.角色或顺序 == 0;
}

struct 任务虚拟存在写集规格结果 final {
  任务虚拟存在专用状态 状态 = 任务虚拟存在专用状态::入口拒绝;
  std::uint32_t 合同版本 = 任务虚拟存在专用合同版本;
  std::uint64_t 事实代次 = 0;
  L1结构所有者身份 任务所有者;
  L1所有者范围写入幂等身份 写入幂等身份;
  std::optional<L1所有者范围写集请求> 写集;
  bool 成功() const noexcept {
    return 状态 == 任务虚拟存在专用状态::已形成写集 &&
           合同版本 == 任务虚拟存在专用合同版本 && 事实代次 != 0 &&
           有效(任务所有者) && 有效(写入幂等身份) && 写集 &&
           写集->期望事实代次 == 事实代次 &&
           写集->写入幂等身份 == 写入幂等身份 && 任务虚拟存在写集完整(*写集);
  }
  friend bool operator==(const 任务虚拟存在写集规格结果 &,
                         const 任务虚拟存在写集规格结果 &) = default;
};

struct 任务虚拟存在查询结果 final {
  任务虚拟存在专用状态 状态 = 任务虚拟存在专用状态::入口拒绝;
  std::uint32_t 合同版本 = 任务虚拟存在专用合同版本;
  std::uint64_t 事实代次 = 0;
  L1结构所有者身份 任务所有者;
  std::optional<任务虚拟存在投影> 投影;
  bool 成功() const noexcept {
    return 状态 == 任务虚拟存在专用状态::已读取 &&
           合同版本 == 任务虚拟存在专用合同版本 && 事实代次 != 0 &&
           有效(任务所有者) && 投影 && 有效(投影->任务节点) &&
           有效(投影->虚拟存在节点) && 有效(投影->任务虚拟存在关系) &&
           投影->创建事实代次 != 0 && 投影->创建事实代次 <= 事实代次;
  }
  friend bool operator==(const 任务虚拟存在查询结果 &,
                         const 任务虚拟存在查询结果 &) = default;
};

inline constexpr std::uint32_t 方法虚拟存在专用合同版本 = 1;

enum class 方法虚拟存在专用状态 : std::uint8_t {
  已形成写集 = 1,
  已读取 = 2,
  未初始化 = 3,
  已初始化 = 4,
  入口拒绝 = 5,
  方法所有者未找到 = 6,
  方法所有者已退出 = 7,
  方法节点未找到 = 8,
  方法节点已退出 = 9,
  关系类型未找到 = 10,
  关系类型已退出 = 11,
  所有者不匹配 = 12,
  结构冲突 = 13,
  事实代次漂移 = 14,
  资源失败 = 15,
  内部不一致 = 16
};

struct 方法虚拟存在写集规格请求 final {
  std::uint32_t 合同版本 = 方法虚拟存在专用合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1结构所有者身份 方法所有者;
  L1所有者范围写入幂等身份 写入幂等身份;
  稳定编码 方法节点{};
  稳定编码 方法虚拟存在关系类型{};
  friend bool operator==(const 方法虚拟存在写集规格请求 &,
                         const 方法虚拟存在写集规格请求 &) = default;
};

struct 方法虚拟存在查询请求 final {
  std::uint32_t 合同版本 = 方法虚拟存在专用合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1结构所有者身份 方法所有者;
  稳定编码 方法节点{};
  稳定编码 方法虚拟存在关系类型{};
  friend bool operator==(const 方法虚拟存在查询请求 &,
                         const 方法虚拟存在查询请求 &) = default;
};

struct 方法虚拟存在投影 final {
  稳定编码 方法节点{};
  稳定编码 虚拟存在节点{};
  稳定编码 方法虚拟存在关系{};
  std::uint64_t 创建事实代次 = 0;
  friend bool operator==(const 方法虚拟存在投影 &,
                         const 方法虚拟存在投影 &) = default;
};

inline bool 方法虚拟存在写集完整(const L1所有者范围写集请求 &写集) noexcept {
  using namespace 存在类数据内部;
  if (写集.合同版本 != L1所有者范围CRUD合同版本 || 写集.期望事实代次 == 0 ||
      !有效(写集.写入幂等身份) || 写集.节点.size() != 1 ||
      写集.关系.size() != 1 || !写集.值.empty() || !写集.属性槽变更.empty() ||
      !写集.退出事实.empty())
    return false;
  const auto &节点 = 写集.节点.front();
  const auto &关系 = 写集.关系.front();
  const auto *源 = std::get_if<稳定编码>(&关系.源节点);
  const auto *目标 = std::get_if<L1所有者范围写集本地键>(&关系.目标节点);
  const auto *类型 = std::get_if<稳定编码>(&关系.关系类型节点);
  return 节点.本地键 == 方法虚拟存在结点本地键 && 节点.种类 == 节点种类::普通 &&
         !节点.属性类型表示 && 关系.本地键 == 方法虚拟存在关系本地键 && 源 &&
         有效(*源) && 目标 && *目标 == 方法虚拟存在结点本地键 && 类型 &&
         有效(*类型) && 关系.角色或顺序 == 0;
}

struct 方法虚拟存在写集规格结果 final {
  方法虚拟存在专用状态 状态 = 方法虚拟存在专用状态::入口拒绝;
  std::uint32_t 合同版本 = 方法虚拟存在专用合同版本;
  std::uint64_t 事实代次 = 0;
  L1结构所有者身份 方法所有者;
  L1所有者范围写入幂等身份 写入幂等身份;
  std::optional<L1所有者范围写集请求> 写集;
  bool 成功() const noexcept {
    return 状态 == 方法虚拟存在专用状态::已形成写集 &&
           合同版本 == 方法虚拟存在专用合同版本 && 事实代次 != 0 &&
           有效(方法所有者) && 有效(写入幂等身份) && 写集 &&
           写集->期望事实代次 == 事实代次 &&
           写集->写入幂等身份 == 写入幂等身份 && 方法虚拟存在写集完整(*写集);
  }
  friend bool operator==(const 方法虚拟存在写集规格结果 &,
                         const 方法虚拟存在写集规格结果 &) = default;
};

struct 方法虚拟存在查询结果 final {
  方法虚拟存在专用状态 状态 = 方法虚拟存在专用状态::入口拒绝;
  std::uint32_t 合同版本 = 方法虚拟存在专用合同版本;
  std::uint64_t 事实代次 = 0;
  L1结构所有者身份 方法所有者;
  std::optional<方法虚拟存在投影> 投影;
  bool 成功() const noexcept {
    return 状态 == 方法虚拟存在专用状态::已读取 &&
           合同版本 == 方法虚拟存在专用合同版本 && 事实代次 != 0 &&
           有效(方法所有者) && 投影 && 有效(投影->方法节点) &&
           有效(投影->虚拟存在节点) && 有效(投影->方法虚拟存在关系) &&
           投影->创建事实代次 != 0 && 投影->创建事实代次 <= 事实代次;
  }
  friend bool operator==(const 方法虚拟存在查询结果 &,
                         const 方法虚拟存在查询结果 &) = default;
};

inline constexpr std::uint32_t 任务方法实例参数规格专用合同版本 = 1;

enum class 任务方法实例参数规格专用状态 : std::uint8_t {
  已形成写集 = 1,
  已读取 = 2,
  未实例化 = 3,
  已实例化 = 4,
  入口拒绝 = 5,
  任务所有者未找到 = 6,
  任务所有者已退出 = 7,
  任务节点未找到 = 8,
  任务节点已退出 = 9,
  任务虚拟存在未初始化 = 10,
  方法节点未找到 = 11,
  方法节点已退出 = 12,
  方法虚拟存在未初始化 = 13,
  参数特征类型未找到 = 14,
  参数特征类型已退出 = 15,
  参数特征类型不是属性类型 = 16,
  参数类别无效 = 17,
  所有者不匹配 = 18,
  关系类型未找到 = 19,
  关系类型已退出 = 20,
  结构冲突 = 21,
  数量预算不足 = 22,
  事实代次漂移 = 23,
  资源失败 = 24,
  内部不一致 = 25
};

enum class 方法参数规格类别 : std::uint8_t { 场景 = 1, 存在 = 2, 特征 = 3 };

struct 任务方法实例参数规格结构类型 final {
  稳定编码 任务虚拟存在关系类型{}, 方法虚拟存在关系类型{};
  稳定编码 方法场景参数关系类型{}, 方法存在参数关系类型{},
      方法特征参数关系类型{};
  稳定编码 实例宿主关系类型{}, 实例来源方法关系类型{},
      实例来源虚拟存在关系类型{};
  稳定编码 实例场景参数关系类型{}, 实例存在参数关系类型{},
      实例特征参数关系类型{};
  稳定编码 实例场景参数数据关系类型{}, 实例存在参数数据关系类型{};
  稳定编码 实例特征参数数据关系类型{};
  friend bool operator==(const 任务方法实例参数规格结构类型 &,
                         const 任务方法实例参数规格结构类型 &) = default;
};

struct 方法参数规格来源项 final {
  方法参数规格类别 类别 = 方法参数规格类别::场景;
  稳定编码 来源关系{}, 参数特征类型{};
  std::uint64_t 顺序 = 0;
  friend bool operator==(const 方法参数规格来源项 &,
                         const 方法参数规格来源项 &) = default;
};

struct 任务方法实例参数规格来源 final {
  稳定编码 方法节点{}, 方法虚拟存在节点{}, 方法虚拟存在关系{};
  std::vector<方法参数规格来源项> 参数规格组;
  friend bool operator==(const 任务方法实例参数规格来源 &,
                         const 任务方法实例参数规格来源 &) = default;
};

struct 任务实例参数规格关系项 final {
  方法参数规格类别 类别 = 方法参数规格类别::场景;
  稳定编码 规格关系{}, 参数特征类型{};
  std::uint64_t 顺序 = 0;
  std::optional<稳定编码> 数据关系;
  std::optional<稳定编码> 特征节点;
  friend bool operator==(const 任务实例参数规格关系项 &,
                         const 任务实例参数规格关系项 &) = default;
};

struct 任务方法实例参数规格投影 final {
  稳定编码 任务节点{}, 任务虚拟存在节点{}, 实例节点{}, 实例宿主关系{};
  稳定编码 来源方法关系{}, 来源虚拟存在关系{};
  任务方法实例参数规格来源 来源;
  std::vector<任务实例参数规格关系项> 实例参数规格组;
  std::uint64_t 创建事实代次 = 0;
  friend bool operator==(const 任务方法实例参数规格投影 &,
                         const 任务方法实例参数规格投影 &) = default;
};

struct 任务方法实例参数规格写集规格请求 final {
  std::uint32_t 合同版本 = 任务方法实例参数规格专用合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1结构所有者身份 任务所有者;
  L1所有者范围写入幂等身份 写入幂等身份;
  稳定编码 任务节点{};
  任务方法实例参数规格来源 来源;
  任务方法实例参数规格结构类型 结构类型;
  std::uint64_t 最大已有实例数量 = 0, 每实例最大参数数量 = 0;
};

struct 任务方法实例参数规格组查询请求 final {
  std::uint32_t 合同版本 = 任务方法实例参数规格专用合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1结构所有者身份 任务所有者;
  稳定编码 任务节点{};
  任务方法实例参数规格结构类型 结构类型;
  std::uint64_t 最大实例数量 = 0, 每实例最大参数数量 = 0;
};

struct 任务实例参数绑定写集规格请求 final {
  std::uint32_t 合同版本 = 任务方法实例参数规格专用合同版本;
  std::uint64_t 期望事实代次 = 0;
  L1结构所有者身份 任务所有者;
  L1所有者范围写入幂等身份 写入幂等身份;
  稳定编码 任务节点{}, 方法节点{};
  方法参数规格类别 类别 = 方法参数规格类别::场景;
  std::uint64_t 顺序 = 0;
  稳定编码 参数特征类型{}, 特征节点{};
  任务方法实例参数规格结构类型 结构类型;
  std::uint64_t 最大实例数量 = 0, 每实例最大参数数量 = 0;
};

struct 任务方法实例参数规格写集规格结果 final {
  任务方法实例参数规格专用状态 状态 = 任务方法实例参数规格专用状态::入口拒绝;
  std::uint32_t 合同版本 = 任务方法实例参数规格专用合同版本;
  std::uint64_t 事实代次 = 0;
  L1结构所有者身份 任务所有者;
  L1所有者范围写入幂等身份 写入幂等身份;
  std::optional<L1所有者范围写集请求> 写集;
  bool 成功() const noexcept {
    return 状态 == 任务方法实例参数规格专用状态::已形成写集 &&
           合同版本 == 任务方法实例参数规格专用合同版本 && 事实代次 != 0 &&
           有效(任务所有者) && 有效(写入幂等身份) && 写集.has_value();
  }
};

struct 任务方法实例参数规格组结果 final {
  任务方法实例参数规格专用状态 状态 = 任务方法实例参数规格专用状态::入口拒绝;
  std::uint32_t 合同版本 = 任务方法实例参数规格专用合同版本;
  std::uint64_t 事实代次 = 0;
  L1结构所有者身份 任务所有者;
  稳定编码 任务节点{};
  std::vector<任务方法实例参数规格投影> 投影组;
  bool 成功() const noexcept {
    return 状态 == 任务方法实例参数规格专用状态::已读取 &&
           合同版本 == 任务方法实例参数规格专用合同版本 && 事实代次 != 0 &&
           有效(任务所有者) && 有效(任务节点);
  }
};

struct 存在历史读取请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0;
  std::uint64_t H = 0;
  稳定编码 宿主{};
  std::uint64_t 关系预算 = 0;
};
struct 存在历史读取结果 final {
  存在历史读取状态 状态 = 存在历史读取状态::入口拒绝;
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0;
  std::uint64_t H = 0;
  std::optional<存在类结点> 存在;
  bool 成功() const noexcept {
    return 状态 == 存在历史读取状态::已读取 && 合同版本 == 1 && H &&
           Gread >= H && 存在 && 有效(存在->结点) && !存在->退出事实代次;
  }
};
struct 存在特征成员历史请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0;
  std::uint64_t H = 0;
  稳定编码 宿主{}, 成员关系{}, 特征实例{};
};
struct 存在特征成员历史结果 final {
  存在历史读取状态 状态 = 存在历史读取状态::入口拒绝;
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0;
  std::uint64_t H = 0;
  稳定编码 宿主{};
  std::optional<存在类成员引用> 成员;
  bool 成功() const noexcept {
    return 状态 == 存在历史读取状态::已读取 && 合同版本 == 1 && H &&
           Gread >= H && 有效(宿主) && 成员 && 有效(成员->成员关系) &&
           有效(成员->目标结点);
  }
};

class 存在类数据服务 final : public 存在结构身份只读提供者,
                             public 存在组成结构只读提供者, public 定位特征已知参与者,
                             public 绑定存在内容参与者, public 世界树根存在参与者 {
public:
  bool 绑定于(const L1事实基座服务 &s) const noexcept override {
    return &s == &第一层服务_;
  }
  const L1事实基座服务 &世界树根底座() const noexcept override {
    return 第一层服务_;
  }
  L1结构所有者身份 世界树根所有者() const noexcept override {
    return 写入端口_.所有者身份();
  }
  bool 世界树根幂等键可用(L1所有者范围写入幂等身份 k) const noexcept override {
    return k.值 && k.值 != 1 && k != 存在族来源初始化幂等身份 &&
           (k.值 >> 48) != 0x4E43;
  }
  bool 世界树根结构已就绪() const noexcept override { return 存在结构登记已就绪(); }
  世界树根存在片段
  准备世界树根存在片段(const 世界树根初始化请求 &r) const override {
    世界树根存在片段 out;
    out.写集 = {{1}, 所有者_, {L1所有者范围CRUD合同版本, r.G0, r.存在幂等身份}};
    out.写集.写集.节点.push_back({{1}, 节点种类::普通, std::nullopt});
    out.写集.写集.关系.push_back({{2}, L1所有者范围写集本地键{1},
                                  存在族锚点_, 存在族归属关系类型_, 1});
    return out;
  }
  L1所有者范围首次写入读取结果 读取世界树根存在首次材料(
      L1所有者范围写入幂等身份 k) const override {
    return 写入端口_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, k});
  }
  std::optional<世界树根存在来源>
  读取世界树根存在来源(std::uint64_t g, std::uint64_t h,
                         稳定编码 e) const override {
    const auto r = 读取存在身份来源历史见证(g, h, e);
    if (!r.成功(g, h, e) || !r.见证)
      return std::nullopt;
    return 世界树根存在来源{g, h, e, *r.见证};
  }
  存在组成读取结果 读取当前组成父(const 存在组成父读取请求 &r) const override {
    存在组成读取结果 o;
    o.Gread = r.G0;
    if (r.版本 != 1 || !r.G0 || !有效(r.子存在) || r.最大关系数量 < 1 ||
        r.最大关系数量 > 4096)
      return o;
    try {
      const auto id = 确认当前存在结构身份(r.G0, r.子存在);
      if (id.状态 != 存在结构身份只读状态::已读取) {
        o.状态 = id.状态;
        o.Gread = id.Gread;
        return o;
      }
      auto q = 第一层服务_.读取所有者范围历史关系组(
          {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标, r.子存在,
           子存在关系类型_, r.G0});
      if (q.读取事实代次 != r.G0) {
        o.状态 = 存在结构身份只读状态::事实代次漂移;
        o.Gread = q.读取事实代次;
        return o;
      }
      if (q.状态 != L1所有者范围读取状态::成功) {
        o.状态 = 映射窄读取状态(q.状态);
        return o;
      }
      if (q.合同版本 != L1所有者范围CRUD合同版本 ||
          q.方向 != L1所有者范围关系端点方向::目标 || q.端点节点 != r.子存在 ||
          q.关系类型节点 != 子存在关系类型_ || q.历史截止事实代次 != r.G0) {
        o.状态 = 存在结构身份只读状态::内部不一致;
        return o;
      }
      if (q.关系组.size() > r.最大关系数量) {
        o.状态 = 存在结构身份只读状态::数量预算不足;
        return o;
      }
      std::sort(
          q.关系组.begin(), q.关系组.end(),
          [](const auto &a, const auto &b) { return a.编码.值 < b.编码.值; });
      for (const auto &e : q.关系组) {
        if (!有效(e.编码) || !有效(e.源节点) || e.目标节点 != r.子存在 ||
            e.关系类型节点 != 子存在关系类型_ || e.写入所有者 != 所有者_ ||
            e.角色或顺序 != 1 || !e.创建事实代次 || e.退出事实代次) {
          o.状态 = 存在结构身份只读状态::内部不一致;
          return o;
        }
      }
      if (q.关系组.size() > 1) {
        o.状态 = 存在结构身份只读状态::内部不一致;
        return o;
      }
      if (!q.关系组.empty()) {
        const auto &e = q.关系组.front();
        o.父 = 存在组成关系事实{r.G0,          r.G0,       e.编码,
                                e.源节点,      e.目标节点, e.创建事实代次,
                                e.退出事实代次};
      }
      o.状态 = 存在结构身份只读状态::已读取;
    } catch (const std::bad_alloc &) {
      o.状态 = 存在结构身份只读状态::资源失败;
    } catch (...) {
      o.状态 = 存在结构身份只读状态::内部不一致;
    }
    return o;
  }

  存在组成读取结果
  读取当前组成子组(const 存在组成子组读取请求 &r) const override {
    存在组成读取结果 o;
    o.Gread = r.G0;
    if (r.版本 != 1 || !r.G0 || !有效(r.父存在) || r.最大关系数量 < 1 ||
        r.最大关系数量 > 4096)
      return o;
    try {
      const auto id = 确认当前存在结构身份(r.G0, r.父存在);
      if (id.状态 != 存在结构身份只读状态::已读取) {
        o.状态 = id.状态;
        o.Gread = id.Gread;
        return o;
      }
      auto q = 第一层服务_.读取所有者范围历史关系组(
          {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, r.父存在,
           子存在关系类型_, r.G0});
      if (q.读取事实代次 != r.G0) {
        o.状态 = 存在结构身份只读状态::事实代次漂移;
        o.Gread = q.读取事实代次;
        return o;
      }
      if (q.状态 != L1所有者范围读取状态::成功) {
        o.状态 = 映射窄读取状态(q.状态);
        return o;
      }
      if (q.合同版本 != L1所有者范围CRUD合同版本 ||
          q.方向 != L1所有者范围关系端点方向::源 || q.端点节点 != r.父存在 ||
          q.关系类型节点 != 子存在关系类型_ || q.历史截止事实代次 != r.G0 ||
          q.关系组.size() > r.最大关系数量) {
        o.状态 = q.关系组.size() > r.最大关系数量
                     ? 存在结构身份只读状态::数量预算不足
                     : 存在结构身份只读状态::内部不一致;
        return o;
      }
      std::sort(
          q.关系组.begin(), q.关系组.end(),
          [](const auto &a, const auto &b) { return a.编码.值 < b.编码.值; });
      for (const auto &e : q.关系组) {
        if (!有效(e.编码) || e.源节点 != r.父存在 || !有效(e.目标节点) ||
            e.关系类型节点 != 子存在关系类型_ || e.写入所有者 != 所有者_ ||
            e.角色或顺序 != 1 || !e.创建事实代次 || e.退出事实代次) {
          o.状态 = 存在结构身份只读状态::内部不一致;
          o.子组.clear();
          return o;
        }
        o.子组.push_back({r.G0, r.G0, e.编码, e.源节点, e.目标节点,
                          e.创建事实代次, e.退出事实代次});
      }
      o.状态 = 存在结构身份只读状态::已读取;
    } catch (const std::bad_alloc &) {
      o.状态 = 存在结构身份只读状态::资源失败;
      o.子组.clear();
    } catch (...) {
      o.状态 = 存在结构身份只读状态::内部不一致;
      o.子组.clear();
    }
    return o;
  }
  存在历史读取结果 读取存在历史事实(const 存在历史读取请求 &r) const {
    return 读取存在历史事实核心(r, true);
  }

private:
  friend class 世界树根数据服务;
  L1所有者范围写端口 &世界树根协调端口() noexcept { return 写入端口_; }
  存在历史读取结果 读取存在历史事实核心(const 存在历史读取请求 &r,
                                        bool 包含当前采用) const {
    using S = 存在历史读取状态;
    存在历史读取结果 o;
    o.Gread = r.Gread;
    o.H = r.H;
    if (r.合同版本 != 1 || !r.H || r.H > r.Gread || !有效(r.宿主) ||
        !r.关系预算)
      return o;
    try {
      const auto 身份 = 读取存在身份来源历史见证(r.Gread, r.H, r.宿主);
      if (!身份.成功(r.Gread, r.H, r.宿主)) {
        o.状态 = 映射窄历史状态(身份.状态);
        o.Gread = 身份.Gread;
        o.H = 身份.H;
        return o;
      }
      auto first = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
      if (first.状态 != L1中性读取状态::成功) {
        o.状态 = first.状态 == L1中性读取状态::资源失败 ? S::资源失败
                                                        : S::内部不一致;
        return o;
      }
      if (first.事实代次 != r.Gread) {
        o.状态 = S::事实代次漂移;
        o.Gread = first.事实代次;
        return o;
      }
      auto nr = 第一层服务_.读取所有者范围历史事实(
          {L1所有者范围CRUD合同版本, r.宿主});
      if (nr.状态 != L1所有者范围读取状态::成功) {
        o.状态 = 存在历史状态(nr.状态);
        return o;
      }
      if (nr.读取事实代次 != r.Gread) {
        o.状态 = S::事实代次漂移;
        o.Gread = nr.读取事实代次;
        return o;
      }
      auto n = nr.事实 ? std::get_if<L1所有者范围节点事实>(&*nr.事实) : nullptr;
      if (nr.合同版本 != L1所有者范围CRUD合同版本 || nr.查询编码 != r.宿主 ||
          !n || n->编码 != r.宿主 || n->写入所有者 != 所有者_ ||
          n->种类 != 节点种类::普通 || n->属性类型表示) {
        o.状态 = S::引用冲突;
        return o;
      }
      if (!n->创建事实代次 || n->创建事实代次 > r.H) {
        o.状态 = S::未找到;
        return o;
      }
      if (n->退出事实代次 && *n->退出事实代次 <= r.H) {
        o.状态 = S::目标已退出;
        return o;
      }
      存在类结点 snapshot;
      snapshot.结点 = r.宿主;
      snapshot.创建事实代次 = n->创建事实代次;
      if (!n->当前属性.empty()) {
        o.状态 = S::引用冲突;
        return o;
      }
      const std::vector<std::pair<存在类成员种类, 稳定编码>> kinds{
          {存在类成员种类::子存在, 子存在关系类型_},
          {存在类成员种类::特征, 特征关系类型_}};
      std::uint64_t count = 0;
      for (const auto &[kind, type] : kinds) {
        auto rr = 第一层服务_.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, r.宿主,
             type, r.H});
        if (rr.状态 != L1所有者范围读取状态::成功) {
          o.状态 = 存在历史状态(rr.状态);
          return o;
        }
        if (rr.读取事实代次 != r.Gread) {
          o.状态 = S::事实代次漂移;
          o.Gread = rr.读取事实代次;
          return o;
        }
        if (rr.合同版本 != L1所有者范围CRUD合同版本 ||
            rr.方向 != L1所有者范围关系端点方向::源 || rr.端点节点 != r.宿主 ||
            rr.关系类型节点 != type || rr.历史截止事实代次 != r.H ||
            !规范化关系组(rr.关系组, r.宿主, type, r.H, std::nullopt)) {
          o.状态 = S::内部不一致;
          return o;
        }
        if (rr.关系组.size() > r.关系预算 - count) {
          o.状态 = S::数量预算不足;
          return o;
        }
        count += rr.关系组.size();
        auto &members = 选择成员组(snapshot, kind);
        for (const auto &edge : rr.关系组) {
          if (!edge.创建事实代次) {
            o.状态 = S::内部不一致;
            return o;
          }
          members.push_back({edge.编码, edge.目标节点});
        }
      }
      auto last = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
      if (last.状态 != L1中性读取状态::成功) {
        o.状态 =
            last.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致;
        return o;
      }
      if (last.事实代次 != r.Gread) {
        o.状态 = S::事实代次漂移;
        o.Gread = last.事实代次;
        return o;
      }
      if (包含当前采用 && 有效(当前采用关系类型_)) {
        try {
          snapshot.当前采用组 =
              读取采用组(r.宿主, r.Gread, r.H, r.关系预算).采用;
          if (snapshot.当前采用组.size() > r.关系预算 - count) {
            o.状态 = S::数量预算不足;
            return o;
          }
        } catch (const 采用失败 &e) {
          switch (e.状态) {
          case 采用S::数量预算不足:
            o.状态 = S::数量预算不足;
            break;
          case 采用S::资源失败:
            o.状态 = S::资源失败;
            break;
          case 采用S::事实代次漂移:
            o.状态 = S::事实代次漂移;
            break;
          case 采用S::历史材料已清理:
            o.状态 = S::历史材料不可用;
            break;
          case 采用S::未找到:
            o.状态 = S::未找到;
            break;
          case 采用S::目标已退出:
            o.状态 = S::目标已退出;
            break;
          case 采用S::引用冲突:
            o.状态 = S::引用冲突;
            break;
          default:
            o.状态 = S::内部不一致;
            break;
          }
          return o;
        }
      }
      o.状态 = S::已读取;
      o.存在 = std::move(snapshot);
      return o;
    } catch (const std::bad_alloc &) {
      o.状态 = S::资源失败;
      return o;
    } catch (...) {
      o.状态 = S::内部不一致;
      return o;
    }
  }

public:
  存在特征成员历史结果
  读取特征成员历史事实(const 存在特征成员历史请求 &r) const {
    using S = 存在历史读取状态;
    存在特征成员历史结果 o;
    o.Gread = r.Gread;
    o.H = r.H;
    o.宿主 = r.宿主;
    if (r.合同版本 != 1 || !r.H || r.H > r.Gread || !有效(r.宿主) ||
        !有效(r.成员关系) || !有效(r.特征实例))
      return o;
    try {
      auto nr = 第一层服务_.读取所有者范围历史事实(
          {L1所有者范围CRUD合同版本, r.宿主});
      auto rr = 第一层服务_.读取所有者范围历史事实(
          {L1所有者范围CRUD合同版本, r.成员关系});
      if (nr.状态 != L1所有者范围读取状态::成功) {
        o.状态 = 存在历史状态(nr.状态);
        return o;
      }
      if (rr.状态 != L1所有者范围读取状态::成功) {
        o.状态 = 存在历史状态(rr.状态);
        return o;
      }
      if (nr.读取事实代次 != r.Gread || rr.读取事实代次 != r.Gread) {
        o.状态 = S::事实代次漂移;
        return o;
      }
      auto n = nr.事实 ? std::get_if<L1所有者范围节点事实>(&*nr.事实) : nullptr;
      auto e = rr.事实 ? std::get_if<L1所有者范围关系事实>(&*rr.事实) : nullptr;
      if (nr.合同版本 != L1所有者范围CRUD合同版本 ||
          rr.合同版本 != L1所有者范围CRUD合同版本 || nr.查询编码 != r.宿主 ||
          rr.查询编码 != r.成员关系 || !n || !e || n->编码 != r.宿主 ||
          n->写入所有者 != 所有者_ || n->种类 != 节点种类::普通 ||
          n->属性类型表示 || e->编码 != r.成员关系 ||
          e->写入所有者 != 所有者_ || e->源节点 != r.宿主 ||
          e->目标节点 != r.特征实例 || e->关系类型节点 != 特征关系类型_ ||
          e->角色或顺序 != 1) {
        o.状态 = S::引用冲突;
        return o;
      }
      if (!n->创建事实代次 || !e->创建事实代次 || n->创建事实代次 > r.H ||
          e->创建事实代次 > r.H) {
        o.状态 = S::未找到;
        return o;
      }
      if ((n->退出事实代次 && *n->退出事实代次 <= r.H) ||
          (e->退出事实代次 && *e->退出事实代次 <= r.H)) {
        o.状态 = S::目标已退出;
        return o;
      }
      auto last = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
      if (last.状态 != L1中性读取状态::成功) {
        o.状态 =
            last.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致;
        return o;
      }
      if (last.事实代次 != r.Gread) {
        o.状态 = S::事实代次漂移;
        return o;
      }
      o.状态 = S::已读取;
      o.成员 = 存在类成员引用{e->编码, e->目标节点};
      return o;
    } catch (const std::bad_alloc &) {
      o.状态 = S::资源失败;
      return o;
    } catch (...) {
      o.状态 = S::内部不一致;
      return o;
    }
  }

  存在类数据服务() = delete;
  存在类数据服务(const 存在类数据服务 &) = delete;
  存在类数据服务 &operator=(const 存在类数据服务 &) = delete;
  存在类数据服务(存在类数据服务 &&) = delete;
  存在类数据服务 &operator=(存在类数据服务 &&) = delete;

  存在类数据服务(const L1事实基座服务 &第一层服务,
                 const 特征类数据服务 &特征服务, L1所有者范围写端口 &&写入端口,
                 稳定编码 子存在关系类型, 稳定编码 特征关系类型,
                 const 存在当前采用结构交付 &当前采用结构)
      : 第一层服务_(第一层服务), 特征服务_(特征服务),
        写入端口_(std::move(写入端口)), 所有者_(写入端口_.所有者身份()),
        子存在关系类型_(子存在关系类型), 特征关系类型_(特征关系类型),
        当前采用关系类型_(当前采用结构.当前采用关系类型) {
    if (!写入端口_.有效() || !写入端口_.绑定于(第一层服务_) ||
        !特征服务_.绑定于(第一层服务_) || !有效(所有者_) || !关系类型组有效())
      throw std::invalid_argument("invalid existence data configuration");
    const auto 所有者读取 =
        第一层服务_.读取当前结构所有者({L1所有者范围CRUD合同版本, 所有者_});
    if (所有者读取.状态 != L1所有者范围读取状态::成功 ||
        所有者读取.合同版本 != L1所有者范围CRUD合同版本 ||
        !所有者读取.所有者事实 || 所有者读取.所有者事实->所有者 != 所有者_ ||
        所有者读取.所有者事实->范围种类 != L1所有者范围种类::独占结构范围 ||
        所有者读取.所有者事实->退出事实代次)
      throw std::invalid_argument("invalid existence data owner");
    if (!关系类型有效(子存在关系类型_) || !关系类型有效(特征关系类型_) ||
        !关系类型有效(当前采用关系类型_))
      throw std::invalid_argument("invalid existence relation types");
    初始化存在族来源();
    if (存在族锚点_ == 子存在关系类型_ || 存在族锚点_ == 特征关系类型_ ||
        存在族锚点_ == 当前采用关系类型_ ||
        存在族归属关系类型_ == 子存在关系类型_ ||
        存在族归属关系类型_ == 特征关系类型_ ||
        存在族归属关系类型_ == 当前采用关系类型_)
      throw std::invalid_argument("invalid existence family metadata");
  }

  存在当前身份确认结果 确认当前存在结构身份(std::uint64_t G0,
                                            稳定编码 身份) const override {
    if (G0 == 0 || !有效(身份))
      return 当前身份失败(存在结构身份只读状态::入口拒绝);
    try {
      L1所有者范围一致当前读取请求 请求;
      请求.合同版本 = L1所有者范围一致当前读取合同版本;
      请求.期望事实代次 = G0;
      请求.所有者 = {所有者_};
      请求.节点 = {身份};
      if (身份 != 存在族锚点_)
        请求.节点.push_back(存在族锚点_);
      if (身份 != 存在族归属关系类型_)
        请求.节点.push_back(存在族归属关系类型_);
      请求.源关系组 = {{身份, 存在族归属关系类型_}};
      const auto 读取 = 第一层服务_.尝试读取所有者范围一致当前投影(请求);
      if (读取.合同版本 != L1所有者范围一致当前读取合同版本 ||
          读取.期望事实代次 != G0)
        return 当前身份失败(存在结构身份只读状态::内部不一致,
                            读取.读取事实代次);
      if (读取.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
        return 当前身份失败(存在结构身份只读状态::事实代次漂移,
                            读取.读取事实代次);
      if (读取.状态 == L1所有者范围一致当前读取状态::资源失败)
        return 当前身份失败(存在结构身份只读状态::资源失败, 读取.读取事实代次);
      if (读取.状态 != L1所有者范围一致当前读取状态::成功 ||
          读取.读取事实代次 != G0)
        return 当前身份失败(存在结构身份只读状态::内部不一致,
                            读取.读取事实代次);
      if (!存在身份当前投影头完整(读取, 请求))
        return 当前身份失败(存在结构身份只读状态::内部不一致, G0);

      const auto *目标项 = 查找一致节点(读取, 身份);
      if (!目标项)
        return 当前身份失败(存在结构身份只读状态::内部不一致, G0);
      if (目标项->状态 == L1所有者范围一致当前读取项目状态::未找到)
        return 当前身份失败(存在结构身份只读状态::未找到, G0);
      if (目标项->状态 == L1所有者范围一致当前读取项目状态::已退出)
        return 当前身份失败(存在结构身份只读状态::目标已退出, G0);
      const auto *目标 = 当前节点事实(*目标项);
      if (!目标 || 目标->写入所有者 != 所有者_ || 身份 == 存在族锚点_ ||
          身份 == 存在族归属关系类型_)
        return 当前身份失败(存在结构身份只读状态::入口拒绝, G0);

      const auto *关系组 = 查找一致源关系组(读取, 身份, 存在族归属关系类型_);
      if (!关系组 || 关系组->成员.empty())
        return 当前身份失败(存在结构身份只读状态::入口拒绝, G0);
      if (!普通空节点当前有效(*目标, 身份))
        return 当前身份失败(存在结构身份只读状态::内部不一致, G0);
      const auto *锚点项 = 查找一致节点(读取, 存在族锚点_);
      const auto *类型项 = 查找一致节点(读取, 存在族归属关系类型_);
      const auto *锚点 = 锚点项 ? 当前节点事实(*锚点项) : nullptr;
      const auto *类型 = 类型项 ? 当前节点事实(*类型项) : nullptr;
      if (!锚点 || !类型 || !普通空节点当前有效(*锚点, 存在族锚点_) ||
          !普通空节点当前有效(*类型, 存在族归属关系类型_) ||
          关系组->成员.size() != 1)
        return 当前身份失败(存在结构身份只读状态::内部不一致, G0);
      const auto &关系 = 关系组->成员.front().关系;
      if (!族归属关系有效(关系, 身份, std::nullopt) ||
          关系组->成员.front().对端节点 != *锚点 ||
          关系.创建事实代次 != 目标->创建事实代次 ||
          锚点->创建事实代次 > 目标->创建事实代次 ||
          类型->创建事实代次 > 目标->创建事实代次)
        return 当前身份失败(存在结构身份只读状态::内部不一致, G0);
      return {存在结构身份只读状态::已读取, 存在结构身份只读合同版本, G0};
    } catch (const std::bad_alloc &) {
      return 当前身份失败(存在结构身份只读状态::资源失败);
    } catch (...) {
      return 当前身份失败(存在结构身份只读状态::内部不一致);
    }
  }

public:
  存在身份来源历史见证读取结果
  读取存在身份来源历史见证(std::uint64_t Gread, std::uint64_t H,
                           稳定编码 身份) const override {
    if (Gread == 0 || H == 0 || H > Gread || !有效(身份))
      return 历史身份失败(存在结构身份只读状态::入口拒绝);
    bool 读取守卫已成立 = false;
    try {
      const auto 前 = 读取当前事实代次();
      if (前.first != 存在结构身份只读状态::已读取)
        return 历史身份失败(前.first, 前.second, H);
      if (前.second != Gread)
        return 历史身份失败(存在结构身份只读状态::事实代次漂移, 前.second, H);
      读取守卫已成立 = true;

      const auto owner =
          第一层服务_.读取历史结构所有者({L1所有者范围CRUD合同版本, 所有者_});
      const auto e =
          第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 身份});
      const auto anchor = 第一层服务_.读取所有者范围历史事实(
          {L1所有者范围CRUD合同版本, 存在族锚点_});
      const auto type = 第一层服务_.读取所有者范围历史事实(
          {L1所有者范围CRUD合同版本, 存在族归属关系类型_});
      const auto links = 第一层服务_.读取所有者范围历史关系组(
          {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, 身份,
           存在族归属关系类型_, H});

      const auto 后 = 读取当前事实代次();
      if (后.first != 存在结构身份只读状态::已读取)
        return 历史身份失败(后.first, 后.second, H);
      if (后.second != Gread)
        return 历史身份失败(存在结构身份只读状态::事实代次漂移, 后.second, H);

      const auto 读取错误 =
          历史来源读取错误(owner, e, anchor, type, links, Gread, H, 身份);
      if (读取错误)
        return 历史身份失败(*读取错误, Gread, H);

      const auto &ownerFact = *owner.所有者事实;
      const auto &eFact = std::get<L1所有者范围节点事实>(*e.事实);
      const auto &anchorFact = std::get<L1所有者范围节点事实>(*anchor.事实);
      const auto &typeFact = std::get<L1所有者范围节点事实>(*type.事实);
      const auto &relation = links.关系组.front();
      存在身份来源历史见证 见证;
      见证.身份 = 身份;
      见证.族锚点 = 存在族锚点_;
      见证.族归属关系类型 = 存在族归属关系类型_;
      见证.族归属关系 = relation.编码;
      见证.角色 = 1;
      见证.节点生命周期 = {eFact.创建事实代次, std::nullopt};
      见证.族锚点生命周期 = {anchorFact.创建事实代次, std::nullopt};
      见证.关系类型生命周期 = {typeFact.创建事实代次, std::nullopt};
      见证.归属关系生命周期 = {relation.创建事实代次, std::nullopt};
      (void)ownerFact;
      存在身份来源历史见证读取结果 结果{存在结构身份只读状态::已读取,
                                        存在结构身份只读合同版本, Gread, H,
                                        std::move(见证)};
      return 结果.成功(Gread, H, 身份)
                 ? 结果
                 : 历史身份失败(存在结构身份只读状态::内部不一致, Gread, H);
    } catch (const std::bad_alloc &) {
      return 历史身份失败(存在结构身份只读状态::资源失败,
                          读取守卫已成立 ? Gread : 0, 读取守卫已成立 ? H : 0);
    } catch (...) {
      return 历史身份失败(存在结构身份只读状态::内部不一致,
                          读取守卫已成立 ? Gread : 0, 读取守卫已成立 ? H : 0);
    }
  }

  存在已知准确特征读取结果
  确认当前已知准确特征(const 存在已知准确特征当前请求 &r) const override {
    const 存在已知准确特征历史请求 history{r.版本, r.G0, r.G0, r.存在, r.特征};
    auto out = 读取已知准确特征历史(history);
    return out;
  }

  存在已知准确特征读取结果
  读取已知准确特征历史(const 存在已知准确特征历史请求 &r) const override {
    using S = 存在已知准确特征只读状态;
    auto fail = [&](S state, std::uint64_t g = 0, std::uint64_t h = 0) {
      return 存在已知准确特征读取结果{
          state,       存在已知准确特征只读合同版本, g, h, r.存在, r.特征,
          std::nullopt};
    };
    if (r.版本 != 存在已知准确特征只读合同版本 || r.Gread == 0 || r.H == 0 ||
        r.H > r.Gread || !有效(r.存在) || !有效(r.特征) || r.存在 == r.特征)
      return fail(S::入口拒绝);
    bool guardEstablished = false;
    try {
      const auto identity = 读取存在身份来源历史见证(r.Gread, r.H, r.存在);
      auto mapIdentity = [](存在结构身份只读状态 state) noexcept {
        switch (state) {
        case 存在结构身份只读状态::入口拒绝:
          return S::入口拒绝;
        case 存在结构身份只读状态::未找到:
          return S::未找到;
        case 存在结构身份只读状态::目标已退出:
          return S::目标已退出;
        case 存在结构身份只读状态::事实代次漂移:
          return S::事实代次漂移;
        case 存在结构身份只读状态::历史材料已清理:
          return S::历史材料已清理;
        case 存在结构身份只读状态::资源失败:
          return S::资源失败;
        default:
          return S::内部不一致;
        }
      };
      if (!identity.成功(r.Gread, r.H, r.存在))
        return fail(mapIdentity(identity.状态), identity.Gread, r.H);
      guardEstablished = true;

      const auto existence =
          读取存在历史事实核心({1, r.Gread, r.H, r.存在, 4096}, false);
      auto mapExistence = [](存在历史读取状态 state) noexcept {
        switch (state) {
        case 存在历史读取状态::入口拒绝:
          return S::入口拒绝;
        case 存在历史读取状态::未找到:
          return S::未找到;
        case 存在历史读取状态::目标已退出:
          return S::目标已退出;
        case 存在历史读取状态::事实代次漂移:
          return S::事实代次漂移;
        case 存在历史读取状态::历史材料不可用:
          return S::历史材料已清理;
        case 存在历史读取状态::资源失败:
          return S::资源失败;
        case 存在历史读取状态::数量预算不足:
          return S::数量预算不足;
        default:
          return S::内部不一致;
        }
      };
      if (!existence.成功() || existence.Gread != r.Gread ||
          existence.H != r.H || !existence.存在 ||
          existence.存在->结点 != r.存在)
        return fail(existence.状态 == 存在历史读取状态::已读取
                        ? S::内部不一致
                        : mapExistence(existence.状态),
                    existence.Gread, r.H);

      const 存在类成员引用 *known = nullptr;
      for (const auto &member : existence.存在->特征组) {
        if (member.目标结点 != r.特征)
          continue;
        if (known)
          return fail(S::内部不一致, r.Gread, r.H);
        known = &member;
      }
      if (!known)
        return fail(S::未找到, r.Gread, r.H);
      if (!有效(known->成员关系) || !有效(known->目标结点))
        return fail(S::内部不一致, r.Gread, r.H);

      const 有界准确特征读取请求 featureRequest{有界准确特征读取合同版本,
                                                r.Gread,
                                                r.H,
                                                特征信息身份{r.特征},
                                                {4096, 4096, 4096, 65536}};
      const auto feature = 特征服务_.读取有界准确特征事实(featureRequest);
      auto mapFeature = [](特征类标量状态 state) noexcept {
        switch (state) {
        case 特征类标量状态::入口拒绝:
          return S::入口拒绝;
        case 特征类标量状态::未找到:
          return S::未找到;
        case 特征类标量状态::已退出:
          return S::目标已退出;
        case 特征类标量状态::事实代次漂移:
          return S::事实代次漂移;
        case 特征类标量状态::历史材料不可用:
          return S::历史材料已清理;
        case 特征类标量状态::预算不足:
          return S::数量预算不足;
        case 特征类标量状态::资源失败:
          return S::资源失败;
        default:
          return S::内部不一致;
        }
      };
      if (!feature.成功() || !feature.事实 ||
          feature.原请求.版本 != featureRequest.版本 ||
          feature.原请求.Gread != r.Gread || feature.原请求.H != r.H ||
          feature.原请求.身份.编码 != r.特征 ||
          feature.事实->Gread != r.Gread || feature.事实->H != r.H ||
          feature.事实->信息.身份.编码 != r.特征) {
        auto observed = r.Gread;
        if (feature.状态 == 特征类标量状态::事实代次漂移) {
          const auto current = 读取当前事实代次();
          observed = current.first == 存在结构身份只读状态::已读取
                         ? current.second
                         : 0;
        }
        return fail(feature.状态 == 特征类标量状态::已读取
                        ? S::内部不一致
                        : mapFeature(feature.状态),
                    observed, r.H);
      }

      const auto last = 读取当前事实代次();
      if (last.first != 存在结构身份只读状态::已读取)
        return fail(mapIdentity(last.first), last.second, r.H);
      if (last.second != r.Gread)
        return fail(S::事实代次漂移, last.second, r.H);

      存在已知准确特征读取结果 out{S::已读取,
                                   存在已知准确特征只读合同版本,
                                   r.Gread,
                                   r.H,
                                   r.存在,
                                   r.特征,
                                   存在已知准确特征见证{known->成员关系, r.特征,
                                                        feature.事实->创建G,
                                                        feature.事实->退出G}};
      return out.历史成功(r) ? out : fail(S::内部不一致, r.Gread, r.H);
    } catch (const std::bad_alloc &) {
      return fail(S::资源失败, guardEstablished ? r.Gread : 0, r.H);
    } catch (const std::length_error &) {
      return fail(S::资源失败, guardEstablished ? r.Gread : 0, r.H);
    } catch (...) {
      return fail(S::内部不一致, guardEstablished ? r.Gread : 0, r.H);
    }
  }
  存在当前采用结果 读取当前采用(const 存在当前采用读取请求 &) const;
  存在当前采用结果 变更当前采用(const 存在当前采用写请求 &);
  存在当前采用结果 收敛当前采用(const 存在当前采用写请求 &r) {
    return 变更当前采用(r);
  }
  任务虚拟存在写集规格结果
  形成任务虚拟存在写集规格(const 任务虚拟存在写集规格请求 &请求) const {
    if (请求.合同版本 != 任务虚拟存在专用合同版本 || 请求.期望事实代次 == 0 ||
        !有效(请求.任务所有者) || !有效(请求.写入幂等身份) ||
        !有效(请求.任务节点) || !有效(请求.任务虚拟存在关系类型))
      return 规格失败(任务虚拟存在专用状态::入口拒绝, 请求);
    try {
      const auto 代次 = 读取任务虚拟存在当前代次();
      if (代次.状态 != 任务虚拟存在专用状态::已读取)
        return 规格失败(代次.状态, 请求, 代次.事实代次);
      if (代次.事实代次 != 请求.期望事实代次)
        return 规格失败(任务虚拟存在专用状态::事实代次漂移, 请求,
                        代次.事实代次);
      if (const auto 状态 = 验证任务所有者(请求.任务所有者, 请求.期望事实代次))
        return 规格失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证任务当前节点(
              请求.任务所有者, 请求.任务节点, 请求.期望事实代次,
              任务虚拟存在专用状态::任务节点未找到,
              任务虚拟存在专用状态::任务节点已退出))
        return 规格失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证任务当前节点(
              请求.任务所有者, 请求.任务虚拟存在关系类型, 请求.期望事实代次,
              任务虚拟存在专用状态::关系类型未找到,
              任务虚拟存在专用状态::关系类型已退出))
        return 规格失败(*状态, 请求, 请求.期望事实代次);
      const auto 当前 = 查询任务虚拟存在(
          {任务虚拟存在专用合同版本, 请求.期望事实代次, 请求.任务所有者,
           请求.任务节点, 请求.任务虚拟存在关系类型});
      if (当前.状态 == 任务虚拟存在专用状态::已读取)
        return 规格失败(任务虚拟存在专用状态::已初始化, 请求, 当前.事实代次);
      if (当前.状态 != 任务虚拟存在专用状态::未初始化)
        return 规格失败(当前.状态, 请求, 当前.事实代次);
      L1所有者范围写集请求 写集;
      写集.合同版本 = L1所有者范围CRUD合同版本;
      写集.期望事实代次 = 请求.期望事实代次;
      写集.写入幂等身份 = 请求.写入幂等身份;
      写集.节点 = {{存在类数据内部::任务虚拟存在结点本地键, 节点种类::普通,
                    std::nullopt}};
      写集.关系 = {{存在类数据内部::任务虚拟存在关系本地键, 请求.任务节点,
                    存在类数据内部::任务虚拟存在结点本地键,
                    请求.任务虚拟存在关系类型, 0}};
      const auto 读后 = 读取任务虚拟存在当前代次();
      if (读后.状态 != 任务虚拟存在专用状态::已读取)
        return 规格失败(读后.状态, 请求, 读后.事实代次);
      if (读后.事实代次 != 请求.期望事实代次)
        return 规格失败(任务虚拟存在专用状态::事实代次漂移, 请求,
                        读后.事实代次);
      任务虚拟存在写集规格结果 结果{任务虚拟存在专用状态::已形成写集,
                                    任务虚拟存在专用合同版本,
                                    请求.期望事实代次,
                                    请求.任务所有者,
                                    请求.写入幂等身份,
                                    std::move(写集)};
      return 结果.成功() ? 结果
                         : 规格失败(任务虚拟存在专用状态::内部不一致, 请求,
                                    请求.期望事实代次);
    } catch (const std::bad_alloc &) {
      return 规格失败(任务虚拟存在专用状态::资源失败, 请求);
    } catch (const std::length_error &) {
      return 规格失败(任务虚拟存在专用状态::资源失败, 请求);
    } catch (...) {
      return 规格失败(任务虚拟存在专用状态::内部不一致, 请求);
    }
  }

  任务虚拟存在查询结果
  查询任务虚拟存在(const 任务虚拟存在查询请求 &请求) const {
    if (请求.合同版本 != 任务虚拟存在专用合同版本 || 请求.期望事实代次 == 0 ||
        !有效(请求.任务所有者) || !有效(请求.任务节点) ||
        !有效(请求.任务虚拟存在关系类型))
      return 查询规格失败(任务虚拟存在专用状态::入口拒绝, 请求);
    try {
      const auto 代次 = 读取任务虚拟存在当前代次();
      if (代次.状态 != 任务虚拟存在专用状态::已读取)
        return 查询规格失败(代次.状态, 请求, 代次.事实代次);
      if (代次.事实代次 != 请求.期望事实代次)
        return 查询规格失败(任务虚拟存在专用状态::事实代次漂移, 请求,
                            代次.事实代次);
      if (const auto 状态 = 验证任务所有者(请求.任务所有者, 请求.期望事实代次))
        return 查询规格失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证任务当前节点(
              请求.任务所有者, 请求.任务节点, 请求.期望事实代次,
              任务虚拟存在专用状态::任务节点未找到,
              任务虚拟存在专用状态::任务节点已退出))
        return 查询规格失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证任务当前节点(
              请求.任务所有者, 请求.任务虚拟存在关系类型, 请求.期望事实代次,
              任务虚拟存在专用状态::关系类型未找到,
              任务虚拟存在专用状态::关系类型已退出))
        return 查询规格失败(*状态, 请求, 请求.期望事实代次);
      const auto 关系组 = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本, 请求.任务节点, 请求.任务虚拟存在关系类型});
      if (关系组.读取事实代次 != 请求.期望事实代次)
        return 查询规格失败(任务虚拟存在专用状态::事实代次漂移, 请求,
                            关系组.读取事实代次);
      if (关系组.状态 != L1所有者范围读取状态::成功)
        return 查询规格失败(
            映射任务虚拟存在L1状态(关系组.状态,
                                   任务虚拟存在专用状态::任务节点未找到,
                                   任务虚拟存在专用状态::任务节点已退出),
            请求, 关系组.读取事实代次);
      if (关系组.合同版本 != L1所有者范围CRUD合同版本 ||
          关系组.源节点 != 请求.任务节点 ||
          关系组.关系类型节点 != 请求.任务虚拟存在关系类型)
        return 查询规格失败(任务虚拟存在专用状态::内部不一致, 请求,
                            关系组.读取事实代次);
      if (关系组.关系组.empty())
        return 查询规格失败(任务虚拟存在专用状态::未初始化, 请求,
                            请求.期望事实代次);
      if (关系组.关系组.size() != 1)
        return 查询规格失败(任务虚拟存在专用状态::结构冲突, 请求,
                            请求.期望事实代次);
      const auto &关系候选 = 关系组.关系组.front();
      if (!有效(关系候选.目标节点))
        return 查询规格失败(任务虚拟存在专用状态::结构冲突, 请求,
                            请求.期望事实代次);
      L1所有者范围一致当前读取请求 投影请求;
      投影请求.合同版本 = L1所有者范围一致当前读取合同版本;
      投影请求.期望事实代次 = 请求.期望事实代次;
      投影请求.所有者 = {请求.任务所有者};
      投影请求.节点 = {请求.任务节点, 关系候选.目标节点,
                       请求.任务虚拟存在关系类型};
      投影请求.关系 = {关系候选.编码};
      const auto 投影 = 第一层服务_.尝试读取所有者范围一致当前投影(投影请求);
      if (投影.状态 != L1所有者范围一致当前读取状态::成功)
        return 查询规格失败(映射任务虚拟存在一致状态(投影.状态), 请求,
                            投影.读取事实代次);
      if (!任务虚拟存在投影头完整(投影, 投影请求))
        return 查询规格失败(任务虚拟存在专用状态::内部不一致, 请求,
                            投影.读取事实代次);
      const auto *任务 = 查找任务虚拟存在节点(投影, 请求.任务节点);
      const auto *Vt = 查找任务虚拟存在节点(投影, 关系候选.目标节点);
      const auto *类型 = 查找任务虚拟存在节点(投影, 请求.任务虚拟存在关系类型);
      const auto *关系 = 查找任务虚拟存在关系(投影, 关系候选.编码);
      if (!任务 || !Vt || !类型 || !关系 ||
          !任务虚拟存在节点完整(*任务, 请求.任务所有者) ||
          !任务虚拟存在节点完整(*Vt, 请求.任务所有者) ||
          !任务虚拟存在节点完整(*类型, 请求.任务所有者) ||
          关系->编码 != 关系候选.编码 || 关系->源节点 != 请求.任务节点 ||
          关系->目标节点 != Vt->编码 ||
          关系->关系类型节点 != 请求.任务虚拟存在关系类型 ||
          关系->角色或顺序 != 0 || 关系->写入所有者 != 请求.任务所有者 ||
          关系->退出事实代次 || 关系->创建事实代次 == 0 ||
          关系->创建事实代次 != Vt->创建事实代次)
        return 查询规格失败(任务虚拟存在专用状态::结构冲突, 请求,
                            投影.读取事实代次);
      const auto 读后 = 读取任务虚拟存在当前代次();
      if (读后.状态 != 任务虚拟存在专用状态::已读取)
        return 查询规格失败(读后.状态, 请求, 读后.事实代次);
      if (读后.事实代次 != 请求.期望事实代次)
        return 查询规格失败(任务虚拟存在专用状态::事实代次漂移, 请求,
                            读后.事实代次);
      任务虚拟存在查询结果 结果{任务虚拟存在专用状态::已读取,
                                任务虚拟存在专用合同版本, 请求.期望事实代次,
                                请求.任务所有者,
                                任务虚拟存在投影{请求.任务节点, Vt->编码,
                                                 关系->编码, Vt->创建事实代次}};
      return 结果.成功() ? 结果
                         : 查询规格失败(任务虚拟存在专用状态::内部不一致, 请求,
                                        请求.期望事实代次);
    } catch (const std::bad_alloc &) {
      return 查询规格失败(任务虚拟存在专用状态::资源失败, 请求);
    } catch (const std::length_error &) {
      return 查询规格失败(任务虚拟存在专用状态::资源失败, 请求);
    } catch (...) {
      return 查询规格失败(任务虚拟存在专用状态::内部不一致, 请求);
    }
  }

  方法虚拟存在写集规格结果
  形成方法虚拟存在写集规格(const 方法虚拟存在写集规格请求 &请求) const {
    if (请求.合同版本 != 方法虚拟存在专用合同版本 || 请求.期望事实代次 == 0 ||
        !有效(请求.方法所有者) || !有效(请求.写入幂等身份) ||
        !有效(请求.方法节点) || !有效(请求.方法虚拟存在关系类型))
      return 方法规格失败(方法虚拟存在专用状态::入口拒绝, 请求);
    try {
      const auto 代次 = 读取方法虚拟存在当前代次();
      if (代次.状态 != 方法虚拟存在专用状态::已读取 ||
          代次.事实代次 != 请求.期望事实代次)
        return 方法规格失败(代次.状态 == 方法虚拟存在专用状态::已读取
                                ? 方法虚拟存在专用状态::事实代次漂移
                                : 代次.状态,
                            请求, 代次.事实代次);
      if (const auto 状态 = 验证方法所有者(请求.方法所有者, 请求.期望事实代次))
        return 方法规格失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证方法当前节点(
              请求.方法所有者, 请求.方法节点, 请求.期望事实代次,
              方法虚拟存在专用状态::方法节点未找到,
              方法虚拟存在专用状态::方法节点已退出))
        return 方法规格失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证方法当前节点(
              请求.方法所有者, 请求.方法虚拟存在关系类型, 请求.期望事实代次,
              方法虚拟存在专用状态::关系类型未找到,
              方法虚拟存在专用状态::关系类型已退出))
        return 方法规格失败(*状态, 请求, 请求.期望事实代次);
      const auto 当前 = 查询方法虚拟存在(
          {方法虚拟存在专用合同版本, 请求.期望事实代次, 请求.方法所有者,
           请求.方法节点, 请求.方法虚拟存在关系类型});
      if (当前.状态 == 方法虚拟存在专用状态::已读取)
        return 方法规格失败(方法虚拟存在专用状态::已初始化, 请求,
                            当前.事实代次);
      if (当前.状态 != 方法虚拟存在专用状态::未初始化)
        return 方法规格失败(当前.状态, 请求, 当前.事实代次);
      L1所有者范围写集请求 写集;
      写集.合同版本 = L1所有者范围CRUD合同版本;
      写集.期望事实代次 = 请求.期望事实代次;
      写集.写入幂等身份 = 请求.写入幂等身份;
      写集.节点 = {{存在类数据内部::方法虚拟存在结点本地键, 节点种类::普通,
                    std::nullopt}};
      写集.关系 = {{存在类数据内部::方法虚拟存在关系本地键, 请求.方法节点,
                    存在类数据内部::方法虚拟存在结点本地键,
                    请求.方法虚拟存在关系类型, 0}};
      const auto 读后 = 读取方法虚拟存在当前代次();
      if (读后.状态 != 方法虚拟存在专用状态::已读取 ||
          读后.事实代次 != 请求.期望事实代次)
        return 方法规格失败(读后.状态 == 方法虚拟存在专用状态::已读取
                                ? 方法虚拟存在专用状态::事实代次漂移
                                : 读后.状态,
                            请求, 读后.事实代次);
      方法虚拟存在写集规格结果 结果{方法虚拟存在专用状态::已形成写集,
                                    方法虚拟存在专用合同版本,
                                    请求.期望事实代次,
                                    请求.方法所有者,
                                    请求.写入幂等身份,
                                    std::move(写集)};
      return 结果.成功() ? 结果
                         : 方法规格失败(方法虚拟存在专用状态::内部不一致, 请求,
                                        请求.期望事实代次);
    } catch (const std::bad_alloc &) {
      return 方法规格失败(方法虚拟存在专用状态::资源失败, 请求);
    } catch (const std::length_error &) {
      return 方法规格失败(方法虚拟存在专用状态::资源失败, 请求);
    } catch (...) {
      return 方法规格失败(方法虚拟存在专用状态::内部不一致, 请求);
    }
  }

  方法虚拟存在查询结果
  查询方法虚拟存在(const 方法虚拟存在查询请求 &请求) const {
    if (请求.合同版本 != 方法虚拟存在专用合同版本 || 请求.期望事实代次 == 0 ||
        !有效(请求.方法所有者) || !有效(请求.方法节点) ||
        !有效(请求.方法虚拟存在关系类型))
      return 方法查询失败(方法虚拟存在专用状态::入口拒绝, 请求);
    try {
      const auto 代次 = 读取方法虚拟存在当前代次();
      if (代次.状态 != 方法虚拟存在专用状态::已读取 ||
          代次.事实代次 != 请求.期望事实代次)
        return 方法查询失败(代次.状态 == 方法虚拟存在专用状态::已读取
                                ? 方法虚拟存在专用状态::事实代次漂移
                                : 代次.状态,
                            请求, 代次.事实代次);
      if (const auto 状态 = 验证方法所有者(请求.方法所有者, 请求.期望事实代次))
        return 方法查询失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证方法当前节点(
              请求.方法所有者, 请求.方法节点, 请求.期望事实代次,
              方法虚拟存在专用状态::方法节点未找到,
              方法虚拟存在专用状态::方法节点已退出))
        return 方法查询失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证方法当前节点(
              请求.方法所有者, 请求.方法虚拟存在关系类型, 请求.期望事实代次,
              方法虚拟存在专用状态::关系类型未找到,
              方法虚拟存在专用状态::关系类型已退出))
        return 方法查询失败(*状态, 请求, 请求.期望事实代次);
      const auto 出边 = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本, 请求.方法节点, 请求.方法虚拟存在关系类型});
      if (出边.读取事实代次 != 请求.期望事实代次)
        return 方法查询失败(方法虚拟存在专用状态::事实代次漂移, 请求,
                            出边.读取事实代次);
      if (出边.状态 != L1所有者范围读取状态::成功 ||
          出边.合同版本 != L1所有者范围CRUD合同版本 ||
          出边.源节点 != 请求.方法节点 ||
          出边.关系类型节点 != 请求.方法虚拟存在关系类型)
        return 方法查询失败(映射方法虚拟存在L1状态(
                                出边.状态, 方法虚拟存在专用状态::方法节点未找到,
                                方法虚拟存在专用状态::方法节点已退出),
                            请求, 出边.读取事实代次);
      if (出边.关系组.empty())
        return 方法查询失败(方法虚拟存在专用状态::未初始化, 请求,
                            请求.期望事实代次);
      if (出边.关系组.size() != 1)
        return 方法查询失败(方法虚拟存在专用状态::结构冲突, 请求,
                            请求.期望事实代次);
      const auto &关系 = 出边.关系组.front();
      L1所有者范围一致当前读取请求 投影请求;
      投影请求.合同版本 = L1所有者范围一致当前读取合同版本;
      投影请求.期望事实代次 = 请求.期望事实代次;
      投影请求.所有者 = {请求.方法所有者};
      投影请求.节点 = {请求.方法节点, 关系.目标节点, 请求.方法虚拟存在关系类型};
      投影请求.关系 = {关系.编码};
      const auto 投影 = 第一层服务_.尝试读取所有者范围一致当前投影(投影请求);
      if (投影.状态 != L1所有者范围一致当前读取状态::成功)
        return 方法查询失败(映射方法虚拟存在一致状态(投影.状态), 请求,
                            投影.读取事实代次);
      if (!方法虚拟存在投影头完整(投影, 投影请求))
        return 方法查询失败(方法虚拟存在专用状态::内部不一致, 请求,
                            投影.读取事实代次);
      const auto *方法 = 查找方法虚拟存在节点(投影, 请求.方法节点);
      const auto *Vm = 查找方法虚拟存在节点(投影, 关系.目标节点);
      const auto *类型 = 查找方法虚拟存在节点(投影, 请求.方法虚拟存在关系类型);
      const auto *投影关系 = 查找方法虚拟存在关系(投影, 关系.编码);
      if (!方法 || !Vm || !类型 || !投影关系 ||
          !方法虚拟存在节点完整(*方法, 请求.方法所有者) ||
          !方法虚拟存在节点完整(*Vm, 请求.方法所有者) ||
          !方法虚拟存在节点完整(*类型, 请求.方法所有者) ||
          投影关系->源节点 != 请求.方法节点 || 投影关系->目标节点 != Vm->编码 ||
          投影关系->关系类型节点 != 请求.方法虚拟存在关系类型 ||
          投影关系->角色或顺序 != 0 ||
          投影关系->写入所有者 != 请求.方法所有者 || 投影关系->退出事实代次 ||
          投影关系->创建事实代次 == 0 ||
          投影关系->创建事实代次 != Vm->创建事实代次)
        return 方法查询失败(方法虚拟存在专用状态::结构冲突, 请求,
                            请求.期望事实代次);
      const auto 入边 = 第一层服务_.读取所有者范围当前目标关系组(
          {L1所有者范围CRUD合同版本, Vm->编码, 请求.方法虚拟存在关系类型});
      if (入边.读取事实代次 != 请求.期望事实代次)
        return 方法查询失败(方法虚拟存在专用状态::事实代次漂移, 请求,
                            入边.读取事实代次);
      if (入边.状态 != L1所有者范围读取状态::成功 ||
          入边.合同版本 != L1所有者范围CRUD合同版本 ||
          入边.关系组.size() != 1 ||
          入边.关系组.front().编码 != 投影关系->编码 ||
          入边.关系组.front().源节点 != 请求.方法节点)
        return 方法查询失败(方法虚拟存在专用状态::结构冲突, 请求,
                            入边.读取事实代次);
      const auto 读后 = 读取方法虚拟存在当前代次();
      if (读后.状态 != 方法虚拟存在专用状态::已读取 ||
          读后.事实代次 != 请求.期望事实代次)
        return 方法查询失败(读后.状态 == 方法虚拟存在专用状态::已读取
                                ? 方法虚拟存在专用状态::事实代次漂移
                                : 读后.状态,
                            请求, 读后.事实代次);
      方法虚拟存在查询结果 结果{
          方法虚拟存在专用状态::已读取, 方法虚拟存在专用合同版本,
          请求.期望事实代次, 请求.方法所有者,
          方法虚拟存在投影{请求.方法节点, Vm->编码, 投影关系->编码,
                           Vm->创建事实代次}};
      return 结果.成功() ? 结果
                         : 方法查询失败(方法虚拟存在专用状态::内部不一致, 请求,
                                        请求.期望事实代次);
    } catch (const std::bad_alloc &) {
      return 方法查询失败(方法虚拟存在专用状态::资源失败, 请求);
    } catch (const std::length_error &) {
      return 方法查询失败(方法虚拟存在专用状态::资源失败, 请求);
    } catch (...) {
      return 方法查询失败(方法虚拟存在专用状态::内部不一致, 请求);
    }
  }

  任务方法实例参数规格写集规格结果 形成任务方法实例参数规格写集规格(
      const 任务方法实例参数规格写集规格请求 &请求) const {
    if (!实例规格请求有效(请求))
      return 实例规格失败(任务方法实例参数规格专用状态::入口拒绝, 请求);
    try {
      const auto 当前 =
          读取实例组内部({任务方法实例参数规格专用合同版本, 请求.期望事实代次,
                          请求.任务所有者, 请求.任务节点, 请求.结构类型,
                          请求.最大已有实例数量, 请求.每实例最大参数数量});
      if (!当前.成功())
        return 实例规格失败(当前.状态, 请求, 当前.事实代次);
      const auto 源验证 = 验证方法参数来源(
          请求.来源, 请求.结构类型, 请求.期望事实代次, 请求.每实例最大参数数量);
      if (源验证 != 任务方法实例参数规格专用状态::已读取)
        return 实例规格失败(源验证, 请求, 请求.期望事实代次);
      const auto it =
          std::find_if(当前.投影组.begin(), 当前.投影组.end(),
                       [&请求](const auto &投影) noexcept {
                         return 投影.来源.方法节点 == 请求.来源.方法节点;
                       });
      if (it != 当前.投影组.end())
        return 实例规格失败(it->来源 == 请求.来源
                                ? 任务方法实例参数规格专用状态::已实例化
                                : 任务方法实例参数规格专用状态::结构冲突,
                            请求, 当前.事实代次);
      using namespace 存在类数据内部;
      L1所有者范围写集请求 写集;
      写集.合同版本 = L1所有者范围CRUD合同版本;
      写集.期望事实代次 = 请求.期望事实代次;
      写集.写入幂等身份 = 请求.写入幂等身份;
      写集.节点 = {{任务方法实例节点本地键, 节点种类::普通, std::nullopt}};
      写集.关系 = {{任务方法实例宿主关系本地键, 当前任务Vm(请求),
                    任务方法实例节点本地键, 请求.结构类型.实例宿主关系类型, 0},
                   {任务方法实例来源方法关系本地键, 任务方法实例节点本地键,
                    请求.来源.方法节点, 请求.结构类型.实例来源方法关系类型, 0},
                   {任务方法实例来源Vm关系本地键, 任务方法实例节点本地键,
                    请求.来源.方法虚拟存在节点,
                    请求.结构类型.实例来源虚拟存在关系类型, 0}};
      for (std::size_t i = 0; i < 请求.来源.参数规格组.size(); ++i) {
        const auto &项 = 请求.来源.参数规格组[i];
        写集.关系.push_back({L1所有者范围写集本地键{static_cast<std::uint32_t>(
                                 任务方法实例参数首关系本地键.值 + i)},
                             任务方法实例节点本地键, 项.参数特征类型,
                             实例规格关系类型(项.类别, 请求.结构类型),
                             static_cast<std::int64_t>(项.顺序)});
      }
      return {任务方法实例参数规格专用状态::已形成写集,
              任务方法实例参数规格专用合同版本,
              请求.期望事实代次,
              请求.任务所有者,
              请求.写入幂等身份,
              std::move(写集)};
    } catch (const std::bad_alloc &) {
      return 实例规格失败(任务方法实例参数规格专用状态::资源失败, 请求);
    } catch (const std::length_error &) {
      return 实例规格失败(任务方法实例参数规格专用状态::资源失败, 请求);
    } catch (...) {
      return 实例规格失败(任务方法实例参数规格专用状态::内部不一致, 请求);
    }
  }

  任务方法实例参数规格写集规格结果
  形成任务实例参数绑定写集规格(const 任务实例参数绑定写集规格请求 &请求) const {
    if (!绑定请求有效(请求))
      return 绑定规格失败(任务方法实例参数规格专用状态::入口拒绝, 请求);
    try {
      const auto 当前 =
          读取实例组内部({任务方法实例参数规格专用合同版本, 请求.期望事实代次,
                          请求.任务所有者, 请求.任务节点, 请求.结构类型,
                          请求.最大实例数量, 请求.每实例最大参数数量});
      if (!当前.成功())
        return 绑定规格失败(当前.状态, 请求, 当前.事实代次);
      const auto 实例 =
          std::find_if(当前.投影组.begin(), 当前.投影组.end(),
                       [&请求](const auto &投影) noexcept {
                         return 投影.来源.方法节点 == 请求.方法节点;
                       });
      if (实例 == 当前.投影组.end())
        return 绑定规格失败(任务方法实例参数规格专用状态::未实例化, 请求,
                            当前.事实代次);
      const auto 槽 =
          std::find_if(实例->实例参数规格组.begin(), 实例->实例参数规格组.end(),
                       [&请求](const auto &项) noexcept {
                         return 项.类别 == 请求.类别 && 项.顺序 == 请求.顺序 &&
                                项.参数特征类型 == 请求.参数特征类型;
                       });
      if (槽 == 实例->实例参数规格组.end())
        return 绑定规格失败(任务方法实例参数规格专用状态::结构冲突, 请求,
                            当前.事实代次);
      if (槽->数据关系 || 槽->特征节点)
        return 绑定规格失败(槽->特征节点 == 请求.特征节点
                                ? 任务方法实例参数规格专用状态::已实例化
                                : 任务方法实例参数规格专用状态::结构冲突,
                            请求, 当前.事实代次);
      using namespace 存在类数据内部;
      L1所有者范围写集请求 写集;
      写集.合同版本 = L1所有者范围CRUD合同版本;
      写集.期望事实代次 = 请求.期望事实代次;
      写集.写入幂等身份 = 请求.写入幂等身份;
      写集.关系 = {{任务实例参数数据关系本地键, 实例->实例节点, 请求.特征节点,
                    实例数据关系类型(请求.类别, 请求.结构类型),
                    static_cast<std::int64_t>(请求.顺序)}};
      return {任务方法实例参数规格专用状态::已形成写集,
              任务方法实例参数规格专用合同版本,
              请求.期望事实代次,
              请求.任务所有者,
              请求.写入幂等身份,
              std::move(写集)};
    } catch (const std::bad_alloc &) {
      return 绑定规格失败(任务方法实例参数规格专用状态::资源失败, 请求);
    } catch (const std::length_error &) {
      return 绑定规格失败(任务方法实例参数规格专用状态::资源失败, 请求);
    } catch (...) {
      return 绑定规格失败(任务方法实例参数规格专用状态::内部不一致, 请求);
    }
  }

  任务方法实例参数规格组结果
  读取任务方法实例参数规格组(const 任务方法实例参数规格组查询请求 &请求) const {
    if (!实例查询请求有效(请求))
      return 实例查询失败(任务方法实例参数规格专用状态::入口拒绝, 请求);
    try {
      return 读取实例组内部(请求);
    } catch (const std::bad_alloc &) {
      return 实例查询失败(任务方法实例参数规格专用状态::资源失败, 请求);
    } catch (const std::length_error &) {
      return 实例查询失败(任务方法实例参数规格专用状态::资源失败, 请求);
    } catch (...) {
      return 实例查询失败(任务方法实例参数规格专用状态::内部不一致, 请求);
    }
  }



  存在类结点结果 查询存在(const 存在类查询请求 &请求) const {
    if (!查询请求有效(请求))
      return 失败(存在类数据状态::入口拒绝);
    try {
      return 读取当前存在(请求.存在结点, 请求.期望事实代次, true);
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::资源失败);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::资源失败);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致);
    }
  }

  // 增删成员关系就是对存在信息的修改；不换代存在身份。
  存在类结点结果 新增成员关系(const 存在类成员新增请求 &请求) {
    if (请求.成员种类 != 存在类成员种类::特征)
      return 失败(存在类数据状态::入口拒绝);
    if (!成员新增请求有效(请求))
      return 失败(存在类数据状态::入口拒绝);
    try {
      std::scoped_lock lock(写入锁_);
      if (const auto 重放 = 尝试重放新增成员(请求))
        return *重放;
      const auto 本体 = 读取当前存在本体(请求.存在结点, 请求.期望事实代次);
      if (!本体.成功() || 本体.状态 != 存在类数据状态::已读取)
        return 本体;
      const auto 成员读取 = 读取当前源关系组(
          请求.存在结点, 关系类型(请求.成员种类), 请求.期望事实代次);
      if (!成员读取.成功)
        return 失败(成员读取.状态, 成员读取.事实代次);
      if (std::any_of(成员读取.关系组.begin(), 成员读取.关系组.end(),
                      [&](const auto &关系) noexcept {
                        return 关系.目标节点 == 请求.目标结点;
                      }))
        return 失败(存在类数据状态::成员已存在, 请求.期望事实代次);
      if (const auto 状态 =
              验证成员目标(请求.成员种类, 请求.目标结点, 请求.期望事实代次))
        return 失败(*状态, 请求.期望事实代次);
      const auto 写集 = 形成新增成员写集(请求);
      return 提交成员变更(写集, 请求.幂等身份, 存在类数据状态::已添加成员,
                          请求.存在结点, 请求.成员种类, 请求.目标结点, true);
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::资源失败);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::资源失败);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致);
    }
  }

  存在类结点结果 删除成员关系(const 存在类成员删除请求 &请求) {
    if (请求.成员种类 != 存在类成员种类::特征)
      return 失败(存在类数据状态::入口拒绝);
    if (!成员删除请求有效(请求))
      return 失败(存在类数据状态::入口拒绝);
    try {
      std::scoped_lock lock(写入锁_);
      if (const auto 重放 = 尝试重放删除成员(请求))
        return *重放;
      const auto 本体 = 读取当前存在本体(请求.存在结点, 请求.期望事实代次);
      if (!本体.成功() || 本体.状态 != 存在类数据状态::已读取)
        return 本体;
      const auto 成员读取 = 读取当前源关系组(
          请求.存在结点, 关系类型(请求.成员种类), 请求.期望事实代次);
      if (!成员读取.成功)
        return 失败(成员读取.状态, 成员读取.事实代次);
      const auto it =
          std::find_if(成员读取.关系组.begin(), 成员读取.关系组.end(),
                       [&](const auto &关系) noexcept {
                         return 关系.目标节点 == 请求.目标结点;
                       });
      if (it == 成员读取.关系组.end())
        return 失败(存在类数据状态::成员未找到, 请求.期望事实代次);
      L1所有者范围写集请求 写集;
      写集.合同版本 = L1所有者范围CRUD合同版本;
      写集.期望事实代次 = 请求.期望事实代次;
      写集.写入幂等身份 = 请求.幂等身份;
      写集.退出事实 = {it->编码};
      if (请求.成员种类 == 存在类成员种类::特征 && 有效(当前采用关系类型_)) {
        try {
          const auto used =
              读取采用组(请求.存在结点, 请求.期望事实代次, 请求.期望事实代次,
                         std::numeric_limits<std::uint64_t>::max());
          if (std::any_of(
                  used.采用.begin(), used.采用.end(),
                  [&](const auto &a) { return a.F.编码 == 请求.目标结点; }))
            return 失败(存在类数据状态::引用冲突, 请求.期望事实代次);
        } catch (const 采用失败 &e) {
          return 失败(e.状态, 请求.期望事实代次);
        }
      }
      return 提交成员变更(写集, 请求.幂等身份, 存在类数据状态::已移除成员,
                          请求.存在结点, 请求.成员种类, 请求.目标结点, false);
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::资源失败);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::资源失败);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致);
    }
  }

  存在类结点结果 新增子存在关系(const 存在类成员新增请求 &请求,
                                const 直接归属联合只读提供者 &联合) {
    if (!成员新增请求有效(请求) || 请求.成员种类 != 存在类成员种类::子存在 ||
        !联合.绑定于(第一层服务_))
      return 失败(存在类数据状态::入口拒绝);
    try {
      {
        std::scoped_lock lock(写入锁_);
        if (const auto 重放 = 尝试重放新增成员(请求))
          return *重放;
      }
      const auto 校验 = 校验联合子存在变更(
          请求.期望事实代次, 请求.存在结点, 请求.目标结点, 联合, true);
      if (校验 != 存在类数据状态::已读取)
        return 失败(校验, 请求.期望事实代次);
      std::scoped_lock lock(写入锁_);
      if (const auto 重放 = 尝试重放新增成员(请求))
        return *重放;
      const auto 本体 = 读取当前存在本体(请求.存在结点, 请求.期望事实代次);
      if (!本体.成功() || 本体.状态 != 存在类数据状态::已读取)
        return 本体;
      if (const auto 状态 =
              验证成员目标(请求.成员种类, 请求.目标结点, 请求.期望事实代次))
        return 失败(*状态, 请求.期望事实代次);
      const auto 写集 = 形成新增成员写集(请求);
      return 提交成员变更(写集, 请求.幂等身份, 存在类数据状态::已添加成员,
                          请求.存在结点, 请求.成员种类, 请求.目标结点, true);
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::资源失败);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::资源失败);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致);
    }
  }

  存在类结点结果 删除子存在关系(const 存在类成员删除请求 &请求,
                                const 直接归属联合只读提供者 &联合) {
    if (!成员删除请求有效(请求) || 请求.成员种类 != 存在类成员种类::子存在 ||
        !联合.绑定于(第一层服务_))
      return 失败(存在类数据状态::入口拒绝);
    try {
      {
        std::scoped_lock lock(写入锁_);
        if (const auto 重放 = 尝试重放删除成员(请求))
          return *重放;
      }
      const auto 校验 = 校验联合子存在变更(
          请求.期望事实代次, 请求.存在结点, 请求.目标结点, 联合, false);
      if (校验 != 存在类数据状态::已读取)
        return 失败(校验, 请求.期望事实代次);
      std::scoped_lock lock(写入锁_);
      if (const auto 重放 = 尝试重放删除成员(请求))
        return *重放;
      const auto 本体 = 读取当前存在本体(请求.存在结点, 请求.期望事实代次);
      if (!本体.成功() || 本体.状态 != 存在类数据状态::已读取)
        return 本体;
      const auto 成员读取 =
          读取当前源关系组(请求.存在结点, 子存在关系类型_, 请求.期望事实代次);
      if (!成员读取.成功)
        return 失败(成员读取.状态, 成员读取.事实代次);
      const auto it = std::find_if(
          成员读取.关系组.begin(), 成员读取.关系组.end(),
          [&](const auto &e) { return e.目标节点 == 请求.目标结点; });
      if (it == 成员读取.关系组.end())
        return 失败(存在类数据状态::成员未找到, 请求.期望事实代次);
      L1所有者范围写集请求 写集{L1所有者范围CRUD合同版本, 请求.期望事实代次,
                                请求.幂等身份};
      写集.退出事实 = {it->编码};
      return 提交成员变更(写集, 请求.幂等身份, 存在类数据状态::已移除成员,
                          请求.存在结点, 请求.成员种类, 请求.目标结点, false);
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::资源失败);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::资源失败);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致);
    }
  }

  存在类结点结果 删除存在(const 存在类删除请求 &请求) {
    if (!删除请求有效(请求))
      return 失败(存在类数据状态::入口拒绝);
    try {
      std::scoped_lock lock(写入锁_);
      if (const auto 重放 = 尝试重放删除存在(请求))
        return *重放;
      const auto 当前 = 读取当前存在(请求.存在结点, 请求.期望事实代次, false);
      if (!当前.成功() || 当前.状态 != 存在类数据状态::已读取)
        return 当前;
      auto 写集 = 形成删除存在写集(请求, *当前.存在结点);
      const auto 族关系组 = 读取当前源关系组(请求.存在结点, 存在族归属关系类型_,
                                             请求.期望事实代次);
      if (!族关系组.成功)
        return 失败(族关系组.状态, 族关系组.事实代次);
      if (族关系组.关系组.size() != 1 ||
          !族归属关系有效(族关系组.关系组.front(), 请求.存在结点, std::nullopt))
        return 失败(存在类数据状态::内部不一致, 请求.期望事实代次);
      写集.退出事实.push_back(族关系组.关系组.front().编码);
      const auto 父关系 = 读取当前父关系(请求.存在结点, 请求.期望事实代次);
      if (!父关系.成功)
        return 失败(父关系.状态, 父关系.事实代次);
      for (const auto &关系 : 父关系.关系组)
        写集.退出事实.push_back(关系.编码);
      std::sort(
          写集.退出事实.begin(), 写集.退出事实.end(),
          [](稳定编码 左, 稳定编码 右) noexcept { return 左.值 < 右.值; });
      写集.退出事实.erase(
          std::unique(写集.退出事实.begin(), 写集.退出事实.end()),
          写集.退出事实.end());
      const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
      const auto 状态 = 映射写入状态(写入.状态, 存在类数据状态::已退出);
      if (状态 != 存在类数据状态::已退出 && 状态 != 存在类数据状态::精确重复)
        return 失败(状态, 写入.事实代次);
      try {
        if (!写入结果头完整(写入, 请求.幂等身份))
          return 失败(存在类数据状态::内部不一致, 写入.事实代次);
        if (!写入.新编码映射.empty() ||
            !退出事实同代闭合(写集.退出事实, 写入.事实代次))
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
        auto 读回 =
            读取历史存在快照(请求.存在结点, 写入.事实代次 - 1, 写入.事实代次,
                             存在类数据状态::已退出, 写入.事实代次);
        if (!读回.成功())
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
        const auto 当前身份 =
            确认当前存在结构身份(写入.事实代次, 请求.存在结点);
        if (当前身份.状态 != 存在结构身份只读状态::目标已退出 ||
            当前身份.Gread != 写入.事实代次)
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
        读回.状态 = 状态;
        return 读回;
      } catch (const std::bad_alloc &) {
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      } catch (const std::length_error &) {
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      } catch (...) {
        return 失败(存在类数据状态::内部不一致, 写入.事实代次);
      }
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::资源失败);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::资源失败);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致);
    }
  }

private:
  inline static constexpr L1所有者范围写入幂等身份 存在族来源初始化幂等身份{
      0x455849535446414DULL};
  inline static constexpr std::uint64_t 子存在联合校验预算 = 4096;

  static 存在类数据状态 联合失败状态(直接归属联合只读状态 s) noexcept {
    switch (s) {
    case 直接归属联合只读状态::事实代次漂移:
      return 存在类数据状态::事实代次漂移;
    case 直接归属联合只读状态::数量预算不足:
      return 存在类数据状态::数量预算不足;
    case 直接归属联合只读状态::历史材料已清理:
      return 存在类数据状态::历史材料已清理;
    case 直接归属联合只读状态::资源失败:
      return 存在类数据状态::资源失败;
    case 直接归属联合只读状态::入口拒绝:
      return 存在类数据状态::入口拒绝;
    default:
      return 存在类数据状态::内部不一致;
    }
  }

  存在类数据状态 校验联合子存在变更(std::uint64_t g, 稳定编码 parent,
                                    稳定编码 child,
                                    const 直接归属联合只读提供者 &joint,
                                    bool adding) const {
    const 直接归属联合父读取请求 pr{1, g, child, 子存在联合校验预算};
    const auto p = joint.读取当前联合父(pr);
    if (!p.父读取成功(pr))
      return 联合失败状态(p.状态);
    std::unordered_set<std::uint64_t> relationships;
    std::unordered_set<std::uint64_t> visited;
    auto overBudget = [&]() noexcept {
      return relationships.size() + visited.size() > 子存在联合校验预算;
    };
    if (p.父 && !relationships.insert(p.父->关系.值).second)
      return 存在类数据状态::内部不一致;
    if (overBudget())
      return 存在类数据状态::数量预算不足;
    if (adding) {
      if (p.父)
        return 存在类数据状态::引用冲突;
    } else {
      if (!p.父)
        return 存在类数据状态::成员未找到;
      if (p.父->来源 != 直接归属来源::存在组成 || p.父->父 != parent)
        return 存在类数据状态::引用冲突;
    }
    std::vector<稳定编码> pending{child};
    std::size_t cursor = 0;
    while (cursor < pending.size()) {
      const auto current = pending[cursor++];
      if (!visited.insert(current.值).second)
        return 存在类数据状态::引用冲突;
      if (overBudget())
        return 存在类数据状态::数量预算不足;
      if (adding && current == parent)
        return 存在类数据状态::引用冲突;
      const 直接归属场景角色读取请求 rr{1, g, current};
      const auto role = joint.读取当前场景角色位置(rr);
      if (!role.成功(rr))
        return 联合失败状态(role.状态);
      if (role.角色) {
        if (role.角色->树证明关系 &&
            !relationships.insert(role.角色->树证明关系->值).second)
          return 存在类数据状态::内部不一致;
        if (overBudget())
          return 存在类数据状态::数量预算不足;
        return 存在类数据状态::引用冲突;
      }
      const auto used = relationships.size() + visited.size();
      const auto remaining = used < 子存在联合校验预算
                                 ? 子存在联合校验预算 - used
                                 : std::uint64_t{1};
      const 直接归属联合子组读取请求 cr{1, g, current, remaining};
      const auto children = joint.读取当前联合子组(cr);
      if (!children.子组读取成功(cr))
        return 联合失败状态(children.状态);
      for (const auto &edge : children.子组) {
        if (!relationships.insert(edge.关系.值).second)
          return 存在类数据状态::内部不一致;
        if (overBudget())
          return 存在类数据状态::数量预算不足;
        pending.push_back(edge.成员);
      }
    }
    return 存在类数据状态::已读取;
  }

  using 一致节点项 = L1所有者范围一致节点读取结果项;
  using 一致源关系组项 = L1所有者范围一致源关系组读取结果项;

  static 存在当前身份确认结果 当前身份失败(存在结构身份只读状态 状态,
                                           std::uint64_t Gread = 0) noexcept {
    return {状态, 存在结构身份只读合同版本, Gread};
  }

  static 存在身份来源历史见证读取结果
  历史身份失败(存在结构身份只读状态 状态, std::uint64_t Gread = 0,
               std::uint64_t H = 0) noexcept {
    return {状态, 存在结构身份只读合同版本, Gread, H, std::nullopt};
  }

  std::pair<存在结构身份只读状态, std::uint64_t> 读取当前事实代次() const {
    const auto 读取 = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (读取.状态 == L1中性读取状态::成功 &&
        读取.合同版本 == L1中性CRUD合同版本 && 读取.事实代次 != 0)
      return {存在结构身份只读状态::已读取, 读取.事实代次};
    if (读取.状态 == L1中性读取状态::资源失败)
      return {存在结构身份只读状态::资源失败, 读取.事实代次};
    return {存在结构身份只读状态::内部不一致, 读取.事实代次};
  }

  static const 一致节点项 *
  查找一致节点(const L1所有者范围一致当前读取结果 &读取,
               稳定编码 编码) noexcept {
    const auto it = std::find_if(
        读取.节点.begin(), 读取.节点.end(),
        [&](const auto &v) noexcept { return v.查询编码 == 编码; });
    return it == 读取.节点.end() ? nullptr : &*it;
  }

  static const 一致源关系组项 *
  查找一致源关系组(const L1所有者范围一致当前读取结果 &读取, 稳定编码 源节点,
                   稳定编码 关系类型) noexcept {
    const auto it =
        std::find_if(读取.源关系组.begin(), 读取.源关系组.end(),
                     [&](const auto &v) noexcept {
                       return v.源节点 == 源节点 && v.关系类型节点 == 关系类型;
                     });
    return it == 读取.源关系组.end() ? nullptr : &*it;
  }

  static const L1所有者范围节点事实 *
  当前节点事实(const 一致节点项 &项) noexcept {
    return 项.状态 == L1所有者范围一致当前读取项目状态::成功 && 项.事实 &&
                   !项.物理清理事实代次 && !项.物理清理墓碑
               ? &*项.事实
               : nullptr;
  }

  bool 存在身份当前投影头完整(
      const L1所有者范围一致当前读取结果 &读取,
      const L1所有者范围一致当前读取请求 &请求) const noexcept {
    if (读取.状态 != L1所有者范围一致当前读取状态::成功 ||
        读取.合同版本 != L1所有者范围一致当前读取合同版本 ||
        读取.期望事实代次 != 请求.期望事实代次 ||
        读取.读取事实代次 != 请求.期望事实代次 || 读取.所有者.size() != 1 ||
        读取.节点.size() != 请求.节点.size() || !读取.关系.empty() ||
        !读取.值.empty() || !读取.属性值.empty() ||
        读取.源关系组.size() != 请求.源关系组.size() ||
        !读取.目标关系组.empty())
      return false;
    const auto &owner = 读取.所有者.front();
    return owner.查询所有者 == 所有者_ &&
           owner.状态 == L1所有者范围一致当前读取项目状态::成功 &&
           owner.所有者事实 && owner.所有者事实->所有者 == 所有者_ &&
           owner.所有者事实->范围种类 == L1所有者范围种类::独占结构范围 &&
           owner.所有者事实->创建事实代次 != 0 &&
           owner.所有者事实->创建事实代次 <= 请求.期望事实代次 &&
           !owner.所有者事实->退出事实代次;
  }

  bool 普通空节点当前有效(const L1所有者范围节点事实 &节点,
                          稳定编码 编码) const noexcept {
    return 节点.编码 == 编码 && 节点.写入所有者 == 所有者_ &&
           节点.种类 == 节点种类::普通 && !节点.属性类型表示 &&
           节点.创建事实代次 != 0 && !节点.退出事实代次 &&
           节点.当前属性.empty();
  }

  bool 族归属关系有效(const L1所有者范围关系事实 &关系, 稳定编码 身份,
                      std::optional<std::uint64_t> 要求退出) const noexcept {
    return 有效(关系.编码) && 关系.源节点 == 身份 &&
           关系.目标节点 == 存在族锚点_ &&
           关系.关系类型节点 == 存在族归属关系类型_ && 关系.角色或顺序 == 1 &&
           关系.写入所有者 == 所有者_ && 关系.创建事实代次 != 0 &&
           关系.退出事实代次 == 要求退出;
  }

  static 存在结构身份只读状态
  映射窄读取状态(L1所有者范围读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::未找到:
      return 存在结构身份只读状态::未找到;
    case L1所有者范围读取状态::已退出:
      return 存在结构身份只读状态::目标已退出;
    case L1所有者范围读取状态::事实代次漂移:
      return 存在结构身份只读状态::事实代次漂移;
    case L1所有者范围读取状态::历史材料已清理:
      return 存在结构身份只读状态::历史材料已清理;
    case L1所有者范围读取状态::资源失败:
      return 存在结构身份只读状态::资源失败;
    case L1所有者范围读取状态::入口拒绝:
    case L1所有者范围读取状态::许可拒绝:
      return 存在结构身份只读状态::入口拒绝;
    default:
      return 存在结构身份只读状态::内部不一致;
    }
  }

  static 存在类数据状态 映射窄数据状态(存在结构身份只读状态 状态) noexcept {
    switch (状态) {
    case 存在结构身份只读状态::已读取:
      return 存在类数据状态::已读取;
    case 存在结构身份只读状态::未找到:
      return 存在类数据状态::未找到;
    case 存在结构身份只读状态::目标已退出:
      return 存在类数据状态::目标已退出;
    case 存在结构身份只读状态::入口拒绝:
      return 存在类数据状态::格式不支持;
    case 存在结构身份只读状态::事实代次漂移:
      return 存在类数据状态::事实代次漂移;
    case 存在结构身份只读状态::历史材料已清理:
      return 存在类数据状态::历史材料已清理;
    case 存在结构身份只读状态::资源失败:
      return 存在类数据状态::资源失败;
    default:
      return 存在类数据状态::内部不一致;
    }
  }

  static 存在历史读取状态 映射窄历史状态(存在结构身份只读状态 状态) noexcept {
    switch (状态) {
    case 存在结构身份只读状态::已读取:
      return 存在历史读取状态::已读取;
    case 存在结构身份只读状态::未找到:
      return 存在历史读取状态::未找到;
    case 存在结构身份只读状态::目标已退出:
      return 存在历史读取状态::目标已退出;
    case 存在结构身份只读状态::事实代次漂移:
      return 存在历史读取状态::事实代次漂移;
    case 存在结构身份只读状态::历史材料已清理:
      return 存在历史读取状态::历史材料不可用;
    case 存在结构身份只读状态::资源失败:
      return 存在历史读取状态::资源失败;
    case 存在结构身份只读状态::入口拒绝:
      return 存在历史读取状态::引用冲突;
    default:
      return 存在历史读取状态::内部不一致;
    }
  }

  std::optional<存在结构身份只读状态> 历史来源读取错误(
      const L1结构所有者历史读取结果 &owner, const L1所有者范围历史读取结果 &e,
      const L1所有者范围历史读取结果 &anchor,
      const L1所有者范围历史读取结果 &type,
      const L1所有者范围历史关系组读取结果 &links, std::uint64_t Gread,
      std::uint64_t H, 稳定编码 身份) const noexcept {
    if (e.读取事实代次 != Gread)
      return 存在结构身份只读状态::事实代次漂移;
    if (e.状态 != L1所有者范围读取状态::成功)
      return 映射窄读取状态(e.状态);
    const auto *eFact =
        e.事实 ? std::get_if<L1所有者范围节点事实>(&*e.事实) : nullptr;
    if (!eFact || e.合同版本 != L1所有者范围CRUD合同版本 || e.查询编码 != 身份)
      return 存在结构身份只读状态::内部不一致;
    if (eFact->创建事实代次 == 0 || eFact->创建事实代次 > H)
      return 存在结构身份只读状态::未找到;
    if (eFact->退出事实代次 && *eFact->退出事实代次 <= H)
      return 存在结构身份只读状态::目标已退出;
    if (eFact->写入所有者 != 所有者_ || 身份 == 存在族锚点_ ||
        身份 == 存在族归属关系类型_)
      return 存在结构身份只读状态::入口拒绝;

    if (links.读取事实代次 != Gread)
      return 存在结构身份只读状态::事实代次漂移;
    if (links.状态 != L1所有者范围读取状态::成功)
      return 映射窄读取状态(links.状态);
    if (links.合同版本 != L1所有者范围CRUD合同版本 ||
        links.方向 != L1所有者范围关系端点方向::源 || links.端点节点 != 身份 ||
        links.关系类型节点 != 存在族归属关系类型_ ||
        links.历史截止事实代次 != H)
      return 存在结构身份只读状态::内部不一致;
    if (links.关系组.empty())
      return 存在结构身份只读状态::入口拒绝;

    const L1所有者范围历史读取结果 *元节点组[]{&anchor, &type};
    const 稳定编码 元编码组[]{存在族锚点_, 存在族归属关系类型_};
    for (std::size_t i = 0; i < 2; ++i) {
      const auto &r = *元节点组[i];
      if (r.读取事实代次 != Gread)
        return 存在结构身份只读状态::事实代次漂移;
      if (r.状态 != L1所有者范围读取状态::成功)
        return 映射窄读取状态(r.状态) == 存在结构身份只读状态::历史材料已清理
                   ? 存在结构身份只读状态::历史材料已清理
                   : 存在结构身份只读状态::内部不一致;
      const auto *n =
          r.事实 ? std::get_if<L1所有者范围节点事实>(&*r.事实) : nullptr;
      if (!n || r.合同版本 != L1所有者范围CRUD合同版本 ||
          r.查询编码 != 元编码组[i] || n->编码 != 元编码组[i] ||
          n->写入所有者 != 所有者_ || n->种类 != 节点种类::普通 ||
          n->属性类型表示 || !n->当前属性.empty() || n->创建事实代次 == 0 ||
          n->创建事实代次 > H || (n->退出事实代次 && *n->退出事实代次 <= H))
        return 存在结构身份只读状态::内部不一致;
    }

    if (owner.读取事实代次 != Gread)
      return 存在结构身份只读状态::事实代次漂移;
    if (owner.状态 != L1所有者范围读取状态::成功)
      return 映射窄读取状态(owner.状态);
    if (owner.合同版本 != L1所有者范围CRUD合同版本 ||
        owner.查询所有者 != 所有者_ || !owner.所有者事实)
      return 存在结构身份只读状态::内部不一致;
    const auto &of = *owner.所有者事实;
    if (of.所有者 != 所有者_ || of.范围种类 != L1所有者范围种类::独占结构范围)
      return 存在结构身份只读状态::内部不一致;
    if (of.创建事实代次 == 0 || of.创建事实代次 > H)
      return 存在结构身份只读状态::入口拒绝;
    if (of.退出事实代次 && *of.退出事实代次 <= H)
      return 存在结构身份只读状态::目标已退出;

    if (eFact->种类 != 节点种类::普通 || eFact->属性类型表示 ||
        !eFact->当前属性.empty() || links.关系组.size() != 1)
      return 存在结构身份只读状态::内部不一致;
    const auto &relation = links.关系组.front();
    if (!族归属关系有效(relation, 身份, relation.退出事实代次) ||
        (relation.退出事实代次 && *relation.退出事实代次 <= H) ||
        relation.创建事实代次 != eFact->创建事实代次)
      return 存在结构身份只读状态::内部不一致;
    return std::nullopt;
  }

  static L1所有者范围写集请求 形成存在族初始化写集(std::uint64_t G) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = G;
    写集.写入幂等身份 = 存在族来源初始化幂等身份;
    写集.节点 = {
        {存在类数据内部::存在族锚点本地键, 节点种类::普通, std::nullopt},
        {存在类数据内部::存在族归属类型本地键, 节点种类::普通, std::nullopt}};
    return 写集;
  }

  void 接受存在族首次材料(const L1所有者范围首次写入读取结果 &首次) {
    if (首次.状态 != L1所有者范围读取状态::成功 ||
        首次.合同版本 != L1所有者范围首次写入读取合同版本 ||
        首次.所有者 != 所有者_ ||
        首次.写入幂等身份 != 存在族来源初始化幂等身份 || !首次.首次规范化写集 ||
        !首次.首次写入结果)
      throw std::logic_error("invalid existence family first write");
    const auto 期望 = 形成存在族初始化写集(首次.首次规范化写集->期望事实代次);
    if (*首次.首次规范化写集 != 期望)
      throw std::logic_error("conflicting existence family layout");
    const auto &写入 = *首次.首次写入结果;
    if (写入.状态 != L1所有者范围写入状态::成功 ||
        写入.合同版本 != L1所有者范围CRUD合同版本 || 写入.所有者 != 所有者_ ||
        写入.写入幂等身份 != 存在族来源初始化幂等身份 ||
        !写入.是否形成内存权威发布 || 写入.事实代次 == 0 ||
        写入.事实代次 != 期望.期望事实代次 + 1 || 写入.新编码映射.size() != 2)
      throw std::logic_error("invalid existence family publication");
    const auto 锚点 =
        存在类数据内部::查找唯一编码(写入, 存在类数据内部::存在族锚点本地键);
    const auto 类型 = 存在类数据内部::查找唯一编码(
        写入, 存在类数据内部::存在族归属类型本地键);
    if (!锚点 || !类型 || *锚点 == *类型)
      throw std::logic_error("invalid existence family mapping");

    L1所有者范围一致当前读取请求 请求;
    请求.合同版本 = L1所有者范围一致当前读取合同版本;
    请求.期望事实代次 = 首次.读取事实代次;
    请求.所有者 = {所有者_};
    请求.节点 = {*锚点, *类型};
    const auto 投影 = 第一层服务_.尝试读取所有者范围一致当前投影(请求);
    if (投影.状态 == L1所有者范围一致当前读取状态::资源失败)
      throw std::runtime_error("existence family projection unavailable");
    if (投影.状态 != L1所有者范围一致当前读取状态::成功 ||
        投影.合同版本 != L1所有者范围一致当前读取合同版本 ||
        投影.期望事实代次 != 首次.读取事实代次 ||
        投影.读取事实代次 != 首次.读取事实代次 || 投影.所有者.size() != 1 ||
        投影.节点.size() != 2 || !投影.关系.empty() || !投影.值.empty() ||
        !投影.属性值.empty() || !投影.源关系组.empty() ||
        !投影.目标关系组.empty())
      throw std::logic_error("invalid existence family projection");
    const auto &owner = 投影.所有者.front();
    if (owner.查询所有者 != 所有者_ ||
        owner.状态 != L1所有者范围一致当前读取项目状态::成功 ||
        !owner.所有者事实 || owner.所有者事实->所有者 != 所有者_ ||
        owner.所有者事实->范围种类 != L1所有者范围种类::独占结构范围 ||
        owner.所有者事实->创建事实代次 == 0 ||
        owner.所有者事实->创建事实代次 > 首次.读取事实代次 ||
        owner.所有者事实->退出事实代次)
      throw std::logic_error("invalid existence family owner projection");
    const auto *锚点项 = 查找一致节点(投影, *锚点);
    const auto *类型项 = 查找一致节点(投影, *类型);
    const auto *锚点事实 = 锚点项 ? 当前节点事实(*锚点项) : nullptr;
    const auto *类型事实 = 类型项 ? 当前节点事实(*类型项) : nullptr;
    if (!锚点事实 || !类型事实 || !普通空节点当前有效(*锚点事实, *锚点) ||
        !普通空节点当前有效(*类型事实, *类型))
      throw std::logic_error("invalid existence family metadata");
    存在族锚点_ = *锚点;
    存在族归属关系类型_ = *类型;
  }

  void 初始化存在族来源() {
    for (int 尝试 = 0; 尝试 < 2; ++尝试) {
      const auto 首次 = 写入端口_.读取首次写入材料(
          {L1所有者范围首次写入读取合同版本, 存在族来源初始化幂等身份});
      if (首次.状态 == L1所有者范围读取状态::成功) {
        接受存在族首次材料(首次);
        return;
      }
      if (首次.状态 == L1所有者范围读取状态::资源失败)
        throw std::runtime_error("existence family storage unavailable");
      if (首次.状态 != L1所有者范围读取状态::未找到 ||
          首次.合同版本 != L1所有者范围首次写入读取合同版本 ||
          首次.所有者 != 所有者_ ||
          首次.写入幂等身份 != 存在族来源初始化幂等身份 ||
          首次.读取事实代次 == 0)
        throw std::logic_error("invalid existence family lookup");

      const auto 写集 = 形成存在族初始化写集(首次.读取事实代次);
      const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
      if (写入.状态 == L1所有者范围写入状态::事实代次漂移 && 尝试 == 0)
        continue;
      if (写入.状态 == L1所有者范围写入状态::资源失败)
        throw std::runtime_error("existence family publication unavailable");
      if (写入.状态 != L1所有者范围写入状态::成功 &&
          写入.状态 != L1所有者范围写入状态::精确重复) {
        if (写入.状态 == L1所有者范围写入状态::事实代次漂移)
          throw std::runtime_error("existence family publication drift");
        throw std::logic_error("invalid existence family publication result");
      }
      const auto 读回 = 写入端口_.读取首次写入材料(
          {L1所有者范围首次写入读取合同版本, 存在族来源初始化幂等身份});
      接受存在族首次材料(读回);
      return;
    }
    throw std::runtime_error("existence family initialization failed");
  }

  static bool 参数类别有效(方法参数规格类别 类别) noexcept {
    return 类别 == 方法参数规格类别::场景 || 类别 == 方法参数规格类别::存在 ||
           类别 == 方法参数规格类别::特征;
  }

  static std::array<稳定编码, 14>
  实例结构类型组(const 任务方法实例参数规格结构类型 &s) {
    return {s.任务虚拟存在关系类型,     s.方法虚拟存在关系类型,
            s.方法场景参数关系类型,     s.方法存在参数关系类型,
            s.方法特征参数关系类型,     s.实例宿主关系类型,
            s.实例来源方法关系类型,     s.实例来源虚拟存在关系类型,
            s.实例场景参数关系类型,     s.实例存在参数关系类型,
            s.实例特征参数关系类型,     s.实例场景参数数据关系类型,
            s.实例存在参数数据关系类型, s.实例特征参数数据关系类型};
  }

  static bool 实例结构类型有效(const 任务方法实例参数规格结构类型 &s) {
    auto 组 = 实例结构类型组(s);
    if (std::any_of(组.begin(), 组.end(),
                    [](稳定编码 x) noexcept { return !有效(x); }))
      return false;
    std::sort(组.begin(), 组.end());
    return std::adjacent_find(组.begin(), 组.end()) == 组.end();
  }

  static bool
  实例查询请求有效(const 任务方法实例参数规格组查询请求 &r) noexcept {
    return r.合同版本 == 任务方法实例参数规格专用合同版本 &&
           r.期望事实代次 != 0 && 有效(r.任务所有者) && 有效(r.任务节点) &&
           r.最大实例数量 != 0 && r.每实例最大参数数量 != 0 &&
           实例结构类型有效(r.结构类型);
  }

  static bool
  实例规格请求有效(const 任务方法实例参数规格写集规格请求 &r) noexcept {
    if (r.合同版本 != 任务方法实例参数规格专用合同版本 || r.期望事实代次 == 0 ||
        !有效(r.任务所有者) || !有效(r.写入幂等身份) || !有效(r.任务节点) ||
        !有效(r.来源.方法节点) || !有效(r.来源.方法虚拟存在节点) ||
        !有效(r.来源.方法虚拟存在关系) || r.最大已有实例数量 == 0 ||
        r.每实例最大参数数量 == 0 || !实例结构类型有效(r.结构类型) ||
        r.来源.参数规格组.size() > r.每实例最大参数数量 ||
        r.来源.参数规格组.size() >
            static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max() -
                                     4))
      return false;
    for (std::size_t i = 0; i < r.来源.参数规格组.size(); ++i) {
      const auto &项 = r.来源.参数规格组[i];
      if (!参数类别有效(项.类别) || !有效(项.来源关系) ||
          !有效(项.参数特征类型) || 项.顺序 != i + 1)
        return false;
    }
    return true;
  }

  static bool 绑定请求有效(const 任务实例参数绑定写集规格请求 &r) noexcept {
    return r.合同版本 == 任务方法实例参数规格专用合同版本 &&
           r.期望事实代次 != 0 && 有效(r.任务所有者) && 有效(r.写入幂等身份) &&
           有效(r.任务节点) && 有效(r.方法节点) && 参数类别有效(r.类别) &&
           r.顺序 != 0 &&
           r.顺序 <= static_cast<std::uint64_t>(
                         std::numeric_limits<std::int64_t>::max()) &&
           有效(r.参数特征类型) && 有效(r.特征节点) && r.最大实例数量 != 0 &&
           r.每实例最大参数数量 != 0 && 实例结构类型有效(r.结构类型);
  }

  static 稳定编码
  方法参数关系类型(方法参数规格类别 c,
                   const 任务方法实例参数规格结构类型 &s) noexcept {
    switch (c) {
    case 方法参数规格类别::场景:
      return s.方法场景参数关系类型;
    case 方法参数规格类别::存在:
      return s.方法存在参数关系类型;
    case 方法参数规格类别::特征:
      return s.方法特征参数关系类型;
    }
    return {};
  }

  static 稳定编码
  实例规格关系类型(方法参数规格类别 c,
                   const 任务方法实例参数规格结构类型 &s) noexcept {
    switch (c) {
    case 方法参数规格类别::场景:
      return s.实例场景参数关系类型;
    case 方法参数规格类别::存在:
      return s.实例存在参数关系类型;
    case 方法参数规格类别::特征:
      return s.实例特征参数关系类型;
    }
    return {};
  }

  static 稳定编码
  实例数据关系类型(方法参数规格类别 c,
                   const 任务方法实例参数规格结构类型 &s) noexcept {
    switch (c) {
    case 方法参数规格类别::场景:
      return s.实例场景参数数据关系类型;
    case 方法参数规格类别::存在:
      return s.实例存在参数数据关系类型;
    case 方法参数规格类别::特征:
      return s.实例特征参数数据关系类型;
    }
    return {};
  }

  稳定编码 当前任务Vm(const 任务方法实例参数规格写集规格请求 &r) const {
    const auto q = 查询任务虚拟存在({任务虚拟存在专用合同版本, r.期望事实代次,
                                     r.任务所有者, r.任务节点,
                                     r.结构类型.任务虚拟存在关系类型});
    return q.成功() && q.投影 ? q.投影->虚拟存在节点 : 稳定编码{};
  }

  任务方法实例参数规格专用状态 验证参数类型节点(稳定编码 ft,
                                                std::uint64_t G0) const {
    // FT由特征定义服务解释；普通类型结点不能再按L1属性类型验形。
    const auto q = 特征服务_.读取I64类型完整域({1, G0, G0, {ft}});
    if (const auto *error = std::get_if<特征数据错误>(&q)) {
      switch (*error) {
      case 特征数据错误::并发变化:
        return 任务方法实例参数规格专用状态::事实代次漂移;
      case 特征数据错误::未找到:
        return 任务方法实例参数规格专用状态::参数特征类型未找到;
      case 特征数据错误::已退出:
        return 任务方法实例参数规格专用状态::参数特征类型已退出;
      case 特征数据错误::资源失败:
        return 任务方法实例参数规格专用状态::资源失败;
      case 特征数据错误::入口拒绝:
      case 特征数据错误::类型不相容:
        return 任务方法实例参数规格专用状态::参数特征类型不是属性类型;
      default:
        return 任务方法实例参数规格专用状态::内部不一致;
      }
    }
    const auto &fact = std::get<特征截止事实<特征规范I64域>>(q);
    if (fact.Gread != G0 || fact.H != G0 || fact.数据.区间.empty())
      return 任务方法实例参数规格专用状态::内部不一致;
    return 任务方法实例参数规格专用状态::已读取;
  }

  任务方法实例参数规格专用状态
  验证方法参数来源(const 任务方法实例参数规格来源 &来源,
                   const 任务方法实例参数规格结构类型 &s, std::uint64_t G0,
                   std::uint64_t 最大参数) const {
    const auto m = 第一层服务_.读取所有者范围当前节点(
        {L1所有者范围CRUD合同版本, 来源.方法节点});
    if (m.读取事实代次 != G0)
      return 任务方法实例参数规格专用状态::事实代次漂移;
    if (m.状态 == L1所有者范围读取状态::未找到)
      return 任务方法实例参数规格专用状态::方法节点未找到;
    if (m.状态 == L1所有者范围读取状态::已退出)
      return 任务方法实例参数规格专用状态::方法节点已退出;
    const auto *mn =
        m.事实 ? std::get_if<L1所有者范围节点事实>(&*m.事实) : nullptr;
    if (m.状态 != L1所有者范围读取状态::成功 || !mn ||
        mn->种类 != 节点种类::普通 || !有效(mn->写入所有者))
      return 任务方法实例参数规格专用状态::内部不一致;
    const auto vm =
        查询方法虚拟存在({方法虚拟存在专用合同版本, G0, mn->写入所有者,
                          来源.方法节点, s.方法虚拟存在关系类型});
    if (vm.状态 == 方法虚拟存在专用状态::未初始化)
      return 任务方法实例参数规格专用状态::方法虚拟存在未初始化;
    if (!vm.成功() || !vm.投影)
      return vm.状态 == 方法虚拟存在专用状态::事实代次漂移
                 ? 任务方法实例参数规格专用状态::事实代次漂移
                 : 任务方法实例参数规格专用状态::内部不一致;
    if (vm.投影->虚拟存在节点 != 来源.方法虚拟存在节点 ||
        vm.投影->方法虚拟存在关系 != 来源.方法虚拟存在关系)
      return 任务方法实例参数规格专用状态::结构冲突;
    std::vector<方法参数规格来源项> 实际;
    for (const auto c : {方法参数规格类别::场景, 方法参数规格类别::存在,
                         方法参数规格类别::特征}) {
      const auto rt = 方法参数关系类型(c, s);
      const auto rtn =
          第一层服务_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, rt});
      const auto *rtf =
          rtn.事实 ? std::get_if<L1所有者范围节点事实>(&*rtn.事实) : nullptr;
      if (rtn.读取事实代次 != G0)
        return 任务方法实例参数规格专用状态::事实代次漂移;
      if (rtn.状态 == L1所有者范围读取状态::未找到)
        return 任务方法实例参数规格专用状态::关系类型未找到;
      if (rtn.状态 == L1所有者范围读取状态::已退出)
        return 任务方法实例参数规格专用状态::关系类型已退出;
      if (rtn.状态 != L1所有者范围读取状态::成功 || !rtf ||
          rtf->写入所有者 != mn->写入所有者 || rtf->种类 != 节点种类::普通 ||
          rtf->属性类型表示 || !rtf->当前属性.empty())
        return 任务方法实例参数规格专用状态::所有者不匹配;
      const auto q = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本, 来源.方法虚拟存在节点, rt});
      if (q.读取事实代次 != G0)
        return 任务方法实例参数规格专用状态::事实代次漂移;
      if (q.状态 != L1所有者范围读取状态::成功)
        return q.状态 == L1所有者范围读取状态::资源失败
                   ? 任务方法实例参数规格专用状态::资源失败
                   : 任务方法实例参数规格专用状态::内部不一致;
      if (实际.size() + q.关系组.size() > 最大参数)
        return 任务方法实例参数规格专用状态::数量预算不足;
      for (const auto &rel : q.关系组) {
        if (rel.源节点 != 来源.方法虚拟存在节点 || rel.关系类型节点 != rt ||
            rel.写入所有者 != mn->写入所有者 || rel.角色或顺序 <= 0 ||
            rel.退出事实代次)
          return 任务方法实例参数规格专用状态::结构冲突;
        const auto ft = 验证参数类型节点(rel.目标节点, G0);
        if (ft != 任务方法实例参数规格专用状态::已读取)
          return ft;
        实际.push_back({c, rel.编码, rel.目标节点,
                        static_cast<std::uint64_t>(rel.角色或顺序)});
      }
    }
    std::sort(实际.begin(), 实际.end(),
              [](const auto &a, const auto &b) { return a.顺序 < b.顺序; });
    for (std::size_t i = 0; i < 实际.size(); ++i)
      if (实际[i].顺序 != i + 1)
        return 任务方法实例参数规格专用状态::结构冲突;
    return 实际 == 来源.参数规格组 ? 任务方法实例参数规格专用状态::已读取
                                   : 任务方法实例参数规格专用状态::结构冲突;
  }

  任务方法实例参数规格组结果
  读取实例组内部(const 任务方法实例参数规格组查询请求 &r) const {
    const auto tvm = 查询任务虚拟存在({任务虚拟存在专用合同版本, r.期望事实代次,
                                       r.任务所有者, r.任务节点,
                                       r.结构类型.任务虚拟存在关系类型});
    if (!tvm.成功() || !tvm.投影)
      return 实例查询失败(
          tvm.状态 == 任务虚拟存在专用状态::未初始化
              ? 任务方法实例参数规格专用状态::任务虚拟存在未初始化
              : 任务方法实例参数规格专用状态::内部不一致,
          r, tvm.事实代次);
    const auto host = 第一层服务_.读取所有者范围当前源关系组(
        {L1所有者范围CRUD合同版本, tvm.投影->虚拟存在节点,
         r.结构类型.实例宿主关系类型});
    if (host.读取事实代次 != r.期望事实代次)
      return 实例查询失败(任务方法实例参数规格专用状态::事实代次漂移, r,
                          host.读取事实代次);
    if (host.状态 != L1所有者范围读取状态::成功)
      return 实例查询失败(任务方法实例参数规格专用状态::内部不一致, r,
                          host.读取事实代次);
    if (host.关系组.size() > r.最大实例数量)
      return 实例查询失败(任务方法实例参数规格专用状态::数量预算不足, r,
                          r.期望事实代次);
    std::vector<任务方法实例参数规格投影> out;
    for (const auto &h : host.关系组) {
      if (h.源节点 != tvm.投影->虚拟存在节点 ||
          h.关系类型节点 != r.结构类型.实例宿主关系类型 ||
          h.写入所有者 != r.任务所有者 || h.角色或顺序 != 0 || h.退出事实代次)
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            r.期望事实代次);
      const auto inode = 第一层服务_.读取所有者范围当前节点(
          {L1所有者范围CRUD合同版本, h.目标节点});
      const auto *in = inode.事实
                           ? std::get_if<L1所有者范围节点事实>(&*inode.事实)
                           : nullptr;
      if (inode.读取事实代次 != r.期望事实代次 ||
          inode.状态 != L1所有者范围读取状态::成功 || !in ||
          in->写入所有者 != r.任务所有者 || in->种类 != 节点种类::普通 ||
          in->属性类型表示 || in->创建事实代次 == 0 || in->退出事实代次 ||
          !in->当前属性.empty())
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            inode.读取事实代次);
      const auto mr = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本, h.目标节点,
           r.结构类型.实例来源方法关系类型});
      const auto vr = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本, h.目标节点,
           r.结构类型.实例来源虚拟存在关系类型});
      if (mr.读取事实代次 != r.期望事实代次 ||
          vr.读取事实代次 != r.期望事实代次 ||
          mr.状态 != L1所有者范围读取状态::成功 ||
          vr.状态 != L1所有者范围读取状态::成功 || mr.关系组.size() != 1 ||
          vr.关系组.size() != 1)
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            r.期望事实代次);
      const auto &mrel = mr.关系组.front();
      const auto &vrel = vr.关系组.front();
      if (mrel.源节点 != h.目标节点 ||
          mrel.关系类型节点 != r.结构类型.实例来源方法关系类型 ||
          mrel.写入所有者 != r.任务所有者 || mrel.角色或顺序 != 0 ||
          mrel.退出事实代次 || vrel.源节点 != h.目标节点 ||
          vrel.关系类型节点 != r.结构类型.实例来源虚拟存在关系类型 ||
          vrel.写入所有者 != r.任务所有者 || vrel.角色或顺序 != 0 ||
          vrel.退出事实代次)
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            r.期望事实代次);
      任务方法实例参数规格来源 source{
          mr.关系组.front().目标节点, vr.关系组.front().目标节点, {}};
      const auto mnq = 第一层服务_.读取所有者范围当前节点(
          {L1所有者范围CRUD合同版本, source.方法节点});
      const auto *mn =
          mnq.事实 ? std::get_if<L1所有者范围节点事实>(&*mnq.事实) : nullptr;
      if (mnq.读取事实代次 != r.期望事实代次 ||
          mnq.状态 != L1所有者范围读取状态::成功 || !mn ||
          mn->种类 != 节点种类::普通 || mn->属性类型表示 ||
          mn->创建事实代次 == 0 || mn->退出事实代次)
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            mnq.读取事实代次);
      const auto vmq = 查询方法虚拟存在(
          {方法虚拟存在专用合同版本, r.期望事实代次, mn->写入所有者,
           source.方法节点, r.结构类型.方法虚拟存在关系类型});
      if (!vmq.成功() || !vmq.投影 ||
          vmq.投影->虚拟存在节点 != source.方法虚拟存在节点)
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            vmq.事实代次);
      source.方法虚拟存在关系 = vmq.投影->方法虚拟存在关系;
      std::vector<任务实例参数规格关系项> specs;
      for (const auto c : {方法参数规格类别::场景, 方法参数规格类别::存在,
                           方法参数规格类别::特征}) {
        const auto sr = 第一层服务_.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本, h.目标节点,
             实例规格关系类型(c, r.结构类型)});
        const auto dr = 第一层服务_.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本, h.目标节点,
             实例数据关系类型(c, r.结构类型)});
        if (sr.读取事实代次 != r.期望事实代次 ||
            dr.读取事实代次 != r.期望事实代次)
          return 实例查询失败(任务方法实例参数规格专用状态::事实代次漂移, r,
                              sr.读取事实代次 != r.期望事实代次
                                  ? sr.读取事实代次
                                  : dr.读取事实代次);
        if (sr.状态 != L1所有者范围读取状态::成功 ||
            dr.状态 != L1所有者范围读取状态::成功)
          return 实例查询失败(sr.状态 == L1所有者范围读取状态::资源失败 ||
                                      dr.状态 == L1所有者范围读取状态::资源失败
                                  ? 任务方法实例参数规格专用状态::资源失败
                                  : 任务方法实例参数规格专用状态::内部不一致,
                              r, r.期望事实代次);
        if (specs.size() + sr.关系组.size() > r.每实例最大参数数量)
          return 实例查询失败(任务方法实例参数规格专用状态::数量预算不足, r,
                              r.期望事实代次);
        for (const auto &rel : sr.关系组) {
          if (rel.源节点 != h.目标节点 ||
              rel.关系类型节点 != 实例规格关系类型(c, r.结构类型) ||
              rel.写入所有者 != r.任务所有者 || rel.角色或顺序 <= 0 ||
              rel.退出事实代次)
            return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                                r.期望事实代次);
          const auto ft = 验证参数类型节点(rel.目标节点, r.期望事实代次);
          if (ft != 任务方法实例参数规格专用状态::已读取)
            return 实例查询失败(ft, r, r.期望事实代次);
          specs.push_back({c, rel.编码, rel.目标节点,
                           static_cast<std::uint64_t>(rel.角色或顺序),
                           std::nullopt, std::nullopt});
        }
        for (const auto &rel : dr.关系组) {
          const auto slot = std::find_if(
              specs.begin(), specs.end(), [c, &rel](const auto &x) noexcept {
                return x.类别 == c && x.顺序 == rel.角色或顺序;
              });
          if (rel.源节点 != h.目标节点 ||
              rel.关系类型节点 != 实例数据关系类型(c, r.结构类型) ||
              rel.写入所有者 != r.任务所有者 || rel.角色或顺序 <= 0 ||
              rel.退出事实代次 || slot == specs.end() || slot->数据关系 ||
              slot->特征节点)
            return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                                r.期望事实代次);
          slot->数据关系 = rel.编码;
          slot->特征节点 = rel.目标节点;
        }
      }
      std::sort(specs.begin(), specs.end(),
                [](const auto &a, const auto &b) { return a.顺序 < b.顺序; });
      for (std::size_t i = 0; i < specs.size(); ++i) {
        if (specs[i].顺序 != i + 1)
          return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                              r.期望事实代次);
      }
      for (const auto c : {方法参数规格类别::场景, 方法参数规格类别::存在,
                           方法参数规格类别::特征}) {
        const auto mpr = 第一层服务_.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本, source.方法虚拟存在节点,
             方法参数关系类型(c, r.结构类型)});
        if (mpr.读取事实代次 != r.期望事实代次 ||
            mpr.状态 != L1所有者范围读取状态::成功)
          return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                              mpr.读取事实代次);
        for (const auto &rel : mpr.关系组)
          source.参数规格组.push_back(
              {c, rel.编码, rel.目标节点,
               static_cast<std::uint64_t>(rel.角色或顺序)});
      }
      std::sort(source.参数规格组.begin(), source.参数规格组.end(),
                [](const auto &a, const auto &b) { return a.顺序 < b.顺序; });
      const auto sourceState = 验证方法参数来源(
          source, r.结构类型, r.期望事实代次, r.每实例最大参数数量);
      if (sourceState != 任务方法实例参数规格专用状态::已读取)
        return 实例查询失败(sourceState, r, r.期望事实代次);
      if (source.参数规格组.size() != specs.size())
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            r.期望事实代次);
      for (std::size_t i = 0; i < specs.size(); ++i)
        if (source.参数规格组[i].类别 != specs[i].类别 ||
            source.参数规格组[i].顺序 != specs[i].顺序 ||
            source.参数规格组[i].参数特征类型 != specs[i].参数特征类型)
          return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                              r.期望事实代次);
      out.push_back({r.任务节点, tvm.投影->虚拟存在节点, h.目标节点, h.编码,
                     mr.关系组.front().编码, vr.关系组.front().编码,
                     std::move(source), std::move(specs), in->创建事实代次});
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) {
      if (a.来源.方法节点 != b.来源.方法节点)
        return a.来源.方法节点 < b.来源.方法节点;
      return a.实例节点 < b.实例节点;
    });
    for (std::size_t i = 1; i < out.size(); ++i)
      if (out[i - 1].来源.方法节点 == out[i].来源.方法节点)
        return 实例查询失败(任务方法实例参数规格专用状态::结构冲突, r,
                            r.期望事实代次);
    return {任务方法实例参数规格专用状态::已读取,
            任务方法实例参数规格专用合同版本,
            r.期望事实代次,
            r.任务所有者,
            r.任务节点,
            std::move(out)};
  }

  static 任务方法实例参数规格写集规格结果
  实例规格失败(任务方法实例参数规格专用状态 s,
               const 任务方法实例参数规格写集规格请求 &r,
               std::uint64_t g = 0) noexcept {
    return {s,
            任务方法实例参数规格专用合同版本,
            g,
            r.任务所有者,
            r.写入幂等身份,
            std::nullopt};
  }

  static 任务方法实例参数规格写集规格结果
  绑定规格失败(任务方法实例参数规格专用状态 s,
               const 任务实例参数绑定写集规格请求 &r,
               std::uint64_t g = 0) noexcept {
    return {s,
            任务方法实例参数规格专用合同版本,
            g,
            r.任务所有者,
            r.写入幂等身份,
            std::nullopt};
  }

  static 任务方法实例参数规格组结果
  实例查询失败(任务方法实例参数规格专用状态 s,
               const 任务方法实例参数规格组查询请求 &r,
               std::uint64_t g = 0) noexcept {
    return {s, 任务方法实例参数规格专用合同版本, g, r.任务所有者, r.任务节点,
            {}};
  }

  struct 方法虚拟存在代次读取值 final {
    方法虚拟存在专用状态 状态 = 方法虚拟存在专用状态::内部不一致;
    std::uint64_t 事实代次 = 0;
  };

  static 方法虚拟存在写集规格结果
  方法规格失败(方法虚拟存在专用状态 状态, const 方法虚拟存在写集规格请求 &请求,
               std::uint64_t 事实代次 = 0) noexcept {
    return {状态,
            方法虚拟存在专用合同版本,
            事实代次,
            请求.方法所有者,
            请求.写入幂等身份,
            std::nullopt};
  }

  static 方法虚拟存在查询结果
  方法查询失败(方法虚拟存在专用状态 状态, const 方法虚拟存在查询请求 &请求,
               std::uint64_t 事实代次 = 0) noexcept {
    return {状态, 方法虚拟存在专用合同版本, 事实代次, 请求.方法所有者,
            std::nullopt};
  }

  方法虚拟存在代次读取值 读取方法虚拟存在当前代次() const {
    const auto 读取 = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (读取.状态 == L1中性读取状态::成功 &&
        读取.合同版本 == L1中性CRUD合同版本 && 读取.事实代次 != 0)
      return {方法虚拟存在专用状态::已读取, 读取.事实代次};
    if (读取.状态 == L1中性读取状态::资源失败)
      return {方法虚拟存在专用状态::资源失败, 读取.事实代次};
    if (读取.状态 == L1中性读取状态::入口拒绝)
      return {方法虚拟存在专用状态::入口拒绝, 读取.事实代次};
    return {方法虚拟存在专用状态::内部不一致, 读取.事实代次};
  }

  std::optional<方法虚拟存在专用状态> 验证方法所有者(L1结构所有者身份 所有者,
                                                     std::uint64_t G0) const {
    const auto 读取 =
        第一层服务_.读取当前结构所有者({L1所有者范围CRUD合同版本, 所有者});
    if (读取.读取事实代次 != G0)
      return 方法虚拟存在专用状态::事实代次漂移;
    if (读取.状态 == L1所有者范围读取状态::未找到)
      return 方法虚拟存在专用状态::方法所有者未找到;
    if (读取.状态 == L1所有者范围读取状态::已退出)
      return 方法虚拟存在专用状态::方法所有者已退出;
    if (读取.状态 == L1所有者范围读取状态::资源失败)
      return 方法虚拟存在专用状态::资源失败;
    if (读取.状态 == L1所有者范围读取状态::入口拒绝)
      return 方法虚拟存在专用状态::入口拒绝;
    if (读取.状态 != L1所有者范围读取状态::成功 ||
        读取.合同版本 != L1所有者范围CRUD合同版本 || !读取.所有者事实)
      return 方法虚拟存在专用状态::内部不一致;
    if (读取.所有者事实->所有者 != 所有者)
      return 方法虚拟存在专用状态::所有者不匹配;
    if (读取.所有者事实->范围种类 != L1所有者范围种类::独占结构范围 ||
        读取.所有者事实->创建事实代次 == 0 ||
        读取.所有者事实->创建事实代次 > G0)
      return 方法虚拟存在专用状态::结构冲突;
    if (读取.所有者事实->退出事实代次)
      return 方法虚拟存在专用状态::方法所有者已退出;
    return std::nullopt;
  }

  std::optional<方法虚拟存在专用状态>
  验证方法当前节点(L1结构所有者身份 所有者, 稳定编码 编码, std::uint64_t G0,
                   方法虚拟存在专用状态 未找到状态,
                   方法虚拟存在专用状态 已退出状态) const {
    const auto 读取 =
        第一层服务_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, 编码});
    if (读取.读取事实代次 != G0)
      return 方法虚拟存在专用状态::事实代次漂移;
    if (读取.状态 != L1所有者范围读取状态::成功)
      return 映射方法虚拟存在L1状态(读取.状态, 未找到状态, 已退出状态);
    const auto *节点 =
        读取.事实 ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
    if (!节点 || 节点->编码 != 编码)
      return 方法虚拟存在专用状态::内部不一致;
    if (节点->写入所有者 != 所有者)
      return 方法虚拟存在专用状态::所有者不匹配;
    if (节点->退出事实代次)
      return 已退出状态;
    if (节点->种类 != 节点种类::普通 || 节点->属性类型表示 ||
        节点->创建事实代次 == 0 || 节点->创建事实代次 > G0 ||
        !节点->当前属性.empty())
      return 方法虚拟存在专用状态::结构冲突;
    return std::nullopt;
  }

  static 方法虚拟存在专用状态
  映射方法虚拟存在L1状态(L1所有者范围读取状态 状态,
                         方法虚拟存在专用状态 未找到状态,
                         方法虚拟存在专用状态 已退出状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::未找到:
      return 未找到状态;
    case L1所有者范围读取状态::已退出:
      return 已退出状态;
    case L1所有者范围读取状态::事实代次漂移:
      return 方法虚拟存在专用状态::事实代次漂移;
    case L1所有者范围读取状态::资源失败:
      return 方法虚拟存在专用状态::资源失败;
    case L1所有者范围读取状态::入口拒绝:
      return 方法虚拟存在专用状态::入口拒绝;
    default:
      return 方法虚拟存在专用状态::内部不一致;
    }
  }

  static 方法虚拟存在专用状态
  映射方法虚拟存在一致状态(L1所有者范围一致当前读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围一致当前读取状态::事实代次漂移:
      return 方法虚拟存在专用状态::事实代次漂移;
    case L1所有者范围一致当前读取状态::资源失败:
      return 方法虚拟存在专用状态::资源失败;
    case L1所有者范围一致当前读取状态::入口拒绝:
      return 方法虚拟存在专用状态::入口拒绝;
    default:
      return 方法虚拟存在专用状态::内部不一致;
    }
  }

  static bool
  方法虚拟存在投影头完整(const L1所有者范围一致当前读取结果 &读取,
                         const L1所有者范围一致当前读取请求 &请求) noexcept {
    if (读取.状态 != L1所有者范围一致当前读取状态::成功 ||
        读取.合同版本 != L1所有者范围一致当前读取合同版本 ||
        读取.期望事实代次 != 请求.期望事实代次 ||
        读取.读取事实代次 != 请求.期望事实代次 || 读取.所有者.size() != 1 ||
        读取.节点.size() != 请求.节点.size() || 读取.关系.size() != 1 ||
        !读取.值.empty() || !读取.属性值.empty() || !读取.源关系组.empty() ||
        !读取.目标关系组.empty())
      return false;
    const auto &所有者 = 读取.所有者.front();
    return 所有者.查询所有者 == 请求.所有者.front() &&
           所有者.状态 == L1所有者范围一致当前读取项目状态::成功 &&
           所有者.所有者事实 &&
           所有者.所有者事实->所有者 == 请求.所有者.front() &&
           所有者.所有者事实->范围种类 == L1所有者范围种类::独占结构范围 &&
           所有者.所有者事实->创建事实代次 != 0 &&
           所有者.所有者事实->创建事实代次 <= 请求.期望事实代次 &&
           !所有者.所有者事实->退出事实代次;
  }

  static const L1所有者范围节点事实 *
  查找方法虚拟存在节点(const L1所有者范围一致当前读取结果 &读取,
                       稳定编码 编码) noexcept {
    const L1所有者范围节点事实 *找到 = nullptr;
    for (const auto &项 : 读取.节点) {
      if (项.查询编码 != 编码)
        continue;
      if (找到 || 项.状态 != L1所有者范围一致当前读取项目状态::成功 ||
          !项.事实 || 项.物理清理事实代次 || 项.物理清理墓碑)
        return nullptr;
      找到 = &*项.事实;
    }
    return 找到;
  }

  static const L1所有者范围关系事实 *
  查找方法虚拟存在关系(const L1所有者范围一致当前读取结果 &读取,
                       稳定编码 编码) noexcept {
    const L1所有者范围关系事实 *找到 = nullptr;
    for (const auto &项 : 读取.关系) {
      if (项.查询编码 != 编码)
        continue;
      if (找到 || 项.状态 != L1所有者范围一致当前读取项目状态::成功 ||
          !项.事实 || 项.物理清理事实代次 || 项.物理清理墓碑)
        return nullptr;
      找到 = &*项.事实;
    }
    return 找到;
  }

  static bool 方法虚拟存在节点完整(const L1所有者范围节点事实 &节点,
                                   L1结构所有者身份 所有者) noexcept {
    return 有效(节点.编码) && 节点.写入所有者 == 所有者 &&
           节点.种类 == 节点种类::普通 && !节点.属性类型表示 &&
           节点.创建事实代次 != 0 && !节点.退出事实代次 &&
           节点.当前属性.empty();
  }

  struct 任务虚拟存在代次读取值 final {
    任务虚拟存在专用状态 状态 = 任务虚拟存在专用状态::内部不一致;
    std::uint64_t 事实代次 = 0;
  };

  static 任务虚拟存在写集规格结果
  规格失败(任务虚拟存在专用状态 状态, const 任务虚拟存在写集规格请求 &请求,
           std::uint64_t 事实代次 = 0) noexcept {
    return {状态,
            任务虚拟存在专用合同版本,
            事实代次,
            请求.任务所有者,
            请求.写入幂等身份,
            std::nullopt};
  }

  static 任务虚拟存在查询结果
  查询规格失败(任务虚拟存在专用状态 状态, const 任务虚拟存在查询请求 &请求,
               std::uint64_t 事实代次 = 0) noexcept {
    return {状态, 任务虚拟存在专用合同版本, 事实代次, 请求.任务所有者,
            std::nullopt};
  }

  任务虚拟存在代次读取值 读取任务虚拟存在当前代次() const {
    const auto 读取 = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (读取.状态 == L1中性读取状态::成功 &&
        读取.合同版本 == L1中性CRUD合同版本 && 读取.事实代次 != 0)
      return {任务虚拟存在专用状态::已读取, 读取.事实代次};
    if (读取.状态 == L1中性读取状态::资源失败)
      return {任务虚拟存在专用状态::资源失败, 读取.事实代次};
    if (读取.状态 == L1中性读取状态::入口拒绝)
      return {任务虚拟存在专用状态::入口拒绝, 读取.事实代次};
    return {任务虚拟存在专用状态::内部不一致, 读取.事实代次};
  }

  std::optional<任务虚拟存在专用状态>
  验证任务所有者(L1结构所有者身份 所有者, std::uint64_t 期望事实代次) const {
    const auto 读取 =
        第一层服务_.读取当前结构所有者({L1所有者范围CRUD合同版本, 所有者});
    if (读取.读取事实代次 != 期望事实代次)
      return 任务虚拟存在专用状态::事实代次漂移;
    if (读取.状态 == L1所有者范围读取状态::未找到)
      return 任务虚拟存在专用状态::任务所有者未找到;
    if (读取.状态 == L1所有者范围读取状态::已退出)
      return 任务虚拟存在专用状态::任务所有者已退出;
    if (读取.状态 == L1所有者范围读取状态::资源失败)
      return 任务虚拟存在专用状态::资源失败;
    if (读取.状态 == L1所有者范围读取状态::入口拒绝)
      return 任务虚拟存在专用状态::入口拒绝;
    if (读取.状态 != L1所有者范围读取状态::成功)
      return 任务虚拟存在专用状态::内部不一致;
    if (读取.合同版本 != L1所有者范围CRUD合同版本 ||
        读取.查询所有者 != 所有者 || !读取.所有者事实)
      return 任务虚拟存在专用状态::内部不一致;
    const auto &事实 = *读取.所有者事实;
    if (事实.所有者 != 所有者)
      return 任务虚拟存在专用状态::所有者不匹配;
    if (事实.范围种类 != L1所有者范围种类::独占结构范围 ||
        事实.创建事实代次 == 0 || 事实.创建事实代次 > 期望事实代次)
      return 任务虚拟存在专用状态::结构冲突;
    if (事实.退出事实代次)
      return 任务虚拟存在专用状态::任务所有者已退出;
    return std::nullopt;
  }

  std::optional<任务虚拟存在专用状态>
  验证任务当前节点(L1结构所有者身份 所有者, 稳定编码 编码,
                   std::uint64_t 期望事实代次, 任务虚拟存在专用状态 未找到状态,
                   任务虚拟存在专用状态 已退出状态) const {
    const auto 读取 =
        第一层服务_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, 编码});
    if (读取.读取事实代次 != 期望事实代次)
      return 任务虚拟存在专用状态::事实代次漂移;
    if (读取.状态 != L1所有者范围读取状态::成功)
      return 映射任务虚拟存在L1状态(读取.状态, 未找到状态, 已退出状态);
    const auto *节点 =
        读取.事实 ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
    if (读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.查询编码 != 编码 ||
        读取.物理清理事实代次 || 读取.物理清理墓碑 || !节点 ||
        节点->编码 != 编码)
      return 任务虚拟存在专用状态::内部不一致;
    if (节点->写入所有者 != 所有者)
      return 任务虚拟存在专用状态::所有者不匹配;
    if (节点->退出事实代次)
      return 已退出状态;
    if (节点->种类 != 节点种类::普通 || 节点->属性类型表示 ||
        节点->创建事实代次 == 0 || 节点->创建事实代次 > 期望事实代次 ||
        !节点->当前属性.empty())
      return 任务虚拟存在专用状态::结构冲突;
    return std::nullopt;
  }

  static 任务虚拟存在专用状态
  映射任务虚拟存在L1状态(L1所有者范围读取状态 状态,
                         任务虚拟存在专用状态 未找到状态,
                         任务虚拟存在专用状态 已退出状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::未找到:
      return 未找到状态;
    case L1所有者范围读取状态::已退出:
      return 已退出状态;
    case L1所有者范围读取状态::事实代次漂移:
      return 任务虚拟存在专用状态::事实代次漂移;
    case L1所有者范围读取状态::资源失败:
      return 任务虚拟存在专用状态::资源失败;
    case L1所有者范围读取状态::入口拒绝:
      return 任务虚拟存在专用状态::入口拒绝;
    default:
      return 任务虚拟存在专用状态::内部不一致;
    }
  }

  static 任务虚拟存在专用状态
  映射任务虚拟存在一致状态(L1所有者范围一致当前读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围一致当前读取状态::事实代次漂移:
      return 任务虚拟存在专用状态::事实代次漂移;
    case L1所有者范围一致当前读取状态::资源失败:
      return 任务虚拟存在专用状态::资源失败;
    case L1所有者范围一致当前读取状态::入口拒绝:
      return 任务虚拟存在专用状态::入口拒绝;
    default:
      return 任务虚拟存在专用状态::内部不一致;
    }
  }

  static bool
  任务虚拟存在投影头完整(const L1所有者范围一致当前读取结果 &读取,
                         const L1所有者范围一致当前读取请求 &请求) noexcept {
    if (读取.状态 != L1所有者范围一致当前读取状态::成功 ||
        读取.合同版本 != L1所有者范围一致当前读取合同版本 ||
        读取.期望事实代次 != 请求.期望事实代次 ||
        读取.读取事实代次 != 请求.期望事实代次 || 读取.所有者.size() != 1 ||
        读取.节点.size() != 请求.节点.size() || 读取.关系.size() != 1 ||
        !读取.值.empty() || !读取.属性值.empty() || !读取.源关系组.empty() ||
        !读取.目标关系组.empty())
      return false;
    const auto &所有者 = 读取.所有者.front();
    return 所有者.查询所有者 == 请求.所有者.front() &&
           所有者.状态 == L1所有者范围一致当前读取项目状态::成功 &&
           所有者.所有者事实 &&
           所有者.所有者事实->所有者 == 请求.所有者.front() &&
           所有者.所有者事实->范围种类 == L1所有者范围种类::独占结构范围 &&
           所有者.所有者事实->创建事实代次 != 0 &&
           所有者.所有者事实->创建事实代次 <= 请求.期望事实代次 &&
           !所有者.所有者事实->退出事实代次;
  }

  static const L1所有者范围节点事实 *
  查找任务虚拟存在节点(const L1所有者范围一致当前读取结果 &读取,
                       稳定编码 编码) noexcept {
    const L1所有者范围节点事实 *找到 = nullptr;
    for (const auto &项 : 读取.节点) {
      if (项.查询编码 != 编码)
        continue;
      if (找到 || 项.状态 != L1所有者范围一致当前读取项目状态::成功 ||
          !项.事实 || 项.物理清理事实代次 || 项.物理清理墓碑)
        return nullptr;
      找到 = &*项.事实;
    }
    return 找到;
  }

  static const L1所有者范围关系事实 *
  查找任务虚拟存在关系(const L1所有者范围一致当前读取结果 &读取,
                       稳定编码 编码) noexcept {
    const L1所有者范围关系事实 *找到 = nullptr;
    for (const auto &项 : 读取.关系) {
      if (项.查询编码 != 编码)
        continue;
      if (找到 || 项.状态 != L1所有者范围一致当前读取项目状态::成功 ||
          !项.事实 || 项.物理清理事实代次 || 项.物理清理墓碑)
        return nullptr;
      找到 = &*项.事实;
    }
    return 找到;
  }

  static bool 任务虚拟存在节点完整(const L1所有者范围节点事实 &节点,
                                   L1结构所有者身份 所有者) noexcept {
    return 有效(节点.编码) && 节点.写入所有者 == 所有者 &&
           节点.种类 == 节点种类::普通 && !节点.属性类型表示 &&
           节点.创建事实代次 != 0 && !节点.退出事实代次 &&
           节点.当前属性.empty();
  }

  struct 关系组读取值 final {
    bool 成功 = false;
    存在类数据状态 状态 = 存在类数据状态::内部不一致;
    std::uint64_t 事实代次 = 0;
    std::vector<L1所有者范围关系事实> 关系组;
  };

  bool 关系类型组有效() const noexcept {
    const std::vector<稳定编码> 类型组{子存在关系类型_, 特征关系类型_,
                                       当前采用关系类型_};
    for (std::size_t i = 0; i < 类型组.size(); ++i) {
      if (!有效(类型组[i]))
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (类型组[i] == 类型组[j])
          return false;
    }
    return true;
  }

  bool 关系类型有效(稳定编码 编码) const {
    const auto 读取 =
        第一层服务_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, 编码});
    const auto *事实 =
        读取.事实 ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
    return 读取.状态 == L1所有者范围读取状态::成功 &&
           读取.合同版本 == L1所有者范围CRUD合同版本 && 读取.查询编码 == 编码 &&
           !读取.物理清理事实代次 && !读取.物理清理墓碑 && 事实 &&
           事实->编码 == 编码 && 事实->写入所有者 == 所有者_ &&
           事实->种类 == 节点种类::普通 && !事实->属性类型表示 &&
           事实->创建事实代次 != 0 && !事实->退出事实代次;
  }

  static bool 成员种类有效(存在类成员种类 种类) noexcept {
    return 种类 == 存在类成员种类::子存在 || 种类 == 存在类成员种类::特征;
  }



  static bool 查询请求有效(const 存在类查询请求 &请求) noexcept {
    return 请求.合同版本 == 存在类数据合同版本 && 请求.期望事实代次 != 0 &&
           有效(请求.存在结点);
  }

  static bool 成员新增请求有效(const 存在类成员新增请求 &请求) noexcept {
    return 请求.合同版本 == 存在类数据合同版本 && 请求.期望事实代次 != 0 &&
           有效(请求.幂等身份) && 有效(请求.存在结点) &&
           成员种类有效(请求.成员种类) && 有效(请求.目标结点) &&
           (请求.成员种类 != 存在类成员种类::子存在 ||
            请求.存在结点 != 请求.目标结点);
  }

  static bool 成员删除请求有效(const 存在类成员删除请求 &请求) noexcept {
    return 请求.合同版本 == 存在类数据合同版本 && 请求.期望事实代次 != 0 &&
           有效(请求.幂等身份) && 有效(请求.存在结点) &&
           成员种类有效(请求.成员种类) && 有效(请求.目标结点);
  }

  static bool 删除请求有效(const 存在类删除请求 &请求) noexcept {
    return 请求.合同版本 == 存在类数据合同版本 && 请求.期望事实代次 != 0 &&
           有效(请求.幂等身份) && 有效(请求.存在结点);
  }

  稳定编码 关系类型(存在类成员种类 种类) const noexcept {
    switch (种类) {
    case 存在类成员种类::子存在:
      return 子存在关系类型_;
    case 存在类成员种类::特征:
      return 特征关系类型_;
    }
    return {};
  }

  static const std::vector<存在类成员引用> &
  选择成员组(const 存在类结点 &存在, 存在类成员种类 种类) noexcept {
    switch (种类) {
    case 存在类成员种类::子存在:
      return 存在.子存在组;
    case 存在类成员种类::特征:
      return 存在.特征组;
    }
    return 存在.子存在组;
  }

  static std::vector<存在类成员引用> &选择成员组(存在类结点 &存在,
                                                 存在类成员种类 种类) noexcept {
    switch (种类) {
    case 存在类成员种类::子存在:
      return 存在.子存在组;
    case 存在类成员种类::特征:
      return 存在.特征组;
    }
    return 存在.子存在组;
  }

  static const 存在类成员引用 *
  查找成员(const std::vector<存在类成员引用> &成员组, 稳定编码 目标) noexcept {
    const auto it = std::find_if(
        成员组.begin(), 成员组.end(),
        [&](const auto &成员) noexcept { return 成员.目标结点 == 目标; });
    return it == 成员组.end() ? nullptr : &*it;
  }

  std::optional<存在类数据状态> 验证成员目标(存在类成员种类 种类, 稳定编码 目标,
                                             std::uint64_t 期望事实代次) const {
    switch (种类) {
    case 存在类成员种类::子存在: {
      const auto 读取 = 读取当前存在本体(目标, 期望事实代次);
      if (读取.成功() && 读取.状态 == 存在类数据状态::已读取)
        return std::nullopt;
      if (读取.状态 == 存在类数据状态::事实代次漂移 ||
          读取.状态 == 存在类数据状态::资源失败 ||
          读取.状态 == 存在类数据状态::入口拒绝)
        return 读取.状态;
      return 存在类数据状态::子存在读取失败;
    }
    case 存在类成员种类::特征: {
      const auto read =
          特征服务_.读取准确特征事实({1, 期望事实代次, 期望事实代次, {目标}});
      if (const auto *error = std::get_if<特征数据错误>(&read))
        return 映射特征状态(*error);
      const auto &f = std::get<准确特征读取事实>(read);
      if (f.Gread != 期望事实代次 || f.H != 期望事实代次 ||
          f.信息.身份.编码 != 目标)
        return 存在类数据状态::内部不一致;
      return std::nullopt;
    }
    }
    return 存在类数据状态::入口拒绝;
  }

  存在类结点结果 读取当前存在本体(稳定编码 存在结点,
                                  std::uint64_t 期望事实代次) const {
    const auto 身份 = 确认当前存在结构身份(期望事实代次, 存在结点);
    if (!身份.成功(期望事实代次))
      return 失败(映射窄数据状态(身份.状态), 身份.Gread);
    const auto 读取 = 第一层服务_.读取所有者范围当前节点(
        {L1所有者范围CRUD合同版本, 存在结点});
    if (读取.读取事实代次 != 期望事实代次)
      return 失败(存在类数据状态::事实代次漂移, 读取.读取事实代次);
    if (读取.状态 != L1所有者范围读取状态::成功)
      return 失败(映射读取状态(读取.状态), 读取.读取事实代次);
    const auto *节点 =
        读取.事实 ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
    if (读取.合同版本 != L1所有者范围CRUD合同版本 ||
        读取.查询编码 != 存在结点 || 读取.物理清理事实代次 ||
        读取.物理清理墓碑 || !节点 || 节点->编码 != 存在结点 ||
        节点->写入所有者 != 所有者_ || 节点->种类 != 节点种类::普通 ||
        节点->属性类型表示 || 节点->创建事实代次 == 0 || 节点->退出事实代次 ||
        !节点->当前属性.empty())
      return 失败(存在类数据状态::内部不一致, 读取.读取事实代次);
    存在类结点 结果;
    结果.结点 = 存在结点;
    结果.创建事实代次 = 节点->创建事实代次;
    return {存在类数据状态::已读取, 存在类数据合同版本, 读取.读取事实代次,
            std::move(结果)};
  }

  关系组读取值 读取当前源关系组(稳定编码 存在结点, 稳定编码 类型,
                                std::uint64_t 期望事实代次) const {
    const auto 读取 = 第一层服务_.读取所有者范围当前源关系组(
        {L1所有者范围CRUD合同版本, 存在结点, 类型});
    if (读取.读取事实代次 != 期望事实代次)
      return {false, 存在类数据状态::事实代次漂移, 读取.读取事实代次, {}};
    if (读取.状态 != L1所有者范围读取状态::成功)
      return {false, 映射读取状态(读取.状态), 读取.读取事实代次, {}};
    if (读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.源节点 != 存在结点 ||
        读取.关系类型节点 != 类型)
      return {false, 存在类数据状态::内部不一致, 读取.读取事实代次, {}};
    auto 关系组 = 读取.关系组;
    if (!规范化关系组(关系组, 存在结点, 类型, std::nullopt, std::nullopt))
      return {false, 存在类数据状态::内部不一致, 读取.读取事实代次, {}};
    return {true, 存在类数据状态::已读取, 读取.读取事实代次, std::move(关系组)};
  }

  存在类结点结果 读取当前存在(稳定编码 存在结点, std::uint64_t 期望事实代次,
                              bool 验证目标) const {
    auto 本体 = 读取当前存在本体(存在结点, 期望事实代次);
    if (!本体.成功() || 本体.状态 != 存在类数据状态::已读取)
      return 本体;
    const std::vector<std::pair<存在类成员种类, 稳定编码>> 分类{
        {存在类成员种类::子存在, 子存在关系类型_},
        {存在类成员种类::特征, 特征关系类型_}};
    for (const auto &[种类, 类型] : 分类) {
      auto 读取 = 读取当前源关系组(存在结点, 类型, 期望事实代次);
      if (!读取.成功)
        return 失败(读取.状态, 读取.事实代次);
      auto &目标组 = 选择成员组(*本体.存在结点, 种类);
      for (const auto &关系 : 读取.关系组) {
        if (验证目标) {
          if (const auto 状态 = 验证成员目标(种类, 关系.目标节点, 期望事实代次))
            return 失败(*状态, 期望事实代次);
        }
        目标组.push_back({关系.编码, 关系.目标节点});
      }
    }
    if (有效(当前采用关系类型_)) {
      try {
        本体.存在结点->当前采用组 =
            读取采用组(存在结点, 期望事实代次, 期望事实代次,
                       std::numeric_limits<std::uint64_t>::max())
                .采用;
      } catch (const 采用失败 &e) {
        return 失败(e.状态, 期望事实代次);
      }
    }
    return 本体;
  }

  L1所有者范围写集请求 形成新增成员写集(const 存在类成员新增请求 &请求) const {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.期望事实代次;
    写集.写入幂等身份 = 请求.幂等身份;
    写集.关系 = {{存在类数据内部::成员关系本地键, 请求.存在结点, 请求.目标结点,
                  关系类型(请求.成员种类), 1}};
    return 写集;
  }

  L1所有者范围写集请求 形成删除存在写集(const 存在类删除请求 &请求,
                                        const 存在类结点 &当前) const {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.期望事实代次;
    写集.写入幂等身份 = 请求.幂等身份;
    写集.退出事实 = {请求.存在结点};
    const auto 加入 = [&](const auto &成员组) {
      for (const auto &成员 : 成员组)
        写集.退出事实.push_back(成员.成员关系);
    };
    加入(当前.子存在组);
    加入(当前.特征组);
    for (const auto &a : 当前.当前采用组)
      写集.退出事实.push_back(a.关系);
    return 写集;
  }



  存在类结点结果 提交成员变更(const L1所有者范围写集请求 &写集,
                              L1所有者范围写入幂等身份 幂等身份,
                              存在类数据状态 成功状态, 稳定编码 存在结点,
                              存在类成员种类 种类, 稳定编码 目标, bool 添加) {
    const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
    const auto 状态 = 映射写入状态(写入.状态, 成功状态);
    if (状态 != 成功状态 && 状态 != 存在类数据状态::精确重复)
      return 失败(状态, 写入.事实代次);
    try {
      const std::size_t 期望映射数量 = 添加 ? 1U : 0U;
      if (!写入结果头完整(写入, 幂等身份))
        return 失败(存在类数据状态::内部不一致, 写入.事实代次);
      if (写入.新编码映射.size() != 期望映射数量)
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      std::optional<稳定编码> 新关系;
      if (添加) {
        新关系 =
            存在类数据内部::查找唯一编码(写入, 存在类数据内部::成员关系本地键);
        if (!新关系)
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      } else if (写集.退出事实.size() != 1 ||
                 !退出事实同代闭合(写集.退出事实, 写入.事实代次)) {
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      }
      std::uint64_t 读取守卫事实代次 = 写入.事实代次;
      if (状态 == 存在类数据状态::精确重复) {
        const auto 当前 = 读取当前事实代次();
        if (当前.first != 存在结构身份只读状态::已读取)
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
        读取守卫事实代次 = 当前.second;
      }
      auto 读回 = 读取历史存在快照(存在结点, 写入.事实代次, 写入.事实代次, 状态,
                                   std::nullopt, 读取守卫事实代次);
      if (!读回.成功() || !读回.存在结点)
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      const auto *成员 = 查找成员(选择成员组(*读回.存在结点, 种类), 目标);
      if ((添加 && (!成员 || 成员->成员关系 != *新关系)) || (!添加 && 成员))
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      读回.状态 = 状态;
      return 读回;
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::已可能发布, 写入.事实代次);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::已可能发布, 写入.事实代次);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致, 写入.事实代次);
    }
  }



  std::optional<存在类结点结果>
  尝试重放新增成员(const 存在类成员新增请求 &请求) {
    const auto 首次 = 读取首次写入(请求.幂等身份);
    if (!首次)
      return std::nullopt;
    if (首次->状态 != L1所有者范围读取状态::成功 || !首次->首次规范化写集 ||
        !首次->首次写入结果)
      return 失败(映射读取状态(首次->状态), 首次->读取事实代次);
    const auto 期望 = 形成新增成员写集(请求);
    if (*首次->首次规范化写集 != 期望 ||
        首次->首次写入结果->状态 != L1所有者范围写入状态::成功)
      return 失败(存在类数据状态::幂等冲突, 首次->首次写入结果->事实代次);
    return 提交成员变更(期望, 请求.幂等身份, 存在类数据状态::精确重复,
                        请求.存在结点, 请求.成员种类, 请求.目标结点, true);
  }

  std::optional<存在类结点结果>
  尝试重放删除成员(const 存在类成员删除请求 &请求) {
    const auto 首次 = 读取首次写入(请求.幂等身份);
    if (!首次)
      return std::nullopt;
    if (首次->状态 != L1所有者范围读取状态::成功 || !首次->首次规范化写集 ||
        !首次->首次写入结果)
      return 失败(映射读取状态(首次->状态), 首次->读取事实代次);
    const auto &写集 = *首次->首次规范化写集;
    if (写集.合同版本 != L1所有者范围CRUD合同版本 ||
        写集.期望事实代次 != 请求.期望事实代次 ||
        写集.写入幂等身份 != 请求.幂等身份 || !写集.节点.empty() ||
        !写集.关系.empty() || !写集.值.empty() || !写集.属性槽变更.empty() ||
        写集.退出事实.size() != 1 ||
        首次->首次写入结果->状态 != L1所有者范围写入状态::成功)
      return 失败(存在类数据状态::幂等冲突, 首次->首次写入结果->事实代次);
    const auto 历史 = 第一层服务_.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 写集.退出事实.front()});
    const auto *关系 =
        历史.事实 ? std::get_if<L1所有者范围关系事实>(&*历史.事实) : nullptr;
    if (历史.状态 != L1所有者范围读取状态::成功 || !关系 ||
        关系->编码 != 写集.退出事实.front() || 关系->写入所有者 != 所有者_ ||
        关系->源节点 != 请求.存在结点 || 关系->目标节点 != 请求.目标结点 ||
        关系->关系类型节点 != 关系类型(请求.成员种类) ||
        关系->退出事实代次 != 首次->首次写入结果->事实代次)
      return 失败(存在类数据状态::幂等冲突, 首次->首次写入结果->事实代次);
    return 提交成员变更(写集, 请求.幂等身份, 存在类数据状态::精确重复,
                        请求.存在结点, 请求.成员种类, 请求.目标结点, false);
  }

  std::optional<存在类结点结果> 尝试重放删除存在(const 存在类删除请求 &请求) {
    const auto 首次 = 读取首次写入(请求.幂等身份);
    if (!首次)
      return std::nullopt;
    if (首次->状态 != L1所有者范围读取状态::成功 || !首次->首次规范化写集 ||
        !首次->首次写入结果)
      return 失败(映射读取状态(首次->状态), 首次->读取事实代次);
    const auto &写集 = *首次->首次规范化写集;
    const auto &结果 = *首次->首次写入结果;
    if (写集.合同版本 != L1所有者范围CRUD合同版本 ||
        写集.期望事实代次 != 请求.期望事实代次 ||
        写集.写入幂等身份 != 请求.幂等身份 || !写集.节点.empty() ||
        !写集.关系.empty() || !写集.值.empty() || !写集.属性槽变更.empty() ||
        写集.退出事实.empty() ||
        std::count(写集.退出事实.begin(), 写集.退出事实.end(), 请求.存在结点) !=
            1 ||
        结果.状态 != L1所有者范围写入状态::成功 ||
        !删除存在写集闭合(写集, 请求.存在结点, 结果.事实代次))
      return 失败(存在类数据状态::幂等冲突, 结果.事实代次);
    const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
    if (写入.状态 != L1所有者范围写入状态::精确重复 ||
        !写入结果头完整(写入, 请求.幂等身份))
      return 失败(存在类数据状态::内部不一致, 写入.事实代次);
    try {
      if (!写入.新编码映射.empty() ||
          !退出事实同代闭合(写集.退出事实, 写入.事实代次))
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      const auto 当前 = 读取当前事实代次();
      if (当前.first != 存在结构身份只读状态::已读取)
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      auto 读回 = 读取历史存在快照(请求.存在结点, 写入.事实代次 - 1,
                                   写入.事实代次, 存在类数据状态::精确重复,
                                   写入.事实代次, 当前.second);
      if (!读回.成功())
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      读回.状态 = 存在类数据状态::精确重复;
      return 读回;
    } catch (const std::bad_alloc &) {
      return 失败(存在类数据状态::已可能发布, 写入.事实代次);
    } catch (const std::length_error &) {
      return 失败(存在类数据状态::已可能发布, 写入.事实代次);
    } catch (...) {
      return 失败(存在类数据状态::内部不一致, 写入.事实代次);
    }
  }

  std::optional<L1所有者范围首次写入读取结果>
  读取首次写入(L1所有者范围写入幂等身份 幂等身份) const {
    const auto 首次 = 写入端口_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 幂等身份});
    if (首次.状态 == L1所有者范围读取状态::未找到)
      return std::nullopt;
    if (首次.合同版本 != L1所有者范围首次写入读取合同版本 ||
        首次.所有者 != 所有者_ || 首次.写入幂等身份 != 幂等身份)
      return L1所有者范围首次写入读取结果{};
    return 首次;
  }

  存在类结点结果 读取历史存在快照(稳定编码 存在结点,
                                  std::uint64_t 历史截止事实代次,
                                  std::uint64_t 结果事实代次,
                                  存在类数据状态 返回状态,
                                  std::optional<std::uint64_t> 要求退出事实代次,
                                  std::uint64_t 读取守卫事实代次 = 0) const {
    if (历史截止事实代次 == 0 || 结果事实代次 == 0)
      return 失败(存在类数据状态::内部不一致, 结果事实代次);
    if (读取守卫事实代次 == 0)
      读取守卫事实代次 = 结果事实代次;
    const auto 身份 =
        读取存在身份来源历史见证(读取守卫事实代次, 历史截止事实代次, 存在结点);
    if (!身份.成功(读取守卫事实代次, 历史截止事实代次, 存在结点))
      return 失败(映射窄数据状态(身份.状态), 身份.Gread);
    const auto 节点读取 = 第一层服务_.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 存在结点});
    if (节点读取.状态 != L1所有者范围读取状态::成功)
      return 失败(映射读取状态(节点读取.状态), 节点读取.读取事实代次);
    const auto *节点 = 节点读取.事实
                           ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实)
                           : nullptr;
    if (节点读取.合同版本 != L1所有者范围CRUD合同版本 ||
        节点读取.查询编码 != 存在结点 || !节点 || 节点->编码 != 存在结点 ||
        节点->写入所有者 != 所有者_ || 节点->种类 != 节点种类::普通 ||
        节点->属性类型表示 || 节点->创建事实代次 == 0 ||
        节点->创建事实代次 > 历史截止事实代次 ||
        (节点->退出事实代次 && *节点->退出事实代次 <= 历史截止事实代次) ||
        (要求退出事实代次 && 节点->退出事实代次 != 要求退出事实代次) ||
        !节点->当前属性.empty())
      return 失败(存在类数据状态::内部不一致, 节点读取.读取事实代次);
    存在类结点 结果;
    结果.结点 = 存在结点;
    结果.创建事实代次 = 节点->创建事实代次;
    结果.退出事实代次 = 要求退出事实代次;
    const std::vector<std::pair<存在类成员种类, 稳定编码>> 分类{
        {存在类成员种类::子存在, 子存在关系类型_},
        {存在类成员种类::特征, 特征关系类型_}};
    for (const auto &[种类, 类型] : 分类) {
      const auto 读取 = 第一层服务_.读取所有者范围历史关系组(
          {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, 存在结点,
           类型, 历史截止事实代次});
      if (读取.状态 != L1所有者范围读取状态::成功)
        return 失败(映射读取状态(读取.状态), 读取.读取事实代次);
      if (读取.合同版本 != L1所有者范围CRUD合同版本 ||
          读取.方向 != L1所有者范围关系端点方向::源 ||
          读取.端点节点 != 存在结点 || 读取.关系类型节点 != 类型 ||
          读取.历史截止事实代次 != 历史截止事实代次)
        return 失败(存在类数据状态::内部不一致, 读取.读取事实代次);
      auto 关系组 = 读取.关系组;
      if (!规范化关系组(关系组, 存在结点, 类型, 历史截止事实代次,
                        要求退出事实代次))
        return 失败(存在类数据状态::内部不一致, 读取.读取事实代次);
      auto &成员组 = 选择成员组(结果, 种类);
      for (const auto &关系 : 关系组)
        成员组.push_back({关系.编码, 关系.目标节点});
    }
    if (有效(当前采用关系类型_)) {
      try {
        结果.当前采用组 =
            读取采用组(存在结点, 节点读取.读取事实代次, 历史截止事实代次,
                       std::numeric_limits<std::uint64_t>::max())
                .采用;
      } catch (const 采用失败 &e) {
        return 失败(e.状态, 结果事实代次);
      }
    }
    return {返回状态, 存在类数据合同版本, 结果事实代次, std::move(结果)};
  }

  关系组读取值 读取当前父关系(稳定编码 子存在,
                              std::uint64_t 期望事实代次) const {
    const auto 读取 = 第一层服务_.读取所有者范围当前目标关系组(
        {L1所有者范围CRUD合同版本, 子存在, 子存在关系类型_});
    if (读取.读取事实代次 != 期望事实代次)
      return {false, 存在类数据状态::事实代次漂移, 读取.读取事实代次, {}};
    if (读取.状态 != L1所有者范围读取状态::成功)
      return {false, 映射读取状态(读取.状态), 读取.读取事实代次, {}};
    if (读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.目标节点 != 子存在 ||
        读取.关系类型节点 != 子存在关系类型_)
      return {false, 存在类数据状态::内部不一致, 读取.读取事实代次, {}};
    auto 关系组 = 读取.关系组;
    std::sort(关系组.begin(), 关系组.end(),
              [](const auto &左, const auto &右) noexcept {
                if (左.源节点 != 右.源节点)
                  return 左.源节点.值 < 右.源节点.值;
                return 左.编码.值 < 右.编码.值;
              });
    for (std::size_t i = 0; i < 关系组.size(); ++i) {
      const auto &关系 = 关系组[i];
      if (!有效(关系.编码) || !有效(关系.源节点) || 关系.目标节点 != 子存在 ||
          关系.关系类型节点 != 子存在关系类型_ || 关系.写入所有者 != 所有者_ ||
          关系.角色或顺序 != 1 || 关系.退出事实代次)
        return {false, 存在类数据状态::内部不一致, 读取.读取事实代次, {}};
      if (i != 0 && 关系组[i - 1].源节点 == 关系.源节点)
        return {false, 存在类数据状态::内部不一致, 读取.读取事实代次, {}};
      const auto 父 = 读取当前存在本体(关系.源节点, 期望事实代次);
      if (!父.成功() || 父.状态 != 存在类数据状态::已读取)
        return {false, 存在类数据状态::子存在读取失败, 父.事实代次, {}};
    }
    return {true, 存在类数据状态::已读取, 读取.读取事实代次, std::move(关系组)};
  }

  bool 规范化关系组(std::vector<L1所有者范围关系事实> &关系组, 稳定编码 源,
                    稳定编码 类型,
                    std::optional<std::uint64_t> 历史截止事实代次,
                    std::optional<std::uint64_t> 要求退出事实代次) const {
    std::sort(关系组.begin(), 关系组.end(),
              [](const auto &左, const auto &右) noexcept {
                if (左.目标节点 != 右.目标节点)
                  return 左.目标节点.值 < 右.目标节点.值;
                return 左.编码.值 < 右.编码.值;
              });
    for (std::size_t i = 0; i < 关系组.size(); ++i) {
      const auto &关系 = 关系组[i];
      if (!有效(关系.编码) || 关系.源节点 != 源 || !有效(关系.目标节点) ||
          关系.关系类型节点 != 类型 || 关系.写入所有者 != 所有者_ ||
          关系.角色或顺序 != 1 || (!历史截止事实代次 && 关系.退出事实代次) ||
          (历史截止事实代次 &&
           (关系.创建事实代次 > *历史截止事实代次 ||
            (关系.退出事实代次 && *关系.退出事实代次 <= *历史截止事实代次))) ||
          (要求退出事实代次 && 关系.退出事实代次 != 要求退出事实代次))
        return false;
      if (i != 0 && 关系组[i - 1].目标节点 == 关系.目标节点)
        return false;
    }
    return true;
  }

  bool 退出事实同代闭合(const std::vector<稳定编码> &编码组,
                        std::uint64_t 退出事实代次) const {
    if (编码组.empty() || 退出事实代次 == 0)
      return false;
    for (const auto 编码 : 编码组) {
      const auto 读取 =
          第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 编码});
      if (读取.状态 != L1所有者范围读取状态::成功 ||
          读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.查询编码 != 编码 ||
          !读取.事实)
        return false;
      bool 闭合 = false;
      if (const auto *节点 = std::get_if<L1所有者范围节点事实>(&*读取.事实))
        闭合 = 节点->编码 == 编码 && 节点->写入所有者 == 所有者_ &&
               节点->退出事实代次 == 退出事实代次;
      else if (const auto *关系 =
                   std::get_if<L1所有者范围关系事实>(&*读取.事实))
        闭合 = 关系->编码 == 编码 && 关系->写入所有者 == 所有者_ &&
               关系->退出事实代次 == 退出事实代次;
      if (!闭合)
        return false;
    }
    return true;
  }

  bool 删除存在写集闭合(const L1所有者范围写集请求 &写集, 稳定编码 存在结点,
                        std::uint64_t 退出事实代次) const {
    bool 找到存在结点 = false;
    for (const auto 编码 : 写集.退出事实) {
      const auto 读取 =
          第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 编码});
      if (读取.状态 != L1所有者范围读取状态::成功 ||
          读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.查询编码 != 编码 ||
          !读取.事实)
        return false;
      if (const auto *节点 = std::get_if<L1所有者范围节点事实>(&*读取.事实)) {
        if (编码 != 存在结点 || 找到存在结点 || 节点->编码 != 存在结点 ||
            节点->写入所有者 != 所有者_ || 节点->种类 != 节点种类::普通 ||
            节点->属性类型表示 || 节点->退出事实代次 != 退出事实代次)
          return false;
        找到存在结点 = true;
        continue;
      }
      const auto *关系 = std::get_if<L1所有者范围关系事实>(&*读取.事实);
      if (!关系 || 关系->编码 != 编码 || 关系->写入所有者 != 所有者_ ||
          关系->退出事实代次 != 退出事实代次)
        return false;
      const bool 是出边 = 关系->源节点 == 存在结点 &&
                          (关系->关系类型节点 == 子存在关系类型_ ||
                           关系->关系类型节点 == 特征关系类型_ ||
                           关系->关系类型节点 == 当前采用关系类型_ ||
                           关系->关系类型节点 == 存在族归属关系类型_);
      const bool 是父边 =
          关系->目标节点 == 存在结点 && 关系->关系类型节点 == 子存在关系类型_;
      if ((!是出边 && !是父边) || 关系->角色或顺序 != 1)
        return false;
    }
    if (!找到存在结点 || 退出事实代次 <= 1)
      return false;
    const auto 当前 = 读取当前事实代次();
    if (当前.first != 存在结构身份只读状态::已读取)
      return false;
    const auto 来源 =
        读取存在身份来源历史见证(当前.second, 退出事实代次 - 1, 存在结点);
    return 来源.成功(当前.second, 退出事实代次 - 1, 存在结点) && 来源.见证 &&
           std::count(写集.退出事实.begin(), 写集.退出事实.end(),
                      来源.见证->族归属关系) == 1;
  }

  bool 写入结果头完整(const L1所有者范围写入结果 &写入,
                      L1所有者范围写入幂等身份 幂等身份) const noexcept {
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功 &&
                          写入.是否形成内存权威发布 &&
                          写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 =
        写入.状态 == L1所有者范围写入状态::精确重复 &&
        !写入.是否形成内存权威发布 &&
        写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    return 写入.合同版本 == L1所有者范围CRUD合同版本 &&
           写入.所有者 == 所有者_ && 写入.写入幂等身份 == 幂等身份 &&
           写入.事实代次 != 0 && (首次完整 || 重复完整);
  }

  static 存在类结点结果 失败(存在类数据状态 状态,
                             std::uint64_t 事实代次 = 0) noexcept {
    return {状态, 存在类数据合同版本, 事实代次, std::nullopt};
  }

  static 存在类数据状态 映射特征状态(特征数据错误 s) noexcept {
    using S = 存在类数据状态;
    switch (s) {
    case 特征数据错误::未找到:
      return S::未找到;
    case 特征数据错误::已退出:
      return S::目标已退出;
    case 特征数据错误::并发变化:
      return S::事实代次漂移;
    case 特征数据错误::入口拒绝:
      return S::入口拒绝;
    case 特征数据错误::资源失败:
      return S::资源失败;
    case 特征数据错误::历史材料不可用:
      return S::历史材料已清理;
    case 特征数据错误::类型不相容:
    case 特征数据错误::引用冲突:
      return S::引用冲突;
    case 特征数据错误::旧格式不支持:
      return S::格式不支持;
    case 特征数据错误::数量预算不足:
      return S::数量预算不足;
    case 特征数据错误::内部不一致:
      return S::内部不一致;
    default:
      return S::特征读取失败;
    }
  }

  static 存在类数据状态 映射读取状态(L1所有者范围读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::未找到:
      return 存在类数据状态::未找到;
    case L1所有者范围读取状态::已退出:
      return 存在类数据状态::目标已退出;
    case L1所有者范围读取状态::事实代次漂移:
      return 存在类数据状态::事实代次漂移;
    case L1所有者范围读取状态::历史材料已清理:
      return 存在类数据状态::历史材料已清理;
    case L1所有者范围读取状态::入口拒绝:
      return 存在类数据状态::入口拒绝;
    case L1所有者范围读取状态::资源失败:
      return 存在类数据状态::资源失败;
    default:
      return 存在类数据状态::内部不一致;
    }
  }

  static 存在类数据状态 映射写入状态(L1所有者范围写入状态 状态,
                                     存在类数据状态 成功状态) noexcept {
    switch (状态) {
    case L1所有者范围写入状态::成功:
      return 成功状态;
    case L1所有者范围写入状态::精确重复:
      return 存在类数据状态::精确重复;
    case L1所有者范围写入状态::入口拒绝:
      return 存在类数据状态::入口拒绝;
    case L1所有者范围写入状态::未找到:
      return 存在类数据状态::结构类型未找到;
    case L1所有者范围写入状态::已退出:
      return 存在类数据状态::结构类型已退出;
    case L1所有者范围写入状态::事实代次漂移:
      return 存在类数据状态::事实代次漂移;
    case L1所有者范围写入状态::幂等冲突:
      return 存在类数据状态::幂等冲突;
    case L1所有者范围写入状态::引用冲突:
      return 存在类数据状态::引用冲突;
    case L1所有者范围写入状态::资源失败:
      return 存在类数据状态::资源失败;
    default:
      return 存在类数据状态::内部不一致;
    }
  }

  static 存在历史读取状态 存在历史状态(L1所有者范围读取状态 s) noexcept {
    using S = 存在历史读取状态;
    switch (s) {
    case L1所有者范围读取状态::成功:
      return S::已读取;
    case L1所有者范围读取状态::未找到:
      return S::未找到;
    case L1所有者范围读取状态::已退出:
      return S::目标已退出;
    case L1所有者范围读取状态::历史材料已清理:
      return S::历史材料不可用;
    case L1所有者范围读取状态::事实代次漂移:
      return S::事实代次漂移;
    case L1所有者范围读取状态::资源失败:
      return S::资源失败;
    default:
      return S::内部不一致;
    }
  }
  using 采用S = 存在类数据状态;
  struct 采用失败 {
    采用S 状态;
  };
  struct 采用事实集 {
    std::vector<L1所有者范围关系事实> 已知;
    std::vector<存在当前采用事实> 采用;
  };
  struct 采用准备 {
    L1所有者范围写集请求 写集;
    std::optional<存在当前采用事实> 当前;
  };
  static void 采用要求(bool yes, 采用S s = 采用S::内部不一致) {
    if (!yes)
      throw 采用失败{s};
  }
  std::uint64_t 采用当前G() const {
    const auto r = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
    采用要求(r.状态 == L1中性读取状态::成功, r.状态 == L1中性读取状态::资源失败
                                                 ? 采用S::资源失败
                                                 : 采用S::内部不一致);
    采用要求(r.合同版本 == L1中性CRUD合同版本 && r.事实代次);
    return r.事实代次;
  }
  void 采用守卫(std::uint64_t g) const {
    采用要求(采用当前G() == g, 采用S::事实代次漂移);
  }
  准确特征读取事实 采用读F(特征信息身份 id, std::uint64_t g,
                           std::uint64_t h) const {
    auto result = 特征服务_.读取准确特征事实({1, g, h, id});
    if (const auto *e = std::get_if<特征数据错误>(&result))
      throw 采用失败{映射特征状态(*e)};
    auto f = std::get<准确特征读取事实>(std::move(result));
    采用要求(f.Gread == g && f.H == h && f.信息.身份 == id && 有效(f.类型关系));
    return f;
  }
  采用事实集 读取采用组(稳定编码 e, std::uint64_t g, std::uint64_t h,
                        std::uint64_t budget) const {
    采用要求(有效(当前采用关系类型_), 采用S::格式不支持);
    采用要求(有效(e) && g && h && h <= g && budget, 采用S::入口拒绝);
    采用守卫(g);
    const auto nr =
        第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, e});
    采用要求(nr.状态 == L1所有者范围读取状态::成功, 映射读取状态(nr.状态));
    采用要求(nr.读取事实代次 == g, 采用S::事实代次漂移);
    const auto *n =
        nr.事实 ? std::get_if<L1所有者范围节点事实>(&*nr.事实) : nullptr;
    采用要求(nr.合同版本 == L1所有者范围CRUD合同版本 && nr.查询编码 == e && n &&
                 n->编码 == e && n->写入所有者 == 所有者_ &&
                 n->种类 == 节点种类::普通 && !n->属性类型表示,
             采用S::引用冲突);
    采用要求(n->创建事实代次 && n->创建事实代次 <= h, 采用S::未找到);
    采用要求(!n->退出事实代次 || *n->退出事实代次 > h, 采用S::目标已退出);
    auto rows = [&](稳定编码 type) {
      const auto r = 第一层服务_.读取所有者范围历史关系组(
          {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, e, type, h});
      采用要求(r.状态 == L1所有者范围读取状态::成功, 映射读取状态(r.状态));
      采用要求(r.读取事实代次 == g, 采用S::事实代次漂移);
      采用要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.端点节点 == e &&
               r.关系类型节点 == type &&
               r.方向 == L1所有者范围关系端点方向::源 &&
               r.历史截止事实代次 == h);
      采用要求(r.关系组.size() <= budget, 采用S::数量预算不足);
      auto out = r.关系组;
      采用要求(规范化关系组(out, e, type, h, std::nullopt));
      return out;
    };
    采用事实集 out;
    out.已知 = rows(特征关系类型_);
    const auto selected = rows(当前采用关系类型_);
    采用要求(selected.size() <= budget - out.已知.size(), 采用S::数量预算不足);
    std::map<std::uint64_t, 特征类型身份> known;
    for (const auto &edge : out.已知) {
      auto f = 采用读F({edge.目标节点}, g, h);
      采用要求(known.emplace(edge.目标节点.值, f.信息.类型).second);
    }
    std::set<std::uint64_t> types;
    for (const auto &edge : selected) {
      auto f = 采用读F({edge.目标节点}, g, h);
      const auto it = known.find(edge.目标节点.值);
      采用要求(it != known.end() && it->second == f.信息.类型, 采用S::引用冲突);
      采用要求(types.insert(f.信息.类型.编码.值).second);
      out.采用.push_back({edge.编码, e, f.信息.类型, f.信息.身份});
    }
    采用守卫(g);
    return out;
  }
  采用准备 准备采用(const 存在当前采用写请求 &r, std::uint64_t g) const {
    采用要求(r.版本 == 1 && r.G && 有效(r.幂等身份) && 有效(r.E) &&
                 有效(r.FT) && r.关系预算 && !r.操作.valueless_by_exception(),
             采用S::入口拒绝);
    auto all = 读取采用组(r.E, g, r.G, r.关系预算);
    采用准备 out;
    auto &ws = out.写集;
    ws.合同版本 = L1所有者范围CRUD合同版本;
    ws.期望事实代次 = r.G;
    ws.写入幂等身份 = r.幂等身份;
    for (const auto &a : all.采用)
      if (a.FT == r.FT) {
        采用要求(!out.当前);
        out.当前 = a;
      }
    std::optional<特征信息身份> next;
    std::optional<存在当前采用事实> expected;
    bool combine = false;
    std::visit(
        [&](const auto &op) {
          using T = std::decay_t<decltype(op)>;
          if constexpr (std::is_same_v<T, 存在当前采用建立>)
            next = op.F;
          else if constexpr (std::is_same_v<T, 存在当前采用替换>) {
            expected = op.预期;
            next = op.新F;
          } else if constexpr (std::is_same_v<T, 存在当前采用解除>)
            expected = op.预期;
          else {
            combine = true;
            next = op.F;
            expected = op.预期;
          }
        },
        r.操作);
    if (expected) {
      采用要求(有效(expected->关系) && expected->E == r.E &&
                   expected->FT == r.FT && 有效(expected->F),
               采用S::入口拒绝);
      const auto old = 采用读F(expected->F, g, r.G);
      采用要求(old.信息.类型 == r.FT && out.当前 == expected, 采用S::引用冲突);
    } else if (out.当前)
      采用要求(next && out.当前->F == *next, 采用S::引用冲突);
    if (next) {
      const auto f = 采用读F(*next, g, r.G);
      采用要求(f.信息.类型 == r.FT, 采用S::引用冲突);
      const bool known =
          std::any_of(all.已知.begin(), all.已知.end(),
                      [&](const auto &e) { return e.目标节点 == next->编码; });
      if (!known) {
        采用要求(combine, 采用S::引用冲突);
        ws.关系.push_back({{1}, r.E, next->编码, 特征关系类型_, 1});
      }
      if (!out.当前 || out.当前->F != *next)
        ws.关系.push_back({{2}, r.E, next->编码, 当前采用关系类型_, 1});
    }
    if (out.当前 && (!next || out.当前->F != *next))
      ws.退出事实.push_back(out.当前->关系);
    // 派发前保证写后完整已知/采用组仍在同一读取预算内，避免明知读回必超限还发布。
    const auto retained =
        all.已知.size() + all.采用.size() - ws.退出事实.size();
    采用要求(retained <= r.关系预算 && ws.关系.size() <= r.关系预算 - retained,
             采用S::数量预算不足);
    return out;
  }
  void 采用互证(const L1所有者范围写集请求 &ws,
                const L1所有者范围写入结果 &saved, std::uint64_t g) const {
    采用要求(saved.合同版本 == L1所有者范围CRUD合同版本 &&
             saved.所有者 == 所有者_ && saved.写入幂等身份 == ws.写入幂等身份 &&
             saved.事实代次 > ws.期望事实代次 && saved.事实代次 <= g &&
             (saved.状态 == L1所有者范围写入状态::成功 ||
              saved.状态 == L1所有者范围写入状态::精确重复) &&
             (saved.状态 != L1所有者范围写入状态::成功 ||
              saved.是否形成内存权威发布) &&
             saved.新编码映射.size() == ws.关系.size());
    std::set<std::uint64_t> ids;
    for (const auto &wanted : ws.关系) {
      const auto id = 存在类数据内部::查找唯一编码(saved, wanted.本地键);
      采用要求(id && ids.insert(id->值).second);
      const auto r =
          第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, *id});
      采用要求(r.状态 == L1所有者范围读取状态::成功, 映射读取状态(r.状态));
      采用要求(r.读取事实代次 == g, 采用S::事实代次漂移);
      const auto *e =
          r.事实 ? std::get_if<L1所有者范围关系事实>(&*r.事实) : nullptr;
      采用要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == *id &&
               e && e->编码 == *id && e->写入所有者 == 所有者_ &&
               e->创建事实代次 == saved.事实代次 &&
               (!e->退出事实代次 || *e->退出事实代次 > saved.事实代次) &&
               e->源节点 == std::get<稳定编码>(wanted.源节点) &&
               e->目标节点 == std::get<稳定编码>(wanted.目标节点) &&
               e->关系类型节点 == std::get<稳定编码>(wanted.关系类型节点) &&
               e->角色或顺序 == wanted.角色或顺序);
    }
    for (auto id : ws.退出事实) {
      const auto r =
          第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
      采用要求(r.状态 == L1所有者范围读取状态::成功, 映射读取状态(r.状态));
      采用要求(r.读取事实代次 == g, 采用S::事实代次漂移);
      const auto *e =
          r.事实 ? std::get_if<L1所有者范围关系事实>(&*r.事实) : nullptr;
      采用要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == id &&
               e && e->编码 == id && e->写入所有者 == 所有者_ &&
               e->退出事实代次 == saved.事实代次);
    }
  }
  using 绑定S = 绑定存在创建状态;
  const L1事实基座服务 &绑定存在底座() const noexcept override {
    return 第一层服务_;
  }
  L1所有者范围写端口 &绑定存在端口() noexcept override { return 写入端口_; }
  bool 绑定存在结构已就绪() const noexcept override {
    return 存在结构登记已就绪();
  }
  bool 绑定存在幂等键可用(L1所有者范围写入幂等身份 k) const noexcept override {
    return k.值 && k.值 != 1 && k != 存在族来源初始化幂等身份 &&
           (k.值 >> 48) != 0x4E43;
  }
  const 存在组成结构只读提供者 &绑定存在组成提供者() const noexcept override {
    return *this;
  }
  static 绑定S 绑定映射(存在结构身份只读状态 s) noexcept {
    using X = 存在结构身份只读状态;
    switch (s) {
    case X::入口拒绝:
      return 绑定S::绑定类型不符;
    case X::未找到:
      return 绑定S::绑定未找到;
    case X::目标已退出:
      return 绑定S::绑定已退出;
    case X::事实代次漂移:
      return 绑定S::事实代次漂移;
    case X::历史材料已清理:
      return 绑定S::历史材料已清理;
    case X::资源失败:
      return 绑定S::资源失败;
    case X::数量预算不足:
      return 绑定S::数量预算不足;
    default:
      return 绑定S::内部不一致;
    }
  }
  template <class T, class F>
  绑定存在参与者结果<T> 绑定保护(std::uint64_t g, std::uint64_t h,
                                 F &&fn) const {
    绑定存在参与者结果<T> out;
    out.Gread = g;
    out.H = h;
    try {
      auto before = 读取当前事实代次();
      if (before.first != 存在结构身份只读状态::已读取)
        throw 绑定映射(before.first);
      if (before.second != g)
        throw 绑定S::事实代次漂移;
      out.数据 = fn();
      auto after = 读取当前事实代次();
      if (after.first != 存在结构身份只读状态::已读取)
        throw 绑定映射(after.first);
      if (after.second != g)
        throw 绑定S::事实代次漂移;
      out.状态 = 绑定S::精确重复;
    } catch (绑定S e) {
      out.状态 = e;
      out.数据.reset();
    } catch (const std::bad_alloc &) {
      out.状态 = 绑定S::资源失败;
      out.数据.reset();
    } catch (const std::length_error &) {
      out.状态 = 绑定S::资源失败;
      out.数据.reset();
    } catch (...) {
      out.状态 = 绑定S::内部不一致;
      out.数据.reset();
    }
    return out;
  }
  绑定存在参与者结果<L1所有者范围首次写入读取结果>
  读取存在出生首次材料(L1所有者范围写入幂等身份 k) const override {
    const auto f =
        写入端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本, k});
    return {绑定S::精确重复, f.读取事实代次, f.读取事实代次, f};
  }
  绑定存在参与者结果<L1有限N分区原子参与者写集_v3>
  准备存在出生片段(const 绑定存在创建请求 &r, std::uint64_t g) const override {
    return 绑定保护<L1有限N分区原子参与者写集_v3>(g, r.G0, [&] {
      const bool single = r.绑定.种类 == 存在初始绑定种类::父存在组成;
      const auto key =
          single ? std::get<存在组成绑定创建键>(r.幂等键).幂等身份
                 : std::get<存在场景绑定创建键>(r.幂等键).存在幂等身份;
      L1有限N分区原子参与者写集_v3 p{
          {1}, 所有者_, {L1所有者范围CRUD合同版本, r.G0, key}};
      p.写集.节点.push_back({{1}, 节点种类::普通, std::nullopt});
      p.写集.关系.push_back({{2},
                             L1所有者范围写集本地键{1},
                             存在族锚点_,
                             存在族归属关系类型_,
                             1});
      if (single) {
        auto parent = 读取存在身份来源历史见证(g, r.G0, r.绑定.绑定节点);
        if (!parent.成功(g, r.G0, r.绑定.绑定节点))
          throw 绑定映射(parent.状态);
        p.写集.关系.push_back({{3},
                               r.绑定.绑定节点,
                               L1所有者范围写集本地键{1},
                               子存在关系类型_,
                               1});
      }
      return p;
    });
  }
  绑定存在参与者结果<存在绑定出生见证>
  读取存在绑定出生(std::uint64_t g, std::uint64_t h, 稳定编码 e,
                   const 存在初始绑定 &b) const override {
    return 绑定保护<存在绑定出生见证>(g, h, [&] {
      auto id = 读取存在身份来源历史见证(g, h, e);
      if (!id.成功(g, h, e))
        throw 绑定映射(id.状态);
      存在绑定出生见证 out{g, h, e, *id.见证, {}};
      if (b.种类 == 存在初始绑定种类::父存在组成) {
        auto q = 第一层服务_.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标, e,
             子存在关系类型_, h});
        if (q.读取事实代次 != g)
          throw 绑定S::事实代次漂移;
        if (q.状态 != L1所有者范围读取状态::成功)
          throw 绑定S::内部不一致;
        for (const auto &x : q.关系组) {
          if (!x.创建事实代次 || x.创建事实代次 > h ||
              (x.退出事实代次 && *x.退出事实代次 <= h))
            continue;
          if (out.组成绑定关系 || x.源节点 != b.绑定节点 || x.目标节点 != e ||
              x.写入所有者 != 所有者_ || x.角色或顺序 != 1)
            throw 绑定S::内部不一致;
          out.组成绑定关系 =
              存在组成关系事实{g, h, x.编码, x.源节点, e, x.创建事实代次, {}};
        }
        if (!out.组成绑定关系)
          throw 绑定S::绑定未找到;
      }
      return out;
    });
  }
  const L1事实基座服务& 定位底座() const noexcept override { return 第一层服务_; }
  L1所有者范围写端口& 定位端口() noexcept override { return 写入端口_; }
  bool 定位幂等键可用(L1所有者范围写入幂等身份 key) const noexcept override {
    return key.值&&key.值!=1&&key!=存在族来源初始化幂等身份&&(key.值>>48)!=0x4E43;
  }
  bool 定位结构已就绪() const noexcept override { return 存在结构登记已就绪(); }
  bool 存在结构登记已就绪() const noexcept {
    try {
      std::lock_guard<std::mutex> lock(写入锁_);
      if(!写入端口_.有效()||!写入端口_.绑定于(第一层服务_)||!特征服务_.绑定于(第一层服务_))return false;
      const auto 首次=写入端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本,存在族来源初始化幂等身份});

    if (首次.状态 != L1所有者范围读取状态::成功 ||
        首次.合同版本 != L1所有者范围首次写入读取合同版本 ||
        首次.所有者 != 所有者_ ||
        首次.写入幂等身份 != 存在族来源初始化幂等身份 || !首次.首次规范化写集 ||
        !首次.首次写入结果)
      throw std::logic_error("invalid existence family first write");
    const auto 期望 = 形成存在族初始化写集(首次.首次规范化写集->期望事实代次);
    if (*首次.首次规范化写集 != 期望)
      throw std::logic_error("conflicting existence family layout");
    const auto &写入 = *首次.首次写入结果;
    if (写入.状态 != L1所有者范围写入状态::成功 ||
        写入.合同版本 != L1所有者范围CRUD合同版本 || 写入.所有者 != 所有者_ ||
        写入.写入幂等身份 != 存在族来源初始化幂等身份 ||
        !写入.是否形成内存权威发布 || 写入.事实代次 == 0 ||
        写入.事实代次 != 期望.期望事实代次 + 1 || 写入.新编码映射.size() != 2)
      throw std::logic_error("invalid existence family publication");
    const auto 锚点 =
        存在类数据内部::查找唯一编码(写入, 存在类数据内部::存在族锚点本地键);
    const auto 类型 = 存在类数据内部::查找唯一编码(
        写入, 存在类数据内部::存在族归属类型本地键);
    if (!锚点 || !类型 || *锚点 == *类型)
      throw std::logic_error("invalid existence family mapping");

    L1所有者范围一致当前读取请求 请求;
    请求.合同版本 = L1所有者范围一致当前读取合同版本;
    请求.期望事实代次 = 首次.读取事实代次;
    请求.所有者 = {所有者_};
    请求.节点 = {*锚点, *类型};
    const auto 投影 = 第一层服务_.尝试读取所有者范围一致当前投影(请求);
    if (投影.状态 == L1所有者范围一致当前读取状态::资源失败)
      throw std::runtime_error("existence family projection unavailable");
    if (投影.状态 != L1所有者范围一致当前读取状态::成功 ||
        投影.合同版本 != L1所有者范围一致当前读取合同版本 ||
        投影.期望事实代次 != 首次.读取事实代次 ||
        投影.读取事实代次 != 首次.读取事实代次 || 投影.所有者.size() != 1 ||
        投影.节点.size() != 2 || !投影.关系.empty() || !投影.值.empty() ||
        !投影.属性值.empty() || !投影.源关系组.empty() ||
        !投影.目标关系组.empty())
      throw std::logic_error("invalid existence family projection");
    const auto &owner = 投影.所有者.front();
    if (owner.查询所有者 != 所有者_ ||
        owner.状态 != L1所有者范围一致当前读取项目状态::成功 ||
        !owner.所有者事实 || owner.所有者事实->所有者 != 所有者_ ||
        owner.所有者事实->范围种类 != L1所有者范围种类::独占结构范围 ||
        owner.所有者事实->创建事实代次 == 0 ||
        owner.所有者事实->创建事实代次 > 首次.读取事实代次 ||
        owner.所有者事实->退出事实代次)
      throw std::logic_error("invalid existence family owner projection");
    const auto *锚点项 = 查找一致节点(投影, *锚点);
    const auto *类型项 = 查找一致节点(投影, *类型);
    const auto *锚点事实 = 锚点项 ? 当前节点事实(*锚点项) : nullptr;
    const auto *类型事实 = 类型项 ? 当前节点事实(*类型项) : nullptr;
    if (!锚点事实 || !类型事实 || !普通空节点当前有效(*锚点事实, *锚点) ||
        !普通空节点当前有效(*类型事实, *类型))
      throw std::logic_error("invalid existence family metadata");
    if(存在族锚点_!=*锚点||存在族归属关系类型_!=*类型)return false;
    if(!关系类型组有效()||!关系类型有效(子存在关系类型_)||!关系类型有效(特征关系类型_)||!关系类型有效(当前采用关系类型_))return false;
    const auto tail=读取当前事实代次();
    return tail.first==存在结构身份只读状态::已读取&&tail.second==首次.读取事实代次;

    }catch(...){return false;}
  }

  static 定位特征状态 定位映射(存在已知准确特征只读状态 state) noexcept {
    using P=定位特征状态; using X=存在已知准确特征只读状态;
    switch(state){
    case X::入口拒绝 : return P::入口拒绝;case X::未找到 : return P::未找到;
    case X::目标已退出 : return P::目标已退出;case X::事实代次漂移 : return P::事实代次漂移;
    case X::历史材料已清理 : return P::历史材料不可用;case X::数量预算不足 : return P::预算不足;
    case X::资源失败 : return P::资源失败;default : return P::内部不一致;}
  }
  template<class T,class F> 定位参与者结果<T> 定位保护(std::uint64_t g,std::uint64_t h,F&& fn) const {
    定位参与者结果<T> out;out.Gread=g;out.H=h;
    try {
      if(!g||!h||h>g)throw 定位特征状态::入口拒绝;
      auto current=读取当前事实代次();
      if(current.first!=存在结构身份只读状态::已读取)throw current.first==存在结构身份只读状态::资源失败 ? 定位特征状态::资源失败 : 定位特征状态::内部不一致;
      if(current.second!=g)throw 定位特征状态::事实代次漂移;
      out.数据=fn();current=读取当前事实代次();
      if(current.first!=存在结构身份只读状态::已读取)throw current.first==存在结构身份只读状态::资源失败 ? 定位特征状态::资源失败 : 定位特征状态::内部不一致;
      if(current.second!=g)throw 定位特征状态::事实代次漂移;
      out.状态=定位特征状态::已读取;
    }catch(定位特征状态 e){out.状态=e;out.数据.reset();}
    catch(const std::bad_alloc&){out.状态=定位特征状态::资源失败;out.数据.reset();}
    catch(const std::length_error&){out.状态=定位特征状态::资源失败;out.数据.reset();}
    catch(...){out.状态=定位特征状态::内部不一致;out.数据.reset();}
    return out;
  }
  定位参与者结果<定位关系见证> 读取定位已知(std::uint64_t g,std::uint64_t h,稳定编码 c,稳定编码 f) const override {
    return 定位保护<定位关系见证>(g,h,[&]{
      const 存在已知准确特征历史请求 request{1,g,h,c,f};
      auto known=读取已知准确特征历史(request);
      if(!known.历史成功(request))throw 定位映射(known.状态);
      auto raw=第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,known.见证->已知关系});
      if(raw.状态!=L1所有者范围读取状态::成功||!raw.事实||raw.读取事实代次!=g)
        throw raw.状态==L1所有者范围读取状态::资源失败 ? 定位特征状态::资源失败 : raw.读取事实代次!=g ? 定位特征状态::事实代次漂移 : 定位特征状态::内部不一致;
      const auto* e=std::get_if<L1所有者范围关系事实>(&*raw.事实);
      if(!e||e->写入所有者!=所有者_||e->编码!=known.见证->已知关系||e->源节点!=c||e->目标节点!=f||e->关系类型节点!=特征关系类型_||e->角色或顺序!=1||e->创建事实代次>h||(e->退出事实代次&&*e->退出事实代次<=h))throw 定位特征状态::内部不一致;
      return 定位关系见证{e->编码,c,f,e->关系类型节点,1,e->创建事实代次,e->退出事实代次};
    });
  }
  定位参与者结果<定位场景已知见证> 核验定位场景与父已知(std::uint64_t g,std::uint64_t h,稳定编码 c,std::optional<稳定编码> parent,const 定位特征预算& b) const override {
    return 定位保护<定位场景已知见证>(g,h,[&]{
      if(!有效(c)||!b.最大候选数||!b.最大路径长度||b.最大路径长度>4096)throw 定位特征状态::入口拒绝;
      const auto identity=读取存在身份来源历史见证(g,h,c);
      if(!identity.成功(g,h,c)){
        using X=存在结构身份只读状态;
        switch(identity.状态){case X::未找到 : throw 定位特征状态::未找到;case X::目标已退出 : throw 定位特征状态::目标已退出;case X::事实代次漂移 : throw 定位特征状态::事实代次漂移;case X::历史材料已清理 : throw 定位特征状态::历史材料不可用;case X::资源失败 : throw 定位特征状态::资源失败;default : throw 定位特征状态::内部不一致;}
      }
      定位场景已知见证 out{c,parent,{}};
      if(parent){auto x=读取定位已知(g,h,c,*parent);if(x.状态!=定位特征状态::已读取||!x.数据)throw x.状态;out.父已知关系=std::move(x.数据);}
      return out;
    });
  }
  定位参与者结果<L1三分区原子参与者写集_v2> 准备定位已知(const 定位特征创建请求& r,std::uint64_t g) const override {
    return 定位保护<L1三分区原子参与者写集_v2>(g,r.G0,[&]{
      for(auto key : {r.组合键,r.内容键,r.已知键,r.组织键})if(!定位幂等键可用(key))throw 定位特征状态::入口拒绝;
      auto c=核验定位场景与父已知(g,r.G0,r.位置.场景,{},r.预算);if(c.状态!=定位特征状态::已读取||!c.数据)throw c.状态;
      L1三分区原子参与者写集_v2 out;out.参与者={2};out.所有者=所有者_;
      out.写集.期望事实代次=r.G0;out.写集.写入幂等身份=r.已知键;
      out.写集.关系.push_back({L1所有者范围写集本地键{1},r.位置.场景,L1三分区原子事实引用_v2{{1},{1}},特征关系类型_,1});return out;
    });
  }
  稳定编码 当前采用关系类型_;
  mutable std::mutex 写入锁_;
  std::optional<存在当前采用写请求> 采用待确认_;
  const L1事实基座服务 &第一层服务_;
  const 特征类数据服务 &特征服务_;
  L1所有者范围写端口 写入端口_;
  L1结构所有者身份 所有者_{};
  稳定编码 子存在关系类型_{};
  稳定编码 特征关系类型_{};
  稳定编码 存在族锚点_{};
  稳定编码 存在族归属关系类型_{};
};

inline 存在当前采用结果
存在类数据服务::读取当前采用(const 存在当前采用读取请求 &r) const {
  存在当前采用结果 out;
  out.Gread = r.Gread;
  out.H = r.H;
  try {
    采用要求(r.版本 == 1 && 有效(r.FT), 采用S::入口拒绝);
    const auto all = 读取采用组(r.E, r.Gread, r.H, r.关系预算);
    for (const auto &a : all.采用)
      if (a.FT == r.FT)
        out.采用 = a;
    out.状态 = 采用S::已读取;
    采用要求(out.成功());
    return out;
  } catch (const 采用失败 &e) {
    out.状态 = e.状态;
  } catch (const std::bad_alloc &) {
    out.状态 = 采用S::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 采用S::资源失败;
  } catch (...) {
    out.状态 = 采用S::内部不一致;
  }
  out.采用.reset();
  return out;
}
inline 存在当前采用结果
存在类数据服务::变更当前采用(const 存在当前采用写请求 &r) {
  存在当前采用结果 out;
  bool uncertain = false, dispatched = false, zero = false;
  std::unique_lock<std::mutex> lock(写入锁_);
  try {
    out.原请求 = r;
    采用要求(r.版本 == 1 && r.G && 有效(r.幂等身份) && 有效(r.E) &&
                 有效(r.FT) && r.关系预算 && !r.操作.valueless_by_exception(),
             采用S::入口拒绝);
    采用要求(!采用待确认_ || *采用待确认_ == r, 采用S::已可能发布);
    uncertain = true;
    const auto first = 写入端口_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, r.幂等身份});
    采用要求(first.合同版本 == L1所有者范围首次写入读取合同版本 &&
             first.所有者 == 所有者_ && first.写入幂等身份 == r.幂等身份 &&
             first.读取事实代次);
    out.Gread = first.读取事实代次;
    采用守卫(out.Gread);
    采用要求(first.状态 == L1所有者范围读取状态::成功 ||
                 first.状态 == L1所有者范围读取状态::未找到,
             映射读取状态(first.状态));
    L1所有者范围写入结果 saved;
    L1所有者范围写集请求 ws;
    const bool replay = first.状态 == L1所有者范围读取状态::成功;
    if (replay) {
      采用要求(first.首次规范化写集 && first.首次写入结果);
      if (first.首次写入结果->事实代次 &&
          first.首次写入结果->事实代次 <= out.Gread)
        out.首次H = first.首次写入结果->事实代次;
      if (first.首次规范化写集->期望事实代次 != r.G) {
        uncertain = false;
        throw 采用失败{采用S::幂等冲突};
      }
      ws = 准备采用(r, out.Gread).写集;
      if (ws != *first.首次规范化写集) {
        uncertain = false;
        throw 采用失败{采用S::幂等冲突};
      }
      saved = *first.首次写入结果;
    } else {
      采用要求(!first.首次规范化写集 && !first.首次写入结果);
      uncertain = false;
      if (out.Gread != r.G) {
        采用待确认_.reset();
        throw 采用失败{采用S::事实代次漂移};
      }
      auto prepared = 准备采用(r, out.Gread);
      ws = std::move(prepared.写集);
      if (ws.关系.empty() && ws.退出事实.empty()) {
        out.状态 = 采用S::精确重复;
        out.H = r.G;
        out.采用 = std::move(prepared.当前);
        采用守卫(out.Gread);
        采用要求(out.成功());
        采用待确认_.reset();
        return out;
      }
      采用待确认_ = r;
      采用守卫(out.Gread);
      dispatched = true;
      saved = 写入端口_.提交所有者范围中性写集(ws);
      const bool header = saved.合同版本 == L1所有者范围CRUD合同版本 &&
                          saved.所有者 == 所有者_ &&
                          saved.写入幂等身份 == r.幂等身份;
      if (header && !saved.是否形成内存权威发布 && saved.新编码映射.empty() &&
          saved.状态 != L1所有者范围写入状态::成功 &&
          saved.状态 != L1所有者范围写入状态::精确重复 &&
          saved.重试边界 != L1所有者范围重试边界::原幂等身份读回收敛) {
        zero = true;
        采用待确认_.reset();
        throw 采用失败{映射写入状态(saved.状态, 采用S::内部不一致)};
      }
      采用要求(header);
      if (saved.事实代次 > r.G &&
          (saved.是否形成内存权威发布 ||
           saved.状态 == L1所有者范围写入状态::精确重复))
        out.首次H = saved.事实代次;
      out.Gread = 采用当前G();
    }
    采用互证(ws, saved, out.Gread);
    out.H = saved.事实代次;
    out.首次H = saved.事实代次;
    const auto all = 读取采用组(r.E, out.Gread, out.H, r.关系预算);
    for (const auto &a : all.采用)
      if (a.FT == r.FT)
        out.采用 = a;
    out.状态 = std::visit(
        [](const auto &op) {
          using T = std::decay_t<decltype(op)>;
          if constexpr (std::is_same_v<T, 存在当前采用解除>)
            return 采用S::已解除当前采用;
          else if constexpr (std::is_same_v<T, 存在当前采用替换>)
            return 采用S::已替换当前采用;
          else if constexpr (std::is_same_v<T, 存在关联已知并采用>)
            return op.预期 ? 采用S::已替换当前采用 : 采用S::已建立当前采用;
          else
            return 采用S::已建立当前采用;
        },
        r.操作);
    if (replay || saved.状态 == L1所有者范围写入状态::精确重复)
      out.状态 = 采用S::精确重复;
    采用守卫(out.Gread);
    采用要求(out.成功());
    采用待确认_.reset();
    return out;
  } catch (const 采用失败 &e) {
    out.状态 = e.状态;
  } catch (const std::bad_alloc &) {
    out.状态 = 采用S::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 采用S::资源失败;
  } catch (...) {
    out.状态 = 采用S::内部不一致;
  }
  out.采用.reset();
  if ((uncertain || dispatched) && !zero) {
    out.状态 = 采用S::已可能发布;
    if (!采用待确认_)
      try {
        采用待确认_ = r;
      } catch (...) {
      }
  } else if (采用待确认_ && *采用待确认_ == r)
    采用待确认_.reset();
  return out;
}
inline bool 存在当前采用结果::成功() const noexcept {
  using S = 存在类数据状态;
  if (版本 != 1 || !H || H > Gread)
    return false;
  if (采用 && (!有效(采用->关系) || !有效(采用->E) || !有效(采用->FT) ||
               !有效(采用->F)))
    return false;
  if (状态 == S::已读取)
    return !首次H && !原请求;
  if (!原请求 || 原请求->版本 != 1 || !原请求->G || 原请求->G > Gread ||
      !有效(原请求->幂等身份) || !有效(原请求->E) || !有效(原请求->FT) ||
      !原请求->关系预算 || 原请求->操作.valueless_by_exception())
    return false;
  if (状态 == S::精确重复) {
    if (首次H && (*首次H != H || !*首次H))
      return false;
  } else if (!首次H || !*首次H || *首次H != H)
    return false;
  if (首次H ? *首次H <= 原请求->G : 状态 != S::精确重复 || H != 原请求->G)
    return false;
  if (采用 && (采用->E != 原请求->E || 采用->FT != 原请求->FT))
    return false;
  return std::visit(
      [&](const auto &op) noexcept {
        using T = std::decay_t<decltype(op)>;
        auto expectedValid = [&](const 存在当前采用事实 &old) {
          return 有效(old.关系) && old.E == 原请求->E && old.FT == 原请求->FT &&
                 有效(old.F);
        };
        if constexpr (std::is_same_v<T, 存在当前采用替换> ||
                      std::is_same_v<T, 存在当前采用解除>) {
          if (!expectedValid(op.预期))
            return false;
        } else if constexpr (std::is_same_v<T, 存在关联已知并采用>) {
          if (op.预期 && !expectedValid(*op.预期))
            return false;
        }
        if constexpr (std::is_same_v<T, 存在当前采用解除>)
          return (状态 == S::精确重复 || 状态 == S::已解除当前采用) && !采用 &&
                 首次H.has_value();
        else {
          const auto expected = [&] {
            if constexpr (std::is_same_v<T, 存在当前采用替换>)
              return op.新F;
            else
              return op.F;
          }();
          if (!采用 || 采用->F != expected)
            return false;
          if constexpr (std::is_same_v<T, 存在当前采用替换>)
            return 状态 == S::精确重复 || 状态 == S::已替换当前采用;
          else if constexpr (std::is_same_v<T, 存在关联已知并采用>)
            return 状态 == S::精确重复 ||
                   状态 == (op.预期 ? S::已替换当前采用 : S::已建立当前采用);
          else
            return 状态 == S::精确重复 || 状态 == S::已建立当前采用;
        }
      },
      原请求->操作);
}
} // namespace 海中鱼巣
