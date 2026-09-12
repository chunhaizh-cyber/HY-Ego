module;
#include <algorithm>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.业务.应用服务.场景成员概念类;
export import 海中鱼巣.业务.应用服务.特征概念类;
export import 海中鱼巣.业务.应用服务.存在概念树类;

export namespace 海中鱼巣 {
struct 存在当前采用意图_v2 final {
    std::uint32_t 版本=2;
    L1所有者范围写入幂等身份 幂等身份{};
    std::optional<存在当前采用事实> 原采用预期;
    std::uint64_t 关系预算=0;
    friend bool operator==(const 存在当前采用意图_v2&,const 存在当前采用意图_v2&)=default;
};
struct 场景成员概念请求_v2 final {
    std::uint32_t 版本=2;
    概念树场景引用 S{};
    概念树存在引用 E{};
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    特征概念观察请求 观察;
    std::optional<存在当前采用意图_v2> 采用;
    std::uint64_t 场景关系预算=0;
    稳定编码 场景成员关系{};
    friend bool operator==(const 场景成员概念请求_v2&,const 场景成员概念请求_v2&)=default;
};
enum class 场景成员概念继续阶段_v2 : std::uint8_t {
    特征应用=1,存在采用=2,最终读回=3
};
struct 场景成员概念继续材料_v2 final {
    std::uint32_t 版本=2;
    场景成员概念继续阶段_v2 阶段=场景成员概念继续阶段_v2::特征应用;
    场景成员概念请求_v2 原请求;
    std::optional<存在当前采用写请求> 采用原请求;
    std::optional<定位特征结果> 已确认定位;
    friend bool operator==(const 场景成员概念继续材料_v2& a,const 场景成员概念继续材料_v2& b) {
        if(a.版本!=b.版本||a.阶段!=b.阶段||a.原请求!=b.原请求||a.采用原请求!=b.采用原请求||a.已确认定位.has_value()!=b.已确认定位.has_value())return false;
        if(!a.已确认定位)return true;const auto& x=*a.已确认定位;const auto& y=*b.已确认定位;
        return x.版本==y.版本&&x.状态==y.状态&&x.Gread==y.Gread&&x.首次发布H==y.首次发布H&&x.事实==y.事实&&x.候选F==y.候选F&&x.原请求==y.原请求;
    }
};
struct 场景特征概念结果_v2 final {
    std::uint32_t 版本=2;
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    特征概念处理结果 处理;
    std::optional<准确特征读取事实> 已确认F;
    std::optional<特征概念事实> 已确认C;
    std::optional<特征名称事实> 已确认名称;
    bool 成功() const noexcept;
};
struct 场景组合投影_v2 final {
    std::uint32_t 版本=2;
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    概念树场景引用 S{};
    概念树存在引用 E{};
    场景直接包含事实 场景成员;
    准确特征读取事实 F;
    特征概念事实 C;
    特征名称事实 名称;
    std::optional<存在当前采用事实> 当前采用;
    定位特征事实 定位;
    bool 完整() const noexcept;
};
enum class 场景成员概念状态_v2 : std::uint8_t {
    完成=1,需选择准确F,待特征应用收敛,待存在采用收敛,
    准确F已成立名称未完成,当前采用未完成,入口拒绝,未找到,规则缺失,
    类型不相容,引用冲突,多义,事实代次漂移,历史材料不可用,数量预算不足,
    幂等冲突,旧格式不支持,不支持,资源失败,内部不一致,已可能发布,
    定位拒绝=22,定位待原请求确认=23,准确F已定位概念未完成=24
};
struct 场景成员概念结果_v2 final {
    std::uint32_t 版本=2;
    场景成员概念状态_v2 状态=场景成员概念状态_v2::入口拒绝;
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    概念树场景引用 S{};
    概念树存在引用 E{};
    场景特征概念结果_v2 特征概念;
    std::optional<存在当前采用结果> 当前采用;
    std::optional<场景成员概念继续材料_v2> 继续;
    std::optional<场景组合投影_v2> 最终投影;
    bool 成功() const noexcept;
};
class 场景成员概念应用服务 final {
    概念树类数据服务& concepts_;
    const 场景类数据服务& scenes_;
    存在类数据服务& existences_;
    特征概念应用服务& features_;
    存在概念树应用服务& existenceConcepts_;
    void 验证入口(const 场景成员概念请求_v2&) const;
    场景直接包含事实 读取成员(const 场景成员概念请求_v2&,std::uint64_t,std::uint64_t) const;
    void 补全特征(场景特征概念结果_v2&,const 场景成员概念请求_v2&,std::uint64_t) const;
    void 完成投影(场景成员概念结果_v2&,const 场景成员概念请求_v2&) const;
    void 处理采用(场景成员概念结果_v2&,const 场景成员概念请求_v2&);
    bool 消费特征结果(场景成员概念结果_v2&,const 场景成员概念请求_v2&,特征概念处理结果,
        const 场景成员概念继续材料_v2* =nullptr);
public:
    场景成员概念应用服务(概念树类数据服务& c,const 场景类数据服务& s,存在类数据服务& e,
        特征概念应用服务& f,存在概念树应用服务& ec)
        :concepts_(c),scenes_(s),existences_(e),features_(f),existenceConcepts_(ec) {
        if(!c.使用同一场景基座(s)||!s.使用存在提供者(e)||!ec.使用存在服务(e)
            ||!f.使用概念服务(c)||!ec.使用概念服务(c)||!f.使用定位参与者(e,s))
            throw std::invalid_argument("scene concept bindings");
    }
    场景成员概念结果_v2 处理场景成员概念(const 场景成员概念请求_v2&);
    场景成员概念结果_v2 收敛场景成员概念(const 场景成员概念继续材料_v2&);
};
}

namespace 海中鱼巣::场景特征应用内部 {
using D=概念树数据状态;
using B=场景成员概念状态_v2;
using P=场景成员概念继续阶段_v2;
struct 失败 {B 状态;};
[[noreturn]] inline void 拒绝(B s=B::入口拒绝){throw 失败{s};}
inline B 映射(D s) noexcept {
    switch(s){
    case D::未找到:case D::目标已退出:return B::未找到;
    case D::规则缺失:return B::规则缺失;
    case D::类型不相容:return B::类型不相容;
    case D::引用冲突:return B::引用冲突;
    case D::事实代次漂移:return B::事实代次漂移;
    case D::历史材料不可用:return B::历史材料不可用;
    case D::数量预算不足:return B::数量预算不足;
    case D::幂等冲突:return B::幂等冲突;
    case D::旧格式不支持:return B::旧格式不支持;
    case D::不支持:return B::不支持;
    case D::资源失败:return B::资源失败;
    case D::已可能发布:return B::已可能发布;
    case D::前次写入待收敛:return B::待特征应用收敛;
    case D::入口拒绝:return B::入口拒绝;
    default:return B::内部不一致;
    }
}
inline B 映射(特征数据错误 s) noexcept {
    switch(s){
    case 特征数据错误::未找到:case 特征数据错误::已退出:return B::未找到;
    case 特征数据错误::未设置:case 特征数据错误::规则缺失:return B::规则缺失;
    case 特征数据错误::类型不相容:return B::类型不相容;
    case 特征数据错误::引用冲突:return B::引用冲突;
    case 特征数据错误::并发变化:return B::事实代次漂移;
    case 特征数据错误::历史材料不可用:return B::历史材料不可用;
    case 特征数据错误::数量预算不足:return B::数量预算不足;
    case 特征数据错误::幂等冲突:return B::幂等冲突;
    case 特征数据错误::旧格式不支持:return B::旧格式不支持;
    case 特征数据错误::能力未提供:return B::不支持;
    case 特征数据错误::资源失败:return B::资源失败;
    case 特征数据错误::发布结果未确认:return B::已可能发布;
    case 特征数据错误::前次写入待收敛:return B::待特征应用收敛;
    case 特征数据错误::入口拒绝:return B::入口拒绝;
    default:return B::内部不一致;
    }
}
inline B 映射(存在类数据状态 s) noexcept {
    switch(s){
    case 存在类数据状态::未找到:case 存在类数据状态::目标已退出:return B::未找到;
    case 存在类数据状态::事实代次漂移:return B::事实代次漂移;
    case 存在类数据状态::历史材料已清理:return B::历史材料不可用;
    case 存在类数据状态::数量预算不足:return B::数量预算不足;
    case 存在类数据状态::幂等冲突:return B::幂等冲突;
    case 存在类数据状态::引用冲突:return B::引用冲突;
    case 存在类数据状态::格式不支持:return B::旧格式不支持;
    case 存在类数据状态::资源失败:return B::资源失败;
    case 存在类数据状态::已可能发布:return B::已可能发布;
    case 存在类数据状态::入口拒绝:return B::入口拒绝;
    default:return B::内部不一致;
    }
}
inline bool 生命周期有效(const 概念树生命周期& l,std::uint64_t h) noexcept {
    return l.创建事实代次&&l.创建事实代次<=h&&(!l.退出事实代次||*l.退出事实代次>h);
}
inline bool 特征有效(const 准确特征读取事实& f,std::uint64_t g,std::uint64_t h) noexcept {
    if(f.Gread!=g||f.H!=h||!h||h>g||!浅层结构有效(f.信息)||!有效(f.类型关系)
        ||!f.准确值事实||!有效(*f.准确值事实)||!f.创建G||f.创建G>h
        ||(f.退出G&&*f.退出G<=h)||f.完整值.valueless_by_exception())return false;
    if(const auto* n=std::get_if<std::int64_t>(&f.信息.准确值)){
        const auto* whole=std::get_if<std::int64_t>(&f.完整值);return whole&&*whole==*n;
    }
    const auto* value=std::get_if<特征值信息>(&f.完整值);
    return value&&value->值身份==std::get<特征值身份>(f.信息.准确值)
        &&value->值身份.编码==*f.准确值事实&&std::holds_alternative<std::int64_t>(value->值内容);
}
inline bool 同特征事实(const 准确特征读取事实& a,const 准确特征读取事实& b) noexcept {
    // Gread/H 是读取封套，不是持久 F 内容；E 写入后最终读回的截止可以更晚。
    return a.信息==b.信息&&a.完整值==b.完整值&&a.类型关系==b.类型关系
        &&a.准确值事实==b.准确值事实&&a.创建G==b.创建G&&a.退出G==b.退出G;
}
inline bool 域同义(const 特征规范I64域& a,const 特征规范I64域& b) noexcept {
    if(a.区间.size()!=b.区间.size())return false;
    for(std::size_t i=0;i<a.区间.size();++i)if(a.区间[i].下界!=b.区间[i].下界||a.区间[i].上界!=b.区间[i].上界)return false;
    return true;
}
inline bool 同名称(const 特征名称事实& a,const 特征名称事实& b) noexcept {
    return a.关系==b.关系&&a.F==b.F&&a.C==b.C&&a.生命周期==b.生命周期;
}
inline bool 同定位(const 定位特征事实& a,const 定位特征事实& b) noexcept {
    return a.F==b.F&&a.正式特征类型==b.正式特征类型&&a.准确I64==b.准确I64
        &&a.位置.场景==b.位置.场景&&a.位置.组织父==b.位置.组织父&&a.已知关系==b.已知关系
        &&a.组织关系==b.组织关系&&a.出生H==b.出生H;
}
inline bool 同概念(const 特征概念事实& a,const 特征概念事实& b) noexcept {
    if(a.身份!=b.身份||a.定义.类型!=b.定义.类型||!域同义(a.定义.域,b.定义.域)||a.本体根!=b.本体根||a.类型根!=b.类型根
        ||a.规则!=b.规则||a.定义记录!=b.定义记录||a.生命周期值事实!=b.生命周期值事实||a.类型关系!=b.类型关系
        ||a.生命周期!=b.生命周期||a.治理状态!=b.治理状态||a.直接上位!=b.直接上位
        ||a.区间事实.size()!=b.区间事实.size()||a.来源事实.size()!=b.来源事实.size())return false;
    for(std::size_t i=0;i<a.区间事实.size();++i){const auto& x=a.区间事实[i];const auto& y=b.区间事实[i];
        if(x.节点!=y.节点||x.成员关系!=y.成员关系||x.下界值事实!=y.下界值事实||x.上界值事实!=y.上界值事实
            ||x.区间.下界!=y.区间.下界||x.区间.上界!=y.区间.上界||x.生命周期!=y.生命周期)return false;}
    for(std::size_t i=0;i<a.来源事实.size();++i){const auto& x=a.来源事实[i];const auto& y=b.来源事实[i];
        if(x.记录!=y.记录||x.所属关系!=y.所属关系||x.目标关系!=y.目标关系||x.截止值事实!=y.截止值事实
            ||x.证据H!=y.证据H||x.生命周期!=y.生命周期||x.当前引用已释放!=y.当前引用已释放||x.来源.index()!=y.来源.index())return false;
        if(const auto* f=std::get_if<特征类型身份>(&x.来源)){if(*f!=std::get<特征类型身份>(y.来源))return false;}
        else if(const auto* o=std::get_if<类型观察事实>(&x.来源)){const auto& z=std::get<类型观察事实>(y.来源);
            if(o->记录!=z.记录||o->证据H!=z.证据H||o->输入.F!=z.输入.F||o->输入.时间!=z.输入.时间
                ||o->输入.键.类型!=z.输入.键.类型||o->输入.键.来源!=z.输入.键.来源||o->输入.键.序号!=z.输入.键.序号)return false;}
        else if(const auto* c=std::get_if<概念树概念身份>(&x.来源)){if(*c!=std::get<概念树概念身份>(y.来源))return false;}
        else return false;
    }
    return true;
}
inline bool 命名有效(const 准确特征读取事实& f,const 特征概念事实& c,
    const 特征名称事实& n,std::uint64_t h) noexcept {
    return 有效(c.身份.值)&&有效(c.定义记录)&&有效(c.类型关系)&&!c.定义.域.区间.empty()
        &&c.定义.类型==f.信息.类型&&生命周期有效(c.生命周期,h)
        &&c.治理状态!=概念树生命周期状态::退役&&有效(n.关系)
        &&n.F==f.信息.身份&&n.C==c.身份&&生命周期有效(n.生命周期,h);
}
inline bool 预算有效(const 特征概念预算& b) noexcept {
    const auto& a=b.基础;
    return a.最大概念数&&a.最大关系数&&a.最大来源数&&a.最大支持数&&a.最大世界成员数
        &&a.最大特征属性数&&a.最大动态槽数&&a.最大动态模板数&&b.最大观察数
        &&b.最大区间数&&b.最大命中数&&b.最大名称数&&b.最大首次材料项数;
}
inline 概念树共享预算 共享预算(const 特征概念预算& b){
    return {b.基础,b,b.最大区间数,b.基础.最大来源数,b.最大名称数,b.最大首次材料项数};
}
inline std::uint64_t 当前(const 概念树类数据服务& c){
    const auto r=c.读取当前事实代次();if(!r.成功())拒绝(映射(r.状态));return r.Gread;
}
inline void 守卫(const 概念树类数据服务& c,std::uint64_t g){if(当前(c)!=g)拒绝(B::事实代次漂移);}
inline bool 采用一致(const 存在当前采用事实& a,概念树存在引用 e,const 准确特征读取事实& f) noexcept {
    return 有效(a.关系)&&a.E==e.值&&a.F==f.信息.身份&&a.FT==f.信息.类型;
}
}

namespace 海中鱼巣 {
bool 场景特征概念结果_v2::成功() const noexcept {
    using namespace 场景特征应用内部;
    return 版本==2&&处理.状态==特征概念处理状态::完成&&H&&H<=Gread
        &&已确认F&&已确认C&&已确认名称&&特征有效(*已确认F,Gread,H)
        &&处理.准确匹配.size()==1&&处理.准确匹配.front()==已确认F->信息.身份
        &&处理.概念==已确认C&&处理.名称==已确认名称
        &&处理.定位结果&&处理.定位结果->成功()&&处理.定位结果->事实->F==已确认F->信息.身份.编码
        &&处理.定位结果->事实->正式特征类型==已确认F->信息.类型.编码
        &&命名有效(*已确认F,*已确认C,*已确认名称,H);
}
bool 场景组合投影_v2::完整() const noexcept {
    using namespace 场景特征应用内部;
    return 版本==2&&Gread&&Gread==H&&有效(S.值)&&有效(E.值)
        &&场景成员.Gread==Gread&&场景成员.H==H&&场景成员.种类==场景直接包含种类::存在成员
        &&场景成员.父场景==S.值&&场景成员.成员==E.值&&有效(场景成员.关系.编码)
        &&场景成员.关系.源==S.值&&场景成员.关系.目标==E.值&&有效(场景成员.关系.关系类型)
        &&场景成员.关系.角色或顺序==1&&场景成员.关系.生命周期.创建事实代次
        &&场景成员.关系.生命周期.创建事实代次<=H&&!场景成员.关系.生命周期.退出事实代次&&特征有效(F,Gread,H)
        &&定位.F==F.信息.身份.编码&&定位.正式特征类型==F.信息.类型.编码&&定位.位置.场景==S.值
        &&有效(定位.位置.组织父)&&有效(定位.已知关系)&&有效(定位.组织关系)&&定位.出生H==F.创建G
        &&命名有效(F,C,名称,H)&&(!当前采用||采用一致(*当前采用,E,F));
}
bool 场景成员概念结果_v2::成功() const noexcept {
    using namespace 场景特征应用内部;
    if(版本!=2||状态!=B::完成||继续||!特征概念.成功()||!最终投影||!最终投影->完整()
        ||最终投影->S!=S||最终投影->E!=E||最终投影->Gread!=Gread||最终投影->H!=H
        ||!同特征事实(最终投影->F,*特征概念.已确认F)||!同概念(最终投影->C,*特征概念.已确认C)
        ||!同名称(最终投影->名称,*特征概念.已确认名称)||!同定位(最终投影->定位,*特征概念.处理.定位结果->事实))return false;
    if(!当前采用)return !最终投影->当前采用;
    return 当前采用->成功()&&当前采用->采用&&采用一致(*当前采用->采用,E,最终投影->F)
        &&最终投影->当前采用==当前采用->采用;
}
void 场景成员概念应用服务::验证入口(const 场景成员概念请求_v2& r) const {
    using namespace 场景特征应用内部;
    const auto& o=r.观察;
    if(r.版本!=2||!有效(r.S.值)||!有效(r.E.值)||!r.H||r.H>r.Gread||!r.场景关系预算||!有效(r.场景成员关系)
        ||!有效(o.观察.类型)||!有效(o.观察.来源)||!o.观察.序号||!o.最大保留观察数
        ||!浅层结构有效(o.准确值)||(o.指定F&&!有效(*o.指定F))||!预算有效(o.预算)
        ||!concepts_.使用同一场景基座(scenes_)||!scenes_.使用存在提供者(existences_)
        ||!features_.使用概念服务(concepts_)||!existenceConcepts_.使用概念服务(concepts_)
        ||!existenceConcepts_.使用存在服务(existences_)||!features_.使用定位参与者(existences_,scenes_)
        ||o.定位.位置.场景!=r.S.值||!有效(o.定位.位置.组织父))拒绝();
    if(r.采用){
        const auto& a=*r.采用;
        if(a.版本!=2||!有效(a.幂等身份)||!a.关系预算)拒绝();
        if(a.原采用预期&&(!有效(a.原采用预期->关系)||!有效(a.原采用预期->F)
            ||a.原采用预期->E!=r.E.值||a.原采用预期->FT!=o.观察.类型))拒绝();
    }
}
场景直接包含事实 场景成员概念应用服务::读取成员(const 场景成员概念请求_v2& r,std::uint64_t g,std::uint64_t h) const {
    using namespace 场景特征应用内部;
    if(!g||!h||h>g||!有效(r.S.值)||!有效(r.E.值)||!有效(r.场景成员关系)||!r.场景关系预算)拒绝();
        const 场景历史身份请求 rq{2,g,h,r.S.值};auto role=scenes_.读取场景角色历史(rq);
        if(role.状态!=场景角色数据状态::已读取)switch(role.状态){
        case 场景角色数据状态::入口拒绝:拒绝();
        case 场景角色数据状态::未找到:case 场景角色数据状态::目标已退出:case 场景角色数据状态::场景角色未启用:拒绝(B::未找到);
        case 场景角色数据状态::引用冲突:拒绝(B::引用冲突);
        case 场景角色数据状态::事实代次漂移:拒绝(B::事实代次漂移);
        case 场景角色数据状态::历史材料已清理:拒绝(B::历史材料不可用);
        case 场景角色数据状态::数量预算不足:拒绝(B::数量预算不足);
        case 场景角色数据状态::资源失败:拒绝(B::资源失败);
        default:拒绝(B::内部不一致);
        }
        if(!role.成功(rq))拒绝(B::内部不一致);
        const auto identity=existences_.读取存在身份来源历史见证(g,h,r.E.值);
        if(identity.状态!=存在结构身份只读状态::已读取)switch(identity.状态){
        case 存在结构身份只读状态::入口拒绝:拒绝();
        case 存在结构身份只读状态::未找到:case 存在结构身份只读状态::目标已退出:拒绝(B::未找到);
        case 存在结构身份只读状态::事实代次漂移:拒绝(B::事实代次漂移);
        case 存在结构身份只读状态::历史材料已清理:拒绝(B::历史材料不可用);
        case 存在结构身份只读状态::数量预算不足:拒绝(B::数量预算不足);
        case 存在结构身份只读状态::资源失败:拒绝(B::资源失败);
        default:拒绝(B::内部不一致);
        }
        if(!identity.成功(g,h,r.E.值))拒绝(B::内部不一致);
        const auto memberRead=scenes_.读取直接存在成员历史({1,g,h,r.场景成员关系});const auto& head=memberRead.结果头;
        if(head.状态!=场景直接包含状态::已读取)switch(head.状态){
        case 场景直接包含状态::入口拒绝:拒绝();
        case 场景直接包含状态::场景未找到:case 场景直接包含状态::场景已退出:case 场景直接包含状态::成员未找到:
        case 场景直接包含状态::成员已退出:case 场景直接包含状态::成员未归属:拒绝(B::未找到);
        case 场景直接包含状态::引用冲突:case 场景直接包含状态::原父不匹配:拒绝(B::引用冲突);
        case 场景直接包含状态::成员多重归属:拒绝(B::多义);
        case 场景直接包含状态::事实代次漂移:拒绝(B::事实代次漂移);
        case 场景直接包含状态::历史材料已清理:拒绝(B::历史材料不可用);
        case 场景直接包含状态::数量预算不足:拒绝(B::数量预算不足);
        case 场景直接包含状态::资源失败:拒绝(B::资源失败);
        default:拒绝(B::内部不一致);
        }
        if(head.版本!=1||head.Gread!=g||head.H!=h||head.首次发布H||!memberRead.成功())拒绝(B::内部不一致);
        const auto& m=*memberRead.包含;const auto& edge=m.关系;
        if(m.Gread!=g||m.H!=h||m.种类!=场景直接包含种类::存在成员||!有效(edge.关系类型)
            ||edge.角色或顺序!=1||!edge.生命周期.创建事实代次||edge.生命周期.创建事实代次>h||edge.生命周期.退出事实代次)拒绝(B::内部不一致);
        if(m.父场景!=r.S.值||m.成员!=r.E.值||edge.编码!=r.场景成员关系||edge.源!=r.S.值||edge.目标!=r.E.值)拒绝(B::引用冲突);
        守卫(concepts_,g);return m;
}
void 场景成员概念应用服务::补全特征(场景特征概念结果_v2& out,const 场景成员概念请求_v2& r,std::uint64_t g) const {
    using namespace 场景特征应用内部;
    const auto& child=out.处理;
    if(child.状态!=特征概念处理状态::完成||child.准确匹配.empty()||!child.概念||!child.名称||!child.观察)拒绝(B::内部不一致);
    if(child.准确匹配.size()!=1)拒绝(B::多义);
    const auto f=child.准确匹配.front();
    const auto locationRead=features_.读取已发布类型观察(r.观察,g);
    if(!locationRead.定位结果||!locationRead.定位结果->成功()||!child.定位结果||!child.定位结果->成功()
        ||locationRead.定位结果->事实!=child.定位结果->事实)拒绝(B::定位拒绝);
    out.处理.定位结果=locationRead.定位结果;
    if((child.观察->输入.键.类型!=r.观察.观察.类型||child.观察->输入.键.来源!=r.观察.观察.来源||child.观察->输入.键.序号!=r.观察.观察.序号)||child.观察->输入.时间!=r.观察.时间
        ||child.观察->输入.F!=f||(r.观察.指定F&&*r.观察.指定F!=f))拒绝(B::幂等冲突);
    auto match=concepts_.判定应用特征模板({2,{1,g,g},child.概念->身份,{1,g,g,f},共享预算(r.观察.预算)});
    if(!match.成功())拒绝(映射(match.状态));
    const auto& a=*match.数据;const auto* c=std::get_if<特征概念事实>(&a.模板);
    if(match.Gread!=g||match.H!=g||!c||c->身份!=child.概念->身份
        ||a.判定.Gread!=g||a.判定.模板H!=g||a.判定.实际H!=g
        ||a.判定.实际.信息.身份!=f||a.判定.实际.信息.类型!=r.观察.观察.类型
        ||!特征有效(a.判定.实际,g,g)||!域同义(a.判定.域,c->定义.域)||a.适用!=a.判定.命中)拒绝(B::内部不一致);
    if(!a.适用)拒绝(B::引用冲突);
    if(!同概念(*c,*child.概念))拒绝(B::引用冲突);
    out.Gread=g;out.H=g;out.已确认F=a.判定.实际;out.已确认C=*c;
    auto name=concepts_.读取特征当前名称({2,{1,g,g},f,r.观察.预算.基础});
    if(!name.成功())拒绝(映射(name.状态));
    if(name.Gread!=g||name.H!=g)拒绝(B::内部不一致);
    if(!*name.数据||!同名称(**name.数据,*child.名称))拒绝(B::引用冲突);
    out.已确认名称=**name.数据;守卫(concepts_,g);
    if(!out.成功())拒绝(B::内部不一致);
}
bool 场景成员概念应用服务::消费特征结果(场景成员概念结果_v2& out,const 场景成员概念请求_v2& r,
    特征概念处理结果 child,const 场景成员概念继续材料_v2* original){
    using namespace 场景特征应用内部;
    if(original)out.继续=*original;
    out.特征概念.处理=std::move(child);const auto& f=out.特征概念.处理;
    if(f.状态==特征概念处理状态::定位拒绝){out.状态=B::定位拒绝;return false;}
    if(f.状态==特征概念处理状态::定位待原请求确认||f.状态==特征概念处理状态::准确F已定位概念未完成){
        if(!out.继续)out.继续=场景成员概念继续材料_v2{2,P::特征应用,r,std::nullopt};
        if(f.定位结果&&f.定位结果->成功())out.继续->已确认定位=f.定位结果;
        out.状态=f.状态==特征概念处理状态::定位待原请求确认 ? B::定位待原请求确认 : B::准确F已定位概念未完成;return false;
    }
    if(f.状态==特征概念处理状态::需选择准确F){out.状态=B::需选择准确F;return false;}
    if(f.状态==特征概念处理状态::待原请求确认||f.待确认请求){
        if(!original)out.继续=场景成员概念继续材料_v2{2,P::特征应用,r,std::nullopt};
        out.状态=B::待特征应用收敛;return false;
    }
    if(f.状态!=特征概念处理状态::完成){
        if(!original&&(f.观察||!f.准确匹配.empty()))
            out.继续=场景成员概念继续材料_v2{2,P::特征应用,r,std::nullopt};
        if(f.原因)out.状态=std::visit([](auto s){return 映射(s);},*f.原因);
        else switch(f.状态){
            case 特征概念处理状态::准确F已成立名称未完成:out.状态=B::准确F已成立名称未完成;break;
            case 特征概念处理状态::规则缺失:out.状态=B::规则缺失;break;
            case 特征概念处理状态::材料缺失:out.状态=B::未找到;break;
            case 特征概念处理状态::旧格式不支持:out.状态=B::旧格式不支持;break;
            case 特征概念处理状态::预算不足:out.状态=B::数量预算不足;break;
            case 特征概念处理状态::入口拒绝:out.状态=B::入口拒绝;break;
            case 特征概念处理状态::资源失败:out.状态=B::资源失败;break;
            default:out.状态=B::内部不一致;break;
        }
        return false;
    }
    if(!original)out.继续=场景成员概念继续材料_v2{2,r.采用 ? P::特征应用 : P::最终读回,r,std::nullopt};
    if(f.定位结果&&f.定位结果->成功())out.继续->已确认定位=f.定位结果;
    补全特征(out.特征概念,r,当前(concepts_));return true;
}
void 场景成员概念应用服务::处理采用(场景成员概念结果_v2& out,const 场景成员概念请求_v2& r){
    using namespace 场景特征应用内部;
    if(!r.采用)return;
    if(!out.特征概念.成功())拒绝(B::内部不一致);
    const auto memberG=当前(concepts_);(void)读取成员(r,memberG,r.H);if(r.H!=memberG)(void)读取成员(r,memberG,memberG);
    const auto& f=*out.特征概念.已确认F;const auto& a=*r.采用;
    const 存在当前采用写请求 request{1,当前(concepts_),a.幂等身份,r.E.值,f.信息.类型,
        存在关联已知并采用{f.信息.身份,a.原采用预期},a.关系预算};
    out.继续=场景成员概念继续材料_v2{2,P::存在采用,r,request,out.特征概念.处理.定位结果};
    out.当前采用=existences_.变更当前采用(request);
    if(!out.当前采用->成功())拒绝(映射(out.当前采用->状态));
    if(!out.当前采用->原请求||*out.当前采用->原请求!=request||!out.当前采用->采用
        ||!采用一致(*out.当前采用->采用,r.E,f))拒绝(B::内部不一致);
    out.继续=场景成员概念继续材料_v2{2,P::最终读回,r,std::nullopt,out.特征概念.处理.定位结果};
}
void 场景成员概念应用服务::完成投影(场景成员概念结果_v2& out,const 场景成员概念请求_v2& r) const {
    using namespace 场景特征应用内部;
    const auto g=当前(concepts_);if(r.H!=g)(void)读取成员(r,g,r.H);场景组合投影_v2 p;
    p.Gread=p.H=g;p.S=r.S;p.E=r.E;p.场景成员=读取成员(r,g,g);
    auto finalFeature=out.特征概念;补全特征(finalFeature,r,g);
    if(!out.特征概念.成功()||!同特征事实(*out.特征概念.已确认F,*finalFeature.已确认F)
        ||out.特征概念.已确认C!=finalFeature.已确认C||out.特征概念.已确认名称!=finalFeature.已确认名称)拒绝(B::引用冲突);
    p.F=*finalFeature.已确认F;p.C=*finalFeature.已确认C;p.名称=*finalFeature.已确认名称;p.定位=*finalFeature.处理.定位结果->事实;
    if(r.采用){
        if(!out.当前采用||!out.当前采用->成功()||!out.当前采用->采用)拒绝(B::当前采用未完成);
        const auto a=existences_.读取当前采用({1,g,g,r.E.值,p.F.信息.类型,r.采用->关系预算});
        if(!a.成功())拒绝(映射(a.状态));
        if(a.Gread!=g||a.H!=g)拒绝(B::内部不一致);
        if(a.采用!=out.当前采用->采用)拒绝(B::引用冲突);p.当前采用=a.采用;
    }
    守卫(concepts_,g);if(p.场景成员.关系.编码!=r.场景成员关系||!p.完整())拒绝(B::内部不一致);
    out.Gread=out.H=g;out.最终投影=std::move(p);out.继续.reset();out.状态=B::完成;
    if(!out.成功())拒绝(B::内部不一致);
}
场景成员概念结果_v2 场景成员概念应用服务::处理场景成员概念(const 场景成员概念请求_v2& r){
    using namespace 场景特征应用内部;
    场景成员概念结果_v2 out;out.S=r.S;out.E=r.E;out.Gread=r.Gread;out.H=r.H;
    try{
        验证入口(r);(void)读取成员(r,r.Gread,r.H);if(r.H!=r.Gread)(void)读取成员(r,r.Gread,r.Gread);
        if(!消费特征结果(out,r,features_.处理类型观察(r.观察)))return out;
        处理采用(out,r);完成投影(out,r);
    }catch(const 失败& e){out.状态=e.状态;out.最终投影.reset();}
    catch(const std::bad_alloc&){out.状态=B::资源失败;out.最终投影.reset();}
    catch(const std::length_error&){out.状态=B::资源失败;out.最终投影.reset();}
    catch(...){out.状态=B::内部不一致;out.最终投影.reset();}
    return out;
}
场景成员概念结果_v2 场景成员概念应用服务::收敛场景成员概念(const 场景成员概念继续材料_v2& m){
    using namespace 场景特征应用内部;
    const auto& r=m.原请求;
    场景成员概念结果_v2 out;out.S=r.S;out.E=r.E;out.Gread=r.Gread;out.H=r.H;
    try{
        out.继续=m;验证入口(r);
        if(m.版本!=2)拒绝();
        switch(m.阶段){
        case P::特征应用:case P::最终读回:if(m.采用原请求)拒绝();break;
        case P::存在采用:if(!r.采用||!m.采用原请求)拒绝();break;
        default:拒绝();
        }
        const auto g=当前(concepts_);if(g<r.Gread)拒绝(B::事实代次漂移);
        if(m.采用原请求){
            const auto& q=*m.采用原请求;const auto& intent=*r.采用;
            const auto* op=std::get_if<存在关联已知并采用>(&q.操作);
            if(q.版本!=1||!q.G||q.G<r.Gread||q.G>g||q.幂等身份!=intent.幂等身份
                ||q.E!=r.E.值||q.FT!=r.观察.观察.类型||q.关系预算!=intent.关系预算
                ||!op||!有效(op->F)||op->预期!=intent.原采用预期
                ||(r.观察.指定F&&op->F!=*r.观察.指定F))拒绝(B::幂等冲突);
        }
        // 恢复读原 H 的成员事实，但读取封套使用当前 G，不伪造原 G 仍是当前。
        out.继续=m;(void)读取成员(r,g,r.H);if(r.H!=g)(void)读取成员(r,g,g);
        if(m.阶段==P::特征应用){
            auto child=m.已确认定位 ? features_.收敛类型观察(r.观察,*m.已确认定位) : features_.收敛类型观察(r.观察);
            if(!消费特征结果(out,r,std::move(child),&m))return out;
        }else if(m.阶段==P::存在采用){
            if(!消费特征结果(out,r,features_.读取已发布类型观察(r.观察,g),&m))return out;
        }else{
            // 最终读回也只核验已发布观察，不重新进入命中、名称等写入流程。
            if(!消费特征结果(out,r,features_.读取已发布类型观察(r.观察,g),&m))return out;
        }
        const auto& f=*out.特征概念.已确认F;
        if(m.阶段==P::存在采用){
            const auto& q=*m.采用原请求;
            const auto& op=std::get<存在关联已知并采用>(q.操作);
            if(q.FT!=f.信息.类型||op.F!=f.信息.身份)拒绝(B::幂等冲突);
            const auto memberG=当前(concepts_);(void)读取成员(r,memberG,r.H);if(r.H!=memberG)(void)读取成员(r,memberG,memberG);
            out.当前采用=existences_.收敛当前采用(q);
            if(!out.当前采用->成功())拒绝(映射(out.当前采用->状态));
            if(!out.当前采用->原请求||*out.当前采用->原请求!=q||!out.当前采用->采用
                ||!采用一致(*out.当前采用->采用,r.E,f))拒绝(B::内部不一致);
            out.继续=场景成员概念继续材料_v2{2,P::最终读回,r,std::nullopt,out.特征概念.处理.定位结果};
        }else if(m.阶段==P::特征应用){
            处理采用(out,r);
        }else if(r.采用){
            // 最终读回阶段已经完成采用；只读，不以新 G 重造写请求或幂等键。
            const auto finalG=当前(concepts_);
            out.当前采用=existences_.读取当前采用({1,finalG,finalG,r.E.值,f.信息.类型,r.采用->关系预算});
            if(!out.当前采用->成功())拒绝(映射(out.当前采用->状态));
            if(out.当前采用->状态!=存在类数据状态::已读取||out.当前采用->Gread!=finalG
                ||out.当前采用->H!=finalG)拒绝(B::内部不一致);
            if(!out.当前采用->采用||!采用一致(*out.当前采用->采用,r.E,f))拒绝(B::当前采用未完成);
        }
        完成投影(out,r);
    }catch(const 失败& e){out.状态=e.状态;out.最终投影.reset();}
    catch(const std::bad_alloc&){out.状态=B::资源失败;out.最终投影.reset();}
    catch(const std::length_error&){out.状态=B::资源失败;out.最终投影.reset();}
    catch(...){out.状态=B::内部不一致;out.最终投影.reset();}
    return out;
}
}
