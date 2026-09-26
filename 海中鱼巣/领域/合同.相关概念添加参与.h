#pragma once

#include <algorithm>
#include <bit>
#include <array>
#include <limits>
#include <set>
#include <cstdint>
#include <optional>
#include <span>
#include <variant>
#include <vector>

#include "../核心/服务.L1事实基座.h"


namespace 海中鱼巣 {

struct 概念树概念身份 final {
  稳定编码 值{};
  概念树概念身份() = default;
  explicit 概念树概念身份(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树概念身份 &,
                         const 概念树概念身份 &) = default;
};

struct 概念树存在引用 final {
  稳定编码 值{};
  概念树存在引用() = default;
  explicit 概念树存在引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树存在引用 &,
                         const 概念树存在引用 &) = default;
};

struct 概念树特征类型引用 final {
  稳定编码 值{};
  概念树特征类型引用() = default;
  explicit 概念树特征类型引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树特征类型引用 &,
                         const 概念树特征类型引用 &) = default;
};

struct 概念树特征引用 final {
  稳定编码 值{};
  概念树特征引用() = default;
  explicit 概念树特征引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树特征引用 &,
                         const 概念树特征引用 &) = default;
};

struct 概念树场景引用 final {
  稳定编码 值{};
  概念树场景引用() = default;
  explicit 概念树场景引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树场景引用 &,
                         const 概念树场景引用 &) = default;
};

using 概念树世界引用 = std::variant<概念树存在引用, 概念树特征引用>;
using 概念树形成世界引用 =
    std::variant<概念树存在引用, 概念树特征引用,
                 概念树特征类型引用, 概念树场景引用>;
using 概念树精确值 =
    std::variant<std::int64_t, std::vector<std::int64_t>,
                 std::vector<std::uint64_t>>;

struct 概念树I64区间 final {
  std::int64_t 下界 = 0;
  std::int64_t 上界 = 0;
  friend bool operator==(const 概念树I64区间 &,
                         const 概念树I64区间 &) = default;
};

using 概念树特征值域 = std::variant<概念树精确值, 概念树I64区间>;

struct 概念树特征定义 final {
  概念树存在引用 形成宿主;
  概念树特征类型引用 特征类型;
  概念树特征值域 值域;
  friend bool operator==(const 概念树特征定义 &,
                         const 概念树特征定义 &) = default;
};

struct 概念树存在定义 final {
  std::vector<概念树概念身份> 特征模板组;
  friend bool operator==(const 概念树存在定义 &,
                         const 概念树存在定义 &) = default;
};

using 概念树定义 = std::variant<概念树特征定义, 概念树存在定义>;

struct 概念树来源项 final {
  概念树世界引用 世界事实;
  friend bool operator==(const 概念树来源项 &,
                         const 概念树来源项 &) = default;
};

struct 概念树预算 final {
  std::uint64_t 最大概念数 = 0;
  std::uint64_t 最大关系数 = 0;
  std::uint64_t 最大来源数 = 0;
  std::uint64_t 最大支持数 = 0;
  std::uint64_t 最大世界成员数 = 0;
  std::uint64_t 最大特征属性数 = 0;
  std::uint64_t 最大动态槽数 = 0;
  std::uint64_t 最大动态模板数 = 0;
  friend bool operator==(const 概念树预算 &,
                         const 概念树预算 &) = default;
};

struct 概念树生命周期 final {
  std::uint64_t 创建事实代次 = 0;
  friend bool operator==(const 概念树生命周期 &,
                         const 概念树生命周期 &) = default;
};

struct 概念树直接上位事实 final {
  稳定编码 关系{};
  概念树概念身份 上位;
  概念树概念身份 下位;
  概念树生命周期 生命周期;
  friend bool operator==(const 概念树直接上位事实 &,
                         const 概念树直接上位事实 &) = default;
};

struct 概念树形成引用事实 final {
  稳定编码 关系{};
  稳定编码 记录{};
  概念树概念身份 所属概念;
  概念树形成世界引用 世界引用;
  概念树生命周期 关系生命周期;
  friend bool operator==(const 概念树形成引用事实 &,
                         const 概念树形成引用事实 &) = default;
};

enum class 相关概念类别 : std::uint8_t { 存在 = 1, 特征 = 2 };

enum class 概念树生命周期状态 : std::uint8_t { 活跃=1, 冷却=2, 退役=3 };
enum class 纯概念状态 : std::uint8_t {
  已读取=1, 已创建=2, 精确重复=3, 已删除=4, 未找到=5,
  入口拒绝=6, 类别冲突=7, 定义不支持=8, 定义不相容=9,
  组织冲突=10, 上位成环=11, 概念已退役=12,
  引用冲突=14, 事实代次漂移=15, 幂等冲突=16, 数量预算不足=17,
  资源失败=19, 内部不一致=20,
  已可能发布=21, 旧格式不支持=22, 已迁移生命周期=23
};

enum class 通用存在定义规则 : std::uint8_t { 未指定 = 0, 不预设特征 = 1 };
struct 通用存在概念定义 final {
  std::uint32_t 规则版本 = 0;
  通用存在定义规则 规则 = 通用存在定义规则::未指定;
  friend bool operator==(const 通用存在概念定义&, const 通用存在概念定义&) = default;
};
struct 纯I64特征概念定义 final {
  概念树特征类型引用 特征类型;
  std::vector<概念树I64区间> 规范域;
  friend bool operator==(const 纯I64特征概念定义&, const 纯I64特征概念定义&) = default;
};
struct 纯合取存在概念定义 final {
  std::vector<概念树概念身份> 特征模板组;
  friend bool operator==(const 纯合取存在概念定义&, const 纯合取存在概念定义&) = default;
};

// 公共概念结构的规则身份。该纯值只承载稳定编码；构造和比较均不访问仓库，
// 也不证明对应规则已经发布、归属正确或仍处于合法生命周期。
struct 概念树规则身份 final {
  稳定编码 值{};
  概念树规则身份() = default;
  explicit 概念树规则身份(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树规则身份 &,
                         const 概念树规则身份 &) = default;
};

struct 概念树读取头 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0;
  friend bool operator==(const 概念树读取头 &,
                         const 概念树读取头 &) = default;
};

struct 概念树写入头 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t 期望事实代次 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  friend bool operator==(const 概念树写入头 &,
                         const 概念树写入头 &) = default;
};
using 纯概念定义 = std::variant<纯I64特征概念定义,
                                纯合取存在概念定义, 通用存在概念定义>;
enum class 概念初始组织指定 : std::uint8_t { 未指定 = 0, 显式顶层 = 1, 具名上位 = 2 };
struct 纯概念读取请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0;
  概念树概念身份 概念;
  概念树预算 预算;
};
struct 纯概念完整读取请求_v3 final {
  std::uint32_t 版本 = 3;
  std::uint64_t Gread = 0;
  概念树概念身份 概念;
  friend bool operator==(const 纯概念完整读取请求_v3 &,
                         const 纯概念完整读取请求_v3 &) = default;
};
struct 纯概念查询请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0;
  纯概念定义 定义;
  概念树预算 预算;
};
struct 纯概念创建请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  纯概念定义 定义;
  概念初始组织指定 组织 = 概念初始组织指定::未指定;
  std::vector<概念树概念身份> 直接上位;
  概念树预算 预算;
};

struct 存在概念使用事实 final {
  稳定编码 关系{}, E{}; 概念树概念身份 EC;
  概念树生命周期 生命周期;
  friend bool operator==(const 存在概念使用事实 &,
                         const 存在概念使用事实 &) = default;
};

enum class 纯概念发布状态 : std::uint8_t {
  未进入=0, 确认未发布=1, 确认发布=2, 可能发布=3
};
enum class 纯概念定义关系种类 : std::uint8_t {
  未指定=0, 定义成员=1, 定义特征类型=2, 定义模板=3
};
struct 纯概念定义关系事实 final {
  稳定编码 关系{},源{},目标{},关系类型{};
  纯概念定义关系种类 种类=纯概念定义关系种类::未指定;
  std::uint64_t 顺序=0; 概念树生命周期 生命周期;
};
struct 纯概念事实 final {
  概念树概念身份 概念; 相关概念类别 类别{}; 纯概念定义 定义;
  稳定编码 定义记录{}; 概念树生命周期 定义记录生命周期;
  std::vector<纯概念定义关系事实> 定义关系组;
  std::vector<概念树直接上位事实> 直接上位;
  概念树生命周期状态 治理状态{};
  稳定编码 生命周期值事实{}; 概念树生命周期 生命周期值生命周期;
  概念树生命周期 生命周期;
};
struct I64特征概念组织读取请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0;
  特征类型身份 FT;
  概念树预算 预算;
};
struct 纯概念查询请求_v3 final {
  std::uint32_t 版本 = 3;
  std::uint64_t Gread = 0;
  纯概念定义 定义;
};
struct 纯概念创建请求_v3 final {
  std::uint32_t 版本 = 3;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  纯概念定义 定义;
  概念初始组织指定 组织 = 概念初始组织指定::未指定;
  std::vector<概念树概念身份> 直接上位;
};
struct I64特征概念组织读取结果 final {
  std::uint32_t 版本 = 2;
  纯概念状态 状态 = 纯概念状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::vector<纯概念事实> 概念组;
  bool 成功(const I64特征概念组织读取请求 &) const noexcept;
};
struct 纯概念结构类型_v2 final {
  稳定编码 类型登记{},概念族成员{},格式版本{},概念类别{},定义成员{},定义种类{};
  稳定编码 定义特征类型{},定义模板{},I64域{},通用规则{},直接上位{},生命周期{},存在概念使用{};
};
struct 纯概念结构交付_v2 final {
  std::uint32_t 版本=2; 稳定编码 格式锚点{},概念族锚点{};
  纯概念结构类型_v2 类型;
};
struct 纯概念结构登记请求_v2 final {
  std::uint32_t 版本=2; std::uint64_t G0=0;
  L1所有者范围写入幂等身份 幂等身份;
  std::uint64_t 最大空域核验事实数=0;
};
struct 纯概念结构登记结果_v2 final {
  std::uint32_t 版本=2; 纯概念状态 状态=纯概念状态::入口拒绝;
  纯概念发布状态 发布=纯概念发布状态::未进入;
  std::uint64_t Gread=0; std::optional<std::uint64_t> 首次发布H;
  std::optional<纯概念结构登记请求_v2> 原请求;
  std::optional<纯概念结构交付_v2> 交付;
  bool 成功(const 纯概念结构登记请求_v2&) const noexcept;
};

// 存在概念不再借用纯概念的定义 variant。它只保存调用方已经裁决的两组引用。
struct 存在概念特征值域项_v3 final {
  特征类型身份 FT{};
  概念树概念身份 FC{};
  friend bool operator==(const 存在概念特征值域项_v3&,
                         const 存在概念特征值域项_v3&) = default;
};
struct 存在概念两组定义_v3 final {
  bool 自身特征组已完整声明 = false;
  std::vector<存在概念特征值域项_v3> 自身特征值域组;
  bool 子存在概念组已完整声明 = false;
  std::vector<概念树概念身份> 已知子存在概念组;
  friend bool operator==(const 存在概念两组定义_v3&,
                         const 存在概念两组定义_v3&) = default;
};
struct 存在概念两组预算_v3 final {
  概念树预算 基础;
  std::uint64_t 最大自身特征项 = 0;
  std::uint64_t 最大子概念项 = 0;
  std::uint64_t 最大候选数 = 0;
  std::uint64_t 最大定义成员数 = 0;
  std::uint64_t 最大首次材料项数 = 0;
  friend bool operator==(const 存在概念两组预算_v3&,
                         const 存在概念两组预算_v3&) = default;
};
enum class 存在概念两组状态_v3 : std::uint8_t {
  入口拒绝=1, 已规范化, 已枚举, 已读取, 未找到, 已创建, 精确重复,
  概念已退役=9, 类别冲突, 定义不相容, 规则缺失,
  事实代次漂移, 幂等冲突, 数量预算不足, 资源失败=17,
  内部不一致, 已可能发布, 旧格式不支持
};
struct 存在概念两组结构类型_v1 final {
  稳定编码 两组定义成员{}, 自身特征值域项{}, 自身项特征类型{},
           自身项值域概念{}, 已知子存在概念{};
  friend bool operator==(const 存在概念两组结构类型_v1&,
                         const 存在概念两组结构类型_v1&) = default;
};
struct 存在概念两组结构交付_v1 final {
  std::uint32_t 版本 = 1;
  稳定编码 格式锚点{};
  存在概念两组结构类型_v1 类型;
  friend bool operator==(const 存在概念两组结构交付_v1&,
                         const 存在概念两组结构交付_v1&) = default;
};
struct 存在概念两组结构登记请求_v1 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  纯概念结构交付_v2 纯概念结构;
  std::uint64_t 最大首次材料项数 = 0;
  friend bool operator==(const 存在概念两组结构登记请求_v1&,
                         const 存在概念两组结构登记请求_v1&) = default;
};
struct 存在概念两组结构首次材料_v1 final {
  std::array<L1所有者范围节点事实, 6> 节点;
  std::array<L1所有者范围关系事实, 5> 类型登记关系;
  L1所有者范围值事实 格式值;
  friend bool operator==(const 存在概念两组结构首次材料_v1&,
                         const 存在概念两组结构首次材料_v1&) = default;
};
struct 存在概念两组结构登记结果_v1 final {
  std::uint32_t 版本 = 1;
  存在概念两组状态_v3 状态 = 存在概念两组状态_v3::入口拒绝;
  纯概念发布状态 发布 = 纯概念发布状态::未进入;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布H;
  std::optional<存在概念两组结构登记请求_v1> 原请求;
  std::optional<存在概念两组结构交付_v1> 交付;
  std::optional<存在概念两组结构首次材料_v1> 首次材料;
  bool 成功(const 存在概念两组结构登记请求_v1&) const noexcept;
};
struct 存在概念两组定义关系事实_v1 final {
  稳定编码 关系{},源{},目标{},关系类型{};
  std::uint64_t 顺序=0;
  概念树生命周期 生命周期;
  friend bool operator==(const 存在概念两组定义关系事实_v1&,
                         const 存在概念两组定义关系事实_v1&) = default;
};
struct 存在概念两组事实_v3 final {
  概念树概念身份 概念;
  稳定编码 定义记录{};
  概念树生命周期 定义记录生命周期;
  概念树生命周期 状态生命周期;
  概念树生命周期 概念生命周期;
  概念树生命周期状态 治理状态{};
  存在概念两组定义_v3 定义;
  std::vector<存在概念两组定义关系事实_v1> 自身特征项关系组;
  std::vector<存在概念两组定义关系事实_v1> 子概念关系组;
  std::vector<概念树直接上位事实> 直接上位;
  friend bool operator==(const 存在概念两组事实_v3&,
                         const 存在概念两组事实_v3&) = default;
};
struct 存在概念两组规范化请求_v1 final {
  std::uint32_t 版本 = 1; std::uint64_t Gread = 0;
  存在概念两组定义_v3 定义; 存在概念两组预算_v3 预算;
};
struct 存在概念两组枚举请求_v1 final {
  std::uint32_t 版本 = 1; std::uint64_t Gread = 0;
  存在概念两组预算_v3 预算;
};
struct 存在概念两组读取请求_v1 final {
  std::uint32_t 版本 = 1; std::uint64_t Gread = 0;
  概念树概念身份 EC; 存在概念两组预算_v3 预算;
};
enum class 存在概念两组完整读取状态_v2 : std::uint8_t {
  已读取 = 1,
  入口拒绝,
  未找到,
  概念已退役 = 5,
  类别冲突,
  定义不相容,
  规则缺失,
  未实现,
  事实代次漂移,
  资源失败=12,
  内部不一致,
  旧格式不支持
};
struct 存在概念两组完整读取请求_v2 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0;
  概念树概念身份 EC;
  friend bool operator==(const 存在概念两组完整读取请求_v2 &,
                         const 存在概念两组完整读取请求_v2 &) = default;
};
struct 存在概念两组查询请求_v1 final {
  std::uint32_t 版本 = 1; std::uint64_t Gread = 0;
  存在概念两组定义_v3 定义; 存在概念两组预算_v3 预算;
};
struct 存在概念两组创建请求_v1 final {
  std::uint32_t 版本 = 1; std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  存在概念两组定义_v3 定义;
  std::vector<概念树概念身份> 直接上位;
  存在概念两组预算_v3 预算;
};
struct 存在概念两组规范化结果_v1 final {
  std::uint32_t 版本 = 1; 存在概念两组状态_v3 状态 = 存在概念两组状态_v3::入口拒绝;
  std::uint64_t Gread = 0; std::optional<存在概念两组定义_v3> 定义;
  bool 成功(const 存在概念两组规范化请求_v1&) const noexcept;
};
struct 存在概念两组枚举结果_v1 final {
  std::uint32_t 版本 = 1; 存在概念两组状态_v3 状态 = 存在概念两组状态_v3::入口拒绝;
  std::uint64_t Gread = 0; std::vector<存在概念两组事实_v3> 候选;
  bool 成功(const 存在概念两组枚举请求_v1&) const noexcept;
};
struct 存在概念两组读取结果_v1 final {
  std::uint32_t 版本 = 1; 存在概念两组状态_v3 状态 = 存在概念两组状态_v3::入口拒绝;
  std::uint64_t Gread = 0; std::optional<存在概念两组事实_v3> 事实;
  bool 成功(const 存在概念两组读取请求_v1&) const noexcept;
};
struct 存在概念两组完整读取结果_v2 final {
  std::uint32_t 版本 = 2;
  存在概念两组完整读取状态_v2 状态 =
      存在概念两组完整读取状态_v2::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<存在概念两组事实_v3> 事实;
  bool 成功(const 存在概念两组完整读取请求_v2 &) const noexcept;
};
struct 存在概念两组查询结果_v1 final {
  std::uint32_t 版本 = 1; 存在概念两组状态_v3 状态 = 存在概念两组状态_v3::入口拒绝;
  std::uint64_t Gread = 0; std::optional<存在概念两组事实_v3> 事实;
  bool 成功(const 存在概念两组查询请求_v1&) const noexcept;
  bool 确认未找到(const 存在概念两组查询请求_v1&) const noexcept;
};
struct 存在概念两组写入结果_v1 final {
  std::uint32_t 版本 = 1; 存在概念两组状态_v3 状态 = 存在概念两组状态_v3::入口拒绝;
  纯概念发布状态 发布 = 纯概念发布状态::未进入;
  std::uint64_t Gread = 0; std::optional<std::uint64_t> 首次发布H;
  std::optional<存在概念两组创建请求_v1> 原请求;
  std::optional<存在概念两组事实_v3> 事实;
  bool 成功(const 存在概念两组创建请求_v1&) const noexcept;
};

struct 纯概念读取结果 final {
  std::uint32_t 版本=2; 纯概念状态 状态=纯概念状态::入口拒绝;
  std::uint64_t Gread=0; std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念读取请求&) const noexcept;
};
struct 纯概念完整读取结果_v3 final {
  std::uint32_t 版本 = 3;
  纯概念状态 状态 = 纯概念状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念完整读取请求_v3 &) const noexcept;
};
struct 纯概念查询结果 final {
  std::uint32_t 版本=2; 纯概念状态 状态=纯概念状态::入口拒绝;
  std::uint64_t Gread=0; std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念查询请求&) const noexcept;
  bool 确认未找到(const 纯概念查询请求&) const noexcept;
};
struct 纯概念查询结果_v3 final {
  std::uint32_t 版本 = 3;
  纯概念状态 状态 = 纯概念状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念查询请求_v3 &) const noexcept;
  bool 确认未找到(const 纯概念查询请求_v3 &) const noexcept;
};
struct 纯概念写入结果 final {
  std::uint32_t 版本=2; 纯概念状态 状态=纯概念状态::入口拒绝;
  纯概念发布状态 发布=纯概念发布状态::未进入;
  std::uint64_t Gread=0; std::optional<std::uint64_t> 首次发布H;
  std::optional<纯概念创建请求> 原请求; std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念创建请求&) const noexcept;
};
struct 纯概念写入结果_v3 final {
  std::uint32_t 版本 = 3;
  纯概念状态 状态 = 纯概念状态::入口拒绝;
  纯概念发布状态 发布 = 纯概念发布状态::未进入;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布H;
  std::optional<纯概念创建请求_v3> 原请求;
  std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念创建请求_v3 &) const noexcept;
};
struct 纯概念创建恢复请求 final {
  std::uint32_t 版本=2; std::uint64_t Gread=0;
  L1所有者范围写入幂等身份 幂等身份;
  纯概念定义 定义;
  概念初始组织指定 组织=概念初始组织指定::未指定;
  std::vector<概念树概念身份> 直接上位; 概念树预算 预算;
};
enum class 纯概念恢复状态 : std::uint8_t {
  已读回=1, 未派发=2, 入口拒绝=3, 幂等冲突=4, 事实代次漂移=5,
  数量预算不足=6, 已可能发布=8,
  资源失败=9, 内部不一致=10, 未找到=11
};
struct 纯概念创建恢复结果 final {
  std::uint32_t 版本=2;
  纯概念恢复状态 状态=纯概念恢复状态::入口拒绝;
  std::uint64_t Gread=0;
  std::optional<std::uint64_t> 首次实际G0, 首次发布H;
  std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念创建恢复请求&) const noexcept;
};
struct 纯概念创建恢复请求_v3 final {
  std::uint32_t 版本 = 3;
  std::uint64_t Gread = 0;
  L1所有者范围写入幂等身份 幂等身份;
  纯概念定义 定义;
  概念初始组织指定 组织 = 概念初始组织指定::未指定;
  std::vector<概念树概念身份> 直接上位;
};
struct 纯概念创建恢复结果_v3 final {
  std::uint32_t 版本 = 3;
  纯概念恢复状态 状态 = 纯概念恢复状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次实际G0, 首次发布H;
  std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念创建恢复请求_v3 &) const noexcept;
};
struct 纯概念生命周期请求 final {
  std::uint32_t 版本=2; std::uint64_t G0=0;
  L1所有者范围写入幂等身份 幂等身份; 概念树概念身份 概念;
  稳定编码 预期生命周期值事实{};
  概念树生命周期状态 预期治理状态{},目标治理状态{}; 概念树预算 预算;
};
struct 纯概念生命周期结果 final {
  std::uint32_t 版本=2; 纯概念状态 状态=纯概念状态::入口拒绝;
  纯概念发布状态 发布=纯概念发布状态::未进入;
  std::uint64_t Gread=0; std::optional<std::uint64_t> 首次发布H;
  std::optional<纯概念生命周期请求> 原请求; std::optional<纯概念事实> 事实;
  bool 成功(const 纯概念生命周期请求&) const noexcept;
};
struct 纯概念退出请求 final {
  std::uint32_t 版本=2; std::uint64_t G0=0;
  L1所有者范围写入幂等身份 幂等身份; 概念树概念身份 概念;
  稳定编码 预期生命周期值事实{}; 概念树预算 预算;
};
struct 纯概念退出结果 final {
  std::uint32_t 版本=2; 纯概念状态 状态=纯概念状态::入口拒绝;
  纯概念发布状态 发布=纯概念发布状态::未进入;
  std::uint64_t Gread=0; std::optional<std::uint64_t> 首次发布H;
  std::optional<纯概念退出请求> 原请求;
  bool 成功(const 纯概念退出请求&) const noexcept;
};
struct 存在概念使用读取请求 final {
  std::uint32_t 版本=2; std::uint64_t Gread=0;
  概念树存在引用 E; 概念树预算 预算;
};
struct 存在概念使用读取结果 final {
  std::uint32_t 版本=2; 纯概念状态 状态=纯概念状态::入口拒绝;
  std::uint64_t Gread=0;
  std::optional<存在概念使用事实> 使用; std::optional<纯概念事实> 概念;
  bool 成功(const 存在概念使用读取请求&) const noexcept;
};

inline constexpr std::uint32_t 存在概念使用当前完整读取合同版本_v3 = 3;
enum class 存在概念使用当前完整读取状态_v3 : std::uint8_t {
  已读取=1, 入口拒绝=2, 未找到=3, 类别冲突=4,
  定义不相容=5, 事实代次漂移=6, 资源失败=7, 内部不一致=8
};
struct 存在概念使用当前完整读取请求_v3 final {
  std::uint32_t 版本=存在概念使用当前完整读取合同版本_v3;
  std::uint64_t Gread=0;
  概念树存在引用 E;
};
struct 存在概念使用当前完整读取结果_v3 final {
  存在概念使用当前完整读取状态_v3 状态=
      存在概念使用当前完整读取状态_v3::入口拒绝;
  std::uint32_t 版本=存在概念使用当前完整读取合同版本_v3;
  std::uint64_t Gread=0;
  std::optional<存在概念使用事实> 使用;
  std::optional<纯概念事实> 概念;
};

enum class 已发布概念引用参与状态 : std::uint8_t {
  已准备=1, 精确重复=2, 入口拒绝=3, 概念未找到=4, 概念已退役=5,
  类别冲突=7, 定义不相容=8, 引用冲突=9,
  事实代次漂移=10, 幂等冲突=11, 数量预算不足=12,
  资源失败=14, 内部不一致=15,
  已可能发布=16, 旧格式不支持=17, 已读取=18, 未派发=19
};
struct 已发布存在概念引用准备请求 final {
  std::uint32_t 版本=2; std::uint64_t G0=0;
  L1所有者范围写入幂等身份 幂等身份;
  L1有限N分区原子参与者身份_v3 参与者;
  L1有限N分区原子事实引用_v3 新存在;
  概念树概念身份 EC; 纯概念定义 预期定义; 概念树预算 预算;
};
struct 已发布概念引用片段结果 final {
  已发布概念引用参与状态 状态=已发布概念引用参与状态::入口拒绝;
  std::uint64_t Gread=0;
  std::optional<L1有限N分区原子参与者写集_v3> 写集;
};
struct 已发布概念引用首次材料结果 final {
  已发布概念引用参与状态 状态=已发布概念引用参与状态::入口拒绝;
  std::uint64_t Gread=0;
  std::optional<L1所有者范围首次写入读取结果> 首次材料;
};
enum class 相关概念方案种类 : std::uint8_t {
  精确复用 = 1,
  创建并引用 = 2
};
enum class 相关概念参与状态 : std::uint8_t {
  已准备 = 1,
  已读取 = 2,
  精确重复 = 3,
  入口拒绝 = 4,
  概念未找到 = 5,
  类别冲突 = 7,
  签名冲突 = 8,
  上位成环 = 9,
  挂靠无效 = 10,
  事实代次漂移 = 11,
  幂等冲突 = 12,
  数量预算不足 = 13,
  资源失败 = 15,
  内部不一致 = 16,
  已可能发布 = 17,
  旧格式不支持 = 18
};

using 相关概念读取预算 = 概念树预算;

struct 相关概念世界挂靠 final {
  概念树形成世界引用 世界事实;
};
struct 相关概念精确复用方案 final {
  概念树概念身份 概念;
  相关概念类别 类别 = 相关概念类别::存在;
  概念树定义 预期定义;
};
struct 相关概念创建方案 final {
  相关概念类别 类别 = 相关概念类别::存在;
  概念树定义 定义;
  std::vector<概念树概念身份> 直接上位;
};
using 相关概念方案 =
    std::variant<相关概念精确复用方案, 相关概念创建方案>;
struct 相关概念参与请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  相关概念方案 方案;
  相关概念世界挂靠 挂靠;
  相关概念读取预算 预算;
};
struct 相关概念参与片段 final {
  相关概念参与状态 状态 = 相关概念参与状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<L1有限N分区原子参与者写集_v3> 写集;
};
struct 相关概念完整事实 final {
  概念树概念身份 概念;
  相关概念类别 类别 = 相关概念类别::存在;
  概念树定义 定义;
  std::vector<概念树直接上位事实> 直接上位;
  概念树形成引用事实 挂接;
  概念树生命周期 生命周期;
};
struct 相关概念参与读回 final {
  相关概念参与状态 状态 = 相关概念参与状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<相关概念完整事实> 概念;
};
struct 相关概念组合提交请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  L1所有者范围写入幂等身份 组合幂等身份;
  相关概念参与请求 概念请求;
  std::vector<L1有限N分区原子参与者写集_v3> 前序参与者写集组;
};
struct 相关概念组合提交结果 final {
  std::uint32_t 版本 = 1;
  相关概念参与状态 概念状态 = 相关概念参与状态::入口拒绝;
  std::uint64_t Gread = 0;
  bool 已进入L1 = false;
  L1有限N分区原子事务结果_v3 事务结果;
};

class 相关概念添加参与者 {
public:
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 相关概念参与片段
  准备相关概念片段(const 相关概念参与请求 &,
                     std::uint64_t Gread,
                     L1有限N分区原子参与者身份_v3) const noexcept = 0;
  virtual 相关概念组合提交结果
  提交相关概念组合事务(const 相关概念组合提交请求 &,
      std::span<L1所有者范围写端口 *const> 前序参与者端口组) noexcept = 0;
  virtual 相关概念参与读回
  读取相关概念结果(const 相关概念参与请求 &,
                     std::uint64_t Gread) const noexcept = 0;
};

struct 相关概念结构类型 final {
  稳定编码 类型登记, 概念族成员, 概念类别, 定义成员, 定义种类;
  稳定编码 定义宿主, 定义特征类型, 定义模板;
  稳定编码 精确I64, 精确I64组, 精确U64组, 区间下界, 区间上界;
  稳定编码 直接上位, 形成成员, 形成存在, 形成特征, 形成特征类型,
      形成场景;
  稳定编码 生命周期;
};
struct 相关概念结构交付 final {
  std::uint32_t 版本 = 1, 格式 = 1;
  稳定编码 格式锚点, 概念族锚点;
  相关概念结构类型 类型;
};

namespace 纯概念结果内部 {
inline bool 生命周期有效(const 概念树生命周期&x,std::uint64_t h) noexcept {
    return x.创建事实代次&&x.创建事实代次<=h;
}
inline bool 允许生命周期迁移(概念树生命周期状态 from,
                              概念树生命周期状态 to) noexcept {
    return (from==概念树生命周期状态::活跃&&to==概念树生命周期状态::冷却)||
           (from==概念树生命周期状态::冷却&&
            (to==概念树生命周期状态::活跃||to==概念树生命周期状态::退役));
}
inline bool 定义匹配(const 纯概念定义&actual,const 纯概念定义&requested) noexcept {
    try {
        纯概念定义 expected=requested;
        if(auto *fc=std::get_if<纯I64特征概念定义>(&expected)) {
            if(!有效(fc->特征类型.值)||fc->规范域.empty())return false;
            std::sort(fc->规范域.begin(),fc->规范域.end(),[](const auto&a,const auto&b){
                return a.下界<b.下界||(a.下界==b.下界&&a.上界<b.上界);
            });
            std::vector<概念树I64区间> merged;
            for(const auto&x:fc->规范域) {
                if(x.下界>x.上界)return false;
                if(merged.empty()){merged.push_back(x);continue;}
                auto&last=merged.back();
                const bool adjacent=last.上界!=std::numeric_limits<std::int64_t>::max()&&
                    x.下界==last.上界+1;
                if(x.下界<=last.上界||adjacent) {
                    if(x.上界>last.上界)last.上界=x.上界;
                } else merged.push_back(x);
            }
            fc->规范域=std::move(merged);
        } else if(auto *ec=std::get_if<纯合取存在概念定义>(&expected)) {
            if(ec->特征模板组.empty())return false;
            std::sort(ec->特征模板组.begin(),ec->特征模板组.end(),
                [](auto a,auto b){return a.值.值<b.值.值;});
            ec->特征模板组.erase(std::unique(ec->特征模板组.begin(),ec->特征模板组.end()),
                                  ec->特征模板组.end());
        } else {
            const auto&universal=std::get<通用存在概念定义>(expected);
            if(universal.规则版本!=1||universal.规则!=通用存在定义规则::不预设特征)return false;
        }
        return actual==expected;
    } catch(...) {return false;}
}
inline bool 父组匹配(const 纯概念事实&f,
                     const std::vector<概念树概念身份>&requested) noexcept {
    try {
        auto expected=requested;
        std::sort(expected.begin(),expected.end(),[](auto a,auto b){return a.值.值<b.值.值;});
        if(std::adjacent_find(expected.begin(),expected.end())!=expected.end()||
           f.直接上位.size()!=expected.size())return false;
        std::vector<概念树概念身份> actual;
        for(const auto&e:f.直接上位)actual.push_back(e.上位);
        std::sort(actual.begin(),actual.end(),[](auto a,auto b){return a.值.值<b.值.值;});
        return actual==expected;
    } catch(...) {return false;}
}
inline bool 事实完整(const 纯概念事实&f,std::uint64_t h) noexcept {
    try {
        if(!h||!有效(f.概念.值)||!有效(f.定义记录)||f.概念.值==f.定义记录||
           !有效(f.生命周期值事实)||!生命周期有效(f.生命周期,h)||
           !生命周期有效(f.定义记录生命周期,h)||
           !生命周期有效(f.生命周期值生命周期,h)||
           f.定义记录生命周期.创建事实代次!=f.生命周期.创建事实代次||
           (f.治理状态!=概念树生命周期状态::活跃&&
            f.治理状态!=概念树生命周期状态::冷却&&
            f.治理状态!=概念树生命周期状态::退役))return false;
        const auto category=std::holds_alternative<纯I64特征概念定义>(f.定义)?
            相关概念类别::特征:相关概念类别::存在;
        if(f.类别!=category||f.定义关系组.empty())return false;
        std::set<std::uint64_t> relationIds;
        for(const auto&e:f.定义关系组)
            if(!有效(e.关系)||!有效(e.源)||!有效(e.目标)||!有效(e.关系类型)||
               !relationIds.insert(e.关系.值).second||!生命周期有效(e.生命周期,h)||
               e.生命周期.创建事实代次!=f.生命周期.创建事实代次)return false;
        const auto&member=f.定义关系组.front();
        if(member.种类!=纯概念定义关系种类::定义成员||member.顺序!=1||
           member.源!=f.概念.值||member.目标!=f.定义记录)return false;
        if(const auto*fc=std::get_if<纯I64特征概念定义>(&f.定义)) {
            if(f.定义关系组.size()!=2||fc->规范域.empty())return false;
            const auto&e=f.定义关系组[1];
            if(e.种类!=纯概念定义关系种类::定义特征类型||e.顺序!=1||
               e.源!=f.定义记录||e.目标!=fc->特征类型.值)return false;
        } else if(const auto*ec=std::get_if<纯合取存在概念定义>(&f.定义)) {
            if(ec->特征模板组.empty()||f.定义关系组.size()!=ec->特征模板组.size()+1)return false;
            for(std::size_t i=0;i<ec->特征模板组.size();++i) {
                const auto&e=f.定义关系组[i+1];
                if(e.种类!=纯概念定义关系种类::定义模板||e.顺序!=i+1||
                   e.源!=f.定义记录||e.目标!=ec->特征模板组[i].值)return false;
            }
        } else {
            const auto&universal=std::get<通用存在概念定义>(f.定义);
            if(universal.规则版本!=1||universal.规则!=通用存在定义规则::不预设特征||
               f.定义关系组.size()!=1)return false;
        }
        std::set<std::uint64_t> parentIds;
        for(const auto&e:f.直接上位)
            if(!有效(e.关系)||!有效(e.上位.值)||e.下位!=f.概念||e.上位==f.概念||
               !parentIds.insert(e.上位.值.值).second||!relationIds.insert(e.关系.值).second||
               !生命周期有效(e.生命周期,h))return false;
        return true;
    } catch(...) {return false;}
}
}

inline bool 纯概念结构登记结果_v2::成功(const 纯概念结构登记请求_v2&r) const noexcept {
    if(!(版本==2&&r.版本==2&&(状态==纯概念状态::已创建||状态==纯概念状态::精确重复)&&
        发布==纯概念发布状态::确认发布&&Gread&&首次发布H&&*首次发布H==r.G0+1&&
        *首次发布H<=Gread&&原请求&&原请求->版本==r.版本&&原请求->G0==r.G0&&
        原请求->幂等身份==r.幂等身份&&
        原请求->最大空域核验事实数==r.最大空域核验事实数&&交付&&交付->版本==2))return false;
    const auto&t=交付->类型;
    const std::array<稳定编码,15> ids{交付->格式锚点,交付->概念族锚点,t.类型登记,
        t.概念族成员,t.格式版本,t.概念类别,t.定义成员,t.定义种类,t.定义特征类型,
        t.定义模板,t.I64域,t.通用规则,t.直接上位,t.生命周期,t.存在概念使用};
    for(std::size_t i=0;i<ids.size();++i){if(!有效(ids[i]))return false;
        for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;}
    return true;
}
inline bool 纯概念读取结果::成功(const 纯概念读取请求&r) const noexcept {
    return 版本==2&&r.版本==2&&r.Gread&&有效(r.概念.值)&&
        状态==纯概念状态::已读取&&Gread==r.Gread&&事实&&
        事实->概念==r.概念&&纯概念结果内部::事实完整(*事实,Gread);
}
inline bool 纯概念完整读取结果_v3::成功(
    const 纯概念完整读取请求_v3 &r) const noexcept {
    return 版本 == 3 && r.版本 == 3 && r.Gread &&
           有效(r.概念.值) && 状态 == 纯概念状态::已读取 &&
           Gread == r.Gread && 事实 && 事实->概念 == r.概念 &&
           纯概念结果内部::事实完整(*事实, Gread);
}
inline bool I64特征概念组织读取结果::成功(const I64特征概念组织读取请求&r) const noexcept {
    if(版本!=2||r.版本!=2||!r.Gread||!有效(r.FT.编码)||
       状态!=纯概念状态::已读取||Gread!=r.Gread)return false;
    std::uint64_t previous=0;
    for(const auto& f:概念组) {
        const auto* d=std::get_if<纯I64特征概念定义>(&f.定义);
        if(!纯概念结果内部::事实完整(f,Gread)||f.类别!=相关概念类别::特征||!d||
           d->特征类型!=概念树特征类型引用{r.FT.编码}||!f.概念.值.值||f.概念.值.值<=previous)return false;
        previous=f.概念.值.值;
    }
    return true;
}
inline bool 纯概念查询结果::成功(const 纯概念查询请求&r) const noexcept {
    return 版本==2&&r.版本==2&&r.Gread&&
        状态==纯概念状态::已读取&&Gread==r.Gread&&事实&&
        纯概念结果内部::定义匹配(事实->定义,r.定义)&&
        纯概念结果内部::事实完整(*事实,Gread);
}
inline bool 纯概念查询结果::确认未找到(const 纯概念查询请求&r) const noexcept {
    return 版本==2&&r.版本==2&&r.Gread&&
        r.预算.最大概念数&&r.预算.最大关系数&&r.预算.最大特征属性数&&
        状态==纯概念状态::未找到&&Gread==r.Gread&&!事实;
}
inline bool 纯概念查询结果_v3::成功(
    const 纯概念查询请求_v3 &r) const noexcept {
    return 版本 == 3 && r.版本 == 3 && r.Gread &&
           状态 == 纯概念状态::已读取 && Gread == r.Gread &&
           事实 && 纯概念结果内部::定义匹配(事实->定义, r.定义) &&
           纯概念结果内部::事实完整(*事实, Gread);
}
inline bool 纯概念查询结果_v3::确认未找到(
    const 纯概念查询请求_v3 &r) const noexcept {
    return 版本 == 3 && r.版本 == 3 && r.Gread &&
           状态 == 纯概念状态::未找到 && Gread == r.Gread &&
           !事实;
}
inline bool 纯概念写入结果::成功(const 纯概念创建请求&r) const noexcept {
    try {
        if(版本!=2||r.版本!=2||
           (状态!=纯概念状态::已创建&&状态!=纯概念状态::精确重复)||
           !Gread||!原请求||!事实||
           原请求->版本!=r.版本||原请求->G0!=r.G0||
           原请求->幂等身份!=r.幂等身份||原请求->定义!=r.定义||
           原请求->组织!=r.组织||原请求->直接上位!=r.直接上位||
           原请求->预算!=r.预算)
            return false;
        const bool ownWrite=发布==纯概念发布状态::确认发布&&首次发布H&&
            *首次发布H==r.G0+1&&*首次发布H<=Gread;
        const bool reused=状态==纯概念状态::精确重复&&
            发布==纯概念发布状态::确认未发布&&!首次发布H;
        if(!ownWrite&&!reused)return false;

        纯概念定义 expected=r.定义;
        if(auto *fc=std::get_if<纯I64特征概念定义>(&expected)) {
            if(!有效(fc->特征类型.值)||fc->规范域.empty())return false;
            std::sort(fc->规范域.begin(),fc->规范域.end(),[](const auto&a,const auto&b){
                return a.下界<b.下界||(a.下界==b.下界&&a.上界<b.上界);
            });
            std::vector<概念树I64区间> merged;
            for(const auto &x:fc->规范域) {
                if(x.下界>x.上界)return false;
                if(merged.empty()) { merged.push_back(x); continue; }
                auto &last=merged.back();
                const bool adjacent=last.上界!=std::numeric_limits<std::int64_t>::max()&&
                    x.下界==last.上界+1;
                if(x.下界<=last.上界||adjacent) {
                    if(x.上界>last.上界)last.上界=x.上界;
                } else merged.push_back(x);
            }
            fc->规范域=std::move(merged);
        } else if(auto *ec=std::get_if<纯合取存在概念定义>(&expected)) {
            if(ec->特征模板组.empty())return false;
            std::sort(ec->特征模板组.begin(),ec->特征模板组.end(),
                [](auto a,auto b){return a.值.值<b.值.值;});
            ec->特征模板组.erase(std::unique(ec->特征模板组.begin(),ec->特征模板组.end()),
                                  ec->特征模板组.end());
        } else {
            const auto &universal=std::get<通用存在概念定义>(expected);
            if(universal.规则版本!=1||universal.规则!=通用存在定义规则::不预设特征)return false;
        }
        if(事实->定义!=expected||!有效(事实->概念.值)||!有效(事实->定义记录)||
           事实->概念.值==事实->定义记录||
           (事实->治理状态!=概念树生命周期状态::活跃&&
            事实->治理状态!=概念树生命周期状态::冷却)||
           !有效(事实->生命周期值事实)||
           !事实->生命周期.创建事实代次||事实->生命周期.创建事实代次>Gread||
           事实->定义记录生命周期.创建事实代次!=事实->生命周期.创建事实代次||
           !事实->生命周期值生命周期.创建事实代次||
           事实->生命周期值生命周期.创建事实代次>Gread)
            return false;
        const auto expectedCategory=std::holds_alternative<纯I64特征概念定义>(expected)?
            相关概念类别::特征:相关概念类别::存在;
        if(事实->类别!=expectedCategory)return false;

        auto parents=r.直接上位;
        std::sort(parents.begin(),parents.end(),[](auto a,auto b){return a.值.值<b.值.值;});
        parents.erase(std::unique(parents.begin(),parents.end()),parents.end());
        if((r.组织==概念初始组织指定::显式顶层&&!parents.empty())||
           (r.组织==概念初始组织指定::具名上位&&parents.empty())||
           事实->直接上位.size()!=parents.size())return false;
        std::vector<概念树概念身份> actualParents;
        for(const auto &edge:事实->直接上位) {
            if(!有效(edge.关系)||!有效(edge.上位.值)||edge.下位!=事实->概念||
               edge.上位==事实->概念||edge.生命周期.创建事实代次>Gread)
                return false;
            actualParents.push_back(edge.上位);
        }
        std::sort(actualParents.begin(),actualParents.end(),[](auto a,auto b){return a.值.值<b.值.值;});
        if(actualParents!=parents)return false;

        const auto &relations=事实->定义关系组;
        if(relations.empty())return false;
        const auto &member=relations.front();
        if(member.种类!=纯概念定义关系种类::定义成员||member.顺序!=1||
           member.源!=事实->概念.值||member.目标!=事实->定义记录||
           !有效(member.关系)||!有效(member.关系类型)||
           member.生命周期.创建事实代次!=事实->生命周期.创建事实代次)
            return false;
        if(const auto *fc=std::get_if<纯I64特征概念定义>(&expected)) {
            if(relations.size()!=2)return false;
            const auto &edge=relations[1];
            if(edge.种类!=纯概念定义关系种类::定义特征类型||edge.顺序!=1||
               edge.源!=事实->定义记录||edge.目标!=fc->特征类型.值||
               !有效(edge.关系)||!有效(edge.关系类型)||
               edge.生命周期.创建事实代次!=事实->生命周期.创建事实代次)
                return false;
        } else if(const auto *ec=std::get_if<纯合取存在概念定义>(&expected)) {
            if(relations.size()!=ec->特征模板组.size()+1)return false;
            for(std::size_t i=0;i<ec->特征模板组.size();++i) {
                const auto &edge=relations[i+1];
                if(edge.种类!=纯概念定义关系种类::定义模板||edge.顺序!=i+1||
                   edge.源!=事实->定义记录||edge.目标!=ec->特征模板组[i].值||
                   !有效(edge.关系)||!有效(edge.关系类型)||
                   edge.生命周期.创建事实代次!=事实->生命周期.创建事实代次)
                    return false;
            }
        } else if(relations.size()!=1)return false;
        return true;
    } catch(...) { return false; }
}
inline bool 纯概念写入结果_v3::成功(
    const 纯概念创建请求_v3 &r) const noexcept {
    try {
        if (版本 != 3 || r.版本 != 3 || !r.G0 || r.G0 == UINT64_MAX ||
            !有效(r.幂等身份) ||
            (状态 != 纯概念状态::已创建 &&
             状态 != 纯概念状态::精确重复) ||
            !Gread || !原请求 || !事实 ||
            原请求->版本 != r.版本 || 原请求->G0 != r.G0 ||
            原请求->幂等身份 != r.幂等身份 || 原请求->定义 != r.定义 ||
            原请求->组织 != r.组织 || 原请求->直接上位 != r.直接上位)
            return false;
        const bool ownWrite = 发布 == 纯概念发布状态::确认发布 &&
                              首次发布H && *首次发布H == r.G0 + 1 &&
                              *首次发布H <= Gread;
        const bool reused = 状态 == 纯概念状态::精确重复 &&
                            发布 == 纯概念发布状态::确认未发布 &&
                            !首次发布H;
        return (ownWrite || reused) &&
               纯概念结果内部::定义匹配(事实->定义, r.定义) &&
               纯概念结果内部::父组匹配(*事实, r.直接上位) &&
               ((r.组织 == 概念初始组织指定::显式顶层 &&
                 r.直接上位.empty()) ||
                (r.组织 == 概念初始组织指定::具名上位 &&
                 !r.直接上位.empty())) &&
               纯概念结果内部::事实完整(*事实, Gread);
    } catch (...) {
        return false;
    }
}
inline bool 纯概念创建恢复结果::成功(const 纯概念创建恢复请求&r) const noexcept {
    return 版本==2&&r.版本==2&&状态==纯概念恢复状态::已读回&&Gread==r.Gread&&
        首次实际G0&&首次发布H&&*首次发布H==*首次实际G0+1&&
        *首次发布H<=Gread&&事实&&
        纯概念结果内部::定义匹配(事实->定义,r.定义)&&
        纯概念结果内部::父组匹配(*事实,r.直接上位)&&
        ((r.组织==概念初始组织指定::显式顶层&&r.直接上位.empty())||
         (r.组织==概念初始组织指定::具名上位&&!r.直接上位.empty()))&&
        纯概念结果内部::事实完整(*事实,Gread);
}
inline bool 纯概念创建恢复结果_v3::成功(
    const 纯概念创建恢复请求_v3 &r) const noexcept {
    return 版本 == 3 && r.版本 == 3 && 状态 == 纯概念恢复状态::已读回 &&
           Gread == r.Gread && 首次实际G0 && 首次发布H &&
           *首次发布H == *首次实际G0 + 1 && *首次发布H <= Gread && 事实 &&
           纯概念结果内部::定义匹配(事实->定义, r.定义) &&
           纯概念结果内部::父组匹配(*事实, r.直接上位) &&
           ((r.组织 == 概念初始组织指定::显式顶层 &&
             r.直接上位.empty()) ||
            (r.组织 == 概念初始组织指定::具名上位 &&
             !r.直接上位.empty())) &&
           纯概念结果内部::事实完整(*事实, Gread);
}
inline bool 纯概念生命周期结果::成功(const 纯概念生命周期请求&r) const noexcept {
    return 版本==2&&r.版本==2&&
        r.G0&&r.G0!=UINT64_MAX&&有效(r.幂等身份)&&有效(r.概念.值)&&
        有效(r.预期生命周期值事实)&&
        纯概念结果内部::允许生命周期迁移(r.预期治理状态,r.目标治理状态)&&
        r.预算.最大概念数&&r.预算.最大关系数&&r.预算.最大特征属性数&&
        (状态==纯概念状态::已迁移生命周期||状态==纯概念状态::精确重复)&&
        发布==纯概念发布状态::确认发布&&Gread&&
        首次发布H&&*首次发布H==r.G0+1&&*首次发布H<=Gread&&原请求&&
        原请求->版本==r.版本&&原请求->G0==r.G0&&原请求->幂等身份==r.幂等身份&&
        原请求->概念==r.概念&&原请求->预期生命周期值事实==r.预期生命周期值事实&&
        原请求->预期治理状态==r.预期治理状态&&原请求->目标治理状态==r.目标治理状态&&
        原请求->预算==r.预算&&事实&&
        事实->概念==r.概念&&事实->治理状态==r.目标治理状态&&
        事实->生命周期值事实!=r.预期生命周期值事实&&
        事实->生命周期值生命周期.创建事实代次==*首次发布H&&
        纯概念结果内部::事实完整(*事实,Gread);
}
inline bool 纯概念退出结果::成功(const 纯概念退出请求&r) const noexcept {
    return 版本==2&&r.版本==2&&
        r.G0&&r.G0!=UINT64_MAX&&有效(r.幂等身份)&&有效(r.概念.值)&&
        有效(r.预期生命周期值事实)&&r.预算.最大概念数&&
        r.预算.最大关系数&&r.预算.最大特征属性数&&
        (状态==纯概念状态::已删除||状态==纯概念状态::精确重复)&&
        发布==纯概念发布状态::确认发布&&Gread&&
        首次发布H&&*首次发布H<=Gread&&*首次发布H==r.G0+1&&
        原请求&&原请求->版本==r.版本&&原请求->G0==r.G0&&
        原请求->幂等身份==r.幂等身份&&原请求->概念==r.概念&&
        原请求->预期生命周期值事实==r.预期生命周期值事实&&
        原请求->预算==r.预算;
}
inline bool 存在概念使用读取结果::成功(const 存在概念使用读取请求&r) const noexcept {
    return 版本==2&&r.版本==2&&状态==纯概念状态::已读取&&Gread==r.Gread&&
        使用&&概念&&使用->E==r.E.值&&使用->EC==概念->概念;
}
inline bool 存在概念两组结构登记结果_v1::成功(
    const 存在概念两组结构登记请求_v1& r) const noexcept {
    if (版本 != 1 || r.版本 != 1 ||
        (状态 != 存在概念两组状态_v3::已创建 && 状态 != 存在概念两组状态_v3::精确重复) ||
        发布 != 纯概念发布状态::确认发布 || !Gread || !首次发布H ||
        *首次发布H != r.G0 + 1 || *首次发布H > Gread || !原请求 ||
        !交付 || !首次材料 || 原请求->版本 != r.版本 ||
        原请求->G0 != r.G0 || 原请求->幂等身份 != r.幂等身份 ||
        原请求->最大首次材料项数 != r.最大首次材料项数) return false;
    const auto& t = 交付->类型;
    const std::array<稳定编码, 6> ids{交付->格式锚点, t.两组定义成员,
        t.自身特征值域项, t.自身项特征类型, t.自身项值域概念, t.已知子存在概念};
    std::set<std::uint64_t> seen;
    return std::all_of(ids.begin(), ids.end(), [&](auto id) {
        return 有效(id) && seen.insert(id.值).second;
    });
}
inline bool 存在概念两组规范化结果_v1::成功(
    const 存在概念两组规范化请求_v1& r) const noexcept {
    return 版本 == 1 && r.版本 == 1 && r.Gread &&
        状态 == 存在概念两组状态_v3::已规范化 && Gread == r.Gread && 定义;
}
inline bool 存在概念两组枚举结果_v1::成功(
    const 存在概念两组枚举请求_v1& r) const noexcept {
    return 版本 == 1 && r.版本 == 1 && r.Gread &&
        状态 == 存在概念两组状态_v3::已枚举 && Gread == r.Gread;
}
inline bool 存在概念两组读取结果_v1::成功(
    const 存在概念两组读取请求_v1& r) const noexcept {
    return 版本 == 1 && r.版本 == 1 && r.Gread &&
        有效(r.EC.值) && 状态 == 存在概念两组状态_v3::已读取 &&
        Gread == r.Gread && 事实 && 事实->概念 == r.EC;
}
inline bool 存在概念两组完整读取结果_v2::成功(
    const 存在概念两组完整读取请求_v2 &r) const noexcept {
    if (版本 != 2 || r.版本 != 2 || !r.Gread ||
        !有效(r.EC.值) || 状态 != 存在概念两组完整读取状态_v2::已读取 ||
        Gread != r.Gread || !事实 || 事实->概念 != r.EC ||
        !事实->定义.自身特征组已完整声明 ||
        !事实->定义.子存在概念组已完整声明 ||
        !纯概念结果内部::生命周期有效(事实->概念生命周期, Gread) ||
        !纯概念结果内部::生命周期有效(事实->定义记录生命周期, Gread) ||
        !纯概念结果内部::生命周期有效(事实->状态生命周期, Gread))
      return false;
    if (事实->定义.自身特征值域组.size() != 事实->自身特征项关系组.size() ||
        事实->定义.已知子存在概念组.size() != 事实->子概念关系组.size())
      return false;
    std::uint64_t previousFt = 0, previousChild = 0;
    for (std::size_t i = 0; i < 事实->定义.自身特征值域组.size(); ++i) {
      const auto &item = 事实->定义.自身特征值域组[i];
      const auto &edge = 事实->自身特征项关系组[i];
      if (!有效(item.FT) || !有效(item.FC.值) || item.FT.编码.值 <= previousFt ||
          !有效(edge.关系) || edge.源 != 事实->定义记录 ||
          !有效(edge.目标) || edge.顺序 != i + 1 ||
          !纯概念结果内部::生命周期有效(edge.生命周期, Gread))
        return false;
      previousFt = item.FT.编码.值;
    }
    for (std::size_t i = 0; i < 事实->定义.已知子存在概念组.size(); ++i) {
      const auto child = 事实->定义.已知子存在概念组[i];
      const auto &edge = 事实->子概念关系组[i];
      if (!有效(child.值) || child.值.值 <= previousChild || !有效(edge.关系) ||
          edge.源 != 事实->定义记录 || edge.目标 != child.值 ||
          edge.顺序 != i + 1 ||
          !纯概念结果内部::生命周期有效(edge.生命周期, Gread))
        return false;
      previousChild = child.值.值;
    }
    return true;
}
inline bool 存在概念两组查询结果_v1::成功(
    const 存在概念两组查询请求_v1& r) const noexcept {
    return 版本 == 1 && r.版本 == 1 && r.Gread &&
        状态 == 存在概念两组状态_v3::已读取 && Gread == r.Gread &&
        事实 && 事实->定义 == r.定义;
}
inline bool 存在概念两组查询结果_v1::确认未找到(
    const 存在概念两组查询请求_v1& r) const noexcept {
    return 版本 == 1 && r.版本 == 1 && r.Gread &&
        状态 == 存在概念两组状态_v3::未找到 && Gread == r.Gread && !事实;
}
inline bool 存在概念两组写入结果_v1::成功(
    const 存在概念两组创建请求_v1& r) const noexcept {
    const bool created = 状态 == 存在概念两组状态_v3::已创建 &&
        发布 == 纯概念发布状态::确认发布 && 首次发布H && *首次发布H == r.G0 + 1 && *首次发布H <= Gread;
    const bool replay = 状态 == 存在概念两组状态_v3::精确重复 &&
        ((发布 == 纯概念发布状态::确认未发布 && !首次发布H) ||
         (发布 == 纯概念发布状态::确认发布 && 首次发布H && *首次发布H <= Gread));
    return 版本 == 1 && r.版本 == 1 && r.G0 && 有效(r.幂等身份) && Gread &&
        (created || replay) && 原请求 && 原请求->版本 == r.版本 && 原请求->G0 == r.G0 &&
        原请求->幂等身份 == r.幂等身份 && 原请求->定义 == r.定义 &&
        原请求->直接上位 == r.直接上位 && 原请求->预算 == r.预算 &&
        事实 && 事实->定义 == r.定义;
}
} // namespace 海中鱼巣
