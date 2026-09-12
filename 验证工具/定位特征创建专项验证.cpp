#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

import 海中鱼巣.领域.数据服务.存在类;
import 海中鱼巣.领域.数据服务.场景类;
import 海中鱼巣.领域.数据服务.定位特征;
import 海中鱼巣.业务.应用服务.场景成员概念类;
namespace {
using namespace 海中鱼巣;
int 通过数=0;
void 要求(bool ok,std::string_view name){if(!ok){std::cerr<<"FAIL "<<name<<'\n';throw std::runtime_error("probe failed");}++通过数;std::cout<<"PASS "<<name<<'\n';}
std::uint64_t 当前代次(const L1事实基座服务& l1){auto r=l1.读取中性当前事实代次({L1中性CRUD合同版本});要求(r.状态==L1中性读取状态::成功&&r.事实代次,"current-generation");return r.事实代次;}
L1所有者范围交付 建立所有者(L1所有者范围签发器& issuer,std::uint64_t key){auto r=issuer.建立所有者范围({L1所有者范围CRUD合同版本,{key},L1所有者范围种类::独占结构范围});要求(r.建立结果.状态==L1所有者范围管理状态::成功&&r.写入端口,"owner-create");return r;}
std::vector<稳定编码> 建立元节点(L1所有者范围写端口& port,const L1事实基座服务& l1,std::uint64_t key,const std::vector<std::pair<节点种类,std::optional<L1所有者范围值表示种类>>>& specs){L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,当前代次(l1),{key}};for(std::uint32_t i=0;i<specs.size();++i)ws.节点.push_back({{i+1},specs[i].first,specs[i].second});auto saved=port.提交所有者范围中性写集(ws);要求(saved.状态==L1所有者范围写入状态::成功,"metadata-write");std::vector<稳定编码> ids(specs.size());for(const auto&[k,id]:saved.新编码映射)if(k.值&&k.值<=ids.size())ids[k.值-1]=id;for(auto id:ids)要求(有效(id),"metadata-id");return ids;}
}
int main(int argc,char** argv){using namespace 海中鱼巣;try{std::cout<<std::unitbuf;
auto runtime=建立L1事实基座运行包();auto&l1=runtime.读取服务();auto&issuer=runtime.所有者范围签发器();
auto fdef=建立所有者(issuer,101),finfo=建立所有者(issuer,102);auto fmeta=建立元节点(*fdef.写入端口,l1,1001,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});
// 起点模型存量夹具：唯一端口在move前写入真实原布局，绝非新的公开无父入口。
std::vector<std::pair<节点种类,std::optional<L1所有者范围值表示种类>>> ds(16,{节点种类::普通,std::nullopt});
for(auto i:{2,9,13})ds[i]={节点种类::属性类型,L1所有者范围值表示种类::U64组};
for(auto i:{3,8})ds[i]={节点种类::属性类型,L1所有者范围值表示种类::I64};ds[14]={节点种类::属性类型,L1所有者范围值表示种类::I64组};
auto dl=建立元节点(*fdef.写入端口,l1,1,ds);auto il=建立元节点(*finfo.写入端口,l1,1,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::普通,std::nullopt}});
L1所有者范围写集请求 tw{L1所有者范围CRUD合同版本,当前代次(l1),{1002}};tw.节点={{{1},节点种类::属性类型,L1所有者范围值表示种类::I64}};
tw.关系={{{2},L1所有者范围写集本地键{1},dl[0],dl[1],1},{{3},L1所有者范围写集本地键{1},fmeta[0],dl[4],1},{{4},L1所有者范围写集本地键{1},fmeta[1],dl[5],1}};
tw.值={{{5},L1所有者范围写集本地键{1},dl[2],std::vector<std::uint64_t>{1,1,1,0,100},fmeta[0]}};tw.属性槽变更={{L1所有者范围写集本地键{1},dl[2],{5}}};
auto ts=fdef.写入端口->提交所有者范围中性写集(tw);要求(ts.状态==L1所有者范围写入状态::成功,"legacy-FT-seed");稳定编码 legacyFT{};for(auto [k,id]:ts.新编码映射)if(k.值==1)legacyFT=id;
std::vector<稳定编码> legacyF,legacyValues;std::vector<std::uint64_t> legacyBirth;
for(std::int64_t value:{60,60,70}){L1所有者范围写集请求 fw{L1所有者范围CRUD合同版本,当前代次(l1),{1100+legacyF.size()}};fw.节点={{{1},节点种类::普通,std::nullopt}};
 fw.关系={{{2},L1所有者范围写集本地键{1},il[0],il[1],1},{{3},L1所有者范围写集本地键{1},legacyFT,il[4],1}};
 fw.值={{{4},L1所有者范围写集本地键{1},il[2],value,fmeta[0]}};fw.属性槽变更={{L1所有者范围写集本地键{1},il[2],{4}}};
 auto fs=finfo.写入端口->提交所有者范围中性写集(fw);要求(fs.状态==L1所有者范围写入状态::成功,"legacy-F-seed");
 for(auto [k,id]:fs.新编码映射){if(k.值==1)legacyF.push_back(id);if(k.值==4)legacyValues.push_back(id);}legacyBirth.push_back(fs.事实代次);
}
特征值类数据服务 values(l1);特征类数据服务 features(l1,std::move(*fdef.写入端口),std::move(*finfo.写入端口),values,fmeta[0]);
auto eo=建立所有者(issuer,103);auto em=建立元节点(*eo.写入端口,l1,2001,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});
存在类数据服务 existence(l1,features,std::move(*eo.写入端口),em[0],em[1],{em[2]});
auto created=existence.新增存在({存在类数据合同版本,当前代次(l1),{2101}});要求(created.成功()&&created.存在结点,"real-C-create");auto C=created.存在结点->结点;
auto so=建立所有者(issuer,104),sto=建立所有者(issuer,105);
auto sm=建立元节点(*sto.写入端口,l1,3001,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::U64组},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::I64}});
状态类数据服务 state(l1,features,std::move(*sto.写入端口),{sm[0],sm[1],sm[2],sm[3],sm[4],sm[5],sm[6],sm[7]});
场景结构登记请求 br{2,当前代次(l1),{1}};auto base=场景类数据服务::登记结构类型(l1,*so.写入端口,br);要求(base.成功(br),"scene-base");
场景特征组织扩展登记请求 er{1,当前代次(l1)};auto ext=场景类数据服务::登记特征组织扩展(l1,*so.写入端口,er);要求(ext.成功(er),"scene-feature-layout");
场景直接包含扩展登记请求 ir{1,当前代次(l1)};auto inc=场景类数据服务::登记直接包含扩展(l1,*so.写入端口,ir);要求(inc.成功(ir),"scene-containment-layout");
场景类数据服务 scene(l1,std::move(*so.写入端口),*base.交付,existence,state,*ext.交付,*inc.交付);
场景角色启用请求 enable{2,当前代次(l1),{4001},C};auto enabled=scene.启用场景角色(enable);要求(enabled.启用成功(enable),"real-C-scene-role");

const auto coldG=当前代次(l1);bool coldRejected=false;
try{定位特征数据服务 cold(features,existence,scene);}catch(const std::invalid_argument&){coldRejected=true;}
要求(coldRejected&&当前代次(l1)==coldG,"ports-valid-both-ready-false-constructor-refused");
要求(std::holds_alternative<std::monostate>(features.初始化特征定义结构()),"external-definition-init");
const auto halfG=当前代次(l1);bool halfRejected=false;
try{定位特征数据服务 half(features,existence,scene);}catch(const std::invalid_argument&){halfRejected=true;}
要求(halfRejected&&当前代次(l1)==halfG,"definition-only-constructor-refused");
要求(std::holds_alternative<std::monostate>(features.初始化准确特征结构()),"feature-information-init");auto ftResult=features.创建先天I64特征类型({fmeta[0],fmeta[1],1,1,{{0,100}},std::nullopt});auto*ft=std::get_if<特征类型身份>(&ftResult);要求(ft&&有效(*ft),"feature-type-create");
const auto g=当前代次(l1);auto view=scene.读取场景角色历史({2,g,g,C});要求(view.成功({2,g,g,C})&&view.角色->四根[0].根.编码==enabled.角色->四根[0].根.编码,"explicit-feature-root");
要求(当前代次(l1)==g,"fixture-read-zero-write");
定位特征数据服务 located(features,existence,scene);要求(当前代次(l1)==g,"combined-constructor-zero-write");
定位特征创建请求 request{1,g,{C,enabled.角色->四根[0].根.编码},ft->编码,std::int64_t{10},std::nullopt,{5001},{5002},{5003},{5004},{100,16}};
auto made=located.创建或复用(request);std::cout<<"CREATE_STATE "<<static_cast<int>(made.状态)<<" G="<<made.Gread<<"\n";
要求(made.成功()&&made.状态==定位特征状态::已创建&&made.首次发布H==g+1,"atomic-feature-create");
要求(当前代次(l1)==g+1,"three-owners-one-generation");
auto replay=located.创建或复用(request);要求(replay.成功()&&replay.状态==定位特征状态::精确重复&&replay.事实==made.事实,"atomic-feature-original-key");
auto reuse=request;reuse.G0=当前代次(l1);reuse.组合键={5011};reuse.内容键={5012};reuse.已知键={5013};reuse.组织键={5014};auto reused=located.创建或复用(reuse);
要求(reused.成功()&&reused.状态==定位特征状态::已复用&&!reused.首次发布H&&reused.事实==made.事实,"same-position-reuse");

auto reject=[&](定位特征创建请求 q,定位特征状态 expected,const char* label){const auto before=当前代次(l1);auto x=located.创建或复用(q);std::cout<<"REJECT_STATE "<<static_cast<int>(x.状态)<<" "<<label<<"\n";要求(x.状态==expected&&!x.成功()&&!x.事实&&x.候选F.empty()&&当前代次(l1)==before,label);};
auto bad=reuse;bad.位置.组织父={};reject(bad,定位特征状态::入口拒绝,"missing-parent-zero-write");
bad=reuse;bad.准确值=std::int64_t{101};reject(bad,定位特征状态::入口拒绝,"outside-FT-domain");
bad=request;bad.准确值=std::int64_t{11};reject(bad,定位特征状态::幂等冲突,"original-key-value-conflict");
bad=request;bad.组合键={7001};reject(bad,定位特征状态::幂等冲突,"owner-books-without-combo-book");
bad=request;bad.指定F=made.事实->F;reject(bad,定位特征状态::入口拒绝,"bound-first-cannot-add-selectedF");
bad=reuse;bad.G0=g;reject(bad,定位特征状态::事实代次漂移,"unbound-old-generation");
for(unsigned i=0;i<4;++i){bad=reuse;auto* key=i==0 ? &bad.组合键 : i==1 ? &bad.内容键 : i==2 ? &bad.已知键 : &bad.组织键;*key={0x4E43abcd00000000ULL+i};reject(bad,定位特征状态::入口拒绝,"entire-maintenance-prefix");}
auto child=request;child.G0=当前代次(l1);child.位置.组织父=made.事实->F;child.准确值=std::int64_t{20};child.组合键={6001};child.内容键={6002};child.已知键={6003};child.组织键={6004};
auto childMade=located.创建或复用(child);要求(childMade.成功()&&childMade.状态==定位特征状态::已创建&&childMade.首次发布H==child.G0+1,"parent-F-atomic-create");
auto deep=child;deep.G0=当前代次(l1);deep.准确值=std::int64_t{30};deep.位置.组织父=childMade.事实->F;deep.预算.最大路径长度=2;deep.组合键={6101};deep.内容键={6102};deep.已知键={6103};deep.组织键={6104};reject(deep,定位特征状态::预算不足,"parent-path-depth-bound");
auto laterReplay=located.创建或复用(request);要求(laterReplay.成功()&&laterReplay.事实==made.事实&&laterReplay.首次发布H==made.首次发布H,"replay-after-other-publication");
const auto released=existence.删除成员关系({存在类数据合同版本,当前代次(l1),{8001},C,存在类成员种类::特征,childMade.事实->F});要求(released.成功(),"release-C-known-child");
定位特征读取请求 missingRead{1,当前代次(l1),当前代次(l1),childMade.事实->F,{100,16}};const auto missing=located.读取定位特征(missingRead);要求(missing.状态==定位特征状态::位置冲突&&!missing.事实,"path-intact-missing-C-known-position-conflict");
auto historical=missingRead;historical.H=*childMade.首次发布H;const auto old=located.读取定位特征(historical);要求(old.成功(historical)&&old.事实==childMade.事实,"historical-position-after-known-release");

auto legacyRequest=reuse;legacyRequest.G0=当前代次(l1);legacyRequest.正式特征类型=legacyFT;legacyRequest.准确值=std::int64_t{70};legacyRequest.组合键={9001};legacyRequest.内容键={9002};legacyRequest.已知键={9003};legacyRequest.组织键={9004};
reject(legacyRequest,定位特征状态::存量未定位,"legacy-unlocated-not-new-F");
auto addLegacy=existence.新增成员关系({存在类数据合同版本,当前代次(l1),{9101},C,存在类成员种类::特征,legacyF[0]});要求(addLegacy.成功(),"legacy-known-add");
场景特征组织请求 organize{1,当前代次(l1),{9102},C,enabled.角色->四根[0].根.编码,{legacyF[0]},16};auto organized=scene.组织特征实例(organize);要求(organized.成功(organize),"legacy-explicit-root-migration");
legacyRequest.G0=当前代次(l1);legacyRequest.准确值=std::int64_t{60};auto multi=located.创建或复用(legacyRequest);
要求(multi.状态==定位特征状态::需选择F&&multi.候选F==std::vector<稳定编码>{legacyF[0],legacyF[1]}&&!multi.事实,"all-exact-candidates-not-location-filtered");
legacyRequest.指定F=legacyF[0];auto selected=located.创建或复用(legacyRequest);要求(selected.成功()&&selected.状态==定位特征状态::已复用&&selected.事实->出生H==legacyBirth[0],"explicit-selected-legacy-reuse-old-birth");
auto adopt=existence.变更当前采用({1,当前代次(l1),{9201},C,{legacyFT},存在当前采用建立{{legacyF[0]}},100});要求(adopt.成功(),"legacy-current-adoption-regression");
auto protectedKnown=existence.删除成员关系({存在类数据合同版本,当前代次(l1),{9202},C,存在类成员种类::特征,legacyF[0]});要求(protectedKnown.状态==存在类数据状态::引用冲突,"adopted-known-protection");
auto boundInput=request;boundInput.G0=当前代次(l1);boundInput.准确值=特征值身份{legacyValues[2]};boundInput.组合键={9301};boundInput.内容键={9302};boundInput.已知键={9303};boundInput.组织键={9304};auto copied=located.创建或复用(boundInput);
要求(copied.成功()&&copied.状态==定位特征状态::已创建&&copied.事实->准确I64==70,"reference-input-copied-I64");
const auto copiedNode=l1.读取所有者范围当前节点({L1所有者范围CRUD合同版本,copied.事实->F});const auto* cn=copiedNode.事实 ? std::get_if<L1所有者范围节点事实>(&*copiedNode.事实) : nullptr;
要求(cn&&cn->当前属性.size()==1,"reference-current-slot-exactly-one");auto cv=l1.读取所有者范围历史事实({L1所有者范围CRUD合同版本,cn->当前属性.front().当前值});const auto* vv=cv.事实 ? std::get_if<L1所有者范围值事实>(&*cv.事实) : nullptr;
要求(vv&&vv->来源节点==fmeta[0]&&std::get<std::int64_t>(vv->材料)==70,"reference-original-producer-and-content");
for(auto reserved:{L1所有者范围写入幂等身份{0x455849535446414DULL},场景特征组织扩展登记固定幂等身份,场景直接包含扩展登记固定幂等身份}){
 auto q=reuse;q.G0=当前代次(l1);q.已知键=reserved;reject(q,定位特征状态::入口拒绝,"owner-reserved-before-first-book");}
for(unsigned path=0;path<3;++path)for(unsigned slot=0;slot<4;++slot)
 for(auto reserved:{L1所有者范围写入幂等身份{0},L1所有者范围写入幂等身份{1},L1所有者范围写入幂等身份{0x4645415449363442ULL},
   L1所有者范围写入幂等身份{0x455849535446414DULL},场景特征组织扩展登记固定幂等身份,场景直接包含扩展登记固定幂等身份,
   L1所有者范围写入幂等身份{0x4E43ffff00000042ULL}}){
 auto q=reuse;q.G0=当前代次(l1);if(path==0)q.准确值=std::int64_t{99};if(path==1)q.指定F=made.事实->F;
 auto* k=slot==0 ? &q.组合键 : slot==1 ? &q.内容键 : slot==2 ? &q.已知键 : &q.组织键;*k=reserved;
 reject(q,定位特征状态::入口拒绝,"all-owner-reserved-four-slots-three-paths");
 }
{
 auto foreignRuntime=建立L1事实基座运行包();auto& foreignL1=foreignRuntime.读取服务();auto& foreignIssuer=foreignRuntime.所有者范围签发器();
 auto fd=建立所有者(foreignIssuer,201),fi=建立所有者(foreignIssuer,202);
 auto fm=建立元节点(*fd.写入端口,foreignL1,20001,{{节点种类::普通,std::nullopt}});
 特征值类数据服务 foreignValues(foreignL1);特征类数据服务 foreignFeatures(foreignL1,std::move(*fd.写入端口),std::move(*fi.写入端口),foreignValues,fm[0]);
 const auto before=当前代次(l1),otherBefore=当前代次(foreignL1);bool rejected=false;
 try{定位特征数据服务 invalid(foreignFeatures,existence,scene);}catch(const std::invalid_argument&){rejected=true;}
 要求(rejected&&当前代次(l1)==before&&当前代次(foreignL1)==otherBefore,"constructor-other-L1-zero-write");
}
// 真实概念 owner 与自有结构登记；所有原生值来源均为所属节点。
auto co=建立所有者(issuer,106);
std::vector<std::pair<节点种类,std::optional<L1所有者范围值表示种类>>> cs(28,{节点种类::普通,std::nullopt});
for(std::size_t i=16;i<27;++i)cs[i]={节点种类::属性类型,i==21||i==24 ? L1所有者范围值表示种类::U64组 : i==23 ? L1所有者范围值表示种类::I64组 : L1所有者范围值表示种类::I64};
auto ct=建立元节点(*co.写入端口,l1,10001,cs);
auto cm=建立元节点(*co.写入端口,l1,10002,std::vector<std::pair<节点种类,std::optional<L1所有者范围值表示种类>>>(8,{节点种类::普通,std::nullopt}));
std::vector<std::pair<节点种类,std::optional<L1所有者范围值表示种类>>> xs(24,{节点种类::普通,std::nullopt});
for(auto i:{4,6,17,22})xs[i]={节点种类::属性类型,L1所有者范围值表示种类::U64组};
for(auto i:{5,9,10,23})xs[i]={节点种类::属性类型,L1所有者范围值表示种类::I64};
auto xt=建立元节点(*co.写入端口,l1,10003,xs);
L1所有者范围写集请求 cw{L1所有者范围CRUD合同版本,当前代次(l1),{10004}};std::uint32_t ck=1;
auto edge=[&](稳定编码 a,稳定编码 b,稳定编码 t,std::int64_t role){cw.关系.push_back({{ck++},a,b,t,role});};
auto attr=[&](稳定编码 n,稳定编码 t,std::int64_t v){L1所有者范围写集本地键 k{ck++};cw.值.push_back({k,n,t,v,n});cw.属性槽变更.push_back({n,t,k});};
for(std::size_t i=0;i<ct.size();++i)edge(cm[0],ct[i],ct[27],i+1);
edge(cm[0],cm[1],ct[1],1);attr(cm[0],ct[16],1);attr(cm[1],ct[16],1);attr(cm[1],ct[17],1);
for(std::size_t i=0;i<4;++i){edge(cm[0],cm[i+2],ct[0],i+1);attr(cm[i+2],ct[18],i+1);}
attr(cm[6],ct[16],1);for(std::size_t i=0;i<xt.size();++i)edge(cm[6],xt[i],ct[27],i+1);
auto cws=co.写入端口->提交所有者范围中性写集(cw);要求(cws.状态==L1所有者范围写入状态::成功,"concept-layout-real-write");
概念树结构交付 cl{cm[0],{概念树概念身份{cm[2]},概念树概念身份{cm[3]},概念树概念身份{cm[4]},概念树概念身份{cm[5]}},概念树规则身份{cm[1]},
 {ct[0],ct[1],ct[2],ct[3],ct[4],ct[5],ct[6],ct[7],ct[8],ct[9],ct[10],ct[11],ct[12],ct[13],ct[14],ct[15],ct[16],ct[17],ct[18],ct[19],ct[20],ct[21],ct[22],ct[23],ct[24],ct[25],ct[26],ct[27]}};
特征概念结构交付 cx;cx.锚点=cm[6];for(std::size_t i=0;i<xt.size();++i)cx.类型[i]=xt[i];
概念树类数据服务 concepts(l1,features,existence,values,std::move(*co.写入端口),cl,cx,概念树存在扩展交付{cm[7]});
const auto conceptG=当前代次(l1);要求(concepts.共享应用已启用(),"concept-shared-provider-ready");
auto appFTResult=features.创建先天I64特征类型({fmeta[0],fmeta[1],1,1,{{0,100}},I64特征域形成参数{2,fmeta[0]}});
auto* appFT=std::get_if<特征类型身份>(&appFTResult);要求(appFT&&有效(*appFT),"app-FT-with-domain-rule");
特征概念预算 cb{{10000,10000,10000,10000,10000,10000,10000,10000},10000,10000,10000,10000,10000};
特征概念观察请求 aq{{*appFT,fmeta[0],1},std::int64_t{45},1,std::nullopt,100,cb,
 {1,当前代次(l1),{C,enabled.角色->四根[0].根.编码},appFT->编码,std::int64_t{45},std::nullopt,{11001},{11002},{11003},{11004},{100,16}}};
// 显式R从真实直接包含结果取得，不从S/E编码反推。
直接归属联合只读组合器 joint(existence,scene);
auto newS=existence.新增存在({存在类数据合同版本,当前代次(l1),{12001}});
auto newE=existence.新增存在({存在类数据合同版本,当前代次(l1),{12002}});
auto otherE=existence.新增存在({存在类数据合同版本,当前代次(l1),{12003}});
要求(newS.成功()&&newE.成功()&&otherE.成功(),"scene-app-real-S-and-members");
const auto sceneS=newS.存在结点->结点,sceneE=newE.存在结点->结点;
场景树根启用请求 rootRequest{1,当前代次(l1),{12004},sceneS,100,100};auto rootCreated=scene.启用并建立场景树根(rootRequest,joint);
要求(rootCreated.建根成功(rootRequest),"scene-app-real-root");
场景直接包含写请求 memberRequest{1,当前代次(l1),{12005},sceneS,sceneE,100,100};auto memberCreated=scene.新增直接存在成员(memberRequest,joint);
要求(memberCreated.成功()&&memberCreated.包含,"scene-app-explicit-R-created");const auto R=memberCreated.包含->关系.编码;
场景成员概念请求_v2 sr;sr.S=概念树场景引用{sceneS};sr.E=概念树存在引用{sceneE};sr.Gread=sr.H=当前代次(l1);sr.场景关系预算=1;sr.场景成员关系=R;
sr.观察=aq;sr.观察.观察.序号=3;sr.观察.准确值=std::int64_t{55};sr.观察.时间=3;
sr.观察.定位={1,sr.Gread,{sceneS,rootCreated.场景角色->四根[0].根.编码},appFT->编码,std::int64_t{55},std::nullopt,{12101},{12102},{12103},{12104},{100,16}};
sr.采用=存在当前采用意图_v2{2,{12105},std::nullopt,100};
特征概念应用服务 scFC(features,concepts,located);存在概念树应用服务 ecApp(concepts,features,existence);场景成员概念应用服务 scApp(concepts,scene,existence,scFC,ecApp);
auto sceneReject=[&](场景成员概念请求_v2 q,场景成员概念状态_v2 expected,const char* label){const auto before=当前代次(l1);auto failed=scApp.处理场景成员概念(q);
 要求(failed.状态==expected&&!failed.最终投影&&!failed.特征概念.处理.定位结果&&当前代次(l1)==before,label);};
auto sq=sr;sq.场景成员关系={};sceneReject(sq,场景成员概念状态_v2::入口拒绝,"scene-missing-R-before-location");
sq=sr;sq.E=概念树存在引用{otherE.存在结点->结点};sceneReject(sq,场景成员概念状态_v2::引用冲突,"scene-valid-R-wrong-E");
sq=sr;sq.场景成员关系=made.事实->组织关系;sceneReject(sq,场景成员概念状态_v2::内部不一致,"scene-organization-edge-is-not-R");
sq=sr;sq.H=sr.Gread+1;sceneReject(sq,场景成员概念状态_v2::入口拒绝,"scene-future-H");
sq=sr;sq.场景关系预算=0;sceneReject(sq,场景成员概念状态_v2::入口拒绝,"scene-zero-relation-budget");

if(argc>1&&std::string_view(argv[1])=="scene-preflight"){
 std::cout<<"SCENE_PREFLIGHT_OK "<<通过数<<" G="<<l1.读取中性当前事实代次({1}).事实代次<<"\n";return 0;}
aq.定位.G0=当前代次(l1);
定位特征结果 appLocation; std::optional<特征概念命中请求> savedHitRequest;std::uint64_t savedHitH=0;
{
 特征概念应用服务 app(features,concepts,located);
 const auto start=当前代次(l1);auto noParent=aq;noParent.定位.位置.组织父={};
 auto refused=app.处理类型观察(noParent);
 要求(refused.状态==特征概念处理状态::定位拒绝&&!refused.观察&&!refused.概念&&!refused.名称&&当前代次(l1)==start,"FC-missing-parent-no-observation-or-concept");
 auto limited=aq;limited.预算.基础.最大概念数=0;auto failed=app.处理类型观察(limited);
 std::cout<<"FC_LIMITED_STATE "<<static_cast<int>(failed.状态)<<"\n";
 要求(failed.状态==特征概念处理状态::准确F已定位概念未完成&&failed.定位结果&&failed.定位结果->成功()
   &&failed.定位结果->首次发布H==start+1&&!failed.概念&&!failed.名称&&当前代次(l1)==start+1,"FC-concept-failure-keeps-atomic-location");
 appLocation=*failed.定位结果; auto inputRead=features.读取准确特征事实({1,当前代次(l1),*appLocation.首次发布H,{appLocation.事实->F}});
 if(auto* inputError=std::get_if<特征数据错误>(&inputRead))std::cout<<"FC_EXACT_ERROR "<<static_cast<int>(*inputError)<<"\n";
 else std::cout<<"FC_EXACT_OK\n";
 要求(std::holds_alternative<准确特征读取事实>(inputRead)&&std::get<准确特征读取事实>(inputRead).信息.身份.编码==appLocation.事实->F,"FC-atomic-F-public-exact-read");
 auto done=app.收敛类型观察(aq,appLocation);std::cout<<"FC_DONE_STATE "<<static_cast<int>(done.状态)<<"\n";
 if(done.待确认请求){std::cout<<"FC_PENDING "<<done.待确认请求->index()<<" G="<<当前代次(l1)<<"\n";
 auto diagnostic=concepts.收敛特征概念写入(*done.待确认请求);
 std::cout<<"FC_OWNER "<<static_cast<int>(diagnostic.状态)<<" H="<<diagnostic.首次H.value_or(0)<<" PUB="<<static_cast<int>(diagnostic.发布状态)<<"\n";}
 if(done.原因)std::visit([](auto e){std::cout<<"FC_REASON "<<static_cast<int>(e)<<"\n";},*done.原因);
 要求(done.状态==特征概念处理状态::完成&&done.定位结果&&done.定位结果->事实==appLocation.事实&&done.观察&&done.概念&&done.名称
   &&done.观察->输入.F.编码==appLocation.事实->F&&done.名称->F==done.观察->输入.F&&done.名称->C==done.概念->身份,"FC-original-location-resume-to-concept-and-name");
 const auto beforeReplay=当前代次(l1);auto observerHead=concepts.取得特征概念写入头(cb);要求(observerHead.成功(),"observation-public-head");
 类型观察登记请求 observerRequest{*observerHead.数据,{{*appFT,fmeta[0],77},done.观察->输入.F,77},100,cb};
 auto observedWrite=concepts.登记类型观察(observerRequest);要求(observedWrite.成功()&&observedWrite.观察&&observedWrite.首次H==beforeReplay+1,"observation-public-write-readback");
 const auto observationH=*observedWrite.首次H;auto observedReplay=concepts.收敛特征概念写入(observerRequest);
 要求(observedReplay.成功()&&observedReplay.状态==概念树数据状态::精确重复&&observedReplay.观察->记录==observedWrite.观察->记录
  &&observedReplay.首次H==observationH&&当前代次(l1)==observationH,"observation-original-key-same-record-zero-write");
 auto historyRead=concepts.读取类型观察({{1,observationH,observationH},observerRequest.输入.键,cb});
 要求(historyRead.成功()&&historyRead.数据->size()==1&&historyRead.数据->front().记录==observedWrite.观察->记录,"observation-current-history-native-owner-direction");
 const auto observationNative=l1.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,cm[6],xt[0],observationH});
 要求(observationNative.状态==L1所有者范围读取状态::成功,"observation-native-outgoing-query");
 std::size_t observationCount=0;for(const auto& edge:observationNative.关系组)if(edge.目标节点==observedWrite.观察->记录){
  ++observationCount;要求(edge.源节点==cm[6]&&edge.关系类型节点==xt[0]&&edge.角色或顺序==1&&edge.创建事实代次==observationH,"observation-native-anchor-to-record");}
 要求(observationCount==1,"observation-single-native-owner");
 auto hitHead=concepts.取得特征概念写入头(cb);要求(hitHead.成功(),"hit-public-head");
 特征概念命中请求 hitRequest{*hitHead.数据,observedWrite.观察->记录,{done.概念->类型根,done.概念->身份},cb};
 要求(hitRequest.完整命中组[0]!=hitRequest.完整命中组[1],"hit-TOP-and-fine-distinct");
 auto hitWrite=concepts.登记特征概念命中(hitRequest);
 要求(hitWrite.成功()&&hitWrite.首次H&&hitWrite.命中组.size()==2,"hit-public-complete-multi-result");
 const auto hitH=*hitWrite.首次H;savedHitRequest=hitRequest;savedHitH=hitH;
 auto badHit=hitWrite;badHit.命中组.pop_back();要求(!badHit.成功(),"hit-result-missing-member-pure-reject");
 badHit=hitWrite;badHit.命中组[1]=badHit.命中组[0];要求(!badHit.成功(),"hit-result-duplicate-pair-pure-reject");
 badHit=hitWrite;badHit.命中组[0].观察记录={};要求(!badHit.成功(),"hit-result-wrong-observation-pure-reject");
 const auto nativeHits=l1.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,cm[6],xt[12],hitH});
 要求(nativeHits.状态==L1所有者范围读取状态::成功,"hit-native-history-outgoing");
 for(const auto& hit:hitWrite.命中组){
  std::size_t count=0;for(const auto& edge:nativeHits.关系组)if(edge.目标节点==hit.记录){
   ++count;要求(edge.源节点==cm[6]&&edge.关系类型节点==xt[12]&&edge.角色或顺序==1&&edge.创建事实代次==hitH,"hit-native-anchor-to-record-same-H");}
  要求(count==1&&hit.观察记录==observedWrite.观察->记录,"hit-single-owner-and-observation");
  const auto os=l1.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,hit.记录,xt[13],hitH});
  const auto cs=l1.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,hit.记录,xt[14],hitH});
  要求(os.状态==L1所有者范围读取状态::成功&&cs.状态==L1所有者范围读取状态::成功&&os.关系组.size()==1&&cs.关系组.size()==1
   &&os.关系组.front().目标节点==hit.观察记录&&cs.关系组.front().目标节点==hit.C.值
   &&os.关系组.front().创建事实代次==hitH&&cs.关系组.front().创建事实代次==hitH,"hit-native-two-references-same-H");
 }
 auto unrelated=existence.新增存在({存在类数据合同版本,当前代次(l1),{12500}});要求(unrelated.成功(),"advance-unrelated-G-after-hit");
 const auto replayG=当前代次(l1);auto hitReplay=concepts.收敛特征概念写入(hitRequest);
 要求(hitReplay.成功()&&hitReplay.状态==概念树数据状态::精确重复&&hitReplay.首次H==hitH&&hitReplay.命中组.size()==2&&当前代次(l1)==replayG,"hit-original-request-after-future-G");
 for(const auto& old:hitWrite.命中组){std::size_t count=0;for(const auto& now:hitReplay.命中组)if(now.记录==old.记录&&now.观察记录==old.观察记录&&now.C==old.C)++count;要求(count==1,"hit-replay-complete-original-identities");}
 auto observationAfter=concepts.收敛特征概念写入(observerRequest);
 要求(observationAfter.成功()&&observationAfter.首次H==observationH&&observationAfter.观察->记录==observedWrite.观察->记录&&当前代次(l1)==replayG,"observation-original-request-after-future-G");

 auto observed=app.读取已发布类型观察(aq,当前代次(l1));
 要求(observed.状态==特征概念处理状态::完成&&observed.定位结果&&observed.定位结果->事实==appLocation.事实
   &&observed.观察->记录==done.观察->记录&&observed.名称->C==done.名称->C,"FC-independent-published-observation-read");
}
{
 特征概念应用服务 app(features,concepts,located);const auto before=当前代次(l1);
 auto badSaved=appLocation;badSaved.事实->位置.组织父=C;auto denied=app.收敛类型观察(aq,badSaved);
 要求(denied.状态!=特征概念处理状态::完成&&当前代次(l1)==before,"FC-tampered-location-receipt-zero-write");
 auto restored=app.收敛类型观察(aq,appLocation);
 要求(restored.状态==特征概念处理状态::完成&&restored.定位结果->事实==appLocation.事实,"FC-new-object-original-location-recovery");
 auto reuseInput=aq;reuseInput.观察.序号=2;reuseInput.时间=2;reuseInput.定位.G0=当前代次(l1);
 reuseInput.定位.组合键={11101};reuseInput.定位.内容键={11102};reuseInput.定位.已知键={11103};reuseInput.定位.组织键={11104};
 auto reusedLocation=located.创建或复用(reuseInput.定位);
 要求(reusedLocation.状态==定位特征状态::已复用&&reusedLocation.成功()&&!reusedLocation.首次发布H,"FC-zero-write-reuse-location-receipt");
 auto reusedDone=app.收敛类型观察(reuseInput,reusedLocation);
 要求(reusedDone.状态==特征概念处理状态::完成&&reusedDone.定位结果->事实==appLocation.事实
  &&reusedDone.观察->输入.键.序号==2,"FC-zero-write-reuse-receipt-consumed");
}
sr.Gread=当前代次(l1);sr.观察.定位.G0=sr.Gread;auto sceneDone=scApp.处理场景成员概念(sr);std::cout<<"SCENE_DONE_STATE "<<static_cast<int>(sceneDone.状态)<<"\n";
要求(sceneDone.成功()&&sceneDone.最终投影->场景成员.关系.编码==R&&sceneDone.最终投影->场景成员.父场景==sceneS
 &&sceneDone.最终投影->场景成员.成员==sceneE&&sceneDone.最终投影->定位.位置.组织父==sr.观察.定位.位置.组织父
 &&sceneDone.最终投影->定位.位置.组织父!=sceneE,"scene-complete-same-R-explicit-P-adopted");
const auto& projection=*sceneDone.最终投影;
概念树共享预算 sharedB{cb.基础,cb,10000,10000,10000,10000};存在概念应用预算 eb{sharedB,1,100,100};
存在概念形成请求 erq;erq.Gread=erq.H=当前代次(l1);erq.E=sr.E;erq.概念key=13001;erq.支持key=13002;erq.预算=eb;
erq.见证.push_back({projection.C.身份,概念树特征引用{projection.F.信息.身份.编码},*projection.F.准确值事实,projection.当前采用->关系});
存在概念场景限制 limit{sr.S,sr.E,sr.H,R};
auto badLimit=limit;badLimit.成员关系={};const auto ecG=当前代次(l1);auto ecRejected=ecApp.形成或复用存在概念(erq,badLimit,scene);
要求(ecRejected.业务状态==存在概念业务状态::入口拒绝&&当前代次(l1)==ecG,"EC-missing-R-zero-write");
auto ecPartial=ecApp.形成或复用存在概念(erq,limit,scene);
要求(!ecPartial.成功()&&ecPartial.继续&&ecPartial.继续->已定位EC&&ecPartial.继续->步骤.size()==1
 &&ecPartial.继续->步骤.front().已知首次H&&ecPartial.继续->步骤.front().回执提示,"EC-same-R-create-before-support-pause");
auto completeBudget=eb;completeBudget.最大派发步骤=8;
存在概念继续请求 ecResume{*ecPartial.继续,当前代次(l1),completeBudget};
badLimit=limit;badLimit.成员关系=appLocation.事实->组织关系;const auto resumeG=当前代次(l1);
auto ecBad=ecApp.继续存在概念操作(ecResume,badLimit,scene);
要求(!ecBad.成功()&&ecBad.继续&&当前代次(l1)==resumeG,"EC-resume-cannot-change-R");
auto ecDone=ecApp.继续存在概念操作(ecResume,limit,scene);
要求(ecDone.成功()&&ecDone.继续&&std::get<存在概念形成请求>(ecDone.继续->原业务).场景限制->成员关系==R,"EC-explicit-R-resume-support-complete");
场景直接包含写请求 removeR{1,当前代次(l1),{14001},sceneS,sceneE,100,100};auto removedR=scene.退出直接存在成员(removeR,joint);
要求(removedR.成功(),"scene-R-real-exit");
erq.Gread=当前代次(l1);const auto afterExitG=erq.Gread;auto covered=ecApp.形成或复用存在概念(erq,limit,scene);
要求(covered.业务状态==存在概念业务状态::被后继覆盖&&当前代次(l1)==afterExitG,"EC-original-H-valid-current-R-exited");
auto readd=removeR;readd.G0=当前代次(l1);readd.幂等身份={14002};auto R2=scene.新增直接存在成员(readd,joint);要求(R2.成功()&&R2.包含->关系.编码!=R,"scene-same-endpoints-new-R2");
erq.Gread=当前代次(l1);const auto afterReaddG=erq.Gread;auto stillCovered=ecApp.形成或复用存在概念(erq,limit,scene);
要求(stillCovered.业务状态==存在概念业务状态::被后继覆盖&&当前代次(l1)==afterReaddG,"EC-R2-does-not-replace-frozen-R");
// 预期的原键低预算失败保留owner待收敛材料，作为本夹具最后一次业务调用。
要求(savedHitRequest.has_value(),"saved-hit-negative-input");const auto lowG=当前代次(l1);
auto lowHit=*savedHitRequest;lowHit.预算.最大命中数=1;auto low=concepts.收敛特征概念写入(lowHit);
要求(!low.成功()&&low.首次H==savedHitH&&low.命中组.empty()&&low.原请求&&当前代次(l1)==lowG,"hit-positive-insufficient-budget-no-success-payload");
const auto finalG=l1.读取中性当前事实代次({1}).事实代次;
std::cout<<"PARENT_BOUND_CORE_OK "<<通过数<<" G="<<finalG<<"\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
