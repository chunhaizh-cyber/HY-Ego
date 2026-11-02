#include "字段询问.本能运行服务.h"
#include "正式表达需求.h"
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
#include <algorithm>
#include <array>
#include <condition_variable>
#include <map>
#include <mutex>
#include <stdexcept>
#include <set>
#include <utility>

namespace 海中鱼巣 {
namespace {
void 要求(bool ok,const char* text) { if (!ok) throw std::logic_error(text); }
thread_local const 方法参数场景* 发送工作场景=nullptr;
thread_local const 新字段询问候选* 发送工作候选=nullptr;
thread_local const std::function<bool()>* 发送工作来源=nullptr;
thread_local const std::vector<基础联合节点快照>* 发送工作来源保护=nullptr;
}
字段询问发送工作守卫::字段询问发送工作守卫(const 方法参数场景& s,
    const 新字段询问候选& c,const std::function<bool()>& guard,
    const std::vector<基础联合节点快照>& protect) {
    要求(!发送工作场景 && guard,"field send work context nested or missing source guard");
    发送工作场景=&s;发送工作候选=&c;发送工作来源=&guard;
    要求(!protect.empty(),"field send source protection missing");发送工作来源保护=&protect;
}
字段询问发送工作守卫::~字段询问发送工作守卫() {
    发送工作场景=nullptr;发送工作候选=nullptr;发送工作来源=nullptr;
    发送工作来源保护=nullptr;
}
新本能函数本次条件结果对 执行本次任务字段发送(const 方法参数场景& scene) {
    要求(发送工作场景==&scene && 发送工作候选 && 发送工作来源,"field send must consume adopted task work context");
    auto* reader=scene.获取字段询问运行接口();
    auto* sender=dynamic_cast<新字段询问发送运行接口*>(reader);
    要求(reader && sender,"field send same-owner runtime unavailable");
    return 执行指定需求字段发送动作核心(scene,*reader,*sender,*发送工作候选,*发送工作来源);
}
struct 字段询问本能运行服务::实现 final {
    实现(新_世界树类& w,新_外设世界树类& p,新_方法类& m,新_需求类& d,
        新_存在类& e,新_特征类& f,新_动态类& a,概念_存在类& ec,概念_特征类& fc,
        std::shared_ptr<const 新字段询问语言接口> l) noexcept
        : 世界(w),外设(p),方法(m),需求(d),存在(e),特征(f),动态(a),存在概念(ec),特征概念(fc),语言(std::move(l)) {}
    新_世界树类& 世界;新_外设世界树类& 外设;新_方法类& 方法;新_需求类& 需求;
    新_存在类& 存在;新_特征类& 特征;新_动态类& 动态;概念_存在类& 存在概念;概念_特征类& 特征概念;
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
    概念_存在类& ec,概念_特征类& fc,std::shared_ptr<const 新字段询问语言接口> l) noexcept
    :实现_(std::make_unique<实现>(w,p,m,d,e,f,a,ec,fc,std::move(l))) {}
字段询问本能运行服务::~字段询问本能运行服务()=default;
bool 字段询问本能运行服务::初始化(const 新字段询问登记配置& c) noexcept {
    try {
        auto& s=*实现_;std::lock_guard lock(s.锁);
        要求(!s.已关闭 && s.语言,"field question initialization dependency unavailable");
        if (s.配置) { 要求(*s.配置==c,"field question immutable configuration mismatch");return true; }
        for (const auto id:{c.需求根概念,c.自我范围概念,c.人类概念,c.通用存在概念,c.输出队列类型,
            c.语言节点,c.第三方指代概念,c.第三方别名关系,c.需求主体类型}) 要求(有效(id),"field question configuration identity missing");
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
        要求(q.条件结果组.empty(),"field question registration must not create learned pairs");
        return s.方法.确保本能方法登记(std::string(新本能函数登记名_询问指定需求字段),q);
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"字段询问登记",L"字段询问方法登记错误。"); }
}
新方法建立结果 字段询问本能运行服务::读取或确保发送方法登记() noexcept {
    try {
        auto& s=*实现_;实现::租约 lease(s);const auto& c=*s.配置;
        新方法知识建立请求 q;q.作用域=新方法作用域::普通世界;
        q.实现方式=新方法实现方式::本能;q.行为=新方法行为::改变;
        q.方法头.作用存在概念组={c.通用存在概念};
        q.方法头.涉及特征类型概念组={c.输出队列类型};q.方法头.可能结果概念组={c.输出队列类型};
        const std::array<新方法有序参数规格,5> specs{{
            {1,新方法参数信息类型::存在,c.需求根概念},{2,新方法参数信息类型::存在,c.自我范围概念},
            {3,新方法参数信息类型::存在,c.人类概念},{4,新方法参数信息类型::存在,c.通用存在概念},
            {5,新方法参数信息类型::特征,c.输出队列类型}}};
        for(const auto& spec:specs){新方法参数规格 p;p.有序规格=spec;q.参数规格组.push_back(std::move(p));}
        q.初始模拟关系组={{新方法参数关系种类::不同,2,3},{新方法参数关系种类::特征所属存在,5,4}};
        新方法初始必要条件定义 necessary;necessary.条件.关系规则=新方法项关系规则::全部成立;
        新方法条件项定义 output;output.参数序号=4;output.特征类型概念节点=c.输出队列类型;
        necessary.条件.条件项组.push_back(std::move(output));q.初始必要条件=std::move(necessary);
        return s.方法.确保本能方法登记(std::string(新本能函数登记名_发送指定需求字段),q);
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"登记独立字段发送方法",L"发送方法登记工程错误。");}
}
新字段询问材料读回 字段询问本能运行服务::读取询问材料(const 方法参数场景& scene) const {
    auto& s=*实现_;实现::租约 lease(s);
    auto out=读取通用询问材料(scene,*s.配置,s.需求);
    if (!out.材料) return out;
    const auto& m=*out.材料;
    const auto w=s.外设.读取对话框来源(m.窗口);
    const auto f=scene.读取绑定特征(m.输出特征);
    const auto e=s.存在.获取存在(m.窗口);
    if (!w || w->定位.场景节点!=m.外设场景 || w->定位.输出队列特征!=m.输出特征
        || !f || !e || std::count(e->特征节点组.begin(),e->特征节点组.end(),m.输出特征)!=1)
        return {新字段询问材料状态::前提已变化,{}};
    return out;
}
新字段询问文字结果 字段询问本能运行服务::生成询问文字(const 新正式字段询问目标& target) const {
    auto& s=*实现_;实现::租约 lease(s);return s.语言->生成字段询问(target,s.需求);
}
bool 字段询问本能运行服务::已请求停止() const noexcept {
    std::lock_guard lock(实现_->锁);return 实现_->已关闭 || 新_本能函数管理类::本次停止已请求();
}
bool 字段询问本能运行服务::复核发送会话(稳定编码 窗口,std::string_view 宿主,std::string_view 对话框) const {
    auto& s=*实现_;实现::租约 lease(s);
    const auto w=s.外设.读取对话框来源(窗口);
    return w && w->公开宿主会话==宿主 && w->对话框标识==对话框;
}
bool 字段询问本能运行服务::原子发送合同已接通() const noexcept {
    std::lock_guard lock(实现_->锁);
    return 实现_->配置.has_value() && 实现_->语言 && !实现_->已关闭;
}
bool 字段询问本能运行服务::准备发送动作定义(const 方法参数场景& scene) {
    auto& s=*实现_;实现::租约 lease(s);
    const auto read=读取询问材料(scene);
    if(read.状态!=新字段询问材料状态::已取得 || !read.材料)return false;
    const auto& m=*read.材料;
    const auto actor=s.存在.获取存在(m.自我),target=s.存在.获取存在(m.窗口);
    const auto feature=s.特征.获取特征(m.输出特征);
    if(!actor || !target || !feature)return false;
    const auto root=s.特征概念.比较单特征概念(feature->特征概念节点,feature->特征概念节点);
    要求(root.状态==特征概念规则状态::已完成 && root.特征类型根.has_value(),
        "field send action definition feature root unreadable");
    const auto result=s.动态.确保动作定义概念({scene.方法节点(),actor->专属存在概念节点,
        target->专属存在概念节点,root.特征类型根,false});
    要求(result.概念节点.has_value(),"field send action definition unavailable");
    return true;
}
std::optional<新字段询问原子发送读回> 字段询问本能运行服务::原子提交询问输出(
    const 方法参数场景& scene,const 新字段询问材料& m,const 新字段询问候选& c,
    const std::function<bool()>& sourceGuard) {
    auto& s=*实现_;实现::租约 lease(s);
    要求(c.话语.状态==新字段询问文字状态::已生成且唯一回读 && !c.话语.完整保护组.empty(),
        "atomic question send requires generated/readback complete protection");
    auto source=新_本能函数管理类::读取本次执行实例(scene);
    if(!source)source=新_本能函数管理类::读取本次直接调用(scene);
    const bool own=!source.has_value();
    if(own)source=s.方法.建立直接调用身份(scene.方法节点(),scene.全部角色绑定(),m.执行场景,m.外设场景).节点;
    const auto w=s.外设.读取对话框来源(m.窗口);
    if(!w || w->定位.场景节点!=m.外设场景 || w->定位.输出队列特征!=m.输出特征
        || !sourceGuard || 已请求停止() || !sourceGuard()) {
        if(own)s.方法.收束直接调用身份(*source);
        return {};
    }
    const auto guard=[&] { return !已请求停止() && sourceGuard(); };
    auto protect=c.话语.完整保护组;
    要求(发送工作来源保护 && !发送工作来源保护->empty(),"atomic question source read set missing");
    for(const auto& item:*发送工作来源保护) {
        const auto it=std::find_if(protect.begin(),protect.end(),[&](const auto& x){
            return x.结构.节点.编码==item.结构.节点.编码;});
        if(it==protect.end()){protect.push_back(item);continue;}
        if(it->结构.节点!=item.结构.节点 || it->结构.字段组!=item.结构.字段组
            || it->入向字段组!=item.入向字段组 || it->入向外部关系组!=item.入向外部关系组
            || it->出向外部关系组!=item.出向外部关系组) {
            if(own)s.方法.收束直接调用身份(*source);
            return {};
        }
    }
    const auto r=s.外设.原子提交发送({w->定位,m.执行场景,m.自我,scene.方法节点(),*source,
        c.话语.UTF8正文,std::move(protect)},guard);
    if(r.输出.状态==新外设输出队列状态::前提已变化) {
        要求(!r.动作节点 && !r.输出.消息,"atomic send rejected with published facts");
        if(own)s.方法.收束直接调用身份(*source);
        return {};
    }
    要求(r.输出.状态==新外设输出队列状态::已入列 && r.动作节点 && r.输出.消息
        && r.输出.入列状态 && r.输出.入列动态,"atomic question send receipt incomplete");
    if(own || 新_本能函数管理类::读取本次直接调用(scene))
        s.方法.绑定直接调用动作(*source,*r.动作节点);
    if(own)s.方法.收束直接调用身份(*source);
    return 新字段询问原子发送读回{*r.动作节点,{r.输出.消息->消息节点,*r.输出.入列状态,
        *r.输出.入列动态,r.输出.消息->来源.来源序号}};
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
    auto* p=&操作;
    const auto capture=[&] {
        r.条件.参数角色组.clear();
        for (const auto& b:场景.全部角色绑定()) {
            新本能函数条件角色材料 c;c.有序引用=b.有序引用;c.世界场景节点=b.世界场景节点;
            r.条件.参数角色组.push_back(std::move(c));
        }
    };
        const auto material=p->读取询问材料(场景);
        if (material.材料) capture();
        auto publishRead=[&](const 新字段询问材料读回& read) {
            新本能字段询问返回材料 v;
            if (read.材料) { v.正式定位=read.材料->定位; v.已读取字段=read.材料->字段内容; }
            switch (read.状态) {
            case 新字段询问材料状态::字段已有值:v.原因=新本能字段询问原因::字段已有值;break;
            case 新字段询问材料状态::字段不适用:v.原因=新本能字段询问原因::字段不适用;break;
            case 新字段询问材料状态::前提已变化:v.原因=新本能字段询问原因::前提已变化;break;
            default:v.原因=新本能字段询问原因::定位未支持;break;
            }
            r.结果.字段询问=std::move(v);
            if (read.状态==新字段询问材料状态::字段已有值 || read.状态==新字段询问材料状态::字段不适用) {
                r.结果.状态=新本能函数执行状态::已按合同返回;
                r.结果.结果取得状态=新本能函数结果取得状态::已取得;
            }
        };
        if (material.状态!=新字段询问材料状态::已取得) { publishRead(material); return r; }
        if (!material.材料) throw std::logic_error("field question material missing");
        const auto& m=*material.材料;
        // 值式表达请求由正式适配器形成候选；不建立输出动作。
        auto target=m.表达上下文;
        target.被询内容=m.定位;
        target.说话者=m.自我; target.受询者=m.受询者;
        target.交互窗口=m.窗口; target.交互作用域=m.外设场景;
        const auto text=p->提交表达需求(target);
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
        新本能字段询问返回材料 out;
        out.原因=新本能字段询问原因::已取得候选;
        out.正式定位=m.定位;out.已读取字段=m.字段内容;
        out.表达候选=装配字段询问候选(从字段询问表达需求形成(target),text);
        r.结果.字段询问=std::move(out);
        r.结果.状态=新本能函数执行状态::已按合同返回;
        r.结果.结果取得状态=新本能函数结果取得状态::已取得;
        return r;
}

新本能函数本次条件结果对 执行指定需求字段发送动作核心(
    const 方法参数场景& 场景,新本能字段询问运行接口& 读取,
    新字段询问发送运行接口& 发送,const 新字段询问候选& 候选,
    const std::function<bool()>& 复核来源) {
    新本能函数本次条件结果对 r;
    const auto 不发送=[&](新本能字段询问原因 原因) {
        新本能字段询问返回材料 v;v.原因=原因;v.正式定位=候选.表达目标.被询内容;
        r.结果.字段询问=std::move(v);return r;
    };
    if(!发送.原子发送合同已接通())return 不发送(新本能字段询问原因::发送合同待对接);
    if(!复核来源 || 读取.已请求停止() || !复核来源())return 不发送(新本能字段询问原因::前提已变化);
    if(候选.话语.状态!=新字段询问文字状态::已生成且唯一回读)
        return 不发送(新本能字段询问原因::缺语言知识);
    const auto material=读取.读取询问材料(场景);
    if(material.状态!=新字段询问材料状态::已取得)
        return 不发送(material.状态==新字段询问材料状态::字段已有值 ? 新本能字段询问原因::字段已有值
            : material.状态==新字段询问材料状态::字段不适用 ? 新本能字段询问原因::字段不适用
            : 新本能字段询问原因::前提已变化);
    if(!material.材料)throw std::logic_error("send decision material missing");
    const auto& m=*material.材料;
    const auto text=读取.生成询问文字(m.表达上下文);
    const auto 当前=装配字段询问候选(从字段询问目标形成表达需求(m.表达上下文),
        text,候选.正式表达需求节点);
    if(!(当前==候选))return 不发送(新本能字段询问原因::前提已变化);
    const auto fresh=读取.读取询问材料(场景);
    if(fresh.状态!=新字段询问材料状态::已取得||!fresh.材料)
        return 不发送(新本能字段询问原因::前提已变化);
    const auto& f=*fresh.材料;
    const auto 末次=装配字段询问候选(从字段询问目标形成表达需求(f.表达上下文),
        text,当前.正式表达需求节点);
    if(!(末次==当前)||f.输出特征!=m.输出特征||f.执行场景!=m.执行场景
        ||读取.已请求停止() || !复核来源())return 不发送(新本能字段询问原因::前提已变化);
    for(const auto& b:场景.全部角色绑定()) {
        新本能函数条件角色材料 c;c.有序引用=b.有序引用;c.世界场景节点=b.世界场景节点;
        if(b.有序引用&&b.有序引用->序号==5) {
            const auto feature=场景.读取绑定特征(b.有序引用->信息节点);
            if(!feature)return 不发送(新本能字段询问原因::前提已变化);
            c.实际特征组.push_back({feature->节点,feature->当前值});
        }
        r.条件.参数角色组.push_back(std::move(c));
    }
    auto* atomic=dynamic_cast<字段询问本能运行服务*>(&发送);
    if(!atomic)throw std::logic_error("atomic field sender owner mismatch");
    const auto published=atomic->原子提交询问输出(场景,f,候选,复核来源);
    if(!published)return 不发送(新本能字段询问原因::前提已变化);
    const auto action=published->动作节点;
    r.结果.动作动态节点=action;r.结果.动作阶段=新本能函数动作阶段::已进入动作;
    const auto& receipt=published->入列;
    新本能字段询问返回材料 out;out.原因=新本能字段询问原因::已入列;
    out.正式定位=f.定位;out.已读取字段=f.字段内容;out.表达候选=候选;
    out.已提交输出=新本能已提交输出结果{f.窗口,f.输出特征,receipt.消息节点,action,
        receipt.入列状态,receipt.入列动态,receipt.提交序号};
    r.结果.字段询问=std::move(out);
    r.结果.状态=新本能函数执行状态::已按合同返回;
    r.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;
    r.结果.结果取得状态=新本能函数结果取得状态::已取得;
    r.结果.已形成状态节点组={receipt.入列状态};r.结果.已形成动态节点组={receipt.入列动态};
    r.结果.建议观察特征节点组={f.输出特征};
    return r;
}

} // namespace 海中鱼巣
