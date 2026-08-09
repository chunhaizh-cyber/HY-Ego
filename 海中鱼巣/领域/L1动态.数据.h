#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1动态合同版本 = 1;
inline constexpr std::uint32_t L1动态规则版本 = 1;
inline constexpr std::uint64_t L1动态登记幂等域 = 0x0FULL;

enum class L1动态状态 : std::uint8_t {
    已登记 = 1, 已读取 = 2, 登记未加载 = 6, 入口拒绝 = 7, 未找到 = 8,
    幂等冲突 = 9, 版本漂移 = 10,
    资源失败 = 12, 内部不一致 = 13
};

struct 动态操作幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(动态操作幂等身份, 动态操作幂等身份) = default;
};

struct L1动态结构登记 final {
    std::uint32_t 合同版本 = L1动态合同版本;
    std::uint32_t 规则版本 = L1动态规则版本;
    动态操作幂等身份 首次幂等身份;
    std::uint64_t 事实截止代次 = 0;
    稳定编码 服务身份;
    稳定编码 关系20动态组成关系类型;
    稳定编码 动态资格属性类型;
    稳定编码 动态种类属性类型;
    稳定编码 聚合规则版本属性类型;
    稳定编码 时间范围属性类型;
    稳定编码 迁移比较合同属性类型;
    稳定编码 迁移差异I64属性类型;
    稳定编码 比较输入引用属性类型;
    稳定编码 比较输入I64组属性类型;
    friend bool operator==(const L1动态结构登记&, const L1动态结构登记&) = default;
};

struct L1动态登记请求 final {
    std::uint32_t 合同版本 = L1动态合同版本;
    std::uint32_t 规则版本 = L1动态规则版本;
    动态操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const L1动态登记请求&, const L1动态登记请求&) = default;
};

struct L1动态登记结果 final {
    L1动态状态 状态 = L1动态状态::入口拒绝;
    std::optional<L1动态结构登记> 登记;
};

// P14-诊断责任：无适用错误分支；最终责任边界：调用谓词的服务。
inline bool 动态操作幂等身份有效(动态操作幂等身份 身份) noexcept {
    return 身份.值 != 0 && 身份.值 <= 0x00FF'FFFF'FFFF'FFFFULL;
}

// P14-诊断责任：无适用错误分支；最终责任边界：调用谓词的服务。
inline bool L1动态结构登记完整(const L1动态结构登记& 登记) noexcept {
    if (登记.合同版本 != L1动态合同版本 || 登记.规则版本 != L1动态规则版本
        || !动态操作幂等身份有效(登记.首次幂等身份) || 登记.事实截止代次 == 0)
        return false;
    const 稳定编码 编码组[] = {登记.服务身份, 登记.关系20动态组成关系类型,
        登记.动态资格属性类型, 登记.动态种类属性类型, 登记.聚合规则版本属性类型,
        登记.时间范围属性类型, 登记.迁移比较合同属性类型,
        登记.迁移差异I64属性类型, 登记.比较输入引用属性类型,
        登记.比较输入I64组属性类型};
    for (std::size_t i = 0; i < 10; ++i) {
        if (!有效(编码组[i])) return false;
        for (std::size_t j = i + 1; j < 10; ++j)
            if (编码组[i] == 编码组[j]) return false;
    }
    return true;
}

} // namespace 海中鱼巣
