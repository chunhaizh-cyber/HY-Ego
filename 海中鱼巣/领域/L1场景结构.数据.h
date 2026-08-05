#pragma once

#ifndef L1场景结构数据已导入公共事实
#include "../核心/L1公共事实.数据.h"
#endif

#include <array>
#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1场景结构合同版本 = 2;

struct 世界结构节点身份见证 final {
    稳定编码 编码;
    节点种类 类型 = 节点种类::普通;
    friend bool operator==(const 世界结构节点身份见证&, const 世界结构节点身份见证&) = default;
};

struct 世界结构关系身份见证 final {
    稳定编码 编码;
    世界结构节点身份见证 关系类型;
    世界结构节点身份见证 源端;
    世界结构节点身份见证 目标端;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const 世界结构关系身份见证&, const 世界结构关系身份见证&) = default;
};

struct 世界结构值见证 final {
    稳定编码 编码;
    世界结构节点身份见证 所属节点;
    世界结构节点身份见证 属性类型;
    std::int64_t I64值 = 0;
    世界结构节点身份见证 来源;
    std::uint64_t 创建事实代次 = 0;
    friend bool operator==(const 世界结构值见证&, const 世界结构值见证&) = default;
};

struct 初始场景事实 final {
    std::uint32_t 合同版本 = L1场景结构合同版本;
    std::uint32_t 世界规则版本 = 世界登记规则版本;
    std::uint64_t 事实截止代次 = 0;
    世界结构节点身份见证 世界根;
    世界结构节点身份见证 初始场景;
    世界结构值见证 场景标记;
    世界结构关系身份见证 直接父关系;
    friend bool operator==(const 初始场景事实&, const 初始场景事实&) = default;
};

struct 初始场景创建请求 final {
    std::uint32_t 合同版本 = L1场景结构合同版本;
    std::uint32_t 世界规则版本 = 世界登记规则版本;
    世界结构节点身份见证 世界根;
    世界操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 初始场景创建请求&, const 初始场景创建请求&) = default;
};

struct 初始场景读取请求 final {
    std::uint32_t 合同版本 = L1场景结构合同版本;
    std::uint32_t 世界规则版本 = 世界登记规则版本;
    世界结构节点身份见证 世界根;
    稳定编码 初始场景;
    稳定编码 场景标记值;
    稳定编码 直接父关系;
    friend bool operator==(const 初始场景读取请求&, const 初始场景读取请求&) = default;
};

enum class 世界结构状态 : std::uint8_t {
    已提交 = 1, 幂等读回 = 2, 已读取 = 3, 未找到 = 4,
    入口拒绝 = 5, 幂等冲突 = 6, 版本漂移 = 7,
    许可拒绝 = 8, 资源失败 = 9, 内部不一致 = 10
};

struct 初始场景结果 final {
    世界结构状态 状态 = 世界结构状态::入口拒绝;
    std::optional<初始场景事实> 事实;
    friend bool operator==(const 初始场景结果&, const 初始场景结果&) = default;
};

struct 初始场景接纳规格请求 final {
    初始场景读取请求 场景;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const 初始场景接纳规格请求&, const 初始场景接纳规格请求&) = default;
};
struct 初始场景接纳规格 final {
    std::uint64_t 事实截止代次 = 0;
    世界结构节点身份见证 场景;
    世界结构节点身份见证 直接成员关系类型;
    friend bool operator==(const 初始场景接纳规格&, const 初始场景接纳规格&) = default;
};
struct 初始场景接纳规格结果 final {
    世界结构状态 状态 = 世界结构状态::入口拒绝;
    std::optional<初始场景接纳规格> 规格;
    friend bool operator==(const 初始场景接纳规格结果&, const 初始场景接纳规格结果&) = default;
};
struct 初始场景接纳读取请求 final {
    初始场景读取请求 场景;
    稳定编码 成员;
    稳定编码 归属关系;
    friend bool operator==(const 初始场景接纳读取请求&, const 初始场景接纳读取请求&) = default;
};
struct 初始场景接纳事实 final {
    std::uint64_t 事实截止代次 = 0;
    世界结构节点身份见证 场景;
    世界结构节点身份见证 成员;
    世界结构关系身份见证 归属关系;
    friend bool operator==(const 初始场景接纳事实&, const 初始场景接纳事实&) = default;
};
struct 初始场景接纳结果 final {
    世界结构状态 状态 = 世界结构状态::入口拒绝;
    std::optional<初始场景接纳事实> 事实;
    friend bool operator==(const 初始场景接纳结果&, const 初始场景接纳结果&) = default;
};

inline bool 世界结构节点见证有效(const 世界结构节点身份见证& 见证) noexcept {
    return 有效(见证.编码);
}

inline bool 世界结构关系见证有效(const 世界结构关系身份见证& 见证) noexcept {
    return 有效(见证.编码) && 世界结构节点见证有效(见证.关系类型)
        && 世界结构节点见证有效(见证.源端)
        && 世界结构节点见证有效(见证.目标端);
}

inline bool 世界结构值见证有效(const 世界结构值见证& 见证) noexcept {
    return 有效(见证.编码) && 世界结构节点见证有效(见证.所属节点)
        && 世界结构节点见证有效(见证.属性类型)
        && 世界结构节点见证有效(见证.来源)
        && 见证.创建事实代次 != 0;
}

inline bool 初始场景事实完整(const 初始场景事实& 事实) noexcept {
    return 事实.合同版本 == L1场景结构合同版本
        && 事实.世界规则版本 == 世界登记规则版本
        && 事实.事实截止代次 != 0
        && 世界结构节点见证有效(事实.世界根)
        && 世界结构节点见证有效(事实.初始场景)
        && 世界结构值见证有效(事实.场景标记)
        && 世界结构关系见证有效(事实.直接父关系)
        && 事实.世界根.类型 == 节点种类::普通
        && 事实.初始场景.类型 == 节点种类::普通
        && 事实.场景标记.属性类型.类型 == 节点种类::属性类型
        && 事实.场景标记.来源.类型 == 节点种类::普通
        && 事实.直接父关系.源端 == 事实.世界根
        && 事实.直接父关系.目标端 == 事实.初始场景
        && 事实.直接父关系.角色或顺序 == 0
        && 事实.场景标记.I64值 == 1
        && 事实.场景标记.创建事实代次 == 事实.事实截止代次;
}

inline bool 初始场景接纳规格完整(const 初始场景接纳规格& 规格) noexcept {
    return 规格.事实截止代次 != 0
        && 世界结构节点见证有效(规格.场景)
        && 世界结构节点见证有效(规格.直接成员关系类型)
        && 规格.场景.类型 == 节点种类::普通
        && 规格.直接成员关系类型.类型 == 节点种类::普通;
}

inline bool 初始场景接纳事实完整(const 初始场景接纳事实& 事实) noexcept {
    return 事实.事实截止代次 != 0
        && 世界结构节点见证有效(事实.场景)
        && 世界结构节点见证有效(事实.成员)
        && 世界结构关系见证有效(事实.归属关系)
        && 事实.场景.类型 == 节点种类::普通
        && 事实.成员.类型 == 节点种类::普通
        && 事实.归属关系.源端 == 事实.场景
        && 事实.归属关系.目标端 == 事实.成员
        && 事实.归属关系.角色或顺序 == 0;
}

} // namespace 海中鱼巣
