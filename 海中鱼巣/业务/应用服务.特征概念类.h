#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../领域/数据服务.特征类.h"
#include "../领域/数据服务.概念树类.h"
#include "../领域/数据服务.存在类.h"

namespace 海中鱼巣 {

struct I64单值出生概念确保请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  特征类型身份 FT;
  std::int64_t 准确I64 = 0;
  L1所有者范围写入幂等身份 概念键;
  概念初始组织指定 初始组织 = 概念初始组织指定::未指定;
  std::vector<概念树概念身份> 直接上位;
  概念树预算 概念预算;
  friend bool operator==(const I64单值出生概念确保请求&,const I64单值出生概念确保请求&)=default;
};
enum class I64单值出生概念确保状态 : std::uint8_t {
  已建立, 已复用, 精确重复, 规则缺失, 类型不相容, 入口拒绝,
  事实代次漂移, 幂等冲突, 历史材料不可用, 资源失败, 内部不一致, 已可能发布
};
struct I64单值出生概念确保结果 final {
  std::uint32_t 版本 = 1;
  I64单值出生概念确保状态 状态 = I64单值出生概念确保状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次H;
  std::optional<纯概念事实> FCv;
  I64单值出生概念确保请求 原请求;
  bool 成功() const noexcept;
};

struct I64原子准确特征出生应用请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  定位特征位置 位置;
  特征类型身份 FT;
  std::int64_t 准确I64 = 0;
  L1所有者范围写入幂等身份 FCv概念键;
  原子I64特征出生键 F出生键;
  原子I64特征候选读取预算 候选读取预算;
  原子I64特征组织读取预算 组织读取预算;
  原子I64特征概念读取预算 出生概念读取预算;
  概念树预算 FCv概念预算;
  friend bool operator==(const I64原子准确特征出生应用请求&,const I64原子准确特征出生应用请求&)=default;
};
enum class I64原子准确特征出生应用状态 : std::uint8_t {
  已创建, 已复用, 精确重复, 需选择F,
  FCv确保失败, 候选查询失败, 候选读取失败, 出生失败, 入口拒绝
};
struct I64原子准确特征出生应用结果 final {
  std::uint32_t 版本 = 1;
  I64原子准确特征出生应用状态 状态 = I64原子准确特征出生应用状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<I64单值出生概念确保结果> FCv确保;
  std::optional<原子I64特征候选查询结果> 候选查询;
  std::vector<原子I64特征出生读取结果> 候选读取组;
  std::optional<原子I64特征出生结果> 出生;
  std::optional<原子I64特征出生事实> 事实;
  I64原子准确特征出生应用请求 原请求;
  bool 成功() const noexcept;
};

enum class 实例特征结构异常 : std::uint8_t {
  半结构, 空R项, 缺材料, 重复R项, 多重命中
};
using 实例特征结构诊断 = void(*)(存在信息身份, 特征类型身份,
    实例特征结构异常, std::uint64_t, std::uint64_t) noexcept;

struct 实例特征R观察请求 final {
  std::uint32_t 版本 = 1;
  存在信息身份 E;
  定位特征位置 位置;
  特征类型身份 FT;
  特征准确值 候选值;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 IFR键;
  原子I64特征出生键 F出生键;
  L1所有者范围写入幂等身份 FCv概念键;
  实例特征IFR读取预算 IFR预算;
  特征R规则读取预算 R规则预算;
  原子I64特征候选读取预算 候选预算;
  原子I64特征组织读取预算 组织预算;
  原子I64特征概念读取预算 概念读取预算;
  概念树预算 概念预算;
  friend bool operator==(const 实例特征R观察请求&,
                         const 实例特征R观察请求&) = default;
};

enum class 实例特征R观察状态 : std::uint8_t {
  已发布IFR, IFR未变更, 读取失败, 结构异常, 比较未启用,
  归组失败, F出生失败, IFR写入失败, 入口拒绝
};

struct 实例特征R观察结果 final {
  std::uint32_t 版本 = 1;
  实例特征R观察状态 状态 = 实例特征R观察状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<实例特征IFR结果> IFR;
  std::optional<I64原子准确特征出生应用结果> F出生;
  friend bool operator==(const 实例特征R观察结果&,
                         const 实例特征R观察结果&) = default;
};

class 特征概念应用服务 final {
  特征类数据服务& 特征服务_;
  概念树类数据服务& 概念服务_;
  原子I64特征出生数据服务& 原子服务_;
  存在类数据服务& 存在服务_;
  实例特征结构诊断 结构诊断_ = nullptr;

  static bool 请求有效(const I64原子准确特征出生应用请求&) noexcept;
  static bool 请求有效(const 实例特征R观察请求&) noexcept;
  static bool 概念请求有效(const I64单值出生概念确保请求&) noexcept;
  static I64单值出生概念确保状态 映射概念状态(纯概念状态) noexcept;
  static I64原子准确特征出生应用状态 映射出生状态(原子I64特征出生状态) noexcept;
  I64单值出生概念确保结果 确保FCv(const I64原子准确特征出生应用请求&);
  std::vector<概念树概念身份> 计算直接上位(const 特征规范I64域&,
      const I64特征概念组织读取结果&, const 特征规范I64域&);

public:
  特征概念应用服务(特征类数据服务&, 概念树类数据服务&,
      原子I64特征出生数据服务&, 存在类数据服务&,
      实例特征结构诊断) noexcept;
  I64原子准确特征出生应用结果 处理I64原子准确特征出生(
      const I64原子准确特征出生应用请求&);
  实例特征R观察结果 处理实例特征R观察(
      const 实例特征R观察请求&) noexcept;
  bool 使用概念服务(const 概念树类数据服务& x) const noexcept { return &概念服务_==&x; }
  bool 使用原子出生服务(const 原子I64特征出生数据服务& x) const noexcept { return &原子服务_==&x; }
  bool 使用存在服务(const 存在类数据服务& x) const noexcept { return &存在服务_==&x; }
};

} // namespace 海中鱼巣
