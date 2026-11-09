#include "任务结算.规则.h"

#include <algorithm>
#include <limits>
#include <set>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {
void 要求结算事实(bool 成立) {
    if (!成立) throw std::logic_error("settlement contribution fact graph is incomplete or inconsistent");
}

std::int64_t 累加贡献(std::int64_t 当前, std::int64_t 增量) {
    要求结算事实(!((增量 > 0 && 当前 > (std::numeric_limits<std::int64_t>::max)() - 增量)
        || (增量 < 0 && 当前 < (std::numeric_limits<std::int64_t>::min)() - 增量)));
    return 当前 + 增量;
}
}

新结算贡献决策 计算结算贡献(const 新结算事实图& 图, 新任务贡献反馈定位 来源) {
    要求结算事实(图.需求任务对应已完整);
    const 新任务信息* 本任务 = nullptr;
    std::set<稳定编码> 任务身份组;
    for (const auto& [需求节点, 项] : 图.需求任务对应) {
        要求结算事实(需求节点 == 项.first.节点);
        if (!项.second) continue;
        // R108：非直接成员映射到同项服务任务；任务身份唯一性只对直接项要求。
        if (项.second->需求节点 != 需求节点) continue;
        要求结算事实(任务身份组.insert(项.second->节点).second);
        if (项.second->节点 == 来源.任务节点) 本任务 = &*项.second;
    }
    要求结算事实(本任务 != nullptr);
    const auto 本轮需求 = 有效(图.本轮服务需求) ? 图.本轮服务需求 : 本任务->需求节点;
    新结算贡献决策 决策;
    决策.来源 = 来源;
    if (本任务->最近执行实例节点 != 来源.执行实例节点
        || (本任务->当前状态 != 新任务状态::已完成
            && !(本任务->完成结算节点 && 本任务->当前状态 == 新任务状态::已结算))) {
        决策.状态 = 新任务贡献推进状态::预期已变化;
        return 决策;
    }
    const auto 父接收 = 图.父直接接收组.find(本任务->节点);
    要求结算事实(父接收 != 图.父直接接收组.end());
    const auto& 接收组 = 父接收->second;
    // R108：父可有早轮R（属其它Ds的子或已被更早K计入）；R唯一性按节点与来源K。
    std::set<稳定编码> 接收身份组, 接收来源组;
    for (const auto& 接收 : 接收组) {
        要求结算事实(接收.父任务节点 == 本任务->节点
            && 接收身份组.insert(接收.节点).second
            && 接收来源组.insert(接收.来源完成结算节点).second);
    }
    新任务完成贡献 贡献;
    bool 有直接子需求 = false;
    for (const auto& [需求节点, 项] : 图.需求任务对应) {
        if (项.first.父需求节点 != 本轮需求) continue;
        有直接子需求 = true;
        const auto& 子 = 项.second;
        if (!子) { 决策.待归集直接子需求节点组.push_back(需求节点); continue; }
        const auto s = 子->当前状态;
        const bool 已失败或取消 = s == 新任务状态::已失败 || s == 新任务状态::已取消;
        // 子任务K链中服务本需求的K（K.D == d），由采集层读入。
        std::vector<const 新任务完成结算信息*> 本需求K;
        for (const auto& [K节点, K] : 图.完成结算组)
            if (K.任务节点 == 子->节点 && K.需求节点 == 需求节点) 本需求K.push_back(&K);
        bool 等待 = false;
        for (const auto* K : 本需求K) {
            const auto 接收 = std::find_if(接收组.begin(), 接收组.end(),
                [&](const auto& r) { return r.来源完成结算节点 == K->节点; });
            if (接收 != 接收组.end()) {
                要求结算事实(K->服务贡献 && 接收->子任务节点 == 子->节点
                    && 接收->贡献值 == K->服务贡献->贡献值);
                if (图.已计入接收.contains(接收->节点)) continue; // 已被本任务更早K计入。
                贡献.贡献值 = 累加贡献(贡献.贡献值, 接收->贡献值);
                贡献.直接子接收节点组.push_back(接收->节点);
                continue;
            }
            // 无R：该K为子任务当前轮且尚未交接 → 等待；子任务此后已重启/续行或已失败取消 → 零贡献收束。
            const bool 当前轮K = 子->完成结算节点 == std::optional{K->节点}
                && 子->最近执行实例节点 == std::optional{K->执行实例节点};
            if (当前轮K && !已失败或取消 && s != 新任务状态::放弃结算) 等待 = true;
        }
        if (等待) { 决策.待归集直接子需求节点组.push_back(需求节点); continue; }
        if (!本需求K.empty()) continue;
        // 子任务尚无服务本需求的K：已终结（含同项任务代为服务、放弃结算、失败取消、未执行满足）→ 零贡献收束。
        const bool 子终结 = s == 新任务状态::已结算 || s == 新任务状态::放弃结算 || 已失败或取消
            || s == 新任务状态::未执行但目标已满足
            || (s == 新任务状态::已完成 && 子->完成结算节点 && [&] {
                const auto K = 图.完成结算组.find(*子->完成结算节点);
                return K != 图.完成结算组.end()
                    && std::optional{K->second.执行实例节点} == 子->最近执行实例节点; }());
        if (子终结) continue;
        决策.待归集直接子需求节点组.push_back(需求节点);
    }
    if (!决策.待归集直接子需求节点组.empty()) {
        决策.状态 = 新任务贡献推进状态::等待直接子结算;
        return 决策;
    }
    if (有直接子需求) {
        贡献.类型 = 新任务贡献计值类型::直接子汇总;
        std::sort(贡献.直接子接收节点组.begin(), 贡献.直接子接收节点组.end());
    } else {
        // R108：叶子轮可留有早轮R（待用R），不再要求无R。
        const auto 类别 = 图.叶子类别组.find(本任务->节点);
        if (类别 == 图.叶子类别组.end()) {
            决策.状态 = 新任务贡献推进状态::等待目标分类;
            return 决策;
        }
        要求结算事实(类别->second == 新任务贡献计值类型::服务叶子
            || 类别->second == 新任务贡献计值类型::治理叶子
            || 类别->second == 新任务贡献计值类型::非服务链叶子);
        贡献.类型 = 类别->second;
        贡献.贡献值 = 贡献.类型 == 新任务贡献计值类型::服务叶子 ? 1 : 0;
    }
    决策.状态 = 新任务贡献推进状态::已完成;
    决策.贡献 = std::move(贡献);
    return 决策;
}
} // namespace 海中鱼巣
