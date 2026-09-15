#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <utility>

#include "候选.支持线程.h"

#define EVENT_LOG_THREAD_NO_INCLUDES
#include "事件日志线程.数据.h"
#undef EVENT_LOG_THREAD_NO_INCLUDES

namespace 海中鱼巣::事件日志线程内部 {

struct 事件日志线程状态;
class 事件日志生产写入适配器;
class 事件日志线程入口;

} // namespace 海中鱼巣::事件日志线程内部

namespace 海中鱼巣 {

struct 事件日志线程启动结果;
struct 事件日志线程停止连接结果;

class 事件日志线程租约 final {
public:
  事件日志线程租约(事件日志线程租约 &&来源) noexcept;
  事件日志线程租约 &operator=(事件日志线程租约 &&) = delete;
  事件日志线程租约(const 事件日志线程租约 &) = delete;
  事件日志线程租约 &operator=(const 事件日志线程租约 &) = delete;
  ~事件日志线程租约();

  事件日志提交结果 提交(事件日志摘要 摘要) noexcept;
  事件日志停止接收结果 停止接收并取得截止() noexcept;
  事件日志处理位置 读取处理位置() const noexcept;
  事件日志完成等待结果 等待完成到截止(
      const 事件日志完成等待请求 &请求) const noexcept;
  事件日志未取出快照结果 冻结并读取未取出快照(
      const 事件日志未取出快照请求 &请求) noexcept;
  事件日志线程停止连接结果 停止并连接(
      const 事件日志线程停止连接请求 &请求) && noexcept;

private:
  friend 事件日志线程启动结果 启动事件日志线程(
      const 事件日志线程启动请求 &请求,
      线程生命周期发布端口 &生命周期端口) noexcept;

  事件日志线程租约(
      std::unique_ptr<事件日志线程内部::事件日志线程状态> 队列状态,
      std::unique_ptr<事件日志线程内部::事件日志生产写入适配器>
          写入适配器,
      std::unique_ptr<事件日志线程内部::事件日志线程入口> 线程入口,
      支持线程候选回收请求 析构回收请求,
      支持线程候选租约 &&支持候选租约) noexcept;

  std::unique_ptr<事件日志线程内部::事件日志线程状态> 队列状态_;
  std::unique_ptr<事件日志线程内部::事件日志生产写入适配器>
      写入适配器_;
  std::unique_ptr<事件日志线程内部::事件日志线程入口> 线程入口_;
  支持线程候选回收请求 析构回收请求_;
  std::optional<支持线程候选租约> 支持候选租约_;
};

struct 事件日志线程启动结果 final {
  事件日志线程启动状态 状态 = 事件日志线程启动状态::请求拒绝;
  支持线程候选见证 候选见证;
  std::optional<事件日志线程租约> 线程租约;
};

struct 事件日志线程停止连接结果 final {
  事件日志线程停止连接状态 状态 =
      事件日志线程停止连接状态::内部不一致;
  支持线程候选见证 最终见证;
  事件日志处理位置 最终位置;
};

事件日志线程启动结果 启动事件日志线程(
    const 事件日志线程启动请求 &请求,
    线程生命周期发布端口 &生命周期端口) noexcept;

} // namespace 海中鱼巣
