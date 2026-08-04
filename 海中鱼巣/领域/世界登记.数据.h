#pragma once

#ifndef 世界登记数据已导入L1
#include "../核心/L1事实基座.数据.h"
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 世界登记合同版本 = 1;
inline constexpr std::uint32_t 世界登记规则版本 = 1;

struct 世界结构登记 final {
    std::uint32_t 合同版本 = 世界登记合同版本;
    std::uint32_t 世界规则版本 = 世界登记规则版本;
    写集幂等键 首次幂等键;
    std::uint64_t 已验证事实代次 = 0;
    稳定编码 服务身份;
    稳定编码 场景标记属性类型;
    稳定编码 直接父场景关系类型;
    稳定编码 直接成员关系类型;
    稳定编码 世界根场景;
    friend bool operator==(const 世界结构登记&, const 世界结构登记&) = default;
};

struct 世界登记建立请求 final {
    std::uint32_t 合同版本 = 世界登记合同版本;
    std::uint32_t 世界规则版本 = 世界登记规则版本;
    写集幂等键 幂等键;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 世界登记建立请求&, const 世界登记建立请求&) = default;
};

enum class 世界登记状态 : std::uint8_t {
    已提交 = 1, 幂等读回 = 2, 已读取 = 3, 登记未加载 = 4,
    入口拒绝 = 5, 幂等冲突 = 6, 事实代次漂移 = 7,
    许可拒绝 = 8, 资源失败 = 9, 内部不一致 = 10
};

struct 世界登记结果 final {
    世界登记状态 状态 = 世界登记状态::入口拒绝;
    std::optional<世界结构登记> 登记;
    friend bool operator==(const 世界登记结果&, const 世界登记结果&) = default;
};

inline bool 世界登记建立请求有效(const 世界登记建立请求& 请求) noexcept {
    return 请求.合同版本 == 世界登记合同版本
        && 请求.世界规则版本 == 世界登记规则版本
        && 有效(请求.幂等键);
}

inline bool 世界结构登记完整(const 世界结构登记& 登记) noexcept {
    const std::array<稳定编码, 5> 编码组{
        登记.服务身份, 登记.场景标记属性类型,
        登记.直接父场景关系类型, 登记.直接成员关系类型,
        登记.世界根场景};
    if (登记.合同版本 != 世界登记合同版本
        || 登记.世界规则版本 != 世界登记规则版本
        || !有效(登记.首次幂等键) || 登记.已验证事实代次 == 0) {
        return false;
    }
    for (std::size_t i = 0; i < 编码组.size(); ++i) {
        if (!有效(编码组[i])) return false;
        for (std::size_t j = i + 1; j < 编码组.size(); ++j)
            if (编码组[i] == 编码组[j]) return false;
    }
    return true;
}

} // namespace 海中鱼巣
