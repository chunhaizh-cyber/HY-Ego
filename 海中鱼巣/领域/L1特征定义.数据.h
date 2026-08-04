#pragma once

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1特征定义合同版本 = 1;
inline constexpr std::uint32_t L1特征定义规则版本 = 1;

struct 特征操作幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 特征操作幂等身份&, const 特征操作幂等身份&) = default;
};

inline bool 有效(特征操作幂等身份 身份) noexcept {
    return 身份.值 != 0 && 身份.值 <= 0x00FF'FFFF'FFFF'FFFFULL;
}

struct 特征定义登记 final {
    std::uint32_t 合同版本 = L1特征定义合同版本;
    std::uint32_t 规则版本 = L1特征定义规则版本;
    std::uint64_t 事实截止代次 = 0;
    稳定编码 服务身份;
    稳定编码 定义标记属性类型;
    稳定编码 I64下界属性类型;
    稳定编码 I64上界属性类型;
};

struct I64特征定义事实 final {
    std::uint32_t 合同版本 = L1特征定义合同版本;
    std::uint32_t 规则版本 = L1特征定义规则版本;
    std::uint64_t 事实截止代次 = 0;
    稳定编码 特征定义;
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    稳定编码 来源服务;
};

struct 特征定义登记请求 final {
    std::uint32_t 合同版本 = L1特征定义合同版本;
    std::uint32_t 规则版本 = L1特征定义规则版本;
    特征操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 特征定义登记请求&, const 特征定义登记请求&) = default;
};

struct I64特征定义建立请求 final {
    std::uint32_t 合同版本 = L1特征定义合同版本;
    std::uint32_t 规则版本 = L1特征定义规则版本;
    特征操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const I64特征定义建立请求&, const I64特征定义建立请求&) = default;
};

struct I64特征定义读取请求 final {
    std::uint32_t 合同版本 = L1特征定义合同版本;
    std::uint32_t 规则版本 = L1特征定义规则版本;
    稳定编码 特征定义;
    稳定编码 定义标记值;
    稳定编码 下界值;
    稳定编码 上界值;
};

enum class 特征定义状态 : std::uint8_t {
    已提交 = 1,
    幂等读回 = 2,
    已读取 = 3,
    未找到 = 4,
    登记未加载 = 5,
    入口拒绝 = 6,
    幂等冲突 = 7,
    事实代次漂移 = 8,
    资源失败 = 9,
    内部不一致 = 10
};

struct 特征定义登记结果 final {
    特征定义状态 状态 = 特征定义状态::入口拒绝;
    std::optional<特征定义登记> 登记;
};

struct I64特征定义结果 final {
    特征定义状态 状态 = 特征定义状态::入口拒绝;
    std::optional<I64特征定义事实> 事实;
    稳定编码 定义标记值;
    稳定编码 下界值;
    稳定编码 上界值;
};

inline bool 特征定义登记请求有效(const 特征定义登记请求& 请求) noexcept {
    return 请求.合同版本 == L1特征定义合同版本
        && 请求.规则版本 == L1特征定义规则版本
        && 有效(请求.幂等身份);
}

inline bool I64特征定义建立请求有效(const I64特征定义建立请求& 请求) noexcept {
    return 请求.合同版本 == L1特征定义合同版本
        && 请求.规则版本 == L1特征定义规则版本
        && 有效(请求.幂等身份) && 请求.下界 <= 请求.上界;
}

inline bool 特征定义登记完整(const 特征定义登记& 登记) noexcept {
    return 登记.合同版本 == L1特征定义合同版本
        && 登记.规则版本 == L1特征定义规则版本
        && 登记.事实截止代次 != 0 && 有效(登记.服务身份)
        && 有效(登记.定义标记属性类型)
        && 有效(登记.I64下界属性类型)
        && 有效(登记.I64上界属性类型);
}

inline bool I64特征定义事实完整(const I64特征定义事实& 事实) noexcept {
    return 事实.合同版本 == L1特征定义合同版本
        && 事实.规则版本 == L1特征定义规则版本
        && 事实.事实截止代次 != 0 && 有效(事实.特征定义)
        && 有效(事实.来源服务) && 事实.下界 <= 事实.上界;
}

} // namespace 海中鱼巣
