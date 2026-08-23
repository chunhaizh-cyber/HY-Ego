#pragma once

#ifndef L2_TASK_SUBGOAL_ACCEPTANCE_RECORD_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "L2任务结构.数据.h"
#include "L2需求结构.数据.h"
#include "L2普通方法结构.数据.h"
#include "L2状态结构.数据.h"
#include "L2特征结构.数据.h"
#include "L2动态结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2任务子目标承接记录合同版本 = 1;

struct L2任务子目标承接记录身份 final {
    稳定编码 值;
    explicit L2任务子目标承接记录身份(稳定编码 编码 = {}) noexcept
        : 值(编码) {}
    friend bool operator==(const L2任务子目标承接记录身份&,
        const L2任务子目标承接记录身份&) = default;
};

enum class L2任务子目标承接记录状态 : std::uint8_t {
    待绑定子需求 = 1,
    等待子需求 = 2,
    子需求已回流待父需求重判 = 3
};

struct L2任务子目标来源材料 final {
    L2方法身份 方法;
    L2方法内容版本 方法内容版本;
    L2方法规格版本 方法规格版本;
    L2方法结果身份 主轴结果;
    L2方法条件身份 条件;
    std::uint64_t 作用对象角色 = 0;
    稳定编码 条件来源稳定编码;
    friend bool operator==(const L2任务子目标来源材料&,
        const L2任务子目标来源材料&) = default;
};

struct L2任务子目标目标材料 final {
    L2存在身份 目标宿主;
    L2特征定义身份 目标特征;
    L2目标状态合同身份 目标状态合同;
    std::optional<L2场景身份> 条件场景;
    friend bool operator==(const L2任务子目标目标材料&,
        const L2任务子目标目标材料&) = default;
};

struct L2任务子目标回流材料 final {
    std::optional<L2任务身份> 子需求承接任务;
    L2状态身份 实际状态;
    std::optional<L2动态身份> 动态证据;
    std::int64_t 发生时间 = 0;
    std::uint64_t 回流版本 = 0;
    std::uint64_t 共同事实截止 = 0;
    friend bool operator==(const L2任务子目标回流材料&,
        const L2任务子目标回流材料&) = default;
};

struct L2任务子目标承接记录事实 final {
    L2任务子目标承接记录身份 身份;
    L2任务身份 任务;
    L2任务虚拟存在身份 任务虚拟存在;
    std::uint64_t 筹办轮次 = 0;
    L2需求身份 父需求;
    L2任务子目标来源材料 来源;
    L2任务子目标目标材料 子目标;
    std::uint64_t 来源共同事实截止 = 0;
    L2结构幂等身份 建立幂等身份;
    L2任务子目标承接记录状态 状态 =
        L2任务子目标承接记录状态::待绑定子需求;
    std::optional<L2需求身份> 子需求;
    std::optional<L2任务子目标回流材料> 回流;
    L2生命周期 生命周期;
    friend bool operator==(const L2任务子目标承接记录事实&,
        const L2任务子目标承接记录事实&) = default;
};

struct L2新增任务子目标承接记录请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务身份 任务;
    L2任务虚拟存在身份 任务虚拟存在;
    std::uint64_t 筹办轮次 = 0;
    L2需求身份 父需求;
    L2任务子目标来源材料 来源;
    L2任务子目标目标材料 子目标;
    std::uint64_t 来源共同事实截止 = 0;
    friend bool operator==(const L2新增任务子目标承接记录请求&,
        const L2新增任务子目标承接记录请求&) = default;
};

struct L2绑定任务子目标承接记录子需求请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务子目标承接记录身份 记录;
    L2需求身份 子需求;
    friend bool operator==(const L2绑定任务子目标承接记录子需求请求&,
        const L2绑定任务子目标承接记录子需求请求&) = default;
};

struct L2登记任务子目标承接记录回流请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务子目标承接记录身份 记录;
    L2需求身份 子需求;
    L2任务子目标回流材料 回流;
    friend bool operator==(const L2登记任务子目标承接记录回流请求&,
        const L2登记任务子目标承接记录回流请求&) = default;
};

struct L2退出任务子目标承接记录请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务子目标承接记录身份 记录;
    friend bool operator==(const L2退出任务子目标承接记录请求&,
        const L2退出任务子目标承接记录请求&) = default;
};

struct L2任务子目标承接记录写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务子目标承接记录事实> 记录;
    bool 成功() const noexcept {
        return (结果头.状态 == L2结构状态::已提交
                || 结果头.状态 == L2结构状态::精确重复)
            && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
            && 记录.has_value();
    }
    friend bool operator==(const L2任务子目标承接记录写入结果&,
        const L2任务子目标承接记录写入结果&) = default;
};

struct L2任务子目标承接记录读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务子目标承接记录身份 记录;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2任务子目标承接记录读取请求&,
        const L2任务子目标承接记录读取请求&) = default;
};

struct L2任务子目标承接记录读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2任务子目标承接记录事实> 记录;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2任务子目标承接记录读取结果&,
        const L2任务子目标承接记录读取结果&) = default;
};

struct L2按任务轮次读取子目标承接记录组请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2任务身份 任务;
    std::uint64_t 筹办轮次 = 0;
    std::uint64_t 历史截止事实代次 = 0;
    std::uint64_t 最大扫描记录数 = 0;
    std::uint64_t 最大返回记录数 = 0;
    friend bool operator==(const L2按任务轮次读取子目标承接记录组请求&,
        const L2按任务轮次读取子目标承接记录组请求&) = default;
};

struct L2按任务轮次读取子目标承接记录组结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::uint64_t 实际扫描记录数 = 0;
    std::vector<L2任务子目标承接记录事实> 记录组;
    bool 成功() const noexcept {
        return 结果头.状态 == L2结构状态::已读取
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次;
    }
    friend bool operator==(const L2按任务轮次读取子目标承接记录组结果&,
        const L2按任务轮次读取子目标承接记录组结果&) = default;
};

struct L2提交任务子目标回流后继筹办准备请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2任务身份 任务;
    L2任务子目标承接记录身份 回流记录;
    L2提交统一轮次推进材料 统一轮次推进;
    friend bool operator==(const L2提交任务子目标回流后继筹办准备请求&,
        const L2提交任务子目标回流后继筹办准备请求&) = default;
};

struct L2提交任务子目标回流后继筹办准备结果 final {
    L2结构结果头 结果头;
    std::optional<L2任务后继筹办准备记录事实> 后继准备记录;
    bool 成功() const noexcept {
        return (结果头.状态 == L2结构状态::已提交
                || 结果头.状态 == L2结构状态::精确重复)
            && 结果头.事实截止代次 != 0
            && 后继准备记录
            && L2任务后继筹办准备记录事实完整(*后继准备记录);
    }
    friend bool operator==(const L2提交任务子目标回流后继筹办准备结果&,
        const L2提交任务子目标回流后继筹办准备结果&) = default;
};

inline bool L2任务子目标来源材料有效(
    const L2任务子目标来源材料& 材料) noexcept {
    return 有效(材料.方法.值) && 材料.方法内容版本.值 != 0
        && 材料.方法规格版本.值 != 0 && 有效(材料.主轴结果.值)
        && 有效(材料.条件.值) && 材料.作用对象角色 != 0
        && 有效(材料.条件来源稳定编码);
}

inline bool L2任务子目标目标材料有效(
    const L2任务子目标目标材料& 材料) noexcept {
    return 有效(材料.目标宿主.值) && 有效(材料.目标特征.值)
        && 有效(材料.目标状态合同.值)
        && (!材料.条件场景 || 有效(材料.条件场景->值));
}

inline bool L2任务子目标回流材料有效(
    const L2任务子目标回流材料& 材料) noexcept {
    return (!材料.子需求承接任务 || 有效(材料.子需求承接任务->值))
        && 有效(材料.实际状态.值)
        && (!材料.动态证据 || 有效(材料.动态证据->值))
        && 材料.发生时间 != 0 && 材料.回流版本 != 0
        && 材料.共同事实截止 != 0;
}

inline bool L2新增任务子目标承接记录请求有效(
    const L2新增任务子目标承接记录请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.任务.值) && 有效(请求.任务虚拟存在.值)
        && 请求.筹办轮次 != 0 && 有效(请求.父需求.值)
        && L2任务子目标来源材料有效(请求.来源)
        && L2任务子目标目标材料有效(请求.子目标)
        && 请求.来源共同事实截止 == 请求.请求头.期望事实代次;
}

inline bool L2绑定任务子目标承接记录子需求请求有效(
    const L2绑定任务子目标承接记录子需求请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.记录.值) && 有效(请求.子需求.值);
}

inline bool L2登记任务子目标承接记录回流请求有效(
    const L2登记任务子目标承接记录回流请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.记录.值) && 有效(请求.子需求.值)
        && L2任务子目标回流材料有效(请求.回流)
        && 请求.回流.共同事实截止 == 请求.请求头.期望事实代次;
}

inline bool L2退出任务子目标承接记录请求有效(
    const L2退出任务子目标承接记录请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.记录.值);
}

inline bool L2任务子目标承接记录读取请求有效(
    const L2任务子目标承接记录读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.记录.值)
        && ((请求.读取类别 == L2读取类别::当前
                && 请求.历史截止事实代次 == 0)
            || (请求.读取类别 == L2读取类别::历史
                && 请求.历史截止事实代次 != 0));
}

inline bool L2按任务轮次读取子目标承接记录组请求有效(
    const L2按任务轮次读取子目标承接记录组请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.任务.值)
        && 请求.筹办轮次 != 0 && 请求.最大扫描记录数 != 0
        && 请求.最大返回记录数 != 0
        && ((请求.读取类别 == L2读取类别::当前
                && 请求.历史截止事实代次 == 0)
            || (请求.读取类别 == L2读取类别::历史
                && 请求.历史截止事实代次 != 0));
}

inline bool L2提交任务子目标回流后继筹办准备请求有效(
    const L2提交任务子目标回流后继筹办准备请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.任务.值) && 有效(请求.回流记录.值)
        && 请求.幂等身份.值 == 请求.回流记录.值.值
        && L2提交统一轮次推进材料完整(请求.统一轮次推进)
        && 请求.幂等身份 == 请求.统一轮次推进.准备幂等身份
        && 请求.幂等身份 == 请求.统一轮次推进.触发业务幂等身份
        && 请求.统一轮次推进.来源共同事实截止
            == 请求.请求头.期望事实代次;
}

static_assert(static_cast<std::uint8_t>(
    L2任务子目标承接记录状态::待绑定子需求) == 1);
static_assert(static_cast<std::uint8_t>(
    L2任务子目标承接记录状态::等待子需求) == 2);
static_assert(static_cast<std::uint8_t>(
    L2任务子目标承接记录状态::子需求已回流待父需求重判) == 3);
static_assert(static_cast<std::uint8_t>(L2结构状态::已提交) == 1);
static_assert(static_cast<std::uint8_t>(L2结构状态::数量预算不足) == 15);

} // namespace 海中鱼巣
