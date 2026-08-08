module;

#include <algorithm>
#include <atomic>
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
#include <type_traits>

#define L1_FACT_BASE_NO_INCLUDES

export module 海中鱼巣.核心.仓库.L1事实基座;
import 海中鱼巣.核心.合同.L1事实基座;
import 海中鱼巣.核心.合同.L1中性CRUD;

export namespace 海中鱼巣 {

class L1事实基座仓库 final {
public:
    L1事实基座仓库() = default;
    L1事实基座仓库(const L1事实基座仓库&) = delete;
    L1事实基座仓库& operator=(const L1事实基座仓库&) = delete;

    // 诊断责任：向上送出；全部非成功均由中性结构化状态携带。
    L1中性写入结果 提交中性写集(const L1中性写集请求& 请求) {
        try {
            const auto 规范化 = 规范化中性写集(请求);
            if (!规范化 || 请求.合同版本 != L1中性CRUD合同版本
                || !有效(请求.幂等键)
                || (请求.节点.empty() && 请求.关系.empty() && 请求.值.empty()
                    && 请求.属性槽变更.empty() && 请求.退出事实.empty()))
                return 中性写入结果(请求, L1中性写入状态::入口拒绝, 0, false,
                    L1中性重试边界::修正请求后可重试);

            std::unordered_set<std::uint32_t> 本地键;
            if (!中性请求结构有效(*规范化, 本地键))
                return 中性写入结果(请求, L1中性写入状态::入口拒绝, 0, false,
                    L1中性重试边界::修正请求后可重试);

            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_))
                return 中性写入结果(请求, L1中性写入状态::内部不一致,
                    状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);

            const auto 中性既有 = 状态_.中性幂等账.find(请求.幂等键.值);
            if (中性既有 != 状态_.中性幂等账.end()) {
                if (中性既有->second.首次规范化写集 == *规范化) {
                    auto 结果 = 中性既有->second.首次结果;
                    结果.状态 = L1中性写入状态::精确重复;
                    结果.是否形成内存权威发布 = false;
                    结果.重试边界 = L1中性重试边界::原幂等键读回收敛;
                    return 结果;
                }
                return 中性写入结果(请求, L1中性写入状态::幂等冲突,
                    状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
            }
            if (状态_.幂等账.contains(请求.幂等键.值))
                return 中性写入结果(请求, L1中性写入状态::幂等冲突,
                    状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
            if (请求.期望事实代次 != 状态_.事实代次)
                return 中性写入结果(请求, L1中性写入状态::事实代次漂移,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);

            状态 候选 = 状态_;
            std::vector<std::pair<L1中性写集本地键, 稳定编码>> 映射;
            映射.reserve(规范化->节点.size() + 规范化->关系.size() + 规范化->值.size());
            auto 分配 = [&]() -> std::optional<稳定编码> {
                while (候选.下个编码 != 0 && 候选.永久占用.contains(候选.下个编码))
                    ++候选.下个编码;
                if (候选.下个编码 == 0) return std::nullopt;
                const 稳定编码 编码{候选.下个编码++};
                return 编码;
            };
            for (const auto& 项 : 规范化->节点) {
                const auto 编码 = 分配();
                if (!编码) return 中性写入结果(请求, L1中性写入状态::资源失败,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);
                映射.emplace_back(项.本地键, *编码);
            }
            for (const auto& 项 : 规范化->关系) {
                const auto 编码 = 分配();
                if (!编码) return 中性写入结果(请求, L1中性写入状态::资源失败,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);
                映射.emplace_back(项.本地键, *编码);
            }
            for (const auto& 项 : 规范化->值) {
                const auto 编码 = 分配();
                if (!编码) return 中性写入结果(请求, L1中性写入状态::资源失败,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);
                映射.emplace_back(项.本地键, *编码);
            }

            auto 解析节点 = [&](const L1中性事实引用& 引用,
                L1中性写入状态& 失败) -> std::optional<稳定编码> {
                return std::visit([&](const auto& 值) -> std::optional<稳定编码> {
                    using 类型 = std::decay_t<decltype(值)>;
                    if constexpr (std::is_same_v<类型, 稳定编码>) {
                        if (!有效(值)) { 失败 = L1中性写入状态::入口拒绝; return std::nullopt; }
                        if (状态_.当前节点.contains(值.值)) return 值;
                        if (状态_.历史.contains(值.值)) 失败 = L1中性写入状态::已退出;
                        else if (状态_.当前关系.contains(值.值) || 状态_.当前值.contains(值.值))
                            失败 = L1中性写入状态::入口拒绝;
                        else 失败 = L1中性写入状态::未找到;
                        return std::nullopt;
                    } else {
                        for (std::size_t i = 0; i < 规范化->节点.size(); ++i)
                            if (规范化->节点[i].本地键 == 值) return 映射[i].second;
                        失败 = L1中性写入状态::入口拒绝;
                        return std::nullopt;
                    }
                }, 引用);
            };
            auto 查映射 = [&](L1中性写集本地键 键) -> std::optional<稳定编码> {
                for (const auto& [本地, 编码] : 映射) if (本地 == 键) return 编码;
                return std::nullopt;
            };
            const auto 新代次 = 状态_.事实代次 + 1;

            for (std::size_t i = 0; i < 规范化->节点.size(); ++i) {
                const auto& 项 = 规范化->节点[i];
                if ((项.种类 == 节点种类::属性类型) != 项.属性类型表示.has_value())
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                候选.当前节点.emplace(映射[i].second.值, 节点事实{映射[i].second,
                    项.种类, 转换值表示种类(项.属性类型表示), 新代次, std::nullopt, {}});
                候选.永久占用.insert(映射[i].second.值);
            }

            const std::size_t 关系偏移 = 规范化->节点.size();
            for (std::size_t i = 0; i < 规范化->关系.size(); ++i) {
                const auto& 项 = 规范化->关系[i];
                L1中性写入状态 失败 = L1中性写入状态::入口拒绝;
                const auto 源 = 解析节点(项.源节点, 失败);
                if (!源) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 目标 = 解析节点(项.目标节点, 失败);
                if (!目标) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型 = 解析节点(项.关系类型节点, 失败);
                if (!类型) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 编码 = 映射[关系偏移 + i].second;
                const 关系事实 事实{编码, *源, *目标, *类型,
                    项.角色或顺序, 新代次, std::nullopt};
                if (!候选.当前关系.emplace(编码.值, 事实).second
                    || !插入当前源关系索引(候选, 事实)
                    || !插入当前目标关系索引(候选, 事实))
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);
                候选.永久占用.insert(编码.值);
            }

            const std::size_t 值偏移 = 关系偏移 + 规范化->关系.size();
            for (std::size_t i = 0; i < 规范化->值.size(); ++i) {
                const auto& 项 = 规范化->值[i];
                L1中性写入状态 失败 = L1中性写入状态::入口拒绝;
                const auto 所属 = 解析节点(项.所属节点, 失败);
                if (!所属) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型编码 = 解析节点(项.属性类型节点, 失败);
                if (!类型编码) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 来源 = 解析节点(项.来源节点, 失败);
                if (!来源) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型节点 = 候选.当前节点.find(类型编码->值);
                const auto 材料 = 转换原始材料(项.材料);
                if (!材料 || 类型节点 == 候选.当前节点.end()
                    || 类型节点->second.种类 != 节点种类::属性类型
                    || !类型节点->second.属性类型表示
                    || !表示匹配(*类型节点->second.属性类型表示, *材料))
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                if (const auto* 引用 = std::get_if<独立材料引用>(&*材料);
                    引用 && !状态_.当前节点.contains(引用->编码.值))
                    return 中性写入结果(请求,
                        状态_.历史.contains(引用->编码.值)
                            ? L1中性写入状态::已退出 : L1中性写入状态::未找到,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                const auto 编码 = 映射[值偏移 + i].second;
                候选.当前值.emplace(编码.值, 值事实{编码, *所属, *类型编码,
                    *材料, *来源, 新代次, std::nullopt});
                候选.永久占用.insert(编码.值);
            }

            auto 退出一个 = [&](稳定编码 编码) {
                if (auto it = 候选.当前节点.find(编码.值); it != 候选.当前节点.end()) {
                    auto 事实 = it->second; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false};
                    候选.当前节点.erase(it); return true;
                }
                if (auto it = 候选.当前关系.find(编码.值); it != 候选.当前关系.end()) {
                    const auto 当前事实 = it->second;
                    if (!删除当前源关系索引(候选, 当前事实)
                        || !删除当前目标关系索引(候选, 当前事实)) return false;
                    auto 事实 = 当前事实; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false};
                    候选.当前关系.erase(it); return true;
                }
                if (auto it = 候选.当前值.find(编码.值); it != 候选.当前值.end()) {
                    auto 事实 = it->second; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false};
                    候选.当前值.erase(it);
                    for (auto& [_, 节点] : 候选.当前节点)
                        节点.当前属性.erase(std::remove_if(节点.当前属性.begin(),
                            节点.当前属性.end(), [&](const 属性槽& 槽) {
                                return 槽.当前值 == 编码;
                            }), 节点.当前属性.end());
                    return true;
                }
                return false;
            };
            for (const auto 编码 : 规范化->退出事实) {
                if (!状态_.当前节点.contains(编码.值)
                    && !状态_.当前关系.contains(编码.值)
                    && !状态_.当前值.contains(编码.值))
                    return 中性写入结果(请求,
                        状态_.历史.contains(编码.值)
                            ? L1中性写入状态::已退出 : L1中性写入状态::未找到,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
            }
            for (const auto 编码 : 规范化->退出事实)
                if (!退出一个(编码))
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);

            for (const auto& 项 : 规范化->属性槽变更) {
                L1中性写入状态 失败 = L1中性写入状态::入口拒绝;
                const auto 所属 = 解析节点(项.所属节点, 失败);
                if (!所属) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型 = 解析节点(项.属性类型节点, 失败);
                if (!类型) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 新值编码 = 查映射(项.新当前值);
                const auto 新值 = 新值编码 ? 候选.当前值.find(新值编码->值)
                    : 候选.当前值.end();
                auto 节点 = 候选.当前节点.find(所属->值);
                if (!新值编码 || 新值 == 候选.当前值.end()
                    || 节点 == 候选.当前节点.end()
                    || 新值->second.所属节点 != *所属
                    || 新值->second.属性类型节点 != *类型)
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                auto& 槽组 = 节点->second.当前属性;
                auto 槽 = std::find_if(槽组.begin(), 槽组.end(), [&](const 属性槽& 值) {
                    return 值.属性类型节点 == *类型;
                });
                if (槽 != 槽组.end()) {
                    const auto 旧值编码 = 槽->当前值;
                    if (auto 旧值 = 候选.当前值.find(旧值编码.值);
                        旧值 != 候选.当前值.end()) {
                        auto 历史值 = 旧值->second;
                        历史值.退出事实代次 = 新代次;
                        候选.历史[旧值编码.值] = {旧值编码, 历史值, false};
                        候选.当前值.erase(旧值);
                    }
                    槽->当前值 = *新值编码;
                } else 槽组.push_back({*类型, *新值编码});
                std::sort(槽组.begin(), 槽组.end(), [](const 属性槽& 左, const 属性槽& 右) {
                    return 左.属性类型节点 < 右.属性类型节点;
                });
            }

            候选.事实代次 = 新代次;
            std::sort(映射.begin(), 映射.end(), [](const auto& 左, const auto& 右) {
                return 左.first < 右.first;
            });
            if (!中性候选读回完整(候选, *规范化, 映射, 新代次)
                || !状态完整(候选))
                return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                    状态_.事实代次, false, L1中性重试边界::修正请求后可重试);

            auto 结果 = 中性写入结果(请求, L1中性写入状态::成功, 新代次, true,
                L1中性重试边界::不适用, 映射);
            候选.中性幂等账.emplace(请求.幂等键.值,
                中性幂等记录{*规范化, 新代次, 映射, 结果});
            if (!状态完整(候选))
                return 中性写入结果(请求, L1中性写入状态::内部不一致,
                    状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);

            std::swap(状态_, 候选);
            if (!中性候选读回完整(状态_, *规范化, 映射, 新代次)) {
                状态_.隔离 = true;
                return 中性写入结果(请求, L1中性写入状态::内部不一致,
                    新代次, true, L1中性重试边界::原幂等键读回收敛);
            }
            return 结果;
        } catch (const std::bad_alloc&) {
            return 中性写入结果(请求, L1中性写入状态::资源失败, 0, false,
                L1中性重试边界::原请求可重试);
        } catch (...) {
            return 中性写入结果(请求, L1中性写入状态::内部不一致, 0, false,
                L1中性重试边界::原幂等键读回收敛);
        }
    }

    L1读取结果 读取当前节点(稳定编码 编码) const { return 读取当前(编码, 状态_.当前节点); }
    L1读取结果 读取当前关系(稳定编码 编码) const { return 读取当前(编码, 状态_.当前关系); }
    L1读取结果 读取当前值(稳定编码 编码) const { return 读取当前(编码, 状态_.当前值); }
    L1中性源关系读取结果 读取中性当前源关系组(
        const L1中性源关系读取请求& 请求) const {
        L1中性源关系读取结果 结果{L1中性读取状态::入口拒绝,
            L1中性CRUD合同版本, 请求.源节点, 请求.关系类型节点, 0, {}};
        if (请求.合同版本 != L1中性CRUD合同版本
            || !有效(请求.源节点) || !有效(请求.关系类型节点)) return 结果;
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        const auto 源 = 状态_.当前源关系索引.find(请求.源节点.值);
        if (源 == 状态_.当前源关系索引.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        const auto 类型 = 源->second.find(请求.关系类型节点.值);
        if (类型 == 源->second.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        if (状态_.事实代次 == 0 || 类型->second.empty()) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        结果.关系组.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 状态_.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 状态_.当前关系.end()
                || 关系->second.编码.值 != 编码
                || 关系->second.源节点 != 请求.源节点
                || 关系->second.关系类型节点 != 请求.关系类型节点
                || 关系->second.退出事实代次.has_value()) {
                结果.状态 = L1中性读取状态::内部不一致;
                结果.关系组.clear();
                return 结果;
            }
            const auto& 事实 = 关系->second;
            结果.关系组.push_back({事实.编码, 事实.源节点, 事实.目标节点,
                事实.关系类型节点, 事实.角色或顺序, 事实.创建事实代次,
                事实.退出事实代次});
            前一编码 = 编码;
        }
        结果.状态 = L1中性读取状态::成功;
        结果.读取事实代次 = 状态_.事实代次;
        return 结果;
    }
    L1中性目标关系读取结果 读取中性当前目标关系组(
        const L1中性目标关系读取请求& 请求) const {
        L1中性目标关系读取结果 结果{L1中性读取状态::入口拒绝,
            L1中性CRUD合同版本, 请求.目标节点, 请求.关系类型节点, 0, {}};
        if (请求.合同版本 != L1中性CRUD合同版本
            || !有效(请求.目标节点) || !有效(请求.关系类型节点)) return 结果;
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        const auto 目标 = 状态_.当前目标关系索引.find(请求.目标节点.值);
        if (目标 == 状态_.当前目标关系索引.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        const auto 类型 = 目标->second.find(请求.关系类型节点.值);
        if (类型 == 目标->second.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        if (状态_.事实代次 == 0 || 类型->second.empty()) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        结果.关系组.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 状态_.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 状态_.当前关系.end()
                || 关系->second.编码.值 != 编码
                || 关系->second.目标节点 != 请求.目标节点
                || 关系->second.关系类型节点 != 请求.关系类型节点
                || 关系->second.退出事实代次.has_value()) {
                结果.状态 = L1中性读取状态::内部不一致;
                结果.关系组.clear();
                return 结果;
            }
            const auto& 事实 = 关系->second;
            结果.关系组.push_back({事实.编码, 事实.源节点, 事实.目标节点,
                事实.关系类型节点, 事实.角色或顺序, 事实.创建事实代次,
                事实.退出事实代次});
            前一编码 = 编码;
        }
        结果.状态 = L1中性读取状态::成功;
        结果.读取事实代次 = 状态_.事实代次;
        return 结果;
    }
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
            const auto it = 状态_.审计.find(幂等键.值);
        if (it == 状态_.审计.end()) return {L1读取状态::未找到, 幂等键, {}};
        return {L1读取状态::成功, 幂等键, it->second};
    }
    // 诊断责任：向上送出；结构化状态由最终调用责任边界处理。
    L1完整快照结果 读取完整快照() const {
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_))
                return {L1读取状态::内部不一致, std::nullopt};
            const auto 快照 = 构造快照(状态_);
            if (!快照) return {L1读取状态::内部不一致, std::nullopt};
            return {L1读取状态::成功, std::move(*快照)};
        } catch (const std::bad_alloc&) {
            return {L1读取状态::资源失败, std::nullopt};
        } catch (...) {
            return {L1读取状态::内部不一致, std::nullopt};
        }
    }
    // 诊断责任：向上送出；竞争、资源失败和内部不一致均由结构化状态携带。
    L1完整快照结果 尝试读取完整快照() const {
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock()) return {L1读取状态::许可拒绝, std::nullopt};
            if (状态_.隔离 || !状态完整(状态_))
                return {L1读取状态::内部不一致, std::nullopt};
            const auto 快照 = 构造快照(状态_);
            if (!快照) return {L1读取状态::内部不一致, std::nullopt};
            return {L1读取状态::成功, std::move(*快照)};
        } catch (const std::bad_alloc&) {
            return {L1读取状态::资源失败, std::nullopt};
        } catch (...) {
            return {L1读取状态::内部不一致, std::nullopt};
        }
    }
    // 诊断责任：向上送出；只复制同一共享许可内的权威事实代次。
    L1事实代次读取结果 尝试读取当前事实代次() const {
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock()) return {L1读取状态::许可拒绝, 0};
            if (状态_.隔离 || !状态完整(状态_))
                return {L1读取状态::内部不一致, 0};
            if (状态_.事实代次 == 0) return {L1读取状态::未找到, 0};
            return {L1读取状态::成功, 状态_.事实代次};
        } catch (...) {
            return {L1读取状态::内部不一致, 0};
        }
    }
    // 诊断责任：向上送出；中性账守门与导出失败均结构化返回。
    L1恢复材料导出结果 导出恢复材料() const {
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (!状态_.中性幂等账.empty())
                return {L1恢复材料导出状态::入口拒绝, 状态_.事实代次,
                    状态_.隔离, std::nullopt};
            if (!状态完整(状态_)
                || 状态_.隔离 != 状态_.当前隔离见证身份.has_value())
                return {L1恢复材料导出状态::内部不一致, 状态_.事实代次,
                    状态_.隔离, std::nullopt};
            L1恢复材料 材料;
            const auto 快照 = 构造快照(状态_);
            if (!快照) return {L1恢复材料导出状态::内部不一致,
                状态_.事实代次, 状态_.隔离, std::nullopt};
            材料.当前快照 = *快照;
            for (const auto& [_, 历史] : 状态_.历史) {
                std::visit([&](const auto& 事实) {
                    using T = std::decay_t<decltype(事实)>;
                    if constexpr (std::is_same_v<T, 节点事实>) 材料.历史节点.push_back(事实);
                    else if constexpr (std::is_same_v<T, 关系事实>) 材料.历史关系.push_back(事实);
                    else 材料.历史值.push_back(事实);
                }, 历史.事实);
            }
            auto 按编码 = [](const auto& 左, const auto& 右) { return 左.编码 < 右.编码; };
            std::sort(材料.历史节点.begin(), 材料.历史节点.end(), 按编码);
            std::sort(材料.历史关系.begin(), 材料.历史关系.end(), 按编码);
            std::sort(材料.历史值.begin(), 材料.历史值.end(), 按编码);
            for (const auto& [_, 账] : 状态_.幂等账) 材料.幂等账.push_back(账);
            std::sort(材料.幂等账.begin(), 材料.幂等账.end(),
                [](const auto& 左, const auto& 右) { return 左.幂等键.值 < 右.幂等键.值; });
            for (const auto& [_, 记录组] : 状态_.审计)
                材料.审计记录.insert(材料.审计记录.end(), 记录组.begin(), 记录组.end());
            std::sort(材料.审计记录.begin(), 材料.审计记录.end(),
                [](const auto& 左, const auto& 右) {
                    return 左.幂等键 != 右.幂等键 ? 左.幂等键.值 < 右.幂等键.值
                        : 左.事件序号 < 右.事件序号;
                });
            材料.发布后读回失败见证组 = 状态_.失败见证;
            std::sort(材料.发布后读回失败见证组.begin(), 材料.发布后读回失败见证组.end(),
                [](const auto& 左, const auto& 右) {
                    return 左.身份.幂等键 != 右.身份.幂等键
                        ? 左.身份.幂等键.值 < 右.身份.幂等键.值
                        : 左.身份.审计事件序号 < 右.身份.审计事件序号;
                });
            材料.当前隔离见证身份 = 状态_.当前隔离见证身份;
            for (const auto& [_, 记录] : 状态_.领域结果见证状态组)
                材料.领域结果见证记录组.push_back(记录);
            std::sort(材料.领域结果见证记录组.begin(), 材料.领域结果见证记录组.end(),
                [](const auto& 左, const auto& 右) { return 左.幂等键.值 < 右.幂等键.值; });
            return {L1恢复材料导出状态::成功, 状态_.事实代次,
                状态_.隔离, std::move(材料)};
        } catch (const std::bad_alloc&) {
            return {L1恢复材料导出状态::资源失败, 0, false, std::nullopt};
        } catch (...) {
            return {L1恢复材料导出状态::内部不一致, 0, false, std::nullopt};
        }
    }
    // 诊断责任：向上送出；中性账守门与候选失败均结构化返回。
    L1恢复结果 建立恢复候选(const L1恢复材料& 材料, std::uint64_t 期望) {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!状态_.中性幂等账.empty())
                return {L1恢复状态::入口拒绝, 状态_.事实代次};
            if (候选_.has_value()) return {L1恢复状态::入口拒绝, 状态_.事实代次};
            if (!状态完整(状态_)) {
                状态_.隔离 = true;
                return {L1恢复状态::内部不一致, 状态_.事实代次};
            }
            if (材料.当前快照.事实代次 != 期望 || 期望 != 状态_.事实代次) return {L1恢复状态::事实代次漂移, 状态_.事实代次};
            状态 值;
            if (!恢复材料转状态(材料, 值) || !状态完整(值)) return {L1恢复状态::材料不完整, 状态_.事实代次};
            候选_ = 候选状态{std::move(值), 状态_.事实代次, true};
            return {L1恢复状态::候选已建立, 状态_.事实代次};
        } catch (const std::bad_alloc&) { return {L1恢复状态::资源失败, 0}; }
        catch (...) { return {L1恢复状态::内部不一致, 0}; }
    }
    // 诊断责任：向上送出；中性账守门与确认失败均结构化返回。
    L1恢复结果 确认恢复候选() {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!状态_.中性幂等账.empty())
                return {L1恢复状态::入口拒绝, 状态_.事实代次};
            if (!候选_) return {L1恢复状态::无候选, 状态_.事实代次};
            if (候选_->基线 != 状态_.事实代次) { 候选_.reset(); return {L1恢复状态::事实代次漂移, 状态_.事实代次}; }
            if (!状态完整(状态_)) {
                状态_.隔离 = true;
                return {L1恢复状态::内部不一致, 状态_.事实代次};
            }
            状态 值 = 候选_->值;
            if (!状态完整(值)) {
                候选_.reset(); 状态_.隔离 = true;
                return {L1恢复状态::内部不一致, 状态_.事实代次};
            }
            值.事实代次 = 状态_.事实代次 + 1;
            值.隔离 = false;
            值.当前隔离见证身份.reset();
            if (!状态完整(值)) { 候选_.reset(); 状态_.隔离 = true; return {L1恢复状态::内部不一致, 状态_.事实代次}; }
            const auto 预期快照 = 构造快照(值);
            std::swap(状态_, 值); 候选_.reset();
            const auto 快照 = 构造快照(状态_);
            const bool 恢复状态完整 = 状态完整(状态_);
            if (!快照 || !预期快照 || *快照 != *预期快照
                || 快照->事实代次 != 状态_.事实代次 || !恢复状态完整) {
                状态_.隔离 = true;
                return {L1恢复状态::内部不一致, 状态_.事实代次};
            }
            return {L1恢复状态::恢复已发布, 状态_.事实代次};
        } catch (const std::bad_alloc&) { return {L1恢复状态::资源失败, 0}; }
        catch (...) { return {L1恢复状态::内部不一致, 0}; }
    }
    // 诊断责任：向上送出；撤销许可与异常均结构化返回。
    L1恢复结果 撤销恢复候选() noexcept {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!候选_) return {L1恢复状态::无候选, 状态_.事实代次};
            if (!状态_.中性幂等账.empty() && !候选_->中性发布前建立)
                return {L1恢复状态::入口拒绝, 状态_.事实代次};
            候选_.reset(); return {L1恢复状态::候选已撤销, 状态_.事实代次};
        } catch (...) { return {L1恢复状态::内部不一致, 0}; }
    }


private:
    struct 中性幂等记录 {
        L1中性写集请求 首次规范化写集;
        std::uint64_t 首次发布事实代次 = 0;
        std::vector<std::pair<L1中性写集本地键, 稳定编码>> 首次新编码映射;
        L1中性写入结果 首次结果;
    };
    struct 状态 {
        std::uint64_t 事实代次 = 0;
        std::uint64_t 下个编码 = 1;
        std::unordered_map<std::uint64_t, 节点事实> 当前节点;
        std::unordered_map<std::uint64_t, 关系事实> 当前关系;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            当前源关系索引;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            当前目标关系索引;
        std::unordered_map<std::uint64_t, 值事实> 当前值;
        std::unordered_map<std::uint64_t, L1历史事实副本> 历史;
        std::unordered_set<std::uint64_t> 永久占用;
        std::unordered_map<std::uint64_t, L1幂等账记录> 幂等账;
        std::unordered_map<std::uint64_t, 中性幂等记录> 中性幂等账;
        std::unordered_map<std::uint64_t, std::vector<L1审计记录>> 审计;
        std::unordered_map<std::uint64_t, L1领域结果见证记录> 领域结果见证状态组;
        std::vector<L1发布后读回失败见证> 失败见证;
        std::optional<L1失败见证身份> 当前隔离见证身份;
        bool 隔离 = false;
    };
    struct 候选状态 {
        状态 值;
        std::uint64_t 基线 = 0;
        bool 中性发布前建立 = false;
    };

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入当前源关系索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.源节点)
            || !有效(事实.关系类型节点) || 事实.退出事实代次) return false;
        auto& 编码组 = 值.当前源关系索引[事实.源节点.值][事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：无适用错误分支；只删除候选状态中的一个派生索引编码。
    static bool 删除当前源关系索引(状态& 值, const 关系事实& 事实) noexcept {
        const auto 源 = 值.当前源关系索引.find(事实.源节点.值);
        if (源 == 值.当前源关系索引.end()) return false;
        const auto 类型 = 源->second.find(事实.关系类型节点.值);
        if (类型 == 源->second.end()) return false;
        const auto 位置 = std::lower_bound(类型->second.begin(), 类型->second.end(), 事实.编码.值);
        if (位置 == 类型->second.end() || *位置 != 事实.编码.值) return false;
        类型->second.erase(位置);
        if (类型->second.empty()) 源->second.erase(类型);
        if (源->second.empty()) 值.当前源关系索引.erase(源);
        return true;
    }

    // 诊断责任：向上送出；只从本状态权威当前关系纯派生非权威索引。
    static bool 派生当前源关系索引(状态& 值) {
        值.当前源关系索引.clear();
        for (const auto& [_, 事实] : 值.当前关系)
            if (!插入当前源关系索引(值, 事实)) return false;
        return true;
    }

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入当前目标关系索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.目标节点)
            || !有效(事实.关系类型节点) || 事实.退出事实代次) return false;
        auto& 编码组 = 值.当前目标关系索引[事实.目标节点.值][事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：无适用错误分支；只删除候选状态中的一个派生索引编码。
    static bool 删除当前目标关系索引(状态& 值, const 关系事实& 事实) noexcept {
        const auto 目标 = 值.当前目标关系索引.find(事实.目标节点.值);
        if (目标 == 值.当前目标关系索引.end()) return false;
        const auto 类型 = 目标->second.find(事实.关系类型节点.值);
        if (类型 == 目标->second.end()) return false;
        const auto 位置 = std::lower_bound(类型->second.begin(), 类型->second.end(), 事实.编码.值);
        if (位置 == 类型->second.end() || *位置 != 事实.编码.值) return false;
        类型->second.erase(位置);
        if (类型->second.empty()) 目标->second.erase(类型);
        if (目标->second.empty()) 值.当前目标关系索引.erase(目标);
        return true;
    }

    // 诊断责任：向上送出；只从本状态权威当前关系纯派生非权威索引。
    static bool 派生当前目标关系索引(状态& 值) {
        值.当前目标关系索引.clear();
        for (const auto& [_, 事实] : 值.当前关系)
            if (!插入当前目标关系索引(值, 事实)) return false;
        return true;
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static L1中性写入结果 中性写入结果(const L1中性写集请求& 请求,
        L1中性写入状态 状态值, std::uint64_t 代次, bool 已发布,
        L1中性重试边界 重试,
        std::vector<std::pair<L1中性写集本地键, 稳定编码>> 映射 = {}) {
        return {状态值, L1中性CRUD合同版本, 请求.幂等键, 代次,
            已发布, 重试, std::move(映射)};
    }

    // 诊断责任：无适用错误分支；纯值排序键转换。
    static std::uint64_t 中性引用排序键(const L1中性事实引用& 引用) {
        return std::visit([](const auto& 值) -> std::uint64_t {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, 稳定编码>) return 值.值;
            else return (1ULL << 63) | 值.值;
        }, 引用);
    }

    // 诊断责任：无适用错误分支；纯值有效性判断。
    static bool 中性引用有效(const L1中性事实引用& 引用) noexcept {
        return std::visit([](const auto& 值) noexcept { return 有效(值); }, 引用);
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static std::optional<L1中性写集请求> 规范化中性写集(
        const L1中性写集请求& 请求) {
        L1中性写集请求 结果 = 请求;
        std::sort(结果.节点.begin(), 结果.节点.end(), [](const auto& 左, const auto& 右) {
            return 左.本地键 < 右.本地键;
        });
        std::sort(结果.关系.begin(), 结果.关系.end(), [](const auto& 左, const auto& 右) {
            return 左.本地键 < 右.本地键;
        });
        std::sort(结果.值.begin(), 结果.值.end(), [](const auto& 左, const auto& 右) {
            return 左.本地键 < 右.本地键;
        });
        std::sort(结果.属性槽变更.begin(), 结果.属性槽变更.end(), [](const auto& 左,
            const auto& 右) {
            const auto 左所属 = 中性引用排序键(左.所属节点);
            const auto 右所属 = 中性引用排序键(右.所属节点);
            if (左所属 != 右所属) return 左所属 < 右所属;
            const auto 左类型 = 中性引用排序键(左.属性类型节点);
            const auto 右类型 = 中性引用排序键(右.属性类型节点);
            if (左类型 != 右类型) return 左类型 < 右类型;
            return 左.新当前值 < 右.新当前值;
        });
        std::sort(结果.退出事实.begin(), 结果.退出事实.end());
        return 结果;
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static bool 中性请求结构有效(const L1中性写集请求& 请求,
        std::unordered_set<std::uint32_t>& 本地键) {
        auto 登记 = [&](L1中性写集本地键 键) {
            return 有效(键) && 本地键.insert(键.值).second;
        };
        for (const auto& 项 : 请求.节点) {
            if (!登记(项.本地键)
                || ((项.种类 == 节点种类::属性类型) != 项.属性类型表示.has_value()))
                return false;
        }
        for (const auto& 项 : 请求.关系)
            if (!登记(项.本地键) || !中性引用有效(项.源节点)
                || !中性引用有效(项.目标节点) || !中性引用有效(项.关系类型节点))
                return false;
        for (const auto& 项 : 请求.值)
            if (!登记(项.本地键) || !中性引用有效(项.所属节点)
                || !中性引用有效(项.属性类型节点) || !中性引用有效(项.来源节点)
                || !L1中性原始材料完整(项.材料)) return false;
        std::unordered_set<std::uint64_t> 退出;
        for (const auto 编码 : 请求.退出事实)
            if (!有效(编码) || !退出.insert(编码.值).second) return false;
        for (std::size_t i = 0; i < 请求.属性槽变更.size(); ++i) {
            const auto& 项 = 请求.属性槽变更[i];
            if (!中性引用有效(项.所属节点) || !中性引用有效(项.属性类型节点)
                || !有效(项.新当前值)) return false;
            if (i != 0) {
                const auto& 前 = 请求.属性槽变更[i - 1];
                if (前.所属节点 == 项.所属节点 && 前.属性类型节点 == 项.属性类型节点)
                    return false;
            }
        }
        return true;
    }

    // 诊断责任：无适用错误分支；纯值枚举转换。
    static std::optional<值表示种类> 转换值表示种类(
        const std::optional<L1中性值表示种类>& 值) noexcept {
        if (!值) return std::nullopt;
        return static_cast<值表示种类>(static_cast<std::uint8_t>(*值));
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static std::optional<原始值材料> 转换原始材料(const L1中性原始值材料& 材料) {
        return std::visit([](const auto& 值) -> std::optional<原始值材料> {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, L1中性独立材料引用>)
                return 原始值材料{独立材料引用{值.编码}};
            else return 原始值材料{值};
        }, 材料);
    }

    // 诊断责任：无适用错误分支；纯值材料比较。
    static bool 中性材料等于(const 原始值材料& 实际,
        const L1中性原始值材料& 期望) noexcept {
        return std::visit([&](const auto& 值) noexcept -> bool {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, L1中性独立材料引用>) {
                const auto* 实际引用 = std::get_if<独立材料引用>(&实际);
                return 实际引用 && 实际引用->编码 == 值.编码;
            } else {
                const auto* 实际值 = std::get_if<类型>(&实际);
                return 实际值 && *实际值 == 值;
            }
        }, 期望);
    }

    // 诊断责任：无适用错误分支；只读候选并返回结构化真假。
    static bool 中性候选读回完整(const 状态& 值,
        const L1中性写集请求& 请求,
        const std::vector<std::pair<L1中性写集本地键, 稳定编码>>& 映射,
        std::uint64_t 发布代次) noexcept {
        const auto 查映射 = [&](L1中性写集本地键 键) -> std::optional<稳定编码> {
            for (const auto& [本地, 编码] : 映射) if (本地 == 键) return 编码;
            return std::nullopt;
        };
        const auto 解析 = [&](const L1中性事实引用& 引用) -> std::optional<稳定编码> {
            return std::visit([&](const auto& 项) -> std::optional<稳定编码> {
                using 类型 = std::decay_t<decltype(项)>;
                if constexpr (std::is_same_v<类型, 稳定编码>) return 项;
                else return 查映射(项);
            }, 引用);
        };
        for (const auto& 项 : 请求.节点) {
            const auto 编码 = 查映射(项.本地键);
            if (!编码) return false;
            const auto it = 值.当前节点.find(编码->值);
            if (it == 值.当前节点.end() || it->second.编码 != *编码
                || it->second.种类 != 项.种类
                || it->second.属性类型表示 != 转换值表示种类(项.属性类型表示)
                || it->second.创建事实代次 != 发布代次 || it->second.退出事实代次)
                return false;
        }
        for (const auto& 项 : 请求.关系) {
            const auto 编码 = 查映射(项.本地键);
            const auto 源 = 解析(项.源节点), 目标 = 解析(项.目标节点),
                类型 = 解析(项.关系类型节点);
            if (!编码 || !源 || !目标 || !类型) return false;
            const auto it = 值.当前关系.find(编码->值);
            if (it == 值.当前关系.end() || it->second.源节点 != *源
                || it->second.目标节点 != *目标 || it->second.关系类型节点 != *类型
                || it->second.角色或顺序 != 项.角色或顺序
                || it->second.创建事实代次 != 发布代次 || it->second.退出事实代次)
                return false;
        }
        for (const auto& 项 : 请求.值) {
            const auto 编码 = 查映射(项.本地键);
            const auto 所属 = 解析(项.所属节点), 类型 = 解析(项.属性类型节点),
                来源 = 解析(项.来源节点);
            if (!编码 || !所属 || !类型 || !来源) return false;
            const auto it = 值.当前值.find(编码->值);
            if (it == 值.当前值.end() || it->second.所属节点 != *所属
                || it->second.属性类型节点 != *类型 || it->second.来源节点 != *来源
                || !中性材料等于(it->second.材料, 项.材料)
                || it->second.创建事实代次 != 发布代次 || it->second.退出事实代次)
                return false;
        }
        for (const auto& 项 : 请求.属性槽变更) {
            const auto 所属 = 解析(项.所属节点), 类型 = 解析(项.属性类型节点),
                当前 = 查映射(项.新当前值);
            if (!所属 || !类型 || !当前) return false;
            const auto 节点 = 值.当前节点.find(所属->值);
            if (节点 == 值.当前节点.end()) return false;
            const auto 槽 = std::find_if(节点->second.当前属性.begin(),
                节点->second.当前属性.end(), [&](const 属性槽& 值项) {
                    return 值项.属性类型节点 == *类型;
                });
            if (槽 == 节点->second.当前属性.end() || 槽->当前值 != *当前) return false;
        }
        for (const auto 编码 : 请求.退出事实) {
            const auto it = 值.历史.find(编码.值);
            if (it == 值.历史.end()) return false;
            const bool 匹配 = std::visit([&](const auto& 事实) {
                return 事实.编码 == 编码 && 事实.退出事实代次 == 发布代次;
            }, it->second.事实);
            if (!匹配) return false;
        }
        return 值.事实代次 == 发布代次;
    }

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
    static bool 幂等记录完整(const L1幂等账记录& 账) {
        if (!(有效(账.幂等键) && 账.摘要合同版本 == L1幂等摘要合同版本
            && 完整(账.请求意图摘要) && 完整(账.首次执行证据摘要)
            && 账.首次发布事实代次 != 0
            && 账.首次完整读回.状态 == L1通用发布后读回状态::成功
            && 账.首次完整读回.发布事实代次 == 账.首次发布事实代次
            && !账.首次完整读回.项目组.empty() && 完整(账.确定性结果摘要)
            && 账.首次执行证据材料
            && L1执行证据材料基本完整(*账.首次执行证据材料, 账.首次规范化写集)
            && 账.首次执行证据材料->材料摘要 == 账.首次规范化写集.执行证据材料->材料摘要)) return false;
        L1写入结果 结果{L1写入状态::成功, 账.首次发布事实代次,
            账.首次新编码映射, 账.首次完整读回, 账.发布后失败见证,
            账.确定性结果摘要版本, 账.确定性结果摘要, 账.首次领域结果见证};
        if (账.确定性结果摘要版本 == L1确定性结果摘要版本)
            return !账.首次领域结果见证 && !完整(账.领域结果见证摘要)
                && 账.确定性结果摘要 == 形成L1确定性结果摘要(账.首次规范化写集, 结果);
        if (账.确定性结果摘要版本 != L1确定性结果摘要版本E01
            || !账.首次领域结果见证 || !账.首次规范化写集.领域结果见证计划
            || 账.领域结果见证摘要 != 形成L1领域结果见证摘要(*账.首次领域结果见证)) return false;
        const auto 重算见证 = 形成L1领域结果见证(*账.首次规范化写集.领域结果见证计划,
            账.首次规范化写集, 账.首次新编码映射, 账.首次发布事实代次);
        return 重算见证 && *重算见证 == *账.首次领域结果见证
            && 账.确定性结果摘要 == 形成L1确定性结果摘要(账.首次规范化写集,
                结果, 账.确定性结果摘要版本, 账.领域结果见证摘要);
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
            if (!插入(编码) || 关系.编码.值 != 编码 || 关系.创建事实代次 == 0
                || 关系.创建事实代次 > 值.事实代次 || 关系.退出事实代次
                || !值.当前节点.contains(关系.源节点.值)
                || !值.当前节点.contains(关系.目标节点.值)
                || !值.当前节点.contains(关系.关系类型节点.值)
                || !有效(关系.源节点) || !有效(关系.目标节点) || !有效(关系.关系类型节点)) return false;
        }
        std::unordered_set<std::uint64_t> 已源索引关系;
        for (const auto& [源编码, 类型组] : 值.当前源关系索引) {
            if (源编码 == 0 || 类型组.empty()) return false;
            for (const auto& [类型编码, 编码组] : 类型组) {
                if (类型编码 == 0 || 编码组.empty()) return false;
                std::uint64_t 前一编码 = 0;
                for (const auto 关系编码 : 编码组) {
                    const auto 关系 = 值.当前关系.find(关系编码);
                    if (关系编码 == 0 || 关系编码 <= 前一编码
                        || !已源索引关系.insert(关系编码).second
                        || 关系 == 值.当前关系.end()
                        || 关系->second.源节点.值 != 源编码
                        || 关系->second.关系类型节点.值 != 类型编码
                        || 关系->second.退出事实代次) return false;
                    前一编码 = 关系编码;
                }
            }
        }
        if (已源索引关系.size() != 值.当前关系.size()) return false;
        std::unordered_set<std::uint64_t> 已目标索引关系;
        for (const auto& [目标编码, 类型组] : 值.当前目标关系索引) {
            if (目标编码 == 0 || 类型组.empty()) return false;
            for (const auto& [类型编码, 编码组] : 类型组) {
                if (类型编码 == 0 || 编码组.empty()) return false;
                std::uint64_t 前一编码 = 0;
                for (const auto 关系编码 : 编码组) {
                    const auto 关系 = 值.当前关系.find(关系编码);
                    if (关系编码 == 0 || 关系编码 <= 前一编码
                        || !已目标索引关系.insert(关系编码).second
                        || 关系 == 值.当前关系.end()
                        || 关系->second.目标节点.值 != 目标编码
                        || 关系->second.关系类型节点.值 != 类型编码
                        || 关系->second.退出事实代次) return false;
                    前一编码 = 关系编码;
                }
            }
        }
        if (已目标索引关系.size() != 值.当前关系.size()) return false;
        for (const auto& [编码, 事实] : 值.当前值) {
            if (!插入(编码) || 事实.编码.值 != 编码 || 事实.创建事实代次 == 0
                || 事实.创建事实代次 > 值.事实代次 || 事实.退出事实代次
                || !有效(事实.所属节点) || !有效(事实.属性类型节点) || !有效(事实.来源节点)
                || !值.当前节点.contains(事实.所属节点.值) || !值.当前节点.contains(事实.来源节点.值)) return false;
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
        std::unordered_set<std::uint64_t> 节点全集;
        for (const auto& [编码, 事实] : 值.当前节点) 节点全集.insert(编码);
        for (const auto& [编码, 历史] : 值.历史) {
            const auto 历史编码 = std::visit([](const auto& 事实) { return 事实.编码.值; }, 历史.事实);
            if (编码 == 0 || 全部.contains(编码) || 历史编码 != 编码) return false;
            全部.insert(编码);
            bool 有效历史 = std::visit([&](const auto& 事实) {
                if (!事实.退出事实代次 || 事实.创建事实代次 == 0
                    || 事实.创建事实代次 > *事实.退出事实代次
                    || *事实.退出事实代次 > 值.事实代次 || 事实.编码.值 != 编码) return false;
                using T = std::decay_t<decltype(事实)>;
                if constexpr (std::is_same_v<T, 节点事实>) { 节点全集.insert(编码); return true; }
                else return true;
            }, 历史.事实);
            if (!有效历史) return false;
        }
        auto 节点存在 = [&](稳定编码 编码) { return 有效(编码) && 节点全集.contains(编码.值); };
        for (const auto& [_, 关系历史] : 值.历史) {
            if (const auto* 关系 = std::get_if<关系事实>(&关系历史.事实))
                if (!节点存在(关系->源节点) || !节点存在(关系->目标节点) || !节点存在(关系->关系类型节点)) return false;
            if (const auto* 事实 = std::get_if<值事实>(&关系历史.事实)) {
                if (!节点存在(事实->所属节点) || !节点存在(事实->来源节点) || !节点存在(事实->属性类型节点)) return false;
                const auto 类型 = 值.当前节点.find(事实->属性类型节点.值);
                if (类型 != 值.当前节点.end() && (类型->second.种类 != 节点种类::属性类型 || !类型->second.属性类型表示 || !表示匹配(*类型->second.属性类型表示, 事实->材料))) return false;
            }
        }
        for (const auto 编码 : 全部) if (!值.永久占用.contains(编码)) return false;
        if (值.永久占用.size() != 全部.size()) return false;
        for (const auto 编码 : 值.永久占用) if (编码 == 0 || !全部.contains(编码)) return false;
        std::uint64_t 最大编码 = 0;
        for (const auto 编码 : 值.永久占用) 最大编码 = std::max(最大编码, 编码);
        if (值.下个编码 == 0 || 值.下个编码 <= 最大编码) return false;
        for (const auto& [键, 账] : 值.幂等账) {
            if (值.中性幂等账.contains(键) || 键 == 0 || !幂等记录完整(账)
                || 账.幂等键.值 != 键 || 账.首次规范化写集.幂等键.值 != 键
                || 账.首次发布事实代次 > 值.事实代次
                || 规范化写集(账.首次规范化写集) != std::optional<L1写集请求>(账.首次规范化写集)) return false;
            std::unordered_set<std::uint32_t> 本地;
            if (!请求结构有效(账.首次规范化写集, 本地)) return false;
            std::unordered_map<std::uint32_t, bool> 新键;
            for (const auto& 项 : 账.首次规范化写集.节点) 新键[项.本地键.值] = true;
            for (const auto& 项 : 账.首次规范化写集.关系) 新键[项.本地键.值] = true;
            for (const auto& 项 : 账.首次规范化写集.值) 新键[项.本地键.值] = true;
            if (账.首次新编码映射.size() != 新键.size()) return false;
            for (const auto& [本地, 编码] : 账.首次新编码映射) {
                if (!新键.contains(本地.值) || !全部.contains(编码.值)) return false;
                bool 创建匹配 = false;
                if (const auto it = 值.当前节点.find(编码.值); it != 值.当前节点.end()) 创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.当前关系.find(编码.值); it != 值.当前关系.end()) 创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.当前值.find(编码.值); it != 值.当前值.end()) 创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.历史.find(编码.值); it != 值.历史.end()) 创建匹配 = std::visit([&](const auto& 事实) { return 事实.创建事实代次 == 账.首次发布事实代次; }, it->second.事实);
                if (!创建匹配) return false;
            }
            const auto 审计 = 值.审计.find(键);
            if (审计 == 值.审计.end()) return false;
            std::size_t 成功数 = 0;
            for (std::size_t i = 0; i < 审计->second.size(); ++i) {
                const auto& 记录 = 审计->second[i];
                if (记录.幂等键.值 != 键 || 记录.事件序号 != i + 1) return false;
                if (记录.事件 == L1审计事件::提交成功) {
                    ++成功数;
                    if (记录.结果状态 != L1写入状态::成功 || 记录.事实代次 != 账.首次发布事实代次 || 记录.新编码映射 != 账.首次新编码映射
                        || 记录.请求意图摘要 != 账.请求意图摘要 || 记录.首次执行证据摘要 != 账.首次执行证据摘要
                        || 记录.确定性结果摘要 != 账.确定性结果摘要
                        || 记录.首次执行证据材料 != 账.首次执行证据材料
                        || 记录.确定性结果摘要版本 != 账.确定性结果摘要版本
                        || 记录.领域结果见证摘要 != 账.领域结果见证摘要
                        || 记录.首次领域结果见证 != 账.首次领域结果见证) return false;
                }
            }
            if (成功数 != 1) return false;
            const auto 结果见证 = 值.领域结果见证状态组.find(键);
            if (账.首次领域结果见证) {
                if (结果见证 == 值.领域结果见证状态组.end()
                    || 结果见证->second != L1领域结果见证记录{账.幂等键,
                        账.首次发布事实代次, *账.首次领域结果见证,
                        账.领域结果见证摘要}) return false;
            } else if (结果见证 != 值.领域结果见证状态组.end()) return false;
        }
        for (const auto& [键, 账] : 值.中性幂等账) {
            if (键 == 0 || 值.幂等账.contains(键)
                || 账.首次规范化写集.合同版本 != L1中性CRUD合同版本
                || 账.首次规范化写集.幂等键.值 != 键
                || 账.首次发布事实代次 == 0 || 账.首次发布事实代次 > 值.事实代次
                || 规范化中性写集(账.首次规范化写集)
                    != std::optional<L1中性写集请求>(账.首次规范化写集)
                || 账.首次结果.状态 != L1中性写入状态::成功
                || 账.首次结果.合同版本 != L1中性CRUD合同版本
                || 账.首次结果.幂等键.值 != 键
                || 账.首次结果.事实代次 != 账.首次发布事实代次
                || !账.首次结果.是否形成内存权威发布
                || 账.首次结果.重试边界 != L1中性重试边界::不适用
                || 账.首次结果.新编码映射 != 账.首次新编码映射)
                return false;
            std::unordered_set<std::uint32_t> 本地;
            if (!中性请求结构有效(账.首次规范化写集, 本地)
                || 账.首次新编码映射.size() != 本地.size()) return false;
            for (std::size_t i = 0; i < 账.首次新编码映射.size(); ++i) {
                const auto& [本地键, 编码] = 账.首次新编码映射[i];
                if (!本地.contains(本地键.值) || !全部.contains(编码.值)
                    || (i != 0 && !(账.首次新编码映射[i - 1].first < 本地键))) return false;
                bool 创建匹配 = false;
                if (const auto it = 值.当前节点.find(编码.值); it != 值.当前节点.end())
                    创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.当前关系.find(编码.值); it != 值.当前关系.end())
                    创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.当前值.find(编码.值); it != 值.当前值.end())
                    创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.历史.find(编码.值); it != 值.历史.end())
                    创建匹配 = std::visit([&](const auto& 事实) {
                        return 事实.创建事实代次 == 账.首次发布事实代次;
                    }, it->second.事实);
                if (!创建匹配) return false;
            }
        }
        for (const auto& [键, 记录] : 值.领域结果见证状态组)
            if (键 == 0 || !值.幂等账.contains(键)
                || 记录.幂等键.值 != 键 || 记录.发布事实代次 == 0
                || 记录.结果见证摘要 != 形成L1领域结果见证摘要(记录.结果见证)) return false;
        for (const auto& [键, 记录组] : 值.审计) if (!值.幂等账.contains(键) || 记录组.empty()) return false;
        for (const auto& 见证 : 值.失败见证) {
            if (!有效(见证.身份.幂等键) || 见证.身份.审计事件序号 == 0
                || 见证.发布事实代次 == 0 || 见证.发布事实代次 > 值.事实代次) return false;
            const auto 账 = 值.幂等账.find(见证.身份.幂等键.值);
            const auto 审计 = 值.审计.find(见证.身份.幂等键.值);
            if (账 == 值.幂等账.end() || 审计 == 值.审计.end()
                || 账->second.发布后失败见证 != 见证.身份
                || 见证.身份.审计事件序号 > 审计->second.size()) return false;
            const auto& 记录 = 审计->second[见证.身份.审计事件序号 - 1];
            if (记录.事件 != L1审计事件::发布后读回失败隔离
                || !记录.失败见证 || *记录.失败见证 != 见证) return false;
        }
        if (值.当前隔离见证身份
            && std::none_of(值.失败见证.begin(), 值.失败见证.end(), [&](const auto& 见证) {
                return 见证.身份 == *值.当前隔离见证身份; })) return false;
        if (值.隔离 != 值.当前隔离见证身份.has_value()) return false;
        return true;
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
        输出.失败见证 = 材料.发布后读回失败见证组;
        输出.当前隔离见证身份 = 材料.当前隔离见证身份;
        输出.隔离 = 输出.当前隔离见证身份.has_value();
        for (const auto& 记录 : 材料.领域结果见证记录组)
            if (!有效(记录.幂等键)
                || !输出.领域结果见证状态组.emplace(记录.幂等键.值, 记录).second) return false;
        if (!派生当前源关系索引(输出)
            || !派生当前目标关系索引(输出)) return false;
        std::uint64_t 最大 = 0; for (const auto 编码 : 输出.永久占用) 最大 = std::max(最大, 编码); 输出.下个编码 = 最大 == (std::numeric_limits<std::uint64_t>::max)() ? 0 : 最大 + 1;
        return true;
    }
    static std::optional<L1完整快照> 构造快照(const 状态& 值) {
        L1完整快照 快照; 快照.事实代次 = 值.事实代次;
        for (const auto& [_, 事实] : 值.当前节点) 快照.当前节点.push_back(事实);
        for (const auto& [_, 事实] : 值.当前关系) 快照.当前关系.push_back(事实);
        for (const auto& [_, 事实] : 值.当前值) 快照.当前值.push_back(事实);
        for (const auto 编码 : 值.永久占用) 快照.永久占用编码.push_back({编码});
        排序(快照); return 快照;
    }
    template<class T> L1读取结果 读取当前(稳定编码 编码, const std::unordered_map<std::uint64_t, T>& 表) const {
        if (!有效(编码)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, 状态_.事实代次, std::nullopt};
        const auto it = 表.find(编码.值);
        if (it != 表.end()) return {L1读取状态::成功, 状态_.事实代次, L1事实副本{it->second}};
        return {状态_.历史.contains(编码.值) ? L1读取状态::已退出 : L1读取状态::未找到, 0, std::nullopt};
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
