#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 线程生命周期投影合同版本 = 1;
inline constexpr std::uint32_t 线程生命周期事件模式版本 = 1;
inline constexpr std::uint64_t 线程生命周期最大线程配置上限 = 4096;
inline constexpr std::uint64_t 线程生命周期最大消息配置上限 = 262144;

struct 线程逻辑身份 final {
  std::uint64_t 值 = 0;
  friend bool operator==(const 线程逻辑身份 &, const 线程逻辑身份 &) = default;
};

enum class 项目线程类别 : std::uint32_t {
  未知 = 0,
  运行宿主 = 1,
  任务管理 = 2,
  任务工作 = 3,
  缓存统计 = 4,
  事件日志 = 5,
  外设采样材料 = 6,
  自我 = 7,
  持久证据 = 8,
  控制面板 = 9
};

enum class 项目线程模块 : std::uint32_t {
  未知 = 0,
  运行宿主 = 1,
  任务管理 = 2,
  任务工作 = 3,
  缓存统计 = 4,
  事件日志 = 5,
  外设采样材料 = 6,
  自我治理 = 7,
  持久证据 = 8,
  控制面板 = 9
};

enum class 线程生命周期投影 : std::uint8_t {
  创建 = 1,
  启动中 = 2,
  运行中 = 3,
  等待 = 4,
  暂停 = 5,
  恢复 = 6,
  停止请求中 = 7,
  收尾中 = 8,
  退出前 = 9,
  已退出 = 10,
  故障 = 11,
  异常退出 = 12
};

struct 线程生命周期消息身份 final {
  std::uint64_t 运行代次 = 0;
  线程逻辑身份 线程身份;
  std::uint64_t 事件序号 = 0;
  friend bool operator==(const 线程生命周期消息身份 &,
                         const 线程生命周期消息身份 &) = default;
};

enum class 线程诊断引用类别 : std::uint8_t {
  任务 = 1,
  工作项 = 2
};

struct 线程诊断引用 final {
  线程诊断引用类别 引用类别 = 线程诊断引用类别::任务;
  std::uint64_t 引用身份 = 0;
  friend bool operator==(const 线程诊断引用 &,
                         const 线程诊断引用 &) = default;
};

struct 线程生命周期发布请求 final {
  std::uint32_t 合同版本 = 线程生命周期投影合同版本;
  线程生命周期消息身份 消息身份;
  项目线程类别 线程类别 = 项目线程类别::未知;
  项目线程模块 所属模块 = 项目线程模块::未知;
  std::optional<std::uint64_t> 系统线程身份;
  线程生命周期投影 当前投影 = 线程生命周期投影::创建;
  std::optional<线程生命周期投影> 前一投影;
  std::uint64_t 发生时间毫秒 = 0;
  std::uint32_t 消息版本 = 线程生命周期事件模式版本;
  std::uint64_t 原因键 = 0;
  std::optional<线程诊断引用> 诊断引用;
  friend bool operator==(const 线程生命周期发布请求 &,
                         const 线程生命周期发布请求 &) = default;
};

enum class 线程生命周期消息当前性 : std::uint8_t {
  当前 = 1,
  历史 = 2
};

struct 线程生命周期消息 final {
  线程生命周期发布请求 发布请求;
  线程生命周期消息当前性 当前性 = 线程生命周期消息当前性::历史;
  friend bool operator==(const 线程生命周期消息 &,
                         const 线程生命周期消息 &) = default;
};

struct 当前线程生命周期投影 final {
  std::uint32_t 合同版本 = 线程生命周期投影合同版本;
  std::uint64_t 运行代次 = 0;
  线程逻辑身份 线程身份;
  项目线程类别 线程类别 = 项目线程类别::未知;
  项目线程模块 所属模块 = 项目线程模块::未知;
  std::optional<std::uint64_t> 系统线程身份;
  线程生命周期消息 当前消息;
  bool 已见创建事件 = false;
  bool 前置不完整 = false;
  bool 已进入终态 = false;
  friend bool operator==(const 当前线程生命周期投影 &,
                         const 当前线程生命周期投影 &) = default;
};

enum class 线程生命周期发布状态 : std::uint8_t {
  已发布 = 1,
  精确重复 = 2,
  已接收迟到消息 = 3,
  请求拒绝 = 4,
  消息身份冲突 = 5,
  前一投影不一致 = 6,
  终态回退 = 7,
  系统线程身份冲突 = 8,
  非法状态转换 = 9,
  历史链冲突 = 10,
  运行代次不匹配 = 11,
  投影暂不可用 = 12,
  容量已满 = 13,
  资源失败 = 14,
  内部不一致 = 15
};

struct 线程生命周期发布结果 final {
  线程生命周期发布状态 状态 = 线程生命周期发布状态::请求拒绝;
  std::optional<线程生命周期消息> 消息;
  std::optional<当前线程生命周期投影> 当前投影;
  bool 已接受() const noexcept;
  bool 已成为当前() const noexcept;
};

enum class 当前线程投影读取状态 : std::uint8_t {
  已读取 = 1,
  未找到 = 2,
  请求拒绝 = 3,
  运行代次不匹配 = 4,
  投影暂不可用 = 5,
  资源失败 = 6,
  内部不一致 = 7
};

struct 当前线程投影读取请求 final {
  std::uint32_t 合同版本 = 线程生命周期投影合同版本;
  std::uint64_t 运行代次 = 0;
  线程逻辑身份 线程身份;
};

struct 当前线程投影读取结果 final {
  当前线程投影读取状态 状态 = 当前线程投影读取状态::请求拒绝;
  std::optional<当前线程生命周期投影> 当前投影;
};

struct 当前线程投影组读取请求 final {
  std::uint32_t 合同版本 = 线程生命周期投影合同版本;
  std::uint64_t 运行代次 = 0;
};

struct 当前线程投影组读取结果 final {
  当前线程投影读取状态 状态 = 当前线程投影读取状态::请求拒绝;
  std::vector<当前线程生命周期投影> 当前投影组;
};

struct 线程生命周期投影服务配置 final {
  std::uint32_t 合同版本 = 线程生命周期投影合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint64_t 最大线程数量 = 0;
  std::uint64_t 最大消息数量 = 0;
};

} // namespace 海中鱼巣
