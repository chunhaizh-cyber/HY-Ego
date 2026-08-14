module;

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <set>
#include <system_error>
#include <thread>
#include <utility>

export module 海中鱼巣.线程.候选.支持线程;

export import 海中鱼巣.线程.投影.线程生命周期;

#define SUPPORT_THREAD_CANDIDATE_NO_INCLUDES
export {
#include "支持线程候选.数据.h"
}
#undef SUPPORT_THREAD_CANDIDATE_NO_INCLUDES

namespace 海中鱼巣::支持线程候选内部 {

struct 候选控制块;
struct 租约访问;

} // namespace 海中鱼巣::支持线程候选内部

export namespace 海中鱼巣 {

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

namespace 海中鱼巣::支持线程候选内部 {

struct 候选控制块 final {
  候选控制块(const 支持线程候选创建请求 &请求,
             支持线程入口端口 &入口端口,
             线程生命周期发布端口 &生命周期端口)
      : 请求(请求), 入口端口(&入口端口), 生命周期端口(&生命周期端口) {}

  支持线程候选创建请求 请求;
  支持线程入口端口 *入口端口 = nullptr;
  线程生命周期发布端口 *生命周期端口 = nullptr;

  mutable std::mutex 见证锁;
  std::condition_variable 见证条件;
  支持线程候选见证 见证;

  std::mutex 生命周期锁;
  std::optional<线程生命周期投影> 当前物理投影;
  std::uint64_t 下一事件序号 = 1;
  std::atomic_uint64_t 投影未接受次数 = 0;
  std::atomic_bool 停止已锁存 = false;
};

std::mutex 身份守卫锁;
std::set<std::pair<std::uint64_t, std::uint64_t>> 已使用身份;

inline bool 请求有效(const 支持线程候选创建请求 &请求) noexcept {
  if (请求.合同版本 != 支持线程候选合同版本 || 请求.运行代次 == 0 ||
      请求.线程身份.值 == 0)
    return false;
  return (请求.线程类别 == 项目线程类别::事件日志 &&
          请求.所属模块 == 项目线程模块::事件日志) ||
         (请求.线程类别 == 项目线程类别::缓存统计 &&
          请求.所属模块 == 项目线程模块::缓存统计) ||
         (请求.线程类别 == 项目线程类别::持久证据 &&
          请求.所属模块 == 项目线程模块::持久证据);
}

inline bool 等待毫秒有效(std::uint32_t 合同版本,
                         std::uint64_t 毫秒) noexcept {
  return 合同版本 == 支持线程候选合同版本 && 毫秒 != 0 &&
         毫秒 <= 支持线程最大等待毫秒;
}

inline std::uint64_t 当前发生时间毫秒() noexcept {
  const auto 当前毫秒 =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();
  return 当前毫秒 > 0 ? static_cast<std::uint64_t>(当前毫秒) : 1;
}

inline bool 已进入终态(线程生命周期投影 投影) noexcept {
  return 投影 == 线程生命周期投影::已退出 ||
         投影 == 线程生命周期投影::异常退出;
}

inline void 记录投影未接受(候选控制块 &控制块) noexcept {
  const auto 当前 = 控制块.投影未接受次数.load(std::memory_order_relaxed);
  if (当前 != std::numeric_limits<std::uint64_t>::max())
    控制块.投影未接受次数.fetch_add(1, std::memory_order_relaxed);
}

inline void 发布生命周期已加锁(
    候选控制块 &控制块,
    线程生命周期投影 当前投影,
    std::uint64_t 原因键,
    std::optional<std::uint64_t> 系统线程身份) noexcept {
  const auto 事件序号 = 控制块.下一事件序号++;
  const 线程生命周期发布请求 请求{
      线程生命周期投影合同版本,
      {控制块.请求.运行代次, 控制块.请求.线程身份, 事件序号},
      控制块.请求.线程类别,
      控制块.请求.所属模块,
      系统线程身份,
      当前投影,
      控制块.当前物理投影,
      当前发生时间毫秒(),
      线程生命周期事件模式版本,
      原因键,
      std::nullopt};
  控制块.当前物理投影 = 当前投影;
  const auto 结果 = 控制块.生命周期端口->发布(请求);
  if (!结果.已接受())
    记录投影未接受(控制块);
}

inline 支持线程候选见证 读取见证值(
    const 候选控制块 &控制块) noexcept {
  try {
    std::lock_guard<std::mutex> 锁(控制块.见证锁);
    auto 结果 = 控制块.见证;
    结果.生命周期投影未接受次数 =
        控制块.投影未接受次数.load(std::memory_order_relaxed);
    return 结果;
  } catch (...) {
    return {};
  }
}

inline 支持线程入口结果 复核入口结果(
    const 支持线程入口结果 &结果,
    bool 已请求停止) noexcept {
  switch (结果.状态) {
  case 支持线程入口退出状态::已响应停止:
    if (已请求停止)
      return {支持线程入口退出状态::已响应停止,
              支持线程原因_正常完成};
    break;
  case 支持线程入口退出状态::内部故障:
    if (结果.原因键 != 0)
      return 结果;
    break;
  }
  return {支持线程入口退出状态::内部故障,
          支持线程原因_入口内部故障};
}

inline void 发布异常终态链已加锁(
    候选控制块 &控制块,
    std::uint64_t 原因键,
    std::optional<std::uint64_t> 系统线程身份) noexcept {
  发布生命周期已加锁(控制块, 线程生命周期投影::故障,
                       原因键, 系统线程身份);
  发布生命周期已加锁(控制块, 线程生命周期投影::收尾中,
                       原因键, 系统线程身份);
  发布生命周期已加锁(控制块, 线程生命周期投影::退出前,
                       原因键, 系统线程身份);
  发布生命周期已加锁(控制块, 线程生命周期投影::异常退出,
                       原因键, 系统线程身份);
}

inline void 发布正常终态链已加锁(
    候选控制块 &控制块,
    std::optional<std::uint64_t> 系统线程身份) noexcept {
  发布生命周期已加锁(控制块, 线程生命周期投影::收尾中,
                       支持线程原因_请求停止, 系统线程身份);
  发布生命周期已加锁(控制块, 线程生命周期投影::退出前,
                       支持线程原因_请求停止, 系统线程身份);
  发布生命周期已加锁(控制块, 线程生命周期投影::已退出,
                       支持线程原因_正常完成, 系统线程身份);
}

inline void 运行入口包装(
    const std::shared_ptr<候选控制块> &控制块,
    std::stop_token 停止令牌) noexcept {
  const auto 系统线程身份 =
      static_cast<std::uint64_t>(::GetCurrentThreadId());
  {
    std::lock_guard<std::mutex> 生命周期锁(控制块->生命周期锁);
    if (!控制块->停止已锁存.load(std::memory_order_acquire)) {
      发布生命周期已加锁(*控制块, 线程生命周期投影::启动中, 0,
                           系统线程身份);
      发布生命周期已加锁(*控制块, 线程生命周期投影::运行中, 0,
                           系统线程身份);
    }
  }
  {
    std::lock_guard<std::mutex> 见证锁(控制块->见证锁);
    控制块->见证.系统线程身份 = 系统线程身份;
    控制块->见证.已进入 = true;
    控制块->见证条件.notify_all();
  }

  支持线程入口结果 入口结果{
      支持线程入口退出状态::内部故障,
      支持线程原因_入口内部故障};
  try {
    入口结果 = 控制块->入口端口->运行线程入口(停止令牌);
  } catch (...) {
    入口结果 = {支持线程入口退出状态::内部故障,
                支持线程原因_入口内部故障};
  }
  入口结果 = 复核入口结果(
      入口结果, 停止令牌.stop_requested());

  {
    std::lock_guard<std::mutex> 生命周期锁(控制块->生命周期锁);
    if (入口结果.状态 == 支持线程入口退出状态::内部故障)
      发布异常终态链已加锁(*控制块, 入口结果.原因键,
                             系统线程身份);
    else
      发布正常终态链已加锁(*控制块, 系统线程身份);
  }
  {
    std::lock_guard<std::mutex> 见证锁(控制块->见证锁);
    控制块->见证.入口结果 = 入口结果;
    控制块->见证.已请求停止 =
        控制块->停止已锁存.load(std::memory_order_acquire);
    控制块->见证.已完成 = true;
    控制块->见证条件.notify_all();
  }
}

inline bool 租约有效(
    const 支持线程候选租约 &候选租约) noexcept;

inline void 锁存停止并发布(
    候选控制块 &控制块,
    std::jthread &线程) noexcept {
  {
    std::lock_guard<std::mutex> 生命周期锁(控制块.生命周期锁);
    控制块.停止已锁存.store(true, std::memory_order_release);
    线程.request_stop();
    if (控制块.当前物理投影 &&
        *控制块.当前物理投影 != 线程生命周期投影::停止请求中 &&
        !已进入终态(*控制块.当前物理投影)) {
      发布生命周期已加锁(控制块, 线程生命周期投影::停止请求中,
                           支持线程原因_请求停止, std::nullopt);
    }
  }
  {
    std::lock_guard<std::mutex> 见证锁(控制块.见证锁);
    控制块.见证.已请求停止 = true;
    控制块.见证条件.notify_all();
  }
}

inline void 安全连接(
    const std::shared_ptr<候选控制块> &控制块,
    std::jthread &线程) noexcept {
  if (!控制块 || !线程.joinable())
    return;
  锁存停止并发布(*控制块, 线程);
  try {
    线程.join();
  } catch (...) {
    std::terminate();
  }
  try {
    std::lock_guard<std::mutex> 见证锁(控制块->见证锁);
    控制块->见证.已连接 = true;
    控制块->见证条件.notify_all();
  } catch (...) {
    std::terminate();
  }
}

struct 租约访问 final {
  static 支持线程候选租约 形成(
      std::shared_ptr<候选控制块> 控制块,
      std::jthread &&线程) noexcept {
    return 支持线程候选租约(std::move(控制块), std::move(线程));
  }

  static const std::shared_ptr<候选控制块> &控制块(
      const 支持线程候选租约 &候选租约) noexcept {
    return 候选租约.控制块_;
  }

  static std::jthread &线程(支持线程候选租约 &候选租约) noexcept {
    return 候选租约.线程_;
  }

  static bool 可连接(const 支持线程候选租约 &候选租约) noexcept {
    return 候选租约.线程_.joinable();
  }
};

inline bool 租约有效(
    const 支持线程候选租约 &候选租约) noexcept {
  return 租约访问::控制块(候选租约) != nullptr &&
         租约访问::可连接(候选租约);
}

} // namespace 海中鱼巣::支持线程候选内部

namespace 海中鱼巣 {

支持线程候选租约::支持线程候选租约(
    std::shared_ptr<支持线程候选内部::候选控制块> 控制块,
    std::jthread &&线程) noexcept
    : 控制块_(std::move(控制块)), 线程_(std::move(线程)) {}

支持线程候选租约::支持线程候选租约(
    支持线程候选租约 &&来源) noexcept
    : 控制块_(std::move(来源.控制块_)), 线程_(std::move(来源.线程_)) {}

支持线程候选租约::~支持线程候选租约() {
  支持线程候选内部::安全连接(控制块_, 线程_);
}

支持线程候选见证 支持线程候选租约::读取见证() const noexcept {
  if (!控制块_)
    return {};
  return 支持线程候选内部::读取见证值(*控制块_);
}

支持线程候选创建结果 创建支持线程候选(
    const 支持线程候选创建请求 &请求,
    支持线程入口端口 &入口端口,
    线程生命周期发布端口 &生命周期端口) noexcept {
  using namespace 支持线程候选内部;
  if (!请求有效(请求))
    return {};

  try {
    {
      std::lock_guard<std::mutex> 锁(身份守卫锁);
      const auto [位置, 已插入] =
          已使用身份.emplace(请求.运行代次, 请求.线程身份.值);
      if (!已插入)
        return {支持线程候选创建状态::身份已使用, std::nullopt};
    }

    std::shared_ptr<候选控制块> 控制块;
    try {
      控制块 = std::make_shared<候选控制块>(请求, 入口端口,
                                             生命周期端口);
    } catch (const std::bad_alloc &) {
      return {支持线程候选创建状态::资源失败, std::nullopt};
    } catch (const std::system_error &) {
      return {支持线程候选创建状态::资源失败, std::nullopt};
    }

    {
      std::lock_guard<std::mutex> 生命周期锁(控制块->生命周期锁);
      发布生命周期已加锁(*控制块, 线程生命周期投影::创建, 0,
                           std::nullopt);
    }

    std::jthread 线程;
    try {
      线程 = std::jthread([控制块](std::stop_token 停止令牌) noexcept {
        运行入口包装(控制块, 停止令牌);
      });
    } catch (const std::bad_alloc &) {
      std::lock_guard<std::mutex> 生命周期锁(控制块->生命周期锁);
      发布异常终态链已加锁(*控制块,
                             支持线程原因_物理线程创建失败,
                             std::nullopt);
      return {支持线程候选创建状态::资源失败, std::nullopt};
    } catch (const std::system_error &) {
      std::lock_guard<std::mutex> 生命周期锁(控制块->生命周期锁);
      发布异常终态链已加锁(*控制块,
                             支持线程原因_物理线程创建失败,
                             std::nullopt);
      return {支持线程候选创建状态::线程创建失败, std::nullopt};
    } catch (...) {
      std::lock_guard<std::mutex> 生命周期锁(控制块->生命周期锁);
      发布异常终态链已加锁(*控制块,
                             支持线程原因_入口内部故障,
                             std::nullopt);
      return {支持线程候选创建状态::内部不一致, std::nullopt};
    }

    try {
      {
        std::lock_guard<std::mutex> 见证锁(控制块->见证锁);
        控制块->见证.已形成物理线程 = true;
      }
      auto 租约 = 租约访问::形成(控制块, std::move(线程));
      return {支持线程候选创建状态::已创建,
              std::optional<支持线程候选租约>(std::move(租约))};
    } catch (...) {
      安全连接(控制块, 线程);
      return {支持线程候选创建状态::内部不一致, std::nullopt};
    }
  } catch (const std::bad_alloc &) {
    return {支持线程候选创建状态::资源失败, std::nullopt};
  } catch (...) {
    return {支持线程候选创建状态::内部不一致, std::nullopt};
  }
}

支持线程进入等待结果 等待支持线程进入见证(
    const 支持线程候选租约 &候选租约,
    const 支持线程进入等待请求 &请求) noexcept {
  using namespace 支持线程候选内部;
  const auto &控制块 = 租约访问::控制块(候选租约);
  if (!租约有效(候选租约) ||
      !等待毫秒有效(请求.合同版本, 请求.最大等待毫秒))
    return {支持线程进入等待状态::请求拒绝,
            控制块 ? 读取见证值(*控制块) : 支持线程候选见证{}};
  try {
    std::unique_lock<std::mutex> 锁(控制块->见证锁);
    if (!控制块->见证.已进入 && !控制块->见证.已完成) {
      控制块->见证条件.wait_for(
          锁, std::chrono::milliseconds(请求.最大等待毫秒),
          [&控制块]() {
            return 控制块->见证.已进入 || 控制块->见证.已完成;
          });
    }
    auto 见证 = 控制块->见证;
    见证.生命周期投影未接受次数 =
        控制块->投影未接受次数.load(std::memory_order_relaxed);
    if (见证.已进入)
      return {支持线程进入等待状态::已进入, 见证};
    if (见证.已完成)
      return {支持线程进入等待状态::内部不一致, 见证};
    return {支持线程进入等待状态::等待超时, 见证};
  } catch (...) {
    return {支持线程进入等待状态::内部不一致,
            读取见证值(*控制块)};
  }
}

支持线程候选回收结果 停止并连接支持线程启动候选(
    支持线程候选租约 &&候选租约,
    const 支持线程候选回收请求 &请求) noexcept {
  using namespace 支持线程候选内部;
  支持线程候选租约 本地租约(std::move(候选租约));
  auto &线程 = 租约访问::线程(本地租约);
  const auto &控制块 = 租约访问::控制块(本地租约);
  if (!控制块 || !线程.joinable())
    return {};

  const bool 请求有效 =
      等待毫秒有效(请求.合同版本, 请求.诊断等待毫秒);
  bool 已超过诊断预算 = false;
  try {
    锁存停止并发布(*控制块, 线程);
    {
      std::unique_lock<std::mutex> 锁(控制块->见证锁);
      if (请求有效) {
        已超过诊断预算 = !控制块->见证条件.wait_for(
            锁, std::chrono::milliseconds(请求.诊断等待毫秒),
            [&控制块]() { return 控制块->见证.已完成; });
      }
      if (!控制块->见证.已完成)
        控制块->见证条件.wait(
            锁, [&控制块]() { return 控制块->见证.已完成; });
    }
    线程.join();
    {
      std::lock_guard<std::mutex> 锁(控制块->见证锁);
      控制块->见证.已连接 = true;
    }
  } catch (...) {
    std::terminate();
  }

  const auto 见证 = 读取见证值(*控制块);
  if (!见证.已形成物理线程 || !见证.已进入 || !见证.已完成 ||
      !见证.已连接 || !见证.系统线程身份 ||
      !见证.入口结果)
    return {支持线程候选回收状态::内部不一致, 见证};
  if (!请求有效)
    return {支持线程候选回收状态::请求拒绝但已安全连接, 见证};
  if (见证.入口结果->状态 == 支持线程入口退出状态::内部故障)
    return {支持线程候选回收状态::入口故障已安全连接, 见证};
  if (已超过诊断预算)
    return {支持线程候选回收状态::超过诊断预算后已安全连接,
            见证};
  return {支持线程候选回收状态::已安全连接, 见证};
}

} // namespace 海中鱼巣
