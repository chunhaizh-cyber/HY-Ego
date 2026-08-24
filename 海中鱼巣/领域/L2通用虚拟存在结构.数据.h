#pragma once

#ifndef L2_VIRTUAL_EXISTENCE_NO_INCLUDES
#include <cstdint>
#include <optional>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2通用虚拟存在合同版本 = 1;

enum class L2通用虚拟存在状态 : std::uint8_t {
    已提交 = 1, 精确重复 = 2, 已读取 = 3, 入口拒绝 = 4,
    许可拒绝 = 5, 幂等冲突 = 6, 未找到 = 7, 已退出 = 8,
    引用冲突 = 9, 事实代次漂移 = 10, 资源失败 = 11,
    内部不一致 = 12
};

struct L2通用虚拟存在新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    稳定编码 ARCHL4宿主引用;
    friend bool operator==(const L2通用虚拟存在新增请求&,
        const L2通用虚拟存在新增请求&) = default;
};

struct L2通用虚拟存在事实 final {
    L2存在身份 身份;
    稳定编码 ARCHL4宿主引用;
    L2生命周期 生命周期;
    friend bool operator==(const L2通用虚拟存在事实&,
        const L2通用虚拟存在事实&) = default;
};

struct L2通用虚拟存在写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2通用虚拟存在事实> 存在;
    friend bool operator==(const L2通用虚拟存在写入结果&,
        const L2通用虚拟存在写入结果&) = default;
};

struct L2通用虚拟存在完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2存在身份 存在;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2通用虚拟存在完整读取请求&,
        const L2通用虚拟存在完整读取请求&) = default;
};

struct L2通用虚拟存在完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2通用虚拟存在事实> 存在;
    friend bool operator==(const L2通用虚拟存在完整读取结果&,
        const L2通用虚拟存在完整读取结果&) = default;
};

struct L2通用虚拟存在退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 存在;
    friend bool operator==(const L2通用虚拟存在退出请求&,
        const L2通用虚拟存在退出请求&) = default;
};

struct L2通用虚拟存在退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2通用虚拟存在事实> 已退出存在;
    friend bool operator==(const L2通用虚拟存在退出结果&,
        const L2通用虚拟存在退出结果&) = default;
};

inline bool L2通用虚拟存在新增请求有效(
    const L2通用虚拟存在新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.ARCHL4宿主引用);
}

} // namespace 海中鱼巣
