#pragma once

#include "数据服务.绑定存在.h"
#include "数据服务.定位特征.h"

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
#include <system_error>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "数据服务.特征类.h"
#include "合同.存在结构身份只读.h"
#include "合同.世界树根.h"

namespace 海中鱼巣::存在类数据内部 {

inline L1所有者范围当前读取结果 读取存在当前事实(
    const L1事实基座服务& l1, 稳定编码 id) {
  const L1所有者范围事实读取请求 request{L1所有者范围CRUD合同版本,id};
  const std::array reads{l1.读取所有者范围当前节点(request),
                         l1.读取所有者范围当前关系(request),
                         l1.读取所有者范围当前值(request)};
  std::optional<L1所有者范围当前读取结果> hit;
  for(const auto& read:reads){
    if(read.状态==L1所有者范围读取状态::成功){if(hit)return {};hit=read;}
    else if(read.状态!=L1所有者范围读取状态::未找到)return read;
  }
  return hit?*hit:reads.front();
}

inline constexpr L1所有者范围写集本地键 存在结点本地键{1};
inline constexpr L1所有者范围写集本地键 成员关系本地键{2};
inline constexpr L1所有者范围写集本地键 存在族归属关系本地键{2};
inline constexpr L1所有者范围写集本地键 存在族锚点本地键{1};
inline constexpr L1所有者范围写集本地键 存在族归属类型本地键{2};
inline constexpr L1所有者范围写集本地键 方法虚拟存在结点本地键{1};
inline constexpr L1所有者范围写集本地键 方法虚拟存在关系本地键{2};

inline std::optional<稳定编码> 查找唯一编码(const L1所有者范围写入结果 &结果,
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


namespace 海中鱼巣 {

inline constexpr std::uint32_t 存在类数据合同版本 = 1;

struct 存在信息身份 final {
  稳定编码 编码{};
  friend bool operator==(const 存在信息身份 &, const 存在信息身份 &) = default;
};
inline bool 有效(存在信息身份 v) noexcept { return 有效(v.编码); }

enum class 存在类成员种类 : std::uint8_t { 子存在 = 1, 特征 = 2 };

enum class 存在类数据状态 : std::uint8_t {
  已创建 = 1,
  精确重复 = 2,
  已读取 = 3,
  已添加成员 = 4,
  已移除成员 = 5,
    已删除 = 6,
  入口拒绝 = 7,
  未找到 = 8,
  成员已存在 = 10,
  成员未找到 = 11,
  子存在读取失败 = 12,
  特征读取失败 = 13,
  结构类型未找到 = 16,
  事实代次漂移 = 18,
  幂等冲突 = 19,
  引用冲突 = 20,
  资源失败 = 22,
  内部不一致 = 23,
  已可能发布 = 24,
  已建立当前采用 = 25,
  已替换当前采用 = 26,
  已解除当前采用 = 27,
  格式不支持 = 28,
  数量预算不足 = 29
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
  std::uint64_t Gread = 0;
  稳定编码 E;
  特征类型身份 FT;
  std::uint64_t 关系预算 = 0;
};
struct 存在当前采用完整读取请求_v2 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0;
  稳定编码 E;
  特征类型身份 FT;
  friend bool operator==(const 存在当前采用完整读取请求_v2 &,
                         const 存在当前采用完整读取请求_v2 &) = default;
};
enum class 存在当前采用完整读取状态_v2 : std::uint8_t {
  已读取 = 1,
  入口拒绝 = 2,
  未找到 = 3,
  事实代次漂移 = 5,
  资源失败 = 7,
  内部不一致 = 8
};
struct 存在当前采用完整读取结果_v2 final {
  std::uint32_t 版本 = 2;
  存在当前采用完整读取状态_v2 状态 =
      存在当前采用完整读取状态_v2::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<存在当前采用事实> 采用;
  bool 成功(const 存在当前采用完整读取请求_v2 &) const noexcept;
  friend bool operator==(const 存在当前采用完整读取结果_v2 &,
                         const 存在当前采用完整读取结果_v2 &) = default;
};

inline constexpr std::uint32_t 存在单例角色当前完整读取合同版本_v2 = 2;
enum class 存在单例角色当前完整读取状态_v2 : std::uint8_t {
  已读取=1, 入口拒绝=2, 未绑定=3, 角色冲突=4,
  事实代次漂移=5, 资源失败=7, 内部不一致=8
};
struct 存在单例角色当前完整读取请求_v2 final {
  std::uint32_t 版本=存在单例角色当前完整读取合同版本_v2;
  std::uint64_t Gread=0;
  存在单例角色身份 角色;
};
struct 存在单例角色当前完整读取结果_v2 final {
  存在单例角色当前完整读取状态_v2 状态=
      存在单例角色当前完整读取状态_v2::入口拒绝;
  std::uint32_t 版本=存在单例角色当前完整读取合同版本_v2;
  std::uint64_t Gread=0;
  std::optional<存在单例角色事实> 事实;
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
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布代次;
  std::optional<存在当前采用事实> 采用;
  std::optional<存在当前采用写请求> 原请求;
  bool 成功() const noexcept;
};

// 4115：这些身份只表达存在 owner 的结构节点；它们不改变准确 F 或当前采用。
struct 实例特征容器身份 final {
  稳定编码 编码{};
  friend bool operator==(const 实例特征容器身份 &, const 实例特征容器身份 &) = default;
};
struct 实例特征R集合身份 final {
  稳定编码 编码{};
  friend bool operator==(const 实例特征R集合身份 &, const 实例特征R集合身份 &) = default;
};
struct 实例特征R集合版本 final {
  稳定编码 编码{};
  friend bool operator==(const 实例特征R集合版本 &, const 实例特征R集合版本 &) = default;
};
struct 实例特征R项身份 final {
  稳定编码 编码{};
  friend bool operator==(const 实例特征R项身份 &, const 实例特征R项身份 &) = default;
};
struct 实例特征结构交付 final {
  std::uint32_t 版本 = 2;
  稳定编码 E到IF{}, IF到F{}, IF到R集合{}, R集合到版本{}, 版本到R项{}, R项到F{};
  稳定编码 R项材料属性类型{};
  friend bool operator==(const 实例特征结构交付 &, const 实例特征结构交付 &) = default;
};
enum class 实例特征结构状态 : std::uint8_t {
  已登记 = 1, 精确重复 = 2, 入口拒绝 = 3, 事实代次漂移 = 4,
  幂等冲突 = 5, 结构冲突 = 6, 已可能发布 = 7, 资源失败 = 8, 内部不一致 = 9
};
struct 实例特征结构登记请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  friend bool operator==(const 实例特征结构登记请求 &, const 实例特征结构登记请求 &) = default;
};
struct 实例特征结构登记结果 final {
  实例特征结构状态 状态 = 实例特征结构状态::入口拒绝;
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布代次;
  std::optional<实例特征结构交付> 交付;
  bool 成功(const 实例特征结构登记请求 &r) const noexcept {
    return r.版本 == 2 && r.G0 && 版本 == 2 && Gread >= r.G0 && 交付 &&
      (状态 == 实例特征结构状态::已登记 || 状态 == 实例特征结构状态::精确重复);
  }
};
struct 实例特征IFR读取预算 final {
  std::uint64_t 最大F成员数 = 0;
  std::uint64_t 最大R项数 = 0;
  std::uint64_t 最大关系数 = 0;
  std::uint64_t 最大材料值数 = 0;
  std::uint64_t 最大单材料U64元素数 = 0;
  friend bool operator==(const 实例特征IFR读取预算 &,
                         const 实例特征IFR读取预算 &) = default;
};
struct 实例特征R项投影 final {
  实例特征R项身份 R项;
  特征R区间材料 材料;
  std::vector<特征信息身份> 形成成员;
  L1所有者范围节点事实 节点事实;
  std::vector<L1所有者范围关系事实> 成员关系;
  L1所有者范围值事实 材料值事实;
  friend bool operator==(const 实例特征R项投影 &,
                         const 实例特征R项投影 &) = default;
};
struct 实例特征IFR完整投影 final {
  存在信息身份 E;
  特征类型身份 FT;
  实例特征容器身份 IF;
  实例特征R集合身份 R集合;
  实例特征R集合版本 版本;
  std::vector<特征信息身份> F成员;
  std::vector<实例特征R项投影> R项;
  实例特征结构交付 结构;
  std::vector<L1所有者范围节点事实> 节点事实;
  std::vector<L1所有者范围关系事实> 六关系事实;
  friend bool operator==(const 实例特征IFR完整投影 &,
                         const 实例特征IFR完整投影 &) = default;
};
struct 读取实例特征IFR请求 final {
  std::uint32_t 版本 = 1;
  存在信息身份 E;
  特征类型身份 FT;
  std::uint64_t Gread = 0;
  实例特征IFR读取预算 预算;
  friend bool operator==(const 读取实例特征IFR请求 &,
                         const 读取实例特征IFR请求 &) = default;
};
struct 实例特征IFR目标项 final {
  特征R区间材料 材料;
  std::vector<特征信息身份> 形成成员;
  friend bool operator==(const 实例特征IFR目标项 &,
                         const 实例特征IFR目标项 &) = default;
};
struct 提交实例特征IFR目标请求 final {
  std::uint32_t 版本 = 1;
  存在信息身份 E;
  特征类型身份 FT;
  std::uint64_t G0 = 0;
  std::optional<实例特征IFR完整投影> 预期快照;
  std::vector<实例特征IFR目标项> 目标R项;
  L1所有者范围写入幂等身份 幂等身份;
  实例特征IFR读取预算 读回预算;
  friend bool operator==(const 提交实例特征IFR目标请求 &,
                         const 提交实例特征IFR目标请求 &) = default;
};
enum class 实例特征IFR状态 : std::uint8_t {
  已读取 = 1, 已发布 = 2, 无须变更 = 3, 精确重复 = 4, 入口拒绝 = 5,
  未找到 = 6, 数量预算不足 = 8, 事实代次漂移 = 9,
  引用冲突 = 10, 幂等冲突 = 11, 资源失败 = 12, 已可能发布 = 13,
  内部不一致 = 14
};
struct 实例特征IFR结果 final {
  std::uint32_t 版本 = 1;
  实例特征IFR状态 状态 = 实例特征IFR状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布代次;
  std::optional<实例特征IFR完整投影> 投影;
  std::optional<提交实例特征IFR目标请求> 原请求;
  friend bool operator==(const 实例特征IFR结果 &,
                         const 实例特征IFR结果 &) = default;
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
    const bool 删除成功 = 状态 == 存在类数据状态::已删除;
    return (状态 == 存在类数据状态::已创建 ||
            状态 == 存在类数据状态::精确重复 ||
            状态 == 存在类数据状态::已读取 ||
            状态 == 存在类数据状态::已添加成员 ||
            状态 == 存在类数据状态::已移除成员 ||
            删除成功) &&
           合同版本 == 存在类数据合同版本 && 事实代次 != 0 &&
           (删除成功 ? !存在结点 : 存在结点.has_value());
  }
};

inline constexpr std::uint32_t 方法虚拟存在专用合同版本 = 1;

enum class 方法虚拟存在专用状态 : std::uint8_t {
  已形成写集 = 1,
  已读取 = 2,
  未初始化 = 3,
  已初始化 = 4,
  入口拒绝 = 5,
  方法所有者未找到 = 6,
  方法节点未找到 = 8,
  关系类型未找到 = 10,
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

class 存在类数据服务 final : public 存在结构身份只读提供者,
                             public 存在组成结构只读提供者, public 原子I64特征holder参与者,
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
  读取世界树根存在来源(std::uint64_t g,
                         稳定编码 e) const override {
    const auto r = 读取当前存在身份来源见证(g, e);
    if (!r.成功(g, e) || !r.见证)
      return std::nullopt;
    return 世界树根存在来源{g, e, *r.见证};
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
      auto q = 第一层服务_.读取所有者范围当前目标关系组(
          {L1所有者范围CRUD合同版本,r.子存在,子存在关系类型_});
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
          q.目标节点 != r.子存在 || q.关系类型节点 != 子存在关系类型_) {
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
            e.角色或顺序 != 1 || !e.创建事实代次) {
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
        o.父 = 存在组成关系事实{r.G0, e.编码, e.源节点,
                                e.目标节点, e.创建事实代次};
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
      auto q = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本,r.父存在,子存在关系类型_});
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
          q.源节点 != r.父存在 || q.关系类型节点 != 子存在关系类型_ ||
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
            e.角色或顺序 != 1 || !e.创建事实代次) {
          o.状态 = 存在结构身份只读状态::内部不一致;
          o.子组.clear();
          return o;
        }
        o.子组.push_back(
            {r.G0, e.编码, e.源节点, e.目标节点, e.创建事实代次});
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

  存在组成当前完整读取结果_v2
  读取当前组成父_v2(const 存在组成父当前完整读取请求_v2 &r) const override {
    if (r.版本 != 存在组成当前完整读取合同版本_v2)
      return {};
    return 读取当前组成_v2(r.G0, r.子存在, true);
  }

  存在组成当前完整读取结果_v2
  读取当前组成子组_v2(const 存在组成子组当前完整读取请求_v2 &r) const override {
    if (r.版本 != 存在组成当前完整读取合同版本_v2)
      return {};
    return 读取当前组成_v2(r.G0, r.父存在, false);
  }
private:
  存在组成当前完整读取结果_v2 读取当前组成_v2(
      std::uint64_t G0, 稳定编码 端点, bool 读取父) const {
    存在组成当前完整读取结果_v2 out;
    out.Gread = G0;
    if (!G0 || !有效(端点))
      return out;
    try {
      const auto 身份 = 确认当前存在结构身份(G0, 端点);
      out.Gread = 身份.Gread;
      if (!身份.成功(G0)) {
        switch (身份.状态) {
        case 存在结构身份只读状态::入口拒绝:
          out.状态 = 存在组成当前完整读取状态_v2::入口拒绝;
          break;
        case 存在结构身份只读状态::未找到:
          out.状态 = 存在组成当前完整读取状态_v2::未找到;
          break;
        case 存在结构身份只读状态::事实代次漂移:
          out.状态 = 存在组成当前完整读取状态_v2::事实代次漂移;
          break;
        case 存在结构身份只读状态::资源失败:
          out.状态 = 存在组成当前完整读取状态_v2::资源失败;
          break;
        default:
          out.状态 = 存在组成当前完整读取状态_v2::内部不一致;
          break;
        }
        return out;
      }

      const L1节点当前完整引用读取请求_v2 请求{
          L1节点当前完整引用读取合同版本, 端点, G0};
      const auto 读取 = 第一层服务_.读取节点全部当前引用_v2(请求);
      out.Gread = 读取.读取事实代次;
      const bool 读取成功 =
          读取.状态 == L1节点当前完整引用读取状态_v2::成功 &&
          读取.合同版本 == L1节点当前完整引用读取合同版本 &&
          读取.节点 == 请求.节点 &&
          读取.期望事实代次 == 请求.期望事实代次 &&
          读取.读取事实代次 == 请求.期望事实代次;
      if (!读取成功) {
        switch (读取.状态) {
        case L1节点当前完整引用读取状态_v2::未找到:
          out.状态 = 存在组成当前完整读取状态_v2::未找到;
          break;
        case L1节点当前完整引用读取状态_v2::事实代次漂移:
          out.状态 = 存在组成当前完整读取状态_v2::事实代次漂移;
          break;
        case L1节点当前完整引用读取状态_v2::资源失败:
          out.状态 = 存在组成当前完整读取状态_v2::资源失败;
          break;
        default:
          out.状态 = 存在组成当前完整读取状态_v2::内部不一致;
          break;
        }
        return out;
      }

      for (const auto &引用 : 读取.引用) {
        const auto *关系 = std::get_if<L1所有者范围关系事实>(&引用);
        if (!关系 || 关系->关系类型节点 != 子存在关系类型_)
          continue;
        const bool 方向正确 = 读取父 ? 关系->目标节点 == 端点
                                   : 关系->源节点 == 端点;
        if (!方向正确)
          continue;
        const auto 另一端 = 读取父 ? 关系->源节点 : 关系->目标节点;
        if (关系->写入所有者 != 所有者_ || 关系->角色或顺序 != 1 ||
            !有效(关系->编码) || !有效(关系->源节点) ||
            !有效(关系->目标节点) || 另一端 == 端点 || !关系->创建事实代次 ||
            关系->创建事实代次 > out.Gread) {
          out.状态 = 存在组成当前完整读取状态_v2::内部不一致;
          out.父.reset();
          out.子组.clear();
          return out;
        }
        存在组成关系事实 事实{out.Gread, 关系->编码, 关系->源节点,
                              关系->目标节点, 关系->创建事实代次};
        if (读取父) {
          if (out.父) {
            out.状态 = 存在组成当前完整读取状态_v2::内部不一致;
            out.父.reset();
            return out;
          }
          out.父 = std::move(事实);
        } else {
          out.子组.push_back(std::move(事实));
        }
      }
      std::sort(out.子组.begin(), out.子组.end(), [](const auto &a, const auto &b) {
        return a.关系.值 < b.关系.值;
      });
      out.状态 = 存在组成当前完整读取状态_v2::已读取;
      return out;
    } catch (const std::bad_alloc &) {
      out.状态 = 存在组成当前完整读取状态_v2::资源失败;
    } catch (const std::length_error &) {
      out.状态 = 存在组成当前完整读取状态_v2::资源失败;
    } catch (const std::system_error &) {
      out.状态 = 存在组成当前完整读取状态_v2::资源失败;
    } catch (...) {
      out.状态 = 存在组成当前完整读取状态_v2::内部不一致;
    }
    out.父.reset();
    out.子组.clear();
    return out;
  }

  friend class ::海中鱼巣::世界树根数据服务;
  L1所有者范围写端口 &世界树根协调端口() noexcept { return 写入端口_; }
public:
  存在类数据服务() = delete;
  存在类数据服务(const 存在类数据服务 &) = delete;
  存在类数据服务 &operator=(const 存在类数据服务 &) = delete;
  存在类数据服务(存在类数据服务 &&) = delete;
  存在类数据服务 &operator=(存在类数据服务 &&) = delete;

  存在类数据服务(const L1事实基座服务 &第一层服务,
                 const 特征类数据服务 &特征服务, L1所有者范围写端口 &&写入端口,
                 稳定编码 子存在关系类型, 稳定编码 特征关系类型,
                 const 存在当前采用结构交付 &当前采用结构,
                 const 实例特征结构交付 &实例特征结构)
      : 第一层服务_(第一层服务), 特征服务_(特征服务),
        写入端口_(std::move(写入端口)), 所有者_(写入端口_.所有者身份()),
        子存在关系类型_(子存在关系类型), 特征关系类型_(特征关系类型),
        当前采用关系类型_(当前采用结构.当前采用关系类型), 实例特征结构_(实例特征结构) {
    if (!写入端口_.有效() || !写入端口_.绑定于(第一层服务_) ||
        !特征服务_.绑定于(第一层服务_) || !有效(所有者_) || !关系类型组有效())
      throw std::invalid_argument("invalid existence data configuration");
    const auto 所有者读取 =
        第一层服务_.读取当前结构所有者({L1所有者范围CRUD合同版本, 所有者_});
    if (所有者读取.状态 != L1所有者范围读取状态::成功 ||
        所有者读取.合同版本 != L1所有者范围CRUD合同版本 ||
        !所有者读取.所有者事实 || 所有者读取.所有者事实->所有者 != 所有者_ ||
        所有者读取.所有者事实->范围种类 != L1所有者范围种类::独占结构范围)
      throw std::invalid_argument("invalid existence data owner");
    if (!关系类型有效(子存在关系类型_) || !关系类型有效(特征关系类型_) ||
        !关系类型有效(当前采用关系类型_))
      throw std::invalid_argument("invalid existence relation types");
    if (!实例特征结构有效())
      throw std::invalid_argument("invalid instance feature layout");
    初始化存在族来源();
    if (存在族锚点_ == 子存在关系类型_ || 存在族锚点_ == 特征关系类型_ ||
        存在族锚点_ == 当前采用关系类型_ ||
        存在族归属关系类型_ == 子存在关系类型_ ||
        存在族归属关系类型_ == 特征关系类型_ ||
        存在族归属关系类型_ == 当前采用关系类型_)
      throw std::invalid_argument("invalid existence family metadata");
  }

  static 存在单例角色结构登记结果 登记单例角色结构(
      const L1事实基座服务 &, L1所有者范围写端口 &,
      const 存在单例角色结构登记请求 &) noexcept;
  static 实例特征结构登记结果 登记实例特征结构(
      const L1事实基座服务 &, L1所有者范围写端口 &,
      const 实例特征结构登记请求 &) noexcept;
  实例特征IFR结果 读取实例特征IFR(
      const 读取实例特征IFR请求 &) const noexcept;
  实例特征IFR结果 提交实例特征IFR目标结构(
      const 提交实例特征IFR目标请求 &) noexcept;

  存在类数据服务(const L1事实基座服务 &l1, const 特征类数据服务 &features,
                 L1所有者范围写端口 &&port, 稳定编码 childType,
                  稳定编码 featureType, const 存在当前采用结构交付 &current,
                  const 实例特征结构交付 &instanceFeature,
                  const 存在单例角色结构交付 &role)
      : 存在类数据服务(l1, features, std::move(port), childType, featureType, current, instanceFeature) {
    if(role.版本!=1||!有效(role.角色登记类型)||!有效(role.角色目标类型)||
       !有效(role.项目角色.值)||role.角色登记类型==role.角色目标类型||
       role.角色登记类型==role.项目角色.值||role.角色目标类型==role.项目角色.值)
      throw std::invalid_argument("invalid singleton role layout");
    角色结构_=role;
    const auto currentCut=读取当前事实代次();
    try {
      if(currentCut.first!=存在结构身份只读状态::已读取||!currentCut.second)
        throw 存在单例角色状态::内部不一致;
      核验单例角色结构首次材料(currentCut.second);
    } catch(...) {
      角色结构_.reset();
      throw std::invalid_argument("invalid singleton role registration");
    }
  }

  存在单例角色读取结果 读取单例角色(
      const 存在单例角色读取请求 &) const noexcept;
  存在单例角色当前完整读取结果_v2 读取单例角色当前完整_v2(
      const 存在单例角色当前完整读取请求_v2 &) const noexcept;
  已发布概念绑定创建结果 添加存在节点(
      const 已发布概念绑定创建请求 &r, 绑定存在数据服务 &binding,
      已发布概念引用参与者 &concepts) noexcept {
    if(r.绑定.种类==存在初始绑定种类::直接子场景||
       !binding.使用存在提供者(*this)) return {};
    return binding.创建绑定存在并引用概念(r,concepts);
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
      if (!族归属关系有效(关系, 身份) ||
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
  存在身份来源当前见证读取结果
  读取当前存在身份来源见证(std::uint64_t Gread,
                           稳定编码 身份) const override {
    if (Gread == 0 || !有效(身份))
      return 当前见证失败(存在结构身份只读状态::入口拒绝);
    bool 读取守卫已成立 = false;
    try {
      const auto 前 = 读取当前事实代次();
      if (前.first != 存在结构身份只读状态::已读取)
        return 当前见证失败(前.first, 前.second);
      if (前.second != Gread)
        return 当前见证失败(存在结构身份只读状态::事实代次漂移, 前.second);
      读取守卫已成立 = true;

      const auto owner =
          第一层服务_.读取当前结构所有者({L1所有者范围CRUD合同版本, 所有者_});
      const auto e = 存在类数据内部::读取存在当前事实(第一层服务_,身份);
      const auto anchor = 存在类数据内部::读取存在当前事实(第一层服务_,存在族锚点_);
      const auto type = 存在类数据内部::读取存在当前事实(第一层服务_,存在族归属关系类型_);
      const auto links = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本,身份,存在族归属关系类型_});

      const auto 后 = 读取当前事实代次();
      if (后.first != 存在结构身份只读状态::已读取)
        return 当前见证失败(后.first, 后.second);
      if (后.second != Gread)
        return 当前见证失败(存在结构身份只读状态::事实代次漂移, 后.second);

      const auto 读取错误 =
          当前来源读取错误(owner, e, anchor, type, links, Gread, 身份);
      if (读取错误)
        return 当前见证失败(*读取错误, Gread);

      const auto &ownerFact = *owner.所有者事实;
      const auto &eFact = std::get<L1所有者范围节点事实>(*e.事实);
      const auto &anchorFact = std::get<L1所有者范围节点事实>(*anchor.事实);
      const auto &typeFact = std::get<L1所有者范围节点事实>(*type.事实);
      const auto &relation = links.关系组.front();
      存在身份来源当前见证 见证;
      见证.身份 = 身份;
      见证.族锚点 = 存在族锚点_;
      见证.族归属关系类型 = 存在族归属关系类型_;
      见证.族归属关系 = relation.编码;
      见证.角色 = 1;
      见证.节点创建事实代次 = eFact.创建事实代次;
      见证.族锚点创建事实代次 = anchorFact.创建事实代次;
      见证.关系类型创建事实代次 = typeFact.创建事实代次;
      见证.归属关系创建事实代次 = relation.创建事实代次;
      (void)ownerFact;
      存在身份来源当前见证读取结果 结果{存在结构身份只读状态::已读取,
                                        存在结构身份只读合同版本, Gread,
                                        std::move(见证)};
      return 结果.成功(Gread, 身份)
                 ? 结果
                 : 当前见证失败(存在结构身份只读状态::内部不一致, Gread);
    } catch (const std::bad_alloc &) {
      return 当前见证失败(存在结构身份只读状态::资源失败,
                          读取守卫已成立 ? Gread : 0);
    } catch (...) {
      return 当前见证失败(存在结构身份只读状态::内部不一致,
                          读取守卫已成立 ? Gread : 0);
    }
  }

  存在已知准确特征读取结果
  确认当前已知准确特征(const 存在已知准确特征当前请求 &r) const override {
    using S = 存在已知准确特征只读状态;
    auto fail = [&](S state, std::uint64_t g = 0) {
      return 存在已知准确特征读取结果{
          state, 存在已知准确特征只读合同版本, g, r.存在, r.特征, std::nullopt};
    };
    if (r.版本 != 存在已知准确特征只读合同版本 || r.G0 == 0 ||
        !有效(r.存在) || !有效(r.特征) || r.存在 == r.特征)
      return fail(S::入口拒绝);
    bool guardEstablished = false;
    try {
      const auto identity = 读取当前存在身份来源见证(r.G0, r.存在);
      auto mapIdentity = [](存在结构身份只读状态 state) noexcept {
        switch (state) {
        case 存在结构身份只读状态::入口拒绝:
          return S::入口拒绝;
        case 存在结构身份只读状态::未找到:
          return S::未找到;
        case 存在结构身份只读状态::事实代次漂移:
          return S::事实代次漂移;
        case 存在结构身份只读状态::资源失败:
          return S::资源失败;
        default:
          return S::内部不一致;
        }
      };
      if (!identity.成功(r.G0, r.存在))
        return fail(mapIdentity(identity.状态), identity.Gread);
      guardEstablished = true;

      const auto existence = 读取当前存在(r.存在, r.G0, false);
      if (!existence.成功() || existence.状态 != 存在类数据状态::已读取 ||
          existence.事实代次 != r.G0 || !existence.存在结点 ||
          existence.存在结点->结点 != r.存在)
        return fail(existence.状态 == 存在类数据状态::事实代次漂移
                        ? S::事实代次漂移
                        : existence.状态 == 存在类数据状态::资源失败
                              ? S::资源失败
                              : existence.状态 == 存在类数据状态::未找到
                                    ? S::未找到
                                    : S::内部不一致,
                    existence.事实代次);

      const 存在类成员引用 *known = nullptr;
      for (const auto &member : existence.存在结点->特征组) {
        if (member.目标结点 != r.特征)
          continue;
        if (known)
          return fail(S::内部不一致, r.G0);
        known = &member;
      }
      if (!known)
        return fail(S::未找到, r.G0);
      if (!有效(known->成员关系) || !有效(known->目标结点))
        return fail(S::内部不一致, r.G0);

      const 有界准确特征读取请求 featureRequest{有界准确特征读取合同版本,
                                                 r.G0,
                                                特征信息身份{r.特征},
                                                {4096, 4096, 4096, 65536}};
      const auto feature = 特征服务_.读取有界准确特征事实(featureRequest);
      auto mapFeature = [](特征类标量状态 state) noexcept {
        switch (state) {
        case 特征类标量状态::入口拒绝:
          return S::入口拒绝;
        case 特征类标量状态::未找到:
          return S::未找到;
        case 特征类标量状态::事实代次漂移:
          return S::事实代次漂移;
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
          feature.原请求.Gread != r.G0 ||
          feature.原请求.身份.编码 != r.特征 ||
          feature.事实->Gread != r.G0 ||
          feature.事实->信息.身份.编码 != r.特征) {
        auto observed = r.G0;
        if (feature.状态 == 特征类标量状态::事实代次漂移) {
          const auto current = 读取当前事实代次();
          observed = current.first == 存在结构身份只读状态::已读取
                         ? current.second
                         : 0;
        }
        return fail(feature.状态 == 特征类标量状态::已读取
                        ? S::内部不一致
                        : mapFeature(feature.状态),
                     observed);
      }

      const auto last = 读取当前事实代次();
      if (last.first != 存在结构身份只读状态::已读取)
        return fail(mapIdentity(last.first), last.second);
      if (last.second != r.G0)
        return fail(S::事实代次漂移, last.second);

      存在已知准确特征读取结果 out{S::已读取,
                                    存在已知准确特征只读合同版本,
                                    r.G0,
                                    r.存在,
                                    r.特征,
                                    存在已知准确特征见证{known->成员关系, r.特征,
                                                         feature.事实->创建G}};
      return out.当前成功(r) ? out : fail(S::内部不一致, r.G0);
    } catch (const std::bad_alloc &) {
      return fail(S::资源失败, guardEstablished ? r.G0 : 0);
    } catch (const std::length_error &) {
      return fail(S::资源失败, guardEstablished ? r.G0 : 0);
    } catch (...) {
      return fail(S::内部不一致, guardEstablished ? r.G0 : 0);
    }
  }
  存在当前采用结果 读取当前采用(const 存在当前采用读取请求 &) const;
  存在当前采用完整读取结果_v2 读取当前采用完整_v2(
      const 存在当前采用完整读取请求_v2 &) const noexcept;
  存在当前采用结果 变更当前采用(const 存在当前采用写请求 &);
  存在当前采用结果 收敛当前采用(const 存在当前采用写请求 &r) {
    return 变更当前采用(r);
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
              方法虚拟存在专用状态::方法节点未找到))
        return 方法规格失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证方法当前节点(
              请求.方法所有者, 请求.方法虚拟存在关系类型, 请求.期望事实代次,
              方法虚拟存在专用状态::关系类型未找到))
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
              方法虚拟存在专用状态::方法节点未找到))
        return 方法查询失败(*状态, 请求, 请求.期望事实代次);
      if (const auto 状态 = 验证方法当前节点(
              请求.方法所有者, 请求.方法虚拟存在关系类型, 请求.期望事实代次,
              方法虚拟存在专用状态::关系类型未找到))
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
                                出边.状态, 方法虚拟存在专用状态::方法节点未找到),
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
          投影关系->写入所有者 != 请求.方法所有者 ||
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
              读取采用组(请求.存在结点, 请求.期望事实代次,
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
      auto 当前 = 读取当前存在(请求.存在结点, 请求.期望事实代次, false);
      if (!当前.成功() || 当前.状态 != 存在类数据状态::已读取)
        return 当前;
      auto 写集 = 形成删除存在写集(请求, *当前.存在结点);
      const auto 族关系组 = 读取当前源关系组(请求.存在结点, 存在族归属关系类型_,
                                             请求.期望事实代次);
      if (!族关系组.成功)
        return 失败(族关系组.状态, 族关系组.事实代次);
      if (族关系组.关系组.size() != 1 ||
          !族归属关系有效(族关系组.关系组.front(), 请求.存在结点))
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
      const auto 状态 = 映射写入状态(写入.状态, 存在类数据状态::已删除);
      if (状态 != 存在类数据状态::已删除 && 状态 != 存在类数据状态::精确重复)
        return 失败(状态, 写入.事实代次);
      try {
        if (!写入结果头完整(写入, 请求.幂等身份))
          return 失败(存在类数据状态::内部不一致, 写入.事实代次);
        std::uint64_t 读回事实代次 = 写入.事实代次;
        if (状态 == 存在类数据状态::精确重复) {
          const auto 当前代次 = 读取当前事实代次();
          if (当前代次.first != 存在结构身份只读状态::已读取)
            return 失败(存在类数据状态::已可能发布, 写入.事实代次);
          读回事实代次 = 当前代次.second;
        }
        if (!写入.新编码映射.empty() ||
            !删除后当前未找到闭合(写集.退出事实, 读回事实代次))
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
        const auto 当前身份 =
            确认当前存在结构身份(读回事实代次, 请求.存在结点);
        if (当前身份.状态 != 存在结构身份只读状态::未找到 ||
            当前身份.Gread != 读回事实代次)
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
        return {存在类数据状态::已删除, 存在类数据合同版本,
                写入.事实代次, std::nullopt};
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

  static 存在身份来源当前见证读取结果
  当前见证失败(存在结构身份只读状态 状态,
               std::uint64_t Gread = 0) noexcept {
    return {状态, 存在结构身份只读合同版本, Gread, std::nullopt};
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
    return 项.状态 == L1所有者范围一致当前读取项目状态::成功 && 项.事实
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
           owner.所有者事实->创建事实代次 <= 请求.期望事实代次;
  }

  bool 普通空节点当前有效(const L1所有者范围节点事实 &节点,
                          稳定编码 编码) const noexcept {
    return 节点.编码 == 编码 && 节点.写入所有者 == 所有者_ &&
           节点.种类 == 节点种类::普通 && !节点.属性类型表示 &&
           节点.创建事实代次 != 0 && 节点.当前属性.empty();
  }

  bool 族归属关系有效(const L1所有者范围关系事实 &关系,
                       稳定编码 身份) const noexcept {
    return 有效(关系.编码) && 关系.源节点 == 身份 &&
           关系.目标节点 == 存在族锚点_ &&
           关系.关系类型节点 == 存在族归属关系类型_ && 关系.角色或顺序 == 1 &&
           关系.写入所有者 == 所有者_ && 关系.创建事实代次 != 0;
  }

  static 存在结构身份只读状态
  映射窄读取状态(L1所有者范围读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::未找到:
      return 存在结构身份只读状态::未找到;
    case L1所有者范围读取状态::事实代次漂移:
      return 存在结构身份只读状态::事实代次漂移;
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
    case 存在结构身份只读状态::入口拒绝:
      return 存在类数据状态::格式不支持;
    case 存在结构身份只读状态::事实代次漂移:
      return 存在类数据状态::事实代次漂移;
    case 存在结构身份只读状态::资源失败:
      return 存在类数据状态::资源失败;
    default:
      return 存在类数据状态::内部不一致;
    }
  }

  std::optional<存在结构身份只读状态> 当前来源读取错误(
      const L1结构所有者当前读取结果 &owner, const L1所有者范围当前读取结果 &e,
      const L1所有者范围当前读取结果 &anchor,
      const L1所有者范围当前读取结果 &type,
      const L1所有者范围源关系组读取结果 &links, std::uint64_t Gread,
      稳定编码 身份) const noexcept {
    if (e.读取事实代次 != Gread)
      return 存在结构身份只读状态::事实代次漂移;
    if (e.状态 != L1所有者范围读取状态::成功)
      return 映射窄读取状态(e.状态);
    const auto *eFact =
        e.事实 ? std::get_if<L1所有者范围节点事实>(&*e.事实) : nullptr;
    if (!eFact || e.合同版本 != L1所有者范围CRUD合同版本 || e.查询编码 != 身份)
      return 存在结构身份只读状态::内部不一致;
    if (eFact->创建事实代次 == 0 || eFact->创建事实代次 > Gread)
      return 存在结构身份只读状态::未找到;
    if (eFact->写入所有者 != 所有者_ || 身份 == 存在族锚点_ ||
        身份 == 存在族归属关系类型_)
      return 存在结构身份只读状态::入口拒绝;

    if (links.读取事实代次 != Gread)
      return 存在结构身份只读状态::事实代次漂移;
    if (links.状态 != L1所有者范围读取状态::成功)
      return 映射窄读取状态(links.状态);
    if (links.合同版本 != L1所有者范围CRUD合同版本 ||
        links.源节点 != 身份 ||
        links.关系类型节点 != 存在族归属关系类型_)
      return 存在结构身份只读状态::内部不一致;
    if (links.关系组.empty())
      return 存在结构身份只读状态::入口拒绝;

    const L1所有者范围当前读取结果 *元节点组[]{&anchor, &type};
    const 稳定编码 元编码组[]{存在族锚点_, 存在族归属关系类型_};
    for (std::size_t i = 0; i < 2; ++i) {
      const auto &r = *元节点组[i];
      if (r.读取事实代次 != Gread)
        return 存在结构身份只读状态::事实代次漂移;
      if (r.状态 != L1所有者范围读取状态::成功)
        return r.状态 == L1所有者范围读取状态::资源失败
                   ? 存在结构身份只读状态::资源失败
                   : 存在结构身份只读状态::内部不一致;
      const auto *n =
          r.事实 ? std::get_if<L1所有者范围节点事实>(&*r.事实) : nullptr;
      if (!n || r.合同版本 != L1所有者范围CRUD合同版本 ||
          r.查询编码 != 元编码组[i] || n->编码 != 元编码组[i] ||
          n->写入所有者 != 所有者_ || n->种类 != 节点种类::普通 ||
          n->属性类型表示 || !n->当前属性.empty() || n->创建事实代次 == 0 ||
          n->创建事实代次 > Gread)
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
    if (of.创建事实代次 == 0 || of.创建事实代次 > Gread)
      return 存在结构身份只读状态::入口拒绝;

    if (eFact->种类 != 节点种类::普通 || eFact->属性类型表示 ||
        !eFact->当前属性.empty() || links.关系组.size() != 1)
      return 存在结构身份只读状态::内部不一致;
    const auto &relation = links.关系组.front();
    if (!族归属关系有效(relation, 身份) ||
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
        owner.所有者事实->创建事实代次 > 首次.读取事实代次)
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
    return std::nullopt;
  }

  std::optional<方法虚拟存在专用状态>
  验证方法当前节点(L1结构所有者身份 所有者, 稳定编码 编码, std::uint64_t G0,
                   方法虚拟存在专用状态 未找到状态) const {
    const auto 读取 =
        第一层服务_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, 编码});
    if (读取.读取事实代次 != G0)
      return 方法虚拟存在专用状态::事实代次漂移;
    if (读取.状态 != L1所有者范围读取状态::成功)
      return 映射方法虚拟存在L1状态(读取.状态, 未找到状态);
    const auto *节点 =
        读取.事实 ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
    if (!节点 || 节点->编码 != 编码)
      return 方法虚拟存在专用状态::内部不一致;
    if (节点->写入所有者 != 所有者)
      return 方法虚拟存在专用状态::所有者不匹配;
    if (节点->种类 != 节点种类::普通 || 节点->属性类型表示 ||
        节点->创建事实代次 == 0 || 节点->创建事实代次 > G0 ||
        !节点->当前属性.empty())
      return 方法虚拟存在专用状态::结构冲突;
    return std::nullopt;
  }

  static 方法虚拟存在专用状态
  映射方法虚拟存在L1状态(L1所有者范围读取状态 状态,
                         方法虚拟存在专用状态 未找到状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::未找到:
      return 未找到状态;
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
           所有者.所有者事实->创建事实代次 <= 请求.期望事实代次;
  }

  static const L1所有者范围节点事实 *
  查找方法虚拟存在节点(const L1所有者范围一致当前读取结果 &读取,
                       稳定编码 编码) noexcept {
    const L1所有者范围节点事实 *找到 = nullptr;
    for (const auto &项 : 读取.节点) {
      if (项.查询编码 != 编码)
        continue;
      if (找到 || 项.状态 != L1所有者范围一致当前读取项目状态::成功 ||
          !项.事实)
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
          !项.事实)
        return nullptr;
      找到 = &*项.事实;
    }
    return 找到;
  }

  static bool 方法虚拟存在节点完整(const L1所有者范围节点事实 &节点,
                                   L1结构所有者身份 所有者) noexcept {
    return 有效(节点.编码) && 节点.写入所有者 == 所有者 &&
           节点.种类 == 节点种类::普通 && !节点.属性类型表示 &&
           节点.创建事实代次 != 0 && 节点.当前属性.empty();
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

  bool 实例特征结构有效() const {
    if (实例特征结构_.版本 != 2)
      return false;
    const std::array<稳定编码, 6> types{
        实例特征结构_.E到IF, 实例特征结构_.IF到F,
        实例特征结构_.IF到R集合, 实例特征结构_.R集合到版本,
        实例特征结构_.版本到R项, 实例特征结构_.R项到F};
    for (std::size_t i = 0; i < types.size(); ++i) {
      if (!关系类型有效(types[i]))
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (types[i] == types[j])
          return false;
      if (types[i] == 子存在关系类型_ || types[i] == 特征关系类型_ ||
          types[i] == 当前采用关系类型_)
        return false;
    }
    const auto material = 第一层服务_.读取所有者范围当前节点(
        {L1所有者范围CRUD合同版本, 实例特征结构_.R项材料属性类型});
    const auto *node = material.事实
        ? std::get_if<L1所有者范围节点事实>(&*material.事实) : nullptr;
    return 有效(实例特征结构_.R项材料属性类型) &&
           std::find(types.begin(), types.end(), 实例特征结构_.R项材料属性类型) == types.end() &&
           实例特征结构_.R项材料属性类型 != 子存在关系类型_ &&
           实例特征结构_.R项材料属性类型 != 特征关系类型_ &&
           实例特征结构_.R项材料属性类型 != 当前采用关系类型_ &&
           material.状态 == L1所有者范围读取状态::成功 &&
           material.合同版本 == L1所有者范围CRUD合同版本 &&
           material.查询编码 == 实例特征结构_.R项材料属性类型 && node &&
           node->编码 == 实例特征结构_.R项材料属性类型 &&
           node->写入所有者 == 所有者_ && node->种类 == 节点种类::属性类型 &&
           node->属性类型表示 == L1所有者范围值表示种类::U64组 &&
           node->创建事实代次 != 0;
  }

  bool 关系类型有效(稳定编码 编码) const {
    const auto 读取 =
        第一层服务_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, 编码});
    const auto *事实 =
        读取.事实 ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
    return 读取.状态 == L1所有者范围读取状态::成功 &&
           读取.合同版本 == L1所有者范围CRUD合同版本 && 读取.查询编码 == 编码 &&
           事实 &&
           事实->编码 == 编码 && 事实->写入所有者 == 所有者_ &&
           事实->种类 == 节点种类::普通 && !事实->属性类型表示 &&
           事实->创建事实代次 != 0;
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
          特征服务_.读取准确特征事实({1, 期望事实代次, {目标}});
      if (const auto *error = std::get_if<特征数据错误>(&read))
        return 映射特征状态(*error);
      const auto &f = std::get<准确特征读取事实>(read);
      if (f.Gread != 期望事实代次 ||
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
        读取.查询编码 != 存在结点 || !节点 || 节点->编码 != 存在结点 ||
        节点->写入所有者 != 所有者_ || 节点->种类 != 节点种类::普通 ||
        节点->属性类型表示 || 节点->创建事实代次 == 0 ||
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
    if (!规范化关系组(关系组, 存在结点, 类型, 期望事实代次))
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
            读取采用组(存在结点, 期望事实代次,
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
      } else if (写集.退出事实.size() != 1) {
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      }
      std::uint64_t 读取守卫事实代次 = 写入.事实代次;
      if (状态 == 存在类数据状态::精确重复) {
        const auto 当前 = 读取当前事实代次();
        if (当前.first != 存在结构身份只读状态::已读取)
          return 失败(存在类数据状态::已可能发布, 写入.事实代次);
        读取守卫事实代次 = 当前.second;
      }
      if (!添加 &&
          !删除后当前未找到闭合(写集.退出事实, 读取守卫事实代次))
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      auto 读回 = 读取当前存在(存在结点, 读取守卫事实代次, true);
      if (!读回.成功() || !读回.存在结点)
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      const auto *成员 = 查找成员(选择成员组(*读回.存在结点, 种类), 目标);
      if ((添加 && (!成员 || 成员->成员关系 != *新关系)) || (!添加 && 成员))
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      读回.状态 = 状态;
      读回.事实代次 = 写入.事实代次;
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
    const auto 当前代次 = 读取当前事实代次();
    if (当前代次.first != 存在结构身份只读状态::已读取)
      return 失败(存在类数据状态::已可能发布, 结果.事实代次);
    if (写集.合同版本 != L1所有者范围CRUD合同版本 ||
        写集.期望事实代次 != 请求.期望事实代次 ||
        写集.写入幂等身份 != 请求.幂等身份 || !写集.节点.empty() ||
        !写集.关系.empty() || !写集.值.empty() || !写集.属性槽变更.empty() ||
        写集.退出事实.empty() ||
        std::count(写集.退出事实.begin(), 写集.退出事实.end(), 请求.存在结点) !=
            1 ||
        结果.状态 != L1所有者范围写入状态::成功 ||
        !删除存在写集闭合(写集, 请求.存在结点, 当前代次.second))
      return 失败(存在类数据状态::幂等冲突, 结果.事实代次);
    const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
    if (写入.状态 != L1所有者范围写入状态::精确重复 ||
        !写入结果头完整(写入, 请求.幂等身份))
      return 失败(存在类数据状态::内部不一致, 写入.事实代次);
    try {
      const auto 当前 = 读取当前事实代次();
      if (当前.first != 存在结构身份只读状态::已读取)
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      if (!写入.新编码映射.empty() ||
          !删除存在写集闭合(写集, 请求.存在结点, 当前.second))
        return 失败(存在类数据状态::已可能发布, 写入.事实代次);
      return 存在类结点结果{存在类数据状态::已删除,
              存在类数据合同版本, 写入.事实代次, std::nullopt};
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
          关系.角色或顺序 != 1)
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
                     稳定编码 类型, std::uint64_t Gread) const {
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
          关系.角色或顺序 != 1 || 关系.创建事实代次 == 0 ||
          关系.创建事实代次 > Gread)
        return false;
      if (i != 0 && 关系组[i - 1].目标节点 == 关系.目标节点)
        return false;
    }
    return true;
  }

  bool 删除后当前未找到闭合(const std::vector<稳定编码> &编码组,
                            std::uint64_t 读回事实代次) const {
    if (编码组.empty() || 读回事实代次 == 0)
      return false;
    for (const auto 编码 : 编码组) {
      const auto 读取 = 存在类数据内部::读取存在当前事实(第一层服务_,编码);
      if (读取.状态 != L1所有者范围读取状态::未找到 ||
          读取.合同版本 != L1所有者范围CRUD合同版本 ||
          读取.查询编码 != 编码 ||
          读取.读取事实代次 != 读回事实代次 || 读取.事实)
        return false;
    }
    return true;
  }

  bool 删除存在写集闭合(const L1所有者范围写集请求 &写集, 稳定编码 存在结点,
                        std::uint64_t 读回事实代次) const {
    if (!有效(存在结点) || 读回事实代次 == 0 ||
        std::count(写集.退出事实.begin(), 写集.退出事实.end(),
                   存在结点) != 1)
      return false;
    return 删除后当前未找到闭合(写集.退出事实, 读回事实代次);
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
    case 特征数据错误::并发变化:
      return S::事实代次漂移;
    case 特征数据错误::入口拒绝:
      return S::入口拒绝;
    case 特征数据错误::资源失败:
      return S::资源失败;
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
    case L1所有者范围读取状态::事实代次漂移:
      return 存在类数据状态::事实代次漂移;
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
  准确特征读取事实 采用读F(特征信息身份 id, std::uint64_t g) const {
    auto result = 特征服务_.读取准确特征事实({1, g, id});
    if (const auto *e = std::get_if<特征数据错误>(&result))
      throw 采用失败{映射特征状态(*e)};
    auto f = std::get<准确特征读取事实>(std::move(result));
    采用要求(f.Gread == g && f.信息.身份 == id && 有效(f.类型关系));
    return f;
  }
  采用事实集 读取采用组(稳定编码 e, std::uint64_t g,
                         std::uint64_t budget) const {
    采用要求(有效(当前采用关系类型_), 采用S::格式不支持);
    采用要求(有效(e) && g && budget, 采用S::入口拒绝);
    采用守卫(g);
    const auto nr = 存在类数据内部::读取存在当前事实(第一层服务_,e);
    采用要求(nr.状态 == L1所有者范围读取状态::成功, 映射读取状态(nr.状态));
    采用要求(nr.读取事实代次 == g, 采用S::事实代次漂移);
    const auto *n =
        nr.事实 ? std::get_if<L1所有者范围节点事实>(&*nr.事实) : nullptr;
    采用要求(nr.合同版本 == L1所有者范围CRUD合同版本 && nr.查询编码 == e && n &&
                 n->编码 == e && n->写入所有者 == 所有者_ &&
                 n->种类 == 节点种类::普通 && !n->属性类型表示,
             采用S::引用冲突);
    采用要求(n->创建事实代次 && n->创建事实代次 <= g, 采用S::未找到);
    auto rows = [&](稳定编码 type) {
      const auto r = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本,e,type});
      采用要求(r.状态 == L1所有者范围读取状态::成功, 映射读取状态(r.状态));
      采用要求(r.读取事实代次 == g, 采用S::事实代次漂移);
      采用要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.源节点 == e &&
               r.关系类型节点 == type);
      采用要求(r.关系组.size() <= budget, 采用S::数量预算不足);
      auto out = r.关系组;
      采用要求(规范化关系组(out, e, type, g));
      return out;
    };
    采用事实集 out;
    out.已知 = rows(特征关系类型_);
    const auto selected = rows(当前采用关系类型_);
    采用要求(selected.size() <= budget - out.已知.size(), 采用S::数量预算不足);
    std::map<std::uint64_t, 特征类型身份> known;
    for (const auto &edge : out.已知) {
      auto f = 采用读F({edge.目标节点}, g);
      采用要求(known.emplace(edge.目标节点.值, f.信息.类型).second);
    }
    std::set<std::uint64_t> types;
    for (const auto &edge : selected) {
      auto f = 采用读F({edge.目标节点}, g);
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
    auto all = 读取采用组(r.E, g, r.关系预算);
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
      const auto old = 采用读F(expected->F, g);
      采用要求(old.信息.类型 == r.FT && out.当前 == expected, 采用S::引用冲突);
    } else if (out.当前)
      采用要求(next && out.当前->F == *next, 采用S::引用冲突);
    if (next) {
      const auto f = 采用读F(*next, g);
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
      const auto r = 存在类数据内部::读取存在当前事实(第一层服务_,*id);
      采用要求(r.状态 == L1所有者范围读取状态::成功, 映射读取状态(r.状态));
      采用要求(r.读取事实代次 == g, 采用S::事实代次漂移);
      const auto *e =
          r.事实 ? std::get_if<L1所有者范围关系事实>(&*r.事实) : nullptr;
      采用要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == *id &&
               e && e->编码 == *id && e->写入所有者 == 所有者_ &&
               e->创建事实代次 == saved.事实代次 &&
               e->源节点 == std::get<稳定编码>(wanted.源节点) &&
               e->目标节点 == std::get<稳定编码>(wanted.目标节点) &&
               e->关系类型节点 == std::get<稳定编码>(wanted.关系类型节点) &&
               e->角色或顺序 == wanted.角色或顺序);
    }
    for (auto id : ws.退出事实) {
      const auto r = 存在类数据内部::读取存在当前事实(第一层服务_,id);
      采用要求(r.状态 == L1所有者范围读取状态::未找到,
               r.状态 == L1所有者范围读取状态::资源失败
                   ? 采用S::资源失败
                   : 采用S::内部不一致);
      采用要求(r.读取事实代次 == g, 采用S::事实代次漂移);
      采用要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == id &&
               !r.事实);
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
    case X::事实代次漂移:
      return 绑定S::事实代次漂移;
    case X::资源失败:
      return 绑定S::资源失败;
    case X::数量预算不足:
      return 绑定S::数量预算不足;
    default:
      return 绑定S::内部不一致;
    }
  }
  template <class T, class F>
  绑定存在参与者结果<T> 绑定保护(std::uint64_t g, F &&fn) const {
    绑定存在参与者结果<T> out;
    out.Gread = g;
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
    return {绑定S::精确重复, f.读取事实代次, f};
  }
  绑定存在参与者结果<L1有限N分区原子参与者写集_v3>
  准备存在出生片段(const 绑定存在创建请求 &r, std::uint64_t g,
                   const std::optional<存在单例角色身份> &role) const override {
    return 绑定保护<L1有限N分区原子参与者写集_v3>(g, [&] {
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
        auto parent = 读取当前存在身份来源见证(g, r.绑定.绑定节点);
        if (!parent.成功(g, r.绑定.绑定节点))
          throw 绑定映射(parent.状态);
        p.写集.关系.push_back({{3},
                               r.绑定.绑定节点,
                               L1所有者范围写集本地键{1},
                               子存在关系类型_,
                               1});
      }
      if(role) {
        if(!角色结构_||*role!=角色结构_->项目角色)
          throw 绑定S::内部不一致;
        const 存在单例角色读取请求 q{1,g,*role,r.预算.最大关系数量};
        const auto existing=读取单例角色(q);
        if(!existing.确认未绑定(q)) {
          if(existing.状态==存在单例角色状态::角色冲突)
            throw 绑定S::包含冲突;
          throw 绑定S::内部不一致;
        }
        p.写集.关系.push_back({{static_cast<std::uint64_t>(single?4U:3U)},角色结构_->项目角色.值,
                               L1所有者范围写集本地键{1},
                               角色结构_->角色目标类型,1});
      }
      return p;
    });
  }
  存在单例角色读取结果 读取绑定单例角色(
      const 存在单例角色读取请求 &r) const noexcept override {
    return 读取单例角色(r);
  }
  绑定存在参与者结果<存在绑定出生见证>
  读取存在绑定出生(std::uint64_t g, 稳定编码 e,
                   const 存在初始绑定 &b) const override {
    return 绑定保护<存在绑定出生见证>(g, [&] {
      auto id = 读取当前存在身份来源见证(g, e);
      if (!id.成功(g, e))
        throw 绑定映射(id.状态);
      存在绑定出生见证 out{g, e, *id.见证, {}};
      if (b.种类 == 存在初始绑定种类::父存在组成) {
        auto q = 第一层服务_.读取所有者范围当前目标关系组(
            {L1所有者范围CRUD合同版本,e,子存在关系类型_});
        if (q.读取事实代次 != g)
          throw 绑定S::事实代次漂移;
        if (q.状态 != L1所有者范围读取状态::成功)
          throw 绑定S::内部不一致;
        for (const auto &x : q.关系组) {
          if (!x.创建事实代次 || x.创建事实代次 > g)
            continue;
          if (out.组成绑定关系 || x.源节点 != b.绑定节点 || x.目标节点 != e ||
              x.写入所有者 != 所有者_ || x.角色或顺序 != 1)
            throw 绑定S::内部不一致;
          out.组成绑定关系 =
              存在组成关系事实{g, x.编码, x.源节点, e, x.创建事实代次};
        }
        if (!out.组成绑定关系)
          throw 绑定S::绑定未找到;
      }
      return out;
    });
  }
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
        owner.所有者事实->创建事实代次 > 首次.读取事实代次)
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

  const L1事实基座服务& 原子I64底座() const noexcept override { return 第一层服务_; }
  L1所有者范围写端口& 原子I64端口() noexcept override { return 写入端口_; }
  bool 原子I64结构已就绪() const noexcept override { return 存在结构登记已就绪(); }
  原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
  准备原子I64出生片段(const 原子I64特征出生请求& r,std::uint64_t g) const override {
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3> out;out.Gread=g;
    try {
      if(!r.G0||g!=r.G0||!有效(r.位置.场景)||!r.键.已知.值)
        throw 原子I64特征出生状态::入口拒绝;
      const auto identity=读取当前存在身份来源见证(g,r.位置.场景);
      if(!identity.成功(g,r.位置.场景)) {
        using X=存在结构身份只读状态;
        switch(identity.状态) {
        case X::未找到: throw 原子I64特征出生状态::未找到;
        case X::事实代次漂移: throw 原子I64特征出生状态::事实代次漂移;
        case X::资源失败: throw 原子I64特征出生状态::资源失败;
        default: throw 原子I64特征出生状态::内部不一致;
        }
      }
      L1有限N分区原子参与者写集_v3 p; p.参与者={2};p.所有者=所有者_;p.写集={L1所有者范围CRUD合同版本,r.G0,r.键.已知};
      p.写集.关系.push_back({{1},r.位置.场景,L1三分区原子事实引用_v2{{1},{1}},特征关系类型_,1});out.数据=std::move(p);out.状态=原子I64特征出生状态::已创建;
    } catch(原子I64特征出生状态 s){out.状态=s;out.数据.reset();}
      catch(const std::bad_alloc&){out.状态=原子I64特征出生状态::资源失败;out.数据.reset();}
      catch(...){out.状态=原子I64特征出生状态::内部不一致;out.数据.reset();} return out;
  }

  static bool IFR持久请求相同(const 提交实例特征IFR目标请求 &left,
                             const 提交实例特征IFR目标请求 &right) noexcept {
    // 4115 v0.9：读回预算不进入首次持久写集，不能参与同键重放判定。
    return left.版本 == right.版本 && left.E == right.E && left.FT == right.FT &&
           left.G0 == right.G0 && left.预期快照 == right.预期快照 &&
           left.目标R项 == right.目标R项 && left.幂等身份 == right.幂等身份;
  }
  原子I64特征窄读取结果<原子I64特征holder事实> 读取原子I64holder(const 原子I64特征holder读取请求& r) const override {
    原子I64特征窄读取结果<原子I64特征holder事实> out;out.Gread=r.Gread;
    auto map=[](L1所有者范围读取状态 s) {using X=原子I64特征窄读取状态;switch(s) {
      case L1所有者范围读取状态::未找到:return X::未找到;
      case L1所有者范围读取状态::事实代次漂移:return X::事实代次漂移;
      case L1所有者范围读取状态::资源失败:return X::资源失败;default:return X::内部不一致;}};
    try {if(r.版本!=1||!r.Gread||!有效(r.holder)||!有效(r.F))return out;
      const auto guard=[&]{const auto x=第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});if(x.状态!=L1中性读取状态::成功||x.事实代次!=r.Gread)throw 原子I64特征窄读取状态::事实代次漂移;};guard();
      const auto rows=第一层服务_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,r.holder,特征关系类型_});
      if(rows.读取事实代次!=r.Gread)throw 原子I64特征窄读取状态::事实代次漂移;
      if(rows.状态!=L1所有者范围读取状态::成功)throw map(rows.状态);
      const L1所有者范围关系事实* selected=nullptr;
      for(const auto& e:rows.关系组) {
        if(e.目标节点!=r.F)continue;
        if(selected||e.写入所有者!=所有者_||e.源节点!=r.holder||
           e.关系类型节点!=特征关系类型_||e.角色或顺序!=1||
           !e.创建事实代次||e.创建事实代次>r.Gread)
          throw 原子I64特征窄读取状态::内部不一致;
        selected=&e;
      }
      if(!selected) {out.状态=原子I64特征窄读取状态::未找到;return out;}
      guard();out.事实=原子I64特征holder事实{r.holder,r.F,selected->编码,
          selected->关系类型节点,selected->创建事实代次};out.状态=原子I64特征窄读取状态::已读取;
    }catch(原子I64特征窄读取状态 s){out.状态=s;out.事实.reset();}catch(...){out.状态=原子I64特征窄读取状态::内部不一致;out.事实.reset();}return out;
  }
  稳定编码 当前采用关系类型_;
  void 核验单例角色结构首次材料(std::uint64_t g) const {
    if(!角色结构_||!g)throw 存在单例角色状态::入口拒绝;
    const auto first=写入端口_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,
         L1所有者范围写入幂等身份{0x1202U}});
    if(first.读取事实代次!=g)throw 存在单例角色状态::事实代次漂移;
    if(first.状态!=L1所有者范围读取状态::成功) {
      if(first.状态==L1所有者范围读取状态::资源失败)
        throw 存在单例角色状态::资源失败;
      if(first.状态==L1所有者范围读取状态::事实代次漂移)
        throw 存在单例角色状态::事实代次漂移;
      if(first.状态==L1所有者范围读取状态::未找到)
        throw 存在单例角色状态::未找到;
      throw 存在单例角色状态::内部不一致;
    }
    if(first.合同版本!=L1所有者范围首次写入读取合同版本||
       first.所有者!=所有者_||
       first.写入幂等身份!=L1所有者范围写入幂等身份{0x1202U}||
       !first.首次规范化写集||!first.首次写入结果)
      throw 存在单例角色状态::内部不一致;
    const auto &saved=*first.首次写入结果;
    const auto &ws=*first.首次规范化写集;
    if(saved.合同版本!=L1所有者范围CRUD合同版本||
       saved.状态!=L1所有者范围写入状态::成功||
       !saved.是否形成内存权威发布||saved.所有者!=所有者_||
       saved.写入幂等身份!=L1所有者范围写入幂等身份{0x1202U}||
       !ws.期望事实代次||saved.事实代次!=ws.期望事实代次+1||
       ws.合同版本!=L1所有者范围CRUD合同版本||
       ws.写入幂等身份!=L1所有者范围写入幂等身份{0x1202U})
      throw 存在单例角色状态::内部不一致;
    L1所有者范围写集请求 expected{
        L1所有者范围CRUD合同版本,ws.期望事实代次,
        L1所有者范围写入幂等身份{0x1202U}};
    expected.节点={{L1所有者范围写集本地键{1},节点种类::普通,std::nullopt},
                   {L1所有者范围写集本地键{2},节点种类::普通,std::nullopt},
                   {L1所有者范围写集本地键{3},节点种类::普通,std::nullopt}};
    expected.关系.push_back({L1所有者范围写集本地键{4},存在族锚点_,
        L1所有者范围事实引用{L1所有者范围写集本地键{3}},
        L1所有者范围事实引用{L1所有者范围写集本地键{1}},1});
    if(ws!=expected||saved.新编码映射.size()!=4)
      throw 存在单例角色状态::内部不一致;
    std::array<std::optional<稳定编码>,4> mapped{};
    for(const auto &[key,id]:saved.新编码映射) {
      if(!key.值||key.值>mapped.size()||mapped[key.值-1]||!有效(id))
        throw 存在单例角色状态::内部不一致;
      mapped[key.值-1]=id;
    }
    if(!mapped[0]||!mapped[1]||!mapped[2]||!mapped[3]||
       *mapped[0]!=角色结构_->角色登记类型||
       *mapped[1]!=角色结构_->角色目标类型||
       *mapped[2]!=角色结构_->项目角色.值)
      throw 存在单例角色状态::内部不一致;
    const auto relation=存在类数据内部::读取存在当前事实(第一层服务_,*mapped[3]);
    const L1所有者范围关系事实 *edge=nullptr;
    if(relation.事实)
      edge=std::get_if<L1所有者范围关系事实>(&*relation.事实);
    if(relation.读取事实代次!=g)throw 存在单例角色状态::事实代次漂移;
    if(relation.状态!=L1所有者范围读取状态::成功||!edge||
       edge->写入所有者!=所有者_||edge->源节点!=存在族锚点_||
       edge->目标节点!=角色结构_->项目角色.值||
       edge->关系类型节点!=角色结构_->角色登记类型||edge->角色或顺序!=1||
       edge->创建事实代次!=saved.事实代次)
      throw 存在单例角色状态::内部不一致;
  }
  mutable std::mutex 写入锁_;
  std::optional<存在当前采用写请求> 采用待确认_;
  std::optional<提交实例特征IFR目标请求> IFR待确认_;
  const L1事实基座服务 &第一层服务_;
  const 特征类数据服务 &特征服务_;
  L1所有者范围写端口 写入端口_;
  L1结构所有者身份 所有者_{};
  稳定编码 子存在关系类型_{};
  稳定编码 特征关系类型_{};
  实例特征结构交付 实例特征结构_{};
  稳定编码 存在族锚点_{};
  稳定编码 存在族归属关系类型_{};
  std::optional<存在单例角色结构交付> 角色结构_;
};

inline 实例特征结构登记结果 存在类数据服务::登记实例特征结构(
    const L1事实基座服务 &l1, L1所有者范围写端口 &port,
    const 实例特征结构登记请求 &r) noexcept {
  constexpr std::uint64_t v1key = 0x4946525354525543ULL;
  constexpr std::uint64_t v2key = 0x4946525354525632ULL;
  实例特征结构登记结果 out; out.Gread = r.G0;
  bool dispatched = false;
  try {
    if (r.版本 != 2 || !r.G0 || r.G0 == UINT64_MAX || r.幂等身份.值 != v2key ||
        !port.有效() || !port.绑定于(l1)) return out;
    const auto owner = port.所有者身份();
    const auto v1 = port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, {v1key}});
    if (v1.合同版本 != L1所有者范围首次写入读取合同版本 ||
        v1.所有者 != owner || v1.写入幂等身份.值 != v1key)
      throw 实例特征结构状态::内部不一致;
    std::array<稳定编码, 6> v1ids{};
    const bool hasV1 = v1.状态 == L1所有者范围读取状态::成功;
    if (hasV1) {
      if (!v1.首次规范化写集 || !v1.首次写入结果 ||
          v1.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          v1.首次写入结果->所有者 != owner ||
          v1.首次写入结果->写入幂等身份.值 != v1key ||
          v1.首次规范化写集->合同版本 != L1所有者范围CRUD合同版本 ||
          v1.首次规范化写集->写入幂等身份.值 != v1key ||
          v1.首次规范化写集->节点.size() != 6 ||
          !v1.首次规范化写集->关系.empty() || !v1.首次规范化写集->值.empty() ||
          !v1.首次规范化写集->属性槽变更.empty() || !v1.首次规范化写集->退出事实.empty() ||
          v1.首次写入结果->新编码映射.size() != 6)
        throw 实例特征结构状态::内部不一致;
      for (std::size_t i = 0; i < 6; ++i) {
        const auto &n = v1.首次规范化写集->节点[i];
        if (n.本地键.值 != i + 1 || n.种类 != 节点种类::普通 || n.属性类型表示)
          throw 实例特征结构状态::内部不一致;
      }
      for (const auto &[local, id] : v1.首次写入结果->新编码映射) {
        if (!local.值 || local.值 > v1ids.size() || !有效(id) ||
            有效(v1ids[local.值 - 1]))
          throw 实例特征结构状态::内部不一致;
        v1ids[local.值 - 1] = id;
      }
      for (const auto id : v1ids) {
        const auto raw = l1.读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, id});
        const auto *node = raw.事实 ? std::get_if<L1所有者范围节点事实>(&*raw.事实) : nullptr;
        if (raw.状态 != L1所有者范围读取状态::成功 ||
            raw.合同版本 != L1所有者范围CRUD合同版本 || raw.查询编码 != id ||
            !node || node->编码 != id || node->写入所有者 != owner ||
            node->种类 != 节点种类::普通 || node->属性类型表示 ||
            !node->创建事实代次)
          throw 实例特征结构状态::内部不一致;
      }
    } else if (v1.状态 != L1所有者范围读取状态::未找到) {
      throw v1.状态 == L1所有者范围读取状态::资源失败
          ? 实例特征结构状态::资源失败 : 实例特征结构状态::内部不一致;
    }
    const auto current = l1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (current.状态 == L1中性读取状态::资源失败)
      throw 实例特征结构状态::资源失败;
    if (current.状态 != L1中性读取状态::成功 || !current.事实代次)
      throw 实例特征结构状态::内部不一致;
    if (current.事实代次 != r.G0)
      throw 实例特征结构状态::事实代次漂移;
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
    if (hasV1) {
      ws.节点.emplace_back(L1所有者范围节点新建项{{1}, 节点种类::属性类型,
          L1所有者范围值表示种类::U64组});
    } else {
      for (std::uint64_t i = 1; i <= 6; ++i)
        ws.节点.emplace_back(L1所有者范围节点新建项{
            L1所有者范围写集本地键{static_cast<std::uint32_t>(i)}, 节点种类::普通, std::nullopt});
      ws.节点.emplace_back(L1所有者范围节点新建项{{7}, 节点种类::属性类型,
          L1所有者范围值表示种类::U64组});
    }
    const auto first = port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != owner || first.写入幂等身份 != r.幂等身份)
      throw 实例特征结构状态::内部不一致;
    const bool replay = first.状态 == L1所有者范围读取状态::成功;
    const auto 写集语义相同 = [](const L1所有者范围写集请求 &首次,
                                  const L1所有者范围写集请求 &本次) {
      return 首次.合同版本 == 本次.合同版本 &&
             首次.写入幂等身份 == 本次.写入幂等身份 &&
             首次.节点 == 本次.节点 && 首次.关系 == 本次.关系 &&
             首次.值 == 本次.值 && 首次.属性槽变更 == 本次.属性槽变更 &&
             首次.退出事实 == 本次.退出事实;
    };
    if (replay) {
      if (!first.首次规范化写集 || !写集语义相同(*first.首次规范化写集, ws) || !first.首次写入结果)
        throw 实例特征结构状态::幂等冲突;
    } else if (first.状态 == L1所有者范围读取状态::未找到) {
      if (first.读取事实代次 != r.G0 || first.首次规范化写集 || first.首次写入结果)
        throw 实例特征结构状态::事实代次漂移;
    } else throw first.状态 == L1所有者范围读取状态::资源失败 ?
        实例特征结构状态::资源失败 : 实例特征结构状态::内部不一致;
    if (!replay) dispatched = true;
    const auto &saved = replay ? *first.首次写入结果 : port.提交所有者范围中性写集(ws);
    out.Gread = replay ? current.事实代次 : saved.事实代次;
    if (saved.状态 != L1所有者范围写入状态::成功 ||
        saved.新编码映射.size() != (hasV1 ? 1U : 7U)) {
      if (saved.状态 == L1所有者范围写入状态::事实代次漂移) throw 实例特征结构状态::事实代次漂移;
      if (saved.状态 == L1所有者范围写入状态::幂等冲突) throw 实例特征结构状态::幂等冲突;
      throw 实例特征结构状态::已可能发布;
    }
    std::array<稳定编码,7> ids{};
    for (const auto &[local,id] : saved.新编码映射) {
      if (!local.值 || local.值 > ids.size() || !有效(id) || 有效(ids[local.值-1]))
        throw 实例特征结构状态::内部不一致;
      ids[local.值-1] = id;
    }
    if (hasV1) {
      for (std::size_t i = 0; i < 6; ++i) ids[i] = v1ids[i];
      ids[6] = saved.新编码映射.front().second;
    }
    for (std::size_t i = 0; i < ids.size(); ++i) {
      const auto id = ids[i];
      const auto raw = 存在类数据内部::读取存在当前事实(l1,id);
      const auto *node = raw.事实 ? std::get_if<L1所有者范围节点事实>(&*raw.事实) : nullptr;
      if (raw.状态 != L1所有者范围读取状态::成功 || raw.读取事实代次 != out.Gread ||
          !node || node->写入所有者 != owner || node->编码 != id ||
          node->种类 != (i == 6 ? 节点种类::属性类型 : 节点种类::普通) ||
          node->属性类型表示 != (i == 6
              ? std::optional<L1所有者范围值表示种类>{L1所有者范围值表示种类::U64组}
              : std::nullopt) ||
          ((replay || (hasV1 && i < 6)) ? node->创建事实代次 == 0
                                         : node->创建事实代次 != saved.事实代次))
        throw 实例特征结构状态::内部不一致;
    }
    out.首次发布代次 = saved.事实代次;
    out.交付 = {2,ids[0],ids[1],ids[2],ids[3],ids[4],ids[5],ids[6]};
    out.版本 = 2;
    out.状态 = replay ? 实例特征结构状态::精确重复 : 实例特征结构状态::已登记;
  } catch (实例特征结构状态 s) {
    out.状态 = dispatched && s != 实例特征结构状态::事实代次漂移 && s != 实例特征结构状态::幂等冲突 ?
        实例特征结构状态::已可能发布 : s;
    if (out.状态 != 实例特征结构状态::已登记 && out.状态 != 实例特征结构状态::精确重复) out.交付.reset();
  } catch (const std::bad_alloc &) { out.状态 = dispatched ? 实例特征结构状态::已可能发布 : 实例特征结构状态::资源失败; out.交付.reset(); }
    catch (...) { out.状态 = dispatched ? 实例特征结构状态::已可能发布 : 实例特征结构状态::内部不一致; out.交付.reset(); }
  return out;
}

inline 存在单例角色结构登记结果 存在类数据服务::登记单例角色结构(
    const L1事实基座服务 &l1,L1所有者范围写端口 &port,
    const 存在单例角色结构登记请求 &r) noexcept {
  存在单例角色结构登记结果 out;out.Gread=r.G0;
  bool entered=false;
  try {
    if(r.版本!=1||!r.G0||r.G0==UINT64_MAX||r.幂等身份.值!=0x1202U||
       !port.有效()||!port.绑定于(l1)) return out;
    const auto owner=port.所有者身份();
    const auto family=port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,存在族来源初始化幂等身份});
    if(family.状态!=L1所有者范围读取状态::成功||!family.首次规范化写集||
       !family.首次写入结果||family.所有者!=owner||
       family.首次规范化写集->写入幂等身份!=存在族来源初始化幂等身份)
      throw 存在单例角色状态::内部不一致;
    const auto anchor=存在类数据内部::查找唯一编码(
        *family.首次写入结果,存在类数据内部::存在族锚点本地键);
    if(!anchor)throw 存在单例角色状态::内部不一致;
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
    ws.节点={{L1所有者范围写集本地键{1},节点种类::普通,std::nullopt},
             {L1所有者范围写集本地键{2},节点种类::普通,std::nullopt},
             {L1所有者范围写集本地键{3},节点种类::普通,std::nullopt}};
    ws.关系.push_back({L1所有者范围写集本地键{4},*anchor,L1所有者范围事实引用{L1所有者范围写集本地键{3}},L1所有者范围事实引用{L1所有者范围写集本地键{1}},1});
    const auto first=port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,r.幂等身份});
    bool replay=false;
    if(first.合同版本!=L1所有者范围首次写入读取合同版本||
       first.所有者!=owner||first.写入幂等身份!=r.幂等身份)
      throw 存在单例角色状态::内部不一致;
    if(first.状态==L1所有者范围读取状态::成功) {
      replay=true;
      if(!first.首次规范化写集||*first.首次规范化写集!=ws||
         !first.首次写入结果||first.首次写入结果->状态!=L1所有者范围写入状态::成功||
         !first.首次写入结果->是否形成内存权威发布)
        throw 存在单例角色状态::幂等冲突;
    } else if(first.状态==L1所有者范围读取状态::未找到) {
      if(first.读取事实代次!=r.G0)
        throw 存在单例角色状态::事实代次漂移;
      if(first.首次规范化写集||first.首次写入结果)
        throw 存在单例角色状态::内部不一致;
    } else {
      if(first.状态==L1所有者范围读取状态::资源失败)
        throw 存在单例角色状态::资源失败;
      throw 存在单例角色状态::内部不一致;
    }
    entered=true;
    const auto saved=port.提交所有者范围中性写集(ws);
    out.Gread=saved.事实代次;
    const auto expected=replay ? L1所有者范围写入状态::精确重复
                               : L1所有者范围写入状态::成功;
    if(saved.状态!=expected||saved.事实代次!=r.G0+1||saved.新编码映射.size()!=4) {
      if(saved.状态==L1所有者范围写入状态::事实代次漂移)
        throw 存在单例角色状态::事实代次漂移;
      if(saved.状态==L1所有者范围写入状态::幂等冲突)
        throw 存在单例角色状态::幂等冲突;
      throw 存在单例角色状态::已可能发布;
    }
    const auto one=[&](std::uint64_t key){
      std::optional<稳定编码> found;
      for(const auto &[k,id]:saved.新编码映射)if(k.值==key){if(found)throw 存在单例角色状态::内部不一致;found=id;}
      if(!found||!有效(*found))throw 存在单例角色状态::内部不一致;
      return *found;
    };
    const auto reg=one(1),target=one(2),role=one(3),edge=one(4);
    if(reg==target||reg==role||target==role)throw 存在单例角色状态::内部不一致;
    for(const auto id:{reg,target,role}) {
      const auto raw=存在类数据内部::读取存在当前事实(l1,id);
      const auto *node=raw.事实 ? std::get_if<L1所有者范围节点事实>(&*raw.事实) : nullptr;
      if(raw.状态!=L1所有者范围读取状态::成功||raw.读取事实代次<saved.事实代次||
         !node||node->写入所有者!=owner||node->创建事实代次!=saved.事实代次||
         node->种类!=节点种类::普通||node->属性类型表示)
        throw 存在单例角色状态::内部不一致;
      out.Gread=raw.读取事实代次;
    }
    const auto er=存在类数据内部::读取存在当前事实(l1,edge);
    const auto *ef=er.事实 ? std::get_if<L1所有者范围关系事实>(&*er.事实) : nullptr;
    if(er.状态!=L1所有者范围读取状态::成功||!ef||ef->写入所有者!=owner||
       ef->源节点!=*anchor||ef->目标节点!=role||ef->关系类型节点!=reg||
       ef->角色或顺序!=1||ef->创建事实代次!=saved.事实代次)
      throw 存在单例角色状态::内部不一致;
    out.Gread=er.读取事实代次;out.首次发布代次=saved.事实代次;
    out.交付=存在单例角色结构交付{1,reg,target,{role}};
    out.状态=replay?存在单例角色状态::精确重复:存在单例角色状态::已登记;
  } catch(存在单例角色状态 s) {
    out.状态=s;
    if(entered&&s!=存在单例角色状态::事实代次漂移&&s!=存在单例角色状态::幂等冲突)
      out.状态=存在单例角色状态::已可能发布;
    if(out.状态!=存在单例角色状态::精确重复&&out.状态!=存在单例角色状态::已登记)
      out.交付.reset();
  } catch(const std::bad_alloc&) {out.状态=entered?存在单例角色状态::已可能发布:存在单例角色状态::资源失败;out.交付.reset();}
    catch(...) {out.状态=entered?存在单例角色状态::已可能发布:存在单例角色状态::内部不一致;out.交付.reset();}
  return out;
}

inline 存在单例角色读取结果 存在类数据服务::读取单例角色(
    const 存在单例角色读取请求 &r) const noexcept {
  存在单例角色读取结果 out;out.Gread=r.Gread;
  try {
    if(!角色结构_||r.版本!=1||!r.Gread||
       r.角色!=角色结构_->项目角色||!r.最大关系数)
      throw 存在单例角色状态::入口拒绝;
    核验单例角色结构首次材料(r.Gread);
    const auto registration=第一层服务_.读取所有者范围当前目标关系组(
        {L1所有者范围CRUD合同版本,r.角色.值,角色结构_->角色登记类型});
    if(registration.读取事实代次!=r.Gread)throw 存在单例角色状态::事实代次漂移;
    if(registration.状态!=L1所有者范围读取状态::成功) {
      if(registration.状态==L1所有者范围读取状态::资源失败)
        throw 存在单例角色状态::资源失败;
      if(registration.状态==L1所有者范围读取状态::事实代次漂移)
        throw 存在单例角色状态::事实代次漂移;
      if(registration.状态==L1所有者范围读取状态::未找到)
        throw 存在单例角色状态::未找到;
      throw 存在单例角色状态::内部不一致;
    }
    if(registration.关系组.empty())throw 存在单例角色状态::未找到;
    if(registration.关系组.size()!=1)throw 存在单例角色状态::内部不一致;
    const auto &reg=registration.关系组.front();
    if(reg.写入所有者!=所有者_||reg.源节点!=存在族锚点_||reg.目标节点!=r.角色.值||
       reg.关系类型节点!=角色结构_->角色登记类型||reg.角色或顺序!=1||
       !reg.创建事实代次||reg.创建事实代次>r.Gread)
      throw 存在单例角色状态::内部不一致;
    const auto targets=第一层服务_.读取所有者范围当前源关系组(
        {L1所有者范围CRUD合同版本,r.角色.值,角色结构_->角色目标类型});
    if(targets.读取事实代次!=r.Gread)throw 存在单例角色状态::事实代次漂移;
    if(targets.状态!=L1所有者范围读取状态::成功) {
      if(targets.状态==L1所有者范围读取状态::资源失败)
        throw 存在单例角色状态::资源失败;
      throw 存在单例角色状态::内部不一致;
    }
    std::vector<L1所有者范围关系事实> active;
    for(const auto &x:targets.关系组)
      if(x.创建事实代次&&x.创建事实代次<=r.Gread)active.push_back(x);
    if(active.size()>r.最大关系数)throw 存在单例角色状态::数量预算不足;
    if(active.empty()){out.状态=存在单例角色状态::未绑定;return out;}
    if(active.size()!=1)throw 存在单例角色状态::角色冲突;
    const auto &target=active.front();
    if(target.写入所有者!=所有者_||target.源节点!=r.角色.值||
       target.关系类型节点!=角色结构_->角色目标类型||target.角色或顺序!=1)
      throw 存在单例角色状态::内部不一致;
    const auto identity=读取当前存在身份来源见证(r.Gread,target.目标节点);
    if(!identity.成功(r.Gread,target.目标节点)||!identity.见证) {
      if(identity.状态==存在结构身份只读状态::事实代次漂移)
        throw 存在单例角色状态::事实代次漂移;
      if(identity.状态==存在结构身份只读状态::数量预算不足)
        throw 存在单例角色状态::数量预算不足;
      if(identity.状态==存在结构身份只读状态::资源失败)
        throw 存在单例角色状态::资源失败;
      throw 存在单例角色状态::内部不一致;
    }
    out.事实=存在单例角色事实{r.角色,reg.编码,target.编码,target.目标节点,
        reg.创建事实代次,target.创建事实代次,*identity.见证};
    out.状态=存在单例角色状态::已读取;
    const auto tail=读取当前事实代次();
    if(tail.first!=存在结构身份只读状态::已读取||tail.second!=r.Gread)
      throw 存在单例角色状态::事实代次漂移;
  } catch(存在单例角色状态 s){out.状态=s;out.事实.reset();}
    catch(const std::bad_alloc&){out.状态=存在单例角色状态::资源失败;out.事实.reset();}
    catch(...){out.状态=存在单例角色状态::内部不一致;out.事实.reset();}
  return out;
}

inline 存在当前采用结果
存在类数据服务::读取当前采用(const 存在当前采用读取请求 &r) const {
  存在当前采用结果 out;
  out.Gread = r.Gread;
  try {
    采用要求(r.版本 == 1 && 有效(r.FT), 采用S::入口拒绝);
    const auto all = 读取采用组(r.E, r.Gread, r.关系预算);
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

inline 存在单例角色当前完整读取结果_v2
存在类数据服务::读取单例角色当前完整_v2(
    const 存在单例角色当前完整读取请求_v2 &r) const noexcept {
  using 状态 = 存在单例角色当前完整读取状态_v2;
  存在单例角色当前完整读取结果_v2 out;
  out.Gread=r.Gread;
  try {
    if(!角色结构_||r.版本!=存在单例角色当前完整读取合同版本_v2||
       !r.Gread||r.角色!=角色结构_->项目角色)
      throw 状态::入口拒绝;
    核验单例角色结构首次材料(r.Gread);
    const auto registration=第一层服务_.读取所有者范围当前目标关系组(
        {L1所有者范围CRUD合同版本,r.角色.值,角色结构_->角色登记类型});
    if(registration.读取事实代次!=r.Gread)throw 状态::事实代次漂移;
    if(registration.状态!=L1所有者范围读取状态::成功) {
      if(registration.状态==L1所有者范围读取状态::资源失败)throw 状态::资源失败;
      if(registration.状态==L1所有者范围读取状态::事实代次漂移)throw 状态::事实代次漂移;
      if(registration.状态==L1所有者范围读取状态::未找到)throw 状态::内部不一致;
      throw 状态::内部不一致;
    }
    if(registration.关系组.size()!=1)throw 状态::内部不一致;
    const auto &reg=registration.关系组.front();
    if(reg.写入所有者!=所有者_||reg.源节点!=存在族锚点_||
       reg.目标节点!=r.角色.值||reg.关系类型节点!=角色结构_->角色登记类型||
       reg.角色或顺序!=1||!reg.创建事实代次||reg.创建事实代次>r.Gread)
      throw 状态::内部不一致;
    const auto targets=第一层服务_.读取所有者范围当前源关系组(
        {L1所有者范围CRUD合同版本,r.角色.值,角色结构_->角色目标类型});
    if(targets.读取事实代次!=r.Gread)throw 状态::事实代次漂移;
    if(targets.状态!=L1所有者范围读取状态::成功) {
      if(targets.状态==L1所有者范围读取状态::资源失败)throw 状态::资源失败;
      if(targets.状态==L1所有者范围读取状态::事实代次漂移)throw 状态::事实代次漂移;
      throw 状态::内部不一致;
    }
    std::vector<L1所有者范围关系事实> active;
    for(const auto &x:targets.关系组)
      if(x.创建事实代次&&x.创建事实代次<=r.Gread)active.push_back(x);
    if(active.empty()){out.状态=状态::未绑定;return out;}
    if(active.size()!=1)throw 状态::角色冲突;
    const auto &target=active.front();
    if(target.写入所有者!=所有者_||target.源节点!=r.角色.值||
       target.关系类型节点!=角色结构_->角色目标类型||target.角色或顺序!=1)
      throw 状态::内部不一致;
    const auto identity=读取当前存在身份来源见证(r.Gread,target.目标节点);
    if(!identity.成功(r.Gread,target.目标节点)||!identity.见证) {
      if(identity.状态==存在结构身份只读状态::事实代次漂移)throw 状态::事实代次漂移;
      if(identity.状态==存在结构身份只读状态::资源失败)throw 状态::资源失败;
      throw 状态::内部不一致;
    }
    out.事实=存在单例角色事实{r.角色,reg.编码,target.编码,target.目标节点,
        reg.创建事实代次,target.创建事实代次,*identity.见证};
    out.状态=状态::已读取;
    const auto tail=读取当前事实代次();
    if(tail.first!=存在结构身份只读状态::已读取||tail.second!=r.Gread)
      throw 状态::事实代次漂移;
  } catch(状态 s){out.状态=s;out.事实.reset();}
    catch(const std::bad_alloc&){out.状态=状态::资源失败;out.事实.reset();}
    catch(...){out.状态=状态::内部不一致;out.事实.reset();}
  return out;
}
inline 存在当前采用完整读取结果_v2
存在类数据服务::读取当前采用完整_v2(
    const 存在当前采用完整读取请求_v2 &r) const noexcept {
  using S = 存在当前采用完整读取状态_v2;
  存在当前采用完整读取结果_v2 out;
  out.Gread = r.Gread;
  const auto fail = [&](S state) {
    out.状态 = state;
    out.采用.reset();
  };
  try {
    if (r.版本 != 2 || !r.Gread || !有效(r.E) || !有效(r.FT)) {
      fail(S::入口拒绝);
      return out;
    }
    采用守卫(r.Gread);
    const auto nodeRead = 存在类数据内部::读取存在当前事实(第一层服务_,r.E);
    if (nodeRead.状态 != L1所有者范围读取状态::成功) {
      fail(nodeRead.状态 == L1所有者范围读取状态::未找到
               ? S::未找到
           : nodeRead.状态 == L1所有者范围读取状态::事实代次漂移
               ? S::事实代次漂移
           : nodeRead.状态 == L1所有者范围读取状态::资源失败
               ? S::资源失败
               : S::内部不一致);
      return out;
    }
    const auto *node = nodeRead.事实
                           ? std::get_if<L1所有者范围节点事实>(&*nodeRead.事实)
                           : nullptr;
    if (nodeRead.合同版本 != L1所有者范围CRUD合同版本 ||
        nodeRead.查询编码 != r.E || nodeRead.读取事实代次 != r.Gread || !node ||
        node->编码 != r.E || node->写入所有者 != 所有者_ ||
        node->种类 != 节点种类::普通 || node->属性类型表示) {
      fail(S::内部不一致);
      return out;
    }
    if (!node->创建事实代次 || node->创建事实代次 > r.Gread) {
      fail(S::未找到);
      return out;
    }
    const auto mapReadState = [](L1所有者范围读取状态 state) noexcept {
      return state == L1所有者范围读取状态::入口拒绝 ? S::入口拒绝
           : state == L1所有者范围读取状态::未找到 ? S::未找到
           : state == L1所有者范围读取状态::事实代次漂移 ? S::事实代次漂移
           : state == L1所有者范围读取状态::资源失败 ? S::资源失败
           : S::内部不一致;
    };
    const auto readRelations = [&](稳定编码 type) {
      const auto result = 第一层服务_.读取所有者范围当前源关系组(
          {L1所有者范围CRUD合同版本,r.E,type});
      if (result.状态 != L1所有者范围读取状态::成功 ||
          result.读取事实代次 != r.Gread)
        throw mapReadState(result.状态);
      return result.关系组;
    };
    std::map<std::uint64_t, 特征类型身份> knownFeatures;
    for (const auto &edge : readRelations(特征关系类型_)) {
      if (edge.写入所有者 != 所有者_ || edge.源节点 != r.E ||
          edge.关系类型节点 != 特征关系类型_ || edge.角色或顺序 != 1) {
        fail(S::内部不一致);
        return out;
      }
      const auto feature = 采用读F({edge.目标节点}, r.Gread);
      if (!knownFeatures.emplace(edge.目标节点.值, feature.信息.类型).second) {
        fail(S::内部不一致);
        return out;
      }
    }
    std::set<std::uint64_t> selectedFeatures;
    std::set<std::uint64_t> selectedTypes;
    for (const auto &edge : readRelations(当前采用关系类型_)) {
      if (edge.写入所有者 != 所有者_ || edge.源节点 != r.E ||
          edge.关系类型节点 != 当前采用关系类型_ || edge.角色或顺序 != 1) {
        fail(S::内部不一致);
        return out;
      }
      const auto feature = 采用读F({edge.目标节点}, r.Gread);
      const auto known = knownFeatures.find(edge.目标节点.值);
      if (known == knownFeatures.end() || known->second != feature.信息.类型 ||
          !selectedFeatures.insert(edge.目标节点.值).second ||
          !selectedTypes.insert(feature.信息.类型.编码.值).second) {
        fail(S::内部不一致);
        return out;
      }
      if (feature.信息.类型 == r.FT) {
        out.采用 = 存在当前采用事实{edge.编码, r.E, r.FT, feature.信息.身份};
      }
    }
    采用守卫(r.Gread);
    out.状态 = S::已读取;
    if (!out.成功(r)) fail(S::内部不一致);
  } catch (const S &state) {
    fail(state);
  } catch (const 采用失败 &e) {
    fail(e.状态 == 采用S::未找到 ? S::未找到
         : e.状态 == 采用S::事实代次漂移 ? S::事实代次漂移
         : e.状态 == 采用S::资源失败 ? S::资源失败
         : e.状态 == 采用S::入口拒绝 ? S::入口拒绝
         : S::内部不一致);
  } catch (const std::bad_alloc &) {
    fail(S::资源失败);
  } catch (const std::length_error &) {
    fail(S::资源失败);
  } catch (...) {
    fail(S::内部不一致);
  }
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
        out.首次发布代次 = first.首次写入结果->事实代次;
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
        out.首次发布代次 = saved.事实代次;
      out.Gread = 采用当前G();
    }
    采用互证(ws, saved, out.Gread);
    out.首次发布代次 = saved.事实代次;
    const auto all = 读取采用组(r.E, out.Gread, r.关系预算);
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
  if (版本 != 1 || !Gread)
    return false;
  if (采用 && (!有效(采用->关系) || !有效(采用->E) || !有效(采用->FT) ||
               !有效(采用->F)))
    return false;
  if (状态 == S::已读取)
    return !首次发布代次 && !原请求;
  if (!原请求 || 原请求->版本 != 1 || !原请求->G || 原请求->G > Gread ||
      !有效(原请求->幂等身份) || !有效(原请求->E) || !有效(原请求->FT) ||
      !原请求->关系预算 || 原请求->操作.valueless_by_exception())
    return false;
  if (状态 == S::精确重复) {
    if (首次发布代次 && (!*首次发布代次 || *首次发布代次 > Gread))
      return false;
  } else if (!首次发布代次 || !*首次发布代次 || *首次发布代次 > Gread)
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
                 首次发布代次.has_value();
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

inline bool 存在当前采用完整读取结果_v2::成功(
    const 存在当前采用完整读取请求_v2 &r) const noexcept {
  if (版本 != 2 || r.版本 != 2 ||
      状态 != 存在当前采用完整读取状态_v2::已读取 || !r.Gread ||
      !有效(r.E) || !有效(r.FT) || Gread != r.Gread)
    return false;
  return !采用 || (有效(采用->关系) && 采用->E == r.E && 采用->FT == r.FT &&
                    有效(采用->F));
}

inline 实例特征IFR结果 存在类数据服务::读取实例特征IFR(
    const 读取实例特征IFR请求 &r) const noexcept {
  using S = 实例特征IFR状态;
  实例特征IFR结果 out;
  out.Gread = r.Gread;
  try {
    const auto budgetValid = [](const 实例特征IFR读取预算 &b) noexcept {
      return b.最大F成员数 && b.最大R项数 && b.最大关系数 &&
             b.最大材料值数 && b.最大单材料U64元素数;
    };
    if (r.版本 != 1 || !有效(r.E) || !有效(r.FT) || !r.Gread ||
        !budgetValid(r.预算))
      return out;
    const auto fail = [&](S s, std::uint64_t g = 0) {
      out.状态 = s;
      out.Gread = g ? g : r.Gread;
      out.投影.reset();
      return out;
    };
    const auto mapRead = [](存在类数据状态 s) noexcept {
      switch (s) {
      case 存在类数据状态::未找到: return S::未找到;
      case 存在类数据状态::事实代次漂移: return S::事实代次漂移;
      case 存在类数据状态::数量预算不足: return S::数量预算不足;
      case 存在类数据状态::资源失败: return S::资源失败;
      default: return S::内部不一致;
      }
    };
    const auto ordinary = [&](稳定编码 id) -> std::optional<L1所有者范围节点事实> {
      const auto q = 第一层服务_.读取所有者范围当前节点(
          {L1所有者范围CRUD合同版本, id});
      const auto *n = q.事实 ? std::get_if<L1所有者范围节点事实>(&*q.事实) : nullptr;
      if (q.读取事实代次 != r.Gread)
        throw S::事实代次漂移;
      if (q.状态 == L1所有者范围读取状态::资源失败)
        throw S::资源失败;
      if (q.状态 == L1所有者范围读取状态::未找到)
        throw S::未找到;
      if (q.状态 != L1所有者范围读取状态::成功 ||
          q.合同版本 != L1所有者范围CRUD合同版本 || q.查询编码 != id ||
          !n || n->编码 != id ||
          n->写入所有者 != 所有者_ || n->种类 != 节点种类::普通 ||
          n->属性类型表示 || !n->创建事实代次)
        throw S::内部不一致;
      return *n;
    };
    const auto relation = [&](稳定编码 source, 稳定编码 type) {
      auto q = 读取当前源关系组(source, type, r.Gread);
      if (!q.成功)
        throw mapRead(q.状态);
      for (const auto &e : q.关系组)
        if (e.写入所有者 != 所有者_ || e.源节点 != source ||
            e.关系类型节点 != type || !有效(e.目标节点) ||
            !e.创建事实代次)
          throw S::内部不一致;
      return q.关系组;
    };
    const auto self = 读取当前存在本体(r.E.编码, r.Gread);
    if (!self.成功())
      return fail(mapRead(self.状态), self.事实代次);
    const auto knownEdges = relation(r.E.编码, 特征关系类型_);
    std::set<稳定编码> known;
    for (const auto &e : knownEdges) known.insert(e.目标节点);
    auto eif = relation(r.E.编码, 实例特征结构_.E到IF);
    if (eif.empty()) return fail(S::未找到);
    std::uint64_t relationCount = 0;
    const auto consumeRelations = [&](std::size_t count) noexcept {
      if (count > r.预算.最大关系数 - relationCount) return false;
      relationCount += static_cast<std::uint64_t>(count);
      return true;
    };
    if (!consumeRelations(eif.size())) return fail(S::数量预算不足);
    std::optional<L1所有者范围关系事实> selectedEif;
    std::optional<L1所有者范围节点事实> ifNode;
    std::vector<L1所有者范围关系事实> ifF;
    std::vector<特征信息身份> fMembers;
    for (const auto &candidateEif : eif) {
      auto candidateIf = ordinary(candidateEif.目标节点);
      auto candidateF = relation(candidateIf->编码, 实例特征结构_.IF到F);
      if (candidateF.empty()) return fail(S::内部不一致);
      if (!consumeRelations(candidateF.size())) return fail(S::数量预算不足);
      std::optional<特征类型身份> candidateFt;
      std::vector<特征信息身份> candidateMembers;
      candidateMembers.reserve(candidateF.size());
      for (const auto &edge : candidateF) {
        if (!known.contains(edge.目标节点)) return fail(S::引用冲突);
        const auto f = 特征服务_.读取准确特征事实(
            {1, r.Gread, {edge.目标节点}});
        if (const auto *err = std::get_if<特征数据错误>(&f)) {
          if (*err == 特征数据错误::资源失败) return fail(S::资源失败);
          return fail(S::引用冲突);
        }
        const auto &fact = std::get<准确特征读取事实>(f);
        if (fact.Gread != r.Gread ||
            fact.信息.身份.编码 != edge.目标节点 || !有效(fact.信息.类型))
          return fail(S::引用冲突);
        if (candidateFt && *candidateFt != fact.信息.类型)
          return fail(S::内部不一致);
        candidateFt = fact.信息.类型;
        candidateMembers.push_back({edge.目标节点});
      }
      if (!candidateFt) return fail(S::内部不一致);
      std::sort(candidateMembers.begin(), candidateMembers.end(),
                [](auto a, auto b) { return a.编码 < b.编码; });
      if (std::adjacent_find(candidateMembers.begin(), candidateMembers.end(),
                             [](auto a, auto b) { return a.编码 == b.编码; }) !=
          candidateMembers.end())
        return fail(S::内部不一致);
      if (*candidateFt != r.FT) continue;
      if (selectedEif) return fail(S::内部不一致);
      if (candidateF.size() > r.预算.最大F成员数)
        return fail(S::数量预算不足);
      selectedEif = candidateEif;
      ifNode = std::move(candidateIf);
      ifF = std::move(candidateF);
      fMembers = std::move(candidateMembers);
    }
    if (!selectedEif) return fail(S::未找到);
    std::sort(fMembers.begin(), fMembers.end(), [](auto a, auto b) { return a.编码 < b.编码; });
    if (std::adjacent_find(fMembers.begin(), fMembers.end(),
        [](auto a, auto b) { return a.编码 == b.编码; }) != fMembers.end()) return fail(S::内部不一致);
    auto ifR = relation(ifNode->编码, 实例特征结构_.IF到R集合);
    if (ifR.size() != 1) return fail(ifR.empty() ? S::内部不一致 : S::引用冲突);
    auto setNode = ordinary(ifR.front().目标节点);
    auto setV = relation(setNode->编码, 实例特征结构_.R集合到版本);
    if (setV.size() != 1) return fail(setV.empty() ? S::内部不一致 : S::引用冲突);
    auto versionNode = ordinary(setV.front().目标节点);
    auto versionR = relation(versionNode->编码, 实例特征结构_.版本到R项);
    if (versionR.empty()) return fail(S::内部不一致);
    if (versionR.size() > r.预算.最大R项数) return fail(S::数量预算不足);
    if (!consumeRelations(ifR.size()) || !consumeRelations(setV.size()) ||
        !consumeRelations(versionR.size()))
      return fail(S::数量预算不足);
    std::vector<实例特征R项投影> items;
    items.reserve(versionR.size());
    std::set<稳定编码> allMember;
    for (const auto &rEdge : versionR) {
      auto rNode = ordinary(rEdge.目标节点);
      auto members = relation(rNode->编码, 实例特征结构_.R项到F);
      if (members.empty()) return fail(S::内部不一致);
      if (!consumeRelations(members.size())) return fail(S::数量预算不足);
      std::vector<特征信息身份> memberIds;
      memberIds.reserve(members.size());
      for (const auto &e : members) {
        if (!std::binary_search(fMembers.begin(), fMembers.end(), 特征信息身份{e.目标节点},
            [](auto a, auto b) { return a.编码 < b.编码; }) || !allMember.insert(e.目标节点).second)
          return fail(S::引用冲突);
        memberIds.push_back({e.目标节点});
      }
      std::sort(memberIds.begin(), memberIds.end(), [](auto a, auto b) { return a.编码 < b.编码; });
      if (std::adjacent_find(memberIds.begin(), memberIds.end(),
          [](auto a, auto b) { return a.编码 == b.编码; }) != memberIds.end()) return fail(S::内部不一致);
      const auto values = 第一层服务_.读取所有者范围来源当前完整值组(
          {L1所有者范围来源当前完整值组读取合同版本, 所有者_,
           rNode->编码, r.Gread});
      if (values.读取事实代次 != r.Gread) return fail(S::事实代次漂移, values.读取事实代次);
      if (values.状态 == L1所有者范围来源当前完整值组读取状态_v2::资源失败)
        return fail(S::资源失败);
      if (values.状态 != L1所有者范围来源当前完整值组读取状态_v2::成功 ||
          values.合同版本 != L1所有者范围来源当前完整值组读取合同版本 ||
          values.所有者 != 所有者_ || values.来源节点 != rNode->编码 ||
          values.期望事实代次 != r.Gread || values.当前值.size() != 1)
        return fail(S::内部不一致);
      const auto &value = values.当前值.front();
      const auto *raw = std::get_if<std::vector<std::uint64_t>>(&value.材料);
      if (value.写入所有者 != 所有者_ || value.所属节点 != rNode->编码 ||
          value.来源节点 != rNode->编码 || value.属性类型节点 != 实例特征结构_.R项材料属性类型 ||
          !value.创建事实代次 || !raw || raw->size() < 4 ||
          raw->size() > r.预算.最大单材料U64元素数 || (*raw)[0] != 1 ||
          ((*raw)[1] != static_cast<std::uint64_t>(特征R材料类别::I64闭区间) &&
           (*raw)[1] != static_cast<std::uint64_t>(特征R材料类别::类型规则U64组)) ||
          (*raw)[2] == 0 || (*raw)[2] != raw->size() - 3)
        return fail(S::内部不一致);
      特征R区间材料 material{static_cast<std::uint32_t>((*raw)[0]),
          static_cast<特征R材料类别>((*raw)[1]),
          std::vector<std::uint64_t>(raw->begin() + 3, raw->end())};
      items.push_back({{rNode->编码}, std::move(material), std::move(memberIds),
                       *rNode, std::move(members), value});
    }
    if (relationCount > r.预算.最大关系数 || allMember.size() != fMembers.size())
      return fail(relationCount > r.预算.最大关系数 ? S::数量预算不足 : S::引用冲突);
    std::sort(items.begin(), items.end(), [](const auto &a, const auto &b) {
      return a.R项.编码 < b.R项.编码;
    });
    实例特征IFR完整投影 p;
    p.E = r.E; p.FT = r.FT; p.IF = {{ifNode->编码}}; p.R集合 = {{setNode->编码}};
    p.版本 = {{versionNode->编码}}; p.F成员 = std::move(fMembers); p.R项 = std::move(items);
    p.结构 = 实例特征结构_;
    p.节点事实 = {*ifNode, *setNode, *versionNode};
    for (const auto &i : p.R项) p.节点事实.push_back(i.节点事实);
    p.六关系事实.push_back(*selectedEif);
    p.六关系事实.insert(p.六关系事实.end(), ifF.begin(), ifF.end());
    p.六关系事实.insert(p.六关系事实.end(), ifR.begin(), ifR.end());
    p.六关系事实.insert(p.六关系事实.end(), setV.begin(), setV.end());
    p.六关系事实.insert(p.六关系事实.end(), versionR.begin(), versionR.end());
    for (const auto &i : p.R项)
      p.六关系事实.insert(p.六关系事实.end(), i.成员关系.begin(), i.成员关系.end());
    std::sort(p.节点事实.begin(), p.节点事实.end(), [](const auto &a, const auto &b) { return a.编码 < b.编码; });
    std::sort(p.六关系事实.begin(), p.六关系事实.end(), [](const auto &a, const auto &b) { return a.编码 < b.编码; });
    out.状态 = S::已读取;
    out.投影 = std::move(p);
    return out;
  } catch (S s) {
    out.状态 = s; out.投影.reset(); return out;
  } catch (const std::bad_alloc &) {
    out.状态 = S::资源失败; out.投影.reset(); return out;
  } catch (...) {
    out.状态 = S::内部不一致; out.投影.reset(); return out;
  }
}

inline 实例特征IFR结果 存在类数据服务::提交实例特征IFR目标结构(
    const 提交实例特征IFR目标请求 &r) noexcept {
  using S = 实例特征IFR状态;
  实例特征IFR结果 out;
  out.Gread = r.G0;
  bool dispatched = false;
  bool zeroWrite = false;
  std::unique_lock<std::mutex> lock(写入锁_);
  try {
    const auto budgetValid = [](const 实例特征IFR读取预算 &b) noexcept {
      return b.最大F成员数 && b.最大R项数 && b.最大关系数 &&
             b.最大材料值数 && b.最大单材料U64元素数;
    };
    const auto materialValid = [](const 特征R区间材料 &m) noexcept {
      return m.格式版本 == 1 &&
             (m.类别 == 特征R材料类别::I64闭区间 ||
              m.类别 == 特征R材料类别::类型规则U64组) &&
             !m.规范化U64组.empty();
    };
    if (r.版本 != 1 || !有效(r.E) || !有效(r.FT) || !r.G0 ||
        !有效(r.幂等身份) || !budgetValid(r.读回预算) || r.目标R项.empty())
      return out;
    out.原请求 = r;
    if (IFR待确认_ && !IFR持久请求相同(*IFR待确认_, r)) {
      out.状态 = S::已可能发布;
      return out;
    }
    std::set<稳定编码> all;
    稳定编码 previousMinimum{};
    for (const auto &item : r.目标R项) {
      if (!materialValid(item.材料) || item.形成成员.empty()) {
        out.状态 = S::入口拒绝;
        return out;
      }
      稳定编码 prior{};
      for (const auto &f : item.形成成员) {
        if (!有效(f) || (有效(prior) && !(prior < f.编码)) || !all.insert(f.编码).second) {
          out.状态 = S::入口拒绝;
          return out;
        }
        prior = f.编码;
      }
      if (有效(previousMinimum) && !(previousMinimum < item.形成成员.front().编码)) {
        out.状态 = S::入口拒绝;
        return out;
      }
      previousMinimum = item.形成成员.front().编码;
    }
    const auto current = 读取实例特征IFR({1, r.E, r.FT, r.G0, r.读回预算});
    if (current.状态 != S::已读取 && current.状态 != S::未找到) {
      out.状态 = current.状态;
      out.Gread = current.Gread;
      return out;
    }
    if ((current.状态 == S::已读取) != r.预期快照.has_value()) {
      out.状态 = S::引用冲突;
      return out;
    }
    if (r.预期快照 && (!current.投影 || *r.预期快照 != *current.投影)) {
      out.状态 = S::引用冲突;
      return out;
    }
    const auto known = 读取当前源关系组(r.E.编码, 特征关系类型_, r.G0);
    if (!known.成功) {
      out.状态 = known.状态 == 存在类数据状态::事实代次漂移 ? S::事实代次漂移 :
                   known.状态 == 存在类数据状态::资源失败 ? S::资源失败 : S::内部不一致;
      out.Gread = known.事实代次;
      return out;
    }
    std::set<稳定编码> knownIds;
    for (const auto &e : known.关系组) knownIds.insert(e.目标节点);
    for (const auto f : all) {
      if (!knownIds.contains(f)) { out.状态 = S::引用冲突; return out; }
      const auto fact = 特征服务_.读取准确特征事实({1, r.G0, {{f}}});
      if (const auto *error = std::get_if<特征数据错误>(&fact)) {
        out.状态 = *error == 特征数据错误::资源失败 ? S::资源失败 : S::引用冲突;
        return out;
      }
      const auto &factual = std::get<准确特征读取事实>(fact);
      if (factual.Gread != r.G0 ||
          factual.信息.身份.编码 != f || factual.信息.类型 != r.FT) {
        out.状态 = S::引用冲突;
        return out;
      }
    }
    const auto same = [&] {
      if (!current.投影 || current.投影->R项.size() != r.目标R项.size()) return false;
      for (std::size_t i = 0; i < r.目标R项.size(); ++i) {
        if (current.投影->R项[i].材料 != r.目标R项[i].材料 ||
            current.投影->R项[i].形成成员 != r.目标R项[i].形成成员) return false;
      }
      return true;
    }();
    if (same) {
      const L1所有者范围写集请求 zero{
          L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
      const auto first = 写入端口_.读取首次写入材料(
          {L1所有者范围首次写入读取合同版本, r.幂等身份});
      if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
          first.所有者 != 所有者_ || first.写入幂等身份 != r.幂等身份) {
        out.状态 = S::内部不一致;
        return out;
      }
      if (first.状态 == L1所有者范围读取状态::成功) {
        if (!first.首次规范化写集 || *first.首次规范化写集 != zero) {
          out.状态 = S::幂等冲突;
          return out;
        }
      } else if (first.状态 != L1所有者范围读取状态::未找到) {
        out.状态 = first.状态 == L1所有者范围读取状态::资源失败
            ? S::资源失败 : S::内部不一致;
        return out;
      }
      out.状态 = S::无须变更;
      out.Gread = current.Gread;
      out.投影 = current.投影;
      return out;
    }
    std::set<稳定编码> oldMembers;
    if (current.投影)
      for (const auto &f : current.投影->F成员) oldMembers.insert(f.编码);
    if (current.投影)
      for (const auto &f : current.投影->F成员)
        if (!all.contains(f.编码)) { out.状态 = S::引用冲突; return out; }
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
    std::uint32_t next = 1;
    const auto local = [&]() { return L1所有者范围写集本地键{next++}; };
    L1所有者范围事实引用 IF;
    L1所有者范围事实引用 collection;
    if (!current.投影) {
      const auto ifKey = local();
      const auto collectionKey = local();
      ws.节点.push_back({ifKey, 节点种类::普通, std::nullopt});
      ws.节点.push_back({collectionKey, 节点种类::普通, std::nullopt});
      IF = ifKey; collection = collectionKey;
      ws.关系.push_back({local(), r.E.编码, IF, 实例特征结构_.E到IF, 1});
      for (const auto f : all)
        ws.关系.push_back({local(), IF, f, 实例特征结构_.IF到F, 1});
      ws.关系.push_back({local(), IF, collection, 实例特征结构_.IF到R集合, 1});
    } else {
      IF = current.投影->IF.编码;
      collection = current.投影->R集合.编码;
      for (const auto f : all)
        if (!oldMembers.contains(f))
          ws.关系.push_back({local(), IF, f, 实例特征结构_.IF到F, 1});
      std::optional<稳定编码> oldVersionRelation;
      for (const auto &edge : current.投影->六关系事实)
        if (edge.源节点 == current.投影->R集合.编码 &&
            edge.目标节点 == current.投影->版本.编码 &&
            edge.关系类型节点 == 实例特征结构_.R集合到版本)
          oldVersionRelation = edge.编码;
      if (!oldVersionRelation) { out.状态 = S::内部不一致; return out; }
      ws.退出事实.push_back(*oldVersionRelation);
    }
    const auto versionKey = local();
    ws.节点.push_back({versionKey, 节点种类::普通, std::nullopt});
    ws.关系.push_back({local(), collection, versionKey, 实例特征结构_.R集合到版本, 1});
    for (const auto &item : r.目标R项) {
      const auto itemKey = local();
      ws.节点.push_back({itemKey, 节点种类::普通, std::nullopt});
      ws.关系.push_back({local(), versionKey, itemKey, 实例特征结构_.版本到R项, 1});
      for (const auto f : item.形成成员)
        ws.关系.push_back({local(), itemKey, f.编码, 实例特征结构_.R项到F, 1});
      std::vector<std::uint64_t> encoded;
      encoded.reserve(item.材料.规范化U64组.size() + 3);
      encoded.push_back(item.材料.格式版本);
      encoded.push_back(static_cast<std::uint64_t>(item.材料.类别));
      encoded.push_back(item.材料.规范化U64组.size());
      encoded.insert(encoded.end(), item.材料.规范化U64组.begin(), item.材料.规范化U64组.end());
      ws.值.push_back({local(), itemKey, 实例特征结构_.R项材料属性类型,
                       std::move(encoded), itemKey});
    }
    const auto first = 写入端口_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != 所有者_ || first.写入幂等身份 != r.幂等身份) {
      out.状态 = S::内部不一致;
      return out;
    }
    const bool replay = first.状态 == L1所有者范围读取状态::成功;
    if (replay) {
      if (!first.首次规范化写集 || *first.首次规范化写集 != ws || !first.首次写入结果) {
        out.状态 = S::幂等冲突;
        return out;
      }
    } else if (first.状态 == L1所有者范围读取状态::未找到) {
      if (first.读取事实代次 != r.G0) { out.状态 = S::事实代次漂移; out.Gread = first.读取事实代次; return out; }
    } else {
      out.状态 = first.状态 == L1所有者范围读取状态::资源失败 ? S::资源失败 : S::内部不一致;
      return out;
    }
    dispatched = true;
    IFR待确认_ = r;
    const auto saved = 写入端口_.提交所有者范围中性写集(ws);
    out.Gread = saved.事实代次;
    const auto expected = replay ? L1所有者范围写入状态::精确重复 : L1所有者范围写入状态::成功;
    if (saved.合同版本 != L1所有者范围CRUD合同版本 || saved.所有者 != 所有者_ ||
        saved.写入幂等身份 != r.幂等身份 || saved.状态 != expected ||
        saved.新编码映射.size() != ws.节点.size() + ws.关系.size() + ws.值.size()) {
      if (saved.状态 == L1所有者范围写入状态::事实代次漂移) { zeroWrite = true; IFR待确认_.reset(); out.状态 = S::事实代次漂移; return out; }
      if (saved.状态 == L1所有者范围写入状态::幂等冲突) { zeroWrite = true; IFR待确认_.reset(); out.状态 = S::幂等冲突; return out; }
      out.状态 = S::已可能发布;
      return out;
    }
    const auto read = 读取实例特征IFR({1, r.E, r.FT, saved.事实代次, r.读回预算});
    if (read.状态 != S::已读取 || !read.投影) {
      out.状态 = S::已可能发布;
      return out;
    }
    out.状态 = replay ? S::精确重复 : S::已发布;
    out.Gread = read.Gread;
    out.首次发布代次 = saved.事实代次;
    out.投影 = read.投影;
    IFR待确认_.reset();
    return out;
  } catch (const std::bad_alloc &) {
    out.状态 = dispatched && !zeroWrite ? S::已可能发布 : S::资源失败;
  } catch (...) {
    out.状态 = dispatched && !zeroWrite ? S::已可能发布 : S::内部不一致;
  }
  if (out.状态 == S::已可能发布) {
    if (!IFR待确认_) try { IFR待确认_ = r; } catch (...) {}
  } else if (IFR待确认_ && IFR持久请求相同(*IFR待确认_, r)) {
    IFR待确认_.reset();
  }
  out.投影.reset();
  return out;
}
} // namespace 海中鱼巣
