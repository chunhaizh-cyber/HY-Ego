#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1状态合同版本 = 1;
inline constexpr std::uint32_t L1状态规则版本 = 1;
inline constexpr std::int64_t 状态节点资格值 = 1;
inline constexpr std::int64_t 状态特征值见证资格值 = 1;

struct 状态操作幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(状态操作幂等身份, 状态操作幂等身份) = default;
};

enum class L1基准状态状态 : std::uint8_t {
    已提交 = 1, 幂等读回 = 2, 已读取 = 3, 未找到 = 4,
    入口拒绝 = 5, 幂等冲突 = 6, 版本漂移 = 7, 许可拒绝 = 8,
    资源失败 = 9, 内部不一致 = 10
};

struct L1状态结构登记 final {
    std::uint32_t 合同版本 = L1状态合同版本;
    std::uint32_t 规则版本 = L1状态规则版本;
    std::uint64_t 事实截止代次 = 0;
    稳定编码 服务身份;
    稳定编码 关系19状态组成关系类型;
    稳定编码 状态节点资格属性类型;
    稳定编码 状态发生时间属性类型;
    稳定编码 状态特征值见证资格属性类型;
    稳定编码 被采样P8值引用属性类型;
    稳定编码 状态采样I64值属性类型;
    friend bool operator==(const L1状态结构登记&, const L1状态结构登记&) = default;
};

struct L1状态登记请求 final {
    std::uint32_t 合同版本 = L1状态合同版本;
    std::uint32_t 规则版本 = L1状态规则版本;
    状态操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const L1状态登记请求&, const L1状态登记请求&) = default;
};

struct L1状态登记结果 final {
    L1基准状态状态 状态 = L1基准状态状态::入口拒绝;
    std::optional<L1状态结构登记> 登记;
};

struct 状态节点见证 final {
    稳定编码 编码;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 状态节点见证&, const 状态节点见证&) = default;
};

struct 状态关系见证 final {
    稳定编码 编码;
    稳定编码 关系类型;
    稳定编码 源;
    稳定编码 目标;
    std::int64_t 顺序号 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 状态关系见证&, const 状态关系见证&) = default;
};

struct 状态I64事实见证 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型;
    std::int64_t 材料 = 0;
    稳定编码 来源节点;
    std::optional<稳定编码> 前一值;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::uint64_t 当前状态发布代次 = 0;
    friend bool operator==(const 状态I64事实见证&, const 状态I64事实见证&) = default;
};

struct 状态U64组事实见证 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型;
    std::vector<std::uint64_t> 材料;
    稳定编码 来源节点;
    std::optional<稳定编码> 前一值;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::uint64_t 当前状态发布代次 = 0;
    friend bool operator==(const 状态U64组事实见证&, const 状态U64组事实见证&) = default;
};

struct 实际存在I64首个基准状态发布请求 final {
    std::uint32_t 合同版本 = L1状态合同版本;
    std::uint32_t 规则版本 = L1状态规则版本;
    状态操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    实际存在I64实例特征读取请求 当前特征;
    std::uint64_t 发生时间戳 = 0;
    friend bool operator==(const 实际存在I64首个基准状态发布请求&,
        const 实际存在I64首个基准状态发布请求&) = default;
};

struct 实际存在I64基准状态读取请求 final {
    std::uint32_t 合同版本 = L1状态合同版本;
    std::uint32_t 规则版本 = L1状态规则版本;
    稳定编码 状态;
    稳定编码 特征值见证;
    稳定编码 状态资格值;
    稳定编码 发生时间值;
    稳定编码 特征值见证资格值;
    稳定编码 被采样P8值引用值;
    稳定编码 状态采样I64值;
    稳定编码 主体关系;
    稳定编码 场景关系;
    稳定编码 特征关系;
    稳定编码 值关系;
    稳定编码 来源存在关系;
    friend bool operator==(const 实际存在I64基准状态读取请求&,
        const 实际存在I64基准状态读取请求&) = default;
};

struct 实际存在I64基准状态事实 final {
    std::uint32_t 合同版本 = L1状态合同版本;
    std::uint32_t 规则版本 = L1状态规则版本;
    std::uint64_t 读取事实截止代次 = 0;
    std::uint64_t 状态采样事实截止代次 = 0;
    状态节点见证 状态;
    状态节点见证 场景;
    状态节点见证 主体;
    状态节点见证 特征;
    状态节点见证 特征值;
    状态节点见证 来源存在;
    状态关系见证 主体关系;
    状态关系见证 场景关系;
    状态关系见证 特征关系;
    状态关系见证 值关系;
    状态关系见证 来源存在关系;
    状态I64事实见证 状态资格;
    状态U64组事实见证 发生时间;
    状态I64事实见证 特征值见证资格;
    状态U64组事实见证 被采样P8值引用;
    状态I64事实见证 采样I64值;
    状态I64事实见证 被采样P8值;
};

struct 实际存在I64基准状态结果 final {
    L1基准状态状态 状态 = L1基准状态状态::入口拒绝;
    std::optional<实际存在I64基准状态事实> 事实;
};

inline bool 状态操作幂等身份有效(状态操作幂等身份 身份) noexcept {
    return 身份.值 != 0 && 身份.值 <= 0x00FF'FFFF'FFFF'FFFFULL;
}

inline bool L1状态结构登记完整(const L1状态结构登记& 登记) noexcept {
    if (登记.合同版本 != L1状态合同版本 || 登记.规则版本 != L1状态规则版本
        || 登记.事实截止代次 == 0) return false;
    const 稳定编码 编码组[] = {登记.服务身份, 登记.关系19状态组成关系类型,
        登记.状态节点资格属性类型, 登记.状态发生时间属性类型,
        登记.状态特征值见证资格属性类型, 登记.被采样P8值引用属性类型,
        登记.状态采样I64值属性类型};
    for (std::size_t i = 0; i < 7; ++i) {
        if (!有效(编码组[i])) return false;
        for (std::size_t j = i + 1; j < 7; ++j)
            if (编码组[i] == 编码组[j]) return false;
    }
    return true;
}

inline bool 实际存在I64基准状态读取请求有效(
    const 实际存在I64基准状态读取请求& 请求) noexcept {
    if (请求.合同版本 != L1状态合同版本 || 请求.规则版本 != L1状态规则版本)
        return false;
    const 稳定编码 编码组[] = {请求.状态, 请求.特征值见证, 请求.状态资格值,
        请求.发生时间值, 请求.特征值见证资格值, 请求.被采样P8值引用值,
        请求.状态采样I64值, 请求.主体关系, 请求.场景关系, 请求.特征关系,
        请求.值关系, 请求.来源存在关系};
    for (std::size_t i = 0; i < 12; ++i) {
        if (!有效(编码组[i])) return false;
        for (std::size_t j = i + 1; j < 12; ++j)
            if (编码组[i] == 编码组[j]) return false;
    }
    return true;
}

} // namespace 海中鱼巣
