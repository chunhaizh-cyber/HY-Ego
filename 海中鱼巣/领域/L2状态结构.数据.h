#pragma once

#ifndef L2_STATE_STRUCTURE_NO_INCLUDES
#include <algorithm>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2状态结构类型登记规则版本 = 1;

#define 定义L2状态结构类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2状态结构类型身份(L2状态主体关系类型身份);
定义L2状态结构类型身份(L2状态特征实例关系类型身份);
定义L2状态结构类型身份(L2状态精确I64值类型身份);
定义L2状态结构类型身份(L2状态精确I64组值类型身份);
定义L2状态结构类型身份(L2状态精确U64组值类型身份);
定义L2状态结构类型身份(L2状态精确独立材料值类型身份);
定义L2状态结构类型身份(L2状态时间I64值类型身份);
定义L2状态结构类型身份(L2状态时间I64组值类型身份);
定义L2状态结构类型身份(L2状态时间U64组值类型身份);
定义L2状态结构类型身份(L2状态时间独立材料值类型身份);

#undef 定义L2状态结构类型身份

enum class L2状态结构类型登记状态 : std::uint8_t {
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

struct L2状态结构类型登记请求 final {
    L2结构请求头 请求头;
    std::uint32_t 规则版本 = L2状态结构类型登记规则版本;
    L2结构幂等身份 幂等身份{1};
    friend bool operator==(const L2状态结构类型登记请求&,
        const L2状态结构类型登记请求&) = default;
};

struct L2状态结构类型登记 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    std::uint32_t 规则版本 = L2状态结构类型登记规则版本;
    L2结构幂等身份 首次幂等身份{1};
    std::uint64_t 首次期望事实代次 = 0;
    std::uint64_t 建立事实代次 = 0;
    std::uint64_t 事实截止代次 = 0;
    L2状态主体关系类型身份 主体关系类型;
    L2状态特征实例关系类型身份 特征实例关系类型;
    L2状态精确I64值类型身份 精确I64值类型;
    L2状态精确I64组值类型身份 精确I64组值类型;
    L2状态精确U64组值类型身份 精确U64组值类型;
    L2状态精确独立材料值类型身份 精确独立材料值类型;
    L2状态时间I64值类型身份 时间I64值类型;
    L2状态时间I64组值类型身份 时间I64组值类型;
    L2状态时间U64组值类型身份 时间U64组值类型;
    L2状态时间独立材料值类型身份 时间独立材料值类型;
    friend bool operator==(const L2状态结构类型登记&,
        const L2状态结构类型登记&) = default;
};

struct L2状态结构类型登记结果 final {
    L2状态结构类型登记状态 状态 = L2状态结构类型登记状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<L2状态结构类型登记> 登记;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态结构类型登记结果&,
        const L2状态结构类型登记结果&) = default;
};

struct L2状态事实 final {
    L2状态身份 身份;
    L2存在身份 主体存在;
    L2特征实例身份 特征实例;
    L2属性事实 精确值;
    L2属性事实 时间值;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2状态事实&, const L2状态事实&) = default;
};

struct L2状态自有关系引用 final {
    稳定编码 族归属关系稳定编码;
    稳定编码 主体关系稳定编码;
    稳定编码 特征实例关系稳定编码;
    friend bool operator==(const L2状态自有关系引用&,
        const L2状态自有关系引用&) = default;
};

struct L2状态身份来源读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2状态身份 状态;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2状态身份来源读取请求&,
        const L2状态身份来源读取请求&) = default;
};

struct L2状态身份来源读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2节点族身份来源事实> 身份来源;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态身份来源读取结果&,
        const L2状态身份来源读取结果&) = default;
};

struct L2状态新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 主体存在;
    L2特征实例身份 特征实例;
    L2原始值材料 精确值材料;
    L2原始值材料 时间值材料;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2状态新增请求&,
        const L2状态新增请求&) = default;
};

struct L2状态写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 状态;
    std::optional<L2状态自有关系引用> 自有关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态写入结果&,
        const L2状态写入结果&) = default;
};

struct L2状态完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2状态身份 状态;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2状态完整读取请求&,
        const L2状态完整读取请求&) = default;
};

struct L2状态完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2状态事实> 状态;
    std::optional<L2状态自有关系引用> 自有关系;
    friend bool operator==(const L2状态完整读取结果&,
        const L2状态完整读取结果&) = default;
};

struct L2按主体当前状态读取请求 final {
    L2结构请求头 请求头;
    L2存在身份 主体存在;
    friend bool operator==(const L2按主体当前状态读取请求&,
        const L2按主体当前状态读取请求&) = default;
};

struct L2按特征实例当前状态读取请求 final {
    L2结构请求头 请求头;
    L2特征实例身份 特征实例;
    friend bool operator==(const L2按特征实例当前状态读取请求&,
        const L2按特征实例当前状态读取请求&) = default;
};

struct L2按来源当前状态读取请求 final {
    L2结构请求头 请求头;
    稳定编码 来源稳定编码;
    std::uint64_t 最大数量 = 0;
    friend bool operator==(const L2按来源当前状态读取请求&,
        const L2按来源当前状态读取请求&) = default;
};

struct L2当前状态组读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2状态事实> 状态;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前状态组读取结果&,
        const L2当前状态组读取结果&) = default;
};

enum class L2状态值角色 : std::uint8_t {
    精确值 = 1,
    时间值 = 2
};

struct L2状态引用对关系引用 final {
    稳定编码 主体关系稳定编码;
    稳定编码 特征实例关系稳定编码;
    friend bool operator==(const L2状态引用对关系引用&,
        const L2状态引用对关系引用&) = default;
};

struct L2状态关系退出事实 final {
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2状态关系退出事实&,
        const L2状态关系退出事实&) = default;
};

struct L2状态引用对关系退出投影 final {
    L2状态关系退出事实 主体关系;
    L2状态关系退出事实 特征实例关系;
    friend bool operator==(const L2状态引用对关系退出投影&,
        const L2状态引用对关系退出投影&) = default;
};

struct L2状态自有关系退出投影 final {
    L2状态关系退出事实 族归属关系;
    L2状态关系退出事实 主体关系;
    L2状态关系退出事实 特征实例关系;
    friend bool operator==(const L2状态自有关系退出投影&,
        const L2状态自有关系退出投影&) = default;
};

struct L2状态引用替换请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2状态身份 状态;
    L2状态引用对关系引用 旧引用;
    L2存在身份 新主体存在;
    L2特征实例身份 新特征实例;
    friend bool operator==(const L2状态引用替换请求&,
        const L2状态引用替换请求&) = default;
};

struct L2状态引用替换结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 当前状态;
    std::optional<L2状态自有关系引用> 当前自有关系;
    std::optional<L2状态引用对关系退出投影> 已退出引用;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态引用替换结果&,
        const L2状态引用替换结果&) = default;
};

struct L2状态值换代请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2状态身份 状态;
    L2状态值角色 值角色 = L2状态值角色::精确值;
    稳定编码 旧值稳定编码;
    L2原始值材料 新值材料;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2状态值换代请求&,
        const L2状态值换代请求&) = default;
};

struct L2状态值换代结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 当前状态;
    std::optional<L2状态自有关系引用> 自有关系;
    std::optional<L2属性事实> 已退出值;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态值换代结果&,
        const L2状态值换代结果&) = default;
};

struct L2状态退出闭包 final {
    L2状态自有关系引用 自有关系;
    稳定编码 精确值稳定编码;
    稳定编码 时间值稳定编码;
    friend bool operator==(const L2状态退出闭包&,
        const L2状态退出闭包&) = default;
};

struct L2状态退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2状态身份 状态;
    L2状态退出闭包 自有事实闭包;
    friend bool operator==(const L2状态退出请求&,
        const L2状态退出请求&) = default;
};

struct L2状态退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 已退出状态;
    std::optional<L2状态自有关系退出投影> 已退出自有关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态退出结果&,
        const L2状态退出结果&) = default;
};

// 诊断责任：无适用错误分支；固定登记合同只接受唯一规则与幂等身份。
inline bool L2状态结构类型登记请求有效(
    const L2状态结构类型登记请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.规则版本 == L2状态结构类型登记规则版本
        && 请求.幂等身份 == L2结构幂等身份{1};
}

// 诊断责任：无适用错误分支；十个登记编码必须非零且互异。
inline bool L2状态结构类型登记完整(
    const L2状态结构类型登记& 登记) noexcept {
    const 稳定编码 编码组[] = {
        登记.主体关系类型.值, 登记.特征实例关系类型.值,
        登记.精确I64值类型.值, 登记.精确I64组值类型.值,
        登记.精确U64组值类型.值, 登记.精确独立材料值类型.值,
        登记.时间I64值类型.值, 登记.时间I64组值类型.值,
        登记.时间U64组值类型.值, 登记.时间独立材料值类型.值};
    if (登记.合同版本 != L2结构合同版本
        || 登记.规则版本 != L2状态结构类型登记规则版本
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

// 诊断责任：无适用错误分支；纯值材料形状判断不读取事实。
inline bool L2状态原始值材料有效(const L2原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型, L2独立材料引用>)
            return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

// 诊断责任：无适用错误分支；只核对两条具名关系编码。
inline bool L2状态引用对关系引用有效(
    const L2状态引用对关系引用& 引用) noexcept {
    return 有效(引用.主体关系稳定编码)
        && 有效(引用.特征实例关系稳定编码)
        && 引用.主体关系稳定编码 != 引用.特征实例关系稳定编码;
}

// 诊断责任：无适用错误分支；只核对带退出生命周期的关系事实。
inline bool L2状态关系退出事实完整(
    const L2状态关系退出事实& 关系, std::uint64_t 退出代次) noexcept {
    return 退出代次 != 0 && 有效(关系.关系稳定编码)
        && L2生命周期完整(关系.生命周期)
        && 关系.生命周期.退出事实代次 == 退出代次;
}

// 诊断责任：无适用错误分支；引用替换请求只接受完整强类型引用对。
inline bool L2状态引用替换请求有效(
    const L2状态引用替换请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.状态.值) && L2状态引用对关系引用有效(请求.旧引用)
        && 有效(请求.新主体存在.值) && 有效(请求.新特征实例.值);
}

// 诊断责任：无适用错误分支；值换代请求只接受两个冻结角色和有效材料。
inline bool L2状态值换代请求有效(
    const L2状态值换代请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.状态.值) && 有效(请求.旧值稳定编码)
        && (请求.值角色 == L2状态值角色::精确值
            || 请求.值角色 == L2状态值角色::时间值)
        && L2状态原始值材料有效(请求.新值材料)
        && 有效(请求.来源稳定编码);
}

// 诊断责任：无适用错误分支；退出闭包五项必须非零、互异且不含状态节点。
inline bool L2状态退出请求有效(const L2状态退出请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !L2结构幂等身份有效(请求.幂等身份)
        || !有效(请求.状态.值)) return false;
    const 稳定编码 编码组[] = {
        请求.自有事实闭包.自有关系.族归属关系稳定编码,
        请求.自有事实闭包.自有关系.主体关系稳定编码,
        请求.自有事实闭包.自有关系.特征实例关系稳定编码,
        请求.自有事实闭包.精确值稳定编码,
        请求.自有事实闭包.时间值稳定编码};
    for (std::size_t 左 = 0; 左 < std::size(编码组); ++左) {
        if (!有效(编码组[左]) || 编码组[左] == 请求.状态.值) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；精确与时间材料分别映射自己的登记角色。
inline bool L2状态属性角色与材料一致(
    const L2状态结构类型登记& 登记, const L2属性事实& 属性,
    bool 精确角色) noexcept {
    return std::visit([&](const auto& 材料) noexcept {
        using 类型 = std::decay_t<decltype(材料)>;
        稳定编码 期望;
        if constexpr (std::is_same_v<类型, std::int64_t>)
            期望 = 精确角色 ? 登记.精确I64值类型.值 : 登记.时间I64值类型.值;
        else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>)
            期望 = 精确角色 ? 登记.精确I64组值类型.值 : 登记.时间I64组值类型.值;
        else if constexpr (std::is_same_v<类型, std::vector<std::uint64_t>>)
            期望 = 精确角色 ? 登记.精确U64组值类型.值 : 登记.时间U64组值类型.值;
        else
            期望 = 精确角色 ? 登记.精确独立材料值类型.值
                : 登记.时间独立材料值类型.值;
        return 属性.属性类型身份 == 期望;
    }, 属性.类型化不可变材料);
}

// 诊断责任：无适用错误分支；只检查具名截止的状态值式投影。
inline bool L2状态事实完整(const L2状态事实& 状态,
    const L2状态结构类型登记& 登记, std::uint64_t 截止) noexcept {
    return 截止 != 0 && L2状态结构类型登记完整(登记)
        && 有效(状态.身份.值) && 有效(状态.主体存在.值)
        && 有效(状态.特征实例.值) && 有效(状态.来源稳定编码)
        && L2生命周期完整(状态.生命周期)
        && 状态.生命周期.创建事实代次 <= 截止
        && (!状态.生命周期.退出事实代次
            || *状态.生命周期.退出事实代次 <= 截止)
        && L2属性事实截止投影完整(状态.精确值, 截止)
        && L2属性事实截止投影完整(状态.时间值, 截止)
        && 状态.精确值.值稳定编码 != 状态.时间值.值稳定编码
        && 状态.精确值.来源稳定编码 == 状态.来源稳定编码
        && 状态.时间值.来源稳定编码 == 状态.来源稳定编码
        && L2状态属性角色与材料一致(登记, 状态.精确值, true)
        && L2状态属性角色与材料一致(登记, 状态.时间值, false);
}

// 诊断责任：无适用错误分支；纯值新增请求判断不读取事实。
inline bool L2状态新增请求有效(const L2状态新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.主体存在.值) && 有效(请求.特征实例.值)
        && 有效(请求.来源稳定编码)
        && L2状态原始值材料有效(请求.精确值材料)
        && L2状态原始值材料有效(请求.时间值材料);
}

// 诊断责任：无适用错误分支；当前与历史读取参数按类别互斥。
inline bool L2状态完整读取请求有效(
    const L2状态完整读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.状态.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；当前与历史来源读取参数按类别互斥。
inline bool L2状态身份来源读取请求有效(
    const L2状态身份来源读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.状态.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；条件读取只接受具名当前端点。
inline bool L2按主体当前状态读取请求有效(
    const L2按主体当前状态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.主体存在.值);
}

// 诊断责任：无适用错误分支；条件读取只接受具名当前端点。
inline bool L2按特征实例当前状态读取请求有效(
    const L2按特征实例当前状态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.特征实例.值);
}

// 诊断责任：无适用错误分支；来源条件读取必须使用非零守卫与数量预算。
inline bool L2按来源当前状态读取请求有效(
    const L2按来源当前状态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 有效(请求.来源稳定编码) && 请求.最大数量 != 0;
}

inline bool L2状态结构类型登记结果::成功() const noexcept {
    const bool 成功状态 = 状态 == L2状态结构类型登记状态::已提交
        || 状态 == L2状态结构类型登记状态::精确重复
        || 状态 == L2状态结构类型登记状态::已读取;
    return 成功状态 && 事实截止代次 != 0 && 登记
        && 登记->事实截止代次 == 事实截止代次
        && L2状态结构类型登记完整(*登记);
}

inline bool L2状态写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    return 成功状态 && 结果头.事实截止代次 != 0
        && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 状态 && 自有关系
        && 有效(自有关系->族归属关系稳定编码)
        && 有效(自有关系->主体关系稳定编码)
        && 有效(自有关系->特征实例关系稳定编码)
        && 自有关系->族归属关系稳定编码
            != 自有关系->主体关系稳定编码
        && 自有关系->族归属关系稳定编码
            != 自有关系->特征实例关系稳定编码
        && 自有关系->主体关系稳定编码
            != 自有关系->特征实例关系稳定编码;
}

inline bool L2状态身份来源读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 身份来源
        && L2节点族身份来源事实完整(*身份来源,
            读取类别 == L2读取类别::历史
                ? 历史截止事实代次 : 结果头.事实截止代次)
        && 身份来源->节点族 == L2节点族::状态;
}

inline bool L2当前状态组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    for (std::size_t 索引 = 0; 索引 < 状态.size(); ++索引) {
        if (状态[索引].生命周期.退出事实代次
            || (索引 != 0 && !(状态[索引 - 1].身份.值 < 状态[索引].身份.值)))
            return false;
    }
    return true;
}

inline bool L2状态引用替换结果::成功() const noexcept {
    if ((结果头.状态 != L2结构状态::已提交
            && 结果头.状态 != L2结构状态::精确重复)
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !当前状态 || !当前自有关系 || !已退出引用
        || !L2状态引用对关系引用有效({当前自有关系->主体关系稳定编码,
            当前自有关系->特征实例关系稳定编码})
        || !有效(当前自有关系->族归属关系稳定编码)) return false;
    const auto 截止 = 结果头.事实截止代次;
    return L2状态关系退出事实完整(已退出引用->主体关系, 截止)
        && L2状态关系退出事实完整(已退出引用->特征实例关系, 截止)
        && 已退出引用->主体关系.关系稳定编码
            != 已退出引用->特征实例关系.关系稳定编码;
}

inline bool L2状态值换代结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 当前状态 && 自有关系 && 已退出值
        && L2属性事实截止投影完整(*已退出值, 结果头.事实截止代次)
        && 已退出值->退出事实代次 == 结果头.事实截止代次;
}

inline bool L2状态退出结果::成功() const noexcept {
    if ((结果头.状态 != L2结构状态::已提交
            && 结果头.状态 != L2结构状态::精确重复)
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !已退出状态 || !已退出自有关系
        || 已退出状态->生命周期.退出事实代次 != 结果头.事实截止代次
        || 已退出状态->精确值.退出事实代次 != 结果头.事实截止代次
        || 已退出状态->时间值.退出事实代次 != 结果头.事实截止代次)
        return false;
    const auto 截止 = 结果头.事实截止代次;
    return L2状态关系退出事实完整(已退出自有关系->族归属关系, 截止)
        && L2状态关系退出事实完整(已退出自有关系->主体关系, 截止)
        && L2状态关系退出事实完整(已退出自有关系->特征实例关系, 截止);
}

} // namespace 海中鱼巣
