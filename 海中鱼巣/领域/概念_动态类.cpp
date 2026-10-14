#include "概念_动态类.h"
#include "概念_存在类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <mutex>
#include <set>
#include <limits>
#include <stdexcept>

namespace 海中鱼巣 {

稳定编码 动态概念图根节点{};

namespace {

std::recursive_mutex 动态概念图互斥;

稳定编码 节点类型字段{};
稳定编码 实例动态概念节点类型{};
稳定编码 聚合动态概念节点类型{};
稳定编码 对应动态字段{};
稳定编码 状态变化项字段{};
稳定编码 状态变化项节点类型{};
稳定编码 顺序字段{};
稳定编码 特征类型概念字段{};
稳定编码 前值域概念字段{};
稳定编码 后值域概念字段{};
稳定编码 相对时间字段{};
稳定编码 动态特征项字段{};
稳定编码 动态特征项节点类型{};
稳定编码 二次特征类型概念字段{};
稳定编码 动态特征准确结果字段{};
稳定编码 动态特征结果值域字段{};
稳定编码 前来源状态字段{};
稳定编码 后来源状态字段{};

稳定编码 定义类别字段{};
稳定编码 动作能力字段{}, 动作抽象方法字段{}, 动作主体概念字段{};
稳定编码 动作主体角色字段{}, 动作目标概念字段{}, 动作目标角色字段{};
稳定编码 动作目标类型字段{}, 动作规格形状字段{}, 动作规格引用字段{};

auto 动作字段组() noexcept {
    return std::array{&定义类别字段, &动作能力字段, &动作抽象方法字段,
        &动作主体概念字段, &动作主体角色字段, &动作目标概念字段,
        &动作目标角色字段, &动作目标类型字段, &动作规格形状字段, &动作规格引用字段};
}

bool 动作字段有效() noexcept {
    const auto 字段 = 动作字段组();
    return std::all_of(字段.begin(), 字段.end(),
        [](const auto* 编码) { return 有效(*编码)
            && 全局基础数据集.查询节点(*编码).has_value(); });
}

constexpr std::int64_t 组织归属关系角色 = 1;
constexpr std::int64_t 概念上下位关系角色 = 2;

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已初始化() noexcept {
    return 有效(动态概念图根节点)
        && 有效(节点类型字段)
        && 有效(实例动态概念节点类型)
        && 有效(聚合动态概念节点类型)
        && 有效(对应动态字段)
        && 有效(状态变化项字段)
        && 有效(状态变化项节点类型)
        && 有效(顺序字段)
        && 有效(特征类型概念字段)
        && 有效(前值域概念字段)
        && 有效(后值域概念字段)
        && 有效(相对时间字段)
        && 有效(动态特征项字段)
        && 有效(动态特征项节点类型)
        && 有效(二次特征类型概念字段)
        && 有效(动态特征准确结果字段)
        && 有效(动态特征结果值域字段)
        && 有效(前来源状态字段)
        && 有效(后来源状态字段)
        && 动作字段有效();
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(动态概念图根节点)
        && 节点仍存在(节点类型字段)
        && 节点仍存在(实例动态概念节点类型)
        && 节点仍存在(聚合动态概念节点类型)
        && 节点仍存在(对应动态字段)
        && 节点仍存在(状态变化项字段)
        && 节点仍存在(状态变化项节点类型)
        && 节点仍存在(顺序字段)
        && 节点仍存在(特征类型概念字段)
        && 节点仍存在(前值域概念字段)
        && 节点仍存在(后值域概念字段)
        && 节点仍存在(相对时间字段)
        && 节点仍存在(动态特征项字段)
        && 节点仍存在(动态特征项节点类型)
        && 节点仍存在(二次特征类型概念字段)
        && 节点仍存在(动态特征准确结果字段)
        && 节点仍存在(动态特征结果值域字段)
        && 节点仍存在(前来源状态字段)
        && 节点仍存在(后来源状态字段);
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 && 节点仍存在(*目标)
        ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<std::int64_t> 读取唯一I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&值->材料);
    return I64 ? std::optional<std::int64_t>{*I64} : std::nullopt;
}

std::optional<std::optional<std::int64_t>> 读取可选I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.empty()) {
        return std::optional<std::optional<std::int64_t>>{
            std::in_place, std::nullopt};
    }
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&值->材料);
    if (!I64) return std::nullopt;
    return std::optional<std::optional<std::int64_t>>{
        std::in_place, std::optional<std::int64_t>{*I64}};
}

std::optional<std::optional<稳定编码>> 读取可选节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.empty()) return std::optional<std::optional<稳定编码>>{
        std::in_place, std::nullopt};
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    if (!目标 || !节点仍存在(*目标)) return std::nullopt;
    return std::optional<std::optional<稳定编码>>{
        std::in_place, std::optional<稳定编码>{*目标}};
}

std::optional<动态概念种类> 读取概念种类(稳定编码 节点) {
    const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
    if (!类型) return std::nullopt;
    if (*类型 == 实例动态概念节点类型) return 动态概念种类::实例;
    if (*类型 == 聚合动态概念节点类型) return 动态概念种类::聚合;
    return std::nullopt;
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

void 排序去重(std::vector<稳定编码>& 节点组) {
    std::sort(节点组.begin(), 节点组.end());
    节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
}

std::optional<特征概念值域> 形成准确单值域(
    const 新状态信息& 状态) {
    if (const auto* I64 = std::get_if<std::int64_t>(&状态.固定准确值)) {
        return 特征概念值域{
            std::vector<特征概念I64闭区间>{{*I64, *I64}}};
    }
    const auto* 节点 = std::get_if<稳定编码>(&状态.固定准确值);
    return 节点 && 节点仍存在(*节点)
        ? std::optional<特征概念值域>{
            特征概念值域{std::vector<稳定编码>{*节点}}}
        : std::nullopt;
}

std::optional<稳定编码> 取得准确值域概念(
    const 新状态信息& 状态,
    概念_特征类& 特征概念服务,
    const 新_特征值类& 特征值服务) {
    const auto 定位 = 特征概念服务.比较单特征概念(
        状态.特征类型概念节点, 状态.特征类型概念节点);
    if (定位.状态 != 特征概念规则状态::已完成
        || !定位.特征类型根) return std::nullopt;
    const auto 根 = 特征概念服务.获取特征概念(*定位.特征类型根);
    if (!根) return std::nullopt;
    const auto 查找 = 特征概念服务.查找值对应的最具体概念(
        *定位.特征类型根, 状态.固定准确值, 根->单位, 特征值服务);
    if (查找.状态 != 特征概念查找状态::已找到 || !查找.概念节点) {
        return std::nullopt;
    }
    const auto 单值域 = 形成准确单值域(状态);
    const auto 已有 = 特征概念服务.获取特征概念(*查找.概念节点);
    if (!单值域 || !已有) return std::nullopt;
    if (已有->值域 == *单值域) return 查找.概念节点;

    特征概念定义 定义;
    定义.材料类型 = 根->材料类型;
    定义.值域 = *单值域;
    定义.单位 = 根->单位;
    定义.比较规则 = 根->比较规则;
    定义.聚合规则 = 根->聚合规则;
    const auto 新概念 = 特征概念服务.建立或取得特征概念(
        定义, 特征值服务, *查找.概念节点);
    return 有效(新概念)
        ? std::optional<稳定编码>{新概念} : std::nullopt;
}

std::optional<std::vector<动态概念状态项>> 形成状态变化组(
    const std::vector<新状态信息>& 状态组,
    概念_特征类& 特征概念服务,
    const 新_特征值类& 特征值服务) {
    if (状态组.size() < 2) return std::nullopt;
    std::vector<动态概念状态项> 结果;
    结果.reserve(状态组.size() - 1);
    for (std::size_t i = 1; i < 状态组.size(); ++i) {
        const auto& 前 = 状态组[i - 1];
        const auto& 后 = 状态组[i];
        if (前.被描述存在节点 != 后.被描述存在节点
            || 前.强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后.强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后.强时间.纳秒 <= 前.强时间.纳秒) {
            return std::nullopt;
        }
        const auto 前特征定位 = 特征概念服务.比较单特征概念(
            前.特征类型概念节点, 前.特征类型概念节点);
        const auto 后特征定位 = 特征概念服务.比较单特征概念(
            后.特征类型概念节点, 后.特征类型概念节点);
        const auto 前值域 = 取得准确值域概念(
            前, 特征概念服务, 特征值服务);
        const auto 后值域 = 取得准确值域概念(
            后, 特征概念服务, 特征值服务);
        if (前特征定位.状态 != 特征概念规则状态::已完成
            || 后特征定位.状态 != 特征概念规则状态::已完成
            || !前特征定位.特征类型根 || !后特征定位.特征类型根
            || *前特征定位.特征类型根 != *后特征定位.特征类型根
            || !前值域 || !后值域) {
            return std::nullopt;
        }
        结果.push_back({*前特征定位.特征类型根, *前值域, *后值域,
            后.强时间.纳秒 - 前.强时间.纳秒});
    }
    return 结果;
}

struct 已建状态项 final {
    稳定编码 节点;
    std::vector<稳定编码> 字段组;
    std::optional<稳定编码> 持有字段;
};

void 清理状态项组(std::vector<已建状态项>& 项组) noexcept {
    for (auto 位置 = 项组.rbegin(); 位置 != 项组.rend(); ++位置) {
        if (位置->持有字段) (void)全局基础数据集.删除字段(*位置->持有字段);
        for (auto 字段 = 位置->字段组.rbegin(); 字段 != 位置->字段组.rend(); ++字段) {
            (void)全局基础数据集.删除字段(*字段);
        }
        (void)全局基础数据集.删除节点(位置->节点);
    }
}

struct 已建动态特征项 final {
    稳定编码 节点;
    std::vector<稳定编码> 字段组;
    std::optional<稳定编码> 持有字段;
};

void 清理动态特征项组(std::vector<已建动态特征项>& 项组) noexcept {
    for (auto 位置 = 项组.rbegin(); 位置 != 项组.rend(); ++位置) {
        if (位置->持有字段) {
            (void)全局基础数据集.删除字段(*位置->持有字段);
        }
        for (auto 字段 = 位置->字段组.rbegin();
            字段 != 位置->字段组.rend(); ++字段) {
            (void)全局基础数据集.删除字段(*字段);
        }
        (void)全局基础数据集.删除节点(位置->节点);
    }
}

bool 动态特征组有效(
    const std::vector<动态概念特征项>& 特征组,
    const 概念_二次特征类& 二次特征概念服务,
    bool 允许省略准确结果 = false) {
    if (特征组.empty()) return false;
    std::set<std::pair<std::int64_t, std::uint64_t>> 唯一项;
    for (const auto& 项 : 特征组) {
        if (项.状态间位置 < 0
            || !有效(项.二次特征类型概念节点)
            || !有效(项.结果值域概念节点)
            || (!项.准确结果 && !允许省略准确结果)
            || 项.前来源状态节点.has_value()
                != 项.后来源状态节点.has_value()
            || (!项.准确结果 && 项.前来源状态节点)
            || (项.前来源状态节点
                && (!节点仍存在(*项.前来源状态节点)
                    || !节点仍存在(*项.后来源状态节点)))
            || !唯一项.insert({项.状态间位置,
                项.二次特征类型概念节点.值}).second) {
            return false;
        }
        const auto 类型 = 二次特征概念服务.获取二次特征概念(
            项.二次特征类型概念节点);
        const auto 值域 = 二次特征概念服务.获取二次特征概念(
            项.结果值域概念节点);
        if (!类型
            || 类型->二次特征类型根节点 != 项.二次特征类型概念节点
            || !值域
            || 值域->二次特征类型根节点 != 项.二次特征类型概念节点) {
            return false;
        }
        if (项.准确结果) {
            const auto* I64值域 = std::get_if<std::vector<特征概念I64闭区间>>(
                &值域->输出特征概念.值域);
            if (!I64值域 || std::none_of(I64值域->begin(), I64值域->end(),
                    [&](const auto& 区间) {
                        return 区间.下界 <= *项.准确结果
                            && *项.准确结果 <= 区间.上界;
                    })) {
                return false;
            }
        }
    }
    return true;
}

std::optional<std::vector<已建动态特征项>> 建立动态特征项组(
    const std::vector<动态概念特征项>& 特征组) {
    std::vector<已建动态特征项> 结果;
    结果.reserve(特征组.size());
    for (const auto& 项 : 特征组) {
        已建动态特征项 已建;
        已建.节点 = 全局基础数据集.新建节点();
        if (!有效(已建.节点)) {
            清理动态特征项组(结果);
            return std::nullopt;
        }
        auto 记录 = [&](稳定编码 字段) {
            if (!有效(字段)) return false;
            已建.字段组.push_back(字段);
            return true;
        };
        bool 完整 =
            记录(全局基础数据集.添加字段节点(
                已建.节点, 节点类型字段, 动态特征项节点类型))
            && 记录(全局基础数据集.添加字段值(
                已建.节点, 顺序字段, 基础值{项.状态间位置}))
            && 记录(全局基础数据集.添加字段节点(
                已建.节点, 二次特征类型概念字段,
                项.二次特征类型概念节点));
        if (完整 && 项.准确结果) {
            完整 = 记录(全局基础数据集.添加字段值(
                已建.节点, 动态特征准确结果字段,
                基础值{*项.准确结果}));
        }
        完整 = 完整
            && 记录(全局基础数据集.添加字段节点(
                已建.节点, 动态特征结果值域字段,
                项.结果值域概念节点));
        if (完整 && 项.前来源状态节点) {
            完整 = 记录(全局基础数据集.添加字段节点(
                    已建.节点, 前来源状态字段,
                    *项.前来源状态节点))
                && 记录(全局基础数据集.添加字段节点(
                    已建.节点, 后来源状态字段,
                    *项.后来源状态节点));
        }
        结果.push_back(std::move(已建));
        if (!完整) {
            清理动态特征项组(结果);
            return std::nullopt;
        }
    }
    return 结果;
}

std::optional<std::vector<动态概念特征项>> 读取动态特征组(
    稳定编码 概念节点,
    const 概念_二次特征类& 二次特征概念服务) {
    std::vector<动态概念特征项> 结果;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        概念节点, 动态特征项字段)) {
        const auto* 项节点 = std::get_if<稳定编码>(&持有.内容);
        if (!项节点 || !节点仍存在(*项节点)) return std::nullopt;
        const auto 类型 = 读取唯一节点字段(*项节点, 节点类型字段);
        const auto 位置 = 读取唯一I64字段(*项节点, 顺序字段);
        const auto 二次类型 = 读取唯一节点字段(
            *项节点, 二次特征类型概念字段);
        const auto 准确结果 = 读取可选I64字段(
            *项节点, 动态特征准确结果字段);
        const auto 结果值域 = 读取唯一节点字段(
            *项节点, 动态特征结果值域字段);
        const auto 前来源 = 读取可选节点字段(*项节点, 前来源状态字段);
        const auto 后来源 = 读取可选节点字段(*项节点, 后来源状态字段);
        if (!类型 || *类型 != 动态特征项节点类型
            || !位置 || !二次类型 || !准确结果 || !结果值域
            || !前来源 || !后来源) {
            return std::nullopt;
        }
        结果.push_back({*位置, *二次类型, *准确结果, *结果值域,
            *前来源, *后来源});
    }
    std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
        if (左.状态间位置 != 右.状态间位置) {
            return 左.状态间位置 < 右.状态间位置;
        }
        return 左.二次特征类型概念节点.值
            < 右.二次特征类型概念节点.值;
    });
    if (!结果.empty()
        && !动态特征组有效(结果, 二次特征概念服务, true)) {
        return std::nullopt;
    }
    return 结果;
}

std::optional<std::vector<已建状态项>> 建立状态项组(
    const std::vector<动态概念状态项>& 状态变化组) {
    std::vector<已建状态项> 结果;
    for (std::size_t i = 0; i < 状态变化组.size(); ++i) {
        const auto& 项 = 状态变化组[i];
        已建状态项 已建;
        已建.节点 = 全局基础数据集.新建节点();
        if (!有效(已建.节点)) {
            清理状态项组(结果);
            return std::nullopt;
        }
        auto 记录 = [&](稳定编码 字段) {
            if (!有效(字段)) return false;
            已建.字段组.push_back(字段);
            return true;
        };
        bool 完整 =
            记录(全局基础数据集.添加字段节点(
                已建.节点, 节点类型字段, 状态变化项节点类型))
            && 记录(全局基础数据集.添加字段值(
                已建.节点, 顺序字段, 基础值{static_cast<std::int64_t>(i)}))
            && 记录(全局基础数据集.添加字段节点(
                已建.节点, 特征类型概念字段, 项.特征类型概念节点))
            && 记录(全局基础数据集.添加字段节点(
                已建.节点, 前值域概念字段, 项.前值域概念节点))
            && 记录(全局基础数据集.添加字段节点(
                已建.节点, 后值域概念字段, 项.后值域概念节点))
            ;
        if (完整 && 项.相对纳秒) {
            完整 = 记录(全局基础数据集.添加字段值(
                已建.节点, 相对时间字段, 基础值{*项.相对纳秒}));
        }
        结果.push_back(std::move(已建));
        if (!完整) {
            清理状态项组(结果);
            return std::nullopt;
        }
    }
    return 结果;
}

std::optional<std::vector<动态概念状态项>> 读取状态变化组(
    稳定编码 概念节点,
    概念_特征类& 特征概念服务) {
    std::vector<std::pair<std::int64_t, 动态概念状态项>> 排序项;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        概念节点, 状态变化项字段)) {
        const auto* 项节点 = std::get_if<稳定编码>(&持有.内容);
        if (!项节点 || !节点仍存在(*项节点)) return std::nullopt;
        const auto 类型 = 读取唯一节点字段(*项节点, 节点类型字段);
        const auto 顺序 = 读取唯一I64字段(*项节点, 顺序字段);
        const auto 特征类型 = 读取唯一节点字段(*项节点, 特征类型概念字段);
        const auto 前值域 = 读取唯一节点字段(*项节点, 前值域概念字段);
        const auto 后值域 = 读取唯一节点字段(*项节点, 后值域概念字段);
        const auto 时间 = 读取可选I64字段(*项节点, 相对时间字段);
        if (!类型 || *类型 != 状态变化项节点类型 || !顺序 || *顺序 < 0
            || !特征类型 || !前值域 || !后值域 || !时间
            || (时间->has_value() && **时间 <= 0)
            || !特征概念服务.是特征概念节点(*特征类型)
            || !特征概念服务.是特征概念节点(*前值域)
            || !特征概念服务.是特征概念节点(*后值域)) {
            return std::nullopt;
        }
        排序项.push_back({*顺序,
            {*特征类型, *前值域, *后值域, *时间}});
    }
    std::sort(排序项.begin(), 排序项.end(), [](const auto& 左, const auto& 右) {
        return 左.first < 右.first;
    });
    std::vector<动态概念状态项> 结果;
    结果.reserve(排序项.size());
    for (std::size_t i = 0; i < 排序项.size(); ++i) {
        if (排序项[i].first != static_cast<std::int64_t>(i)) return std::nullopt;
        结果.push_back(排序项[i].second);
    }
    return 结果;
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

bool 状态变化可由上位包含(
    const std::vector<动态概念状态项>& 上位,
    const std::vector<动态概念状态项>& 下位,
    概念_特征类& 特征概念服务) {
    if (上位.size() != 下位.size()) return false;
    for (std::size_t i = 0; i < 上位.size(); ++i) {
        if (上位[i].特征类型概念节点 != 下位[i].特征类型概念节点
            || (上位[i].相对纳秒
                && 上位[i].相对纳秒 != 下位[i].相对纳秒)) return false;
        for (const auto 对 : {
            std::pair{上位[i].前值域概念节点, 下位[i].前值域概念节点},
            std::pair{上位[i].后值域概念节点, 下位[i].后值域概念节点}}) {
            const auto 比较 = 特征概念服务.比较单特征概念(对.first, 对.second);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系
                || (*比较.关系 != 单特征概念关系::同一
                    && *比较.关系 != 单特征概念关系::左为上位)) return false;
        }
    }
    return true;
}

bool 动态特征可由上位包含(
    const std::vector<动态概念特征项>& 上位,
    const std::vector<动态概念特征项>& 下位,
    概念_特征类& 特征概念服务) {
    if (上位.size() != 下位.size()) return false;
    for (std::size_t i = 0; i < 上位.size(); ++i) {
        if (上位[i].状态间位置 != 下位[i].状态间位置
            || 上位[i].二次特征类型概念节点
                != 下位[i].二次特征类型概念节点) {
            return false;
        }
        const auto 上位值域 = 特征概念服务.获取特征概念(
            上位[i].结果值域概念节点);
        const auto 下位值域 = 特征概念服务.获取特征概念(
            下位[i].结果值域概念节点);
        if (!上位值域 || !下位值域
            || 上位值域->材料类型 != 下位值域->材料类型) {
            return false;
        }
        const auto 上位比较域 = 上位[i].准确结果
            ? 特征概念值域{std::vector<特征概念I64闭区间>{
                {*上位[i].准确结果, *上位[i].准确结果}}}
            : 上位值域->值域;
        const auto 下位比较域 = 下位[i].准确结果
            ? 特征概念值域{std::vector<特征概念I64闭区间>{
                {*下位[i].准确结果, *下位[i].准确结果}}}
            : 下位值域->值域;
        const auto 比较 = 特征概念服务.比较值域(
            上位值域->材料类型, 上位比较域, 下位比较域);
        if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系
            || (*比较.关系 != 特征概念值域关系::相等
                && *比较.关系 != 特征概念值域关系::左包含右)) {
            return false;
        }
    }
    return true;
}

std::optional<std::vector<动态概念特征项>> 聚合动态特征组(
    const std::vector<std::vector<动态概念特征项>>& 来源特征组,
    概念_二次特征类& 二次特征概念服务) {
    if (来源特征组.size() < 2 || 来源特征组.front().empty()) {
        return std::nullopt;
    }
    const auto 数量 = 来源特征组.front().size();
    for (const auto& 特征组 : 来源特征组) {
        if (特征组.size() != 数量) return std::nullopt;
    }

    std::vector<动态概念特征项> 结果;
    结果.reserve(数量);
    for (std::size_t 位置 = 0; 位置 < 数量; ++位置) {
        const auto 状态间位置 = 来源特征组.front()[位置].状态间位置;
        const auto 类型根 =
            来源特征组.front()[位置].二次特征类型概念节点;
        const auto 类型 = 二次特征概念服务.获取二次特征概念(类型根);
        if (!类型 || 类型->二次特征类型根节点 != 类型根) return std::nullopt;
        if (类型->算法 == 二次特征算法::服务任务集合判等) {
            bool 有不同 = false;
            bool 有相同 = false;
            const auto 收集值 = [&](std::int64_t 值) {
                if (值 == 0) 有不同 = true;
                else if (值 == 10000) 有相同 = true;
                else return false;
                return true;
            };
            for (const auto& 特征组 : 来源特征组) {
                const auto& 项 = 特征组[位置];
                if (项.状态间位置 != 状态间位置
                    || 项.二次特征类型概念节点 != 类型根) return std::nullopt;
                if (项.准确结果) {
                    if (!收集值(*项.准确结果)) return std::nullopt;
                    continue;
                }
                const auto 域 = 二次特征概念服务.获取二次特征概念(项.结果值域概念节点);
                if (!域 || 域->二次特征类型根节点 != 类型根) return std::nullopt;
                const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(
                    &域->输出特征概念.值域);
                if (!区间组 || 区间组->empty()) return std::nullopt;
                for (const auto& 区间 : *区间组) {
                    if (区间.下界 != 区间.上界 || !收集值(区间.下界)) return std::nullopt;
                }
            }
            std::vector<特征概念I64闭区间> 离散域;
            if (有不同) 离散域.push_back({0, 0});
            if (有相同) 离散域.push_back({10000, 10000});
            const auto 已确保 = 二次特征概念服务.建立或取得结果值域概念(
                类型根, 特征概念值域{离散域});
            if ((已确保.状态 != 二次特征概念操作状态::已建立
                    && 已确保.状态 != 二次特征概念操作状态::已复用)
                || !已确保.概念节点) return std::nullopt;
            结果.push_back({状态间位置, 类型根, std::nullopt,
                *已确保.概念节点, std::nullopt, std::nullopt});
            continue;
        }
        std::optional<std::int64_t> 最小值;
        std::optional<std::int64_t> 最大值;
        for (const auto& 特征组 : 来源特征组) {
            const auto& 项 = 特征组[位置];
            if (项.状态间位置 != 状态间位置
                || 项.二次特征类型概念节点 != 类型根) {
                return std::nullopt;
            }
            if (项.准确结果) {
                最小值 = 最小值
                    ? std::min(*最小值, *项.准确结果) : *项.准确结果;
                最大值 = 最大值
                    ? std::max(*最大值, *项.准确结果) : *项.准确结果;
                continue;
            }
            const auto 值域 = 二次特征概念服务.获取二次特征概念(
                项.结果值域概念节点);
            if (!值域 || 值域->二次特征类型根节点 != 类型根
                || 值域->输出特征概念.材料类型
                    != 特征概念材料物理类型::I64标量) {
                return std::nullopt;
            }
            const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(
                &值域->输出特征概念.值域);
            if (!区间组 || 区间组->empty()) return std::nullopt;
            for (const auto& 区间 : *区间组) {
                最小值 = 最小值 ? std::min(*最小值, 区间.下界) : 区间.下界;
                最大值 = 最大值 ? std::max(*最大值, 区间.上界) : 区间.上界;
            }
        }
        if (!最小值 || !最大值) return std::nullopt;
        const auto 已确保 = 二次特征概念服务.建立或取得结果值域概念(
            类型根,
            特征概念值域{std::vector<特征概念I64闭区间>{
                {*最小值, *最大值}}});
        if ((已确保.状态 != 二次特征概念操作状态::已建立
                && 已确保.状态 != 二次特征概念操作状态::已复用)
            || !已确保.概念节点) {
            return std::nullopt;
        }
        结果.push_back({状态间位置, 类型根, std::nullopt,
            *已确保.概念节点, std::nullopt, std::nullopt});
    }
    return 结果;
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

} // namespace

概念_动态类::概念_动态类(
    概念_特征类& 特征概念服务,
    const 新_特征值类& 特征值服务,
    概念_二次特征类& 二次特征概念服务,
    const 概念_存在类* 动作存在概念读取) noexcept
    : 特征概念服务_(特征概念服务),
      特征值服务_(特征值服务),
      动作存在概念读取_(动作存在概念读取),
      二次特征概念服务_(二次特征概念服务) {}

bool 概念_动态类::初始化() noexcept {
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!二次特征概念服务_.初始化()) return false;
        if (结构已初始化()) return 结构仍存在();
        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        for (auto* 字段 : 动作字段组()) if (!建立(*字段)) return false;
        return 建立(动态概念图根节点)
            && 建立(节点类型字段)
            && 建立(实例动态概念节点类型)
            && 建立(聚合动态概念节点类型)
            && 建立(对应动态字段)
            && 建立(状态变化项字段)
            && 建立(状态变化项节点类型)
            && 建立(顺序字段)
            && 建立(特征类型概念字段)
            && 建立(前值域概念字段)
            && 建立(后值域概念字段)
            && 建立(相对时间字段)
            && 建立(动态特征项字段)
            && 建立(动态特征项节点类型)
            && 建立(二次特征类型概念字段)
            && 建立(动态特征准确结果字段)
            && 建立(动态特征结果值域字段)
            && 建立(前来源状态字段)
            && 建立(后来源状态字段);
    } catch (...) {
        return false;
    }
}

namespace {

bool 动作对象类别合法(动作参数对象类别 类别) noexcept {
    switch (类别) {
    case 动作参数对象类别::普通世界:
    case 动作参数对象类别::任务虚拟存在:
    case 动作参数对象类别::服务要求来源人类:
    case 动作参数对象类别::服务要求目标存在:
    case 动作参数对象类别::服务要求目标特征类型:
    case 动作参数对象类别::服务要求目标定义:
    case 动作参数对象类别::服务要求确认: return true;
    }
    return false;
}

动作概念定义 规范化动作定义(动作概念定义 定义,
    const 概念_特征类& 特征, const 概念_二次特征类& 二次,
    const 概念_存在类* 存在) {
    if (!存在) throw std::runtime_error("action existence concept reader not configured");
    const auto 校验存在C = [&](const auto& C) {
        if (C && !存在->获取存在概念(*C)) throw std::runtime_error("action role is not an existence concept");
    };
    校验存在C(定义.执行主体.存在概念节点); 校验存在C(定义.作用目标.存在概念节点);
    const auto 特征根 = [&](稳定编码 C) {
        const auto 根 = 特征.比较单特征概念(C, C);
        if (根.状态 != 特征概念规则状态::已完成 || !根.特征类型根)
            throw std::runtime_error("action feature concept type is not readable");
        return *根.特征类型根;
    };
    const auto 可选存在 = [](const auto& 节点) { return !节点 || 节点仍存在(*节点); };
    if (!节点仍存在(定义.能力定义节点) || !节点仍存在(定义.抽象方法节点)
        || !定义.执行主体.存在概念节点
        || !可选存在(定义.执行主体.存在概念节点)
        || !可选存在(定义.执行主体.参数角色节点)
        || !可选存在(定义.作用目标.存在概念节点)
        || !可选存在(定义.作用目标.参数角色节点)
        || 定义.执行主体.对象类别 != 动作参数对象类别::普通世界
        || (定义.作用目标.对象类别 != 动作参数对象类别::普通世界
            && 定义.作用目标.对象类别 != 动作参数对象类别::任务虚拟存在)
        || (定义.作用目标.对象类别 == 动作参数对象类别::普通世界
            && !定义.作用目标.存在概念节点)
        || (定义.作用目标.对象类别 == 动作参数对象类别::任务虚拟存在
            && (定义.作用目标.存在概念节点 || !定义.作用目标.参数角色节点)))
        throw std::runtime_error("invalid action concept roles");
    if (定义.目标特征类型概念节点 && 特征根(*定义.目标特征类型概念节点) != *定义.目标特征类型概念节点)
        throw std::runtime_error("invalid action target feature concept");
    std::set<稳定编码> 角色;
    for (auto& 规格 : 定义.参数规格组) {
        校验存在C(规格.存在概念节点);
        if (!节点仍存在(规格.参数角色节点) || !角色.insert(规格.参数角色节点).second
            || !可选存在(规格.存在概念节点) || !动作对象类别合法(规格.对象类别))
            throw std::runtime_error("invalid action parameter role");
        if (规格.对象类别 == 动作参数对象类别::任务虚拟存在
            && (规格.存在概念节点 || 规格.要求场景 || 规格.特征规格组.size() != 1))
            throw std::runtime_error("invalid virtual action parameter");
        if (规格.对象类别 != 动作参数对象类别::普通世界
            && 规格.对象类别 != 动作参数对象类别::任务虚拟存在
            && (规格.存在概念节点 || 规格.要求场景 || !规格.特征规格组.empty()))
            throw std::runtime_error("invalid service material parameter");
        std::sort(规格.特征规格组.begin(), 规格.特征规格组.end(),
            [](const auto& a, const auto& b) { return a.特征类型概念节点 < b.特征类型概念节点; });
        std::set<稳定编码> 类型组;
        for (auto& 项 : 规格.特征规格组) {
            const auto 类型 = 特征.获取特征概念(项.特征类型概念节点);
            if (!类型 || 特征根(项.特征类型概念节点) != 项.特征类型概念节点
                || !类型组.insert(项.特征类型概念节点).second)
                throw std::runtime_error("invalid action feature specification");
            if (项.值域概念节点) {
                const auto 域关系 = 特征.比较单特征概念(项.特征类型概念节点, *项.值域概念节点);
                if (域关系.状态 != 特征概念规则状态::已完成 || 域关系.特征类型根 != 项.特征类型概念节点)
                    throw std::runtime_error("invalid action parameter domain");
            }
            排序去重(项.二次特征概念组);
            for (const auto 概念 : 项.二次特征概念组)
                if (!二次.获取二次特征概念(概念))
                    throw std::runtime_error("invalid secondary action parameter concept");
        }
    }
    if (定义.作用目标.对象类别 == 动作参数对象类别::任务虚拟存在) {
        const auto 项 = std::find_if(定义.参数规格组.begin(), 定义.参数规格组.end(),
            [&](const auto& 规格) { return 规格.参数角色节点 == 定义.作用目标.参数角色节点; });
        if (项 == 定义.参数规格组.end() || 项->对象类别 != 动作参数对象类别::任务虚拟存在
            || !定义.目标特征类型概念节点
            || 项->特征规格组.front().特征类型概念节点 != 定义.目标特征类型概念节点)
            throw std::runtime_error("virtual action target does not match declared role");
    }
    return 定义;
}

std::int64_t 形状数量(std::size_t 数量) {
    if (数量 > static_cast<std::size_t>((std::numeric_limits<std::int64_t>::max)()))
        throw std::length_error("action specification exceeds I64 shape");
    return static_cast<std::int64_t>(数量);
}

std::vector<基础节点初始字段> 编码动作定义(const 动作概念定义& 定义) {
    std::vector<基础节点初始字段> 字段{
        {节点类型字段, 聚合动态概念节点类型},
        {定义类别字段, 基础值{static_cast<std::int64_t>(动态概念定义类别::动作)}},
        {动作能力字段, 定义.能力定义节点}, {动作抽象方法字段, 定义.抽象方法节点},
        {动作主体概念字段, *定义.执行主体.存在概念节点}};
    const auto 可选 = [&](稳定编码 键, const auto& 值) { if (值) 字段.push_back({键, *值}); };
    可选(动作主体角色字段, 定义.执行主体.参数角色节点);
    可选(动作目标概念字段, 定义.作用目标.存在概念节点);
    可选(动作目标角色字段, 定义.作用目标.参数角色节点);
    可选(动作目标类型字段, 定义.目标特征类型概念节点);
    std::vector<std::int64_t> 形状{
        static_cast<std::int64_t>(定义.执行主体.对象类别),
        static_cast<std::int64_t>(定义.作用目标.对象类别),
        定义.执行主体.参数角色节点.has_value(), 定义.作用目标.存在概念节点.has_value(),
        定义.作用目标.参数角色节点.has_value(), 定义.目标特征类型概念节点.has_value(),
        形状数量(定义.参数规格组.size())};
    const auto 引用 = [&](稳定编码 值) { 字段.push_back({动作规格引用字段, 值}); };
    for (const auto& 规格 : 定义.参数规格组) {
        形状.insert(形状.end(), {static_cast<std::int64_t>(规格.对象类别),
            规格.存在概念节点.has_value(), 规格.要求场景, 形状数量(规格.特征规格组.size())});
        引用(规格.参数角色节点);
        if (规格.存在概念节点) 引用(*规格.存在概念节点);
        for (const auto& 项 : 规格.特征规格组) {
            形状.insert(形状.end(), {项.值域概念节点.has_value(), 形状数量(项.二次特征概念组.size())});
            引用(项.特征类型概念节点);
            if (项.值域概念节点) 引用(*项.值域概念节点);
            for (const auto 二次 : 项.二次特征概念组) 引用(二次);
        }
    }
    字段.push_back({动作规格形状字段, 基础值{std::move(形状)}});
    return 字段;
}

动作概念定义 解码动作定义(稳定编码 节点) {
    const auto 能力 = 读取唯一节点字段(节点, 动作能力字段);
    const auto 方法 = 读取唯一节点字段(节点, 动作抽象方法字段);
    const auto 主体 = 读取唯一节点字段(节点, 动作主体概念字段);
    const auto 主体角色 = 读取可选节点字段(节点, 动作主体角色字段);
    const auto 目标 = 读取可选节点字段(节点, 动作目标概念字段);
    const auto 目标角色 = 读取可选节点字段(节点, 动作目标角色字段);
    const auto 目标类型 = 读取可选节点字段(节点, 动作目标类型字段);
    if (!能力 || !方法 || !主体 || !主体角色 || !目标 || !目标角色 || !目标类型)
        throw std::runtime_error("incomplete action concept references");
    const auto 形状字段 = 全局基础数据集.查询字段(节点, 动作规格形状字段);
    const auto* 值 = 形状字段.size() == 1 ? std::get_if<基础值>(&形状字段.front().内容) : nullptr;
    const auto* 形状 = 值 ? std::get_if<std::vector<std::int64_t>>(&值->材料) : nullptr;
    if (!形状 || 形状->size() < 7) throw std::runtime_error("invalid action concept shape");
    auto 引用字段 = 全局基础数据集.查询字段(节点, 动作规格引用字段);
    std::sort(引用字段.begin(), 引用字段.end(), [](const auto& a, const auto& b) { return a.编码 < b.编码; });
    std::size_t si = 0, ri = 0;
    const auto 数字 = [&]() { return 形状->at(si++); };
    const auto 布尔 = [&]() { const auto v = 数字(); if (v != 0 && v != 1) throw std::runtime_error("invalid action shape bool"); return v != 0; };
    const auto 数量 = [&]() { const auto v = 数字(); if (v < 0 || static_cast<std::uint64_t>(v) > 引用字段.size()) throw std::runtime_error("invalid action shape count"); return static_cast<std::size_t>(v); };
    const auto 引用 = [&]() { const auto* v = std::get_if<稳定编码>(&引用字段.at(ri++).内容); if (!v || !节点仍存在(*v)) throw std::runtime_error("invalid action concept specification reference"); return *v; };
    动作概念定义 定义;
    定义.能力定义节点 = *能力; 定义.抽象方法节点 = *方法;
    定义.执行主体 = {*主体, *主体角色, static_cast<动作参数对象类别>(数字())};
    定义.作用目标 = {*目标, *目标角色, static_cast<动作参数对象类别>(数字())};
    const auto 有主体角色 = 布尔(), 有目标 = 布尔(), 有目标角色 = 布尔(), 有类型 = 布尔();
    if (有主体角色 != 主体角色->has_value() || 有目标 != 目标->has_value()
        || 有目标角色 != 目标角色->has_value() || 有类型 != 目标类型->has_value())
        throw std::runtime_error("action concept shape/reference mismatch");
    定义.目标特征类型概念节点 = *目标类型;
    const auto 规格数 = 数量();
    for (std::size_t i = 0; i < 规格数; ++i) {
        动作参数规格 规格;
        规格.对象类别 = static_cast<动作参数对象类别>(数字());
        const auto 有存在 = 布尔(); 规格.要求场景 = 布尔(); const auto 特征数 = 数量();
        规格.参数角色节点 = 引用(); if (有存在) 规格.存在概念节点 = 引用();
        for (std::size_t j = 0; j < 特征数; ++j) {
            动作参数特征规格 项;
            const auto 有域 = 布尔(); const auto 二次数 = 数量();
            项.特征类型概念节点 = 引用(); if (有域) 项.值域概念节点 = 引用();
            for (std::size_t k = 0; k < 二次数; ++k) 项.二次特征概念组.push_back(引用());
            规格.特征规格组.push_back(std::move(项));
        }
        定义.参数规格组.push_back(std::move(规格));
    }
    if (si != 形状->size() || ri != 引用字段.size()) throw std::runtime_error("trailing action concept specification data");
    return 定义;
}

} // namespace

动态概念建立结果 概念_动态类::建立或取得动作概念(const 动作概念定义& 输入) noexcept {
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!结构仍存在()) throw std::runtime_error("action concept owner not initialized");
        const auto 定义 = 规范化动作定义(输入, 特征概念服务_, 二次特征概念服务_, 动作存在概念读取_);
        for (const auto 节点 : 查询全部动态概念()) {
            const auto 已有 = 获取动态概念(节点);
            if (已有 && 已有->定义类别 == 动态概念定义类别::动作 && 已有->动作定义 == 定义)
                return {动态概念操作状态::已复用, 节点};
        }
        const auto 新建 = 全局基础数据集.原子建立节点({动态概念图根节点, 编码动作定义(定义)});
        if (新建.状态 != 基础结构操作状态::已完成 || !新建.节点)
            throw std::runtime_error("atomic action concept publication failed");
        const auto 读回 = 获取动态概念(*新建.节点);
        if (!读回 || 读回->动作定义 != 定义) throw std::runtime_error("action concept readback mismatch");
        return {动态概念操作状态::已建立, 新建.节点};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"动作概念发布", L"动作概念定义或原子发布异常。", e.what());
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"动作概念发布", L"未知工程异常。"); }
}

bool 概念_动态类::是动作概念节点(稳定编码 节点) const noexcept {
    const auto 信息 = 获取动态概念(节点);
    return 信息 && 信息->定义类别 == 动态概念定义类别::动作;
}

动态概念建立结果 概念_动态类::建立实例动态概念(
    稳定编码 实际动态节点,
    const std::vector<新状态信息>& 有序状态组) noexcept {
    动态概念建立结果 结果;
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!结构仍存在() || !节点仍存在(实际动态节点)) return 结果;
        if (有序状态组.size() != 2) {
            结果.状态 = 动态概念操作状态::状态数量不合法;
            return 结果;
        }
        const auto 状态变化 = 形成状态变化组(
            有序状态组, 特征概念服务_, 特征值服务_);
        if (!状态变化) {
            结果.状态 = 有序状态组.size() < 2
                ? 动态概念操作状态::状态不足
                : 动态概念操作状态::状态不一致;
            return 结果;
        }
        auto 已建项 = 建立状态项组(*状态变化);
        if (!已建项) {
            结果.状态 = 动态概念操作状态::资源失败;
            return 结果;
        }
        const auto 概念节点 = 全局基础数据集.新建节点();
        std::vector<稳定编码> 已写字段;
        std::optional<稳定编码> 组织关系;
        auto 失败清理 = [&]() noexcept {
            if (组织关系) (void)全局基础数据集.删除关系(*组织关系);
            for (auto 字段 = 已写字段.rbegin(); 字段 != 已写字段.rend(); ++字段) {
                (void)全局基础数据集.删除字段(*字段);
            }
            if (有效(概念节点)) (void)全局基础数据集.删除节点(概念节点);
            清理状态项组(*已建项);
        };
        if (!有效(概念节点)) {
            失败清理();
            结果.状态 = 动态概念操作状态::资源失败;
            return 结果;
        }
        auto 记录 = [&](稳定编码 字段) {
            if (!有效(字段)) return false;
            已写字段.push_back(字段);
            return true;
        };
        bool 完整 = 记录(全局基础数据集.添加字段节点(
                概念节点, 节点类型字段, 实例动态概念节点类型))
            && 记录(全局基础数据集.添加字段节点(
                概念节点, 对应动态字段, 实际动态节点));
        for (auto& 项 : *已建项) {
            if (!完整) break;
            const auto 字段 = 全局基础数据集.添加字段节点(
                概念节点, 状态变化项字段, 项.节点);
            if (!有效(字段)) {
                完整 = false;
                break;
            }
            项.持有字段 = 字段;
        }
        if (完整) {
            const auto 关系 = 全局基础数据集.添加关系(
                动态概念图根节点, 概念节点,
                基础外部关系类型::父子, 组织归属关系角色);
            if (有效(关系)) 组织关系 = 关系;
            else 完整 = false;
        }
        if (!完整) {
            失败清理();
            结果.状态 = 动态概念操作状态::资源失败;
            return 结果;
        }
        const auto 读回 = 获取动态概念(概念节点);
        if (!读回 || 读回->种类 != 动态概念种类::实例
            || 读回->对应动态节点 != 实际动态节点
            || 读回->状态变化组 != *状态变化) {
            失败清理();
            结果.状态 = 动态概念操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 动态概念操作状态::已建立;
        结果.概念节点 = 概念节点;
        return 结果;
    } catch (...) {
        结果.状态 = 动态概念操作状态::资源失败;
        结果.概念节点.reset();
        return 结果;
    }
}

动态概念建立结果 概念_动态类::建立复合实例动态概念(
    稳定编码 实际动态节点,
    const std::vector<稳定编码>& 有序成员动态概念组,
    const std::vector<动态概念特征项>& 跨特征时间特征组) noexcept {
    动态概念建立结果 结果;
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!结构仍存在() || !节点仍存在(实际动态节点)) return 结果;
        if (有序成员动态概念组.size() < 2) {
            结果.状态 = 动态概念操作状态::来源不足;
            return 结果;
        }

        std::vector<动态概念状态项> 状态变化组;
        状态变化组.reserve(有序成员动态概念组.size());
        for (const auto 成员概念节点 : 有序成员动态概念组) {
            const auto 成员概念 = 获取动态概念(成员概念节点);
            if (!成员概念 || 成员概念->定义类别 != 动态概念定义类别::变化) {
                结果.状态 = 动态概念操作状态::概念不存在;
                return 结果;
            }
            if (成员概念->状态变化组.size() != 1) {
                结果.状态 = 动态概念操作状态::状态数量不合法;
                return 结果;
            }
            状态变化组.push_back(成员概念->状态变化组.front());
        }

        auto 规范特征组 = 跨特征时间特征组;
        std::sort(规范特征组.begin(), 规范特征组.end(),
            [](const auto& 左, const auto& 右) {
                if (左.状态间位置 != 右.状态间位置) {
                    return 左.状态间位置 < 右.状态间位置;
                }
                return 左.二次特征类型概念节点.值
                    < 右.二次特征类型概念节点.值;
            });
        if (!动态特征组有效(规范特征组, 二次特征概念服务_)) {
            结果.状态 = 动态概念操作状态::入口拒绝;
            return 结果;
        }

        auto 已建状态项组 = 建立状态项组(状态变化组);
        if (!已建状态项组) {
            结果.状态 = 动态概念操作状态::资源失败;
            return 结果;
        }
        auto 已建特征项组 = 建立动态特征项组(规范特征组);
        if (!已建特征项组) {
            清理状态项组(*已建状态项组);
            结果.状态 = 动态概念操作状态::资源失败;
            return 结果;
        }

        const auto 概念节点 = 全局基础数据集.新建节点();
        std::vector<稳定编码> 已写字段;
        std::optional<稳定编码> 组织关系;
        auto 失败清理 = [&]() noexcept {
            if (组织关系) (void)全局基础数据集.删除关系(*组织关系);
            清理动态特征项组(*已建特征项组);
            清理状态项组(*已建状态项组);
            for (auto 字段 = 已写字段.rbegin(); 字段 != 已写字段.rend(); ++字段) {
                (void)全局基础数据集.删除字段(*字段);
            }
            if (有效(概念节点)) (void)全局基础数据集.删除节点(概念节点);
        };
        if (!有效(概念节点)) {
            失败清理();
            结果.状态 = 动态概念操作状态::资源失败;
            return 结果;
        }
        auto 记录 = [&](稳定编码 字段) {
            if (!有效(字段)) return false;
            已写字段.push_back(字段);
            return true;
        };
        bool 完整 = 记录(全局基础数据集.添加字段节点(
                概念节点, 节点类型字段, 实例动态概念节点类型))
            && 记录(全局基础数据集.添加字段节点(
                概念节点, 对应动态字段, 实际动态节点));
        for (auto& 项 : *已建状态项组) {
            if (!完整) break;
            const auto 字段 = 全局基础数据集.添加字段节点(
                概念节点, 状态变化项字段, 项.节点);
            if (!有效(字段)) 完整 = false;
            else 项.持有字段 = 字段;
        }
        for (auto& 项 : *已建特征项组) {
            if (!完整) break;
            const auto 字段 = 全局基础数据集.添加字段节点(
                概念节点, 动态特征项字段, 项.节点);
            if (!有效(字段)) 完整 = false;
            else 项.持有字段 = 字段;
        }
        if (完整) {
            const auto 关系 = 全局基础数据集.添加关系(
                动态概念图根节点, 概念节点,
                基础外部关系类型::父子, 组织归属关系角色);
            if (有效(关系)) 组织关系 = 关系;
            else 完整 = false;
        }
        if (!完整) {
            失败清理();
            结果.状态 = 动态概念操作状态::资源失败;
            return 结果;
        }

        const auto 读回 = 获取动态概念(概念节点);
        if (!读回 || 读回->种类 != 动态概念种类::实例
            || 读回->对应动态节点 != 实际动态节点
            || 读回->状态变化组 != 状态变化组
            || 读回->动态特征组 != 规范特征组) {
            失败清理();
            结果.状态 = 动态概念操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 动态概念操作状态::已建立;
        结果.概念节点 = 概念节点;
        return 结果;
    } catch (...) {
        结果.状态 = 动态概念操作状态::资源失败;
        结果.概念节点.reset();
        return 结果;
    }
}

bool 概念_动态类::删除实例动态概念(
    稳定编码 实例概念节点,
    稳定编码 实际动态节点) noexcept {
    const auto 结果 = 删除实例概念及动态结构(实例概念节点, 实际动态节点, {}, {}, {});
    if (结果 == 基础结构操作状态::内部不一致 || 结果 == 基础结构操作状态::资源失败)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除实例动态概念", L"私有删除结构损坏或工程操作失败。");
    return 结果 == 基础结构操作状态::已完成;
}

基础结构操作状态 概念_动态类::删除实例概念及动态结构(
    稳定编码 实例概念节点, 稳定编码 实际动态节点,
    const std::vector<稳定编码>& 动态私有节点组,
    const std::vector<稳定编码>& 动态字段关系组,
    const std::vector<稳定编码>& 动态外部关系组) noexcept {
    try {
        std::lock_guard 锁(动态概念图互斥);
        const auto 信息 = 获取动态概念(实例概念节点);
        if (!信息 || 信息->种类 != 动态概念种类::实例
            || 信息->对应动态节点 != 实际动态节点) {
            return 基础结构操作状态::入口拒绝;
        }
        const bool 联合删除 = !动态私有节点组.empty();
        if (联合删除 && std::find(动态私有节点组.begin(), 动态私有节点组.end(), 实际动态节点)
            == 动态私有节点组.end()) return 基础结构操作状态::入口拒绝;
        if (全局基础数据集.有入向字段引用(实例概念节点, 联合删除 ? 实际动态节点 : 稳定编码{}))
            return 基础结构操作状态::引用仍存在;
        auto 删除节点组 = 动态私有节点组;
        auto 删除字段组 = 动态字段关系组;
        auto 删除关系组 = 动态外部关系组;
        删除节点组.push_back(实例概念节点);
        for (const auto& 关系 : 查询概念关系(实例概念节点, true)) {
            删除关系组.push_back(关系.编码);
        }
        for (const auto& 关系 : 查询概念关系(实例概念节点, false)) {
            删除关系组.push_back(关系.编码);
        }
        for (const auto& 关系 : 全局基础数据集.查询源关系(
            动态概念图根节点, 基础外部关系类型::父子)) {
            if (关系.目标节点 == 实例概念节点
                && 关系.角色或顺序 == 组织归属关系角色) {
                删除关系组.push_back(关系.编码);
            }
        }
        for (const auto& 持有 : 全局基础数据集.查询字段(
            实例概念节点, 状态变化项字段)) {
            const auto* 项节点 = std::get_if<稳定编码>(&持有.内容);
            if (!项节点) return 基础结构操作状态::内部不一致;
            删除字段组.push_back(持有.编码);
            删除节点组.push_back(*项节点);
            for (const auto 字段 : std::array{
                节点类型字段, 顺序字段, 特征类型概念字段,
                前值域概念字段, 后值域概念字段, 相对时间字段}) {
                const auto 项字段组 = 全局基础数据集.查询字段(*项节点, 字段);
                if (项字段组.size() != 1) return 基础结构操作状态::内部不一致;
                删除字段组.push_back(项字段组.front().编码);
            }
        }
        for (const auto& 持有 : 全局基础数据集.查询字段(
            实例概念节点, 动态特征项字段)) {
            const auto* 项节点 = std::get_if<稳定编码>(&持有.内容);
            if (!项节点) return 基础结构操作状态::内部不一致;
            删除字段组.push_back(持有.编码);
            删除节点组.push_back(*项节点);
            for (const auto 字段 : std::array{
                节点类型字段, 顺序字段, 二次特征类型概念字段,
                动态特征准确结果字段, 动态特征结果值域字段}) {
                const auto 项字段组 = 全局基础数据集.查询字段(*项节点, 字段);
                if (项字段组.size() != 1) return 基础结构操作状态::内部不一致;
                删除字段组.push_back(项字段组.front().编码);
            }
            for (const auto 字段 : {前来源状态字段, 后来源状态字段}) {
                const auto 项字段组 = 全局基础数据集.查询字段(*项节点, 字段);
                if (项字段组.size() > 1) return 基础结构操作状态::内部不一致;
                if (!项字段组.empty()) 删除字段组.push_back(项字段组.front().编码);
            }
        }
        for (const auto 字段 : {节点类型字段, 对应动态字段}) {
            const auto 字段组 = 全局基础数据集.查询字段(实例概念节点, 字段);
            if (字段组.size() != 1) return 基础结构操作状态::内部不一致;
            删除字段组.push_back(字段组.front().编码);
        }
        const auto 类别字段组 = 全局基础数据集.查询字段(实例概念节点, 定义类别字段);
        if (类别字段组.size() > 1) return 基础结构操作状态::内部不一致;
        if (!类别字段组.empty()) 删除字段组.push_back(类别字段组.front().编码);
        std::sort(删除关系组.begin(), 删除关系组.end());
        删除关系组.erase(std::unique(删除关系组.begin(), 删除关系组.end()), 删除关系组.end());
        const auto 删除结果 = 全局基础数据集.原子删除节点结构(删除节点组, 删除字段组, 删除关系组);
        if (删除结果 == 基础结构操作状态::引用仍存在) return 删除结果;
        if (删除结果 != 基础结构操作状态::已完成)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除实例动态概念", L"完整结构删除失败。");
        return 删除结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除实例动态概念", L"收集或删除概念结构发生工程异常。");
    }
}

动态概念聚合结果 概念_动态类::聚合共同动态概念(
    const std::vector<稳定编码>& 来源概念组) noexcept {
    动态概念聚合结果 结果;
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!结构仍存在()) return 结果;
        auto 来源 = 来源概念组;
        排序去重(来源);
        if (来源.size() < 2) {
            结果.状态 = 动态概念操作状态::来源不足;
            return 结果;
        }
        std::vector<std::vector<动态概念状态项>> 来源变化组;
        std::vector<std::vector<动态概念特征项>> 来源特征组;
        for (const auto 来源概念 : 来源) {
            const auto 信息 = 获取动态概念(来源概念);
            if (!信息 || 信息->定义类别 != 动态概念定义类别::变化) {
                结果.状态 = 动态概念操作状态::概念不存在;
                return 结果;
            }
            if (信息->动态特征组.empty()) {
                结果.状态 = 动态概念操作状态::状态不足;
                return 结果;
            }
            来源变化组.push_back(信息->状态变化组);
            来源特征组.push_back(信息->动态特征组);
        }
        const auto 数量 = 来源变化组.front().size();
        if (数量 == 0) {
            结果.状态 = 动态概念操作状态::无共同点;
            return 结果;
        }
        for (const auto& 变化组 : 来源变化组) {
            if (变化组.size() != 数量) {
                结果.状态 = 动态概念操作状态::无共同点;
                return 结果;
            }
        }
        for (std::size_t 位置 = 0; 位置 < 数量; ++位置) {
            auto 共同前值域 = std::optional<稳定编码>{
                来源变化组.front()[位置].前值域概念节点};
            auto 共同后值域 = std::optional<稳定编码>{
                来源变化组.front()[位置].后值域概念节点};
            const auto 特征类型 = 来源变化组.front()[位置].特征类型概念节点;
            auto 相对时间 = 来源变化组.front()[位置].相对纳秒;
            for (std::size_t i = 1; i < 来源变化组.size(); ++i) {
                const auto& 项 = 来源变化组[i][位置];
                if (项.特征类型概念节点 != 特征类型) {
                    共同前值域.reset();
                    共同后值域.reset();
                    break;
                }
                if (项.相对纳秒 != 相对时间) 相对时间.reset();
                共同前值域 = 共同上位特征概念(
                    *共同前值域, 项.前值域概念节点, 特征概念服务_);
                共同后值域 = 共同上位特征概念(
                    *共同后值域, 项.后值域概念节点, 特征概念服务_);
                if (!共同前值域 || !共同后值域) break;
            }
            if (!共同前值域 || !共同后值域) {
                结果.共同状态变化组.clear();
                结果.状态 = 动态概念操作状态::无共同点;
                return 结果;
            }
            结果.共同状态变化组.push_back(
                {特征类型, *共同前值域, *共同后值域, 相对时间});
        }
        const auto 共同动态特征 = 聚合动态特征组(
            来源特征组, 二次特征概念服务_);
        if (!共同动态特征) {
            结果.状态 = 动态概念操作状态::无共同点;
            return 结果;
        }
        结果.共同动态特征组 = *共同动态特征;

        std::optional<稳定编码> 聚合节点;
        for (const auto& 组织关系 : 全局基础数据集.查询源关系(
            动态概念图根节点, 基础外部关系类型::父子)) {
            if (组织关系.角色或顺序 != 组织归属关系角色) continue;
            const auto 已有 = 获取动态概念(组织关系.目标节点);
            if (已有 && 已有->种类 == 动态概念种类::聚合
                && 已有->状态变化组 == 结果.共同状态变化组
                && 已有->动态特征组 == 结果.共同动态特征组) {
                if (聚合节点) {
                    结果.状态 = 动态概念操作状态::结构不一致;
                    return 结果;
                }
                聚合节点 = 组织关系.目标节点;
            }
        }

        bool 新建 = false;
        std::optional<std::vector<已建状态项>> 新建状态项组;
        std::optional<std::vector<已建动态特征项>> 新建特征项组;
        std::optional<稳定编码> 新建类型字段;
        std::optional<稳定编码> 新建组织关系;
        if (!聚合节点) {
            新建状态项组 = 建立状态项组(结果.共同状态变化组);
            if (!新建状态项组) {
                结果.状态 = 动态概念操作状态::资源失败;
                return 结果;
            }
            新建特征项组 = 建立动态特征项组(结果.共同动态特征组);
            if (!新建特征项组) {
                清理状态项组(*新建状态项组);
                结果.状态 = 动态概念操作状态::资源失败;
                return 结果;
            }
            const auto 节点 = 全局基础数据集.新建节点();
            bool 完整 = 有效(节点);
            if (完整) {
                const auto 字段 = 全局基础数据集.添加字段节点(
                    节点, 节点类型字段, 聚合动态概念节点类型);
                if (有效(字段)) 新建类型字段 = 字段;
                else 完整 = false;
            }
            for (auto& 项 : *新建状态项组) {
                if (!完整) break;
                const auto 字段 = 全局基础数据集.添加字段节点(
                    节点, 状态变化项字段, 项.节点);
                if (!有效(字段)) {
                    完整 = false;
                    break;
                }
                项.持有字段 = 字段;
            }
            for (auto& 项 : *新建特征项组) {
                if (!完整) break;
                const auto 字段 = 全局基础数据集.添加字段节点(
                    节点, 动态特征项字段, 项.节点);
                if (!有效(字段)) {
                    完整 = false;
                    break;
                }
                项.持有字段 = 字段;
            }
            if (完整) {
                const auto 关系 = 全局基础数据集.添加关系(
                    动态概念图根节点, 节点,
                    基础外部关系类型::父子, 组织归属关系角色);
                if (有效(关系)) 新建组织关系 = 关系;
                else 完整 = false;
            }
            if (!完整) {
                if (新建组织关系) {
                    (void)全局基础数据集.删除关系(*新建组织关系);
                }
                if (新建类型字段) {
                    (void)全局基础数据集.删除字段(*新建类型字段);
                }
                清理动态特征项组(*新建特征项组);
                清理状态项组(*新建状态项组);
                if (有效(节点)) (void)全局基础数据集.删除节点(节点);
                结果.状态 = 动态概念操作状态::资源失败;
                return 结果;
            }
            聚合节点 = 节点;
            新建 = true;
        }

        std::vector<稳定编码> 本次新增关系;
        const auto 回滚新增聚合 = [&]() noexcept {
            for (auto 关系 = 本次新增关系.rbegin();
                关系 != 本次新增关系.rend(); ++关系) {
                (void)全局基础数据集.删除关系(*关系);
            }
            if (!新建) return;
            if (新建组织关系) {
                (void)全局基础数据集.删除关系(*新建组织关系);
            }
            if (新建类型字段) {
                (void)全局基础数据集.删除字段(*新建类型字段);
            }
            if (新建特征项组) 清理动态特征项组(*新建特征项组);
            if (新建状态项组) 清理状态项组(*新建状态项组);
            if (聚合节点) (void)全局基础数据集.删除节点(*聚合节点);
        };

        const auto 聚合读回 = 获取动态概念(*聚合节点);
        if (!聚合读回
            || 聚合读回->状态变化组 != 结果.共同状态变化组
            || 聚合读回->动态特征组 != 结果.共同动态特征组) {
            回滚新增聚合();
            结果.状态 = 动态概念操作状态::结构不一致;
            结果.聚合概念节点.reset();
            return 结果;
        }
        for (const auto 来源概念 : 来源) {
            if (来源概念 == *聚合节点) continue;
            bool 已存在 = false;
            for (const auto& 关系 : 查询概念关系(*聚合节点, false)) {
                if (关系.目标节点 == 来源概念) {
                    已存在 = true;
                    break;
                }
            }
            if (已存在) continue;
            const auto 来源信息 = 获取动态概念(来源概念);
            const bool 会成环 = 向下可达(来源概念, *聚合节点);
            if (!来源信息
                || !状态变化可由上位包含(
                    结果.共同状态变化组, 来源信息->状态变化组,
                    特征概念服务_)
                || !动态特征可由上位包含(
                    结果.共同动态特征组, 来源信息->动态特征组,
                    特征概念服务_)
                || 会成环) {
                回滚新增聚合();
                结果.状态 = 会成环
                    ? 动态概念操作状态::会形成环
                    : 动态概念操作状态::层级不成立;
                return 结果;
            }
            const auto 关系 = 全局基础数据集.添加关系(
                *聚合节点, 来源概念,
                基础外部关系类型::父子, 概念上下位关系角色);
            if (!有效(关系)) {
                回滚新增聚合();
                结果.状态 = 动态概念操作状态::资源失败;
                return 结果;
            }
            本次新增关系.push_back(关系);
        }
        结果.状态 = 新建
            ? 动态概念操作状态::已建立
            : 动态概念操作状态::已复用;
        结果.聚合概念节点 = *聚合节点;
        return 结果;
    } catch (...) {
        结果.状态 = 动态概念操作状态::资源失败;
        return 结果;
    }
}

动态概念包含结果 概念_动态类::判断动态概念包含(
    稳定编码 上位概念节点,
    稳定编码 下位概念节点) const noexcept {
    动态概念包含结果 结果;
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!有效(上位概念节点) || !有效(下位概念节点)) return 结果;
        const auto 上位 = 获取动态概念(上位概念节点);
        const auto 下位 = 获取动态概念(下位概念节点);
        if (!上位 || !下位) {
            结果.状态 = 动态概念操作状态::概念不存在;
            return 结果;
        }
        if (上位->定义类别 != 下位->定义类别) {
            结果.状态 = 动态概念操作状态::层级不成立;
            return 结果;
        }
        if (上位->定义类别 == 动态概念定义类别::动作) {
            结果.包含 = 上位->动作定义 == 下位->动作定义;
            结果.状态 = 动态概念操作状态::已判断;
            return 结果;
        }
        结果.包含 = 状态变化可由上位包含(
                上位->状态变化组, 下位->状态变化组, 特征概念服务_)
            && 动态特征可由上位包含(
                上位->动态特征组, 下位->动态特征组, 特征概念服务_);
        结果.状态 = 动态概念操作状态::已判断;
        return 结果;
    } catch (...) {
        结果.状态 = 动态概念操作状态::资源失败;
        结果.包含.reset();
        return 结果;
    }
}

std::vector<稳定编码> 概念_动态类::查询全部动态概念() const noexcept {
    std::vector<稳定编码> 结果;
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!结构仍存在()) return 结果;
        for (const auto& 关系 : 全局基础数据集.查询源关系(
            动态概念图根节点, 基础外部关系类型::父子)) {
            if (关系.角色或顺序 != 组织归属关系角色 && 关系.角色或顺序 != 0) continue;
            const auto 信息 = 获取动态概念(关系.目标节点);
            if (!信息) throw std::runtime_error("dynamic concept root contains incomplete definition");
            if (信息 && (关系.角色或顺序 == 组织归属关系角色
                || (关系.角色或顺序 == 0 && 信息->定义类别 == 动态概念定义类别::动作))) {
                结果.push_back(关系.目标节点);
            }
        }
        排序去重(结果);
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"枚举动态概念", L"概念枚举异常，禁止当作空集合继续创建。", e.what());
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"枚举动态概念", L"未知工程异常。"); }
    return 结果;
}

动态概念操作状态 概念_动态类::保存动态特征(
    稳定编码 概念节点,
    const std::vector<动态概念特征项>& 动态特征组) noexcept {
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!结构仍存在() || !节点仍存在(概念节点)
            || !动态特征组有效(动态特征组, 二次特征概念服务_)) {
            return 动态概念操作状态::入口拒绝;
        }
        const auto 信息 = 获取动态概念(概念节点);
        if (!信息) return 动态概念操作状态::概念不存在;
        if (信息->定义类别 != 动态概念定义类别::变化) return 动态概念操作状态::层级不成立;
        auto 规范组 = 动态特征组;
        std::sort(规范组.begin(), 规范组.end(), [](const auto& 左, const auto& 右) {
            if (左.状态间位置 != 右.状态间位置) {
                return 左.状态间位置 < 右.状态间位置;
            }
            return 左.二次特征类型概念节点.值
                < 右.二次特征类型概念节点.值;
        });
        if (信息->动态特征组 == 规范组) {
            return 动态概念操作状态::无变化;
        }
        // 动态概念定义不在本入口原地改义；已有不同特征必须由上层形成
        // 新概念，而不是覆盖已经发布的概念内容。
        if (!信息->动态特征组.empty()) {
            return 动态概念操作状态::层级不成立;
        }

        auto 已建项 = 建立动态特征项组(规范组);
        if (!已建项) return 动态概念操作状态::资源失败;
        bool 完整 = true;
        for (auto& 项 : *已建项) {
            const auto 字段 = 全局基础数据集.添加字段节点(
                概念节点, 动态特征项字段, 项.节点);
            if (!有效(字段)) {
                完整 = false;
                break;
            }
            项.持有字段 = 字段;
        }
        if (!完整) {
            清理动态特征项组(*已建项);
            return 动态概念操作状态::资源失败;
        }
        const auto 读回 = 获取动态概念(概念节点);
        if (!读回 || 读回->动态特征组 != 规范组) {
            清理动态特征项组(*已建项);
            return 动态概念操作状态::结构不一致;
        }
        return 动态概念操作状态::已更新;
    } catch (...) {
        return 动态概念操作状态::资源失败;
    }
}

std::optional<动态概念信息> 概念_动态类::获取动态概念(
    稳定编码 概念节点) const noexcept {
    try {
        std::lock_guard 锁(动态概念图互斥);
        if (!结构仍存在() || !节点仍存在(概念节点)) return std::nullopt;
        const auto 种类 = 读取概念种类(概念节点);
        if (!种类) return std::nullopt;
        const auto 类别字段组 = 全局基础数据集.查询字段(概念节点, 定义类别字段);
        const auto 类别 = 读取唯一I64字段(概念节点, 定义类别字段);
        const auto 根归属组 = 全局基础数据集.查询目标关系(概念节点, 基础外部关系类型::父子);
        if (类别字段组.empty() && std::any_of(根归属组.begin(), 根归属组.end(), [&](const auto& r) {
                return r.源节点 == 动态概念图根节点 && r.角色或顺序 == 0;
            })) throw std::runtime_error("action concept definition category missing");
        if (!类别字段组.empty()) {
            if (!类别 || (*类别 != static_cast<std::int64_t>(动态概念定义类别::动作)
                && *类别 != static_cast<std::int64_t>(动态概念定义类别::变化)))
                throw std::runtime_error("invalid dynamic concept definition category");
            if (*类别 == static_cast<std::int64_t>(动态概念定义类别::动作)) {
                if (*种类 != 动态概念种类::聚合
                    || !全局基础数据集.查询字段(概念节点, 对应动态字段).empty()
                    || !全局基础数据集.查询字段(概念节点, 状态变化项字段).empty()
                    || !全局基础数据集.查询字段(概念节点, 动态特征项字段).empty()
                    || !查询概念关系(概念节点, true).empty()
                    || !查询概念关系(概念节点, false).empty())
                    throw std::runtime_error("mixed action/change concept structure");
                const auto 归属 = 全局基础数据集.查询目标关系(概念节点, 基础外部关系类型::父子);
                if (std::count_if(归属.begin(), 归属.end(), [&](const auto& r) {
                        return r.源节点 == 动态概念图根节点 && r.角色或顺序 == 0;
                    }) != 1) throw std::runtime_error("action concept root membership mismatch");
                const auto 定义 = 解码动作定义(概念节点);
                if (规范化动作定义(定义, 特征概念服务_, 二次特征概念服务_, 动作存在概念读取_) != 定义)
                    throw std::runtime_error("noncanonical action concept definition");
                动态概念信息 信息;
                信息.节点 = 概念节点; 信息.种类 = 动态概念种类::聚合;
                信息.定义类别 = 动态概念定义类别::动作; 信息.动作定义 = 定义;
                return 信息;
            }
        }
        std::optional<稳定编码> 对应动态;
        const auto 对应字段组 = 全局基础数据集.查询字段(概念节点, 对应动态字段);
        if (*种类 == 动态概念种类::实例) {
            if (对应字段组.size() != 1) return std::nullopt;
            const auto* 目标 = std::get_if<稳定编码>(&对应字段组.front().内容);
            if (!目标 || !节点仍存在(*目标)) return std::nullopt;
            对应动态 = *目标;
        } else if (!对应字段组.empty()) return std::nullopt;
        const auto 状态变化 = 读取状态变化组(概念节点, 特征概念服务_);
        if (!状态变化 || 状态变化->empty()) return std::nullopt;
        const auto 动态特征 = 读取动态特征组(
            概念节点, 二次特征概念服务_);
        if (!动态特征) return std::nullopt;
        if (*种类 == 动态概念种类::实例) {
            if (std::any_of(状态变化->begin(), 状态变化->end(),
                    [](const auto& 项) { return !项.相对纳秒; })
                || std::any_of(动态特征->begin(), 动态特征->end(),
                    [](const auto& 项) { return !项.准确结果; })) {
                return std::nullopt;
            }
        }
        std::vector<稳定编码> 上位;
        std::vector<稳定编码> 下位;
        for (const auto& 关系 : 查询概念关系(概念节点, true)) 上位.push_back(关系.源节点);
        for (const auto& 关系 : 查询概念关系(概念节点, false)) 下位.push_back(关系.目标节点);
        排序去重(上位);
        排序去重(下位);
        return 动态概念信息{概念节点, *种类, 对应动态,
            *状态变化, *动态特征, std::move(上位), std::move(下位)};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取动态概念", L"概念结构读取异常。", e.what());
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取动态概念", L"未知工程异常。"); }
}

bool 概念_动态类::是动态概念节点(稳定编码 节点) const noexcept {
    return 获取动态概念(节点).has_value();
}

bool 概念_动态类::是实例动态概念节点(稳定编码 节点) const noexcept {
    const auto 信息 = 获取动态概念(节点);
    return 信息 && 信息->种类 == 动态概念种类::实例;
}

bool 概念_动态类::是聚合动态概念节点(稳定编码 节点) const noexcept {
    const auto 信息 = 获取动态概念(节点);
    return 信息 && 信息->种类 == 动态概念种类::聚合;
}

std::vector<稳定编码> 概念_动态类::查询上位动态概念(
    稳定编码 概念节点) const noexcept {
    const auto 信息 = 获取动态概念(概念节点);
    return 信息 ? 信息->上位概念组 : std::vector<稳定编码>{};
}

std::vector<稳定编码> 概念_动态类::查询下位动态概念(
    稳定编码 概念节点) const noexcept {
    const auto 信息 = 获取动态概念(概念节点);
    return 信息 ? 信息->下位概念组 : std::vector<稳定编码>{};
}

} // namespace 海中鱼巣
