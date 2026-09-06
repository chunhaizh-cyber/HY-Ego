module;
#include <algorithm>
#include <cstdint>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
export module 海中鱼巣.业务.应用服务.场景成员概念类;
export import 海中鱼巣.业务.应用服务.存在概念树类;
export namespace 海中鱼巣 {
struct 场景存在概念支持投影 final {
    概念树支持事实 支持;
    概念树应用定义事实 概念;
    friend bool operator==(const 场景存在概念支持投影 &, const 场景存在概念支持投影 &) = default;
};
struct 场景组合投影 final {
    概念树场景引用 S;
    场景类成员引用 场景成员;
    存在类结点 E;
    std::vector<共享特征实例事实> 特征组;
    std::vector<场景存在概念支持投影> 存在概念组;
    bool 完整(std::uint64_t) const noexcept;
    friend bool operator==(const 场景组合投影 &, const 场景组合投影 &) = default;
};
struct 场景特征概念请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树场景引用 S;
    std::variant<共享特征生成请求, 共享特征关联请求> 子请求;
    共享特征应用预算 预算;
    friend bool operator==(const 场景特征概念请求 &, const 场景特征概念请求 &) = default;
};
struct 场景存在概念请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树场景引用 S;
    存在概念形成请求 子请求;
    共享特征应用预算 预算;
    friend bool operator==(const 场景存在概念请求 &, const 场景存在概念请求 &) = default;
};
struct 场景成员概念继续材料 final {
    std::uint32_t 版本 = 1;
    std::variant<场景特征概念请求, 场景存在概念请求> 原业务;
    std::variant<共享特征继续材料, 存在概念继续材料> 子材料;
};
struct 场景成员概念结果 final {
    std::uint32_t 版本 = 1;
    共享特征业务状态 业务状态 = 共享特征业务状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    概念树场景引用 S;
    概念树存在引用 E;
    std::variant<共享特征操作结果, 存在概念操作结果> 子结果;
    std::optional<场景组合投影> 最终投影;
    std::optional<场景成员概念继续材料> 继续;
    bool 成功() const noexcept;
};
struct 场景成员概念继续请求 final {
    场景成员概念继续材料 材料;
    std::uint64_t Gread = 0;
    共享特征应用预算 本次读取预算;
    std::vector<共享步骤键> 补充键;
};
struct 共享场景特征见证 final {
    概念树特征引用 F;稳定编码 值事实{},成员关系{};特征类型域身份 类型域身份;
    friend bool operator==(const 共享场景特征见证&,const 共享场景特征见证&)=default;
};
struct 场景成员概念读取请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树场景引用 S;
    概念树存在引用 E;
    std::vector<共享场景特征见证> 指定特征;
    共享特征应用预算 预算;
};
class 场景成员概念应用服务 final {
    概念树类数据服务 &concepts_;
    const 场景类数据服务 &scenes_;
    const 存在类数据服务 &existences_;
    特征概念应用服务 &features_;
    存在概念树应用服务 &existenceConcepts_;
    void 完成投影(场景成员概念结果 &, const 共享特征应用预算 &) const;

  public:
    场景成员概念应用服务(概念树类数据服务 &c, const 场景类数据服务 &s, const 存在类数据服务 &e,
                         特征概念应用服务 &f, 存在概念树应用服务 &ec)
        : concepts_(c), scenes_(s), existences_(e), features_(f), existenceConcepts_(ec) {
        if (!c.使用同一场景基座(s) || !s.使用存在服务(e) || !f.使用存在服务(e) || !ec.使用存在服务(e) ||
            !f.使用概念服务(c) || !ec.使用概念服务(c))
            throw std::invalid_argument("scene concept bindings");
    }
    场景成员概念结果 生成或关联场景特征(const 场景特征概念请求 &);
    场景成员概念结果 形成场景成员存在概念(const 场景存在概念请求 &);
    场景成员概念结果 继续场景成员概念(const 场景成员概念继续请求 &);
    概念树应用读取结果<场景组合投影> 读取场景成员概念(const 场景成员概念读取请求 &) const;
};
} // namespace 海中鱼巣
namespace 海中鱼巣::共享场景应用内部 {
using D=概念树数据状态;using B=共享特征业务状态;
struct 失败 {D 数据;B 业务;};
inline B 映射(D s){switch(s){case D::事实代次漂移:return B::待继续;case D::数量预算不足:return B::预算不足;case D::历史材料不可用:return B::历史不可用;case D::资源失败:return B::资源失败;case D::内部不一致:return B::内部不一致;case D::不支持:return B::不支持;case D::未找到:case D::目标已退出:return B::被后继覆盖;default:return B::入口拒绝;}}
[[noreturn]]inline void 拒绝(D s=D::入口拒绝){throw 失败{s,映射(s)};}
inline std::uint64_t 当前(const 概念树类数据服务&c){auto r=c.读取当前事实代次();if(!r.成功())拒绝(r.状态);return r.Gread;}
inline void 守卫(const 概念树类数据服务&c,std::uint64_t g){if(!g||当前(c)!=g)拒绝(D::事实代次漂移);}
inline bool 同限制(const 共享场景限制&a,const 共享场景限制&b){return a.S==b.S&&a.E==b.E&&a.原场景H==b.原场景H;}
inline 概念树概念身份 身份(const 概念树应用定义事实&f){return std::visit([](const auto&v){return v.概念;},f);}
inline std::uint64_t 子H(const std::variant<共享特征生成请求,共享特征关联请求>&r){return std::visit([](const auto&q){if constexpr(requires{q.原H;})return q.原H;else return q.H;},r);}
inline void 读头(const 概念树类数据服务&c,std::uint32_t version,std::uint64_t g,std::uint64_t h){if(version!=1||!g||!h||h>g)拒绝();守卫(c,g);}
}
namespace 海中鱼巣 {
bool 场景组合投影::完整(std::uint64_t h)const noexcept{
    if(!有效(S.值)||!有效(场景成员.成员关系)||场景成员.目标结点!=E.结点||!有效(E.结点)||!E.创建事实代次||E.创建事实代次>h||(E.退出事实代次&&*E.退出事实代次<=h))return false;
    for(std::size_t i=0;i<特征组.size();++i){const auto&f=特征组[i].匹配;if(f.存在.结点!=E.结点||!有效(f.特征.结点)||!有效(f.特征.值事实)||f.成员.目标结点!=f.特征.结点||std::count(E.特征组.begin(),E.特征组.end(),f.成员)!=1)return false;for(std::size_t j=0;j<i;++j)if(特征组[j].匹配.特征.结点==f.特征.结点)return false;}
    for(std::size_t i=0;i<存在概念组.size();++i){const auto&v=存在概念组[i];if(v.概念.valueless_by_exception())return false;const auto valid=std::visit([&](const auto&c){using C=std::decay_t<decltype(c)>;
        if constexpr(std::is_same_v<C,概念树共享定义事实>)return false;
        else{if constexpr(std::is_same_v<C,概念树概念事实>){if(!c.是本体根||c.根角色!=概念树根角色::存在||c.概念!=c.本体根)return false;}
            return c.概念==v.支持.目标概念&&c.生命周期.创建事实代次&&c.生命周期.创建事实代次<=h&&(!c.生命周期.退出事实代次||*c.生命周期.退出事实代次>h);}
    },v.概念);if(!valid||v.支持.世界引用!=概念树世界引用{概念树存在引用{E.结点}}||!v.支持.证据H||v.支持.证据H>=v.支持.记录生命周期.创建事实代次||v.支持.记录生命周期.创建事实代次>h||(v.支持.记录生命周期.退出事实代次&&*v.支持.记录生命周期.退出事实代次<=h))return false;
        for(std::size_t j=0;j<i;++j)if(存在概念组[j].支持.目标概念==v.支持.目标概念)return false;
    }return true;
}
bool 场景成员概念结果::成功()const noexcept{
    if(版本!=1||业务状态!=共享特征业务状态::完成||!H||H>Gread||!有效(S.值)||!有效(E.值)||子结果.valueless_by_exception()||!继续||!最终投影||最终投影->S!=S||最终投影->E.结点!=E.值||!最终投影->完整(Gread))return false;
    return std::visit([&](const auto&r){return r.成功()&&r.Gread==Gread;},子结果);
}
概念树应用读取结果<场景组合投影> 场景成员概念应用服务::读取场景成员概念(const 场景成员概念读取请求&r)const{
    using namespace 共享场景应用内部;概念树应用读取结果<场景组合投影>out;out.Gread=r.Gread;out.H=r.H;
    try{
        读头(concepts_,r.版本,r.Gread,r.H);const auto g=r.Gread,h=r.H;const auto&b=r.预算.数据;
        if(!有效(r.S.值)||!有效(r.E.值))拒绝();if(r.指定特征.size()>b.基础.最大世界成员数)拒绝(D::数量预算不足);
        std::set<std::uint64_t>ids;for(const auto&w:r.指定特征)if(!有效(w.F.值)||!有效(w.值事实)||!有效(w.成员关系)||!有效(w.类型域身份.值)||!ids.insert(w.F.值.值).second)拒绝();
        auto scene=scenes_.读取场景历史事实({1,g,h,r.S.值,b.基础.最大世界成员数});if(!scene.成功()){
            switch(scene.状态){case 场景类数据状态::历史材料已清理:拒绝(D::历史材料不可用);case 场景类数据状态::事实代次漂移:拒绝(D::事实代次漂移);case 场景类数据状态::数量预算不足:拒绝(D::数量预算不足);case 场景类数据状态::资源失败:拒绝(D::资源失败);case 场景类数据状态::未找到:拒绝(D::未找到);case 场景类数据状态::目标已退出:拒绝(D::目标已退出);case 场景类数据状态::入口拒绝:拒绝();default:拒绝(D::内部不一致);}}
        if(scene.Gread!=g||scene.H!=h||scene.场景结点->结点!=r.S.值)拒绝(D::内部不一致);
        auto e=existences_.读取存在历史事实({1,g,h,r.E.值,b.基础.最大世界成员数});if(!e.成功()){
            switch(e.状态){case 特征引用读取状态::历史材料不可用:拒绝(D::历史材料不可用);case 特征引用读取状态::事实代次漂移:拒绝(D::事实代次漂移);case 特征引用读取状态::数量预算不足:拒绝(D::数量预算不足);case 特征引用读取状态::资源失败:拒绝(D::资源失败);case 特征引用读取状态::未找到:拒绝(D::未找到);case 特征引用读取状态::目标已退出:拒绝(D::目标已退出);case 特征引用读取状态::入口拒绝:拒绝();default:拒绝(D::内部不一致);}}
        if(e.Gread!=g||e.H!=h||e.存在->结点!=r.E.值)拒绝(D::内部不一致);
        场景组合投影 p;p.S=r.S;p.E=std::move(*e.存在);bool member=false;
        for(const auto&m:scene.场景结点->存在组)if(m.目标结点==r.E.值){if(member)拒绝(D::内部不一致);p.场景成员=m;member=true;}if(!member)拒绝(D::引用冲突);
        for(const auto&w:r.指定特征){auto f=features_.读取共享特征实例({1,g,h,r.E,w.F,w.值事实,w.成员关系,w.类型域身份,r.预算});if(!f.成功())拒绝(f.状态);if(f.Gread!=g||f.H!=h||f.数据->匹配.特征.结点!=w.F.值||f.数据->匹配.特征.值事实!=w.值事实||f.数据->匹配.成员.成员关系!=w.成员关系||f.数据->匹配.类型域.身份!=w.类型域身份||std::count(p.E.特征组.begin(),p.E.特征组.end(),f.数据->匹配.成员)!=1)拒绝(D::引用冲突);p.特征组.push_back(std::move(*f.数据));}
        auto graph=concepts_.读取应用概念图({1,{{1,g,h},概念树根角色::存在,false,b.基础},b});if(!graph.成功())拒绝(graph.状态);
        auto supports=concepts_.读取应用概念关联({1,概念树按世界支持读取请求{{1,g,h},r.E,b.基础},b});if(!supports.成功())拒绝(supports.状态);
        if(graph.Gread!=g||graph.H!=h||supports.Gread!=g||supports.H!=h||!std::holds_alternative<std::vector<概念树支持事实>>(*supports.数据))拒绝(D::内部不一致);
        for(const auto&s:std::get<std::vector<概念树支持事实>>(*supports.数据)){
            const auto c=std::find_if(graph.数据->节点组.begin(),graph.数据->节点组.end(),[&](const auto&c){return 身份(c)==s.目标概念;});if(c==graph.数据->节点组.end())continue;
            if(std::holds_alternative<概念树共享定义事实>(*c))拒绝(D::内部不一致);if(const auto*root=std::get_if<概念树概念事实>(&*c)){if(!root->是本体根||root->概念!=graph.数据->根身份)拒绝(D::内部不一致);}
            auto reverse=concepts_.读取应用概念关联({1,概念树按概念支持读取请求{{1,g,h},s.目标概念,b.基础},b});if(!reverse.成功())拒绝(reverse.状态);if(reverse.Gread!=g||reverse.H!=h||!std::holds_alternative<std::vector<概念树支持事实>>(*reverse.数据)||std::count(std::get<std::vector<概念树支持事实>>(*reverse.数据).begin(),std::get<std::vector<概念树支持事实>>(*reverse.数据).end(),s)!=1)拒绝(D::内部不一致);
            p.存在概念组.push_back({s,*c});
        }
        守卫(concepts_,g);if(!p.完整(h))拒绝(D::内部不一致);out.数据=std::move(p);out.状态=D::已读取;
    }catch(const 失败&e){out.状态=e.数据;}catch(const std::bad_alloc&){out.状态=D::资源失败;}catch(const std::length_error&){out.状态=D::资源失败;}catch(...){out.状态=D::内部不一致;}return out;
}
void 场景成员概念应用服务::完成投影(场景成员概念结果&out,const 共享特征应用预算&b)const{
    using namespace 共享场景应用内部;if(!std::visit([](const auto&r){return r.成功();},out.子结果))return;
    const auto g=当前(concepts_);if(std::visit([&](const auto&r){return r.Gread!=g;},out.子结果))拒绝(D::事实代次漂移);
    std::vector<共享场景特征见证>witnesses;
    if(const auto*child=std::get_if<共享特征操作结果>(&out.子结果)){
        if(!child->实例||!child->继续材料||!out.继续)拒绝(D::内部不一致);const auto&instance=child->实例->匹配;
        const auto*original=std::get_if<场景特征概念请求>(&out.继续->原业务);if(!original||instance.存在.结点!=out.E.值)拒绝(D::内部不一致);
        std::visit([&](const auto&q){if(instance.类型域.身份!=q.类型域身份||q.E!=out.E)拒绝(D::引用冲突);using Q=std::decay_t<decltype(q)>;
            if constexpr(std::is_same_v<Q,共享特征关联请求>){if(instance.特征.结点!=q.F.值||instance.特征.值事实!=q.值事实||instance.成员.成员关系!=q.成员关系)拒绝(D::引用冲突);}
            else{const auto&m=*child->继续材料;if(!m.生成观察H||!m.已定位F||!m.已定位成员||*m.生成观察H<=q.原H||instance.特征.结点!=m.已定位F->结点||instance.特征.值事实!=m.已定位F->值事实||instance.成员.成员关系!=m.已定位成员->成员关系)拒绝(D::引用冲突);}
        },original->子请求);
        witnesses.push_back({概念树特征引用{instance.特征.结点},instance.特征.值事实,instance.成员.成员关系,instance.类型域.身份});
    }
    auto projection=读取场景成员概念({1,g,g,out.S,out.E,witnesses,b});if(!projection.成功()){
        if(projection.状态==D::引用冲突||projection.状态==D::未找到||projection.状态==D::目标已退出)throw 失败{projection.状态,B::被后继覆盖};拒绝(projection.状态);}
    out.最终投影=std::move(projection.数据);out.Gread=g;守卫(concepts_,g);if(!out.成功())拒绝(D::内部不一致);
}
} // namespace 海中鱼巣
namespace 海中鱼巣 {
场景成员概念结果 场景成员概念应用服务::生成或关联场景特征(const 场景特征概念请求&r){
    using namespace 共享场景应用内部;场景成员概念结果 out;out.S=r.S;out.H=r.H;out.Gread=r.Gread;
    try{
        if(r.子请求.valueless_by_exception())拒绝();out.E=std::visit([](const auto&q){return q.E;},r.子请求);
        out.继续=场景成员概念继续材料{1,r,共享特征继续材料{}};
        读头(concepts_,r.版本,r.Gread,r.H);if(!有效(r.S.值)||子H(r.子请求)!=r.H||std::visit([](const auto&q){return q.Gread;},r.子请求)!=r.Gread)拒绝();
        const 共享场景限制 limit{r.S,out.E,r.H};
        auto result=std::visit([&](const auto&q)->共享特征操作结果{
            if constexpr(std::is_same_v<std::decay_t<decltype(q)>,共享特征生成请求>)return features_.生成并关联共享特征(q,limit,scenes_);
            else return features_.关联已有共享特征(q,limit,scenes_);
        },r.子请求);
        out.Gread=result.Gread;out.业务状态=result.状态;out.子结果=std::move(result);
        const auto&saved=std::get<共享特征操作结果>(out.子结果);if(saved.继续材料)out.继续->子材料=*saved.继续材料;
        完成投影(out,r.预算);
    }catch(const 失败&e){out.业务状态=e.业务;out.最终投影.reset();}catch(const std::bad_alloc&){out.业务状态=B::资源失败;out.最终投影.reset();}catch(const std::length_error&){out.业务状态=B::资源失败;out.最终投影.reset();}catch(...){out.业务状态=B::内部不一致;out.最终投影.reset();}return out;
}
场景成员概念结果 场景成员概念应用服务::形成场景成员存在概念(const 场景存在概念请求&input){
    using namespace 共享场景应用内部;场景成员概念结果 out;out.S=input.S;out.E=input.子请求.E;out.H=input.H;out.Gread=input.Gread;
    try{
        auto r=input;const 共享场景限制 limit{r.S,r.子请求.E,r.H};if(r.子请求.场景限制&&!同限制(*r.子请求.场景限制,limit))拒绝();r.子请求.场景限制=limit;
        out.继续=场景成员概念继续材料{1,r,存在概念继续材料{}};
        读头(concepts_,r.版本,r.Gread,r.H);if(!有效(r.S.值)||r.子请求.Gread!=r.Gread||r.子请求.H!=r.H)拒绝();
        auto result=existenceConcepts_.形成或复用存在概念(r.子请求,limit,scenes_);
        out.Gread=result.Gread;out.业务状态=result.业务状态;out.子结果=std::move(result);
        const auto&saved=std::get<存在概念操作结果>(out.子结果);if(saved.继续)out.继续->子材料=*saved.继续;
        完成投影(out,r.预算);
    }catch(const 失败&e){out.业务状态=e.业务;out.最终投影.reset();}catch(const std::bad_alloc&){out.业务状态=B::资源失败;out.最终投影.reset();}catch(const std::length_error&){out.业务状态=B::资源失败;out.最终投影.reset();}catch(...){out.业务状态=B::内部不一致;out.最终投影.reset();}return out;
}
} // namespace 海中鱼巣

namespace 海中鱼巣 {
场景成员概念结果 场景成员概念应用服务::继续场景成员概念(const 场景成员概念继续请求&r){
    using namespace 共享场景应用内部;场景成员概念结果 out;out.Gread=r.Gread;
    // 在任何复制或业务互证前保存无需分配的待核验分账。
    if(const auto*m=std::get_if<共享特征继续材料>(&r.材料.子材料)){
        auto&child=std::get<共享特征操作结果>(out.子结果);child.原H=m->原H;child.Gread=r.Gread;child.可能已发布=!m->步骤.empty()||m->缺步骤恢复.has_value();
        if(child.可能已发布)child.阶段=共享特征阶段状态::先前阶段待核验;
    }else if(const auto*m=std::get_if<存在概念继续材料>(&r.材料.子材料)){
        存在概念操作结果 child;child.Gread=r.Gread;child.可能已发布=!m->步骤.empty();if(child.可能已发布)child.阶段=共享特征阶段状态::先前阶段待核验;out.子结果=std::move(child);
    }
    try{
        out.继续=r.材料;
        if(r.材料.原业务.valueless_by_exception()||r.材料.子材料.valueless_by_exception())拒绝();
        std::visit([&](const auto&q){out.S=q.S;out.H=q.H;},r.材料.原业务);
        if(r.材料.版本!=1)拒绝();
        if(const auto*original=std::get_if<场景特征概念请求>(&r.材料.原业务)){
            const auto*m=std::get_if<共享特征继续材料>(&r.材料.子材料);if(!m||original->子请求.valueless_by_exception())拒绝();
            out.E=std::visit([](const auto&q){return q.E;},original->子请求);
            auto&saved=std::get<共享特征操作结果>(out.子结果);saved.继续材料=*m;
            const 共享场景限制 limit{out.S,out.E,out.H};
            if(original->版本!=1||!有效(out.S.值)||!有效(out.E.值)||子H(original->子请求)!=out.H||m->原H!=out.H||!m->场景限制||!同限制(*m->场景限制,limit))拒绝();
            const bool same=std::visit([&](const auto&q){using Q=std::decay_t<decltype(q)>;const auto*child=std::get_if<Q>(&m->原业务);return child&&*child==q&&q.Gread==original->Gread;},original->子请求);if(!same)拒绝();
            auto result=features_.继续共享特征操作({1,r.Gread,*m,r.补充键,r.本次读取预算},limit,scenes_);
            out.Gread=result.Gread;out.业务状态=result.状态;out.子结果=std::move(result);
            const auto&child=std::get<共享特征操作结果>(out.子结果);if(child.继续材料)out.继续->子材料=*child.继续材料;
        }else{
            const auto&ecOriginal=std::get<场景存在概念请求>(r.材料.原业务);const auto*m=std::get_if<存在概念继续材料>(&r.材料.子材料);if(!m)拒绝();
            out.E=ecOriginal.子请求.E;auto&saved=std::get<存在概念操作结果>(out.子结果);saved.H=out.H;saved.继续=*m;
            const 共享场景限制 limit{out.S,out.E,out.H};const auto*child=std::get_if<存在概念形成请求>(&m->原业务);
            if(!r.补充键.empty()||ecOriginal.版本!=1||!有效(out.S.值)||!有效(out.E.值)||ecOriginal.子请求.H!=out.H||ecOriginal.子请求.Gread!=ecOriginal.Gread||!ecOriginal.子请求.场景限制||!同限制(*ecOriginal.子请求.场景限制,limit)||!child||*child!=ecOriginal.子请求)拒绝();
            auto result=existenceConcepts_.继续存在概念操作({*m,r.Gread,r.本次读取预算},limit,scenes_);
            out.Gread=result.Gread;out.业务状态=result.业务状态;out.子结果=std::move(result);
            const auto&actual=std::get<存在概念操作结果>(out.子结果);if(actual.继续)out.继续->子材料=*actual.继续;
        }
        完成投影(out,r.本次读取预算);
    }catch(const 失败&e){out.业务状态=e.业务;out.最终投影.reset();}catch(const std::bad_alloc&){out.业务状态=B::资源失败;out.最终投影.reset();}catch(const std::length_error&){out.业务状态=B::资源失败;out.最终投影.reset();}catch(...){out.业务状态=B::内部不一致;out.最终投影.reset();}return out;
}
} // namespace 海中鱼巣
