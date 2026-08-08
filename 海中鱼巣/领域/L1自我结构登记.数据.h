#pragma once

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1自我结构登记合同版本 = 1;
inline constexpr std::uint32_t L1自我结构登记规则版本 = 1;

enum class 自我结构登记状态 : std::uint8_t {
    已提交 = 1,
    幂等读回 = 2,
    已读取 = 3,
    登记未加载 = 4,
    入口拒绝 = 5,
    幂等冲突 = 6,
    事实代次漂移 = 7,
    许可拒绝 = 8,
    资源失败 = 9,
    内部不一致 = 10
};

struct 自我结构登记操作幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 自我结构登记操作幂等身份&,
        const 自我结构登记操作幂等身份&) = default;
};

struct 自我结构关系登记 final {
    std::uint32_t 合同版本 = L1自我结构登记合同版本;
    std::uint32_t 规则版本 = L1自我结构登记规则版本;
    自我结构登记操作幂等身份 首次幂等身份;
    std::uint64_t 事实截止代次 = 0;
    稳定编码 服务身份;
    稳定编码 自我结构关系类型;
    friend bool operator==(const 自我结构关系登记&,
        const 自我结构关系登记&) = default;
};

struct 自我结构登记请求 final {
    std::uint32_t 合同版本 = L1自我结构登记合同版本;
    std::uint32_t 规则版本 = L1自我结构登记规则版本;
    自我结构登记操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 自我结构登记请求&,
        const 自我结构登记请求&) = default;
};

struct 自我结构登记结果 final {
    自我结构登记状态 状态 = 自我结构登记状态::入口拒绝;
    std::optional<自我结构关系登记> 登记;
    bool 成功() const noexcept;
    friend bool operator==(const 自我结构登记结果&,
        const 自我结构登记结果&) = default;
};

// 诊断责任：无适用错误分支；纯值幂等身份判断不读取状态、不产生副作用。
inline bool 自我结构登记操作幂等身份有效(
    自我结构登记操作幂等身份 身份) noexcept {
    return 身份.值 != 0 && 身份.值 <= 0x00FF'FFFF'FFFF'FFFFULL;
}

// 诊断责任：无适用错误分支；纯值请求判断不读取状态、不产生副作用。
inline bool 自我结构登记请求有效(const 自我结构登记请求& 请求) noexcept {
    return 请求.合同版本 == L1自我结构登记合同版本
        && 请求.规则版本 == L1自我结构登记规则版本
        && 自我结构登记操作幂等身份有效(请求.幂等身份)
        && 请求.期望事实代次 != 0;
}

// 诊断责任：无适用错误分支；纯值登记完整性判断不读取状态、不产生副作用。
inline bool 自我结构关系登记完整(const 自我结构关系登记& 登记) noexcept {
    return 登记.合同版本 == L1自我结构登记合同版本
        && 登记.规则版本 == L1自我结构登记规则版本
        && 自我结构登记操作幂等身份有效(登记.首次幂等身份)
        && 登记.事实截止代次 != 0
        && 有效(登记.服务身份)
        && 有效(登记.自我结构关系类型)
        && 登记.服务身份 != 登记.自我结构关系类型;
}

// 诊断责任：无适用错误分支；纯值结果成功判断不读取状态、不产生副作用。
inline bool 自我结构登记结果::成功() const noexcept {
    return (状态 == 自我结构登记状态::已提交
            || 状态 == 自我结构登记状态::幂等读回
            || 状态 == 自我结构登记状态::已读取)
        && 登记.has_value() && 自我结构关系登记完整(*登记);
}

} // namespace 海中鱼巣
