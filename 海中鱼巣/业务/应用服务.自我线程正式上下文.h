#pragma once

#include "应用服务.自我形成.h"
#include "../线程/线程_自我.h"

namespace 海中鱼巣 {

class 需求类数据服务;
class L2方法结构服务;

class 自我线程正式上下文提供者 final
    : public 自我线程正式上下文端口 {
public:
  自我线程正式上下文提供者(
      世界树应用服务& 世界,
      const 真实自我形成服务& 自我,
      const 需求类数据服务& 需求,
      const L2方法结构服务& 方法,
      存在单例角色身份 角色) noexcept;

  自我线程正式上下文结果_v1 读取正式上下文(
      const 自我线程正式上下文请求_v1& 请求) noexcept override;

private:
  世界树应用服务& 世界_;
  const 真实自我形成服务& 自我_;
  const 需求类数据服务& 需求_;
  const L2方法结构服务& 方法_;
  存在单例角色身份 角色_;
};

} // namespace 海中鱼巣
