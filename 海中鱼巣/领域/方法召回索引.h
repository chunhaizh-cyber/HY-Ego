// 文件规则：方法召回索引只维护可清空重建的非权威值式快照；命中后必须复核方法权威结构。
#pragma once

#include "方法服务.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 方法召回索引规则版本 = 1;

enum class 方法召回索引来源状态 : std::uint32_t {
    无效 = 0,
    当前 = 1,
    已过期 = 2
};

struct 方法结果能力键 {
    std::uint32_t 规则版本 = 0;
    节点句柄 目标特征;
    std::int64_t 目标状态值材料 = 0;

    bool 完整() const {
        return 规则版本 == 方法召回索引规则版本
            && 句柄有效(目标特征);
    }
};

inline bool operator==(const 方法结果能力键& 左, const 方法结果能力键& 右) {
    return 左.规则版本 == 右.规则版本
        && 左.目标特征 == 右.目标特征
        && 左.目标状态值材料 == 右.目标状态值材料;
}

inline bool 方法召回节点句柄稳定小于(const 节点句柄& 左, const 节点句柄& 右) {
    if (左.仓库编号 != 右.仓库编号) {
        return 左.仓库编号 < 右.仓库编号;
    }
    if (左.节点编号 != 右.节点编号) {
        return 左.节点编号 < 右.节点编号;
    }
    return 左.版本号 < 右.版本号;
}

inline bool 方法结果能力键稳定小于(const 方法结果能力键& 左, const 方法结果能力键& 右) {
    if (左.规则版本 != 右.规则版本) {
        return 左.规则版本 < 右.规则版本;
    }
    if (!(左.目标特征 == 右.目标特征)) {
        return 方法召回节点句柄稳定小于(左.目标特征, 右.目标特征);
    }
    return 左.目标状态值材料 < 右.目标状态值材料;
}

struct 方法结果能力键哈希 {
    std::size_t operator()(const 方法结果能力键& 键) const noexcept {
        std::uint64_t 结果 = 0x9E37'79B9'7F4A'7C15ULL;
        const auto 合并 = [&结果](std::uint64_t 值) {
            值 += 0x9E37'79B9'7F4A'7C15ULL;
            值 = (值 ^ (值 >> 30)) * 0xBF58'476D'1CE4'E5B9ULL;
            值 = (值 ^ (值 >> 27)) * 0x94D0'49BB'1331'11EBULL;
            值 ^= 值 >> 31;
            结果 ^= 值 + 0x9E37'79B9'7F4A'7C15ULL + (结果 << 6) + (结果 >> 2);
        };
        合并(键.规则版本);
        合并(键.目标特征.仓库编号);
        合并(键.目标特征.节点编号);
        合并(键.目标特征.版本号);
        合并(static_cast<std::uint64_t>(键.目标状态值材料));
        return static_cast<std::size_t>(结果);
    }
};

struct 方法结果索引项 {
    方法结果能力键 能力键;
    节点句柄 方法首;
    节点句柄 结果节点;
    关系句柄 登记关系;
    关系句柄 生命周期关系;
    std::uint32_t 生命周期版本 = 0;
    关系句柄 结果特征关系;
    关系句柄 结果目标状态关系;
    std::uint32_t 结果规格规则版本 = 0;

    bool 完整() const {
        return 能力键.完整()
            && 句柄有效(方法首)
            && 句柄有效(结果节点)
            && 句柄有效(登记关系)
            && 句柄有效(生命周期关系)
            && 生命周期版本 == 生命周期关系.版本号
            && 句柄有效(结果特征关系)
            && 句柄有效(结果目标状态关系)
            && 结果规格规则版本 == 方法召回索引规则版本;
    }
};

inline bool 方法结果索引项身份相同(const 方法结果索引项& 左, const 方法结果索引项& 右) {
    return 左.能力键 == 右.能力键
        && 左.方法首 == 右.方法首
        && 左.结果节点 == 右.结果节点;
}

inline bool 方法结果索引项完整相同(const 方法结果索引项& 左, const 方法结果索引项& 右) {
    return 方法结果索引项身份相同(左, 右)
        && 左.登记关系 == 右.登记关系
        && 左.生命周期关系 == 右.生命周期关系
        && 左.生命周期版本 == 右.生命周期版本
        && 左.结果特征关系 == 右.结果特征关系
        && 左.结果目标状态关系 == 右.结果目标状态关系
        && 左.结果规格规则版本 == 右.结果规格规则版本;
}

inline bool 方法结果索引项稳定小于(const 方法结果索引项& 左, const 方法结果索引项& 右) {
    if (!(左.能力键 == 右.能力键)) {
        return 方法结果能力键稳定小于(左.能力键, 右.能力键);
    }
    if (!(左.方法首 == 右.方法首)) {
        return 方法召回节点句柄稳定小于(左.方法首, 右.方法首);
    }
    return 方法召回节点句柄稳定小于(左.结果节点, 右.结果节点);
}

inline bool 方法结果索引项组相同(
    const std::vector<方法结果索引项>& 左,
    const std::vector<方法结果索引项>& 右) {
    if (左.size() != 右.size()) {
        return false;
    }
    for (std::size_t 索引 = 0; 索引 < 左.size(); ++索引) {
        if (!方法结果索引项完整相同(左[索引], 右[索引])) {
            return false;
        }
    }
    return true;
}

struct 方法结果索引重建候选 {
    std::uint32_t 规则版本 = 0;
    方法召回索引来源状态 来源状态 = 方法召回索引来源状态::无效;
    std::vector<方法结果索引项> 索引项组;

    bool 完整() const {
        return 规则版本 == 方法召回索引规则版本
            && 来源状态 == 方法召回索引来源状态::当前
            && std::all_of(索引项组.begin(), 索引项组.end(), [](const 方法结果索引项& 项) {
                return 项.完整() && 项.能力键.规则版本 == 方法召回索引规则版本;
            });
    }
};

struct 方法召回索引状态材料 {
    std::uint32_t 规则版本 = 0;
    std::uint64_t 发布版本 = 0;
    std::uint64_t 键数量 = 0;
    std::uint64_t 索引项数量 = 0;
    bool 非权威 = true;

    bool 完整() const {
        return 规则版本 == 方法召回索引规则版本
            && 发布版本 != 0
            && 非权威;
    }
};

struct 方法结果索引读取材料 {
    方法结果能力键 能力键;
    std::uint64_t 发布版本 = 0;
    std::vector<方法结果索引项> 索引项组;
    bool 非权威 = true;

    bool 完整() const {
        if (!能力键.完整() || 发布版本 == 0 || !非权威) {
            return false;
        }
        for (std::size_t 索引 = 0; 索引 < 索引项组.size(); ++索引) {
            if (!索引项组[索引].完整()
                || !(索引项组[索引].能力键 == 能力键)
                || (索引 != 0 && !方法结果索引项稳定小于(
                    索引项组[索引 - 1], 索引项组[索引]))) {
                return false;
            }
        }
        return true;
    }
};

struct 方法召回索引快照材料 {
    方法召回索引状态材料 状态;
    std::vector<方法结果索引项> 索引项组;

    bool 完整() const {
        if (!状态.完整()
            || 状态.索引项数量 != static_cast<std::uint64_t>(索引项组.size())) {
            return false;
        }
        std::uint64_t 键数量 = 0;
        for (std::size_t 索引 = 0; 索引 < 索引项组.size(); ++索引) {
            if (!索引项组[索引].完整()
                || (索引 != 0 && !方法结果索引项稳定小于(
                    索引项组[索引 - 1], 索引项组[索引]))) {
                return false;
            }
            if (索引 == 0
                || !(索引项组[索引 - 1].能力键 == 索引项组[索引].能力键)) {
                ++键数量;
            }
        }
        return 状态.键数量 == 键数量;
    }
};

namespace 方法召回索引细节 {

inline bool 方法登记项相同(const 方法登记项材料& 左, const 方法登记项材料& 右) {
    return 左.登记根 == 右.登记根
        && 左.方法首 == 右.方法首
        && 左.登记关系 == 右.登记关系
        && 左.生命周期状态 == 右.生命周期状态
        && 左.生命周期关系 == 右.生命周期关系
        && 左.生命周期版本 == 右.生命周期版本;
}

inline std::optional<std::vector<方法结果索引项>> 读取一次方法结果索引项组(
    const 方法服务& 方法,
    const 状态服务& 状态,
    const 特征服务& 特征) {
    const auto 来源组 = 方法.读取活跃方法结果规格索引来源组(状态, 特征);
    if (!来源组.has_value()) {
        return std::nullopt;
    }
    std::vector<方法结果索引项> 索引项组;
    索引项组.reserve(来源组->size());
    for (const auto& 来源 : 来源组.value()) {
        if (!来源.完整()) {
            return std::nullopt;
        }
        方法结果索引项 索引项;
        索引项.能力键 = {
            方法召回索引规则版本,
            来源.结果规格.目标特征,
            来源.结果规格.目标状态值材料};
        索引项.方法首 = 来源.登记项.方法首;
        索引项.结果节点 = 来源.结果规格.结果节点;
        索引项.登记关系 = 来源.登记项.登记关系;
        索引项.生命周期关系 = 来源.登记项.生命周期关系;
        索引项.生命周期版本 = 来源.登记项.生命周期版本;
        索引项.结果特征关系 = 来源.结果规格.特征关系;
        索引项.结果目标状态关系 = 来源.结果规格.目标状态关系;
        索引项.结果规格规则版本 = 来源.结果规格.规格规则版本;
        if (!索引项.完整()) {
            return std::nullopt;
        }
        索引项组.push_back(索引项);
    }
    std::sort(索引项组.begin(), 索引项组.end(), 方法结果索引项稳定小于);
    for (std::size_t 索引 = 1; 索引 < 索引项组.size(); ++索引) {
        if (方法结果索引项身份相同(
                索引项组[索引 - 1], 索引项组[索引])) {
            return std::nullopt;
        }
    }
    return 索引项组;
}

}

inline 方法结果索引重建候选 构造方法结果索引重建候选(
    std::uint32_t 规则版本,
    const 方法服务& 方法,
    const 状态服务& 状态,
    const 特征服务& 特征) {
    方法结果索引重建候选 候选;
    候选.规则版本 = 规则版本;
    if (规则版本 != 方法召回索引规则版本) {
        return 候选;
    }
    const auto 首次观察 = 方法召回索引细节::读取一次方法结果索引项组(
        方法, 状态, 特征);
    const auto 复核观察 = 方法召回索引细节::读取一次方法结果索引项组(
        方法, 状态, 特征);
    if (!首次观察.has_value() || !复核观察.has_value()) {
        return 候选;
    }
    if (!方法结果索引项组相同(首次观察.value(), 复核观察.value())) {
        候选.来源状态 = 方法召回索引来源状态::已过期;
        return 候选;
    }
    候选.来源状态 = 方法召回索引来源状态::当前;
    候选.索引项组 = 复核观察.value();
    return 候选;
}

inline 方法召回索引来源状态 复核方法结果索引项来源(
    const 方法结果索引项& 原索引项,
    const 方法服务& 方法,
    const 状态服务& 状态,
    const 特征服务& 特征) {
    if (!原索引项.完整()) {
        return 方法召回索引来源状态::无效;
    }
    const auto 当前登记项 = 方法.读取方法登记项(原索引项.方法首, 状态);
    if (!当前登记项.has_value() || !当前登记项->完整()) {
        return 方法召回索引来源状态::无效;
    }
    if (当前登记项->生命周期状态 != 方法生命周期状态::活跃
        || !(当前登记项->登记关系 == 原索引项.登记关系)
        || !(当前登记项->生命周期关系 == 原索引项.生命周期关系)
        || 当前登记项->生命周期版本 != 原索引项.生命周期版本) {
        return 方法召回索引来源状态::已过期;
    }
    const auto 当前规格 = 方法.读取方法结果规格(
        原索引项.方法首, 原索引项.结果节点, 状态, 特征);
    if (!当前规格.has_value() || !当前规格->完整()) {
        return 方法召回索引来源状态::无效;
    }
    const auto 规格来源状态 = 方法.复核方法结果规格来源(
        当前规格.value(), 状态, 特征);
    if (规格来源状态 == 方法规格来源状态::无效) {
        return 方法召回索引来源状态::无效;
    }
    方法结果能力键 当前能力键{
        方法召回索引规则版本,
        当前规格->目标特征,
        当前规格->目标状态值材料};
    if (规格来源状态 == 方法规格来源状态::已过期
        || !(当前能力键 == 原索引项.能力键)
        || !(当前规格->特征关系 == 原索引项.结果特征关系)
        || !(当前规格->目标状态关系 == 原索引项.结果目标状态关系)
        || 当前规格->规格规则版本 != 原索引项.结果规格规则版本) {
        return 方法召回索引来源状态::已过期;
    }
    return 方法召回索引来源状态::当前;
}

template <typename 能力键哈希器>
class 方法召回索引实现 {
public:
    std::optional<方法召回索引状态材料> 发布重建候选(
        const 方法结果索引重建候选& 重建候选) {
        if (重建候选.规则版本 != 方法召回索引规则版本
            || 重建候选.来源状态 != 方法召回索引来源状态::当前) {
            return std::nullopt;
        }
        if (!重建候选.完整()) {
            (void)追根因检查(false, L"待发布的方法召回索引当前候选内部不完整。");
            return std::nullopt;
        }
        auto 排序索引项组 = 重建候选.索引项组;
        std::sort(排序索引项组.begin(), 排序索引项组.end(), 方法结果索引项稳定小于);
        for (std::size_t 索引 = 1; 索引 < 排序索引项组.size(); ++索引) {
            if (方法结果索引项身份相同(
                    排序索引项组[索引 - 1], 排序索引项组[索引])) {
                return std::nullopt;
            }
        }

        桶映射 候选桶组;
        候选桶组.reserve(排序索引项组.size());
        for (const auto& 索引项 : 排序索引项组) {
            候选桶组[索引项.能力键].push_back(索引项);
        }
        if (!桶组完整(候选桶组)) {
            (void)追根因检查(false, L"待发布的方法召回索引桶组装后内部不完整。");
            return std::nullopt;
        }
        const auto 候选键数量 = static_cast<std::uint64_t>(候选桶组.size());
        const auto 候选索引项数量 = static_cast<std::uint64_t>(排序索引项组.size());

        bool 发布后内部完整 = false;
        方法召回索引状态材料 发布状态;
        {
            std::unique_lock<std::shared_mutex> 锁(索引锁_);
            if (发布版本_ == std::numeric_limits<std::uint64_t>::max()) {
                return std::nullopt;
            }
            桶组_.swap(候选桶组);
            键数量_ = 候选键数量;
            索引项数量_ = 候选索引项数量;
            ++发布版本_;
            发布状态 = 读取状态_已加锁();
            发布后内部完整 = 发布状态.完整()
                && 发布状态.键数量 == 候选键数量
                && 发布状态.索引项数量 == 候选索引项数量;
        }
        if (!发布后内部完整) {
            (void)追根因检查(false, L"方法召回索引替换发布后内部快照不完整。");
            return std::nullopt;
        }
        return 发布状态;
    }

    std::optional<方法结果索引读取材料> 按结果能力键读取候选(
        const 方法结果能力键& 能力键) const {
        if (!能力键.完整()) {
            return std::nullopt;
        }
        方法结果索引读取材料 材料;
        bool 内部完整 = false;
        {
            std::shared_lock<std::shared_mutex> 锁(索引锁_);
            if (发布版本_ == 0) {
                return std::nullopt;
            }
            材料.能力键 = 能力键;
            材料.发布版本 = 发布版本_;
            const auto 位置 = 桶组_.find(能力键);
            if (位置 != 桶组_.end()) {
                材料.索引项组 = 位置->second;
            }
        }
        内部完整 = 材料.完整();
        if (!内部完整) {
            (void)追根因检查(false, L"读取方法召回索引桶时内部材料不完整。");
            return std::nullopt;
        }
        return 材料;
    }

    std::optional<方法召回索引状态材料> 清空() {
        桶映射 原桶组;
        bool 清空后内部完整 = false;
        方法召回索引状态材料 清空状态;
        {
            std::unique_lock<std::shared_mutex> 锁(索引锁_);
            if (发布版本_ == std::numeric_limits<std::uint64_t>::max()) {
                return std::nullopt;
            }
            桶组_.swap(原桶组);
            键数量_ = 0;
            索引项数量_ = 0;
            ++发布版本_;
            清空状态 = 读取状态_已加锁();
            清空后内部完整 = 清空状态.完整()
                && 清空状态.键数量 == 0
                && 清空状态.索引项数量 == 0
                && 桶组_.empty();
        }
        if (!清空后内部完整) {
            (void)追根因检查(false, L"清空方法召回索引后内部快照不完整。");
            return std::nullopt;
        }
        return 清空状态;
    }

    std::uint64_t 读取发布版本() const {
        std::shared_lock<std::shared_mutex> 锁(索引锁_);
        return 发布版本_;
    }

    std::optional<方法召回索引状态材料> 读取索引状态() const {
        方法召回索引状态材料 状态;
        {
            std::shared_lock<std::shared_mutex> 锁(索引锁_);
            if (发布版本_ == 0) {
                return std::nullopt;
            }
            状态 = 读取状态_已加锁();
        }
        if (!状态.完整()) {
            (void)追根因检查(false, L"读取方法召回索引状态时内部快照不完整。");
            return std::nullopt;
        }
        return 状态;
    }

    std::optional<方法召回索引快照材料> 读取快照() const {
        方法召回索引快照材料 快照;
        bool 内部完整 = false;
        {
            std::shared_lock<std::shared_mutex> 锁(索引锁_);
            if (发布版本_ == 0) {
                return std::nullopt;
            }
            快照.状态 = 读取状态_已加锁();
            快照.索引项组.reserve(static_cast<std::size_t>(快照.状态.索引项数量));
            for (const auto& [能力键, 索引项组] : 桶组_) {
                (void)能力键;
                快照.索引项组.insert(
                    快照.索引项组.end(), 索引项组.begin(), 索引项组.end());
            }
        }
        std::sort(快照.索引项组.begin(), 快照.索引项组.end(), 方法结果索引项稳定小于);
        内部完整 = 快照.完整();
        if (!内部完整) {
            (void)追根因检查(false, L"读取方法召回索引快照时内部材料不完整。");
            return std::nullopt;
        }
        return 快照;
    }

private:
    using 桶映射 = std::unordered_map<
        方法结果能力键,
        std::vector<方法结果索引项>,
        能力键哈希器>;

    static bool 桶组完整(const 桶映射& 桶组) {
        for (const auto& [能力键, 索引项组] : 桶组) {
            if (!能力键.完整() || 索引项组.empty()) {
                return false;
            }
            for (std::size_t 索引 = 0; 索引 < 索引项组.size(); ++索引) {
                if (!索引项组[索引].完整()
                    || !(索引项组[索引].能力键 == 能力键)
                    || (索引 != 0 && !方法结果索引项稳定小于(
                        索引项组[索引 - 1], 索引项组[索引]))) {
                    return false;
                }
            }
        }
        return true;
    }

    方法召回索引状态材料 读取状态_已加锁() const {
        return {
            方法召回索引规则版本,
            发布版本_,
            键数量_,
            索引项数量_,
            true};
    }

    mutable std::shared_mutex 索引锁_;
    桶映射 桶组_;
    std::uint64_t 发布版本_ = 0;
    std::uint64_t 键数量_ = 0;
    std::uint64_t 索引项数量_ = 0;
};

using 方法召回索引 = 方法召回索引实现<方法结果能力键哈希>;

}
