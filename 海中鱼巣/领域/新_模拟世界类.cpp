#include "新_模拟世界类.h"
#include "世界挂接.全局根.h"
#include "新_方法类.h"
#include "新_本能函数管理类.h"
#include "消息连接.规则.h"
#include "新_语言类.h"
#include "动作概念.只读投影合同.h"
#include "概念_动态类.h"
#include "新_动态类.h"
#include "新_存在类.h"
#include "../核心/容错检查.h"
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
#include "../核心/日志系统.h"
#include <sstream>
#endif
#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <mutex>
#include <map>
#include <new>
#include <stdexcept>

namespace 海中鱼巣 {
稳定编码 模拟世界树根节点{};
thread_local int 模拟样例拒绝行=0;
namespace {
enum 字段 : std::size_t { 类型, 根, 所属, 特征类型, 当前材料, 数值, 任务引用, 虚拟引用,
    状态特征引用, 方法引用, 动作引用, 前状态引用, 后状态引用, 动态引用, 时间语义, 步骤,
    集合成员, 历史引用, 正文引用, 方向值, 来源引用, 对端引用, 窗口引用, 接收引用, 发送引用,
    消息宿主,消息窗口标识,消息来源序号,
    调用主动作引用,调用阶段成员,调用传输成员,阶段所属调用,阶段种类值,阶段目标位置值,
    阶段目标特征引用,调用窗口状态值,调用取消值,调用进展值,提交序号值,传输状态值,
    队列特征引用,输出消息引用,入列状态引用,入列动态引用,领取状态引用,领取动态引用,
    发送状态引用,发送动态引用,过程收先值,方法直接写入值,传输发送消息引用,接收状态引用,
    内容概念引用,附加概念引用,主体引用,目标引用,目标形式值,目标域引用,目标比较引用,目标方向值,
    执行域值,权重值,活动值,父需求引用,根角色值,来源类型值,理解引用,确认引用,要求引用,
    外部要求引用,服务确认引用,语言场景引用,会话值,要求标识值,需重新确认值,固定语义引用,
    场景成员引用,场景边界引用,场景特征引用,实例存在引用,实例概念引用,实例种类值,
    结构类别值,结构材料值,结构引用,结构子项,结构顺序值,结构单位引用,
    完整实例区间引用,完整聚合引用,标准值引用,历史项引用,历史命中引用,命中次数值,
    需求发布成员,发布需求引用,首次发布动作引用,首次发布记录引用,
    引用内容需求引用,引用内容路径引用,完善阶段值,询问入列成员,字段数 };
enum 节点种类 : std::int64_t { 根种类 = 1, 场景种类, 存在种类, 特征种类, 材料种类,
    任务种类, 虚拟种类, 动作种类, 状态种类, 动态种类,
    消息种类, 过程种类,
    调用种类,阶段来源种类,传输种类,需求种类,理解种类,确认种类,要求种类,
    语言场景种类,语义快照种类,语言实例关联种类,需求发布种类,队列入列种类 };
std::array<稳定编码, 字段数> 字段组{};
std::mutex 根互斥;
void 合并模拟联合预期(基础节点组追加请求& q) {
    std::map<稳定编码,基础联合节点快照> merged;
    for(const auto& p:q.预期联合节点组) {
        const auto [it,added]=merged.emplace(p.结构.节点.编码,p);
        if(!added&&(it->second.结构.字段组!=p.结构.字段组||it->second.入向字段组!=p.入向字段组
            ||it->second.入向外部关系组!=p.入向外部关系组||it->second.出向外部关系组!=p.出向外部关系组))
            throw std::runtime_error("simulation input definition premise changed");
    }
    q.预期联合节点组.clear();for(auto& [n,p]:merged)q.预期联合节点组.push_back(std::move(p));
}
// 外部边及字段替换/删除必须保护既有端点的完整联合结构。
void 补齐模拟追加联合预期(基础节点组追加请求& q) {
    std::vector<稳定编码> 节点;
    for (const auto& s : q.预期既有节点组) 节点.push_back(s.节点.编码);
    for (const auto& e : q.追加外部关系组) {
        if (const auto* n = std::get_if<稳定编码>(&e.源节点)) 节点.push_back(*n);
        if (const auto* n = std::get_if<稳定编码>(&e.目标节点)) 节点.push_back(*n);
    }
    for (const auto& s : q.预期既有节点组) for (const auto& f : s.字段组)
        if (const auto* n = std::get_if<稳定编码>(&f.内容)) 节点.push_back(*n);
    std::sort(节点.begin(),节点.end());
    节点.erase(std::unique(节点.begin(),节点.end()),节点.end());
    auto r = 全局基础数据集.查询节点组完整结构(节点);
    if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 节点.size())
        throw std::runtime_error("simulation joint snapshot read failed");
    for (auto& x : r.节点组) {
        if (!x.快照) throw std::runtime_error("simulation joint snapshot missing");
        q.预期联合节点组.push_back(std::move(*x.快照));
    }
    合并模拟联合预期(q);
}
void 补齐隔离追加联合预期(基础节点组追加请求& q) {
    std::set<稳定编码> nodes;
    const auto field=[&](稳定编码 key,const 基础组内字段内容& value) {
        nodes.insert(key);if(const auto* n=std::get_if<稳定编码>(&value))nodes.insert(*n);
    };
    for(const auto& n:q.新节点组)for(const auto& f:n.字段组)field(f.字段节点,f.内容);
    for(const auto& f:q.追加字段组){nodes.insert(f.所属节点);field(f.字段节点,f.内容);}
    const auto existing=[&](稳定编码 id) {
        const auto f=全局基础数据集.查询字段关系(id);
        if(!f)throw std::runtime_error("isolation update field missing");
        nodes.insert(f->所属节点);nodes.insert(f->字段节点);
        if(const auto* n=std::get_if<稳定编码>(&f->内容))nodes.insert(*n);
    };
    for(const auto& f:q.替换字段组){existing(f.字段关系);if(const auto* n=std::get_if<稳定编码>(&f.新内容))nodes.insert(*n);}
    for(const auto f:q.删除字段关系组)existing(f);
    for(const auto& e:q.追加外部关系组) {
        if(const auto* n=std::get_if<稳定编码>(&e.源节点))nodes.insert(*n);
        if(const auto* n=std::get_if<稳定编码>(&e.目标节点))nodes.insert(*n);
    }
    auto r=全局基础数据集.查询节点组完整结构({nodes.begin(),nodes.end()});
    if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=nodes.size())throw std::runtime_error("isolation premise capture failed");
    for(auto& n:r.节点组){if(!n.快照)throw std::runtime_error("isolation endpoint missing");q.预期联合节点组.push_back(std::move(*n.快照));}
    合并模拟联合预期(q);
}

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
        // 只消费已建成的有效身份；底座 noexcept 创建失败以无效编码返回。
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
    auto 组 = 全局基础数据集.查询字段(n, 字段组[f]);
    if (组.size() != 1) return std::nullopt;
    return std::move(组.front());
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
    if (有效(模拟世界树根节点))
        return 根有效()
            && 新_全局根类::查询世界根(世界类型登记键::模拟) == 模拟世界树根节点;
    创建批 批;
    const auto n = 批.建节点();
    std::array<稳定编码, 字段数> 新字段{};
    for (auto& f : 新字段) { f = 批.建节点(); 批.挂接(n, f); }
    批.值(n, 新字段[类型], 根种类);
    // 模拟世界按需建立；挂接失败时由批回收新根，不留下未挂接的世界根。
    const auto 全局根 = 新_全局根类::初始化();
    if (全局根 != 全局根操作状态::已完成 && 全局根 != 全局根操作状态::无变化) return false;
    const auto 挂接 = 新_全局根类::挂接世界根(世界类型登记键::模拟, n);
    if (挂接 != 全局根操作状态::已完成) return false;
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
std::vector<稳定编码> 读引用组(稳定编码 n,字段 key) {
    std::vector<稳定编码> out;
    for(const auto& f:全局基础数据集.查询字段(n,字段组[key])) {
        const auto* ref=std::get_if<稳定编码>(&f.内容);
        if(!ref||!全局基础数据集.查询节点(*ref))throw std::runtime_error("simulation typed reference list corrupt");
        out.push_back(*ref);
    }return out;
}
template<class T>T 读结构材料(稳定编码 n,字段 key) {
    auto f=唯一字段(n,key);auto* raw=f?std::get_if<基础值>(&f->内容):nullptr;
    auto* x=raw?std::get_if<T>(&raw->材料):nullptr;
    if(!x)throw std::runtime_error("simulation typed material corrupt");return std::move(*x);
}
稳定编码 必读引用(稳定编码 n,字段 key) {
    const auto x=读引用(n,key);if(!x)throw std::runtime_error("simulation typed mandatory reference missing");return *x;
}
std::int64_t 必读数(稳定编码 n,字段 key) {
    const auto x=读值(n,key);if(!x)throw std::runtime_error("simulation typed mandatory number missing");return *x;
}
稳定编码 建样例节点(创建批& 批, 节点种类 k, 稳定编码 scene) {
    const auto n = 批.建节点();
    批.值(n, 字段组[类型], k); 批.引用(n, 字段组[根], 模拟世界树根节点);
    批.挂接(scene, n); return n;
}
// 仅为本world的组内结构编解码；所有引用由L1保留类型，不序列化成裸编码字串。
struct 模拟结构写入器 final {
    基础节点组追加请求& q;
    std::size_t 场景;
    std::size_t 节点(std::int64_t subtype=0) {
        const auto n=q.新节点组.size();
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(语义快照种类)}},
            {字段组[根],模拟世界树根节点},{字段组[结构类别值],基础值{subtype}}}});
        q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{n},基础外部关系类型::父子});return n;
    }
    void 写(std::size_t n,字段 key,基础组内字段内容 v){q.新节点组.at(n).字段组.push_back({字段组[key],std::move(v)});}
    void 数(std::size_t n,字段 key,std::int64_t x){写(n,key,基础值{x});}
    void 子(std::size_t n,字段 key,std::size_t x){写(n,key,基础组内节点引用{x});}
    std::size_t 域(const 特征概念值域& d) {
        const auto n=节点(static_cast<std::int64_t>(d.index()+1));
        std::visit([&](const auto& value) {
            using T=std::decay_t<decltype(value)>;
            if constexpr(std::is_same_v<T,std::vector<特征概念I64闭区间>>) {
                std::vector<std::int64_t> a;for(const auto& r:value){a.push_back(r.下界);a.push_back(r.上界);}
                写(n,结构材料值,基础值{a});
            } else if constexpr(std::is_same_v<T,std::vector<稳定编码>>) {
                for(const auto ref:value)写(n,结构引用,ref);
            } else if constexpr(std::is_same_v<T,特征概念结构化I64值域>) {
                for(const auto& c:value.分量) {
                    const auto x=节点();写(x,结构引用,c.角色);if(c.单位)写(x,结构单位引用,*c.单位);
                    子(x,结构子项,域(特征概念值域{c.值域}));子(n,结构子项,x);
                }
            } else if constexpr(std::is_same_v<T,特征概念存在引用值域>)写(n,结构引用,value.存在概念节点);
            else if constexpr(std::is_same_v<T,特征概念信息引用值域>) {
                写(n,结构引用,value.内容概念);数(n,结构顺序值,static_cast<std::int64_t>(value.语义));
            } else if constexpr(std::is_same_v<T,特征概念任务集合值域>) {
                if(!value.必含任务节点组.empty()||!value.必含需求任务组.empty()||!value.必含服务要求任务组.empty())
                    throw std::invalid_argument("task domain instance closure is unsupported");
                // R102：本地语义快照结构未承载概念成员/概念子集，显式拒绝而非静默丢弃。
                if(!value.含概念成员组.empty()||!value.全部属于概念组.empty())
                    throw std::invalid_argument("set domain concept members are unsupported in local snapshot");
                数(n,结构顺序值,value.至少一个任务);
            } else if constexpr(std::is_same_v<T,特征概念需求集合值域>) {
                if(!value.含概念成员组.empty()||!value.全部属于概念组.empty())
                    throw std::invalid_argument("set domain concept members are unsupported in local snapshot");
                for(const auto x:value.必含需求节点组)写(n,结构引用,x);数(n,结构顺序值,value.至少一个需求);
            } else if constexpr(std::is_same_v<T,特征概念存在集合值域>) {
                if(!value.含概念成员组.empty()||!value.全部属于概念组.empty())
                    throw std::invalid_argument("set domain concept members are unsupported in local snapshot");
                for(const auto x:value.必含存在节点组)写(n,结构引用,x);数(n,结构顺序值,value.至少一个存在);
            }
        },d);return n;
    }
    std::size_t 特征(const 服务要求特征语义快照& f) {
        const auto n=节点();写(n,结构引用,f.概念节点);
        写(n,结构材料值,基础值{std::vector<std::int64_t>{static_cast<std::int64_t>(f.材料类型),static_cast<std::int64_t>(f.比较规则)}});
        if(f.单位)写(n,结构单位引用,*f.单位);return n;
    }
    std::size_t 语义域(const 服务要求值域语义快照& f) {
        const auto n=节点();子(n,特征类型,特征(f.特征语义));子(n,结构子项,域(f.值域));return n;
    }
    std::size_t 语义(const 服务要求目标语义快照& f) {
        const auto n=节点();子(n,特征类型,特征(f.目标类型));
        if(f.目标域)子(n,目标域引用,语义域(*f.目标域));
        if(f.比较) {
            const auto& b=*f.比较;const auto x=节点();
            写(x,目标比较引用,b.比较概念节点);写(x,特征类型,b.类型根节点);
            数(x,结构类别值,static_cast<std::int64_t>(b.算法));
            // 节点默认类别已存在，替换为唯一算法字段。
            q.新节点组[x].字段组.erase(q.新节点组[x].字段组.begin()+2);
            if(b.参照概念节点)写(x,目标域引用,*b.参照概念节点);
            子(x,固定语义引用,语义域(b.输出语义));
            for(const auto& in:b.输入定义组) {
                const auto v=节点();数(v,结构顺序值,in.顺序);
                写(v,结构材料值,基础值{std::vector<std::int64_t>{static_cast<std::int64_t>(in.角色),static_cast<std::int64_t>(in.来源种类)}});
                if(in.特征类型概念节点)写(v,结构引用,*in.特征类型概念节点);子(x,结构子项,v);
            }
            std::vector<std::int64_t> kinds;for(const auto k:b.结果域种类组)kinds.push_back(static_cast<std::int64_t>(k));
            写(x,结构材料值,基础值{kinds});子(n,目标比较引用,x);
        }return n;
    }
};
特征概念值域 读结构域(稳定编码 n) {
    const auto tag=必读数(n,结构类别值);
    switch(tag) {
    case 1:{auto a=读结构材料<std::vector<std::int64_t>>(n,结构材料值);if(a.size()%2)throw std::runtime_error("odd range payload");
        std::vector<特征概念I64闭区间> out;for(std::size_t i=0;i<a.size();i+=2){if(a[i]>a[i+1])throw std::runtime_error("reversed local range");out.push_back({a[i],a[i+1]});}return out;}
    case 2:return 特征概念全材料值域{};
    case 3:return 读引用组(n,结构引用);
    case 4:{特征概念结构化I64值域 out;for(const auto c:读引用组(n,结构子项)) {
        const auto domain=读结构域(必读引用(c,结构子项));const auto* ranges=std::get_if<std::vector<特征概念I64闭区间>>(&domain);
        if(!ranges)throw std::runtime_error("structured component domain corrupt");
        out.分量.push_back({必读引用(c,结构引用),读引用(c,结构单位引用),*ranges});}return out;}
    case 5:return 特征概念存在引用值域{必读引用(n,结构引用)};
    case 6:{特征概念任务集合值域 d;d.至少一个任务=必读数(n,结构顺序值)!=0;return d;}
    case 7:return 特征概念需求集合值域{读引用组(n,结构引用),必读数(n,结构顺序值)!=0};
    case 8:return 特征概念存在集合值域{读引用组(n,结构引用),必读数(n,结构顺序值)!=0};
    case 9:return 特征概念信息引用值域{static_cast<信息引用语义>(必读数(n,结构顺序值)),必读引用(n,结构引用)};
    default:throw std::runtime_error("local domain kind corrupt");
    }
}
服务要求特征语义快照 读结构特征语义(稳定编码 n) {
    const auto a=读结构材料<std::vector<std::int64_t>>(n,结构材料值);
    if(a.size()!=2)throw std::runtime_error("local feature semantics shape corrupt");
    return {必读引用(n,结构引用),static_cast<特征概念材料物理类型>(a[0]),读引用(n,结构单位引用),static_cast<特征概念比较规则>(a[1])};
}
服务要求值域语义快照 读结构域语义(稳定编码 n) {
    return {读结构特征语义(必读引用(n,特征类型)),读结构域(必读引用(n,结构子项))};
}
服务要求目标语义快照 读结构目标语义(稳定编码 n) {
    服务要求目标语义快照 out{读结构特征语义(必读引用(n,特征类型))};
    if(const auto d=读引用(n,目标域引用))out.目标域=读结构域语义(*d);
    if(const auto b=读引用(n,目标比较引用)) {
        服务要求比较语义快照 x;x.比较概念节点=必读引用(*b,目标比较引用);x.类型根节点=必读引用(*b,特征类型);
        x.算法=static_cast<二次特征算法>(必读数(*b,结构类别值));x.参照概念节点=读引用(*b,目标域引用);
        x.输出语义=读结构域语义(必读引用(*b,固定语义引用));
        for(const auto in:读引用组(*b,结构子项)) {
            const auto a=读结构材料<std::vector<std::int64_t>>(in,结构材料值);if(a.size()!=2)throw std::runtime_error("local compare input corrupt");
            x.输入定义组.push_back({必读数(in,结构顺序值),static_cast<二次特征输入角色>(a[0]),static_cast<二次特征输入来源种类>(a[1]),读引用(in,结构引用)});
        }
        for(const auto k:读结构材料<std::vector<std::int64_t>>(*b,结构材料值))x.结果域种类组.push_back(static_cast<二次特征结果域种类>(k));
        out.比较=std::move(x);
    }return out;
}
}

struct 新_模拟世界类::实现 final {
    新_方法类& 方法;
    const 新_二次特征类* 二次计算=nullptr;
    mutable std::recursive_mutex 互斥;
    const 动作方法定义只读提供者* 动作定义读取=nullptr;
    新_动态类* 动作定义发布=nullptr;
    const 概念_动态类* 动作概念=nullptr;
    const 新方法场景事实构成读取接口* 场景构成=nullptr;
    const 新_存在类* 源身份=nullptr;
    const 概念_存在类* 存在概念=nullptr;
    const 概念_特征类* 特征概念=nullptr;
    const 概念_二次特征类* 二次概念=nullptr;
    std::shared_ptr<const 新字段询问语言接口> 字段询问语言;
    std::optional<新字段询问登记配置> 字段询问配置;
    std::optional<新模拟执行主体输入> 执行主体来源;
    std::optional<稳定编码> 本地执行主体;
    mutable std::optional<稳定编码> 需求动作许可;
    std::optional<稳定编码> 当前调用;
    std::function<bool()> 停止检查;
    bool 请求取消=false;
    bool 通用=false;
    稳定编码 通用方法,通用场景;
    std::optional<稳定编码> 通用执行分支;
    std::vector<新方法参数角色绑定> 通用绑定;
    std::vector<稳定编码> 通用输入节点; // 全部有序输入（含N之后私有追加输入）的本地节点。
    std::map<稳定编码,新模拟有序参数输入> 通用参数;
    bool 是本地参数(稳定编码 n,新方法参数信息类型 kind) const {
        const auto it=通用参数.find(n);return it!=通用参数.end()&&it->second.规格.信息类型==kind;
    }
    bool 是普通存在参数(稳定编码 n) const {
        const auto it=通用参数.find(n);
        return it!=通用参数.end()&&it->second.规格.信息类型==新方法参数信息类型::存在
            &&!it->second.需求参数输入&&!本地需求.contains(n);
    }
    std::map<稳定编码,材料物理类型> 通用材料;
    // 仅身份/来源索引；业务内容始终从L1字段完整重建。
    std::map<稳定编码,std::optional<稳定编码>> 本地需求,本地理解,本地确认,本地要求,本地语言场景;
    std::map<稳定编码,std::vector<std::uint32_t>> 内容参数位置;
    mutable std::vector<新模拟内容读取记录> 内容读取组;
    template<class T> void 记录内容(std::uint8_t kind,稳定编码 n,const T& value,bool copied) {
        if(!记录复制读取)return;
        // 有类型的读取由学习侧逐条覆盖；该标志只保留给无正式表达的隐藏读取。
        (void)copied;
        内容读取组.push_back({kind,n,内容参数位置[n],value,!本次已进入动作});
    }
    struct 任务投影 final { 稳定编码 来源; 新任务虚拟存在特征组 本地; };
    std::map<稳定编码,任务投影> 通用任务;
    std::vector<稳定编码> 通用节点,通用字段,通用外部边;
    std::vector<新模拟输入事件> 通用事件;
    struct 消息事件 final {
        新方法模拟消息输入定义 声明;
        std::string 正文;
        bool 已交付=false;
        std::set<稳定编码> 触发基线;
    };
    std::vector<消息事件> 消息事件组;
    新_方法类::执行结构读取结果 消息定义保护;
    std::uint64_t 消息序号=0;
    std::size_t 事件游标=0;
    std::int64_t 模拟步骤=0;
    bool 记录复制读取=false,已读取复制上下文=false;
    bool 本次已进入动作=false;
    std::vector<新本能函数条件角色材料> 本次存在特征读取;
    enum class 写入来源 : std::uint8_t { 未写入, 方法, 环境 };
    bool 记录写回 = false, 写回窗口已关闭 = false;
    std::map<稳定编码, 写入来源> 最后写入来源组;
    std::vector<新模拟任务成员读取> 任务成员读取组;
    struct 场景事实 final {
        新模拟场景事实输入 输入;
        新交互连接过程 过程;
        std::vector<基础联合节点快照> 固定节点;
    };
    std::vector<场景事实> 交互事实;
    std::vector<新方法初始场景事实限定> 本次场景读取;
    void 追加进展(基础节点组追加请求& q) {
        if(!记录写回)return;
        if(!当前调用)throw std::runtime_error("isolation progress without current call");
        const auto f=唯一字段(*当前调用,调用进展值);const auto n=读值(*当前调用,调用进展值);
        if(!f||!n||*n<0||*n==(std::numeric_limits<std::int64_t>::max)())throw std::runtime_error("isolation progress corrupt or exhausted");
        q.替换字段组.push_back({f->编码,基础值{*n+1}});
    }
    void 预留追加(const 基础节点组追加请求& q) {
        通用节点.reserve(通用节点.size()+q.新节点组.size()+1);
        std::size_t fields=q.追加字段组.size();for(const auto& n:q.新节点组)fields+=n.字段组.size();
        通用字段.reserve(通用字段.size()+fields);通用外部边.reserve(通用外部边.size()+q.追加外部关系组.size());
    }
    写入来源* 准备写回槽(稳定编码 特征) {
        if (!记录写回) return nullptr;
        const auto 槽 = 最后写入来源组.find(特征);
        if (槽 == 最后写入来源组.end())
            throw std::runtime_error("simulation write target has no call-local attribution slot");
        return &槽->second;
    }
    void 收集追加(const 基础节点组追加结果& p) {
        通用节点.insert(通用节点.end(),p.新节点组.begin(),p.新节点组.end());
        通用字段.insert(通用字段.end(),p.追加字段关系组.begin(),p.追加字段关系组.end());
        for(const auto& g:p.新节点初始字段关系组)
            通用字段.insert(通用字段.end(),g.begin(),g.end());
        通用外部边.insert(通用外部边.end(),p.追加外部关系组.begin(),p.追加外部关系组.end());
    }

    explicit 实现(新_方法类& m) : 方法(m) {}
};
新_模拟世界类::新_模拟世界类(新_方法类& m) noexcept {
    try { 实现_ = std::make_unique<实现>(m); } catch (...) {}
}
新_模拟世界类::新_模拟世界类(新_方法类& m,const 动作方法定义只读提供者& p,
    新_动态类& 发布,const 概念_动态类& 概念) noexcept : 新_模拟世界类(m) {
    if(实现_){实现_->动作定义读取=&p;实现_->动作定义发布=&发布;实现_->动作概念=&概念;}
}
新_模拟世界类::新_模拟世界类(新_方法类& m,const 动作方法定义只读提供者& p,
    新_动态类& publish,const 概念_动态类& c,const 新方法场景事实构成读取接口& facts) noexcept : 新_模拟世界类(m,p,publish,c) {
    if(实现_)实现_->场景构成=&facts;
}
void 新_模拟世界类::配置二次特征计算(const 新_二次特征类& calculator) {
    if(!实现_)throw std::runtime_error("simulation world unavailable");
    std::lock_guard lock(实现_->互斥);
    if((实现_->二次计算&&实现_->二次计算!=&calculator)||(!实现_->二次计算&&实现_->通用))
        throw std::logic_error("simulation comparison provider already in use");
    实现_->二次计算=&calculator;
}
void 新_模拟世界类::配置需求执行读取(const 新_存在类& 源身份,const 概念_存在类& 存在,
    const 概念_特征类& 特征,const 概念_二次特征类& 二次) {
    if(!实现_)throw std::runtime_error("simulation owner unavailable");
    auto& 内部=*实现_;std::lock_guard 锁(内部.互斥);
    if(内部.通用||(内部.源身份&&内部.源身份!=&源身份)||(内部.存在概念&&内部.存在概念!=&存在)
        ||(内部.特征概念&&内部.特征概念!=&特征)||(内部.二次概念&&内部.二次概念!=&二次))
        throw std::logic_error("simulation demand read provider conflict");
    内部.源身份=&源身份;内部.存在概念=&存在;内部.特征概念=&特征;内部.二次概念=&二次;
}
void 新_模拟世界类::设置字段询问语言(std::shared_ptr<const 新字段询问语言接口> 语言,
    std::optional<新字段询问登记配置> 配置) {
    if(!实现_)throw std::runtime_error("simulation owner unavailable");
    if(static_cast<bool>(语言)!=配置.has_value())
        throw std::invalid_argument("simulation question dependencies must be paired");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(s.通用||s.记录写回)throw std::logic_error("simulation question dependencies are in use");
    if(语言&&s.字段询问语言&&(语言!=s.字段询问语言||配置!=s.字段询问配置))
        throw std::logic_error("simulation question dependency conflict");
    s.字段询问语言=std::move(语言);s.字段询问配置=std::move(配置);
}
新本能字段询问运行接口* 新_模拟世界类::获取字段询问运行接口() noexcept {
    if(!实现_)return nullptr;
    std::lock_guard lock(实现_->互斥);
    return 实现_->通用&&实现_->字段询问语言&&实现_->字段询问配置 ? this : nullptr;
}
新字段询问材料读回 新_模拟世界类::读取询问材料(const 方法参数场景& scene) const {
    if(!实现_)throw std::runtime_error("simulation question owner unavailable");
    新字段询问登记配置 配置;
    {
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||!s.字段询问语言||!s.字段询问配置)
            throw std::logic_error("simulation question dependencies unavailable");
        if(scene.用途()!=新方法参数场景用途::模拟||scene.获取模拟运行接口()!=this
            ||scene.方法节点()!=s.通用方法||scene.全部角色绑定()!=s.通用绑定)
            throw std::invalid_argument("simulation question scene owner mismatch");
        配置=*s.字段询问配置;
    }
    return 读取通用询问材料(scene,配置,*this);
}
新字段询问文字结果 新_模拟世界类::生成询问文字(const 新正式字段询问目标& 目标) const {
    if(!实现_)throw std::runtime_error("simulation question owner unavailable");
    std::shared_ptr<const 新字段询问语言接口> 语言;
    {
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||!s.字段询问语言||!s.字段询问配置)
            throw std::logic_error("simulation question dependencies unavailable");
        语言=s.字段询问语言;
    }
    return 语言->生成字段询问(目标,*this);
}
稳定编码 新_模拟世界类::建立输出动作(const 方法参数场景& scene,const 新字段询问材料& 材料) {
    const auto 自我=scene.查询参数(2),窗口=scene.查询参数(4),输出=scene.查询参数(5);
    if(!自我||!窗口||!输出||自我->信息节点!=材料.自我||窗口->信息节点!=材料.窗口
        ||输出->信息节点!=材料.输出特征)
        throw std::invalid_argument("simulation question action materials differ");
    return 建立参数动作(scene,{2,4,5});
}
新出站表达留档状态 新_模拟世界类::留档出站具体表达(const 方法参数场景& scene,
    const 新字段询问材料& 材料,稳定编码 动作,const 新字段询问文字结果& 文字) {
    if(scene.用途()!=新方法参数场景用途::模拟||scene.获取模拟运行接口()!=this||!有效(动作)
        ||文字.状态!=新字段询问文字状态::已生成且唯一回读)
        throw std::invalid_argument("simulation archive call does not belong to this execution");
    (void)材料;
    // 模拟学习不构成真实交互；不在任何世界建立语言留档。
    return 新出站表达留档状态::模拟不留档;
}
新字段询问入列读回 新_模拟世界类::提交询问输出(const 方法参数场景& scene,
    const 新字段询问材料& 材料,稳定编码 动作,std::string_view 正文) {
    const auto 窗口=scene.查询参数(4),对端=scene.查询参数(3),输出=scene.查询参数(5);
    if(!窗口||!对端||!输出||窗口->信息节点!=材料.窗口||对端->信息节点!=材料.受询者
        ||输出->信息节点!=材料.输出特征)
        throw std::invalid_argument("simulation question output materials differ");
    const auto r=提交本地输出队列(scene,{4,3,5,动作,std::string(正文)});
    return {r.输出消息,r.入列状态,r.入列动态,r.提交序号};
}
新_模拟世界类::~新_模拟世界类() {
    try { 释放有序样例(); }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"释放模拟样例",L"临时模拟对象仍被引用或释放失败。"); }
}


稳定编码 新_模拟世界类::模拟根节点() const noexcept {

    if(实现_) { std::lock_guard lock(实现_->互斥);
        if(实现_->通用) return 模拟世界树根节点; }

    return {};
}
std::optional<稳定编码> 新_模拟世界类::读取有序输入节点(std::uint32_t 位置) const noexcept {
    try {
        if(!实现_)return std::nullopt;
        std::lock_guard lock(实现_->互斥);
        if(!实现_->通用||!位置||位置>实现_->通用输入节点.size())return std::nullopt;
        const auto n=实现_->通用输入节点[位置-1];
        if(!n.值||!全局基础数据集.查询节点(n))return std::nullopt;
        return n;
    } catch (...) { return std::nullopt; }
}
std::optional<std::vector<新方法参数角色绑定>> 新_模拟世界类::读取参数绑定() const noexcept {
    try {
        if(实现_) { std::lock_guard lock(实现_->互斥);
            if(实现_->通用) {
                for(const auto& b:实现_->通用绑定)
                    if(!b.有序引用||!全局基础数据集.查询节点(b.有序引用->信息节点)) return {};
                return 实现_->通用绑定;
            }
        }

    } catch (...) { return {}; }
    return std::nullopt;
}
bool 新_模拟世界类::核验模拟参数(稳定编码 method, const std::vector<新方法参数角色绑定>& b) const noexcept {
    if(!实现_)return false;
    std::lock_guard lock(实现_->互斥);
    const auto 绑定 = 读取参数绑定(); return 绑定 && 实现_->通用 && 实现_->通用方法 == method && *绑定 == b;
}
std::optional<新特征信息> 新_模拟世界类::读取模拟特征(稳定编码 n) const noexcept {
    return 读取模拟特征内部(n,false);
}
新方法参数特征读取结果 新_模拟世界类::读取模拟存在参数特征(
    std::uint32_t 位置,稳定编码 FT) const noexcept {
    using S=新方法参数特征读取状态;
    try {
        if(!实现_)throw std::logic_error("simulation instance feature owner unavailable");
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||!位置||位置>s.通用绑定.size())return {S::参数未绑定,{},{}};
        const auto& b=s.通用绑定[位置-1];
        if(!b.有序引用||b.有序引用->信息类型!=新方法参数信息类型::存在)
            return {S::信息类型不符,{},{}};
        if(!s.是普通存在参数(b.有序引用->信息节点))return {S::信息类型不符,{},{}};
        const auto 声明=s.方法.读取存在参数特征声明(s.通用方法,s.通用执行分支,位置,FT);
        if(声明.状态!=S::已读取)return {声明.状态,{},{}};
        const auto E=b.有序引用->信息节点;
        std::map<稳定编码,新特征匹配定义快照> 定义组;
        std::vector<稳定编码> 命中;
        for(const auto& [F,p]:s.通用参数) {
            if(p.规格.信息类型!=新方法参数信息类型::特征||读引用(F,所属)!=std::optional{E})continue;
            const auto d=s.方法.读取模拟特征范围(p.规格.范围概念节点);
            if(!d)throw std::logic_error("simulation attached feature definition missing");
            定义组.emplace(F,*d);if(d->特征类型根==FT)命中.push_back(F);
        }
        新方法参数特征读取结果 out;
        out.状态=命中.empty() ? S::特征未取得 : S::特征匹配不唯一;
        if(命中.size()==1) {
            const auto F=命中.front();
            const auto 完整=读取模拟特征内部(F,true);
            if(!完整)throw std::logic_error("simulation attached feature unreadable");
            out.特征=*完整;
            out.状态=out.特征->当前值 ? S::已读取 : S::当前值未取得;
            if(out.特征->当前值) {
                const auto& d=定义组.at(F);
                if(d.材料类型!=特征概念材料物理类型::I64标量
                    &&d.材料类型!=特征概念材料物理类型::存在节点引用
                    &&d.材料类型!=特征概念材料物理类型::需求节点引用) {
                    const auto* V=std::get_if<稳定编码>(&*out.特征->当前值);
                    if(!V)throw std::logic_error("simulation attached material identity malformed");
                    out.当前完整材料=读取模拟特征值材料(*V);
                    if(!out.当前完整材料)throw std::logic_error("simulation attached complete material missing");
                }
            }
        }
        for(const auto& [F,d]:定义组)
            if(s.方法.读取模拟特征范围(s.通用参数.at(F).规格.范围概念节点)!=std::optional{d})
                return {S::定义前提已变化,{},{}};
        const auto 再声明=s.方法.读取存在参数特征声明(s.通用方法,s.通用执行分支,位置,FT);
        if(再声明.状态!=S::已读取||再声明.特征定义!=声明.特征定义)
            return {S::定义前提已变化,{},{}};
        if(out.特征&&s.记录复制读取&&!s.本次已进入动作) {
                const auto F=out.特征->节点;
                auto r=std::find_if(s.本次存在特征读取.begin(),s.本次存在特征读取.end(),
                    [&](const auto& x){return x.有序引用==b.有序引用;});
                if(r==s.本次存在特征读取.end()) {
                    新本能函数条件角色材料 x;x.有序引用=b.有序引用;x.世界场景节点=b.世界场景节点;
                    s.本次存在特征读取.push_back(std::move(x));r=std::prev(s.本次存在特征读取.end());
                }
                const auto old=std::find_if(r->实际特征组.begin(),r->实际特征组.end(),
                    [&](const auto& x){return x.特征节点==F;});
                if(old!=r->实际特征组.end()&&old->动作前准确值!=out.特征->当前值)
                    return {S::定义前提已变化,{},{}};
                if(old==r->实际特征组.end())r->实际特征组.push_back({F,out.特征->当前值});
        }
        return out;
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟存在参数特征",L"本地所属、定义或材料发生工程读取错误。");
    }
}
std::vector<新本能函数条件角色材料> 新_模拟世界类::读取本次存在参数特征读取组() const {
    if(!实现_)throw std::logic_error("simulation feature read trace owner unavailable");
    std::lock_guard lock(实现_->互斥);
    if(实现_->记录复制读取||!实现_->写回窗口已关闭)
        throw std::logic_error("simulation feature read trace requires closed invocation");
    return 实现_->本次存在特征读取;
}
std::optional<新特征信息> 新_模拟世界类::读取模拟特征内部(稳定编码 n,bool 正式定位) const noexcept {
    try {
        if(实现_) { auto& s=*实现_;std::lock_guard lock(s.互斥);
            if(s.通用) {
                const auto p=s.通用参数.find(n);
                if(p==s.通用参数.end()||p->second.规格.信息类型!=新方法参数信息类型::特征) return {};
                新特征信息 f;f.节点=n;f.特征概念节点=p->second.规格.范围概念节点;
                const auto def=s.方法.读取模拟特征范围(f.特征概念节点);
                if(!def) return {};
                if(s.记录复制读取&&p->second.复制上下文&&!正式定位
                    &&(def->材料类型!=特征概念材料物理类型::需求节点引用||!s.内容参数位置.contains(n)))
                    s.已读取复制上下文=true; // 本叶只为已映射需求F提供完整前态覆盖，其它复制读取保留旧门。
                if(p->second.完整复制特征)f.实例单位=读引用(n,结构单位引用);
                else f.实例单位=def->单位;
                if(const auto v=读值(n,数值)) f.当前值=*v;
                else if(const auto v=读引用(n,当前材料)) f.当前值=*v;
                for(const auto& h:全局基础数据集.查询字段(n,字段组[历史引用])) {
                    const auto* ref=std::get_if<稳定编码>(&h.内容);
                    if(!ref)throw std::runtime_error("simulation history reference malformed");
                    f.历史不同值.emplace_back(*ref);
                }
                const auto accurate=[](const 基础字段内容& value)->新特征准确值 {
                    if(const auto* ref=std::get_if<稳定编码>(&value))return *ref;
                    const auto* raw=std::get_if<基础值>(&value);const auto* x=raw?std::get_if<std::int64_t>(&raw->材料):nullptr;
                    if(!x)throw std::runtime_error("copied accurate feature value malformed");return *x;
                };
                const auto standards=全局基础数据集.查询字段(n,字段组[标准值引用]);
                if(standards.size()>1)throw std::runtime_error("multiple local standard feature values");
                if(!standards.empty())f.标准值=accurate(standards.front().内容);
                for(const auto& h:全局基础数据集.查询字段(n,字段组[历史项引用]))f.历史不同值.push_back(accurate(h.内容));
                std::vector<新特征准确值> 去重历史;
                for(const auto& 值:f.历史不同值)
                    if(std::find(去重历史.begin(),去重历史.end(),值)==去重历史.end())去重历史.push_back(值);
                f.历史不同值=std::move(去重历史);
                for(const auto hit:读引用组(n,历史命中引用)) {
                    const auto v=唯一字段(hit,结构引用);if(!v)throw std::runtime_error("local history hit value missing");
                    const auto count=必读数(hit,命中次数值);if(count<0)throw std::runtime_error("local history hit count invalid");
                    f.历史值命中信息.push_back({accurate(v->内容),count});
                }
                if(const auto domain=读引用(n,完整实例区间引用))f.实例区间值=读结构域(*domain);
                if(const auto domain=读引用(n,完整聚合引用))f.实例历史聚合值域=读结构域(*domain);
                s.记录内容(6,n,f,p->second.复制上下文);
                return f;
            }
        }

    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取完整模拟特征",L"本地完整特征结构损坏，禁止解释为当前未知。"); }
    return std::nullopt;
}
std::optional<新特征值信息> 新_模拟世界类::读取模拟特征值材料(稳定编码 节点) const noexcept {
    try {
        if (!实现_) return std::nullopt;
        std::lock_guard 锁(实现_->互斥);

        if(实现_->通用) {
            const auto k=实现_->通用材料.find(节点);
            if(k==实现_->通用材料.end())return {};
            auto f=唯一字段(节点,数值);
            auto* v=f?std::get_if<基础值>(&f->内容):nullptr;
            if(!v)throw std::runtime_error("registered local material payload missing");
            新特征值信息 r;r.节点=节点;r.物理类型=k->second;
            if(auto* a=std::get_if<std::vector<std::uint64_t>>(&v->材料))r.材料=std::move(*a);
            else if(auto* a=std::get_if<std::vector<std::int64_t>>(&v->材料))r.材料=std::move(*a);
            else if(auto* a=std::get_if<std::string>(&v->材料))r.材料=std::move(*a);
            else throw std::runtime_error("registered local material payload category corrupt");
            if(实现_->记录复制读取) {
                for(const auto& [F,p]:实现_->通用参数)if(p.规格.信息类型==新方法参数信息类型::特征) {
                    实现_->记录复制读取=false;
                    const auto f=读取模拟特征内部(F,false);
                    实现_->记录复制读取=true;
                    if(!f)throw std::runtime_error("material owning feature unreadable");
                    const auto holds=[&](const std::optional<新特征准确值>& x){const auto* ref=x?std::get_if<稳定编码>(&*x):nullptr;return ref&&*ref==节点;};
                    bool referenced=holds(f->当前值)||holds(f->标准值);
                    for(const auto& x:f->历史不同值)if(const auto* ref=std::get_if<稳定编码>(&x);ref&&*ref==节点)referenced=true;
                    if(referenced)实现_->记录内容(6,F,*f,p.复制上下文);
                }
            }
            return r;
        }

        return std::nullopt;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取完整模拟材料",L"已登记本地材料结构无法读取。"); }
}
std::optional<新任务虚拟存在读回> 新_模拟世界类::读取任务虚拟存在引用(const 新任务虚拟存在引用& v) const noexcept {
    if(!实现_)return {};
    std::lock_guard lock(实现_->互斥);
    if(实现_->通用) {
        const auto f=读取任务虚拟存在特征组(v);
        return f?std::optional<新任务虚拟存在读回>{{v,f->状态.特征类型概念节点,f->状态.当前值}}:std::nullopt;
    }
    return std::nullopt;
}
std::optional<新任务虚拟存在引用> 新_模拟世界类::查找任务虚拟存在引用(稳定编码 T) const noexcept {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.通用任务.find(T);
    if(s.记录复制读取&&it!=s.通用任务.end())s.已读取复制上下文=true;
    return s.通用&&it!=s.通用任务.end()?std::optional{it->second.本地.引用}:std::nullopt;
}
std::optional<新任务虚拟存在特征组> 新_模拟世界类::读取任务虚拟存在特征组(
    const 新任务虚拟存在引用& ref) const noexcept {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.通用任务.find(ref.任务节点);
    if(!s.通用||it==s.通用任务.end()||it->second.本地.引用!=ref)return {};
    if(s.记录复制读取)s.已读取复制上下文=true;
    auto result=it->second.本地;
    const auto current=[&](新任务虚拟存在特征读回& f) {
        const auto v=读值(f.特征节点,数值);
        if(!v)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟任务治理特征",L"本地治理特征没有可读当前值，禁止回退现实。");
        f.当前值=*v;
    };
    current(result.状态);if(result.服务值)current(*result.服务值);
    return result;
}
std::optional<稳定编码> 新_模拟世界类::读取模拟任务来源(稳定编码 T) const {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.通用任务.find(T);
    return s.通用&&it!=s.通用任务.end()?std::optional{it->second.来源}:std::nullopt;
}
namespace {
新需求目标定义 读模拟目标(稳定编码 n) {
    新需求目标定义 t;t.目标特征类型概念节点=必读引用(n,特征类型);
    t.目标形式=static_cast<新需求目标形式>(必读数(n,目标形式值));
    t.目标值域概念节点=读引用(n,目标域引用);t.目标比较关系概念节点=读引用(n,目标比较引用);
    if(const auto x=读值(n,目标方向值))t.目标变化方向=static_cast<新需求变化方向>(*x);
    if(const auto D=读引用(n,引用内容需求引用)) {
        const auto path=读引用组(n,引用内容路径引用);
        if(path.size()!=1||t.目标形式!=新需求目标形式::值||!t.目标值域概念节点
            ||t.目标比较关系概念节点||t.目标变化方向)
            throw std::runtime_error("local referenced-content target malformed");
        t.引用内容定位=新需求引用内容定位{*D,path};
        return t;
    }
    if(t.目标形式==新需求目标形式::值) {
        if(!t.目标值域概念节点||!t.目标比较关系概念节点||t.目标变化方向)throw std::runtime_error("local value target malformed");
    } else if(t.目标形式==新需求目标形式::变化方向) {
        if(t.目标值域概念节点||t.目标比较关系概念节点||!t.目标变化方向
            ||(*t.目标变化方向!=新需求变化方向::增加&&*t.目标变化方向!=新需求变化方向::减少))
            throw std::runtime_error("local direction target malformed");
    } else throw std::runtime_error("local target form invalid");
    return t;
}
}
新模拟需求读回 新_模拟世界类::读取模拟需求(稳定编码 n) const {
    if(!实现_)return {};auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.本地需求.find(n);if(!s.通用||it==s.本地需求.end())return {};
    if(!属于样例(n,需求种类,s.通用场景))throw std::runtime_error("local demand identity damaged");
    新模拟需求信息 d;d.节点=n;d.内容概念=必读引用(n,内容概念引用);d.附加内容概念组=读引用组(n,附加概念引用);
    d.需求主体存在节点=必读引用(n,主体引用);
    d.信息完善阶段=static_cast<新需求信息完善阶段>(读值(n,完善阶段值).value_or(
        static_cast<std::int64_t>(新需求信息完善阶段::已完善)));
    const bool partial=d.信息完善阶段==新需求信息完善阶段::待完善;
    if(partial) {
        新部分服务要求目标 t;
        t.目标存在节点=读引用(n,目标引用);t.目标特征类型概念节点=读引用(n,特征类型);
        if(const auto x=读值(n,目标形式值))t.目标形式=static_cast<新需求目标形式>(*x);
        t.目标值域概念节点=读引用(n,目标域引用);t.目标比较关系概念节点=读引用(n,目标比较引用);
        if(const auto x=读值(n,目标方向值))t.目标变化方向=static_cast<新需求变化方向>(*x);
        if(t.目标存在节点)d.目标存在节点=*t.目标存在节点;
        d.已知目标=std::move(t);
    } else if(d.信息完善阶段==新需求信息完善阶段::已完善) {
        d.目标存在节点=必读引用(n,目标引用);d.目标=读模拟目标(n);
        if(d.目标.引用内容定位&&!s.本地需求.contains(d.目标.引用内容定位->引用需求节点))
            throw std::runtime_error("local content target points outside sample");
    } else throw std::runtime_error("local demand completion stage invalid");
    if(!s.是普通存在参数(d.需求主体存在节点)
        ||(有效(d.目标存在节点)&&!s.是普通存在参数(d.目标存在节点)))
        throw std::runtime_error("local demand contains foreign existence");
    d.执行域=static_cast<新需求执行域>(必读数(n,执行域值));d.权重=必读数(n,权重值);
    if((d.执行域!=新需求执行域::自我执行&&d.执行域!=新需求执行域::被认知)||d.权重<0)
        throw std::runtime_error("local demand execution-domain/weight invalid");
    const auto active=必读数(n,活动值);if(active<1||active>4)throw std::runtime_error("local demand activity invalid");
    d.活动状态=static_cast<新需求活动状态>(active);d.父需求节点=读引用(n,父需求引用);
    if(d.父需求节点&&!s.本地需求.contains(*d.父需求节点))throw std::runtime_error("local parent demand outside sample");
    if(const auto x=读值(n,根角色值))d.根角色=static_cast<新需求根角色>(*x);
    if(const auto x=读值(n,来源类型值))d.来源类型=static_cast<新需求来源类型>(*x);
    const auto u=读引用(n,理解引用),c=读引用(n,确认引用),r=读引用(n,要求引用);
    if(u||c||r) {
        if(!u||!c||!s.本地理解.contains(*u)||!s.本地确认.contains(*c)||(r&&!s.本地要求.contains(*r)))
            throw std::runtime_error("local demand provenance graph broken");
        if(必读引用(*c,理解引用)!=*u||必读引用(*u,主体引用)!=d.需求主体存在节点
            ||必读引用(*u,目标引用)!=d.目标存在节点||读模拟目标(*u)!=d.目标||读引用(*u,要求引用)!=r)
            throw std::runtime_error("local demand provenance fields disagree");
        d.认知来源=新需求认知来源{*u,*c,r};
    }
    if(partial&&(d.执行域!=新需求执行域::被认知||d.认知来源||u||c||r))
        throw std::runtime_error("local partial demand carries complete provenance");
    if(d.执行域==新需求执行域::被认知&&((!partial&&!d.认知来源)||d.权重!=0||d.父需求节点||d.根角色))
        throw std::runtime_error("recognized local demand provenance or governance invalid");
    d.服务要求确认组=读引用组(n,服务确认引用);for(const auto x:d.服务要求确认组)
        if(!s.本地确认.contains(x))throw std::runtime_error("local demand confirmation outside sample");
    d.外部要求节点=读引用(n,外部要求引用);
    if(d.外部要求节点&&!s.本地要求.contains(*d.外部要求节点))throw std::runtime_error("local demand external requirement outside sample");
    s.记录内容(1,n,d,it->second.has_value());return {新模拟需求状态::已完成,std::move(d)};
}
正式信息参数读取结果 新_模拟世界类::读取正式信息参数保护(稳定编码 N,稳定编码 C) const noexcept {
    using S=正式信息参数读取状态;
    try {
        if(!实现_)return {S::信息不存在,{},{}};
        auto& s=*实现_;
        const auto roots=s.方法.读取模拟需求概念组();
        if(roots.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(roots.状态==正式结构操作状态::依赖未就绪||roots.状态==正式结构操作状态::未找到)
            return {S::依赖未就绪,{},{}};
        if(roots.状态!=正式结构操作状态::已读取||!roots.信息)
            throw std::runtime_error("formal simulation demand roots unavailable");
        if(C!=roots.信息->需求)return {S::范围未支持,{},{}};
        const auto root=s.方法.读取模拟正式结构(C);
        if(root.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(root.状态==正式结构操作状态::依赖未就绪||root.状态==正式结构操作状态::未找到)
            return {S::依赖未就绪,{},{}};
        if(root.状态!=正式结构操作状态::已读取||!root.信息||root.完整保护组.empty()
            ||!std::holds_alternative<信息结构概念定义>(root.信息->定义))
            throw std::runtime_error("formal simulation demand root structure damaged");
        const auto same=[](const 基础联合节点快照& a,const 基础联合节点快照& b) {
            return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组
                &&a.入向字段组==b.入向字段组&&a.入向外部关系组==b.入向外部关系组
                &&a.出向外部关系组==b.出向外部关系组;
        };
        const auto capture=[&](const std::vector<稳定编码>& locals,bool 复核=false) {
            std::set<稳定编码> ids(locals.begin(),locals.end());
            const auto read=[&](const std::set<稳定编码>& group) {
                const auto r=全局基础数据集.查询节点组完整结构({group.begin(),group.end()});
                if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
                if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=group.size())
                    throw std::runtime_error("formal simulation parameter snapshot unavailable");
                std::map<稳定编码,基础联合节点快照> out;
                for(const auto& item:r.节点组) {
                    if(!item.快照&&复核)throw 新模拟世界状态::定义前提已变化;
                    if(!item.快照||item.查询节点!=item.快照->结构.节点.编码)
                        throw std::runtime_error("registered simulation parameter endpoint missing");
                    out.emplace(item.查询节点,*item.快照);
                }return out;
            };
            const auto local=read(ids);
            // 只取样例字段的共享端点一跳；不沿概念或场景树扩展。
            for(const auto& [id,item]:local)for(const auto& field:item.结构.字段组) {
                ids.insert(field.字段节点);
                if(const auto* ref=std::get_if<稳定编码>(&field.内容))ids.insert(*ref);
            }
            auto all=read(ids);
            for(const auto& [id,item]:local)if(!same(item,all.at(id)))throw 新模拟世界状态::定义前提已变化;
            return all;
        };
        稳定编码 场景,方法;
        std::vector<稳定编码> 本地节点;
        using 来源索引=std::map<稳定编码,std::optional<稳定编码>>;
        std::array<来源索引,5> 索引;
        std::map<稳定编码,基础联合节点快照> expected;
        新模拟需求信息 d;
        {
            std::lock_guard lock(s.互斥);
            if(!s.通用||!s.本地需求.contains(N))return {S::信息不存在,{},{}};
            场景=s.通用场景;方法=s.通用方法;本地节点=s.通用节点;
            索引={s.本地需求,s.本地理解,s.本地确认,s.本地要求,s.本地语言场景};
            auto ids=本地节点;ids.push_back(模拟世界树根节点);
            expected=capture(ids);
            const auto read=读取模拟需求(N);
            if(read.状态!=新模拟需求状态::已完成||!read.信息)
                throw std::runtime_error("registered formal simulation demand unreadable");
            d=*read.信息;
            // 正式来源只从本地owner重建，沿实际D/U/C/R/L引用去重核验。
            std::set<稳定编码> visited;std::vector<稳定编码> todo{N};
            for(std::size_t i=0;i<todo.size();++i) {
                const auto id=todo[i];if(!visited.insert(id).second)continue;
                if(s.本地需求.contains(id)) {if(!读取模拟需求(id).信息)throw std::runtime_error("local demand closure missing");}
                else if(s.本地理解.contains(id)) {if(!读取模拟理解(id).信息)throw std::runtime_error("local understanding closure missing");}
                else if(s.本地确认.contains(id)) {if(!读取模拟确认(id).信息)throw std::runtime_error("local confirmation closure missing");}
                else if(s.本地要求.contains(id)) {if(!读取模拟要求(id).信息)throw std::runtime_error("local requirement closure missing");}
                else if(s.本地语言场景.contains(id)) {if(!读取模拟语言场景(id).信息)throw std::runtime_error("local language closure missing");}
                for(const auto& field:expected.at(id).结构.字段组)if(const auto* ref=std::get_if<稳定编码>(&field.内容))
                    if(s.本地需求.contains(*ref)||s.本地理解.contains(*ref)||s.本地确认.contains(*ref)
                        ||s.本地要求.contains(*ref)||s.本地语言场景.contains(*ref))todo.push_back(*ref);
            }
        }
        const auto merge=[&](const std::vector<基础联合节点快照>& items) {
            for(const auto& item:items) {
                const auto [it,added]=expected.emplace(item.结构.节点.编码,item);
                if(!added&&!same(it->second,item))return false;
            }return true;
        };
        if(!merge(root.完整保护组))return {S::前提已变化,{},{}};
        using namespace 先天特征类型槽;
        const std::uint32_t slots[]{需求主体,需求执行域,需求目标存在,需求目标FT,需求目标形式,
            需求权重,需求活动,目标域,目标比较,目标方向,父需求,需求来源,需求根角色,理解来源,内容确认来源};
        std::set<稳定编码> types;
        正式信息参数投影 out{N,C,{}};
        using K=正式内容值种类;using V=正式内容值状态;
        const bool recognized=d.执行域==新需求执行域::被认知;
        新部分服务要求目标 target;
        if(d.已知目标)target=*d.已知目标;
        else {
            target.目标存在节点=d.目标存在节点;target.目标特征类型概念节点=d.目标.目标特征类型概念节点;
            target.目标形式=d.目标.目标形式;target.目标值域概念节点=d.目标.目标值域概念节点;
            target.目标比较关系概念节点=d.目标.目标比较关系概念节点;target.目标变化方向=d.目标.目标变化方向;
        }
        for(const auto slot:slots) {
            const auto ft=s.方法.读取模拟先天特征类型槽(slot);
            if(ft.状态==特征概念规则状态::资源失败)return {S::资源失败,{},{}};
            if(ft.状态==特征概念规则状态::前提已变化)return {S::前提已变化,{},{}};
            if(ft.状态==特征概念规则状态::读取接口未装配||ft.状态==特征概念规则状态::值不存在)
                return {S::依赖未就绪,{},{}};
            if(ft.状态!=特征概念规则状态::已完成||!ft.类型根||ft.完整保护组.empty()||!types.insert(*ft.类型根).second)
                throw std::runtime_error("formal simulation demand feature slots damaged");
            if(!merge(ft.完整保护组))return {S::前提已变化,{},{}};
            正式内容值 value{K::I64数值,V::当前未知,{}};
            const auto number=[&](std::int64_t x){value.值=正式内容准确值{x};value.状态=V::已知;};
            const auto reference=[&](K k,std::optional<稳定编码> x){value.种类=k;if(x){value.值=正式内容准确值{*x};value.状态=V::已知;}};
            const auto inapplicable=[&]{value.状态=V::不适用;value.值.reset();};
            switch(slot) {
            case 需求主体:reference(K::存在引用,d.需求主体存在节点);break;
            case 需求执行域:number(static_cast<std::int64_t>(d.执行域));break;
            case 需求目标存在:reference(K::存在引用,target.目标存在节点);break;
            case 需求目标FT:reference(K::概念引用,target.目标特征类型概念节点);break;
            case 需求目标形式:if(target.目标形式)number(static_cast<std::int64_t>(*target.目标形式));break;
            case 需求权重:number(d.权重);break;
            case 需求活动:number(static_cast<std::int64_t>(d.活动状态));break;
            case 目标域:reference(K::概念引用,target.目标值域概念节点);if(target.目标形式==新需求目标形式::变化方向)inapplicable();break;
            case 目标比较:reference(K::概念引用,target.目标比较关系概念节点);if(target.目标形式==新需求目标形式::变化方向)inapplicable();break;
            case 目标方向:if(target.目标变化方向)number(static_cast<std::int64_t>(*target.目标变化方向));if(target.目标形式==新需求目标形式::值)inapplicable();break;
            case 父需求:reference(K::需求引用,d.父需求节点);if(recognized||d.根角色)inapplicable();break;
            case 需求来源:if(d.来源类型)number(static_cast<std::int64_t>(*d.来源类型));if(recognized||d.根角色)inapplicable();break;
            case 需求根角色:if(d.根角色)number(static_cast<std::int64_t>(*d.根角色));else inapplicable();break;
            case 理解来源:reference(K::语言理解引用,d.认知来源 ? std::optional{d.认知来源->理解节点} : std::nullopt);if(!recognized)inapplicable();break;
            case 内容确认来源:reference(K::内容确认引用,d.认知来源 ? std::optional{d.认知来源->内容确认节点} : std::nullopt);if(!recognized)inapplicable();break;
            default:throw std::logic_error("unknown formal demand slot");
            }
            out.直接内容组.push_back({*ft.类型根,std::move(value)});
        }
        正式引用概念语义 semantic;
        semantic.状态=d.已知目标 ? 正式引用概念语义状态::部分 : 正式引用概念语义状态::完整;
        if(d.目标.引用内容定位) {
            semantic.状态=正式引用概念语义状态::引用内容未展开;
            semantic.引用内容定位=正式内容引用定位{d.目标.引用内容定位->引用需求节点,
                d.目标.引用内容定位->正式内容路径};
        }
        out.引用概念语义=std::move(semantic);
        const auto rootsAfter=s.方法.读取模拟需求概念组();
        const auto rootAfter=s.方法.读取模拟正式结构(C);
        if(rootsAfter.状态==正式结构操作状态::前提已变化||rootsAfter.信息!=roots.信息
            ||rootAfter.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(rootsAfter.状态!=正式结构操作状态::已读取||rootAfter.状态!=正式结构操作状态::已读取||!rootAfter.信息)
            throw std::runtime_error("formal demand definition disappeared during parameter read");
        if(!merge(rootAfter.完整保护组))return {S::前提已变化,{},{}};
        {
            std::lock_guard lock(s.互斥);
            if(!s.通用||s.通用场景!=场景||s.通用方法!=方法||s.通用节点!=本地节点
                ||索引!=std::array<来源索引,5>{s.本地需求,s.本地理解,s.本地确认,s.本地要求,s.本地语言场景})
                return {S::前提已变化,{},{}};
            const auto fresh=读取模拟需求(N);
            if(!fresh.信息||*fresh.信息!=d)return {S::前提已变化,{},{}};
            auto ids=本地节点;ids.push_back(模拟世界树根节点);
            const auto current=capture(ids,true);
            for(const auto& [id,item]:current) {
                const auto old=expected.find(id);
                if(old==expected.end()||!same(old->second,item))return {S::前提已变化,{},{}};
            }
        }
        std::vector<稳定编码> ids;for(const auto& [id,item]:expected)ids.push_back(id);
        const auto current=全局基础数据集.查询节点组完整结构(ids);
        if(current.状态==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        if(current.状态!=基础结构操作状态::已完成||current.节点组.size()!=ids.size())
            throw std::runtime_error("formal simulation parameter final protection unavailable");
        for(const auto& item:current.节点组)
            if(!item.快照||!same(expected.at(item.查询节点),*item.快照))return {S::前提已变化,{},{}};
        std::vector<基础联合节点快照> protection;for(auto& [id,item]:expected)protection.push_back(std::move(item));
        return {S::已读取,std::move(out),std::move(protection)};
    }catch(新模拟世界状态 state) {
        if(state==新模拟世界状态::定义前提已变化)return {S::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟正式需求参数",L"内部读取状态不一致。");
    }catch(const std::bad_alloc&) {return {S::资源失败,{},{}};}
    catch(const std::exception& e) {工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟正式需求参数",L"本地结构或保护读取工程故障。",e.what());}
    catch(...) {工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟正式需求参数",L"未知工程故障。");}
}
新模拟需求关联读回<服务要求理解信息> 新_模拟世界类::读取模拟理解(稳定编码 n) const {
    if(!实现_)return {};auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.本地理解.find(n);if(!s.通用||it==s.本地理解.end())return {};
    if(!属于样例(n,理解种类,s.通用场景))throw std::runtime_error("local understanding identity damaged");
    服务要求理解信息 u;u.节点=n;u.语言场景节点=必读引用(n,语言场景引用);
    u.来源人类节点=必读引用(n,主体引用);u.目标存在节点=必读引用(n,目标引用);
    if(!s.本地语言场景.contains(u.语言场景节点)||!s.是普通存在参数(u.来源人类节点)||!s.是普通存在参数(u.目标存在节点))
        throw std::runtime_error("local understanding contains foreign reference");
    u.公开会话标识=读结构材料<std::string>(n,会话值);u.目标=读模拟目标(n);
    u.固定语义=读结构目标语义(必读引用(n,固定语义引用));u.来源要求节点=读引用(n,要求引用);
    if(u.固定语义.目标类型.概念节点!=u.目标.目标特征类型概念节点)
        throw std::runtime_error("local understanding fixed feature meaning differs from target");
    if(u.目标.目标形式==新需求目标形式::值) {
        if(!u.固定语义.目标域||!u.固定语义.比较
            ||u.固定语义.目标域->特征语义.概念节点!=*u.目标.目标值域概念节点
            ||u.固定语义.比较->比较概念节点!=*u.目标.目标比较关系概念节点
            ||u.固定语义.比较->参照概念节点!=u.目标.目标值域概念节点)throw std::runtime_error("local understanding fixed value target malformed");
    } else if(u.固定语义.目标域||u.固定语义.比较)throw std::runtime_error("local direction understanding contains value target snapshots");
    if(u.公开会话标识.empty()||!新_语言类::严格UTF8字节串有效(u.公开会话标识)
        ||(u.来源要求节点&&!s.本地要求.contains(*u.来源要求节点)))throw std::runtime_error("local understanding source invalid");
    s.记录内容(2,n,u,it->second.has_value());return {新模拟需求状态::已完成,std::move(u)};
}
新模拟需求关联读回<服务要求确认信息> 新_模拟世界类::读取模拟确认(稳定编码 n) const {
    if(!实现_)return {};auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.本地确认.find(n);if(!s.通用||it==s.本地确认.end())return {};
    if(!属于样例(n,确认种类,s.通用场景))throw std::runtime_error("local confirmation identity damaged");
    const auto u=读取模拟理解(必读引用(n,理解引用));if(!u.信息)throw std::runtime_error("local confirmation understanding missing");
    const auto flag=必读数(n,需重新确认值);if(flag!=0&&flag!=1)throw std::runtime_error("local confirmation flag corrupt");
    服务要求确认信息 c{n,*u.信息,flag!=0};s.记录内容(3,n,c,it->second.has_value());return {新模拟需求状态::已完成,std::move(c)};
}
新模拟需求关联读回<新部分服务要求材料> 新_模拟世界类::读取模拟要求(稳定编码 n) const {
    if(!实现_)return {};auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.本地要求.find(n);if(!s.通用||it==s.本地要求.end())return {};
    if(!属于样例(n,要求种类,s.通用场景))throw std::runtime_error("local requirement identity damaged");
    新部分服务要求材料 r;r.要求节点=n;r.来源人类节点=必读引用(n,主体引用);
    if(!s.是普通存在参数(r.来源人类节点))throw std::runtime_error("local requirement subject foreign or not existence");
    r.公开会话标识=读结构材料<std::string>(n,会话值);r.来源要求标识=读结构材料<std::string>(n,要求标识值);
    auto& t=r.已知目标;t.目标存在节点=读引用(n,目标引用);t.目标特征类型概念节点=读引用(n,特征类型);
    if(const auto x=读值(n,目标形式值))t.目标形式=static_cast<新需求目标形式>(*x);
    t.目标值域概念节点=读引用(n,目标域引用);t.目标比较关系概念节点=读引用(n,目标比较引用);
    if(const auto x=读值(n,目标方向值))t.目标变化方向=static_cast<新需求变化方向>(*x);
    r.理解节点=读引用(n,理解引用);r.确认节点=读引用(n,确认引用);
    if((t.目标存在节点&&!s.是普通存在参数(*t.目标存在节点))||(r.理解节点&&!s.本地理解.contains(*r.理解节点))
        ||(r.确认节点&&!s.本地确认.contains(*r.确认节点))||r.公开会话标识.empty()||r.来源要求标识.empty()
        ||!新_语言类::严格UTF8字节串有效(r.公开会话标识)||!新_语言类::严格UTF8字节串有效(r.来源要求标识))
        throw std::runtime_error("local requirement contents invalid");
    if(r.确认节点&&!r.理解节点)throw std::runtime_error("local requirement confirmation without understanding");
    if(r.理解节点) {
        const auto u=读取模拟理解(*r.理解节点);if(!u.信息)throw std::runtime_error("local requirement understanding missing");
        const auto& i=*u.信息;
        if(i.来源要求节点!=n||i.来源人类节点!=r.来源人类节点||i.公开会话标识!=r.公开会话标识
            ||t.目标存在节点!=std::optional{i.目标存在节点}||t.目标特征类型概念节点!=std::optional{i.目标.目标特征类型概念节点}
            ||t.目标形式!=std::optional{i.目标.目标形式}||t.目标值域概念节点!=i.目标.目标值域概念节点
            ||t.目标比较关系概念节点!=i.目标.目标比较关系概念节点||t.目标变化方向!=i.目标.目标变化方向)
            throw std::runtime_error("local requirement and understanding contents disagree");
        if(r.确认节点&&必读引用(*r.确认节点,理解引用)!=*r.理解节点)throw std::runtime_error("local requirement confirmation differs from understanding");
    }
    s.记录内容(4,n,r,it->second.has_value());return {新模拟需求状态::已完成,std::move(r)};
}
新模拟需求关联读回<新模拟语言场景信息> 新_模拟世界类::读取模拟语言场景(稳定编码 n) const {
    if(!实现_)return {};auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.本地语言场景.find(n);if(!s.通用||it==s.本地语言场景.end())return {};
    if(!属于样例(n,语言场景种类,s.通用场景))throw std::runtime_error("local language scene identity damaged");
    新模拟语言场景信息 l;l.节点=n;l.专属存在概念=读引用(n,特征类型);
    l.特征节点组=读引用组(n,场景特征引用);l.直接成员存在组=读引用组(n,场景成员引用);l.边界存在组=读引用组(n,场景边界引用);
    for(const auto x:l.特征节点组)if(!s.是本地参数(x,新方法参数信息类型::特征)||读引用(x,所属)!=n)throw std::runtime_error("local language feature owner corrupt");
    for(const auto x:l.直接成员存在组)if(!s.是普通存在参数(x)||读引用(x,所属)!=n)throw std::runtime_error("local language scene member corrupt");
    for(const auto x:l.边界存在组)if(!s.是普通存在参数(x))throw std::runtime_error("local language boundary foreign");
    for(const auto x:读引用组(n,结构子项)) {
        const auto E=必读引用(x,实例存在引用);if(std::find(l.直接成员存在组.begin(),l.直接成员存在组.end(),E)==l.直接成员存在组.end())throw std::runtime_error("local concept instance not a scene member");
        l.概念实例组.push_back({E,n,必读引用(x,实例概念引用),static_cast<新自然语言概念实例种类>(必读数(x,实例种类值))});
    }s.记录内容(5,n,l,it->second.has_value());return {新模拟需求状态::已完成,std::move(l)};
}
std::optional<稳定编码> 新_模拟世界类::读取模拟需求来源(稳定编码 n) const {
    if(!实现_)return {};auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.本地需求.find(n);return it==s.本地需求.end()?std::nullopt:it->second;
}
std::vector<新模拟内容读取记录> 新_模拟世界类::读取本次内容读取组() const {
    if(!实现_)return {};std::lock_guard lock(实现_->互斥);return 实现_->内容读取组;
}
std::vector<稳定编码> 新_模拟世界类::读取本地实例身份组() const {
    if(!实现_)return {};std::lock_guard lock(实现_->互斥);return 实现_->通用节点;
}
} // namespace 海中鱼巣

namespace 海中鱼巣 {
新模拟世界建立结果 新_模拟世界类::建立有序样例(稳定编码 M,
    const std::vector<新模拟有序参数输入>& 输入,const std::vector<新模拟输入事件>& 事件,
    const std::vector<新模拟任务治理输入>& 任务组,const std::vector<新模拟场景事实输入>& 场景输入,
    std::optional<稳定编码> 执行分支,const std::optional<新模拟消息方案输入>& 消息输入,
    const 新模拟需求输入图& 需求图,const std::optional<新模拟执行主体输入>& 执行主体输入) {
    // 只允许图内重映射的内容引用；来源世界的实例身份不能穿透进样例。
    for(const auto& 需求:需求图.需求组) {
        if(需求.目标.引用内容定位)return {新模拟世界状态::入口拒绝,{}};
        if(需求.信息完善阶段==新需求信息完善阶段::待完善) {
            if(需求.执行域!=新需求执行域::被认知||!需求.已知目标||需求.权重!=0
                ||需求.父需求输入||需求.根角色||需求.理解输入||需求.确认输入
                ||需求.来源要求输入||需求.外部要求输入||!需求.服务要求确认输入组.empty()
                ||需求.引用内容定位输入||需求.目标!=新需求目标定义{}
                ||需求.已知目标->目标存在节点)return {新模拟世界状态::入口拒绝,{}};
            const auto& t=*需求.已知目标;
            if((t.目标形式&&*t.目标形式!=新需求目标形式::值&&*t.目标形式!=新需求目标形式::变化方向)
                ||(t.目标形式==新需求目标形式::值&&t.目标变化方向)
                ||(t.目标形式==新需求目标形式::变化方向&&(t.目标值域概念节点||t.目标比较关系概念节点))
                ||(t.目标变化方向&&*t.目标变化方向!=新需求变化方向::增加&&*t.目标变化方向!=新需求变化方向::减少))
                return {新模拟世界状态::入口拒绝,{}};
        } else if(需求.信息完善阶段!=新需求信息完善阶段::已完善||需求.已知目标)
            return {新模拟世界状态::入口拒绝,{}};
        if(需求.引用内容定位输入) {
            const auto& p=*需求.引用内容定位输入;
            if(需求.执行域!=新需求执行域::自我执行||!p.引用需求输入
                ||p.引用需求输入>需求图.需求组.size()||p.引用需求输入==需求.序号
                ||p.正式内容路径.size()!=1||!有效(p.正式内容路径.front())
                ||需求.目标.目标形式!=新需求目标形式::值||!需求.目标.目标值域概念节点
                ||需求.目标.目标比较关系概念节点||需求.目标.目标变化方向)
                return {新模拟世界状态::入口拒绝,{}};
        }
    }
    for(const auto& 理解:需求图.理解组)
        if(理解.目标.引用内容定位)return {新模拟世界状态::入口拒绝,{}};
    if(!实现_||!确保根()) throw std::runtime_error("generic simulation owner unavailable");
    auto& s=*实现_;
    if(执行主体输入) {
        if(!s.源身份||!s.存在概念||!s.特征概念||!s.二次概念||!有效(执行主体输入->来源存在)
            ||!有效(执行主体输入->专属概念)||执行主体输入->来源身份保护.empty()){ 模拟样例拒绝行=__LINE__; return {}; }
        const auto 身份=s.源身份->读取存在身份保护(执行主体输入->来源存在);
        if(身份.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(身份.前提已变化||!身份.信息||身份.信息->专属存在概念节点!=执行主体输入->专属概念)
            return {新模拟世界状态::定义前提已变化,{}};
        const auto 类别=s.存在概念->读取普通存在概念定义保护(执行主体输入->专属概念);
        if(类别.状态==正式概念读取状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
        if(类别.状态!=正式概念读取状态::已读取||!类别.信息
            ||类别.信息->种类!=存在概念种类::专属||类别.信息->对应存在节点!=std::optional{执行主体输入->来源存在})
            throw std::logic_error("simulation actor dedicated concept source invalid");
    }
    auto m=s.方法.获取方法(M);
    if(!m||m->参数规格组.size()>输入.size()) { 模拟样例拒绝行=__LINE__; return {}; }
    新_方法类::执行结构读取结果 消息声明保护;
    if(消息输入) {
        消息声明保护=s.方法.读取方法执行结构闭包({M});
        if(消息声明保护.状态!=新_方法类::执行结构读取状态::已读取||消息声明保护.方法组.size()!=1
            ||消息声明保护.方法组.front()!=*m)return {新模拟世界状态::定义前提已变化,{}};
    }
    std::unique_lock lock(s.互斥);
    if(s.通用) { 模拟样例拒绝行=__LINE__; return {}; }
    s.消息定义保护=消息声明保护;
    if(m->初始模拟输入方案组.empty()!=!消息输入){ 模拟样例拒绝行=__LINE__; return {}; }
    if(消息输入) {
        if(消息输入->方案序号>=m->初始模拟输入方案组.size()){ 模拟样例拒绝行=__LINE__; return {}; }
        const auto& plan=m->初始模拟输入方案组[消息输入->方案序号];
        if(plan.消息组.size()!=消息输入->消息组.size()){ 模拟样例拒绝行=__LINE__; return {}; }
        for(std::size_t i=0;i<plan.消息组.size();++i) {
            const auto& e=plan.消息组[i];const auto& body=消息输入->消息组[i];
            if(body.消息序号!=i||!新_语言类::严格UTF8字节串有效(body.UTF8正文)||(e.正文非空&&body.UTF8正文.empty())){ 模拟样例拒绝行=__LINE__; return {}; }
            const auto d=s.方法.读取模拟特征范围(e.正文范围概念);
            if(!d||d->材料类型!=特征概念材料物理类型::UTF8字符串){ 模拟样例拒绝行=__LINE__; return {}; }
            if(const auto* ids=std::get_if<std::vector<稳定编码>>(&d->值域)) {
                bool found=false;for(const auto id:*ids){const auto v=s.方法.读取模拟候选材料(id);if(!v)throw std::runtime_error("message payload material missing");
                    if(v->材料==新特征值材料{body.UTF8正文})found=true;}
                if(!found){ 模拟样例拒绝行=__LINE__; return {}; }
            } else if(!std::holds_alternative<特征概念全材料值域>(d->值域)){ 模拟样例拒绝行=__LINE__; return {}; }
        }
    }
    for(const auto& in:输入)if(in.规格.信息类型==新方法参数信息类型::特征) {
        if(in.初值方式!=新模拟特征初值方式::常规输入)continue;
        const auto d=s.方法.读取模拟特征范围(in.规格.范围概念节点);
        if(d&&d->比较规则==特征概念比较规则::消息集合身份判等) {
            const auto* a=in.材料 ? std::get_if<std::vector<std::uint64_t>>(&*in.材料):nullptr;
            if(!a||!a->empty()||in.值){ 模拟样例拒绝行=__LINE__; return {}; }
        }
    }
    {
        const auto 有效声明=s.方法.读取执行分支必要条件(M,执行分支);
        if(有效声明.状态==新方法执行分支读取状态::前提已变化)
            return {新模拟世界状态::定义前提已变化,{}};
        if(有效声明.状态!=新方法执行分支读取状态::已读取) { 模拟样例拒绝行=__LINE__; return {}; }
        m->初始必要条件=有效声明.有效必要条件;
    }
    s.通用执行分支=执行分支;
    if(m->初始必要条件&&m->初始必要条件->场景事实限定组.size()!=场景输入.size())
        throw std::invalid_argument("simulation scene fact input set is incomplete");
    for(const auto& fact:场景输入) {
        if(!m->初始必要条件||std::find(m->初始必要条件->场景事实限定组.begin(),m->初始必要条件->场景事实限定组.end(),fact.限定)==m->初始必要条件->场景事实限定组.end()
            ||fact.限定.关系概念节点!=fact.构成.关系概念||!s.方法.读取初始场景事实概念保护(fact.构成.关系概念).信息
            ||!s.方法.获取方法(fact.构成.连接方法))throw std::invalid_argument("simulation scene construction formal identity mismatch");
    }
    for(std::size_t i=0;i<m->参数规格组.size();++i)
        if(!m->参数规格组[i].有序规格||*m->参数规格组[i].有序规格!=输入[i].规格
            ||输入[i].规格.序号!=i+1) { 模拟样例拒绝行=__LINE__; return {}; }
    std::vector<bool> 需求参数(输入.size(),false);
    for(std::size_t i=0;i<输入.size();++i) {
        const auto& p=输入[i];
        if(p.规格.信息类型==新方法参数信息类型::存在) {
            const auto formal=s.方法.读取模拟正式结构(p.规格.范围概念节点);
            if(formal.状态==正式结构操作状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
            if(formal.信息&&std::holds_alternative<信息结构概念定义>(formal.信息->定义)) {
                const auto roots=s.方法.读取模拟需求概念组();
                if(roots.状态==正式结构操作状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
                if(roots.状态!=正式结构操作状态::已读取||!roots.信息||p.规格.范围概念节点!=roots.信息->需求){ 模拟样例拒绝行=__LINE__; return {}; }
                需求参数[i]=true;
                if(!p.需求参数输入||p.需求参数输入>需求图.需求组.size()
                    ||p.所属存在参数||p.值||p.材料||!p.集合成员参数组.empty()||p.引用存在参数
                    ||p.来源存在||!p.来源任务成员组.empty()||p.构造存在概念||!p.历史引用存在参数组.empty()
                    ||p.引用需求输入||p.初值方式!=新模拟特征初值方式::常规输入||p.完整复制特征
                    ||p.所属语言场景输入||p.引用语言场景输入){ 模拟样例拒绝行=__LINE__; return {}; }
                if(需求图.需求组[p.需求参数输入-1].内容概念!=roots.信息->需求){ 模拟样例拒绝行=__LINE__; return {}; }
            }
        }
        if(!需求参数[i]&&p.需求参数输入){ 模拟样例拒绝行=__LINE__; return {}; }
    }
    const auto 是普通存在=[&](std::size_t i){return 输入[i].规格.信息类型==新方法参数信息类型::存在&&!需求参数[i];};
    std::vector<std::size_t> 代表(输入.size());
    for(std::size_t i=0;i<代表.size();++i)代表[i]=i;
    const auto 查代表=[&](std::size_t i) { while(代表[i]!=i)i=代表[i];return i; };
    for(std::size_t i=0;i<输入.size();++i) if(输入[i].同一参数) {
        const auto j=static_cast<std::size_t>(输入[i].同一参数-1);
        if(j>=输入.size()||输入[i].规格.信息类型!=输入[j].规格.信息类型||需求参数[i]!=需求参数[j]) { 模拟样例拒绝行=__LINE__; return {}; }
        const auto a=查代表(i),b=查代表(j);代表[(std::max)(a,b)]=(std::min)(a,b);
    }
    for(std::size_t i=0;i<代表.size();++i)代表[i]=查代表(i);
    const auto 引用代表 = [&](const 新模拟有序参数输入& p) -> std::optional<std::size_t> {
        if(!p.引用存在参数){ 模拟样例拒绝行=__LINE__; return {}; }
        const auto pos=static_cast<std::size_t>(p.引用存在参数-1);
        if(pos>=输入.size()||!是普通存在(pos))
            throw std::invalid_argument("simulation reference target is not an existence parameter");
        if(p.值||p.材料||!p.集合成员参数组.empty()||!p.来源任务成员组.empty())
            throw std::invalid_argument("simulation reference conflicts with value material");
        return 代表[pos];
    };
    const auto 范围包含 = [&](稳定编码 child,稳定编码 parent) {
        std::vector<稳定编码> active,done;
        std::function<bool(稳定编码)> visit=[&](稳定编码 c) {
            if(std::find(active.begin(),active.end(),c)!=active.end())throw std::runtime_error("cyclic simulation existence range");
            if(std::find(done.begin(),done.end(),c)!=done.end())return false;
            const auto info=s.方法.读取模拟存在范围(c);
            if(!info)throw std::runtime_error("simulation existence range unavailable");
            active.push_back(c);bool found=c==parent;
            for(const auto p:info->上位概念组)found=visit(p)||found;
            active.pop_back();done.push_back(c);return found;
        };
        return visit(child);
    };
    std::vector<稳定编码> 构造范围(输入.size());
    for(std::size_t i=0;i<输入.size();++i) {
        if(需求参数[i])continue;
        if(输入[i].规格.信息类型!=新方法参数信息类型::存在) {
            if(输入[i].构造存在概念){ 模拟样例拒绝行=__LINE__; return {}; }
            continue;
        }
        构造范围[i]=输入[i].构造存在概念.value_or(输入[i].规格.范围概念节点);
        if(!范围包含(构造范围[i],输入[i].规格.范围概念节点)){ 模拟样例拒绝行=__LINE__; return {}; }
    }
    for(std::size_t i=0;i<输入.size();++i)if(是普通存在(i)&&代表[i]!=i) {
        const auto 主=代表[i];
        if(范围包含(构造范围[i],构造范围[主]))构造范围[主]=构造范围[i];
        else if(!范围包含(构造范围[主],构造范围[i]))
            throw std::invalid_argument("别名存在的有效构造范围无法证明相容");
    }
    for(std::size_t i=0;i<输入.size();++i)if(是普通存在(i))构造范围[i]=构造范围[代表[i]];
    auto 初始关系组=m->初始模拟关系组;
    if(m->初始必要条件) {
        const auto& 必要=*m->初始必要条件;
        if(必要.条件.关系规则!=新方法项关系规则::全部成立){ 模拟样例拒绝行=__LINE__; return {}; }
        for(auto 条件:必要.条件.条件项组) {
            if(条件.参数序号&&条件.参数序号<=m->参数规格组.size()&&需求参数[条件.参数序号-1]) {
                新方法条件项定义 expected;expected.参数序号=条件.参数序号;
                expected.存在概念节点=输入[条件.参数序号-1].规格.范围概念节点;
                if(条件!=expected){ 模拟样例拒绝行=__LINE__; return {}; }
                continue;
            }
            if(条件.参数序号&&条件.参数序号<=m->参数规格组.size()
                &&输入[条件.参数序号-1].规格.信息类型==新方法参数信息类型::存在
                &&条件.特征类型概念节点) {
                const auto E位置=条件.参数序号-1;
                if(条件.存在概念节点&&!范围包含(构造范围[E位置],*条件.存在概念节点)){ 模拟样例拒绝行=__LINE__; return {}; }
                std::optional<std::size_t> F位置;
                for(std::size_t j=0;j<输入.size();++j)if(代表[j]==j&&输入[j].规格.信息类型==新方法参数信息类型::特征
                    &&输入[j].所属存在参数&&输入[j].所属存在参数<=输入.size()
                    &&代表[输入[j].所属存在参数-1]==代表[E位置]) {
                    const auto d=s.方法.读取模拟特征范围(输入[j].规格.范围概念节点);
                    if(!d)throw std::logic_error("attached simulation input feature definition missing");
                    if(d->特征类型根==*条件.特征类型概念节点){if(F位置){ 模拟样例拒绝行=__LINE__; return {}; }F位置=j;}
                }
                if(!F位置){ 模拟样例拒绝行=__LINE__; return {}; }
                条件.参数序号=static_cast<std::uint32_t>(*F位置+1);条件.存在概念节点.reset();
                const auto& in=输入[*F位置];
                const auto d=s.方法.读取模拟特征范围(in.规格.范围概念节点);
                if(!d)throw std::logic_error("attached simulation input type missing");
                if(d->材料类型!=特征概念材料物理类型::I64标量) {
                    if(条件.精确数值域){ 模拟样例拒绝行=__LINE__; return {}; }
                    if(条件.特征值域概念节点) {
                        const auto 域=s.方法.读取模拟特征范围(*条件.特征值域概念节点);
                        if(!域||域->特征类型根!=d->特征类型根||域->材料类型!=d->材料类型){ 模拟样例拒绝行=__LINE__; return {}; }
                        if(const auto* 有限=std::get_if<std::vector<稳定编码>>(&域->值域)) {
                            if(!in.材料){ 模拟样例拒绝行=__LINE__; return {}; }
                            bool 命中=false;
                            for(const auto V:*有限){const auto v=s.方法.读取模拟候选材料(V);if(v&&v->材料==*in.材料)命中=true;}
                            if(!命中){ 模拟样例拒绝行=__LINE__; return {}; }
                        } else if(const auto* ref=std::get_if<特征概念存在引用值域>(&域->值域)) {
                            if(!in.引用存在参数||in.引用存在参数>输入.size()
                                ||!范围包含(构造范围[in.引用存在参数-1],ref->存在概念节点)){ 模拟样例拒绝行=__LINE__; return {}; }
                        } else if(const auto* a=std::get_if<特征概念结构化I64值域>(&域->值域)) {
                            const auto* values=in.材料 ? std::get_if<std::vector<std::int64_t>>(&*in.材料) : nullptr;
                            if(!values||values->size()!=a->分量.size()){ 模拟样例拒绝行=__LINE__; return {}; }
                            for(std::size_t k=0;k<values->size();++k)
                                if(std::none_of(a->分量[k].值域.begin(),a->分量[k].值域.end(),[&](const auto& r){return r.下界<=(*values)[k]&&(*values)[k]<=r.上界;})){ 模拟样例拒绝行=__LINE__; return {}; }
                        } else if(!std::holds_alternative<特征概念全材料值域>(域->值域)){ 模拟样例拒绝行=__LINE__; return {}; }
                    }
                    continue;
                }
            }
            if(条件.参数序号&&条件.参数序号<=输入.size()
                &&输入[条件.参数序号-1].规格.信息类型==新方法参数信息类型::特征) {
                const auto& in=输入[条件.参数序号-1];
                const auto* v=in.值 ? std::get_if<std::int64_t>(&*in.值) : nullptr;
                const auto d=s.方法.读取模拟特征范围(in.规格.范围概念节点);
                auto shape=条件;shape.特征类型概念节点.reset();shape.特征值域概念节点.reset();shape.精确数值域.reset();
                新方法条件项定义 expected;expected.参数序号=条件.参数序号;
                // 存在引用特征参数的值域条件（如“自我.交互者有值”）：模拟只记录条件实参、不构造值存在，
                // 值域由筹办按分支必要条件核验（与模拟学习侧口径一致）。
                if(d&&d->材料类型==特征概念材料物理类型::存在节点引用&&shape==expected
                    &&条件.特征类型概念节点==std::optional{d->特征类型根}&&条件.特征值域概念节点&&!条件.精确数值域)
                    continue;
                if(!v||!d||shape!=expected||条件.特征类型概念节点!=std::optional{d->特征类型根}){ 模拟样例拒绝行=__LINE__; return {}; }
                if(条件.精确数值域&&(*v<条件.精确数值域->下界||*v>条件.精确数值域->上界)){ 模拟样例拒绝行=__LINE__; return {}; }
                if(条件.特征值域概念节点) {
                    const auto 范围=s.方法.读取模拟特征范围(*条件.特征值域概念节点);
                    const auto* 域=范围 ? std::get_if<std::vector<特征概念I64闭区间>>(&范围->值域) : nullptr;
                    if(!范围||范围->特征类型根!=d->特征类型根||!域||
                        !std::any_of(域->begin(),域->end(),[&](const auto& r){return r.下界<=*v&&*v<=r.上界;})){ 模拟样例拒绝行=__LINE__; return {}; }
                }
                continue;
            }
            新方法条件项定义 仅存在范围;仅存在范围.参数序号=条件.参数序号;仅存在范围.存在概念节点=条件.存在概念节点;
            if(!条件.参数序号||条件.参数序号>m->参数规格组.size()||!条件.存在概念节点
                ||条件!=仅存在范围||输入[条件.参数序号-1].规格.信息类型!=新方法参数信息类型::存在){ 模拟样例拒绝行=__LINE__; return {}; }
            if(!范围包含(构造范围[条件.参数序号-1],*条件.存在概念节点)){ 模拟样例拒绝行=__LINE__; return {}; }
        }
        for(const auto& 关系:必要.条件.参数关系组)
            if(std::find(初始关系组.begin(),初始关系组.end(),关系)==初始关系组.end())初始关系组.push_back(关系);
    }
    for(const auto& 关系:初始关系组) {
        if(static_cast<std::int64_t>(关系.种类)<1||static_cast<std::int64_t>(关系.种类)>8){ 模拟样例拒绝行=__LINE__; return {}; }
        if(!关系.左参数序号||!关系.右参数序号||关系.左参数序号>m->参数规格组.size()||关系.右参数序号>m->参数规格组.size()){ 模拟样例拒绝行=__LINE__; return {}; }
        const auto 左=代表[关系.左参数序号-1],右=代表[关系.右参数序号-1];
        if(关系.种类!=新方法参数关系种类::内容引用参数&&!关系.左正式内容路径.empty()){ 模拟样例拒绝行=__LINE__; return {}; }
        if(关系.种类==新方法参数关系种类::内容引用参数) {
            if(!需求参数[左]||!是普通存在(右)||关系.左正式内容路径.size()!=1||!s.特征概念){ 模拟样例拒绝行=__LINE__; return {}; }
            const auto 主体FT=s.特征概念->读取先天特征类型槽(先天特征类型槽::需求主体);
            if(主体FT.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            if(主体FT.状态==特征概念规则状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
            if(!主体FT.类型根||关系.左正式内容路径.front()!=*主体FT.类型根){ 模拟样例拒绝行=__LINE__; return {}; }
            const auto D=std::find_if(需求图.需求组.begin(),需求图.需求组.end(),[&](const auto& d){return d.序号==输入[左].需求参数输入;});
            if(D==需求图.需求组.end()||!D->主体存在输入||D->主体存在输入>代表.size()
                ||代表[D->主体存在输入-1]!=右){ 模拟样例拒绝行=__LINE__; return {}; }
            continue;
        }
        if((需求参数[左]||需求参数[右])&&关系.种类!=新方法参数关系种类::同一&&关系.种类!=新方法参数关系种类::不同
            &&!(关系.种类==新方法参数关系种类::特征引用存在&&需求参数[右])){ 模拟样例拒绝行=__LINE__; return {}; }
        if(关系.种类==新方法参数关系种类::同一&&左!=右){ 模拟样例拒绝行=__LINE__; return {}; }
        if(关系.种类==新方法参数关系种类::不同&&左==右){ 模拟样例拒绝行=__LINE__; return {}; }
        if(关系.种类==新方法参数关系种类::不同&&需求参数[左]&&需求参数[右]
            &&输入[左].需求参数输入==输入[右].需求参数输入){ 模拟样例拒绝行=__LINE__; return {}; }
    }
    for(std::size_t 位置=0;位置<输入.size();++位置)if(输入[位置].初值方式==新模拟特征初值方式::概念构造未知) {
        const auto& 项=输入[位置];const auto 定义=s.方法.读取模拟特征范围(项.规格.范围概念节点);
        if(项.规格.信息类型!=新方法参数信息类型::特征||!定义
            ||定义->材料类型!=特征概念材料物理类型::需求节点引用||定义->比较规则!=特征概念比较规则::需求身份精确
            ||项.复制上下文||项.来源存在||项.完整复制特征||项.值||项.材料||项.引用需求输入||项.引用存在参数
            ||!项.历史引用存在参数组.empty()||!项.集合成员参数组.empty()||!项.来源任务成员组.empty()){ 模拟样例拒绝行=__LINE__; return {}; }
        const auto 同簇=[&](std::uint32_t 参数){return 参数&&参数<=代表.size()&&代表[参数-1]==代表[位置];};
        bool 受限=false;
        const auto 核条件=[&](const 新方法条件项定义& 条件) {
            if(同簇(条件.参数序号)) {
                新方法条件项定义 类型;类型.参数序号=条件.参数序号;类型.特征类型概念节点=条件.特征类型概念节点;
                if(条件.特征前态限定) {
                    const auto& 前态=*条件.特征前态限定;
                    if(前态.当前值状态==新方法当前值状态::未知&&前态.历史不同值数量==std::optional<std::uint64_t>{0})
                        类型.特征前态限定=前态;
                }
                if(条件!=类型)受限=true;
            }
            if(条件.参数序号&&条件.参数序号<=代表.size()&&项.所属存在参数
                &&代表[条件.参数序号-1]==代表[项.所属存在参数-1]
                &&条件.特征类型概念节点==std::optional{定义->特征类型根})受限=true;
        };
        if(m->初始必要条件)for(const auto& 条件:m->初始必要条件->条件.条件项组)核条件(条件);
        for(const auto& 配对:m->条件结果组)for(const auto& 条件:配对.条件.条件项组)核条件(条件);
        for(const auto& 关系:初始关系组)if((同簇(关系.左参数序号)||同簇(关系.右参数序号))
            &&关系.种类!=新方法参数关系种类::同一&&关系.种类!=新方法参数关系种类::不同)受限=true;
        if(受限){ 模拟样例拒绝行=__LINE__; return {}; }
    }
    // 位置关系1..6与正式场景事实14分线；完整声明必须有同限定输入构成。
    if(m->初始必要条件)for(const auto& 限定:m->初始必要条件->场景事实限定组) {
        const auto 保护=s.方法.读取初始场景事实概念保护(限定.关系概念节点);
        if(保护.状态!=正式结构操作状态::已读取||!保护.信息)
            throw std::invalid_argument("模拟样例初始场景关系缺完整正式定义或隔离输入提供者");
        const auto 数量=std::count_if(场景输入.begin(),场景输入.end(),[&](const auto& 输入项) {
            return 输入项.限定==限定&&输入项.构成.关系概念==限定.关系概念节点;
        });
        if(数量!=1)throw std::invalid_argument("simulation scene fact requires one complete matching construction");
    }
    for(std::size_t i=0;i<输入.size();++i) {
        const auto target=引用代表(输入[i]);
        if(!target)continue;
        if(输入[i].规格.信息类型!=新方法参数信息类型::特征){ 模拟样例拒绝行=__LINE__; return {}; }
        const auto d=s.方法.读取模拟特征范围(输入[i].规格.范围概念节点);
        const auto* domain=d ? std::get_if<特征概念存在引用值域>(&d->值域) : nullptr;
        if(!d||d->材料类型!=特征概念材料物理类型::存在节点引用
            ||d->比较规则!=特征概念比较规则::存在身份精确||!domain
            ||!范围包含(构造范围[*target],domain->存在概念节点)){ 模拟样例拒绝行=__LINE__; return {}; }
    }
    for(const auto& r:初始关系组)if(r.种类==新方法参数关系种类::特征引用存在) {
        if(需求参数[r.右参数序号-1]) {
            const auto& F=输入[r.左参数序号-1];
            const auto d=s.方法.读取模拟特征范围(F.规格.范围概念节点);
            if(F.规格.信息类型!=新方法参数信息类型::特征||!d
                ||d->材料类型!=特征概念材料物理类型::需求节点引用||d->比较规则!=特征概念比较规则::需求身份精确
                ||!F.引用需求输入||F.引用需求输入!=输入[r.右参数序号-1].需求参数输入){ 模拟样例拒绝行=__LINE__; return {}; }
            continue;
        }
        const auto target=引用代表(输入[r.左参数序号-1]);
        if(!target||*target!=代表[r.右参数序号-1]){ 模拟样例拒绝行=__LINE__; return {}; }
    }
    for(std::size_t i=0;i<输入.size();++i) if(代表[i]!=i) {
        const auto& a=输入[i];const auto& b=输入[代表[i]];
        if(a.值!=b.值||a.材料!=b.材料||a.来源任务成员组!=b.来源任务成员组
            ||a.引用需求输入!=b.引用需求输入||a.需求参数输入!=b.需求参数输入||a.初值方式!=b.初值方式||a.完整复制特征!=b.完整复制特征){ 模拟样例拒绝行=__LINE__; return {}; }
        if(引用代表(a)!=引用代表(b)){ 模拟样例拒绝行=__LINE__; return {}; }
        if(a.规格.信息类型==新方法参数信息类型::特征) {
            if(a.所属存在参数&&b.所属存在参数
                &&代表.at(a.所属存在参数-1)!=代表.at(b.所属存在参数-1)){ 模拟样例拒绝行=__LINE__; return {}; }
            if(a.规格.范围概念节点!=b.规格.范围概念节点) {
                const auto x=s.方法.读取模拟特征范围(a.规格.范围概念节点);
                const auto y=s.方法.读取模拟特征范围(b.规格.范围概念节点);
                if(!x||!y)throw std::runtime_error("simulation alias definition missing");
                if(x->特征类型根!=y->特征类型根||x->材料类型!=y->材料类型
                    ||x->比较规则!=y->比较规则){ 模拟样例拒绝行=__LINE__; return {}; }
                const auto 符合=[&](const 新特征匹配定义快照& d) {
                    if(std::holds_alternative<特征概念全材料值域>(d.值域))return true;
                    if(const auto* ref=std::get_if<特征概念存在引用值域>(&d.值域)) {
                        const auto target=引用代表(a);
                        return target&&范围包含(构造范围[*target],ref->存在概念节点);
                    }
                    const auto 在域=[](std::int64_t v,const auto& ranges) {
                        return std::any_of(ranges.begin(),ranges.end(),[&](const auto& r) {
                            return r.下界<=v&&v<=r.上界;
                        });
                    };
                    if(const auto* r=std::get_if<std::vector<特征概念I64闭区间>>(&d.值域)) {
                        const auto* v = a.值 ? std::get_if<std::int64_t>(&*a.值) : nullptr;
                        return v&&在域(*v,*r);
                    }
                    if(const auto* r=std::get_if<特征概念结构化I64值域>(&d.值域)) {
                        const auto* v = a.材料 ? std::get_if<std::vector<std::int64_t>>(&*a.材料) : nullptr;
                        if(!v||v->size()!=r->分量.size())return false;
                        for(std::size_t n=0;n<v->size();++n)if(!在域((*v)[n],r->分量[n].值域))return false;
                        return true;
                    }
                    if(const auto* r=std::get_if<std::vector<稳定编码>>(&d.值域)) {
                        for(const auto V:*r) {
                            const auto v=s.方法.读取模拟候选材料(V);
                            if(!v)throw std::runtime_error("simulation alias material missing");
                            if(d.比较规则==特征概念比较规则::服务任务集合身份判等) {
                                const auto* members=std::get_if<std::vector<std::uint64_t>>(&v->材料);
                                std::vector<std::uint64_t> sources;
                                for(const auto T:a.来源任务成员组)sources.push_back(T.值);
                                std::sort(sources.begin(),sources.end());
                                sources.erase(std::unique(sources.begin(),sources.end()),sources.end());
                                if(members&&sources==*members)return true;
                                continue;
                            }
                            if(a.材料&&*a.材料==v->材料)return true;
                        }
                        return false;
                    }
                    throw std::runtime_error("simulation alias domain shape unsupported");
                };
                if(!符合(*x)||!符合(*y)){ 模拟样例拒绝行=__LINE__; return {}; }
            }
        }
    }
    // E先私有准备身份，不挂模拟根、不提供参数绑定；随后一次原子追加发布完整图。
    // R97：场景包含存在退役——场景参数只来自场景事实限定，全部存在落入样例场景，不建独立成员子场景。
    std::optional<std::size_t> 场景参数;
    std::set<std::size_t> 显式场景成员;
    for(const auto& fact:场景输入) {
        const auto pos=fact.限定.场景参数序号;
        if(!pos||pos>输入.size()||!是普通存在(pos-1))
            throw std::invalid_argument("simulation scene input position invalid");
        const auto i=代表[pos-1];
        if(场景参数&&*场景参数!=i)throw std::invalid_argument("multiple simulation interaction scenes unsupported");
        场景参数=i;
    }
    std::vector<std::size_t> E位置;
    for(std::size_t i=0;i<输入.size();++i)
        if(代表[i]==i&&是普通存在(i)&&(!场景参数||i!=*场景参数)
            &&!输入[i].引用语言场景输入) E位置.push_back(i);
    std::map<std::size_t,稳定编码> E节点;
    std::map<稳定编码,稳定编码> T节点;
    std::map<稳定编码,bool> 治理源身份;
    for(const auto& t:任务组) {
        const auto& f=t.治理特征;
        if(!有效(f.引用.任务节点)||!有效(f.引用.虚拟存在节点)||!有效(f.引用.状态特征节点)
            ||f.引用.状态特征节点!=f.状态.特征节点
            ||(f.服务值&&f.服务值->特征节点==f.状态.特征节点)
            ||!T节点.emplace(f.引用.任务节点,稳定编码{}).second)
            throw std::invalid_argument("invalid or duplicate simulation task governance input");
        const auto identity=[&](稳定编码 n) {
            if(!有效(n)||!治理源身份.emplace(n,true).second)
                throw std::invalid_argument("simulation task governance source identities overlap");
        };
        identity(f.引用.任务节点);identity(f.引用.虚拟存在节点);identity(f.状态.特征节点);
        if(f.服务值)identity(f.服务值->特征节点);
    }
    std::vector<稳定编码> D节点,U节点,C节点,R节点,L节点;
    const auto 图节点数=需求图.需求组.size()+需求图.理解组.size()+需求图.确认组.size()+需求图.要求组.size()+需求图.语言场景组.size();
    if(!E位置.empty()||!任务组.empty()||图节点数) {
        const auto count=E位置.size()+任务组.size()+图节点数;
        auto 准备=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(count));
        if(准备.状态!=基础结构操作状态::已完成||准备.节点组.size()!=count)
            throw std::runtime_error("simulation private existence preparation failed");
        s.通用节点=std::move(准备.节点组);s.通用=true;s.通用方法=M;
        for(std::size_t i=0;i<E位置.size();++i)E节点.emplace(E位置[i],s.通用节点[i]);
        for(std::size_t i=0;i<任务组.size();++i)
            T节点.at(任务组[i].治理特征.引用.任务节点)=s.通用节点[E位置.size()+i];
        auto next=E位置.size()+任务组.size();
        const auto take=[&](auto& out,std::size_t size){for(std::size_t i=0;i<size;++i)out.push_back(s.通用节点.at(next++));};
        take(D节点,需求图.需求组.size());take(U节点,需求图.理解组.size());take(C节点,需求图.确认组.size());
        take(R节点,需求图.要求组.size());take(L节点,需求图.语言场景组.size());
        for(std::size_t i=0;i<输入.size();++i)if(输入[i].引用语言场景输入) {
            if(输入[i].规格.序号||输入[i].规格.信息类型!=新方法参数信息类型::存在
                ||输入[i].引用语言场景输入>L节点.size())throw std::invalid_argument("invalid language scene alias");
            E节点.emplace(i,L节点.at(输入[i].引用语言场景输入-1));
        }
    }
    创建批 主场景准备;
    const auto 场景预留=主场景准备.建节点();
    基础节点组追加请求 q;
    q.预期联合节点组=消息声明保护.完整保护组;
    q.预期联合节点组.insert(q.预期联合节点组.end(),需求图.正式定义保护.begin(),需求图.正式定义保护.end());
    if(执行主体输入)q.预期联合节点组.insert(q.预期联合节点组.end(),执行主体输入->来源身份保护.begin(),执行主体输入->来源身份保护.end());
    q.只读预期=需求图.存在范围语义保护;
    // 语义合法性由需求/概念owner在采集时核验；这里要求这些共享引用确实进入同次原子前提。
    const auto 需正式保护=[&](稳定编码 C) {
        if(!有效(C)||std::none_of(需求图.正式定义保护.begin(),需求图.正式定义保护.end(),[&](const auto& x){return x.结构.节点.编码==C;}))
            throw std::invalid_argument("typed demand definition lacks formal owner protection");
    };
    const auto 需目标保护=[&](const 新需求目标定义& t) {
        需正式保护(t.目标特征类型概念节点);
        if(t.目标值域概念节点)需正式保护(*t.目标值域概念节点);
        if(t.目标比较关系概念节点)需正式保护(*t.目标比较关系概念节点);
    };
    for(const auto& d:需求图.需求组) {
        需正式保护(d.内容概念);for(const auto C:d.附加内容概念组)需正式保护(C);
        if(d.已知目标) {
            const auto& t=*d.已知目标;
            if(t.目标特征类型概念节点)需正式保护(*t.目标特征类型概念节点);
            if(t.目标值域概念节点)需正式保护(*t.目标值域概念节点);
            if(t.目标比较关系概念节点)需正式保护(*t.目标比较关系概念节点);
        } else 需目标保护(d.目标);
        if(d.引用内容定位输入)for(const auto FT:d.引用内容定位输入->正式内容路径)需正式保护(FT);
    }
    for(const auto& u:需求图.理解组)需目标保护(u.目标);
    for(const auto& r:需求图.要求组) {
        if(r.已知目标.目标特征类型概念节点)需正式保护(*r.已知目标.目标特征类型概念节点);
        if(r.已知目标.目标值域概念节点)需正式保护(*r.已知目标.目标值域概念节点);
        if(r.已知目标.目标比较关系概念节点)需正式保护(*r.已知目标.目标比较关系概念节点);
    }
    const auto 新节点=[&](节点种类 k)->std::size_t {
        const auto n=q.新节点组.size();
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(k)}},
            {字段组[根],模拟世界树根节点}}});return n;
    };
    const auto 场景=新节点(场景种类);
    std::optional<std::size_t> 独立执行主体;
    if(执行主体输入&&!执行主体输入->同源存在输入) {
        独立执行主体=新节点(存在种类);
        q.新节点组[*独立执行主体].字段组.push_back({字段组[所属],基础组内节点引用{场景}});
        q.新节点组[*独立执行主体].字段组.push_back({字段组[特征类型],执行主体输入->专属概念});
        q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{*独立执行主体},基础外部关系类型::父子});
    }
    if(执行主体输入&&执行主体输入->同源存在输入) {
        const auto 位置=static_cast<std::size_t>(执行主体输入->同源存在输入-1);
        if(位置>=输入.size()||!是普通存在(位置)||输入[位置].来源存在!=std::optional{执行主体输入->来源存在}
            ||构造范围[位置]!=执行主体输入->专属概念){ 模拟样例拒绝行=__LINE__; return {}; }
    }
    std::map<std::size_t,std::size_t> 独立成员场景;
    if(!显式场景成员.empty())for(const auto position:E位置)
        if(!显式场景成员.contains(position)&&!输入[position].引用语言场景输入) {
            const auto local=新节点(场景种类);独立成员场景.emplace(position,local);
            q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{local},基础外部关系类型::父子});
        }
    if(场景参数)q.新节点组[场景].字段组.push_back({字段组[特征类型],构造范围[*场景参数]});
    q.追加外部关系组.push_back({模拟世界树根节点,基础组内节点引用{场景},基础外部关系类型::父子});
    struct 任务索引 final { std::size_t Vt,状态F;std::optional<std::size_t> 服务F; };
    std::vector<任务索引> 任务索引组;
    for(const auto& input:任务组) {
        const auto& t=input.治理特征;
        const auto T=T节点.at(t.引用.任务节点);
        const auto before=全局基础数据集.查询完整节点结构(T);
        if(!before.结构||!before.结构->字段组.empty())throw std::runtime_error("simulation private task changed");
        q.预期既有节点组.push_back(*before.结构);
        const auto vt=新节点(虚拟种类);
        q.追加字段组.push_back({T,字段组[类型],基础值{static_cast<std::int64_t>(任务种类)}});
        q.追加字段组.push_back({T,字段组[根],模拟世界树根节点});
        q.追加字段组.push_back({T,字段组[虚拟引用],基础组内节点引用{vt}});
        q.新节点组[vt].字段组.push_back({字段组[任务引用],T});
        const auto feature=[&](const 新任务虚拟存在特征读回& f) {
            if(!有效(f.特征类型概念节点)||!有效(f.当前特征概念节点))
                throw std::invalid_argument("simulation task governance feature concept missing");
            const auto definition=s.方法.读取模拟特征范围(f.当前特征概念节点);
            if(!definition||definition->特征类型根!=f.特征类型概念节点
                ||definition->材料类型!=特征概念材料物理类型::I64标量)
                throw std::invalid_argument("simulation task governance feature definition disagrees");
            const auto n=新节点(特征种类);
            q.新节点组[n].字段组.push_back({字段组[所属],基础组内节点引用{vt}});
            q.新节点组[n].字段组.push_back({字段组[特征类型],f.当前特征概念节点});
            q.新节点组[n].字段组.push_back({字段组[数值],基础值{f.当前值}});
            q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{n},基础外部关系类型::父子});
            return n;
        };
        任务索引 idx{vt,feature(t.状态),{}};
        if(t.服务值)idx.服务F=feature(*t.服务值);
        q.新节点组[vt].字段组.push_back({字段组[状态特征引用],基础组内节点引用{idx.状态F}});
        q.追加外部关系组.push_back({基础组内节点引用{场景},T,基础外部关系类型::父子});
        q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{vt},基础外部关系类型::父子});
        任务索引组.push_back(idx);
    }
    std::vector<std::size_t> index(输入.size());
    const auto 引用=[&](std::size_t i)->std::variant<稳定编码,基础组内节点引用> {
        i=代表.at(i);
        if(场景参数&&i==*场景参数)return 场景预留;
        if(需求参数[i])return D节点.at(输入[i].需求参数输入-1);
        if(输入[i].规格.信息类型==新方法参数信息类型::存在)return E节点.at(i);
        return 基础组内节点引用{index.at(i)};
    };
    const auto 字段引用=[&](std::size_t i)->基础组内字段内容 {
        return std::visit([](const auto& v)->基础组内字段内容{return v;},引用(i));
    };
    std::map<std::size_t,std::size_t> 语言成员场景;
    for(std::size_t l=0;l<需求图.语言场景组.size();++l)for(const auto pos:需求图.语言场景组[l].成员存在输入组) {
        if(!pos||pos>输入.size()||!是普通存在(pos-1)
            ||!语言成员场景.emplace(代表[pos-1],l).second)throw std::invalid_argument("language member input owner conflict");
    }
    for(std::size_t i=0;i<输入.size();++i) if(代表[i]==i) {
        const auto& p=输入[i];
        if(需求参数[i])continue; // D由需求图prepare统一发表，不再分配或写成普通E。
        if(场景参数&&i==*场景参数)continue;
        if(p.规格.信息类型==新方法参数信息类型::存在) {
            const auto n=E节点.at(i);
            if(p.引用语言场景输入)continue;
            const auto 快照=全局基础数据集.查询完整节点结构(n);
            if(快照.状态!=基础结构操作状态::已完成||!快照.结构||!快照.结构->字段组.empty())
                throw std::runtime_error("simulation private existence changed");
            q.预期既有节点组.push_back(*快照.结构);
            q.追加字段组.push_back({n,字段组[类型],基础值{static_cast<std::int64_t>(存在种类)}});
            q.追加字段组.push_back({n,字段组[根],模拟世界树根节点});
            q.追加字段组.push_back({n,字段组[特征类型],构造范围[i]});
            const auto member=语言成员场景.find(i);
            const auto independent=独立成员场景.find(i);
            q.追加字段组.push_back({n,字段组[所属],member==语言成员场景.end()
                ?基础组内字段内容{基础组内节点引用{independent==独立成员场景.end()?场景:independent->second}}
                :基础组内字段内容{L节点.at(member->second)}});
        } else {
            index[i]=新节点(特征种类);
            q.新节点组[index[i]].字段组.push_back({字段组[特征类型],p.规格.范围概念节点});
        }
        auto 所在场景=场景;
        if(p.规格.信息类型==新方法参数信息类型::存在) {
            if(const auto found=独立成员场景.find(i);found!=独立成员场景.end())所在场景=found->second;
        } else if(p.所属存在参数) {
            if(const auto found=独立成员场景.find(代表.at(p.所属存在参数-1));found!=独立成员场景.end())所在场景=found->second;
        }
        q.追加外部关系组.push_back({基础组内节点引用{所在场景},引用(i),基础外部关系类型::父子});
    }
    for(std::size_t i=0;i<输入.size();++i)if(代表[i]!=i)index[i]=index[代表[i]];
    模拟结构写入器 writer{q,场景};
    const auto 图引用=[&](const auto& nodes,std::uint32_t pos)->稳定编码 {
        if(!pos||pos>nodes.size())throw std::invalid_argument("local typed graph index out of range");return nodes[pos-1];
    };
    const auto E引用=[&](std::uint32_t pos)->基础组内字段内容 {
        if(!pos||pos>输入.size()||!是普通存在(pos-1))
            throw std::invalid_argument("typed demand graph existence index invalid");return 字段引用(pos-1);
    };
    const auto put=[&](稳定编码 n,字段 k,基础组内字段内容 v){q.追加字段组.push_back({n,字段组[k],std::move(v)});};
    const auto num=[&](稳定编码 n,字段 k,std::int64_t v){put(n,k,基础值{v});};
    const auto prepare=[&](稳定编码 n,节点种类 k) {
        const auto before=全局基础数据集.查询完整节点结构(n);
        if(!before.结构||!before.结构->字段组.empty())throw std::runtime_error("local graph prepared identity changed");
        q.预期既有节点组.push_back(*before.结构);num(n,类型,k);put(n,根,模拟世界树根节点);
        put(n,所属,基础组内节点引用{场景});q.追加外部关系组.push_back({基础组内节点引用{场景},n,基础外部关系类型::父子});
    };
    const auto goal=[&](稳定编码 n,const 新需求目标定义& t) {
        put(n,特征类型,t.目标特征类型概念节点);num(n,目标形式值,static_cast<std::int64_t>(t.目标形式));
        if(t.目标值域概念节点)put(n,目标域引用,*t.目标值域概念节点);
        if(t.目标比较关系概念节点)put(n,目标比较引用,*t.目标比较关系概念节点);
        if(t.目标变化方向)num(n,目标方向值,static_cast<std::int64_t>(*t.目标变化方向));
    };
    for(std::size_t i=0;i<D节点.size();++i) {
        const auto& d=需求图.需求组[i];const auto n=D节点[i];if(d.序号!=i+1)throw std::invalid_argument("demand graph ordinal gap");
        prepare(n,需求种类);put(n,内容概念引用,d.内容概念);for(const auto c:d.附加内容概念组)put(n,附加概念引用,c);
        put(n,主体引用,E引用(d.主体存在输入));
        num(n,完善阶段值,static_cast<std::int64_t>(d.信息完善阶段));
        if(d.已知目标) {
            const auto& t=*d.已知目标;
            if(d.目标存在输入)put(n,目标引用,E引用(d.目标存在输入));
            if(t.目标特征类型概念节点)put(n,特征类型,*t.目标特征类型概念节点);
            if(t.目标形式)num(n,目标形式值,static_cast<std::int64_t>(*t.目标形式));
            if(t.目标值域概念节点)put(n,目标域引用,*t.目标值域概念节点);
            if(t.目标比较关系概念节点)put(n,目标比较引用,*t.目标比较关系概念节点);
            if(t.目标变化方向)num(n,目标方向值,static_cast<std::int64_t>(*t.目标变化方向));
        } else {
            put(n,目标引用,E引用(d.目标存在输入));goal(n,d.目标);
            if(d.引用内容定位输入) {
                put(n,引用内容需求引用,图引用(D节点,d.引用内容定位输入->引用需求输入));
                for(const auto FT:d.引用内容定位输入->正式内容路径)put(n,引用内容路径引用,FT);
            }
        }
        num(n,执行域值,static_cast<std::int64_t>(d.执行域));num(n,权重值,d.权重);num(n,活动值,static_cast<std::int64_t>(d.活动状态));
        if(d.父需求输入)put(n,父需求引用,图引用(D节点,*d.父需求输入));
        if(d.根角色)num(n,根角色值,static_cast<std::int64_t>(*d.根角色));
        if(d.来源类型)num(n,来源类型值,static_cast<std::int64_t>(*d.来源类型));
        if(d.理解输入)put(n,理解引用,图引用(U节点,d.理解输入));if(d.确认输入)put(n,确认引用,图引用(C节点,d.确认输入));
        if(d.来源要求输入)put(n,要求引用,图引用(R节点,d.来源要求输入));if(d.外部要求输入)put(n,外部要求引用,图引用(R节点,d.外部要求输入));
        for(const auto c:d.服务要求确认输入组)put(n,服务确认引用,图引用(C节点,c));
        std::set<std::uint32_t> parents;auto parent=d.父需求输入;
        while(parent){if(!*parent||!parents.insert(*parent).second||*parent>D节点.size()||*parent==i+1)throw std::invalid_argument("cyclic local parent demand");parent=需求图.需求组[*parent-1].父需求输入;}
    }
    for(std::size_t i=0;i<U节点.size();++i) {
        const auto& u=需求图.理解组[i];const auto n=U节点[i];if(u.序号!=i+1)throw std::invalid_argument("understanding graph ordinal gap");
        prepare(n,理解种类);put(n,语言场景引用,图引用(L节点,u.语言场景输入));put(n,主体引用,E引用(u.来源存在输入));put(n,目标引用,E引用(u.目标存在输入));
        put(n,会话值,基础值{u.公开会话标识});goal(n,u.目标);put(n,固定语义引用,基础组内节点引用{writer.语义(u.固定语义)});
        if(u.来源要求输入)put(n,要求引用,图引用(R节点,u.来源要求输入));
    }
    for(std::size_t i=0;i<C节点.size();++i) {
        const auto& c=需求图.确认组[i];const auto n=C节点[i];if(c.序号!=i+1)throw std::invalid_argument("confirmation graph ordinal gap");
        prepare(n,确认种类);put(n,理解引用,图引用(U节点,c.理解输入));num(n,需重新确认值,c.需重新确认);
    }
    for(std::size_t i=0;i<R节点.size();++i) {
        const auto& r=需求图.要求组[i];const auto n=R节点[i];if(r.序号!=i+1||r.已知目标.目标存在节点)throw std::invalid_argument("requirement graph ordinal or reference invalid");
        prepare(n,要求种类);put(n,主体引用,E引用(r.来源存在输入));if(r.目标存在输入)put(n,目标引用,E引用(*r.目标存在输入));
        put(n,会话值,基础值{r.公开会话标识});put(n,要求标识值,基础值{r.来源要求标识});const auto& t=r.已知目标;
        if(t.目标特征类型概念节点)put(n,特征类型,*t.目标特征类型概念节点);
        if(t.目标形式)num(n,目标形式值,static_cast<std::int64_t>(*t.目标形式));if(t.目标值域概念节点)put(n,目标域引用,*t.目标值域概念节点);
        if(t.目标比较关系概念节点)put(n,目标比较引用,*t.目标比较关系概念节点);if(t.目标变化方向)num(n,目标方向值,static_cast<std::int64_t>(*t.目标变化方向));
        if(r.理解输入)put(n,理解引用,图引用(U节点,r.理解输入));if(r.确认输入)put(n,确认引用,图引用(C节点,r.确认输入));
    }
    for(std::size_t i=0;i<L节点.size();++i) {
        const auto& l=需求图.语言场景组[i];const auto n=L节点[i];if(l.序号!=i+1)throw std::invalid_argument("language scene graph ordinal gap");
        prepare(n,语言场景种类);if(l.专属存在概念)put(n,特征类型,*l.专属存在概念);
        for(const auto x:l.特征输入组){if(!x||x>输入.size()||输入[x-1].规格.信息类型!=新方法参数信息类型::特征)throw std::invalid_argument("language feature position invalid");put(n,场景特征引用,字段引用(x-1));}
        for(const auto x:l.成员存在输入组)put(n,场景成员引用,E引用(x));for(const auto x:l.边界存在输入组)put(n,场景边界引用,E引用(x));
        for(const auto& x:l.实例组) {
            const auto a=writer.节点();writer.数(a,类型,语言实例关联种类);q.新节点组[a].字段组.erase(q.新节点组[a].字段组.begin());
            writer.写(a,实例存在引用,E引用(x.存在输入));writer.写(a,实例概念引用,x.概念);writer.数(a,实例种类值,static_cast<std::int64_t>(x.种类));put(n,结构子项,基础组内节点引用{a});
        }
    }
    std::map<std::size_t,材料物理类型> material;
    std::map<稳定编码,std::size_t> 复制材料索引;
    std::map<std::pair<新模拟复制值种类,std::vector<std::uint32_t>>,std::size_t> 复制集合索引;
    const auto 复制准确=[&](const 新模拟复制准确值& value)->基础组内字段内容 {
        using K=新模拟复制值种类;
        switch(value.种类) {
        case K::I64:return 基础值{std::get<std::int64_t>(value.内容)};
        case K::存在引用:return E引用(std::get<std::uint32_t>(value.内容));
        case K::需求引用:return 图引用(D节点,std::get<std::uint32_t>(value.内容));
        case K::普通材料: {
            const auto& v=std::get<新特征值信息>(value.内容);
            if(有效(v.节点))if(const auto it=复制材料索引.find(v.节点);it!=复制材料索引.end())return 基础组内节点引用{it->second};
            const auto x=新节点(材料种类);std::visit([&](const auto& raw){q.新节点组[x].字段组.push_back({字段组[数值],基础值{raw}});},v.材料);
            q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{x},基础外部关系类型::父子});
            material.emplace(x,v.物理类型);if(有效(v.节点))复制材料索引.emplace(v.节点,x);return 基础组内节点引用{x};
        }
        case K::存在集合:case K::需求集合:case K::任务集合: {
            auto positions=std::get<std::vector<std::uint32_t>>(value.内容);
            std::sort(positions.begin(),positions.end());positions.erase(std::unique(positions.begin(),positions.end()),positions.end());
            const auto key=std::pair{value.种类,positions};
            if(const auto it=复制集合索引.find(key);it!=复制集合索引.end())return 基础组内节点引用{it->second};
            std::vector<std::uint64_t> ids;
            for(const auto pos:positions) {
                if(value.种类==K::任务集合){
                    if(!pos||pos>任务组.size())throw std::invalid_argument("copied task collection position invalid");
                    const auto 来源任务=任务组[pos-1].治理特征.引用.任务节点;
                    const auto 本地任务=T节点.find(来源任务);
                    if(本地任务==T节点.end()||!有效(本地任务->second))throw std::invalid_argument("copied task collection has no prepared local task");
                    ids.push_back(本地任务->second.值);
                }else if(value.种类==K::需求集合)ids.push_back(图引用(D节点,pos).值);
                else {
                    const auto e=E引用(pos);const auto* ref=std::get_if<稳定编码>(&e);
                    if(!ref)throw std::invalid_argument("copied existence collection reference must have a prepared identity");ids.push_back(ref->值);
                }
            }
            std::sort(ids.begin(),ids.end());ids.erase(std::unique(ids.begin(),ids.end()),ids.end());
            const auto x=新节点(材料种类);q.新节点组[x].字段组.push_back({字段组[数值],基础值{ids}});
            material.emplace(x,材料物理类型::U64数组);
            复制集合索引.emplace(key,x);
            q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{x},基础外部关系类型::父子});return 基础组内节点引用{x};
        }}throw std::invalid_argument("unknown typed copied value");
    };
    const auto 复制域=[&](const 新模拟复制值域& v) {
        if(const auto* domain=std::get_if<特征概念值域>(&v.内容)) {
            if(std::holds_alternative<std::vector<稳定编码>>(*domain))throw std::invalid_argument("copied finite domain must be typed and remapped");
            if(const auto* x=std::get_if<特征概念存在集合值域>(domain);x&&!x->必含存在节点组.empty())throw std::invalid_argument("copied existence domain contains source identities");
            if(const auto* x=std::get_if<特征概念需求集合值域>(domain);x&&!x->必含需求节点组.empty())throw std::invalid_argument("copied demand domain contains source identities");
            return writer.域(*domain);
        }
        const auto n=writer.节点(3);
        for(const auto& value:std::get<std::vector<新模拟复制准确值>>(v.内容))writer.写(n,结构引用,复制准确(value));
        return n;
    };
    for(std::size_t i=0;i<输入.size();++i) {
        if(代表[i]!=i||输入[i].规格.信息类型==新方法参数信息类型::存在)continue;
        const auto& p=输入[i];
        if(p.所属语言场景输入) {
            if(p.所属存在参数)throw std::invalid_argument("copied feature has two owners");
            q.新节点组[index[i]].字段组.push_back({字段组[所属],图引用(L节点,p.所属语言场景输入)});
        } else if(p.所属存在参数) {
            if(p.所属存在参数>输入.size()||!是普通存在(p.所属存在参数-1)){ 模拟样例拒绝行=__LINE__; return {}; }
            q.新节点组[index[i]].字段组.push_back({字段组[所属],字段引用(p.所属存在参数-1)});
        } else q.新节点组[index[i]].字段组.push_back({字段组[所属],基础组内节点引用{场景}});
        if(p.初值方式==新模拟特征初值方式::概念构造未知)continue;
        if(p.初值方式!=新模拟特征初值方式::常规输入) {
            if(!p.完整复制特征||p.值||p.材料||p.引用存在参数||p.引用需求输入
                ||!p.集合成员参数组.empty()||!p.来源任务成员组.empty())throw std::invalid_argument("copied feature input has conflicting authorities");
            const auto& f=*p.完整复制特征;
            if((p.初值方式==新模拟特征初值方式::复制准确)!=f.当前值.has_value())throw std::invalid_argument("copied feature current mode mismatch");
            if(f.实例单位)writer.写(index[i],结构单位引用,*f.实例单位);
            if(f.实例区间)writer.子(index[i],完整实例区间引用,复制域(*f.实例区间));
            if(f.历史聚合)writer.子(index[i],完整聚合引用,复制域(*f.历史聚合));
            if(f.当前值) {
                const auto v=复制准确(*f.当前值);const auto scalar=std::holds_alternative<基础值>(v);
                writer.写(index[i],scalar ? 数值 : 当前材料,v);
            }
            if(f.标准值)writer.写(index[i],标准值引用,复制准确(*f.标准值));
            for(const auto& h:f.历史不同值)writer.写(index[i],历史项引用,复制准确(h));
            for(const auto& h:f.历史值命中信息) {
                if(h.命中次数<0)throw std::invalid_argument("negative copied feature hit count");
                const auto hit=writer.节点();writer.写(hit,结构引用,复制准确(h.值));writer.数(hit,命中次数值,h.命中次数);writer.子(index[i],历史命中引用,hit);
            }continue;
        }
        if(p.完整复制特征)throw std::invalid_argument("copied feature structure without a copied mode");
        if(p.引用需求输入) {
            if(p.值||p.材料||p.引用存在参数||!p.集合成员参数组.empty())throw std::invalid_argument("demand input conflicts with material");
            const auto def=s.方法.读取模拟特征范围(p.规格.范围概念节点);
            if(!def||def->材料类型!=特征概念材料物理类型::需求节点引用||def->比较规则!=特征概念比较规则::需求身份精确)
                throw std::invalid_argument("demand input is not a single-demand feature");
            writer.写(index[i],当前材料,图引用(D节点,p.引用需求输入));continue;
        }
        if(p.引用存在参数) {
            if(p.引用存在参数>输入.size()
                ||!是普通存在(p.引用存在参数-1)){ 模拟样例拒绝行=__LINE__; return {}; }
            q.新节点组[index[i]].字段组.push_back({字段组[当前材料],字段引用(p.引用存在参数-1)});
            continue;
        }        auto 材料=p.材料;
        std::vector<std::uint64_t> 成员;
        for(std::size_t j=0;j<输入.size();++j)if(代表[j]==i)
            for(const auto 位置:输入[j].集合成员参数组) {
                if(!位置||位置>输入.size()||!是普通存在(位置-1)){ 模拟样例拒绝行=__LINE__; return {}; }
                if(场景参数&&代表[位置-1]==*场景参数)成员.push_back(场景预留.值);
                else 成员.push_back(E节点.at(代表[位置-1]).值);
            }
        for(std::size_t j=0;j<输入.size();++j)if(代表[j]==i)
            for(const auto source:输入[j].来源任务成员组) {
                const auto it=T节点.find(source);
                if(it==T节点.end())throw std::invalid_argument("simulation task member has no copied governance input");
                成员.push_back(it->second.值);
            }
        if(!成员.empty()) {
            if(材料&&(!std::holds_alternative<std::vector<std::uint64_t>>(*材料)
                ||!std::get<std::vector<std::uint64_t>>(*材料).empty())){ 模拟样例拒绝行=__LINE__; return {}; }
            std::sort(成员.begin(),成员.end());成员.erase(std::unique(成员.begin(),成员.end()),成员.end());
            材料=新特征值材料{std::move(成员)};
        }
        if(材料) {
            const auto 定义=s.方法.读取模拟特征范围(p.规格.范围概念节点);
            if(!定义)throw std::runtime_error("simulation material definition unavailable");
            if(定义->比较规则==特征概念比较规则::存在集合身份判等) {
                const auto* a=std::get_if<std::vector<std::uint64_t>>(&*材料);
                if(!a)throw std::invalid_argument("simulation existence collection shape mismatch");
                for(const auto ref:*a)if(ref!=场景预留.值&&std::none_of(E节点.begin(),E节点.end(),[&](const auto& x){return x.second.值==ref;}))
                    throw std::invalid_argument("simulation collection contains an unmapped source existence");
            } else if(定义->比较规则==特征概念比较规则::服务任务集合身份判等) {
                const auto* a=std::get_if<std::vector<std::uint64_t>>(&*材料);
                if(!a)throw std::invalid_argument("simulation task collection payload mismatch");
                for(const auto ref:*a)if(std::none_of(T节点.begin(),T节点.end(),[&](const auto& x){return x.second.值==ref;}))
                    throw std::invalid_argument("simulation task collection contains an unmapped source task");
            } else if(定义->比较规则==特征概念比较规则::需求集合身份判等) {
                const auto* a=std::get_if<std::vector<std::uint64_t>>(&*材料);
                if(!a||!a->empty())throw std::invalid_argument("simulation task/demand collection has no isolated target owner");
            }
            const auto v=新节点(材料种类);
            q.新节点组[index[i]].字段组.push_back({字段组[当前材料],基础组内节点引用{v}});
            q.新节点组[v].字段组.push_back({字段组[所属],基础组内节点引用{index[i]}});
            std::visit([&](const auto& value){q.新节点组[v].字段组.push_back({字段组[数值],基础值{value}});},*材料);
            材料物理类型 物理类型;
            switch(定义->材料类型) {
            case 特征概念材料物理类型::U64数组:物理类型=材料物理类型::U64数组;break;
            case 特征概念材料物理类型::I64数组:物理类型=材料物理类型::I64数组;break;
            case 特征概念材料物理类型::UTF8字符串:物理类型=材料物理类型::UTF8字符串;break;
            case 特征概念材料物理类型::二维二值格:物理类型=材料物理类型::二维二值格;break;
            case 特征概念材料物理类型::三维二值格:物理类型=材料物理类型::三维二值格;break;
            default:throw std::invalid_argument("simulation material physical type mismatch");
            }
            material.emplace(v,物理类型);
            q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{v},基础外部关系类型::父子});
        } else {
            if(!p.值||!std::holds_alternative<std::int64_t>(*p.值)){ 模拟样例拒绝行=__LINE__; return {}; }
            q.新节点组[index[i]].字段组.push_back({字段组[数值],基础值{std::get<std::int64_t>(*p.值)}});
        }
    }
    s.通用节点.reserve(s.通用节点.size()+q.新节点组.size()+32);
    s.通用字段.reserve(q.新节点组.size()*8+q.追加字段组.size()+128);
    s.通用外部边.reserve(q.追加外部关系组.size()+32);
    s.通用绑定.resize(m->参数规格组.size());s.通用输入节点.assign(输入.size(),稳定编码{});s.通用事件=事件;
    // 所有身份先私有预分配；将组内占位引用解析后，以一次追加发表全部字段与挂根。
    // 这也使数组材料中的场景引用有真实本地身份，而不需要第二次补写半成品。
    创建批 图身份准备;
    std::vector<稳定编码> 图身份(q.新节点组.size());图身份.at(场景)=场景预留;
    if(图身份.size()>1) {
        auto prepared=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(图身份.size()-1));
        if(prepared.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(prepared.状态!=基础结构操作状态::已完成||prepared.节点组.size()!=图身份.size()-1)throw std::runtime_error("local graph identity allocation failed");
        图身份准备.节点=std::move(prepared.节点组);std::size_t next=0;
        for(std::size_t i=0;i<图身份.size();++i)if(i!=场景)图身份[i]=图身份准备.节点[next++];
    }
    const auto resolveField=[&](基础组内字段内容 value)->基础组内字段内容 {
        if(const auto* local=std::get_if<基础组内节点引用>(&value))return 图身份.at(local->索引);
        return value;
    };
    for(std::size_t i=0;i<q.新节点组.size();++i) {
        const auto before=全局基础数据集.查询完整节点结构(图身份[i]);
        if(!before.结构||!before.结构->字段组.empty())throw std::runtime_error("prepared local graph identity changed");q.预期既有节点组.push_back(*before.结构);
        for(const auto& f:q.新节点组[i].字段组)q.追加字段组.push_back({图身份[i],f.字段节点,resolveField(f.内容)});
    }
    for(auto& f:q.追加字段组)f.内容=resolveField(std::move(f.内容));
    for(auto& e:q.追加外部关系组) {
        if(const auto* ref=std::get_if<基础组内节点引用>(&e.源节点))e.源节点=图身份.at(ref->索引);
        if(const auto* ref=std::get_if<基础组内节点引用>(&e.目标节点))e.目标节点=图身份.at(ref->索引);
    }
    q.新节点组.clear();
    s.通用字段.reserve(s.通用字段.size()+q.追加字段组.size());
    补齐模拟追加联合预期(q);
    auto p=全局基础数据集.原子追加节点组(q);
    if(p.状态==基础节点组追加状态::预期不符)return {新模拟世界状态::定义前提已变化,{}};
    if(p.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
    if(p.状态!=基础节点组追加状态::已完成)throw std::runtime_error("generic simulation graph publication failed, status="+std::to_string(static_cast<int>(p.状态)));
    p.新节点组=std::move(图身份);主场景准备.保留=true;图身份准备.保留=true;
    s.通用=true;s.通用方法=M;s.收集追加(p);s.通用场景=p.新节点组[场景];
    s.执行主体来源=执行主体输入;
    if(执行主体输入)s.本地执行主体=独立执行主体 ? p.新节点组[*独立执行主体]
        :E节点.at(代表.at(执行主体输入->同源存在输入-1));
    const auto registerGraph=[&](auto& map,const auto& nodes,const auto& inputs,const auto& source) {
        for(std::size_t i=0;i<nodes.size();++i)map.emplace(nodes[i],source(inputs[i]));
    };
    registerGraph(s.本地需求,D节点,需求图.需求组,[](const auto& x){return x.来源需求;});
    registerGraph(s.本地理解,U节点,需求图.理解组,[](const auto& x){return x.来源身份;});
    registerGraph(s.本地确认,C节点,需求图.确认组,[](const auto& x){return x.来源身份;});
    registerGraph(s.本地要求,R节点,需求图.要求组,[](const auto& x){return x.来源身份;});
    registerGraph(s.本地语言场景,L节点,需求图.语言场景组,[](const auto& x){return x.来源身份;});
    for(std::size_t i=0;i<任务组.size();++i) {
        auto local=任务组[i].治理特征;
        const auto source=local.引用.任务节点;
        const auto& idx=任务索引组[i];
        local.引用={T节点.at(source),p.新节点组[idx.Vt],p.新节点组[idx.状态F]};
        local.状态.特征节点=local.引用.状态特征节点;
        if(local.服务值)local.服务值->特征节点=p.新节点组[*idx.服务F];
        s.通用任务.emplace(local.引用.任务节点,实现::任务投影{source,local});
        const auto registerF=[&](const 新任务虚拟存在特征读回& f) {
            新模拟有序参数输入 info{{0,新方法参数信息类型::特征,f.当前特征概念节点}};
            info.复制上下文=true;
            s.通用参数.emplace(f.特征节点,std::move(info));
        };
        registerF(local.状态);if(local.服务值)registerF(*local.服务值);
    }
    for(std::size_t i=0;i<输入.size();++i) {
        const auto n=输入[i].规格.信息类型==新方法参数信息类型::存在
            ? (需求参数[i] ? D节点.at(输入[i].需求参数输入-1)
                :场景参数&&代表[i]==*场景参数 ? s.通用场景 : E节点.at(代表[i])) : p.新节点组[index[i]];
        auto 本地参数=输入[i];
        if(是普通存在(i)) {
            本地参数.构造存在概念=构造范围[i];
            if(!本地参数.引用语言场景输入&&读引用(n,特征类型)!=std::optional<稳定编码>{构造范围[i]})
                throw std::runtime_error("模拟存在落图的有效构造范围读回不一致");
        }
        s.通用参数.emplace(n,std::move(本地参数));
        s.通用输入节点[i]=n;
        if(i>=s.通用绑定.size())continue;
        auto& b=s.通用绑定[i];b.世界场景节点=s.通用场景;
        auto owner = 是普通存在(i)
            ? std::optional<稳定编码>{n} : 读引用(n,所属);
        if(owner&&*owner!=s.通用场景)if(const auto actual=读引用(*owner,所属))b.世界场景节点=*actual;
        b.有序引用=新方法有序参数引用{static_cast<std::uint32_t>(i+1),输入[i].规格.信息类型,n};
    }
    for(const auto& [n,k]:material)s.通用材料.emplace(p.新节点组[n],k);
    for(const auto& [F,input]:s.通用参数)
        if(input.规格.信息类型==新方法参数信息类型::特征&&input.所属存在参数)
            if(const auto value=读值(F,数值))发布模拟数值变化(F,*value,true);
    // 所有参数位置保留，不以首个绑定冒充其它别名；传播D来源图闭包。
    for(std::size_t i=0;i<s.通用绑定.size();++i) {
        const auto id=s.通用绑定[i].有序引用->信息节点;
        std::set<稳定编码> visited;std::vector<稳定编码> todo{id};
        for(std::size_t k=0;k<todo.size();++k) {
            if(!visited.insert(todo[k]).second)continue;
            auto& positions=s.内容参数位置[todo[k]];positions.push_back(static_cast<std::uint32_t>(i+1));
            const auto structure=全局基础数据集.查询完整节点结构(todo[k]);
            if(!structure.结构)throw std::runtime_error("local content parameter closure unreadable");
            for(const auto& field:structure.结构->字段组)if(const auto* ref=std::get_if<稳定编码>(&field.内容))
                if(s.本地需求.contains(*ref)||s.本地理解.contains(*ref)||s.本地确认.contains(*ref)
                    ||s.本地要求.contains(*ref)||s.本地语言场景.contains(*ref))todo.push_back(*ref);
        }
    }
    // 全图正式读回在调用窗外，不计作原函数实际读取。
    for(const auto n:D节点)if(!读取模拟需求(n).信息)throw std::runtime_error("local demand publication readback failed");
    for(const auto n:U节点)if(!读取模拟理解(n).信息)throw std::runtime_error("local understanding publication readback failed");
    for(const auto n:C节点)if(!读取模拟确认(n).信息)throw std::runtime_error("local confirmation publication readback failed");
    for(const auto n:R节点)if(!读取模拟要求(n).信息)throw std::runtime_error("local requirement publication readback failed");
    for(const auto n:L节点)if(!读取模拟语言场景(n).信息)throw std::runtime_error("local language scene publication readback failed");
    for(std::size_t i=0;i<输入.size();++i)if(输入[i].规格.信息类型==新方法参数信息类型::特征) {
        const auto F=p.新节点组[index[i]];
        if(!输入[i].历史引用存在参数组.empty()) {
            const auto d=s.方法.读取模拟特征范围(输入[i].规格.范围概念节点);
            if(!d||d->材料类型!=特征概念材料物理类型::存在节点引用||d->比较规则!=特征概念比较规则::存在身份精确)
                throw std::invalid_argument("simulation history is only legal for single existence references");
        }
        std::vector<稳定编码> refs;
        for(const auto pos:输入[i].历史引用存在参数组) {
            if(!pos||pos>输入.size()||!是普通存在(pos-1))
                throw std::invalid_argument("simulation history requires existence position");
            const auto d=s.方法.读取模拟特征范围(输入[i].规格.范围概念节点);
            const auto* range=d?std::get_if<特征概念存在引用值域>(&d->值域):nullptr;
            if(!range||!范围包含(构造范围[pos-1],range->存在概念节点))throw std::invalid_argument("simulation history outside formal existence domain");
            const auto E=场景参数&&代表[pos-1]==*场景参数 ? s.通用场景 : E节点.at(代表[pos-1]);
            if(std::find(refs.begin(),refs.end(),E)==refs.end())refs.push_back(E);
        }
        for(const auto E:refs) {
            const auto existing=全局基础数据集.查询字段(F,字段组[历史引用]);
            if(std::any_of(existing.begin(),existing.end(),[&](const auto& f){const auto* n=std::get_if<稳定编码>(&f.内容);return n&&*n==E;}))continue;
            const auto f=全局基础数据集.添加字段节点(F,字段组[历史引用],E);
            if(!有效(f))throw std::runtime_error("simulation history publication failed");
            s.通用字段.push_back(f);
        }
    }
    for(const auto& input:场景输入) {
        const auto at=[&](std::uint32_t pos) {
            if(!pos||pos>s.通用绑定.size()||!s.通用绑定[pos-1].有序引用)
                throw std::invalid_argument("simulation scene fact binding invalid");
            return s.通用绑定[pos-1].有序引用->信息节点;
        };
        实现::场景事实 fact;fact.输入=input;
        auto& process=fact.过程;auto& b=process.绑定;
        b.场景=at(input.限定.场景参数序号);b.自我=at(input.限定.主体参数序号);
        b.对端=at(input.限定.对象参数序号);b.交互者特征=at(input.限定.特征参数序号);
        if(b.场景!=s.通用场景||b.自我==b.对端)throw std::invalid_argument("simulation scene fact identity mismatch");
        创建批 batch;
        b.外设场景=建样例节点(batch,场景种类,s.通用场景);
        b.窗口=建样例节点(batch,存在种类,b.外设场景);
        batch.引用(b.窗口,字段组[特征类型],input.构成.窗口范围概念);
        b.接收数量特征=建样例节点(batch,特征种类,b.外设场景);
        process.来源身份=建样例节点(batch,过程种类,b.外设场景);
        process.过程节点=建样例节点(batch,过程种类,b.场景);
        process.动作节点=建样例节点(batch,动作种类,b.场景);
        process.预期连接方法=input.构成.连接方法;process.预期动作概念=input.构成.连接动作概念;
        batch.引用(process.过程节点,字段组[动作引用],process.动作节点);
        batch.引用(process.过程节点,字段组[来源引用],process.来源身份);
        batch.引用(process.过程节点,字段组[所属],b.场景);
        batch.引用(process.过程节点,字段组[对端引用],b.对端);
        batch.引用(process.过程节点,字段组[窗口引用],b.窗口);
        batch.引用(process.动作节点,字段组[方法引用],input.构成.连接方法);
        batch.引用(process.动作节点,字段组[特征类型],input.构成.连接动作概念);
        batch.引用(process.动作节点,字段组[来源引用],process.来源身份);
        batch.引用(process.动作节点,字段组[所属],b.自我);
        batch.引用(process.动作节点,字段组[状态特征引用],b.交互者特征);
        batch.值(process.动作节点,字段组[步骤],2);
        batch.值(process.动作节点,字段组[时间语义],static_cast<std::int64_t>(新状态时间语义::抽象相对));
        auto& action=process.动作;action.节点=process.动作节点;action.种类=新动态种类::动作;
        action.动作定义节点=input.构成.连接方法;action.动态概念节点=input.构成.连接动作概念;
        action.动作来源节点=process.来源身份;action.主题存在节点=b.自我;action.动作目标存在节点=b.自我;
        action.动作目标特征节点=b.交互者特征;action.动作时间=新状态强时间{新状态时间语义::抽象相对,2};
        const auto quantity=s.方法.读取模拟特征范围(input.构成.接收数量特征概念);
        if(!quantity||quantity->材料类型!=特征概念材料物理类型::I64标量)
            throw std::invalid_argument("simulation prior quantity definition invalid");
        process.接收数量定义=*quantity;
        batch.引用(b.接收数量特征,字段组[所属],b.窗口);
        batch.引用(b.接收数量特征,字段组[特征类型],quantity->概念节点);
        batch.值(b.接收数量特征,字段组[数值],1);
        const auto state=[&](std::int64_t value,std::int64_t time) {
            const auto n=建样例节点(batch,状态种类,b.外设场景);
            batch.引用(n,字段组[所属],b.窗口);batch.引用(n,字段组[状态特征引用],b.接收数量特征);
            batch.引用(n,字段组[特征类型],quantity->概念节点);batch.值(n,字段组[数值],value);
            batch.值(n,字段组[步骤],time);batch.值(n,字段组[时间语义],static_cast<std::int64_t>(新状态时间语义::抽象相对));
            return 新状态信息{n,b.窗口,b.接收数量特征,quantity->概念节点,新特征准确值{value},{新状态时间语义::抽象相对,time},0};
        };
        process.基准状态=state(0,1);process.更新状态=state(1,3);
        batch.引用(process.过程节点,字段组[前状态引用],process.基准状态.节点);
        batch.引用(process.过程节点,字段组[后状态引用],process.更新状态.节点);
        const auto F=读取模拟特征(b.交互者特征);
        if(!F||核验消息连接过程(process,*F,b,input.构成)!=新交互求值状态::已取得)
            throw std::invalid_argument("simulation prior message process does not satisfy formal relation");
        for(const auto n:batch.节点) {
            const auto snapshot=全局基础数据集.查询节点组完整结构({n});
            if(snapshot.状态!=基础结构操作状态::已完成||snapshot.节点组.size()!=1||!snapshot.节点组.front().快照)
                throw std::runtime_error("simulation prior process readback failed");
            fact.固定节点.push_back(*snapshot.节点组.front().快照);
        }
        s.通用节点.insert(s.通用节点.end(),batch.节点.begin(),batch.节点.end());
        s.通用字段.insert(s.通用字段.end(),batch.字段关系.begin(),batch.字段关系.end());
        s.通用外部边.insert(s.通用外部边.end(),batch.外部关系.begin(),batch.外部关系.end());
        s.交互事实.push_back(std::move(fact));batch.保留=true;s.模拟步骤=3;
    }
    if(消息输入) {
        const auto& plan=m->初始模拟输入方案组[消息输入->方案序号];
        for(std::size_t i=0;i<plan.消息组.size();++i) {
            实现::消息事件 e{plan.消息组[i],消息输入->消息组[i].UTF8正文};
            if(e.声明.触发)for(const auto& msg:读取模拟消息组(s.通用绑定.at(e.声明.触发->特征参数序号-1).有序引用->信息节点))e.触发基线.insert(msg.消息节点);
            s.消息事件组.push_back(std::move(e));
        }
        // 基线全部先捕获；调用前事实只能在后续推进中触发其它项。
        const auto initialScene=s.通用场景;
        for(std::size_t i=0;i<s.消息事件组.size();++i)if(s.消息事件组[i].声明.时机==新方法模拟输入时机::调用前) {
            const auto event=s.消息事件组[i];
            lock.unlock();const auto definition=s.方法.读取方法执行结构闭包({M});lock.lock();
            if(!s.通用||s.通用方法!=M||s.通用场景!=initialScene||s.记录写回||i>=s.消息事件组.size())
                throw std::runtime_error("before-call message sample changed");
            if(definition.状态!=新_方法类::执行结构读取状态::已读取||definition.方法组.size()!=1)
                throw std::runtime_error("before-call message definition unavailable");
            s.消息定义保护=definition;
            发布模拟消息(event.声明,event.正文,0);s.消息事件组[i].已交付=true;
        }
    }
    const auto 完成场景=s.通用场景;
    std::vector<std::pair<稳定编码,稳定编码>> 直接需求;
    for(std::size_t i=0;i<m->参数规格组.size();++i)if(需求参数[i])
        直接需求.push_back({s.通用绑定[i].有序引用->信息节点,输入[i].规格.范围概念节点});
    lock.unlock();
    for(const auto& [D,C]:直接需求) {
        const auto read=读取正式信息参数保护(D,C);
        if(read.状态==正式信息参数读取状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
        if(read.状态==正式信息参数读取状态::资源失败)return {新模拟世界状态::资源失败,{}};
        if(read.状态!=正式信息参数读取状态::已读取||!read.信息||read.信息->直接内容组.size()!=15)
            throw std::runtime_error("local formal demand parameter publication readback failed");
    }
    const auto 核任务值=[&](const 新模拟复制准确值& 原值,const std::optional<新特征准确值>& 实际) {
        if(原值.种类!=新模拟复制值种类::任务集合)return;
        const auto* 材料节点=实际 ? std::get_if<稳定编码>(&*实际) : nullptr;
        const auto 读=材料节点 ? 读取模拟特征值材料(*材料节点) : std::nullopt;
        const auto* 成员组=读 ? std::get_if<std::vector<std::uint64_t>>(&读->材料) : nullptr;
        if(!材料节点||!读||读->节点!=*材料节点||读->物理类型!=材料物理类型::U64数组||!成员组)
            throw std::runtime_error("copied task value publication readback malformed");
        std::set<稳定编码> 期望来源,实际来源;
        for(const auto 位置:std::get<std::vector<std::uint32_t>>(原值.内容)){
            if(!位置||位置>任务组.size())throw std::runtime_error("copied task value readback position invalid");
            期望来源.insert(任务组[位置-1].治理特征.引用.任务节点);
        }
        for(const auto 成员:*成员组){
            const auto 来源=读取模拟任务来源(稳定编码{成员});
            if(!来源||!有效(*来源))throw std::runtime_error("copied task value contains nonlocal task");
            实际来源.insert(*来源);
        }
        if(实际来源!=期望来源)throw std::runtime_error("copied task value lost members or identity aliases");
    };
    const auto 核任务域=[&](const std::optional<新模拟复制值域>& 原域,const std::optional<特征概念值域>& 实际) {
        if(!原域)return;
        const auto* 值组=std::get_if<std::vector<新模拟复制准确值>>(&原域->内容);
        if(!值组)return;
        const auto* 实际组=实际 ? std::get_if<std::vector<稳定编码>>(&*实际) : nullptr;
        for(std::size_t 项号=0;项号<值组->size();++项号)if((*值组)[项号].种类==新模拟复制值种类::任务集合){
            if(!实际组||实际组->size()!=值组->size())throw std::runtime_error("copied finite task domain readback malformed");
            核任务值((*值组)[项号],新特征准确值{(*实际组)[项号]});
        }
    };
    for(std::size_t 参数索引=0;参数索引<输入.size();++参数索引)if(输入[参数索引].完整复制特征){
        const auto 特征=读取模拟特征(p.新节点组[index[参数索引]]);
        if(!特征)throw std::runtime_error("copied feature publication readback unavailable");
        const auto& 原=*输入[参数索引].完整复制特征;
        if(原.当前值)核任务值(*原.当前值,特征->当前值);
        if(原.标准值)核任务值(*原.标准值,特征->标准值);
        核任务域(原.实例区间,特征->实例区间值);核任务域(原.历史聚合,特征->实例历史聚合值域);
        for(std::size_t 项号=0;项号<原.历史不同值.size();++项号)if(原.历史不同值[项号].种类==新模拟复制值种类::任务集合){
            if(特征->历史不同值.size()!=原.历史不同值.size())throw std::runtime_error("copied task history size differs");
            核任务值(原.历史不同值[项号],特征->历史不同值[项号]);
        }
        for(std::size_t 项号=0;项号<原.历史值命中信息.size();++项号)if(原.历史值命中信息[项号].值.种类==新模拟复制值种类::任务集合){
            if(特征->历史值命中信息.size()!=原.历史值命中信息.size()||特征->历史值命中信息[项号].命中次数!=原.历史值命中信息[项号].命中次数)
                throw std::runtime_error("copied task history hits differ");
            核任务值(原.历史值命中信息[项号].值,特征->历史值命中信息[项号].值);
        }
    }
    return {新模拟世界状态::已完成,完成场景};
}
namespace {
struct 模拟需求前提变化 final {};
struct 模拟需求规格未支持 final { std::string 诊断; };
bool 模拟需求快照相同(const 基础联合节点快照& 左,const 基础联合节点快照& 右) {
    return 左.结构.节点==右.结构.节点&&左.结构.字段组==右.结构.字段组
        &&左.入向字段组==右.入向字段组&&左.入向外部关系组==右.入向外部关系组&&左.出向外部关系组==右.出向外部关系组;
}
void 合并模拟需求保护(std::vector<基础联合节点快照>& 输出,const std::vector<基础联合节点快照>& 输入) {
    for(const auto& 项:输入) {
        const auto 旧=std::find_if(输出.begin(),输出.end(),[&](const auto& 值){return 值.结构.节点==项.结构.节点;});
        if(旧==输出.end())输出.push_back(项);
        else if(!模拟需求快照相同(*旧,项))throw 模拟需求前提变化{};
    }
}
bool 模拟需求保护仍同(const std::vector<基础联合节点快照>& 保护) {
    std::vector<稳定编码> 节点;for(const auto& 项:保护)节点.push_back(项.结构.节点.编码);
    const auto 读取=全局基础数据集.查询节点组完整结构(节点);
    if(读取.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(读取.状态!=基础结构操作状态::已完成||读取.节点组.size()!=节点.size())
        throw std::runtime_error("simulation demand protection read failed");
    for(const auto& 项:读取.节点组) {
        const auto 原值=std::find_if(保护.begin(),保护.end(),[&](const auto& 原){return 原.结构.节点.编码==项.查询节点;});
        if(原值==保护.end()||!项.快照||!模拟需求快照相同(*原值,*项.快照))return false;
    }
    return true;
}
bool 模拟需求业务相同(const 新本能需求材料& 左,const 新本能需求材料& 右) {
    if(左.服务对象!=右.服务对象||左.需求特征!=右.需求特征||左.需求节点!=右.需求节点
        ||左.动作前需求!=右.动作前需求||左.动作前需求特征!=右.动作前需求特征||左.需求内容.size()!=右.需求内容.size())return false;
    for(std::size_t 位置=0;位置<左.需求内容.size();++位置) {
        const auto& 前=左.需求内容[位置];const auto& 后=右.需求内容[位置];
        if(前.特征类型!=后.特征类型||前.内容.种类!=后.内容.种类||前.内容.状态!=后.内容.状态||前.内容.值!=后.内容.值)return false;
    }return true;
}
}
新本能需求材料读回 新_模拟世界类::读取本地需求材料(const 方法参数场景& 场景) const {
    using 材料状态=新本能需求材料状态;using 符合=正式内容符合状态;
    try {
        if(!实现_)return {};
        auto& 内部=*实现_;
        const auto 主体=场景.查询参数(1),特征=场景.查询参数(2),需求=场景.查询参数(3);
        {
            std::lock_guard 锁(内部.互斥);
            if(!内部.通用||场景.用途()!=新方法参数场景用途::模拟||场景.方法节点()!=内部.通用方法
                ||场景.全部角色绑定()!=内部.通用绑定||场景.全部角色绑定().size()!=3
                ||!主体||!特征||!需求||主体->信息类型!=新方法参数信息类型::存在
                ||特征->信息类型!=新方法参数信息类型::特征||需求->信息类型!=新方法参数信息类型::存在
                ||!内部.是普通存在参数(主体->信息节点)||!内部.本地需求.contains(需求->信息节点)
                ||读引用(特征->信息节点,所属)!=std::optional{主体->信息节点})return {};
            if(!内部.存在概念||!内部.特征概念||!内部.二次概念||!内部.本地执行主体||!内部.执行主体来源)
                throw 模拟需求规格未支持{"demand execution read providers or actor mapping absent"};
        }
        const auto 正式=场景.读取正式信息参数保护(3);
        if(正式.状态==正式信息参数读取状态::前提已变化)throw 模拟需求前提变化{};
        if(正式.状态==正式信息参数读取状态::资源失败)throw std::bad_alloc{};
        if(正式.状态!=正式信息参数读取状态::已读取||!正式.信息)
            throw 模拟需求规格未支持{"formal local demand parameter is not readable"};
        const auto 本地=读取模拟需求(需求->信息节点);
        const auto 实际特征=读取模拟特征(特征->信息节点);
        if(!本地.信息||!实际特征)throw std::logic_error("registered demand parameter missing");
        const auto& 内容=*本地.信息;
        if(内容.需求主体存在节点!=主体->信息节点||内容.执行域!=新需求执行域::被认知)return {};
        std::vector<新特征准确值> 已有引用=实际特征->历史不同值;
        if(实际特征->当前值)已有引用.push_back(*实际特征->当前值);
        std::set<稳定编码> 命中身份;
        for(const auto& 命中:实际特征->历史值命中信息) {
            const auto* 节点=std::get_if<稳定编码>(&命中.值);
            if(!节点||命中.命中次数<0||!命中身份.insert(*节点).second)throw std::logic_error("local demand feature hit record corrupt");
            已有引用.push_back(命中.值);
        }
        for(const auto& 引用:已有引用) {
            const auto* 节点=std::get_if<稳定编码>(&引用);
            if(!节点)throw std::logic_error("local demand feature contains a non-reference value");
            const auto 项=读取模拟需求(*节点);
            if(!项.信息)throw std::logic_error("local demand feature contains a foreign or missing identity");
            // 通用候选可组合出另一合法主体的D；这是不适用输入，必须在动作前筛掉。
            if(项.信息->需求主体存在节点!=主体->信息节点||项.信息->执行域!=新需求执行域::被认知)return {};
        }
        const auto 特征定义=内部.特征概念->读取特征概念定义保护(实际特征->特征概念节点);
        if(特征定义.前提已变化)throw 模拟需求前提变化{};
        if(特征定义.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(!特征定义.信息||特征定义.信息->定义.材料类型!=特征概念材料物理类型::需求节点引用
            ||特征定义.信息->定义.比较规则!=特征概念比较规则::需求身份精确)return {};
        if(!内容.认知来源)return {材料状态::材料待齐,{}};
        const auto 确认=读取模拟确认(内容.认知来源->内容确认节点);
        const auto 理解=读取模拟理解(内容.认知来源->理解节点);
        if(!确认.信息||!理解.信息)throw std::logic_error("local demand provenance missing");
        if(确认.信息->需重新确认)return {材料状态::需重新确认,{}};
        if(确认.信息->理解!=*理解.信息||理解.信息->来源人类节点!=主体->信息节点
            ||理解.信息->目标存在节点!=内容.目标存在节点||理解.信息->目标!=内容.目标
            ||理解.信息->来源要求节点!=内容.认知来源->来源要求节点)
            throw std::logic_error("local demand provenance differs from actual demand");
        const auto 结果概念=内部.存在概念->读取引用内容筹办概念();
        if(结果概念.状态==正式结构操作状态::前提已变化)throw 模拟需求前提变化{};
        if(结果概念.状态!=正式结构操作状态::已读取||!结果概念.信息)
            throw 模拟需求规格未支持{"recognized demand result concept absent"};
        const auto 目标核验=核验需求目标定义保护(内容.目标,*内部.特征概念,*内部.二次概念);
        if(目标核验.状态==新需求目标定义核验状态::前提已变化)throw 模拟需求前提变化{};
        if(目标核验.状态==新需求目标定义核验状态::资源失败)throw std::bad_alloc{};
        if(目标核验.状态==新需求目标定义核验状态::内部不一致)throw std::logic_error("local demand target definition corrupted");
        if(目标核验.状态!=新需求目标定义核验状态::已完成)return {材料状态::材料待齐,{}};
        新本能需求材料 输出;输出.服务对象=主体->信息节点;输出.需求特征=特征->信息节点;输出.需求节点=需求->信息节点;
        输出.动作前需求特征=*实际特征;输出.需求内容=正式.信息->直接内容组;输出.完整保护=正式.完整保护组;
        合并模拟需求保护(输出.完整保护,特征定义.预期节点组);
        合并模拟需求保护(输出.完整保护,结果概念.完整保护组);合并模拟需求保护(输出.完整保护,目标核验.完整保护);
        std::map<稳定编码,正式内容值> 值组;
        for(const auto& 项:输出.需求内容)if(!值组.emplace(项.特征类型,项.内容).second)throw std::logic_error("duplicate local demand content feature");
        const 正式直接内容读取器 读=[&](稳定编码 节点,稳定编码 类型) {
            if(节点!=输出.需求节点)throw std::logic_error("demand evaluation changed its target");
            const auto 项=值组.find(类型);if(项==值组.end())throw 正式内容符合状态::未支持;
            return 项->second;
        };
        const 正式引用身份核验器 身份=[&](基础概念原语 原语,const auto& 数值,const auto&,const 正式内容值& 值)->正式内容符合结果 {
            if(!值.值||!std::holds_alternative<稳定编码>(*值.值))return {符合::不符合,{}};
            const auto 节点=std::get<稳定编码>(*值.值);
            if(原语==基础概念原语::合法存在) {
                std::lock_guard 锁(内部.互斥);
                return {值.种类==正式内容值种类::存在引用&&内部.是普通存在参数(节点)
                    &&读引用(节点,所属).has_value() ? 符合::符合 : 符合::不符合,正式.完整保护组};
            }
            if(原语==基础概念原语::具名领域身份||原语==基础概念原语::正式信息身份) {
                std::lock_guard 锁(内部.互斥);
                bool 合法=false;
                if(值.种类==正式内容值种类::需求引用)合法=内部.本地需求.contains(节点);
                if(值.种类==正式内容值种类::语言理解引用)合法=节点==内容.认知来源->理解节点&&内部.本地理解.contains(节点);
                if(值.种类==正式内容值种类::内容确认引用)合法=节点==内容.认知来源->内容确认节点&&内部.本地确认.contains(节点);
                if(原语==基础概念原语::具名领域身份) {
                    if(数值.size()!=1)return {符合::未支持,{}};
                    合法=合法&&((数值.front()==2&&值.种类==正式内容值种类::语言理解引用)
                        ||(数值.front()==3&&值.种类==正式内容值种类::内容确认引用)
                        ||(数值.front()==15&&值.种类==正式内容值种类::需求引用));
                }return {合法 ? 符合::符合 : 符合::不符合,正式.完整保护组};
            }
            if(值.种类!=正式内容值种类::概念引用)return {符合::不符合,{}};
            if(原语==基础概念原语::合法FT根||原语==基础概念原语::合法特征概念) {
                const auto 定义=内部.特征概念->读取特征概念定义保护(节点);
                if(定义.前提已变化)return {符合::前提已变化,{}};
                if(定义.状态==基础结构操作状态::资源失败)return {符合::资源失败,{}};
                if(定义.状态!=基础结构操作状态::已完成&&定义.状态!=基础结构操作状态::节点不存在&&定义.状态!=基础结构操作状态::入口拒绝)
                    throw std::logic_error("formal feature identity provider failed");
                return {定义.信息&&(原语!=基础概念原语::合法FT根||定义.信息->类型根==节点) ? 符合::符合 : 符合::不符合,定义.预期节点组};
            }
            if(原语==基础概念原语::合法比较关系) {
                const auto 定义=内部.二次概念->读取二次特征定义保护(节点);
                if(定义.前提已变化)return {符合::前提已变化,{}};
                if(定义.状态==基础结构操作状态::资源失败)return {符合::资源失败,{}};
                if(定义.状态!=基础结构操作状态::已完成&&定义.状态!=基础结构操作状态::节点不存在&&定义.状态!=基础结构操作状态::入口拒绝)
                    throw std::logic_error("formal comparison identity provider failed");
                return {定义.信息 ? 符合::符合 : 符合::不符合,定义.预期节点组};
            }
            if(原语==基础概念原语::正式概念身份) {
                const 正式概念身份读取接口* 提供者[]{内部.特征概念,内部.二次概念,内部.存在概念};
                for(const auto* 提供:提供者) {
                    const auto 读取=提供->读取正式概念身份保护(节点);
                    if(读取.状态==正式概念读取状态::前提已变化)return {符合::前提已变化,{}};
                    if(读取.身份)return {符合::符合,读取.身份->完整保护组};
                }return {符合::不符合,{}};
            }
            return {符合::未支持,{}};
        };
        const auto 特征语义=[](const 特征概念信息& 值){return 服务要求特征语义快照{值.节点,值.材料类型,值.单位,值.比较规则};};
        const auto 目标类型=内部.特征概念->读取特征概念定义保护(内容.目标.目标特征类型概念节点);
        if(目标类型.前提已变化)throw 模拟需求前提变化{};
        if(目标类型.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(!目标类型.信息)throw 模拟需求前提变化{};
        if(理解.信息->固定语义.目标类型!=特征语义(目标类型.信息->定义))return {材料状态::需重新确认,{}};
        合并模拟需求保护(输出.完整保护,目标类型.预期节点组);
        if(内容.目标.目标形式==新需求目标形式::值) {
            const auto 域=内部.特征概念->读取特征概念定义保护(*内容.目标.目标值域概念节点);
            const auto 比较=内部.二次概念->读取二次特征定义保护(*内容.目标.目标比较关系概念节点);
            if(域.前提已变化||比较.前提已变化)throw 模拟需求前提变化{};
            if(域.状态==基础结构操作状态::资源失败||比较.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(!域.信息||!比较.信息)throw 模拟需求前提变化{};
            合并模拟需求保护(输出.完整保护,域.预期节点组);合并模拟需求保护(输出.完整保护,比较.预期节点组);
            const auto& 定义=*比较.信息;
            const std::vector<二次特征概念输入定义> 输入{{0,二次特征输入角色::左输入,二次特征输入来源种类::特征准确值,内容.目标.目标特征类型概念节点},
                {1,二次特征输入角色::右输入,二次特征输入来源种类::特征准确值,内容.目标.目标特征类型概念节点}};
            if(定义.算法!=二次特征算法::有向特征差值||定义.输入定义组!=输入
                ||定义.结果域种类组!=std::vector{二次特征结果域种类::包含于参照值域})
                throw 模拟需求规格未支持{"demand comparison algorithm/result domain is not supported"};
            const 服务要求值域语义快照 域语义{特征语义(域.信息->定义),域.信息->定义.值域};
            const 服务要求比较语义快照 比较语义{定义.节点,定义.二次特征类型根节点,定义.算法,定义.输入定义组,
                定义.参照特征概念节点,{特征语义(定义.输出特征概念),定义.输出特征概念.值域},定义.结果域种类组};
            if(理解.信息->固定语义.目标域!=std::optional{域语义}||理解.信息->固定语义.比较!=std::optional{比较语义})return {材料状态::需重新确认,{}};
            const auto& 根定义=目标类型.信息->定义;
            if(域.信息->类型根!=内容.目标.目标特征类型概念节点||域.信息->定义.材料类型!=根定义.材料类型
                ||域.信息->定义.单位!=根定义.单位||域.信息->定义.比较规则!=根定义.比较规则)return {材料状态::材料待齐,{}};
            const auto 包含=内部.特征概念->比较值域(目标类型.信息->类型根,根定义.值域,域.信息->定义.值域);
            if(包含.状态==特征概念规则状态::前提已变化)throw 模拟需求前提变化{};
            if(包含.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            if(!包含.关系||(*包含.关系!=特征概念值域关系::相等&&*包含.关系!=特征概念值域关系::左包含右))return {材料状态::材料待齐,{}};
        } else if(理解.信息->固定语义.目标域||理解.信息->固定语义.比较)return {材料状态::需重新确认,{}};
        const 正式关联约束核验器 关联=[&](稳定编码 节点,const 结构约束定义& 约束)->正式内容符合结果 {
            if(节点!=输出.需求节点)throw std::logic_error("demand relation evaluation changed instance");
            if(约束.原语!=结构约束原语::域属于FT&&约束.原语!=结构约束原语::比较适用于目标)return {符合::未支持,{}};
            std::vector<std::uint32_t> 槽=约束.原语==结构约束原语::域属于FT
                ? std::vector<std::uint32_t>{先天特征类型槽::目标域,先天特征类型槽::需求目标FT}
                : std::vector<std::uint32_t>{先天特征类型槽::目标比较,先天特征类型槽::需求目标FT,先天特征类型槽::目标域};
            std::vector<稳定编码> 预期;
            for(const auto 序号:槽) {
                const auto 类型=内部.特征概念->读取先天特征类型槽(序号);
                if(类型.状态==特征概念规则状态::资源失败)return {符合::资源失败,{}};
                if(类型.状态==特征概念规则状态::前提已变化)return {符合::前提已变化,{}};
                if(!类型.类型根)return {符合::未支持,{}};
                try{合并模拟需求保护(输出.完整保护,类型.完整保护组);}catch(const 模拟需求前提变化&){return {符合::前提已变化,{}};}
                预期.push_back(*类型.类型根);
            }
            if(约束.项目FT组!=预期||!约束.整数参数.empty()||!约束.引用参数.empty())return {符合::未支持,{}};
            return {符合::符合,目标核验.完整保护}; // 上方已核同一D的FT/域/RC和固定语义，不跨D拼接。
        };
        const auto 判定=内部.存在概念->判定正式结构内容(结果概念.信息->完整被认知需求,输出.需求节点,读,身份,关联);
        if(判定.状态==符合::前提已变化)throw 模拟需求前提变化{};
        if(判定.状态==符合::资源失败)throw std::bad_alloc{};
        if(判定.状态==符合::未支持)throw 模拟需求规格未支持{"recognized demand structure contains unsupported content constraints"};
        if(判定.状态!=符合::符合)return {材料状态::材料待齐,{}};
        合并模拟需求保护(输出.完整保护,判定.完整保护组);
        auto& 治理=输出.动作前需求;治理.节点=内容.节点;治理.根角色=内容.根角色;治理.父需求节点=内容.父需求节点;
        治理.来源类型=内容.来源类型;治理.权重=内容.权重;治理.活动状态=内容.活动状态;治理.服务要求确认组=内容.服务要求确认组;
        治理.完整目标=新需求完整目标{内容.目标存在节点,内容.目标};治理.外部要求节点=内容.外部要求节点;
        if(内容.外部要求节点)治理.外部要求材料=读取模拟要求(*内容.外部要求节点).信息;
        治理.需求主体存在节点=内容.需求主体存在节点;治理.执行域=内容.执行域;治理.认知来源=内容.认知来源;
        治理.信息完善阶段=新需求信息完善阶段::已完善;
        if(!模拟需求保护仍同(输出.完整保护))throw 模拟需求前提变化{};
        return {材料状态::已读取,std::move(输出)};
    } catch(const 模拟需求前提变化&) {return {材料状态::前提已变化,{}};}
}
新本能需求运行接口* 新_模拟世界类::获取需求运行接口() noexcept {
    if(!实现_)return nullptr;
    std::lock_guard 锁(实现_->互斥);
    return 实现_->通用&&实现_->本地执行主体&&实现_->执行主体来源&&实现_->源身份
        &&实现_->存在概念&&实现_->特征概念&&实现_->二次概念&&实现_->动作定义读取&&实现_->动作定义发布&&实现_->动作概念 ? this : nullptr;
}
新模拟需求执行预检结果 新_模拟世界类::预检需求执行(const 方法参数场景& 场景) const {
    using 预检状态=新模拟需求执行预检状态;
    if(!实现_)return {预检状态::依赖未就绪,"isolated demand operation provider is not configured"};
    {
        std::lock_guard 锁(实现_->互斥);
        if(!实现_->源身份||!实现_->存在概念||!实现_->特征概念||!实现_->二次概念
            ||!实现_->动作定义读取||!实现_->动作定义发布||!实现_->动作概念||!实现_->本地执行主体)
            return {预检状态::依赖未就绪,"isolated demand operation provider is not configured"};
    }
    try {
        const auto 材料=读取本地需求材料(场景);
        if(材料.状态==新本能需求材料状态::前提已变化)return {预检状态::前提已变化,{}};
        if(材料.状态==新本能需求材料状态::参数不合法)return {预检状态::规格未支持,"local H/F/D identities or subject do not match"};
        if(!材料.材料)return {预检状态::已就绪,{}}; // 业务待齐由原函数返回，非调用前伪造结果。
        const auto& 实际=*材料.材料;auto& 内部=*实现_;
        const auto 方法=内部.方法.获取方法(场景.方法节点());
        const auto 结果=内部.存在概念->读取引用内容筹办概念();
        if(!方法||!结果.信息||方法->方法头.可能结果概念组!=std::vector{结果.信息->完整被认知需求})
            return {预检状态::规格未支持,"demand result declaration differs from the supported formal structure"};
        std::lock_guard 锁(内部.互斥);
        std::optional<稳定编码> 首记录;
        for(const auto 节点:内部.通用节点)if(属于样例(节点,需求发布种类,内部.通用场景)
            &&读引用(节点,发布需求引用)==std::optional{实际.需求节点}&&!读引用(节点,首次发布记录引用)) {
            if(首记录)throw std::logic_error("multiple local first demand publications");首记录=节点;
        }
        const auto 值=新特征准确值{实际.需求节点};
        const bool 当前相同=实际.动作前需求特征.当前值==std::optional{值};
        const bool 历史含有=std::find(实际.动作前需求特征.历史不同值.begin(),实际.动作前需求特征.历史不同值.end(),值)!=实际.动作前需求特征.历史不同值.end();
        if((当前相同||历史含有)&&!首记录)return {预检状态::规格未支持,"initial demand reference has no local first-publication action"};
        if(历史含有&&!当前相同)return {预检状态::规格未支持,"reactivating a historical demand is not supported"};
        return {预检状态::已就绪,{}};
    }catch(const 模拟需求规格未支持& 错误){return {预检状态::规格未支持,错误.诊断};}
    catch(const 模拟需求前提变化&){return {预检状态::前提已变化,{}};}
}
新本能需求材料读回 新_模拟世界类::读取新需求材料(const 方法参数场景& 场景) const {
    if(const_cast<新_模拟世界类*>(this)->请求停止已成立())return {新本能需求材料状态::前提已变化,{}};
    try{return 读取本地需求材料(场景);}
    catch(const 模拟需求规格未支持&){return {新本能需求材料状态::前提已变化,{}};}
}
新本能需求材料状态 新_模拟世界类::复核新需求材料(const 方法参数场景& 场景,const 新本能需求材料& 原材料) const {
    const auto 现在=读取新需求材料(场景);
    if(现在.状态!=新本能需求材料状态::已读取||!现在.材料)return 现在.状态;
    if(!模拟需求业务相同(原材料,*现在.材料)||!模拟需求保护仍同(原材料.完整保护))return 新本能需求材料状态::前提已变化;
    auto& 内部=*实现_;std::lock_guard 锁(内部.互斥);
    if(!内部.当前调用||!内部.记录写回||内部.请求取消||读值(*内部.当前调用,调用窗口状态值)!=0)
        return 新本能需求材料状态::前提已变化;
    内部.需求动作许可=内部.当前调用;
    return 新本能需求材料状态::已读取;
}
稳定编码 新_模拟世界类::开始本次需求动作(const 方法参数场景& 场景) {
    auto& 内部=*实现_;
    for(;;) {
        新隔离调用身份 调用;稳定编码 主体,目标,目标特征,目标概念,主体概念;
        新模拟执行主体输入 来源;
        {
            std::lock_guard 锁(内部.互斥);调用=读取当前调用身份();
            if(内部.需求动作许可!=std::optional{调用.调用节点}||场景.全部角色绑定()!=内部.通用绑定
                ||!内部.本地执行主体||!内部.执行主体来源)throw std::logic_error("demand action without matching material permission");
            if(调用.主动作)return *调用.主动作;
            主体=*内部.本地执行主体;来源=*内部.执行主体来源;主体概念=来源.专属概念;
            if(!属于样例(主体,存在种类,内部.通用场景)||读引用(主体,特征类型)!=std::optional{来源.专属概念}
                ||!读引用(主体,所属))throw std::logic_error("local demand actor mapping corrupted");
            目标=场景.查询参数(1)->信息节点;目标特征=场景.查询参数(2)->信息节点;
            目标概念=内部.通用参数.at(目标).构造存在概念.value_or(内部.通用参数.at(目标).规格.范围概念节点);
        }
        const auto 特征定义=内部.特征概念->读取特征概念定义保护(读取模拟特征(目标特征)->特征概念节点);
        if(!特征定义.信息)throw std::logic_error("demand action feature definition missing");
        const 动作方法投影请求 请求{调用.方法节点,主体概念,目标概念,特征定义.信息->类型根,false};
        const auto 定义=内部.动作定义读取->读取动作方法投影(请求);
        const auto 概念=内部.动作定义发布->确保动作定义概念(请求);
        if(!概念.概念节点)throw std::logic_error("demand action definition not published");
        // 上述确保及actor落图都可能改变概念入向；只使用这里重新取得的保护。
        const auto 身份=内部.源身份->读取存在身份保护(来源.来源存在);
        const auto 类别=内部.存在概念->读取普通存在概念定义保护(来源.专属概念);
        const auto 动作保护=内部.动作概念->读取动作概念定义保护(*概念.概念节点);
        if(身份.前提已变化||类别.状态==正式概念读取状态::前提已变化||动作保护.状态==动作概念定义保护状态::前提已变化)continue;
        if(身份.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(!身份.信息||身份.信息->专属存在概念节点!=来源.专属概念||!类别.信息
            ||类别.信息->对应存在节点!=std::optional{来源.来源存在}||!动作保护.信息
            ||动作保护.信息->动作定义!=std::optional{定义})throw std::logic_error("demand action actor or formal definition changed identity");
        std::lock_guard 锁(内部.互斥);
        if(内部.当前调用!=std::optional{调用.调用节点}||!内部.记录写回||场景.全部角色绑定()!=内部.通用绑定
            ||内部.本地执行主体!=std::optional{主体})throw std::logic_error("demand action sample changed");
        if(内部.模拟步骤==(std::numeric_limits<std::int64_t>::max)())throw std::overflow_error("demand action time exhausted");
        const auto 时间=内部.模拟步骤+1;
        基础节点组追加请求 追加;
        try {合并模拟需求保护(追加.预期联合节点组,身份.预期节点组);合并模拟需求保护(追加.预期联合节点组,类别.完整保护组);
            合并模拟需求保护(追加.预期联合节点组,动作保护.完整保护组);}catch(const 模拟需求前提变化&){continue;}
        追加.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(动作种类)}},{字段组[根],模拟世界树根节点},
            {字段组[所属],目标},{字段组[对端引用],主体},{字段组[状态特征引用],目标特征},{字段组[方法引用],调用.方法节点},
            {字段组[来源引用],调用.调用节点},{字段组[特征类型],*概念.概念节点},
            {字段组[时间语义],基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)}},{字段组[步骤],基础值{时间}}}});
        追加.追加外部关系组.push_back({内部.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
        追加.追加字段组.push_back({调用.调用节点,字段组[调用主动作引用],基础组内节点引用{0}});
        内部.追加进展(追加);补齐隔离追加联合预期(追加);内部.预留追加(追加);
        const auto 发布=全局基础数据集.原子追加节点组(追加);
        if(发布.状态==基础节点组追加状态::预期不符)continue;
        if(发布.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
        if(发布.状态!=基础节点组追加状态::已完成||发布.新节点组.size()!=1)throw std::logic_error("demand action publication failed");
        内部.收集追加(发布);内部.模拟步骤=时间;内部.本次已进入动作=true;
        if(!读取本地动作(发布.新节点组.front()))throw std::logic_error("demand action readback failed");
        return 发布.新节点组.front();
    }
}
新本能需求发布读回 新_模拟世界类::发布新需求(const 方法参数场景& 场景,const 新本能需求材料& 原材料,稳定编码 动作) {
    新本能需求发布读回 输出;
    if(请求停止已成立())return 输出;
    新本能需求材料读回 读取;
    try{读取=读取本地需求材料(场景);}catch(const 模拟需求规格未支持&){return 输出;}
    if(读取.状态==新本能需求材料状态::需重新确认){输出.状态=新本能需求发布状态::需重新确认;return 输出;}
    if(读取.状态!=新本能需求材料状态::已读取||!读取.材料||!模拟需求业务相同(原材料,*读取.材料))return 输出;
    const auto& 材料=*读取.材料;auto& 内部=*实现_;std::unique_lock 锁(内部.互斥);
    const auto 调用=读取当前调用身份();const auto 实际动作=读取本地动作(动作);
    if(调用.主动作!=std::optional{动作}||!实际动作||实际动作->主题存在节点!=*内部.本地执行主体
        ||实际动作->动作目标存在节点!=材料.服务对象||实际动作->动作目标特征节点!=材料.需求特征)
        throw std::logic_error("demand publication action does not match its supplied target");
    const auto 结果场景=读引用(材料.服务对象,所属);if(!结果场景)throw std::logic_error("local demand subject scene absent");
    const auto 当前=材料.动作前需求特征.当前值;
    const auto 新值=新特征准确值{材料.需求节点};
    const bool 改变=当前!=std::optional{新值};
    std::optional<稳定编码> 首记录,首次动作,复用状态;
    for(const auto 节点:内部.通用节点)if(属于样例(节点,需求发布种类,内部.通用场景)
        &&读引用(节点,发布需求引用)==std::optional{材料.需求节点}&&!读引用(节点,首次发布记录引用)) {
        if(首记录)throw std::logic_error("local demand first publication is not unique");
        首记录=节点;首次动作=读引用(节点,首次发布动作引用);复用状态=读引用(节点,后状态引用);
    }
    if(!改变&&(!首记录||!首次动作||!复用状态))throw std::logic_error("known local demand has no first-publication source");
    if(改变&&首记录)throw std::logic_error("historical demand reactivation was not admitted");
    基础节点组追加请求 追加;追加.预期联合节点组=材料.完整保护;
    const auto 新建=[&](节点种类 种类) {
        const auto 位置=追加.新节点组.size();追加.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(种类)}},{字段组[根],模拟世界树根节点}}});
        追加.追加外部关系组.push_back({内部.通用场景,基础组内节点引用{位置},基础外部关系类型::父子});return 位置;
    };
    const auto 写=[&](std::size_t 节点,字段 键,基础组内字段内容 值){追加.新节点组[节点].字段组.push_back({字段组[键],std::move(值)});};
    if(内部.模拟步骤>(std::numeric_limits<std::int64_t>::max)()-2)throw std::overflow_error("demand publication time exhausted");
    const auto 时间=内部.模拟步骤+1;
    const auto 特征定义=内部.通用参数.at(材料.需求特征).规格.范围概念节点;
    const auto 写状态=[&](std::size_t 节点,稳定编码 值,std::int64_t 时刻) {
        写(节点,所属,材料.服务对象);写(节点,状态特征引用,材料.需求特征);写(节点,特征类型,特征定义);写(节点,当前材料,值);
        写(节点,步骤,基础值{时刻});写(节点,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});
    };
    std::optional<std::size_t> 后状态,前状态,变化;
    if(改变) {
        const auto 旧字段=唯一字段(材料.需求特征,当前材料);
        if(当前) {
            const auto* 旧=std::get_if<稳定编码>(&*当前);
            if(!旧||!旧字段)throw std::logic_error("local demand current reference corrupt");
            const auto 旧需求=读取模拟需求(*旧);
            if(!旧需求.信息||旧需求.信息->需求主体存在节点!=材料.服务对象||旧需求.信息->执行域!=新需求执行域::被认知)
                throw std::logic_error("local demand previous value belongs to a different subject");
            前状态=新建(状态种类);写状态(*前状态,*旧,实际动作->动作时间->纳秒-1);
            追加.替换字段组.push_back({旧字段->编码,材料.需求节点});
        } else {
            if(旧字段)throw std::logic_error("local demand current field disagrees with reader");
            追加.追加字段组.push_back({材料.需求特征,字段组[当前材料],材料.需求节点});
        }
        后状态=新建(状态种类);写状态(*后状态,材料.需求节点,时间);写(*后状态,动作引用,动作);
        if(前状态) {
            变化=新建(动态种类);写(*变化,所属,材料.服务对象);写(*变化,状态特征引用,材料.需求特征);
            写(*变化,前状态引用,基础组内节点引用{*前状态});写(*变化,后状态引用,基础组内节点引用{*后状态});
            写(*变化,动作引用,动作);写(*变化,来源引用,调用.调用节点);写(*变化,方法直接写入值,基础值{std::int64_t{1}});
            追加.追加字段组.push_back({动作,字段组[动态引用],基础组内节点引用{*变化}});
            追加.追加字段组.push_back({调用.调用节点,字段组[动态引用],基础组内节点引用{*变化}});
        }
        const auto& 历史=材料.动作前需求特征.历史不同值;
        std::set<稳定编码> 去重;
        for(const auto& 值:历史) {
            const auto* 节点=std::get_if<稳定编码>(&值);if(!节点)throw std::logic_error("demand history value is not a reference");
            const auto 项=读取模拟需求(*节点);
            if(!项.信息||项.信息->需求主体存在节点!=材料.服务对象||项.信息->执行域!=新需求执行域::被认知)
                throw std::logic_error("demand history contains foreign information");
            去重.insert(*节点);
        }
        std::set<稳定编码> 已有命中;
        for(const auto& 项:材料.动作前需求特征.历史值命中信息) {
            const auto* 值=std::get_if<稳定编码>(&项.值);
            if(!值||项.命中次数<0||!已有命中.insert(*值).second)throw std::logic_error("demand history hit counts corrupt");
        }
        const auto 加历史=[&](稳定编码 值) {
            if(去重.insert(值).second)追加.追加字段组.push_back({材料.需求特征,字段组[历史项引用],值});
            if(已有命中.insert(值).second) {
                const auto 命中=新建(语义快照种类);写(命中,结构引用,值);写(命中,命中次数值,基础值{std::int64_t{1}});
                追加.追加字段组.push_back({材料.需求特征,字段组[历史命中引用],基础组内节点引用{命中}});
            }
        };
        if(当前)加历史(std::get<稳定编码>(*当前));加历史(材料.需求节点);
    }
    const auto 记录=新建(需求发布种类);写(记录,所属,*结果场景);写(记录,主体引用,材料.服务对象);
    写(记录,状态特征引用,材料.需求特征);写(记录,发布需求引用,材料.需求节点);写(记录,来源引用,调用.调用节点);
    写(记录,动作引用,动作);写(记录,首次发布动作引用,首次动作.value_or(动作));
    写(记录,后状态引用,后状态 ? 基础组内字段内容{基础组内节点引用{*后状态}} : 基础组内字段内容{*复用状态});
    if(首记录)写(记录,首次发布记录引用,*首记录);
    if(前状态)写(记录,前状态引用,基础组内节点引用{*前状态});if(变化)写(记录,动态引用,基础组内节点引用{*变化});
    追加.追加字段组.push_back({调用.调用节点,字段组[需求发布成员],基础组内节点引用{记录}});
    内部.追加进展(追加);补齐隔离追加联合预期(追加);内部.预留追加(追加);
    const auto 发布=全局基础数据集.原子追加节点组(追加);
    if(发布.状态==基础节点组追加状态::预期不符)return 输出;
    if(发布.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
    if(发布.状态!=基础节点组追加状态::已完成)throw std::logic_error("local demand atomic publication failed");
    内部.收集追加(发布);内部.模拟步骤=时间;
    if(改变)if(auto* 槽=内部.准备写回槽(材料.需求特征))*槽=实现::写入来源::方法;
    const 新本能已发布需求结果 结果{材料.需求节点,材料.服务对象,材料.需求特征,*结果场景,动作,首次动作.value_or(动作)};
    锁.unlock();
    新本能已发布需求核验结果 核验;
    try{核验=核验本地需求结果(场景,调用.调用节点,结果,false);}
    catch(const 模拟需求前提变化&){}
    catch(const 模拟需求规格未支持&){}
    if(核验.状态==新本能已发布需求核验状态::前提已变化){输出.状态=新本能需求发布状态::已发布但当前读回变化;return 输出;}
    if(核验.状态!=新本能已发布需求核验状态::已核验)throw std::logic_error("local demand publication readback failed");
    输出.需求节点=材料.需求节点;输出.已发布需求=结果;
    if(后状态)输出.结果状态节点组.push_back(发布.新节点组[*后状态]);
    输出.状态=改变 ? 新本能需求发布状态::已落实并发布 : 新本能需求发布状态::已复用并发布;return 输出;
}
新本能已发布需求核验结果 新_模拟世界类::核验本地需求结果(const 方法参数场景& 场景,
    稳定编码 调用,const 新本能已发布需求结果& 结果,bool 要求关闭) const {
    using 状态=新本能已发布需求核验状态;
    if(!实现_)return {状态::已关闭,{},{}};
    auto& 内部=*实现_;
    新模拟执行主体输入 来源;
    稳定编码 执行主体,样例场景,方法,目标概念;
    std::set<稳定编码> 动作定义组;
    {
        std::lock_guard 锁(内部.互斥);
        if(!内部.通用)return {状态::已关闭,{},{}};
        if(内部.当前调用!=std::optional{调用}||场景.方法节点()!=内部.通用方法||场景.全部角色绑定()!=内部.通用绑定
            ||读值(调用,调用窗口状态值)!=(要求关闭 ? 1 : 0))return {};
        if(读值(调用,调用取消值)!=0)return {};
        if(!内部.执行主体来源||!内部.本地执行主体)throw std::logic_error("demand result actor mapping missing");
        来源=*内部.执行主体来源;执行主体=*内部.本地执行主体;样例场景=内部.通用场景;方法=内部.通用方法;
        const auto 主体=场景.查询参数(1);
        if(!主体||!内部.通用参数.contains(主体->信息节点))throw std::logic_error("demand result subject binding missing");
        const auto& 参数=内部.通用参数.at(主体->信息节点);
        目标概念=参数.构造存在概念.value_or(参数.规格.范围概念节点);
    }
    const auto 读取=读取本地需求材料(场景);
    if(读取.状态!=新本能需求材料状态::已读取||!读取.材料)return {};
    const auto& 材料=*读取.材料;auto 保护=材料.完整保护;
    if(材料.动作前需求特征.当前值!=std::optional{新特征准确值{材料.需求节点}})return {};
    const auto 身份=内部.源身份->读取存在身份保护(来源.来源存在);
    const auto 类别=内部.存在概念->读取普通存在概念定义保护(来源.专属概念);
    if(身份.前提已变化||类别.状态==正式概念读取状态::前提已变化)return {};
    if(身份.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(!身份.信息||身份.信息->专属存在概念节点!=来源.专属概念||!类别.信息
        ||类别.信息->对应存在节点!=std::optional{来源.来源存在})return {};
    合并模拟需求保护(保护,身份.预期节点组);合并模拟需求保护(保护,类别.完整保护组);
    {
        std::lock_guard 锁(内部.互斥);
        if(!内部.通用)return {状态::已关闭,{},{}};
        if(内部.当前调用!=std::optional{调用}||场景.全部角色绑定()!=内部.通用绑定
            ||内部.通用场景!=样例场景||内部.通用方法!=方法||内部.本地执行主体!=std::optional{执行主体}
            ||!内部.执行主体来源||内部.执行主体来源->来源存在!=来源.来源存在||内部.执行主体来源->专属概念!=来源.专属概念)return {};
        if(!属于样例(执行主体,存在种类,样例场景)||读引用(执行主体,特征类型)!=std::optional{来源.专属概念}
            ||!读引用(执行主体,所属))throw std::logic_error("demand result local actor identity corrupted");
        const auto 发布组=读引用组(调用,需求发布成员);
        if(发布组.size()!=1)throw std::logic_error("demand invocation lacks unique local publication");
        const auto 记录=发布组.front();
        if(!属于样例(记录,需求发布种类,内部.通用场景)||读引用(记录,来源引用)!=std::optional{调用})
            throw std::logic_error("demand publication source corrupted");
        const auto 首记录=读引用(记录,首次发布记录引用).value_or(记录);
        if(!属于样例(首记录,需求发布种类,内部.通用场景)||读引用(首记录,首次发布记录引用))
            throw std::logic_error("demand first-publication chain corrupted");
        const auto 首动作=必读引用(首记录,首次发布动作引用),动作=必读引用(记录,动作引用);
        const auto 首来源=必读引用(首记录,来源引用),后状态=必读引用(首记录,后状态引用);
        const auto 结果场景=必读引用(材料.服务对象,所属);
        const 新本能已发布需求结果 实际{材料.需求节点,材料.服务对象,材料.需求特征,结果场景,动作,首动作};
        if(结果!=实际||必读引用(记录,首次发布动作引用)!=首动作||必读引用(首记录,动作引用)!=首动作
            ||读引用(调用,调用主动作引用)!=std::optional{动作}||读引用(首来源,调用主动作引用)!=std::optional{首动作})
            throw std::logic_error("demand result identity does not match local source records");
        for(const auto 项:{记录,首记录})if(读引用(项,主体引用)!=std::optional{材料.服务对象}
            ||读引用(项,状态特征引用)!=std::optional{材料.需求特征}||读引用(项,发布需求引用)!=std::optional{材料.需求节点}
            ||读引用(项,所属)!=std::optional{结果场景}||读引用(项,后状态引用)!=std::optional{后状态})
            throw std::logic_error("demand publication target chain corrupted");
        const auto 本次动作=读取本地动作(动作),首次=读取本地动作(首动作);const auto 后=读取本地状态(后状态);
        if(!本次动作||!首次||!后||!属于样例(首来源,调用种类,内部.通用场景))throw std::logic_error("demand result action/state missing");
        for(const auto& 项:{*本次动作,*首次})if(项.主题存在节点!=*内部.本地执行主体||项.动作定义节点!=场景.方法节点()
            ||项.动作目标存在节点!=材料.服务对象||项.动作目标特征节点!=材料.需求特征||!项.动作时间)
            throw std::logic_error("demand action actor/method/target differs");
        for(const auto& 项:{*本次动作,*首次}) {
            if(!项.动态概念节点)throw std::logic_error("demand action formal concept missing");
            动作定义组.insert(*项.动态概念节点);
        }
        if(本次动作->动作来源节点!=调用||首次->动作来源节点!=首来源
            ||后->被描述存在节点!=材料.服务对象||后->来源实例特征节点!=材料.需求特征
            ||后->固定准确值!=新特征准确值{材料.需求节点}||后->特征类型概念节点!=材料.动作前需求特征.特征概念节点
            ||后->强时间.纳秒<=首次->动作时间->纳秒||读引用(后状态,动作引用)!=std::optional{首动作})
            throw std::logic_error("demand post-state differs from actual publication");
        const auto 前=读引用(首记录,前状态引用),变化=读引用(首记录,动态引用);
        if(前.has_value()!=变化.has_value())throw std::logic_error("demand first-value/change distinction corrupted");
        if(变化) {
            const auto 前值=读取本地状态(*前);const auto 实际变化=读取本地变化(*变化);
            if(!前值||!实际变化||前值->来源实例特征节点!=材料.需求特征||前值->被描述存在节点!=材料.服务对象
                ||前值->固定准确值==后->固定准确值||前值->强时间.纳秒>=首次->动作时间->纳秒
                ||实际变化->状态节点组!=std::vector{*前,后状态}||读引用(*变化,动作引用)!=std::optional{首动作}
                ||读引用(*变化,来源引用)!=std::optional{首来源}||读值(*变化,方法直接写入值)!=1
                ||首次->动作结果组!=std::vector{新动作结果关联{新动作结果关系类型::导致变化,*变化}})
                throw std::logic_error("demand change is not the actual two-state transition");
            const auto 来源变化=读引用组(首来源,动态引用);
            if(std::count(来源变化.begin(),来源变化.end(),*变化)!=1)throw std::logic_error("demand change not attributed to first call");
        } else if(!首次->动作结果组.empty())throw std::logic_error("first demand value fabricated a change");
        if(记录!=首记录&&(读引用(记录,前状态引用)||读引用(记录,动态引用)||!本次动作->动作结果组.empty()))
            throw std::logic_error("demand reuse fabricated another transition");
        if(std::find(材料.动作前需求特征.历史不同值.begin(),材料.动作前需求特征.历史不同值.end(),新特征准确值{材料.需求节点})
            ==材料.动作前需求特征.历史不同值.end())throw std::logic_error("actual demand absent from feature history");
    }
    const auto 特征=内部.特征概念->读取特征概念定义保护(材料.动作前需求特征.特征概念节点);
    if(特征.前提已变化)return {};
    if(特征.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(!特征.信息)throw std::logic_error("demand action target feature definition absent");
    合并模拟需求保护(保护,特征.预期节点组);
    const 动作方法投影请求 请求{方法,来源.专属概念,目标概念,特征.信息->类型根,false};
    const auto 预期定义=内部.动作定义读取->读取动作方法投影(请求);
    for(const auto 概念:动作定义组) {
        const auto 定义=内部.动作概念->读取动作概念定义保护(概念);
        if(定义.状态==动作概念定义保护状态::前提已变化)return {};
        if(定义.状态!=动作概念定义保护状态::已读取||!定义.信息||定义.完整保护组.empty()
            ||定义.信息->动作定义!=std::optional{预期定义})throw std::logic_error("demand action formal definition differs from actual method/actor/target");
        合并模拟需求保护(保护,定义.完整保护组);
    }
    if(!模拟需求保护仍同(保护))return {};
    return {状态::已核验,结果,std::move(保护)};
}
新本能已发布需求核验结果 新_模拟世界类::核验本次需求执行结果(const 方法参数场景& 场景,
    稳定编码 调用,const 新本能已发布需求结果& 结果) const noexcept {
    try{return 核验本地需求结果(场景,调用,结果,true);}
    catch(const 模拟需求前提变化&){return {};}
    catch(const 模拟需求规格未支持&){return {};}
    catch(const std::bad_alloc&){return {新本能已发布需求核验状态::资源失败,{},{}};}
    catch(const std::exception& 错误){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"核验模拟需求执行结果",L"本地发布来源、状态或身份损坏。",错误.what());}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"核验模拟需求执行结果",L"未知工程异常。");}
}
void 新_模拟世界类::释放有序样例() {
    if(!实现_)return;
    auto& s=*实现_;
    bool open=false;
    {std::lock_guard lock(s.互斥);open=s.通用&&s.记录写回;}
    // 关闭会锁外询问协作停止，不能持world锁调用该回调。
    if(open)(void)结束记录复制上下文读取();
    std::lock_guard lock(s.互斥);
    if(!s.通用)return;
    const auto r=全局基础数据集.原子删除节点结构(s.通用节点,s.通用字段,s.通用外部边);
    if(r!=基础结构操作状态::已完成) throw std::runtime_error("simulation temporary nodes still referenced");
    s.通用=false;s.通用执行分支.reset();s.通用绑定.clear();s.通用输入节点.clear();s.通用参数.clear();s.通用材料.clear();s.通用任务.clear();s.任务成员读取组.clear();
    s.本地需求.clear();s.本地理解.clear();s.本地确认.clear();s.本地要求.clear();s.本地语言场景.clear();s.内容参数位置.clear();s.内容读取组.clear();
    s.记录复制读取=false;s.已读取复制上下文=false;s.本次已进入动作=false;s.本次存在特征读取.clear();
    s.交互事实.clear();s.本次场景读取.clear();
    s.记录写回=false;s.写回窗口已关闭=false;s.最后写入来源组.clear();
    s.当前调用.reset();s.停止检查={};s.请求取消=false;
    s.执行主体来源.reset();s.本地执行主体.reset();s.需求动作许可.reset();
    s.消息定义保护={};
    s.通用节点.clear();s.通用字段.clear();s.通用外部边.clear();s.通用事件.clear();s.消息事件组.clear();s.消息序号=0;
}

std::optional<新外设消息信息> 新_模拟世界类::读取模拟消息(稳定编码 n) const {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||!属于样例(n,消息种类,s.通用场景))return {};
    const auto text=[&](稳定编码 node,字段 key) {
        const auto f=唯一字段(node,key);const auto* v=f?std::get_if<基础值>(&f->内容):nullptr;
        const auto* x=v?std::get_if<std::string>(&v->材料):nullptr;
        if(!x)throw std::runtime_error("local message string field unreadable");return *x;
    };
    const auto W=读引用(n,窗口引用),B=读引用(n,正文引用);const auto dir=读值(n,方向值),time=读值(n,步骤),sem=读值(n,时间语义);
    const auto seq=唯一字段(n,消息来源序号);const auto* value=seq?std::get_if<基础值>(&seq->内容):nullptr;
    const auto* serial=value?std::get_if<std::vector<std::uint64_t>>(&value->材料):nullptr;
    if(!W||!B||!dir||(*dir!=1&&*dir!=2&&*dir!=3)||!time||*time<0||!sem||*sem!=static_cast<std::int64_t>(新状态时间语义::抽象相对)
        ||!serial||serial->size()!=1||!serial->front()||!属于样例(*W,存在种类,s.通用场景)||!属于样例(*B,材料种类,s.通用场景))
        throw std::runtime_error("local message identity or time corrupt");
    auto body=text(*B,数值);
    if(!新_语言类::严格UTF8字节串有效(body))throw std::runtime_error("local message invalid UTF8");
    return 新外设消息信息{n,s.通用场景,*W,*B,{text(n,消息宿主),text(n,消息窗口标识),static_cast<新外设消息方向>(*dir),serial->front()},
        std::move(body),{新状态时间语义::抽象相对,*time}};
}

std::vector<新外设消息信息> 新_模拟世界类::读取模拟消息组(稳定编码 F) const {
    if(!实现_)throw std::invalid_argument("local message world unavailable");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||!s.通用参数.contains(F)||s.通用参数.at(F).规格.信息类型!=新方法参数信息类型::特征)
        throw std::invalid_argument("message collection is not a local feature");
    const auto d=s.方法.读取模拟特征范围(s.通用参数.at(F).规格.范围概念节点);
    if(!d||d->比较规则!=特征概念比较规则::消息集合身份判等)throw std::invalid_argument("local feature is not a message collection");
    std::set<稳定编码> ids;
    const auto material=[&](稳定编码 V) {
        if(!属于样例(V,材料种类,s.通用场景))throw std::runtime_error("message collection material outside sample");
        const auto f=唯一字段(V,数值);const auto* v=f?std::get_if<基础值>(&f->内容):nullptr;
        const auto* a=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
        if(!a)throw std::runtime_error("message collection payload corrupt");
        for(const auto n:*a)ids.insert(稳定编码{n});
    };
    const auto current=读引用(F,当前材料);if(!current)throw std::runtime_error("message current material unavailable");material(*current);
    for(const auto& f:全局基础数据集.查询字段(F,字段组[历史引用])) {
        const auto* n=std::get_if<稳定编码>(&f.内容);if(!n)throw std::runtime_error("message history reference corrupt");material(*n);
    }
    std::vector<新外设消息信息> out;
    for(const auto n:ids){const auto m=读取模拟消息(n);if(!m)throw std::runtime_error("local message collection contains foreign identity");out.push_back(*m);}
    return out;
}

void 新_模拟世界类::发布模拟消息(const 新方法模拟消息输入定义& e,const std::string& body,std::int64_t trigger) {
    auto& s=*实现_;
    const auto& proof=s.消息定义保护;
    if(proof.状态!=新_方法类::执行结构读取状态::已读取||proof.方法组.size()!=1)
        throw std::runtime_error("message input declaration protection unavailable");
    bool declared=false;for(const auto& plan:proof.方法组.front().初始模拟输入方案组)
        if(std::find(plan.消息组.begin(),plan.消息组.end(),e)!=plan.消息组.end())declared=true;
    if(!declared)throw std::runtime_error("message input declaration no longer present");
    const auto bodyDomain=s.方法.读取模拟特征范围(e.正文范围概念);
    if(!bodyDomain||bodyDomain->材料类型!=特征概念材料物理类型::UTF8字符串
        ||!新_语言类::严格UTF8字节串有效(body)||(e.正文非空&&body.empty()))
        throw std::runtime_error("message payload definition premise changed");
    if(const auto* ids=std::get_if<std::vector<稳定编码>>(&bodyDomain->值域)) {
        bool found=false;for(const auto id:*ids){const auto m=s.方法.读取模拟候选材料(id);if(!m)throw std::runtime_error("message payload finite material missing");
            if(m->材料==新特征值材料{body})found=true;}
        if(!found)throw std::runtime_error("message payload outside current formal domain");
    } else if(!std::holds_alternative<特征概念全材料值域>(bodyDomain->值域))throw std::runtime_error("message payload domain no longer supported");
    const auto at=[&](std::uint32_t p) {if(!p||p>s.通用绑定.size()||!s.通用绑定[p-1].有序引用)throw std::invalid_argument("message binding absent");return s.通用绑定[p-1].有序引用->信息节点;};
    const auto F=at(e.目标特征参数序号),W=at(e.窗口存在参数序号),peer=at(e.对端存在参数序号);
    (void)发布本地消息(F,W,peer,e.方向,body,trigger,{},{},false);
}
新隔离提交结果 新_模拟世界类::发布本地消息(稳定编码 F,稳定编码 W,稳定编码 peer,
    新外设消息方向 direction,const std::string& body,std::int64_t trigger,
    std::optional<std::uint64_t> serial,std::optional<稳定编码> action,bool method,
    const std::function<void(基础节点组追加请求&,稳定编码,std::size_t,std::size_t,std::uint64_t)>& extra) {
    auto& s=*实现_;
    const auto& proof=s.消息定义保护;
    if(proof.状态!=新_方法类::执行结构读取状态::已读取||proof.方法组.size()!=1)
        throw std::runtime_error("local message method definition unavailable");
    if(!新_语言类::严格UTF8字节串有效(body))throw std::invalid_argument("local message invalid UTF8");
    if(direction!=新外设消息方向::输出)(void)读取模拟消息组(F);
    const auto owner=读引用(F,所属),windowScene=读引用(W,所属);
    if(owner!=W||windowScene!=s.通用场景||!属于样例(peer,存在种类,s.通用场景))throw std::invalid_argument("message channel bindings disagree");
    const auto old=读引用(F,当前材料);const auto current=唯一字段(F,当前材料);
    const auto oldfield=old?唯一字段(*old,数值):std::nullopt;const auto* oldvalue=oldfield?std::get_if<基础值>(&oldfield->内容):nullptr;
    const auto* oldmembers=oldvalue?std::get_if<std::vector<std::uint64_t>>(&oldvalue->材料):nullptr;
    if(!old||!current||!oldmembers)throw std::runtime_error("message publication prior material unavailable");
    if(s.消息序号==(std::numeric_limits<std::uint64_t>::max)()||(std::max)(s.模拟步骤,trigger)>(std::numeric_limits<std::int64_t>::max)()-2)
        throw std::overflow_error("simulation message sequence exhausted");
    const auto seq=serial.value_or(s.消息序号+1);const auto t=(std::max)(s.模拟步骤,trigger)+2;
    if(!seq||(serial&&*serial>s.消息序号))throw std::runtime_error("local message submission sequence not reserved");
    const auto session=std::string("simulation:")+std::to_string(s.通用场景.值);
    const auto dialog=std::to_string(W.值)+":"+std::to_string(peer.值);
    基础节点组追加请求 q;
    q.预期联合节点组=proof.完整保护组;
    const auto field=[&](std::size_t n,字段 k,基础组内字段内容 v){q.新节点组[n].字段组.push_back({字段组[k],std::move(v)});};
    const auto node=[&](节点种类 type) {
        const auto n=q.新节点组.size();q.新节点组.emplace_back();
        field(n,类型,基础值{static_cast<std::int64_t>(type)});field(n,根,模拟世界树根节点);
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{n},基础外部关系类型::父子});return n;
    };
    std::optional<稳定编码> prior;
    std::int64_t priorTime=-1;
    for(const auto n:s.通用节点)if(属于样例(n,状态种类,s.通用场景)&&读引用(n,状态特征引用)==F
        &&读引用(n,所属)==W&&读引用(n,当前材料)==old&&读值(n,时间语义)==static_cast<std::int64_t>(新状态时间语义::抽象相对)) {
        const auto time=读值(n,步骤);if(!time||*time<0||*time>s.模拟步骤)throw std::runtime_error("message prior state time corrupt");
        if(*time==priorTime&&prior&&*prior!=n)throw std::runtime_error("ambiguous message prior state");
        if(*time>priorTime){prior=n;priorTime=*time;}
    }
    const auto B=node(材料种类),V=node(材料种类),S1=node(状态种类),D=node(动态种类);
    const std::optional<std::size_t> S0=prior?std::nullopt:std::optional{node(状态种类)};
    s.通用节点.reserve(s.通用节点.size()+6);s.通用字段.reserve(s.通用字段.size()+80);s.通用外部边.reserve(s.通用外部边.size()+6);
    // 与样例存在预分配一致：先保留一个尚无类型/字段/归属的私有身份，
    // 使U64成员材料可引用其准确身份；消息内容及可见挂接仍在同一事务出生。
    const auto M=全局基础数据集.新建节点();if(!有效(M))throw std::bad_alloc{};
    bool published=false;
    try {
        const auto mf=[&](字段 k,基础组内字段内容 v){q.追加字段组.push_back({M,字段组[k],std::move(v)});};
        mf(类型,基础值{static_cast<std::int64_t>(消息种类)});mf(根,模拟世界树根节点);
        q.追加外部关系组.push_back({s.通用场景,M,基础外部关系类型::父子});
        field(B,数值,基础值{body});mf(正文引用,基础组内节点引用{B});mf(窗口引用,W);mf(对端引用,peer);
        mf(方向值,基础值{static_cast<std::int64_t>(direction)});mf(步骤,基础值{t});mf(时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});
        mf(消息宿主,基础值{session});mf(消息窗口标识,基础值{dialog});mf(消息来源序号,基础值{std::vector<std::uint64_t>{seq}});
        auto members=*oldmembers;members.push_back(M.值);field(V,数值,基础值{std::move(members)});
        const auto FT=读引用(F,特征类型);if(!FT)throw std::runtime_error("message feature formal type absent");
        std::vector<std::size_t> states{S1};if(S0)states.push_back(*S0);
        for(const auto S:states) {
            field(S,所属,W);field(S,状态特征引用,F);field(S,特征类型,*FT);
            field(S,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});
            field(S,步骤,基础值{S==S1?t:t-1});
        }
        if(S0)field(*S0,当前材料,*old);field(S1,当前材料,基础组内节点引用{V});
        if(prior)field(D,前状态引用,*prior);else field(D,前状态引用,基础组内节点引用{*S0});
        field(D,后状态引用,基础组内节点引用{S1});field(D,状态特征引用,F);
        field(D,所属,W);field(D,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});field(D,步骤,基础值{t});
        if(action) {
            const auto A=读取本地动作(*action);
            if(!A||A->动作目标存在节点!=std::optional{W}||A->动作目标特征节点!=std::optional{F})
                throw std::runtime_error("local message action target differs");
            field(D,动作引用,*action);q.追加字段组.push_back({*action,字段组[动态引用],基础组内节点引用{D}});
        }
        if(s.记录写回) {
            if(!s.当前调用)throw std::runtime_error("local message call identity absent");
            field(D,来源引用,*s.当前调用);field(D,方法直接写入值,基础值{std::int64_t{method?1:0}});
            q.追加字段组.push_back({*s.当前调用,字段组[动态引用],基础组内节点引用{D}});
            s.追加进展(q);
        }
        if(extra)extra(q,M,S1,D,seq);
        q.删除字段关系组.push_back(current->编码);q.追加字段组.push_back({F,字段组[当前材料],基础组内节点引用{V}});
        bool history=false;for(const auto& f:全局基础数据集.查询字段(F,字段组[历史引用]))if(f.内容==基础字段内容{*old})history=true;
        if(!history)q.追加字段组.push_back({F,字段组[历史引用],*old});
        std::set<稳定编码> endpoints{F,W,peer,*old,M,s.通用场景,模拟世界树根节点};
        const auto add=[&](稳定编码 key,const 基础组内字段内容& value){endpoints.insert(key);if(const auto* n=std::get_if<稳定编码>(&value))endpoints.insert(*n);};
        for(const auto& n:q.新节点组)for(const auto& f:n.字段组)add(f.字段节点,f.内容);
        for(const auto& f:q.追加字段组){endpoints.insert(f.所属节点);add(f.字段节点,f.内容);}
        const auto snapshots=全局基础数据集.查询节点组完整结构({endpoints.begin(),endpoints.end()});
        if(snapshots.状态!=基础结构操作状态::已完成||snapshots.节点组.size()!=endpoints.size())throw std::runtime_error("message publication premise capture failed");
        for(const auto& n:snapshots.节点组){if(!n.快照)throw std::runtime_error("message publication endpoint missing");q.预期联合节点组.push_back(*n.快照);}
        合并模拟联合预期(q);
        补齐隔离追加联合预期(q);s.预留追加(q);
        const auto result=全局基础数据集.原子追加节点组(q);
        if(result.状态==基础节点组追加状态::内部不一致)published=true;
        if(result.状态!=基础节点组追加状态::已完成)throw std::runtime_error("simulation message atomic publication refused");
        published=true;s.通用节点.push_back(M);s.收集追加(result);
        s.通用字段.erase(std::remove(s.通用字段.begin(),s.通用字段.end(),current->编码),s.通用字段.end());
        s.通用材料.emplace(result.新节点组[B],材料物理类型::UTF8字符串);s.通用材料.emplace(result.新节点组[V],材料物理类型::U64数组);
        s.模拟步骤=t;if(!serial)s.消息序号=seq;
        if(auto* slot=s.准备写回槽(F))*slot=method?实现::写入来源::方法:实现::写入来源::环境;
        const auto read=读取模拟消息(M);const auto state=读引用(result.新节点组[S1],当前材料);
        if(!read||read->UTF8正文!=body||read->来源!=新外设消息来源键{session,dialog,direction,seq}
            ||read->正式记录时间.纳秒!=t||state!=result.新节点组[V]
            ||读引用(F,当前材料)!=state||!读引用(result.新节点组[D],前状态引用)||!读引用(result.新节点组[D],后状态引用))
            throw std::runtime_error("simulation message complete publication readback failed");
        return {seq,M,result.新节点组[S1],result.新节点组[D]};
    } catch(...) {
        if(published)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"发布模拟环境消息",L"消息事实已发表但未完整确认，保留现场且不重发。");
        if(!全局基础数据集.删除节点(M))throw std::runtime_error("unpublished private message cleanup failed");
        throw;
    }
}
std::optional<稳定编码> 新_模拟世界类::读取模拟存在来源(稳定编码 E) const {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.通用参数.find(E);
    if(!s.通用||it==s.通用参数.end()||!s.是普通存在参数(E))return {};
    return it->second.来源存在;
}
void 新_模拟世界类::开始记录复制上下文读取(const std::function<bool()>& stop) {
    if(!实现_)throw std::runtime_error("simulation owner unavailable");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用)throw std::runtime_error("simulation sample unavailable");
    if (s.记录写回 || s.记录复制读取)
        throw std::runtime_error("simulation call recording already active");
    std::map<稳定编码, 实现::写入来源> 新槽组;
    for (const auto& [节点, 参数] : s.通用参数)
        if (参数.规格.信息类型 == 新方法参数信息类型::特征)
            新槽组.emplace(节点, 实现::写入来源::未写入);
    for (const auto& [任务, 投影] : s.通用任务) {
        新槽组.emplace(投影.本地.状态.特征节点, 实现::写入来源::未写入);
        if (投影.本地.服务值)
            新槽组.emplace(投影.本地.服务值->特征节点, 实现::写入来源::未写入);
    }
    auto callback=stop;
    基础节点组追加请求 q;
    q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(调用种类)}},
        {字段组[根],模拟世界树根节点},{字段组[所属],s.通用场景},{字段组[方法引用],s.通用方法},
        {字段组[调用窗口状态值],基础值{std::int64_t{0}}},{字段组[调用取消值],基础值{std::int64_t{0}}},
        {字段组[调用进展值],基础值{std::int64_t{0}}}}});
    q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
    补齐隔离追加联合预期(q);s.预留追加(q);
    const auto made=全局基础数据集.原子追加节点组(q);
    if(made.状态!=基础节点组追加状态::已完成||made.新节点组.size()!=1)throw std::runtime_error("simulation call identity publication failed");
    s.收集追加(made);s.当前调用=made.新节点组.front();s.请求取消=false;s.停止检查=std::move(callback);
    s.最后写入来源组.swap(新槽组);
    s.需求动作许可.reset();
    s.写回窗口已关闭=false;s.记录写回=true;
    s.已读取复制上下文=false;s.任务成员读取组.clear();s.本次场景读取.clear();s.本次存在特征读取.clear();s.内容读取组.clear();s.本次已进入动作=false;s.记录复制读取=true;
}
bool 新_模拟世界类::结束记录复制上下文读取() {
    if(!实现_)throw std::runtime_error("simulation owner unavailable");
    (void)请求停止已成立();
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if (!s.记录写回 || !s.记录复制读取)
        throw std::runtime_error("simulation call recording is not active");
    const auto f=s.当前调用 ? 唯一字段(*s.当前调用,调用窗口状态值) : std::nullopt;
    const auto cancel=s.当前调用 ? 唯一字段(*s.当前调用,调用取消值) : std::nullopt;
    if(!f||!cancel)throw std::runtime_error("simulation call close identity missing");
    基础节点组追加请求 q;
    q.替换字段组.push_back({f->编码,基础值{std::int64_t{1}}});
    q.替换字段组.push_back({cancel->编码,基础值{std::int64_t{s.请求取消 ? 1 : 0}}});
    补齐隔离追加联合预期(q);
    if(全局基础数据集.原子追加节点组(q).状态!=基础节点组追加状态::已完成)
        throw std::runtime_error("simulation call close publication failed");
    s.记录写回=false;s.写回窗口已关闭=true;
    s.记录复制读取=false;s.停止检查={};return s.已读取复制上下文;
}
std::vector<稳定编码> 新_模拟世界类::读取本次写回特征组() const {
    if (!实现_) throw std::runtime_error("simulation owner unavailable");
    const auto& s=*实现_;std::lock_guard 锁(s.互斥);
    if (!s.通用 || s.记录写回 || !s.写回窗口已关闭)
        throw std::runtime_error("simulation write attribution requires a closed call window");
    std::vector<稳定编码> 结果;
    for (const auto& [特征, 来源] : s.最后写入来源组)
        if (来源 == 实现::写入来源::方法) 结果.push_back(特征);
    return 结果;
}
std::optional<新任务虚拟存在特征组> 新_模拟世界类::读取模拟集合任务治理(
    std::uint32_t 位置, 稳定编码 T) const noexcept {
    try {
    if (!实现_) return {};
    auto& s = *实现_; std::lock_guard lock(s.互斥);
    if (!s.通用 || !位置 || 位置 > s.通用绑定.size()) return {};
    const auto& 参数 = s.通用绑定[位置 - 1];
    if (!参数.有序引用 || 参数.有序引用->序号 != 位置
        || 参数.有序引用->信息类型 != 新方法参数信息类型::特征) return {};
    const auto F = s.通用参数.find(参数.有序引用->信息节点);
    const auto 定义 = F != s.通用参数.end() ? s.方法.读取模拟特征范围(F->second.规格.范围概念节点) : std::nullopt;
    if (!定义 || 定义->比较规则 != 特征概念比较规则::服务任务集合身份判等) return {};
    const auto V = 读引用(参数.有序引用->信息节点, 当前材料);
    const auto 材料 = V ? 读取模拟特征值材料(*V) : std::nullopt;
    const auto* 成员 = 材料 ? std::get_if<std::vector<std::uint64_t>>(&材料->材料) : nullptr;
    if (!成员 || std::find(成员->begin(), 成员->end(), T.值) == 成员->end()) return {};
    const auto it = s.通用任务.find(T);
    if (it == s.通用任务.end()) return {};
    const bool 记录 = s.记录复制读取;
    s.记录复制读取 = false; // 该已解析路径自身不算未解析owner读取。
    const auto result = 读取任务虚拟存在特征组(it->second.本地.引用);
    s.记录复制读取 = 记录;
    if (记录 && result) {
        const auto 旧 = std::find_if(s.任务成员读取组.begin(), s.任务成员读取组.end(), [&](const auto& x) {
            return x.集合参数序号 == 位置 && x.动作前.引用.任务节点 == T;
        });
        if (旧 == s.任务成员读取组.end()) s.任务成员读取组.push_back({位置, *result});
    }
    return result;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"记录模拟成员读取",L"成员路径读取或记录失败。",e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"记录模拟成员读取",L"成员路径读取或记录失败。");
    }
}
std::vector<新模拟任务成员读取> 新_模拟世界类::读取任务成员记录() const {
    if (!实现_) return {};
    std::lock_guard lock(实现_->互斥);
    return 实现_->任务成员读取组;
}
std::optional<std::vector<稳定编码>> 新_模拟世界类::读取模拟存在特征组(稳定编码 E) const noexcept {
    if(!实现_) return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.通用参数.find(E);
    if(!s.通用||it==s.通用参数.end()||!s.是普通存在参数(E)) return {};
    if(s.记录复制读取&&it->second.来源存在)s.已读取复制上下文=true;
    std::vector<稳定编码> f;
    for(const auto& [n,p]:s.通用参数)
        if(p.规格.信息类型==新方法参数信息类型::特征&&读引用(n,所属)==E) f.push_back(n);
    return f;
}
std::optional<稳定编码> 新_模拟世界类::读取模拟所属场景(稳定编码 E) const noexcept {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.通用参数.find(E);
    if(s.记录复制读取&&it!=s.通用参数.end()&&it->second.来源存在)
        s.已读取复制上下文=true; // 本轮投影不表达来源场景组织，不能据此形成长期知识。
    if(!s.通用||!s.通用参数.contains(E))return {};
    const auto scene=读引用(E,所属);
    if(scene&&(*scene==s.通用场景||s.本地语言场景.contains(*scene)
        ||(读值(*scene,类型)==场景种类&&std::find(s.通用节点.begin(),s.通用节点.end(),*scene)!=s.通用节点.end())))return scene;
    if(E==s.通用场景)return s.通用场景;
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟场景归属",L"本地存在所属场景不完整。");
}
稳定编码 新_模拟世界类::建立模拟动作() {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用)return {};
    基础节点组追加请求 q;
    q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(动作种类)}},
        {字段组[根],模拟世界树根节点},{字段组[方法引用],s.通用方法},
        {字段组[时间语义],基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)}},
        {字段组[步骤],基础值{++s.模拟步骤}}}});
    q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
    补齐模拟追加联合预期(q);
    const auto p=全局基础数据集.原子追加节点组(q);
    if(p.状态!=基础节点组追加状态::已完成)throw std::runtime_error("simulation action create failed");
    s.收集追加(p);if(s.记录写回)s.本次已进入动作=true;return p.新节点组.front();
}
namespace {
void 替换模拟数值(稳定编码 F,std::int64_t x) {
    const auto r=全局基础数据集.查询完整节点结构(F);
    const auto f=唯一字段(F,数值);
    if(!r.结构||!f) throw std::runtime_error("simulation numeric feature missing");
    基础节点组追加请求 q;q.预期既有节点组.push_back(*r.结构);
    q.替换字段组.push_back({f->编码,基础值{x}});
    补齐模拟追加联合预期(q);
    const auto p=全局基础数据集.原子追加节点组(q);
    if(p.状态!=基础节点组追加状态::已完成)throw std::runtime_error("simulation numeric publish failed");
}
}
void 新_模拟世界类::发布模拟数值变化(稳定编码 F,std::int64_t value,bool environment) {
    auto& s=*实现_;
    const auto owner=读引用(F,所属),FT=读引用(F,特征类型);
    const auto scene=owner?读引用(*owner,所属):std::nullopt;
    const auto old=读值(F,数值);
    const auto field=唯一字段(F,数值);
    if(!owner||!FT||!scene||!old||!field)throw std::runtime_error("isolated numeric change target incomplete");
    const auto prior=读取最新特征状态(*scene,*owner,F);
    if(prior&&*old==value)return;
    if(s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())throw std::overflow_error("isolated numeric time exhausted");
    const auto time=prior?s.模拟步骤+1:s.模拟步骤;
    基础节点组追加请求 q;
    if(*old!=value)q.替换字段组.push_back({field->编码,基础值{value}});
    const auto node=[&](节点种类 kind) {
        const auto n=q.新节点组.size();
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(kind)}},{字段组[根],模拟世界树根节点}}});
        q.追加外部关系组.push_back({*scene,基础组内节点引用{n},基础外部关系类型::父子});return n;
    };
    const auto state=node(状态种类);
    q.新节点组[state].字段组.insert(q.新节点组[state].字段组.end(),{
        {字段组[所属],*owner},{字段组[状态特征引用],F},{字段组[特征类型],*FT},
        {字段组[数值],基础值{value}},{字段组[步骤],基础值{time}},
        {字段组[时间语义],基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)}}});
    if(prior&&*old!=value) {
        const auto D=node(动态种类);
        q.新节点组[D].字段组.insert(q.新节点组[D].字段组.end(),{
            {字段组[所属],*owner},{字段组[状态特征引用],F},
            {字段组[前状态引用],prior->节点},{字段组[后状态引用],基础组内节点引用{state}}});
        if(s.记录写回&&s.当前调用) {
            q.新节点组[D].字段组.push_back({字段组[来源引用],*s.当前调用});
            q.新节点组[D].字段组.push_back({字段组[方法直接写入值],基础值{std::int64_t{environment?0:1}}});
            q.追加字段组.push_back({*s.当前调用,字段组[动态引用],基础组内节点引用{D}});
        }
    }
    if(*old!=value) {
        const auto history=全局基础数据集.查询字段(F,字段组[历史项引用]);
        for(const auto x:{*old,value}) {
            if(std::none_of(history.begin(),history.end(),[&](const auto& h){return h.内容==基础字段内容{基础值{x}};}))
                q.追加字段组.push_back({F,字段组[历史项引用],基础值{x}});
        }
    }
    s.追加进展(q);补齐隔离追加联合预期(q);s.预留追加(q);
    const auto published=全局基础数据集.原子追加节点组(q);
    if(published.状态!=基础节点组追加状态::已完成)throw std::runtime_error("isolated numeric F/S/D publication failed");
    s.收集追加(published);s.模拟步骤=time;
    if(auto* slot=s.准备写回槽(F))*slot=environment?实现::写入来源::环境:实现::写入来源::方法;
}
新模拟操作状态 新_模拟世界类::模拟递增计数(稳定编码 F,稳定编码 A) {
    if(!实现_)return 新模拟操作状态::输入不适用;
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||!s.通用参数.contains(F)||!属于样例(A,动作种类,s.通用场景)) return 新模拟操作状态::输入不适用;
    const auto& 输入=s.通用参数.at(F);
    if(输入.规格.序号==0&&输入.所属存在参数&&输入.所属存在参数<=s.通用绑定.size()) {
        const auto d=s.方法.读取模拟特征范围(输入.规格.范围概念节点);
        if(!d)throw std::logic_error("counter attached input definition unavailable");
        const auto 前=读取模拟存在参数特征(输入.所属存在参数,d->特征类型根);
        if(前.状态!=新方法参数特征读取状态::已读取||!前.特征||前.特征->节点!=F)
            return 新模拟操作状态::输入不适用;
    }
    const auto x=读值(F,数值);
    if(!x||*x<0||*x==(std::numeric_limits<std::int64_t>::max)())return 新模拟操作状态::输入不适用;
    auto* 写回槽=s.准备写回槽(F);
    替换模拟数值(F,*x+1);
    if (读值(F,数值) != *x+1) throw std::runtime_error("simulation counter write readback disagrees");
    if (写回槽) *写回槽=实现::写入来源::方法;
    return 新模拟操作状态::已完成;
}
新模拟操作状态 新_模拟世界类::推进模拟输入() {
    if(!实现_)return 新模拟操作状态::输入不适用;
    auto& s=*实现_;
    稳定编码 method{};std::optional<稳定编码> call;
    {std::lock_guard lock(s.互斥);method=s.通用方法;call=s.当前调用;}
    const auto proof=s.方法.读取方法执行结构闭包({method});
    std::lock_guard lock(s.互斥);
    if(!s.通用)return 新模拟操作状态::输入不适用;
    if(method!=s.通用方法||call!=s.当前调用||!s.记录写回)throw std::runtime_error("message input invocation changed");
    if(proof.状态!=新_方法类::执行结构读取状态::已读取||proof.方法组.size()!=1)throw std::runtime_error("message input definition unavailable");
    s.消息定义保护=proof;
    bool pending=false;
    for(auto& event:s.消息事件组)if(!event.已交付) {
        pending=true;
        if(!event.声明.触发)throw std::runtime_error("unpublished before-call message input remained");
        const auto& def=event.声明;
        const auto at=[&](std::uint32_t pos){return s.通用绑定.at(pos-1).有序引用->信息节点;};
        const auto F=at(def.触发->特征参数序号),W=at(def.窗口存在参数序号),peer=at(def.对端存在参数序号);
        const auto session=std::string("simulation:")+std::to_string(s.通用场景.值),dialog=std::to_string(W.值)+":"+std::to_string(peer.值);
        std::optional<新外设消息信息> trigger;
        for(const auto& msg:读取模拟消息组(F)) {
            if(event.触发基线.contains(msg.消息节点)||msg.窗口节点!=W||msg.来源.公开宿主会话!=session
                ||msg.来源.对话框标识!=dialog||msg.来源.方向!=def.触发->方向)continue;
            bool formal=false;
            for(const auto n:s.通用节点)if(属于样例(n,状态种类,s.通用场景)
                &&读引用(n,状态特征引用)==F&&读引用(n,所属)==W&&读值(n,步骤)==msg.正式记录时间.纳秒) {
                const auto v=读引用(n,当前材料);const auto f=v?唯一字段(*v,数值):std::nullopt;
                const auto* raw=f?std::get_if<基础值>(&f->内容):nullptr;const auto* a=raw?std::get_if<std::vector<std::uint64_t>>(&raw->材料):nullptr;
                if(a&&std::find(a->begin(),a->end(),msg.消息节点.值)!=a->end())formal=true;
            }
            if(!formal)throw std::runtime_error("trigger message lacks local formal state");
            const auto key=std::pair{msg.正式记录时间.纳秒,msg.来源.来源序号};
            if(trigger&&key==std::pair{trigger->正式记录时间.纳秒,trigger->来源.来源序号}&&msg.消息节点!=trigger->消息节点)
                throw std::runtime_error("ambiguous equal-order local messages");
            if(!trigger||key<std::pair{trigger->正式记录时间.纳秒,trigger->来源.来源序号})trigger=msg;
        }
        if(trigger){发布模拟消息(def,event.正文,trigger->正式记录时间.纳秒);event.已交付=true;return 新模拟操作状态::已完成;}
    }
    if(s.事件游标>=s.通用事件.size())return pending?新模拟操作状态::等待触发:新模拟操作状态::输入耗尽;
    const auto& e=s.通用事件[s.事件游标];
    if(!e.目标参数||e.目标参数>s.通用绑定.size())throw std::runtime_error("simulation event position");
    const auto& b=s.通用绑定[e.目标参数-1];
    if(!b.有序引用||b.有序引用->信息类型!=新方法参数信息类型::特征)throw std::runtime_error("simulation event kind");
    const auto 特征=b.有序引用->信息节点;
    auto* 写回槽=s.准备写回槽(特征);
    发布模拟数值变化(特征,e.值,true);
    if (读值(特征,数值) != e.值) throw std::runtime_error("simulation input event readback disagrees");
    if (写回槽) *写回槽=实现::写入来源::环境;
    ++s.事件游标;
    return 新模拟操作状态::已完成;
}
新模拟操作状态 新_模拟世界类::模拟集合加入成员(稳定编码 F,稳定编码 E,稳定编码 A) {
    if(!实现_)return 新模拟操作状态::输入不适用;
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||!s.通用参数.contains(F)||!s.通用参数.contains(E)
        ||!属于样例(A,动作种类,s.通用场景))return 新模拟操作状态::输入不适用;
    const auto& fInfo=s.通用参数.at(F);
    if(fInfo.规格.信息类型!=新方法参数信息类型::特征
        ||!s.是普通存在参数(E))
        return 新模拟操作状态::输入不适用;
    const auto definition=s.方法.读取模拟特征范围(fInfo.规格.范围概念节点);
    if(!definition||definition->比较规则!=特征概念比较规则::存在集合身份判等)
        return 新模拟操作状态::输入不适用; // 此端口只接收存在成员，不得污染任务集合。
    const auto old=读引用(F,当前材料);
    if(!old||!s.通用材料.contains(*old)||s.通用材料.at(*old)!=材料物理类型::U64数组) return 新模拟操作状态::输入不适用;
    const auto field=唯一字段(*old,数值);
    const auto* raw=field?std::get_if<基础值>(&field->内容):nullptr;
    const auto* a=raw?std::get_if<std::vector<std::uint64_t>>(&raw->材料):nullptr;
    if(!a)throw std::runtime_error("simulation collection payload");
    auto members=*a;
    if(std::find(members.begin(),members.end(),E.值)!=members.end())return 新模拟操作状态::已完成;
    members.push_back(E.值);std::sort(members.begin(),members.end());
    auto* 写回槽=s.准备写回槽(F);
    const auto structure=全局基础数据集.查询完整节点结构(F);
    const auto f=唯一字段(F,当前材料);
    if(!structure.结构||!f)throw std::runtime_error("simulation collection feature");
    基础节点组追加请求 q;q.预期既有节点组.push_back(*structure.结构);
    q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(材料种类)}},
        {字段组[根],模拟世界树根节点},{字段组[所属],F},{字段组[数值],基础值{members}}}});
    q.删除字段关系组.push_back(f->编码);
    q.追加字段组.push_back({F,字段组[当前材料],基础组内节点引用{0}});
    q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
    补齐模拟追加联合预期(q);
    const auto p=全局基础数据集.原子追加节点组(q);
    if(p.状态!=基础节点组追加状态::已完成)throw std::runtime_error("simulation collection publish failed");
    s.通用字段.erase(std::remove(s.通用字段.begin(),s.通用字段.end(),f->编码),s.通用字段.end());
    s.收集追加(p);s.通用材料.emplace(p.新节点组.front(),材料物理类型::U64数组);
    const auto 新材料=读引用(F,当前材料);
    const auto 后值=新材料 ? 读取模拟特征值材料(*新材料) : std::nullopt;
    const auto* 后成员=后值 ? std::get_if<std::vector<std::uint64_t>>(&后值->材料) : nullptr;
    if (!新材料 || *新材料 != p.新节点组.front() || !后成员 || *后成员 != members)
        throw std::runtime_error("simulation collection write readback disagrees");
    if (写回槽) *写回槽=实现::写入来源::方法;
    return 新模拟操作状态::已完成;
}
}

namespace 海中鱼巣 {
新本能交互运行接口* 新_模拟世界类::获取交互运行接口() noexcept {
    if(!实现_)return nullptr;
    std::lock_guard lock(实现_->互斥);return 实现_->通用 ? this : nullptr;
}
bool 新_模拟世界类::已请求停止() const noexcept {
    try {
        if(!实现_)return true;
        {
            std::lock_guard lock(实现_->互斥);
            if(!实现_->通用||!实现_->记录写回)return 实现_->请求取消;
        }
        // 同一停止路径负责在锁外询问协作回调并记录本调用取消；
        // 短本能没有等待循环，也必须能在动作前看到该信号。
        return const_cast<新_模拟世界类*>(this)->请求停止已成立();
    } catch(const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟方法停止信号",L"调用停止材料读取失败。",e.what());
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟方法停止信号",L"调用停止材料读取失败。");
    }
}
std::optional<新方法初始必要条件定义> 新_模拟世界类::读取本次必要条件(const 方法参数场景& scene) const {
    if(!实现_)return {};
    std::lock_guard lock(实现_->互斥);
    if(scene.用途()!=新方法参数场景用途::模拟||!核验模拟参数(scene.方法节点(),scene.全部角色绑定()))return {};
    if(scene.执行分支节点()!=实现_->通用执行分支)return {};
    const auto 有效声明=实现_->方法.读取执行分支必要条件(scene.方法节点(),scene.执行分支节点());
    if(有效声明.状态!=新方法执行分支读取状态::已读取)return {};
    return 有效声明.有效必要条件;
}
新交互求值状态 新_模拟世界类::读取绑定环境(const 方法参数场景& scene,新交互绑定& out) {
    if(!实现_)return 新交互求值状态::材料不足;
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!读取本次必要条件(scene))return 新交互求值状态::材料不足;
    for(const auto& fact:s.交互事实) {
        const auto& r=fact.输入.限定;
        const auto bound=[&](std::uint32_t p,稳定编码 n) {const auto* a=scene.查询参数(p);return a&&a->信息节点==n;};
        const auto& b=fact.过程.绑定;
        if(!bound(r.场景参数序号,b.场景)||!bound(r.主体参数序号,b.自我)
            ||!bound(r.对象参数序号,b.对端)||!bound(r.特征参数序号,b.交互者特征))continue;
        if(读值(b.场景,类型)!=场景种类||b.场景!=s.通用场景
            ||读引用(b.自我,所属)!=std::optional{b.场景}||读引用(b.对端,所属)!=std::optional{b.场景}
            ||读引用(b.交互者特征,所属)!=std::optional{b.自我})return 新交互求值状态::不符合;
        const auto f=s.通用参数.find(b.交互者特征);
        const auto d=f==s.通用参数.end() ? std::nullopt : s.方法.读取模拟特征范围(f->second.规格.范围概念节点);
        if(!d)throw std::logic_error("interaction feature formal type unavailable");
        const auto 定位=scene.读取存在参数特征(2,d->特征类型根);
        if(定位.状态==新方法参数特征读取状态::未声明||定位.状态==新方法参数特征读取状态::信息类型不符
            ||定位.状态==新方法参数特征读取状态::参数未绑定)
            throw std::logic_error("interaction feature ownership declaration differs");
        if((定位.状态!=新方法参数特征读取状态::已读取&&定位.状态!=新方法参数特征读取状态::当前值未取得)
            ||!定位.特征||定位.特征->节点!=b.交互者特征)return 新交互求值状态::前提变化;
        out=b;return 新交互求值状态::已取得;
    }
    return 新交互求值状态::材料不足;
}
std::optional<新特征信息> 新_模拟世界类::读取交互者(稳定编码 F) {
    const auto result=读取模拟特征(F);
    if(!result)throw std::runtime_error("simulation bound interactor feature read failed");
    return result;
}
新交互求值状态 新_模拟世界类::读取场景事实(const 新方法初始场景事实限定& r,
    const std::vector<新方法参数角色绑定>& bindings) const {
    if(!实现_)return 新交互求值状态::材料不足;
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!核验模拟参数(s.通用方法,bindings))return 新交互求值状态::前提变化;
    const auto 声明=s.方法.读取执行分支必要条件(s.通用方法,s.通用执行分支);
    if(声明.状态!=新方法执行分支读取状态::已读取||std::find(声明.有效必要条件.场景事实限定组.begin(),声明.有效必要条件.场景事实限定组.end(),r)==声明.有效必要条件.场景事实限定组.end())
        return 新交互求值状态::材料不足;
    for(const auto& fact:s.交互事实)if(fact.输入.限定==r) {
        for(const auto& old:fact.固定节点) {
            const auto current=全局基础数据集.查询节点组完整结构({old.结构.节点.编码});
            if(current.状态!=基础结构操作状态::已完成||current.节点组.size()!=1||!current.节点组.front().快照)
                throw std::runtime_error("simulation prior process snapshot unavailable");
            const auto& now=*current.节点组.front().快照;
            if(now.结构.字段组!=old.结构.字段组||now.入向字段组!=old.入向字段组
                ||now.入向外部关系组!=old.入向外部关系组||now.出向外部关系组!=old.出向外部关系组)
                return 新交互求值状态::前提变化;
        }
        // 该读取完整由正式14覆盖；只有此已核验闭包不记为未表达隐藏读取。
        const auto old=s.记录复制读取;s.记录复制读取=false;
        const auto F=读取模拟特征(fact.过程.绑定.交互者特征);s.记录复制读取=old;
        if(!F)return 新交互求值状态::材料不足;
        const auto state=核验消息连接过程(fact.过程,*F,fact.过程.绑定,fact.输入.构成);
        if(state==新交互求值状态::已取得&&s.记录复制读取
            &&std::find(s.本次场景读取.begin(),s.本次场景读取.end(),r)==s.本次场景读取.end())s.本次场景读取.push_back(r);
        return state;
    }
    return 新交互求值状态::材料不足;
}
std::vector<新方法初始场景事实限定> 新_模拟世界类::读取本次场景事实组() const {
    if(!实现_)return {};
    std::lock_guard lock(实现_->互斥);return 实现_->本次场景读取;
}
新存在概念匹配状态 新_模拟世界类::核验绑定存在概念(稳定编码 E,稳定编码 C) {
    if(!实现_)return 新存在概念匹配状态::材料不足;
    if(已请求停止())return 新存在概念匹配状态::材料不足;
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto it=s.通用参数.find(E);
    if(it==s.通用参数.end()||!s.是普通存在参数(E))return 新存在概念匹配状态::材料不足;
    const auto sourceConcept=it->second.构造存在概念.value_or(it->second.规格.范围概念节点);
    const auto source=s.方法.读取模拟存在范围(sourceConcept),target=s.方法.读取模拟存在范围(C);
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
    const auto 存在核验诊断=[&](const wchar_t* 步,int 值) {
        try {
            std::wostringstream t;
            t<<L"步骤="<<步<<L"；状态="<<值<<L"；E="<<E.值<<L"；来源C="<<sourceConcept.值<<L"；目标C="<<C.值
             <<L"；来源种类="<<(source ? static_cast<int>(source->种类) : -1)<<L"；目标种类="<<(target ? static_cast<int>(target->种类) : -1)
             <<L"；来源必需特征类型=";
            if(source)for(const auto& f:source->特征组)t<<f.特征类型概念.值<<L"/"<<f.具体特征概念.值<<L",";
            t<<L"；目标必需特征类型=";
            if(target)for(const auto& f:target->特征组)t<<f.特征类型概念.值<<L"/"<<f.具体特征概念.值<<L",";
            (void)记录调试日志(L"SIMULATION_PROGRESS",L"模拟存在核验",L"未匹配",t.str());
        } catch(...) {}
    };
#else
    const auto 存在核验诊断=[](const wchar_t*,int) {};
#endif
    if(!source||!target)throw std::runtime_error("simulation existence formal definition unavailable");
    // 本地存在两类来源：复制专属原型（专属C），或按正式定义构造的具体实例（构造C为抽象）。
    // 复制专属原型的样例在隔离世界中代表该原型，其专属概念即来源C；归属与现实算法（CONCEPT-INFO-MEMBERSHIP）一致
    // （2026-11-08用户裁决）：实例关系（来源C沿上位链到达C）即属于；C为另一专属概念按身份不属于；
    // 否则只按C的必需特征逐项定义匹配。
    bool 仅目标定义匹配=false;
    // 构造实例的归类证明是构造C沿正式上下位链属于目标C；随后同样逐项比较目标必需特征，不因同概念直接判匹配。
    if(source->种类==存在概念种类::抽象) {
        std::set<稳定编码> 已访;
        std::function<bool(稳定编码)> 属于=[&](稳定编码 c)->bool {
            if(c==C)return true;
            if(!已访.insert(c).second)return false;
            const auto r=c==sourceConcept ? source : s.方法.读取模拟存在范围(c);
            if(!r)throw std::runtime_error("simulation existence classification definition unavailable");
            return std::any_of(r->上位概念组.begin(),r->上位概念组.end(),[&](稳定编码 p){return 属于(p);});
        };
        if(!属于(sourceConcept)){存在核验诊断(L"上位链不属于",3);return 新存在概念匹配状态::材料不足;}
    } else {
        std::set<稳定编码> 已访;
        std::vector<稳定编码> 待访{sourceConcept};
        while(!待访.empty()) {
            const auto c=待访.back();待访.pop_back();
            if(!已访.insert(c).second)continue;
            if(c==C)return 新存在概念匹配状态::匹配;
            const auto r=c==sourceConcept ? source : s.方法.读取模拟存在范围(c);
            if(!r)throw std::runtime_error("simulation existence instance-chain definition unavailable");
            待访.insert(待访.end(),r->上位概念组.begin(),r->上位概念组.end());
        }
        if(target->种类==存在概念种类::专属){存在核验诊断(L"身份不属于",2);return 新存在概念匹配状态::不符合;}
        仅目标定义匹配=true;
    }
    新存在概念匹配材料 material;material.存在节点=E;material.专属存在概念节点=sourceConcept;material.目标存在概念节点=C;
    auto required=source->特征组;
    for(const auto& f:target->特征组)if(std::none_of(required.begin(),required.end(),[&](const auto& x){return x.特征类型概念==f.特征类型概念;}))required.push_back(f);
    for(const auto& f:required) {
        const auto d=s.方法.读取模拟特征范围(f.具体特征概念);
        if(!d)throw std::runtime_error("simulation required feature definition unavailable");
        material.必需特征组.push_back({f,*d});
    }
    // 此完整存在读取由C中的存在概念约束表达；不放过任何额外方法读取。
    struct 复制读取恢复 final {bool& 标志;bool 原值;~复制读取恢复(){标志=原值;}} 恢复{s.记录复制读取,s.记录复制读取};
    s.记录复制读取=false;
    const auto features=读取模拟存在特征组(E);
    if(features)for(const auto F:*features) {
        const auto f=读取模拟特征(F);const auto d=f?s.方法.读取模拟特征范围(f->特征概念节点):std::nullopt;
        if(!f||!d)throw std::runtime_error("simulation local existence feature unavailable");
        新特征匹配实例快照 snap;snap.特征节点=F;snap.上级特征节点=f->上级特征节点;
        snap.定义=*d;snap.实例单位=f->实例单位;snap.当前值=f->当前值;snap.实例区间值=f->实例区间值;
        if(f->当前值&&d->材料类型!=特征概念材料物理类型::I64标量&&d->材料类型!=特征概念材料物理类型::存在节点引用) {
            const auto* V=std::get_if<稳定编码>(&*f->当前值);
            if(V)snap.当前完整材料=读取模拟特征值材料(*V);
        }
        material.实际特征组.push_back(std::move(snap));
    }
    using 匹配状态=新存在概念匹配状态;
    const auto 本地集合=[&](稳定编码 材料节点,const 新特征值信息& 材料) {
        const auto* 成员组=std::get_if<std::vector<std::uint64_t>>(&材料.材料);
        if(!有效(材料节点)||材料.节点!=材料节点||材料.物理类型!=材料物理类型::U64数组||!成员组)
            throw std::runtime_error("local task collection material identity or payload invalid");
        std::set<稳定编码> 来源;
        for(const auto 成员:*成员组){
            const auto 任务=s.通用任务.find(稳定编码{成员});
            if(任务==s.通用任务.end()||!有效(任务->second.来源)||任务->second.本地.引用.任务节点!=任务->first
                ||!有效(任务->second.本地.引用.虚拟存在节点)||!有效(任务->second.本地.引用.状态特征节点)
                ||任务->second.本地.状态.特征节点!=任务->second.本地.引用.状态特征节点
                ||std::count_if(s.通用任务.begin(),s.通用任务.end(),[&](const auto& 项){return 项.second.来源==任务->second.来源;})!=1)
                throw std::runtime_error("local task collection has no unique consistent source mapping");
            来源.insert(任务->second.来源);
        }
        return std::pair{true,std::move(来源)};
    };
    const auto 正式域=[&](const 特征概念值域& 值域) {
        std::vector<std::set<稳定编码>> 候选;
        if(std::holds_alternative<特征概念全材料值域>(值域))return std::pair{true,候选};
        const auto* 材料组=std::get_if<std::vector<稳定编码>>(&值域);
        if(!材料组)return std::pair{false,候选};
        bool 完整=true;
        for(const auto 材料节点:*材料组){
            const auto 材料=s.方法.读取模拟候选材料(材料节点);
            const auto* 成员组=材料 ? std::get_if<std::vector<std::uint64_t>>(&材料->材料) : nullptr;
            if(!有效(材料节点)||!材料||材料->节点!=材料节点||材料->物理类型!=材料物理类型::U64数组||!成员组)
                throw std::runtime_error("formal finite task collection material malformed");
            std::set<稳定编码> 组;
            for(const auto 成员:*成员组){
                if(!成员)throw std::runtime_error("formal finite task collection has empty identity");
                if(std::none_of(s.通用任务.begin(),s.通用任务.end(),[&](const auto& 项){return 项.second.来源==稳定编码{成员};}))完整=false;
                组.insert(稳定编码{成员});
            }
            候选.push_back(std::move(组));
        }
        return std::pair{完整,std::move(候选)};
    };
    const auto 窄比较=[&](const 新存在概念必需特征快照& 必需)->std::pair<bool,匹配状态> {
        const auto& 定义=必需.定义;
        const 新特征匹配实例快照* 实际=nullptr;
        for(const auto& 特征:material.实际特征组)if(特征.定义.特征类型根==定义.特征类型根){
            if(实际)throw std::runtime_error("duplicate local task feature type");实际=&特征;
        }
        if(!实际||!实际->当前值||!实际->当前完整材料)return {false,匹配状态::材料不足};
        if(!有效(实际->特征节点)||!有效(实际->定义.概念节点)||!有效(实际->定义.特征类型根)||实际->上级特征节点)
            throw std::runtime_error("local task feature has invalid top-level identity");
        if(实际->定义.材料类型!=特征概念材料物理类型::U64数组
            ||实际->定义.比较规则!=特征概念比较规则::服务任务集合身份判等
            ||实际->定义.单位!=定义.单位||实际->实例单位!=实际->定义.单位)return {false,匹配状态::材料不足};
        const auto* 材料节点=std::get_if<稳定编码>(&*实际->当前值);
        if(!材料节点)throw std::runtime_error("local task collection current value is not material identity");
        const auto 当前=本地集合(*材料节点,*实际->当前完整材料);
        if(!当前.first)return {false,匹配状态::材料不足};
        bool 实例域完整=true;
        if(实际->实例区间值&&!std::holds_alternative<特征概念全材料值域>(*实际->实例区间值)){
            const auto* 有限组=std::get_if<std::vector<稳定编码>>(&*实际->实例区间值);
            if(!有限组)实例域完整=false;
            else for(const auto 本地材料节点:*有限组){
                const auto 材料=读取模拟特征值材料(本地材料节点);
                if(!材料)throw std::runtime_error("local finite task domain material unavailable");
                if(!本地集合(本地材料节点,*材料).first)实例域完整=false;
            }
        }
        const auto 实际域=正式域(实际->定义.值域);
        const auto 目标域=正式域(定义.值域);
        if(!实例域完整||!实际域.first||!目标域.first)return {false,匹配状态::材料不足};
        if(std::holds_alternative<特征概念全材料值域>(定义.值域))return {true,匹配状态::匹配};
        if(!std::holds_alternative<std::vector<稳定编码>>(定义.值域))return {false,匹配状态::材料不足};
        const bool 相等=std::find(目标域.second.begin(),目标域.second.end(),当前.second)!=目标域.second.end();
        return {true,相等 ? 匹配状态::匹配 : 匹配状态::不符合};
    };
    const auto 比较阶段=[&]() {
        std::set<稳定编码> 已见;
        for(const auto& 特征:material.必需特征组){
            if(!有效(特征.来源.特征类型概念)||!有效(特征.来源.具体特征概念)
                ||特征.定义.特征类型根!=特征.来源.特征类型概念||特征.定义.概念节点!=特征.来源.具体特征概念
                ||!已见.insert(特征.来源.特征类型概念).second)
                throw std::runtime_error("simulation required feature identities or duplicates invalid");
        }
        auto 工作=material;工作.必需特征组.clear();bool 不符合=false,不足=false;
        for(const auto& 特征:material.必需特征组){
            if(特征.定义.比较规则==特征概念比较规则::服务任务集合身份判等
                &&特征.定义.材料类型==特征概念材料物理类型::U64数组){
                const auto 结论=窄比较(特征);
                if(结论.first){不符合|=结论.second==匹配状态::不符合;continue;}
                不足=true;
            }
            工作.必需特征组.push_back(特征);
        }
        const auto 通用状态=s.方法.比较模拟存在匹配材料(工作).状态;
        switch(通用状态){case 匹配状态::匹配:break;case 匹配状态::不符合:不符合=true;break;case 匹配状态::材料不足:不足=true;break;
        default:throw std::runtime_error("simulation existence matcher returned unknown state");}
        return 不符合 ? 匹配状态::不符合 : 不足 ? 匹配状态::材料不足 : 匹配状态::匹配;
    };
    if(仅目标定义匹配) {
        material.必需特征组.clear();
        for(const auto& f:target->特征组) {
            const auto d=s.方法.读取模拟特征范围(f.具体特征概念);
            if(!d)throw std::runtime_error("simulation target feature definition unavailable");
            material.必需特征组.push_back({f,*d});
        }
        const auto 定义匹配=比较阶段();
        if(定义匹配!=匹配状态::匹配)存在核验诊断(L"定义匹配",static_cast<int>(定义匹配));
        return 定义匹配;
    }
    const auto first=比较阶段();
    if(first!=匹配状态::匹配){存在核验诊断(L"第一比较阶段",static_cast<int>(first));return first;}
    material.必需特征组.clear();
    for(const auto& f:target->特征组) {
        const auto d=s.方法.读取模拟特征范围(f.具体特征概念);
        if(!d)throw std::runtime_error("simulation target feature definition unavailable");
        material.必需特征组.push_back({f,*d});
    }
    const auto second=比较阶段();
    if(second!=匹配状态::匹配)存在核验诊断(L"第二比较阶段",static_cast<int>(second));
    return second;
}
稳定编码 新_模拟世界类::建立本次动作(const 方法参数场景& scene,const 新交互绑定& b) {
    // 写回目标可以是环境交互者特征，或自我持有的其它本地特征参数（获取服务对象写参数5，R93）。
    新交互绑定 actual;if(读取绑定环境(scene,actual)!=新交互求值状态::已取得||actual.自我!=b.自我)return {};
    if(actual.交互者特征!=b.交互者特征&&(!实现_||!实现_->通用参数.contains(b.交互者特征)
        ||读引用(b.交互者特征,所属)!=std::optional{b.自我}))return {};
    if(实现_&&实现_->动作定义读取&&实现_->动作定义发布&&实现_->动作概念&&实现_->记录写回) {
        const auto F=读取模拟特征(b.交互者特征);
        const auto d=F?实现_->方法.读取模拟特征范围(F->特征概念节点):std::nullopt;
        if(!d)throw std::runtime_error("isolation interactor action type unavailable");
        return 建立阶段动作(scene,{2,d->特征类型根,新隔离动作阶段::主动作});
    }
    const auto A=建立模拟动作();if(!有效(A))return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    for(const auto& pair:std::array{std::pair{所属,b.自我},std::pair{状态特征引用,b.交互者特征}}) {
        const auto f=全局基础数据集.添加字段节点(A,字段组[pair.first],pair.second);
        if(!有效(f))throw std::runtime_error("simulation current action binding publication failed");
        s.通用字段.push_back(f);
    }
    return A;
}
新交互写入读回 新_模拟世界类::写入交互者(const 新交互绑定& b,稳定编码 A) {
    新交互写入读回 result;if(!实现_)return result;
    auto& s=*实现_;std::unique_lock lock(s.互斥);
    if(!属于样例(A,动作种类,s.通用场景)||读引用(A,所属)!=std::optional{b.自我}
        ||读引用(A,状态特征引用)!=std::optional{b.交互者特征})return result;
    const auto captured=全局基础数据集.查询完整节点结构(b.交互者特征);
    if(!captured.结构)throw std::runtime_error("simulation interactor preparation feature unavailable");
    const auto call=s.当前调用;
    lock.unlock();
    const auto f=读取模拟特征(b.交互者特征);
    const auto d=f?s.方法.读取模拟特征范围(f->特征概念节点):std::nullopt;
    lock.lock();
    const auto refreshed=全局基础数据集.查询完整节点结构(b.交互者特征);
    if(!refreshed.结构)throw std::runtime_error("simulation interactor preparation readback unavailable");
    if(s.当前调用!=call||refreshed.结构->节点!=captured.结构->节点||refreshed.结构->字段组!=captured.结构->字段组) {
        result.状态=新交互求值状态::前提变化;return result;
    }
    if(!f||!d||d->材料类型!=特征概念材料物理类型::存在节点引用||!s.通用参数.contains(b.对端))return result;
    auto* slot=s.准备写回槽(b.交互者特征);
    const auto before=全局基础数据集.查询完整节点结构(b.交互者特征);
    const auto current=唯一字段(b.交互者特征,当前材料);
    if(!before.结构)throw std::runtime_error("simulation interactor feature missing");
    const bool changed=f->当前值!=std::optional<新特征准确值>{新特征准确值{b.对端}};
    if(!changed) {
        const auto after=读取交互者(b.交互者特征);
        if(after->当前值!=std::optional<新特征准确值>{新特征准确值{b.对端}})
            throw std::runtime_error("simulation unchanged interactor confirmation failed");
        if(slot)*slot=实现::写入来源::方法;
        result.状态=新交互求值状态::已取得;return result;
    }
    基础节点组追加请求 q;q.预期既有节点组.push_back(*before.结构);
    if(current)q.替换字段组.push_back({current->编码,b.对端});
    else q.追加字段组.push_back({b.交互者特征,字段组[当前材料],b.对端});
    const auto add=[&](节点种类 kind) {
        const auto n=q.新节点组.size();q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(kind)}},{字段组[根],模拟世界树根节点}}});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{n},基础外部关系类型::父子});return n;
    };
    std::optional<std::size_t> previous;
    std::optional<稳定编码> prior;
    if(f->当前值) {
        const auto* old=std::get_if<稳定编码>(&*f->当前值);
        if(!old)throw std::runtime_error("simulation old interactor reference shape invalid");
        std::int64_t latest=-1;
        for(const auto n:s.通用节点)if(属于样例(n,状态种类,s.通用场景)&&读引用(n,状态特征引用)==std::optional{b.交互者特征}
            &&读引用(n,当前材料)==std::optional{*old}) {
            const auto state=读取本地状态(n);
            if(state&&state->强时间.纳秒>latest){prior=n;latest=state->强时间.纳秒;}
        }
        if(!prior) {
            previous=add(状态种类);
            q.新节点组[*previous].字段组.insert(q.新节点组[*previous].字段组.end(),{{字段组[所属],b.自我},{字段组[状态特征引用],b.交互者特征},
                {字段组[特征类型],d->概念节点},{字段组[当前材料],*old},{字段组[步骤],基础值{s.模拟步骤}},
                {字段组[时间语义],基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)}}});
        }
    }
    if(s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())throw std::overflow_error("isolation interactor time exhausted");
    const auto time=s.模拟步骤+1;
    const auto state=add(状态种类);
    q.新节点组[state].字段组.insert(q.新节点组[state].字段组.end(),{{字段组[所属],b.自我},{字段组[状态特征引用],b.交互者特征},
        {字段组[特征类型],d->概念节点},{字段组[当前材料],b.对端},{字段组[动作引用],A},{字段组[步骤],基础值{time}},
        {字段组[时间语义],基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)}}});
    std::optional<std::size_t> dynamic;
    dynamic=add(动态种类);
    q.新节点组[*dynamic].字段组.insert(q.新节点组[*dynamic].字段组.end(),{{字段组[所属],b.自我},{字段组[状态特征引用],b.交互者特征},
        {字段组[动作引用],A},{字段组[后状态引用],基础组内节点引用{state}}});
    if(previous)q.新节点组[*dynamic].字段组.push_back({字段组[前状态引用],基础组内节点引用{*previous}});
    if(prior)q.新节点组[*dynamic].字段组.push_back({字段组[前状态引用],*prior});
    q.追加字段组.push_back({A,字段组[动态引用],基础组内节点引用{*dynamic}});
    if(s.记录写回&&s.当前调用) {
        q.新节点组[*dynamic].字段组.push_back({字段组[来源引用],*s.当前调用});
        q.新节点组[*dynamic].字段组.push_back({字段组[方法直接写入值],基础值{std::int64_t{1}}});
        q.追加字段组.push_back({*s.当前调用,字段组[动态引用],基础组内节点引用{*dynamic}});
    }
    std::vector<稳定编码> newHistory;
    const auto recordHistory=[&](稳定编码 E) {
        if(std::find(f->历史不同值.begin(),f->历史不同值.end(),新特征准确值{E})==f->历史不同值.end()
            &&std::find(newHistory.begin(),newHistory.end(),E)==newHistory.end()) {
            q.追加字段组.push_back({b.交互者特征,字段组[历史引用],E});newHistory.push_back(E);
        }
    };
    if(f->当前值)if(const auto* old=std::get_if<稳定编码>(&*f->当前值))recordHistory(*old);
    recordHistory(b.对端);
    s.追加进展(q);补齐隔离追加联合预期(q);s.预留追加(q);
    const auto published=全局基础数据集.原子追加节点组(q);
    if(published.状态!=基础节点组追加状态::已完成)throw std::runtime_error("simulation interactor write failed");
    s.收集追加(published);s.模拟步骤=time;
    const auto after=读取模拟特征(b.交互者特征);
    if(!after||after->当前值!=std::optional<新特征准确值>{新特征准确值{b.对端}})throw std::runtime_error("simulation interactor write readback mismatch");
    if(slot)*slot=实现::写入来源::方法;
    result.状态=新交互求值状态::已取得;result.新状态节点=published.新节点组[state];
    if(dynamic)result.变化动态节点=published.新节点组[*dynamic];return result;
}

稳定编码 新_模拟世界类::建立阶段动作(const 方法参数场景& scene,const 新隔离阶段动作请求& request) {
    if((request.阶段==新隔离动作阶段::主动作&&request.目标存在位置!=2)
        ||(request.阶段==新隔离动作阶段::输出入列&&request.目标存在位置!=5))
        throw std::invalid_argument("isolation stage target position differs");
    return 建立本地参数动作(scene,request,2);
}
稳定编码 新_模拟世界类::建立参数动作(const 方法参数场景& scene,const 新模拟参数动作请求& request) {
    if(!实现_||scene.用途()!=新方法参数场景用途::模拟||scene.获取模拟运行接口()!=this)
        throw std::invalid_argument("simulation parameter action scene owner differs");
    const auto 主体=scene.查询参数(request.主体位置),目标=scene.查询参数(request.目标存在位置),
        参数=scene.查询参数(request.目标特征位置);
    if(!主体||!目标||!参数||主体->信息类型!=新方法参数信息类型::存在
        ||目标->信息类型!=新方法参数信息类型::存在||参数->信息类型!=新方法参数信息类型::特征)
        throw std::invalid_argument("simulation parameter action positions invalid");
    const auto 特征=scene.读取绑定特征(参数->信息节点);
    if(!特征)throw std::invalid_argument("simulation parameter action feature absent");
    const auto 定位=scene.读取存在参数特征(request.目标存在位置,特征->特征概念节点);
    if(!定位.特征||定位.特征->节点!=特征->节点)
        throw std::invalid_argument("simulation parameter action feature ownership differs");
    return 建立本地参数动作(scene,{request.目标存在位置,特征->特征概念节点,新隔离动作阶段::主动作},request.主体位置);
}
稳定编码 新_模拟世界类::建立本地参数动作(const 方法参数场景& scene,
    const 新隔离阶段动作请求& request,std::uint32_t 主体位置) {
    if(!实现_||scene.用途()!=新方法参数场景用途::模拟)throw std::invalid_argument("isolation action scene unavailable");
    auto& s=*实现_;
    const auto N=读取当前调用身份();
    if(!s.动作定义读取||!s.动作定义发布||!s.动作概念)throw std::runtime_error("isolation action definition provider absent");
    if(request.阶段!=新隔离动作阶段::主动作&&request.阶段!=新隔离动作阶段::输出入列)
        throw std::invalid_argument("isolation action stage invalid");
    const auto binding=scene.读取存在参数特征(request.目标存在位置,request.目标特征类型);
    if((binding.状态!=新方法参数特征读取状态::已读取&&binding.状态!=新方法参数特征读取状态::当前值未取得)||!binding.特征)
        throw std::invalid_argument("isolation action target not formally declared");
    const auto* target=scene.查询参数(request.目标存在位置);const auto* self=scene.查询参数(主体位置);
    if(!target||!self||target->信息类型!=新方法参数信息类型::存在||self->信息类型!=新方法参数信息类型::存在)
        throw std::invalid_argument("isolation action target binding absent");
    稳定编码 selfConcept{},targetConcept{};
    {
        std::lock_guard lock(s.互斥);
        if(scene.方法节点()!=s.通用方法||scene.全部角色绑定()!=s.通用绑定
            ||!s.是普通存在参数(target->信息节点)||!s.是普通存在参数(self->信息节点))
            throw std::invalid_argument("isolation action binding changed");
        selfConcept=s.通用参数.at(self->信息节点).规格.范围概念节点;
        targetConcept=s.通用参数.at(target->信息节点).规格.范围概念节点;
    }
    const 动作方法投影请求 definitionRequest{N.方法节点,selfConcept,targetConcept,request.目标特征类型,false};
    const auto projection=s.动作定义读取->读取动作方法投影(definitionRequest);
    const auto C=s.动作定义发布->确保动作定义概念(definitionRequest);
    if(C.状态!=动态概念操作状态::已建立&&C.状态!=动态概念操作状态::已复用)
        throw std::runtime_error("isolation action concept unavailable");
    if(!C.概念节点)throw std::runtime_error("isolation action concept identity absent");
    const auto protection=s.动作概念->读取动作概念定义保护(*C.概念节点);
    if(protection.状态!=动作概念定义保护状态::已读取||!protection.信息||protection.信息->动作定义!=std::optional{projection})
        throw std::runtime_error("isolation action concept protection unavailable");
    std::lock_guard lock(s.互斥);
    if(s.当前调用!=std::optional{N.调用节点}||!s.记录写回||scene.全部角色绑定()!=s.通用绑定)
        throw std::runtime_error("isolation action call changed during preparation");
    const auto F=binding.特征->节点;
    if(读引用(F,所属)!=std::optional{target->信息节点})throw std::runtime_error("isolation action target ownership changed");
    if(request.阶段==新隔离动作阶段::主动作) {
        if(const auto A=读引用(N.调用节点,调用主动作引用)) {
            const auto old=读取本地动作(*A);
            if(!old||old->主题存在节点!=self->信息节点||old->动作目标存在节点!=std::optional{target->信息节点}||old->动作目标特征节点!=std::optional{F}
                ||old->动态概念节点!=C.概念节点)throw std::runtime_error("isolation main action idempotency conflict");
            return *A;
        }
    } else for(const auto& field:全局基础数据集.查询字段(N.调用节点,字段组[调用阶段成员])) {
        const auto* J=std::get_if<稳定编码>(&field.内容);
        if(!J||读引用(*J,阶段所属调用)!=std::optional{N.调用节点})throw std::runtime_error("isolation phase owner corrupt");
        if(读值(*J,阶段种类值)!=static_cast<std::int64_t>(request.阶段))continue;
        const auto A=读引用(*J,动作引用);const auto old=A?读取本地动作(*A):std::nullopt;
        if(!A||!old||读值(*J,阶段目标位置值)!=request.目标存在位置||读引用(*J,阶段目标特征引用)!=std::optional{F}
            ||old->动态概念节点!=C.概念节点)throw std::runtime_error("isolation phase action idempotency conflict");
        return *A;
    }
    if(s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())throw std::overflow_error("isolation action time exhausted");
    const auto time=s.模拟步骤+1;
    基础节点组追加请求 q;q.预期联合节点组=protection.完整保护组;
    const auto node=[&](节点种类 kind) {
        const auto i=q.新节点组.size();q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(kind)}},{字段组[根],模拟世界树根节点}}});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{i},基础外部关系类型::父子});return i;
    };
    const auto A=node(动作种类);
    const auto put=[&](std::size_t n,字段 key,基础组内字段内容 v){q.新节点组[n].字段组.push_back({字段组[key],std::move(v)});};
    if(binding.特征->当前值) {
        bool captured=false;
        for(const auto n:s.通用节点)if(属于样例(n,状态种类,s.通用场景)&&读引用(n,状态特征引用)==std::optional{F}) {
            const auto state=读取本地状态(n);
            if(state&&state->固定准确值==binding.特征->当前值)captured=true;
        }
        if(!captured) {
            const auto S=node(状态种类);put(S,所属,target->信息节点);put(S,状态特征引用,F);
            put(S,特征类型,binding.特征->特征概念节点);put(S,步骤,基础值{s.模拟步骤});
            put(S,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});
            if(const auto* V=std::get_if<稳定编码>(&*binding.特征->当前值))put(S,当前材料,*V);
            else if(const auto* value=std::get_if<std::int64_t>(&*binding.特征->当前值))put(S,数值,基础值{*value});
            else throw std::runtime_error("isolation action prior value unsupported");
        }
    }
    put(A,方法引用,N.方法节点);put(A,特征类型,*C.概念节点);put(A,所属,target->信息节点);
    put(A,对端引用,self->信息节点);put(A,状态特征引用,F);put(A,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});put(A,步骤,基础值{time});
    if(request.阶段==新隔离动作阶段::主动作) {
        put(A,来源引用,N.调用节点);q.追加字段组.push_back({N.调用节点,字段组[调用主动作引用],基础组内节点引用{A}});
    } else {
        const auto J=node(阶段来源种类);
        put(J,阶段所属调用,N.调用节点);put(J,阶段种类值,基础值{static_cast<std::int64_t>(request.阶段)});
        put(J,阶段目标位置值,基础值{static_cast<std::int64_t>(request.目标存在位置)});put(J,阶段目标特征引用,F);
        put(J,动作引用,基础组内节点引用{A});put(A,来源引用,基础组内节点引用{J});
        q.追加字段组.push_back({N.调用节点,字段组[调用阶段成员],基础组内节点引用{J}});
    }
    s.追加进展(q);补齐隔离追加联合预期(q);s.预留追加(q);
    const auto made=全局基础数据集.原子追加节点组(q);
    if(made.状态!=基础节点组追加状态::已完成||made.新节点组.size()!=q.新节点组.size())throw std::runtime_error("isolation action publication failed");
    s.收集追加(made);s.模拟步骤=time;s.本次已进入动作=true;
    if(!读取本地动作(made.新节点组[A]))throw std::runtime_error("isolation action readback failed");
    return made.新节点组[A];
}
新方法隔离执行接口* 新_模拟世界类::获取隔离执行接口() noexcept {
    if(!实现_)return nullptr;
    std::lock_guard lock(实现_->互斥);
    return 实现_->通用&&实现_->动作定义读取&&实现_->动作定义发布&&实现_->动作概念&&实现_->场景构成 ? this : nullptr;
}
新隔离调用身份 新_模拟世界类::读取当前调用身份() const {
    if(!实现_)throw std::runtime_error("isolation owner absent");
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||!s.当前调用||!s.记录写回||读值(*s.当前调用,调用窗口状态值)!=0
        ||!属于样例(*s.当前调用,调用种类,s.通用场景))throw std::runtime_error("isolation call window absent");
    return {*s.当前调用,s.通用方法,s.通用场景,读引用(*s.当前调用,调用主动作引用)};
}
std::uint64_t 新_模拟世界类::读取进展序号() const {
    const auto id=读取当前调用身份();
    const auto value=读值(id.调用节点,调用进展值);
    if(!value||*value<0)throw std::runtime_error("isolation progress unavailable");
    return static_cast<std::uint64_t>(*value);
}
bool 新_模拟世界类::请求停止已成立() {
    if(!实现_)throw std::runtime_error("isolation owner absent");
    auto& s=*实现_;std::unique_lock lock(s.互斥);
    const auto id=读取当前调用身份();
    const auto cb=s.停止检查;const bool prior=s.请求取消;
    lock.unlock();const bool stop=prior||(cb&&cb());lock.lock();
    if(s.当前调用!=std::optional{id.调用节点}||!s.记录写回)throw std::runtime_error("isolation stop call changed");
    if(stop&&!s.请求取消) {
        const auto f=唯一字段(id.调用节点,调用取消值);
        if(!f)throw std::runtime_error("isolation cancellation field absent");
        基础节点组追加请求 q;q.替换字段组.push_back({f->编码,基础值{std::int64_t{1}}});
        补齐隔离追加联合预期(q);
        if(全局基础数据集.原子追加节点组(q).状态!=基础节点组追加状态::已完成)throw std::runtime_error("isolation cancellation publication failed");
        s.请求取消=true;
    }
    return s.请求取消;
}
std::optional<新状态信息> 新_模拟世界类::读取本地状态(稳定编码 n) const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||std::find(s.通用节点.begin(),s.通用节点.end(),n)==s.通用节点.end()
        ||读值(n,类型)!=状态种类||读引用(n,根)!=模拟世界树根节点)return {};
    const auto E=读引用(n,所属),F=读引用(n,状态特征引用),FT=读引用(n,特征类型);
    const auto time=读值(n,步骤),sem=读值(n,时间语义);
    if(!E||!F||!FT||!time||*time<0||!sem||*sem!=static_cast<std::int64_t>(新状态时间语义::抽象相对))
        throw std::runtime_error("isolation state shape corrupt");
    新特征准确值 value;
    if(const auto v=读引用(n,当前材料))value=*v;
    else if(const auto v=读值(n,数值))value=*v;
    else throw std::runtime_error("isolation state has no accurate value");
    return 新状态信息{n,*E,*F,*FT,value,{新状态时间语义::抽象相对,*time},0};
}
std::optional<新状态信息> 新_模拟世界类::读取最新特征状态(
    稳定编码 scene,稳定编码 owner,稳定编码 F) const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(读引用(owner,所属)!=std::optional{scene}||读引用(F,所属)!=std::optional{owner})
        throw std::runtime_error("isolated latest state target differs");
    std::optional<新状态信息> found;
    for(const auto n:s.通用节点) {
        // 隔离事实的物理归属是样例根；owner可处于其内的语言子场景。
        if(!属于样例(n,状态种类,s.通用场景)||读引用(n,状态特征引用)!=std::optional{F})continue;
        const auto state=读取本地状态(n);
        if(!state||state->被描述存在节点!=owner)throw std::runtime_error("isolated latest state owner corrupt");
        if(found&&state->强时间.纳秒==found->强时间.纳秒&&state->节点!=found->节点)
            throw std::runtime_error("isolated latest quantity state ambiguous");
        if(!found||state->强时间.纳秒>found->强时间.纳秒)found=*state;
    }
    return found;
}
新二次特征计算结果 新_模拟世界类::比较接收数量状态(
    const 新状态信息& 最新,const 新状态信息& 上轮) const {
    if(!实现_||!实现_->二次计算)throw std::runtime_error("simulation comparison provider unavailable");
    const auto definition=读取连接构成();
    if(definition.状态!=新交互求值状态::已取得||!definition.构成)
        throw std::runtime_error("simulation quantity definition unavailable");
    const auto FT=definition.构成->接收数量特征概念;
    // R98：接收数量必须有值；未知不进入二次比较DTO。
    if (!最新.固定准确值 || !上轮.固定准确值) throw std::runtime_error("simulation quantity state value missing");
    新二次特征计算请求 request;
    request.左输入=二次特征准确特征输入{最新.节点,FT,*最新.固定准确值};
    request.右输入=二次特征准确特征输入{上轮.节点,FT,*上轮.固定准确值};
    return 实现_->二次计算->计算(request);
}
std::optional<新状态信息> 新_模拟世界类::读取固定消息状态(稳定编码 M,稳定编码 F) const {
    const auto msg=读取模拟消息(M);
    if(!msg)throw std::invalid_argument("isolation fixed message outside sample");
    if(!实现_)throw std::runtime_error("isolation owner absent");
    const auto& s=*实现_;std::unique_lock lock(s.互斥);
    if(!s.通用参数.contains(F)||读引用(F,所属)!=std::optional{msg->窗口节点})
        throw std::invalid_argument("isolation fixed message feature differs");
    const auto range=s.通用参数.at(F).规格.范围概念节点;
    lock.unlock();const auto def=s.方法.读取模拟特征范围(range);lock.lock();
    if(!def||def->比较规则!=特征概念比较规则::消息集合身份判等)
        throw std::invalid_argument("isolation fixed state is not a message feature");
    std::optional<新状态信息> found;
    for(const auto n:s.通用节点) {
        if(!属于样例(n,状态种类,s.通用场景)||读引用(n,状态特征引用)!=std::optional{F}
            ||读值(n,步骤)!=msg->正式记录时间.纳秒)continue;
        const auto state=读取本地状态(n);
        if(!state||state->被描述存在节点!=msg->窗口节点||state->强时间!=msg->正式记录时间
            ||state->特征类型概念节点!=def->概念节点)continue;
        const auto* V=(state->固定准确值 ? std::get_if<稳定编码>(&*state->固定准确值) : nullptr);
        const auto material=V?读取模拟特征值材料(*V):std::nullopt;
        const auto* ids=material?std::get_if<std::vector<std::uint64_t>>(&material->材料):nullptr;
        if(!ids||std::count(ids->begin(),ids->end(),M.值)!=1)throw std::runtime_error("isolation message state payload differs");
        if(found)throw std::runtime_error("isolation message generation state ambiguous");
        found=state;
    }
    if(!found)throw std::runtime_error("isolation message generation state absent");
    return found;
}
std::optional<新动态信息> 新_模拟世界类::读取本地动作(稳定编码 A) const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||!属于样例(A,动作种类,s.通用场景))return {};
    const auto M=读引用(A,方法引用),source=读引用(A,来源引用),E=读引用(A,所属),F=读引用(A,状态特征引用),C=读引用(A,特征类型);
    const auto time=读值(A,步骤),sem=读值(A,时间语义);
    if(!M||!source||!E||!F||!C||!time||!sem||*sem!=static_cast<std::int64_t>(新状态时间语义::抽象相对))
        throw std::runtime_error("isolation formal action shape corrupt");
    新动态信息 result;result.节点=A;result.种类=新动态种类::动作;
    result.主题存在节点=读引用(A,对端引用).value_or(*E);
    result.动作定义节点=*M;result.动作来源节点=*source;result.动作目标存在节点=*E;
    result.动作目标特征节点=*F;result.动态概念节点=*C;
    result.动作时间=新状态强时间{新状态时间语义::抽象相对,*time};
    for(const auto& f:全局基础数据集.查询字段(A,字段组[动态引用])) {
        const auto* D=std::get_if<稳定编码>(&f.内容);
        if(!D||读引用(*D,动作引用)!=std::optional{A})throw std::runtime_error("isolation action result reference corrupt");
        result.动作结果组.push_back({新动作结果关系类型::导致变化,*D});
    }
    return result;
}
std::optional<新动态信息> 新_模拟世界类::读取本地变化(稳定编码 D) const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||std::find(s.通用节点.begin(),s.通用节点.end(),D)==s.通用节点.end()
        ||读值(D,类型)!=动态种类||读引用(D,根)!=模拟世界树根节点)return {};
    const auto after=读引用(D,后状态引用),before=读引用(D,前状态引用),E=读引用(D,所属);
    if(!after||!E||!读取本地状态(*after)||(before&&!读取本地状态(*before)))throw std::runtime_error("isolation change endpoints corrupt");
    新动态信息 r;r.节点=D;r.主题存在节点=*E;
    if(before)r.状态节点组.push_back(*before);r.状态节点组.push_back(*after);return r;
}
std::optional<新隔离调用证据> 新_模拟世界类::读取已关闭调用证据() const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.当前调用||!s.写回窗口已关闭||s.记录写回)return {};
    const auto N=*s.当前调用;
    if(读值(N,调用窗口状态值)!=1)throw std::runtime_error("isolation call not formally closed");
    const auto cancelled=读值(N,调用取消值);
    if(!cancelled||(*cancelled!=0&&*cancelled!=1))throw std::runtime_error("isolation cancellation corrupt");
    新隔离调用证据 r;r.身份={N,s.通用方法,s.通用场景,读引用(N,调用主动作引用)};r.已请求取消=*cancelled!=0;
    if(r.身份.主动作){if(!读取本地动作(*r.身份.主动作))throw std::runtime_error("isolation main action evidence corrupt");r.阶段动作组.push_back(*r.身份.主动作);}
    for(const auto& f:全局基础数据集.查询字段(N,字段组[调用阶段成员])) {
        const auto* J=std::get_if<稳定编码>(&f.内容);const auto A=J?读引用(*J,动作引用):std::nullopt;
        const auto 动作=A ? 读取本地动作(*A) : std::nullopt;
        if(!J||!A||读引用(*J,阶段所属调用)!=std::optional{N}||!动作
            ||动作->动作来源节点!=std::optional{*J})throw std::runtime_error("isolation phase evidence corrupt");
        r.阶段动作组.push_back(*A);
    }
    for(const auto& f:全局基础数据集.查询字段(N,字段组[动态引用])) {
        const auto* D=std::get_if<稳定编码>(&f.内容);
        const auto F=D?读引用(*D,状态特征引用):std::nullopt,after=D?读引用(*D,后状态引用):std::nullopt;
        const auto method=D?读值(*D,方法直接写入值):std::nullopt;
        if(!D||!F||!after||!method||(*method!=0&&*method!=1)||读引用(*D,来源引用)!=std::optional{N}||!读取本地变化(*D))
            throw std::runtime_error("isolation operation evidence corrupt");
        r.变化组.push_back({N,*F,*after,*D,读引用(*D,前状态引用),读引用(*D,动作引用),*method!=0});
    }
    for(const auto 记录:读引用组(N,需求发布成员)) {
        if(!属于样例(记录,需求发布种类,s.通用场景)||读引用(记录,来源引用)!=std::optional{N})
            throw std::logic_error("closed demand publication source invalid");
        const auto A=读引用(记录,动作引用),S=读引用(记录,后状态引用),H=读引用(记录,主体引用),F=读引用(记录,状态特征引用),D=读引用(记录,发布需求引用);
        const auto 状态=S ? 读取本地状态(*S) : std::nullopt;
        if(!A||!S||!H||!F||!D||r.身份.主动作!=A||!读取本地动作(*A)||!状态
            ||状态->被描述存在节点!=*H||状态->来源实例特征节点!=*F||状态->固定准确值!=新特征准确值{*D})
            throw std::logic_error("closed demand first-value/post-state evidence invalid");
    }
    for(const auto 记录:读引用组(N,询问入列成员)) {
        if(!属于样例(记录,队列入列种类,s.通用场景)||读引用(记录,来源引用)!=std::optional{N})
            throw std::logic_error("closed queue receipt source invalid");
        const auto A=读引用(记录,动作引用),M=读引用(记录,输出消息引用),S=读引用(记录,入列状态引用),
            D=读引用(记录,入列动态引用),W=读引用(记录,所属),F=读引用(记录,队列特征引用);
        const auto 动作=A ? 读取本地动作(*A) : std::nullopt;
        const auto 消息=M ? 读取模拟消息(*M) : std::nullopt;
        const auto 状态=S ? 读取本地状态(*S) : std::nullopt;
        if(!A||!M||!S||!D||!W||!F||!动作||!消息||!状态||r.身份.主动作!=A
            ||动作->动作来源节点!=std::optional{N}||动作->动作目标存在节点!=W||动作->动作目标特征节点!=F
            ||消息->来源.方向!=新外设消息方向::输出||消息->窗口节点!=*W
            ||状态->被描述存在节点!=*W||状态->来源实例特征节点!=*F
            ||读引用(*D,来源引用)!=std::optional{N}||读引用(*D,动作引用)!=A||读引用(*D,后状态引用)!=S
            ||!读取本地变化(*D))throw std::logic_error("closed queue receipt evidence invalid");
    }
    return r;
}
新场景事实构成读取结果 新_模拟世界类::读取连接构成() const {
    if(!实现_||!实现_->场景构成)return {};
    const auto C=实现_->方法.确保已确认消息连接概念();
    // 正式构成读取只在并发写入使只读保护失配时报告前提变化；有限次重读，其它状态原样返回。
    auto r=实现_->场景构成->读取场景事实构成(C);
    for(int 重读=0;重读<16&&r.状态==新交互求值状态::前提变化;++重读)r=实现_->场景构成->读取场景事实构成(C);
    return r;
}

新交互写入读回 新_模拟世界类::发布本地队列(稳定编码 F,const std::vector<std::uint64_t>& members,
    std::optional<稳定编码> A,bool method,
    const std::function<void(基础节点组追加请求&,std::size_t,std::size_t)>& extra) {
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto W=读引用(F,所属),FT=读引用(F,特征类型),old=读引用(F,当前材料);
    const auto current=唯一字段(F,当前材料);
    if(!W||!FT||!old||!current||!属于样例(F,特征种类,s.通用场景))throw std::runtime_error("local queue target corrupt");
    const auto v=读取模拟特征值材料(*old);
    const auto* values=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
    if(!values||*values==members)throw std::runtime_error("local queue transition has no actual change");
    for(const auto n:members)if(!读取模拟消息(稳定编码{n}))throw std::runtime_error("local queue contains foreign message");
    if(s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())throw std::overflow_error("local queue time exhausted");
    const auto time=s.模拟步骤+1;
    基础节点组追加请求 q;
    const auto add=[&](节点种类 kind) {
        const auto n=q.新节点组.size();q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(kind)}},{字段组[根],模拟世界树根节点}}});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{n},基础外部关系类型::父子});return n;
    };
    const auto field=[&](std::size_t n,字段 k,基础组内字段内容 value){q.新节点组[n].字段组.push_back({字段组[k],std::move(value)});};
    std::optional<稳定编码> prior;std::int64_t latest=-1;
    for(const auto n:s.通用节点)if(属于样例(n,状态种类,s.通用场景)&&读引用(n,状态特征引用)==F&&读引用(n,当前材料)==old) {
        const auto t=读值(n,步骤);
        if(!t||*t<0||*t>s.模拟步骤)throw std::runtime_error("queue prior state time corrupt");
        if(*t==latest&&prior&&*prior!=n)throw std::runtime_error("queue prior state ambiguous");
        if(*t>latest){prior=n;latest=*t;}
    }
    const auto V=add(材料种类),S=add(状态种类),D=add(动态种类);
    field(V,数值,基础值{members});field(V,所属,F);
    const auto state=[&](std::size_t n,基础组内字段内容 value,std::int64_t t) {
        field(n,所属,*W);field(n,状态特征引用,F);field(n,特征类型,*FT);field(n,当前材料,std::move(value));
        field(n,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});field(n,步骤,基础值{t});
    };
    state(S,基础组内节点引用{V},time);
    if(prior)field(D,前状态引用,*prior);else {const auto B=add(状态种类);state(B,*old,s.模拟步骤);field(D,前状态引用,基础组内节点引用{B});}
    field(D,所属,*W);field(D,状态特征引用,F);field(D,后状态引用,基础组内节点引用{S});
    field(D,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});field(D,步骤,基础值{time});
    if(A) {
        const auto a=读取本地动作(*A);
        if(!a||a->动作目标存在节点!=W||a->动作目标特征节点!=std::optional{F})throw std::runtime_error("queue action target differs");
        field(D,动作引用,*A);q.追加字段组.push_back({*A,字段组[动态引用],基础组内节点引用{D}});
    }
    if(!s.当前调用||!s.记录写回)throw std::runtime_error("queue transition outside open call");
    field(D,来源引用,*s.当前调用);field(D,方法直接写入值,基础值{std::int64_t{method?1:0}});
    q.追加字段组.push_back({*s.当前调用,字段组[动态引用],基础组内节点引用{D}});
    q.删除字段关系组.push_back(current->编码);q.追加字段组.push_back({F,字段组[当前材料],基础组内节点引用{V}});
    bool history=false;for(const auto& f:全局基础数据集.查询字段(F,字段组[历史引用]))if(f.内容==基础字段内容{*old})history=true;
    if(!history)q.追加字段组.push_back({F,字段组[历史引用],*old});
    if(extra)extra(q,S,D);
    s.追加进展(q);补齐隔离追加联合预期(q);s.预留追加(q);
    const auto p=全局基础数据集.原子追加节点组(q);
    if(p.状态!=基础节点组追加状态::已完成)throw std::runtime_error("local queue atomic publication failed");
    s.收集追加(p);s.通用字段.erase(std::remove(s.通用字段.begin(),s.通用字段.end(),current->编码),s.通用字段.end());
    s.通用材料.emplace(p.新节点组[V],材料物理类型::U64数组);s.模拟步骤=time;
    if(auto* slot=s.准备写回槽(F))*slot=method?实现::写入来源::方法:实现::写入来源::环境;
    const auto after=读取模拟特征值材料(p.新节点组[V]);
    if(读引用(F,当前材料)!=std::optional{p.新节点组[V]}||!after||after->材料!=新特征值材料{members}||!读取本地变化(p.新节点组[D]))
        throw std::runtime_error("local queue publication readback failed");
    return {新交互求值状态::已取得,p.新节点组[S],p.新节点组[D]};
}

新隔离提交结果 新_模拟世界类::提交本地输出队列(const 方法参数场景& scene,
    const 新模拟队列提交请求& request) {
    if(!实现_||scene.用途()!=新方法参数场景用途::模拟||scene.获取模拟运行接口()!=this)
        throw std::invalid_argument("local queue scene owner differs");
    const auto window=scene.查询参数(request.窗口位置),peer=scene.查询参数(request.对端位置),
        output=scene.查询参数(request.输出特征位置);
    if(!window||!peer||!output||window->信息类型!=新方法参数信息类型::存在
        ||peer->信息类型!=新方法参数信息类型::存在||output->信息类型!=新方法参数信息类型::特征
        ||!有效(request.动作)||request.UTF8正文.empty()||!新_语言类::严格UTF8字节串有效(request.UTF8正文))
        throw std::invalid_argument("local queue parameters or UTF8 body invalid");
    const auto queue=scene.读取绑定特征(output->信息节点);
    if(!queue)throw std::invalid_argument("local queue feature unavailable");
    const auto declared=scene.读取存在参数特征(request.窗口位置,queue->特征概念节点);
    if(declared.状态!=新方法参数特征读取状态::已读取||!declared.特征
        ||declared.特征->节点!=output->信息节点||!declared.当前完整材料
        ||!std::holds_alternative<std::vector<std::uint64_t>>(declared.当前完整材料->材料))
        throw std::invalid_argument("local queue target is not the declared message queue");
    const auto identity=读取当前调用身份();
    if(scene.方法节点()!=identity.方法节点)throw std::invalid_argument("local queue method differs");
    const auto prepared=实现_->方法.读取方法执行结构闭包({identity.方法节点});
    if(prepared.状态!=新_方法类::执行结构读取状态::已读取||prepared.方法组.size()!=1)
        throw std::runtime_error("local queue method definition unavailable");
    const auto domain=实现_->方法.读取模拟特征范围(queue->特征概念节点);
    if(!domain||domain->比较规则!=特征概念比较规则::消息集合身份判等)
        throw std::invalid_argument("local queue feature is not a message collection");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto W=window->信息节点,H=peer->信息节点,F=output->信息节点;
    if(!s.通用||scene.全部角色绑定()!=s.通用绑定||s.当前调用!=std::optional{identity.调用节点}
        ||!s.记录写回||读值(identity.调用节点,调用窗口状态值)!=0||读值(identity.调用节点,调用取消值)!=0
        ||读引用(F,所属)!=std::optional{W}||读引用(W,所属)!=std::optional{s.通用场景}
        ||!s.是普通存在参数(W)||!s.是普通存在参数(H))
        throw std::invalid_argument("local queue binding or active call changed");
    const auto action=读取本地动作(request.动作);
    if(!action||action->动作来源节点!=std::optional{identity.调用节点}
        ||identity.主动作!=std::optional{request.动作}||action->动作目标存在节点!=std::optional{W}
        ||action->动作目标特征节点!=std::optional{F})
        throw std::invalid_argument("local queue action identity differs");
    const auto readReceipt=[&]() -> std::optional<新隔离提交结果> {
        std::optional<新隔离提交结果> receipt;
        for(const auto& field:全局基础数据集.查询字段(identity.调用节点,字段组[询问入列成员])) {
            const auto* T=std::get_if<稳定编码>(&field.内容);
            if(!T||!属于样例(*T,队列入列种类,s.通用场景)
                ||读引用(*T,来源引用)!=std::optional{identity.调用节点})
                throw std::runtime_error("local queue receipt source corrupt");
            if(读引用(*T,动作引用)!=std::optional{request.动作})continue;
            const auto M=读引用(*T,输出消息引用),S=读引用(*T,入列状态引用),D=读引用(*T,入列动态引用);
            const auto message=M ? 读取模拟消息(*M) : std::nullopt;
            const auto state=S ? 读取本地状态(*S) : std::nullopt;
            const auto change=D ? 读取本地变化(*D) : std::nullopt;
            const auto serialField=唯一字段(*T,提交序号值);
            const auto* value=serialField ? std::get_if<基础值>(&serialField->内容) : nullptr;
            const auto* serial=value ? std::get_if<std::vector<std::uint64_t>>(&value->材料) : nullptr;
            if(!M||!S||!D||!message||!state||!change||!serial||serial->size()!=1||!serial->front()
                ||message->UTF8正文!=request.UTF8正文||message->窗口节点!=W
                ||message->来源.方向!=新外设消息方向::输出||message->来源.来源序号!=serial->front()
                ||读引用(*T,所属)!=std::optional{W}||读引用(*T,对端引用)!=std::optional{H}
                ||读引用(*T,队列特征引用)!=std::optional{F}||读引用(*M,对端引用)!=std::optional{H}
                ||state->来源实例特征节点!=F||state->被描述存在节点!=W
                ||读引用(*D,来源引用)!=std::optional{identity.调用节点}||读引用(*D,动作引用)!=std::optional{request.动作}
                ||读引用(*D,后状态引用)!=S||读值(*D,方法直接写入值)!=1)
                throw std::runtime_error("local queue receipt idempotency conflict");
            const auto* V=(state->固定准确值 ? std::get_if<稳定编码>(&*state->固定准确值) : nullptr);
            const auto material=V ? 读取模拟特征值材料(*V) : std::nullopt;
            const auto* members=material ? std::get_if<std::vector<std::uint64_t>>(&material->材料) : nullptr;
            const auto source=读取本地动作(request.动作);
            if(!members||std::count(members->begin(),members->end(),M->值)!=1||receipt||!source
                ||std::none_of(source->动作结果组.begin(),source->动作结果组.end(),[&](const auto& r){
                    return r.类型==新动作结果关系类型::导致变化&&r.结果动态节点==*D;
                }))throw std::runtime_error("local queue receipt or action relation corrupt");
            receipt=新隔离提交结果{serial->front(),*M,*S,*D};
        }
        return receipt;
    };
    if(const auto receipt=readReceipt())return *receipt;
    s.消息定义保护=prepared;
    const auto result=发布本地消息(F,W,H,新外设消息方向::输出,request.UTF8正文,s.模拟步骤,{},request.动作,true,
        [&](基础节点组追加请求& q,稳定编码 M,std::size_t S,std::size_t D,std::uint64_t serial) {
            const auto T=q.新节点组.size();
            q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(队列入列种类)}},
                {字段组[根],模拟世界树根节点},{字段组[来源引用],identity.调用节点},{字段组[动作引用],request.动作},
                {字段组[所属],W},{字段组[对端引用],H},{字段组[队列特征引用],F},{字段组[输出消息引用],M},
                {字段组[提交序号值],基础值{std::vector<std::uint64_t>{serial}}},
                {字段组[入列状态引用],基础组内节点引用{S}},{字段组[入列动态引用],基础组内节点引用{D}}}});
            q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{T},基础外部关系类型::父子});
            q.追加字段组.push_back({identity.调用节点,字段组[询问入列成员],基础组内节点引用{T}});
        });
    const auto receipt=readReceipt();
    if(!receipt||receipt->提交序号!=result.提交序号||receipt->输出消息!=result.输出消息
        ||receipt->入列状态!=result.入列状态||receipt->入列动态!=result.入列动态)
        throw std::runtime_error("local queue published receipt readback differs");
    return result;
}

新隔离提交结果 新_模拟世界类::提交输出(const 方法参数场景& scene,const 新隔离输出请求& request) {
    const auto identity=读取当前调用身份();
    if(request.窗口位置!=5||request.对端位置!=3||request.发送特征位置!=7||scene.方法节点()!=identity.方法节点)
        throw std::invalid_argument("local output ordered positions differ");
    const auto prepared=实现_->方法.读取方法执行结构闭包({identity.方法节点});
    if(prepared.状态!=新_方法类::执行结构读取状态::已读取||prepared.方法组.size()!=1)throw std::runtime_error("local output method prepare unavailable");
    const auto queue=scene.读取存在参数特征(5,request.队列特征类型);
    if(queue.状态!=新方法参数特征读取状态::已读取||!queue.特征||!queue.当前完整材料)throw std::runtime_error("local output queue unavailable");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(scene.全部角色绑定()!=s.通用绑定||s.通用绑定.size()!=7||s.当前调用!=std::optional{identity.调用节点}
        ||!s.记录写回||读值(identity.调用节点,调用窗口状态值)!=0||读值(identity.调用节点,调用取消值)!=0)
        throw std::invalid_argument("local output binding or active call changed");
    s.消息定义保护=prepared;
    const auto at=[&](std::uint32_t p){const auto& b=s.通用绑定.at(p-1);if(!b.有序引用)throw std::runtime_error("local output parameter missing");return b.有序引用->信息节点;};
    const auto W=at(5),peer=at(3),send=at(7),F=queue.特征->节点;
    const auto action=读取本地动作(request.阶段动作);
    const auto J=读引用(request.阶段动作,来源引用);
    if(!action||!J||!属于样例(*J,阶段来源种类,s.通用场景)||读引用(*J,阶段所属调用)!=std::optional{identity.调用节点}
        ||读值(*J,阶段种类值)!=static_cast<std::int64_t>(新隔离动作阶段::输出入列)
        ||action->动作目标存在节点!=std::optional{W}||action->动作目标特征节点!=std::optional{F}
        ||读引用(send,所属)!=std::optional{W})throw std::invalid_argument("local output stage identity differs");
    const auto number=[&](稳定编码 T) {
        const auto f=唯一字段(T,提交序号值);const auto* v=f?std::get_if<基础值>(&f->内容):nullptr;
        const auto* a=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
        if(!a||a->size()!=1||!a->front())throw std::runtime_error("local transmission sequence corrupt");return a->front();
    };
    std::optional<新隔离提交结果> reused;
    for(const auto& f:全局基础数据集.查询字段(identity.调用节点,字段组[调用传输成员])) {
        const auto* T=std::get_if<稳定编码>(&f.内容);if(!T||!属于样例(*T,传输种类,s.通用场景))throw std::runtime_error("local transmission member corrupt");
        if(读引用(*T,动作引用)!=std::optional{request.阶段动作})continue;
        const auto M=读引用(*T,输出消息引用),S=读引用(*T,入列状态引用),D=读引用(*T,入列动态引用);
        const auto msg=M?读取模拟消息(*M):std::nullopt;
        const auto state=S?读取本地状态(*S):std::nullopt;
        const auto change=D?读取本地变化(*D):std::nullopt;
        if(!msg||!S||!D||msg->UTF8正文!=request.UTF8正文||读引用(*T,所属)!=std::optional{W}
            ||读引用(*T,对端引用)!=std::optional{peer}||读引用(*T,发送引用)!=std::optional{send}||读引用(*T,队列特征引用)!=std::optional{F}
            ||读引用(*T,来源引用)!=std::optional{identity.调用节点}||!state||!change||state->来源实例特征节点!=F
            ||state->被描述存在节点!=W||读引用(*D,来源引用)!=std::optional{identity.调用节点}
            ||读引用(*D,动作引用)!=std::optional{request.阶段动作}||读引用(*D,后状态引用)!=S
            ||读值(*D,方法直接写入值)!=1||msg->来源.方向!=新外设消息方向::输出)
            throw std::runtime_error("local output idempotency conflict");
        if (!state->固定准确值) throw std::runtime_error("local output state value missing");
        const auto material=读取模拟特征值材料(std::get<稳定编码>(*state->固定准确值));
        const auto* members=material?std::get_if<std::vector<std::uint64_t>>(&material->材料):nullptr;
        if(!members||std::count(members->begin(),members->end(),M->值)!=1||reused)throw std::runtime_error("local output receipt ambiguous");
        reused=新隔离提交结果{number(*T),*M,*S,*D};
    }
    if(reused)return *reused;
    const auto& definition=s.消息定义保护;
    if(definition.状态!=新_方法类::执行结构读取状态::已读取||definition.方法组.size()!=1)throw std::runtime_error("output body declaration unavailable");
    bool valid=false;
    for(const auto& plan:definition.方法组.front().初始模拟输入方案组)for(const auto& input:plan.消息组) {
        const auto domain=s.方法.读取模拟特征范围(input.正文范围概念);
        if(!domain||domain->材料类型!=特征概念材料物理类型::UTF8字符串)throw std::runtime_error("output body domain unavailable");
        if(input.正文非空&&request.UTF8正文.empty())continue;
        if(std::holds_alternative<特征概念全材料值域>(domain->值域))valid=true;
        else if(const auto* values=std::get_if<std::vector<稳定编码>>(&domain->值域))for(const auto value:*values) {
            const auto material=s.方法.读取模拟候选材料(value);
            if(!material)throw std::runtime_error("output body domain material unavailable");
            if(material->材料==新特征值材料{request.UTF8正文})valid=true;
        }
    }
    if(!valid||!新_语言类::严格UTF8字节串有效(request.UTF8正文))throw std::invalid_argument("output body outside declared domain");
    return 发布本地消息(F,W,peer,新外设消息方向::输出,request.UTF8正文,s.模拟步骤,{},{request.阶段动作},true,
        [&](基础节点组追加请求& q,稳定编码 M,std::size_t S,std::size_t D,std::uint64_t seq) {
            const auto T=q.新节点组.size();
            q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(传输种类)}},{字段组[根],模拟世界树根节点},
                {字段组[来源引用],identity.调用节点},{字段组[动作引用],request.阶段动作},{字段组[所属],W},{字段组[对端引用],peer},
                {字段组[发送引用],send},{字段组[队列特征引用],F},{字段组[输出消息引用],M},
                {字段组[提交序号值],基础值{std::vector<std::uint64_t>{seq}}},{字段组[传输状态值],基础值{std::int64_t{1}}},
                {字段组[入列状态引用],基础组内节点引用{S}},{字段组[入列动态引用],基础组内节点引用{D}}}});
            q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{T},基础外部关系类型::父子});
            q.追加字段组.push_back({identity.调用节点,字段组[调用传输成员],基础组内节点引用{T}});
            q.预期联合节点组.insert(q.预期联合节点组.end(),definition.完整保护组.begin(),definition.完整保护组.end());
        });
}

新隔离推进状态 新_模拟世界类::推进输出() {
    if(请求停止已成立())return 新隔离推进状态::已停止;
    const auto identity=读取当前调用身份();
    const auto proof=实现_->方法.读取方法执行结构闭包({identity.方法节点});
    if(proof.状态!=新_方法类::执行结构读取状态::已读取||proof.方法组.size()!=1)throw std::runtime_error("output advance definition unavailable");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto N=读取当前调用身份().调用节点;
    if(N!=identity.调用节点||s.通用方法!=identity.方法节点)throw std::runtime_error("output advance invocation changed");
    s.消息定义保护=proof;
    for(const auto& member:全局基础数据集.查询字段(N,字段组[调用传输成员])) {
        const auto* T=std::get_if<稳定编码>(&member.内容);
        if(!T||!属于样例(*T,传输种类,s.通用场景)||读引用(*T,来源引用)!=std::optional{N})throw std::runtime_error("transmission source corrupt");
        const auto stage=读值(*T,传输状态值);const auto status=唯一字段(*T,传输状态值);
        const auto F=读引用(*T,队列特征引用),M=读引用(*T,输出消息引用),W=读引用(*T,所属),peer=读引用(*T,对端引用),send=读引用(*T,发送引用);
        const auto msg=M?读取模拟消息(*M):std::nullopt;
        if(!stage||*stage<1||*stage>3||!status||!F||!M||!W||!peer||!send||!msg)throw std::runtime_error("transmission structure corrupt");
        if(*stage==3)continue;
        if(*stage==1) {
            const auto V=读引用(*F,当前材料);const auto values=V?读取模拟特征值材料(*V):std::nullopt;
            const auto* current=values?std::get_if<std::vector<std::uint64_t>>(&values->材料):nullptr;
            if(!current||current->empty()||current->front()!=M->值)throw std::runtime_error("transmission output is not actual queue head");
            auto remaining=*current;remaining.erase(remaining.begin());
            (void)发布本地队列(*F,remaining,{},false,[&](基础节点组追加请求& q,std::size_t S,std::size_t D) {
                q.替换字段组.push_back({status->编码,基础值{std::int64_t{2}}});
                q.追加字段组.push_back({*T,字段组[领取状态引用],基础组内节点引用{S}});
                q.追加字段组.push_back({*T,字段组[领取动态引用],基础组内节点引用{D}});
            });
        } else {
            (void)发布本地消息(*send,*W,*peer,新外设消息方向::发送,msg->UTF8正文,msg->正式记录时间.纳秒,msg->来源.来源序号,{},false,
                [&](基础节点组追加请求& q,稳定编码 sent,std::size_t S,std::size_t D,std::uint64_t) {
                    q.替换字段组.push_back({status->编码,基础值{std::int64_t{3}}});
                    q.追加字段组.push_back({*T,字段组[传输发送消息引用],sent});
                    q.追加字段组.push_back({*T,字段组[发送状态引用],基础组内节点引用{S}});
                    q.追加字段组.push_back({*T,字段组[发送动态引用],基础组内节点引用{D}});
                });
        }
        return 新隔离推进状态::已推进;
    }
    return 新隔离推进状态::已耗尽;
}

std::optional<新外设消息信息> 新_模拟世界类::按提交读取发送消息(std::uint64_t seq) const {
    if(!实现_||!seq)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);if(!s.当前调用)return {};
    std::optional<新外设消息信息> result;
    bool matched=false;
    for(const auto& field:全局基础数据集.查询字段(*s.当前调用,字段组[调用传输成员])) {
        const auto* T=std::get_if<稳定编码>(&field.内容);if(!T||!属于样例(*T,传输种类,s.通用场景))throw std::runtime_error("transmission member invalid");
        const auto serial=唯一字段(*T,提交序号值);const auto* raw=serial?std::get_if<基础值>(&serial->内容):nullptr;
        const auto* values=raw?std::get_if<std::vector<std::uint64_t>>(&raw->材料):nullptr;
        if(!values||values->size()!=1||!values->front())throw std::runtime_error("transmission sequence invalid");
        if(values->front()!=seq)continue;
        if(matched)throw std::runtime_error("duplicate call-local submission sequence");matched=true;
        const auto stage=读值(*T,传输状态值);if(!stage||*stage<1||*stage>3)throw std::runtime_error("transmission stage invalid");
        if(*stage!=3)continue;
        const auto M=读引用(*T,传输发送消息引用),S=读引用(*T,发送状态引用),D=读引用(*T,发送动态引用),F=读引用(*T,发送引用);
        const auto msg=M?读取模拟消息(*M):std::nullopt;
        const auto state=M&&F?读取固定消息状态(*M,*F):std::nullopt;
        if(!msg||!S||!D||!state||state->节点!=*S||!读取本地变化(*D)||msg->来源.来源序号!=seq||msg->来源.方向!=新外设消息方向::发送)
            throw std::runtime_error("completed transmission lacks real message and state");
        result=msg;
    }
    return result;
}
bool 新_模拟世界类::提交已完整写出(std::uint64_t seq) const {return 按提交读取发送消息(seq).has_value();}

新交互求值状态 新_模拟世界类::读取隔离连接过程(稳定编码 N,稳定编码 A,
    const 新交互绑定& b,const 新消息连接输入构成& definition,新交互连接过程& out) const {
    out={};
    if(!实现_)return 新交互求值状态::材料不足;
    auto& s=*实现_;
    const auto quantity=s.方法.读取模拟特征范围(definition.接收数量特征概念);
    if(!quantity)throw std::runtime_error("isolated quantity definition unavailable");
    std::lock_guard lock(s.互斥);
    if(!s.通用||s.当前调用!=std::optional{N}||读引用(N,调用主动作引用)!=std::optional{A})
        return 新交互求值状态::不符合;
    std::optional<稳定编码> process;
    for(const auto& field:全局基础数据集.查询字段(N,字段组[集合成员])) {
        const auto* P=std::get_if<稳定编码>(&field.内容);
        if(!P||!属于样例(*P,过程种类,s.通用场景)||读引用(*P,来源引用)!=std::optional{N})
            throw std::runtime_error("isolated quantity process member corrupt");
        if(process)throw std::runtime_error("isolated quantity process ambiguous");
        process=*P;
    }
    if(!process)return 新交互求值状态::尚未具备;
    const auto P=*process;
    if(读引用(P,动作引用)!=std::optional{A}||读引用(P,所属)!=std::optional{b.场景}
        ||读引用(P,对端引用)!=std::optional{b.对端}||读引用(P,窗口引用)!=std::optional{b.窗口}
        ||读引用(P,状态特征引用)!=std::optional{b.交互者特征})return 新交互求值状态::不符合;
    const auto before=读引用(P,前状态引用),after=读引用(P,后状态引用);
    if(!before||!after)throw std::runtime_error("isolated quantity process states absent");
    const auto baseline=读取本地状态(*before),updated=读取本地状态(*after);
    const auto action=读取本地动作(A);
    if(!baseline||!updated||!action)throw std::runtime_error("isolated quantity process evidence absent");
    新交互连接过程 result;
    result.过程节点=P;result.来源身份=N;result.动作节点=A;result.绑定=b;
    result.基准状态=*baseline;result.更新状态=*updated;result.接收数量定义=*quantity;
    result.动作=*action;result.预期连接方法=definition.连接方法;result.预期动作概念=definition.连接动作概念;
    const auto F=读取模拟特征(b.交互者特征);
    if(!F)throw std::runtime_error("isolated interactor feature absent");
    const auto comparison=比较接收数量状态(*updated,*baseline);
    if(comparison.状态!=新二次特征计算状态::已完成||!comparison.结果值)
        throw std::runtime_error("isolated quantity comparison unavailable");
    if(!*comparison.结果值)return 新交互求值状态::不符合;
    const auto status=核验消息连接过程(result,*F,b,definition);
    if(status==新交互求值状态::已取得)out=std::move(result);
    return status;
}

新交互写入读回 新_模拟世界类::写入本次连接交互者(
    const 方法参数场景& scene,const 新隔离连接完成材料& request) {
    新交互写入读回 out;
    if(!实现_||scene.用途()!=新方法参数场景用途::模拟)throw std::invalid_argument("isolated connection requires simulation");
    if(请求停止已成立())return out;
    auto& s=*实现_;const auto definition=读取连接构成();
    if(definition.状态!=新交互求值状态::已取得||!definition.构成)return out;
    const auto& b=request.绑定;
    const auto bound=[&](std::uint32_t p,新方法参数信息类型 t,稳定编码 n) {
        const auto* v=scene.查询参数(p);return v&&v->信息类型==t&&v->信息节点==n;
    };
    if(scene.全部角色绑定().size()!=6||!bound(1,新方法参数信息类型::存在,b.场景)
        ||!bound(2,新方法参数信息类型::存在,b.自我)||!bound(3,新方法参数信息类型::存在,b.对端)
        ||!bound(4,新方法参数信息类型::特征,b.交互者特征)||!bound(5,新方法参数信息类型::存在,b.窗口)
        ||!bound(6,新方法参数信息类型::特征,b.接收数量特征)||b.自我==b.对端)
        throw std::invalid_argument("isolated connection six bindings differ");
    std::unique_lock lock(s.互斥);
    if(!s.记录写回||s.当前调用!=std::optional{request.调用节点}||scene.全部角色绑定()!=s.通用绑定
        ||scene.方法节点()!=s.通用方法||b.场景!=s.通用场景
        ||读引用(request.调用节点,调用主动作引用)!=std::optional{request.主动作}
        ||读值(request.调用节点,调用窗口状态值)!=0||读值(request.调用节点,调用取消值)!=0)
        throw std::invalid_argument("isolated connection call differs");
    if(读引用(b.交互者特征,所属)!=std::optional{b.自我}
        ||读引用(b.接收数量特征,所属)!=std::optional{b.窗口}||读引用(b.窗口,所属)!=std::optional{b.外设场景})
        throw std::invalid_argument("isolated connection target ownership differs");
    const auto action=读取本地动作(request.主动作);
    const auto before=读取本地状态(request.基准状态),after=读取本地状态(request.更新状态);
    if(!action||!before||!after||action->动作来源节点!=std::optional{request.调用节点}
        ||action->动作定义节点!=std::optional{scene.方法节点()}
        ||action->动作目标存在节点!=std::optional{b.自我}
        ||action->动作目标特征节点!=std::optional{b.交互者特征}
        ||before->被描述存在节点!=b.窗口||after->被描述存在节点!=b.窗口
        ||before->来源实例特征节点!=b.接收数量特征||after->来源实例特征节点!=b.接收数量特征
        ||before->强时间.语义!=after->强时间.语义||before->强时间.纳秒>=after->强时间.纳秒)
        throw std::invalid_argument("isolated connection state evidence differs");
    const auto comparison=比较接收数量状态(*after,*before);
    if(comparison.状态!=新二次特征计算状态::已完成||!comparison.结果值||!*comparison.结果值)
        throw std::runtime_error("isolated connection has no quantity state update");
    if(!全局基础数据集.查询字段(request.调用节点,字段组[集合成员]).empty()) {
        新交互连接过程 process;
        if(读取隔离连接过程(request.调用节点,request.主动作,b,*definition.构成,process)!=新交互求值状态::已取得
            ||process.基准状态.节点!=request.基准状态||process.更新状态.节点!=request.更新状态)
            throw std::runtime_error("isolated connection process identity conflict");
        out.状态=新交互求值状态::已取得;return out;
    }
    lock.unlock();out=写入交互者(b,request.主动作);
    if(out.状态!=新交互求值状态::已取得)return out;
    lock.lock();
    // 交互者写回本身会新增引用正式概念的状态/动态，旧构成保护随之过期；重读同一构成并用其当前保护发布过程。
    const auto 当前构成=读取连接构成();
    if(当前构成.状态!=新交互求值状态::已取得||!当前构成.构成||[&]{const auto& x=*当前构成.构成;const auto& y=*definition.构成;
            return x.关系概念!=y.关系概念||x.连接方法!=y.连接方法||x.连接动作概念!=y.连接动作概念
                ||x.通用存在概念!=y.通用存在概念||x.窗口范围概念!=y.窗口范围概念||x.接收数量特征概念!=y.接收数量特征概念;}())
        throw std::runtime_error("isolated connection composition changed during target update");
    基础节点组追加请求 q;q.预期联合节点组=当前构成.完整保护;
    q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(过程种类)}},{字段组[根],模拟世界树根节点},
        {字段组[所属],b.场景},{字段组[来源引用],request.调用节点},{字段组[动作引用],request.主动作},
        {字段组[对端引用],b.对端},{字段组[窗口引用],b.窗口},{字段组[状态特征引用],b.交互者特征},
        {字段组[前状态引用],request.基准状态},{字段组[后状态引用],request.更新状态}}});
    q.追加字段组.push_back({request.调用节点,字段组[集合成员],基础组内节点引用{0}});
    q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
    s.追加进展(q);补齐隔离追加联合预期(q);s.预留追加(q);
    const auto published=全局基础数据集.原子追加节点组(q);
    if(published.状态!=基础节点组追加状态::已完成||published.新节点组.size()!=1)
        throw std::runtime_error("isolated connection process publish failed after target update");
    s.收集追加(published);
    新交互连接过程 process;
    if(读取隔离连接过程(request.调用节点,request.主动作,b,*definition.构成,process)!=新交互求值状态::已取得
        ||process.过程节点!=published.新节点组.front())throw std::runtime_error("isolated quantity process readback failed");
    return out;
}

新交互求值状态 新_模拟世界类::读取本次结果场景事实(const 新方法本次场景事实引用& ref,
    const std::vector<新方法参数角色绑定>& bindings,稳定编码 expectedN,稳定编码 expectedA,新交互连接过程& out) const {
    out={};if(!实现_)return 新交互求值状态::材料不足;
    if(ref.事实来源身份!=expectedN||ref.动作节点!=expectedA)return 新交互求值状态::不符合;
    const auto definition=读取连接构成();
    if(definition.状态!=新交互求值状态::已取得||!definition.构成)return definition.状态;
    if(ref.限定!=新方法初始场景事实限定{definition.构成->关系概念,1,2,3,4})return 新交互求值状态::不支持;
    auto& s=*实现_;新交互绑定 b;
    {
        std::lock_guard lock(s.互斥);
        if(!s.通用||!s.写回窗口已关闭||s.记录写回||s.当前调用!=std::optional{expectedN}
            ||!属于样例(expectedN,调用种类,s.通用场景)||读值(expectedN,调用窗口状态值)!=1
            ||读值(expectedN,调用取消值)!=0||读引用(expectedN,方法引用)!=std::optional{s.通用方法}
            ||读引用(expectedN,所属)!=std::optional{s.通用场景}||读引用(expectedN,调用主动作引用)!=std::optional{expectedA}
            ||bindings!=s.通用绑定)return 新交互求值状态::不符合;
        const auto at=[&](std::size_t i,新方法参数信息类型 type) {
            if(i>=bindings.size()||!bindings[i].有序引用||bindings[i].有序引用->序号!=i+1
                ||bindings[i].有序引用->信息类型!=type)throw std::invalid_argument("isolated result ordered binding differs");
            return bindings[i].有序引用->信息节点;
        };
        b.场景=at(0,新方法参数信息类型::存在);b.自我=at(1,新方法参数信息类型::存在);b.对端=at(2,新方法参数信息类型::存在);
        b.交互者特征=at(3,新方法参数信息类型::特征);b.窗口=at(4,新方法参数信息类型::存在);
        b.接收数量特征=at(5,新方法参数信息类型::特征);
        const auto C=读引用(b.窗口,所属);if(!C)throw std::runtime_error("isolated result window scene absent");b.外设场景=*C;
    }
    return 读取隔离连接过程(expectedN,expectedA,b,*definition.构成,out);
}
}
