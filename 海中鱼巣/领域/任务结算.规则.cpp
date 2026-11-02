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
        要求结算事实(项.second->需求节点 == 需求节点
            && 任务身份组.insert(项.second->节点).second);
        if (项.second->节点 == 来源.任务节点) 本任务 = &*项.second;
    }
    要求结算事实(本任务 != nullptr);
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
    std::set<稳定编码> 接收身份组, 接收子任务组;
    for (const auto& 接收 : 接收组) {
        要求结算事实(接收.父任务节点 == 本任务->节点
            && 接收身份组.insert(接收.节点).second
            && 接收子任务组.insert(接收.子任务节点).second);
        const auto 子 = std::find_if(图.需求任务对应.begin(), 图.需求任务对应.end(),
            [&](const auto& 项) { return 项.second.second && 项.second.second->节点 == 接收.子任务节点; });
        要求结算事实(子 != 图.需求任务对应.end() && 子->second.first.父需求节点 == 本任务->需求节点);
    }
    新任务完成贡献 贡献;
    bool 有直接子需求 = false;
    for (const auto& [需求节点, 项] : 图.需求任务对应) {
        if (项.first.父需求节点 != 本任务->需求节点) continue;
        有直接子需求 = true;
        const auto& 子 = 项.second;
        const auto 接收 = 子 ? std::find_if(接收组.begin(), 接收组.end(),
            [&](const auto& 项接收) { return 项接收.子任务节点 == 子->节点; }) : 接收组.end();
        const bool 未执行满足=子&&子->当前状态==新任务状态::未执行但目标已满足;
        if(未执行满足&&!子->完成结算节点){要求结算事实(接收==接收组.end());continue;}
        const bool 已失败或取消 = 子 && (子->当前状态 == 新任务状态::已失败
            || 子->当前状态 == 新任务状态::已取消);
        if (已失败或取消 && 接收 == 接收组.end()) continue;
        if (!已失败或取消 && !未执行满足 && (!子 || (子->当前状态 != 新任务状态::已完成
            && 子->当前状态 != 新任务状态::已结算) || !子->完成结算节点)) {
            决策.待归集直接子需求节点组.push_back(需求节点);
            continue;
        }
        要求结算事实(子 && 子->完成结算节点);
        const auto 完成 = 图.完成结算组.find(*子->完成结算节点);
        要求结算事实(完成 != 图.完成结算组.end()
            && 完成->second.节点 == *子->完成结算节点
            && 完成->second.任务节点 == 子->节点 && 完成->second.需求节点 == 需求节点);
        if(未执行满足){
            要求结算事实(完成->second.服务贡献.has_value());
            if(接收!=接收组.end())要求结算事实(接收->来源完成结算节点==完成->second.节点
                &&接收->贡献值==完成->second.服务贡献->贡献值);
        }
        if (!完成->second.服务贡献 || 接收 == 接收组.end()
            || 接收->来源完成结算节点 != 完成->second.节点) {
            要求结算事实(!已失败或取消);
            决策.待归集直接子需求节点组.push_back(需求节点);
            continue;
        }
        要求结算事实(接收->来源完成结算节点 == 完成->second.节点
            && 接收->贡献值 == 完成->second.服务贡献->贡献值);
        贡献.贡献值 = 累加贡献(贡献.贡献值, 接收->贡献值);
        贡献.直接子接收节点组.push_back(接收->节点);
    }
    if (!决策.待归集直接子需求节点组.empty()) {
        决策.状态 = 新任务贡献推进状态::等待直接子结算;
        return 决策;
    }
    if (有直接子需求) {
        贡献.类型 = 新任务贡献计值类型::直接子汇总;
        std::sort(贡献.直接子接收节点组.begin(), 贡献.直接子接收节点组.end());
    } else {
        要求结算事实(接收组.empty());
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
