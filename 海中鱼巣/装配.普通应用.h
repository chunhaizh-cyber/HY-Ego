#pragma once

#include "业务/应用服务.自我形成.h"

#include <cstdint>
#include <filesystem>
#include <optional>
#include "核心/服务.L1事实基座.h"

namespace 海中鱼巣 {

enum class 普通应用装配状态 : std::uint8_t {
  已装配 = 1,
  入口拒绝 = 2,
  所有者建立失败 = 3,
  元结构建立失败 = 4,
  服务建立失败 = 5,
  世界树根失败 = 6,
  世界树应用失败 = 7,
  资源失败 = 8,
  内部不一致 = 9,
  概念结构失败 = 10,
  单例角色结构失败 = 11,
  自我服务建立失败 = 12,
  L1事实基座持久恢复失败 = 13
};

struct 普通应用配置 final {
  std::uint32_t 版本 = 1;
  L1事实基座持久存储配置_v1 L1事实基座持久存储;
  friend bool operator==(const 普通应用配置&,const 普通应用配置&)=default;
};

struct 普通应用装配结果 final {
  普通应用装配状态 状态 = 普通应用装配状态::入口拒绝;
  std::uint64_t 根事实代次 = 0;
  std::optional<纯概念状态> 概念原因;
  std::optional<存在单例角色状态> 角色原因;
  std::optional<L1事实基座持久恢复结果_v1> 持久恢复;
  bool 成功() const noexcept {
    return 状态 == 普通应用装配状态::已装配 && 根事实代次&&
        持久恢复&&持久恢复->成功()&&!概念原因&&!角色原因;
  }
};

普通应用装配结果 构造普通应用上下文(const 普通应用配置&) noexcept;
普通应用装配结果 构造普通应用上下文() noexcept;
真实自我形成结果 初始化普通应用自我() noexcept;
真实自我形成结果 读取普通应用自我() noexcept;

} // namespace 海中鱼巣
