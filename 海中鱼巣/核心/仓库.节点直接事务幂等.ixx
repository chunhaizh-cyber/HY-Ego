// 文件规则：本模块只承载节点直接事务幂等不可变记录及独立持久证据状态侧账。
module;

#include "节点直接结构合同.数据.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>

export module 海中鱼巣.核心.仓库.节点直接事务幂等;

export namespace 海中鱼巣 {

class 节点直接事务幂等仓库;

class 节点直接事务幂等候选 final {
public:
    节点直接事务幂等候选(节点直接事务幂等候选&& 其它) noexcept
        : 仓库_(其它.仓库_), 事务序号_(其它.事务序号_), 身份_(其它.身份_), 阶段_(其它.阶段_) {
        其它.仓库_ = nullptr;
        其它.事务序号_ = 0;
        其它.阶段_ = 节点直接仓候选阶段::已移动;
    }
    节点直接事务幂等候选(const 节点直接事务幂等候选&) = delete;
    节点直接事务幂等候选& operator=(const 节点直接事务幂等候选&) = delete;
    节点直接仓候选阶段 读取阶段() const noexcept { return 阶段_; }
    std::optional<节点直接事务幂等记录> 读取候选记录() const;

private:
    friend class 节点直接事务幂等仓库;
    节点直接事务幂等候选(
        节点直接事务幂等仓库* 仓库,
        std::uint64_t 事务序号,
        节点直接事务幂等身份 身份) noexcept
        : 仓库_(仓库), 事务序号_(事务序号), 身份_(身份) {}

    节点直接事务幂等仓库* 仓库_ = nullptr;
    std::uint64_t 事务序号_ = 0;
    节点直接事务幂等身份 身份_;
    节点直接仓候选阶段 阶段_ = 节点直接仓候选阶段::持有;
};

struct 节点直接事务幂等候选结果 {
    节点直接仓候选操作状态 状态 = 节点直接仓候选操作状态::入口拒绝;
    std::optional<节点直接事务幂等候选> 候选;
};

class 节点直接事务幂等仓库 final {
public:
    std::optional<节点直接事务幂等记录> 读取(节点直接事务幂等身份 身份) const {
        if (!节点直接事务幂等身份完整(身份)) return std::nullopt;
        std::shared_lock 锁(记录锁_);
        const auto 位置 = 查找记录_(身份);
        return 位置 != 记录组_.end() && 位置->已发布
            ? std::optional<节点直接事务幂等记录>{位置->记录}
            : std::nullopt;
    }

    std::optional<节点直接持久证据侧账记录> 读取持久证据侧账(
        节点直接事务幂等身份 身份) const noexcept {
        try {
            std::shared_lock 锁(侧账锁_);
            const auto 位置 = 查找侧账_(身份);
            return 位置 == 侧账组_.end()
                ? std::nullopt : std::optional<节点直接持久证据侧账记录>{*位置};
        } catch (...) {
            return std::nullopt;
        }
    }

    持久证据状态 读取持久证据状态(节点直接事务幂等身份 身份) const noexcept {
        const auto 侧账 = 读取持久证据侧账(身份);
        return 侧账 ? 侧账->状态 : 持久证据状态::不适用;
    }

    bool 建立临时持久证据侧账(
        节点直接事务幂等身份 身份, std::uint64_t 尝试序号) {
        if (!节点直接事务幂等身份完整(身份) || 尝试序号 == 0) return false;
        std::unique_lock 锁(侧账锁_);
        if (查找侧账_(身份) != 侧账组_.end()) return false;
        侧账组_.push_back({身份, 尝试序号, 持久证据状态::待持久化});
        return true;
    }

    bool 清除临时持久证据侧账(
        节点直接事务幂等身份 身份, std::uint64_t 尝试序号) noexcept {
        try {
            std::unique_lock 锁(侧账锁_);
            const auto 位置 = 查找侧账_(身份);
            if (位置 == 侧账组_.end() || 位置->尝试序号 != 尝试序号
                || 位置->状态 != 持久证据状态::待持久化) return false;
            侧账组_.erase(位置);
            return true;
        } catch (...) { return false; }
    }

    bool 单调记录持久证据状态(
        节点直接事务幂等身份 身份,
        持久证据状态 状态) noexcept {
        const auto 侧账 = 读取持久证据侧账(身份);
        return 侧账 && 单调记录持久证据状态(身份, 侧账->尝试序号, 状态);
    }

    bool 单调记录持久证据状态(
        节点直接事务幂等身份 身份,
        std::uint64_t 尝试序号,
        持久证据状态 状态) noexcept {
        if (!节点直接事务幂等身份完整(身份) || 尝试序号 == 0) return false;
        try {
            std::unique_lock 锁(侧账锁_);
            auto 位置 = 查找侧账_(身份);
            if (位置 == 侧账组_.end()) {
                return false;
            }
            if (位置->尝试序号 != 尝试序号) return false;
            if (位置->状态 == 状态) return true;
            const bool 允许 = (位置->状态 == 持久证据状态::待持久化
                    && (状态 == 持久证据状态::已与内存代次一致
                        || 状态 == 持久证据状态::持久化结果未知))
                || (位置->状态 == 持久证据状态::持久化结果未知
                    && 状态 == 持久证据状态::已与内存代次一致)
                || (位置->状态 != 持久证据状态::持久证据损坏
                    && 状态 == 持久证据状态::持久证据损坏);
            if (!允许) return false;
            位置->状态 = 状态;
            return true;
        } catch (...) {
            return false;
        }
    }

    节点直接事务幂等候选结果 结构化建立记录未发布候选(
        const 节点直接事务幂等记录& 计划记录,
        std::uint64_t 事务序号) {
        节点直接事务幂等候选结果 结果;
        if (事务序号 == 0 || !记录完整_(计划记录)) return 结果;
        std::unique_lock 锁(记录锁_);
        const auto 位置 = 查找记录_(计划记录.幂等身份);
        if (位置 != 记录组_.end()) {
            结果.状态 = 位置->记录 == 计划记录
                ? 节点直接仓候选操作状态::精确同义
                : 节点直接仓候选操作状态::内部不一致;
            return 结果;
        }
        try {
            记录组_.push_back({计划记录, false, 事务序号});
            结果.候选.emplace(节点直接事务幂等候选{
                this, 事务序号, 计划记录.幂等身份});
            结果.状态 = 节点直接仓候选操作状态::已形成候选;
        } catch (...) {
            const auto 新位置 = 查找记录_(计划记录.幂等身份);
            if (新位置 != 记录组_.end() && !新位置->已发布 && 新位置->事务序号 == 事务序号)
                记录组_.erase(新位置);
            结果.状态 = 节点直接仓候选操作状态::资源失败;
        }
        return 结果;
    }

    节点直接仓候选操作状态 确认候选(
        节点直接事务幂等候选& 候选,
        std::uint64_t 事务序号) {
        std::unique_lock 锁(记录锁_);
        if (!候选匹配_(候选, 事务序号, 节点直接仓候选阶段::持有))
            return 节点直接仓候选操作状态::入口拒绝;
        const auto 位置 = 查找记录_(候选.身份_);
        if (位置 == 记录组_.end() || 位置->已发布 || 位置->事务序号 != 事务序号)
            return 节点直接仓候选操作状态::内部不一致;
        候选.阶段_ = 节点直接仓候选阶段::已确认待发布;
        return 节点直接仓候选操作状态::已确认待发布;
    }

    节点直接仓候选操作状态 撤销候选(
        节点直接事务幂等候选& 候选,
        std::uint64_t 事务序号) {
        std::unique_lock 锁(记录锁_);
        if (候选.仓库_ != this || 候选.事务序号_ != 事务序号 || 事务序号 == 0
            || (候选.阶段_ != 节点直接仓候选阶段::持有
                && 候选.阶段_ != 节点直接仓候选阶段::已确认待发布))
            return 节点直接仓候选操作状态::入口拒绝;
        const auto 位置 = 查找记录_(候选.身份_);
        if (位置 == 记录组_.end() || 位置->已发布 || 位置->事务序号 != 事务序号)
            return 节点直接仓候选操作状态::内部不一致;
        记录组_.erase(位置);
        候选.阶段_ = 节点直接仓候选阶段::已撤销;
        return 节点直接仓候选操作状态::已撤销;
    }

    节点直接仓候选操作状态 完成发布(
        节点直接事务幂等候选& 候选,
        std::uint64_t 事务序号) noexcept {
        try {
            std::unique_lock 锁(记录锁_);
            if (!候选匹配_(候选, 事务序号, 节点直接仓候选阶段::已确认待发布))
                return 节点直接仓候选操作状态::入口拒绝;
            const auto 位置 = 查找记录_(候选.身份_);
            if (位置 == 记录组_.end() || 位置->已发布 || 位置->事务序号 != 事务序号)
                return 节点直接仓候选操作状态::内部不一致;
            位置->记录.状态 = 节点直接事务幂等记录状态::已发布;
            位置->已发布 = true;
            位置->事务序号 = 0;
            候选.阶段_ = 节点直接仓候选阶段::已发布;
            return 节点直接仓候选操作状态::已发布;
        } catch (...) {
            return 节点直接仓候选操作状态::内部不一致;
        }
    }

private:
    struct 记录条目 { 节点直接事务幂等记录 记录; bool 已发布 = false; std::uint64_t 事务序号 = 0; };
    using 侧账条目 = 节点直接持久证据侧账记录;
    using 记录位置 = std::vector<记录条目>::iterator;
    using 只读记录位置 = std::vector<记录条目>::const_iterator;
    using 侧账位置 = std::vector<侧账条目>::iterator;
    using 只读侧账位置 = std::vector<侧账条目>::const_iterator;

    static bool 摘要非零_(const std::array<std::uint8_t, 32>& 摘要) noexcept {
        return std::any_of(摘要.begin(), 摘要.end(), [](std::uint8_t 值) { return 值 != 0; });
    }
    static bool 记录完整_(const 节点直接事务幂等记录& 记录) noexcept {
        return 节点直接事务幂等身份完整(记录.幂等身份)
            && 摘要非零_(记录.请求意图摘要) && 摘要非零_(记录.执行证据摘要)
            && 记录.材料格式版本 != 0 && 记录.写集规则版本 != 0
            && !记录.写集材料.empty()
            && 记录.状态 == 节点直接事务幂等记录状态::待发布
            && 记录.发布代次 != 0 && 摘要非零_(记录.结果摘要);
    }
    记录位置 查找记录_(节点直接事务幂等身份 身份) {
        return std::find_if(记录组_.begin(), 记录组_.end(), [&](const 记录条目& 值) { return 值.记录.幂等身份 == 身份; });
    }
    只读记录位置 查找记录_(节点直接事务幂等身份 身份) const {
        return std::find_if(记录组_.cbegin(), 记录组_.cend(), [&](const 记录条目& 值) { return 值.记录.幂等身份 == 身份; });
    }
    侧账位置 查找侧账_(节点直接事务幂等身份 身份) {
        return std::find_if(侧账组_.begin(), 侧账组_.end(), [&](const 侧账条目& 值) { return 值.幂等身份 == 身份; });
    }
    只读侧账位置 查找侧账_(节点直接事务幂等身份 身份) const {
        return std::find_if(侧账组_.cbegin(), 侧账组_.cend(), [&](const 侧账条目& 值) { return 值.幂等身份 == 身份; });
    }
    bool 候选匹配_(const 节点直接事务幂等候选& 候选, std::uint64_t 事务序号,
        节点直接仓候选阶段 阶段) const noexcept {
        return 事务序号 != 0 && 候选.仓库_ == this && 候选.事务序号_ == 事务序号 && 候选.阶段_ == 阶段;
    }
    friend class 节点直接事务幂等候选;
    mutable std::shared_mutex 记录锁_;
    std::vector<记录条目> 记录组_;
    mutable std::shared_mutex 侧账锁_;
    std::vector<侧账条目> 侧账组_;
};

std::optional<节点直接事务幂等记录> 节点直接事务幂等候选::读取候选记录() const {
    if (仓库_ == nullptr || 事务序号_ == 0
        || (阶段_ != 节点直接仓候选阶段::持有
            && 阶段_ != 节点直接仓候选阶段::已确认待发布)) return std::nullopt;
    std::shared_lock 锁(仓库_->记录锁_);
    const auto 位置 = 仓库_->查找记录_(身份_);
    return 位置 != 仓库_->记录组_.end() && !位置->已发布 && 位置->事务序号 == 事务序号_
        ? std::optional<节点直接事务幂等记录>{位置->记录}
        : std::nullopt;
}

} // namespace 海中鱼巣
