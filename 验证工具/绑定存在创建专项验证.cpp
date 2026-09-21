#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "../海中鱼巣/领域/数据服务.世界树根.h"
#include "../海中鱼巣/领域/数据服务.概念树类.h"
#include "../海中鱼巣/领域/数据服务.特征值域比较类.h"
#include "../海中鱼巣/业务/应用服务.世界树类.h"

namespace {
using namespace 海中鱼巣;

int 通过数 = 0;

void 要求(bool ok, std::string_view name) {
  if (!ok) {
    std::cerr << "FAIL " << name << '\n';
    throw std::runtime_error("bound existence probe failed");
  }
  ++通过数;
  std::cout << "PASS " << name << '\n';
}

std::uint64_t 当前代次(const L1事实基座服务 &l1) {
  const auto result = l1.读取中性当前事实代次({L1中性CRUD合同版本});
  要求(result.状态 == L1中性读取状态::成功 && result.事实代次,
       "current-generation");
  return result.事实代次;
}

L1所有者范围交付 建立所有者(L1所有者范围签发器 &issuer,
                                std::uint64_t key) {
  auto result = issuer.建立所有者范围(
      {L1所有者范围CRUD合同版本, {key}, L1所有者范围种类::独占结构范围});
  要求(result.建立结果.状态 == L1所有者范围管理状态::成功 &&
           result.写入端口 && result.写入端口->有效(),
       "owner-create");
  return result;
}

using 元节点规格 =
    std::pair<节点种类, std::optional<L1所有者范围值表示种类>>;

std::vector<稳定编码> 建立元节点(L1所有者范围写端口 &port,
                                  const L1事实基座服务 &l1,
                                  std::uint64_t key,
                                  const std::vector<元节点规格> &specs) {
  L1所有者范围写集请求 request{
      L1所有者范围CRUD合同版本, 当前代次(l1), {key}};
  for (std::uint32_t i = 0; i < specs.size(); ++i)
    request.节点.push_back({{i + 1}, specs[i].first, specs[i].second});
  const auto saved = port.提交所有者范围中性写集(request);
  要求(saved.状态 == L1所有者范围写入状态::成功 &&
           saved.新编码映射.size() == specs.size(),
       "metadata-write");
  std::vector<稳定编码> ids(specs.size());
  for (const auto &[local, id] : saved.新编码映射)
    if (local.值 && local.值 <= ids.size())
      ids[local.值 - 1] = id;
  for (const auto id : ids)
    要求(有效(id), "metadata-id");
  return ids;
}

struct 夹具 final {
  std::unique_ptr<L1事实基座运行包> runtime;
  L1所有者范围交付 特征定义所有者, 特征信息所有者, 存在所有者,
      场景所有者, 状态所有者, 概念所有者;
  std::vector<稳定编码> 特征来源;
  std::unique_ptr<特征值类数据服务> values;
  std::unique_ptr<特征类数据服务> features;
  std::unique_ptr<存在类数据服务> existence;
  std::unique_ptr<状态类数据服务> state;
  std::unique_ptr<场景类数据服务> scene;
  std::unique_ptr<概念树类数据服务> concepts;
  std::unique_ptr<世界树根数据服务> roots;

  夹具() {
    runtime = std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto &l1 = runtime->读取服务();
    auto &issuer = runtime->所有者范围签发器();
    特征定义所有者 = 建立所有者(issuer, 0x6101);
    特征信息所有者 = 建立所有者(issuer, 0x6102);
    存在所有者 = 建立所有者(issuer, 0x6103);
    场景所有者 = 建立所有者(issuer, 0x6104);
    状态所有者 = 建立所有者(issuer, 0x6105);
    概念所有者 = 建立所有者(issuer, 0x6106);

    特征来源 = 建立元节点(
        *特征定义所有者.写入端口, l1, 0x6111,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    std::vector<元节点规格> definitions(
        16, {节点种类::普通, std::nullopt});
    for (const auto i : {2, 9, 13})
      definitions[i] =
          {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (const auto i : {3, 8})
      definitions[i] =
          {节点种类::属性类型, L1所有者范围值表示种类::I64};
    definitions[14] =
        {节点种类::属性类型, L1所有者范围值表示种类::I64组};
    (void)建立元节点(*特征定义所有者.写入端口, l1, 1, definitions);
    (void)建立元节点(
        *特征信息所有者.写入端口, l1, 1,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::普通, std::nullopt}});

    const auto existenceLayout = 建立元节点(
        *存在所有者.写入端口, l1, 0x6121,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    (void)建立元节点(
        *存在所有者.写入端口, l1, 0x455849535446414DULL,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    const auto stateLayout = 建立元节点(
        *状态所有者.写入端口, l1, 0x6131,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::U64组},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64}});

    values = std::make_unique<特征值类数据服务>(l1);
    features = std::make_unique<特征类数据服务>(
        l1, std::move(*特征定义所有者.写入端口),
        std::move(*特征信息所有者.写入端口), *values, 特征来源.front());

    const 存在单例角色结构登记请求 roleRequest{
        1, 当前代次(l1), {0x1202}};
    const auto role = 存在类数据服务::登记单例角色结构(
        l1, *存在所有者.写入端口, roleRequest);
    要求(role.成功(roleRequest) && role.交付, "existence-role-layout");
    const 实例特征结构登记请求 instanceFeatureRequest{
        2, 当前代次(l1), {0x4946525354525632ULL}};
    const auto instanceFeature = 存在类数据服务::登记实例特征结构(
        l1, *存在所有者.写入端口, instanceFeatureRequest);
    要求(instanceFeature.成功(instanceFeatureRequest) && instanceFeature.交付,
         "existence-instance-feature-layout");
    existence = std::make_unique<存在类数据服务>(
        l1, *features, std::move(*存在所有者.写入端口), existenceLayout[0],
        existenceLayout[1], 存在当前采用结构交付{existenceLayout[2]},
        *instanceFeature.交付, *role.交付);

    state = std::make_unique<状态类数据服务>(
        l1, *features, std::move(*状态所有者.写入端口),
        状态类结构交付{stateLayout[0], stateLayout[1], stateLayout[2],
                       stateLayout[3], stateLayout[4], stateLayout[5],
                       stateLayout[6], stateLayout[7]});
    const 场景结构登记请求 sceneBaseRequest{2, 当前代次(l1), {1}};
    const auto sceneBase = 场景类数据服务::登记结构类型(
        l1, *场景所有者.写入端口, sceneBaseRequest);
    const 场景特征组织扩展登记请求 sceneFeatureRequest{
        1, 当前代次(l1)};
    const auto sceneFeature = 场景类数据服务::登记特征组织扩展(
        l1, *场景所有者.写入端口, sceneFeatureRequest);
    const 场景直接包含扩展登记请求 sceneIncludeRequest{
        1, 当前代次(l1)};
    const auto sceneInclude = 场景类数据服务::登记直接包含扩展(
        l1, *场景所有者.写入端口, sceneIncludeRequest);
    要求(sceneBase.成功(sceneBaseRequest) && sceneBase.交付 &&
             sceneFeature.成功(sceneFeatureRequest) && sceneFeature.交付 &&
             sceneInclude.成功(sceneIncludeRequest) && sceneInclude.交付,
         "scene-layout");
    scene = std::make_unique<场景类数据服务>(
        l1, std::move(*场景所有者.写入端口), *sceneBase.交付, *existence,
        *state, *sceneFeature.交付, *sceneInclude.交付);

    const 纯概念结构登记请求_v2 pureRequest{
        2, 当前代次(l1), {0x6161}, 1048576};
    const auto pure = 概念树类数据服务::登记纯概念结构_v2(
        l1, *概念所有者.写入端口, pureRequest);
    要求(pure.成功(pureRequest) && pure.交付, "concept-pure-layout");
    const 特征概念出生使用结构登记请求 birthRequest{
        1, 当前代次(l1), {0x6162}, *pure.交付, 4};
    const auto birth = 概念树类数据服务::登记特征概念出生使用结构(
        l1, *概念所有者.写入端口, birthRequest);
    要求(birth.成功(birthRequest) && birth.交付,
         "concept-birth-layout");
    const 存在概念两组结构登记请求_v1 twoGroupRequest{
        1, 当前代次(l1), {0x6163}, *pure.交付, 18};
    const auto twoGroup = 概念树类数据服务::登记存在概念两组结构_v1(
        l1, *概念所有者.写入端口, twoGroupRequest);
    要求(twoGroup.成功(twoGroupRequest) && twoGroup.交付,
         "concept-two-group-layout");
    concepts = std::make_unique<概念树类数据服务>(
        l1, *features, *existence, *values, *scene,
        std::move(*概念所有者.写入端口), *pure.交付, *birth.交付,
        *twoGroup.交付);
    roots = std::make_unique<世界树根数据服务>(*existence, *scene);
  }

  L1事实基座服务 &l1() { return runtime->读取服务(); }
};

void 验证绑定业务(夹具 &fixture, 稳定编码 root,
                  世界树应用服务 &world,
                  const 概念树预算 &conceptBudget) {
  const 世界树场景创建请求_v4 childRequest{
      4,
      当前代次(fixture.l1()),
      root,
      通用存在概念定义{1, 通用存在定义规则::不预设特征},
      概念初始组织指定::显式顶层,
      {},
      {0x6201},
      存在场景概念创建键_v2{{0x6202}, {0x6203}, {0x6204}, {0x6205}},
      std::nullopt,
      {128, 256, 128},
      conceptBudget};
  const auto child = world.创建场景并纳入现实世界(childRequest);
  要求(child.成功(childRequest) && child.投影 &&
           child.投影->内容.绑定.种类 == 存在初始绑定种类::直接子场景,
       "bound-child-scene-created");
  const auto afterChild = 当前代次(fixture.l1());
  const auto childReplay = world.创建场景并纳入现实世界(childRequest);
  要求(childReplay.成功(childRequest) &&
           childReplay.状态 == 世界树概念创建状态_v4::精确重复 &&
           当前代次(fixture.l1()) == afterChild,
       "bound-child-scene-replay-zero-write");

  const 世界树存在创建请求_v4 memberRequest{
      4,
      当前代次(fixture.l1()),
      {存在初始绑定种类::场景成员, root},
      通用存在概念定义{1, 通用存在定义规则::不预设特征},
      概念初始组织指定::显式顶层,
      {},
      {0x6211},
      存在场景概念创建键_v2{{0x6212}, {0x6213}, {0x6214}, {0x6215}},
      std::nullopt,
      {128, 256, 128},
      conceptBudget};
  const auto member = world.创建存在并纳入现实世界(memberRequest);
  要求(member.成功(memberRequest) && member.投影 &&
           member.投影->内容.绑定.种类 == 存在初始绑定种类::场景成员,
       "bound-scene-member-created");
  const auto memberId = member.投影->E;
  const auto afterMember = 当前代次(fixture.l1());
  const auto memberReplay = world.创建存在并纳入现实世界(memberRequest);
  要求(memberReplay.成功(memberRequest) &&
           memberReplay.状态 == 世界树概念创建状态_v4::精确重复 &&
           当前代次(fixture.l1()) == afterMember,
       "bound-scene-member-replay-zero-write");

  const 世界树存在创建请求_v4 componentRequest{
      4,
      当前代次(fixture.l1()),
      {存在初始绑定种类::父存在组成, memberId},
      通用存在概念定义{1, 通用存在定义规则::不预设特征},
      概念初始组织指定::显式顶层,
      {},
      {0x6221},
      存在组成概念创建键_v2{{0x6222}, {0x6223}, {0x6224}},
      std::nullopt,
      {128, 256, 128},
      conceptBudget};
  const auto component = world.创建存在并纳入现实世界(componentRequest);
  要求(component.成功(componentRequest) && component.投影 &&
           component.投影->内容.绑定.种类 == 存在初始绑定种类::父存在组成,
       "bound-parent-composition-created");

  auto invalid = memberRequest;
  invalid.G0 = 当前代次(fixture.l1());
  invalid.绑定.绑定节点 = {};
  invalid.概念定义键 = {0x6231};
  invalid.世界键 =
      存在场景概念创建键_v2{{0x6232}, {0x6233}, {0x6234}, {0x6235}};
  const auto beforeInvalid = 当前代次(fixture.l1());
  const auto rejected = world.创建存在并纳入现实世界(invalid);
  要求(!rejected.成功(invalid) &&
           rejected.状态 == 世界树概念创建状态_v4::入口拒绝 &&
           当前代次(fixture.l1()) == beforeInvalid,
       "bound-invalid-target-zero-write");
}

void 验证当前概念(夹具 &fixture, const 概念树预算 &conceptBudget) {
  auto &l1 = fixture.l1();
  const 存在概念两组预算_v3 existenceBudget{
      conceptBudget, 10000, 10000, 10000, 10000, 10000};
  const 特征值域比较预算_v1 comparisonBudget{
      {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000},
      conceptBudget,
      10000};
  特征值域比较数据服务 comparison(
      *fixture.concepts, *fixture.features, *fixture.values);
  要求(comparison.绑定于(l1), "concept-current-provider-ready");

  std::vector<特征类型身份> featureTypes;
  std::vector<概念树概念身份> featureConcepts;
  for (std::uint32_t i = 0; i < 3; ++i) {
    const auto ftResult = fixture.features->形成或读取I64基础特征类型(
        {1,
         当前代次(l1),
         {0x4654424153450000ULL + i},
         {特征类型来源::外设能够获取,
          fixture.特征来源[0],
          I64基础特征单位绑定::既有稳定单位,
          fixture.特征来源[1],
          i + 1,
          1,
          {{0, 100}},
          I64特征域形成参数{2, fixture.特征来源[0]}}});
    要求(ftResult.成功() && ftResult.事实, "concept-feature-type");
    const auto ft = ftResult.事实->数据.身份;
    const 纯概念定义 definition = 纯I64特征概念定义{
        概念树特征类型引用{ft.编码}, {{0, 100}}};
    const 纯概念创建请求 request{
        2, 当前代次(l1), {0x6301 + i}, definition,
        概念初始组织指定::显式顶层, {}, conceptBudget};
    const auto result = fixture.concepts->创建或复用纯概念(request);
    要求(result.成功(request) && result.事实,
         "concept-feature-top-level");
    featureTypes.push_back(ft);
    featureConcepts.push_back(result.事实->概念);
  }

  auto request = [&](std::vector<存在概念特征值域项_v3> features,
                     std::vector<概念树概念身份> parents,
                     std::uint64_t key) {
    return 存在概念两组创建请求_v1{
        1,
        当前代次(l1),
        {key},
        {true, std::move(features), true, {}},
        std::move(parents),
        existenceBudget};
  };

  const auto firstRequest = request(
      {{featureTypes[0], featureConcepts[0]}}, {}, 0x6401);
  const auto first = fixture.concepts->创建或复用存在概念(
      firstRequest, comparison, comparisonBudget);
  要求(first.成功(firstRequest) && first.事实 &&
           first.事实->直接上位.empty(),
       "concept-empty-parent-top-level");
  const auto afterFirst = 当前代次(l1);
  const auto firstReplay = fixture.concepts->创建或复用存在概念(
      firstRequest, comparison, comparisonBudget);
  要求(firstReplay.成功(firstRequest) &&
           firstReplay.状态 == 存在概念两组状态_v3::精确重复 &&
           当前代次(l1) == afterFirst,
       "concept-exact-replay-zero-write");

  const auto secondRequest = request(
      {{featureTypes[1], featureConcepts[1]}}, {}, 0x6402);
  const auto second = fixture.concepts->创建或复用存在概念(
      secondRequest, comparison, comparisonBudget);
  要求(second.成功(secondRequest) && second.事实,
       "concept-second-top-level");

  const auto multiRequest = request(
      {{featureTypes[2], featureConcepts[2]}},
      {first.事实->概念, second.事实->概念}, 0x6403);
  const auto multi = fixture.concepts->创建或复用存在概念(
      multiRequest, comparison, comparisonBudget);
  要求(multi.成功(multiRequest) && multi.事实 &&
           multi.事实->直接上位.size() == 2,
       "concept-multiple-same-category-parents");

  const auto wrongRequest = request(
      {{featureTypes[2], featureConcepts[2]}}, {featureConcepts[0]}, 0x6404);
  const auto beforeWrong = 当前代次(l1);
  const auto wrong = fixture.concepts->创建或复用存在概念(
      wrongRequest, comparison, comparisonBudget);
  要求(!wrong.成功(wrongRequest) &&
           wrong.状态 == 存在概念两组状态_v3::类别冲突 &&
           当前代次(l1) == beforeWrong,
       "concept-wrong-category-parent-zero-write");

  const auto readG = 当前代次(l1);
  const 存在概念两组读取请求_v1 readRequest{
      1, readG, multi.H, multi.事实->概念, existenceBudget};
  const auto read = fixture.concepts->读取存在概念两组定义(
      readRequest, comparison, comparisonBudget);
  要求(read.成功(readRequest) && read.事实 &&
           read.事实->定义 == multiRequest.定义 &&
           read.事实->直接上位.size() == 2,
       "concept-current-two-group-readback");
}

} // namespace

int main() {
  try {
    std::cout << std::unitbuf;
    夹具 fixture;
    要求(std::holds_alternative<std::monostate>(
             fixture.features->初始化特征定义结构()),
         "feature-definition-init");
    要求(std::holds_alternative<std::monostate>(
             fixture.features->初始化准确特征结构()),
         "feature-information-init");

    const 世界树根初始化请求 rootRequest{
        世界树根合同版本, 当前代次(fixture.l1()), {0x6501}, {0x6502},
        {0x6503}, 256, 64};
    const auto rootResult = fixture.roots->初始化世界树根(rootRequest);
    要求(rootResult.成功(rootRequest) && rootResult.事实,
         "world-root-created");
    const auto root = rootResult.事实->根;
    const 世界树根验证请求 verify{
        2, 当前代次(fixture.l1()), {128, 256}};
    auto worldResult = 建立世界树应用服务(
        *fixture.scene, *fixture.existence, *fixture.concepts, verify);
    要求(worldResult.成功() && worldResult.服务,
         "world-service-current-constructor");

    const 概念树预算 conceptBudget{
        10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
    验证绑定业务(fixture, root, *worldResult.服务, conceptBudget);
    验证当前概念(fixture, conceptBudget);

    std::cout << "BOUND_EXISTENCE_OK " << 通过数
              << " G=" << 当前代次(fixture.l1()) << '\n';
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "ERROR " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "ERROR unexpected failure\n";
    return 2;
  }
}
