#pragma once

#include "../领域/数据服务.方法登记根.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

struct 方法登记根生产初始化请求 final {
  std::uint32_t 合同版本 = 1;
  friend bool operator==(const 方法登记根生产初始化请求 &,
                         const 方法登记根生产初始化请求 &) = default;
};

enum class 方法登记根生产初始化状态 : std::uint8_t {
  已初始化, 精确重复, 入口拒绝, 根结构失败,
  当前性漂移, 资源失败, 内部不一致
};

struct 方法登记根生产初始化结果 final {
  方法登记根生产初始化状态 状态 = 方法登记根生产初始化状态::入口拒绝;
  方法登记根生产初始化请求 请求回显{};
  std::uint64_t 最终Gread = 0;
  std::optional<方法登记根事实> 根;
  bool 成功() const noexcept;
};

class 方法登记根生产初始化提供者 final {
 public:
  explicit 方法登记根生产初始化提供者(L2方法结构聚合服务 &) noexcept;
  方法登记根生产初始化提供者() = delete;
  方法登记根生产初始化提供者(const 方法登记根生产初始化提供者 &) = delete;
  方法登记根生产初始化提供者 &operator=(const 方法登记根生产初始化提供者 &) = delete;
  方法登记根生产初始化提供者(方法登记根生产初始化提供者 &&) = delete;
  方法登记根生产初始化提供者 &operator=(方法登记根生产初始化提供者 &&) = delete;

  方法登记根生产初始化结果 初始化(
      const 方法登记根生产初始化请求 &) noexcept;

 private:
  L2方法结构聚合服务 &方法结构聚合服务_;
};

}  // namespace 海中鱼巣
