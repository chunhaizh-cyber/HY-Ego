// 文件规则：本模块只承载 4170 运行期不可变发布记录侧账及其事务参与者，不复制原始值、不写结构仓库、不实现快照或恢复。
module;

#include "../核心/句柄.h"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.参与者.特征批次发布记录;

import 海中鱼巣.核心.执行器.结构写入;

export namespace 海中鱼巣 {

struct 特征批次身份 {
    std::uint64_t 编号 = 0;

    bool 有效() const noexcept { return 编号 != 0; }
};

inline bool operator==(特征批次身份 左, 特征批次身份 右) noexcept {
    return 左.编号 == 右.编号;
}

enum class 特征批次项目种类 : std::uint8_t {
    未建立 = 0,
    初始 = 1,
    换代 = 2
};

struct 特征批次发布项目 {
    std::uint32_t 顺序号 = 0;
    特征批次项目种类 种类 = 特征批次项目种类::未建立;
    节点句柄 特征定义;
    节点句柄 实例槽位;
    节点句柄 新特征值;
    节点句柄 来源;
    关系句柄 写前当前关系;
    节点句柄 写前当前值;
    std::uint64_t 写前原始值版本 = 0;
    关系句柄 发布后新当前关系;

    bool 完整() const noexcept {
        if (!句柄有效(特征定义) || !句柄有效(实例槽位)
            || !句柄有效(新特征值) || !句柄有效(来源)
            || !句柄有效(发布后新当前关系)) return false;
        if (种类 == 特征批次项目种类::初始) {
            return !句柄有效(写前当前关系) && !句柄有效(写前当前值)
                && 写前原始值版本 == 0;
        }
        if (种类 == 特征批次项目种类::换代) {
            return 句柄有效(写前当前关系) && 句柄有效(写前当前值)
                && 写前原始值版本 != 0
                && 写前当前关系 != 发布后新当前关系
                && 写前当前值 != 新特征值;
        }
        return false;
    }
};

inline bool operator==(
    const 特征批次发布项目& 左,
    const 特征批次发布项目& 右) noexcept {
    return 左.顺序号 == 右.顺序号 && 左.种类 == 右.种类
        && 左.特征定义 == 右.特征定义 && 左.实例槽位 == 右.实例槽位
        && 左.新特征值 == 右.新特征值 && 左.来源 == 右.来源
        && 左.写前当前关系 == 右.写前当前关系
        && 左.写前当前值 == 右.写前当前值
        && 左.写前原始值版本 == 右.写前原始值版本
        && 左.发布后新当前关系 == 右.发布后新当前关系;
}

struct 特征批次发布记录 {
    static constexpr std::uint32_t 当前记录格式版本 = 1;

    特征批次身份 批次;
    std::uint32_t 记录格式版本 = 当前记录格式版本;
    std::uint32_t 业务规则版本 = 0;
    节点句柄 宿主;
    std::vector<特征批次发布项目> 项目组;

    bool 完整() const noexcept {
        if (!批次.有效() || 记录格式版本 == 0 || 业务规则版本 == 0
            || !句柄有效(宿主) || 项目组.empty()) return false;
        if (项目组.size() > static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())) {
            return false;
        }
        for (std::size_t 位置 = 0; 位置 < 项目组.size(); ++位置) {
            const auto& 项目 = 项目组[位置];
            if (项目.顺序号 != static_cast<std::uint32_t>(位置) || !项目.完整()) return false;
            for (std::size_t 已有位置 = 0; 已有位置 < 位置; ++已有位置) {
                const auto& 已有 = 项目组[已有位置];
                if (已有.实例槽位 == 项目.实例槽位
                    || 已有.新特征值 == 项目.新特征值
                    || 已有.发布后新当前关系 == 项目.发布后新当前关系) return false;
            }
        }
        return true;
    }
};

inline bool 同一完整语义(
    const 特征批次发布记录& 左,
    const 特征批次发布记录& 右) noexcept {
    return 左.完整() && 右.完整() && 左.批次 == 右.批次
        && 左.记录格式版本 == 右.记录格式版本
        && 左.业务规则版本 == 右.业务规则版本
        && 左.宿主 == 右.宿主 && 左.项目组 == 右.项目组;
}

enum class 特征批次发布记录读取状态 : std::uint8_t {
    未找到 = 1,
    可读 = 2,
    许可竞争 = 3,
    未知格式 = 4,
    内部不一致 = 5
};

struct 特征批次发布记录读取结果 {
    特征批次发布记录读取状态 状态 = 特征批次发布记录读取状态::未找到;
    特征批次发布记录 记录;

    bool 可读() const noexcept {
        return 状态 == 特征批次发布记录读取状态::可读
            && 记录.记录格式版本 == 特征批次发布记录::当前记录格式版本
            && 记录.完整();
    }
};

class 特征批次发布记录账 final {
public:
    特征批次发布记录账() = default;
    特征批次发布记录账(const 特征批次发布记录账&) = delete;
    特征批次发布记录账& operator=(const 特征批次发布记录账&) = delete;
    特征批次发布记录账(特征批次发布记录账&&) = delete;
    特征批次发布记录账& operator=(特征批次发布记录账&&) = delete;

#ifdef HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST
    bool 自检注入已发布记录(特征批次发布记录 记录) {
        if (!记录.批次.有效()) return false;
        std::unique_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
        if (!锁.owns_lock()) return false;
        for (const auto& 已有 : 记录组_) {
            if (已有.记录.批次 == 记录.批次) return false;
        }
        try {
            记录组_.push_back({std::move(记录), true});
        } catch (...) {
            return false;
        }
        return true;
    }

    template <typename 调用类型>
    bool 自检独占期间(调用类型&& 调用) {
        std::unique_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
        if (!锁.owns_lock()) return false;
        try {
            std::forward<调用类型>(调用)();
        } catch (...) {
            return false;
        }
        return true;
    }
#endif

private:
    friend class 特征批次发布记录账访问器;
    friend class 特征批次发布记录事务参与者;

    struct 内部记录 {
        特征批次发布记录 记录;
        bool 已发布 = false;
    };

    mutable std::shared_mutex 锁_;
    std::vector<内部记录> 记录组_;
};

class 特征批次发布记录账访问器 final {
public:
    特征批次发布记录账访问器() = delete;
    explicit 特征批次发布记录账访问器(const 特征批次发布记录账& 账) : 账_(账) {}
    特征批次发布记录账访问器(const 特征批次发布记录账访问器&) = delete;
    特征批次发布记录账访问器& operator=(const 特征批次发布记录账访问器&) = delete;
    特征批次发布记录账访问器(特征批次发布记录账访问器&&) = delete;
    特征批次发布记录账访问器& operator=(特征批次发布记录账访问器&&) = delete;

    特征批次发布记录读取结果 读取(特征批次身份 批次) const {
        特征批次发布记录读取结果 输出;
        if (!批次.有效()) {
            输出.状态 = 特征批次发布记录读取状态::内部不一致;
            return 输出;
        }
        std::shared_lock<std::shared_mutex> 锁(账_.锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 特征批次发布记录读取状态::许可竞争;
            return 输出;
        }
        const 特征批次发布记录账::内部记录* 匹配 = nullptr;
        for (const auto& 当前 : 账_.记录组_) {
            if (当前.记录.批次 != 批次) continue;
            if (匹配 != nullptr) {
                输出.状态 = 特征批次发布记录读取状态::内部不一致;
                return 输出;
            }
            匹配 = &当前;
        }
        if (匹配 == nullptr) return 输出;
        if (!匹配->已发布) {
            输出.状态 = 特征批次发布记录读取状态::内部不一致;
            return 输出;
        }
        if (匹配->记录.记录格式版本 != 特征批次发布记录::当前记录格式版本) {
            输出.状态 = 特征批次发布记录读取状态::未知格式;
            return 输出;
        }
        if (!匹配->记录.完整()) {
            输出.状态 = 特征批次发布记录读取状态::内部不一致;
            return 输出;
        }
        try {
            输出.记录 = 匹配->记录;
        } catch (...) {
            输出.状态 = 特征批次发布记录读取状态::内部不一致;
            return 输出;
        }
        输出.状态 = 特征批次发布记录读取状态::可读;
        return 输出;
    }

private:
    const 特征批次发布记录账& 账_;
};

class 特征批次发布记录事务参与者 final : public 结构写入事务参与者 {
public:
    特征批次发布记录事务参与者() = delete;
    explicit 特征批次发布记录事务参与者(特征批次发布记录账& 账) : 账_(账) {}
    特征批次发布记录事务参与者(const 特征批次发布记录事务参与者&) = delete;
    特征批次发布记录事务参与者& operator=(const 特征批次发布记录事务参与者&) = delete;
    特征批次发布记录事务参与者(特征批次发布记录事务参与者&&) = delete;
    特征批次发布记录事务参与者& operator=(特征批次发布记录事务参与者&&) = delete;

    ~特征批次发布记录事务参与者() override {
        if (!账锁_.owns_lock()) return;
        (void)撤销已登记记录_已加锁();
        账锁_.unlock();
    }

    bool 登记候选(特征批次发布记录 记录) {
        if (阶段_ != 阶段::未登记 || !记录.完整()
            || 记录.记录格式版本 != 特征批次发布记录::当前记录格式版本) return false;
        try {
            待发布记录_.emplace(std::move(记录));
        } catch (...) {
            return false;
        }
        阶段_ = 阶段::已登记;
        return true;
    }

private:
    enum class 阶段 : std::uint8_t {
        未登记,
        已登记,
        已准备,
        已确认待发布,
        已发布,
        已撤销
    };

    结构写入结果 准备提交(const 结构提交准备只读视图& 视图) override {
        if (阶段_ != 阶段::已登记 || !待发布记录_.has_value()) {
            return {结构写入状态::入口拒绝, 0, 0, 0};
        }
        for (const auto& 项目 : 待发布记录_->项目组) {
            if (!视图.节点是本会话候选(项目.新特征值)
                || 视图.读取候选节点类型(项目.新特征值)
                    != std::optional<节点类型>{节点类型::特征值}) {
                return {结构写入状态::内部不一致, 项目.新特征值.节点编号, 0, 0};
            }
            if (项目.种类 == 特征批次项目种类::初始
                && (!视图.节点是本会话候选(项目.实例槽位)
                    || 视图.读取候选节点类型(项目.实例槽位)
                        != std::optional<节点类型>{节点类型::特征})) {
                return {结构写入状态::内部不一致, 项目.实例槽位.节点编号, 0, 0};
            }
        }

        std::unique_lock<std::shared_mutex> 锁(账_.锁_, std::try_to_lock);
        if (!锁.owns_lock()) return {结构写入状态::许可拒绝, 0, 0, 0};
        for (const auto& 已有 : 账_.记录组_) {
            if (已有.记录.批次 == 待发布记录_->批次) {
                return {结构写入状态::许可拒绝, 待发布记录_->批次.编号, 0, 0};
            }
        }
        try {
            账_.记录组_.push_back({std::move(*待发布记录_), false});
        } catch (...) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
        待发布记录_.reset();
        候选位置_ = 账_.记录组_.size() - 1;
        账锁_ = std::move(锁);
        阶段_ = 阶段::已准备;
        return {结构写入状态::候选已确认, 0, 0, 0};
    }

    结构写入结果 确认待发布() override {
        if (阶段_ != 阶段::已准备 || !账锁_.owns_lock() || !当前候选有效_已加锁()) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::已确认待发布;
        return {结构写入状态::候选已确认, 0, 0, 0};
    }

    void 完成发布() noexcept override {
        if (阶段_ != 阶段::已确认待发布 || !账锁_.owns_lock()
            || !当前候选有效_已加锁()) return;
        账_.记录组_[候选位置_].已发布 = true;
        候选位置_ = 无候选位置;
        阶段_ = 阶段::已发布;
        账锁_.unlock();
    }

    结构写入结果 完成撤销() override {
        if (阶段_ == 阶段::已撤销) return {结构写入状态::候选已撤销, 0, 0, 0};
        if (阶段_ == 阶段::已发布) return {结构写入状态::内部不一致, 0, 0, 0};
        bool 完整 = true;
        if (阶段_ == 阶段::已准备 || 阶段_ == 阶段::已确认待发布) {
            if (!账锁_.owns_lock()) return {结构写入状态::内部不一致, 0, 0, 0};
            完整 = 撤销已登记记录_已加锁();
            账锁_.unlock();
        }
        待发布记录_.reset();
        候选位置_ = 无候选位置;
        阶段_ = 阶段::已撤销;
        return {完整 ? 结构写入状态::候选已撤销 : 结构写入状态::内部不一致, 0, 0, 0};
    }

    bool 当前候选有效_已加锁() const noexcept {
        return 候选位置_ < 账_.记录组_.size()
            && !账_.记录组_[候选位置_].已发布
            && 账_.记录组_[候选位置_].记录.完整();
    }

    bool 撤销已登记记录_已加锁() noexcept {
        if (!当前候选有效_已加锁()) return false;
        账_.记录组_.erase(账_.记录组_.begin() + static_cast<std::ptrdiff_t>(候选位置_));
        候选位置_ = 无候选位置;
        return true;
    }

    static constexpr std::size_t 无候选位置 = std::numeric_limits<std::size_t>::max();

    特征批次发布记录账& 账_;
    阶段 阶段_ = 阶段::未登记;
    std::optional<特征批次发布记录> 待发布记录_;
    std::size_t 候选位置_ = 无候选位置;
    std::unique_lock<std::shared_mutex> 账锁_;
};

}
