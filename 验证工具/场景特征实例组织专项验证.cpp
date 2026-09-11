#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#if defined(SCENE_FEATURE_EXISTENCE_PROVIDER_PROBE)
import 海中鱼巣.领域.数据服务.存在类;
#elif defined(SCENE_FEATURE_SCENE_ORGANIZATION_PROBE)
import 海中鱼巣.领域.数据服务.状态类;
import 海中鱼巣.领域.数据服务.场景类;
#else
#error 必须选择且只选择一个场景特征专项探针
#endif

namespace {
using namespace 海中鱼巣;
int 通过数=0;
void 要求(bool ok,std::string_view name){if(!ok){std::cerr<<"FAIL "<<name<<'\n';throw std::runtime_error("probe failed");}++通过数;std::cout<<"PASS "<<name<<'\n';}
std::uint64_t 当前代次(const L1事实基座服务& l1){auto r=l1.读取中性当前事实代次({L1中性CRUD合同版本});要求(r.状态==L1中性读取状态::成功&&r.事实代次,"current-generation");return r.事实代次;}
L1所有者范围交付 建立所有者(L1所有者范围签发器& issuer,std::uint64_t key){auto r=issuer.建立所有者范围({L1所有者范围CRUD合同版本,{key},L1所有者范围种类::独占结构范围});要求(r.建立结果.状态==L1所有者范围管理状态::成功&&r.写入端口,"owner-create");return r;}
std::vector<稳定编码> 建立元节点(L1所有者范围写端口& port,const L1事实基座服务& l1,std::uint64_t key,const std::vector<std::pair<节点种类,std::optional<L1所有者范围值表示种类>>>& specs){L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,当前代次(l1),{key}};for(std::uint32_t i=0;i<specs.size();++i)ws.节点.push_back({{i+1},specs[i].first,specs[i].second});auto saved=port.提交所有者范围中性写集(ws);要求(saved.状态==L1所有者范围写入状态::成功,"metadata-write");std::vector<稳定编码> ids(specs.size());for(const auto&[k,id]:saved.新编码映射)if(k.值&&k.值<=ids.size())ids[k.值-1]=id;for(auto id:ids)要求(有效(id),"metadata-id");return ids;}
特征信息身份 建立特征(特征类数据服务& service,特征类型身份 type,std::int64_t value){auto r=service.创建准确特征(type,value);auto*p=std::get_if<特征信息身份>(&r);要求(p&&有效(*p),"feature-create");return *p;}
}

#if defined(SCENE_FEATURE_SCENE_ORGANIZATION_PROBE)
namespace {
class 严格存在桩 final:public 存在结构身份只读提供者{
public:explicit 严格存在桩(const L1事实基座服务&l1):l1_(l1){}
bool 绑定于(const L1事实基座服务&x)const noexcept override{return &x==&l1_;}
存在当前身份确认结果 确认当前存在结构身份(std::uint64_t g,稳定编码 id)const override{return 有效(id)&&g?存在当前身份确认结果{存在结构身份只读状态::已读取,2,g}:存在当前身份确认结果{};}
存在身份来源历史见证读取结果 读取存在身份来源历史见证(std::uint64_t g,std::uint64_t h,稳定编码 id)const override{if(!g||!h||h>g||!有效(id))return{};存在身份来源历史见证 w{id,{900001},{900002},{900003},1,{1,std::nullopt},{1,std::nullopt},{1,std::nullopt},{1,std::nullopt}};return {存在结构身份只读状态::已读取,2,g,h,w};}
存在已知准确特征读取结果 确认当前已知准确特征(const 存在已知准确特征当前请求&r)const override{return 读取已知准确特征历史({r.版本,r.G0,r.G0,r.存在,r.特征});}
存在已知准确特征读取结果 读取已知准确特征历史(const 存在已知准确特征历史请求&r)const override{存在已知准确特征读取结果 out{存在已知准确特征只读状态::未找到,1,r.Gread,r.H,r.存在,r.特征,std::nullopt};if(r.版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.存在)||!有效(r.特征)||r.存在==r.特征)return {};for(const auto&x:known_)if(x.first==r.存在&&x.second==r.特征){out.状态=存在已知准确特征只读状态::已读取;out.见证=存在已知准确特征见证{{800000+r.特征.值},r.特征,1,std::nullopt};break;}return out;}
void 知道(稳定编码 e,稳定编码 f){known_.push_back({e,f});}
private:const L1事实基座服务&l1_;std::vector<std::pair<稳定编码,稳定编码>> known_;
};
}
#endif

int main(){using namespace 海中鱼巣;try{
static_assert(存在结构身份只读合同版本==2);static_assert(存在已知准确特征只读合同版本==1);
auto runtime=建立L1事实基座运行包();auto&l1=runtime.读取服务();auto&issuer=runtime.所有者范围签发器();
auto fdef=建立所有者(issuer,101),finfo=建立所有者(issuer,102);auto fmeta=建立元节点(*fdef.写入端口,l1,1001,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});特征值类数据服务 values(l1);特征类数据服务 features(l1,std::move(*fdef.写入端口),std::move(*finfo.写入端口),values,fmeta[0]);要求(std::holds_alternative<std::monostate>(features.初始化特征定义结构()),"feature-definition-init");要求(std::holds_alternative<std::monostate>(features.初始化准确特征结构()),"feature-information-init");auto ftResult=features.创建先天I64特征类型({fmeta[0],fmeta[1],1,1,{{0,100}},std::nullopt});auto*ft=std::get_if<特征类型身份>(&ftResult);要求(ft&&有效(*ft),"feature-type-create");auto f1=建立特征(features,*ft,10),f2=建立特征(features,*ft,20),f3=建立特征(features,*ft,30);
#if defined(SCENE_FEATURE_EXISTENCE_PROVIDER_PROBE)
auto owner=建立所有者(issuer,103);auto meta=建立元节点(*owner.写入端口,l1,2001,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});存在类数据服务 existence(l1,features,std::move(*owner.写入端口),meta[0],meta[1],{meta[2]});auto e=existence.新增存在({存在类数据合同版本,当前代次(l1),{2101}});要求(e.成功()&&e.存在结点,"existence-create");auto add=existence.新增成员关系({存在类数据合同版本,当前代次(l1),{2201},e.存在结点->结点,存在类成员种类::特征,f1.编码});要求(add.成功(),"known-add");auto adopted=existence.变更当前采用({1,当前代次(l1),{2301},e.存在结点->结点,*ft,存在当前采用建立{f1},4096});要求(adopted.成功(),"current-adoption-create");auto exitedExistence=existence.新增存在({存在类数据合同版本,当前代次(l1),{2102}});要求(exitedExistence.成功()&&exitedExistence.存在结点,"exited-existence-create");auto exitedId=exitedExistence.存在结点->结点;auto exited=existence.删除存在({存在类数据合同版本,当前代次(l1),{2401},exitedId});要求(exited.成功(),"exited-existence-remove");auto g=当前代次(l1);auto exitedRead=existence.确认当前已知准确特征({1,g,exitedId,f1.编码});要求(exitedRead.状态==存在已知准确特征只读状态::目标已退出&&!exitedRead.见证,"F03-existence-exited");存在已知准确特征当前请求 now{1,g,e.存在结点->结点,f1.编码};auto a=existence.确认当前已知准确特征(now);要求(a.当前成功(now),"F03-current");要求(a.当前成功(now),"F03-current-adoption-independent");存在已知准确特征历史请求 history{1,g,g,e.存在结点->结点,f1.编码};auto b=existence.读取已知准确特征历史(history);要求(b.历史成功(history),"F03-history");auto missing=existence.确认当前已知准确特征({1,g,e.存在结点->结点,f3.编码});要求(missing.状态==存在已知准确特征只读状态::未找到&&!missing.见证,"F03-missing");auto bad=existence.读取已知准确特征历史({1,g,g,e.存在结点->结点,e.存在结点->结点});要求(bad.状态==存在已知准确特征只读状态::入口拒绝&&!bad.见证,"F03-bad-shape");要求(当前代次(l1)==g,"F03-zero-write");std::cout<<"SCENE_FEATURE_EXISTENCE_PROVIDER_OK "<<通过数<<" PASS\n";
#else
static_assert(场景角色组织合同版本==2);static_assert(场景特征组织合同版本==1);严格存在桩 existence(l1);auto eowner=建立所有者(issuer,103);auto es=建立元节点(*eowner.写入端口,l1,2100,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});auto e1=es[0],e2=es[1];existence.知道(e1,f1.编码);existence.知道(e1,f2.编码);existence.知道(e2,f1.编码);existence.知道(e2,f3.编码);
auto sceneOwner=建立所有者(issuer,104),stateOwner=建立所有者(issuer,105);auto smeta=建立元节点(*stateOwner.写入端口,l1,3001,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::U64组},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::I64}});状态类数据服务 state(l1,features,std::move(*stateOwner.写入端口),{smeta[0],smeta[1],smeta[2],smeta[3],smeta[4],smeta[5],smeta[6],smeta[7]});场景结构登记请求 baseRequest{2,当前代次(l1),{1}};auto base=场景类数据服务::登记结构类型(l1,*sceneOwner.写入端口,baseRequest);要求(base.成功(baseRequest),"F01-base");场景特征组织扩展登记请求 extensionRequest{1,当前代次(l1)};auto extension=场景类数据服务::登记特征组织扩展(l1,*sceneOwner.写入端口,extensionRequest);要求(extension.成功(extensionRequest),"F02-extension");auto extensionReplay=场景类数据服务::登记特征组织扩展(l1,*sceneOwner.写入端口,extensionRequest);要求(extensionReplay.成功(extensionRequest)&&extensionReplay.状态==场景角色数据状态::精确重复,"F02-extension-replay");场景类数据服务 scene(l1,std::move(*sceneOwner.写入端口),*base.交付,existence,state,*extension.交付);
场景角色启用请求 c1r{2,当前代次(l1),{4001},e1};auto c1=scene.启用场景角色(c1r);要求(c1.启用成功(c1r),"scene-one");场景角色启用请求 c2r{2,当前代次(l1),{4002},e2};auto c2=scene.启用场景角色(c2r);要求(c2.启用成功(c2r),"scene-two");auto root1=c1.角色->四根[0].根.编码,root2=c2.角色->四根[0].根.编码;场景特征组织请求 r1{1,当前代次(l1),{5001},e1,root1,f1,4};auto w1=scene.组织特征实例(r1);要求(w1.成功(r1),"F04-root");auto replay=scene.组织特征实例(r1);要求(replay.成功(r1)&&replay.状态==场景角色数据状态::精确重复,"F07-replay");场景特征组织请求 r2{1,当前代次(l1),{5002},e1,f1.编码,f2,4};auto w2=scene.组织特征实例(r2);要求(w2.成功(r2),"F05-child");auto path=scene.读取特征组织历史({1,当前代次(l1),w2.首次发布代次,f2,4});要求(path.成功({1,当前代次(l1),w2.首次发布代次,f2,4})&&path.组织->路径边.size()==2,"F05-path");auto shortPath=scene.读取特征组织历史({1,当前代次(l1),w2.首次发布代次,f2,1});要求(shortPath.状态==场景角色数据状态::数量预算不足&&!shortPath.组织,"F06-budget");auto duplicate=scene.组织特征实例({1,当前代次(l1),{5003},e2,root2,f1,4});要求(duplicate.状态==场景角色数据状态::实例已组织,"F06-single-parent");auto unknown=scene.组织特征实例({1,当前代次(l1),{5004},e1,root1,f3,4});要求(unknown.状态==场景角色数据状态::未找到,"F06-unknown");auto blocked=scene.退出场景角色({2,当前代次(l1),{6001},e1});要求(blocked.状态==场景角色数据状态::引用冲突,"F08-exit-protect");场景角色退出请求 emptyRequest{2,当前代次(l1),{6002},e2};auto empty=scene.退出场景角色(emptyRequest);要求(empty.退出成功(emptyRequest),"F08-empty-exit");auto reserved=scene.启用场景角色({2,当前代次(l1),场景特征组织扩展登记固定幂等身份,e1});要求(reserved.状态==场景角色数据状态::入口拒绝,"F09-reserved");std::cout<<"SCENE_FEATURE_ORGANIZATION_OK "<<通过数<<" PASS\n";
#endif
return 0;}catch(const std::exception&e){std::cerr<<"scene feature probe aborted: "<<e.what()<<'\n';return 1;}}
