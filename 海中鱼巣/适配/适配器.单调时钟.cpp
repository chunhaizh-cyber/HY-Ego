#include "适配器.单调时钟.h"

#include <limits>

namespace 海中鱼巣 {

单调时钟适配器::单调时钟适配器(单调时钟适配器配置 配置) noexcept
    : 配置_(配置), 纪元起点_(std::chrono::steady_clock::now()) {}

单调时间证据读取结果 单调时钟适配器::读取可信单调时间证据(
    const 单调时间证据读取请求& 请求) const noexcept {
  if (配置_.运行代次 == 0 || 配置_.时间纪元身份 == 0) {
    return {单调时间证据读取状态::内部不一致, std::nullopt};
  }
  if (请求.合同版本 != 单调时间证据合同版本 ||
      请求.运行代次 == 0 || 请求.时间纪元身份 == 0 ||
      请求.期望时间源版本 == 0) {
    return {单调时间证据读取状态::请求拒绝, std::nullopt};
  }
  if (请求.运行代次 != 配置_.运行代次 ||
      请求.时间纪元身份 != 配置_.时间纪元身份) {
    return {单调时间证据读取状态::纪元错配, std::nullopt};
  }
  if (请求.期望时间源版本 != 稳态时钟时间源版本) {
    return {单调时间证据读取状态::时间源版本漂移, std::nullopt};
  }

  const auto 当前时点 = std::chrono::steady_clock::now();
  if (当前时点 < 纪元起点_) {
    return {单调时间证据读取状态::内部不一致, std::nullopt};
  }
  const auto 经过纳秒 = std::chrono::duration_cast<std::chrono::nanoseconds>(
      当前时点 - 纪元起点_).count();
  if (经过纳秒 < 0 ||
      static_cast<std::uintmax_t>(经过纳秒) >
          static_cast<std::uintmax_t>(std::numeric_limits<std::int64_t>::max())) {
    return {单调时间证据读取状态::计数不可表示, std::nullopt};
  }

  可信单调时间证据 证据;
  证据.运行代次 = 配置_.运行代次;
  证据.时间纪元身份 = 配置_.时间纪元身份;
  证据.从纪元起点经过纳秒 = static_cast<std::int64_t>(经过纳秒);
  return {单调时间证据读取状态::已读取, 证据};
}

} // namespace 海中鱼巣
