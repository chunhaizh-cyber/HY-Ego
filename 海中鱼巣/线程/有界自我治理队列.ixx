// 文件规则：有界自我治理队列只承载非权威治理材料、等待和整批冻结；不得调用领域入口、写机器结构或持锁执行外部操作。
module;

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

export module 海中鱼巣.线程.有界自我治理队列;

import 海中鱼巣.线程.自我治理消息协议;

export namespace 海中鱼巣 {

using 自我治理任务句柄 = decltype(自我治理只读材料句柄组{}.目标任务);

enum class 自我治理邮箱状态 : std::uint32_t {
    运行中 = 1,
    已停止 = 2
};

enum class 自我治理提交状态 : std::uint32_t {
    已拒绝 = 0,
    已提交 = 1,
    重复材料 = 2,
    内部不一致 = 3
};

enum class 自我治理冻结状态 : std::uint32_t {
    已冻结 = 1,
    等待超时 = 2,
    已停止 = 3,
    内部不一致 = 4
};

enum class 自我治理队列拒绝原因 : std::uint32_t {
    无 = 0,
    容量为零 = 1,
    协议拒绝 = 2,
    协议内部不一致 = 3,
    重复材料 = 4,
    队列已满 = 5,
    队列已停止 = 6,
    等待超时 = 7,
    空批次 = 8
};

enum class 自我治理停止结果 : std::uint32_t {
    已停止并唤醒 = 1,
    已经停止 = 2
};

struct 自我治理提交结果 {
    自我治理提交状态 状态 = 自我治理提交状态::已拒绝;
    自我治理队列拒绝原因 拒绝原因 = 自我治理队列拒绝原因::无;
    自我治理准入结果 协议结果;
};

struct 自我治理邮箱快照 {
    std::uint64_t 容量 = 0;
    std::uint64_t 待处理数量 = 0;
    std::uint64_t 已见幂等键数量 = 0;
    std::uint64_t 同任务顺序数量 = 0;
    std::uint64_t 已冻结批次数量 = 0;
    自我治理邮箱状态 状态 = 自我治理邮箱状态::运行中;
};

class 有界自我治理邮箱;

class 自我治理冻结批次 {
public:
    std::uint64_t 批次序号() const {
        return 批次序号_;
    }

    std::uint64_t 数量() const {
        return static_cast<std::uint64_t>(消息组_.size());
    }

    const std::vector<自我治理消息>& 读取消息组() const {
        return 消息组_;
    }

private:
    friend class 有界自我治理邮箱;

    std::uint64_t 批次序号_ = 0;
    std::vector<自我治理消息> 消息组_;
};

struct 自我治理冻结结果 {
    自我治理冻结状态 状态 = 自我治理冻结状态::内部不一致;
    自我治理队列拒绝原因 拒绝原因 = 自我治理队列拒绝原因::空批次;
    自我治理冻结批次 批次;
};

class 有界自我治理邮箱 {
public:
    explicit 有界自我治理邮箱(std::uint64_t 容量)
        : 容量上限_(容量) {
    }

    有界自我治理邮箱(const 有界自我治理邮箱&) = delete;
    有界自我治理邮箱& operator=(const 有界自我治理邮箱&) = delete;

    自我治理提交结果 提交(const 自我治理消息& 消息, std::uint64_t 当前时间戳) {
        std::unique_lock<std::mutex> 锁(队列锁_);
        if (容量上限_ == 0) {
            return 拒绝提交(自我治理队列拒绝原因::容量为零);
        }

        const auto 准入上下文 = 构造准入上下文(消息, 当前时间戳);
        const auto 协议结果 = 复核自我治理消息(消息, 准入上下文);
        if (协议结果.状态 == 自我治理准入状态::已拒绝) {
            return {自我治理提交状态::已拒绝, 自我治理队列拒绝原因::协议拒绝, 协议结果};
        }
        if (协议结果.状态 == 自我治理准入状态::重复材料) {
            return {自我治理提交状态::重复材料, 自我治理队列拒绝原因::重复材料, 协议结果};
        }
        if (协议结果.状态 == 自我治理准入状态::内部不一致) {
            return {自我治理提交状态::内部不一致, 自我治理队列拒绝原因::协议内部不一致, 协议结果};
        }
        if (协议结果.状态 != 自我治理准入状态::可准入) {
            return {自我治理提交状态::内部不一致, 自我治理队列拒绝原因::协议内部不一致, 协议结果};
        }

        const bool 是停止消息 = 消息.类型 == 自我治理消息类型::停止请求;
        if (状态_ == 自我治理邮箱状态::已停止) {
            return {自我治理提交状态::已拒绝, 自我治理队列拒绝原因::队列已停止, 协议结果};
        }

        if (是停止消息) {
            while (待处理消息_.size() >= 容量上限_ && !待处理消息_.empty()) {
                待处理消息_.pop_back();
            }
            记录提交索引(消息);
            待处理消息_.push_front(消息);
            状态_ = 自我治理邮箱状态::已停止;
            锁.unlock();
            等待条件_.notify_all();
            return {自我治理提交状态::已提交, 自我治理队列拒绝原因::无, 协议结果};
        }

        if (待处理消息_.size() >= 容量上限_) {
            return {自我治理提交状态::已拒绝, 自我治理队列拒绝原因::队列已满, 协议结果};
        }

        记录提交索引(消息);
        按优先级插入(消息);
        锁.unlock();
        等待条件_.notify_one();
        return {自我治理提交状态::已提交, 自我治理队列拒绝原因::无, 协议结果};
    }

    自我治理冻结结果 等待并冻结下一批(std::chrono::milliseconds 最大等待时间) {
        std::unique_lock<std::mutex> 锁(队列锁_);
        const bool 已唤醒 = 等待条件_.wait_for(锁, 最大等待时间, [this]() {
            return !待处理消息_.empty() || 状态_ == 自我治理邮箱状态::已停止;
        });
        if (!已唤醒) {
            return {自我治理冻结状态::等待超时, 自我治理队列拒绝原因::等待超时, {}};
        }
        if (待处理消息_.empty()) {
            if (状态_ == 自我治理邮箱状态::已停止) {
                return {自我治理冻结状态::已停止, 自我治理队列拒绝原因::队列已停止, {}};
            }
            return {自我治理冻结状态::内部不一致, 自我治理队列拒绝原因::空批次, {}};
        }

        自我治理冻结结果 结果;
        结果.状态 = 自我治理冻结状态::已冻结;
        结果.拒绝原因 = 自我治理队列拒绝原因::无;
        结果.批次.批次序号_ = ++已冻结批次数量_;
        结果.批次.消息组_.reserve(待处理消息_.size());
        while (!待处理消息_.empty()) {
            结果.批次.消息组_.push_back(std::move(待处理消息_.front()));
            待处理消息_.pop_front();
        }
        return 结果;
    }

    自我治理停止结果 请求停止并唤醒() {
        std::lock_guard<std::mutex> 锁(队列锁_);
        if (状态_ == 自我治理邮箱状态::已停止) {
            等待条件_.notify_all();
            return 自我治理停止结果::已经停止;
        }
        状态_ = 自我治理邮箱状态::已停止;
        等待条件_.notify_all();
        return 自我治理停止结果::已停止并唤醒;
    }

    自我治理邮箱快照 读取快照() const {
        std::lock_guard<std::mutex> 锁(队列锁_);
        return {
            容量上限_,
            static_cast<std::uint64_t>(待处理消息_.size()),
            static_cast<std::uint64_t>(已见幂等消息_.size()),
            static_cast<std::uint64_t>(同任务最新序号_.size()),
            已冻结批次数量_,
            状态_
        };
    }

private:
    struct 节点句柄哈希 {
        std::size_t operator()(const 自我治理任务句柄& 句柄) const noexcept {
            std::size_t 结果 = std::hash<std::uint64_t>{}(句柄.仓库编号);
            结果 ^= std::hash<std::uint64_t>{}(句柄.节点编号)
                + static_cast<std::size_t>(0x9e3779b9U) + (结果 << 6U) + (结果 >> 2U);
            结果 ^= std::hash<std::uint32_t>{}(句柄.版本号)
                + static_cast<std::size_t>(0x9e3779b9U) + (结果 << 6U) + (结果 >> 2U);
            return 结果;
        }
    };

    struct 节点句柄相等 {
        bool operator()(const 自我治理任务句柄& 左, const 自我治理任务句柄& 右) const noexcept {
            return 左.仓库编号 == 右.仓库编号
                && 左.节点编号 == 右.节点编号
                && 左.版本号 == 右.版本号;
        }
    };

    std::uint64_t 容量上限_ = 0;
    mutable std::mutex 队列锁_;
    std::condition_variable 等待条件_;
    std::deque<自我治理消息> 待处理消息_;
    std::unordered_map<std::uint64_t, 自我治理消息> 已见幂等消息_;
    std::unordered_map<自我治理任务句柄, std::uint64_t, 节点句柄哈希, 节点句柄相等> 同任务最新序号_;
    std::uint64_t 已冻结批次数量_ = 0;
    自我治理邮箱状态 状态_ = 自我治理邮箱状态::运行中;

    自我治理提交结果 拒绝提交(自我治理队列拒绝原因 原因) const {
        return {自我治理提交状态::已拒绝, 原因, {}};
    }

    自我治理准入上下文 构造准入上下文(
        const 自我治理消息& 消息,
        std::uint64_t 当前时间戳) const {
        自我治理准入上下文 上下文;
        上下文.当前时间戳 = 当前时间戳;
        const auto 幂等位置 = 已见幂等消息_.find(消息.幂等键);
        if (幂等位置 != 已见幂等消息_.end()) {
            上下文.同幂等键既有消息 = 幂等位置->second;
        }
        if (消息.来源任务序号 != 0
            && !自我治理句柄为空(消息.句柄组.目标任务)
            && 自我治理可选句柄形状有效(消息.句柄组.目标任务)) {
            const auto 顺序位置 = 同任务最新序号_.find(消息.句柄组.目标任务);
            if (顺序位置 != 同任务最新序号_.end()) {
                上下文.同任务目标 = 消息.句柄组.目标任务;
                上下文.同任务最新序号 = 顺序位置->second;
            }
        }
        return 上下文;
    }

    void 记录提交索引(const 自我治理消息& 消息) {
        已见幂等消息_.emplace(消息.幂等键, 消息);
        if (消息.来源任务序号 != 0
            && !自我治理句柄为空(消息.句柄组.目标任务)
            && 自我治理可选句柄形状有效(消息.句柄组.目标任务)) {
            auto& 最新序号 = 同任务最新序号_[消息.句柄组.目标任务];
            if (消息.来源任务序号 > 最新序号) {
                最新序号 = 消息.来源任务序号;
            }
        }
    }

    void 按优先级插入(const 自我治理消息& 消息) {
        const auto 优先级值 = static_cast<std::uint32_t>(消息.优先级);
        auto 位置 = 待处理消息_.begin();
        while (位置 != 待处理消息_.end()
            && static_cast<std::uint32_t>(位置->优先级) >= 优先级值) {
            ++位置;
        }
        待处理消息_.insert(位置, 消息);
    }
};

}
