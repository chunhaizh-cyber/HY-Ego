#pragma once

#ifndef L2_ORDINARY_METHOD_STRUCTURE_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#include "L2状态结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2普通方法结构合同版本 = 1;
inline constexpr std::uint32_t L2普通方法规格引用合同版本 = 1;

#define 定义L2普通方法节点身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2普通方法节点身份(L2方法治理虚拟存在身份);
定义L2普通方法节点身份(L2方法条件身份);
定义L2普通方法节点身份(L2方法结果身份);
定义L2普通方法节点身份(L2方法动作入口身份);

#undef 定义L2普通方法节点身份

#define 定义L2普通方法规格引用身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2普通方法规格引用身份(L2方法作用对象绑定身份);
定义L2普通方法规格引用身份(L2方法参数规格身份);
定义L2普通方法规格引用身份(L2方法禁止项规格身份);
定义L2普通方法规格引用身份(L2方法适用范围身份);

#undef 定义L2普通方法规格引用身份

struct L2方法稳定动作键 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法稳定动作键&,
        const L2方法稳定动作键&) = default;
};

struct L2方法内容版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法内容版本&,
        const L2方法内容版本&) = default;
};

struct L2方法规格版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法规格版本&,
        const L2方法规格版本&) = default;
};

struct L2方法生命周期版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法生命周期版本&,
        const L2方法生命周期版本&) = default;
};

struct L2方法规格材料 final {
    std::uint64_t 角色 = 0;
    std::uint64_t 顺序 = 0;
    L2原始值材料 值 = std::int64_t{0};
    friend bool operator==(const L2方法规格材料&,
        const L2方法规格材料&) = default;
};

struct L2方法规格提交项 final {
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法规格提交项&,
        const L2方法规格提交项&) = default;
};

struct L2方法条件提交项 final {
    std::uint64_t 作用对象绑定角色 = 0;
    L2场景身份 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 目标状态合同;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法条件提交项&,
        const L2方法条件提交项&) = default;
};

struct L2方法结果提交项 final {
    std::uint64_t 作用对象绑定角色 = 0;
    L2场景身份 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 目标状态合同;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法结果提交项&,
        const L2方法结果提交项&) = default;
};

struct L2方法动作入口提交项 final {
    L2场景身份 输入场景;
    L2场景身份 输出场景;
    L2方法稳定动作键 稳定动作键;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法动作入口提交项&,
        const L2方法动作入口提交项&) = default;
};

struct L2方法作用对象绑定事实 final {
    L2方法作用对象绑定身份 身份;
    L2方法身份 方法;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法作用对象绑定事实&,
        const L2方法作用对象绑定事实&) = default;
};

struct L2方法参数规格事实 final {
    L2方法参数规格身份 身份;
    L2方法身份 方法;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法参数规格事实&,
        const L2方法参数规格事实&) = default;
};

struct L2方法禁止项规格事实 final {
    L2方法禁止项规格身份 身份;
    L2方法身份 方法;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法禁止项规格事实&,
        const L2方法禁止项规格事实&) = default;
};

struct L2方法适用范围事实 final {
    L2方法适用范围身份 身份;
    L2方法身份 方法;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法适用范围事实&,
        const L2方法适用范围事实&) = default;
};

struct L2方法条件事实 final {
    L2方法条件身份 身份;
    L2方法身份 方法;
    L2方法作用对象绑定身份 作用对象绑定;
    L2场景身份 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 目标状态合同;
    L2方法规格版本 规格版本;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法条件事实&,
        const L2方法条件事实&) = default;
};

struct L2方法结果事实 final {
    L2方法结果身份 身份;
    L2方法身份 方法;
    L2方法作用对象绑定身份 作用对象绑定;
    L2场景身份 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 目标状态合同;
    L2方法规格版本 规格版本;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法结果事实&,
        const L2方法结果事实&) = default;
};

struct L2方法动作入口事实 final {
    L2方法动作入口身份 身份;
    L2方法身份 方法;
    L2场景身份 输入场景;
    L2场景身份 输出场景;
    L2方法稳定动作键 稳定动作键;
    L2方法规格版本 规格版本;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法动作入口事实&,
        const L2方法动作入口事实&) = default;
};

struct L2方法生命周期事实 final {
    L2方法身份 方法;
    L2方法生命周期版本 生命周期版本;
    std::int64_t 发生时间 = 0;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法生命周期事实&,
        const L2方法生命周期事实&) = default;
};

struct L2普通方法事实 final {
    L2方法身份 方法;
    L2节点族身份来源事实 身份来源;
    L2方法治理虚拟存在身份 治理虚拟存在;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    std::vector<L2方法条件事实> 条件;
    std::vector<L2方法结果事实> 结果;
    L2方法动作入口事实 动作入口;
    std::vector<L2方法作用对象绑定事实> 作用对象绑定;
    std::vector<L2方法参数规格事实> 参数;
    std::vector<L2方法禁止项规格事实> 禁止项;
    std::vector<L2方法适用范围事实> 适用范围;
    L2方法生命周期事实 生命周期事实;
    friend bool operator==(const L2普通方法事实&,
        const L2普通方法事实&) = default;
};

struct L2普通方法提交材料 final {
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法生命周期版本 生命周期版本;
    std::int64_t 生命周期发生时间 = 0;
    稳定编码 生命周期来源稳定编码;
    std::vector<L2方法规格提交项> 作用对象绑定;
    std::vector<L2方法规格提交项> 参数;
    std::vector<L2方法规格提交项> 禁止项;
    std::vector<L2方法规格提交项> 适用范围;
    std::vector<L2方法条件提交项> 条件;
    std::vector<L2方法结果提交项> 结果;
    L2方法动作入口提交项 动作入口;
    friend bool operator==(const L2普通方法提交材料&,
        const L2普通方法提交材料&) = default;
};

struct L2普通方法新增请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2结构幂等身份 幂等身份;
    L2普通方法提交材料 方法;
    friend bool operator==(const L2普通方法新增请求&,
        const L2普通方法新增请求&) = default;
};

struct L2普通方法替换请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2结构幂等身份 幂等身份;
    L2方法身份 旧方法;
    L2方法内容版本 旧内容版本;
    L2方法规格版本 旧规格版本;
    L2方法生命周期版本 旧生命周期版本;
    L2普通方法提交材料 新方法;
    friend bool operator==(const L2普通方法替换请求&,
        const L2普通方法替换请求&) = default;
};

struct L2普通方法退出请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2结构幂等身份 幂等身份;
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法生命周期版本 生命周期版本;
    friend bool operator==(const L2普通方法退出请求&,
        const L2普通方法退出请求&) = default;
};

struct L2普通方法读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2方法身份 方法;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2普通方法读取请求&,
        const L2普通方法读取请求&) = default;
};

struct L2方法结果规格键 final {
    L2方法规格材料 作用对象绑定;
    L2场景身份 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 目标状态合同;
    L2方法规格版本 规格版本;
    friend bool operator==(const L2方法结果规格键&,
        const L2方法结果规格键&) = default;
};

struct L2当前方法候选读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2方法结果规格键 结果规格;
    std::uint64_t 最大扫描结果事实数 = 0;
    std::uint64_t 最大候选方法数 = 0;
    friend bool operator==(const L2当前方法候选读取请求&,
        const L2当前方法候选读取请求&) = default;
};

struct L2普通方法写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通方法事实> 方法;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通方法写入结果&,
        const L2普通方法写入结果&) = default;
};

struct L2普通方法替换结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通方法事实> 当前方法;
    std::optional<L2普通方法事实> 已退出方法;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通方法替换结果&,
        const L2普通方法替换结果&) = default;
};

struct L2普通方法退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通方法事实> 已退出方法;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通方法退出结果&,
        const L2普通方法退出结果&) = default;
};

struct L2普通方法读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2普通方法事实> 方法;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通方法读取结果&,
        const L2普通方法读取结果&) = default;
};

struct L2当前方法候选读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2方法身份> 候选方法;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前方法候选读取结果&,
        const L2当前方法候选读取结果&) = default;
};

// 诊断责任：无适用错误分支；只判断治理虚拟存在身份的非零稳定编码。
inline bool L2方法治理虚拟存在身份有效(
    L2方法治理虚拟存在身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断方法条件身份的非零稳定编码。
inline bool L2方法条件身份有效(L2方法条件身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断方法结果身份的非零稳定编码。
inline bool L2方法结果身份有效(L2方法结果身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断方法动作入口身份的非零稳定编码。
inline bool L2方法动作入口身份有效(L2方法动作入口身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断独立方法内容版本非零。
inline bool L2方法内容版本有效(L2方法内容版本 版本) noexcept {
    return 版本.值 != 0;
}

// 诊断责任：无适用错误分支；只判断独立方法规格版本非零。
inline bool L2方法规格版本有效(L2方法规格版本 版本) noexcept {
    return 版本.值 != 0;
}

// 诊断责任：无适用错误分支；只判断作用对象绑定身份的非零稳定编码。
inline bool L2方法作用对象绑定身份有效(
    L2方法作用对象绑定身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断参数规格身份的非零稳定编码。
inline bool L2方法参数规格身份有效(L2方法参数规格身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断禁止项规格身份的非零稳定编码。
inline bool L2方法禁止项规格身份有效(
    L2方法禁止项规格身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断适用范围身份的非零稳定编码。
inline bool L2方法适用范围身份有效(L2方法适用范围身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断不透明稳定动作键非零。
inline bool L2方法稳定动作键有效(L2方法稳定动作键 动作键) noexcept {
    return 动作键.值 != 0;
}

// 诊断责任：无适用错误分支；只判断独立方法生命周期版本非零。
inline bool L2方法生命周期版本有效(
    L2方法生命周期版本 版本) noexcept {
    return 版本.值 != 0;
}

// 诊断责任：无适用错误分支；只判断角色、顺序和类型化材料的值式形状。
inline bool L2方法规格材料有效(const L2方法规格材料& 材料) noexcept {
    if (材料.角色 == 0 || 材料.顺序 == 0) return false;
    return std::visit([](const auto& 值) noexcept {
        using 值类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<值类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<值类型, L2独立材料引用>)
            return 有效(值.编码);
        else return !值.empty();
    }, 材料.值);
}

namespace L2普通方法数据内部 {

inline bool 生命周期在截止完整(
    const L2生命周期& 生命周期, std::uint64_t 截止) noexcept {
    return 截止 != 0 && L2生命周期完整(生命周期)
        && 生命周期.创建事实代次 <= 截止
        && (!生命周期.退出事实代次
            || *生命周期.退出事实代次 <= 截止);
}

template<class 组类型, class 取身份>
inline bool 身份严格升序(const 组类型& 组, 取身份 取值) noexcept {
    std::uint64_t 前一 = 0;
    for (const auto& 项 : 组) {
        const auto 当前 = 取值(项);
        if (当前 == 0 || 当前 <= 前一) return false;
        前一 = 当前;
    }
    return true;
}

inline bool 规格提交组有效(
    const std::vector<L2方法规格提交项>& 组) noexcept {
    std::uint64_t 前一角色 = 0;
    std::uint64_t 前一顺序 = 0;
    for (const auto& 项 : 组) {
        if (!L2方法规格材料有效(项.规格) || !有效(项.来源稳定编码))
            return false;
        if (项.规格.角色 < 前一角色
            || (项.规格.角色 == 前一角色 && 项.规格.顺序 <= 前一顺序))
            return false;
        前一角色 = 项.规格.角色;
        前一顺序 = 项.规格.顺序;
    }
    return true;
}

inline const L2方法规格提交项* 按角色查找绑定(
    const std::vector<L2方法规格提交项>& 绑定, std::uint64_t 角色) noexcept {
    const L2方法规格提交项* 找到 = nullptr;
    for (const auto& 项 : 绑定) {
        if (项.规格.角色 != 角色) continue;
        if (找到) return nullptr;
        找到 = &项;
    }
    return 找到;
}

inline bool 条件结果提交项有效(const L2方法条件提交项& 项,
    const std::vector<L2方法规格提交项>& 绑定) noexcept {
    return 项.作用对象绑定角色 != 0 && 按角色查找绑定(绑定,
        项.作用对象绑定角色) && 有效(项.场景.值)
        && 有效(项.特征定义.值) && 有效(项.目标状态合同.值)
        && 有效(项.来源稳定编码);
}

inline bool 条件结果提交项有效(const L2方法结果提交项& 项,
    const std::vector<L2方法规格提交项>& 绑定) noexcept {
    return 项.作用对象绑定角色 != 0 && 按角色查找绑定(绑定,
        项.作用对象绑定角色) && 有效(项.场景.值)
        && 有效(项.特征定义.值) && 有效(项.目标状态合同.值)
        && 有效(项.来源稳定编码);
}

inline bool 普通方法提交材料有效(
    const L2普通方法提交材料& 方法) noexcept {
    if (!L2方法内容版本有效(方法.内容版本)
        || !L2方法规格版本有效(方法.规格版本)
        || !L2方法生命周期版本有效(方法.生命周期版本)
        || !有效(方法.生命周期来源稳定编码)
        || 方法.条件.empty() || 方法.结果.empty()
        || !规格提交组有效(方法.作用对象绑定)
        || 方法.作用对象绑定.empty()
        || !规格提交组有效(方法.参数)
        || !规格提交组有效(方法.禁止项)
        || !规格提交组有效(方法.适用范围)
        || !有效(方法.动作入口.输入场景.值)
        || !有效(方法.动作入口.输出场景.值)
        || !L2方法稳定动作键有效(方法.动作入口.稳定动作键)
        || !有效(方法.动作入口.来源稳定编码)) return false;
    for (const auto& 项 : 方法.条件)
        if (!条件结果提交项有效(项, 方法.作用对象绑定)) return false;
    for (const auto& 项 : 方法.结果)
        if (!条件结果提交项有效(项, 方法.作用对象绑定)) return false;
    return true;
}

inline bool 规格事实共同完整(L2方法身份 方法,
    const L2方法规格材料& 规格, 稳定编码 来源,
    const L2生命周期& 生命周期, std::uint64_t 截止) noexcept {
    return 有效(方法.值) && L2方法规格材料有效(规格) && 有效(来源)
        && 生命周期在截止完整(生命周期, 截止);
}

} // namespace L2普通方法数据内部

inline bool L2普通方法新增请求有效(
    const L2普通方法新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 请求.方法.生命周期版本.值 == 1
        && L2普通方法数据内部::普通方法提交材料有效(请求.方法);
}

inline bool L2普通方法替换请求有效(
    const L2普通方法替换请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && L2结构幂等身份有效(请求.幂等身份) && 有效(请求.旧方法.值)
        && L2方法内容版本有效(请求.旧内容版本)
        && L2方法规格版本有效(请求.旧规格版本)
        && L2方法生命周期版本有效(请求.旧生命周期版本)
        && L2普通方法数据内部::普通方法提交材料有效(请求.新方法)
        && 请求.新方法.生命周期版本.值
            == 请求.旧生命周期版本.值 + 1;
}

inline bool L2普通方法退出请求有效(
    const L2普通方法退出请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && L2结构幂等身份有效(请求.幂等身份) && 有效(请求.方法.值)
        && L2方法内容版本有效(请求.内容版本)
        && L2方法规格版本有效(请求.规格版本)
        && L2方法生命周期版本有效(请求.生命周期版本);
}

inline bool L2普通方法读取请求有效(
    const L2普通方法读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && 有效(请求.方法.值)
        && ((请求.读取类别 == L2读取类别::当前
                && 请求.历史截止事实代次 == 0)
            || (请求.读取类别 == L2读取类别::历史
                && 请求.历史截止事实代次 != 0
                && 请求.历史截止事实代次
                    <= 请求.请求头.期望事实代次));
}

inline bool L2方法结果规格键有效(
    const L2方法结果规格键& 键) noexcept {
    return L2方法规格材料有效(键.作用对象绑定)
        && 有效(键.场景.值) && 有效(键.特征定义.值)
        && 有效(键.目标状态合同.值)
        && L2方法规格版本有效(键.规格版本);
}

inline bool L2当前方法候选读取请求有效(
    const L2当前方法候选读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && L2方法结果规格键有效(请求.结果规格)
        && 请求.最大扫描结果事实数 != 0
        && 请求.最大候选方法数 != 0;
}

inline bool L2普通方法事实完整(
    const L2普通方法事实& 方法, std::uint64_t 截止) noexcept {
    using namespace L2普通方法数据内部;
    if (截止 == 0 || !有效(方法.方法.值)
        || !L2节点族身份来源事实完整(方法.身份来源, 截止)
        || 方法.身份来源.节点族 != L2节点族::方法
        || 方法.身份来源.节点稳定编码 != 方法.方法.值
        || !L2方法治理虚拟存在身份有效(方法.治理虚拟存在)
        || !L2方法内容版本有效(方法.内容版本)
        || !L2方法规格版本有效(方法.规格版本)
        || 方法.条件.empty() || 方法.结果.empty()
        || !L2方法生命周期版本有效(
            方法.生命周期事实.生命周期版本)
        || 方法.生命周期事实.方法 != 方法.方法
        || !有效(方法.生命周期事实.来源稳定编码)
        || !生命周期在截止完整(方法.生命周期事实.生命周期, 截止))
        return false;

    if (!身份严格升序(方法.作用对象绑定,
            [](const auto& 项) { return 项.身份.值.值; })
        || !身份严格升序(方法.参数,
            [](const auto& 项) { return 项.身份.值.值; })
        || !身份严格升序(方法.禁止项,
            [](const auto& 项) { return 项.身份.值.值; })
        || !身份严格升序(方法.适用范围,
            [](const auto& 项) { return 项.身份.值.值; })
        || !身份严格升序(方法.条件,
            [](const auto& 项) { return 项.身份.值.值; })
        || !身份严格升序(方法.结果,
            [](const auto& 项) { return 项.身份.值.值; })) return false;

    for (const auto& 项 : 方法.作用对象绑定)
        if (!L2方法作用对象绑定身份有效(项.身份)
            || 项.方法 != 方法.方法 || !规格事实共同完整(项.方法,
                项.规格, 项.来源稳定编码, 项.生命周期, 截止)) return false;
    for (const auto& 项 : 方法.参数)
        if (!L2方法参数规格身份有效(项.身份)
            || 项.方法 != 方法.方法 || !规格事实共同完整(项.方法,
                项.规格, 项.来源稳定编码, 项.生命周期, 截止)) return false;
    for (const auto& 项 : 方法.禁止项)
        if (!L2方法禁止项规格身份有效(项.身份)
            || 项.方法 != 方法.方法 || !规格事实共同完整(项.方法,
                项.规格, 项.来源稳定编码, 项.生命周期, 截止)) return false;
    for (const auto& 项 : 方法.适用范围)
        if (!L2方法适用范围身份有效(项.身份)
            || 项.方法 != 方法.方法 || !规格事实共同完整(项.方法,
                项.规格, 项.来源稳定编码, 项.生命周期, 截止)) return false;

    const auto 绑定存在 = [&方法](L2方法作用对象绑定身份 身份) noexcept {
        for (const auto& 绑定 : 方法.作用对象绑定)
            if (绑定.身份 == 身份) return true;
        return false;
    };
    for (const auto& 项 : 方法.条件)
        if (!L2方法条件身份有效(项.身份) || 项.方法 != 方法.方法
            || !绑定存在(项.作用对象绑定) || !有效(项.场景.值)
            || !有效(项.特征定义.值) || !有效(项.目标状态合同.值)
            || 项.规格版本 != 方法.规格版本 || !有效(项.来源稳定编码)
            || !生命周期在截止完整(项.生命周期, 截止)) return false;
    for (const auto& 项 : 方法.结果)
        if (!L2方法结果身份有效(项.身份) || 项.方法 != 方法.方法
            || !绑定存在(项.作用对象绑定) || !有效(项.场景.值)
            || !有效(项.特征定义.值) || !有效(项.目标状态合同.值)
            || 项.规格版本 != 方法.规格版本 || !有效(项.来源稳定编码)
            || !生命周期在截止完整(项.生命周期, 截止)) return false;
    return L2方法动作入口身份有效(方法.动作入口.身份)
        && 方法.动作入口.方法 == 方法.方法
        && 有效(方法.动作入口.输入场景.值)
        && 有效(方法.动作入口.输出场景.值)
        && L2方法稳定动作键有效(方法.动作入口.稳定动作键)
        && 方法.动作入口.规格版本 == 方法.规格版本
        && 有效(方法.动作入口.来源稳定编码)
        && 生命周期在截止完整(方法.动作入口.生命周期, 截止);
}

inline bool L2普通方法写入结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 方法 && L2普通方法事实完整(*方法, 结果头.事实截止代次);
}

inline bool L2普通方法替换结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 当前方法 && 已退出方法
        && L2普通方法事实完整(*当前方法, 结果头.事实截止代次)
        && L2普通方法事实完整(*已退出方法, 结果头.事实截止代次)
        && 当前方法->方法 != 已退出方法->方法
        && 已退出方法->生命周期事实.生命周期.退出事实代次
        && *已退出方法->生命周期事实.生命周期.退出事实代次
            == 结果头.事实截止代次;
}

inline bool L2普通方法退出结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 已退出方法
        && L2普通方法事实完整(*已退出方法, 结果头.事实截止代次)
        && 已退出方法->生命周期事实.生命周期.退出事实代次
        && *已退出方法->生命周期事实.生命周期.退出事实代次
            == 结果头.事实截止代次;
}

inline bool L2普通方法读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 方法 && L2普通方法事实完整(*方法, 结果头.事实截止代次)
        && ((读取类别 == L2读取类别::当前 && 历史截止事实代次 == 0
                && !方法->生命周期事实.生命周期.退出事实代次)
            || (读取类别 == L2读取类别::历史
                && 历史截止事实代次 == 结果头.事实截止代次));
}

inline bool L2当前方法候选读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && L2普通方法数据内部::身份严格升序(候选方法,
            [](const auto& 身份) { return 身份.值.值; });
}

} // namespace 海中鱼巣
