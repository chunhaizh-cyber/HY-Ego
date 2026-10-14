#include "基础数据集.h"
#include "容错检查.h"

#include <limits>
#include <mutex>
#include <new>
#include <stdexcept>
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
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"基础数据集::查询节点",
            L"节点读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"基础数据集::查询节点",
            L"节点读取发生未知C++异常，未形成业务结果。");
    }
}

基础节点建立结果 基础数据集::原子建立节点(
    const 基础节点建立请求& 请求) noexcept {
    using 状态 = 基础结构操作状态;
    try {
        // 单节点入口只适配值式请求，发布仍经同一个组构建实现。
        std::vector<基础组内新节点> 组请求(1);
        组请求.front().字段组.reserve(请求.字段组.size());
        for (const auto& 字段 : 请求.字段组) {
            if (const auto* 值 = std::get_if<基础值>(&字段.内容))
                组请求.front().字段组.push_back({字段.字段节点, *值});
            else if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容))
                组请求.front().字段组.push_back({字段.字段节点, *引用});
            else return {状态::入口拒绝, {}};
        }
        std::unique_lock 锁(互斥_);
        const auto 结果 = 原子建立节点组已加锁(组请求, 请求.上级节点);
        if (结果.状态 != 状态::已完成) return {结果.状态, {}};
        return {状态::已完成, 结果.节点组.front()};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (...) { return {状态::内部不一致, {}}; }
}

基础节点组建立结果 基础数据集::原子建立节点组(
    const std::vector<基础组内新节点>& 请求) noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::unique_lock 锁(互斥_);
        return 原子建立节点组已加锁(请求, std::nullopt);
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (...) { return {状态::内部不一致, {}}; }
}

基础节点组建立结果 基础数据集::原子建立节点组已加锁(
    const std::vector<基础组内新节点>& 请求,
    std::optional<稳定编码> 上级节点) {
    using 状态 = 基础结构操作状态;
    if (请求.empty() || (上级节点 && 请求.size() != 1))
        return {状态::入口拒绝, {}};
    if (上级节点 && !有效(*上级节点)) return {状态::入口拒绝, {}};
    if (上级节点 && !节点存在(*上级节点)) return {状态::引用不存在, {}};
    for (const auto& 节点 : 请求) {
        for (const auto& 字段 : 节点.字段组) {
            if (!有效(字段.字段节点) || 字段.内容.valueless_by_exception())
                return {状态::入口拒绝, {}};
            if (!节点存在(字段.字段节点)) return {状态::引用不存在, {}};
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) {
                if (!有效(*引用)) return {状态::入口拒绝, {}};
                if (!节点存在(*引用)) return {状态::引用不存在, {}};
            } else if (const auto* 组内 = std::get_if<基础组内节点引用>(&字段.内容)) {
                if (组内->索引 >= 请求.size()) return {状态::入口拒绝, {}};
            } else if (std::get<基础值>(字段.内容).材料.valueless_by_exception()) {
                return {状态::入口拒绝, {}};
            }
        }
    }

    // 独占锁内构造隔离候选；分配水位属于同一分配器，失败也不回退。
    // 这里不调用任何上层服务、回调、I/O或本对象公开入口。
    auto 候选节点 = 节点_;
    auto 候选字段 = 字段关系_;
    auto 候选关系 = 外部关系_;
    基础节点组建立结果 结果{状态::已完成, {}};
    结果.节点组.reserve(请求.size());
    const auto 编码已占用 = [&](稳定编码 编码) {
        return 候选节点.contains(编码.值) || 候选字段.contains(编码.值)
            || 候选关系.contains(编码.值);
    };
    for (std::size_t 索引 = 0; 索引 < 请求.size(); ++索引) {
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {状态::资源失败, {}};
        if (编码已占用(编码)) return {状态::内部不一致, {}};
        候选节点.emplace(编码.值, 基础节点{编码});
        结果.节点组.push_back(编码);
    }
    for (std::size_t 索引 = 0; 索引 < 请求.size(); ++索引) {
        for (const auto& 字段 : 请求[索引].字段组) {
            基础字段内容 内容;
            if (const auto* 值 = std::get_if<基础值>(&字段.内容)) 内容 = *值;
            else if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) 内容 = *引用;
            else 内容 = 结果.节点组[std::get<基础组内节点引用>(字段.内容).索引];
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::资源失败, {}};
            if (编码已占用(编码)) return {状态::内部不一致, {}};
            候选字段.emplace(编码.值, 基础字段关系{
                编码, 结果.节点组[索引], 字段.字段节点, std::move(内容)});
        }
    }
    if (上级节点) {
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {状态::资源失败, {}};
        if (编码已占用(编码)) return {状态::内部不一致, {}};
        候选关系.emplace(编码.值, 基础外部关系{
            编码, *上级节点, 结果.节点组.front(), 基础外部关系类型::父子, 0});
    }
    // map使用标准分配器和无状态比较器，交换不分配、不抛异常。
    // 读者只能在本次独占锁释放后观察到全部候选。
    节点_.swap(候选节点);
    字段关系_.swap(候选字段);
    外部关系_.swap(候选关系);
    return 结果;
}

基础节点结构结果 基础数据集::查询完整节点结构(稳定编码 节点) const noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 节点_.find(节点.值);
        if (位置 == 节点_.end()) return {状态::节点不存在, {}};
        基础节点结构 结构{位置->second, {}};
        for (const auto& [编码, 字段] : 字段关系_) {
            if (字段.所属节点 == 节点) 结构.字段组.push_back(字段);
        }
        return {状态::已完成, std::move(结构)};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (...) { return {状态::内部不一致, {}}; }
}

基础引用节点结构结果 基础数据集::查询字段引用节点结构(
    稳定编码 目标节点, 稳定编码 字段节点) const noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::shared_lock 锁(互斥_);
        if (!节点存在(目标节点) || !节点存在(字段节点))
            return {状态::节点不存在, {}, {}};
        基础引用节点结构结果 结果{状态::已完成, {}, {}};
        std::map<std::uint64_t, 基础节点结构> 来源组;
        for (const auto& [编码, 字段] : 字段关系_) {
            if (字段.字段节点 != 字段节点) continue;
            const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
            if (!引用 || *引用 != 目标节点) continue;
            const auto 来源 = 节点_.find(字段.所属节点.值);
            if (来源 == 节点_.end()) return {状态::内部不一致, {}, {}};
            结果.引用字段组.push_back(字段);
            来源组.try_emplace(来源->first, 基础节点结构{来源->second, {}});
        }
        for (const auto& [编码, 字段] : 字段关系_) {
            const auto 来源 = 来源组.find(字段.所属节点.值);
            if (来源 != 来源组.end()) 来源->second.字段组.push_back(字段);
        }
        结果.来源节点组.reserve(来源组.size());
        for (auto& [编码, 来源] : 来源组) 结果.来源节点组.push_back(std::move(来源));
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}, {}};
    } catch (...) { return {状态::内部不一致, {}, {}}; }
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
