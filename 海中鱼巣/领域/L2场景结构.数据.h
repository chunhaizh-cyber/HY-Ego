#pragma once

#ifndef L2_SCENE_STRUCTURE_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

struct L2场景父子关系事实 final {
    稳定编码 关系稳定编码;
    L2场景身份 父场景;
    L2场景身份 子场景;
    std::uint64_t 顺序 = 0;
    L2生命周期 生命周期;
    friend bool operator==(const L2场景父子关系事实&,
        const L2场景父子关系事实&) = default;
};

struct L2场景成员关系事实 final {
    稳定编码 关系稳定编码;
    L2场景身份 场景;
    L2存在身份 存在;
    std::uint64_t 顺序 = 0;
    L2生命周期 生命周期;
    friend bool operator==(const L2场景成员关系事实&,
        const L2场景成员关系事实&) = default;
};

struct L2场景事实 final {
    L2场景身份 身份;
    L2生命周期 生命周期;
    std::vector<L2属性事实> 固定属性;
    std::vector<L2场景父子关系事实> 直接子场景;
    std::vector<L2场景成员关系事实> 直接成员;
    friend bool operator==(const L2场景事实&, const L2场景事实&) = default;
};

struct L2场景节点新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    friend bool operator==(const L2场景节点新增请求&,
        const L2场景节点新增请求&) = default;
};

struct L2场景节点读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2场景身份 场景;
    friend bool operator==(const L2场景节点读取请求&,
        const L2场景节点读取请求&) = default;
};

struct L2场景节点写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2场景事实> 场景;
    friend bool operator==(const L2场景节点写入结果&,
        const L2场景节点写入结果&) = default;
};

struct L2场景节点读取结果 final {
    L2结构结果头 结果头;
    std::optional<L2场景事实> 场景;
    friend bool operator==(const L2场景节点读取结果&,
        const L2场景节点读取结果&) = default;
};

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2场景节点新增请求有效(const L2场景节点新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份);
}

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2场景节点读取请求有效(const L2场景节点读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && (请求.读取类别 == L2读取类别::当前
            || 请求.读取类别 == L2读取类别::历史)
        && 有效(请求.场景.值);
}

// 诊断责任：无适用错误分支；只判断 S1 场景值式事实的完整形状。
inline bool L2场景S1事实完整(const L2场景事实& 场景) noexcept {
    return 有效(场景.身份.值) && L2生命周期完整(场景.生命周期)
        && 场景.固定属性.empty() && 场景.直接子场景.empty()
        && 场景.直接成员.empty();
}

} // namespace 海中鱼巣
