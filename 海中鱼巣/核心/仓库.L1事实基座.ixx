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
#include <stdexcept>
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
    enum class 一致当前事实种类 : std::uint8_t {
        节点 = 1, 关系 = 2, 值 = 3
    };

    struct 一致具名事实内部结果项 final {
        稳定编码 查询编码;
        L1中性一致当前读取项目状态 状态 =
            L1中性一致当前读取项目状态::未找到;
        std::optional<L1事实副本> 事实;
    };

    struct 一致属性值内部投影 final {
        属性槽 属性槽值;
        值事实 当前值事实;
    };

    struct 一致属性值内部结果项 final {
        稳定编码 节点;
        稳定编码 属性类型;
        L1中性一致当前读取项目状态 状态 =
            L1中性一致当前读取项目状态::未找到;
        std::optional<一致属性值内部投影> 投影;
    };

    struct 一致关系对端内部投影 final {
        关系事实 关系;
        节点事实 对端节点;
    };

    struct 一致源关系组内部结果项 final {
        稳定编码 源节点;
        稳定编码 关系类型节点;
        std::vector<一致关系对端内部投影> 成员;
    };

    struct 一致目标关系组内部结果项 final {
        稳定编码 目标节点;
        稳定编码 关系类型节点;
        std::vector<一致关系对端内部投影> 成员;
    };

    struct 一致当前读取内部结果 final {
        L1中性一致当前读取状态 状态 =
            L1中性一致当前读取状态::入口拒绝;
        std::uint64_t 读取事实代次 = 0;
        std::vector<一致具名事实内部结果项> 节点;
        std::vector<一致具名事实内部结果项> 关系;
        std::vector<一致具名事实内部结果项> 值;
        std::vector<一致属性值内部结果项> 属性值;
        std::vector<一致源关系组内部结果项> 源关系组;
        std::vector<一致目标关系组内部结果项> 目标关系组;
    };

    L1事实基座仓库() = default;
    L1事实基座仓库(const L1事实基座仓库&) = delete;
    L1事实基座仓库& operator=(const L1事实基座仓库&) = delete;

    // 诊断责任：向上送出；许可、资源与局部结构矛盾均由中性结构化状态携带。
    一致当前读取内部结果 尝试读取一致当前内部投影(
        const L1中性一致当前读取请求& 请求) const {
        const auto 失败 = [](L1中性一致当前读取状态 状态,
            std::uint64_t 代次 = 0) {
            一致当前读取内部结果 结果;
            结果.状态 = 状态;
            结果.读取事实代次 = 代次;
            return 结果;
        };
        try {
            if (!一致当前请求有效(请求))
                return 失败(L1中性一致当前读取状态::入口拒绝);

            std::shared_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock())
                return 失败(L1中性一致当前读取状态::许可拒绝);
            if (状态_.隔离 || 状态_.事实代次 == 0)
                return 失败(L1中性一致当前读取状态::内部不一致);
            if (状态_.事实代次 != 请求.期望事实代次)
                return 失败(L1中性一致当前读取状态::事实代次漂移,
                    状态_.事实代次);

            一致当前读取内部结果 结果;
            结果.状态 = L1中性一致当前读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            结果.节点.reserve(请求.节点.size());
            结果.关系.reserve(请求.关系.size());
            结果.值.reserve(请求.值.size());
            结果.属性值.reserve(请求.属性值.size());
            结果.源关系组.reserve(请求.源关系组.size());
            结果.目标关系组.reserve(请求.目标关系组.size());

            for (const auto 编码 : 请求.节点) {
                auto 项 = 读取一致具名当前事实(状态_, 编码,
                    一致当前事实种类::节点);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.节点.push_back(std::move(*项));
            }
            for (const auto 编码 : 请求.关系) {
                auto 项 = 读取一致具名当前事实(状态_, 编码,
                    一致当前事实种类::关系);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.关系.push_back(std::move(*项));
            }
            for (const auto 编码 : 请求.值) {
                auto 项 = 读取一致具名当前事实(状态_, 编码,
                    一致当前事实种类::值);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.值.push_back(std::move(*项));
            }

            for (const auto& 选择 : 请求.属性值) {
                一致属性值内部结果项 项{选择.节点, 选择.属性类型,
                    L1中性一致当前读取项目状态::未找到, std::nullopt};
                const auto 节点项 = 读取一致具名当前事实(状态_, 选择.节点,
                    一致当前事实种类::节点);
                if (!节点项)
                    return 失败(L1中性一致当前读取状态::内部不一致);
                if (节点项->状态 != L1中性一致当前读取项目状态::成功) {
                    项.状态 = 节点项->状态;
                    结果.属性值.push_back(std::move(项));
                    continue;
                }
                const auto* 节点 = 节点项->事实
                    ? std::get_if<节点事实>(&*节点项->事实) : nullptr;
                if (!节点)
                    return 失败(L1中性一致当前读取状态::内部不一致);
                const auto 槽 = std::lower_bound(节点->当前属性.begin(),
                    节点->当前属性.end(), 选择.属性类型,
                    [](const 属性槽& 左, 稳定编码 右) {
                        return 左.属性类型节点 < 右;
                    });
                if (槽 == 节点->当前属性.end()
                    || 槽->属性类型节点 != 选择.属性类型) {
                    项.状态 = L1中性一致当前读取项目状态::属性未设置;
                    结果.属性值.push_back(std::move(项));
                    continue;
                }
                const auto 当前值 = 状态_.当前值.find(槽->当前值.值);
                if (当前值 == 状态_.当前值.end()
                    || !一致当前值局部完整(状态_, 当前值->first, 当前值->second)
                    || 当前值->second.所属节点 != 选择.节点
                    || 当前值->second.属性类型节点 != 选择.属性类型)
                    return 失败(L1中性一致当前读取状态::内部不一致);
                项.状态 = L1中性一致当前读取项目状态::成功;
                项.投影 = 一致属性值内部投影{*槽, 当前值->second};
                结果.属性值.push_back(std::move(项));
            }

            for (const auto& 选择 : 请求.源关系组) {
                auto 项 = 读取一致源关系组(状态_, 选择);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.源关系组.push_back(std::move(*项));
            }
            for (const auto& 选择 : 请求.目标关系组) {
                auto 项 = 读取一致目标关系组(状态_, 选择);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.目标关系组.push_back(std::move(*项));
            }

            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1中性一致当前读取状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L1中性一致当前读取状态::资源失败);
        } catch (...) {
            return 失败(L1中性一致当前读取状态::内部不一致);
        }
    }

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
                    || !插入当前目标关系索引(候选, 事实)
                    || !插入历史源关系候选索引(候选, 事实)
                    || !插入历史目标关系候选索引(候选, 事实))
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

    // 诊断责任：向上送出；索引或权威事实矛盾由中性结构化状态携带。
    L1中性历史关系组读取结果 读取中性历史关系组(
        const L1中性历史关系组读取请求& 请求) const {
        const auto 失败 = [&](L1中性历史关系组读取状态 状态,
            std::uint64_t 读取代次 = 0) {
            return L1中性历史关系组读取结果{状态, L1中性CRUD合同版本,
                请求.方向, 请求.端点节点, 请求.关系类型节点,
                请求.历史截止事实代次, 读取代次, {}};
        };
        const bool 方向有效 = 请求.方向 == L1中性关系端点方向::源
            || 请求.方向 == L1中性关系端点方向::目标;
        if (请求.合同版本 != L1中性CRUD合同版本 || !方向有效
            || !有效(请求.端点节点) || !有效(请求.关系类型节点)
            || 请求.历史截止事实代次 == 0)
            return 失败(L1中性历史关系组读取状态::入口拒绝);

        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离)
            return 失败(L1中性历史关系组读取状态::内部不一致);
        if (状态_.事实代次 < 请求.历史截止事实代次)
            return 失败(L1中性历史关系组读取状态::事实代次漂移,
                状态_.事实代次);

        const auto& 端点索引 = 请求.方向 == L1中性关系端点方向::源
            ? 状态_.历史源关系候选索引 : 状态_.历史目标关系候选索引;
        const auto 端点 = 端点索引.find(请求.端点节点.值);
        if (端点 == 端点索引.end())
            return 失败(L1中性历史关系组读取状态::成功, 状态_.事实代次);
        const auto 类型 = 端点->second.find(请求.关系类型节点.值);
        if (类型 == 端点->second.end())
            return 失败(L1中性历史关系组读取状态::成功, 状态_.事实代次);
        if (类型->second.empty())
            return 失败(L1中性历史关系组读取状态::内部不一致);

        L1中性历史关系组读取结果 结果{
            L1中性历史关系组读取状态::成功, L1中性CRUD合同版本,
            请求.方向, 请求.端点节点, 请求.关系类型节点,
            请求.历史截止事实代次, 状态_.事实代次, {}};
        结果.关系组.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 当前 = 状态_.当前关系.find(编码);
            const auto 历史 = 状态_.历史.find(编码);
            const bool 当前命中 = 当前 != 状态_.当前关系.end();
            const bool 历史命中 = 历史 != 状态_.历史.end();
            const auto* 历史关系 = 历史命中
                ? std::get_if<关系事实>(&历史->second.事实) : nullptr;
            const 关系事实* 事实 = 当前命中 ? &当前->second : 历史关系;
            if (编码 == 0 || 编码 <= 前一编码 || 当前命中 == 历史命中
                || !事实 || 事实->编码.值 != 编码
                || 事实->关系类型节点 != 请求.关系类型节点
                || (请求.方向 == L1中性关系端点方向::源
                    ? 事实->源节点 != 请求.端点节点
                    : 事实->目标节点 != 请求.端点节点)
                || 事实->创建事实代次 == 0
                || 事实->创建事实代次 > 状态_.事实代次
                || (当前命中 && 事实->退出事实代次)
                || (历史命中 && (!事实->退出事实代次
                    || 事实->创建事实代次 > *事实->退出事实代次
                    || *事实->退出事实代次 > 状态_.事实代次)))
                return 失败(L1中性历史关系组读取状态::内部不一致);

            if (事实->创建事实代次 <= 请求.历史截止事实代次
                && (!事实->退出事实代次
                    || 请求.历史截止事实代次 < *事实->退出事实代次))
                结果.关系组.push_back({事实->编码, 事实->源节点,
                    事实->目标节点, 事实->关系类型节点, 事实->角色或顺序,
                    事实->创建事实代次, 事实->退出事实代次});
            前一编码 = 编码;
        }
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
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            历史源关系候选索引;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            历史目标关系候选索引;
        std::unordered_map<std::uint64_t, 值事实> 当前值;
        std::unordered_map<std::uint64_t, L1历史事实副本> 历史;
        std::unordered_set<std::uint64_t> 永久占用;
        std::unordered_map<std::uint64_t, 中性幂等记录> 中性幂等账;
        bool 隔离 = false;
    };

    // 诊断责任：向上送出；临时唯一性集合分配异常由公开仓库入口映射。
    static bool 一致当前请求有效(const L1中性一致当前读取请求& 请求) {
        if (请求.合同版本 != L1中性一致当前读取合同版本
            || 请求.期望事实代次 == 0
            || (请求.节点.empty() && 请求.关系.empty() && 请求.值.empty()
                && 请求.属性值.empty() && 请求.源关系组.empty()
                && 请求.目标关系组.empty())) return false;

        std::unordered_set<std::uint64_t> 具名编码;
        const auto 登记编码 = [&](稳定编码 编码) {
            return 有效(编码) && 具名编码.insert(编码.值).second;
        };
        for (const auto 编码 : 请求.节点) if (!登记编码(编码)) return false;
        for (const auto 编码 : 请求.关系) if (!登记编码(编码)) return false;
        for (const auto 编码 : 请求.值) if (!登记编码(编码)) return false;

        std::vector<std::pair<std::uint64_t, std::uint64_t>> 属性键;
        std::vector<std::pair<std::uint64_t, std::uint64_t>> 源关系键;
        std::vector<std::pair<std::uint64_t, std::uint64_t>> 目标关系键;
        属性键.reserve(请求.属性值.size());
        源关系键.reserve(请求.源关系组.size());
        目标关系键.reserve(请求.目标关系组.size());
        const auto 登记组合 = [](auto& 组, 稳定编码 左, 稳定编码 右) {
            if (!有效(左) || !有效(右)) return false;
            const auto 键 = std::pair{左.值, 右.值};
            if (std::find(组.begin(), 组.end(), 键) != 组.end()) return false;
            组.push_back(键);
            return true;
        };
        for (const auto& 项 : 请求.属性值)
            if (!登记组合(属性键, 项.节点, 项.属性类型)) return false;
        for (const auto& 项 : 请求.源关系组)
            if (!登记组合(源关系键, 项.源节点, 项.关系类型节点)) return false;
        for (const auto& 项 : 请求.目标关系组)
            if (!登记组合(目标关系键, 项.目标节点, 项.关系类型节点)) return false;
        return true;
    }

    // 诊断责任：无适用错误分支；只检查一个编码在当前或历史账中的唯一占用。
    static bool 一致编码唯一当前(const 状态& 值, std::uint64_t 编码,
        一致当前事实种类 种类) noexcept {
        const std::size_t 当前数量 = (值.当前节点.contains(编码) ? 1U : 0U)
            + (值.当前关系.contains(编码) ? 1U : 0U)
            + (值.当前值.contains(编码) ? 1U : 0U);
        const bool 期望存在 = 种类 == 一致当前事实种类::节点
            ? 值.当前节点.contains(编码)
            : 种类 == 一致当前事实种类::关系
                ? 值.当前关系.contains(编码) : 值.当前值.contains(编码);
        return 当前数量 == 1 && 期望存在 && !值.历史.contains(编码);
    }

    // 诊断责任：无适用错误分支；只核对当前节点自身生命周期与结构种类。
    static bool 一致当前节点基本完整(const 状态& 值, std::uint64_t 键,
        const 节点事实& 事实) noexcept {
        const bool 种类和表示有效 = 事实.种类 == 节点种类::普通
            ? !事实.属性类型表示
            : 事实.种类 == 节点种类::属性类型 && 事实.属性类型表示
                && (*事实.属性类型表示 == 值表示种类::I64
                    || *事实.属性类型表示 == 值表示种类::I64组
                    || *事实.属性类型表示 == 值表示种类::U64组
                    || *事实.属性类型表示 == 值表示种类::独立材料引用);
        return 键 != 0 && 事实.编码.值 == 键 && 有效(事实.编码)
            && 事实.创建事实代次 != 0 && 事实.创建事实代次 <= 值.事实代次
            && !事实.退出事实代次 && 种类和表示有效;
    }

    // 诊断责任：无适用错误分支；只核对具名值及其直接节点和类型引用。
    static bool 一致当前值局部完整(const 状态& 值, std::uint64_t 键,
        const 值事实& 事实) noexcept {
        if (!一致编码唯一当前(值, 键, 一致当前事实种类::值)
            || 事实.编码.值 != 键 || 事实.创建事实代次 == 0
            || 事实.创建事实代次 > 值.事实代次 || 事实.退出事实代次
            || !有效(事实.所属节点) || !有效(事实.属性类型节点)
            || !有效(事实.来源节点)) return false;
        const auto 所属 = 值.当前节点.find(事实.所属节点.值);
        const auto 类型 = 值.当前节点.find(事实.属性类型节点.值);
        const auto 来源 = 值.当前节点.find(事实.来源节点.值);
        if (所属 == 值.当前节点.end() || 类型 == 值.当前节点.end()
            || 来源 == 值.当前节点.end()
            || !一致编码唯一当前(值, 所属->first, 一致当前事实种类::节点)
            || !一致编码唯一当前(值, 类型->first, 一致当前事实种类::节点)
            || !一致编码唯一当前(值, 来源->first, 一致当前事实种类::节点)
            || !一致当前节点基本完整(值, 所属->first, 所属->second)
            || !一致当前节点基本完整(值, 类型->first, 类型->second)
            || !一致当前节点基本完整(值, 来源->first, 来源->second)
            || 类型->second.种类 != 节点种类::属性类型
            || !类型->second.属性类型表示
            || !表示匹配(*类型->second.属性类型表示, 事实.材料)) return false;
        if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料)) {
            const auto 材料节点 = 值.当前节点.find(引用->编码.值);
            if (!有效(引用->编码) || 材料节点 == 值.当前节点.end()
                || !一致编码唯一当前(值, 材料节点->first,
                    一致当前事实种类::节点)
                || !一致当前节点基本完整(值, 材料节点->first,
                    材料节点->second)) return false;
        }
        return true;
    }

    // 诊断责任：无适用错误分支；只核对具名节点和其直接属性槽闭包。
    static bool 一致当前节点局部完整(const 状态& 值, std::uint64_t 键,
        const 节点事实& 事实) noexcept {
        if (!一致编码唯一当前(值, 键, 一致当前事实种类::节点)
            || !一致当前节点基本完整(值, 键, 事实)) return false;
        std::uint64_t 前一属性类型 = 0;
        for (const auto& 槽 : 事实.当前属性) {
            if (!有效(槽.属性类型节点) || !有效(槽.当前值)
                || 槽.属性类型节点.值 <= 前一属性类型) return false;
            const auto 类型 = 值.当前节点.find(槽.属性类型节点.值);
            const auto 当前值 = 值.当前值.find(槽.当前值.值);
            if (类型 == 值.当前节点.end() || 当前值 == 值.当前值.end()
                || 类型->second.种类 != 节点种类::属性类型
                || !类型->second.属性类型表示
                || !一致当前节点基本完整(值, 类型->first, 类型->second)
                || !一致编码唯一当前(值, 类型->first,
                    一致当前事实种类::节点)
                || !一致当前值局部完整(值, 当前值->first, 当前值->second)
                || 当前值->second.所属节点 != 事实.编码
                || 当前值->second.属性类型节点 != 槽.属性类型节点
                || !表示匹配(*类型->second.属性类型表示,
                    当前值->second.材料)) return false;
            前一属性类型 = 槽.属性类型节点.值;
        }
        return true;
    }

    // 诊断责任：无适用错误分支；只核对具名关系及直接端点和类型节点。
    static bool 一致当前关系局部完整(const 状态& 值, std::uint64_t 键,
        const 关系事实& 事实) noexcept {
        if (!一致编码唯一当前(值, 键, 一致当前事实种类::关系)
            || 事实.编码.值 != 键 || 事实.创建事实代次 == 0
            || 事实.创建事实代次 > 值.事实代次 || 事实.退出事实代次
            || !有效(事实.源节点) || !有效(事实.目标节点)
            || !有效(事实.关系类型节点)) return false;
        const auto 源 = 值.当前节点.find(事实.源节点.值);
        const auto 目标 = 值.当前节点.find(事实.目标节点.值);
        const auto 类型 = 值.当前节点.find(事实.关系类型节点.值);
        return 源 != 值.当前节点.end() && 目标 != 值.当前节点.end()
            && 类型 != 值.当前节点.end()
            && 一致编码唯一当前(值, 源->first, 一致当前事实种类::节点)
            && 一致编码唯一当前(值, 目标->first, 一致当前事实种类::节点)
            && 一致编码唯一当前(值, 类型->first, 一致当前事实种类::节点)
            && 一致当前节点基本完整(值, 源->first, 源->second)
            && 一致当前节点基本完整(值, 目标->first, 目标->second)
            && 一致当前节点基本完整(值, 类型->first, 类型->second)
            && 类型->second.种类 == 节点种类::普通
            && !类型->second.属性类型表示;
    }

    // 诊断责任：无适用错误分支；只按单编码历史记录判定退出或种类不匹配。
    static std::optional<L1中性一致当前读取项目状态> 一致当前缺失状态(
        const 状态& 值, 稳定编码 编码, 一致当前事实种类 期望种类) noexcept {
        const std::size_t 当前数量 = (值.当前节点.contains(编码.值) ? 1U : 0U)
            + (值.当前关系.contains(编码.值) ? 1U : 0U)
            + (值.当前值.contains(编码.值) ? 1U : 0U);
        const auto 历史 = 值.历史.find(编码.值);
        if (当前数量 != 0) {
            if (当前数量 != 1 || 历史 != 值.历史.end()) return std::nullopt;
            return L1中性一致当前读取项目状态::种类不匹配;
        }
        if (历史 == 值.历史.end())
            return L1中性一致当前读取项目状态::未找到;
        if (历史->second.当前有效 || 历史->second.查询编码 != 编码)
            return std::nullopt;
        一致当前事实种类 历史种类 = 一致当前事实种类::节点;
        const bool 历史完整 = std::visit([&](const auto& 事实) {
            using 类型 = std::decay_t<decltype(事实)>;
            if constexpr (std::is_same_v<类型, 节点事实>)
                历史种类 = 一致当前事实种类::节点;
            else if constexpr (std::is_same_v<类型, 关系事实>)
                历史种类 = 一致当前事实种类::关系;
            else 历史种类 = 一致当前事实种类::值;
            return 事实.编码 == 编码 && 事实.创建事实代次 != 0
                && 事实.退出事实代次
                && 事实.创建事实代次 <= *事实.退出事实代次
                && *事实.退出事实代次 <= 值.事实代次;
        }, 历史->second.事实);
        if (!历史完整) return std::nullopt;
        return 历史种类 == 期望种类
            ? L1中性一致当前读取项目状态::已退出
            : L1中性一致当前读取项目状态::种类不匹配;
    }

    // 诊断责任：向上送出；事实副本分配异常由公开仓库入口映射。
    static std::optional<一致具名事实内部结果项> 读取一致具名当前事实(
        const 状态& 值, 稳定编码 编码, 一致当前事实种类 种类) {
        一致具名事实内部结果项 结果{编码,
            L1中性一致当前读取项目状态::未找到, std::nullopt};
        if (种类 == 一致当前事实种类::节点) {
            const auto it = 值.当前节点.find(编码.值);
            if (it != 值.当前节点.end()) {
                if (!一致当前节点局部完整(值, it->first, it->second))
                    return std::nullopt;
                结果.状态 = L1中性一致当前读取项目状态::成功;
                结果.事实 = L1事实副本{it->second};
                return 结果;
            }
        } else if (种类 == 一致当前事实种类::关系) {
            const auto it = 值.当前关系.find(编码.值);
            if (it != 值.当前关系.end()) {
                if (!一致当前关系局部完整(值, it->first, it->second))
                    return std::nullopt;
                结果.状态 = L1中性一致当前读取项目状态::成功;
                结果.事实 = L1事实副本{it->second};
                return 结果;
            }
        } else {
            const auto it = 值.当前值.find(编码.值);
            if (it != 值.当前值.end()) {
                if (!一致当前值局部完整(值, it->first, it->second))
                    return std::nullopt;
                结果.状态 = L1中性一致当前读取项目状态::成功;
                结果.事实 = L1事实副本{it->second};
                return 结果;
            }
        }
        const auto 状态 = 一致当前缺失状态(值, 编码, 种类);
        if (!状态) return std::nullopt;
        结果.状态 = *状态;
        return 结果;
    }

    // 诊断责任：向上送出；关系与对端副本分配异常由公开仓库入口映射。
    static std::optional<一致源关系组内部结果项> 读取一致源关系组(
        const 状态& 值, const L1中性一致源关系组选择项& 选择) {
        一致源关系组内部结果项 结果{选择.源节点, 选择.关系类型节点, {}};
        const auto 源 = 值.当前源关系索引.find(选择.源节点.值);
        if (源 == 值.当前源关系索引.end()) return 结果;
        const auto 类型 = 源->second.find(选择.关系类型节点.值);
        if (类型 == 源->second.end()) return 结果;
        if (类型->second.empty()) return std::nullopt;
        结果.成员.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 值.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 值.当前关系.end()
                || !一致当前关系局部完整(值, 编码, 关系->second)
                || 关系->second.源节点 != 选择.源节点
                || 关系->second.关系类型节点 != 选择.关系类型节点)
                return std::nullopt;
            const auto 对端 = 值.当前节点.find(关系->second.目标节点.值);
            if (对端 == 值.当前节点.end()
                || !一致当前节点局部完整(值, 对端->first, 对端->second))
                return std::nullopt;
            结果.成员.push_back({关系->second, 对端->second});
            前一编码 = 编码;
        }
        return 结果;
    }

    // 诊断责任：向上送出；关系与对端副本分配异常由公开仓库入口映射。
    static std::optional<一致目标关系组内部结果项> 读取一致目标关系组(
        const 状态& 值, const L1中性一致目标关系组选择项& 选择) {
        一致目标关系组内部结果项 结果{选择.目标节点, 选择.关系类型节点, {}};
        const auto 目标 = 值.当前目标关系索引.find(选择.目标节点.值);
        if (目标 == 值.当前目标关系索引.end()) return 结果;
        const auto 类型 = 目标->second.find(选择.关系类型节点.值);
        if (类型 == 目标->second.end()) return 结果;
        if (类型->second.empty()) return std::nullopt;
        结果.成员.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 值.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 值.当前关系.end()
                || !一致当前关系局部完整(值, 编码, 关系->second)
                || 关系->second.目标节点 != 选择.目标节点
                || 关系->second.关系类型节点 != 选择.关系类型节点)
                return std::nullopt;
            const auto 对端 = 值.当前节点.find(关系->second.源节点.值);
            if (对端 == 值.当前节点.end()
                || !一致当前节点局部完整(值, 对端->first, 对端->second))
                return std::nullopt;
            结果.成员.push_back({关系->second, 对端->second});
            前一编码 = 编码;
        }
        return 结果;
    }

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

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入历史源关系候选索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.源节点)
            || !有效(事实.关系类型节点) || 事实.创建事实代次 == 0) return false;
        auto& 编码组 = 值.历史源关系候选索引[事实.源节点.值]
            [事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入历史目标关系候选索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.目标节点)
            || !有效(事实.关系类型节点) || 事实.创建事实代次 == 0) return false;
        auto& 编码组 = 值.历史目标关系候选索引[事实.目标节点.值]
            [事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
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
        const auto 历史关系索引完整 = [&](const auto& 索引, bool 源端) {
            std::unordered_set<std::uint64_t> 已索引关系;
            for (const auto& [端点编码, 类型组] : 索引) {
                if (端点编码 == 0 || 类型组.empty()) return false;
                for (const auto& [类型编码, 编码组] : 类型组) {
                    if (类型编码 == 0 || 编码组.empty()) return false;
                    std::uint64_t 前一编码 = 0;
                    for (const auto 关系编码 : 编码组) {
                        const auto 当前 = 值.当前关系.find(关系编码);
                        const auto 历史 = 值.历史.find(关系编码);
                        const bool 当前命中 = 当前 != 值.当前关系.end();
                        const bool 历史命中 = 历史 != 值.历史.end();
                        const auto* 历史关系 = 历史命中
                            ? std::get_if<关系事实>(&历史->second.事实) : nullptr;
                        const 关系事实* 关系 = 当前命中 ? &当前->second : 历史关系;
                        if (关系编码 == 0 || 关系编码 <= 前一编码
                            || !已索引关系.insert(关系编码).second
                            || 当前命中 == 历史命中 || !关系
                            || 关系->编码.值 != 关系编码
                            || (源端 ? 关系->源节点.值 : 关系->目标节点.值)
                                != 端点编码
                            || 关系->关系类型节点.值 != 类型编码
                            || 关系->创建事实代次 == 0
                            || 关系->创建事实代次 > 值.事实代次
                            || (当前命中 && 关系->退出事实代次)
                            || (历史命中 && (!关系->退出事实代次
                                || 关系->创建事实代次 > *关系->退出事实代次
                                || *关系->退出事实代次 > 值.事实代次))) return false;
                        前一编码 = 关系编码;
                    }
                }
            }
            std::size_t 权威关系数 = 值.当前关系.size();
            for (const auto& [_, 历史] : 值.历史)
                if (std::holds_alternative<关系事实>(历史.事实)) ++权威关系数;
            return 已索引关系.size() == 权威关系数;
        };
        if (!历史关系索引完整(值.历史源关系候选索引, true)
            || !历史关系索引完整(值.历史目标关系候选索引, false)) return false;
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
        for (const auto& [键, 账] : 值.中性幂等账) {
            if (键 == 0 || 账.首次规范化写集.合同版本 != L1中性CRUD合同版本
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
    mutable std::shared_mutex 锁_;
    状态 状态_;
};

} // namespace 海中鱼巣
