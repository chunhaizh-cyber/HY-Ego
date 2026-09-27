#pragma once

#include "应用服务.自我形成.h"
#include "../领域/数据服务.需求类.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

class L2方法结构服务;

enum class 自我线程正式上下文状态 : std::uint8_t {
  成功 = 1,
  精确重复 = 2,
  未实现 = 3,
  依赖未就绪 = 4,
  引用冲突 = 6,
  不可比较 = 7,
  资源失败 = 8,
  入口拒绝 = 9,
  内部错误 = 10
};

struct 自我线程正式上下文请求 final {
  稳定编码 期望世界根{}, 期望自我{}, 期望所在场景{};
  存在单例角色身份 角色;
  特征信息身份 安全实际特征, 服务实际特征;
  friend bool operator==(const 自我线程正式上下文请求&,
                         const 自我线程正式上下文请求&) = default;
};

struct 自我线程正式上下文投影 final {
  稳定编码 世界{}, 自我所在场景{}, 自我{};
  本能根材料 安全根, 服务根;
  bool 完整() const noexcept;
  friend bool operator==(const 自我线程正式上下文投影&,
                         const 自我线程正式上下文投影&) = default;
};

struct 自我线程正式上下文结果 final {
  自我线程正式上下文状态 状态 = 自我线程正式上下文状态::入口拒绝;
  std::optional<自我线程正式上下文投影> 投影;
  bool 写业务事实 = false;
  bool 成功(const 自我线程正式上下文请求&) const noexcept;
};

class 自我线程正式上下文提供者 final {
public:
  自我线程正式上下文提供者(
      世界树应用服务&, const 真实自我形成服务&,
      const 需求类数据服务&, const L2方法结构服务&,
      存在单例角色身份) noexcept;

  自我线程正式上下文结果 读取正式上下文(
      const 自我线程正式上下文请求&) noexcept;

private:
  世界树应用服务& 世界_;
  const 真实自我形成服务& 自我_;
  const 需求类数据服务& 需求_;
  const L2方法结构服务& 方法_;
  存在单例角色身份 角色_;
};

} // namespace 海中鱼巣
