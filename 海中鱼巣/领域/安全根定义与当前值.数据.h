#pragma once

#ifndef INSTINCT_SAFETY_ROOT_DEFINITION_NO_INCLUDES
#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#include "L2特征结构.数据.h"
#include "../业务/本能根运行初始化.数据.h"
#include "../核心/L1所有者范围CRUD.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 安全根定义合同版本_v1 = 1;
inline constexpr L1所有者范围建立幂等身份
    安全根定义所有者建立身份_v1{0x494E'5354'5341'4645ULL};

struct 安全根定义事实身份_v1 final {
    稳定编码 值{};
    friend bool operator==(const 安全根定义事实身份_v1&,
        const 安全根定义事实身份_v1&) = default;
};

inline bool 有效(安全根定义事实身份_v1 身份) noexcept {
    return 有效(身份.值);
}

struct 安全根定义事实_v1 final {
    安全根定义事实身份_v1 身份{};
    L2存在身份 自我{稳定编码{}};
    L2特征定义身份 安全根特征定义{稳定编码{}};
    L2特征实例身份 安全根特征实例{稳定编码{}};
    std::int64_t 低位阈值 = 0;
    std::int64_t 高位阈值 = 0;
    std::uint32_t 定义版本 = 0;
    std::uint32_t 主动安全结算规则版本 = 0;
    std::uint32_t 被动服务维护规则版本 = 0;
    稳定编码 来源稳定编码{};
    std::uint64_t 形成事实代次 = 0;
    friend bool operator==(const 安全根定义事实_v1&,
        const 安全根定义事实_v1&) = default;
};

inline bool 安全根定义事实完整_v1(const 安全根定义事实_v1& 事实) noexcept {
    return 有效(事实.身份) && 有效(事实.自我.值)
        && 有效(事实.安全根特征定义.值)
        && 有效(事实.安全根特征实例.值)
        && 事实.低位阈值 > 1 && 事实.低位阈值 < 事实.高位阈值
        && 事实.高位阈值 <= 本能根I64最大目标_v1
        && 事实.定义版本 != 0 && 事实.主动安全结算规则版本 != 0
        && 事实.被动服务维护规则版本 != 0
        && 有效(事实.来源稳定编码) && 事实.形成事实代次 != 0;
}

struct 发布安全根定义请求_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    L2结构请求头 请求头{};
    L2结构幂等身份 幂等身份{};
    本能根运行锚点_v1 本能根锚点;
    std::int64_t 低位阈值 = 0;
    std::int64_t 高位阈值 = 0;
    std::uint32_t 新定义版本 = 0;
    std::uint32_t 主动安全结算规则版本 = 0;
    std::uint32_t 被动服务维护规则版本 = 0;
    稳定编码 来源稳定编码{};
    friend bool operator==(const 发布安全根定义请求_v1&,
        const 发布安全根定义请求_v1&) = default;
};

enum class 安全根定义发布状态_v1 : std::uint8_t {
    已发布 = 1,
    精确重复 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    当前性漂移 = 5,
    版本漂移 = 6,
    幂等冲突 = 7,
    引用冲突 = 8,
    资源失败 = 9,
    内部错误 = 10,
    已可能发布 = 11
};

struct 安全根定义发布结果_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    安全根定义发布状态_v1 状态 = 安全根定义发布状态_v1::入口拒绝;
    L2结构幂等身份 幂等身份{};
    std::optional<安全根定义事实_v1> 定义{};
    std::uint64_t 首次提交事实代次 = 0;
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 安全根定义合同版本_v1
            && (状态 == 安全根定义发布状态_v1::已发布
                || 状态 == 安全根定义发布状态_v1::精确重复)
            && L2结构幂等身份有效(幂等身份)
            && 定义 && 安全根定义事实完整_v1(*定义)
            && 首次提交事实代次 != 0 && 本次正式读回截止 != 0
            && 首次提交事实代次 <= 本次正式读回截止;
    }

    friend bool operator==(const 安全根定义发布结果_v1&,
        const 安全根定义发布结果_v1&) = default;
};

struct 安全根定义读取请求_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    L2结构请求头 请求头{};
    本能根运行锚点_v1 本能根锚点;
    friend bool operator==(const 安全根定义读取请求_v1&,
        const 安全根定义读取请求_v1&) = default;
};

enum class 安全根定义读取状态_v1 : std::uint8_t {
    已读取 = 1,
    未发布 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    当前性漂移 = 5,
    引用冲突 = 6,
    资源失败 = 7,
    内部错误 = 8
};

struct 安全根定义读取结果_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    安全根定义读取状态_v1 状态 = 安全根定义读取状态_v1::入口拒绝;
    std::optional<安全根定义事实_v1> 定义{};
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 安全根定义合同版本_v1
            && 状态 == 安全根定义读取状态_v1::已读取
            && 定义 && 安全根定义事实完整_v1(*定义)
            && 本次正式读回截止 != 0
            && 定义->形成事实代次 <= 本次正式读回截止;
    }

    friend bool operator==(const 安全根定义读取结果_v1&,
        const 安全根定义读取结果_v1&) = default;
};

struct 主动安全结算来源规则事实身份_v1 final {
    稳定编码 值{};
    friend bool operator==(const 主动安全结算来源规则事实身份_v1&,
        const 主动安全结算来源规则事实身份_v1&) = default;
};

inline bool 有效(主动安全结算来源规则事实身份_v1 身份) noexcept {
    return 有效(身份.值);
}

inline bool 主动安全结算来源方法组规范_v1(
    const std::vector<L2方法身份>& 方法组) noexcept {
    for (std::size_t i = 0; i != 方法组.size(); ++i) {
        if (!有效(方法组[i].值)
            || (i != 0 && 方法组[i - 1].值.值 >= 方法组[i].值.值))
            return false;
    }
    return true;
}

struct 主动安全结算来源规则事实_v1 final {
    主动安全结算来源规则事实身份_v1 身份{};
    L2存在身份 自我{稳定编码{}};
    L2特征实例身份 安全根特征实例{稳定编码{}};
    std::uint32_t 规则版本 = 0;
    std::vector<L2方法身份> 完整来源方法组;
    稳定编码 来源稳定编码{};
    std::uint64_t 形成事实代次 = 0;
    friend bool operator==(const 主动安全结算来源规则事实_v1&,
        const 主动安全结算来源规则事实_v1&) = default;
};

inline bool 主动安全结算来源规则事实完整_v1(
    const 主动安全结算来源规则事实_v1& 事实) noexcept {
    return 有效(事实.身份) && 有效(事实.自我.值)
        && 有效(事实.安全根特征实例.值) && 事实.规则版本 != 0
        && 主动安全结算来源方法组规范_v1(事实.完整来源方法组)
        && 有效(事实.来源稳定编码) && 事实.形成事实代次 != 0;
}

struct 发布主动安全结算来源规则请求_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    L2结构请求头 请求头{};
    L2结构幂等身份 幂等身份{};
    本能根运行锚点_v1 本能根锚点;
    std::uint32_t 新规则版本 = 0;
    std::vector<L2方法身份> 完整来源方法组;
    稳定编码 来源稳定编码{};
    friend bool operator==(const 发布主动安全结算来源规则请求_v1&,
        const 发布主动安全结算来源规则请求_v1&) = default;
};

enum class 主动安全结算来源规则发布状态_v1 : std::uint8_t {
    已发布 = 1,
    精确重复 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    当前性漂移 = 5,
    版本漂移 = 6,
    幂等冲突 = 7,
    引用冲突 = 8,
    资源失败 = 9,
    内部错误 = 10,
    已可能发布 = 11
};

struct 主动安全结算来源规则发布结果_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    主动安全结算来源规则发布状态_v1 状态 =
        主动安全结算来源规则发布状态_v1::入口拒绝;
    L2结构幂等身份 幂等身份{};
    std::optional<主动安全结算来源规则事实_v1> 规则{};
    std::uint64_t 首次提交事实代次 = 0;
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 安全根定义合同版本_v1
            && (状态 == 主动安全结算来源规则发布状态_v1::已发布
                || 状态 == 主动安全结算来源规则发布状态_v1::精确重复)
            && L2结构幂等身份有效(幂等身份)
            && 规则 && 主动安全结算来源规则事实完整_v1(*规则)
            && 首次提交事实代次 != 0 && 本次正式读回截止 != 0
            && 首次提交事实代次 <= 本次正式读回截止;
    }

    friend bool operator==(const 主动安全结算来源规则发布结果_v1&,
        const 主动安全结算来源规则发布结果_v1&) = default;
};

struct 主动安全结算来源规则读取请求_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    L2结构请求头 请求头{};
    本能根运行锚点_v1 本能根锚点;
    friend bool operator==(const 主动安全结算来源规则读取请求_v1&,
        const 主动安全结算来源规则读取请求_v1&) = default;
};

enum class 主动安全结算来源规则读取状态_v1 : std::uint8_t {
    已读取 = 1,
    未发布 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    当前性漂移 = 5,
    引用冲突 = 6,
    资源失败 = 7,
    内部错误 = 8
};

struct 主动安全结算来源规则读取结果_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    主动安全结算来源规则读取状态_v1 状态 =
        主动安全结算来源规则读取状态_v1::入口拒绝;
    std::optional<主动安全结算来源规则事实_v1> 规则{};
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 安全根定义合同版本_v1
            && 状态 == 主动安全结算来源规则读取状态_v1::已读取
            && 规则 && 主动安全结算来源规则事实完整_v1(*规则)
            && 本次正式读回截止 != 0
            && 规则->形成事实代次 <= 本次正式读回截止;
    }

    friend bool operator==(const 主动安全结算来源规则读取结果_v1&,
        const 主动安全结算来源规则读取结果_v1&) = default;
};

struct 安全根定义与当前值读取请求_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    L2结构请求头 请求头{};
    本能根运行锚点_v1 本能根锚点;
    friend bool operator==(const 安全根定义与当前值读取请求_v1&,
        const 安全根定义与当前值读取请求_v1&) = default;
};

struct 安全根定义与当前值快照_v1 final {
    安全根定义事实_v1 定义;
    L2特征值事实 当前值事实;
    std::int64_t 当前安全根值 = 0;
    std::uint64_t 本次正式读回截止 = 0;
    friend bool operator==(const 安全根定义与当前值快照_v1&,
        const 安全根定义与当前值快照_v1&) = default;
};

inline bool 安全根定义与当前值快照完整_v1(
    const 安全根定义与当前值快照_v1& 快照) noexcept {
    const auto* 值 = std::get_if<std::int64_t>(
        &快照.当前值事实.类型化不可变材料);
    return 安全根定义事实完整_v1(快照.定义)
        && 快照.本次正式读回截止 != 0
        && L2特征值事实截止投影完整(
            快照.当前值事实, 快照.本次正式读回截止)
        && 快照.当前值事实.特征实例 == 快照.定义.安全根特征实例
        && !快照.当前值事实.生命周期.退出事实代次
        && 值 && *值 == 快照.当前安全根值
        && 快照.当前安全根值 >= 0
        && 快照.当前安全根值 <= 本能根I64最大目标_v1;
}

enum class 安全根定义与当前值读取状态_v1 : std::uint8_t {
    已读取 = 1,
    定义未发布 = 2,
    当前值未设置 = 3,
    入口拒绝 = 4,
    许可拒绝 = 5,
    当前性漂移 = 6,
    引用冲突 = 7,
    资源失败 = 8,
    内部错误 = 9
};

struct 安全根定义与当前值读取结果_v1 final {
    std::uint32_t 合同版本 = 安全根定义合同版本_v1;
    安全根定义与当前值读取状态_v1 状态 =
        安全根定义与当前值读取状态_v1::入口拒绝;
    std::optional<安全根定义与当前值快照_v1> 快照{};
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 安全根定义合同版本_v1
            && 状态 == 安全根定义与当前值读取状态_v1::已读取
            && 快照 && 安全根定义与当前值快照完整_v1(*快照)
            && 本次正式读回截止 == 快照->本次正式读回截止;
    }

    friend bool operator==(const 安全根定义与当前值读取结果_v1&,
        const 安全根定义与当前值读取结果_v1&) = default;
};

} // namespace 海中鱼巣
