#include "服务对象需求.发布服务.h"
#include "../领域/信息内容.数据.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <map>
#include <new>
#include <set>
#include <source_location>
#include <sstream>
#include <thread>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {
std::mutex 需求发生来源占用互斥;
std::set<稳定编码> 活动需求发生来源;
struct 需求发生来源占用 final {
    稳定编码 来源;
    bool 已取得=false;
    explicit 需求发生来源占用(稳定编码 n):来源(n) {
        std::lock_guard 锁(需求发生来源占用互斥);
        已取得=活动需求发生来源.insert(n).second;
    }
    需求发生来源占用(const 需求发生来源占用&)=delete;
    需求发生来源占用& operator=(const 需求发生来源占用&)=delete;
    需求发生来源占用(需求发生来源占用&&)=delete;
    需求发生来源占用& operator=(需求发生来源占用&&)=delete;
    ~需求发生来源占用() {
        if(已取得) {std::lock_guard 锁(需求发生来源占用互斥);活动需求发生来源.erase(来源);}
    }
};
[[noreturn]] void 发布故障(const wchar_t* 原因) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象需求特征发布", 原因);
}
bool 需求内容快照相同(const 基础联合节点快照& a,const 基础联合节点快照& b) {
    return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组&&a.入向字段组==b.入向字段组
        &&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;
}
bool 合并需求内容保护(std::vector<基础联合节点快照>& 保护,const std::vector<基础联合节点快照>& 新增) {
    for(const auto& 项:新增) {
        const auto 原=std::find_if(保护.begin(),保护.end(),[&](const auto& 值){return 值.结构.节点.编码==项.结构.节点.编码;});
        if(原==保护.end())保护.push_back(项);
        else if(!需求内容快照相同(*原,项))return false;
    }
    return true;
}
bool 需求内容联合仍相同(const std::vector<基础联合节点快照>& 保护) {
    std::map<稳定编码,const 基础联合节点快照*> 原组;
    for(const auto& 项:保护) {
        const auto [位置,新增]=原组.emplace(项.结构.节点.编码,&项);
        if(!新增&&!需求内容快照相同(*位置->second,项))return false;
    }
    std::vector<稳定编码> 节点组;
    for(const auto& [节点,快照]:原组)节点组.push_back(节点);
    const auto 当前=全局基础数据集.查询节点组完整结构(节点组);
    if(当前.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(当前.状态!=基础结构操作状态::已完成||当前.节点组.size()!=节点组.size())
        发布故障(L"当前需求内容联合保护读回不完整");
    for(std::size_t i=0;i<节点组.size();++i) {
        const auto& 项=当前.节点组[i];
        if(项.查询节点!=节点组[i]||(项.快照&&项.快照->结构.节点.编码!=节点组[i]))
            发布故障(L"当前需求内容联合保护身份错误");
        if(!项.快照||!需求内容快照相同(*原组.at(节点组[i]),*项.快照))return false;
    }
    return true;
}
struct 需求结果读取变化 final {};
void 核需求结果断言(bool value,const std::vector<基础联合节点快照>& protection,const wchar_t* why) {
    if(value)return;
    if(!需求内容联合仍相同(protection))throw 需求结果读取变化{};
    发布故障(why);
}
void 捕获需求结果节点(std::vector<基础联合节点快照>& protection,稳定编码 node) {
    const auto read=全局基础数据集.查询节点组完整结构({node});
    if(read.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    核需求结果断言(read.状态==基础结构操作状态::已完成&&read.节点组.size()==1
        &&read.节点组.front().查询节点==node&&read.节点组.front().快照.has_value(),protection,L"需求结果引用节点不可读");
    if(!合并需求内容保护(protection,{*read.节点组.front().快照}))throw 需求结果读取变化{};
}
void 记录迁移缺口(稳定编码 场景, 稳定编码 主体, const 需求特征迁移缺口& 缺口) noexcept {
    // 仅投影已取得的合法非成功材料；日志设施失败不改变该主体的迁移阻挡或其它主体的就绪。
    try {
        std::wstring 原因 = L"原因文本无法转换，请核对正式读取结果";
        if (缺口.原因.size() <= static_cast<std::size_t>((std::numeric_limits<int>::max)())) {
            const auto 长度 = static_cast<int>(缺口.原因.size());
            const auto 宽长度 = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                缺口.原因.data(), 长度, nullptr, 0);
            if (宽长度 > 0) {
                std::wstring 文本(static_cast<std::size_t>(宽长度), L'\0');
                if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, 缺口.原因.data(),
                    长度, 文本.data(), 宽长度) == 宽长度) 原因 = std::move(文本);
            }
        }
        const auto 可选编码 = [](const std::optional<稳定编码>& 节点) {
            return 节点 ? std::to_wstring(节点->值) : std::wstring(L"无");
        };
        const auto 内容 = L"旧需求特征待迁移；仅阻挡该主体消费，保留旧材料；场景="
            + std::to_wstring(场景.值) + L"；主体=" + std::to_wstring(主体.值)
            + L"；旧特征=" + std::to_wstring(缺口.原特征节点.值)
            + L"；需求=" + 可选编码(缺口.需求节点) + L"；动作=" + 可选编码(缺口.动作节点)
            + L"；状态=" + 可选编码(缺口.状态节点) + L"；原因=" + 原因;
        (void)记录运行日志(L"服务对象需求特征发布", L"初始化旧需求迁移缺口", 内容);
    } catch (...) { /* 尽力诊断失败不改变业务结果。 */ }
}
}

struct 服务对象需求发布服务::实现 final {
    新_世界树类& 世界;
    新_需求类& 需求;
    新世界事实服务引用 事实;
    稳定编码 类型, 旧类型;
    mutable std::mutex 互斥;
    mutable std::condition_variable 等待;
    bool 就绪 = false;
    mutable std::set<稳定编码> 占用;
    // 只读调用的共享占用：读与读可并行，与独占（发布、迁移）互斥。
    mutable std::map<稳定编码,std::uint32_t> 共享占用;
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
    mutable std::map<稳定编码,std::pair<std::uint_least32_t,std::thread::id>> 占用持有者;
#endif
    实现(新_世界树类& w, 新_需求类& d) : 世界(w), 需求(d), 事实(w.获取事实服务引用()) {}
    服务对象需求发布结果 发布(const 服务对象需求发布请求&,新需求发生方式) const;
    服务对象需求发生读取结果 读取发生(稳定编码) const;
    新本能已发布需求核验结果 核验已发布结果材料(const 新本能已发布需求结果&) const;
    bool 核验已完成迁移关联(稳定编码 C,稳定编码 H,稳定编码 F,
        const 新需求特征迁移信息&,const 新需求特征发布关联&,
        std::vector<基础联合节点快照>&) const;
    服务对象需求内容读取结果 读取当前结构(稳定编码,稳定编码,稳定编码,std::optional<稳定编码>) const;
    新需求目标定义核验结果 核验内容定义(const 新需求完整目标&) const;
    服务对象需求发布状态 取得(稳定编码 E,bool 共享=false,std::source_location 位置=std::source_location::current()) const {
        std::lock_guard 锁(互斥);
        if (!就绪) return 服务对象需求发布状态::已关闭;
        // 并发只读不是业务前提变化；只有与独占调用相遇才返回前提已变化。
        const bool 取得成功=共享 ? (!占用.contains(E)&&(++共享占用[E],true))
            : (!共享占用.contains(E)&&占用.insert(E).second);
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        if(取得成功&&!共享)占用持有者[E]={位置.line(),std::this_thread::get_id()};
        else if(!取得成功)try{
            const auto h=占用持有者.find(E);
            std::wostringstream t;t<<L"E="<<E.值<<L" 请求行="<<位置.line()
                <<L" 持有行="<<(h!=占用持有者.end()?h->second.first:0)
                <<L" 同线程="<<(h!=占用持有者.end()&&h->second.second==std::this_thread::get_id());
            (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"需求发布主体占用冲突",L"占用",t.str());
        }catch(...){}
#else
        (void)位置;
#endif
        return 取得成功 ? 服务对象需求发布状态::已读取 : 服务对象需求发布状态::前提已变化;
    }
    void 释放(稳定编码 E,bool 共享=false) const noexcept {
        std::lock_guard 锁(互斥);
        if(共享){const auto i=共享占用.find(E);if(i!=共享占用.end()&&--i->second==0)共享占用.erase(i);}
        else {
            占用.erase(E);
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
            占用持有者.erase(E);
#endif
        }
        等待.notify_all();
    }
    struct 调用结束 final {
        const 实现& 服务; 稳定编码 主体; bool 共享=false;
        ~调用结束() { 服务.释放(主体,共享); }
    };
    bool 场景有效(稳定编码 C, 稳定编码 E) const {
        const auto root = 世界.获取世界树根场景();
        return root && 事实.场景服务.是场景节点(C) && 事实.场景服务.存在属于场景范围(*root, C)
            && (C == E || 事实.场景服务.查询所属场景(E) == std::optional<稳定编码>{C});
    }
    std::optional<稳定编码> 找特征(稳定编码 E, 稳定编码 T,
        std::vector<基础联合节点快照>* protection=nullptr) const {
        const auto e = 事实.存在服务.获取存在(E); if (!e) return {};
        std::optional<稳定编码> found;
        for (const auto F : e->特征节点组) {
            if(protection)捕获需求结果节点(*protection,F);
            const auto f = 事实.特征服务.获取特征(F);
            const auto t = f ? 事实.特征服务.捕获特征匹配定义(f->特征概念节点) : std::nullopt;
            if (!f || !t) {
                if(protection)核需求结果断言(false,*protection,L"主体持有特征不可读");
                发布故障(L"主体持有特征不可读");
            }
            if(protection) {
                const auto definition=事实.特征概念服务.读取特征概念定义保护(f->特征概念节点);
                if(definition.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
                if(definition.前提已变化)throw 需求结果读取变化{};
                核需求结果断言(definition.信息&&definition.信息->类型根==t->特征类型根,*protection,L"主体特征类型定义不一致");
                if(!合并需求内容保护(*protection,definition.预期节点组))throw 需求结果读取变化{};
            }
            if (t->特征类型根 != T) continue;
            if (found || f->上级特征节点) {
                if(protection)核需求结果断言(false,*protection,L"主体需求特征不唯一或不是顶层");
                发布故障(L"主体需求特征不唯一或不是顶层");
            }
            found = F;
        }
        return found;
    }
    稳定编码 建空特征(稳定编码 E) const {
        if (const auto f = 找特征(E, 类型)) return *f;
        const auto add = 事实.存在服务.添加无当前值特征(E, 类型);
        if (add.状态 != 新存在操作状态::已完成) 发布故障(L"需求空特征建立未完成");
        const auto f = 找特征(E, 类型); if (!f) 发布故障(L"需求空特征读回缺失"); return *f;
    }
    服务对象需求发布状态 确认(const 新需求信息& D) const {
        using Q = 新被认知需求资格状态;
        using S = 服务对象需求发布状态;
        const auto q = 需求.读取被认知需求完整资格保护(D.节点, D.需求主体存在节点);
        switch (q.状态) {
        case Q::已读取: break;
        case Q::需求不存在: return S::需求不存在;
        case Q::主体不符: return S::主体不符;
        case Q::非被认知需求: return S::非被认知需求;
        case Q::内容待完善: return S::需求信息待完善;
        case Q::来源不符: return S::来源不符;
        case Q::需重新确认: return S::需重新确认;
        case Q::前提已变化: return S::前提已变化;
        case Q::资源失败: throw std::bad_alloc{};
        // 依赖未就绪属合法等待；本服务对外已把已关闭映射为依赖未就绪。
        case Q::依赖未就绪: return S::已关闭;
        default: 发布故障(L"需求完整资格返回未约定的工程状态");
        }
        if (!q.信息 || q.完整保护.empty()) 发布故障(L"需求完整资格成功缺少实际材料");
        const auto& actual = q.信息->需求;
        if (actual.信息完善阶段 != 新需求信息完善阶段::已完善) return S::需求信息待完善;
        const auto fresh = 需求.获取需求(D.节点);
        return fresh && *fresh == D && 需求内容联合仍相同(q.完整保护)
            ? S::已读取 : S::前提已变化;
    }
    std::vector<稳定编码> 集合成员(const 新特征准确值& value) const {
        const auto* n = std::get_if<稳定编码>(&value);
        const auto material = n ? 事实.特征值服务.获取特征值(*n) : std::nullopt;
        const auto* raw = material && material->物理类型 == 材料物理类型::U64数组
            ? std::get_if<std::vector<std::uint64_t>>(&material->材料) : nullptr;
        if (!raw) 发布故障(L"旧需求集合材料损坏");
        std::vector<稳定编码> result;
        for (const auto x : *raw) { if (!x) 发布故障(L"旧需求集合含无效节点"); result.push_back(稳定编码{x}); }
        std::sort(result.begin(), result.end());
        if (std::adjacent_find(result.begin(), result.end()) != result.end()) 发布故障(L"旧需求集合含重复成员");
        return result;
    }
    std::vector<稳定编码> 历史(const 新特征信息& f) const {
        std::vector<稳定编码> result;
        for (const auto& v : f.历史不同值) {
            const auto* d = std::get_if<稳定编码>(&v); if (!d || !有效(*d)) 发布故障(L"需求历史不是合法单节点引用");
            result.push_back(*d);
        }
        if (std::adjacent_find(result.begin(), result.end()) != result.end()) 发布故障(L"需求历史身份重复");
        if (f.当前值) {
            const auto* d = std::get_if<稳定编码>(&*f.当前值);
            if (!d || !std::binary_search(result.begin(), result.end(), *d)) 发布故障(L"需求当前值不在历史中");
        } else if (!result.empty() || f.标准值) 发布故障(L"需求无当前值却有历史");
        return result;
    }
    bool 核验新关联(稳定编码 C, 稳定编码 E, 稳定编码 F, const 新需求特征发布关联& source,
        std::vector<基础联合节点快照>* 输出保护=nullptr) const {
        std::vector<基础联合节点快照> protection;
        const auto capture = [&](稳定编码 n) {
            if (std::any_of(protection.begin(), protection.end(), [&](const auto& x) { return x.结构.节点.编码 == n; })) return;
            const auto read = 全局基础数据集.查询节点组完整结构({n});
            if(read.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if (read.状态 != 基础结构操作状态::已完成 || read.节点组.size() != 1
                || read.节点组.front().查询节点 != n || !read.节点组.front().快照) 发布故障(L"需求发布关联联合保护不可读");
            protection.push_back(*read.节点组.front().快照);
        };
        const auto fail = [&](const wchar_t* why) {
            if (!需求内容联合仍相同(protection)) return false;
            发布故障(why);
        };
        for (const auto n : {C, E, F, source.动作节点}) capture(n);
        if (source.场景节点 != C || source.主体存在节点 != E) 发布故障(L"需求发布来源场景或主体不符");
        const auto root = 世界.获取世界树根场景();
        if (!root) return fail(L"需求发布关联缺少世界根");
        std::set<稳定编码> chain;
        std::optional<稳定编码> CA;
        auto current = C;
        for (;;) {
            if (!chain.insert(current).second) return fail(L"需求发布关联父链循环");
            capture(current);
            const auto scene = 事实.场景服务.获取场景(current);
            if (!scene) return fail(L"需求发布关联场景损坏");
            if (std::find(scene->动态节点组.begin(), scene->动态节点组.end(), source.动作节点) != scene->动态节点组.end()) {
                if (CA) return fail(L"需求发布动作重复挂接祖先场景");
                CA = current;
            }
            if (current == *root) {
                if (scene->父场景节点) return fail(L"需求发布关联世界根具有父场景");
                break;
            }
            if (!scene->父场景节点) return fail(L"需求发布关联不属于当前世界");
            const auto parent = *scene->父场景节点;
            capture(parent);
            const auto p = 事实.场景服务.获取场景(parent);
            if (!p || std::count(p->直接子场景组.begin(), p->直接子场景组.end(), current) != 1)
                return fail(L"需求发布关联父子场景关系损坏");
            current = parent;
        }
        if (!CA) return fail(L"需求发布动作未唯一挂接于结果场景祖先");
        const auto actionProtection = std::find_if(protection.begin(), protection.end(),
            [&](const auto& x) { return x.结构.节点.编码 == source.动作节点; });
        // 复制入向字段，后面的capture可能重分配protection。
        const auto incoming = actionProtection->入向字段组;
        for (const auto& field : incoming) {
            if (!事实.场景服务.是场景节点(field.所属节点)) continue;
            capture(field.所属节点);
            const auto scene = 事实.场景服务.获取场景(field.所属节点);
            if (!scene) return fail(L"需求发布动作挂接场景损坏");
            if (field.所属节点 != *CA && std::find(scene->动态节点组.begin(), scene->动态节点组.end(), source.动作节点) != scene->动态节点组.end())
                return fail(L"需求发布动作跨场景重复挂接");
        }
        // 读取的是历史事实，不要求动作主体今天仍位于过去的CA。
        const auto a = 事实.动态服务.获取动态(source.动作节点);
        const auto ds = 事实.场景服务.查询动态列表(C);
        if (!a || a->种类 != 新动态种类::动作 || a->动作目标种类 != 新动作目标种类::普通存在
            || a->动作目标存在节点 != E || a->动作目标特征节点 != F || !a->动作时间
            || !a->动作定义节点 || !a->动作来源节点) return fail(L"需求发布动作来源不符");
        capture(*a->动作定义节点); capture(*a->动作来源节点);
        std::optional<稳定编码> after;
        if (source.首状态节点) {
            if (!a->动作结果组.empty()) return fail(L"需求首值伪有变化动态");
            after = source.首状态节点;
        } else {
            if (a->动作结果组.size() != 1 || a->动作结果组.front().类型 != 新动作结果关系类型::导致变化) return fail(L"需求发布缺少明确变化来源");
            capture(a->动作结果组.front().结果动态节点);
            const auto d = 事实.动态服务.获取动态(a->动作结果组.front().结果动态节点);
            if (!d || d->种类 != 新动态种类::原子 || d->主题存在节点 != E || d->状态节点组.size() != 2
                || std::count(ds.begin(), ds.end(), d->节点) != 1) return fail(L"需求变化动态不可读");
            capture(d->状态节点组.front());
            const auto pre = 事实.状态服务.获取状态(d->状态节点组.front());
            const auto states = 事实.场景服务.查询状态列表(C);
            if (!pre || pre->被描述存在节点 != E || pre->来源实例特征节点 != F
                || pre->强时间.语义 != 新状态时间语义::实例绝对UTC || a->动作时间->纳秒 <= pre->强时间.纳秒
                || std::count(states.begin(), states.end(), pre->节点) != 1) return fail(L"需求前状态来源不符");
            after = d->状态节点组.back();
        }
        capture(*after);
        const auto s = 事实.状态服务.获取状态(*after);
        const auto ss = 事实.场景服务.查询状态列表(C);
        if (!s || s->被描述存在节点 != E || s->来源实例特征节点 != F
            || s->固定准确值 != 新特征准确值{source.需求节点} || s->强时间.语义 != 新状态时间语义::实例绝对UTC
            || a->动作时间->语义 != 新状态时间语义::实例绝对UTC || s->强时间.纳秒 <= a->动作时间->纳秒
            || std::count(ss.begin(), ss.end(), *after) != 1) return fail(L"需求后状态与具体需求不符");
        if(!需求内容联合仍相同(protection))return false;
        return !输出保护||合并需求内容保护(*输出保护,protection);
    }

    // 所有旧材料只读；没有唯一完整证据时保留原数据并给出具名缺口。
    服务对象需求发布状态 迁移(稳定编码 C, 稳定编码 E, std::vector<需求特征迁移缺口>& gaps, bool 允许恢复 = false) const {
        using S = 服务对象需求发布状态;
        const auto oldId = 找特征(E, 旧类型); if (!oldId) return S::已读取;
        const auto old = 事实.特征服务.获取特征(*oldId); if (!old) 发布故障(L"旧需求特征不可读");
        std::vector<稳定编码> all;
        std::vector<std::vector<稳定编码>> history;
        for (const auto& v : old->历史不同值) {
            auto members = 集合成员(v); all.insert(all.end(), members.begin(), members.end()); history.push_back(std::move(members));
        }
        std::vector<稳定编码> current;
        if (old->当前值) { current = 集合成员(*old->当前值); all.insert(all.end(), current.begin(), current.end()); }
        std::sort(all.begin(), all.end()); all.erase(std::unique(all.begin(), all.end()), all.end());
        if (all.empty()) return S::已读取;
        const auto gap = [&](const char* why, std::optional<稳定编码> D = {}, std::optional<稳定编码> A = {}, std::optional<稳定编码> state = {}) {
            gaps.push_back({*oldId, D, A, state, why});
        };
        if (!old->当前值) { gap("旧需求特征无当前集合，不能恢复末需求"); return S::旧需求特征待迁移; }
        struct Step { 新需求特征迁移来源项 item; std::vector<稳定编码> before, after; std::int64_t start, end; };
        std::vector<Step> steps;
        for (const auto D : all) {
            const auto d = 需求.获取需求(D);
            if (!d || d->执行域 != 新需求执行域::被认知 || d->需求主体存在节点 != E) 发布故障(L"旧需求集合引用非法需求或异主体");
            const auto origin = 需求.读取主体需求集合发布关联(D);
            if (origin.状态 == 新需求操作状态::前提已变化) return S::前提已变化;
            if (origin.状态 != 新需求操作状态::已找到) 发布故障(L"旧需求发布来源读取失败");
            if (!origin.关联) { gap("缺少需求原首次发布来源", D); continue; }
            const auto& source = *origin.关联;
            const auto a = 事实.动态服务.获取动态(source.动作节点);
            if (source.主体存在节点 != E || source.场景节点 != C || !a || a->种类 != 新动态种类::动作
                || a->动作目标存在节点 != E || a->动作目标特征节点 != *oldId
                || a->动作结果组.size() != 1 || a->动作结果组.front().类型 != 新动作结果关系类型::导致变化) {
                gap("旧首次发布动作缺少唯一变化结果", D, source.动作节点); continue;
            }
            const auto dynamic = 事实.动态服务.获取动态(a->动作结果组.front().结果动态节点);
            if (!dynamic || dynamic->种类 != 新动态种类::原子 || dynamic->主题存在节点 != E || dynamic->状态节点组.size() != 2) {
                gap("原动作缺少完整两状态变化", D, source.动作节点); continue;
            }
            const auto pre = 事实.状态服务.获取状态(dynamic->状态节点组.front()), post = 事实.状态服务.获取状态(dynamic->状态节点组.back());
            if (!pre || !post || pre->来源实例特征节点 != *oldId || post->来源实例特征节点 != *oldId
                || pre->被描述存在节点 != E || post->被描述存在节点 != E) { gap("原前后状态来源不完整", D, source.动作节点); continue; }
            if (!pre->固定准确值 || !post->固定准确值) { gap("原前后状态缺少准确值材料", D, source.动作节点); continue; }
            auto before = 集合成员(*pre->固定准确值), after = 集合成员(*post->固定准确值), expected = before;
            if (std::binary_search(before.begin(), before.end(), D)) { gap("需求在首次发布前已存在，命中次数不可恢复", D, source.动作节点); continue; }
            expected.push_back(D); std::sort(expected.begin(), expected.end());
            if (after != expected || pre->强时间.语义 != 新状态时间语义::实例绝对UTC
                || post->强时间.语义 != 新状态时间语义::实例绝对UTC || pre->强时间.纳秒 >= post->强时间.纳秒) {
                gap("原变化不是唯一追加该需求或时间非法", D, source.动作节点); continue;
            }
            steps.push_back({{D, source.动作节点, pre->节点, post->节点}, std::move(before), std::move(after), pre->强时间.纳秒, post->强时间.纳秒});
        }
        if (!gaps.empty()) return S::旧需求特征待迁移;
        std::vector<新需求特征迁移来源项> ordered;
        std::vector<稳定编码> prefix;
        std::optional<std::int64_t> time;
        std::set<稳定编码> used;
        while (ordered.size() < steps.size()) {
            const Step* selected = nullptr;
            for (const auto& step : steps) if (!used.contains(step.item.需求节点) && step.before == prefix) {
                if (selected) { gap("旧追加链分叉，不能确定需求时序"); return S::旧需求特征待迁移; }
                selected = &step;
            }
            if (!selected || (time && selected->start < *time)) { gap("旧追加链断裂或时间冲突"); return S::旧需求特征待迁移; }
            used.insert(selected->item.需求节点); ordered.push_back(selected->item); prefix = selected->after; time = selected->end;
        }
        if (prefix != current) { gap("旧追加链末集合不等于当前集合"); return S::旧需求特征待迁移; }
        for (const auto& h : history) {
            const bool empty = h.empty();
            if (!empty && std::none_of(steps.begin(), steps.end(), [&](const auto& step) { return step.after == h; })) {
                gap("历史集合不属于追加链前缀"); return S::旧需求特征待迁移;
            }
        }
        const auto found = 找特征(E, 类型);
        if (!found && !允许恢复) { gap("旧需求需要显式迁移，读取不建立新需求特征"); return S::旧需求特征待迁移; }
        const auto F = found ? *found : 建空特征(E);
        新需求特征事实核验输入 input{新需求特征事实用途::旧集合迁移, C, E, *oldId, F, ordered.back().需求节点, {}, {}, ordered};
        auto proof = 世界.核验需求特征发布事实(input);
        if (proof.状态 == 新需求特征事实核验状态::前提已变化) return S::前提已变化;
        if (proof.状态 != 新需求特征事实核验状态::已核验 || !proof.凭据) { gap("世界未能核验完整旧集合动作状态链"); return S::旧需求特征待迁移; }
        const auto existing = 需求.读取需求特征迁移(F);
        if (existing.状态 != 新需求操作状态::已找到) 发布故障(L"需求迁移映射读取失败");
        if (existing.信息) {
            const auto& m = *existing.信息;
            if (m.主体存在节点 != E || m.场景节点 != C || m.原特征节点 != *oldId || m.新特征节点 != F
                || m.有序来源组 != ordered || m.当前需求节点 != ordered.back().需求节点) 发布故障(L"已有需求迁移映射异义");
            if (!m.迁移当前状态节点) 发布故障(L"需求迁移核心已发表但无直接派生回执，需工程恢复");
            const auto f = 事实.特征服务.获取特征(F); if (!f) 发布故障(L"迁移后需求特征不可读");
            const auto ds = 历史(*f);
            for (const auto D : all) if (!std::binary_search(ds.begin(), ds.end(), D)) 发布故障(L"已迁移需求不在新特征历史中");
            return S::已读取;
        }
        if (!允许恢复) { gap("旧需求尚无已完成迁移映射，读取不执行历史恢复"); return S::旧需求特征待迁移; }
        auto mapping = 需求.准备需求特征迁移(*proof.凭据);
        if (mapping.状态 == 新需求操作状态::前提已变化) return S::前提已变化;
        if (mapping.状态 != 新需求操作状态::已建立 || !mapping.准备) 发布故障(L"需求迁移来源映射准备失败");
        新特征需求历史恢复请求 restore;
        restore.特征节点 = F; restore.当前需求节点 = ordered.back().需求节点; restore.标准需求节点 = ordered.front().需求节点;
        restore.来源保护组 = proof.凭据->依据().完整保护组;
        for (const auto& item : ordered) restore.历史值命中信息.push_back({新特征准确值{item.需求节点}, 1});
        auto prepared = 事实.特征服务.准备顶层需求引用历史恢复(restore);
        if (prepared.状态 == 新特征I64准备状态::预期已变化 || prepared.状态 == 新特征I64准备状态::联合更新占用) return S::前提已变化;
        if (prepared.状态 != 新特征I64准备状态::已准备 || !prepared.准备) 发布故障(L"需求单引用历史恢复准备失败");
        const auto commit = 事实.特征服务.提交顶层需求引用更新(*prepared.准备, {mapping.准备->片段()});
        if (commit.状态 == 基础节点组追加状态::预期不符) { 事实.特征服务.取消顶层需求引用更新(*prepared.准备); return S::前提已变化; }
        if (commit.状态 != 基础节点组追加状态::已完成) 发布故障(L"需求历史恢复与映射联合提交失败");
        std::optional<基础片段发布回执> receipt;
        for (const auto& x : commit.附加片段回执组) if (x.对应片段(mapping.准备->片段())) {
            if (receipt) 发布故障(L"需求迁移映射回执重复"); receipt = x;
        }
        if (!receipt) 发布故障(L"需求迁移映射回执缺失");
        需求.完成需求特征迁移核心(*mapping.准备, *receipt);
        const auto restored = 事实.特征服务.完成顶层需求引用历史恢复(*prepared.准备);
        if (restored.状态 != 新特征写入状态::已完成 || restored.命中次数已增加) 发布故障(L"需求历史恢复完成不符");
        auto fresh = 世界.核验需求特征发布事实(input);
        if (fresh.状态 != 新需求特征事实核验状态::已核验 || !fresh.凭据) 发布故障(L"需求迁移核心已发表但旧链重核验失败");
        auto derive = 需求.准备需求特征迁移派生(F, *fresh.凭据);
        if (derive.状态 != 新需求操作状态::已建立 || !derive.凭据) 发布故障(L"需求迁移派生来源准备失败");
        const auto changed = 世界.完成需求引用迁移派生(*derive.凭据, *prepared.准备);
        if (!changed.新状态节点 || changed.状态 != 新世界特征变化状态::已生成状态但无可连接前状态) 发布故障(L"需求迁移状态派生失败");
        input.用途 = 新需求特征事实用途::迁移当前状态; input.实际状态节点 = changed.新状态节点;
        auto stateProof = 世界.核验需求特征发布事实(input);
        if (stateProof.状态 != 新需求特征事实核验状态::已核验 || !stateProof.凭据) 发布故障(L"需求迁移实际状态核验失败");
        const auto done = 需求.完成需求特征迁移状态(F, *stateProof.凭据);
        if (done != 新需求操作状态::已建立 && done != 新需求操作状态::已复用) 发布故障(L"需求迁移状态映射完成失败");
        const auto map = 需求.读取需求特征迁移(F); const auto f = 事实.特征服务.获取特征(F);
        if (!map.信息 || map.信息->迁移当前状态节点 != changed.新状态节点 || !f
            || f->当前值 != std::optional<新特征准确值>{新特征准确值{ordered.back().需求节点}}
            || f->标准值 != std::optional<新特征准确值>{新特征准确值{ordered.front().需求节点}}
            || 历史(*f) != all || f->历史值命中信息.size() != all.size()) 发布故障(L"需求迁移最终完整读回失败");
        for (const auto& hit : f->历史值命中信息) if (hit.命中次数 != 1) 发布故障(L"需求迁移命中次数不符");
        事实.特征服务.结束顶层需求引用更新(*prepared.准备);
        return S::已读取;
    }

    服务对象需求读取结果 读取(稳定编码 C, 稳定编码 E) const {
        using S = 服务对象需求发布状态;
        服务对象需求读取结果 out;
        if (!场景有效(C, E)) return out;
        out.状态 = 迁移(C, E, out.迁移缺口组); if (out.状态 != S::已读取) return out;
        out.需求特征节点 = 找特征(E, 类型); if (!out.需求特征节点) return out;
        const auto F = *out.需求特征节点;
        const auto f = 事实.特征服务.获取特征(F); if (!f) 发布故障(L"需求特征不可读");
        out.历史需求节点组 = 历史(*f);
        if (f->当前值) out.当前需求节点 = std::get<稳定编码>(*f->当前值);
        const auto mapping = 需求.读取需求特征迁移(F);
        if (mapping.状态 != 新需求操作状态::已找到) 发布故障(L"读取需求迁移映射失败");
        for (const auto n : out.历史需求节点组) {
            const auto d = 需求.读取需求治理信息(n);
            if (!d || d->执行域 != 新需求执行域::被认知 || d->需求主体存在节点 != E) 发布故障(L"需求历史含非法或异主体需求");
            const auto source = 需求.读取主体需求特征发布关联(n);
            // 并发变化与依赖未就绪按本函数既有口径返回；资源失败在发布链属工程故障，保持终止。
            if (source.状态 == 新需求操作状态::前提已变化) { out.状态 = S::前提已变化; return out; }
            if (source.状态 == 新需求操作状态::目标内容依赖未就绪) { out.状态 = S::已关闭; return out; }
            if (source.状态 != 新需求操作状态::已找到 || !source.关联) 发布故障(L"需求历史缺少首次发布来源");
            const auto oldItem = mapping.信息 ? std::find_if(mapping.信息->有序来源组.begin(), mapping.信息->有序来源组.end(), [&](const auto& x) { return x.需求节点 == n; }) : std::vector<新需求特征迁移来源项>::const_iterator{};
            if (mapping.信息 && oldItem != mapping.信息->有序来源组.end()) {
                if (!mapping.信息->迁移当前状态节点 || oldItem->动作节点 != source.关联->动作节点
                    || source.关联->主体存在节点 != E || source.关联->场景节点 != C) 发布故障(L"迁移需求原发布来源与映射不符");
            } else if (!核验新关联(C, E, F, *source.关联)) { out.状态 = S::前提已变化; return out; }
            if(d->信息完善阶段==新需求信息完善阶段::待完善)out.待完善节点组.push_back(n);
            else {
                const auto full=需求.获取需求(n);
                if(!full){out.状态=S::前提已变化;return out;}
                const auto status=确认(*full);
                if(status==S::需重新确认)out.需重新确认节点组.push_back(n);
                else if(status==S::前提已变化||status==S::已关闭){out.状态=status;return out;}
                else if(status!=S::已读取)发布故障(L"需求历史确认来源损坏");
            }
        }
        if (事实.特征服务.获取特征(F) != f) out.状态 = S::前提已变化;
        return out;
    }
};

bool 服务对象需求发布服务::实现::核验已完成迁移关联(稳定编码 C,稳定编码 H,稳定编码 F,
    const 新需求特征迁移信息& mapping,const 新需求特征发布关联& source,
    std::vector<基础联合节点快照>& protection) const {
    const auto require=[&](bool ok,const wchar_t* why){核需求结果断言(ok,protection,why);};
    const auto capture=[&](稳定编码 n){捕获需求结果节点(protection,n);};
    require(mapping.场景节点==C&&mapping.主体存在节点==H&&mapping.新特征节点==F
        &&有效(mapping.原特征节点)&&mapping.原特征节点!=F&&有效(mapping.迁移记录节点)
        &&mapping.迁移当前状态节点&&!mapping.有序来源组.empty(),L"需求结果迁移定位不完整");
    std::vector<稳定编码> layer{mapping.迁移记录节点};
    // 固定两层直接引用闭包包含来源项物理节点及其D/A/前后S，不无界遍历。
    for(int depth=0;depth<3;++depth) {
        std::set<稳定编码> next;
        for(const auto n:layer) {
            capture(n);
            const auto& snap=*std::find_if(protection.begin(),protection.end(),[&](const auto& s){return s.结构.节点.编码==n;});
            for(const auto& f:snap.结构.字段组)if(const auto* ref=std::get_if<稳定编码>(&f.内容))next.insert(*ref);
        }
        layer.assign(next.begin(),next.end());
    }
    const auto sameMap=[&](const 新需求特征迁移信息& m) {
        return m.迁移记录节点==mapping.迁移记录节点&&m.场景节点==C&&m.主体存在节点==H
            &&m.原特征节点==mapping.原特征节点&&m.新特征节点==F
            &&m.当前需求节点==mapping.当前需求节点&&m.有序来源组==mapping.有序来源组
            &&m.迁移当前状态节点==mapping.迁移当前状态节点;
    };
    const auto fresh=需求.读取需求特征迁移(F);
    require(fresh.状态==新需求操作状态::已找到&&fresh.信息&&sameMap(*fresh.信息),L"需求结果迁移映射异义");
    const auto selected=std::find_if(mapping.有序来源组.begin(),mapping.有序来源组.end(),
        [&](const auto& x){return x.需求节点==source.需求节点;});
    require(selected!=mapping.有序来源组.end()
        &&std::count_if(mapping.有序来源组.begin(),mapping.有序来源组.end(),[&](const auto& x){return x.需求节点==source.需求节点;})==1
        &&selected->动作节点==source.动作节点&&source.场景节点==C&&source.主体存在节点==H,
        L"需求结果迁移来源不唯一或错配");
    const auto uniqueInScene=[&](稳定编码 n,bool dynamic) {
        capture(n);capture(C);
        const auto list=dynamic?事实.场景服务.查询动态列表(C):事实.场景服务.查询状态列表(C);
        require(std::count(list.begin(),list.end(),n)==1,L"迁移需求结果未唯一挂接原场景");
        const auto snapshot=*std::find_if(protection.begin(),protection.end(),[&](const auto& s){return s.结构.节点.编码==n;});
        for(const auto& field:snapshot.入向字段组)if(事实.场景服务.是场景节点(field.所属节点)) {
            capture(field.所属节点);
            const auto values=dynamic?事实.场景服务.查询动态列表(field.所属节点):事实.场景服务.查询状态列表(field.所属节点);
            require(field.所属节点==C||std::find(values.begin(),values.end(),n)==values.end(),L"迁移需求结果跨场景重复挂接");
        }
    };
    const auto oldFeature=事实.特征服务.获取特征(mapping.原特征节点);
    const auto newFeature=事实.特征服务.获取特征(F);
    require(oldFeature&&newFeature,L"迁移需求结果特征不可读");
    const auto oldType=事实.特征概念服务.读取特征概念定义保护(oldFeature->特征概念节点);
    const auto newType=事实.特征概念服务.读取特征概念定义保护(newFeature->特征概念节点);
    if(oldType.状态==基础结构操作状态::资源失败||newType.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(oldType.前提已变化||newType.前提已变化)throw 需求结果读取变化{};
    require(oldType.信息&&newType.信息,L"迁移需求结果特征类型不可读");
    if(!合并需求内容保护(protection,oldType.预期节点组)||!合并需求内容保护(protection,newType.预期节点组))throw 需求结果读取变化{};
    const auto state=[&](稳定编码 n,稳定编码 feature,稳定编码 root) {
        uniqueInScene(n,false);const auto s=事实.状态服务.获取状态(n);
        require(s&&s->被描述存在节点==H&&s->来源实例特征节点==feature
            &&s->强时间.语义==新状态时间语义::实例绝对UTC,L"迁移需求结果状态身份不符");
        const auto type=事实.特征概念服务.读取特征概念定义保护(s->特征类型概念节点);
        if(type.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(type.前提已变化)throw 需求结果读取变化{};
        require(type.信息&&type.信息->类型根==root,L"迁移需求结果状态特征类型不符");
        if(!合并需求内容保护(protection,type.预期节点组))throw 需求结果读取变化{};
        require(s->固定准确值.has_value(),L"迁移需求结果状态缺少准确值");
        if(const auto* v=(s->固定准确值 ? std::get_if<稳定编码>(&*s->固定准确值) : nullptr))capture(*v);
        return *s;
    };
    uniqueInScene(source.动作节点,true);
    const auto action=事实.动态服务.获取动态(source.动作节点);
    require(action&&action->种类==新动态种类::动作&&action->动作目标种类==新动作目标种类::普通存在
        &&action->动作目标存在节点==H&&action->动作目标特征节点==mapping.原特征节点
        &&action->动作定义节点&&action->动作来源节点&&action->动作时间
        &&action->动作时间->语义==新状态时间语义::实例绝对UTC&&action->动作结果组.size()==1
        &&action->动作结果组.front().类型==新动作结果关系类型::导致变化,L"迁移需求结果原动作不符");
    capture(*action->动作定义节点);capture(*action->动作来源节点);capture(action->主题存在节点);
    const auto changeId=action->动作结果组.front().结果动态节点;uniqueInScene(changeId,true);
    const auto change=事实.动态服务.获取动态(changeId);
    require(change&&change->种类==新动态种类::原子&&change->主题存在节点==H
        &&change->状态节点组==std::vector<稳定编码>{selected->前状态节点,selected->后状态节点},L"迁移需求结果原变化不符");
    const auto before=state(selected->前状态节点,mapping.原特征节点,oldType.信息->类型根);
    const auto after=state(selected->后状态节点,mapping.原特征节点,oldType.信息->类型根);
    require(before.强时间.纳秒<=action->动作时间->纳秒&&action->动作时间->纳秒<after.强时间.纳秒,
        L"迁移需求结果原动作时间不符");
    const auto previous=集合成员(*before.固定准确值);const auto following=集合成员(*after.固定准确值);
    require(!std::binary_search(previous.begin(),previous.end(),source.需求节点),L"迁移需求结果原前集合已含D");
    auto expected=previous;expected.push_back(source.需求节点);std::sort(expected.begin(),expected.end());
    require(expected==following,L"迁移需求结果不是唯一追加D");
    const auto& tail=mapping.有序来源组.back();
    const auto last=state(tail.后状态节点,mapping.原特征节点,oldType.信息->类型根);
    const auto migrated=state(*mapping.迁移当前状态节点,F,newType.信息->类型根);
    require(mapping.当前需求节点==tail.需求节点&&last.强时间==migrated.强时间
        &&migrated.固定准确值==新特征准确值{mapping.当前需求节点},L"迁移需求结果派生状态不符");
    const auto history=历史(*newFeature);
    for(const auto& item:mapping.有序来源组)require(std::binary_search(history.begin(),history.end(),item.需求节点),L"迁移需求结果历史遗漏原D");
    return 需求内容联合仍相同(protection);
}
新本能已发布需求核验结果 服务对象需求发布服务::实现::核验已发布结果材料(
    const 新本能已发布需求结果& result) const {
    using S=新本能已发布需求核验状态;
    std::vector<基础联合节点快照> protection;
    const auto require=[&](bool ok,const wchar_t* why){核需求结果断言(ok,protection,why);};
    const auto capture=[&](稳定编码 n){捕获需求结果节点(protection,n);};
    for(const auto n:{result.需求节点,result.主体存在节点,result.需求特征节点,
        result.结果场景节点,result.本次动作节点,result.首次发布动作节点}) {
        require(有效(n),L"需求结果缺少正式引用");capture(n);
    }
    const auto C=result.结果场景节点,H=result.主体存在节点,F=result.需求特征节点,D=result.需求节点;
    if(!场景有效(C,H)) {
        const auto source=需求.读取主体需求特征发布关联(D);
        require(source.状态==新需求操作状态::已找到&&source.关联&&source.关联->场景节点==C
            &&source.关联->主体存在节点==H&&source.关联->动作节点==result.首次发布动作节点,
            L"需求结果主体原发布场景不符");
        // 正式关联证明的是过去C；H今天已不在C仅使本次读回前提变化，不改旧事实。
        return {};
    }
    require(找特征(H,类型,&protection)==std::optional{F},L"需求结果主体特征不符");
    const auto f=事实.特征服务.获取特征(F);require(f.has_value(),L"需求结果特征不可读");
    const auto history=历史(*f);require(std::binary_search(history.begin(),history.end(),D),L"需求结果未进入正式历史");
    const auto content=需求.读取被认知需求完整资格保护(D,H);
    if(content.状态==新被认知需求资格状态::资源失败)throw std::bad_alloc{};
    if(content.状态==新被认知需求资格状态::前提已变化)throw 需求结果读取变化{};
    require(content.状态==新被认知需求资格状态::已读取&&content.信息
        &&content.信息->需求.信息完善阶段==新需求信息完善阶段::已完善,L"需求结果完整来源资格不可读");
    if(!合并需求内容保护(protection,content.完整保护))throw 需求结果读取变化{};
    const auto demand=需求.获取需求(D);
    require(demand&&demand->执行域==新需求执行域::被认知&&demand->需求主体存在节点==H,L"需求结果不是完整被认知需求");
    const auto origin=需求.读取主体需求特征发布关联(D);
    require(origin.状态==新需求操作状态::已找到&&origin.关联
        &&origin.关联->需求节点==D&&origin.关联->主体存在节点==H&&origin.关联->场景节点==C
        &&origin.关联->动作节点==result.首次发布动作节点,L"需求结果首次发布关联不符");
    const auto mapping=需求.读取需求特征迁移(F);
    require(mapping.状态==新需求操作状态::已找到,L"需求结果迁移身份读取失败");
    const bool migrated=mapping.信息&&std::any_of(mapping.信息->有序来源组.begin(),mapping.信息->有序来源组.end(),[&](const auto& x){return x.需求节点==D;});
    if(migrated) {
        if(!核验已完成迁移关联(C,H,F,*mapping.信息,*origin.关联,protection))throw 需求结果读取变化{};
        require(result.首次发布动作节点!=result.本次动作节点,L"迁移复用不得改写原A目标");
    } else if(!核验新关联(C,H,F,*origin.关联,&protection))throw 需求结果读取变化{};
    const auto action=事实.动态服务.获取动态(result.本次动作节点);
    require(action&&action->种类==新动态种类::动作&&action->动作目标种类==新动作目标种类::普通存在
        &&action->动作目标存在节点==H&&action->动作目标特征节点==F&&action->动作定义节点
        &&action->动作来源节点&&action->动作时间&&action->动作时间->语义==新状态时间语义::实例绝对UTC,
        L"需求结果本次动作不符");
    capture(*action->动作定义节点);capture(*action->动作来源节点);capture(action->主题存在节点);
    const auto root=世界.获取世界树根场景();require(root.has_value(),L"需求结果本次动作缺少世界根");
    std::optional<稳定编码> actionScene;
    std::set<稳定编码> visited;
    auto current=C;
    while(true) {
        require(visited.insert(current).second,L"需求结果本次动作祖先链循环");capture(current);
        const auto scene=事实.场景服务.获取场景(current);require(scene.has_value(),L"需求结果本次动作祖先不可读");
        const auto count=std::count(scene->动态节点组.begin(),scene->动态节点组.end(),result.本次动作节点);
        require(count<=1,L"需求结果本次动作在场景内重复");
        if(count){require(!actionScene,L"需求结果本次动作跨祖先重复挂接");actionScene=current;}
        if(current==*root){require(!scene->父场景节点,L"需求结果世界根具有父场景");break;}
        require(scene->父场景节点.has_value(),L"需求结果本次动作不属当前世界");
        const auto parent=*scene->父场景节点;capture(parent);const auto p=事实.场景服务.获取场景(parent);
        require(p&&std::count(p->直接子场景组.begin(),p->直接子场景组.end(),current)==1,L"需求结果父子场景不一致");
        current=parent;
    }
    require(actionScene.has_value(),L"需求结果本次动作未挂接结果场景祖先");
    const auto actionSnapshot=*std::find_if(protection.begin(),protection.end(),[&](const auto& p){return p.结构.节点.编码==result.本次动作节点;});
    for(const auto& f:actionSnapshot.入向字段组)if(事实.场景服务.是场景节点(f.所属节点)) {
        capture(f.所属节点);const auto list=事实.场景服务.查询动态列表(f.所属节点);
        require(f.所属节点==*actionScene||std::find(list.begin(),list.end(),result.本次动作节点)==list.end(),L"需求结果本次动作跨场景重复");
    }
    if(result.首次发布动作节点!=result.本次动作节点) {
        const auto first=事实.动态服务.获取动态(result.首次发布动作节点);
        require(first&&first->动作时间&&first->动作时间->语义==新状态时间语义::实例绝对UTC
            &&first->动作时间->纳秒<action->动作时间->纳秒,L"需求结果复用动作时序不符");
    }
    if(!需求内容联合仍相同(protection))return {};
    return {S::已核验,result,std::move(protection)};
}
服务对象需求发布服务::服务对象需求发布服务(新_世界树类& w, 新_需求类& d) noexcept {
    try { 实现_ = std::make_unique<实现>(w, d); } catch (...) { 发布故障(L"构造需求发布服务失败"); }
}
服务对象需求发布服务::~服务对象需求发布服务() { 关闭(); }
bool 服务对象需求发布服务::初始化(稳定编码 T, 稳定编码 oldT) noexcept {
    try {
        const auto innate = 实现_->世界.获取先天特征概念();
        if (!innate || T != innate->需求 || oldT != innate->服务对象需求集合 || T == oldT) return false;
        {
            std::lock_guard lock(实现_->互斥);
            if (实现_->就绪) return 实现_->类型 == T && 实现_->旧类型 == oldT;
            实现_->类型 = T; 实现_->旧类型 = oldT;
        }
        // 初始化是明确迁移入口；所有owner调用均在运行期占用锁外。
        const auto root = 实现_->世界.获取世界树根场景();
        if (!root) return false;
        const auto 范围 = 实现_->事实.场景服务.读取指定场景范围完整(*root);
        if (范围.状态 == 新场景范围成员读取状态::读取已变化) return false;
        if (范围.状态 != 新场景范围成员读取状态::已完成)
            发布故障(L"初始化迁移场景范围未能完整读取，不能当作空范围");
        for (const auto& 场景项 : 范围.场景成员组) {
            const auto C = 场景项.场景节点;
            auto members = 场景项.直接成员存在组;
            members.push_back(C);
            std::sort(members.begin(), members.end()); members.erase(std::unique(members.begin(), members.end()), members.end());
            for (const auto E : members) {
                if (!实现_->找特征(E, oldT)) continue;
                {
                    std::lock_guard lock(实现_->互斥);
                    if (实现_->共享占用.contains(E)||!实现_->占用.insert(E).second) return false;
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
                    实现_->占用持有者[E]={static_cast<std::uint_least32_t>(__LINE__),std::this_thread::get_id()};
#endif
                }
                实现::调用结束 release{*实现_, E};
                std::vector<需求特征迁移缺口> gaps;
                const auto state = 实现_->迁移(C, E, gaps, true);
                if (state != 服务对象需求发布状态::已读取 && state != 服务对象需求发布状态::旧需求特征待迁移)
                    return false;
                if (state == 服务对象需求发布状态::旧需求特征待迁移)
                    for (const auto& 缺口 : gaps) 记录迁移缺口(C, E, 缺口);
                // 不可还原材料原样保留，已在初始化具名报告；后续正式读取仍返回完整缺口。
            }
        }
        std::lock_guard lock(实现_->互斥); 实现_->就绪 = true; return true;
    } catch (...) { 发布故障(L"初始化需求发布服务失败"); }
}
void 服务对象需求发布服务::关闭() noexcept {
    try {
        if (!实现_) return;
        std::unique_lock lock(实现_->互斥); 实现_->就绪 = false;
        实现_->等待.wait(lock, [&] { return 实现_->占用.empty()&&实现_->共享占用.empty(); });
    } catch (...) { 发布故障(L"关闭需求发布服务失败"); }
}
服务对象需求读取结果 服务对象需求发布服务::读取主体需求(稳定编码 C, 稳定编码 E) const noexcept {
    try {
        const auto s = 实现_->取得(E);
        if (s != 服务对象需求发布状态::已读取) { 服务对象需求读取结果 r; r.状态 = s; return r; }
        实现::调用结束 end{*实现_, E}; return 实现_->读取(C, E);
    } catch (...) { 发布故障(L"读取主体需求工程异常"); }
}
新本能已发布需求核验结果 服务对象需求发布服务::核验已发布需求结果(
    const 新本能已发布需求结果& result) const noexcept {
    using S=新本能已发布需求核验状态;
    try {
        const auto acquired=实现_->取得(result.主体存在节点,true);
        if(acquired==服务对象需求发布状态::已关闭)return {S::已关闭,{},{}};
        if(acquired==服务对象需求发布状态::前提已变化)return {};
        if(acquired!=服务对象需求发布状态::已读取)发布故障(L"需求结果核验调用占用状态异常");
        实现::调用结束 release{*实现_,result.主体存在节点,true};
        return 实现_->核验已发布结果材料(result);
    } catch(const 需求结果读取变化&) {return {};}
    catch(const std::bad_alloc&) {return {S::资源失败,{},{}};}
    catch(...) {发布故障(L"需求结果只读核验工程异常");}
}
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
#ifndef HY_前提诊断
#define HY_前提诊断(文件,行) do{try{(void)调试日志<调试切片::SIMULATION_PROGRESS>(L"前提已变化返回",文件,L"行="+std::to_wstring(行));}catch(...){}}while(0)
#endif
#else
#ifndef HY_前提诊断
#define HY_前提诊断(文件,行) ((void)0)
#endif
#endif
新需求目标定义核验结果 服务对象需求发布服务::实现::核验内容定义(const 新需求完整目标& 完整目标) const {
    using S=新需求目标定义核验状态;
    const auto& t=完整目标.目标;
    if(!t.引用内容定位||!有效(完整目标.目标存在节点)||!有效(t.引用内容定位->引用需求节点)
        ||t.目标形式!=新需求目标形式::值||!t.目标值域概念节点||!有效(*t.目标值域概念节点)
        ||t.目标比较关系概念节点||t.目标变化方向||t.目标特征类型概念节点!=类型)return {};
    if(t.引用内容定位->正式内容路径.size()>1)return {S::未支持,{}};
    std::vector<基础联合节点快照> 保护;
    // 实时合并：同一节点后读取者取代先读取者，不因内容变化作废（2026-11-09用户裁决）。
    const auto 实时合并=[&](const std::vector<基础联合节点快照>& 组){
        for(const auto& 项:组){
            const auto 旧=std::find_if(保护.begin(),保护.end(),[&](const auto& x){return x.结构.节点.编码==项.结构.节点.编码;});
            if(旧==保护.end())保护.push_back(项);else *旧=项;
        }
    };
    const auto 主体=事实.存在服务.读取存在身份保护(完整目标.目标存在节点);
    if(主体.前提已变化){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
    if(主体.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(主体.状态==基础结构操作状态::入口拒绝||主体.状态==基础结构操作状态::节点不存在)return {};
    if(主体.状态!=基础结构操作状态::已完成||!主体.信息)发布故障(L"内容目标主体身份读取工程失败");
    实时合并(主体.预期节点组);
    const auto FT=事实.特征概念服务.读取特征概念定义保护(类型);
    if(FT.前提已变化){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
    if(FT.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(FT.状态==基础结构操作状态::入口拒绝||FT.状态==基础结构操作状态::节点不存在)return {};
    if(FT.状态!=基础结构操作状态::已完成||!FT.信息)发布故障(L"内容目标需求FT定义读取工程失败");
    if(FT.信息->类型根!=类型||FT.信息->定义.材料类型!=特征概念材料物理类型::需求节点引用)return {};
    实时合并(FT.预期节点组);
    const auto D=需求.读取被认知需求直接内容保护(t.引用内容定位->引用需求节点,完整目标.目标存在节点,{});
    if(D.状态==新需求内容读取状态::依赖未就绪)return {S::依赖未就绪,{}};
    if(D.状态==新需求内容读取状态::前提已变化){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
    if(D.状态==新需求内容读取状态::资源失败)throw std::bad_alloc{};
    if(D.状态==新需求内容读取状态::需求不存在||D.状态==新需求内容读取状态::主体不符
        ||D.状态==新需求内容读取状态::非被认知需求||D.状态==新需求内容读取状态::输入不合法)return {};
    if(D.状态!=新需求内容读取状态::已读取||!D.信息)发布故障(L"内容目标D身份读取工程失败");
    实时合并(D.完整保护组);
    const auto C=*t.目标值域概念节点;
    const auto 定义=事实.存在概念服务.读取正式结构概念保护(C);
    if(定义.状态==正式结构操作状态::依赖未就绪)return {S::依赖未就绪,{}};
    if(定义.状态==正式结构操作状态::前提已变化){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
    if(定义.状态==正式结构操作状态::未找到||定义.状态==正式结构操作状态::输入拒绝)return {};
    if(定义.状态!=正式结构操作状态::已读取||!定义.信息)发布故障(L"内容目标范围正式定义读取工程失败");
    if(std::holds_alternative<结构约束定义>(定义.信息->定义))return {};
    实时合并(定义.完整保护组);
    if(t.引用内容定位->正式内容路径.empty()) {
        const auto 根=事实.存在概念服务.读取先天需求任务方法概念();
        if(根.状态==正式结构操作状态::依赖未就绪||根.状态==正式结构操作状态::未找到)return {S::依赖未就绪,{}};
        if(根.状态==正式结构操作状态::前提已变化){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
        if(根.状态!=正式结构操作状态::已读取||!根.信息)发布故障(L"先天需求根定义读取工程失败");
        std::set<稳定编码> 已读;std::vector<稳定编码> 待读{C};bool 命中=false;
        while(!待读.empty()) {
            const auto n=待读.back();待读.pop_back();if(!已读.insert(n).second)continue;
            // 到达先天需求根即命中，不再向其上位展开：根之上的概念不是信息结构概念，读取为“未找到”，
            // 原实现把它当作前提已变化，使引用内容需求的定义核验恒失败。
            if(n==根.信息->需求){命中=true;continue;}
            const auto r=事实.存在概念服务.读取正式结构概念保护(n);
            if(r.状态==正式结构操作状态::前提已变化){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
            if(r.状态==正式结构操作状态::依赖未就绪)return {S::依赖未就绪,{}};
            if(r.状态==正式结构操作状态::未找到){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
            if(r.状态!=正式结构操作状态::已读取||!r.信息)发布故障(L"内容目标上位结构读取工程失败");
            实时合并(r.完整保护组);
            const auto* d=std::get_if<信息结构概念定义>(&r.信息->定义);if(!d)return {};
            if(n==根.信息->需求)命中=true;
            待读.insert(待读.end(),d->上位结构概念组.begin(),d->上位结构概念组.end());
        }
        if(!命中)return {};
    } else {
        const auto 路径FT=t.引用内容定位->正式内容路径.front();if(!有效(路径FT))return {};
        const auto 读=需求.读取被认知需求直接内容保护(t.引用内容定位->引用需求节点,完整目标.目标存在节点,{路径FT});
        if(读.状态==新需求内容读取状态::内容项未支持)return {S::未支持,{}};
        if(读.状态==新需求内容读取状态::依赖未就绪)return {S::依赖未就绪,{}};
        if(读.状态==新需求内容读取状态::前提已变化){HY_前提诊断(L"服务对象需求.发布服务.cpp",__LINE__);return {S::前提已变化,{}};}
        if(读.状态==新需求内容读取状态::资源失败)throw std::bad_alloc{};
        if(读.状态!=新需求内容读取状态::已读取)return {};
        实时合并(读.完整保护组);
    }
    // 快照保护核对已删除（2026-11-09用户裁决，以实时读取为准）。
    return {S::已完成,std::move(保护)};
}
新需求目标定义核验结果 服务对象需求发布服务::核验引用内容目标定义保护(const 新需求完整目标& t) const noexcept {
    using S=新需求目标定义核验状态;
    try {
        const auto s=实现_->取得(t.目标存在节点,true);
        if(s==服务对象需求发布状态::已关闭)return {S::依赖未就绪,{}};
        if(s==服务对象需求发布状态::前提已变化)return {S::前提已变化,{}};
        实现::调用结束 结束{*实现_,t.目标存在节点,true};return 实现_->核验内容定义(t);
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(...){发布故障(L"引用内容目标定义核验工程错误");}
}
新引用内容目标判定 服务对象需求发布服务::求值引用内容目标(const 新需求完整目标& t) const noexcept {
    return 求值内容内部(t,true);
}
新引用内容目标判定 服务对象需求发布服务::求值指定需求内容(const 新需求完整目标& t) const noexcept {
    return 求值内容内部(t,false);
}
struct 服务对象需求发布服务::整体目标关联读取结果 final {
    enum class 状态类型 : std::uint8_t {
        已读取=1, 依赖未就绪, 未支持, 前提已变化, 资源失败
    };
    状态类型 状态=状态类型::依赖未就绪;
    bool 已关联=false;
    std::optional<稳定编码> 特征节点;
    std::vector<基础联合节点快照> 完整保护组;
};
服务对象需求发布服务::整体目标关联读取结果 服务对象需求发布服务::读取整体目标关联(
    稳定编码 场景节点,稳定编码 主体节点,稳定编码 指定需求节点,
    const std::vector<基础联合节点快照>& 初始保护) const {
    using S=整体目标关联读取结果::状态类型;
    auto 保护=初始保护;
    核需求结果断言(std::any_of(保护.begin(),保护.end(),[&](const auto& 项){
        return 项.结构.节点.编码==主体节点;
    }),保护,L"整体需求目标缺少主体完整保护");
    const auto 主体=实现_->事实.存在服务.获取存在(主体节点);
    核需求结果断言(主体&&主体->节点==主体节点,保护,L"整体需求目标主体不可读");
    const auto 根=实现_->世界.获取世界树根场景();
    核需求结果断言(根.has_value(),保护,L"整体需求目标缺少世界根");

    // 父链只使用场景owner的公开DTO；完整保护也固定主体不持有该类型的负事实。
    std::vector<新场景信息> 场景链;
    std::set<稳定编码> 已读场景;
    auto 当前场景=场景节点;
    for(;;) {
        核需求结果断言(已读场景.insert(当前场景).second,保护,L"整体需求目标场景父链循环");
        捕获需求结果节点(保护,当前场景);
        const auto 场景=实现_->事实.场景服务.获取场景(当前场景);
        核需求结果断言(场景&&场景->节点==当前场景,保护,L"整体需求目标场景不可读");
        场景链.push_back(*场景);
        if(当前场景==*根) {
            核需求结果断言(!场景->父场景节点,保护,L"整体需求目标世界根有父场景");
            break;
        }
        核需求结果断言(场景->父场景节点.has_value(),保护,L"整体需求目标场景不属于本世界");
        当前场景=*场景->父场景节点;
    }
    const auto 直接所属=实现_->事实.场景服务.查询所属场景(主体节点);
    核需求结果断言(主体节点==场景节点||直接所属==std::optional{场景节点},保护,
        L"整体需求目标主体不在指定场景");
    const auto 特征=实现_->找特征(主体节点,实现_->类型,&保护);
    bool 旧型阻挡=false;
    bool 已关联=false;
    std::optional<新特征信息> 特征信息;
    if(!特征) {
        旧型阻挡=实现_->找特征(主体节点,实现_->旧类型,&保护).has_value();
    } else {
        特征信息=实现_->事实.特征服务.获取特征(*特征);
        核需求结果断言(特征信息&&特征信息->节点==*特征&&!特征信息->上级特征节点
            &&std::find(主体->特征节点组.begin(),主体->特征节点组.end(),*特征)!=主体->特征节点组.end(),
            保护,L"整体需求目标特征归属或身份不符");
        const auto 定义=实现_->事实.特征概念服务.读取特征概念定义保护(特征信息->特征概念节点);
        if(定义.状态==基础结构操作状态::资源失败)return {S::资源失败,false,{},{}};
        if(定义.前提已变化)return {S::前提已变化,false,{},{}};
        if(!合并需求内容保护(保护,定义.预期节点组))return {S::前提已变化,false,{},{}};
        核需求结果断言(定义.状态==基础结构操作状态::已完成&&定义.信息
            &&定义.信息->类型根==实现_->类型
            &&定义.信息->定义.材料类型==特征概念材料物理类型::需求节点引用
            &&定义.信息->定义.比较规则==特征概念比较规则::需求身份精确
            &&!定义.信息->定义.单位&&!特征信息->实例单位,保护,L"整体需求目标特征类型定义不符");
        if(特征信息->当前值) {
            const auto* D=std::get_if<稳定编码>(&*特征信息->当前值);
            核需求结果断言(D&&有效(*D),保护,L"整体需求目标当前值不是需求节点引用");
            const auto 身份=实现_->需求.读取需求节点引用身份(*D);
            if(身份.状态==特征概念规则状态::读取接口未装配)return {S::依赖未就绪,false,{},{}};
            if(身份.状态==特征概念规则状态::资源失败)return {S::资源失败,false,{},{}};
            if(身份.状态==特征概念规则状态::前提已变化)return {S::前提已变化,false,{},{}};
            if(!合并需求内容保护(保护,身份.身份保护))return {S::前提已变化,false,{},{}};
            核需求结果断言(身份.状态==特征概念规则状态::已完成&&身份.身份
                &&!身份.身份保护.empty()&&身份.身份->需求节点==*D
                &&身份.身份->主体存在节点==主体节点&&身份.身份->被认知,
                保护,L"整体需求目标当前引用的需求身份、主体或执行域损坏");
            已关联=*D==指定需求节点;
        }
        // current为空允许历史保留；历史中的目标引用不能替代当前关联。
    }
    if(实现_->事实.存在服务.获取存在(主体节点)!=主体
        ||实现_->世界.获取世界树根场景()!=根
        ||实现_->事实.场景服务.查询所属场景(主体节点)!=直接所属)
        return {S::前提已变化,false,{},{}};
    for(const auto& 场景:场景链)
        if(实现_->事实.场景服务.获取场景(场景.节点)!=std::optional{场景})
            return {S::前提已变化,false,{},{}};
    if(实现_->找特征(主体节点,实现_->类型,&保护)!=特征)
        return {S::前提已变化,false,{},{}};
    if(!特征&&实现_->找特征(主体节点,实现_->旧类型,&保护).has_value()!=旧型阻挡)
        return {S::前提已变化,false,{},{}};
    if(特征&&实现_->事实.特征服务.获取特征(*特征)!=特征信息)
        return {S::前提已变化,false,{},{}};
    if(!需求内容联合仍相同(保护))return {S::前提已变化,false,{},{}};
    if(旧型阻挡)return {S::未支持,false,{},{}};
    return {S::已读取,已关联,特征,std::move(保护)};
}
新引用内容目标判定 服务对象需求发布服务::求值内容内部(
    const 新需求完整目标& t,bool 要求当前引用) const noexcept {
    using S=新引用内容目标状态;
    try {
        const auto 占用=实现_->取得(t.目标存在节点,true);
        if(占用==服务对象需求发布状态::已关闭)return {};
        if(占用==服务对象需求发布状态::前提已变化){HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}
        实现::调用结束 结束{*实现_,t.目标存在节点,true};
        auto 定义=实现_->核验内容定义(t);
        switch(定义.状态) {
        case 新需求目标定义核验状态::已完成:break;
        case 新需求目标定义核验状态::依赖未就绪:return {};
        case 新需求目标定义核验状态::未支持:return {S::未支持,{},{},{},{},{}};
        case 新需求目标定义核验状态::前提已变化:{HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}
        case 新需求目标定义核验状态::资源失败:return {S::资源失败,{},{},{},{},{}};
        case 新需求目标定义核验状态::输入不合法:return {S::定义不合法,{},{},{},{},{}};
        default:发布故障(L"引用内容目标定义端口返回内部不一致");
        }
        auto 保护=std::move(定义.完整保护);
        const auto E=t.目标存在节点;const auto& 定位=*t.目标.引用内容定位;const auto C=*t.目标.目标值域概念节点;
        const auto 场景=实现_->事实.场景服务.是场景节点(E) ? std::optional{E} : 实现_->事实.场景服务.查询所属场景(E);
        if(!场景){HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}
        std::optional<稳定编码> 实际特征;
        bool 已关联=true;
        if(要求当前引用&&定位.正式内容路径.empty()) {
            auto 关联=读取整体目标关联(*场景,E,定位.引用需求节点,保护);
            using A=整体目标关联读取结果::状态类型;
            switch(关联.状态) {
            case A::已读取:break;
            case A::依赖未就绪:return {};
            case A::未支持:return {S::未支持,{},{},{},{},{}};
            case A::前提已变化:{HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}
            case A::资源失败:return {S::资源失败,{},{},{},{},{}};
            default:发布故障(L"整体需求目标关联读取状态异常");
            }
            if(!合并需求内容保护(保护,关联.完整保护组)){HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}
            实际特征=关联.特征节点;已关联=关联.已关联;
        // CONV-INSTINCT 6.3／5170 §2.2：路径非空的内容目标（补问叶子及按字段的方法内容条件）
        // 不以H需求特征当前引用D为前提：待完善D不进当前引用，字段是否已有确定值只由D owner内容判定。
        } else {
            // 只固定主体的真实场景，不以需求特征及其current作为内容判断前提。
            if(!实现_->场景有效(*场景,E)){HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}
            捕获需求结果节点(保护,*场景);
        }
        // 同一主体占用内只消费D owner纯内容判定，不回入publisher。
        const auto 内容=实现_->需求.判定被认知需求内容保护(
            定位.引用需求节点,E,C,定位.正式内容路径);
        if(内容.状态==S::资源失败||内容.状态==S::引用前提已变化)
            {HY_前提诊断(L"内容求值D判定",__LINE__);return {内容.状态,{},{},{},{},{}};}
        if(!合并需求内容保护(保护,内容.完整保护组)||!需求内容联合仍相同(保护))
            {HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}
        新引用内容目标判定 输出;
        输出.场景节点=*场景;输出.实际特征节点=实际特征;
        输出.状态=内容.状态;
        输出.必要项目组=内容.必要项目组;输出.分支组=内容.分支组;
        if(输出.状态==S::已满足&&!已关联)输出.状态=S::未满足;
        输出.完整保护组=std::move(保护);return 输出;
    }catch(const 需求结果读取变化&){{HY_前提诊断(L"内容求值",__LINE__);return {S::引用前提已变化,{},{},{},{},{}};}}
    catch(const std::bad_alloc&){return {S::资源失败,{},{},{},{},{}};}
    catch(...){发布故障(L"引用内容目标实际求值工程错误");}
}
服务对象需求内容读取结果 服务对象需求发布服务::实现::读取当前结构(
    稳定编码 C,稳定编码 E,稳定编码 结构概念,std::optional<稳定编码> 预期D) const {
    using S=服务对象需求内容读取状态;
    std::vector<基础联合节点快照> 保护;
    auto& 概念服务=事实.存在概念服务;
    const auto 先天组=概念服务.读取先天需求任务方法概念();
    if(先天组.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
    if(先天组.状态==正式结构操作状态::依赖未就绪||先天组.状态==正式结构操作状态::未找到)
        return {S::未就绪,{},{}};
    if(先天组.状态!=正式结构操作状态::已读取||!先天组.信息)
        发布故障(L"当前需求内容的先天需求根读取异常");
    auto 结构=概念服务.读取正式结构有效内容保护(结构概念);
    switch(结构.状态) {
    case 正式结构有效内容读取状态::未找到:case 正式结构有效内容读取状态::非信息结构:
        return {S::结构定义不适用,{},{}};
    case 正式结构有效内容读取状态::依赖未就绪:return {S::未就绪,{},{}};
    case 正式结构有效内容读取状态::前提已变化:return {S::前提已变化,{},{}};
    case 正式结构有效内容读取状态::资源失败:return {S::资源失败,{},{}};
    case 正式结构有效内容读取状态::已读取:break;
    default:发布故障(L"当前需求有效结构返回异常状态");
    }
    if(!结构.信息||结构.信息->概念节点!=结构概念||结构.完整保护组.empty())
        发布故障(L"当前需求有效结构载荷不完整");
    if(!合并需求内容保护(保护,结构.完整保护组))return {S::前提已变化,{},{}};
    std::set<稳定编码> 已读;
    std::vector<稳定编码> 待读{结构概念};
    bool 需求系=false;
    while(!待读.empty()) {
        const auto 节点=待读.back();待读.pop_back();
        if(!已读.insert(节点).second)continue;
        const auto 定义=概念服务.读取正式结构概念保护(节点);
        if(定义.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(定义.状态==正式结构操作状态::依赖未就绪)return {S::未就绪,{},{}};
        if(定义.状态!=正式结构操作状态::已读取||!定义.信息) {
            if(!需求内容联合仍相同(保护))return {S::前提已变化,{},{}};
            发布故障(L"已核验结构的上位定义不可读");
        }
        if(!合并需求内容保护(保护,定义.完整保护组))return {S::前提已变化,{},{}};
        const auto* 内容=std::get_if<信息结构概念定义>(&定义.信息->定义);
        if(!内容)发布故障(L"已核验结构的上位不是信息结构");
        if(节点==先天组.信息->需求)需求系=true;
        待读.insert(待读.end(),内容->上位结构概念组.begin(),内容->上位结构概念组.end());
    }
    if(!需求系)return {S::结构定义不适用,{},{}};
    std::vector<稳定编码> FT组;
    for(const auto& 项:结构.信息->内容项组) {
        if(!项.读取路径.empty())return {S::内容项未支持,{},{}};
        FT组.push_back(项.特征类型);
    }
    if(!场景有效(C,E))return {S::主体不在场景,{},{}};
    const auto F=找特征(E,类型);
    if(!F) {
        if(预期D)return {S::前提已变化,{},{}};
        if(找特征(E,旧类型))return {S::旧需求特征待迁移,{},{}};
        return {S::需求特征未取得,{},{}};
    }
    const auto 依据=世界.读取需求引用更新依据(C,E,*F);
    if(依据.状态==新世界需求引用组合状态::前提已变化)return {S::前提已变化,{},{}};
    if(依据.状态!=新世界需求引用组合状态::已读取||!依据.依据) {
        if(!场景有效(C,E)||找特征(E,类型)!=F)return {S::前提已变化,{},{}};
        发布故障(L"已定位需求特征的正式当前引用依据不可读");
    }
    const auto& 基础=*依据.依据;
    if(基础.类型根!=类型)发布故障(L"需求内容引用类型根与服务配置不符");
    if(!合并需求内容保护(保护,基础.定位保护组)||!合并需求内容保护(保护,{基础.特征保护}))
        return {S::前提已变化,{},{}};
    if(预期D&&基础.原需求节点!=预期D)return {S::前提已变化,{},{}};
    if(!基础.原需求节点)return {S::当前需求未取得,{},{}};
    const auto D=*基础.原需求节点;
    // 固定本次D0；后续即使current变化也不改选另一条历史需求。
    const auto 当前D=全局基础数据集.查询节点组完整结构({D});
    if(当前D.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(当前D.状态!=基础结构操作状态::已完成||当前D.节点组.size()!=1||当前D.节点组.front().查询节点!=D)
        发布故障(L"固定当前需求的联合身份捕获异常");
    if(!当前D.节点组.front().快照)return {S::前提已变化,{},{}};
    if(!合并需求内容保护(保护,{*当前D.节点组.front().快照}))return {S::前提已变化,{},{}};
    auto 内容=需求.读取被认知需求直接内容保护(D,E,FT组);
    switch(内容.状态) {
    case 新需求内容读取状态::已读取:break;
    case 新需求内容读取状态::内容项未支持:return {S::内容项未支持,{},{}};
    case 新需求内容读取状态::依赖未就绪:return {S::未就绪,{},{}};
    case 新需求内容读取状态::前提已变化:return {S::前提已变化,{},{}};
    case 新需求内容读取状态::资源失败:return {S::资源失败,{},{}};
    case 新需求内容读取状态::需求不存在:case 新需求内容读取状态::主体不符:case 新需求内容读取状态::非被认知需求:
        if(!需求内容联合仍相同(保护))return {S::前提已变化,{},{}};
        发布故障(L"当前需求引用的正式身份、主体或执行域损坏");
    default:发布故障(L"需求owner直接内容返回异常状态");
    }
    if(!内容.信息||内容.信息->需求节点!=D||内容.信息->主体存在节点!=E||内容.完整保护组.empty()
        ||内容.信息->内容项组.size()!=FT组.size())发布故障(L"需求owner直接内容成功载荷不完整");
    for(std::size_t i=0;i<FT组.size();++i)if(内容.信息->内容项组[i].特征类型!=FT组[i])
        发布故障(L"需求owner直接内容顺序或FT身份不符");
    if(!合并需求内容保护(保护,内容.完整保护组))return {S::前提已变化,{},{}};
    const auto 末尾组=概念服务.读取先天需求任务方法概念();
    if(末尾组.状态==正式结构操作状态::前提已变化||末尾组.信息!=先天组.信息)return {S::前提已变化,{},{}};
    if(末尾组.状态!=正式结构操作状态::已读取)发布故障(L"末尾先天需求根读取异常");
    if(!需求内容联合仍相同(保护))return {S::前提已变化,{},{}};
    服务对象当前需求结构内容 输出{C,E,*F,D,std::move(*结构.信息),std::move(*内容.信息)};
    return {S::已读取,std::move(输出),std::move(保护)};
}
服务对象需求内容读取结果 服务对象需求发布服务::读取当前需求结构内容(
    稳定编码 C,稳定编码 E,稳定编码 结构概念,std::optional<稳定编码> 预期D) const noexcept {
    using S=服务对象需求内容读取状态;
    try {
        if(!有效(C)||!有效(E)||!有效(结构概念)||(预期D&&!有效(*预期D)))return {};
        const auto 状态=实现_->取得(E,true);
        if(状态==服务对象需求发布状态::已关闭)return {S::未就绪,{},{}};
        if(状态==服务对象需求发布状态::前提已变化)return {S::前提已变化,{},{}};
        if(状态!=服务对象需求发布状态::已读取)发布故障(L"当前需求内容调用占用状态异常");
        实现::调用结束 结束{*实现_,E,true};
        return 实现_->读取当前结构(C,E,结构概念,预期D);
    }catch(const std::bad_alloc&) {return {S::资源失败,{},{}};}
    catch(const std::exception&) {发布故障(L"当前需求结构内容读取工程异常");}
    catch(...) {发布故障(L"当前需求结构内容读取未知工程异常");}
}
服务对象需求发生读取结果 服务对象需求发布服务::实现::读取发生(稳定编码 S) const {
    using R=服务对象需求发生读取状态;
    const auto 状态=事实.状态服务.获取状态(S);
    if(!状态) {
        if(需求.读取主体需求发生关联按状态(S).关联||需求.是需求迁移恢复状态(S))
            发布故障(L"正式需求发生或恢复引用的状态缺失或损坏");
        if(!有效(S)||!全局基础数据集.查询节点(S))return {R::状态未找到,{}};
        if(事实.状态服务.是状态节点(S))发布故障(L"正式需求发生状态结构不完整");
        return {R::非需求状态,{}};
    }
    const auto 定义=事实.特征概念服务.读取特征概念定义保护(状态->特征类型概念节点);
    if(定义.状态!=基础结构操作状态::已完成||!定义.信息)发布故障(L"需求发生状态的特征概念不可读");
    const auto* D=(状态->固定准确值 ? std::get_if<稳定编码>(&*状态->固定准确值) : nullptr);
    if(定义.信息->类型根!=类型||!D||!有效(*D))return {R::非需求状态,{}};
    const auto 记录=需求.读取主体需求发生关联按状态(S);
    if(需求.是需求迁移恢复状态(S))return {R::迁移恢复状态,{}};
    std::optional<新需求发生关联> 关联=记录.关联;
    if(!关联) {
        const auto 首次=需求.读取主体需求特征发布关联(*D);
        if(首次.状态!=新需求操作状态::已找到)发布故障(L"需求发生首来源读取异常");
        if(!首次.关联)return {R::发生来源未取得,{}};
        const auto& 原=*首次.关联;
        const auto A=事实.动态服务.获取动态(原.动作节点);
        if(!A||A->种类!=新动态种类::动作||!A->动作来源节点)发布故障(L"需求发生首动作结构损坏");
        bool 精确=原.首状态节点==std::optional{S};
        if(!精确&&A->动作结果组.size()==1&&A->动作结果组.front().类型==新动作结果关系类型::导致变化) {
            const auto 变化=事实.动态服务.获取动态(A->动作结果组.front().结果动态节点);
            if(!变化)发布故障(L"需求发生首动作结果悬空");
            精确=变化->种类==新动态种类::原子&&变化->状态节点组.size()==2&&变化->状态节点组.back()==S;
        }
        if(!精确)return {R::发生来源未取得,{}};
        关联=新需求发生关联{{原.场景节点,原.主体存在节点,状态->来源实例特征节点,*D,原.动作节点,新需求发生方式::新需求},*A->动作来源节点,S};
    }
    const auto& p=关联->定位;
    const auto d=需求.读取需求治理信息(p.需求节点);
    const auto a=事实.动态服务.获取动态(p.动作节点);
    if(关联->状态节点!=S||p.需求节点!=*D||p.主体存在节点!=状态->被描述存在节点
        ||p.特征节点!=状态->来源实例特征节点||!d||d->需求主体存在节点!=p.主体存在节点
        ||d->执行域!=新需求执行域::被认知||!a||a->种类!=新动态种类::动作
        ||a->动作来源节点!=std::optional{关联->动作来源节点}
        ||a->动作目标存在节点!=std::optional{p.主体存在节点}
        ||a->动作目标特征节点!=std::optional{p.特征节点})发布故障(L"需求发生关联、状态或实际动作不一致");
    return {R::已读取,服务对象需求发生投影{*关联,*状态}};
}
服务对象需求发生读取结果 服务对象需求发布服务::读取需求发生状态(稳定编码 S) const noexcept {
    try {return 实现_->读取发生(S);}
    catch(...){发布故障(L"读取需求发生状态工程异常");}
}
服务对象需求发布结果 服务对象需求发布服务::实现::发布(const 服务对象需求发布请求& q,新需求发生方式 方式) const {
    try {
        using S = 服务对象需求发布状态;
        服务对象需求发布结果 out; out.状态 = 取得(q.服务对象节点);
        if (out.状态 != S::已读取) return out;
        调用结束 end{*this, q.服务对象节点};
        if(!有效(q.动作来源节点)){out.状态=S::来源不符;return out;}
        需求发生来源占用 来源占用{q.动作来源节点};
        if(!来源占用.已取得){out.状态=S::前提已变化;return out;}
        const auto 完成来源=需求.读取主体需求发生关联按来源(q.动作来源节点);
        const auto 完成动作=q.既有动作节点 ? 需求.读取主体需求发生关联按动作(*q.既有动作节点) : 新需求发生关联读取结果{};
        if(完成来源.关联||完成动作.关联) {
            const auto& 完成=完成来源.关联 ? *完成来源.关联 : *完成动作.关联;
            const auto& p=完成.定位;
            const auto a=事实.动态服务.获取动态(p.动作节点);
            if((完成来源.关联&&完成动作.关联&&*完成来源.关联!=*完成动作.关联)
                ||p.场景节点!=q.场景节点||p.主体存在节点!=q.服务对象节点||p.需求节点!=q.需求节点
                ||p.方式!=方式||完成.动作来源节点!=q.动作来源节点
                ||(q.既有动作节点&&p.动作节点!=*q.既有动作节点)||!a
                ||a->动作定义节点!=std::optional{q.动作定义节点}||a->主题存在节点!=q.执行主体节点) {
                out.状态=S::来源不符;return out;
            }
            const auto 发生=读取发生(完成.状态节点);
            if(发生.状态!=服务对象需求发生读取状态::已读取||!发生.发生)发布故障(L"已完成需求发生读回失败");
            out.状态=S::已复用;out.需求特征节点=p.特征节点;out.需求节点=p.需求节点;out.动作节点=p.动作节点;out.需求发生=发生.发生;
            return out;
        }
        const auto d = 需求.读取需求治理信息(q.需求节点);
        if (!d) { out.状态 = S::需求不存在; return out; }
        if (d->执行域 != 新需求执行域::被认知) { out.状态 = S::非被认知需求; return out; }
        if (d->需求主体存在节点 != q.服务对象节点) { out.状态 = S::主体不符; return out; }
        if(d->信息完善阶段==新需求信息完善阶段::待完善) {
            out.状态=S::需求信息待完善;return out;
        } else {
            const auto full=需求.获取需求(q.需求节点);
            if(!full){out.状态=S::前提已变化;return out;}
            out.状态=确认(*full);if(out.状态!=S::已读取)return out;
        }
        if (!场景有效(q.场景节点, q.服务对象节点)) { out.状态 = S::来源不符; return out; }
        out.状态 = 迁移(q.场景节点, q.服务对象节点, out.迁移缺口组, true);
        if (out.状态 != S::已读取) return out;
        auto read = 读取(q.场景节点, q.服务对象节点);
        out.迁移缺口组 = read.迁移缺口组;
        if (read.状态 != S::已读取) { out.状态 = read.状态; return out; }
        const auto F = read.需求特征节点.value_or(稳定编码{});
        const auto target = 有效(F) ? F : 建空特征(q.服务对象节点);
        out.需求特征节点 = target; out.需求节点 = q.需求节点;
        const auto origin = 需求.读取主体需求特征发布关联(q.需求节点);
        // 与本函数准备来源的既有口径一致：前提已变化原样返回，目标内容依赖未就绪按需求信息待完善等待。
        if (origin.状态 == 新需求操作状态::前提已变化) { out.状态 = S::前提已变化; return out; }
        if (origin.状态 == 新需求操作状态::目标内容依赖未就绪) { out.状态 = S::需求信息待完善; return out; }
        if (origin.状态 != 新需求操作状态::已找到) 发布故障(L"需求发布来源读取失败");
        const bool present = std::binary_search(read.历史需求节点组.begin(), read.历史需求节点组.end(), q.需求节点);
        if (present != origin.关联.has_value()) 发布故障(L"需求历史与发布来源不一致");
        if(需求.读取需求治理信息(q.需求节点)!=d){out.状态=S::前提已变化;return out;}
        if (present&&方式==新需求发生方式::新需求) {
            out.状态=S::已复用;out.动作节点=origin.关联->动作节点;
            const auto 首次发生=需求.读取主体需求发生关联按动作(*out.动作节点);
            std::optional<稳定编码> 旧S=首次发生.关联 ? std::optional{首次发生.关联->状态节点} : origin.关联->首状态节点;
            if(!旧S) {
                const auto a=事实.动态服务.获取动态(*out.动作节点);
                if(!a)发布故障(L"普通历史需求首次动作悬空");
                if(a->动作结果组.size()==1&&a->动作结果组.front().类型==新动作结果关系类型::导致变化) {
                    const auto 变化=事实.动态服务.获取动态(a->动作结果组.front().结果动态节点);
                    if(!变化)发布故障(L"普通历史需求首次变化悬空");
                    if(变化->种类==新动态种类::原子&&变化->状态节点组.size()==2)旧S=变化->状态节点组.back();
                }
            }
            if(旧S) {
                const auto 发生=读取发生(*旧S);
                if(发生.状态==服务对象需求发生读取状态::已读取)out.需求发生=发生.发生;
            }
            return out;
        }
        if(方式==新需求发生方式::已满足再次需要) {
            if(!present){out.状态=S::来源不符;return out;}
            if(d->活动状态!=新需求活动状态::已满足){out.状态=S::需求未满足;return out;}
        }
        const auto basis = 世界.读取需求引用更新依据(q.场景节点, q.服务对象节点, target);
        if (basis.状态 == 新世界需求引用组合状态::前提已变化) { out.状态 = S::前提已变化; return out; }
        if (basis.状态 != 新世界需求引用组合状态::已读取 || !basis.依据) 发布故障(L"需求发布依据不可读");
        auto prepared = 世界.准备需求引用改变({*basis.依据, q.需求节点, q.动作定义节点, q.动作来源节点, q.执行主体节点, q.既有动作节点,方式});
        out.动作节点 = prepared.已发生动作节点;
        if (prepared.状态 == 新世界需求引用组合状态::前提已变化) { out.状态 = S::前提已变化; return out; }
        if (prepared.状态 == 新世界需求引用组合状态::目标不合法) { out.状态 = S::来源不符; return out; }
        if (prepared.状态 != 新世界需求引用组合状态::已准备 || !prepared.准备 || !out.动作节点) 发布故障(L"需求发布准备失败");
        const auto material = 世界.读取需求引用组合准备(*prepared.准备);
        if(需求.读取需求治理信息(q.需求节点)!=d){out.状态=S::前提已变化;return out;}
        auto source = 方式==新需求发生方式::已满足再次需要
            ?需求.准备主体需求再次需要(q.需求节点,q.服务对象节点,target,q.场景节点,*out.动作节点,material.特征保护)
            :需求.准备主体需求特征发布(q.需求节点, q.服务对象节点, target, q.场景节点, *out.动作节点, material.特征保护);
        if(source.状态==新需求操作状态::需求未满足){out.状态=S::需求未满足;return out;}
        if(source.状态==新需求操作状态::目标内容依赖未就绪){out.状态=S::需求信息待完善;return out;}
        if (source.状态 == 新需求操作状态::需重新确认) { out.状态 = S::需重新确认; return out; }
        if (source.状态 == 新需求操作状态::前提已变化 || source.状态 == 新需求操作状态::已复用) { out.状态 = S::前提已变化; return out; }
        if (source.状态 != 新需求操作状态::已建立 || !source.准备) 发布故障(L"需求发布来源准备失败");
        const auto published = 世界.提交需求引用改变并形成状态动态(*prepared.准备, *source.准备);
        if (published.状态 == 新世界需求引用组合状态::前提已变化) { out.状态 = S::前提已变化; return out; }
        if (published.状态 != 新世界需求引用组合状态::已完成 || !published.变化 || !published.附加片段回执 || !published.变化->新状态节点) 发布故障(L"需求联合发布未完成");
        out.变化 = published.变化;
        if(方式==新需求发生方式::已满足再次需要)需求.完成主体需求再次需要(*source.准备,*published.附加片段回执);
        else 需求.完成主体需求特征发布(*source.准备, *published.附加片段回执);
        if (方式==新需求发生方式::新需求&&!published.变化->前状态节点) {
            新需求特征事实核验输入 input{新需求特征事实用途::首次状态, q.场景节点, q.服务对象节点, {}, target,
                q.需求节点, out.动作节点, published.变化->新状态节点, {}};
            auto proof = 世界.核验需求特征发布事实(input);
            if (proof.状态 != 新需求特征事实核验状态::已核验 || !proof.凭据) 发布故障(L"需求首状态实际回执核验失败");
            const auto status = 需求.完成主体需求首状态关联(q.需求节点, *proof.凭据);
            if (status != 新需求操作状态::已建立 && status != 新需求操作状态::已复用) 发布故障(L"需求首状态来源关联失败");
        }
        const auto a=事实.动态服务.获取动态(*out.动作节点);
        if(!a||a->动作来源节点!=std::optional{q.动作来源节点})发布故障(L"需求发生实际动作来源丢失");
        const auto S节点=*published.变化->新状态节点;
        if(事实.状态服务.增加引用(S节点)!=新状态操作状态::已完成)发布故障(L"需求发生状态引用保留失败");
        const 新需求发生关联 关联{{q.场景节点,q.服务对象节点,target,q.需求节点,*out.动作节点,方式},*a->动作来源节点,S节点};
        const auto 关联状态=需求.记录主体需求发生关联(关联);
        // 已发布的状态/动态保留；引用并发变化按前提已变化返回，幂等已复用视同成功。
        if(关联状态==新需求操作状态::前提已变化){out.状态=S::前提已变化;return out;}
        if(关联状态!=新需求操作状态::已建立&&关联状态!=新需求操作状态::已复用)发布故障(L"需求发生关联追加失败");
        const auto 发生=读取发生(S节点);
        if(发生.状态!=服务对象需求发生读取状态::已读取||!发生.发生)发布故障(L"需求发生发表后读回失败");
        out.需求发生=发生.发生;out.本次新建发生=true;
        const auto final = 读取(q.场景节点, q.服务对象节点);
        // 已发布事实保留；读回遇并发变化或依赖未就绪（已关闭）均不能确认当前值。
        if (final.状态 == S::前提已变化 || final.状态 == S::已关闭) { out.状态 = S::已发布但当前读回变化; return out; }
        if (final.状态 != S::已读取 || final.需求特征节点 != target
            || !std::binary_search(final.历史需求节点组.begin(), final.历史需求节点组.end(), q.需求节点)) 发布故障(L"需求发布最终来源和历史读回失败");
        out.状态 = final.当前需求节点 != q.需求节点
            || std::find(final.需重新确认节点组.begin(), final.需重新确认节点组.end(), q.需求节点) != final.需重新确认节点组.end()
            ? S::已发布但当前读回变化 : S::已发布;
        return out;
    } catch (...) { 发布故障(L"发布已确认需求工程异常"); }
}
服务对象需求发布结果 服务对象需求发布服务::发布已确认需求(const 服务对象需求发布请求& q) noexcept {
    return 实现_->发布(q,新需求发生方式::新需求);
}
服务对象需求发布结果 服务对象需求发布服务::发布待完善需求(const 服务对象需求发布请求& q) noexcept {
    return 实现_->发布(q,新需求发生方式::新需求);
}
服务对象需求发布结果 服务对象需求发布服务::明确再次需要已满足需求(const 服务对象需求发布请求& q) noexcept {
    return 实现_->发布(q,新需求发生方式::已满足再次需要);
}
}
