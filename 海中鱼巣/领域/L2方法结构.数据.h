#pragma once

#ifndef L2_METHOD_STRUCTURE_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

enum class L2方法登记根角色 : std::uint8_t {
    方法登记根 = 1
};

struct L2方法登记根角色事实 final {
    L2方法登记根角色 角色 = L2方法登记根角色::方法登记根;
    friend bool operator==(const L2方法登记根角色事实&,
        const L2方法登记根角色事实&) = default;
};

struct L2方法登记根事实 final {
    L2方法身份 根方法;
    L2节点族身份来源事实 身份来源;
    L2方法登记根角色事实 根角色;
    L2结构幂等身份 首次幂等身份;
    std::uint64_t 首次期望事实代次 = 0;
    friend bool operator==(const L2方法登记根事实&,
        const L2方法登记根事实&) = default;
};

struct L2当前方法登记根组读取请求 final {
    L2结构请求头 请求头;
    friend bool operator==(const L2当前方法登记根组读取请求&,
        const L2当前方法登记根组读取请求&) = default;
};

struct L2方法登记根建立请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2方法登记根角色 角色 = L2方法登记根角色::方法登记根;
    friend bool operator==(const L2方法登记根建立请求&,
        const L2方法登记根建立请求&) = default;
};

struct L2当前方法登记根读取请求 final {
    L2结构请求头 请求头;
    L2方法登记根角色 角色 = L2方法登记根角色::方法登记根;
    friend bool operator==(const L2当前方法登记根读取请求&,
        const L2当前方法登记根读取请求&) = default;
};

struct L2当前方法登记根组读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2方法登记根事实> 方法根;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前方法登记根组读取结果&,
        const L2当前方法登记根组读取结果&) = default;
};

struct L2方法登记根写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2方法登记根事实> 方法根;
    bool 成功() const noexcept;
    friend bool operator==(const L2方法登记根写入结果&,
        const L2方法登记根写入结果&) = default;
};

struct L2当前方法登记根读取结果 final {
    L2结构结果头 结果头;
    std::optional<L2方法登记根事实> 方法根;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前方法登记根读取结果&,
        const L2当前方法登记根读取结果&) = default;
};

// 诊断责任：无适用错误分支；只判断方法登记根正式负载在具名截止的值式完整性。
inline bool L2方法登记根事实完整(
    const L2方法登记根事实& 事实,
    std::uint64_t 事实截止代次) noexcept {
    const bool 身份来源全部当前 = !事实.身份来源.所有者生命周期.退出事实代次
        && !事实.身份来源.节点生命周期.退出事实代次
        && !事实.身份来源.族锚点生命周期.退出事实代次
        && !事实.身份来源.关系类型生命周期.退出事实代次
        && !事实.身份来源.归属关系生命周期.退出事实代次;
    return 事实截止代次 != 0 && 有效(事实.根方法.值)
        && L2节点族身份来源事实完整(事实.身份来源, 事实截止代次)
        && 事实.身份来源.节点族 == L2节点族::方法
        && 事实.身份来源.节点稳定编码 == 事实.根方法.值
        && 身份来源全部当前
        && 事实.根角色.角色 == L2方法登记根角色::方法登记根
        && L2结构幂等身份有效(事实.首次幂等身份)
        && 事实.首次期望事实代次 != 0
        && 事实.首次期望事实代次 < 事实截止代次;
}

// 诊断责任：无适用错误分支；纯值校验建立请求的版本、守卫、幂等身份和唯一角色。
inline bool L2方法登记根建立请求有效(
    const L2方法登记根建立请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 请求.角色 == L2方法登记根角色::方法登记根;
}

// 诊断责任：无适用错误分支；只判断完整的已提交或精确重复写结果。
inline bool L2方法登记根写入结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 方法根 && L2方法登记根事实完整(*方法根, 结果头.事实截止代次)
        && 结果头.事实截止代次 > 方法根->首次期望事实代次;
}

// 诊断责任：无适用错误分支；只判断完整的已读取单根结果。
inline bool L2当前方法登记根读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 方法根 && L2方法登记根事实完整(*方法根, 结果头.事实截止代次);
}

// 诊断责任：无适用错误分支；组读允许空组或唯一完整根。
inline bool L2当前方法登记根组读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 方法根.size() <= 1
        && (方法根.empty()
            || L2方法登记根事实完整(方法根.front(), 结果头.事实截止代次));
}

} // namespace 海中鱼巣
