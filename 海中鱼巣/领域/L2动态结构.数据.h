#pragma once

#ifndef L2_DYNAMIC_STRUCTURE_NO_INCLUDES
#include <algorithm>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2动态结构类型登记规则版本 = 1;

#define 定义L2动态结构类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2动态结构类型身份(L2动态主体关系类型身份);
定义L2动态结构类型身份(L2动态前状态关系类型身份);
定义L2动态结构类型身份(L2动态后状态关系类型身份);
定义L2动态结构类型身份(L2动态起始I64值类型身份);
定义L2动态结构类型身份(L2动态起始I64组值类型身份);
定义L2动态结构类型身份(L2动态起始U64组值类型身份);
定义L2动态结构类型身份(L2动态起始独立材料值类型身份);
定义L2动态结构类型身份(L2动态结束I64值类型身份);
定义L2动态结构类型身份(L2动态结束I64组值类型身份);
定义L2动态结构类型身份(L2动态结束U64组值类型身份);
定义L2动态结构类型身份(L2动态结束独立材料值类型身份);

#undef 定义L2动态结构类型身份

enum class L2动态结构类型登记状态 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    已读取 = 3,
    登记未加载 = 4,
    入口拒绝 = 5,
    许可拒绝 = 6,
    事实代次漂移 = 7,
    幂等冲突 = 8,
    资源失败 = 9,
    内部不一致 = 10
};

struct L2动态结构类型登记请求 final {
    L2结构请求头 请求头;
    std::uint32_t 规则版本 = L2动态结构类型登记规则版本;
    L2结构幂等身份 幂等身份{1};
    friend bool operator==(const L2动态结构类型登记请求&,
        const L2动态结构类型登记请求&) = default;
};

struct L2动态结构类型登记 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    std::uint32_t 规则版本 = L2动态结构类型登记规则版本;
    L2结构幂等身份 首次幂等身份{1};
    std::uint64_t 首次期望事实代次 = 0;
    std::uint64_t 建立事实代次 = 0;
    std::uint64_t 事实截止代次 = 0;
    L2动态主体关系类型身份 主体关系类型;
    L2动态前状态关系类型身份 前状态关系类型;
    L2动态后状态关系类型身份 后状态关系类型;
    L2动态起始I64值类型身份 起始I64值类型;
    L2动态起始I64组值类型身份 起始I64组值类型;
    L2动态起始U64组值类型身份 起始U64组值类型;
    L2动态起始独立材料值类型身份 起始独立材料值类型;
    L2动态结束I64值类型身份 结束I64值类型;
    L2动态结束I64组值类型身份 结束I64组值类型;
    L2动态结束U64组值类型身份 结束U64组值类型;
    L2动态结束独立材料值类型身份 结束独立材料值类型;
    friend bool operator==(const L2动态结构类型登记&,
        const L2动态结构类型登记&) = default;
};

struct L2动态结构类型登记结果 final {
    L2动态结构类型登记状态 状态 = L2动态结构类型登记状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<L2动态结构类型登记> 登记;
    bool 成功() const noexcept;
    friend bool operator==(const L2动态结构类型登记结果&,
        const L2动态结构类型登记结果&) = default;
};

struct L2动态事实 final {
    L2动态身份 身份;
    L2存在身份 主体存在;
    L2状态身份 前状态;
    L2状态身份 后状态;
    L2属性事实 起始时间值;
    L2属性事实 结束时间值;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2动态事实&, const L2动态事实&) = default;
};

struct L2动态自有关系引用 final {
    稳定编码 族归属关系稳定编码;
    稳定编码 主体关系稳定编码;
    稳定编码 前状态关系稳定编码;
    稳定编码 后状态关系稳定编码;
    friend bool operator==(const L2动态自有关系引用&,
        const L2动态自有关系引用&) = default;
};

struct L2动态身份来源读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2动态身份 动态;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2动态身份来源读取请求&,
        const L2动态身份来源读取请求&) = default;
};

struct L2动态身份来源读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2节点族身份来源事实> 身份来源;
    bool 成功() const noexcept;
    friend bool operator==(const L2动态身份来源读取结果&,
        const L2动态身份来源读取结果&) = default;
};

struct L2动态新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 主体存在;
    L2状态身份 前状态;
    L2状态身份 后状态;
    L2原始值材料 起始时间材料;
    L2原始值材料 结束时间材料;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2动态新增请求&,
        const L2动态新增请求&) = default;
};

struct L2动态写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2动态事实> 动态;
    std::optional<L2动态自有关系引用> 自有关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2动态写入结果&,
        const L2动态写入结果&) = default;
};

struct L2动态完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2动态身份 动态;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2动态完整读取请求&,
        const L2动态完整读取请求&) = default;
};

struct L2动态完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2动态事实> 动态;
    std::optional<L2动态自有关系引用> 自有关系;
    friend bool operator==(const L2动态完整读取结果&,
        const L2动态完整读取结果&) = default;
};

struct L2按主体当前动态读取请求 final {
    L2结构请求头 请求头;
    L2存在身份 主体存在;
    friend bool operator==(const L2按主体当前动态读取请求&,
        const L2按主体当前动态读取请求&) = default;
};

struct L2按前状态当前动态读取请求 final {
    L2结构请求头 请求头;
    L2状态身份 前状态;
    friend bool operator==(const L2按前状态当前动态读取请求&,
        const L2按前状态当前动态读取请求&) = default;
};

struct L2按后状态当前动态读取请求 final {
    L2结构请求头 请求头;
    L2状态身份 后状态;
    friend bool operator==(const L2按后状态当前动态读取请求&,
        const L2按后状态当前动态读取请求&) = default;
};

struct L2当前动态组读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2动态事实> 动态;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前动态组读取结果&,
        const L2当前动态组读取结果&) = default;
};

inline bool L2动态结构类型登记请求有效(
    const L2动态结构类型登记请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.规则版本 == L2动态结构类型登记规则版本
        && 请求.幂等身份 == L2结构幂等身份{1};
}

inline bool L2动态结构类型登记完整(
    const L2动态结构类型登记& 登记) noexcept {
    const 稳定编码 编码组[] = {
        登记.主体关系类型.值, 登记.前状态关系类型.值, 登记.后状态关系类型.值,
        登记.起始I64值类型.值, 登记.起始I64组值类型.值,
        登记.起始U64组值类型.值, 登记.起始独立材料值类型.值,
        登记.结束I64值类型.值, 登记.结束I64组值类型.值,
        登记.结束U64组值类型.值, 登记.结束独立材料值类型.值};
    if (登记.合同版本 != L2结构合同版本
        || 登记.规则版本 != L2动态结构类型登记规则版本
        || 登记.首次幂等身份 != L2结构幂等身份{1}
        || 登记.首次期望事实代次 == 0 || 登记.建立事实代次 == 0
        || 登记.事实截止代次 < 登记.建立事实代次) return false;
    for (std::size_t 左 = 0; 左 < std::size(编码组); ++左) {
        if (!有效(编码组[左])) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return false;
    }
    return true;
}

inline bool L2动态原始值材料有效(const L2原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型, L2独立材料引用>)
            return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

inline bool L2动态属性角色与材料一致(
    const L2动态结构类型登记& 登记, const L2属性事实& 属性,
    bool 起始角色) noexcept {
    return std::visit([&](const auto& 材料) noexcept {
        using 类型 = std::decay_t<decltype(材料)>;
        稳定编码 期望;
        if constexpr (std::is_same_v<类型, std::int64_t>)
            期望 = 起始角色 ? 登记.起始I64值类型.值 : 登记.结束I64值类型.值;
        else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>)
            期望 = 起始角色 ? 登记.起始I64组值类型.值 : 登记.结束I64组值类型.值;
        else if constexpr (std::is_same_v<类型, std::vector<std::uint64_t>>)
            期望 = 起始角色 ? 登记.起始U64组值类型.值 : 登记.结束U64组值类型.值;
        else
            期望 = 起始角色 ? 登记.起始独立材料值类型.值
                : 登记.结束独立材料值类型.值;
        return 属性.属性类型身份 == 期望;
    }, 属性.类型化不可变材料);
}

inline bool L2动态事实完整(const L2动态事实& 动态,
    const L2动态结构类型登记& 登记, std::uint64_t 截止) noexcept {
    return 截止 != 0 && L2动态结构类型登记完整(登记)
        && 有效(动态.身份.值) && 有效(动态.主体存在.值)
        && 有效(动态.前状态.值) && 有效(动态.后状态.值)
        && 有效(动态.来源稳定编码) && L2生命周期完整(动态.生命周期)
        && 动态.生命周期.创建事实代次 <= 截止
        && (!动态.生命周期.退出事实代次
            || *动态.生命周期.退出事实代次 <= 截止)
        && L2属性事实截止投影完整(动态.起始时间值, 截止)
        && L2属性事实截止投影完整(动态.结束时间值, 截止)
        && 动态.起始时间值.值稳定编码 != 动态.结束时间值.值稳定编码
        && 动态.起始时间值.来源稳定编码 == 动态.来源稳定编码
        && 动态.结束时间值.来源稳定编码 == 动态.来源稳定编码
        && L2动态属性角色与材料一致(登记, 动态.起始时间值, true)
        && L2动态属性角色与材料一致(登记, 动态.结束时间值, false);
}

inline bool L2动态新增请求有效(const L2动态新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.主体存在.值) && 有效(请求.前状态.值)
        && 有效(请求.后状态.值) && 有效(请求.来源稳定编码)
        && L2动态原始值材料有效(请求.起始时间材料)
        && L2动态原始值材料有效(请求.结束时间材料);
}

inline bool L2动态完整读取请求有效(
    const L2动态完整读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.动态.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

inline bool L2动态身份来源读取请求有效(
    const L2动态身份来源读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.动态.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

inline bool L2按主体当前动态读取请求有效(
    const L2按主体当前动态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.主体存在.值);
}

inline bool L2按前状态当前动态读取请求有效(
    const L2按前状态当前动态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.前状态.值);
}

inline bool L2按后状态当前动态读取请求有效(
    const L2按后状态当前动态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.后状态.值);
}

inline bool L2动态结构类型登记结果::成功() const noexcept {
    const bool 成功状态 = 状态 == L2动态结构类型登记状态::已提交
        || 状态 == L2动态结构类型登记状态::精确重复
        || 状态 == L2动态结构类型登记状态::已读取;
    return 成功状态 && 事实截止代次 != 0 && 登记
        && 登记->事实截止代次 == 事实截止代次
        && L2动态结构类型登记完整(*登记);
}

inline bool L2动态写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    const 稳定编码 关系[] = {自有关系 ? 自有关系->族归属关系稳定编码 : 稳定编码{},
        自有关系 ? 自有关系->主体关系稳定编码 : 稳定编码{},
        自有关系 ? 自有关系->前状态关系稳定编码 : 稳定编码{},
        自有关系 ? 自有关系->后状态关系稳定编码 : 稳定编码{}};
    if (!成功状态 || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次 || !动态 || !自有关系)
        return false;
    for (std::size_t 左 = 0; 左 < std::size(关系); ++左) {
        if (!有效(关系[左])) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (关系[左] == 关系[右]) return false;
    }
    return true;
}

inline bool L2动态身份来源读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 身份来源 && L2节点族身份来源事实完整(*身份来源,
            读取类别 == L2读取类别::历史
                ? 历史截止事实代次 : 结果头.事实截止代次)
        && 身份来源->节点族 == L2节点族::动态;
}

inline bool L2当前动态组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    for (std::size_t 索引 = 0; 索引 < 动态.size(); ++索引) {
        if (动态[索引].生命周期.退出事实代次
            || (索引 != 0 && !(动态[索引 - 1].身份.值 < 动态[索引].身份.值)))
            return false;
    }
    return true;
}

} // namespace 海中鱼巣
