#include "自检.模拟学习闭环.h"

#include "模拟学习.服务.h"
#include "../线程/新_学习线程类.h"
#include "../线程/新_任务管理线程类.h"
#include "../领域/新_模拟世界类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/新_场景类.h"
#include "../领域/概念_二次特征类.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

namespace 海中鱼巣 {
namespace {
using 整数 = std::int64_t;
using 区间 = 特征概念I64闭区间;
using namespace std::chrono_literals;

void 要求(bool 成立, const char* 说明) {
    if (!成立) throw std::runtime_error(说明);
}

// 测试只替代登记材料来源，不替代原本能、领域owner、算法或worker。
// 任何非读取调用均记为越界，不生成伪成功材料。
class 只读登记材料端口 final : public 新服务值结算本能端口 {
public:
    新服务值结算登记材料 材料;
    mutable std::atomic<unsigned> 读取次数{0};
    std::atomic<unsigned> 越界次数{0};
    std::function<void()> 读取前;
    std::optional<新服务值结算登记材料> 读取登记材料() const noexcept override {
        try { ++读取次数; if (读取前) 读取前(); return 材料; }
        catch (...) { return std::nullopt; }
    }
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override {
        ++越界次数; return {};
    }
    新需求建立结果 确保首子需求(稳定编码) noexcept override { ++越界次数; return {}; }
    新服务值结算动作回执 建立普通动作(稳定编码, const 新任务虚拟存在引用&,
        稳定编码, 稳定编码, 稳定编码, 整数) noexcept override { ++越界次数; return {}; }
    新服务值结算变化回执 更新普通结果(稳定编码, 稳定编码, 稳定编码,
        稳定编码, 整数) noexcept override { ++越界次数; return {}; }
    void 报告并停止(std::string_view) noexcept override { ++越界次数; }
};

struct 真实端口守卫 final {
    只读登记材料端口& 端口;
    explicit 真实端口守卫(只读登记材料端口& p) : 端口(p) {
        要求(新_本能函数管理类::装配服务值结算端口(p) == 新本能函数端口装配状态::已装配,
            "install watched real port");
    }
    ~真实端口守卫() {
        (void)新_本能函数管理类::关闭服务值结算端口(端口);
        (void)新_本能函数管理类::卸载服务值结算端口(端口);
    }
};

struct 自检服务 final {
    新_特征值类 特征值{全局基础数据集};
    概念_特征类 特征概念;
    新_特征类 特征{特征值, 特征概念};
    新_状态类 状态{特征};
    概念_存在类 存在概念{特征概念};
    新_存在类 存在{特征, 存在概念, 状态};
    概念_二次特征类 二次概念{特征概念, 特征值};
    概念_动态类 动态概念{特征概念, 特征值, 二次概念};
    新_动态类 动态{状态, 动态概念, 特征概念};
    稳定编码 场景根;
    新_场景类 场景{场景根, 存在, 状态, 动态};
    新_方法类 方法{存在, 场景, 特征值, 特征, 存在概念, 特征概念};
    只读登记材料端口 端口;
    稳定编码 方法节点;
    std::vector<稳定编码> 概念组;

    稳定编码 建域(整数 下, 整数 上, std::optional<稳定编码> 父 = {}) {
        特征概念定义 定义;
        定义.值域 = std::vector<区间>{{下, 上}};
        const auto 节点 = 特征概念.建立或取得特征概念(定义, 特征值, 父);
        要求(有效(节点), "create actual feature domain");
        概念组.push_back(节点);
        return 节点;
    }

    自检服务() {
        要求(特征值.初始化() && 特征概念.初始化(特征值)
            && 场景.初始化() && 二次概念.初始化() && 方法.初始化(), "real owners initialization");
        const auto 注册 = 新_本能函数管理类::初始化();
        要求(注册 == 新本能函数登记状态::已登记 || 注册 == 新本能函数登记状态::精确重复,
            "original builtins registration");
        auto& m = 端口.材料;
        m.冻结前值 = 4;
        m.服务值类型根 = 建域(0, 100);
        m.任务状态类型根 = 建域(0, 12);
        m.服务值合法值域 = m.服务值类型根;
        m.动作前单值域 = 建域(4, 4, m.服务值类型根);
        m.目标值域 = 建域(5, 5, m.服务值类型根);
        m.任务完成值域 = 建域(9, 9, m.任务状态类型根);
        const std::vector<二次特征概念输入定义> 输入{
            {0, 二次特征输入角色::左输入, 二次特征输入来源种类::特征准确值, m.服务值类型根},
            {1, 二次特征输入角色::右输入, 二次特征输入来源种类::特征准确值, m.服务值类型根}};
        const auto 方向 = 二次概念.建立或取得默认二次特征概念(二次特征算法::有向特征差值, 输入);
        要求(方向.概念节点.has_value(), "direction concept");
        const auto 增 = 二次概念.建立或取得参照结果域概念(*方向.概念节点, 二次特征结果域种类::大于);
        const auto 等 = 二次概念.建立或取得参照结果域概念(*方向.概念节点, 二次特征结果域种类::等于);
        要求(增.概念节点 && 等.概念节点, "direction domains");
        m.增加方向概念 = *增.概念节点;
        m.不变方向概念 = m.准确相等概念 = *等.概念节点;
        const auto 根 = 场景.建立根场景();
        要求(根.场景节点.has_value(), "ordinary root");
        m.自我所在场景 = *根.场景节点;
        新存在建立请求 请求;
        请求.初始特征组 = {{m.动作前单值域, 新特征准确值{整数{4}}}};
        const auto 自我 = 场景.建立成员存在(*根.场景节点, 请求, {新状态时间语义::实例绝对UTC, 1});
        要求(自我.存在节点.has_value(), "ordinary existence");
        m.自我存在 = *自我.存在节点;
        const auto 存在信息 = 存在.获取存在(m.自我存在);
        要求(存在信息 && 存在信息->特征节点组.size() == 1, "ordinary feature");
        m.服务值特征 = 存在信息->特征节点组.front();
        // 仅登记材料中的只读身份锚；本自检不宣称建立了生产服务需求树。
        m.服务根需求 = 全局基础数据集.新建节点();
        要求(有效(m.服务根需求), "read-only requirement identity anchor");
        新方法知识建立请求 声明;
        声明.方法头.涉及特征类型概念组 = {m.服务值类型根};
        声明.方法头.可能结果概念组 = {m.目标值域};
        新方法参数规格 服务规格;
        服务规格.参数角色节点 = m.服务值类型根;
        服务规格.要求场景 = true;
        服务规格.特征规格组 = {{m.服务值类型根, m.动作前单值域, {}}};
        新方法参数规格 任务规格;
        任务规格.参数角色节点 = m.任务状态类型根;
        任务规格.对象种类 = 新方法参数对象种类::任务虚拟存在;
        任务规格.特征规格组 = {{m.任务状态类型根, m.任务完成值域, {}}};
        声明.参数规格组 = {服务规格, 任务规格};
        const auto 登记 = 方法.确保本能方法登记(std::string(新本能函数登记名_服务值结算), 声明);
        要求(登记.状态 == 新方法操作状态::已建立 && 登记.方法节点, "owner head-only registration");
        方法节点 = *登记.方法节点;
    }

    std::shared_ptr<新_模拟世界类> 新样例() {
        auto 样例 = std::make_shared<新_模拟世界类>(方法);
        const auto 建立 = 样例->建立服务结算样例(方法节点, 端口.材料, 4, 9);
        要求(建立.状态 == 新模拟世界状态::已完成 && 建立.场景节点, "physical sample creation");
        要求(样例->读取样例().has_value() && 样例->读取参数绑定().has_value(), "physical sample readback");
        return 样例;
    }
};

// 合法的缺依赖输入：投影属于真实模拟owner，但不提供写端口，不能fallback。
class 缺端口模拟 final : public 新方法模拟运行接口 {
    std::shared_ptr<新_模拟世界类> 样例_;
public:
    explicit 缺端口模拟(std::shared_ptr<新_模拟世界类> p) : 样例_(std::move(p)) {}
    稳定编码 模拟根节点() const noexcept override { return 样例_->模拟根节点(); }
    bool 核验模拟参数(稳定编码 m, const std::vector<新方法参数角色绑定>& b) const noexcept override {
        return 样例_->核验模拟参数(m, b);
    }
    std::optional<新特征信息> 读取模拟特征(稳定编码 n) const noexcept override { return 样例_->读取模拟特征(n); }
    std::optional<新特征值信息> 读取模拟特征值材料(稳定编码 n) const noexcept override { return 样例_->读取模拟特征值材料(n); }
    std::optional<新任务虚拟存在读回> 读取任务虚拟存在引用(const 新任务虚拟存在引用& r) const noexcept override {
        return 样例_->读取任务虚拟存在引用(r);
    }
    新服务值结算本能端口* 获取服务值结算模拟端口() noexcept override { return nullptr; }
};

struct 读取闸门 final {
    std::mutex 互斥;
    std::condition_variable 变化;
    bool 已进入 = false, 放行 = false;
    void 等待() {
        std::unique_lock 锁(互斥);
        已进入 = true; 变化.notify_all();
        要求(变化.wait_for(锁, 5s, [&] { return 放行; }), "read gate timed out");
    }
    bool 等待进入() {
        std::unique_lock 锁(互斥);
        return 变化.wait_for(锁, 5s, [&] { return 已进入; });
    }
    void 释放() {
        std::lock_guard 锁(互斥); 放行 = true; 变化.notify_all();
    }
};
} // namespace

int 运行模拟学习闭环自检() noexcept {
    std::size_t 通过 = 0, 失败 = 0;
    const auto 检查 = [&](const char* 名称, const auto& 函数) {
        try { 函数(); ++通过; std::cout << "PASS " << 名称 << '\n'; }
        catch (const std::exception& e) { ++失败; std::cout << "FAIL " << 名称 << ": " << e.what() << '\n'; }
        catch (...) { ++失败; std::cout << "FAIL " << 名称 << ": unknown exception\n"; }
    };
    try {
        std::cout << "SETUP actual project/domain owners/original builtin/worker; read-only registration-material substitute; NOT production startup proof\n";
        自检服务 s;
        真实端口守卫 端口守卫{s.端口};
        模拟学习服务 学习{s.方法, s.特征概念, s.端口};
        const auto 普通特征前 = s.特征.获取特征(s.端口.材料.服务值特征);
        const auto 普通存在前 = s.存在.获取存在(s.端口.材料.自我存在);
        const auto 普通场景前 = s.场景.获取场景(s.场景根);
        要求(普通特征前 && 普通存在前 && 普通场景前, "ordinary world snapshots");
        std::vector<特征概念信息> 概念前;
        for (const auto n : s.概念组) {
            const auto c = s.特征概念.获取特征概念(n);
            要求(c.has_value(), "concept snapshot"); 概念前.push_back(*c);
        }
        检查("head-only registration and declared singleton inputs", [&] {
            const auto m = s.方法.获取方法(s.方法节点);
            要求(m && m->条件结果组.empty() && m->参数规格组.size() == 2, "head/spec registration");
            要求(m->参数规格组[0].特征规格组.front().值域概念节点 == s.端口.材料.动作前单值域
                && m->参数规格组[1].特征规格组.front().值域概念节点 == s.端口.材料.任务完成值域,
                "declared domains");
        });
        检查("sample inputs reject wrong state and MAX before action", [&] {
            新_模拟世界类 bad{s.方法};
            要求(bad.建立服务结算样例(s.方法节点, s.端口.材料, 4, 8).状态 != 新模拟世界状态::已完成,
                "wrong task state accepted");
            要求(!bad.读取样例(), "rejected sample published");
            auto m = s.端口.材料; m.冻结前值 = (std::numeric_limits<整数>::max)();
            要求(bad.建立服务结算样例(s.方法节点, m, m.冻结前值, 9).状态 != 新模拟世界状态::已完成,
                "MAX accepted");
        });
        检查("learning rejects unsupported input and supports cancellation", [&] {
            要求(学习.学习({}).状态 == 新模拟学习状态::方法不可读, "invalid method accepted");
            要求(学习.学习(s.方法节点, [] { return true; }).状态 == 新模拟学习状态::已取消,
                "pre-start cancellation ignored");
            要求(学习.学习(s.方法节点, []() -> bool { throw std::runtime_error("cancel"); }).状态
                == 新模拟学习状态::已取消, "throwing cancellation ignored");
            只读登记材料端口 bad; bad.材料 = s.端口.材料;
            bad.材料.冻结前值 = (std::numeric_limits<整数>::max)();
            模拟学习服务 invalid{s.方法, s.特征概念, bad};
            要求(invalid.学习(s.方法节点).状态 == 新模拟学习状态::输入不支持, "invalid material accepted");
            要求(s.方法.获取方法(s.方法节点)->条件结果组.empty(), "rejected learning published CR");
        });
        检查("fake binding and missing simulated port reject without real fallback", [&] {
            const auto sample = s.新样例();
            auto b = sample->读取参数绑定(); 要求(b.has_value(), "sample bindings");
            (*b)[0].实际特征组.front().特征节点 = s.端口.材料.服务值特征;
            要求(!s.方法.建立模拟参数场景(s.方法节点, *b, sample).参数场景, "real feature accepted as simulated");
            b = sample->读取参数绑定();
            要求(!s.方法.建立模拟参数场景(s.方法节点, *b, std::make_shared<缺端口模拟>(sample)).参数场景,
                "missing port accepted");
            要求(s.端口.越界次数 == 0, "real port used");
        });
        检查("original builtin executes physical sample and fresh output", [&] {
            auto sample = s.新样例();
            const auto before = sample->读取样例();
            const auto b = sample->读取参数绑定();
            要求(before && b && before->服务当前值 == 4 && before->任务当前值 == 9 && !before->动作节点,
                "fresh initial sample");
            要求(before->模拟根节点 != s.场景根 && before->场景节点 != s.场景根
                && before->自我节点 != s.端口.材料.自我存在
                && before->服务特征节点 != s.端口.材料.服务值特征, "simulation identity isolation");
            auto p = s.方法.建立模拟参数场景(s.方法节点, *b, sample);
            要求(p.参数场景 && p.参数场景->用途() == 新方法参数场景用途::模拟, "formal simulation scene");
            const auto run = s.方法.调用本能函数(s.方法节点, *p.参数场景);
            const auto after = sample->读取样例();
            要求(run.状态 == 新方法本能调用状态::已调用 && run.本次条件结果对
                && run.本次条件结果对->结果.状态 == 新本能函数执行状态::已按合同返回,
                "original builtin failed");
            要求(after && after->服务当前值 == 5 && after->任务当前值 == 9
                && after->动作节点 && after->前状态节点 && after->后状态节点 && after->结果动态节点,
                "physical result closure");
            要求(run.本次条件结果对->结果.动作动态节点 == after->动作节点
                && run.本次条件结果对->结果.已形成动态节点组 == std::vector<稳定编码>{*after->结果动态节点},
                "runtime and physical evidence mismatch");
            const auto fresh = p.参数场景->读取绑定特征(after->服务特征节点);
            要求(fresh && fresh->当前值 == 新特征准确值{整数{5}},
                "parameter scene cached old value");
            const auto other = s.新样例()->读取样例();
            要求(other && other->场景节点 != after->场景节点 && other->服务当前值 == 4 && !other->动作节点,
                "samples reused mutable state");
            const auto id = after->场景节点;
            p.参数场景.reset();
            sample.reset();
            要求(全局基础数据集.查询节点(id).has_value(), "published sample deleted");
            要求(s.方法.获取方法(s.方法节点)->条件结果组.empty(), "runtime call implicitly published CR");
        });
        检查("all webpage builtins reject simulation before external dispatch", [&] {
            const auto sample = s.新样例();
            const auto p = s.方法.建立模拟参数场景(s.方法节点, *sample->读取参数绑定(), sample);
            要求(p.参数场景 != nullptr, "simulation scene");
            const 新本能函数 函数组[]{新_本能函数集::读取网页对话消息,
                新_本能函数集::设置网页对话输入文本, 新_本能函数集::提交网页对话消息};
            for (const auto f : 函数组) {
                const auto r = f(*p.参数场景);
                要求(r.结果.状态 == 新本能函数执行状态::参数不匹配
                    && r.结果.动作阶段 == 新本能函数动作阶段::未进入动作 && !r.结果.动作动态节点,
                    "web simulation reached real dispatch");
            }
        });
        检查("worker duplicate, pending acknowledgement and stop cancellation", [&] {
            auto gate = std::make_shared<读取闸门>();
            s.端口.读取前 = [gate] { gate->等待(); };
            新_学习线程类 worker{学习};
            要求(worker.启动(3000) == 新学习操作状态::已完成, "worker start");
            const auto first = worker.提交学习(s.方法节点);
            要求(first.处理结果.valid() && gate->等待进入(), "worker not active");
            const auto duplicate = worker.提交学习(s.方法节点);
            要求(duplicate.状态 == 新学习操作状态::精确重复 && duplicate.处理结果.valid(), "duplicate not coalesced");
            要求(worker.确认结果已接收(s.方法节点) == 新学习操作状态::入口拒绝, "unfinished acknowledgement accepted");
            const auto queued = worker.提交学习(s.端口.材料.服务根需求);
            要求(queued.处理结果.valid(), "queued work missing future");
            要求(worker.请求停止() == 新学习操作状态::已完成, "stop request");
            gate->释放();
            要求(worker.等待停止(5000) == 新学习操作状态::已完成, "worker did not join");
            s.端口.读取前 = {};
            要求(first.处理结果.wait_for(0s) == std::future_status::ready
                && duplicate.处理结果.wait_for(0s) == std::future_status::ready
                && queued.处理结果.wait_for(0s) == std::future_status::ready, "accepted future lost");
            要求(first.处理结果.get().状态 == 新模拟学习状态::已取消
                && duplicate.处理结果.get().状态 == 新模拟学习状态::已取消
                && queued.处理结果.get().状态 == 新模拟学习状态::已取消, "stop reported false success");
            要求(worker.提交学习(s.方法节点).状态 == 新学习操作状态::已停止接收, "stopped worker accepted work");
            要求(s.方法.获取方法(s.方法节点)->条件结果组.empty(), "cancelled work published CR");
        });
        s.端口.读取前 = {};
        检查("worker forms one private CR with zero hits and wakes consumer", [&] {
            std::atomic<unsigned> 通知{0};
            std::atomic<新学习操作状态> 自等结果{新学习操作状态::内部错误};
            新_学习线程类* worker地址 = nullptr;
            新_学习线程类 worker{学习, [&] {
                自等结果 = worker地址->等待停止(0);
                ++通知;
                throw std::runtime_error("injected completion notification failure");
            }};
            worker地址 = &worker;
            要求(worker.启动(3000) == 新学习操作状态::已完成, "worker start");
            const auto accepted = worker.提交学习(s.方法节点);
            要求(accepted.处理结果.valid() && accepted.处理结果.wait_for(5s) == std::future_status::ready,
                "learning result timeout");
            const auto r = accepted.处理结果.get();
            要求(r.状态 == 新模拟学习状态::已形成知识 && r.配对节点 && r.模拟场景节点, "learning did not publish");
            const auto duplicate = worker.提交学习(s.方法节点);
            要求(duplicate.状态 == 新学习操作状态::精确重复
                && duplicate.处理结果.get().配对节点 == r.配对节点, "completed result not retained");
            要求(worker.确认结果已接收(s.方法节点) == 新学习操作状态::已完成, "result acknowledgement");
            (void)worker.请求停止();
            要求(worker.等待停止(5000) == 新学习操作状态::已完成 && 通知 > 0, "completion wake/join");
            要求(自等结果 == 新学习操作状态::不能等待自身,
                "completion callback could wait for own worker");
            const auto m = s.方法.获取方法(s.方法节点);
            要求(m && m->条件结果组.size() == 1, "published pair count");
            const auto& pair = m->条件结果组.front();
            要求(pair.命中次数 == 0 && pair.配对节点 == *r.配对节点
                && pair.条件.条件项组.size() == 2 && pair.结果.结果项组.size() == 1, "pair shape/hit count");
            要求(pair.条件.条件项组[0].精确数值域 == 区间{4,4}
                && pair.条件.条件项组[1].精确数值域 == 区间{9,9}
                && pair.结果.结果项组[0].精确数值域 == 区间{5,5}, "observed singleton domains");
            for (const auto& c : pair.条件.条件项组) 要求(!c.特征值域概念节点, "condition dual domain");
            要求(!pair.结果.结果项组[0].特征值域概念节点, "result dual domain");
        });
        检查("repeat learning and method copy preserve knowledge/hit zero", [&] {
            const auto before = s.方法.获取方法(s.方法节点);
            要求(before && before->条件结果组.size() == 1, "missing learned method");
            const auto r = 学习.学习(s.方法节点);
            要求(r.状态 == 新模拟学习状态::精确重复 && s.方法.获取方法(s.方法节点) == before,
                "repeat changed learned knowledge");
            const auto copy = s.方法.复制方法(s.方法节点);
            要求(copy.状态 == 新方法复制状态::已复制 && copy.方法实例节点, "method copy failed");
            const auto m = s.方法.获取方法(*copy.方法实例节点);
            要求(m && m->条件结果组.size() == 1 && m->条件结果组.front().命中次数 == 0
                && m->条件结果组.front().配对节点 != before->条件结果组.front().配对节点
                && m->条件结果组.front().条件 == before->条件结果组.front().条件
                && m->条件结果组.front().结果 == before->条件结果组.front().结果, "copy lost learned private domains");
            要求(学习.学习(*copy.方法实例节点).状态 == 新模拟学习状态::输入不支持, "copy accepted as abstract learning target");
            const 新方法条件结果定义 pair{before->条件结果组.front().条件, before->条件结果组.front().结果};
            要求(s.方法.追加模拟条件结果(s.方法节点, {pair}).状态 == 新方法学习发布状态::已具备条件结果,
                "owner appended duplicate pair");
        });
        检查("ordinary scene/existence/feature and known concepts remain unchanged", [&] {
            要求(s.特征.获取特征(s.端口.材料.服务值特征) == 普通特征前
                && s.存在.获取存在(s.端口.材料.自我存在) == 普通存在前
                && s.场景.获取场景(s.场景根) == 普通场景前, "simulation changed ordinary world");
            for (std::size_t i=0; i<s.概念组.size(); ++i)
                要求(s.特征概念.获取特征概念(s.概念组[i]) == 概念前[i], "simulation changed shared concept");
            要求(s.端口.越界次数 == 0, "simulation called real write/registration/stop port");
        });
        检查("manager reads learned knowledge and waits without creating or advancing tasks", [&] {
            新_需求类 需求{s.特征值, s.存在, s.特征, s.特征概念, s.二次概念};
            要求(需求.初始化(), "actual demand owner initialization");
            新_任务类 任务{需求, s.方法, s.特征值, s.特征, s.特征概念,
                s.状态, s.场景, s.存在, s.二次概念, s.存在概念};
            新_任务管理线程类 manager{任务};
            const auto learned = 学习.学习(s.方法节点);
            要求(learned.状态 == 新模拟学习状态::精确重复, "learned knowledge readback");
            要求(!manager.提交方法学习结果(learned), "unbound/stopped manager accepted learning");
            要求(manager.绑定方法学习读取(s.方法) && manager.绑定方法学习读取(s.方法),
                "same learning reader not idempotent");
            新_方法类 other方法{s.存在, s.场景, s.特征值, s.特征, s.存在概念, s.特征概念};
            要求(!manager.绑定方法学习读取(other方法), "manager replaced bound owner");
            要求(!manager.提交方法学习结果(learned), "not-started manager accepted learning");
            要求(manager.启动({8, 10}, 3000).状态 == 新任务管理操作状态::已完成, "manager start");
            const auto before = 任务.查询全部任务();
            要求(before.状态 == 新任务操作状态::已找到 && before.任务节点组.empty(), "unexpected baseline tasks");
            要求(!manager.提交方法学习结果({}), "manager accepted invalid learning result");
            要求(manager.提交方法学习结果(learned), "manager rejected learned knowledge");
            新任务管理快照 snapshot;
            const auto deadline = std::chrono::steady_clock::now() + 5s;
            do {
                snapshot = manager.读取快照();
                if (snapshot.最近方法学习结果) break;
                std::this_thread::sleep_for(1ms);
            } while (std::chrono::steady_clock::now() < deadline);
            要求(snapshot.最近方法学习结果
                && snapshot.最近方法学习结果->方法节点 == s.方法节点
                && snapshot.等待方法实例消费 && snapshot.活动工作数 == 0,
                "manager did not expose waiting-for-instance-consumption state");
            const auto after = 任务.查询全部任务();
            要求(after.状态 == before.状态 && after.任务节点组 == before.任务节点组,
                "knowledge notification created/advanced tasks");
            要求(manager.请求停止().状态 == 新任务管理操作状态::已完成
                && manager.等待停止(5000).状态 == 新任务管理操作状态::已完成, "manager stop/join");
            要求(!manager.提交方法学习结果(learned), "stopped manager accepted knowledge");
        });
    } catch (const std::exception& e) { ++失败; std::cout << "FAIL initialization/driver: " << e.what() << '\n'; }
    catch (...) { ++失败; std::cout << "FAIL initialization/driver: unknown exception\n"; }
    std::cout << "NOT_RUN production startup/self-manager integration (separate main-run evidence required)\n"
        << "NOT_RUN physical corruption/allocation failure/post-publication unknown injection (no public injection contract)\n"
        << "NOT_RUN full concept inventory equality/Release/Win32/recovery\n"
        << "SUMMARY " << 通过 << " PASS / " << 失败 << " FAIL\n";
    return 失败 == 0 ? 0 : 1;
}
} // namespace 海中鱼巣
