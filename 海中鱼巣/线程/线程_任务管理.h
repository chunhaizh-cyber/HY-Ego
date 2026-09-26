#pragma once

#include "线程_自我.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 任务管理线程合同版本_v1=1;
enum class 任务管理线程生命周期_v1:std::uint8_t{
  未创建=1,启动中=2,接收中=3,停止排空中=4,已停止=5,内部错误=6
};
enum class 任务管理线程操作状态_v1:std::uint8_t{
  已创建=1,精确重复=2,已请求停止=3,已停止=4,
  入口拒绝=5,选择冲突=6,等待超时=7,资源失败=8,内部不一致=9
};
struct 任务管理线程创建请求_v1 final{
  std::uint32_t 合同版本=任务管理线程合同版本_v1;
  std::uint64_t 邮箱容量=0;
  friend bool operator==(const 任务管理线程创建请求_v1&,
                         const 任务管理线程创建请求_v1&)=default;
};
struct 任务管理线程操作结果_v1 final{
  任务管理线程操作状态_v1 状态=任务管理线程操作状态_v1::入口拒绝;
  std::uint32_t 合同版本=任务管理线程合同版本_v1;
  任务管理线程生命周期_v1 生命周期=任务管理线程生命周期_v1::未创建;
};
struct 任务管理线程诊断快照_v1 final{
  std::uint32_t 合同版本=任务管理线程合同版本_v1;
  任务管理线程生命周期_v1 生命周期=任务管理线程生命周期_v1::未创建;
  bool 线程已进入=false,停止已请求=false,线程已完成=false;
  std::uint64_t 邮箱数量=0,邮箱容量=0;
  std::optional<自我线程消息身份> 当前消息;
  std::uint64_t 已接收数量=0,已完成承接数量=0,具名失败数量=0,
                停机未完成数量=0,自动重试数量=0;
  本能根任务阶段状态_v1 最近owner状态=本能根任务阶段状态_v1::未执行;
  bool 内部错误锁存=false;
};

class 任务管理线程 final:public 自我线程根治理意图接收端口_v1{
public:
  任务管理线程() noexcept;
  ~任务管理线程() noexcept;
  任务管理线程(const 任务管理线程&)=delete;
  任务管理线程& operator=(const 任务管理线程&)=delete;
  任务管理线程(任务管理线程&&)=delete;
  任务管理线程& operator=(任务管理线程&&)=delete;

  任务管理线程操作结果_v1 创建并等待就绪(
      const 任务管理线程创建请求_v1&,本能根任务核心端口_v1&,
      std::uint64_t 等待毫秒) noexcept;
  bool 已就绪() const noexcept override;
  根治理意图投递结果_v1 提交(
      const 自我到任务管理本能根承接消息_v1&) noexcept override;
  任务管理线程操作结果_v1 请求停止() noexcept;
  任务管理线程操作结果_v1 等待停止(std::uint64_t 等待毫秒) noexcept;
  任务管理线程诊断快照_v1 读取诊断快照() const noexcept;
private:
  struct 实现;
  实现* 实现_=nullptr;
};

} // namespace 海中鱼巣
