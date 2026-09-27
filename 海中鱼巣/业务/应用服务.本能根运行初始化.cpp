#include "应用服务.本能根运行初始化.h"

#include <array>
#include <new>
#include <stdexcept>

namespace 海中鱼巣 {

bool 本能根运行单根锚点::完整() const noexcept {
  const 稳定编码 值[]{需求, 列表项, 实际特征.编码, 目标合同};
  for (std::size_t i = 0; i < std::size(值); ++i) {
    if (!有效(值[i])) return false;
    for (std::size_t j = 0; j < i; ++j)
      if (值[i] == 值[j]) return false;
  }
  return true;
}

bool 本能根运行锚点::完整() const noexcept {
  if (!有效(自我) || !安全根.完整() || !服务根.完整()) return false;
  const 稳定编码 值[]{安全根.需求, 安全根.列表项,
                       安全根.实际特征.编码, 安全根.目标合同,
                       服务根.需求, 服务根.列表项,
                       服务根.实际特征.编码, 服务根.目标合同};
  for (std::size_t i = 0; i < std::size(值); ++i) {
    if (值[i] == 自我) return false;
    for (std::size_t j = 0; j < i; ++j)
      if (值[i] == 值[j]) return false;
  }
  return true;
}

bool 本能根运行初始化结果::成功() const noexcept {
  return 状态 == 本能根运行初始化状态::已形成 && 锚点 &&
         锚点->完整() && 锚点->自我 == 原请求.唯一自我;
}

本能根运行初始化提供者::本能根运行初始化提供者(
    特征概念应用服务& 特征概念, const 存在类数据服务& 存在,
    需求类数据服务& 需求, const 真实自我读取请求& 自我读取请求,
    const 真实自我形成结果& 自我读取结果)
    : 特征概念_(特征概念), 存在_(存在), 需求_(需求),
      自我投影_(自我读取结果.投影.value_or(真实自我投影{})) {
  if (!自我读取结果.成功(自我读取请求) || !自我读取结果.投影 ||
      自我读取结果.投影->E == 自我读取请求.期望世界根)
    throw std::invalid_argument("本能根运行初始化需要唯一自我正式读回");
}

本能根运行初始化结果 本能根运行初始化提供者::初始化(
    const 本能根运行初始化请求& 请求) noexcept {
  本能根运行初始化结果 结果;
  结果.原请求 = 请求;
  if (已调用_ || !有效(请求.唯一自我) || 请求.唯一自我 != 自我投影_.E)
    return 结果;
  已调用_ = true;

  // 待实现：安全值和服务值实际F仍依赖尚未闭合的IF/R current-only入口。
  // 在双根不能由一个联合当前投影正式读回前，不建立根材料，不返回锚点。
  结果.状态 = 本能根运行初始化状态::未实现;
  return 结果;
}

} // namespace 海中鱼巣
