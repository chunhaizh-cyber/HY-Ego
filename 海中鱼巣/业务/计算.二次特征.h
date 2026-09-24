#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <new>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include "../领域/算法.有序I64特征比较.h"
namespace 海中鱼巣 {
inline constexpr std::uint32_t 二次特征计算合同版本 = 1;
enum class 二次计算状态 : std::uint8_t {
    已计算=1, 入口拒绝=2, 来源缺失=3, 历史材料不可用=4,
    事实冲突=5, 未注册=6, 算法未实现=7, 类型不匹配=8,
    单位量化不匹配=9, 参照不相容=10, 运算溢出=11,
    预算不足=12, 事实代次漂移=13, 固定规则不兼容=14,
    结果范围不满足=15, 资源失败=16, 内部不一致=17
};
struct 二次准确F来源 final { 特征信息身份 F;     friend bool operator==(const 二次准确F来源&, const 二次准确F来源&) = default;
};
struct 二次已保存定义输出来源 final {
    特征类定义身份 定义;
    特征类标量结果角色 输出角色 = 特征类标量结果角色::排序;
    friend bool operator==(const 二次已保存定义输出来源&, const 二次已保存定义输出来源&) = default;
};
struct 二次本次输出来源 final {
    std::uint64_t 局部编号 = 0;
    特征类标量结果角色 输出角色 = 特征类标量结果角色::排序;
    friend bool operator==(const 二次本次输出来源&, const 二次本次输出来源&) = default;
};
struct 二次已保存定义节点身份 final { 特征类定义身份 定义;     friend bool operator==(const 二次已保存定义节点身份&, const 二次已保存定义节点身份&) = default;
};
struct 二次本次节点身份 final { std::uint64_t 局部编号 = 0;     friend bool operator==(const 二次本次节点身份&, const 二次本次节点身份&) = default;
};
using 二次计算节点身份 = std::variant<二次已保存定义节点身份,
    二次本次节点身份>;
using 二次计算来源 = std::variant<二次准确F来源,
    二次已保存定义输出来源, 二次本次输出来源>;
struct 二次计算输入 final {
    std::uint32_t 顺序 = 0;
    特征I64输入角色 角色 = 特征I64输入角色::识别左;
    二次计算来源 来源;
    friend bool operator==(const 二次计算输入&, const 二次计算输入&) = default;
};
struct 二次计算项 final {
    std::uint64_t 局部编号 = 0;
    特征类型身份 输入FT;
    特征I64比较用途 用途 = 特征I64比较用途::识别区分;
    std::optional<特征I64比较绑定身份> 预期K;
    std::array<二次计算输入, 2> 输入;
    std::uint8_t 要求结果位 = 0;
    friend bool operator==(const 二次计算项&, const 二次计算项&) = default;
};
struct 二次根输出 final {
    std::variant<二次已保存定义输出来源, 二次本次输出来源> 来源;
    friend bool operator==(const 二次根输出&, const 二次根输出&) = default;
};
struct 二次计算上下文 final {
    std::optional<稳定编码> 参与者A, 参与者B;
    std::optional<std::int64_t> 左时间, 右时间;
    std::optional<稳定编码> 参照;
    friend bool operator==(const 二次计算上下文&, const 二次计算上下文&) = default;
};
struct 二次计算预算 final {
    std::uint64_t 最大图项数=0, 最大来源边数=0, 最大基础叶数=0,
        最大读取材料数=0, 最大展开深度=0, 最大结果项数=0;
    bool 有效() const noexcept { return 最大图项数&&最大来源边数&&最大基础叶数&&最大读取材料数&&最大展开深度&&最大结果项数; }
    friend bool operator==(const 二次计算预算&, const 二次计算预算&) = default;
};
struct 二次计算请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t G = 0, H = 0, 请求身份 = 0;
    std::vector<二次计算项> 图项组;
    std::vector<二次根输出> 根输出组;
    二次计算上下文 上下文;
    二次计算预算 预算;
    friend bool operator==(const 二次计算请求&, const 二次计算请求&) = default;
};
struct 二次基础叶回执 final { 准确特征读取事实 事实;     friend bool operator==(const 二次基础叶回执&, const 二次基础叶回执&) = default;
};
struct 二次计算输入回执 final {
    std::uint32_t 顺序 = 0;
    特征I64输入角色 角色 = 特征I64输入角色::识别左;
    二次计算来源 来源;
    特征类型身份 实际FT;
    std::int64_t 值 = 0;
    std::uint32_t 实际阶次 = 0;
    friend bool operator==(const 二次计算输入回执&, const 二次计算输入回执&) = default;
};
struct 二次计算输出回执 final {
    二次计算节点身份 节点;
    特征I64比较绑定身份 K;
    特征类型身份 输出FT;
    特征类标量结果角色 输出角色 = 特征类标量结果角色::排序;
    std::int64_t 值 = 0;
    特征类标量量化合同 量化;
    std::uint32_t 真实阶次 = 0;
    friend bool operator==(const 二次计算输出回执&, const 二次计算输出回执&) = default;
};
struct 二次计算项回执 final {
    二次计算节点身份 节点;
    有序I64比较合同快照 K;
    std::array<二次计算输入回执, 2> 输入;
    std::vector<二次计算输出回执> 输出组;
    std::uint32_t 真实阶次 = 0;
    friend bool operator==(const 二次计算项回执&, const 二次计算项回执&) = default;
};
struct 二次准确计算结果 final {
    std::uint32_t 版本 = 1;
    二次计算状态 状态 = 二次计算状态::入口拒绝;
    std::uint64_t G = 0, H = 0, 请求身份 = 0;
    二次计算上下文 上下文;
    std::vector<二次根输出> 根输出组;
    std::vector<二次基础叶回执> 基础叶组;
    std::vector<二次计算项回执> 计算项组;
    std::vector<二次计算输出回执> 结果组;
    bool 成功() const noexcept;
    friend bool operator==(const 二次准确计算结果&, const 二次准确计算结果&) = default;
};

namespace 二次计算内部 {
using 节点键=std::pair<bool,std::uint64_t>;
节点键 键(const 二次计算节点身份& id);
二次计算节点身份 身份(节点键 k);
二次计算来源 来源(const 二次根输出& r);
节点键 输出键(const 二次计算来源& s);
特征类标量结果角色 输出角色(const 二次计算来源& s);
unsigned 角色位(特征类标量结果角色 r) noexcept;
unsigned 上下文位(const 二次计算上下文& c) noexcept;
bool 上下文有效(const 二次计算上下文& c) noexcept;
bool 来源有效(const 二次计算来源& s) noexcept;
std::optional<std::int64_t> 整数(const 准确特征读取事实& f) noexcept;
bool 准确来源完整(const 准确特征读取事实& f) noexcept;
bool 快照完整(const 有序I64比较合同快照& k);
bool 同量化(const 特征类标量量化合同& a,const 特征类标量量化合同& b) noexcept;
有序I64比较合同快照 快照(const 特征I64比较绑定事实& f,std::uint64_t g);
bool 固定相容(const 有序I64比较合同快照& a,const 有序I64比较合同快照& b) noexcept;
}

// 每次调用只持有局部图与局部读取缓存；无写端口、owner 或跨请求缓存。
class 二次特征计算应用服务 final {
    const 特征类数据服务& feature_;
    const 有序I64特征比较提供者& provider_;
    using S=二次计算状态;
    struct 失败 {S 状态;};
    static void 要求(bool condition,S state=S::内部不一致){if(!condition)throw 失败{state};}
    void 守卫(std::uint64_t g) const {
        const auto r=feature_.核验当前事实代次(g);
        if(r.状态==特征类标量状态::入口拒绝)throw 失败{S::入口拒绝};
        if(r.状态==特征类标量状态::事实代次漂移)throw 失败{S::事实代次漂移};
        if(r.状态==特征类标量状态::资源失败)throw 失败{S::资源失败};
        要求(r.成功(),S::内部不一致);
    }
    static S 映射(特征类标量状态 s) noexcept {
        switch(s) {
        case 特征类标量状态::未找到:case 特征类标量状态::已退出:return S::来源缺失;
        case 特征类标量状态::历史材料不可用:return S::历史材料不可用;
        case 特征类标量状态::预算不足:return S::预算不足;
        case 特征类标量状态::事实代次漂移:return S::事实代次漂移;
        case 特征类标量状态::资源失败:return S::资源失败;
        case 特征类标量状态::类型不匹配:case 特征类标量状态::格式不支持:return S::类型不匹配;
        case 特征类标量状态::单位量化不匹配:return S::单位量化不匹配;
        case 特征类标量状态::算法不支持:return S::算法未实现;
        default:return S::内部不一致;
        }
    }
public:
    二次特征计算应用服务(const 特征类数据服务& feature,const 有序I64特征比较提供者& provider)
        :feature_(feature),provider_(provider) {}
    二次特征计算应用服务()=delete;
    二次特征计算应用服务(const 二次特征计算应用服务&)=delete;
    二次特征计算应用服务& operator=(const 二次特征计算应用服务&)=delete;
    二次特征计算应用服务(二次特征计算应用服务&&)=delete;
    二次特征计算应用服务& operator=(二次特征计算应用服务&&)=delete;
    bool 与特征服务同底座(const 特征类数据服务& feature) const noexcept {
        try { return feature_.与特征服务同底座(feature); }
        catch (...) { return false; }
    }
    二次准确计算结果 计算(const 二次计算请求& r) const noexcept {
        using namespace 二次计算内部;
        二次准确计算结果 out;out.版本=1;out.G=r.G;out.H=r.H;out.请求身份=r.请求身份;out.上下文=r.上下文;
        auto fail=[&](S s){out.状态=s;out.根输出组.clear();out.基础叶组.clear();out.计算项组.clear();out.结果组.clear();};
        try {
            要求(r.版本==1&&r.G&&r.H&&r.H<=r.G&&r.请求身份&&r.预算.有效()&&上下文有效(r.上下文)
                &&!r.根输出组.empty(),S::入口拒绝);
            要求(r.图项组.size()<=r.预算.最大图项数&&r.根输出组.size()<=r.预算.最大结果项数,S::预算不足);
            守卫(r.G);
            auto remaining=r.预算;
            auto take=[&](std::uint64_t& amount,std::uint64_t used){要求(used<=amount,S::预算不足);amount-=used;};
            take(remaining.最大图项数,r.图项组.size());
            std::map<std::uint64_t,const 二次计算项*> locals;
            for(const auto& item:r.图项组) {
                要求(item.局部编号&&有效(item.输入FT)&&item.要求结果位>=1&&item.要求结果位<=7
                    &&(!item.预期K||有效(*item.预期K))&&locals.emplace(item.局部编号,&item).second,S::入口拒绝);
                for(unsigned i=0;i<2;++i)要求(item.输入[i].顺序==i+1&&来源有效(item.输入[i].来源),S::入口拒绝);
            }
            for(const auto& [id,item]:locals)for(const auto& input:item->输入)
                if(const auto* local=std::get_if<二次本次输出来源>(&input.来源))要求(locals.contains(local->局部编号),S::入口拒绝);
            std::map<std::uint64_t,准确特征读取事实> leaves;
            std::map<std::uint64_t,特征类标量派生事实> saved;
            std::map<std::pair<std::uint64_t,unsigned>,有序I64比较合同快照> current;
            std::map<节点键,二次计算项回执> nodes;
            std::map<节点键,unsigned> needed;
            std::map<节点键,std::uint64_t> heights;
            std::set<节点键> gray,done;
            std::vector<节点键> postorder;
            std::set<std::pair<节点键,unsigned>> roots;
            auto loadLeaf=[&](特征信息身份 id)->const 准确特征读取事实& {
                if(leaves.contains(id.编码.值))return leaves.at(id.编码.值);
                要求(remaining.最大基础叶数&&remaining.最大读取材料数,S::预算不足);
                const auto limit=remaining.最大读取材料数;
                auto value=feature_.读取有界准确特征事实({1,r.G,r.H,id,{limit,limit,limit,limit}});
                take(remaining.最大读取材料数,value.用量.材料总数);
                if(!value.成功())throw 失败{映射(value.状态)};
                要求(value.事实->Gread==r.G&&value.事实->H==r.H&&value.事实->信息.身份==id);
                take(remaining.最大基础叶数,1);
                return leaves.emplace(id.编码.值,std::move(*value.事实)).first->second;
            };
            std::set<稳定编码> savedRoots;
            auto collect=[&](const 二次计算来源& source){
                if(const auto* d=std::get_if<二次已保存定义输出来源>(&source))savedRoots.insert(d->定义.结点);
            };
            for(const auto& item:r.图项组)for(const auto& input:item.输入)collect(input.来源);
            for(const auto& root:r.根输出组)collect(来源(root));
            if(!savedRoots.empty()){
                const auto limit=remaining.最大读取材料数;
                特征类标量派生批量读取请求 request;
                request.Gread=r.G;request.H=r.H;
                request.预算={std::min(remaining.最大图项数,limit),limit,
                    std::min(remaining.最大基础叶数,limit),limit,remaining.最大展开深度,limit};
                要求(request.预算.有效(),S::预算不足);
                for(auto id:savedRoots)request.根定义组.push_back({id});
                auto value=feature_.批量读取标量派生定义(request);
                if(!value.成功())throw 失败{value.状态==特征类标量状态::已读取 ? S::内部不一致 : 映射(value.状态)};
                要求(value.Gread==r.G&&value.H==r.H&&value.原请求.根定义组==request.根定义组
                    &&value.原请求.预算==request.预算);
                const auto count=static_cast<std::uint64_t>(value.完整定义组.size());
                要求(count<=remaining.最大图项数&&count<=UINT64_MAX/2&&2*count<=remaining.最大来源边数
                    &&value.基础叶组.size()<=remaining.最大基础叶数&&value.用量.材料总数<=remaining.最大读取材料数,S::预算不足);
                // 联合预取四项预算先统一准入，随后只扣一次；缓存写入失败则整次失败。
                remaining.最大图项数-=count;remaining.最大来源边数-=2*count;
                remaining.最大基础叶数-=value.基础叶组.size();remaining.最大读取材料数-=value.用量.材料总数;
                for(const auto& leaf:value.基础叶组){
                    准确特征读取事实 f;f.Gread=r.G;f.H=r.H;f.信息.身份={leaf.F};f.信息.类型={leaf.FT};
                    f.信息.准确值=std::holds_alternative<std::int64_t>(leaf.完整值) ? 特征准确值{leaf.值}
                        :特征准确值{std::get<特征值信息>(leaf.完整值).值身份};
                    f.完整值=leaf.完整值;f.类型关系=leaf.类型关系;f.准确值事实=leaf.值事实;
                    f.创建G=leaf.创建G;f.退出G=leaf.退出G;
                    要求(准确来源完整(f)&&leaves.emplace(leaf.F.值,std::move(f)).second);
                }
                for(auto& definition:value.完整定义组)
                    要求(saved.emplace(definition.定义身份.结点.值,std::move(definition)).second);
            }
            auto loadCurrent=[&](特征类型身份 ft,特征I64比较用途 purpose)->有序I64比较合同快照 {
                const auto key=std::make_pair(ft.编码.值,static_cast<unsigned>(purpose));
                if(current.contains(key))return current.at(key);
                auto value=feature_.读取当前I64比较绑定_v2({2,r.G,ft,purpose});
                if(!value.成功()) {
                    using K=特征I64比较绑定读取状态_v2;
                    switch(value.状态) {
                    case K::未找到:throw 失败{S::未注册};
                    case K::事实代次漂移:throw 失败{S::事实代次漂移};
                    case K::历史材料不可用:throw 失败{S::历史材料不可用};
                    case K::资源失败:throw 失败{S::资源失败};
                    case K::注册不唯一:case K::目标已退出:case K::格式不支持:throw 失败{S::事实冲突};
                    default:throw 失败{S::内部不一致};
                    }
                }
                return current.emplace(key,快照(*value.事实,r.G)).first->second;
            };
            std::function<void(节点键,unsigned,std::uint64_t)> expand;
            expand=[&](节点键 key,unsigned mask,std::uint64_t depth) {
                要求(key.second&&mask&&mask<=7,S::入口拒绝);
                要求(depth<=r.预算.最大展开深度,S::预算不足);
                if(key.first){
                    要求(locals.contains(key.second),S::入口拒绝);
                    要求(!(mask&~locals.at(key.second)->要求结果位),S::入口拒绝);
                }
                needed[key]|=mask;要求(!gray.contains(key));
                if(done.contains(key)){要求(heights.at(key)<=r.预算.最大展开深度-depth+1,S::预算不足);return;}
                if(key.first)take(remaining.最大来源边数,2);
                gray.insert(key);
                二次计算项回执 node;node.节点=身份(key);
                const 二次计算项* local=nullptr;const 特征类标量派生事实* definition=nullptr;
                std::array<二次计算输入,2> inputs;
                if(key.first) {
                    要求(locals.contains(key.second),S::入口拒绝);local=locals.at(key.second);inputs=local->输入;
                    要求(!(mask&~local->要求结果位),S::入口拒绝);
                } else {
                    要求(saved.contains(key.second));
                    definition=&saved.at(key.second);
                    for(unsigned i=0;i<2;++i) {
                        const auto& s=definition->来源组[i].内容;
                        inputs[i].顺序=i+1;
                        inputs[i].角色=definition->注册.用途==特征类比较用途::目标判断 ? (i ? 特征I64输入角色::目标值:特征I64输入角色::当前事实)
                            :(i ? 特征I64输入角色::新状态:特征I64输入角色::旧状态);
                        if(const auto* base=std::get_if<特征类标量基础来源>(&s.来源))inputs[i].来源=二次准确F来源{base->F};
                        else {const auto& p=std::get<特征类标量派生来源>(s.来源);inputs[i].来源=二次已保存定义输出来源{p.定义,p.上游输出角色};}
                    }
                }
                std::uint64_t height=1;
                for(unsigned i=0;i<2;++i) {
                    const auto& input=inputs[i];auto& result=node.输入[i];
                    result.顺序=i+1;result.角色=input.角色;result.来源=input.来源;
                    if(const auto* base=std::get_if<二次准确F来源>(&input.来源)) {
                        要求(depth<r.预算.最大展开深度,S::预算不足);
                        const auto& leaf=loadLeaf(base->F);要求(整数(leaf).has_value(),S::类型不匹配);
                        result.实际FT=leaf.信息.类型;result.实际阶次=1;height=std::max(height,std::uint64_t{2});
                    } else {
                        const auto child=输出键(input.来源);const auto role=输出角色(input.来源);
                        expand(child,角色位(role),depth+1);
                        const auto& childNode=nodes.at(child);
                        const auto spec=std::find_if(childNode.K.输出组.begin(),childNode.K.输出组.end(),[&](const auto& x){return x.输出.角色==role;});
                        要求(spec!=childNode.K.输出组.end(),S::类型不匹配);
                        result.实际FT=spec->输出FT;result.实际阶次=childNode.真实阶次;height=std::max(height,heights.at(child)+1);
                    }
                }
                要求(node.输入[0].实际FT==node.输入[1].实际FT,S::类型不匹配);
                const auto ft=node.输入[0].实际FT;
                if(local) {
                    要求(ft==local->输入FT,S::类型不匹配);node.K=loadCurrent(ft,local->用途);
                    要求(!local->预期K||*local->预期K==node.K.K,S::固定规则不兼容);
                } else {
                    const auto& old=definition->注册;auto& k=node.K;
                    k.来源=特征比较合同来源::已保存定义固定K;k.K={definition->注册身份.值};k.输入FT=ft;
                    k.用途=old.用途==特征类比较用途::目标判断 ? 特征I64比较用途::目标判断:特征I64比较用途::变化分析;
                    k.左角色=inputs[0].角色;k.右角色=inputs[1].角色;k.上下文要求位=0;
                    k.算法版本=old.算法版本;k.输入量化=old.输入量化;k.误差合同版本=old.误差合同版本;
                    k.误差预算=old.误差预算;k.相等容差=old.相等容差;k.Gread=r.G;k.H=r.H;
                    for(const auto& output:definition->输出组) {
                        k.输出组.push_back({output.声明,{output.特征类型},output.归属关系});
                        if(output.声明.角色==特征类标量结果角色::关系)
                            k.关系编码=old.用途==特征类比较用途::目标判断 ? 特征I64关系编码{1,2,3}:特征I64关系编码{4,5,6};
                    }
                    要求(固定相容(k,loadCurrent(ft,k.用途)),S::固定规则不兼容);
                }
                const auto maximum=std::max(node.输入[0].实际阶次,node.输入[1].实际阶次);
                要求(maximum<UINT32_MAX,S::预算不足);node.真实阶次=maximum+1;
                if(definition)要求(node.真实阶次==definition->真实阶次);
                for(unsigned i=0;i<2;++i)要求(node.输入[i].角色==(i ? node.K.右角色:node.K.左角色),S::入口拒绝);
                要求(height<=r.预算.最大展开深度-depth+1,S::预算不足);
                heights.emplace(key,height);nodes.emplace(key,std::move(node));postorder.push_back(key);gray.erase(key);done.insert(key);
            };
            for(const auto& root:r.根输出组) {
                const auto source=来源(root);要求(来源有效(source),S::入口拒绝);
                const auto key=输出键(source);const auto role=输出角色(source);
                要求(roots.emplace(key,static_cast<unsigned>(role)).second,S::入口拒绝);
                take(remaining.最大结果项数,1);expand(key,角色位(role),1);
            }
            unsigned contexts=0;for(const auto& [key,node]:nodes)contexts|=node.K.上下文要求位;
            要求(contexts==上下文位(r.上下文),S::入口拒绝);
            for(const auto& key:postorder) {
                auto& node=nodes.at(key);特征比较执行请求 request;
                request.请求身份=r.请求身份;request.K=node.K;request.要求结果位=static_cast<std::uint8_t>(needed.at(key));
                for(unsigned i=0;i<2;++i) {
                    auto& input=node.输入[i];
                    if(const auto* base=std::get_if<二次准确F来源>(&input.来源))input.值=*整数(leaves.at(base->F.编码.值));
                    else {
                        const auto& child=nodes.at(输出键(input.来源));
                        const auto role=输出角色(input.来源);
                        const auto value=std::find_if(child.输出组.begin(),child.输出组.end(),[&](const auto& x){return x.输出角色==role;});
                        要求(value!=child.输出组.end());
                        要求(同量化(value->量化,node.K.输入量化),S::单位量化不匹配);input.值=value->值;
                    }
                    request.输入[i]={input.角色,input.值};
                }
                auto result=provider_.比较(request);
                if(!result.成功()) {
                    if(result.状态==特征比较状态::资源失败)throw 失败{S::资源失败};
                    if(result.状态==特征比较状态::未实现)throw 失败{S::算法未实现};
                    if(result.拒绝原因==特征比较拒绝原因::差异不可表示)throw 失败{S::运算溢出};
                    if(result.拒绝原因==特征比较拒绝原因::值域不允许)throw 失败{S::结果范围不满足};
                    throw 失败{S::内部不一致};
                }
                要求(result.K==node.K.K&&result.请求身份==r.请求身份&&result.实际结果位==request.要求结果位
                    &&result.算法版本==node.K.算法版本);
                for(const auto& value:result.输出组)node.输出组.push_back({node.节点,node.K.K,value.输出FT,value.角色,value.值,value.量化,node.真实阶次});
                out.计算项组.push_back(node);
            }
            for(const auto& [id,leaf]:leaves)out.基础叶组.push_back({leaf});
            for(const auto& root:r.根输出组) {
                const auto source=来源(root);const auto& node=nodes.at(输出键(source));const auto role=输出角色(source);
                const auto value=std::find_if(node.输出组.begin(),node.输出组.end(),[&](const auto& x){return x.输出角色==role;});
                要求(value!=node.输出组.end());out.结果组.push_back(*value);
            }
            out.根输出组=r.根输出组;out.状态=S::已计算;守卫(r.G);要求(out.成功());
        } catch(const 失败& e){fail(e.状态);}
        catch(const std::bad_alloc&){fail(S::资源失败);}
        catch(const std::length_error&){fail(S::资源失败);}
        catch(...){fail(S::内部不一致);}
        return out;
    }
};
}
