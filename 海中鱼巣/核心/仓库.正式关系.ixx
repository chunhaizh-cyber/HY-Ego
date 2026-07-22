// 文件规则：本模块只承载隔离新域正式关系事实；业务角色、基数和顺序必须由具名准入注入。
module;

#include "句柄.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>
#include <vector>

export module 海中鱼巣.核心.仓库.正式关系;

import 海中鱼巣.核心.仓库.节点直接身份;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 正式关系类型ABI数量 = 24;

enum class 正式关系操作状态 : std::uint32_t {
    已创建候选 = 1,
    已变更候选 = 2,
    幂等读回 = 3,
    已确认待发布 = 4,
    已撤销 = 5,
    已发布 = 6,
    入口拒绝_未知关系类型 = 7,
    入口拒绝_无效事务 = 8,
    入口拒绝_无效端点 = 9,
    入口拒绝_具名准入 = 10,
    入口拒绝_基数冲突 = 11,
    入口拒绝_版本漂移 = 12,
    入口拒绝_状态转换 = 13,
    入口拒绝_父链成环 = 14,
    内部不一致 = 15
};

struct 正式关系记录 {
    std::uint64_t 关系编号 = 0;
    关系类型 类型 = 关系类型::普通父子;
    节点句柄 源节点;
    节点句柄 目标节点;
    std::int64_t 顺序号 = 0;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
};

inline bool 正式关系记录完整(const 正式关系记录& 记录) noexcept {
    return 记录.关系编号 != 0
        && 句柄有效(记录.源节点)
        && 句柄有效(记录.目标节点)
        && 记录.版本号 != 0
        && (记录.状态 == 记录状态::有效 || 记录.状态 == 记录状态::已失效);
}

inline 关系句柄 形成正式关系句柄(
    std::uint64_t 仓库编号,
    const 正式关系记录& 记录) noexcept {
    return {仓库编号, 记录.关系编号, 记录.版本号};
}

using 正式关系角色准入函数 = bool (*)(
    节点类型 源类型,
    节点类型 目标类型,
    std::int64_t 顺序号) noexcept;

struct 具名正式关系准入 {
    关系类型 类型 = 关系类型::普通父子;
    std::uint64_t 授权身份 = 0;
    std::uint32_t 规则版本 = 0;
    bool 允许同源多目标 = false;
    bool 允许同目标多来源 = false;
    bool 允许重挂 = false;
    正式关系角色准入函数 裁决 = nullptr;

    bool 完整() const noexcept {
        const auto 数值 = static_cast<std::uint32_t>(类型);
        return 数值 < 正式关系类型ABI数量
            && 授权身份 != 0
            && 规则版本 != 0
            && 裁决 != nullptr;
    }
};

struct 完整正式关系准入表 {
    std::array<具名正式关系准入, 正式关系类型ABI数量> 规则组{};

    bool 完整() const noexcept {
        for (std::size_t 序号 = 0; 序号 < 规则组.size(); ++序号) {
            if (!规则组[序号].完整()
                || static_cast<std::uint32_t>(规则组[序号].类型) != 序号) {
                return false;
            }
        }
        return true;
    }
};

enum class 正式关系候选种类 : std::uint8_t {
    新关系 = 1,
    失效已发布关系 = 2,
    重挂已发布关系 = 3
};

enum class 正式关系候选阶段 : std::uint8_t {
    持有 = 1,
    已确认待发布 = 2,
    已撤销 = 3,
    已发布 = 4,
    已移动 = 5
};

class 正式关系仓库;

class 正式关系候选 {
public:
    正式关系候选() = delete;
    正式关系候选(const 正式关系候选&) = delete;
    正式关系候选& operator=(const 正式关系候选&) = delete;
    正式关系候选(正式关系候选&& 其它) noexcept
        : 仓库编号_(其它.仓库编号_), 事务序号_(其它.事务序号_), 种类_(其它.种类_),
          写前记录_(其它.写前记录_), 写后记录_(其它.写后记录_), 阶段_(其它.阶段_) {
        其它.仓库编号_ = 0;
        其它.事务序号_ = 0;
        其它.阶段_ = 正式关系候选阶段::已移动;
    }
    正式关系候选& operator=(正式关系候选&&) = delete;

    std::uint64_t 读取事务序号() const noexcept { return 事务序号_; }
    正式关系候选种类 读取种类() const noexcept { return 种类_; }
    正式关系候选阶段 读取阶段() const noexcept { return 阶段_; }
    const std::optional<正式关系记录>& 读取写前记录() const noexcept { return 写前记录_; }
    const 正式关系记录& 读取写后记录() const noexcept { return 写后记录_; }

private:
    friend class 正式关系仓库;
    正式关系候选(
        std::uint64_t 仓库编号,
        std::uint64_t 事务序号,
        正式关系候选种类 种类,
        std::optional<正式关系记录> 写前记录,
        正式关系记录 写后记录) noexcept
        : 仓库编号_(仓库编号), 事务序号_(事务序号), 种类_(种类),
          写前记录_(std::move(写前记录)), 写后记录_(写后记录) {
    }

    std::uint64_t 仓库编号_ = 0;
    std::uint64_t 事务序号_ = 0;
    正式关系候选种类 种类_ = 正式关系候选种类::新关系;
    std::optional<正式关系记录> 写前记录_;
    正式关系记录 写后记录_;
    正式关系候选阶段 阶段_ = 正式关系候选阶段::持有;
};

struct 正式关系写入结果 {
    正式关系操作状态 状态 = 正式关系操作状态::入口拒绝_具名准入;
    std::optional<关系句柄> 当前关系;
    std::optional<正式关系候选> 候选;

    bool 成功() const noexcept {
        return 状态 == 正式关系操作状态::已创建候选
            || 状态 == 正式关系操作状态::已变更候选
            || 状态 == 正式关系操作状态::幂等读回;
    }
};

struct 正式关系审计材料 {
    正式关系记录 当前记录;
    std::vector<正式关系记录> 历史记录组;
};

class 正式关系仓库 {
public:
    正式关系仓库(
        const 节点直接身份仓库& 节点,
        完整正式关系准入表 准入表,
        std::uint64_t 仓库编号)
        : 节点_(节点), 准入表_(std::move(准入表)), 仓库编号_(仓库编号) {
    }

    bool 有效() const noexcept {
        return 仓库编号_ != 0 && 准入表_.完整();
    }

    std::uint64_t 仓库编号() const noexcept { return 仓库编号_; }

    正式关系写入结果 结构化创建关系未发布候选(
        关系类型 类型,
        节点句柄 源节点,
        节点句柄 目标节点,
        std::int64_t 顺序号,
        std::uint64_t 事务序号) {
        正式关系写入结果 结果;
        const auto 类型数值 = static_cast<std::uint32_t>(类型);
        if (类型数值 >= 正式关系类型ABI数量) {
            结果.状态 = 正式关系操作状态::入口拒绝_未知关系类型;
            return 结果;
        }
        if (!有效() || 事务序号 == 0) {
            结果.状态 = 正式关系操作状态::入口拒绝_无效事务;
            return 结果;
        }
        const auto 源记录 = 节点_.读取节点(源节点, 事务序号);
        const auto 目标记录 = 节点_.读取节点(目标节点, 事务序号);
        if (!源记录.has_value() || !目标记录.has_value()
            || 源记录->状态 != 记录状态::有效 || 目标记录->状态 != 记录状态::有效) {
            结果.状态 = 正式关系操作状态::入口拒绝_无效端点;
            return 结果;
        }
        const auto& 规则 = 准入表_.规则组[类型数值];
        if (!规则.裁决(源记录->类型, 目标记录->类型, 顺序号)) {
            结果.状态 = 正式关系操作状态::入口拒绝_具名准入;
            return 结果;
        }

        std::unique_lock 锁(仓库锁_);
        for (const auto& [编号, 条目] : 关系表_) {
            if ((! 条目.已发布 && 条目.候选事务序号 != 事务序号)
                || 条目.当前记录.状态 != 记录状态::有效 || 条目.当前记录.类型 != 类型) continue;
            if (条目.当前记录.源节点 == 源节点
                && 条目.当前记录.目标节点 == 目标节点
                && 条目.当前记录.顺序号 == 顺序号) {
                结果.状态 = 正式关系操作状态::幂等读回;
                结果.当前关系 = 形成正式关系句柄(仓库编号_, 条目.当前记录);
                return 结果;
            }
            if ((!规则.允许同源多目标 && 条目.当前记录.源节点 == 源节点)
                || (!规则.允许同目标多来源 && 条目.当前记录.目标节点 == 目标节点)) {
                结果.状态 = 正式关系操作状态::入口拒绝_基数冲突;
                return 结果;
            }
        }
        if (类型 == 关系类型::普通父子
            && (源节点 == 目标节点 || 子链包含节点_已加锁(目标节点, 源节点, 事务序号))) {
            结果.状态 = 正式关系操作状态::入口拒绝_父链成环;
            return 结果;
        }
        if (下个关系编号_ == 0 || 下个关系编号_ == std::numeric_limits<std::uint64_t>::max()) {
            结果.状态 = 正式关系操作状态::内部不一致;
            return 结果;
        }
        const auto 关系编号 = 下个关系编号_++;
        正式关系记录 写后{关系编号, 类型, 源节点, 目标节点, 顺序号, 1, 记录状态::有效};
        try {
            auto [位置, 已有] = 关系表_.emplace(关系编号, 关系条目{写后, {}, false, 事务序号});
            if (!已有) {
                结果.状态 = 正式关系操作状态::内部不一致;
                return 结果;
            }
        } catch (...) {
            结果.状态 = 正式关系操作状态::内部不一致;
            return 结果;
        }
        结果.状态 = 正式关系操作状态::已创建候选;
        结果.当前关系 = 形成正式关系句柄(仓库编号_, 写后);
        结果.候选.emplace(正式关系候选{
            仓库编号_, 事务序号, 正式关系候选种类::新关系, std::nullopt, 写后});
        return 结果;
    }

    正式关系写入结果 结构化失效已发布关系(
        关系句柄 关系,
        std::uint64_t 事务序号) {
        return 形成已发布变更候选_(
            关系, {}, {}, 事务序号, 正式关系候选种类::失效已发布关系);
    }

    正式关系写入结果 结构化重挂已发布关系(
        关系句柄 关系,
        节点句柄 新源节点,
        节点句柄 新目标节点,
        std::uint64_t 事务序号) {
        return 形成已发布变更候选_(
            关系, 新源节点, 新目标节点, 事务序号,
            正式关系候选种类::重挂已发布关系);
    }

    std::optional<正式关系记录> 读取关系(关系句柄 关系) const {
        auto 记录 = 读取关系核心_(关系, std::nullopt, false);
        if (!记录.has_value()) return std::nullopt;
        if (!节点_.读取节点(记录->源节点).has_value()
            || !节点_.读取节点(记录->目标节点).has_value()) return std::nullopt;
        return 记录;
    }

    std::optional<正式关系记录> 读取关系(
        关系句柄 关系,
        std::uint64_t 事务序号) const {
        auto 记录 = 读取关系核心_(关系, 事务序号, false);
        if (!记录.has_value()) return std::nullopt;
        if (!节点_.读取节点(记录->源节点, 事务序号).has_value()
            || !节点_.读取节点(记录->目标节点, 事务序号).has_value()) return std::nullopt;
        return 记录;
    }

    std::optional<正式关系审计材料> 读取关系审计(关系句柄 关系) const {
        std::shared_lock 锁(仓库锁_);
        const auto 位置 = 关系表_.find(关系.关系编号);
        if (!句柄属于当前记录_(关系, 位置) || !位置->second.已发布) return std::nullopt;
        return 正式关系审计材料{位置->second.当前记录, 位置->second.历史记录组};
    }

    std::optional<正式关系审计材料> 读取关系审计(
        关系句柄 关系,
        std::uint64_t 事务序号) const {
        std::shared_lock 锁(仓库锁_);
        const auto 位置 = 关系表_.find(关系.关系编号);
        if (!句柄属于当前记录_(关系, 位置)
            || (!位置->second.已发布 && 位置->second.候选事务序号 != 事务序号)) return std::nullopt;
        return 正式关系审计材料{位置->second.当前记录, 位置->second.历史记录组};
    }

    std::vector<正式关系记录> 读取全部关系审计() const {
        std::vector<正式关系记录> 结果;
        std::shared_lock 锁(仓库锁_);
        for (const auto& [编号, 条目] : 关系表_) {
            if (!条目.已发布) continue;
            结果.insert(结果.end(), 条目.历史记录组.begin(), 条目.历史记录组.end());
            结果.push_back(条目.当前记录);
        }
        std::sort(结果.begin(), 结果.end(), [] (const auto& 左, const auto& 右) {
            if (左.关系编号 != 右.关系编号) return 左.关系编号 < 右.关系编号;
            return 左.版本号 < 右.版本号;
        });
        return 结果;
    }

    std::vector<正式关系记录> 读取有效关系组(
        节点句柄 源节点,
        关系类型 类型) const {
        std::vector<正式关系记录> 结果;
        std::shared_lock 锁(仓库锁_);
        for (const auto& [编号, 条目] : 关系表_) {
            if (条目.已发布 && 条目.当前记录.状态 == 记录状态::有效
                && 条目.当前记录.源节点 == 源节点 && 条目.当前记录.类型 == 类型) {
                结果.push_back(条目.当前记录);
            }
        }
        std::sort(结果.begin(), 结果.end(), [] (const auto& 左, const auto& 右) {
            if (左.顺序号 != 右.顺序号) return 左.顺序号 < 右.顺序号;
            return 左.关系编号 < 右.关系编号;
        });
        return 结果;
    }

    正式关系操作状态 确认候选(正式关系候选& 候选, std::uint64_t 事务序号) {
        std::unique_lock 锁(仓库锁_);
        if (!候选匹配_(候选, 事务序号, 正式关系候选阶段::持有)) {
            return 正式关系操作状态::入口拒绝_无效事务;
        }
        const auto 位置 = 关系表_.find(候选.写后记录_.关系编号);
        if (位置 == 关系表_.end() || 位置->second.已发布
            || 位置->second.候选事务序号 != 事务序号
            || !记录相同_(位置->second.当前记录, 候选.写后记录_)) {
            return 正式关系操作状态::内部不一致;
        }
        候选.阶段_ = 正式关系候选阶段::已确认待发布;
        return 正式关系操作状态::已确认待发布;
    }

    正式关系操作状态 撤销候选(正式关系候选& 候选, std::uint64_t 事务序号) {
        std::unique_lock 锁(仓库锁_);
        if (!候选匹配_(候选, 事务序号, 候选.阶段_)
            || (候选.阶段_ != 正式关系候选阶段::持有
                && 候选.阶段_ != 正式关系候选阶段::已确认待发布)) {
            return 正式关系操作状态::入口拒绝_无效事务;
        }
        const auto 位置 = 关系表_.find(候选.写后记录_.关系编号);
        if (位置 == 关系表_.end() || 位置->second.已发布
            || 位置->second.候选事务序号 != 事务序号) {
            return 正式关系操作状态::内部不一致;
        }
        if (候选.种类_ == 正式关系候选种类::新关系) {
            关系表_.erase(位置);
        } else {
            if (!候选.写前记录_.has_value() || 位置->second.历史记录组.empty()
                || !记录相同_(位置->second.历史记录组.back(), 候选.写前记录_.value())) {
                return 正式关系操作状态::内部不一致;
            }
            位置->second.当前记录 = 候选.写前记录_.value();
            位置->second.历史记录组.pop_back();
            位置->second.已发布 = true;
            位置->second.候选事务序号 = 0;
        }
        候选.阶段_ = 正式关系候选阶段::已撤销;
        return 正式关系操作状态::已撤销;
    }

    void 完成发布(正式关系候选& 候选, std::uint64_t 事务序号) noexcept {
        if (!候选匹配_(候选, 事务序号, 正式关系候选阶段::已确认待发布)) return;
        std::unique_lock 锁(仓库锁_);
        const auto 位置 = 关系表_.find(候选.写后记录_.关系编号);
        if (位置 == 关系表_.end() || 位置->second.已发布
            || 位置->second.候选事务序号 != 事务序号) return;
        位置->second.已发布 = true;
        位置->second.候选事务序号 = 0;
        候选.阶段_ = 正式关系候选阶段::已发布;
    }

private:
    struct 关系条目 {
        正式关系记录 当前记录;
        std::vector<正式关系记录> 历史记录组;
        bool 已发布 = false;
        std::uint64_t 候选事务序号 = 0;
    };

    using 关系位置 = std::unordered_map<std::uint64_t, 关系条目>::const_iterator;

    bool 句柄属于当前记录_(关系句柄 句柄, 关系位置 位置) const noexcept {
        return 句柄有效(句柄) && 句柄.仓库编号 == 仓库编号_
            && 位置 != 关系表_.end() && 位置->second.当前记录.版本号 == 句柄.版本号;
    }

    std::optional<正式关系记录> 读取关系核心_(
        关系句柄 关系,
        std::optional<std::uint64_t> 事务序号,
        bool 允许非有效) const {
        std::shared_lock 锁(仓库锁_);
        const auto 位置 = 关系表_.find(关系.关系编号);
        if (!句柄属于当前记录_(关系, 位置)) return std::nullopt;
        if (!位置->second.已发布
            && (!事务序号.has_value() || 位置->second.候选事务序号 != 事务序号.value())) return std::nullopt;
        if (!允许非有效 && 位置->second.当前记录.状态 != 记录状态::有效) return std::nullopt;
        return 位置->second.当前记录;
    }

    bool 子链包含节点_已加锁(
        节点句柄 起点,
        节点句柄 目标,
        std::uint64_t 事务序号) const {
        std::vector<节点句柄> 待扫描{起点};
        std::vector<std::uint64_t> 已访问;
        while (!待扫描.empty()) {
            const auto 当前 = 待扫描.back();
            待扫描.pop_back();
            if (当前 == 目标) return true;
            if (std::find(已访问.begin(), 已访问.end(), 当前.节点编号) != 已访问.end()) continue;
            已访问.push_back(当前.节点编号);
            for (const auto& [编号, 条目] : 关系表_) {
                const auto& 关系 = 条目.当前记录;
                if ((条目.已发布 || 条目.候选事务序号 == 事务序号)
                    && 关系.状态 == 记录状态::有效
                    && 关系.类型 == 关系类型::普通父子 && 关系.源节点 == 当前) {
                    待扫描.push_back(关系.目标节点);
                }
            }
        }
        return false;
    }

    bool 候选匹配_(
        const 正式关系候选& 候选,
        std::uint64_t 事务序号,
        正式关系候选阶段 阶段) const noexcept {
        return 事务序号 != 0 && 候选.仓库编号_ == 仓库编号_
            && 候选.事务序号_ == 事务序号 && 候选.阶段_ == 阶段;
    }

    static bool 记录相同_(const 正式关系记录& 左, const 正式关系记录& 右) noexcept {
        return 左.关系编号 == 右.关系编号 && 左.类型 == 右.类型
            && 左.源节点 == 右.源节点 && 左.目标节点 == 右.目标节点
            && 左.顺序号 == 右.顺序号 && 左.版本号 == 右.版本号 && 左.状态 == 右.状态;
    }

    正式关系写入结果 形成已发布变更候选_(
        关系句柄 关系,
        节点句柄 新源节点,
        节点句柄 新目标节点,
        std::uint64_t 事务序号,
        正式关系候选种类 种类) {
        正式关系写入结果 结果;
        if (!有效() || 事务序号 == 0) {
            结果.状态 = 正式关系操作状态::入口拒绝_无效事务;
            return 结果;
        }
        if (!句柄有效(关系) || 关系.仓库编号 != 仓库编号_) {
            结果.状态 = 正式关系操作状态::入口拒绝_版本漂移;
            return 结果;
        }

        std::optional<节点直接身份记录> 新源记录;
        std::optional<节点直接身份记录> 新目标记录;
        if (种类 == 正式关系候选种类::重挂已发布关系) {
            新源记录 = 节点_.读取节点(新源节点, 事务序号);
            新目标记录 = 节点_.读取节点(新目标节点, 事务序号);
            if (!新源记录.has_value() || !新目标记录.has_value()) {
                结果.状态 = 正式关系操作状态::入口拒绝_无效端点;
                return 结果;
            }
        }

        std::unique_lock 锁(仓库锁_);
        const auto 位置 = 关系表_.find(关系.关系编号);
        if (位置 == 关系表_.end() || !位置->second.已发布) {
            结果.状态 = 正式关系操作状态::入口拒绝_版本漂移;
            return 结果;
        }
        const auto 写前 = 位置->second.当前记录;
        if (写前.版本号 != 关系.版本号 || 写前.状态 != 记录状态::有效) {
            结果.状态 = 正式关系操作状态::入口拒绝_状态转换;
            return 结果;
        }
        if (种类 == 正式关系候选种类::重挂已发布关系
            && 写前.源节点 == 新源节点 && 写前.目标节点 == 新目标节点) {
            结果.状态 = 正式关系操作状态::幂等读回;
            结果.当前关系 = 关系;
            return 结果;
        }
        if (写前.版本号 == std::numeric_limits<std::uint32_t>::max()) {
            结果.状态 = 正式关系操作状态::内部不一致;
            return 结果;
        }
        auto 写后 = 写前;
        ++写后.版本号;
        if (种类 == 正式关系候选种类::失效已发布关系) {
            写后.状态 = 记录状态::已失效;
        } else {
            const auto 类型数值 = static_cast<std::uint32_t>(写前.类型);
            const auto& 规则 = 准入表_.规则组[类型数值];
            if (!规则.允许重挂
                || !规则.裁决(新源记录->类型, 新目标记录->类型, 写前.顺序号)) {
                结果.状态 = 正式关系操作状态::入口拒绝_具名准入;
                return 结果;
            }
            for (const auto& [编号, 条目] : 关系表_) {
                if (编号 == 写前.关系编号 || !条目.已发布
                    || 条目.当前记录.状态 != 记录状态::有效
                    || 条目.当前记录.类型 != 写前.类型) continue;
                if ((!规则.允许同源多目标 && 条目.当前记录.源节点 == 新源节点)
                    || (!规则.允许同目标多来源 && 条目.当前记录.目标节点 == 新目标节点)) {
                    结果.状态 = 正式关系操作状态::入口拒绝_基数冲突;
                    return 结果;
                }
            }
            if (写前.类型 == 关系类型::普通父子
                && (新源节点 == 新目标节点 || 子链包含节点_已加锁(新目标节点, 新源节点, 事务序号))) {
                结果.状态 = 正式关系操作状态::入口拒绝_父链成环;
                return 结果;
            }
            写后.源节点 = 新源节点;
            写后.目标节点 = 新目标节点;
        }
        try {
            位置->second.历史记录组.push_back(写前);
        } catch (...) {
            结果.状态 = 正式关系操作状态::内部不一致;
            return 结果;
        }
        位置->second.当前记录 = 写后;
        位置->second.已发布 = false;
        位置->second.候选事务序号 = 事务序号;
        结果.状态 = 正式关系操作状态::已变更候选;
        结果.当前关系 = 形成正式关系句柄(仓库编号_, 写后);
        结果.候选.emplace(正式关系候选{
            仓库编号_, 事务序号, 种类, 写前, 写后});
        return 结果;
    }

    const 节点直接身份仓库& 节点_;
    完整正式关系准入表 准入表_;
    std::uint64_t 仓库编号_ = 0;
    std::uint64_t 下个关系编号_ = 1;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 关系条目> 关系表_;
};

}
