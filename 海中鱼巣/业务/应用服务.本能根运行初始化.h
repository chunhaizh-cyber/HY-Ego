#pragma once

#include "本能根运行初始化.数据.h"
#include "应用服务.特征概念类.h"
#include "应用服务.自我形成.h"
#include "../领域/数据服务.需求类.h"

namespace 海中鱼巣 {

class 本能根运行初始化提供者 final {
public:
  本能根运行初始化提供者(
      特征概念应用服务 &特征概念, const 存在类数据服务 &存在,
      需求类数据服务 &需求, const 真实自我读取请求 &自我读取请求,
      const 真实自我形成结果 &自我读取结果);

  本能根运行初始化提供者() = delete;
  本能根运行初始化提供者(const 本能根运行初始化提供者 &) = delete;
  本能根运行初始化提供者 &
  operator=(const 本能根运行初始化提供者 &) = delete;
  本能根运行初始化提供者(本能根运行初始化提供者 &&) = delete;
  本能根运行初始化提供者 &
  operator=(本能根运行初始化提供者 &&) = delete;

  本能根运行初始化结果_v1
  初始化(const 本能根运行初始化请求_v1 &) noexcept;

private:
  特征概念应用服务 &特征概念_;
  const 存在类数据服务 &存在_;
  需求类数据服务 &需求_;
  真实自我投影 自我投影_;
  bool 已调用_ = false;
};

} // namespace 海中鱼巣
