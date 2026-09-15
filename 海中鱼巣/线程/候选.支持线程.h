#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <memory>
#include <thread>
#include <utility>
#include "投影.线程生命周期.h"

#define SUPPORT_THREAD_CANDIDATE_NO_INCLUDES
#include "支持线程候选.数据.h"
#undef SUPPORT_THREAD_CANDIDATE_NO_INCLUDES

namespace 海中鱼巣::支持线程候选内部 {

struct 候选控制块;
struct 租约访问;

} // namespace 海中鱼巣::支持线程候选内部

namespace 海中鱼巣 {

class 支持线程入口端口 {
public:
  virtual ~支持线程入口端口() = default;
  virtual 支持线程入口结果 运行线程入口(
      std::stop_token 停止令牌) = 0;
};

struct 支持线程候选创建结果;
struct 支持线程进入等待结果;
struct 支持线程候选回收结果;

class 支持线程候选租约 final {
public:
  支持线程候选租约(支持线程候选租约 &&来源) noexcept;
  支持线程候选租约 &operator=(支持线程候选租约 &&) = delete;
  支持线程候选租约(const 支持线程候选租约 &) = delete;
  支持线程候选租约 &operator=(const 支持线程候选租约 &) = delete;
  ~支持线程候选租约();

  支持线程候选见证 读取见证() const noexcept;

private:
  friend struct 支持线程候选内部::租约访问;

  支持线程候选租约(
      std::shared_ptr<支持线程候选内部::候选控制块> 控制块,
      std::jthread &&线程) noexcept;

  std::shared_ptr<支持线程候选内部::候选控制块> 控制块_;
  std::jthread 线程_;
};

struct 支持线程候选创建结果 final {
  支持线程候选创建状态 状态 = 支持线程候选创建状态::请求拒绝;
  std::optional<支持线程候选租约> 候选租约;
};

struct 支持线程进入等待结果 final {
  支持线程进入等待状态 状态 = 支持线程进入等待状态::请求拒绝;
  支持线程候选见证 见证;
};

struct 支持线程候选回收结果 final {
  支持线程候选回收状态 状态 = 支持线程候选回收状态::内部不一致;
  支持线程候选见证 最终见证;
};

支持线程候选创建结果 创建支持线程候选(
    const 支持线程候选创建请求 &请求,
    支持线程入口端口 &入口端口,
    线程生命周期发布端口 &生命周期端口) noexcept;

支持线程进入等待结果 等待支持线程进入见证(
    const 支持线程候选租约 &候选租约,
    const 支持线程进入等待请求 &请求) noexcept;

支持线程候选回收结果 停止并连接支持线程启动候选(
    支持线程候选租约 &&候选租约,
    const 支持线程候选回收请求 &请求) noexcept;

} // namespace 海中鱼巣
