// 文件规则：本模块只实现五组权限、单任务权限和候选排序的纯值算法，不派发任务或写入安全事实。
module;

#include "../核心/句柄.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <new>
#include <optional>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.算法.安全任务权限;

import 海中鱼巣.领域.协议.分层安全维护;

namespace 海中鱼巣::安全任务权限细节 {

inline bool 句柄小于(const 节点句柄& 左, const 节点句柄& 右) noexcept {
    if (左.仓库编号 != 右.仓库编号) return 左.仓库编号 < 右.仓库编号;
    if (左.节点编号 != 右.节点编号) return 左.节点编号 < 右.节点编号;
    return 左.版本号 < 右.版本号;
}

inline bool 非负乘除向下(
    std::int64_t 左,
    std::int64_t 右,
    std::int64_t 除数,
    std::int64_t& 结果) noexcept {
    if (左 < 0 || 右 < 0 || 除数 <= 0) return false;
    const auto 无符号左 = static_cast<std::uint64_t>(左);
    const auto 无符号右 = static_cast<std::uint64_t>(右);
    const auto 无符号除数 = static_cast<std::uint64_t>(除数);
    std::uint64_t 商 = 0;
    std::uint64_t 余数 = 0;
    for (int 位 = 63; 位 >= 0; --位) {
        if (余数 > ((std::numeric_limits<std::uint64_t>::max)() >> 1)) return false;
        余数 <<= 1;
        余数 |= (无符号左 >> 位) & 1ULL;
        if (余数 >= 无符号除数) {
            余数 -= 无符号除数;
            商 |= 1ULL << 位;
        }
    }
    if (无符号右 != 0
        && 商 > (std::numeric_limits<std::uint64_t>::max)() / 无符号右) {
        return false;
    }
    const auto 主体 = 商 * 无符号右;
    if (余数 != 0
        && 无符号右 > (std::numeric_limits<std::uint64_t>::max)() / 余数) {
        return false;
    }
    const auto 尾部 = (余数 * 无符号右) / 无符号除数;
    if (主体 > (std::numeric_limits<std::uint64_t>::max)() - 尾部) return false;
    const auto 总值 = 主体 + 尾部;
    if (总值 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)())) {
        return false;
    }
    结果 = static_cast<std::int64_t>(总值);
    return true;
}

inline bool 层稳定小于(const 安全组层快照& 左, const 安全组层快照& 右) noexcept {
    if (左.优先级组 != 右.优先级组) return 左.优先级组 < 右.优先级组;
    if (左.来源需求 != 右.来源需求) return 句柄小于(左.来源需求, 右.来源需求);
    return 句柄小于(左.来源因果信息, 右.来源因果信息);
}

inline bool 候选稳定小于(
    const 安全任务执行候选快照& 左,
    const 安全任务执行候选快照& 右) noexcept {
    if (左.任务稳定句柄 != 右.任务稳定句柄) {
        return 句柄小于(左.任务稳定句柄, 右.任务稳定句柄);
    }
    return 左.创建序号 < 右.创建序号;
}

inline 安全结果分类 复核权限入口(
    const 权限读取请求& 请求) noexcept {
    if (!句柄有效(请求.自我)
        || !句柄有效(请求.适用场景)
        || 请求.事实截止版本 == 0
        || 请求.图版本 == 0
        || 请求.权限规则版本 != 安全权限规则版本) {
        return 安全结果分类::入口拒绝;
    }
    return 安全结果分类::已形成;
}

inline std::optional<std::int64_t> 计算释放倍率(const 安全组层快照& 层) noexcept {
    if (层.值域下界 >= 层.值域上界
        || 层.低位阈值 < 层.值域下界
        || 层.高位阈值 > 层.值域上界
        || 层.低位阈值 > 层.高位阈值
        || 层.当前值 < 层.值域下界
        || 层.当前值 > 层.值域上界) {
        return std::nullopt;
    }
    if (层.当前值 <= 层.低位阈值) return std::int64_t{0};
    if (层.当前值 >= 层.高位阈值) return 安全最大释放倍率;
    const auto 分子值 = 层.当前值 - 层.低位阈值;
    const auto 分母值 = 层.高位阈值 - 层.低位阈值;
    std::int64_t 结果 = 0;
    if (!非负乘除向下(分子值, 安全最大释放倍率, 分母值, 结果)) {
        return std::nullopt;
    }
    return 结果;
}

inline 权限读取结果 无权限载荷(安全结果分类 分类) noexcept {
    return {分类, std::nullopt};
}

inline 任务权限结果 无任务载荷(安全结果分类 分类) noexcept {
    return {分类, std::nullopt};
}

inline 任务排序结果 无排序载荷(安全结果分类 分类) noexcept {
    return {分类, std::nullopt};
}

inline const 安全任务执行候选快照* 查找候选(
    const 安全任务事实来源载荷& 事实,
    const 节点句柄& 任务) noexcept {
    const 安全任务执行候选快照* 结果 = nullptr;
    for (const auto& 候选 : 事实.任务执行候选组) {
        if (候选.任务稳定句柄 != 任务) continue;
        if (结果 != nullptr) return nullptr;
        结果 = &候选;
    }
    return 结果;
}

inline const 安全后果严重度材料* 查找严重度(
    const 安全任务事实来源载荷& 事实,
    const 节点句柄& 来源需求) noexcept {
    for (const auto& 材料 : 事实.后果严重度材料组) {
        if (材料.来源需求 == 来源需求 && 材料.当前性 == 记录当前性::当前) return &材料;
    }
    return nullptr;
}

inline const 安全预计时间材料* 查找预计时间(
    const 安全任务事实来源载荷& 事实,
    const 节点句柄& 来源需求) noexcept {
    for (const auto& 材料 : 事实.预计时间证据组) {
        if (材料.来源需求 == 来源需求 && 材料.当前性 == 记录当前性::当前) return &材料;
    }
    return nullptr;
}

inline const 安全因果证据序材料* 查找因果证据(
    const 安全任务事实来源载荷& 事实,
    const 节点句柄& 来源需求) noexcept {
    for (const auto& 材料 : 事实.因果证据序材料组) {
        if (材料.来源需求 == 来源需求 && 材料.当前性 == 记录当前性::当前) return &材料;
    }
    return nullptr;
}

inline const 安全任务执行候选快照* 查找任务候选(
    const 安全任务事实来源载荷& 事实,
    const 节点句柄& 任务) noexcept {
    for (const auto& 候选 : 事实.任务执行候选组) {
        if (候选.任务稳定句柄 == 任务) return &候选;
    }
    return nullptr;
}

}

export namespace 海中鱼巣 {

权限读取结果 计算安全五组权限(
    const std::vector<安全组层快照>& 有序层组,
    const 权限读取请求& 请求) noexcept {
    using namespace 安全任务权限细节;
    try {
        const auto 入口 = 复核权限入口(请求);
        if (入口 != 安全结果分类::已形成) return 无权限载荷(入口);
        for (std::size_t 序号 = 0; 序号 < 有序层组.size(); ++序号) {
            const auto& 层 = 有序层组[序号];
            if (!句柄有效(层.来源需求)
                || !句柄有效(层.来源因果信息)
                || !句柄有效(层.目标安全结果)
                || 层.精确深度 <= 0
                || 层.优先级组 < 1 || 层.优先级组 > 5
                || 层.优先级组 != (std::min)(层.精确深度, std::int64_t{5})) {
                return 无权限载荷(安全结果分类::结构拒绝);
            }
            if (序号 != 0 && !层稳定小于(有序层组[序号 - 1], 层)) {
                return 无权限载荷(安全结果分类::结构拒绝);
            }
            if (层.图版本 != 请求.图版本 || 层.规则版本 != 请求.权限规则版本) {
                return 无权限载荷(安全结果分类::版本漂移);
            }
        }

        权限读取载荷 载荷;
        载荷.全部安全组层快照 = 有序层组;
        载荷.全部来源版本.图版本 = 请求.图版本;
        载荷.全部来源版本.事实截止版本 = 请求.事实截止版本;
        载荷.全部来源版本.权限规则版本 = 请求.权限规则版本;
        载荷.权限规则版本 = 请求.权限规则版本;
        载荷.收到权限.fill(0);
        载荷.保留权限.fill(0);
        载荷.释放倍率.fill(0);
        载荷.权限状态.fill(安全组权限状态::权限为零);

        std::optional<std::int64_t> 首个缺口组;
        std::array<std::vector<const 安全组层快照*>, 5> 分组;
        for (const auto& 层 : 有序层组) 分组[static_cast<std::size_t>(层.优先级组 - 1)].push_back(&层);
        for (std::size_t 组序号 = 0; 组序号 < 5; ++组序号) {
            for (const auto* 层 : 分组[组序号]) {
                if (层->值版本 == 0 || 层->阈值版本 == 0) {
                    if (!首个缺口组) 首个缺口组 = static_cast<std::int64_t>(组序号 + 1);
                    载荷.材料缺口.push_back({
                        static_cast<std::int64_t>(组序号 + 1),
                        层->来源需求,
                        请求.事实截止版本,
                        std::nullopt});
                    continue;
                }
                const auto 倍率 = 计算释放倍率(*层);
                if (!倍率) return 无权限载荷(安全结果分类::结构拒绝);
                if (*倍率 != 层->释放倍率) {
                    return 无权限载荷(安全结果分类::结构拒绝);
                }
            }
        }

        std::int64_t 收到 = 安全权限满刻度;
        for (std::size_t 组序号 = 0; 组序号 < 5; ++组序号) {
            if (首个缺口组 && static_cast<std::int64_t>(组序号 + 1) >= *首个缺口组) {
                载荷.收到权限[组序号] = 0;
                载荷.保留权限[组序号] = 0;
                载荷.权限状态[组序号] = 安全组权限状态::材料缺口;
                载荷.受影响组.push_back(static_cast<std::int64_t>(组序号 + 1));
                continue;
            }
            载荷.收到权限[组序号] = 收到;
            if (组序号 == 4) {
                载荷.保留权限[组序号] = 收到;
                载荷.权限状态[组序号] = 收到 == 0
                    ? 安全组权限状态::权限为零 : 安全组权限状态::已形成;
                break;
            }
            std::int64_t 最小倍率 = 0;
            if (!分组[组序号].empty()) {
                最小倍率 = 安全最大释放倍率;
                for (const auto* 层 : 分组[组序号]) {
                    const auto 倍率 = 计算释放倍率(*层);
                    if (!倍率) return 无权限载荷(安全结果分类::结构拒绝);
                    if (*倍率 < 最小倍率) {
                        最小倍率 = *倍率;
                        载荷.限制层来源[组序号].clear();
                        载荷.限制层来源[组序号].push_back(层->来源需求);
                    } else if (*倍率 == 最小倍率) {
                        载荷.限制层来源[组序号].push_back(层->来源需求);
                    }
                }
            }
            载荷.释放倍率[组序号] = 最小倍率;
            std::int64_t 向上 = 0;
            if (!非负乘除向下(收到, 最小倍率, 安全权限满刻度, 向上)) {
                return 无权限载荷(安全结果分类::结构拒绝);
            }
            载荷.保留权限[组序号] = 收到 - 向上;
            载荷.权限状态[组序号] = 载荷.保留权限[组序号] == 0
                ? 安全组权限状态::权限为零 : 安全组权限状态::已形成;
            收到 = 向上;
        }

        std::int64_t 总量 = 0;
        for (std::size_t 组序号 = 0; 组序号 < 5; ++组序号) {
            if (载荷.权限状态[组序号] == 安全组权限状态::材料缺口) continue;
            if (载荷.保留权限[组序号] < 0
                || 载荷.保留权限[组序号] > 安全权限满刻度
                || 总量 > 安全权限满刻度 - 载荷.保留权限[组序号]) {
                return 无权限载荷(安全结果分类::结构拒绝);
            }
            总量 += 载荷.保留权限[组序号];
        }
        载荷.五组已形成权限总量 = 总量;
        if (首个缺口组) return {安全结果分类::材料缺失, std::move(载荷)};
        if (总量 != 安全权限满刻度) {
            return 无权限载荷(安全结果分类::结构拒绝);
        }
        const bool 全零 = std::all_of(
            载荷.保留权限.begin(), 载荷.保留权限.end(),
            [](auto 值) { return 值 == 0; });
        return {全零 ? 安全结果分类::权限为零 : 安全结果分类::已形成,
            std::move(载荷)};
    } catch (const std::bad_alloc&) {
        return 无权限载荷(安全结果分类::资源失败);
    } catch (...) {
        return 无权限载荷(安全结果分类::资源失败);
    }
}

任务权限结果 计算单任务安全权限(
    const 安全任务事实来源载荷& 任务事实,
    const 权限读取载荷& 权限,
    const 任务权限请求& 请求) noexcept {
    using namespace 安全任务权限细节;
    try {
        if (!句柄有效(请求.自我)
            || !句柄有效(请求.任务稳定句柄)
            || 请求.事实截止版本 == 0
            || 请求.权限规则版本 != 安全权限规则版本) {
            return 无任务载荷(安全结果分类::入口拒绝);
        }
        if (任务事实.来源版本.事实截止版本 != 请求.事实截止版本
            || 权限.全部来源版本.事实截止版本 != 请求.事实截止版本
            || 权限.权限规则版本 != 请求.权限规则版本
            || 任务事实.来源版本.任务集合版本 == 0) {
            return 无任务载荷(安全结果分类::版本漂移);
        }
        const auto* 候选 = 查找候选(任务事实, 请求.任务稳定句柄);
        if (候选 == nullptr
            || 候选->基础优先级 < 0
            || 候选->授权版本 == 0
            || 候选->状态版本 == 0
            || 候选->方法冻结版本 == 0) {
            return 无任务载荷(安全结果分类::材料缺失);
        }

        std::vector<安全任务来源权限> 来源组;
        for (const auto& 关联 : 任务事实.任务安全关联组) {
            if (关联.任务稳定句柄 != 请求.任务稳定句柄) continue;
            if (!句柄有效(关联.来源需求)
                || !句柄有效(关联.来源因果信息)
                || !句柄有效(关联.目标安全结果)
                || 关联.当前性 != 记录当前性::当前
                || 关联.关联版本 == 0
                || 关联.精确深度适用性 != 适用性::适用
                || 关联.分组适用性 != 适用性::适用
                || !关联.优先级组
                || *关联.优先级组 < 1 || *关联.优先级组 > 5
                || 关联.精确深度 <= 0
                || *关联.优先级组 != (std::min)(关联.精确深度, std::int64_t{5})) {
                return 无任务载荷(安全结果分类::材料缺失);
            }
            const auto 索引 = static_cast<std::size_t>(*关联.优先级组 - 1);
            if (权限.权限状态[索引] == 安全组权限状态::材料缺口) {
                return 无任务载荷(安全结果分类::材料缺失);
            }
            安全因果路径快照 路径;
            路径.因果信息序列.push_back(关联.来源因果信息);
            来源组.push_back({
                关联.来源需求,
                权限.保留权限[索引],
                关联.精确深度,
                std::move(路径)});
        }
        if (来源组.empty()) return 无任务载荷(安全结果分类::材料缺失);
        std::sort(来源组.begin(), 来源组.end(), [](const auto& 左, const auto& 右) {
            if (左.来源需求 != 右.来源需求) return 句柄小于(左.来源需求, 右.来源需求);
            return 左.精确深度 < 右.精确深度;
        });
        for (std::size_t 序号 = 1; 序号 < 来源组.size(); ++序号) {
            if (来源组[序号 - 1].来源需求 == 来源组[序号].来源需求
                && 来源组[序号 - 1].精确深度 == 来源组[序号].精确深度) {
                return 无任务载荷(安全结果分类::结构拒绝);
            }
        }
        const auto 最高 = std::max_element(
            来源组.begin(), 来源组.end(),
            [](const auto& 左, const auto& 右) { return 左.权限 < 右.权限; })->权限;
        std::vector<节点句柄> 并列;
        for (const auto& 来源 : 来源组) {
            if (来源.权限 == 最高) 并列.push_back(来源.来源需求);
        }
        std::sort(并列.begin(), 并列.end(), 句柄小于);
        std::int64_t 有效优先级 = 0;
        if (!非负乘除向下(
                候选->基础优先级, 最高, 安全权限满刻度, 有效优先级)) {
            return 无任务载荷(安全结果分类::结构拒绝);
        }
        任务权限载荷 载荷{
            请求.任务稳定句柄,
            最高,
            有效优先级,
            并列.front(),
            std::move(并列),
            std::move(来源组),
            任务事实.来源版本,
            请求.权限规则版本};
        return {最高 == 0 ? 安全结果分类::权限为零 : 安全结果分类::已形成,
            std::move(载荷)};
    } catch (const std::bad_alloc&) {
        return 无任务载荷(安全结果分类::资源失败);
    } catch (...) {
        return 无任务载荷(安全结果分类::资源失败);
    }
}

任务排序结果 排序安全任务执行候选(
    const 安全任务事实来源载荷& 任务事实,
    const 权限读取载荷& 权限,
    const 任务排序请求& 请求) noexcept {
    using namespace 安全任务权限细节;
    try {
        if (!句柄有效(请求.自我)
            || !句柄有效(请求.适用场景)
            || 请求.事实截止版本 == 0
            || 请求.权限规则版本 != 安全权限规则版本
            || 请求.排序规则版本 != 安全任务排序规则版本) {
            return 无排序载荷(安全结果分类::入口拒绝);
        }
        if (任务事实.来源版本.事实截止版本 != 请求.事实截止版本
            || 权限.全部来源版本.事实截止版本 != 请求.事实截止版本
            || 任务事实.来源版本.任务集合版本 == 0
            || 权限.权限规则版本 != 请求.权限规则版本) {
            return 无排序载荷(安全结果分类::版本漂移);
        }
        for (std::size_t 序号 = 0; 序号 < 请求.候选任务组.size(); ++序号) {
            const auto& 候选 = 请求.候选任务组[序号];
            if (!句柄有效(候选.任务稳定句柄)
                || 候选.基础优先级 < 0
                || 候选.授权版本 == 0 || 候选.状态版本 == 0
                || 候选.方法冻结版本 == 0) {
                return 无排序载荷(安全结果分类::结构拒绝);
            }
            if (序号 != 0 && !候选稳定小于(请求.候选任务组[序号 - 1], 候选)) {
                return 无排序载荷(安全结果分类::结构拒绝);
            }
            const auto* 事实候选 = 查找任务候选(任务事实, 候选.任务稳定句柄);
            if (事实候选 == nullptr
                || 事实候选->授权版本 != 候选.授权版本
                || 事实候选->状态版本 != 候选.状态版本
                || 事实候选->方法冻结版本 != 候选.方法冻结版本) {
                return 无排序载荷(安全结果分类::版本漂移);
            }
        }

        任务排序载荷 载荷;
        载荷.全部输入版本 = 任务事实.来源版本;
        载荷.排序规则版本 = 请求.排序规则版本;
        for (const auto& 候选 : 请求.候选任务组) {
            任务权限请求 权限请求{
                请求.自我,
                候选.任务稳定句柄,
                请求.事实截止版本,
                请求.权限规则版本};
            auto 单项 = 计算单任务安全权限(任务事实, 权限, 权限请求);
            if (单项.结果分类 == 安全结果分类::已形成 && 单项.载荷) {
                载荷.有序任务权限载荷组.push_back(std::move(*单项.载荷));
            } else if (单项.结果分类 == 安全结果分类::权限为零) {
                载荷.零权限任务组.push_back(候选);
            } else if (单项.结果分类 == 安全结果分类::材料缺失) {
                载荷.材料缺口任务组.push_back(候选);
            } else {
                return 无排序载荷(单项.结果分类);
            }
        }

        std::stable_sort(
            载荷.有序任务权限载荷组.begin(),
            载荷.有序任务权限载荷组.end(),
            [&](const 任务权限载荷& 左, const 任务权限载荷& 右) {
                const auto* 左候选 = 查找任务候选(任务事实, 左.任务稳定句柄);
                const auto* 右候选 = 查找任务候选(任务事实, 右.任务稳定句柄);
                const auto 左深度 = std::min_element(
                    左.全部来源权限.begin(), 左.全部来源权限.end(),
                    [](const auto& 甲, const auto& 乙) { return 甲.精确深度 < 乙.精确深度; })->精确深度;
                const auto 右深度 = std::min_element(
                    右.全部来源权限.begin(), 右.全部来源权限.end(),
                    [](const auto& 甲, const auto& 乙) { return 甲.精确深度 < 乙.精确深度; })->精确深度;
                const bool 左组五 = 左深度 >= 5;
                const bool 右组五 = 右深度 >= 5;
                if (左组五 != 右组五) return !左组五;
                if (!左组五) {
                    if (左.有效执行优先级 != 右.有效执行优先级) {
                        return 左.有效执行优先级 > 右.有效执行优先级;
                    }
                    if (左.最高权限 != 右.最高权限) return 左.最高权限 > 右.最高权限;
                    if (左候选->基础优先级 != 右候选->基础优先级) {
                        return 左候选->基础优先级 > 右候选->基础优先级;
                    }
                } else {
                    if (左.最高权限 != 右.最高权限) return 左.最高权限 > 右.最高权限;
                    const auto* 左严重度 = 查找严重度(任务事实, 左.主来源需求);
                    const auto* 右严重度 = 查找严重度(任务事实, 右.主来源需求);
                    const auto 左严重度值 = 左严重度 == nullptr ? std::int64_t{0} : 左严重度->严重度;
                    const auto 右严重度值 = 右严重度 == nullptr ? std::int64_t{0} : 右严重度->严重度;
                    if (左严重度值 != 右严重度值) return 左严重度值 > 右严重度值;
                    const auto* 左时间 = 查找预计时间(任务事实, 左.主来源需求);
                    const auto* 右时间 = 查找预计时间(任务事实, 右.主来源需求);
                    const bool 左可预计 = 左时间 != nullptr && 左时间->时间适用性 == 适用性::适用
                        && 左时间->预计剩余单调时间.has_value();
                    const bool 右可预计 = 右时间 != nullptr && 右时间->时间适用性 == 适用性::适用
                        && 右时间->预计剩余单调时间.has_value();
                    if (左可预计 != 右可预计) return 左可预计;
                    if (左可预计
                        && *左时间->预计剩余单调时间 != *右时间->预计剩余单调时间) {
                        return *左时间->预计剩余单调时间 < *右时间->预计剩余单调时间;
                    }
                    const auto* 左证据 = 查找因果证据(任务事实, 左.主来源需求);
                    const auto* 右证据 = 查找因果证据(任务事实, 右.主来源需求);
                    const auto 左反例 = 左证据 == nullptr
                        ? (std::numeric_limits<std::int64_t>::max)() : 左证据->当前反例数;
                    const auto 右反例 = 右证据 == nullptr
                        ? (std::numeric_limits<std::int64_t>::max)() : 右证据->当前反例数;
                    if (左反例 != 右反例) return 左反例 < 右反例;
                    const auto 左正样本 = 左证据 == nullptr ? std::int64_t{0} : 左证据->有效基础正样本数;
                    const auto 右正样本 = 右证据 == nullptr ? std::int64_t{0} : 右证据->有效基础正样本数;
                    if (左正样本 != 右正样本) return 左正样本 > 右正样本;
                    if (左候选->基础优先级 != 右候选->基础优先级) {
                        return 左候选->基础优先级 > 右候选->基础优先级;
                    }
                    if (左深度 != 右深度) return 左深度 < 右深度;
                    if (左.主来源需求 != 右.主来源需求) {
                        return 句柄小于(左.主来源需求, 右.主来源需求);
                    }
                }
                if (左候选->创建序号 != 右候选->创建序号) {
                    return 左候选->创建序号 < 右候选->创建序号;
                }
                return 句柄小于(左.任务稳定句柄, 右.任务稳定句柄);
            });
        return {载荷.有序任务权限载荷组.empty()
                ? 安全结果分类::当前无可执行任务
                : 安全结果分类::已形成,
            std::move(载荷)};
    } catch (const std::bad_alloc&) {
        return 无排序载荷(安全结果分类::资源失败);
    } catch (...) {
        return 无排序载荷(安全结果分类::资源失败);
    }
}

}
