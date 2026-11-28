#include "世界挂接.全局根.h"

#include "../核心/基础数据集.持久运行包.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <mutex>
#include <new>
#include <stdexcept>
#include <string_view>

namespace 海中鱼巣 {

稳定编码 全局根场景节点{};

namespace {

// 角色值与场景组成（3）、存在组成（2）等既有父子角色互不相同，
// 使场景与存在的父链过滤器不会把挂接边误读为组成或层级关系。
constexpr std::int64_t 世界类型登记关系角色 = 9101;
constexpr std::int64_t 世界根挂接关系角色 = 9102;
constexpr std::size_t 世界类型数 = 5;
constexpr std::array<std::string_view, 4> 恢复标记组{
    "HY-Ego/4202/global-root-type-field/v1",
    "HY-Ego/4202/global-root-type-value/v1",
    "HY-Ego/4202/world-type-registration-field/v1",
    "HY-Ego/4202/world-owner-type-field/v1"
};

std::mutex 全局根互斥;
// 私有结构节点：只作字段键或类型值使用，不承载事实。
稳定编码 全局根类型字段{};
稳定编码 全局根类型{};
稳定编码 世界类型登记键字段{};
稳定编码 所属世界类型字段{};
std::array<稳定编码, 世界类型数> 世界类型节点{};

std::array<稳定编码, 恢复标记组.size()> 当前恢复标记键() noexcept {
    return {全局根类型字段, 全局根类型, 世界类型登记键字段, 所属世界类型字段};
}

bool 是恢复标记(const 基础字段关系& 字段, 稳定编码 节点, std::string_view 标记) noexcept {
    const auto* 值 = std::get_if<基础值>(&字段.内容);
    const auto* 字符串 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
    return 字段.所属节点 == 节点 && 字段.字段节点 == 节点
        && 字符串 && *字符串 == 标记;
}

std::optional<std::size_t> 键序号(世界类型登记键 键) noexcept {
    const auto 值 = static_cast<std::int64_t>(键);
    if (值 < 1 || 值 > static_cast<std::int64_t>(世界类型数)) return std::nullopt;
    return static_cast<std::size_t>(值 - 1);
}

bool 节点存在(稳定编码 节点) {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

std::optional<稳定编码> 唯一引用字段(稳定编码 节点, 稳定编码 字段, bool& 多项) {
    多项 = false;
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.empty()) return std::nullopt;
    if (组.size() != 1) { 多项 = true; return std::nullopt; }
    const auto* 目标 = std::get_if<稳定编码>(&组.front().内容);
    if (!目标 || !节点存在(*目标)) { 多项 = true; return std::nullopt; }
    return *目标;
}

std::optional<std::int64_t> 唯一整数字段(const 基础数据集& 数据, 稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 数据.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* 数 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    return 数 ? std::optional<std::int64_t>{*数} : std::nullopt;
}

std::vector<基础外部关系> 角色出边(稳定编码 节点, std::int64_t 角色) {
    auto 组 = 全局基础数据集.查询源关系(节点, 基础外部关系类型::父子);
    std::erase_if(组, [&](const 基础外部关系& 边) { return 边.角色或顺序 != 角色; });
    return 组;
}

// 调用者持 全局根互斥。只核结构，不写入。
bool 全局根结构完整已加锁() {
    if (!节点存在(全局根场景节点) || !节点存在(全局根类型字段) || !节点存在(全局根类型)
        || !节点存在(世界类型登记键字段) || !节点存在(所属世界类型字段)) return false;
    const auto 标记键 = 当前恢复标记键();
    for (std::size_t i = 0; i < 标记键.size(); ++i) {
        const auto 字段 = 全局基础数据集.查询字段(标记键[i], 标记键[i]);
        if (字段.size() != 1 || !是恢复标记(字段.front(), 标记键[i], 恢复标记组[i]))
            return false;
    }
    bool 多项 = false;
    const auto 类型 = 唯一引用字段(全局根场景节点, 全局根类型字段, 多项);
    if (!类型 || *类型 != 全局根类型) return false;
    for (const auto& 边 : 全局基础数据集.查询源关系(全局根场景节点, 基础外部关系类型::兄弟))
        if (有效(边.编码)) return false;
    for (const auto& 边 : 全局基础数据集.查询源关系(全局根场景节点, 基础外部关系类型::父子))
        if (边.角色或顺序 != 世界类型登记关系角色 && 边.角色或顺序 != 世界根挂接关系角色) return false;
    const auto 登记边 = 角色出边(全局根场景节点, 世界类型登记关系角色);
    if (登记边.size() != 世界类型数) return false;
    for (std::size_t i = 0; i < 世界类型数; ++i) {
        const auto 节点 = 世界类型节点[i];
        if (!节点存在(节点)
            || 唯一整数字段(全局基础数据集, 节点, 世界类型登记键字段) != static_cast<std::int64_t>(i + 1))
            return false;
        if (std::count_if(登记边.begin(), 登记边.end(),
                [&](const 基础外部关系& 边) { return 边.目标节点 == 节点; }) != 1)
            return false;
    }
    return true;
}

// 调用者持 全局根互斥。读出全部挂接；发现同类型多根、类型字段缺失或错挂返回 false。
bool 读取挂接已加锁(std::vector<世界根挂接信息>& 输出) {
    输出.clear();
    std::array<bool, 世界类型数> 已占{};
    for (const auto& 边 : 角色出边(全局根场景节点, 世界根挂接关系角色)) {
        if (!节点存在(边.目标节点)) return false;
        bool 多项 = false;
        const auto 类型 = 唯一引用字段(边.目标节点, 所属世界类型字段, 多项);
        if (!类型) return false;
        const auto 位置 = std::find(世界类型节点.begin(), 世界类型节点.end(), *类型);
        if (位置 == 世界类型节点.end()) return false;
        const auto 序号 = static_cast<std::size_t>(位置 - 世界类型节点.begin());
        if (已占[序号]) return false;
        已占[序号] = true;
        输出.push_back({*类型, 边.目标节点, static_cast<std::int64_t>(序号 + 1)});
    }
    std::sort(输出.begin(), 输出.end(),
        [](const 世界根挂接信息& 左, const 世界根挂接信息& 右) { return 左.登记键 < 右.登记键; });
    return true;
}

全局根操作状态 映射追加状态(基础节点组追加状态 状态) noexcept {
    switch (状态) {
    case 基础节点组追加状态::已完成: return 全局根操作状态::已完成;
    case 基础节点组追加状态::资源失败: return 全局根操作状态::资源失败;
    default: return 全局根操作状态::结构不一致;
    }
}

[[noreturn]] void 全局根工程失败(const wchar_t* 入口, const wchar_t* 原因) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, 入口, 原因);
}

} // namespace

全局根操作状态 新_全局根类::初始化() noexcept {
    try {
        std::lock_guard 锁(全局根互斥);
        if (const auto 会话 = 基础持久运行包::读取已绑定会话(全局基础数据集)) {
            const auto 视图 = 会话->读取();
            if (!视图) return 全局根操作状态::结构不一致;
            const auto 读回 = 读取恢复全局根(*视图);
            return 读回.状态 == 全局根操作状态::已完成 ? 全局根操作状态::无变化 : 读回.状态;
        }
        if (有效(全局根场景节点))
            return 全局根结构完整已加锁() ? 全局根操作状态::无变化 : 全局根操作状态::结构不一致;
        for (auto* 键 : {&全局根类型字段, &全局根类型, &世界类型登记键字段, &所属世界类型字段}) {
            if (!有效(*键)) *键 = 全局基础数据集.新建节点();
            if (!有效(*键)) return 全局根操作状态::资源失败;
        }
        基础节点组追加请求 请求;
        请求.新节点组.push_back({{{全局根类型字段, 基础组内字段内容{全局根类型}}}});
        for (std::size_t i = 0; i < 世界类型数; ++i) {
            请求.新节点组.push_back({{{世界类型登记键字段,
                基础组内字段内容{基础值{static_cast<std::int64_t>(i + 1)}}}}});
            请求.追加外部关系组.push_back({基础组内节点引用{0}, 基础组内节点引用{i + 1},
                基础外部关系类型::父子, 世界类型登记关系角色});
        }
        const auto 标记键 = 当前恢复标记键();
        const auto 键结构 = 全局基础数据集.查询节点组完整结构(
            std::vector<稳定编码>(标记键.begin(), 标记键.end()));
        if (键结构.状态 == 基础结构操作状态::资源失败)
            return 全局根操作状态::资源失败;
        if (键结构.状态 != 基础结构操作状态::已完成 || 键结构.节点组.size() != 标记键.size())
            return 全局根操作状态::结构不一致;
        for (const auto& 项 : 键结构.节点组) {
            if (!项.快照) return 全局根操作状态::结构不一致;
            请求.预期联合节点组.push_back(*项.快照);
        }
        for (std::size_t i = 0; i < 标记键.size(); ++i) {
            const auto 键 = 标记键[i];
            请求.只读预期.节点组.push_back({键});
            请求.只读预期.字段集合组.push_back({键, 键, {}, {}});
            请求.追加字段组.push_back({键, 键, 基础值{std::string(恢复标记组[i])}});
        }
        const auto 结果 = 全局基础数据集.原子追加节点组(请求);
        if (结果.状态 != 基础节点组追加状态::已完成) return 映射追加状态(结果.状态);
        if (结果.新节点组.size() != 世界类型数 + 1)
            全局根工程失败(L"全局根初始化", L"原子追加回执节点数与请求不符。");
        全局根场景节点 = 结果.新节点组[0];
        for (std::size_t i = 0; i < 世界类型数; ++i) 世界类型节点[i] = 结果.新节点组[i + 1];
        if (!全局根结构完整已加锁())
            全局根工程失败(L"全局根初始化", L"全局根与世界类型登记发布后读回不符。");
        return 全局根操作状态::已完成;
    } catch (const std::bad_alloc&) {
        return 全局根操作状态::资源失败;
    } catch (...) {
        全局根工程失败(L"全局根初始化", L"全局根初始化发生未预期工程异常。");
    }
}

全局根操作状态 新_全局根类::挂接世界根(世界类型登记键 键, 稳定编码 世界根) noexcept {
    try {
        const auto 序号 = 键序号(键);
        std::lock_guard 锁(全局根互斥);
        if (const auto 会话 = 基础持久运行包::读取已绑定会话(全局基础数据集)) {
            if (!序号 || !有效(世界根)) return 全局根操作状态::入口拒绝;
            const auto 视图 = 会话->读取();
            if (!视图) return 全局根操作状态::结构不一致;
            if (!视图->结构().查询节点(世界根)) return 全局根操作状态::入口拒绝;
            const auto 读回 = 读取恢复全局根(*视图);
            if (读回.状态 != 全局根操作状态::已完成) return 读回.状态;
            for (const auto& 项 : 读回.挂接组) {
                if (项.登记键 == static_cast<std::int64_t>(键))
                    return 项.世界根节点 == 世界根 ? 全局根操作状态::无变化 : 全局根操作状态::结构不一致;
                if (项.世界根节点 == 世界根) return 全局根操作状态::结构不一致;
            }
            return 全局根操作状态::入口拒绝;
        }
        if (!序号 || !有效(全局根场景节点) || !节点存在(世界根))
            return 全局根操作状态::入口拒绝;
        if (!全局根结构完整已加锁()) return 全局根操作状态::结构不一致;
        const auto 类型节点 = 世界类型节点[*序号];
        std::vector<世界根挂接信息> 现有;
        if (!读取挂接已加锁(现有)) return 全局根操作状态::结构不一致;
        for (const auto& 项 : 现有) {
            if (项.世界根节点 == 世界根)
                return 项.世界类型节点 == 类型节点
                    ? 全局根操作状态::无变化 : 全局根操作状态::结构不一致;
            if (项.世界类型节点 == 类型节点) return 全局根操作状态::结构不一致;
        }
        // 未挂接的根不得已带所属世界类型字段（否则是半结构或被其它路径写入）。
        if (!全局基础数据集.查询字段(世界根, 所属世界类型字段).empty())
            return 全局根操作状态::结构不一致;
        基础节点组追加请求 请求;
        请求.追加字段组.push_back({世界根, 所属世界类型字段, 基础组内字段内容{类型节点}});
        请求.追加外部关系组.push_back({全局根场景节点, 世界根,
            基础外部关系类型::父子, 世界根挂接关系角色});
        // 只读预期引用的宿主、字段键与对端节点须同时列入节点组（基础数据集规范化只读预期）。
        请求.只读预期.节点组.push_back({世界根});
        请求.只读预期.节点组.push_back({所属世界类型字段});
        请求.只读预期.节点组.push_back({全局根场景节点});
        请求.只读预期.字段集合组.push_back({世界根, 所属世界类型字段, {}, {}});
        请求.只读预期.外部关系集合组.push_back({全局根场景节点, true,
            基础外部关系类型::父子, 世界根挂接关系角色, 世界根, {}});
        // 追加字段宿主（世界根）与外部关系两端（全局根、世界根）须带联合快照；字段引用的世界类型节点列入只读节点组。
        {
            const auto 宿主 = 全局基础数据集.查询节点组完整结构({全局根场景节点, 世界根});
            if (宿主.状态 != 基础结构操作状态::已完成 || 宿主.节点组.size() != 2) return 全局根操作状态::结构不一致;
            for (const auto& 项 : 宿主.节点组) {
                if (!项.快照) return 全局根操作状态::结构不一致;
                请求.预期联合节点组.push_back(*项.快照);
            }
            请求.只读预期.节点组.push_back({类型节点});
        }
        const auto 结果 = 全局基础数据集.原子追加节点组(请求);
        if (结果.状态 != 基础节点组追加状态::已完成) return 映射追加状态(结果.状态);
        std::vector<世界根挂接信息> 读回;
        if (!读取挂接已加锁(读回)
            || std::count(读回.begin(), 读回.end(),
                世界根挂接信息{类型节点, 世界根, static_cast<std::int64_t>(*序号 + 1)}) != 1)
            全局根工程失败(L"世界根挂接", L"世界根挂接发布后读回不符。");
        return 全局根操作状态::已完成;
    } catch (const std::bad_alloc&) {
        return 全局根操作状态::资源失败;
    } catch (...) {
        全局根工程失败(L"世界根挂接", L"世界根挂接发生未预期工程异常。");
    }
}

std::optional<稳定编码> 新_全局根类::查询世界根(世界类型登记键 键) noexcept {
    const auto 读回 = 完整读回();
    if (读回.状态 != 全局根操作状态::已完成) return std::nullopt;
    for (const auto& 项 : 读回.挂接组)
        if (项.登记键 == static_cast<std::int64_t>(键)) return 项.世界根节点;
    return std::nullopt;
}

全局根读回结果 新_全局根类::完整读回() noexcept {
    全局根读回结果 结果;
    try {
        if (const auto 会话 = 基础持久运行包::读取已绑定会话(全局基础数据集)) {
            const auto 视图 = 会话->读取();
            return 视图 ? 读取恢复全局根(*视图) : 全局根读回结果{全局根操作状态::结构不一致};
        }
        std::lock_guard 锁(全局根互斥);
        if (!有效(全局根场景节点)) return 结果;
        if (!全局根结构完整已加锁() || !读取挂接已加锁(结果.挂接组)) {
            结果.挂接组.clear();
            结果.状态 = 全局根操作状态::结构不一致;
            return 结果;
        }
        结果.全局根节点 = 全局根场景节点;
        结果.状态 = 全局根操作状态::已完成;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果 = {};
        结果.状态 = 全局根操作状态::资源失败;
        return 结果;
    } catch (...) {
        全局根工程失败(L"全局根读回", L"全局根读回发生未预期工程异常。");
    }
}

全局根读回结果 新_全局根类::读取恢复全局根(const 基础持久视图& 视图) noexcept {
    using S = 全局根操作状态;
    using R = 基础外部关系类型;
    try {
        const auto& 数据 = 视图.结构();
        std::array<稳定编码, 恢复标记组.size()> 标记键{};
        for (std::size_t i = 0; i < 标记键.size(); ++i) {
            const auto 命中 = 视图.查询自键标记(恢复标记组[i]);
            if (命中.size() != 1) return {S::结构不一致};
            const auto 键 = 命中.front().所属节点;
            if (!有效(键) || std::find(标记键.begin(), 标记键.end(), 键) != 标记键.end())
                return {S::结构不一致};
            const auto 字段 = 数据.查询字段(键, 键);
            if (字段.size() != 1 || 字段.front() != 命中.front()
                || !是恢复标记(字段.front(), 键, 恢复标记组[i]))
                return {S::结构不一致};
            标记键[i] = 键;
        }

        const auto [根字段, 根类型, 登记字段, 所属字段] = 标记键;
        const auto 根引用 = 数据.查询目标字段(根类型, 根字段);
        if (根引用.size() != 1) return {S::结构不一致};
        const auto 根 = 根引用.front().所属节点;
        if (std::find(标记键.begin(), 标记键.end(), 根) != 标记键.end()
            || 数据.查询字段(根, 根字段) != 根引用
            || !数据.查询源关系(根, R::兄弟).empty()
            || !数据.查询目标关系(根, R::兄弟).empty()
            || !数据.查询目标关系(根, R::父子).empty())
            return {S::结构不一致};

        std::array<稳定编码, 世界类型数> 类型节点{};
        std::vector<稳定编码> 挂接目标;
        for (const auto& 边 : 数据.查询源关系(根, R::父子)) {
            const auto 目标 = 边.目标节点;
            if (目标 == 根 || std::find(标记键.begin(), 标记键.end(), 目标) != 标记键.end())
                return {S::结构不一致};
            if (边.角色或顺序 == 世界类型登记关系角色) {
                const auto 键 = 唯一整数字段(数据, 目标, 登记字段);
                const auto 序号 = 键 ? 键序号(static_cast<世界类型登记键>(*键)) : std::nullopt;
                if (!序号 || 有效(类型节点[*序号])) return {S::结构不一致};
                const auto 入边 = 数据.查询目标关系(目标, R::父子);
                if (std::count_if(入边.begin(), 入边.end(), [](const auto& 项) {
                        return 项.角色或顺序 == 世界类型登记关系角色;
                    }) != 1)
                    return {S::结构不一致};
                类型节点[*序号] = 目标;
            } else if (边.角色或顺序 == 世界根挂接关系角色) {
                挂接目标.push_back(目标);
            } else {
                return {S::结构不一致};
            }
        }
        if (!std::ranges::all_of(类型节点, 有效)) return {S::结构不一致};

        std::array<稳定编码, 世界类型数> 各世界根{};
        for (const auto 目标 : 挂接目标) {
            if (std::find(类型节点.begin(), 类型节点.end(), 目标) != 类型节点.end())
                return {S::结构不一致};
            const auto 所属 = 数据.查询字段(目标, 所属字段);
            if (所属.size() != 1) return {S::结构不一致};
            const auto* 类型 = std::get_if<稳定编码>(&所属.front().内容);
            if (!类型) return {S::结构不一致};
            const auto 位置 = std::find(类型节点.begin(), 类型节点.end(), *类型);
            if (位置 == 类型节点.end()) return {S::结构不一致};
            const auto 序号 = static_cast<std::size_t>(位置 - 类型节点.begin());
            const auto 入边 = 数据.查询目标关系(目标, R::父子);
            if (有效(各世界根[序号]) || std::count_if(入边.begin(), 入边.end(), [](const auto& 项) {
                    return 项.角色或顺序 == 世界根挂接关系角色;
                }) != 1)
                return {S::结构不一致};
            各世界根[序号] = 目标;
        }

        全局根读回结果 结果;
        for (std::size_t i = 0; i < 各世界根.size(); ++i) {
            const auto 键 = static_cast<std::int64_t>(i + 1);
            const auto 世界根 = 各世界根[i];
            const auto 类型使用 = 数据.查询目标字段(类型节点[i], 所属字段);
            if (!有效(世界根)) {
                if (键 != static_cast<std::int64_t>(世界类型登记键::模拟) || !类型使用.empty())
                    return {S::结构不一致};
                continue;
            }
            if (类型使用.size() != 1 || 类型使用.front().所属节点 != 世界根)
                return {S::结构不一致};
            结果.挂接组.push_back({类型节点[i], 世界根, 键});
        }
        结果.全局根节点 = 根;
        结果.状态 = S::已完成;
        return 结果;
    } catch (const std::bad_alloc&) {
        return {S::资源失败};
    } catch (const std::length_error&) {
        return {S::资源失败};
    } catch (...) {
        全局根工程失败(L"恢复全局根读回", L"恢复全局根读回发生未预期工程异常。");
    }
}

} // namespace 海中鱼巣
