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
#include "数据操作.执行结果参与.h"
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
// 名称查询返回的多个正式概念按编码排序去重。
using 归类签名 = std::vector<std::uint64_t>;

归类签名 规范归类签名(归类签名 签名) {
    std::sort(签名.begin(),签名.end());
    签名.erase(std::unique(签名.begin(),签名.end()),签名.end());
    return 签名;
}

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
    引用内容需求引用,引用内容路径引用,完善阶段值,询问入列成员,
    词条语言引用,词条规范化合同引用,词条词面值,词条特征引用,
    词典回执引用,词典格式,词典词条,词典材料特征,词典外设场景,词典查询记录,
    词典查阅结局,词典采用阶段,词典新状态标记,词典材料,词典前状态,词典后状态,
    词典变化阶段,词典无需原因,词典变化动态,动作观察动态引用,归类目标词条引用,归类采用概念引用,普通名称前态已知,普通名称成员,集合概念引用,全部名称前态已知,全部名称概念成员,分界采用回执引用,字段数 };
enum 节点种类 : std::int64_t { 根种类 = 1, 场景种类, 存在种类, 特征种类, 材料种类,
    任务种类, 虚拟种类, 动作种类, 状态种类, 动态种类,
    消息种类, 过程种类,
    调用种类,阶段来源种类,传输种类,需求种类,理解种类,确认种类,要求种类,
    语言场景种类,语义快照种类,语言实例关联种类,需求发布种类,队列入列种类,词条种类,词典回执种类,归类材料种类,模拟普通名称种类,模拟归类特征概念种类,
    语言范围种类, 语言分界材料种类, 分界采用回执种类 };
std::array<稳定编码, 字段数> 字段组{};
std::mutex 根互斥;
struct 模拟联合预期变化 final : std::runtime_error {
    模拟联合预期变化() : std::runtime_error("simulation input definition premise changed") {}
};
void 合并模拟联合预期(基础节点组追加请求& q) {
    std::map<稳定编码,基础联合节点快照> merged;
    for(const auto& p:q.预期联合节点组) {
        const auto [it,added]=merged.emplace(p.结构.节点.编码,p);
        if(!added&&(it->second.结构.字段组!=p.结构.字段组||it->second.入向字段组!=p.入向字段组
            ||it->second.入向外部关系组!=p.入向外部关系组||it->second.出向外部关系组!=p.出向外部关系组))
            throw 模拟联合预期变化{};
    }
    q.预期联合节点组.clear();for(auto& [n,p]:merged)q.预期联合节点组.push_back(std::move(p));
}
// 外部边及字段替换/删除必须保护既有端点的完整联合结构。
void 补齐模拟追加联合预期(基础节点组追加请求& q) {
    std::vector<稳定编码> 节点;
    for (const auto& s : q.预期既有节点组) 节点.push_back(s.节点.编码);
    const auto 需要完整端点=[&](稳定编码 n) {
        return std::find(q.窄关系端点组.begin(),q.窄关系端点组.end(),n)==q.窄关系端点组.end();
    };
    for (const auto& e : q.追加外部关系组) {
        if (const auto* n = std::get_if<稳定编码>(&e.源节点);n && 需要完整端点(*n)) 节点.push_back(*n);
        if (const auto* n = std::get_if<稳定编码>(&e.目标节点);n && 需要完整端点(*n)) 节点.push_back(*n);
    }
    for (const auto& s : q.预期既有节点组) for (const auto& f : s.字段组)
        if (const auto* n = std::get_if<稳定编码>(&f.内容)) 节点.push_back(*n);
    std::sort(节点.begin(),节点.end());
    节点.erase(std::unique(节点.begin(),节点.end()),节点.end());
    auto r = 全局基础数据集.查询节点组完整结构(节点);
    if (r.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
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
    if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
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
    std::size_t 起点=0;
    std::optional<稳定编码> 已有场景;
    std::size_t 节点(std::int64_t subtype=0) {
        const auto n=起点+q.新节点组.size();
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(语义快照种类)}},
            {字段组[根],模拟世界树根节点},{字段组[结构类别值],基础值{subtype}}}});
        q.追加外部关系组.push_back({已有场景 ? std::variant<稳定编码,基础组内节点引用>{*已有场景}
            : std::variant<稳定编码,基础组内节点引用>{基础组内节点引用{场景}},基础组内节点引用{n},基础外部关系类型::父子});return n;
    }
    void 写(std::size_t n,字段 key,基础组内字段内容 v){q.新节点组.at(n-起点).字段组.push_back({字段组[key],std::move(v)});}
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
            } else if constexpr(std::is_same_v<T,特征概念概念集合值域>) {
                for(const auto x:value.必含概念节点组)写(n,结构引用,x);
                数(n,结构顺序值,static_cast<std::int64_t>(value.至少一个概念));
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
            q.新节点组[x-起点].字段组.erase(q.新节点组[x-起点].字段组.begin()+2);
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
    case 10:return 特征概念概念集合值域{读引用组(n,结构引用),必读数(n,结构顺序值)!=0};
    default:throw std::runtime_error("local domain kind corrupt");
    }
}
// 同批解码归类特征复制域，概念身份值直接保留引用。
特征概念值域 读归类结构域批次(稳定编码 n,const std::vector<基础联合节点读取项>& batch) {
    const auto row=std::find_if(batch.begin(),batch.end(),[&](const auto& item){return item.查询节点==n;});
    if(row==batch.end() || !row->快照)throw std::logic_error("classification domain snapshot missing");
    std::optional<std::int64_t> tag,order;std::vector<稳定编码> refs;
    for(const auto& field:row->快照->结构.字段组)if(field.字段节点==字段组[结构类别值]) {
        const auto raw=std::get_if<基础值>(&field.内容);
        const auto value=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
        if(tag || !value)throw std::logic_error("classification domain tag invalid");
        tag=*value;
    }
    if(tag==2)return 特征概念全材料值域{};
    if(tag==3 || tag==9 || tag==10)for(const auto& field:row->快照->结构.字段组) {
        if(field.字段节点==字段组[结构引用]) {
            const auto id=std::get_if<稳定编码>(&field.内容);
            if(!id || !有效(*id))throw std::logic_error("classification domain reference invalid");
            refs.push_back(*id);
        } else if((tag==9 || tag==10) && field.字段节点==字段组[结构顺序值]) {
            const auto raw=std::get_if<基础值>(&field.内容);
            const auto value=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
            if(order || !value)throw std::logic_error("classification domain order invalid");
            order=*value;
        }
    }
    if(tag==3)return refs;
    if(tag==9 && refs.size()==1 && order && (*order==1 || *order==2))
        return 特征概念信息引用值域{static_cast<信息引用语义>(*order),refs.front()};
    if(tag==10 && order)return 特征概念概念集合值域{std::move(refs),*order!=0};
    throw std::logic_error("classification domain shape unsupported");
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
    std::optional<新模拟归类主体输入> 归类主体来源;
    std::mutex 归类动作阶段互斥;
    bool 归类动作在途=false;
    bool 归类观察在途=false;
    struct 本地归类调用 final {
        新词条归类本能输入 输入;
        新本能函数本次条件 条件;
        std::optional<稳定编码> 动作;
        std::optional<新本能词条归类回执> 回执;
        // L1实际前后字段，用于后继状态、动态处理；不会重新读取替代。
        std::vector<基础当前字段变化> 实际字段变化;
    };
    std::map<稳定编码,本地归类调用> 归类调用组;
    std::optional<稳定编码> 本地执行主体;
    mutable std::optional<稳定编码> 需求动作许可;
    std::optional<稳定编码> 当前调用;
    std::function<bool()> 停止检查;
    bool 请求取消=false;
    bool 通用=false;
    bool 样例就绪=false;
    std::shared_ptr<新本能词典运行接口> 词典运行;
    词典材料类* 词典材料=nullptr;
    外设词典记录类* 词典记录=nullptr;
    新_特征值类* 词典值=nullptr;
    概念_特征类* 词典保持=nullptr;
    稳定编码 词典接收FT;
    std::optional<新模拟词典接收定位> 词典接收定位;
    std::vector<新词典查询定位> 词典查询责任;
    std::vector<稳定编码> 词典接收值组;
    std::size_t 词典在途=0;
    std::optional<稳定编码> 词典材料组织;
    std::vector<std::pair<稳定编码,稳定编码>> 词典材料来源; // localQ -> sourceQ，仅技术来源映射。
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
            &&!it->second.需求参数输入&&!it->second.复制归类材料&&!本地需求.contains(n)
            &&(!本地语言范围||本地语言范围->本地范围!=n);
    }
    struct 语言范围索引 final {
        稳定编码 本地范围, 来源范围, 结构概念;
        std::array<稳定编码, 4> 本地特征, 来源特征;
    };
    std::optional<语言范围索引> 本地语言范围;
    std::map<稳定编码,材料物理类型> 通用材料;
    // 值式排序元数据，不是对正式材料的持久引用。
    std::map<稳定编码,稳定编码> 复制材料来源顺序;
    // 仅身份/来源索引；业务内容始终从L1字段完整重建。
    std::map<稳定编码,std::optional<稳定编码>> 本地需求,本地理解,本地确认,本地要求,本地语言场景,本地归类材料;
    std::map<稳定编码,std::vector<std::uint32_t>> 内容参数位置;
    mutable std::vector<新模拟内容读取记录> 内容读取组;
    mutable std::vector<新隔离归类名称读取> 归类名称读取组;
    template<class T> void 记录内容(std::uint8_t kind,稳定编码 n,const T& value,bool copied) {
        if(!记录复制读取)return;
        // 有类型的读取由学习侧逐条覆盖；该标志只保留给无正式表达的隐藏读取。
        (void)copied;
        std::optional<bool> 归类已提交;
        if(归类主体来源) {
            const auto it=当前调用 ? 归类调用组.find(*当前调用) : 归类调用组.end();
            归类已提交=it!=归类调用组.end() && it->second.回执.has_value();
        }
        内容读取组.push_back({kind,n,内容参数位置[n],value,!本次已进入动作,归类已提交});
    }
    struct 任务投影 final { 稳定编码 来源; 新任务虚拟存在特征组 本地; };
    std::map<稳定编码,任务投影> 通用任务;
    std::vector<稳定编码> 通用节点,通用字段,通用外部边;
    // 首次Q由材料owner提交：稳定槽在发表前分配，回调只保存不可变回执。
    struct 词典回执追踪 final {
        std::optional<基础片段发布回执> receipt;
        std::size_t nodeStart=0,fieldCount=0,edgeCount=0;
    };
    std::vector<std::shared_ptr<词典回执追踪>> 词典回执组;
    void 合并词典回执退出(基础节点组追加请求& q) const {
        for(const auto& slot:词典回执组)if(slot->receipt) {
            const auto& r=*slot->receipt;
            q.删除节点组.insert(q.删除节点组.end(),r.新节点组().begin()+slot->nodeStart,r.新节点组().end());
            q.删除字段关系组.insert(q.删除字段关系组.end(),r.追加字段关系组().end()-slot->fieldCount,r.追加字段关系组().end());
            q.删除外部关系组.insert(q.删除外部关系组.end(),r.追加外部关系组().end()-slot->edgeCount,r.追加外部关系组().end());
            for(std::size_t i=slot->nodeStart;i<r.新节点初始字段关系组().size();++i) {
                const auto& fields=r.新节点初始字段关系组()[i];q.删除字段关系组.insert(q.删除字段关系组.end(),fields.begin(),fields.end());
            }
        }
    }
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
        std::size_t fields=q.追加字段组.size()+q.当前字段赋值组.size();
        for(const auto& n:q.新节点组)fields+=n.字段组.size();
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
        for(const auto& change:p.当前字段变化组)
            if(!change.前)通用字段.push_back(change.后.编码);
        通用外部边.insert(通用外部边.end(),p.追加外部关系组.begin(),p.追加外部关系组.end());
    }

    explicit 实现(新_方法类& m) : 方法(m) {}
};
namespace {
void 本地词典要求(bool ok,const char* message) {if(!ok)throw std::logic_error(message);}
const 基础字段关系* 本地词典字段(const 基础节点结构& n,字段 key) {
    const 基础字段关系* out=nullptr;
    for(const auto& f:n.字段组)if(f.字段节点==字段组[key]) {
        本地词典要求(!out,"duplicate local dictionary receipt field");out=&f;
    }
    return out;
}
std::optional<稳定编码> 本地词典引用(const 基础节点结构& n,字段 key) {
    const auto* f=本地词典字段(n,key);if(!f)return {};
    const auto* v=std::get_if<稳定编码>(&f->内容);
    本地词典要求(v&&有效(*v),"invalid local dictionary receipt reference");return *v;
}
std::optional<std::int64_t> 本地词典整数(const 基础节点结构& n,字段 key) {
    const auto* f=本地词典字段(n,key);if(!f)return {};
    const auto* raw=std::get_if<基础值>(&f->内容);
    const auto* v=raw?std::get_if<std::int64_t>(&raw->材料):nullptr;
    本地词典要求(v!=nullptr,"invalid local dictionary receipt number");return *v;
}
std::uint64_t 本地词典标识(const 基础节点结构& n,字段 key) {
    const auto* f=本地词典字段(n,key);const auto* raw=f?std::get_if<基础值>(&f->内容):nullptr;
    const auto* v=raw?std::get_if<std::vector<std::uint64_t>>(&raw->材料):nullptr;
    本地词典要求(v&&v->size()==1&&v->front()!=0,"invalid local dictionary external identity");return v->front();
}
std::optional<基础联合节点快照> 本地词典快照(稳定编码 N) {
    auto r=全局基础数据集.查询节点组完整结构({N});
    if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    本地词典要求(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1,"local dictionary snapshot unavailable");
    return std::move(r.节点组.front().快照);
}
新本能词典读取回执 解码本地词典回执(const 基础节点结构& n, 稳定编码 A) {
    本地词典要求(本地词典整数(n,类型)==词典回执种类 && 本地词典整数(n,词典格式)==1,
        "dictionary receipt schema invalid");
    新本能词典读取回执 r;
    r.动作=A;
    r.词条=本地词典引用(n,词典词条).value_or(稳定编码{});
    r.材料特征=本地词典引用(n,词典材料特征).value_or(稳定编码{});
    r.查询来源={本地词典标识(n,词典外设场景),本地词典标识(n,词典查询记录)};
    const auto outcome=本地词典整数(n,词典查阅结局), stage=本地词典整数(n,词典采用阶段);
    const auto fresh=本地词典整数(n,词典新状态标记);
    本地词典要求(有效(A)&&有效(r.词条)&&有效(r.材料特征)&&outcome&&(*outcome==1||*outcome==2)
        &&stage&&*stage>=1&&*stage<=3&&fresh&&(*fresh==0||*fresh==1),"dictionary receipt required material missing");
    r.查阅结局=static_cast<新词典查阅结局>(*outcome);
    r.采用阶段=static_cast<新词典材料采用阶段>(*stage);
    r.本次形成新状态=*fresh==1;
    r.材料Q=本地词典引用(n,词典材料);
    r.前状态=本地词典引用(n,词典前状态);
    r.后状态=本地词典引用(n,词典后状态);
    r.变化动态=本地词典引用(n,词典变化动态);
    if(const auto v=本地词典整数(n,词典变化阶段)) {
        本地词典要求(*v>=1&&*v<=3,"dictionary receipt change stage invalid");
        r.变化阶段=static_cast<新词典变化处理阶段>(*v);
    }
    if(const auto v=本地词典整数(n,词典无需原因)) {
        本地词典要求(*v>=0&&*v<=2,"dictionary receipt no-change reason invalid");
        r.无需形成原因=static_cast<新词条动态记录原因>(*v);
    }
    if(r.查阅结局==新词典查阅结局::正式未命中) {
        本地词典要求(r.采用阶段==新词典材料采用阶段::未取得&&!r.材料Q&&!r.前状态&&!r.后状态
            &&!r.本次形成新状态&&!r.变化阶段&&!r.无需形成原因&&!r.变化动态,
            "dictionary miss contains invented material");
    } else {
        本地词典要求(r.材料Q.has_value()&&r.采用阶段!=新词典材料采用阶段::未取得,"dictionary hit missing material");
        if(r.采用阶段==新词典材料采用阶段::已取得未采用)
            本地词典要求(!r.前状态&&!r.后状态&&!r.本次形成新状态&&!r.变化阶段&&!r.无需形成原因&&!r.变化动态,
                "unadopted dictionary material has change evidence");
        else {
            本地词典要求(r.变化阶段.has_value()&&r.本次形成新状态==r.后状态.has_value(),"dictionary adoption stage incomplete");
            if(*r.变化阶段==新词典变化处理阶段::已形成)
                本地词典要求(r.变化动态.has_value()&&!r.无需形成原因&&r.前状态&&r.后状态,"dictionary change evidence missing");
            else if(*r.变化阶段==新词典变化处理阶段::无需形成)
                本地词典要求(!r.变化动态&&r.无需形成原因.has_value(),"dictionary no-change reason missing");
            else 本地词典要求(!r.变化动态&&!r.无需形成原因&&r.后状态.has_value(),"dictionary pending change incomplete");
        }
    }
    return r;
}
struct 本地词典调用读取 final {
    基础联合节点快照 N;
    std::optional<基础联合节点快照> 记录;
    std::optional<新本能词典读取回执> 回执;
    稳定编码 动作;
};
std::optional<本地词典调用读取> 读取本地词典调用(稳定编码 N) {
    for(;;) {
        auto n=本地词典快照(N);if(!n)return {};
        本地词典要求(本地词典整数(n->结构,类型)==调用种类,"dictionary receipt owner is not local call");
        本地词典调用读取 result{std::move(*n)};
        result.动作=本地词典引用(result.N.结构,调用主动作引用).value_or(稳定编码{});
        const auto record=本地词典引用(result.N.结构,词典回执引用);
        if(!record)return result;
        result.记录=本地词典快照(*record);
        if(!result.记录) {
            const auto current=本地词典快照(N);
            if(!current||本地词典引用(current->结构,词典回执引用)!=record)continue;
            throw std::logic_error("local call refers to missing dictionary receipt");
        }
        result.回执=解码本地词典回执(result.记录->结构,result.动作);return result;
    }
}
基础组内新节点 构造本地词典首次记录(const 新词典执行结果来源& source,bool hit) {
    return {{{字段组[类型],基础值{static_cast<std::int64_t>(词典回执种类)}},
        {字段组[根],模拟世界树根节点},{字段组[词典格式],基础值{std::int64_t{1}}},
        {字段组[词典词条],source.词条},{字段组[词典材料特征],source.材料特征},
        {字段组[词典外设场景],基础值{std::vector<std::uint64_t>{source.查询来源.场景}}},
        {字段组[词典查询记录],基础值{std::vector<std::uint64_t>{source.查询来源.查询记录}}},
        {字段组[词典查阅结局],基础值{static_cast<std::int64_t>(hit?新词典查阅结局::命中:新词典查阅结局::正式未命中)}},
        {字段组[词典采用阶段],基础值{static_cast<std::int64_t>(hit?新词典材料采用阶段::已取得未采用:新词典材料采用阶段::未取得)}},
        {字段组[词典新状态标记],基础值{std::int64_t{0}}}}};
}
// 构造面只消费已捕获材料；不读world、不取业务锁。回执只追踪本产品后缀。
class 本地词典结果准备 final : public 新词典结果参与准备 {
    const std::thread::id thread_=std::this_thread::get_id();
    本地词典调用读取 read_;
    新词典执行结果来源 source_;
    std::optional<稳定编码> previous_;
    基础节点组追加请求 base_;
    稳定编码 scene_,organization_;
    bool adoption_=false, published_=false, done_=false, cancelled_=false;
    mutable bool built_=false;
    mutable std::size_t start_=0,fields_=0,edges_=0;
    std::optional<基础片段发布回执> receipt_;
    std::function<void(const 基础片段发布回执&,std::size_t,std::size_t,std::size_t)> track_;
    void Begin() const {
        本地词典要求(thread_==std::this_thread::get_id()&&!built_&&!published_&&!cancelled_,"local dictionary preparation reused");built_=true;
    }
    基础节点组追加请求 Adopt(bool fresh,std::optional<基础组内字段内容> state) const {
        本地词典要求(adoption_&&read_.记录&&read_.回执&&read_.回执->材料Q,"local dictionary adoption missing");
        auto q=base_;const auto R=read_.记录->结构.节点.编码;
        const auto replace=[&](字段 key,std::int64_t v) {
            const auto* f=本地词典字段(read_.记录->结构,key);
            本地词典要求(f!=nullptr,"local receipt adoption field missing");q.替换字段组.push_back({f->编码,基础值{v}});
        };
        replace(词典采用阶段,static_cast<std::int64_t>(新词典材料采用阶段::已采用));replace(词典新状态标记,fresh?1:0);
        if(fresh&&previous_)q.追加字段组.push_back({R,字段组[词典前状态],*previous_});
        if(state)q.追加字段组.push_back({R,字段组[词典后状态],*state});
        const bool pending=fresh&&previous_.has_value();
        q.追加字段组.push_back({R,字段组[词典变化阶段],基础值{static_cast<std::int64_t>(pending?新词典变化处理阶段::待处理:新词典变化处理阶段::无需形成)}});
        if(!pending)q.追加字段组.push_back({R,字段组[词典无需原因],基础值{static_cast<std::int64_t>(fresh?新词条动态记录原因::无前态:新词条动态记录原因::无值变化)}});
        fields_=q.追加字段组.size();edges_=q.追加外部关系组.size();return q;
    }
public:
    本地词典结果准备(本地词典调用读取 read,新词典执行结果来源 source,std::optional<稳定编码> previous,
        稳定编码 scene,稳定编码 organization,bool adoption,基础节点组追加请求 base,decltype(track_) track)
        :read_(std::move(read)),source_(source),previous_(previous),base_(std::move(base)),scene_(scene),organization_(organization),adoption_(adoption),track_(std::move(track)) {}
    ~本地词典结果准备() override {
        if(published_&&!done_)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"隔离词典结果准备析构",L"已发表回执尚未完成记账。");
    }
    新材料结果追加构造 构造同组材料引用(const 新同组材料端点& endpoint) const override {
        Begin();本地词典要求(!adoption_&&endpoint.内部组织==organization_,"local acquisition organization mismatch");
        auto q=base_;auto record=构造本地词典首次记录(source_,true);
        std::visit([&](auto value){record.字段组.push_back({字段组[词典材料],value});},endpoint.材料定位);
        start_=endpoint.后续新节点起点;q.新节点组.push_back(std::move(record));
        q.追加字段组.push_back({read_.N.结构.节点.编码,字段组[词典回执引用],基础组内节点引用{start_}});
        q.追加外部关系组.push_back({scene_,基础组内节点引用{start_},基础外部关系类型::父子});
        fields_=q.追加字段组.size();edges_=q.追加外部关系组.size();return {std::move(q)};
    }
    新状态结果追加构造 构造同组状态引用(const 新同组状态端点& endpoint) const override {
        Begin();本地词典要求(read_.回执&&endpoint.被描述节点==source_.词条&&endpoint.来源特征节点==source_.材料特征
            &&endpoint.固定准确值==std::optional<新特征准确值>{新特征准确值{*read_.回执->材料Q}},"local adoption state endpoint mismatch");
        start_=endpoint.后续新节点起点;
        return {Adopt(true,std::visit([](auto v)->基础组内字段内容{return v;},endpoint.状态定位)),0};
    }
    新材料结果追加构造 构造无新状态采用() const override {Begin();return {Adopt(false,{})};}
    void 登记共同发表回执(const 基础片段发布回执& r) noexcept override {
        if(thread_!=std::this_thread::get_id()||!built_||published_||cancelled_)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"登记隔离词典结果回执",L"发表阶段不合法。");
        receipt_=r;published_=true;
        // 采用产品没有新节点，取得产品的节点位于Q组之后。
        track_(r,adoption_?r.新节点组().size():start_,fields_,edges_);
    }
    void 完成共同发表() noexcept override {
        if(thread_!=std::this_thread::get_id()||!published_||!receipt_)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"完成隔离词典结果回执",L"没有本次成功回执。");
        done_=true;
    }
    void 取消未发表准备() noexcept override {
        if(thread_!=std::this_thread::get_id()||published_)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"取消隔离词典结果准备",L"不能取消已发表事实。");
        cancelled_=true;
    }
};
} // namespace
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
void 新_模拟世界类::设置词典运行(std::shared_ptr<新本能词典运行接口> runtime) {
    if(!实现_||!runtime)throw std::invalid_argument("local dictionary runtime missing");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(s.通用||s.词典运行)throw std::logic_error("local dictionary runtime already bound");
    s.词典运行=std::move(runtime);
}
新本能词典运行接口* 新_模拟世界类::获取词典运行接口() noexcept {
    if(!实现_)return nullptr;std::lock_guard lock(实现_->互斥);return 实现_->词典运行.get();
}
std::optional<稳定编码> 新_模拟世界类::读取本地词典材料组织() const noexcept {
    if(!实现_)return {};std::lock_guard lock(实现_->互斥);return 实现_->词典材料组织;
}

新状态强时间 新_模拟世界类::读取当前模拟时间() const {
    if(!实现_)throw std::logic_error("simulation time owner unavailable");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||!s.记录写回)throw std::logic_error("simulation time requested outside active call");
    return {新状态时间语义::抽象相对,s.模拟步骤};
}
void 新_模拟世界类::配置词典材料(词典材料类& owner) {
    if(!实现_)throw std::logic_error("simulation world unavailable");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(s.通用||(s.词典材料&&s.词典材料!=&owner))
        throw std::logic_error("simulation dictionary material owner already in use");
    s.词典材料=&owner;
}
新词典材料场景读取结果 新_模拟世界类::读取材料所属场景(稳定编码 L) const noexcept {
    using S=新词典材料操作状态;
    try {
        if(!实现_)return {S::未装配,{}};
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||!s.本地语言场景.contains(L))return {S::未找到,{}};
        if(!属于样例(L,语言场景种类,s.通用场景))
            throw std::logic_error("local dictionary language scene damaged");
        return {S::已完成,新词典材料场景身份{L,新词典隔离语言归属{s.通用场景}}};
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::exception& e){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取隔离材料场景",L"样例场景结构损坏。",e.what());}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取隔离材料场景",L"未知工程异常。");}
}
新词典材料读取结果 新_模拟世界类::读取本地词典材料(稳定编码 Q) const noexcept {
    using S=新词典材料操作状态;
    try {
        if(!实现_)return {S::未装配,{}};
        词典材料类* owner=nullptr;std::optional<稳定编码> O;
        {auto& s=*实现_;std::lock_guard lock(s.互斥);
            if(!s.通用||!s.样例就绪)return {S::未找到,{}};
            owner=s.词典材料;O=s.词典材料组织;
        }
        if(!owner)return {S::未装配,{}};
        if(!O)return {S::未找到,{}};
        auto r=owner->读取材料(Q);
        if(r.信息&&r.信息->内部组织!=*O)return {S::未找到,{}};
        return r;
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::exception& e){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取隔离词典材料",L"本地材料结构损坏。",e.what());}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取隔离词典材料",L"未知工程异常。");}
}
新词典执行回执读取结果 新_模拟世界类::读取调用词典回执(稳定编码 N) const noexcept {
    using S=新词典材料操作状态;
    try {
        if(!实现_)return {S::未装配,{}};
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||std::find(s.通用节点.begin(),s.通用节点.end(),N)==s.通用节点.end())return {S::未找到,{}};
        const auto r=读取本地词典调用(N);
        if(r&&r->回执)return {S::已完成,r->回执};
        return {S::未找到,{}};
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::length_error&){return {S::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取隔离调用词典回执",L"本地结果结构读取失败。");}
}
新词典结果参与准备结果 新_模拟世界类::准备调用词典材料参与(
    稳定编码 N,const 新词典执行结果来源& source) noexcept {
    return 准备调用词典结果内部(N,&source,{},{});
}
新词典结果参与准备结果 新_模拟世界类::准备调用词典采用参与(
    稳定编码 N,稳定编码 Q,std::optional<稳定编码> previous) noexcept {
    return 准备调用词典结果内部(N,nullptr,Q,previous);
}
新词典结果参与准备结果 新_模拟世界类::准备调用词典结果内部(
    稳定编码 N,const 新词典执行结果来源* source,std::optional<稳定编码> Q,std::optional<稳定编码> previous) noexcept {
    using S=新词典材料操作状态;
    try {
        if(!实现_)return {S::未装配,{},{}};
        if(Q) {
            const auto local=读取本地词典材料(*Q);
            if(local.状态==S::资源失败)return {S::资源失败,{},{}};
            if(!local.信息)return {S::输入拒绝,{},{}};
        }
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||s.当前调用!=std::optional{N})return {S::未找到,{},{}};
        auto read=读取本地词典调用(N);if(!read)return {S::未找到,{},{}};
        if(read->回执&&(!Q||read->回执->采用阶段==新词典材料采用阶段::已采用
            ||read->回执->查阅结局==新词典查阅结局::正式未命中))return {S::已复用,{},read->回执};
        if(!s.样例就绪||!s.记录写回||s.请求取消||!有效(read->动作)
            ||本地词典整数(read->N.结构,调用窗口状态值)!=0)return {S::条件不具备,{},{}};
        新词典执行结果来源 actual;
        if(Q) {
            if(!read->回执)return {S::条件不具备,{},{}};
            if(read->回执->材料Q!=Q)return {S::输入拒绝,{},{}};
            actual={read->回执->词条,read->回执->材料特征,read->回执->查询来源};
        } else {
            if(!source||!有效(source->词条)||!有效(source->材料特征)
                ||!source->查询来源.场景||!source->查询来源.查询记录)return {S::输入拒绝,{},{}};
            actual=*source;
        }
        if(!s.词典材料组织)return {S::条件不具备,{},{}};
        if(s.通用输入节点.size()<6||s.通用输入节点[0]!=actual.词条||s.通用输入节点[1]!=actual.材料特征
            ||!s.是本地参数(actual.词条,新方法参数信息类型::词条)
            ||读引用(actual.材料特征,所属)!=std::optional{actual.词条})return {S::输入拒绝,{},{}};
        if(previous) {
            const auto state=读取本地状态(*previous);
            if(!state||state->被描述存在节点!=actual.词条||state->来源实例特征节点!=actual.材料特征)
                return {S::输入拒绝,{},{}};
        }
        基础节点组追加请求 base;
        base.预期联合节点组.push_back(read->N);
        if(read->记录)base.预期联合节点组.push_back(*read->记录);
        // L1只保护本轮实际记录及必需身份；不证明所有上层内容保持不变。
        const std::vector<稳定编码> ids{actual.词条,actual.材料特征,s.通用场景};
        auto identity=全局基础数据集.查询节点组完整结构(ids);
        if(identity.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        本地词典要求(identity.状态==基础结构操作状态::已完成,"local receipt identity read failed");
        for(const auto& item:identity.节点组) {
            if(!item.快照)return {S::条件不具备,{},{}};
            base.只读预期.节点组.push_back(item.快照->结构.节点);
        }
        std::erase_if(s.词典回执组,[](const auto& slot){return !slot->receipt&&slot.use_count()==1;});
        const auto slot=std::make_shared<实现::词典回执追踪>();
        s.词典回执组.push_back(slot);
        const auto track=[&s,slot](const 基础片段发布回执& receipt,std::size_t start,std::size_t fields,std::size_t edges) noexcept {
            std::lock_guard lock(s.互斥);
            if(slot->receipt||start>receipt.新节点组().size()||fields>receipt.追加字段关系组().size()
                ||edges>receipt.追加外部关系组().size())
                工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"登记隔离词典追踪",L"片段回执范围不匹配。");
            slot->receipt=receipt;slot->nodeStart=start;slot->fieldCount=fields;slot->edgeCount=edges;
        };
        auto p=std::make_unique<本地词典结果准备>(std::move(*read),actual,previous,s.通用场景,*s.词典材料组织,Q.has_value(),std::move(base),track);
        return {S::已完成,std::move(p),{}};
    }catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"准备隔离词典结果",L"本地结果参与准备失败。");}
}
新词典材料操作状态 新_模拟世界类::记录调用词典未命中(
    稳定编码 N,const 新词典执行结果来源& source) noexcept {
    using S=新词典材料操作状态;
    try {
        if(!实现_)return S::未装配;
        if(!有效(source.词条)||!有效(source.材料特征)||!source.查询来源.场景||!source.查询来源.查询记录)return S::输入拒绝;
        auto& s=*实现_;
        for(;;) {
            std::unique_lock lock(s.互斥);
            if(!s.通用||s.当前调用!=std::optional{N})return S::未找到;
            const auto read=读取本地词典调用(N);if(!read)return S::未找到;
            if(read->回执)return S::已复用;
            if(!s.样例就绪||!s.记录写回||s.请求取消||!有效(read->动作)
                ||本地词典整数(read->N.结构,调用窗口状态值)!=0)return S::条件不具备;
            if(s.通用输入节点.size()<6||s.通用输入节点[0]!=source.词条||s.通用输入节点[1]!=source.材料特征
                ||!s.是本地参数(source.词条,新方法参数信息类型::词条)
                ||读引用(source.材料特征,所属)!=std::optional{source.词条})return S::输入拒绝;
            基础节点组追加请求 q;q.新节点组.push_back(构造本地词典首次记录(source,false));
            q.追加字段组.push_back({N,字段组[词典回执引用],基础组内节点引用{0}});
            q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
            补齐隔离追加联合预期(q);q.预期联合节点组.push_back(read->N);s.预留追加(q);
            const auto published=全局基础数据集.原子追加节点组(q);
            if(published.状态==基础节点组追加状态::已完成){s.收集追加(published);return S::已完成;}
            if(published.状态==基础节点组追加状态::资源失败)return S::资源失败;
            if(published.状态!=基础节点组追加状态::预期不符)throw std::logic_error("local dictionary miss publication rejected");
            lock.unlock();std::this_thread::yield();
        }
    }catch(const std::bad_alloc&){return S::资源失败;}
    catch(const std::length_error&){return S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"记录隔离词典未命中",L"本地未命中记录失败。");}
}
struct 新模拟词典采用准备::实现 final {
    新_模拟世界类* owner=nullptr;
    const std::thread::id thread=std::this_thread::get_id();
    基础节点组追加请求 request;
    std::unique_ptr<新词典结果参与准备> participant;
    std::optional<基础片段发布回执> receipt;
    新本能词典读取回执 result;
    std::optional<std::size_t> stateSlot;
    std::size_t ownFields=0,ownEdges=0;
    std::int64_t time=0;
    bool submitted=false,completed=false,cancelled=false,featureChanged=false;
    稳定编码 call;
    ~实现() {
        if(receipt&&!completed)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"隔离词典采用准备析构",L"已发表采用尚未完成收尾。");
    }
};
新模拟词典采用准备::新模拟词典采用准备(std::unique_ptr<实现> p) noexcept:实现_(std::move(p)){}
新模拟词典采用准备::~新模拟词典采用准备()=default;
新模拟词典采用准备::新模拟词典采用准备(新模拟词典采用准备&&) noexcept=default;
新模拟词典采用准备& 新模拟词典采用准备::operator=(新模拟词典采用准备&&) noexcept=default;
新模拟词典采用准备结果 新_模拟世界类::准备调用词典采用(稳定编码 N) noexcept {
    using S=新词典材料操作状态;
    try {
        auto read=读取调用词典回执(N);
        if(!read.回执)return {read.状态,{},{}};
        if(read.回执->采用阶段==新词典材料采用阶段::已采用||read.回执->查阅结局==新词典查阅结局::正式未命中)
            return {S::已复用,{},read.回执};
        const auto Q=read.回执->材料Q;
        if(!Q)throw std::logic_error("local unadopted receipt has no Q");
        const auto material=读取本地词典材料(*Q);
        if(material.状态==S::资源失败)return {S::资源失败,{},{}};
        if(!material.信息)throw std::logic_error("held local dictionary material unavailable");
        const auto F=read.回执->材料特征,W=read.回执->词条;
        // FT/FC只读准备在进入world锁之前，避免反向嵌套领域锁。
        const auto feature=读取模拟特征(F);
        if(!feature)throw std::logic_error("local dictionary feature unavailable");
        auto p=std::make_unique<新模拟词典采用准备::实现>();p->owner=this;p->call=N;p->result=*read.回执;
        auto& s=*实现_;std::unique_lock lock(s.互斥);
        if(!s.样例就绪||!s.记录写回||s.当前调用!=std::optional{N}||s.请求取消)return {S::条件不具备,{},{}};
        const auto fSnapshot=本地词典快照(F);
        if(!fSnapshot)throw std::logic_error("local dictionary feature snapshot missing");
        std::vector<基础联合节点快照> consumed{*fSnapshot};
        const auto* current=本地词典字段(fSnapshot->结构,当前材料);
        const auto old=本地词典引用(fSnapshot->结构,当前材料);
        if(current&&!old)throw std::logic_error("local dictionary current is not Q reference");
        const bool changed=old!=Q;
        std::optional<稳定编码> previous;
        std::int64_t last=-1;
        if(changed&&old)for(const auto n:s.通用节点) {
            if(读值(n,类型)!=状态种类||读引用(n,状态特征引用)!=std::optional{F}
                ||读引用(n,所属)!=std::optional{W}||读引用(n,当前材料)!=old)continue;
            const auto time=必读数(n,步骤);
            if(time<0||time>s.模拟步骤)throw std::logic_error("local dictionary previous time invalid");
            if(time==last&&previous!=std::optional{n})throw std::logic_error("local dictionary previous state ambiguous");
            if(time>last){previous=n;last=time;}
        }

        auto& q=p->request;
        const auto add=[&](节点种类 kind) {
            const auto n=q.新节点组.size();q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(kind)}},{字段组[根],模拟世界树根节点}}});
            q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{n},基础外部关系类型::父子});return n;
        };
        const auto put=[&](std::size_t n,字段 key,基础组内字段内容 v){q.新节点组[n].字段组.push_back({字段组[key],std::move(v)});};
        if(changed) {
            if(current)q.替换字段组.push_back({current->编码,*Q});
            else q.追加字段组.push_back({F,字段组[当前材料],*Q});
        }
        std::map<稳定编码,std::pair<基础字段关系,std::int64_t>> counts;
        for(const auto& member:fSnapshot->结构.字段组)if(member.字段节点==字段组[历史命中引用]) {
            const auto* node=std::get_if<稳定编码>(&member.内容);
            if(!node)throw std::logic_error("local dictionary history member malformed");
            const auto snapshot=本地词典快照(*node);
            if(!snapshot)throw std::logic_error("local dictionary history missing");
            const auto value=本地词典引用(snapshot->结构,结构引用);
            const auto* count=本地词典字段(snapshot->结构,命中次数值);
            const auto number=本地词典整数(snapshot->结构,命中次数值);
            if(!value||!count||!number||*number<0||!counts.emplace(*value,std::pair{*count,*number}).second)
                throw std::logic_error("local dictionary history corrupt");
            consumed.push_back(*snapshot);
        }
        std::int64_t newCount=1;
        if(const auto found=counts.find(*Q);found!=counts.end()) {
            newCount=found->second.second;
            if(newCount!=(std::numeric_limits<std::int64_t>::max)())++newCount;
            if(newCount!=found->second.second)q.替换字段组.push_back({found->second.first.编码,基础值{newCount}});
        } else {
            const auto h=add(语义快照种类);put(h,结构引用,*Q);put(h,命中次数值,基础值{newCount});
            q.追加字段组.push_back({F,字段组[历史命中引用],基础组内节点引用{h}});
        }
        const auto* standard=本地词典字段(fSnapshot->结构,标准值引用);
        const auto standardQ=本地词典引用(fSnapshot->结构,标准值引用);
        if(standard&&!standardQ)throw std::logic_error("local dictionary standard is not Q reference");
        if(!standard)q.追加字段组.push_back({F,字段组[标准值引用],*Q});
        else if(standardQ!=Q) {
            const auto found=counts.find(*standardQ);
            if(found==counts.end())throw std::logic_error("local dictionary standard absent from history");
            if(newCount>found->second.second)q.替换字段组.push_back({standard->编码,*Q});
        }
        p->time=s.模拟步骤;
        if(changed) {
            if(s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())throw std::overflow_error("local dictionary state time exhausted");
            p->time=s.模拟步骤+1;p->stateSlot=add(状态种类);const auto st=*p->stateSlot;
            put(st,所属,W);put(st,状态特征引用,F);put(st,特征类型,feature->特征概念节点);put(st,当前材料,*Q);
            put(st,步骤,基础值{p->time});put(st,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});
        }
        p->featureChanged=!q.新节点组.empty()||!q.追加字段组.empty()||!q.替换字段组.empty();
        // 世界部分和回执部分各自只追踪一次，所有引用都由同一个组合提交公开。
        p->ownFields=q.追加字段组.size();p->ownEdges=q.追加外部关系组.size();
        // 锁内一次捕获F/历史/前S以及N，放锁后才调用材料owner与N参与提供者。
        补齐隔离追加联合预期(q);
        const auto callSnapshot=本地词典快照(N);
        if(!callSnapshot)throw std::logic_error("local call disappeared during adoption");
        q.预期联合节点组.push_back(*callSnapshot);
        q.预期联合节点组.insert(q.预期联合节点组.end(),consumed.begin(),consumed.end());
        (void)s.准备写回槽(F);
        lock.unlock();
        auto participant=准备调用词典采用参与(N,*Q,previous);
        if(!participant.准备)return {participant.状态,{},participant.已有回执};
        p->participant=std::move(participant.准备);
        if(changed) {
            新同组状态端点 endpoint{基础组内节点引用{*p->stateSlot},W,F,feature->特征概念节点,
                新特征准确值{*Q},{新状态时间语义::抽象相对,p->time},q.新节点组.size()};
            auto result=p->participant->构造同组状态引用(endpoint);合并同组结果构造(q,std::move(result.追加请求));
        } else {auto result=p->participant->构造无新状态采用();合并同组结果构造(q,std::move(result.追加请求));}
        // 保留参与者捕获的N/记录预期；新的端点快照由L1合并并裁定零发表。
        auto premise=std::move(q.预期联合节点组);q.预期联合节点组.clear();
        补齐隔离追加联合预期(q);
        q.预期联合节点组.insert(q.预期联合节点组.end(),premise.begin(),premise.end());
        lock.lock();s.预留追加(q);lock.unlock();
        p->result.采用阶段=新词典材料采用阶段::已采用;p->result.本次形成新状态=changed;
        p->result.前状态=changed?previous:std::nullopt;
        p->result.变化阶段=changed&&previous?新词典变化处理阶段::待处理:新词典变化处理阶段::无需形成;
        if(!changed||!previous)p->result.无需形成原因=changed?新词条动态记录原因::无前态:新词条动态记录原因::无值变化;
        return {S::已完成,std::unique_ptr<新模拟词典采用准备>(new 新模拟词典采用准备(std::move(p))),{}};
    }catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"准备隔离词典采用",L"本地F/S/N采用准备失败。");}
}
新词典材料提交结果 新_模拟世界类::提交调用词典采用(新模拟词典采用准备& preparation) noexcept {
    try {
        auto& p=*preparation.实现_;
        本地词典要求(p.owner==this&&p.thread==std::this_thread::get_id()&&!p.submitted&&!p.cancelled,"local dictionary adoption preparation consumed");
        p.submitted=true;
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.样例就绪||!s.记录写回||s.请求取消||s.当前调用!=std::optional{p.call})
            return {基础节点组追加状态::入口拒绝,{}}; // 技术零发表；运行编排按当前停止/资格处理。
        s.预留追加(p.request);
        const auto sealed=全局基础数据集.封存原子追加片段(p.request);
        if(!sealed.片段)return {sealed.状态,{}};
        const auto published=全局基础数据集.原子追加封存片段组({*sealed.片段});
        if(published.状态!=基础节点组追加状态::已完成)return {published.状态,{}};
        if(published.片段回执组.size()!=1)throw std::logic_error("local dictionary adoption receipt missing");
        p.receipt=published.片段回执组.front();
        p.participant->登记共同发表回执(*p.receipt);
        const auto& r=*p.receipt;
        s.通用节点.insert(s.通用节点.end(),r.新节点组().begin(),r.新节点组().end());
        for(const auto& f:r.新节点初始字段关系组())s.通用字段.insert(s.通用字段.end(),f.begin(),f.end());
        s.通用字段.insert(s.通用字段.end(),r.追加字段关系组().begin(),r.追加字段关系组().begin()+p.ownFields);
        s.通用外部边.insert(s.通用外部边.end(),r.追加外部关系组().begin(),r.追加外部关系组().begin()+p.ownEdges);
        if(p.stateSlot)p.result.后状态=r.新节点组()[*p.stateSlot];
        if(p.stateSlot)s.模拟步骤=p.time;
        if(p.featureChanged)if(auto* slot=s.准备写回槽(p.result.材料特征))*slot=实现::写入来源::方法;
        return {基础节点组追加状态::已完成,p.receipt};
    }catch(const std::bad_alloc&){return {基础节点组追加状态::资源失败,{}};}
    catch(const std::length_error&){return {基础节点组追加状态::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"提交隔离词典采用",L"采用提交异常，保留已发表事实。");}
}
新词典执行回执读取结果 新_模拟世界类::完成调用词典采用(新模拟词典采用准备& preparation) noexcept {
    auto& p=*preparation.实现_;
    if(p.owner!=this||p.thread!=std::this_thread::get_id()||!p.receipt||p.cancelled)
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"完成隔离词典采用",L"没有本次成功回执。");
    if(!p.completed){p.participant->完成共同发表();p.completed=true;}
    return {新词典材料操作状态::已完成,p.result};
}
void 新_模拟世界类::取消调用词典采用准备(新模拟词典采用准备& preparation) noexcept {
    auto& p=*preparation.实现_;
    if(p.owner!=this||p.thread!=std::this_thread::get_id()||p.receipt)
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"取消隔离词典采用",L"不能取消已发表采用。");
    if(!p.cancelled){p.participant->取消未发表准备();p.cancelled=true;}
}
新词典执行回执读取结果 新_模拟世界类::续接调用词典变化(稳定编码 N) noexcept {
    using S=新词典材料操作状态;
    try {
        if(!有效(N))return {S::输入拒绝,{}};
        if(!实现_)return {S::未装配,{}};
        auto& s=*实现_;
        for(;;) {
            std::unique_lock lock(s.互斥);
            if(!s.通用||std::find(s.通用节点.begin(),s.通用节点.end(),N)==s.通用节点.end())
                return {S::未找到,{}};
            const auto read=读取本地词典调用(N);
            if(!read||!read->回执)return {S::条件不具备,{}};
            auto result=*read->回执;
            if(result.查阅结局==新词典查阅结局::正式未命中)return {S::已完成,result};
            if(result.采用阶段!=新词典材料采用阶段::已采用)return {S::条件不具备,result};
            if(result.变化阶段!=新词典变化处理阶段::待处理)return {S::已完成,result};
            // 停止不撤销采用；本次N仍开放时只完成其已发生事实的后继。
            if(!s.记录写回||s.当前调用!=std::optional{N})return {S::条件不具备,result};
            本地词典要求(read->记录&&result.后状态&&result.材料Q,
                "local dictionary pending change has no adopted state");
            const auto after=读取本地状态(*result.后状态);
            本地词典要求(after&&after->被描述存在节点==result.词条
                &&after->来源实例特征节点==result.材料特征
                &&after->固定准确值==std::optional<新特征准确值>{新特征准确值{*result.材料Q}},
                "local dictionary after-state differs from adopted W/F/Q");
            std::optional<新词条动态记录原因> reason;
            std::optional<稳定编码> delta;
            if(!result.前状态)reason=新词条动态记录原因::无前态;
            else {
                const auto before=读取本地状态(*result.前状态);
                本地词典要求(before&&before->被描述存在节点==result.词条
                    &&before->来源实例特征节点==result.材料特征&&before->固定准确值,
                    "local dictionary before-state differs from W/F");
                本地词典要求(before->固定准确值!=after->固定准确值,
                    "local dictionary two-state change contains equal values");
                if(after->强时间.纳秒<=before->强时间.纳秒)reason=新词条动态记录原因::时间不递增;
                else for(const auto n:s.通用节点) {
                    if(读值(n,类型)!=动态种类||读引用(n,所属)!=std::optional{result.词条}
                        ||读引用(n,前状态引用)!=result.前状态||读引用(n,后状态引用)!=result.后状态)continue;
                    本地词典要求(!delta,"local dictionary state pair has duplicate changes");
                    本地词典要求(读取本地变化(n).has_value(),"local dictionary existing change unreadable");
                    delta=n;
                }
            }
            基础节点组追加请求 q;
            const auto record=read->记录->结构.节点.编码;
            const auto* stage=本地词典字段(read->记录->结构,词典变化阶段);
            本地词典要求(stage!=nullptr,"local dictionary change stage missing");
            std::optional<std::size_t> deltaSlot;
            if(reason) {
                result.变化阶段=新词典变化处理阶段::无需形成;result.无需形成原因=reason;
                q.追加字段组.push_back({record,字段组[词典无需原因],基础值{static_cast<std::int64_t>(*reason)}});
            } else {
                基础组内字段内容 endpoint;
                if(delta)endpoint=*delta;
                else {
                    deltaSlot=q.新节点组.size();endpoint=基础组内节点引用{*deltaSlot};
                    q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(动态种类)}},
                        {字段组[根],模拟世界树根节点},{字段组[所属],result.词条},
                        {字段组[状态特征引用],result.材料特征},{字段组[前状态引用],*result.前状态},
                        {字段组[后状态引用],*result.后状态},{字段组[来源引用],N}}});
                    q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{*deltaSlot},基础外部关系类型::父子});
                }
                result.变化阶段=新词典变化处理阶段::已形成;
                q.追加字段组.push_back({record,字段组[词典变化动态],endpoint});
                const auto action=读取本地动作(result.动作);
                本地词典要求(action&&action->动作时间&&action->动作目标词条节点==std::optional{result.词条}
                    &&action->动作目标特征节点==std::optional{result.材料特征},
                    "local dictionary action differs from W/F");
                // 读取动作不冒充造成变化；时间不成立时Δ保留为独立事实。
                if(after->强时间.纳秒<=action->动作时间->纳秒) {
                    bool linked=false;
                    for(const auto& item:action->动作结果组)if(delta&&item.结果动态节点==*delta) {
                        本地词典要求(item.类型==新动作结果关系类型::观察取得,"local dictionary action has causal result");linked=true;
                    }
                    if(!linked)q.追加字段组.push_back({result.动作,字段组[动作观察动态引用],endpoint});
                }
            }
            q.替换字段组.push_back({stage->编码,基础值{static_cast<std::int64_t>(*result.变化阶段)}});
            s.追加进展(q);
            补齐隔离追加联合预期(q);
            // 消费过的旧材料只交L1判定；并发变化不在上层抛错，也不重放已发表Δ。
            q.预期联合节点组.push_back(read->N);q.预期联合节点组.push_back(*read->记录);
            s.预留追加(q);
            const auto made=全局基础数据集.原子追加节点组(q);
            if(made.状态==基础节点组追加状态::预期不符){lock.unlock();std::this_thread::yield();continue;}
            if(made.状态==基础节点组追加状态::资源失败)return {S::资源失败,{}};
            本地词典要求(made.状态==基础节点组追加状态::已完成&&made.新节点组.size()==q.新节点组.size(),
                "local dictionary change publication rejected");
            s.收集追加(made);
            if(deltaSlot)delta=made.新节点组[*deltaSlot];
            result.变化动态=delta;
            return {S::已完成,result};
        }
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::length_error&){return {S::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"续接隔离词典变化",L"本地变化续接失败，保留已发表事实。");}
}

struct 新_模拟世界类::词典接收提供者 final : 新词典接收场景提供接口 {
    新_模拟世界类& world;
    explicit 词典接收提供者(新_模拟世界类& w):world(w){}
    新词典接收准备结果 准备接收(稳定编码 C,const std::vector<稳定编码>& M) noexcept override {
        return world.准备本地词典接收(C,M);
    }
    新词典接收提交结果 续接变化(稳定编码 C,新词典处理阶段参与接口& p) noexcept override {
        return world.续接本地词典接收(C,p);
    }
};
struct 新_模拟世界类::词典接收准备 final : 新词典接收参与准备 {
    新_模拟世界类& world;
    新词典接收准备投影 projection;
    基础联合节点快照 feature,scene;
    std::vector<稳定编码> messages;
    新消息集合保持材料 retention;
    bool used=false;
    词典接收准备(新_模拟世界类& w,新词典接收准备投影 v,
        基础联合节点快照 f,基础联合节点快照 c,std::vector<稳定编码> m,新消息集合保持材料 r)
        :world(w),projection(v),feature(std::move(f)),scene(std::move(c)),messages(std::move(m)),retention(std::move(r)){}
    新词典接收准备投影 读取接收投影() const override {return projection;}
    新词典接收提交结果 提交接收(新消息待发表凭据&& p,新状态结果引用参与接口& b) noexcept override {
        return world.提交本地词典接收(*this,std::move(p),b);
    }
    void 取消未发表准备() noexcept override {used=true;}
};
void 新_模拟世界类::配置词典接收(外设词典记录类& records,新_特征值类& values,
    概念_特征类& concepts,稳定编码 FT) {
    if(!实现_||!有效(FT))throw std::invalid_argument("local dictionary receive configuration missing");
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(s.通用||(s.词典记录&&s.词典记录!=&records)||(s.词典值&&s.词典值!=&values)
        ||(s.词典保持&&s.词典保持!=&concepts)||(有效(s.词典接收FT)&&s.词典接收FT!=FT))
        throw std::logic_error("local dictionary receive configuration conflict");
    s.词典记录=&records;s.词典值=&values;s.词典保持=&concepts;s.词典接收FT=FT;
}
void 新_模拟世界类::建立本地词典接收场景() {
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.词典记录)return;
    if(s.词典接收定位)return;
    using K=新方法参数信息类型;
    const std::array<K,6> kinds{K::词条,K::特征,K::存在,K::存在,K::特征,K::特征};
    if(!s.通用||s.通用绑定.size()!=kinds.size()||s.通用输入节点.size()<kinds.size())
        throw std::logic_error("dictionary receive sample is not the declared six-parameter method");
    for(std::size_t i=0;i<kinds.size();++i) {
        const auto& b=s.通用绑定[i];
        if(!b.有序引用||b.有序引用->信息节点!=s.通用输入节点[i]||!s.是本地参数(s.通用输入节点[i],kinds[i]))
            throw std::logic_error("dictionary receive parameter role differs from declaration");
    }
    if(读引用(s.通用输入节点[1],所属)!=std::optional{s.通用输入节点[0]}
        ||读引用(s.通用输入节点[4],所属)!=std::optional{s.通用输入节点[2]}
        ||读引用(s.通用输入节点[5],所属)!=std::optional{s.通用输入节点[2]})
        throw std::logic_error("dictionary receive parameter ownership differs");
    for(;;) {
        基础节点组追加请求 q;
        const auto node=[&](节点种类 k){q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(k)}},{字段组[根],模拟世界树根节点}}});};
        node(场景种类);node(存在种类);node(特征种类);
        q.新节点组[0].字段组.push_back({字段组[所属],s.通用场景});
        q.新节点组[0].字段组.push_back({字段组[场景成员引用],基础组内节点引用{1}});
        q.新节点组[0].字段组.push_back({字段组[场景特征引用],基础组内节点引用{2}});
        q.新节点组[1].字段组.push_back({字段组[所属],基础组内节点引用{0}});
        q.新节点组[2].字段组.push_back({字段组[所属],基础组内节点引用{0}});
        q.新节点组[2].字段组.push_back({字段组[特征类型],s.词典接收FT});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
        q.追加外部关系组.push_back({基础组内节点引用{0},基础组内节点引用{1},基础外部关系类型::父子});
        q.追加外部关系组.push_back({基础组内节点引用{0},基础组内节点引用{2},基础外部关系类型::父子});
        补齐隔离追加联合预期(q);s.预留追加(q);
        const auto made=全局基础数据集.原子追加节点组(q);
        if(made.状态==基础节点组追加状态::预期不符)continue;
        if(made.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
        本地词典要求(made.状态==基础节点组追加状态::已完成&&made.新节点组.size()==3,"local dictionary receive scene publication failed");
        s.收集追加(made);s.词典接收定位=新模拟词典接收定位{made.新节点组[0],made.新节点组[1],made.新节点组[2]};return;
    }
}
std::optional<新模拟词典接收定位> 新_模拟世界类::读取本地词典接收定位() const noexcept {
    if(!实现_)return {};std::lock_guard lock(实现_->互斥);
    if(!实现_->通用)return {};return 实现_->词典接收定位;
}
新词典记录操作状态 新_模拟世界类::读取本样例外设场景(稳定编码 C) const noexcept {
    using S=新词典记录操作状态;
    try {
        if(!实现_)return S::未装配;auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||!s.词典接收定位||s.词典接收定位->场景!=C)return S::未找到;
        const auto n=本地词典快照(C);
        本地词典要求(n&&本地词典整数(n->结构,类型)==场景种类
            &&本地词典引用(n->结构,所属)==std::optional{s.通用场景},"owned dictionary scene missing or corrupt");
        return S::已完成;
    }catch(const std::bad_alloc&){return S::资源失败;}
    catch(const std::length_error&){return S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取隔离词典退出场景",L"本样例场景结构损坏。");}
}
新词典查询发表结果 新_模拟世界类::发表本地词典查询(const 新词典查询事实& facts) noexcept {
    using S=新词典记录操作状态;
    try {
        if(!实现_)return {S::未装配,{}};auto& s=*实现_;
        {
            std::lock_guard lock(s.互斥);
            if(!s.词典记录||!s.词典接收定位)return {S::未装配,{}};
            if(!s.样例就绪||s.请求取消||!s.记录写回)return {S::未找到,{}};
            if(s.词典在途)return {S::尚需续接,{}};
            const auto& loc=*s.词典接收定位;
            if(facts.定位.外设场景!=loc.场景||facts.词典说话者!=loc.说话者||!s.当前调用
                ||读引用(*s.当前调用,调用主动作引用)!=std::optional{稳定编码{facts.定位.调用来源标识}}
                ||facts.查询完成时间.语义!=新状态时间语义::抽象相对)return {S::输入拒绝,{}};
            if(std::none_of(s.词典查询责任.begin(),s.词典查询责任.end(),[&](const auto& k){return k.外设场景==facts.定位.外设场景&&k.调用来源标识==facts.定位.调用来源标识;}))
                s.词典查询责任.push_back(facts.定位); // 任何R发表前接管责任，包含未命中。
            ++s.词典在途;
        }
        struct End {实现& s;~End(){std::lock_guard lock(s.互斥);--s.词典在途;}} end{s};
        auto provider=std::make_shared<词典接收提供者>(*this);
        auto prepared=s.词典记录->准备查询记录(facts,provider);
        if(prepared.既有记录)return {新词典记录操作状态::已复用,std::move(prepared.既有记录)};
        if(!prepared.准备)return {prepared.状态,{}};
        auto result=s.词典记录->提交查询接收(*prepared.准备);
        s.词典记录->取消未发表查询(*prepared.准备); // 已发表只释放在途资格，不撤销事实。
        return result;
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::length_error&){return {S::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"发表隔离词典查询",L"查询工程异常，保留按来源登记的退出责任。");}
}
新词典查询读取结果 新_模拟世界类::续接本地词典查询(稳定编码 R) noexcept {
    using S=新词典记录操作状态;
    try {
        if(!实现_)return {S::未装配,{}};auto& s=*实现_;
        {std::lock_guard lock(s.互斥);if(!s.通用||s.请求取消||!s.词典记录)return {S::未找到,{}};if(s.词典在途)return {S::尚需续接,{}};++s.词典在途;}
        struct End {实现& s;~End(){std::lock_guard lock(s.互斥);--s.词典在途;}} end{s};
        auto current=s.词典记录->读取查询记录(R);if(!current.信息)return current;
        {std::lock_guard lock(s.互斥);
            const auto& where=current.信息->定位;
            if(std::none_of(s.词典查询责任.begin(),s.词典查询责任.end(),[&](const auto& k){return k.外设场景==where.外设场景&&k.调用来源标识==where.调用来源标识;}))
                return {S::输入拒绝,{}};
        }
        词典接收提供者 provider{*this};return s.词典记录->续接查询变化(R,provider);
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::length_error&){return {S::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"续接隔离词典接收",L"本地接收续接异常，保留实际阶段。");}
}
新词典接收准备结果 新_模拟世界类::准备本地词典接收(稳定编码 C,const std::vector<稳定编码>& M) noexcept {
    using S=新词典记录操作状态;
    try {
        auto& s=*实现_;新词典接收准备投影 v;基础联合节点快照 f,c;
        {
            std::lock_guard lock(s.互斥);
            if(!s.词典接收定位||s.词典接收定位->场景!=C||!s.通用)return {S::未找到,{}};
            if(M.empty())return {S::输入拒绝,{}};
            v.接收特征=s.词典接收定位->接收特征;
            auto cf=本地词典快照(v.接收特征),cc=本地词典快照(C);
            本地词典要求(cf&&cc,"local receive feature or scene missing");f=std::move(*cf);c=std::move(*cc);
            std::int64_t latest=-1;const auto old=本地词典引用(f.结构,当前材料);
            if(old)for(const auto n:s.通用节点) {
                const auto st=读取本地状态(n);if(!st||st->被描述存在节点!=C||st->来源实例特征节点!=v.接收特征
                    ||st->固定准确值!=std::optional<新特征准确值>{新特征准确值{*old}})continue;
                本地词典要求(st->强时间.纳秒!=latest,"ambiguous local receive previous state");
                if(st->强时间.纳秒>latest){latest=st->强时间.纳秒;v.前状态=n;}
            }
            if(s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())throw std::overflow_error("local receive time exhausted");
            v.本轮状态时间={新状态时间语义::抽象相对,s.模拟步骤+1};
        }
        std::vector<std::uint64_t> members;
        if(const auto old=本地词典引用(f.结构,当前材料)) {
            const auto x=s.词典值->获取特征值(*old);
            本地词典要求(x&&x->物理类型==材料物理类型::U64数组,"local receive material missing");
            members=std::get<std::vector<std::uint64_t>>(x->材料);
        }
        for(const auto m:M){if(!有效(m))return {S::输入拒绝,{}};members.push_back(m.值);}
        std::sort(members.begin(),members.end());members.erase(std::unique(members.begin(),members.end()),members.end());
        v.新消息集合值=s.词典值->保存或取得特征值(材料物理类型::U64数组,members);
        if(!有效(v.新消息集合值))return {S::资源失败,{}};
        auto held=s.词典保持->读取消息集合保持材料(v.新消息集合值,*s.词典值);
        if(held.状态==特征概念规则状态::资源失败)return {S::资源失败,{}};
        本地词典要求(held.状态==特征概念规则状态::已完成&&held.材料.has_value(),"local message retention preparation invalid");
        return {S::已完成,std::make_unique<词典接收准备>(*this,v,std::move(f),std::move(c),M,std::move(*held.材料))};
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::length_error&){return {S::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"准备隔离词典接收",L"本地接收材料结构异常。");}
}
新词典接收提交结果 新_模拟世界类::提交本地词典接收(词典接收准备& p,
    新消息待发表凭据&& credential,新状态结果引用参与接口& participant) noexcept {
    using L=基础节点组追加状态;
    try {
        本地词典要求(!p.used&&!credential.已采用&&credential.消息节点组==p.messages,"local receive preparation consumed or foreign");p.used=true;
        auto& s=*实现_;
        auto holding=s.词典保持->构造消息集合成员保持(std::move(p.retention),credential);
        credential.已采用=true;
        const auto hs=全局基础数据集.封存原子追加片段(holding);if(!hs.片段)return {hs.状态,{},{}};
        新特征状态参与材料 state;auto& q=state.请求_;
        const auto C=p.scene.结构.节点.编码,F=p.projection.接收特征,V=p.projection.新消息集合值;
        const auto* old=本地词典字段(p.feature.结构,当前材料);
        本地词典要求(本地词典引用(p.feature.结构,当前材料)!=std::optional{V},"fresh dictionary batch did not change receive collection");
        if(old)q.替换字段组.push_back({old->编码,V});else q.追加字段组.push_back({F,字段组[当前材料],V});
        q.追加字段组.push_back({F,字段组[历史引用],V});
        const auto* standard=本地词典字段(p.feature.结构,标准值引用);
        if(!standard)q.追加字段组.push_back({F,字段组[标准值引用],V}); // 每次累积新V命中1；平手不换标准。
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(状态种类)}},
            {字段组[根],模拟世界树根节点},{字段组[所属],C},{字段组[状态特征引用],F},
            {字段组[特征类型],s.词典接收FT},{字段组[当前材料],V},
            {字段组[时间语义],基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)}},
            {字段组[步骤],基础值{p.projection.本轮状态时间.纳秒}}}});
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(语义快照种类)}},
            {字段组[根],模拟世界树根节点},{字段组[结构引用],V},{字段组[命中次数值],基础值{std::int64_t{1}}}}});
        q.追加字段组.push_back({F,字段组[历史命中引用],基础组内节点引用{1}});
        q.追加字段组.push_back({C,字段组[接收状态引用],基础组内节点引用{0}});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{1},基础外部关系类型::父子});
        const auto nodes=q.新节点组.size(),fields=q.追加字段组.size(),edges=q.追加外部关系组.size();
        新同组状态端点 endpoint{基础组内节点引用{0},C,F,s.词典接收FT,新特征准确值{V},p.projection.本轮状态时间,nodes};
        auto result=participant.构造同组状态引用(endpoint);合并同组结果构造(q,std::move(result.追加请求));
        auto prior=std::move(q.预期联合节点组);q.预期联合节点组.clear();补齐隔离追加联合预期(q);
        q.预期联合节点组.insert(q.预期联合节点组.end(),prior.begin(),prior.end());
        q.预期联合节点组.push_back(p.feature);q.预期联合节点组.push_back(p.scene);
        const auto sealed=全局基础数据集.封存原子追加片段(q);if(!sealed.片段)return {sealed.状态,{},{}};
        std::lock_guard lock(s.互斥);s.预留追加(q);s.词典接收值组.reserve(s.词典接收值组.size()+1);
        auto made=全局基础数据集.原子追加封存片段组({credential.身份及来源片段,*sealed.片段,*hs.片段});
        if(made.状态!=L::已完成)return {made.状态,{},{}};
        const auto& receipt=made.片段回执组.at(1);
        participant.登记共同发表回执(receipt); // 首先确认R已发表；其后只有已预留容器的赋值。
        s.通用节点.insert(s.通用节点.end(),receipt.新节点组().begin(),receipt.新节点组().begin()+nodes);
        s.通用字段.insert(s.通用字段.end(),receipt.追加字段关系组().begin(),receipt.追加字段关系组().begin()+fields);
        for(std::size_t i=0;i<nodes;++i){const auto& fs=receipt.新节点初始字段关系组()[i];s.通用字段.insert(s.通用字段.end(),fs.begin(),fs.end());}
        s.通用外部边.insert(s.通用外部边.end(),receipt.追加外部关系组().begin(),receipt.追加外部关系组().begin()+edges);
        s.词典接收值组.push_back(V);s.模拟步骤=(std::max)(s.模拟步骤,p.projection.本轮状态时间.纳秒);
        const auto S=receipt.新节点组()[0];return {L::已完成,std::move(made.片段回执组),S};
    }catch(const std::bad_alloc&){return {L::资源失败,{},{}};}
    catch(const std::length_error&){return {L::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"提交隔离词典接收",L"本地接收共同发表异常，保留已发表事实。");}
}
新词典接收提交结果 新_模拟世界类::续接本地词典接收(稳定编码 C,新词典处理阶段参与接口& participant) noexcept {
    using L=基础节点组追加状态;
    try {
        auto& s=*实现_;const auto b=participant.读取本轮处理材料();std::unique_lock lock(s.互斥);
        本地词典要求(s.词典接收定位&&s.词典接收定位->场景==C&&b.后状态.has_value(),"local receive continuation outside sample");
        const auto after=读取本地状态(*b.后状态);
        本地词典要求(after&&after->被描述存在节点==C&&after->来源实例特征节点==s.词典接收定位->接收特征,"local receive after-state invalid");
        std::optional<基础封存追加片段> record;
        if(b.阶段==新词典接收处理阶段::接收已发表) {
            std::optional<新词条动态记录原因> reason;
            if(!b.前状态)reason=新词条动态记录原因::无前态;
            else {
                const auto before=读取本地状态(*b.前状态);
                本地词典要求(before&&before->被描述存在节点==C&&before->来源实例特征节点==after->来源实例特征节点
                    &&before->固定准确值!=after->固定准确值,"local receive state pair invalid");
                if(after->强时间.纳秒<=before->强时间.纳秒)reason=新词条动态记录原因::时间不递增;
            }
            if(reason)record=participant.构造处理收束(新词典接收处理阶段::无需动态,reason);
            else {
                std::optional<稳定编码> delta;
                for(const auto n:s.通用节点)if(读值(n,类型)==动态种类&&读引用(n,所属)==std::optional{C}
                    &&读引用(n,前状态引用)==b.前状态&&读引用(n,后状态引用)==b.后状态) {
                    本地词典要求(!delta,"duplicate local receive change");delta=n;
                }
                if(!delta) {
                    基础节点组追加请求 q;
                    q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(动态种类)}},
                        {字段组[根],模拟世界树根节点},{字段组[所属],C},{字段组[状态特征引用],after->来源实例特征节点},
                        {字段组[前状态引用],*b.前状态},{字段组[后状态引用],*b.后状态}}});
                    q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{0},基础外部关系类型::父子});
                    补齐隔离追加联合预期(q);s.预留追加(q);
                    const auto made=全局基础数据集.原子追加节点组(q);if(made.状态!=L::已完成)return {made.状态,{},{}};
                    s.收集追加(made);delta=made.新节点组[0];
                }
                record=participant.构造动态已建立(*delta);
            }
            auto made=全局基础数据集.原子追加封存片段组({*record});
            if(made.状态==L::已完成)participant.登记共同发表回执(made.片段回执组.front());
            return {made.状态,std::move(made.片段回执组),{}};
        }
        本地词典要求(b.阶段==新词典接收处理阶段::动态已建立&&b.动态&&读取本地变化(*b.动态).has_value(),"local receive change stage invalid");
        基础节点组追加请求 q;
        const auto linked=全局基础数据集.查询字段(C,字段组[动态引用]);
        if(std::none_of(linked.begin(),linked.end(),[&](const auto& f){const auto* d=std::get_if<稳定编码>(&f.内容);return d&&*d==*b.动态;}))
            q.追加字段组.push_back({C,字段组[动态引用],*b.动态});
        补齐隔离追加联合预期(q);s.预留追加(q);
        const auto own=全局基础数据集.封存原子追加片段(q);if(!own.片段)return {own.状态,{},{}};
        record=participant.构造处理收束(新词典接收处理阶段::场景已挂接,{});
        auto made=全局基础数据集.原子追加封存片段组({*own.片段,*record});
        if(made.状态==L::已完成) {
            participant.登记共同发表回执(made.片段回执组[1]);
            const auto& fs=made.片段回执组[0].追加字段关系组();s.通用字段.insert(s.通用字段.end(),fs.begin(),fs.end());
        }
        return {made.状态,std::move(made.片段回执组),{}};
    }catch(新词典记录操作状态 status){
        if(status==新词典记录操作状态::资源失败)return {L::资源失败,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"续接隔离接收动态",L"记录参与者拒绝本次合法阶段。");
    }catch(const std::bad_alloc&){return {L::资源失败,{},{}};}
    catch(const std::length_error&){return {L::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"续接隔离接收动态",L"本地接收阶段异常，保留已发表事实。");}
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
稳定编码 新_模拟世界类::建立输出动作(const 方法参数场景&,const 新字段询问材料&) {
    // 方案乙与§6.6后，模拟世界无外设窗口对应，读取询问材料恒为“定位未支持”，不会进入动作。
    throw std::logic_error("simulation field question cannot reach output action");
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
新字段询问入列读回 新_模拟世界类::提交询问输出(const 方法参数场景&,
    const 新字段询问材料&,稳定编码,std::string_view) {
    throw std::logic_error("simulation field question cannot reach output submission");
}
新_模拟世界类::~新_模拟世界类() {
    try { 释放有序样例(); }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"释放模拟样例",L"临时模拟对象仍被引用或释放失败。"); }
}


稳定编码 新_模拟世界类::模拟根节点() const noexcept {

    if(实现_) { std::lock_guard lock(实现_->互斥);
        if(实现_->通用&&实现_->样例就绪) return 模拟世界树根节点; }

    return {};
}
std::optional<稳定编码> 新_模拟世界类::读取有序输入节点(std::uint32_t 位置) const noexcept {
    try {
        if(!实现_)return std::nullopt;
        std::lock_guard lock(实现_->互斥);
        if(!实现_->通用||!实现_->样例就绪||!位置||位置>实现_->通用输入节点.size())return std::nullopt;
        const auto n=实现_->通用输入节点[位置-1];
        if(!n.值||!全局基础数据集.查询节点(n))return std::nullopt;
        return n;
    } catch (...) { return std::nullopt; }
}
std::optional<std::vector<新方法参数角色绑定>> 新_模拟世界类::读取参数绑定() const noexcept {
    try {
        if(实现_) { std::lock_guard lock(实现_->互斥);
            if(实现_->通用&&实现_->样例就绪) {
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
    if(!实现_)return {};
    std::lock_guard lock(实现_->互斥);
    if(!实现_->样例就绪)return {};
    return 读取模拟特征内部(n,false);
}
新模拟局部特征概念读取结果 新_模拟世界类::读取模拟局部特征概念(稳定编码 C) const noexcept {
    using S=新模拟局部特征概念状态;
    try {
        if(!实现_)return {S::样例已关闭,{}};
        const auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用 || !s.样例就绪)return {S::样例已关闭,{}};
        if(std::find(s.通用节点.begin(),s.通用节点.end(),C)==s.通用节点.end())return {S::非本地,{}};
        const auto local=读取本地归类特征概念(C);
        if(!local || std::find(s.通用节点.begin(),s.通用节点.end(),local->类型根)!=s.通用节点.end())
            return {S::结构损坏,{}};
        return {S::已读取,新模拟局部特征概念身份{local->节点,local->类型根,local->父概念,local->覆盖正式概念}};
    } catch(const std::bad_alloc&) {return {S::资源失败,{}};}
    catch(const std::length_error&) {return {S::资源失败,{}};}
    catch(...) {return {S::结构损坏,{}};}
}
新方法词条参数读取结果 新_模拟世界类::读取模拟词条挂接(稳定编码 W) const noexcept {
    using S=新方法词条参数读取状态;
    try {
        if(!实现_)return {S::依赖未就绪,{}};
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||!s.样例就绪||!s.是本地参数(W,新方法参数信息类型::词条))return {S::参数已失效,{}};
        const auto scene=读引用(W,所属);
        if(!scene||!s.本地语言场景.contains(*scene)||!属于样例(W,词条种类,*scene))
            throw std::logic_error("local word identity or scene damaged");
        新词条特征信息 out;
        out.词条={W,必读引用(W,词条语言引用),必读引用(W,词条规范化合同引用),
            读结构材料<std::string>(W,词条词面值),{}};
        out.所属自然语言场景=*scene;
        std::set<稳定编码> types;
        for(const auto F:读引用组(W,词条特征引用)) {
            if(!s.是本地参数(F,新方法参数信息类型::特征)||读引用(F,所属)!=std::optional{W})
                throw std::logic_error("local word feature ownership damaged");
            const auto d=读取本地特征实际定义(F);
            if(!d||!types.insert(d->特征类型根).second)
                throw std::logic_error("local word feature type unavailable or ambiguous");
            out.特征组.push_back({d->特征类型根,F});
        }
        return {S::已读取,std::move(out)};
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::exception& e){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟词条",L"本地词条结构读取失败。",e.what());}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取模拟词条",L"未知工程异常。");}
}
std::optional<std::vector<新模拟普通名称信息>> 新_模拟世界类::读取模拟普通名称(稳定编码 W) const {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用 || !s.是本地参数(W,新方法参数信息类型::词条))return {};
    if(!读值(W,普通名称前态已知))return {};
    if(读值(W,普通名称前态已知)!=1)throw std::runtime_error("local ordinary name readiness corrupt");
    const auto scene=必读引用(W,所属);
    std::vector<新模拟普通名称信息> out;
    std::set<稳定编码> concepts;
    for(const auto N:读引用组(W,普通名称成员)) {
        if(!属于样例(N,模拟普通名称种类,scene)
            ||读引用(N,归类目标词条引用)!=std::optional{W})
            throw std::runtime_error("local ordinary name identity corrupt");
        const auto C=必读引用(N,归类采用概念引用);
        if(!concepts.insert(C).second)throw std::runtime_error("duplicate local ordinary name");
        out.push_back({C,N});
    }
    return out;
}
std::optional<std::vector<稳定编码>> 新_模拟世界类::读取模拟全部名称概念(稳定编码 W) const {
    if(!实现_)return {};
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用 || !s.是本地参数(W,新方法参数信息类型::词条))return {};
    const auto ready=读值(W,全部名称前态已知);
    if(!ready)return {};
    if(*ready!=1)throw std::runtime_error("local all-name readiness corrupt");
    auto concepts=读引用组(W,全部名称概念成员);
    std::sort(concepts.begin(),concepts.end());
    if(std::adjacent_find(concepts.begin(),concepts.end())!=concepts.end())
        throw std::runtime_error("duplicate local all-name concept");
    return concepts;
}
新方法参数特征读取结果 新_模拟世界类::读取模拟存在参数特征(
    std::uint32_t 位置,稳定编码 FT) const noexcept {
    using S=新方法参数特征读取状态;
    try {
        if(!实现_)throw std::logic_error("simulation instance feature owner unavailable");
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(!s.通用||!s.样例就绪||!位置||位置>s.通用绑定.size())return {S::参数未绑定,{},{}};
        const auto& b=s.通用绑定[位置-1];
        if(!b.有序引用)return {S::信息类型不符,{},{}};
        const bool 词条参数=b.有序引用->信息类型==新方法参数信息类型::词条;
        if(词条参数) {
            const auto word=读取模拟词条挂接(b.有序引用->信息节点);
            if(word.状态==新方法词条参数读取状态::资源失败)throw std::bad_alloc{};
            if(word.状态==新方法词条参数读取状态::参数已失效)return {S::参数未绑定,{},{}};
            if(word.状态!=新方法词条参数读取状态::已读取||!word.信息)
                throw std::logic_error("simulation word reader unavailable");
        } else if(b.有序引用->信息类型!=新方法参数信息类型::存在
            ||(!s.是普通存在参数(b.有序引用->信息节点)
                &&(!s.本地语言范围||s.本地语言范围->本地范围!=b.有序引用->信息节点)))
            return {S::信息类型不符,{},{}};
        const auto 声明=s.方法.读取存在参数特征声明(s.通用方法,s.通用执行分支,位置,FT);
        if(声明.状态!=S::已读取)return {声明.状态,{},{}};
        const auto E=b.有序引用->信息节点;
        std::map<稳定编码,新特征匹配定义快照> 定义组;
        std::vector<稳定编码> 命中;
        for(const auto& [F,p]:s.通用参数) {
            if(p.规格.信息类型!=新方法参数信息类型::特征||读引用(F,所属)!=std::optional{E})continue;
            const auto d=读取本地特征实际定义(F);
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
                    &&d.材料类型!=特征概念材料物理类型::需求节点引用
                    &&d.材料类型!=特征概念材料物理类型::信息节点引用) {
                    const auto* V=std::get_if<稳定编码>(&*out.特征->当前值);
                    if(!V)throw std::logic_error("simulation attached material identity malformed");
                    out.当前完整材料=读取模拟特征值材料(*V);
                    if(!out.当前完整材料)throw std::logic_error("simulation attached complete material missing");
                }
            }
        }
        if(!词条参数) {
            for(const auto& [F,d]:定义组)
                if(读取本地特征实际定义(F)!=std::optional{d})
                    return {S::定义前提已变化,{},{}};
            const auto 再声明=s.方法.读取存在参数特征声明(s.通用方法,s.通用执行分支,位置,FT);
            if(再声明.状态!=S::已读取||再声明.特征定义!=声明.特征定义)
                return {S::定义前提已变化,{},{}};
        }
        if(out.特征&&s.记录复制读取&&!s.本次已进入动作) {
                const auto F=out.特征->节点;
                auto r=std::find_if(s.本次存在特征读取.begin(),s.本次存在特征读取.end(),
                    [&](const auto& x){return x.有序引用==b.有序引用;});
                if(r==s.本次存在特征读取.end()) {
                    新本能函数条件角色材料 x;x.有序引用=b.有序引用;x.世界场景节点=b.世界场景节点;
                    s.本次存在特征读取.push_back(std::move(x));r=std::prev(s.本次存在特征读取.end());
                }
                if(词条参数) {
                    // 词条归类须保留每次实际读取，不能以不同值拒绝正常执行。
                    // 定向导出必须逐条消费；不允许把首条记录冒充完整读取过程。
                    r->实际特征组.push_back({F,out.特征->当前值});
                } else {
                    const auto old=std::find_if(r->实际特征组.begin(),r->实际特征组.end(),
                        [&](const auto& x){return x.特征节点==F;});
                    if(old!=r->实际特征组.end()&&old->动作前准确值!=out.特征->当前值)
                        return {S::定义前提已变化,{},{}};
                    if(old==r->实际特征组.end())r->实际特征组.push_back({F,out.特征->当前值});
                }
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
std::optional<新特征信息> 新_模拟世界类::读取模拟特征内部(稳定编码 n,bool 正式定位,
    std::optional<新特征匹配定义快照>* 实际定义,const 新特征匹配定义快照* 已取得定义,
    const 基础节点结构* 特征快照,const std::vector<基础联合节点读取项>* 命中快照组) const noexcept {
    try {
        if(实际定义)实际定义->reset();
        if(实现_) { auto& s=*实现_;std::lock_guard lock(s.互斥);
            if(s.通用) {
                const auto p=s.通用参数.find(n);
                if(p==s.通用参数.end()||p->second.规格.信息类型!=新方法参数信息类型::特征) return {};
                新特征信息 f;f.节点=n;
                if(特征快照 && 特征快照->节点.编码!=n)throw std::logic_error("simulation feature snapshot identity differs");
                const auto fields=[&](字段 key) {
                    if(!特征快照)return 全局基础数据集.查询字段(n,字段组[key]);
                    std::vector<基础字段关系> out;
                    for(const auto& f:特征快照->字段组)if(f.字段节点==字段组[key])out.push_back(f);
                    return out;
                };
                const auto ref=[&](字段 key)->std::optional<稳定编码> {
                    const auto fs=fields(key);if(fs.size()!=1)return {};
                    const auto id=std::get_if<稳定编码>(&fs.front().内容);
                    return id && (特征快照 || 全局基础数据集.查询节点(*id)) ? std::optional{*id} : std::nullopt;
                };
                const auto number=[&](字段 key)->std::optional<std::int64_t> {
                    const auto fs=fields(key);if(fs.size()!=1)return {};
                    const auto raw=std::get_if<基础值>(&fs.front().内容);
                    const auto value=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
                    return value ? std::optional{*value} : std::nullopt;
                };
                const auto def=已取得定义 ? std::optional{*已取得定义} : 读取本地特征实际定义(n);
                if(!def) return {};
                if(实际定义)*实际定义=*def;
                f.特征概念节点=def->概念节点;
                if(s.记录复制读取&&p->second.复制上下文&&!正式定位
                    &&(def->材料类型!=特征概念材料物理类型::需求节点引用||!s.内容参数位置.contains(n)))
                    s.已读取复制上下文=true; // 本叶只为已映射需求F提供完整前态覆盖，其它复制读取保留旧门。
                const bool 分界F=s.本地语言范围 && std::find(s.本地语言范围->本地特征.begin(),
                    s.本地语言范围->本地特征.end(),n)!=s.本地语言范围->本地特征.end();
                if(p->second.完整复制特征||分界F)f.实例单位=ref(结构单位引用);
                else f.实例单位=def->单位;
                if(const auto v=number(数值)) f.当前值=*v;
                else if(const auto v=ref(当前材料)) f.当前值=*v;
                for(const auto& h:fields(历史引用)) {
                    const auto* ref=std::get_if<稳定编码>(&h.内容);
                    if(!ref)throw std::runtime_error("simulation history reference malformed");
                    f.历史不同值.emplace_back(*ref);
                }
                const auto accurate=[](const 基础字段内容& value)->新特征准确值 {
                    if(const auto* ref=std::get_if<稳定编码>(&value))return *ref;
                    const auto* raw=std::get_if<基础值>(&value);const auto* x=raw?std::get_if<std::int64_t>(&raw->材料):nullptr;
                    if(!x)throw std::runtime_error("copied accurate feature value malformed");return *x;
                };
                const auto standards=fields(标准值引用);
                if(standards.size()>1)throw std::runtime_error("multiple local standard feature values");
                if(!standards.empty())f.标准值=accurate(standards.front().内容);
                for(const auto& h:fields(历史项引用))f.历史不同值.push_back(accurate(h.内容));
                std::vector<新特征准确值> 去重历史;
                for(const auto& 值:f.历史不同值)
                    if(std::find(去重历史.begin(),去重历史.end(),值)==去重历史.end())去重历史.push_back(值);
                f.历史不同值=std::move(去重历史);
                for(const auto& hitField:fields(历史命中引用)) {
                    const auto hitRef=std::get_if<稳定编码>(&hitField.内容);
                    if(!hitRef || (!命中快照组 && !全局基础数据集.查询节点(*hitRef)))
                        throw std::runtime_error("simulation history hit reference malformed");
                    const auto hit=*hitRef;
                    const auto hitFieldOf=[&](字段 key)->std::optional<基础字段关系> {
                        if(!命中快照组)return 唯一字段(hit,key);
                        const auto it=std::find_if(命中快照组->begin(),命中快照组->end(),
                            [&](const auto& item){return item.查询节点==hit;});
                        if(it==命中快照组->end() || !it->快照)return {};
                        std::optional<基础字段关系> result;
                        for(const auto& field:it->快照->结构.字段组)if(field.字段节点==字段组[key]) {
                            if(result)return {};result=field;
                        }
                        return result;
                    };
                    const auto v=hitFieldOf(结构引用),countField=hitFieldOf(命中次数值);
                    const auto raw=countField ? std::get_if<基础值>(&countField->内容) : nullptr;
                    const auto countPtr=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
                    if(!v || !countPtr || *countPtr<0)throw std::runtime_error("local history hit data invalid");
                    const auto count=*countPtr;
                    const auto value=accurate(v->内容);
                    f.历史值命中信息.push_back({value,count});
                    if((def->材料类型==特征概念材料物理类型::信息节点引用
                        || def->比较规则==特征概念比较规则::概念集合身份判等)
                        &&std::find(f.历史不同值.begin(),f.历史不同值.end(),value)==f.历史不同值.end())
                        f.历史不同值.push_back(value);
                }
                if(const auto domain=ref(完整实例区间引用))f.实例区间值=命中快照组
                    ? 读归类结构域批次(*domain,*命中快照组) : 读结构域(*domain);
                if(const auto domain=ref(完整聚合引用))f.实例历史聚合值域=命中快照组
                    ? 读归类结构域批次(*domain,*命中快照组) : 读结构域(*domain);
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
                // 归类样例保存本次实际读取的材料内容，后续不以节点当前内容补造。
                if(实现_->归类主体来源)实现_->记录内容(8,节点,r,true);
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
正式信息参数读取结果 新_模拟世界类::读取本地归类材料(稳定编码 N,稳定编码 C) const {
    using S=正式信息参数读取状态;
    auto& s=*实现_;
    std::lock_guard lock(s.互斥);
    const auto local=s.本地归类材料.find(N);
    const auto parameter=s.通用参数.find(N);
    if(!s.通用 || local==s.本地归类材料.end())return {S::信息不存在,{},{}};
    if(parameter==s.通用参数.end() || !parameter->second.复制归类材料)
        throw std::runtime_error("local classification parameter configuration missing");
    const auto& config=*parameter->second.复制归类材料;
    if(C!=config.范围.材料范围 && C!=config.范围.明确材料范围)return {S::范围未支持,{},{}};
    const auto field=[](const 基础联合节点快照& snapshot,字段 key)->std::optional<稳定编码> {
        std::optional<稳定编码> value;
        for(const auto& item:snapshot.结构.字段组)if(item.字段节点==字段组[key]) {
            const auto* ref=std::get_if<稳定编码>(&item.内容);
            if(value || !ref || !有效(*ref))throw std::runtime_error("local classification reference malformed");
            value=*ref;
        }
        return value;
    };
    std::set<稳定编码> ids{N,config.范围.材料范围,config.范围.目标词条FT,config.范围.采用概念FT};
    if(C==config.范围.明确材料范围)ids.insert(C);
    for(;;) {
        auto read=全局基础数据集.查询节点组完整结构({ids.begin(),ids.end()});
        if(read.状态==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        if(read.状态!=基础结构操作状态::已完成 || read.节点组.size()!=ids.size())
            throw std::runtime_error("local classification material batch unavailable");
        const auto find=[&](稳定编码 id)->const 基础联合节点快照* {
            const auto it=std::find_if(read.节点组.begin(),read.节点组.end(),
                [&](const auto& item){return item.查询节点==id;});
            return it==read.节点组.end() || !it->快照 ? nullptr : &*it->快照;
        };
        const auto* current=find(N);
        if(!current)throw std::runtime_error("local classification material missing");
        const auto word=field(*current,归类目标词条引用);
        const auto adopted=field(*current,归类采用概念引用);
        if(!word)throw std::runtime_error("local classification target word missing");
        const auto oldSize=ids.size();
        ids.insert(*word);
        if(adopted)ids.insert(*adopted);
        if(ids.size()!=oldSize)continue; // 依当前批的引用补足读取，不比较旧内容。
        std::set<稳定编码> required{N,*word,config.范围.材料范围,
            config.范围.目标词条FT,config.范围.采用概念FT};
        if(adopted)required.insert(*adopted);
        if(C==config.范围.明确材料范围)required.insert(C);
        std::vector<基础联合节点快照> protection;
        protection.reserve(required.size());
        for(const auto id:required) {
            const auto* snapshot=find(id);
            if(!snapshot)throw std::runtime_error("local classification referenced endpoint missing");
            protection.push_back(*snapshot);
        }
        if(field(*current,内容概念引用)!=std::optional{config.范围.材料范围}
            || !s.是本地参数(*word,新方法参数信息类型::词条))
            throw std::runtime_error("local classification material identity damaged");
        if(C==config.范围.明确材料范围 && !adopted)return {S::范围未支持,{},{}};
        正式内容值 selected{正式内容值种类::概念引用,正式内容值状态::当前未知,{}};
        if(adopted)selected={正式内容值种类::概念引用,正式内容值状态::已知,正式内容准确值{*adopted}};
        正式信息参数投影 out{N,config.范围.材料范围,{
            {config.范围.目标词条FT,{正式内容值种类::词条引用,正式内容值状态::已知,正式内容准确值{*word}}},
            {config.范围.采用概念FT,std::move(selected)}},std::nullopt};
        s.记录内容(7,N,out,local->second.has_value());
        return {S::已读取,std::move(out),std::move(protection)};
    }
}
正式信息参数读取结果 新_模拟世界类::读取本地语言范围参数(稳定编码 R,稳定编码 C) const {
    using S=正式信息参数读取状态;
    const auto& 范围=*实现_->本地语言范围;
    if (C!=范围.结构概念) return {S::范围未支持,{},{}};
    std::vector<稳定编码> 节点组{R};
    节点组.insert(节点组.end(),范围.本地特征.begin(),范围.本地特征.end());
    auto 批=全局基础数据集.查询节点组完整结构(节点组);
    if (批.状态==基础结构操作状态::资源失败) return {S::资源失败,{},{}};
    if (批.状态==基础结构操作状态::节点不存在) return {S::前提已变化,{},{}};
    if (批.状态!=基础结构操作状态::已完成 || 批.节点组.size()!=节点组.size())
        throw std::logic_error("local language range snapshot failed");

    正式信息参数投影 投影{R,C,{},{}};
    std::vector<基础联合节点快照> 保护;
    for (std::size_t i=0; i<节点组.size(); ++i) {
        const auto it=std::lower_bound(批.节点组.begin(),批.节点组.end(),节点组[i],
            [](const auto& 项,稳定编码 节点) { return 项.查询节点<节点; });
        if (it==批.节点组.end() || it->查询节点!=节点组[i])
            throw std::logic_error("local language range batch identity differs");
        auto& 项=*it;
        if (!项.快照) {
            if (i==0) return {S::信息不存在,{},{}};
            throw std::logic_error("registered local language feature is missing");
        }
        if (i) {
            const auto FT=实现_->通用参数.at(项.查询节点).规格.范围概念节点;
            std::optional<稳定编码> 材料;
            for (const auto& 字段:项.快照->结构.字段组) {
                if (字段.字段节点==字段组[当前材料]) 材料=std::get<稳定编码>(字段.内容);
            }
            正式内容值 值{正式内容值种类::信息对象引用,正式内容值状态::当前未知,{}};
            if (材料) {
                值.状态=正式内容值状态::已知;
                值.值=正式内容准确值{*材料};
            }
            投影.直接内容组.push_back({FT,std::move(值)});
            投影.特征定位组.push_back({FT,项.查询节点});
        }
        保护.push_back(std::move(*项.快照));
    }
    实现_->记录内容(7,R,投影,true);
    return {S::已读取,std::move(投影),std::move(保护)};
}

新语言分界调用结果<新语言范围原文信息> 新_模拟世界类::读取本地语言范围原文(稳定编码 R) const {
    using S=新语言分界工程状态;
    try {
        if (!实现_) return {S::未找到,{},{}};
        const auto& s=*实现_;
        std::lock_guard 锁(s.互斥);
        if (!s.通用 || !s.样例就绪 || !s.本地语言范围 || s.本地语言范围->本地范围!=R)
            return {S::未找到,{},{}};
        const auto 区间=读结构材料<std::vector<std::uint64_t>>(R,结构材料值);
        if (区间.size()!=2) throw std::logic_error("local language range coordinates corrupt");
        新语言材料范围请求 定位{必读引用(R,来源引用),区间[0],区间[1],
            必读引用(R,词条语言引用),必读引用(R,词条规范化合同引用),s.通用场景};
        return {S::已完成,新语言范围原文信息{
            {R,定位,s.本地语言范围->本地特征},必读引用(R,正文引用),
            读结构材料<std::string>(R,固定语义引用),读结构材料<std::string>(R,数值)},{}};
    } catch (const std::bad_alloc&) {
        return {S::资源失败,{},{}};
    }
}

新语言分界调用结果<新语言分界材料信息> 新_模拟世界类::读取本地语言分界材料(稳定编码 节点) const {
    using S=新语言分界工程状态;
    try {
        if (!实现_) return {S::未找到,{},{}};
        const auto& s=*实现_;
        std::lock_guard 锁(s.互斥);
        if (!s.通用 || !s.样例就绪 || !s.本地语言范围
            || !属于样例(节点,语言分界材料种类,s.通用场景))
            return {S::未找到,{},{}};
        const auto R=s.本地语言范围->本地范围;
        if (读引用(节点,所属)!=R) throw std::logic_error("local boundary material has a different range");
        const auto 有序下位=[](稳定编码 持有者,字段 键) {
            std::vector<std::pair<std::int64_t,稳定编码>> 组;
            for (const auto 子:读引用组(持有者,键))
                组.emplace_back(必读数(子,结构顺序值),子);
            std::sort(组.begin(),组.end());
            return 组;
        };
        const auto 读词段=[&](稳定编码 持有者) {
            std::vector<新语言分界词段> 词段组;
            for (const auto& 项:有序下位(持有者,结构子项)) {
                const auto 区间=读结构材料<std::vector<std::uint64_t>>(项.second,结构材料值);
                if (区间.size()!=2) throw std::logic_error("local boundary word coordinates corrupt");
                const auto 类别=必读数(项.second,结构类别值);
                if (类别<0 || 类别>static_cast<std::int64_t>(新语言分界段用途::分隔))
                    throw std::logic_error("local boundary segment kind corrupt");
                const auto 用途=类别==0 ? 新语言分界段用途::词面 : static_cast<新语言分界段用途>(类别);
                词段组.push_back({区间[0],区间[1],读引用(项.second,词条特征引用),用途});
            }
            return 词段组;
        };
        新语言分界材料定义 定义;
        using 种类=新语言范围特征种类;
        switch (static_cast<种类>(必读数(节点,结构类别值))) {
        case 种类::分词候选材料: {
            新语言分界候选定义 候选;
            for (const auto& 项:有序下位(节点,结构子项)) {
                新语言分界候选路径 路径{读词段(项.second),{}};
                for (const auto& 应用:有序下位(项.second,场景成员引用))
                    路径.规则应用记录组.push_back(必读引用(应用.second,来源引用));
                候选.路径组.push_back(std::move(路径));
            }
            定义=std::move(候选);
            break;
        }
        case 种类::分界依据: {
            const auto 来源=必读引用(节点,来源引用);
            const auto 问题=读引用(节点,目标引用);
            const 新语言分界取材来源 依据来源=问题
                ? 新语言分界取材来源{新语言回答分界来源{*问题,必读引用(节点,确认引用),来源}}
                : 新语言分界取材来源{新语言规则分界来源{来源}};
            定义=新语言分界依据定义{必读引用(节点,结构引用),{读词段(节点),依据来源}};
            break;
        }
        case 种类::有效分界结果:
            定义=新语言有效分界定义{必读引用(节点,结构引用)};
            break;
        case 种类::词条准入结果:
            定义=新语言词条准入定义{必读引用(节点,结构引用),读词段(节点)};
            break;
        default:
            throw std::logic_error("local boundary material kind corrupt");
        }
        return {S::已完成,新语言分界材料信息{节点,R,std::move(定义)},{}};
    } catch (const std::bad_alloc&) {
        return {S::资源失败,{},{}};
    }
}

正式信息参数读取结果 新_模拟世界类::读取正式信息参数保护(稳定编码 N,稳定编码 C) const noexcept {
    using S=正式信息参数读取状态;
    try {
        if(!实现_)return {S::信息不存在,{},{}};
        auto& s=*实现_;
        {
            std::lock_guard lock(s.互斥);
            if(!s.通用)return {S::信息不存在,{},{}};
            if(s.本地归类材料.contains(N))return 读取本地归类材料(N,C);
            if(s.本地语言范围&&s.本地语言范围->本地范围==N) {
                if(!s.样例就绪)return {S::信息不存在,{},{}};
                return 读取本地语言范围参数(N,C);
            }
        }
        const auto roots=s.方法.读取模拟需求概念组();
        if(roots.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(roots.状态==正式结构操作状态::依赖未就绪||roots.状态==正式结构操作状态::未找到)
            return {S::依赖未就绪,{},{}};
        if(roots.状态!=正式结构操作状态::已读取||!roots.信息)
            throw std::runtime_error("formal simulation demand roots unavailable");
        // 方案D（2026-11-12用户裁决）：范围可为需求根或先天下位概念“补问需求”（槽233）“被认知需求”（槽208），
        // 归属按与需求owner相同的事实判定（需求属于补问需求/需求属于被认知需求）。
        const auto 槽节点=[&](std::uint32_t 槽)->std::optional<稳定编码> {
            for(const auto& [x,n]:roots.信息->子类与支持定义)if(x==槽)return n;
            return std::nullopt;
        };
        const bool 补问范围=槽节点(233)==std::optional{C},被认知范围=槽节点(208)==std::optional{C};
        if(C!=roots.信息->需求&&!补问范围&&!被认知范围)return {S::范围未支持,{},{}};
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
            if(被认知范围&&d.执行域!=新需求执行域::被认知)return {S::范围未支持,{},{}};
            if(补问范围) {
                const auto& 定位=d.目标.引用内容定位;
                if(d.执行域!=新需求执行域::自我执行||d.来源类型!=std::optional{新需求来源类型::任务子目标}
                    ||!定位||定位->正式内容路径.size()!=1)return {S::范围未支持,{},{}};
                const auto 被引用=s.本地需求.contains(定位->引用需求节点)
                    ? 读取模拟需求(定位->引用需求节点) : 新模拟需求读回{};
                if(!被引用.信息||被引用.信息->执行域!=新需求执行域::被认知)return {S::范围未支持,{},{}};
            }
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
        // 复读时定义已被并发删除：属内部世界变化；输入拒绝、定义冲突等仍属工程故障。
        if(rootsAfter.状态==正式结构操作状态::未找到||rootAfter.状态==正式结构操作状态::未找到)
            return {S::前提已变化,{},{}};
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
        // 最终保护读取时节点已被并发删除：前提已变化；内部不一致仍属工程故障。
        if(current.状态==基础结构操作状态::节点不存在||current.状态==基础结构操作状态::引用不存在)
            return {S::前提已变化,{},{}};
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
    for(const auto x:l.直接成员存在组)if((!s.是普通存在参数(x)&&!s.是本地参数(x,新方法参数信息类型::词条))||读引用(x,所属)!=n)throw std::runtime_error("local language scene member corrupt");
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
    if(!实现_)return {};std::lock_guard lock(实现_->互斥);
    基础节点组追加请求 q;q.删除节点组=实现_->通用节点;实现_->合并词典回执退出(q);
    return std::move(q.删除节点组);
}
} // namespace 海中鱼巣

namespace 海中鱼巣 {
namespace {
struct 模拟语言范围前态 final {
    新语言范围原文信息 原文;
    std::array<稳定编码, 4> 特征类型;
    std::array<std::optional<稳定编码>, 4> 当前材料;
    std::array<新特征信息, 4> 特征组;
    std::map<稳定编码, 新语言分界材料定义> 材料组;
};

新模拟世界状态 取得模拟语言范围前态(const 新模拟语言范围样例来源& 来源,
    模拟语言范围前态& 前态) {
    using 状态 = 新模拟世界状态;
    using 材料状态 = 新语言分界工程状态;
    const auto 待重取 = [](材料状态 读回) {
        return 读回 == 材料状态::资源失败 ? 状态::资源失败
            : 读回 == 材料状态::前提已变化 ? 状态::定义前提已变化 : 状态::需重新组织输入;
    };
    const auto 归属=来源.语言.读取材料范围特征(来源.范围);
    if (归属.状态!=材料状态::已完成) return 待重取(归属.状态);
    auto 正文=来源.语言.读取材料范围原文(来源.范围,来源.特征值);
    if (正文.状态!=材料状态::已完成) return 待重取(正文.状态);
    if (归属.信息->范围.特征节点组!=正文.信息->范围.特征节点组) return 状态::定义前提已变化;
    前态.原文=std::move(*正文.信息);
    前态.特征类型=归属.信息->特征类型组;

    // 完整特征读取会返回历史；不能把只读到的空当前值解释成没有历史。
    std::vector<std::pair<稳定编码,std::size_t>> 待读取;
    using 特征状态=新特征完整读取状态;
    for (std::size_t i=0; i<前态.特征类型.size(); ++i) {
        auto 读取=来源.特征.读取特征完整保护(前态.原文.范围.特征节点组[i]);
        if (读取.状态==特征状态::前提已变化) return 状态::定义前提已变化;
        if (读取.状态==特征状态::资源失败) return 状态::资源失败;
        if (读取.状态==特征状态::类型未支持) return 状态::能力未支持;
        if (读取.状态!=特征状态::已读取) return 状态::需重新组织输入;
        if (读取.信息->特征类型根!=前态.特征类型[i]) return 状态::定义前提已变化;
        auto& F=前态.特征组[i];
        F=std::move(读取.信息->特征);
        const auto 是引用域=[](const auto& 域) {
            return !域 || std::holds_alternative<特征概念信息引用值域>(*域);
        };
        if (F.上级特征节点 || !是引用域(F.实例区间值) || !是引用域(F.实例历史聚合值域))
            return 状态::能力未支持;
        const auto 录入=[&](const 新特征准确值& 值) {
            待读取.emplace_back(std::get<稳定编码>(值),i);
            return 待读取.back().first;
        };
        if (F.当前值) 前态.当前材料[i]=录入(*F.当前值);
        if (F.标准值) 录入(*F.标准值);
        for (const auto& 历史:F.历史不同值) 录入(历史);
        for (const auto& 命中:F.历史值命中信息) 录入(命中.值);
    }
    while (!待读取.empty()) {
        const auto [节点, 种类]=待读取.back();
        待读取.pop_back();
        auto 已读=前态.材料组.find(节点);
        if (已读 != 前态.材料组.end()) {
            if (已读->second.index()!=种类) throw std::logic_error("language range material used with conflicting roles");
            continue;
        }
        auto 材料=来源.语言.读取分界材料(节点);
        if (材料.状态 != 材料状态::已完成) return 待重取(材料.状态);
        if (材料.信息->所属范围 != 来源.范围 || 材料.信息->定义.index()!=种类)
            throw std::logic_error("formal language range material reference has a different owner or kind");
        auto& 定义=材料.信息->定义;
        if (const auto* 依据=std::get_if<新语言分界依据定义>(&定义))
            待读取.emplace_back(依据->候选材料,0);
        if (const auto* 有效=std::get_if<新语言有效分界定义>(&定义))
            待读取.emplace_back(有效->正式依据,1);
        if (const auto* 准入=std::get_if<新语言词条准入定义>(&定义))
            待读取.emplace_back(准入->有效结果,2);
        前态.材料组.emplace(节点,std::move(定义));
    }
    return 状态::已完成;
}
} // namespace

新模拟世界建立结果 新_模拟世界类::建立有序样例(稳定编码 M,
    const std::vector<新模拟有序参数输入>& 正式输入,const std::vector<新模拟输入事件>& 事件,
    const std::vector<新模拟任务治理输入>& 任务组,const std::vector<新模拟场景事实输入>& 场景输入,
    std::optional<稳定编码> 执行分支,const std::optional<新模拟消息方案输入>& 消息输入,
    const 新模拟需求输入图& 需求图,const std::optional<新模拟执行主体输入>& 执行主体输入,
    const std::vector<新模拟词典材料复制输入>& 词典材料输入,
    const std::optional<新模拟归类主体输入>& 归类主体输入,
    const std::optional<新模拟语言范围样例来源>& 语言范围来源) {
    if(执行主体输入 && 归类主体输入)return {新模拟世界状态::入口拒绝,{}};
    if(归类主体输入 && (!有效(归类主体输入->来源存在) || !有效(归类主体输入->专属概念)
        || 归类主体输入->同源存在输入!=4))
        return {新模拟世界状态::入口拒绝,{}};
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
    if(!m||m->参数规格组.size()>正式输入.size()) { 模拟样例拒绝行=__LINE__; return {}; }
    std::optional<模拟语言范围前态> 语言前态;
    std::vector<新模拟有序参数输入> 语言样例输入;
    std::array<std::size_t, 4> 语言特征位置{};
    if (语言范围来源) {
        using P=新方法参数信息类型;
        const bool 分词=m->本能函数登记名==新本能函数登记名_语言片段分词;
        const bool 准入=m->本能函数登记名==新本能函数登记名_未登记片段建立或取得词条;
        if ((!分词&&!准入) || !执行分支 || 正式输入.size()!=m->参数规格组.size()
            || 正式输入.size()!=(分词 ? 5 : 4) || 正式输入.front().规格.信息类型!=P::存在)
            return {新模拟世界状态::入口拒绝,{}};
        const auto C=正式输入.front().规格.范围概念节点;
        const auto 结构=s.方法.读取模拟正式结构(C);
        if (结构.状态==正式结构操作状态::前提已变化)
            return {新模拟世界状态::定义前提已变化,{}};
        if (!结构.信息 || !std::holds_alternative<信息结构概念定义>(结构.信息->定义))
            return {新模拟世界状态::能力未支持,{}};
        语言前态.emplace();
        const auto 取材=取得模拟语言范围前态(*语言范围来源,*语言前态);
        if (取材!=新模拟世界状态::已完成) return {取材,{}};
        if (const auto& 来源=语言范围来源->定向输入) {
            using F=新语言范围特征种类;
            const auto 当前=[&](F 槽){return 语言前态->当前材料[static_cast<std::size_t>(槽)];};
            if (来源->范围!=语言范围来源->范围 || 来源->执行分支!=执行分支
                || 当前(F::分词候选材料)!=来源->候选材料 || 当前(F::分界依据)!=来源->分界依据
                || 当前(F::有效分界结果))
                return {新模拟世界状态::需重新组织输入,{}};
        }
        语言样例输入=正式输入;
        const auto& 类型组=语言前态->特征类型;
        for (std::size_t i=0; i<类型组.size(); ++i) {
            auto 位置=std::find_if(语言样例输入.begin(),语言样例输入.end(),[&](const auto& 项) {
                return 项.规格.信息类型==P::特征 && 项.规格.所属存在位置==1
                    && 项.规格.范围概念节点==类型组[i];
            });
            const auto 索引=static_cast<std::size_t>(位置-语言样例输入.begin());
            语言特征位置[i]=索引;
            if (位置==语言样例输入.end()) {
                新模拟有序参数输入 私有字段{{static_cast<std::uint32_t>(索引+1),P::特征,类型组[i],{},1}};
                私有字段.所属存在参数=1;
                语言样例输入.push_back(std::move(私有字段));
            } else {
                if (位置->值 || 位置->材料 || 位置->同一参数 || 位置->完整复制特征 || 位置->引用存在参数
                    || 位置->引用需求输入 || 位置->所属语言场景输入 || 位置->来源存在 || 位置->复制上下文
                    || !位置->集合成员参数组.empty() || !位置->来源任务成员组.empty()
                    || !位置->历史引用存在参数组.empty() || 位置->复制特征概念
                    || 位置->初值方式!=新模拟特征初值方式::常规输入
                    || (位置->所属存在参数 && 位置->所属存在参数!=1))
                    return {新模拟世界状态::入口拒绝,{}};
                位置->所属存在参数=1;
            }
        }
    }
    const auto& 输入=语言前态 ? 语言样例输入 : 正式输入;
    const auto 是语言范围=[&](std::size_t i) { return 语言前态.has_value() && i==0; };
    const auto 语言槽=[&](std::size_t i)->std::optional<std::size_t> {
        if (!语言前态) return {};
        const auto it=std::find(语言特征位置.begin(),语言特征位置.end(),i);
        if (it==语言特征位置.end()) return {};
        return static_cast<std::size_t>(it-语言特征位置.begin());
    };
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
        if (语言前态) {
            if (!有效声明.分支) return {新模拟世界状态::入口拒绝,{}};
            const auto& 当前=语言前态->当前材料;
            const bool 准入=m->本能函数登记名==新本能函数登记名_未登记片段建立或取得词条;
            if (准入 && !当前[2]) return {新模拟世界状态::需重新组织输入,{}};
            if (!准入 && 有效声明.分支->定义.顺序==1) {
                if (!当前[0] || !当前[1] || std::get<新语言分界依据定义>(
                    语言前态->材料组.at(*当前[1])).候选材料 != 当前[0])
                    return {新模拟世界状态::需重新组织输入,{}};
            }
        }
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
    std::vector<bool> 需求参数(输入.size(),false),归类参数(输入.size(),false);
    for(std::size_t i=0;i<输入.size();++i) {
        const auto& p=输入[i];
        if(p.复制特征概念) {
            if(m->本能函数登记名!=std::optional<std::string>{新本能函数登记名_按归类材料挂接词条}
                || i!=1 || p.规格.信息类型!=新方法参数信息类型::特征
                || p.所属存在参数!=1 || !p.完整复制特征 || !有效(*p.复制特征概念))
                return {新模拟世界状态::入口拒绝,{}};
            const auto actual=s.方法.读取模拟特征范围(*p.复制特征概念);
            const auto declared=s.方法.读取模拟特征范围(p.规格.范围概念节点);
            if(!actual || !declared || actual->特征类型根!=declared->特征类型根
                || actual->材料类型!=declared->材料类型
                || actual->比较规则!=特征概念比较规则::信息引用身份精确
                || declared->比较规则!=actual->比较规则)
                return {新模拟世界状态::入口拒绝,{}};
        }
        if(p.规格.信息类型==新方法参数信息类型::词条) {
            if(!s.存在概念) throw std::logic_error("simulation word concept reader unavailable");
            const auto d=s.存在概念->读取当前基础范围概念(p.规格.范围概念节点);
            if(d.状态==新基础范围读取状态::资源失败)throw std::bad_alloc{};
            if(d.状态==新基础范围读取状态::依赖未就绪)
                throw std::logic_error("simulation word range reader unavailable");
            if(!d.定义||d.定义->原语!=基础概念原语::具名领域身份
                ||d.定义->整数参数!=std::vector<std::int64_t>{16}) { 模拟样例拒绝行=__LINE__; return {}; }
            if(!p.复制词条||!有效(p.复制词条->节点)||!有效(p.复制词条->语言节点)
                ||!有效(p.复制词条->规范化合同节点)||p.复制词条->规范化词面.empty()
                ||!新_语言类::严格UTF8字节串有效(p.复制词条->规范化词面)
                ||p.所属存在参数||p.值||p.材料||p.引用存在参数||p.来源存在
                ||p.构造存在概念||p.引用需求输入||p.完整复制特征||p.所属语言场景输入
                ||p.引用语言场景输入||p.需求参数输入||!p.集合成员参数组.empty()
                ||!p.来源任务成员组.empty()||!p.历史引用存在参数组.empty()
                ||p.初值方式!=新模拟特征初值方式::常规输入) { 模拟样例拒绝行=__LINE__; return {}; }
        } else if(p.复制词条) { 模拟样例拒绝行=__LINE__; return {}; }
        if(是语言范围(i)) {
            if(p.同一参数 || p.复制归类材料 || p.复制词条 || p.需求参数输入 || p.来源存在
                || p.构造存在概念 || p.引用语言场景输入 || p.值 || p.材料)
                return {新模拟世界状态::入口拒绝,{}};
            continue;
        }
        if(p.复制归类材料) {
            const auto& r=*p.复制归类材料;
            if(p.规格.信息类型!=新方法参数信息类型::存在 || !有效(r.来源材料) || !有效(r.来源词条)
                || !有效(r.范围.材料范围) || !有效(r.范围.明确材料范围)
                || !有效(r.范围.目标词条FT) || !有效(r.范围.采用概念FT)
                || (r.采用概念 && !有效(*r.采用概念))
                || (p.规格.范围概念节点!=r.范围.材料范围 && p.规格.范围概念节点!=r.范围.明确材料范围)
                || (p.规格.范围概念节点==r.范围.明确材料范围 && !r.采用概念)
                || !r.目标词条参数 || r.目标词条参数>输入.size()
                || !输入[r.目标词条参数-1].复制词条
                || 输入[r.目标词条参数-1].规格.信息类型!=新方法参数信息类型::词条
                || 输入[r.目标词条参数-1].复制词条->节点!=r.来源词条
                || p.需求参数输入 || p.所属存在参数 || p.值 || p.材料 || !p.集合成员参数组.empty()
                || p.引用存在参数 || p.来源存在 || p.复制上下文 || !p.来源任务成员组.empty()
                || p.构造存在概念 || !p.历史引用存在参数组.empty() || p.引用需求输入
                || p.初值方式!=新模拟特征初值方式::常规输入 || p.完整复制特征
                || p.所属语言场景输入 || p.引用语言场景输入) { 模拟样例拒绝行=__LINE__; return {}; }
            归类参数[i]=true;
            continue;
        }
        if(p.规格.信息类型==新方法参数信息类型::存在) {
            const auto formal=s.方法.读取模拟正式结构(p.规格.范围概念节点);
            if(formal.状态==正式结构操作状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
            if(formal.信息&&std::holds_alternative<信息结构概念定义>(formal.信息->定义)) {
                const auto roots=s.方法.读取模拟需求概念组();
                if(roots.状态==正式结构操作状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
                if(roots.状态!=正式结构操作状态::已读取||!roots.信息){ 模拟样例拒绝行=__LINE__; return {}; }
                // 方案D：需求参数范围可为需求根或先天下位概念“补问需求”（233）“被认知需求”（208）；实参归属由正式参数读取按事实判定。
                if(p.规格.范围概念节点!=roots.信息->需求&&std::none_of(roots.信息->子类与支持定义.begin(),roots.信息->子类与支持定义.end(),
                    [&](const auto& x){return (x.first==233||x.first==208)&&x.second==p.规格.范围概念节点;})){ 模拟样例拒绝行=__LINE__; return {}; }
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
    const auto 是普通存在=[&](std::size_t i){return 输入[i].规格.信息类型==新方法参数信息类型::存在&&!需求参数[i]&&!归类参数[i]&&!是语言范围(i);};
    std::vector<std::size_t> 代表(输入.size());
    for(std::size_t i=0;i<代表.size();++i)代表[i]=i;
    const auto 查代表=[&](std::size_t i) { while(代表[i]!=i)i=代表[i];return i; };
    for(std::size_t i=0;i<输入.size();++i) if(输入[i].同一参数) {
        const auto j=static_cast<std::size_t>(输入[i].同一参数-1);
        if(j>=输入.size()||是语言范围(i)||是语言范围(j)||语言槽(i)||语言槽(j)
            ||输入[i].规格.信息类型!=输入[j].规格.信息类型||需求参数[i]!=需求参数[j]||归类参数[i]!=归类参数[j]) { 模拟样例拒绝行=__LINE__; return {}; }
        const auto a=查代表(i),b=查代表(j);代表[(std::max)(a,b)]=(std::min)(a,b);
    }
    for(std::size_t i=0;i<代表.size();++i)代表[i]=查代表(i);
    std::map<std::size_t,std::vector<稳定编码>> 普通名称前态,全部名称前态;
    for(std::size_t i=0;i<输入.size();++i)if(归类参数[i]) {
        const auto& r=*输入[i].复制归类材料;
        if(!r.普通名称概念组)continue;
        auto names=*r.普通名称概念组;
        if(std::any_of(names.begin(),names.end(),[](稳定编码 C){return !有效(C);}))
            return {新模拟世界状态::入口拒绝,{}};
        std::sort(names.begin(),names.end());
        names.erase(std::unique(names.begin(),names.end()),names.end());
        const auto word=代表.at(r.目标词条参数-1);
        const auto [it,added]=普通名称前态.emplace(word,names);
        if(!added && it->second!=names)return {新模拟世界状态::需重新组织输入,{}};
    }
    for(std::size_t i=0;i<输入.size();++i)if(归类参数[i]) {
        const auto& r=*输入[i].复制归类材料;
        if(!r.全部名称概念组)continue;
        auto names=*r.全部名称概念组;
        if(std::any_of(names.begin(),names.end(),[](稳定编码 C){return !有效(C);}))
            return {新模拟世界状态::入口拒绝,{}};
        std::sort(names.begin(),names.end());names.erase(std::unique(names.begin(),names.end()),names.end());
        const auto word=代表.at(r.目标词条参数-1);
        const auto [it,added]=全部名称前态.emplace(word,names);
        if(!added && it->second!=names)return {新模拟世界状态::需重新组织输入,{}};
    }
    for(const auto& [word,names]:普通名称前态) {
        const auto all=全部名称前态.find(word);
        if(all!=全部名称前态.end() && !std::includes(all->second.begin(),all->second.end(),names.begin(),names.end()))
            return {新模拟世界状态::需重新组织输入,{}};
    }
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
        if(需求参数[i]||归类参数[i]||是语言范围(i))continue;
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
    const auto 输入数值=[](const 新模拟有序参数输入& in)->const std::int64_t* {
        if(in.初值方式==新模拟特征初值方式::常规输入)
            return in.值 ? std::get_if<std::int64_t>(&*in.值) : nullptr;
        if(in.初值方式!=新模拟特征初值方式::复制准确||!in.完整复制特征
            ||!in.完整复制特征->当前值)return nullptr;
        const auto& current=*in.完整复制特征->当前值;
        return current.种类==新模拟复制值种类::I64 ? std::get_if<std::int64_t>(&current.内容) : nullptr;
    };
    const auto 输入材料=[](const 新模拟有序参数输入& in)->const 新特征值材料* {
        if(in.初值方式==新模拟特征初值方式::常规输入)return in.材料 ? &*in.材料 : nullptr;
        if(in.初值方式!=新模拟特征初值方式::复制准确||!in.完整复制特征
            ||!in.完整复制特征->当前值)return nullptr;
        const auto& current=*in.完整复制特征->当前值;
        const auto* value=current.种类==新模拟复制值种类::普通材料
            ? std::get_if<新特征值信息>(&current.内容) : nullptr;
        return value ? &value->材料 : nullptr;
    };
    const auto 输入概念引用命中=[&](const 新模拟有序参数输入& in,
        const 新方法条件项定义& 条件,const 新特征匹配定义快照& 域) {
        if(!in.完整复制特征 || !域.概念身份范围)return false;
        const auto accepts=[&](const 新模拟复制准确值& value) {
            const auto* C=value.种类==新模拟复制值种类::正式概念引用
                ? std::get_if<稳定编码>(&value.内容) : nullptr;
            return C && 有效(*C) && (!域.概念身份范围->单点概念 || *C==*域.概念身份范围->单点概念);
        };
        if(条件.取值位置==特征取值位置::历史不同值)
            return std::any_of(in.完整复制特征->历史不同值.begin(),in.完整复制特征->历史不同值.end(),accepts);
        return in.完整复制特征->当前值 && accepts(*in.完整复制特征->当前值);
    };
    if(m->初始必要条件) {
        const auto& 必要=*m->初始必要条件;
        if(必要.条件.关系规则!=新方法项关系规则::全部成立){ 模拟样例拒绝行=__LINE__; return {}; }
        for(auto 条件:必要.条件.条件项组) {
            if (条件.参数序号) {
                if (是语言范围(条件.参数序号-1)) {
                    新方法条件项定义 声明;
                    声明.参数序号=条件.参数序号;
                    声明.存在概念节点=输入.front().规格.范围概念节点;
                    if (条件!=声明) return {新模拟世界状态::能力未支持,{}};
                    continue;
                }
                if (const auto 槽=语言槽(条件.参数序号-1)) {
                    新方法条件项定义 声明;
                    声明.参数序号=条件.参数序号;
                    声明.特征类型概念节点=语言前态->特征类型[*槽];
                    声明.特征值域概念节点=声明.特征类型概念节点;
                    if (条件!=声明) return {新模拟世界状态::能力未支持,{}};
                    if (!语言前态->当前材料[*槽]) return {新模拟世界状态::需重新组织输入,{}};
                    continue;
                }
            }
            if(条件.参数序号&&条件.参数序号<=m->参数规格组.size()&&(需求参数[条件.参数序号-1]||归类参数[条件.参数序号-1])) {
                const auto position=条件.参数序号-1;
                新方法条件项定义 expected;expected.参数序号=条件.参数序号;
                expected.存在概念节点=输入[position].规格.范围概念节点;
                if(条件==expected)continue;
                if(归类参数[position]) {
                    const auto& material=*输入[position].复制归类材料;
                    expected.存在概念节点.reset();
                    expected.引用内容条件=新方法引用内容条件{{},material.范围.明确材料范围,新方法内容作用点::参数自身};
                    if(条件==expected && material.采用概念)continue;
                }
                { 模拟样例拒绝行=__LINE__; return {}; }
            }
            if(条件.参数序号&&条件.参数序号<=m->参数规格组.size()
                &&(输入[条件.参数序号-1].规格.信息类型==新方法参数信息类型::存在
                    ||输入[条件.参数序号-1].规格.信息类型==新方法参数信息类型::词条)
                &&条件.特征类型概念节点) {
                const auto E位置=条件.参数序号-1;
                if(条件.存在概念节点) {
                    const bool compatible=输入[E位置].规格.信息类型==新方法参数信息类型::词条
                        ? *条件.存在概念节点==输入[E位置].规格.范围概念节点
                        : 范围包含(构造范围[E位置],*条件.存在概念节点);
                    if(!compatible){ 模拟样例拒绝行=__LINE__; return {}; }
                }
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
                        if(域->概念身份范围) {
                            if(!输入概念引用命中(in,条件,*域)){ 模拟样例拒绝行=__LINE__; return {}; }
                        } else if(条件.取值位置==特征取值位置::历史不同值) {
                            return {新模拟世界状态::能力未支持,{}};
                        } else if(const auto* 有限=std::get_if<std::vector<稳定编码>>(&域->值域)) {
                            const auto* 当前材料=输入材料(in);
                            if(!当前材料){ 模拟样例拒绝行=__LINE__; return {}; }
                            bool 命中=false;
                            for(const auto V:*有限){const auto v=s.方法.读取模拟候选材料(V);if(v&&v->材料==*当前材料)命中=true;}
                            if(!命中){ 模拟样例拒绝行=__LINE__; return {}; }
                        } else if(const auto* ref=std::get_if<特征概念存在引用值域>(&域->值域)) {
                            if(!in.引用存在参数||in.引用存在参数>输入.size()
                                ||!范围包含(构造范围[in.引用存在参数-1],ref->存在概念节点)){ 模拟样例拒绝行=__LINE__; return {}; }
                        } else if(const auto* a=std::get_if<特征概念结构化I64值域>(&域->值域)) {
                            const auto* 当前材料=输入材料(in);
                            const auto* values=当前材料 ? std::get_if<std::vector<std::int64_t>>(当前材料) : nullptr;
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
                const auto* v=输入数值(in);
                const auto d=s.方法.读取模拟特征范围(in.规格.范围概念节点);
                auto shape=条件;shape.特征类型概念节点.reset();shape.特征值域概念节点.reset();shape.精确数值域.reset();
                新方法条件项定义 expected;expected.参数序号=条件.参数序号;
                if(d && d->概念身份范围) {
                    expected.取值位置=条件.取值位置;
                    if(shape!=expected || 条件.精确数值域
                        || 条件.特征类型概念节点!=std::optional{d->特征类型根}){ 模拟样例拒绝行=__LINE__; return {}; }
                    if(条件.特征值域概念节点) {
                        const auto 域=s.方法.读取模拟特征范围(*条件.特征值域概念节点);
                        if(!域 || 域->特征类型根!=d->特征类型根
                            || !输入概念引用命中(in,条件,*域)){ 模拟样例拒绝行=__LINE__; return {}; }
                    } else if(条件.取值位置!=特征取值位置::当前值) return {新模拟世界状态::能力未支持,{}};
                    continue;
                }
                if(条件.取值位置!=特征取值位置::当前值) return {新模拟世界状态::能力未支持,{}};
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
            if(条件.参数序号&&条件.参数序号<=输入.size()
                &&输入[条件.参数序号-1].规格.信息类型==新方法参数信息类型::词条) {
                新方法条件项定义 expected;expected.参数序号=条件.参数序号;
                expected.存在概念节点=输入[条件.参数序号-1].规格.范围概念节点;
                if(条件!=expected){ 模拟样例拒绝行=__LINE__; return {}; }
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
            if(归类参数[左]) {
                const auto& r=*输入[左].复制归类材料;
                if(输入[右].规格.信息类型!=新方法参数信息类型::词条
                    || 关系.左正式内容路径!=std::vector<稳定编码>{r.范围.目标词条FT}
                    || 代表[r.目标词条参数-1]!=右) { 模拟样例拒绝行=__LINE__; return {}; }
                continue;
            }
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
        if((需求参数[左]||需求参数[右]||归类参数[左]||归类参数[右])&&关系.种类!=新方法参数关系种类::同一&&关系.种类!=新方法参数关系种类::不同
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
        auto 归类a=a.复制归类材料,归类b=b.复制归类材料;
        if(归类a&&归类b) {
            const auto canonical=[](auto& names) {
                if(!names)return;
                std::sort(names->begin(),names->end());
                names->erase(std::unique(names->begin(),names->end()),names->end());
            };
            canonical(归类a->普通名称概念组);canonical(归类b->普通名称概念组);
            if(!归类a->普通名称概念组)归类a->普通名称概念组=归类b->普通名称概念组;
            if(!归类b->普通名称概念组)归类b->普通名称概念组=归类a->普通名称概念组;
            canonical(归类a->全部名称概念组);canonical(归类b->全部名称概念组);
            if(!归类a->全部名称概念组)归类a->全部名称概念组=归类b->全部名称概念组;
            if(!归类b->全部名称概念组)归类b->全部名称概念组=归类a->全部名称概念组;
            归类a->目标词条参数=static_cast<std::uint32_t>(代表.at(归类a->目标词条参数-1)+1);
            归类b->目标词条参数=static_cast<std::uint32_t>(代表.at(归类b->目标词条参数-1)+1);
        }
        if(a.值!=b.值||a.材料!=b.材料||a.来源任务成员组!=b.来源任务成员组
            ||a.引用需求输入!=b.引用需求输入||a.需求参数输入!=b.需求参数输入||a.初值方式!=b.初值方式||a.完整复制特征!=b.完整复制特征||a.复制词条!=b.复制词条||a.复制特征概念!=b.复制特征概念||归类a!=归类b){ 模拟样例拒绝行=__LINE__; return {}; }
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
    if(执行主体输入)q.预期联合节点组.insert(q.预期联合节点组.end(),执行主体输入->来源身份保护.begin(),执行主体输入->来源身份保护.end());
    q.只读预期=需求图.存在范围语义保护;
    for(const auto& definition:需求图.正式定义保护)
        q.只读预期.节点组.push_back(definition.结构.节点);
    if(归类主体输入) {
        const auto& expected=归类主体输入->身份预期;
        // 仅保留本次引用所需的节点身份，不以采集时的主体字段作为变化门槛。
        q.只读预期.节点组.insert(q.只读预期.节点组.end(),expected.节点组.begin(),expected.节点组.end());
        q.只读预期.节点组.push_back({归类主体输入->来源存在});
        q.只读预期.节点组.push_back({归类主体输入->专属概念});
    }
    const auto 新节点=[&](节点种类 k)->std::size_t {
        const auto n=q.新节点组.size();
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(k)}},
            {字段组[根],模拟世界树根节点}}});return n;
    };
    std::vector<std::size_t> 延后词典特征;
    std::vector<bool> 延后词典初值(输入.size(),false);
    const auto 是Q=[](const 新模拟复制准确值& v){return v.种类==新模拟复制值种类::词典材料引用;};
    for(std::size_t i=0;i<输入.size();++i)if(代表[i]==i&&输入[i].完整复制特征) {
        const auto& f=*输入[i].完整复制特征;
        const auto 域含Q=[&](const auto& d) {
            const auto* values=d ? std::get_if<std::vector<新模拟复制准确值>>(&d->内容) : nullptr;
            return values&&std::any_of(values->begin(),values->end(),是Q);
        };
        if((f.当前值&&是Q(*f.当前值))||(f.标准值&&是Q(*f.标准值))
            ||std::any_of(f.历史不同值.begin(),f.历史不同值.end(),是Q)
            ||std::any_of(f.历史值命中信息.begin(),f.历史值命中信息.end(),[&](const auto& h){return 是Q(h.值);})
            ||域含Q(f.实例区间)||域含Q(f.历史聚合)) {
            if(!s.词典材料||词典材料输入.empty())throw std::logic_error("local dictionary initial material provider unavailable");
            const auto pos=输入[i].所属存在参数;
            if(!pos||pos>输入.size()||输入[代表[pos-1]].规格.信息类型!=新方法参数信息类型::词条)
                throw std::invalid_argument("dictionary initial feature is not owned by a word");
            延后词典初值[i]=true;延后词典特征.push_back(i);
        }
    }
    for(const auto& Q:词典材料输入)
        if(!Q.来源存在输入||Q.来源存在输入>输入.size()||!是普通存在(Q.来源存在输入-1))
            throw std::invalid_argument("copied dictionary source has no local existence input");
    if(!词典材料输入.empty()&&延后词典特征.empty())
        throw std::invalid_argument("copied dictionary materials have no initial feature references");
    const auto 场景=新节点(场景种类);
    std::optional<std::size_t> 词条场景;
    if(std::any_of(输入.begin(),输入.end(),[](const auto& p){return p.规格.信息类型==新方法参数信息类型::词条;})) {
        词条场景=新节点(语言场景种类);
        q.新节点组[*词条场景].字段组.push_back({字段组[所属],基础组内节点引用{场景}});
        q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{*词条场景},基础外部关系类型::父子});
    }
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
    if(归类主体输入) {
        const auto 位置=static_cast<std::size_t>(归类主体输入->同源存在输入-1);
        if(位置>=输入.size() || !是普通存在(位置) || !E节点.contains(代表.at(位置))
            || 输入[位置].来源存在!=std::optional{归类主体输入->来源存在}
            || 构造范围[位置]!=归类主体输入->专属概念){ 模拟样例拒绝行=__LINE__; return {}; }
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
        if(归类参数[i]||是语言范围(i))return 基础组内节点引用{index.at(i)};
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
        if(归类参数[i]) { index[i]=新节点(归类材料种类); continue; }
        if(是语言范围(i)) {
            index[i]=新节点(语言范围种类);
            q.追加外部关系组.push_back({基础组内节点引用{场景},引用(i),基础外部关系类型::父子});
            continue;
        }
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
        } else if(p.规格.信息类型==新方法参数信息类型::词条) {
            index[i]=新节点(词条种类);
            const auto& w=*p.复制词条;
            auto& fields=q.新节点组[index[i]].字段组;
            fields.push_back({字段组[所属],基础组内节点引用{*词条场景}});
            fields.push_back({字段组[词条语言引用],w.语言节点});
            fields.push_back({字段组[词条规范化合同引用],w.规范化合同节点});
            fields.push_back({字段组[词条词面值],基础值{w.规范化词面}});
            q.新节点组[*词条场景].字段组.push_back({字段组[场景成员引用],基础组内节点引用{index[i]}});
        } else {
            if(延后词典初值[i]) {index[i]=q.新节点组.size();q.新节点组.emplace_back();}
            else {
                index[i]=新节点(特征种类);
                const auto 分界槽=语言槽(i);
                const auto C=分界槽 ? 语言前态->特征组[*分界槽].特征概念节点
                    : p.复制特征概念.value_or(p.规格.范围概念节点);
                q.新节点组[index[i]].字段组.push_back({字段组[特征类型],C});
            }
        }
        if(延后词典初值[i])continue; // 仅预留身份；完整F与首次Q持有同组发表。
        auto 所在场景=p.规格.信息类型==新方法参数信息类型::词条 ? *词条场景 : 场景;
        if(p.规格.信息类型==新方法参数信息类型::存在) {
            if(const auto found=独立成员场景.find(i);found!=独立成员场景.end())所在场景=found->second;
        } else if(p.所属存在参数) {
            if(const auto found=独立成员场景.find(代表.at(p.所属存在参数-1));found!=独立成员场景.end())所在场景=found->second;
        }
        q.追加外部关系组.push_back({基础组内节点引用{所在场景},引用(i),基础外部关系类型::父子});
    }
    for(std::size_t i=0;i<输入.size();++i)if(代表[i]!=i)index[i]=index[代表[i]];
    for(std::size_t i=0;i<输入.size();++i) if(归类参数[i] && 代表[i]==i) {
        const auto& r=*输入[i].复制归类材料;
        auto& fields=q.新节点组[index[i]].字段组;
        fields.push_back({字段组[所属],基础组内节点引用{*词条场景}});
        fields.push_back({字段组[内容概念引用],r.范围.材料范围});
        fields.push_back({字段组[归类目标词条引用],字段引用(r.目标词条参数-1)});
        if(r.采用概念)fields.push_back({字段组[归类采用概念引用],*r.采用概念});
        q.追加外部关系组.push_back({基础组内节点引用{*词条场景},基础组内节点引用{index[i]},基础外部关系类型::父子});
    }
    for(const auto& [word,names]:普通名称前态) {
        auto& fields=q.新节点组[index[word]].字段组;
        fields.push_back({字段组[普通名称前态已知],基础值{std::int64_t{1}}});
        for(const auto C:names) {
            const auto name=新节点(模拟普通名称种类);
            q.新节点组[name].字段组.push_back({字段组[归类目标词条引用],基础组内节点引用{index[word]}});
            q.新节点组[name].字段组.push_back({字段组[归类采用概念引用],C});
            q.新节点组[index[word]].字段组.push_back({字段组[普通名称成员],基础组内节点引用{name}});
            q.追加外部关系组.push_back({基础组内节点引用{*词条场景},基础组内节点引用{name},基础外部关系类型::父子});
        }
    }
    for(const auto& [word,names]:全部名称前态) {
        auto& fields=q.新节点组[index[word]].字段组;
        fields.push_back({字段组[全部名称前态已知],基础值{std::int64_t{1}}});
        for(const auto C:names)fields.push_back({字段组[全部名称概念成员],C});
    }
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
    using 复制材料键=std::tuple<稳定编码,材料物理类型,新特征值材料>;
    std::map<复制材料键,std::size_t> 复制材料索引;
    std::map<std::pair<新模拟复制值种类,std::vector<std::uint32_t>>,std::size_t> 复制集合索引;
    std::set<std::pair<std::size_t,稳定编码>> 已保持概念;
    std::set<稳定编码> 正式复制概念;
    const auto 保持集合概念=[&](std::size_t materialIndex,const 新特征值材料& value) {
        const auto* members=std::get_if<std::vector<std::uint64_t>>(&value);
        if(!members)throw std::invalid_argument("simulation concept collection material is not U64 array");
        for(const auto id:*members) {
            const 稳定编码 conceptId{id};
            if(!有效(conceptId))throw std::invalid_argument("simulation concept collection contains invalid identity");
            if(已保持概念.emplace(materialIndex,conceptId).second)
                q.新节点组[materialIndex].字段组.push_back({字段组[集合概念引用],conceptId});
        }
    };
    const auto 复制准确=[&](const 新模拟复制准确值& value,bool conceptSet)->基础组内字段内容 {
        using K=新模拟复制值种类;
        if(conceptSet && value.种类!=K::普通材料)
            throw std::invalid_argument("simulation concept collection requires copied material");
        switch(value.种类) {
        case K::正式概念引用: {
            const auto C=std::get<稳定编码>(value.内容);
            if(!有效(C))throw std::invalid_argument("copied formal concept identity invalid");
            正式复制概念.insert(C);
            q.只读预期.节点组.push_back(基础节点{C});
            return C;
        }
        case K::I64:return 基础值{std::get<std::int64_t>(value.内容)};
        case K::存在引用:return E引用(std::get<std::uint32_t>(value.内容));
        case K::需求引用:return 图引用(D节点,std::get<std::uint32_t>(value.内容));
        case K::普通材料: {
            const auto& v=std::get<新特征值信息>(value.内容);
            if(conceptSet && v.物理类型!=材料物理类型::U64数组)
                throw std::invalid_argument("simulation concept collection physical type differs");
            const 复制材料键 key{v.节点,v.物理类型,v.材料};
            if(有效(v.节点))if(const auto it=复制材料索引.find(key);it!=复制材料索引.end()) {
                if(conceptSet && material.at(it->second)!=材料物理类型::U64数组)
                    throw std::invalid_argument("simulation cached concept collection physical type differs");
                if(conceptSet) {
                    const auto& fields=q.新节点组.at(it->second).字段组;
                    const auto payload=std::find_if(fields.begin(),fields.end(),[](const auto& f) {
                        return f.字段节点==字段组[数值];
                    });
                    const auto* raw=payload!=fields.end() ? std::get_if<基础值>(&payload->内容) : nullptr;
                    const auto* stored=raw ? std::get_if<std::vector<std::uint64_t>>(&raw->材料) : nullptr;
                    if(!stored)throw std::logic_error("simulation cached concept collection payload missing");
                    // 引用对应样例内已保存的材料，不比较也不混入另一时刻的读取。
                    const 新特征值材料 saved{*stored};
                    保持集合概念(it->second,saved);
                }
                return 基础组内节点引用{it->second};
            }
            const auto x=新节点(材料种类);std::visit([&](const auto& raw){q.新节点组[x].字段组.push_back({字段组[数值],基础值{raw}});},v.材料);
            if(conceptSet)保持集合概念(x,v.材料);
            q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{x},基础外部关系类型::父子});
            material.emplace(x,v.物理类型);if(有效(v.节点))复制材料索引.emplace(key,x);return 基础组内节点引用{x};
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
    const auto 复制域=[&](const 新模拟复制值域& v,bool conceptSet) {
        if(const auto* domain=std::get_if<特征概念值域>(&v.内容)) {
            if(std::holds_alternative<std::vector<稳定编码>>(*domain))throw std::invalid_argument("copied finite domain must be typed and remapped");
            if(const auto* x=std::get_if<特征概念存在集合值域>(domain);x&&!x->必含存在节点组.empty())throw std::invalid_argument("copied existence domain contains source identities");
            if(const auto* x=std::get_if<特征概念需求集合值域>(domain);x&&!x->必含需求节点组.empty())throw std::invalid_argument("copied demand domain contains source identities");
            return writer.域(*domain);
        }
        const auto n=writer.节点(3);
        for(const auto& value:std::get<std::vector<新模拟复制准确值>>(v.内容))writer.写(n,结构引用,复制准确(value,conceptSet));
        return n;
    };
    for(std::size_t i=0;i<输入.size();++i) {
        if(代表[i]!=i||输入[i].规格.信息类型!=新方法参数信息类型::特征)continue;
        if(延后词典初值[i])continue;
        const auto& p=输入[i];
        if(语言槽(i)) continue; // 与本地 R、固定材料和其引用一起由下方图写入。
        if(p.所属语言场景输入) {
            if(p.所属存在参数)throw std::invalid_argument("copied feature has two owners");
            q.新节点组[index[i]].字段组.push_back({字段组[所属],图引用(L节点,p.所属语言场景输入)});
        } else if(p.所属存在参数) {
            if(p.所属存在参数>输入.size()) { 模拟样例拒绝行=__LINE__; return {}; }
            const auto holder=代表[p.所属存在参数-1];
            const bool word=输入[holder].规格.信息类型==新方法参数信息类型::词条;
            if(!是普通存在(holder)&&!word){ 模拟样例拒绝行=__LINE__; return {}; }
            q.新节点组[index[i]].字段组.push_back({字段组[所属],字段引用(holder)});
            if(word) q.新节点组[index[holder]].字段组.push_back({字段组[词条特征引用],字段引用(i)});
        } else q.新节点组[index[i]].字段组.push_back({字段组[所属],基础组内节点引用{场景}});
        if(p.初值方式==新模拟特征初值方式::概念构造未知)continue;
        if(p.初值方式!=新模拟特征初值方式::常规输入) {
            const auto definition=s.方法.读取模拟特征范围(p.规格.范围概念节点);
            if(!definition)throw std::runtime_error("simulation copied feature definition unavailable");
            const bool conceptSet=definition->比较规则==特征概念比较规则::概念集合身份判等;
            if(!p.完整复制特征||p.值||p.材料||p.引用存在参数||p.引用需求输入
                ||!p.集合成员参数组.empty()||!p.来源任务成员组.empty())throw std::invalid_argument("copied feature input has conflicting authorities");
            const auto& f=*p.完整复制特征;
            if((p.初值方式==新模拟特征初值方式::复制准确)!=f.当前值.has_value())throw std::invalid_argument("copied feature current mode mismatch");
            if(f.实例单位)writer.写(index[i],结构单位引用,*f.实例单位);
            if(f.实例区间)writer.子(index[i],完整实例区间引用,复制域(*f.实例区间,conceptSet));
            if(f.历史聚合)writer.子(index[i],完整聚合引用,复制域(*f.历史聚合,conceptSet));
            if(f.当前值) {
                const auto v=复制准确(*f.当前值,conceptSet);const auto scalar=std::holds_alternative<基础值>(v);
                writer.写(index[i],scalar ? 数值 : 当前材料,v);
            }
            if(f.标准值)writer.写(index[i],标准值引用,复制准确(*f.标准值,conceptSet));
            for(const auto& h:f.历史不同值) {
                const bool 已有命中=conceptSet && std::any_of(f.历史值命中信息.begin(),f.历史值命中信息.end(),
                    [&](const auto& hit){return hit.值==h;});
                if(!已有命中)writer.写(index[i],历史项引用,复制准确(h,conceptSet));
            }
            for(const auto& h:f.历史值命中信息) {
                if(h.命中次数<0)throw std::invalid_argument("negative copied feature hit count");
                const auto hit=writer.节点();writer.写(hit,结构引用,复制准确(h.值,conceptSet));writer.数(hit,命中次数值,h.命中次数);writer.子(index[i],历史命中引用,hit);
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
            if(定义->比较规则==特征概念比较规则::概念集合身份判等)保持集合概念(v,*材料);
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
    std::map<稳定编码,std::size_t> 语言材料索引;
    if (语言前态) {
        const auto R=index.front();
        const auto& 文本=语言前态->原文;
        const auto& 定位=文本.范围.定位;
        writer.写(R,所属,基础组内节点引用{场景});
        writer.写(R,特征类型,输入.front().规格.范围概念节点);
        writer.写(R,来源引用,定位.前置来源节点);
        writer.写(R,词条语言引用,定位.语言节点);
        writer.写(R,词条规范化合同引用,定位.规范化合同节点);
        writer.写(R,正文引用,文本.固定正文材料);
        writer.写(R,结构材料值,基础值{std::vector<std::uint64_t>{定位.起始标量,定位.结束标量}});
        writer.写(R,固定语义引用,基础值{文本.根UTF8原文});
        writer.写(R,数值,基础值{文本.范围UTF8原文});

        for (const auto& [来源,定义]:语言前态->材料组) {
            const auto N=新节点(语言分界材料种类);
            语言材料索引.emplace(来源,N);
            writer.子(N,所属,R);
            writer.数(N,结构类别值,static_cast<std::int64_t>(定义.index()));
            q.追加外部关系组.push_back({基础组内节点引用{场景},基础组内节点引用{N},基础外部关系类型::父子});
        }
        const auto 写词段=[&](std::size_t 持有者,const std::vector<新语言分界词段>& 词段组) {
            for (std::size_t i=0; i<词段组.size(); ++i) {
                const auto& 词段=词段组[i];
                const auto T=writer.节点(static_cast<std::int64_t>(词段.用途));
                writer.子(持有者,结构子项,T);
                writer.数(T,结构顺序值,static_cast<std::int64_t>(i));
                writer.写(T,结构材料值,基础值{std::vector<std::uint64_t>{词段.起始标量,词段.结束标量}});
                if (词段.词条) writer.写(T,词条特征引用,*词段.词条);
            }
        };
        for (const auto& [来源,定义]:语言前态->材料组) {
            const auto N=语言材料索引.at(来源);
            if (const auto* 候选=std::get_if<新语言分界候选定义>(&定义)) {
                for (std::size_t i=0; i<候选->路径组.size(); ++i) {
                    const auto P=writer.节点();
                    writer.子(N,结构子项,P);
                    writer.数(P,结构顺序值,static_cast<std::int64_t>(i));
                    const auto& 路径=候选->路径组[i];
                    写词段(P,路径.词段组);
                    for (std::size_t k=0; k<路径.规则应用记录组.size(); ++k) {
                        const auto 应用=writer.节点();
                        writer.子(P,场景成员引用,应用);
                        writer.数(应用,结构顺序值,static_cast<std::int64_t>(k));
                        writer.写(应用,来源引用,路径.规则应用记录组[k]);
                    }
                }
            } else if (const auto* 依据=std::get_if<新语言分界依据定义>(&定义)) {
                writer.子(N,结构引用,语言材料索引.at(依据->候选材料));
                写词段(N,依据->取材.已证明词段);
                if (const auto* 规则=std::get_if<新语言规则分界来源>(&依据->取材.来源)) {
                    writer.写(N,来源引用,规则->正式匹配记录);
                } else {
                    const auto& 回答=std::get<新语言回答分界来源>(依据->取材.来源);
                    writer.写(N,目标引用,回答.正式问题);
                    writer.写(N,确认引用,回答.正式回答);
                    writer.写(N,来源引用,回答.分段对齐材料);
                }
            } else if (const auto* 有效=std::get_if<新语言有效分界定义>(&定义)) {
                writer.子(N,结构引用,语言材料索引.at(有效->正式依据));
            } else {
                const auto& 准入=std::get<新语言词条准入定义>(定义);
                writer.子(N,结构引用,语言材料索引.at(准入.有效结果));
                写词段(N,准入.词段组);
            }
        }
        for (std::size_t 槽=0; 槽<语言特征位置.size(); ++槽) {
            const auto F=index[语言特征位置[槽]];
            writer.子(R,场景特征引用,F);
            writer.子(F,所属,R);
            const auto& 前态=语言前态->特征组[槽];
            const auto 本地材料=[&](const 新特征准确值& 值) {
                return 语言材料索引.at(std::get<稳定编码>(值));
            };
            if (前态.实例单位) writer.写(F,结构单位引用,*前态.实例单位);
            if (前态.实例区间值) writer.子(F,完整实例区间引用,writer.域(*前态.实例区间值));
            if (前态.实例历史聚合值域) writer.子(F,完整聚合引用,writer.域(*前态.实例历史聚合值域));
            if (前态.当前值) writer.子(F,当前材料,本地材料(*前态.当前值));
            if (前态.标准值) writer.子(F,标准值引用,本地材料(*前态.标准值));
            for (const auto& 历史:前态.历史不同值) writer.子(F,历史项引用,本地材料(历史));
            for (const auto& 命中:前态.历史值命中信息) {
                const auto H=writer.节点();
                writer.子(H,结构引用,本地材料(命中.值));
                writer.数(H,命中次数值,命中.命中次数);
                writer.子(F,历史命中引用,H);
            }
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
    std::map<稳定编码,稳定编码> 材料来源顺序准备;
    for(const auto& [来源键,索引]:复制材料索引)
        if(!材料来源顺序准备.emplace(图身份.at(索引),std::get<0>(来源键)).second)
            throw std::logic_error("duplicate local material source mapping");
    for (const auto& [来源,槽]:语言材料索引)
        材料来源顺序准备.emplace(图身份.at(槽),来源);
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
    const auto 概念来源已退出=[&]() {
        auto concepts=正式复制概念;
        for(const auto& [material,C]:已保持概念)concepts.insert(C);
        if(concepts.empty())return false;
        const auto read=全局基础数据集.查询节点组完整结构({concepts.begin(),concepts.end()});
        if(read.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(read.状态!=基础结构操作状态::已完成 || read.节点组.size()!=concepts.size())
            throw std::runtime_error("simulation concept source read failed");
        return std::any_of(read.节点组.begin(),read.节点组.end(),[](const auto& item){return !item.快照;});
    };
    const auto 归类主体已退出=[&]() {
        if(!归类主体输入)return false;
        const auto read=全局基础数据集.查询节点组完整结构(
            {归类主体输入->来源存在,归类主体输入->专属概念});
        if(read.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(read.状态!=基础结构操作状态::已完成)throw std::runtime_error("classification actor source read failed");
        return std::any_of(read.节点组.begin(),read.节点组.end(),[](const auto& x){return !x.快照;});
    };
    // 样例关系只依赖已有端点身份；私有写宿主仍使用上面预留的写入合同。
    std::set<稳定编码> 关系端点;
    for(const auto& edge:q.追加外部关系组) {
        if(const auto* n=std::get_if<稳定编码>(&edge.源节点))关系端点.insert(*n);
        if(const auto* n=std::get_if<稳定编码>(&edge.目标节点))关系端点.insert(*n);
    }
    for(const auto id:关系端点) {
        q.窄关系端点组.push_back(id);
        q.只读预期.节点组.push_back(基础节点{id});
    }
    try { 补齐模拟追加联合预期(q); }
    catch(const 模拟联合预期变化&) { return {新模拟世界状态::定义前提已变化,{}}; }
    catch(const std::runtime_error&) {
        if(概念来源已退出() || 归类主体已退出())return {新模拟世界状态::需重新组织输入,{}};
        throw;
    }
    auto p=全局基础数据集.原子追加节点组(q);
    if(p.状态==基础节点组追加状态::预期不符)return {新模拟世界状态::定义前提已变化,{}};
    if((p.状态==基础节点组追加状态::节点不存在 || p.状态==基础节点组追加状态::引用不存在)
        &&(语言前态 || 概念来源已退出() || 归类主体已退出()))return {新模拟世界状态::需重新组织输入,{}};
    if(p.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
    if(p.状态!=基础节点组追加状态::已完成)throw std::runtime_error("generic simulation graph publication failed, status="+std::to_string(static_cast<int>(p.状态)));
    p.新节点组=std::move(图身份);主场景准备.保留=true;图身份准备.保留=true;
    s.通用=true;s.通用方法=M;s.收集追加(p);s.通用场景=p.新节点组[场景];
    s.复制材料来源顺序=std::move(材料来源顺序准备);
    if (语言前态) {
        实现::语言范围索引 范围{p.新节点组[index.front()],语言前态->原文.范围.节点,
            输入.front().规格.范围概念节点,{},语言前态->原文.范围.特征节点组};
        for (std::size_t 槽=0; 槽<语言特征位置.size(); ++槽)
            范围.本地特征[槽]=p.新节点组[index[语言特征位置[槽]]];
        s.本地语言范围=范围;
    }
    s.执行主体来源=执行主体输入;
    s.归类主体来源=归类主体输入;
    if(归类主体输入)s.本地执行主体=E节点.at(代表.at(归类主体输入->同源存在输入-1));
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
    if(词条场景)s.本地语言场景.emplace(p.新节点组[*词条场景],std::nullopt);
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
            ? ((归类参数[i]||是语言范围(i)) ? p.新节点组[index[i]] : 需求参数[i] ? D节点.at(输入[i].需求参数输入-1)
                :场景参数&&代表[i]==*场景参数 ? s.通用场景 : E节点.at(代表[i])) : p.新节点组[index[i]];
        auto 本地参数=输入[i];
        if(是普通存在(i)) {
            本地参数.构造存在概念=构造范围[i];
            if(!本地参数.引用语言场景输入&&读引用(n,特征类型)!=std::optional<稳定编码>{构造范围[i]})
                throw std::runtime_error("模拟存在落图的有效构造范围读回不一致");
        }
        if(归类参数[i])s.本地归类材料.emplace(n,输入[i].复制归类材料->来源材料);
        s.通用参数.emplace(n,std::move(本地参数));
        s.通用输入节点[i]=n;
        if(i>=s.通用绑定.size())continue;
        auto& b=s.通用绑定[i];b.世界场景节点=s.通用场景;
        if(延后词典初值[代表[i]])b.世界场景节点=p.新节点组.at(*词条场景);
        auto owner = 是普通存在(i)||归类参数[i]||是语言范围(i)||输入[i].规格.信息类型==新方法参数信息类型::词条
            ? std::optional<稳定编码>{n} : 读引用(n,所属);
        if(owner&&*owner!=s.通用场景)if(const auto actual=读引用(*owner,所属))b.世界场景节点=*actual;
        b.有序引用=新方法有序参数引用{static_cast<std::uint32_t>(i+1),输入[i].规格.信息类型,n};
    }
    if(!延后词典特征.empty()||(词条场景&&s.词典材料)) {
        const auto nodes=s.通用输入节点;
        const auto language=p.新节点组.at(*词条场景);
        lock.unlock();
        if(!复制词典材料初值(输入,词典材料输入,延后词典特征,nodes,language))
            return {新模拟世界状态::需重新组织输入,{}};
        lock.lock();
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
        const auto F=读取模拟特征内部(b.交互者特征,true);
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
    if (const auto& 本地=s.本地语言范围) {
        const auto 读回=读取本地语言范围参数(本地->本地范围,本地->结构概念);
        if (读回.状态==正式信息参数读取状态::资源失败) return {新模拟世界状态::资源失败,{}};
        if (读回.状态!=正式信息参数读取状态::已读取 || 读回.信息->直接内容组.size()!=4)
            throw std::logic_error("local language range publication readback failed");
    }
    std::vector<std::pair<稳定编码,稳定编码>> 直接需求;
    for(std::size_t i=0;i<m->参数规格组.size();++i)if(需求参数[i])
        直接需求.push_back({s.通用绑定[i].有序引用->信息节点,输入[i].规格.范围概念节点});
    std::vector<std::pair<稳定编码,稳定编码>> 直接归类材料;
    for(std::size_t i=0;i<m->参数规格组.size();++i)if(归类参数[i])
        直接归类材料.push_back({s.通用绑定[i].有序引用->信息节点,输入[i].规格.范围概念节点});
    lock.unlock();
    for(const auto& [D,C]:直接需求) {
        const auto read=读取正式信息参数保护(D,C);
        if(read.状态==正式信息参数读取状态::前提已变化)return {新模拟世界状态::定义前提已变化,{}};
        if(read.状态==正式信息参数读取状态::资源失败)return {新模拟世界状态::资源失败,{}};
        if(read.状态!=正式信息参数读取状态::已读取||!read.信息||read.信息->直接内容组.size()!=15)
            throw std::runtime_error("local formal demand parameter publication readback failed");
    }
    for(const auto& [R,C]:直接归类材料) {
        const auto read=读取正式信息参数保护(R,C);
        if(read.状态==正式信息参数读取状态::信息不存在 || read.状态==正式信息参数读取状态::前提已变化)
            return {新模拟世界状态::定义前提已变化,{}};
        if(read.状态==正式信息参数读取状态::资源失败)return {新模拟世界状态::资源失败,{}};
        if(read.状态!=正式信息参数读取状态::已读取 || !read.信息 || read.信息->直接内容组.size()!=2) {
            std::lock_guard check(s.互斥);
            if(!s.通用 || s.通用场景!=完成场景)return {新模拟世界状态::定义前提已变化,{}};
            throw std::runtime_error("local classification material publication readback failed");
        }
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
        const auto 特征=读取模拟特征内部(p.新节点组[index[参数索引]],true);
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
    建立本地词典接收场景();
    {std::lock_guard ready(s.互斥);s.样例就绪=true;}
    return {新模拟世界状态::已完成,完成场景};
}
bool 新_模拟世界类::复制词典材料初值(const std::vector<新模拟有序参数输入>& 输入,
    const std::vector<新模拟词典材料复制输入>& 材料输入,const std::vector<std::size_t>& 特征位置,
    const std::vector<稳定编码>& 本地输入,稳定编码 L) {
    auto& s=*实现_;
    auto& owner=*s.词典材料;
    // O一旦发表立即接管退出责任；后续任何失败均由样例共同退出处理。
    const auto org=owner.建立或取得材料组织(L,*this);
    if(org.状态==新词典材料操作状态::资源失败)throw std::bad_alloc{};
    if(!org.信息)throw std::logic_error("local dictionary material organization unavailable");
    {std::lock_guard lock(s.互斥);s.词典材料组织=org.信息->节点;
        s.词典材料来源.resize(材料输入.size());
        for(std::size_t i=0;i<材料输入.size();++i)s.词典材料来源[i].second=材料输入[i].材料.节点;
    }
    if(材料输入.empty())return true; // 空F的首次查询也需要已登记退出责任的本地O。
    std::vector<新词典内部材料建立请求> requests;
    requests.reserve(材料输入.size());
    struct 初值参与 final : 新隔离材料初值参与接口 {
        实现& state;
        std::function<新材料结果追加构造(const 新隔离材料组端点&)> build;
        mutable std::size_t nodeStart=0,fieldCount=0,edgeCount=0;
        mutable std::vector<std::variant<稳定编码,基础组内节点引用>> endpoints;
        std::optional<基础片段发布回执> receipt;
        初值参与(实现& s,decltype(build) f):state(s),build(std::move(f)){}
        新材料结果追加构造 构造同组初始引用(const 新隔离材料组端点& points) const override {
            auto result=build(points);
            std::lock_guard lock(state.互斥);
            nodeStart=points.后续新节点起点;
            fieldCount=result.追加请求.追加字段组.size();edgeCount=result.追加请求.追加外部关系组.size();
            endpoints.reserve(points.材料组.size());
            for(const auto& p:points.材料组)endpoints.push_back(p.材料定位);
            state.预留追加(result.追加请求);
            return result;
        }
        void 登记共同发表回执(const 基础片段发布回执& r) noexcept override {
            std::lock_guard lock(state.互斥);
            receipt=r; // 成功先保存回执；所有容量已在提交前预留。
            const auto& nodes=r.新节点组();const auto& fields=r.追加字段关系组();const auto& edges=r.追加外部关系组();
            state.通用节点.insert(state.通用节点.end(),nodes.begin()+nodeStart,nodes.end());
            state.通用字段.insert(state.通用字段.end(),fields.end()-fieldCount,fields.end());
            state.通用外部边.insert(state.通用外部边.end(),edges.end()-edgeCount,edges.end());
            for(std::size_t i=nodeStart;i<r.新节点初始字段关系组().size();++i) {
                const auto& group=r.新节点初始字段关系组()[i];
                state.通用字段.insert(state.通用字段.end(),group.begin(),group.end());
            }
            for(std::size_t i=0;i<endpoints.size();++i)
                state.词典材料来源[i].first=std::visit([&](auto x)->稳定编码 {
                    if constexpr(std::is_same_v<decltype(x),稳定编码>)return x;
                    else return nodes[x.索引];
                },endpoints[i]);
        }
    };
    for(;;) {
        // 零发表后重新读取来源Q，不用固定的失效输入无限重试。
        requests.clear();
        for(const auto& source:材料输入) {
            auto current=owner.读取材料(source.材料.节点);
            if(current.状态==新词典材料操作状态::资源失败)throw std::bad_alloc{};
            if(current.状态==新词典材料操作状态::未找到)return false;
            if(!current.信息)throw std::logic_error("source dictionary material unavailable");
            const auto& q=*current.信息;
            新词典内部材料建立请求 request{org.信息->节点,本地输入.at(source.来源存在输入-1),q.释义语言,
                q.查询键,q.可信批次标识,q.原包标识,q.外设来源,{}};
            request.义项组.reserve(q.义项组.size());
            for(const auto& item:q.义项组)request.义项组.push_back({item.外设消息标识,item.内容});
            requests.push_back(std::move(request));
        }
        auto prepared=owner.准备隔离初值材料组(requests,*this);
        if(prepared.状态==新词典材料操作状态::资源失败)throw std::bad_alloc{};
        if(!prepared.准备)throw std::logic_error("local dictionary initial group preparation failed");
        初值参与 participant(s,[&](const 新隔离材料组端点& points) {
            新材料结果追加构造 result;auto& q=result.追加请求;
            模拟结构写入器 writer{q,0,points.后续新节点起点,s.通用场景};
            const auto value=[&](const 新模拟复制准确值& v)->基础组内字段内容 {
                const auto* ref=std::get_if<新模拟词典材料引用>(&v.内容);
                if(v.种类!=新模拟复制值种类::词典材料引用||!ref||!ref->材料输入||ref->材料输入>points.材料组.size())
                    throw std::invalid_argument("local dictionary initial reference is not a typed group position");
                return std::visit([](auto x)->基础组内字段内容{return x;},points.材料组[ref->材料输入-1].材料定位);
            };
            const auto domain=[&](const 新模拟复制值域& d) {
                if(const auto* type=std::get_if<特征概念值域>(&d.内容)) {
                    if(std::holds_alternative<std::vector<稳定编码>>(*type))
                        throw std::invalid_argument("dictionary finite domain must use typed local references");
                    return writer.域(*type);
                }
                const auto n=writer.节点(3);
                for(const auto& v:std::get<std::vector<新模拟复制准确值>>(d.内容))writer.写(n,结构引用,value(v));
                return n;
            };
            for(const auto pos:特征位置) {
                const auto& in=输入.at(pos);const auto& f=*in.完整复制特征;
                if(in.值||in.材料||in.引用存在参数||in.引用需求输入||!in.集合成员参数组.empty()
                    ||!in.来源任务成员组.empty()||(in.初值方式==新模拟特征初值方式::复制准确)!=f.当前值.has_value()
                    ||(in.初值方式!=新模拟特征初值方式::复制准确&&in.初值方式!=新模拟特征初值方式::复制未知))
                    throw std::invalid_argument("dictionary copied feature has conflicting value authority");
                const auto F=本地输入.at(pos),W=本地输入.at(in.所属存在参数-1);
                const auto put=[&](字段 key,基础组内字段内容 v){q.追加字段组.push_back({F,字段组[key],std::move(v)});};
                put(类型,基础值{static_cast<std::int64_t>(特征种类)});put(根,模拟世界树根节点);
                put(所属,W);put(特征类型,in.规格.范围概念节点);
                q.追加字段组.push_back({W,字段组[词条特征引用],F});
                q.追加外部关系组.push_back({s.通用场景,F,基础外部关系类型::父子});
                if(f.实例单位)put(结构单位引用,*f.实例单位);
                if(f.实例区间)put(完整实例区间引用,基础组内节点引用{domain(*f.实例区间)});
                if(f.历史聚合)put(完整聚合引用,基础组内节点引用{domain(*f.历史聚合)});
                if(f.当前值)put(当前材料,value(*f.当前值));
                if(f.标准值)put(标准值引用,value(*f.标准值));
                // Q历史的唯一承载是命中记录；不另存第二组历史值引用。
                for(const auto& h:f.历史值命中信息) {
                    if(h.命中次数<0)throw std::invalid_argument("negative dictionary history count");
                    const auto n=writer.节点();writer.写(n,结构引用,value(h.值));writer.数(n,命中次数值,h.命中次数);
                    put(历史命中引用,基础组内节点引用{n});
                }
            }
            补齐隔离追加联合预期(q);
            return result;
        });
        const auto published=owner.提交隔离初值材料首次持有(*prepared.准备,participant);
        if(published.状态==基础节点组追加状态::已完成) {
            const auto done=owner.完成隔离初值材料发表(*prepared.准备);
            if(done.状态!=新词典材料操作状态::已完成||!participant.receipt)
                throw std::logic_error("published dictionary initial group completion missing");
            return true;
        }
        owner.取消隔离初值材料准备(*prepared.准备);
        if(published.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
        if(published.状态!=基础节点组追加状态::预期不符&&published.状态!=基础节点组追加状态::引用不存在
            &&published.状态!=基础节点组追加状态::节点不存在)
            throw std::logic_error("local dictionary initial group publication rejected");
        // 本轮零发表才可重新准备；已发表Q/F不会走到这里。
        std::this_thread::yield();
    }
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
                const 正式概念身份读取接口* 提供者[]{内部.特征概念,内部.二次概念,内部.存在概念,内部.动作概念};
                bool 依赖未就绪=false;
                for(const auto* 提供:提供者) {
                    if(!提供) {依赖未就绪=true;continue;}
                    const auto 读取=提供->读取正式概念身份保护(节点);
                    if(读取.状态==正式概念读取状态::前提已变化)return {符合::前提已变化,{}};
                    if(读取.状态==正式概念读取状态::依赖未就绪)依赖未就绪=true;
                    if(读取.身份)return {符合::符合,读取.身份->完整保护组};
                }
                return {依赖未就绪 ? 符合::未支持 : 符合::不符合,{}};
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
    if(!尝试释放有序样例())throw std::runtime_error("simulation temporary nodes still referenced");
}
void 新_模拟世界类::停止样例后继发布() noexcept {
    if(!实现_)return;
    auto& s=*实现_;std::lock_guard lock(s.互斥);
    s.样例就绪=false;s.请求取消=true;s.停止检查={};
}
bool 新_模拟世界类::尝试释放有序样例() {
    if(!实现_)return true;
    auto& s=*实现_;
    bool open=false;
    停止样例后继发布();
    {std::lock_guard lock(s.互斥);if(s.词典在途 || s.归类动作在途 || s.归类观察在途)return false;open=s.通用&&s.记录写回;}
    // 关闭会锁外询问协作停止，不能持world锁调用该回调。
    if(open)(void)结束记录复制上下文读取();
    std::unique_lock lock(s.互斥);
    if(!s.通用)return true;
    if(s.词典材料组织||!s.词典查询责任.empty()||!s.词典接收值组.empty()) {
        const auto O=s.词典材料组织;
        const auto keys=s.词典查询责任;
        const auto values=s.词典接收值组;
        lock.unlock();
        for(;;) {
            std::vector<新词典记录退出准备> records;
            std::set<稳定编码> distinct;
            for(const auto& key:keys) {
                const auto read=s.词典记录->按调用来源读取查询记录(key);
                if(read.状态==新词典记录操作状态::未找到)continue; // 私有查询尚未发表。
                if(read.状态==新词典记录操作状态::资源失败)throw std::bad_alloc{};
                本地词典要求(read.信息.has_value(),"local dictionary query exit identity unavailable");
                if(!distinct.insert(read.信息->查询记录).second)continue;
                auto p=s.词典记录->准备查询记录退出(read.信息->查询记录,新词典记录退出用途::隔离整样例退出,this);
                if(p.状态==新词典记录操作状态::资源失败)throw std::bad_alloc{};
                本地词典要求(p.准备.has_value(),"local dictionary query exit preparation unavailable");
                records.push_back(std::move(*p.准备));
            }
            std::unique_ptr<新词典材料退出准备> material;
            if(O) {
                auto p=s.词典材料->准备隔离材料组织退出(*O,*this);
                if(p.状态==新词典材料操作状态::资源失败)throw std::bad_alloc{};
                本地词典要求(p.准备!=nullptr,"local dictionary exit organization unavailable");
                material=std::move(p.准备);
            }
            基础节点组追加请求 q;
            {std::lock_guard snapshot(s.互斥);
                q.删除节点组=s.通用节点;q.删除字段关系组=s.通用字段;q.删除外部关系组=s.通用外部边;
                s.合并词典回执退出(q);
            }
            const auto current=全局基础数据集.查询节点组完整结构(q.删除节点组);
            if(current.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            本地词典要求(current.状态==基础结构操作状态::已完成,"local dictionary exit graph unavailable");
            for(const auto& n:current.节点组) {
                本地词典要求(n.快照.has_value(),"owned simulation node disappeared before exit");
                q.预期联合节点组.push_back(*n.快照);
            }
            const auto own=全局基础数据集.封存原子追加片段(q);
            if(own.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
            本地词典要求(own.片段.has_value(),"local dictionary exit fragment invalid");
            std::vector<基础封存追加片段> other;
            try {
                for(auto& record:records)other.push_back(s.词典记录->封存查询记录退出(record));
                if(material)other.push_back(s.词典材料->封存材料退出(*material));
            }catch(新词典记录操作状态 status){
                if(status==新词典记录操作状态::资源失败)throw std::bad_alloc{};
                throw std::logic_error("local dictionary record exit sealing rejected");
            }catch(新词典材料操作状态 status){
                if(status==新词典材料操作状态::资源失败)throw std::bad_alloc{};
                throw std::logic_error("local dictionary material exit sealing rejected");
            }
            基础片段组合结果 direct;
            std::shared_ptr<const 基础片段组合结果> held;
            基础节点组追加状态 status;
            const auto offset=values.empty()?std::size_t{1}:std::size_t{2};
            if(values.empty()) {
                std::vector<基础封存追加片段> all{*own.片段};all.insert(all.end(),other.begin(),other.end());
                direct=全局基础数据集.原子追加封存片段组(all);status=direct.状态;
            } else {
                新消息集合退出来源 source{*own.片段,q.删除字段关系组,q.删除外部关系组};
                auto retention=s.词典保持->准备消息集合保持退出(values,*s.词典值,std::move(source));
                status=retention.状态;
                if(retention.准备) {
                    auto result=s.词典保持->提交消息集合保持联合退出(*retention.准备,other);
                    status=result.状态;held=std::move(result.共同回执);
                }
            }
            if(status==基础节点组追加状态::已完成) {
                const auto& receipts=held?held->片段回执组:direct.片段回执组;
                本地词典要求(receipts.size()==offset+records.size()+(material?1:0),"local dictionary joint exit receipts missing");
                // 所有owner先登记成功，再执行索引收尾；任何后续问题都不能重放删除。
                for(std::size_t i=0;i<records.size();++i)s.词典记录->登记查询记录退出发表(records[i],receipts[offset+i]);
                if(material)s.词典材料->登记材料退出发表(*material,receipts[offset+records.size()]);
                for(auto& record:records)s.词典记录->完成查询记录退出(record);
                if(material)s.词典材料->完成材料退出(*material);
                break;
            }
            for(auto& record:records)s.词典记录->取消查询记录退出(record);
            if(material)s.词典材料->取消材料退出(*material);
            if(status==基础节点组追加状态::引用仍存在)return false;
            if(status==基础节点组追加状态::资源失败)throw std::bad_alloc{};
            本地词典要求(status==基础节点组追加状态::预期不符
                ||status==基础节点组追加状态::节点不存在||status==基础节点组追加状态::引用不存在,
                "local dictionary joint exit rejected");
            std::this_thread::yield();
        }
        lock.lock();s.词典材料组织.reset();s.词典材料来源.clear();
    } else {
        基础节点组追加请求 q;
        q.删除节点组=s.通用节点;q.删除字段关系组=s.通用字段;q.删除外部关系组=s.通用外部边;
        s.合并词典回执退出(q);
        const auto r=全局基础数据集.原子删除节点结构(q.删除节点组,q.删除字段关系组,q.删除外部关系组);
        if(r==基础结构操作状态::引用仍存在)return false;
        if(r==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(r!=基础结构操作状态::已完成)throw std::runtime_error("simulation temporary node exit failed");
    }
    s.词典接收定位.reset();s.词典查询责任.clear();s.词典接收值组.clear();
    s.通用=false;s.通用执行分支.reset();s.通用绑定.clear();s.通用输入节点.clear();s.通用参数.clear();s.通用材料.clear();s.复制材料来源顺序.clear();s.通用任务.clear();s.任务成员读取组.clear();
    s.本地语言范围.reset();
    s.本地归类材料.clear();s.本地需求.clear();s.本地理解.clear();s.本地确认.clear();s.本地要求.clear();s.本地语言场景.clear();s.内容参数位置.clear();s.内容读取组.clear();s.归类名称读取组.clear();
    s.记录复制读取=false;s.已读取复制上下文=false;s.本次已进入动作=false;s.本次存在特征读取.clear();
    s.交互事实.clear();s.本次场景读取.clear();
    s.记录写回=false;s.写回窗口已关闭=false;s.最后写入来源组.clear();
    s.当前调用.reset();s.停止检查={};s.请求取消=false;
    s.执行主体来源.reset();s.归类主体来源.reset();s.归类调用组.clear();s.本地执行主体.reset();s.需求动作许可.reset();
    s.消息定义保护={};
    s.词典回执组.clear();s.通用节点.clear();s.通用字段.clear();s.通用外部边.clear();s.通用事件.clear();s.消息事件组.clear();s.消息序号=0;
    auto runtime=std::move(s.词典运行);lock.unlock();runtime.reset();
    return true;
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
    if(!s.通用||!s.样例就绪)throw std::runtime_error("simulation sample unavailable or exiting");
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
    s.已读取复制上下文=false;s.任务成员读取组.clear();s.本次场景读取.clear();s.本次存在特征读取.clear();s.内容读取组.clear();s.归类名称读取组.clear();s.本次已进入动作=false;s.记录复制读取=true;
}
bool 新_模拟世界类::结束记录复制上下文读取() {
    if(!实现_)throw std::runtime_error("simulation owner unavailable");
    (void)请求停止已成立();
    auto& s=*实现_;
    // 与归类动作保持相同的阶段锁顺序，等待本次动作离开跨owner读取和提交阶段后再关闭调用窗口。
    std::lock_guard phase(s.归类动作阶段互斥);
    for(;;) {
    std::unique_lock lock(s.互斥);
    if (!s.记录写回 || !s.记录复制读取)
        throw std::runtime_error("simulation call recording is not active");
    const auto f=s.当前调用 ? 唯一字段(*s.当前调用,调用窗口状态值) : std::nullopt;
    const auto cancel=s.当前调用 ? 唯一字段(*s.当前调用,调用取消值) : std::nullopt;
    if(!f||!cancel)throw std::runtime_error("simulation call close identity missing");
    基础节点组追加请求 q;
    q.替换字段组.push_back({f->编码,基础值{std::int64_t{1}}});
    q.替换字段组.push_back({cancel->编码,基础值{std::int64_t{s.请求取消 ? 1 : 0}}});
    补齐隔离追加联合预期(q);
    const auto published=全局基础数据集.原子追加节点组(q);
    if(published.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
    if(published.状态==基础节点组追加状态::预期不符) {
        lock.unlock();std::this_thread::yield();continue;
    }
    if(published.状态!=基础节点组追加状态::已完成)throw std::runtime_error("simulation call close publication failed");
    s.记录写回=false;s.写回窗口已关闭=true;
    s.记录复制读取=false;s.停止检查={};return s.已读取复制上下文;
    }
}
std::optional<新特征信息> 新_模拟世界类::观察本次归类结果() const {
    if(!实现_)return {};
    auto& s=*实现_;
    std::lock_guard phase(s.归类动作阶段互斥);
    稳定编码 F;
    {
        std::lock_guard lock(s.互斥);
        if(!s.通用 || !s.样例就绪 || s.请求取消 || !s.记录写回 || !s.记录复制读取 || !s.当前调用)return {};
        const auto it=s.归类调用组.find(*s.当前调用);
        if(it==s.归类调用组.end() || !it->second.回执)return {};
        F=it->second.回执->实际输入.挂接特征;
        s.归类观察在途=true;
    }
    struct 观察租约 final {
        实现& 所有者;
        ~观察租约(){std::lock_guard lock(所有者.互斥);所有者.归类观察在途=false;}
    } lease{s};
    // 参数2为正式定位的样例特征；观察结果不作为方法实际读取的隐藏前提。
    return 读取模拟特征内部(F,true);
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
    auto proof=s.方法.读取方法执行结构闭包({method});
    while(proof.状态==新_方法类::执行结构读取状态::前提已变化) {
        {
            std::lock_guard lock(s.互斥);
            if(!s.通用 || method!=s.通用方法 || call!=s.当前调用 || !s.记录写回)
                return 新模拟操作状态::停止取消;
        }
        if(请求停止已成立())return 新模拟操作状态::停止取消;
        proof=s.方法.读取方法执行结构闭包({method});
    }
    std::lock_guard lock(s.互斥);
    if(!s.通用)return 新模拟操作状态::输入不适用;
    // 方法定义读取在样例锁外进行；原调用结束或切换时不向新调用投递旧输入。
    if(method!=s.通用方法||call!=s.当前调用||!s.记录写回)return 新模拟操作状态::停止取消;
    if(proof.状态==新_方法类::执行结构读取状态::方法不存在
        ||proof.状态==新_方法类::执行结构读取状态::来源不是抽象方法)
        return 新模拟操作状态::输入不适用;
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
            (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"模拟存在核验",L"未匹配",t.str());
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
std::optional<新_模拟世界类::本地归类特征概念> 新_模拟世界类::读取本地归类特征概念(稳定编码 C) const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用 || std::find(s.通用节点.begin(),s.通用节点.end(),C)==s.通用节点.end()
        || !属于样例(C,模拟归类特征概念种类,s.通用场景))return {};
    if(读引用(C,所属)!=std::optional{s.通用场景})throw std::logic_error("local classification concept scene invalid");
    const auto root=必读引用(C,内容概念引用),parent=必读引用(C,结构引用),domain=必读引用(C,完整聚合引用);
    if(!属于样例(domain,语义快照种类,s.通用场景))throw std::logic_error("local classification concept domain outside sample");
    const auto value=读结构域(domain);
    const auto materials=std::get_if<std::vector<稳定编码>>(&value);
    if(!materials || materials->empty())throw std::logic_error("local classification concept domain invalid");
    for(const auto id:*materials) {
        const auto it=s.通用材料.find(id);
        if(it==s.通用材料.end() || it->second!=材料物理类型::U64数组
            || !属于样例(id,材料种类,s.通用场景))
            throw std::logic_error("local classification concept material outside sample or wrong type");
    }
    const auto sourceFields=全局基础数据集.查询字段(C,字段组[来源引用]);
    if(sourceFields.size()>1)throw std::logic_error("local classification overlay source duplicated");
    std::optional<稳定编码> source;
    if(!sourceFields.empty()) {
        const auto id=std::get_if<稳定编码>(&sourceFields.front().内容);
        if(!id || !有效(*id))throw std::logic_error("local classification overlay source malformed");
        source=*id;
    }
    return 本地归类特征概念{C,root,parent,domain,*materials,source};
}

std::optional<新_模拟世界类::本地归类特征概念> 新_模拟世界类::查找本地归类概念覆盖(稳定编码 source) const {
    if(!实现_ || !有效(source))return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用)return {};
    std::optional<本地归类特征概念> found;
    for(const auto& field:全局基础数据集.查询目标字段(source,字段组[来源引用])) {
        if(std::find(s.通用节点.begin(),s.通用节点.end(),field.所属节点)==s.通用节点.end()
            || !属于样例(field.所属节点,模拟归类特征概念种类,s.通用场景))continue;
        const auto localConcept=读取本地归类特征概念(field.所属节点);
        if(!localConcept || localConcept->覆盖正式概念!=std::optional{source})
            throw std::logic_error("local classification overlay identity invalid");
        if(found)throw std::logic_error("local classification overlay source duplicated");
        found=std::move(localConcept);
    }
    return found;
}

std::optional<std::vector<稳定编码>> 新_模拟世界类::读取归类混合子概念(稳定编码 parent) const {
    if(!实现_ || !有效(parent))return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用 || !s.样例就绪)return {};
    const auto isLocal=[&](稳定编码 id) {
        return std::find(s.通用节点.begin(),s.通用节点.end(),id)!=s.通用节点.end();
    };
    std::optional<稳定编码> formalParent;
    std::optional<稳定编码> localParent;
    if(isLocal(parent)) {
        const auto local=读取本地归类特征概念(parent);
        if(!local)return {};
        localParent=parent;formalParent=local->覆盖正式概念;
    } else {
        formalParent=parent;
        if(const auto overlay=查找本地归类概念覆盖(parent))localParent=overlay->节点;
    }
    const auto belongs=[&](稳定编码 nodeParent) {
        return (formalParent && nodeParent==*formalParent) || (localParent && nodeParent==*localParent);
    };
    std::set<稳定编码> result;
    if(formalParent)for(const auto& edge:全局基础数据集.查询源关系(*formalParent,基础外部关系类型::父子)) {
        if(const auto overlay=查找本地归类概念覆盖(edge.目标节点)) {
            if(belongs(overlay->父概念))result.insert(overlay->节点);
        } else result.insert(edge.目标节点);
    }
    const auto addLocal=[&](稳定编码 nodeParent) {
        for(const auto& field:全局基础数据集.查询目标字段(nodeParent,字段组[结构引用])) {
            // 根覆盖以正式根作为锚点，不是自己的子概念。
            if(localParent && field.所属节点==*localParent)continue;
            if(!isLocal(field.所属节点) || !属于样例(field.所属节点,模拟归类特征概念种类,s.通用场景))continue;
            const auto local=读取本地归类特征概念(field.所属节点);
            if(!local || !belongs(local->父概念))throw std::logic_error("local classification mixed child invalid");
            result.insert(local->节点);
        }
    };
    if(formalParent)addLocal(*formalParent);
    if(localParent && localParent!=formalParent)addLocal(*localParent);
    return std::vector<稳定编码>{result.begin(),result.end()};
}

std::optional<新特征匹配定义快照> 新_模拟世界类::读取本地特征实际定义(稳定编码 F) const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    const auto p=s.通用参数.find(F);
    if(!s.通用 || p==s.通用参数.end() || p->second.规格.信息类型!=新方法参数信息类型::特征)return {};
    const bool 分类特征=s.归类主体来源 && s.通用绑定.size()>=2
        && s.通用绑定[1].有序引用
        && s.通用绑定[1].有序引用->信息类型==新方法参数信息类型::特征
        && s.通用绑定[1].有序引用->信息节点==F;
    if(!分类特征) {
        if (s.本地语言范围 && std::find(s.本地语言范围->本地特征.begin(),
                s.本地语言范围->本地特征.end(),F)!=s.本地语言范围->本地特征.end())
            return s.方法.读取模拟特征范围(必读引用(F,特征类型));
        return s.方法.读取模拟特征范围(p->second.规格.范围概念节点);
    }
    const auto actual=读引用(F,特征类型);
    if(!actual)throw std::logic_error("local classification feature concept missing");
    std::optional<本地归类特征概念> local;
    if(std::find(s.通用节点.begin(),s.通用节点.end(),*actual)!=s.通用节点.end()) {
        local=读取本地归类特征概念(*actual);
        if(!local)throw std::logic_error("local classification feature concept invalid");
    } else local=查找本地归类概念覆盖(*actual);
    if(local) {
        auto root=s.方法.读取模拟特征范围(local->类型根);
        if(!root || root->特征类型根!=local->类型根
            || root->材料类型!=特征概念材料物理类型::U64数组
            || root->比较规则!=特征概念比较规则::概念集合身份判等)
            throw std::logic_error("local classification feature root unavailable");
        root->概念节点=local->节点;root->值域=local->值域材料;
        return root;
    }
    return s.方法.读取模拟特征范围(*actual);
}

新_模拟世界类::本地归类名称准备 新_模拟世界类::准备本地归类名称(
    稳定编码 W,稳定编码 C,稳定编码 F,基础节点组追加请求& q) const {
    if(!实现_ || !有效(C))throw std::invalid_argument("local classification name input invalid");
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用 || !s.是本地参数(W,新方法参数信息类型::词条))return {};
    // 从词条开始按批次扩展，避免批次外逐个读取会变化的名称节点。
    std::set<稳定编码> wanted{W};
    std::vector<新模拟普通名称信息> names;
    std::vector<稳定编码> all;
    稳定编码 scene;
    for(;;) {
        const auto batch=全局基础数据集.查询节点组完整结构({wanted.begin(),wanted.end()});
        if(batch.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(batch.状态!=基础结构操作状态::已完成 || batch.节点组.size()!=wanted.size())
            throw std::logic_error("local classification name batch unavailable");
        const auto find=[&](稳定编码 id)->const 基础联合节点快照* {
            const auto it=std::find_if(batch.节点组.begin(),batch.节点组.end(),
                [&](const auto& item){return item.查询节点==id;});
            return it==batch.节点组.end() || !it->快照 ? nullptr : &*it->快照;
        };
        const auto word=find(W);if(!word)return {};
        const auto fields=[&](const 基础联合节点快照& node,字段 key) {
            std::vector<基础字段关系> result;
            for(const auto& f:node.结构.字段组)if(f.字段节点==字段组[key])result.push_back(f);
            return result;
        };
        const auto oneRef=[&](const 基础联合节点快照& node,字段 key)->std::optional<稳定编码> {
            const auto fs=fields(node,key);
            if(fs.size()!=1)return {};
            const auto id=std::get_if<稳定编码>(&fs.front().内容);
            return id && 有效(*id) ? std::optional{*id} : std::nullopt;
        };
        const auto oneNum=[&](const 基础联合节点快照& node,字段 key)->std::optional<std::int64_t> {
            const auto fs=fields(node,key);if(fs.size()!=1)return {};
            const auto raw=std::get_if<基础值>(&fs.front().内容);
            const auto value=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
            return value ? std::optional{*value} : std::nullopt;
        };
        if(oneNum(*word,普通名称前态已知)!=1 || oneNum(*word,全部名称前态已知)!=1)return {};
        const auto sceneId=oneRef(*word,所属);if(!sceneId)return {};scene=*sceneId;
        std::vector<稳定编码> ids;all.clear();
        for(const auto key:{普通名称成员,全部名称概念成员})for(const auto& f:fields(*word,key)) {
            const auto id=std::get_if<稳定编码>(&f.内容);
            if(!id || !有效(*id))throw std::logic_error("local classification name batch reference invalid");
            (key==普通名称成员 ? ids : all).push_back(*id);
        }
        std::sort(ids.begin(),ids.end());std::sort(all.begin(),all.end());
        if(std::adjacent_find(ids.begin(),ids.end())!=ids.end()
            || std::adjacent_find(all.begin(),all.end())!=all.end())
            throw std::logic_error("local classification name batch duplicates");
        const auto before=wanted.size();wanted.insert(ids.begin(),ids.end());
        if(wanted.size()!=before)continue;

        const auto bound=fields(*word,词条特征引用);
        if(std::none_of(bound.begin(),bound.end(),[&](const auto& f){
            const auto id=std::get_if<稳定编码>(&f.内容);return id && *id==F;
        }))return {};
        names.clear();std::set<稳定编码> concepts;
        for(const auto id:ids) {
            const auto node=find(id);if(!node)return {};
            if(oneNum(*node,类型)!=模拟普通名称种类 || oneRef(*node,根)!=std::optional{模拟世界树根节点}
                || oneRef(*node,归类目标词条引用)!=std::optional{W})
                throw std::logic_error("local classification name batch identity invalid");
            const auto nameConcept=oneRef(*node,归类采用概念引用);
            if(!nameConcept || !concepts.insert(*nameConcept).second)
                throw std::logic_error("local classification name batch concept invalid");
            基础只读外部关系集合预期 edges{id,false,基础外部关系类型::父子,{},{},{}};
            for(const auto& edge:node->入向外部关系组)if(edge.类型==基础外部关系类型::父子)edges.关系组.push_back(edge);
            if(edges.关系组.size()!=1 || edges.关系组.front().源节点!=scene)return {};
            names.push_back({*nameConcept,id});
        }
        for(const auto& item:names)if(!std::binary_search(all.begin(),all.end(),item.概念节点))
            throw std::logic_error("local classification ordinary name missing from all names");
        break;
    }
    if(s.记录复制读取 && s.归类主体来源) {
        新隔离归类名称读取 trace;trace.词条=W;trace.全部名称概念组=all;
        for(const auto& item:names)trace.普通名称概念组.push_back(item.概念节点);
        trace.前序内容读取数=static_cast<std::uint64_t>(s.内容读取组.size());
        trace.动作前=!s.本次已进入动作;
        const auto it=s.当前调用 ? s.归类调用组.find(*s.当前调用) : s.归类调用组.end();
        trace.归类结果已提交=it!=s.归类调用组.end() && it->second.回执.has_value();
        s.归类名称读取组.push_back(std::move(trace));
    }
    const auto found=std::find_if(names.begin(),names.end(),[&](const auto& item){return item.概念节点==C;});
    if(found!=names.end())return {true,false,基础组内字段内容{found->名称节点},{}};
    本地归类名称准备 result;result.已取得=true;result.新建名称=true;
    result.目标概念组.reserve(all.size()+1);
    for(const auto item:all)result.目标概念组.push_back(item.值);
    result.目标概念组.push_back(C.值);
    result.目标概念组=规范归类签名(std::move(result.目标概念组));
    const auto slot=q.新节点组.size();
    q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(模拟普通名称种类)}},
        {字段组[根],模拟世界树根节点},{字段组[归类目标词条引用],W},{字段组[归类采用概念引用],C}}});
    q.当前追加字段键组.push_back({W,字段组[普通名称成员]});
    q.追加字段组.push_back({W,字段组[普通名称成员],基础组内节点引用{slot}});
    if(!std::binary_search(all.begin(),all.end(),C)) {
        q.当前追加字段键组.push_back({W,字段组[全部名称概念成员]});
        q.追加字段组.push_back({W,字段组[全部名称概念成员],C});
    }
    q.只读预期.节点组.push_back(基础节点{scene});
    q.窄关系端点组.push_back(scene);
    q.追加外部关系组.push_back({scene,基础组内节点引用{slot},基础外部关系类型::父子});
    result.名称引用=基础组内字段内容{基础组内节点引用{slot}};
    return result;
}

新_模拟世界类::本地归类历史准备 新_模拟世界类::准备本地归类历史更新(
    稳定编码 F,const 基础组内字段内容& 新材料,基础节点组追加请求& q,
    const 基础节点结构& 特征快照,const std::vector<基础联合节点读取项>& 命中批次) const {
    if(!实现_)throw std::logic_error("local classification history owner unavailable");
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用 || !s.是本地参数(F,新方法参数信息类型::特征))
        throw std::invalid_argument("local classification history feature unavailable");
    const auto target=std::get_if<稳定编码>(&新材料);
    if(!target && !std::holds_alternative<基础组内节点引用>(新材料))
        throw std::invalid_argument("local classification history material reference invalid");
    if(target && !有效(*target))throw std::invalid_argument("local classification history material invalid");
    const auto fields=[&](const 基础节点结构& node,字段 key) {
        std::vector<基础字段关系> out;
        for(const auto& field:node.字段组)if(field.字段节点==字段组[key])out.push_back(field);
        return out;
    };
    if(特征快照.节点.编码!=F)throw std::logic_error("classification history feature snapshot differs");
    struct 历史记录 final {std::int64_t 次数;std::optional<稳定编码> 次数字段;std::optional<稳定编码> 命中节点;};
    std::map<稳定编码,历史记录> history;
    for(const auto& hitField:fields(特征快照,历史命中引用)) {
        const auto node=std::get_if<稳定编码>(&hitField.内容);
        if(!node)throw std::logic_error("classification history reference invalid");
        const auto it=std::find_if(命中批次.begin(),命中批次.end(),[&](const auto& item){return item.查询节点==*node;});
        if(it==命中批次.end() || !it->快照)throw std::logic_error("classification history snapshot missing");
        const auto values=fields(it->快照->结构,结构引用),counts=fields(it->快照->结构,命中次数值);
        if(values.size()!=1 || counts.size()!=1)throw std::logic_error("classification history snapshot malformed");
        const auto value=std::optional{values.front()},count=std::optional{counts.front()};
        const auto id=value ? std::get_if<稳定编码>(&value->内容) : nullptr;
        const auto raw=count ? std::get_if<基础值>(&count->内容) : nullptr;
        const auto num=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
        if(!id || !有效(*id) || !num || *num<=0 || !history.emplace(*id,历史记录{*num,count->编码,*node}).second)
            throw std::logic_error("local classification history record corrupt");
    }
    本地归类历史准备 result;
    const auto addRecord=[&](基础组内字段内容 value,std::int64_t count) {
        const auto index=q.新节点组.size();
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(语义快照种类)}},
            {字段组[根],模拟世界树根节点},{字段组[结构引用],value},{字段组[命中次数值],基础值{count}}}});
        q.当前追加字段键组.push_back({F,字段组[历史命中引用]});
        q.追加字段组.push_back({F,字段组[历史命中引用],基础组内节点引用{index}});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{index},基础外部关系类型::父子});
    };
    std::set<稳定编码> direct;
    for(const auto key:{历史引用,历史项引用})for(const auto& field:fields(特征快照,key)) {
        const auto id=std::get_if<稳定编码>(&field.内容);
        if(!id || !有效(*id))throw std::logic_error("local classification direct history corrupt");
        direct.insert(*id);
    }
    for(const auto id:direct)if(!history.contains(id)) {
        const auto count=std::int64_t{target && *target==id ? 2 : 1};
        history.emplace(id,历史记录{count,std::nullopt,std::nullopt});
        addRecord(id,count);
    }
    // 单概念历史由命中记录读回，不为已有命中再复制一份历史项。
    if(target && history.contains(*target)) {
        auto& active=history.at(*target);
        if(active.次数字段) {
            if(!active.命中节点)throw std::logic_error("classification hit owner missing");
            q.当前整数递增组.push_back({*active.命中节点,字段组[命中次数值]});
            // 本次读取只用于标准值选择；递增及饱和由L1对写入时的当前值处理。
            if(active.次数!=(std::numeric_limits<std::int64_t>::max)())++active.次数;
        }
        result.新命中次数=active.次数;
    } else {
        result.新命中次数=1;
        addRecord(新材料,1);
    }
    const auto standardFields=fields(特征快照,标准值引用);
    if(standardFields.size()>1)throw std::logic_error("local classification multiple standard values");
    const auto standard=standardFields.empty() ? std::optional<基础字段关系>{}
        : std::optional<基础字段关系>{standardFields.front()};
    bool change=!standard;
    if(standard) {
        const auto old=std::get_if<稳定编码>(&standard->内容);
        if(!old || !history.contains(*old))throw std::logic_error("local classification standard not in history");
        if(!target || *old!=*target)change=result.新命中次数>history.at(*old).次数;
    }
    if(change) {
        q.当前字段赋值组.push_back({F,字段组[标准值引用],新材料});
        result.标准值改变=true;
    }
    return result;
}

std::optional<新_模拟世界类::本地归类候选> 新_模拟世界类::准备本地归类候选(
    稳定编码 W,稳定编码 C,稳定编码 N,稳定编码 A,稳定编码 F,
    std::unique_lock<std::recursive_mutex>& lock,bool& 已观察停止) const {
    if(!实现_)throw std::logic_error("local classification candidate owner unavailable");
    auto& s=*实现_;
    if(!lock.owns_lock() || lock.mutex()!=&s.互斥)
        throw std::logic_error("local classification candidate requires sample lock");
    if(!s.通用 || !s.归类动作在途 || !s.是本地参数(W,新方法参数信息类型::词条)
        || !s.是本地参数(F,新方法参数信息类型::特征))return {};
    // 动作与调用的实际字段由恢复锁后的同批读取判断，不遍历既有动作结果。
    if(s.当前调用!=std::optional{N})return {};
    std::optional<稳定编码> 已读正式概念;
    std::optional<新特征匹配定义快照> 已读正式定义;
重新读取归类特征:
    std::set<稳定编码> fWanted{F};
    基础联合节点查询结果 fBatch;
    for(;;) {
        fBatch=全局基础数据集.查询节点组完整结构({fWanted.begin(),fWanted.end()});
        if(fBatch.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(fBatch.状态!=基础结构操作状态::已完成 || fBatch.节点组.size()!=fWanted.size())
            throw std::logic_error("local classification feature batch unavailable");
        const auto current=std::find_if(fBatch.节点组.begin(),fBatch.节点组.end(),
            [&](const auto& item){return item.查询节点==F;});
        if(current==fBatch.节点组.end() || !current->快照)return {};
        const auto before=fWanted.size();
        std::optional<稳定编码> batchConcept;
        for(const auto& field:current->快照->结构.字段组)if(field.字段节点==字段组[特征类型]) {
            const auto ref=std::get_if<稳定编码>(&field.内容);
            if(batchConcept || !ref)return {};batchConcept=*ref;
        }
        if(!batchConcept)return {};
        for(const auto& field:current->快照->结构.字段组) {
            if(field.字段节点!=字段组[历史命中引用] && field.字段节点!=字段组[当前材料]
                && field.字段节点!=字段组[历史引用] && field.字段节点!=字段组[历史项引用]
                && field.字段节点!=字段组[标准值引用] && field.字段节点!=字段组[完整实例区间引用]
                && field.字段节点!=字段组[完整聚合引用])continue;
            const auto id=std::get_if<稳定编码>(&field.内容);
            if(!id || !有效(*id))throw std::logic_error("local classification history reference invalid");
            fWanted.insert(*id);
            if(field.字段节点==字段组[历史命中引用]) {
                const auto hit=std::find_if(fBatch.节点组.begin(),fBatch.节点组.end(),
                    [&](const auto& item){return item.查询节点==*id;});
                if(hit==fBatch.节点组.end())continue;
                if(!hit->快照)return {};
                for(const auto& h:hit->快照->结构.字段组)if(h.字段节点==字段组[结构引用]) {
                    const auto value=std::get_if<稳定编码>(&h.内容);
                    if(!value || !有效(*value))throw std::logic_error("local classification hit value invalid");
                    fWanted.insert(*value);
                }
            }
        }
        if(fWanted.size()==before)break;
    }
    const auto fItem=std::find_if(fBatch.节点组.begin(),fBatch.节点组.end(),
        [&](const auto& item){return item.查询节点==F;});
    const auto& fSnapshot=*fItem->快照;
    const auto snapshotRef=[&](字段 key)->std::optional<稳定编码> {
        const 基础字段关系* one=nullptr;
        for(const auto& field:fSnapshot.结构.字段组)if(field.字段节点==字段组[key]) {
            if(one)return {};one=&field;
        }
        const auto id=one ? std::get_if<稳定编码>(&one->内容) : nullptr;
        return id ? std::optional{*id} : std::nullopt;
    };
    const auto actualConcept=snapshotRef(特征类型);
    if(!actualConcept || snapshotRef(所属)!=std::optional{W})return {};
    const auto formalConcept=*actualConcept;
    if(已读正式概念!=std::optional{formalConcept}) {
        lock.unlock();
        已读正式定义=s.方法.读取模拟特征范围(formalConcept);
        lock.lock();
        if(!s.通用 || !s.样例就绪 || s.请求取消 || !s.归类动作在途
            || s.当前调用!=std::optional{N} || !已读正式定义)return {};
        已读正式概念=formalConcept;
        goto 重新读取归类特征;
    }
    const auto currentDefinition=已读正式定义;
    if(currentDefinition->材料类型!=特征概念材料物理类型::信息节点引用
        || !currentDefinition->概念身份范围
        || currentDefinition->比较规则!=特征概念比较规则::信息引用身份精确)
        throw std::logic_error("classification feature is not a formal concept reference");
    const auto invocation=s.归类调用组.find(N);
    if(invocation==s.归类调用组.end() || invocation->second.动作!=std::optional{A}
        || invocation->second.输入.词条!=W || invocation->second.输入.挂接特征!=F
        || invocation->second.输入.采用概念!=C || invocation->second.回执)
        throw std::logic_error("local classification candidate invocation identity invalid");
    const auto currentFeature=读取模拟特征内部(F,false,nullptr,&*currentDefinition,&fSnapshot.结构,&fBatch.节点组);
    if(!currentFeature)return {};
    const auto& feature=*currentFeature;
    本地归类候选 candidate;
    // 只登记本次写入实际引用的已有节点身份；具体字段值仍仅来自原读取批次。
    const auto finishCandidate=[&]() {
        auto& q=candidate.请求;
        std::sort(q.当前追加字段键组.begin(),q.当前追加字段键组.end(),[](const auto& a,const auto& b) {
            return std::tie(a.所属节点,a.字段节点)<std::tie(b.所属节点,b.字段节点);
        });
        q.当前追加字段键组.erase(std::unique(q.当前追加字段键组.begin(),q.当前追加字段键组.end(),[](const auto& a,const auto& b) {
            return a.所属节点==b.所属节点 && a.字段节点==b.字段节点;
        }),q.当前追加字段键组.end());
        // 旧写入口仍要求实际写键的字段集合；其余读取结果不作为事后变化门槛。
        std::set<std::pair<稳定编码,稳定编码>> keys;
        for(const auto& key:q.窄写字段键组)keys.emplace(key.所属节点,key.字段节点);
        std::map<std::pair<稳定编码,稳定编码>,基础只读字段集合预期> required;
        for(const auto& selector:q.只读预期.字段集合组) {
            if(!selector.字段节点 || !selector.排除字段键组.empty())continue;
            const auto key=std::make_pair(selector.节点,*selector.字段节点);
            if(keys.contains(key))required.try_emplace(key,selector);
        }
        q.只读预期.字段集合组.clear();
        for(auto& [key,selector]:required)q.只读预期.字段集合组.push_back(std::move(selector));
        q.只读预期.外部关系集合组.clear();
        q.只读预期.节点组.clear();
        q.窄关系端点组.clear();
        std::set<稳定编码> ids;
        for(const auto& [host,key]:keys){ids.insert(host);ids.insert(key);}
        for(const auto& key:q.当前追加字段键组) {
            ids.insert(key.所属节点);ids.insert(key.字段节点);
        }
        for(const auto node:q.删除节点组)ids.insert(node);
        const auto content=[&](const auto& value) {
            if(const auto ref=std::get_if<稳定编码>(&value))ids.insert(*ref);
        };
        for(const auto& node:q.新节点组)for(const auto& field:node.字段组) {
            ids.insert(field.字段节点);content(field.内容);
        }
        for(const auto& field:q.追加字段组) {
            ids.insert(field.所属节点);ids.insert(field.字段节点);content(field.内容);
        }
        for(const auto& field:q.当前字段赋值组) {
            ids.insert(field.所属节点);ids.insert(field.字段节点);content(field.内容);
        }
        for(const auto& field:q.当前整数递增组) {
            ids.insert(field.所属节点);ids.insert(field.字段节点);
        }
        std::set<稳定编码> oldFields(q.删除字段关系组.begin(),q.删除字段关系组.end());
        for(const auto& field:q.替换字段组) {
            oldFields.insert(field.字段关系);content(field.新内容);
        }
        for(const auto& selector:q.只读预期.字段集合组)for(const auto& field:selector.字段组)
            if(oldFields.contains(field.编码)) {
                ids.insert(field.所属节点);ids.insert(field.字段节点);content(field.内容);
            }
        const auto endpoint=[&](const auto& value) {
            if(const auto ref=std::get_if<稳定编码>(&value)) {
                ids.insert(*ref);q.窄关系端点组.push_back(*ref);
            }
        };
        for(const auto& relation:q.追加外部关系组) {
            endpoint(relation.源节点);endpoint(relation.目标节点);
        }
        for(const auto id:ids)q.只读预期.节点组.push_back(基础节点{id});
        // 调用方继续持有样例锁至提交；在事实发表前预留清理账本容量和写入来源槽。
        s.预留追加(q);
        if(candidate.值改变)(void)s.准备写回槽(F);
    };
    candidate.待填回执.实际输入=invocation->second.输入;
    candidate.待填回执.动作=A;
    candidate.待填回执.名称关系组.reserve(1);
    candidate.名称=准备本地归类名称(W,C,F,candidate.请求);
    if(!candidate.名称.已取得)return {};
    // 恢复样例锁后按同一批次取得实际动作与调用身份，避免沿用释放锁之前的读数。
    const auto actionBatch=全局基础数据集.查询节点组完整结构({A,N,W});
    if(actionBatch.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(actionBatch.状态!=基础结构操作状态::已完成)return {};
    const auto snap=[&](稳定编码 id)->const 基础联合节点快照* {
        const auto it=std::find_if(actionBatch.节点组.begin(),actionBatch.节点组.end(),
            [&](const auto& row){return row.查询节点==id;});
        return it==actionBatch.节点组.end() || !it->快照 ? nullptr : &*it->快照;
    };
    const auto actionSnap=snap(A), callSnap=snap(N), wordSnap=snap(W);
    if(!actionSnap || !callSnap || !wordSnap)return {};
    const auto refIs=[&](const 基础联合节点快照& source,字段 key,稳定编码 value) {
        const 基础字段关系* found=nullptr;
        for(const auto& field:source.结构.字段组)if(field.字段节点==字段组[key]) {
            if(found)return false;found=&field;
        }
        const auto ref=found ? std::get_if<稳定编码>(&found->内容) : nullptr;
        return ref && *ref==value;
    };
    const auto numberIs=[&](const 基础联合节点快照& source,字段 key,std::int64_t value) {
        const 基础字段关系* found=nullptr;
        for(const auto& field:source.结构.字段组)if(field.字段节点==字段组[key]) {
            if(found)return false;found=&field;
        }
        const auto raw=found ? std::get_if<基础值>(&found->内容) : nullptr;
        const auto number=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
        return number && *number==value;
    };
    if(!refIs(*actionSnap,来源引用,N) || !refIs(*actionSnap,所属,W)
        || !refIs(*actionSnap,状态特征引用,F) || !refIs(*callSnap,调用主动作引用,A)
        || !numberIs(*actionSnap,类型,动作种类) || !refIs(*actionSnap,根,模拟世界树根节点)
        || !numberIs(*wordSnap,类型,词条种类))return {};
    基础只读外部关系集合预期 actionParents{A,false,基础外部关系类型::父子,{},{},{}};
    for(const auto& edge:actionSnap->入向外部关系组)
        if(edge.类型==基础外部关系类型::父子)actionParents.关系组.push_back(edge);
    if(actionParents.关系组.size()!=1 || actionParents.关系组.front().源节点!=s.通用场景)return {};
    const auto prepareProgress=[&]() {
        auto& q=candidate.请求;
        const bool writes=!q.新节点组.empty() || !q.追加字段组.empty() || !q.替换字段组.empty()
            || !q.追加外部关系组.empty() || !q.删除字段关系组.empty()
            || !q.删除节点组.empty() || !q.删除外部关系组.empty()
            || !q.当前字段赋值组.empty() || !q.当前整数递增组.empty();
        if(!writes || !s.记录写回)return;
        q.当前整数递增组.push_back({N,字段组[调用进展值]});
    };
    candidate.待填回执.新增名称=candidate.名称.新建名称;
    if(!candidate.名称.名称引用)throw std::logic_error("local classification name candidate missing");
    if(const auto id=std::get_if<稳定编码>(&*candidate.名称.名称引用))
        candidate.待填回执.名称关系组.push_back(*id);
    else if(std::holds_alternative<基础组内节点引用>(*candidate.名称.名称引用))
        candidate.待填回执.名称关系组.push_back({}); // 仅供发表后按原子结果槽填回。
    else throw std::logic_error("local classification name candidate type invalid");
    if(!candidate.名称.新建名称) {candidate.概念分支已准备=true;prepareProgress();finishCandidate();return candidate;}
    candidate.目标材料=基础组内字段内容{C};
    candidate.值改变=feature.当前值!=std::optional<新特征准确值>{新特征准确值{C}};
    candidate.概念分支已准备=true;
    candidate.请求.当前字段赋值组.push_back({feature.节点,字段组[当前材料],C});
    const auto history=准备本地归类历史更新(feature.节点,*candidate.目标材料,candidate.请求,fSnapshot.结构,fBatch.节点组);
    candidate.删除的本地字段.insert(candidate.删除的本地字段.end(),
        history.删除的本地字段.begin(),history.删除的本地字段.end());
    if(!candidate.值改变) {prepareProgress();finishCandidate();return candidate;}
    // 正式owner读取已结束；调用方继续持样例锁，按最新状态准备本次状态与动态。
    if(s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())
        throw std::overflow_error("local classification time exhausted");
    candidate.新步骤=s.模拟步骤+1;
    // 跨领域读取已结束，当前持样例锁直到发表；状态选择使用此时读取的材料。
    {
        // 以F的入向状态特征引用扩批，再从同一批解码并选择最新状态。
        std::set<稳定编码> wanted{F};
        基础联合节点查询结果 batch;
        std::set<稳定编码> sources;
        for(;;) {
            batch=全局基础数据集.查询节点组完整结构({wanted.begin(),wanted.end()});
            if(batch.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(batch.状态!=基础结构操作状态::已完成 || batch.节点组.size()!=wanted.size())
                throw std::logic_error("classification state batch unavailable");
            const auto f=std::find_if(batch.节点组.begin(),batch.节点组.end(),[&](const auto& item){return item.查询节点==F;});
            if(f==batch.节点组.end() || !f->快照)return {};
            sources.clear();
            for(const auto& ref:f->快照->入向字段组)if(ref.字段节点==字段组[状态特征引用]
                && std::find(s.通用节点.begin(),s.通用节点.end(),ref.所属节点)!=s.通用节点.end())sources.insert(ref.所属节点);
            const auto before=wanted.size();wanted.insert(sources.begin(),sources.end());
            if(wanted.size()==before)break;
        }
        const auto currentRow=std::find_if(batch.节点组.begin(),batch.节点组.end(),
            [&](const auto& item){return item.查询节点==F;});
        if(currentRow==batch.节点组.end() || !currentRow->快照)return {};
        std::optional<稳定编码> old;
        for(const auto& field:currentRow->快照->结构.字段组) {
            if(field.字段节点!=字段组[当前材料])continue;
            if(const auto* value=std::get_if<稳定编码>(&field.内容))old=*value;
            break;
        }
        std::optional<稳定编码> latest;
        std::optional<std::int64_t> latestTime;
        std::optional<新特征准确值> latestValue;
        bool 同时刻不唯一=false,最新值无法解释=false;
        for(const auto node:sources) {
            const auto row=std::find_if(batch.节点组.begin(),batch.节点组.end(),[&](const auto& item){return item.查询节点==node;});
            if(row==batch.节点组.end() || !row->快照)return {};
            const auto& snap=*row->快照;
            const auto fields=[&](字段 key) {
                std::vector<基础字段关系> out;
                for(const auto& f:snap.结构.字段组)if(f.字段节点==字段组[key])out.push_back(f);
                return out;
            };
            const auto number=[&](字段 key)->std::optional<std::int64_t> {
                const auto fs=fields(key);if(fs.size()!=1)return {};
                const auto raw=std::get_if<基础值>(&fs.front().内容);
                const auto value=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
                return value ? std::optional{*value} : std::nullopt;
            };
            const auto ref=[&](字段 key)->std::optional<稳定编码> {
                const auto fs=fields(key);if(fs.size()!=1)return {};
                const auto value=std::get_if<稳定编码>(&fs.front().内容);
                return value ? std::optional{*value} : std::nullopt;
            };
            if(number(类型)!=状态种类)continue;
            if(ref(根)!=std::optional{模拟世界树根节点})continue;
            基础只读外部关系集合预期 parents{node,false,基础外部关系类型::父子,{},{},{}};
            for(const auto& edge:snap.入向外部关系组)if(edge.类型==基础外部关系类型::父子)parents.关系组.push_back(edge);
            if(parents.关系组.size()!=1 || parents.关系组.front().源节点!=s.通用场景)continue;
            if(ref(所属)!=std::optional{W} || ref(状态特征引用)!=std::optional{F})continue;
            const auto time=number(步骤),sem=number(时间语义);
            if(!time || *time<0 || !sem || *sem!=static_cast<std::int64_t>(新状态时间语义::抽象相对))
                throw std::logic_error("local classification state snapshot malformed");
            std::optional<新特征准确值> value;
            if(const auto v=ref(当前材料))value=新特征准确值{*v};
            else if(const auto v=number(数值))value=新特征准确值{*v};
            // 保留最新状态的位置；无法解释的值不能冒充未知，也不能回退到较旧状态。
            const bool 无法解释=!value && (!fields(当前材料).empty() || !fields(数值).empty());
            if(!latestTime || *time>*latestTime) {
                latest=node;latestTime=*time;latestValue=std::move(value);
                最新值无法解释=无法解释;同时刻不唯一=false;
            } else if(*time==*latestTime && latest && *latest!=node)同时刻不唯一=true;
        }
        bool 与当前值相同=latest.has_value() && !old && !latestValue;
        if(old && latestValue)if(const auto stateValue=std::get_if<稳定编码>(&*latestValue)) {
            if(*stateValue==*old)与当前值相同=true;

        }
        if(latest && latestTime && !同时刻不唯一 && !最新值无法解释 && 与当前值相同) {
            if(*latestTime<*candidate.新步骤)candidate.前状态=*latest;
            else candidate.待填回执.动态原因=新词条动态记录原因::时间不递增;
        }
    }
    if(!candidate.前状态 && candidate.待填回执.动态原因!=新词条动态记录原因::时间不递增)
        candidate.待填回执.动态原因=新词条动态记录原因::无前态;
    const auto add=[&](节点种类 kind) {
        const auto slot=candidate.请求.新节点组.size();
        candidate.请求.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(kind)}},
            {字段组[根],模拟世界树根节点}}});
        candidate.请求.追加外部关系组.push_back({s.通用场景,基础组内节点引用{slot},基础外部关系类型::父子});
        return slot;
    };
    candidate.后状态槽=add(状态种类);
    auto& after=candidate.请求.新节点组[*candidate.后状态槽].字段组;
    after.insert(after.end(),{{字段组[所属],W},{字段组[状态特征引用],feature.节点},
        {字段组[当前材料],*candidate.目标材料},
        {字段组[动作引用],A},{字段组[步骤],基础值{*candidate.新步骤}},
        {字段组[时间语义],基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)}}});
    if(candidate.前状态) {
        candidate.待填回执.动态原因=新词条动态记录原因::已形成;
        candidate.变化槽=add(动态种类);
        auto& change=candidate.请求.新节点组[*candidate.变化槽].字段组;
        change.insert(change.end(),{{字段组[所属],W},{字段组[状态特征引用],feature.节点},
            {字段组[前状态引用],*candidate.前状态},{字段组[后状态引用],基础组内节点引用{*candidate.后状态槽}},
            {字段组[动作引用],A},{字段组[来源引用],N},{字段组[方法直接写入值],基础值{std::int64_t{1}}}});
        candidate.请求.当前追加字段键组.push_back({A,字段组[动态引用]});
        candidate.请求.当前追加字段键组.push_back({N,字段组[动态引用]});
        candidate.请求.追加字段组.push_back({A,字段组[动态引用],基础组内节点引用{*candidate.变化槽}});
        candidate.请求.追加字段组.push_back({N,字段组[动态引用],基础组内节点引用{*candidate.变化槽}});
    }
    prepareProgress();
    finishCandidate();
    return candidate;
}

新本能词条归类回执 新_模拟世界类::填回本地归类回执(
    本地归类候选&& candidate,const 基础节点组追加结果& published) {
    if(published.状态!=基础节点组追加状态::已完成
        || published.新节点组.size()!=candidate.请求.新节点组.size()
        || candidate.待填回执.名称关系组.size()!=1 || !candidate.名称.名称引用)
        throw std::logic_error("local classification publication receipt shape invalid");
    const auto resolve=[&](std::size_t index) {
        if(index>=published.新节点组.size() || !有效(published.新节点组[index]))
            throw std::logic_error("local classification published node slot invalid");
        return published.新节点组[index];
    };
    auto& receipt=candidate.待填回执;
    if(const auto slot=std::get_if<基础组内节点引用>(&*candidate.名称.名称引用))
        receipt.名称关系组[0]=resolve(slot->索引);
    else if(const auto existing=std::get_if<稳定编码>(&*candidate.名称.名称引用))
        receipt.名称关系组[0]=*existing;
    else throw std::logic_error("local classification name receipt invalid");
    if(candidate.后状态槽)receipt.新状态=resolve(*candidate.后状态槽);
    if(candidate.变化槽)receipt.变化动态=resolve(*candidate.变化槽);
    return std::move(receipt);
}

// 只接管已经成功的L1结果；调用方须从准备至接管持续持有样例锁。
const 新本能词条归类回执& 新_模拟世界类::接管本地归类结果(
    稳定编码 call,本地归类候选&& candidate,基础节点组追加结果&& published,
    std::unique_lock<std::recursive_mutex>& lock) {
    if(!实现_ || !lock.owns_lock() || lock.mutex()!=&实现_->互斥)
        throw std::logic_error("local classification receipt requires sample lock");
    auto& s=*实现_;
    const auto invocation=s.归类调用组.find(call);
    if(invocation==s.归类调用组.end() || invocation->second.回执
        || invocation->second.动作!=std::optional{candidate.待填回执.动作})
        throw std::logic_error("local classification invocation receipt slot unavailable");
    if(published.状态!=基础节点组追加状态::已完成
        || published.新节点组.size()!=candidate.请求.新节点组.size())
        throw std::logic_error("local classification receipt does not match publication");
    // 预留在发表前完成；原子结果中的身份只登记一次，不重新执行写入。
    s.收集追加(published);
    for(const auto old:candidate.删除的本地字段)
        s.通用字段.erase(std::remove(s.通用字段.begin(),s.通用字段.end(),old),s.通用字段.end());
    if(candidate.新步骤)s.模拟步骤=*candidate.新步骤;
    if(candidate.值改变)
        if(auto* slot=s.准备写回槽(candidate.待填回执.实际输入.挂接特征))*slot=实现::写入来源::方法;
    static_assert(std::is_nothrow_move_constructible_v<新本能词条归类回执>);
    static_assert(std::is_nothrow_move_assignable_v<std::vector<基础当前字段变化>>);
    invocation->second.实际字段变化=std::move(published.当前字段变化组);
    invocation->second.回执.emplace(填回本地归类回执(std::move(candidate),published));
    return *invocation->second.回执;
}

新词条归类提交结果 新_模拟世界类::读取本地归类结果(const 方法参数场景& scene) const {
    using S=新词条归类运行状态;
    if(!实现_ || scene.用途()!=新方法参数场景用途::模拟 || scene.获取模拟运行接口()!=this)
        throw std::invalid_argument("local classification result scene owner differs");
    const auto& s=*实现_;
    std::lock_guard lock(s.互斥);
    if(scene.方法节点()!=s.通用方法 || scene.全部角色绑定()!=s.通用绑定 || !s.当前调用)
        return {S::需重新组织};
    const auto it=s.归类调用组.find(*s.当前调用);
    if(it!=s.归类调用组.end() && it->second.回执)return {S::已完成,it->second.回执,{}};
    if(!s.样例就绪 || s.请求取消)return {S::已停止};
    return {S::需重新组织};
}

// 按已登记归类调用组合发表，返回原调用的正式回执。
新词条归类提交结果 新_模拟世界类::提交本地归类候选(const 方法参数场景& scene) {
    using S=新词条归类运行状态;
    using L=基础节点组追加状态;
    if(!实现_ || scene.用途()!=新方法参数场景用途::模拟 || scene.获取模拟运行接口()!=this)
        throw std::invalid_argument("local classification submit scene owner differs");
    auto& s=*实现_;
    std::lock_guard phase(s.归类动作阶段互斥);
    {
        std::lock_guard lock(s.互斥);
        if(scene.方法节点()!=s.通用方法 || scene.全部角色绑定()!=s.通用绑定 || !s.当前调用)
            return {S::需重新组织};
        const auto old=s.归类调用组.find(*s.当前调用);
        if(old!=s.归类调用组.end() && old->second.回执)return {S::已完成,old->second.回执,{}};
        if(!s.样例就绪 || s.请求取消)return {S::已停止};
        s.归类动作在途=true;
    }
    struct 阶段租约 final {
        实现& 所有者;
        ~阶段租约(){std::lock_guard lock(所有者.互斥);所有者.归类动作在途=false;}
    } lease{s};
    for(;;) {
        if(请求停止已成立())return {S::已停止};
        std::unique_lock lock(s.互斥);
        if(!s.样例就绪 || s.请求取消)return {S::已停止};
        if(scene.方法节点()!=s.通用方法 || scene.全部角色绑定()!=s.通用绑定 || !s.当前调用)
            return {S::需重新组织};
        const auto N=*s.当前调用;
        const auto it=s.归类调用组.find(N);
        if(it==s.归类调用组.end() || !it->second.动作)return {S::需重新组织};
        if(it->second.回执)return {S::已完成,it->second.回执,{}};
        const auto A=*it->second.动作;
        const auto W=it->second.输入.词条,C=it->second.输入.采用概念,F=it->second.输入.挂接特征;
        bool 已观察停止=false;
        auto candidate=准备本地归类候选(W,C,N,A,F,lock,已观察停止);
        // 准备期间可能为跨 owner 读取释放样例锁；停止后不能报告为材料需重新组织。
        if(!s.样例就绪 || s.请求取消)return {S::已停止};
        if(已观察停止) {
            lock.unlock();
            (void)处理隔离停止(true);
            return {S::已停止};
        }
        if(!candidate || !candidate->概念分支已准备)return {S::需重新组织};
        const auto& q=candidate->请求;
        const bool hasWrites=!q.新节点组.empty() || !q.追加字段组.empty() || !q.替换字段组.empty()
            || !q.追加外部关系组.empty() || !q.删除字段关系组.empty()
            || !q.删除节点组.empty() || !q.删除外部关系组.empty()
            || !q.当前字段赋值组.empty() || !q.当前整数递增组.empty();
        基础节点组追加结果 result;
        if(hasWrites)result=全局基础数据集.原子追加节点组(q);
        else result.状态=L::已完成; // 已有名称、值无需再次写入。
        if(result.状态==L::预期不符 || result.状态==L::节点不存在 || result.状态==L::引用不存在) {
            if(result.状态==L::节点不存在 || result.状态==L::引用不存在) {
                std::vector<稳定编码> ids;
                ids.reserve(q.只读预期.节点组.size());
                for(const auto& node:q.只读预期.节点组)ids.push_back(node.编码);
                const auto current=全局基础数据集.查询节点组完整结构(ids);
                if(current.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
                if(current.状态!=基础结构操作状态::已完成)
                    throw std::logic_error("local classification referenced node reread failed");
                if(current.节点组.size()!=ids.size()
                    || std::any_of(current.节点组.begin(),current.节点组.end(),[](const auto& row){return !row.快照;}))
                    return {S::需重新组织};
            }
            lock.unlock();continue;
        }
        if(result.状态==L::资源失败)throw std::bad_alloc{};
        if(result.状态!=L::已完成)throw std::logic_error("local classification atomic publication rejected");
        const auto& receipt=接管本地归类结果(N,std::move(*candidate),std::move(result),lock);
        return {S::已完成,receipt,{}};
    }
}

新词条归类动作结果 新_模拟世界类::准备本地归类动作(const 方法参数场景& scene) {
    using S=新词条归类运行状态;
    if(!实现_ || scene.用途()!=新方法参数场景用途::模拟 || scene.获取模拟运行接口()!=this)
        throw std::invalid_argument("local classification scene owner differs");
    auto& s=*实现_;
    std::lock_guard phase(s.归类动作阶段互斥);
    {
        std::lock_guard lock(s.互斥);
        if(!s.样例就绪 || s.请求取消)return {S::已停止};
        s.归类动作在途=true;
    }
    struct 阶段租约 final {
        实现& 所有者;
        ~阶段租约() {std::lock_guard lock(所有者.互斥);所有者.归类动作在途=false;}
    } lease{s};
    for(;;) {
    {
        std::lock_guard lock(s.互斥);
        if(!s.样例就绪 || s.请求取消)return {S::已停止};
    }
    const auto call=读取当前调用身份();
    {
        std::lock_guard lock(s.互斥);
        if(s.当前调用!=std::optional{call.调用节点} || !s.记录写回)return {S::需重新组织};
        if(scene.方法节点()!=s.通用方法 || scene.全部角色绑定()!=s.通用绑定 || !s.归类主体来源)
            throw std::invalid_argument("local classification sample differs");
        const auto prior=s.归类调用组.find(call.调用节点);
        if(prior!=s.归类调用组.end() && (prior->second.动作 || call.主动作)) {
            if(prior->second.动作 && call.主动作!=prior->second.动作)
                throw std::logic_error("local classification action identity differs");
            if(!prior->second.动作)prior->second.动作=call.主动作;
            return {S::已就绪,prior->second.动作,prior->second.条件,prior->second.输入,{}};
        }
        if(prior==s.归类调用组.end() && call.主动作)
            throw std::logic_error("local classification action input missing");
    }
    if(请求停止已成立())return {S::已停止};
    const std::array kinds{新方法参数信息类型::词条,新方法参数信息类型::特征,
        新方法参数信息类型::存在,新方法参数信息类型::存在};
    std::array<const 新方法有序参数引用*,4> p{};
    if(scene.全部角色绑定().size()!=p.size())throw std::invalid_argument("local classification requires four parameters");
    for(std::size_t i=0;i<p.size();++i) {
        p[i]=scene.查询参数(static_cast<std::uint32_t>(i+1));
        if(!p[i] || p[i]->信息类型!=kinds[i])throw std::invalid_argument("local classification parameter shape invalid");
    }
    新词典归类信息概念组 config;
    {
        std::lock_guard lock(s.互斥);
        const auto it=s.通用参数.find(p[2]->信息节点);
        if(it==s.通用参数.end() || !it->second.复制归类材料
            || s.本地执行主体!=std::optional{p[3]->信息节点})
            throw std::logic_error("local classification material or actor unavailable");
        config=it->second.复制归类材料->范围;
    }
    const auto word=scene.读取词条参数挂接(1);
    if(word.状态==新方法词条参数读取状态::资源失败)throw std::bad_alloc{};
    if(word.状态==新方法词条参数读取状态::参数已失效)return {S::需重新组织};
    if(word.状态!=新方法词条参数读取状态::已读取 || !word.信息)
        throw std::logic_error("local classification word unavailable");
    const auto bound=scene.读取绑定特征(p[1]->信息节点);
    if(!bound)throw std::logic_error("local classification bound feature unavailable");
    const auto definition=读取本地特征实际定义(p[1]->信息节点);
    if(!definition)throw std::logic_error("local classification feature definition unavailable");
    const auto f=scene.读取存在参数特征(1,definition->特征类型根);
    if(f.状态==新方法参数特征读取状态::定义前提已变化){std::this_thread::yield();continue;}
    if(f.状态==新方法参数特征读取状态::特征未取得)return {S::条件未具备};
    if(!f.特征 || (f.状态!=新方法参数特征读取状态::已读取
        && f.状态!=新方法参数特征读取状态::当前值未取得))
        throw std::logic_error("local classification feature unavailable");
    if(f.特征->节点!=p[1]->信息节点)return {S::需重新组织};
    const auto r=scene.读取正式信息参数保护(3);
    if(r.状态==正式信息参数读取状态::资源失败)throw std::bad_alloc{};
    if(r.状态==正式信息参数读取状态::前提已变化){std::this_thread::yield();continue;}
    if(r.状态==正式信息参数读取状态::信息不存在)return {S::需重新组织};
    if(r.状态!=正式信息参数读取状态::已读取 || !r.信息)throw std::logic_error("local classification input unavailable");
    const auto field=[&](稳定编码 key)->const 正式内容值& {
        const auto it=std::find_if(r.信息->直接内容组.begin(),r.信息->直接内容组.end(),
            [&](const auto& x){return x.特征类型==key;});
        if(it==r.信息->直接内容组.end())throw std::logic_error("local classification input field absent");
        return it->内容;
    };
    const auto& target=field(config.目标词条FT);const auto& adopted=field(config.采用概念FT);
    if(target.种类!=正式内容值种类::词条引用 || adopted.种类!=正式内容值种类::概念引用)
        throw std::logic_error("local classification input field type invalid");
    if(target.状态!=正式内容值状态::已知 || !target.值 || adopted.状态!=正式内容值状态::已知 || !adopted.值)
        return {S::条件未具备};
    const auto W=std::get_if<稳定编码>(&*target.值);const auto C=std::get_if<稳定编码>(&*adopted.值);
    if(!W || !C || !有效(*C))throw std::logic_error("local classification reference invalid");
    if(*W!=p[0]->信息节点)return {S::需重新组织};
    if(!读取模拟普通名称(*W) || !读取模拟全部名称概念(*W))return {S::条件未具备};
    新词条归类本能输入 input{*W,p[1]->信息节点,p[2]->信息节点,p[3]->信息节点,*C,{},std::nullopt};
    if(f.特征->当前值) {
        const auto* current=std::get_if<稳定编码>(&*f.特征->当前值);
        if(!current || !有效(*current))throw std::logic_error("local classification concept identity invalid");
        input.动作前概念组.push_back(*current);
    }
    新本能函数本次条件 conditions;conditions.执行分支节点=scene.执行分支节点();
    for(const auto& binding:scene.全部角色绑定()) {
        if(!binding.有序引用)throw std::logic_error("local classification ordered binding missing");
        新本能函数条件角色材料 row;row.参数角色节点=binding.参数角色节点;
        row.世界场景节点=binding.世界场景节点;row.有序引用=binding.有序引用;
        if(binding.有序引用->序号==2)row.实际特征组.push_back({f.特征->节点,f.特征->当前值});
        conditions.参数角色组.push_back(std::move(row));
    }
    {
        std::lock_guard lock(s.互斥);
        if(s.当前调用!=std::optional{call.调用节点} || !s.记录写回)return {S::需重新组织};
        const auto it=s.归类调用组.find(call.调用节点);
        if(it!=s.归类调用组.end() && it->second.动作)
            return {S::已就绪,it->second.动作,it->second.条件,it->second.输入,{}};
        if(it==s.归类调用组.end())s.归类调用组.emplace(call.调用节点,实现::本地归类调用{std::move(input),std::move(conditions)});
        else it->second=实现::本地归类调用{std::move(input),std::move(conditions)};
    }
    const auto action=建立参数动作(scene,{4,1,2});
    if(!有效(action))return {S::已停止};
    std::lock_guard lock(s.互斥);
    const auto it=s.归类调用组.find(call.调用节点);
    if(it==s.归类调用组.end())throw std::logic_error("local classification call record lost");
    it->second.动作=action;
    return {S::已就绪,action,it->second.条件,it->second.输入,{}};
    }
}

稳定编码 新_模拟世界类::建立参数动作(const 方法参数场景& scene,const 新模拟参数动作请求& request) {
    if(!实现_||scene.用途()!=新方法参数场景用途::模拟||scene.获取模拟运行接口()!=this)
        throw std::invalid_argument("simulation parameter action scene owner differs");
    const auto 主体=scene.查询参数(request.主体位置),目标=scene.查询参数(request.目标存在位置),
        参数=scene.查询参数(request.目标特征位置);
    if(!主体||!目标||!参数||主体->信息类型!=新方法参数信息类型::存在
        ||(目标->信息类型!=新方法参数信息类型::存在&&目标->信息类型!=新方法参数信息类型::词条)||参数->信息类型!=新方法参数信息类型::特征)
        throw std::invalid_argument("simulation parameter action positions invalid");
    const bool word=目标->信息类型==新方法参数信息类型::词条;
    if(word) {
        auto& s=*实现_;std::lock_guard lock(s.互斥);
        if(scene.方法节点()!=s.通用方法||scene.全部角色绑定()!=s.通用绑定)
            throw std::invalid_argument("local word action sample differs");
        if(s.当前调用)if(const auto prior=读引用(*s.当前调用,调用主动作引用)) {
            const auto action=读取本地动作(*prior);
            if(!action||action->主题存在节点!=主体->信息节点||action->动作来源节点!=s.当前调用
                ||action->动作目标词条节点!=std::optional{目标->信息节点}
                ||action->动作目标特征节点!=std::optional{参数->信息节点})
                throw std::logic_error("local word action fixed parameter conflict");
            return *prior;
        }
        if(!s.样例就绪||s.请求取消)return {};
    }
    const auto 特征=scene.读取绑定特征(参数->信息节点);
    if(!特征)throw std::invalid_argument("simulation parameter action feature absent");
    auto type=特征->特征概念节点;
    if(word) {
        const auto definition=读取本地特征实际定义(参数->信息节点);
        if(!definition)throw std::logic_error("local word action feature definition absent");
        type=definition->特征类型根;
    }
    const auto 定位=scene.读取存在参数特征(request.目标存在位置,type);
    if(!定位.特征||定位.特征->节点!=特征->节点)
        throw std::invalid_argument("simulation parameter action feature ownership differs");
    return 建立本地参数动作(scene,{request.目标存在位置,type,新隔离动作阶段::主动作},request.主体位置);
}
稳定编码 新_模拟世界类::建立本地参数动作(const 方法参数场景& scene,
    const 新隔离阶段动作请求& request,std::uint32_t 主体位置,std::optional<稳定编码> 语言输入) {
    if(!实现_||scene.用途()!=新方法参数场景用途::模拟)throw std::invalid_argument("isolation action scene unavailable");
    auto& s=*实现_;
    for(;;) {
    const auto N=读取当前调用身份();
    if(request.阶段!=新隔离动作阶段::主动作&&request.阶段!=新隔离动作阶段::输出入列)
        throw std::invalid_argument("isolation action stage invalid");
    const auto* target=scene.查询参数(request.目标存在位置);const auto* self=scene.查询参数(主体位置);
    if(!target||!self||(target->信息类型!=新方法参数信息类型::存在&&target->信息类型!=新方法参数信息类型::词条)||self->信息类型!=新方法参数信息类型::存在)
        throw std::invalid_argument("isolation action target binding absent");
    const bool word=target->信息类型==新方法参数信息类型::词条;
    bool 语言范围=false;
    稳定编码 selfConcept{},targetConcept{};
    {
        std::lock_guard lock(s.互斥);
        语言范围=s.本地语言范围&&s.本地语言范围->本地范围==target->信息节点;
        if(scene.方法节点()!=s.通用方法||scene.全部角色绑定()!=s.通用绑定
            ||!(word?s.是本地参数(target->信息节点,新方法参数信息类型::词条):语言范围||s.是普通存在参数(target->信息节点))
            ||!s.是普通存在参数(self->信息节点))
            throw std::invalid_argument("isolation action binding changed");
        if(语言输入&&(!语言范围||!属于样例(*语言输入,语言分界材料种类,s.通用场景)
            ||读引用(*语言输入,所属)!=std::optional{target->信息节点}))
            throw std::invalid_argument("isolation boundary input is outside its range");
        if(word&&request.阶段==新隔离动作阶段::主动作)if(const auto prior=读引用(N.调用节点,调用主动作引用)) {
            const auto action=读取本地动作(*prior);
            if(!action||action->主题存在节点!=self->信息节点||action->动作目标词条节点!=std::optional{target->信息节点}
                ||!action->动作目标特征节点||action->动作来源节点!=std::optional{N.调用节点}
                ||读引用(*action->动作目标特征节点,所属)!=std::optional{target->信息节点})
                throw std::logic_error("local word action source conflict");
            return *prior; // 本次已发生动作不按后来方法定义重建。
        }
        if(语言范围 && request.阶段==新隔离动作阶段::主动作) {
            if(const auto prior=读引用(N.调用节点,调用主动作引用)) {
                const auto 动作=读取本地动作(*prior);
                if(!动作 || 动作->主题存在节点!=self->信息节点
                    || 动作->动作目标存在节点!=target->信息节点 || 动作->动作语言输入材料节点!=语言输入
                    || !动作->动作目标特征节点 || 读引用(*动作->动作目标特征节点,所属)!=target->信息节点)
                    throw std::logic_error("local language action source conflict");
                return *prior;
            }
        }
        if((word||语言范围)&&(!s.样例就绪||s.请求取消))return {};
        selfConcept=s.通用参数.at(self->信息节点).规格.范围概念节点;
        targetConcept=s.通用参数.at(target->信息节点).规格.范围概念节点;
    }
    if(!s.动作定义读取||!s.动作定义发布||!s.动作概念)throw std::runtime_error("isolation action definition provider absent");
    const auto binding=scene.读取存在参数特征(request.目标存在位置,request.目标特征类型);
    if((binding.状态!=新方法参数特征读取状态::已读取&&binding.状态!=新方法参数特征读取状态::当前值未取得)||!binding.特征)
        throw std::invalid_argument("isolation action target not formally declared");
    动作方法投影请求 definitionRequest{N.方法节点,selfConcept,targetConcept,request.目标特征类型,false};
    if(word){definitionRequest.普通目标概念节点.reset();definitionRequest.词条目标范围概念节点=targetConcept;}
    if(语言范围){definitionRequest.普通目标概念节点.reset();definitionRequest.语言范围目标概念节点=targetConcept;}
    const auto projection=s.动作定义读取->读取动作方法投影(definitionRequest);
    const auto C=s.动作定义发布->确保动作定义概念(definitionRequest);
    if(C.状态!=动态概念操作状态::已建立&&C.状态!=动态概念操作状态::已复用)
        throw std::runtime_error("isolation action concept unavailable");
    if(!C.概念节点)throw std::runtime_error("isolation action concept identity absent");
    const auto protection=s.动作概念->读取动作概念定义保护(*C.概念节点);
    if((word||语言范围)&&(protection.状态==动作概念定义保护状态::前提已变化
        ||protection.状态==动作概念定义保护状态::概念不存在
        ||(protection.信息&&protection.信息->动作定义!=std::optional{projection}))) {
        std::lock_guard guard(s.互斥);
        if(!s.样例就绪||s.请求取消)return {};
        continue;
    }
    if(protection.状态!=动作概念定义保护状态::已读取||!protection.信息||protection.信息->动作定义!=std::optional{projection})
        throw std::runtime_error("isolation action concept protection unavailable");
    std::unique_lock lock(s.互斥);
    if((word||语言范围)&&(!s.样例就绪||s.请求取消))return {};
    if(s.当前调用!=std::optional{N.调用节点}||!s.记录写回||scene.全部角色绑定()!=s.通用绑定)
        throw std::runtime_error("isolation action call changed during preparation");
    const auto F=binding.特征->节点;
    if(读引用(F,所属)!=std::optional{target->信息节点})throw std::runtime_error("isolation action target ownership changed");
    if(request.阶段==新隔离动作阶段::主动作) {
        if(const auto A=读引用(N.调用节点,调用主动作引用)) {
            const auto old=读取本地动作(*A);
            if(!old||old->主题存在节点!=self->信息节点||(word?old->动作目标词条节点:old->动作目标存在节点)!=std::optional{target->信息节点}||old->动作目标特征节点!=std::optional{F}
                ||old->动作语言输入材料节点!=语言输入
                ||(!word&&old->动态概念节点!=C.概念节点))throw std::runtime_error("isolation main action idempotency conflict");
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
    if(!word&&binding.特征->当前值) {
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
    if(语言输入)put(A,结构引用,*语言输入);
    if(request.阶段==新隔离动作阶段::主动作) {
        put(A,来源引用,N.调用节点);q.追加字段组.push_back({N.调用节点,字段组[调用主动作引用],基础组内节点引用{A}});
    } else {
        const auto J=node(阶段来源种类);
        put(J,阶段所属调用,N.调用节点);put(J,阶段种类值,基础值{static_cast<std::int64_t>(request.阶段)});
        put(J,阶段目标位置值,基础值{static_cast<std::int64_t>(request.目标存在位置)});put(J,阶段目标特征引用,F);
        put(J,动作引用,基础组内节点引用{A});put(A,来源引用,基础组内节点引用{J});
        q.追加字段组.push_back({N.调用节点,字段组[调用阶段成员],基础组内节点引用{J}});
    }
    s.追加进展(q);
    if(word) {
        auto prior=std::move(q.预期联合节点组);q.预期联合节点组.clear();
        补齐隔离追加联合预期(q);q.预期联合节点组.insert(q.预期联合节点组.end(),prior.begin(),prior.end());
    } else {
        try { 补齐隔离追加联合预期(q); }
        catch(const 模拟联合预期变化&) {
            if(!语言范围)throw;
            lock.unlock();std::this_thread::yield();continue;
        }
    }
    s.预留追加(q);
    const auto made=全局基础数据集.原子追加节点组(q);
    if((word||语言范围)&&made.状态==基础节点组追加状态::预期不符){lock.unlock();std::this_thread::yield();continue;}
    if(made.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
    if(made.状态!=基础节点组追加状态::已完成||made.新节点组.size()!=q.新节点组.size())throw std::runtime_error("isolation action publication failed");
    s.收集追加(made);s.模拟步骤=time;s.本次已进入动作=true;
    if(!word&&!读取本地动作(made.新节点组[A]))throw std::runtime_error("isolation action readback failed");
    return made.新节点组[A];
    }
}
新方法隔离执行接口* 新_模拟世界类::获取隔离执行接口() noexcept {
    if(!实现_)return nullptr;
    std::lock_guard lock(实现_->互斥);
    return 实现_->通用&&实现_->动作定义读取&&实现_->动作定义发布&&实现_->动作概念&&实现_->场景构成 ? this : nullptr;
}
新本能语言分界运行接口* 新_模拟世界类::获取语言分界运行接口() noexcept {
    if (!实现_) return nullptr;
    std::lock_guard 锁(实现_->互斥);
    return 实现_->样例就绪 && 实现_->本地语言范围 ? this : nullptr;
}

std::optional<新隔离分界回执> 新_模拟世界类::读取本地分界回执(稳定编码 N) const {
    const auto& s=*实现_;
    const auto 回执=读引用(N,分界采用回执引用);
    if (!回执) return {};
    if (!属于样例(*回执,分界采用回执种类,s.通用场景) || 读引用(*回执,来源引用)!=N)
        throw std::logic_error("isolated boundary receipt owner differs");
    新隔离分界回执 r{N,必读引用(*回执,动作引用),必读引用(*回执,所属),
        必读引用(*回执,结构引用),必读引用(*回执,确认引用),必读引用(*回执,当前材料),
        必读引用(*回执,状态特征引用),必读引用(*回执,后状态引用)};
    const auto 动作=读取本地动作(r.动作);
    const auto 状态=读取本地状态(r.状态);
    const auto 结果=读取本地语言分界材料(r.结果);
    if (结果.状态==新语言分界工程状态::资源失败) throw std::bad_alloc{};
    const auto* 有效结果=结果.信息 ? std::get_if<新语言有效分界定义>(&结果.信息->定义) : nullptr;
    if (!动作 || !状态 || !有效结果 || 结果.信息->所属范围!=r.范围 || 有效结果->正式依据!=r.依据
        || 动作->动作目标种类!=新动作目标种类::语言材料范围 || 动作->动作来源节点!=N
        || 动作->动作目标存在节点!=r.范围 || 动作->动作目标特征节点!=r.特征
        || 动作->动作语言输入材料节点!=r.依据 || 状态->被描述存在节点!=r.范围
        || 状态->来源实例特征节点!=r.特征 || 状态->固定准确值!=新特征准确值{r.结果})
        throw std::logic_error("isolated boundary receipt does not match action and state");
    return r;
}

新本能函数本次条件结果对 新_模拟世界类::运行语言片段分词(const 方法参数场景& 场景) {
    新本能函数本次条件结果对 返回;
    auto& 结果=返回.结果;
    using 执行=新本能函数执行状态;
    using 参数=新方法参数信息类型;
    if (!实现_ || 场景.用途()!=新方法参数场景用途::模拟 || 场景.获取模拟运行接口()!=this) {
        结果.状态=执行::参数不匹配;
        return 返回;
    }
    auto& s=*实现_;
    const auto 分支=s.方法.读取执行分支必要条件(场景.方法节点(),场景.执行分支节点());
    if (分支.状态!=新方法执行分支读取状态::已读取 || !分支.分支 || 分支.分支->定义.顺序!=1)
        return 返回;
    返回.条件.执行分支节点=分支.分支->节点;
    const auto* R参数=场景.查询参数(1);
    const auto* E参数=场景.查询参数(5);
    if (!R参数 || !E参数 || R参数->信息类型!=参数::存在 || E参数->信息类型!=参数::存在
        || 场景.全部角色绑定().size()!=5) {
        结果.状态=执行::参数不匹配;
        return 返回;
    }

    const auto R=R参数->信息节点;
    std::lock_guard 阶段(s.归类动作阶段互斥);
    if (请求停止已成立()) return 返回;
    const auto N=读取当前调用身份().调用节点;
    {
        std::lock_guard 锁(s.互斥);
        if (!s.本地语言范围 || s.本地语言范围->本地范围!=R || s.本地执行主体!=E参数->信息节点
            || 场景.全部角色绑定()!=s.通用绑定) {
            结果.状态=执行::参数不匹配;
            return 返回;
        }
        for (std::uint32_t 位置=2;位置<=4;++位置) {
            const auto* 项=场景.查询参数(位置);
            if (!项 || 项->信息类型!=参数::特征 || 项->信息节点!=s.本地语言范围->本地特征[位置-2]) {
                结果.状态=执行::参数不匹配;
                return 返回;
            }
        }
    }
    for (const auto& 绑定:场景.全部角色绑定()) {
        新本能函数条件角色材料 角色;
        角色.有序引用=绑定.有序引用;
        角色.世界场景节点=绑定.世界场景节点;
        返回.条件.参数角色组.push_back(std::move(角色));
    }
    const auto 读取=[&](std::size_t 位置){
        auto F=场景.读取绑定特征(场景.查询参数(static_cast<std::uint32_t>(位置))->信息节点);
        if (F) 返回.条件.参数角色组[位置-1].实际特征组.push_back({F->节点,F->当前值});
        return F;
    };
    const auto 候选特征=读取(2),依据特征=读取(3),目标特征=读取(4);
    if (!候选特征 || !依据特征 || !目标特征 || !候选特征->当前值 || !依据特征->当前值)
        return 返回;
    const auto* C=std::get_if<稳定编码>(&*候选特征->当前值);
    const auto* B=std::get_if<稳定编码>(&*依据特征->当前值);
    if (!C || !B) throw std::logic_error("isolated boundary inputs are not references");
    const auto 候选=读取本地语言分界材料(*C),依据=读取本地语言分界材料(*B);
    if (候选.状态==新语言分界工程状态::资源失败 || 依据.状态==新语言分界工程状态::资源失败)
        throw std::bad_alloc{};
    if (!候选.信息 || !依据.信息) return 返回;
    const auto* 定义=std::get_if<新语言分界依据定义>(&依据.信息->定义);
    if (!定义 || !std::holds_alternative<新语言分界候选定义>(候选.信息->定义)
        || 依据.信息->所属范围!=R || 候选.信息->所属范围!=R)
        throw std::logic_error("isolated boundary input type or range differs");
    if (定义->候选材料!=*C) return 返回;

    const auto 完成=[&](const 新隔离分界回执& a) {
        结果.动作动态节点=a.动作;
        结果.已形成状态节点组.push_back(a.状态);
        结果.状态=执行::已按合同返回;
        结果.动作阶段=新本能函数动作阶段::已按函数合同返回;
        结果.结果取得状态=新本能函数结果取得状态::已取得;
    };
    {
        std::lock_guard 锁(s.互斥);
        if (const auto 已有=读取本地分界回执(N)) {
            完成(*已有);
            return 返回;
        }
    }
    if (目标特征->当前值 || 请求停止已成立()) return 返回;
    const auto FT=目标特征->特征概念节点,F=目标特征->节点;
    const auto A=建立本地参数动作(场景,{1,FT,新隔离动作阶段::主动作},5,*B);
    if (!有效(A)) return 返回;
    结果.动作动态节点=A;
    结果.状态=执行::动作结果未知;
    结果.动作阶段=新本能函数动作阶段::结果未知;
    if (请求停止已成立()) return 返回;

    std::lock_guard 锁(s.互斥);
    if (!s.记录写回 || s.当前调用!=N || s.请求取消) return 返回;
    if (s.模拟步骤==(std::numeric_limits<std::int64_t>::max)())
        throw std::overflow_error("isolated boundary state time exhausted");
    const auto 时刻=s.模拟步骤+1;
    基础节点组追加请求 q;
    const auto 新建=[&](节点种类 种类) {
        const auto 槽=q.新节点组.size();
        q.新节点组.push_back({{{字段组[类型],基础值{static_cast<std::int64_t>(种类)}},
            {字段组[根],模拟世界树根节点}}});
        q.追加外部关系组.push_back({s.通用场景,基础组内节点引用{槽},基础外部关系类型::父子});
        return 基础组内节点引用{槽};
    };
    const auto 写=[&](基础组内节点引用 目标,字段 键,基础组内字段内容 值) {
        q.新节点组[目标.索引].字段组.push_back({字段组[键],std::move(值)});
    };
    const auto V=新建(语言分界材料种类),S=新建(状态种类),L=新建(分界采用回执种类);
    写(V,所属,R);写(V,结构引用,*B);
    写(V,结构类别值,基础值{static_cast<std::int64_t>(新语言范围特征种类::有效分界结果)});
    写(S,所属,R);写(S,状态特征引用,F);写(S,特征类型,FT);写(S,当前材料,V);
    写(S,步骤,基础值{时刻});写(S,时间语义,基础值{static_cast<std::int64_t>(新状态时间语义::抽象相对)});
    写(L,来源引用,N);写(L,动作引用,A);写(L,所属,R);写(L,状态特征引用,F);
    写(L,结构引用,*C);写(L,确认引用,*B);写(L,当前材料,V);写(L,后状态引用,S);
    q.追加字段组.push_back({N,字段组[分界采用回执引用],L});
    q.当前字段赋值组.push_back({F,字段组[当前材料],V});
    std::vector<稳定编码> 历史节点{F};
    const auto 命中组=读引用组(F,历史命中引用);
    历史节点.insert(历史节点.end(),命中组.begin(),命中组.end());
    const auto 历史快照=全局基础数据集.查询节点组完整结构(历史节点);
    if (历史快照.状态==基础结构操作状态::资源失败) throw std::bad_alloc{};
    const auto f=std::find_if(历史快照.节点组.begin(),历史快照.节点组.end(),
        [&](const auto& 项){return 项.查询节点==F;});
    if (历史快照.状态!=基础结构操作状态::已完成 || f==历史快照.节点组.end() || !f->快照)
        throw std::logic_error("isolated boundary target history is unavailable");
    (void)准备本地归类历史更新(F,V,q,f->快照->结构,历史快照.节点组);
    s.追加进展(q);
    补齐隔离追加联合预期(q);
    s.预留追加(q);
    auto* const 归因=s.准备写回槽(F);
    const auto 发布=全局基础数据集.原子追加节点组(q);
    if (发布.状态==基础节点组追加状态::资源失败) throw std::bad_alloc{};
    if (发布.状态==基础节点组追加状态::预期不符) return 返回;
    if (发布.状态!=基础节点组追加状态::已完成 || 发布.新节点组.size()!=q.新节点组.size())
        throw std::logic_error("isolated boundary result publication failed");
    s.收集追加(发布);
    s.模拟步骤=时刻;
    *归因=实现::写入来源::方法;
    const auto 回执=读取本地分界回执(N);
    if (!回执) throw std::logic_error("isolated boundary publication receipt missing");
    完成(*回执);
    return 返回;
}

新本能函数本次条件结果对 新_模拟世界类::运行词条准入(const 方法参数场景&) {
    新本能函数本次条件结果对 返回;
    返回.结果.状态=新本能函数执行状态::参数不匹配;
    return 返回;
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
bool 新_模拟世界类::请求停止已成立() { return 处理隔离停止(false); }
bool 新_模拟世界类::处理隔离停止(bool 已观察停止) {
    if(!实现_)throw std::runtime_error("isolation owner absent");
    auto& s=*实现_;std::unique_lock lock(s.互斥);
    // 已没有活动调用时，本次调用无需继续；不尝试读取已关闭的调用窗口。
    if(!s.通用 || !s.当前调用 || !s.记录写回)return true;
    const auto id=读取当前调用身份();
    const auto cb=s.停止检查;const bool prior=s.请求取消;
    lock.unlock();const bool stop=prior||已观察停止||(cb&&cb());lock.lock();
    // 回调在锁外执行；原调用已关闭或被替换时，本次旧调用停止，不操作新的调用。
    if(s.当前调用!=std::optional{id.调用节点}||!s.记录写回)return true;
    if(stop&&!s.请求取消) {
        for(;;) {
            const auto f=唯一字段(id.调用节点,调用取消值);
            if(!f)throw std::runtime_error("isolation cancellation field absent");
            基础节点组追加请求 q;q.替换字段组.push_back({f->编码,基础值{std::int64_t{1}}});
            补齐隔离追加联合预期(q);
            const auto result=全局基础数据集.原子追加节点组(q);
            if(result.状态==基础节点组追加状态::预期不符)continue;
            if(result.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
            if(result.状态!=基础节点组追加状态::已完成)throw std::runtime_error("isolation cancellation publication failed");
            break;
        }
        s.请求取消=true;
    }
    return s.请求取消;
}
std::optional<新状态信息> 新_模拟世界类::读取本地状态(稳定编码 n) const {
    if(!实现_)return {};
    const auto& s=*实现_;std::lock_guard lock(s.互斥);
    if(!s.通用||std::find(s.通用节点.begin(),s.通用节点.end(),n)==s.通用节点.end()
        ||读值(n,类型)!=状态种类||读引用(n,根)!=模拟世界树根节点)return {};
    const auto E=读引用(n,所属),F=读引用(n,状态特征引用);
    // 状态的特征类型来自来源特征的当前定义；旧状态上保存的类型不再作为事实。
    const auto FT=F ? 读引用(*F,特征类型) : std::nullopt;
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
    result.动作定义节点=*M;result.动作来源节点=*source;
    const auto 目标类型=读值(*E,类型);
    if(目标类型==词条种类){result.动作目标种类=新动作目标种类::词条;result.动作目标词条节点=*E;}
    else {
        result.动作目标存在节点=*E;
        if(目标类型==语言范围种类) {
            result.动作目标种类=新动作目标种类::语言材料范围;
            result.动作语言输入材料节点=读引用(A,结构引用);
        }
    }
    result.动作目标特征节点=*F;result.动态概念节点=*C;
    result.动作时间=新状态强时间{新状态时间语义::抽象相对,*time};
    for(const auto& f:全局基础数据集.查询字段(A,字段组[动态引用])) {
        const auto* D=std::get_if<稳定编码>(&f.内容);
        if(!D||读引用(*D,动作引用)!=std::optional{A})throw std::runtime_error("isolation action result reference corrupt");
        result.动作结果组.push_back({新动作结果关系类型::导致变化,*D});
    }
    for(const auto& f:全局基础数据集.查询字段(A,字段组[动作观察动态引用])) {
        const auto* D=std::get_if<稳定编码>(&f.内容);
        if(!D||!读取本地变化(*D))throw std::logic_error("isolation observed action result corrupt");
        result.动作结果组.push_back({新动作结果关系类型::观察取得,*D});
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
    const auto dictionary=读取本地词典调用(N);
    if(dictionary)r.词典读取=dictionary->回执;
    // 只交付本调用已保存的归类回执，不从当前特征重建历史结果。
    const auto classification=s.归类调用组.find(N);
    if(classification!=s.归类调用组.end())r.词条归类=classification->second.回执;
    if(s.本地语言范围)r.有效分界=读取本地分界回执(N);
    if(s.归类主体来源)r.归类名称读取组=s.归类名称读取组;
    if(s.归类主体来源)for(std::size_t i=0;i<s.内容读取组.size();++i) {
        const auto& trace=s.内容读取组[i];
        const auto 序号=static_cast<std::uint64_t>(i);
        if(trace.类别==7) {
            const auto* 内容=std::get_if<正式信息参数投影>(&trace.投影);
            if(!内容 || !trace.归类结果已提交.has_value())
                throw std::logic_error("classification material read trace malformed");
            r.归类材料读取组.push_back({*内容,trace.参数位置组,trace.动作前,*trace.归类结果已提交,序号});
        } else if(trace.类别==6) {
            const auto* 内容=std::get_if<新特征信息>(&trace.投影);
            if(!内容 || !trace.归类结果已提交.has_value())
                throw std::logic_error("classification feature read trace malformed");
            r.归类特征读取组.push_back({*内容,trace.参数位置组,trace.动作前,*trace.归类结果已提交,序号});
        } else if(trace.类别==8) {
            const auto* 内容=std::get_if<新特征值信息>(&trace.投影);
            if(!内容 || !trace.归类结果已提交.has_value())
                throw std::logic_error("classification value read trace malformed");
            r.归类值读取组.push_back({*内容,trace.动作前,*trace.归类结果已提交,序号});
        } else {
            if(!trace.归类结果已提交.has_value())
                throw std::logic_error("classification other read trace stage missing");
            r.归类其他读取组.push_back({trace.类别,trace.节点,trace.参数位置组,trace.投影,
                trace.动作前,*trace.归类结果已提交,序号});
        }
    }
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
