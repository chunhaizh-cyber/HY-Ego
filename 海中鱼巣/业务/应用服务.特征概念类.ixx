module;
#include <algorithm>
#include <cstdint>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.业务.应用服务.特征概念类;
export import 海中鱼巣.领域.数据服务.概念树类;

export namespace 海中鱼巣 {
struct 特征概念观察请求 final {
    类型观察键 观察;特征准确值 准确值;std::int64_t 时间=0;
    std::optional<特征信息身份> 指定F;std::uint64_t 最大保留观察数=0;特征概念预算 预算;
    friend bool operator==(const 特征概念观察请求&,const 特征概念观察请求&)=default;
};
enum class 特征概念处理状态 : std::uint8_t {
    完成,需选择准确F,准确F已成立名称未完成,待原请求确认,
    规则缺失,材料缺失,旧格式不支持,预算不足,入口拒绝,资源失败,内部不一致
};
struct 特征概念处理结果 final {
    特征概念处理状态 状态=特征概念处理状态::入口拒绝;
    std::vector<特征信息身份> 准确匹配;std::optional<类型观察事实> 观察;
    std::optional<特征概念事实> 概念;std::optional<特征名称事实> 名称;
    std::optional<特征概念写请求> 待确认请求;
    std::optional<std::variant<特征数据错误,概念树数据状态>> 原因;
};
struct 特征命名视图 final {
    特征信息 特征;std::optional<概念树概念身份> 名称;
    friend bool operator==(const 特征命名视图&,const 特征命名视图&)=default;
};
struct 特征概念应用读取请求 final {准确特征读取请求 特征;特征概念预算 预算;};
class 特征概念应用服务 final {
    using D=概念树数据状态;using B=特征概念处理状态;
    static D 数据错误映射(const std::variant<特征数据错误,D>& reason) noexcept {
        if(const auto* data=std::get_if<D>(&reason))return *data;
        switch(std::get<特征数据错误>(reason)) {
        case 特征数据错误::未找到:return D::未找到;
        case 特征数据错误::未设置:case 特征数据错误::规则缺失:return D::规则缺失;
        case 特征数据错误::入口拒绝:return D::入口拒绝;
        case 特征数据错误::类型不相容:return D::类型不相容;
        case 特征数据错误::能力未提供:return D::不支持;
        case 特征数据错误::旧格式不支持:return D::旧格式不支持;
        case 特征数据错误::已退出:return D::目标已退出;
        case 特征数据错误::历史材料不可用:return D::历史材料不可用;
        case 特征数据错误::引用冲突:return D::引用冲突;
        case 特征数据错误::算术不可表示:return D::差异不可表示;
        case 特征数据错误::并发变化:return D::事实代次漂移;
        case 特征数据错误::资源失败:return D::资源失败;
        case 特征数据错误::幂等冲突:return D::幂等冲突;
        case 特征数据错误::数量预算不足:return D::数量预算不足;
        case 特征数据错误::前次写入待收敛:return D::前次写入待收敛;
        default:return D::内部不一致;
        }
    }
    struct 失败 {std::variant<特征数据错误,D> 原因;};
    特征类数据服务& features_;概念树类数据服务& concepts_;
    const 概念树概念身份 root_;
    std::mutex mutex_;
    std::optional<特征概念观察请求> pending_input_;
    std::optional<特征概念写请求> pending_;
    bool pending_feature_=false;
    特征概念处理结果 confirmed_;
    inline static std::mutex root_mutex_;
    inline static const 概念树类数据服务* root_provider_=nullptr;
    inline static 概念树概念身份 root_reference_;
    inline static std::size_t root_count_=0;

    template<class T> static T 特征结果(特征数据结果<T> value) {
        if(const auto* e=std::get_if<特征数据错误>(&value))throw 失败{*e};
        return std::get<T>(std::move(value));
    }
    template<class T> static T 概念结果(概念树应用读取结果<T> value,std::uint64_t g,std::uint64_t h) {
        if(!value.成功())throw 失败{value.状态};
        if(value.Gread!=g||value.H!=h)throw 失败{D::内部不一致};
        return std::move(*value.数据);
    }
    std::uint64_t 当前() const {
        const auto r=concepts_.读取当前事实代次();if(!r.成功())throw 失败{r.状态};return r.Gread;
    }
    void 守卫(std::uint64_t g) const {if(当前()!=g)throw 失败{D::事实代次漂移};}
    概念树写入头 写头(const 特征概念预算& b) const {
        auto r=concepts_.取得特征概念写入头(b);
        if(!r.成功()||!r.数据)throw 失败{r.状态};
        if(r.Gread!=r.H||r.数据->期望事实代次!=r.Gread||!r.数据->幂等身份)throw 失败{D::内部不一致};
        return *r.数据;
    }
    std::vector<特征概念事实> 图(特征类型身份 type,std::uint64_t g,std::uint64_t h,const 特征概念预算& b) const {
        return 概念结果(concepts_.查找特征概念({{1,g,h},type,b}),g,h);
    }
    bool 包含(const 特征概念定义& a,const 特征概念定义& b,std::uint64_t g,std::uint64_t h) const {
        if(a.类型!=b.类型)return false;
        const auto r=特征结果(features_.判定I64域包含({{1,g,h,a.类型},a.域,b.域}));
        if(r.Gread!=g||r.H!=h)throw 失败{D::内部不一致};return r.数据;
    }
    bool 命中(const 特征概念事实& c,特征信息身份 f,std::uint64_t g,std::uint64_t h) const {
        const auto r=特征结果(features_.判定准确特征命中域({{1,g,h,f},c.定义.域}));
        if(r.Gread!=g||r.H!=h)throw 失败{D::内部不一致};return r.数据;
    }
    void 接受(const 特征概念写入结果& r) {
        if(r.观察)confirmed_.观察=r.观察;
        if(r.概念)confirmed_.概念=r.概念;
        if(r.名称)confirmed_.名称=r.名称;
    }
    特征概念写入结果 消费待确认写入() {
        if(!pending_)throw 失败{D::内部不一致};
        auto r=concepts_.收敛特征概念写入(*pending_);
        接受(r);
        if(!r.成功()) {
            if(r.发布状态!=概念树发布状态::可能已发布
                &&r.状态!=D::已可能发布&&r.状态!=D::前次写入待收敛)pending_.reset();
            throw 失败{r.状态};
        }
        pending_.reset();return r;
    }
    特征概念写入结果 写(特征概念写请求 request) {
        if(pending_)throw 失败{D::前次写入待收敛};
        pending_=std::move(request);
        return 消费待确认写入();
    }
    特征概念事实 确保概念(const 特征概念定义& definition,
        const std::variant<特征类型身份,稳定编码,std::vector<概念树概念身份>>& source,
        const 特征概念预算& budget) {
        const auto head=写头(budget);
        const auto g=head.期望事实代次;
        auto all=图(definition.类型,g,g,budget);std::optional<特征概念事实> same;
        for(const auto& f:all)if(f.定义==definition) {
            if(same)throw 失败{D::内部不一致};
            if(f.治理状态==概念树生命周期状态::退役)throw 失败{D::引用冲突};
            same=f;
        }
        if(same){confirmed_.概念=same;return *same;}
        const auto full=特征结果(features_.读取I64类型完整域({1,g,g,definition.类型})).数据;
        std::vector<概念树概念身份> parents;std::vector<特征概念父组变更> changes;
        if(definition.域==full)parents.push_back(root_);
        else {
            for(const auto& candidate:all) {
                if(candidate.治理状态==概念树生命周期状态::退役||!包含(candidate.定义,definition,g,g))continue;
                bool minimal=true;
                for(const auto& inner:all)
                    if(inner.身份!=candidate.身份&&inner.治理状态!=概念树生命周期状态::退役
                        &&包含(candidate.定义,inner.定义,g,g)&&包含(inner.定义,definition,g,g)){minimal=false;break;}
                if(minimal)parents.push_back(candidate.身份);
            }
            if(parents.empty())throw 失败{D::引用冲突};
        }
        for(const auto& child:all) {
            if(child.定义==definition||!包含(definition,child.定义,g,g))continue;
            bool maximal=true;
            for(const auto& middle:all)
                if(middle.身份!=child.身份&&middle.定义!=definition
                    &&包含(definition,middle.定义,g,g)&&包含(middle.定义,child.定义,g,g)){maximal=false;break;}
            if(!maximal)continue;
            特征概念父组变更 change{child.身份,child.直接上位,{}};
            for(const auto& edge:child.直接上位) {
                const auto p=std::find_if(all.begin(),all.end(),[&](const auto& f){return f.身份==edge.上位;});
                if(p==all.end())throw 失败{D::内部不一致};
                if(!包含(p->定义,definition,g,g))change.新父组.push_back(p->身份);
            }
            changes.push_back(std::move(change));
        }
        const auto saved=写(特征概念建立请求{head,definition,source,parents,changes,budget});
        if(!saved.概念)throw 失败{D::内部不一致};
        return *saved.概念;
    }
    特征概念处理结果 推进(const 特征概念观察请求& input,bool resume) {
        std::scoped_lock lock(mutex_);
        auto error=[&](std::variant<特征数据错误,D> reason) {
            confirmed_.原因=reason;
            confirmed_.待确认请求=pending_;
            if(pending_||pending_feature_) {
                confirmed_.状态=B::待原请求确认;return confirmed_;
            }
            if(!confirmed_.准确匹配.empty()){confirmed_.状态=B::准确F已成立名称未完成;return confirmed_;}
            confirmed_.状态=std::visit([](auto e) {
                using T=std::decay_t<decltype(e)>;
                if constexpr(std::is_same_v<T,特征数据错误>) {
                    switch(e) {
                    case 特征数据错误::规则缺失:case 特征数据错误::未设置:return B::规则缺失;
                    case 特征数据错误::未找到:case 特征数据错误::历史材料不可用:return B::材料缺失;
                    case 特征数据错误::旧格式不支持:return B::旧格式不支持;
                    case 特征数据错误::资源失败:return B::资源失败;
                    case 特征数据错误::数量预算不足:return B::预算不足;
                    case 特征数据错误::内部不一致:return B::内部不一致;
                    default:return B::入口拒绝;
                    }
                } else {
                    switch(e) {
                    case D::规则缺失:return B::规则缺失;
                    case D::未找到:case D::历史材料不可用:return B::材料缺失;
                    case D::旧格式不支持:return B::旧格式不支持;
                    case D::数量预算不足:return B::预算不足;
                    case D::资源失败:return B::资源失败;
                    case D::内部不一致:return B::内部不一致;
                    default:return B::入口拒绝;
                    }
                }
            },reason);return confirmed_;
        };
        try {
            const bool 本实例待确认=pending_.has_value()||pending_feature_;
            const bool 同请求已确认=pending_input_&&*pending_input_==input
                &&(!confirmed_.准确匹配.empty()||confirmed_.观察.has_value());
            std::optional<特征信息身份> 原已确认F;
            if(同请求已确认&&confirmed_.准确匹配.size()==1)原已确认F=confirmed_.准确匹配.front();
            if((pending_||pending_feature_)&&(!pending_input_||*pending_input_!=input))
                return error(D::前次写入待收敛);
            if(!pending_&&!pending_feature_&&!同请求已确认) {confirmed_={};pending_input_=input;}
            if(!有效(input.观察.类型)||!有效(input.观察.来源)||!input.观察.序号||!input.最大保留观察数
                ||!浅层结构有效(input.准确值))throw 失败{特征数据错误::入口拒绝};
            if(pending_||pending_feature_) {
                if(!resume)return error(D::前次写入待收敛);
                if(pending_feature_) {
                    const auto recovered=features_.收敛待确认写入();
                    if(const auto* e=std::get_if<特征数据错误>(&recovered))
                        pending_feature_=*e==特征数据错误::发布结果未确认||*e==特征数据错误::前次写入待收敛;
                    else pending_feature_=false;
                    特征结果(recovered);
                }
                if(pending_) {
                    (void)消费待确认写入();
                }
            }
            const auto 类型=特征结果(features_.读取先天I64特征类型(input.观察.类型));
            // 原观察键已经绑定 F 时沿该持久事实续办；后续新增同值 F 不重开选择。
            if(!confirmed_.观察) {
                const auto observedG=当前();
                const auto existing=概念结果(concepts_.读取类型观察({{1,observedG,observedG},input.观察,input.预算}),observedG,observedG);
                if(existing.size()>1)throw 失败{D::内部不一致};
                if(!existing.empty())confirmed_.观察=existing.front();
                if(resume&&!本实例待确认&&!原已确认F&&existing.empty())throw 失败{D::不支持};
            }
            const auto exact=特征结果(features_.查询准确特征(input.观察.类型,input.准确值));
            confirmed_.准确匹配.clear();
            for(const auto& f:exact)confirmed_.准确匹配.push_back(f.身份);
            特征信息身份 actual;
            if(confirmed_.观察) {
                const auto& original=*confirmed_.观察;
                if(original.输入.键!=input.观察||original.输入.时间!=input.时间
                    ||(input.指定F&&*input.指定F!=original.输入.F)
                    ||std::find(confirmed_.准确匹配.begin(),confirmed_.准确匹配.end(),original.输入.F)==confirmed_.准确匹配.end())
                    throw 失败{D::引用冲突};
                actual=original.输入.F;
            } else if(原已确认F) {
                if((input.指定F&&*input.指定F!=*原已确认F)
                    ||std::find(confirmed_.准确匹配.begin(),confirmed_.准确匹配.end(),*原已确认F)==confirmed_.准确匹配.end())
                    throw 失败{特征数据错误::引用冲突};
                actual=*原已确认F;
            } else if(input.指定F) {
                if(std::find(confirmed_.准确匹配.begin(),confirmed_.准确匹配.end(),*input.指定F)==confirmed_.准确匹配.end())
                    throw 失败{特征数据错误::引用冲突};
                actual=*input.指定F;
            } else if(exact.size()>1) {
                confirmed_.状态=B::需选择准确F;confirmed_.原因.reset();return confirmed_;
            } else if(exact.size()==1)actual=exact.front().身份;
            else {
                auto created=features_.创建准确特征(input.观察.类型,input.准确值);
                if(const auto* e=std::get_if<特征数据错误>(&created)) {
                    pending_feature_=*e==特征数据错误::发布结果未确认||*e==特征数据错误::前次写入待收敛;
                    throw 失败{*e};
                }
                actual=std::get<特征信息身份>(created);confirmed_.准确匹配={actual};
            }
            confirmed_.准确匹配={actual};
            if(!confirmed_.观察) {
                const auto saved=写(类型观察登记请求{写头(input.预算),{input.观察,actual,input.时间},
                    input.最大保留观察数,input.预算});
                if(!saved.观察)throw 失败{D::内部不一致};
            }
            const auto observed=*confirmed_.观察;
            if(observed.输入.键!=input.观察||observed.输入.F!=actual||observed.输入.时间!=input.时间)
                throw 失败{D::内部不一致};
            const auto g=当前();
            const auto full=特征结果(features_.读取I64类型完整域({1,g,g,input.观察.类型})).数据;
            auto h=g;auto all=图(input.观察.类型,h,h,input.预算);
            auto 最细适用=[&] {
                std::vector<特征概念事实> 结果;
                for(const auto& f:all) {
                    if(f.定义.域==full||f.治理状态==概念树生命周期状态::退役||!命中(f,actual,h,h))continue;
                    bool 最细=true;
                    for(const auto& d:all)
                        if(d.身份!=f.身份&&d.治理状态!=概念树生命周期状态::退役&&d.定义!=f.定义
                            &&包含(f.定义,d.定义,h,h)&&命中(d,actual,h,h)){最细=false;break;}
                    if(最细)结果.push_back(f);
                }
                return 结果;
            };
            auto finest=最细适用();
            特征概念事实 selected;
            // 既有域匹配不以新域形成规则为前置；规则完整时才核对独立新细域。
            if(finest.empty()||类型.规格.域形成) {
                const auto formed=特征结果(features_.形成I64特征域({1,h,h,actual}));
                bool 应形成=finest.empty();
                if(!应形成&&formed.域!=full)
                    for(const auto& 上位:all)
                        if(上位.治理状态!=概念树生命周期状态::退役&&上位.定义.域!=formed.域
                            &&包含(上位.定义,{input.观察.类型,formed.域},h,h)){应形成=true;break;}
                if(应形成) {
                    selected=确保概念({input.观察.类型,full},input.观察.类型,input.预算);
                    selected=确保概念({input.观察.类型,formed.域},observed.记录,input.预算);
                    h=当前();all=图(input.观察.类型,h,h,input.预算);finest=最细适用();
                }
            }
            if(finest.size()==1)selected=finest.front();
            else if(finest.size()>1) {
                特征规范I64域 combined;std::vector<概念树概念身份> why;
                for(const auto& f:finest) {
                    combined.区间.insert(combined.区间.end(),f.定义.域.区间.begin(),f.定义.域.区间.end());
                    why.push_back(f.身份);
                }
                auto d=特征结果(features_.规范化I64特征域({{1,h,h,input.观察.类型},combined})).数据;
                selected=确保概念({input.观察.类型,d},why,input.预算);
            }
            confirmed_.概念=selected;
            const auto hitHead=写头(input.预算);h=hitHead.期望事实代次;
            all=图(input.观察.类型,h,h,input.预算);std::vector<概念树概念身份> hits;
            for(const auto& f:all)
                if(f.治理状态!=概念树生命周期状态::退役&&命中(f,actual,h,h))hits.push_back(f.身份);
            (void)写(特征概念命中请求{hitHead,observed.记录,hits,input.预算});
            const auto nameHead=写头(input.预算);h=nameHead.期望事实代次;
            const auto name=概念结果(concepts_.读取特征当前名称({2,{1,h,h},actual,input.预算.基础}),h,h);
            const auto named=写(特征名称变更请求{nameHead,actual,name,selected.身份,input.预算});
            if(!named.名称||named.名称->F!=actual||named.名称->C!=selected.身份)throw 失败{D::内部不一致};
            confirmed_.名称=named.名称;confirmed_.状态=B::完成;confirmed_.原因.reset();confirmed_.待确认请求.reset();
            pending_input_.reset();return confirmed_;
        } catch(const 失败& e){return error(e.原因);}
        catch(const std::bad_alloc&){return error(D::资源失败);}
        catch(const std::length_error&){return error(D::资源失败);}
        catch(...){return error(D::内部不一致);}
    }
public:
    特征概念应用服务(特征类数据服务& f,概念树类数据服务& c):features_(f),concepts_(c),root_(c.特征根引用()) {
        if(!有效(root_.值)||!c.共享应用已启用()||!c.使用特征服务(f))throw std::invalid_argument("feature concept providers");
        const auto g=当前();概念树预算 b;
        b.最大概念数=b.最大关系数=b.最大来源数=b.最大支持数=b.最大世界成员数=b.最大特征属性数=1;
        b.最大动态槽数=b.最大动态模板数=1;
        const auto root=c.读取概念({{1,g,g},root_,b});
        if(!root.成功()||!root.概念||root.概念->概念!=root_||!root.概念->是本体根
            ||root.概念->根角色!=概念树根角色::特征||root.概念->本体根!=root_||root.概念->定义
            ||有效(root.概念->定义记录)||!root.概念->直接上位组.empty())throw std::invalid_argument("feature concept root");
        守卫(g);std::scoped_lock lock(root_mutex_);
        if(root_count_&&(root_provider_!=&c||root_reference_!=root_))throw std::invalid_argument("feature root provider");
        if(root_count_==std::numeric_limits<std::size_t>::max())throw std::length_error("feature root bindings");
        root_provider_=&c;root_reference_=root_;++root_count_;
    }
    ~特征概念应用服务() noexcept {
        std::scoped_lock lock(root_mutex_);
        if(!root_count_||root_provider_!=&concepts_||root_reference_!=root_)std::terminate();
        if(!--root_count_){root_reference_={};root_provider_=nullptr;}
    }
    特征概念应用服务(const 特征概念应用服务&)=delete;
    特征概念应用服务& operator=(const 特征概念应用服务&)=delete;
    static std::optional<概念树概念身份> 读取特征概念根节点() {
        std::scoped_lock lock(root_mutex_);if(!root_count_)return std::nullopt;return root_reference_;
    }
    bool 使用概念服务(const 概念树类数据服务& c) const noexcept{return &concepts_==&c;}
    特征概念处理结果 处理类型观察(const 特征概念观察请求& r){return 推进(r,false);}
    特征概念处理结果 收敛类型观察(const 特征概念观察请求& r){return 推进(r,true);}
    特征概念处理结果 读取已发布类型观察(const 特征概念观察请求&,std::uint64_t Gread) const;
    概念树应用读取结果<std::vector<特征概念事实>> 查找适用特征概念(const 特征概念应用读取请求& r) const;
    概念树应用读取结果<特征命名视图> 读取命名特征(const 特征概念应用读取请求& r) const;
};
特征概念处理结果 特征概念应用服务::读取已发布类型观察(
    const 特征概念观察请求& r,std::uint64_t g) const {
    特征概念处理结果 out;
    const auto 记录失败=[&](const std::variant<特征数据错误,D>& reason){
        out.原因=reason;
        switch(数据错误映射(reason)){
        case D::规则缺失:out.状态=B::规则缺失;break;
        case D::未找到:case D::目标已退出:case D::历史材料不可用:out.状态=B::材料缺失;break;
        case D::旧格式不支持:out.状态=B::旧格式不支持;break;
        case D::数量预算不足:out.状态=B::预算不足;break;
        case D::资源失败:out.状态=B::资源失败;break;
        case D::内部不一致:case D::差异不可表示:out.状态=B::内部不一致;break;
        default:out.状态=B::入口拒绝;break;
        }
    };
    try {
        const auto& b=r.预算;const auto& base=b.基础;
        if(!g||!有效(r.观察.类型)||!有效(r.观察.来源)||!r.观察.序号
            ||!浅层结构有效(r.准确值)||(r.指定F&&!有效(*r.指定F))||!r.最大保留观察数
            ||!base.最大概念数||!base.最大关系数||!base.最大来源数||!base.最大支持数
            ||!base.最大世界成员数||!base.最大特征属性数||!base.最大动态槽数||!base.最大动态模板数
            ||!b.最大观察数||!b.最大区间数||!b.最大命中数||!b.最大名称数||!b.最大首次材料项数)
            throw 失败{D::入口拒绝};
        守卫(g);
        // 查询负责解引用内容等值；外部材料身份与 F 自有值身份无需相同。
        const auto exact=特征结果(features_.查询准确特征(r.观察.类型,r.准确值));
        const auto observed=概念结果(concepts_.读取类型观察({{1,g,g},r.观察,b}),g,g);
        if(observed.empty())throw 失败{D::未找到};
        if(observed.size()!=1)throw 失败{D::内部不一致};
        const auto& o=observed.front();const auto f=o.输入.F;
        if(!有效(o.记录)||!有效(f)||!o.证据H||o.证据H>g)throw 失败{D::内部不一致};
        if(o.输入.键!=r.观察||o.输入.时间!=r.时间||(r.指定F&&*r.指定F!=f))
            throw 失败{特征数据错误::幂等冲突};
        const 特征信息* matched=nullptr;
        for(const auto& candidate:exact)if(candidate.身份==f){
            if(matched)throw 失败{D::内部不一致};matched=&candidate;
        }
        if(!matched)throw 失败{特征数据错误::幂等冲突};
        if(!浅层结构有效(*matched)||matched->类型!=r.观察.类型)throw 失败{D::内部不一致};
        out.准确匹配={f};out.观察=o;
        const auto name=概念结果(concepts_.读取特征当前名称({2,{1,g,g},f,base}),g,g);
        if(!name){
            守卫(g);out.状态=B::准确F已成立名称未完成;
        }else{
            const auto active=[&](const 概念树生命周期& life){
                return life.创建事实代次&&life.创建事实代次<=g
                    &&(!life.退出事实代次||*life.退出事实代次>g);
            };
            if(name->F!=f||!有效(name->C.值)||!有效(name->关系)||!active(name->生命周期))
                throw 失败{D::引用冲突};
            const 概念树共享预算 budget{base,b,b.最大区间数,base.最大来源数,b.最大名称数,b.最大首次材料项数};
            const auto definition=概念结果(concepts_.读取应用特征模板({2,{1,g,g},name->C,budget}),g,g);
            const auto* c=std::get_if<特征概念事实>(&definition);
            if(!c||c->身份!=name->C||!有效(c->定义记录)||!有效(c->类型关系))throw 失败{D::内部不一致};
            if(c->定义.类型!=r.观察.类型)throw 失败{D::类型不相容};
            if(!active(c->生命周期)||c->治理状态==概念树生命周期状态::退役)throw 失败{D::引用冲突};
            if(c->定义.域.区间.empty())throw 失败{D::规则缺失};
            const auto matchedDomain=概念结果(concepts_.判定应用特征模板({2,{1,g,g},c->身份,{1,g,g,f},budget}),g,g);
            const auto* checkedC=std::get_if<特征概念事实>(&matchedDomain.模板);
            const auto& check=matchedDomain.判定;const auto& actual=check.实际;
            if(!checkedC||*checkedC!=*c||check.Gread!=g||check.模板H!=g||check.实际H!=g
                ||actual.Gread!=g||actual.H!=g||actual.信息!=*matched||!有效(actual.类型关系)
                ||!actual.准确值事实||!有效(*actual.准确值事实)||!actual.创建G||actual.创建G>g
                ||(actual.退出G&&*actual.退出G<=g)||check.域!=c->定义.域
                ||matchedDomain.适用!=check.命中)throw 失败{D::内部不一致};
            if(const auto* scalar=std::get_if<std::int64_t>(&actual.信息.准确值)){
                const auto* full=std::get_if<std::int64_t>(&actual.完整值);
                if(!full||*full!=*scalar)throw 失败{D::内部不一致};
            }else{
                const auto* full=std::get_if<特征值信息>(&actual.完整值);
                if(!full||full->值身份!=std::get<特征值身份>(actual.信息.准确值)
                    ||full->值身份.编码!=*actual.准确值事实||!std::holds_alternative<std::int64_t>(full->值内容))
                    throw 失败{D::内部不一致};
            }
            if(!matchedDomain.适用)throw 失败{D::引用冲突};
            守卫(g);out.概念=*c;out.名称=*name;out.状态=B::完成;
        }
    }catch(const 失败& e){记录失败(e.原因);}
    catch(const std::bad_alloc&){记录失败(D::资源失败);}
    catch(const std::length_error&){记录失败(D::资源失败);}
    catch(...){记录失败(D::内部不一致);}
    // 成功与失败统一在出口守卫；零 G 是非法输入，不尝试用当前代次替代它。
    if(g)try{守卫(g);}
    catch(const 失败& e){out=特征概念处理结果{};记录失败(e.原因);}
    catch(const std::bad_alloc&){out=特征概念处理结果{};记录失败(D::资源失败);}
    catch(const std::length_error&){out=特征概念处理结果{};记录失败(D::资源失败);}
    catch(...){out=特征概念处理结果{};记录失败(D::内部不一致);}
    return out;
}
inline 概念树应用读取结果<std::vector<特征概念事实>> 特征概念应用服务::查找适用特征概念(
    const 特征概念应用读取请求& r) const {
    概念树应用读取结果<std::vector<特征概念事实>> out;out.Gread=r.特征.Gread;out.H=r.特征.H;
    try {
        const auto f=特征结果(features_.读取准确特征事实(r.特征));auto all=图(f.信息.类型,out.Gread,out.H,r.预算);
        std::erase_if(all,[&](const auto& c){return c.治理状态==概念树生命周期状态::退役
            ||!命中(c,f.信息.身份,out.Gread,out.H);});
        守卫(out.Gread);out.数据=std::move(all);out.状态=D::已读取;
    }catch(const 失败& e) {
        out.状态=数据错误映射(e.原因);
    }catch(const std::bad_alloc&){out.状态=D::资源失败;}
    catch(const std::length_error&){out.状态=D::资源失败;}
    catch(...){out.状态=D::内部不一致;}
    return out;
}
inline 概念树应用读取结果<特征命名视图> 特征概念应用服务::读取命名特征(
    const 特征概念应用读取请求& r) const {
    概念树应用读取结果<特征命名视图> out;out.Gread=r.特征.Gread;out.H=r.特征.H;
    try {
        const auto f=特征结果(features_.读取准确特征事实(r.特征));
        const auto name=概念结果(concepts_.读取特征当前名称({2,{1,out.Gread,out.H},r.特征.身份,r.预算.基础}),out.Gread,out.H);
        if(f.Gread!=out.Gread||f.H!=out.H||f.信息.身份!=r.特征.身份||(name&&name->F!=r.特征.身份))
            throw 失败{D::内部不一致};
        out.数据=特征命名视图{f.信息,name?std::optional{name->C}:std::nullopt};守卫(out.Gread);out.状态=D::已读取;
    }catch(const 失败& e){out.状态=数据错误映射(e.原因);}
    catch(const std::bad_alloc&){out.状态=D::资源失败;}
    catch(const std::length_error&){out.状态=D::资源失败;}catch(...){out.状态=D::内部不一致;}
    if(out.状态!=D::已读取)out.数据.reset();return out;
}
} // namespace 海中鱼巣
