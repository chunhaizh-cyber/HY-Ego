#include "新_模拟世界类.h"
#include "新_方法类.h"
#include "新_本能函数管理类.h"
#include "方法问题归档.模拟世界.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <array>
#include <limits>
#include <mutex>
#include <new>
#include <stdexcept>

namespace 海中鱼巣 {
稳定编码 模拟世界树根节点{};
namespace {
enum 字段 : std::size_t { 类型, 根, 所属, 特征类型, 当前材料, 数值, 任务引用, 虚拟引用,
    状态特征引用, 方法引用, 动作引用, 前状态引用, 后状态引用, 动态引用, 时间语义, 步骤,
    归档格式, 归档身份, 归档载荷, 归档分类, 集合成员, 字段数 };
enum 节点种类 : std::int64_t { 根种类 = 1, 场景种类, 存在种类, 特征种类, 材料种类,
    任务种类, 虚拟种类, 动作种类, 状态种类, 动态种类,
    问题归档种类, 归档快照种类, 归档内容种类 };
std::array<稳定编码, 字段数> 字段组{};
std::mutex 根互斥;
std::mutex 问题归档互斥;

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
        // 先取得回滚槽再调用底座，避免成功分配后记账扩容失败泄漏。
        节点.emplace_back();
        const auto n = 全局基础数据集.新建节点();
        if (!有效(n)) throw std::bad_alloc{};
        节点.back()=n; return n;
    }
    void 值(稳定编码 n, 稳定编码 f, std::int64_t v) {
        原值(n,f,基础值{v});
    }
    void 原值(稳定编码 n, 稳定编码 f, 基础值 v) {
        字段关系.emplace_back();
        const auto r = 全局基础数据集.添加字段值(n, f, std::move(v));
        // 归档路径只消费已建成的有效身份；底座 noexcept 创建失败以无效编码返回。
        if (!有效(r)) throw std::bad_alloc{};
        字段关系.back()=r;
    }
    void 引用(稳定编码 n, 稳定编码 f, 稳定编码 v) {
        字段关系.emplace_back();
        const auto r = 全局基础数据集.添加字段节点(n, f, v);
        if (!有效(r)) throw std::bad_alloc{};
        字段关系.back()=r;
    }
    void 挂接(稳定编码 p, 稳定编码 c) {
        外部关系.emplace_back();
        const auto r = 全局基础数据集.添加关系(p, c, 基础外部关系类型::父子);
        if (!有效(r)) throw std::bad_alloc{};
        外部关系.back()=r;
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
    稳定编码 集合特征, 集合材料;
    std::optional<稳定编码> 动作, 前状态, 后状态, 动态;
    新服务值结算登记材料 材料;
    explicit 实现(新_方法类& m) : 方法(m) {}
};
新_模拟世界类::新_模拟世界类(新_方法类& m) noexcept {
    try { 实现_ = std::make_unique<实现>(m); } catch (...) {}
}
新_模拟世界类::~新_模拟世界类() = default;

namespace {
using 问题编码格式 = 方法问题归档内部::格式;
constexpr const char* 旧问题格式 = "方法问题场景归档/1";
constexpr const char* 组织问题格式 = "方法问题场景归档/2";
问题编码格式 解析问题格式(const std::string& name) {
    if (name==旧问题格式) return 问题编码格式::旧方法副本;
    if (name==组织问题格式) return 问题编码格式::任务执行组织;
    throw std::runtime_error("不支持的归档格式");
}
问题编码格式 材料格式(const 新方法问题反馈材料& x) {
    switch (x.采用来源) {
    case 新方法问题采用来源::旧方法副本: return 问题编码格式::旧方法副本;
    case 新方法问题采用来源::任务执行组织: return 问题编码格式::任务执行组织;
    }
    throw std::runtime_error("不支持的问题采用来源");
}
template<class T> T 读归档值(稳定编码 n,字段 f) {
    const auto field=唯一字段(n,f);
    if (!field) throw std::runtime_error("归档字段缺失或重复");
    const auto* raw=std::get_if<基础值>(&field->内容);
    const auto* v=raw ? std::get_if<T>(&raw->材料) : nullptr;
    if (!v) throw std::runtime_error("归档字段类型非法");
    return *v;
}
std::vector<std::uint64_t> 问题身份(const 新方法问题反馈材料& x) {
    if (x.采用来源==新方法问题采用来源::任务执行组织) {
        if (!x.任务采用) throw std::runtime_error("问题任务采用缺失");
        const auto& p=x.任务采用->定位;
        return {x.任务节点.值,x.执行实例节点.值,x.来源抽象方法节点.值,
            p.组织节点.值,p.方法作用域节点.值,p.配对要求节点.值};
    }
    if (x.采用来源!=新方法问题采用来源::旧方法副本) throw std::runtime_error("问题采用来源非法");
    return {x.任务节点.值,x.执行实例节点.值,x.来源抽象方法节点.值,x.实例方法节点.值};
}
bool 问题入口有效(const 新方法问题反馈材料& x) {
    if (!有效(x.任务节点) || !有效(x.执行实例节点) || !有效(x.来源抽象方法节点)
        || !方法问题归档内部::采用身份有效(x) || !有效(x.影响评估节点)
        || x.问题组.empty() || x.动作前场景.阶段!=新场景快照阶段::动作前
        || x.结果场景.阶段!=新场景快照阶段::结果取得时) return false;
    std::vector<新方法问题种类> expected;
    // 这里只复核上游现有任务验证枚举的原值2，不重新预测或认定因果。
    if (x.原方向验证值==2) expected.push_back(新方法问题种类::方向不符);
    if (x.原值域验证值==2) expected.push_back(新方法问题种类::值域不符);
    if (!x.预计但未发生项组.empty()) expected.push_back(新方法问题种类::预计变化未发生);
    if (!x.计划外状态组.empty()) expected.push_back(新方法问题种类::发生计划外变化);
    return expected==x.问题组;
}
std::int64_t 归档顺序(std::size_t n) {
    if (n>static_cast<std::size_t>((std::numeric_limits<std::int64_t>::max)()))
        throw std::length_error("归档节点顺序溢出");
    return static_cast<std::int64_t>(n);
}
void 写归档快照(创建批& batch,稳定编码 sample,const 新执行场景快照& value,std::int64_t order,问题编码格式 format) {
    auto records=方法问题归档内部::编码场景快照(value,format);
    const auto stage=建样例节点(batch,归档快照种类,sample);
    batch.值(stage,字段组[步骤],order);
    batch.原值(stage,字段组[归档载荷],基础值{std::move(records.front().载荷)});
    for (std::size_t i=1;i<records.size();++i) {
        const auto child=建样例节点(batch,归档内容种类,stage);
        batch.值(child,字段组[步骤],归档顺序(i));
        batch.值(child,字段组[归档分类],records[i].类型);
        batch.原值(child,字段组[归档载荷],基础值{std::move(records[i].载荷)});
    }
}
新执行场景快照 读归档快照(稳定编码 stage,稳定编码 sample,std::int64_t order,问题编码格式 format) {
    if (!属于样例(stage,归档快照种类,sample) || 读值(stage,步骤)!=order)
        throw std::runtime_error("归档快照归属非法");
    auto children=全局基础数据集.查询源关系(stage,基础外部关系类型::父子);
    std::vector<std::pair<std::int64_t,方法问题归档内部::编码记录>> ordered;
    ordered.reserve(children.size());
    for (const auto& edge:children) {
        const auto n=edge.目标节点;
        if (edge.角色或顺序!=0 || !属于样例(n,归档内容种类,stage)
            || !全局基础数据集.查询源关系(n,基础外部关系类型::父子).empty())
            throw std::runtime_error("归档内容归属非法");
        const auto step=读值(n,步骤), kind=读值(n,归档分类);
        if (!step || !kind) throw std::runtime_error("归档内容缺少类型或顺序");
        ordered.push_back({*step,{*kind,读归档值<std::vector<std::uint64_t>>(n,归档载荷)}});
    }
    std::sort(ordered.begin(),ordered.end(),[](const auto& a,const auto& b) { return a.first<b.first; });
    std::vector<方法问题归档内部::编码记录> records;
    records.push_back({0,读归档值<std::vector<std::uint64_t>>(stage,归档载荷)});
    for (std::size_t i=0;i<ordered.size();++i) {
        if (ordered[i].first!=归档顺序(i+1)) throw std::runtime_error("归档内容顺序不连续");
        records.push_back(std::move(ordered[i].second));
    }
    return 方法问题归档内部::解码场景快照(records,format);
}
新方法问题反馈材料 读归档材料(稳定编码 sample,bool published) {
    if (!根有效() || !全局基础数据集.查询节点(sample) || 读值(sample,类型)!=问题归档种类
        || 读引用(sample,根)!=模拟世界树根节点)
        throw std::runtime_error("归档样本身份非法");
    const auto parents=全局基础数据集.查询目标关系(sample,基础外部关系类型::父子);
    if (published ? (parents.size()!=1 || parents.front().源节点!=模拟世界树根节点
            || parents.front().角色或顺序!=0) : !parents.empty())
        throw std::runtime_error("归档发布归属非法");
    const auto format=解析问题格式(读归档值<std::string>(sample,归档格式));
    auto result=方法问题归档内部::解码问题头(读归档值<std::vector<std::uint64_t>>(sample,归档载荷),format);
    if (读归档值<std::vector<std::uint64_t>>(sample,归档身份)!=问题身份(result))
        throw std::runtime_error("归档身份与载荷不一致");
    const auto children=全局基础数据集.查询源关系(sample,基础外部关系类型::父子);
    if (children.size()!=2) throw std::runtime_error("归档首后场景数量非法");
    std::optional<稳定编码> before,after;
    for (const auto& edge:children) {
        if (edge.角色或顺序!=0) throw std::runtime_error("归档阶段关系非法");
        const auto step=读值(edge.目标节点,步骤);
        if (step==1 && !before) before=edge.目标节点;
        else if (step==2 && !after) after=edge.目标节点;
        else throw std::runtime_error("归档首后场景重复或缺失");
    }
    if (!before || !after) throw std::runtime_error("归档首后场景缺失");
    result.动作前场景=读归档快照(*before,sample,1,format);
    result.结果场景=读归档快照(*after,sample,2,format);
    if (!问题入口有效(result)) throw std::runtime_error("归档问题材料非法");
    return result;
}
std::optional<稳定编码> 查归档身份(稳定编码 task,稳定编码 execution) {
    std::optional<稳定编码> result;
    for (const auto& edge:全局基础数据集.查询源关系(模拟世界树根节点,基础外部关系类型::父子)) {
        if (读值(edge.目标节点,类型)!=问题归档种类) continue;
        const auto identity=读归档值<std::vector<std::uint64_t>>(edge.目标节点,归档身份);
        const auto format=解析问题格式(读归档值<std::string>(edge.目标节点,归档格式));
        const std::size_t count=format==问题编码格式::旧方法副本 ? 4U : 6U;
        if (identity.size()!=count || std::any_of(identity.begin(),identity.end(),
            [](std::uint64_t n) { return n==0; })) throw std::runtime_error("归档来源身份非法");
        if (identity[0]!=task.值 || identity[1]!=execution.值) continue;
        if (result) throw std::runtime_error("同次执行存在重复归档");
        result=edge.目标节点;
    }
    return result;
}
} // namespace

新方法问题归档结果 新_模拟世界类::保存方法问题(const 新方法问题反馈材料& value) noexcept {
    新方法问题归档结果 result;
    try {
        if (!实现_) throw std::runtime_error("模拟世界实现未建立");
        if (!问题入口有效(value)) return result;
        std::lock_guard lock(问题归档互斥);
        if (!确保根()) throw std::runtime_error("模拟世界根不一致");
        if (const auto existing=查归档身份(value.任务节点,value.执行实例节点)) {
            result.归档节点=*existing;
            result.状态=读归档材料(*existing,true)==value ? 新方法问题归档状态::精确重复
                : 新方法问题归档状态::身份冲突;
            return result;
        }
        创建批 batch;
        const auto sample=batch.建节点();
        batch.值(sample,字段组[类型],问题归档种类);
        batch.引用(sample,字段组[根],模拟世界树根节点);
        const auto format=材料格式(value);
        batch.原值(sample,字段组[归档格式],基础值{std::string{
            format==问题编码格式::旧方法副本 ? 旧问题格式 : 组织问题格式}});
        batch.原值(sample,字段组[归档身份],基础值{问题身份(value)});
        batch.原值(sample,字段组[归档载荷],基础值{方法问题归档内部::编码问题头(value,format)});
        写归档快照(batch,sample,value.动作前场景,1,format);
        写归档快照(batch,sample,value.结果场景,2,format);
        if (读归档材料(sample,false)!=value) throw std::runtime_error("归档发布前独立读回不相等");
        // 唯一公开可见点。随后工程失败保留已发布归档并终止，不能删除或重做真实动作。
        batch.挂接(模拟世界树根节点,sample);
        batch.保留=true; result.归档节点=sample;
        if (读归档材料(sample,true)!=value) throw std::runtime_error("归档发布后读回不相等");
        result.状态=新方法问题归档状态::已保存;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"保存方法问题归档", L"归档编码、发布或完整读回失败", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"保存方法问题归档", L"未知归档工程故障");
    }
    return result;
}
新方法问题归档读回结果 新_模拟世界类::读取方法问题归档(稳定编码 sample) const noexcept {
    新方法问题归档读回结果 result;
    if (!有效(sample)) return result;
    try {
        std::lock_guard lock(问题归档互斥);
        std::lock_guard rootLock(根互斥);
        if (!有效(模拟世界树根节点) || !全局基础数据集.查询节点(sample)) {
            result.状态=新方法问题归档状态::未找到; return result;
        }
        auto data=std::make_shared<const 新方法问题反馈材料>(读归档材料(sample,true));
        result.归档节点=sample; result.材料=std::move(data); result.状态=新方法问题归档状态::已保存;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法问题归档", L"归档解码或完整读回失败", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法问题归档", L"未知归档工程故障");
    }
    return result;
}
新方法问题归档读回结果 新_模拟世界类::查询方法问题归档(稳定编码 task,稳定编码 execution) const noexcept {
    新方法问题归档读回结果 result;
    if (!有效(task) || !有效(execution)) return result;
    try {
        std::lock_guard lock(问题归档互斥);
        std::lock_guard rootLock(根互斥);
        if (!有效(模拟世界树根节点)) { result.状态=新方法问题归档状态::未找到; return result; }
        if (!根有效()) throw std::runtime_error("模拟世界根不一致");
        const auto sample=查归档身份(task,execution);
        if (!sample) { result.状态=新方法问题归档状态::未找到; return result; }
        auto data=std::make_shared<const 新方法问题反馈材料>(读归档材料(*sample,true));
        if (data->任务节点!=task || data->执行实例节点!=execution)
            throw std::runtime_error("归档查询来源不一致");
        result.归档节点=sample; result.材料=std::move(data); result.状态=新方法问题归档状态::已保存;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询方法问题归档", L"归档身份查询或完整读回失败", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询方法问题归档", L"未知归档工程故障");
    }
    return result;
}

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
        s.集合特征 = 建样例节点(批, 特征种类, s.场景);
        s.集合材料 = 建样例节点(批, 材料种类, s.场景);
        批.引用(s.集合特征, 字段组[所属], s.自我);
        批.引用(s.集合特征, 字段组[特征类型], base.服务任务集合类型根);
        批.引用(s.集合特征, 字段组[当前材料], s.集合材料);
        批.引用(s.集合材料, 字段组[所属], s.集合特征);
        批.原值(s.集合材料, 字段组[集合成员], 基础值{std::vector<std::uint64_t>{s.任务.值}});
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
            || !属于样例(s.集合特征, 特征种类, s.场景)
            || !属于样例(s.集合材料, 材料种类, s.场景)
            || 读引用(s.集合特征, 所属) != s.自我
            || 读引用(s.集合特征, 特征类型) != s.材料.服务任务集合类型根
            || 读引用(s.集合特征, 当前材料) != s.集合材料
            || 读引用(s.集合材料, 所属) != s.集合特征
            || 读引用(s.自我, 所属) != s.场景 || 读引用(s.服务特征, 所属) != s.自我
            || 读引用(s.服务特征, 特征类型) != s.材料.服务值类型根
            || 读引用(s.服务特征, 当前材料) != s.服务材料 || 读引用(s.服务材料, 所属) != s.服务特征
            || 读引用(s.任务, 虚拟引用) != s.虚拟 || 读引用(s.虚拟, 任务引用) != s.任务
            || 读引用(s.虚拟, 状态特征引用) != s.状态特征 || 读引用(s.状态特征, 所属) != s.虚拟
            || 读引用(s.状态特征, 特征类型) != s.材料.任务状态类型根
            || 读引用(s.状态特征, 当前材料) != s.状态材料 || 读引用(s.状态材料, 所属) != s.状态特征) return std::nullopt;
        const auto v = 读值(s.服务材料, 数值), t = 读值(s.状态材料, 数值);
        if (!v || !t) return std::nullopt;
        if (读归档值<std::vector<std::uint64_t>>(s.集合材料, 集合成员)
            != std::vector<std::uint64_t>{s.任务.值}) return std::nullopt;
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
            {s.任务,s.虚拟,s.状态特征},*v,*t,s.动作,s.前状态,s.后状态,s.动态,s.集合特征,s.集合材料};
    } catch (...) { return std::nullopt; }
}
稳定编码 新_模拟世界类::模拟根节点() const noexcept {
    const auto r = 读取样例(); return r ? r->模拟根节点 : 稳定编码{};
}
std::optional<std::vector<新方法参数角色绑定>> 新_模拟世界类::读取参数绑定() const noexcept {
    try {
        const auto r = 读取样例(); if (!r) return std::nullopt;
        return std::vector<新方法参数角色绑定>{
            {实现_->材料.服务值类型根,r->场景节点,r->自我节点,{{r->服务特征节点},{r->集合特征节点}},std::nullopt},
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
        else if (n == r->集合特征节点) { f.特征概念节点 = 实现_->材料.服务任务集合类型根; f.当前值 = r->集合材料节点; }
        else return std::nullopt;
        return f;
    } catch (...) { return std::nullopt; }
}
std::optional<新特征值信息> 新_模拟世界类::读取模拟特征值材料(稳定编码 节点) const noexcept {
    try {
        if (!实现_) return std::nullopt;
        std::lock_guard 锁(实现_->互斥);
        const auto r = 读取样例();
        if (!r || r->集合材料节点 != 节点) return std::nullopt;
        return 新特征值信息{节点, 材料物理类型::U64数组,
            读归档值<std::vector<std::uint64_t>>(节点, 集合成员)};
    } catch (...) { return std::nullopt; }
}
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
