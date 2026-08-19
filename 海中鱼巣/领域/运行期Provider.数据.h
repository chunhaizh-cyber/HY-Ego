#pragma once

// 三个 provider 的 DTO：完整秒时钟服务、运行期事实版本服务、自我治理批次路由
// 对齐 BIZ-L3-002-01-02/03 v0.2、BIZ-L2-002-01 v0.2
#ifndef 运行期Provider_NO_INCLUDES
#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 完整秒时钟合同版本 = 1;
inline constexpr std::uint32_t 事实版本服务合同版本 = 1;
inline constexpr std::uint32_t 批次路由合同版本 = 1;

// ===== BIZ-L3-002-01-02 完整秒时钟服务 =====

struct 时间纪元身份 final {
  std::uint64_t 值 = 0;
  friend bool operator==(const 时间纪元身份&, const 时间纪元身份&) = default;
};

enum class 完整秒状态 : std::uint8_t {
  已读取 = 1,
  无新完整秒 = 2,
  请求拒绝 = 3,
  纪元错配 = 4,
  时间源版本漂移 = 5,
  时间倒退 = 6,
  计数不可表示 = 7,
  内部不一致 = 8
};

struct 完整秒观察 final {
  std::uint64_t 完整秒边界 = 0;
  std::uint64_t 纳秒偏移 = 0;
  friend bool operator==(const 完整秒观察&, const 完整秒观察&) = default;
};

struct 完整秒区间 final {
  std::uint64_t 起始完整秒 = 0;
  std::uint64_t 结束完整秒 = 0;
  friend bool operator==(const 完整秒区间&, const 完整秒区间&) = default;
};

struct 完整秒边界读取请求 final {
  std::uint32_t 合同版本 = 完整秒时钟合同版本;
  std::uint64_t 运行代次 = 0;
  时间纪元身份 纪元身份;
  std::uint64_t 上一已消费完整秒边界 = 0;
  std::uint32_t 期望时间源版本 = 0;
};

struct 完整秒边界读取结果 final {
  完整秒状态 状态 = 完整秒状态::内部不一致;
  完整秒观察 观察;
  std::optional<完整秒区间> 区间;
};

// ===== BIZ-L3-002-01-03 运行期事实版本服务 =====

enum class 共享事实截止状态 : std::uint8_t {
  已读取 = 1,
  请求拒绝 = 2,
  资源失败 = 3,
  内部不一致 = 4
};

struct 共享事实截止读取请求 final {
  std::uint32_t 合同版本 = 事实版本服务合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint32_t 事实范围版本 = 0;
};

struct 共享事实截止读取结果 final {
  共享事实截止状态 状态 = 共享事实截止状态::内部不一致;
  std::uint64_t 共享事实截止 = 0;
};

// ===== BIZ-L2-002-01 自我治理批次路由 =====

enum class 治理批次状态 : std::uint8_t {
  已冻结 = 1,
  等待超时 = 2,
  已停止 = 3,
  请求拒绝 = 4,
  版本漂移 = 5,
  资源失败 = 6,
  时间证据缺口 = 7,
  内部不一致 = 8
};

struct 待重试见证 final {
  std::uint64_t 批次序号 = 0;
  std::uint64_t 运行代次 = 0;
  时间纪元身份 纪元身份;
  std::uint32_t 事实范围版本 = 0;
  friend bool operator==(const 待重试见证&, const 待重试见证&) = default;
};

struct 治理批次组合请求 final {
  std::uint32_t 合同版本 = 批次路由合同版本;
  std::uint64_t 运行代次 = 0;
  时间纪元身份 纪元身份;
  std::uint32_t 期望时间源版本 = 0;
  std::uint64_t 最大等待毫秒 = 0;
  std::uint64_t 上一已完成批次序号 = 0;
  std::uint64_t 上一已消费完整秒边界 = 0;
  std::uint32_t 事实范围版本 = 0;
  std::optional<待重试见证> 重试见证;
};

// 不可变治理批次：邮箱消息组 + 完整秒观察 + 可选区间 + 共享事实截止 + 批次序号
struct 不可变治理批次 final {
  std::uint64_t 批次序号 = 0;
  std::vector<std::uint64_t> 消息身份组;
  完整秒观察 完整秒;
  std::optional<完整秒区间> 完整秒区间;
  std::uint64_t 共享事实截止 = 0;
};

struct 自我治理批次组合结果 final {
  治理批次状态 状态 = 治理批次状态::内部不一致;
  std::optional<不可变治理批次> 治理批次;
  std::optional<待重试见证> 重试见证;
};

} // namespace 海中鱼巣
