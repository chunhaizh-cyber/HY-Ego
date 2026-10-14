#include "概念_存在类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <limits>
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
稳定编码 名称关系字段{};

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
        && 有效(具体特征概念字段)
        && 有效(名称关系字段);
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
        && 节点仍存在(具体特征概念字段)
        && 节点仍存在(名称关系字段);
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
            && 建立(具体特征概念字段)
            && 建立(名称关系字段);
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
            || !查询概念关系(专属概念节点, false).empty()
            || !全局基础数据集.查询字段(
                专属概念节点, 名称关系字段).empty()) {
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

存在概念建立结果 概念_存在类::建立人工抽象存在概念(
    const std::vector<稳定编码>& 直接下位概念组) noexcept {
    存在概念建立结果 结果;
    稳定编码 节点;
    稳定编码 类型字段;
    稳定编码 组织关系;
    std::vector<稳定编码> 下位关系组;
    const auto 回滚 = [&]() noexcept {
        for (auto 位置 = 下位关系组.rbegin();
            位置 != 下位关系组.rend(); ++位置) {
            (void)全局基础数据集.删除关系(*位置);
        }
        if (有效(组织关系)) (void)全局基础数据集.删除关系(组织关系);
        if (有效(类型字段)) (void)全局基础数据集.删除字段(类型字段);
        if (有效(节点)) (void)全局基础数据集.删除节点(节点);
    };
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在()) return 结果;
        auto 下位概念组 = 直接下位概念组;
        排序去重(下位概念组);
        if (下位概念组.empty()) {
            结果.状态 = 存在概念操作状态::来源不足;
            return 结果;
        }
        for (const auto 下位概念 : 下位概念组) {
            if (!是存在概念节点(下位概念)) {
                结果.状态 = 存在概念操作状态::概念不存在;
                return 结果;
            }
        }

        节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) {
            结果.状态 = 存在概念操作状态::资源失败;
            return 结果;
        }
        类型字段 = 全局基础数据集.添加字段节点(
            节点, 节点类型字段, 抽象存在概念节点类型);
        组织关系 = 有效(类型字段)
            ? 全局基础数据集.添加关系(
                存在概念图根节点, 节点,
                基础外部关系类型::父子, 组织归属关系角色)
            : 稳定编码{};
        if (有效(组织关系)) {
            for (const auto 下位概念 : 下位概念组) {
                const auto 关系 = 全局基础数据集.添加关系(
                    节点, 下位概念,
                    基础外部关系类型::父子, 概念上下位关系角色);
                if (!有效(关系)) break;
                下位关系组.push_back(关系);
            }
        }

        if (!有效(组织关系) || 下位关系组.size() != 下位概念组.size()) {
            回滚();
            结果.状态 = 存在概念操作状态::资源失败;
            return 结果;
        }

        const auto 读回 = 获取存在概念(节点);
        if (!读回 || 读回->种类 != 存在概念种类::抽象
            || !读回->特征组.empty()
            || 读回->下位概念组 != 下位概念组
            || !读回->上位概念组.empty()) {
            回滚();
            结果.状态 = 存在概念操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 存在概念操作状态::已建立;
        结果.概念节点 = 节点;
        return 结果;
    } catch (...) {
        回滚();
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
        const auto 上位概念组 = 查询上位存在概念(概念节点);
        const auto 下位概念组 = 查询下位存在概念(概念节点);
        // 自动抽象概念由共同特征定义；人工分类概念可以没有共同特征，
        // 但必须已经持有至少一个正式下位概念，不能成为孤立空概念。
        if (*种类 == 存在概念种类::抽象
            && 特征组->empty() && 下位概念组.empty()) {
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
        for (const auto& 字段 : 全局基础数据集.查询字段(
            概念节点, 名称关系字段)) {
            const auto* 关系节点 = std::get_if<稳定编码>(&字段.内容);
            if (!关系节点 || !节点仍存在(*关系节点)) return std::nullopt;
            结果.名称关系.push_back(*关系节点);
        }
        排序去重(结果.名称关系);
        if (结果.名称关系.size() != 全局基础数据集.查询字段(
            概念节点, 名称关系字段).size()) {
            return std::nullopt;
        }
        结果.上位概念组 = 上位概念组;
        结果.下位概念组 = 下位概念组;
        return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"获取存在概念", L"概念读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"获取存在概念", L"概念读取未知异常。");
    }
}

bool 概念_存在类::绑定名称关系(
    稳定编码 概念节点, 稳定编码 名称关系节点) noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !是存在概念节点(概念节点)
            || !节点仍存在(名称关系节点)) {
            return false;
        }
        const auto 字段组 = 全局基础数据集.查询字段(
            概念节点, 名称关系字段);
        std::size_t 命中数 = 0;
        for (const auto& 字段 : 字段组) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (!目标) return false;
            if (*目标 == 名称关系节点) ++命中数;
        }
        if (命中数 == 1) return true;
        if (命中数 != 0) return false;
        const auto 新字段 = 全局基础数据集.添加字段节点(
            概念节点, 名称关系字段, 名称关系节点);
        if (!有效(新字段)) return false;
        const auto 读回 = 全局基础数据集.查询字段(
            概念节点, 名称关系字段);
        const auto 读回命中 = std::count_if(
            读回.begin(), 读回.end(), [&](const auto& 字段) {
                const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
                return 目标 && *目标 == 名称关系节点;
            });
        if (读回命中 == 1) return true;
        (void)全局基础数据集.删除字段(新字段);
        return false;
    } catch (...) {
        return false;
    }
}

bool 概念_存在类::解除名称关系(
    稳定编码 概念节点, 稳定编码 名称关系节点) noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !是存在概念节点(概念节点)) return false;
        const auto 字段组 = 全局基础数据集.查询字段(
            概念节点, 名称关系字段);
        std::optional<稳定编码> 待删字段;
        for (const auto& 字段 : 字段组) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (!目标) return false;
            if (*目标 != 名称关系节点) continue;
            if (待删字段) return false;
            待删字段 = 字段.编码;
        }
        return 待删字段 && 全局基础数据集.删除字段(*待删字段);
    } catch (...) {
        return false;
    }
}

bool 概念_存在类::是存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        return 结构仍存在() && 节点仍存在(节点)
            && 读取概念种类(节点).has_value();
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是存在概念节点",
            L"概念身份读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是存在概念节点",
            L"概念身份读取发生未知C++异常，未形成业务结果。");
    }
}

bool 概念_存在类::是专属存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        const auto 种类 = 结构仍存在() ? 读取概念种类(节点) : std::nullopt;
        return 种类 && *种类 == 存在概念种类::专属;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是专属存在概念节点",
            L"专属概念身份读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是专属存在概念节点",
            L"专属概念身份读取发生未知C++异常，未形成业务结果。");
    }
}

bool 概念_存在类::是抽象存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        const auto 种类 = 结构仍存在() ? 读取概念种类(节点) : std::nullopt;
        return 种类 && *种类 == 存在概念种类::抽象;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是抽象存在概念节点",
            L"抽象概念身份读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是抽象存在概念节点",
            L"抽象概念身份读取发生未知C++异常，未形成业务结果。");
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
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::查询上位存在概念",
            L"上位概念读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::查询上位存在概念",
            L"上位概念读取发生未知C++异常，未形成业务结果。");
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

namespace {

// 顺序与 INIT-PRONOUN-DEFINITION-PROVIDER v0.1 的十三个私有键一致。
std::array<稳定编码, 13> 指代键{};
std::array<稳定编码, 13> 指代待核验键{};
enum 指代键位 : std::size_t {
    定义类型, 规则类型, 约束类型, 范围键, 参数键, 规则项键, 约束项键,
    规则种类键, 约束角色键, FT根键, 允许区间键, FT快照键, FT单位键
};

enum class 指代内部状态 {
    完成, 输入拒绝, 依赖缺失, 依赖已变化, 结构不一致,
    资源失败, 读取未完成, 读取漂移, 未找到
};

指代内部状态 映射基础状态(基础结构操作状态 状态) {
    switch (状态) {
    case 基础结构操作状态::已完成: return 指代内部状态::完成;
    case 基础结构操作状态::节点不存在:
    case 基础结构操作状态::引用不存在: return 指代内部状态::依赖缺失;
    case 基础结构操作状态::资源失败: return 指代内部状态::资源失败;
    case 基础结构操作状态::入口拒绝: return 指代内部状态::读取未完成;
    default: return 指代内部状态::结构不一致;
    }
}

指代概念查询状态 查询状态(指代内部状态 状态) {
    switch (状态) {
    case 指代内部状态::完成: return 指代概念查询状态::已找到;
    case 指代内部状态::未找到: return 指代概念查询状态::未找到;
    case 指代内部状态::输入拒绝: return 指代概念查询状态::输入拒绝;
    case 指代内部状态::依赖缺失: return 指代概念查询状态::依赖缺失;
    case 指代内部状态::依赖已变化: return 指代概念查询状态::依赖已变化;
    case 指代内部状态::结构不一致: return 指代概念查询状态::结构不一致;
    case 指代内部状态::资源失败: return 指代概念查询状态::资源失败;
    case 指代内部状态::读取漂移: return 指代概念查询状态::读取漂移;
    default: return 指代概念查询状态::读取未完成;
    }
}

指代概念写入状态 写入状态(指代内部状态 状态) {
    switch (状态) {
    case 指代内部状态::输入拒绝: return 指代概念写入状态::输入拒绝;
    case 指代内部状态::依赖缺失: return 指代概念写入状态::依赖缺失;
    case 指代内部状态::依赖已变化: return 指代概念写入状态::依赖已变化;
    case 指代内部状态::结构不一致: return 指代概念写入状态::结构不一致;
    case 指代内部状态::资源失败: return 指代概念写入状态::资源失败;
    case 指代内部状态::读取漂移: return 指代概念写入状态::读取漂移;
    default: return 指代概念写入状态::读取未完成;
    }
}

bool 规范化区间(std::vector<特征概念I64闭区间>& 区间) {
    if (区间.empty()) return false;
    for (const auto& 项 : 区间) if (项.下界 > 项.上界) return false;
    std::sort(区间.begin(), 区间.end(), [](const auto& 左, const auto& 右) {
        return 左.下界 < 右.下界
            || (左.下界 == 右.下界 && 左.上界 < 右.上界);
    });
    std::vector<特征概念I64闭区间> 合并;
    for (const auto& 项 : 区间) {
        if (合并.empty() || (合并.back().上界 < 项.下界
            && (合并.back().上界 == std::numeric_limits<std::int64_t>::max()
                || 合并.back().上界 + 1 < 项.下界))) {
            合并.push_back(项);
        } else if (项.上界 > 合并.back().上界) {
            合并.back().上界 = 项.上界;
        }
    }
    区间 = std::move(合并);
    return true;
}

bool 规范化形状(指代概念定义& 定义) {
    if (定义.目标范围 != 指代目标范围::单个已登记强类型信息节点) return false;
    std::vector<指代参数定义> 预期;
    switch (定义.规则) {
    case 指代声明规则::作用域说话者:
    case 指代声明规则::作用域受话者:
    case 指代声明规则::作用域第三方:
        预期 = {{指代参数角色::目标, 指代参数类型::强类型信息节点},
            {指代参数角色::作用域, 指代参数类型::交互作用域}};
        break;
    case 指代声明规则::相对近指:
    case 指代声明规则::相对远指:
        预期 = {{指代参数角色::目标, 指代参数类型::强类型信息节点},
            {指代参数角色::作用域, 指代参数类型::交互作用域},
            {指代参数角色::参照中心, 指代参数类型::参照信息节点},
            {指代参数角色::参照维度规则, 指代参数类型::正式参照规则}};
        break;
    case 指代声明规则::受限身份变量:
        预期 = {{指代参数角色::目标, 指代参数类型::身份变量},
            {指代参数角色::作用域, 指代参数类型::交互作用域},
            {指代参数角色::主体类型限制, 指代参数类型::正式主体类型限制}};
        break;
    default: return false;
    }
    std::sort(定义.参数组.begin(), 定义.参数组.end(), [](const auto& 左, const auto& 右) {
        return 左.角色 < 右.角色;
    });
    if (定义.参数组 != 预期) return false;
    for (auto& 约束 : 定义.特征约束组) {
        const auto 参数 = std::find_if(定义.参数组.begin(), 定义.参数组.end(),
            [&](const auto& 项) { return 项.角色 == 约束.被约束角色; });
        if (参数 == 定义.参数组.end()
            || (参数->类型 != 指代参数类型::强类型信息节点
                && 参数->类型 != 指代参数类型::参照信息节点)
            || !有效(约束.特征类型根) || !规范化区间(约束.允许值域)
            || 约束.特征类型快照.材料类型 != 特征概念材料物理类型::I64标量
            || 约束.特征类型快照.比较规则 != 特征概念比较规则::I64数值
            || (约束.特征类型快照.聚合规则 != 特征概念聚合规则::不自动聚合
                && 约束.特征类型快照.聚合规则 != 特征概念聚合规则::I64连续值归组)
            || !约束.特征类型快照.名称关系.empty()
            || (约束.特征类型快照.单位 && !有效(*约束.特征类型快照.单位))) return false;
        auto* 根域 = std::get_if<std::vector<特征概念I64闭区间>>(
            &约束.特征类型快照.值域);
        if (!根域 || !规范化区间(*根域)) return false;
    }
    std::sort(定义.特征约束组.begin(), 定义.特征约束组.end(),
        [](const auto& 左, const auto& 右) {
            if (左.被约束角色 != 右.被约束角色)
                return 左.被约束角色 < 右.被约束角色;
            return 左.特征类型根 < 右.特征类型根;
        });
    for (std::size_t i = 1; i < 定义.特征约束组.size(); ++i) {
        const auto& 左 = 定义.特征约束组[i - 1];
        const auto& 右 = 定义.特征约束组[i];
        if (左.被约束角色 == 右.被约束角色
            && 左.特征类型根 == 右.特征类型根) return false;
    }
    return true;
}

指代内部状态 结构键状态() {
    if (std::all_of(指代键.begin(), 指代键.end(), [](auto 值) { return !有效(值); })) {
        return std::all_of(指代待核验键.begin(), 指代待核验键.end(),
            [](auto 值) { return !有效(值); })
            ? 指代内部状态::依赖缺失 : 指代内部状态::结构不一致;
    }
    std::set<稳定编码> 身份;
    for (const auto 编码 : 指代键) {
        if (!有效(编码) || 编码 == 节点类型字段 || !身份.insert(编码).second)
            return 指代内部状态::结构不一致;
        const auto 结构 = 全局基础数据集.查询完整节点结构(编码);
        if (结构.状态 != 基础结构操作状态::已完成)
            return 映射基础状态(结构.状态) == 指代内部状态::资源失败
                ? 指代内部状态::资源失败 : 指代内部状态::结构不一致;
        if (!结构.结构 || !结构.结构->字段组.empty())
            return 指代内部状态::结构不一致;
    }
    return 指代内部状态::完成;
}

std::optional<std::int64_t> 唯一I64(const 基础节点结构& 结构, 稳定编码 键) {
    std::optional<std::int64_t> 值;
    for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 键) {
        if (值) return std::nullopt;
        const auto* 原始 = std::get_if<基础值>(&字段.内容);
        const auto* 数 = 原始 ? std::get_if<std::int64_t>(&原始->材料) : nullptr;
        if (!数) return std::nullopt;
        值 = *数;
    }
    return 值;
}

std::optional<稳定编码> 唯一引用(const 基础节点结构& 结构, 稳定编码 键) {
    std::optional<稳定编码> 值;
    for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 键) {
        if (值) return std::nullopt;
        const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
        if (!引用 || !有效(*引用)) return std::nullopt;
        值 = *引用;
    }
    return 值;
}

std::optional<std::vector<std::int64_t>> 唯一数组(
    const 基础节点结构& 结构, 稳定编码 键) {
    std::optional<std::vector<std::int64_t>> 值;
    for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 键) {
        if (值) return std::nullopt;
        const auto* 原始 = std::get_if<基础值>(&字段.内容);
        const auto* 数 = 原始 ? std::get_if<std::vector<std::int64_t>>(&原始->材料) : nullptr;
        if (!数) return std::nullopt;
        值 = *数;
    }
    return 值;
}

bool 字段闭合(const 基础节点结构& 结构, std::initializer_list<稳定编码> 允许) {
    for (const auto& 字段 : 结构.字段组)
        if (std::find(允许.begin(), 允许.end(), 字段.字段节点) == 允许.end()) return false;
    return true;
}

std::optional<std::vector<特征概念I64闭区间>> 解码区间(
    const std::vector<std::int64_t>& 数组, std::size_t 前缀) {
    if (数组.size() <= 前缀 || 数组[前缀] <= 0) return std::nullopt;
    const auto 数量 = static_cast<std::uint64_t>(数组[前缀]);
    if (数量 > (std::numeric_limits<std::size_t>::max() - 前缀 - 1) / 2
        || 数组.size() != 前缀 + 1 + 2 * static_cast<std::size_t>(数量)) return std::nullopt;
    std::vector<特征概念I64闭区间> 结果;
    for (std::size_t i = 前缀 + 1; i < 数组.size(); i += 2)
        结果.push_back({数组[i], 数组[i + 1]});
    auto 原样 = 结果;
    if (!规范化区间(结果) || 结果 != 原样) return std::nullopt;
    return 结果;
}

std::vector<std::int64_t> 编码区间(const std::vector<特征概念I64闭区间>& 区间) {
    std::vector<std::int64_t> 数组{static_cast<std::int64_t>(区间.size())};
    for (const auto& 项 : 区间) { 数组.push_back(项.下界); 数组.push_back(项.上界); }
    return 数组;
}

指代内部状态 读取结构(稳定编码 节点, 基础节点结构& 输出) {
    const auto 读取 = 全局基础数据集.查询完整节点结构(节点);
    if (读取.状态 != 基础结构操作状态::已完成)
        return 映射基础状态(读取.状态);
    if (!读取.结构) return 指代内部状态::读取未完成;
    输出 = *读取.结构;
    return 指代内部状态::完成;
}

bool 唯一持有(稳定编码 子项, 稳定编码 所属, 稳定编码 字段键) {
    const auto 反向 = 全局基础数据集.查询目标字段(子项, 字段键);
    return 反向.size() == 1 && 反向.front().所属节点 == 所属
        && std::get_if<稳定编码>(&反向.front().内容)
        && *std::get_if<稳定编码>(&反向.front().内容) == 子项;
}

struct 指代候选快照 final {
    std::vector<基础字段关系> 召回;
    std::vector<基础节点结构> 结构;
    std::vector<指代概念信息> 信息;
};

bool 同一候选快照(const 指代候选快照& 左, const 指代候选快照& 右) {
    if (左.召回 != 右.召回 || 左.信息 != 右.信息
        || 左.结构.size() != 右.结构.size()) return false;
    for (std::size_t i = 0; i < 左.结构.size(); ++i)
        if (左.结构[i].节点 != 右.结构[i].节点
            || 左.结构[i].字段组 != 右.结构[i].字段组) return false;
    return true;
}

指代内部状态 读取指代定义(稳定编码 节点, 指代候选快照& 快照) {
    基础节点结构 主;
    const auto 首读 = 全局基础数据集.查询节点组完整结构({节点});
    if (首读.状态 != 基础结构操作状态::已完成)
        return 映射基础状态(首读.状态);
    if (首读.节点组.size() != 1 || !首读.节点组.front().快照)
        return 指代内部状态::依赖缺失;
    主 = 首读.节点组.front().快照->结构;
    std::vector<稳定编码> 名称;
    for (const auto& 字段 : 主.字段组) if (字段.字段节点 == 名称关系字段) {
        const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
        if (!引用 || !有效(*引用)
            || std::find(名称.begin(), 名称.end(), *引用) != 名称.end())
            return 指代内部状态::结构不一致;
        名称.push_back(*引用);
    }
    std::vector<稳定编码> 联合节点{节点};
    联合节点.insert(联合节点.end(), 名称.begin(), 名称.end());
    const auto 联合 = 全局基础数据集.查询节点组完整结构(联合节点);
    if (联合.状态 != 基础结构操作状态::已完成)
        return 映射基础状态(联合.状态);
    if (联合.节点组.size() != 联合节点.size())
        return 指代内部状态::结构不一致;
    bool 主已核对 = false;
    for (const auto& 项 : 联合.节点组) {
        if (!项.快照) return 指代内部状态::结构不一致;
        if (项.查询节点 == 节点) {
            const auto& 前 = *首读.节点组.front().快照;
            const auto& 后 = *项.快照;
            if (前.结构.节点 != 后.结构.节点
                || 前.结构.字段组 != 后.结构.字段组
                || 前.入向外部关系组 != 后.入向外部关系组
                || 前.出向外部关系组 != 后.出向外部关系组
                || 前.入向字段组 != 后.入向字段组)
                return 指代内部状态::读取漂移;
            主已核对 = true;
        }
    }
    if (!主已核对) return 指代内部状态::结构不一致;
    std::sort(名称.begin(), 名称.end());
    auto 状态 = 指代内部状态::完成;
    const auto 类型 = 唯一引用(主, 节点类型字段);
    if (!类型 || *类型 != 指代键[定义类型]
        || !字段闭合(主, {节点类型字段, 指代键[范围键], 指代键[参数键],
            指代键[规则项键], 指代键[约束项键], 名称关系字段}))
        return 指代内部状态::结构不一致;
    const auto 范围 = 唯一I64(主, 指代键[范围键]);
    const auto 参数数组 = 唯一数组(主, 指代键[参数键]);
    const auto 规则节点 = 唯一引用(主, 指代键[规则项键]);
    if (!范围 || *范围 != 1 || !参数数组 || !规则节点
        || 参数数组->size() % 2 != 0)
        return 指代内部状态::结构不一致;
    指代概念定义 定义;
    定义.目标范围 = static_cast<指代目标范围>(*范围);
    for (std::size_t i = 0; i < 参数数组->size(); i += 2) {
        if ((*参数数组)[i] < 1 || (*参数数组)[i] > 5
            || (*参数数组)[i + 1] < 1 || (*参数数组)[i + 1] > 6)
            return 指代内部状态::结构不一致;
        定义.参数组.push_back({static_cast<指代参数角色>((*参数数组)[i]),
            static_cast<指代参数类型>((*参数数组)[i + 1])});
    }
    快照.结构.push_back(主);

    基础节点结构 规则;
    状态 = 读取结构(*规则节点, 规则);
    if (状态 != 指代内部状态::完成) return 状态;
    const auto 规则类型标记 = 唯一引用(规则, 节点类型字段);
    const auto 规则种类值 = 唯一I64(规则, 指代键[规则种类键]);
    if (!规则类型标记 || *规则类型标记 != 指代键[规则类型] || !规则种类值
        || *规则种类值 < 1 || *规则种类值 > 6
        || !字段闭合(规则, {节点类型字段, 指代键[规则种类键]})
        || !唯一持有(*规则节点, 节点, 指代键[规则项键]))
        return 指代内部状态::结构不一致;
    定义.规则 = static_cast<指代声明规则>(*规则种类值);
    快照.结构.push_back(规则);

    std::set<稳定编码> 项身份;
    for (const auto& 字段 : 主.字段组) {
        if (字段.字段节点 != 指代键[约束项键]) continue;
        const auto* 项节点 = std::get_if<稳定编码>(&字段.内容);
        if (!项节点 || !有效(*项节点) || !项身份.insert(*项节点).second
            || !唯一持有(*项节点, 节点, 指代键[约束项键]))
            return 指代内部状态::结构不一致;
        基础节点结构 项;
        状态 = 读取结构(*项节点, 项);
        if (状态 != 指代内部状态::完成) return 状态;
        const auto 项类型标记 = 唯一引用(项, 节点类型字段);
        const auto 角色 = 唯一I64(项, 指代键[约束角色键]);
        const auto 根 = 唯一引用(项, 指代键[FT根键]);
        const auto 允许数组 = 唯一数组(项, 指代键[允许区间键]);
        const auto 快照数组 = 唯一数组(项, 指代键[FT快照键]);
        if (!项类型标记 || *项类型标记 != 指代键[约束类型] || !角色
            || *角色 < 1 || *角色 > 5
            || !根 || !允许数组 || !快照数组 || 快照数组->size() < 7
            || (*快照数组)[0] != 1 || (*快照数组)[1] != 1
            || (*快照数组)[2] != 1
            || (*快照数组)[3] < 1 || (*快照数组)[3] > 2
            || !字段闭合(项,
                {节点类型字段, 指代键[约束角色键], 指代键[FT根键],
                指代键[允许区间键], 指代键[FT快照键], 指代键[FT单位键]}))
            return 指代内部状态::结构不一致;
        const auto 允许域 = 解码区间(*允许数组, 0);
        const auto 根域 = 解码区间(*快照数组, 4);
        if (!允许域 || !根域) return 指代内部状态::结构不一致;
        指代I64特征约束 约束;
        约束.被约束角色 = static_cast<指代参数角色>(*角色);
        约束.特征类型根 = *根;
        约束.允许值域 = *允许域;
        约束.特征类型快照.材料类型 = static_cast<特征概念材料物理类型>((*快照数组)[1]);
        约束.特征类型快照.比较规则 = static_cast<特征概念比较规则>((*快照数组)[2]);
        约束.特征类型快照.聚合规则 = static_cast<特征概念聚合规则>((*快照数组)[3]);
        约束.特征类型快照.值域 = *根域;
        std::size_t 单位数 = 0;
        for (const auto& 子字段 : 项.字段组) if (子字段.字段节点 == 指代键[FT单位键]) {
            ++单位数;
            const auto* 单位 = std::get_if<稳定编码>(&子字段.内容);
            if (!单位 || !有效(*单位)) return 指代内部状态::结构不一致;
            约束.特征类型快照.单位 = *单位;
        }
        if (单位数 > 1) return 指代内部状态::结构不一致;
        定义.特征约束组.push_back(std::move(约束));
        快照.结构.push_back(std::move(项));
    }
    auto 规范定义 = 定义;
    if (!规范化形状(规范定义) || 规范定义 != 定义)
        return 指代内部状态::结构不一致;
    快照.信息.push_back({节点, std::move(定义), std::move(名称)});
    return 指代内部状态::完成;
}

指代内部状态 读取候选全集(指代候选快照& 快照) {
    快照.召回 = 全局基础数据集.查询目标字段(指代键[定义类型], 节点类型字段);
    std::set<稳定编码> 节点身份;
    for (const auto& 字段 : 快照.召回) {
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (!目标 || *目标 != 指代键[定义类型]
            || !节点身份.insert(字段.所属节点).second)
            return 指代内部状态::结构不一致;
        const auto 状态 = 读取指代定义(字段.所属节点, 快照);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    for (std::size_t i = 0; i < 快照.信息.size(); ++i)
        for (std::size_t j = i + 1; j < 快照.信息.size(); ++j)
            if (快照.信息[i].定义 == 快照.信息[j].定义)
                return 指代内部状态::结构不一致;
    return 指代内部状态::完成;
}

std::vector<稳定编码> 收集FT根(const 指代候选快照& 快照,
    const 指代概念定义* 请求 = nullptr) {
    std::vector<稳定编码> 结果;
    if (请求) for (const auto& 项 : 请求->特征约束组) 结果.push_back(项.特征类型根);
    for (const auto& 信息 : 快照.信息)
        for (const auto& 项 : 信息.定义.特征约束组) 结果.push_back(项.特征类型根);
    排序去重(结果);
    return 结果;
}

指代内部状态 映射FT状态(特征概念规则状态 状态) {
    switch (状态) {
    case 特征概念规则状态::已完成: return 指代内部状态::完成;
    case 特征概念规则状态::入口拒绝:
    case 特征概念规则状态::读取接口未装配: return 指代内部状态::读取未完成;
    case 特征概念规则状态::值不存在: return 指代内部状态::依赖缺失;
    case 特征概念规则状态::资源失败: return 指代内部状态::资源失败;
    default: return 指代内部状态::结构不一致;
    }
}

struct 指代FT探针 final {
    稳定编码 根;
    特征概念定义 正式;
    基础节点结构 基础;
    std::vector<基础外部关系> 入向;
};

指代内部状态 正式读取FT(概念_特征类& 服务,
    稳定编码 根, 特征概念定义& 定义) {
    const auto 自比较 = 服务.比较单特征概念(根, 根);
    const auto 状态 = 映射FT状态(自比较.状态);
    if (状态 != 指代内部状态::完成) return 状态;
    if (!自比较.关系 || *自比较.关系 != 单特征概念关系::同一
        || !自比较.特征类型根 || !有效(*自比较.特征类型根))
        return 指代内部状态::结构不一致;
    if (*自比较.特征类型根 != 根) return 指代内部状态::输入拒绝;
    const auto 读取 = 服务.获取特征概念(根);
    if (!读取) return 指代内部状态::读取未完成;
    定义 = {读取->材料类型, 读取->值域, 读取->单位,
        读取->比较规则, 读取->聚合规则, {}};
    if (定义.材料类型 != 特征概念材料物理类型::I64标量
        || 定义.比较规则 != 特征概念比较规则::I64数值
        || (定义.单位 && !有效(*定义.单位))) return 指代内部状态::输入拒绝;
    auto* 域 = std::get_if<std::vector<特征概念I64闭区间>>(&定义.值域);
    if (!域 || !规范化区间(*域)) return 指代内部状态::输入拒绝;
    return 指代内部状态::完成;
}

指代内部状态 验证约束组(概念_特征类& 服务,
    const 指代概念定义& 定义, const std::vector<指代FT探针>& 探针,
    bool 请求) {
    for (const auto& 项 : 定义.特征约束组) {
        const auto 位置 = std::find_if(探针.begin(), 探针.end(),
            [&](const auto& 值) { return 值.根 == 项.特征类型根; });
        if (位置 == 探针.end()) return 指代内部状态::结构不一致;
        if (项.特征类型快照 != 位置->正式)
            return 指代内部状态::依赖已变化;
        const auto 比较 = 服务.比较值域(特征概念材料物理类型::I64标量,
            特征概念值域{项.允许值域}, 位置->正式.值域);
        const auto 状态 = 映射FT状态(比较.状态);
        if (状态 != 指代内部状态::完成) return 状态;
        if (!比较.关系) return 指代内部状态::结构不一致;
        if (*比较.关系 != 特征概念值域关系::相等
            && *比较.关系 != 特征概念值域关系::右包含左)
            return 请求 ? 指代内部状态::输入拒绝 : 指代内部状态::结构不一致;
    }
    return 指代内部状态::完成;
}

struct 指代准备 final {
    指代候选快照 候选;
    std::vector<指代FT探针> FT;
};

指代内部状态 准备指代操作(概念_特征类& 服务,
    const 指代概念定义* 请求, 指代准备& 准备) {
    {
        std::lock_guard 锁(存在概念图互斥);
        const auto 结构 = 结构键状态();
        if (结构 != 指代内部状态::完成) return 结构;
        const auto 状态 = 读取候选全集(准备.候选);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    for (const auto 根 : 收集FT根(准备.候选, 请求)) {
        指代FT探针 探针;
        探针.根 = 根;
        auto 状态 = 正式读取FT(服务, 根, 探针.正式);
        if (状态 != 指代内部状态::完成) return 状态;
        状态 = 读取结构(根, 探针.基础);
        if (状态 != 指代内部状态::完成) return 状态;
        探针.入向 = 全局基础数据集.查询目标关系(根, 基础外部关系类型::父子);
        特征概念定义 再读;
        状态 = 正式读取FT(服务, 根, 再读);
        if (状态 != 指代内部状态::完成) return 状态;
        if (再读 != 探针.正式) return 指代内部状态::依赖已变化;
        准备.FT.push_back(std::move(探针));
    }
    if (请求) {
        const auto 状态 = 验证约束组(服务, *请求, 准备.FT, true);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    for (const auto& 信息 : 准备.候选.信息) {
        const auto 状态 = 验证约束组(服务, 信息.定义, 准备.FT, false);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    return 指代内部状态::完成;
}

指代内部状态 复核基础准备(const 指代准备& 准备) {
    const auto 结构状态 = 结构键状态();
    if (结构状态 != 指代内部状态::完成) return 结构状态;
    指代候选快照 现在;
    auto 状态 = 读取候选全集(现在);
    if (状态 == 指代内部状态::结构不一致
        || 状态 == 指代内部状态::依赖缺失)
        return 指代内部状态::读取漂移;
    if (状态 != 指代内部状态::完成) return 状态;
    if (!同一候选快照(现在, 准备.候选)) return 指代内部状态::读取漂移;
    for (const auto& 项 : 准备.FT) {
        基础节点结构 当前;
        状态 = 读取结构(项.根, 当前);
        if (状态 == 指代内部状态::依赖缺失)
            return 指代内部状态::读取漂移;
        if (状态 != 指代内部状态::完成) return 状态;
        if (当前.节点 != 项.基础.节点 || 当前.字段组 != 项.基础.字段组
            || 全局基础数据集.查询目标关系(项.根,
                基础外部关系类型::父子) != 项.入向)
            return 指代内部状态::读取漂移;
    }
    return 指代内部状态::完成;
}

指代内部状态 发布后FT复核(概念_特征类& 服务,
    const 指代概念定义& 定义) {
    for (const auto& 项 : 定义.特征约束组) {
        特征概念定义 现在;
        const auto 状态 = 正式读取FT(服务, 项.特征类型根, 现在);
        if (状态 == 指代内部状态::输入拒绝)
            return 指代内部状态::依赖已变化;
        if (状态 != 指代内部状态::完成) return 状态;
        if (现在 != 项.特征类型快照) return 指代内部状态::依赖已变化;
    }
    return 指代内部状态::完成;
}

std::vector<基础组内新节点> 构造指代节点组(const 指代概念定义& 定义) {
    std::vector<基础组内新节点> 组(2 + 定义.特征约束组.size());
    const auto 加 = [&](std::size_t 索引, 稳定编码 键, 基础组内字段内容 内容) {
        组[索引].字段组.push_back({键, std::move(内容)});
    };
    const auto 加数 = [&](std::size_t 索引, 稳定编码 键, auto 值) {
        加(索引, 键, 基础值{static_cast<std::int64_t>(值)});
    };
    加(0, 节点类型字段, 指代键[定义类型]);
    加数(0, 指代键[范围键], 定义.目标范围);
    std::vector<std::int64_t> 参数;
    for (const auto& 项 : 定义.参数组) {
        参数.push_back(static_cast<std::int64_t>(项.角色));
        参数.push_back(static_cast<std::int64_t>(项.类型));
    }
    加(0, 指代键[参数键], 基础值{std::move(参数)});
    加(0, 指代键[规则项键], 基础组内节点引用{1});
    加(1, 节点类型字段, 指代键[规则类型]);
    加数(1, 指代键[规则种类键], 定义.规则);
    for (std::size_t i = 0; i < 定义.特征约束组.size(); ++i) {
        const auto& 项 = 定义.特征约束组[i];
        const auto 索引 = i + 2;
        加(0, 指代键[约束项键], 基础组内节点引用{索引});
        加(索引, 节点类型字段, 指代键[约束类型]);
        加数(索引, 指代键[约束角色键], 项.被约束角色);
        加(索引, 指代键[FT根键], 项.特征类型根);
        加(索引, 指代键[允许区间键], 基础值{编码区间(项.允许值域)});
        const auto& 根域 = std::get<std::vector<特征概念I64闭区间>>(
            项.特征类型快照.值域);
        auto 快照数组 = 编码区间(根域);
        快照数组.insert(快照数组.begin(), {
            1, static_cast<std::int64_t>(项.特征类型快照.材料类型),
            static_cast<std::int64_t>(项.特征类型快照.比较规则),
            static_cast<std::int64_t>(项.特征类型快照.聚合规则)});
        加(索引, 指代键[FT快照键], 基础值{std::move(快照数组)});
        if (项.特征类型快照.单位)
            加(索引, 指代键[FT单位键], *项.特征类型快照.单位);
    }
    return 组;
}

} // namespace

指代定义结构状态 概念_存在类::初始化指代定义结构() {
    std::lock_guard 锁(存在概念图互斥);
    if (!结构已初始化()) return 指代定义结构状态::入口拒绝;
    for (const auto 编码 : {存在概念图根节点, 节点类型字段,
        专属存在概念节点类型, 抽象存在概念节点类型, 对应存在字段,
        特征项字段, 特征项节点类型, 特征类型概念字段,
        具体特征概念字段, 名称关系字段}) {
        const auto 读取 = 全局基础数据集.查询完整节点结构(编码);
        if (读取.状态 == 基础结构操作状态::资源失败)
            return 指代定义结构状态::资源失败;
        if (读取.状态 != 基础结构操作状态::已完成 || !读取.结构)
            return 指代定义结构状态::内部不一致;
    }
    const auto 既有状态 = 结构键状态();
    if (既有状态 == 指代内部状态::完成)
        return 指代定义结构状态::已就绪;
    if (std::any_of(指代键.begin(), 指代键.end(), [](auto 值) { return 有效(值); }))
        return 既有状态 == 指代内部状态::资源失败
            ? 指代定义结构状态::资源失败 : 指代定义结构状态::内部不一致;
    auto& 待核验 = 指代待核验键;
    if (std::all_of(待核验.begin(), 待核验.end(), [](auto 值) { return !有效(值); })) {
        std::vector<基础组内新节点> 请求(13);
        const auto 建立 = 全局基础数据集.原子建立节点组(请求);
        if (建立.状态 == 基础结构操作状态::资源失败)
            return 指代定义结构状态::资源失败;
        if (建立.状态 != 基础结构操作状态::已完成)
            return 指代定义结构状态::内部不一致;
        for (std::size_t i = 0; i < std::min(待核验.size(), 建立.节点组.size()); ++i)
            待核验[i] = 建立.节点组[i];
        if (建立.节点组.size() != 待核验.size())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                L"初始化指代定义结构", L"L1 已发布技术节点，但返回身份数量违反原子组合同。");
    }
    std::set<稳定编码> 身份;
    for (const auto 编码 : 待核验) {
        if (!有效(编码) || 编码 == 节点类型字段 || !身份.insert(编码).second)
            return 指代定义结构状态::内部不一致;
        const auto 读取 = 全局基础数据集.查询完整节点结构(编码);
        if (读取.状态 == 基础结构操作状态::资源失败)
            return 指代定义结构状态::资源失败;
        if (读取.状态 != 基础结构操作状态::已完成 || !读取.结构
            || !读取.结构->字段组.empty()) return 指代定义结构状态::内部不一致;
    }
    指代键 = 待核验;
    return 指代定义结构状态::已就绪;
}

指代概念查询结果 概念_存在类::查询指代概念(
    const 指代概念定义& 请求) const {
    指代概念查询结果 结果;
    auto 规范请求 = 请求;
    if (!规范化形状(规范请求)) return 结果;
    指代准备 准备;
    auto 状态 = 准备指代操作(特征概念服务_, &规范请求, 准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    std::optional<指代概念信息> 命中;
    {
        std::lock_guard 锁(存在概念图互斥);
        状态 = 复核基础准备(准备);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(状态);
            return 结果;
        }
        for (const auto& 候选 : 准备.候选.信息) if (候选.定义 == 规范请求) {
            if (命中) {
                结果.状态 = 指代概念查询状态::结构不一致;
                return 结果;
            }
            命中 = 候选;
        }
    }
    if (!命中) { 结果.状态 = 指代概念查询状态::未找到; return 结果; }
    状态 = 发布后FT复核(特征概念服务_, 命中->定义);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    结果.状态 = 指代概念查询状态::已找到;
    结果.信息 = std::move(命中);
    return 结果;
}

指代概念查询结果 概念_存在类::获取指代概念(稳定编码 概念节点) const {
    指代概念查询结果 结果;
    if (!有效(概念节点)) return 结果;
    {
        std::lock_guard 锁(存在概念图互斥);
        const auto 结构状态 = 结构键状态();
        if (结构状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(结构状态);
            return 结果;
        }
        基础节点结构 目标;
        const auto 读取状态 = 读取结构(概念节点, 目标);
        if (读取状态 == 指代内部状态::依赖缺失) {
            结果.状态 = 指代概念查询状态::未找到;
            return 结果;
        }
        if (读取状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(读取状态);
            return 结果;
        }
        std::size_t 类型数量 = 0;
        std::optional<稳定编码> 类型;
        for (const auto& 字段 : 目标.字段组) if (字段.字段节点 == 节点类型字段) {
            ++类型数量;
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) 类型 = *引用;
        }
        if (类型数量 > 1 || (类型数量 == 1 && (!类型 || !有效(*类型)))) {
            结果.状态 = 指代概念查询状态::结构不一致;
            return 结果;
        }
        if (!类型 || *类型 != 指代键[定义类型]) {
            结果.状态 = 指代概念查询状态::未找到;
            return 结果;
        }
    }
    指代准备 准备;
    auto 状态 = 准备指代操作(特征概念服务_, nullptr, 准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    if (std::none_of(准备.候选.信息.begin(), 准备.候选.信息.end(),
        [&](const auto& 项) { return 项.节点 == 概念节点; })) {
        结果.状态 = 指代概念查询状态::结构不一致;
        return 结果;
    }
    std::optional<指代概念信息> 命中;
    {
        std::lock_guard 锁(存在概念图互斥);
        状态 = 复核基础准备(准备);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(状态);
            return 结果;
        }
        for (const auto& 候选 : 准备.候选.信息) if (候选.节点 == 概念节点) {
            命中 = 候选;
            break;
        }
    }
    if (!命中) { 结果.状态 = 指代概念查询状态::未找到; return 结果; }
    状态 = 发布后FT复核(特征概念服务_, 命中->定义);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    结果.状态 = 指代概念查询状态::已找到;
    结果.信息 = std::move(命中);
    return 结果;
}

指代概念写入结果 概念_存在类::建立或取得指代概念(
    const 指代概念定义& 请求) {
    指代概念写入结果 结果;
    try {
        auto 规范请求 = 请求;
        if (!规范化形状(规范请求)) return 结果;
        指代准备 准备;
        auto 状态 = 准备指代操作(特征概念服务_, &规范请求, 准备);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 写入状态(状态);
            return 结果;
        }
        bool 新建 = false;
        std::optional<指代概念信息> 读回信息;
        {
            std::lock_guard 锁(存在概念图互斥);
            状态 = 复核基础准备(准备);
            if (状态 != 指代内部状态::完成) {
                结果.状态 = 写入状态(状态);
                return 结果;
            }
            for (const auto& 候选 : 准备.候选.信息) if (候选.定义 == 规范请求) {
                if (结果.已发生身份) {
                    结果.状态 = 指代概念写入状态::结构不一致;
                    return 结果;
                }
                结果.已发生身份 = 候选.节点;
                结果.阶段 = 指代概念发布阶段::已发布待核验;
            }
            if (!结果.已发生身份) {
                const auto 建立 = 全局基础数据集.原子建立节点组(
                    构造指代节点组(规范请求));
                if (建立.状态 != 基础结构操作状态::已完成) {
                    结果.状态 = 写入状态(映射基础状态(建立.状态));
                    return 结果;
                }
                if (建立.节点组.empty() || !有效(建立.节点组.front()))
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                        L"建立或取得指代概念", L"L1 已完成发布但未返回真实定义身份。");
                // 先保存真实身份，再核验返回的剩余组形状。
                结果.已发生身份 = 建立.节点组.front();
                结果.阶段 = 指代概念发布阶段::已发布待核验;
                if (建立.节点组.size() != 2 + 规范请求.特征约束组.size()) {
                    结果.状态 = 指代概念写入状态::结构不一致;
                    return 结果;
                }
                // L1 已发布：任何随后读回、复制或诊断异常都保留真实身份。
                新建 = true;
            }
            指代候选快照 读回;
            状态 = 读取指代定义(*结果.已发生身份, 读回);
            if (状态 != 指代内部状态::完成) {
                结果.状态 = 写入状态(状态);
                return 结果;
            }
            if (读回.信息.size() != 1 || 读回.信息.front().定义 != 规范请求) {
                结果.状态 = 指代概念写入状态::结构不一致;
                return 结果;
            }
            读回信息 = std::move(读回.信息.front());
        }
        状态 = 发布后FT复核(特征概念服务_, 规范请求);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 写入状态(状态);
            return 结果;
        }
        // 身份与完整信息同时核验后才宣称本次建立或复用成功。
        结果.信息 = std::move(读回信息);
        结果.阶段 = 指代概念发布阶段::已完整读回;
        结果.状态 = 新建 ? 指代概念写入状态::已建立
            : 指代概念写入状态::已复用;
        return 结果;
    } catch (...) {
        if (!结果.已发生身份) throw;
        结果.信息.reset();
        结果.阶段 = 指代概念发布阶段::已发布待核验;
        结果.状态 = 指代概念写入状态::资源失败;
        结果.发布后异常 = std::current_exception();
        return 结果;
    }
}

namespace {
bool 同一联合快照(const 基础联合节点快照& 左,
    const 基础联合节点快照& 右) {
    return 左.结构.节点 == 右.结构.节点
        && 左.结构.字段组 == 右.结构.字段组
        && 左.入向外部关系组 == 右.入向外部关系组
        && 左.出向外部关系组 == 右.出向外部关系组
        && 左.入向字段组 == 右.入向字段组;
}

基础互引节点操作状态 合并名称预期(
    const std::vector<基础联合节点快照>& 概念侧,
    const std::vector<基础联合节点快照>& 名称侧,
    std::vector<基础联合节点快照>& 输出) {
    using 状态 = 基础互引节点操作状态;
    std::map<稳定编码, 基础联合节点快照> 合并;
    for (const auto* 来源 : {&概念侧, &名称侧}) {
        for (const auto& 项 : *来源) {
            const auto 编码 = 项.结构.节点.编码;
            if (!有效(编码)) return 状态::入口拒绝;
            const auto [位置, 新增] = 合并.emplace(编码, 项);
            if (!新增 && !同一联合快照(位置->second, 项))
                return 状态::预期不符;
        }
    }
    for (auto& [_, 项] : 合并) 输出.push_back(std::move(项));
    return 状态::已完成;
}
} // namespace

指代名称联合建立请求结果 指代名称反向参与规格::形成建立请求(
    const 基础节点建立请求& 名称节点请求,
    const std::vector<基础联合节点快照>& 名称侧预期) const {
    using 状态 = 基础互引节点操作状态;
    指代名称联合建立请求结果 结果;
    if (!有效(概念节点_) || !有效(名称字段节点_)
        || !名称节点请求.上级节点 || !有效(*名称节点请求.上级节点))
        return 结果;
    基础互引节点建立请求 请求;
    请求.新节点 = 名称节点请求;
    请求.反向引用所属节点 = 概念节点_;
    请求.反向引用字段节点 = 名称字段节点_;
    结果.状态 = 合并名称预期(预期节点组_, 名称侧预期, 请求.预期节点组);
    if (结果.状态 == 状态::已完成) 结果.请求 = std::move(请求);
    return 结果;
}

指代名称联合删除请求结果 指代名称反向参与规格::形成删除请求(
    稳定编码 名称节点, 稳定编码 名称上级节点,
    const std::vector<基础联合节点快照>& 名称侧预期) const {
    using 状态 = 基础互引节点操作状态;
    指代名称联合删除请求结果 结果;
    if (!有效(名称节点) || !有效(名称上级节点)) return 结果;
    std::optional<稳定编码> 反向身份;
    for (const auto& 字段 : 反向名称字段组_) {
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (目标 && *目标 == 名称节点) {
            if (反向身份) return 结果;
            反向身份 = 字段.编码;
        }
    }
    if (!反向身份) return 结果;
    基础互引节点删除请求 请求;
    请求.节点 = 名称节点;
    请求.上级节点 = 名称上级节点;
    请求.反向字段关系 = *反向身份;
    结果.状态 = 合并名称预期(预期节点组_, 名称侧预期, 请求.预期节点组);
    if (结果.状态 == 状态::已完成) 结果.请求 = std::move(请求);
    return 结果;
}

指代名称反向参与结果 概念_存在类::准备指代名称反向参与(
    稳定编码 概念节点, const 指代概念定义& 预期定义) const {
    指代名称反向参与结果 结果;
    auto 规范定义 = 预期定义;
    if (!有效(概念节点) || !规范化形状(规范定义)) return 结果;
    const auto 正式 = 获取指代概念(概念节点);
    if (正式.状态 != 指代概念查询状态::已找到) {
        结果.状态 = 正式.状态;
        return 结果;
    }
    if (!正式.信息) {
        结果.状态 = 指代概念查询状态::结构不一致;
        return 结果;
    }
    if (正式.信息->定义 != 规范定义) {
        结果.状态 = 指代概念查询状态::依赖已变化;
        return 结果;
    }
    指代准备 准备;
    auto 状态 = 准备指代操作(特征概念服务_, &规范定义, 准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    std::lock_guard 锁(存在概念图互斥);
    状态 = 复核基础准备(准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    const auto 命中 = std::find_if(准备.候选.信息.begin(), 准备.候选.信息.end(),
        [&](const auto& 项) { return 项.节点 == 概念节点; });
    if (命中 == 准备.候选.信息.end() || *命中 != *正式.信息) {
        结果.状态 = 指代概念查询状态::读取漂移;
        return 结果;
    }
    std::set<稳定编码> 身份{概念节点, 名称关系字段};
    for (const auto& 项 : 准备.候选.结构) 身份.insert(项.节点.编码);
    for (const auto& 项 : 准备.FT) 身份.insert(项.根);
    for (const auto 名称 : 命中->名称关系) 身份.insert(名称);
    const std::vector<稳定编码> 查询身份(身份.begin(), 身份.end());
    const auto 联合 = 全局基础数据集.查询节点组完整结构(查询身份);
    if (联合.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 查询状态(映射基础状态(联合.状态));
        return 结果;
    }
    指代名称反向参与规格 规格;
    规格.概念节点_ = 概念节点;
    规格.名称字段节点_ = 名称关系字段;
    if (联合.节点组.size() != 身份.size()) {
        结果.状态 = 指代概念查询状态::结构不一致;
        return 结果;
    }
    for (const auto& 前 : 准备.候选.结构) {
        const auto 位置 = std::find_if(联合.节点组.begin(), 联合.节点组.end(),
            [&](const auto& 项) { return 项.查询节点 == 前.节点.编码; });
        if (位置 == 联合.节点组.end() || !位置->快照
            || 位置->快照->结构.节点 != 前.节点
            || 位置->快照->结构.字段组 != 前.字段组) {
            结果.状态 = 指代概念查询状态::读取漂移;
            return 结果;
        }
    }
    for (const auto& 前 : 准备.FT) {
        const auto 位置 = std::find_if(联合.节点组.begin(), 联合.节点组.end(),
            [&](const auto& 项) { return 项.查询节点 == 前.根; });
        if (位置 == 联合.节点组.end() || !位置->快照
            || 位置->快照->结构.节点 != 前.基础.节点
            || 位置->快照->结构.字段组 != 前.基础.字段组
            || 位置->快照->入向外部关系组 != 前.入向) {
            结果.状态 = 指代概念查询状态::读取漂移;
            return 结果;
        }
    }
    for (const auto& 项 : 联合.节点组) {
        if (!项.快照) {
            结果.状态 = 指代概念查询状态::结构不一致;
            return 结果;
        }
        if (项.查询节点 == 概念节点) {
            for (const auto& 字段 : 项.快照->结构.字段组)
                if (字段.字段节点 == 名称关系字段)
                    规格.反向名称字段组_.push_back(字段);
            std::vector<稳定编码> 当前名称;
            for (const auto& 字段 : 规格.反向名称字段组_) {
                const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                if (!引用 || !有效(*引用)) {
                    结果.状态 = 指代概念查询状态::结构不一致;
                    return 结果;
                }
                当前名称.push_back(*引用);
            }
            std::sort(当前名称.begin(), 当前名称.end());
            if (当前名称 != 命中->名称关系) {
                结果.状态 = 指代概念查询状态::读取漂移;
                return 结果;
            }
        }
        规格.预期节点组_.push_back(*项.快照);
    }
    结果.状态 = 指代概念查询状态::已找到;
    结果.规格 = std::move(规格);
    return 结果;
}

} // namespace 海中鱼巣
