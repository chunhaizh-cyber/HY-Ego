#pragma once

#ifndef INSTINCT_PASSIVE_MAINTENANCE_CURSOR_NO_INCLUDES
#include <cstdint>
#include <optional>
#include "L2结构公共.数据.h"
#include "../业务/本能根运行初始化.数据.h"
#include "../核心/L1所有者范围CRUD.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 本能被动维护游标合同版本_v1 = 1;
inline constexpr std::uint32_t 服务时间维护规则版本_v1 = 1;
inline constexpr L1所有者范围建立幂等身份
    本能被动维护游标所有者建立身份_v1{0x494E'5354'4355'5253ULL};

struct 本能被动维护游标事实_v1 final {
    L2存在身份 自我{};
    std::uint64_t 运行代次 = 0;
    std::uint64_t 时间纪元身份 = 0;
    std::int64_t 上一已维护完整秒边界 = 0;
    std::uint32_t 时间源版本 = 0;
    std::uint32_t 服务时间维护规则版本 = 0;
    std::uint64_t 形成事实代次 = 0;
    friend bool operator==(const 本能被动维护游标事实_v1&,
        const 本能被动维护游标事实_v1&) = default;
};

inline bool 本能被动维护游标事实完整_v1(
    const 本能被动维护游标事实_v1& 事实) noexcept {
    return 有效(事实.自我.值) && 事实.运行代次 != 0
        && 事实.时间纪元身份 != 0
        && 事实.上一已维护完整秒边界 >= 0
        && 事实.时间源版本 != 0
        && 事实.服务时间维护规则版本 != 0
        && 事实.形成事实代次 != 0;
}

struct 建立或恢复本能被动维护游标请求_v1 final {
    std::uint32_t 合同版本 = 本能被动维护游标合同版本_v1;
    L2结构请求头 请求头{};
    L2结构幂等身份 幂等身份{};
    本能根运行锚点_v1 本能根锚点;
    std::uint64_t 运行代次 = 0;
    std::uint64_t 时间纪元身份 = 0;
    std::int64_t 当前完整秒边界 = 0;
    std::uint32_t 时间源版本 = 0;
    std::uint32_t 服务时间维护规则版本 = 服务时间维护规则版本_v1;
    friend bool operator==(const 建立或恢复本能被动维护游标请求_v1&,
        const 建立或恢复本能被动维护游标请求_v1&) = default;
};

enum class 本能被动维护游标建立状态_v1 : std::uint8_t {
    已建立 = 1,
    已恢复 = 2,
    已建立新纪元 = 3,
    入口拒绝 = 4,
    当前性漂移 = 5,
    幂等冲突 = 6,
    引用冲突 = 7,
    资源失败 = 8,
    内部错误 = 9,
    已可能发布 = 10
};

struct 本能被动维护游标建立结果_v1 final {
    std::uint32_t 合同版本 = 本能被动维护游标合同版本_v1;
    本能被动维护游标建立状态_v1 状态 =
        本能被动维护游标建立状态_v1::入口拒绝;
    L2结构幂等身份 幂等身份{};
    std::optional<本能被动维护游标事实_v1> 游标{};
    std::uint64_t 首次提交事实代次 = 0;
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 本能被动维护游标合同版本_v1
            && (状态 == 本能被动维护游标建立状态_v1::已建立
                || 状态 == 本能被动维护游标建立状态_v1::已恢复
                || 状态 == 本能被动维护游标建立状态_v1::已建立新纪元)
            && L2结构幂等身份有效(幂等身份)
            && 游标 && 本能被动维护游标事实完整_v1(*游标)
            && 首次提交事实代次 != 0
            && 本次正式读回截止 != 0
            && 首次提交事实代次 <= 本次正式读回截止;
    }

    friend bool operator==(const 本能被动维护游标建立结果_v1&,
        const 本能被动维护游标建立结果_v1&) = default;
};

struct 本能被动维护游标读取请求_v1 final {
    std::uint32_t 合同版本 = 本能被动维护游标合同版本_v1;
    L2结构请求头 请求头{};
    L2存在身份 自我{};
    friend bool operator==(const 本能被动维护游标读取请求_v1&,
        const 本能被动维护游标读取请求_v1&) = default;
};

enum class 本能被动维护游标读取状态_v1 : std::uint8_t {
    已读取 = 1,
    未建立 = 2,
    入口拒绝 = 3,
    当前性漂移 = 4,
    引用冲突 = 5,
    资源失败 = 6,
    内部错误 = 7
};

struct 本能被动维护游标读取结果_v1 final {
    std::uint32_t 合同版本 = 本能被动维护游标合同版本_v1;
    本能被动维护游标读取状态_v1 状态 =
        本能被动维护游标读取状态_v1::入口拒绝;
    std::optional<本能被动维护游标事实_v1> 游标{};
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 本能被动维护游标合同版本_v1
            && 状态 == 本能被动维护游标读取状态_v1::已读取
            && 游标 && 本能被动维护游标事实完整_v1(*游标)
            && 本次正式读回截止 != 0
            && 游标->形成事实代次 <= 本次正式读回截止;
    }

    friend bool operator==(const 本能被动维护游标读取结果_v1&,
        const 本能被动维护游标读取结果_v1&) = default;
};

} // namespace 海中鱼巣
