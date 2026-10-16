#include "基础数据集.h"
#include "容错检查.h"

#include <algorithm>
#include <limits>
#include <mutex>
#include <set>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace 海中鱼巣 {

基础数据集 全局基础数据集;

namespace {

bool 有效(基础外部关系类型 类型) noexcept {
    return 类型 == 基础外部关系类型::父子
        || 类型 == 基础外部关系类型::兄弟;
}

bool 联合快照相等(const 基础联合节点快照& 左,
    const 基础联合节点快照& 右) {
    return 左.结构.节点 == 右.结构.节点
        && 左.结构.字段组 == 右.结构.字段组
        && 左.入向外部关系组 == 右.入向外部关系组
        && 左.出向外部关系组 == 右.出向外部关系组
        && 左.入向字段组 == 右.入向字段组;
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

void 基础数据集::插入字段已加锁(基础字段关系 字段) {
    const auto 编码 = 字段.编码.值;
    const 字段索引键 所属键{字段.所属节点.值, 字段.字段节点.值, 编码};
    const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
    const std::optional<字段索引键> 目标键 = 引用
        ? std::optional<字段索引键>{{字段.字段节点.值, 引用->值, 编码}} : std::nullopt;
    const auto [位置, 已插入] = 字段关系_.emplace(编码, std::move(字段));
    if (!已插入) throw std::logic_error("field identity collision");
    bool 所属已插入 = false;
    try {
        所属已插入 = 所属字段索引_.insert(所属键).second;
        if (!所属已插入) throw std::logic_error("forward field index collision");
        if (目标键 && !目标字段索引_.insert(*目标键).second)
            throw std::logic_error("reverse field index collision");
    } catch (...) {
        if (所属已插入) 所属字段索引_.erase(所属键);
        字段关系_.erase(位置);
        throw;
    }
}

void 基础数据集::核验字段索引已加锁(const 基础字段关系& 字段) const {
    if (!所属字段索引_.contains({字段.所属节点.值, 字段.字段节点.值, 字段.编码.值}))
        throw std::logic_error("forward field index missing");
    if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
        引用 && !目标字段索引_.contains({字段.字段节点.值, 引用->值, 字段.编码.值}))
        throw std::logic_error("reverse field index missing");
}

void 基础数据集::删除字段已加锁(const 基础字段关系& 字段) noexcept {
    所属字段索引_.erase({字段.所属节点.值, 字段.字段节点.值, 字段.编码.值});
    if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容))
        目标字段索引_.erase({字段.字段节点.值, 引用->值, 字段.编码.值});
    字段关系_.erase(字段.编码.值);
}

void 基础数据集::读取节点字段已加锁(
    稳定编码 节点, std::vector<基础字段关系>& 结果) const {
    for (auto 位置 = 所属字段索引_.lower_bound({节点.值, 0, 0});
        位置 != 所属字段索引_.end() && std::get<0>(*位置) == 节点.值; ++位置)
        结果.push_back(字段关系_.at(std::get<2>(*位置)));
    // 跨字段类型的索引顺序不是旧表的编码顺序；对外仍保持原顺序。
    std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
        return 左.编码.值 < 右.编码.值;
    });
}

基础联合节点快照 基础数据集::读取联合快照已加锁(稳定编码 节点) const {
    基础联合节点快照 结果;
    结果.结构.节点 = 节点_.at(节点.值);
    for (const auto& [_, 字段] : 字段关系_) {
        if (字段.所属节点 == 节点) {
            核验字段索引已加锁(字段);
            结果.结构.字段组.push_back(字段);
        }
    }
    for (auto 位置 = 所属字段索引_.lower_bound({节点.值, 0, 0});
        位置 != 所属字段索引_.end() && std::get<0>(*位置) == 节点.值; ++位置) {
        const auto 字段位置 = 字段关系_.find(std::get<2>(*位置));
        if (字段位置 == 字段关系_.end() || 字段位置->second.所属节点 != 节点
            || 字段位置->second.字段节点.值 != std::get<1>(*位置))
            throw std::logic_error("union forward field index inconsistent");
    }
    for (const auto& [_, 关系] : 外部关系_) {
        if (关系.目标节点 == 节点) 结果.入向外部关系组.push_back(关系);
        if (关系.源节点 == 节点) 结果.出向外部关系组.push_back(关系);
    }
    for (const auto& [_, 字段] : 字段关系_) {
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (目标 && *目标 == 节点) {
            核验字段索引已加锁(字段);
            结果.入向字段组.push_back(字段);
        }
    }
    for (const auto& [键, 目标, 编码] : 目标字段索引_) {
        if (目标 != 节点.值) continue;
        const auto 位置 = 字段关系_.find(编码);
        if (位置 == 字段关系_.end() || 位置->second.字段节点.值 != 键
            || !std::holds_alternative<稳定编码>(位置->second.内容)
            || std::get<稳定编码>(位置->second.内容) != 节点)
            throw std::logic_error("union reverse field index inconsistent");
    }
    return 结果;
}

基础互引节点操作状态 基础数据集::复核联合预期已加锁(
    const std::vector<基础联合节点快照>& 预期,
    const std::set<std::uint64_t>& 必需) const {
    using 状态 = 基础互引节点操作状态;
    std::set<std::uint64_t> 已提供;
    for (const auto& 项 : 预期) {
        const auto 编码 = 项.结构.节点.编码;
        if (!有效(编码) || !已提供.insert(编码.值).second) return 状态::入口拒绝;
    }
    if (!std::includes(已提供.begin(), 已提供.end(), 必需.begin(), 必需.end()))
        return 状态::入口拒绝;
    for (const auto& 项 : 预期) {
        const auto 编码 = 项.结构.节点.编码;
        if (!节点存在(编码)) return 状态::节点不存在;
        if (!联合快照相等(读取联合快照已加锁(编码), 项))
            return 状态::预期不符;
    }
    return 状态::已完成;
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

bool 基础数据集::有入向字段引用(稳定编码 节点) const noexcept {
    return 有入向字段引用(节点, {});
}

bool 基础数据集::有入向字段引用(稳定编码 节点, 稳定编码 忽略所属节点) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        if (!节点存在(节点)) throw std::logic_error("reference target missing");
        for (const auto& [编码, 字段] : 字段关系_) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (字段.所属节点 != 节点 && 字段.所属节点 != 忽略所属节点
                && 目标 && *目标 == 节点) return true;
        }
        return false;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取入向字段引用",
            L"引用检查失败，禁止解释为无引用。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取入向字段引用", L"未知工程异常。");
    }
}

基础结构操作状态 基础数据集::原子删除节点结构(
    const std::vector<稳定编码>& 节点组,
    const std::vector<稳定编码>& 字段关系组,
    const std::vector<稳定编码>& 外部关系组) noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::unique_lock 锁(互斥_);
        if (节点组.empty()) return 状态::入口拒绝;
        const auto 合法集合 = [](const auto& 组, auto& 集合) {
            for (const auto 编码 : 组)
                if (!有效(编码) || !集合.insert(编码.值).second) return false;
            return true;
        };
        std::set<std::uint64_t> 节点集, 字段集, 关系集;
        if (!合法集合(节点组, 节点集) || !合法集合(字段关系组, 字段集)
            || !合法集合(外部关系组, 关系集)) return 状态::入口拒绝;
        for (const auto 编码 : 节点集)
            if (!节点_.contains(编码)) return 状态::节点不存在;
        for (const auto 编码 : 字段集) {
            const auto 位置 = 字段关系_.find(编码);
            if (位置 == 字段关系_.end()) return 状态::引用不存在;
            if (!节点集.contains(位置->second.所属节点.值)) return 状态::入口拒绝;
            核验字段索引已加锁(位置->second);
        }
        for (const auto 编码 : 关系集) {
            const auto 位置 = 外部关系_.find(编码);
            if (位置 == 外部关系_.end()) return 状态::引用不存在;
            if (!节点集.contains(位置->second.源节点.值)
                && !节点集.contains(位置->second.目标节点.值)) return 状态::入口拒绝;
        }
        for (const auto& [编码, 字段] : 字段关系_) {
            if (字段集.contains(编码)) continue;
            // 私有字段遗漏也是未闭合结构，不能只删除节点。
            if (节点集.contains(字段.所属节点.值)) return 状态::内部不一致;
            if (节点集.contains(字段.字段节点.值)) return 状态::引用仍存在;
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (目标 && 节点集.contains(目标->值)) return 状态::引用仍存在;
        }
        for (const auto& [编码, 关系] : 外部关系_) {
            if (!关系集.contains(编码) && 节点集.contains(关系.源节点.值)
                && 节点集.contains(关系.目标节点.值)) return 状态::内部不一致;
            if (!关系集.contains(编码) && (节点集.contains(关系.源节点.值)
                || 节点集.contains(关系.目标节点.值))) return 状态::引用仍存在;
        }
        // 此后只在已核验的标准map/set中删除，不再分配；无外部调用。
        for (const auto 编码 : 字段集) 删除字段已加锁(字段关系_.at(编码));
        for (const auto 编码 : 关系集) 外部关系_.erase(外部关系_.find(编码));
        for (const auto 编码 : 节点集) 节点_.erase(节点_.find(编码));
        return 状态::已完成;
    } catch (const std::bad_alloc&) { return 状态::资源失败;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除节点结构", L"字段结构与索引删除异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除节点结构", L"字段结构与索引删除未知异常。");
    }
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
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点", L"字段与索引候选建立异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点", L"字段与索引候选建立未知异常。");
    }
}

基础节点组建立结果 基础数据集::原子建立节点组(
    const std::vector<基础组内新节点>& 请求) noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::unique_lock 锁(互斥_);
        return 原子建立节点组已加锁(请求, std::nullopt);
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点组", L"字段与索引候选建立异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点组", L"字段与索引候选建立未知异常。");
    }
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
    auto 候选所属索引 = 所属字段索引_;
    auto 候选目标索引 = 目标字段索引_;
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
            const auto& 新字段 = 候选字段.at(编码.值);
            if (!候选所属索引.emplace(新字段.所属节点.值, 新字段.字段节点.值, 编码.值).second)
                throw std::logic_error("atomic forward field index collision");
            if (const auto* 引用 = std::get_if<稳定编码>(&新字段.内容);
                引用 && !候选目标索引.emplace(新字段.字段节点.值, 引用->值, 编码.值).second)
                throw std::logic_error("atomic reverse field index collision");
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
    所属字段索引_.swap(候选所属索引);
    目标字段索引_.swap(候选目标索引);
    return 结果;
}

基础节点组追加结果 基础数据集::原子追加节点组(
    const 基础节点组追加请求& 请求) noexcept {
    using 状态 = 基础节点组追加状态;
    try {
        std::unique_lock 锁(互斥_);
        if (请求.新节点组.empty() && 请求.追加字段组.empty()
            && 请求.替换字段组.empty() && 请求.追加外部关系组.empty())
            return {状态::入口拒绝, {}, {}};
        const auto 检查字段 = [&](稳定编码 键, const 基础组内字段内容& 内容) {
            if (!有效(键) || 内容.valueless_by_exception()) return 状态::入口拒绝;
            if (!节点存在(键)) return 状态::引用不存在;
            if (const auto* 引用 = std::get_if<稳定编码>(&内容)) {
                if (!有效(*引用)) return 状态::入口拒绝;
                if (!节点存在(*引用)) return 状态::引用不存在;
            } else if (const auto* 内部 = std::get_if<基础组内节点引用>(&内容)) {
                if (内部->索引 >= 请求.新节点组.size()) return 状态::入口拒绝;
            } else if (std::get<基础值>(内容).材料.valueless_by_exception()) return 状态::入口拒绝;
            return 状态::已完成;
        };
        std::set<稳定编码> 宿主组;
        for (const auto& 结构 : 请求.预期既有节点组) {
            const auto 宿主 = 结构.节点.编码;
            if (!有效(宿主) || !宿主组.insert(宿主).second) return {状态::入口拒绝, {}, {}};
            if (!节点存在(宿主)) return {状态::节点不存在, {}, {}};
            auto 预期字段 = 结构.字段组;
            std::sort(预期字段.begin(), 预期字段.end(), [](const auto& 左, const auto& 右) {
                return 左.编码 < 右.编码;
            });
            稳定编码 上项;
            for (const auto& 项 : 预期字段) {
                if (!有效(项.编码) || 项.编码 == 上项 || 项.所属节点 != 宿主
                    || 项.内容.valueless_by_exception()) return {状态::入口拒绝, {}, {}};
                上项 = 项.编码;
                const auto 内容 = std::visit([](const auto& 值) -> 基础组内字段内容 { return 值; }, 项.内容);
                const auto 检查 = 检查字段(项.字段节点, 内容);
                if (检查 != 状态::已完成) return {检查, {}, {}};
            }
            std::vector<基础字段关系> 当前字段;
            读取节点字段已加锁(宿主, 当前字段);
            std::sort(当前字段.begin(), 当前字段.end(), [](const auto& 左, const auto& 右) {
                return 左.编码 < 右.编码;
            });
            if (当前字段 != 预期字段) return {状态::预期不符, {}, {}};
        }
        std::set<稳定编码> 联合宿主组;
        for (const auto& 快照 : 请求.预期联合节点组) {
            const auto 宿主 = 快照.结构.节点.编码;
            if (!有效(宿主) || !联合宿主组.insert(宿主).second)
                return {状态::入口拒绝, {}, {}};
            if (!节点存在(宿主)) return {状态::节点不存在, {}, {}};
            if (!联合快照相等(读取联合快照已加锁(宿主), 快照))
                return {状态::预期不符, {}, {}};
            宿主组.insert(宿主);
        }
        std::set<稳定编码> 替换字段码;
        for (const auto& 项 : 请求.替换字段组) {
            if (!有效(项.字段关系) || !替换字段码.insert(项.字段关系).second
                || 项.新内容.valueless_by_exception()) return {状态::入口拒绝, {}, {}};
            const auto 位置 = 字段关系_.find(项.字段关系.值);
            if (位置 == 字段关系_.end() || !联合宿主组.contains(位置->second.所属节点))
                return {状态::入口拒绝, {}, {}};
            const auto 内容 = std::visit([](const auto& 值) -> 基础组内字段内容 { return 值; }, 项.新内容);
            const auto 检查 = 检查字段(位置->second.字段节点, 内容);
            if (检查 != 状态::已完成) return {检查, {}, {}};
            const auto& 旧内容 = std::as_const(位置->second.内容);
            for (const auto* 引用 : {std::get_if<稳定编码>(&旧内容),
                    std::get_if<稳定编码>(&项.新内容)})
                if (引用 && !联合宿主组.contains(*引用)) return {状态::入口拒绝, {}, {}};
            核验字段索引已加锁(位置->second);
        }
        const auto 检查端点 = [&](const std::variant<稳定编码, 基础组内节点引用>& 端点) {
            if (端点.valueless_by_exception()) return 状态::入口拒绝;
            if (const auto* 编码 = std::get_if<稳定编码>(&端点)) {
                if (!有效(*编码) || !联合宿主组.contains(*编码)) return 状态::入口拒绝;
                return 节点存在(*编码) ? 状态::已完成 : 状态::引用不存在;
            }
            return std::get<基础组内节点引用>(端点).索引 < 请求.新节点组.size()
                ? 状态::已完成 : 状态::入口拒绝;
        };
        for (const auto& 项 : 请求.追加外部关系组) {
            if (!有效(项.类型)) return {状态::入口拒绝, {}, {}};
            const auto 源检查 = 检查端点(项.源节点), 目标检查 = 检查端点(项.目标节点);
            if (源检查 != 状态::已完成) return {源检查, {}, {}};
            if (目标检查 != 状态::已完成) return {目标检查, {}, {}};
        }
        for (const auto& 节点 : 请求.新节点组) for (const auto& 项 : 节点.字段组) {
            const auto 检查 = 检查字段(项.字段节点, 项.内容);
            if (检查 != 状态::已完成) return {检查, {}, {}};
        }
        for (const auto& 项 : 请求.追加字段组) {
            if (!有效(项.所属节点) || !宿主组.contains(项.所属节点)) return {状态::入口拒绝, {}, {}};
            if ((!请求.替换字段组.empty() || !请求.追加外部关系组.empty())
                && !联合宿主组.contains(项.所属节点)) return {状态::入口拒绝, {}, {}};
            const auto 检查 = 检查字段(项.字段节点, 项.内容);
            if (检查 != 状态::已完成) return {检查, {}, {}};
        }
        auto 候选节点 = 节点_;
        auto 候选字段 = 字段关系_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        // 纯追加字段仍保持旧成本；只有新增外部边时复制关系表。
        std::optional<decltype(外部关系_)> 候选关系;
        if (!请求.追加外部关系组.empty()) 候选关系 = 外部关系_;
        基础节点组追加结果 结果{状态::已完成, {}, {}};
        结果.新节点组.reserve(请求.新节点组.size());
        结果.追加字段关系组.reserve(请求.追加字段组.size());
        结果.追加外部关系组.reserve(请求.追加外部关系组.size());
        const auto 已占用 = [&](稳定编码 编码) {
            return 候选节点.contains(编码.值) || 候选字段.contains(编码.值)
                || (候选关系 ? 候选关系->contains(编码.值) : 外部关系_.contains(编码.值));
        };
        for (std::size_t i = 0; i < 请求.新节点组.size(); ++i) {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::资源失败, {}, {}};
            if (已占用(编码)) throw std::logic_error("append node identity collision");
            候选节点.emplace(编码.值, 基础节点{编码});
            结果.新节点组.push_back(编码);
        }
        for (const auto& 项 : 请求.替换字段组) {
            auto& 字段 = 候选字段.at(项.字段关系.值);
            if (const auto* 旧 = std::get_if<稳定编码>(&字段.内容);
                旧 && 候选目标索引.erase({字段.字段节点.值, 旧->值, 字段.编码.值}) != 1)
                throw std::logic_error("append replacement reverse index missing");
            字段.内容 = 项.新内容;
            if (const auto* 新 = std::get_if<稳定编码>(&字段.内容);
                新 && !候选目标索引.emplace(字段.字段节点.值, 新->值, 字段.编码.值).second)
                throw std::logic_error("append replacement reverse index collision");
        }
        const auto 加字段 = [&](稳定编码 宿主, 稳定编码 键, const 基础组内字段内容& 源) {
            基础字段内容 内容;
            if (const auto* 值 = std::get_if<基础值>(&源)) 内容 = *值;
            else if (const auto* 引用 = std::get_if<稳定编码>(&源)) 内容 = *引用;
            else 内容 = 结果.新节点组.at(std::get<基础组内节点引用>(源).索引);
            const auto 编码 = 分配编码();
            if (!有效(编码)) return 编码;
            if (已占用(编码)) throw std::logic_error("append field identity collision");
            候选字段.emplace(编码.值, 基础字段关系{编码, 宿主, 键, std::move(内容)});
            if (!候选所属索引.emplace(宿主.值, 键.值, 编码.值).second)
                throw std::logic_error("append forward index collision");
            if (const auto* 引用 = std::get_if<稳定编码>(&候选字段.at(编码.值).内容);
                引用 && !候选目标索引.emplace(键.值, 引用->值, 编码.值).second)
                throw std::logic_error("append reverse index collision");
            return 编码;
        };
        for (std::size_t i = 0; i < 请求.新节点组.size(); ++i)
            for (const auto& 项 : 请求.新节点组[i].字段组)
                if (!有效(加字段(结果.新节点组[i], 项.字段节点, 项.内容))) return {状态::资源失败, {}, {}};
        for (const auto& 项 : 请求.追加字段组) {
            const auto 字段码 = 加字段(项.所属节点, 项.字段节点, 项.内容);
            if (!有效(字段码)) return {状态::资源失败, {}, {}};
            结果.追加字段关系组.push_back(字段码);
        }
        const auto 解析端点 = [&](const std::variant<稳定编码, 基础组内节点引用>& 端点) {
            if (const auto* 编码 = std::get_if<稳定编码>(&端点)) return *编码;
            return 结果.新节点组.at(std::get<基础组内节点引用>(端点).索引);
        };
        for (const auto& 项 : 请求.追加外部关系组) {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::资源失败, {}, {}};
            if (已占用(编码)) throw std::logic_error("append relation identity collision");
            候选关系->emplace(编码.值, 基础外部关系{编码,
                解析端点(项.源节点), 解析端点(项.目标节点), 项.类型, 项.角色或顺序});
            结果.追加外部关系组.push_back(编码);
        }
        // 全部候选与索引已闭合；以下交换不再分配。
        节点_.swap(候选节点); 字段关系_.swap(候选字段);
        所属字段索引_.swap(候选所属索引); 目标字段索引_.swap(候选目标索引);
        if (候选关系) 外部关系_.swap(*候选关系);
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加节点组", L"追加结构或索引异常", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加节点组", L"未知追加异常");
    }
}

基础节点结构结果 基础数据集::查询完整节点结构(稳定编码 节点) const noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 节点_.find(节点.值);
        if (位置 == 节点_.end()) return {状态::节点不存在, {}};
        基础节点结构 结构{位置->second, {}};
        读取节点字段已加锁(节点, 结构.字段组);
        return {状态::已完成, std::move(结构)};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询完整节点结构", L"字段索引读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询完整节点结构", L"字段索引读取未知异常。");
    }
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
        for (auto 位置 = 目标字段索引_.lower_bound({字段节点.值, 目标节点.值, 0});
            位置 != 目标字段索引_.end() && std::get<0>(*位置) == 字段节点.值
                && std::get<1>(*位置) == 目标节点.值; ++位置) {
            const auto& 字段 = 字段关系_.at(std::get<2>(*位置));
            const auto 来源 = 节点_.find(字段.所属节点.值);
            if (来源 == 节点_.end()) return {状态::内部不一致, {}, {}};
            结果.引用字段组.push_back(字段);
            来源组.try_emplace(来源->first, 基础节点结构{来源->second, {}});
        }
        结果.来源节点组.reserve(来源组.size());
        for (auto& [编码, 来源] : 来源组) {
            读取节点字段已加锁(来源.节点.编码, 来源.字段组);
            结果.来源节点组.push_back(std::move(来源));
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询字段引用节点结构", L"反向字段索引读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询字段引用节点结构", L"反向字段索引读取未知异常。");
    }
}

基础联合节点查询结果 基础数据集::查询节点组完整结构(
    const std::vector<稳定编码>& 节点组) const noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::shared_lock 锁(互斥_);
        if (节点组.empty()) return {状态::入口拒绝, {}};
        std::set<std::uint64_t> 身份;
        for (const auto 编码 : 节点组)
            if (!有效(编码) || !身份.insert(编码.值).second)
                return {状态::入口拒绝, {}};
        基础联合节点查询结果 结果{状态::已完成, {}};
        结果.节点组.reserve(身份.size());
        for (const auto 编码 : 身份) {
            const 稳定编码 节点{编码};
            if (节点存在(节点)) {
                结果.节点组.push_back({节点, 读取联合快照已加锁(节点)});
                continue;
            }
            for (const auto& [_, 字段] : 字段关系_) {
                const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
                if (字段.所属节点 == 节点 || 字段.字段节点 == 节点
                    || (目标 && *目标 == 节点)) return {状态::内部不一致, {}};
            }
            for (const auto& [_, 关系] : 外部关系_)
                if (关系.源节点 == 节点 || 关系.目标节点 == 节点)
                    return {状态::内部不一致, {}};
            结果.节点组.push_back({节点, std::nullopt});
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询节点组完整结构",
            L"联合结构读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询节点组完整结构",
            L"联合结构读取未知异常。");
    }
}

基础字段组合变化结果 基础数据集::原子比较变更字段组(
    const 基础字段组合变化请求& 请求) noexcept {
    using 状态 = 基础字段组合变化状态;
    try {
        std::unique_lock 锁(互斥_);
        if (请求.预期节点组.empty()
            || (请求.删除字段关系组.empty() && 请求.替换字段组.empty()
                && 请求.追加字段组.empty())) return {状态::入口拒绝, {}};

        const auto 内容合法 = [](const 基础字段内容& 内容) noexcept {
            if (内容.valueless_by_exception()) return false;
            if (const auto* 引用 = std::get_if<稳定编码>(&内容)) return 有效(*引用);
            return !std::get<基础值>(内容).材料.valueless_by_exception();
        };
        const auto 引用目标 = [](const 基础字段内容& 内容) noexcept {
            return std::get_if<稳定编码>(&内容);
        };
        std::set<std::uint64_t> 预期节点;
        std::map<std::uint64_t, const 基础字段关系*> 预期字段;
        for (const auto& 快照 : 请求.预期节点组) {
            const auto 节点 = 快照.结构.节点.编码;
            if (!有效(节点) || !预期节点.insert(节点.值).second)
                return {状态::入口拒绝, {}};
            for (const auto& 字段 : 快照.结构.字段组) {
                if (!有效(字段.编码) || !有效(字段.字段节点)
                    || 字段.所属节点 != 节点 || !内容合法(字段.内容)
                    || !预期字段.emplace(字段.编码.值, &字段).second)
                    return {状态::入口拒绝, {}};
            }
        }
        std::set<std::uint64_t> 已触碰字段;
        for (const auto 编码 : 请求.删除字段关系组) {
            if (!有效(编码) || !已触碰字段.insert(编码.值).second)
                return {状态::入口拒绝, {}};
            const auto 位置 = 预期字段.find(编码.值);
            if (位置 == 预期字段.end()) return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(位置->second->内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
        }
        for (const auto& 替换 : 请求.替换字段组) {
            if (!有效(替换.字段关系) || !内容合法(替换.新内容)
                || !已触碰字段.insert(替换.字段关系.值).second)
                return {状态::入口拒绝, {}};
            const auto 位置 = 预期字段.find(替换.字段关系.值);
            if (位置 == 预期字段.end()) return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(位置->second->内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(替换.新内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
        }
        for (const auto& 追加 : 请求.追加字段组) {
            if (!有效(追加.所属节点) || !预期节点.contains(追加.所属节点.值)
                || !有效(追加.字段节点) || !内容合法(追加.内容))
                return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(追加.内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
        }
        for (const auto& 快照 : 请求.预期节点组)
            if (!节点存在(快照.结构.节点.编码)) return {状态::节点不存在, {}};
        for (const auto& 快照 : 请求.预期节点组) {
            if (!联合快照相等(
                    读取联合快照已加锁(快照.结构.节点.编码), 快照))
                return {状态::预期不符, {}};
            // 已证明是当前字段，端点失效才属于底座不变量破损。
            for (const auto& 字段 : 快照.结构.字段组) {
                if (!节点存在(字段.字段节点))
                    throw std::logic_error("current field key node missing");
                if (const auto* 目标 = 引用目标(字段.内容);
                    目标 && !节点存在(*目标))
                    throw std::logic_error("current field target node missing");
            }
        }
        for (const auto& 追加 : 请求.追加字段组)
            if (!节点存在(追加.字段节点)) return {状态::引用不存在, {}};
        for (const auto 编码 : 已触碰字段) {
            const auto 位置 = 字段关系_.find(编码);
            if (位置 == 字段关系_.end()
                || 位置->second != *预期字段.at(编码))
                throw std::logic_error("current touched field disagrees with complete snapshot");
            核验字段索引已加锁(位置->second);
        }

        auto 候选字段 = 字段关系_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        基础字段组合变化结果 结果{状态::已完成, {}};
        结果.追加字段关系组.reserve(请求.追加字段组.size());
        const auto 移除候选字段 = [&](稳定编码 编码) {
            const auto 位置 = 候选字段.find(编码.值);
            if (位置 == 候选字段.end())
                throw std::logic_error("candidate field to remove missing");
            const auto& 字段 = 位置->second;
            if (候选所属索引.erase({字段.所属节点.值,
                    字段.字段节点.值, 编码.值}) != 1)
                throw std::logic_error("candidate forward field index missing");
            if (const auto* 目标 = 引用目标(字段.内容);
                目标 && 候选目标索引.erase({字段.字段节点.值,
                    目标->值, 编码.值}) != 1)
                throw std::logic_error("candidate reverse field index missing");
            候选字段.erase(位置);
        };
        for (const auto 编码 : 请求.删除字段关系组) 移除候选字段(编码);
        for (const auto& 替换 : 请求.替换字段组) {
            const auto 位置 = 候选字段.find(替换.字段关系.值);
            if (位置 == 候选字段.end())
                throw std::logic_error("candidate field to replace missing");
            const auto& 字段 = 位置->second;
            if (const auto* 旧目标 = 引用目标(字段.内容);
                旧目标 && 候选目标索引.erase({字段.字段节点.值,
                    旧目标->值, 替换.字段关系.值}) != 1)
                throw std::logic_error("candidate old reverse index missing");
            位置->second.内容 = 替换.新内容;
            if (const auto* 新目标 = 引用目标(位置->second.内容);
                新目标 && !候选目标索引.emplace(字段.字段节点.值,
                    新目标->值, 替换.字段关系.值).second)
                throw std::logic_error("candidate new reverse index collision");
        }
        for (const auto& 追加 : 请求.追加字段组) {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::编码耗尽, {}};
            if (节点_.contains(编码.值) || 外部关系_.contains(编码.值)
                || 候选字段.contains(编码.值))
                throw std::logic_error("allocated field identity collision");
            const auto [位置, 已插入] = 候选字段.emplace(编码.值,
                基础字段关系{编码, 追加.所属节点, 追加.字段节点, 追加.内容});
            if (!已插入) throw std::logic_error("candidate field identity collision");
            if (!候选所属索引.emplace(追加.所属节点.值,
                    追加.字段节点.值, 编码.值).second)
                throw std::logic_error("candidate forward index collision");
            if (const auto* 目标 = 引用目标(位置->second.内容);
                目标 && !候选目标索引.emplace(追加.字段节点.值,
                    目标->值, 编码.值).second)
                throw std::logic_error("candidate reverse index collision");
            结果.追加字段关系组.push_back(编码);
        }

        static_assert(std::is_nothrow_swappable_v<decltype(字段关系_)>);
        static_assert(std::is_nothrow_swappable_v<decltype(所属字段索引_)>);
        static_assert(std::is_nothrow_swappable_v<decltype(目标字段索引_)>);
        static_assert(std::is_nothrow_move_constructible_v<基础字段组合变化结果>);
        字段关系_.swap(候选字段);
        所属字段索引_.swap(候选所属索引);
        目标字段索引_.swap(候选目标索引);
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子比较变更字段组",
            L"完整预期字段组合变化发生工程异常，未形成业务否定。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子比较变更字段组",
            L"完整预期字段组合变化发生未知工程异常，未形成业务否定。");
    }
}

基础互引节点建立结果 基础数据集::原子建立互引节点(
    const 基础互引节点建立请求& 请求) noexcept {
    using 状态 = 基础互引节点操作状态;
    try {
        std::unique_lock 锁(互斥_);
        if (!请求.新节点.上级节点 || !有效(*请求.新节点.上级节点)
            || !有效(请求.反向引用所属节点)
            || !有效(请求.反向引用字段节点)) return {状态::入口拒绝, {}, {}, {}};
        std::set<std::uint64_t> 必需{
            请求.新节点.上级节点->值, 请求.反向引用所属节点.值,
            请求.反向引用字段节点.值};
        for (const auto& 字段 : 请求.新节点.字段组) {
            if (!有效(字段.字段节点) || 字段.内容.valueless_by_exception())
                return {状态::入口拒绝, {}, {}, {}};
            必需.insert(字段.字段节点.值);
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) {
                if (!有效(*引用)) return {状态::入口拒绝, {}, {}, {}};
                必需.insert(引用->值);
            } else if (std::get<基础值>(字段.内容).材料.valueless_by_exception())
                return {状态::入口拒绝, {}, {}, {}};
        }
        const auto 预期状态 = 复核联合预期已加锁(请求.预期节点组, 必需);
        if (预期状态 != 状态::已完成) return {预期状态, {}, {}, {}};
        auto 候选节点 = 节点_;
        auto 候选字段 = 字段关系_;
        auto 候选关系 = 外部关系_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        const auto 编码已占用 = [&](稳定编码 编码) {
            return 候选节点.contains(编码.值) || 候选字段.contains(编码.值)
                || 候选关系.contains(编码.值);
        };
        const auto 节点 = 分配编码();
        if (!有效(节点)) return {状态::资源失败, {}, {}, {}};
        if (编码已占用(节点)) return {状态::内部不一致, {}, {}, {}};
        候选节点.emplace(节点.值, 基础节点{节点});
        const auto 加字段 = [&](稳定编码 所属, 稳定编码 键, 基础字段内容 内容)
            -> std::optional<稳定编码> {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return std::nullopt;
            if (编码已占用(编码)) throw std::logic_error("mutual field identity collision");
            候选字段.emplace(编码.值, 基础字段关系{编码, 所属, 键, std::move(内容)});
            const auto& 字段 = 候选字段.at(编码.值);
            if (!候选所属索引.emplace(所属.值, 键.值, 编码.值).second)
                throw std::logic_error("mutual forward index collision");
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                引用 && !候选目标索引.emplace(键.值, 引用->值, 编码.值).second)
                throw std::logic_error("mutual reverse index collision");
            return 编码;
        };
        for (const auto& 字段 : 请求.新节点.字段组)
            if (!加字段(节点, 字段.字段节点, 字段.内容))
                return {状态::资源失败, {}, {}, {}};
        const auto 反向 = 加字段(请求.反向引用所属节点,
            请求.反向引用字段节点, 节点);
        if (!反向) return {状态::资源失败, {}, {}, {}};
        const auto 关系 = 分配编码();
        if (!有效(关系)) return {状态::资源失败, {}, {}, {}};
        if (编码已占用(关系)) return {状态::内部不一致, {}, {}, {}};
        候选关系.emplace(关系.值, 基础外部关系{关系,
            *请求.新节点.上级节点, 节点, 基础外部关系类型::父子, 0});
        基础互引节点建立结果 结果{状态::已完成, 节点, *反向, 关系};
        节点_.swap(候选节点);
        字段关系_.swap(候选字段);
        外部关系_.swap(候选关系);
        所属字段索引_.swap(候选所属索引);
        目标字段索引_.swap(候选目标索引);
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}, {}, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}, {}, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立互引节点",
            L"联合建立候选与索引异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立互引节点",
            L"联合建立未知异常。");
    }
}

基础互引节点操作状态 基础数据集::原子删除互引节点(
    const 基础互引节点删除请求& 请求) noexcept {
    using 状态 = 基础互引节点操作状态;
    try {
        std::unique_lock 锁(互斥_);
        if (!有效(请求.节点) || !有效(请求.上级节点)
            || !有效(请求.反向字段关系)) return 状态::入口拒绝;
        if (请求.节点 == 请求.上级节点) return 状态::入口拒绝;
        if (!节点存在(请求.节点)) return 状态::节点不存在;
        const auto 反向位置 = 字段关系_.find(请求.反向字段关系.值);
        if (反向位置 == 字段关系_.end()) return 状态::引用不存在;
        const auto& 反向 = 反向位置->second;
        const auto* 目标 = std::get_if<稳定编码>(&反向.内容);
        if (!目标 || *目标 != 请求.节点 || 反向.所属节点 == 请求.节点)
            return 状态::入口拒绝;
        核验字段索引已加锁(反向);
        std::set<std::uint64_t> 必需{
            请求.节点.值, 请求.上级节点.值,
            反向.所属节点.值, 反向.字段节点.值};
        std::vector<std::uint64_t> 待删字段;
        for (const auto& [编码, 字段] : 字段关系_) {
            if (字段.字段节点 == 请求.节点) return 状态::引用仍存在;
            if (字段.所属节点 == 请求.节点) {
                核验字段索引已加锁(字段);
                if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                    引用 && *引用 == 请求.节点)
                    return 状态::引用仍存在;
                待删字段.push_back(编码);
                必需.insert(字段.字段节点.值);
                if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容))
                    必需.insert(引用->值);
                continue;
            }
            if (编码 == 请求.反向字段关系.值) continue;
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                引用 && *引用 == 请求.节点) return 状态::引用仍存在;
        }
        std::optional<std::uint64_t> 父边;
        for (const auto& [编码, 关系] : 外部关系_) {
            if (关系.目标节点 == 请求.节点
                && 关系.源节点 == 请求.上级节点
                && 关系.类型 == 基础外部关系类型::父子
                && 关系.角色或顺序 == 0) {
                if (父边) return 状态::内部不一致;
                父边 = 编码;
            } else if (关系.源节点 == 请求.节点 || 关系.目标节点 == 请求.节点) {
                return 状态::引用仍存在;
            }
        }
        if (!父边) return 状态::引用不存在;
        const auto 预期状态 = 复核联合预期已加锁(请求.预期节点组, 必需);
        if (预期状态 != 状态::已完成) return 预期状态;
        auto 候选节点 = 节点_;
        auto 候选字段 = 字段关系_;
        auto 候选关系 = 外部关系_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        const auto 删字段 = [&](const 基础字段关系& 字段) {
            if (候选所属索引.erase({字段.所属节点.值,
                字段.字段节点.值, 字段.编码.值}) != 1)
                throw std::logic_error("union delete forward index missing");
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                引用 && 候选目标索引.erase({字段.字段节点.值,
                    引用->值, 字段.编码.值}) != 1)
                throw std::logic_error("union delete reverse index missing");
            if (候选字段.erase(字段.编码.值) != 1)
                throw std::logic_error("union delete field missing");
        };
        for (const auto 编码 : 待删字段) 删字段(字段关系_.at(编码));
        删字段(反向);
        if (候选关系.erase(*父边) != 1
            || 候选节点.erase(请求.节点.值) != 1)
            throw std::logic_error("union delete node or edge missing");
        节点_.swap(候选节点);
        字段关系_.swap(候选字段);
        外部关系_.swap(候选关系);
        所属字段索引_.swap(候选所属索引);
        目标字段索引_.swap(候选目标索引);
        return 状态::已完成;
    } catch (const std::bad_alloc&) { return 状态::资源失败;
    } catch (const std::length_error&) { return 状态::资源失败;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除互引节点",
            L"联合删除候选与索引异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除互引节点",
            L"联合删除未知异常。");
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
        插入字段已加锁(基础字段关系{编码, 节点, 字段节点, std::move(值)});
        return 编码;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段值", L"字段及索引插入异常，已撤销本次插项。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段值", L"字段及索引插入未知异常。");
    }
}

稳定编码 基础数据集::添加字段节点(
    稳定编码 节点, 稳定编码 字段节点, 稳定编码 目标节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(节点) || !节点存在(字段节点) || !节点存在(目标节点)) return {};
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        插入字段已加锁(基础字段关系{编码, 节点, 字段节点, 目标节点});
        return 编码;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段节点", L"引用字段及索引插入异常，已撤销本次插项。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段节点", L"引用字段及索引插入未知异常。");
    }
}

bool 基础数据集::修改字段值(稳定编码 字段关系, 基础值 值) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
        核验字段索引已加锁(位置->second);
        static_assert(std::is_nothrow_swappable_v<基础字段内容>);
        基础字段内容 候选{std::move(值)};
        位置->second.内容.swap(候选);
        if (const auto* 旧引用 = std::get_if<稳定编码>(&候选))
            目标字段索引_.erase({位置->second.字段节点.值, 旧引用->值, 字段关系.值});
        return true;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段值", L"字段内容及索引更新异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段值", L"字段内容及索引更新未知异常。");
    }
}

bool 基础数据集::修改字段节点(
    稳定编码 字段关系, 稳定编码 目标节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(目标节点)) return false;
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
        核验字段索引已加锁(位置->second);
        const auto& 字段 = 位置->second;
        const auto* 旧引用 = std::get_if<稳定编码>(&字段.内容);
        if (旧引用 && *旧引用 == 目标节点) return true;
        const std::optional<字段索引键> 旧键 = 旧引用
            ? std::optional<字段索引键>{{字段.字段节点.值, 旧引用->值, 字段关系.值}} : std::nullopt;
        基础字段内容 候选{目标节点};
        if (!目标字段索引_.emplace(字段.字段节点.值, 目标节点.值, 字段关系.值).second)
            throw std::logic_error("modified reverse field index collision");
        static_assert(std::is_nothrow_swappable_v<基础字段内容>);
        位置->second.内容.swap(候选);
        if (旧键) 目标字段索引_.erase(*旧键);
        return true;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段节点", L"引用目标及索引更新异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段节点", L"引用目标及索引更新未知异常。");
    }
}

bool 基础数据集::删除字段(稳定编码 字段关系) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!有效(字段关系)) return false;
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
        核验字段索引已加锁(位置->second);
        删除字段已加锁(位置->second);
        return true;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除字段", L"字段及索引删除异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除字段", L"字段及索引删除未知异常。");
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
    for (auto 位置 = 所属字段索引_.lower_bound({节点.值, 字段节点.值, 0});
        位置 != 所属字段索引_.end() && std::get<0>(*位置) == 节点.值
            && std::get<1>(*位置) == 字段节点.值; ++位置)
        结果.push_back(字段关系_.at(std::get<2>(*位置)));
    return 结果;
}

std::vector<基础字段关系> 基础数据集::查询目标字段(
    稳定编码 目标节点, 稳定编码 字段节点) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础字段关系> 结果;
    if (!节点存在(目标节点) || !节点存在(字段节点)) return 结果;
    for (auto 位置 = 目标字段索引_.lower_bound({字段节点.值, 目标节点.值, 0});
        位置 != 目标字段索引_.end() && std::get<0>(*位置) == 字段节点.值
            && std::get<1>(*位置) == 目标节点.值; ++位置)
        结果.push_back(字段关系_.at(std::get<2>(*位置)));
    return 结果;
}

} // namespace 海中鱼巣
