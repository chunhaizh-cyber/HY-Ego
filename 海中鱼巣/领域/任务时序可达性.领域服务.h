#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

enum class 需求时间硬度 : std::uint8_t {
    硬约束 = 1,
    软约束 = 2
};

enum class 转换候选前置状态 : std::uint8_t {
    已通过 = 1,
    安全硬门否决,
    方法保护约束不兼容,
    材料不足
};

enum class 需求时序顺序 : std::uint8_t {
    第一后第二 = 1,
    第二后第一
};

enum class 时序路线状态 : std::uint8_t {
    可保证同时满足 = 1,
    仅按预计耗时可同时满足,
    不可同时满足,
    转换合同缺失,
    前置材料不足,
    安全硬门否决,
    方法保护约束不兼容,
    计算不可表示
};

enum class 需求时序裁决状态 : std::uint8_t {
    可保证同时满足 = 1,
    仅存在风险路线,
    时序冲突选择第一需求,
    时序冲突选择第二需求,
    时序冲突无法裁决,
    两个需求均不可保证,
    材料不足,
    输入不合法,
    计算不可表示
};

struct 需求时序约束投影 final {
    稳定编码 需求节点;
    稳定编码 目标位置节点;
    std::int64_t 最早满足纳秒 = 0;
    std::int64_t 最晚满足纳秒 = 0;
    std::int64_t 最小保持纳秒 = 0;
    需求时间硬度 时间硬度 = 需求时间硬度::硬约束;
    bool 安全硬约束 = false;
    std::int64_t 错过损失 = 0;
    std::int64_t 需求权重 = 0;
    friend bool operator==(const 需求时序约束投影&,
        const 需求时序约束投影&) = default;
};

struct 转换时间合同投影 final {
    稳定编码 方法节点;
    稳定编码 起点位置节点;
    稳定编码 终点位置节点;
    std::int64_t 最短耗时纳秒 = 0;
    std::int64_t 预计耗时纳秒 = 0;
    std::int64_t 保证最大耗时纳秒 = 0;
    转换候选前置状态 前置状态 = 转换候选前置状态::材料不足;
    friend bool operator==(const 转换时间合同投影&,
        const 转换时间合同投影&) = default;
};

struct 需求时序可达性裁决请求 final {
    std::int64_t 当前绝对时间纳秒 = 0;
    稳定编码 当前位置节点;
    需求时序约束投影 第一需求;
    需求时序约束投影 第二需求;
    std::vector<转换时间合同投影> 转换候选组;
};

struct 时序转换选择 final {
    稳定编码 起点位置节点;
    稳定编码 终点位置节点;
    std::optional<稳定编码> 方法节点;
    std::int64_t 采用耗时纳秒 = 0;
};

struct 时序路线评估 final {
    需求时序顺序 顺序 = 需求时序顺序::第一后第二;
    时序路线状态 状态 = 时序路线状态::不可同时满足;
    std::vector<时序转换选择> 保证路线;
    std::vector<时序转换选择> 预计路线;
    std::optional<std::int64_t> 第一需求满足时刻;
    std::optional<std::int64_t> 第二需求满足时刻;
    std::optional<std::int64_t> 最小窗口余量纳秒;
};

struct 需求时序可达性裁决结果 final {
    需求时序裁决状态 状态 = 需求时序裁决状态::输入不合法;
    时序路线评估 第一后第二;
    时序路线评估 第二后第一;
    std::optional<需求时序顺序> 采用顺序;
    std::optional<稳定编码> 优先需求节点;
    std::optional<稳定编码> 等待重新确认需求节点;
};

class 任务时序可达性裁决器 final {
public:
    static 需求时序可达性裁决结果 裁决(
        const 需求时序可达性裁决请求& 请求) noexcept;
};

} // namespace 海中鱼巣
