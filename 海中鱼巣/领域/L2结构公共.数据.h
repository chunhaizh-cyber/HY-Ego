#pragma once

#ifndef L2_STRUCTURE_COMMON_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "../核心/L1公共事实.数据.h"
#include "../核心/L1所有者范围CRUD.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2结构合同版本 = 1;

struct L2结构幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2结构幂等身份&,
        const L2结构幂等身份&) = default;
};

#define 定义L2强类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2强类型身份(L2场景身份);
定义L2强类型身份(L2存在身份);
定义L2强类型身份(L2特征定义身份);
定义L2强类型身份(L2特征实例身份);
定义L2强类型身份(L2状态身份);
定义L2强类型身份(L2动态身份);
定义L2强类型身份(L2因果身份);

#undef 定义L2强类型身份

enum class L2读取类别 : std::uint8_t { 当前 = 1, 历史 = 2 };

enum class L2结构状态 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    已读取 = 3,
    未实现 = 4,
    入口拒绝 = 5,
    许可拒绝 = 6,
    未找到 = 7,
    已退出 = 8,
    属性未设置 = 9,
    事实代次漂移 = 10,
    幂等冲突 = 11,
    引用冲突 = 12,
    资源失败 = 13,
    内部不一致 = 14,
    数量预算不足 = 15
};

struct L2结构请求头 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const L2结构请求头&, const L2结构请求头&) = default;
};

struct L2结构结果头 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    L2结构状态 状态 = L2结构状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<std::uint64_t> 变更事实代次;
    friend bool operator==(const L2结构结果头&, const L2结构结果头&) = default;
};

struct L2有序身份引用 final {
    std::uint64_t 顺序 = 0;
    稳定编码 目标;
    friend bool operator==(const L2有序身份引用&,
        const L2有序身份引用&) = default;
};

struct L2独立材料引用 final {
    稳定编码 编码;
    friend bool operator==(const L2独立材料引用&,
        const L2独立材料引用&) = default;
};

using L2原始值材料 = std::variant<std::int64_t, std::vector<std::int64_t>,
    std::vector<std::uint64_t>, L2独立材料引用>;

struct L2属性提交项 final {
    稳定编码 属性类型身份;
    L2原始值材料 原始值材料;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2属性提交项&, const L2属性提交项&) = default;
};

struct L2属性事实 final {
    稳定编码 属性类型身份;
    稳定编码 值稳定编码;
    L2原始值材料 类型化不可变材料;
    稳定编码 来源稳定编码;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const L2属性事实&, const L2属性事实&) = default;
};

struct L2生命周期 final {
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const L2生命周期&, const L2生命周期&) = default;
};

enum class L2节点族 : std::uint8_t {
    场景 = 1,
    存在 = 2,
    特征定义 = 3,
    特征实例 = 4,
    状态 = 5,
    动态 = 6,
    因果 = 7
};

struct L2节点族身份来源事实 final {
    L2节点族 节点族 = L2节点族::存在;
    稳定编码 节点稳定编码;
    L1结构所有者身份 写入所有者;
    稳定编码 族锚点稳定编码;
    稳定编码 族归属关系类型稳定编码;
    稳定编码 族归属关系稳定编码;
    std::int64_t 角色或顺序 = 0;
    L2生命周期 所有者生命周期;
    L2生命周期 节点生命周期;
    L2生命周期 族锚点生命周期;
    L2生命周期 关系类型生命周期;
    L2生命周期 归属关系生命周期;
    friend bool operator==(const L2节点族身份来源事实&,
        const L2节点族身份来源事实&) = default;
};

// 诊断责任：无适用错误分支；纯值合同版本判断不读取事实。
inline bool L2结构请求头合同有效(const L2结构请求头& 请求头) noexcept {
    return 请求头.合同版本 == L2结构合同版本;
}

// 诊断责任：无适用错误分支；纯值幂等身份判断不读取事实。
inline bool L2结构幂等身份有效(L2结构幂等身份 身份) noexcept {
    return 身份.值 != 0;
}

// 诊断责任：无适用错误分支；纯值属性提交项判断不读取事实。
inline bool L2属性提交项有效(const L2属性提交项& 属性) noexcept {
    const bool 材料有效 = std::visit([](const auto& 材料) noexcept {
        using 材料类型 = std::decay_t<decltype(材料)>;
        if constexpr (std::is_same_v<材料类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<材料类型, L2独立材料引用>)
            return 有效(材料.编码);
        else return !材料.empty();
    }, 属性.原始值材料);
    return 有效(属性.属性类型身份) && 有效(属性.来源稳定编码) && 材料有效;
}

// 诊断责任：无适用错误分支；只判断属性事实在具名截止的值式投影。
inline bool L2属性事实截止投影完整(
    const L2属性事实& 属性, std::uint64_t 截止) noexcept {
    const bool 材料完整 = std::visit([](const auto& 材料) noexcept {
        using 材料类型 = std::decay_t<decltype(材料)>;
        if constexpr (std::is_same_v<材料类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<材料类型, L2独立材料引用>)
            return 有效(材料.编码);
        else return !材料.empty();
    }, 属性.类型化不可变材料);
    return 截止 != 0 && 有效(属性.属性类型身份) && 有效(属性.值稳定编码)
        && 有效(属性.来源稳定编码) && 材料完整
        && 属性.创建事实代次 != 0 && 属性.创建事实代次 <= 截止
        && (!属性.退出事实代次
            || (*属性.退出事实代次 >= 属性.创建事实代次
                && *属性.退出事实代次 <= 截止));
}

// 诊断责任：无适用错误分支；纯值生命周期判断不读取事实。
inline bool L2生命周期完整(const L2生命周期& 生命周期) noexcept {
    return 生命周期.创建事实代次 != 0
        && (!生命周期.退出事实代次
            || *生命周期.退出事实代次 >= 生命周期.创建事实代次);
}

// 诊断责任：无适用错误分支；只判断共同身份来源事实的纯值完整性。
inline bool L2节点族身份来源事实完整(
    const L2节点族身份来源事实& 来源, std::uint64_t 截止) noexcept {
    const bool 节点族有效 = 来源.节点族 == L2节点族::场景
        || 来源.节点族 == L2节点族::存在
        || 来源.节点族 == L2节点族::特征定义
        || 来源.节点族 == L2节点族::特征实例
        || 来源.节点族 == L2节点族::状态
        || 来源.节点族 == L2节点族::动态
        || 来源.节点族 == L2节点族::因果;
    const auto 生命周期适用 = [截止](const L2生命周期& 生命周期) noexcept {
        return L2生命周期完整(生命周期)
            && 生命周期.创建事实代次 <= 截止
            && (!生命周期.退出事实代次
                || *生命周期.退出事实代次 <= 截止);
    };
    return 截止 != 0 && 节点族有效 && 有效(来源.节点稳定编码)
        && 有效(来源.写入所有者) && 有效(来源.族锚点稳定编码)
        && 有效(来源.族归属关系类型稳定编码)
        && 有效(来源.族归属关系稳定编码)
        && 来源.节点稳定编码 != 来源.族锚点稳定编码
        && 来源.节点稳定编码 != 来源.族归属关系类型稳定编码
        && 来源.族锚点稳定编码 != 来源.族归属关系类型稳定编码
        && 来源.角色或顺序 == 1
        && 生命周期适用(来源.所有者生命周期)
        && 生命周期适用(来源.节点生命周期)
        && 生命周期适用(来源.族锚点生命周期)
        && 生命周期适用(来源.关系类型生命周期)
        && 生命周期适用(来源.归属关系生命周期);
}

} // namespace 海中鱼巣
