// 文件规则：D455 帧材料缓存只承载进程内有界候选与已发布只读材料；不得调用 SDK、消息队列、仓库或领域服务。
module;

#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

export module 海中鱼巣.线程.缓存.D455帧材料;
import 海中鱼巣.适配.协议.D455采样材料;

export namespace 海中鱼巣 {

enum class D455缓存拒绝原因 : std::uint8_t {
    无 = 0,
    配置无效 = 1,
    已停止接收 = 2,
    材料无效 = 3,
    单帧超限 = 4,
    条数已满 = 5,
    总字节已满 = 6,
    材料已过期 = 7,
    目标不存在 = 8,
    版本不匹配 = 9,
    候选能力不匹配 = 10,
    状态不匹配 = 11,
    内部计数不一致 = 12
};

struct D455帧材料缓存配置 {
    std::size_t 最大条数 = 0;
    std::uint64_t 最大总字节数 = 0;
    std::uint64_t 单帧最大字节数 = 0;
    std::uint64_t 最大材料年龄微秒 = 0;
};

struct D455帧材料句柄 {
    std::uint64_t 材料编号 = 0;
    std::uint64_t 版本 = 0;

    bool 有效() const noexcept {
        return 材料编号 != 0 && 版本 != 0;
    }
};

struct D455缓存快照 {
    bool 配置有效 = false;
    bool 正在接收 = false;
    std::size_t 候选数量 = 0;
    std::size_t 已发布数量 = 0;
    std::uint64_t 当前总字节数 = 0;
    std::uint64_t 下一个材料编号 = 0;
};

struct D455缓存读取结果 {
    bool 成功 = false;
    bool 追根因 = false;
    D455缓存拒绝原因 拒绝原因 = D455缓存拒绝原因::无;
    D455帧材料句柄 句柄;
    std::shared_ptr<const D455同步帧材料> 材料;
    std::uint64_t 字节数 = 0;
};

struct D455缓存变更结果 {
    bool 成功 = false;
    bool 追根因 = false;
    D455缓存拒绝原因 拒绝原因 = D455缓存拒绝原因::无;
    D455帧材料句柄 句柄;
};

class D455帧材料缓存 {
public:
    class D455帧材料候选 {
    public:
        D455帧材料候选() = delete;
        D455帧材料候选(const D455帧材料候选&) = delete;
        D455帧材料候选& operator=(const D455帧材料候选&) = delete;
        D455帧材料候选(D455帧材料候选&& 来源) noexcept
            : 所属缓存_(std::exchange(来源.所属缓存_, nullptr)),
              句柄_(std::exchange(来源.句柄_, {})),
              能力编号_(std::exchange(来源.能力编号_, 0)) {
        }

        D455帧材料候选& operator=(D455帧材料候选&& 来源) noexcept {
            if (this != &来源) {
                所属缓存_ = std::exchange(来源.所属缓存_, nullptr);
                句柄_ = std::exchange(来源.句柄_, {});
                能力编号_ = std::exchange(来源.能力编号_, 0);
            }
            return *this;
        }

    private:
        friend class D455帧材料缓存;

        D455帧材料候选(
            const D455帧材料缓存* 所属缓存,
            D455帧材料句柄 句柄,
            std::uint64_t 能力编号) noexcept
            : 所属缓存_(所属缓存), 句柄_(句柄), 能力编号_(能力编号) {
        }

        const D455帧材料缓存* 所属缓存_ = nullptr;
        D455帧材料句柄 句柄_;
        std::uint64_t 能力编号_ = 0;
    };

    struct D455候选准备结果 {
        bool 成功 = false;
        bool 追根因 = false;
        D455缓存拒绝原因 拒绝原因 = D455缓存拒绝原因::无;
        std::optional<D455帧材料候选> 候选;
    };

    explicit D455帧材料缓存(D455帧材料缓存配置 配置) noexcept
        : 配置_(配置), 配置有效_(配置.最大条数 != 0
            && 配置.最大总字节数 != 0
            && 配置.单帧最大字节数 != 0
            && 配置.最大材料年龄微秒 != 0) {
    }

    D455帧材料缓存(const D455帧材料缓存&) = delete;
    D455帧材料缓存& operator=(const D455帧材料缓存&) = delete;
    D455帧材料缓存(D455帧材料缓存&&) = delete;
    D455帧材料缓存& operator=(D455帧材料缓存&&) = delete;

    D455候选准备结果 准备帧材料候选(
        std::shared_ptr<const D455同步帧材料> 材料,
        std::uint64_t 当前单调时间戳) {
        if (!配置有效_) {
            return {false, false, D455缓存拒绝原因::配置无效, std::nullopt};
        }
        if (!材料 || !D455同步帧材料有效(*材料) || 当前单调时间戳 == 0) {
            return {false, false, D455缓存拒绝原因::材料无效, std::nullopt};
        }
        if (已过期(*材料, 当前单调时间戳)) {
            return {false, false, D455缓存拒绝原因::材料已过期, std::nullopt};
        }
        const auto 字节数 = 计算D455同步帧字节数(*材料);
        if (字节数 == 0 || 字节数 > 配置_.单帧最大字节数) {
            return {false, false, D455缓存拒绝原因::单帧超限, std::nullopt};
        }

        std::scoped_lock 锁(互斥_);
        if (!正在接收_) {
            return {false, false, D455缓存拒绝原因::已停止接收, std::nullopt};
        }
        if (条目组_.size() >= 配置_.最大条数) {
            return {false, false, D455缓存拒绝原因::条数已满, std::nullopt};
        }
        if (当前总字节数_ > 配置_.最大总字节数
            || 字节数 > 配置_.最大总字节数 - 当前总字节数_) {
            return {false, false, D455缓存拒绝原因::总字节已满, std::nullopt};
        }
        if (下一个材料编号_ == 0 || 下一个能力编号_ == 0) {
            return {false, true, D455缓存拒绝原因::内部计数不一致, std::nullopt};
        }

        const D455帧材料句柄 句柄{下一个材料编号_++, 1};
        const auto 能力编号 = 下一个能力编号_++;
        条目 新条目;
        新条目.句柄 = 句柄;
        新条目.状态 = 条目状态::未发布候选;
        新条目.能力编号 = 能力编号;
        新条目.材料 = std::move(材料);
        新条目.字节数 = 字节数;
        const auto [位置, 已插入] = 条目组_.emplace(句柄.材料编号, std::move(新条目));
        if (!已插入) {
            return {false, true, D455缓存拒绝原因::内部计数不一致, std::nullopt};
        }
        当前总字节数_ += 字节数;
        D455候选准备结果 结果;
        结果.成功 = true;
        结果.候选 = D455帧材料候选(this, 句柄, 能力编号);
        return 结果;
    }

    D455缓存读取结果 读取候选(const D455帧材料候选& 候选) const {
        std::scoped_lock 锁(互斥_);
        const auto* 条目值 = 查找候选条目(候选);
        if (条目值 == nullptr) {
            return {false, true, D455缓存拒绝原因::候选能力不匹配, {}, {}, 0};
        }
        if (条目值->状态 != 条目状态::未发布候选) {
            return {false, true, D455缓存拒绝原因::状态不匹配, 条目值->句柄, {}, 0};
        }
        return {true, false, D455缓存拒绝原因::无,
            条目值->句柄, 条目值->材料, 条目值->字节数};
    }

    D455缓存变更结果 确认发布帧材料(const D455帧材料候选& 候选) {
        std::scoped_lock 锁(互斥_);
        auto* 条目值 = 查找候选条目(候选);
        if (条目值 == nullptr) {
            return {false, true, D455缓存拒绝原因::候选能力不匹配, {}};
        }
        if (条目值->状态 != 条目状态::未发布候选) {
            return {false, true, D455缓存拒绝原因::状态不匹配, 条目值->句柄};
        }
        条目值->状态 = 条目状态::已发布;
        return {true, false, D455缓存拒绝原因::无, 条目值->句柄};
    }

    D455缓存变更结果 精确撤销帧材料(D455帧材料句柄 句柄) {
        if (!句柄.有效()) {
            return {false, false, D455缓存拒绝原因::版本不匹配, 句柄};
        }
        std::scoped_lock 锁(互斥_);
        const auto 位置 = 条目组_.find(句柄.材料编号);
        if (位置 == 条目组_.end()) {
            return {false, false, D455缓存拒绝原因::目标不存在, 句柄};
        }
        if (位置->second.句柄.版本 != 句柄.版本) {
            return {false, false, D455缓存拒绝原因::版本不匹配, 句柄};
        }
        if (当前总字节数_ < 位置->second.字节数) {
            return {false, true, D455缓存拒绝原因::内部计数不一致, 句柄};
        }
        当前总字节数_ -= 位置->second.字节数;
        条目组_.erase(位置);
        return {true, false, D455缓存拒绝原因::无, 句柄};
    }

    D455缓存读取结果 按编号版本读取(D455帧材料句柄 句柄) const {
        if (!句柄.有效()) {
            return {false, false, D455缓存拒绝原因::版本不匹配, 句柄, {}, 0};
        }
        std::scoped_lock 锁(互斥_);
        const auto 位置 = 条目组_.find(句柄.材料编号);
        if (位置 == 条目组_.end()) {
            return {false, false, D455缓存拒绝原因::目标不存在, 句柄, {}, 0};
        }
        if (位置->second.句柄.版本 != 句柄.版本) {
            return {false, false, D455缓存拒绝原因::版本不匹配, 句柄, {}, 0};
        }
        if (位置->second.状态 != 条目状态::已发布) {
            return {false, false, D455缓存拒绝原因::状态不匹配, 句柄, {}, 0};
        }
        return {true, false, D455缓存拒绝原因::无,
            句柄, 位置->second.材料, 位置->second.字节数};
    }

    std::size_t 丢弃过期材料(std::uint64_t 当前单调时间戳) {
        if (当前单调时间戳 == 0) {
            return 0;
        }
        std::scoped_lock 锁(互斥_);
        std::vector<std::uint64_t> 待删除编号组;
        for (const auto& [编号, 条目值] : 条目组_) {
            if (条目值.材料 && 已过期(*条目值.材料, 当前单调时间戳)) {
                待删除编号组.push_back(编号);
            }
        }
        std::size_t 已删除数量 = 0;
        for (const auto 编号 : 待删除编号组) {
            const auto 位置 = 条目组_.find(编号);
            if (位置 == 条目组_.end() || 当前总字节数_ < 位置->second.字节数) {
                continue;
            }
            当前总字节数_ -= 位置->second.字节数;
            条目组_.erase(位置);
            ++已删除数量;
        }
        return 已删除数量;
    }

    D455缓存快照 读取缓存快照() const {
        std::scoped_lock 锁(互斥_);
        D455缓存快照 快照;
        快照.配置有效 = 配置有效_;
        快照.正在接收 = 正在接收_;
        快照.当前总字节数 = 当前总字节数_;
        快照.下一个材料编号 = 下一个材料编号_;
        for (const auto& [编号, 条目值] : 条目组_) {
            (void)编号;
            if (条目值.状态 == 条目状态::未发布候选) {
                ++快照.候选数量;
            }
            else if (条目值.状态 == 条目状态::已发布) {
                ++快照.已发布数量;
            }
        }
        return 快照;
    }

    void 停止接收() noexcept {
        std::scoped_lock 锁(互斥_);
        正在接收_ = false;
    }

private:
    enum class 条目状态 : std::uint8_t {
        未发布候选 = 1,
        已发布 = 2
    };

    struct 条目 {
        D455帧材料句柄 句柄;
        条目状态 状态 = 条目状态::未发布候选;
        std::uint64_t 能力编号 = 0;
        std::shared_ptr<const D455同步帧材料> 材料;
        std::uint64_t 字节数 = 0;
    };

    bool 已过期(const D455同步帧材料& 材料, std::uint64_t 当前单调时间戳) const noexcept {
        return 当前单调时间戳 > 材料.宿主接收单调时间戳
            && 当前单调时间戳 - 材料.宿主接收单调时间戳 > 配置_.最大材料年龄微秒;
    }

    const 条目* 查找候选条目(const D455帧材料候选& 候选) const noexcept {
        if (候选.所属缓存_ != this || !候选.句柄_.有效() || 候选.能力编号_ == 0) {
            return nullptr;
        }
        const auto 位置 = 条目组_.find(候选.句柄_.材料编号);
        if (位置 == 条目组_.end()
            || 位置->second.句柄.版本 != 候选.句柄_.版本
            || 位置->second.能力编号 != 候选.能力编号_) {
            return nullptr;
        }
        return &位置->second;
    }

    条目* 查找候选条目(const D455帧材料候选& 候选) noexcept {
        return const_cast<条目*>(std::as_const(*this).查找候选条目(候选));
    }

    D455帧材料缓存配置 配置_;
    bool 配置有效_ = false;
    mutable std::mutex 互斥_;
    bool 正在接收_ = true;
    std::uint64_t 下一个材料编号_ = 1;
    std::uint64_t 下一个能力编号_ = 1;
    std::uint64_t 当前总字节数_ = 0;
    std::unordered_map<std::uint64_t, 条目> 条目组_;
};

}
