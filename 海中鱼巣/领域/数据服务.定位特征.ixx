module;
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.定位特征;
export import 海中鱼巣.领域.数据服务.特征值类;

export namespace 海中鱼巣 {
enum class 定位特征状态 : std::uint8_t {
    已创建=1, 已复用=2, 精确重复=3, 已读取=4, 入口拒绝=5,
    未找到=6, 目标已退出=7, 位置冲突=8, 需选择F=9, 存量未定位=10,
    事实代次漂移=11, 幂等冲突=12, 预算不足=13, 历史材料不可用=14,
    表示不支持=15, 资源失败=16, 内部不一致=17, 已可能发布=18
};
struct 定位特征位置 final {
    稳定编码 场景{},组织父{};
    friend bool operator==(const 定位特征位置&,const 定位特征位置&)=default;
};
struct 定位特征预算 final {
    std::uint64_t 最大候选数{},最大路径长度{};
    friend bool operator==(const 定位特征预算&,const 定位特征预算&)=default;
};
struct 定位特征创建请求 final {
    std::uint32_t 版本=1;
    std::uint64_t G0=0;
    定位特征位置 位置;
    稳定编码 正式特征类型{};
    std::variant<std::int64_t,特征值身份> 准确值;
    std::optional<稳定编码> 指定F;
    L1所有者范围写入幂等身份 组合键,内容键,已知键,组织键;
    定位特征预算 预算;
    friend bool operator==(const 定位特征创建请求&,const 定位特征创建请求&)=default;
};
struct 定位特征事实 final {
    稳定编码 F{},正式特征类型{};
    std::int64_t 准确I64{};
    定位特征位置 位置;
    稳定编码 已知关系{},组织关系{};
    std::uint64_t 出生H{};
    friend bool operator==(const 定位特征事实&,const 定位特征事实&)=default;
};
struct 定位特征结果 final {
    std::uint32_t 版本=1;
    定位特征状态 状态=定位特征状态::入口拒绝;
    std::uint64_t Gread=0;
    std::optional<std::uint64_t> 首次发布H;
    std::optional<定位特征事实> 事实;
    std::vector<稳定编码> 候选F;
    定位特征创建请求 原请求;
    bool 成功() const noexcept;
};
struct 定位特征读取请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread{},H{};
    稳定编码 F{};
    定位特征预算 预算;
};
struct 定位特征读取结果 final {
    std::uint32_t 版本=1;
    定位特征状态 状态=定位特征状态::入口拒绝;
    std::uint64_t Gread{},H{};
    std::optional<定位特征事实> 事实;
    bool 成功(const 定位特征读取请求&) const noexcept;
};
struct 定位内容见证 final {
    稳定编码 F{},正式特征类型{};
    std::int64_t 准确I64{};
    std::uint64_t 创建H{};
    std::optional<std::uint64_t> 退出H;
};
struct 定位关系见证 final {
    稳定编码 关系{},源{},目标{},关系类型{};
    std::int64_t 角色{};
    std::uint64_t 创建H{};
    std::optional<std::uint64_t> 退出H;
};
struct 定位场景已知见证 final {
    稳定编码 场景{};
    std::optional<稳定编码> 父F;
    std::optional<定位关系见证> 父已知关系;
};
struct 定位父路径见证 final {
    稳定编码 场景{},特征根{},组织父{};
    std::vector<稳定编码> 路径节点;
    std::vector<定位关系见证> 路径关系;
};
struct 定位路径见证 final {
    稳定编码 场景{},特征根{},组织父{},F{};
    std::vector<稳定编码> 路径节点;
    std::vector<定位关系见证> 路径关系;
};
template<class T> struct 定位参与者结果 final {
    定位特征状态 状态=定位特征状态::入口拒绝;
    std::uint64_t Gread{},H{};
    std::optional<T> 数据;
};

class 定位特征数据服务;
class 定位特征内容参与者 {
public:
    virtual ~定位特征内容参与者()=default;
private:
    friend class 定位特征数据服务;
    virtual const L1事实基座服务& 定位底座() const noexcept=0;
    virtual L1所有者范围写端口& 定位端口() noexcept=0;
    virtual bool 定位结构已就绪() const noexcept=0;
    virtual bool 定位幂等键可用(L1所有者范围写入幂等身份) const noexcept=0;
    virtual 定位参与者结果<L1三分区原子参与者写集_v2> 准备定位内容(const 定位特征创建请求&,std::uint64_t) const=0;
    virtual 定位参与者结果<定位内容见证> 读取定位内容(std::uint64_t,std::uint64_t,稳定编码) const=0;
    virtual 定位参与者结果<std::vector<稳定编码>> 查询定位准确候选(std::uint64_t,std::uint64_t,稳定编码,const std::variant<std::int64_t,特征值身份>&,std::uint64_t) const=0;
};
class 定位特征已知参与者 {
public:
    virtual ~定位特征已知参与者()=default;
private:
    friend class 定位特征数据服务;
    virtual const L1事实基座服务& 定位底座() const noexcept=0;
    virtual L1所有者范围写端口& 定位端口() noexcept=0;
    virtual bool 定位结构已就绪() const noexcept=0;
    virtual bool 定位幂等键可用(L1所有者范围写入幂等身份) const noexcept=0;
    virtual 定位参与者结果<L1三分区原子参与者写集_v2> 准备定位已知(const 定位特征创建请求&,std::uint64_t) const=0;
    virtual 定位参与者结果<定位场景已知见证> 核验定位场景与父已知(std::uint64_t,std::uint64_t,稳定编码,std::optional<稳定编码>,const 定位特征预算&) const=0;
    virtual 定位参与者结果<定位关系见证> 读取定位已知(std::uint64_t,std::uint64_t,稳定编码,稳定编码) const=0;
};
class 定位特征组织参与者 {
public:
    virtual ~定位特征组织参与者()=default;
private:
    friend class 定位特征数据服务;
    virtual const L1事实基座服务& 定位底座() const noexcept=0;
    virtual L1所有者范围写端口& 定位端口() noexcept=0;
    virtual bool 定位结构已就绪() const noexcept=0;
    virtual bool 定位幂等键可用(L1所有者范围写入幂等身份) const noexcept=0;
    virtual 定位参与者结果<L1三分区原子参与者写集_v2> 准备定位组织(const 定位特征创建请求&,std::uint64_t) const=0;
    virtual 定位参与者结果<定位父路径见证> 核验定位父(std::uint64_t,std::uint64_t,稳定编码,稳定编码,std::uint64_t) const=0;
    virtual 定位参与者结果<定位路径见证> 读取定位路径(std::uint64_t,std::uint64_t,稳定编码,std::uint64_t) const=0;
};

class 定位特征数据服务 final {
public:
    定位特征数据服务(定位特征内容参与者&,定位特征已知参与者&,定位特征组织参与者&);
    定位特征结果 创建或复用(const 定位特征创建请求&);
    定位特征读取结果 读取定位特征(const 定位特征读取请求&) const;
    bool 使用内容参与者(const 定位特征内容参与者& x) const noexcept { return &x==&content_; }
    bool 使用已知参与者(const 定位特征已知参与者& x) const noexcept { return &x==&known_; }
    bool 使用组织参与者(const 定位特征组织参与者& x) const noexcept { return &x==&tree_; }
private:
    using S=定位特征状态;
    static void 要求(bool b,S s=S::内部不一致) { if(!b) throw s; }
    template<class T> static T 取(定位参与者结果<T> r,std::uint64_t g,std::uint64_t h) {
        要求(r.状态==S::已读取,r.状态);
        要求(r.Gread==g&&r.H==h&&r.数据.has_value());
        return std::move(*r.数据);
    }
    using 映射表=std::array<std::vector<std::pair<L1所有者范围写集本地键,稳定编码>>,3>;
    static 稳定编码 编码(const 映射表&,std::size_t,L1所有者范围写集本地键);
    static void 核验映射(const 映射表&);
    static L1所有者范围写集请求 规范片段(const L1三分区原子写集请求_v2&,const 映射表&);
    void 核验片段事实(const L1所有者范围写集请求&,L1结构所有者身份,std::size_t,
        const 映射表&,std::uint64_t,std::uint64_t) const;
    L1三分区原子事务请求_v2 准备(const 定位特征创建请求&,std::uint64_t) const;
    std::uint64_t 当前G() const;
    定位特征事实 读取事实(std::uint64_t,std::uint64_t,稳定编码,const 定位特征预算&) const;
    定位特征内容参与者& content_;
    定位特征已知参与者& known_;
    定位特征组织参与者& tree_;
    mutable std::mutex mutex_;
};

inline 定位特征数据服务::定位特征数据服务(定位特征内容参与者& c,
    定位特征已知参与者& k,定位特征组织参与者& t):content_(c),known_(k),tree_(t) {
    try {
        const auto& base=c.定位底座();
        if(&base!=&k.定位底座()||&base!=&t.定位底座())throw S::入口拒绝;
        auto& cp=c.定位端口();auto& kp=k.定位端口();auto& tp=t.定位端口();
        if(!cp.有效()||!kp.有效()||!tp.有效()||!cp.绑定于(base)||!kp.绑定于(base)||!tp.绑定于(base)
            ||cp.所有者身份()==kp.所有者身份()||cp.所有者身份()==tp.所有者身份()||kp.所有者身份()==tp.所有者身份())throw S::入口拒绝;
        const auto g=当前G();
        if(!c.定位结构已就绪()||!k.定位结构已就绪()||!t.定位结构已就绪()||当前G()!=g)throw S::入口拒绝;
    }catch(...){throw std::invalid_argument("parent-bound feature providers not ready");}
}

inline bool 定位特征结果::成功() const noexcept {
    using S=定位特征状态;
    if(版本!=1 || 原请求.版本!=1 || !Gread || !事实 || !候选F.empty()
        || (状态!=S::已创建 && 状态!=S::已复用 && 状态!=S::精确重复)) return false;
    const auto& f=*事实;
    if(!有效(f.F)||!有效(f.正式特征类型)||!有效(f.位置.场景)||!有效(f.位置.组织父)
        ||f.位置.场景==f.位置.组织父||!有效(f.已知关系)||!有效(f.组织关系)
        ||!f.出生H||f.出生H>Gread||f.正式特征类型!=原请求.正式特征类型
        ||f.位置!=原请求.位置||原请求.准确值.valueless_by_exception()
        ||(原请求.指定F&&*原请求.指定F!=f.F)) return false;
    if(const auto* v=std::get_if<std::int64_t>(&原请求.准确值)) {
        if(*v!=f.准确I64) return false;
    } else if(!有效(std::get<特征值身份>(原请求.准确值))) return false;
    if(状态==S::已复用) return !首次发布H;
    return !原请求.指定F&&首次发布H&&*首次发布H==f.出生H
        &&原请求.G0<UINT64_MAX&&*首次发布H==原请求.G0+1;
}
inline bool 定位特征读取结果::成功(const 定位特征读取请求& r) const noexcept {
    return 版本==1&&r.版本==1&&状态==定位特征状态::已读取&&Gread==r.Gread
        &&H==r.H&&H&&H<=Gread&&事实&&事实->F==r.F&&有效(r.F)
        &&有效(事实->正式特征类型)&&有效(事实->位置.场景)&&有效(事实->位置.组织父)
        &&事实->位置.场景!=事实->位置.组织父&&有效(事实->已知关系)
        &&有效(事实->组织关系)&&事实->出生H&&事实->出生H<=H;
}
inline std::uint64_t 定位特征数据服务::当前G() const {
    const auto q=content_.定位底座().读取中性当前事实代次({L1中性CRUD合同版本});
    要求(q.状态==L1中性读取状态::成功,
        q.状态==L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致);
    要求(q.合同版本==L1中性CRUD合同版本&&q.事实代次!=0);
    return q.事实代次;
}
inline 定位特征事实 定位特征数据服务::读取事实(std::uint64_t g,std::uint64_t h,
    稳定编码 f,const 定位特征预算& b) const {
    const auto c=取(content_.读取定位内容(g,h,f),g,h);
    要求(c.F==f&&有效(c.正式特征类型)&&c.创建H&&c.创建H<=h&&(!c.退出H||*c.退出H>h));
    const auto p=取(tree_.读取定位路径(g,h,f,b.最大路径长度),g,h);
    要求(p.F==f&&有效(p.场景)&&有效(p.特征根)&&有效(p.组织父)
        &&p.路径节点.size()==p.路径关系.size()+1&&!p.路径关系.empty()
        &&p.路径节点.front()==p.特征根&&p.路径节点.back()==f
        &&p.路径关系.size()<=b.最大路径长度);
    for(std::size_t i=0;i<p.路径关系.size();++i) {
        const auto& e=p.路径关系[i];
        要求(有效(e.关系)&&有效(e.关系类型)&&e.角色==1&&e.源==p.路径节点[i]
            &&e.目标==p.路径节点[i+1]&&e.创建H&&e.创建H<=h&&(!e.退出H||*e.退出H>h));
        for(std::size_t j=0;j<i+1;++j) 要求(p.路径节点[j]!=e.目标);
    }
    const auto& last=p.路径关系.back();要求(last.源==p.组织父);
    auto known=known_.读取定位已知(g,h,p.场景,f);
    要求(known.状态!=S::未找到,S::位置冲突);
    const auto k=取(std::move(known),g,h);
    要求(有效(k.关系)&&有效(k.关系类型)&&k.角色==1&&k.源==p.场景&&k.目标==f
        &&k.创建H&&k.创建H<=h&&(!k.退出H||*k.退出H>h));
    // 存量明确迁移允许关系后建；F本体出生截止保持原事实。
    要求(k.创建H>=c.创建H&&last.创建H>=c.创建H);
    return {f,c.正式特征类型,c.准确I64,{p.场景,p.组织父},k.关系,last.关系,c.创建H};
}
inline 定位特征读取结果 定位特征数据服务::读取定位特征(const 定位特征读取请求& r) const {
    定位特征读取结果 out;out.Gread=r.Gread;out.H=r.H;
    try {
        std::lock_guard<std::mutex> lock(mutex_);
        要求(r.版本==1&&r.Gread&&r.H&&r.H<=r.Gread&&有效(r.F),S::入口拒绝);
        要求(r.预算.最大候选数&&r.预算.最大路径长度&&r.预算.最大路径长度<=4096,S::入口拒绝);
        要求(当前G()==r.Gread,S::事实代次漂移);
        out.事实=读取事实(r.Gread,r.H,r.F,r.预算);
        要求(当前G()==r.Gread,S::事实代次漂移);out.状态=S::已读取;要求(out.成功(r));
    }catch(S s){out.状态=s;out.事实.reset();}
    catch(const std::bad_alloc&){out.状态=S::资源失败;out.事实.reset();}
    catch(const std::length_error&){out.状态=S::资源失败;out.事实.reset();}
    catch(...){out.状态=S::内部不一致;out.事实.reset();}
    return out;
}
inline 稳定编码 定位特征数据服务::编码(const 映射表& maps,std::size_t owner,L1所有者范围写集本地键 key) {
    要求(owner<3);稳定编码 result{};
    for(const auto& [k,id]:maps[owner])if(k==key){要求(!有效(result));result=id;}
    要求(有效(result));return result;
}
inline void 定位特征数据服务::核验映射(const 映射表& maps) {
    std::array<稳定编码,6> all{};std::size_t used=0;
    for(std::size_t i=0;i<3;++i){const std::uint32_t count=i==0 ? 4 : 1;要求(maps[i].size()==count);
        for(std::uint32_t k=1;k<=count;++k){auto id=编码(maps,i,{k});
            for(std::size_t j=0;j<used;++j)要求(all[j]!=id);all[used++]=id;}}
}
inline L1所有者范围写集请求 定位特征数据服务::规范片段(const L1三分区原子写集请求_v2& w,const 映射表& maps) {
    auto ref=[&](const L1三分区原子事实引用值_v2& r)->L1所有者范围事实引用 {
        if(auto* id=std::get_if<稳定编码>(&r))return *id;
        if(auto* key=std::get_if<L1所有者范围写集本地键>(&r))return *key;
        auto* cross=std::get_if<L1三分区原子事实引用_v2>(&r);要求(cross&&cross->参与者.值&&cross->参与者.值<=3);
        return 编码(maps,cross->参与者.值-1,cross->本地键);
    };
    L1所有者范围写集请求 out{w.合同版本,w.期望事实代次,w.写入幂等身份};out.退出事实=w.退出事实;
    for(const auto& n:w.节点)out.节点.push_back({n.本地键,n.种类,n.属性类型表示});
    for(const auto& e:w.关系)out.关系.push_back({e.本地键,ref(e.源节点),ref(e.目标节点),ref(e.关系类型节点),e.角色或顺序});
    for(const auto& v:w.值)out.值.push_back({v.本地键,ref(v.所属节点),ref(v.属性类型节点),v.材料,ref(v.来源节点)});
    for(const auto& a:w.属性槽变更)out.属性槽变更.push_back({ref(a.所属节点),ref(a.属性类型节点),a.新当前值});
    return out;
}
inline void 定位特征数据服务::核验片段事实(const L1所有者范围写集请求& w,L1结构所有者身份 owner,
    std::size_t index,const 映射表& maps,std::uint64_t g,std::uint64_t h) const {
    const auto& base=content_.定位底座();
    auto resolve=[&](const L1所有者范围事实引用& r)->稳定编码 {
        if(const auto* id=std::get_if<稳定编码>(&r))return *id;
        const auto* key=std::get_if<L1所有者范围写集本地键>(&r);要求(key);return 编码(maps,index,*key);
    };
    auto raw=[&](稳定编码 id)->L1所有者范围事实副本 {
        const auto q=base.读取所有者范围历史事实({L1所有者范围CRUD合同版本,id});
        要求(q.读取事实代次==g,S::事实代次漂移);
        if(q.状态==L1所有者范围读取状态::资源失败)throw S::资源失败;
        if(q.物理清理墓碑||q.物理清理事实代次)throw S::历史材料不可用;
        要求(q.状态==L1所有者范围读取状态::成功&&q.合同版本==L1所有者范围CRUD合同版本&&q.查询编码==id&&q.事实);
        return *q.事实;
    };
    for(const auto& n:w.节点){const auto id=编码(maps,index,n.本地键);auto f=raw(id);auto* p=std::get_if<L1所有者范围节点事实>(&f);
        要求(p&&p->编码==id&&p->种类==n.种类&&p->属性类型表示==n.属性类型表示&&p->写入所有者==owner
            &&p->创建事实代次==h&&(!p->退出事实代次||*p->退出事实代次>h));}
    for(const auto& e:w.关系){const auto id=编码(maps,index,e.本地键);auto f=raw(id);auto* p=std::get_if<L1所有者范围关系事实>(&f);
        要求(p&&p->编码==id&&p->源节点==resolve(e.源节点)&&p->目标节点==resolve(e.目标节点)
            &&p->关系类型节点==resolve(e.关系类型节点)&&p->角色或顺序==e.角色或顺序&&p->写入所有者==owner
            &&p->创建事实代次==h&&(!p->退出事实代次||*p->退出事实代次>h));}
    for(const auto& v:w.值){const auto id=编码(maps,index,v.本地键);auto f=raw(id);auto* p=std::get_if<L1所有者范围值事实>(&f);
        要求(p&&p->编码==id&&p->所属节点==resolve(v.所属节点)&&p->属性类型节点==resolve(v.属性类型节点)
            &&p->来源节点==resolve(v.来源节点)&&p->材料==v.材料&&p->写入所有者==owner
            &&p->创建事实代次==h&&(!p->退出事实代次||*p->退出事实代次>h));}
    for(const auto& a:w.属性槽变更){const auto id=resolve(a.所属节点);
        const auto q=base.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本,id,h});
        要求(q.读取事实代次==g,S::事实代次漂移);
        要求(q.状态==L1所有者范围读取状态::成功&&q.合同版本==L1所有者范围CRUD合同版本
            &&q.所属节点==id&&q.历史截止事实代次==h&&q.属性值组.size()==1);
        要求(q.属性值组.front().编码==编码(maps,index,a.新当前值)&&q.属性值组.front().属性类型节点==resolve(a.属性类型节点));}
}
inline L1三分区原子事务请求_v2 定位特征数据服务::准备(const 定位特征创建请求& r,std::uint64_t g) const {
    const auto p=取(tree_.核验定位父(g,r.G0,r.位置.场景,r.位置.组织父,r.预算.最大路径长度),g,r.G0);
    要求(p.场景==r.位置.场景&&p.组织父==r.位置.组织父&&有效(p.特征根));
    const auto k=known_.核验定位场景与父已知(g,r.G0,r.位置.场景,
        p.组织父==p.特征根 ? std::nullopt : std::optional<稳定编码>{p.组织父},r.预算);
    要求(k.状态!=S::未找到||p.组织父==p.特征根,S::位置冲突);
    const auto witness=取(k,g,r.G0);要求(witness.场景==r.位置.场景);
    L1三分区原子事务请求_v2 out{2,r.G0,r.组合键,{}};
    out.参与者写集组.push_back(取(content_.准备定位内容(r,g),g,r.G0));
    out.参与者写集组.push_back(取(known_.准备定位已知(r,g),g,r.G0));
    out.参与者写集组.push_back(取(tree_.准备定位组织(r,g),g,r.G0));
    const std::array<L1结构所有者身份,3> owners{content_.定位端口().所有者身份(),known_.定位端口().所有者身份(),tree_.定位端口().所有者身份()};
    const std::array<L1所有者范围写入幂等身份,3> keys{r.内容键,r.已知键,r.组织键};
    for(std::size_t i=0;i<3;++i){const auto& p=out.参与者写集组[i];要求(p.参与者.值==i+1&&p.所有者==owners[i]
        &&p.写集.合同版本==L1所有者范围CRUD合同版本&&p.写集.期望事实代次==r.G0&&p.写集.写入幂等身份==keys[i]);}
    要求(当前G()==g,S::事实代次漂移);return out;
}
inline 定位特征结果 定位特征数据服务::创建或复用(const 定位特征创建请求& r) {
    定位特征结果 out;bool dispatched=false;
    auto fail=[&](S state){out.状态=dispatched ? S::已可能发布 : state;out.事实.reset();out.候选F.clear();};
    try {
        std::lock_guard<std::mutex> lock(mutex_);out.原请求=r;
        要求(r.版本==1&&r.G0&&r.G0<UINT64_MAX&&有效(r.位置.场景)&&有效(r.位置.组织父)
            &&r.位置.场景!=r.位置.组织父&&有效(r.正式特征类型)&&(!r.指定F||有效(*r.指定F))&&!r.准确值.valueless_by_exception(),S::入口拒绝);
        要求(r.预算.最大候选数&&r.预算.最大路径长度&&r.预算.最大路径长度<=4096,S::入口拒绝);
        if(auto* id=std::get_if<特征值身份>(&r.准确值))要求(有效(*id),S::入口拒绝);
        const std::array<L1所有者范围写入幂等身份,4> allkeys{r.组合键,r.内容键,r.已知键,r.组织键};
        for(std::size_t i=0;i<4;++i){要求(allkeys[i].值&&allkeys[i].值!=1&&(allkeys[i].值>>48)!=0x4E43,S::入口拒绝);
            for(std::size_t j=0;j<i;++j)要求(allkeys[i]!=allkeys[j],S::入口拒绝);
            要求(content_.定位幂等键可用(allkeys[i])&&known_.定位幂等键可用(allkeys[i])&&tree_.定位幂等键可用(allkeys[i]),S::入口拒绝);}
        out.Gread=当前G();要求(r.G0<=out.Gread,S::事实代次漂移);
        const std::array<L1所有者范围写端口*,3> ports{&content_.定位端口(),&known_.定位端口(),&tree_.定位端口()};
        std::array<L1所有者范围首次写入读取结果,3> first;映射表 maps;std::size_t found=0;
        for(std::size_t i=0;i<3;++i){first[i]=ports[i]->读取首次写入材料({L1所有者范围首次写入读取合同版本,allkeys[i+1]});const auto& f=first[i];
            要求(f.读取事实代次==out.Gread,S::事实代次漂移);
            要求(f.合同版本==L1所有者范围首次写入读取合同版本&&f.所有者==ports[i]->所有者身份()&&f.写入幂等身份==allkeys[i+1]);
            if(f.状态==L1所有者范围读取状态::未找到){要求(!f.首次规范化写集&&!f.首次写入结果);continue;}
            要求(f.状态==L1所有者范围读取状态::成功,f.状态==L1所有者范围读取状态::资源失败 ? S::资源失败 : S::内部不一致);
            要求(f.首次规范化写集&&f.首次写入结果);const auto& w=*f.首次规范化写集;const auto& result=*f.首次写入结果;
            要求(w.合同版本==L1所有者范围CRUD合同版本&&w.写入幂等身份==allkeys[i+1]&&w.期望事实代次&&w.期望事实代次<UINT64_MAX
                &&result.状态==L1所有者范围写入状态::成功&&result.合同版本==L1所有者范围CRUD合同版本&&result.所有者==ports[i]->所有者身份()
                &&result.写入幂等身份==allkeys[i+1]&&result.事实代次==w.期望事实代次+1&&result.是否形成内存权威发布
                &&result.重试边界==L1所有者范围重试边界::不适用);
            maps[i]=result.新编码映射;++found;
        }
        要求(found==0||found==3);const bool replay=found==3;
        if(replay){const auto h=first[0].首次写入结果->事实代次;
            要求(first[1].首次写入结果->事实代次==h&&first[2].首次写入结果->事实代次==h);out.首次发布H=h;
            核验映射(maps);for(std::size_t i=0;i<3;++i)核验片段事实(*first[i].首次规范化写集,ports[i]->所有者身份(),i,maps,out.Gread,h);
            要求(!r.指定F,S::入口拒绝);要求(h==r.G0+1,S::幂等冲突);
        }else 要求(out.Gread==r.G0,S::事实代次漂移);
        const auto transaction=准备(r,out.Gread);
        if(replay){for(std::size_t i=0;i<3;++i)要求(规范片段(transaction.参与者写集组[i].写集,maps)==*first[i].首次规范化写集,S::幂等冲突);}
        else {
            auto candidates=取(content_.查询定位准确候选(out.Gread,r.G0,r.正式特征类型,r.准确值,r.预算.最大候选数),out.Gread,r.G0);
            要求(candidates.size()<=r.预算.最大候选数);
            for(std::size_t i=0;i<candidates.size();++i)要求(有效(candidates[i])&&(!i||candidates[i-1].值<candidates[i].值));
            if(!r.指定F&&candidates.size()>1){要求(当前G()==out.Gread,S::事实代次漂移);out.候选F=std::move(candidates);out.状态=S::需选择F;return out;}
            if(r.指定F)要求(std::find(candidates.begin(),candidates.end(),*r.指定F)!=candidates.end(),S::入口拒绝);
            if(!candidates.empty()){const auto id=r.指定F ? *r.指定F : candidates.front();const auto fact=读取事实(out.Gread,r.G0,id,r.预算);
                要求(fact.位置==r.位置,S::位置冲突);要求(fact.正式特征类型==r.正式特征类型);
                out.事实=fact;out.状态=S::已复用;要求(当前G()==out.Gread,S::事实代次漂移);要求(out.成功());return out;}
        }
        要求(当前G()==out.Gread,S::事实代次漂移);dispatched=true;
        const auto saved=ports[0]->提交三分区原子事务_v2(transaction,*ports[1],*ports[2]);
        要求(saved.合同版本==2&&saved.组合写入幂等身份==r.组合键);
        using L=L1三分区原子事务状态_v2;
        if(saved.状态!=L::已提交&&saved.状态!=L::精确重复){
            if(saved.状态==L::已可能发布){if(saved.共同事实代次==r.G0+1)out.首次发布H=r.G0+1;throw S::已可能发布;}
            要求(!saved.是否已确认形成内存权威发布&&saved.参与者结果组.empty());
            dispatched=false;
            switch(saved.状态){case L::入口拒绝:throw S::入口拒绝;case L::事实代次漂移:throw replay ? S::幂等冲突 : S::事实代次漂移;
                case L::幂等冲突:throw S::幂等冲突;case L::资源失败:throw S::资源失败;default:throw S::内部不一致;}
        }
        要求(saved.共同事实代次==r.G0+1);out.首次发布H=r.G0+1;
        const bool repeated=saved.状态==L::精确重复;
        要求(!replay||repeated);要求(saved.是否已确认形成内存权威发布==!repeated
            &&saved.重试边界==(repeated ? L1所有者范围重试边界::原幂等身份读回收敛 : L1所有者范围重试边界::不适用));
        要求(saved.参与者结果组.size()==3);映射表 published;std::array<bool,3> seen{};
        for(const auto& p:saved.参与者结果组){要求(p.参与者.值&&p.参与者.值<=3);const auto i=p.参与者.值-1;
            要求(!seen[i]&&p.所有者==ports[i]->所有者身份());seen[i]=true;published[i]=p.新编码映射;}
        核验映射(published);if(replay)要求(published==maps);maps=std::move(published);
        out.Gread=当前G();for(std::size_t i=0;i<3;++i)核验片段事实(规范片段(transaction.参与者写集组[i].写集,maps),ports[i]->所有者身份(),i,maps,out.Gread,*out.首次发布H);
        out.事实=读取事实(out.Gread,*out.首次发布H,编码(maps,0,{1}),r.预算);
        要求(out.事实->已知关系==编码(maps,1,{1})&&out.事实->组织关系==编码(maps,2,{1})&&out.事实->出生H==*out.首次发布H);
        out.状态=repeated ? S::精确重复 : S::已创建;要求(out.成功());要求(当前G()==out.Gread,S::事实代次漂移);
    }catch(S s){fail(s);}catch(const std::bad_alloc&){fail(S::资源失败);}catch(const std::length_error&){fail(S::资源失败);}catch(...){fail(S::内部不一致);}
    return out;
}

}
