#include "新_模拟世界类.h"
#include "新_方法类.h"
#include "新_本能函数管理类.h"
#include <array>
#include <limits>
#include <mutex>
#include <stdexcept>

namespace 海中鱼巣 {
稳定编码 模拟世界树根节点{};
namespace {
enum 字段 : std::size_t { 类型, 根, 所属, 特征类型, 当前材料, 数值, 任务引用, 虚拟引用,
    状态特征引用, 方法引用, 动作引用, 前状态引用, 后状态引用, 动态引用, 时间语义, 步骤, 字段数 };
enum 节点种类 : std::int64_t { 根种类 = 1, 场景种类, 存在种类, 特征种类, 材料种类,
    任务种类, 虚拟种类, 动作种类, 状态种类, 动态种类 };
std::array<稳定编码, 字段数> 字段组{};
std::mutex 根互斥;

// 只回滚尚未挂根的自建材料；已发表模拟事实不会随运行期对象析构删除。
struct 创建批 final {
    std::vector<稳定编码> 节点, 字段关系, 外部关系;
    bool 保留 = false;
    创建批() { 节点.reserve(64); 字段关系.reserve(256); 外部关系.reserve(64); }
    ~创建批() {
        if (保留) return;
        for (auto i = 字段关系.rbegin(); i != 字段关系.rend(); ++i) 全局基础数据集.删除字段(*i);
        for (auto i = 外部关系.rbegin(); i != 外部关系.rend(); ++i) 全局基础数据集.删除关系(*i);
        for (auto i = 节点.rbegin(); i != 节点.rend(); ++i) 全局基础数据集.删除节点(*i);
    }
    稳定编码 建节点() {
        if (节点.size() == 节点.capacity()) throw std::bad_alloc();
        const auto n = 全局基础数据集.新建节点();
        if (!有效(n)) throw std::runtime_error("模拟节点分配失败");
        节点.push_back(n); return n;
    }
    void 值(稳定编码 n, 稳定编码 f, std::int64_t v) {
        if (字段关系.size() == 字段关系.capacity()) throw std::bad_alloc();
        const auto r = 全局基础数据集.添加字段值(n, f, 基础值{v});
        if (!有效(r)) throw std::runtime_error("模拟字段写入失败");
        字段关系.push_back(r);
    }
    void 引用(稳定编码 n, 稳定编码 f, 稳定编码 v) {
        if (字段关系.size() == 字段关系.capacity()) throw std::bad_alloc();
        const auto r = 全局基础数据集.添加字段节点(n, f, v);
        if (!有效(r)) throw std::runtime_error("模拟引用写入失败");
        字段关系.push_back(r);
    }
    void 挂接(稳定编码 p, 稳定编码 c) {
        if (外部关系.size() == 外部关系.capacity()) throw std::bad_alloc();
        const auto r = 全局基础数据集.添加关系(p, c, 基础外部关系类型::父子);
        if (!有效(r)) throw std::runtime_error("模拟挂接失败");
        外部关系.push_back(r);
    }
};
std::optional<基础字段关系> 唯一字段(稳定编码 n, 字段 f) {
    const auto 组 = 全局基础数据集.查询字段(n, 字段组[f]);
    if (组.size() != 1) return std::nullopt;
    return 组.front();
}
std::optional<std::int64_t> 读值(稳定编码 n, 字段 f) {
    const auto 项 = 唯一字段(n, f); if (!项) return std::nullopt;
    const auto* v = std::get_if<基础值>(&项->内容);
    if (!v) return std::nullopt;
    const auto* x = std::get_if<std::int64_t>(&v->材料);
    return x ? std::optional<std::int64_t>{*x} : std::nullopt;
}
std::optional<稳定编码> 读引用(稳定编码 n, 字段 f) {
    const auto 项 = 唯一字段(n, f); if (!项) return std::nullopt;
    const auto* v = std::get_if<稳定编码>(&项->内容);
    return v && 全局基础数据集.查询节点(*v) ? std::optional<稳定编码>{*v} : std::nullopt;
}
bool 根有效() {
    if (!全局基础数据集.查询节点(模拟世界树根节点)
        || 读值(模拟世界树根节点, 类型) != 根种类) return false;
    for (const auto f : 字段组) {
        if (!全局基础数据集.查询节点(f)) return false;
        const auto 边 = 全局基础数据集.查询目标关系(f, 基础外部关系类型::父子);
        if (边.size() != 1 || 边.front().源节点 != 模拟世界树根节点) return false;
    }
    return true;
}
bool 确保根() {
    std::lock_guard 锁(根互斥);
    if (有效(模拟世界树根节点)) return 根有效();
    创建批 批;
    const auto n = 批.建节点();
    std::array<稳定编码, 字段数> 新字段{};
    for (auto& f : 新字段) { f = 批.建节点(); 批.挂接(n, f); }
    批.值(n, 新字段[类型], 根种类);
    字段组 = 新字段;
    模拟世界树根节点 = n;
    批.保留 = true;
    return true;
}
bool 属于样例(稳定编码 n, 节点种类 k, 稳定编码 scene) {
    if (!全局基础数据集.查询节点(n) || 读值(n, 类型) != k
        || 读引用(n, 根) != 模拟世界树根节点) return false;
    const auto 边 = 全局基础数据集.查询目标关系(n, 基础外部关系类型::父子);
    return 边.size() == 1 && 边.front().源节点 == scene;
}
稳定编码 建样例节点(创建批& 批, 节点种类 k, 稳定编码 scene) {
    const auto n = 批.建节点();
    批.值(n, 字段组[类型], k); 批.引用(n, 字段组[根], 模拟世界树根节点);
    批.挂接(scene, n); return n;
}
}

struct 新_模拟世界类::实现 final {
    新_方法类& 方法;
    mutable std::recursive_mutex 互斥;
    bool 已建立 = false, 停止 = false;
    稳定编码 方法节点, 场景, 自我, 服务特征, 服务材料, 任务, 虚拟, 状态特征, 状态材料;
    std::optional<稳定编码> 动作, 前状态, 后状态, 动态;
    新服务值结算登记材料 材料;
    explicit 实现(新_方法类& m) : 方法(m) {}
};
新_模拟世界类::新_模拟世界类(新_方法类& m) noexcept {
    try { 实现_ = std::make_unique<实现>(m); } catch (...) {}
}
新_模拟世界类::~新_模拟世界类() = default;

新模拟世界建立结果 新_模拟世界类::建立服务结算样例(稳定编码 method,
    const 新服务值结算登记材料& base, std::int64_t value, std::int64_t task) noexcept {
    新模拟世界建立结果 结果;
    if (!实现_) { 结果.状态 = 新模拟世界状态::资源失败; return 结果; }
    try {
        std::lock_guard 锁(实现_->互斥); auto& s = *实现_;
        if (s.已建立 || s.停止 || value != base.冻结前值 || value < 0
            || value >= (std::numeric_limits<std::int64_t>::max)() - 1 || task != 9) return 结果;
        const auto m = s.方法.获取方法(method);
        if (!m || m->来源方法节点 || m->实例根节点 || m->实现方式 != 新方法实现方式::本能
            || m->启用状态 != 新方法启用状态::启用
            || m->本能函数登记名 != 新本能函数登记名_服务值结算) return 结果;
        if (!确保根()) { 结果.状态 = 新模拟世界状态::结构不一致; return 结果; }
        创建批 批;
        s.场景 = 批.建节点(); 批.值(s.场景, 字段组[类型], 场景种类);
        批.引用(s.场景, 字段组[根], 模拟世界树根节点);
        s.自我 = 建样例节点(批, 存在种类, s.场景);
        s.服务特征 = 建样例节点(批, 特征种类, s.场景);
        s.服务材料 = 建样例节点(批, 材料种类, s.场景);
        s.任务 = 建样例节点(批, 任务种类, s.场景);
        s.虚拟 = 建样例节点(批, 虚拟种类, s.场景);
        s.状态特征 = 建样例节点(批, 特征种类, s.场景);
        s.状态材料 = 建样例节点(批, 材料种类, s.场景);
        批.引用(s.自我, 字段组[所属], s.场景);
        批.引用(s.服务特征, 字段组[所属], s.自我);
        批.引用(s.服务特征, 字段组[特征类型], base.服务值类型根);
        批.引用(s.服务特征, 字段组[当前材料], s.服务材料);
        批.引用(s.服务材料, 字段组[所属], s.服务特征); 批.值(s.服务材料, 字段组[数值], value);
        批.引用(s.任务, 字段组[虚拟引用], s.虚拟);
        批.引用(s.虚拟, 字段组[任务引用], s.任务);
        批.引用(s.虚拟, 字段组[状态特征引用], s.状态特征);
        批.引用(s.状态特征, 字段组[所属], s.虚拟);
        批.引用(s.状态特征, 字段组[特征类型], base.任务状态类型根);
        批.引用(s.状态特征, 字段组[当前材料], s.状态材料);
        批.引用(s.状态材料, 字段组[所属], s.状态特征); 批.值(s.状态材料, 字段组[数值], task);
        s.方法节点 = method; s.材料 = base;
        s.材料.自我所在场景 = s.场景; s.材料.自我存在 = s.自我; s.材料.服务值特征 = s.服务特征;
        批.挂接(模拟世界树根节点, s.场景);
        批.保留 = true; s.已建立 = true;
        结果.场景节点 = s.场景;
        结果.状态 = 读取样例() ? 新模拟世界状态::已完成 : 新模拟世界状态::结构不一致;
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = 新模拟世界状态::资源失败; return 结果; }
    catch (...) { 结果.状态 = 新模拟世界状态::结构不一致; return 结果; }
}

std::optional<新模拟服务结算样例读回> 新_模拟世界类::读取样例() const noexcept {
    if (!实现_) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥); const auto& s = *实现_;
        if (!s.已建立 || !根有效() || !属于样例(s.场景, 场景种类, 模拟世界树根节点)
            || !属于样例(s.自我, 存在种类, s.场景) || !属于样例(s.服务特征, 特征种类, s.场景)
            || !属于样例(s.服务材料, 材料种类, s.场景) || !属于样例(s.任务, 任务种类, s.场景)
            || !属于样例(s.虚拟, 虚拟种类, s.场景) || !属于样例(s.状态特征, 特征种类, s.场景)
            || !属于样例(s.状态材料, 材料种类, s.场景)
            || 读引用(s.自我, 所属) != s.场景 || 读引用(s.服务特征, 所属) != s.自我
            || 读引用(s.服务特征, 特征类型) != s.材料.服务值类型根
            || 读引用(s.服务特征, 当前材料) != s.服务材料 || 读引用(s.服务材料, 所属) != s.服务特征
            || 读引用(s.任务, 虚拟引用) != s.虚拟 || 读引用(s.虚拟, 任务引用) != s.任务
            || 读引用(s.虚拟, 状态特征引用) != s.状态特征 || 读引用(s.状态特征, 所属) != s.虚拟
            || 读引用(s.状态特征, 特征类型) != s.材料.任务状态类型根
            || 读引用(s.状态特征, 当前材料) != s.状态材料 || 读引用(s.状态材料, 所属) != s.状态特征) return std::nullopt;
        const auto v = 读值(s.服务材料, 数值), t = 读值(s.状态材料, 数值);
        if (!v || !t) return std::nullopt;
        if (s.动作 && (!属于样例(*s.动作, 动作种类, s.场景)
            || 读引用(*s.动作, 方法引用) != s.方法节点 || 读引用(*s.动作, 任务引用) != s.任务
            || 读引用(*s.动作, 所属) != s.服务特征 || 读值(*s.动作, 步骤) != 1
            || 读值(*s.动作, 时间语义) != static_cast<std::int64_t>(新状态时间语义::抽象相对))) return std::nullopt;
        if (s.前状态 && (!属于样例(*s.前状态, 状态种类, s.场景)
            || 读引用(*s.前状态, 所属) != s.服务特征
            || 读值(*s.前状态, 数值) != s.材料.冻结前值 || 读值(*s.前状态, 步骤) != 1
            || 读值(*s.前状态, 时间语义) != static_cast<std::int64_t>(新状态时间语义::抽象相对))) return std::nullopt;
        if (s.后状态 && (!属于样例(*s.后状态, 状态种类, s.场景)
            || 读引用(*s.后状态, 所属) != s.服务特征 || 读值(*s.后状态, 数值) != v
            || 读值(*s.后状态, 步骤) != 2
            || 读值(*s.后状态, 时间语义) != static_cast<std::int64_t>(新状态时间语义::抽象相对))) return std::nullopt;
        if (s.动态) {
            if (!s.动作 || !s.前状态 || !s.后状态 || !属于样例(*s.动态, 动态种类, s.场景)
                || !属于样例(*s.前状态, 状态种类, s.场景) || !属于样例(*s.后状态, 状态种类, s.场景)
                || 读引用(*s.动态, 动作引用) != s.动作 || 读引用(*s.动态, 前状态引用) != s.前状态
                || 读引用(*s.动态, 后状态引用) != s.后状态 || 读引用(*s.动作, 动态引用) != s.动态
                || 读引用(*s.前状态, 所属) != s.服务特征 || 读引用(*s.后状态, 所属) != s.服务特征
                || 读值(*s.前状态, 数值) != s.材料.冻结前值 || 读值(*s.后状态, 数值) != v
                || 读值(*s.前状态, 步骤) != 1 || 读值(*s.后状态, 步骤) != 2
                || 读值(*s.前状态, 时间语义) != static_cast<std::int64_t>(新状态时间语义::抽象相对)
                || 读值(*s.后状态, 时间语义) != static_cast<std::int64_t>(新状态时间语义::抽象相对)) return std::nullopt;
        }
        return 新模拟服务结算样例读回{模拟世界树根节点,s.场景,s.自我,s.服务特征,
            {s.任务,s.虚拟,s.状态特征},*v,*t,s.动作,s.前状态,s.后状态,s.动态};
    } catch (...) { return std::nullopt; }
}
稳定编码 新_模拟世界类::模拟根节点() const noexcept {
    const auto r = 读取样例(); return r ? r->模拟根节点 : 稳定编码{};
}
std::optional<std::vector<新方法参数角色绑定>> 新_模拟世界类::读取参数绑定() const noexcept {
    try {
        const auto r = 读取样例(); if (!r) return std::nullopt;
        return std::vector<新方法参数角色绑定>{
            {实现_->材料.服务值类型根,r->场景节点,r->自我节点,{{r->服务特征节点}},std::nullopt},
            {实现_->材料.任务状态类型根,std::nullopt,std::nullopt,{{r->来源任务.状态特征节点}},r->来源任务}};
    } catch (...) { return std::nullopt; }
}
bool 新_模拟世界类::核验模拟参数(稳定编码 method, const std::vector<新方法参数角色绑定>& b) const noexcept {
    const auto 绑定 = 读取参数绑定(); return 绑定 && 实现_->方法节点 == method && *绑定 == b;
}
std::optional<新特征信息> 新_模拟世界类::读取模拟特征(稳定编码 n) const noexcept {
    try {
        const auto r = 读取样例(); if (!r) return std::nullopt;
        新特征信息 f; f.节点 = n;
        if (n == r->服务特征节点) { f.特征概念节点 = 实现_->材料.服务值类型根; f.当前值 = r->服务当前值; }
        else if (n == r->来源任务.状态特征节点) { f.特征概念节点 = 实现_->材料.任务状态类型根; f.当前值 = r->任务当前值; }
        else return std::nullopt;
        return f;
    } catch (...) { return std::nullopt; }
}
std::optional<新特征值信息> 新_模拟世界类::读取模拟特征值材料(稳定编码) const noexcept { return std::nullopt; }
std::optional<新任务虚拟存在读回> 新_模拟世界类::读取任务虚拟存在引用(const 新任务虚拟存在引用& v) const noexcept {
    const auto r = 读取样例(); if (!r || r->来源任务 != v) return std::nullopt;
    return 新任务虚拟存在读回{v,实现_->材料.任务状态类型根,r->任务当前值};
}
新服务值结算本能端口* 新_模拟世界类::获取服务值结算模拟端口() noexcept { return this; }
std::optional<新服务值结算登记材料> 新_模拟世界类::读取登记材料() const noexcept {
    if (!实现_) return std::nullopt;
    try { std::lock_guard 锁(实现_->互斥); return !实现_->停止 && 读取样例()
        ? std::optional<新服务值结算登记材料>{实现_->材料} : std::nullopt; } catch (...) { return std::nullopt; }
}
新方法建立结果 新_模拟世界类::确保方法登记(const 新方法知识建立请求& q) noexcept {
    if (!读取登记材料() || !q.条件结果组.empty()) return {};
    const auto m = 实现_->方法.获取方法(实现_->方法节点);
    if (!m || m->本能函数登记名 != 新本能函数登记名_服务值结算 || m->来源方法节点 || m->实例根节点
        || m->启用状态 != 新方法启用状态::启用 || m->作用域 != q.作用域 || m->实现方式 != q.实现方式
        || m->参数规格组 != q.参数规格组 || m->方法头 != q.方法头) return {};
    return {新方法操作状态::已找到, m->节点};
}
新需求建立结果 新_模拟世界类::确保首子需求(稳定编码) noexcept { return {}; }
void 新_模拟世界类::报告并停止(std::string_view) noexcept {
    if (!实现_) return;
    try { std::lock_guard 锁(实现_->互斥); 实现_->停止 = true; } catch (...) {}
}
新服务值结算动作回执 新_模拟世界类::建立普通动作(稳定编码 method,
    const 新任务虚拟存在引用& source, 稳定编码 scene, 稳定编码 self, 稳定编码 feature, std::int64_t value) noexcept {
    新服务值结算动作回执 out;
    if (!实现_) return out;
    try {
        std::lock_guard 锁(实现_->互斥); auto& s = *实现_;
        const auto r = 读取样例();
        if (s.停止 || !r || r->来源任务 != source || r->任务当前值 != 9 || method != s.方法节点
            || scene != r->场景节点 || self != r->自我节点 || feature != r->服务特征节点
            || value != r->服务当前值 || value != s.材料.冻结前值 || s.动作) return out;
        创建批 批;
        const auto a = 建样例节点(批, 动作种类, scene);
        // 已挂已发表场景后，无论后续成败都保留这次已知动作现场。
        批.保留 = true; s.动作 = a;
        批.引用(a, 字段组[方法引用], method); 批.引用(a, 字段组[任务引用], source.任务节点);
        批.引用(a, 字段组[所属], feature); 批.值(a, 字段组[时间语义], static_cast<std::int64_t>(新状态时间语义::抽象相对));
        批.值(a, 字段组[步骤], 1);
        out.动作节点 = a; out.动作时间 = 新状态强时间{新状态时间语义::抽象相对,1};
        out.状态 = 读取样例() ? 新服务值结算协作状态::已完成 : 新服务值结算协作状态::工程停止;
        return out;
    } catch (...) { 报告并停止("模拟动作建立失败"); out.状态 = 新服务值结算协作状态::工程停止; return out; }
}
新服务值结算变化回执 新_模拟世界类::更新普通结果(稳定编码 action, 稳定编码 scene,
    稳定编码 self, 稳定编码 feature, std::int64_t value) noexcept {
    新服务值结算变化回执 out;
    if (!实现_) return out;
    try {
        std::lock_guard 锁(实现_->互斥); auto& s = *实现_;
        const auto r = 读取样例();
        if (s.停止 || !r || r->动作节点 != action || s.动态 || scene != s.场景 || self != s.自我
            || feature != s.服务特征 || r->服务当前值 != s.材料.冻结前值
            || value <= r->服务当前值 || value == (std::numeric_limits<std::int64_t>::max)()) return out;
        const auto current = 唯一字段(s.服务材料, 数值); if (!current) return out;
        out.改变进度 = 新服务值结算改变进度::未知;
        const bool 写入 = 全局基础数据集.修改字段值(current->编码, 基础值{value});
        const auto fresh = 读值(s.服务材料, 数值);
        if (fresh == value) out.改变进度 = 新服务值结算改变进度::已改变;
        else if (fresh == r->服务当前值) out.改变进度 = 新服务值结算改变进度::未改变;
        if (!写入 || out.改变进度 != 新服务值结算改变进度::已改变) { 报告并停止("模拟值更新失败"); return out; }
        创建批 批; 批.保留 = true;
        s.前状态 = 建样例节点(批, 状态种类, scene); s.后状态 = 建样例节点(批, 状态种类, scene);
        for (const auto n : {*s.前状态,*s.后状态}) {
            批.引用(n, 字段组[所属], feature);
            批.值(n, 字段组[时间语义], static_cast<std::int64_t>(新状态时间语义::抽象相对));
        }
        批.值(*s.前状态, 字段组[数值], r->服务当前值); 批.值(*s.前状态, 字段组[步骤], 1);
        批.值(*s.后状态, 字段组[数值], *fresh); 批.值(*s.后状态, 字段组[步骤], 2);
        s.动态 = 建样例节点(批, 动态种类, scene);
        批.引用(*s.动态, 字段组[动作引用], action); 批.引用(*s.动态, 字段组[前状态引用], *s.前状态);
        批.引用(*s.动态, 字段组[后状态引用], *s.后状态); 批.引用(action, 字段组[动态引用], *s.动态);
        out.前状态节点 = s.前状态; out.后状态节点 = s.后状态; out.结果动态节点 = s.动态;
        out.状态 = 读取样例() ? 新服务值结算协作状态::已完成 : 新服务值结算协作状态::工程停止;
        return out;
    } catch (...) { 报告并停止("模拟结果建立失败"); out.状态 = 新服务值结算协作状态::工程停止; return out; }
}
} // namespace 海中鱼巣
