#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "数据服务.特征值类.h"
#include "合同.相关概念添加参与.h"

namespace 海中鱼巣 {
struct 定位特征位置 final {
    稳定编码 场景{},组织父{};
    friend bool operator==(const 定位特征位置&,const 定位特征位置&)=default;
};

// I64 出生入口只承载 I64；其它值表示不属于本合同。
struct 原子I64特征出生键 final {
    L1所有者范围写入幂等身份 组合{},内容{},已知{},组织{},概念使用{};
    friend bool operator==(const 原子I64特征出生键&,const 原子I64特征出生键&)=default;
};
struct 原子I64特征概念读取预算 final {
    std::uint64_t 最大概念数{},最大关系数{},最大特征属性数{};
    friend bool operator==(const 原子I64特征概念读取预算&,const 原子I64特征概念读取预算&)=default;
};
struct 原子I64特征组织读取预算 final { std::uint64_t 最大路径长度{}; friend bool operator==(const 原子I64特征组织读取预算&,const 原子I64特征组织读取预算&)=default; };
// 仅描述候选召回的上限；它不携带 C→…→P 路径预算，也不承担概念读取预算。
struct 原子I64特征候选读取预算 final { std::uint64_t 最大候选数{}; friend bool operator==(const 原子I64特征候选读取预算&,const 原子I64特征候选读取预算&)=default; };
struct 原子I64特征候选查询请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0;
    定位特征位置 位置;
    稳定编码 正式特征类型{};
    std::int64_t 准确I64=0;
    原子I64特征候选读取预算 候选读取预算;
    原子I64特征组织读取预算 组织读取预算;
    friend bool operator==(const 原子I64特征候选查询请求&,const 原子I64特征候选查询请求&)=default;
};
struct 原子I64特征候选 final {
    稳定编码 F{};
    std::uint64_t 出生事实代次=0;
    friend bool operator==(const 原子I64特征候选&,const 原子I64特征候选&)=default;
};
enum class 原子I64特征候选查询状态 : std::uint8_t {
    已读取=1,入口拒绝,位置冲突,事实代次漂移,
    预算不足,资源失败,内部不一致
};
struct 原子I64特征候选查询结果 final {
    std::uint32_t 版本=1;
    原子I64特征候选查询状态 状态=原子I64特征候选查询状态::入口拒绝;
    std::uint64_t Gread=0;
    std::vector<原子I64特征候选> 候选;
    原子I64特征候选查询请求 原请求;
};
struct 特征概念出生使用事实 final {
    稳定编码 关系{};特征信息身份 F{};概念树概念身份 FCv;概念树生命周期 生命周期;
    friend bool operator==(const 特征概念出生使用事实&,const 特征概念出生使用事实&)=default;
};
struct 原子I64特征出生请求 final {
    std::uint32_t 版本=1;
    std::uint64_t G0=0;
    定位特征位置 位置;
    稳定编码 正式特征类型{};
    std::int64_t 准确I64=0;
    概念树概念身份 FCv;
    原子I64特征出生键 键;
    原子I64特征候选读取预算 候选读取预算;
    原子I64特征组织读取预算 组织读取预算;
    原子I64特征概念读取预算 概念读取预算;
    friend bool operator==(const 原子I64特征出生请求&,const 原子I64特征出生请求&)=default;
};
struct 原子I64特征出生事实 final {
    稳定编码 F{},正式特征类型{},FCv{};
    std::int64_t 准确I64=0;
    定位特征位置 位置;
    稳定编码 holder{},已知关系{},组织关系{},概念使用关系{};
    std::uint64_t 出生事实代次=0;
    friend bool operator==(const 原子I64特征出生事实&,const 原子I64特征出生事实&)=default;
};
enum class 原子I64特征出生状态 : std::uint8_t {
    已创建=1,精确重复=2,入口拒绝=3,未找到=4,
    位置冲突=5,存量未定位=6,概念不适配=7,事实代次漂移=8,
    幂等冲突=9,预算不足=10,资源失败=11,内部不一致=12,已可能发布=13,
    已读取=14
};
struct 原子I64特征出生结果 final {
    std::uint32_t 版本=1;
    原子I64特征出生状态 状态=原子I64特征出生状态::入口拒绝;
    std::uint64_t Gread=0;
    std::optional<std::uint64_t> 首次发布代次;
    std::optional<原子I64特征出生事实> 事实;
    原子I64特征出生请求 原请求;
    bool 成功() const noexcept;
};
struct 原子I64特征出生读取请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0;
    稳定编码 F{};
    定位特征位置 位置;
    原子I64特征组织读取预算 组织读取预算;
    原子I64特征概念读取预算 概念读取预算;
};
enum class 原子I64特征窄读取状态 : std::uint8_t { 已读取=1,入口拒绝,未找到,位置冲突,事实代次漂移,预算不足,资源失败,内部不一致 };
template<class T> struct 原子I64特征窄读取结果 final { 原子I64特征窄读取状态 状态=原子I64特征窄读取状态::入口拒绝;std::uint64_t Gread=0;std::optional<T> 事实;};
struct 原子I64特征内容读取请求 final {std::uint32_t 版本=1;std::uint64_t Gread=0;稳定编码 F{};};
struct 原子I64特征内容事实 final {稳定编码 F{},正式特征类型{},类型关系{},I64值事实{};std::int64_t 准确I64=0;std::uint64_t 创建事实代次=0;};
struct 原子I64特征holder读取请求 final {std::uint32_t 版本=1;std::uint64_t Gread=0;稳定编码 holder{},F{};};
struct 原子I64特征holder事实 final {稳定编码 holder{},F{},已知关系{},关系类型{};std::uint64_t 创建事实代次=0;};
struct 原子I64特征组织读取请求 final {std::uint32_t 版本=1;std::uint64_t Gread=0;稳定编码 C{},P{},F{};原子I64特征组织读取预算 预算;};
struct 原子I64特征场景路径边事实 final {稳定编码 关系{},源{},目标{},关系类型{};std::int64_t 角色=0;std::uint64_t 创建事实代次=0;};
struct 原子I64特征组织事实 final {稳定编码 C{},P{},F{},组织关系{},组织关系类型{};std::vector<原子I64特征场景路径边事实> 路径;std::uint64_t 创建事实代次=0;};
struct 原子I64特征出生读取结果 final {
    std::uint32_t 版本=1;
    原子I64特征出生状态 状态=原子I64特征出生状态::入口拒绝;
    std::uint64_t Gread=0;
    std::optional<原子I64特征出生事实> 事实;
    bool 成功(const 原子I64特征出生读取请求&) const noexcept;
};
enum class 原子I64特征出生使用读取状态 : std::uint8_t {
    已读取=1,入口拒绝=2,未找到=3,概念不适配=4,
    事实代次漂移=5,预算不足=6,资源失败=7,内部不一致=8
};
struct 原子I64特征出生使用读取请求 final {
    std::uint32_t 版本=1;std::uint64_t Gread=0;稳定编码 F{};
    原子I64特征概念读取预算 概念读取预算;
};
struct 原子I64特征出生使用读取结果 final {
    std::uint32_t 版本=1;原子I64特征出生使用读取状态 状态=原子I64特征出生使用读取状态::入口拒绝;
    std::uint64_t Gread=0;std::optional<特征概念出生使用事实> 事实;
    bool 成功(const 原子I64特征出生使用读取请求&) const noexcept;
};
struct 原子I64特征出生使用退出片段请求 final {
    std::uint32_t 版本=1;std::uint64_t G0=0;稳定编码 F{};
    L1所有者范围写入幂等身份 概念使用退出键{};
    原子I64特征概念读取预算 概念读取预算;
};
template<class T> struct 原子I64特征参与结果 final {
    原子I64特征出生状态 状态=原子I64特征出生状态::入口拒绝;
    std::uint64_t Gread=0;
    std::optional<T> 数据;
};
class 原子I64特征内容参与者;
class 原子I64特征holder参与者;
class 原子I64特征组织参与者;
class 原子I64特征概念参与者;

// 四个参与者只暴露组合器需要的窄合同；概念参与者不形成 FCv，
// 仅核验已发布 FCv 后写 F -> FCv 出生使用边。
class 原子I64特征内容参与者 {
public: virtual ~原子I64特征内容参与者()=default;
private: friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept=0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept=0;
    virtual bool 原子I64结构已就绪() const noexcept=0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
        准备原子I64出生片段(const 原子I64特征出生请求&,std::uint64_t) const=0;
    virtual 原子I64特征候选查询结果 查询原子I64内容候选(
        const 原子I64特征候选查询请求&) const=0;
    virtual 原子I64特征窄读取结果<原子I64特征内容事实> 读取原子I64内容(const 原子I64特征内容读取请求&) const=0;
};
class 原子I64特征holder参与者 {
public: virtual ~原子I64特征holder参与者()=default;
private: friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept=0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept=0;
    virtual bool 原子I64结构已就绪() const noexcept=0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
        准备原子I64出生片段(const 原子I64特征出生请求&,std::uint64_t) const=0;
    virtual 原子I64特征窄读取结果<原子I64特征holder事实> 读取原子I64holder(const 原子I64特征holder读取请求&) const=0;
};
class 原子I64特征组织参与者 {
public: virtual ~原子I64特征组织参与者()=default;
private: friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept=0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept=0;
    virtual bool 原子I64结构已就绪() const noexcept=0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
        准备原子I64出生片段(const 原子I64特征出生请求&,std::uint64_t) const=0;
    virtual 原子I64特征窄读取结果<原子I64特征组织事实> 读取原子I64组织(const 原子I64特征组织读取请求&) const=0;
};
class 原子I64特征概念参与者 {
public: virtual ~原子I64特征概念参与者()=default;
private: friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept=0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept=0;
    virtual bool 原子I64结构已就绪() const noexcept=0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
        准备原子I64出生片段(const 原子I64特征出生请求&,std::uint64_t) const=0;
    virtual 原子I64特征出生使用读取结果 读取原子I64出生使用(
        const 原子I64特征出生使用读取请求&) const=0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
        准备原子I64出生使用退出片段(const 原子I64特征出生使用退出片段请求&,std::uint64_t) const=0;
};
class 原子I64特征出生数据服务 final {
public:
    原子I64特征出生数据服务(原子I64特征内容参与者&,原子I64特征holder参与者&,
        原子I64特征组织参与者&,原子I64特征概念参与者&);
    原子I64特征候选查询结果 查询精确候选(const 原子I64特征候选查询请求&) const;
    原子I64特征出生结果 创建(const 原子I64特征出生请求&);
    原子I64特征出生读取结果 读取(const 原子I64特征出生读取请求&) const;
private:
    原子I64特征内容参与者& 内容_; 原子I64特征holder参与者& holder_;
    原子I64特征组织参与者& 组织_; 原子I64特征概念参与者& 概念_;
    mutable std::mutex mutex_;
};

inline 原子I64特征出生数据服务::原子I64特征出生数据服务(
    原子I64特征内容参与者& c,原子I64特征holder参与者& h,
    原子I64特征组织参与者& o,原子I64特征概念参与者& k):内容_(c),holder_(h),组织_(o),概念_(k) {
    const auto& base=c.原子I64底座();
    if(&base!=&h.原子I64底座()||&base!=&o.原子I64底座()||&base!=&k.原子I64底座()||
       !c.原子I64端口().有效()||!h.原子I64端口().有效()||!o.原子I64端口().有效()||!k.原子I64端口().有效()||
       !c.原子I64端口().绑定于(base)||!h.原子I64端口().绑定于(base)||!o.原子I64端口().绑定于(base)||!k.原子I64端口().绑定于(base)||
       c.原子I64端口().所有者身份()==h.原子I64端口().所有者身份()||c.原子I64端口().所有者身份()==o.原子I64端口().所有者身份()||
       c.原子I64端口().所有者身份()==k.原子I64端口().所有者身份()||h.原子I64端口().所有者身份()==o.原子I64端口().所有者身份()||
       h.原子I64端口().所有者身份()==k.原子I64端口().所有者身份()||o.原子I64端口().所有者身份()==k.原子I64端口().所有者身份())
         throw std::invalid_argument("four owner I64 providers");
}
inline 原子I64特征候选查询结果 原子I64特征出生数据服务::查询精确候选(
    const 原子I64特征候选查询请求& r) const {
    原子I64特征候选查询结果 out;out.Gread=r.Gread;out.原请求=r;
    auto mapNarrow=[](原子I64特征窄读取状态 s) {
        using N=原子I64特征窄读取状态;using Q=原子I64特征候选查询状态;
        switch(s) {case N::入口拒绝:return Q::入口拒绝;
            case N::位置冲突:return Q::位置冲突;case N::事实代次漂移:return Q::事实代次漂移;
            case N::预算不足:return Q::预算不足;
            case N::资源失败:return Q::资源失败;default:return Q::内部不一致;}
    };
    if(r.版本!=1||!r.Gread||!有效(r.位置.场景)||!有效(r.位置.组织父)||!有效(r.正式特征类型)||
       !r.候选读取预算.最大候选数||!r.组织读取预算.最大路径长度)return out;
    const auto content=内容_.查询原子I64内容候选(r);
    if(content.状态!=原子I64特征候选查询状态::已读取)return content;
    if(content.Gread!=r.Gread||content.原请求!=r)return 原子I64特征候选查询结果{1,原子I64特征候选查询状态::内部不一致,r.Gread,{},r};
    if(content.候选.size()>r.候选读取预算.最大候选数)return 原子I64特征候选查询结果{1,原子I64特征候选查询状态::预算不足,r.Gread,{},r};
    try {
        for(const auto& candidate:content.候选) {
            if(!有效(candidate.F)||!candidate.出生事实代次||candidate.出生事实代次>r.Gread)
                throw 原子I64特征候选查询状态::内部不一致;
            const auto holder=holder_.读取原子I64holder({1,r.Gread,r.位置.场景,candidate.F});
            if(holder.状态==原子I64特征窄读取状态::未找到)continue;
            if(holder.状态!=原子I64特征窄读取状态::已读取||!holder.事实)
                throw mapNarrow(holder.状态);
            if(holder.Gread!=r.Gread||holder.事实->创建事实代次!=candidate.出生事实代次||holder.事实->holder!=r.位置.场景||holder.事实->F!=candidate.F)
                throw 原子I64特征候选查询状态::内部不一致;
            const auto organization=组织_.读取原子I64组织({1,r.Gread,r.位置.场景,r.位置.组织父,candidate.F,r.组织读取预算});
            if(organization.状态!=原子I64特征窄读取状态::已读取||!organization.事实)
                throw mapNarrow(organization.状态);
            if(organization.Gread!=r.Gread||organization.事实->创建事实代次!=candidate.出生事实代次||organization.事实->C!=r.位置.场景||
               organization.事实->P!=r.位置.组织父||organization.事实->F!=candidate.F)
                throw 原子I64特征候选查询状态::内部不一致;
            out.候选.push_back(candidate);
        }
        out.状态=原子I64特征候选查询状态::已读取;
    } catch(原子I64特征候选查询状态 s) {out.状态=s;out.候选.clear();}
      catch(const std::bad_alloc&) {out.状态=原子I64特征候选查询状态::资源失败;out.候选.clear();}
      catch(...) {out.状态=原子I64特征候选查询状态::内部不一致;out.候选.clear();}
    return out;
}
inline 原子I64特征出生结果 原子I64特征出生数据服务::创建(const 原子I64特征出生请求& r) {
    原子I64特征出生结果 out;out.原请求=r;bool entered=false;
    auto fail=[&](原子I64特征出生状态 s){out.状态=entered?原子I64特征出生状态::已可能发布:s;out.事实.reset();};
    try { std::lock_guard<std::mutex> lock(mutex_);
        const auto& base=内容_.原子I64底座();
        if(r.版本!=1||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()||!有效(r.位置.场景)||!有效(r.位置.组织父)||!有效(r.正式特征类型)||!有效(r.FCv.值)||
            !r.候选读取预算.最大候选数||!r.组织读取预算.最大路径长度||!r.概念读取预算.最大概念数||!r.概念读取预算.最大关系数||!r.概念读取预算.最大特征属性数)
            throw 原子I64特征出生状态::入口拒绝;
        const std::array<L1所有者范围写入幂等身份,5> keys{r.键.组合,r.键.内容,r.键.已知,r.键.组织,r.键.概念使用};
        for(std::size_t i=0;i<keys.size();++i) {if(!有效(keys[i]))throw 原子I64特征出生状态::入口拒绝;for(std::size_t j=0;j<i;++j)if(keys[i]==keys[j])throw 原子I64特征出生状态::入口拒绝;}
        const std::array<L1所有者范围写端口*,4> ownerPorts{&内容_.原子I64端口(),&holder_.原子I64端口(),&组织_.原子I64端口(),&概念_.原子I64端口()};
        std::array<L1所有者范围首次写入读取结果,4> first{};std::size_t present=0;
        for(std::size_t i=0;i<ownerPorts.size();++i) {first[i]=ownerPorts[i]->读取首次写入材料({L1所有者范围首次写入读取合同版本,keys[i+1]});
          if(first[i].合同版本!=L1所有者范围首次写入读取合同版本||first[i].所有者!=ownerPorts[i]->所有者身份()||first[i].写入幂等身份!=keys[i+1])throw 原子I64特征出生状态::内部不一致;
          if(first[i].状态==L1所有者范围读取状态::成功) {if(!first[i].首次规范化写集||!first[i].首次写入结果||first[i].首次写入结果->状态!=L1所有者范围写入状态::成功||!first[i].首次写入结果->是否形成内存权威发布)throw 原子I64特征出生状态::已可能发布;++present;continue;}
          if(first[i].状态==L1所有者范围读取状态::未找到) {if(first[i].首次规范化写集||first[i].首次写入结果)throw 原子I64特征出生状态::内部不一致;continue;}
          if(first[i].状态==L1所有者范围读取状态::资源失败)throw 原子I64特征出生状态::资源失败;throw 原子I64特征出生状态::已可能发布;
        }
        if(present&&present!=4)throw 原子I64特征出生状态::已可能发布;
        if(present==4) {
          const std::array<std::size_t,4> expectedMapSizes{4,1,1,1};
          auto verifyFirst=[&](std::size_t i) {
            const auto& w=*first[i].首次规范化写集;
            const auto& saved=*first[i].首次写入结果;
            if(w.合同版本!=L1所有者范围CRUD合同版本||w.期望事实代次!=r.G0||w.写入幂等身份!=keys[i+1]||
               saved.合同版本!=L1所有者范围CRUD合同版本||saved.所有者!=ownerPorts[i]->所有者身份()||
               saved.写入幂等身份!=keys[i+1]||saved.状态!=L1所有者范围写入状态::成功||
               !saved.是否形成内存权威发布||saved.重试边界!=L1所有者范围重试边界::不适用||
               saved.事实代次!=r.G0+1||saved.新编码映射.size()!=expectedMapSizes[i])
                throw 原子I64特征出生状态::已可能发布;
            std::array<bool,4> mapped{};
            for(const auto& [key,id]:saved.新编码映射) {
              if(!key.值||key.值>mapped.size()||mapped[key.值-1]||!有效(id))
                throw 原子I64特征出生状态::已可能发布;
              mapped[key.值-1]=true;
            }
            for(std::size_t j=0;j<expectedMapSizes[i];++j)
              if(!mapped[j])throw 原子I64特征出生状态::已可能发布;
          };
          for(std::size_t i=0;i<4;++i)verifyFirst(i);
          auto storedId=[&](std::size_t i,std::uint32_t key) {
            for(const auto& [k,v]:first[i].首次写入结果->新编码映射)
              if(k.值==key)return v;
            throw 原子I64特征出生状态::已可能发布;
          };
          const auto storedF=storedId(0,1);
          const auto storedKnown=storedId(1,1);
          const auto storedOrganization=storedId(2,1);
          const auto storedUse=storedId(3,1);
          const auto isLocal=[](const L1所有者范围事实引用& x,std::uint32_t key) {
            const auto* p=std::get_if<L1所有者范围写集本地键>(&x);return p&&p->值==key;
          };
          const auto isStable=[](const L1所有者范围事实引用& x,稳定编码 id) {
            const auto* p=std::get_if<稳定编码>(&x);return p&&*p==id;
          };
          const auto& w1=*first[0].首次规范化写集;
          const auto& w2=*first[1].首次规范化写集;
          const auto& w3=*first[2].首次规范化写集;
          const auto& w4=*first[3].首次规范化写集;
          if(w1.节点.size()!=1||w1.关系.size()!=2||w1.值.size()!=1||w1.属性槽变更.size()!=1||!w1.退出事实.empty()||
             w1.节点[0].本地键.值!=1||w1.节点[0].种类!=节点种类::普通||w1.节点[0].属性类型表示||
             w1.关系[0].本地键.值!=2||!isLocal(w1.关系[0].源节点,1)||w1.关系[0].角色或顺序!=1||
             w1.关系[1].本地键.值!=3||!isLocal(w1.关系[1].源节点,1)||!isStable(w1.关系[1].目标节点,r.正式特征类型)||w1.关系[1].角色或顺序!=1||
             w1.值[0].本地键.值!=4||!isLocal(w1.值[0].所属节点,1)||w1.值[0].材料!=L1所有者范围原始值材料{r.准确I64}||
             !isLocal(w1.属性槽变更[0].所属节点,1)||w1.属性槽变更[0].新当前值.值!=4||
             w2.节点.size()||w2.关系.size()!=1||w2.值.size()||w2.属性槽变更.size()||!w2.退出事实.empty()||
             w2.关系[0].本地键.值!=1||!isStable(w2.关系[0].源节点,r.位置.场景)||!isStable(w2.关系[0].目标节点,storedF)||w2.关系[0].角色或顺序!=1||
             w3.节点.size()||w3.关系.size()!=1||w3.值.size()||w3.属性槽变更.size()||!w3.退出事实.empty()||
             w3.关系[0].本地键.值!=1||!isStable(w3.关系[0].源节点,r.位置.组织父)||!isStable(w3.关系[0].目标节点,storedF)||w3.关系[0].角色或顺序!=1||
             w4.节点.size()||w4.关系.size()!=1||w4.值.size()||w4.属性槽变更.size()||!w4.退出事实.empty()||
             w4.关系[0].本地键.值!=1||!isStable(w4.关系[0].源节点,storedF)||!isStable(w4.关系[0].目标节点,r.FCv.值)||w4.关系[0].角色或顺序!=1)
            throw 原子I64特征出生状态::已可能发布;
          const auto h=first[0].首次写入结果->事实代次;for(std::size_t i=1;i<4;++i)if(first[i].首次写入结果->事实代次!=h)throw 原子I64特征出生状态::已可能发布;
          if(h!=r.G0+1)throw 原子I64特征出生状态::已可能发布;
          auto rebuild=[](const L1所有者范围写集请求& w) { L1有限N分区原子写集请求_v3 x{w.合同版本,w.期望事实代次,w.写入幂等身份};x.退出事实=w.退出事实;
            const auto convert=[](const L1所有者范围事实引用& ref) -> L1有限N分区原子事实引用值_v3 {
              return std::visit([](const auto& value) -> L1有限N分区原子事实引用值_v3 { return value; },ref);
            };
            for(const auto& n:w.节点)x.节点.push_back({n.本地键,n.种类,n.属性类型表示});
            for(const auto& e:w.关系)x.关系.push_back({e.本地键,convert(e.源节点),convert(e.目标节点),convert(e.关系类型节点),e.角色或顺序});
            for(const auto& v:w.值)x.值.push_back({v.本地键,convert(v.所属节点),convert(v.属性类型节点),v.材料,convert(v.来源节点)});
            for(const auto& a:w.属性槽变更)x.属性槽变更.push_back({convert(a.所属节点),convert(a.属性类型节点),a.新当前值});return x; };
          L1有限N分区原子事务请求_v3 replay{L1有限N分区原子事务合同版本_v3,r.G0,r.键.组合,{}};
          for(std::size_t i=0;i<4;++i)replay.参与者写集组.push_back({{static_cast<std::uint8_t>(i+1)},ownerPorts[i]->所有者身份(),rebuild(*first[i].首次规范化写集)});
          const std::vector<const L1所有者范围写端口*> others{ownerPorts[1],ownerPorts[2],ownerPorts[3]};
          entered=true;const auto saved=内容_.原子I64端口().提交有限N分区原子事务_v3(replay,others);using L=L1有限N分区原子事务状态_v3;
          if(saved.合同版本!=L1有限N分区原子事务合同版本_v3||saved.组合写入幂等身份!=r.键.组合||
             saved.状态!=L::精确重复||saved.共同事实代次!=h||saved.参与者结果组.size()!=4||saved.是否已确认形成内存权威发布||saved.重试边界!=L1所有者范围重试边界::原幂等身份读回收敛)throw 原子I64特征出生状态::已可能发布;
          std::array<bool,4> replaySeen{};
          for(const auto& p:saved.参与者结果组) {
            if(!p.参与者.值||p.参与者.值>4||replaySeen[p.参与者.值-1]||
               p.所有者!=ownerPorts[p.参与者.值-1]->所有者身份()||
               p.新编码映射!=first[p.参与者.值-1].首次写入结果->新编码映射)
              throw 原子I64特征出生状态::已可能发布;
            replaySeen[p.参与者.值-1]=true;
          }
          for(bool seen:replaySeen)if(!seen)throw 原子I64特征出生状态::已可能发布;
          const auto F=storedF,known=storedKnown,org=storedOrganization,use=storedUse;const auto tail=base.读取中性当前事实代次({L1中性CRUD合同版本});
          if(tail.状态!=L1中性读取状态::成功||tail.事实代次<h)throw 原子I64特征出生状态::已可能发布;const auto read=读取({1,tail.事实代次,F,r.位置,r.组织读取预算,r.概念读取预算});
          if(read.状态!=原子I64特征出生状态::已读取||!read.事实||read.事实->已知关系!=known||read.事实->组织关系!=org||read.事实->概念使用关系!=use||read.事实->正式特征类型!=r.正式特征类型||read.事实->准确I64!=r.准确I64||read.事实->FCv!=r.FCv.值||read.事实->出生事实代次!=h)throw 原子I64特征出生状态::已可能发布;
          out.Gread=tail.事实代次;out.首次发布代次=h;out.事实=*read.事实;out.状态=原子I64特征出生状态::精确重复;return out;
        }
        const auto current=base.读取中性当前事实代次({L1中性CRUD合同版本});
        if(current.状态!=L1中性读取状态::成功||current.事实代次!=r.G0)throw 原子I64特征出生状态::事实代次漂移;
        if(!内容_.原子I64结构已就绪()||!holder_.原子I64结构已就绪()||!组织_.原子I64结构已就绪()||!概念_.原子I64结构已就绪())throw 原子I64特征出生状态::入口拒绝;
        auto take=[&](auto part,std::uint8_t expected) {if(part.状态!=原子I64特征出生状态::已创建)throw part.状态;if(part.Gread!=r.G0||!part.数据||part.数据->参与者.值!=expected||part.数据->所有者!=ownerPorts[expected-1]->所有者身份()||part.数据->写集.合同版本!=L1所有者范围CRUD合同版本||part.数据->写集.期望事实代次!=r.G0||part.数据->写集.写入幂等身份!=keys[expected])throw 原子I64特征出生状态::内部不一致;return std::move(*part.数据);};
        L1有限N分区原子事务请求_v3 transaction{L1有限N分区原子事务合同版本_v3,r.G0,r.键.组合,{}};
        transaction.参与者写集组.push_back(take(内容_.准备原子I64出生片段(r,r.G0),1));
        transaction.参与者写集组.push_back(take(holder_.准备原子I64出生片段(r,r.G0),2));
        transaction.参与者写集组.push_back(take(组织_.准备原子I64出生片段(r,r.G0),3));
        transaction.参与者写集组.push_back(take(概念_.准备原子I64出生片段(r,r.G0),4));
        const std::vector<const L1所有者范围写端口*> others{ownerPorts[1],ownerPorts[2],ownerPorts[3]};
        entered=true;const auto saved=内容_.原子I64端口().提交有限N分区原子事务_v3(transaction,others);
        using L=L1有限N分区原子事务状态_v3;
        if(saved.状态!=L::已提交&&saved.状态!=L::精确重复) {
            if(saved.状态==L::已可能发布) {out.首次发布代次 = saved.共同事实代次 ? saved.共同事实代次 : std::optional<std::uint64_t>{};throw 原子I64特征出生状态::已可能发布;}
            entered=false;switch(saved.状态){case L::事实代次漂移:throw 原子I64特征出生状态::事实代次漂移;case L::幂等冲突:throw 原子I64特征出生状态::幂等冲突;case L::资源失败:throw 原子I64特征出生状态::资源失败;case L::入口拒绝:throw 原子I64特征出生状态::入口拒绝;default:throw 原子I64特征出生状态::内部不一致;}
        }
        if(saved.合同版本!=L1有限N分区原子事务合同版本_v3||saved.组合写入幂等身份!=r.键.组合||
           saved.共同事实代次!=r.G0+1||saved.参与者结果组.size()!=4||
           saved.是否已确认形成内存权威发布!=(saved.状态==L::已提交)||
           saved.重试边界!=(saved.状态==L::已提交 ? L1所有者范围重试边界::不适用 : L1所有者范围重试边界::原幂等身份读回收敛))throw 原子I64特征出生状态::已可能发布;
        std::array<std::vector<std::pair<L1所有者范围写集本地键,稳定编码>>,4> maps;std::array<bool,4> seen{};
        const std::array<std::size_t,4> expectedMapSizes{4,1,1,1};
        for(const auto& p:saved.参与者结果组){if(!p.参与者.值||p.参与者.值>4||seen[p.参与者.值-1]||p.所有者!=ownerPorts[p.参与者.值-1]->所有者身份()||p.新编码映射.size()!=expectedMapSizes[p.参与者.值-1])throw 原子I64特征出生状态::已可能发布;seen[p.参与者.值-1]=true;maps[p.参与者.值-1]=p.新编码映射;}
        for(std::size_t i=0;i<4;++i) {if(!seen[i])throw 原子I64特征出生状态::已可能发布;std::array<bool,4> mapped{};for(const auto& [key,id]:maps[i]) {if(!key.值||key.值>mapped.size()||mapped[key.值-1]||!有效(id))throw 原子I64特征出生状态::已可能发布;mapped[key.值-1]=true;}for(std::size_t key=0;key<expectedMapSizes[i];++key)if(!mapped[key])throw 原子I64特征出生状态::已可能发布;}
        auto id=[&](std::size_t i,std::uint32_t key){for(const auto& [k,v]:maps[i])if(k.值==key)return v;throw 原子I64特征出生状态::已可能发布;};
        const auto F=id(0,1),known=id(1,1),org=id(2,1),use=id(3,1);out.首次发布代次=saved.共同事实代次;
        const auto tail=base.读取中性当前事实代次({L1中性CRUD合同版本});
        if(tail.状态!=L1中性读取状态::成功||tail.事实代次<*out.首次发布代次)throw 原子I64特征出生状态::已可能发布;
        const auto read=读取({1,tail.事实代次,F,r.位置,r.组织读取预算,r.概念读取预算});
        if(read.状态!=原子I64特征出生状态::已读取||!read.事实||read.事实->已知关系!=known||read.事实->组织关系!=org||read.事实->概念使用关系!=use)throw 原子I64特征出生状态::已可能发布;
        if(read.事实->正式特征类型!=r.正式特征类型||read.事实->准确I64!=r.准确I64||read.事实->FCv!=r.FCv.值||read.事实->出生事实代次!=*out.首次发布代次)throw 原子I64特征出生状态::已可能发布;
        out.Gread=tail.事实代次;out.事实=*read.事实;
        out.状态 = saved.状态 == L::精确重复 ? 原子I64特征出生状态::精确重复 : 原子I64特征出生状态::已创建;
    } catch(原子I64特征出生状态 s){fail(s);}catch(const std::bad_alloc&){fail(原子I64特征出生状态::资源失败);}catch(...){fail(原子I64特征出生状态::内部不一致);}return out;
}
inline 原子I64特征出生读取结果 原子I64特征出生数据服务::读取(const 原子I64特征出生读取请求& r) const {
    原子I64特征出生读取结果 out;out.Gread=r.Gread;
    auto mapNarrow=[](原子I64特征窄读取状态 s) {
        using N=原子I64特征窄读取状态;using B=原子I64特征出生状态;
        switch(s) {case N::入口拒绝:return B::入口拒绝;case N::未找到:return B::未找到;
            case N::位置冲突:return B::位置冲突;
            case N::事实代次漂移:return B::事实代次漂移;case N::预算不足:return B::预算不足;
            case N::资源失败:return B::资源失败;
            default:return B::内部不一致;}
    };
    auto mapUse=[](原子I64特征出生使用读取状态 s) {
        using U=原子I64特征出生使用读取状态;using B=原子I64特征出生状态;
        switch(s) {case U::入口拒绝:return B::入口拒绝;case U::未找到:return B::未找到;
            case U::概念不适配:return B::概念不适配;
            case U::事实代次漂移:return B::事实代次漂移;case U::预算不足:return B::预算不足;
            case U::资源失败:return B::资源失败;
            default:return B::内部不一致;}
    };
    if(r.版本!=1||!r.Gread||!有效(r.F)||!r.概念读取预算.最大概念数||!r.概念读取预算.最大关系数||!r.概念读取预算.最大特征属性数)
        return out;
    if(!有效(r.位置.场景)||!有效(r.位置.组织父)||!r.组织读取预算.最大路径长度)return out;
    const auto content=内容_.读取原子I64内容({1,r.Gread,r.F});
    if(content.状态!=原子I64特征窄读取状态::已读取||!content.事实) {out.状态=mapNarrow(content.状态);return out;}
    if(content.Gread!=r.Gread) {out.状态=原子I64特征出生状态::内部不一致;return out;}
    const auto holder=holder_.读取原子I64holder({1,r.Gread,r.位置.场景,r.F});
    if(holder.状态!=原子I64特征窄读取状态::已读取||!holder.事实) {out.状态=mapNarrow(holder.状态);return out;}
    if(holder.Gread!=r.Gread) {out.状态=原子I64特征出生状态::内部不一致;return out;}
    const auto tree=组织_.读取原子I64组织({1,r.Gread,r.位置.场景,r.位置.组织父,r.F,r.组织读取预算});
    if(tree.状态!=原子I64特征窄读取状态::已读取||!tree.事实) {out.状态=mapNarrow(tree.状态);return out;}
    if(tree.Gread!=r.Gread) {out.状态=原子I64特征出生状态::内部不一致;return out;}
    const auto use=概念_.读取原子I64出生使用({1,r.Gread,r.F,r.概念读取预算});
    if(use.状态!=原子I64特征出生使用读取状态::已读取||!use.事实) {out.状态=mapUse(use.状态);return out;}
    if(use.Gread!=r.Gread) {out.状态=原子I64特征出生状态::内部不一致;return out;}
    const auto birth=content.事实->创建事实代次;
    if(content.事实->F!=r.F||!有效(content.事实->正式特征类型)||!有效(content.事实->类型关系)||!有效(content.事实->I64值事实)||!birth||birth>r.Gread||holder.事实->holder!=r.位置.场景||holder.事实->F!=r.F||!有效(holder.事实->已知关系)||!有效(holder.事实->关系类型)||holder.事实->创建事实代次!=birth||tree.事实->C!=r.位置.场景||tree.事实->P!=r.位置.组织父||tree.事实->F!=r.F||!有效(tree.事实->组织关系)||!有效(tree.事实->组织关系类型)||tree.事实->创建事实代次!=birth||tree.事实->路径.empty()||tree.事实->路径.back().关系!=tree.事实->组织关系||tree.事实->路径.back().源!=r.位置.组织父||tree.事实->路径.back().目标!=r.F||use.事实->F.编码!=r.F||!有效(use.事实->关系)||!有效(use.事实->FCv.值)||use.事实->生命周期.创建事实代次!=birth) {out.状态=原子I64特征出生状态::内部不一致;return out;}
    out.事实=原子I64特征出生事实{r.F,content.事实->正式特征类型,use.事实->FCv.值,content.事实->准确I64,r.位置,r.位置.场景,holder.事实->已知关系,tree.事实->组织关系,use.事实->关系,birth};out.状态=原子I64特征出生状态::已读取;
    return out;
}
inline bool 原子I64特征出生结果::成功() const noexcept {
    return 版本==1&&原请求.版本==1&&(状态==原子I64特征出生状态::已创建||状态==原子I64特征出生状态::精确重复)&&事实.has_value()&&有效(事实->F)&&有效(事实->FCv)&&有效(事实->概念使用关系);
}
inline bool 原子I64特征出生读取结果::成功(const 原子I64特征出生读取请求& r) const noexcept {
    return 版本==1&&r.版本==1&&状态==原子I64特征出生状态::已读取&&Gread==r.Gread&&事实.has_value()&&事实->F==r.F;
}


}
