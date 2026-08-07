#pragma once

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1实际存在合同版本 = 2;
inline constexpr std::uint32_t L1实际存在规则版本 = 1;
inline constexpr std::int64_t 实际存在资格值 = 1;

enum class 实际存在状态 : std::uint8_t {
    已提交 = 1, 幂等读回 = 2, 已读取 = 3, 未找到 = 4,
    入口拒绝 = 5, 幂等冲突 = 6, 版本漂移 = 7,
    许可拒绝 = 8, 资源失败 = 9, 内部不一致 = 10
};

struct 实际存在操作幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 实际存在操作幂等身份&, const 实际存在操作幂等身份&) = default;
};

struct 实际存在结构登记 final {
    std::uint32_t 合同版本 = L1实际存在合同版本;
    std::uint32_t 规则版本 = L1实际存在规则版本;
    实际存在操作幂等身份 首次幂等身份;
    std::uint64_t 事实截止代次 = 0;
    稳定编码 服务身份;
    稳定编码 实际存在资格属性类型;
    friend bool operator==(const 实际存在结构登记&, const 实际存在结构登记&) = default;
};

struct 实际存在登记请求 final {
    std::uint32_t 合同版本 = L1实际存在合同版本;
    std::uint32_t 规则版本 = L1实际存在规则版本;
    实际存在操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 实际存在登记请求&, const 实际存在登记请求&) = default;
};

struct 实际存在登记结果 final {
    实际存在状态 状态 = 实际存在状态::入口拒绝;
    std::optional<实际存在结构登记> 登记;
    bool 成功() const noexcept;
    friend bool operator==(const 实际存在登记结果&, const 实际存在登记结果&) = default;
};

struct 实际存在创建请求 final {
    std::uint32_t 合同版本 = L1实际存在合同版本;
    std::uint32_t 规则版本 = L1实际存在规则版本;
    初始场景读取请求 目标场景;
    实际存在操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 实际存在创建请求&, const 实际存在创建请求&) = default;
};

struct 实际存在写入规格请求 final {
    std::uint32_t 合同版本 = L1实际存在合同版本;
    std::uint32_t 规则版本 = L1实际存在规则版本;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 实际存在写入规格请求&, const 实际存在写入规格请求&) = default;
};

struct 实际存在写入规格 final {
    std::uint64_t 事实截止代次 = 0;
    世界结构节点身份见证 服务身份;
    世界结构节点身份见证 资格属性类型;
    std::int64_t 资格值 = 实际存在资格值;
    friend bool operator==(const 实际存在写入规格&, const 实际存在写入规格&) = default;
};

struct 实际存在写入规格结果 final {
    实际存在状态 状态 = 实际存在状态::入口拒绝;
    std::optional<实际存在写入规格> 规格;
    friend bool operator==(const 实际存在写入规格结果&, const 实际存在写入规格结果&) = default;
};

struct 实际存在资格读取请求 final {
    std::uint32_t 合同版本 = L1实际存在合同版本;
    std::uint32_t 规则版本 = L1实际存在规则版本;
    稳定编码 存在;
    稳定编码 资格值;
    friend bool operator==(const 实际存在资格读取请求&, const 实际存在资格读取请求&) = default;
};

struct 实际存在资格事实 final {
    std::uint64_t 事实截止代次 = 0;
    世界结构节点身份见证 存在;
    世界结构值见证 资格;
    friend bool operator==(const 实际存在资格事实&, const 实际存在资格事实&) = default;
};

struct 实际存在资格结果 final {
    实际存在状态 状态 = 实际存在状态::入口拒绝;
    std::optional<实际存在资格事实> 事实;
    friend bool operator==(const 实际存在资格结果&, const 实际存在资格结果&) = default;
};

struct 实际存在读取请求 final {
    std::uint32_t 合同版本 = L1实际存在合同版本;
    std::uint32_t 规则版本 = L1实际存在规则版本;
    初始场景读取请求 当前场景;
    稳定编码 存在;
    稳定编码 资格值;
    稳定编码 当前场景归属关系;
    friend bool operator==(const 实际存在读取请求&, const 实际存在读取请求&) = default;
};

struct 实际存在事实 final {
    std::uint32_t 合同版本 = L1实际存在合同版本;
    std::uint32_t 规则版本 = L1实际存在规则版本;
    std::uint64_t 事实截止代次 = 0;
    世界结构节点身份见证 世界根;
    世界结构节点身份见证 存在;
    世界结构节点身份见证 当前场景;
    世界结构值见证 资格;
    世界结构关系身份见证 当前场景归属关系;
    friend bool operator==(const 实际存在事实&, const 实际存在事实&) = default;
};

struct 实际存在结果 final {
    实际存在状态 状态 = 实际存在状态::入口拒绝;
    std::optional<实际存在事实> 事实;
    bool 成功() const noexcept;
    friend bool operator==(const 实际存在结果&, const 实际存在结果&) = default;
};

inline bool 实际存在操作幂等身份有效(实际存在操作幂等身份 身份) noexcept {
    return 身份.值 != 0 && 身份.值 <= 0x00FF'FFFF'FFFF'FFFFULL;
}

// 诊断责任：无适用错误分支；纯值创建请求判断不读取状态、不产生副作用。
inline bool 实际存在创建请求有效(const 实际存在创建请求& 请求) noexcept {
    return 请求.合同版本 == L1实际存在合同版本
        && 请求.规则版本 == L1实际存在规则版本
        && 初始场景读取请求有效(请求.目标场景)
        && 实际存在操作幂等身份有效(请求.幂等身份)
        && 请求.期望事实代次 != 0;
}

// 诊断责任：无适用错误分支；纯值资格读取请求判断不读取状态、不产生副作用。
inline bool 实际存在资格读取请求有效(
    const 实际存在资格读取请求& 请求) noexcept {
    return 请求.合同版本 == L1实际存在合同版本
        && 请求.规则版本 == L1实际存在规则版本
        && 有效(请求.存在) && 有效(请求.资格值)
        && 请求.存在 != 请求.资格值;
}

// 诊断责任：无适用错误分支；纯值完整读取请求判断不读取状态、不产生副作用。
inline bool 实际存在读取请求有效(const 实际存在读取请求& 请求) noexcept {
    return 请求.合同版本 == L1实际存在合同版本
        && 请求.规则版本 == L1实际存在规则版本
        && 初始场景读取请求有效(请求.当前场景)
        && 有效(请求.存在) && 有效(请求.资格值)
        && 有效(请求.当前场景归属关系)
        && 请求.存在 != 请求.资格值
        && 请求.存在 != 请求.当前场景归属关系
        && 请求.资格值 != 请求.当前场景归属关系;
}

// 诊断责任：无适用错误分支；纯值请求有效性判断不读取状态、不产生副作用。
inline bool 实际存在登记请求有效(const 实际存在登记请求& 请求) noexcept {
    return 请求.合同版本 == L1实际存在合同版本
        && 请求.规则版本 == L1实际存在规则版本
        && 实际存在操作幂等身份有效(请求.幂等身份)
        && 请求.期望事实代次 != 0;
}

inline bool 实际存在结构登记完整(const 实际存在结构登记& 登记) noexcept {
    return 登记.合同版本 == L1实际存在合同版本
        && 登记.规则版本 == L1实际存在规则版本
        && 实际存在操作幂等身份有效(登记.首次幂等身份)
        && 登记.事实截止代次 != 0 && 有效(登记.服务身份)
        && 有效(登记.实际存在资格属性类型)
        && 登记.服务身份 != 登记.实际存在资格属性类型;
}

// 诊断责任：无适用错误分支；纯值结果成功判断不读取状态、不产生副作用。
inline bool 实际存在登记结果::成功() const noexcept {
    return (状态 == 实际存在状态::已提交
            || 状态 == 实际存在状态::幂等读回
            || 状态 == 实际存在状态::已读取)
        && 登记.has_value() && 实际存在结构登记完整(*登记);
}

inline bool 实际存在写入规格完整(const 实际存在写入规格& 规格) noexcept {
    return 规格.事实截止代次 != 0
        && 世界结构节点见证有效(规格.服务身份)
        && 世界结构节点见证有效(规格.资格属性类型)
        && 规格.服务身份.类型 == 节点种类::普通
        && 规格.资格属性类型.类型 == 节点种类::属性类型
        && 规格.资格值 == 实际存在资格值;
}

inline bool 实际存在资格事实完整(const 实际存在资格事实& 事实) noexcept {
    return 事实.事实截止代次 != 0
        && 世界结构节点见证有效(事实.存在)
        && 世界结构值见证有效(事实.资格)
        && 事实.存在.类型 == 节点种类::普通
        && 事实.资格.所属节点 == 事实.存在
        && 事实.资格.属性类型.类型 == 节点种类::属性类型
        && 事实.资格.来源.类型 == 节点种类::普通
        && 事实.资格.I64值 == 实际存在资格值;
}

inline bool 实际存在事实完整(const 实际存在事实& 事实) noexcept {
    return 事实.合同版本 == L1实际存在合同版本
        && 事实.规则版本 == L1实际存在规则版本
        && 事实.事实截止代次 != 0
        && 世界结构节点见证有效(事实.世界根)
        && 世界结构节点见证有效(事实.存在)
        && 世界结构节点见证有效(事实.当前场景)
        && 世界结构值见证有效(事实.资格)
        && 世界结构关系见证有效(事实.当前场景归属关系)
        && 事实.世界根.类型 == 节点种类::普通
        && 事实.存在.类型 == 节点种类::普通
        && 事实.当前场景.类型 == 节点种类::普通
        && 事实.资格.所属节点 == 事实.存在
        && 事实.资格.I64值 == 实际存在资格值
        && 事实.当前场景归属关系.源端 == 事实.当前场景
        && 事实.当前场景归属关系.目标端 == 事实.存在
        && 事实.当前场景归属关系.角色或顺序 == 0;
}

// 诊断责任：无适用错误分支；纯值结果成功判断不读取状态、不产生副作用。
inline bool 实际存在结果::成功() const noexcept {
    return (状态 == 实际存在状态::已提交
            || 状态 == 实际存在状态::幂等读回
            || 状态 == 实际存在状态::已读取)
        && 事实.has_value() && 实际存在事实完整(*事实);
}

} // namespace 海中鱼巣
