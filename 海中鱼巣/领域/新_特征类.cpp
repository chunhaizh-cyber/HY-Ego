#include "新_特征类.h"

#include <algorithm>
#include <mutex>

namespace 海中鱼巣 {

namespace {

std::recursive_mutex 新特征互斥;

稳定编码 节点类型字段{};
稳定编码 特征节点类型{};
稳定编码 特征概念字段{};
稳定编码 I64实例值域字段{};
稳定编码 非I64实例值域字段{};
稳定编码 实例单位字段{};
稳定编码 当前值字段{};
稳定编码 历史值字段{};

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已初始化() noexcept {
    return 有效(节点类型字段)
        && 有效(特征节点类型)
        && 有效(特征概念字段)
        && 有效(I64实例值域字段)
        && 有效(非I64实例值域字段)
        && 有效(实例单位字段)
        && 有效(当前值字段)
        && 有效(历史值字段);
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(节点类型字段)
        && 节点仍存在(特征节点类型)
        && 节点仍存在(特征概念字段)
        && 节点仍存在(I64实例值域字段)
        && 节点仍存在(非I64实例值域字段)
        && 节点仍存在(实例单位字段)
        && 节点仍存在(当前值字段)
        && 节点仍存在(历史值字段);
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<新特征准确值> 解码准确值(const 基础字段关系& 字段) {
    if (const auto* 目标 = std::get_if<稳定编码>(&字段.内容)) {
        return 新特征准确值{*目标};
    }
    const auto* 值 = std::get_if<基础值>(&字段.内容);
    if (!值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&值->材料);
    return I64 ? std::optional<新特征准确值>{新特征准确值{*I64}}
        : std::nullopt;
}

稳定编码 写入准确值字段(
    稳定编码 节点, 稳定编码 字段, const 新特征准确值& 值) noexcept {
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        return 全局基础数据集.添加字段值(
            节点, 字段, 基础值{基础原始值{*I64}});
    }
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    return 值节点
        ? 全局基础数据集.添加字段节点(节点, 字段, *值节点)
        : 稳定编码{};
}

bool 修改准确值字段(
    稳定编码 字段关系, const 新特征准确值& 值) noexcept {
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        return 全局基础数据集.修改字段值(
            字段关系, 基础值{基础原始值{*I64}});
    }
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    return 值节点 && 全局基础数据集.修改字段节点(字段关系, *值节点);
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

bool 准确值材料相容(
    const 特征概念信息& 概念,
    const 新特征准确值& 值,
    const 新_特征值类& 特征值服务) noexcept {
    if (概念.材料类型 == 特征概念材料物理类型::I64标量) {
        return std::holds_alternative<std::int64_t>(值);
    }
    const auto 期望类型 = 转换特征值材料类型(概念.材料类型);
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    if (!期望类型 || !值节点 || !有效(*值节点)) return false;
    const auto 信息 = 特征值服务.获取特征值(*值节点);
    return 信息 && 信息->物理类型 == *期望类型;
}

std::vector<std::int64_t> 展平I64值域(
    const std::vector<特征概念I64闭区间>& 区间组) {
    std::vector<std::int64_t> 结果;
    结果.reserve(区间组.size() * 2);
    for (const auto& 区间 : 区间组) {
        结果.push_back(区间.下界);
        结果.push_back(区间.上界);
    }
    return 结果;
}

bool 写入实例值域(
    稳定编码 特征节点,
    const 特征概念值域& 值域,
    std::vector<稳定编码>& 已写字段) {
    if (const auto* I64域 =
        std::get_if<std::vector<特征概念I64闭区间>>(&值域)) {
        const auto 关系 = 全局基础数据集.添加字段值(
            特征节点, I64实例值域字段,
            基础值{基础原始值{展平I64值域(*I64域)}});
        if (有效(关系)) 已写字段.push_back(关系);
        return 有效(关系);
    }
    for (const auto 值节点 : std::get<std::vector<稳定编码>>(值域)) {
        const auto 关系 = 全局基础数据集.添加字段节点(
            特征节点, 非I64实例值域字段, 值节点);
        if (!有效(关系)) return false;
        已写字段.push_back(关系);
    }
    return true;
}

std::optional<特征概念值域> 读取实例值域(
    稳定编码 特征节点,
    特征概念材料物理类型 材料类型) {
    const auto I64字段组 = 全局基础数据集.查询字段(
        特征节点, I64实例值域字段);
    const auto 非I64字段组 = 全局基础数据集.查询字段(
        特征节点, 非I64实例值域字段);
    if (材料类型 == 特征概念材料物理类型::I64标量) {
        if (I64字段组.size() != 1 || !非I64字段组.empty()) return std::nullopt;
        const auto* 值 = std::get_if<基础值>(&I64字段组.front().内容);
        if (!值) return std::nullopt;
        const auto* 展平 = std::get_if<std::vector<std::int64_t>>(&值->材料);
        if (!展平 || 展平->empty() || 展平->size() % 2 != 0) return std::nullopt;
        std::vector<特征概念I64闭区间> 区间组;
        for (std::size_t i = 0; i < 展平->size(); i += 2) {
            if ((*展平)[i] > (*展平)[i + 1]) return std::nullopt;
            区间组.push_back({(*展平)[i], (*展平)[i + 1]});
        }
        return 特征概念值域{std::move(区间组)};
    }

    if (!I64字段组.empty() || 非I64字段组.empty()) return std::nullopt;
    std::vector<稳定编码> 节点组;
    for (const auto& 字段 : 非I64字段组) {
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (!目标) return std::nullopt;
        节点组.push_back(*目标);
    }
    std::sort(节点组.begin(), 节点组.end());
    const auto 去重末尾 = std::unique(节点组.begin(), 节点组.end());
    if (去重末尾 != 节点组.end()) return std::nullopt;
    return 特征概念值域{std::move(节点组)};
}

bool 写回实例值域(
    稳定编码 特征节点,
    const 特征概念值域& 新值域) noexcept {
    if (const auto* I64域 =
        std::get_if<std::vector<特征概念I64闭区间>>(&新值域)) {
        const auto I64字段组 = 全局基础数据集.查询字段(
            特征节点, I64实例值域字段);
        if (I64字段组.size() != 1
            || !全局基础数据集.查询字段(
                特征节点, 非I64实例值域字段).empty()) {
            return false;
        }
        return 全局基础数据集.修改字段值(
            I64字段组.front().编码,
            基础值{基础原始值{展平I64值域(*I64域)}});
    }

    if (!全局基础数据集.查询字段(
        特征节点, I64实例值域字段).empty()) {
        return false;
    }
    const auto& 目标节点组 = std::get<std::vector<稳定编码>>(新值域);
    std::vector<稳定编码> 已有节点组;
    for (const auto& 字段 : 全局基础数据集.查询字段(
        特征节点, 非I64实例值域字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (!目标) return false;
        已有节点组.push_back(*目标);
    }
    std::sort(已有节点组.begin(), 已有节点组.end());
    if (std::unique(已有节点组.begin(), 已有节点组.end()) != 已有节点组.end()) {
        return false;
    }
    if (!std::all_of(已有节点组.begin(), 已有节点组.end(),
        [&](稳定编码 节点) {
            return std::binary_search(目标节点组.begin(), 目标节点组.end(), 节点);
        })) {
        return false;
    }

    std::vector<稳定编码> 新增字段;
    for (const auto 节点 : 目标节点组) {
        if (std::binary_search(已有节点组.begin(), 已有节点组.end(), 节点)) continue;
        const auto 字段 = 全局基础数据集.添加字段节点(
            特征节点, 非I64实例值域字段, 节点);
        if (!有效(字段)) {
            for (auto 位置 = 新增字段.rbegin(); 位置 != 新增字段.rend(); ++位置) {
                全局基础数据集.删除字段(*位置);
            }
            return false;
        }
        新增字段.push_back(字段);
    }
    return true;
}

void 回滚字段和节点(
    稳定编码 节点, const std::vector<稳定编码>& 字段关系组) noexcept {
    for (auto 位置 = 字段关系组.rbegin(); 位置 != 字段关系组.rend(); ++位置) {
        全局基础数据集.删除字段(*位置);
    }
    全局基础数据集.删除节点(节点);
}

} // namespace

新_特征类::新_特征类(
    新_特征值类& 特征值服务,
    概念_特征类& 特征概念服务) noexcept
    : 特征值服务_(特征值服务), 特征概念服务_(特征概念服务) {}

bool 新_特征类::初始化() noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!特征值服务_.初始化() || !特征概念服务_.初始化()) return false;
        if (结构已初始化()) return 结构仍存在();

        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        return 建立(节点类型字段)
            && 建立(特征节点类型)
            && 建立(特征概念字段)
            && 建立(I64实例值域字段)
            && 建立(非I64实例值域字段)
            && 建立(实例单位字段)
            && 建立(当前值字段)
            && 建立(历史值字段);
    } catch (...) {
        return false;
    }
}

稳定编码 新_特征类::建立或取得特征(
    稳定编码 持有节点,
    稳定编码 特征概念节点) noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在()
            || !节点仍存在(持有节点)
            || !特征概念服务_.是特征概念节点(特征概念节点)) {
            return {};
        }
        const auto 概念信息 = 特征概念服务_.获取特征概念(特征概念节点);
        if (!概念信息) return {};

        std::vector<稳定编码> 已有;
        for (const auto& 关系 : 全局基础数据集.查询源关系(
            持有节点, 基础外部关系类型::父子)) {
            const auto 类型 = 读取唯一节点字段(关系.目标节点, 节点类型字段);
            const auto 概念 = 读取唯一节点字段(关系.目标节点, 特征概念字段);
            if (类型 && *类型 == 特征节点类型
                && 概念 && *概念 == 特征概念节点) {
                已有.push_back(关系.目标节点);
            }
        }
        std::sort(已有.begin(), 已有.end());
        已有.erase(std::unique(已有.begin(), 已有.end()), 已有.end());
        if (已有.size() == 1) {
            return 获取特征(已有.front())
                ? 已有.front() : 稳定编码{};
        }
        if (!已有.empty()) return {};

        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) return {};
        std::vector<稳定编码> 已写字段;
        const auto 类型关系 = 全局基础数据集.添加字段节点(
            节点, 节点类型字段, 特征节点类型);
        if (有效(类型关系)) 已写字段.push_back(类型关系);
        const auto 概念关系 = 有效(类型关系)
            ? 全局基础数据集.添加字段节点(
                节点, 特征概念字段, 特征概念节点)
            : 稳定编码{};
        if (有效(概念关系)) 已写字段.push_back(概念关系);
        bool 完成 = 有效(类型关系) && 有效(概念关系);
        if (完成) {
            完成 = 写入实例值域(节点, 概念信息->值域, 已写字段);
        }
        if (完成 && 概念信息->单位) {
            const auto 单位关系 = 全局基础数据集.添加字段节点(
                节点, 实例单位字段, *概念信息->单位);
            if (有效(单位关系)) 已写字段.push_back(单位关系);
            完成 = 有效(单位关系);
        }
        if (!完成) {
            回滚字段和节点(节点, 已写字段);
            return {};
        }
        if (!有效(全局基础数据集.添加关系(
            持有节点, 节点, 基础外部关系类型::父子))) {
            回滚字段和节点(节点, 已写字段);
            return {};
        }
        return 节点;
    } catch (...) {
        return {};
    }
}

std::optional<稳定编码> 新_特征类::查询特征(
    稳定编码 持有节点,
    稳定编码 特征概念节点) const noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !节点仍存在(持有节点)
            || !特征概念服务_.是特征概念节点(特征概念节点)) {
            return std::nullopt;
        }
        std::vector<稳定编码> 结果;
        for (const auto& 关系 : 全局基础数据集.查询源关系(
            持有节点, 基础外部关系类型::父子)) {
            const auto 类型 = 读取唯一节点字段(关系.目标节点, 节点类型字段);
            const auto 概念 = 读取唯一节点字段(关系.目标节点, 特征概念字段);
            if (类型 && *类型 == 特征节点类型
                && 概念 && *概念 == 特征概念节点) {
                结果.push_back(关系.目标节点);
            }
        }
        std::sort(结果.begin(), 结果.end());
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果.size() == 1
            ? std::optional<稳定编码>{结果.front()} : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新特征信息> 新_特征类::获取特征(
    稳定编码 特征节点) const noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !节点仍存在(特征节点)) return std::nullopt;
        const auto 类型 = 读取唯一节点字段(特征节点, 节点类型字段);
        const auto 概念 = 读取唯一节点字段(特征节点, 特征概念字段);
        if (!类型 || *类型 != 特征节点类型 || !概念
            || !特征概念服务_.是特征概念节点(*概念)) {
            return std::nullopt;
        }
        const auto 概念信息 = 特征概念服务_.获取特征概念(*概念);
        if (!概念信息) return std::nullopt;

        const auto 父关系 = 全局基础数据集.查询目标关系(
            特征节点, 基础外部关系类型::父子);
        if (父关系.size() != 1) return std::nullopt;

        新特征信息 结果;
        结果.节点 = 特征节点;
        结果.持有节点 = 父关系.front().源节点;
        结果.特征概念节点 = *概念;
        const auto 实例值域 = 读取实例值域(特征节点, 概念信息->材料类型);
        if (!实例值域) return std::nullopt;
        if (const auto* 值节点组 = std::get_if<std::vector<稳定编码>>(&*实例值域)) {
            for (const auto 值节点 : *值节点组) {
                if (!准确值材料相容(
                    *概念信息, 新特征准确值{值节点}, 特征值服务_)) {
                    return std::nullopt;
                }
            }
        }
        结果.实例值域 = *实例值域;

        const auto 单位字段组 = 全局基础数据集.查询字段(
            特征节点, 实例单位字段);
        if (单位字段组.size() > 1) return std::nullopt;
        if (概念信息->单位.has_value() != !单位字段组.empty()) {
            return std::nullopt;
        }
        if (概念信息->单位) {
            const auto* 单位节点 = std::get_if<稳定编码>(&单位字段组.front().内容);
            if (!单位节点 || !节点仍存在(*单位节点)
                || *单位节点 != *概念信息->单位) {
                return std::nullopt;
            }
            结果.实例单位 = *单位节点;
        }

        const auto 当前字段组 = 全局基础数据集.查询字段(特征节点, 当前值字段);
        if (当前字段组.size() > 1) return std::nullopt;
        if (!当前字段组.empty()) {
            结果.当前值 = 解码准确值(当前字段组.front());
            if (!结果.当前值) return std::nullopt;
        }

        for (const auto& 字段 : 全局基础数据集.查询字段(特征节点, 历史值字段)) {
            const auto 值 = 解码准确值(字段);
            if (!值 || !准确值材料相容(*概念信息, *值, 特征值服务_)) {
                return std::nullopt;
            }
            结果.历史不同值.push_back(*值);
        }
        std::sort(结果.历史不同值.begin(), 结果.历史不同值.end(),
            [](const 新特征准确值& 左, const 新特征准确值& 右) {
                if (左.index() != 右.index()) return 左.index() < 右.index();
                if (const auto* 左I64 = std::get_if<std::int64_t>(&左)) {
                    return *左I64 < std::get<std::int64_t>(右);
                }
                return std::get<稳定编码>(左) < std::get<稳定编码>(右);
            });
        if (结果.当前值
            && !准确值材料相容(*概念信息, *结果.当前值, 特征值服务_)) {
            return std::nullopt;
        }

        auto 值相等 = [&](const 新特征准确值& 左, const 新特征准确值& 右)
            -> std::optional<bool> {
            const auto 比较 = 特征概念服务_.执行比较规则(
                概念信息->材料类型, 概念信息->比较规则,
                左, 右, 特征值服务_);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.相等) {
                return std::nullopt;
            }
            return *比较.相等;
        };
        for (std::size_t 左 = 0; 左 < 结果.历史不同值.size(); ++左) {
            for (std::size_t 右 = 左 + 1; 右 < 结果.历史不同值.size(); ++右) {
                const auto 相等 = 值相等(
                    结果.历史不同值[左], 结果.历史不同值[右]);
                if (!相等 || *相等) return std::nullopt;
            }
        }
        if (结果.当前值) {
            bool 在历史中 = false;
            for (const auto& 历史值 : 结果.历史不同值) {
                const auto 相等 = 值相等(*结果.当前值, 历史值);
                if (!相等) return std::nullopt;
                if (*相等) {
                    在历史中 = true;
                    break;
                }
            }
            if (!在历史中) return std::nullopt;
        }
        return 结果;
    } catch (...) {
        return std::nullopt;
    }
}

bool 新_特征类::是特征节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !节点仍存在(节点)) return false;
        const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
        return 类型 && *类型 == 特征节点类型;
    } catch (...) {
        return false;
    }
}

新特征值添加结果 新_特征类::添加特征值(
    稳定编码 特征节点,
    const 新特征准确值& 新值) noexcept {
    新特征值添加结果 结果;
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !有效(特征节点)) return 结果;
        const auto 特征 = 获取特征(特征节点);
        if (!特征) {
            结果.状态 = 节点仍存在(特征节点)
                ? 新特征写入状态::结构不一致
                : 新特征写入状态::特征不存在;
            return 结果;
        }
        const auto 概念 = 特征概念服务_.获取特征概念(特征->特征概念节点);
        if (!概念) {
            结果.状态 = 新特征写入状态::概念不存在;
            return 结果;
        }
        if (!准确值材料相容(*概念, 新值, 特征值服务_)) {
            结果.状态 = 新特征写入状态::材料类型不相容;
            return 结果;
        }

        auto 值相等 = [&](const 新特征准确值& 左, const 新特征准确值& 右)
            -> std::optional<bool> {
            const auto 比较 = 特征概念服务_.执行比较规则(
                概念->材料类型, 概念->比较规则, 左, 右, 特征值服务_);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.相等) {
                return std::nullopt;
            }
            return *比较.相等;
        };

        bool 历史已有 = false;
        for (const auto& 历史值 : 特征->历史不同值) {
            const auto 相等 = 值相等(历史值, 新值);
            if (!相等) {
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            if (*相等) {
                历史已有 = true;
                break;
            }
        }

        稳定编码 新历史字段关系{};
        if (!历史已有) {
            新历史字段关系 = 写入准确值字段(特征节点, 历史值字段, 新值);
            if (!有效(新历史字段关系)) {
                结果.状态 = 新特征写入状态::资源失败;
                return 结果;
            }
            结果.历史集合已增加 = true;
        }

        bool 当前相同 = false;
        if (特征->当前值) {
            const auto 相等 = 值相等(*特征->当前值, 新值);
            if (!相等) {
                if (有效(新历史字段关系)) 全局基础数据集.删除字段(新历史字段关系);
                结果.历史集合已增加 = false;
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            当前相同 = *相等;
        }

        if (!当前相同) {
            const auto 当前字段组 = 全局基础数据集.查询字段(特征节点, 当前值字段);
            bool 写入成功 = false;
            if (当前字段组.empty()) {
                写入成功 = 有效(写入准确值字段(特征节点, 当前值字段, 新值));
            } else if (当前字段组.size() == 1) {
                写入成功 = 修改准确值字段(当前字段组.front().编码, 新值);
            }
            if (!写入成功) {
                if (有效(新历史字段关系)) 全局基础数据集.删除字段(新历史字段关系);
                结果.历史集合已增加 = false;
                结果.状态 = 当前字段组.size() > 1
                    ? 新特征写入状态::结构不一致
                    : 新特征写入状态::资源失败;
                return 结果;
            }
            结果.当前值已改变 = true;
        }

        auto 聚合输入 = 特征->历史不同值;
        if (结果.历史集合已增加) 聚合输入.push_back(新值);
        结果.聚合结果 = 特征概念服务_.执行聚合规则(
            概念->材料类型, 概念->聚合规则, 聚合输入, 特征值服务_);
        if (结果.聚合结果->状态 != 特征概念规则状态::已完成
            && 结果.聚合结果->状态 != 特征概念规则状态::无变化) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        if (!结果.聚合结果->值域) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        const bool 值域需要写回 = 特征->实例值域 != *结果.聚合结果->值域;
        if (值域需要写回
            && !写回实例值域(特征节点, *结果.聚合结果->值域)) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        if (!结果.历史集合已增加 && !结果.当前值已改变 && !值域需要写回) {
            结果.状态 = 新特征写入状态::无变化;
            return 结果;
        }
        结果.状态 = 新特征写入状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 结果.历史集合已增加 || 结果.当前值已改变
            ? 新特征写入状态::值已更新但聚合未完成
            : 新特征写入状态::资源失败;
        return 结果;
    }
}

} // namespace 海中鱼巣
