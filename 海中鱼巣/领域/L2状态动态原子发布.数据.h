#pragma once

#ifndef L2_STATE_DYNAMIC_ATOMIC_PUBLISH_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "L2结构公共.数据.h"
#include "L2特征结构.数据.h"
#include "L2状态结构.数据.h"
#include "L2动态结构.数据.h"
#include "../核心/L1所有者范围CRUD.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2状态动态原子发布合同版本 = 1;
inline constexpr std::uint32_t L2状态动态迁移专属合同版本_v2 = 2;
inline constexpr std::uint64_t L2状态动态原子发布幂等域 = 0xA2C0'0201ULL;
inline constexpr std::uint32_t L2特征当前值变化记录合同版本_v1 = 1;
inline constexpr 稳定编码 L2特征当前值记录状态角色_v1{
    0x4E53'4355'5252'454EULL};

enum class L2特征当前值变化记录状态_v1 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    无变化 = 3,
    入口拒绝 = 4,
    当前性漂移 = 5,
    幂等冲突 = 6,
    材料不足 = 7,
    引用冲突 = 8,
    资源失败 = 9,
    内部错误 = 10,
    已可能发布 = 11
};

struct L2特征当前值变化记录请求_v1 final {
    std::uint32_t 合同版本 = L2特征当前值变化记录合同版本_v1;
    L2结构请求头 请求头{};
    L2结构幂等身份 组合幂等身份{};
    L2结构幂等身份 特征参与者幂等身份{};
    L2结构幂等身份 状态参与者幂等身份{};
    L2结构幂等身份 动态参与者幂等身份{};
    L2特征实例身份 特征实例{};
    L2特征值事实 预期旧当前值{稳定编码{},
        L2特征实例身份{稳定编码{}},
        L2原始值材料{std::int64_t{0}}, 稳定编码{}, L2生命周期{}};
    L2原始值材料 新值材料{};
    std::int64_t 变化UTC纳秒 = 0;
    L2存在身份 主体存在{};
    L2场景身份 共同场景{};
    L2存在身份 来源存在{};
    std::optional<L2方法身份> 来源方法{};
    friend bool operator==(const L2特征当前值变化记录请求_v1&,
        const L2特征当前值变化记录请求_v1&) = default;
};

struct L2特征当前值变化记录结果_v1 final {
    std::uint32_t 合同版本 = L2特征当前值变化记录合同版本_v1;
    L2特征当前值变化记录状态_v1 状态 =
        L2特征当前值变化记录状态_v1::入口拒绝;
    L2结构幂等身份 组合幂等身份{};
    L2结构幂等身份 特征参与者幂等身份{};
    L2结构幂等身份 状态参与者幂等身份{};
    L2结构幂等身份 动态参与者幂等身份{};
    std::optional<L2特征值事实> 当前值{};
    std::optional<L2中性状态事实_v1> 前状态{};
    std::optional<L2中性状态事实_v1> 后状态{};
    std::optional<L2中性状态引用事实_v1> 当前选择{};
    std::optional<L2中性状态引用事实_v1> 已退出旧选择{};
    std::optional<L2中性动态事实_v1> 状态迁移动能{};
    std::optional<L2中性动态事实_v1> 动作致变动态{};
    std::vector<L2中性状态上下文事实_v1> 状态上下文组{};
    std::vector<L2中性动态上下文事实_v1> 动态上下文组{};
    bool legacy基线兼容 = false;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征当前值变化记录结果_v1&,
        const L2特征当前值变化记录结果_v1&) = default;
};

struct L2特征当前值变化记录读取请求_v1 final {
    std::uint32_t 合同版本 = L2特征当前值变化记录合同版本_v1;
    L2结构请求头 请求头{};
    L2特征实例身份 特征实例{};
    稳定编码 新当前值稳定编码{};
    L2中性状态引用身份_v1 新当前选择{};
    L2状态身份 前状态{};
    L2状态身份 后状态{};
    L2动态身份 状态迁移动能{};
    std::optional<L2动态身份> 动作致变动态{};
    std::uint64_t 首次事实代次 = 0;
    friend bool operator==(const L2特征当前值变化记录读取请求_v1&,
        const L2特征当前值变化记录读取请求_v1&) = default;
};

enum class L2特征当前值变化记录读取状态_v1 : std::uint8_t {
    已读取 = 1,
    入口拒绝 = 2,
    当前性漂移 = 3,
    材料不足 = 4,
    引用冲突 = 5,
    资源失败 = 6,
    内部错误 = 7
};

struct L2特征当前值变化记录读取结果_v1 final {
    std::uint32_t 合同版本 = L2特征当前值变化记录合同版本_v1;
    L2特征当前值变化记录读取状态_v1 状态 =
        L2特征当前值变化记录读取状态_v1::入口拒绝;
    std::optional<L2特征值事实> 当前值{};
    std::optional<L2中性状态事实_v1> 前状态{};
    std::optional<L2中性状态事实_v1> 后状态{};
    std::optional<L2中性状态引用事实_v1> 当前选择{};
    std::optional<L2中性动态事实_v1> 状态迁移动能{};
    std::optional<L2中性动态事实_v1> 动作致变动态{};
    std::vector<L2中性状态上下文事实_v1> 状态上下文组{};
    std::vector<L2中性动态上下文事实_v1> 动态上下文组{};
    bool legacy基线兼容 = false;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征当前值变化记录读取结果_v1&,
        const L2特征当前值变化记录读取结果_v1&) = default;
};

inline constexpr L1所有者范围写入幂等身份
    L2特征当前值变化账登记写入身份_v2{
        L2特征当前值变化账登记写入身份值_v2};

enum class L2特征当前值变化账登记状态_v2 : std::uint8_t {
    已登记 = 1,
    精确重复 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    当前性漂移 = 5,
    幂等冲突 = 6,
    引用冲突 = 7,
    资源失败 = 8,
    内部错误 = 9,
    已可能登记 = 10
};

struct L2特征当前值变化账登记请求_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2结构请求头 请求头{};
    friend bool operator==(const L2特征当前值变化账登记请求_v2&,
        const L2特征当前值变化账登记请求_v2&) = default;
};

struct L2特征当前值变化账登记结果_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2特征当前值变化账登记状态_v2 状态 =
        L2特征当前值变化账登记状态_v2::入口拒绝;
    std::optional<稳定编码> 账锚点{};
    std::uint64_t 首次登记事实代次 = 0;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征当前值变化账登记结果_v2&,
        const L2特征当前值变化账登记结果_v2&) = default;
};

using L2特征当前值变化记录状态_v2 = L2特征当前值变化记录状态_v1;

struct L2特征当前值变化记录请求_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2结构请求头 请求头{};
    L2结构幂等身份 组合幂等身份{};
    L2结构幂等身份 特征参与者幂等身份{};
    L2结构幂等身份 状态参与者幂等身份{};
    L2结构幂等身份 动态参与者幂等身份{};
    L2特征实例身份 特征实例{};
    L2特征值事实 预期旧当前值{稳定编码{},
        L2特征实例身份{稳定编码{}},
        L2原始值材料{std::int64_t{0}}, 稳定编码{}, L2生命周期{}};
    L2原始值材料 新值材料{};
    std::int64_t 变化UTC纳秒 = 0;
    L2存在身份 主体存在{};
    L2场景身份 共同场景{};
    L2存在身份 来源存在{};
    std::optional<L2方法身份> 来源方法{};
    friend bool operator==(const L2特征当前值变化记录请求_v2&,
        const L2特征当前值变化记录请求_v2&) = default;
};

struct L2特征当前值变化记录结果_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2特征当前值变化记录状态_v2 状态 =
        L2特征当前值变化记录状态_v2::入口拒绝;
    L2结构幂等身份 组合幂等身份{};
    L2结构幂等身份 特征参与者幂等身份{};
    L2结构幂等身份 状态参与者幂等身份{};
    L2结构幂等身份 动态参与者幂等身份{};
    std::optional<L2特征值事实> 当前值{};
    std::optional<L2中性状态事实_v1> 前状态{};
    std::optional<L2中性状态事实_v1> 后状态{};
    std::optional<L2中性状态引用事实_v1> 当前选择{};
    std::optional<L2中性状态引用事实_v1> 已退出旧选择{};
    std::optional<L2中性动态事实_v1> 状态迁移动能{};
    std::optional<L2中性动态事实_v1> 动作致变动态{};
    std::vector<L2中性状态上下文事实_v1> 状态上下文组{};
    std::vector<L2中性动态上下文事实_v1> 动态上下文组{};
    std::optional<L2特征当前值变化账事实_v2> 变化账事实{};
    bool legacy基线兼容 = false;
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征当前值变化记录结果_v2&,
        const L2特征当前值变化记录结果_v2&) = default;
};

struct L2特征当前值变化组读取请求_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2结构请求头 请求头{};
    L2特征实例身份 特征实例{};
    std::int64_t 包含起始UTC纳秒 = 0;
    std::int64_t 排除结束UTC纳秒 = 0;
    std::uint64_t 数量预算 = 0;
    friend bool operator==(const L2特征当前值变化组读取请求_v2&,
        const L2特征当前值变化组读取请求_v2&) = default;
};

struct L2特征当前值变化完整集合见证_v2 final {
    std::uint64_t 账登记事实代次 = 0;
    L2特征实例身份 特征实例{};
    std::int64_t 包含起始UTC纳秒 = 0;
    std::int64_t 排除结束UTC纳秒 = 0;
    std::uint64_t 声明成员数 = 0;
    std::vector<L2特征当前值变化账身份_v2> 规范成员身份组{};
    std::uint64_t G0 = 0;
    friend bool operator==(const L2特征当前值变化完整集合见证_v2&,
        const L2特征当前值变化完整集合见证_v2&) = default;
};

enum class L2特征当前值变化组读取状态_v2 : std::uint8_t {
    已读取 = 1,
    账未登记 = 2,
    入口拒绝 = 3,
    当前性漂移 = 4,
    数量预算不足 = 5,
    集合不闭合 = 6,
    引用冲突 = 7,
    资源失败 = 8,
    内部错误 = 9
};

struct L2特征当前值变化组读取结果_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2特征当前值变化组读取状态_v2 状态 =
        L2特征当前值变化组读取状态_v2::入口拒绝;
    std::vector<L2特征当前值变化账事实_v2> 完整变化组{};
    std::optional<L2特征当前值变化完整集合见证_v2> 完整集合见证{};
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征当前值变化组读取结果_v2&,
        const L2特征当前值变化组读取结果_v2&) = default;
};

struct L2特征当前值变化事实代次组读取请求_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2结构请求头 请求头{};
    L2特征实例身份 特征实例{};
    std::uint64_t 排除起始事实代次 = 0;
    std::uint64_t 数量预算 = 0;
    friend bool operator==(const L2特征当前值变化事实代次组读取请求_v2&,
        const L2特征当前值变化事实代次组读取请求_v2&) = default;
};

struct L2特征当前值变化事实代次完整集合见证_v2 final {
    std::uint64_t 账登记事实代次 = 0;
    L2特征实例身份 特征实例{};
    std::uint64_t 排除起始事实代次 = 0;
    std::uint64_t 包含结束事实代次 = 0;
    std::uint64_t 声明成员数 = 0;
    std::vector<L2特征当前值变化账身份_v2> 规范成员身份组{};
    std::uint64_t G0 = 0;
    friend bool operator==(
        const L2特征当前值变化事实代次完整集合见证_v2&,
        const L2特征当前值变化事实代次完整集合见证_v2&) = default;
};

enum class L2特征当前值变化事实代次组读取状态_v2 : std::uint8_t {
    已读取 = 1,
    账未登记 = 2,
    入口拒绝 = 3,
    当前性漂移 = 4,
    覆盖边界不可用 = 5,
    数量预算不足 = 6,
    集合不闭合 = 7,
    引用冲突 = 8,
    资源失败 = 9,
    内部错误 = 10
};

struct L2特征当前值变化事实代次组读取结果_v2 final {
    std::uint32_t 合同版本 = L2特征当前值变化永久账合同版本_v2;
    L2特征当前值变化事实代次组读取状态_v2 状态 =
        L2特征当前值变化事实代次组读取状态_v2::入口拒绝;
    std::vector<L2特征当前值变化账事实_v2> 完整变化组{};
    std::optional<L2特征当前值变化事实代次完整集合见证_v2>
        完整集合见证{};
    std::uint64_t 本次正式读回截止 = 0;
    bool 成功() const noexcept;
    friend bool operator==(const L2特征当前值变化事实代次组读取结果_v2&,
        const L2特征当前值变化事实代次组读取结果_v2&) = default;
};

// 以下旧原子发布 DTO 仅作源兼容和历史诊断；生产入口不再新增事实。

enum class L2状态动态原子发布状态 : std::uint8_t {
    已提交 = 1, 精确重复 = 2, 入口拒绝 = 3, 许可拒绝 = 4,
    未找到 = 5, 已退出 = 6, 事实代次漂移 = 7, 幂等冲突 = 8,
    引用冲突 = 9, 资源失败 = 10, 内部不一致 = 11
};

struct L2状态动态原子发布请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 主体存在;
    L2状态身份 旧当前状态;
    L2特征实例身份 新状态特征实例;
    L2原始值材料 新状态精确值材料;
    L2原始值材料 新状态时间值材料;
    L2原始值材料 动态起始时间材料;
    L2原始值材料 动态结束时间材料;
    稳定编码 来源方法稳定编码;
    friend bool operator==(const L2状态动态原子发布请求&,
        const L2状态动态原子发布请求&) = default;
};

struct L2状态动态原子发布结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 新状态;
    std::optional<L2状态自有关系引用> 新状态自有关系;
    std::optional<L2动态事实> 新动态;
    std::optional<L2动态自有关系引用> 新动态自有关系;
    std::optional<L2状态自有关系退出投影> 已退出旧状态关系;
    friend bool operator==(const L2状态动态原子发布结果&,
        const L2状态动态原子发布结果&) = default;
};

enum class L2状态动态迁移操作状态_v2 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    待迁移 = 4,
    已有异义当前 = 5,
    已可能发布 = 6,
    入口拒绝 = 8,
    许可拒绝 = 9,
    事实代次漂移 = 10,
    幂等冲突 = 11,
    引用冲突 = 12,
    资源失败 = 13,
    内部不一致 = 14
};

struct L2状态动态迁移请求头_v2 final {
    L2结构请求头 共同请求头{};
    std::uint32_t 迁移专属合同版本 = L2状态动态迁移专属合同版本_v2;
    friend bool operator==(const L2状态动态迁移请求头_v2&,
        const L2状态动态迁移请求头_v2&) = default;
    bool 有效() const noexcept;
};

struct L2状态动态迁移请求_v2 final {
    L2状态动态迁移请求头_v2 请求头{};
    L2结构幂等身份 幂等身份{};
    L2存在身份 主体存在{};
    L2状态身份 旧当前状态{};
    L2特征实例身份 新状态特征实例{};
    L2原始值材料 新状态精确值材料{};
    L2原始值材料 新状态时间值材料{};
    L2原始值材料 动态起始时间材料{};
    L2原始值材料 动态结束时间材料{};
    稳定编码 来源方法稳定编码{};
    L2状态当前选择关系引用 旧当前选择{};
    friend bool operator==(const L2状态动态迁移请求_v2&,
        const L2状态动态迁移请求_v2&) = default;
    bool 有效() const noexcept;
};

struct L2状态动态迁移结果_v2 final {
    L2结构结果头 结果头{};
    std::uint32_t 原子发布合同版本 = L2状态动态迁移专属合同版本_v2;
    L2状态动态迁移操作状态_v2 迁移操作状态 =
        L2状态动态迁移操作状态_v2::入口拒绝;
    std::optional<L2状态事实> 新状态{};
    std::optional<L2状态自有关系引用> 新状态自有关系{};
    std::optional<L2动态事实> 新动态{};
    std::optional<L2动态自有关系引用> 新动态自有关系{};
    std::optional<L2状态当前选择关系引用> 新当前选择关系{};
    std::optional<L2状态当前选择关系引用> 已退出旧当前选择关系{};
    std::uint64_t 共同事实截止代次 = 0;
    friend bool operator==(const L2状态动态迁移结果_v2&,
        const L2状态动态迁移结果_v2&) = default;
    bool 成功() const noexcept;
};

inline bool L2状态动态原子发布请求有效(
    const L2状态动态原子发布请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.主体存在.值) && 有效(请求.旧当前状态.值)
        && 有效(请求.新状态特征实例.值)
        && 有效(请求.来源方法稳定编码);
}

inline bool L2状态动态迁移请求头_v2::有效() const noexcept {
    return L2结构请求头合同有效(共同请求头)
        && 共同请求头.期望事实代次 != 0
        && 迁移专属合同版本 == L2状态动态迁移专属合同版本_v2;
}

inline bool L2状态动态迁移请求_v2::有效() const noexcept {
    return 请求头.有效()
        && L2结构幂等身份有效(幂等身份)
        && 海中鱼巣::有效(主体存在.值)
        && 海中鱼巣::有效(旧当前状态.值)
        && 海中鱼巣::有效(新状态特征实例.值)
        && L2状态原始值材料有效(新状态精确值材料)
        && L2状态原始值材料有效(新状态时间值材料)
        && L2动态原始值材料有效(动态起始时间材料)
        && L2动态原始值材料有效(动态结束时间材料)
        && 动态起始时间材料.index() == 动态结束时间材料.index()
        && 海中鱼巣::有效(来源方法稳定编码)
        && L2状态当前选择关系引用完整(
            旧当前选择, 请求头.共同请求头.期望事实代次)
        && 旧当前选择.特征实例 == 新状态特征实例
        && 旧当前选择.状态 == 旧当前状态
        && !旧当前选择.生命周期.退出事实代次
        ;
}

} // namespace 海中鱼巣
