// 文件规则：任务工作线程只消费已确认的 L4 强类型执行请求并调用实例方法执行提供者；不得预形成正式任务结果或裁决任务完成。
module;

#include <chrono>
#include <cstdint>
#include <mutex>
#include <optional>
#include <thread>

export module 海中鱼巣.线程.任务工作线程;

import 海中鱼巣.线程.任务管理线程;
import 海中鱼巣.领域.内部治理.服务.实例方法执行;
import 海中鱼巣.领域.内部治理.服务.任务执行最终当前性复核;
import 海中鱼巣.线程.协议.任务动作完成消息;
import 海中鱼巣.线程.服务.任务动作结果收束;

export namespace 海中鱼巣 {

enum class 任务工作线程状态 : std::uint8_t {
    未启动 = 1,
    运行中 = 2,
    正在排空 = 3,
    已停止 = 4,
    内部错误 = 5
};

struct 任务工作线程操作结果 final {
    bool 成功 = false;
    任务工作线程状态 状态 = 任务工作线程状态::未启动;
};

struct 任务工作线程快照 final {
    任务工作线程状态 状态 = 任务工作线程状态::未启动;
    std::uint64_t 已取出数量 = 0;
    std::uint64_t 已形成待验证材料数量 = 0;
    std::uint64_t 已拒绝数量 = 0;
    std::optional<实例方法执行结果> 最后执行结果;
    std::optional<任务动作结果收束结果> 最后收束结果;
};

class 任务工作线程 final {
public:
    任务工作线程(
        任务管理线程& 请求来源,
        任务执行最终当前性复核提供者& 最终复核提供者,
        实例方法执行提供者& 执行提供者,
        任务动作结果收束提供者& 结果收束提供者) noexcept
        : 请求来源_(请求来源), 最终复核提供者_(最终复核提供者),
          执行提供者_(执行提供者), 结果收束提供者_(结果收束提供者) {
    }

    ~任务工作线程() noexcept {
        (void)停止接收并排空();
        (void)收口等待();
    }

    任务工作线程(const 任务工作线程&) = delete;
    任务工作线程& operator=(const 任务工作线程&) = delete;
    任务工作线程(任务工作线程&&) = delete;
    任务工作线程& operator=(任务工作线程&&) = delete;

    任务工作线程操作结果 启动() noexcept {
        std::lock_guard<std::mutex> 锁(锁_);
        if (状态_ != 任务工作线程状态::未启动
            || 请求来源_.当前状态() != 任务管理线程状态::运行中) {
            return {false, 状态_};
        }
        try {
            状态_ = 任务工作线程状态::运行中;
            线程_ = std::thread([this]() noexcept { 运行强类型执行循环(); });
            return {true, 状态_};
        } catch (...) {
            状态_ = 任务工作线程状态::内部错误;
            return {false, 状态_};
        }
    }

    任务工作线程操作结果 停止接收并排空() noexcept {
        {
            std::lock_guard<std::mutex> 锁(锁_);
            if (状态_ == 任务工作线程状态::未启动
                || 状态_ == 任务工作线程状态::已停止) {
                return {状态_ == 任务工作线程状态::已停止, 状态_};
            }
            if (状态_ != 任务工作线程状态::内部错误) {
                状态_ = 任务工作线程状态::正在排空;
            }
        }
        (void)请求来源_.请求停止新派发();
        std::lock_guard<std::mutex> 锁(锁_);
        return {状态_ == 任务工作线程状态::正在排空, 状态_};
    }

    任务工作线程操作结果 收口等待() noexcept {
        if (线程_.joinable()) 线程_.join();
        std::lock_guard<std::mutex> 锁(锁_);
        if (状态_ != 任务工作线程状态::内部错误) {
            状态_ = 任务工作线程状态::已停止;
        }
        return {状态_ == 任务工作线程状态::已停止, 状态_};
    }

    任务工作线程快照 读取快照() const noexcept {
        try {
            std::lock_guard<std::mutex> 锁(锁_);
            return {状态_, 已取出数量_, 已形成待验证材料数量_,
                已拒绝数量_, 最后执行结果_, 最后收束结果_};
        } catch (...) {
            return {任务工作线程状态::内部错误};
        }
    }

private:
    struct 收束槽 final {
        任务强类型执行请求 请求;
        任务动作待验证材料 待验证材料;
        任务动作结果收束结果 最近收束;
    };

    void 运行强类型执行循环() noexcept {
        for (;;) {
            if (收束槽_) {
                const auto 收束 = 结果收束提供者_.收束待验证任务动作结果(
                    收束槽_->待验证材料);
                {
                    std::lock_guard<std::mutex> 锁(锁_);
                    收束槽_->最近收束 = 收束;
                    最后收束结果_ = 收束;
                }
                if (收束.成功() && 收束.工作结果定位) {
                    const auto 提交 = 请求来源_.提交任务工作结果定位(
                        *收束.工作结果定位);
                    if (提交.成功()) {
                        std::lock_guard<std::mutex> 锁(锁_);
                        收束槽_.reset();
                        continue;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            const auto 取出 = 请求来源_.取出强类型任务执行请求();
            if (取出.状态 == 强类型任务执行请求队列状态::已取出
                && 取出.请求) {
                const auto 复核 = 最终复核提供者_.复核即将执行的强类型请求(
                    *取出.请求);
                if (!复核.成功() || !复核.已复核请求) {
                    std::lock_guard<std::mutex> 锁(锁_);
                    ++已取出数量_;
                    ++已拒绝数量_;
                    continue;
                }
                const auto 执行 = 执行提供者_.执行已冻结实例方法(
                    *复核.已复核请求);
                std::lock_guard<std::mutex> 锁(锁_);
                ++已取出数量_;
                最后执行结果_ = 执行;
                if (执行.成功() && 执行.待验证材料) {
                    ++已形成待验证材料数量_;
                    收束槽_ = 收束槽{*复核.已复核请求,
                        *执行.待验证材料, {}};
                } else {
                    ++已拒绝数量_;
                }
                continue;
            }
            if (取出.状态 == 强类型任务执行请求队列状态::内部不一致) {
                std::lock_guard<std::mutex> 锁(锁_);
                状态_ = 任务工作线程状态::内部错误;
                break;
            }
            if (取出.队列已停止) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::lock_guard<std::mutex> 锁(锁_);
        if (状态_ != 任务工作线程状态::内部错误) {
            状态_ = 任务工作线程状态::已停止;
        }
    }

    任务管理线程& 请求来源_;
    任务执行最终当前性复核提供者& 最终复核提供者_;
    实例方法执行提供者& 执行提供者_;
    任务动作结果收束提供者& 结果收束提供者_;
    mutable std::mutex 锁_;
    任务工作线程状态 状态_ = 任务工作线程状态::未启动;
    std::uint64_t 已取出数量_ = 0;
    std::uint64_t 已形成待验证材料数量_ = 0;
    std::uint64_t 已拒绝数量_ = 0;
    std::optional<实例方法执行结果> 最后执行结果_;
    std::optional<任务动作结果收束结果> 最后收束结果_;
    std::optional<收束槽> 收束槽_;
    std::thread 线程_;
};

} // namespace 海中鱼巣
