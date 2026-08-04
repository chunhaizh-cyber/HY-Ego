module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#define L1_FACT_BASE_NO_INCLUDES

export module 海中鱼巣.核心.仓库.L1事实基座;

export {
#include "L1事实基座.数据.h"
}

export namespace 海中鱼巣 {

class L1事实基座仓库 final {
public:
    L1事实基座仓库() = default;
    L1事实基座仓库(const L1事实基座仓库&) = delete;
    L1事实基座仓库& operator=(const L1事实基座仓库&) = delete;

    L1写入结果 提交(const L1写集请求& 请求) {
        try {
            const auto 规范化 = 规范化写集(请求);
            if (!规范化 || !有效(请求.幂等键)
                || (规范化->节点.empty() && 规范化->关系.empty()
                    && 规范化->值.empty() && 规范化->属性槽变更.empty()
                    && 规范化->退出事实.empty())) return {};

            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离) return {L1写入状态::内部不一致, 状态_.事实代次, {}};
            const auto 既有 = 状态_.幂等账.find(请求.幂等键.值);
            if (既有 != 状态_.幂等账.end()) {
                if (既有->second.规范化写集 == *规范化) {
                    状态 候选 = 状态_;
                    记录审计(候选, 请求.幂等键, L1审计事件::精确重复,
                        L1写入状态::精确重复, 既有->second.首次结果.事实代次,
                        既有->second.首次结果.新编码映射);
                    auto 结果 = 既有->second.首次结果;
                    结果.状态 = L1写入状态::精确重复;
                    std::swap(状态_, 候选);
                    return 结果;
                }
                状态 候选 = 状态_;
                记录审计(候选, 请求.幂等键, L1审计事件::幂等冲突,
                    L1写入状态::幂等冲突, 状态_.事实代次, {});
                const auto 代次 = 状态_.事实代次;
                std::swap(状态_, 候选);
                return {L1写入状态::幂等冲突, 代次, {}};
            }
            if (请求.期望事实代次 != 状态_.事实代次)
                return {L1写入状态::事实代次漂移, 状态_.事实代次, {}};

            std::unordered_set<std::uint32_t> 本地键;
            if (!请求结构有效(*规范化, 本地键)) return {};
            状态 候选 = 状态_;
            std::unordered_set<std::uint64_t> 本地编码;
            auto 分配 = [&]() {
                while (候选.下个编码 == 0 || 候选.永久占用.contains(候选.下个编码)
                    || 本地编码.contains(候选.下个编码)) ++候选.下个编码;
                const 稳定编码 编码{候选.下个编码++};
                本地编码.insert(编码.值);
                return 编码;
            };
            std::vector<std::pair<写集本地键, 稳定编码>> 映射;
            for (const auto& 项 : 规范化->节点) 映射.emplace_back(项.本地键, 分配());
            for (const auto& 项 : 规范化->关系) 映射.emplace_back(项.本地键, 分配());
            for (const auto& 项 : 规范化->值) 映射.emplace_back(项.本地键, 分配());
            auto 解析 = [&](const 事实引用& 引用) -> std::optional<稳定编码> {
                return std::visit([&](const auto& 值) -> std::optional<稳定编码> {
                    using T = std::decay_t<decltype(值)>;
                    if constexpr (std::is_same_v<T, 稳定编码>) {
                        return 有效(值) && (状态_.当前节点.contains(值.值)
                            || 状态_.当前关系.contains(值.值) || 状态_.当前值.contains(值.值))
                            ? std::optional<稳定编码>(值) : std::nullopt;
                    } else {
                        for (const auto& [键, 编码] : 映射) if (键 == 值) return 编码;
                        return std::nullopt;
                    }
                }, 引用);
            };
            const auto 新代次 = 状态_.事实代次 + 1;
            for (const auto& 项 : 规范化->节点) {
                if ((项.种类 == 节点种类::属性类型) != 项.属性类型表示.has_value()) return {};
                const auto 编码 = 映射[static_cast<std::size_t>(&项 - 规范化->节点.data())].second;
                候选.当前节点.emplace(编码.值,
                    节点事实{编码, 项.种类, 项.属性类型表示, 新代次, std::nullopt, {}});
                候选.永久占用.insert(编码.值);
            }
            const std::size_t 关系偏移 = 规范化->节点.size();
            for (std::size_t i = 0; i < 规范化->关系.size(); ++i) {
                const auto& 项 = 规范化->关系[i];
                const auto 源 = 解析(项.源节点); const auto 目标 = 解析(项.目标节点);
                const auto 类型 = 解析(项.关系类型节点);
                if (!源 || !目标 || !类型) return {};
                const auto 编码 = 映射[关系偏移 + i].second;
                候选.当前关系.emplace(编码.值,
                    关系事实{编码, *源, *目标, *类型, 项.角色或顺序, 新代次, std::nullopt});
                候选.永久占用.insert(编码.值);
            }
            const std::size_t 值偏移 = 关系偏移 + 规范化->关系.size();
            for (std::size_t i = 0; i < 规范化->值.size(); ++i) {
                const auto& 项 = 规范化->值[i];
                const auto 所属 = 解析(项.所属节点); const auto 类型 = 解析(项.属性类型节点);
                const auto 来源 = 解析(项.来源节点);
                if (!所属 || !类型 || !来源) return {};
                if (const auto* 引用 = std::get_if<独立材料引用>(&项.材料); 引用
                    && !状态_.当前节点.contains(引用->编码.值)) return {};
                const auto 编码 = 映射[值偏移 + i].second;
                候选.当前值.emplace(编码.值,
                    值事实{编码, *所属, *类型, 项.材料, *来源, 新代次, std::nullopt});
                候选.永久占用.insert(编码.值);
            }
            auto 退出一个 = [&](稳定编码 编码) {
                if (auto it = 候选.当前节点.find(编码.值); it != 候选.当前节点.end()) {
                    auto 事实 = it->second; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false}; 候选.当前节点.erase(it); return true;
                }
                if (auto it = 候选.当前关系.find(编码.值); it != 候选.当前关系.end()) {
                    auto 事实 = it->second; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false}; 候选.当前关系.erase(it); return true;
                }
                if (auto it = 候选.当前值.find(编码.值); it != 候选.当前值.end()) {
                    auto 事实 = it->second; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false}; 候选.当前值.erase(it);
                    for (auto& [节点编码, 节点] : 候选.当前节点) {
                        (void)节点编码;
                        节点.当前属性.erase(
                            std::remove_if(节点.当前属性.begin(), 节点.当前属性.end(),
                                [&](const 属性槽& 槽) { return 槽.当前值 == 编码; }),
                            节点.当前属性.end());
                    }
                    return true;
                }
                return false;
            };
            for (const auto 编码 : 规范化->退出事实)
                if (!状态_.当前节点.contains(编码.值) && !状态_.当前关系.contains(编码.值)
                    && !状态_.当前值.contains(编码.值)) return {L1写入状态::未找到, 状态_.事实代次, {}};
            for (const auto 编码 : 规范化->退出事实) if (!退出一个(编码)) return {};

            for (const auto& 项 : 规范化->属性槽变更) {
                const auto 所属 = 解析(项.所属节点); const auto 类型 = 解析(项.属性类型节点);
                const auto 新值编码 = [&]() -> std::optional<稳定编码> {
                    for (const auto& [键, 编码] : 映射) if (键 == 项.新当前值) return 编码;
                    return std::nullopt;
                }();
                if (!所属 || !类型 || !新值编码) return {};
                auto 节点 = 候选.当前节点.find(所属->值);
                if (节点 == 候选.当前节点.end()) return {L1写入状态::未找到, 状态_.事实代次, {}};
                auto& 槽组 = 节点->second.当前属性;
                auto 槽 = std::find_if(槽组.begin(), 槽组.end(), [&](const 属性槽& 值) { return 值.属性类型节点 == *类型; });
                if (槽 != 槽组.end() && 槽->当前值 == *新值编码) continue;
                if (槽 != 槽组.end()) {
                    const auto 旧值 = 槽->当前值;
                    if (auto 值 = 候选.当前值.find(旧值.值); 值 != 候选.当前值.end()) {
                        auto 历 = 值->second; 历.退出事实代次 = 新代次;
                        候选.历史[旧值.值] = {旧值, 历, false}; 候选.当前值.erase(值);
                    }
                    槽->当前值 = *新值编码;
                } else 槽组.push_back({*类型, *新值编码});
                std::sort(槽组.begin(), 槽组.end(), [](const 属性槽& 左, const 属性槽& 右) { return 左.属性类型节点 < 右.属性类型节点; });
            }
            候选.事实代次 = 新代次;
            if (!状态完整(候选)) return {L1写入状态::入口拒绝, 状态_.事实代次, {}};
            L1写入结果 结果{L1写入状态::成功, 新代次, 映射};
            候选.幂等账.emplace(请求.幂等键.值, L1幂等账记录{请求.幂等键, *规范化, 结果});
            记录审计(候选, 请求.幂等键, L1审计事件::提交成功, L1写入状态::成功, 新代次, 映射);
            std::swap(状态_, 候选);
            return 结果;
        } catch (const std::bad_alloc&) { return {L1写入状态::资源失败, 状态_.事实代次, {}}; }
        catch (...) { return {L1写入状态::内部不一致, 状态_.事实代次, {}}; }
    }

    L1读取结果 读取当前节点(稳定编码 编码) const { return 读取当前(编码, 状态_.当前节点); }
    L1读取结果 读取当前关系(稳定编码 编码) const { return 读取当前(编码, 状态_.当前关系); }
    L1读取结果 读取当前值(稳定编码 编码) const { return 读取当前(编码, 状态_.当前值); }
    L1属性读取结果 读取当前属性(稳定编码 节点, 稳定编码 类型) const {
        if (!有效(节点) || !有效(类型)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, 状态_.事实代次, std::nullopt};
        const auto it = 状态_.当前节点.find(节点.值);
        if (it == 状态_.当前节点.end()) return {状态_.历史.contains(节点.值) ? L1读取状态::已退出 : L1读取状态::未找到, 0, std::nullopt};
        const auto 槽 = std::find_if(it->second.当前属性.begin(), it->second.当前属性.end(), [&](const 属性槽& 值) { return 值.属性类型节点 == 类型; });
        if (槽 == it->second.当前属性.end()) return {L1读取状态::属性未设置, 状态_.事实代次, std::nullopt};
        return {L1读取状态::成功, 状态_.事实代次, L1属性读取副本{节点, 类型, 槽->当前值}};
    }
    L1历史读取结果 读取历史事实(稳定编码 编码) const {
        if (!有效(编码)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, std::nullopt};
        if (auto it = 状态_.当前节点.find(编码.值); it != 状态_.当前节点.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 状态_.当前关系.find(编码.值); it != 状态_.当前关系.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 状态_.当前值.find(编码.值); it != 状态_.当前值.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 状态_.历史.find(编码.值); it != 状态_.历史.end()) return {L1读取状态::成功, it->second};
        return {L1读取状态::未找到, std::nullopt};
    }
    L1审计读取结果 读取审计(写集幂等键 幂等键) const {
        if (!有效(幂等键)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离) return {L1读取状态::内部不一致, 幂等键, {}};
            const auto it = 状态_.审计.find(幂等键.值);
        if (it == 状态_.审计.end()) return {L1读取状态::未找到, 幂等键, {}};
        return {L1读取状态::成功, 幂等键, it->second};
    }
    L1完整快照结果 读取完整快照() const {
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, std::nullopt};
        L1完整快照 快照; 快照.事实代次 = 状态_.事实代次;
        for (const auto& [_, 事实] : 状态_.当前节点) 快照.当前节点.push_back(事实);
        for (const auto& [_, 事实] : 状态_.当前关系) 快照.当前关系.push_back(事实);
        for (const auto& [_, 事实] : 状态_.当前值) 快照.当前值.push_back(事实);
        for (const auto 编码 : 状态_.永久占用) 快照.永久占用编码.push_back({编码});
        排序(快照); return {L1读取状态::成功, std::move(快照)};
    }
    L1恢复结果 建立恢复候选(const L1恢复材料& 材料, std::uint64_t 期望) {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离) return {L1恢复状态::内部不一致, 状态_.事实代次};
            if (候选_.has_value() || 材料.当前快照.事实代次 != 期望 || 期望 != 状态_.事实代次) return {L1恢复状态::事实代次漂移, 状态_.事实代次};
            状态 值;
            if (!恢复材料转状态(材料, 值) || !状态完整(值)) return {L1恢复状态::材料不完整, 状态_.事实代次};
            候选_ = 候选状态{std::move(值), 状态_.事实代次};
            return {L1恢复状态::候选已建立, 状态_.事实代次};
        } catch (const std::bad_alloc&) { return {L1恢复状态::资源失败, 0}; }
        catch (...) { return {L1恢复状态::内部不一致, 0}; }
    }
    L1恢复结果 确认恢复候选() {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离) return {L1恢复状态::内部不一致, 状态_.事实代次};
            if (!候选_) return {L1恢复状态::无候选, 状态_.事实代次};
            if (候选_->基线 != 状态_.事实代次) { 候选_.reset(); return {L1恢复状态::事实代次漂移, 状态_.事实代次}; }
            状态 值 = 候选_->值;
            if (!状态完整(值)) {
                候选_.reset(); 状态_.隔离 = true;
                return {L1恢复状态::内部不一致, 状态_.事实代次};
            }
            值.事实代次 = 状态_.事实代次 + 1;
            if (!值.幂等账.empty()) {
                const auto 键 = 值.幂等账.begin()->second.幂等键;
                记录审计(值, 键, L1审计事件::恢复发布, L1写入状态::成功, 值.事实代次, {});
            }
            std::swap(状态_, 值); 候选_.reset();
            return {L1恢复状态::恢复已发布, 状态_.事实代次};
        } catch (const std::bad_alloc&) { return {L1恢复状态::资源失败, 0}; }
        catch (...) { return {L1恢复状态::内部不一致, 0}; }
    }
    L1恢复结果 撤销恢复候选() noexcept {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!候选_) return {L1恢复状态::无候选, 状态_.事实代次};
            候选_.reset(); return {L1恢复状态::候选已撤销, 状态_.事实代次};
        } catch (...) { return {L1恢复状态::内部不一致, 0}; }
    }

private:
    struct 状态 {
        std::uint64_t 事实代次 = 0;
        std::uint64_t 下个编码 = 1;
        std::unordered_map<std::uint64_t, 节点事实> 当前节点;
        std::unordered_map<std::uint64_t, 关系事实> 当前关系;
        std::unordered_map<std::uint64_t, 值事实> 当前值;
        std::unordered_map<std::uint64_t, L1历史事实副本> 历史;
        std::unordered_set<std::uint64_t> 永久占用;
        std::unordered_map<std::uint64_t, L1幂等账记录> 幂等账;
        std::unordered_map<std::uint64_t, std::vector<L1审计记录>> 审计;
        bool 隔离 = false;
    };
    struct 候选状态 { 状态 值; std::uint64_t 基线 = 0; };

    static std::uint64_t 引用排序键(const 事实引用& 引用) {
        return std::visit([](const auto& 值) -> std::uint64_t {
            using T = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<T, 稳定编码>) return 值.值;
            else return (1ULL << 63) | 值.值;
        }, 引用);
    }
    static std::optional<L1写集请求> 规范化写集(const L1写集请求& 请求) {
        L1写集请求 结果 = 请求;
        std::sort(结果.节点.begin(), 结果.节点.end(), [](const auto& 左, const auto& 右) { return 左.本地键 < 右.本地键; });
        std::sort(结果.关系.begin(), 结果.关系.end(), [](const auto& 左, const auto& 右) { return 左.本地键 < 右.本地键; });
        std::sort(结果.值.begin(), 结果.值.end(), [](const auto& 左, const auto& 右) { return 左.本地键 < 右.本地键; });
        std::sort(结果.属性槽变更.begin(), 结果.属性槽变更.end(), [](const auto& 左, const auto& 右) {
            const auto 左键 = 引用排序键(左.所属节点); const auto 右键 = 引用排序键(右.所属节点);
            if (左键 != 右键) return 左键 < 右键;
            const auto 左类型 = 引用排序键(左.属性类型节点); const auto 右类型 = 引用排序键(右.属性类型节点);
            if (左类型 != 右类型) return 左类型 < 右类型;
            return 左.新当前值 < 右.新当前值;
        });
        std::sort(结果.退出事实.begin(), 结果.退出事实.end());
        return 结果;
    }
    static bool 请求结构有效(const L1写集请求& 请求, std::unordered_set<std::uint32_t>& 本地键) {
        auto 登记 = [&](写集本地键 键) { return 有效(键) && 本地键.insert(键.值).second; };
        for (const auto& 项 : 请求.节点) if (!登记(项.本地键)) return false;
        for (const auto& 项 : 请求.关系) if (!登记(项.本地键)) return false;
        for (const auto& 项 : 请求.值) if (!登记(项.本地键) || !原始材料完整(项.材料)) return false;
        std::unordered_set<std::uint64_t> 退出;
        for (const auto 编码 : 请求.退出事实) if (!有效(编码) || !退出.insert(编码.值).second) return false;
        for (const auto& 项 : 请求.属性槽变更) if (!有效(项.新当前值)) return false;
        return true;
    }
    static bool 表示匹配(值表示种类 表示, const 原始值材料& 材料) {
        return (表示 == 值表示种类::I64 && std::holds_alternative<std::int64_t>(材料))
            || (表示 == 值表示种类::I64组 && std::holds_alternative<std::vector<std::int64_t>>(材料))
            || (表示 == 值表示种类::U64组 && std::holds_alternative<std::vector<std::uint64_t>>(材料))
            || (表示 == 值表示种类::独立材料引用 && std::holds_alternative<独立材料引用>(材料));
    }
    static bool 状态完整(const 状态& 值) {
        std::unordered_set<std::uint64_t> 全部;
        auto 插入 = [&](std::uint64_t 编码) { return 编码 != 0 && 全部.insert(编码).second; };
        for (const auto& [编码, 节点] : 值.当前节点) {
            if (节点.编码.值 != 编码 || 节点.创建事实代次 == 0 || 节点.创建事实代次 > 值.事实代次
                || (节点.种类 == 节点种类::属性类型) != 节点.属性类型表示.has_value()
                || !属性排序唯一(节点.当前属性) || !插入(编码)) return false;
        }
        for (const auto& [编码, 关系] : 值.当前关系) {
            if (!插入(编码) || !值.当前节点.contains(关系.源节点.值)
                || !值.当前节点.contains(关系.目标节点.值)
                || !值.当前节点.contains(关系.关系类型节点.值)
                || 关系.退出事实代次) return false;
        }
        for (const auto& [编码, 事实] : 值.当前值) {
            if (!插入(编码) || !值.当前节点.contains(事实.所属节点.值) || !值.当前节点.contains(事实.来源节点.值)) return false;
            const auto 类型 = 值.当前节点.find(事实.属性类型节点.值);
            if (类型 == 值.当前节点.end() || 类型->second.种类 != 节点种类::属性类型
                || !类型->second.属性类型表示 || !表示匹配(*类型->second.属性类型表示, 事实.材料)) return false;
            if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料); 引用 && !值.当前节点.contains(引用->编码.值)) return false;
        }
        for (const auto& [编码, 节点] : 值.当前节点) for (const auto& 槽 : 节点.当前属性) {
            const auto 类型 = 值.当前节点.find(槽.属性类型节点.值);
            const auto 当前值 = 值.当前值.find(槽.当前值.值);
            if (类型 == 值.当前节点.end() || 类型->second.种类 != 节点种类::属性类型
                || 当前值 == 值.当前值.end() || 当前值->second.所属节点.值 != 编码
                || 当前值->second.属性类型节点 != 槽.属性类型节点) return false;
        }
        for (const auto& [编码, 历史] : 值.历史) {
            if (编码 == 0 || 全部.contains(编码)) return false;
            bool 已退出 = std::visit([](const auto& 事实) { return 事实.退出事实代次.has_value(); }, 历史.事实);
            if (!已退出) return false;
        }
        for (const auto 编码 : 全部) if (!值.永久占用.contains(编码)) return false;
        for (const auto 编码 : 值.永久占用) if (编码 == 0) return false;
        for (const auto& [键, 账] : 值.幂等账)
            if (键 == 0 || !有效(账.幂等键) || 账.幂等键.值 != 键 || 账.首次结果.状态 != L1写入状态::成功) return false;
        return !值.隔离;
    }
    static bool 恢复材料转状态(const L1恢复材料& 材料, 状态& 输出) {
        输出.事实代次 = 材料.当前快照.事实代次;
        for (const auto& 事实 : 材料.当前快照.当前节点) if (!输出.当前节点.emplace(事实.编码.值, 事实).second) return false;
        for (const auto& 事实 : 材料.当前快照.当前关系) if (!输出.当前关系.emplace(事实.编码.值, 事实).second) return false;
        for (const auto& 事实 : 材料.当前快照.当前值) if (!输出.当前值.emplace(事实.编码.值, 事实).second) return false;
        for (const auto 编码 : 材料.当前快照.永久占用编码) if (!输出.永久占用.insert(编码.值).second) return false;
        for (const auto& 事实 : 材料.历史节点) if (!输出.历史.emplace(事实.编码.值, L1历史事实副本{事实.编码, 事实, false}).second) return false;
        for (const auto& 事实 : 材料.历史关系) if (!输出.历史.emplace(事实.编码.值, L1历史事实副本{事实.编码, 事实, false}).second) return false;
        for (const auto& 事实 : 材料.历史值) if (!输出.历史.emplace(事实.编码.值, L1历史事实副本{事实.编码, 事实, false}).second) return false;
        for (const auto& 账 : 材料.幂等账) if (!有效(账.幂等键) || !输出.幂等账.emplace(账.幂等键.值, 账).second) return false;
        for (const auto& 记录 : 材料.审计记录) { if (!有效(记录.幂等键)) return false; 输出.审计[记录.幂等键.值].push_back(记录); }
        std::uint64_t 最大 = 0; for (const auto 编码 : 输出.永久占用) 最大 = std::max(最大, 编码); 输出.下个编码 = 最大 + 1;
        return true;
    }
    template<class T> L1读取结果 读取当前(稳定编码 编码, const std::unordered_map<std::uint64_t, T>& 表) const {
        if (!有效(编码)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, 状态_.事实代次, std::nullopt};
        const auto it = 表.find(编码.值);
        if (it != 表.end()) return {L1读取状态::成功, 状态_.事实代次, L1事实副本{it->second}};
        return {状态_.历史.contains(编码.值) ? L1读取状态::已退出 : L1读取状态::未找到, 0, std::nullopt};
    }
    static void 记录审计(状态& 值, 写集幂等键 幂等键, L1审计事件 事件, L1写入状态 结果状态, std::uint64_t 代次, const std::vector<std::pair<写集本地键, 稳定编码>>& 映射) {
        auto& 组 = 值.审计[幂等键.值];
        组.push_back({幂等键, 组.size() + 1, 事件, 结果状态, 代次, 映射});
    }
    static void 排序(L1完整快照& 快照) {
        auto 编码排序 = [](const auto& 左, const auto& 右) { return 左.编码 < 右.编码; };
        std::sort(快照.当前节点.begin(), 快照.当前节点.end(), 编码排序);
        std::sort(快照.当前关系.begin(), 快照.当前关系.end(), 编码排序);
        std::sort(快照.当前值.begin(), 快照.当前值.end(), 编码排序);
        std::sort(快照.永久占用编码.begin(), 快照.永久占用编码.end());
    }

    mutable std::shared_mutex 锁_;
    状态 状态_;
    std::optional<候选状态> 候选_;
};

} // namespace 海中鱼巣
