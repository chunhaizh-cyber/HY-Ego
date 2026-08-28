#pragma once

#ifndef INSTINCT_ROOT_RUNTIME_INIT_NO_INCLUDES
#include <cstdint>
#include <optional>
#include "../领域/L2存在结构.数据.h"
#include "../领域/L2特征结构.数据.h"
#include "../领域/L2状态结构.数据.h"
#include "../领域/L2需求结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 本能根运行锚点合同版本_v1 = 1;
inline constexpr std::int64_t 本能根I64最大目标_v1 = 9223372036854775807LL;
inline constexpr std::int64_t 本能根初始值_v1 = 4611686018427387903LL;

struct 本能根运行锚点_v1 final {
    std::uint32_t 合同版本 = 本能根运行锚点合同版本_v1;
    L2存在身份 唯一自我;

    L2需求身份 安全根需求;
    L2需求列表项身份 安全根列表项;
    L2特征定义身份 安全根特征定义;
    L2特征实例身份 安全根特征实例;
    L2目标状态合同身份 安全根目标合同;
    L2特征定义身份 安全差值二次特征概念;

    L2需求身份 服务根需求;
    L2需求列表项身份 服务根列表项;
    L2特征定义身份 服务根特征定义;
    L2特征实例身份 服务根特征实例;
    L2目标状态合同身份 服务根目标合同;
    L2特征定义身份 服务差值二次特征概念;

    std::uint64_t 事实截止代次 = 0;

    bool 完整() const noexcept {
        return 合同版本 == 本能根运行锚点合同版本_v1
            && 有效(唯一自我.值)
            && 有效(安全根需求.值) && 有效(安全根列表项.值)
            && 有效(安全根特征定义.值) && 有效(安全根特征实例.值)
            && 有效(安全根目标合同.值) && 有效(安全差值二次特征概念.值)
            && 有效(服务根需求.值) && 有效(服务根列表项.值)
            && 有效(服务根特征定义.值) && 有效(服务根特征实例.值)
            && 有效(服务根目标合同.值) && 有效(服务差值二次特征概念.值)
            && 事实截止代次 != 0
            && 安全根需求 != 服务根需求
            && 安全根列表项 != 服务根列表项
            && 安全根特征定义 != 服务根特征定义
            && 安全根特征实例 != 服务根特征实例
            && 安全根目标合同 != 服务根目标合同
            && 安全差值二次特征概念 != 服务差值二次特征概念;
    }

    friend bool operator==(const 本能根运行锚点_v1&,
        const 本能根运行锚点_v1&) = default;
};

struct 本能根差值二次特征值_v1 final {
    L2特征定义身份 概念;
    std::int64_t 值 = 0;
    friend bool operator==(const 本能根差值二次特征值_v1&,
        const 本能根差值二次特征值_v1&) = default;
};

inline std::optional<本能根差值二次特征值_v1>
形成本能根差值二次特征值_v1(
    L2特征定义身份 概念, std::int64_t 当前值) noexcept {
    if (!有效(概念.值) || 当前值 < 0 || 当前值 > 本能根I64最大目标_v1)
        return std::nullopt;
    return 本能根差值二次特征值_v1{
        概念, 本能根I64最大目标_v1 - 当前值};
}

inline constexpr std::uint32_t 本能根运行初始化合同版本_v1 = 1;

struct 本能根运行初始化请求_v1 final {
    std::uint32_t 合同版本 = 本能根运行初始化合同版本_v1;
    L2存在身份 唯一自我;
    std::uint64_t 本次启动UTC纳秒 = 0;
    friend bool operator==(const 本能根运行初始化请求_v1&,
        const 本能根运行初始化请求_v1&) = default;
};

enum class 本能根运行初始化状态_v1 : std::uint8_t {
    已形成 = 1,
    已恢复 = 2,
    入口拒绝 = 3,
    当前性漂移 = 4,
    幂等冲突 = 5,
    引用冲突 = 6,
    材料未闭合 = 7,
    角色已退出 = 8,
    已可能发布 = 9,
    资源失败 = 10,
    内部不一致 = 11
};

struct 本能根运行初始化结果_v1 final {
    std::uint32_t 合同版本 = 本能根运行初始化合同版本_v1;
    本能根运行初始化状态_v1 状态 =
        本能根运行初始化状态_v1::内部不一致;
    std::optional<本能根运行锚点_v1> 锚点;

    bool 成功() const noexcept {
        return 合同版本 == 本能根运行初始化合同版本_v1
            && (状态 == 本能根运行初始化状态_v1::已形成
                || 状态 == 本能根运行初始化状态_v1::已恢复)
            && 锚点 && 锚点->完整();
    }

    friend bool operator==(const 本能根运行初始化结果_v1&,
        const 本能根运行初始化结果_v1&) = default;
};

} // namespace 海中鱼巣
