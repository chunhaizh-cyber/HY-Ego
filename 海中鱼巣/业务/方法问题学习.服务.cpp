#include "方法问题学习.服务.h"
#include "../领域/新_方法类.h"
#include "../领域/新_存在类.h"
#include "../领域/概念_特征类.h"
#include "../领域/新_特征值类.h"
#include "../核心/日志系统.h"
#include <map>
#include <algorithm>
#include <set>

namespace 海中鱼巣 {
namespace {
bool 结果定义为空(const 新方法结果定义& r) {
    return r.结果项组.empty() && r.参数关系组.empty() && r.场景事实限定组.empty();
}
const 新本能函数条件角色材料* 查找有序参数(const 新本能函数本次条件& 条件, std::uint32_t 序号) {
    for (const auto& 角色 : 条件.参数角色组)
        if (角色.有序引用 && 角色.有序引用->序号 == 序号) return &角色;
    return nullptr;
}
// R-1：概念层值域（实例身份 / 集合约束）原样保留；其余材料值域本版不收窄。
bool 概念层值域(const 新特征匹配定义快照& d) {
    return d.比较规则 == 特征概念比较规则::存在身份精确
        || d.比较规则 == 特征概念比较规则::存在集合身份判等
        || d.比较规则 == 特征概念比较规则::需求集合身份判等
        || d.比较规则 == 特征概念比较规则::服务任务集合身份判等
        || d.比较规则 == 特征概念比较规则::消息集合身份判等
        || d.材料类型 == 特征概念材料物理类型::存在节点引用
        || d.材料类型 == 特征概念材料物理类型::需求节点引用
        || d.材料类型 == 特征概念材料物理类型::信息节点引用;
}
std::wstring 处理状态文字(新方法问题处理状态 s) {
    switch (s) {
    case 新方法问题处理状态::待处理: return L"待处理";
    case 新方法问题处理状态::已发布: return L"已发布";
    case 新方法问题处理状态::前提已变化: return L"前提已变化";
    case 新方法问题处理状态::缺口: return L"缺口";
    case 新方法问题处理状态::逻辑错误: return L"逻辑错误";
    }
    return L"?";
}
}

// TRIAL-LEARNING-DIRECT §3（候选规则沿 TRIAL-LEARNING-REVIEW §4）。诊断责任：读取不到定义按缺口返回，工程异常向上送出。
新尝试补全导出结果 形成尝试补全候选(const 新方法问题反馈材料& 材料,
    const 新方法信息& m, const 新_方法类& 方法读取, const 新尝试补全概念服务& 概念) {
    新尝试补全导出结果 出;
    const auto 缺口 = [&](std::wstring 原因) {
        出.种类 = 新尝试补全导出种类::缺口; 出.候选.reset(); 出.原因 = std::move(原因); return 出;
    };
    const auto 逻辑错误 = [&](std::wstring 原因) {
        出.种类 = 新尝试补全导出种类::逻辑错误; 出.候选.reset(); 出.原因 = std::move(原因); return 出;
    };
    // 4.1 前提
    if (!材料.本次运行对) return 缺口(L"无本次运行对");
    const auto& 采用 = 材料.任务采用;
    const auto& 运行 = *材料.本次运行对;
    if (运行.结果.结果取得状态 != 新本能函数结果取得状态::已取得)
        return 缺口(L"结果未取得 取得状态=" + std::to_wstring(static_cast<int>(运行.结果.结果取得状态)));
    if (!结果定义为空(采用.结果)) return 缺口(L"采用配对结果非空（验证另行设计）");
    if (m.节点 != 材料.来源抽象方法节点) return 缺口(L"方法与执行来源方法不同");
    const auto 原 = std::find_if(m.条件结果组.begin(), m.条件结果组.end(),
        [&](const auto& p) { return p.配对节点 == 采用.来源配对节点; });
    if (原 == m.条件结果组.end()) return 缺口(L"原配对已不被方法引用 配对=" + std::to_wstring(采用.来源配对节点.值));
    if (原->条件 != 采用.原始条件 || !结果定义为空(原->结果))
        return 缺口(L"原配对已变化 配对=" + std::to_wstring(原->配对节点.值));
    新尝试补全候选 c;
    c.方法节点 = m.节点; c.原配对节点 = 原->配对节点; c.执行实例节点 = 材料.执行实例节点;
    c.原配对条件 = 原->条件;
    c.实际条件 = 采用.原始条件; c.来源执行分支节点组 = 采用.来源执行分支节点组;
    // 4.2 实际条件：只收窄I64区间到动作前实际值，不推广
    bool 有收窄 = false;
    for (auto& 项 : c.实际条件.条件项组) {
        if (项.任务成员变量序号) return 缺口(L"任务成员变量条件项未支持");
        if (项.精确数值域) {
            const auto 区间 = *项.精确数值域;
            const auto* 角色 = 查找有序参数(运行.条件, 项.参数序号);
            if (!角色 || !角色->有序引用) return 缺口(L"条件项参数无本次实参 参数=" + std::to_wstring(项.参数序号));
            if (角色->有序引用->信息类型 != 新方法参数信息类型::特征)
                return 缺口(L"存在参数附属数值条件未支持 参数=" + std::to_wstring(项.参数序号));
            const auto 实际 = std::find_if(角色->实际特征组.begin(), 角色->实际特征组.end(),
                [&](const auto& f) { return f.特征节点 == 角色->有序引用->信息节点; });
            const auto* v = 实际 != 角色->实际特征组.end() && 实际->动作前准确值
                ? std::get_if<std::int64_t>(&*实际->动作前准确值) : nullptr;
            if (!v) return 缺口(L"条件项实际值缺失 参数=" + std::to_wstring(项.参数序号));
            if (*v < 区间.下界 || *v > 区间.上界)
                return 逻辑错误(L"合同违规：实际值不在已核验区间 参数=" + std::to_wstring(项.参数序号)
                    + L" 值=" + std::to_wstring(*v));
            if (区间.下界 != 区间.上界) {
                项.精确数值域 = 特征概念I64闭区间{*v, *v};
                c.差异项组.push_back({新尝试补全差异种类::条件收窄, 项.参数序号, 0});
                有收窄 = true;
            }
        } else if (项.特征值域概念节点) {
            const auto d = 方法读取.读取模拟特征范围(*项.特征值域概念节点);
            if (!d) return 缺口(L"条件值域定义不可读 参数=" + std::to_wstring(项.参数序号));
            if (!概念层值域(*d)) return 缺口(L"条件材料类型未支持 参数=" + std::to_wstring(项.参数序号));
        }
    }
    c.与原条件相同 = !有收窄;
    // 4.3 实际结果：只取本次函数报告形成、且在执行实际状态中的状态
    std::set<稳定编码> 声明结果类型;
    for (const auto ft : m.方法头.可能结果概念组)
        if (const auto d = 方法读取.读取模拟特征范围(ft)) 声明结果类型.insert(d->特征类型根);
    std::map<std::uint32_t, const 新状态信息*> 按参数;
    // 登记结果组（R110）中不依附参数的项：本次状态落在其作用对象上时按“结果作用存在”记录（零参数方法）。
    std::vector<const 新状态信息*> 按作用存在;
    for (const auto 节点 : 运行.结果.已形成状态节点组) {
        const auto s = std::find_if(材料.实际状态组.begin(), 材料.实际状态组.end(),
            [&](const auto& x) { return x.节点 == 节点; });
        if (s == 材料.实际状态组.end()) continue;
        const 新本能函数条件角色材料* 参 = nullptr;
        for (const auto& 角色 : 运行.条件.参数角色组)
            if (角色.有序引用 && 角色.有序引用->信息类型 == 新方法参数信息类型::特征
                && 角色.有序引用->信息节点 == s->来源实例特征节点) 参 = &角色;
        if (!参) {
            const auto ds = 方法读取.读取模拟特征范围(s->特征类型概念节点);
            const bool 属登记项 = ds && std::any_of(m.登记结果组.begin(), m.登记结果组.end(), [&](const auto& 项) {
                if (项.参数序号 != 0 || !项.作用存在概念节点) return false;
                const auto d = 方法读取.读取模拟特征范围(项.特征类型概念节点);
                return d && d->特征类型根 == ds->特征类型根
                    && 概念.存在.存在节点属于概念(s->被描述存在节点, *项.作用存在概念节点);
            });
            if (!属登记项) return 缺口(L"表达能力不足：本次形成状态不在参数特征上 状态=" + std::to_wstring(s->节点.值)
                + L" 特征=" + std::to_wstring(s->来源实例特征节点.值));
            if (std::any_of(按作用存在.begin(), 按作用存在.end(), [&](const auto* x) {
                    return x->来源实例特征节点 == s->来源实例特征节点; }))
                return 缺口(L"同特征多个本次状态 特征=" + std::to_wstring(s->来源实例特征节点.值));
            按作用存在.push_back(&*s);
            continue;
        }
        if (!按参数.emplace(参->有序引用->序号, &*s).second)
            return 缺口(L"同特征多个本次状态 参数=" + std::to_wstring(参->有序引用->序号));
    }
    if (按参数.empty() && 按作用存在.empty()) return 缺口(L"已取得但本次无形成状态");
    // 写回值取值域：存在身份精确时为“值域＝该存在专属概念”的特征概念（2026-11-10 用户裁决，记实际写回的存在本身）。
    const auto 存在值域 = [&](稳定编码 类型根, 稳定编码 E) -> std::optional<稳定编码> {
        const auto 存在 = 概念.存在.获取存在(E);
        const auto 根 = 概念.特征概念.获取特征概念(类型根);
        if (!存在 || !根) return std::nullopt;
        特征概念定义 定义;
        定义.材料类型 = 根->材料类型; 定义.单位 = 根->单位;
        定义.比较规则 = 根->比较规则; 定义.聚合规则 = 根->聚合规则;
        定义.值域 = 特征概念存在引用值域{存在->专属存在概念节点};
        const auto 值域概念 = 概念.特征概念.建立或取得特征概念(定义, 概念.特征值, 类型根);
        if (!有效(值域概念)) return std::nullopt;
        return 值域概念;
    };
    for (const auto* s : 按作用存在) {
        const auto ds = 方法读取.读取模拟特征范围(s->特征类型概念节点);
        if (!ds) return 缺口(L"结果类型定义不可读 作用存在=" + std::to_wstring(s->被描述存在节点.值));
        if (!声明结果类型.contains(ds->特征类型根))
            return 逻辑错误(L"合同违规：本次形成状态的特征类型未在方法头声明为结果 作用存在="
                + std::to_wstring(s->被描述存在节点.值));
        if (!s->固定准确值) return 缺口(L"本次状态值未知 作用存在=" + std::to_wstring(s->被描述存在节点.值));
        新方法结果项定义 r; r.结果作用存在 = s->被描述存在节点; r.特征类型概念节点 = ds->特征类型根;
        if (ds->比较规则 == 特征概念比较规则::存在身份精确) {
            const auto* E = std::get_if<稳定编码>(&*s->固定准确值);
            if (!E) return 缺口(L"存在引用结果值形状不符 作用存在=" + std::to_wstring(s->被描述存在节点.值));
            const auto 值域 = 存在值域(ds->特征类型根, *E);
            if (!值域) return 缺口(L"写回存在值域概念未能建立 值=" + std::to_wstring(E->值));
            r.特征值域概念节点 = *值域;
        } else if (const auto* v = std::get_if<std::int64_t>(&*s->固定准确值)) {
            r.精确数值域 = 特征概念I64闭区间{*v, *v};
        } else {
            return 缺口(L"结果材料类型未支持 作用存在=" + std::to_wstring(s->被描述存在节点.值));
        }
        c.实际结果.结果项组.push_back(r);
        c.差异项组.push_back({新尝试补全差异种类::结果新增, 0, 0});
    }
    for (const auto& [p, s] : 按参数) {
        const auto 规格 = std::find_if(m.参数规格组.begin(), m.参数规格组.end(),
            [&](const auto& x) { return x.有序规格 && x.有序规格->序号 == p; });
        if (规格 == m.参数规格组.end()) return 缺口(L"结果参数无有序规格 参数=" + std::to_wstring(p));
        const auto d = 方法读取.读取模拟特征范围(规格->有序规格->范围概念节点);
        const auto ds = 方法读取.读取模拟特征范围(s->特征类型概念节点);
        if (!d || !ds) return 缺口(L"结果类型定义不可读 参数=" + std::to_wstring(p));
        if (ds->特征类型根 != d->特征类型根 || !声明结果类型.contains(d->特征类型根))
            return 逻辑错误(L"合同违规：本次形成状态的特征类型未在方法头声明为结果 参数=" + std::to_wstring(p)
                + L" 类型=" + std::to_wstring(s->特征类型概念节点.值));
        if (!s->固定准确值) return 缺口(L"本次状态值未知 参数=" + std::to_wstring(p));
        新方法结果项定义 r; r.参数序号 = p; r.特征类型概念节点 = d->特征类型根;
        if (d->比较规则 == 特征概念比较规则::存在身份精确) {
            const auto* E = std::get_if<稳定编码>(&*s->固定准确值);
            if (!E) return 缺口(L"存在引用结果值形状不符 参数=" + std::to_wstring(p));
            const 新本能函数条件角色材料* q = nullptr;
            for (const auto& 角色 : 运行.条件.参数角色组)
                if (角色.有序引用 && 角色.有序引用->信息类型 == 新方法参数信息类型::存在
                    && 角色.有序引用->信息节点 == *E) q = &角色;
            if (q) {
                c.实际结果.参数关系组.push_back({新方法参数关系种类::特征引用存在, p, q->有序引用->序号});
                c.差异项组.push_back({新尝试补全差异种类::结果关系新增, p, q->有序引用->序号});
            } else {
                // 用户裁决（2026-11-10）：写回值不是参数实参时，结果记实际写回的存在本身——
                // 在结果特征类型根下取得值域为该存在专属概念的特征概念。
                const auto 存在 = 概念.存在.获取存在(*E);
                if (!存在) return 缺口(L"写回存在不可读 参数=" + std::to_wstring(p) + L" 值=" + std::to_wstring(E->值));
                const auto 根 = 概念.特征概念.获取特征概念(d->特征类型根);
                if (!根) return 缺口(L"结果类型根不可读 参数=" + std::to_wstring(p));
                特征概念定义 定义;
                定义.材料类型 = 根->材料类型; 定义.单位 = 根->单位;
                定义.比较规则 = 根->比较规则; 定义.聚合规则 = 根->聚合规则;
                定义.值域 = 特征概念存在引用值域{存在->专属存在概念节点};
                const auto 值域概念 = 概念.特征概念.建立或取得特征概念(定义, 概念.特征值, d->特征类型根);
                if (!有效(值域概念)) return 缺口(L"写回存在值域概念未能建立 参数=" + std::to_wstring(p)
                    + L" 值=" + std::to_wstring(E->值));
                r.特征值域概念节点 = 值域概念;
            }
        } else if (const auto* v = std::get_if<std::int64_t>(&*s->固定准确值)) {
            r.精确数值域 = 特征概念I64闭区间{*v, *v};
        } else {
            return 缺口(L"结果材料类型未支持 参数=" + std::to_wstring(p));
        }
        c.实际结果.结果项组.push_back(r);
        c.差异项组.push_back({新尝试补全差异种类::结果新增, p, 0});
    }
    c.实际结果.关系规则 = 新方法项关系规则::全部成立;
    出.种类 = 新尝试补全导出种类::候选;
    出.候选 = std::move(c);
    return 出;
}

方法问题学习服务::方法问题学习服务(新_方法类& 方法, const 新_存在类& 存在,
    概念_特征类& 特征概念, const 新_特征值类& 特征值) noexcept
    : 方法_(方法), 概念_{存在, 特征概念, 特征值} {}

// TRIAL-LEARNING-DIRECT §4—§5。诊断责任：本地记录（处理结论一次写运行日志，逻辑错误写逻辑错误日志）；资源失败与发布未知以学习运行异常向上送出。
std::pair<新方法问题处理状态, std::wstring> 方法问题学习服务::处理尝试补全(
    const 新方法问题学习消息& 消息, const 新方法问题反馈材料& 材料) {
    using S = 新方法问题处理状态;
    const auto 方法 = 方法_.获取方法(消息.方法节点);
    if (!方法) return {S::前提已变化, L"方法不可读"};
    const auto 导出 = 形成尝试补全候选(材料, *方法, 方法_, 概念_);
    if (导出.种类 == 新尝试补全导出种类::缺口) return {S::缺口, 导出.原因};
    if (导出.种类 == 新尝试补全导出种类::逻辑错误) return {S::逻辑错误, 导出.原因};
    const auto& 候选 = *导出.候选;
    if (方法_.查找尝试配对(候选.方法节点, 候选.实际条件, 候选.实际结果))
        return {S::已发布, L"同义尝试配对已存在"};
    const auto 发布 = 方法_.发布尝试条件结果(候选);
    const auto 新配对文字 = [&] {
        return 发布.新配对节点组.empty() ? std::wstring{} : L" 新配对=" + std::to_wstring(发布.新配对节点组.front().值);
    };
    switch (发布.状态) {
    case 新方法学习发布状态::已发布:
        return {S::已发布, (候选.与原条件相同 ? L"替换原配对" : L"追加尝试配对") + 新配对文字()};
    case 新方法学习发布状态::前提已变化:
    case 新方法学习发布状态::方法不可用:
    case 新方法学习发布状态::条件结果不合法:
        return {S::前提已变化, L"发布：前提已变化 状态=" + std::to_wstring(static_cast<int>(发布.状态))};
    case 新方法学习发布状态::资源失败:
        throw 新学习运行异常(消息.方法节点, 新学习运行异常原因::资源不足, "trial publication resource failure");
    case 新方法学习发布状态::发布结果未知:
        if (方法_.查找尝试配对(候选.方法节点, 候选.实际条件, 候选.实际结果))
            return {S::已发布, L"发布未知后按原请求读回确认"};
        throw 新学习运行异常(消息.方法节点, 新学习运行异常原因::发布结果未知, "trial publication unknown");
    default:
        throw 新学习运行异常(消息.方法节点, 新学习运行异常原因::结构不一致,
            "trial publication status " + std::to_string(static_cast<int>(发布.状态)));
    }
}

新学习结束通知 方法问题学习服务::学习(const 新方法问题学习消息& 消息,
    const std::function<bool()>& 停止检查) {
    auto 拒绝 = [&](新学习运行异常原因 原因) { throw 新学习运行异常(消息.方法节点, 原因); };
    try {
        if (!有效(消息.方法节点) || !有效(消息.任务节点)
            || !有效(消息.执行实例节点) || !消息.材料) 拒绝(新学习运行异常原因::输入不合法);
        const auto& 材料 = *消息.材料;
        if (材料.任务节点!=消息.任务节点 || 材料.执行实例节点!=消息.执行实例节点
            || 材料.来源抽象方法节点!=消息.方法节点 || !有效(材料.影响评估节点) || 材料.问题组.empty())
            拒绝(新学习运行异常原因::结构不一致);
        for (const auto 问题 : 材料.问题组)
            if (问题<新方法问题种类::方向不符 || 问题>新方法问题种类::尝试执行)
                拒绝(新学习运行异常原因::结构不一致);
        if (停止检查 && 停止检查()) 拒绝(新学习运行异常原因::停止取消);
        {
            std::lock_guard 锁(互斥_);
            const auto [位置,插入]=收集组_.try_emplace(消息.执行实例节点,
                新方法问题学习收集结果{消息, 材料.本次运行对.has_value()});
            if (!插入) {
                if (位置->second.消息!=消息) 拒绝(新学习运行异常原因::结构不一致);
                // 已结束直接返回；待处理继续处理。
                if (位置->second.处理状态!=新方法问题处理状态::待处理) return {消息.方法节点};
            }
        }
        std::pair<新方法问题处理状态, std::wstring> 处理{新方法问题处理状态::缺口, L"材料不完整（无本次运行对）"};
        if (材料.本次运行对) 处理 = 处理尝试补全(消息, 材料);
        {
            std::lock_guard 锁(互斥_);
            if (const auto 位置=收集组_.find(消息.执行实例节点); 位置!=收集组_.end()) {
                位置->second.处理状态=处理.first;
                位置->second.处理原因=处理.second;
            }
        }
        const auto 文本 = L"方法=" + std::to_wstring(消息.方法节点.值) + L" 任务=" + std::to_wstring(消息.任务节点.值)
            + L" 执行=" + std::to_wstring(消息.执行实例节点.值)
            + L" 处理=" + 处理状态文字(处理.first) + L" 原因=" + 处理.second;
        // R-2：合同违规经正式逻辑错误入口记录，不弹窗、不终止；其余处理结论写运行日志。
        if (处理.first==新方法问题处理状态::逻辑错误) (void)记录逻辑错误日志(L"方法问题学习", L"尝试补全", 文本);
        else (void)记录运行日志(L"方法问题学习", L"尝试补全", 文本);
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
        try { (void)记录调试日志(L"PARAM_BINDING", L"方法问题学习", L"尝试补全", 文本); } catch (...) {}
#endif
        return {消息.方法节点};
    } catch (const std::bad_alloc&) { 拒绝(新学习运行异常原因::资源不足); }
    throw 新学习运行异常(消息.方法节点,新学习运行异常原因::内部错误);
}
std::optional<新方法问题学习收集结果> 方法问题学习服务::读取收集结果(稳定编码 执行实例节点) const noexcept {
    try {
        std::lock_guard 锁(互斥_);
        if (const auto 位置=收集组_.find(执行实例节点); 位置!=收集组_.end()) return 位置->second;
    } catch (...) {}
    return std::nullopt;
}
}
