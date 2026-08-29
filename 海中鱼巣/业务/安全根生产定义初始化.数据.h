#pragma once

#ifndef INSTINCT_PRODUCTION_SAFETY_DEFINITION_INIT_NO_INCLUDES
#include <cstdint>
#include <optional>
#include "本能根运行初始化.数据.h"
#include "../领域/安全根定义与当前值.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 安全根生产定义初始化合同版本_v1 = 1;
inline constexpr std::int64_t 安全根生产低位阈值_v1 =
    2767011611056432742LL;
inline constexpr std::int64_t 安全根生产高位阈值_v1 =
    7378697629483820645LL;
inline constexpr std::uint32_t 安全根生产定义版本_v1 = 1;
inline constexpr std::uint32_t 主动安全结算规则版本_v1 = 1;
inline constexpr std::uint32_t 被动服务维护规则版本_v1 = 1;
inline constexpr L2结构幂等身份 安全根生产定义发布幂等身份_v1{
    0x494E'5354'5344'5031ULL};
inline constexpr 稳定编码 安全根生产定义来源_v1{
    0x494E'5354'5352'4331ULL};

static_assert(安全根生产低位阈值_v1
    == 本能根I64最大目标_v1 / 10 * 3
        + (本能根I64最大目标_v1 % 10) * 3 / 10);
static_assert(安全根生产高位阈值_v1
    == 本能根I64最大目标_v1 / 10 * 8
        + (本能根I64最大目标_v1 % 10) * 8 / 10);
static_assert(安全根生产低位阈值_v1 > 1
    && 安全根生产低位阈值_v1 < 安全根生产高位阈值_v1
    && 安全根生产高位阈值_v1 <= 本能根I64最大目标_v1);

struct 安全根生产定义初始化请求_v1 final {
    std::uint32_t 合同版本 = 安全根生产定义初始化合同版本_v1;
    本能根运行锚点_v1 本能根锚点;
    friend bool operator==(const 安全根生产定义初始化请求_v1&,
        const 安全根生产定义初始化请求_v1&) = default;
};

enum class 安全根生产定义初始化状态_v1 : std::uint8_t {
    已发布 = 1,
    已恢复 = 2,
    入口拒绝 = 3,
    当前性漂移 = 4,
    定义冲突 = 5,
    许可拒绝 = 6,
    资源失败 = 7,
    内部错误 = 8,
    已可能发布 = 9
};

inline bool 安全根生产定义匹配_v1(
    const 安全根定义事实_v1& 定义,
    const 本能根运行锚点_v1& 锚点) noexcept {
    return 锚点.完整() && 安全根定义事实完整_v1(定义)
        && 定义.自我 == 锚点.唯一自我
        && 定义.安全根特征定义 == 锚点.安全根特征定义
        && 定义.安全根特征实例 == 锚点.安全根特征实例
        && 定义.低位阈值 == 安全根生产低位阈值_v1
        && 定义.高位阈值 == 安全根生产高位阈值_v1
        && 定义.定义版本 == 安全根生产定义版本_v1
        && 定义.主动安全结算规则版本 == 主动安全结算规则版本_v1
        && 定义.被动服务维护规则版本 == 被动服务维护规则版本_v1
        && 定义.来源稳定编码 == 安全根生产定义来源_v1;
}

struct 安全根生产定义初始化结果_v1 final {
    std::uint32_t 合同版本 = 安全根生产定义初始化合同版本_v1;
    安全根生产定义初始化状态_v1 状态 =
        安全根生产定义初始化状态_v1::入口拒绝;
    std::optional<安全根定义事实_v1> 定义;
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return 合同版本 == 安全根生产定义初始化合同版本_v1
            && (状态 == 安全根生产定义初始化状态_v1::已发布
                || 状态 == 安全根生产定义初始化状态_v1::已恢复)
            && 定义 && 安全根定义事实完整_v1(*定义)
            && 定义->低位阈值 == 安全根生产低位阈值_v1
            && 定义->高位阈值 == 安全根生产高位阈值_v1
            && 定义->定义版本 == 安全根生产定义版本_v1
            && 定义->主动安全结算规则版本
                == 主动安全结算规则版本_v1
            && 定义->被动服务维护规则版本
                == 被动服务维护规则版本_v1
            && 定义->来源稳定编码 == 安全根生产定义来源_v1
            && 本次正式读回截止 != 0
            && 定义->形成事实代次 <= 本次正式读回截止;
    }

    friend bool operator==(const 安全根生产定义初始化结果_v1&,
        const 安全根生产定义初始化结果_v1&) = default;
};

} // namespace 海中鱼巣
