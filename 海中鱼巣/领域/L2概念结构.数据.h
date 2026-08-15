#pragma once

#ifndef L2_CONCEPT_STRUCTURE_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

enum class L2概念本体根角色 : std::uint8_t {
    存在 = 1,
    特征 = 2,
    动态 = 3,
    因果链 = 4
};

struct L2概念本体根角色事实 final {
    L2概念本体根角色 角色 = L2概念本体根角色::存在;
    L2概念身份 根概念;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念本体根角色事实&,
        const L2概念本体根角色事实&) = default;
};

struct L2概念本体根事实 final {
    L2概念身份 根概念;
    L2节点族身份来源事实 身份来源;
    L2概念本体根角色事实 根角色;
    L2结构幂等身份 首次幂等身份;
    std::uint64_t 首次期望事实代次 = 0;
    std::vector<L2概念身份> 直接上位概念;
    friend bool operator==(const L2概念本体根事实&,
        const L2概念本体根事实&) = default;
};

struct L2概念本体根建立请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2概念本体根角色 角色 = L2概念本体根角色::存在;
    friend bool operator==(const L2概念本体根建立请求&,
        const L2概念本体根建立请求&) = default;
};

struct L2概念本体根写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2概念本体根事实> 本体根;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念本体根写入结果&,
        const L2概念本体根写入结果&) = default;
};

struct L2当前概念本体根读取请求 final {
    L2结构请求头 请求头;
    L2概念本体根角色 角色 = L2概念本体根角色::存在;
    friend bool operator==(const L2当前概念本体根读取请求&,
        const L2当前概念本体根读取请求&) = default;
};

struct L2当前概念本体根读取结果 final {
    L2结构结果头 结果头;
    std::optional<L2概念本体根事实> 本体根;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前概念本体根读取结果&,
        const L2当前概念本体根读取结果&) = default;
};

struct L2当前概念本体根组读取请求 final {
    L2结构请求头 请求头;
    friend bool operator==(const L2当前概念本体根组读取请求&,
        const L2当前概念本体根组读取请求&) = default;
};

struct L2当前概念本体根组读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2概念本体根事实> 本体根;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前概念本体根组读取结果&,
        const L2当前概念本体根组读取结果&) = default;
};

// 诊断责任：无适用错误分支；纯值判断角色是否属于四本体根闭集。
inline bool L2概念本体根角色有效(L2概念本体根角色 角色) noexcept {
    return 角色 == L2概念本体根角色::存在
        || 角色 == L2概念本体根角色::特征
        || 角色 == L2概念本体根角色::动态
        || 角色 == L2概念本体根角色::因果链;
}

// 诊断责任：无适用错误分支；只判断根角色事实在具名截止的值式完整性。
inline bool L2概念本体根角色事实完整(
    const L2概念本体根角色事实& 事实,
    std::uint64_t 事实截止代次) noexcept {
    return 事实截止代次 != 0 && L2概念本体根角色有效(事实.角色)
        && 有效(事实.根概念.值) && 有效(事实.关系稳定编码)
        && L2生命周期完整(事实.生命周期)
        && 事实.生命周期.创建事实代次 <= 事实截止代次
        && !事实.生命周期.退出事实代次;
}

// 诊断责任：无适用错误分支；只判断本体根正式负载在具名截止的值式完整性。
inline bool L2概念本体根事实完整(
    const L2概念本体根事实& 事实,
    std::uint64_t 事实截止代次) noexcept {
    const bool 身份来源全部当前 = !事实.身份来源.所有者生命周期.退出事实代次
        && !事实.身份来源.节点生命周期.退出事实代次
        && !事实.身份来源.族锚点生命周期.退出事实代次
        && !事实.身份来源.关系类型生命周期.退出事实代次
        && !事实.身份来源.归属关系生命周期.退出事实代次;
    return 事实截止代次 != 0 && 有效(事实.根概念.值)
        && L2节点族身份来源事实完整(事实.身份来源, 事实截止代次)
        && 事实.身份来源.节点族 == L2节点族::概念
        && 事实.身份来源.节点稳定编码 == 事实.根概念.值
        && 身份来源全部当前
        && L2概念本体根角色事实完整(事实.根角色, 事实截止代次)
        && 事实.根角色.根概念 == 事实.根概念
        && L2结构幂等身份有效(事实.首次幂等身份)
        && 事实.首次期望事实代次 != 0
        && 事实.首次期望事实代次 < 事实.根角色.生命周期.创建事实代次
        && 事实.直接上位概念.empty();
}

// 诊断责任：无适用错误分支；纯值校验建立请求的版本、守卫、幂等身份和角色。
inline bool L2概念本体根建立请求有效(
    const L2概念本体根建立请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && L2概念本体根角色有效(请求.角色);
}

// 诊断责任：无适用错误分支；纯值校验单根当前读取的版本、非零守卫和角色。
inline bool L2当前概念本体根读取请求有效(
    const L2当前概念本体根读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2概念本体根角色有效(请求.角色);
}

// 诊断责任：无适用错误分支；全组读取只校验版本，零守卫保留最新快照语义。
inline bool L2当前概念本体根组读取请求有效(
    const L2当前概念本体根组读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头);
}

// 诊断责任：无适用错误分支；只判断写结果是否为完整的已提交或精确重复形状。
inline bool L2概念本体根写入结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 本体根 && L2概念本体根事实完整(*本体根, 结果头.事实截止代次)
        && 结果头.事实截止代次 > 本体根->首次期望事实代次;
}

// 诊断责任：无适用错误分支；只判断单根读取结果的完整已读取形状。
inline bool L2当前概念本体根读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 本体根 && L2概念本体根事实完整(*本体根, 结果头.事实截止代次);
}

// 诊断责任：无适用错误分支；只判断全组读取结果的完整、排序和互异形状。
inline bool L2当前概念本体根组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次
        || 本体根.size() > 4)
        return false;
    for (std::size_t 索引 = 0; 索引 < 本体根.size(); ++索引) {
        if (!L2概念本体根事实完整(本体根[索引], 结果头.事实截止代次))
            return false;
        if (索引 != 0
            && static_cast<std::uint8_t>(本体根[索引 - 1].根角色.角色)
                >= static_cast<std::uint8_t>(本体根[索引].根角色.角色))
            return false;
        for (std::size_t 已有索引 = 0; 已有索引 < 索引; ++已有索引) {
            if (本体根[已有索引].根概念 == 本体根[索引].根概念)
                return false;
        }
    }
    return true;
}

} // namespace 海中鱼巣
