#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "仓库.L1事实基座.h"

#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <fstream>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <shared_mutex>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

namespace 海中鱼巣 {
namespace {

template<class T>
bool 严格升序且非零(const std::vector<T>& 组) noexcept {
    for (std::size_t i = 0; i < 组.size(); ++i) {
        if (!有效(组[i]) || (i != 0 && !(组[i - 1] < 组[i]))) return false;
    }
    return true;
}

std::optional<原始值材料> 转换材料(const L1所有者范围原始值材料& 材料) {
    return std::visit([](const auto& 值) -> std::optional<原始值材料> {
        using T = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<T, L1所有者范围独立材料引用>)
            return 原始值材料{独立材料引用{值.编码}};
        else return 原始值材料{值};
    }, 材料);
}

std::optional<L1所有者范围值表示种类> 转换表示(值表示种类 表示) noexcept {
    switch (表示) {
    case 值表示种类::I64: return L1所有者范围值表示种类::I64;
    case 值表示种类::I64组: return L1所有者范围值表示种类::I64组;
    case 值表示种类::U64组: return L1所有者范围值表示种类::U64组;
    case 值表示种类::独立材料引用: return L1所有者范围值表示种类::独立材料引用;
    }
    return std::nullopt;
}

std::optional<L1中性值表示种类> 转换中性表示(值表示种类 表示) noexcept {
    switch (表示) {
    case 值表示种类::I64: return L1中性值表示种类::I64;
    case 值表示种类::I64组: return L1中性值表示种类::I64组;
    case 值表示种类::U64组: return L1中性值表示种类::U64组;
    case 值表示种类::独立材料引用: return L1中性值表示种类::独立材料引用;
    }
    return std::nullopt;
}

L1所有者范围节点事实 转换节点(const 节点事实& 事实) {
    L1所有者范围节点事实 结果;
    结果.编码 = 事实.编码;
    结果.种类 = 事实.种类;
    if (事实.属性类型表示) 结果.属性类型表示 = 转换表示(*事实.属性类型表示);
    结果.写入所有者 = 事实.写入所有者;
    for (const auto& 槽 : 事实.当前属性)
        结果.当前属性.push_back({槽.属性类型节点, 槽.当前值});
    return 结果;
}

L1所有者范围关系事实 转换关系(const 关系事实& 事实) noexcept {
    return {事实.编码, 事实.源节点, 事实.目标节点, 事实.关系类型节点,
        事实.角色或顺序, 事实.写入所有者};
}

L1所有者范围值事实 转换值(const 值事实& 事实) {
    L1所有者范围原始值材料 材料 = std::visit([](const auto& 值)
        -> L1所有者范围原始值材料 {
        using T = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<T, 独立材料引用>)
            return L1所有者范围独立材料引用{值.编码};
        else return 值;
    }, 事实.材料);
    return {事实.编码, 事实.所属节点, 事实.属性类型节点,
        std::move(材料), 事实.来源节点, 事实.写入所有者};
}

L1中性节点事实 转换中性节点(const 节点事实& 事实) {
    L1中性节点事实 结果;
    结果.编码 = 事实.编码;
    结果.种类 = 事实.种类;
    if (事实.属性类型表示) 结果.属性类型表示 = 转换中性表示(*事实.属性类型表示);
    for (const auto& 槽 : 事实.当前属性)
        结果.当前属性.push_back({槽.属性类型节点, 槽.当前值});
    return 结果;
}

L1中性关系事实 转换中性关系(const 关系事实& 事实) noexcept {
    return {事实.编码, 事实.源节点, 事实.目标节点,
        事实.关系类型节点, 事实.角色或顺序};
}

L1中性值事实 转换中性值(const 值事实& 事实) {
    L1中性原始值材料 材料 = std::visit([](const auto& 值)
        -> L1中性原始值材料 {
        using T = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<T, 独立材料引用>)
            return L1中性独立材料引用{值.编码};
        else return 值;
    }, 事实.材料);
    return {事实.编码, 事实.所属节点, 事实.属性类型节点,
        std::move(材料), 事实.来源节点};
}

bool 表示匹配(值表示种类 表示, const 原始值材料& 材料) noexcept {
    switch (表示) {
    case 值表示种类::I64: return std::holds_alternative<std::int64_t>(材料);
    case 值表示种类::I64组: return std::holds_alternative<std::vector<std::int64_t>>(材料);
    case 值表示种类::U64组: return std::holds_alternative<std::vector<std::uint64_t>>(材料);
    case 值表示种类::独立材料引用: return std::holds_alternative<独立材料引用>(材料);
    }
    return false;
}

template<class T>
void 按编码排序(std::vector<T>& 组) {
    std::sort(组.begin(), 组.end(), [](const T& 左, const T& 右) {
        return 左.编码 < 右.编码;
    });
}

} // namespace

struct L1事实基座仓库::实现 final {
    struct 所有者建立记录 final {
        L1所有者范围建立请求 请求;
        L1所有者范围建立结果 结果;
    };
    struct 中性写入记录 final {
        L1中性写集请求 请求;
        L1中性写入结果 结果;
    };
    struct 所有者写入记录 final {
        L1所有者范围写集请求 请求;
        L1所有者范围写入结果 结果;
    };
    struct 跨所有者记录 final {
        L1跨所有者原子事务请求 请求;
        L1跨所有者原子事务结果 结果;
    };
    struct 三分区记录 final {
        L1三分区原子事务请求 请求;
        L1三分区原子事务结果 结果;
    };
    struct 有限N记录 final {
        L1有限N分区原子事务请求 请求;
        L1有限N分区原子事务结果 结果;
    };
    struct 状态 final {
        std::uint64_t 下个编码 = 1;
        std::unordered_map<std::uint64_t, L1结构所有者事实> 所有者;
        std::unordered_map<std::uint64_t, 节点事实> 节点;
        std::unordered_map<std::uint64_t, 关系事实> 关系;
        std::unordered_map<std::uint64_t, 值事实> 值;
        std::unordered_map<std::uint64_t, 所有者建立记录> 所有者建立账;
        std::unordered_map<std::uint64_t, 中性写入记录> 中性写入账;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, 所有者写入记录>> 所有者写入账;
        std::unordered_map<std::uint64_t, 跨所有者记录> 跨所有者账;
        std::unordered_map<std::uint64_t, 三分区记录> 三分区账;
        std::unordered_map<std::uint64_t, 有限N记录> 有限N账;
        std::optional<L1结构所有者身份> 旧共享所有者;
        bool 隔离 = false;
    };
    struct 持久会话 final {
        std::filesystem::path 根;
        HANDLE 锁文件 = INVALID_HANDLE_VALUE;
        std::uint64_t 快照序号 = 0;
        std::uint8_t 活动槽 = 0;
        bool 已毒化 = false;
        ~持久会话() {
            if (锁文件 != INVALID_HANDLE_VALUE) CloseHandle(锁文件);
        }
    };

    mutable std::shared_mutex 锁;
    状态 当前;
    std::unique_ptr<持久会话> 持久;

    static std::optional<稳定编码> 分配(状态& 值) noexcept {
        if (值.下个编码 == 0 || 值.下个编码 == (std::numeric_limits<std::uint64_t>::max)())
            return std::nullopt;
        return 稳定编码{值.下个编码++};
    }

    static bool 当前所有者有效(const 状态& 值, L1结构所有者身份 所有者) {
        const auto it = 值.所有者.find(所有者.编码.值);
        return 有效(所有者) && it != 值.所有者.end()
            && it->second.所有者 == 所有者;
    }

    static bool 状态完整(const 状态& 值) {
        if (值.下个编码 == 0) return false;
        std::unordered_set<std::uint64_t> 编码组;
        std::uint64_t 最大编码 = 0;
        const auto 占用 = [&](std::uint64_t 编码) {
            if (编码 == 0 || !编码组.insert(编码).second) return false;
            最大编码 = (std::max)(最大编码, 编码);
            return true;
        };
        for (const auto& [编码, 所有者] : 值.所有者) {
            if (!占用(编码) || 所有者.所有者.编码.值 != 编码) return false;
        }
        for (const auto& [编码, 节点] : 值.节点) {
            if (!占用(编码) || 节点.编码.值 != 编码
                || !当前所有者有效(值, 节点.写入所有者)
                || !属性排序唯一(节点.当前属性)
                || ((节点.种类 == 节点种类::属性类型)
                    != 节点.属性类型表示.has_value())) return false;
        }
        for (const auto& [编码, 关系] : 值.关系) {
            if (!占用(编码) || 关系.编码.值 != 编码
                || !当前所有者有效(值, 关系.写入所有者)
                || !值.节点.contains(关系.源节点.值)
                || !值.节点.contains(关系.目标节点.值)
                || !值.节点.contains(关系.关系类型节点.值)
                || 值.节点.at(关系.关系类型节点.值).写入所有者
                    != 关系.写入所有者) return false;
        }
        for (const auto& [编码, 事实] : 值.值) {
            if (!占用(编码) || 事实.编码.值 != 编码
                || !当前所有者有效(值, 事实.写入所有者)) return false;
            const auto 所属 = 值.节点.find(事实.所属节点.值);
            const auto 类型 = 值.节点.find(事实.属性类型节点.值);
            if (所属 == 值.节点.end() || 类型 == 值.节点.end()
                || !值.节点.contains(事实.来源节点.值)
                || 所属->second.写入所有者 != 事实.写入所有者
                || 类型->second.写入所有者 != 事实.写入所有者
                || 类型->second.种类 != 节点种类::属性类型
                || !类型->second.属性类型表示
                || !表示匹配(*类型->second.属性类型表示, 事实.材料)) return false;
            if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料);
                引用 && !值.节点.contains(引用->编码.值)) return false;
        }
        for (const auto& [编码, 节点] : 值.节点) {
            for (const auto& 槽 : 节点.当前属性) {
                const auto 类型 = 值.节点.find(槽.属性类型节点.值);
                const auto 当前值 = 值.值.find(槽.当前值.值);
                if (类型 == 值.节点.end() || 当前值 == 值.值.end()
                    || 类型->second.种类 != 节点种类::属性类型
                    || 当前值->second.所属节点.值 != 编码
                    || 当前值->second.属性类型节点 != 槽.属性类型节点
                    || 节点.写入所有者 != 类型->second.写入所有者
                    || 节点.写入所有者 != 当前值->second.写入所有者)
                    return false;
            }
        }
        return 值.下个编码 > 最大编码;
    }

    static std::optional<稳定编码> 解析引用(
        const L1所有者范围事实引用& 引用,
        const std::unordered_map<std::uint32_t, 稳定编码>& 映射) {
        if (const auto* 编码 = std::get_if<稳定编码>(&引用))
            return 有效(*编码) ? std::optional<稳定编码>{*编码} : std::nullopt;
        const auto 键 = std::get<L1所有者范围写集本地键>(引用);
        const auto it = 映射.find(键.值);
        return it == 映射.end() ? std::nullopt : std::optional<稳定编码>{it->second};
    }

    static L1所有者范围写入状态 应用写集(
        状态& 候选, L1结构所有者身份 所有者,
        const L1所有者范围写集请求& 请求,
        std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& 输出映射) {
        if (!当前所有者有效(候选, 所有者) || !有效(请求.写入幂等身份))
            return L1所有者范围写入状态::入口拒绝;
        std::unordered_map<std::uint32_t, 稳定编码> 映射;
        std::unordered_set<std::uint32_t> 本地键;
        const auto 登记本地键 = [&](L1所有者范围写集本地键 键) {
            return 有效(键) && 本地键.insert(键.值).second;
        };
        for (const auto& 项 : 请求.节点) if (!登记本地键(项.本地键))
            return L1所有者范围写入状态::入口拒绝;
        for (const auto& 项 : 请求.关系) if (!登记本地键(项.本地键))
            return L1所有者范围写入状态::入口拒绝;
        for (const auto& 项 : 请求.值) if (!登记本地键(项.本地键))
            return L1所有者范围写入状态::入口拒绝;
        for (const auto& 项 : 请求.属性槽变更)
            if (!有效(项.新当前值) || !本地键.contains(项.新当前值.值))
                return L1所有者范围写入状态::入口拒绝;

        for (const auto& 项 : 请求.节点) {
            const auto 编码 = 分配(候选);
            if (!编码) return L1所有者范围写入状态::资源失败;
            if ((项.种类 == 节点种类::属性类型) != 项.属性类型表示.has_value())
                return L1所有者范围写入状态::引用冲突;
            std::optional<值表示种类> 表示;
            if (项.属性类型表示) 表示 = static_cast<值表示种类>(*项.属性类型表示);
            候选.节点.emplace(编码->值, 节点事实{*编码, 项.种类, 表示, {}, 所有者});
            映射.emplace(项.本地键.值, *编码);
            输出映射.emplace_back(项.本地键, *编码);
        }
        for (const auto& 项 : 请求.关系) {
            const auto 源 = 解析引用(项.源节点, 映射);
            const auto 目标 = 解析引用(项.目标节点, 映射);
            const auto 类型 = 解析引用(项.关系类型节点, 映射);
            const auto 编码 = 分配(候选);
            if (!源 || !目标 || !类型) return L1所有者范围写入状态::未找到;
            if (!编码) return L1所有者范围写入状态::资源失败;
            候选.关系.emplace(编码->值,
                关系事实{*编码, *源, *目标, *类型, 项.角色或顺序, 所有者});
            映射.emplace(项.本地键.值, *编码);
            输出映射.emplace_back(项.本地键, *编码);
        }
        for (const auto& 项 : 请求.值) {
            const auto 所属 = 解析引用(项.所属节点, 映射);
            const auto 类型 = 解析引用(项.属性类型节点, 映射);
            const auto 来源 = 解析引用(项.来源节点, 映射);
            const auto 材料 = 转换材料(项.材料);
            const auto 编码 = 分配(候选);
            if (!所属 || !类型 || !来源) return L1所有者范围写入状态::未找到;
            if (!材料 || !编码) return L1所有者范围写入状态::资源失败;
            候选.值.emplace(编码->值,
                值事实{*编码, *所属, *类型, *材料, *来源, 所有者});
            映射.emplace(项.本地键.值, *编码);
            输出映射.emplace_back(项.本地键, *编码);
        }
        for (const auto& 项 : 请求.属性槽变更) {
            const auto 节点编码 = 解析引用(项.所属节点, 映射);
            const auto 类型编码 = 解析引用(项.属性类型节点, 映射);
            const auto 新值 = 映射.find(项.新当前值.值);
            if (!节点编码 || !类型编码 || 新值 == 映射.end())
                return L1所有者范围写入状态::未找到;
            const auto 节点项 = 候选.节点.find(节点编码->值);
            if (节点项 == 候选.节点.end()) return L1所有者范围写入状态::未找到;
            auto& 槽组 = 节点项->second.当前属性;
            const auto 槽 = std::lower_bound(槽组.begin(), 槽组.end(), *类型编码,
                [](const 属性槽& 左, 稳定编码 右) { return 左.属性类型节点 < 右; });
            if (槽 != 槽组.end() && 槽->属性类型节点 == *类型编码)
                槽->当前值 = 新值->second;
            else 槽组.insert(槽, 属性槽{*类型编码, 新值->second});
        }
        for (const auto 编码 : 请求.退出事实) {
            if (!有效(编码)) return L1所有者范围写入状态::入口拒绝;
            bool 已删除 = false;
            if (const auto it = 候选.关系.find(编码.值); it != 候选.关系.end()) {
                if (it->second.写入所有者 != 所有者) return L1所有者范围写入状态::许可拒绝;
                候选.关系.erase(it); 已删除 = true;
            } else if (const auto it = 候选.值.find(编码.值); it != 候选.值.end()) {
                if (it->second.写入所有者 != 所有者) return L1所有者范围写入状态::许可拒绝;
                候选.值.erase(it);已删除 = true;
                for (auto& [_, 节点] : 候选.节点)
                    std::erase_if(节点.当前属性,
                        [&](const 属性槽& 槽) { return 槽.当前值 == 编码; });
            } else if (const auto it = 候选.节点.find(编码.值); it != 候选.节点.end()) {
                if (it->second.写入所有者 != 所有者) return L1所有者范围写入状态::许可拒绝;
                候选.节点.erase(it); 已删除 = true;
            }
            if (!已删除) return L1所有者范围写入状态::未找到;
        }
        std::sort(输出映射.begin(), 输出映射.end(), [](const auto& 左, const auto& 右) {
            return 左.first < 右.first;
        });
        return 状态完整(候选)
            ? L1所有者范围写入状态::成功
            : L1所有者范围写入状态::引用冲突;
    }

    static bool 应用多分区写集(
        状态& 候选,
        const std::vector<L1三分区原子参与者写集>& 参与者组,
        std::vector<L1三分区原子参与者结果>& 结果组,
        L1所有者范围写入状态& 失败状态) {
        std::unordered_set<std::uint64_t> 所有者组;
        std::vector<std::unordered_map<std::uint32_t, 稳定编码>> 已完成映射(
            参与者组.size() + 1);
        for (std::size_t i = 0; i < 参与者组.size(); ++i) {
            const auto& 参与者 = 参与者组[i];
            const auto 身份 = static_cast<std::uint8_t>(i + 1);
            if (参与者.参与者.值 != 身份 || !有效(参与者.所有者)
                || !所有者组.insert(参与者.所有者.编码.值).second) {
                失败状态 = L1所有者范围写入状态::入口拒绝;
                return false;
            }
            const auto 转换引用 = [&](const L1三分区原子事实引用值& 引用)
                -> std::optional<L1所有者范围事实引用> {
                if (const auto* 编码 = std::get_if<稳定编码>(&引用))
                    return 有效(*编码) ? std::optional<L1所有者范围事实引用>{*编码}
                        : std::nullopt;
                if (const auto* 本地 = std::get_if<L1所有者范围写集本地键>(&引用))
                    return 有效(*本地) ? std::optional<L1所有者范围事实引用>{*本地}
                        : std::nullopt;
                const auto& 跨 = std::get<L1三分区原子事实引用>(引用);
                if (跨.参与者.值 == 0 || 跨.参与者.值 >= 身份 || !有效(跨.本地键))
                    return std::nullopt;
                const auto it = 已完成映射[跨.参与者.值].find(跨.本地键.值);
                return it == 已完成映射[跨.参与者.值].end()
                    ? std::nullopt
                    : std::optional<L1所有者范围事实引用>{it->second};
            };
            L1所有者范围写集请求 写集;
            写集.写入幂等身份 = 参与者.写集.写入幂等身份;
            for (const auto& 项 : 参与者.写集.节点)
                写集.节点.push_back({项.本地键, 项.种类, 项.属性类型表示});
            for (const auto& 项 : 参与者.写集.关系) {
                const auto 源 = 转换引用(项.源节点);
                const auto 目标 = 转换引用(项.目标节点);
                const auto 类型 = 转换引用(项.关系类型节点);
                if (!源 || !目标 || !类型) {
                    失败状态 = L1所有者范围写入状态::引用冲突;
                    return false;
                }
                写集.关系.push_back({项.本地键, *源, *目标, *类型, 项.角色或顺序});
            }
            for (const auto& 项 : 参与者.写集.值) {
                const auto 所属 = 转换引用(项.所属节点);
                const auto 类型 = 转换引用(项.属性类型节点);
                const auto 来源 = 转换引用(项.来源节点);
                if (!所属 || !类型 || !来源) {
                    失败状态 = L1所有者范围写入状态::引用冲突;
                    return false;
                }
                写集.值.push_back({项.本地键, *所属, *类型, 项.材料, *来源});
            }
            for (const auto& 项 : 参与者.写集.属性槽变更) {
                const auto 所属 = 转换引用(项.所属节点);
                const auto 类型 = 转换引用(项.属性类型节点);
                if (!所属 || !类型) {
                    失败状态 = L1所有者范围写入状态::引用冲突;
                    return false;
                }
                写集.属性槽变更.push_back({*所属, *类型, 项.新当前值});
            }
            写集.退出事实 = 参与者.写集.退出事实;
            std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 映射;
            失败状态 = 应用写集(候选, 参与者.所有者, 写集, 映射);
            if (失败状态 != L1所有者范围写入状态::成功) return false;
            for (const auto& [本地, 编码] : 映射)
                已完成映射[身份].emplace(本地.值, 编码);
            结果组.push_back({参与者.参与者, 参与者.所有者, std::move(映射)});
        }
        return 状态完整(候选);
    }

    bool 准备持久发布(const 状态& 候选) noexcept;
    L1事实基座核心持久恢复结果 初始化持久恢复(
        const std::filesystem::path& 根) noexcept;

    struct 编码器 final {
        std::vector<std::uint8_t> 数据;
        void U8(std::uint8_t 值) { 数据.push_back(值); }
        void U32(std::uint32_t 值) { for (unsigned i = 0; i != 4; ++i) U8(static_cast<std::uint8_t>(值 >> (i * 8))); }
        void U64(std::uint64_t 值) { for (unsigned i = 0; i != 8; ++i) U8(static_cast<std::uint8_t>(值 >> (i * 8))); }
        void I64(std::int64_t 值) { U64(static_cast<std::uint64_t>(值)); }
        void 布尔(bool 值) { U8(值 ? 1 : 0); }
    };
    struct 解码器 final {
        const std::vector<std::uint8_t>& 数据;
        std::size_t 位置 = 0;
        bool U8(std::uint8_t& 值) { if (位置 >= 数据.size()) return false; 值 = 数据[位置++]; return true; }
        bool U32(std::uint32_t& 值) { 值 = 0; std::uint8_t 项{}; for (unsigned i = 0; i != 4; ++i) { if (!U8(项)) return false; 值 |= std::uint32_t{项} << (i * 8); } return true; }
        bool U64(std::uint64_t& 值) { 值 = 0; std::uint8_t 项{}; for (unsigned i = 0; i != 8; ++i) { if (!U8(项)) return false; 值 |= std::uint64_t{项} << (i * 8); } return true; }
        bool I64(std::int64_t& 值) { std::uint64_t 原{}; if (!U64(原)) return false; 值 = static_cast<std::int64_t>(原); return true; }
        bool 布尔(bool& 值) { std::uint8_t 原{}; if (!U8(原) || 原 > 1) return false; 值 = 原 != 0; return true; }
        bool 完结() const noexcept { return 位置 == 数据.size(); }
    };

    template<class T, class F> static void 写组(编码器& 出, const std::vector<T>& 组, F 写一) {
        出.U64(static_cast<std::uint64_t>(组.size())); for (const auto& 项 : 组) 写一(出, 项);
    }
    template<class T, class F> static bool 读组(解码器& 入, std::vector<T>& 组, F 读一) {
        std::uint64_t 数{}; if (!入.U64(数) || 数 > 入.数据.size() - 入.位置) return false;
        组.clear(); 组.reserve(static_cast<std::size_t>(数));
        for (std::uint64_t i = 0; i != 数; ++i) { T 项{}; if (!读一(入, 项)) return false; 组.push_back(std::move(项)); }
        return true;
    }
    template<class T, class F> static void 写可选(编码器& 出, const std::optional<T>& 值, F 写一) {
        出.U8(值 ? 1 : 0); if (值) 写一(出, *值);
    }
    template<class T, class F> static bool 读可选(解码器& 入, std::optional<T>& 值, F 读一) {
        std::uint8_t 有{}; if (!入.U8(有) || 有 > 1) return false; if (!有) { 值.reset(); return true; }
        T 项{}; if (!读一(入, 项)) return false; 值 = std::move(项); return true;
    }
    template<class E> static void 写枚举(编码器& 出, E 值) { 出.U8(static_cast<std::uint8_t>(值)); }
    template<class E> static bool 读枚举(解码器& 入, E& 值, std::initializer_list<std::uint8_t> 允许) {
        std::uint8_t 原{}; if (!入.U8(原) || std::find(允许.begin(), 允许.end(), 原) == 允许.end()) return false;
        值 = static_cast<E>(原); return true;
    }
    template<class T, class F> static void 写表(编码器& 出, const std::unordered_map<std::uint64_t, T>& 表, F 写值) {
        std::vector<std::uint64_t> 键; 键.reserve(表.size()); for (const auto& [k, _] : 表) 键.push_back(k);
        std::sort(键.begin(), 键.end()); 出.U64(static_cast<std::uint64_t>(键.size()));
        for (const auto k : 键) { 出.U64(k); 写值(出, 表.at(k)); }
    }
    template<class T, class F> static bool 读表(解码器& 入, std::unordered_map<std::uint64_t, T>& 表, F 读值) {
        std::uint64_t 数{}; if (!入.U64(数) || 数 > 入.数据.size() - 入.位置) return false;
        表.clear(); 表.reserve(static_cast<std::size_t>(数)); std::uint64_t 前{};
        for (std::uint64_t i = 0; i != 数; ++i) { std::uint64_t 键{}; T 值{}; if (!入.U64(键) || (i && 键 <= 前) || !读值(入, 值) || !表.emplace(键, std::move(值)).second) return false; 前 = 键; }
        return true;
    }

    static void 写(编码器& 出, 稳定编码 值) { 出.U64(值.值); }
    static bool 读(解码器& 入, 稳定编码& 值) { return 入.U64(值.值); }
    static void 写(编码器& 出, L1结构所有者身份 值) { 写(出, 值.编码); }
    static bool 读(解码器& 入, L1结构所有者身份& 值) { return 读(入, 值.编码); }
    static void 写(编码器& 出, L1中性写集本地键 值) { 出.U32(值.值); }
    static bool 读(解码器& 入, L1中性写集本地键& 值) { return 入.U32(值.值); }
    static void 写(编码器& 出, L1所有者范围写集本地键 值) { 出.U32(值.值); }
    static bool 读(解码器& 入, L1所有者范围写集本地键& 值) { return 入.U32(值.值); }
    static void 写(编码器& 出, L1中性写集幂等键 值) { 出.U64(值.值); }
    static bool 读(解码器& 入, L1中性写集幂等键& 值) { return 入.U64(值.值); }
    static void 写(编码器& 出, L1所有者范围建立幂等身份 值) { 出.U64(值.值); }
    static bool 读(解码器& 入, L1所有者范围建立幂等身份& 值) { return 入.U64(值.值); }
    static void 写(编码器& 出, L1所有者范围写入幂等身份 值) { 出.U64(值.值); }
    static bool 读(解码器& 入, L1所有者范围写入幂等身份& 值) { return 入.U64(值.值); }

    template<class 材料类型, class 引用类型> static void 写材料(编码器& 出, const 材料类型& 值) {
        if (const auto* 项 = std::get_if<std::int64_t>(&值)) { 出.U8(1); 出.I64(*项); }
        else if (const auto* 项 = std::get_if<std::vector<std::int64_t>>(&值)) { 出.U8(2); 写组(出, *项, [](auto& e, auto v){ e.I64(v); }); }
        else if (const auto* 项 = std::get_if<std::vector<std::uint64_t>>(&值)) { 出.U8(3); 写组(出, *项, [](auto& e, auto v){ e.U64(v); }); }
        else { 出.U8(4); 写(出, std::get<引用类型>(值).编码); }
    }
    template<class 材料类型, class 引用类型> static bool 读材料(解码器& 入, 材料类型& 值) {
        std::uint8_t 标签{}; if (!入.U8(标签)) return false;
        if (标签 == 1) { std::int64_t 项{}; if (!入.I64(项)) return false; 值 = 项; return true; }
        if (标签 == 2) { std::vector<std::int64_t> 项; if (!读组(入, 项, [](auto& d, auto& v){ return d.I64(v); })) return false; 值 = std::move(项); return true; }
        if (标签 == 3) { std::vector<std::uint64_t> 项; if (!读组(入, 项, [](auto& d, auto& v){ return d.U64(v); })) return false; 值 = std::move(项); return true; }
        if (标签 == 4) { 引用类型 项; if (!读(入, 项.编码)) return false; 值 = 项; return true; }
        return false;
    }
    static void 写(编码器& 出, const 属性槽& 值) { 写(出, 值.属性类型节点); 写(出, 值.当前值); }
    static bool 读(解码器& 入, 属性槽& 值) { return 读(入, 值.属性类型节点) && 读(入, 值.当前值); }
    static void 写(编码器& 出, const L1结构所有者事实& 值) { 写(出, 值.所有者); 写枚举(出, 值.范围种类); }
    static bool 读(解码器& 入, L1结构所有者事实& 值) { return 读(入, 值.所有者) && 读枚举(入, 值.范围种类, {1, 2}); }
    static void 写(编码器& 出, const 节点事实& 值) { 写(出, 值.编码); 写枚举(出, 值.种类); 写可选(出, 值.属性类型表示, [](auto& e, auto v){ 写枚举(e, v); }); 写组(出, 值.当前属性, [](auto& e, const auto& v){ 写(e, v); }); 写(出, 值.写入所有者); }
    static bool 读(解码器& 入, 节点事实& 值) { return 读(入, 值.编码) && 读枚举(入, 值.种类, {1, 2, 3}) && 读可选(入, 值.属性类型表示, [](auto& d, auto& v){ return 读枚举(d, v, {1, 2, 3, 4}); }) && 读组(入, 值.当前属性, [](auto& d, auto& v){ return 读(d, v); }) && 读(入, 值.写入所有者); }
    static void 写(编码器& 出, const 关系事实& 值) { 写(出, 值.编码); 写(出, 值.源节点); 写(出, 值.目标节点); 写(出, 值.关系类型节点); 出.I64(值.角色或顺序); 写(出, 值.写入所有者); }
    static bool 读(解码器& 入, 关系事实& 值) { return 读(入, 值.编码) && 读(入, 值.源节点) && 读(入, 值.目标节点) && 读(入, 值.关系类型节点) && 入.I64(值.角色或顺序) && 读(入, 值.写入所有者); }
    static void 写(编码器& 出, const 值事实& 值) { 写(出, 值.编码); 写(出, 值.所属节点); 写(出, 值.属性类型节点); 写材料<原始值材料, 独立材料引用>(出, 值.材料); 写(出, 值.来源节点); 写(出, 值.写入所有者); }
    static bool 读(解码器& 入, 值事实& 值) { return 读(入, 值.编码) && 读(入, 值.所属节点) && 读(入, 值.属性类型节点) && 读材料<原始值材料, 独立材料引用>(入, 值.材料) && 读(入, 值.来源节点) && 读(入, 值.写入所有者); }

    static void 写中性引用(编码器& 出, const L1中性事实引用& 值) { if (const auto* 项 = std::get_if<稳定编码>(&值)) { 出.U8(1); 写(出, *项); } else { 出.U8(2); 写(出, std::get<L1中性写集本地键>(值)); } }
    static bool 读中性引用(解码器& 入, L1中性事实引用& 值) { std::uint8_t 标签{}; if (!入.U8(标签)) return false; if (标签 == 1) { 稳定编码 项; if (!读(入, 项)) return false; 值 = 项; return true; } if (标签 == 2) { L1中性写集本地键 项; if (!读(入, 项)) return false; 值 = 项; return true; } return false; }
    static void 写所有者引用(编码器& 出, const L1所有者范围事实引用& 值) { if (const auto* 项 = std::get_if<稳定编码>(&值)) { 出.U8(1); 写(出, *项); } else { 出.U8(2); 写(出, std::get<L1所有者范围写集本地键>(值)); } }
    static bool 读所有者引用(解码器& 入, L1所有者范围事实引用& 值) { std::uint8_t 标签{}; if (!入.U8(标签)) return false; if (标签 == 1) { 稳定编码 项; if (!读(入, 项)) return false; 值 = 项; return true; } if (标签 == 2) { L1所有者范围写集本地键 项; if (!读(入, 项)) return false; 值 = 项; return true; } return false; }

    static void 写(编码器& 出, const L1中性写集请求& 值) {
        写(出, 值.幂等键);
        写组(出, 值.节点, [](auto& e, const auto& v){ 写(e, v.本地键); 写枚举(e, v.种类); 写可选(e, v.属性类型表示, [](auto& x, auto y){ 写枚举(x, y); }); });
        写组(出, 值.关系, [](auto& e, const auto& v){ 写(e, v.本地键); 写中性引用(e, v.源节点); 写中性引用(e, v.目标节点); 写中性引用(e, v.关系类型节点); e.I64(v.角色或顺序); });
        写组(出, 值.值, [](auto& e, const auto& v){ 写(e, v.本地键); 写中性引用(e, v.所属节点); 写中性引用(e, v.属性类型节点); 写材料<L1中性原始值材料, L1中性独立材料引用>(e, v.材料); 写中性引用(e, v.来源节点); });
        写组(出, 值.属性槽变更, [](auto& e, const auto& v){ 写中性引用(e, v.所属节点); 写中性引用(e, v.属性类型节点); 写(e, v.新当前值); });
        写组(出, 值.退出事实, [](auto& e, auto v){ 写(e, v); });
    }
    static bool 读(解码器& 入, L1中性写集请求& 值) {
        return 读(入, 值.幂等键)
            && 读组(入, 值.节点, [](auto& d, auto& v){ return 读(d, v.本地键) && 读枚举(d, v.种类, {1,2,3}) && 读可选(d, v.属性类型表示, [](auto& x, auto& y){ return 读枚举(x, y, {1,2,3,4}); }); })
            && 读组(入, 值.关系, [](auto& d, auto& v){ return 读(d, v.本地键) && 读中性引用(d, v.源节点) && 读中性引用(d, v.目标节点) && 读中性引用(d, v.关系类型节点) && d.I64(v.角色或顺序); })
            && 读组(入, 值.值, [](auto& d, auto& v){ return 读(d, v.本地键) && 读中性引用(d, v.所属节点) && 读中性引用(d, v.属性类型节点) && 读材料<L1中性原始值材料, L1中性独立材料引用>(d, v.材料) && 读中性引用(d, v.来源节点); })
            && 读组(入, 值.属性槽变更, [](auto& d, auto& v){ return 读中性引用(d, v.所属节点) && 读中性引用(d, v.属性类型节点) && 读(d, v.新当前值); })
            && 读组(入, 值.退出事实, [](auto& d, auto& v){ return 读(d, v); });
    }
    static void 写(编码器& 出, const L1中性写入结果& 值) { 写枚举(出, 值.状态); 写(出, 值.幂等键); 出.布尔(值.是否形成内存权威发布); 写枚举(出, 值.重试边界); 写组(出, 值.新编码映射, [](auto& e, const auto& v){ 写(e, v.first); 写(e, v.second); }); }
    static bool 读(解码器& 入, L1中性写入结果& 值) { return 读枚举(入, 值.状态, {1,2,3,4,7,8,9,10}) && 读(入, 值.幂等键) && 入.布尔(值.是否形成内存权威发布) && 读枚举(入, 值.重试边界, {1,2,3,4}) && 读组(入, 值.新编码映射, [](auto& d, auto& v){ return 读(d, v.first) && 读(d, v.second); }); }

    static void 写(编码器& 出, const L1所有者范围建立请求& 值) { 写(出, 值.建立幂等身份); 写枚举(出, 值.范围种类); }
    static bool 读(解码器& 入, L1所有者范围建立请求& 值) { return 读(入, 值.建立幂等身份) && 读枚举(入, 值.范围种类, {1,2}); }
    static void 写(编码器& 出, const L1所有者范围建立结果& 值) { 写枚举(出, 值.状态); 写(出, 值.建立幂等身份); 写可选(出, 值.所有者事实, [](auto& e, const auto& v){ 写(e, v); }); 出.布尔(值.是否形成内存权威发布); 写枚举(出, 值.重试边界); }
    static bool 读(解码器& 入, L1所有者范围建立结果& 值) { return 读枚举(入, 值.状态, {1,2,3,4,5,8,9,10,11}) && 读(入, 值.建立幂等身份) && 读可选(入, 值.所有者事实, [](auto& d, auto& v){ return 读(d, v); }) && 入.布尔(值.是否形成内存权威发布) && 读枚举(入, 值.重试边界, {1,2,3,4}); }

    static void 写(编码器& 出, const L1所有者范围写集请求& 值) {
        写(出, 值.写入幂等身份);
        写组(出, 值.节点, [](auto& e, const auto& v){ 写(e, v.本地键); 写枚举(e, v.种类); 写可选(e, v.属性类型表示, [](auto& x, auto y){ 写枚举(x, y); }); });
        写组(出, 值.关系, [](auto& e, const auto& v){ 写(e, v.本地键); 写所有者引用(e, v.源节点); 写所有者引用(e, v.目标节点); 写所有者引用(e, v.关系类型节点); e.I64(v.角色或顺序); });
        写组(出, 值.值, [](auto& e, const auto& v){ 写(e, v.本地键); 写所有者引用(e, v.所属节点); 写所有者引用(e, v.属性类型节点); 写材料<L1所有者范围原始值材料, L1所有者范围独立材料引用>(e, v.材料); 写所有者引用(e, v.来源节点); });
        写组(出, 值.属性槽变更, [](auto& e, const auto& v){ 写所有者引用(e, v.所属节点); 写所有者引用(e, v.属性类型节点); 写(e, v.新当前值); });
        写组(出, 值.退出事实, [](auto& e, auto v){ 写(e, v); });
    }
    static bool 读(解码器& 入, L1所有者范围写集请求& 值) {
        return 读(入, 值.写入幂等身份)
            && 读组(入, 值.节点, [](auto& d, auto& v){ return 读(d, v.本地键) && 读枚举(d, v.种类, {1,2,3}) && 读可选(d, v.属性类型表示, [](auto& x, auto& y){ return 读枚举(x, y, {1,2,3,4}); }); })
            && 读组(入, 值.关系, [](auto& d, auto& v){ return 读(d, v.本地键) && 读所有者引用(d, v.源节点) && 读所有者引用(d, v.目标节点) && 读所有者引用(d, v.关系类型节点) && d.I64(v.角色或顺序); })
            && 读组(入, 值.值, [](auto& d, auto& v){ return 读(d, v.本地键) && 读所有者引用(d, v.所属节点) && 读所有者引用(d, v.属性类型节点) && 读材料<L1所有者范围原始值材料, L1所有者范围独立材料引用>(d, v.材料) && 读所有者引用(d, v.来源节点); })
            && 读组(入, 值.属性槽变更, [](auto& d, auto& v){ return 读所有者引用(d, v.所属节点) && 读所有者引用(d, v.属性类型节点) && 读(d, v.新当前值); })
            && 读组(入, 值.退出事实, [](auto& d, auto& v){ return 读(d, v); });
    }
    static void 写(编码器& 出, const L1所有者范围写入结果& 值) { 写枚举(出, 值.状态); 写(出, 值.所有者); 写(出, 值.写入幂等身份); 出.布尔(值.是否形成内存权威发布); 写枚举(出, 值.重试边界); 写组(出, 值.新编码映射, [](auto& e, const auto& v){ 写(e, v.first); 写(e, v.second); }); }
    static bool 读(解码器& 入, L1所有者范围写入结果& 值) { return 读枚举(入, 值.状态, {1,2,3,4,5,8,9,10,11}) && 读(入, 值.所有者) && 读(入, 值.写入幂等身份) && 入.布尔(值.是否形成内存权威发布) && 读枚举(入, 值.重试边界, {1,2,3,4}) && 读组(入, 值.新编码映射, [](auto& d, auto& v){ return 读(d, v.first) && 读(d, v.second); }); }

    template<class 引用> static void 写跨引用(编码器& 出, const 引用& 值) {
        if (const auto* 稳定项 = std::get_if<稳定编码>(&值)) { 出.U8(1); 写(出, *稳定项); }
        else if (const auto* 本地项 = std::get_if<L1所有者范围写集本地键>(&值)) { 出.U8(2); 写(出, *本地项); }
        else {
            出.U8(3);
            const auto& 跨项 = std::get<2>(值);
            using 参与者类型 = std::decay_t<decltype(跨项.参与者)>;
            if constexpr (std::is_enum_v<参与者类型>) 出.U8(static_cast<std::uint8_t>(跨项.参与者));
            else 出.U8(跨项.参与者.值);
            写(出, 跨项.本地键);
        }
    }
    static bool 读跨引用(解码器& 入, L1跨所有者原子事实引用值& 值) { std::uint8_t 标签{}; if (!入.U8(标签)) return false; if (标签 == 1) { 稳定编码 项; if (!读(入, 项)) return false; 值=项; return true; } if (标签 == 2) { L1所有者范围写集本地键 项; if (!读(入, 项)) return false; 值=项; return true; } if (标签 == 3) { L1跨所有者原子事实引用 项; std::uint8_t p{}; if (!入.U8(p) || p<1 || p>2 || !读(入, 项.本地键)) return false; 项.参与者=static_cast<L1跨所有者原子事务参与者序号>(p); 值=项; return true; } return false; }
    static bool 读跨引用(解码器& 入, L1三分区原子事实引用值& 值) { std::uint8_t 标签{}; if (!入.U8(标签)) return false; if (标签 == 1) { 稳定编码 项; if (!读(入, 项)) return false; 值=项; return true; } if (标签 == 2) { L1所有者范围写集本地键 项; if (!读(入, 项)) return false; 值=项; return true; } if (标签 == 3) { L1三分区原子事实引用 项; if (!入.U8(项.参与者.值) || 项.参与者.值<1 || !读(入, 项.本地键)) return false; 值=项; return true; } return false; }

    static void 写(编码器& 出, const L1跨所有者原子写集请求& 值) {
        写(出, 值.写入幂等身份);
        写组(出, 值.节点, [](auto& e,const auto& v){写(e,v.本地键);写枚举(e,v.种类);写可选(e,v.属性类型表示,[](auto&x,auto y){写枚举(x,y);});});
        写组(出, 值.关系, [](auto&e,const auto&v){写(e,v.本地键);写跨引用(e,v.源节点);写跨引用(e,v.目标节点);写跨引用(e,v.关系类型节点);e.I64(v.角色或顺序);});
        写组(出, 值.值, [](auto&e,const auto&v){写(e,v.本地键);写跨引用(e,v.所属节点);写跨引用(e,v.属性类型节点);写材料<L1所有者范围原始值材料,L1所有者范围独立材料引用>(e,v.材料);写跨引用(e,v.来源节点);});
        写组(出, 值.属性槽变更, [](auto&e,const auto&v){写跨引用(e,v.所属节点);写跨引用(e,v.属性类型节点);写(e,v.新当前值);}); 写组(出,值.退出事实,[](auto&e,auto v){写(e,v);});
    }
    static bool 读(解码器& 入, L1跨所有者原子写集请求& 值) { return 读(入,值.写入幂等身份) && 读组(入,值.节点,[](auto&d,auto&v){return 读(d,v.本地键)&&读枚举(d,v.种类,{1,2,3})&&读可选(d,v.属性类型表示,[](auto&x,auto&y){return 读枚举(x,y,{1,2,3,4});});}) && 读组(入,值.关系,[](auto&d,auto&v){return 读(d,v.本地键)&&读跨引用(d,v.源节点)&&读跨引用(d,v.目标节点)&&读跨引用(d,v.关系类型节点)&&d.I64(v.角色或顺序);}) && 读组(入,值.值,[](auto&d,auto&v){return 读(d,v.本地键)&&读跨引用(d,v.所属节点)&&读跨引用(d,v.属性类型节点)&&读材料<L1所有者范围原始值材料,L1所有者范围独立材料引用>(d,v.材料)&&读跨引用(d,v.来源节点);}) && 读组(入,值.属性槽变更,[](auto&d,auto&v){return 读跨引用(d,v.所属节点)&&读跨引用(d,v.属性类型节点)&&读(d,v.新当前值);}) && 读组(入,值.退出事实,[](auto&d,auto&v){return 读(d,v);}); }
    static void 写(编码器& 出, const L1跨所有者原子参与者写集& 值) { 写枚举(出,值.参与者);写(出,值.所有者);写(出,值.写集); }
    static bool 读(解码器& 入, L1跨所有者原子参与者写集& 值) { return 读枚举(入,值.参与者,{1,2})&&读(入,值.所有者)&&读(入,值.写集); }
    static void 写(编码器& 出, const L1跨所有者原子事务请求& 值) { 写(出,值.组合写入幂等身份);写(出,值.状态写集);写(出,值.动态写集); }
    static bool 读(解码器& 入, L1跨所有者原子事务请求& 值) { return 读(入,值.组合写入幂等身份)&&读(入,值.状态写集)&&读(入,值.动态写集); }
    static void 写(编码器& 出, const L1跨所有者原子事务结果& 值) { 写枚举(出,值.状态);出.布尔(值.是否形成内存权威发布);写枚举(出,值.重试边界);写组(出,值.状态编码映射,[](auto&e,const auto&v){写(e,v.first);写(e,v.second);});写组(出,值.动态编码映射,[](auto&e,const auto&v){写(e,v.first);写(e,v.second);}); }
    static bool 读(解码器& 入, L1跨所有者原子事务结果& 值) { return 读枚举(入,值.状态,{1,2,3,4,5,8,9,10,11})&&入.布尔(值.是否形成内存权威发布)&&读枚举(入,值.重试边界,{1,2,3,4})&&读组(入,值.状态编码映射,[](auto&d,auto&v){return 读(d,v.first)&&读(d,v.second);})&&读组(入,值.动态编码映射,[](auto&d,auto&v){return 读(d,v.first)&&读(d,v.second);}); }

    static void 写三写集(编码器& 出, const L1三分区原子写集请求& 值) {
        写(出,值.写入幂等身份);写组(出,值.节点,[](auto&e,const auto&v){写(e,v.本地键);写枚举(e,v.种类);写可选(e,v.属性类型表示,[](auto&x,auto y){写枚举(x,y);});});
        写组(出,值.关系,[](auto&e,const auto&v){写(e,v.本地键);写跨引用(e,v.源节点);写跨引用(e,v.目标节点);写跨引用(e,v.关系类型节点);e.I64(v.角色或顺序);});
        写组(出,值.值,[](auto&e,const auto&v){写(e,v.本地键);写跨引用(e,v.所属节点);写跨引用(e,v.属性类型节点);写材料<L1所有者范围原始值材料,L1所有者范围独立材料引用>(e,v.材料);写跨引用(e,v.来源节点);});
        写组(出,值.属性槽变更,[](auto&e,const auto&v){写跨引用(e,v.所属节点);写跨引用(e,v.属性类型节点);写(e,v.新当前值);});写组(出,值.退出事实,[](auto&e,auto v){写(e,v);});
    }
    static bool 读三写集(解码器& 入, L1三分区原子写集请求& 值) { return 读(入,值.写入幂等身份)&&读组(入,值.节点,[](auto&d,auto&v){return 读(d,v.本地键)&&读枚举(d,v.种类,{1,2,3})&&读可选(d,v.属性类型表示,[](auto&x,auto&y){return 读枚举(x,y,{1,2,3,4});});})&&读组(入,值.关系,[](auto&d,auto&v){return 读(d,v.本地键)&&读跨引用(d,v.源节点)&&读跨引用(d,v.目标节点)&&读跨引用(d,v.关系类型节点)&&d.I64(v.角色或顺序);})&&读组(入,值.值,[](auto&d,auto&v){return 读(d,v.本地键)&&读跨引用(d,v.所属节点)&&读跨引用(d,v.属性类型节点)&&读材料<L1所有者范围原始值材料,L1所有者范围独立材料引用>(d,v.材料)&&读跨引用(d,v.来源节点);})&&读组(入,值.属性槽变更,[](auto&d,auto&v){return 读跨引用(d,v.所属节点)&&读跨引用(d,v.属性类型节点)&&读(d,v.新当前值);})&&读组(入,值.退出事实,[](auto&d,auto&v){return 读(d,v);}); }
    static void 写三参与者(编码器& 出,const L1三分区原子参与者写集& 值){出.U8(值.参与者.值);写(出,值.所有者);写三写集(出,值.写集);}
    static bool 读三参与者(解码器& 入,L1三分区原子参与者写集& 值){return 入.U8(值.参与者.值)&&值.参与者.值>=1&&读(入,值.所有者)&&读三写集(入,值.写集);}
    static void 写三请求(编码器& 出,const L1三分区原子事务请求& 值){写(出,值.组合写入幂等身份);写组(出,值.参与者写集组,[](auto&e,const auto&v){写三参与者(e,v);});}
    static bool 读三请求(解码器& 入,L1三分区原子事务请求& 值){return 读(入,值.组合写入幂等身份)&&读组(入,值.参与者写集组,[](auto&d,auto&v){return 读三参与者(d,v);});}
    static void 写三参与者结果(编码器& 出,const L1三分区原子参与者结果& 值){出.U8(值.参与者.值);写(出,值.所有者);写组(出,值.新编码映射,[](auto&e,const auto&v){写(e,v.first);写(e,v.second);});}
    static bool 读三参与者结果(解码器& 入,L1三分区原子参与者结果& 值){return 入.U8(值.参与者.值)&&值.参与者.值>=1&&读(入,值.所有者)&&读组(入,值.新编码映射,[](auto&d,auto&v){return 读(d,v.first)&&读(d,v.second);});}
    template<class 结果类型> static void 写多结果(编码器& 出,const 结果类型& 值){写枚举(出,值.状态);写(出,值.组合写入幂等身份);出.布尔(值.是否已确认形成内存权威发布);写枚举(出,值.重试边界);写组(出,值.参与者结果组,[](auto&e,const auto&v){写三参与者结果(e,v);});}
    static bool 读三结果(解码器& 入,L1三分区原子事务结果& 值){return 读枚举(入,值.状态,{1,2,3,5,6,7,8,9})&&读(入,值.组合写入幂等身份)&&入.布尔(值.是否已确认形成内存权威发布)&&读枚举(入,值.重试边界,{1,2,3,4})&&读组(入,值.参与者结果组,[](auto&d,auto&v){return 读三参与者结果(d,v);});}
    static bool 读N结果(解码器& 入,L1有限N分区原子事务结果& 值){return 读枚举(入,值.状态,{1,2,3,5,6,7,8,9})&&读(入,值.组合写入幂等身份)&&入.布尔(值.是否已确认形成内存权威发布)&&读枚举(入,值.重试边界,{1,2,3,4})&&读组(入,值.参与者结果组,[](auto&d,auto&v){return 读三参与者结果(d,v);});}

    static std::vector<std::uint8_t> 编码状态(const 状态& 值);
    static bool 解码状态(const std::vector<std::uint8_t>& 数据, 状态& 值);
    static bool SHA256(const std::vector<std::uint8_t>& 数据, std::array<std::uint8_t, 32>& 摘要) noexcept;
    static bool 读文件(const std::filesystem::path& 路径, std::vector<std::uint8_t>& 数据) noexcept;
    static bool 写文件并刷新(const std::filesystem::path& 路径, const std::vector<std::uint8_t>& 数据) noexcept;
    static std::vector<std::uint8_t> 编码清单(std::uint64_t 序号, std::uint8_t 槽, std::uint64_t 长度, const std::array<std::uint8_t, 32>& 摘要);
    static bool 解码清单(const std::vector<std::uint8_t>& 数据, std::uint64_t& 序号, std::uint8_t& 槽, std::uint64_t& 长度, std::array<std::uint8_t, 32>& 摘要);
};

std::vector<std::uint8_t> L1事实基座仓库::实现::编码状态(const 状态& 值) {
    编码器 出;
    出.U64(0x3150414E534C3148ULL);
    出.U32(5);
    出.U64(值.下个编码);
    写表(出, 值.所有者, [](auto& e, const auto& v){ 写(e, v); });
    写表(出, 值.节点, [](auto& e, const auto& v){ 写(e, v); });
    写表(出, 值.关系, [](auto& e, const auto& v){ 写(e, v); });
    写表(出, 值.值, [](auto& e, const auto& v){ 写(e, v); });
    写表(出, 值.所有者建立账, [](auto& e, const auto& v){ 写(e, v.请求); 写(e, v.结果); });
    写表(出, 值.中性写入账, [](auto& e, const auto& v){ 写(e, v.请求); 写(e, v.结果); });

    std::vector<std::uint64_t> 所有者键;
    所有者键.reserve(值.所有者写入账.size());
    for (const auto& [键, _] : 值.所有者写入账) 所有者键.push_back(键);
    std::sort(所有者键.begin(), 所有者键.end());
    出.U64(static_cast<std::uint64_t>(所有者键.size()));
    for (const auto 所有者 : 所有者键) {
        出.U64(所有者);
        写表(出, 值.所有者写入账.at(所有者), [](auto& e, const auto& v){ 写(e, v.请求); 写(e, v.结果); });
    }
    写表(出, 值.跨所有者账, [](auto& e, const auto& v){ 写(e, v.请求); 写(e, v.结果); });
    写表(出, 值.三分区账, [](auto& e, const auto& v){ 写三请求(e, v.请求); 写多结果(e, v.结果); });
    写表(出, 值.有限N账, [](auto& e, const auto& v){ 写(e, v.请求.组合写入幂等身份); 写组(e, v.请求.参与者写集组, [](auto& x, const auto& y){ 写三参与者(x, y); }); 写多结果(e, v.结果); });
    写可选(出, 值.旧共享所有者, [](auto& e, const auto& v){ 写(e, v); });
    return std::move(出.数据);
}

bool L1事实基座仓库::实现::解码状态(const std::vector<std::uint8_t>& 数据, 状态& 值) {
    解码器 入{数据};
    std::uint64_t 魔数{};
    std::uint32_t 版本{};
    状态 候选;
    if (!入.U64(魔数) || 魔数 != 0x3150414E534C3148ULL || !入.U32(版本) || 版本 != 5
        || !入.U64(候选.下个编码)
        || !读表(入, 候选.所有者, [](auto& d, auto& v){ return 读(d, v); })
        || !读表(入, 候选.节点, [](auto& d, auto& v){ return 读(d, v); })
        || !读表(入, 候选.关系, [](auto& d, auto& v){ return 读(d, v); })
        || !读表(入, 候选.值, [](auto& d, auto& v){ return 读(d, v); })) return false;

    if (!读表(入, 候选.所有者建立账, [](auto& d, auto& v){ return 读(d, v.请求) && 读(d, v.结果); })) return false;
    for (const auto& [键, 记录] : 候选.所有者建立账)
        if (键 == 0 || 记录.请求.建立幂等身份.值 != 键 || 记录.结果.建立幂等身份.值 != 键) return false;

    if (!读表(入, 候选.中性写入账, [](auto& d, auto& v){ return 读(d, v.请求) && 读(d, v.结果); })) return false;
    for (const auto& [键, 记录] : 候选.中性写入账)
        if (键 == 0 || 记录.请求.幂等键.值 != 键 || 记录.结果.幂等键.值 != 键) return false;

    std::uint64_t 所有者数{};
    if (!入.U64(所有者数) || 所有者数 > 入.数据.size() - 入.位置) return false;
    std::uint64_t 前一所有者{};
    for (std::uint64_t i = 0; i != 所有者数; ++i) {
        std::uint64_t 所有者{};
        if (!入.U64(所有者) || 所有者 == 0 || (i && 所有者 <= 前一所有者)
            || !候选.所有者.contains(所有者)) return false;
        auto& 内层 = 候选.所有者写入账[所有者];
        if (!读表(入, 内层, [](auto& d, auto& v){ return 读(d, v.请求) && 读(d, v.结果); })) return false;
        for (const auto& [键, 记录] : 内层)
            if (键 == 0 || 记录.请求.写入幂等身份.值 != 键
                || 记录.结果.写入幂等身份.值 != 键
                || 记录.结果.所有者.编码.值 != 所有者) return false;
        前一所有者 = 所有者;
    }

    if (!读表(入, 候选.跨所有者账, [](auto& d, auto& v){ return 读(d, v.请求) && 读(d, v.结果); })) return false;
    for (const auto& [键, 记录] : 候选.跨所有者账)
        if (键 == 0 || 记录.请求.组合写入幂等身份.值 != 键) return false;

    if (!读表(入, 候选.三分区账, [](auto& d, auto& v){ return 读三请求(d, v.请求) && 读三结果(d, v.结果); })) return false;
    for (const auto& [键, 记录] : 候选.三分区账)
        if (键 == 0 || 记录.请求.组合写入幂等身份.值 != 键 || 记录.结果.组合写入幂等身份.值 != 键) return false;

    if (!读表(入, 候选.有限N账, [](auto& d, auto& v){ return 读(d, v.请求.组合写入幂等身份) && 读组(d, v.请求.参与者写集组, [](auto& x, auto& y){ return 读三参与者(x, y); }) && 读N结果(d, v.结果); })) return false;
    for (const auto& [键, 记录] : 候选.有限N账)
        if (键 == 0 || 记录.请求.组合写入幂等身份.值 != 键 || 记录.结果.组合写入幂等身份.值 != 键) return false;

    if (!读可选(入, 候选.旧共享所有者, [](auto& d, auto& v){ return 读(d, v); })
        || !入.完结() || !状态完整(候选)) return false;
    if (候选.旧共享所有者 && !当前所有者有效(候选, *候选.旧共享所有者)) return false;
    std::swap(值, 候选);
    return true;
}

bool L1事实基座仓库::实现::SHA256(
    const std::vector<std::uint8_t>& 数据,
    std::array<std::uint8_t, 32>& 摘要) noexcept {
    BCRYPT_ALG_HANDLE 算法{};
    BCRYPT_HASH_HANDLE 哈希{};
    std::vector<std::uint8_t> 对象;
    DWORD 对象长度{}, 已取{};
    bool 成功 = false;
    if (BCryptOpenAlgorithmProvider(&算法, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0) return false;
    if (BCryptGetProperty(算法, BCRYPT_OBJECT_LENGTH,
            reinterpret_cast<PUCHAR>(&对象长度), sizeof(对象长度), &已取, 0) >= 0) {
        try { 对象.resize(对象长度); } catch (...) { BCryptCloseAlgorithmProvider(算法, 0); return false; }
        if (BCryptCreateHash(算法, &哈希, 对象.data(), 对象长度, nullptr, 0, 0) >= 0) {
            const auto* 当前位置 = 数据.data();
            std::size_t 剩余 = 数据.size();
            NTSTATUS 状态 = 0;
            while (剩余 && 状态 >= 0) {
                const auto 长度 = static_cast<ULONG>((std::min)(剩余,
                    static_cast<std::size_t>((std::numeric_limits<ULONG>::max)())));
                状态 = BCryptHashData(哈希, const_cast<PUCHAR>(当前位置), 长度, 0);
                当前位置 += 长度; 剩余 -= 长度;
            }
            成功 = 状态 >= 0 && BCryptFinishHash(哈希, 摘要.data(),
                static_cast<ULONG>(摘要.size()), 0) >= 0;
        }
    }
    if (哈希) BCryptDestroyHash(哈希);
    BCryptCloseAlgorithmProvider(算法, 0);
    return 成功;
}

bool L1事实基座仓库::实现::读文件(
    const std::filesystem::path& 路径, std::vector<std::uint8_t>& 数据) noexcept {
    HANDLE 文件 = CreateFileW(路径.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (文件 == INVALID_HANDLE_VALUE) return false;
    LARGE_INTEGER 大小{};
    bool 成功 = GetFileSizeEx(文件, &大小) && 大小.QuadPart >= 0
        && 大小.QuadPart <= static_cast<LONGLONG>((std::numeric_limits<std::uint32_t>::max)());
    if (成功) try { 数据.resize(static_cast<std::size_t>(大小.QuadPart)); } catch (...) { 成功 = false; }
    std::size_t 偏移 = 0;
    while (成功 && 偏移 < 数据.size()) {
        DWORD 已读{};
        const DWORD 要读 = static_cast<DWORD>((std::min)(数据.size() - 偏移, static_cast<std::size_t>(1u << 30)));
        if (!ReadFile(文件, 数据.data() + 偏移, 要读, &已读, nullptr) || 已读 != 要读) 成功 = false;
        偏移 += 已读;
    }
    CloseHandle(文件);
    return 成功;
}

bool L1事实基座仓库::实现::写文件并刷新(
    const std::filesystem::path& 路径, const std::vector<std::uint8_t>& 数据) noexcept {
    HANDLE 文件 = CreateFileW(路径.c_str(), GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (文件 == INVALID_HANDLE_VALUE) return false;
    bool 成功 = true;
    std::size_t 偏移 = 0;
    while (成功 && 偏移 < 数据.size()) {
        DWORD 已写{};
        const DWORD 要写 = static_cast<DWORD>((std::min)(数据.size() - 偏移, static_cast<std::size_t>(1u << 30)));
        if (!WriteFile(文件, 数据.data() + 偏移, 要写, &已写, nullptr) || 已写 != 要写) 成功 = false;
        偏移 += 已写;
    }
    if (成功) 成功 = FlushFileBuffers(文件) != 0;
    CloseHandle(文件);
    return 成功;
}

std::vector<std::uint8_t> L1事实基座仓库::实现::编码清单(
    std::uint64_t 序号, std::uint8_t 槽, std::uint64_t 长度,
    const std::array<std::uint8_t, 32>& 摘要) {
    编码器 出;
    出.U64(0x31464E414D314C48ULL);
    出.U32(2);
    出.U64(序号);
    出.U8(槽);
    出.U64(长度);
    for (const auto 字节 : 摘要) 出.U8(字节);
    return std::move(出.数据);
}

bool L1事实基座仓库::实现::解码清单(
    const std::vector<std::uint8_t>& 数据, std::uint64_t& 序号,
    std::uint8_t& 槽, std::uint64_t& 长度,
    std::array<std::uint8_t, 32>& 摘要) {
    解码器 入{数据};
    std::uint64_t 魔数{};
    std::uint32_t 版本{};
    if (!入.U64(魔数) || 魔数 != 0x31464E414D314C48ULL
        || !入.U32(版本) || 版本 != 2 || !入.U64(序号) || 序号 == 0
        || !入.U8(槽) || (槽 != 1 && 槽 != 2) || !入.U64(长度) || 长度 == 0) return false;
    for (auto& 字节 : 摘要) if (!入.U8(字节)) return false;
    return 入.完结();
}

bool L1事实基座仓库::实现::准备持久发布(const 状态& 候选) noexcept {
    if (!持久) return true;
    if (持久->已毒化) { 当前.隔离 = true; return false; }
    try {
        if (!状态完整(候选)) return false;
        auto 载荷 = 编码状态(候选);
        std::array<std::uint8_t, 32> 摘要{};
        if (!SHA256(载荷, 摘要)) return false;
        const std::uint8_t 新槽 = 持久->活动槽 == 1 ? 2 : 1;
        const auto 槽临时 = 持久->根 / L"snapshot.tmp";
        if (!写文件并刷新(槽临时, 载荷)) return false;
        const auto 新槽路径 = 持久->根 / (新槽 == 1 ? L"snapshot-a.bin" : L"snapshot-b.bin");
        if (!MoveFileExW(槽临时.c_str(), 新槽路径.c_str(),
                MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
            持久->已毒化 = true; 当前.隔离 = true; return false;
        }
        const auto 新序号 = 持久->快照序号 + 1;
        if (新序号 == 0) { 持久->已毒化 = true; 当前.隔离 = true; return false; }
        const auto 清单 = 编码清单(新序号, 新槽,
            static_cast<std::uint64_t>(载荷.size()), 摘要);
        const auto 清单临时 = 持久->根 / L"manifest.tmp";
        if (!写文件并刷新(清单临时, 清单)) return false;
        if (!MoveFileExW(清单临时.c_str(), (持久->根 / L"manifest.bin").c_str(),
                MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
            持久->已毒化 = true; 当前.隔离 = true; return false;
        }
        持久->快照序号 = 新序号;
        持久->活动槽 = 新槽;
        return true;
    } catch (...) { return false; }
}

L1事实基座核心持久恢复结果 L1事实基座仓库::实现::初始化持久恢复(
    const std::filesystem::path& 根) noexcept {
    const auto 失败 = [](L1事实基座核心持久恢复状态 状态值) {
        return L1事实基座核心持久恢复结果{状态值, std::nullopt};
    };
    if (根.empty() || !根.is_absolute() || 根 != 根.lexically_normal())
        return 失败(L1事实基座核心持久恢复状态::入口拒绝);
    try {
        std::unique_lock 锁定(锁);
        if (持久 || 当前.下个编码 != 1 || !当前.所有者.empty() || !当前.节点.empty()
            || !当前.关系.empty() || !当前.值.empty() || !当前.所有者建立账.empty()
            || !当前.中性写入账.empty() || !当前.所有者写入账.empty()
            || !当前.跨所有者账.empty() || !当前.三分区账.empty()
            || !当前.有限N账.empty() || 当前.旧共享所有者 || 当前.隔离)
            return 失败(L1事实基座核心持久恢复状态::内部不一致);

        std::error_code 错误;
        std::filesystem::create_directories(根, 错误);
        if (错误) return 失败(L1事实基座核心持久恢复状态::资源失败);
        auto 会话 = std::make_unique<持久会话>();
        会话->根 = 根;
        会话->锁文件 = CreateFileW((根 / L"session.lock").c_str(),
            GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, nullptr);
        if (会话->锁文件 == INVALID_HANDLE_VALUE)
            return 失败(GetLastError() == ERROR_SHARING_VIOLATION
                ? L1事实基座核心持久恢复状态::存储占用
                : L1事实基座核心持久恢复状态::资源失败);

        const auto 清单路径 = 根 / L"manifest.bin";
        const auto 槽A = 根 / L"snapshot-a.bin";
        const auto 槽B = 根 / L"snapshot-b.bin";
        const bool 有清单 = std::filesystem::exists(清单路径, 错误);
        if (错误) return 失败(L1事实基座核心持久恢复状态::资源失败);
        const bool 有A = std::filesystem::exists(槽A, 错误);
        if (错误) return 失败(L1事实基座核心持久恢复状态::资源失败);
        const bool 有B = std::filesystem::exists(槽B, 错误);
        if (错误) return 失败(L1事实基座核心持久恢复状态::资源失败);
        if (!有清单) {
            if (有A || 有B) return 失败(L1事实基座核心持久恢复状态::材料不完整);
            持久 = std::move(会话);
            return 失败(L1事实基座核心持久恢复状态::已建立空仓);
        }

        std::vector<std::uint8_t> 清单;
        if (!读文件(清单路径, 清单)) return 失败(L1事实基座核心持久恢复状态::资源失败);
        std::uint64_t 序号{}, 长度{};
        std::uint8_t 活动槽{};
        std::array<std::uint8_t, 32> 摘要{};
        if (!解码清单(清单, 序号, 活动槽, 长度, 摘要))
            return 失败(L1事实基座核心持久恢复状态::格式不支持);
        const auto 活动路径 = 活动槽 == 1 ? 槽A : 槽B;
        if (!std::filesystem::exists(活动路径, 错误) || 错误)
            return 失败(错误 ? L1事实基座核心持久恢复状态::资源失败
                : L1事实基座核心持久恢复状态::材料不完整);
        std::vector<std::uint8_t> 载荷;
        if (!读文件(活动路径, 载荷)) return 失败(L1事实基座核心持久恢复状态::资源失败);
        if (载荷.size() != 长度) return 失败(L1事实基座核心持久恢复状态::摘要不一致);
        std::array<std::uint8_t, 32> 实际摘要{};
        if (!SHA256(载荷, 实际摘要)) return 失败(L1事实基座核心持久恢复状态::资源失败);
        if (实际摘要 != 摘要) return 失败(L1事实基座核心持久恢复状态::摘要不一致);

        解码器 版本读取{载荷};
        std::uint64_t 魔数{};
        std::uint32_t 格式版本{};
        if (!版本读取.U64(魔数) || 魔数 != 0x3150414E534C3148ULL
            || !版本读取.U32(格式版本) || 格式版本 != 5)
            return 失败(L1事实基座核心持久恢复状态::格式不支持);
        状态 候选;
        if (!解码状态(载荷, 候选))
            return 失败(L1事实基座核心持久恢复状态::编码或所有者冲突);
        会话->快照序号 = 序号;
        会话->活动槽 = 活动槽;
        std::swap(当前, 候选);
        持久 = std::move(会话);
        return {L1事实基座核心持久恢复状态::已恢复,
            L1事实基座核心持久恢复见证{5, 序号, 摘要}};
    } catch (const std::bad_alloc&) {
        return 失败(L1事实基座核心持久恢复状态::资源失败);
    } catch (const std::filesystem::filesystem_error&) {
        return 失败(L1事实基座核心持久恢复状态::资源失败);
    } catch (...) {
        return 失败(L1事实基座核心持久恢复状态::内部不一致);
    }
}

L1事实基座仓库::L1事实基座仓库() : 实现_(std::make_unique<实现>()) {}
L1事实基座仓库::~L1事实基座仓库() = default;
L1事实基座仓库::L1事实基座仓库(L1事实基座仓库&&) noexcept = default;
L1事实基座仓库& L1事实基座仓库::operator=(L1事实基座仓库&&) noexcept = default;

L1事实基座核心持久恢复结果 L1事实基座仓库::初始化持久恢复(
    const std::filesystem::path& 绝对受控根) noexcept {
    return 实现_->初始化持久恢复(绝对受控根);
}

L1所有者范围建立结果 L1事实基座仓库::建立所有者范围(
    const L1所有者范围建立请求& 请求) {
    L1所有者范围建立结果 失败;
    失败.建立幂等身份 = 请求.建立幂等身份;
    失败.状态 = L1所有者范围管理状态::入口拒绝;
    失败.重试边界 = L1所有者范围重试边界::修正请求后可重试;
    if (!有效(请求.建立幂等身份)
        || 请求.范围种类 != L1所有者范围种类::独占结构范围) return 失败;
    try {
        std::unique_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            失败.状态 = L1所有者范围管理状态::内部不一致;
            return 失败;
        }
        if (const auto it = 实现_->当前.所有者建立账.find(请求.建立幂等身份.值);
            it != 实现_->当前.所有者建立账.end()) {
            if (it->second.请求 != 请求) {
                失败.状态 = L1所有者范围管理状态::幂等冲突;
                return 失败;
            }
            auto 结果 = it->second.结果;
            结果.状态 = L1所有者范围管理状态::精确重复;
            结果.是否形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原幂等身份读回收敛;
            return 结果;
        }
        auto 候选 = 实现_->当前;
        const auto 编码 = 实现::分配(候选);
        if (!编码) {
            失败.状态 = L1所有者范围管理状态::资源失败;
            失败.重试边界 = L1所有者范围重试边界::原请求可重试;
            return 失败;
        }
        const L1结构所有者事实 事实{{*编码}, 请求.范围种类};
        候选.所有者.emplace(编码->值, 事实);
        L1所有者范围建立结果 结果;
        结果.状态 = L1所有者范围管理状态::成功;
        结果.建立幂等身份 = 请求.建立幂等身份;
        结果.所有者事实 = 事实;
        结果.是否形成内存权威发布 = true;
        结果.重试边界 = L1所有者范围重试边界::不适用;
        候选.所有者建立账.emplace(请求.建立幂等身份.值,
            实现::所有者建立记录{请求, 结果});
        if (!实现::状态完整(候选)) {
            失败.状态 = L1所有者范围管理状态::内部不一致;
            return 失败;
        }
        if (!实现_->准备持久发布(候选)) {
            失败.状态 = L1所有者范围管理状态::资源失败;
            失败.重试边界 = L1所有者范围重试边界::原请求可重试;
            return 失败;
        }
        std::swap(实现_->当前, 候选);
        return 结果;
    } catch (const std::bad_alloc&) {
        失败.状态 = L1所有者范围管理状态::资源失败;
        失败.重试边界 = L1所有者范围重试边界::原请求可重试;
        return 失败;
    } catch (...) {
        失败.状态 = L1所有者范围管理状态::内部不一致;
        return 失败;
    }
}

L1所有者范围重入结果 L1事实基座仓库::验证所有者范围重入(
    const L1所有者范围重入请求& 请求) const {
    L1所有者范围重入结果 结果;
    结果.建立幂等身份 = 请求.建立幂等身份;
    结果.重试边界 = L1所有者范围重试边界::修正请求后可重试;
    if (!有效(请求.所有者) || !有效(请求.建立幂等身份)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            结果.状态 = L1所有者范围管理状态::内部不一致;
            return 结果;
        }
        const auto 建立 = 实现_->当前.所有者建立账.find(请求.建立幂等身份.值);
        if (建立 == 实现_->当前.所有者建立账.end()) {
            结果.状态 = L1所有者范围管理状态::未找到;
            return 结果;
        }
        if (!建立->second.结果.所有者事实
            || 建立->second.结果.所有者事实->所有者 != 请求.所有者) {
            结果.状态 = L1所有者范围管理状态::幂等冲突;
            return 结果;
        }
        const auto 当前 = 实现_->当前.所有者.find(请求.所有者.编码.值);
        if (当前 == 实现_->当前.所有者.end()) {
            结果.状态 = L1所有者范围管理状态::未找到;
            return 结果;
        }
        if (当前->second != *建立->second.结果.所有者事实) {
            结果.状态 = L1所有者范围管理状态::引用冲突;
            return 结果;
        }
        结果.状态 = L1所有者范围管理状态::成功;
        结果.所有者事实 = 当前->second;
        结果.重试边界 = L1所有者范围重试边界::不适用;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1所有者范围管理状态::资源失败;
        结果.重试边界 = L1所有者范围重试边界::原请求可重试;
        return 结果;
    } catch (...) {
        结果.状态 = L1所有者范围管理状态::内部不一致;
        return 结果;
    }
}

L1所有者范围退出结果 L1事实基座仓库::退出所有者范围(
    const L1所有者范围退出请求& 请求) {
    L1所有者范围退出结果 结果;
    结果.建立幂等身份 = 请求.建立幂等身份;
    结果.重试边界 = L1所有者范围重试边界::修正请求后可重试;
    if (!有效(请求.所有者) || !有效(请求.建立幂等身份)) return 结果;
    try {
        std::unique_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            结果.状态 = L1所有者范围管理状态::内部不一致;
            return 结果;
        }
        const auto 建立 = 实现_->当前.所有者建立账.find(请求.建立幂等身份.值);
        if (建立 == 实现_->当前.所有者建立账.end()) {
            结果.状态 = L1所有者范围管理状态::未找到;
            return 结果;
        }
        if (!建立->second.结果.所有者事实
            || 建立->second.结果.所有者事实->所有者 != 请求.所有者) {
            结果.状态 = L1所有者范围管理状态::幂等冲突;
            return 结果;
        }
        const auto 被持有 = [&](const auto& 表) {
            return std::any_of(表.begin(), 表.end(), [&](const auto& 项) {
                return 项.second.写入所有者 == 请求.所有者;
            });
        };
        if (被持有(实现_->当前.节点) || 被持有(实现_->当前.关系)
            || 被持有(实现_->当前.值)) {
            结果.状态 = L1所有者范围管理状态::引用冲突;
            return 结果;
        }
        auto 候选 = 实现_->当前;
        if (候选.所有者.erase(请求.所有者.编码.值) == 0) {
            结果.状态 = L1所有者范围管理状态::未找到;
            return 结果;
        }
        if (!实现::状态完整(候选)) {
            结果.状态 = L1所有者范围管理状态::内部不一致;
            return 结果;
        }
        if (!实现_->准备持久发布(候选)) {
            结果.状态 = L1所有者范围管理状态::资源失败;
            结果.重试边界 = L1所有者范围重试边界::原请求可重试;
            return 结果;
        }
        std::swap(实现_->当前, 候选);
        结果.状态 = L1所有者范围管理状态::成功;
        结果.是否形成内存权威发布 = true;
        结果.重试边界 = L1所有者范围重试边界::不适用;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1所有者范围管理状态::资源失败;
        结果.重试边界 = L1所有者范围重试边界::原请求可重试;
        return 结果;
    } catch (...) {
        结果.状态 = L1所有者范围管理状态::内部不一致;
        return 结果;
    }
}

L1所有者范围写入结果 L1事实基座仓库::提交所有者范围中性写集(
    L1结构所有者身份 所有者, const L1所有者范围写集请求& 请求) {
    L1所有者范围写入结果 结果;
    结果.所有者 = 所有者;
    结果.写入幂等身份 = 请求.写入幂等身份;
    结果.重试边界 = L1所有者范围重试边界::修正请求后可重试;
    if (!有效(所有者) || !有效(请求.写入幂等身份)) return 结果;
    try {
        std::unique_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            结果.状态 = L1所有者范围写入状态::内部不一致;
            return 结果;
        }
        auto& 账 = 实现_->当前.所有者写入账[所有者.编码.值];
        if (const auto it = 账.find(请求.写入幂等身份.值); it != 账.end()) {
            if (it->second.请求 != 请求) {
                结果.状态 = L1所有者范围写入状态::幂等冲突;
                return 结果;
            }
            结果 = it->second.结果;
            结果.状态 = L1所有者范围写入状态::精确重复;
            结果.是否形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原幂等身份读回收敛;
            return 结果;
        }
        auto 候选 = 实现_->当前;
        std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 映射;
        const auto 状态 = 实现::应用写集(候选, 所有者, 请求, 映射);
        if (状态 != L1所有者范围写入状态::成功) {
            结果.状态 = 状态;
            return 结果;
        }
        结果.状态 = L1所有者范围写入状态::成功;
        结果.是否形成内存权威发布 = true;
        结果.重试边界 = L1所有者范围重试边界::不适用;
        结果.新编码映射 = std::move(映射);
        候选.所有者写入账[所有者.编码.值].emplace(
            请求.写入幂等身份.值, 实现::所有者写入记录{请求, 结果});
        if (!实现_->准备持久发布(候选)) {
            结果.状态 = L1所有者范围写入状态::资源失败;
            结果.是否形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原请求可重试;
            结果.新编码映射.clear();
            return 结果;
        }
        std::swap(实现_->当前, 候选);
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1所有者范围写入状态::资源失败;
        结果.重试边界 = L1所有者范围重试边界::原请求可重试;
        return 结果;
    } catch (...) {
        结果.状态 = L1所有者范围写入状态::内部不一致;
        return 结果;
    }
}

L1所有者范围首次写入读取结果 L1事实基座仓库::读取所有者范围首次写入材料(
    L1结构所有者身份 所有者,
    const L1所有者范围首次写入读取请求& 请求) const {
    L1所有者范围首次写入读取结果 结果;
    结果.所有者 = 所有者;
    结果.写入幂等身份 = 请求.写入幂等身份;
    if (!有效(所有者) || !有效(请求.写入幂等身份)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            return 结果;
        }
        const auto 外 = 实现_->当前.所有者写入账.find(所有者.编码.值);
        if (外 == 实现_->当前.所有者写入账.end()) {
            结果.状态 = L1所有者范围读取状态::未找到;
            return 结果;
        }
        const auto 内 = 外->second.find(请求.写入幂等身份.值);
        if (内 == 外->second.end()) {
            结果.状态 = L1所有者范围读取状态::未找到;
            return 结果;
        }
        结果.状态 = L1所有者范围读取状态::成功;
        结果.首次规范化写集 = 内->second.请求;
        结果.首次写入结果 = 内->second.结果;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1所有者范围读取状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = L1所有者范围读取状态::内部不一致;
        return 结果;
    }
}

L1中性写入结果 L1事实基座仓库::提交中性写集(const L1中性写集请求& 请求) {
    L1中性写入结果 结果;
    结果.幂等键 = 请求.幂等键;
    结果.重试边界 = L1中性重试边界::修正请求后可重试;
    if (!有效(请求.幂等键)) return 结果;
    try {
        std::unique_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            结果.状态 = L1中性写入状态::内部不一致;
            return 结果;
        }
        if (const auto it = 实现_->当前.中性写入账.find(请求.幂等键.值);
            it != 实现_->当前.中性写入账.end()) {
            if (it->second.请求 != 请求) {
                结果.状态 = L1中性写入状态::幂等冲突;
                return 结果;
            }
            结果 = it->second.结果;
            结果.状态 = L1中性写入状态::精确重复;
            结果.是否形成内存权威发布 = false;
            结果.重试边界 = L1中性重试边界::原幂等键读回收敛;
            return 结果;
        }
        auto 候选 = 实现_->当前;
        if (!候选.旧共享所有者) {
            const auto 编码 = 实现::分配(候选);
            if (!编码) {
                结果.状态 = L1中性写入状态::资源失败;
                结果.重试边界 = L1中性重试边界::原请求可重试;
                return 结果;
            }
            候选.旧共享所有者 = L1结构所有者身份{*编码};
            候选.所有者.emplace(编码->值,
                L1结构所有者事实{*候选.旧共享所有者,
                    L1所有者范围种类::旧共享范围});
        }
        L1所有者范围写集请求 所有者请求;
        所有者请求.写入幂等身份 = {请求.幂等键.值};
        for (const auto& 项 : 请求.节点) {
            L1所有者范围节点新建项 新项;
            新项.本地键 = {项.本地键.值};
            新项.种类 = 项.种类;
            if (项.属性类型表示)
                新项.属性类型表示 = static_cast<L1所有者范围值表示种类>(*项.属性类型表示);
            所有者请求.节点.push_back(std::move(新项));
        }
        const auto 转换引用 = [](const L1中性事实引用& 引用)
            -> L1所有者范围事实引用 {
            if (const auto* 编码 = std::get_if<稳定编码>(&引用)) return *编码;
            return L1所有者范围写集本地键{
                std::get<L1中性写集本地键>(引用).值};
        };
        for (const auto& 项 : 请求.关系) {
            所有者请求.关系.push_back({{项.本地键.值}, 转换引用(项.源节点),
                转换引用(项.目标节点), 转换引用(项.关系类型节点),
                项.角色或顺序});
        }
        for (const auto& 项 : 请求.值) {
            L1所有者范围原始值材料 材料 = std::visit([](const auto& 值)
                -> L1所有者范围原始值材料 {
                using T = std::decay_t<decltype(值)>;
                if constexpr (std::is_same_v<T, L1中性独立材料引用>)
                    return L1所有者范围独立材料引用{值.编码};
                else return 值;
            }, 项.材料);
            所有者请求.值.push_back({{项.本地键.值}, 转换引用(项.所属节点),
                转换引用(项.属性类型节点), std::move(材料),
                转换引用(项.来源节点)});
        }
        for (const auto& 项 : 请求.属性槽变更)
            所有者请求.属性槽变更.push_back({转换引用(项.所属节点),
                转换引用(项.属性类型节点), {项.新当前值.值}});
        所有者请求.退出事实 = 请求.退出事实;

        std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 映射;
        const auto 写状态 = 实现::应用写集(
            候选, *候选.旧共享所有者, 所有者请求, 映射);
        switch (写状态) {
        case L1所有者范围写入状态::成功: 结果.状态 = L1中性写入状态::成功; break;
        case L1所有者范围写入状态::未找到: 结果.状态 = L1中性写入状态::未找到; break;
        case L1所有者范围写入状态::资源失败: 结果.状态 = L1中性写入状态::资源失败; break;
        case L1所有者范围写入状态::引用冲突:
        case L1所有者范围写入状态::许可拒绝: 结果.状态 = L1中性写入状态::引用冲突; break;
        default: 结果.状态 = L1中性写入状态::入口拒绝; break;
        }
        if (结果.状态 != L1中性写入状态::成功) return 结果;
        结果.是否形成内存权威发布 = true;
        结果.重试边界 = L1中性重试边界::不适用;
        for (const auto& [本地, 编码] : 映射)
            结果.新编码映射.push_back({{本地.值}, 编码});
        候选.中性写入账.emplace(请求.幂等键.值,
            实现::中性写入记录{请求, 结果});
        if (!实现_->准备持久发布(候选)) {
            结果.状态 = L1中性写入状态::资源失败;
            结果.是否形成内存权威发布 = false;
            结果.重试边界 = L1中性重试边界::原请求可重试;
            结果.新编码映射.clear();
            return 结果;
        }
        std::swap(实现_->当前, 候选);
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1中性写入状态::资源失败;
        结果.重试边界 = L1中性重试边界::原请求可重试;
        return 结果;
    } catch (...) {
        结果.状态 = L1中性写入状态::内部不一致;
        return 结果;
    }
}

L1中性写入首次结果读取结果 L1事实基座仓库::读取中性写入首次结果(
    const L1中性写入首次结果读取请求& 请求) const noexcept {
    L1中性写入首次结果读取结果 结果;
    结果.幂等键 = 请求.幂等键;
    if (!有效(请求.幂等键)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) {
            结果.状态 = L1中性写入首次结果读取状态::内部错误;
            return 结果;
        }
        const auto it = 实现_->当前.中性写入账.find(请求.幂等键.值);
        if (it == 实现_->当前.中性写入账.end()) {
            结果.状态 = L1中性写入首次结果读取状态::未找到;
            return 结果;
        }
        结果.状态 = L1中性写入首次结果读取状态::已读取;
        结果.首次规范请求等价材料 = it->second.请求;
        结果.首次状态 = it->second.结果.状态;
        结果.首次稳定编码映射 = it->second.结果.新编码映射;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1中性写入首次结果读取状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = L1中性写入首次结果读取状态::内部错误;
        return 结果;
    }
}

L1跨所有者原子事务结果 L1事实基座仓库::提交跨所有者原子事务(
    const L1跨所有者原子事务请求& 请求) {
    L1跨所有者原子事务结果 结果;
    结果.重试边界 = L1所有者范围重试边界::修正请求后可重试;
    if (!有效(请求.组合写入幂等身份)
        || 请求.状态写集.参与者 != L1跨所有者原子事务参与者序号::状态
        || 请求.动态写集.参与者 != L1跨所有者原子事务参与者序号::动态
        || 请求.状态写集.所有者 == 请求.动态写集.所有者) return 结果;
    try {
        std::unique_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            结果.状态 = L1跨所有者原子事务状态::内部不一致;
            return 结果;
        }
        if (const auto it = 实现_->当前.跨所有者账.find(请求.组合写入幂等身份.值);
            it != 实现_->当前.跨所有者账.end()) {
            if (it->second.请求 != 请求) {
                结果.状态 = L1跨所有者原子事务状态::幂等冲突;
                return 结果;
            }
            结果 = it->second.结果;
            结果.状态 = L1跨所有者原子事务状态::精确重复;
            结果.是否形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原幂等身份读回收敛;
            return 结果;
        }
        const auto 转换参与者 = [](const L1跨所有者原子参与者写集& 原,
            std::uint8_t 身份) -> std::optional<L1三分区原子参与者写集> {
            L1三分区原子参与者写集 新;
            新.参与者 = {身份};
            新.所有者 = 原.所有者;
            新.写集.写入幂等身份 = 原.写集.写入幂等身份;
            const auto 转换引用 = [](const L1跨所有者原子事实引用值& 引用)
                -> L1三分区原子事实引用值 {
                if (const auto* 编码 = std::get_if<稳定编码>(&引用)) return *编码;
                if (const auto* 本地 = std::get_if<L1所有者范围写集本地键>(&引用))
                    return *本地;
                const auto& 跨 = std::get<L1跨所有者原子事实引用>(引用);
                return L1三分区原子事实引用{
                    {static_cast<std::uint8_t>(跨.参与者)}, 跨.本地键};
            };
            for (const auto& 项 : 原.写集.节点)
                新.写集.节点.push_back({项.本地键, 项.种类, 项.属性类型表示});
            for (const auto& 项 : 原.写集.关系)
                新.写集.关系.push_back({项.本地键, 转换引用(项.源节点),
                    转换引用(项.目标节点), 转换引用(项.关系类型节点),
                    项.角色或顺序});
            for (const auto& 项 : 原.写集.值)
                新.写集.值.push_back({项.本地键, 转换引用(项.所属节点),
                    转换引用(项.属性类型节点), 项.材料,
                    转换引用(项.来源节点)});
            for (const auto& 项 : 原.写集.属性槽变更)
                新.写集.属性槽变更.push_back({转换引用(项.所属节点),
                    转换引用(项.属性类型节点), 项.新当前值});
            新.写集.退出事实 = 原.写集.退出事实;
            return 新;
        };
        std::vector<L1三分区原子参与者写集> 参与者组;
        参与者组.push_back(*转换参与者(请求.状态写集, 1));
        参与者组.push_back(*转换参与者(请求.动态写集, 2));
        auto 候选 = 实现_->当前;
        std::vector<L1三分区原子参与者结果> 参与者结果;
        L1所有者范围写入状态 失败状态{};
        if (!实现::应用多分区写集(候选, 参与者组, 参与者结果, 失败状态)) {
            if (失败状态 == L1所有者范围写入状态::资源失败)
                结果.状态 = L1跨所有者原子事务状态::资源失败;
            else if (失败状态 == L1所有者范围写入状态::许可拒绝)
                结果.状态 = L1跨所有者原子事务状态::许可拒绝;
            else if (失败状态 == L1所有者范围写入状态::入口拒绝)
                结果.状态 = L1跨所有者原子事务状态::入口拒绝;
            else 结果.状态 = L1跨所有者原子事务状态::引用冲突;
            return 结果;
        }
        结果.状态 = L1跨所有者原子事务状态::已提交;
        结果.是否形成内存权威发布 = true;
        结果.重试边界 = L1所有者范围重试边界::不适用;
        结果.状态编码映射 = 参与者结果[0].新编码映射;
        结果.动态编码映射 = 参与者结果[1].新编码映射;
        候选.跨所有者账.emplace(请求.组合写入幂等身份.值,
            实现::跨所有者记录{请求, 结果});
        if (!实现_->准备持久发布(候选)) {
            结果.状态 = L1跨所有者原子事务状态::资源失败;
            结果.是否形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原请求可重试;
            结果.状态编码映射.clear();
            结果.动态编码映射.clear();
            return 结果;
        }
        std::swap(实现_->当前, 候选);
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1跨所有者原子事务状态::资源失败;
        结果.重试边界 = L1所有者范围重试边界::原请求可重试;
        return 结果;
    } catch (...) {
        结果.状态 = L1跨所有者原子事务状态::内部不一致;
        return 结果;
    }
}

L1三分区原子事务结果 L1事实基座仓库::提交三分区原子事务(
    const L1三分区原子事务请求& 请求) noexcept {
    L1三分区原子事务结果 结果;
    结果.组合写入幂等身份 = 请求.组合写入幂等身份;
    结果.重试边界 = L1所有者范围重试边界::修正请求后可重试;
    if (!有效(请求.组合写入幂等身份) || 请求.参与者写集组.size() != 3)
        return 结果;
    try {
        std::unique_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            结果.状态 = L1三分区原子事务状态::内部不一致;
            return 结果;
        }
        if (const auto it = 实现_->当前.三分区账.find(请求.组合写入幂等身份.值);
            it != 实现_->当前.三分区账.end()) {
            if (it->second.请求 != 请求) {
                结果.状态 = L1三分区原子事务状态::幂等冲突;
                return 结果;
            }
            结果 = it->second.结果;
            结果.状态 = L1三分区原子事务状态::精确重复;
            结果.是否已确认形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原幂等身份读回收敛;
            return 结果;
        }
        auto 候选 = 实现_->当前;
        L1所有者范围写入状态 失败状态{};
        if (!实现::应用多分区写集(
                候选, 请求.参与者写集组, 结果.参与者结果组, 失败状态)) {
            结果.状态 = 失败状态 == L1所有者范围写入状态::资源失败
                ? L1三分区原子事务状态::资源失败
                : 失败状态 == L1所有者范围写入状态::入口拒绝
                    ? L1三分区原子事务状态::入口拒绝
                    : L1三分区原子事务状态::引用冲突;
            结果.参与者结果组.clear();
            return 结果;
        }
        结果.状态 = L1三分区原子事务状态::已提交;
        结果.是否已确认形成内存权威发布 = true;
        结果.重试边界 = L1所有者范围重试边界::不适用;
        候选.三分区账.emplace(请求.组合写入幂等身份.值,
            实现::三分区记录{请求, 结果});
        if (!实现_->准备持久发布(候选)) {
            结果.状态 = L1三分区原子事务状态::资源失败;
            结果.是否已确认形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原请求可重试;
            结果.参与者结果组.clear();
            return 结果;
        }
        std::swap(实现_->当前, 候选);
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1三分区原子事务状态::资源失败;
        结果.重试边界 = L1所有者范围重试边界::原请求可重试;
        return 结果;
    } catch (...) {
        结果.状态 = L1三分区原子事务状态::内部不一致;
        return 结果;
    }
}

L1有限N分区原子事务结果 L1事实基座仓库::提交有限N分区原子事务(
    const L1有限N分区原子事务请求& 请求) noexcept {
    L1有限N分区原子事务结果 结果;
    结果.组合写入幂等身份 = 请求.组合写入幂等身份;
    结果.重试边界 = L1所有者范围重试边界::修正请求后可重试;
    if (!有效(请求.组合写入幂等身份)
        || 请求.参与者写集组.size() < L1有限N分区原子事务最小参与者数
        || 请求.参与者写集组.size() > L1有限N分区原子事务最大参与者数)
        return 结果;
    try {
        std::unique_lock 锁(实现_->锁);
        if (实现_->当前.隔离 || !实现::状态完整(实现_->当前)) {
            结果.状态 = L1有限N分区原子事务状态::内部不一致;
            return 结果;
        }
        if (const auto it = 实现_->当前.有限N账.find(请求.组合写入幂等身份.值);
            it != 实现_->当前.有限N账.end()) {
            if (it->second.请求 != 请求) {
                结果.状态 = L1有限N分区原子事务状态::幂等冲突;
                return 结果;
            }
            结果 = it->second.结果;
            结果.状态 = L1有限N分区原子事务状态::精确重复;
            结果.是否已确认形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原幂等身份读回收敛;
            return 结果;
        }
        auto 候选 = 实现_->当前;
        L1所有者范围写入状态 失败状态{};
        if (!实现::应用多分区写集(
                候选, 请求.参与者写集组, 结果.参与者结果组, 失败状态)) {
            结果.状态 = 失败状态 == L1所有者范围写入状态::资源失败
                ? L1有限N分区原子事务状态::资源失败
                : 失败状态 == L1所有者范围写入状态::入口拒绝
                    ? L1有限N分区原子事务状态::入口拒绝
                    : L1有限N分区原子事务状态::引用冲突;
            结果.参与者结果组.clear();
            return 结果;
        }
        结果.状态 = L1有限N分区原子事务状态::已提交;
        结果.是否已确认形成内存权威发布 = true;
        结果.重试边界 = L1所有者范围重试边界::不适用;
        候选.有限N账.emplace(请求.组合写入幂等身份.值,
            实现::有限N记录{请求, 结果});
        if (!实现_->准备持久发布(候选)) {
            结果.状态 = L1有限N分区原子事务状态::资源失败;
            结果.是否已确认形成内存权威发布 = false;
            结果.重试边界 = L1所有者范围重试边界::原请求可重试;
            结果.参与者结果组.clear();
            return 结果;
        }
        std::swap(实现_->当前, 候选);
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1有限N分区原子事务状态::资源失败;
        结果.重试边界 = L1所有者范围重试边界::原请求可重试;
        return 结果;
    } catch (...) {
        结果.状态 = L1有限N分区原子事务状态::内部不一致;
        return 结果;
    }
}

L1读取结果 L1事实基座仓库::读取当前节点(稳定编码 编码) const {
    if (!有效(编码)) return {};
    std::shared_lock 锁(实现_->锁);
    if (实现_->当前.隔离) return {L1读取状态::内部不一致, std::nullopt};
    const auto it = 实现_->当前.节点.find(编码.值);
    return it == 实现_->当前.节点.end()
        ? L1读取结果{L1读取状态::未找到, std::nullopt}
        : L1读取结果{L1读取状态::成功, L1事实副本{it->second}};
}

L1读取结果 L1事实基座仓库::读取当前关系(稳定编码 编码) const {
    if (!有效(编码)) return {};
    std::shared_lock 锁(实现_->锁);
    if (实现_->当前.隔离) return {L1读取状态::内部不一致, std::nullopt};
    const auto it = 实现_->当前.关系.find(编码.值);
    return it == 实现_->当前.关系.end()
        ? L1读取结果{L1读取状态::未找到, std::nullopt}
        : L1读取结果{L1读取状态::成功, L1事实副本{it->second}};
}

L1读取结果 L1事实基座仓库::读取当前值(稳定编码 编码) const {
    if (!有效(编码)) return {};
    std::shared_lock 锁(实现_->锁);
    if (实现_->当前.隔离) return {L1读取状态::内部不一致, std::nullopt};
    const auto it = 实现_->当前.值.find(编码.值);
    return it == 实现_->当前.值.end()
        ? L1读取结果{L1读取状态::未找到, std::nullopt}
        : L1读取结果{L1读取状态::成功, L1事实副本{it->second}};
}

L1属性读取结果 L1事实基座仓库::读取当前属性(
    稳定编码 节点, 稳定编码 类型) const {
    L1属性读取结果 结果;
    if (!有效(节点) || !有效(类型)) return 结果;
    std::shared_lock 锁(实现_->锁);
    if (实现_->当前.隔离) {
        结果.状态 = L1读取状态::内部不一致;
        return 结果;
    }
    const auto it = 实现_->当前.节点.find(节点.值);
    if (it == 实现_->当前.节点.end()) {
        结果.状态 = L1读取状态::未找到;
        return 结果;
    }
    const auto 槽 = std::lower_bound(it->second.当前属性.begin(), it->second.当前属性.end(),
        类型, [](const 属性槽& 左, 稳定编码 右) { return 左.属性类型节点 < 右; });
    if (槽 == it->second.当前属性.end() || 槽->属性类型节点 != 类型) {
        结果.状态 = L1读取状态::属性未设置;
        return 结果;
    }
    结果.状态 = L1读取状态::成功;
    结果.属性 = L1属性读取副本{节点, 类型, 槽->当前值};
    return 结果;
}

L1中性源关系读取结果 L1事实基座仓库::读取中性当前源关系组(
    const L1中性源关系读取请求& 请求) const {
    L1中性源关系读取结果 结果;
    结果.源节点 = 请求.源节点;
    结果.关系类型节点 = 请求.关系类型节点;
    if (!有效(请求.源节点) || !有效(请求.关系类型节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        if (!实现_->当前.节点.contains(请求.源节点.值)
            || !实现_->当前.节点.contains(请求.关系类型节点.值)) {
            结果.状态 = L1中性读取状态::未找到;
            return 结果;
        }
        for (const auto& [_, 关系] : 实现_->当前.关系)
            if (关系.源节点 == 请求.源节点
                && 关系.关系类型节点 == 请求.关系类型节点)
                结果.关系组.push_back(转换中性关系(关系));
        按编码排序(结果.关系组);
        结果.状态 = L1中性读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1中性读取状态::资源失败;
        结果.关系组.clear();
        return 结果;
    } catch (...) {
        结果.状态 = L1中性读取状态::内部不一致;
        结果.关系组.clear();
        return 结果;
    }
}

L1中性目标关系读取结果 L1事实基座仓库::读取中性当前目标关系组(
    const L1中性目标关系读取请求& 请求) const {
    L1中性目标关系读取结果 结果;
    结果.目标节点 = 请求.目标节点;
    结果.关系类型节点 = 请求.关系类型节点;
    if (!有效(请求.目标节点) || !有效(请求.关系类型节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        if (!实现_->当前.节点.contains(请求.目标节点.值)
            || !实现_->当前.节点.contains(请求.关系类型节点.值)) {
            结果.状态 = L1中性读取状态::未找到;
            return 结果;
        }
        for (const auto& [_, 关系] : 实现_->当前.关系)
            if (关系.目标节点 == 请求.目标节点
                && 关系.关系类型节点 == 请求.关系类型节点)
                结果.关系组.push_back(转换中性关系(关系));
        按编码排序(结果.关系组);
        结果.状态 = L1中性读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = L1中性读取状态::资源失败;
        结果.关系组.clear();
        return 结果;
    } catch (...) {
        结果.状态 = L1中性读取状态::内部不一致;
        结果.关系组.clear();
        return 结果;
    }
}

L1所有者范围当前读取结果 L1事实基座仓库::读取所有者范围当前节点(
    const L1所有者范围事实读取请求& 请求) const {
    L1所有者范围当前读取结果 结果;
    结果.查询编码 = 请求.编码;
    if (!有效(请求.编码)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            return 结果;
        }
        const auto it = 实现_->当前.节点.find(请求.编码.值);
        if (it == 实现_->当前.节点.end()) {
            结果.状态 = L1所有者范围读取状态::未找到;
            return 结果;
        }
        结果.状态 = L1所有者范围读取状态::成功;
        结果.事实 = 转换节点(it->second);
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围读取状态::资源失败; return 结果; }
    catch (...) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
}

L1所有者范围当前读取结果 L1事实基座仓库::读取所有者范围当前关系(
    const L1所有者范围事实读取请求& 请求) const {
    L1所有者范围当前读取结果 结果;
    结果.查询编码 = 请求.编码;
    if (!有效(请求.编码)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
        const auto it = 实现_->当前.关系.find(请求.编码.值);
        if (it == 实现_->当前.关系.end()) { 结果.状态 = L1所有者范围读取状态::未找到; return 结果; }
        结果.状态 = L1所有者范围读取状态::成功;
        结果.事实 = 转换关系(it->second);
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围读取状态::资源失败; return 结果; }
    catch (...) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
}

L1所有者范围当前读取结果 L1事实基座仓库::读取所有者范围当前值(
    const L1所有者范围事实读取请求& 请求) const {
    L1所有者范围当前读取结果 结果;
    结果.查询编码 = 请求.编码;
    if (!有效(请求.编码)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
        const auto it = 实现_->当前.值.find(请求.编码.值);
        if (it == 实现_->当前.值.end()) { 结果.状态 = L1所有者范围读取状态::未找到; return 结果; }
        结果.状态 = L1所有者范围读取状态::成功;
        结果.事实 = 转换值(it->second);
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围读取状态::资源失败; return 结果; }
    catch (...) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
}

L1所有者范围当前事实读取结果 L1事实基座仓库::读取所有者范围当前事实(
    const L1所有者范围当前事实读取请求& 请求) const noexcept {
    L1所有者范围当前事实读取结果 结果;
    结果.所有者 = 请求.所有者;
    结果.事实编码 = 请求.事实编码;
    if (!有效(请求.所有者) || !有效(请求.事实编码)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围当前事实读取状态::内部不一致; return 结果; }
        if (!实现::当前所有者有效(实现_->当前, 请求.所有者)) {
            结果.状态 = L1所有者范围当前事实读取状态::未找到;
            return 结果;
        }
        if (const auto it = 实现_->当前.节点.find(请求.事实编码.值);
            it != 实现_->当前.节点.end() && it->second.写入所有者 == 请求.所有者)
            结果.载荷 = 转换节点(it->second);
        else if (const auto it = 实现_->当前.关系.find(请求.事实编码.值);
            it != 实现_->当前.关系.end() && it->second.写入所有者 == 请求.所有者)
            结果.载荷 = 转换关系(it->second);
        else if (const auto it = 实现_->当前.值.find(请求.事实编码.值);
            it != 实现_->当前.值.end() && it->second.写入所有者 == 请求.所有者)
            结果.载荷 = 转换值(it->second);
        else {
            结果.状态 = L1所有者范围当前事实读取状态::未找到;
            return 结果;
        }
        结果.状态 = L1所有者范围当前事实读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围当前事实读取状态::资源失败; return 结果; }
    catch (...) { 结果.状态 = L1所有者范围当前事实读取状态::内部不一致; return 结果; }
}

L1所有者范围所属节点当前完整值组读取结果
L1事实基座仓库::读取所有者范围所属节点当前完整值组(
    const L1所有者范围所属节点当前完整值组读取请求& 请求) const noexcept {
    L1所有者范围所属节点当前完整值组读取结果 结果;
    结果.所有者 = 请求.所有者; 结果.所属节点 = 请求.所属节点;
    if (!有效(请求.所有者) || !有效(请求.所属节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围所属节点当前完整值组读取状态::内部不一致; return 结果; }
        const auto 节点 = 实现_->当前.节点.find(请求.所属节点.值);
        if (!实现::当前所有者有效(实现_->当前, 请求.所有者)
            || 节点 == 实现_->当前.节点.end()
            || 节点->second.写入所有者 != 请求.所有者) {
            结果.状态 = L1所有者范围所属节点当前完整值组读取状态::未找到;
            return 结果;
        }
        for (const auto& [_, 值] : 实现_->当前.值)
            if (值.写入所有者 == 请求.所有者 && 值.所属节点 == 请求.所属节点)
                结果.载荷.push_back(转换值(值));
        按编码排序(结果.载荷);
        结果.状态 = L1所有者范围所属节点当前完整值组读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围所属节点当前完整值组读取状态::资源失败; 结果.载荷.clear(); return 结果; }
    catch (...) { 结果.状态 = L1所有者范围所属节点当前完整值组读取状态::内部不一致; 结果.载荷.clear(); return 结果; }
}

L1所有者范围来源当前完整值组读取结果
L1事实基座仓库::读取所有者范围来源当前完整值组(
    const L1所有者范围来源当前完整值组读取请求& 请求) const noexcept {
    L1所有者范围来源当前完整值组读取结果 结果;
    结果.所有者 = 请求.所有者; 结果.来源节点 = 请求.来源节点;
    if (!有效(请求.所有者) || !有效(请求.来源节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围来源当前完整值组读取状态::内部不一致; return 结果; }
        if (!实现::当前所有者有效(实现_->当前, 请求.所有者)
            || !实现_->当前.节点.contains(请求.来源节点.值)) {
            结果.状态 = L1所有者范围来源当前完整值组读取状态::未找到;
            return 结果;
        }
        for (const auto& [_, 值] : 实现_->当前.值)
            if (值.写入所有者 == 请求.所有者 && 值.来源节点 == 请求.来源节点)
                结果.当前值.push_back(转换值(值));
        按编码排序(结果.当前值);
        结果.状态 = L1所有者范围来源当前完整值组读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围来源当前完整值组读取状态::资源失败; 结果.当前值.clear(); return 结果; }
    catch (...) { 结果.状态 = L1所有者范围来源当前完整值组读取状态::内部不一致; 结果.当前值.clear(); return 结果; }
}

L1所有者范围属性类型当前完整值组读取结果
L1事实基座仓库::读取所有者范围属性类型当前完整值组(
    const L1所有者范围属性类型当前完整值组读取请求& 请求) const noexcept {
    L1所有者范围属性类型当前完整值组读取结果 结果;
    结果.所有者 = 请求.所有者; 结果.属性类型节点 = 请求.属性类型节点;
    if (!有效(请求.所有者) || !有效(请求.属性类型节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围属性类型当前完整值组读取状态::内部不一致; return 结果; }
        const auto 类型 = 实现_->当前.节点.find(请求.属性类型节点.值);
        if (!实现::当前所有者有效(实现_->当前, 请求.所有者)
            || 类型 == 实现_->当前.节点.end()
            || 类型->second.写入所有者 != 请求.所有者
            || 类型->second.种类 != 节点种类::属性类型) {
            结果.状态 = L1所有者范围属性类型当前完整值组读取状态::未找到;
            return 结果;
        }
        for (const auto& [_, 值] : 实现_->当前.值)
            if (值.写入所有者 == 请求.所有者
                && 值.属性类型节点 == 请求.属性类型节点)
                结果.当前值.push_back(转换值(值));
        按编码排序(结果.当前值);
        结果.状态 = L1所有者范围属性类型当前完整值组读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围属性类型当前完整值组读取状态::资源失败; 结果.当前值.clear(); return 结果; }
    catch (...) { 结果.状态 = L1所有者范围属性类型当前完整值组读取状态::内部不一致; 结果.当前值.clear(); return 结果; }
}

L1所有者范围空域完整读取结果 L1事实基座仓库::读取所有者范围完整空域(
    const L1所有者范围空域完整读取请求& 请求) const noexcept {
    L1所有者范围空域完整读取结果 结果;
    结果.所有者 = 请求.所有者;
    if (!有效(请求.所有者)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        const auto 所有者 = 实现_->当前.所有者.find(请求.所有者.编码.值);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围空域完整读取状态::内部不一致; return 结果; }
        if (所有者 == 实现_->当前.所有者.end()) { 结果.状态 = L1所有者范围空域完整读取状态::未找到; return 结果; }
        if (所有者->second.范围种类 != L1所有者范围种类::独占结构范围) {
            结果.状态 = L1所有者范围空域完整读取状态::范围不支持;
            return 结果;
        }
        const auto 有事实 = [&](const auto& 表) {
            return std::any_of(表.begin(), 表.end(), [&](const auto& 项) {
                return 项.second.写入所有者 == 请求.所有者;
            });
        };
        结果.空域 = !(有事实(实现_->当前.节点) || 有事实(实现_->当前.关系)
            || 有事实(实现_->当前.值));
        结果.状态 = L1所有者范围空域完整读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围空域完整读取状态::资源失败; return 结果; }
    catch (...) { 结果.状态 = L1所有者范围空域完整读取状态::内部不一致; return 结果; }
}

L1节点当前完整引用读取结果 L1事实基座仓库::读取节点全部当前引用(
    const L1节点当前完整引用读取请求& 请求) const noexcept {
    L1节点当前完整引用读取结果 结果;
    结果.节点 = 请求.节点;
    if (!有效(请求.节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1节点当前完整引用读取状态::内部不一致; return 结果; }
        if (!实现_->当前.节点.contains(请求.节点.值)) { 结果.状态 = L1节点当前完整引用读取状态::未找到; return 结果; }
        for (const auto& [_, 关系] : 实现_->当前.关系)
            if (关系.源节点 == 请求.节点 || 关系.目标节点 == 请求.节点
                || 关系.关系类型节点 == 请求.节点)
                结果.引用.emplace_back(转换关系(关系));
        for (const auto& [_, 值] : 实现_->当前.值) {
            bool 命中 = 值.所属节点 == 请求.节点 || 值.属性类型节点 == 请求.节点
                || 值.来源节点 == 请求.节点;
            if (const auto* 引用 = std::get_if<独立材料引用>(&值.材料))
                命中 = 命中 || 引用->编码 == 请求.节点;
            if (命中) 结果.引用.emplace_back(转换值(值));
        }
        std::sort(结果.引用.begin(), 结果.引用.end(), [](const auto& 左, const auto& 右) {
            const auto 编码 = [](const auto& 项) { return std::visit([](const auto& 值) { return 值.编码; }, 项); };
            return 编码(左) < 编码(右);
        });
        结果.状态 = L1节点当前完整引用读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1节点当前完整引用读取状态::资源失败; 结果.引用.clear(); return 结果; }
    catch (...) { 结果.状态 = L1节点当前完整引用读取状态::内部不一致; 结果.引用.clear(); return 结果; }
}

L1结构所有者当前读取结果 L1事实基座仓库::读取当前结构所有者(
    const L1结构所有者读取请求& 请求) const {
    L1结构所有者当前读取结果 结果;
    结果.查询所有者 = 请求.所有者;
    if (!有效(请求.所有者)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
        const auto it = 实现_->当前.所有者.find(请求.所有者.编码.值);
        if (it == 实现_->当前.所有者.end()) { 结果.状态 = L1所有者范围读取状态::未找到; return 结果; }
        结果.状态 = L1所有者范围读取状态::成功;
        结果.所有者事实 = it->second;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围读取状态::资源失败; return 结果; }
    catch (...) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
}

L1所有者范围源关系组读取结果 L1事实基座仓库::读取所有者范围当前源关系组(
    const L1所有者范围源关系组读取请求& 请求) const {
    L1所有者范围源关系组读取结果 结果;
    结果.源节点 = 请求.源节点; 结果.关系类型节点 = 请求.关系类型节点;
    if (!有效(请求.源节点) || !有效(请求.关系类型节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
        if (!实现_->当前.节点.contains(请求.源节点.值)
            || !实现_->当前.节点.contains(请求.关系类型节点.值)) {
            结果.状态 = L1所有者范围读取状态::未找到; return 结果;
        }
        for (const auto& [_, 关系] : 实现_->当前.关系)
            if (关系.源节点 == 请求.源节点 && 关系.关系类型节点 == 请求.关系类型节点)
                结果.关系组.push_back(转换关系(关系));
        按编码排序(结果.关系组);
        结果.状态 = L1所有者范围读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围读取状态::资源失败; 结果.关系组.clear(); return 结果; }
    catch (...) { 结果.状态 = L1所有者范围读取状态::内部不一致; 结果.关系组.clear(); return 结果; }
}

L1所有者范围目标关系组读取结果 L1事实基座仓库::读取所有者范围当前目标关系组(
    const L1所有者范围目标关系组读取请求& 请求) const {
    L1所有者范围目标关系组读取结果 结果;
    结果.目标节点 = 请求.目标节点; 结果.关系类型节点 = 请求.关系类型节点;
    if (!有效(请求.目标节点) || !有效(请求.关系类型节点)) return 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围读取状态::内部不一致; return 结果; }
        if (!实现_->当前.节点.contains(请求.目标节点.值)
            || !实现_->当前.节点.contains(请求.关系类型节点.值)) {
            结果.状态 = L1所有者范围读取状态::未找到; return 结果;
        }
        for (const auto& [_, 关系] : 实现_->当前.关系)
            if (关系.目标节点 == 请求.目标节点 && 关系.关系类型节点 == 请求.关系类型节点)
                结果.关系组.push_back(转换关系(关系));
        按编码排序(结果.关系组);
        结果.状态 = L1所有者范围读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = L1所有者范围读取状态::资源失败; 结果.关系组.clear(); return 结果; }
    catch (...) { 结果.状态 = L1所有者范围读取状态::内部不一致; 结果.关系组.clear(); return 结果; }
}

L1中性一致当前读取结果 L1事实基座仓库::尝试读取中性一致当前投影(
    const L1中性一致当前读取请求& 请求) const {
    L1中性一致当前读取结果 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1中性一致当前读取状态::内部不一致; return 结果; }
        for (const auto 编码 : 请求.节点) {
            L1中性一致节点读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.节点.find(编码.值); it != 实现_->当前.节点.end()) {
                项.状态 = L1中性一致当前读取项目状态::成功; 项.事实 = 转换中性节点(it->second);
            }
            结果.节点.push_back(std::move(项));
        }
        for (const auto 编码 : 请求.关系) {
            L1中性一致关系读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.关系.find(编码.值); it != 实现_->当前.关系.end()) {
                项.状态 = L1中性一致当前读取项目状态::成功; 项.事实 = 转换中性关系(it->second);
            }
            结果.关系.push_back(std::move(项));
        }
        for (const auto 编码 : 请求.值) {
            L1中性一致值读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.值.find(编码.值); it != 实现_->当前.值.end()) {
                项.状态 = L1中性一致当前读取项目状态::成功; 项.事实 = 转换中性值(it->second);
            }
            结果.值.push_back(std::move(项));
        }
        for (const auto& 选择 : 请求.属性值) {
            L1中性一致属性值读取结果项 项;
            项.节点 = 选择.节点; 项.属性类型 = 选择.属性类型;
            const auto 节点 = 实现_->当前.节点.find(选择.节点.值);
            if (节点 != 实现_->当前.节点.end()) {
                const auto 槽 = std::lower_bound(节点->second.当前属性.begin(), 节点->second.当前属性.end(),
                    选择.属性类型, [](const 属性槽& 左, 稳定编码 右) { return 左.属性类型节点 < 右; });
                if (槽 == 节点->second.当前属性.end() || 槽->属性类型节点 != 选择.属性类型)
                    项.状态 = L1中性一致当前读取项目状态::属性未设置;
                else if (const auto 值 = 实现_->当前.值.find(槽->当前值.值); 值 != 实现_->当前.值.end()) {
                    项.状态 = L1中性一致当前读取项目状态::成功;
                    项.投影 = L1中性一致属性值投影{{槽->属性类型节点, 槽->当前值}, 转换中性值(值->second)};
                } else 项.状态 = L1中性一致当前读取项目状态::种类不匹配;
            }
            结果.属性值.push_back(std::move(项));
        }
        for (const auto& 选择 : 请求.源关系组) {
            L1中性一致源关系组读取结果项 项{选择.源节点, 选择.关系类型节点, {}};
            for (const auto& [_, 关系] : 实现_->当前.关系)
                if (关系.源节点 == 选择.源节点 && 关系.关系类型节点 == 选择.关系类型节点) {
                    const auto 对端 = 实现_->当前.节点.find(关系.目标节点.值);
                    if (对端 == 实现_->当前.节点.end()) { 结果.状态 = L1中性一致当前读取状态::内部不一致; return 结果; }
                    项.成员.push_back({转换中性关系(关系), 转换中性节点(对端->second)});
                }
            std::sort(项.成员.begin(), 项.成员.end(), [](const auto& 左, const auto& 右) { return 左.关系.编码 < 右.关系.编码; });
            结果.源关系组.push_back(std::move(项));
        }
        for (const auto& 选择 : 请求.目标关系组) {
            L1中性一致目标关系组读取结果项 项{选择.目标节点, 选择.关系类型节点, {}};
            for (const auto& [_, 关系] : 实现_->当前.关系)
                if (关系.目标节点 == 选择.目标节点 && 关系.关系类型节点 == 选择.关系类型节点) {
                    const auto 对端 = 实现_->当前.节点.find(关系.源节点.值);
                    if (对端 == 实现_->当前.节点.end()) { 结果.状态 = L1中性一致当前读取状态::内部不一致; return 结果; }
                    项.成员.push_back({转换中性关系(关系), 转换中性节点(对端->second)});
                }
            std::sort(项.成员.begin(), 项.成员.end(), [](const auto& 左, const auto& 右) { return 左.关系.编码 < 右.关系.编码; });
            结果.目标关系组.push_back(std::move(项));
        }
        结果.状态 = L1中性一致当前读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果 = {}; 结果.状态 = L1中性一致当前读取状态::资源失败; return 结果; }
    catch (...) { 结果 = {}; 结果.状态 = L1中性一致当前读取状态::内部不一致; return 结果; }
}

L1所有者范围一致当前读取结果 L1事实基座仓库::尝试读取所有者范围一致当前投影(
    const L1所有者范围一致当前读取请求& 请求) const {
    L1所有者范围一致当前读取结果 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围一致当前读取状态::内部不一致; return 结果; }
        std::unordered_set<std::uint64_t> 允许所有者;
        for (const auto 所有者 : 请求.所有者) {
            L1所有者范围一致所有者读取结果项 项;
            项.查询所有者 = 所有者;
            const auto it = 实现_->当前.所有者.find(所有者.编码.值);
            if (it != 实现_->当前.所有者.end()) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功;
                项.所有者事实 = it->second;
                允许所有者.insert(所有者.编码.值);
            }
            结果.所有者.push_back(std::move(项));
        }
        const auto 允许 = [&](L1结构所有者身份 所有者) {
            return 允许所有者.contains(所有者.编码.值);
        };
        for (const auto 编码 : 请求.节点) {
            L1所有者范围一致节点读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.节点.find(编码.值);
                it != 实现_->当前.节点.end() && 允许(it->second.写入所有者)) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功; 项.事实 = 转换节点(it->second);
            }
            结果.节点.push_back(std::move(项));
        }
        for (const auto 编码 : 请求.关系) {
            L1所有者范围一致关系读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.关系.find(编码.值);
                it != 实现_->当前.关系.end() && 允许(it->second.写入所有者)) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功; 项.事实 = 转换关系(it->second);
            }
            结果.关系.push_back(std::move(项));
        }
        for (const auto 编码 : 请求.值) {
            L1所有者范围一致值读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.值.find(编码.值);
                it != 实现_->当前.值.end() && 允许(it->second.写入所有者)) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功; 项.事实 = 转换值(it->second);
            }
            结果.值.push_back(std::move(项));
        }
        for (const auto& 选择 : 请求.属性值) {
            L1所有者范围一致属性值读取结果项 项;
            项.节点 = 选择.节点; 项.属性类型 = 选择.属性类型;
            const auto 节点 = 实现_->当前.节点.find(选择.节点.值);
            if (节点 != 实现_->当前.节点.end() && 允许(节点->second.写入所有者)) {
                const auto 槽 = std::lower_bound(节点->second.当前属性.begin(), 节点->second.当前属性.end(),
                    选择.属性类型, [](const 属性槽& 左, 稳定编码 右) { return 左.属性类型节点 < 右; });
                if (槽 == 节点->second.当前属性.end() || 槽->属性类型节点 != 选择.属性类型)
                    项.状态 = L1所有者范围一致当前读取项目状态::属性未设置;
                else if (const auto 值 = 实现_->当前.值.find(槽->当前值.值);
                    值 != 实现_->当前.值.end() && 允许(值->second.写入所有者)) {
                    项.状态 = L1所有者范围一致当前读取项目状态::成功;
                    项.投影 = L1所有者范围一致属性值投影{
                        {槽->属性类型节点, 槽->当前值}, 转换值(值->second)};
                } else 项.状态 = L1所有者范围一致当前读取项目状态::种类不匹配;
            }
            结果.属性值.push_back(std::move(项));
        }
        for (const auto& 选择 : 请求.源关系组) {
            L1所有者范围一致源关系组读取结果项 项{选择.源节点, 选择.关系类型节点, {}};
            for (const auto& [_, 关系] : 实现_->当前.关系)
                if (关系.源节点 == 选择.源节点 && 关系.关系类型节点 == 选择.关系类型节点
                    && 允许(关系.写入所有者)) {
                    const auto 对端 = 实现_->当前.节点.find(关系.目标节点.值);
                    if (对端 == 实现_->当前.节点.end()) { 结果.状态 = L1所有者范围一致当前读取状态::内部不一致; return 结果; }
                    项.成员.push_back({转换关系(关系), 转换节点(对端->second)});
                }
            std::sort(项.成员.begin(), 项.成员.end(), [](const auto& 左, const auto& 右) { return 左.关系.编码 < 右.关系.编码; });
            结果.源关系组.push_back(std::move(项));
        }
        for (const auto& 选择 : 请求.目标关系组) {
            L1所有者范围一致目标关系组读取结果项 项{选择.目标节点, 选择.关系类型节点, {}};
            for (const auto& [_, 关系] : 实现_->当前.关系)
                if (关系.目标节点 == 选择.目标节点 && 关系.关系类型节点 == 选择.关系类型节点
                    && 允许(关系.写入所有者)) {
                    const auto 对端 = 实现_->当前.节点.find(关系.源节点.值);
                    if (对端 == 实现_->当前.节点.end()) { 结果.状态 = L1所有者范围一致当前读取状态::内部不一致; return 结果; }
                    项.成员.push_back({转换关系(关系), 转换节点(对端->second)});
                }
            std::sort(项.成员.begin(), 项.成员.end(), [](const auto& 左, const auto& 右) { return 左.关系.编码 < 右.关系.编码; });
            结果.目标关系组.push_back(std::move(项));
        }
        结果.状态 = L1所有者范围一致当前读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果 = {}; 结果.状态 = L1所有者范围一致当前读取状态::资源失败; return 结果; }
    catch (...) { 结果 = {}; 结果.状态 = L1所有者范围一致当前读取状态::内部不一致; return 结果; }
}

L1所有者范围一致关系类型闭包读取结果
L1事实基座仓库::尝试读取所有者范围一致关系类型闭包投影(
    const L1所有者范围一致关系类型闭包读取请求& 请求) const {
    L1所有者范围一致关系类型闭包读取结果 结果;
    try {
        std::shared_lock 锁(实现_->锁);
        if (实现_->当前.隔离) { 结果.状态 = L1所有者范围一致当前读取状态::内部不一致; return 结果; }
        std::unordered_set<std::uint64_t> 允许所有者;
        for (const auto 所有者 : 请求.所有者) {
            L1所有者范围一致所有者读取结果项 项;
            项.查询所有者 = 所有者;
            if (const auto it = 实现_->当前.所有者.find(所有者.编码.值);
                it != 实现_->当前.所有者.end()) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功;
                项.所有者事实 = it->second;
                允许所有者.insert(所有者.编码.值);
            }
            结果.所有者.push_back(std::move(项));
        }
        const auto 允许 = [&](L1结构所有者身份 所有者) {
            return 允许所有者.contains(所有者.编码.值);
        };
        for (const auto 编码 : 请求.节点) {
            L1所有者范围一致节点读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.节点.find(编码.值);
                it != 实现_->当前.节点.end() && 允许(it->second.写入所有者)) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功; 项.事实 = 转换节点(it->second);
            }
            结果.节点.push_back(std::move(项));
        }
        for (const auto 编码 : 请求.关系) {
            L1所有者范围一致关系读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.关系.find(编码.值);
                it != 实现_->当前.关系.end() && 允许(it->second.写入所有者)) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功; 项.事实 = 转换关系(it->second);
            }
            结果.关系.push_back(std::move(项));
        }
        for (const auto 编码 : 请求.值) {
            L1所有者范围一致值读取结果项 项; 项.查询编码 = 编码;
            if (const auto it = 实现_->当前.值.find(编码.值);
                it != 实现_->当前.值.end() && 允许(it->second.写入所有者)) {
                项.状态 = L1所有者范围一致当前读取项目状态::成功; 项.事实 = 转换值(it->second);
            }
            结果.值.push_back(std::move(项));
        }
        const auto 读属性 = [&](稳定编码 节点编码, 稳定编码 类型编码) {
            L1所有者范围一致属性值读取结果项 项;
            项.节点 = 节点编码; 项.属性类型 = 类型编码;
            const auto 节点 = 实现_->当前.节点.find(节点编码.值);
            if (节点 == 实现_->当前.节点.end() || !允许(节点->second.写入所有者)) return 项;
            const auto 槽 = std::lower_bound(节点->second.当前属性.begin(), 节点->second.当前属性.end(),
                类型编码, [](const 属性槽& 左, 稳定编码 右) { return 左.属性类型节点 < 右; });
            if (槽 == 节点->second.当前属性.end() || 槽->属性类型节点 != 类型编码) {
                项.状态 = L1所有者范围一致当前读取项目状态::属性未设置;
                return 项;
            }
            const auto 值 = 实现_->当前.值.find(槽->当前值.值);
            if (值 == 实现_->当前.值.end() || !允许(值->second.写入所有者)) {
                项.状态 = L1所有者范围一致当前读取项目状态::种类不匹配;
                return 项;
            }
            项.状态 = L1所有者范围一致当前读取项目状态::成功;
            项.投影 = L1所有者范围一致属性值投影{
                {槽->属性类型节点, 槽->当前值}, 转换值(值->second)};
            return 项;
        };
        for (const auto& 选择 : 请求.属性值)
            结果.属性值.push_back(读属性(选择.节点, 选择.属性类型));
        const auto 读源关系组 = [&](稳定编码 节点编码, 稳定编码 类型编码) {
            L1所有者范围一致源关系组读取结果项 项{节点编码, 类型编码, {}};
            for (const auto& [_, 关系] : 实现_->当前.关系)
                if (关系.源节点 == 节点编码 && 关系.关系类型节点 == 类型编码
                    && 允许(关系.写入所有者)) {
                    const auto 对端 = 实现_->当前.节点.find(关系.目标节点.值);
                    if (对端 != 实现_->当前.节点.end())
                        项.成员.push_back({转换关系(关系), 转换节点(对端->second)});
                }
            std::sort(项.成员.begin(), 项.成员.end(), [](const auto& 左, const auto& 右) { return 左.关系.编码 < 右.关系.编码; });
            return 项;
        };
        const auto 读目标关系组 = [&](稳定编码 节点编码, 稳定编码 类型编码) {
            L1所有者范围一致目标关系组读取结果项 项{节点编码, 类型编码, {}};
            for (const auto& [_, 关系] : 实现_->当前.关系)
                if (关系.目标节点 == 节点编码 && 关系.关系类型节点 == 类型编码
                    && 允许(关系.写入所有者)) {
                    const auto 对端 = 实现_->当前.节点.find(关系.源节点.值);
                    if (对端 != 实现_->当前.节点.end())
                        项.成员.push_back({转换关系(关系), 转换节点(对端->second)});
                }
            std::sort(项.成员.begin(), 项.成员.end(), [](const auto& 左, const auto& 右) { return 左.关系.编码 < 右.关系.编码; });
            return 项;
        };
        for (const auto& 选择 : 请求.源关系组)
            结果.源关系组.push_back(读源关系组(选择.源节点, 选择.关系类型节点));
        for (const auto& 选择 : 请求.目标关系组)
            结果.目标关系组.push_back(读目标关系组(选择.目标节点, 选择.关系类型节点));

        for (const auto& 选择 : 请求.关系类型闭包) {
            L1所有者范围一致关系类型闭包读取结果项 闭包;
            闭包.入口关系类型节点 = 选择.入口关系类型节点;
            const auto 类型事实 = 实现_->当前.节点.find(选择.入口关系类型节点.值);
            if (类型事实 == 实现_->当前.节点.end()
                || !允许(类型事实->second.写入所有者)) {
                闭包.状态 = L1所有者范围一致当前读取项目状态::未找到;
                结果.关系类型闭包.push_back(std::move(闭包));
                continue;
            }
            闭包.状态 = L1所有者范围一致当前读取项目状态::成功;
            闭包.关系类型事实 = 转换节点(类型事实->second);
            for (const auto& [_, 关系] : 实现_->当前.关系) {
                if (关系.关系类型节点 != 选择.入口关系类型节点
                    || !允许(关系.写入所有者)) continue;
                const auto 源 = 实现_->当前.节点.find(关系.源节点.值);
                const auto 目标 = 实现_->当前.节点.find(关系.目标节点.值);
                if (源 == 实现_->当前.节点.end() || 目标 == 实现_->当前.节点.end()) {
                    结果 = {};
                    结果.状态 = L1所有者范围一致当前读取状态::内部不一致;
                    return 结果;
                }
                L1所有者范围一致关系类型闭包成员 成员;
                成员.关系 = 转换关系(关系);
                成员.源节点 = 转换节点(源->second);
                成员.目标节点 = 转换节点(目标->second);
                for (const auto 类型 : 选择.源节点属性类型)
                    成员.源节点属性值.push_back(读属性(关系.源节点, 类型));
                for (const auto 类型 : 选择.目标节点属性类型)
                    成员.目标节点属性值.push_back(读属性(关系.目标节点, 类型));
                const auto 读闭包源组 = [&](稳定编码 端点, 稳定编码 类型) {
                    L1所有者范围一致闭包端点关系组读取结果项 项;
                    项.端点节点 = 端点; 项.关系类型节点 = 类型;
                    if (const auto t = 实现_->当前.节点.find(类型.值); t != 实现_->当前.节点.end()) {
                        项.状态 = L1所有者范围一致当前读取项目状态::成功;
                        项.关系类型事实 = 转换节点(t->second);
                        for (auto& m : 读源关系组(端点, 类型).成员) 项.成员.push_back(std::move(m));
                    }
                    return 项;
                };
                const auto 读闭包目标组 = [&](稳定编码 端点, 稳定编码 类型) {
                    L1所有者范围一致闭包端点关系组读取结果项 项;
                    项.端点节点 = 端点; 项.关系类型节点 = 类型;
                    if (const auto t = 实现_->当前.节点.find(类型.值); t != 实现_->当前.节点.end()) {
                        项.状态 = L1所有者范围一致当前读取项目状态::成功;
                        项.关系类型事实 = 转换节点(t->second);
                        for (auto& m : 读目标关系组(端点, 类型).成员) 项.成员.push_back(std::move(m));
                    }
                    return 项;
                };
                for (const auto 类型 : 选择.源节点源关系类型) 成员.源节点源关系组.push_back(读闭包源组(关系.源节点, 类型));
                for (const auto 类型 : 选择.源节点目标关系类型) 成员.源节点目标关系组.push_back(读闭包目标组(关系.源节点, 类型));
                for (const auto 类型 : 选择.目标节点源关系类型) 成员.目标节点源关系组.push_back(读闭包源组(关系.目标节点, 类型));
                for (const auto 类型 : 选择.目标节点目标关系类型) 成员.目标节点目标关系组.push_back(读闭包目标组(关系.目标节点, 类型));
                闭包.成员.push_back(std::move(成员));
            }
            std::sort(闭包.成员.begin(), 闭包.成员.end(), [](const auto& 左, const auto& 右) { return 左.关系.编码 < 右.关系.编码; });
            结果.关系类型闭包.push_back(std::move(闭包));
        }
        结果.状态 = L1所有者范围一致当前读取状态::成功;
        return 结果;
    } catch (const std::bad_alloc&) { 结果 = {}; 结果.状态 = L1所有者范围一致当前读取状态::资源失败; return 结果; }
    catch (...) { 结果 = {}; 结果.状态 = L1所有者范围一致当前读取状态::内部不一致; return 结果; }
}

} // namespace 海中鱼巣
