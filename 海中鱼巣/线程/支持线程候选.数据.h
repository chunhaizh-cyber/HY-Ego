#pragma once

#ifndef SUPPORT_THREAD_CANDIDATE_NO_INCLUDES
#include <cstdint>
#include <optional>

#include "线程生命周期.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 支持线程候选合同版本 = 1;
inline constexpr std::uint64_t 支持线程最大等待毫秒 =
    0x7FFF'FFFF'FFFF'FFFFULL;
inline constexpr std::uint64_t 支持线程原因_物理线程创建失败 =
    0x53555001;
inline constexpr std::uint64_t 支持线程原因_请求停止 = 0x53555002;
inline constexpr std::uint64_t 支持线程原因_入口内部故障 = 0x53555003;
inline constexpr std::uint64_t 支持线程原因_正常完成 = 0x53555004;

struct 支持线程候选创建请求 final {
  std::uint32_t 合同版本 = 支持线程候选合同版本;
  std::uint64_t 运行代次 = 0;
  线程逻辑身份 线程身份;
  项目线程类别 线程类别 = 项目线程类别::未知;
  项目线程模块 所属模块 = 项目线程模块::未知;
  friend bool operator==(const 支持线程候选创建请求 &,
                         const 支持线程候选创建请求 &) = default;
};

enum class 支持线程入口退出状态 : std::uint8_t {
  已响应停止 = 1,
  内部故障 = 2
};

struct 支持线程入口结果 final {
  支持线程入口退出状态 状态 = 支持线程入口退出状态::内部故障;
  std::uint64_t 原因键 = 0;
};

enum class 支持线程候选创建状态 : std::uint8_t {
  已创建 = 1,
  请求拒绝 = 2,
  身份已使用 = 3,
  线程创建失败 = 4,
  资源失败 = 5,
  内部不一致 = 6
};

enum class 支持线程进入等待状态 : std::uint8_t {
  已进入 = 1,
  等待超时 = 2,
  请求拒绝 = 3,
  内部不一致 = 4
};

enum class 支持线程候选回收状态 : std::uint8_t {
  已安全连接 = 1,
  超过诊断预算后已安全连接 = 2,
  入口故障已安全连接 = 3,
  请求拒绝但已安全连接 = 4,
  内部不一致 = 5
};

struct 支持线程进入等待请求 final {
  std::uint32_t 合同版本 = 支持线程候选合同版本;
  std::uint64_t 最大等待毫秒 = 0;
};

struct 支持线程候选回收请求 final {
  std::uint32_t 合同版本 = 支持线程候选合同版本;
  std::uint64_t 诊断等待毫秒 = 0;
};

struct 支持线程候选见证 final {
  bool 已形成物理线程 = false;
  bool 已进入 = false;
  bool 已请求停止 = false;
  bool 已完成 = false;
  bool 已连接 = false;
  std::optional<std::uint64_t> 系统线程身份;
  std::optional<支持线程入口结果> 入口结果;
  std::uint64_t 生命周期投影未接受次数 = 0;
};

} // namespace 海中鱼巣
