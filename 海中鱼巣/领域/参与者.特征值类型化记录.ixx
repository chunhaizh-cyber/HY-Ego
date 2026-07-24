// 文件规则：本模块只承载特征值类型化记录仓、事务参与者和值式只读访问器，不解释槽位拓扑、来源或批次语义。
module;

#include "../核心/句柄.h"
#include "节点直接领域合同.数据.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <mutex>
#include <new>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.参与者.特征值类型化记录;

export import 海中鱼巣.核心.执行器.节点直接身份结构写入;
export import 海中鱼巣.领域.参与者.特征批次发布记录;

export {
#include "特征值类型化记录.数据.h"
}

export namespace 海中鱼巣 {

enum class 特征值类型化记录读取状态 : std::uint32_t {
    合法未找到 = 1,
    可读 = 2,
    许可竞争 = 3,
    未知格式 = 4,
    内部不一致 = 5
};

struct 特征值类型化记录读取结果 {
    特征值类型化记录读取状态 状态 = 特征值类型化记录读取状态::合法未找到;
    std::optional<特征值类型化记录> 记录;

    bool 可读() const noexcept {
        return 状态 == 特征值类型化记录读取状态::可读
            && 记录.has_value() && 记录->完整();
    }
};

enum class 特征值类型化记录全量读取状态 : std::uint32_t {
    已形成 = 1,
    许可竞争 = 2,
    资源失败 = 3,
    内部不一致 = 0x8000
};

struct 特征值类型化记录全量读取结果 final {
    特征值类型化记录全量读取状态 状态 =
        特征值类型化记录全量读取状态::内部不一致;
    std::uint64_t 结构版本 = 0;
    std::vector<特征值类型化记录> 记录组;
};

class 特征值类型化记录仓 final {
public:
    特征值类型化记录仓() = default;
    特征值类型化记录仓(const 特征值类型化记录仓&) = delete;
    特征值类型化记录仓& operator=(const 特征值类型化记录仓&) = delete;
    特征值类型化记录仓(特征值类型化记录仓&&) = delete;
    特征值类型化记录仓& operator=(特征值类型化记录仓&&) = delete;

private:
    friend class 特征值类型化记录事务参与者;
    friend class 特征值类型化记录只读访问器;

    struct 内部记录 {
        特征值类型化记录 记录;
        bool 已发布 = false;
    };

    mutable std::shared_mutex 锁_;
    std::vector<内部记录> 记录组_;
    std::uint64_t 结构版本_ = 1;
};

class 特征值类型化记录只读访问器 final {
public:
    explicit 特征值类型化记录只读访问器(const 特征值类型化记录仓& 记录仓) noexcept
        : 记录仓_(记录仓) {
    }

    特征值类型化记录读取结果 读取当前记录(节点句柄 特征值) const {
        特征值类型化记录读取结果 输出;
        if (!句柄有效(特征值)) {
            输出.状态 = 特征值类型化记录读取状态::内部不一致;
            return 输出;
        }
        std::shared_lock<std::shared_mutex> 锁(记录仓_.锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 特征值类型化记录读取状态::许可竞争;
            return 输出;
        }

        const 特征值类型化记录* 当前 = nullptr;
        for (const auto& 候选 : 记录仓_.记录组_) {
            if (!候选.已发布 || 候选.记录.特征值 != 特征值
                || 候选.记录.状态 != 记录状态::有效) {
                continue;
            }
            if (!候选.记录.完整()) {
                输出.状态 = 特征值类型化记录读取状态::内部不一致;
                return 输出;
            }
            if (当前 != nullptr) {
                输出.状态 = 特征值类型化记录读取状态::内部不一致;
                return 输出;
            }
            当前 = &候选.记录;
        }
        if (当前 == nullptr) return 输出;
        try {
            输出.记录 = *当前;
        } catch (...) {
            输出.状态 = 特征值类型化记录读取状态::内部不一致;
            return 输出;
        }
        输出.状态 = 特征值类型化记录读取状态::可读;
        return 输出;
    }

    特征值类型化记录读取结果 读取审计记录(
        节点句柄 特征值,
        std::uint64_t 原始值版本) const {
        特征值类型化记录读取结果 输出;
        if (!句柄有效(特征值) || 原始值版本 == 0) {
            输出.状态 = 特征值类型化记录读取状态::内部不一致;
            return 输出;
        }
        std::shared_lock<std::shared_mutex> 锁(记录仓_.锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 特征值类型化记录读取状态::许可竞争;
            return 输出;
        }

        const 特征值类型化记录* 匹配 = nullptr;
        for (const auto& 候选 : 记录仓_.记录组_) {
            if (!候选.已发布 || 候选.记录.特征值 != 特征值
                || 候选.记录.原始值版本 != 原始值版本) {
                continue;
            }
            if (!候选.记录.完整() || 匹配 != nullptr) {
                输出.状态 = 特征值类型化记录读取状态::内部不一致;
                return 输出;
            }
            匹配 = &候选.记录;
        }
        if (匹配 == nullptr) return 输出;
        try {
            输出.记录 = *匹配;
        } catch (...) {
            输出.状态 = 特征值类型化记录读取状态::内部不一致;
            return 输出;
        }
        输出.状态 = 特征值类型化记录读取状态::可读;
        return 输出;
    }

    std::uint64_t 读取结构版本() const noexcept {
        std::shared_lock<std::shared_mutex> 锁(记录仓_.锁_, std::try_to_lock);
        return 锁.owns_lock() ? 记录仓_.结构版本_ : 0;
    }

    特征值类型化记录全量读取结果 复制全部已发布记录() const {
        特征值类型化记录全量读取结果 输出;
        std::shared_lock<std::shared_mutex> 锁(记录仓_.锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 特征值类型化记录全量读取状态::许可竞争;
            return 输出;
        }
        if (记录仓_.结构版本_ == 0) return 输出;

        try {
            输出.记录组.reserve(记录仓_.记录组_.size());
            for (const auto& 候选 : 记录仓_.记录组_) {
                if (!候选.已发布 || !候选.记录.完整()
                    || 候选.记录.记录格式版本 != 特征值类型化记录当前格式版本) {
                    输出.记录组.clear();
                    return 输出;
                }
                for (const auto& 已有 : 输出.记录组) {
                    if (已有.特征值 == 候选.记录.特征值
                        && 已有.原始值版本 == 候选.记录.原始值版本) {
                        输出.记录组.clear();
                        return 输出;
                    }
                    if (已有.特征值 == 候选.记录.特征值
                        && 已有.状态 == 记录状态::有效
                        && 候选.记录.状态 == 记录状态::有效) {
                        输出.记录组.clear();
                        return 输出;
                    }
                }
                输出.记录组.push_back(候选.记录);
            }
        } catch (const std::bad_alloc&) {
            输出.记录组.clear();
            输出.状态 = 特征值类型化记录全量读取状态::资源失败;
            return 输出;
        } catch (...) {
            输出.记录组.clear();
            return 输出;
        }

        输出.状态 = 特征值类型化记录全量读取状态::已形成;
        输出.结构版本 = 记录仓_.结构版本_;
        return 输出;
    }

private:
    const 特征值类型化记录仓& 记录仓_;
};

class 特征值类型化记录事务参与者 final : public 节点直接身份结构写入事务参与者 {
public:
    explicit 特征值类型化记录事务参与者(特征值类型化记录仓& 记录仓) noexcept
        : 记录仓_(记录仓) {
    }

    特征值类型化记录事务参与者(const 特征值类型化记录事务参与者&) = delete;
    特征值类型化记录事务参与者& operator=(const 特征值类型化记录事务参与者&) = delete;
    特征值类型化记录事务参与者(特征值类型化记录事务参与者&&) = delete;
    特征值类型化记录事务参与者& operator=(特征值类型化记录事务参与者&&) = delete;
    ~特征值类型化记录事务参与者() override = default;

    bool 登记初始记录(特征值类型化记录规格 规格) {
        if (阶段_ != 阶段::登记中 || !规格.完整() || 规格.原始值版本 != 1) return false;
        const auto 重复 = std::find_if(待发布规格组_.cbegin(), 待发布规格组_.cend(),
            [&规格](const auto& 已有) { return 已有.特征值 == 规格.特征值; });
        if (重复 != 待发布规格组_.cend()) return false;
        try {
            待发布规格组_.push_back(std::move(规格));
        } catch (...) {
            return false;
        }
        return true;
    }

    std::size_t 读取已登记数量() const noexcept { return 待发布规格组_.size(); }

private:
    enum class 阶段 : std::uint8_t {
        登记中,
        已准备,
        已确认待发布,
        已发布,
        已撤销
    };

    节点直接身份结构写入结果 准备提交(
        const 节点直接身份结构提交准备只读视图& 视图) override {
        if (阶段_ != 阶段::登记中 || 待发布规格组_.empty()) {
            return {节点直接身份结构写入状态::入口拒绝, 0, 0, 0};
        }
        for (const auto& 规格 : 待发布规格组_) {
            const auto 稳定主键 = 视图.读取候选节点稳定主键(规格.特征值);
            if (!视图.节点是本会话候选(规格.特征值)
                || 视图.读取候选节点类型(规格.特征值)
                    != std::optional<节点类型>{节点类型::特征值}
                || !稳定主键.has_value() || !稳定主键有效(*稳定主键)
                || 稳定主键->命名域 != static_cast<std::uint64_t>(
                    节点稳定主键命名域::特征值身份)) {
                return {节点直接身份结构写入状态::内部不一致,
                    规格.特征值.节点编号, 0, 0};
            }
        }

        std::unique_lock<std::shared_mutex> 锁(记录仓_.锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            return {节点直接身份结构写入状态::许可拒绝, 0, 0, 0};
        }
        for (const auto& 规格 : 待发布规格组_) {
            const auto 已有数量 = std::count_if(
                记录仓_.记录组_.cbegin(), 记录仓_.记录组_.cend(),
                [&规格](const auto& 已有) {
                    return 已有.已发布 && 已有.记录.特征值 == 规格.特征值
                        && 已有.记录.状态 == 记录状态::有效;
                });
            if (已有数量 > 1) {
                return {节点直接身份结构写入状态::内部不一致,
                    规格.特征值.节点编号, 0, 0};
            }
            if (已有数量 == 1) {
                return {节点直接身份结构写入状态::版本漂移,
                    规格.特征值.节点编号, 规格.特征值.版本号, 规格.特征值.版本号};
            }
        }

        try {
            记录仓_.记录组_.reserve(记录仓_.记录组_.size() + 待发布规格组_.size());
            已插入位置组_.reserve(待发布规格组_.size());
            for (const auto& 规格 : 待发布规格组_) {
                特征值类型化记录 记录{
                    规格.特征值,
                    特征值类型化记录当前格式版本,
                    规格.原始值版本,
                    记录状态::有效,
                    规格.原始值};
                if (!记录.完整()) {
                    (void)撤销已插入记录_已加锁();
                    return {节点直接身份结构写入状态::内部不一致,
                        规格.特征值.节点编号, 0, 0};
                }
                记录仓_.记录组_.push_back({std::move(记录), false});
                已插入位置组_.push_back(记录仓_.记录组_.size() - 1);
            }
        } catch (...) {
            (void)撤销已插入记录_已加锁();
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }

        记录锁_ = std::move(锁);
        阶段_ = 阶段::已准备;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    节点直接身份结构写入结果 确认待发布() override {
        if (阶段_ != 阶段::已准备 || !记录锁_.owns_lock()
            || !候选组完整_已加锁()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::已确认待发布;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    void 完成发布() noexcept override {
        if (阶段_ != 阶段::已确认待发布 || !记录锁_.owns_lock()
            || !候选组完整_已加锁()) {
            std::terminate();
        }
        for (const auto 位置 : 已插入位置组_) 记录仓_.记录组_[位置].已发布 = true;
        ++记录仓_.结构版本_;
        已插入位置组_.clear();
        阶段_ = 阶段::已发布;
        记录锁_.unlock();
    }

    节点直接身份结构写入结果 完成撤销() override {
        if (阶段_ == 阶段::已撤销) {
            return {节点直接身份结构写入状态::候选已撤销, 0, 0, 0};
        }
        if (阶段_ == 阶段::已发布) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        bool 完整 = true;
        if (阶段_ == 阶段::已准备 || 阶段_ == 阶段::已确认待发布) {
            if (!记录锁_.owns_lock()) {
                return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
            }
            完整 = 撤销已插入记录_已加锁();
            记录锁_.unlock();
        }
        待发布规格组_.clear();
        阶段_ = 阶段::已撤销;
        return {完整 ? 节点直接身份结构写入状态::候选已撤销
                         : 节点直接身份结构写入状态::内部不一致,
            0, 0, 0};
    }

    bool 候选组完整_已加锁() const noexcept {
        if (已插入位置组_.size() != 待发布规格组_.size()) return false;
        for (std::size_t 序号 = 0; 序号 < 已插入位置组_.size(); ++序号) {
            const auto 位置 = 已插入位置组_[序号];
            if (位置 >= 记录仓_.记录组_.size()
                || 记录仓_.记录组_[位置].已发布
                || !记录仓_.记录组_[位置].记录.完整()
                || 记录仓_.记录组_[位置].记录.特征值
                    != 待发布规格组_[序号].特征值) {
                return false;
            }
        }
        return true;
    }

    bool 撤销已插入记录_已加锁() noexcept {
        bool 完整 = true;
        for (auto 位置 = 已插入位置组_.rbegin(); 位置 != 已插入位置组_.rend(); ++位置) {
            if (*位置 >= 记录仓_.记录组_.size()
                || 记录仓_.记录组_[*位置].已发布) {
                完整 = false;
                continue;
            }
            记录仓_.记录组_.erase(
                记录仓_.记录组_.begin() + static_cast<std::ptrdiff_t>(*位置));
        }
        已插入位置组_.clear();
        return 完整;
    }

    特征值类型化记录仓& 记录仓_;
    阶段 阶段_ = 阶段::登记中;
    std::vector<特征值类型化记录规格> 待发布规格组_;
    std::vector<std::size_t> 已插入位置组_;
    std::unique_lock<std::shared_mutex> 记录锁_;
};

}
