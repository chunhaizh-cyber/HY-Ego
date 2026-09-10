module;
#include <algorithm>
#include <bit>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>
export module 海中鱼巣.业务.应用服务.需求目标方向;
export import 海中鱼巣.业务.计算.二次特征;
export import 海中鱼巣.领域.数据服务.需求类;
export namespace 海中鱼巣 {
inline constexpr std::uint32_t 需求目标方向合同版本 = 1;
enum class 需求目标方向状态 : std::uint8_t {
    已计算=1, 入口拒绝=2, 需求未找到=3, 需求已退出=4,
    目标宿主未找到=5, 目标宿主已退出=6, 当前F未找到=7,
    当前F已退出=8, 目标F未找到=9, 目标F已退出=10,
    当前F不属于宿主=11, 目标F不属于宿主=12, 当前采用不匹配=13,
    方向定义未找到=14, 方向定义已退出=15, 方向定义不匹配=16,
    方向来源不匹配=17, 方向计算失败=18, 历史材料不可用=19,
    事实代次漂移=20, 预算不足=21, 资源失败=22, 内部不一致=23
};
struct 需求目标方向请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t G = 0, 请求身份 = 0;
    需求类记录身份 需求;
    特征信息身份 当前F;
    std::uint8_t 要求结果位 = 0;
    二次计算预算 预算;
    friend bool operator==(const 需求目标方向请求&, const 需求目标方向请求&) = default;
};
struct 需求目标方向结果 final {
    std::uint32_t 版本 = 1;
    需求目标方向状态 状态 = 需求目标方向状态::入口拒绝;
    需求目标方向请求 原请求;
    std::uint64_t G = 0;
    需求类记录身份 需求;
    稳定编码 目标宿主{};
    特征信息身份 当前F, 目标F;
    特征类定义身份 方向定义;
    std::optional<二次准确计算结果> 计算;
    bool 成功() const noexcept;
    friend bool operator==(const 需求目标方向结果&, const 需求目标方向结果&) = default;
};

bool 需求目标方向结果::成功() const noexcept {
    const auto& r=原请求;
    if(版本!=1||状态!=需求目标方向状态::已计算||r.版本!=1||!r.G||!r.请求身份
        ||!有效(r.需求.值)||!有效(r.当前F)||r.要求结果位<1||r.要求结果位>7||!r.预算.有效()
        ||G!=r.G||需求!=r.需求||当前F!=r.当前F||!有效(目标宿主)||!有效(目标F)||!有效(方向定义.结点)
        ||!计算||!计算->成功()||计算->G!=G||计算->H!=G||计算->请求身份!=r.请求身份
        ||计算->上下文!=二次计算上下文{}||计算->根输出组.size()!=std::popcount(r.要求结果位))return false;
    std::size_t index=0;
    for(unsigned role=1;role<=3;++role)if(r.要求结果位&(1U<<(role-1))) {
        const auto* root=std::get_if<二次已保存定义输出来源>(&计算->根输出组[index++].来源);
        if(!root||root->定义!=方向定义||static_cast<unsigned>(root->输出角色)!=role)return false;
    }
    const 二次计算项回执* direction=nullptr;
    for(const auto& node:计算->计算项组){
        const auto* identity=std::get_if<二次已保存定义节点身份>(&node.节点);
        if(identity&&identity->定义==方向定义){if(direction)return false;direction=&node;}
    }
    if(!direction||direction->K.用途!=特征I64比较用途::目标判断
        ||direction->输入[0].角色!=特征I64输入角色::当前事实
        ||direction->输入[1].角色!=特征I64输入角色::目标值)return false;
    auto reaches=[&](auto&& self,const 二次计算来源& source,特征信息身份 id,std::size_t depth)->bool {
        if(depth>计算->计算项组.size())return false;
        if(const auto* leaf=std::get_if<二次准确F来源>(&source))return leaf->F==id;
        if(source.valueless_by_exception())return false;
        std::optional<二次计算节点身份> identity;
        if(const auto* saved=std::get_if<二次已保存定义输出来源>(&source))
            identity=二次已保存定义节点身份{saved->定义};
        else if(const auto* local=std::get_if<二次本次输出来源>(&source))
            identity=二次本次节点身份{local->局部编号};
        if(!identity)return false;
        const 二次计算项回执* found=nullptr;
        for(const auto& node:计算->计算项组)if(node.节点==*identity){if(found)return false;found=&node;}
        return found&&(self(self,found->输入[0].来源,id,depth+1)||self(self,found->输入[1].来源,id,depth+1));
    };
    return reaches(reaches,direction->输入[0].来源,当前F,0)&&reaches(reaches,direction->输入[1].来源,目标F,0);
}

class 需求目标方向应用服务 final {
    const 需求类数据服务& demand_;
    const 存在类数据服务& existence_;
    const 特征类数据服务& feature_;
    const 二次特征计算应用服务& calculation_;
    using S=需求目标方向状态;
    struct 失败{S 状态;};
    static void 要求(bool ok,S s=S::内部不一致){if(!ok)throw 失败{s};}
    static S 映射(需求类数据状态 s) noexcept {
        using D=需求类数据状态;
        switch(s) {
        case D::入口拒绝:return S::入口拒绝;case D::未找到:return S::需求未找到;
        case D::目标宿主未找到:return S::目标宿主未找到;case D::目标宿主已退出:return S::目标宿主已退出;
        case D::静态目标特征未找到:return S::目标F未找到;case D::静态目标特征已退出:return S::目标F已退出;
        case D::当前事实特征未找到:return S::当前F未找到;case D::当前事实特征已退出:return S::当前F已退出;
        case D::目标特征不属于宿主:return S::目标F不属于宿主;case D::当前特征不属于宿主:return S::当前F不属于宿主;
        case D::方向定义未找到:return S::方向定义未找到;case D::方向定义已退出:return S::方向定义已退出;
        case D::方向定义不匹配:return S::方向定义不匹配;case D::方向来源不匹配:return S::方向来源不匹配;
        case D::数量预算不足:return S::预算不足;case D::事实代次漂移:return S::事实代次漂移;
        case D::资源失败:return S::资源失败;default:return S::内部不一致;
        }
    }
    static S 映射(存在类数据状态 s) noexcept {
        using E=存在类数据状态;
        switch(s) {
        case E::未找到:return S::目标宿主未找到;case E::目标已退出:return S::目标宿主已退出;
        case E::成员未找到:return S::当前采用不匹配;case E::事实代次漂移:return S::事实代次漂移;
        case E::数量预算不足:return S::预算不足;case E::资源失败:return S::资源失败;
        case E::历史材料已清理:return S::历史材料不可用;default:return S::内部不一致;
        }
    }
    static S 映射(特征类标量状态 s) noexcept {
        using D=特征类标量状态;
        switch(s) {
        case D::未找到:return S::方向定义未找到;case D::已退出:return S::方向定义已退出;
        case D::来源不匹配:return S::方向来源不匹配;
        case D::类型不匹配:case D::格式不支持:return S::方向定义不匹配;
        case D::预算不足:return S::预算不足;case D::历史材料不可用:return S::历史材料不可用;
        case D::事实代次漂移:return S::事实代次漂移;case D::资源失败:return S::资源失败;
        default:return S::内部不一致;
        }
    }
    static S 映射(特征数据错误 s,bool target) noexcept {
        switch(s) {
        case 特征数据错误::未找到:return target ? S::目标F未找到:S::当前F未找到;
        case 特征数据错误::已退出:return target ? S::目标F已退出:S::当前F已退出;
        case 特征数据错误::类型不相容:case 特征数据错误::旧格式不支持:return S::方向定义不匹配;
        case 特征数据错误::数量预算不足:return S::预算不足;
        case 特征数据错误::历史材料不可用:return S::历史材料不可用;
        case 特征数据错误::并发变化:return S::事实代次漂移;
        case 特征数据错误::资源失败:return S::资源失败;default:return S::内部不一致;
        }
    }
    void 守卫(std::uint64_t g) const {
        const auto read=demand_.核验当前事实代次(g);
        if(read.状态==需求类数据状态::入口拒绝)throw 失败{S::入口拒绝};
        if(read.状态==需求类数据状态::事实代次漂移)throw 失败{S::事实代次漂移};
        if(read.状态==需求类数据状态::资源失败)throw 失败{S::资源失败};
        要求(read.成功(),S::内部不一致);
    }
public:
    需求目标方向应用服务(const 需求类数据服务& demand,const 存在类数据服务& existence,
        const 特征类数据服务& feature,const 二次特征计算应用服务& calculation)
        :demand_(demand),existence_(existence),feature_(feature),calculation_(calculation) {
        if(!demand_.与存在服务同底座(existence_)||!demand_.与特征服务同底座(feature_)
            ||!calculation_.与特征服务同底座(feature_))throw std::invalid_argument("需求方向数据绑定");
    }
    需求目标方向应用服务()=delete;
    需求目标方向应用服务(const 需求目标方向应用服务&)=delete;
    需求目标方向应用服务& operator=(const 需求目标方向应用服务&)=delete;
    需求目标方向应用服务(需求目标方向应用服务&&)=delete;
    需求目标方向应用服务& operator=(需求目标方向应用服务&&)=delete;
    bool 与需求服务同底座(const 需求类数据服务& demand) const noexcept {
        try { return demand_.与需求服务同底座(demand); }
        catch (...) { return false; }
    }
    // 结构读取与当前采用核验只产生本次方向依据；诊断向上送出，不写需求。
    需求目标方向结果 读取并计算(const 需求目标方向请求& r) const noexcept {
        需求目标方向结果 out;out.原请求=r;out.G=r.G;out.需求=r.需求;out.当前F=r.当前F;
        auto fail=[&](S s){
            out.状态=s;
            if(out.计算&&out.计算->状态==二次计算状态::已计算) {
                out.计算->状态=s==S::事实代次漂移 ? 二次计算状态::事实代次漂移:二次计算状态::内部不一致;
                out.计算->根输出组.clear();out.计算->基础叶组.clear();out.计算->计算项组.clear();out.计算->结果组.clear();
            }
        };
        try {
            要求(r.版本==1&&r.G&&r.请求身份&&有效(r.需求.值)&&有效(r.当前F)
                &&r.要求结果位>=1&&r.要求结果位<=7&&r.预算.有效(),S::入口拒绝);
            守卫(r.G);
            const auto record=demand_.查询需求记录({需求类数据合同版本,r.G,r.需求});
            if(record.状态!=需求类数据状态::已读取)throw 失败{映射(record.状态)};
            要求(record.成功()&&record.事实代次==r.G&&record.记录&&record.记录->身份==r.需求);
            const auto& d=*record.记录;out.目标宿主=d.目标宿主;out.目标F={d.静态目标特征};out.方向定义=d.方向二次特征;
            const auto host=existence_.查询存在({存在类数据合同版本,r.G,d.目标宿主});
            if(host.状态!=存在类数据状态::已读取)throw 失败{映射(host.状态)};
            要求(host.成功()&&host.存在结点&&host.事实代次==r.G&&host.存在结点->结点==d.目标宿主);
            auto own=[&](稳定编码 id){
                return std::count_if(host.存在结点->特征组.begin(),host.存在结点->特征组.end(),
                    [&](const auto& member){return member.目标结点==id&&有效(member.成员关系);})==1;
            };
            auto readF=[&](特征信息身份 id,bool target) {
                auto value=feature_.读取准确特征事实({1,r.G,r.G,id});
                if(const auto* e=std::get_if<特征数据错误>(&value))throw 失败{映射(*e,target)};
                const auto f=std::get<准确特征读取事实>(value);
                要求(f.Gread==r.G&&f.H==r.G&&f.信息.身份==id&&浅层结构有效(f.信息)
                    &&f.创建G&&f.创建G<=r.G&&(!f.退出G||*f.退出G>r.G));
                return f;
            };
            const auto now=readF(r.当前F,false);const auto target=readF(out.目标F,true);
            要求(own(now.信息.身份.编码),S::当前F不属于宿主);要求(own(target.信息.身份.编码),S::目标F不属于宿主);
            const auto adoption=existence_.读取当前采用({1,r.G,r.G,d.目标宿主,now.信息.类型,r.预算.最大读取材料数});
            if(adoption.状态!=存在类数据状态::已读取)throw 失败{映射(adoption.状态)};
            要求(adoption.成功()&&adoption.Gread==r.G&&adoption.H==r.G);
            要求(adoption.采用.has_value(),S::当前采用不匹配);
            要求(adoption.采用->E==d.目标宿主&&adoption.采用->FT==now.信息.类型&&adoption.采用->F==r.当前F,S::当前采用不匹配);
            const auto budget=特征类派生读取预算{r.预算.最大图项数,r.预算.最大读取材料数,
                r.预算.最大基础叶数,r.预算.最大读取材料数,r.预算.最大展开深度,r.预算.最大读取材料数};
            const auto definition=feature_.读取标量派生定义({2,r.G,r.G,d.方向二次特征,budget});
            if(definition.状态!=特征类标量状态::已读取)throw 失败{映射(definition.状态)};
            要求(definition.成功()&&definition.Gread==r.G&&definition.H==r.G
                &&definition.定义事实->定义身份==d.方向二次特征);
            const auto& f=*definition.定义事实;
            要求(f.宿主E&&*f.宿主E==d.目标宿主&&f.注册.用途==特征类比较用途::目标判断
                &&f.注册.左角色==特征类比较角色::当前事实&&f.注册.右角色==特征类比较角色::目标状态,S::方向定义不匹配);
            要求(std::find(definition.左叶组.begin(),definition.左叶组.end(),r.当前F.编码)!=definition.左叶组.end()
                &&std::find(definition.右叶组.begin(),definition.右叶组.end(),out.目标F.编码)!=definition.右叶组.end(),S::方向来源不匹配);
            for(const auto& leaf:definition.基础叶组)要求(own(leaf.F),S::方向来源不匹配);
            二次计算请求 request;request.G=request.H=r.G;request.请求身份=r.请求身份;request.预算=r.预算;
            for(unsigned role=1;role<=3;++role)if(r.要求结果位&(1U<<(role-1)))
                request.根输出组.push_back({二次已保存定义输出来源{d.方向二次特征,static_cast<特征类标量结果角色>(role)}});
            out.计算=calculation_.计算(request);
            if(!out.计算->成功()) {
                switch(out.计算->状态) {
                case 二次计算状态::历史材料不可用:throw 失败{S::历史材料不可用};
                case 二次计算状态::事实代次漂移:throw 失败{S::事实代次漂移};
                case 二次计算状态::预算不足:throw 失败{S::预算不足};
                case 二次计算状态::资源失败:throw 失败{S::资源失败};
                default:throw 失败{S::方向计算失败};
                }
            }
            要求(out.计算->根输出组==request.根输出组);
            for(const auto& leaf:out.计算->基础叶组) {
                const auto it=std::find_if(definition.基础叶组.begin(),definition.基础叶组.end(),[&](const auto& x){return x.F==leaf.事实.信息.身份.编码;});
                要求(it!=definition.基础叶组.end()&&it->完整值==leaf.事实.完整值&&it->类型关系==leaf.事实.类型关系
                    &&it->值事实==leaf.事实.准确值事实&&it->FT==leaf.事实.信息.类型.编码,S::方向来源不匹配);
            }
            守卫(r.G);out.状态=S::已计算;要求(out.成功());
        } catch(const 失败& e){fail(e.状态);}
        catch(const std::bad_alloc&){fail(S::资源失败);}
        catch(const std::length_error&){fail(S::资源失败);}
        catch(...){fail(S::内部不一致);}
        return out;
    }
};
}
