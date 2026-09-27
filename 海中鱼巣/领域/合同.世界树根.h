#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "合同.场景角色组织.h"

namespace 海中鱼巣 {

class 世界树根数据服务;

inline constexpr std::uint32_t 世界树根合同版本 = 1;

enum class 世界树根状态 : std::uint8_t {
  已建立 = 1,
  精确重复 = 2,
  入口拒绝 = 3,
  已有不同根 = 4,
  幂等冲突 = 6,
  引用冲突 = 7,
  数量预算不足 = 8,
  资源失败 = 10,
  内部不一致 = 11,
  已可能发布 = 12
};

struct 世界树根初始化请求 final {
  std::uint32_t 版本 = 世界树根合同版本;
  L1所有者范围写入幂等身份 组合幂等身份{}, 存在幂等身份{}, 场景幂等身份{};
  std::uint64_t 最大关系数量 = 0, 最大场景数量 = 0;
  friend bool operator==(const 世界树根初始化请求 &,
                         const 世界树根初始化请求 &) = default;
};

struct 世界树根存在片段 final {
  L1有限N分区原子参与者写集 写集;
};

struct 世界树根场景片段 final {
  L1有限N分区原子参与者写集 写集;
};

struct 世界树根存在来源 final {
  稳定编码 根{};
  存在身份来源当前见证 来源;
};

struct 世界树根组读取请求 final {
  std::uint32_t 版本 = 世界树根合同版本;
  std::uint64_t 最大关系数量 = 0;
};

struct 世界树根组读取结果 final {
  世界树根状态 状态 = 世界树根状态::入口拒绝;
  std::vector<稳定编码> 根组;
  bool 成功(const 世界树根组读取请求 &) const noexcept;
};

struct 世界树根事实 final {
  稳定编码 根{};
  场景树节点当前事实 根场景;
};

struct 世界树根初始化结果 final {
  世界树根状态 状态 = 世界树根状态::入口拒绝;
  std::uint32_t 版本 = 世界树根合同版本;
  std::optional<世界树根初始化请求> 原请求;
  std::optional<世界树根事实> 事实;
  bool 成功(const 世界树根初始化请求 &) const noexcept;
};

class 世界树根存在参与者 {
public:
  virtual ~世界树根存在参与者() = default;
  virtual const L1事实基座服务 &世界树根底座() const noexcept = 0;
  virtual L1结构所有者身份 世界树根所有者() const noexcept = 0;
  virtual bool 世界树根幂等键可用(L1所有者范围写入幂等身份) const noexcept = 0;
  virtual bool 世界树根结构已就绪() const noexcept = 0;
  virtual 世界树根存在片段
  准备世界树根存在片段(const 世界树根初始化请求 &) const = 0;
  virtual L1所有者范围首次写入读取结果
  读取世界树根存在首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual std::optional<世界树根存在来源>
  读取世界树根存在来源(稳定编码) const = 0;
};

class 世界树根场景参与者 {
public:
  virtual ~世界树根场景参与者() = default;
  virtual const L1事实基座服务 &世界树根底座() const noexcept = 0;
  virtual L1结构所有者身份 世界树根所有者() const noexcept = 0;
  virtual bool 世界树根幂等键可用(L1所有者范围写入幂等身份) const noexcept = 0;
  virtual bool 世界树根结构已就绪() const noexcept = 0;
  virtual 世界树根场景片段
  准备世界树根场景片段(const 世界树根初始化请求 &) const = 0;
  virtual L1所有者范围首次写入读取结果
  读取世界树根场景首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual 世界树根组读取结果
  读取当前世界树根组(const 世界树根组读取请求 &) const = 0;
};

} // namespace 海中鱼巣
