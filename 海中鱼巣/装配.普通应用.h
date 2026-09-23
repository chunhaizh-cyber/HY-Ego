#pragma once

#include "业务/应用服务.自我形成.h"
#include "业务/应用服务.本能根运行初始化.h"
#include "业务/应用服务.特征概念类.h"
#include "业务/应用服务.方法登记根生产初始化.h"
#include "线程/线程_自我.h"

#include <cstdint>
#include <filesystem>
#include <optional>
#include "核心/服务.L1事实基座.h"

namespace 海中鱼巣 {

inline constexpr std::uint64_t 普通应用默认自我线程邮箱容量 = 256;
inline constexpr std::uint64_t 普通应用默认自我线程进入停门等待毫秒 = 5000;
inline constexpr std::uint64_t 普通应用默认自我线程停止回收诊断等待毫秒 = 5000;

class 需求类数据服务;

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
  L1事实基座持久恢复失败 = 13,
  本能先天特征概念初始化失败 = 14
};

struct 普通应用配置 final {
  std::uint32_t 版本 = 2;
  L1事实基座持久存储配置_v1 L1事实基座持久存储;
  std::uint64_t 自我线程邮箱容量 = 普通应用默认自我线程邮箱容量;
  std::uint64_t 自我线程进入停门等待毫秒 =
      普通应用默认自我线程进入停门等待毫秒;
  std::uint64_t 自我线程停止回收诊断等待毫秒 =
      普通应用默认自我线程停止回收诊断等待毫秒;
  friend bool operator==(const 普通应用配置&,const 普通应用配置&)=default;
};

struct 普通应用装配结果 final {
  普通应用装配状态 状态 = 普通应用装配状态::入口拒绝;
  std::uint64_t 根事实代次 = 0;
  std::optional<纯概念状态> 概念原因;
  std::optional<存在单例角色状态> 角色原因;
  std::optional<本能先天特征概念初始化状态> 本能先天特征概念原因;
  std::optional<L1事实基座持久恢复结果_v1> 持久恢复;
  bool 成功() const noexcept {
    return 状态 == 普通应用装配状态::已装配 && 根事实代次&&
        持久恢复&&持久恢复->成功()&&!概念原因&&!角色原因&&!本能先天特征概念原因;
  }
};

普通应用装配结果 构造普通应用上下文(const 普通应用配置&) noexcept;
普通应用装配结果 构造普通应用上下文() noexcept;
真实自我形成结果 初始化普通应用自我() noexcept;
真实自我形成结果 读取普通应用自我() noexcept;
方法登记根生产初始化结果 初始化普通应用方法登记根() noexcept;
本能根运行初始化结果_v1 初始化普通应用本能根运行锚点(
    const 方法登记根生产初始化结果&) noexcept;
自我线程创建结果_v1 创建并停门普通应用自我线程() noexcept;
自我线程操作结果_v1 停止并回收普通应用自我线程() noexcept;
std::optional<本能先天特征概念初始化结果>
读取普通应用本能先天特征概念初始化() noexcept;
需求类数据服务* 读取普通应用需求服务() noexcept;

} // namespace 海中鱼巣
