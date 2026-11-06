#include "服务需求.本能运行服务.h"
#include "新_世界树类.h"
#include "服务对象需求.发布服务.h"
#include "本能需求.执行核心.h"
#include "../领域/本能需求.运行接口.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/新_任务类.h"
#include "../适配/适配器.UTC时钟.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"
#include <algorithm>
#include <chrono>
#include <map>
#include <mutex>
#include <set>
#include <condition_variable>
#include <new>
#include <stdexcept>

namespace 海中鱼巣 {
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
namespace {
struct 新需求启动观察项 { std::wstring 模块, 入口, 内容; };
thread_local std::vector<新需求启动观察项> 新需求启动观察缓存;
}
void 暂存新需求启动观察(std::wstring_view 模块, std::wstring_view 入口,
    std::wstring_view 内容) noexcept {
    try { 新需求启动观察缓存.push_back({std::wstring(模块), std::wstring(入口), std::wstring(内容)}); }
    catch (...) {} // 可选诊断丢失不改变原返回或恢复业务。
}
void 输出新需求启动观察() noexcept {
    try {
        auto items = std::move(新需求启动观察缓存);
        新需求启动观察缓存.clear();
        for (const auto& item : items)
            (void)记录调试日志(L"NEW_DEMAND_STARTUP", item.模块, item.入口, item.内容);
    } catch (...) {}
}
#endif
namespace {
struct 新需求材料变化 final {};
struct 新需求材料未齐 final {};
struct 新需求参数不合法 final {};
struct 新需求需重新确认 final {};
void 核对新需求资格状态(新被认知需求资格状态 state) {
    using S=新被认知需求资格状态;
    switch(state) {
    case S::已读取:return;
    case S::内容待完善:throw 新需求材料未齐{};
    case S::需重新确认:throw 新需求需重新确认{};
    case S::需求不存在:
    case S::前提已变化:throw 新需求材料变化{};
    case S::主体不符:
    case S::非被认知需求:
    case S::来源不符:throw 新需求参数不合法{};
    case S::资源失败:throw std::bad_alloc{};
    default:throw std::logic_error("recognized demand qualification dependency unavailable or invalid state");
    }
}
// 仅做人读诊断；默认关闭，不改变任何业务返回或owner调用。
class 新需求登记阶段观察 final {
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
    const wchar_t* 名称_;
    std::chrono::steady_clock::time_point 开始_;
    bool 已结束_ = false;
#endif
public:
    explicit 新需求登记阶段观察(const wchar_t* 名称) noexcept {
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
        名称_ = 名称; 开始_ = std::chrono::steady_clock::now();
        暂存新需求启动观察(L"服务对象新需求", 名称_, L"阶段=开始");
#else
        (void)名称;
#endif
    }
    void 结束(std::int64_t 返回值, bool 材料已取得 = true) noexcept {
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
        if (已结束_) return;
        const auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - 开始_).count();
        已结束_ = true;
        try {
            std::wostringstream text;
            text << L"阶段=结束；耗时毫秒=" << elapsed << L"；返回值=" << 返回值
                 << L"；材料已取得=" << 材料已取得;
            暂存新需求启动观察(L"服务对象新需求", 名称_, text.str());
        } catch (...) {}
#else
        (void)返回值; (void)材料已取得;
#endif
    }
    ~新需求登记阶段观察() {
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
        if (!已结束_) 结束(-1, false);
#endif
    }
};
bool 新需求保护相同(const 基础联合节点快照& a, const 基础联合节点快照& b) {
    return a.结构.节点 == b.结构.节点 && a.结构.字段组 == b.结构.字段组
        && a.入向字段组 == b.入向字段组 && a.入向外部关系组 == b.入向外部关系组
        && a.出向外部关系组 == b.出向外部关系组;
}
void 新需求要求(bool value) {
    if (!value) throw std::runtime_error("new service-object demand material inconsistent");
}
void 新需求合保护(std::vector<基础联合节点快照>& target,
    const std::vector<基础联合节点快照>& source) {
    for (const auto& item : source) {
        const auto it = std::find_if(target.begin(), target.end(), [&](const auto& old) {
            return old.结构.节点.编码 == item.结构.节点.编码;
        });
        if (it == target.end()) target.push_back(item);
        else if (!新需求保护相同(*it, item)) throw 新需求材料变化{};
    }
}
void 新需求加节点保护(std::vector<基础联合节点快照>& target,
    const std::vector<稳定编码>& nodes) {
    auto unique = nodes;
    std::sort(unique.begin(), unique.end());
    unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
    const auto read = 全局基础数据集.查询节点组完整结构(unique);
    新需求要求(read.状态 == 基础结构操作状态::已完成 && read.节点组.size() == unique.size());
    for (const auto& item : read.节点组) {
        if (!item.快照) throw 新需求材料变化{};
        新需求合保护(target, {*item.快照});
    }
}
bool 新需求保护仍在(const std::vector<基础联合节点快照>& source) {
    std::vector<基础联合节点快照> fresh = source;
    std::vector<稳定编码> nodes;
    for (const auto& item : source) nodes.push_back(item.结构.节点.编码);
    try { 新需求加节点保护(fresh, nodes); return true; }
    catch (const 新需求材料变化&) { return false; }
}

bool 新需求规格一致(const 新方法信息& m, const 新服务对象新需求配置& c,bool 要求启用=true) {
    if (m.本能函数登记名 != 新本能函数登记名_获取服务对象需求节点
        || m.作用域 != 新方法作用域::普通世界 || m.实现方式 != 新方法实现方式::本能
        || (要求启用&&m.启用状态 != 新方法启用状态::启用) || m.行为 != 新方法行为::改变
        || m.参数规格组.size() != 3
        || m.方法头.作用存在概念组 != std::vector<稳定编码>{c.人类概念节点}
        || m.方法头.涉及特征类型概念组 != std::vector<稳定编码>{c.对象需求类型}
        || m.方法头.可能结果概念组 != std::vector<稳定编码>{c.完整被认知需求概念}) return false;
    const auto 预期 = 服务对象新需求有序规格(c);
    for (std::uint32_t i = 0; i < 3; ++i) {
        const auto& p = m.参数规格组[i];
        if (p.有序规格 != std::optional{预期[i]}
            || p.参数角色节点.值 || p.存在概念节点 || p.要求场景 || !p.特征规格组.empty()
            || p.集合成员约束 || p.对象种类 != 新方法参数对象种类::普通世界) return false;
    }
    新方法初始必要条件定义 needed;
    needed.条件.关系规则 = 新方法项关系规则::全部成立;
    新方法条件项定义 human;
    human.参数序号 = 1; human.存在概念节点 = c.人类概念节点;
    needed.条件.条件项组.push_back(std::move(human));
    return m.初始模拟关系组.empty()
        && m.初始必要条件 == std::optional{needed} && m.执行分支组.empty()
        && m.初始模拟事件参数组.empty() && m.初始模拟输入方案组.empty();
}
}

struct 服务对象新需求本能运行服务::实现 final {
    新_世界树类& 世界;
    新_方法类& 方法;
    新_需求类& 需求;
    新_动态类& 动态;
    服务对象需求发布服务& 发布;
    const 新服务要求材料读取接口& 材料读取;
    新_任务类& 任务;
    mutable std::mutex 互斥;
    std::condition_variable 退出条件;
    std::optional<新服务对象新需求配置> 配置;
    std::optional<稳定编码> 方法节点;
    std::size_t 活动租约 = 0;
    std::set<稳定编码> 活动来源;
    bool 就绪 = false, 已关闭 = false, 初始化中 = false;
    class 参数提供者;
    enum class 来源类别 : std::uint8_t { 直接调用, 任务执行 };

    std::vector<稳定编码> 读取支持方法链(稳定编码 method,bool requireReady=true,bool requireEnabled=true) const {
        std::optional<新服务对象新需求配置> cfg;
        std::optional<稳定编码> registered;
        bool ready;
        { std::lock_guard lock(互斥); cfg=配置; registered=方法节点; ready=就绪&&!已关闭; }
        std::set<稳定编码> visited;
        std::vector<稳定编码> chain;
        auto current=method;
        bool named=false, reached=false;
        while(true) {
            新需求要求(有效(current)&&visited.insert(current).second);
            const auto m=方法.获取方法(current);
            新需求要求(m.has_value());
            if(chain.empty())named=m->本能函数登记名==新本能函数登记名_获取服务对象需求节点;
            chain.push_back(current);
            if(registered&&current==*registered){reached=true;break;}
            if(!m->来源方法节点)break;
            current=*m->来源方法节点;
        }
        if(!reached) { 新需求要求(!named); return {}; }
        新需求要求((!requireReady||ready)&&cfg&&registered);
        for(const auto id:chain) {
            const auto m=方法.获取方法(id);
            新需求要求(m&&新需求规格一致(*m,*cfg,requireEnabled));
            if(id==*registered)新需求要求(!m->来源方法节点&&!m->实例根节点);
            else 新需求要求(m->来源方法节点&&m->实例根节点);
        }
        return chain;
    }
    std::pair<新任务执行信息,新任务本能采用材料> 读取任务请求(
        const 新本能需求任务参数请求& request) const {
        const auto execution=任务.获取执行实例(request.执行实例节点);
        新需求要求(execution.has_value());
        // 已有A永远不是可恢复的零动作漂移，不允许另建provider再执行一次。
        新需求要求(!execution->普通动作节点);
        const auto task=任务.获取任务(execution->任务节点);
        新需求要求(task.has_value());
        if(execution->状态!=新任务执行状态::执行中||execution->结束时间
            ||task->当前状态!=新任务状态::执行中
            ||task->最近执行实例节点!=std::optional{request.执行实例节点}
            ||task->最近方法节点!=std::optional{request.方法节点})throw 新本能需求任务前提变化{};
        新需求要求(execution->方法节点==request.方法节点);
        const auto adopted=任务.读取执行本能采用(request.执行实例节点);
        新需求要求(adopted&&execution->本能采用==adopted
            &&adopted->参数绑定组==request.参数绑定组&&adopted->执行分支节点==request.执行分支节点);
        const auto chain=读取支持方法链(request.方法节点,false);
        新需求要求(!chain.empty()&&std::find(chain.begin(),chain.end(),adopted->来源方法节点)!=chain.end());
        return {*execution,*adopted};
    }

    新本能需求材料 读取材料(const 方法参数场景& scene, 稳定编码 需求类型) const {
        const auto checked = 方法.建立参数场景(scene.方法节点(), scene.全部角色绑定(), nullptr,
            &材料读取, {}, scene.执行分支节点());
        if (!checked.参数场景) {
            新需求要求(checked.状态 != 新方法操作状态::资源失败
                && checked.状态 != 新方法操作状态::结构不一致);
            throw 新需求材料变化{};
        }
        const auto p1=scene.查询参数(1),p2=scene.查询参数(2),p3=scene.查询参数(3);
        新需求要求(p1&&p2&&p3&&scene.全部角色绑定().size()==3
            &&p1->信息类型==新方法参数信息类型::存在
            &&p2->信息类型==新方法参数信息类型::特征
            &&p3->信息类型==新方法参数信息类型::存在);
        新本能需求材料 out;
        out.服务对象=p1->信息节点;out.需求特征=p2->信息节点;out.需求节点=p3->信息节点;
        const auto 参数=scene.读取正式信息参数保护(3);
        if(参数.状态==正式信息参数读取状态::前提已变化)throw 新需求材料变化{};
        if(参数.状态==正式信息参数读取状态::资源失败)throw std::bad_alloc{};
        if(参数.状态==正式信息参数读取状态::信息不存在)throw 新需求参数不合法{};
        新需求要求(参数.状态==正式信息参数读取状态::已读取&&参数.信息
            &&参数.信息->信息节点==out.需求节点);
        out.需求内容=参数.信息->直接内容组;
        新需求合保护(out.完整保护,参数.完整保护组);
        auto facts=世界.获取事实服务引用();
        新需求加节点保护(out.完整保护,{out.服务对象,out.需求特征});
        const auto h=facts.存在服务.获取存在(out.服务对象);
        if(!h)throw 新需求材料变化{};
        if(std::count(h->特征节点组.begin(),h->特征节点组.end(),out.需求特征)!=1)
            throw 新需求参数不合法{};
        const auto humanScene=facts.场景服务.查询所属场景(out.服务对象);
        if(!humanScene)throw 新需求材料变化{};
        新需求加节点保护(out.完整保护,{*humanScene});
        const auto f=facts.特征服务.获取特征(out.需求特征);
        新需求要求(f.has_value());
        const auto fd=facts.特征概念服务.读取特征概念定义保护(f->特征概念节点);
        if(fd.前提已变化)throw 新需求材料变化{};
        新需求要求(fd.信息&&fd.信息->类型根==需求类型);
        新需求合保护(out.完整保护,fd.预期节点组);
        out.动作前需求特征=*f;
        const auto d=需求.读取需求治理信息(out.需求节点);
        if(!d)throw 新需求材料变化{};
        if(d->需求主体存在节点!=out.服务对象||d->执行域!=新需求执行域::被认知)
            throw 新需求参数不合法{};
        out.动作前需求=*d;
        const auto qualification=需求.读取被认知需求完整资格保护(out.需求节点,out.服务对象);
        核对新需求资格状态(qualification.状态);
        新需求要求(qualification.信息&&!qualification.完整保护.empty());
        新需求要求(qualification.信息->来源种类==新被认知需求来源种类::旧理解内容确认
            ||qualification.信息->来源种类==新被认知需求来源种类::首次接收认知);
        if(qualification.信息->需求!=*d)throw 新需求材料变化{};
        if(qualification.信息->来源种类==新被认知需求来源种类::旧理解内容确认
            &&d->信息完善阶段!=新需求信息完善阶段::已完善)throw 新需求材料未齐{};
        新需求合保护(out.完整保护,qualification.完整保护);
        if(!新需求保护仍在(out.完整保护))throw 新需求材料变化{};
        return out;
    }
};

// 每次调用的参数能力；共享服务租约保活事实owner，恢复材料不以K形成第二份需求账。
class 服务对象新需求本能运行服务::实现::参数提供者 final : public 新本能需求运行接口 {
    std::shared_ptr<服务对象新需求本能运行服务> 服务_;
    稳定编码 方法_, 场景_, 主体_, 来源_, 需求类型_;
    来源类别 来源类别_;
    std::optional<稳定编码> 任务节点_, 执行分支_;
    std::vector<新方法参数角色绑定> 参数_;
    std::optional<稳定编码> 动作_;
    mutable std::optional<新需求治理信息> 动作前需求基线_;
    std::optional<新需求治理信息> 阶段后需求基线_;
    mutable std::optional<新特征信息> 动作前特征基线_;
    bool 阶段完善已尝试_=false, 特征发布已尝试_=false;
    mutable std::mutex 调用互斥_;
    bool 同参数(const 方法参数场景& scene) const {
        return scene.方法节点() == 方法_ && scene.全部角色绑定() == 参数_
            && scene.执行分支节点() == 执行分支_;
    }
    新本能需求材料读回 读取实际材料(const 方法参数场景& scene) const {
        if (!同参数(scene)) return {};
        try { return {新本能需求材料状态::已读取, 服务_->实现_->读取材料(scene, 需求类型_)}; }
        catch (const 新需求材料未齐&) { return {新本能需求材料状态::材料待齐, {}}; }
        catch (const 新需求材料变化&) { return {新本能需求材料状态::前提已变化, {}}; }
        catch (const 新需求需重新确认&) { return {新本能需求材料状态::需重新确认, {}}; }
        catch (const 新需求参数不合法&) { return {新本能需求材料状态::参数不合法, {}}; }
    }
public:
    参数提供者(std::shared_ptr<服务对象新需求本能运行服务> owner,
        稳定编码 method, std::vector<新方法参数角色绑定> params,
        稳定编码 scene, 稳定编码 actor, 稳定编码 source, 稳定编码 type,
        来源类别 kind=来源类别::直接调用, std::optional<稳定编码> task={},
        std::optional<稳定编码> branch={})
        : 服务_(std::move(owner)), 方法_(method), 场景_(scene), 主体_(actor), 来源_(source),
          需求类型_(type), 来源类别_(kind), 任务节点_(task), 执行分支_(branch), 参数_(std::move(params)) {}
    ~参数提供者() override {
        auto& s = *服务_->实现_;
        std::lock_guard lock(s.互斥);
        s.活动来源.erase(来源_); --s.活动租约; s.退出条件.notify_all();
    }
    新本能需求材料读回 读取新需求材料(const 方法参数场景& scene) const override {
        std::lock_guard lock(调用互斥_);
        // 动作后只允许沿同provider继续发布/读回，不把重入核心伪报成一次零动作调用。
        新需求要求(!动作_);
        return 读取实际材料(scene);
    }
    新本能需求材料状态 复核新需求材料(const 方法参数场景& scene,
        const 新本能需求材料& before) const override {
        std::lock_guard lock(调用互斥_);
        新需求要求(!动作_);
        const auto fresh = 读取实际材料(scene);
        if (fresh.状态 != 新本能需求材料状态::已读取 || !fresh.材料) return fresh.状态;
        const bool same=before.动作前需求 == fresh.材料->动作前需求
            && before.服务对象 == fresh.材料->服务对象 && before.需求特征 == fresh.材料->需求特征
            && before.需求节点 == fresh.材料->需求节点
            && before.动作前需求特征 == fresh.材料->动作前需求特征
            && 新需求保护仍在(before.完整保护);
        if(!same)return 新本能需求材料状态::前提已变化;
        动作前需求基线_=before.动作前需求;
        动作前特征基线_=before.动作前需求特征;
        return 新本能需求材料状态::已读取;
    }
    稳定编码 开始本次需求动作(const 方法参数场景& scene) override {
        std::lock_guard lock(调用互斥_);
        新需求要求(同参数(scene));
        if (动作_) return *动作_;
        新需求要求(动作前需求基线_&&动作前特征基线_);
        auto& s = *服务_->实现_;
        const auto h = scene.查询参数(1), f = scene.查询参数(2);
        新需求要求(h && f);
        const auto time = UTC时钟适配器{}.读取当前UTC时间证据_v1({});
        新需求要求(time.成功());
        const auto action = s.世界.建立动作动态(场景_, {主体_, 方法_, 来源_, h->信息节点,
            f->信息节点, {新状态时间语义::实例绝对UTC, time.证据->UTC纳秒}});
        新需求要求(action.成功());
        const auto read = s.动态.按来源查询动作(来源_);
        新需求要求(read.状态 == 新动态操作状态::已完成 && read.动作);
        动作_ = read.动作->节点;
        if(来源类别_==来源类别::直接调用) {
            s.方法.绑定直接调用动作(来源_, *动作_);
        } else {
            新需求要求(任务节点_.has_value());
            const auto bound=s.任务.记录本能动作身份(*任务节点_,来源_,*动作_,s.动态);
            新需求要求(bound==新任务操作状态::已修改||bound==新任务操作状态::精确重复);
        }
        return *动作_;
    }
    新本能需求发布读回 发布新需求(const 方法参数场景& scene,
        const 新本能需求材料& material, 稳定编码 action) override {
        std::lock_guard lock(调用互斥_);
        using S=新本能需求发布状态;
        新本能需求发布读回 out;out.需求节点=material.需求节点;
        新需求要求(同参数(scene)&&动作_==std::optional{action});
        新需求要求(动作前需求基线_&&动作前特征基线_
            &&material.动作前需求==*动作前需求基线_
            &&material.动作前需求特征==*动作前特征基线_);
        auto& s=*服务_->实现_;
        auto fresh=读取实际材料(scene);
        if(fresh.状态==新本能需求材料状态::需重新确认){out.状态=S::需重新确认;return out;}
        if(fresh.状态!=新本能需求材料状态::已读取||!fresh.材料)return out;
        if(material.需求节点!=fresh.材料->需求节点)return out;
        // 写后返回漂移时只接纳“同一份原内容、仅阶段完善”的实际读回，
        // 不把任意fresh DTO当成本provider的阶段结果，也不推断阶段由谁造成。
        if(阶段完善已尝试_&&!阶段后需求基线_) {
            auto expected=*动作前需求基线_;
            expected.信息完善阶段=新需求信息完善阶段::已完善;
            if(fresh.材料->动作前需求==expected)阶段后需求基线_=fresh.材料->动作前需求;
        }
        const auto& expected=阶段后需求基线_ ? *阶段后需求基线_ : *动作前需求基线_;
        if(fresh.材料->动作前需求!=expected)return out;
        bool 已有发布=false;
        if(特征发布已尝试_) {
            const auto old=s.需求.读取主体需求特征发布关联(material.需求节点);
            新需求要求(old.状态==新需求操作状态::已找到);
            已有发布=old.关联.has_value();
            if(已有发布)新需求要求(old.关联->需求节点==material.需求节点
                &&old.关联->主体存在节点==material.服务对象);
        }
        if(!已有发布&&fresh.材料->动作前需求特征!=*动作前特征基线_)return out;
        if(fresh.材料->动作前需求.首次接收认知来源
            &&fresh.材料->动作前需求.信息完善阶段==新需求信息完善阶段::待完善) {
            out.状态=S::已建需求尚未发布;
            阶段完善已尝试_=true;
            const auto completed=s.需求.完善接收认知需求信息(material.需求节点,expected);
            if(completed.状态==新需求操作状态::资源失败)throw std::bad_alloc{};
            if(completed.状态==新需求操作状态::前提已变化)return out;
            新需求要求((completed.状态==新需求操作状态::已修改||completed.状态==新需求操作状态::无变化)
                &&completed.信息.has_value());
            auto after=expected;after.信息完善阶段=新需求信息完善阶段::已完善;
            新需求要求(*completed.信息==after);
            阶段后需求基线_=*completed.信息;
            fresh=读取实际材料(scene);
            if(fresh.状态==新本能需求材料状态::需重新确认){out.状态=S::需重新确认;return out;}
            if(fresh.状态!=新本能需求材料状态::已读取||!fresh.材料
                ||fresh.材料->动作前需求!=*阶段后需求基线_
                ||fresh.材料->动作前需求特征!=*动作前特征基线_)return out;
        }
        const auto targetScene=s.世界.获取事实服务引用().场景服务.查询所属场景(material.服务对象);
        if(!targetScene)return out;
        const auto basis=s.世界.读取需求引用更新依据(*targetScene,material.服务对象,material.需求特征);
        if(basis.状态==新世界需求引用组合状态::前提已变化)return out;
        新需求要求(basis.状态==新世界需求引用组合状态::已读取&&basis.依据);
        const auto a=s.动态.获取动态(action);
        新需求要求(a&&a->动作时间&&a->动作定义节点==方法_&&a->动作来源节点==来源_
            &&a->主题存在节点==主体_&&a->动作目标存在节点==material.服务对象
            &&a->动作目标特征节点==material.需求特征);
        if(!已有发布&&basis.依据->前状态节点) {
            const auto before=s.世界.获取事实服务引用().状态服务.获取状态(*basis.依据->前状态节点);
            新需求要求(before.has_value());
            if(a->动作时间->纳秒<=before->强时间.纳秒)return out;
        }
        // D阶段完善与F引用发布分开保留；这里只落实同一D，不新建或覆盖目标。
        out.状态=S::已建需求尚未发布;
        特征发布已尝试_=true;
        const auto published=s.发布.发布已确认需求({*targetScene,material.服务对象,material.需求节点,
            方法_,来源_,主体_,action});
        if(published.状态==服务对象需求发布状态::前提已变化)return out;
        if(published.状态==服务对象需求发布状态::需重新确认){out.状态=S::需重新确认;return out;}
        if(published.状态==服务对象需求发布状态::需求信息待完善)return out;
        新需求要求(published.状态==服务对象需求发布状态::已发布
            ||published.状态==服务对象需求发布状态::已复用
            ||published.状态==服务对象需求发布状态::已发布但当前读回变化);
        新需求要求(!published.本次新建发生||published.需求发生.has_value());
        if(published.需求发生) {
            const auto& 原=*published.需求发生;
            const auto occurrence=s.发布.读取需求发生状态(原.状态.节点);
            新需求要求(occurrence.状态==服务对象需求发生读取状态::已读取&&occurrence.发生.has_value());
            const auto& 已读=*occurrence.发生;
            const auto& 定位=已读.关联.定位;
            新需求要求(已读.关联==原.关联&&定位.主体存在节点==material.服务对象
                &&定位.特征节点==material.需求特征&&定位.需求节点==material.需求节点);
            const auto 原动作=s.动态.获取动态(定位.动作节点);
            新需求要求(原动作&&原动作->动作来源节点==std::optional{已读.关联.动作来源节点});
            if(published.本次新建发生||已读.关联.动作来源节点==来源_)
                新需求要求(定位.动作节点==action&&已读.关联.动作来源节点==来源_);
            else {
                const auto 首次=s.需求.读取主体需求特征发布关联(material.需求节点);
                新需求要求(首次.关联&&首次.关联->动作节点==定位.动作节点);
            }
            out.结果状态节点组.push_back(已读.状态.节点);
        }
        if(published.状态==服务对象需求发布状态::已发布但当前读回变化) {
            out.状态=S::已发布但当前读回变化;return out;
        }
        const auto origin=s.需求.读取主体需求特征发布关联(material.需求节点);
        新需求要求(origin.状态==新需求操作状态::已找到&&origin.关联
            &&origin.关联->需求节点==material.需求节点&&origin.关联->主体存在节点==material.服务对象
            &&origin.关联->场景节点==*targetScene);
        if(published.状态==服务对象需求发布状态::已发布)
            新需求要求(origin.关联->动作节点==action);
        const 新本能已发布需求结果 typed{material.需求节点,material.服务对象,material.需求特征,
            origin.关联->场景节点,action,origin.关联->动作节点};
        const auto proof=s.发布.核验已发布需求结果(typed);
        if(proof.状态==新本能已发布需求核验状态::前提已变化)return out;
        if(proof.状态==新本能已发布需求核验状态::资源失败)throw std::bad_alloc{};
        新需求要求(proof.状态==新本能已发布需求核验状态::已核验&&proof.结果==std::optional{typed});
        const auto ready=读取实际材料(scene);
        if(ready.状态!=新本能需求材料状态::已读取||!ready.材料
            ||ready.材料->动作前需求!=(阶段后需求基线_ ? *阶段后需求基线_ : *动作前需求基线_)
            ||!新需求保护仍在(proof.完整保护))return out;
        out.已发布需求=typed;
        out.状态=published.状态==服务对象需求发布状态::已发布
            ? S::已落实并发布 : S::已复用并发布;
        return out;
    }
};

服务对象新需求本能运行服务::服务对象新需求本能运行服务(新_世界树类& world,
    新_方法类& method, 新_需求类& demand, 新_动态类& dynamic,
    服务对象需求发布服务& publish, const 新服务要求材料读取接口& reader, 新_任务类& task) noexcept {
    try { 实现_ = std::make_unique<实现>(world, method, demand, dynamic, publish, reader, task); }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象新需求", L"运行服务构造失败。"); }
}
服务对象新需求本能运行服务::~服务对象新需求本能运行服务() {
    // 最后一份shared端口租约可能在静态退出时释放；此处不再进入其它owner。
    if (实现_) { 实现_->已关闭 = true; 实现_->就绪 = false; }
}
bool 服务对象新需求本能运行服务::初始化(const 新服务对象新需求配置& config) noexcept {
    新需求登记阶段观察 总计(L"初始化总计");
    try {
        auto& s = *实现_;
        {
            新需求登记阶段观察 状态观察(L"初始化状态核对");
            bool 重复 = false, 拒绝 = false;
            {
            std::lock_guard lock(s.互斥);
            拒绝 = s.已关闭 || s.初始化中 || (s.配置 && *s.配置 != config);
            重复 = !拒绝 && s.就绪;
            if (!拒绝 && !重复) { s.初始化中 = true; s.配置 = config; }
            }
            状态观察.结束(拒绝 ? 0 : 重复 ? 2 : 1, !拒绝);
            if (拒绝) { 总计.结束(0, false); return false; }
            if (重复) { 总计.结束(1); return true; }
        }
        struct 完成 final { 实现& s; ~完成() { std::lock_guard lock(s.互斥); s.初始化中 = false; } } done{s};
        const auto facts = s.世界.获取事实服务引用();
        {
            新需求登记阶段观察 t(L"核对自我存在");
            const auto ok = facts.存在服务.是存在节点(config.自我存在节点);
            t.结束(ok, ok); if (!ok) { 总计.结束(0, false); return false; }
        }
        {
            新需求登记阶段观察 t(L"核对人类概念");
            const auto c = facts.存在概念服务.获取存在概念(config.人类概念节点);
            t.结束(c.has_value(), c.has_value()); if (!c) { 总计.结束(0, false); return false; }
        }
        {
            新需求登记阶段观察 t(L"核对通用存在概念");
            const auto c = facts.存在概念服务.获取存在概念(config.通用存在概念节点);
            t.结束(c.has_value(), c.has_value()); if (!c) { 总计.结束(0, false); return false; }
        }
        {
            新需求登记阶段观察 t(L"核对需求特征类型");
            const auto c = facts.特征概念服务.获取特征概念(config.对象需求类型);
            t.结束(c.has_value(), c.has_value()); if (!c) { 总计.结束(0, false); return false; }
        }
        新需求登记阶段观察 内容观察(L"读取完整被认知需求结构");
        const auto 根=facts.存在概念服务.读取先天需求任务方法概念();
        if(根.状态!=正式结构操作状态::已读取||!根.信息||根.信息->需求!=config.需求身份概念) {
            总计.结束(0,false);return false;
        }
        const auto complete = facts.存在概念服务.读取正式结构有效内容保护(config.完整被认知需求概念);
        内容观察.结束(static_cast<std::int64_t>(complete.状态), complete.信息.has_value());
        if (complete.状态 != 正式结构有效内容读取状态::已读取 || !complete.信息) { 总计.结束(0, false); return false; }
        新需求登记阶段观察 表观察(L"初始化本能函数注册表");
        const auto registration = 新_本能函数管理类::初始化();
        表观察.结束(static_cast<std::int64_t>(registration));
        if (registration != 新本能函数登记状态::已登记 && registration != 新本能函数登记状态::精确重复) { 总计.结束(0, false); return false; }
        新需求登记阶段观察 端口观察(L"装配新需求运行端口");
        const auto attach = 新_本能函数管理类::装配服务对象新需求端口(shared_from_this());
        端口观察.结束(static_cast<std::int64_t>(attach));
        if (attach != 新本能函数端口装配状态::已装配 && attach != 新本能函数端口装配状态::精确重复) { 总计.结束(0, false); return false; }
        新需求登记阶段观察 场景观察(L"建立登记用途空参数场景");
        const auto scene = s.方法.建立服务需求登记参数场景();
        场景观察.结束(static_cast<std::int64_t>(scene.状态), scene.参数场景 != nullptr);
        if (!scene.参数场景) { 总计.结束(0, false); return false; }
        新需求登记阶段观察 调用观察(L"调用本能函数生成头与规格");
        const auto result = 新_本能函数管理类::执行(新本能函数登记名_获取服务对象需求节点, *scene.参数场景);
        调用观察.结束(static_cast<std::int64_t>(result.结果.状态));
        if (result.结果.状态 != 新本能函数执行状态::已按合同返回) { 总计.结束(0, false); return false; }
        新需求登记阶段观察 就绪观察(L"发布本地就绪状态");
        bool ready;
        { std::lock_guard lock(s.互斥); s.就绪 = !s.已关闭 && s.方法节点.has_value(); ready = s.就绪; }
        就绪观察.结束(ready, ready); 总计.结束(ready, ready); return ready;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象新需求", L"初始化发生工程异常。"); }
}
bool 服务对象新需求本能运行服务::关闭() noexcept {
    try {
        auto& s = *实现_;
        { std::lock_guard lock(s.互斥); if (s.初始化中) return false; s.已关闭 = true; s.就绪 = false; }
        const auto status = 新_本能函数管理类::关闭服务对象新需求端口(*this);
        {
            std::unique_lock lock(s.互斥);
            s.退出条件.wait(lock, [&] { return s.活动租约 == 0; });
        }
        return status == 新本能函数端口装配状态::已关闭;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象新需求", L"关闭发生工程异常。"); }
}
std::optional<新服务对象新需求配置> 服务对象新需求本能运行服务::读取登记材料() const noexcept {
    try { std::lock_guard lock(实现_->互斥); return 实现_->已关闭 ? std::nullopt : 实现_->配置; }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象新需求", L"登记材料不可读。"); }
}
新方法建立结果 服务对象新需求本能运行服务::确保方法登记(const 新方法知识建立请求& request) noexcept {
    新需求登记阶段观察 总计(L"确保方法登记总计");
    try {
        新需求登记阶段观察 配置观察(L"登记读取冻结配置");
        const auto cfg = 读取登记材料();
        配置观察.结束(cfg.has_value(), cfg.has_value());
        if (!cfg || !request.条件结果组.empty()) return {};
        新需求登记阶段观察 声明观察(L"核对函数自身固定声明");
        新方法信息 expected;
        expected.本能函数登记名 = 新本能函数登记名_获取服务对象需求节点;
        expected.作用域 = request.作用域; expected.实现方式 = request.实现方式;
        expected.方法头 = request.方法头; expected.参数规格组 = request.参数规格组;
        expected.行为 = request.行为; expected.初始模拟关系组 = request.初始模拟关系组;
        expected.初始必要条件 = request.初始必要条件;
        expected.初始模拟事件参数组 = request.初始模拟事件参数组;
        expected.初始模拟输入方案组 = request.初始模拟输入方案组;
        if (!request.执行分支组.empty()) { 声明观察.结束(0, false); return {新方法操作状态::参数规格不合法, {}}; }
        const auto 规格符合 = 新需求规格一致(expected, *cfg);
        声明观察.结束(规格符合, 规格符合);
        if (!规格符合) return {新方法操作状态::参数规格不合法, {}};
        新需求登记阶段观察 建立观察(L"方法owner建立或复用头与规格");
        const auto result = 实现_->方法.确保本能方法登记(std::string(新本能函数登记名_获取服务对象需求节点), request);
        建立观察.结束(static_cast<std::int64_t>(result.状态), result.方法节点.has_value());
        if (!result.方法节点) { 总计.结束(static_cast<std::int64_t>(result.状态), false); return result; }
        auto 旧声明=request;
        旧声明.参数规格组.at(2).有序规格->范围概念节点=cfg->通用存在概念节点;
        const auto 退役=实现_->方法.退役旧服务对象需求登记(旧声明);
        新需求要求(退役.状态==新方法操作状态::无变化||退役.状态==新方法操作状态::登记已退役);
        新需求登记阶段观察 读回观察(L"正式读回方法头与固定规格");
        const auto read = 实现_->方法.获取方法(*result.方法节点);
        新需求要求(read && 新需求规格一致(*read, *cfg) && !read->来源方法节点 && !read->实例根节点);
        读回观察.结束(1);
        { std::lock_guard lock(实现_->互斥); 实现_->方法节点 = result.方法节点; }
        总计.结束(static_cast<std::int64_t>(result.状态)); return result;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象新需求", L"方法登记发生工程异常。"); }
}
std::shared_ptr<新本能需求运行接口> 服务对象新需求本能运行服务::建立参数操作提供者(
    稳定编码 method, const std::vector<新方法参数角色绑定>& params,
    稳定编码 actionScene, 稳定编码 actor, 稳定编码 source) {
    auto& s = *实现_;
    稳定编码 type;
    {
        std::lock_guard lock(s.互斥);
        新需求要求(s.就绪 && !s.已关闭 && s.配置 && s.方法节点 == std::optional{method}
            && actor == s.配置->自我存在节点 && !s.活动来源.contains(source));
        type = s.配置->对象需求类型;
        s.活动来源.insert(source);
        ++s.活动租约;
    }
    try {
        // 正式调用来源只在外层构造能力时核验；核心及材料读取不查询K或线程身份。
        const auto checked = s.方法.建立参数场景(method, params, nullptr, &s.材料读取);
        新需求要求(checked.参数场景 != nullptr && 有效(actionScene) && 有效(actor) && 有效(source));
        const auto call = s.方法.读取直接调用身份(source);
        新需求要求(call && !call->已收束 && !call->动作节点
            && call->方法节点 == method && call->现实场景 == actionScene);
        std::vector<新方法有序参数引用> refs;
        for (std::uint32_t i = 1; i <= 3; ++i) {
            const auto p = checked.参数场景->查询参数(i);
            新需求要求(p != nullptr); refs.push_back(*p);
        }
        新需求要求(call->参数引用组 == refs);
        return std::make_shared<实现::参数提供者>(shared_from_this(), method, params,
            actionScene, actor, source, type);
    } catch (...) {
        std::lock_guard lock(s.互斥);
        s.活动来源.erase(source); --s.活动租约; s.退出条件.notify_all();
        throw;
    }
}
新方法参数场景结果 服务对象新需求本能运行服务::建立执行参数场景(
    稳定编码 method, std::vector<新方法参数角色绑定> params,
    稳定编码 actionScene, 稳定编码 actor, 稳定编码 source) {
    auto provider = 建立参数操作提供者(method, params, actionScene, actor, source);
    return 实现_->方法.建立参数场景(method, std::move(params), nullptr,
        &实现_->材料读取, {}, {}, std::move(provider));
}
bool 服务对象新需求本能运行服务::支持任务方法(稳定编码 method) const noexcept {
    try { return !实现_->读取支持方法链(method).empty(); }
    catch(const std::exception& error) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"新需求任务参数工厂",
            L"方法来源、固定规格或运行服务状态不合法。",error.what());
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"新需求任务参数工厂",L"方法覆盖核验工程异常。");
    }
}
新本能已发布需求核验结果 服务对象新需求本能运行服务::核验任务新需求结果(
    稳定编码 method,const 新本能已发布需求结果& result) const noexcept {
    using S=新本能已发布需求核验状态;
    try {
        auto& s=*实现_;
        {std::lock_guard lock(s.互斥);if(s.已关闭)return {S::已关闭,{},{}};}
        const auto chain=s.读取支持方法链(method,false,false);
        新需求要求(!chain.empty());
        std::vector<基础联合节点快照> protection;
        const auto capture=[&](const std::vector<稳定编码>& nodes) {
            const auto read=全局基础数据集.查询节点组完整结构(nodes);
            if(read.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            新需求要求(read.状态==基础结构操作状态::已完成&&read.节点组.size()==nodes.size());
            for(const auto& item:read.节点组) {
                新需求要求(item.快照.has_value());新需求合保护(protection,{*item.快照});
            }
        };
        // 历史窄读独立区分资源失败；不改变旧执行读取helper的工程错误合同。
        const auto unchanged=[&]() {
            std::map<稳定编码,const 基础联合节点快照*> expected;
            for(const auto& item:protection) {
                const auto [it,added]=expected.emplace(item.结构.节点.编码,&item);
                if(!added&&!新需求保护相同(*it->second,item))return false;
            }
            std::vector<稳定编码> nodes;
            for(const auto& [node,snapshot]:expected){(void)snapshot;nodes.push_back(node);}
            const auto fresh=全局基础数据集.查询节点组完整结构(nodes);
            if(fresh.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            新需求要求(fresh.状态==基础结构操作状态::已完成&&fresh.节点组.size()==nodes.size());
            for(std::size_t i=0;i<nodes.size();++i) {
                const auto& item=fresh.节点组[i];
                新需求要求(item.查询节点==nodes[i]);
                if(!item.快照||!新需求保护相同(*expected.at(nodes[i]),*item.快照))return false;
            }
            return true;
        };
        std::set<稳定编码> roots(chain.begin(),chain.end()),metadata;
        for(const auto id:chain) {
            const auto m=s.方法.获取方法(id);新需求要求(m.has_value());
            roots.insert(m->方法虚拟存在节点);roots.insert(m->启用特征节点);metadata.insert(m->方法虚拟存在节点);
        }
        capture({roots.begin(),roots.end()});
        // Vm固定规格的物理子项也纳入保护，不只保护外层方法编码。
        for(int depth=0;depth<2;++depth) {
            std::set<稳定编码> next;
            for(const auto n:metadata) {
                const auto it=std::find_if(protection.begin(),protection.end(),[&](const auto& p){return p.结构.节点.编码==n;});
                新需求要求(it!=protection.end());
                for(const auto& f:it->结构.字段组)if(const auto* ref=std::get_if<稳定编码>(&f.内容))next.insert(*ref);
            }
            capture({next.begin(),next.end()});metadata=std::move(next);
        }
        if(s.读取支持方法链(method,false,false)!=chain||!unchanged())return {};
        auto proof=s.发布.核验已发布需求结果(result);
        if(proof.状态!=S::已核验)return proof;
        新需求要求(proof.结果==std::optional{result});
        新需求合保护(protection,proof.完整保护);
        if(!unchanged())return {};
        proof.完整保护=std::move(protection);return proof;
    }catch(const 新需求材料变化&) {return {};}
    catch(const std::bad_alloc&) {return {S::资源失败,{},{}};}
    catch(const std::exception& error) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"新需求结果历史核验",L"正式方法或已发布需求引用不一致。",error.what());
    }catch(...) {工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"新需求结果历史核验",L"读取发生未知工程异常。");}
}
std::shared_ptr<新本能需求运行接口> 服务对象新需求本能运行服务::建立任务参数操作提供者(
    const 新本能需求任务参数请求& request) {
    auto& s=*实现_;
    新需求要求(支持任务方法(request.方法节点));
    稳定编码 actor,type;
    {
        std::lock_guard lock(s.互斥);
        新需求要求(s.就绪&&!s.已关闭&&s.配置&&有效(request.执行实例节点)
            &&!s.活动来源.contains(request.执行实例节点));
        actor=s.配置->自我存在节点;type=s.配置->对象需求类型;
        s.活动来源.insert(request.执行实例节点);++s.活动租约;
    }
    try {
        const auto [execution,adopted]=s.读取任务请求(request);
        const auto checked=s.方法.建立参数场景(request.方法节点,request.参数绑定组,
            &s.任务,&s.材料读取,adopted.任务成员绑定,request.执行分支节点);
        新需求要求(checked.参数场景!=nullptr);
        // A只挂共同祖先CA；H.F的结果场景C仍由发布owner独立取得，不搬移H或A。
        const auto h=checked.参数场景->查询参数(1);
        新需求要求(h&&h->信息类型==新方法参数信息类型::存在);
        const auto& scenes=s.世界.获取事实服务引用().场景服务;
        const auto root=s.世界.获取世界树根场景();
        const auto actorScene=scenes.查询所属场景(actor),targetScene=scenes.查询所属场景(h->信息节点);
        新需求要求(root&&actorScene&&targetScene
            &&scenes.存在属于场景范围(*root,actor)&&scenes.存在属于场景范围(*root,h->信息节点));
        const auto ancestors=[&](稳定编码 start) {
            std::vector<稳定编码> path;
            std::set<稳定编码> visited;
            auto current=start;
            while(true) {
                新需求要求(有效(current)&&visited.insert(current).second);
                const auto scene=scenes.获取场景(current);
                新需求要求(scene&&scene->节点==current&&scenes.存在属于场景范围(*root,current));
                path.push_back(current);
                if(current==*root){新需求要求(!scene->父场景节点);break;}
                新需求要求(scene->父场景节点.has_value());current=*scene->父场景节点;
            }
            return path;
        };
        const auto actorAncestors=ancestors(*actorScene),targetAncestors=ancestors(*targetScene);
        std::optional<稳定编码> actionScene;
        for(const auto scene:targetAncestors)
            if(std::find(actorAncestors.begin(),actorAncestors.end(),scene)!=actorAncestors.end()) {
                actionScene=scene;break;
            }
        新需求要求(actionScene&&scenes.存在属于场景范围(*actionScene,actor)
            &&scenes.存在属于场景范围(*actionScene,h->信息节点));
        const auto [freshExecution,freshAdopted]=s.读取任务请求(request);
        新需求要求(freshExecution.节点==execution.节点&&freshAdopted==adopted);
        return std::make_shared<实现::参数提供者>(shared_from_this(),request.方法节点,
            request.参数绑定组,*actionScene,actor,request.执行实例节点,type,
            实现::来源类别::任务执行,execution.任务节点,request.执行分支节点);
    } catch(...) {
        std::lock_guard lock(s.互斥);
        s.活动来源.erase(request.执行实例节点);--s.活动租约;s.退出条件.notify_all();
        throw;
    }
}
新方法参数场景结果 服务对象新需求本能运行服务::建立任务参数场景(
    const 新本能需求任务参数请求& request,const 新内部治理对象读取接口* governance,
    std::optional<新方法任务成员绑定> member) {
    auto& s=*实现_;
    auto provider=建立任务参数操作提供者(request);
    const auto adopted=s.读取任务请求(request).second;
    新需求要求(governance==static_cast<const 新内部治理对象读取接口*>(&s.任务)
        &&member==adopted.任务成员绑定);
    const auto result=s.方法.建立参数场景(request.方法节点,request.参数绑定组,governance,
        &s.材料读取,member,request.执行分支节点,std::move(provider));
    新需求要求(result.参数场景!=nullptr);
    (void)s.读取任务请求(request);
    return result;
}
新本能函数本次条件结果对 服务对象新需求本能运行服务::执行(const 方法参数场景& scene) noexcept {
    try {
        auto* provider = scene.获取需求运行接口();
        新需求要求(provider != nullptr);
        return 执行服务对象新需求核心(scene, *provider);
    } catch (const std::exception& error) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象新需求", L"执行工程故障，保留已发生动作及需求。", error.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象新需求", L"执行未知工程故障。");
    }
}
} // namespace 海中鱼巣
