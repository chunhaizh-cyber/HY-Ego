// 文件规则：本模块只在不可变因果图快照上计算无环最短路径和安全分组，不读取或写入领域事实。
module;

#include "../核心/句柄.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <new>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.算法.安全因果层级;

import 海中鱼巣.领域.协议.分层安全维护;

namespace 海中鱼巣::安全因果层级细节 {

inline bool 句柄小于(const 节点句柄& 左, const 节点句柄& 右) noexcept {
    if (左.仓库编号 != 右.仓库编号) return 左.仓库编号 < 右.仓库编号;
    if (左.节点编号 != 右.节点编号) return 左.节点编号 < 右.节点编号;
    return 左.版本号 < 右.版本号;
}

inline bool 路径小于(const 安全因果路径快照& 左, const 安全因果路径快照& 右) {
    const auto 信息比较 = std::lexicographical_compare(
        左.因果信息序列.begin(), 左.因果信息序列.end(),
        右.因果信息序列.begin(), 右.因果信息序列.end(), 句柄小于);
    const auto 反向信息比较 = std::lexicographical_compare(
        右.因果信息序列.begin(), 右.因果信息序列.end(),
        左.因果信息序列.begin(), 左.因果信息序列.end(), 句柄小于);
    if (信息比较 || 反向信息比较) return 信息比较;
    return std::lexicographical_compare(
        左.直接因果边序列.begin(), 左.直接因果边序列.end(),
        右.直接因果边序列.begin(), 右.直接因果边序列.end(), 句柄小于);
}

inline bool 路径相等(const 安全因果路径快照& 左, const 安全因果路径快照& 右) {
    return 左.因果信息序列 == 右.因果信息序列
        && 左.直接因果边序列 == 右.直接因果边序列;
}

inline 安全分层读取结果 无载荷(安全结果分类 分类) noexcept {
    return {分类, std::nullopt};
}

}

export namespace 海中鱼巣 {

安全分层读取结果 计算安全因果层级(
    const 安全因果图快照& 图,
    const 安全分层读取请求& 请求) noexcept {
    using namespace 安全因果层级细节;
    try {
        if (!句柄有效(请求.自我)
            || !句柄有效(请求.来源因果信息)
            || !句柄有效(请求.目标安全结果)
            || !句柄有效(请求.适用场景)
            || 请求.自我 != 图.自我
            || 请求.目标安全结果 != 图.目标安全结果
            || 请求.适用场景 != 图.适用场景) {
            return 无载荷(安全结果分类::入口拒绝);
        }
        if (请求.期望图版本 == 0
            || 请求.事实截止版本 == 0
            || 图.图版本 != 请求.期望图版本
            || 图.节点集合版本 == 0
            || 图.边集合版本 == 0
            || 图.场景版本 == 0
            || 图.图版本 > 请求.事实截止版本) {
            return 无载荷(安全结果分类::版本漂移);
        }
        const auto 节点数 = 图.因果信息组.size();
        std::size_t 来源序号 = 节点数;
        std::vector<std::size_t> 目标序号组;
        for (std::size_t 序号 = 0; 序号 < 节点数; ++序号) {
            const auto& 信息 = 图.因果信息组[序号];
            if (!句柄有效(信息.因果信息身份)
                || !句柄有效(信息.对应结果身份)
                || !句柄有效(信息.适用场景)
                || 信息.适用场景 != 请求.适用场景
                || 信息.信息版本 == 0
                || 信息.当前性 != 记录当前性::当前) {
                return 无载荷(安全结果分类::结构拒绝);
            }
            if (序号 != 0
                && !句柄小于(
                    图.因果信息组[序号 - 1].因果信息身份, 信息.因果信息身份)) {
                return 无载荷(安全结果分类::结构拒绝);
            }
            if (信息.因果信息身份 == 请求.来源因果信息) 来源序号 = 序号;
            if (信息.对应结果身份 == 请求.目标安全结果
                && (信息.信息类型 == 安全因果信息类型::具体安全结果
                    || 信息.信息类型 == 安全因果信息类型::安全根结果)) {
                目标序号组.push_back(序号);
            }
        }
        if (来源序号 == 节点数) return 无载荷(安全结果分类::结构拒绝);

        struct 邻接项 final {
            std::size_t 目标序号 = 0;
            节点句柄 边身份;
        };
        std::vector<std::vector<邻接项>> 邻接表(节点数);
        std::vector<std::size_t> 入度(节点数, 0);
        auto 查找序号 = [&](const 节点句柄& 身份) {
            const auto 位置 = std::lower_bound(
                图.因果信息组.begin(), 图.因果信息组.end(), 身份,
                [](const 安全因果信息快照& 信息, const 节点句柄& 值) {
                    return 句柄小于(信息.因果信息身份, 值);
                });
            if (位置 == 图.因果信息组.end() || 位置->因果信息身份 != 身份) return 节点数;
            return static_cast<std::size_t>(位置 - 图.因果信息组.begin());
        };
        for (std::size_t 序号 = 0; 序号 < 图.直接因果边组.size(); ++序号) {
            const auto& 边 = 图.直接因果边组[序号];
            if (!句柄有效(边.边身份)
                || !句柄有效(边.原因因果信息)
                || !句柄有效(边.结果因果信息)
                || !句柄有效(边.证据动态)
                || 边.适用场景 != 请求.适用场景
                || 边.记录版本 == 0
                || 边.因果规则版本 == 0
                || 边.当前性 != 记录当前性::当前) {
                return 无载荷(安全结果分类::结构拒绝);
            }
            if (序号 != 0) {
                const auto& 前 = 图.直接因果边组[序号 - 1];
                const bool 顺序正确 = 句柄小于(前.原因因果信息, 边.原因因果信息)
                    || (前.原因因果信息 == 边.原因因果信息
                        && (句柄小于(前.结果因果信息, 边.结果因果信息)
                            || (前.结果因果信息 == 边.结果因果信息
                                && 句柄小于(前.边身份, 边.边身份))));
                if (!顺序正确) return 无载荷(安全结果分类::结构拒绝);
            }
            const auto 原因序号 = 查找序号(边.原因因果信息);
            const auto 结果序号 = 查找序号(边.结果因果信息);
            if (原因序号 == 节点数 || 结果序号 == 节点数 || 原因序号 == 结果序号) {
                return 无载荷(安全结果分类::结构拒绝);
            }
            邻接表[原因序号].push_back({结果序号, 边.边身份});
            ++入度[结果序号];
        }
        for (auto& 邻接组 : 邻接表) {
            std::sort(邻接组.begin(), 邻接组.end(),
                [&](const 邻接项& 左, const 邻接项& 右) {
                    const auto& 左身份 = 图.因果信息组[左.目标序号].因果信息身份;
                    const auto& 右身份 = 图.因果信息组[右.目标序号].因果信息身份;
                    if (左身份 != 右身份) return 句柄小于(左身份, 右身份);
                    return 句柄小于(左.边身份, 右.边身份);
                });
        }

        auto 剩余入度 = 入度;
        std::vector<std::size_t> 零入度;
        for (std::size_t 序号 = 0; 序号 < 节点数; ++序号) {
            if (剩余入度[序号] == 0) 零入度.push_back(序号);
        }
        std::sort(零入度.begin(), 零入度.end(), [&](auto 左, auto 右) {
            return 句柄小于(
                图.因果信息组[左].因果信息身份, 图.因果信息组[右].因果信息身份);
        });
        std::size_t 已处理 = 0;
        while (!零入度.empty()) {
            const auto 当前序号 = 零入度.front();
            零入度.erase(零入度.begin());
            ++已处理;
            for (const auto& 邻接 : 邻接表[当前序号]) {
                if (--剩余入度[邻接.目标序号] == 0) {
                    const auto 插入位置 = std::lower_bound(
                        零入度.begin(), 零入度.end(), 邻接.目标序号,
                        [&](auto 左, auto 右) {
                            return 句柄小于(
                                图.因果信息组[左].因果信息身份,
                                图.因果信息组[右].因果信息身份);
                        });
                    零入度.insert(插入位置, 邻接.目标序号);
                }
            }
        }
        if (已处理 != 节点数) return 无载荷(安全结果分类::结构拒绝);

        struct 前驱项 final {
            std::size_t 来源序号 = 0;
            节点句柄 边身份;
        };
        const auto 未访问 = (std::numeric_limits<std::int64_t>::max)();
        std::vector<std::int64_t> 距离(节点数, 未访问);
        std::vector<std::vector<前驱项>> 前驱表(节点数);
        std::queue<std::size_t> 队列;
        距离[来源序号] = 0;
        队列.push(来源序号);
        while (!队列.empty()) {
            const auto 当前序号 = 队列.front();
            队列.pop();
            if (距离[当前序号] == 未访问 - 1) {
                return 无载荷(安全结果分类::结构拒绝);
            }
            const auto 下一距离 = 距离[当前序号] + 1;
            for (const auto& 邻接 : 邻接表[当前序号]) {
                if (距离[邻接.目标序号] == 未访问) {
                    距离[邻接.目标序号] = 下一距离;
                    前驱表[邻接.目标序号].push_back({当前序号, 邻接.边身份});
                    队列.push(邻接.目标序号);
                } else if (距离[邻接.目标序号] == 下一距离) {
                    前驱表[邻接.目标序号].push_back({当前序号, 邻接.边身份});
                }
            }
        }

        std::int64_t 最短距离 = 未访问;
        std::vector<std::size_t> 最短目标组;
        for (const auto 目标序号 : 目标序号组) {
            if (距离[目标序号] < 最短距离) {
                最短距离 = 距离[目标序号];
                最短目标组.assign(1, 目标序号);
            } else if (距离[目标序号] == 最短距离) {
                最短目标组.push_back(目标序号);
            }
        }

        安全分层读取载荷 载荷;
        载荷.自我 = 请求.自我;
        载荷.来源因果信息 = 请求.来源因果信息;
        载荷.目标安全结果 = 请求.目标安全结果;
        载荷.图版本 = 图.图版本;
        载荷.规则版本 = 分层安全维护规则版本;
        载荷.事实截止版本 = 请求.事实截止版本;
        if (最短距离 == 未访问) {
            return {安全结果分类::未归层, std::move(载荷)};
        }

        std::vector<安全因果路径快照> 路径组;
        std::vector<节点句柄> 反向信息;
        std::vector<节点句柄> 反向边;
        std::function<void(std::size_t)> 回溯 = [&](std::size_t 当前序号) {
            反向信息.push_back(图.因果信息组[当前序号].因果信息身份);
            if (当前序号 == 来源序号) {
                安全因果路径快照 路径;
                路径.因果信息序列.assign(反向信息.rbegin(), 反向信息.rend());
                路径.直接因果边序列.assign(反向边.rbegin(), 反向边.rend());
                路径组.push_back(std::move(路径));
            } else {
                auto 前驱组 = 前驱表[当前序号];
                std::sort(前驱组.begin(), 前驱组.end(), [&](const auto& 左, const auto& 右) {
                    const auto& 左身份 = 图.因果信息组[左.来源序号].因果信息身份;
                    const auto& 右身份 = 图.因果信息组[右.来源序号].因果信息身份;
                    if (左身份 != 右身份) return 句柄小于(左身份, 右身份);
                    return 句柄小于(左.边身份, 右.边身份);
                });
                for (const auto& 前驱 : 前驱组) {
                    反向边.push_back(前驱.边身份);
                    回溯(前驱.来源序号);
                    反向边.pop_back();
                }
            }
            反向信息.pop_back();
        };
        for (const auto 目标序号 : 最短目标组) 回溯(目标序号);
        std::sort(路径组.begin(), 路径组.end(), 路径小于);
        路径组.erase(std::unique(路径组.begin(), 路径组.end(), 路径相等), 路径组.end());
        if (路径组.empty()) return 无载荷(安全结果分类::结构拒绝);

        载荷.精确深度适用性 = 适用性::适用;
        载荷.精确深度 = 最短距离;
        if (最短距离 == 0) {
            载荷.分组适用性 = 适用性::不适用;
            载荷.优先级组 = std::nullopt;
        } else {
            载荷.分组适用性 = 适用性::适用;
            载荷.优先级组 = (std::min)(最短距离, std::int64_t{5});
        }
        载荷.规范化主路径 = 路径组.front();
        载荷.全部并列最短路径 = std::move(路径组);
        return {安全结果分类::已形成, std::move(载荷)};
    } catch (const std::bad_alloc&) {
        return 无载荷(安全结果分类::资源失败);
    } catch (...) {
        return 无载荷(安全结果分类::资源失败);
    }
}

}
