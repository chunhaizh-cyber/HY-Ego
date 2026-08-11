#pragma once

#ifndef L2_EXISTENCE_STRUCTURE_NO_INCLUDES
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

struct L2存在事实 final {
    L2存在身份 身份;
    L2生命周期 生命周期;
    std::vector<L2属性事实> 固定属性;
    friend bool operator==(const L2存在事实&, const L2存在事实&) = default;
};

struct L2存在节点新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    friend bool operator==(const L2存在节点新增请求&,
        const L2存在节点新增请求&) = default;
};

struct L2存在节点读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2存在身份 存在;
    friend bool operator==(const L2存在节点读取请求&,
        const L2存在节点读取请求&) = default;
};

struct L2存在节点写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2存在事实> 存在;
    friend bool operator==(const L2存在节点写入结果&,
        const L2存在节点写入结果&) = default;
};

struct L2存在节点读取结果 final {
    L2结构结果头 结果头;
    std::optional<L2存在事实> 存在;
    friend bool operator==(const L2存在节点读取结果&,
        const L2存在节点读取结果&) = default;
};

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2存在节点新增请求有效(const L2存在节点新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份);
}

// 诊断责任：无适用错误分支；纯值请求判断不读取事实。
inline bool L2存在节点读取请求有效(const L2存在节点读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && (请求.读取类别 == L2读取类别::当前
            || 请求.读取类别 == L2读取类别::历史)
        && 有效(请求.存在.值);
}

// 诊断责任：无适用错误分支；只判断 E1 存在值式事实的完整形状。
inline bool L2存在E1事实完整(const L2存在事实& 存在) noexcept {
    return 有效(存在.身份.值) && L2生命周期完整(存在.生命周期)
        && 存在.固定属性.empty();
}

} // namespace 海中鱼巣
