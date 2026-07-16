// 文件规则：动态模式算法只做不可变值的完整比较、确定排序和稳定哈希；不得读取结构、时间或线程状态。
module;

#include "../核心/句柄.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.算法.动态模式;

import 海中鱼巣.领域.材料.动态模式;
import 海中鱼巣.线程.任务结果回执协议;

export namespace 海中鱼巣 {

bool 动态模式回合身份相同(
    const 动态模式回合身份& 左,
    const 动态模式回合身份& 右) noexcept {
    return 任务结果回执材料相同(左.回执, 右.回执);
}

bool 动态模式回合锚点相同(
    const 动态模式回合身份& 左,
    const 动态模式回合身份& 右) noexcept {
    if (!左.回执.来源执行请求.has_value() || !右.回执.来源执行请求.has_value()) {
        return false;
    }
    const auto& 左请求 = 左.回执.来源执行请求->调度请求;
    const auto& 右请求 = 右.回执.来源执行请求->调度请求;
    return 左请求.请求编号 == 右请求.请求编号
        && 左.回执.回执编号 == 右.回执.回执编号
        && 左.回执.幂等键 == 右.回执.幂等键
        && 左.回执.来源任务序号 == 右.回执.来源任务序号
        && 左.回执.批次编号 == 右.回执.批次编号;
}

bool 动态模式结构键相同(
    const 动态模式结构键& 左,
    const 动态模式结构键& 右) noexcept {
    return 左.被改变目标 == 右.被改变目标
        && 左.前状态值 == 右.前状态值
        && 左.后状态值 == 右.后状态值
        && 左.有来源动作 == 右.有来源动作
        && 左.来源动作 == 右.来源动作
        && 左.规则版本 == 右.规则版本
        && 左.二次特征角色 == 右.二次特征角色;
}

namespace 动态模式算法内部 {

constexpr std::uint64_t 哈希偏移 = 1469598103934665603ULL;
constexpr std::uint64_t 哈希质数 = 1099511628211ULL;

void 混入(std::uint64_t& 哈希, std::uint64_t 值) noexcept {
    for (std::size_t 索引 = 0; 索引 < sizeof(值); ++索引) {
        哈希 ^= (值 >> (索引 * 8U)) & 0xffU;
        哈希 *= 哈希质数;
    }
}

void 混入(std::uint64_t& 哈希, const 节点句柄& 句柄) noexcept {
    混入(哈希, 句柄.仓库编号);
    混入(哈希, 句柄.节点编号);
    混入(哈希, 句柄.版本号);
}

auto 回合排序键(const 动态模式回合身份& 回合) noexcept {
    const auto& 回执 = 回合.回执;
    const auto 请求编号 = 回执.来源执行请求.has_value()
        ? 回执.来源执行请求->调度请求.请求编号 : 0;
    return std::tuple{
        请求编号, 回执.回执编号, 回执.幂等键,
        回执.来源任务序号, 回执.批次编号};
}

bool 句柄小于(const 节点句柄& 左, const 节点句柄& 右) noexcept {
    return std::tuple{左.仓库编号, 左.节点编号, 左.版本号}
        < std::tuple{右.仓库编号, 右.节点编号, 右.版本号};
}

bool 步骤小于(const 规范动态步& 左, const 规范动态步& 右) noexcept {
    const auto 左回合 = 回合排序键(左.回合);
    const auto 右回合 = 回合排序键(右.回合);
    if (左回合 != 右回合) return 左回合 < 右回合;
    if (左.片段序号 != 右.片段序号) return 左.片段序号 < 右.片段序号;
    if (左.发生时间戳 != 右.发生时间戳) return 左.发生时间戳 < 右.发生时间戳;
    return 句柄小于(左.来源动态, 右.来源动态);
}

}

std::uint64_t 计算动态模式结构键稳定哈希(const 动态模式结构键& 键) noexcept {
    if (!键.完整()) return 0;
    std::uint64_t 哈希 = 动态模式算法内部::哈希偏移;
    动态模式算法内部::混入(哈希, 键.被改变目标);
    动态模式算法内部::混入(哈希, static_cast<std::uint64_t>(键.前状态值));
    动态模式算法内部::混入(哈希, static_cast<std::uint64_t>(键.后状态值));
    动态模式算法内部::混入(哈希, 键.有来源动作 ? 1U : 0U);
    动态模式算法内部::混入(哈希, 键.来源动作);
    动态模式算法内部::混入(哈希, 键.规则版本);
    动态模式算法内部::混入(
        哈希, 键.二次特征角色.has_value()
            ? static_cast<std::uint64_t>(*键.二次特征角色) : 0U);
    return 哈希 == 0 ? 1 : 哈希;
}

std::size_t 计算动态模式结构键桶位(
    const 动态模式结构键& 键,
    std::size_t 桶数量) noexcept {
    if (桶数量 == 0) return 0;
    return static_cast<std::size_t>(计算动态模式结构键稳定哈希(键) % 桶数量);
}

std::optional<规范动态步> 形成规范动态步(const 动态模式权威批次& 批次) {
    if (!批次.完整()) return std::nullopt;
    规范动态步 输出;
    输出.回合 = {批次.请求.回执};
    输出.片段序号 = 批次.请求.片段序号;
    输出.结构键 = {
        批次.被改变目标.身份.节点,
        批次.前状态.状态值,
        批次.后状态.状态值,
        true,
        批次.来源动作.身份.节点,
        批次.请求.规则版本,
        std::nullopt};
    输出.发生时间戳 = 批次.动态.发生时间戳;
    输出.来源动态 = 批次.动态.身份.节点;
    输出.场景 = 批次.动态.场景;
    输出.主体 = 批次.动态.主体;
    输出.任务 = 批次.任务.节点;
    输出.方法 = 批次.方法.节点;
    输出.来源批次 = 批次;
    return 输出.完整() ? std::optional<规范动态步>{std::move(输出)} : std::nullopt;
}

std::optional<规范动态窗口> 形成规范动态窗口(
    const std::vector<动态模式权威批次>& 批次组) {
    if (批次组.empty()) return std::nullopt;
    规范动态窗口 输出;
    输出.步骤组.reserve(批次组.size());
    for (const auto& 批次 : 批次组) {
        auto 步骤 = 形成规范动态步(批次);
        if (!步骤.has_value()) return std::nullopt;
        输出.步骤组.push_back(std::move(*步骤));
    }
    std::stable_sort(输出.步骤组.begin(), 输出.步骤组.end(), 动态模式算法内部::步骤小于);

    std::uint64_t 哈希 = 动态模式算法内部::哈希偏移;
    for (const auto& 步骤 : 输出.步骤组) {
        const auto& 回执 = 步骤.回合.回执;
        const auto 请求编号 = 回执.来源执行请求.has_value()
            ? 回执.来源执行请求->调度请求.请求编号 : 0;
        动态模式算法内部::混入(哈希, 请求编号);
        动态模式算法内部::混入(哈希, 回执.回执编号);
        动态模式算法内部::混入(哈希, 回执.幂等键);
        动态模式算法内部::混入(哈希, 步骤.片段序号);
        动态模式算法内部::混入(哈希, 步骤.发生时间戳);
        动态模式算法内部::混入(哈希, 步骤.来源动态);
        动态模式算法内部::混入(
            哈希, 计算动态模式结构键稳定哈希(步骤.结构键));
    }
    输出.稳定哈希 = 哈希 == 0 ? 1 : 哈希;
    return 输出.完整() ? std::optional<规范动态窗口>{std::move(输出)} : std::nullopt;
}

}
