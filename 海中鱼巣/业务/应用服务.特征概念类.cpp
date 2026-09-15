#include "应用服务.特征概念类.h"

特征概念事实 特征概念应用服务::确保概念(const 特征概念定义& definition,
        const std::variant<特征类型身份,稳定编码,std::vector<概念树概念身份>>& source,
        const 特征概念预算& budget) {
        const auto head=写头(budget);
        const auto g=head.期望事实代次;
        auto all=图(definition.类型,g,g,budget);std::optional<特征概念事实> same;
        for(const auto& f:all)if(定义同义(f.定义,definition)) {
            if(same)throw 失败{D::内部不一致};
            if(f.治理状态==概念树生命周期状态::退役)throw 失败{D::引用冲突};
            same=f;
        }
        if(same){confirmed_.概念=same;return *same;}
        const auto full=特征结果(features_.读取I64类型完整域({1,g,g,definition.类型})).数据;
        std::vector<概念树概念身份> parents;std::vector<特征概念父组变更> changes;
        if(域同义(definition.域,full))parents.push_back(root_);
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
            if(定义同义(child.定义,definition)||!包含(definition,child.定义,g,g))continue;
            bool maximal=true;
            for(const auto& middle:all)
                if(middle.身份!=child.身份&&!定义同义(middle.定义,definition)
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

特征概念处理结果 特征概念应用服务::推进(const 特征概念观察请求& input,bool resume,const 定位特征结果* saved) {
        std::scoped_lock lock(mutex_);
        auto error=[&](std::variant<特征数据错误,D> reason) {
            confirmed_.原因=reason;
            confirmed_.待确认请求=pending_;
            if(confirmed_.定位结果&&confirmed_.定位结果->成功()) {
                confirmed_.状态=B::准确F已定位概念未完成;return confirmed_;
            }
            if(pending_location_) {confirmed_.状态=B::定位待原请求确认;return confirmed_;}
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
            const bool same=pending_input_&&*pending_input_==input;
            if((pending_||pending_location_)&&!same)return error(D::前次写入待收敛);
            if(!same){confirmed_={};pending_input_=input;}
            if(!有效(input.观察.类型)||!有效(input.观察.来源)||!input.观察.序号||!input.最大保留观察数
                ||!浅层结构有效(input.准确值)||!定位输入相同(input))throw 失败{D::入口拒绝};
            if(saved){核验定位回执(input,*saved);confirmed_.定位结果=*saved;pending_location_=false;}
            if(!(confirmed_.定位结果&&confirmed_.定位结果->成功())) {
                auto located=location_.创建或复用(input.定位);
                confirmed_.定位结果=std::move(located);const auto& lr=*confirmed_.定位结果;
                pending_location_=lr.状态==定位特征状态::已可能发布;
                if(!lr.成功()) {
                    confirmed_.观察.reset();confirmed_.概念.reset();confirmed_.名称.reset();confirmed_.准确匹配.clear();
                    confirmed_.定位原因=lr.状态;
                    if(lr.状态==定位特征状态::需选择F){for(auto id:lr.候选F)confirmed_.准确匹配.push_back({id});confirmed_.状态=B::需选择准确F;}
                    else if(pending_location_)confirmed_.状态=B::定位待原请求确认;
                    else if(resume&&lr.状态==定位特征状态::事实代次漂移&&!same&&!saved)confirmed_.状态=B::材料缺失;
                    else confirmed_.状态=B::定位拒绝;
                    return confirmed_;
                }
            }else 核验定位回执(input,*confirmed_.定位结果);
            confirmed_.定位原因.reset();
            const 特征信息身份 actual{confirmed_.定位结果->事实->F};confirmed_.准确匹配={actual};
            if(pending_){if(!resume)return error(D::前次写入待收敛);(void)消费待确认写入();}
            const auto 类型=特征结果(features_.读取先天I64特征类型(input.观察.类型));
            if(!confirmed_.观察) {
                const auto g=当前();const auto existing=概念结果(concepts_.读取类型观察({{1,g,g},input.观察,input.预算}),g,g);
                if(existing.size()>1)throw 失败{D::内部不一致};
                if(!existing.empty())confirmed_.观察=existing.front();
            }
            if(confirmed_.观察&&(!观察键同义(confirmed_.观察->输入.键,input.观察)||confirmed_.观察->输入.F!=actual||confirmed_.观察->输入.时间!=input.时间))throw 失败{D::幂等冲突};
            if(!confirmed_.观察) {
                const auto saved=写(类型观察登记请求{写头(input.预算),{input.观察,actual,input.时间},
                    input.最大保留观察数,input.预算});
                if(!saved.观察)throw 失败{D::内部不一致};
            }
            const auto observed=*confirmed_.观察;
            if(!观察键同义(observed.输入.键,input.观察)||observed.输入.F!=actual||observed.输入.时间!=input.时间)
                throw 失败{D::内部不一致};
            const auto g=当前();
            const auto full=特征结果(features_.读取I64类型完整域({1,g,g,input.观察.类型})).数据;
            auto h=g;auto all=图(input.观察.类型,h,h,input.预算);
            auto 最细适用=[&] {
                std::vector<特征概念事实> 结果;
                for(const auto& f:all) {
                    if(域同义(f.定义.域,full)||f.治理状态==概念树生命周期状态::退役||!命中(f,actual,h,h))continue;
                    bool 最细=true;
                    for(const auto& d:all)
                        if(d.身份!=f.身份&&d.治理状态!=概念树生命周期状态::退役&&!定义同义(d.定义,f.定义)
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
                if(!应形成&&!域同义(formed.域,full))
                    for(const auto& 上位:all)
                        if(上位.治理状态!=概念树生命周期状态::退役&&!域同义(上位.定义.域,formed.域)
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
        if(!观察键同义(o.输入.键,r.观察)||o.输入.时间!=r.时间||(r.指定F&&*r.指定F!=f))
            throw 失败{特征数据错误::幂等冲突};
        const 特征信息* matched=nullptr;
        for(const auto& candidate:exact)if(candidate.身份==f){
            if(matched)throw 失败{D::内部不一致};matched=&candidate;
        }
        if(!matched)throw 失败{特征数据错误::幂等冲突};
        if(!浅层结构有效(*matched)||matched->类型!=r.观察.类型)throw 失败{D::内部不一致};
        if(!定位输入相同(r))throw 失败{D::入口拒绝};
        定位特征读取请求 locationRequest{1,g,g,f.编码,r.定位.预算};auto located=location_.读取定位特征(locationRequest);
        if(!located.成功(locationRequest)||located.事实->位置!=r.定位.位置||located.事实->正式特征类型!=r.定位.正式特征类型) {
            out.状态=B::定位拒绝;out.定位原因=located.状态;return out;
        }
        定位特征结果 receipt;receipt.状态=定位特征状态::已复用;receipt.Gread=g;receipt.事实=located.事实;receipt.原请求=r.定位;
        if(!receipt.成功())throw 失败{D::内部不一致};out.定位结果=std::move(receipt);
        out.准确匹配={f};out.观察=o;
        const auto name=概念结果(concepts_.读取特征当前名称({2,{1,g,g},f,base}),g,g);
        if(!name){
            守卫(g);out.状态=B::准确F已定位概念未完成;
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
                ||(actual.退出G&&*actual.退出G<=g)||!域同义(check.域,c->定义.域)
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
