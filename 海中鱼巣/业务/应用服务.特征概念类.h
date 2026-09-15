#pragma once

#include "../领域/数据服务.概念树类.h"
#include "../领域/数据服务.定位特征.h"
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

namespace 海中鱼巣 {
struct 特征概念观察请求 final {
    类型观察键 观察;特征准确值 准确值;std::int64_t 时间=0;
    std::optional<特征信息身份> 指定F;std::uint64_t 最大保留观察数=0;特征概念预算 预算;
    定位特征创建请求 定位;
    friend bool operator==(const 特征概念观察请求&,const 特征概念观察请求&)=default;
};
enum class 特征概念处理状态 : std::uint8_t {
    完成,需选择准确F,准确F已成立名称未完成,待原请求确认,
    规则缺失,材料缺失,旧格式不支持,预算不足,入口拒绝,资源失败,内部不一致,
    定位拒绝=11,定位待原请求确认=12,准确F已定位概念未完成=13
};
struct 特征概念处理结果 final {
    特征概念处理状态 状态=特征概念处理状态::入口拒绝;
    std::vector<特征信息身份> 准确匹配;std::optional<类型观察事实> 观察;
    std::optional<特征概念事实> 概念;std::optional<特征名称事实> 名称;
    std::optional<特征概念写请求> 待确认请求;
    std::optional<std::variant<特征数据错误,概念树数据状态>> 原因;
    std::optional<定位特征结果> 定位结果;
    std::optional<定位特征状态> 定位原因;
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
    特征类数据服务& features_;概念树类数据服务& concepts_;定位特征数据服务& location_;
    const 概念树概念身份 root_;
    std::mutex mutex_;
    std::optional<特征概念观察请求> pending_input_;
    std::optional<特征概念写请求> pending_;
    bool pending_location_=false;
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
    static bool 域同义(const 特征规范I64域& a,const 特征规范I64域& b) noexcept {
        if(a.区间.size()!=b.区间.size())return false;
        for(std::size_t i=0;i<a.区间.size();++i)
            if(a.区间[i].下界!=b.区间[i].下界||a.区间[i].上界!=b.区间[i].上界)return false;
        return true;
    }
    static bool 观察键同义(const 类型观察键& a,const 类型观察键& b) noexcept {
        return a.类型.编码.值==b.类型.编码.值&&a.来源.值==b.来源.值&&a.序号==b.序号;
    }
    static bool 定义同义(const 特征概念定义& a,const 特征概念定义& b) noexcept {
        return a.类型.编码.值==b.类型.编码.值&&域同义(a.域,b.域);
    }
    特征概念事实 确保概念(const 特征概念定义& definition,
        const std::variant<特征类型身份,稳定编码,std::vector<概念树概念身份>>& source,
        const 特征概念预算& budget);
    static bool 定位输入相同(const 特征概念观察请求& r) {
        return r.观察.类型.编码==r.定位.正式特征类型&&r.准确值==r.定位.准确值
            &&(!r.指定F ? !r.定位.指定F : r.定位.指定F&&r.指定F->编码==*r.定位.指定F);
    }
    void 核验定位回执(const 特征概念观察请求& r,const 定位特征结果& saved) const {
        if(!定位输入相同(r)||saved.原请求!=r.定位||!saved.成功())throw 失败{D::入口拒绝};
        const auto g=当前();定位特征读取请求 q{1,g,saved.Gread,saved.事实->F,r.定位.预算};
        auto read=location_.读取定位特征(q);
        if(!read.成功(q)||read.事实!=saved.事实)throw 失败{D::历史材料不可用};
        守卫(g);
    }
    特征概念处理结果 推进(const 特征概念观察请求& input,bool resume,const 定位特征结果* saved=nullptr);
public:
    特征概念应用服务(特征类数据服务& f,概念树类数据服务& c,定位特征数据服务& located):features_(f),concepts_(c),location_(located),root_(c.特征根引用()) {
        if(!有效(root_.值)||!c.共享应用已启用()||!c.使用特征服务(f)||!located.使用内容参与者(f))throw std::invalid_argument("feature concept providers");
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
    特征概念处理结果 收敛类型观察(const 特征概念观察请求& r,const 定位特征结果& saved){return 推进(r,true,&saved);}
    bool 使用定位参与者(const 定位特征已知参与者& e,const 定位特征组织参与者& scene) const noexcept {
        return location_.使用已知参与者(e)&&location_.使用组织参与者(scene);
    }
    特征概念处理结果 读取已发布类型观察(const 特征概念观察请求&,std::uint64_t Gread) const;
    概念树应用读取结果<std::vector<特征概念事实>> 查找适用特征概念(const 特征概念应用读取请求& r) const;
    概念树应用读取结果<特征命名视图> 读取命名特征(const 特征概念应用读取请求& r) const;
};
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
