#pragma once

#ifndef L2_TASK_STRUCTURE_NO_INCLUDES
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#include "L2需求结构.数据.h"
#endif

namespace 海中鱼巣 {

// ===== 强类型身份 =====
// L2任务身份已在 L2结构公共.数据.h 中定义
// L2需求列表项身份已在 L2需求结构.数据.h 中定义
// 此处新增任务专属强类型身份，严格区分任务核心、虚拟存在、路径与实例方法

#define 定义L2任务强类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2任务强类型身份(L2任务虚拟存在身份);
定义L2任务强类型身份(L2任务方法路径身份);
定义L2任务强类型身份(L2实例方法身份);

#undef 定义L2任务强类型身份

// ===== 结构类型登记 =====

inline constexpr std::uint32_t L2任务结构类型登记规则版本 = 1;

// 任务核心族关系类型身份
#define 定义L2任务结构类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2任务结构类型身份(L2任务族归属关系类型身份);
定义L2任务结构类型身份(L2任务需求列表项引用关系类型身份);
定义L2任务结构类型身份(L2任务虚拟存在归属关系类型身份);

// 方法路径族关系类型身份
定义L2任务结构类型身份(L2任务路径族归属关系类型身份);
定义L2任务结构类型身份(L2任务路径任务引用关系类型身份);
定义L2任务结构类型身份(L2任务当前选中路径关系类型身份);

// 实例方法族关系类型身份
定义L2任务结构类型身份(L2实例方法族归属关系类型身份);
定义L2任务结构类型身份(L2实例方法任务引用关系类型身份);
定义L2任务结构类型身份(L2实例方法路径引用关系类型身份);
定义L2任务结构类型身份(L2实例方法当前关系类型身份);

#undef 定义L2任务结构类型身份

// ===== 身份来源事实 =====

struct L2任务身份来源事实 final {
    L1结构所有者身份 任务所有者;
    稳定编码 任务节点;
    稳定编码 任务族锚点;
    稳定编码 任务族归属关系类型;
    稳定编码 任务族归属关系;
    L2生命周期 任务节点生命周期;
    L2生命周期 族归属关系生命周期;
    friend bool operator==(const L2任务身份来源事实&,
        const L2任务身份来源事实&) = default;
};

inline bool L2任务身份来源事实完整(
    const L2任务身份来源事实& 来源) noexcept {
    return 有效(来源.任务所有者.编码)
        && 有效(来源.任务节点)
        && 有效(来源.任务族锚点)
        && 有效(来源.任务族归属关系类型)
        && 有效(来源.任务族归属关系)
        && 来源.任务节点生命周期.创建事实代次 != 0
        && 来源.族归属关系生命周期.创建事实代次 != 0;
}

// ===== L2任务事实 =====

struct L2任务事实 final {
    L2任务身份 身份;
    L2需求列表项身份 需求列表项;
    L2任务虚拟存在身份 任务虚拟存在;
    L2生命周期 生命周期;
    friend bool operator==(const L2任务事实&,
        const L2任务事实&) = default;
};

// ===== L2任务虚拟存在事实 =====

struct L2任务虚拟存在事实 final {
    L2任务虚拟存在身份 身份;
    L2任务身份 任务;
    L2生命周期 生命周期;
    friend bool operator==(const L2任务虚拟存在事实&,
        const L2任务虚拟存在事实&) = default;
};

// ===== L2完整路径材料 =====

struct L2完整路径材料 final {
    稳定编码 条件绑定;
    稳定编码 输入绑定;
    稳定编码 参数绑定;
    稳定编码 动作入口;
    稳定编码 预期结果;
    稳定编码 验证合同;
    稳定编码 归因合同;
    friend bool operator==(const L2完整路径材料&,
        const L2完整路径材料&) = default;
};

inline bool L2完整路径材料完整(
    const L2完整路径材料& 材料) noexcept {
    return 有效(材料.条件绑定)
        && 有效(材料.输入绑定)
        && 有效(材料.参数绑定)
        && 有效(材料.动作入口)
        && 有效(材料.预期结果)
        && 有效(材料.验证合同)
        && 有效(材料.归因合同);
}

// ===== L2任务方法路径事实 =====

struct L2任务方法路径事实 final {
    L2任务方法路径身份 身份;
    L2任务身份 任务;
    std::uint64_t 筹办轮次 = 0;
    L2方法身份 来源方法;
    稳定编码 来源方法内容版本;
    L2完整路径材料 完整路径材料;
    L2生命周期 生命周期;
    friend bool operator==(const L2任务方法路径事实&,
        const L2任务方法路径事实&) = default;
};

// ===== L2实例方法事实 =====

struct L2实例方法事实 final {
    L2实例方法身份 身份;
    L2任务身份 任务;
    L2任务方法路径身份 当前选中路径;
    std::uint64_t 筹办轮次 = 0;
    std::uint64_t 执行轮次 = 0;
    L2方法身份 来源方法;
    稳定编码 来源方法内容版本;
    L2生命周期 生命周期;
    friend bool operator==(const L2实例方法事实&,
        const L2实例方法事实&) = default;
};

// ===== 请求头有效性守卫 =====

inline bool L2任务结构请求头合同有效(
    const L2结构请求头& 头) noexcept {
    return L2结构请求头合同有效(头);
}

// ===== 通用读取请求守卫 =====

inline bool L2任务读取类别有效(
    L2读取类别 类别, std::uint64_t 历史截止,
    std::uint64_t 期望代次) noexcept {
    return 类别 == L2读取类别::当前
        ? 历史截止 == 0
        : 类别 == L2读取类别::历史
            && 历史截止 != 0
            && 历史截止 <= 期望代次;
}

// ===== 读取任务身份来源 =====

struct L2任务身份来源读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2任务身份来源读取请求&,
        const L2任务身份来源读取请求&) = default;
};

struct L2任务身份来源读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2任务身份来源事实> 身份来源;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
            && 身份来源 && L2任务身份来源事实完整(*身份来源);
    }
    friend bool operator==(const L2任务身份来源读取结果&,
        const L2任务身份来源读取结果&) = default;
};

inline bool L2任务身份来源读取请求有效(
    const L2任务身份来源读取请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 新增任务 =====

struct L2新增任务请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2需求列表项身份 需求列表项;
    friend bool operator==(const L2新增任务请求&,
        const L2新增任务请求&) = default;
};

struct L2新增任务结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务事实> 任务;
    std::optional<L2任务身份来源事实> 身份来源;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已提交
            && 结果头.变更事实代次 != 0
            && 任务 && 身份来源
            && L2任务身份来源事实完整(*身份来源);
    }
    friend bool operator==(const L2新增任务结果&,
        const L2新增任务结果&) = default;
};

inline bool L2新增任务请求有效(
    const L2新增任务请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.需求列表项.值);
}

// ===== 读取任务 =====

struct L2任务读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2任务读取请求&,
        const L2任务读取请求&) = default;
};

struct L2任务读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2任务事实> 任务;
    std::optional<L2任务身份来源事实> 身份来源;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
            && 任务 && 身份来源
            && L2任务身份来源事实完整(*身份来源);
    }
    friend bool operator==(const L2任务读取结果&,
        const L2任务读取结果&) = default;
};

inline bool L2任务读取请求有效(
    const L2任务读取请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 按需求列表项读取当前任务 =====

struct L2按需求列表项读取当前任务请求 final {
    L2结构请求头 请求头;
    L2需求列表项身份 需求列表项;
    friend bool operator==(const L2按需求列表项读取当前任务请求&,
        const L2按需求列表项读取当前任务请求&) = default;
};

struct L2按需求列表项读取当前任务结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务事实> 任务;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2按需求列表项读取当前任务结果&,
        const L2按需求列表项读取当前任务结果&) = default;
};

inline bool L2按需求列表项读取当前任务请求有效(
    const L2按需求列表项读取当前任务请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 有效(请求.需求列表项.值);
}

// ===== 读取任务虚拟存在 =====

struct L2任务虚拟存在读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2任务虚拟存在读取请求&,
        const L2任务虚拟存在读取请求&) = default;
};

struct L2任务虚拟存在读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2任务虚拟存在事实> 任务虚拟存在;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
            && 任务虚拟存在;
    }
    friend bool operator==(const L2任务虚拟存在读取结果&,
        const L2任务虚拟存在读取结果&) = default;
};

inline bool L2任务虚拟存在读取请求有效(
    const L2任务虚拟存在读取请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 退出任务 =====

struct L2任务退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务身份 任务;
    friend bool operator==(const L2任务退出请求&,
        const L2任务退出请求&) = default;
};

struct L2任务退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务事实> 退出后任务;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已提交
            && 结果头.变更事实代次 != 0
            && 退出后任务
            && 退出后任务->生命周期.退出事实代次 != 0;
    }
    friend bool operator==(const L2任务退出结果&,
        const L2任务退出结果&) = default;
};

inline bool L2任务退出请求有效(
    const L2任务退出请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.任务.值);
}

// ===== 新增任务方法路径 =====

struct L2新增任务方法路径请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务身份 任务;
    std::uint64_t 筹办轮次 = 0;
    L2方法身份 来源方法;
    稳定编码 来源方法内容版本;
    L2完整路径材料 完整路径材料;
    friend bool operator==(const L2新增任务方法路径请求&,
        const L2新增任务方法路径请求&) = default;
};

struct L2新增任务方法路径结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务方法路径事实> 路径;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已提交
            && 结果头.变更事实代次 != 0 && 路径;
    }
    friend bool operator==(const L2新增任务方法路径结果&,
        const L2新增任务方法路径结果&) = default;
};

inline bool L2新增任务方法路径请求有效(
    const L2新增任务方法路径请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.任务.值)
        && 请求.筹办轮次 != 0
        && 有效(请求.来源方法.值)
        && 有效(请求.来源方法内容版本)
        && L2完整路径材料完整(请求.完整路径材料);
}

// ===== 读取任务方法路径 =====

struct L2任务方法路径读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务方法路径身份 路径;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2任务方法路径读取请求&,
        const L2任务方法路径读取请求&) = default;
};

struct L2任务方法路径读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2任务方法路径事实> 路径;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
            && 路径;
    }
    friend bool operator==(const L2任务方法路径读取结果&,
        const L2任务方法路径读取结果&) = default;
};

inline bool L2任务方法路径读取请求有效(
    const L2任务方法路径读取请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.路径.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 按任务读取当前方法路径组 =====

struct L2按任务读取当前方法路径组请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2按任务读取当前方法路径组请求&,
        const L2按任务读取当前方法路径组请求&) = default;
};

struct L2按任务读取当前方法路径组结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::vector<L2任务方法路径事实> 当前路径组;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2按任务读取当前方法路径组结果&,
        const L2按任务读取当前方法路径组结果&) = default;
};

inline bool L2按任务读取当前方法路径组请求有效(
    const L2按任务读取当前方法路径组请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 按任务与筹办轮次读取路径组 =====

struct L2按任务与筹办轮次读取路径组请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 筹办轮次 = 0;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2按任务与筹办轮次读取路径组请求&,
        const L2按任务与筹办轮次读取路径组请求&) = default;
};

struct L2按任务与筹办轮次读取路径组结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::uint64_t 筹办轮次 = 0;
    std::vector<L2任务方法路径事实> 路径组;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2按任务与筹办轮次读取路径组结果&,
        const L2按任务与筹办轮次读取路径组结果&) = default;
};

inline bool L2按任务与筹办轮次读取路径组请求有效(
    const L2按任务与筹办轮次读取路径组请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)
        || 请求.筹办轮次 == 0) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 退出任务方法路径 =====

struct L2任务方法路径退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务方法路径身份 路径;
    friend bool operator==(const L2任务方法路径退出请求&,
        const L2任务方法路径退出请求&) = default;
};

struct L2任务方法路径退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务方法路径事实> 退出后路径;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已提交
            && 结果头.变更事实代次 != 0
            && 退出后路径
            && 退出后路径->生命周期.退出事实代次 != 0;
    }
    friend bool operator==(const L2任务方法路径退出结果&,
        const L2任务方法路径退出结果&) = default;
};

inline bool L2任务方法路径退出请求有效(
    const L2任务方法路径退出请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.路径.值);
}

// ===== 发布当前选中路径 =====

struct L2发布当前选中路径请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务身份 任务;
    L2任务方法路径身份 新路径;
    friend bool operator==(const L2发布当前选中路径请求&,
        const L2发布当前选中路径请求&) = default;
};

struct L2发布当前选中路径结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务方法路径身份> 旧当前选中路径;
    std::optional<L2任务方法路径身份> 新当前选中路径;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已提交
            && 结果头.变更事实代次 != 0
            && 新当前选中路径;
    }
    friend bool operator==(const L2发布当前选中路径结果&,
        const L2发布当前选中路径结果&) = default;
};

inline bool L2发布当前选中路径请求有效(
    const L2发布当前选中路径请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.任务.值)
        && 有效(请求.新路径.值);
}

// ===== 读取当前选中路径 =====

struct L2读取当前选中路径请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2读取当前选中路径请求&,
        const L2读取当前选中路径请求&) = default;
};

struct L2读取当前选中路径结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2任务方法路径身份> 当前选中路径;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2读取当前选中路径结果&,
        const L2读取当前选中路径结果&) = default;
};

inline bool L2读取当前选中路径请求有效(
    const L2读取当前选中路径请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 创建实例方法 =====

struct L2创建实例方法请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务身份 任务;
    L2任务方法路径身份 当前选中路径;
    std::uint64_t 筹办轮次 = 0;
    std::uint64_t 执行轮次 = 0;
    L2方法身份 来源方法;
    稳定编码 来源方法内容版本;
    friend bool operator==(const L2创建实例方法请求&,
        const L2创建实例方法请求&) = default;
};

struct L2创建实例方法结果 final {
    L2结构结果头 结果头;
    std::optional<L2实例方法事实> 实例方法;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已提交
            && 结果头.变更事实代次 != 0 && 实例方法;
    }
    friend bool operator==(const L2创建实例方法结果&,
        const L2创建实例方法结果&) = default;
};

inline bool L2创建实例方法请求有效(
    const L2创建实例方法请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.任务.值)
        && 有效(请求.当前选中路径.值)
        && 请求.筹办轮次 != 0
        && 请求.执行轮次 != 0
        && 有效(请求.来源方法.值)
        && 有效(请求.来源方法内容版本);
}

// ===== 读取实例方法 =====

struct L2实例方法读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2实例方法身份 实例方法;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2实例方法读取请求&,
        const L2实例方法读取请求&) = default;
};

struct L2实例方法读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2实例方法事实> 实例方法;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
            && 实例方法;
    }
    friend bool operator==(const L2实例方法读取结果&,
        const L2实例方法读取结果&) = default;
};

inline bool L2实例方法读取请求有效(
    const L2实例方法读取请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.实例方法.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 按任务读取当前实例方法 =====

struct L2按任务读取当前实例方法请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2按任务读取当前实例方法请求&,
        const L2按任务读取当前实例方法请求&) = default;
};

struct L2按任务读取当前实例方法结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2实例方法事实> 当前实例方法;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2按任务读取当前实例方法结果&,
        const L2按任务读取当前实例方法结果&) = default;
};

inline bool L2按任务读取当前实例方法请求有效(
    const L2按任务读取当前实例方法请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)) return false;
    return L2任务读取类别有效(请求.读取类别,
        请求.历史截止事实代次, 请求.请求头.期望事实代次);
}

// ===== 退出当前实例方法 =====

struct L2退出当前实例方法请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2实例方法身份 实例方法;
    friend bool operator==(const L2退出当前实例方法请求&,
        const L2退出当前实例方法请求&) = default;
};

struct L2退出当前实例方法结果 final {
    L2结构结果头 结果头;
    std::optional<L2实例方法事实> 退出后实例方法;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已提交
            && 结果头.变更事实代次 != 0
            && 退出后实例方法;
    }
    friend bool operator==(const L2退出当前实例方法结果&,
        const L2退出当前实例方法结果&) = default;
};

inline bool L2退出当前实例方法请求有效(
    const L2退出当前实例方法请求& 请求) noexcept {
    return L2任务结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.实例方法.值);
}

// ===== 读取实例方法历史 =====

struct L2实例方法历史读取请求 final {
    L2结构请求头 请求头;
    L2任务身份 任务;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2实例方法历史读取请求&,
        const L2实例方法历史读取请求&) = default;
};

struct L2实例方法历史读取结果 final {
    L2结构结果头 结果头;
    std::uint64_t 历史截止事实代次 = 0;
    std::vector<L2实例方法事实> 实例方法历史;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2实例方法历史读取结果&,
        const L2实例方法历史读取结果&) = default;
};

inline bool L2实例方法历史读取请求有效(
    const L2实例方法历史读取请求& 请求) noexcept {
    if (!L2任务结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !有效(请求.任务.值)
        || 请求.历史截止事实代次 == 0
        || 请求.历史截止事实代次 > 请求.请求头.期望事实代次)
        return false;
    return true;
}

} // namespace 海中鱼巣
