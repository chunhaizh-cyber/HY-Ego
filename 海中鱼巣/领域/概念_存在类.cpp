#include "概念_存在类.h"

#include <algorithm>
#include <mutex>
#include <set>

namespace 海中鱼巣 {

稳定编码 存在概念图根节点{};

namespace {

std::recursive_mutex 存在概念图互斥;

稳定编码 节点类型字段{};
稳定编码 专属存在概念节点类型{};
稳定编码 抽象存在概念节点类型{};
稳定编码 对应存在字段{};
稳定编码 特征项字段{};
稳定编码 特征项节点类型{};
稳定编码 特征类型概念字段{};
稳定编码 具体特征概念字段{};

constexpr std::int64_t 组织归属关系角色 = 1;
constexpr std::int64_t 概念上下位关系角色 = 2;

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已初始化() noexcept {
    return 有效(存在概念图根节点)
        && 有效(节点类型字段)
        && 有效(专属存在概念节点类型)
        && 有效(抽象存在概念节点类型)
        && 有效(对应存在字段)
        && 有效(特征项字段)
        && 有效(特征项节点类型)
        && 有效(特征类型概念字段)
        && 有效(具体特征概念字段);
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(存在概念图根节点)
        && 节点仍存在(节点类型字段)
        && 节点仍存在(专属存在概念节点类型)
        && 节点仍存在(抽象存在概念节点类型)
        && 节点仍存在(对应存在字段)
        && 节点仍存在(特征项字段)
        && 节点仍存在(特征项节点类型)
        && 节点仍存在(特征类型概念字段)
        && 节点仍存在(具体特征概念字段);
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<存在概念种类> 读取概念种类(稳定编码 节点) {
    const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
    if (!类型) return std::nullopt;
    if (*类型 == 专属存在概念节点类型) return 存在概念种类::专属;
    if (*类型 == 抽象存在概念节点类型) return 存在概念种类::抽象;
    return std::nullopt;
}

void 排序去重(std::vector<稳定编码>& 节点组) {
    std::sort(节点组.begin(), 节点组.end());
    节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
}

void 排序特征项(std::vector<存在概念特征项>& 特征组) {
    std::sort(特征组.begin(), 特征组.end(), [](const auto& 左, const auto& 右) {
        return 左.特征类型概念 < 右.特征类型概念
            || (左.特征类型概念 == 右.特征类型概念
                && 左.具体特征概念 < 右.具体特征概念);
    });
}

std::optional<std::vector<存在概念特征项>> 规范化特征组(
    const std::vector<稳定编码>& 具体特征概念组,
    概念_特征类& 特征概念服务) {
    std::vector<存在概念特征项> 结果;
    for (const auto 具体概念 : 具体特征概念组) {
        if (!特征概念服务.是特征概念节点(具体概念)) return std::nullopt;
        const auto 定位 = 特征概念服务.比较单特征概念(
            具体概念, 具体概念);
        if (定位.状态 != 特征概念规则状态::已完成
            || !定位.特征类型根) {
            return std::nullopt;
        }
        结果.push_back({*定位.特征类型根, 具体概念});
    }
    排序特征项(结果);
    for (std::size_t i = 1; i < 结果.size(); ++i) {
        if (结果[i - 1].特征类型概念 == 结果[i].特征类型概念) {
            return std::nullopt;
        }
    }
    return 结果;
}

std::optional<std::vector<存在概念特征项>> 读取特征组(
    稳定编码 概念节点,
    概念_特征类& 特征概念服务) {
    std::vector<存在概念特征项> 结果;
    for (const auto& 持有字段 : 全局基础数据集.查询字段(
        概念节点, 特征项字段)) {
        const auto* 特征项节点 = std::get_if<稳定编码>(&持有字段.内容);
        if (!特征项节点 || !节点仍存在(*特征项节点)) return std::nullopt;
        const auto 项类型 = 读取唯一节点字段(*特征项节点, 节点类型字段);
        const auto 特征类型 = 读取唯一节点字段(
            *特征项节点, 特征类型概念字段);
        const auto 具体概念 = 读取唯一节点字段(
            *特征项节点, 具体特征概念字段);
        if (!项类型 || *项类型 != 特征项节点类型
            || !特征类型 || !具体概念
            || !特征概念服务.是特征概念节点(*具体概念)) {
            return std::nullopt;
        }
        const auto 定位 = 特征概念服务.比较单特征概念(
            *具体概念, *具体概念);
        if (定位.状态 != 特征概念规则状态::已完成
            || !定位.特征类型根 || *定位.特征类型根 != *特征类型) {
            return std::nullopt;
        }
        结果.push_back({*特征类型, *具体概念});
    }
    排序特征项(结果);
    for (std::size_t i = 1; i < 结果.size(); ++i) {
        if (结果[i - 1].特征类型概念 == 结果[i].特征类型概念) {
            return std::nullopt;
        }
    }
    return 结果;
}

struct 已建特征项 final {
    稳定编码 节点;
    std::vector<稳定编码> 字段组;
    std::optional<稳定编码> 持有字段;
};

std::optional<std::vector<已建特征项>> 建立特征项节点组(
    const std::vector<存在概念特征项>& 特征组) {
    std::vector<已建特征项> 结果;
    const auto 清理 = [&]() noexcept {
        for (auto 位置 = 结果.rbegin(); 位置 != 结果.rend(); ++位置) {
            for (auto 字段 = 位置->字段组.rbegin();
                字段 != 位置->字段组.rend(); ++字段) {
                (void)全局基础数据集.删除字段(*字段);
            }
            (void)全局基础数据集.删除节点(位置->节点);
        }
    };
    for (const auto& 特征 : 特征组) {
        已建特征项 已建;
        已建.节点 = 全局基础数据集.新建节点();
        if (!有效(已建.节点)) {
            清理();
            return std::nullopt;
        }
        const auto 类型字段 = 全局基础数据集.添加字段节点(
            已建.节点, 节点类型字段, 特征项节点类型);
        if (有效(类型字段)) 已建.字段组.push_back(类型字段);
        const auto 类型概念字段 = 有效(类型字段)
            ? 全局基础数据集.添加字段节点(
                已建.节点, 特征类型概念字段, 特征.特征类型概念)
            : 稳定编码{};
        if (有效(类型概念字段)) 已建.字段组.push_back(类型概念字段);
        const auto 具体概念字段 = 有效(类型概念字段)
            ? 全局基础数据集.添加字段节点(
                已建.节点, 具体特征概念字段, 特征.具体特征概念)
            : 稳定编码{};
        if (有效(具体概念字段)) 已建.字段组.push_back(具体概念字段);
        结果.push_back(std::move(已建));
        if (!有效(具体概念字段)) {
            清理();
            return std::nullopt;
        }
    }
    return 结果;
}

void 删除特征项节点组(std::vector<已建特征项>& 特征项组) noexcept {
    for (auto 位置 = 特征项组.rbegin(); 位置 != 特征项组.rend(); ++位置) {
        if (位置->持有字段) {
            (void)全局基础数据集.删除字段(*位置->持有字段);
        }
        for (auto 字段 = 位置->字段组.rbegin(); 字段 != 位置->字段组.rend(); ++字段) {
            (void)全局基础数据集.删除字段(*字段);
        }
        (void)全局基础数据集.删除节点(位置->节点);
    }
}

bool 挂接特征项节点组(
    稳定编码 概念节点,
    std::vector<已建特征项>& 特征项组) noexcept {
    for (auto& 特征项 : 特征项组) {
        const auto 持有字段 = 全局基础数据集.添加字段节点(
            概念节点, 特征项字段, 特征项.节点);
        if (!有效(持有字段)) return false;
        特征项.持有字段 = 持有字段;
    }
    return true;
}

void 删除既有特征项(稳定编码 概念节点) noexcept {
    for (const auto& 持有字段 : 全局基础数据集.查询字段(
        概念节点, 特征项字段)) {
        const auto* 特征项节点 = std::get_if<稳定编码>(&持有字段.内容);
        (void)全局基础数据集.删除字段(持有字段.编码);
        if (!特征项节点) continue;
        for (const auto 字段 : {
            节点类型字段, 特征类型概念字段, 具体特征概念字段}) {
            for (const auto& 关系 : 全局基础数据集.查询字段(
                *特征项节点, 字段)) {
                (void)全局基础数据集.删除字段(关系.编码);
            }
        }
        (void)全局基础数据集.删除节点(*特征项节点);
    }
}

std::vector<基础外部关系> 查询概念关系(
    稳定编码 节点, bool 查询上位) {
    std::vector<基础外部关系> 结果;
    const auto 关系组 = 查询上位
        ? 全局基础数据集.查询目标关系(节点, 基础外部关系类型::父子)
        : 全局基础数据集.查询源关系(节点, 基础外部关系类型::父子);
    for (const auto& 关系 : 关系组) {
        if (关系.角色或顺序 == 概念上下位关系角色) 结果.push_back(关系);
    }
    return 结果;
}

bool 可由上位包含(
    const std::vector<存在概念特征项>& 上位,
    const std::vector<存在概念特征项>& 下位,
    概念_特征类& 特征概念服务) {
    if (上位.size() > 下位.size()) return false;
    for (const auto& 上位项 : 上位) {
        const auto 下位位置 = std::find_if(下位.begin(), 下位.end(),
            [&](const auto& 项) {
                return 项.特征类型概念 == 上位项.特征类型概念;
            });
        if (下位位置 == 下位.end()) return false;
        const auto 关系 = 特征概念服务.比较单特征概念(
            上位项.具体特征概念, 下位位置->具体特征概念);
        if (关系.状态 != 特征概念规则状态::已完成 || !关系.关系) {
            return false;
        }
        if (*关系.关系 != 单特征概念关系::同一
            && *关系.关系 != 单特征概念关系::左为上位) {
            return false;
        }
    }
    return true;
}

bool 向下可达(稳定编码 起点, 稳定编码 目标) {
    std::vector<稳定编码> 待访问{起点};
    std::set<std::uint64_t> 已访问;
    while (!待访问.empty()) {
        const auto 当前 = 待访问.back();
        待访问.pop_back();
        if (当前 == 目标) return true;
        if (!已访问.insert(当前.值).second) continue;
        for (const auto& 关系 : 查询概念关系(当前, false)) {
            待访问.push_back(关系.目标节点);
        }
    }
    return false;
}

std::optional<稳定编码> 共同上位特征概念(
    稳定编码 左,
    稳定编码 右,
    概念_特征类& 特征概念服务) {
    const auto 比较 = 特征概念服务.比较单特征概念(左, 右);
    if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系) {
        return std::nullopt;
    }
    switch (*比较.关系) {
    case 单特征概念关系::同一:
    case 单特征概念关系::左为上位:
        return 左;
    case 单特征概念关系::右为上位:
        return 右;
    case 单特征概念关系::具有共同上位:
        return 比较.最近共同上位;
    case 单特征概念关系::无可比关系:
        return std::nullopt;
    }
    return std::nullopt;
}

} // namespace

概念_存在类::概念_存在类(概念_特征类& 特征概念服务) noexcept
    : 特征概念服务_(特征概念服务) {}

bool 概念_存在类::初始化() noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (结构已初始化()) return 结构仍存在();
        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        return 建立(存在概念图根节点)
            && 建立(节点类型字段)
            && 建立(专属存在概念节点类型)
            && 建立(抽象存在概念节点类型)
            && 建立(对应存在字段)
            && 建立(特征项字段)
            && 建立(特征项节点类型)
            && 建立(特征类型概念字段)
            && 建立(具体特征概念字段);
    } catch (...) {
        return false;
    }
}

存在概念建立结果 概念_存在类::建立专属存在概念(
    稳定编码 实际存在节点,
    const std::vector<稳定编码>& 具体特征概念组) noexcept {
    存在概念建立结果 结果;
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !节点仍存在(实际存在节点)) return 结果;
        for (const auto& 组织关系 : 全局基础数据集.查询源关系(
            存在概念图根节点, 基础外部关系类型::父子)) {
            if (组织关系.角色或顺序 != 组织归属关系角色
                || !读取概念种类(组织关系.目标节点)
                || 读取概念种类(组织关系.目标节点)
                    != 存在概念种类::专属) {
                continue;
            }
            const auto 已有对应存在 = 读取唯一节点字段(
                组织关系.目标节点, 对应存在字段);
            if (已有对应存在 && *已有对应存在 == 实际存在节点) {
                结果.状态 = 存在概念操作状态::结构不一致;
                return 结果;
            }
        }
        const auto 特征组 = 规范化特征组(
            具体特征概念组, 特征概念服务_);
        if (!特征组) {
            结果.状态 = 存在概念操作状态::特征概念不一致;
            return 结果;
        }

        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) {
            结果.状态 = 存在概念操作状态::资源失败;
            return 结果;
        }
        std::vector<稳定编码> 已写字段;
        const auto 类型字段 = 全局基础数据集.添加字段节点(
            节点, 节点类型字段, 专属存在概念节点类型);
        if (有效(类型字段)) 已写字段.push_back(类型字段);
        const auto 存在字段 = 有效(类型字段)
            ? 全局基础数据集.添加字段节点(
                节点, 对应存在字段, 实际存在节点)
            : 稳定编码{};
        if (有效(存在字段)) 已写字段.push_back(存在字段);
        auto 特征项组 = 有效(存在字段)
            ? 建立特征项节点组(*特征组) : std::nullopt;
        const bool 特征完成 = 特征项组
            && 挂接特征项节点组(节点, *特征项组);
        const auto 组织关系 = 特征完成
            ? 全局基础数据集.添加关系(
                存在概念图根节点, 节点,
                基础外部关系类型::父子, 组织归属关系角色)
            : 稳定编码{};
        if (有效(组织关系)) {
            结果.状态 = 存在概念操作状态::已建立;
            结果.概念节点 = 节点;
            return 结果;
        }
        if (特征项组) 删除特征项节点组(*特征项组);
        for (auto 位置 = 已写字段.rbegin(); 位置 != 已写字段.rend(); ++位置) {
            (void)全局基础数据集.删除字段(*位置);
        }
        (void)全局基础数据集.删除节点(节点);
        结果.状态 = 存在概念操作状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = 存在概念操作状态::资源失败;
        return 结果;
    }
}

存在概念操作状态 概念_存在类::更新专属存在概念(
    稳定编码 专属概念节点,
    const std::vector<稳定编码>& 具体特征概念组) noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!是专属存在概念节点(专属概念节点)) {
            return 存在概念操作状态::概念不存在;
        }
        const auto 新特征组 = 规范化特征组(
            具体特征概念组, 特征概念服务_);
        const auto 原特征组 = 读取特征组(
            专属概念节点, 特征概念服务_);
        if (!新特征组 || !原特征组) {
            return 存在概念操作状态::特征概念不一致;
        }
        if (*新特征组 == *原特征组) return 存在概念操作状态::无变化;

        auto 新特征项组 = 建立特征项节点组(*新特征组);
        if (!新特征项组
            || !挂接特征项节点组(专属概念节点, *新特征项组)) {
            if (新特征项组) 删除特征项节点组(*新特征项组);
            return 存在概念操作状态::资源失败;
        }

        const auto 全部持有字段 = 全局基础数据集.查询字段(
            专属概念节点, 特征项字段);
        std::set<std::uint64_t> 新字段身份;
        for (const auto& 项 : *新特征项组) {
            if (项.持有字段) 新字段身份.insert(项.持有字段->值);
        }
        for (const auto& 字段 : 全部持有字段) {
            if (新字段身份.contains(字段.编码.值)) continue;
            const auto* 旧项节点 = std::get_if<稳定编码>(&字段.内容);
            (void)全局基础数据集.删除字段(字段.编码);
            if (!旧项节点) continue;
            for (const auto 项字段 : {
                节点类型字段, 特征类型概念字段, 具体特征概念字段}) {
                for (const auto& 关系 : 全局基础数据集.查询字段(
                    *旧项节点, 项字段)) {
                    (void)全局基础数据集.删除字段(关系.编码);
                }
            }
            (void)全局基础数据集.删除节点(*旧项节点);
        }

        for (const auto& 上位关系 : 查询概念关系(专属概念节点, true)) {
            const auto 上位特征组 = 读取特征组(
                上位关系.源节点, 特征概念服务_);
            if (!上位特征组
                || !可由上位包含(*上位特征组, *新特征组, 特征概念服务_)) {
                (void)全局基础数据集.删除关系(上位关系.编码);
            }
        }
        return 存在概念操作状态::已更新;
    } catch (...) {
        return 存在概念操作状态::资源失败;
    }
}

bool 概念_存在类::撤销未绑定专属存在概念(
    稳定编码 专属概念节点,
    稳定编码 实际存在节点) noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!是专属存在概念节点(专属概念节点)
            || !查询概念关系(专属概念节点, true).empty()
            || !查询概念关系(专属概念节点, false).empty()) {
            return false;
        }
        const auto 对应存在 = 读取唯一节点字段(
            专属概念节点, 对应存在字段);
        if (!对应存在 || *对应存在 != 实际存在节点) return false;

        const auto 组织关系组 = 全局基础数据集.查询目标关系(
            专属概念节点, 基础外部关系类型::父子);
        std::optional<稳定编码> 组织关系;
        for (const auto& 关系 : 组织关系组) {
            if (关系.源节点 == 存在概念图根节点
                && 关系.角色或顺序 == 组织归属关系角色) {
                if (组织关系) return false;
                组织关系 = 关系.编码;
            }
        }
        if (!组织关系 || !全局基础数据集.删除关系(*组织关系)) return false;
        删除既有特征项(专属概念节点);
        for (const auto 字段 : {节点类型字段, 对应存在字段}) {
            for (const auto& 关系 : 全局基础数据集.查询字段(
                专属概念节点, 字段)) {
                if (!全局基础数据集.删除字段(关系.编码)) return false;
            }
        }
        return 全局基础数据集.删除节点(专属概念节点);
    } catch (...) {
        return false;
    }
}

存在概念抽象结果 概念_存在类::抽象共同存在概念(
    const std::vector<稳定编码>& 来源概念组) noexcept {
    存在概念抽象结果 结果;
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在()) return 结果;
        auto 来源 = 来源概念组;
        排序去重(来源);
        if (来源.size() < 2) {
            结果.状态 = 存在概念操作状态::来源不足;
            return 结果;
        }
        std::vector<std::vector<存在概念特征项>> 来源特征组;
        for (const auto 来源概念 : 来源) {
            if (!是存在概念节点(来源概念)) {
                结果.状态 = 存在概念操作状态::概念不存在;
                return 结果;
            }
            const auto 特征组 = 读取特征组(来源概念, 特征概念服务_);
            if (!特征组) {
                结果.状态 = 存在概念操作状态::结构不一致;
                return 结果;
            }
            来源特征组.push_back(*特征组);
        }

        for (const auto& 首项 : 来源特征组.front()) {
            auto 共同概念 = std::optional<稳定编码>{首项.具体特征概念};
            for (std::size_t i = 1; i < 来源特征组.size() && 共同概念; ++i) {
                const auto 位置 = std::find_if(
                    来源特征组[i].begin(), 来源特征组[i].end(),
                    [&](const auto& 项) {
                        return 项.特征类型概念 == 首项.特征类型概念;
                    });
                if (位置 == 来源特征组[i].end()) {
                    共同概念.reset();
                    break;
                }
                共同概念 = 共同上位特征概念(
                    *共同概念, 位置->具体特征概念, 特征概念服务_);
            }
            if (共同概念) {
                结果.共同特征组.push_back({首项.特征类型概念, *共同概念});
            }
        }
        排序特征项(结果.共同特征组);
        if (结果.共同特征组.empty()) {
            结果.状态 = 存在概念操作状态::无共同点;
            return 结果;
        }

        std::optional<稳定编码> 抽象节点;
        for (const auto& 组织关系 : 全局基础数据集.查询源关系(
            存在概念图根节点, 基础外部关系类型::父子)) {
            if (组织关系.角色或顺序 != 组织归属关系角色
                || !是抽象存在概念节点(组织关系.目标节点)) {
                continue;
            }
            const auto 已有特征组 = 读取特征组(
                组织关系.目标节点, 特征概念服务_);
            if (已有特征组 && *已有特征组 == 结果.共同特征组) {
                if (抽象节点) {
                    结果.状态 = 存在概念操作状态::结构不一致;
                    return 结果;
                }
                抽象节点 = 组织关系.目标节点;
            }
        }

        bool 新建 = false;
        if (!抽象节点) {
            const auto 节点 = 全局基础数据集.新建节点();
            if (!有效(节点)) {
                结果.状态 = 存在概念操作状态::资源失败;
                return 结果;
            }
            const auto 类型字段 = 全局基础数据集.添加字段节点(
                节点, 节点类型字段, 抽象存在概念节点类型);
            auto 特征项组 = 有效(类型字段)
                ? 建立特征项节点组(结果.共同特征组) : std::nullopt;
            const bool 特征完成 = 特征项组
                && 挂接特征项节点组(节点, *特征项组);
            const auto 组织关系 = 特征完成
                ? 全局基础数据集.添加关系(
                    存在概念图根节点, 节点,
                    基础外部关系类型::父子, 组织归属关系角色)
                : 稳定编码{};
            if (!有效(组织关系)) {
                if (特征项组) 删除特征项节点组(*特征项组);
                if (有效(类型字段)) (void)全局基础数据集.删除字段(类型字段);
                (void)全局基础数据集.删除节点(节点);
                结果.状态 = 存在概念操作状态::资源失败;
                return 结果;
            }
            抽象节点 = 节点;
            新建 = true;
        }

        for (const auto 来源概念 : 来源) {
            if (来源概念 == *抽象节点) continue;
            bool 已存在 = false;
            for (const auto& 关系 : 查询概念关系(*抽象节点, false)) {
                if (关系.目标节点 == 来源概念) {
                    已存在 = true;
                    break;
                }
            }
            if (已存在) continue;
            const auto 来源特征 = 读取特征组(来源概念, 特征概念服务_);
            if (!来源特征
                || !可由上位包含(
                    结果.共同特征组, *来源特征, 特征概念服务_)
                || 向下可达(来源概念, *抽象节点)) {
                结果.状态 = 向下可达(来源概念, *抽象节点)
                    ? 存在概念操作状态::会形成环
                    : 存在概念操作状态::层级不成立;
                return 结果;
            }
            const auto 关系 = 全局基础数据集.添加关系(
                *抽象节点, 来源概念,
                基础外部关系类型::父子, 概念上下位关系角色);
            if (!有效(关系)) {
                结果.状态 = 存在概念操作状态::资源失败;
                return 结果;
            }
        }
        结果.状态 = 新建
            ? 存在概念操作状态::已建立
            : 存在概念操作状态::已复用;
        结果.抽象概念节点 = *抽象节点;
        return 结果;
    } catch (...) {
        结果.状态 = 存在概念操作状态::资源失败;
        return 结果;
    }
}

std::optional<存在概念信息> 概念_存在类::获取存在概念(
    稳定编码 概念节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !节点仍存在(概念节点)) return std::nullopt;
        const auto 种类 = 读取概念种类(概念节点);
        const auto 特征组 = 读取特征组(概念节点, 特征概念服务_);
        if (!种类 || !特征组) return std::nullopt;
        if (*种类 == 存在概念种类::抽象 && 特征组->empty()) {
            return std::nullopt;
        }
        存在概念信息 结果;
        结果.节点 = 概念节点;
        结果.种类 = *种类;
        if (*种类 == 存在概念种类::专属) {
            结果.对应存在节点 = 读取唯一节点字段(
                概念节点, 对应存在字段);
            if (!结果.对应存在节点) return std::nullopt;
        } else if (!全局基础数据集.查询字段(
            概念节点, 对应存在字段).empty()) {
            return std::nullopt;
        }
        结果.特征组 = *特征组;
        结果.上位概念组 = 查询上位存在概念(概念节点);
        结果.下位概念组 = 查询下位存在概念(概念节点);
        return 结果;
    } catch (...) {
        return std::nullopt;
    }
}

bool 概念_存在类::是存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        return 结构仍存在() && 节点仍存在(节点)
            && 读取概念种类(节点).has_value();
    } catch (...) {
        return false;
    }
}

bool 概念_存在类::是专属存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        const auto 种类 = 结构仍存在() ? 读取概念种类(节点) : std::nullopt;
        return 种类 && *种类 == 存在概念种类::专属;
    } catch (...) {
        return false;
    }
}

bool 概念_存在类::是抽象存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        const auto 种类 = 结构仍存在() ? 读取概念种类(节点) : std::nullopt;
        return 种类 && *种类 == 存在概念种类::抽象;
    } catch (...) {
        return false;
    }
}

std::vector<稳定编码> 概念_存在类::查询上位存在概念(
    稳定编码 概念节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        std::vector<稳定编码> 结果;
        if (!是存在概念节点(概念节点)) return 结果;
        for (const auto& 关系 : 查询概念关系(概念节点, true)) {
            if (是抽象存在概念节点(关系.源节点)) 结果.push_back(关系.源节点);
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 概念_存在类::查询下位存在概念(
    稳定编码 概念节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        std::vector<稳定编码> 结果;
        if (!是存在概念节点(概念节点)) return 结果;
        for (const auto& 关系 : 查询概念关系(概念节点, false)) {
            if (是存在概念节点(关系.目标节点)) 结果.push_back(关系.目标节点);
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

} // namespace 海中鱼巣
