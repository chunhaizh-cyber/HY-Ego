// 文件规则：本模块只实现分层安全定义准入和纯值维护候选，不读取仓库、时钟、日志或线程状态。
module;

#include "../核心/句柄.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <new>
#include <optional>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.算法.分层安全维护;

import 海中鱼巣.领域.协议.分层安全维护;

namespace 海中鱼巣::分层安全维护细节 {

inline bool 句柄小于(const 节点句柄& 左, const 节点句柄& 右) noexcept {
    if (左.仓库编号 != 右.仓库编号) return 左.仓库编号 < 右.仓库编号;
    if (左.节点编号 != 右.节点编号) return 左.节点编号 < 右.节点编号;
    return 左.版本号 < 右.版本号;
}

inline bool 安全加法(
    std::int64_t 左, std::int64_t 右, std::int64_t& 结果) noexcept {
    if (右 > 0 && 左 > (std::numeric_limits<std::int64_t>::max)() - 右) return false;
    if (右 < 0 && 左 < (std::numeric_limits<std::int64_t>::min)() - 右) return false;
    结果 = 左 + 右;
    return true;
}

inline bool 非负乘除向下(
    std::int64_t 左,
    std::int64_t 右,
    std::int64_t 除数,
    std::int64_t& 结果) noexcept {
    if (左 < 0 || 右 < 0 || 除数 <= 0) return false;
    const auto 无符号左 = static_cast<std::uint64_t>(左);
    const auto 无符号右 = static_cast<std::uint64_t>(右);
    const auto 无符号除数 = static_cast<std::uint64_t>(除数);
    std::uint64_t 商 = 0;
    std::uint64_t 余数 = 0;
    for (int 位 = 63; 位 >= 0; --位) {
        if (余数 > ((std::numeric_limits<std::uint64_t>::max)() >> 1)) return false;
        余数 <<= 1;
        余数 |= (无符号左 >> 位) & 1ULL;
        if (余数 >= 无符号除数) {
            余数 -= 无符号除数;
            商 |= 1ULL << 位;
        }
    }
    std::uint64_t 累计商 = 0;
    std::uint64_t 累计余数 = 0;
    for (int 位 = 63; 位 >= 0; --位) {
        if (((无符号右 >> 位) & 1ULL) == 0) continue;
        if (位 != 0 && 商 > ((std::numeric_limits<std::uint64_t>::max)() >> 位)) return false;
        const std::uint64_t 商增量 = 商 << 位;
        if (累计商 > (std::numeric_limits<std::uint64_t>::max)() - 商增量) return false;
        累计商 += 商增量;
        std::uint64_t 本位余数 = 累计余数;
        for (int 移位 = 0; 移位 < 位; ++移位) {
            if (本位余数 >= 无符号除数 - 本位余数) {
                if (累计商 == (std::numeric_limits<std::uint64_t>::max)()) return false;
                ++累计商;
                本位余数 = 本位余数 - (无符号除数 - 本位余数);
            } else {
                本位余数 += 本位余数;
            }
        }
        if (本位余数 >= 无符号除数 - 累计余数) {
            if (累计商 == (std::numeric_limits<std::uint64_t>::max)()) return false;
            ++累计商;
            累计余数 = 本位余数 - (无符号除数 - 累计余数);
        } else {
            累计余数 += 本位余数;
        }
    }
    if (累计商 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)())) {
        return false;
    }
    结果 = static_cast<std::int64_t>(累计商);
    return true;
}

inline bool 同一维护输入(
    const 安全层定义快照& 定义,
    const 安全层当前快照& 当前,
    const 安全维护请求& 请求) noexcept {
    return 句柄有效(请求.幂等主键)
        && 句柄有效(请求.自我)
        && 句柄有效(请求.维护族)
        && 句柄有效(请求.适用场景)
        && 请求.自我 == 当前.自我
        && 请求.维护族 == 当前.维护族
        && 当前.定义快照.定义身份 == 定义.定义身份
        && 当前.定义快照.定义版本 == 定义.定义版本
        && 当前.当前安全值 >= 定义.值域下界
        && 当前.当前安全值 <= 定义.值域上界;
}

inline bool 版本闭合(
    const 安全层定义快照& 定义,
    const 安全层当前快照& 当前,
    const 安全维护事实来源载荷& 事实,
    const 安全维护请求& 请求) noexcept {
    return 请求.期望当前值版本 != 0
        && 请求.期望当前值版本 == 当前.值版本
        && 请求.事实截止版本 != 0
        && 请求.维护规则版本 == 分层安全维护规则版本
        && 定义.规则版本 == 请求.维护规则版本
        && 当前.来源当前性 == 记录当前性::当前
        && 当前.图版本 != 0
        && 当前.残余约束版本 != 0
        && 事实.来源版本.图版本 == 当前.图版本
        && 事实.来源版本.事实截止版本 == 请求.事实截止版本
        && 事实.来源版本.值集合版本.has_value()
        && 事实.来源版本.值集合版本 == 当前.值版本
        && 事实.来源版本.维护规则版本.has_value()
        && 事实.来源版本.维护规则版本 == 请求.维护规则版本;
}

inline bool 因素已排除(
    const 安全因素快照& 因素,
    const std::vector<因素排除快照>& 排除组,
    const 节点句柄& 场景) noexcept {
    for (const auto& 排除 : 排除组) {
        if (排除.事件 == 因素.事件
            && 排除.因素 == 因素.因素
            && 排除.适用场景 == 场景
            && 排除.排除结论 == 因素排除结论::已验证排除
            && 排除.有效起始版本 != 0
            && 排除.排除版本 != 0) {
            return true;
        }
    }
    return false;
}

inline bool 有效证据段(
    const 未复发证据段& 段,
    const 安全因素快照& 因素,
    const 节点句柄& 场景,
    std::uint64_t 运行代次) noexcept {
    return 段.事件 == 因素.事件
        && 段.因素 == 因素.因素
        && 段.场景 == 场景
        && 段.运行代次 == 运行代次
        && 段.单调结束时间 > 段.单调开始时间
        && 段.复发机会 == 复发机会状态::成立
        && 段.感知覆盖 == 感知覆盖状态::成立
        && 段.身份比较 == 身份比较状态::成立
        && 段.结论 == 复发结论::确认未复发
        && 段.证据版本 != 0;
}

inline bool 计算因素有效增量(
    const 安全因素快照& 因素,
    const std::vector<未复发证据段>& 全部段,
    const 节点句柄& 场景,
    std::int64_t 截止时间,
    std::int64_t& 增量) {
    std::vector<std::pair<std::int64_t, std::int64_t>> 区间;
    std::uint64_t 运行代次 = 0;
    for (const auto& 段 : 全部段) {
        if (段.事件 != 因素.事件 || 段.因素 != 因素.因素 || 段.场景 != 场景) continue;
        if (运行代次 == 0) 运行代次 = 段.运行代次;
        if (!有效证据段(段, 因素, 场景, 运行代次)) return false;
        const auto 结束 = (std::min)(段.单调结束时间, 截止时间);
        if (结束 > 段.单调开始时间) 区间.emplace_back(段.单调开始时间, 结束);
    }
    if (区间.empty()) return false;
    std::sort(区间.begin(), 区间.end());
    std::int64_t 开始 = 区间.front().first;
    std::int64_t 结束 = 区间.front().second;
    std::int64_t 总量 = 0;
    for (std::size_t 序号 = 1; 序号 < 区间.size(); ++序号) {
        if (区间[序号].first <= 结束) {
            结束 = (std::max)(结束, 区间[序号].second);
            continue;
        }
        std::int64_t 片段 = 0;
        if (!安全加法(结束, -开始, 片段) || !安全加法(总量, 片段, 总量)) return false;
        开始 = 区间[序号].first;
        结束 = 区间[序号].second;
    }
    std::int64_t 片段 = 0;
    if (!安全加法(结束, -开始, 片段) || !安全加法(总量, 片段, 总量)) return false;
    增量 = 总量;
    return true;
}

inline 安全维护结果 无载荷(安全结果分类 分类) noexcept {
    return {分类, std::nullopt};
}

}

export namespace 海中鱼巣 {

安全结果分类 复核安全层定义(
    const 安全层定义快照& 定义) noexcept {
    if (!句柄有效(定义.定义身份) || !句柄有效(定义.被动维护提交方法)) {
        return 安全结果分类::入口拒绝;
    }
    if (定义.定义版本 == 0
        || 定义.规则版本 == 0
        || 定义.规则版本 != 分层安全维护规则版本) {
        return 安全结果分类::入口拒绝;
    }
    if (定义.值域下界 >= 定义.值域上界) return 安全结果分类::结构拒绝;
    if (定义.低位阈值 < 定义.值域下界
        || 定义.高位阈值 > 定义.值域上界
        || 定义.低位阈值 > 定义.高位阈值) {
        return 安全结果分类::结构拒绝;
    }
    if (定义.低位回升速率 < 0 || 定义.高位回落速率 < 0) {
        return 安全结果分类::结构拒绝;
    }
    if (定义.维护时间单位 <= 0) return 安全结果分类::结构拒绝;
    return 安全结果分类::已形成;
}

安全维护结果 计算分层安全维护候选(
    const 安全层定义快照& 定义,
    const 安全层当前快照& 当前,
    const 安全维护事实来源载荷& 事实,
    const 安全维护请求& 请求) noexcept {
    using namespace 分层安全维护细节;
    try {
        const auto 定义结果 = 复核安全层定义(定义);
        if (定义结果 != 安全结果分类::已形成) return 无载荷(定义结果);
        if (!同一维护输入(定义, 当前, 请求)) {
            return 无载荷(安全结果分类::入口拒绝);
        }
        if (!版本闭合(定义, 当前, 事实, 请求)) {
            return 无载荷(安全结果分类::版本漂移);
        }
        if (请求.事实截止版本 < 当前.事实处理游标) {
            return 无载荷(安全结果分类::版本漂移);
        }
        if (请求.事实截止版本 == 当前.事实处理游标) {
            安全维护载荷 重放{
                当前.维护族,
                当前.当前安全值,
                当前.当前安全值,
                0,
                安全维护方向::无变化,
                当前.累计有效时间,
                请求.事实截止版本,
                当前.值版本,
                std::nullopt,
                std::nullopt,
                当前.事实处理游标,
                当前.方向维护纪元,
                请求.维护规则版本};
            return {安全结果分类::精确重复, std::move(重放)};
        }

        for (std::size_t 左 = 0; 左 < 事实.安全因素组.size(); ++左) {
            const auto& 因素 = 事实.安全因素组[左];
            if (!句柄有效(因素.事件) || !句柄有效(因素.因素)
                || !句柄有效(因素.适用场景) || 因素.记录版本 == 0) {
                return 无载荷(安全结果分类::结构拒绝);
            }
            for (std::size_t 右 = 左 + 1; 右 < 事实.安全因素组.size(); ++右) {
                if (因素.事件 == 事实.安全因素组[右].事件
                    && 因素.因素 == 事实.安全因素组[右].因素) {
                    return 无载荷(安全结果分类::结构拒绝);
                }
            }
        }

        std::int64_t 后值 = 当前.当前安全值;
        std::int64_t 新累计时间 = 当前.累计有效时间;
        安全维护方向 方向 = 安全维护方向::无变化;
        bool 主动事实存在 = false;
        std::optional<节点句柄> 来源已发布事实批次;
        for (const auto& 主动 : 事实.主动安全事实结算快照组) {
            if (主动.维护族 != 当前.维护族) continue;
            if (!句柄有效(主动.事实批次) || 主动.结算版本 == 0
                || 主动.后值 < 定义.值域下界 || 主动.后值 > 定义.值域上界) {
                return 无载荷(安全结果分类::结构拒绝);
            }
            主动事实存在 = true;
            后值 = 主动.后值;
            新累计时间 = 0;
            方向 = 安全维护方向::主动事实优先;
            来源已发布事实批次 = 主动.事实批次;
        }

        if (!主动事实存在) {
            std::vector<const 安全因素快照*> 残余因素;
            for (const auto& 因素 : 事实.安全因素组) {
                if (因素.适用场景 != 请求.适用场景) continue;
                if (因素.因素状态 == 安全因素状态::已验证排除
                    || 因素已排除(因素, 事实.因素排除组, 请求.适用场景)) {
                    continue;
                }
                残余因素.push_back(&因素);
            }
            std::sort(残余因素.begin(), 残余因素.end(),
                [](const auto* 左, const auto* 右) {
                    if (左->事件 != 右->事件) return 句柄小于(左->事件, 右->事件);
                    return 句柄小于(左->因素, 右->因素);
                });

            std::int64_t 本轮有效时间 = (std::numeric_limits<std::int64_t>::max)();
            if (!残余因素.empty()) {
                for (const auto* 因素 : 残余因素) {
                    std::int64_t 因素时间 = 0;
                    if (!计算因素有效增量(
                            *因素, 事实.未复发证据段组, 请求.适用场景,
                            请求.当前单调时间, 因素时间)) {
                        return 无载荷(安全结果分类::材料缺失);
                    }
                    本轮有效时间 = (std::min)(本轮有效时间, 因素时间);
                }
            } else {
                本轮有效时间 = 0;
            }

            if (当前.当前安全值 < 定义.低位阈值) {
                方向 = 安全维护方向::低位回升;
            } else if (当前.当前安全值 > 定义.高位阈值 && !残余因素.empty()) {
                方向 = 安全维护方向::高位回落;
            } else {
                方向 = 安全维护方向::无变化;
                本轮有效时间 = 0;
                新累计时间 = 0;
            }

            if (本轮有效时间 > 0 && 方向 != 安全维护方向::无变化) {
                if (!安全加法(当前.累计有效时间, 本轮有效时间, 新累计时间)) {
                    return 无载荷(安全结果分类::结构拒绝);
                }
                const auto 速率 = 方向 == 安全维护方向::低位回升
                    ? 定义.低位回升速率 : 定义.高位回落速率;
                std::int64_t 旧应变化 = 0;
                std::int64_t 新应变化 = 0;
                if (!非负乘除向下(
                        速率, 当前.累计有效时间, 定义.维护时间单位, 旧应变化)
                    || !非负乘除向下(
                        速率, 新累计时间, 定义.维护时间单位, 新应变化)
                    || 新应变化 < 旧应变化) {
                    return 无载荷(安全结果分类::结构拒绝);
                }
                const auto 变化量 = 新应变化 - 旧应变化;
                if (方向 == 安全维护方向::低位回升) {
                    std::int64_t 候选 = 0;
                    if (!安全加法(当前.当前安全值, 变化量, 候选)) {
                        return 无载荷(安全结果分类::结构拒绝);
                    }
                    后值 = (std::min)(定义.低位阈值, 候选);
                } else {
                    std::int64_t 候选 = 0;
                    if (!安全加法(当前.当前安全值, -变化量, 候选)) {
                        return 无载荷(安全结果分类::结构拒绝);
                    }
                    后值 = (std::max)(定义.高位阈值, 候选);
                }
            }
        }

        std::int64_t 有符号变化 = 0;
        if (!安全加法(后值, -当前.当前安全值, 有符号变化)) {
            return 无载荷(安全结果分类::结构拒绝);
        }
        const auto 绝对变化 = 有符号变化 < 0 ? -有符号变化 : 有符号变化;
        安全维护载荷 载荷{
            当前.维护族,
            当前.当前安全值,
            后值,
            绝对变化,
            方向,
            新累计时间,
            请求.事实截止版本,
            当前.值版本,
            绝对变化 == 0 ? std::nullopt
                          : std::optional<std::uint64_t>(当前.值版本 + 1),
            绝对变化 == 0 ? std::nullopt : 来源已发布事实批次,
            当前.事实处理游标 + 1,
            当前.方向维护纪元 + (方向 == 安全维护方向::主动事实优先 ? 1 : 0),
            请求.维护规则版本};
        return {绝对变化 == 0 ? 安全结果分类::无变化 : 安全结果分类::已维护,
            std::move(载荷)};
    } catch (const std::bad_alloc&) {
        return 无载荷(安全结果分类::资源失败);
    } catch (...) {
        return 无载荷(安全结果分类::资源失败);
    }
}

}
