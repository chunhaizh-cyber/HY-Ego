#pragma once

#include "合同.世界树根.h"

namespace 海中鱼巣 {

class 存在类数据服务;
class 场景类数据服务;

class 世界树根数据服务 final {
public:
  世界树根数据服务() = delete;
  世界树根数据服务(const 世界树根数据服务 &) = delete;
  世界树根数据服务 &operator=(const 世界树根数据服务 &) = delete;
  世界树根数据服务(世界树根数据服务 &&) = delete;
  世界树根数据服务 &operator=(世界树根数据服务 &&) = delete;

  世界树根数据服务(存在类数据服务 &, 场景类数据服务 &);

  世界树根初始化结果
  初始化世界树根(const 世界树根初始化请求 &) noexcept;

private:
  bool 请求有效(const 世界树根初始化请求 &) const noexcept;

  存在类数据服务 &existence_;
  场景类数据服务 &scene_;
};

} // namespace 海中鱼巣
