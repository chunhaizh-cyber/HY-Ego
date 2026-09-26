#pragma once
#include "应用服务.世界树类.h"
#include <array>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>

namespace 海中鱼巣 {

struct 真实自我形成请求 final {
  std::uint32_t 版本 = 1;
  稳定编码 期望世界根{};
  存在单例角色身份 角色;
  L1所有者范围写入幂等身份 概念定义键;
  存在场景概念创建键_v2 世界键;
  世界树绑定创建预算 预算;
  概念树预算 概念预算;
  friend bool operator==(const 真实自我形成请求 &,
                         const 真实自我形成请求 &) = default;
};

struct 真实自我读取请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  稳定编码 期望世界根{};
  存在单例角色身份 角色;
  世界树绑定创建预算 预算;
  概念树预算 概念预算;
  friend bool operator==(const 真实自我读取请求 &,
                         const 真实自我读取请求 &) = default;
};

enum class 真实自我形成状态 : std::uint8_t {
  已形成 = 1, 精确重复 = 2, 已读取 = 3, 入口拒绝 = 4,
  请求冲突 = 5, 根无效 = 6, 角色冲突 = 7, 概念阶段失败 = 8,
  世界阶段失败 = 9, 读取未完成 = 10, 已可能发布 = 11,
  资源失败 = 12, 内部不一致 = 13
};

struct 真实自我投影 final {
  std::uint64_t Gread = 0;
  稳定编码 E{}, 世界根{};
  存在单例角色事实 角色;
  世界树层级位置 位置;
  场景树节点当前事实 场景;
  纯概念事实 概念;
  存在概念使用事实 使用;
  friend bool operator==(const 真实自我投影 &,
                         const 真实自我投影 &) = default;
};

struct 真实自我形成结果 final {
  std::uint32_t 版本 = 1;
  真实自我形成状态 状态 = 真实自我形成状态::入口拒绝;
  std::optional<真实自我形成请求> 请求回显;
  std::optional<世界树概念创建结果_v4> 世界结果;
  std::optional<真实自我投影> 投影;
  bool 成功(const 真实自我形成请求 &) const noexcept;
  bool 成功(const 真实自我读取请求 &) const noexcept;
  friend bool operator==(const 真实自我形成结果 &,
                         const 真实自我形成结果 &) = default;
};

class 真实自我形成服务 final {
public:
  真实自我形成服务(世界树应用服务 &, const 存在类数据服务 &,
                     存在单例角色身份);
  真实自我形成服务() = delete;
  真实自我形成服务(const 真实自我形成服务 &) = delete;
  真实自我形成服务 &operator=(const 真实自我形成服务 &) = delete;
  真实自我形成服务(真实自我形成服务 &&) = delete;
  真实自我形成服务 &operator=(真实自我形成服务 &&) = delete;

  真实自我形成结果 形成(const 真实自我形成请求 &) noexcept;
  真实自我形成结果
  读取当前自我(const 真实自我读取请求 &) const noexcept;

private:
  世界树应用服务 &世界_;
  const 存在类数据服务 &存在_;
  存在单例角色身份 角色_;
};

} // namespace 海中鱼巣
