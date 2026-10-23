#include "因果.任务筹办服务.h"
#include "../领域/新_因果类.h"
#include "../领域/新_方法类.h"
#include "../领域/概念_动态类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <stdexcept>

namespace 海中鱼巣 {
因果任务筹办服务::因果任务筹办服务(const 因果反向查询服务& 反查, const 新_因果类& 因果,
    const 新_方法类& 方法, const 概念_动态类& 动作概念) noexcept
    : 反查_(反查), 因果_(因果), 方法_(方法), 动作概念_(动作概念) {}

std::vector<新先天因果信息> 因果任务筹办服务::读取先天结果规则(稳定编码 类型) const {
    const auto 节点组 = 因果_.查询先天结果(类型);
    auto 排序 = 节点组;
    std::sort(排序.begin(), 排序.end());
    if (std::adjacent_find(排序.begin(), 排序.end()) != 排序.end())
        throw std::logic_error("causal preparation duplicate innate identity");
    std::vector<新先天因果信息> 结果;
    for (auto 节点 : 排序) {
        const auto 规则 = 因果_.读取先天因果(节点);
        if (!规则 || 规则->节点 != 节点 || 规则->定义.唯一结果.结果特征类型 != 类型)
            throw std::logic_error("causal preparation listed rule unreadable");
        结果.push_back(*规则);
    }
    return 结果;
}

新因果筹办读回 因果任务筹办服务::逆推(const 新因果筹办请求& 请求) const {
    try {
        auto 结果 = 反查_.查询筹办(请求);
        if (结果.状态 == 新因果筹办读取状态::材料已变化) return 结果;
        const auto& 图 = 结果.通用反查图;
        for (const auto& 分支 : 图.分支组) {
            if (分支.筹办去向 == 反向因果筹办去向::追溯被动触发条件) continue;
            const auto 添加缺口 = [&](新因果能力缺口种类 种类,
                std::optional<稳定编码> 动作 = std::nullopt) {
                新因果能力缺口 缺口;
                缺口.种类 = 种类; 缺口.图分支编号 = 分支.编号;
                缺口.因果节点 = 分支.因果节点; 缺口.动作概念节点 = 动作;
                缺口.目标方向 = 请求.目标.目标变化方向;
                结果.能力缺口组.push_back(std::move(缺口));
            };
            if (分支.筹办去向 == 反向因果筹办去向::动作未知) {
                添加缺口(新因果能力缺口种类::未知动作); continue;
            }
            if (分支.筹办去向 == 反向因果筹办去向::动作触发方式尚未确定) {
                添加缺口(新因果能力缺口种类::触发方式未确定); continue;
            }
            if (分支.动作触发方式 != 新因果动作触发方式::主动调用)
                throw std::logic_error("causal active route without formal active trigger");
            bool 有动作 = false;
            for (auto 编号 : 分支.全部前提节点) {
                const auto 位置 = std::find_if(图.节点组.begin(), 图.节点组.end(),
                    [&](const auto& 节点) { return 节点.编号 == 编号; });
                if (位置 == 图.节点组.end()) throw std::logic_error("causal branch endpoint missing");
                if (位置->类型 != 反向因果端类型::动作) continue;
                有动作 = true;
                if (!位置->动作概念节点) throw std::logic_error("causal active action concept missing");
                const auto 动作 = 动作概念_.获取动态概念(*位置->动作概念节点);
                if (!动作 || 动作->定义类别 != 动态概念定义类别::动作 || !动作->动作定义)
                    throw std::logic_error("causal active action definition unreadable");
                新因果主动方法参考 参考;
                参考.图分支编号 = 分支.编号; 参考.因果节点 = 分支.因果节点;
                参考.动作概念节点 = 动作->节点; 参考.角色绑定组 = 分支.角色映射;
                const auto& 定义 = *动作->动作定义;
                if (定义.能力类别 == 动作能力定义类别::方法能力 && 定义.抽象方法节点) {
                    const auto 方法 = 方法_.获取方法(*定义.抽象方法节点);
                    if (!方法) throw std::logic_error("causal referenced method identity unreadable");
                    if (方法->启用状态 == 新方法启用状态::启用 && !方法->来源方法节点
                        && !方法->实例根节点 && !方法_.是历史服务结算方法(方法->节点))
                        参考.正式方法节点组.push_back(方法->节点);
                }
                if (参考.正式方法节点组.empty())
                    添加缺口(新因果能力缺口种类::主动动作无对应方法, 动作->节点);
                结果.主动方法参考组.push_back(std::move(参考));
            }
            if (!有动作) throw std::logic_error("causal active branch omitted its action");
        }
        return 结果;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"因果任务筹办",
            L"正式因果或方法读取失败，不转换为无方法。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"因果任务筹办", L"未知工程异常。");
    }
}
} // namespace 海中鱼巣
