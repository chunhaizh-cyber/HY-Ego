#include "词条归类.本能运行服务.h"
#include "../领域/新_方法类.h"
#include "../领域/新_任务类.h"
#include "../领域/概念_名称关系类.h"
#include "../领域/新_场景类.h"
#include "../领域/新_动态类.h"
#include "../领域/新_本能函数管理类.h"
#include "../适配/适配器.UTC时钟.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <type_traits>

namespace 海中鱼巣 {
namespace {
using S = 新词条归类运行状态;
void 要求归类(bool ok,const char* why) {
    if (!ok) throw std::logic_error(why);
}
新状态强时间 归类动作时间() {
    const auto now = UTC时钟适配器{}.读取当前UTC时间证据_v1({});
    要求归类(now.成功(),"classification action UTC unavailable");
    return {新状态时间语义::实例绝对UTC,now.证据->UTC纳秒};
}
struct 归类当前输入 final {
    S 状态 = S::条件未具备;
    std::optional<新词条归类本能输入> 输入;
    新本能函数本次条件 条件;
    std::vector<稳定编码> 依赖;
};
归类当前输入 读取归类输入(const 方法参数场景& scene,const 新词条归类登记配置& config,
    const std::optional<新词典归类需求来源>& source,const std::function<bool()>& stop) {
    for (;;) {
        归类当前输入 out;
        if (stop()) { out.状态 = S::已停止; return out; }
        const std::array kinds{新方法参数信息类型::词条,新方法参数信息类型::特征,
            新方法参数信息类型::存在,新方法参数信息类型::存在};
        std::array<const 新方法有序参数引用*,4> p{};
        要求归类(scene.全部角色绑定().size() == p.size(),"classification requires four ordered parameters");
        for (std::size_t i = 0; i < p.size(); ++i) {
            p[i] = scene.查询参数(static_cast<std::uint32_t>(i + 1));
            要求归类(p[i] && p[i]->信息类型 == kinds[i],"classification ordered parameter shape invalid");
            out.依赖.push_back(p[i]->信息节点);
        }
        const auto word = scene.读取词条参数挂接(1);
        if (word.状态 == 新方法词条参数读取状态::资源失败) throw std::bad_alloc{};
        if (word.状态 == 新方法词条参数读取状态::参数已失效) { out.状态 = S::需重新组织; return out; }
        要求归类(word.状态 == 新方法词条参数读取状态::已读取 && word.信息,"classification word reader unavailable");
        const auto f = scene.读取存在参数特征(1,config.挂接特征类型);
        if (f.状态 == 新方法参数特征读取状态::定义前提已变化) { std::this_thread::yield(); continue; }
        if (f.状态 == 新方法参数特征读取状态::特征未取得) return out;
        要求归类((f.状态 == 新方法参数特征读取状态::已读取 || f.状态 == 新方法参数特征读取状态::当前值未取得)
            && f.特征,"classification word feature unavailable");
        if (f.特征->节点 != p[1]->信息节点) { out.状态 = S::需重新组织; return out; }
        const auto r = scene.读取正式信息参数保护(3);
        if (r.状态 == 正式信息参数读取状态::前提已变化) { std::this_thread::yield(); continue; }
        if (r.状态 == 正式信息参数读取状态::资源失败) throw std::bad_alloc{};
        if (r.状态 == 正式信息参数读取状态::信息不存在) { out.状态 = S::需重新组织; return out; }
        要求归类(r.状态 == 正式信息参数读取状态::已读取 && r.信息,"classification R reader unavailable");
        const auto field = [&](稳定编码 FT) -> const 正式内容值& {
            const auto i = std::find_if(r.信息->直接内容组.begin(),r.信息->直接内容组.end(),
                [&](const auto& v) { return v.特征类型 == FT; });
            要求归类(i != r.信息->直接内容组.end(),"classification R required field missing");
            return i->内容;
        };
        const auto& target = field(config.归类范围.目标词条FT);
        const auto& conceptValue = field(config.归类范围.采用概念FT);
        if (target.状态 != 正式内容值状态::已知 || !target.值
            || conceptValue.状态 != 正式内容值状态::已知 || !conceptValue.值) return out;
        要求归类(target.种类 == 正式内容值种类::词条引用 && conceptValue.种类 == 正式内容值种类::概念引用,
            "classification R contains wrong field types");
        if (std::get<稳定编码>(*target.值) != p[0]->信息节点) { out.状态 = S::需重新组织; return out; }
        新词条归类本能输入 input{p[0]->信息节点,p[1]->信息节点,p[2]->信息节点,p[3]->信息节点,
            std::get<稳定编码>(*conceptValue.值),{},source};
        if (f.特征->当前值) {
            const auto* C=std::get_if<稳定编码>(&*f.特征->当前值);
            要求归类(C && 有效(*C),"classification current concept identity invalid");
            input.动作前概念组.push_back(*C);
        }
        out.条件.执行分支节点 = scene.执行分支节点();
        for (const auto& binding : scene.全部角色绑定()) {
            新本能函数条件角色材料 row;
            row.参数角色节点 = binding.参数角色节点; row.世界场景节点 = binding.世界场景节点;
            row.有序引用 = binding.有序引用;
            if (binding.有序引用->序号 == 2) row.实际特征组.push_back({f.特征->节点,f.特征->当前值});
            out.条件.参数角色组.push_back(std::move(row));
        }
        out.输入 = std::move(input); out.状态 = S::已就绪;
        return out;
    }
}
} // namespace

struct 词条归类本能运行服务::实现 final {
    新词条归类执行依赖 执行;
    const 新词条归类登记配置 配置;
    std::mutex 生命周期锁, 注册锁;
    std::condition_variable 空闲;
    bool 已关闭 = false;
    std::size_t 活动 = 0;
    std::optional<稳定编码> 方法;
    struct 调用;
    struct 注册项 { std::shared_ptr<调用> 上下文; std::shared_ptr<const 方法参数场景> 场景; };
    std::map<稳定编码,注册项> 调用组;
    实现(新词条归类执行依赖 d,新词条归类登记配置 c) : 执行(d),配置(c) {}
    struct 租约 {
        实现& s; bool 已取得 = false;
        explicit 租约(实现& v) : s(v) {
            std::lock_guard lock(s.生命周期锁);
            if (!s.已关闭) { ++s.活动; 已取得 = true; }
        }
        ~租约() {
            if (!已取得) return;
            std::lock_guard lock(s.生命周期锁); --s.活动; s.空闲.notify_all();
        }
    };
    bool 停止() {
        std::lock_guard lock(生命周期锁);
        return 已关闭 || 新_本能函数管理类::本次停止已请求();
    }
};

struct 词条归类本能运行服务::实现::调用 final : 新本能词条归类运行接口 {
    std::weak_ptr<实现> 宿主;
    const 新本能词条归类任务参数请求 请求;
    const 方法参数场景* 场景身份 = nullptr;
    std::mutex 阶段锁;
    std::optional<新词条归类本能输入> 输入;
    std::optional<新本能函数本次条件> 条件;
    std::optional<稳定编码> A, 主体场景;
    std::vector<稳定编码> 概念组;
    std::optional<新本能词条归类回执> 回执;
    std::optional<概念名称关系组建立结果> 原名称;
    enum class 名称提交阶段 { 未进入, 结果未确认, 明确未发表, 已取得原回执 };
    名称提交阶段 名称阶段 = 名称提交阶段::未进入;
    std::optional<新词条归类未提交原因> 未提交原因;
    bool 收束已保存 = false;
    bool 动作已挂场景 = false, 输入已保存 = false, 关联已处理 = false, 回执已保存 = false;
    调用(std::shared_ptr<实现> s,新本能词条归类任务参数请求 q) : 宿主(s),请求(std::move(q)) {}
    void 确认场景(const 方法参数场景& scene) const {
        要求归类(&scene == 场景身份 && scene.方法节点() == 请求.方法节点
            && scene.执行分支节点() == 请求.执行分支节点 && scene.全部角色绑定() == 请求.参数绑定组,
            "classification invocation used another parameter scene");
    }
    新词条归类动作结果 动作结果(S state) const {
        return {state,A,A ? 条件 : std::nullopt,A ? 输入 : std::nullopt,{}};
    }
    bool 挂接动作(实现& s) {
        if (!A || 动作已挂场景) return true;
        要求归类(主体场景.has_value(),"classification action original scene missing");
        const auto attached = s.执行.场景.添加动态(*主体场景,*A);
        if (attached == 新场景操作状态::资源失败) throw std::bad_alloc{};
        if (attached == 新场景操作状态::场景不存在 || attached == 新场景操作状态::动态不存在) return false;
        要求归类(attached == 新场景操作状态::已完成 || attached == 新场景操作状态::无变化,
            "classification action scene attachment failed");
        动作已挂场景 = true;
        return true;
    }
    void 保存输入(实现& s) {
        if (!A) return;
        if (!输入已保存) {
            const auto saved = s.执行.任务.记录执行词条归类动作(请求.任务节点,请求.执行实例节点,*A,*输入);
            if (saved == 新任务操作状态::资源失败) throw std::bad_alloc{};
            要求归类(saved == 新任务操作状态::已修改 || saved == 新任务操作状态::精确重复,
                "classification original input could not be recorded");
            输入已保存 = true;
        }
    }
    bool 确定未提交() const noexcept {
        return 名称阶段 == 名称提交阶段::未进入 || 名称阶段 == 名称提交阶段::明确未发表;
    }
    S 收束状态() const {
        return 未提交原因 == 新词条归类未提交原因::主动停止 ? S::已停止 : S::需重新组织;
    }
    void 保存未提交收束(实现& s,新词条归类未提交原因 reason) {
        要求归类(A && 输入 && 输入已保存 && !原名称 && 确定未提交(),
            "classification closure lacks certain unsubmitted action input");
        if (!未提交原因) 未提交原因 = reason;
        if (收束已保存) return;
        const auto saved = s.执行.任务.记录执行词条归类未提交(
            请求.任务节点,请求.执行实例节点,*A,*输入,*未提交原因);
        if (saved == 新任务操作状态::资源失败) throw std::bad_alloc{};
        要求归类(saved == 新任务操作状态::已修改 || saved == 新任务操作状态::精确重复,
            "classification original unsubmitted closure could not be recorded");
        收束已保存 = true;
    }
    void 保存回执(实现& s) {
        if (!原名称 || 回执已保存) return;
        if (!关联已处理) {
            if (回执->变化动态 && 回执->新状态) {
                const auto linked = s.执行.动态.关联动作结果(*A,*回执->变化动态,新动作结果关系类型::导致变化);
                if (linked == 新动态操作状态::资源失败) throw std::bad_alloc{};
                要求归类(linked == 新动态操作状态::已完成 || linked == 新动态操作状态::无变化
                    || linked == 新动态操作状态::时间不合法,"classification action result association failed");
            }
            关联已处理 = true;
        }
        const auto saved = s.执行.任务.记录执行词条归类回执(请求.任务节点,请求.执行实例节点,*回执);
        if (saved == 新任务操作状态::资源失败) throw std::bad_alloc{};
        要求归类(saved == 新任务操作状态::已修改 || saved == 新任务操作状态::精确重复,
            "classification original receipt could not be recorded");
        回执已保存 = true;
    }
    新词条归类动作结果 开始或取得归类动作(const 方法参数场景& scene) override {
        const auto s = 宿主.lock();
        if (!s) { std::lock_guard lock(阶段锁); return 动作结果(S::已停止); }
        租约 lease(*s); std::lock_guard lock(阶段锁); 确认场景(scene);
        if (!lease.已取得) return 动作结果(S::已停止);
        if (原名称) { 保存回执(*s); return 动作结果(S::已就绪); }
        if (未提交原因) { 保存未提交收束(*s,*未提交原因); return 动作结果(收束状态()); }
        if (!A && s->停止()) return 动作结果(S::已停止);
        while (!A) {
            const auto existing = s->执行.任务.读取执行词条归类(请求.任务节点,请求.执行实例节点);
            if (existing.状态 == 新词条归类执行读取状态::资源失败) throw std::bad_alloc{};
            if (existing.状态 == 新词条归类执行读取状态::执行不存在) return 动作结果(S::需重新组织);
            要求归类(existing.状态 == 新词条归类执行读取状态::无记录,
                "classification X has historical input without its original invocation conditions");
            const auto prior = s->执行.动态.按来源查询动作(请求.执行实例节点);
            if (prior.状态 == 新动态操作状态::资源失败) throw std::bad_alloc{};
            if (prior.状态 == 新动态操作状态::入口拒绝) return 动作结果(S::需重新组织);
            要求归类(prior.状态 == 新动态操作状态::已完成 && !prior.动作,
                "classification X has an action without its original invocation conditions");
            if (s->停止()) return 动作结果(S::已停止);
            auto read = 读取归类输入(scene,s->配置,请求.来源,[&] { return s->停止(); });
            if (read.状态 != S::已就绪) return {read.状态,{},{},{},std::move(read.依赖)};
            const auto actorScene = s->执行.场景.查询所属场景(read.输入->执行主体);
            if (!actorScene) return 动作结果(S::需重新组织);
            输入 = std::move(read.输入); 条件 = std::move(read.条件); 主体场景 = actorScene;
            概念组 = {输入->采用概念};
            回执.emplace(); 回执->实际输入 = *输入; 回执->名称关系组.reserve(1);
            const auto made = s->执行.动态.建立词条动作动态({输入->执行主体,请求.方法节点,
                请求.执行实例节点,输入->词条,输入->挂接特征,归类动作时间()});
            if (made.动态节点) { A = made.动态节点; 回执->动作 = *A; }
            if (made.状态 == 新动态操作状态::资源失败) throw std::bad_alloc{};
            if (!A && made.状态 == 新动态操作状态::动作前提已变化) {
                输入.reset(); 条件.reset(); 回执.reset(); 主体场景.reset(); std::this_thread::yield(); continue;
            }
            要求归类(made.状态 == 新动态操作状态::已完成 && A,"classification action could not be established");
        }
        if (!挂接动作(*s)) return 动作结果(S::需重新组织);
        保存输入(*s);
        if (s->停止()) {
            保存未提交收束(*s,新词条归类未提交原因::主动停止);
            return 动作结果(收束状态());
        }
        return 动作结果(S::已就绪);
    }
    新词条归类提交结果 提交或取得归类结果(const 方法参数场景& scene) override {
        const auto s = 宿主.lock();
        if (!s) return {S::已停止,{},{}};
        租约 lease(*s); std::lock_guard lock(阶段锁); 确认场景(scene);
        if (!lease.已取得) return {S::已停止,{},{}};
        要求归类(A && 输入 && 条件 && 输入已保存,"classification commit before action input was recorded");
        if (原名称) { 保存回执(*s); return {S::已完成,回执,{}}; }
        if (未提交原因) { 保存未提交收束(*s,*未提交原因); return {收束状态(),{},{}}; }
        要求归类(确定未提交(),"classification name publication outcome is not yet known");
        for (;;) {
            if (s->停止()) {
                保存未提交收束(*s,新词条归类未提交原因::主动停止);
                return {收束状态(),{},{}};
            }
            const auto current = s->执行.任务.读回任务执行(请求.任务节点,请求.执行实例节点);
            if (current.状态 == 新任务治理执行读回状态::资源失败) throw std::bad_alloc{};
            if (current.状态 == 新任务治理执行读回状态::执行不在当前位
                || current.状态 == 新任务治理执行读回状态::已读回等待评估
                || current.状态 == 新任务治理执行读回状态::已读回影响评估) {
                保存未提交收束(*s,新词条归类未提交原因::原任务职责已移走);
                return {收束状态(),{},{}};
            }
            要求归类(current.状态 == 新任务治理执行读回状态::已读回等待观察,
                "classification execution responsibility could not be read");
            const auto word = scene.读取词条参数挂接(1);
            if (word.状态 == 新方法词条参数读取状态::资源失败) throw std::bad_alloc{};
            if (word.状态 == 新方法词条参数读取状态::参数已失效) {
                保存未提交收束(*s,新词条归类未提交原因::输入身份已退出);
                return {收束状态(),{}, {输入->词条}};
            }
            要求归类(word.状态 == 新方法词条参数读取状态::已读取 && word.信息,"classification target word unavailable");
            if (std::none_of(word.信息->特征组.begin(),word.信息->特征组.end(),
                [&](const auto& f) { return f.特征节点 == 输入->挂接特征 && f.特征类型根 == s->配置.挂接特征类型; })) {
                保存未提交收束(*s,新词条归类未提交原因::输入身份已退出);
                return {收束状态(),{}, {输入->词条,输入->挂接特征}};
            }
            if (s->停止()) {
                保存未提交收束(*s,新词条归类未提交原因::主动停止);
                return {收束状态(),{},{}};
            }
            名称阶段 = 名称提交阶段::结果未确认;
            auto published = s->执行.名称.建立或取得名称关系组(输入->词条,概念组);
            if (published.状态 == 概念名称关系建立状态::资源失败) {
                名称阶段 = 名称提交阶段::明确未发表;
                throw std::bad_alloc{};
            }
            if (published.状态 == 概念名称关系建立状态::依赖已变化) {
                名称阶段 = 名称提交阶段::明确未发表;
                std::this_thread::yield(); continue;
            }
            if (published.状态 == 概念名称关系建立状态::依赖不存在) {
                名称阶段 = 名称提交阶段::明确未发表;
                // 只证明本次名称挂接未发表，依赖状态不足以证明原输入身份退出。
                return {S::条件未具备,{}, {输入->词条,输入->挂接特征,输入->采用概念}};
            }
            要求归类(published.状态 == 概念名称关系建立状态::已建立 || published.状态 == 概念名称关系建立状态::已复用,
                "classification name publication failed");
            static_assert(std::is_nothrow_move_constructible_v<概念名称关系组建立结果>);
            原名称.emplace(std::move(published));
            名称阶段 = 名称提交阶段::已取得原回执;
            要求归类(原名称->名称关系组.size() == 1,"classification name receipt has wrong cardinality");
            回执->名称关系组.push_back(原名称->名称关系组.front().节点);
            回执->新状态 = 原名称->新状态; 回执->变化动态 = 原名称->变化动态;
            回执->新增名称 = 原名称->状态 == 概念名称关系建立状态::已建立;
            回执->动态原因 = 原名称->动态原因;
            保存回执(*s);
            return {S::已完成,回执,{}};
        }
    }
};

词条归类本能运行服务::词条归类本能运行服务(新词条归类执行依赖 d,新词条归类登记配置 c)
    : 实现_(std::make_shared<实现>(d,c)) {}
词条归类本能运行服务::~词条归类本能运行服务() { (void)关闭(); }
std::optional<新词条归类登记配置> 词条归类本能运行服务::读取登记材料() const noexcept {
    try {
        auto s = 实现_; std::lock_guard lock(s->生命周期锁);
        return s->已关闭 ? std::nullopt : std::optional{s->配置};
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取词条归类登记材料",L"归类登记材料读取工程故障。"); }
}
新方法建立结果 词条归类本能运行服务::确保方法登记(const 新方法知识建立请求& q) noexcept {
    try {
        auto s = 实现_; 实现::租约 lease(*s);
        要求归类(lease.已取得 && q.条件结果组.empty(),"classification registration closed or contains preset pairs");
        const auto result = s->执行.方法.确保本能方法登记(std::string(新本能函数登记名_按归类材料挂接词条),q);
        if (result.方法节点) {
            std::lock_guard lock(s->注册锁);
            要求归类(!s->方法 || s->方法 == result.方法节点,"classification registered method identity changed");
            s->方法 = result.方法节点;
        }
        return result;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"登记词条归类方法",L"归类方法登记工程故障。"); }
}
bool 词条归类本能运行服务::支持任务方法(稳定编码 M) const noexcept {
    try {
        auto s = 实现_; 实现::租约 lease(*s);
        if (!lease.已取得) return false;
        std::lock_guard lock(s->注册锁); return s->方法 == std::optional{M};
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"识别词条归类方法",L"归类工厂方法身份读取工程故障。"); }
}
新词条归类任务参数场景结果 词条归类本能运行服务::建立任务参数场景(
    const 新本能词条归类任务参数请求& q,const 新内部治理对象读取接口* governance,
    std::optional<新方法任务成员绑定> member) {
    auto s = 实现_; 实现::租约 lease(*s);
    if (!lease.已取得) return {S::已停止,{},{}};
    const auto 取已有 = [&]() -> std::shared_ptr<const 方法参数场景> {
        const auto i = s->调用组.find(q.执行实例节点);
        if (i == s->调用组.end()) return {};
        const auto& prior = i->second.上下文->请求;
        要求归类(prior.方法节点 == q.方法节点 && prior.任务节点 == q.任务节点
            && prior.参数绑定组 == q.参数绑定组 && prior.执行分支节点 == q.执行分支节点,
            "classification X parameter factory changed frozen identity");
        return i->second.场景;
    };
    {
        std::lock_guard lock(s->注册锁);
        要求归类(s->方法 == std::optional{q.方法节点} && 有效(q.任务节点) && 有效(q.执行实例节点),
            "classification factory received another method or missing task/X");
        if (auto prior = 取已有()) return {S::已就绪,std::move(prior),{}};
    }
    auto call = std::make_shared<实现::调用>(s,q);
    新方法参数场景结果 made;
    do {
        if (s->停止()) return {S::已停止,{},{}};
        made = s->执行.方法.建立参数场景(q.方法节点,q.参数绑定组,governance,nullptr,member,q.执行分支节点,{},{},call);
        if (made.状态 == 新方法操作状态::定义前提已变化) std::this_thread::yield();
    } while (made.状态 == 新方法操作状态::定义前提已变化);
    if (made.状态 == 新方法操作状态::资源失败) throw std::bad_alloc{};
    if (made.状态 == 新方法操作状态::参数材料未到) return {S::条件未具备,{},{}};
    if (made.状态 == 新方法操作状态::参数概念不相容)
        return {S::需重新组织,{},{}};
    要求归类(made.状态 == 新方法操作状态::已建立 && made.参数场景,"classification parameter scene construction failed");
    call->场景身份 = made.参数场景.get();
    std::lock_guard lock(s->注册锁);
    if (auto prior = 取已有()) return {S::已就绪,std::move(prior),{}};
    s->调用组.emplace(q.执行实例节点,实现::注册项{std::move(call),made.参数场景});
    return {S::已就绪,made.参数场景,{}};
}
新词条归类任务参数场景结果 词条归类本能运行服务::取得任务续接参数场景(稳定编码 X) {
    auto s = 实现_; 实现::租约 lease(*s);
    if (!lease.已取得) return {S::已停止,{},{}};
    std::lock_guard lock(s->注册锁);
    const auto i = s->调用组.find(X);
    return i == s->调用组.end() ? 新词条归类任务参数场景结果{S::需重新组织,{},{}}
        : 新词条归类任务参数场景结果{S::已就绪,i->second.场景,{}};
}
bool 词条归类本能运行服务::关闭() noexcept {
    try {
        auto s = 实现_;
        { std::unique_lock lock(s->生命周期锁); s->已关闭 = true; s->空闲.wait(lock,[&] { return s->活动 == 0; }); }
        std::vector<std::shared_ptr<实现::调用>> calls;
        {
            std::lock_guard registry(s->注册锁);
            calls.reserve(s->调用组.size());
            for (const auto& [X,item] : s->调用组) calls.push_back(item.上下文);
        }
        bool complete = true;
        for (const auto& call : calls) {
            std::lock_guard phase(call->阶段锁);
            try {
                if (!call->挂接动作(*s)) { complete = false; continue; }
                call->保存输入(*s);
                if (call->原名称) call->保存回执(*s);
                else if (call->A) {
                    if (!call->确定未提交()) { complete = false; continue; }
                    call->保存未提交收束(*s,新词条归类未提交原因::主动停止);
                }
            } catch (const std::bad_alloc&) { complete = false; }
            catch (const std::length_error&) { complete = false; }
        }
        if (complete) { std::lock_guard registry(s->注册锁); s->调用组.clear(); }
        return complete;
    } catch (const std::bad_alloc&) { return false; }
    catch (const std::length_error&) { return false; }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"关闭词条归类服务",L"归类调用回收工程故障，保留已发生事实。"); }
}
} // namespace 海中鱼巣
