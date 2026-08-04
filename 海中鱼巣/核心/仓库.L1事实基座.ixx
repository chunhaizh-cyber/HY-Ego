module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <new>
#include <mutex>
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
            if (!有效(请求.幂等键) || (请求.节点.empty() && 请求.关系.empty()
                && 请求.值.empty() && 请求.属性槽变更.empty() && 请求.退出事实.empty())) {
                return {};
            }
            std::unique_lock<std::shared_mutex> 锁(锁_);
            const auto 既有 = 幂等账_.find(请求.幂等键.值);
            if (既有 != 幂等账_.end()) {
                if (既有->second.规范化写集 == 请求) {
                    记录审计(请求.幂等键, L1审计事件::精确重复,
                        L1写入状态::精确重复, 既有->second.首次结果.事实代次,
                        既有->second.首次结果.新编码映射);
                    auto 结果 = 既有->second.首次结果;
                    结果.状态 = L1写入状态::精确重复;
                    return 结果;
                }
                记录审计(请求.幂等键, L1审计事件::幂等冲突,
                    L1写入状态::幂等冲突, 事实代次_, {});
                return {L1写入状态::幂等冲突, 事实代次_, {}};
            }
            if (请求.期望事实代次 != 事实代次_) {
                return {L1写入状态::事实代次漂移, 事实代次_, {}};
            }
            std::unordered_set<std::uint64_t> 本地编码;
            std::unordered_set<std::uint32_t> 本地键;
            for (const auto& 项 : 请求.节点) {
                if (!有效(项.本地键) || !本地键.insert(项.本地键.值).second) return {};
            }
            for (const auto& 项 : 请求.关系) {
                if (!有效(项.本地键) || !本地键.insert(项.本地键.值).second) return {};
            }
            for (const auto& 项 : 请求.值) {
                if (!有效(项.本地键) || !本地键.insert(项.本地键.值).second
                    || !原始材料完整(项.材料)) return {};
            }
            for (const auto& 项 : 请求.属性槽变更) if (!有效(项.新当前值)) return {};
            auto 可用稳定编码 = [&](稳定编码 编码) {
                return 有效(编码) && !永久占用_.contains(编码.值) && !本地编码.contains(编码.值);
            };
            auto 分配 = [&]() {
                while (下个编码_ == 0 || 永久占用_.contains(下个编码_) || 本地编码.contains(下个编码_)) ++下个编码_;
                const 稳定编码 编码{下个编码_++};
                本地编码.insert(编码.值);
                return 编码;
            };
            std::vector<std::pair<写集本地键, 稳定编码>> 映射;
            for (const auto& 项 : 请求.节点) 映射.emplace_back(项.本地键, 分配());
            for (const auto& 项 : 请求.关系) 映射.emplace_back(项.本地键, 分配());
            for (const auto& 项 : 请求.值) 映射.emplace_back(项.本地键, 分配());
            auto 解析 = [&](const 事实引用& 引用) -> std::optional<稳定编码> {
                return std::visit([&](const auto& 值) -> std::optional<稳定编码> {
                    using T = std::decay_t<decltype(值)>;
                    if constexpr (std::is_same_v<T, 稳定编码>) {
                        return 有效(值) ? std::optional<稳定编码>(值) : std::nullopt;
                    } else {
                        for (const auto& [键, 编码] : 映射) if (键 == 值) return 编码;
                        return std::nullopt;
                    }
                }, 引用);
            };
            for (const auto& 项 : 请求.关系) {
                if (!解析(项.源节点).has_value() || !解析(项.目标节点).has_value()
                    || !解析(项.关系类型节点).has_value()) return {};
            }
            for (const auto& 项 : 请求.值) {
                if (!解析(项.所属节点).has_value() || !解析(项.属性类型节点).has_value()
                    || !解析(项.来源节点).has_value()) return {};
            }
            const auto 新代次 = 事实代次_ + 1;
            std::unordered_map<std::uint64_t, 节点事实> 新节点 = 当前节点_;
            std::unordered_map<std::uint64_t, 关系事实> 新关系 = 当前关系_;
            std::unordered_map<std::uint64_t, 值事实> 新值 = 当前值_;
            auto 新历史 = 历史_;
            auto 新占用 = 永久占用_;
            for (std::size_t i = 0; i < 请求.节点.size(); ++i) {
                const auto 编码 = 映射[i].second;
                新节点.emplace(编码.值, 节点事实{编码, 请求.节点[i].种类, 新代次, std::nullopt, {}});
                新占用.insert(编码.值);
            }
            const std::size_t 关系偏移 = 请求.节点.size();
            for (std::size_t i = 0; i < 请求.关系.size(); ++i) {
                const auto& 项 = 请求.关系[i];
                const auto 源 = 解析(项.源节点); const auto 目标 = 解析(项.目标节点);
                const auto 类型 = 解析(项.关系类型节点);
                if (!源 || !目标 || !类型 || !新节点.contains(源->值) || !新节点.contains(目标->值)) return {};
                const auto 编码 = 映射[关系偏移 + i].second;
                新关系.emplace(编码.值, 关系事实{编码, *源, *目标, *类型, 项.角色或顺序, 新代次, std::nullopt});
                新占用.insert(编码.值);
            }
            const std::size_t 值偏移 = 关系偏移 + 请求.关系.size();
            for (std::size_t i = 0; i < 请求.值.size(); ++i) {
                const auto& 项 = 请求.值[i];
                const auto 所属 = 解析(项.所属节点); const auto 类型 = 解析(项.属性类型节点);
                const auto 来源 = 解析(项.来源节点);
                if (!所属 || !类型 || !来源 || !新节点.contains(所属->值) || !新节点.contains(来源->值)) return {};
                const auto 编码 = 映射[值偏移 + i].second;
                新值.emplace(编码.值, 值事实{编码, *所属, *类型, 项.材料, *来源, 新代次, std::nullopt});
                新占用.insert(编码.值);
            }
            auto 退出一个 = [&](稳定编码 编码) {
                if (auto it = 新节点.find(编码.值); it != 新节点.end()) { auto 事实 = it->second; 事实.退出事实代次 = 新代次; 新历史[编码.值] = {编码, 事实, false}; 新节点.erase(it); return true; }
                if (auto it = 新关系.find(编码.值); it != 新关系.end()) { auto 事实 = it->second; 事实.退出事实代次 = 新代次; 新历史[编码.值] = {编码, 事实, false}; 新关系.erase(it); return true; }
                if (auto it = 新值.find(编码.值); it != 新值.end()) { auto 事实 = it->second; 事实.退出事实代次 = 新代次; 新历史[编码.值] = {编码, 事实, false}; 新值.erase(it); return true; }
                return false;
            };
            for (const auto 编码 : 请求.退出事实) if (!退出一个(编码)) return {L1写入状态::未找到, 事实代次_, {}};
            for (const auto& 项 : 请求.属性槽变更) {
                const auto 所属 = 解析(项.所属节点); const auto 类型 = 解析(项.属性类型节点);
                const auto 新值编码 = [&]() -> std::optional<稳定编码> { for (const auto& [键, 编码] : 映射) if (键 == 项.新当前值) return 编码; return std::nullopt; }();
                if (!所属 || !类型 || !新值编码) return {};
                auto 节点 = 新节点.find(所属->值); if (节点 == 新节点.end()) return {L1写入状态::未找到, 事实代次_, {}};
                auto& 槽组 = 节点->second.当前属性;
                auto 槽 = std::find_if(槽组.begin(), 槽组.end(), [&](const 属性槽& 槽) { return 槽.属性类型节点 == *类型; });
                if (槽 != 槽组.end() && 槽->当前值 == *新值编码) continue;
                if (槽 != 槽组.end()) { const auto 旧值 = 槽->当前值; auto 值 = 新值.find(旧值.值); if (值 != 新值.end()) { auto 历 = 值->second; 历.退出事实代次 = 新代次; 新历史[旧值.值] = {旧值, 历, false}; 新值.erase(值); } 槽->当前值 = *新值编码; }
                else 槽组.push_back({*类型, *新值编码});
                std::sort(槽组.begin(), 槽组.end(), [](const 属性槽& 左, const 属性槽& 右) { return 左.属性类型节点 < 右.属性类型节点; });
            }
            当前节点_ = std::move(新节点); 当前关系_ = std::move(新关系); 当前值_ = std::move(新值);
            历史_ = std::move(新历史); 永久占用_ = std::move(新占用); 事实代次_ = 新代次;
            L1写入结果 结果{L1写入状态::成功, 事实代次_, 映射};
            幂等账_.emplace(请求.幂等键.值, L1幂等账记录{请求.幂等键, 请求, 结果});
            记录审计(请求.幂等键, L1审计事件::提交成功, L1写入状态::成功, 事实代次_, 映射);
            return 结果;
        } catch (const std::bad_alloc&) { return {L1写入状态::资源失败, 事实代次_, {}}; }
        catch (...) { return {L1写入状态::内部不一致, 事实代次_, {}}; }
    }

    L1读取结果 读取当前节点(稳定编码 编码) const { return 读取当前(编码, 当前节点_); }
    L1读取结果 读取当前关系(稳定编码 编码) const { return 读取当前(编码, 当前关系_); }
    L1读取结果 读取当前值(稳定编码 编码) const { return 读取当前(编码, 当前值_); }
    L1属性读取结果 读取当前属性(稳定编码 节点, 稳定编码 类型) const {
        if (!有效(节点) || !有效(类型)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        const auto it = 当前节点_.find(节点.值); if (it == 当前节点_.end()) return {历史_.contains(节点.值) ? L1读取状态::已退出 : L1读取状态::未找到, 0, std::nullopt};
        const auto 槽 = std::find_if(it->second.当前属性.begin(), it->second.当前属性.end(), [&](const 属性槽& 项) { return 项.属性类型节点 == 类型; });
        if (槽 == it->second.当前属性.end()) return {L1读取状态::属性未设置, 事实代次_, std::nullopt};
        return {L1读取状态::成功, 事实代次_, L1属性读取副本{节点, 类型, 槽->当前值}};
    }
    L1历史读取结果 读取历史事实(稳定编码 编码) const {
        if (!有效(编码)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (auto it = 当前节点_.find(编码.值); it != 当前节点_.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 当前关系_.find(编码.值); it != 当前关系_.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 当前值_.find(编码.值); it != 当前值_.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 历史_.find(编码.值); it != 历史_.end()) return {L1读取状态::成功, it->second};
        return {L1读取状态::未找到, std::nullopt};
    }
    L1审计读取结果 读取审计(写集幂等键 幂等键) const {
        if (!有效(幂等键)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        const auto it = 审计_.find(幂等键.值); if (it == 审计_.end()) return {L1读取状态::未找到, {}};
        return {L1读取状态::成功, it->second};
    }
    L1完整快照结果 读取完整快照() const {
        std::shared_lock<std::shared_mutex> 锁(锁_);
        L1完整快照 快照; 快照.事实代次 = 事实代次_;
        for (const auto& [_, 事实] : 当前节点_) 快照.当前节点.push_back(事实);
        for (const auto& [_, 事实] : 当前关系_) 快照.当前关系.push_back(事实);
        for (const auto& [_, 事实] : 当前值_) 快照.当前值.push_back(事实);
        for (const auto 编码 : 永久占用_) 快照.永久占用编码.push_back({编码});
        排序(快照); return {L1读取状态::成功, std::move(快照)};
    }
    L1恢复结果 建立恢复候选(const L1恢复材料& 材料, std::uint64_t 期望) {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (候选_.has_value() || 材料.当前快照.事实代次 != 期望 || 期望 != 事实代次_) return {L1恢复状态::事实代次漂移, 事实代次_};
            if (!恢复材料完整(材料)) return {L1恢复状态::材料不完整, 事实代次_};
            候选_ = 候选状态{材料, 事实代次_};
            记录恢复审计(材料, L1审计事件::恢复候选建立, L1写入状态::成功, 事实代次_);
            return {L1恢复状态::候选已建立, 事实代次_};
        } catch (const std::bad_alloc&) { return {L1恢复状态::资源失败, 事实代次_}; }
        catch (...) { return {L1恢复状态::内部不一致, 事实代次_}; }
    }
    L1恢复结果 确认恢复候选() {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!候选_) return {L1恢复状态::无候选, 事实代次_};
            if (候选_->基线 != 事实代次_) { 候选_.reset(); return {L1恢复状态::事实代次漂移, 事实代次_}; }
            当前节点_.clear(); 当前关系_.clear(); 当前值_.clear(); 历史_.clear();
            for (const auto& 事实 : 候选_->材料.当前快照.当前节点) 当前节点_.emplace(事实.编码.值, 事实);
            for (const auto& 事实 : 候选_->材料.当前快照.当前关系) 当前关系_.emplace(事实.编码.值, 事实);
            for (const auto& 事实 : 候选_->材料.当前快照.当前值) 当前值_.emplace(事实.编码.值, 事实);
            永久占用_.clear(); for (const auto 编码 : 候选_->材料.当前快照.永久占用编码) 永久占用_.insert(编码.值);
            for (const auto& 事实 : 候选_->材料.历史节点) 历史_[事实.编码.值] = {事实.编码, 事实, false};
            for (const auto& 事实 : 候选_->材料.历史关系) 历史_[事实.编码.值] = {事实.编码, 事实, false};
            for (const auto& 事实 : 候选_->材料.历史值) 历史_[事实.编码.值] = {事实.编码, 事实, false};
            幂等账_.clear(); 审计_.clear(); for (const auto& 账 : 候选_->材料.幂等账) 幂等账_[账.幂等键.值] = 账; for (const auto& 记录 : 候选_->材料.审计记录) 审计_[记录.幂等键.值].push_back(记录);
            ++事实代次_; 候选_.reset(); return {L1恢复状态::恢复已发布, 事实代次_};
        } catch (const std::bad_alloc&) { return {L1恢复状态::资源失败, 事实代次_}; }
        catch (...) { return {L1恢复状态::内部不一致, 事实代次_}; }
    }
    L1恢复结果 撤销恢复候选() noexcept {
        try { std::unique_lock<std::shared_mutex> 锁(锁_); if (!候选_) return {L1恢复状态::无候选, 事实代次_}; 候选_.reset(); return {L1恢复状态::候选已撤销, 事实代次_}; }
        catch (...) { return {L1恢复状态::内部不一致, 事实代次_}; }
    }

private:
    struct 候选状态 { L1恢复材料 材料; std::uint64_t 基线 = 0; };
    template<class T> L1读取结果 读取当前(稳定编码 编码, const std::unordered_map<std::uint64_t, T>& 表) const {
        if (!有效(编码)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_); const auto it = 表.find(编码.值);
        if (it != 表.end()) return {L1读取状态::成功, 事实代次_, L1事实副本{it->second}};
        return {历史_.contains(编码.值) ? L1读取状态::已退出 : L1读取状态::未找到, 0, std::nullopt};
    }
    void 记录审计(写集幂等键 幂等键, L1审计事件 事件, L1写入状态 状态, std::uint64_t 代次, const std::vector<std::pair<写集本地键, 稳定编码>>& 映射) {
        auto& 组 = 审计_[幂等键.值]; 组.push_back({幂等键, 组.size() + 1, 事件, 状态, 代次, 映射});
    }
    void 记录恢复审计(const L1恢复材料& 材料, L1审计事件 事件, L1写入状态 状态, std::uint64_t 代次) { if (!材料.幂等账.empty()) 记录审计(材料.幂等账.front().幂等键, 事件, 状态, 代次, {}); }
    static void 排序(L1完整快照& 快照) {
        auto 编码排序=[](const auto& 左,const auto& 右){return 左.编码<右.编码;}; std::sort(快照.当前节点.begin(),快照.当前节点.end(),编码排序); std::sort(快照.当前关系.begin(),快照.当前关系.end(),编码排序); std::sort(快照.当前值.begin(),快照.当前值.end(),编码排序); std::sort(快照.永久占用编码.begin(),快照.永久占用编码.end());
    }
    static bool 恢复材料完整(const L1恢复材料& 材料) {
        std::unordered_set<std::uint64_t> 所有; for (const auto& 项 : 材料.当前快照.当前节点) if (!有效(项.编码)||!所有.insert(项.编码.值).second) return false; for (const auto& 项 : 材料.当前快照.当前关系) if (!有效(项.编码)||!所有.insert(项.编码.值).second) return false; for (const auto& 项 : 材料.当前快照.当前值) if (!有效(项.编码)||!所有.insert(项.编码.值).second||!原始材料完整(项.材料)) return false; for (const auto& 项 : 材料.历史节点) if (!有效(项.编码)||!项.退出事实代次||!所有.insert(项.编码.值).second) return false; for (const auto& 项 : 材料.历史关系) if (!有效(项.编码)||!项.退出事实代次||!所有.insert(项.编码.值).second) return false; for (const auto& 项 : 材料.历史值) if (!有效(项.编码)||!项.退出事实代次||!所有.insert(项.编码.值).second) return false; std::unordered_set<std::uint64_t> 占用; for (const auto 编码:材料.当前快照.永久占用编码) if(!有效(编码)||!占用.insert(编码.值).second) return false; for (auto 编码:所有) if(!占用.contains(编码)) return false; for (const auto& 项:材料.当前快照.当前关系) if(!所有.contains(项.源节点.值)||!所有.contains(项.目标节点.值)||!所有.contains(项.关系类型节点.值)) return false; return true;
    }
    mutable std::shared_mutex 锁_;
    std::uint64_t 事实代次_ = 0;
    std::uint64_t 下个编码_ = 1;
    std::unordered_map<std::uint64_t, 节点事实> 当前节点_;
    std::unordered_map<std::uint64_t, 关系事实> 当前关系_;
    std::unordered_map<std::uint64_t, 值事实> 当前值_;
    std::unordered_map<std::uint64_t, L1历史事实副本> 历史_;
    std::unordered_set<std::uint64_t> 永久占用_;
    std::unordered_map<std::uint64_t, L1幂等账记录> 幂等账_;
    std::unordered_map<std::uint64_t, std::vector<L1审计记录>> 审计_;
    std::optional<候选状态> 候选_;
};

} // namespace 海中鱼巣
