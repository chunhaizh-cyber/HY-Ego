#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 单调时间证据合同版本 = 1;
inline constexpr std::uint32_t 稳态时钟时间源版本 = 1;

struct 单调时钟适配器配置 final {
  std::uint64_t 运行代次 = 0;
  std::uint64_t 时间纪元身份 = 0;
};

struct 单调时间证据读取请求 final {
  std::uint32_t 合同版本 = 单调时间证据合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint64_t 时间纪元身份 = 0;
  std::uint32_t 期望时间源版本 = 稳态时钟时间源版本;
};

enum class 单调时间证据读取状态 : std::uint8_t {
  已读取 = 1,
  请求拒绝 = 2,
  纪元错配 = 3,
  时间源版本漂移 = 4,
  计数不可表示 = 5,
  内部不一致 = 6
};

struct 可信单调时间证据 final {
  std::uint32_t 合同版本 = 单调时间证据合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint64_t 时间纪元身份 = 0;
  std::int64_t 从纪元起点经过纳秒 = 0;
  std::uint32_t 时间源版本 = 稳态时钟时间源版本;
};

struct 单调时间证据读取结果 final {
  单调时间证据读取状态 状态 = 单调时间证据读取状态::请求拒绝;
  std::optional<可信单调时间证据> 证据;
};

class 单调时钟适配器 final {
public:
  explicit 单调时钟适配器(单调时钟适配器配置 配置) noexcept;

  单调时钟适配器(const 单调时钟适配器&) = delete;
  单调时钟适配器& operator=(const 单调时钟适配器&) = delete;
  单调时钟适配器(单调时钟适配器&&) = delete;
  单调时钟适配器& operator=(单调时钟适配器&&) = delete;

  单调时间证据读取结果 读取可信单调时间证据(
      const 单调时间证据读取请求& 请求) const noexcept;

private:
  const 单调时钟适配器配置 配置_;
  const std::chrono::steady_clock::time_point 纪元起点_;
};

} // namespace 海中鱼巣
