module;
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstddef>
#include <exception>
#include <mutex>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
export module 海中鱼巣.业务.应用服务.存在概念树类;
export import 海中鱼巣.业务.应用服务.特征概念类;

export namespace 海中鱼巣 {
struct 存在概念特征见证 final {
    概念树概念身份 模板;
    概念树特征引用 实例;
    稳定编码 值事实{},成员关系{};
    friend bool operator==(const 存在概念特征见证&,const 存在概念特征见证&)=default;
};
struct 存在概念形成请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,H=0;
    概念树存在引用 E;
    std::vector<存在概念特征见证>见证;
    std::optional<概念树概念身份>显式目标;
    std::uint64_t 概念key=0,支持key=0;
    共享特征应用预算 预算;
    std::optional<共享场景限制>场景限制;
    friend bool operator==(const 存在概念形成请求&,const 存在概念形成请求&)=default;
};
struct 存在概念上位请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,H=0;
    概念树概念身份 下位;
    std::vector<概念树概念身份>保留模板;
    std::uint64_t 概念key=0,接边key=0;
    共享特征应用预算 预算;
    friend bool operator==(const 存在概念上位请求&,const 存在概念上位请求&)=default;
};
enum class 存在概念步骤阶段 :std::uint8_t {概念=1,支持,接边};
using 存在概念步骤请求=std::variant<概念树存在建立请求_v2,概念树应用写请求,概念树存在治理请求_v2>;
using 存在概念步骤回执=std::variant<概念树存在写入结果_v2,概念树应用写入结果>;
struct 存在概念步骤 final {
    存在概念步骤阶段 阶段=存在概念步骤阶段::概念;
    存在概念步骤请求 原请求;
    std::optional<std::uint64_t>已知首次H;
    std::optional<存在概念步骤回执>回执提示;
};
struct 存在概念继续材料 final {
    std::uint32_t 版本=1;
    std::variant<存在概念形成请求,存在概念上位请求>原业务;
    std::uint64_t 初始G=0;
    std::optional<概念树存在定义>冻结定义;
    std::optional<概念树概念身份>已定位EC,已定位父;
    std::vector<存在概念步骤>步骤,已确认未绑定旧尝试;
};
struct 存在概念继续请求 final {
    存在概念继续材料 材料;
    std::uint64_t Gread=0;
    共享特征应用预算 本次读取预算;
};
using 存在概念确认事实=std::variant<概念树应用定义事实,概念树支持事实,概念树直接上位事实>;
struct 存在概念操作结果 final {
    std::uint32_t 版本=1;
    共享特征业务状态 业务状态=共享特征业务状态::入口拒绝;
    std::uint64_t Gread=0,H=0;
    共享特征阶段状态 阶段=共享特征阶段状态::未派发;
    bool 可能已发布=false;
    std::vector<存在概念确认事实>已确认;
    std::optional<存在概念继续材料>继续;
    bool 成功()const noexcept;
};
class 存在概念树应用服务 final {
    概念树类数据服务 &concepts_;
    const 特征类数据服务 &features_;
    const 存在类数据服务 &existences_;
    // 仅保存已核验根的进程内引用；根事实仍由数据服务持有。
    const 概念树概念身份 root_;
    inline static 概念树概念身份 存在概念根节点{};
    inline static std::mutex 根节点互斥_;
    inline static const 概念树类数据服务 *根绑定服务_ = nullptr;
    inline static std::size_t 根绑定实例数_ = 0;

    void 绑定全局根() {
        std::lock_guard lock(根节点互斥_);
        if (!根绑定实例数_) {
            if (根绑定服务_ || 有效(存在概念根节点.值))
                throw std::logic_error("inconsistent concept root binding");
            存在概念根节点 = root_;
            根绑定服务_ = &concepts_;
            根绑定实例数_ = 1;
            return;
        }
        if (根绑定服务_ != &concepts_ || 存在概念根节点 != root_)
            throw std::invalid_argument("concept root is bound to another provider");
        if (根绑定实例数_ == std::numeric_limits<std::size_t>::max())
            throw std::length_error("concept root binding count exhausted");
        ++根绑定实例数_;
    }
    void 释放全局根() noexcept {
        std::lock_guard lock(根节点互斥_);
        if (!根绑定实例数_ || 根绑定服务_ != &concepts_ || 存在概念根节点 != root_)
            std::terminate();
        if (--根绑定实例数_ == 0) {
            存在概念根节点 = {};
            根绑定服务_ = nullptr;
        }
    }

    概念树存在定义 形成见证(const 存在概念形成请求&,std::uint64_t,const 概念树共享预算&)const;
    std::optional<概念树存在定义>上位见证(const 存在概念上位请求&,std::uint64_t,const 概念树共享预算&)const;
    概念树应用定义事实 定义(概念树概念身份,std::uint64_t,std::uint64_t,const 概念树共享预算&)const;
    std::optional<概念树应用定义事实>查找(const std::optional<概念树存在定义>&,std::uint64_t,std::uint64_t,const 概念树共享预算&)const;
    void 核验材料(const 存在概念继续材料&,std::uint64_t,const 共享特征应用预算&)const;
    存在概念操作结果 推进(存在概念继续材料,std::uint64_t,const 共享特征应用预算&,const 场景类数据服务*);
public:
    static std::optional<概念树概念身份>读取存在概念根节点(){std::lock_guard lock(根节点互斥_);if(!根绑定实例数_)return std::nullopt;return 存在概念根节点;}
    ~存在概念树应用服务()noexcept{释放全局根();}
    存在概念树应用服务()=delete;
    存在概念树应用服务(const 存在概念树应用服务&)=delete;
    存在概念树应用服务&operator=(const 存在概念树应用服务&)=delete;
    存在概念树应用服务(存在概念树应用服务&&)=delete;
    存在概念树应用服务&operator=(存在概念树应用服务&&)=delete;
    存在概念树应用服务(概念树类数据服务&,const 特征类数据服务&,const 存在类数据服务&);
    bool 使用存在服务(const 存在类数据服务&e)const noexcept{return &e==&existences_;}
    bool 使用概念服务(const 概念树类数据服务&c)const noexcept{return &c==&concepts_;}
    存在概念操作结果 形成或复用存在概念(const 存在概念形成请求&);
    存在概念操作结果 形成上位存在概念(const 存在概念上位请求&);
    存在概念操作结果 继续存在概念操作(const 存在概念继续请求&);
    存在概念操作结果 形成或复用存在概念(const 存在概念形成请求&,const 共享场景限制&,const 场景类数据服务&);
    存在概念操作结果 继续存在概念操作(const 存在概念继续请求&,const 共享场景限制&,const 场景类数据服务&);
    概念树应用读取结果<概念树应用图事实>读取存在概念树(const 概念树应用图读取请求&)const;
    概念树应用写入结果 退出存在支持(const 概念树应用写请求&);
    概念树存在写入结果_v2 迁移存在概念(const 概念树存在治理请求_v2&);
    概念树存在写入结果_v2 退出存在概念(const 概念树存在治理请求_v2&);
    概念树存在写入结果_v2 释放存在抽象依据(const 概念树存在依据释放请求_v2&);
};
} // namespace 海中鱼巣
namespace 海中鱼巣::共享存在应用内部 {
using D=概念树数据状态;using B=共享特征业务状态;
struct 失败 final {D 数据;B 业务;};
inline B 映射(D s)noexcept{
    switch(s){
    case D::事实代次漂移:return B::待继续;
    case D::数量预算不足:return B::预算不足;
    case D::历史材料不可用:return B::历史不可用;
    case D::资源失败:return B::资源失败;
    case D::不支持:return B::不支持;
    case D::未找到:case D::目标已退出:return B::被后继覆盖;
    case D::内部不一致:case D::差异不可表示:return B::内部不一致;
    default:return B::入口拒绝;
    }
}
[[noreturn]] inline void 拒绝(D d=D::入口拒绝){throw 失败{d,映射(d)};}
[[noreturn]] inline void 业务拒绝(B b){throw 失败{D::入口拒绝,b};}
inline std::uint64_t 当前(const 概念树类数据服务&c){auto g=c.读取当前事实代次();if(!g.成功())拒绝(g.状态);return g.Gread;}
inline void 守卫(const 概念树类数据服务&c,std::uint64_t g){if(!g||当前(c)!=g)拒绝(D::事实代次漂移);}
inline bool 合法键(std::uint64_t k)noexcept{return k&&(k&0xFFFF000000000000ULL)!=0x4E43000000000000ULL;}
inline bool 限制相等(const 共享场景限制&a,const 共享场景限制&b)noexcept{return a.S==b.S&&a.E==b.E&&a.原场景H==b.原场景H;}
inline 概念树概念身份 身份(const 概念树应用定义事实&v){return std::visit([](const auto&f){return f.概念;},v);}
inline 概念树生命周期状态 治理(const 概念树应用定义事实&v){return std::visit([](const auto&f){return f.治理状态;},v);}
inline const std::vector<概念树直接上位事实>&父组(const 概念树应用定义事实&v){return std::visit([](const auto&f)->const std::vector<概念树直接上位事实>&{return f.直接上位组;},v);}
inline 存在类结点 存在(const 存在类数据服务&e,概念树存在引用 id,std::uint64_t g,std::uint64_t h,const 概念树共享预算&b){
    auto r=e.读取存在历史事实({1,g,h,id.值,b.基础.最大世界成员数});
    if(!r.成功()){
        switch(r.状态){
        case 特征引用读取状态::未找到:拒绝(D::未找到);
        case 特征引用读取状态::目标已退出:拒绝(D::目标已退出);
        case 特征引用读取状态::历史材料不可用:拒绝(D::历史材料不可用);
        case 特征引用读取状态::事实代次漂移:拒绝(D::事实代次漂移);
        case 特征引用读取状态::数量预算不足:拒绝(D::数量预算不足);
        case 特征引用读取状态::资源失败:拒绝(D::资源失败);
        case 特征引用读取状态::入口拒绝:拒绝();
        default:拒绝(D::内部不一致);
        }
    }
    if(r.Gread!=g||r.H!=h||r.存在->结点!=id.值)拒绝(D::内部不一致);return std::move(*r.存在);
}
inline 特征类结点 特征(const 特征类数据服务&f,概念树特征引用 id,std::uint64_t g,std::uint64_t h,const 概念树共享预算&b){
    auto r=f.按实例读取特征历史事实({1,g,h,id.值,b.基础.最大特征属性数});
    if(!r.成功()){
        switch(r.状态){
        case 特征引用读取状态::未找到:拒绝(D::未找到);
        case 特征引用读取状态::目标已退出:拒绝(D::目标已退出);
        case 特征引用读取状态::历史材料不可用:拒绝(D::历史材料不可用);
        case 特征引用读取状态::事实代次漂移:拒绝(D::事实代次漂移);
        case 特征引用读取状态::数量预算不足:拒绝(D::数量预算不足);
        case 特征引用读取状态::资源失败:拒绝(D::资源失败);
        case 特征引用读取状态::入口拒绝:拒绝();
        default:拒绝(D::内部不一致);
        }
    }
    if(r.Gread!=g||r.H!=h||r.特征->结点!=id.值)拒绝(D::内部不一致);return std::move(*r.特征);
}
inline void 消费(特征长期预算&b,const 特征长期只读用量&u){
    auto take=[](auto&limit,auto cost){if(cost>limit)拒绝(D::数量预算不足);limit-=cost;};
    take(b.最大记录数,u.记录数);take(b.最大关系数,u.关系数);take(b.最大属性数,u.属性数);take(b.最大样本数,u.样本数);
    take(b.最大像素数,u.像素数);take(b.最大边界点对数,u.点对数);take(b.最大历史事实数,u.历史数);
}
inline 概念树写入头 头(const 存在概念步骤请求&q){return std::visit([](const auto&r)->概念树写入头{
    if constexpr(requires{r.写入头;})return r.写入头;else return std::visit([](const auto&w){if constexpr(requires{w.写入头;})return w.写入头;else return w.头;},r.操作);
},q);}
inline 存在概念步骤请求 调整预算(存在概念步骤请求 q,const 概念树共享预算&b){std::visit([&](auto&r){r.预算=b;if constexpr(requires{r.操作;})std::visit([&](auto&w){w.预算=b.基础;},r.操作);},q);return q;}
inline bool 等义(const 存在概念步骤请求&a,const 存在概念步骤请求&b){return 调整预算(a,{})==调整预算(b,{});}
inline void 改G(存在概念步骤请求&q,std::uint64_t g){std::visit([&](auto&r){if constexpr(requires{r.写入头;})r.写入头.期望事实代次=g;else std::visit([&](auto&w){if constexpr(requires{w.写入头;})w.写入头.期望事实代次=g;else w.头.期望事实代次=g;},r.操作);},q);}
inline void 场景检查(const 概念树类数据服务&c,const 存在类数据服务&e,const 场景类数据服务*s,const std::optional<共享场景限制>&limit,概念树存在引用 E,std::uint64_t g,const 概念树共享预算&b){
    if(!limit)return;
    if(!s||limit->E!=E||!有效(limit->S.值)||!limit->原场景H||limit->原场景H>g||!c.使用同一场景基座(*s)||!s->使用存在服务(e))拒绝();
    for(auto h:{limit->原场景H,g}){
        auto r=s->读取场景历史事实({1,g,h,limit->S.值,b.基础.最大世界成员数});
        if(!r.成功()){
            switch(r.状态){
            case 场景类数据状态::未找到:case 场景类数据状态::目标已退出:业务拒绝(B::被后继覆盖);
            case 场景类数据状态::历史材料已清理:拒绝(D::历史材料不可用);
            case 场景类数据状态::事实代次漂移:拒绝(D::事实代次漂移);
            case 场景类数据状态::数量预算不足:拒绝(D::数量预算不足);
            case 场景类数据状态::资源失败:拒绝(D::资源失败);
            case 场景类数据状态::入口拒绝:拒绝();
            default:拒绝(D::内部不一致);
            }
        }
        if(r.Gread!=g||r.H!=h)拒绝(D::内部不一致);
        if(std::count_if(r.场景结点->存在组.begin(),r.场景结点->存在组.end(),[&](const auto&m){return m.目标结点==E.值;})!=1)业务拒绝(B::被后继覆盖);
    }
}
} // namespace 海中鱼巣::共享存在应用内部
namespace 海中鱼巣 {
存在概念树应用服务::存在概念树应用服务(概念树类数据服务&c,const 特征类数据服务&f,const 存在类数据服务&e)
    :concepts_(c),features_(f),existences_(e),root_(c.存在根引用()){
    using namespace 共享存在应用内部;
    if(!有效(root_.值)||!c.共享应用已启用()||!c.使用特征存在服务(f,e))throw std::invalid_argument("shared existence binding");
    const auto g=当前(c);概念树预算 b;b.最大概念数=b.最大关系数=b.最大来源数=b.最大支持数=b.最大世界成员数=b.最大特征属性数=1;
    auto root=c.读取概念({{1,g,g},root_,b});
    if(!root.成功()||root.Gread!=g||root.H!=g||!root.概念||!root.概念->是本体根||root.概念->概念!=root_||root.概念->本体根!=root_||
        root.概念->根角色!=概念树根角色::存在||root.概念->定义||有效(root.概念->定义记录)||!root.概念->直接上位组.empty()||root.概念->治理状态!=概念树生命周期状态::活跃)
        throw std::invalid_argument("shared existence root facts");
    守卫(c,g);绑定全局根();
}
概念树应用定义事实 存在概念树应用服务::定义(概念树概念身份 id,std::uint64_t g,std::uint64_t h,const 概念树共享预算&b)const{
    using namespace 共享存在应用内部;
    auto r=concepts_.读取应用概念定义({1,{1,g,h},id,b});if(!r.成功())拒绝(r.状态);
    if(r.Gread!=g||r.H!=h||身份(*r.数据)!=id)拒绝(D::内部不一致);
    if(const auto*root=std::get_if<概念树概念事实>(&*r.数据)){
        if(id!=root_||!root->是本体根||root->本体根!=root_||root->根角色!=概念树根角色::存在)拒绝(D::引用冲突);
    }else{const auto*ec=std::get_if<概念树应用存在事实>(&*r.数据);if(!ec||ec->本体根!=root_)拒绝(D::引用冲突);}
    return std::move(*r.数据);
}
std::optional<概念树应用定义事实>存在概念树应用服务::查找(const std::optional<概念树存在定义>&definition,std::uint64_t g,std::uint64_t h,const 概念树共享预算&b)const{
    using namespace 共享存在应用内部;
    if(!definition)return 定义(root_,g,h,b);
    auto graph=concepts_.读取应用概念图({1,{{1,g,h},概念树根角色::存在,false,b.基础},b});if(!graph.成功())拒绝(graph.状态);
    if(graph.Gread!=g||graph.H!=h||graph.数据->根身份!=root_)拒绝(D::内部不一致);
    std::optional<概念树应用定义事实>found;
    for(const auto&node:graph.数据->节点组){const auto*ec=std::get_if<概念树应用存在事实>(&node);if(ec&&ec->定义==*definition){if(found)拒绝(D::内部不一致);found=node;}}
    return found;
}
概念树存在定义 存在概念树应用服务::形成见证(const 存在概念形成请求&r,std::uint64_t g,const 概念树共享预算&b)const{
    using namespace 共享存在应用内部;
    if(r.版本!=1||!r.H||r.H>g||!有效(r.E.值)||r.见证.empty()||!合法键(r.概念key)||!合法键(r.支持key)||r.概念key==r.支持key)拒绝();
    if(r.见证.size()>b.基础.最大关系数)拒绝(D::数量预算不足);
    const auto e=存在(existences_,r.E,g,r.H,b);概念树存在定义 out;std::set<std::uint64_t>ids;auto remaining=b;
    for(const auto&w:r.见证){
        if(!有效(w.模板.值)||!有效(w.实例.值)||!有效(w.值事实)||!有效(w.成员关系)||!ids.insert(w.模板.值.值).second)拒绝();
        if(std::count_if(e.特征组.begin(),e.特征组.end(),[&](const auto&m){return m.目标结点==w.实例.值&&m.成员关系==w.成员关系;})!=1)拒绝(D::引用冲突);
        auto f=特征(features_,w.实例,g,r.H,b);if(f.值事实!=w.值事实)拒绝(D::引用冲突);
        auto match=concepts_.判定应用特征模板({1,{1,g,r.H},w.模板,{w.实例.值,w.值事实,f.特征类型,r.H},remaining});if(!match.成功())拒绝(match.状态);
        if(match.Gread!=g||match.H!=r.H)拒绝(D::内部不一致);消费(remaining.特征,match.数据->特征用量);
        if(std::holds_alternative<概念树动态模板事实>(match.数据->模板))拒绝(D::不支持);
        if(std::get<概念树共享定义事实>(match.数据->模板).治理状态==概念树生命周期状态::退役)业务拒绝(B::需显式治理);
        if(!match.数据->适用)拒绝(D::引用冲突);out.特征模板组.push_back(w.模板);
    }
    std::sort(out.特征模板组.begin(),out.特征模板组.end(),[](auto a,auto b){return a.值.值<b.值.值;});守卫(concepts_,g);return out;
}
std::optional<概念树存在定义>存在概念树应用服务::上位见证(const 存在概念上位请求&r,std::uint64_t g,const 概念树共享预算&b)const{
    using namespace 共享存在应用内部;
    if(r.版本!=1||!r.H||r.H>g||!有效(r.下位.值)||!合法键(r.接边key)||(!r.保留模板.empty()&&(!合法键(r.概念key)||r.概念key==r.接边key)))拒绝();
    auto lower=定义(r.下位,g,r.H,b);const auto*ec=std::get_if<概念树应用存在事实>(&lower);if(!ec)拒绝(D::引用冲突);
    if(ec->治理状态==概念树生命周期状态::退役)业务拒绝(B::需显式治理);
    if(r.保留模板.size()>=ec->定义.特征模板组.size())拒绝(D::引用冲突);
    if(r.保留模板.empty())return std::nullopt;
    概念树存在定义 out{r.保留模板};std::sort(out.特征模板组.begin(),out.特征模板组.end(),[](auto a,auto b){return a.值.值<b.值.值;});
    for(std::size_t i=0;i<out.特征模板组.size();++i){auto id=out.特征模板组[i];if(!有效(id.值)||(i&&id==out.特征模板组[i-1])||std::find(ec->定义.特征模板组.begin(),ec->定义.特征模板组.end(),id)==ec->定义.特征模板组.end())拒绝(D::引用冲突);}
    return out;
}
} // namespace 海中鱼巣
namespace 海中鱼巣 {
void 存在概念树应用服务::核验材料(const 存在概念继续材料&m,std::uint64_t g,const 共享特征应用预算&b)const{
    using namespace 共享存在应用内部;
    if(m.版本!=1||m.原业务.valueless_by_exception()||!m.初始G||m.初始G>g)拒绝();
    const auto*form=std::get_if<存在概念形成请求>(&m.原业务);const auto*upper=std::get_if<存在概念上位请求>(&m.原业务);
    const auto h=form ? form->H : upper->H;
    if(std::visit([](const auto&r){return r.Gread;},m.原业务)!=m.初始G||!h||h>m.初始G)拒绝();
    if(!b.最大派发步骤||!b.最大保留尝试||!b.最大遍历节点||m.步骤.size()>b.最大保留尝试||m.已确认未绑定旧尝试.size()>b.最大保留尝试-m.步骤.size())拒绝(D::数量预算不足);
    const auto actual=form ? std::optional<概念树存在定义>{形成见证(*form,g,b.数据)} : 上位见证(*upper,g,b.数据);
    if(actual!=m.冻结定义)拒绝(D::引用冲突);
    auto check=[&](const 存在概念步骤&s){
        if(s.原请求.valueless_by_exception())拒绝();const auto head=头(s.原请求);
        if(head.合同版本!=1||head.期望事实代次<m.初始G||head.期望事实代次>g||!合法键(head.幂等身份)||
           (s.已知首次H&&(*s.已知首次H<=head.期望事实代次||*s.已知首次H>g)))拒绝();
        std::visit([&](const auto&r){using T=std::decay_t<decltype(r)>;
            if constexpr(std::is_same_v<T,概念树存在建立请求_v2>){
                const auto source=form ? 概念树存在依据项_v2{form->E,h} : 概念树存在依据项_v2{upper->下位,h};
                if(s.阶段!=存在概念步骤阶段::概念||r.版本!=2||!m.冻结定义||r.定义!=*m.冻结定义||r.直接上位!=std::vector<概念树概念身份>{root_}||r.依据!=source||head.幂等身份!=(form ? form->概念key : upper->概念key))拒绝(D::引用冲突);
            }else if constexpr(std::is_same_v<T,概念树应用写请求>){
                const auto*w=std::get_if<概念树支持建立请求>(&r.操作);
                if(!form||r.版本!=1||s.阶段!=存在概念步骤阶段::支持||!w||!m.已定位EC||w->目标概念!=*m.已定位EC||w->世界引用!=概念树世界引用{form->E}||w->证据H!=h||head.幂等身份!=form->支持key)拒绝(D::引用冲突);
            }else{
                const auto*w=std::get_if<概念树上位操作请求>(&r.操作);
                if(!upper||r.版本!=2||s.阶段!=存在概念步骤阶段::接边||!w||!m.已定位父||w->下位!=upper->下位||w->新上位!=m.已定位父||head.幂等身份!=upper->接边key)拒绝(D::引用冲突);
                if(w->操作!=概念树上位操作::新增&&w->操作!=概念树上位操作::替换)拒绝();
                if(w->操作==概念树上位操作::新增&&(w->旧上位||w->旧关系))拒绝();
                if(w->操作==概念树上位操作::替换&&(w->旧上位!=std::optional<概念树概念身份>{root_}||!w->旧关系))拒绝();
            }
        },s.原请求);
    };
    unsigned previous=0;for(const auto&s:m.步骤){if(static_cast<unsigned>(s.阶段)<=previous)拒绝();previous=static_cast<unsigned>(s.阶段);check(s);}
    for(const auto&s:m.已确认未绑定旧尝试){
        check(s);if(s.已知首次H)拒绝();const auto oldG=头(s.原请求).期望事实代次;
        if(std::none_of(m.步骤.begin(),m.步骤.end(),[&](const auto&n){if(n.阶段!=s.阶段||头(n.原请求).期望事实代次<=oldG)return false;auto q=n.原请求;改G(q,oldG);return 等义(q,s.原请求);}))拒绝(D::引用冲突);
    }
}
存在概念操作结果 存在概念树应用服务::推进(存在概念继续材料 material,std::uint64_t expectedG,const 共享特征应用预算&budget,const 场景类数据服务*scene){
    using namespace 共享存在应用内部;
    存在概念操作结果 out;out.继续=std::move(material);auto&m=*out.继续;
    out.Gread=expectedG;
    bool unknown=!m.步骤.empty();std::set<unsigned>verified;
    auto confirmed=[&](存在概念确认事实 fact){out.已确认.push_back(std::move(fact));out.阶段=共享特征阶段状态::部分确认;};
    try{
        out.H=std::visit([](const auto&r){return r.H;},m.原业务);
        auto g=当前(concepts_);out.Gread=g;if(g!=expectedG)拒绝(D::事实代次漂移);
        核验材料(m,g,budget);
        const auto*form=std::get_if<存在概念形成请求>(&m.原业务);const auto*upper=std::get_if<存在概念上位请求>(&m.原业务);
        const auto h=out.H;auto&target=form ? m.已定位EC : m.已定位父;
        auto sceneCheck=[&](){if(form)场景检查(concepts_,existences_,scene,form->场景限制,form->E,g,budget.数据);};
        sceneCheck();
        const auto selected=查找(m.冻结定义,g,h,budget.数据);
        if(form&&form->显式目标&&(!selected||身份(*selected)!=*form->显式目标))拒绝(D::引用冲突);
        if(selected&&治理(*selected)==概念树生命周期状态::退役)业务拒绝(B::需显式治理);
        const bool hasCreate=std::any_of(m.步骤.begin(),m.步骤.end(),[](const auto&s){return s.阶段==存在概念步骤阶段::概念;});
        if(hasCreate&&selected)拒绝(D::引用冲突);
        if(!hasCreate){if(target&&(!selected||*target!=身份(*selected)))拒绝(D::引用冲突);if(selected)target=身份(*selected);}
        if(upper){if(m.已定位EC&&*m.已定位EC!=upper->下位)拒绝();m.已定位EC=upper->下位;}
        auto currentWitness=[&](std::uint64_t at){
            auto current=*form;current.H=at;
            try{if(形成见证(current,g,budget.数据)!=m.冻结定义)业务拒绝(B::被后继覆盖);}
            catch(const 失败&e){if(e.数据==D::未找到||e.数据==D::目标已退出||e.数据==D::引用冲突||e.业务==B::需显式治理)业务拒绝(B::被后继覆盖);throw;}
        };
        auto capacity=[&](){if(m.步骤.size()+m.已确认未绑定旧尝试.size()>=budget.最大保留尝试)拒绝(D::数量预算不足);};
        auto edgeRequest=[&](std::uint64_t pg){
            auto low=定义(upper->下位,g,pg,budget.数据);const auto&parents=父组(low);
            const auto rootEdge=std::find_if(parents.begin(),parents.end(),[&](const auto&e){return e.上位==root_;});
            概念树上位操作请求 q{{1,pg,upper->接边key},概念树上位操作::新增,upper->下位,{},target,{},budget.数据.基础};
            if(rootEdge!=parents.end()&&*target!=root_){q.操作=概念树上位操作::替换;q.旧上位=root_;q.旧关系=rootEdge->关系;}
            return 概念树存在治理请求_v2{2,q,budget.数据};
        };
        std::uint64_t calls=0;
        auto step=[&](存在概念步骤阶段 phase,auto make){
            auto it=std::find_if(m.步骤.begin(),m.步骤.end(),[&](const auto&s){return s.阶段==phase;});
            if(it==m.步骤.end()){if(calls>=budget.最大派发步骤)业务拒绝(B::待继续);capacity();sceneCheck();m.步骤.push_back({phase,make(g),{}, {}});it=m.步骤.end()-1;}
            if(verified.contains(static_cast<unsigned>(phase)))return;
            if(calls>=budget.最大派发步骤)业务拒绝(B::待继续);
            sceneCheck();守卫(concepts_,g);核验材料(m,g,budget);
            const auto pg=头(it->原请求).期望事实代次;
            if(phase!=存在概念步骤阶段::概念){
                if(!target)拒绝();auto dependency=定义(*target,g,pg,budget.数据);
                if(治理(dependency)==概念树生命周期状态::退役)拒绝(D::引用冲突);
                if(m.冻结定义){const auto*ec=std::get_if<概念树应用存在事实>(&dependency);if(!ec||ec->定义!=*m.冻结定义)拒绝(D::引用冲突);}
                if(form)currentWitness(pg);
                else{auto at=*upper;at.H=pg;if(上位见证(at,g,budget.数据)!=m.冻结定义||!等义(it->原请求,存在概念步骤请求{edgeRequest(pg)}))拒绝(D::引用冲突);}
            }
            ++calls;unknown=true;
            auto request=调整预算(it->原请求,budget.数据);
            auto result=std::visit([&](const auto&q)->存在概念步骤回执{using Q=std::decay_t<decltype(q)>;
                if constexpr(std::is_same_v<Q,概念树存在建立请求_v2>)return concepts_.创建存在概念(q);
                else if constexpr(std::is_same_v<Q,概念树应用写请求>)return concepts_.执行应用概念写入(q);
                else return concepts_.治理存在概念(q);
            },request);
            it->回执提示=std::move(result);
            const auto status=std::visit([](const auto&r){return r.状态;},*it->回执提示);
            std::visit([&](const auto&r){if(r.首次H)it->已知首次H=r.首次H;},*it->回执提示);
            if(!std::visit([](const auto&r){return r.成功();},*it->回执提示)){
                const bool unbound=std::visit([](const auto&r){return r.状态==D::事实代次漂移&&!r.首次H&&r.发布状态==概念树发布状态::未派发;},*it->回执提示);
                if(unbound&&!it->已知首次H){capacity();const auto fresh=当前(concepts_);if(fresh<=pg)拒绝(D::事实代次漂移);g=fresh;out.Gread=g;sceneCheck();if(form&&phase==存在概念步骤阶段::支持)currentWitness(g);
                    m.已确认未绑定旧尝试.push_back(*it);改G(it->原请求,g);it->回执提示.reset();unknown=false;业务拒绝(B::待继续);}
                拒绝(status);
            }
            if(phase==存在概念步骤阶段::概念){const auto&r=std::get<概念树存在写入结果_v2>(*it->回执提示);if(!r.概念)拒绝(D::内部不一致);target=r.概念->概念;confirmed(概念树应用定义事实{*r.概念});}
            else if(phase==存在概念步骤阶段::支持){const auto&r=std::get<概念树应用写入结果>(*it->回执提示);if(!r.支持)拒绝(D::内部不一致);confirmed(*r.支持);}
            else{const auto&r=std::get<概念树存在写入结果_v2>(*it->回执提示);auto edge=std::find_if(r.关系组.begin(),r.关系组.end(),[&](const auto&e){return e.下位==upper->下位&&e.上位==*target;});if(edge==r.关系组.end())拒绝(D::内部不一致);confirmed(*edge);}
            g=当前(concepts_);out.Gread=g;verified.insert(static_cast<unsigned>(phase));unknown=false;
        };
        std::vector<存在概念步骤阶段>old;for(const auto&s:m.步骤)old.push_back(s.阶段);
        for(const auto phase:old)step(phase,[](auto)->存在概念步骤请求{拒绝(D::内部不一致);});
        if(!target){step(存在概念步骤阶段::概念,[&](auto pg)->存在概念步骤请求{
            if(!m.冻结定义)拒绝(D::内部不一致);
            auto source=form ? 概念树存在依据项_v2{form->E,h} : 概念树存在依据项_v2{upper->下位,h};
            return 概念树存在建立请求_v2{2,{1,pg,form ? form->概念key : upper->概念key},*m.冻结定义,{root_},source,budget.数据};
        });}
        if(!target)拒绝(D::内部不一致);auto current=定义(*target,g,g,budget.数据);
        if(治理(current)==概念树生命周期状态::退役)业务拒绝(B::被后继覆盖);confirmed(current);
        if(form){
            currentWitness(g);const auto*ec=std::get_if<概念树应用存在事实>(&current);if(!ec||ec->定义!=*m.冻结定义)拒绝(D::引用冲突);
            auto readSupport=[&](概念树应用关联操作 q){auto r=concepts_.读取应用概念关联({1,std::move(q),budget.数据});if(!r.成功())拒绝(r.状态);if(r.Gread!=g||r.H!=g||!std::holds_alternative<std::vector<概念树支持事实>>(*r.数据))拒绝(D::内部不一致);return std::get<std::vector<概念树支持事实>>(std::move(*r.数据));};
            auto a=readSupport(概念树按概念支持读取请求{{1,g,g},*target,budget.数据.基础});
            const auto has=std::count_if(a.begin(),a.end(),[&](const auto&s){return s.世界引用==概念树世界引用{form->E};});if(has>1)拒绝(D::内部不一致);
            if(!has)step(存在概念步骤阶段::支持,[&](auto pg)->存在概念步骤请求{currentWitness(pg);return 概念树应用写请求{1,概念树支持建立请求{{1,pg,form->支持key},*target,form->E,h,budget.数据.基础},budget.数据};});
            sceneCheck();currentWitness(g);a=readSupport(概念树按概念支持读取请求{{1,g,g},*target,budget.数据.基础});
            auto z=readSupport(概念树按世界支持读取请求{{1,g,g},form->E,budget.数据.基础});
            auto support=std::find_if(a.begin(),a.end(),[&](const auto&s){return s.世界引用==概念树世界引用{form->E};});
            if(support==a.end()||std::count(z.begin(),z.end(),*support)!=1)业务拒绝(B::被后继覆盖);
            for(const auto&s:m.步骤)if(s.阶段==存在概念步骤阶段::支持){const auto&receipt=std::get<概念树应用写入结果>(*s.回执提示);if(!receipt.支持||receipt.支持->记录!=support->记录)业务拒绝(B::被后继覆盖);}
            current=定义(*target,g,g,budget.数据);if(治理(current)==概念树生命周期状态::退役)业务拒绝(B::被后继覆盖);confirmed(current);confirmed(*support);
        }else{
            auto live=*upper;live.H=g;if(上位见证(live,g,budget.数据)!=m.冻结定义)业务拒绝(B::被后继覆盖);
            auto lower=定义(upper->下位,g,g,budget.数据);const auto&parents=父组(lower);
            if(std::none_of(parents.begin(),parents.end(),[&](const auto&e){return e.上位==*target;}))step(存在概念步骤阶段::接边,[&](auto pg)->存在概念步骤请求{return edgeRequest(pg);});
            auto graph=concepts_.读取应用概念图({1,{{1,g,g},概念树根角色::存在,false,budget.数据.基础},budget.数据});if(!graph.成功())拒绝(graph.状态);
            if(graph.Gread!=g||graph.H!=g)拒绝(D::内部不一致);
            auto edge=std::find_if(graph.数据->直接边组.begin(),graph.数据->直接边组.end(),[&](const auto&e){return e.下位==upper->下位&&e.上位==*target;});if(edge==graph.数据->直接边组.end())业务拒绝(B::被后继覆盖);
            for(const auto&s:m.步骤)if(s.阶段==存在概念步骤阶段::接边){const auto&q=std::get<概念树上位操作请求>(std::get<概念树存在治理请求_v2>(s.原请求).操作);const auto&r=std::get<概念树存在写入结果_v2>(*s.回执提示);
                if(q.旧关系&&(!r.首次H||std::find(r.已退出事实.begin(),r.已退出事实.end(),*q.旧关系)==r.已退出事实.end()||std::any_of(graph.数据->直接边组.begin(),graph.数据->直接边组.end(),[&](const auto&e){return e.关系==*q.旧关系;})))业务拒绝(B::被后继覆盖);
            }
            lower=定义(upper->下位,g,g,budget.数据);current=定义(*target,g,g,budget.数据);
            if(治理(lower)==概念树生命周期状态::退役||治理(current)==概念树生命周期状态::退役)业务拒绝(B::被后继覆盖);confirmed(lower);confirmed(current);confirmed(*edge);
        }
        守卫(concepts_,g);out.Gread=g;out.业务状态=B::完成;out.阶段=共享特征阶段状态::全部确认;out.可能已发布=false;return out;
    }catch(const 失败&e){out.业务状态=e.业务;}
    catch(const std::bad_alloc&){out.业务状态=B::资源失败;}catch(const std::length_error&){out.业务状态=B::资源失败;}catch(...){out.业务状态=B::内部不一致;}
    out.可能已发布=unknown||verified.size()!=m.步骤.size();
    if(out.可能已发布)out.阶段=共享特征阶段状态::先前阶段待核验;
    else out.阶段=out.已确认.empty() ? 共享特征阶段状态::未派发 : 共享特征阶段状态::部分确认;
    return out;
}
} // namespace 海中鱼巣
namespace 海中鱼巣 {
bool 存在概念操作结果::成功()const noexcept{
    if(版本!=1||业务状态!=共享特征业务状态::完成||阶段!=共享特征阶段状态::全部确认||可能已发布||!Gread||!H||H>Gread||!继续||继续->版本!=1||已确认.empty())return false;
    for(const auto&v:已确认){if(v.valueless_by_exception())return false;if(const auto*c=std::get_if<概念树应用定义事实>(&v)){if(c->valueless_by_exception()||!std::visit([](const auto&f){return 有效(f.概念.值)&&有效(f.本体根.值)&&f.治理状态!=概念树生命周期状态::退役;},*c))return false;}}
    return true;
}
存在概念操作结果 存在概念树应用服务::形成或复用存在概念(const 存在概念形成请求&r){
    using namespace 共享存在应用内部;存在概念操作结果 out;out.Gread=r.Gread;out.H=r.H;
    try{存在概念继续材料 m;m.原业务=r;m.初始G=r.Gread;out.继续=m;if(r.场景限制)拒绝();守卫(concepts_,r.Gread);m.冻结定义=形成见证(r,r.Gread,r.预算.数据);return 推进(std::move(m),r.Gread,r.预算,nullptr);}
    catch(const 失败&e){out.业务状态=e.业务;}catch(const std::bad_alloc&){out.业务状态=B::资源失败;}catch(const std::length_error&){out.业务状态=B::资源失败;}catch(...){out.业务状态=B::内部不一致;}return out;
}
存在概念操作结果 存在概念树应用服务::形成或复用存在概念(const 存在概念形成请求&r,const 共享场景限制&limit,const 场景类数据服务&scene){
    using namespace 共享存在应用内部;存在概念操作结果 out;out.Gread=r.Gread;out.H=r.H;
    try{auto request=r;if(request.场景限制&&!限制相等(*request.场景限制,limit))拒绝();request.场景限制=limit;
        存在概念继续材料 m;m.原业务=request;m.初始G=r.Gread;out.继续=m;守卫(concepts_,r.Gread);场景检查(concepts_,existences_,&scene,limit,r.E,r.Gread,r.预算.数据);m.冻结定义=形成见证(request,r.Gread,r.预算.数据);return 推进(std::move(m),r.Gread,r.预算,&scene);}
    catch(const 失败&e){out.业务状态=e.业务;}catch(const std::bad_alloc&){out.业务状态=B::资源失败;}catch(const std::length_error&){out.业务状态=B::资源失败;}catch(...){out.业务状态=B::内部不一致;}return out;
}
存在概念操作结果 存在概念树应用服务::形成上位存在概念(const 存在概念上位请求&r){
    using namespace 共享存在应用内部;存在概念操作结果 out;out.Gread=r.Gread;out.H=r.H;
    try{存在概念继续材料 m;m.原业务=r;m.初始G=r.Gread;out.继续=m;守卫(concepts_,r.Gread);m.冻结定义=上位见证(r,r.Gread,r.预算.数据);return 推进(std::move(m),r.Gread,r.预算,nullptr);}
    catch(const 失败&e){out.业务状态=e.业务;}catch(const std::bad_alloc&){out.业务状态=B::资源失败;}catch(const std::length_error&){out.业务状态=B::资源失败;}catch(...){out.业务状态=B::内部不一致;}return out;
}
存在概念操作结果 存在概念树应用服务::继续存在概念操作(const 存在概念继续请求&r){
    存在概念操作结果 out;out.Gread=r.Gread;out.可能已发布=!r.材料.步骤.empty();if(out.可能已发布)out.阶段=共享特征阶段状态::先前阶段待核验;
    try{out.继续=r.材料;out.H=std::visit([](const auto&q){return q.H;},r.材料.原业务);return 推进(std::move(*out.继续),r.Gread,r.本次读取预算,nullptr);}
    catch(const std::bad_alloc&){out.业务状态=共享特征业务状态::资源失败;}catch(const std::length_error&){out.业务状态=共享特征业务状态::资源失败;}catch(...){out.业务状态=共享特征业务状态::内部不一致;}return out;
}
存在概念操作结果 存在概念树应用服务::继续存在概念操作(const 存在概念继续请求&r,const 共享场景限制&limit,const 场景类数据服务&scene){
    存在概念操作结果 out;out.Gread=r.Gread;out.可能已发布=!r.材料.步骤.empty();if(out.可能已发布)out.阶段=共享特征阶段状态::先前阶段待核验;
    try{out.继续=r.材料;out.H=std::visit([](const auto&q){return q.H;},r.材料.原业务);const auto*form=std::get_if<存在概念形成请求>(&r.材料.原业务);if(!form||!form->场景限制||!共享存在应用内部::限制相等(*form->场景限制,limit))return out;return 推进(std::move(*out.继续),r.Gread,r.本次读取预算,&scene);}
    catch(const std::bad_alloc&){out.业务状态=共享特征业务状态::资源失败;}catch(const std::length_error&){out.业务状态=共享特征业务状态::资源失败;}catch(...){out.业务状态=共享特征业务状态::内部不一致;}return out;
}
概念树应用读取结果<概念树应用图事实> 存在概念树应用服务::读取存在概念树(const 概念树应用图读取请求&r)const{
    if(r.图请求.根角色!=概念树根角色::存在)return {};return concepts_.读取应用概念图(r);
}
概念树应用写入结果 存在概念树应用服务::退出存在支持(const 概念树应用写请求&r){
    using namespace 共享存在应用内部;概念树应用写入结果 out;out.原请求=r;
    try{const auto*q=std::get_if<概念树支持退出请求>(&r.操作);if(!q)拒绝();const auto g=当前(concepts_);(void)定义(q->预期支持.目标概念,g,q->写入头.期望事实代次,r.预算);return concepts_.执行应用概念写入(r);}
    catch(const 失败&e){out.状态=e.数据;}catch(const std::bad_alloc&){out.状态=D::资源失败;}catch(const std::length_error&){out.状态=D::资源失败;}catch(...){out.状态=D::内部不一致;}return out;
}
概念树存在写入结果_v2 存在概念树应用服务::迁移存在概念(const 概念树存在治理请求_v2&r){
    using namespace 共享存在应用内部;概念树存在写入结果_v2 out;out.原请求=r;
    try{const auto*q=std::get_if<概念树生命周期迁移请求>(&r.操作);if(!q)拒绝();const auto g=当前(concepts_);(void)定义(q->概念,g,q->写入头.期望事实代次,r.预算);return concepts_.治理存在概念(r);}
    catch(const 失败&e){out.状态=e.数据;}catch(const std::bad_alloc&){out.状态=D::资源失败;}catch(const std::length_error&){out.状态=D::资源失败;}catch(...){out.状态=D::内部不一致;}return out;
}
概念树存在写入结果_v2 存在概念树应用服务::退出存在概念(const 概念树存在治理请求_v2&r){
    using namespace 共享存在应用内部;概念树存在写入结果_v2 out;out.原请求=r;
    try{const auto*q=std::get_if<概念树概念退出请求>(&r.操作);if(!q)拒绝();const auto g=当前(concepts_);(void)定义(q->概念,g,q->写入头.期望事实代次,r.预算);return concepts_.治理存在概念(r);}
    catch(const 失败&e){out.状态=e.数据;}catch(const std::bad_alloc&){out.状态=D::资源失败;}catch(const std::length_error&){out.状态=D::资源失败;}catch(...){out.状态=D::内部不一致;}return out;
}
概念树存在写入结果_v2 存在概念树应用服务::释放存在抽象依据(const 概念树存在依据释放请求_v2&r){
    using namespace 共享存在应用内部;概念树存在写入结果_v2 out;out.原请求=r;
    try{const auto g=当前(concepts_);(void)定义(r.所属EC,g,r.写入头.期望事实代次,r.预算);return concepts_.释放存在抽象依据(r);}
    catch(const 失败&e){out.状态=e.数据;}catch(const std::bad_alloc&){out.状态=D::资源失败;}catch(const std::length_error&){out.状态=D::资源失败;}catch(...){out.状态=D::内部不一致;}return out;
}
} // namespace 海中鱼巣
