#include "新_世界树类.h"
#include "../核心/容错检查.h"
#include "../领域/世界挂接.全局根.h"
#include "../适配/适配器.UTC时钟.h"

#include <algorithm>
#include <chrono>
#include <limits>
#include <mutex>
#include <set>
#include <thread>
#include <utility>

namespace 海中鱼巣 {
namespace {

std::recursive_mutex 新世界树互斥;

bool 动态概念处理成功(
    新动态概念自动抽象状态 状态) noexcept {
    return 状态 == 新动态概念自动抽象状态::已采用已有概念
        || 状态 == 新动态概念自动抽象状态::已形成并采用上位概念
        || 状态 == 新动态概念自动抽象状态::已是聚合概念
        || 状态 == 新动态概念自动抽象状态::无同结构候选;
}

// 以下组合入口及凭据生命周期是工程错误的最终责任边界，不交付业务失败值。
[[noreturn]] void 世界I64组合故障(const wchar_t* 原因) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"世界I64组合更新", 原因);
}

新状态强时间 读取组合真实时间() noexcept {
    const auto 时间 = UTC时钟适配器{}.读取当前UTC时间证据_v1({});
    if (!时间.成功()) 世界I64组合故障(L"无法取得真实UTC时间，已发生事实保留。");
    return {新状态时间语义::实例绝对UTC, 时间.证据->UTC纳秒};
}

bool 组合快照相同(const 基础联合节点快照& 左, const 基础联合节点快照& 右) noexcept {
    return 左.结构.节点 == 右.结构.节点 && 左.结构.字段组 == 右.结构.字段组
        && 左.入向外部关系组 == 右.入向外部关系组
        && 左.出向外部关系组 == 右.出向外部关系组 && 左.入向字段组 == 右.入向字段组;
}

基础联合节点快照 读取组合完整保护(稳定编码 节点) {
    const auto 读取 = 全局基础数据集.查询节点组完整结构({节点});
    if (读取.状态 != 基础结构操作状态::已完成 || 读取.节点组.size() != 1
        || 读取.节点组.front().查询节点 != 节点 || !读取.节点组.front().快照)
        世界I64组合故障(L"组合目标完整联合保护不可读。");
    return *读取.节点组.front().快照;
}

std::pair<新特征信息, 稳定编码> 读取组合目标(
    新_场景类& 场景服务, 新_存在类& 存在服务, 新_特征类& 特征服务,
    稳定编码 根场景, const 新世界I64组合请求& 请求) {
    if (!有效(请求.场景节点) || !有效(请求.存在节点) || !有效(请求.特征节点)
        || !有效(请求.动作定义节点) || !有效(请求.动作来源节点) || !有效(请求.执行主体节点)
        || !全局基础数据集.查询节点(请求.动作定义节点)
        || !全局基础数据集.查询节点(请求.动作来源节点)
        || !场景服务.是场景节点(请求.场景节点)
        || !场景服务.存在属于场景范围(根场景, 请求.场景节点)
        || !场景服务.存在属于场景范围(请求.场景节点, 请求.执行主体节点))
        世界I64组合故障(L"组合请求身份或world场景归属不合法。");
    if (请求.存在节点 != 请求.场景节点
        && 场景服务.查询所属场景(请求.存在节点) != std::optional<稳定编码>{请求.场景节点})
        世界I64组合故障(L"组合目标存在不属于指定场景。");
    const auto 存在 = 存在服务.获取存在(请求.存在节点);
    const auto 特征 = 特征服务.获取特征(请求.特征节点);
    if (!存在 || !特征 || 特征->上级特征节点 || !特征->当前值
        || !std::holds_alternative<std::int64_t>(*特征->当前值)
        || std::find(存在->特征节点组.begin(), 存在->特征节点组.end(), 请求.特征节点)
            == 存在->特征节点组.end())
        世界I64组合故障(L"组合目标不是该存在持有的准确I64顶层特征。");
    const auto 定义 = 特征服务.捕获特征匹配定义(特征->特征概念节点);
    if (!定义 || 定义->材料类型 != 特征概念材料物理类型::I64标量)
        世界I64组合故障(L"组合目标正式特征类型不可读或不是I64。");
    return {*特征, 定义->特征类型根};
}

bool 组合场景挂接成功(新场景操作状态 状态) noexcept {
    return 状态 == 新场景操作状态::已完成 || 状态 == 新场景操作状态::无变化;
}

bool 应用需求动作增量(基础联合节点快照& s, const 新事实追加保护增量& d) {
    const auto n = s.结构.节点.编码;
    const auto 加入 = [](auto& v, const auto& x) {
        const auto p = std::find_if(v.begin(), v.end(), [&](const auto& a) { return a.编码 == x.编码; });
        if (p == v.end()) v.push_back(x);
        else if (*p != x) 世界I64组合故障(L"需求动作追加回执冲突");
        std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.编码 < b.编码; });
    };
    for (const auto& f : d.新增字段组) {
        if (f.所属节点 == n) 加入(s.结构.字段组, f);
        const auto* r = std::get_if<稳定编码>(&f.内容);
        if (r && *r == n) 加入(s.入向字段组, f);
    }
    for (const auto& r : d.新增外部关系组) {
        if (r.源节点 == n) 加入(s.出向外部关系组, r);
        if (r.目标节点 == n) 加入(s.入向外部关系组, r);
    }
    for (const auto& f : d.字段变化组) {
        if (f.修改前.所属节点 != n) continue;
        auto p = std::find(s.结构.字段组.begin(), s.结构.字段组.end(), f.修改前);
        if (p == s.结构.字段组.end()) return false;
        *p = f.修改后;
    }
    return true;
}

// 需求的结果留在C；显式动作只在C的一个合法祖先CA挂接，不搬移动作。
bool 捕获需求动作位置(const 新_场景类& scenes, 稳定编码 root, 稳定编码 C,
    稳定编码 H, 稳定编码 actor, 稳定编码 A, 稳定编码& CA,
    std::vector<基础联合节点快照>& protections) {
    const auto capture = [&](稳定编码 n) {
        if (std::none_of(protections.begin(), protections.end(), [&](const auto& x) { return x.结构.节点.编码 == n; }))
            protections.push_back(读取组合完整保护(n));
    };
    const auto unchanged = [&] {
        for (const auto& x : protections)
            if (!组合快照相同(x, 读取组合完整保护(x.结构.节点.编码))) return false;
        return true;
    };
    capture(A); capture(H); capture(actor);
    std::vector<稳定编码> chain;
    const auto path = [&](稳定编码 start, std::vector<稳定编码>& nodes) {
        std::set<稳定编码> seen;
        auto current = start;
        for (;;) {
            if (!seen.insert(current).second) {
                if (!unchanged()) return false;
                世界I64组合故障(L"需求动作场景父链循环");
            }
            capture(current);
            const auto scene = scenes.获取场景(current);
            if (!scene) { if (!unchanged()) return false; 世界I64组合故障(L"需求动作场景父链损坏"); }
            nodes.push_back(current);
            if (current == root) {
                if (scene->父场景节点) 世界I64组合故障(L"需求动作世界根具有父场景");
                return true;
            }
            if (!scene->父场景节点) return false;
            const auto parent = *scene->父场景节点;
            capture(parent);
            const auto p = scenes.获取场景(parent);
            if (!p || std::count(p->直接子场景组.begin(), p->直接子场景组.end(), current) != 1) {
                if (!unchanged()) return false;
                世界I64组合故障(L"需求动作场景父子关系不一致");
            }
            current = parent;
        }
    };
    if (!path(C, chain)) return false;
    std::optional<稳定编码> found;
    for (const auto n : chain) {
        const auto ds = scenes.查询动态列表(n);
        if (std::find(ds.begin(), ds.end(), A) == ds.end()) continue;
        if (found) {
            if (!unchanged()) return false;
            世界I64组合故障(L"需求动作在多个祖先场景重复挂接");
        }
        found = n;
    }
    if (!found) return false;
    // 动作的入向联合结构同时保护挂接唯一性，不把其它业务引用当作挂接。
    const auto action = 读取组合完整保护(A);
    for (const auto& field : action.入向字段组) {
        if (!scenes.是场景节点(field.所属节点)) continue;
        capture(field.所属节点);
        const auto scene = scenes.获取场景(field.所属节点);
        if (!scene) 世界I64组合故障(L"需求动作挂接场景损坏");
        if (std::find(scene->动态节点组.begin(), scene->动态节点组.end(), A) != scene->动态节点组.end()
            && field.所属节点 != *found) {
            if (!unchanged()) return false;
            世界I64组合故障(L"需求动作具有跨场景重复挂接");
        }
    }
    const auto actorScene = scenes.是场景节点(actor) ? std::optional<稳定编码>{actor} : scenes.查询所属场景(actor);
    if (!actorScene) return false;
    std::vector<稳定编码> actorPath;
    if (!path(*actorScene, actorPath)) return false;
    if ((H != C && scenes.查询所属场景(H) != std::optional<稳定编码>{C})
        || !scenes.存在属于场景范围(*found, H) || !scenes.存在属于场景范围(*found, actor)) return false;
    if (!unchanged()) return false;
    CA = *found;
    return true;
}

} // namespace

struct 新世界需求引用组合准备::实现 final {
    const 新_世界树类* 所有者 = nullptr;
    新_特征类* 特征服务 = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    新世界需求引用准备材料 材料;
    新需求发生方式 发生方式=新需求发生方式::新需求;
    std::optional<新特征需求引用更新准备> 特征准备;
    std::optional<基础封存追加片段> 保护片段;
    新状态强时间 动作时间;
    稳定编码 动作场景, 执行主体;
    bool 核心已发表 = false, 已结束 = false;
    void 核对() const noexcept {
        if (线程 != std::this_thread::get_id()) 世界I64组合故障(L"需求组合准备不可跨线程使用");
    }
    ~实现() noexcept {
        核对();
        if (!已结束 && 特征准备) {
            if (核心已发表) 世界I64组合故障(L"需求核心已发表但派生未完成");
            特征服务->取消顶层需求引用更新(*特征准备);
        }
    }
};
新世界需求引用组合准备::新世界需求引用组合准备(std::unique_ptr<实现> p) noexcept : 实现_(std::move(p)) {}
新世界需求引用组合准备::~新世界需求引用组合准备() noexcept = default;
新世界需求引用组合准备::新世界需求引用组合准备(新世界需求引用组合准备&& p) noexcept {
    if (p.实现_) p.实现_->核对();
    实现_ = std::move(p.实现_);
}
新世界需求引用组合准备& 新世界需求引用组合准备::operator=(新世界需求引用组合准备&& p) noexcept {
    if (实现_) 实现_->核对();
    if (p.实现_) p.实现_->核对();
    if (this != &p) 实现_ = std::move(p.实现_);
    return *this;
}

新世界需求引用依据读取结果 新_世界树类::读取需求引用更新依据(
    稳定编码 C, 稳定编码 E, 稳定编码 F) const noexcept {
    using S = 新世界需求引用组合状态;
    try {
        if (!已初始化_ || !有效(C) || !有效(E) || !有效(F)) return {};
        新世界需求引用更新依据 a;
        a.场景节点 = C; a.存在节点 = E; a.特征节点 = F;
        for (const auto n : {C, E})
            if (std::none_of(a.定位保护组.begin(), a.定位保护组.end(), [&](const auto& s) { return s.结构.节点.编码 == n; }))
                a.定位保护组.push_back(读取组合完整保护(n));
        a.特征保护 = 读取组合完整保护(F);
        if (!场景服务_.是场景节点(C) || !场景服务_.存在属于场景范围(世界树根场景_, C)) return {};
        if (C != E && 场景服务_.查询所属场景(E) != std::optional<稳定编码>{C}) return {S::前提已变化, {}};
        const auto e = 存在服务_.获取存在(E);
        const auto f = 特征服务_.获取特征(F);
        if (!e || !f || f->上级特征节点 || std::count(e->特征节点组.begin(), e->特征节点组.end(), F) != 1) return {};
        const auto t = 特征服务_.捕获特征匹配定义(f->特征概念节点);
        if (!t || t->材料类型 != 特征概念材料物理类型::需求节点引用
            || t->比较规则 != 特征概念比较规则::需求身份精确 || t->单位 || f->实例单位) return {};
        a.类型根 = t->特征类型根;
        for (const auto other : e->特征节点组) {
            if (other == F) continue;
            const auto of = 特征服务_.获取特征(other);
            const auto ot = of ? 特征服务_.捕获特征匹配定义(of->特征概念节点) : std::nullopt;
            if (!of || !ot) 世界I64组合故障(L"需求更新存在特征不完整");
            if (ot->特征类型根 == a.类型根) return {};
        }
        for (const auto& v : f->历史不同值) {
            if (!std::holds_alternative<稳定编码>(v)) 世界I64组合故障(L"需求历史值物理类型错误");
            const auto q = 特征服务_.按概念比较特征值(a.类型根, v, v);
            if (q.状态 != 新特征比较状态::已完成 || q.比较值 != 10000) 世界I64组合故障(L"需求历史身份不可读");
        }
        bool valid = false;
        const auto s = 查找最近同类型状态(C, E, a.类型根, valid);
        if (!valid) 世界I64组合故障(L"需求前状态查询失败：状态、概念、时间或结构不合法");
        if (f->当前值) {
            const auto* d = std::get_if<稳定编码>(&*f->当前值);
            if (!d) 世界I64组合故障(L"需求当前值不是节点引用");
            const auto q = 特征服务_.按概念比较特征值(a.类型根, *f->当前值, *f->当前值);
            if (q.状态 != 新特征比较状态::已完成 || q.比较值 != 10000) 世界I64组合故障(L"需求当前身份不可读");
            if (!s || s->来源实例特征节点 != F || s->固定准确值 != f->当前值) return {S::前提已变化, {}};
            a.原需求节点 = *d; a.前状态节点 = s->节点;
            a.定位保护组.push_back(读取组合完整保护(s->节点));
        } else if (s || !f->历史不同值.empty() || f->标准值) return {S::前提已变化, {}};
        a.定位保护组.push_back(读取组合完整保护(f->特征概念节点));
        if (t->特征类型根 != f->特征概念节点) a.定位保护组.push_back(读取组合完整保护(t->特征类型根));
        for (const auto& x : a.定位保护组)
            if (!组合快照相同(x, 读取组合完整保护(x.结构.节点.编码))) return {S::前提已变化, {}};
        if (!组合快照相同(a.特征保护, 读取组合完整保护(F))) return {S::前提已变化, {}};
        return {S::已读取, std::move(a)};
    } catch (...) { 世界I64组合故障(L"需求引用依据读取工程异常"); }
}

新世界需求引用准备结果 新_世界树类::准备需求引用改变(const 新世界需求引用组合请求& r) noexcept {
    using S = 新世界需求引用组合状态;
    try {
        新世界需求引用准备结果 out;
        auto a = 读取需求引用更新依据(r.预期依据.场景节点, r.预期依据.存在节点, r.预期依据.特征节点);
        const auto 同依据 = [](const auto& x, const auto& y) {
            if (x.场景节点 != y.场景节点 || x.存在节点 != y.存在节点 || x.特征节点 != y.特征节点
                || x.类型根 != y.类型根 || x.原需求节点 != y.原需求节点 || x.前状态节点 != y.前状态节点
                || !组合快照相同(x.特征保护, y.特征保护) || x.定位保护组.size() != y.定位保护组.size()) return false;
            for (std::size_t i = 0; i < x.定位保护组.size(); ++i) if (!组合快照相同(x.定位保护组[i], y.定位保护组[i])) return false;
            return true;
        };
        if (a.状态 != S::已读取 || !a.依据 || !同依据(*a.依据, r.预期依据)) return {S::前提已变化, {}, {}};
        auto basis = *a.依据;
        if(r.发生方式!=新需求发生方式::新需求&&r.发生方式!=新需求发生方式::已满足再次需要)return out;
        if(basis.原需求节点==r.新需求节点&&r.发生方式!=新需求发生方式::已满足再次需要)return out;
        const auto v = 特征服务_.按概念比较特征值(basis.类型根, 新特征准确值{r.新需求节点}, 新特征准确值{r.新需求节点});
        if (v.状态 != 新特征比较状态::已完成 || v.比较值 != 10000) 世界I64组合故障(L"新需求引用身份不合法");
        if (!有效(r.动作定义节点) || !有效(r.动作来源节点) || !有效(r.执行主体节点)
            || (!r.既有动作节点 && !场景服务_.存在属于场景范围(basis.场景节点, r.执行主体节点))) return out;
        std::vector<基础联合节点快照> extra;
        for (const auto n : {r.动作定义节点, r.动作来源节点, r.执行主体节点}) extra.push_back(读取组合完整保护(n));
        auto now = 读取组合真实时间();
        if (basis.前状态节点) {
            const auto s = 状态服务_.获取状态(*basis.前状态节点);
            if (!s) 世界I64组合故障(L"需求前状态丢失");
            if (now.纳秒 <= s->强时间.纳秒) return {S::前提已变化, {}, {}};
        }
        std::vector<新事实追加保护增量> deltas;
        auto 既有动作=r.既有动作节点;
        if(!既有动作) {
            const auto 旧=动态服务_.按来源查询动作(r.动作来源节点);
            if(旧.状态!=新动态操作状态::已完成)世界I64组合故障(L"需求动作来源查询未完成");
            if(旧.动作)既有动作=旧.动作->节点;
        }
        if (既有动作) out.已发生动作节点 = 既有动作;
        else {
            新事实追加保护增量 d, c;
            const auto act = 动态服务_.建立动作动态({r.执行主体节点, r.动作定义节点, r.动作来源节点,
                basis.存在节点, basis.特征节点, now}, d);
            if (act.状态 != 新动态操作状态::已完成 || !act.动态节点) 世界I64组合故障(L"需求发布动作未建立");
            out.已发生动作节点 = *act.动态节点;
            if (!组合场景挂接成功(场景服务_.添加动态(basis.场景节点, *act.动态节点, c))) 世界I64组合故障(L"需求发布动作未挂接");
            deltas.push_back(std::move(d)); deltas.push_back(std::move(c));
        }
        const auto act = 动态服务_.获取动态(*out.已发生动作节点);
        const auto acts = 场景服务_.查询动态列表(basis.场景节点);
        if (!act || act->种类 != 新动态种类::动作 || act->主题存在节点 != r.执行主体节点
            || act->动作目标种类 != 新动作目标种类::普通存在 || act->动作目标存在节点 != basis.存在节点
            || act->动作目标特征节点 != basis.特征节点 || act->动作定义节点 != r.动作定义节点
            || act->动作来源节点 != r.动作来源节点 || !act->动作时间 || !act->动作结果组.empty()
            || act->动作时间->语义 != 新状态时间语义::实例绝对UTC || act->动作时间->纳秒 > now.纳秒
            || (!既有动作 && std::count(acts.begin(), acts.end(), *out.已发生动作节点) != 1)) { out.状态 = S::前提已变化; return out; }
        if (basis.前状态节点) {
            const auto pre = 状态服务_.获取状态(*basis.前状态节点);
            if (!pre || act->动作时间->纳秒 <= pre->强时间.纳秒) { out.状态 = S::前提已变化; return out; }
        }
        for (const auto& d : deltas) {
            if (!应用需求动作增量(basis.特征保护, d)) 世界I64组合故障(L"需求F自有回执失配");
            for (auto& x : basis.定位保护组) if (!应用需求动作增量(x, d)) 世界I64组合故障(L"需求定位自有回执失配");
            for (auto& x : extra) if (!应用需求动作增量(x, d)) 世界I64组合故障(L"需求动作来源回执失配");
        }
        auto fresh = 读取需求引用更新依据(basis.场景节点, basis.存在节点, basis.特征节点);
        if (fresh.状态 != S::已读取 || !fresh.依据 || !同依据(*fresh.依据, basis)) { out.状态 = S::前提已变化; return out; }
        auto actionScene = basis.场景节点;
        if (既有动作 && !捕获需求动作位置(场景服务_, 世界树根场景_, basis.场景节点,
            basis.存在节点, r.执行主体节点, *out.已发生动作节点, actionScene, extra)) {
            out.状态 = S::前提已变化; return out;
        }
        基础节点组追加请求 protect;
        protect.预期联合节点组 = basis.定位保护组; protect.预期联合节点组.push_back(basis.特征保护);
        protect.预期联合节点组.insert(protect.预期联合节点组.end(), extra.begin(), extra.end());
        protect.预期联合节点组.push_back(读取组合完整保护(*out.已发生动作节点));
        // 去重时必须同一快照，不以最后一次读取覆盖竞争。
        std::sort(protect.预期联合节点组.begin(), protect.预期联合节点组.end(), [](const auto& x, const auto& y) { return x.结构.节点.编码 < y.结构.节点.编码; });
        for (std::size_t i = 1; i < protect.预期联合节点组.size(); ++i)
            if (protect.预期联合节点组[i-1].结构.节点.编码 == protect.预期联合节点组[i].结构.节点.编码
                && !组合快照相同(protect.预期联合节点组[i-1], protect.预期联合节点组[i])) { out.状态 = S::前提已变化; return out; }
        protect.预期联合节点组.erase(std::unique(protect.预期联合节点组.begin(), protect.预期联合节点组.end(), [](const auto& x, const auto& y) { return x.结构.节点.编码 == y.结构.节点.编码; }), protect.预期联合节点组.end());
        const auto seal = 全局基础数据集.封存原子追加片段(protect);
        if (seal.状态 != 基础节点组追加状态::已完成 || !seal.片段) 世界I64组合故障(L"需求定位保护封存失败");
        auto f = 特征服务_.准备顶层需求引用更新(basis.特征节点, r.新需求节点);
        if (f.状态 == 新特征I64准备状态::预期已变化 || f.状态 == 新特征I64准备状态::联合更新占用) { out.状态 = S::前提已变化; return out; }
        if (f.状态 != 新特征I64准备状态::已准备 || !f.准备) 世界I64组合故障(L"需求特征准备未完成");
        auto p = std::make_unique<新世界需求引用组合准备::实现>();
        p->所有者 = this; p->特征服务 = &特征服务_; p->材料 = {basis, r.新需求节点, *out.已发生动作节点, basis.特征保护};
        p->发生方式=r.发生方式;
        p->动作时间 = *act->动作时间; p->保护片段 = *seal.片段; p->特征准备 = std::move(f.准备);
        p->动作场景 = actionScene; p->执行主体 = r.执行主体节点;
        out.准备 = 新世界需求引用组合准备{std::move(p)}; out.状态 = S::已准备; return out;
    } catch (...) { 世界I64组合故障(L"需求引用准备工程异常"); }
}

新世界需求引用准备材料 新_世界树类::读取需求引用组合准备(const 新世界需求引用组合准备& p) const noexcept {
    if (!p.实现_ || p.实现_->所有者 != this || p.实现_->已结束) 世界I64组合故障(L"需求组合准备owner不符");
    p.实现_->核对(); return p.实现_->材料;
}

新世界需求引用组合结果 新_世界树类::提交需求引用改变并形成状态动态(
    新世界需求引用组合准备& p, const 新需求集合发布准备& 需求准备) noexcept {
    using S = 新世界需求引用组合状态;
    try {
        const auto m = 读取需求引用组合准备(p); auto& x = *p.实现_;
        const 新需求发生核心定位 预期{m.依据.场景节点,m.依据.存在节点,m.依据.特征节点,m.新需求节点,m.动作节点,x.发生方式};
        if(需求准备.发生定位()!=预期)世界I64组合故障(L"需求发生核准准备与世界定位不一致");
        const auto& source=需求准备.片段();
        const bool 同值=m.依据.原需求节点==m.新需求节点;
        if(同值&&x.发生方式!=新需求发生方式::已满足再次需要)世界I64组合故障(L"同值需求发生未经again核准");
        const auto commit = 特征服务_.提交顶层需求引用更新(*x.特征准备, {*x.保护片段, source});
        if (commit.状态 == 基础节点组追加状态::预期不符) {
            特征服务_.取消顶层需求引用更新(*x.特征准备); x.已结束 = true; return {S::前提已变化, {}, {}};
        }
        if (commit.状态 != 基础节点组追加状态::已完成) 世界I64组合故障(L"需求及来源联合提交失败");
        x.核心已发表 = true;
        std::optional<基础片段发布回执> receipt;
        for (const auto& r : commit.附加片段回执组) if (r.对应片段(source)) {
            if (receipt) 世界I64组合故障(L"需求来源片段回执重复"); receipt = r;
        }
        if (!receipt) 世界I64组合故障(L"需求来源片段回执缺失");
        const auto f = 特征服务_.完成顶层需求引用更新(*x.特征准备);
        if (f.状态 != 新特征写入状态::已完成 || !f.命中次数已增加 || f.当前值已改变==同值) 世界I64组合故障(L"需求核心完成不符核准发生");
        auto time = 读取组合真实时间();
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        while (time.纳秒 <= x.动作时间.纳秒) {
            if (time.纳秒 < x.动作时间.纳秒 || std::chrono::steady_clock::now() >= deadline) 世界I64组合故障(L"需求后状态UTC不推进");
            std::this_thread::yield(); time = 读取组合真实时间();
        }
        const auto e = 同值
            ? 存在服务_.完成已提交同值需求发生(m.依据.存在节点,m.依据.特征节点,f,m.依据.类型根,m.新需求节点,time)
            : 存在服务_.完成已提交需求引用特征变化(m.依据.存在节点, m.依据.特征节点, f, m.依据.类型根, m.新需求节点, time);
        if (e.状态 != 新存在操作状态::已完成 || !e.状态结果 || !e.状态结果->状态节点) 世界I64组合故障(L"需求已发表但EC/S派生失败");
        const auto s = *e.状态结果->状态节点;
        if (!组合场景挂接成功(场景服务_.添加状态(m.依据.场景节点, s))) 世界I64组合故障(L"需求后状态挂接失败");
        新世界特征变化结果 change;
        change.前状态节点 = m.依据.前状态节点; change.新状态节点 = s;
        change.特征更新结果 = 新场景特征值更新结果{新场景操作状态::已完成, e, s};
        if (m.依据.前状态节点&&!同值) {
            change.动态发布结果 = 完成动态发布(m.依据.场景节点, 动态服务_.建立原子动态(m.依据.存在节点, *m.依据.前状态节点, s));
            if (!change.动态发布结果->成功() || !change.动态发布结果->动态建立结果 || !change.动态发布结果->动态建立结果->动态节点) 世界I64组合故障(L"需求变化动态未完成");
            const auto D = *change.动态发布结果->动态建立结果->动态节点;
            std::vector<基础联合节点快照> protection;
            auto CA = x.动作场景;
            if (!捕获需求动作位置(场景服务_, 世界树根场景_, m.依据.场景节点,
                m.依据.存在节点, x.执行主体, m.动作节点, CA, protection) || CA != x.动作场景)
                世界I64组合故障(L"需求核心已发表但动作场景前提变化");
            for (const auto n : {D, s, m.依据.特征节点}) protection.push_back(读取组合完整保护(n));
            const auto dynamic = 动态服务_.获取动态(D);
            const auto post = 状态服务_.获取状态(s);
            const auto act = 动态服务_.获取动态(m.动作节点);
            const auto ds = 场景服务_.查询动态列表(m.依据.场景节点);
            const auto ss = 场景服务_.查询状态列表(m.依据.场景节点);
            if (!dynamic || dynamic->种类 != 新动态种类::原子 || dynamic->主题存在节点 != m.依据.存在节点
                || dynamic->状态节点组 != std::vector<稳定编码>{*m.依据.前状态节点, s}
                || std::count(ds.begin(), ds.end(), D) != 1 || std::count(ss.begin(), ss.end(), s) != 1
                || !post || post->来源实例特征节点 != m.依据.特征节点 || post->被描述存在节点 != m.依据.存在节点
                || post->强时间 != time || !act || act->动作时间 != std::optional<新状态强时间>{x.动作时间}
                || !act->动作定义节点 || !act->动作来源节点
                || act->动作目标存在节点 != m.依据.存在节点 || act->动作目标特征节点 != m.依据.特征节点)
                世界I64组合故障(L"需求动作结果关联前事实不一致");
            protection.push_back(读取组合完整保护(*act->动作定义节点));
            protection.push_back(读取组合完整保护(*act->动作来源节点));
            基础节点组追加请求 expected;
            expected.预期联合节点组 = std::move(protection);
            std::sort(expected.预期联合节点组.begin(), expected.预期联合节点组.end(),
                [](const auto& a, const auto& b) { return a.结构.节点.编码 < b.结构.节点.编码; });
            for (std::size_t i = 1; i < expected.预期联合节点组.size(); ++i)
                if (expected.预期联合节点组[i-1].结构.节点.编码 == expected.预期联合节点组[i].结构.节点.编码
                    && !组合快照相同(expected.预期联合节点组[i-1], expected.预期联合节点组[i]))
                    世界I64组合故障(L"需求已发表但动作结果组合快照冲突");
            expected.预期联合节点组.erase(std::unique(expected.预期联合节点组.begin(), expected.预期联合节点组.end(),
                [](const auto& a, const auto& b) { return a.结构.节点.编码 == b.结构.节点.编码; }), expected.预期联合节点组.end());
            const auto sealed = 全局基础数据集.封存原子追加片段(expected);
            if (sealed.状态 != 基础节点组追加状态::已完成 || !sealed.片段
                || 全局基础数据集.原子追加封存片段组({*sealed.片段}).状态 != 基础节点组追加状态::已完成)
                世界I64组合故障(L"需求已发表但动作结果位置保护变化");
            // 只在本需求专用路径允许合法祖先CA→结果C；公共同场景wrapper保持不变。
            change.动作关联结果 = 动态服务_.关联动作结果(m.动作节点, D, 新动作结果关系类型::导致变化);
            if (*change.动作关联结果 != 新动态操作状态::已完成 && *change.动作关联结果 != 新动态操作状态::无变化) 世界I64组合故障(L"需求动作结果未关联");
            // 正式owner只修改A/D；若位置等其它前提并发漂移，保留已发表事实并工程中止。
            for (const auto& snapshot : expected.预期联合节点组) {
                const auto node = snapshot.结构.节点.编码;
                if (node != m.动作节点 && node != D
                    && !组合快照相同(snapshot, 读取组合完整保护(node)))
                    世界I64组合故障(L"需求动作结果已关联但场景前提变化");
            }
        }
        const auto actual = 状态服务_.获取状态(s);
        if (!actual || actual->来源实例特征节点 != m.依据.特征节点 || actual->被描述存在节点 != m.依据.存在节点
            || actual->固定准确值 != 新特征准确值{m.新需求节点} || actual->强时间 != time) 世界I64组合故障(L"需求后S读回不符");
        change.状态 = 同值 || m.依据.前状态节点 ? 新世界特征变化状态::已完成 : 新世界特征变化状态::已生成状态但无可连接前状态;
        特征服务_.结束顶层需求引用更新(*x.特征准备); x.已结束 = true;
        return {S::已完成, std::move(change), std::move(receipt)};
    } catch (...) { 世界I64组合故障(L"需求引用联合提交派生工程异常"); }
}

新需求特征事实核验结果 新_世界树类::核验需求特征发布事实(const 新需求特征事实核验输入& in) const noexcept {
    using S = 新需求特征事实核验状态;
    try {
        if (!已初始化_ || !有效(in.场景节点) || !有效(in.主体存在节点)
            || !有效(in.新特征节点) || !有效(in.当前需求节点)) return {};
        const bool first = in.用途 == 新需求特征事实用途::首次状态;
        const bool migration = in.用途 == 新需求特征事实用途::旧集合迁移;
        const bool migratedState = in.用途 == 新需求特征事实用途::迁移当前状态;
        if ((!first && !migration && !migratedState)
            || (first && (有效(in.原特征节点) || !in.有序来源组.empty() || !in.动作节点 || !in.实际状态节点))
            || (!first && (!有效(in.原特征节点) || in.有序来源组.empty() || in.动作节点))
            || (migration && in.实际状态节点) || (migratedState && !in.实际状态节点)) return {};
        std::vector<基础联合节点快照> protections;
        const auto capture = [&](稳定编码 n) {
            const auto p = std::find_if(protections.begin(), protections.end(), [&](const auto& s) { return s.结构.节点.编码 == n; });
            if (p == protections.end()) protections.push_back(读取组合完整保护(n));
        };
        for (const auto n : {in.场景节点, in.主体存在节点, in.新特征节点}) capture(n);
        if (!场景服务_.是场景节点(in.场景节点) || !场景服务_.存在属于场景范围(世界树根场景_, in.场景节点)) return {};
        if (in.主体存在节点 != in.场景节点 && 场景服务_.查询所属场景(in.主体存在节点) != in.场景节点) return {S::前提已变化, {}};
        const auto e = 存在服务_.获取存在(in.主体存在节点);
        const auto f = 特征服务_.获取特征(in.新特征节点);
        if (!e || !f || f->上级特征节点 || std::count(e->特征节点组.begin(), e->特征节点组.end(), in.新特征节点) != 1) return {};
        capture(f->特征概念节点);
        const auto ft = 特征服务_.捕获特征匹配定义(f->特征概念节点);
        if (!ft || ft->材料类型 != 特征概念材料物理类型::需求节点引用 || ft->比较规则 != 特征概念比较规则::需求身份精确 || ft->单位 || f->实例单位) return {};
        capture(ft->特征类型根);
        const auto scenes = 场景服务_.查询状态列表(in.场景节点);
        const auto dynamics = 场景服务_.查询动态列表(in.场景节点);
        bool actionLocationChanged = false;
        const auto state = [&](稳定编码 n, 稳定编码 source) -> std::optional<新状态信息> {
            capture(n);
            const auto s = 状态服务_.获取状态(n);
            if (!s || s->被描述存在节点 != in.主体存在节点 || s->来源实例特征节点 != source
                || s->强时间.语义 != 新状态时间语义::实例绝对UTC || std::count(scenes.begin(), scenes.end(), n) != 1) return {};
            return s;
        };
        const auto action = [&](稳定编码 n, 稳定编码 source) -> std::optional<新动态信息> {
            capture(n);
            const auto a = 动态服务_.获取动态(n);
            if (!a || a->种类 != 新动态种类::动作 || a->动作目标种类 != 新动作目标种类::普通存在
                || a->动作目标存在节点 != in.主体存在节点 || a->动作目标特征节点 != source
                || !a->动作定义节点 || !a->动作来源节点 || !a->动作时间
                || a->动作时间->语义 != 新状态时间语义::实例绝对UTC
                || (!first && std::count(dynamics.begin(), dynamics.end(), n) != 1)) return {};
            capture(a->主题存在节点); capture(*a->动作定义节点); capture(*a->动作来源节点);
            if (first) {
                auto CA = in.场景节点;
                if (!捕获需求动作位置(场景服务_, 世界树根场景_, in.场景节点,
                    in.主体存在节点, a->主题存在节点, n, CA, protections)) { actionLocationChanged = true; return {}; }
            } else if (!场景服务_.存在属于场景范围(in.场景节点, a->主题存在节点)) return {};
            return a;
        };
        if (first) {
            const auto s = state(*in.实际状态节点, in.新特征节点);
            const auto a = action(*in.动作节点, in.新特征节点);
            if (actionLocationChanged) return {S::前提已变化, {}};
            if (!s || !a || s->固定准确值 != 新特征准确值{in.当前需求节点}
                || s->强时间.纳秒 <= a->动作时间->纳秒 || !a->动作结果组.empty()) return {};
            // 首值只允许该F的唯一状态，不能拿后来任意状态补首S。
            for (const auto n : scenes) {
                if (n == s->节点) continue;
                capture(n);
                const auto other = 状态服务_.获取状态(n);
                if (!other) 世界I64组合故障(L"首需求状态列表损坏");
                if (other->被描述存在节点 == in.主体存在节点 && other->来源实例特征节点 == in.新特征节点) return {};
            }
            if (f->当前值 != std::optional<新特征准确值>{新特征准确值{in.当前需求节点}}
                || f->历史不同值 != std::vector<新特征准确值>{新特征准确值{in.当前需求节点}}) return {S::前提已变化, {}};
        } else {
            capture(in.原特征节点);
            const auto old = 特征服务_.获取特征(in.原特征节点);
            if (!old || old->上级特征节点 || !old->当前值 || std::count(e->特征节点组.begin(), e->特征节点组.end(), in.原特征节点) != 1) return {};
            capture(old->特征概念节点);
            const auto ot = 特征服务_.捕获特征匹配定义(old->特征概念节点);
            if (!ot || ot->比较规则 != 特征概念比较规则::需求集合身份判等) return {};
            capture(ot->特征类型根);
            const auto members = [&](const 新特征准确值& v) -> std::optional<std::vector<稳定编码>> {
                const auto* id = std::get_if<稳定编码>(&v); if (!id) return {};
                capture(*id);
                const auto material = 特征值服务_.获取特征值(*id);
                if (!material || material->物理类型 != 材料物理类型::U64数组) return {};
                const auto* raw = std::get_if<std::vector<std::uint64_t>>(&material->材料); if (!raw) return {};
                std::vector<稳定编码> result;
                for (const auto n : *raw) { if (!n) return {}; result.push_back(稳定编码{n}); }
                std::sort(result.begin(), result.end());
                if (std::adjacent_find(result.begin(), result.end()) != result.end()) return {};
                return result;
            };
            std::vector<std::vector<稳定编码>> prefixes{{}};
            std::vector<稳定编码> current, seenActions, seenStates;
            std::optional<std::int64_t> lastTime;
            std::optional<新状态信息> last;
            for (const auto& item : in.有序来源组) {
                if (!有效(item.需求节点) || std::find(current.begin(), current.end(), item.需求节点) != current.end()
                    || std::find(seenActions.begin(), seenActions.end(), item.动作节点) != seenActions.end()) return {};
                capture(item.需求节点);
                const auto a = action(item.动作节点, in.原特征节点);
                const auto pre = state(item.前状态节点, in.原特征节点), post = state(item.后状态节点, in.原特征节点);
                if (!a || !pre || !post || a->动作结果组.size() != 1
                    || a->动作结果组.front().类型 != 新动作结果关系类型::导致变化
                    || pre->强时间.纳秒 >= post->强时间.纳秒
                    || a->动作时间->纳秒 < pre->强时间.纳秒 || a->动作时间->纳秒 >= post->强时间.纳秒
                    || (lastTime && (pre->强时间.纳秒 < *lastTime
                        || (pre->强时间.纳秒 == *lastTime && last && pre->节点 != last->节点)))) return {};
                if (!pre->固定准确值 || !post->固定准确值) return {};
                const auto before = members(*pre->固定准确值), after = members(*post->固定准确值);
                if (!before || !after || *before != current) return {};
                auto expected = current; expected.push_back(item.需求节点); std::sort(expected.begin(), expected.end());
                if (*after != expected) return {};
                const auto d = a->动作结果组.front().结果动态节点; capture(d);
                const auto dynamic = 动态服务_.获取动态(d);
                if (!dynamic || dynamic->种类 != 新动态种类::原子 || dynamic->主题存在节点 != in.主体存在节点
                    || dynamic->状态节点组 != std::vector<稳定编码>{item.前状态节点, item.后状态节点}
                    || std::count(dynamics.begin(), dynamics.end(), d) != 1) return {};
                if (pre->特征类型概念节点 != old->特征概念节点 || post->特征类型概念节点 != old->特征概念节点) {
                    const auto ptype = 特征服务_.捕获特征匹配定义(pre->特征类型概念节点);
                    const auto qtype = 特征服务_.捕获特征匹配定义(post->特征类型概念节点);
                    if (!ptype || !qtype || ptype->特征类型根 != ot->特征类型根 || qtype->特征类型根 != ot->特征类型根) return {};
                    capture(pre->特征类型概念节点); capture(post->特征类型概念节点);
                }
                seenActions.push_back(item.动作节点); seenStates.push_back(item.前状态节点); seenStates.push_back(item.后状态节点);
                current = std::move(expected); prefixes.push_back(current); lastTime = post->强时间.纳秒; last = post;
            }
            if (!last || in.有序来源组.back().需求节点 != in.当前需求节点) return {};
            const auto oldCurrent = members(*old->当前值);
            if (!oldCurrent || *oldCurrent != current) return {S::前提已变化, {}};
            std::vector<std::vector<稳定编码>> historicalSets;
            for (const auto& v : old->历史不同值) {
                const auto m = members(v);
                if (!m || std::find(prefixes.begin(), prefixes.end(), *m) == prefixes.end()) return {};
                const auto legal = 特征服务_.按概念比较特征值(ot->特征类型根, v, v);
                if (legal.状态 != 新特征比较状态::已完成 || legal.比较值 != 10000) 世界I64组合故障(L"旧需求集合身份不可读");
                historicalSets.push_back(*m);
            }
            for (const auto& prefix : prefixes)
                if (std::find(historicalSets.begin(), historicalSets.end(), prefix) == historicalSets.end()) return {};
            // 不接受未解释的旧F状态或其他首次发布动作。
            for (const auto n : scenes) {
                capture(n); const auto s = 状态服务_.获取状态(n);
                if (!s) 世界I64组合故障(L"迁移场景状态不完整");
                if (s->被描述存在节点 == in.主体存在节点 && s->来源实例特征节点 == in.原特征节点) {
                    if (std::find(seenStates.begin(), seenStates.end(), n) == seenStates.end()) return {};
                    if (!s->固定准确值) return {};
                    const auto m = members(*s->固定准确值);
                    if (!m || std::find(prefixes.begin(), prefixes.end(), *m) == prefixes.end()) return {};
                }
            }
            for (const auto n : dynamics) {
                capture(n); const auto a = 动态服务_.获取动态(n);
                if (!a) 世界I64组合故障(L"迁移场景动作不完整");
                if (a->种类 == 新动态种类::动作 && a->动作目标存在节点 == in.主体存在节点
                    && a->动作目标特征节点 == in.原特征节点 && !a->动作结果组.empty()
                    && std::find(seenActions.begin(), seenActions.end(), n) == seenActions.end()) return {};
            }
            if (migratedState) {
                const auto s = state(*in.实际状态节点, in.新特征节点);
                if (!s || s->固定准确值 != 新特征准确值{in.当前需求节点} || s->强时间 != last->强时间
                    || f->当前值 != std::optional<新特征准确值>{新特征准确值{in.当前需求节点}}) return {};
            }
        }
        // 只预期不写：封存快照仍须在签发前以L1验证，消费方再次共同CAS。
        基础节点组追加请求 req; req.预期联合节点组 = protections;
        const auto sealed = 全局基础数据集.封存原子追加片段(req);
        if (sealed.状态 != 基础节点组追加状态::已完成 || !sealed.片段) 世界I64组合故障(L"需求事实保护封存失败");
        const auto check = 全局基础数据集.原子追加封存片段组({*sealed.片段});
        if (check.状态 == 基础节点组追加状态::预期不符) return {S::前提已变化, {}};
        if (check.状态 != 基础节点组追加状态::已完成) 世界I64组合故障(L"需求事实保护验证失败");
        新需求特征事实依据 basis{in, std::move(protections), *sealed.片段};
        return {S::已核验, 新需求特征事实凭据{std::move(basis), this}};
    } catch (...) { 世界I64组合故障(L"需求发布事实核验工程异常"); }
}

新世界特征变化结果 新_世界树类::完成需求引用迁移派生(
    const 新需求特征迁移派生凭据& source, 新特征需求引用更新准备& recovery) noexcept {
    try {
        新世界特征变化结果 out;
        const auto& b = source.依据();
        const auto token = 特征服务_.读取需求引用历史恢复准备(recovery);
        if (token.状态 != 新特征写入状态::已完成 || token.特征节点 != b.新特征节点
            || token.当前需求节点 != b.当前需求节点 || !token.核心已提交 || !token.恢复已完成)
            世界I64组合故障(L"需求迁移派生不持有同owner已完成恢复准备");
        if (!已初始化_ || !场景服务_.是场景节点(b.场景节点) || !场景服务_.存在属于场景范围(世界树根场景_, b.场景节点)) return out;
        const auto e = 存在服务_.获取存在(b.主体存在节点);
        const auto f = 特征服务_.获取特征(b.新特征节点);
        const auto t = f ? 特征服务_.捕获特征匹配定义(f->特征概念节点) : std::nullopt;
        if (!e || !f || !t || t->特征类型根 != b.需求类型根 || t->材料类型 != 特征概念材料物理类型::需求节点引用
            || t->比较规则 != 特征概念比较规则::需求身份精确 || t->单位 || f->实例单位
            || f->当前值 != std::optional<新特征准确值>{新特征准确值{b.当前需求节点}}
            || std::count(e->特征节点组.begin(), e->特征节点组.end(), b.新特征节点) != 1
            || (b.主体存在节点 != b.场景节点 && 场景服务_.查询所属场景(b.主体存在节点) != b.场景节点)) return out;
        const auto s = 状态服务_.获取状态(b.原末状态节点);
        const auto ss = 场景服务_.查询状态列表(b.场景节点);
        if (!s || s->被描述存在节点 != b.主体存在节点 || s->来源实例特征节点 != b.原特征节点
            || s->强时间.语义 != 新状态时间语义::实例绝对UTC || std::count(ss.begin(), ss.end(), b.原末状态节点) != 1) return out;
        for (const auto n : ss) {
            const auto previous = 状态服务_.获取状态(n);
            if (!previous) 世界I64组合故障(L"需求迁移已有状态不可读");
            if (previous->被描述存在节点 == b.主体存在节点 && previous->来源实例特征节点 == b.新特征节点)
                世界I64组合故障(L"需求迁移已有状态无本次直接回执，不能重建或猜测");
        }
        if (!组合快照相同(读取组合完整保护(b.新特征节点), b.新特征保护)) { out.状态 = 新世界特征变化状态::前提已变化; return out; }
        const auto checked = 全局基础数据集.原子追加封存片段组({b.来源保护片段});
        if (checked.状态 == 基础节点组追加状态::预期不符) { out.状态 = 新世界特征变化状态::前提已变化; return out; }
        if (checked.状态 != 基础节点组追加状态::已完成) 世界I64组合故障(L"需求迁移来源保护核验失败");
        // 不接受时间参数：只沿凭据指明的真实旧末状态恢复。
        const auto result = 存在服务_.完成已恢复需求引用特征变化(b.主体存在节点, b.新特征节点, b.需求类型根, b.当前需求节点, s->强时间);
        if (result.状态 != 新存在操作状态::已完成 || !result.状态结果 || !result.状态结果->状态节点) 世界I64组合故障(L"需求迁移核心已发表但状态派生失败");
        const auto n = *result.状态结果->状态节点;
        if (!组合场景挂接成功(场景服务_.添加状态(b.场景节点, n))) 世界I64组合故障(L"需求迁移状态挂接失败");
        const auto actual = 状态服务_.获取状态(n);
        if (!actual || actual->固定准确值 != 新特征准确值{b.当前需求节点} || actual->强时间 != s->强时间
            || actual->来源实例特征节点 != b.新特征节点 || actual->被描述存在节点 != b.主体存在节点) 世界I64组合故障(L"需求迁移状态读回不符");
        out.新状态节点 = n; out.特征更新结果 = 新场景特征值更新结果{新场景操作状态::已完成, result, n};
        out.状态 = 新世界特征变化状态::已生成状态但无可连接前状态;
        // publisher完成映射、完整读回后才释放recovery；这里不结束F占用。
        return out;
    } catch (...) { 世界I64组合故障(L"需求迁移派生工程异常"); }
}

struct 新世界I64组合准备::实现 final {
    const 新_世界树类* 所有者 = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    新世界I64组合请求 请求;
    新世界I64组合准备材料 材料;
    新特征信息 原特征;
    稳定编码 特征类型根;
    std::optional<新状态信息> 前状态;
    std::optional<新动态信息> 动作;
    bool 已完成 = false;

    void 核对线程() const noexcept {
        if (线程 != std::this_thread::get_id())
            世界I64组合故障(L"世界组合准备不能跨线程移动、读取、提交或析构。");
    }
    void 核对可释放() const noexcept {
        核对线程();
        if (!已完成) 世界I64组合故障(L"组合准备尚未完整提交，不得丢弃或覆盖已发表现场。");
    }
};

新世界I64组合准备::新世界I64组合准备(std::unique_ptr<实现> 内容) noexcept
    : 实现_(std::move(内容)) {}

新世界I64组合准备::~新世界I64组合准备() noexcept {
    if (实现_) 实现_->核对可释放();
}

新世界I64组合准备::新世界I64组合准备(新世界I64组合准备&& 其它) noexcept {
    if (其它.实现_) 其它.实现_->核对线程();
    实现_ = std::move(其它.实现_);
}

新世界I64组合准备& 新世界I64组合准备::operator=(新世界I64组合准备&& 其它) noexcept {
    if (实现_) 实现_->核对线程();
    if (其它.实现_) 其它.实现_->核对线程();
    if (this != &其它) {
        if (实现_) 实现_->核对可释放();
        实现_ = std::move(其它.实现_);
    }
    return *this;
}

struct 新世界引用集合组合准备::实现 final {
    const 新_世界树类* 所有者 = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    新世界引用集合组合准备材料 材料;
    新世界引用集合更新依据 依据;
    std::vector<基础联合节点快照> 本次新增保护组;
    新动态信息 动作;
    新状态信息 前状态;
    bool 已消费 = false;
    bool 核心已发布 = false;
    bool 已完成 = false;
    void 核对线程() const noexcept {
        if (线程 != std::this_thread::get_id()) 世界I64组合故障(L"集合组合准备不能跨线程使用");
    }
    void 核对释放() const noexcept {
        核对线程();
        if (核心已发布 && !已完成) 世界I64组合故障(L"集合核心已发布但尚未完成派生");
    }
};
新世界引用集合组合准备::新世界引用集合组合准备(std::unique_ptr<实现> 内容) noexcept
    : 实现_(std::move(内容)) {}
新世界引用集合组合准备::新世界引用集合组合准备(新世界引用集合组合准备&& 其它) noexcept {
    if (其它.实现_) 其它.实现_->核对线程();
    实现_ = std::move(其它.实现_);
}
新世界引用集合组合准备& 新世界引用集合组合准备::operator=(新世界引用集合组合准备&& 其它) noexcept {
    if (实现_) 实现_->核对释放();
    if (其它.实现_) 其它.实现_->核对线程();
    if (this != &其它) 实现_ = std::move(其它.实现_);
    return *this;
}
新世界引用集合组合准备::~新世界引用集合组合准备() noexcept {
    if (实现_) 实现_->核对释放();
}

新世界引用集合依据读取结果 新_世界树类::读取存在引用集合更新依据(
    稳定编码 场景, 稳定编码 存在, 稳定编码 特征) const noexcept {
    try {
        新世界引用集合依据读取结果 结果;
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !场景服务_.是场景节点(场景)
            || !场景服务_.存在属于场景范围(世界树根场景_, 场景)
            || (存在 != 场景 && 场景服务_.查询所属场景(存在) != std::optional<稳定编码>{场景}))
            return 结果;
        const auto E = 存在服务_.获取存在(存在);
        const auto 完整F = 特征服务_.捕获引用集合更新保护(特征);
        if (完整F.状态 == 新特征引用集合保护状态::前提已变化)
            return {新世界引用集合组合状态::前提已变化, {}};
        if (完整F.状态 != 新特征引用集合保护状态::已读取 || !完整F.信息)
            世界I64组合故障(L"集合完整特征依据读取失败");
        const auto F = 特征服务_.获取特征(特征);
        if (!E || !F || F->上级特征节点 || !F->当前值
            || std::find(E->特征节点组.begin(), E->特征节点组.end(), 特征) == E->特征节点组.end())
            return 结果;
        const auto 定义 = 特征服务_.捕获特征匹配定义(F->特征概念节点);
        const auto* 材料节点 = std::get_if<稳定编码>(&*F->当前值);
        if (!定义 || (定义->比较规则 != 特征概念比较规则::需求集合身份判等 && 定义->比较规则 != 特征概念比较规则::存在集合身份判等) || !材料节点)
            return 结果;
        const auto 材料 = 特征值服务_.获取特征值(*材料节点);
        const auto* 数组 = 材料 ? std::get_if<std::vector<std::uint64_t>>(&材料->材料) : nullptr;
        if (!数组) 世界I64组合故障(L"集合当前准确材料不可读");
        const auto 合法 = 特征服务_.按概念比较特征值(定义->特征类型根, *F->当前值, *F->当前值);
        if (合法.状态 != 新特征比较状态::已完成 || 合法.比较值 != 10000)
            世界I64组合故障(L"集合当前值成员不合法");
        std::vector<稳定编码> 节点组{场景, 存在, 特征};
        std::sort(节点组.begin(), 节点组.end());
        节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
        const auto 快照 = 全局基础数据集.查询节点组完整结构(节点组);
        if (快照.状态 != 基础结构操作状态::已完成 || 快照.节点组.size() != 节点组.size())
            世界I64组合故障(L"集合更新依据完整读取失败");
        新世界引用集合更新依据 依据;
        依据.特征完整保护组 = 完整F.信息->完整保护组;
        依据.场景节点 = 场景; 依据.存在节点 = 存在; 依据.特征节点 = 特征;
        依据.类型根 = 定义->特征类型根; 依据.旧准确材料节点 = *材料节点;
        for (const auto n : *数组) 依据.旧成员组.push_back(稳定编码{n});
        std::sort(依据.旧成员组.begin(), 依据.旧成员组.end());
        依据.旧成员组.erase(std::unique(依据.旧成员组.begin(), 依据.旧成员组.end()), 依据.旧成员组.end());
        for (const auto& 项 : 快照.节点组) {
            if (!项.快照) return {新世界引用集合组合状态::前提已变化, {}};
            if (项.查询节点 == 特征) 依据.特征保护 = *项.快照;
            else 依据.定位保护组.push_back(*项.快照);
        }
        if (特征服务_.获取特征(特征) != F || 存在服务_.获取存在(存在) != E
            || 特征服务_.捕获特征匹配定义(F->特征概念节点) != 定义)
            return {新世界引用集合组合状态::前提已变化, {}};
        for (const auto& 项 : 依据.定位保护组)
            if (!组合快照相同(项, 读取组合完整保护(项.结构.节点.编码)))
                return {新世界引用集合组合状态::前提已变化, {}};
        if (!组合快照相同(依据.特征保护, 读取组合完整保护(特征)))
            return {新世界引用集合组合状态::前提已变化, {}};
        for (const auto& 项 : 依据.特征完整保护组)
            if (!组合快照相同(项, 读取组合完整保护(项.结构.节点.编码)))
                return {新世界引用集合组合状态::前提已变化, {}};
        结果.状态 = 新世界引用集合组合状态::已读取; 结果.依据 = std::move(依据);
        return 结果;
    } catch (...) { 世界I64组合故障(L"读取集合更新依据遇到工程异常"); }
}

新世界引用集合准备结果 新_世界树类::准备存在引用集合改变(
    const 新世界引用集合组合请求& 请求) noexcept {
    try {
        using S = 新世界引用集合组合状态;
        新世界引用集合准备结果 结果;
        auto 原 = 请求.预期依据;
        auto 当前 = 读取存在引用集合更新依据(原.场景节点, 原.存在节点, 原.特征节点);
        const auto 同依据 = [&](const 新世界引用集合更新依据& a) {
            if (a.类型根 != 原.类型根 || a.旧准确材料节点 != 原.旧准确材料节点
                || a.旧成员组 != 原.旧成员组 || !组合快照相同(a.特征保护, 原.特征保护)
                || a.定位保护组.size() != 原.定位保护组.size()
                || a.特征完整保护组.size() != 原.特征完整保护组.size()) return false;
            for (std::size_t i = 0; i < a.定位保护组.size(); ++i)
                if (!组合快照相同(a.定位保护组[i], 原.定位保护组[i])) return false;
            for (std::size_t i = 0; i < a.特征完整保护组.size(); ++i)
                if (!组合快照相同(a.特征完整保护组[i], 原.特征完整保护组[i])) return false;
            return true;
        };
        if (当前.状态 != S::已读取 || !当前.依据 || !同依据(*当前.依据)) {
            结果.状态 = S::前提已变化; return 结果;
        }
        if (!有效(请求.动作定义节点) || !有效(请求.动作来源节点)
            || !全局基础数据集.查询节点(请求.动作定义节点)
            || !全局基础数据集.查询节点(请求.动作来源节点)
            || !场景服务_.存在属于场景范围(原.场景节点, 请求.执行主体节点)) return 结果;
        const auto 根定义 = 特征概念服务_.获取特征概念(原.类型根);
        if (根定义 && 根定义->比较规则 == 特征概念比较规则::存在集合身份判等) {
            std::vector<基础联合节点快照> 旧依据;
            const auto 加入依据 = [&](const 基础联合节点快照& 项) {
                const auto 位置 = std::find_if(旧依据.begin(), 旧依据.end(),
                    [&](const auto& 已有) { return 已有.结构.节点.编码 == 项.结构.节点.编码; });
                if (位置 != 旧依据.end()) return 组合快照相同(*位置, 项);
                旧依据.push_back(项);
                return true;
            };
            if (!加入依据(原.特征保护)) return {S::前提已变化, {}};
            for (const auto& 项 : 原.定位保护组)
                if (!加入依据(项)) return {S::前提已变化, {}};
            for (const auto& 项 : 原.特征完整保护组)
                if (!加入依据(项)) return {S::前提已变化, {}};
            auto 保护 = 特征概念服务_.准备存在集合材料保护(
                原.类型根, 请求.新准确材料节点, 特征值服务_, 旧依据);
            if (保护.状态 == 特征概念规则状态::前提已变化)
                return {S::前提已变化, {}};
            if (保护.状态 != 特征概念规则状态::已完成 || !保护.凭据)
                世界I64组合故障(L"集合准备未取得正式成员保护凭据");
            const auto 推进 = 特征概念服务_.消费存在集合保护依据(std::move(*保护.凭据));
            if (推进.状态 == 特征概念规则状态::前提已变化)
                return {S::前提已变化, {}};
            if (推进.状态 != 特征概念规则状态::已完成
                || 推进.推进后依据.size() != 旧依据.size())
                世界I64组合故障(L"集合保护依据推进未返回完整结果");
            // 只接受FC凭据证明的自身增量，不用当前快照替换调用方原依据。
            for (const auto& 旧 : 旧依据) {
                if (std::count_if(推进.推进后依据.begin(), 推进.推进后依据.end(),
                    [&](const auto& 项) { return 项.结构.节点.编码 == 旧.结构.节点.编码; }) != 1)
                    世界I64组合故障(L"集合保护依据推进身份缺失或重复");
            }
            const auto 应用推进 = [&](基础联合节点快照& 项) {
                项 = *std::find_if(推进.推进后依据.begin(), 推进.推进后依据.end(),
                    [&](const auto& 新项) { return 新项.结构.节点.编码 == 项.结构.节点.编码; });
            };
            应用推进(原.特征保护);
            for (auto& 项 : 原.定位保护组) 应用推进(项);
            for (auto& 项 : 原.特征完整保护组) 应用推进(项);
            当前 = 读取存在引用集合更新依据(原.场景节点, 原.存在节点, 原.特征节点);
            if (当前.状态 != S::已读取 || !当前.依据 || !同依据(*当前.依据))
                return {S::前提已变化, {}};
        }
        const auto 比较 = 特征服务_.按概念比较特征值(原.类型根,
            新特征准确值{原.旧准确材料节点}, 新特征准确值{请求.新准确材料节点});
        if (比较.状态 != 新特征比较状态::已完成) 世界I64组合故障(L"集合准备新材料不合法");
        if (比较.比较值 == 10000) return 结果;
        auto 内容 = std::make_unique<新世界引用集合组合准备::实现>();
        内容->所有者 = this;
        auto& m = 内容->材料;
        m.场景节点 = 原.场景节点; m.存在节点 = 原.存在节点; m.特征节点 = 原.特征节点;
        m.前准确材料节点 = 原.旧准确材料节点; m.后准确材料节点 = 请求.新准确材料节点;
        std::vector<新事实追加保护增量> 回执组;
        const auto 应用回执 = [&](基础联合节点快照& 预期) {
            const auto 节点 = 预期.结构.节点.编码;
            const auto 加入 = [](auto& 组, const auto& 项) {
                const auto it = std::find_if(组.begin(), 组.end(), [&](const auto& a) { return a.编码 == 项.编码; });
                if (it != 组.end()) {
                    if (*it != 项) 世界I64组合故障(L"本次追加回执身份冲突");
                } else 组.push_back(项);
                std::sort(组.begin(), 组.end(), [](const auto& a, const auto& b) { return a.编码 < b.编码; });
            };
            for (const auto& 回执 : 回执组) {
                for (const auto& f : 回执.新增字段组) {
                    if (f.所属节点 == 节点) 加入(预期.结构.字段组, f);
                    const auto* 引用 = std::get_if<稳定编码>(&f.内容);
                    if (引用 && *引用 == 节点) 加入(预期.入向字段组, f);
                }
                for (const auto& r : 回执.新增外部关系组) {
                    if (r.源节点 == 节点) 加入(预期.出向外部关系组, r);
                    if (r.目标节点 == 节点) 加入(预期.入向外部关系组, r);
                }
                for (const auto& 修改 : 回执.字段变化组) {
                    if (修改.修改前.所属节点 != 节点) continue;
                    auto it = std::find(预期.结构.字段组.begin(), 预期.结构.字段组.end(), 修改.修改前);
                    if (it == 预期.结构.字段组.end()) return false;
                    *it = 修改.修改后;
                }
            }
            return true;
        };
        const auto 允许自有增量 = [&](const 基础联合节点快照& 新, 基础联合节点快照 预期) {
            if (!应用回执(预期)) return false;
            return 组合快照相同(新, 预期);
        };
        const auto 刷新 = [&]() {
            auto 读 = 读取存在引用集合更新依据(原.场景节点, 原.存在节点, 原.特征节点);
            if (读.状态 != S::已读取 || !读.依据) return false;
            const auto& a = *读.依据;
            if (a.旧准确材料节点 != 原.旧准确材料节点 || a.旧成员组 != 原.旧成员组
                || a.类型根 != 原.类型根 || !允许自有增量(a.特征保护, 原.特征保护)
                || a.定位保护组.size() != 原.定位保护组.size()) return false;
            for (std::size_t i = 0; i < a.定位保护组.size(); ++i)
                if (!允许自有增量(a.定位保护组[i], 原.定位保护组[i])) return false;
            if (a.特征完整保护组.size() != 原.特征完整保护组.size()) return false;
            for (std::size_t i = 0; i < a.特征完整保护组.size(); ++i)
                if (!允许自有增量(a.特征完整保护组[i], 原.特征完整保护组[i])) return false;
            std::vector<基础联合节点快照> 新节点保护;
            for (const auto& 回执 : 回执组) for (const auto n : 回执.新建节点组) {
                基础联合节点快照 预期;
                预期.结构.节点.编码 = n;
                if (!应用回执(预期)) return false;
                if (!组合快照相同(读取组合完整保护(n), 预期)) return false;
                新节点保护.push_back(std::move(预期));
            }
            内容->本次新增保护组 = std::move(新节点保护);
            内容->依据 = std::move(*读.依据); return true;
        };
        bool 有效结构 = false;
        auto 前 = 查找最近同类型状态(原.场景节点, 原.存在节点, 原.类型根, 有效结构);
        if (!有效结构) 世界I64组合故障(L"集合前状态列表不完整");
        if (!前 || 前->固定准确值 != 新特征准确值{原.旧准确材料节点}) {
            新事实追加保护增量 取样增量, 挂接增量;
            const auto 取样 = 存在服务_.建立当前引用集合特征状态(
                原.存在节点, 原.特征节点, 原.类型根, 原.旧准确材料节点, 读取组合真实时间(), 取样增量);
            if (取样.状态结果 && 取样.状态结果->状态节点) {
                结果.已取样前状态节点 = *取样.状态结果->状态节点;
                if (!组合场景挂接成功(场景服务_.添加状态(原.场景节点, *结果.已取样前状态节点, 挂接增量)))
                    世界I64组合故障(L"集合真实前状态挂接失败");
                回执组.push_back(std::move(取样增量));
                回执组.push_back(std::move(挂接增量));
            }
            if (!刷新()) { 结果.状态 = S::前提已变化; return 结果; }
            if (取样.状态 != 新存在操作状态::已完成 || !结果.已取样前状态节点)
                世界I64组合故障(L"集合当前取样未完成");
            前 = 状态服务_.获取状态(*结果.已取样前状态节点);
        }
        if (!前 || 前->来源实例特征节点 != 原.特征节点
            || 前->固定准确值 != 新特征准确值{原.旧准确材料节点})
            世界I64组合故障(L"集合前状态读回不符");
        if (!刷新()) { 结果.状态 = S::前提已变化; return 结果; }
        const auto 时间 = 读取组合真实时间();
        if (时间.纳秒 < 前->强时间.纳秒) 世界I64组合故障(L"集合动作时间倒退");
        if (请求.既有动作节点) {
            const auto& 动作 = *请求.既有动作节点;
            const auto 已有 = 动态服务_.获取动态(动作);
            const auto 场景动态 = 场景服务_.查询动态列表(原.场景节点);
            if (!已有 || 已有->种类 != 新动态种类::动作 || !已有->动作时间
                || 已有->动作时间->语义 != 新状态时间语义::实例绝对UTC
                || 已有->动作时间->纳秒 < 前->强时间.纳秒
                || 已有->动作时间->纳秒 > 时间.纳秒
                || 已有->主题存在节点 != 请求.执行主体节点
                || 已有->动作定义节点 != 请求.动作定义节点
                || 已有->动作来源节点 != 请求.动作来源节点
                || 已有->动作目标种类 != 新动作目标种类::普通存在
                || 已有->动作目标存在节点 != 原.存在节点
                || 已有->动作目标特征节点 != 原.特征节点
                || !已有->动作结果组.empty()
                || std::count(场景动态.begin(), 场景动态.end(), 动作) != 1)
                return {S::前提已变化, {}, {}};
            结果.已发生动作节点 = 动作;
        } else {
            新事实追加保护增量 动作增量, 动作挂接增量;
            const auto 动作 = 动态服务_.建立动作动态({请求.执行主体节点,
                请求.动作定义节点, 请求.动作来源节点, 原.存在节点, 原.特征节点, 时间}, 动作增量);
            if (动作.状态 != 新动态操作状态::已完成 || !动作.动态节点)
                世界I64组合故障(L"集合动作未新建，不能重放旧动作");
            结果.已发生动作节点 = *动作.动态节点;
            if (!组合场景挂接成功(场景服务_.添加动态(原.场景节点, *动作.动态节点, 动作挂接增量)))
                世界I64组合故障(L"集合动作挂接失败");
            回执组.push_back(std::move(动作增量));
            回执组.push_back(std::move(动作挂接增量));
            if (!刷新()) { 结果.状态 = S::前提已变化; return 结果; }
        }
        const auto 动作信息 = 动态服务_.获取动态(*结果.已发生动作节点);
        if (!动作信息 || 动作信息->种类 != 新动态种类::动作
            || 动作信息->主题存在节点 != 请求.执行主体节点
            || 动作信息->动作定义节点 != 请求.动作定义节点
            || 动作信息->动作来源节点 != 请求.动作来源节点
            || !动作信息->动作时间
            || (!请求.既有动作节点 && 动作信息->动作时间 != 时间)
            || 动作信息->动作目标种类 != 新动作目标种类::普通存在
            || 动作信息->动作目标任务虚拟存在
            || 动作信息->动作目标存在节点 != 原.存在节点
            || 动作信息->动作目标特征节点 != 原.特征节点 || !动作信息->动作结果组.empty())
            世界I64组合故障(L"集合动作正式读回不符");
        内容->动作 = *动作信息; 内容->前状态 = *前;
        m.动作节点 = *结果.已发生动作节点; m.前状态节点 = 前->节点;
        m.核心时间 = 读取组合真实时间(); m.特征保护 = 内容->依据.特征保护;
        结果.状态 = S::已准备;
        结果.准备 = 新世界引用集合组合准备{std::move(内容)};
        return 结果;
    } catch (...) { 世界I64组合故障(L"集合准备遇到工程异常，保留真实取样与动作"); }
}

新世界引用集合组合准备材料 新_世界树类::读取存在引用集合组合准备(
    const 新世界引用集合组合准备& 准备) const noexcept {
    if (!准备.实现_) 世界I64组合故障(L"读取集合准备为空");
    准备.实现_->核对线程();
    if (准备.实现_->所有者 != this) 世界I64组合故障(L"集合准备owner不符");
    return 准备.实现_->材料;
}

新世界引用集合组合结果 新_世界树类::提交存在引用集合改变并形成动态(
    新世界引用集合组合准备& 准备, const 基础封存追加片段& 附加) noexcept {
    return 提交引用集合改变内部(准备, &附加);
}
新世界引用集合组合结果 新_世界树类::提交存在引用集合改变并形成动态(
    新世界引用集合组合准备& 准备) noexcept {
    return 提交引用集合改变内部(准备, nullptr);
}
新世界引用集合组合结果 新_世界树类::提交引用集合改变内部(
    新世界引用集合组合准备& 准备, const 基础封存追加片段* 附加) noexcept {
    try {
        using S = 新世界引用集合组合状态;
        if (!准备.实现_) 世界I64组合故障(L"集合提交缺少准备");
        auto& 内容 = *准备.实现_; 内容.核对线程();
        if (内容.所有者 != this || 内容.已消费) 世界I64组合故障(L"集合准备重复消费或owner不符");
        内容.已消费 = true;
        const auto& m = 内容.材料;
        const auto 当前 = 读取存在引用集合更新依据(m.场景节点, m.存在节点, m.特征节点);
        if (当前.状态 != S::已读取 || !当前.依据
            || 当前.依据->旧准确材料节点 != m.前准确材料节点
            || !组合快照相同(当前.依据->特征保护, m.特征保护)
            || 当前.依据->定位保护组.size() != 内容.依据.定位保护组.size())
            return {S::前提已变化, {}, {}};
        for (std::size_t i = 0; i < 内容.依据.定位保护组.size(); ++i)
            if (!组合快照相同(当前.依据->定位保护组[i], 内容.依据.定位保护组[i]))
                return {S::前提已变化, {}, {}};
        if (当前.依据->特征完整保护组.size() != 内容.依据.特征完整保护组.size())
            return {S::前提已变化, {}, {}};
        for (std::size_t i = 0; i < 内容.依据.特征完整保护组.size(); ++i)
            if (!组合快照相同(当前.依据->特征完整保护组[i], 内容.依据.特征完整保护组[i]))
                return {S::前提已变化, {}, {}};
        for (const auto& 项 : 内容.本次新增保护组)
            if (!组合快照相同(读取组合完整保护(项.结构.节点.编码), 项))
                return {S::前提已变化, {}, {}};
        if (动态服务_.获取动态(m.动作节点) != std::optional<新动态信息>{内容.动作}
            || 状态服务_.获取状态(m.前状态节点) != std::optional<新状态信息>{内容.前状态})
            return {S::前提已变化, {}, {}};
        auto F准备 = 特征服务_.准备顶层引用集合更新(m.特征节点, m.后准确材料节点);
        if (F准备.状态 == 新特征I64准备状态::预期已变化
            || F准备.状态 == 新特征I64准备状态::联合更新占用)
            return {S::前提已变化, {}, {}};
        if (F准备.状态 != 新特征I64准备状态::已准备 || !F准备.准备)
            世界I64组合故障(L"集合特征核心准备异常");
        // F准备取得自身完整保护后再次核验调用方旧依据，防止准备窗口覆盖竞争写。
        if (!组合快照相同(读取组合完整保护(m.特征节点), m.特征保护)) {
            特征服务_.取消顶层引用集合更新(*F准备.准备);
            return {S::前提已变化, {}, {}};
        }
        for (const auto& 项 : 内容.依据.特征完整保护组) {
            if (!组合快照相同(读取组合完整保护(项.结构.节点.编码), 项)) {
                特征服务_.取消顶层引用集合更新(*F准备.准备);
                return {S::前提已变化, {}, {}};
            }
        }
        const auto 提交 = 特征服务_.提交顶层引用集合更新(*F准备.准备,
            附加 ? std::vector<基础封存追加片段>{*附加} : std::vector<基础封存追加片段>{});
        if (提交.状态 == 基础节点组追加状态::预期不符) {
            特征服务_.取消顶层引用集合更新(*F准备.准备);
            return {S::前提已变化, {}, {}};
        }
        if (提交.状态 != 基础节点组追加状态::已完成
            || 提交.附加片段回执组.size() != (附加 ? 1u : 0u)
            || (附加 && !提交.附加片段回执组.front().对应片段(*附加)))
            世界I64组合故障(L"集合与需求来源联合发布失败");
        内容.核心已发布 = true;
        const auto F结果 = 特征服务_.完成顶层引用集合更新(*F准备.准备);
        if (F结果.状态 != 新特征写入状态::已完成 || !F结果.当前值已改变)
            世界I64组合故障(L"集合已发布但历史聚合收尾失败");
        auto 后时间 = 读取组合真实时间();
        const auto 截止 = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        for (;;) {
            if (后时间.纳秒 < 内容.前状态.强时间.纳秒
                || 后时间.纳秒 < m.核心时间.纳秒 || 后时间.纳秒 < 内容.动作.动作时间->纳秒)
                世界I64组合故障(L"集合后状态真实时间倒退");
            if (后时间.纳秒 > 内容.前状态.强时间.纳秒
                && 后时间.纳秒 > 内容.动作.动作时间->纳秒) break;
            if (std::chrono::steady_clock::now() >= 截止)
                世界I64组合故障(L"集合后状态真实时钟在一秒内未推进");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            后时间 = 读取组合真实时间();
        }
        const auto E结果 = 存在服务_.完成已提交引用集合特征变化(
            m.存在节点, m.特征节点, F结果, 内容.依据.类型根, m.后准确材料节点, 后时间);
        if (E结果.状态 != 新存在操作状态::已完成 || !E结果.状态结果 || !E结果.状态结果->状态节点)
            世界I64组合故障(L"集合已发表但存在概念或后状态未完成");
        const auto 后S = *E结果.状态结果->状态节点;
        if (!组合场景挂接成功(场景服务_.添加状态(m.场景节点, 后S)))
            世界I64组合故障(L"集合后状态挂接失败");
        新世界动态发布结果 D结果;
        D结果.动态建立结果 = 动态服务_.建立原子动态(m.存在节点, m.前状态节点, 后S);
        if (D结果.动态建立结果->状态 != 新动态操作状态::已完成 || !D结果.动态建立结果->动态节点)
            世界I64组合故障(L"集合真实变化动态未完成");
        const auto D = *D结果.动态建立结果->动态节点;
        D结果.场景挂接结果 = 场景服务_.添加动态(m.场景节点, D);
        if (!组合场景挂接成功(*D结果.场景挂接结果)) 世界I64组合故障(L"集合动态挂接失败");
        D结果.概念处理结果 = 动态概念抽象服务_.处理新动态(D);
        if (!动态概念处理成功(D结果.概念处理结果->状态)) 世界I64组合故障(L"集合动态概念处理失败");
        switch (D结果.概念处理结果->状态) {
        case 新动态概念自动抽象状态::已采用已有概念:
            D结果.状态 = 新世界动态发布状态::已完成并采用已有概念; break;
        case 新动态概念自动抽象状态::已形成并采用上位概念:
            D结果.状态 = 新世界动态发布状态::已完成并形成上位概念; break;
        case 新动态概念自动抽象状态::已是聚合概念:
            D结果.状态 = 新世界动态发布状态::已完成且已有聚合概念; break;
        default: D结果.状态 = 新世界动态发布状态::已完成并保留实例概念; break;
        }
        const auto 关联 = 动态服务_.关联动作结果(m.动作节点, D, 新动作结果关系类型::导致变化);
        if (关联 != 新动态操作状态::已完成 && 关联 != 新动态操作状态::无变化)
            世界I64组合故障(L"集合动作结果关联失败");
        const auto 动作 = 动态服务_.获取动态(m.动作节点);
        const auto 动态 = 动态服务_.获取动态(D);
        const auto 状态 = 状态服务_.获取状态(后S);
        if (!动作 || 动作->动作结果组 != std::vector<新动作结果关联>{{新动作结果关系类型::导致变化, D}}
            || !动态 || 动态->主题存在节点 != m.存在节点
            || 动态->状态节点组 != std::vector<稳定编码>{m.前状态节点, 后S}
            || !状态 || 状态->来源实例特征节点 != m.特征节点
            || 状态->固定准确值 != 新特征准确值{m.后准确材料节点})
            世界I64组合故障(L"集合动作动态状态完整读回失败");
        const auto D组 = 场景服务_.查询动态列表(m.场景节点);
        const auto S组 = 场景服务_.查询状态列表(m.场景节点);
        if (std::count(D组.begin(), D组.end(), D) != 1 || std::count(D组.begin(), D组.end(), m.动作节点) != 1
            || std::count(S组.begin(), S组.end(), 后S) != 1 || std::count(S组.begin(), S组.end(), m.前状态节点) != 1)
            世界I64组合故障(L"集合场景组织完整读回失败");
        新世界特征变化结果 变化;
        变化.状态 = 新世界特征变化状态::已完成;
        变化.前状态节点 = m.前状态节点; 变化.新状态节点 = 后S;
        变化.特征更新结果 = 新场景特征值更新结果{新场景操作状态::已完成, E结果, 后S};
        变化.动态发布结果 = std::move(D结果); 变化.动作关联结果 = 关联;
        特征服务_.结束顶层引用集合更新(*F准备.准备);
        内容.已完成 = true;
        return {S::已完成, std::move(变化), 附加
            ? std::optional<基础片段发布回执>{提交.附加片段回执组.front()} : std::nullopt};
    } catch (...) { 世界I64组合故障(L"集合组合发布或派生发生工程异常，保留真实事实"); }
}

bool 新世界动态发布结果::成功() const noexcept {
    return 状态 == 新世界动态发布状态::已完成并保留实例概念
        || 状态 == 新世界动态发布状态::已完成并采用已有概念
        || 状态 == 新世界动态发布状态::已完成并形成上位概念
        || 状态 == 新世界动态发布状态::已完成且已有聚合概念;
}

bool 新世界动作发布结果::成功() const noexcept {
    return 状态 == 新世界动作发布状态::已完成
        || 状态 == 新世界动作发布状态::精确重复;
}

新_世界树类::新_世界树类() noexcept
    : 特征值服务_(全局基础数据集),
      特征服务_(特征值服务_, 特征概念服务_),
      二次特征概念服务_(特征概念服务_, 特征值服务_),
      二次特征服务_(特征服务_, 二次特征概念服务_),
      存在概念服务_(特征概念服务_),
      词服务_(语言服务_),
      名称关系服务_(词服务_, 特征概念服务_, 存在概念服务_),
      状态服务_(特征服务_),
      存在服务_(特征服务_, 存在概念服务_, 状态服务_),
      动态概念服务_(
          特征概念服务_, 特征值服务_, 二次特征概念服务_, &存在概念服务_),
      动态服务_(
          状态服务_, 动态概念服务_, 存在服务_, 二次特征服务_, 特征概念服务_),
      场景服务_(
          世界树根场景_, 存在服务_, 状态服务_, 动态服务_),
      因果服务_(状态服务_,动态服务_,存在服务_,存在概念服务_,特征概念服务_,
          动态概念服务_,特征值服务_,场景服务_),
      动态特征抽取服务_(
          动态服务_, 状态服务_, 二次特征服务_, 动态概念服务_, 二次特征概念服务_, 特征概念服务_),
      动态概念抽象服务_(
          动态服务_, 动态概念服务_, 二次特征概念服务_,
          动态特征抽取服务_) {
    特征概念服务_.装配存在引用读取接口(存在服务_);
    特征服务_.装配存在引用读取接口(存在服务_);
}

新_世界树类::~新_世界树类() = default;

新世界初始化结果 新_世界树类::初始化() noexcept {
    新世界初始化结果 结果;
    try {
        std::lock_guard 锁(新世界树互斥);
        const bool 依赖完整 = 特征值服务_.初始化()
            && 特征概念服务_.初始化(特征值服务_)
            && 特征服务_.初始化()
            && 二次特征概念服务_.初始化()
            && 存在概念服务_.初始化()
            && 语言服务_.初始化()
            && 词服务_.初始化()
            && 名称关系服务_.初始化()
            && 状态服务_.初始化()
            && 存在服务_.初始化()
            && 动态概念服务_.初始化()
            && 动态服务_.初始化()
            && 场景服务_.初始化()
            && 因果服务_.初始化();
        if (!依赖完整) {
            结果.状态 = 新世界初始化状态::依赖初始化失败;
            return 结果;
        }
        const auto 映射全局根状态 = [](全局根操作状态 状态) noexcept {
            return 状态 == 全局根操作状态::资源失败
                ? 新世界初始化状态::资源失败 : 新世界初始化状态::根场景建立失败;
        };
        const auto 全局根 = 新_全局根类::初始化();
        if (全局根 != 全局根操作状态::已完成 && 全局根 != 全局根操作状态::无变化) {
            结果.状态 = 映射全局根状态(全局根);
            return 结果;
        }
        const auto 根结果 = 场景服务_.建立根场景();
        if (根结果.状态 != 新场景操作状态::已完成
            && 根结果.状态 != 新场景操作状态::无变化) {
            结果.状态 = 根结果.状态 == 新场景操作状态::结构不一致
                ? 新世界初始化状态::结构不一致
                : 新世界初始化状态::根场景建立失败;
            return 结果;
        }
        if (!根结果.场景节点 || !根结果.专属存在概念节点) {
            结果.状态 = 新世界初始化状态::结构不一致;
            return 结果;
        }
        const auto 挂接 = 新_全局根类::挂接世界根(世界类型登记键::现实, *根结果.场景节点);
        if (挂接 != 全局根操作状态::已完成 && 挂接 != 全局根操作状态::无变化) {
            结果.状态 = 映射全局根状态(挂接);
            return 结果;
        }
        已初始化_ = true;
        结果.状态 = 根结果.状态 == 新场景操作状态::已完成
            ? 新世界初始化状态::已完成
            : 新世界初始化状态::无变化;
        结果.世界树根场景 = 根结果.场景节点;
        结果.根场景专属存在概念 = 根结果.专属存在概念节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新世界初始化状态::资源失败;
        return 结果;
    }
}

bool 新_世界树类::已初始化() const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_
            && 场景服务_.是场景节点(世界树根场景_);
    } catch (...) {
        return false;
    }
}

std::optional<稳定编码> 新_世界树类::获取世界树根场景() const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ && 场景服务_.是场景节点(世界树根场景_)
            ? std::optional<稳定编码>{世界树根场景_} : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

新世界事实服务引用 新_世界树类::获取事实服务引用() noexcept {
    return {特征值服务_, 特征概念服务_, 特征服务_,
        二次特征概念服务_, 存在概念服务_, 语言服务_,
        词服务_, 名称关系服务_, 状态服务_,
        存在服务_, 动态服务_, 场景服务_,因果服务_, 动态概念服务_};
}

std::optional<先天特征概念集合>
新_世界树类::获取先天特征概念() const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 特征概念服务_.获取先天特征概念()
            : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

稳定编码 新_世界树类::建立特征概念(
    const 特征概念定义& 定义,
    std::optional<稳定编码> 上位概念) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_) return {};
        return 特征概念服务_.建立或取得特征概念(
            定义, 特征值服务_, 上位概念);
    } catch (...) {
        return {};
    }
}

特征概念编码操作结果 新_世界树类::建立特征概念强类型(
    const 特征概念定义& 定义,
    std::optional<稳定编码> 上位概念) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_) return {特征概念规则状态::入口拒绝, {}};
        return 特征概念服务_.建立或取得特征概念强类型(
            定义, 特征值服务_, 上位概念);
    } catch (...) {
        世界I64组合故障(L"世界特征概念强类型建立发生工程异常");
    }
}

新场景建立结果 新_世界树类::建立子场景(
    稳定编码 父场景节点) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.建立场景(父场景节点)
            : 新场景建立结果{};
    } catch (...) {
        return {};
    }
}

新场景成员建立结果 新_世界树类::建立场景内存在(
    稳定编码 场景节点,
    const 新存在建立请求& 请求,
    const 新状态强时间& 初始状态时间) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_
            ? 场景服务_.建立成员存在(场景节点, 请求, 初始状态时间)
            : 新场景成员建立结果{};
    } catch (...) {
        return {};
    }
}

新场景操作状态 新_世界树类::迁移子场景(
    稳定编码 场景节点,
    稳定编码 新父场景节点) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_
            ? 场景服务_.迁移子场景(场景节点, 新父场景节点)
            : 新场景操作状态::入口拒绝;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_世界树类::迁移存在(
    稳定编码 存在节点,
    稳定编码 目标场景节点) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        // 已登记自我只经迁移自我改变场景，保证自我所在场景随同一次迁移更新。
        if (自我节点_ && *自我节点_ == 存在节点) return 新场景操作状态::入口拒绝;
        return 已初始化_
            ? 场景服务_.迁移成员存在(存在节点, 目标场景节点)
            : 新场景操作状态::入口拒绝;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_世界树类::登记自我(
    稳定编码 自我节点,
    稳定编码 场景节点) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !有效(自我节点) || !有效(场景节点)) return 新场景操作状态::入口拒绝;
        if (自我节点_) {
            return *自我节点_ == 自我节点 && 自我原场景_ == std::optional{场景节点}
                ? 新场景操作状态::无变化 : 新场景操作状态::入口拒绝;
        }
        if (场景服务_.查询所属场景(自我节点) != std::optional{场景节点})
            return 新场景操作状态::结构不一致;
        自我节点_ = 自我节点;
        自我所在场景_ = 场景节点;
        自我原场景_ = 场景节点;
        return 新场景操作状态::已完成;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

std::optional<稳定编码> 新_世界树类::读取自我所在场景() const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 自我所在场景_;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<稳定编码> 新_世界树类::读取自我原场景() const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 自我原场景_;
    } catch (...) {
        return std::nullopt;
    }
}

新场景成员建立结果 新_世界树类::查找标识副本(
    稳定编码 场景节点, 稳定编码 标识类型, 稳定编码 外设节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !有效(场景节点) || !有效(标识类型) || !有效(外设节点)
            || 外设节点.值 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()))
            return {};
        const auto 场景 = 场景服务_.获取场景(场景节点);
        if (!场景) return {新场景操作状态::场景不存在, {}, {}, {}};
        std::optional<稳定编码> 命中;
        for (const auto 成员 : 场景->直接成员存在组) {
            const auto 存在 = 存在服务_.获取存在(成员);
            if (!存在) return {新场景操作状态::结构不一致, {}, {}, {}};
            for (const auto 特征节点 : 存在->特征节点组) {
                const auto 特征 = 特征服务_.获取特征(特征节点);
                if (!特征) return {新场景操作状态::结构不一致, {}, {}, {}};
                const auto 根 = 特征概念服务_.比较单特征概念(
                    特征->特征概念节点, 特征->特征概念节点).特征类型根;
                if (根 != std::optional{标识类型} || !特征->当前值) continue;
                const auto* 值 = std::get_if<std::int64_t>(&*特征->当前值);
                if (!值 || *值 != static_cast<std::int64_t>(外设节点.值)) continue;
                if (命中) return {新场景操作状态::结构不一致, {}, {}, {}};
                命中 = 成员;
            }
        }
        if (!命中) return {新场景操作状态::存在不存在, {}, {}, {}};
        return {新场景操作状态::无变化, 命中, {}, {}};
    } catch (...) {
        return {新场景操作状态::资源失败, {}, {}, {}};
    }
}

新场景成员建立结果 新_世界树类::建立或取得现实交互场景(
    稳定编码 外设场景节点, 稳定编码 外设场景标识类型) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !自我原场景_ || !有效(外设场景节点) || !有效(外设场景标识类型)
            || 外设场景节点.值 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()))
            return {};
        const auto 原场景 = 场景服务_.获取场景(*自我原场景_);
        if (!原场景) return {新场景操作状态::结构不一致, {}, {}, {}};
        // 同一外设交互场景只对应一个现实交互场景：在自我原场景的直接子场景中按标识查找。
        std::optional<std::pair<稳定编码, 稳定编码>> 已有;
        for (const auto 子场景 : 原场景->直接子场景组) {
            const auto 副本 = 查找标识副本(子场景, 外设场景标识类型, 外设场景节点);
            if (副本.状态 == 新场景操作状态::存在不存在) continue;
            if (副本.状态 != 新场景操作状态::无变化 || !副本.存在节点 || 已有)
                return {新场景操作状态::结构不一致, {}, {}, {}};
            已有 = std::pair{子场景, *副本.存在节点};
        }
        if (已有) return {新场景操作状态::无变化, 已有->second, {}, {}, };
        const auto 场景 = 场景服务_.建立场景(*自我原场景_);
        if (场景.状态 != 新场景操作状态::已完成 || !场景.场景节点)
            return {场景.状态 == 新场景操作状态::已完成 ? 新场景操作状态::结构不一致 : 场景.状态, {}, {}, {}};
        新存在建立请求 请求;
        请求.初始特征组.push_back({外设场景标识类型, static_cast<std::int64_t>(外设场景节点.值)});
        auto 窗口 = 场景服务_.建立成员存在(*场景.场景节点, 请求, 读取组合真实时间());
        if (窗口.状态 != 新场景操作状态::已完成 || !窗口.存在节点) {
            (void)场景服务_.删除空场景(*场景.场景节点);
            return {窗口.状态 == 新场景操作状态::已完成 ? 新场景操作状态::结构不一致 : 窗口.状态, {}, {}, {}};
        }
        // 必需特征“参与自我”“参与交互者”随场景建立，暂无当前值（待成立的现实交互场景）。
        if (参与自我类型_ && 参与交互者类型_)
            for (const auto 类型 : {*参与自我类型_, *参与交互者类型_}) {
                const auto 添加 = 存在服务_.添加无当前值特征(*场景.场景节点, 类型);
                if (添加.状态 != 新存在操作状态::已完成 || !添加.特征节点)
                    return {新场景操作状态::结构不一致, {}, {}, {}};
            }
        return 窗口;
    } catch (...) {
        return {新场景操作状态::资源失败, {}, {}, {}};
    }
}

新场景操作状态 新_世界树类::配置现实交互场景特征(
    稳定编码 参与自我类型, 稳定编码 参与交互者类型) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !有效(参与自我类型) || !有效(参与交互者类型) || 参与自我类型 == 参与交互者类型)
            return 新场景操作状态::入口拒绝;
        if (参与自我类型_)
            return *参与自我类型_ == 参与自我类型 && 参与交互者类型_ == std::optional{参与交互者类型}
                ? 新场景操作状态::无变化 : 新场景操作状态::入口拒绝;
        参与自我类型_ = 参与自我类型;
        参与交互者类型_ = 参与交互者类型;
        return 新场景操作状态::已完成;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

std::optional<稳定编码> 新_世界树类::查找顶层特征已加锁(稳定编码 持有者, 稳定编码 类型根) const {
    const auto 存在 = 存在服务_.获取存在(持有者);
    if (!存在) return std::nullopt;
    std::optional<稳定编码> 命中;
    for (const auto 特征节点 : 存在->特征节点组) {
        const auto 特征 = 特征服务_.获取特征(特征节点);
        if (!特征) throw std::runtime_error("现实交互场景特征不可读");
        if (特征概念服务_.比较单特征概念(特征->特征概念节点, 特征->特征概念节点).特征类型根 != std::optional{类型根})
            continue;
        if (命中) throw std::runtime_error("现实交互场景特征类型重复");
        命中 = 特征节点;
    }
    return 命中;
}

新场景成员建立结果 新_世界树类::建立或取得交互者副本(
    稳定编码 现实交互场景, 稳定编码 外设交互者, 稳定编码 外设存在标识类型) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !有效(现实交互场景) || !有效(外设交互者) || !有效(外设存在标识类型)
            || 外设交互者.值 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()))
            return {};
        auto 副本 = 查找标识副本(现实交互场景, 外设存在标识类型, 外设交互者);
        if (副本.状态 == 新场景操作状态::存在不存在) {
            新存在建立请求 请求;
            请求.初始特征组.push_back({外设存在标识类型, static_cast<std::int64_t>(外设交互者.值)});
            副本 = 场景服务_.建立成员存在(现实交互场景, 请求, 读取组合真实时间());
        }
        if ((副本.状态 != 新场景操作状态::已完成 && 副本.状态 != 新场景操作状态::无变化) || !副本.存在节点)
            return 副本;
        // 同一次 owner 调用中写场景的“参与交互者”＝副本（R95），不另起动作。
        if (参与交互者类型_) {
            const auto F = 查找顶层特征已加锁(现实交互场景, *参与交互者类型_);
            if (!F) return {新场景操作状态::结构不一致, {}, {}, {}};
            const auto 特征 = 特征服务_.获取特征(*F);
            if (!特征) return {新场景操作状态::结构不一致, {}, {}, {}};
            if (特征->当前值 != std::optional<新特征准确值>{新特征准确值{*副本.存在节点}}) {
                const auto 更新 = 场景服务_.更新场景内存在特征值(现实交互场景, 现实交互场景, *F,
                    新特征准确值{*副本.存在节点}, 读取组合真实时间());
                if (更新.状态 != 新场景操作状态::已完成 && 更新.状态 != 新场景操作状态::无变化)
                    return {更新.状态, {}, {}, {}};
            }
        }
        return 副本;
    } catch (...) {
        return {新场景操作状态::资源失败, {}, {}, {}};
    }
}

新场景操作状态 新_世界树类::迁移自我(稳定编码 目标场景节点) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !自我节点_) return 新场景操作状态::入口拒绝;
        const auto 状态 = 场景服务_.迁移成员存在(*自我节点_, 目标场景节点);
        if (状态 != 新场景操作状态::已完成) return 状态;
        自我所在场景_ = 目标场景节点;
        // 同一次 owner 调用维护目标场景的“参与自我”（R95）；目标不是现实交互场景时没有该特征。
        if (参与自我类型_) {
            const auto F = 查找顶层特征已加锁(目标场景节点, *参与自我类型_);
            if (F) {
                const auto 特征 = 特征服务_.获取特征(*F);
                if (!特征) return 新场景操作状态::结构不一致;
                if (特征->当前值 != std::optional<新特征准确值>{新特征准确值{*自我节点_}}) {
                    const auto 更新 = 场景服务_.更新场景内存在特征值(目标场景节点, 目标场景节点, *F,
                        新特征准确值{*自我节点_}, 读取组合真实时间());
                    if (更新.状态 != 新场景操作状态::已完成 && 更新.状态 != 新场景操作状态::无变化)
                        return 更新.状态;
                }
            }
        }
        return 状态;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_世界树类::删除空场景(
    稳定编码 场景节点) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.删除空场景(场景节点)
            : 新场景操作状态::入口拒绝;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景特征添加结果 新_世界树类::给存在添加特征(
    稳定编码 场景节点,
    稳定编码 存在节点,
    稳定编码 特征概念节点,
    const 新特征准确值& 初始值,
    const 新状态强时间& 强时间) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.添加场景内存在特征(
            场景节点, 存在节点, 特征概念节点, 初始值, 强时间)
            : 新场景特征添加结果{};
    } catch (...) {
        新场景特征添加结果 结果;
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

新场景特征添加结果 新_世界树类::给场景添加特征(
    稳定编码 场景节点,
    稳定编码 特征概念节点,
    const 新特征准确值& 初始值,
    const 新状态强时间& 强时间) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.添加场景特征(
            场景节点, 特征概念节点, 初始值, 强时间)
            : 新场景特征添加结果{};
    } catch (...) {
        新场景特征添加结果 结果;
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

std::optional<新状态信息> 新_世界树类::查找最近同类型状态(
    稳定编码 场景节点,
    稳定编码 存在节点,
    稳定编码 当前特征概念节点,
    bool& 结构有效) const noexcept {
    结构有效 = false;
    try {
        std::optional<新状态信息> 最近;
        for (const auto 状态节点 : 场景服务_.查询状态列表(场景节点)) {
            const auto 状态 = 状态服务_.获取状态(状态节点);
            if (!状态) return std::nullopt;
            if (状态->被描述存在节点 != 存在节点) continue;
            const auto 概念关系 = 特征概念服务_.比较单特征概念(
                当前特征概念节点, 状态->特征类型概念节点);
            if (概念关系.状态 != 特征概念规则状态::已完成
                || !概念关系.关系) {
                return std::nullopt;
            }
            if (*概念关系.关系 == 单特征概念关系::无可比关系) continue;
            if (状态->强时间.语义 != 新状态时间语义::实例绝对UTC
                || 状态->强时间.纳秒 < 0) {
                return std::nullopt;
            }
            if (!最近 || 状态->强时间.纳秒 > 最近->强时间.纳秒) {
                最近 = *状态;
            } else if (状态->强时间.纳秒 == 最近->强时间.纳秒
                && 状态->节点 != 最近->节点) {
                return std::nullopt;
            }
        }
        结构有效 = true;
        return 最近;
    } catch (...) {
        return std::nullopt;
    }
}

新世界I64组合准备 新_世界树类::准备存在I64增量(
    const 新世界I64组合请求& 请求) noexcept {
    try {
        auto 内容 = std::make_unique<新世界I64组合准备::实现>();
        内容->所有者 = this;
        内容->请求 = 请求;
        auto& 材料 = 内容->材料;
        材料.场景节点 = 请求.场景节点;
        材料.存在节点 = 请求.存在节点;
        材料.特征节点 = 请求.特征节点;
        {
            std::lock_guard 锁(新世界树互斥);
            if (!已初始化_) 世界I64组合故障(L"组合world尚未初始化。");
            auto [特征, 类型] = 读取组合目标(
                场景服务_, 存在服务_, 特征服务_, 世界树根场景_, 请求);
            内容->原特征 = std::move(特征);
            内容->特征类型根 = 类型;
            材料.前值 = std::get<std::int64_t>(*内容->原特征.当前值);
            if ((请求.增量 > 0 && 材料.前值 > (std::numeric_limits<std::int64_t>::max)() - 请求.增量)
                || (请求.增量 < 0 && 材料.前值 < (std::numeric_limits<std::int64_t>::min)() - 请求.增量))
                世界I64组合故障(L"I64增量算术越界，不钳制为上限或零增量。");
            材料.后值 = 材料.前值 + 请求.增量;
        }
        const auto 复核定位 = [&]() {
            const auto [特征, 类型] = 读取组合目标(
                场景服务_, 存在服务_, 特征服务_, 世界树根场景_, 请求);
            if (!已初始化_ || 特征 != 内容->原特征 || 类型 != 内容->特征类型根)
                世界I64组合故障(L"组合准备期间目标定位、当前值或特征类型发生变化。");
        };
        if (请求.增量 != 0) {
            bool 结构有效 = false;
            auto 前状态 = 查找最近同类型状态(
                请求.场景节点, 请求.存在节点, 内容->原特征.特征概念节点, 结构有效);
            if (!结构有效) 世界I64组合故障(L"组合目标最近同类型状态结构不可读。");
            if (!前状态) {
                const auto 取样时间 = 读取组合真实时间();
                const auto 取样 = 存在服务_.建立当前I64特征状态(
                    请求.存在节点, 请求.特征节点, 内容->特征类型根, 材料.前值, 取样时间);
                if (取样.状态 != 新存在操作状态::已完成 || !取样.状态结果
                    || !取样.状态结果->状态节点 || 取样.特征结果 || 取样.专属概念结果)
                    世界I64组合故障(L"缺失前状态的当前准确值取样未完成。");
                {
                    std::lock_guard 锁(新世界树互斥);
                    复核定位();
                    if (!组合场景挂接成功(场景服务_.添加状态(
                        请求.场景节点, *取样.状态结果->状态节点)))
                        世界I64组合故障(L"真实当前前状态挂接失败，保留已发表状态。");
                }
                前状态 = 状态服务_.获取状态(*取样.状态结果->状态节点);
            }
            if (!前状态 || 前状态->被描述存在节点 != 请求.存在节点
                || 前状态->来源实例特征节点 != 请求.特征节点
                || 前状态->固定准确值 != 新特征准确值{材料.前值}
                || 前状态->强时间.语义 != 新状态时间语义::实例绝对UTC
                || 前状态->强时间.纳秒 < 0)
                世界I64组合故障(L"最近前状态不能证明本次准确前值，禁止补造过去状态。");
            const auto 前定义 = 特征服务_.捕获特征匹配定义(前状态->特征类型概念节点);
            if (!前定义 || 前定义->特征类型根 != 内容->特征类型根)
                世界I64组合故障(L"组合前状态与目标特征不属于同一类型根。");
            const auto 动作时间 = 读取组合真实时间();
            if (动作时间.纳秒 < 前状态->强时间.纳秒)
                世界I64组合故障(L"动作真实时间早于前状态，禁止伪造递增时间。");
            {
                std::lock_guard 锁(新世界树互斥);
                复核定位();
            }
            // 动作owner可能回调task方法投影，必须在world/feature锁外执行。
            const auto 动作结果 = 动态服务_.建立动作动态({请求.执行主体节点,
                请求.动作定义节点, 请求.动作来源节点, 请求.存在节点, 请求.特征节点, 动作时间});
            if (动作结果.状态 != 新动态操作状态::已完成 || !动作结果.动态节点)
                世界I64组合故障(L"本次普通动作未新建完成或已有未消费动作，禁止重放。");
            {
                std::lock_guard 锁(新世界树互斥);
                复核定位();
                if (!组合场景挂接成功(场景服务_.添加动态(请求.场景节点, *动作结果.动态节点)))
                    世界I64组合故障(L"普通动作场景挂接失败，禁止删除已发表动作。");
            }
            内容->动作 = 动态服务_.获取动态(*动作结果.动态节点);
            内容->前状态 = 状态服务_.获取状态(前状态->节点);
            if (!内容->动作 || 内容->动作->种类 != 新动态种类::动作
                || 内容->动作->主题存在节点 != 请求.执行主体节点
                || 内容->动作->动作定义节点 != std::optional<稳定编码>{请求.动作定义节点}
                || 内容->动作->动作来源节点 != std::optional<稳定编码>{请求.动作来源节点}
                || 内容->动作->动作目标存在节点 != std::optional<稳定编码>{请求.存在节点}
                || 内容->动作->动作目标特征节点 != std::optional<稳定编码>{请求.特征节点}
                || 内容->动作->动作目标种类 != 新动作目标种类::普通存在
                || 内容->动作->动作目标任务虚拟存在 || !内容->动作->动作结果组.empty()
                || 内容->动作->动作时间 != std::optional<新状态强时间>{动作时间}
                || !内容->前状态 || 内容->前状态->被描述存在节点 != 请求.存在节点
                || 内容->前状态->来源实例特征节点 != 请求.特征节点
                || 内容->前状态->固定准确值 != 新特征准确值{材料.前值}
                || 内容->前状态->强时间 != 前状态->强时间)
                世界I64组合故障(L"组合动作或前状态正式读回不完整。");
            材料.普通动作节点 = 动作结果.动态节点;
            材料.前状态节点 = 前状态->节点;
        }
        材料.核心时间 = 读取组合真实时间();
        if (内容->动作 && 材料.核心时间.纳秒 < 内容->动作->动作时间->纳秒)
            世界I64组合故障(L"组合见证时间早于已发表动作。");
        {
            std::lock_guard 锁(新世界树互斥);
            复核定位();
            // 所有前S/动作/P对F的消费者引用均已建立，随后只交付只读见证。
            材料.特征保护 = 读取组合完整保护(请求.特征节点);
            复核定位();
        }
        return 新世界I64组合准备{std::move(内容)};
    } catch (...) {
        世界I64组合故障(L"世界I64组合准备遇到工程异常，保留已发表现场。");
    }
}

新世界I64组合准备材料 新_世界树类::读取存在I64组合准备(
    const 新世界I64组合准备& 准备) const noexcept {
    try {
        if (!准备.实现_) 世界I64组合故障(L"读取缺少组合准备。");
        const auto& 内容 = *准备.实现_;
        内容.核对线程();
        if (内容.所有者 != this || 内容.已完成)
            世界I64组合故障(L"读取组合准备owner不符或已经消费。");
        return 内容.材料;
    } catch (...) {
        世界I64组合故障(L"读取组合准备出现工程异常。");
    }
}

新世界I64组合结果 新_世界树类::提交存在I64增量并形成动态(
    新世界I64组合准备& 准备, const 基础封存追加片段& 附加片段) noexcept {
    try {
        if (!准备.实现_) 世界I64组合故障(L"提交缺少组合准备。");
        auto& 内容 = *准备.实现_;
        内容.核对线程();
        if (内容.所有者 != this || 内容.已完成)
            世界I64组合故障(L"提交组合准备owner不符或重复消费。");
        const auto& 材料 = 内容.材料;
        const auto 复核定位 = [&]() {
            if (!已初始化_) 世界I64组合故障(L"组合world已失效。");
            const auto [特征, 类型] = 读取组合目标(
                场景服务_, 存在服务_, 特征服务_, 世界树根场景_, 内容.请求);
            if (特征 != 内容.原特征 || 类型 != 内容.特征类型根)
                世界I64组合故障(L"动作准备后组合目标材料发生变化，禁止覆盖或重放。");
        };
        {
            std::lock_guard 锁(新世界树互斥);
            复核定位();
            if (!组合快照相同(读取组合完整保护(材料.特征节点), 材料.特征保护))
                世界I64组合故障(L"封存后特征完整保护发生变化，已发表动作保留。");
        }

        if (内容.请求.增量 == 0) {
            if (材料.前值 != 材料.后值 || 材料.普通动作节点 || 材料.前状态节点
                || 内容.动作 || 内容.前状态)
                世界I64组合故障(L"零增量组合准备包含虚构动作或状态。");
            // 真实非空附加片段自身携带完整F预期；不造只含保护的空world片段。
            const auto 发布 = 全局基础数据集.原子追加封存片段组({附加片段});
            if (发布.状态 != 基础节点组追加状态::已完成 || 发布.片段回执组.size() != 1
                || !发布.片段回执组.front().对应片段(附加片段))
                世界I64组合故障(L"零增量附加片段发布未完整完成，保留现场。");
            内容.已完成 = true;
            return {true, std::nullopt, 发布.片段回执组.front()};
        }

        if (!内容.前状态 || !内容.动作 || !材料.前状态节点 || !材料.普通动作节点)
            世界I64组合故障(L"非零组合缺少本次真实前状态或动作。");
        // 显式动态读取与投影均不持world锁。
        const auto 动作 = 动态服务_.获取动态(*材料.普通动作节点);
        const auto 前状态 = 状态服务_.获取状态(*材料.前状态节点);
        if (!动作 || *动作 != *内容.动作 || !前状态 || *前状态 != *内容.前状态)
            世界I64组合故障(L"提交前普通动作或前状态偏离冻结材料。");
        {
            std::lock_guard 锁(新世界树互斥);
            const auto 场景动作 = 场景服务_.查询动态列表(材料.场景节点);
            const auto 场景状态 = 场景服务_.查询状态列表(材料.场景节点);
            if (std::count(场景动作.begin(), 场景动作.end(), *材料.普通动作节点) != 1
                || std::count(场景状态.begin(), 场景状态.end(), *材料.前状态节点) != 1)
                世界I64组合故障(L"组合动作或前状态的场景组织引用丢失。");
            复核定位();
        }

        auto 特征准备 = 特征服务_.准备顶层I64更新(材料.特征节点, 材料.后值);
        if (特征准备.状态 != 新特征I64准备状态::已准备 || !特征准备.准备
            || 特征准备.准备->特征节点() != 材料.特征节点
            || 特征准备.准备->原当前值() != std::optional<std::int64_t>{材料.前值}
            || 特征准备.准备->新当前值() != 材料.后值 || !特征准备.准备->核心有变化())
            世界I64组合故障(L"联合核心准备未取得本次准确前后值，已发表动作保留。");
        const auto 提交 = 特征服务_.提交顶层I64更新(*特征准备.准备, {附加片段});
        if (提交.状态 != 基础节点组追加状态::已完成 || 提交.附加片段回执组.size() != 1
            || !提交.附加片段回执组.front().对应片段(附加片段))
            世界I64组合故障(L"F与附加片段联合提交未完整完成，不重放动作。");
        const auto 特征结果 = 特征服务_.完成顶层I64更新(*特征准备.准备);
        if (特征结果.状态 != 新特征写入状态::已完成 || !特征结果.当前值已改变)
            世界I64组合故障(L"F核心已发表但聚合收尾不完整，保留已发表事实。");

        // 这里只重取真实时间。逻辑占用继续阻止F写/删，不持F互斥等待时钟。
        auto 后时间 = 读取组合真实时间();
        if (后时间.纳秒 < 材料.核心时间.纳秒)
            世界I64组合故障(L"真实UTC时钟倒退，无法形成动作后的真实状态。");
        while (后时间.纳秒 == 内容.动作->动作时间->纳秒) {
            std::this_thread::yield();
            后时间 = 读取组合真实时间();
            if (后时间.纳秒 < 材料.核心时间.纳秒)
                世界I64组合故障(L"等待真实后状态时间期间UTC倒退。");
        }
        const auto 存在结果 = 存在服务_.完成已提交I64特征变化(
            材料.存在节点, 材料.特征节点, 特征结果, 内容.特征类型根, 材料.后值, 后时间);
        if (存在结果.状态 != 新存在操作状态::已完成 || !存在结果.状态结果
            || !存在结果.状态结果->状态节点)
            世界I64组合故障(L"核心已发表但EC/后状态派生未完整完成，不重写F。");
        const auto 后状态节点 = *存在结果.状态结果->状态节点;
        const auto 后状态 = 状态服务_.获取状态(后状态节点);
        if (!后状态 || 后状态->被描述存在节点 != 材料.存在节点
            || 后状态->来源实例特征节点 != 材料.特征节点
            || 后状态->固定准确值 != 新特征准确值{材料.后值}
            || 后状态->强时间 != 后时间)
            世界I64组合故障(L"已发表后状态读回与本次组合结果不符。");
        const auto 后定义 = 特征服务_.捕获特征匹配定义(后状态->特征类型概念节点);
        if (!后定义 || 后定义->特征类型根 != 内容.特征类型根)
            世界I64组合故障(L"组合前后状态特征类型根不一致。");
        {
            std::lock_guard 锁(新世界树互斥);
            const auto [当前特征, 当前类型] = 读取组合目标(
                场景服务_, 存在服务_, 特征服务_, 世界树根场景_, 内容.请求);
            if (当前类型 != 内容.特征类型根
                || 当前特征.当前值 != std::optional<新特征准确值>{新特征准确值{材料.后值}}
                || !组合场景挂接成功(场景服务_.添加状态(材料.场景节点, 后状态节点)))
                世界I64组合故障(L"组合后状态场景挂接失败，保留已发表事实。");
        }

        新世界动态发布结果 动态发布;
        动态发布.动态建立结果 = 动态服务_.建立原子动态(
            材料.存在节点, *材料.前状态节点, 后状态节点);
        if (动态发布.动态建立结果->状态 != 新动态操作状态::已完成
            || !动态发布.动态建立结果->动态节点)
            世界I64组合故障(L"组合前后状态的真实变化动态建立未完成。");
        const auto 变化动态节点 = *动态发布.动态建立结果->动态节点;
        {
            std::lock_guard 锁(新世界树互斥);
            动态发布.场景挂接结果 = 场景服务_.添加动态(材料.场景节点, 变化动态节点);
            if (!组合场景挂接成功(*动态发布.场景挂接结果))
                世界I64组合故障(L"真实变化动态挂接失败，禁止删除已发表D。");
        }
        动态发布.概念处理结果 = 动态概念抽象服务_.处理新动态(变化动态节点);
        if (!动态概念处理成功(动态发布.概念处理结果->状态))
            世界I64组合故障(L"真实变化动态概念分类或抽象未完成。");
        switch (动态发布.概念处理结果->状态) {
        case 新动态概念自动抽象状态::已采用已有概念:
            动态发布.状态 = 新世界动态发布状态::已完成并采用已有概念; break;
        case 新动态概念自动抽象状态::已形成并采用上位概念:
            动态发布.状态 = 新世界动态发布状态::已完成并形成上位概念; break;
        case 新动态概念自动抽象状态::已是聚合概念:
            动态发布.状态 = 新世界动态发布状态::已完成且已有聚合概念; break;
        default:
            动态发布.状态 = 新世界动态发布状态::已完成并保留实例概念; break;
        }
        const auto 关联 = 动态服务_.关联动作结果(
            *材料.普通动作节点, 变化动态节点, 新动作结果关系类型::导致变化);
        if (关联 != 新动态操作状态::已完成 && 关联 != 新动态操作状态::无变化)
            世界I64组合故障(L"真实变化动态未能关联本次普通动作。");
        const auto 最终动态 = 动态服务_.获取动态(变化动态节点);
        const auto 最终动作 = 动态服务_.获取动态(*材料.普通动作节点);
        if (!最终动态 || 最终动态->种类 != 新动态种类::原子
            || 最终动态->主题存在节点 != 材料.存在节点
            || 最终动态->状态节点组 != std::vector<稳定编码>{*材料.前状态节点, 后状态节点}
            || !最终动作 || 最终动作->动作结果组 != std::vector<新动作结果关联>{
                {新动作结果关系类型::导致变化, 变化动态节点}})
            世界I64组合故障(L"动作和变化动态完成引用未精确读回。");
        {
            std::lock_guard 锁(新世界树互斥);
            const auto 动态组 = 场景服务_.查询动态列表(材料.场景节点);
            const auto 状态组 = 场景服务_.查询状态列表(材料.场景节点);
            if (std::count(动态组.begin(), 动态组.end(), *材料.普通动作节点) != 1
                || std::count(动态组.begin(), 动态组.end(), 变化动态节点) != 1
                || std::count(状态组.begin(), 状态组.end(), *材料.前状态节点) != 1
                || std::count(状态组.begin(), 状态组.end(), 后状态节点) != 1)
                世界I64组合故障(L"组合结果的场景组织未完整读回。");
        }
        新世界特征变化结果 变化;
        变化.状态 = 新世界特征变化状态::已完成;
        变化.前状态节点 = 材料.前状态节点;
        变化.新状态节点 = 后状态节点;
        变化.特征更新结果 = 新场景特征值更新结果{
            新场景操作状态::已完成, 存在结果, 后状态节点};
        变化.动态发布结果 = std::move(动态发布);
        变化.动作关联结果 = 关联;
        特征服务_.结束顶层I64更新(*特征准备.准备);
        内容.已完成 = true;
        return {false, std::move(变化), 提交.附加片段回执组.front()};
    } catch (...) {
        世界I64组合故障(L"组合提交或派生遇到工程异常，保留动作和全部已发表事实。");
    }
}

新动态概念自动抽象结果 新_世界树类::处理已登记动态概念(
    const 新_场景类& 场景视图, 稳定编码 场景节点, 稳定编码 动态节点) noexcept {
    新动态概念自动抽象结果 结果;
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !有效(动态节点) || !场景视图.是场景节点(场景节点)) return 结果;
        const auto 场景 = 场景视图.获取场景(场景节点);
        const auto 动态 = 动态服务_.获取动态(动态节点);
        结果.状态 = 新动态概念自动抽象状态::结构不一致;
        if (!场景 || !动态 || 动态->种类 != 新动态种类::原子 || 动态->状态节点组.size() != 2
            || std::count(场景->动态节点组.begin(), 场景->动态节点组.end(), 动态节点) != 1
            || std::count(场景->直接成员存在组.begin(), 场景->直接成员存在组.end(), 动态->主题存在节点) != 1)
            return 结果;
        const auto 前 = 状态服务_.获取状态(动态->状态节点组[0]);
        const auto 后 = 状态服务_.获取状态(动态->状态节点组[1]);
        const auto 存在 = 存在服务_.获取存在(动态->主题存在节点);
        if (!前 || !后 || !存在 || 前->被描述存在节点 != 动态->主题存在节点
            || 后->被描述存在节点 != 动态->主题存在节点 || 前->来源实例特征节点 != 后->来源实例特征节点
            || !有效(前->来源实例特征节点)
            || std::count(存在->特征节点组.begin(), 存在->特征节点组.end(), 前->来源实例特征节点) != 1
            || std::count(场景->状态节点组.begin(), 场景->状态节点组.end(), 前->节点) != 1
            || std::count(场景->状态节点组.begin(), 场景->状态节点组.end(), 后->节点) != 1
            || 前->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 前->强时间.纳秒 < 0 || 后->强时间.纳秒 <= 前->强时间.纳秒) return 结果;
        const auto 特征 = 特征服务_.获取特征(前->来源实例特征节点);
        if (!特征) return 结果;
        const auto 前类型 = 特征概念服务_.比较单特征概念(前->特征类型概念节点, 前->特征类型概念节点);
        const auto 后类型 = 特征概念服务_.比较单特征概念(后->特征类型概念节点, 后->特征类型概念节点);
        const auto 特征类型 = 特征概念服务_.比较单特征概念(特征->特征概念节点, 特征->特征概念节点);
        if (前类型.状态 != 特征概念规则状态::已完成 || !前类型.特征类型根
            || 后类型.状态 != 特征概念规则状态::已完成 || 后类型.特征类型根 != 前类型.特征类型根
            || 特征类型.状态 != 特征概念规则状态::已完成 || 特征类型.特征类型根 != 前类型.特征类型根)
            return 结果;
        return 动态概念抽象服务_.处理新动态(动态节点);
    } catch (...) {
        结果.状态 = 新动态概念自动抽象状态::资源失败;
        return 结果;
    }
}

新世界特征变化结果 新_世界树类::更新存在特征并形成动态(
    稳定编码 场景节点,
    稳定编码 存在节点,
    稳定编码 特征节点,
    const 新特征准确值& 新值,
    const 新状态强时间& 强时间,
    std::optional<稳定编码> 动作动态节点) noexcept {
    新世界特征变化结果 结果;
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_) {
            结果.状态 = 新世界特征变化状态::世界树未初始化;
            return 结果;
        }
        if (强时间.语义 != 新状态时间语义::实例绝对UTC
            || 强时间.纳秒 < 0) {
            结果.状态 = 新世界特征变化状态::时间不合法;
            return 结果;
        }
        if (!场景服务_.是场景节点(场景节点)) {
            结果.状态 = 新世界特征变化状态::场景不存在;
            return 结果;
        }
        const bool 是场景自身 = 存在节点 == 场景节点;
        const auto 所属场景 = 是场景自身
            ? std::optional<稳定编码>{场景节点}
            : 场景服务_.查询所属场景(存在节点);
        if (!所属场景 || *所属场景 != 场景节点) {
            结果.状态 = 新世界特征变化状态::存在不属于场景;
            return 结果;
        }
        const auto 存在 = 存在服务_.获取存在(存在节点);
        const auto 特征 = 特征服务_.获取特征(特征节点);
        if (!存在 || !特征
            || std::find(存在->特征节点组.begin(),
                存在->特征节点组.end(), 特征节点)
                == 存在->特征节点组.end()) {
            结果.状态 = 新世界特征变化状态::特征不属于存在;
            return 结果;
        }

        std::optional<新动态信息> 动作信息;
        if (动作动态节点) {
            const auto 场景动态 = 场景服务_.查询动态列表(场景节点);
            动作信息 = 动态服务_.获取动态(*动作动态节点);
            if (!动作信息 || 动作信息->种类 != 新动态种类::动作
                || !动作信息->动作目标存在节点
                || !动作信息->动作时间
                || *动作信息->动作目标存在节点 != 存在节点
                || 动作信息->动作目标特征节点
                    != std::optional<稳定编码>{特征节点}
                || 动作信息->动作时间->语义
                    != 新状态时间语义::实例绝对UTC
                || 动作信息->动作时间->纳秒 > 强时间.纳秒
                || std::find(场景动态.begin(), 场景动态.end(),
                    *动作动态节点) == 场景动态.end()) {
                结果.状态 = 新世界特征变化状态::动作不匹配;
                return 结果;
            }
        }

        bool 状态结构有效 = false;
        const auto 前状态 = 查找最近同类型状态(
            场景节点, 存在节点, 特征->特征概念节点, 状态结构有效);
        if (!状态结构有效) {
            结果.状态 = 新世界特征变化状态::结构不一致;
            return 结果;
        }
        if (前状态 && 强时间.纳秒 <= 前状态->强时间.纳秒) {
            结果.状态 = 新世界特征变化状态::时间非递增;
            结果.前状态节点 = 前状态->节点;
            return 结果;
        }
        if (前状态 && 动作信息
            && 动作信息->动作时间->纳秒 < 前状态->强时间.纳秒) {
            结果.状态 = 新世界特征变化状态::动作不匹配;
            结果.前状态节点 = 前状态->节点;
            return 结果;
        }

        结果.特征更新结果 = 场景服务_.更新场景内存在特征值(
            场景节点, 存在节点, 特征节点, 新值, 强时间);
        if (结果.特征更新结果->状态 == 新场景操作状态::存在前提已变化) {
            结果.状态 = 新世界特征变化状态::前提已变化;
            return 结果;
        }
        if (结果.特征更新结果->状态 == 新场景操作状态::无变化) {
            结果.状态 = 新世界特征变化状态::无变化;
            if (前状态) 结果.前状态节点 = 前状态->节点;
            return 结果;
        }
        if (结果.特征更新结果->状态 != 新场景操作状态::已完成
            || !结果.特征更新结果->状态节点) {
            结果.状态 = 新世界特征变化状态::特征更新未完成;
            if (前状态) 结果.前状态节点 = 前状态->节点;
            if (结果.特征更新结果->状态节点) {
                结果.新状态节点 = 结果.特征更新结果->状态节点;
            }
            return 结果;
        }
        结果.新状态节点 = 结果.特征更新结果->状态节点;
        const auto 新状态 = 状态服务_.获取状态(*结果.新状态节点);
        if (!新状态) {
            结果.状态 = 新世界特征变化状态::结构不一致;
            return 结果;
        }
        if (!前状态) {
            结果.状态 = 新世界特征变化状态::已生成状态但无可连接前状态;
            return 结果;
        }
        const auto 前定位 = 特征概念服务_.比较单特征概念(
            前状态->特征类型概念节点, 前状态->特征类型概念节点);
        const auto 后定位 = 特征概念服务_.比较单特征概念(
            新状态->特征类型概念节点, 新状态->特征类型概念节点);
        if (前定位.状态 != 特征概念规则状态::已完成 || !前定位.特征类型根
            || 后定位.状态 != 特征概念规则状态::已完成 || !后定位.特征类型根
            || !特征概念服务_.获取特征概念(*前定位.特征类型根)
            || !特征概念服务_.获取特征概念(*后定位.特征类型根)) {
            结果.状态 = 新世界特征变化状态::结构不一致;
            return 结果;
        }
        if (前定位.特征类型根 != 后定位.特征类型根) {
            结果.状态 = 新世界特征变化状态::已生成状态但无可连接前状态;
            return 结果;
        }
        结果.前状态节点 = 前状态->节点;
        结果.动态发布结果 = 完成动态发布(
            场景节点,
            动态服务_.建立原子动态(
                存在节点, 前状态->节点, 新状态->节点));
        if (!结果.动态发布结果->成功()) {
            结果.状态 = 新世界特征变化状态::动态发布未完成;
            return 结果;
        }
        if (动作动态节点) {
            const auto 结果动态 = 结果.动态发布结果->动态建立结果
                ? 结果.动态发布结果->动态建立结果->动态节点
                : std::nullopt;
            if (!结果动态) {
                结果.状态 = 新世界特征变化状态::结构不一致;
                return 结果;
            }
            结果.动作关联结果 = 关联动作结果(
                场景节点, *动作动态节点, *结果动态,
                新动作结果关系类型::导致变化);
            if (*结果.动作关联结果 != 新动态操作状态::已完成
                && *结果.动作关联结果 != 新动态操作状态::无变化) {
                结果.状态 = 新世界特征变化状态::已改变但动作关联未完成;
                return 结果;
            }
        }
        结果.状态 = 新世界特征变化状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 新世界特征变化状态::资源失败;
        return 结果;
    }
}

新世界特征变化结果 新_世界树类::更新存在引用并形成动态(
    稳定编码 场景, 稳定编码 主体, 稳定编码 特征,
    稳定编码 新目标, 稳定编码 既有动作) noexcept {
    try {
        新世界特征变化结果 结果;
        const auto 前提变化 = [&]() {
            结果.状态 = 新世界特征变化状态::前提已变化;
            return 结果;
        };
        if (!已初始化_) { 结果.状态 = 新世界特征变化状态::世界树未初始化; return 结果; }
        // 在读取语义之前取得定位保护，后续同CAS验证；不引入世界总锁。
        std::vector<稳定编码> 定位节点{场景, 主体, 特征, 既有动作};
        std::sort(定位节点.begin(), 定位节点.end());
        定位节点.erase(std::unique(定位节点.begin(), 定位节点.end()), 定位节点.end());
        基础节点组追加请求 保护请求;
        for (const auto 节点 : 定位节点)
            保护请求.预期联合节点组.push_back(读取组合完整保护(节点));
        if (!场景服务_.是场景节点(场景)
            || !场景服务_.存在属于场景范围(世界树根场景_, 场景))
            世界I64组合故障(L"单引用更新场景不属于本世界");
        if (主体 != 场景 && 场景服务_.查询所属场景(主体) != std::optional<稳定编码>{场景})
            return 前提变化();
        const auto 存在信息 = 存在服务_.获取存在(主体);
        const auto F = 特征服务_.获取特征(特征);
        const auto 动作 = 动态服务_.获取动态(既有动作);
        if (!存在信息 || !F || F->上级特征节点
            || std::find(存在信息->特征节点组.begin(), 存在信息->特征节点组.end(), 特征)
                == 存在信息->特征节点组.end()) return 前提变化();
        const auto 定义 = 特征服务_.捕获特征匹配定义(F->特征概念节点);
        if (!定义 || 定义->材料类型 != 特征概念材料物理类型::存在节点引用
            || (F->当前值 && !std::holds_alternative<稳定编码>(*F->当前值)))
            世界I64组合故障(L"单引用更新目标不是存在引用特征");
        const auto 场景动作 = 场景服务_.查询动态列表(场景);
        if (!动作 || 动作->种类 != 新动态种类::动作 || 动作->主题存在节点 != 主体
            || 动作->动作目标存在节点 != std::optional<稳定编码>{主体}
            || 动作->动作目标特征节点 != std::optional<稳定编码>{特征}
            || !动作->动作定义节点 || !动作->动作来源节点 || !动作->动作时间
            || 动作->动作时间->语义 != 新状态时间语义::实例绝对UTC
            || std::count(场景动作.begin(), 场景动作.end(), 既有动作) != 1)
            世界I64组合故障(L"单引用更新动作身份或场景不匹配");
        for (const auto 节点 : {*动作->动作定义节点, *动作->动作来源节点}) {
            if (std::find(定位节点.begin(), 定位节点.end(), 节点) != 定位节点.end()) continue;
            定位节点.push_back(节点);
            保护请求.预期联合节点组.push_back(读取组合完整保护(节点));
        }
        bool 状态结构有效 = false;
        const auto 前状态 = 查找最近同类型状态(场景, 主体, 定义->特征类型根, 状态结构有效);
        if (!状态结构有效) return 前提变化();
        if (F->当前值) {
            if (!前状态 || 前状态->来源实例特征节点 != 特征
                || 前状态->固定准确值 != F->当前值
                || 动作->动作时间->纳秒 <= 前状态->强时间.纳秒) return 前提变化();
            结果.前状态节点 = 前状态->节点;
            保护请求.预期联合节点组.push_back(读取组合完整保护(前状态->节点));
        } else if (前状态) return 前提变化();
        if (特征服务_.获取特征(特征) != F || 存在服务_.获取存在(主体) != 存在信息
            || 动态服务_.获取动态(既有动作) != 动作
            || 特征服务_.捕获特征匹配定义(F->特征概念节点) != 定义) return 前提变化();
        const auto 封存 = 全局基础数据集.封存原子追加片段(保护请求);
        if (封存.状态 != 基础节点组追加状态::已完成 || !封存.片段) {
            const std::wstring 说明 = L"单引用更新定位保护无法封存 状态=" + std::to_wstring(static_cast<int>(封存.状态))
                + L" 保护数=" + std::to_wstring(保护请求.预期联合节点组.size());
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"世界I64组合更新", 说明.c_str());
        }
        auto 准备 = 特征服务_.准备顶层存在引用更新(特征, 新目标);
        if (准备.状态 == 新特征I64准备状态::预期已变化
            || 准备.状态 == 新特征I64准备状态::联合更新占用) return 前提变化();
        if (准备.状态 != 新特征I64准备状态::已准备 || !准备.准备)
            世界I64组合故障(L"单引用更新准备失败");
        const auto 提交 = 特征服务_.提交顶层存在引用更新(*准备.准备, {*封存.片段});
        if (提交.状态 == 基础节点组追加状态::预期不符) {
            特征服务_.取消顶层存在引用更新(*准备.准备);
            return 前提变化();
        }
        if (提交.状态 != 基础节点组追加状态::已完成 || 提交.附加片段回执组.size() != 1
            || !提交.附加片段回执组.front().对应片段(*封存.片段))
            世界I64组合故障(L"单引用更新与定位保护联合提交失败");
        const auto F结果 = 特征服务_.完成顶层存在引用更新(*准备.准备);
        if (F结果.状态 != 新特征写入状态::已完成 && F结果.状态 != 新特征写入状态::无变化)
            世界I64组合故障(L"单引用更新历史聚合未完成");
        if (!F结果.当前值已改变) {
            特征服务_.结束顶层存在引用更新(*准备.准备);
            结果.状态 = 新世界特征变化状态::无变化;
            return 结果;
        }
        auto 后时间 = 读取组合真实时间();
        const auto 截止 = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        while (后时间.纳秒 <= 动作->动作时间->纳秒
            || (前状态 && 后时间.纳秒 <= 前状态->强时间.纳秒)) {
            if (后时间.纳秒 < 动作->动作时间->纳秒
                || std::chrono::steady_clock::now() >= 截止)
                世界I64组合故障(L"单引用更新后状态UTC未正常推进");
            std::this_thread::yield();
            后时间 = 读取组合真实时间();
        }
        const auto E结果 = 存在服务_.完成已提交存在引用特征变化(
            主体, 特征, F结果, 定义->特征类型根, 新目标, 后时间);
        if (E结果.状态 != 新存在操作状态::已完成 || !E结果.状态结果 || !E结果.状态结果->状态节点)
            世界I64组合故障(L"单引用已发表但存在概念或状态派生失败");
        const auto 后S = *E结果.状态结果->状态节点;
        if (!组合场景挂接成功(场景服务_.添加状态(场景, 后S)))
            世界I64组合故障(L"单引用后状态挂接失败");
        结果.新状态节点 = 后S;
        结果.特征更新结果 = 新场景特征值更新结果{新场景操作状态::已完成, E结果, 后S};
        if (前状态) {
            结果.动态发布结果 = 完成动态发布(场景,
                动态服务_.建立原子动态(主体, 前状态->节点, 后S));
            if (!结果.动态发布结果->成功() || !结果.动态发布结果->动态建立结果
                || !结果.动态发布结果->动态建立结果->动态节点)
                世界I64组合故障(L"单引用真实变化动态派生失败");
            const auto D = *结果.动态发布结果->动态建立结果->动态节点;
            结果.动作关联结果 = 关联动作结果(场景, 既有动作, D, 新动作结果关系类型::导致变化);
            if (*结果.动作关联结果 != 新动态操作状态::已完成
                && *结果.动作关联结果 != 新动态操作状态::无变化)
                世界I64组合故障(L"单引用动作结果关联失败");
            const auto 后动作 = 动态服务_.获取动态(既有动作);
            const auto 后动态 = 动态服务_.获取动态(D);
            if (!后动作 || std::count(后动作->动作结果组.begin(), 后动作->动作结果组.end(),
                    新动作结果关联{新动作结果关系类型::导致变化, D}) != 1
                || !后动态 || 后动态->主题存在节点 != 主体
                || 后动态->状态节点组 != std::vector<稳定编码>{前状态->节点, 后S})
                世界I64组合故障(L"单引用动作和变化动态正式读回失败");
        }
        const auto 后状态 = 状态服务_.获取状态(后S);
        if (!后状态 || 后状态->被描述存在节点 != 主体
            || 后状态->来源实例特征节点 != 特征
            || 后状态->固定准确值 != 新特征准确值{新目标})
            世界I64组合故障(L"单引用后状态正式读回失败");
        const auto 场景状态 = 场景服务_.查询状态列表(场景);
        if (std::count(场景状态.begin(), 场景状态.end(), 后S) != 1)
            世界I64组合故障(L"单引用后状态场景组织读回失败");
        特征服务_.结束顶层存在引用更新(*准备.准备);
        结果.状态 = 前状态 ? 新世界特征变化状态::已完成
            : 新世界特征变化状态::已生成状态但无可连接前状态;
        return 结果;
    } catch (...) { 世界I64组合故障(L"单引用更新或事实派生遇到工程异常"); }
}

新世界特征变化结果 新_世界树类::更新场景特征并形成动态(
    稳定编码 场景节点,
    稳定编码 特征节点,
    const 新特征准确值& 新值,
    const 新状态强时间& 强时间,
    std::optional<稳定编码> 动作动态节点) noexcept {
    return 更新存在特征并形成动态(
        场景节点, 场景节点, 特征节点, 新值, 强时间, 动作动态节点);
}

新世界动作发布结果 新_世界树类::建立动作动态(
    稳定编码 场景节点,
    const 新动作动态建立请求& 请求) noexcept {
    新世界动作发布结果 结果;
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_) {
            结果.状态 = 新世界动作发布状态::世界树未初始化;
            return 结果;
        }
        if (!场景服务_.是场景节点(场景节点)) return 结果;
        结果.动作建立结果 = 动态服务_.建立动作动态(请求);
        const bool 新建 = 结果.动作建立结果->状态
            == 新动态操作状态::已完成;
        const bool 重放 = 结果.动作建立结果->状态
            == 新动态操作状态::无变化;
        if ((!新建 && !重放) || !结果.动作建立结果->动态节点) {
            结果.状态 = 新世界动作发布状态::动作建立失败;
            return 结果;
        }
        结果.场景挂接结果 = 场景服务_.添加动态(
            场景节点, *结果.动作建立结果->动态节点);
        if (*结果.场景挂接结果 != 新场景操作状态::已完成
            && *结果.场景挂接结果 != 新场景操作状态::无变化) {
            if (新建) {
                结果.失败回滚结果 = 动态服务_.删除动态(
                    *结果.动作建立结果->动态节点);
            }
            结果.状态 = !新建 || (结果.失败回滚结果
                    && *结果.失败回滚结果 == 新动态操作状态::已完成)
                ? 新世界动作发布状态::动作挂接失败
                : 新世界动作发布状态::动作挂接失败且回滚失败;
            return 结果;
        }
        结果.状态 = 重放
            ? 新世界动作发布状态::精确重复
            : 新世界动作发布状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 新世界动作发布状态::资源失败;
        return 结果;
    }
}

新动态操作状态 新_世界树类::关联动作结果(
    稳定编码 场景节点,
    稳定编码 动作动态节点,
    稳定编码 结果动态节点,
    新动作结果关系类型 关系类型) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_ || !场景服务_.是场景节点(场景节点)) {
            return 新动态操作状态::入口拒绝;
        }
        const auto 场景动态 = 场景服务_.查询动态列表(场景节点);
        if (std::find(场景动态.begin(), 场景动态.end(), 动作动态节点)
                == 场景动态.end()
            || std::find(场景动态.begin(), 场景动态.end(), 结果动态节点)
                == 场景动态.end()) {
            return 新动态操作状态::动作结果不匹配;
        }
        return 动态服务_.关联动作结果(
            动作动态节点, 结果动态节点, 关系类型);
    } catch (...) {
        return 新动态操作状态::资源失败;
    }
}

新世界动态发布结果 新_世界树类::完成动态发布(
    稳定编码 场景节点,
    新动态建立结果 建立结果) noexcept {
    新世界动态发布结果 结果;
    结果.动态建立结果 = 建立结果;
    if (建立结果.状态 != 新动态操作状态::已完成
        || !建立结果.动态节点) {
        结果.状态 = 新世界动态发布状态::动态建立失败;
        return 结果;
    }
    结果.场景挂接结果 = 场景服务_.添加动态(
        场景节点, *建立结果.动态节点);
    if (*结果.场景挂接结果 != 新场景操作状态::已完成
        && *结果.场景挂接结果 != 新场景操作状态::无变化) {
        结果.失败回滚结果 = 动态服务_.删除动态(*建立结果.动态节点);
        结果.状态 = *结果.失败回滚结果 == 新动态操作状态::已完成
            ? 新世界动态发布状态::动态挂接失败
            : 新世界动态发布状态::动态挂接失败且回滚失败;
        return 结果;
    }

    结果.概念处理结果 = 动态概念抽象服务_.处理新动态(
        *建立结果.动态节点);
    if (!动态概念处理成功(结果.概念处理结果->状态)) {
        结果.状态 = 结果.概念处理结果->状态
                == 新动态概念自动抽象状态::最具体概念冲突
            ? 新世界动态发布状态::已登记但概念识别冲突
            : 新世界动态发布状态::动态已登记但概念处理未完成;
        return 结果;
    }
    switch (结果.概念处理结果->状态) {
    case 新动态概念自动抽象状态::已采用已有概念:
        结果.状态 = 新世界动态发布状态::已完成并采用已有概念;
        break;
    case 新动态概念自动抽象状态::已形成并采用上位概念:
        结果.状态 = 新世界动态发布状态::已完成并形成上位概念;
        break;
    case 新动态概念自动抽象状态::已是聚合概念:
        结果.状态 = 新世界动态发布状态::已完成且已有聚合概念;
        break;
    case 新动态概念自动抽象状态::无同结构候选:
        结果.状态 = 新世界动态发布状态::已完成并保留实例概念;
        break;
    default:
        结果.状态 = 新世界动态发布状态::动态已登记但概念处理未完成;
        break;
    }
    return 结果;
}

新世界动态发布结果 新_世界树类::建立原子动态(
    稳定编码 场景节点,
    稳定编码 主题存在节点,
    稳定编码 前状态节点,
    稳定编码 后状态节点) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_) {
            新世界动态发布结果 结果;
            结果.状态 = 新世界动态发布状态::世界树未初始化;
            return 结果;
        }
        return 完成动态发布(场景节点,
            动态服务_.建立原子动态(
                主题存在节点, 前状态节点, 后状态节点));
    } catch (...) {
        新世界动态发布结果 结果;
        结果.状态 = 新世界动态发布状态::资源失败;
        return 结果;
    }
}

新世界动态发布结果 新_世界树类::建立复合动态(
    稳定编码 场景节点,
    稳定编码 主题存在节点,
    const std::vector<稳定编码>& 成员动态组) noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        if (!已初始化_) {
            新世界动态发布结果 结果;
            结果.状态 = 新世界动态发布状态::世界树未初始化;
            return 结果;
        }
        return 完成动态发布(场景节点,
            动态服务_.建立复合动态(主题存在节点, 成员动态组));
    } catch (...) {
        新世界动态发布结果 结果;
        结果.状态 = 新世界动态发布状态::资源失败;
        return 结果;
    }
}

std::optional<新场景信息> 新_世界树类::查询场景(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.获取场景(场景节点) : std::nullopt;
    } catch (...) { return std::nullopt; }
}

std::optional<新存在信息> 新_世界树类::查询存在(
    稳定编码 存在节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ && !场景服务_.属于内部治理组织(存在节点)
            ? 存在服务_.获取存在(存在节点) : std::nullopt;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"世界查询存在", L"实例世界归属读取异常。"); }
}

std::optional<新特征信息> 新_世界树类::查询特征(
    稳定编码 特征节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ && !场景服务_.属于内部治理组织(特征节点)
            ? 特征服务_.获取特征(特征节点) : std::nullopt;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"世界查询特征", L"实例世界归属读取异常。"); }
}

std::optional<新状态信息> 新_世界树类::查询状态(
    稳定编码 状态节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ && !场景服务_.属于内部治理组织(状态节点)
            ? 状态服务_.获取状态(状态节点) : std::nullopt;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"世界查询状态", L"实例世界归属读取异常。"); }
}

std::optional<新动态信息> 新_世界树类::查询动态(
    稳定编码 动态节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ && !场景服务_.属于内部治理组织(动态节点)
            ? 动态服务_.获取动态(动态节点) : std::nullopt;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"世界查询动态", L"实例世界归属读取异常。"); }
}

std::optional<动态概念信息> 新_世界树类::查询动态概念(
    稳定编码 动态概念节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_
            ? 动态概念服务_.获取动态概念(动态概念节点) : std::nullopt;
    } catch (...) { return std::nullopt; }
}

std::vector<稳定编码> 新_世界树类::查询直接子场景(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.查询直接子场景(场景节点)
            : std::vector<稳定编码>{};
    } catch (...) { return {}; }
}

std::vector<稳定编码> 新_世界树类::查询直接成员存在(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.查询直接成员存在(场景节点)
            : std::vector<稳定编码>{};
    } catch (...) { return {}; }
}

std::vector<稳定编码> 新_世界树类::查询场景状态(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.查询状态列表(场景节点)
            : std::vector<稳定编码>{};
    } catch (...) { return {}; }
}

std::vector<稳定编码> 新_世界树类::查询场景动态(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新世界树互斥);
        return 已初始化_ ? 场景服务_.查询动态列表(场景节点)
            : std::vector<稳定编码>{};
    } catch (...) { return {}; }
}

} // namespace 海中鱼巣
