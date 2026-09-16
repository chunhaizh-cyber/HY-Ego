#pragma once

#include "应用服务.特征概念类.h"

namespace 海中鱼巣 {

// 场景成员层不再保存观察、采用、定位或候选决策；I64 原子出生的
// 所有判断都由唯一的特征概念应用服务完成。
class 场景成员概念应用服务 final {
  特征概念应用服务& 特征概念服务_;

public:
  explicit 场景成员概念应用服务(特征概念应用服务& service) noexcept
      : 特征概念服务_(service) {}

  I64原子准确特征出生应用结果 处理I64原子准确特征出生(
      const I64原子准确特征出生应用请求& request);
  bool 使用特征概念服务(const 特征概念应用服务& service) const noexcept {
    return &特征概念服务_==&service;
  }
};

} // namespace 海中鱼巣
