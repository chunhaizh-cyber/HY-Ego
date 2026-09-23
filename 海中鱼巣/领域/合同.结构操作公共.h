#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <utility>
#include "../核心/服务.L1事实基座.h"

namespace 海中鱼巣 {

struct 不可变材料身份_B1 final {
  稳定编码 值{};
  friend bool operator==(const 不可变材料身份_B1 &,
                         const 不可变材料身份_B1 &) = default;
};
inline bool 有效(不可变材料身份_B1 v) noexcept { return 有效(v.值); }

struct 不可变材料格式身份_B1 final {
  稳定编码 值{};
  friend bool operator==(const 不可变材料格式身份_B1 &,
                         const 不可变材料格式身份_B1 &) = default;
};
inline bool 有效(不可变材料格式身份_B1 v) noexcept { return 有效(v.值); }

struct 结构读取截止_B1 final {
  std::uint64_t Gread{};
  std::uint64_t H{};
  friend bool operator==(const 结构读取截止_B1 &,
                         const 结构读取截止_B1 &) = default;
};

struct 世界结构预算_B1 final {
  std::uint64_t 最大节点数{}, 最大关系数{}, 最大值数{}, 最大祖先数{}, 最大后代数{};
  std::uint64_t 最大候选数{}, 最大值元素数{}, 最大材料字节数{}, 最大域原子数{};
  friend bool operator==(const 世界结构预算_B1 &,
                         const 世界结构预算_B1 &) = default;
};

struct 世界结构用量_B1 final {
  std::uint64_t 最大节点数{}, 最大关系数{}, 最大值数{}, 最大祖先数{}, 最大后代数{};
  std::uint64_t 最大候选数{}, 最大值元素数{}, 最大材料字节数{}, 最大域原子数{};
  friend bool operator==(const 世界结构用量_B1 &,
                         const 世界结构用量_B1 &) = default;
};

// 跨结构只读 provider 共用的物理事实预算。四个字段都是硬上限，
// 0 不表示无限；材料总数由节点、关系和值三类实际首次读回共同消耗。
struct 有界事实读取预算_B1 final {
  std::uint64_t 最大节点数{}, 最大关系数{}, 最大值数{}, 最大材料总数{};
  friend bool operator==(const 有界事实读取预算_B1 &,
                         const 有界事实读取预算_B1 &) = default;
};

struct 有界事实读取用量_B1 final {
  std::uint64_t 节点数{}, 关系数{}, 值数{}, 材料总数{};
  friend bool operator==(const 有界事实读取用量_B1 &,
                         const 有界事实读取用量_B1 &) = default;
};

inline bool 世界结构预算有效(const 世界结构预算_B1 &b) noexcept {
  constexpr std::uint64_t normal = 1'048'576;
  return b.最大节点数 <= normal && b.最大关系数 <= normal &&
         b.最大值数 <= normal && b.最大祖先数 <= normal &&
         b.最大后代数 <= normal && b.最大候选数 <= normal &&
         b.最大值元素数 <= normal &&
         b.最大材料字节数 <= 16'777'216 && b.最大域原子数 <= normal;
}

struct 结构生命周期_B1 final {
  std::uint64_t 创建H{};
  std::optional<std::uint64_t> 退出H;
  friend bool operator==(const 结构生命周期_B1 &,
                         const 结构生命周期_B1 &) = default;
};
inline bool 生命周期完整(const 结构生命周期_B1 &v,
                         std::uint64_t g) noexcept {
  return v.创建H != 0 && v.创建H <= g &&
         (!v.退出H || (*v.退出H > v.创建H && *v.退出H <= g));
}
inline bool 活动于(const 结构生命周期_B1 &v, std::uint64_t h) noexcept {
  return v.创建H != 0 && v.创建H <= h && (!v.退出H || h < *v.退出H);
}

struct 结构节点见证_B1 final {
  稳定编码 编码{};
  节点种类 种类{节点种类::普通};
  std::optional<L1所有者范围值表示种类> 属性表示;
  结构生命周期_B1 生命周期;
  friend bool operator==(const 结构节点见证_B1 &,
                         const 结构节点见证_B1 &) = default;
};

struct 结构关系见证_B1 final {
  稳定编码 编码{}, 源{}, 目标{}, 类型{};
  std::int64_t 角色{};
  结构生命周期_B1 生命周期;
  friend bool operator==(const 结构关系见证_B1 &,
                         const 结构关系见证_B1 &) = default;
};

enum class 当前终态_B1 : std::uint8_t {
  未读取 = 0,
  原后态仍成立 = 1,
  合法后继 = 2,
  已退出 = 3
};

struct 结构事务键_B1 final {
  std::uint8_t 参与掩码{};
  std::array<L1所有者范围写入幂等身份, 4> 分区键{};
  std::optional<L1所有者范围写入幂等身份> 组合键;
  friend bool operator==(const 结构事务键_B1 &,
                         const 结构事务键_B1 &) = default;
};

} // namespace 海中鱼巣
