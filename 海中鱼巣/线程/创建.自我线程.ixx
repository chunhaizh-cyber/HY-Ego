module;

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <system_error>
#include <thread>
#include <utility>

export module 海中鱼巣.线程.创建.自我线程;

import 海中鱼巣.线程.有界自我治理队列;
import 海中鱼巣.线程.投影.线程生命周期;
// I2 值式材料当前由普通应用模块拥有；激活时按最终公开模块机械匹配。
import 海中鱼巣.装配.普通应用;

export {
#include "自我线程生产创建.数据.h"
}

namespace 海中鱼巣::自我线程生产创建内部 {

// 身份等同：只需确认两棵根消费材料指向同一棵树和同一个根场景。
// 不深比较 L2 场景树/场景事实结构体（其 =default operator== 在 MSVC 模块边界被删）。
inline bool 根消费材料相同(const 自我世界树根消费材料 &左,
                            const 自我世界树根消费材料 &右) noexcept {
  return 左.请求.树 == 右.请求.树 &&
         左.请求.根场景 == 右.请求.根场景;
}

inline bool 请求语义相同(const 自我线程生产创建请求 &左,
                          const 自我线程生产创建请求 &右) noexcept {
  return 左.合同版本 == 右.合同版本 &&
         左.创建幂等身份 == 右.创建幂等身份 &&
         左.线程逻辑身份 == 右.线程逻辑身份 &&
         左.运行代次 == 右.运行代次 &&
         根消费材料相同(左.根消费材料, 右.根消费材料) &&
         左.治理邮箱容量 == 右.治理邮箱容量;
}

inline bool 请求有效(const 自我线程生产创建请求 &请求) noexcept {
  return 请求.合同版本 == 自我线程生产创建合同版本 &&
         请求.创建幂等身份.值 != 0 && 请求.治理邮箱容量 != 0 &&
         请求.线程逻辑身份.值 != 0 && 请求.运行代次 != 0 &&
         请求.入口停门等待毫秒 != 0 && 请求.失败回收诊断毫秒 != 0 &&
         请求.根消费材料.本次事实截止代次 != 0;
}

inline bool 停门见证完整(const 自我线程停门见证 &见证) noexcept {
  return 见证.线程逻辑身份.值 != 0 && 见证.运行代次 != 0 &&
         见证.入口见证序号 != 0 && 见证.停门见证序号 != 0 &&
         见证.停门见证序号 > 见证.入口见证序号 &&
         见证.运行门 == 自我线程治理运行门状态::关闭 &&
         见证.治理邮箱.已冻结批次数量 == 0 &&
         见证.治理邮箱.状态 == 自我治理邮箱状态::运行中;
}

inline 自我线程生产创建状态 合并启动失败与回收状态(
    自我线程生产创建状态 原失败,
    const 自我线程启动候选回收结果 &回收) noexcept {
  switch (回收.状态) {
  case 自我线程启动候选回收状态::已回收:
  case 自我线程启动候选回收状态::无候选:
    return 原失败;
  case 自我线程启动候选回收状态::等待完成超时:
    return 自我线程生产创建状态::启动候选回收超时;
  case 自我线程启动候选回收状态::内部不一致:
  default:
    return 自我线程生产创建状态::内部不一致;
  }
}

} // namespace 海中鱼巣::自我线程生产创建内部

export namespace 海中鱼巣 {

class 自我线程 final {
public:
  explicit 自我线程(
      线程生命周期发布端口 *生命周期发布端口) noexcept
      : 生命周期发布端口_(生命周期发布端口) {}

  ~自我线程() noexcept {
    {
      std::lock_guard<std::mutex> 锁(状态锁_);
      停止已锁存_ = true;
      治理运行门条件_.notify_all();
    }
    if (线程_.joinable())
      线程_.join();
  }

  自我线程(const 自我线程 &) = delete;
  自我线程 &operator=(const 自我线程 &) = delete;
  自我线程(自我线程 &&) = delete;
  自我线程 &operator=(自我线程 &&) = delete;

  自我线程生产创建结果 创建并使自我线程停在治理运行门(
      const 自我线程生产创建请求 &请求) noexcept {
    try {
      // 序列锁覆盖完整创建调用。状态条件等待仍只释放状态锁，因此同请求
      // 并发不会在首次调用停门等待期间误判为第二个 joinable 候选。
      std::lock_guard<std::mutex> 创建调用序列锁(创建调用锁_);
      std::unique_lock<std::mutex> 锁(状态锁_);
      自我线程生产创建结果 结果;
      if (!自我线程生产创建内部::请求有效(请求))
        return 结果;
      if (锁定请求_ &&
          !自我线程生产创建内部::请求语义相同(*锁定请求_, 请求)) {
        结果.状态 = 自我线程生产创建状态::已发布选择冲突;
        return 结果;
      }
      if (已发布停门见证_) {
        结果.状态 = 自我线程生产创建状态::精确重复;
        结果.停门见证 = 已发布停门见证_;
        return 结果;
      }
      if (线程_.joinable()) {
        结果.状态 = 自我线程生产创建状态::内部不一致;
        return 结果;
      }
      if (!锁定请求_)
        锁定请求_ = 请求;

      治理邮箱_ = std::make_unique<有界自我治理邮箱>(请求.治理邮箱容量);
      线程逻辑身份_ = 请求.线程逻辑身份;
      运行代次_ = 请求.运行代次;
      入口已进入_ = false;
      已停在关闭门_ = false;
      已完成_ = false;
      线程入口异常已锁存_ = false;
      停止已锁存_ = false;
      运行门已开放_ = false;
      入口见证序号_ = 0;
      停门见证序号_ = 0;

      锁.unlock();
      结果.创建投影 = 发布生命周期(
          线程生命周期投影::创建, std::nullopt, 0, std::nullopt);
      锁.lock();
      try {
        线程_ = std::thread([this]() noexcept { 线程入口(); });
      } catch (const std::system_error &) {
        结果.状态 = 自我线程生产创建状态::线程候选创建失败;
        return 结果;
      }
      锁.unlock();
      结果.入口投影 = 发布生命周期(
          线程生命周期投影::启动中,
          线程生命周期投影::创建, 0, std::nullopt);
      锁.lock();

      const auto 等待时限 =
          std::chrono::milliseconds(请求.入口停门等待毫秒);
      const bool 已到达 = 状态条件_.wait_for(锁, 等待时限, [this]() {
        return 已停在关闭门_ || 已完成_;
      });
      if (已到达 && 已停在关闭门_ && !已完成_ && 治理邮箱_) {
        自我线程停门见证 见证{
            线程逻辑身份_, 运行代次_, 入口见证序号_, 停门见证序号_,
            自我线程治理运行门状态::关闭, 治理邮箱_->读取快照()};
        if (!自我线程生产创建内部::停门见证完整(见证)) {
          锁.unlock();
          const auto 回收 =
              收口启动失败候选内部(请求.失败回收诊断毫秒);
          结果.状态 = 自我线程生产创建内部::合并启动失败与回收状态(
              自我线程生产创建状态::治理运行门停驻失败, 回收);
          return 结果;
        }
        已发布停门见证_ = 见证;
        结果.状态 = 自我线程生产创建状态::已创建并停门;
        结果.停门见证 = 见证;
        锁.unlock();
        结果.停门投影 = 发布生命周期(
            线程生命周期投影::等待,
            线程生命周期投影::启动中, 0, std::nullopt);
        return 结果;
      }

      const auto 失败状态 = 线程入口异常已锁存_.load()
          ? 自我线程生产创建状态::内部不一致
          : (!入口已进入_
                 ? 自我线程生产创建状态::线程入口未进入
                 : 自我线程生产创建状态::治理运行门停驻失败);
      锁.unlock();
      const auto 回收 =
          收口启动失败候选内部(请求.失败回收诊断毫秒);
      结果.状态 = 自我线程生产创建内部::合并启动失败与回收状态(
          失败状态, 回收);
      return 结果;
    } catch (const std::bad_alloc &) {
      const auto 回收 = 收口启动失败候选(请求.失败回收诊断毫秒);
      return {自我线程生产创建内部::合并启动失败与回收状态(
          自我线程生产创建状态::资源失败, 回收)};
    } catch (...) {
      const auto 回收 = 收口启动失败候选(请求.失败回收诊断毫秒);
      return {自我线程生产创建内部::合并启动失败与回收状态(
          自我线程生产创建状态::内部不一致, 回收)};
    }
  }

  自我线程启动候选回收结果 收口启动失败候选(
      std::uint64_t 完成等待毫秒) noexcept {
    try {
      std::lock_guard<std::mutex> 创建调用序列锁(创建调用锁_);
      {
        std::lock_guard<std::mutex> 锁(状态锁_);
        if (已发布停门见证_)
          return {自我线程启动候选回收状态::内部不一致};
      }
      return 收口启动失败候选内部(完成等待毫秒);
    } catch (...) {
      return {自我线程启动候选回收状态::内部不一致};
    }
  }

  std::optional<自我线程停门见证> 读取停门见证() const {
    std::lock_guard<std::mutex> 锁(状态锁_);
    return 已发布停门见证_;
  }

  // 诊断责任：无适用错误分支；开放治理运行门，唤醒停在关闭门的自我线程进入空转。
  void 开放治理运行门() noexcept {
    {
      std::lock_guard<std::mutex> 锁(状态锁_);
      运行门已开放_ = true;
    }
    治理运行门条件_.notify_all();
  }

private:
  mutable std::mutex 创建调用锁_;
  mutable std::mutex 状态锁_;
  std::condition_variable 状态条件_;
  std::condition_variable 治理运行门条件_;
  线程生命周期发布端口 *生命周期发布端口_ = nullptr;
  std::optional<自我线程生产创建请求> 锁定请求_;
  std::unique_ptr<有界自我治理邮箱> 治理邮箱_;
  std::thread 线程_;
  线程逻辑身份 线程逻辑身份_;
  std::uint64_t 运行代次_ = 0;
  std::atomic_uint64_t 生命周期事件序号_ = 0;
  std::uint64_t 见证序号_ = 0;
  std::uint64_t 入口见证序号_ = 0;
  std::uint64_t 停门见证序号_ = 0;
  bool 入口已进入_ = false;
  bool 已停在关闭门_ = false;
  bool 运行门已开放_ = false;
  bool 停止已锁存_ = false;
  std::atomic_bool 线程入口异常已锁存_ = false;
  std::atomic_bool 已完成_ = false;
  std::optional<自我线程停门见证> 已发布停门见证_;

  自我线程启动候选回收结果 收口启动失败候选内部(
      std::uint64_t 完成等待毫秒) noexcept {
    try {
      {
        std::unique_lock<std::mutex> 锁(状态锁_);
        if (!线程_.joinable())
          return {自我线程启动候选回收状态::无候选};
        停止已锁存_ = true;
        治理运行门条件_.notify_all();
        锁.unlock();
        发布生命周期(线程生命周期投影::停止请求中,
                       线程生命周期投影::等待, 0, std::nullopt);
        锁.lock();
        const bool 已完成 = 状态条件_.wait_for(
            锁, std::chrono::milliseconds(完成等待毫秒),
            [this]() -> bool { return 已完成_; });
        if (!已完成)
          return {自我线程启动候选回收状态::等待完成超时};
      }
      if (线程_.joinable())
        线程_.join();
      std::lock_guard<std::mutex> 锁(状态锁_);
      治理邮箱_.reset();
      已发布停门见证_.reset();
      return {自我线程启动候选回收状态::已回收};
    } catch (...) {
      return {自我线程启动候选回收状态::内部不一致};
    }
  }

  std::optional<线程生命周期发布结果> 发布生命周期(
      线程生命周期投影 当前,
      std::optional<线程生命周期投影> 前一,
      std::uint64_t 原因键,
      std::optional<std::uint64_t> 系统线程身份) noexcept {
    if (!生命周期发布端口_)
      return std::nullopt;
    const auto 当前毫秒 = std::chrono::duration_cast<std::chrono::milliseconds>(
                              std::chrono::system_clock::now()
                                  .time_since_epoch())
                              .count();
    if (当前毫秒 <= 0)
      return std::nullopt;
    const auto 事件序号 = 生命周期事件序号_.fetch_add(1) + 1;
    if (事件序号 == 0)
      return std::nullopt;
    return 生命周期发布端口_->发布(
        {线程生命周期投影合同版本,
         {运行代次_, 线程逻辑身份_, 事件序号},
         项目线程类别::自我,
         项目线程模块::自我治理,
         系统线程身份,
         当前,
         前一,
         static_cast<std::uint64_t>(当前毫秒),
         线程生命周期投影合同版本,
         原因键,
         std::nullopt});
  }

  void 执行线程入口正文() {
    {
      std::unique_lock<std::mutex> 锁(状态锁_);
      入口已进入_ = true;
      入口见证序号_ = ++见证序号_;
      if (!运行门已开放_ && !停止已锁存_) {
        已停在关闭门_ = true;
        停门见证序号_ = ++见证序号_;
      }
      状态条件_.notify_all();
      治理运行门条件_.wait(锁, [this]() {
        return 运行门已开放_ || 停止已锁存_;
      });
      // 空转：治理运行门开放后等待停止信号。
      // 本叶只负责空转；治理循环由后继计划接管；在其实现前不得消费治理邮箱。
      if (运行门已开放_ && !停止已锁存_) {
        治理运行门条件_.wait(锁, [this]() { return 停止已锁存_; });
      }
      已停在关闭门_ = false;
    }
    发布生命周期(线程生命周期投影::退出前,
                   线程生命周期投影::等待, 0, std::nullopt);
    发布生命周期(线程生命周期投影::已退出,
                   线程生命周期投影::退出前, 0, std::nullopt);
    {
      std::lock_guard<std::mutex> 锁(状态锁_);
      已完成_ = true;
      状态条件_.notify_all();
    }
  }

  void 线程入口() noexcept {
    try {
      执行线程入口正文();
    } catch (...) {
      // 异常闭环不得再次依赖可能已经失败的状态锁；原子完成见证只让
      // 创建方进入阶段20回收，不把异常线程升级为停门成功。
      线程入口异常已锁存_.store(true);
      已完成_.store(true);
      状态条件_.notify_all();
    }
  }
};

bool 自我线程生产创建结果::成功() const noexcept {
  return (状态 == 自我线程生产创建状态::已创建并停门 ||
          状态 == 自我线程生产创建状态::精确重复) &&
         停门见证 &&
         自我线程生产创建内部::停门见证完整(*停门见证);
}

} // namespace 海中鱼巣
