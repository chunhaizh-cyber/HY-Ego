#pragma once

#include "本能根运行初始化.数据.h"
#include "应用服务.特征概念类.h"
#include "../领域/数据服务.需求类.h"

namespace 海中鱼巣 {
struct 真实自我投影;
}

namespace 海中鱼巣::本能根运行初始化内部 {

class 本能根运行初始化调用端口 {
public:
  本能根运行初始化调用端口() = default;
  virtual ~本能根运行初始化调用端口() = default;
  本能根运行初始化调用端口(const 本能根运行初始化调用端口 &) = delete;
  本能根运行初始化调用端口 &
  operator=(const 本能根运行初始化调用端口 &) = delete;
  本能根运行初始化调用端口(本能根运行初始化调用端口 &&) = delete;
  本能根运行初始化调用端口 &
  operator=(本能根运行初始化调用端口 &&) = delete;

  virtual 本能根I64实际F结果
  形成或读取实际F(const 本能根I64实际F请求 &) noexcept = 0;
  virtual 本能根材料结果
  建立或读取根材料(const 本能根材料请求 &) = 0;
  virtual 需求当前事实代次核验结果
  核验当前事实代次(std::uint64_t) const noexcept = 0;
};

本能根运行初始化结果_v1
执行组合(本能根运行初始化调用端口 &, const 真实自我投影 &,
         const 本能根运行初始化请求_v1 &) noexcept;

} // namespace 海中鱼巣::本能根运行初始化内部
