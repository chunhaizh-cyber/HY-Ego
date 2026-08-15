#pragma once

#ifndef EVENT_LOG_THREAD_NO_INCLUDES
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "线程生命周期.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 事件日志线程合同版本 = 1;
inline constexpr std::uint64_t 事件日志摘要队列最大容量 = 65'536;
inline constexpr std::size_t 事件日志模块最大字符数 = 128;
inline constexpr std::size_t 事件日志操作最大字符数 = 128;
inline constexpr std::size_t 事件日志来源摘要最大字符数 = 1'024;
inline constexpr std::size_t 事件日志人读摘要最大字符数 = 4'096;

enum class 事件日志诊断分类 : std::uint8_t {
  运行观察 = 1,
  结构观察 = 2,
  生命周期观察 = 3,
  支持降级观察 = 4,
  审计观察 = 5
};

struct 事件日志摘要 final {
  std::uint32_t 合同版本 = 事件日志线程合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint64_t 摘要身份 = 0;
  std::uint64_t 事件发生时间毫秒 = 0;
  std::wstring 所属模块;
  std::wstring 入口或操作;
  事件日志诊断分类 诊断分类 = 事件日志诊断分类::运行观察;
  std::optional<std::uint64_t> 稳定编码;
  std::optional<std::wstring> 来源请求摘要;
  std::wstring 人读摘要;
};

enum class 事件日志提交状态 : std::uint8_t {
  已接受 = 1,
  请求拒绝 = 2,
  队列已满 = 3,
  队列已停止 = 4,
  资源失败 = 5,
  内部不一致 = 6
};

struct 事件日志提交结果 final {
  事件日志提交状态 状态 = 事件日志提交状态::请求拒绝;
  std::optional<std::uint64_t> 接受位置;
};

enum class 事件日志停止接收状态 : std::uint8_t {
  已停止并取得截止 = 1,
  精确重复 = 2,
  内部不一致 = 3
};

struct 事件日志停止接收结果 final {
  事件日志停止接收状态 状态 =
      事件日志停止接收状态::内部不一致;
  std::uint64_t 接受截止 = 0;
};

struct 事件日志处理位置 final {
  std::uint64_t 接受位置 = 0;
  std::uint64_t 已完成位置 = 0;
  std::uint64_t 未取出数量 = 0;
  std::optional<std::uint64_t> 正在处理位置;
  std::uint64_t 写入未成功次数 = 0;
  bool 已停止接收 = false;
  bool 已冻结取出 = false;
  bool 入口已故障 = false;
};

struct 事件日志完成等待请求 final {
  std::uint32_t 合同版本 = 事件日志线程合同版本;
  std::uint64_t 接受截止 = 0;
  std::uint64_t 最大等待毫秒 = 0;
};

enum class 事件日志完成等待状态 : std::uint8_t {
  已到达 = 1,
  无需等待 = 2,
  等待超时 = 3,
  请求拒绝 = 4,
  入口故障 = 5,
  已完成但未到达 = 6,
  内部不一致 = 7
};

struct 事件日志完成等待结果 final {
  事件日志完成等待状态 状态 =
      事件日志完成等待状态::请求拒绝;
  事件日志处理位置 位置;
};

struct 事件日志排队项 final {
  std::uint64_t 接受位置 = 0;
  事件日志摘要 摘要;
};

struct 事件日志未取出快照请求 final {
  std::uint32_t 合同版本 = 事件日志线程合同版本;
  std::uint64_t 接受截止 = 0;
};

enum class 事件日志未取出快照状态 : std::uint8_t {
  已冻结并读取 = 1,
  无未取出项 = 2,
  精确重复 = 3,
  请求拒绝 = 4,
  资源失败 = 5,
  内部不一致 = 6
};

struct 事件日志未取出快照结果 final {
  事件日志未取出快照状态 状态 =
      事件日志未取出快照状态::请求拒绝;
  std::uint64_t 接受截止 = 0;
  std::optional<std::uint64_t> 正在处理位置;
  std::vector<事件日志排队项> 未取出项;
};

struct 事件日志线程启动请求 final {
  std::uint32_t 合同版本 = 事件日志线程合同版本;
  std::uint64_t 运行代次 = 0;
  线程逻辑身份 线程身份;
  std::uint64_t 队列容量 = 0;
  std::uint64_t 进入等待毫秒 = 0;
  std::uint64_t 启动失败回收诊断等待毫秒 = 0;
};

enum class 事件日志线程启动状态 : std::uint8_t {
  已进入 = 1,
  请求拒绝 = 2,
  身份已使用 = 3,
  线程创建失败 = 4,
  资源失败 = 5,
  进入超时已安全连接 = 6,
  入口故障已安全连接 = 7,
  超过诊断预算后已安全连接 = 8,
  内部不一致 = 9
};

struct 事件日志线程停止连接请求 final {
  std::uint32_t 合同版本 = 事件日志线程合同版本;
  std::uint64_t 诊断等待毫秒 = 0;
};

enum class 事件日志线程停止连接状态 : std::uint8_t {
  已安全连接 = 1,
  超过诊断预算后已安全连接 = 2,
  入口故障已安全连接 = 3,
  请求拒绝但已安全连接 = 4,
  前置未闭合但已安全连接 = 5,
  租约已消费 = 6,
  内部不一致 = 7
};

} // namespace 海中鱼巣
