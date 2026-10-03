#include "新_因果类.h"

#include <algorithm>
#include <limits>
#include <mutex>
#include <tuple>
#include <utility>

namespace 海中鱼巣 {

稳定编码 新因果信息图根节点{};

namespace {

inline constexpr std::int64_t 新因果根成员关系角色 = 0x43415553;

std::recursive_mutex 新因果互斥;

稳定编码 节点类型字段{};
稳定编码 因果图根类型{};
稳定编码 因果节点类型{};
稳定编码 状态模式节点类型{};
稳定编码 动态模式节点类型{};
稳定编码 动作定义节点类型{};

稳定编码 因果类型字段{};
稳定编码 因果条件状态字段{};
稳定编码 因果条件动态字段{};
稳定编码 因果动作字段{};
稳定编码 因果结果状态字段{};
稳定编码 因果形成来源字段{};
稳定编码 因果符合次数字段{};
稳定编码 因果不符合次数字段{};
稳定编码 因果自我尝试次数字段{};
稳定编码 因果自我尝试符合次数字段{};
稳定编码 因果已计数发生字段{};

稳定编码 状态模式存在字段{};
稳定编码 状态模式特征类型字段{};
稳定编码 状态模式准确值字段{};

稳定编码 动态模式主题字段{};
稳定编码 动态模式概念字段{};

稳定编码 动作概念字段{};
稳定编码 动作执行主体字段{};
稳定编码 动作作用部位字段{};
稳定编码 动作作用对象字段{};
稳定编码 动作本能方法字段{};

struct 待形成发生 final {
    新因果状态模式 结果状态;
    std::vector<新因果状态模式> 条件状态组;
    std::vector<新因果动态模式> 条件动态组;
    std::optional<新因果动作定义> 动作;
    稳定编码 发生身份;
    新因果形成来源 来源 = 新因果形成来源::结果状态筛选;
};

std::vector<待形成发生> 待形成发生组;

std::vector<稳定编码*> 架构编码组() {
    return {
        &新因果信息图根节点, &节点类型字段, &因果图根类型,
        &因果节点类型, &状态模式节点类型, &动态模式节点类型,
        &动作定义节点类型, &因果类型字段, &因果条件状态字段,
        &因果条件动态字段, &因果动作字段, &因果结果状态字段,
        &因果形成来源字段, &因果符合次数字段, &因果不符合次数字段,
        &因果自我尝试次数字段, &因果自我尝试符合次数字段,
        &因果已计数发生字段, &状态模式存在字段,
        &状态模式特征类型字段, &状态模式准确值字段,
        &动态模式主题字段, &动态模式概念字段, &动作概念字段,
        &动作执行主体字段, &动作作用部位字段, &动作作用对象字段,
        &动作本能方法字段};
}

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已分配() noexcept {
    const auto 组 = 架构编码组();
    return std::any_of(组.begin(), 组.end(), [](const auto* 编码) {
        return 有效(*编码);
    });
}

bool 结构仍存在() noexcept {
    const auto 组 = 架构编码组();
    return std::all_of(组.begin(), 组.end(), [](const auto* 编码) {
        return 节点仍存在(*编码);
    });
}

bool 是因果类型值(std::int64_t 值) noexcept {
    return 值 == static_cast<std::int64_t>(新因果类型::未知动作)
        || 值 == static_cast<std::int64_t>(新因果类型::已知动作);
}

bool 是形成来源值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新因果形成来源::结果状态筛选)
        && 值 <= static_cast<std::int64_t>(新因果形成来源::外部动作动态反向);
}

struct 唯一I64字段记录 final {
    稳定编码 关系;
    std::int64_t 值 = 0;
};

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&组.front().内容);
    return 目标 && 节点仍存在(*目标)
        ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<稳定编码> 读取可选节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.empty()) return 稳定编码{};
    if (组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&组.front().内容);
    return 目标 && 节点仍存在(*目标)
        ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<唯一I64字段记录> 读取唯一I64字段记录(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    return I64 ? std::optional<唯一I64字段记录>{
        {组.front().编码, *I64}} : std::nullopt;
}

std::optional<新特征准确值> 读取唯一准确值字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    if (const auto* 目标 = std::get_if<稳定编码>(&组.front().内容)) {
        return 节点仍存在(*目标)
            ? std::optional<新特征准确值>{新特征准确值{*目标}}
            : std::nullopt;
    }
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    return I64 ? std::optional<新特征准确值>{新特征准确值{*I64}}
        : std::nullopt;
}

std::vector<稳定编码> 读取多节点字段(
    稳定编码 节点, 稳定编码 字段, bool& 完整) {
    std::vector<稳定编码> 结果;
    完整 = true;
    for (const auto& 关系 : 全局基础数据集.查询字段(节点, 字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&关系.内容);
        if (!目标 || !节点仍存在(*目标)) {
            完整 = false;
            return {};
        }
        结果.push_back(*目标);
    }
    std::sort(结果.begin(), 结果.end());
    if (std::adjacent_find(结果.begin(), 结果.end()) != 结果.end()) {
        完整 = false;
        return {};
    }
    return 结果;
}

bool 节点类型为(稳定编码 节点, 稳定编码 类型) {
    const auto 实际 = 读取唯一节点字段(节点, 节点类型字段);
    return 实际 && *实际 == 类型;
}

auto 准确值排序键(const 新特征准确值& 值) {
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        return std::tuple<std::size_t, std::uint64_t>{
            0, static_cast<std::uint64_t>(*I64) ^ (std::uint64_t{1} << 63)};
    }
    return std::tuple<std::size_t, std::uint64_t>{
        1, std::get<稳定编码>(值).值};
}

bool 状态模式小于(const 新因果状态模式& 左,
    const 新因果状态模式& 右) {
    return std::tuple{左.被描述存在节点.值, 左.特征类型概念节点.值,
            准确值排序键(左.准确值)}
        < std::tuple{右.被描述存在节点.值, 右.特征类型概念节点.值,
            准确值排序键(右.准确值)};
}

bool 动态模式小于(const 新因果动态模式& 左,
    const 新因果动态模式& 右) noexcept {
    return std::tuple{左.主题存在节点.值, 左.动态概念节点.值}
        < std::tuple{右.主题存在节点.值, 右.动态概念节点.值};
}

template<class T, class 小于>
void 排序去重(std::vector<T>& 组, 小于&& 比较) {
    std::sort(组.begin(), 组.end(), 比较);
    组.erase(std::unique(组.begin(), 组.end()), 组.end());
}

bool 状态模式有效(const 新因果状态模式& 模式) noexcept {
    if (!节点仍存在(模式.被描述存在节点)
        || !节点仍存在(模式.特征类型概念节点)) return false;
    if (const auto* 值节点 = std::get_if<稳定编码>(&模式.准确值)) {
        return 节点仍存在(*值节点);
    }
    return true;
}

bool 动态模式有效(const 新因果动态模式& 模式) noexcept {
    return 节点仍存在(模式.主题存在节点)
        && 节点仍存在(模式.动态概念节点);
}

bool 动作定义有效(const 新因果动作定义& 动作) noexcept {
    if (!节点仍存在(动作.动作概念节点)
        || !节点仍存在(动作.执行主体存在节点)) return false;
    const auto 可选有效 = [](const std::optional<稳定编码>& 节点) {
        return !节点 || 节点仍存在(*节点);
    };
    return 可选有效(动作.作用部位存在节点)
        && 可选有效(动作.作用对象存在节点)
        && 可选有效(动作.本能方法节点);
}

std::optional<新因果状态模式> 从状态形成模式(
    新_状态类& 状态服务, 稳定编码 状态节点) {
    const auto 状态 = 状态服务.获取状态(状态节点);
    if (!状态) return std::nullopt;
    return 新因果状态模式{
        状态->被描述存在节点,
        状态->特征类型概念节点,
        状态->固定准确值};
}

std::optional<新因果动态模式> 从动态形成模式(
    新_动态类& 动态服务, 稳定编码 动态节点) {
    const auto 动态 = 动态服务.获取动态(动态节点);
    if (!动态 || !动态->动态概念节点) return std::nullopt;
    return 新因果动态模式{动态->主题存在节点, *动态->动态概念节点};
}

bool 包含全部(const std::vector<新因果状态模式>& 全集,
    const std::vector<新因果状态模式>& 子集) {
    return std::all_of(子集.begin(), 子集.end(), [&](const auto& 项) {
        return std::find(全集.begin(), 全集.end(), 项) != 全集.end();
    });
}

bool 包含全部(const std::vector<新因果动态模式>& 全集,
    const std::vector<新因果动态模式>& 子集) {
    return std::all_of(子集.begin(), 子集.end(), [&](const auto& 项) {
        return std::find(全集.begin(), 全集.end(), 项) != 全集.end();
    });
}

template<class T>
std::vector<T> 求交集(const std::vector<T>& 左,
    const std::vector<T>& 右) {
    std::vector<T> 结果;
    for (const auto& 项 : 左) {
        if (std::find(右.begin(), 右.end(), 项) != 右.end()) 结果.push_back(项);
    }
    return 结果;
}

bool 同一形成分组(const 待形成发生& 左, const 待形成发生& 右) {
    return 左.结果状态 == 右.结果状态 && 左.动作 == 右.动作;
}

struct 构建记录 final {
    std::vector<稳定编码> 节点组;
    std::vector<稳定编码> 字段组;
    std::vector<稳定编码> 关系组;
};

void 回滚构建(构建记录& 记录) noexcept {
    for (auto it = 记录.关系组.rbegin(); it != 记录.关系组.rend(); ++it) {
        (void)全局基础数据集.删除关系(*it);
    }
    for (auto it = 记录.字段组.rbegin(); it != 记录.字段组.rend(); ++it) {
        (void)全局基础数据集.删除字段(*it);
    }
    for (auto it = 记录.节点组.rbegin(); it != 记录.节点组.rend(); ++it) {
        (void)全局基础数据集.删除节点(*it);
    }
}

bool 记节点字段(构建记录& 记录, 稳定编码 节点,
    稳定编码 字段, 稳定编码 目标) {
    const auto 关系 = 全局基础数据集.添加字段节点(节点, 字段, 目标);
    if (!有效(关系)) return false;
    记录.字段组.push_back(关系);
    return true;
}

bool 记I64字段(构建记录& 记录, 稳定编码 节点,
    稳定编码 字段, std::int64_t 值) {
    const auto 关系 = 全局基础数据集.添加字段值(
        节点, 字段, 基础值{值});
    if (!有效(关系)) return false;
    记录.字段组.push_back(关系);
    return true;
}

bool 记准确值字段(构建记录& 记录, 稳定编码 节点,
    稳定编码 字段, const 新特征准确值& 值) {
    稳定编码 关系{};
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        关系 = 全局基础数据集.添加字段值(节点, 字段, 基础值{*I64});
    } else {
        关系 = 全局基础数据集.添加字段节点(
            节点, 字段, std::get<稳定编码>(值));
    }
    if (!有效(关系)) return false;
    记录.字段组.push_back(关系);
    return true;
}

std::optional<稳定编码> 写状态模式(
    构建记录& 记录, const 新因果状态模式& 模式) {
    const auto 节点 = 全局基础数据集.新建节点();
    if (!有效(节点)) return std::nullopt;
    记录.节点组.push_back(节点);
    if (!记节点字段(记录, 节点, 节点类型字段, 状态模式节点类型)
        || !记节点字段(记录, 节点, 状态模式存在字段,
            模式.被描述存在节点)
        || !记节点字段(记录, 节点, 状态模式特征类型字段,
            模式.特征类型概念节点)
        || !记准确值字段(记录, 节点, 状态模式准确值字段,
            模式.准确值)) return std::nullopt;
    return 节点;
}

std::optional<稳定编码> 写动态模式(
    构建记录& 记录, const 新因果动态模式& 模式) {
    const auto 节点 = 全局基础数据集.新建节点();
    if (!有效(节点)) return std::nullopt;
    记录.节点组.push_back(节点);
    if (!记节点字段(记录, 节点, 节点类型字段, 动态模式节点类型)
        || !记节点字段(记录, 节点, 动态模式主题字段,
            模式.主题存在节点)
        || !记节点字段(记录, 节点, 动态模式概念字段,
            模式.动态概念节点)) return std::nullopt;
    return 节点;
}

std::optional<稳定编码> 写动作定义(
    构建记录& 记录, const 新因果动作定义& 动作) {
    const auto 节点 = 全局基础数据集.新建节点();
    if (!有效(节点)) return std::nullopt;
    记录.节点组.push_back(节点);
    if (!记节点字段(记录, 节点, 节点类型字段, 动作定义节点类型)
        || !记节点字段(记录, 节点, 动作概念字段, 动作.动作概念节点)
        || !记节点字段(记录, 节点, 动作执行主体字段,
            动作.执行主体存在节点)) return std::nullopt;
    if (动作.作用部位存在节点
        && !记节点字段(记录, 节点, 动作作用部位字段,
            *动作.作用部位存在节点)) return std::nullopt;
    if (动作.作用对象存在节点
        && !记节点字段(记录, 节点, 动作作用对象字段,
            *动作.作用对象存在节点)) return std::nullopt;
    if (动作.本能方法节点
        && !记节点字段(记录, 节点, 动作本能方法字段,
            *动作.本能方法节点)) return std::nullopt;
    return 节点;
}

std::optional<新因果状态模式> 读状态模式(稳定编码 节点) {
    if (!节点类型为(节点, 状态模式节点类型)) return std::nullopt;
    const auto 存在 = 读取唯一节点字段(节点, 状态模式存在字段);
    const auto 类型 = 读取唯一节点字段(节点, 状态模式特征类型字段);
    const auto 值 = 读取唯一准确值字段(节点, 状态模式准确值字段);
    if (!存在 || !类型 || !值) return std::nullopt;
    return 新因果状态模式{*存在, *类型, *值};
}

std::optional<新因果动态模式> 读动态模式(稳定编码 节点) {
    if (!节点类型为(节点, 动态模式节点类型)) return std::nullopt;
    const auto 主题 = 读取唯一节点字段(节点, 动态模式主题字段);
    const auto 概念 = 读取唯一节点字段(节点, 动态模式概念字段);
    if (!主题 || !概念) return std::nullopt;
    return 新因果动态模式{*主题, *概念};
}

std::optional<新因果动作定义> 读动作定义(稳定编码 节点) {
    if (!节点类型为(节点, 动作定义节点类型)) return std::nullopt;
    const auto 概念 = 读取唯一节点字段(节点, 动作概念字段);
    const auto 主体 = 读取唯一节点字段(节点, 动作执行主体字段);
    const auto 部位 = 读取可选节点字段(节点, 动作作用部位字段);
    const auto 对象 = 读取可选节点字段(节点, 动作作用对象字段);
    const auto 方法 = 读取可选节点字段(节点, 动作本能方法字段);
    if (!概念 || !主体 || !部位 || !对象 || !方法) return std::nullopt;
    return 新因果动作定义{
        *概念, *主体,
        有效(*部位) ? std::optional<稳定编码>{*部位} : std::nullopt,
        有效(*对象) ? std::optional<稳定编码>{*对象} : std::nullopt,
        有效(*方法) ? std::optional<稳定编码>{*方法} : std::nullopt};
}

std::vector<稳定编码> 查询全部因果节点() {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询源关系(
        新因果信息图根节点, 基础外部关系类型::父子)) {
        if (关系.角色或顺序 == 新因果根成员关系角色
            && 节点仍存在(关系.目标节点)) {
            结果.push_back(关系.目标节点);
        }
    }
    std::sort(结果.begin(), 结果.end());
    结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
    return 结果;
}

bool 在因果根下(稳定编码 节点) {
    const auto 组 = 全局基础数据集.查询目标关系(
        节点, 基础外部关系类型::父子);
    return std::count_if(组.begin(), 组.end(), [&](const auto& 关系) {
        return 关系.源节点 == 新因果信息图根节点
            && 关系.角色或顺序 == 新因果根成员关系角色;
    }) == 1;
}

std::optional<新因果信息> 读取因果信息(稳定编码 因果节点) {
    if (!节点类型为(因果节点, 因果节点类型) || !在因果根下(因果节点)) {
        return std::nullopt;
    }
    const auto 类型记录 = 读取唯一I64字段记录(因果节点, 因果类型字段);
    const auto 符合 = 读取唯一I64字段记录(因果节点, 因果符合次数字段);
    const auto 不符合 = 读取唯一I64字段记录(因果节点, 因果不符合次数字段);
    const auto 尝试 = 读取唯一I64字段记录(因果节点, 因果自我尝试次数字段);
    const auto 尝试符合 = 读取唯一I64字段记录(
        因果节点, 因果自我尝试符合次数字段);
    if (!类型记录 || !是因果类型值(类型记录->值)
        || !符合 || !不符合 || !尝试 || !尝试符合
        || 符合->值 < 2 || 不符合->值 < 0 || 尝试->值 < 0
        || 尝试符合->值 < 0 || 尝试符合->值 > 尝试->值) {
        return std::nullopt;
    }

    bool 完整 = false;
    const auto 条件状态节点组 = 读取多节点字段(
        因果节点, 因果条件状态字段, 完整);
    if (!完整) return std::nullopt;
    const auto 条件动态节点组 = 读取多节点字段(
        因果节点, 因果条件动态字段, 完整);
    if (!完整 || (条件状态节点组.empty() && 条件动态节点组.empty())) {
        return std::nullopt;
    }
    const auto 结果节点组 = 读取多节点字段(
        因果节点, 因果结果状态字段, 完整);
    if (!完整 || 结果节点组.size() != 1) return std::nullopt;
    const auto 动作节点组 = 读取多节点字段(因果节点, 因果动作字段, 完整);
    if (!完整 || 动作节点组.size() > 1) return std::nullopt;

    新因果信息 信息;
    信息.节点 = 因果节点;
    信息.定义.类型 = static_cast<新因果类型>(类型记录->值);
    for (const auto 节点 : 条件状态节点组) {
        const auto 项 = 读状态模式(节点);
        if (!项) return std::nullopt;
        信息.定义.条件状态组.push_back(*项);
    }
    for (const auto 节点 : 条件动态节点组) {
        const auto 项 = 读动态模式(节点);
        if (!项) return std::nullopt;
        信息.定义.条件动态组.push_back(*项);
    }
    const auto 结果 = 读状态模式(结果节点组.front());
    if (!结果) return std::nullopt;
    信息.定义.结果状态 = *结果;
    if (!动作节点组.empty()) {
        const auto 动作 = 读动作定义(动作节点组.front());
        if (!动作) return std::nullopt;
        信息.定义.动作 = *动作;
    }
    if ((信息.定义.类型 == 新因果类型::已知动作) != 信息.定义.动作.has_value()) {
        return std::nullopt;
    }

    for (const auto& 字段 : 全局基础数据集.查询字段(
        因果节点, 因果形成来源字段)) {
        const auto* 值 = std::get_if<基础值>(&字段.内容);
        const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
        if (!I64 || !是形成来源值(*I64)) return std::nullopt;
        信息.形成来源组.push_back(static_cast<新因果形成来源>(*I64));
    }
    std::sort(信息.形成来源组.begin(), 信息.形成来源组.end());
    if (信息.形成来源组.empty()
        || std::adjacent_find(信息.形成来源组.begin(),
            信息.形成来源组.end()) != 信息.形成来源组.end()) {
        return std::nullopt;
    }

    const auto 最大计数 = (std::numeric_limits<std::int64_t>::max)();
    if (符合->值 > 最大计数 - 不符合->值) return std::nullopt;
    信息.统计 = {符合->值, 不符合->值, 尝试->值, 尝试符合->值};
    信息.已计数发生身份组 = 读取多节点字段(
        因果节点, 因果已计数发生字段, 完整);
    if (!完整 || 信息.已计数发生身份组.size()
        != static_cast<std::size_t>(符合->值 + 不符合->值)) {
        return std::nullopt;
    }
    排序去重(信息.定义.条件状态组, 状态模式小于);
    排序去重(信息.定义.条件动态组, 动态模式小于);
    return 信息;
}

std::optional<新因果信息> 发布因果(
    const 新因果定义& 定义,
    std::vector<新因果形成来源> 来源组,
    std::vector<稳定编码> 发生身份组) {
    构建记录 记录;
    const auto 因果节点 = 全局基础数据集.新建节点();
    if (!有效(因果节点)) return std::nullopt;
    记录.节点组.push_back(因果节点);
    if (!记节点字段(记录, 因果节点, 节点类型字段, 因果节点类型)
        || !记I64字段(记录, 因果节点, 因果类型字段,
            static_cast<std::int64_t>(定义.类型))) {
        回滚构建(记录);
        return std::nullopt;
    }
    for (const auto& 模式 : 定义.条件状态组) {
        const auto 项 = 写状态模式(记录, 模式);
        if (!项 || !记节点字段(记录, 因果节点, 因果条件状态字段, *项)) {
            回滚构建(记录);
            return std::nullopt;
        }
    }
    for (const auto& 模式 : 定义.条件动态组) {
        const auto 项 = 写动态模式(记录, 模式);
        if (!项 || !记节点字段(记录, 因果节点, 因果条件动态字段, *项)) {
            回滚构建(记录);
            return std::nullopt;
        }
    }
    if (定义.动作) {
        const auto 动作 = 写动作定义(记录, *定义.动作);
        if (!动作 || !记节点字段(记录, 因果节点, 因果动作字段, *动作)) {
            回滚构建(记录);
            return std::nullopt;
        }
    }
    const auto 结果 = 写状态模式(记录, 定义.结果状态);
    if (!结果 || !记节点字段(
        记录, 因果节点, 因果结果状态字段, *结果)) {
        回滚构建(记录);
        return std::nullopt;
    }

    std::sort(来源组.begin(), 来源组.end());
    来源组.erase(std::unique(来源组.begin(), 来源组.end()), 来源组.end());
    for (const auto 来源 : 来源组) {
        if (!记I64字段(记录, 因果节点, 因果形成来源字段,
            static_cast<std::int64_t>(来源))) {
            回滚构建(记录);
            return std::nullopt;
        }
    }
    std::sort(发生身份组.begin(), 发生身份组.end());
    发生身份组.erase(std::unique(发生身份组.begin(), 发生身份组.end()),
        发生身份组.end());
    if (发生身份组.size() != 2) {
        回滚构建(记录);
        return std::nullopt;
    }
    for (const auto 身份 : 发生身份组) {
        if (!记节点字段(记录, 因果节点, 因果已计数发生字段, 身份)) {
            回滚构建(记录);
            return std::nullopt;
        }
    }
    if (!记I64字段(记录, 因果节点, 因果符合次数字段, 2)
        || !记I64字段(记录, 因果节点, 因果不符合次数字段, 0)
        || !记I64字段(记录, 因果节点, 因果自我尝试次数字段, 0)
        || !记I64字段(记录, 因果节点,
            因果自我尝试符合次数字段, 0)) {
        回滚构建(记录);
        return std::nullopt;
    }
    const auto 根关系 = 全局基础数据集.添加关系(
        新因果信息图根节点, 因果节点, 基础外部关系类型::父子,
        新因果根成员关系角色);
    if (!有效(根关系)) {
        回滚构建(记录);
        return std::nullopt;
    }
    记录.关系组.push_back(根关系);
    const auto 读回 = 读取因果信息(因果节点);
    if (!读回) {
        回滚构建(记录);
        return std::nullopt;
    }
    return 读回;
}

bool 定义匹配发生(const 新因果定义& 定义,
    const 待形成发生& 发生) {
    if (定义.结果状态 != 发生.结果状态 || 定义.动作 != 发生.动作) {
        return false;
    }
    return 包含全部(发生.条件状态组, 定义.条件状态组)
        && 包含全部(发生.条件动态组, 定义.条件动态组);
}

std::optional<新因果信息> 查找最具体匹配因果(const 待形成发生& 发生) {
    std::optional<新因果信息> 最佳;
    std::size_t 最佳条件数 = 0;
    for (const auto 节点 : 查询全部因果节点()) {
        const auto 信息 = 读取因果信息(节点);
        if (!信息 || !定义匹配发生(信息->定义, 发生)) continue;
        const auto 条件数 = 信息->定义.条件状态组.size()
            + 信息->定义.条件动态组.size();
        if (!最佳 || 条件数 > 最佳条件数
            || (条件数 == 最佳条件数 && 信息->节点 < 最佳->节点)) {
            最佳 = 信息;
            最佳条件数 = 条件数;
        }
    }
    return 最佳;
}

新因果操作状态 更新普通统计(
    稳定编码 因果节点, 稳定编码 发生身份, bool 符合,
    std::optional<新因果形成来源> 新来源 = std::nullopt) {
    const auto 信息 = 读取因果信息(因果节点);
    if (!信息) return 新因果操作状态::因果不存在;
    if (!节点仍存在(发生身份)) return 新因果操作状态::入口拒绝;
    if (std::find(信息->已计数发生身份组.begin(),
        信息->已计数发生身份组.end(), 发生身份)
        != 信息->已计数发生身份组.end()) {
        return 新因果操作状态::重复发生;
    }
    const auto& 当前字段 = 符合 ? 因果符合次数字段 : 因果不符合次数字段;
    const auto 当前 = 读取唯一I64字段记录(因果节点, 当前字段);
    if (!当前 || 当前->值 == (std::numeric_limits<std::int64_t>::max)()) {
        return 当前 ? 新因果操作状态::计数溢出
            : 新因果操作状态::结构不一致;
    }
    std::vector<稳定编码> 已加字段;
    const auto 发生字段 = 全局基础数据集.添加字段节点(
        因果节点, 因果已计数发生字段, 发生身份);
    if (!有效(发生字段)) return 新因果操作状态::资源失败;
    已加字段.push_back(发生字段);
    if (新来源 && std::find(信息->形成来源组.begin(),
        信息->形成来源组.end(), *新来源) == 信息->形成来源组.end()) {
        const auto 来源字段 = 全局基础数据集.添加字段值(
            因果节点, 因果形成来源字段,
            基础值{static_cast<std::int64_t>(*新来源)});
        if (!有效(来源字段)) {
            (void)全局基础数据集.删除字段(发生字段);
            return 新因果操作状态::资源失败;
        }
        已加字段.push_back(来源字段);
    }
    if (!全局基础数据集.修改字段值(
        当前->关系, 基础值{当前->值 + 1})) {
        for (auto it = 已加字段.rbegin(); it != 已加字段.rend(); ++it) {
            (void)全局基础数据集.删除字段(*it);
        }
        return 新因果操作状态::资源失败;
    }
    return 读取因果信息(因果节点)
        ? 新因果操作状态::已更新 : 新因果操作状态::结构不一致;
}

std::optional<待形成发生> 规范化发生(
    新_状态类& 状态服务,
    新_动态类& 动态服务,
    const 新因果发生记录& 输入,
    新因果形成来源 来源,
    const std::optional<新因果动作定义>& 动作,
    新因果操作状态& 失败状态) {
    失败状态 = 新因果操作状态::入口拒绝;
    if (!节点仍存在(输入.发生身份) || !节点仍存在(输入.场景节点)) {
        return std::nullopt;
    }
    const auto 结果状态 = 从状态形成模式(状态服务, 输入.结果状态节点);
    if (!结果状态) {
        失败状态 = 新因果操作状态::状态不存在;
        return std::nullopt;
    }
    if (来源 == 新因果形成来源::结果动态反向) {
        if (!输入.结果动态节点) {
            失败状态 = 新因果操作状态::动态不存在;
            return std::nullopt;
        }
        const auto 结果动态 = 动态服务.获取动态(*输入.结果动态节点);
        if (!结果动态
            || std::find(结果动态->状态节点组.begin(),
                结果动态->状态节点组.end(), 输入.结果状态节点)
                == 结果动态->状态节点组.end()) {
            失败状态 = 新因果操作状态::动态不存在;
            return std::nullopt;
        }
    }
    if (动作 && !动作定义有效(*动作)) {
        失败状态 = 新因果操作状态::动作证据不存在;
        return std::nullopt;
    }

    待形成发生 发生;
    发生.结果状态 = *结果状态;
    发生.动作 = 动作;
    发生.发生身份 = 输入.发生身份;
    发生.来源 = 来源;
    for (const auto 节点 : 输入.前置状态节点组) {
        if (节点 == 输入.结果状态节点) continue;
        const auto 模式 = 从状态形成模式(状态服务, 节点);
        if (!模式) {
            失败状态 = 新因果操作状态::状态不存在;
            return std::nullopt;
        }
        发生.条件状态组.push_back(*模式);
    }
    for (const auto 节点 : 输入.前置动态节点组) {
        if (输入.结果动态节点 && 节点 == *输入.结果动态节点) continue;
        const auto 模式 = 从动态形成模式(动态服务, 节点);
        if (!模式) {
            失败状态 = 新因果操作状态::动态不存在;
            return std::nullopt;
        }
        发生.条件动态组.push_back(*模式);
    }
    排序去重(发生.条件状态组, 状态模式小于);
    排序去重(发生.条件动态组, 动态模式小于);
    if (发生.条件状态组.empty() && 发生.条件动态组.empty()) {
        失败状态 = 新因果操作状态::无共同条件;
        return std::nullopt;
    }
    return 发生;
}

} // namespace

新_因果类::新_因果类(
    新_状态类& 状态服务,
    新_动态类& 动态服务) noexcept
    : 状态服务_(状态服务), 动态服务_(动态服务) {}

新_因果类::新_因果类(
    新_状态类& 状态服务,
    新_动态类& 动态服务,
    const 新因果动作证据提供者& 动作证据提供者) noexcept
    : 状态服务_(状态服务), 动态服务_(动态服务),
      动作证据提供者_(&动作证据提供者) {}

bool 新_因果类::初始化() noexcept {
    try {
        std::lock_guard 锁(新因果互斥);
        if (!状态服务_.初始化() || !动态服务_.初始化()) return false;
        if (结构已分配()) return 结构仍存在();
        for (auto* 编码 : 架构编码组()) {
            *编码 = 全局基础数据集.新建节点();
            if (!有效(*编码)) return false;
        }
        return 有效(全局基础数据集.添加字段节点(
            新因果信息图根节点, 节点类型字段, 因果图根类型));
    } catch (...) {
        return false;
    }
}

新因果处理结果 新_因果类::处理发生(
    const 新因果发生记录& 输入,
    新因果形成来源 来源,
    const std::optional<新因果动作定义>& 动作) noexcept {
    新因果处理结果 结果;
    try {
        std::lock_guard 锁(新因果互斥);
        if (!结构仍存在()) {
            结果.状态 = 新因果操作状态::尚未初始化;
            return 结果;
        }
        新因果操作状态 失败状态{};
        const auto 发生 = 规范化发生(
            状态服务_, 动态服务_, 输入, 来源, 动作, 失败状态);
        if (!发生) {
            结果.状态 = 失败状态;
            return 结果;
        }

        待形成发生组.erase(std::remove_if(
            待形成发生组.begin(), 待形成发生组.end(),
            [](const auto& 项) { return !节点仍存在(项.发生身份); }),
            待形成发生组.end());

        if (const auto 已有 = 查找最具体匹配因果(*发生)) {
            const auto 更新 = 更新普通统计(
                已有->节点, 发生->发生身份, true, 来源);
            结果.状态 = 更新;
            结果.因果节点 = 已有->节点;
            结果.因果信息 = 读取因果信息(已有->节点);
            return 结果;
        }

        if (std::any_of(待形成发生组.begin(), 待形成发生组.end(),
            [&](const auto& 项) { return 项.发生身份 == 发生->发生身份; })) {
            结果.状态 = 新因果操作状态::重复发生;
            return 结果;
        }

        bool 找到同组但无共同条件 = false;
        for (auto it = 待形成发生组.begin(); it != 待形成发生组.end(); ++it) {
            if (!同一形成分组(*it, *发生)) continue;
            auto 共同状态 = 求交集(it->条件状态组, 发生->条件状态组);
            auto 共同动态 = 求交集(it->条件动态组, 发生->条件动态组);
            if (共同状态.empty() && 共同动态.empty()) {
                找到同组但无共同条件 = true;
                continue;
            }
            新因果定义 定义;
            定义.类型 = 动作 ? 新因果类型::已知动作 : 新因果类型::未知动作;
            定义.条件状态组 = std::move(共同状态);
            定义.条件动态组 = std::move(共同动态);
            定义.动作 = 动作;
            定义.结果状态 = 发生->结果状态;
            const auto 已建立 = 发布因果(
                定义, {it->来源, 来源}, {it->发生身份, 发生->发生身份});
            if (!已建立) {
                结果.状态 = 新因果操作状态::资源失败;
                return 结果;
            }
            const auto 旧身份 = it->发生身份;
            待形成发生组.erase(std::remove_if(
                待形成发生组.begin(), 待形成发生组.end(),
                [&](const auto& 项) {
                    return 项.发生身份 == 旧身份
                        || 项.发生身份 == 发生->发生身份;
                }), 待形成发生组.end());
            结果.状态 = 新因果操作状态::已建立;
            结果.因果节点 = 已建立->节点;
            结果.因果信息 = *已建立;
            return 结果;
        }
        待形成发生组.push_back(*发生);
        结果.状态 = 找到同组但无共同条件
            ? 新因果操作状态::无共同条件
            : 新因果操作状态::已记录首例;
        return 结果;
    } catch (...) {
        结果.状态 = 新因果操作状态::资源失败;
        return 结果;
    }
}

新因果处理结果 新_因果类::处理结果状态(
    const 新因果发生记录& 发生) noexcept {
    return 处理发生(发生, 新因果形成来源::结果状态筛选, std::nullopt);
}

新因果处理结果 新_因果类::处理结果动态(
    const 新因果发生记录& 发生) noexcept {
    return 处理发生(发生, 新因果形成来源::结果动态反向, std::nullopt);
}

新因果处理结果 新_因果类::处理动作事实(
    const 新因果发生记录& 发生) noexcept {
    if (!动作证据提供者_) {
        return 新因果处理结果{新因果操作状态::依赖未配置};
    }
    if (!发生.动作证据节点 || !节点仍存在(*发生.动作证据节点)) {
        return 新因果处理结果{新因果操作状态::动作证据不存在};
    }
    const auto 动作 = 动作证据提供者_->读取自我动作事实(*发生.动作证据节点);
    if (!动作) return 新因果处理结果{新因果操作状态::动作证据不存在};
    return 处理发生(发生, 新因果形成来源::动作事实反向, 动作);
}

新因果处理结果 新_因果类::处理外部动作动态(
    const 新因果发生记录& 发生) noexcept {
    if (!动作证据提供者_) {
        return 新因果处理结果{新因果操作状态::依赖未配置};
    }
    if (!发生.动作证据节点 || !节点仍存在(*发生.动作证据节点)) {
        return 新因果处理结果{新因果操作状态::动作证据不存在};
    }
    const auto 动作 = 动作证据提供者_->推断外部动作(*发生.动作证据节点);
    if (!动作) return 新因果处理结果{新因果操作状态::动作证据不存在};
    return 处理发生(发生, 新因果形成来源::外部动作动态反向, 动作);
}

std::optional<新因果信息> 新_因果类::获取因果(
    稳定编码 因果节点) const noexcept {
    try {
        std::lock_guard 锁(新因果互斥);
        if (!结构仍存在()) return std::nullopt;
        return 读取因果信息(因果节点);
    } catch (...) {
        return std::nullopt;
    }
}

bool 新_因果类::是因果节点(稳定编码 节点) const noexcept {
    return 获取因果(节点).has_value();
}

新因果查询结果 新_因果类::根据结果状态查询(
    const 新因果状态模式& 结果状态) const noexcept {
    新因果查询结果 结果;
    try {
        std::lock_guard 锁(新因果互斥);
        if (!结构仍存在()) {
            结果.状态 = 新因果操作状态::尚未初始化;
            return 结果;
        }
        if (!状态模式有效(结果状态)) return 结果;
        for (const auto 节点 : 查询全部因果节点()) {
            const auto 信息 = 读取因果信息(节点);
            if (!信息) {
                结果.状态 = 新因果操作状态::结构不一致;
                结果.因果节点组.clear();
                return 结果;
            }
            if (信息->定义.结果状态 == 结果状态) 结果.因果节点组.push_back(节点);
        }
        结果.状态 = 结果.因果节点组.empty()
            ? 新因果操作状态::无匹配 : 新因果操作状态::已找到;
        return 结果;
    } catch (...) {
        结果.状态 = 新因果操作状态::资源失败;
        结果.因果节点组.clear();
        return 结果;
    }
}

新因果查询结果 新_因果类::根据条件状态查询(
    const 新因果状态模式& 条件状态) const noexcept {
    新因果查询结果 结果;
    try {
        std::lock_guard 锁(新因果互斥);
        if (!结构仍存在()) {
            结果.状态 = 新因果操作状态::尚未初始化;
            return 结果;
        }
        if (!状态模式有效(条件状态)) return 结果;
        for (const auto 节点 : 查询全部因果节点()) {
            const auto 信息 = 读取因果信息(节点);
            if (!信息) {
                结果.状态 = 新因果操作状态::结构不一致;
                结果.因果节点组.clear();
                return 结果;
            }
            if (std::find(信息->定义.条件状态组.begin(),
                信息->定义.条件状态组.end(), 条件状态)
                != 信息->定义.条件状态组.end()) {
                结果.因果节点组.push_back(节点);
            }
        }
        结果.状态 = 结果.因果节点组.empty()
            ? 新因果操作状态::无匹配 : 新因果操作状态::已找到;
        return 结果;
    } catch (...) {
        结果.状态 = 新因果操作状态::资源失败;
        结果.因果节点组.clear();
        return 结果;
    }
}

新因果查询结果 新_因果类::根据动作概念查询(
    稳定编码 动作概念节点) const noexcept {
    新因果查询结果 结果;
    try {
        std::lock_guard 锁(新因果互斥);
        if (!结构仍存在()) {
            结果.状态 = 新因果操作状态::尚未初始化;
            return 结果;
        }
        if (!节点仍存在(动作概念节点)) return 结果;
        for (const auto 节点 : 查询全部因果节点()) {
            const auto 信息 = 读取因果信息(节点);
            if (!信息) {
                结果.状态 = 新因果操作状态::结构不一致;
                结果.因果节点组.clear();
                return 结果;
            }
            if (信息->定义.动作
                && 信息->定义.动作->动作概念节点 == 动作概念节点) {
                结果.因果节点组.push_back(节点);
            }
        }
        结果.状态 = 结果.因果节点组.empty()
            ? 新因果操作状态::无匹配 : 新因果操作状态::已找到;
        return 结果;
    } catch (...) {
        结果.状态 = 新因果操作状态::资源失败;
        结果.因果节点组.clear();
        return 结果;
    }
}

std::vector<新因果方法参考> 新_因果类::为目标状态查询方法参考(
    const 新因果状态模式& 目标状态) const noexcept {
    try {
        std::lock_guard 锁(新因果互斥);
        std::vector<新因果方法参考> 结果;
        if (!结构仍存在() || !状态模式有效(目标状态)) return 结果;
        for (const auto 节点 : 查询全部因果节点()) {
            const auto 信息 = 读取因果信息(节点);
            if (!信息 || 信息->定义.结果状态 != 目标状态
                || !信息->定义.动作 || !信息->定义.动作->本能方法节点) {
                continue;
            }
            结果.push_back({节点, *信息->定义.动作->本能方法节点,
                *信息->定义.动作, 信息->统计});
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
            const auto 左净符合 = 左.统计.符合出现次数 - 左.统计.不符合出现次数;
            const auto 右净符合 = 右.统计.符合出现次数 - 右.统计.不符合出现次数;
            if (左净符合 != 右净符合) return 左净符合 > 右净符合;
            return 左.因果节点 < 右.因果节点;
        });
        return 结果;
    } catch (...) {
        return {};
    }
}

新因果操作状态 新_因果类::登记因果发生结果(
    稳定编码 因果节点,
    稳定编码 发生身份,
    bool 符合) noexcept {
    try {
        std::lock_guard 锁(新因果互斥);
        if (!结构仍存在()) return 新因果操作状态::尚未初始化;
        return 更新普通统计(因果节点, 发生身份, 符合);
    } catch (...) {
        return 新因果操作状态::资源失败;
    }
}

新因果操作状态 新_因果类::登记方法执行结果(
    稳定编码 因果节点,
    稳定编码 发生身份,
    bool 符合) noexcept {
    try {
        std::lock_guard 锁(新因果互斥);
        if (!结构仍存在()) return 新因果操作状态::尚未初始化;
        const auto 信息 = 读取因果信息(因果节点);
        if (!信息) return 新因果操作状态::因果不存在;
        if (!信息->定义.动作 || !信息->定义.动作->本能方法节点) {
            return 新因果操作状态::依赖未配置;
        }
        if (!节点仍存在(发生身份)) return 新因果操作状态::入口拒绝;
        if (std::find(信息->已计数发生身份组.begin(),
            信息->已计数发生身份组.end(), 发生身份)
            != 信息->已计数发生身份组.end()) {
            return 新因果操作状态::重复发生;
        }
        const auto 尝试 = 读取唯一I64字段记录(
            因果节点, 因果自我尝试次数字段);
        const auto 尝试符合 = 读取唯一I64字段记录(
            因果节点, 因果自我尝试符合次数字段);
        const auto 普通计数 = 读取唯一I64字段记录(
            因果节点, 符合 ? 因果符合次数字段 : 因果不符合次数字段);
        const auto 最大 = (std::numeric_limits<std::int64_t>::max)();
        if (!尝试 || !尝试符合 || !普通计数) {
            return 新因果操作状态::结构不一致;
        }
        if (尝试->值 == 最大 || 普通计数->值 == 最大
            || (符合 && 尝试符合->值 == 最大)) {
            return 新因果操作状态::计数溢出;
        }

        const auto 发生字段 = 全局基础数据集.添加字段节点(
            因果节点, 因果已计数发生字段, 发生身份);
        if (!有效(发生字段)) return 新因果操作状态::资源失败;
        if (!全局基础数据集.修改字段值(
            尝试->关系, 基础值{尝试->值 + 1})) {
            (void)全局基础数据集.删除字段(发生字段);
            return 新因果操作状态::资源失败;
        }
        if (符合 && !全局基础数据集.修改字段值(
            尝试符合->关系, 基础值{尝试符合->值 + 1})) {
            (void)全局基础数据集.修改字段值(
                尝试->关系, 基础值{尝试->值});
            (void)全局基础数据集.删除字段(发生字段);
            return 新因果操作状态::资源失败;
        }
        if (!全局基础数据集.修改字段值(
            普通计数->关系, 基础值{普通计数->值 + 1})) {
            if (符合) (void)全局基础数据集.修改字段值(
                尝试符合->关系, 基础值{尝试符合->值});
            (void)全局基础数据集.修改字段值(
                尝试->关系, 基础值{尝试->值});
            (void)全局基础数据集.删除字段(发生字段);
            return 新因果操作状态::资源失败;
        }
        return 读取因果信息(因果节点)
            ? 新因果操作状态::已更新 : 新因果操作状态::结构不一致;
    } catch (...) {
        return 新因果操作状态::资源失败;
    }
}

} // namespace 海中鱼巣
