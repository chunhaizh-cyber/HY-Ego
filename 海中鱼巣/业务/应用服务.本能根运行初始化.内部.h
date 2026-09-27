#pragma once

#include "应用服务.本能根运行初始化.h"

namespace 海中鱼巣::本能根运行初始化内部 {

// 仅供本叶实现与确定性专项共用；不构成公开业务ABI。
class 本能根运行初始化调用端口 {
public:
  本能根运行初始化调用端口() = default;
  virtual ~本能根运行初始化调用端口() = default;
  本能根运行初始化调用端口(const 本能根运行初始化调用端口&) = delete;
  本能根运行初始化调用端口& operator=(
      const 本能根运行初始化调用端口&) = delete;
  本能根运行初始化调用端口(本能根运行初始化调用端口&&) = delete;
  本能根运行初始化调用端口& operator=(
      本能根运行初始化调用端口&&) = delete;

  virtual 本能根I64实际F结果 形成或读取实际F(
      const 本能根I64实际F请求&) noexcept = 0;
  virtual 本能根材料结果 建立或读取根材料(
      const 本能根材料请求&) = 0;
};

本能根运行初始化结果 执行组合(
    本能根运行初始化调用端口&, const 真实自我投影&,
    const 本能根运行初始化请求&) noexcept;

} // namespace 海中鱼巣::本能根运行初始化内部
