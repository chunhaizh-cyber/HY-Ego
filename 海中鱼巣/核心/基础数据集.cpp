#include "基础数据集.h"

#include <limits>
#include <mutex>
#include <utility>

namespace 海中鱼巣 {

基础数据集 全局基础数据集;

namespace {

bool 有效(基础外部关系类型 类型) noexcept {
    return 类型 == 基础外部关系类型::父子
        || 类型 == 基础外部关系类型::兄弟;
}

} // namespace

稳定编码 基础数据集::分配编码() noexcept {
    if (下一个编码_ == 0
        || 下一个编码_ == (std::numeric_limits<std::uint64_t>::max)()) {
        return {};
    }
    return {下一个编码_++};
}

bool 基础数据集::节点存在(稳定编码 节点) const noexcept {
    return 有效(节点) && 节点_.contains(节点.值);
}

bool 基础数据集::节点被引用(稳定编码 节点) const noexcept {
    for (const auto& [_, 关系] : 外部关系_) {
        if (关系.源节点 == 节点 || 关系.目标节点 == 节点) return true;
    }
    for (const auto& [_, 字段] : 字段关系_) {
        if (字段.所属节点 == 节点 || 字段.字段节点 == 节点) return true;
        if (const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            目标 && *目标 == 节点) return true;
    }
    return false;
}

稳定编码 基础数据集::新建节点() noexcept {
    try {
        std::unique_lock 锁(互斥_);
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        节点_.emplace(编码.值, 基础节点{编码});
        return 编码;
    } catch (...) {
        return {};
    }
}

稳定编码 基础数据集::新建节点(稳定编码 上级节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(上级节点)) return {};

        const auto 节点编码 = 分配编码();
        const auto 关系编码 = 分配编码();
        if (!有效(节点编码) || !有效(关系编码)) return {};

        const auto [节点位置, 节点已插入] =
            节点_.emplace(节点编码.值, 基础节点{节点编码});
        if (!节点已插入) return {};
        try {
            const auto [_, 关系已插入] = 外部关系_.emplace(关系编码.值,
                基础外部关系{关系编码, 上级节点, 节点编码,
                    基础外部关系类型::父子, 0});
            if (!关系已插入) {
                节点_.erase(节点位置);
                return {};
            }
        } catch (...) {
            节点_.erase(节点位置);
            throw;
        }
        return 节点编码;
    } catch (...) {
        return {};
    }
}

bool 基础数据集::删除节点(稳定编码 节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(节点) || 节点被引用(节点)) return false;
        return 节点_.erase(节点.值) == 1;
    } catch (...) {
        return false;
    }
}

std::optional<基础节点> 基础数据集::查询节点(稳定编码 节点) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 节点_.find(节点.值);
        return 位置 == 节点_.end()
            ? std::nullopt : std::optional<基础节点>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

稳定编码 基础数据集::添加关系(稳定编码 源节点, 稳定编码 目标节点,
    基础外部关系类型 类型, std::int64_t 角色或顺序) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(源节点) || !节点存在(目标节点) || !有效(类型)) return {};
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        外部关系_.emplace(编码.值,
            基础外部关系{编码, 源节点, 目标节点, 类型, 角色或顺序});
        return 编码;
    } catch (...) {
        return {};
    }
}

bool 基础数据集::修改关系(稳定编码 关系, 稳定编码 源节点,
    稳定编码 目标节点, 基础外部关系类型 类型,
    std::int64_t 角色或顺序) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(源节点) || !节点存在(目标节点) || !有效(类型)) return false;
        const auto 位置 = 外部关系_.find(关系.值);
        if (位置 == 外部关系_.end()) return false;
        位置->second = {关系, 源节点, 目标节点, 类型, 角色或顺序};
        return true;
    } catch (...) {
        return false;
    }
}

bool 基础数据集::删除关系(稳定编码 关系) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        return 有效(关系) && 外部关系_.erase(关系.值) == 1;
    } catch (...) {
        return false;
    }
}

std::optional<基础外部关系> 基础数据集::查询关系(稳定编码 关系) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 外部关系_.find(关系.值);
        return 位置 == 外部关系_.end()
            ? std::nullopt : std::optional<基础外部关系>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<基础外部关系> 基础数据集::查询源关系(
    稳定编码 源节点, 基础外部关系类型 类型) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础外部关系> 结果;
    if (!节点存在(源节点) || !有效(类型)) return 结果;
    for (const auto& [_, 关系] : 外部关系_) {
        if (关系.源节点 == 源节点 && 关系.类型 == 类型) 结果.push_back(关系);
    }
    return 结果;
}

std::vector<基础外部关系> 基础数据集::查询目标关系(
    稳定编码 目标节点, 基础外部关系类型 类型) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础外部关系> 结果;
    if (!节点存在(目标节点) || !有效(类型)) return 结果;
    for (const auto& [_, 关系] : 外部关系_) {
        if (关系.目标节点 == 目标节点 && 关系.类型 == 类型) 结果.push_back(关系);
    }
    return 结果;
}

稳定编码 基础数据集::添加字段值(
    稳定编码 节点, 稳定编码 字段节点, 基础值 值) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(节点) || !节点存在(字段节点)) return {};
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        字段关系_.emplace(编码.值,
            基础字段关系{编码, 节点, 字段节点, std::move(值)});
        return 编码;
    } catch (...) {
        return {};
    }
}

稳定编码 基础数据集::添加字段节点(
    稳定编码 节点, 稳定编码 字段节点, 稳定编码 目标节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(节点) || !节点存在(字段节点) || !节点存在(目标节点)) return {};
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        字段关系_.emplace(编码.值,
            基础字段关系{编码, 节点, 字段节点, 目标节点});
        return 编码;
    } catch (...) {
        return {};
    }
}

bool 基础数据集::修改字段值(稳定编码 字段关系, 基础值 值) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
        位置->second.内容 = std::move(值);
        return true;
    } catch (...) {
        return false;
    }
}

bool 基础数据集::修改字段节点(
    稳定编码 字段关系, 稳定编码 目标节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(目标节点)) return false;
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
        位置->second.内容 = 目标节点;
        return true;
    } catch (...) {
        return false;
    }
}

bool 基础数据集::删除字段(稳定编码 字段关系) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        return 有效(字段关系) && 字段关系_.erase(字段关系.值) == 1;
    } catch (...) {
        return false;
    }
}

std::optional<基础字段关系> 基础数据集::查询字段关系(
    稳定编码 字段关系) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 字段关系_.find(字段关系.值);
        return 位置 == 字段关系_.end()
            ? std::nullopt : std::optional<基础字段关系>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<基础字段关系> 基础数据集::查询字段(
    稳定编码 节点, 稳定编码 字段节点) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础字段关系> 结果;
    if (!节点存在(节点) || !节点存在(字段节点)) return 结果;
    for (const auto& [_, 字段] : 字段关系_) {
        if (字段.所属节点 == 节点 && 字段.字段节点 == 字段节点) 结果.push_back(字段);
    }
    return 结果;
}

std::vector<基础字段关系> 基础数据集::查询目标字段(
    稳定编码 目标节点, 稳定编码 字段节点) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础字段关系> 结果;
    if (!节点存在(目标节点) || !节点存在(字段节点)) return 结果;
    for (const auto& [_, 字段] : 字段关系_) {
        if (字段.字段节点 != 字段节点) continue;
        const auto* 内容节点 = std::get_if<稳定编码>(&字段.内容);
        if (内容节点 && *内容节点 == 目标节点) 结果.push_back(字段);
    }
    return 结果;
}

} // namespace 海中鱼巣
