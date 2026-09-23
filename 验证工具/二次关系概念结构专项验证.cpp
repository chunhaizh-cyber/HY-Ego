#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "../海中鱼巣/业务/初始化.二次关系概念结构.h"
#include "../海中鱼巣/领域/数据服务.场景类.h"
#include "../海中鱼巣/领域/数据服务.概念树类.h"
#include "../海中鱼巣/领域/数据服务.存在类.h"
#include "../海中鱼巣/领域/数据服务.特征值类.h"
#include "../海中鱼巣/领域/数据服务.特征类.h"
#include "../海中鱼巣/领域/数据服务.特征值域比较类.h"
#include "../海中鱼巣/领域/数据服务.状态类.h"

namespace {
using namespace 海中鱼巣;
int passed=0;

void require(bool value,std::string_view name) {
  if(!value) { std::cerr<<"FAIL "<<name<<'\n'; throw std::runtime_error("RC-P1 probe failed"); }
  ++passed; std::cout<<"PASS "<<name<<'\n';
}

std::uint64_t generation(const L1事实基座服务& l1) {
  const auto r=l1.读取中性当前事实代次({L1中性CRUD合同版本});
  require(r.状态==L1中性读取状态::成功&&r.事实代次,"generation-read");
  return r.事实代次;
}

L1所有者范围交付 owner(L1所有者范围签发器& issuer,std::uint64_t key) {
  auto r=issuer.建立所有者范围({L1所有者范围CRUD合同版本,{key},L1所有者范围种类::独占结构范围});
  require((r.建立结果.状态==L1所有者范围管理状态::成功||
           r.建立结果.状态==L1所有者范围管理状态::精确重复)&&
          r.写入端口&&r.写入端口->有效(),"owner-create");
  return r;
}

using Spec=std::pair<节点种类,std::optional<L1所有者范围值表示种类>>;
std::vector<稳定编码> metadata(L1所有者范围写端口& port,const L1事实基座服务& l1,
                                std::uint64_t key,const std::vector<Spec>& specs) {
  L1所有者范围写集请求 q{L1所有者范围CRUD合同版本,generation(l1),{key}};
  for(std::uint32_t i=0;i<specs.size();++i) q.节点.push_back({{i+1},specs[i].first,specs[i].second});
  const auto saved=port.提交所有者范围中性写集(q);
  require(saved.状态==L1所有者范围写入状态::成功&&saved.新编码映射.size()==specs.size(),"metadata-publish");
  std::vector<稳定编码> ids(specs.size());
  for(const auto& [local,id]:saved.新编码映射) if(local.值&&local.值<=ids.size()) ids[local.值-1]=id;
  return ids;
}

void bounded_history_group_probe() {
  auto runtime=建立L1事实基座运行包();
  auto& l1=runtime.读取服务();
  auto delivery=owner(runtime.所有者范围签发器(),0xB101);
  L1所有者范围写集请求 write{L1所有者范围CRUD合同版本,generation(l1),{0xB102}};
  write.节点={{ {1},节点种类::普通,std::nullopt},
              { {2},节点种类::普通,std::nullopt},
              { {3},节点种类::普通,std::nullopt},
              { {4},节点种类::普通,std::nullopt},
              { {5},节点种类::属性类型,L1所有者范围值表示种类::I64},
              { {6},节点种类::属性类型,L1所有者范围值表示种类::I64},
              { {7},节点种类::普通,std::nullopt}};
  write.关系.push_back({{10},L1所有者范围写集本地键{1},L1所有者范围写集本地键{2},
                        L1所有者范围写集本地键{4},2});
  write.关系.push_back({{11},L1所有者范围写集本地键{1},L1所有者范围写集本地键{3},
                        L1所有者范围写集本地键{4},1});
  write.值.push_back({{20},L1所有者范围写集本地键{1},L1所有者范围写集本地键{5},
                      std::int64_t{20},L1所有者范围写集本地键{1}});
  write.值.push_back({{21},L1所有者范围写集本地键{1},L1所有者范围写集本地键{6},
                      std::int64_t{21},L1所有者范围写集本地键{1}});
  write.属性槽变更.push_back({L1所有者范围写集本地键{1},L1所有者范围写集本地键{5},{20}});
  write.属性槽变更.push_back({L1所有者范围写集本地键{1},L1所有者范围写集本地键{6},{21}});
  const auto saved=delivery.写入端口->提交所有者范围中性写集(write);
  require(saved.状态==L1所有者范围写入状态::成功,"bounded-group-fixture");
  const auto code=[&](std::uint64_t key){
    for(const auto& [local,id]:saved.新编码映射)if(local.值==key)return id;
    return 稳定编码{};
  };
  const auto endpoint=code(1),type=code(4),edge1=code(10),isolated=code(7);
  const auto g=generation(l1);
  const L1所有者范围有界历史关系组读取请求 relationRequest{
    L1所有者范围有界历史组读取合同版本,L1所有者范围关系端点方向::源,
    endpoint,type,g,g,2};
  const auto relations=l1.读取所有者范围有界历史关系组(relationRequest);
  require(relations.成功(relationRequest)&&relations.关系组.size()==2&&
          relations.关系组[0].编码.值<relations.关系组[1].编码.值,
          "bounded-relations-exact-and-sorted");
  auto shortRelations=relationRequest;shortRelations.最大数量=1;
  const auto relationShort=l1.读取所有者范围有界历史关系组(shortRelations);
  require(relationShort.状态==L1所有者范围读取状态::数量预算不足&&relationShort.关系组.empty(),
          "bounded-relations-n-plus-one");
  auto driftRelations=relationRequest;driftRelations.期望读取事实代次=g-1;driftRelations.历史截止事实代次=g-1;
  const auto relationDrift=l1.读取所有者范围有界历史关系组(driftRelations);
  require(relationDrift.状态==L1所有者范围读取状态::事实代次漂移&&relationDrift.关系组.empty(),
          "bounded-relations-generation-drift");
  auto emptyRelations=relationRequest;emptyRelations.关系类型节点=code(5);emptyRelations.最大数量=1;
  const auto relationEmpty=l1.读取所有者范围有界历史关系组(emptyRelations);
  require(relationEmpty.成功(emptyRelations)&&relationEmpty.关系组.empty(),"bounded-relations-empty");
  const L1所有者范围有界历史属性值组读取请求 valueRequest{
    L1所有者范围有界历史组读取合同版本,endpoint,g,g,2};
  const auto values=l1.读取所有者范围有界历史属性值组(valueRequest);
  require(values.成功(valueRequest)&&values.属性值组.size()==2&&
          values.属性值组[0].编码.值<values.属性值组[1].编码.值,
          "bounded-values-exact-and-sorted");
  auto shortValues=valueRequest;shortValues.最大数量=1;
  const auto valueShort=l1.读取所有者范围有界历史属性值组(shortValues);
  require(valueShort.状态==L1所有者范围读取状态::数量预算不足&&valueShort.属性值组.empty(),
          "bounded-values-n-plus-one");
  L1所有者范围写集请求 exit{L1所有者范围CRUD合同版本,g,{0xB103}};
  exit.退出事实.push_back(edge1);
  const auto exited=delivery.写入端口->提交所有者范围中性写集(exit);
  require(exited.状态==L1所有者范围写入状态::成功,"bounded-relation-exit");
  const auto afterExit=generation(l1);
  auto afterExitRequest=relationRequest;afterExitRequest.期望读取事实代次=afterExit;
  afterExitRequest.历史截止事实代次=afterExit;
  const auto afterExitRead=l1.读取所有者范围有界历史关系组(afterExitRequest);
  require(afterExitRead.成功(afterExitRequest)&&afterExitRead.关系组.size()==1,
          "bounded-relations-exit-filter");
  const auto cleaned=l1.执行L1物理清理({L1物理清理合同版本,afterExit,{0xB104},
      {{L1物理清理事实种类::关系,edge1}}});
  require(cleaned.成功(),"bounded-relation-cleanup");
  const auto afterCleanup=generation(l1);
  auto cleanedHistory=relationRequest;cleanedHistory.期望读取事实代次=afterCleanup;
  const auto cleanedRead=l1.读取所有者范围有界历史关系组(cleanedHistory);
  require(cleanedRead.成功(cleanedHistory)&&cleanedRead.关系组.size()==1,
          "bounded-relations-cleaned-item-excluded");
  L1所有者范围写集请求 exitNode{L1所有者范围CRUD合同版本,afterCleanup,{0xB105}};
  exitNode.退出事实.push_back(isolated);
  const auto exitedNode=delivery.写入端口->提交所有者范围中性写集(exitNode);
  require(exitedNode.状态==L1所有者范围写入状态::成功,"bounded-endpoint-exit");
  const auto nodeExitG=generation(l1);
  const auto cleanedNode=l1.执行L1物理清理({L1物理清理合同版本,nodeExitG,{0xB106},
      {{L1物理清理事实种类::节点,isolated}}});
  require(cleanedNode.成功(),"bounded-endpoint-cleanup");
  const auto nodeCleanupG=generation(l1);
  const L1所有者范围有界历史关系组读取请求 cleanedEndpointRequest{
    L1所有者范围有界历史组读取合同版本,L1所有者范围关系端点方向::源,
    isolated,type,nodeCleanupG,g,1};
  const auto cleanedEndpoint=l1.读取所有者范围有界历史关系组(cleanedEndpointRequest);
  require(cleanedEndpoint.状态==L1所有者范围读取状态::历史材料已清理&&
          cleanedEndpoint.关系组.empty(),"bounded-relations-cleaned-history");
}

概念树预算 concept_budget() { return {1024,8192,1024,1024,1024,1024,1024,1024}; }
二次关系预算 rc_budget() { return {16384,16384,16384,32768,1024,1024,64,1024,1024,1024,32768}; }

struct Fixture {
  std::unique_ptr<L1事实基座运行包> runtime;
  L1所有者范围交付 fdef,finfo,existOwner,sceneOwner,stateOwner,conceptOwner,externalOwner;
  std::unique_ptr<特征值类数据服务> values;
  std::unique_ptr<特征类数据服务> features;
  std::unique_ptr<存在类数据服务> existences;
  std::unique_ptr<状态类数据服务> states;
  std::unique_ptr<场景类数据服务> scenes;
  std::unique_ptr<概念树类数据服务> concepts;
  纯概念结构交付_v2 pure;
  特征概念出生使用结构交付 birth;
  存在概念两组结构交付_v1 two;
  二次关系结构交付 rc;
  稳定编码 provider{},relationType{};

  Fixture() {
    runtime=std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto& l1=runtime->读取服务(); auto& issuer=runtime->所有者范围签发器();
    fdef=owner(issuer,0xA101); finfo=owner(issuer,0xA102); existOwner=owner(issuer,0xA103);
    sceneOwner=owner(issuer,0xA104); stateOwner=owner(issuer,0xA105); conceptOwner=owner(issuer,0xA106);
    externalOwner=owner(issuer,0xA107);
    const auto producers=metadata(*fdef.写入端口,l1,0xA111,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});
    relationType=producers.front(); provider=producers.back();
    std::vector<Spec> definitions(16,{节点种类::普通,std::nullopt});
    for(auto i:{2,9,13}) definitions[i]={节点种类::属性类型,L1所有者范围值表示种类::U64组};
    for(auto i:{3,8}) definitions[i]={节点种类::属性类型,L1所有者范围值表示种类::I64};
    definitions[14]={节点种类::属性类型,L1所有者范围值表示种类::I64组};
    (void)metadata(*fdef.写入端口,l1,1,definitions);
    (void)metadata(*finfo.写入端口,l1,1,{{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},
      {节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::普通,std::nullopt}});
    const auto existenceLayout=metadata(*existOwner.写入端口,l1,0xA121,
      {{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});
    (void)metadata(*existOwner.写入端口,l1,0x455849535446414DULL,
      {{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt}});
    const auto stateLayout=metadata(*stateOwner.写入端口,l1,0xA131,
      {{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},{节点种类::普通,std::nullopt},
       {节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::U64组},
       {节点种类::属性类型,L1所有者范围值表示种类::I64},{节点种类::属性类型,L1所有者范围值表示种类::I64},
       {节点种类::属性类型,L1所有者范围值表示种类::I64}});
    values=std::make_unique<特征值类数据服务>(l1);
    features=std::make_unique<特征类数据服务>(l1,std::move(*fdef.写入端口),std::move(*finfo.写入端口),*values,producers.front());
    require(std::holds_alternative<std::monostate>(features->初始化特征定义结构())&&
            std::holds_alternative<std::monostate>(features->初始化准确特征结构()),
            "feature-structures");
    const 存在单例角色结构登记请求 roleRequest{1,generation(l1),{0x1202}};
    const auto role=存在类数据服务::登记单例角色结构(l1,*existOwner.写入端口,roleRequest);
    require(role.成功(roleRequest)&&role.交付,"existence-role-layout");
    const 实例特征结构登记请求 ifRequest{2,generation(l1),{0x4946525354525632ULL}};
    const auto ifLayout=存在类数据服务::登记实例特征结构(l1,*existOwner.写入端口,ifRequest);
    if(!(ifLayout.成功(ifRequest)&&ifLayout.交付))
      std::cerr<<"instance-feature-state="<<static_cast<unsigned>(ifLayout.状态)<<'\n';
    require(ifLayout.成功(ifRequest)&&ifLayout.交付,"existence-if-layout");
    existences=std::make_unique<存在类数据服务>(l1,*features,std::move(*existOwner.写入端口),
      existenceLayout[0],existenceLayout[1],存在当前采用结构交付{existenceLayout[2]},*ifLayout.交付,*role.交付);
    states=std::make_unique<状态类数据服务>(l1,*features,std::move(*stateOwner.写入端口),
      状态类结构交付{stateLayout[0],stateLayout[1],stateLayout[2],stateLayout[3],stateLayout[4],stateLayout[5],stateLayout[6],stateLayout[7]});
    场景结构登记请求 baseRequest{2,generation(l1),{1}};
    const auto base=场景类数据服务::登记结构类型(l1,*sceneOwner.写入端口,baseRequest);
    场景特征组织扩展登记请求 featureRequest{1,generation(l1)};
    const auto sceneFeature=场景类数据服务::登记特征组织扩展(l1,*sceneOwner.写入端口,featureRequest);
    场景直接包含扩展登记请求 includeRequest{1,generation(l1)};
    const auto include=场景类数据服务::登记直接包含扩展(l1,*sceneOwner.写入端口,includeRequest);
    require(base.成功(baseRequest)&&base.交付&&sceneFeature.成功(featureRequest)&&sceneFeature.交付&&
            include.成功(includeRequest)&&include.交付,"scene-layout");
    scenes=std::make_unique<场景类数据服务>(l1,std::move(*sceneOwner.写入端口),*base.交付,*existences,*states,*sceneFeature.交付,*include.交付);
    const 纯概念结构登记请求_v2 pureRequest{2,generation(l1),{0xA151},1048576};
    const auto pureResult=概念树类数据服务::登记纯概念结构_v2(l1,*conceptOwner.写入端口,pureRequest);
    require(pureResult.成功(pureRequest)&&pureResult.交付,"pure-layout"); pure=*pureResult.交付;
    const 特征概念出生使用结构登记请求 birthRequest{1,generation(l1),{0xA152},pure,4};
    const auto birthResult=概念树类数据服务::登记特征概念出生使用结构(l1,*conceptOwner.写入端口,birthRequest);
    require(birthResult.成功(birthRequest)&&birthResult.交付,"birth-layout"); birth=*birthResult.交付;
    const 存在概念两组结构登记请求_v1 twoRequest{1,generation(l1),{0xA153},pure,18};
    const auto twoResult=概念树类数据服务::登记存在概念两组结构_v1(l1,*conceptOwner.写入端口,twoRequest);
    require(twoResult.成功(twoRequest)&&twoResult.交付,"two-group-layout"); two=*twoResult.交付;
    const auto before=generation(l1);
    const 二次关系初始化请求 shortRequest{1,before,{0xA154},pure,52};
    const auto shortResult=初始化二次关系结构(l1,*conceptOwner.写入端口,shortRequest);
    require(shortResult.状态==二次关系数据状态::入口拒绝&&generation(l1)==before,"rc-layout-budget-52-zero-write");
    const 二次关系初始化请求 rcRequest{1,before,{0xA154},pure,53};
    const auto created=初始化二次关系结构(l1,*conceptOwner.写入端口,rcRequest);
    require(created.成功()&&created.状态==二次关系数据状态::已创建&&created.交付&&
            created.正式回执&&created.正式回执->新编码映射.size()==53,"rc-layout-create-53");
    rc=*created.交付; const auto after=generation(l1);
    const auto replay=初始化二次关系结构(l1,*conceptOwner.写入端口,rcRequest);
    if(!replay.成功()) std::cerr<<"rc-replay-state="<<static_cast<unsigned>(replay.状态)
      <<" G="<<replay.Gread<<" H="<<replay.H<<" receipt="<<replay.正式回执.has_value()
      <<" delivery="<<replay.交付.has_value()<<'\n';
    require(replay.成功()&&replay.状态==二次关系数据状态::精确重放&&replay.交付==created.交付&&generation(l1)==after,
            "rc-layout-exact-replay-zero-write");
    auto conflict=rcRequest; conflict.G0=after;
    const auto conflictResult=初始化二次关系结构(l1,*conceptOwner.写入端口,conflict);
    require(conflictResult.状态==二次关系数据状态::幂等冲突&&generation(l1)==after,"rc-layout-same-key-different-set");
    const 二次关系初始化请求 reuseRequest{1,after,{0xA155},pure,53};
    const auto reused=初始化二次关系结构(l1,*conceptOwner.写入端口,reuseRequest);
    require(reused.成功()&&reused.状态==二次关系数据状态::已复用&&!reused.正式回执&&generation(l1)==after,
            "rc-layout-complete-reuse-zero-write");
    concepts=std::make_unique<概念树类数据服务>(l1,*features,*existences,*values,*scenes,
      std::move(*conceptOwner.写入端口),pure,birth,two,rc,*scenes);
  }
  L1事实基座服务& l1() { return runtime->读取服务(); }
};

void contract_shape_probe() {
  二次关系初始化结果 invalid;
  require(!invalid.成功(),"init-success-rejects-default");
  二次关系图结果 graph; graph.状态=二次关系数据状态::已读取; graph.Gread=graph.H=1;
  require(graph.成功(),"empty-rc-graph-is-legal");
  graph.直接边.push_back({{1},{2},{3},{4},1,{1,{}}});
  require(!graph.成功(),"graph-rejects-edge-without-endpoints");
}

std::vector<std::uint64_t> rc_structure_codes(const 二次关系结构交付& delivery) {
  const auto& t=delivery.类型;
  return {
      delivery.锚点.值,delivery.规范化规则.值.值,
      t.规范化规则归属.值,t.规则版本.值,t.定义种类.值,t.定义格式.值,
      t.域掩码.值,t.输出角色.值,t.固定K.值,t.约束成员.值,t.约束FC.值,
      t.约束EC.值,t.合取成员.值,t.子RC.值,t.来源成员.值,t.来源F.值,
      t.来源B.值,t.来源概念.值,t.来源截止.值,t.用途成员.值,t.用途目标.值,
      t.用途业务依据.值,t.用途业务标识.值,t.用途角色.值,t.用途时间.值};
}

struct CrossProcessState final {
  std::uint64_t pureG0=0,rcG0=0;
  std::vector<std::uint64_t> codes;
};

void write_cross_process_state(const std::filesystem::path& path,
                               const CrossProcessState& state) {
  std::ofstream out(path,std::ios::trunc);
  require(out.is_open(),"cross-process-state-open-write");
  out<<state.pureG0<<' '<<state.rcG0<<' '<<state.codes.size()<<'\n';
  for(const auto code:state.codes) out<<code<<'\n';
  out.flush();
  require(out.good(),"cross-process-state-write");
}

CrossProcessState read_cross_process_state(const std::filesystem::path& path) {
  std::ifstream in(path);
  require(in.is_open(),"cross-process-state-open-read");
  CrossProcessState state;
  std::size_t count=0;
  require(static_cast<bool>(in>>state.pureG0>>state.rcG0>>count)&&count==25,
          "cross-process-state-header");
  state.codes.resize(count);
  for(auto& code:state.codes)
    require(static_cast<bool>(in>>code),"cross-process-state-code");
  return state;
}

int cross_process_create(const std::filesystem::path& root,
                         const std::filesystem::path& statePath) {
  auto established=建立L1事实基座持久运行包_v1({1,root});
  require(established.成功()&&
          established.恢复.状态==L1事实基座持久恢复状态_v1::已建立空仓,
          "cross-process-empty-store");
  auto runtime=std::move(established.运行包);
  auto& l1=runtime->读取服务();
  auto conceptOwner=owner(runtime->所有者范围签发器(),0xC101);
  const auto pureG0=generation(l1);
  const 纯概念结构登记请求_v2 pureRequest{2,pureG0,{0xC102},1048576};
  const auto pure=概念树类数据服务::登记纯概念结构_v2(
      l1,*conceptOwner.写入端口,pureRequest);
  require(pure.成功(pureRequest)&&pure.交付,"cross-process-pure-create");
  const auto rcG0=generation(l1);
  const 二次关系初始化请求 rcRequest{1,rcG0,{0xC103},*pure.交付,53};
  const auto rc=初始化二次关系结构(l1,*conceptOwner.写入端口,rcRequest);
  require(rc.成功()&&rc.状态==二次关系数据状态::已创建&&rc.交付,
          "cross-process-rc-create");
  write_cross_process_state(statePath,
      CrossProcessState{pureG0,rcG0,rc_structure_codes(*rc.交付)});
  std::cout<<"PASS cross-process-create total="<<passed<<'\n';
  return 0;
}

int cross_process_read(const std::filesystem::path& root,
                       const std::filesystem::path& statePath) {
  const auto state=read_cross_process_state(statePath);
  auto restored=建立L1事实基座持久运行包_v1({1,root});
  require(restored.成功()&&
          restored.恢复.状态==L1事实基座持久恢复状态_v1::已恢复,
          "cross-process-store-restored");
  auto runtime=std::move(restored.运行包);
  auto& l1=runtime->读取服务();
  auto conceptOwner=owner(runtime->所有者范围签发器(),0xC101);
  const 纯概念结构登记请求_v2 pureRequest{2,state.pureG0,{0xC102},1048576};
  const auto pure=概念树类数据服务::登记纯概念结构_v2(
      l1,*conceptOwner.写入端口,pureRequest);
  require(pure.成功(pureRequest)&&pure.交付,"cross-process-pure-replay");
  const 二次关系初始化请求 rcRequest{1,state.rcG0,{0xC103},*pure.交付,53};
  const auto rc=初始化二次关系结构(l1,*conceptOwner.写入端口,rcRequest);
  require(rc.成功()&&rc.状态==二次关系数据状态::精确重放&&rc.交付,
          "cross-process-rc-replay");
  require(rc_structure_codes(*rc.交付)==state.codes,
          "cross-process-rc-independent-readback");
  std::cout<<"PASS cross-process-read total="<<passed<<'\n';
  return 0;
}

void rc_core_probe() {
  Fixture f; auto& l1=f.l1();
  I64基础特征类型形成规格 spec;
  spec.来源=特征类型来源::外设能够获取; spec.外设提供者=f.provider;
  spec.单位绑定=I64基础特征单位绑定::新FT自身;
  spec.缩放分子=spec.缩放分母=1; spec.允许集合={{-100,100}};
  spec.域形成=I64特征域形成参数{0,f.provider};
  const I64基础特征类型定义请求 ftRequest{1,generation(l1),{0xA201},spec};
  const auto ftResult=f.features->形成或读取I64基础特征类型(ftRequest);
  if(!(ftResult.成功()&&ftResult.事实)) std::cerr<<"ft-state="<<static_cast<unsigned>(ftResult.状态)<<'\n';
  require(ftResult.成功()&&ftResult.事实,"i64-ft-create");
  const auto ft=ftResult.事实->数据.身份;
  const 特征类标量量化合同 q{ft.编码,ft.编码,ft.编码,1,1,-100,100,特征类标量量纲::有量纲};
  特征I64比较绑定定义 kd; kd.输入FT=ft; kd.用途=特征I64比较用途::概念材料;
  kd.左角色=特征I64输入角色::概念参照B; kd.右角色=特征I64输入角色::概念被描述A;
  kd.输入量化=q; kd.输出组={{{特征类标量结果角色::差异,q},ft}};
  const 特征I64比较绑定建立请求 kRequest{1,generation(l1),{0xA202},kd};
  const auto kResult=f.features->建立I64比较绑定(kRequest);
  require(kResult.成功()&&kResult.事实,"comparison-binding-create");
  const auto k=kResult.事实->身份;
  const auto kg=generation(l1);
  const 有界事实读取预算_B1 fullKBudget{4096,4096,4096,8192};
  const auto fixedK=f.features->读取I64比较绑定({1,kg,kg,k,fullKBudget});
  require(fixedK.成功()&&fixedK.事实&&fixedK.读取用量.材料总数==
          fixedK.读取用量.节点数+fixedK.读取用量.关系数+fixedK.读取用量.值数,
          "fixed-k-real-usage");
  const auto fixedLessOne=[&](unsigned axis,std::string_view name) {
    auto budget=fullKBudget;
    const std::uint64_t used[]={fixedK.读取用量.节点数,fixedK.读取用量.关系数,
      fixedK.读取用量.值数,fixedK.读取用量.材料总数};
    require(used[axis]>0,"fixed-k-axis-used");
    if(axis==0)budget.最大节点数=used[axis]-1;
    else if(axis==1)budget.最大关系数=used[axis]-1;
    else if(axis==2)budget.最大值数=used[axis]-1;
    else budget.最大材料总数=used[axis]-1;
    const auto rejected=f.features->读取I64比较绑定({1,kg,kg,k,budget});
    require(rejected.状态==特征I64比较绑定状态::数量预算不足&&!rejected.事实,name);
  };
  fixedLessOne(0,"fixed-k-node-less-one");fixedLessOne(1,"fixed-k-edge-less-one");
  fixedLessOne(2,"fixed-k-value-less-one");fixedLessOne(3,"fixed-k-total-less-one");
  const 特征I64当前比较绑定读取请求 currentKRequest{
    1,kg,ft,特征I64比较用途::概念材料,1024,fullKBudget};
  const auto currentK=f.features->读取当前I64比较绑定(currentKRequest);
  require(currentK.成功()&&currentK.事实->身份==k&&currentK.读取用量.材料总数==
          currentK.读取用量.节点数+currentK.读取用量.关系数+currentK.读取用量.值数,
          "current-k-real-usage");
  const auto currentLessOne=[&](unsigned axis,std::string_view name) {
    auto request=currentKRequest;
    const std::uint64_t used[]={currentK.读取用量.节点数,currentK.读取用量.关系数,
      currentK.读取用量.值数,currentK.读取用量.材料总数};
    require(used[axis]>0,"current-k-axis-used");
    if(axis==0)request.读取预算.最大节点数=used[axis]-1;
    else if(axis==1)request.读取预算.最大关系数=used[axis]-1;
    else if(axis==2)request.读取预算.最大值数=used[axis]-1;
    else request.读取预算.最大材料总数=used[axis]-1;
    const auto rejected=f.features->读取当前I64比较绑定(request);
    require(rejected.状态==特征I64比较绑定状态::数量预算不足&&!rejected.事实,name);
  };
  currentLessOne(0,"current-k-node-less-one");currentLessOne(1,"current-k-edge-less-one");
  currentLessOne(2,"current-k-value-less-one");currentLessOne(3,"current-k-total-less-one");
  const auto cb=concept_budget();
  const 纯概念创建请求 fcRequest{2,generation(l1),{0xA203},
    纯I64特征概念定义{概念树特征类型引用{ft.编码},{{-10,10}}},概念初始组织指定::显式顶层,{},cb};
  const auto fcResult=f.concepts->创建或复用纯概念(fcRequest);
  require(fcResult.成功(fcRequest)&&fcResult.事实,"fc-create");
  const auto fc=fcResult.事实->概念;
  特征值域比较数据服务 comparison(*f.concepts,*f.features,*f.values);
  const 特征值域比较预算_v1 comparisonBudget{
    {4096,4096,4096,4096,4096,4096,4096,4096,4096},cb,
    {4096,4096,4096,12288},4096};
  const auto domainG=generation(l1);
  const 特征值域读取请求_v1 domainRequest{1,domainG,domainG,fc,comparisonBudget};
  const auto domain=comparison.读取特征值域(domainRequest);
  require(domain.成功(domainRequest)&&domain.域&&
          domain.域->值与材料用量.最大节点数&&
          domain.域->值与材料用量.最大关系数&&
          domain.域->值与材料用量.最大值数,
          "fc-domain-real-feature-usage");
  const auto domainLessOne=[&](unsigned axis,std::string_view name) {
    auto request=domainRequest;
    const std::array<std::uint64_t,3> used{
      domain.域->值与材料用量.最大节点数,
      domain.域->值与材料用量.最大关系数,
      domain.域->值与材料用量.最大值数};
    require(used[axis]>0,"fc-domain-axis-used");
    if(axis==0)request.预算.值与材料预算.最大节点数=used[axis]-1;
    else if(axis==1)request.预算.值与材料预算.最大关系数=used[axis]-1;
    else request.预算.值与材料预算.最大值数=used[axis]-1;
    const auto rejected=comparison.读取特征值域(request);
    require(rejected.状态==特征值域比较状态_v1::数量预算不足&&
            !rejected.域,name);
  };
  domainLessOne(0,"fc-domain-node-less-one");
  domainLessOne(1,"fc-domain-edge-less-one");
  domainLessOne(2,"fc-domain-value-less-one");
  const 存在概念两组预算_v3 existenceBudget{cb,64,64,1024,1024,32768};
  const 存在概念两组创建请求_v1 specificEcRequest{
    1,generation(l1),{0xA217},{true,{{ft,fc}},true,{}},{},existenceBudget};
  const auto specificEcResult=f.concepts->创建或复用存在概念(
    specificEcRequest,comparison,comparisonBudget);
  require(specificEcResult.成功(specificEcRequest)&&specificEcResult.事实,
          "specific-ec-create");
  const auto specificEc=specificEcResult.事实->概念;
  const 纯概念创建请求 ecRequest{2,generation(l1),{0xA204},
    通用存在概念定义{1,通用存在定义规则::不预设特征},概念初始组织指定::显式顶层,{},cb};
  const auto ecResult=f.concepts->创建或复用纯概念(ecRequest);
  require(ecResult.成功(ecRequest)&&ecResult.事实,"ec-create");
  const auto ec=ecResult.事实->概念;
  const auto rb=rc_budget(); const auto g=generation(l1);
  const auto specificConstraint=f.concepts->读取二次关系约束定义(
    {1,{1,g,g},specificEc,rb});
  require(specificConstraint.成功()&&specificConstraint.定义&&
          std::holds_alternative<存在概念两组事实_v3>(*specificConstraint.定义)&&
          specificConstraint.用量.材料数==specificConstraint.用量.节点数+
            specificConstraint.用量.关系数+specificConstraint.用量.值数,
          "specific-ec-constraint-real-usage");
  二次关系原子定义 atom{{7},{{{二次关系约束角色::共同,fc}},{{二次关系约束角色::共同,ec}}},
                           k,特征类标量结果角色::差异};
  const auto normalized=f.concepts->规范化二次关系定义({1,{1,g,g},atom,rb});
  require(normalized.成功()&&normalized.规范形&&normalized.规范形->原子组.size()==1,"rc-atom-normalize");
  require(normalized.用量.材料数==normalized.用量.节点数+
          normalized.用量.关系数+normalized.用量.值数,
          "rc-normalize-physical-usage-sum");
  const auto rcLessOne=[&](unsigned axis,std::string_view name) {
    auto budget=rb;
    const std::array<std::uint64_t,4> used{normalized.用量.节点数,
      normalized.用量.关系数,normalized.用量.值数,normalized.用量.材料数};
    require(used[axis]>0,"rc-normalize-axis-used");
    if(axis==0)budget.最大节点数=used[axis]-1;
    else if(axis==1)budget.最大关系数=used[axis]-1;
    else if(axis==2)budget.最大值数=used[axis]-1;
    else budget.最大材料数=used[axis]-1;
    const auto low=f.concepts->规范化二次关系定义({1,{1,g,g},atom,budget});
    require(low.状态==二次关系数据状态::预算不足&&!low.规范形,name);
  };
  rcLessOne(0,"rc-normalize-node-less-one");
  rcLessOne(1,"rc-normalize-edge-less-one");
  rcLessOne(2,"rc-normalize-value-less-one");
  rcLessOne(3,"rc-normalize-total-less-one");
  二次关系原子定义 specificAtom{
    {7},{{{二次关系约束角色::共同,fc}},
         {{二次关系约束角色::共同,specificEc}}},
    k,特征类标量结果角色::差异};
  const auto specificNormalized=f.concepts->规范化二次关系定义(
    {1,{1,g,g},specificAtom,rb});
  require(specificNormalized.成功()&&specificNormalized.规范形&&
          specificNormalized.用量.材料数==specificNormalized.用量.节点数+
            specificNormalized.用量.关系数+specificNormalized.用量.值数,
          "specific-ec-rc-normalize-real-usage");
  const auto specificLessOne=[&](unsigned axis,std::string_view name) {
    auto budget=rb;
    const std::array<std::uint64_t,4> used{
      specificNormalized.用量.节点数,specificNormalized.用量.关系数,
      specificNormalized.用量.值数,specificNormalized.用量.材料数};
    require(used[axis]>0,"specific-ec-rc-axis-used");
    if(axis==0)budget.最大节点数=used[axis]-1;
    else if(axis==1)budget.最大关系数=used[axis]-1;
    else if(axis==2)budget.最大值数=used[axis]-1;
    else budget.最大材料数=used[axis]-1;
    const auto low=f.concepts->规范化二次关系定义(
      {1,{1,g,g},specificAtom,budget});
    require(low.状态==二次关系数据状态::预算不足&&!low.规范形,name);
  };
  specificLessOne(0,"specific-ec-rc-node-less-one");
  specificLessOne(1,"specific-ec-rc-edge-less-one");
  specificLessOne(2,"specific-ec-rc-value-less-one");
  specificLessOne(3,"specific-ec-rc-total-less-one");
  auto bad=atom; bad.D.掩码=0;
  const auto rejected=f.concepts->规范化二次关系定义({1,{1,g,g},bad,rb});
  require(!rejected.成功()&&!rejected.规范形,"rc-domain-zero-rejected");
  const 二次关系概念建立请求 create{1,{1,g,{0xA205}},atom,概念初始组织指定::显式顶层,{}, {},rb};
  const auto created=f.concepts->建立二次关系概念(create);
  if(!created.成功())
    std::cerr<<"create-state="<<static_cast<unsigned>(created.状态)
             <<" usage="<<created.用量.节点数<<'/'<<created.用量.关系数<<'/'
             <<created.用量.值数<<'/'<<created.用量.材料数
             <<" concepts="<<created.用量.概念数<<" first="<<created.用量.首次材料数<<'\n';
  require(created.成功()&&created.状态==二次关系数据状态::已创建&&created.事实,"rc-create-and-readback");
  const auto rcid=created.事实->身份; const auto current=generation(l1);
  const auto createReplay=f.concepts->建立二次关系概念(create);
  if(!createReplay.成功())
    std::cerr<<"create-replay-state="<<static_cast<unsigned>(createReplay.状态)
             <<" G="<<createReplay.Gread<<" H="
             <<(createReplay.首次H.has_value()?createReplay.首次H.value():0)
             <<" usage="<<createReplay.用量.材料数<<'/'<<createReplay.用量.首次材料数<<'\n';
  require(createReplay.成功()&&createReplay.状态==二次关系数据状态::精确重放&&
          createReplay.事实->身份==rcid&&generation(l1)==current,
          "rc-create-exact-replay-after-generation-advance");
  auto createConflict=create;
  auto conflictingAtom=atom; conflictingAtom.D.掩码=1; createConflict.定义=conflictingAtom;
  const auto createConflictResult=f.concepts->建立二次关系概念(createConflict);
  require(createConflictResult.状态==二次关系数据状态::幂等冲突&&generation(l1)==current,
          "rc-create-same-key-different-definition");
  const auto read=f.concepts->读取二次关系概念({1,{1,current,current},rcid,rb});
  require(read.成功()&&read.事实->身份==rcid,"rc-current-read");
  const auto found=f.concepts->查找二次关系完整定义({1,{1,current,current},atom,rb});
  require(found.成功()&&found.事实->身份==rcid,"rc-full-definition-find");
  const 二次关系概念建立请求 reuse{1,{1,current,{0xA206}},atom,概念初始组织指定::显式顶层,{}, {},rb};
  const auto reused=f.concepts->建立二次关系概念(reuse);
  require(reused.成功()&&reused.状态==二次关系数据状态::已复用&&reused.事实->身份==rcid,
          "rc-different-key-synonym-reuse");
  const auto sources=f.concepts->读取二次关系来源({1,{1,current,current},rcid,rb});
  const auto uses=f.concepts->读取二次关系用途({1,{1,current,current},rcid,rb});
  require(sources.成功()&&sources.来源组.empty()&&uses.成功()&&uses.用途组.empty(),"rc-empty-source-use-read");
  const auto graph=f.concepts->读取二次关系概念图({1,{1,current,current},rb});
  require(graph.成功()&&graph.RC组.size()==1&&graph.直接边.empty(),"rc-single-top-level-graph");

  const auto sourceG=generation(l1);
  const 二次关系形成来源 source{fc,sourceG};
  const 二次关系来源写入请求 sourceRequest{1,{1,sourceG,{0xA207}},rcid,{source},rb};
  const auto sourceAdded=f.concepts->添加二次关系形成来源(sourceRequest);
  require(sourceAdded.成功()&&sourceAdded.状态==二次关系数据状态::已变更&&
          sourceAdded.来源组.size()==1&&sourceAdded.来源组.front().来源==source,
          "rc-source-add");
  const auto sourceAfter=generation(l1);
  const auto sourceReplay=f.concepts->添加二次关系形成来源(sourceRequest);
  require(sourceReplay.成功()&&sourceReplay.状态==二次关系数据状态::精确重放&&
          sourceReplay.来源组.size()==1&&generation(l1)==sourceAfter,
          "rc-source-exact-replay");
  auto sourceDifferentH=sourceRequest;
  sourceDifferentH.写入头={1,sourceAfter,{0xA208}};
  sourceDifferentH.来源.front().H=sourceG-1;
  const auto sourceConflict=f.concepts->添加二次关系形成来源(sourceDifferentH);
  require(sourceConflict.状态==二次关系数据状态::引用冲突&&generation(l1)==sourceAfter,
          "rc-source-same-identity-different-h");

  const auto useG=generation(l1);
  const 二次关系用途请求 useRequest{1,{1,useG,{0xA209}},rcid,0xB001,1,7,
                                        f.provider,useG,rb};
  const auto useAdded=f.concepts->记录二次关系实际采用(useRequest);
  require(useAdded.成功()&&useAdded.状态==二次关系数据状态::已变更&&
          useAdded.用途组.size()==1,"rc-use-add");
  const auto useAfter=generation(l1);
  const auto useReplay=f.concepts->记录二次关系实际采用(useRequest);
  require(useReplay.成功()&&useReplay.状态==二次关系数据状态::精确重放&&
          useReplay.用途组.size()==1&&generation(l1)==useAfter,
          "rc-use-exact-replay");
  auto useConflict=useRequest; useConflict.时间=8;
  const auto useConflictResult=f.concepts->记录二次关系实际采用(useConflict);
  require(useConflictResult.状态==二次关系数据状态::幂等冲突&&generation(l1)==useAfter,
          "rc-use-same-key-different-payload");

  const auto currentSources=f.concepts->读取二次关系来源(
      {1,{1,useAfter,useAfter},rcid,rb});
  require(currentSources.成功()&&currentSources.来源组.size()==1,"rc-source-read-for-release");
  const 二次关系来源释放请求 releaseRequest{
      1,{1,useAfter,{0xA20A}},rcid,currentSources.来源组,rb};
  const auto released=f.concepts->释放二次关系形成来源(releaseRequest);
  require(released.成功()&&released.状态==二次关系数据状态::已变更,
          "rc-source-release");
  const auto releaseAfter=generation(l1);
  const auto releaseReplay=f.concepts->释放二次关系形成来源(releaseRequest);
  require(releaseReplay.成功()&&releaseReplay.状态==二次关系数据状态::精确重放&&
          generation(l1)==releaseAfter,"rc-source-release-exact-replay");

  auto middleAtom=atom; middleAtom.D.掩码=3;
  const auto middleG=generation(l1);
  const 二次关系概念建立请求 middleRequest{
      1,{1,middleG,{0xA20B}},middleAtom,概念初始组织指定::具名上位,{rcid},{},rb};
  const auto middleResult=f.concepts->建立二次关系概念(middleRequest);
  require(middleResult.成功()&&middleResult.状态==二次关系数据状态::已创建,
          "rc-middle-create");
  const auto middle=middleResult.事实->身份;
  auto leafAtom=atom; leafAtom.D.掩码=1;
  const auto leafG=generation(l1);
  const 二次关系概念建立请求 leafRequest{
      1,{1,leafG,{0xA20C}},leafAtom,概念初始组织指定::具名上位,{middle},{},rb};
  const auto leafResult=f.concepts->建立二次关系概念(leafRequest);
  require(leafResult.成功()&&leafResult.状态==二次关系数据状态::已创建,
          "rc-leaf-create");
  const auto leaf=leafResult.事实->身份;
  const auto cycleG=generation(l1);
  const 二次关系父组请求 cycleRequest{
      1,{1,cycleG,{0xA20D}},rcid,{}, {leaf},rb};
  const auto cycleResult=f.concepts->替换二次关系父组(cycleRequest);
  require(cycleResult.状态==二次关系数据状态::形成环&&generation(l1)==cycleG,
          "rc-parent-descendant-cycle-rejected");

  auto migrate=[&](std::uint64_t key,概念树生命周期状态 from,
                   概念树生命周期状态 to,std::string_view name) {
    const auto before=generation(l1);
    const 二次关系生命周期请求 request{
        1,{1,before,{key}},leaf,from,to,f.provider,rb};
    const auto result=f.concepts->迁移二次关系生命周期(request);
    require(result.成功()&&result.事实&&result.事实->治理状态==to,name);
  };
  migrate(0xA20E,概念树生命周期状态::活跃,概念树生命周期状态::冷却,
          "rc-life-active-to-cooling");
  migrate(0xA20F,概念树生命周期状态::冷却,概念树生命周期状态::退役,
          "rc-life-cooling-to-retired");
  migrate(0xA210,概念树生命周期状态::退役,概念树生命周期状态::活跃,
          "rc-life-retired-to-active");
  migrate(0xA211,概念树生命周期状态::活跃,概念树生命周期状态::冷却,
          "rc-life-active-to-cooling-again");
  migrate(0xA212,概念树生命周期状态::冷却,概念树生命周期状态::退役,
          "rc-life-cooling-to-retired-again");

  L1所有者范围写集请求 externalWrite{
      L1所有者范围CRUD合同版本,generation(l1),{0xA213}};
  externalWrite.节点.push_back({{1},节点种类::普通,std::nullopt});
  externalWrite.节点.push_back({{2},节点种类::普通,std::nullopt});
  externalWrite.关系.push_back({{3},L1所有者范围写集本地键{1},
                                  leaf.值,L1所有者范围写集本地键{2},1});
  const auto externalSaved=f.externalOwner.写入端口->提交所有者范围中性写集(externalWrite);
  require(externalSaved.状态==L1所有者范围写入状态::成功&&
          externalSaved.新编码映射.size()==3,"rc-external-reference-create");
  const auto blockedExitG=generation(l1);
  const 二次关系退出请求 blockedExit{
      1,{1,blockedExitG,{0xA214}},leaf,rb};
  const auto blockedExitResult=f.concepts->退出二次关系概念(blockedExit);
  require(blockedExitResult.状态==二次关系数据状态::引用冲突&&
          generation(l1)==blockedExitG,"rc-exit-external-reference-rejected");

  L1所有者范围写集请求 removeExternal{
      L1所有者范围CRUD合同版本,blockedExitG,{0xA215}};
  const auto externalRelation=std::find_if(externalSaved.新编码映射.begin(),
      externalSaved.新编码映射.end(),[](const auto& item){return item.first.值==3;});
  require(externalRelation!=externalSaved.新编码映射.end(),
          "rc-external-reference-relation-mapping");
  removeExternal.退出事实.push_back(externalRelation->second);
  const auto removed=f.externalOwner.写入端口->提交所有者范围中性写集(removeExternal);
  require(removed.状态==L1所有者范围写入状态::成功,"rc-external-reference-remove");
  const auto exitG=generation(l1);
  const 二次关系退出请求 exitRequest{1,{1,exitG,{0xA216}},leaf,rb};
  const auto exited=f.concepts->退出二次关系概念(exitRequest);
  require(exited.成功()&&exited.状态==二次关系数据状态::已变更,
          "rc-exit-after-reference-release");
  const auto exitAfter=generation(l1);
  const auto exitReplay=f.concepts->退出二次关系概念(exitRequest);
  require(exitReplay.成功()&&exitReplay.状态==二次关系数据状态::精确重放&&
          generation(l1)==exitAfter,"rc-exit-exact-replay");
}
}

int wmain(int argc,wchar_t** argv) {
  try {
    if(argc==4&&std::wstring_view(argv[1])==L"--cross-process-create")
      return cross_process_create(argv[2],argv[3]);
    if(argc==4&&std::wstring_view(argv[1])==L"--cross-process-read")
      return cross_process_read(argv[2],argv[3]);
    if(argc!=1) throw std::runtime_error("unsupported RC-P1 probe arguments");
    contract_shape_probe(); bounded_history_group_probe(); rc_core_probe(); std::cout<<"PASS total="<<passed<<'\n'; return 0;
  }
  catch(const std::exception& e) { std::cerr<<"ERROR "<<e.what()<<'\n'; return 1; }
}
