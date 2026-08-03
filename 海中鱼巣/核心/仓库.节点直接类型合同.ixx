// 文件规则：本模块只承载节点直接类型合同不可变记录及其候选生命周期。
module;

#include "节点直接结构合同.数据.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <unordered_map>
#include <vector>

export module 海中鱼巣.核心.仓库.节点直接类型合同;

export namespace 海中鱼巣 {

class 节点直接类型合同仓库;

class 节点直接类型合同候选 final {
public:
    节点直接类型合同候选(节点直接类型合同候选&& 其它) noexcept
        : 仓库_(其它.仓库_), 事务序号_(其它.事务序号_), 合同身份_(其它.合同身份_),
          合同版本_(其它.合同版本_), 阶段_(其它.阶段_) {
        其它.仓库_ = nullptr;
        其它.事务序号_ = 0;
        其它.阶段_ = 节点直接仓候选阶段::已移动;
    }
    节点直接类型合同候选(const 节点直接类型合同候选&) = delete;
    节点直接类型合同候选& operator=(const 节点直接类型合同候选&) = delete;

    节点直接仓候选阶段 读取阶段() const noexcept { return 阶段_; }
    std::optional<类型合同读回> 读取候选合同() const;

private:
    friend class 节点直接类型合同仓库;
    节点直接类型合同候选(
        节点直接类型合同仓库* 仓库,
        std::uint64_t 事务序号,
        类型合同稳定身份 合同身份,
        std::uint32_t 合同版本) noexcept
        : 仓库_(仓库), 事务序号_(事务序号), 合同身份_(合同身份), 合同版本_(合同版本) {}

    节点直接类型合同仓库* 仓库_ = nullptr;
    std::uint64_t 事务序号_ = 0;
    类型合同稳定身份 合同身份_;
    std::uint32_t 合同版本_ = 0;
    节点直接仓候选阶段 阶段_ = 节点直接仓候选阶段::持有;
};

struct 节点直接类型合同候选结果 {
    节点直接仓候选操作状态 状态 = 节点直接仓候选操作状态::入口拒绝;
    std::optional<节点直接类型合同候选> 候选;
};

class 节点直接类型合同仓库 final {
public:
    std::optional<类型合同读回> 读取精确合同(
        类型合同稳定身份 身份,
        std::uint32_t 版本) const {
        if (!类型合同稳定身份完整(身份) || 版本 == 0) return std::nullopt;
        std::shared_lock 锁(仓库锁_);
        const auto 位置 = 查找_(身份, 版本);
        return 位置 != 记录组_.end() && 位置->已发布
            ? std::optional<类型合同读回>{位置->记录}
            : std::nullopt;
    }

    节点直接类型合同候选结果 结构化发布合同未发布候选(
        const 类型合同读回& 计划合同,
        std::uint64_t 事务序号) {
        节点直接类型合同候选结果 结果;
        if (事务序号 == 0 || !合同完整_(计划合同)) return 结果;
        std::unique_lock 锁(仓库锁_);
        const auto 位置 = 查找_(计划合同.合同身份, 计划合同.合同版本);
        if (位置 != 记录组_.end()) {
            结果.状态 = 位置->记录 == 计划合同
                ? 节点直接仓候选操作状态::精确同义
                : 节点直接仓候选操作状态::内部不一致;
            return 结果;
        }
        const auto 命名域 = 计划合同.合同身份.命名域;
        const auto 高水位位置 = 每域高水位_.find(命名域);
        const auto 当前高水位 = 高水位位置 == 每域高水位_.end() ? 0 : 高水位位置->second;
        if (当前高水位 == std::numeric_limits<std::uint64_t>::max()
            || 计划合同.合同身份.键值 != 当前高水位 + 1) {
            结果.状态 = 节点直接仓候选操作状态::版本漂移;
            return 结果;
        }
        try {
            记录组_.push_back({计划合同, false, 事务序号});
            每域高水位_[命名域] = 计划合同.合同身份.键值;
            结果.候选.emplace(节点直接类型合同候选{
                this, 事务序号, 计划合同.合同身份, 计划合同.合同版本});
            结果.状态 = 节点直接仓候选操作状态::已形成候选;
        } catch (...) {
            const auto 新位置 = 查找_(计划合同.合同身份, 计划合同.合同版本);
            if (新位置 != 记录组_.end() && !新位置->已发布 && 新位置->事务序号 == 事务序号) {
                记录组_.erase(新位置);
            }
            if (当前高水位 == 0) 每域高水位_.erase(命名域);
            else 每域高水位_[命名域] = 当前高水位;
            结果.状态 = 节点直接仓候选操作状态::资源失败;
        }
        return 结果;
    }

    节点直接仓候选操作状态 确认候选(
        节点直接类型合同候选& 候选,
        std::uint64_t 事务序号) {
        std::unique_lock 锁(仓库锁_);
        if (!候选匹配_(候选, 事务序号, 节点直接仓候选阶段::持有)) {
            return 节点直接仓候选操作状态::入口拒绝;
        }
        const auto 位置 = 查找_(候选.合同身份_, 候选.合同版本_);
        if (位置 == 记录组_.end() || 位置->已发布 || 位置->事务序号 != 事务序号) {
            return 节点直接仓候选操作状态::内部不一致;
        }
        候选.阶段_ = 节点直接仓候选阶段::已确认待发布;
        return 节点直接仓候选操作状态::已确认待发布;
    }

    节点直接仓候选操作状态 撤销候选(
        节点直接类型合同候选& 候选,
        std::uint64_t 事务序号) {
        std::unique_lock 锁(仓库锁_);
        if (候选.仓库_ != this || 候选.事务序号_ != 事务序号 || 事务序号 == 0
            || (候选.阶段_ != 节点直接仓候选阶段::持有
                && 候选.阶段_ != 节点直接仓候选阶段::已确认待发布)) {
            return 节点直接仓候选操作状态::入口拒绝;
        }
        const auto 位置 = 查找_(候选.合同身份_, 候选.合同版本_);
        if (位置 == 记录组_.end() || 位置->已发布 || 位置->事务序号 != 事务序号) {
            return 节点直接仓候选操作状态::内部不一致;
        }
        const auto 命名域 = 候选.合同身份_.命名域;
        const auto 高水位位置 = 每域高水位_.find(命名域);
        if (高水位位置 == 每域高水位_.end()
            || 高水位位置->second != 候选.合同身份_.键值) {
            return 节点直接仓候选操作状态::内部不一致;
        }
        记录组_.erase(位置);
        if (候选.合同身份_.键值 == 1) 每域高水位_.erase(命名域);
        else 高水位位置->second = 候选.合同身份_.键值 - 1;
        候选.阶段_ = 节点直接仓候选阶段::已撤销;
        return 节点直接仓候选操作状态::已撤销;
    }

    节点直接仓候选操作状态 完成发布(
        节点直接类型合同候选& 候选,
        std::uint64_t 事务序号) noexcept {
        try {
            std::unique_lock 锁(仓库锁_);
            if (!候选匹配_(候选, 事务序号, 节点直接仓候选阶段::已确认待发布)) {
                return 节点直接仓候选操作状态::入口拒绝;
            }
            const auto 位置 = 查找_(候选.合同身份_, 候选.合同版本_);
            if (位置 == 记录组_.end() || 位置->已发布 || 位置->事务序号 != 事务序号) {
                return 节点直接仓候选操作状态::内部不一致;
            }
            位置->已发布 = true;
            位置->事务序号 = 0;
            历史占用_.push_back({候选.合同身份_});
            候选.阶段_ = 节点直接仓候选阶段::已发布;
            return 节点直接仓候选操作状态::已发布;
        } catch (...) {
            return 节点直接仓候选操作状态::内部不一致;
        }
    }

    std::uint64_t 读取命名域高水位(std::uint64_t 命名域) const noexcept {
        try {
            std::shared_lock 锁(仓库锁_);
            const auto 位置 = 每域高水位_.find(命名域);
            return 位置 == 每域高水位_.end() ? 0 : 位置->second;
        } catch (...) { return 0; }
    }

    节点直接类型合同仓库权威材料 导出权威状态() const {
        std::shared_lock 锁(仓库锁_);
        节点直接类型合同仓库权威材料 材料;
        for (const auto& [命名域, 高水位] : 每域高水位_) 材料.每域高水位.push_back({命名域, 高水位});
        材料.历史占用 = 历史占用_;
        for (const auto& 条目 : 记录组_) if (条目.已发布) 材料.记录组.push_back(条目.记录);
        std::sort(材料.每域高水位.begin(), 材料.每域高水位.end(), [](const auto& 左, const auto& 右) {
            return 左.命名域 < 右.命名域;
        });
        std::sort(材料.历史占用.begin(), 材料.历史占用.end(), [](const auto& 左, const auto& 右) {
            return 左.身份.命名域 != 右.身份.命名域 ? 左.身份.命名域 < 右.身份.命名域
                : 左.身份.键值 < 右.身份.键值;
        });
        std::sort(材料.记录组.begin(), 材料.记录组.end(), [](const auto& 左, const auto& 右) {
            if (左.合同身份.命名域 != 右.合同身份.命名域) return 左.合同身份.命名域 < 右.合同身份.命名域;
            if (左.合同身份.键值 != 右.合同身份.键值) return 左.合同身份.键值 < 右.合同身份.键值;
            return 左.合同版本 < 右.合同版本;
        });
        return 材料;
    }

private:
    struct 条目 { 类型合同读回 记录; bool 已发布 = false; std::uint64_t 事务序号 = 0; };
    using 位置类型 = std::vector<条目>::iterator;
    using 只读位置类型 = std::vector<条目>::const_iterator;

    static bool 合同完整_(const 类型合同读回& 合同) noexcept {
        if (!类型合同稳定身份完整(合同.合同身份)
            || 合同.命名空间.命名域 == 0 || 合同.命名空间.键值 == 0
            || 合同.合同版本 == 0 || 合同.表示 == 类型化值表示种类::未定义
            || !服务稳定身份完整(合同.所有者服务)
            || 合同.首次发布代次 == 0
            || 合同.当前状态发布代次 < 合同.首次发布代次) return false;
        const auto 预期分支 = static_cast<std::size_t>(合同.表示) - 1;
        if (合同.值域.index() != 预期分支) return false;
        for (const auto& 声明 : 合同.直接兼容组) {
            if (!类型合同稳定身份完整(声明.另一合同身份) || 声明.另一合同版本 == 0) return false;
        }
        return true;
    }

    位置类型 查找_(类型合同稳定身份 身份, std::uint32_t 版本) {
        return std::find_if(记录组_.begin(), 记录组_.end(), [&](const 条目& 条目值) {
            return 条目值.记录.合同身份 == 身份 && 条目值.记录.合同版本 == 版本;
        });
    }
    只读位置类型 查找_(类型合同稳定身份 身份, std::uint32_t 版本) const {
        return std::find_if(记录组_.cbegin(), 记录组_.cend(), [&](const 条目& 条目值) {
            return 条目值.记录.合同身份 == 身份 && 条目值.记录.合同版本 == 版本;
        });
    }
    bool 候选匹配_(const 节点直接类型合同候选& 候选, std::uint64_t 事务序号,
        节点直接仓候选阶段 阶段) const noexcept {
        return 事务序号 != 0 && 候选.仓库_ == this && 候选.事务序号_ == 事务序号
            && 候选.阶段_ == 阶段;
    }

    friend class 节点直接类型合同候选;
    mutable std::shared_mutex 仓库锁_;
    std::vector<条目> 记录组_;
    std::unordered_map<std::uint64_t, std::uint64_t> 每域高水位_;
    std::vector<类型合同稳定身份历史占用> 历史占用_;
};

std::optional<类型合同读回> 节点直接类型合同候选::读取候选合同() const {
    if (仓库_ == nullptr || 事务序号_ == 0
        || (阶段_ != 节点直接仓候选阶段::持有
            && 阶段_ != 节点直接仓候选阶段::已确认待发布)) return std::nullopt;
    std::shared_lock 锁(仓库_->仓库锁_);
    const auto 位置 = 仓库_->查找_(合同身份_, 合同版本_);
    return 位置 != 仓库_->记录组_.end() && !位置->已发布 && 位置->事务序号 == 事务序号_
        ? std::optional<类型合同读回>{位置->记录}
        : std::nullopt;
}

} // namespace 海中鱼巣
