#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <limits>
#include <new>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "合同.动态结构.h"
#include "数据服务.特征值类.h"

namespace 海中鱼巣 {

class 动态类数据服务 final : public 动态结构只读提供者 {
public:
    动态类数据服务() = delete;
    动态类数据服务(const 动态类数据服务&) = delete;
    动态类数据服务& operator=(const 动态类数据服务&) = delete;
    动态类数据服务(动态类数据服务&&) = delete;
    动态类数据服务& operator=(动态类数据服务&&) = delete;

    动态类数据服务(L1事实基座服务& l1,
        L1所有者范围写端口&& port,
        const 动态结构交付& layout,
        const 存在结构身份只读提供者& existence,
        const 状态使用绑定只读提供者& binding,
        const 状态类数据服务& state,
        const 特征类数据服务& feature,
        const 特征值类数据服务& value)
        : l1_(l1), port_(std::move(port)), owner_(port_.所有者身份()),
          layout_(layout), existence_(existence), binding_(binding), state_(state),
          feature_(feature), value_(value) {
        if (!绑定于(l1_) || !有效(owner_) || !布局浅层有效())
            throw std::invalid_argument("invalid dynamic configuration");
        const auto o=l1_.读取当前结构所有者({L1所有者范围CRUD合同版本,owner_});
        if(o.状态!=L1所有者范围读取状态::成功||!o.所有者事实
            ||o.所有者事实->所有者!=owner_
            ||o.所有者事实->范围种类!=L1所有者范围种类::独占结构范围
            ||!布局材料有效())
            throw std::invalid_argument("invalid dynamic structure");
    }

    bool 绑定于(const L1事实基座服务& s) const noexcept override {
        return &s==&l1_&&port_.绑定于(s)&&existence_.绑定于(s)&&binding_.绑定于(s)
            &&state_.绑定于(s)&&feature_.绑定于(s)&&value_.绑定于(s);
    }

    动态单项结果 创建原子动态(const 动态原子创建请求& r) {
        if(!原子请求有效(r))return 单项失败(动态数据状态::入口拒绝);
        try {
            if(auto replay=重放创建(r))return *replay;
            const auto source=核验原子来源(r);
            if(source.状态!=动态数据状态::已读取)return 单项失败(source.状态,source.Gread);
            const auto utc=当前UTC纳秒();
            if(utc<0)return 单项失败(动态数据状态::资源失败,r.G0);
            return 提交创建(形成原子写集(r,utc),r.幂等身份,r.预算);
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态单项结果 创建组合动态(const 动态组合创建请求& r) {
        if(!组合请求有效(r))return 单项失败(动态数据状态::入口拒绝);
        try {
            if(auto replay=重放创建(r))return *replay;
            std::set<std::uint64_t> visited,active;
            std::uint64_t dynamics=0,edges=0,materials=0,maxDepth=0;
            for(const auto&m:r.变化.成员组) {
                if(m.角色!=动态成员角色::同主体)
                    return 单项失败(动态数据状态::组成发生期依据不足,r.G0);
                const auto s=核验组合来源(m.子动态,r.主体存在,r.G0,r.预算,1,
                    visited,active,dynamics,edges,materials,maxDepth);
                if(s!=动态数据状态::已读取)return 单项失败(s,r.G0);
            }
            const auto memberCount=static_cast<std::uint64_t>(r.变化.成员组.size());
            if(dynamics>=r.预算.最大动态数||edges>r.预算.最大来源边数-memberCount
                ||materials>r.预算.最大读取材料数-(4+memberCount)
                ||maxDepth>=r.预算.最大深度)
                return 单项失败(动态数据状态::数量预算不足,r.G0);
            const auto utc=当前UTC纳秒();
            if(utc<0)return 单项失败(动态数据状态::资源失败,r.G0);
            return 提交创建(形成组合写集(r,utc),r.幂等身份,r.预算);
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态单项结果 读取当前动态(const 动态当前读取请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.身份)||!预算有效(r.预算))
            return 单项失败(动态数据状态::入口拒绝);
        try {
            const auto before=读取当前代次();
            if(before.状态!=动态数据状态::已读取)return 单项失败(before.状态,before.Gread);
            if(before.Gread!=r.G0)return 单项失败(动态数据状态::事实代次漂移,before.Gread);
            auto out=读取当前内容(r.G0,r.身份,r.预算);
            if(out.操作.状态!=动态数据状态::已读取)return out;
            const auto after=读取当前代次();
            if(after.状态!=动态数据状态::已读取)return 单项失败(after.状态,after.Gread);
            return after.Gread==r.G0?out:单项失败(动态数据状态::事实代次漂移,after.Gread);
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态组结果 按主体查询动态(const 动态按主体查询请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.主体存在)||!预算有效(r.预算))
            return 组失败(动态数据状态::入口拒绝);
        const auto source=existence_.确认当前存在结构身份(r.G0,r.主体存在);
        if(source.状态!=存在结构身份只读状态::已读取||source.合同版本!=存在结构身份只读合同版本
            ||source.Gread!=r.G0)return 组失败(映射存在(source.状态),source.Gread);
        return 查询关系(r.G0,r.主体存在,layout_.主体关系类型,查询方向::目标,r.预算);
    }

    动态组结果 按绑定反查原子动态(const 动态按绑定查询请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.绑定)||!预算有效(r.预算))
            return 组失败(动态数据状态::入口拒绝);
        const auto source=binding_.读取当前状态使用绑定({1,r.G0,r.绑定});
        if(!绑定当前成功(source,r.G0,r.绑定))
            return 组失败(映射绑定(source.结果头.状态),source.结果头.Gread);
        auto a=查询关系(r.G0,r.绑定.编码,layout_.前绑定关系类型,查询方向::目标,r.预算);
        if(a.操作.状态!=动态数据状态::已读取)return a;
        auto b=查询关系(r.G0,r.绑定.编码,layout_.后绑定关系类型,查询方向::目标,r.预算);
        if(b.操作.状态!=动态数据状态::已读取)return b;
        a.动态组.insert(a.动态组.end(),b.动态组.begin(),b.动态组.end());
        std::sort(a.动态组.begin(),a.动态组.end(),内容身份小于);
        a.动态组.erase(std::unique(a.动态组.begin(),a.动态组.end(),内容身份相同),a.动态组.end());
        return a.动态组.size()<=r.预算.最大动态数?a:组失败(动态数据状态::数量预算不足,r.G0);
    }

    动态组结果 按子动态反查父动态(const 动态按子动态查询请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.子动态)||!预算有效(r.预算))
            return 组失败(动态数据状态::入口拒绝);
        const auto source=读取当前身份(r.G0,r.子动态);
        if(source!=动态数据状态::已读取)return 组失败(映射来源动态(source),r.G0);
        return 查询关系(r.G0,r.子动态.编码,layout_.同主体成员关系类型,查询方向::目标,r.预算);
    }

    动态展开结果 展开动态来源(const 动态来源展开请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.身份)
            ||(r.方式!=动态展开方式::直接&&r.方式!=动态展开方式::递归)||!预算有效(r.预算))
            return 展开失败(动态数据状态::入口拒绝);
        try {
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 展开失败(guard.状态,guard.Gread);
            if(guard.Gread!=r.G0)return 展开失败(动态数据状态::事实代次漂移,guard.Gread);
            动态来源展开事实 facts{r.G0,r.身份,{},{}};
            std::set<std::uint64_t> visited,active;
            std::uint64_t edges=0,materials=0;
            const auto s=展开一个(r.身份,r.方式,r.预算,1,visited,active,edges,materials,facts);
            if(s!=动态数据状态::已读取)return 展开失败(s,r.G0);
            std::sort(facts.动态组.begin(),facts.动态组.end(),内容身份小于);
            std::sort(facts.原子来源组.begin(),facts.原子来源组.end(),
                [](const auto&a,const auto&b){return a.动态.编码<b.动态.编码;});
            const auto after=读取当前代次();
            if(after.状态!=动态数据状态::已读取)return 展开失败(after.状态,after.Gread);
            if(after.Gread!=r.G0)return 展开失败(动态数据状态::事实代次漂移,after.Gread);
            return {读取头(r.G0),std::move(facts)};
        } catch(const std::bad_alloc&) {return 展开失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 展开失败(动态数据状态::资源失败);}
        catch(...) {return 展开失败(动态数据状态::内部不一致);}
    }

    动态操作结果 确认当前动态结构身份(const 动态当前身份请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.身份))return 失败头(动态数据状态::入口拒绝);
        const auto guard=读取当前代次();
        if(guard.状态!=动态数据状态::已读取)return 失败头(guard.状态,guard.Gread);
        if(guard.Gread!=r.G0)return 失败头(动态数据状态::事实代次漂移,guard.Gread);
        const auto s=读取当前身份(r.G0,r.身份);
        if(s!=动态数据状态::已读取)return 失败头(s,r.G0);
        const auto after=读取当前代次();
        return after.状态==动态数据状态::已读取&&after.Gread==r.G0
            ?读取头(r.G0):失败头(after.状态==动态数据状态::已读取?动态数据状态::事实代次漂移:after.状态,after.Gread);
    }

    动态单项结果 退出动态(const 动态退出请求& r) {
        if(r.合同版本!=1||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()
            ||!有效(r.幂等身份)||!有效(r.身份)||!预算有效(r.预算))
            return 单项失败(动态数据状态::入口拒绝);
        try {
            if(auto replay=重放退出(r))return *replay;
            const auto current=读取当前动态({1,r.G0,r.身份,r.预算});
            if(current.操作.状态!=动态数据状态::已读取||!current.内容)
                return 单项失败(current.操作.状态,current.操作.Gread);
            auto ws=形成退出写集(*current.内容,r.G0,r.幂等身份);
            const auto saved=port_.提交所有者范围中性写集(ws);
            const auto status=映射写入(saved.状态,动态数据状态::已删除);
            if(status!=动态数据状态::已删除&&status!=动态数据状态::精确重复)
                return 提交失败(status,saved.事实代次);
            if(!写入头完整(saved,r.幂等身份,ws.期望事实代次)||!saved.新编码映射.empty())
                return 提交后失败(saved.事实代次);
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取||guard.Gread!=saved.事实代次)
                return 提交后失败(saved.事实代次,guard.Gread);
            const auto missing=读取当前身份(guard.Gread,r.身份);
            if(missing!=动态数据状态::未找到)return 提交后失败(saved.事实代次,guard.Gread);
            return {{status,1,guard.Gread,saved.事实代次,动态发布阶段::已读回},std::nullopt};
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

private:
    inline static constexpr L1所有者范围写集本地键 节点键{1},族关系键{2},主体关系键{3},首次UTC键{4};
    enum class 查询方向{源,目标};
    struct 代次结果{动态数据状态 状态=动态数据状态::内部不一致;std::uint64_t Gread=0;};
    struct 来源核验结果{动态数据状态 状态=动态数据状态::内部不一致;std::uint64_t Gread=0;};
    struct I64读取结果{动态数据状态 状态=动态数据状态::内部不一致;std::optional<std::int64_t> 值;};

    static bool 预算有效(const 动态读取预算&v)noexcept{return v.最大候选数&&v.最大动态数
        &&v.最大来源边数&&v.最大深度&&v.最大读取材料数;}
    static bool 原子请求有效(const 动态原子创建请求&r)noexcept{return r.合同版本==1&&r.G0
        &&r.G0!=std::numeric_limits<std::uint64_t>::max()&&有效(r.幂等身份)&&有效(r.主体存在)
        &&有效(r.变化.前绑定)&&有效(r.变化.后绑定)&&r.变化.前绑定!=r.变化.后绑定&&预算有效(r.预算);}
    static bool 组合请求有效(const 动态组合创建请求&r)noexcept{
        if(r.合同版本!=1||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()
            ||!有效(r.幂等身份)||!有效(r.主体存在)||r.变化.成员组.empty()||!预算有效(r.预算))return false;
        for(std::size_t i=0;i<r.变化.成员组.size();++i){const auto&m=r.变化.成员组[i];
            if(m.顺序!=i+1||!有效(m.子动态)||m.角色!=动态成员角色::同主体)return false;
            for(std::size_t j=0;j<i;++j)if(r.变化.成员组[j].子动态==m.子动态)return false;}
        return true;}
    static std::int64_t 当前UTC纳秒()noexcept{return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();}
    代次结果 读取当前代次()const{const auto q=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
        if(q.状态==L1中性读取状态::资源失败)return{动态数据状态::资源失败,q.事实代次};
        if(q.状态!=L1中性读取状态::成功||q.合同版本!=L1中性CRUD合同版本||!q.事实代次)
            return{动态数据状态::内部不一致,q.事实代次};
        return{动态数据状态::已读取,q.事实代次};}

    bool 布局浅层有效()const noexcept{const 稳定编码 ids[]{layout_.格式锚点,layout_.字段登记关系类型,layout_.动态族锚点,
        layout_.族归属关系类型,layout_.主体关系类型,layout_.前绑定关系类型,layout_.后绑定关系类型,
        layout_.同主体成员关系类型,layout_.首次形成UTC属性类型};
        for(std::size_t i=0;i<9;++i){if(!有效(ids[i]))return false;for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;}return true;}
    bool 节点类型有效(稳定编码 id,节点种类 kind,std::optional<L1所有者范围值表示种类> repr)const{
        const auto q=l1_.读取所有者范围当前事实({L1所有者范围当前事实读取合同版本_v2,owner_,id,读取当前代次().Gread});
        const auto*n=q.载荷 ? std::get_if<L1所有者范围节点事实>(&*q.载荷) : nullptr;
        return q.状态==L1所有者范围当前事实读取状态_v2::成功&&n&&n->编码==id&&n->写入所有者==owner_
            &&n->种类==kind&&n->属性类型表示==repr;}
    bool 布局材料有效()const{
        const 稳定编码 plain[]{layout_.格式锚点,layout_.字段登记关系类型,layout_.动态族锚点,layout_.族归属关系类型,
            layout_.主体关系类型,layout_.前绑定关系类型,layout_.后绑定关系类型,layout_.同主体成员关系类型};
        for(auto id:plain)if(!节点类型有效(id,节点种类::普通,std::nullopt))return false;
        if(!节点类型有效(layout_.首次形成UTC属性类型,节点种类::属性类型,L1所有者范围值表示种类::I64))return false;
        const auto q=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,layout_.格式锚点,layout_.字段登记关系类型});
        if(q.状态!=L1所有者范围读取状态::成功||q.关系组.size()!=7)return false;
        const 稳定编码 expected[]{layout_.动态族锚点,layout_.族归属关系类型,layout_.主体关系类型,layout_.前绑定关系类型,
            layout_.后绑定关系类型,layout_.同主体成员关系类型,layout_.首次形成UTC属性类型};
        for(std::size_t i=0;i<7;++i){std::size_t count=0;for(const auto&e:q.关系组)if(e.角色或顺序==static_cast<std::int64_t>(i+1)){
            if(e.写入所有者!=owner_||e.源节点!=layout_.格式锚点||e.关系类型节点!=layout_.字段登记关系类型
                ||e.目标节点!=expected[i])return false;++count;}if(count!=1)return false;}
        return true;}

    static 动态操作结果 失败头(动态数据状态 s,std::uint64_t g=0)noexcept{return{s,1,g,std::nullopt,动态发布阶段::无写入};}
    static 动态操作结果 读取头(std::uint64_t g)noexcept{return{动态数据状态::已读取,1,g,std::nullopt,动态发布阶段::无写入};}
    static 动态单项结果 单项失败(动态数据状态 s,std::uint64_t g=0)noexcept{return{失败头(s,g),std::nullopt};}
    static 动态组结果 组失败(动态数据状态 s,std::uint64_t g=0)noexcept{return{失败头(s,g),{}};}
    static 动态展开结果 展开失败(动态数据状态 s,std::uint64_t g=0)noexcept{return{失败头(s,g),std::nullopt};}
    static 动态单项结果 提交失败(动态数据状态 s,std::uint64_t g)noexcept{
        if(s==动态数据状态::已可能发布)return{{s,1,g,g?std::optional<std::uint64_t>{g}:std::nullopt,动态发布阶段::未知},std::nullopt};
        return 单项失败(s,g);}
    static 动态单项结果 提交后失败(std::uint64_t published,std::uint64_t guard=0)noexcept{
        return{{动态数据状态::内部不一致,1,guard,published,动态发布阶段::已提交待读回},std::nullopt};}
    static bool 内容身份小于(const 动态内容事实&a,const 动态内容事实&b){return a.信息.身份.编码<b.信息.身份.编码;}
    static bool 内容身份相同(const 动态内容事实&a,const 动态内容事实&b){return a.信息.身份==b.信息.身份;}
    static bool 绑定当前成功(const 状态使用绑定读取结果&r,std::uint64_t g,状态使用绑定身份 id)noexcept{
        return r.结果头.状态==状态使用绑定数据状态::已读取&&r.结果头.合同版本==场景状态使用绑定合同版本
            &&r.结果头.Gread==g&&!r.结果头.变更事实代次&&r.绑定&&r.绑定->Gread==g&&r.绑定->信息.身份==id;}
    static bool 状态当前成功(const 状态读取结果&r,std::uint64_t g,状态信息身份 id)noexcept{
        return r.结果头.状态==状态类数据状态::已读取&&r.结果头.合同版本==状态类数据合同版本
            &&r.结果头.事实截止代次==g&&!r.结果头.变更事实代次&&r.内容&&r.内容->Gread==g&&r.内容->信息.身份==id;}

    static std::optional<稳定编码> 映射编码(const L1所有者范围写入结果&r,L1所有者范围写集本地键 key){std::optional<稳定编码> out;
        for(const auto&[k,id]:r.新编码映射)if(k==key){if(out||!有效(id))return std::nullopt;out=id;}return out;}
    bool 写入头完整(const L1所有者范围写入结果&r,L1所有者范围写入幂等身份 key,std::uint64_t expected)const noexcept{
        const bool first=r.状态==L1所有者范围写入状态::成功&&r.是否形成内存权威发布&&r.重试边界==L1所有者范围重试边界::不适用;
        const bool replay=r.状态==L1所有者范围写入状态::精确重复&&!r.是否形成内存权威发布&&r.重试边界==L1所有者范围重试边界::原幂等身份读回收敛;
        return expected!=std::numeric_limits<std::uint64_t>::max()&&r.合同版本==L1所有者范围CRUD合同版本&&r.所有者==owner_
            &&r.写入幂等身份==key&&r.事实代次==expected+1&&(first||replay);}

    L1所有者范围写集请求 形成原子写集(const 动态原子创建请求&r,std::int64_t utc)const{
        L1所有者范围写集请求 w{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};w.节点={{节点键,节点种类::普通,std::nullopt}};
        w.关系={{族关系键,节点键,layout_.动态族锚点,layout_.族归属关系类型,1},{主体关系键,节点键,r.主体存在,layout_.主体关系类型,0},
            {{5},节点键,r.变化.前绑定.编码,layout_.前绑定关系类型,0},{{6},节点键,r.变化.后绑定.编码,layout_.后绑定关系类型,0}};
        w.值={{首次UTC键,节点键,layout_.首次形成UTC属性类型,utc,节点键}};w.属性槽变更={{节点键,layout_.首次形成UTC属性类型,首次UTC键}};return w;}
    L1所有者范围写集请求 形成组合写集(const 动态组合创建请求&r,std::int64_t utc)const{
        L1所有者范围写集请求 w{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};w.节点={{节点键,节点种类::普通,std::nullopt}};
        w.关系={{族关系键,节点键,layout_.动态族锚点,layout_.族归属关系类型,1},{主体关系键,节点键,r.主体存在,layout_.主体关系类型,0}};
        for(const auto&m:r.变化.成员组)w.关系.push_back({{0x100u+m.顺序},节点键,m.子动态.编码,layout_.同主体成员关系类型,static_cast<std::int64_t>(m.顺序)});
        w.值={{首次UTC键,节点键,layout_.首次形成UTC属性类型,utc,节点键}};w.属性槽变更={{节点键,layout_.首次形成UTC属性类型,首次UTC键}};return w;}

    template<class R> std::optional<动态单项结果> 重放创建(const R&r){
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||first.所有者!=owner_||first.写入幂等身份!=r.幂等身份)
            return 单项失败(动态数据状态::内部不一致,first.读取事实代次);
        if(first.状态==L1所有者范围读取状态::未找到){if(!first.读取事实代次||first.首次规范化写集||first.首次写入结果)
                return 单项失败(动态数据状态::内部不一致,first.读取事实代次);return std::nullopt;}
        if(first.状态!=L1所有者范围读取状态::成功||!first.首次规范化写集||!first.首次写入结果)
            return 单项失败(first.状态==L1所有者范围读取状态::成功?动态数据状态::内部不一致:映射读取(first.状态),first.读取事实代次);
        const auto&ws=*first.首次规范化写集;std::optional<std::int64_t>utc;
        for(const auto&v:ws.值)if(v.本地键==首次UTC键){if(const auto*n=std::get_if<std::int64_t>(&v.材料))utc=*n;}
        if(!utc)return 单项失败(动态数据状态::幂等冲突,first.读取事实代次);
        const auto expected=[&](){if constexpr(std::is_same_v<R,动态原子创建请求>)return 形成原子写集(r,*utc);else return 形成组合写集(r,*utc);}();
        if(ws!=expected)return 单项失败(动态数据状态::幂等冲突,first.读取事实代次);
        const auto saved=port_.提交所有者范围中性写集(ws);
        if(saved.状态!=L1所有者范围写入状态::精确重复||!写入头完整(saved,r.幂等身份,ws.期望事实代次))
            return 提交失败(映射写入(saved.状态,动态数据状态::已创建),saved.事实代次);
        const auto id=映射编码(saved,节点键);if(!id)return 提交后失败(saved.事实代次);
        const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取)return 提交后失败(saved.事实代次,guard.Gread);
        auto read=读取当前动态({1,guard.Gread,{*id},r.预算});
        if(read.操作.状态!=动态数据状态::已读取||!read.内容)return 提交后失败(saved.事实代次,read.操作.Gread);
        return 动态单项结果{{动态数据状态::精确重复,1,guard.Gread,saved.事实代次,动态发布阶段::已读回},std::move(read.内容)};
    }

    动态单项结果 提交创建(L1所有者范围写集请求 ws,L1所有者范围写入幂等身份 key,动态读取预算 budget){
        const auto saved=port_.提交所有者范围中性写集(ws);const auto s=映射写入(saved.状态,动态数据状态::已创建);
        if(s!=动态数据状态::已创建&&s!=动态数据状态::精确重复)return 提交失败(s,saved.事实代次);
        if(!写入头完整(saved,key,ws.期望事实代次))return 提交后失败(saved.事实代次);
        const auto id=映射编码(saved,节点键);if(!id)return 提交后失败(saved.事实代次);
        const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取)return 提交后失败(saved.事实代次,guard.Gread);
        const auto read=读取当前动态({1,guard.Gread,{*id},budget});
        if(read.操作.状态!=动态数据状态::已读取||!read.内容||read.内容->创建事实代次!=saved.事实代次)
            return 提交后失败(saved.事实代次,read.操作.Gread);
        return{{s,1,guard.Gread,saved.事实代次,动态发布阶段::已读回},read.内容};
    }

    来源核验结果 核验原子来源(const 动态原子创建请求&r)const{
        const auto ew=existence_.确认当前存在结构身份(r.G0,r.主体存在);
        if(ew.状态!=存在结构身份只读状态::已读取||ew.合同版本!=存在结构身份只读合同版本||ew.Gread!=r.G0)
            return{映射存在(ew.状态),ew.Gread};
        const auto before=binding_.读取当前状态使用绑定({1,r.G0,r.变化.前绑定});
        const auto after=binding_.读取当前状态使用绑定({1,r.G0,r.变化.后绑定});
        if(!绑定当前成功(before,r.G0,r.变化.前绑定)||!绑定当前成功(after,r.G0,r.变化.后绑定))
            return{映射绑定(!绑定当前成功(before,r.G0,r.变化.前绑定)?before.结果头.状态:after.结果头.状态),r.G0};
        if(before.绑定->信息.发生场景!=after.绑定->信息.发生场景||before.绑定->信息.被描述存在!=r.主体存在
            ||after.绑定->信息.被描述存在!=r.主体存在)return{动态数据状态::主体不一致,r.G0};
        const auto bs=state_.读取当前状态({2,r.G0,before.绑定->信息.状态});
        const auto as=state_.读取当前状态({2,r.G0,after.绑定->信息.状态});
        if(!状态当前成功(bs,r.G0,before.绑定->信息.状态)||!状态当前成功(as,r.G0,after.绑定->信息.状态))
            return{映射状态(!状态当前成功(bs,r.G0,before.绑定->信息.状态)?bs.结果头.状态:as.结果头.状态),r.G0};
        if(bs.内容->信息.正式特征类型!=as.内容->信息.正式特征类型)return{动态数据状态::正式特征类型不一致,r.G0};
        if(bs.内容->信息.强时间.语义!=状态强时间语义::实例绝对UTC纳秒
            ||as.内容->信息.强时间.语义!=状态强时间语义::实例绝对UTC纳秒
            ||bs.内容->信息.强时间.纳秒>=as.内容->信息.强时间.纳秒)return{动态数据状态::时间不递增,r.G0};
        const auto left=读取I64(r.G0,*bs.内容);if(!left.值)return{left.状态,r.G0};
        const auto right=读取I64(r.G0,*as.内容);if(!right.值)return{right.状态,r.G0};
        if(*left.值==*right.值)return{动态数据状态::无变化,r.G0};
        return{核验比较(r,*bs.内容,*left.值,*right.值),r.G0};
    }

    I64读取结果 读取I64(std::uint64_t g,const 状态内容事实&s)const{
        const auto check=feature_.核验正式特征类型准确值({1,g,s.信息.正式特征类型,s.信息.固定准确值});
        if(!check.成功())return{映射特征核验(check.状态),std::nullopt};
        if(const auto*n=std::get_if<std::int64_t>(&s.信息.固定准确值))return{动态数据状态::已读取,*n};
        const auto expected=std::get<特征值身份>(s.信息.固定准确值);
        const auto read=value_.获取特征值(expected);const auto*v=std::get_if<特征值信息>(&read);
        if(!v)return{映射特征值(std::get<特征值读取错误>(read)),std::nullopt};
        if(v->值身份!=expected)return{动态数据状态::内部不一致,std::nullopt};
        const auto*n=std::get_if<std::int64_t>(&v->值内容);return n?I64读取结果{动态数据状态::已读取,*n}:I64读取结果{动态数据状态::比较依据不支持,std::nullopt};
    }

    动态数据状态 核验比较(const 动态原子创建请求&r,const 状态内容事实&s,std::int64_t left,std::int64_t right)const{
        const auto binding=feature_.读取当前I64比较绑定_v2({2,r.G0,s.信息.正式特征类型,特征I64比较用途::变化分析});
        if(!binding.成功()||!binding.事实)return 映射比较绑定(binding.状态);
        const auto&f=*binding.事实;
        有序I64比较合同快照 k{特征比较合同来源::当前独立绑定,f.身份,f.定义.输入FT,f.定义.用途,
            f.定义.算法族,f.定义.算法版本,f.定义.左角色,f.定义.右角色,f.定义.上下文要求位,f.定义.输入量化,
            f.定义.误差合同版本,f.定义.误差预算,f.定义.相等容差,f.定义.关系编码,f.输出组,binding.Gread};
        if(!(r.变化依据.比较请求.K==k)||r.变化依据.比较请求.输入[0].值!=left||r.变化依据.比较请求.输入[1].值!=right)
            return 动态数据状态::比较依据不支持;
        const auto&result=r.变化依据.比较结果;
        if(!result.成功()||result.请求身份!=r.变化依据.比较请求.请求身份)return 动态数据状态::比较依据不支持;
        for(const auto&o:result.输出组)if(o.角色==特征类标量结果角色::排序)
            return o.值!=0?动态数据状态::已读取:动态数据状态::无变化;
        return 动态数据状态::比较依据不支持;
    }

    动态数据状态 核验组合来源(动态信息身份 id,稳定编码 subject,std::uint64_t g,const 动态读取预算&budget,
        std::uint64_t depth,std::set<std::uint64_t>&visited,std::set<std::uint64_t>&active,std::uint64_t&dynamics,
        std::uint64_t&edges,std::uint64_t&materials,std::uint64_t&maxDepth)const{
        if(depth>budget.最大深度)return 动态数据状态::数量预算不足;
        if(active.contains(id.编码.值))return 动态数据状态::来源成环;
        maxDepth=std::max(maxDepth,depth);if(visited.contains(id.编码.值))return 动态数据状态::已读取;
        if(++dynamics>budget.最大动态数)return 动态数据状态::数量预算不足;
        active.insert(id.编码.值);const auto read=读取当前动态({1,g,id,budget});
        if(read.操作.状态!=动态数据状态::已读取||!read.内容)return 映射来源动态(read.操作.状态);
        if(read.内容->信息.主体存在!=subject)return 动态数据状态::主体不一致;
        edges+=read.内容->自有事实.变化关系组.size()+2;materials+=4+read.内容->自有事实.变化关系组.size();
        if(edges>budget.最大来源边数||materials>budget.最大读取材料数)return 动态数据状态::数量预算不足;
        if(const auto*c=std::get_if<动态组合内容>(&read.内容->信息.变化))for(const auto&m:c->成员组){
            const auto s=核验组合来源(m.子动态,subject,g,budget,depth+1,visited,active,dynamics,edges,materials,maxDepth);
            if(s!=动态数据状态::已读取)return s;}
        active.erase(id.编码.值);visited.insert(id.编码.值);return 动态数据状态::已读取;
    }

    动态单项结果 读取当前内容(std::uint64_t g,动态信息身份 id,const 动态读取预算&budget)const{
        if(是交付元节点(id.编码))return 单项失败(动态数据状态::未找到,g);
        const auto raw=l1_.读取所有者范围当前事实({L1所有者范围当前事实读取合同版本_v2,owner_,id.编码,g});
        if(raw.状态!=L1所有者范围当前事实读取状态_v2::成功||!raw.载荷)
            return 单项失败(映射当前事实(raw.状态),raw.读取事实代次);
        const auto*n=std::get_if<L1所有者范围节点事实>(&*raw.载荷);
        if(!n||n->写入所有者!=owner_||n->种类!=节点种类::普通||n->属性类型表示)
            return 单项失败(动态数据状态::内部不一致,g);
        const auto family=读取源关系(g,id.编码,layout_.族归属关系类型);
        const auto subject=读取源关系(g,id.编码,layout_.主体关系类型);
        const auto pre=读取源关系(g,id.编码,layout_.前绑定关系类型);
        const auto post=读取源关系(g,id.编码,layout_.后绑定关系类型);
        const auto members=读取源关系(g,id.编码,layout_.同主体成员关系类型);
        if(!family||!subject||!pre||!post||!members)return 单项失败(动态数据状态::内部不一致,g);
        if(family->size()!=1||(*family)[0].目标节点!=layout_.动态族锚点||(*family)[0].角色或顺序!=1||subject->size()!=1)
            return 单项失败(动态数据状态::未找到,g);
        const auto values=l1_.读取所有者范围所属节点当前完整值组({L1所有者范围所属节点当前完整值组读取合同版本_v2,owner_,id.编码,g});
        if(values.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功||values.读取事实代次!=g||values.载荷.size()!=1)
            return 单项失败(映射值组(values.状态),values.读取事实代次);
        const auto&v=values.载荷.front();const auto*utc=std::get_if<std::int64_t>(&v.材料);
        if(v.属性类型节点!=layout_.首次形成UTC属性类型||v.来源节点!=id.编码||!utc||*utc<0)
            return 单项失败(动态数据状态::内部不一致,g);
        动态变化内容 change;
        std::vector<稳定编码> changeIds;
        if(pre->size()==1&&post->size()==1&&members->empty()&&(*pre)[0].目标节点!=(*post)[0].目标节点){
            change=动态原子内容{{(*pre)[0].目标节点},{(*post)[0].目标节点}};
            changeIds={(*pre)[0].编码,(*post)[0].编码};
        }else if(pre->empty()&&post->empty()&&!members->empty()){
            auto sorted=*members;std::sort(sorted.begin(),sorted.end(),[](const auto&a,const auto&b){return a.角色或顺序<b.角色或顺序;});
            动态组合内容 c;std::set<std::uint64_t>seen;
            for(std::size_t i=0;i<sorted.size();++i){if(sorted[i].角色或顺序!=static_cast<std::int64_t>(i+1)
                    ||!seen.insert(sorted[i].目标节点.值).second)return 单项失败(动态数据状态::内部不一致,g);
                c.成员组.push_back({static_cast<std::uint32_t>(i+1),动态成员角色::同主体,{sorted[i].目标节点}});
                changeIds.push_back(sorted[i].编码);}change=std::move(c);
        }else return 单项失败(动态数据状态::内部不一致,g);
        const std::uint64_t materialCount=4+changeIds.size();
        if(budget.最大动态数<1||budget.最大来源边数<changeIds.size()+2||budget.最大读取材料数<materialCount)
            return 单项失败(动态数据状态::数量预算不足,g);
        return {读取头(g),动态内容事实{g,{id,(*subject)[0].目标节点,std::move(change)},
            {(*family)[0].编码,(*subject)[0].编码,v.编码,std::move(changeIds)},*utc,n->创建事实代次}};
    }

    std::optional<std::vector<L1所有者范围关系事实>> 读取源关系(std::uint64_t g,稳定编码 source,稳定编码 type)const{
        const auto q=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,source,type});
        if(q.状态!=L1所有者范围读取状态::成功||q.合同版本!=L1所有者范围CRUD合同版本
            ||q.源节点!=source||q.关系类型节点!=type||q.读取事实代次!=g)return std::nullopt;
        for(const auto&e:q.关系组)if(e.写入所有者!=owner_||e.源节点!=source||e.关系类型节点!=type)return std::nullopt;
        return q.关系组;
    }

    动态组结果 查询关系(std::uint64_t g,稳定编码 endpoint,稳定编码 type,查询方向 direction,const 动态读取预算&budget)const{
        std::vector<L1所有者范围关系事实> relations;
        if(direction==查询方向::目标){const auto q=l1_.读取所有者范围当前目标关系组({L1所有者范围CRUD合同版本,endpoint,type});
            if(q.状态!=L1所有者范围读取状态::成功||q.读取事实代次!=g)return 组失败(映射读取(q.状态),q.读取事实代次);relations=q.关系组;}
        else {const auto q=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,endpoint,type});
            if(q.状态!=L1所有者范围读取状态::成功||q.读取事实代次!=g)return 组失败(映射读取(q.状态),q.读取事实代次);relations=q.关系组;}
        if(relations.size()>budget.最大候选数)return 组失败(动态数据状态::数量预算不足,g);
        std::vector<动态内容事实> out;std::set<std::uint64_t>ids;
        for(const auto&e:relations){const auto id=direction==查询方向::目标?e.源节点:e.目标节点;
            if(!ids.insert(id.值).second)continue;const auto d=读取当前动态({1,g,{id},budget});
            if(d.操作.状态!=动态数据状态::已读取||!d.内容)return 组失败(d.操作.状态,d.操作.Gread);out.push_back(*d.内容);}
        std::sort(out.begin(),out.end(),内容身份小于);
        return out.size()<=budget.最大动态数?动态组结果{读取头(g),std::move(out)}:组失败(动态数据状态::数量预算不足,g);
    }

    动态数据状态 展开一个(动态信息身份 id,动态展开方式 mode,const 动态读取预算&budget,std::uint64_t depth,
        std::set<std::uint64_t>&visited,std::set<std::uint64_t>&active,std::uint64_t&edges,std::uint64_t&materials,动态来源展开事实&facts)const{
        if(depth>budget.最大深度||active.contains(id.编码.值))return active.contains(id.编码.值)?动态数据状态::来源成环:动态数据状态::数量预算不足;
        if(visited.contains(id.编码.值))return 动态数据状态::已读取;
        const auto read=读取当前动态({1,facts.Gread,id,budget});
        if(read.操作.状态!=动态数据状态::已读取||!read.内容)return 映射来源动态(read.操作.状态);
        active.insert(id.编码.值);facts.动态组.push_back(*read.内容);
        if(facts.动态组.size()>budget.最大动态数)return 动态数据状态::数量预算不足;
        edges+=read.内容->自有事实.变化关系组.size()+2;materials+=4+read.内容->自有事实.变化关系组.size();
        if(edges>budget.最大来源边数||materials>budget.最大读取材料数)return 动态数据状态::数量预算不足;
        if(const auto*a=std::get_if<动态原子内容>(&read.内容->信息.变化)){
            const auto b=binding_.读取当前状态使用绑定({1,facts.Gread,a->前绑定});
            const auto c=binding_.读取当前状态使用绑定({1,facts.Gread,a->后绑定});
            if(!绑定当前成功(b,facts.Gread,a->前绑定)||!绑定当前成功(c,facts.Gread,a->后绑定))return 动态数据状态::来源未找到;
            const auto bs=state_.读取当前状态({2,facts.Gread,b.绑定->信息.状态});
            const auto cs=state_.读取当前状态({2,facts.Gread,c.绑定->信息.状态});
            if(!状态当前成功(bs,facts.Gread,b.绑定->信息.状态)||!状态当前成功(cs,facts.Gread,c.绑定->信息.状态))return 动态数据状态::来源未找到;
            facts.原子来源组.push_back({id,*b.绑定,*c.绑定,*bs.内容,*cs.内容});
        }else if(mode==动态展开方式::递归){const auto&members=std::get<动态组合内容>(read.内容->信息.变化).成员组;
            for(const auto&m:members){const auto s=展开一个(m.子动态,mode,budget,depth+1,visited,active,edges,materials,facts);
                if(s!=动态数据状态::已读取)return s;}}
        active.erase(id.编码.值);visited.insert(id.编码.值);return 动态数据状态::已读取;
    }

    动态数据状态 读取当前身份(std::uint64_t g,动态信息身份 id)const{
        const auto raw=l1_.读取所有者范围当前事实({L1所有者范围当前事实读取合同版本_v2,owner_,id.编码,g});
        if(raw.状态!=L1所有者范围当前事实读取状态_v2::成功)return 映射当前事实(raw.状态);
        const auto*n=raw.载荷?std::get_if<L1所有者范围节点事实>(&*raw.载荷):nullptr;
        if(!n||n->写入所有者!=owner_||n->种类!=节点种类::普通||n->属性类型表示)return 动态数据状态::内部不一致;
        const auto q=读取源关系(g,id.编码,layout_.族归属关系类型);
        return q&&q->size()==1&&q->front().目标节点==layout_.动态族锚点&&q->front().角色或顺序==1
            ?动态数据状态::已读取:动态数据状态::未找到;
    }

    bool 是交付元节点(稳定编码 id)const noexcept{return id==layout_.格式锚点||id==layout_.字段登记关系类型||id==layout_.动态族锚点
        ||id==layout_.族归属关系类型||id==layout_.主体关系类型||id==layout_.前绑定关系类型||id==layout_.后绑定关系类型
        ||id==layout_.同主体成员关系类型||id==layout_.首次形成UTC属性类型;}
    static void 添加退出事实(std::vector<稳定编码>&out,const 动态内容事实&d){out={d.信息.身份.编码,d.自有事实.族归属关系,
        d.自有事实.主体关系,d.自有事实.首次形成UTC值};out.insert(out.end(),d.自有事实.变化关系组.begin(),d.自有事实.变化关系组.end());}
    L1所有者范围写集请求 形成退出写集(const 动态内容事实&d,std::uint64_t g,L1所有者范围写入幂等身份 key)const{
        L1所有者范围写集请求 w{L1所有者范围CRUD合同版本,g,key};添加退出事实(w.退出事实,d);std::sort(w.退出事实.begin(),w.退出事实.end());return w;}
    static bool 退出写集形状有效(const L1所有者范围写集请求&w,L1所有者范围写入幂等身份 key)noexcept{
        return w.合同版本==L1所有者范围CRUD合同版本&&w.期望事实代次&&w.写入幂等身份==key
            &&w.节点.empty()&&w.关系.empty()&&w.值.empty()&&w.属性槽变更.empty()&&!w.退出事实.empty()
            &&std::adjacent_find(w.退出事实.begin(),w.退出事实.end(),[](auto a,auto b){return !(a<b);})==w.退出事实.end();}
    std::optional<动态单项结果> 重放退出(const 动态退出请求&r){
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.状态==L1所有者范围读取状态::未找到)return std::nullopt;
        if(first.状态!=L1所有者范围读取状态::成功||!first.首次规范化写集||!first.首次写入结果)
            return 单项失败(映射读取(first.状态),first.读取事实代次);
        const auto&ws=*first.首次规范化写集;
        if(!退出写集形状有效(ws,r.幂等身份))return 单项失败(动态数据状态::幂等冲突,first.读取事实代次);
        const auto saved=port_.提交所有者范围中性写集(ws);
        if(saved.状态!=L1所有者范围写入状态::精确重复||!写入头完整(saved,r.幂等身份,ws.期望事实代次))
            return 提交失败(映射写入(saved.状态,动态数据状态::已删除),saved.事实代次);
        return 动态单项结果{{动态数据状态::精确重复,1,saved.事实代次,saved.事实代次,动态发布阶段::已读回},std::nullopt};
    }

    static 动态数据状态 映射当前事实(L1所有者范围当前事实读取状态_v2 s)noexcept{switch(s){
        case L1所有者范围当前事实读取状态_v2::成功:return 动态数据状态::已读取;
        case L1所有者范围当前事实读取状态_v2::未找到:return 动态数据状态::未找到;
        case L1所有者范围当前事实读取状态_v2::事实代次漂移:return 动态数据状态::事实代次漂移;
        case L1所有者范围当前事实读取状态_v2::资源失败:return 动态数据状态::资源失败;
        case L1所有者范围当前事实读取状态_v2::入口拒绝:return 动态数据状态::入口拒绝;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射值组(L1所有者范围所属节点当前完整值组读取状态_v2 s)noexcept{switch(s){
        case L1所有者范围所属节点当前完整值组读取状态_v2::成功:return 动态数据状态::已读取;
        case L1所有者范围所属节点当前完整值组读取状态_v2::未找到:return 动态数据状态::未找到;
        case L1所有者范围所属节点当前完整值组读取状态_v2::事实代次漂移:return 动态数据状态::事实代次漂移;
        case L1所有者范围所属节点当前完整值组读取状态_v2::资源失败:return 动态数据状态::资源失败;
        case L1所有者范围所属节点当前完整值组读取状态_v2::入口拒绝:return 动态数据状态::入口拒绝;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射来源动态(动态数据状态 s)noexcept{return s==动态数据状态::未找到?动态数据状态::来源未找到:s;}
    static 动态数据状态 映射存在(存在结构身份只读状态 s)noexcept{switch(s){
        case 存在结构身份只读状态::未实现:return 动态数据状态::依赖未实现;
        case 存在结构身份只读状态::未找到:return 动态数据状态::来源未找到;
        case 存在结构身份只读状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case 存在结构身份只读状态::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射绑定(状态使用绑定数据状态 s)noexcept{switch(s){
        case 状态使用绑定数据状态::未实现:return 动态数据状态::依赖未实现;
        case 状态使用绑定数据状态::未找到:case 状态使用绑定数据状态::场景未找到:case 状态使用绑定数据状态::存在未找到:
        case 状态使用绑定数据状态::状态未找到:return 动态数据状态::来源未找到;
        case 状态使用绑定数据状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case 状态使用绑定数据状态::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射状态(状态类数据状态 s)noexcept{switch(s){
        case 状态类数据状态::未找到:return 动态数据状态::来源未找到;
        case 状态类数据状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case 状态类数据状态::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射特征核验(特征类型准确值核验状态 s)noexcept{switch(s){
        case 特征类型准确值核验状态::正式特征类型未找到:case 特征类型准确值核验状态::准确值未找到:return 动态数据状态::来源未找到;
        case 特征类型准确值核验状态::准确值不相容:return 动态数据状态::比较依据不支持;
        case 特征类型准确值核验状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case 特征类型准确值核验状态::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射特征值(特征值读取错误 s)noexcept{switch(s){
        case 特征值读取错误::未找到:return 动态数据状态::来源未找到;
        case 特征值读取错误::能力未提供:return 动态数据状态::比较依据不支持;
        case 特征值读取错误::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射比较绑定(特征I64比较绑定读取状态_v2 s)noexcept{switch(s){
        case 特征I64比较绑定读取状态_v2::未找到:
        case 特征I64比较绑定读取状态_v2::格式不支持:case 特征I64比较绑定读取状态_v2::注册不唯一:return 动态数据状态::比较依据不支持;
        case 特征I64比较绑定读取状态_v2::事实代次漂移:return 动态数据状态::事实代次漂移;
        case 特征I64比较绑定读取状态_v2::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射读取(L1所有者范围读取状态 s)noexcept{switch(s){
        case L1所有者范围读取状态::未找到:return 动态数据状态::未找到;
        case L1所有者范围读取状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case L1所有者范围读取状态::资源失败:return 动态数据状态::资源失败;
        case L1所有者范围读取状态::入口拒绝:return 动态数据状态::入口拒绝;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射写入(L1所有者范围写入状态 s,动态数据状态 ok)noexcept{switch(s){
        case L1所有者范围写入状态::成功:return ok;case L1所有者范围写入状态::精确重复:return 动态数据状态::精确重复;
        case L1所有者范围写入状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case L1所有者范围写入状态::幂等冲突:return 动态数据状态::幂等冲突;
        case L1所有者范围写入状态::引用冲突:return 动态数据状态::引用冲突;
        case L1所有者范围写入状态::资源失败:return 动态数据状态::资源失败;
        case L1所有者范围写入状态::内部不一致:return 动态数据状态::已可能发布;
        default:return 动态数据状态::内部不一致;}}

    L1事实基座服务& l1_;L1所有者范围写端口 port_;L1结构所有者身份 owner_{};动态结构交付 layout_;
    const 存在结构身份只读提供者& existence_;const 状态使用绑定只读提供者& binding_;const 状态类数据服务& state_;
    const 特征类数据服务& feature_;const 特征值类数据服务& value_;
};

} // namespace 海中鱼巣
