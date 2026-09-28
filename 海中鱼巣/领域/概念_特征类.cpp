#include "概念_特征类.h"

#include <algorithm>
#include <limits>
#include <mutex>

namespace 海中鱼巣 {

稳定编码 特征概念树{};

namespace {

std::recursive_mutex 特征概念树互斥;

bool 是I64类型(特征概念材料物理类型 类型) noexcept;
bool 相邻或重叠(
    const 特征概念I64闭区间& 左,
    const 特征概念I64闭区间& 右) noexcept;

稳定编码 节点类型字段{};
稳定编码 特征概念节点类型{};
稳定编码 材料类型字段{};
稳定编码 I64值域字段{};
稳定编码 非I64值域字段{};
稳定编码 单位字段{};
稳定编码 比较规则字段{};
稳定编码 聚合规则字段{};
稳定编码 名称关系字段{};

bool 结构已初始化() noexcept {
    return 有效(特征概念树)
        && 有效(节点类型字段)
        && 有效(特征概念节点类型)
        && 有效(材料类型字段)
        && 有效(I64值域字段)
        && 有效(非I64值域字段)
        && 有效(单位字段)
        && 有效(比较规则字段)
        && 有效(聚合规则字段)
        && 有效(名称关系字段);
}

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(特征概念树)
        && 节点仍存在(节点类型字段)
        && 节点仍存在(特征概念节点类型)
        && 节点仍存在(材料类型字段)
        && 节点仍存在(I64值域字段)
        && 节点仍存在(非I64值域字段)
        && 节点仍存在(单位字段)
        && 节点仍存在(比较规则字段)
        && 节点仍存在(聚合规则字段)
        && 节点仍存在(名称关系字段);
}

稳定编码 新建结构节点() noexcept {
    return 全局基础数据集.新建节点(特征概念树);
}

bool 是支持的材料类型(特征概念材料物理类型 类型) noexcept {
    switch (类型) {
    case 特征概念材料物理类型::I64标量:
    case 特征概念材料物理类型::U64数组:
    case 特征概念材料物理类型::I64数组:
    case 特征概念材料物理类型::UTF8字符串:
    case 特征概念材料物理类型::二维二值格:
    case 特征概念材料物理类型::三维二值格:
        return true;
    }
    return false;
}

bool 是支持的比较规则(特征概念比较规则 规则) noexcept {
    return 规则 == 特征概念比较规则::I64数值
        || 规则 == 特征概念比较规则::完整材料精确;
}

bool 是支持的聚合规则(特征概念聚合规则 规则) noexcept {
    return 规则 == 特征概念聚合规则::不自动聚合
        || 规则 == 特征概念聚合规则::I64连续值归组
        || 规则 == 特征概念聚合规则::完整材料精确集合;
}

bool 规则与材料相容(const 特征概念定义& 定义) noexcept {
    if (!是支持的材料类型(定义.材料类型)
        || !是支持的比较规则(定义.比较规则)
        || !是支持的聚合规则(定义.聚合规则)) {
        return false;
    }
    if (是I64类型(定义.材料类型)) {
        return 定义.比较规则 == 特征概念比较规则::I64数值
            && 定义.聚合规则 != 特征概念聚合规则::完整材料精确集合
            && std::holds_alternative<std::vector<特征概念I64闭区间>>(定义.值域);
    }
    return 定义.比较规则 == 特征概念比较规则::完整材料精确
        && 定义.聚合规则 != 特征概念聚合规则::I64连续值归组
        && std::holds_alternative<std::vector<稳定编码>>(定义.值域);
}

std::optional<特征概念定义> 规范化定义(const 特征概念定义& 输入) {
    if (!规则与材料相容(输入)) return std::nullopt;

    特征概念定义 结果 = 输入;
    if (auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(&结果.值域)) {
        if (区间组->empty()) return std::nullopt;
        for (const auto& 区间 : *区间组) {
            if (区间.下界 > 区间.上界) return std::nullopt;
        }
        std::sort(区间组->begin(), 区间组->end(), [](const auto& 左, const auto& 右) {
            return 左.下界 < 右.下界
                || (左.下界 == 右.下界 && 左.上界 < 右.上界);
        });
        std::vector<特征概念I64闭区间> 归并;
        for (const auto& 区间 : *区间组) {
            if (归并.empty() || !相邻或重叠(归并.back(), 区间)) {
                归并.push_back(区间);
            } else if (区间.上界 > 归并.back().上界) {
                归并.back().上界 = 区间.上界;
            }
        }
        *区间组 = std::move(归并);
    } else {
        auto& 节点组 = std::get<std::vector<稳定编码>>(结果.值域);
        if (节点组.empty()
            || std::any_of(节点组.begin(), 节点组.end(),
                [](稳定编码 节点) { return !有效(节点); })) {
            return std::nullopt;
        }
        std::sort(节点组.begin(), 节点组.end());
        节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
    }

    if (结果.单位 && !有效(*结果.单位)) return std::nullopt;
    if (std::any_of(结果.名称关系.begin(), 结果.名称关系.end(),
        [](稳定编码 节点) { return !有效(节点); })) {
        return std::nullopt;
    }
    std::sort(结果.名称关系.begin(), 结果.名称关系.end());
    结果.名称关系.erase(
        std::unique(结果.名称关系.begin(), 结果.名称关系.end()),
        结果.名称关系.end());
    return 结果;
}

std::optional<std::int64_t> 读取唯一I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    return std::get_if<std::int64_t>(&值->材料)
        ? std::optional<std::int64_t>{std::get<std::int64_t>(值->材料)}
        : std::nullopt;
}

std::optional<std::vector<std::int64_t>> 读取唯一I64组字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* 数组 = std::get_if<std::vector<std::int64_t>>(&值->材料);
    return 数组 ? std::optional<std::vector<std::int64_t>>{*数组} : std::nullopt;
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::vector<稳定编码> 读取多节点字段(稳定编码 节点, 稳定编码 字段) {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询字段(节点, 字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&关系.内容);
        if (!目标) return {};
        结果.push_back(*目标);
    }
    std::sort(结果.begin(), 结果.end());
    结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
    return 结果;
}

void 回滚字段和节点(
    稳定编码 节点, const std::vector<稳定编码>& 字段关系组) noexcept {
    for (auto 位置 = 字段关系组.rbegin(); 位置 != 字段关系组.rend(); ++位置) {
        全局基础数据集.删除字段(*位置);
    }
    全局基础数据集.删除节点(节点);
}

bool 是I64类型(特征概念材料物理类型 类型) noexcept {
    return 类型 == 特征概念材料物理类型::I64标量;
}

std::optional<材料物理类型> 转换特征值材料类型(
    特征概念材料物理类型 类型) noexcept {
    switch (类型) {
    case 特征概念材料物理类型::U64数组:
        return 材料物理类型::U64数组;
    case 特征概念材料物理类型::I64数组:
        return 材料物理类型::I64数组;
    case 特征概念材料物理类型::UTF8字符串:
        return 材料物理类型::UTF8字符串;
    case 特征概念材料物理类型::二维二值格:
        return 材料物理类型::二维二值格;
    case 特征概念材料物理类型::三维二值格:
        return 材料物理类型::三维二值格;
    case 特征概念材料物理类型::I64标量:
        return std::nullopt;
    }
    return std::nullopt;
}

std::uint64_t 计算I64绝对差(std::int64_t 左值, std::int64_t 右值) noexcept {
    constexpr auto 符号位 = std::uint64_t{1} << 63;
    const auto 左有序值 = static_cast<std::uint64_t>(左值) ^ 符号位;
    const auto 右有序值 = static_cast<std::uint64_t>(右值) ^ 符号位;
    return 左有序值 >= 右有序值
        ? 左有序值 - 右有序值 : 右有序值 - 左有序值;
}

bool 相邻或重叠(
    const 特征概念I64闭区间& 左, const 特征概念I64闭区间& 右) noexcept {
    if (右.下界 <= 左.上界) return true;
    return 左.上界 != (std::numeric_limits<std::int64_t>::max)()
        && 右.下界 == 左.上界 + 1;
}

} // namespace

bool 特征概念比较结果::成功() const noexcept {
    return 状态 == 特征概念规则状态::已完成 && 相等.has_value();
}

bool 特征概念聚合结果::成功() const noexcept {
    return (状态 == 特征概念规则状态::已完成
        || 状态 == 特征概念规则状态::无变化)
        && 值域.has_value();
}

bool 概念_特征类::初始化() noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (结构已初始化()) return 结构仍存在();
        if (有效(特征概念树) && !节点仍存在(特征概念树)) return false;
        if (!有效(特征概念树)) {
            特征概念树 = 全局基础数据集.新建节点();
            if (!有效(特征概念树)) return false;
        }

        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 新建结构节点();
            return 有效(节点);
        };
        return 建立(节点类型字段)
            && 建立(特征概念节点类型)
            && 建立(材料类型字段)
            && 建立(I64值域字段)
            && 建立(非I64值域字段)
            && 建立(单位字段)
            && 建立(比较规则字段)
            && 建立(聚合规则字段)
            && 建立(名称关系字段);
    } catch (...) {
        return false;
    }
}

稳定编码 概念_特征类::建立或取得特征概念(
    const 特征概念定义& 定义,
    const 新_特征值类& 特征值服务,
    std::optional<稳定编码> 上位概念) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在()) return {};
        const auto 规范定义 = 规范化定义(定义);
        if (!规范定义) return {};

        const auto 父节点 = 上位概念.value_or(特征概念树);
        if (父节点 != 特征概念树 && !是特征概念节点(父节点)) return {};
        if (父节点 == 特征概念树 && !节点仍存在(父节点)) return {};
        if (规范定义->单位 && !节点仍存在(*规范定义->单位)) return {};
        for (const auto 名称 : 规范定义->名称关系) {
            if (!节点仍存在(名称)) return {};
        }

        if (const auto* 值节点组 = std::get_if<std::vector<稳定编码>>(&规范定义->值域)) {
            const auto 期望类型 = 转换特征值材料类型(规范定义->材料类型);
            if (!期望类型) return {};
            for (const auto 值节点 : *值节点组) {
                const auto 值信息 = 特征值服务.获取特征值(值节点);
                if (!值信息 || 值信息->物理类型 != *期望类型) return {};
            }
        }

        const auto 已有 = 查询特征概念(*规范定义);
        if (!已有.empty()) {
            if (已有.size() != 1) return {};
            const auto 节点 = 已有.front();
            const auto 父关系 = 全局基础数据集.查询源关系(
                父节点, 基础外部关系类型::父子);
            const bool 已挂接 = std::any_of(父关系.begin(), 父关系.end(),
                [节点](const 基础外部关系& 关系) { return 关系.目标节点 == 节点; });
            if (!已挂接 && !有效(全局基础数据集.添加关系(
                父节点, 节点, 基础外部关系类型::父子))) {
                return {};
            }
            return 节点;
        }

        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) return {};
        std::vector<稳定编码> 已写字段;
        auto 写节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
            const auto 关系 = 全局基础数据集.添加字段节点(节点, 字段, 目标);
            if (有效(关系)) 已写字段.push_back(关系);
            return 有效(关系);
        };
        auto 写I64字段 = [&](稳定编码 字段, std::int64_t 值) {
            const auto 关系 = 全局基础数据集.添加字段值(
                节点, 字段, 基础值{基础原始值{值}});
            if (有效(关系)) 已写字段.push_back(关系);
            return 有效(关系);
        };

        bool 完成 = 写节点字段(节点类型字段, 特征概念节点类型)
            && 写I64字段(材料类型字段, static_cast<std::int64_t>(规范定义->材料类型));

        if (完成) {
            if (const auto* 区间组 =
                std::get_if<std::vector<特征概念I64闭区间>>(&规范定义->值域)) {
                std::vector<std::int64_t> 展平值域;
                展平值域.reserve(区间组->size() * 2);
                for (const auto& 区间 : *区间组) {
                    展平值域.push_back(区间.下界);
                    展平值域.push_back(区间.上界);
                }
                const auto 关系 = 全局基础数据集.添加字段值(
                    节点, I64值域字段, 基础值{基础原始值{std::move(展平值域)}});
                if (有效(关系)) 已写字段.push_back(关系);
                完成 = 有效(关系);
            } else {
                for (const auto 值节点 : std::get<std::vector<稳定编码>>(规范定义->值域)) {
                    if (!写节点字段(非I64值域字段, 值节点)) {
                        完成 = false;
                        break;
                    }
                }
            }
        }
        if (完成 && 规范定义->单位) {
            完成 = 写节点字段(单位字段, *规范定义->单位);
        }
        if (完成) {
            完成 = 写I64字段(比较规则字段,
                    static_cast<std::int64_t>(规范定义->比较规则))
                && 写I64字段(聚合规则字段,
                    static_cast<std::int64_t>(规范定义->聚合规则));
        }
        if (完成) {
            for (const auto 名称 : 规范定义->名称关系) {
                if (!写节点字段(名称关系字段, 名称)) {
                    完成 = false;
                    break;
                }
            }
        }
        if (!完成) {
            回滚字段和节点(节点, 已写字段);
            return {};
        }

        if (!有效(全局基础数据集.添加关系(
            父节点, 节点, 基础外部关系类型::父子))) {
            回滚字段和节点(节点, 已写字段);
            return {};
        }
        return 节点;
    } catch (...) {
        return {};
    }
}

std::optional<特征概念信息> 概念_特征类::获取特征概念(
    稳定编码 特征概念节点) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !节点仍存在(特征概念节点)) return std::nullopt;
        const auto 类型 = 读取唯一节点字段(特征概念节点, 节点类型字段);
        if (!类型 || *类型 != 特征概念节点类型) return std::nullopt;

        const auto 材料 = 读取唯一I64字段(特征概念节点, 材料类型字段);
        const auto 比较 = 读取唯一I64字段(特征概念节点, 比较规则字段);
        const auto 聚合 = 读取唯一I64字段(特征概念节点, 聚合规则字段);
        if (!材料 || !比较 || !聚合) return std::nullopt;

        特征概念定义 定义;
        定义.材料类型 = static_cast<特征概念材料物理类型>(*材料);
        定义.比较规则 = static_cast<特征概念比较规则>(*比较);
        定义.聚合规则 = static_cast<特征概念聚合规则>(*聚合);
        if (定义.材料类型 == 特征概念材料物理类型::I64标量) {
            const auto 展平值域 = 读取唯一I64组字段(特征概念节点, I64值域字段);
            if (!展平值域 || 展平值域->empty() || 展平值域->size() % 2 != 0
                || !全局基础数据集.查询字段(特征概念节点, 非I64值域字段).empty()) {
                return std::nullopt;
            }
            std::vector<特征概念I64闭区间> 区间组;
            for (std::size_t i = 0; i < 展平值域->size(); i += 2) {
                区间组.push_back({(*展平值域)[i], (*展平值域)[i + 1]});
            }
            定义.值域 = std::move(区间组);
        } else {
            if (!全局基础数据集.查询字段(特征概念节点, I64值域字段).empty()) {
                return std::nullopt;
            }
            const auto 原始值域字段组 =
                全局基础数据集.查询字段(特征概念节点, 非I64值域字段);
            auto 值域 = 读取多节点字段(特征概念节点, 非I64值域字段);
            if (值域.empty() || 值域.size() != 原始值域字段组.size()) {
                return std::nullopt;
            }
            定义.值域 = std::move(值域);
        }

        const auto 单位组 = 全局基础数据集.查询字段(特征概念节点, 单位字段);
        if (单位组.size() > 1) return std::nullopt;
        if (!单位组.empty()) {
            const auto* 单位 = std::get_if<稳定编码>(&单位组.front().内容);
            if (!单位) return std::nullopt;
            定义.单位 = *单位;
        }
        定义.名称关系 = 读取多节点字段(特征概念节点, 名称关系字段);
        if (定义.名称关系.size()
            != 全局基础数据集.查询字段(特征概念节点, 名称关系字段).size()) {
            return std::nullopt;
        }

        const auto 规范定义 = 规范化定义(定义);
        if (!规范定义) return std::nullopt;
        return 特征概念信息{
            特征概念节点,
            规范定义->材料类型,
            规范定义->值域,
            规范定义->单位,
            规范定义->比较规则,
            规范定义->聚合规则,
            规范定义->名称关系};
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<稳定编码> 概念_特征类::查询特征概念(
    const 特征概念定义& 定义) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto 规范定义 = 规范化定义(定义);
        if (!结构仍存在() || !规范定义) return {};

        std::vector<稳定编码> 待查{特征概念树};
        std::vector<稳定编码> 已查;
        std::vector<稳定编码> 结果;
        while (!待查.empty()) {
            const auto 父节点 = 待查.back();
            待查.pop_back();
            if (std::find(已查.begin(), 已查.end(), 父节点) != 已查.end()) continue;
            已查.push_back(父节点);
            for (const auto& 关系 : 全局基础数据集.查询源关系(
                父节点, 基础外部关系类型::父子)) {
                const auto 子节点 = 关系.目标节点;
                待查.push_back(子节点);
                const auto 信息 = 获取特征概念(子节点);
                if (!信息) continue;
                const 特征概念定义 候选{
                    信息->材料类型, 信息->值域, 信息->单位,
                    信息->比较规则, 信息->聚合规则, 信息->名称关系};
                if (候选 == *规范定义) 结果.push_back(子节点);
            }
        }
        std::sort(结果.begin(), 结果.end());
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    } catch (...) {
        return {};
    }
}

bool 概念_特征类::是特征概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !节点仍存在(节点)) return false;
        const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
        return 类型 && *类型 == 特征概念节点类型;
    } catch (...) {
        return false;
    }
}

特征概念比较结果 概念_特征类::执行比较规则(
    特征概念材料物理类型 材料类型,
    特征概念比较规则 规则,
    const 特征概念准确值& 左值,
    const 特征概念准确值& 右值,
    const 新_特征值类& 特征值服务) const noexcept {
    特征概念比较结果 结果;
    try {
        if (规则 == 特征概念比较规则::I64数值) {
            if (!是I64类型(材料类型)) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            const auto* 左 = std::get_if<std::int64_t>(&左值);
            const auto* 右 = std::get_if<std::int64_t>(&右值);
            if (!左 || !右) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            结果.次序 = *左 < *右 ? 特征概念比较次序::小于
                : *左 > *右 ? 特征概念比较次序::大于
                : 特征概念比较次序::相等;
            结果.相等 = *左 == *右;
            结果.I64绝对差 = 计算I64绝对差(*左, *右);
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }

        if (规则 != 特征概念比较规则::完整材料精确
            || 是I64类型(材料类型)) {
            结果.状态 = 特征概念规则状态::规则不支持;
            return 结果;
        }
        const auto 期望类型 = 转换特征值材料类型(材料类型);
        const auto* 左节点 = std::get_if<稳定编码>(&左值);
        const auto* 右节点 = std::get_if<稳定编码>(&右值);
        if (!期望类型 || !左节点 || !右节点
            || !有效(*左节点) || !有效(*右节点)) {
            结果.状态 = 特征概念规则状态::材料类型不相容;
            return 结果;
        }
        const auto 左信息 = 特征值服务.获取特征值(*左节点);
        const auto 右信息 = 特征值服务.获取特征值(*右节点);
        if (!左信息 || !右信息) {
            结果.状态 = 特征概念规则状态::值不存在;
            return 结果;
        }
        if (左信息->物理类型 != *期望类型 || 右信息->物理类型 != *期望类型) {
            结果.状态 = 特征概念规则状态::材料类型不相容;
            return 结果;
        }
        结果.相等 = 左信息->材料 == 右信息->材料;
        if (*结果.相等) 结果.次序 = 特征概念比较次序::相等;
        结果.状态 = 特征概念规则状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 特征概念规则状态::资源失败;
        结果.相等.reset();
        结果.次序.reset();
        结果.I64绝对差.reset();
        return 结果;
    }
}

特征概念聚合结果 概念_特征类::执行聚合规则(
    特征概念材料物理类型 材料类型,
    特征概念聚合规则 规则,
    const std::vector<特征概念准确值>& 历史不同值,
    const 新_特征值类& 特征值服务) const noexcept {
    特征概念聚合结果 结果;
    try {
        if (历史不同值.empty()) return 结果;

        if (规则 == 特征概念聚合规则::不自动聚合) {
            if (是I64类型(材料类型)) {
                std::vector<特征概念I64闭区间> 域;
                for (const auto& 值 : 历史不同值) {
                    const auto* I64 = std::get_if<std::int64_t>(&值);
                    if (!I64) {
                        结果.状态 = 特征概念规则状态::材料类型不相容;
                        return 结果;
                    }
                    域.push_back({*I64, *I64});
                }
                std::sort(域.begin(), 域.end(), [](const auto& 左, const auto& 右) {
                    return 左.下界 < 右.下界;
                });
                域.erase(std::unique(域.begin(), 域.end()), 域.end());
                结果.值域 = std::move(域);
            } else {
                std::vector<稳定编码> 域;
                const auto 期望类型 = 转换特征值材料类型(材料类型);
                if (!期望类型) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                for (const auto& 值 : 历史不同值) {
                    const auto* 节点 = std::get_if<稳定编码>(&值);
                    if (!节点 || !有效(*节点)) {
                        结果.状态 = 特征概念规则状态::材料类型不相容;
                        return 结果;
                    }
                    const auto 信息 = 特征值服务.获取特征值(*节点);
                    if (!信息) {
                        结果.状态 = 特征概念规则状态::值不存在;
                        return 结果;
                    }
                    if (信息->物理类型 != *期望类型) {
                        结果.状态 = 特征概念规则状态::材料类型不相容;
                        return 结果;
                    }
                    域.push_back(*节点);
                }
                std::sort(域.begin(), 域.end(), [](稳定编码 左, 稳定编码 右) {
                    return 左.值 < 右.值;
                });
                域.erase(std::unique(域.begin(), 域.end()), 域.end());
                结果.值域 = std::move(域);
            }
            结果.状态 = 特征概念规则状态::无变化;
            return 结果;
        }

        if (规则 == 特征概念聚合规则::I64连续值归组) {
            if (!是I64类型(材料类型)) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            std::vector<std::int64_t> 值组;
            值组.reserve(历史不同值.size());
            for (const auto& 值 : 历史不同值) {
                const auto* I64 = std::get_if<std::int64_t>(&值);
                if (!I64) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                值组.push_back(*I64);
            }
            std::sort(值组.begin(), 值组.end());
            值组.erase(std::unique(值组.begin(), 值组.end()), 值组.end());
            std::vector<特征概念I64闭区间> 域;
            for (const auto 值 : 值组) {
                const 特征概念I64闭区间 单值{值, 值};
                if (域.empty() || !相邻或重叠(域.back(), 单值)) {
                    域.push_back(单值);
                } else {
                    域.back().上界 = 值;
                }
            }
            结果.值域 = std::move(域);
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }

        if (规则 == 特征概念聚合规则::完整材料精确集合) {
            if (是I64类型(材料类型)) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            std::vector<稳定编码> 域;
            const auto 期望类型 = 转换特征值材料类型(材料类型);
            if (!期望类型) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            for (const auto& 值 : 历史不同值) {
                const auto* 节点 = std::get_if<稳定编码>(&值);
                if (!节点 || !有效(*节点)) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                const auto 信息 = 特征值服务.获取特征值(*节点);
                if (!信息) {
                    结果.状态 = 特征概念规则状态::值不存在;
                    return 结果;
                }
                if (信息->物理类型 != *期望类型) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                域.push_back(*节点);
            }
            std::sort(域.begin(), 域.end(), [](稳定编码 左, 稳定编码 右) {
                return 左.值 < 右.值;
            });
            域.erase(std::unique(域.begin(), 域.end()), 域.end());
            结果.值域 = std::move(域);
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }

        结果.状态 = 特征概念规则状态::规则不支持;
        return 结果;
    } catch (...) {
        结果.状态 = 特征概念规则状态::资源失败;
        结果.值域.reset();
        return 结果;
    }
}

} // namespace 海中鱼巣
