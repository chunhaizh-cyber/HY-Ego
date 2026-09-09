module;

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

export module 海中鱼巣.领域.数据服务.动态类;

export import 海中鱼巣.领域.合同.动态结构;
import 海中鱼巣.领域.数据服务.特征值类;

export namespace 海中鱼巣 {

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
        const 特征值类数据服务& value,
        动态保留策略 policy = {})
        : l1_(l1), port_(std::move(port)), owner_(port_.所有者身份()),
          layout_(layout), existence_(existence), binding_(binding), state_(state),
          feature_(feature), value_(value), policy_(policy) {
        if (!绑定于(l1_) || !有效(owner_) || !布局浅层有效()
            || !policy_.实例动态TTL纳秒 || !policy_.实例动态数量上限)
            throw std::invalid_argument("invalid dynamic configuration");
        const auto o=l1_.读取当前结构所有者({L1所有者范围CRUD合同版本,owner_});
        if(o.状态!=L1所有者范围读取状态::成功||!o.所有者事实
            ||o.所有者事实->所有者!=owner_
            ||o.所有者事实->范围种类!=L1所有者范围种类::独占结构范围
            ||o.所有者事实->退出事实代次||!布局材料有效())
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
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 单项失败(guard.状态,guard.Gread);
            if(guard.Gread!=r.G0)return 单项失败(动态数据状态::事实代次漂移,guard.Gread);
            const auto source=核验原子来源(r);
            if(source.状态!=动态数据状态::已读取)return 单项失败(source.状态,source.Gread);
            const auto utc=当前UTC纳秒();
            if(utc<0)return 单项失败(动态数据状态::资源失败,r.G0);
            auto remaining=r.预算;
            if(!扣除并保留(remaining.最大来源边数,source.已用边数,4)
                ||!扣除并保留(remaining.最大读取材料数,source.已用材料数,10)
                ||remaining.最大动态数<1||remaining.最大深度<1)
                return 单项失败(动态数据状态::数量预算不足,r.G0);
            return 提交创建(形成原子写集(r,utc),r.幂等身份,remaining);
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态单项结果 创建组合动态(const 动态组合创建请求& r) {
        if(!组合请求有效(r))return 单项失败(动态数据状态::入口拒绝);
        try {
            if(auto replay=重放创建(r))return *replay;
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 单项失败(guard.状态,guard.Gread);
            if(guard.Gread!=r.G0)return 单项失败(动态数据状态::事实代次漂移,guard.Gread);
            for(const auto&m:r.变化.成员组)
                if(m.角色==动态成员角色::组成存在)
                    return 单项失败(动态数据状态::组成发生期依据不足,r.G0);
            std::set<std::uint64_t> visited,active;
            std::uint64_t dynamics=0,edges=0,materials=0,maxDepth=0;
            for(const auto&m:r.变化.成员组) {
                const auto s=核验组合来源(m.子动态,r.主体存在,r.G0,r.来源截止,
                    r.预算,1,visited,active,dynamics,edges,materials,maxDepth);
                if(s!=动态数据状态::已读取)return 单项失败(s,r.G0);
            }
            const auto utc=当前UTC纳秒();
            if(utc<0)return 单项失败(动态数据状态::资源失败,r.G0);
            const auto memberCount=static_cast<std::uint64_t>(r.变化.成员组.size());
            auto remaining=r.预算;
            if(dynamics==std::numeric_limits<std::uint64_t>::max()
                ||memberCount>std::numeric_limits<std::uint64_t>::max()-4
                ||edges>std::numeric_limits<std::uint64_t>::max()-memberCount
                ||materials>std::numeric_limits<std::uint64_t>::max()-4-memberCount
                ||!扣除并保留(remaining.最大动态数,dynamics,dynamics+1)
                ||!扣除并保留(remaining.最大来源边数,edges,edges+memberCount)
                ||!扣除并保留(remaining.最大读取材料数,materials,materials+4+memberCount)
                ||maxDepth==std::numeric_limits<std::uint64_t>::max()
                ||remaining.最大深度<maxDepth+1)
                return 单项失败(动态数据状态::数量预算不足,r.G0);
            return 提交创建(形成组合写集(r,utc),r.幂等身份,remaining);
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态单项结果 读取当前动态(const 动态当前读取请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.身份)||!预算有效(r.预算))
            return 单项失败(动态数据状态::入口拒绝);
        try {
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 单项失败(guard.状态,guard.Gread);
            if(guard.Gread!=r.G0)return 单项失败(动态数据状态::事实代次漂移,guard.Gread);
            auto out=读取历史({1,r.G0,r.G0,r.身份,r.预算},std::nullopt);
            if(out.成功()) {
                const auto after=读取当前代次();
                if(after.状态!=动态数据状态::已读取)return 单项失败(after.状态,after.Gread);
                if(after.Gread!=r.G0)return 单项失败(动态数据状态::事实代次漂移,after.Gread);
            }
            return out;
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态单项结果 读取动态历史(const 动态历史读取请求& r) const override {
        if(!历史请求有效(r))return 单项失败(动态数据状态::入口拒绝);
        try {
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 单项失败(guard.状态,guard.Gread);
            if(guard.Gread!=r.Gread)return 单项失败(动态数据状态::事实代次漂移,guard.Gread);
            auto out=读取历史(r,std::nullopt);if(!out.成功())return out;
            const auto after=读取当前代次();if(after.状态!=动态数据状态::已读取)return 单项失败(after.状态,after.Gread,r.H);
            return after.Gread==r.Gread?out:单项失败(动态数据状态::事实代次漂移,after.Gread,r.H);
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态组结果 按主体查询动态(const 动态按主体查询请求& r) const override {
        if(r.合同版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.主体存在)||!预算有效(r.预算))
            return 组失败(动态数据状态::入口拒绝);
        try {const auto source=existence_.读取存在身份来源历史见证(r.Gread,r.H,r.主体存在);
            if(!source.成功(r.Gread,r.H,r.主体存在))return 组失败(映射存在(source.状态),source.Gread,r.H);
            std::uint64_t candidates=0,dynamics=0,materials=0;
            auto out=查询关系(r.Gread,r.H,r.主体存在,layout_.主体关系类型,查询方向::目标,r.预算,candidates,dynamics,materials);
            if(!out.成功())return out;const auto after=读取当前代次();if(after.状态!=动态数据状态::已读取)return 组失败(after.状态,after.Gread,r.H);
            return after.Gread==r.Gread?out:组失败(动态数据状态::事实代次漂移,after.Gread,r.H);
        }catch(const std::bad_alloc&){return 组失败(动态数据状态::资源失败);}
        catch(const std::length_error&){return 组失败(动态数据状态::资源失败);}
        catch(...){return 组失败(动态数据状态::内部不一致);}
    }

    动态组结果 按绑定反查原子动态(const 动态按绑定查询请求& r) const override {
        if(r.合同版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.绑定)||!预算有效(r.预算))
            return 组失败(动态数据状态::入口拒绝);
        try {
            const auto source=binding_.读取状态使用绑定历史({1,r.Gread,r.H,r.绑定});
            if(!source.成功())return 组失败(映射绑定(source.结果头.状态),source.结果头.事实截止代次,r.H);
            if(source.绑定->信息.身份!=r.绑定||source.绑定->Gread!=r.Gread||source.绑定->H!=r.H)
                return 组失败(动态数据状态::内部不一致,r.Gread,r.H);
            std::uint64_t candidates=0,dynamics=0,materials=0;
            auto a=查询关系(r.Gread,r.H,r.绑定.编码,layout_.前绑定关系类型,查询方向::目标,r.预算,candidates,dynamics,materials);
            if(!a.成功())return a;
            auto b=查询关系(r.Gread,r.H,r.绑定.编码,layout_.后绑定关系类型,查询方向::目标,r.预算,candidates,dynamics,materials);
            if(!b.成功())return b;
            a.动态组.insert(a.动态组.end(),b.动态组.begin(),b.动态组.end());
            std::sort(a.动态组.begin(),a.动态组.end(),内容身份小于);
            a.动态组.erase(std::unique(a.动态组.begin(),a.动态组.end(),内容身份相同),a.动态组.end());
            if(a.动态组.size()>r.预算.最大动态数)return 组失败(动态数据状态::数量预算不足,r.Gread,r.H);
            const auto after=读取当前代次();if(after.状态!=动态数据状态::已读取)return 组失败(after.状态,after.Gread,r.H);
            return after.Gread==r.Gread?a:组失败(动态数据状态::事实代次漂移,after.Gread,r.H);
        } catch(const std::bad_alloc&) {return 组失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 组失败(动态数据状态::资源失败);}
        catch(...) {return 组失败(动态数据状态::内部不一致);}
    }

    动态组结果 按子动态反查父动态(const 动态按子动态查询请求& r) const override {
        if(r.合同版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.子动态)||!预算有效(r.预算))
            return 组失败(动态数据状态::入口拒绝);
        try {const auto source=读取身份({1,r.Gread,r.H,r.子动态});if(!source.成功())return 组失败(source.操作.状态,source.操作.Gread,r.H);
            std::uint64_t candidates=0,dynamics=0,materials=0;
            auto out=查询关系(r.Gread,r.H,r.子动态.编码,layout_.同主体成员关系类型,查询方向::目标,r.预算,candidates,dynamics,materials);
            if(!out.成功())return out;const auto after=读取当前代次();if(after.状态!=动态数据状态::已读取)return 组失败(after.状态,after.Gread,r.H);
            return after.Gread==r.Gread?out:组失败(动态数据状态::事实代次漂移,after.Gread,r.H);
        }catch(const std::bad_alloc&){return 组失败(动态数据状态::资源失败);}
        catch(const std::length_error&){return 组失败(动态数据状态::资源失败);}
        catch(...){return 组失败(动态数据状态::内部不一致);}
    }

    动态展开结果 展开动态来源(const 动态来源展开请求& r) const override {
        if(r.合同版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.身份)
            ||(r.方式!=动态展开方式::直接&&r.方式!=动态展开方式::递归)||!预算有效(r.预算))
            return 展开失败(动态数据状态::入口拒绝);
        try {
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 展开失败(guard.状态,guard.Gread,r.H);
            if(guard.Gread!=r.Gread)return 展开失败(动态数据状态::事实代次漂移,guard.Gread,r.H);
            动态来源展开事实 facts{r.Gread,r.H,r.身份,{},{}};
            std::set<std::uint64_t> visited,active;
            std::uint64_t edges=0,materials=0;
            const auto s=展开一个(r.身份,r.方式,r.预算,1,visited,active,edges,materials,facts);
            if(s!=动态数据状态::已读取)return 展开失败(s,r.Gread,r.H);
            std::sort(facts.动态组.begin(),facts.动态组.end(),内容身份小于);
            std::sort(facts.原子来源组.begin(),facts.原子来源组.end(),[](const auto&a,const auto&b){return a.动态.编码<b.动态.编码;});
            动态展开结果 out{读取头(r.Gread,r.H),std::move(facts)};
            if(!out.成功())return 展开失败(动态数据状态::内部不一致,r.Gread,r.H);
            const auto after=读取当前代次();if(after.状态!=动态数据状态::已读取)return 展开失败(after.状态,after.Gread,r.H);
            return after.Gread==r.Gread?out:展开失败(动态数据状态::事实代次漂移,after.Gread,r.H);
        } catch(const std::bad_alloc&) {return 展开失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 展开失败(动态数据状态::资源失败);}
        catch(...) {return 展开失败(动态数据状态::内部不一致);}
    }

    动态操作结果 确认当前动态结构身份(const 动态当前身份请求& r) const override {
        if(r.合同版本!=1||!r.G0||!有效(r.身份))return 失败头(动态数据状态::入口拒绝);
        try {
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 失败头(guard.状态,guard.Gread);
            if(guard.Gread!=r.G0)return 失败头(动态数据状态::事实代次漂移,guard.Gread);
            const auto w=读取身份({1,r.G0,r.G0,r.身份});
            if(!w.成功())return w.操作;
            const auto after=读取当前代次();
            if(after.状态!=动态数据状态::已读取)return 失败头(after.状态,after.Gread);
            return after.Gread==r.G0?读取头(r.G0,r.G0):失败头(动态数据状态::事实代次漂移,after.Gread);
        } catch(const std::bad_alloc&) {return 失败头(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 失败头(动态数据状态::资源失败);}
        catch(...) {return 失败头(动态数据状态::内部不一致);}
    }

    动态身份历史结果 读取动态身份历史(const 动态历史身份请求& r) const override {
        if(r.合同版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.身份))
            return 身份失败(动态数据状态::入口拒绝);
        try {
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 身份失败(guard.状态,guard.Gread,r.H);
            if(guard.Gread!=r.Gread)return 身份失败(动态数据状态::事实代次漂移,guard.Gread,r.H);
            auto out=读取身份(r);if(!out.成功())return out;
            const auto after=读取当前代次();if(after.状态!=动态数据状态::已读取)return 身份失败(after.状态,after.Gread,r.H);
            return after.Gread==r.Gread?out:身份失败(动态数据状态::事实代次漂移,after.Gread,r.H);
        } catch(const std::bad_alloc&) {return 身份失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 身份失败(动态数据状态::资源失败);}
        catch(...) {return 身份失败(动态数据状态::内部不一致);}
    }

    动态单项结果 退出动态(const 动态退出请求& r) {
        if(r.合同版本!=1||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()
            ||!有效(r.幂等身份)||!有效(r.身份)||!预算有效(r.预算))
            return 单项失败(动态数据状态::入口拒绝);
        try {
            if(auto replay=重放退出(r))return *replay;
            const auto current=读取当前动态({1,r.G0,r.身份,r.预算});
            if(!current.成功())return 单项失败(current.操作.状态,current.操作.Gread,current.操作.H);
            auto ws=形成退出写集(*current.内容,r.G0,r.幂等身份);
            const auto saved=port_.提交所有者范围中性写集(ws);
            const auto status=映射写入(saved.状态,动态数据状态::已退出);
            if(status!=动态数据状态::已退出&&status!=动态数据状态::精确重复)
                return 提交失败(status,saved.事实代次);
            if(!写入头完整(saved,r.幂等身份,ws.期望事实代次)||!saved.新编码映射.empty())
                return 提交后失败(saved.事实代次);
            const auto guard=读取当前代次();
            if(guard.状态!=动态数据状态::已读取)return 提交后失败(saved.事实代次,guard.Gread);
            auto read=读取历史({1,guard.Gread,saved.事实代次-1,r.身份,r.预算},saved.事实代次);
            if(!read.成功())return 提交后失败(saved.事实代次,read.操作.Gread);
            read.内容->Gread=guard.Gread;
            return {{status,1,guard.Gread,saved.事实代次-1,saved.事实代次,动态发布阶段::已读回},std::move(read.内容)};
        } catch(const std::bad_alloc&) {return 单项失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 单项失败(动态数据状态::资源失败);}
        catch(...) {return 单项失败(动态数据状态::内部不一致);}
    }

    动态清理结果 同步清理实例动态(const 动态清理请求& r) {
        if(r.合同版本!=1||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()
            ||!有效(r.退出幂等身份)||!r.清理幂等身份.值||r.当前UTC纳秒<0||!预算有效(r.预算))
            return 清理失败(动态数据状态::入口拒绝);
        try {
            if(auto replay=重放清理(r))return *replay;
            const auto q=l1_.按属性类型读取所有者范围全部当前值({L1所有者范围属性类型当前值组读取合同版本,
                owner_,layout_.首次形成UTC属性类型,r.G0,r.预算.最大候选数});
            if(q.合同版本!=L1所有者范围属性类型当前值组读取合同版本||q.所有者!=owner_
                ||q.属性类型节点!=layout_.首次形成UTC属性类型||q.期望事实代次!=r.G0)
                return 清理失败(动态数据状态::内部不一致,q.读取事实代次);
            const auto qs=映射值组(q.状态);
            if(qs!=动态数据状态::已读取)return 清理失败(qs,q.读取事实代次);
            if(q.当前值.size()>r.预算.最大动态数)return 清理失败(动态数据状态::数量预算不足,r.G0);
            std::vector<动态内容事实> all;std::uint64_t usedMaterials=0;
            for(const auto&v:q.当前值){const auto*n=std::get_if<std::int64_t>(&v.材料);
                if(!n||*n<0)return 清理失败(动态数据状态::内部不一致,r.G0);
                auto d=读取当前动态({1,r.G0,{v.所属节点},r.预算});
                if(!d.成功())return 清理失败(d.操作.状态,d.操作.Gread,d.操作.H);
                if(d.内容->首次形成UTC纳秒!=*n)return 清理失败(动态数据状态::内部不一致,r.G0);
                const auto cost=4+d.内容->自有事实.变化关系组.size();
                if(cost>r.预算.最大读取材料数||usedMaterials>r.预算.最大读取材料数-cost)
                    return 清理失败(动态数据状态::数量预算不足,r.G0);
                usedMaterials+=cost;
                all.push_back(*d.内容);}
            std::sort(all.begin(),all.end(),[](const auto&a,const auto&b){return a.首次形成UTC纳秒<b.首次形成UTC纳秒
                ||(a.首次形成UTC纳秒==b.首次形成UTC纳秒&&a.信息.身份.编码<b.信息.身份.编码);});
            const auto excess=all.size()>policy_.实例动态数量上限
                ? all.size()-static_cast<std::size_t>(policy_.实例动态数量上限) : 0;
            std::vector<动态内容事实> victims;
            for(std::size_t i=0;i<all.size();++i){if(r.当前UTC纳秒<all[i].首次形成UTC纳秒)
                    return 清理失败(动态数据状态::内部不一致,r.G0);
                const auto age=static_cast<std::uint64_t>(r.当前UTC纳秒-all[i].首次形成UTC纳秒);
                if(i<excess||age>=policy_.实例动态TTL纳秒)victims.push_back(all[i]);}
            if(victims.empty())return {读取头(r.G0,r.G0),{}};
            L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.退出幂等身份};
            for(const auto&v:victims)添加退出事实(ws.退出事实,v);
            std::sort(ws.退出事实.begin(),ws.退出事实.end());
            ws.退出事实.erase(std::unique(ws.退出事实.begin(),ws.退出事实.end()),ws.退出事实.end());
            const auto exited=port_.提交所有者范围中性写集(ws);
            const auto es=映射写入(exited.状态,动态数据状态::已退出);
            if(es!=动态数据状态::已退出&&es!=动态数据状态::精确重复)
                return 清理失败(es,exited.事实代次);
            if(!写入头完整(exited,r.退出幂等身份,ws.期望事实代次)||!exited.新编码映射.empty())
                return 清理提交后失败(exited.事实代次);
            L1物理清理请求 clean{L1物理清理合同版本,exited.事实代次,r.清理幂等身份,{}};
            for(auto id:ws.退出事实)clean.待清理事实身份组.push_back({L1物理清理事实种类::值,id});
            return 执行清理(clean,exited.事实代次);
        } catch(const std::bad_alloc&) {return 清理失败(动态数据状态::资源失败);}
        catch(const std::length_error&) {return 清理失败(动态数据状态::资源失败);}
        catch(...) {return 清理失败(动态数据状态::内部不一致);}
    }

private:
    inline static constexpr L1所有者范围写集本地键 节点键{1},族关系键{2},主体关系键{3},首次UTC键{4};
    enum class 查询方向{源,目标};
    struct 代次结果{动态数据状态 状态=动态数据状态::内部不一致;std::uint64_t Gread=0;};
    struct 来源核验结果{动态数据状态 状态=动态数据状态::内部不一致;std::uint64_t Gread=0,已用边数=0,已用材料数=0;};
    struct I64读取结果{动态数据状态 状态=动态数据状态::内部不一致;std::optional<std::int64_t> 值;};

    static bool 预算有效(const 动态读取预算&v)noexcept{return v.最大候选数&&v.最大动态数
        &&v.最大来源边数&&v.最大深度&&v.最大读取材料数;}
    static bool 扣除并保留(std::uint64_t& available,std::uint64_t used,std::uint64_t reserve)noexcept{
        if(used>available||reserve>available-used)return false;available-=used;return true;}
    static bool 历史请求有效(const 动态历史读取请求&r)noexcept{return r.合同版本==1&&r.Gread&&r.H&&r.H<=r.Gread&&有效(r.身份)&&预算有效(r.预算);}
    static bool 原子请求有效(const 动态原子创建请求&r)noexcept{return r.合同版本==1&&r.G0
        &&r.G0!=std::numeric_limits<std::uint64_t>::max()&&r.来源截止&&r.来源截止<=r.G0&&有效(r.幂等身份)
        &&有效(r.主体存在)&&有效(r.变化.前绑定)&&有效(r.变化.后绑定)&&r.变化.前绑定!=r.变化.后绑定&&预算有效(r.预算);}
    static bool 组合请求有效(const 动态组合创建请求&r)noexcept{
        if(r.合同版本!=1||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()||!r.来源截止||r.来源截止>r.G0
            ||!有效(r.幂等身份)||!有效(r.主体存在)||r.变化.成员组.empty()||!预算有效(r.预算))return false;
        for(std::size_t i=0;i<r.变化.成员组.size();++i){const auto&m=r.变化.成员组[i];
            if(m.顺序!=i+1||!有效(m.子动态)||(m.角色!=动态成员角色::同主体&&m.角色!=动态成员角色::组成存在))return false;
            if(m.角色==动态成员角色::同主体&&m.组成依据)return false;
            if(m.角色==动态成员角色::组成存在){if(!m.组成依据||!m.组成依据->事实截止
                ||m.组成依据->发生起始UTC纳秒>=m.组成依据->发生终止UTC纳秒
                ||m.组成依据->由父到子的组成关系身份组.empty())return false;}
            for(std::size_t j=0;j<i;++j)if(r.变化.成员组[j].子动态==m.子动态)return false;}
        return true;}
    static std::int64_t 当前UTC纳秒()noexcept{return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();}
    代次结果 读取当前代次()const{const auto q=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
        if(q.状态==L1中性读取状态::资源失败)return{动态数据状态::资源失败,q.事实代次};
        if(q.状态!=L1中性读取状态::成功||q.合同版本!=L1中性CRUD合同版本||!q.事实代次)return{动态数据状态::内部不一致,q.事实代次};
        return{动态数据状态::已读取,q.事实代次};}

    bool 布局浅层有效()const noexcept{const 稳定编码 ids[]{layout_.格式锚点,layout_.字段登记关系类型,layout_.动态族锚点,
        layout_.族归属关系类型,layout_.主体关系类型,layout_.前绑定关系类型,layout_.后绑定关系类型,
        layout_.同主体成员关系类型,layout_.首次形成UTC属性类型};
        for(std::size_t i=0;i<9;++i){if(!有效(ids[i]))return false;for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;}return true;}
    bool 是交付元节点(稳定编码 id)const noexcept{return id==layout_.格式锚点||id==layout_.字段登记关系类型||id==layout_.动态族锚点
        ||id==layout_.族归属关系类型||id==layout_.主体关系类型||id==layout_.前绑定关系类型||id==layout_.后绑定关系类型
        ||id==layout_.同主体成员关系类型||id==layout_.首次形成UTC属性类型;}
    bool 节点类型有效(稳定编码 id,节点种类 kind,std::optional<L1所有者范围值表示种类> repr)const{
        const auto q=l1_.读取所有者范围当前节点({L1所有者范围CRUD合同版本,id});const auto*n=q.事实 ? std::get_if<L1所有者范围节点事实>(&*q.事实) : nullptr;
        return q.状态==L1所有者范围读取状态::成功&&n&&n->编码==id&&n->写入所有者==owner_&&n->种类==kind&&n->属性类型表示==repr&&!n->退出事实代次;}
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
                ||e.目标节点!=expected[i]||e.退出事实代次)return false;++count;}if(count!=1)return false;}
        return true;}

    static 动态操作结果 失败头(动态数据状态 s,std::uint64_t g=0,std::uint64_t h=0)noexcept{return{s,1,g,h,std::nullopt,动态发布阶段::无写入};}
    static 动态操作结果 读取头(std::uint64_t g,std::uint64_t h)noexcept{return{动态数据状态::已读取,1,g,h,std::nullopt,动态发布阶段::无写入};}
    static 动态单项结果 单项失败(动态数据状态 s,std::uint64_t g=0,std::uint64_t h=0)noexcept{return{失败头(s,g,h),std::nullopt};}
    static 动态组结果 组失败(动态数据状态 s,std::uint64_t g=0,std::uint64_t h=0)noexcept{return{失败头(s,g,h),{}};}
    static 动态展开结果 展开失败(动态数据状态 s,std::uint64_t g=0,std::uint64_t h=0)noexcept{return{失败头(s,g,h),std::nullopt};}
    static 动态身份历史结果 身份失败(动态数据状态 s,std::uint64_t g=0,std::uint64_t h=0)noexcept{return{失败头(s,g,h),std::nullopt};}
    static 动态清理结果 清理失败(动态数据状态 s,std::uint64_t g=0,std::uint64_t h=0)noexcept{return{失败头(s,g,h),{}};}
    static 动态单项结果 提交失败(动态数据状态 s,std::uint64_t g)noexcept{
        if(s==动态数据状态::已可能发布)return{{s,1,g,0,g ? std::optional<std::uint64_t>{g} : std::nullopt,动态发布阶段::未知},std::nullopt};
        return 单项失败(s,g);}
    static 动态单项结果 提交后失败(std::uint64_t published,std::uint64_t guard=0)noexcept{
        return{{动态数据状态::内部不一致,1,guard,0,published,动态发布阶段::已提交待读回},std::nullopt};}
    static 动态清理结果 清理提交后失败(std::uint64_t published,std::uint64_t guard=0)noexcept{
        return{{动态数据状态::内部不一致,1,guard,0,published,动态发布阶段::已提交待读回},{}};}

    static bool 内容身份小于(const 动态内容事实&a,const 动态内容事实&b){return a.信息.身份.编码<b.信息.身份.编码;}
    static bool 内容身份相同(const 动态内容事实&a,const 动态内容事实&b){return a.信息.身份==b.信息.身份;}
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
        if(first.状态!=L1所有者范围读取状态::成功)return 单项失败(映射读取(first.状态),first.读取事实代次);
        if(!first.首次规范化写集||!first.首次写入结果)return 单项失败(动态数据状态::内部不一致,first.读取事实代次);
        const auto&ws=*first.首次规范化写集;std::optional<std::int64_t>utc;
        for(const auto&v:ws.值)if(v.本地键==首次UTC键)utc=std::get_if<std::int64_t>(&v.材料) ? std::optional<std::int64_t>{*std::get_if<std::int64_t>(&v.材料)} : std::nullopt;
        if(!utc)return 单项失败(动态数据状态::幂等冲突,first.读取事实代次);
        const auto expected=[&](){if constexpr(std::is_same_v<R,动态原子创建请求>)return 形成原子写集(r,*utc);else return 形成组合写集(r,*utc);}();
        if(ws!=expected)return 单项失败(动态数据状态::幂等冲突,first.读取事实代次);
        const auto saved=port_.提交所有者范围中性写集(ws);
        if(saved.状态!=L1所有者范围写入状态::精确重复||!写入头完整(saved,r.幂等身份,ws.期望事实代次))
            return 提交失败(映射写入(saved.状态,动态数据状态::已创建),saved.事实代次);
        const auto id=映射编码(saved,节点键);if(!id)return 提交后失败(saved.事实代次);
        const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取)return 提交后失败(saved.事实代次,guard.Gread);
        auto read=读取历史({1,guard.Gread,saved.事实代次, {*id},r.预算},std::nullopt);
        if(!read.成功())return 提交后失败(saved.事实代次,read.操作.Gread);
        return 动态单项结果{{动态数据状态::精确重复,1,guard.Gread,saved.事实代次,saved.事实代次,动态发布阶段::已读回},std::move(read.内容)};
    }

    动态单项结果 提交创建(L1所有者范围写集请求 ws,L1所有者范围写入幂等身份 key,动态读取预算 budget){
        const auto saved=port_.提交所有者范围中性写集(ws);const auto s=映射写入(saved.状态,动态数据状态::已创建);
        if(s!=动态数据状态::已创建&&s!=动态数据状态::精确重复)return 提交失败(s,saved.事实代次);
        if(!写入头完整(saved,key,ws.期望事实代次))return 提交后失败(saved.事实代次);
        const auto id=映射编码(saved,节点键);if(!id)return 提交后失败(saved.事实代次);
        const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取)return 提交后失败(saved.事实代次,guard.Gread);
        if(!预算有效(budget))return 提交后失败(saved.事实代次,guard.Gread);
        const auto source=展开动态来源({1,guard.Gread,saved.事实代次,{*id},动态展开方式::递归,budget});
        if(!source.成功())return 提交后失败(saved.事实代次,source.操作.Gread);
        const auto found=std::lower_bound(source.来源->动态组.begin(),source.来源->动态组.end(),*id,
            [](const auto&l,const auto&r){return l.信息.身份.编码<r;});
        if(found==source.来源->动态组.end()||found->信息.身份.编码!=*id)return 提交后失败(saved.事实代次,source.操作.Gread);
        return{{s,1,guard.Gread,saved.事实代次,saved.事实代次,动态发布阶段::已读回},*found};
    }

    来源核验结果 核验原子来源(const 动态原子创建请求&r)const{
        const auto ew=existence_.读取存在身份来源历史见证(r.G0,r.来源截止,r.主体存在);
        if(!ew.成功(r.G0,r.来源截止,r.主体存在))return{映射存在(ew.状态),ew.Gread};
        const auto ec=existence_.确认当前存在结构身份(r.G0,r.主体存在);
        if(!ec.成功(r.G0))return{映射存在(ec.状态),ec.Gread};
        const auto before=binding_.读取状态使用绑定历史({1,r.G0,r.来源截止,r.变化.前绑定});
        if(!before.成功())return{映射绑定(before.结果头.状态),before.结果头.事实截止代次};
        const auto after=binding_.读取状态使用绑定历史({1,r.G0,r.来源截止,r.变化.后绑定});
        if(!after.成功())return{映射绑定(after.结果头.状态),after.结果头.事实截止代次};
        if(before.结果头.合同版本!=1||before.结果头.事实截止代次!=r.G0||before.结果头.变更事实代次
            ||before.绑定->Gread!=r.G0||before.绑定->H!=r.来源截止||before.绑定->信息.身份!=r.变化.前绑定
            ||after.结果头.合同版本!=1||after.结果头.事实截止代次!=r.G0||after.结果头.变更事实代次
            ||after.绑定->Gread!=r.G0||after.绑定->H!=r.来源截止||after.绑定->信息.身份!=r.变化.后绑定)
            return{动态数据状态::内部不一致,r.G0};
        if(before.绑定->信息.发生场景!=after.绑定->信息.发生场景||before.绑定->信息.被描述存在!=r.主体存在
            ||after.绑定->信息.被描述存在!=r.主体存在)return{动态数据状态::主体不一致,r.G0};
        const auto bc=binding_.读取当前状态使用绑定({1,r.G0,r.变化.前绑定});
        const auto ac=binding_.读取当前状态使用绑定({1,r.G0,r.变化.后绑定});
        if(!bc.成功()||!ac.成功())return{!bc.成功() ? 映射绑定(bc.结果头.状态) : 映射绑定(ac.结果头.状态),r.G0};
        if(bc.结果头.合同版本!=1||bc.结果头.事实截止代次!=r.G0||bc.结果头.变更事实代次
            ||bc.绑定->Gread!=r.G0||bc.绑定->H!=r.G0||bc.绑定->信息.身份!=r.变化.前绑定
            ||ac.结果头.合同版本!=1||ac.结果头.事实截止代次!=r.G0||ac.结果头.变更事实代次
            ||ac.绑定->Gread!=r.G0||ac.绑定->H!=r.G0||ac.绑定->信息.身份!=r.变化.后绑定
            ||bc.绑定->信息!=before.绑定->信息||ac.绑定->信息!=after.绑定->信息)
            return{动态数据状态::内部不一致,r.G0};
        const auto bs=state_.读取状态历史内容({2,r.G0,r.来源截止,before.绑定->信息.状态});
        const auto as=state_.读取状态历史内容({2,r.G0,r.来源截止,after.绑定->信息.状态});
        if(!bs.成功()||!as.成功())return{!bs.成功() ? 映射状态(bs.结果头.状态) : 映射状态(as.结果头.状态),r.G0};
        if(bs.结果头.合同版本!=2||bs.结果头.事实截止代次!=r.G0||bs.结果头.变更事实代次
            ||bs.内容->Gread!=r.G0||bs.内容->H!=r.来源截止||bs.内容->信息.身份!=before.绑定->信息.状态
            ||as.结果头.合同版本!=2||as.结果头.事实截止代次!=r.G0||as.结果头.变更事实代次
            ||as.内容->Gread!=r.G0||as.内容->H!=r.来源截止||as.内容->信息.身份!=after.绑定->信息.状态)
            return{动态数据状态::内部不一致,r.G0};
        if(bs.内容->信息.正式特征类型!=as.内容->信息.正式特征类型)return{动态数据状态::正式特征类型不一致,r.G0};
        if(bs.内容->信息.强时间.语义!=状态强时间语义::实例绝对UTC纳秒
            ||as.内容->信息.强时间.语义!=状态强时间语义::实例绝对UTC纳秒
            ||bs.内容->信息.强时间.纳秒>=as.内容->信息.强时间.纳秒)return{动态数据状态::时间不递增,r.G0};
        const auto left=读取I64(r.G0,r.来源截止,*bs.内容);if(!left.值)return{left.状态,r.G0};
        const auto right=读取I64(r.G0,r.来源截止,*as.内容);if(!right.值)return{right.状态,r.G0};
        if(*left.值==*right.值)return{动态数据状态::无变化,r.G0};
        if(r.预算.最大来源边数<4||r.预算.最大读取材料数<=4)
            return{动态数据状态::数量预算不足,r.G0};
        std::uint64_t kMaterials=0;
        const auto compared=核验比较(r,*bs.内容,*left.值,*right.值,
            r.预算.最大候选数,r.预算.最大读取材料数-4,kMaterials);
        if(compared!=动态数据状态::已读取)return{compared,r.G0};
        if(kMaterials>r.预算.最大读取材料数-4)return{动态数据状态::内部不一致,r.G0};
        return{动态数据状态::已读取,r.G0,4,4+kMaterials};
    }

    I64读取结果 读取I64(std::uint64_t g,std::uint64_t h,const 状态内容事实&s)const{
        const auto check=feature_.核验正式特征类型准确值({1,g,h,s.信息.正式特征类型,s.信息.固定准确值});
        if(!check.成功())return{映射特征核验(check.状态),std::nullopt};
        if(!check.事实||check.事实->Gread!=g||check.事实->H!=h||check.事实->正式特征类型!=s.信息.正式特征类型
            ||check.事实->准确值!=s.信息.固定准确值)return{动态数据状态::内部不一致,std::nullopt};
        if(const auto*n=std::get_if<std::int64_t>(&s.信息.固定准确值))return{动态数据状态::已读取,*n};
        const auto expected=std::get<特征值身份>(s.信息.固定准确值);
        const auto read=value_.获取特征值(std::get<特征值身份>(s.信息.固定准确值));
        const auto*v=std::get_if<特征值信息>(&read);
        if(!v)return{映射特征值(std::get<特征值读取错误>(read)),std::nullopt};
        if(v->值身份!=expected)return{动态数据状态::内部不一致,std::nullopt};
        const auto*n=std::get_if<std::int64_t>(&v->值内容);if(!n)return{动态数据状态::比较依据不支持,std::nullopt};
        const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取||guard.Gread!=g)
            return{guard.状态==动态数据状态::已读取 ? 动态数据状态::事实代次漂移 : guard.状态,std::nullopt};
        return{动态数据状态::已读取,*n};
    }

    动态数据状态 核验比较(const 动态原子创建请求&r,const 状态内容事实&s,std::int64_t left,std::int64_t right,
        std::uint64_t maxCandidates,std::uint64_t maxMaterials,std::uint64_t& usedMaterials)const{
        const auto binding=feature_.读取当前I64比较绑定({1,r.G0,s.信息.正式特征类型,特征I64比较用途::变化分析,
            maxCandidates,maxMaterials});
        if(!binding.成功())return 映射比较绑定(binding.状态);
        if(!binding.事实||binding.Gread!=r.G0||binding.H!=r.G0)return 动态数据状态::内部不一致;
        if(!binding.当前读取原请求||binding.当前读取原请求->最大扫描候选数量!=maxCandidates
            ||binding.当前读取原请求->最大读取材料数!=maxMaterials
            ||binding.当前读取用量.材料总数>maxMaterials
            ||binding.当前读取用量.扫描候选数>maxCandidates)return 动态数据状态::内部不一致;
        usedMaterials=binding.当前读取用量.材料总数;
        const auto&f=*binding.事实;
        if(f.定义.输入FT!=s.信息.正式特征类型||f.定义.用途!=特征I64比较用途::变化分析
            ||f.定义.左角色!=特征I64输入角色::旧状态||f.定义.右角色!=特征I64输入角色::新状态
            ||f.定义.上下文要求位!=0)return 动态数据状态::内部不一致;
        有序I64比较合同快照 k{特征比较合同来源::当前独立绑定,f.身份,f.定义.输入FT,f.定义.用途,
            f.定义.算法族,f.定义.算法版本,f.定义.左角色,f.定义.右角色,f.定义.上下文要求位,f.定义.输入量化,
            f.定义.误差合同版本,f.定义.误差预算,f.定义.相等容差,f.定义.关系编码,f.输出组,binding.Gread,binding.H};
        if(!(r.变化依据.比较请求.K==k)||r.变化依据.比较请求.版本!=1||!r.变化依据.比较请求.请求身份
            ||r.变化依据.比较请求.输入[0].角色!=特征I64输入角色::旧状态||r.变化依据.比较请求.输入[1].角色!=特征I64输入角色::新状态
            ||r.变化依据.比较请求.输入[0].值!=left||r.变化依据.比较请求.输入[1].值!=right
            ||!(r.变化依据.比较请求.要求结果位&static_cast<std::uint8_t>(特征比较结果要求::排序三态)))return 动态数据状态::比较依据不支持;
        const auto&result=r.变化依据.比较结果;if(!result.成功()||result.请求身份!=r.变化依据.比较请求.请求身份
            ||result.K!=k.K||result.算法版本!=k.算法版本||result.实际结果位!=r.变化依据.比较请求.要求结果位)
            return 动态数据状态::比较依据不支持;
        for(const auto&o:result.输出组)if(o.角色==特征类标量结果角色::排序)
            return o.值!=0 ? 动态数据状态::已读取 : 动态数据状态::无变化;
        return 动态数据状态::比较依据不支持;
    }

    动态数据状态 核验组合来源(动态信息身份 id,稳定编码 subject,std::uint64_t g,std::uint64_t h,const 动态读取预算&budget,
        std::uint64_t depth,std::set<std::uint64_t>&visited,std::set<std::uint64_t>&active,std::uint64_t&dynamics,std::uint64_t&edges,
        std::uint64_t&materials,std::uint64_t&maxDepth)const{
        if(depth>budget.最大深度)return 动态数据状态::数量预算不足;if(active.contains(id.编码.值))return 动态数据状态::来源成环;
        maxDepth=std::max(maxDepth,depth);if(visited.contains(id.编码.值))return 动态数据状态::已读取;
        if(++dynamics>budget.最大动态数)return 动态数据状态::数量预算不足;
        active.insert(id.编码.值);auto read=读取历史({1,g,h,id,budget},std::nullopt);if(!read.成功())return 映射来源动态(read.操作.状态);
        const auto selfMaterials=4+read.内容->自有事实.变化关系组.size();
        if(selfMaterials>budget.最大读取材料数||materials>budget.最大读取材料数-selfMaterials)return 动态数据状态::数量预算不足;materials+=selfMaterials;
        const auto current=读取身份({1,g,g,id});if(!current.成功())return 映射来源动态(current.操作.状态);
        if(read.内容->信息.主体存在!=subject)return 动态数据状态::主体不一致;
        if(const auto*a=std::get_if<动态原子内容>(&read.内容->信息.变化)){
            if(budget.最大来源边数<4||edges>budget.最大来源边数-4)return 动态数据状态::数量预算不足;edges+=4;
            const auto b=binding_.读取状态使用绑定历史({1,g,h,a->前绑定});const auto c=binding_.读取状态使用绑定历史({1,g,h,a->后绑定});
            if(!b.成功()||!c.成功())return !b.成功() ? 映射绑定(b.结果头.状态) : 映射绑定(c.结果头.状态);
            if(b.结果头.合同版本!=1||b.结果头.事实截止代次!=g||b.结果头.变更事实代次||b.绑定->Gread!=g||b.绑定->H!=h
                ||c.结果头.合同版本!=1||c.结果头.事实截止代次!=g||c.结果头.变更事实代次||c.绑定->Gread!=g||c.绑定->H!=h
                ||b.绑定->信息.身份!=a->前绑定||c.绑定->信息.身份!=a->后绑定)
                return 动态数据状态::内部不一致;
            const auto bs=state_.读取状态历史内容({2,g,h,b.绑定->信息.状态});
            const auto cs=state_.读取状态历史内容({2,g,h,c.绑定->信息.状态});
            if(!bs.成功()||!cs.成功())return !bs.成功()?映射状态(bs.结果头.状态):映射状态(cs.结果头.状态);
            if(bs.结果头.合同版本!=2||bs.结果头.事实截止代次!=g||bs.结果头.变更事实代次||bs.内容->Gread!=g||bs.内容->H!=h
                ||cs.结果头.合同版本!=2||cs.结果头.事实截止代次!=g||cs.结果头.变更事实代次||cs.内容->Gread!=g||cs.内容->H!=h
                ||b.绑定->信息.状态!=bs.内容->信息.身份||c.绑定->信息.状态!=cs.内容->信息.身份)
                return 动态数据状态::内部不一致;
            if(budget.最大读取材料数<4||materials>budget.最大读取材料数-4)return 动态数据状态::数量预算不足;materials+=4;
        }else for(const auto&m:std::get<动态组合内容>(read.内容->信息.变化).成员组){if(++edges>budget.最大来源边数)return 动态数据状态::数量预算不足;
            const auto s=核验组合来源(m.子动态,subject,g,h,budget,depth+1,visited,active,dynamics,edges,materials,maxDepth);if(s!=动态数据状态::已读取)return s;}
        active.erase(id.编码.值);visited.insert(id.编码.值);return 动态数据状态::已读取;
    }

    动态单项结果 读取历史(const 动态历史读取请求&r,std::optional<std::uint64_t>expectedExit)const{
        if(是交付元节点(r.身份.编码))return 单项失败(动态数据状态::未找到,r.Gread,r.H);
        const auto nr=l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,r.身份.编码});
        if(nr.合同版本!=L1所有者范围CRUD合同版本||nr.查询编码!=r.身份.编码)return 单项失败(动态数据状态::内部不一致,nr.读取事实代次,r.H);
        if(nr.状态!=L1所有者范围读取状态::成功)return 单项失败(映射读取(nr.状态),nr.读取事实代次,r.H);
        if(nr.读取事实代次!=r.Gread)return 单项失败(动态数据状态::事实代次漂移,nr.读取事实代次,r.H);
        const auto*n=nr.事实 ? std::get_if<L1所有者范围节点事实>(&*nr.事实) : nullptr;
        if(!n||n->编码!=r.身份.编码||n->写入所有者!=owner_||n->种类!=节点种类::普通||n->属性类型表示)
            return 单项失败(动态数据状态::未找到,r.Gread,r.H);
        if(!n->创建事实代次)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        if(n->创建事实代次>r.H)return 单项失败(动态数据状态::未找到,r.Gread,r.H);
        if(n->退出事实代次&&*n->退出事实代次<=r.H)return 单项失败(动态数据状态::目标已退出,r.Gread,r.H);
        if(expectedExit&&n->退出事实代次!=expectedExit)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        auto families=关系组(r.身份.编码,layout_.族归属关系类型,r.Gread,r.H,n->创建事实代次,n->退出事实代次);
        if(!families)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        if(families->empty())return 单项失败(动态数据状态::旧格式不支持,r.Gread,r.H);
        if(families->size()!=1)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        const auto family=families->front();
        if(family.目标节点!=layout_.动态族锚点||family.角色或顺序!=1)
            return 单项失败(动态数据状态::旧格式不支持,r.Gread,r.H);
        auto subjects=关系组(r.身份.编码,layout_.主体关系类型,r.Gread,r.H,n->创建事实代次,n->退出事实代次);
        if(!subjects||subjects->size()!=1)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        const auto subject=subjects->front();
        if(subject.角色或顺序!=0)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        const auto attrs=l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本,r.身份.编码,r.H});
        if(attrs.状态!=L1所有者范围读取状态::成功||attrs.合同版本!=L1所有者范围CRUD合同版本
            ||attrs.读取事实代次!=r.Gread||attrs.所属节点!=r.身份.编码||attrs.历史截止事实代次!=r.H)
            return 单项失败(映射读取(attrs.状态),attrs.读取事实代次,r.H);
        std::vector<L1所有者范围值事实> ownValues;for(const auto&v:attrs.属性值组){
            if(v.写入所有者==owner_)ownValues.push_back(v);
            else if(v.属性类型节点==layout_.首次形成UTC属性类型)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);}
        if(ownValues.size()!=1)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        const auto&utc=ownValues.front();const auto*formed=std::get_if<std::int64_t>(&utc.材料);
        if(!formed||*formed<0||utc.所属节点!=r.身份.编码||utc.属性类型节点!=layout_.首次形成UTC属性类型
            ||utc.写入所有者!=owner_||utc.来源节点!=r.身份.编码||utc.创建事实代次!=n->创建事实代次
            ||utc.退出事实代次!=n->退出事实代次)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        auto pre=关系组(r.身份.编码,layout_.前绑定关系类型,r.Gread,r.H,n->创建事实代次,n->退出事实代次);
        auto post=关系组(r.身份.编码,layout_.后绑定关系类型,r.Gread,r.H,n->创建事实代次,n->退出事实代次);
        auto members=关系组(r.身份.编码,layout_.同主体成员关系类型,r.Gread,r.H,n->创建事实代次,n->退出事实代次);
        if(!pre||!post||!members)return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        动态变化内容 change;std::vector<稳定编码>changes;
        if(pre->size()==1&&post->size()==1&&members->empty()){
            if(pre->front().角色或顺序||post->front().角色或顺序||pre->front().目标节点==post->front().目标节点)
                return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
            change=动态原子内容{{pre->front().目标节点},{post->front().目标节点}};changes={pre->front().编码,post->front().编码};
        }else if(pre->empty()&&post->empty()&&!members->empty()){
            std::sort(members->begin(),members->end(),[](const auto&a,const auto&b){return a.角色或顺序<b.角色或顺序;});动态组合内容 c;
            for(std::size_t i=0;i<members->size();++i){const auto&e=(*members)[i];if(e.角色或顺序!=static_cast<std::int64_t>(i+1))return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
                c.成员组.push_back({static_cast<std::uint32_t>(i+1),动态成员角色::同主体,{e.目标节点},std::nullopt});changes.push_back(e.编码);}change=std::move(c);
        }else return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        if(4+changes.size()>r.预算.最大读取材料数)return 单项失败(动态数据状态::数量预算不足,r.Gread,r.H);
        动态内容事实 out{r.Gread,r.H,{r.身份,subject.目标节点,std::move(change)},
            {family.编码,subject.编码,utc.编码,std::move(changes)},*formed,n->创建事实代次,n->退出事实代次};
        动态单项结果 result{读取头(r.Gread,r.H),std::move(out)};
        if(!result.成功())return 单项失败(动态数据状态::内部不一致,r.Gread,r.H);
        return result;
    }

    std::optional<std::vector<L1所有者范围关系事实>> 关系组(稳定编码 source,稳定编码 type,std::uint64_t g,std::uint64_t h,
        std::uint64_t create,std::optional<std::uint64_t> exit)const{
        const auto q=l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,source,type,h});
        if(q.状态!=L1所有者范围读取状态::成功||q.合同版本!=L1所有者范围CRUD合同版本||q.读取事实代次!=g
            ||q.方向!=L1所有者范围关系端点方向::源||q.端点节点!=source||q.关系类型节点!=type
            ||q.历史截止事实代次!=h)return std::nullopt;
        std::vector<L1所有者范围关系事实> out;for(const auto&e:q.关系组){
            if(e.写入所有者!=owner_||!有效(e.编码)||e.源节点!=source||e.关系类型节点!=type
                ||e.创建事实代次!=create||e.退出事实代次!=exit)return std::nullopt;
            out.push_back(e);}return out;}

    动态组结果 查询关系(std::uint64_t g,std::uint64_t h,稳定编码 target,稳定编码 type,查询方向 direction,const 动态读取预算&budget,
        std::uint64_t&usedCandidates,std::uint64_t&usedDynamics,std::uint64_t&usedMaterials)const{
        try {const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取)return 组失败(guard.状态,guard.Gread,h);
            if(guard.Gread!=g)return 组失败(动态数据状态::事实代次漂移,guard.Gread,h);
            const auto q=l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,
                direction==查询方向::目标 ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源,target,type,h});
            if(q.状态!=L1所有者范围读取状态::成功)return 组失败(映射读取(q.状态),q.读取事实代次,h);
            if(q.合同版本!=L1所有者范围CRUD合同版本||q.读取事实代次!=g||q.端点节点!=target||q.关系类型节点!=type
                ||q.历史截止事实代次!=h||q.方向!=(direction==查询方向::目标 ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源))
                return 组失败(q.读取事实代次!=g ? 动态数据状态::事实代次漂移 : 动态数据状态::内部不一致,q.读取事实代次,h);
            std::vector<动态信息身份>ids;for(const auto&e:q.关系组){
                if(e.写入所有者!=owner_||!有效(e.编码)||e.关系类型节点!=type||(direction==查询方向::目标 ? e.目标节点!=target : e.源节点!=target)
                    ||!e.创建事实代次||e.创建事实代次>h||(e.退出事实代次&&*e.退出事实代次<=h))return 组失败(动态数据状态::内部不一致,g,h);
                if(++usedCandidates>budget.最大候选数)return 组失败(动态数据状态::数量预算不足,g,h);
                ids.push_back({direction==查询方向::目标 ? e.源节点 : e.目标节点});}
            std::sort(ids.begin(),ids.end(),[](auto a,auto b){return a.编码<b.编码;});ids.erase(std::unique(ids.begin(),ids.end()),ids.end());
            if(ids.size()>budget.最大动态数||usedDynamics>budget.最大动态数-ids.size())return 组失败(动态数据状态::数量预算不足,g,h);usedDynamics+=ids.size();
            动态组结果 out{读取头(g,h),{}};for(auto id:ids){auto d=读取历史({1,g,h,id,budget},std::nullopt);if(!d.成功())return 组失败(d.操作.状态,d.操作.Gread,h);
                const auto cost=4+d.内容->自有事实.变化关系组.size();if(cost>budget.最大读取材料数||usedMaterials>budget.最大读取材料数-cost)
                    return 组失败(动态数据状态::数量预算不足,g,h);usedMaterials+=cost;out.动态组.push_back(*d.内容);}return out;
        }catch(const std::bad_alloc&){return 组失败(动态数据状态::资源失败);}
        catch(const std::length_error&){return 组失败(动态数据状态::资源失败);}
        catch(...){return 组失败(动态数据状态::内部不一致);}}

    动态数据状态 展开一个(动态信息身份 id,动态展开方式 mode,const 动态读取预算&budget,std::uint64_t depth,
        std::set<std::uint64_t>&visited,std::set<std::uint64_t>&active,std::uint64_t&edges,std::uint64_t&materials,动态来源展开事实&facts)const{
        if(depth>budget.最大深度)return 动态数据状态::数量预算不足;if(active.contains(id.编码.值))return 动态数据状态::来源成环;
        if(visited.contains(id.编码.值))return 动态数据状态::已读取;if(facts.动态组.size()>=budget.最大动态数)return 动态数据状态::数量预算不足;
        active.insert(id.编码.值);auto read=读取历史({1,facts.Gread,facts.H,id,budget},std::nullopt);if(!read.成功())return depth==1?read.操作.状态:映射来源动态(read.操作.状态);
        const auto selfMaterials=4+read.内容->自有事实.变化关系组.size();
        if(selfMaterials>budget.最大读取材料数||materials>budget.最大读取材料数-selfMaterials)return 动态数据状态::数量预算不足;
        materials+=selfMaterials;facts.动态组.push_back(*read.内容);
        if(const auto*a=std::get_if<动态原子内容>(&read.内容->信息.变化)){
            if(budget.最大来源边数<4||budget.最大读取材料数<4||edges>budget.最大来源边数-4
                ||materials>budget.最大读取材料数-4)return 动态数据状态::数量预算不足;edges+=4;materials+=4;
            const auto b=binding_.读取状态使用绑定历史({1,facts.Gread,facts.H,a->前绑定});const auto c=binding_.读取状态使用绑定历史({1,facts.Gread,facts.H,a->后绑定});
            if(!b.成功()||!c.成功())return !b.成功() ? 映射绑定(b.结果头.状态) : 映射绑定(c.结果头.状态);
            if(b.结果头.合同版本!=1||b.结果头.事实截止代次!=facts.Gread||b.结果头.变更事实代次
                ||b.绑定->Gread!=facts.Gread||b.绑定->H!=facts.H||b.绑定->信息.身份!=a->前绑定
                ||c.结果头.合同版本!=1||c.结果头.事实截止代次!=facts.Gread||c.结果头.变更事实代次
                ||c.绑定->Gread!=facts.Gread||c.绑定->H!=facts.H||c.绑定->信息.身份!=a->后绑定)
                return 动态数据状态::内部不一致;
            const auto bs=state_.读取状态历史内容({2,facts.Gread,facts.H,b.绑定->信息.状态});const auto cs=state_.读取状态历史内容({2,facts.Gread,facts.H,c.绑定->信息.状态});
            if(!bs.成功()||!cs.成功())return !bs.成功() ? 映射状态(bs.结果头.状态) : 映射状态(cs.结果头.状态);
            if(bs.结果头.合同版本!=2||bs.结果头.事实截止代次!=facts.Gread||bs.结果头.变更事实代次
                ||bs.内容->Gread!=facts.Gread||bs.内容->H!=facts.H||bs.内容->信息.身份!=b.绑定->信息.状态
                ||cs.结果头.合同版本!=2||cs.结果头.事实截止代次!=facts.Gread||cs.结果头.变更事实代次
                ||cs.内容->Gread!=facts.Gread||cs.内容->H!=facts.H||cs.内容->信息.身份!=c.绑定->信息.状态)
                return 动态数据状态::内部不一致;
            facts.原子来源组.push_back({id,*b.绑定,*c.绑定,*bs.内容,*cs.内容});
        }else if(mode==动态展开方式::递归||depth==1){for(const auto&m:std::get<动态组合内容>(read.内容->信息.变化).成员组){if(++edges>budget.最大来源边数)return 动态数据状态::数量预算不足;
            const auto s=展开一个(m.子动态,mode,budget,depth+1,visited,active,edges,materials,facts);if(s!=动态数据状态::已读取)return s;}}
        active.erase(id.编码.值);visited.insert(id.编码.值);return 动态数据状态::已读取;
    }

    动态身份历史结果 读取身份(const 动态历史身份请求&r)const{
        if(是交付元节点(r.身份.编码))return 身份失败(动态数据状态::未找到,r.Gread,r.H);
        const auto raw=l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,r.身份.编码});
        if(raw.合同版本!=L1所有者范围CRUD合同版本||raw.查询编码!=r.身份.编码)
            return 身份失败(动态数据状态::内部不一致,raw.读取事实代次,r.H);
        if(raw.状态!=L1所有者范围读取状态::成功)return 身份失败(映射读取(raw.状态),raw.读取事实代次,r.H);
        if(raw.读取事实代次!=r.Gread)return 身份失败(动态数据状态::事实代次漂移,raw.读取事实代次,r.H);
        const auto*n=raw.事实 ? std::get_if<L1所有者范围节点事实>(&*raw.事实) : nullptr;
        if(!n||n->编码!=r.身份.编码||n->写入所有者!=owner_||n->种类!=节点种类::普通||n->属性类型表示)
            return 身份失败(动态数据状态::未找到,r.Gread,r.H);
        if(!n->创建事实代次)return 身份失败(动态数据状态::内部不一致,r.Gread,r.H);
        if(n->创建事实代次>r.H)return 身份失败(动态数据状态::未找到,r.Gread,r.H);
        if(n->退出事实代次&&*n->退出事实代次<=r.H)return 身份失败(动态数据状态::目标已退出,r.Gread,r.H);
        const auto q=l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,
            r.身份.编码,layout_.族归属关系类型,r.H});
        if(q.状态!=L1所有者范围读取状态::成功)return 身份失败(映射读取(q.状态),q.读取事实代次,r.H);
        if(q.合同版本!=L1所有者范围CRUD合同版本||q.读取事实代次!=r.Gread
            ||q.方向!=L1所有者范围关系端点方向::源||q.端点节点!=r.身份.编码
            ||q.关系类型节点!=layout_.族归属关系类型||q.历史截止事实代次!=r.H)
            return 身份失败(q.读取事实代次!=r.Gread?动态数据状态::事实代次漂移:动态数据状态::内部不一致,q.读取事实代次,r.H);
        std::vector<L1所有者范围关系事实> own;for(const auto&e:q.关系组){if(e.写入所有者!=owner_)
                return 身份失败(动态数据状态::内部不一致,r.Gread,r.H);own.push_back(e);}
        if(own.empty())return 身份失败(动态数据状态::旧格式不支持,r.Gread,r.H);
        if(own.size()!=1)return 身份失败(动态数据状态::内部不一致,r.Gread,r.H);
        const auto&f=own.front();
        if(f.编码==r.身份.编码||f.源节点!=r.身份.编码||f.目标节点!=layout_.动态族锚点
            ||f.关系类型节点!=layout_.族归属关系类型||f.角色或顺序!=1
            ||f.创建事实代次!=n->创建事实代次||f.退出事实代次!=n->退出事实代次)
            return 身份失败(动态数据状态::内部不一致,r.Gread,r.H);
        动态身份历史结果 out{读取头(r.Gread,r.H),动态身份历史事实{r.Gread,r.H,r.身份,layout_.动态族锚点,
            layout_.族归属关系类型,f.编码,n->创建事实代次,n->退出事实代次}};
        return out.成功()?out:身份失败(动态数据状态::内部不一致,r.Gread,r.H);
    }
    static void 添加退出事实(std::vector<稳定编码>&out,const 动态内容事实&v){out.push_back(v.信息.身份.编码);out.push_back(v.自有事实.族归属关系);
        out.push_back(v.自有事实.主体关系);out.push_back(v.自有事实.首次形成UTC值);out.insert(out.end(),v.自有事实.变化关系组.begin(),v.自有事实.变化关系组.end());}
    L1所有者范围写集请求 形成退出写集(const 动态内容事实&v,std::uint64_t g,L1所有者范围写入幂等身份 key)const{
        L1所有者范围写集请求 w{L1所有者范围CRUD合同版本,g,key};添加退出事实(w.退出事实,v);std::sort(w.退出事实.begin(),w.退出事实.end());return w;}
    std::optional<动态单项结果> 重放退出(const 动态退出请求&r){
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.状态==L1所有者范围读取状态::未找到)return std::nullopt;
        if(first.状态!=L1所有者范围读取状态::成功||!first.首次规范化写集||!first.首次写入结果)return 单项失败(映射读取(first.状态),first.读取事实代次);
        const auto&ws=*first.首次规范化写集;if(ws.期望事实代次!=r.G0||ws.写入幂等身份!=r.幂等身份||ws.退出事实.empty())return 单项失败(动态数据状态::幂等冲突,first.读取事实代次);
        const auto saved=port_.提交所有者范围中性写集(ws);if(saved.状态!=L1所有者范围写入状态::精确重复||!写入头完整(saved,r.幂等身份,ws.期望事实代次))return 提交失败(映射写入(saved.状态,动态数据状态::已退出),saved.事实代次);
        const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取)return 提交后失败(saved.事实代次,guard.Gread);
        auto read=读取历史({1,guard.Gread,saved.事实代次-1,r.身份,r.预算},saved.事实代次);if(!read.成功())return 提交后失败(saved.事实代次,read.操作.Gread);
        std::vector<稳定编码>expected;添加退出事实(expected,*read.内容);std::sort(expected.begin(),expected.end());if(expected!=ws.退出事实)return 单项失败(动态数据状态::幂等冲突,guard.Gread);
        return 动态单项结果{{动态数据状态::精确重复,1,guard.Gread,saved.事实代次-1,saved.事实代次,动态发布阶段::已读回},std::move(read.内容)};}

    static bool 退出写集形状有效(const L1所有者范围写集请求&w,L1所有者范围写入幂等身份 key)noexcept{
        if(w.合同版本!=L1所有者范围CRUD合同版本||!w.期望事实代次||w.写入幂等身份!=key
            ||!w.节点.empty()||!w.关系.empty()||!w.值.empty()||!w.属性槽变更.empty()||w.退出事实.empty()
            ||!std::is_sorted(w.退出事实.begin(),w.退出事实.end()))return false;
        for(std::size_t i=0;i<w.退出事实.size();++i)if(!有效(w.退出事实[i])||(i&&w.退出事实[i]==w.退出事实[i-1]))return false;
        return true;}

    bool 清理事实组结构完整(const std::vector<L1所有者范围节点事实>&nodes,
        const std::vector<L1所有者范围关系事实>&relations,const std::vector<L1所有者范围值事实>&values,
        std::uint64_t exitG)const noexcept{
        if(nodes.empty()||values.size()!=nodes.size())return false;
        for(const auto&n:nodes){
            if(!有效(n.编码)||n.写入所有者!=owner_||n.种类!=节点种类::普通||n.属性类型表示
                ||!n.创建事实代次||n.退出事实代次!=exitG)return false;
            std::size_t family=0,subject=0,pre=0,post=0;std::optional<稳定编码>preTarget,postTarget;
            std::vector<std::pair<std::int64_t,稳定编码>>members;
            for(const auto&e:relations){if(e.源节点!=n.编码)continue;
                if(!有效(e.编码)||e.写入所有者!=owner_||e.创建事实代次!=n.创建事实代次
                    ||e.退出事实代次!=exitG)return false;
                if(e.关系类型节点==layout_.族归属关系类型&&e.目标节点==layout_.动态族锚点&&e.角色或顺序==1)++family;
                else if(e.关系类型节点==layout_.主体关系类型&&有效(e.目标节点)&&e.角色或顺序==0)++subject;
                else if(e.关系类型节点==layout_.前绑定关系类型&&有效(e.目标节点)&&e.角色或顺序==0){++pre;preTarget=e.目标节点;}
                else if(e.关系类型节点==layout_.后绑定关系类型&&有效(e.目标节点)&&e.角色或顺序==0){++post;postTarget=e.目标节点;}
                else if(e.关系类型节点==layout_.同主体成员关系类型&&有效(e.目标节点)&&e.角色或顺序>0)members.push_back({e.角色或顺序,e.目标节点});
                else return false;}
            std::size_t utc=0;for(const auto&v:values){if(v.所属节点!=n.编码)continue;
                const auto*x=std::get_if<std::int64_t>(&v.材料);
                if(!有效(v.编码)||v.写入所有者!=owner_||v.属性类型节点!=layout_.首次形成UTC属性类型
                    ||v.来源节点!=n.编码||v.创建事实代次!=n.创建事实代次||v.退出事实代次!=exitG||!x||*x<0)return false;++utc;}
            if(family!=1||subject!=1||utc!=1)return false;
            if(pre==1&&post==1&&members.empty()&&preTarget&&postTarget&&*preTarget!=*postTarget)continue;
            if(pre||post||members.empty())return false;
            std::sort(members.begin(),members.end());std::set<std::uint64_t>targets;
            for(std::size_t i=0;i<members.size();++i)if(members[i].first!=static_cast<std::int64_t>(i+1)||!targets.insert(members[i].second.值).second)return false;
        }
        for(const auto&e:relations)if(std::none_of(nodes.begin(),nodes.end(),[&](const auto&n){return n.编码==e.源节点;}))return false;
        for(const auto&v:values)if(std::none_of(nodes.begin(),nodes.end(),[&](const auto&n){return n.编码==v.所属节点;}))return false;
        return true;}

    static bool 清理墓碑组形状完整(const std::vector<L1所有者范围物理清理墓碑>&tombs)noexcept{
        struct Counts{std::size_t nodes=0,relations=0,values=0;};std::map<std::uint64_t,Counts>groups;
        for(const auto&m:tombs){auto&g=groups[m.创建事实代次];
            if(m.事实种类==L1所有者范围物理清理事实种类::节点)++g.nodes;
            else if(m.事实种类==L1所有者范围物理清理事实种类::关系)++g.relations;
            else if(m.事实种类==L1所有者范围物理清理事实种类::值)++g.values;else return false;}
        if(groups.empty())return false;for(const auto&[_,g]:groups)if(g.nodes!=1||g.values!=1||g.relations<3)return false;return true;}

    std::optional<动态清理结果> 重放清理(const 动态清理请求&r){
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.退出幂等身份});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||first.所有者!=owner_||first.写入幂等身份!=r.退出幂等身份)
            return 清理失败(动态数据状态::内部不一致,first.读取事实代次);
        if(first.状态==L1所有者范围读取状态::未找到){if(!first.读取事实代次||first.首次规范化写集||first.首次写入结果)
                return 清理失败(动态数据状态::内部不一致,first.读取事实代次);return std::nullopt;}
        if(first.状态!=L1所有者范围读取状态::成功)return 清理失败(映射读取(first.状态),first.读取事实代次);
        if(!first.首次规范化写集||!first.首次写入结果)return 清理失败(动态数据状态::内部不一致,first.读取事实代次);
        const auto&ws=*first.首次规范化写集;
        if(!退出写集形状有效(ws,r.退出幂等身份)||first.首次写入结果->状态!=L1所有者范围写入状态::成功
            ||!写入头完整(*first.首次写入结果,r.退出幂等身份,ws.期望事实代次)||!first.首次写入结果->新编码映射.empty())
            return 清理失败(动态数据状态::幂等冲突,first.读取事实代次);
        const auto exited=port_.提交所有者范围中性写集(ws);
        if(exited.状态!=L1所有者范围写入状态::精确重复||!写入头完整(exited,r.退出幂等身份,ws.期望事实代次))
            return 清理失败(映射写入(exited.状态,动态数据状态::已退出),exited.事实代次);
        std::uint64_t cleanG=r.G0;
        if(r.G0==ws.期望事实代次)cleanG=exited.事实代次;
        else if(r.G0<exited.事实代次)return 清理失败(动态数据状态::事实代次漂移,first.读取事实代次);
        L1物理清理请求 clean{L1物理清理合同版本,cleanG,r.清理幂等身份,{}};
        for(auto id:ws.退出事实)clean.待清理事实身份组.push_back({L1物理清理事实种类::值,id});
        return 执行清理(std::move(clean),exited.事实代次);
    }

    动态清理结果 执行清理(L1物理清理请求 q,std::uint64_t exitG){
        std::vector<L1所有者范围节点事实>nodes;std::vector<L1所有者范围关系事实>relations;
        std::vector<L1所有者范围值事实>values;std::vector<L1所有者范围物理清理墓碑>priorTombstones;std::size_t tombstones=0;
        for(auto&item:q.待清理事实身份组){const auto raw=l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,item.编码});
            if(raw.合同版本!=L1所有者范围CRUD合同版本||raw.查询编码!=item.编码)return 清理失败(动态数据状态::内部不一致,raw.读取事实代次);
            if(raw.状态==L1所有者范围读取状态::成功&&raw.事实&&!raw.物理清理事实代次&&!raw.物理清理墓碑){
                bool valid=false;std::visit([&](const auto&fact){using T=std::decay_t<decltype(fact)>;
                    if constexpr(std::is_same_v<T,L1所有者范围节点事实>){item.事实种类=L1物理清理事实种类::节点;nodes.push_back(fact);}
                    else if constexpr(std::is_same_v<T,L1所有者范围关系事实>){item.事实种类=L1物理清理事实种类::关系;relations.push_back(fact);}
                    else{item.事实种类=L1物理清理事实种类::值;values.push_back(fact);}
                    valid=fact.编码==item.编码&&fact.写入所有者==owner_&&fact.创建事实代次
                        &&fact.创建事实代次<exitG&&fact.退出事实代次==exitG;},*raw.事实);
                if(!valid)return 清理失败(动态数据状态::内部不一致,raw.读取事实代次);
            }else if(raw.状态==L1所有者范围读取状态::历史材料已清理&&!raw.事实&&raw.物理清理事实代次&&raw.物理清理墓碑){
                const auto&m=*raw.物理清理墓碑;
                if(m.编码!=item.编码||m.内部结构分区!=owner_.编码||!m.创建事实代次||m.创建事实代次>=m.退出事实代次
                    ||m.退出事实代次!=exitG||m.退出事实代次>=m.物理清理事实代次||m.物理清理事实代次!=*raw.物理清理事实代次)
                    return 清理失败(动态数据状态::内部不一致,raw.读取事实代次);
                item.事实种类=static_cast<L1物理清理事实种类>(m.事实种类);priorTombstones.push_back(m);++tombstones;
            }else return 清理失败(映射读取(raw.状态),raw.读取事实代次);}
        if(tombstones&&tombstones!=q.待清理事实身份组.size())return 清理失败(动态数据状态::内部不一致);
        std::sort(q.待清理事实身份组.begin(),q.待清理事实身份组.end());
        for(std::size_t i=0;i<q.待清理事实身份组.size();++i)
            if(!L1物理清理事实身份有效(q.待清理事实身份组[i])||(i&&q.待清理事实身份组[i].编码==q.待清理事实身份组[i-1].编码))
                return 清理失败(动态数据状态::内部不一致);
        if(tombstones){if(!清理墓碑组形状完整(priorTombstones))return 清理失败(动态数据状态::内部不一致);}
        else if(!清理事实组结构完整(nodes,relations,values,exitG))return 清理失败(动态数据状态::内部不一致);
        const auto cleaned=l1_.执行L1物理清理(q);const auto s=映射清理(cleaned.状态);
        if(s!=动态数据状态::已清理&&s!=动态数据状态::精确重复){
            if(tombstones&&(cleaned.状态==L1物理清理状态::事实代次漂移||cleaned.状态==L1物理清理状态::未找到))
                return 清理失败(动态数据状态::幂等冲突,cleaned.物理清理事实代次);
            return 清理失败(s,cleaned.物理清理事实代次);}
        if(cleaned.合同版本!=L1物理清理合同版本||cleaned.幂等身份!=q.幂等身份||cleaned.墓碑组.size()!=q.待清理事实身份组.size()
            ||cleaned.稳定编码映射.size()!=q.待清理事实身份组.size()||!cleaned.物理清理事实代次
            ||cleaned.物理清理事实代次!=q.期望事实代次+1)return 清理提交后失败(cleaned.物理清理事实代次);
        for(const auto&expected:q.待清理事实身份组){std::size_t maps=0,tombs=0;for(const auto&[id,code]:cleaned.稳定编码映射)if(id==expected){if(code!=expected.编码)return 清理提交后失败(cleaned.物理清理事实代次);++maps;}
            for(const auto&m:cleaned.墓碑组)if(m.编码==expected.编码){if(m.事实种类!=expected.事实种类||m.内部结构分区!=owner_.编码
                    ||!m.创建事实代次||m.创建事实代次>=m.退出事实代次||m.退出事实代次!=exitG
                    ||m.退出事实代次>=m.物理清理事实代次||m.物理清理事实代次!=cleaned.物理清理事实代次)
                    return 清理提交后失败(cleaned.物理清理事实代次);++tombs;}if(maps!=1||tombs!=1)return 清理提交后失败(cleaned.物理清理事实代次);}
        const auto guard=读取当前代次();if(guard.状态!=动态数据状态::已读取||guard.Gread<cleaned.物理清理事实代次)
            return 清理提交后失败(cleaned.物理清理事实代次,guard.Gread);
        auto tombstonesOut=cleaned.墓碑组;std::sort(tombstonesOut.begin(),tombstonesOut.end(),[](const auto&a,const auto&b){
            return L1物理清理事实身份{a.事实种类,a.编码}<L1物理清理事实身份{b.事实种类,b.编码};});
        动态清理结果 out{{s,1,guard.Gread,cleaned.物理清理事实代次,cleaned.物理清理事实代次,动态发布阶段::已读回},std::move(tombstonesOut)};
        return out.成功()?out:清理提交后失败(cleaned.物理清理事实代次,guard.Gread);}

    static 动态数据状态 映射特征核验(特征类型准确值核验状态 s)noexcept{switch(s){
        case 特征类型准确值核验状态::正式特征类型未找到:case 特征类型准确值核验状态::准确值未找到:return 动态数据状态::来源未找到;
        case 特征类型准确值核验状态::正式特征类型已退出:case 特征类型准确值核验状态::准确值已退出:return 动态数据状态::来源已退出;
        case 特征类型准确值核验状态::准确值不相容:return 动态数据状态::比较依据不支持;
        case 特征类型准确值核验状态::历史材料不可用:return 动态数据状态::历史材料已清理;
        case 特征类型准确值核验状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case 特征类型准确值核验状态::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射特征值(特征值读取错误 s)noexcept{switch(s){
        case 特征值读取错误::未找到:return 动态数据状态::来源未找到;
        case 特征值读取错误::材料已清理:return 动态数据状态::历史材料已清理;
        case 特征值读取错误::能力未提供:return 动态数据状态::比较依据不支持;
        case 特征值读取错误::资源失败:return 动态数据状态::资源失败;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射比较绑定(特征I64比较绑定状态 s)noexcept{switch(s){
        case 特征I64比较绑定状态::未找到:case 特征I64比较绑定状态::目标已退出:
        case 特征I64比较绑定状态::格式不支持:case 特征I64比较绑定状态::注册不唯一:return 动态数据状态::比较依据不支持;
        case 特征I64比较绑定状态::历史材料不可用:return 动态数据状态::历史材料已清理;
        case 特征I64比较绑定状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case 特征I64比较绑定状态::资源失败:return 动态数据状态::资源失败;
        case 特征I64比较绑定状态::数量预算不足:return 动态数据状态::数量预算不足;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射来源动态(动态数据状态 s)noexcept{switch(s){
        case 动态数据状态::未找到:return 动态数据状态::来源未找到;
        case 动态数据状态::目标已退出:return 动态数据状态::来源已退出;
        default:return s;}}
    static 动态数据状态 映射存在(存在结构身份只读状态 s)noexcept{switch(s){case 存在结构身份只读状态::未实现:return 动态数据状态::依赖未实现;
        case 存在结构身份只读状态::未找到:return 动态数据状态::来源未找到;case 存在结构身份只读状态::目标已退出:return 动态数据状态::来源已退出;
        case 存在结构身份只读状态::事实代次漂移:return 动态数据状态::事实代次漂移;case 存在结构身份只读状态::历史材料已清理:return 动态数据状态::历史材料已清理;
        case 存在结构身份只读状态::资源失败:return 动态数据状态::资源失败;default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射绑定(状态使用绑定数据状态 s)noexcept{switch(s){case 状态使用绑定数据状态::未实现:return 动态数据状态::依赖未实现;
        case 状态使用绑定数据状态::未找到:case 状态使用绑定数据状态::场景未找到:case 状态使用绑定数据状态::存在未找到:case 状态使用绑定数据状态::状态未找到:return 动态数据状态::来源未找到;
        case 状态使用绑定数据状态::目标已退出:case 状态使用绑定数据状态::场景已退出:case 状态使用绑定数据状态::存在已退出:case 状态使用绑定数据状态::状态已退出:return 动态数据状态::来源已退出;
        case 状态使用绑定数据状态::事实代次漂移:return 动态数据状态::事实代次漂移;case 状态使用绑定数据状态::历史材料已清理:return 动态数据状态::历史材料已清理;
        case 状态使用绑定数据状态::资源失败:return 动态数据状态::资源失败;default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射状态(状态类数据状态 s)noexcept{switch(s){case 状态类数据状态::未找到:return 动态数据状态::来源未找到;case 状态类数据状态::目标已退出:return 动态数据状态::来源已退出;
        case 状态类数据状态::事实代次漂移:return 动态数据状态::事实代次漂移;case 状态类数据状态::历史材料已清理:return 动态数据状态::历史材料已清理;
        case 状态类数据状态::资源失败:return 动态数据状态::资源失败;default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射读取(L1所有者范围读取状态 s)noexcept{switch(s){case L1所有者范围读取状态::未找到:return 动态数据状态::未找到;
        case L1所有者范围读取状态::已退出:return 动态数据状态::目标已退出;case L1所有者范围读取状态::历史材料已清理:return 动态数据状态::历史材料已清理;
        case L1所有者范围读取状态::事实代次漂移:return 动态数据状态::事实代次漂移;case L1所有者范围读取状态::资源失败:return 动态数据状态::资源失败;
        case L1所有者范围读取状态::入口拒绝:return 动态数据状态::入口拒绝;default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射写入(L1所有者范围写入状态 s,动态数据状态 ok)noexcept{switch(s){case L1所有者范围写入状态::成功:return ok;
        case L1所有者范围写入状态::精确重复:return 动态数据状态::精确重复;case L1所有者范围写入状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case L1所有者范围写入状态::幂等冲突:return 动态数据状态::幂等冲突;case L1所有者范围写入状态::引用冲突:return 动态数据状态::引用冲突;
        case L1所有者范围写入状态::资源失败:return 动态数据状态::资源失败;case L1所有者范围写入状态::内部不一致:return 动态数据状态::已可能发布;
        default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射值组(L1所有者范围属性类型当前值组读取状态 s)noexcept{switch(s){case L1所有者范围属性类型当前值组读取状态::成功:return 动态数据状态::已读取;
        case L1所有者范围属性类型当前值组读取状态::数量预算不足:return 动态数据状态::数量预算不足;case L1所有者范围属性类型当前值组读取状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case L1所有者范围属性类型当前值组读取状态::资源失败:return 动态数据状态::资源失败;default:return 动态数据状态::内部不一致;}}
    static 动态数据状态 映射清理(L1物理清理状态 s)noexcept{switch(s){case L1物理清理状态::已清理:return 动态数据状态::已清理;
        case L1物理清理状态::精确重复:return 动态数据状态::精确重复;case L1物理清理状态::事实代次漂移:return 动态数据状态::事实代次漂移;
        case L1物理清理状态::引用冲突:return 动态数据状态::引用冲突;case L1物理清理状态::同键冲突:return 动态数据状态::幂等冲突;
        case L1物理清理状态::资源失败:return 动态数据状态::资源失败;default:return 动态数据状态::内部不一致;}}

    L1事实基座服务& l1_;L1所有者范围写端口 port_;L1结构所有者身份 owner_{};动态结构交付 layout_;
    const 存在结构身份只读提供者& existence_;const 状态使用绑定只读提供者& binding_;const 状态类数据服务& state_;
    const 特征类数据服务& feature_;const 特征值类数据服务& value_;动态保留策略 policy_;
};

} // namespace 海中鱼巣
