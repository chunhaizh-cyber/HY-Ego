#include "字段询问.本能运行服务.h"
#include "../领域/新_语言类.h"
#include "../领域/新_自然语言世界树类.h"
#include "新_世界树类.h"
#include "新_外设世界树类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_需求类.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/新_存在类.h"
#include "../领域/新_特征类.h"
#include "../领域/新_动态类.h"
#include "../领域/概念_存在类.h"
#include "../领域/概念_特征类.h"
#include "../适配/适配器.UTC时钟.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"
#include <algorithm>
#include <condition_variable>
#include <map>
#include <mutex>
#include <stdexcept>
#include <set>
#include <utility>

namespace 海中鱼巣 {
extern thread_local int 字段询问材料诊断行;
thread_local int 字段询问运行核对行=0;
namespace {
void 要求(bool ok,const char* text) { if (!ok) throw std::logic_error(text); }
}
struct 字段询问本能运行服务::实现 final {
    实现(新_世界树类& w,新_外设世界树类& p,新_方法类& m,新_需求类& d,
        新_存在类& e,新_特征类& f,新_动态类& a,概念_存在类& ec,概念_特征类& fc,
        新_语言类& owner,新_自然语言世界树类& world,std::shared_ptr<const 新字段询问语言接口> l) noexcept
        : 世界(w),外设(p),方法(m),需求(d),存在(e),特征(f),动态(a),存在概念(ec),特征概念(fc),语言owner(owner),语言世界(world),语言(std::move(l)) {}
    新_世界树类& 世界;新_外设世界树类& 外设;新_方法类& 方法;新_需求类& 需求;
    新_存在类& 存在;新_特征类& 特征;新_动态类& 动态;概念_存在类& 存在概念;概念_特征类& 特征概念;
    新_语言类& 语言owner;新_自然语言世界树类& 语言世界;
    std::shared_ptr<const 新字段询问语言接口> 语言;
    mutable std::mutex 锁;mutable std::condition_variable 空闲;
    mutable std::size_t 活动=0;bool 已关闭=false;
    std::optional<新字段询问登记配置> 配置;
    std::map<稳定编码,稳定编码> 自建调用;
    struct 租约 final {
        const 实现& s;
        explicit 租约(const 实现& v):s(v) { std::lock_guard lock(s.锁);要求(!s.已关闭 && s.配置.has_value(),"field question runtime closed");++s.活动; }
        ~租约() { std::lock_guard lock(s.锁);--s.活动;s.空闲.notify_all(); }
    };
};
字段询问本能运行服务::字段询问本能运行服务(新_世界树类& w,新_外设世界树类& p,
    新_方法类& m,新_需求类& d,新_存在类& e,新_特征类& f,新_动态类& a,
    概念_存在类& ec,概念_特征类& fc,新_语言类& owner,新_自然语言世界树类& world,
    std::shared_ptr<const 新字段询问语言接口> l) noexcept
    :实现_(std::make_unique<实现>(w,p,m,d,e,f,a,ec,fc,owner,world,std::move(l))) {}
字段询问本能运行服务::~字段询问本能运行服务()=default;
bool 字段询问本能运行服务::初始化(const 新字段询问登记配置& c) noexcept {
    try {
        auto& s=*实现_;std::lock_guard lock(s.锁);
        要求(!s.已关闭 && s.语言,"field question initialization dependency unavailable");
        if (s.配置) { 要求(*s.配置==c,"field question immutable configuration mismatch");return true; }
        for (const auto id:{c.需求根概念,c.自我范围概念,c.交互者特征类型,c.通用存在概念,c.输出队列类型,c.外设场景标识类型,
            c.语言节点,c.第三方指代概念,c.第三方别名关系,c.需求主体类型,c.需求目标存在类型,c.需求目标特征类型,c.补问需求概念})
            要求(有效(id),"field question configuration identity missing");
        要求(c.支持字段类型组.size()==6,"field question supported slot count invalid");
        std::set<稳定编码> unique;
        for (const auto id:c.支持字段类型组)
            要求(有效(id) && unique.insert(id).second,"field question supported slot identity invalid or repeated");
        s.配置=c;return true;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"字段询问初始化",L"字段询问初始化工程错误。"); }
}
void 字段询问本能运行服务::关闭() noexcept {
    try { auto& s=*实现_;std::unique_lock lock(s.锁);s.已关闭=true;s.空闲.wait(lock,[&]{return s.活动==0;}); }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"字段询问关闭",L"字段询问关闭工程错误。"); }
}
std::optional<新字段询问登记配置> 字段询问本能运行服务::读取登记材料() const noexcept {
    try { auto& s=*实现_;std::lock_guard lock(s.锁);return s.已关闭 ? std::nullopt : s.配置; }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"字段询问登记材料",L"字段询问登记材料读取错误。"); }
}
新方法建立结果 字段询问本能运行服务::确保方法登记(const 新方法知识建立请求& q) noexcept {
    try {
        auto& s=*实现_;实现::租约 lease(s);
        // 先天被动结果配对已撤回（2026-11-10用户裁决 5380 v1.4）：只登记方法头、规格与必要条件，结果从实际运行学得。
        要求(q.条件结果组.empty(),"field question registration must not preset condition-result pairs");
        return s.方法.确保本能方法登记(std::string(新本能函数登记名_询问指定需求字段),q);
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"字段询问登记",L"字段询问方法登记错误。"); }
}
namespace {
新字段询问答案来源 映射答案来源(新问题答案来源 v) {
    switch (v) {
    case 新问题答案来源::消息转换: return 新字段询问答案来源::消息转换;
    case 新问题答案来源::自我写入: return 新字段询问答案来源::自我写入;
    default: throw std::logic_error("question answer source undefined");
    }
}
}
新字段询问答案读回 字段询问本能运行服务::读取执行问题答案(稳定编码 出站来源) const noexcept {
    try {
        auto& s=*实现_;
        { std::lock_guard lock(s.锁); if (s.已关闭 || !s.配置) return {}; }
        实现::租约 lease(s);
        const auto q=s.语言owner.按出站来源读取问题(出站来源);
        if (!q) return {};
        if (q->答案) return {新字段询问答案状态::已答,q->问题节点,映射答案来源(q->答案->来源),false};
        const auto found=s.语言owner.查找问题答案(q->问题节点);
        return {新字段询问答案状态::未答,q->问题节点,std::nullopt,!found.答案组.empty()};
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取执行问题答案",L"问题答案读取工程错误。"); }
}
新字段询问答案读回 字段询问本能运行服务::取得执行问题答案(稳定编码 出站来源) noexcept {
    try {
        auto& s=*实现_;
        { std::lock_guard lock(s.锁); if (s.已关闭 || !s.配置) return {}; }
        实现::租约 lease(s);
        const auto q=s.语言owner.按出站来源读取问题(出站来源);
        if (!q) return {};
        if (q->答案) return {新字段询问答案状态::已答,q->问题节点,映射答案来源(q->答案->来源),false};
        const auto found=s.语言owner.查找问题答案(q->问题节点);
        if (found.答案组.empty()) return {新字段询问答案状态::未答,q->问题节点,std::nullopt,false};
        // 7240 §5.5 第4项：按查得来源登记（多条均符合时由父任务判断是否采用，此处登记第一条）。
        const auto& first=found.答案组.front();
        const auto reg=s.语言owner.登记问题答案(q->问题节点,first.答案信息,first.来源);
        switch (reg.状态) {
        case 新问题答案登记状态::已登记:
        case 新问题答案登记状态::精确重复:
        case 新问题答案登记状态::已有其它答案:
            要求(reg.当前答案.has_value(),"registered question answer missing");
            return {新字段询问答案状态::已答,q->问题节点,映射答案来源(reg.当前答案->来源),false};
        case 新问题答案登记状态::前提已变化:
            // 并发写入使本次登记的读取前提失效：保持未答，由下一次就绪通知或复核重读。
            return {新字段询问答案状态::未答,q->问题节点,std::nullopt,true};
        default:
            throw std::logic_error("question answer found by structure match was rejected");
        }
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"取得执行问题答案",L"问题答案登记工程错误。"); }
}
新字段询问材料读回 字段询问本能运行服务::读取询问材料(const 方法参数场景& scene) const {
    auto& s=*实现_;实现::租约 lease(s);
    // 方案乙：窗口副本经外设场景标识对应外设对话框；副本须是该外设场景在现实世界的唯一副本。
    const auto 对应=[&](稳定编码 外设场景,稳定编码 副本)->std::optional<std::pair<稳定编码,稳定编码>> {
        const auto 现实=s.世界.查找现实交互场景(外设场景,s.配置->外设场景标识类型);
        if (现实.状态!=新场景操作状态::无变化 || 现实.存在节点!=std::optional{副本}) { 字段询问运行核对行=__LINE__; return std::nullopt; }
        const auto 外设=s.外设.按场景读取交互场景(外设场景);
        if (!外设 || !外设->来源.定位.输出队列特征) { 字段询问运行核对行=__LINE__; return std::nullopt; }
        return std::pair{外设->来源.定位.窗口节点,*外设->来源.定位.输出队列特征};
    };
    字段询问运行核对行=0;
    // §6.6：受话者＝自我当前交互者，直接读取自我的交互者特征当前值（不经参数窄读）。
    const auto 当前交互者=[&](稳定编码 自我)->std::optional<稳定编码> {
        const auto E=s.存在.获取存在(自我);
        if (!E) return std::nullopt;
        for (const auto f:E->特征节点组) {
            const auto F=s.特征.获取特征(f);
            if (!F) return std::nullopt;
            if (s.特征概念.比较单特征概念(F->特征概念节点,F->特征概念节点).特征类型根!=std::optional{s.配置->交互者特征类型}) continue;
            if (!F->当前值) return std::nullopt;
            const auto* v=std::get_if<稳定编码>(&*F->当前值);
            return v ? std::optional{*v} : std::nullopt;
        }
        return std::nullopt;
    };
    auto out=读取通用询问材料(scene,*s.配置,s.需求,对应,当前交互者);
    if (!out.材料) return out;
    auto& m=*out.材料;
    const auto w=s.外设.读取对话框来源(m.窗口);
    const auto f=s.特征.获取特征(m.输出特征);
    const auto e=s.存在.获取存在(m.窗口);
    if (!w || w->定位.场景节点!=m.外设场景 || w->定位.输出队列特征!=m.输出特征
        || !f || !e || std::count(e->特征节点组.begin(),e->特征节点组.end(),m.输出特征)!=1)
        { 字段询问运行核对行=__LINE__; return {新字段询问材料状态::前提已变化,{}}; }
    // 条件“自然语言世界中该问题尚无答案”（5380 §3.6，初始必要条件；现有条件项无法表达语言世界事实，
    // 按运行时复核）：同一被认知需求、主体与被问栏的问题已有答案（已登记或按结构比对可找到）→ 目标已满足，不问。
    if (out.状态==新字段询问材料状态::已取得) {
        要求(有效(m.问题主体) && 有效(m.被问栏),"field question asked structure missing");
        for (const auto& q:s.语言owner.查询同结构问题(m.定位.信息节点,m.问题主体,m.被问栏)) {
            if (q.答案 || !s.语言owner.查找问题答案(q.问题节点).答案组.empty()) {
                m.已答问题=q.问题节点;out.状态=新字段询问材料状态::问题已有答案;
                return out;
            }
        }
    }
    return out;
}
新字段询问文字结果 字段询问本能运行服务::生成询问文字(const 新正式字段询问目标& target) const {
    auto& s=*实现_;实现::租约 lease(s);return s.语言->生成字段询问(target,s.需求);
}
bool 字段询问本能运行服务::已请求停止() const noexcept {
    std::lock_guard lock(实现_->锁);return 实现_->已关闭 || 新_本能函数管理类::本次停止已请求();
}
稳定编码 字段询问本能运行服务::建立输出动作(const 方法参数场景& scene,const 新字段询问材料& m) {
    auto& s=*实现_;实现::租约 lease(s);
    auto source=新_本能函数管理类::读取本次执行实例(scene);
    if (!source) source=新_本能函数管理类::读取本次直接调用(scene);
    bool own=false;
    if (!source) { const auto call=s.方法.建立直接调用身份(scene.方法节点(),scene.全部角色绑定(),m.执行场景,m.外设场景);source=call.节点;own=true; }
    const auto time=UTC时钟适配器{}.读取当前UTC时间证据_v1({});要求(time.成功(),"field question UTC unavailable");
    const auto action=s.世界.建立动作动态(m.执行场景,{m.自我,scene.方法节点(),*source,m.窗口,m.输出特征,
        {新状态时间语义::实例绝对UTC,time.证据->UTC纳秒}});
    要求(action.成功(),"field question action publication failed");
    const auto read=s.动态.按来源查询动作(*source);
    要求(read.状态==新动态操作状态::已完成 && read.动作.has_value(),"field question action readback failed");
    const auto a=read.动作->节点;
    if (own || 新_本能函数管理类::读取本次直接调用(scene)) s.方法.绑定直接调用动作(*source,a);
    if (own) { std::lock_guard lock(s.锁);要求(s.自建调用.emplace(a,*source).second,"field question action source repeated"); }
    return a;
}
新出站表达留档状态 字段询问本能运行服务::留档出站具体表达(const 方法参数场景& scene,
    const 新字段询问材料& m,稳定编码 action,const 新字段询问文字结果& text) {
    auto& s=*实现_;实现::租约 lease(s);
    auto expected=新_本能函数管理类::读取本次执行实例(scene);
    if(!expected)expected=新_本能函数管理类::读取本次直接调用(scene);
    if(!expected){std::lock_guard lock(s.锁);const auto i=s.自建调用.find(action);if(i!=s.自建调用.end())expected=i->second;}
    const auto a=s.动态.获取动态(action);
    要求(expected&&a&&a->动作来源节点==expected&&a->主题存在节点==m.自我
        &&a->动作定义节点==scene.方法节点()&&a->动作目标存在节点==m.窗口&&a->动作目标特征节点==m.输出特征,
        "field question archive explicit action mismatch");
    要求(text.状态==新字段询问文字状态::已生成且唯一回读&&有效(text.所用组织知识节点),"field question archive meaning missing");
    // 使用动作中正式保存的来源，不能用动作节点充当来源。
    const auto 来源=*a->动作来源节点;
    // 留档前提变化时本次不入列：自建直接调用身份须在此收束，与提交成功路径一致，不留未收束身份。
    const auto 前提已变化=[&]{
        std::optional<稳定编码> own;
        { std::lock_guard lock(s.锁);const auto i=s.自建调用.find(action);if(i!=s.自建调用.end()){own=i->second;s.自建调用.erase(i);} }
        if(own)s.方法.收束直接调用身份(*own);
        return 新出站表达留档状态::前提已变化;
    };
    const auto 场景=s.语言世界.建立或取得出站来源场景(来源);
    if(场景.状态==新来源子场景状态::前提已变化)return 前提已变化();
    要求((场景.状态==新来源子场景状态::已建立||场景.状态==新来源子场景状态::已存在)&&场景.信息.has_value(),"outbound language scene publication failed");
    const auto 留档=s.语言owner.建立或取得出站具体表达(场景.信息->场景节点,text.含义网,text.所用组织知识节点,text.UTF8正文,来源);
    if(!留档)return 前提已变化();
    const auto back=s.语言owner.读取出站具体表达(场景.信息->场景节点);
    要求(back&&back->节点==留档->节点&&back->场景==场景.信息->场景节点&&back->来源==来源
        &&back->含义网==text.含义网&&back->所用组织知识==text.所用组织知识节点&&back->候选话语原文==text.UTF8正文,
        "field question archive readback mismatch");
    return 新出站表达留档状态::已留档;
}
新字段询问问题建立读回 字段询问本能运行服务::建立出站问题(const 方法参数场景& scene,
    const 新字段询问材料& m,稳定编码 action) {
    auto& s=*实现_;实现::租约 lease(s);
    auto expected=新_本能函数管理类::读取本次执行实例(scene);
    if(!expected)expected=新_本能函数管理类::读取本次直接调用(scene);
    if(!expected){std::lock_guard lock(s.锁);const auto i=s.自建调用.find(action);if(i!=s.自建调用.end())expected=i->second;}
    const auto a=s.动态.获取动态(action);
    要求(expected&&a&&a->动作来源节点==expected&&a->主题存在节点==m.自我&&a->动作定义节点==scene.方法节点(),
        "field question establishment explicit action mismatch");
    要求(有效(m.问题主体)&&有效(m.被问栏)&&有效(m.交互者)&&有效(m.外设场景),"field question asked structure missing");
    const auto 来源=*a->动作来源节点;
    const auto 前提已变化=[&]{
        std::optional<稳定编码> own;
        { std::lock_guard lock(s.锁);const auto i=s.自建调用.find(action);if(i!=s.自建调用.end()){own=i->second;s.自建调用.erase(i);} }
        if(own)s.方法.收束直接调用身份(*own);
        return 新字段询问问题建立读回{};
    };
    // 问题建在已留档出站具体表达所在的出站来源语言场景（7240 §5.5 第1项）。
    const auto 场景=s.语言世界.建立或取得出站来源场景(来源);
    if(场景.状态==新来源子场景状态::前提已变化)return 前提已变化();
    要求((场景.状态==新来源子场景状态::已建立||场景.状态==新来源子场景状态::已存在)&&场景.信息.has_value(),"outbound language scene publication failed");
    const auto time=UTC时钟适配器{}.读取当前UTC时间证据_v1({});要求(time.成功(),"field question UTC unavailable");
    // R120/R121：被问栏为需求根概念即目标栏问题（答案按需求信息比对），否则载荷栏问题（按值陈述比对）。
    const 新语言问题定义 def{场景.信息->场景节点,来源,m.外设场景,m.交互者,m.定位.信息节点,m.问题主体,m.被问栏,time.证据->UTC纳秒,
        m.被问栏==s.配置->需求根概念 ? 新问题被问种类::目标栏 : 新问题被问种类::载荷栏};
    const auto q=s.语言owner.建立或取得出站问题(def);
    switch(q.状态){
    case 新语言问题建立状态::前提已变化:return 前提已变化();
    case 新语言问题建立状态::已建立:case 新语言问题建立状态::已存在:break;
    default:throw std::logic_error("outbound question conflicts with archived expression or existing question");
    }
    要求(q.问题节点.has_value()&&有效(*q.问题节点),"outbound question node missing");
    const auto back=s.语言owner.读取问题(*q.问题节点);
    要求(back&&back->定义.出站来源==来源&&back->定义.被认知需求==def.被认知需求&&back->定义.主体==def.主体
        &&back->定义.被问栏==def.被问栏,"outbound question readback mismatch");
    return {新字段询问问题建立状态::已建立,*q.问题节点};
}
新字段询问入列读回 字段询问本能运行服务::提交询问输出(const 方法参数场景& scene,
    const 新字段询问材料& m,稳定编码 action,std::string_view text) {
    auto& s=*实现_;实现::租约 lease(s);
    auto expected=新_本能函数管理类::读取本次执行实例(scene);
    if (!expected) expected=新_本能函数管理类::读取本次直接调用(scene);
    if (!expected) {
        std::lock_guard lock(s.锁);
        const auto i=s.自建调用.find(action);
        if (i!=s.自建调用.end()) expected=i->second;
    }
    const auto a=s.动态.获取动态(action);
    要求(expected && a && a->动作来源节点==expected
        && a->主题存在节点==m.自我 && a->动作定义节点==scene.方法节点()
        && a->动作目标存在节点==m.窗口 && a->动作目标特征节点==m.输出特征,"field question explicit action mismatch");
    const auto w=s.外设.读取对话框来源(m.窗口);要求(w.has_value(),"field question window disappeared after action");
    const auto r=s.外设.提交输出({w->定位,std::string(text),action});
    要求(r.状态==新外设输出队列状态::已入列 && r.消息 && r.入列状态 && r.入列动态,"field question queue publication incomplete");
    std::optional<稳定编码> own;
    { std::lock_guard lock(s.锁);const auto i=s.自建调用.find(action);if(i!=s.自建调用.end()){own=i->second;s.自建调用.erase(i);} }
    if (own) s.方法.收束直接调用身份(*own);
    return {r.消息->消息节点,*r.入列状态,*r.入列动态,r.消息->来源.来源序号};
}
新本能函数本次条件结果对 执行指定需求字段询问核心(
    const 方法参数场景& 场景,新本能字段询问运行接口& 操作) {
    新本能函数本次条件结果对 r;
    // 实际使用的执行分支即参数场景所带分支（任务核对冻结采用分支）。
    r.条件.执行分支节点=场景.执行分支节点();
    auto* p=&操作;
    const auto capture=[&] {
        r.条件.参数角色组.clear();
        for (const auto& b:场景.全部角色绑定()) {
            新本能函数条件角色材料 c;c.有序引用=b.有序引用;c.世界场景节点=b.世界场景节点;
            if (b.有序引用 && b.有序引用->序号==4) {
                const auto feature=场景.读取绑定特征(b.有序引用->信息节点);
                if (!feature) throw std::logic_error("output feature disappeared before action");
                c.实际特征组.push_back({feature->节点,feature->当前值});
            }
            r.条件.参数角色组.push_back(std::move(c));
        }
    };
        const auto material=p->读取询问材料(场景);
        if (场景.用途()!=新方法参数场景用途::模拟)
            (void)记录运行日志(L"字段询问",L"读取询问材料",L"材料状态="+std::to_wstring(static_cast<int>(material.状态))
                +L" 通用读取行="+std::to_wstring(字段询问材料诊断行)+L" 运行核对行="+std::to_wstring(字段询问运行核对行)
                );
        if (material.材料) capture();
        auto publishRead=[&](const 新字段询问材料读回& read) {
            新本能字段询问返回材料 v;
            if (read.材料) { v.正式定位=read.材料->定位; v.已读取字段=read.材料->字段内容; }
            switch (read.状态) {
            case 新字段询问材料状态::字段已有值:v.原因=新本能字段询问原因::字段已有值;break;
            case 新字段询问材料状态::字段不适用:v.原因=新本能字段询问原因::字段不适用;break;
            case 新字段询问材料状态::前提已变化:v.原因=新本能字段询问原因::前提已变化;break;
            case 新字段询问材料状态::问题已有答案:v.原因=新本能字段询问原因::问题已有答案;
                if (read.材料) v.问题节点=read.材料->已答问题;
                break;
            default:v.原因=新本能字段询问原因::定位未支持;break;
            }
            r.结果.字段询问=std::move(v);
            // 目标判定（5380 §2.4、§3.6）：D该字段已有值或不适用、或自然语言世界中该问题已有答案 → 零动作返回
            // 目标已满足，不报“已取得”。只在动作前到达（动作后的中断只以前提已变化发布）。
            if (read.状态==新字段询问材料状态::字段已有值 || read.状态==新字段询问材料状态::字段不适用
                || read.状态==新字段询问材料状态::问题已有答案) {
                要求(r.结果.动作阶段==新本能函数动作阶段::未进入动作,"field question goal judgment after action");
                r.结果.状态=新本能函数执行状态::目标已满足;
                r.结果.结果取得状态=新本能函数结果取得状态::未取得;
            }
        };
        if (material.状态!=新字段询问材料状态::已取得) { publishRead(material); return r; }
        if (!material.材料) throw std::logic_error("field question material missing");
        const auto& m=*material.材料;
        // 最简表达需求只在本次调用内部派生，不新增正式D或任务。
        auto target=m.表达上下文;
        target.被询内容=m.定位;
        target.说话者=m.自我; target.受询者=m.交互者; // §6.6：受话者取自我当前交互者
        target.交互窗口=m.窗口; target.交互作用域=m.外设场景;
        const auto text=p->生成询问文字(target);
        if (场景.用途()!=新方法参数场景用途::模拟)
            (void)记录运行日志(L"字段询问",L"生成询问文字",L"文字状态="+std::to_wstring(static_cast<int>(text.状态)));
        if (text.状态!=新字段询问文字状态::已生成且唯一回读) {
            新本能字段询问返回材料 v; v.正式定位=m.定位;v.已读取字段=m.字段内容;
            switch (text.状态) {
            case 新字段询问文字状态::缺知识:v.原因=新本能字段询问原因::缺语言知识;break;
            case 新字段询问文字状态::缺名称:v.原因=新本能字段询问原因::缺名称;break;
            case 新字段询问文字状态::缺指称:v.原因=新本能字段询问原因::缺指称;break;
            case 新字段询问文字状态::多义:v.原因=新本能字段询问原因::多义;break;
            case 新字段询问文字状态::前提已变化:v.原因=新本能字段询问原因::前提已变化;break;
            default:v.原因=新本能字段询问原因::定位未支持;break;
            }
            r.结果.字段询问=std::move(v);return r;
        }
        if (text.UTF8正文.empty()) throw std::logic_error("generated field question is empty");
        const auto fresh=p->读取询问材料(场景);
        if (fresh.状态!=新字段询问材料状态::已取得) { publishRead(fresh);return r; }
        if (!fresh.材料) throw std::logic_error("field question recheck missing");
        const auto& f=*fresh.材料;
        if (f.定位.信息节点!=m.定位.信息节点 || f.定位.正式内容路径!=m.定位.正式内容路径
            || f.自我!=m.自我 || f.交互者!=m.交互者 || f.窗口!=m.窗口 || f.输出特征!=m.输出特征
            || f.问题主体!=m.问题主体 || f.被问栏!=m.被问栏) {
            publishRead({新字段询问材料状态::前提已变化,{}});return r;
        }
        if (p->已请求停止()) return r;
        capture();
        if (新_本能函数管理类::读取本次执行实例(场景))
            新_本能函数管理类::标记本次动作入口(场景);
        const auto action=p->建立输出动作(场景,f);
        if (!有效(action)) throw std::logic_error("field question action was not published");
        r.结果.动作动态节点=action;r.结果.动作阶段=新本能函数动作阶段::已进入动作;
        // 已进入动作后的中断（留档前提变化）报动作结果未知，不保留默认“入口拒绝”（5380 §2.5）。
        r.结果.状态=新本能函数执行状态::动作结果未知;
        r.结果.结果取得状态=新本能函数结果取得状态::部分取得;
        const auto archive=p->留档出站具体表达(场景,f,action,text);
        switch(archive){
        case 新出站表达留档状态::前提已变化:
            publishRead({新字段询问材料状态::前提已变化,{}});return r;
        case 新出站表达留档状态::已留档:
        case 新出站表达留档状态::模拟不留档:break;
        default:throw std::logic_error("field question archive status undefined");
        }
        // 7240 §5.5 运行1：语言owner在出站来源语言场景中建立“问题”（被问结构＋答案初无值），问题节点随结果返回。
        const auto question=p->建立出站问题(场景,f,action);
        if (question.状态==新字段询问问题建立状态::前提已变化) { publishRead({新字段询问材料状态::前提已变化,{}});return r; }
        if (question.状态!=新字段询问问题建立状态::已建立 || !有效(question.问题节点))
            throw std::logic_error("field question establishment status undefined");
        const auto receipt=p->提交询问输出(场景,f,action,text.UTF8正文);
        if (!有效(receipt.消息节点)||!有效(receipt.入列状态)||!有效(receipt.入列动态)||receipt.提交序号==0)
            throw std::logic_error("field question output receipt incomplete");
        新本能字段询问返回材料 out;out.原因=新本能字段询问原因::已入列;
        out.正式定位=f.定位;out.已读取字段=f.字段内容;
        out.已提交输出=新本能已提交输出结果{f.窗口,f.输出特征,receipt.消息节点,action,receipt.入列状态,receipt.入列动态,receipt.提交序号};
        out.问题节点=question.问题节点;
        r.结果.字段询问=std::move(out);
        // ASK-READ-SCENE v0.5 §3.2（5380 §3.6）：问话入列后进入“动作已发出、等待结果”（等待中）。
        // 结果（问题的“答案”由无值变有值）由任务按出站来源到自然语言世界按结构比对检测并登记。
        r.结果.状态=新本能函数执行状态::异步已受理;
        r.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;
        r.结果.结果取得状态=新本能函数结果取得状态::待取得;
        r.结果.已形成状态节点组={receipt.入列状态};r.结果.已形成动态节点组={receipt.入列动态};
        r.结果.建议观察特征节点组={f.输出特征};
        return r;
}

} // namespace 海中鱼巣
