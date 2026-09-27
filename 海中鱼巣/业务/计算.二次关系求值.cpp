#include "计算.二次关系求值.h"

#include <stdexcept>

namespace 海中鱼巣 {
namespace {

bool 参与者形状有效(const 二次关系参与者材料& participant) noexcept {
  if (!有效(participant.E) || participant.来源组.empty()) return false;
  for (const auto& source : participant.来源组) {
    if (source.valueless_by_exception()) return false;
    if (const auto* value = std::get_if<二次关系准确F来源>(&source)) {
      if (!有效(value->F)) return false;
    } else if (const auto* binding = std::get_if<二次关系状态端点来源>(&source)) {
      if (!有效(binding->B)) return false;
    } else {
      const auto& value = std::get<二次关系本能根目标合同值来源>(source);
      if (!有效(value.根需求) || !有效(value.根目标合同) ||
          !有效(value.目标值事实) || !有效(value.对应实际特征)) return false;
    }
  }
  return true;
}

二次关系判断结果 未实现结果(std::uint64_t request) noexcept {
  二次关系判断结果 result;
  result.状态 = 二次关系判断状态::未实现;
  result.请求身份 = request;
  return result;
}

} // namespace

二次关系求值应用服务::二次关系求值应用服务(
    const L1事实基座服务& l1,
    const 概念树类数据服务& concepts,
    const 状态使用绑定只读提供者& bindings,
    const 状态类数据服务& states,
    const 存在类数据服务& existences,
    const 特征类数据服务& features,
    const 特征值类数据服务& values,
    const 需求类数据服务& demands,
    const 有序I64特征比较提供者& comparison)
    : l1_(l1), 概念_(concepts), 绑定_(bindings), 状态_(states),
      存在_(existences), 特征_(features), 特征值_(values),
      需求_(demands), 比较_(comparison) {
  if (!绑定于(l1)) throw std::invalid_argument("二次关系求值依赖未绑定同一事实基座");
}

bool 二次关系求值应用服务::绑定于(
    const L1事实基座服务& l1) const noexcept {
  return &l1_ == &l1 && 概念_.绑定于(l1) && 绑定_.绑定于(l1) &&
      状态_.绑定于(l1) && 存在_.绑定于(l1) && 特征_.绑定于(l1) &&
      特征值_.绑定于(l1) && 需求_.绑定于(l1);
}

bool 二次关系求值应用服务::使用概念服务(
    const 概念树类数据服务& service) const noexcept {
  return &概念_ == &service;
}

二次关系判断结果 二次关系求值应用服务::求值二次关系(
    const 二次关系求值请求& request) const noexcept {
  if (!request.请求身份 || !有效(request.RC.值) ||
      !参与者形状有效(request.A) || !参与者形状有效(request.B))
    return {};
  // 待实现：现行概念树没有RC完整定义的current-only读取入口。
  return 未实现结果(request.请求身份);
}

二次关系判断结果 二次关系求值应用服务::求值候选定义(
    const 二次关系候选求值请求& request) const noexcept {
  if (!request.请求身份 || request.定义.valueless_by_exception() ||
      !参与者形状有效(request.A) || !参与者形状有效(request.B))
    return {};
  // 待实现：FC/EC/K及准确来源尚无一次current-only联合读取提供者。
  return 未实现结果(request.请求身份);
}

二次关系判断结果 二次关系求值应用服务::求值特征概念(
    const 二次关系FC求值请求& request) const noexcept {
  if (!request.请求身份 || !有效(request.FC.值) ||
      !参与者形状有效(request.参与者) ||
      request.来源下标 >= request.参与者.来源组.size()) return {};
  // 待实现：缺少FC完整域与指定准确来源的一次current-only联合读取。
  return 未实现结果(request.请求身份);
}

二次关系判断结果 二次关系求值应用服务::求值存在概念(
    const 二次关系EC求值请求& request) const noexcept {
  if (!request.请求身份 || !有效(request.EC.值) ||
      !参与者形状有效(request.参与者)) return {};
  // 待实现：缺少EC完整定义与参与存在的一次current-only联合读取。
  return 未实现结果(request.请求身份);
}

} // namespace 海中鱼巣
