#pragma once

#ifndef L2_TASK_REALITY_HARD_DENIAL_RULE_STRUCTURE_NO_INCLUDES
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include "../L2任务结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2任务现实执行硬否决规则结构专属合同版本_v2 = 2;

struct L2任务现实执行硬否决规则宇宙身份 final {
    稳定编码 值;
    friend bool operator==(const L2任务现实执行硬否决规则宇宙身份&,
        const L2任务现实执行硬否决规则宇宙身份&) = default;
};

struct L2任务现实执行硬否决规则语义身份 final {
    稳定编码 值;
    friend bool operator==(const L2任务现实执行硬否决规则语义身份&,
        const L2任务现实执行硬否决规则语义身份&) = default;
};

struct L2任务现实执行硬否决规则事实身份 final {
    稳定编码 值;
    friend bool operator==(const L2任务现实执行硬否决规则事实身份&,
        const L2任务现实执行硬否决规则事实身份&) = default;
};

enum class L2硬否决约束模式 : std::uint8_t {
    任意 = 1,
    精确集合 = 2
};

struct L2硬否决self约束_v2 final {
    L2硬否决约束模式 模式 = L2硬否决约束模式::任意;
    std::vector<L2存在身份> 成员;
    friend bool operator==(const L2硬否决self约束_v2&,
        const L2硬否决self约束_v2&) = default;
};

struct L2硬否决场景约束_v2 final {
    L2硬否决约束模式 模式 = L2硬否决约束模式::任意;
    std::vector<L2场景身份> 成员;
    friend bool operator==(const L2硬否决场景约束_v2&,
        const L2硬否决场景约束_v2&) = default;
};

struct L2硬否决动作作用范围约束_v2 final {
    std::vector<任务动作作用范围> 包含范围;
    friend bool operator==(const L2硬否决动作作用范围约束_v2&,
        const L2硬否决动作作用范围约束_v2&) = default;
};

struct L2任务现实执行硬否决规则定义材料_v2 final {
    L2任务现实执行硬否决规则语义身份 语义身份;
    L2硬否决self约束_v2 self约束;
    L2硬否决场景约束_v2 场景约束;
    L2硬否决动作作用范围约束_v2 动作作用范围约束;
    std::vector<稳定编码> 归一物理来源事实组;
    friend bool operator==(const L2任务现实执行硬否决规则定义材料_v2&,
        const L2任务现实执行硬否决规则定义材料_v2&) = default;
};

struct L2任务现实执行硬否决规则宇宙事实_v2 final {
    L2任务现实执行硬否决规则宇宙身份 宇宙;
    std::uint64_t 规则版本 = 0;
    std::uint64_t 成员集合版本 = 0;
    std::vector<L2任务现实执行硬否决规则语义身份> 规范成员语义身份组;
    L2生命周期 生命周期;
    friend bool operator==(const L2任务现实执行硬否决规则宇宙事实_v2&,
        const L2任务现实执行硬否决规则宇宙事实_v2&) = default;
};

struct L2任务现实执行硬否决规则事实_v2 final {
    L2任务现实执行硬否决规则事实身份 身份;
    L2任务现实执行硬否决规则宇宙身份 宇宙;
    std::uint64_t 规则版本 = 0;
    std::uint64_t 成员集合版本 = 0;
    L2任务现实执行硬否决规则定义材料_v2 定义;
    L2生命周期 生命周期;
    friend bool operator==(const L2任务现实执行硬否决规则事实_v2&,
        const L2任务现实执行硬否决规则事实_v2&) = default;
};

struct L2任务现实执行硬否决规则覆盖项_v2 final {
    L2任务现实执行硬否决规则语义身份 规则语义身份;
    L2任务现实执行硬否决规则事实身份 规则事实身份;
    bool 规则版本一致 = false;
    bool Gnow生命周期有效 = false;
    bool self匹配 = false;
    bool 场景匹配 = false;
    bool 动作作用范围包含 = false;

    bool 适用() const noexcept {
        return 规则版本一致 && Gnow生命周期有效 && self匹配
            && 场景匹配 && 动作作用范围包含;
    }

    friend bool operator==(const L2任务现实执行硬否决规则覆盖项_v2&,
        const L2任务现实执行硬否决规则覆盖项_v2&) = default;
};

struct L2任务现实执行硬否决规则完整集合见证_v2 final {
    L2任务现实执行硬否决规则宇宙身份 宇宙;
    std::uint64_t 规则版本 = 0;
    std::uint64_t 成员集合版本 = 0;
    std::uint64_t 声明成员数 = 0;
    std::vector<L2任务现实执行硬否决规则覆盖项_v2> 覆盖项组;
    std::uint64_t Gnow = 0;

    bool 完整() const noexcept {
        if (!有效(宇宙.值) || 规则版本 == 0 || 成员集合版本 == 0
            || Gnow == 0 || 声明成员数 != 覆盖项组.size())
            return false;
        std::uint64_t 前语义 = 0;
        for (const auto& 项 : 覆盖项组) {
            if (!有效(项.规则语义身份.值) || !有效(项.规则事实身份.值)
                || 项.规则语义身份.值.值 <= 前语义)
                return false;
            前语义 = 项.规则语义身份.值.值;
        }
        return true;
    }

    friend bool operator==(const L2任务现实执行硬否决规则完整集合见证_v2&,
        const L2任务现实执行硬否决规则完整集合见证_v2&) = default;
};

enum class L2任务现实执行硬否决规则版本切换原因_v2 : std::uint8_t {
    首次发布 = 1,
    撤销 = 2,
    失效 = 3,
    替代 = 4,
    组合修订 = 5
};

enum class L2任务现实执行硬否决规则宇宙发布状态_v2 : std::uint8_t {
    已发布 = 1,
    精确重复 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    当前性漂移 = 5,
    版本漂移 = 6,
    幂等冲突 = 7,
    规则集合不闭合 = 8,
    引用冲突 = 9,
    资源失败 = 10,
    内部错误 = 11,
    已可能发布 = 12
};

struct L2任务现实执行硬否决规则宇宙发布请求_v2 final {
    std::uint32_t 合同版本 = L2任务现实执行硬否决规则结构专属合同版本_v2;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    std::uint64_t 新规则版本 = 0;
    std::uint64_t 新成员集合版本 = 0;
    L2任务现实执行硬否决规则版本切换原因_v2 切换原因 =
        L2任务现实执行硬否决规则版本切换原因_v2::首次发布;
    稳定编码 切换依据事件;
    std::vector<L2任务现实执行硬否决规则定义材料_v2> 完整规则组;
    friend bool operator==(const L2任务现实执行硬否决规则宇宙发布请求_v2&,
        const L2任务现实执行硬否决规则宇宙发布请求_v2&) = default;
};

struct L2任务现实执行硬否决规则宇宙提交见证_v2 final {
    L1结构所有者身份 所有者;
    L2结构幂等身份 幂等身份;
    std::uint64_t 首次提交事实代次 = 0;
    std::uint64_t 新规则版本 = 0;
    friend bool operator==(const L2任务现实执行硬否决规则宇宙提交见证_v2&,
        const L2任务现实执行硬否决规则宇宙提交见证_v2&) = default;
};

struct L2任务现实执行硬否决规则宇宙发布结果_v2 final {
    L2任务现实执行硬否决规则宇宙发布状态_v2 状态 =
        L2任务现实执行硬否决规则宇宙发布状态_v2::内部错误;
    std::optional<L2任务现实执行硬否决规则宇宙事实_v2> 宇宙;
    std::vector<L2任务现实执行硬否决规则事实_v2> 完整规则组;
    std::optional<L2任务现实执行硬否决规则宇宙提交见证_v2> 提交见证;
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        return (状态 == L2任务现实执行硬否决规则宇宙发布状态_v2::已发布
                || 状态 == L2任务现实执行硬否决规则宇宙发布状态_v2::精确重复)
            && 宇宙 && !提交见证 && 本次正式读回截止 != 0
            && 宇宙->规则版本 != 0 && 宇宙->成员集合版本 != 0
            && 宇宙->规范成员语义身份组.size() == 完整规则组.size();
    }

    friend bool operator==(const L2任务现实执行硬否决规则宇宙发布结果_v2&,
        const L2任务现实执行硬否决规则宇宙发布结果_v2&) = default;
};

enum class L2任务现实执行硬否决规则全集读取状态_v2 : std::uint8_t {
    已读取 = 1,
    未发布 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    当前性漂移 = 5,
    场景范围不闭合 = 6,
    规则宇宙不闭合 = 7,
    适用性未裁定 = 8,
    引用冲突 = 9,
    资源失败 = 10,
    内部错误 = 11
};

struct L2任务现实执行硬否决规则全集读取请求_v2 final {
    std::uint32_t 合同版本 = L2任务现实执行硬否决规则结构专属合同版本_v2;
    L2结构请求头 请求头;
    L2存在身份 自我;
    std::vector<任务冻结动作项> 完整有序冻结动作范围;
    friend bool operator==(const L2任务现实执行硬否决规则全集读取请求_v2&,
        const L2任务现实执行硬否决规则全集读取请求_v2&) = default;
};

struct L2任务现实执行硬否决规则全集读取结果_v2 final {
    L2任务现实执行硬否决规则全集读取状态_v2 状态 =
        L2任务现实执行硬否决规则全集读取状态_v2::内部错误;
    std::optional<L2任务现实执行硬否决规则宇宙事实_v2> 宇宙;
    std::vector<L2任务现实执行硬否决规则事实_v2> 完整规则组;
    std::vector<L2任务现实执行硬否决规则事实身份> 适用规则事实身份组;
    std::optional<L2任务现实执行硬否决规则完整集合见证_v2> 完整集合见证;
    std::uint64_t 本次正式读回截止 = 0;

    bool 成功() const noexcept {
        if (状态 != L2任务现实执行硬否决规则全集读取状态_v2::已读取
            || !宇宙 || !完整集合见证 || 本次正式读回截止 == 0
            || 本次正式读回截止 != 完整集合见证->Gnow
            || !完整集合见证->完整()
            || 完整集合见证->宇宙 != 宇宙->宇宙
            || 完整集合见证->规则版本 != 宇宙->规则版本
            || 完整集合见证->成员集合版本 != 宇宙->成员集合版本
            || 完整集合见证->声明成员数 != 完整规则组.size())
            return false;
        std::size_t 适用数 = 0;
        for (const auto& 项 : 完整集合见证->覆盖项组)
            if (项.适用()) ++适用数;
        return 适用数 == 适用规则事实身份组.size();
    }

    friend bool operator==(const L2任务现实执行硬否决规则全集读取结果_v2&,
        const L2任务现实执行硬否决规则全集读取结果_v2&) = default;
};

} // namespace 海中鱼巣
