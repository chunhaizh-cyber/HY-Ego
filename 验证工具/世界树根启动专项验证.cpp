#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <utility>
#include <variant>
#include <vector>

#include "../海中鱼巣/启动选项.数据.h"
#include "../海中鱼巣/程序运行结果.数据.h"

import 海中鱼巣.启动.应用程序;
import 海中鱼巣.领域.数据服务.世界树根;
import 海中鱼巣.业务.应用服务.世界树类;

namespace {
using namespace 海中鱼巣;
int passed = 0;

void require(bool value, std::string_view name) {
  if (!value) {
    std::cerr << "FAIL " << name << '\n';
    throw std::runtime_error("world root probe failed");
  }
  ++passed;
  std::cout << "PASS " << name << '\n';
}

std::uint64_t generation(const L1事实基座服务 &l1) {
  const auto read = l1.读取中性当前事实代次({L1中性CRUD合同版本});
  require(read.状态 == L1中性读取状态::成功 && read.事实代次,
          "generation-read");
  return read.事实代次;
}

L1所有者范围交付 owner(L1所有者范围签发器 &issuer, std::uint64_t key) {
  auto result = issuer.建立所有者范围(
      {L1所有者范围CRUD合同版本, {key},
       L1所有者范围种类::独占结构范围});
  require(result.建立结果.状态 == L1所有者范围管理状态::成功 &&
              result.写入端口 && result.写入端口->有效(),
          "owner-create");
  return result;
}

using Spec = std::pair<节点种类,
                       std::optional<L1所有者范围值表示种类>>;

std::vector<稳定编码> metadata(L1所有者范围写端口 &port,
                               const L1事实基座服务 &l1,
                               std::uint64_t key,
                               const std::vector<Spec> &specs) {
  L1所有者范围写集请求 request{
      L1所有者范围CRUD合同版本, generation(l1), {key}};
  for (std::uint32_t i = 0; i < specs.size(); ++i)
    request.节点.push_back({{i + 1}, specs[i].first, specs[i].second});
  const auto saved = port.提交所有者范围中性写集(request);
  require(saved.状态 == L1所有者范围写入状态::成功 &&
              saved.新编码映射.size() == specs.size(),
          "metadata-publish");
  const auto first = port.读取首次写入材料(
      {L1所有者范围首次写入读取合同版本, {key}});
  require(first.状态 == L1所有者范围读取状态::成功 &&
              first.首次规范化写集 && first.首次写入结果 &&
              *first.首次规范化写集 == request &&
              first.首次写入结果->新编码映射 == saved.新编码映射,
          "metadata-first-ledger");
  std::vector<稳定编码> ids(specs.size());
  for (const auto &[local, id] : saved.新编码映射)
    if (local.值 && local.值 <= ids.size())
      ids[local.值 - 1] = id;
  for (std::size_t i = 0; i < ids.size(); ++i) {
    const auto read = l1.读取所有者范围当前节点(
        {L1所有者范围CRUD合同版本, ids[i]});
    const auto *node = read.事实
                           ? std::get_if<L1所有者范围节点事实>(&*read.事实)
                           : nullptr;
    require(node && node->写入所有者 == port.所有者身份() &&
                node->种类 == specs[i].first &&
                node->属性类型表示 == specs[i].second,
            "metadata-independent-readback");
  }
  return ids;
}

struct Fixture {
  std::unique_ptr<L1事实基座运行包> runtime;
  L1所有者范围交付 fdefOwner, finfoOwner, existenceOwner, sceneOwner,
      stateOwner;
  std::unique_ptr<特征值类数据服务> values;
  std::unique_ptr<特征类数据服务> features;
  std::unique_ptr<存在类数据服务> existence;
  std::unique_ptr<状态类数据服务> state;
  std::unique_ptr<场景类数据服务> scene;
  std::unique_ptr<世界树根数据服务> roots;

  Fixture() {
    runtime = std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto &l1 = runtime->读取服务();
    auto &issuer = runtime->所有者范围签发器();
    fdefOwner = owner(issuer, 0x7101);
    finfoOwner = owner(issuer, 0x7102);
    existenceOwner = owner(issuer, 0x7103);
    sceneOwner = owner(issuer, 0x7104);
    stateOwner = owner(issuer, 0x7105);
    const auto producers = metadata(
        *fdefOwner.写入端口, l1, 0x7111,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    std::vector<Spec> definitions(16, {节点种类::普通, std::nullopt});
    for (auto i : {2, 9, 13})
      definitions[i] =
          {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (auto i : {3, 8})
      definitions[i] =
          {节点种类::属性类型, L1所有者范围值表示种类::I64};
    definitions[14] =
        {节点种类::属性类型, L1所有者范围值表示种类::I64组};
    (void)metadata(*fdefOwner.写入端口, l1, 1, definitions);
    (void)metadata(
        *finfoOwner.写入端口, l1, 1,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::普通, std::nullopt}});
    const auto existenceLayout = metadata(
        *existenceOwner.写入端口, l1, 0x7121,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    (void)metadata(
        *existenceOwner.写入端口, l1, 0x455849535446414DULL,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    const auto stateLayout = metadata(
        *stateOwner.写入端口, l1, 0x7131,
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
        l1, std::move(*fdefOwner.写入端口), std::move(*finfoOwner.写入端口),
        *values, producers.front());
    existence = std::make_unique<存在类数据服务>(
        l1, *features, std::move(*existenceOwner.写入端口),
        existenceLayout[0], existenceLayout[1],
        存在当前采用结构交付{existenceLayout[2]});
    state = std::make_unique<状态类数据服务>(
        l1, *features, std::move(*stateOwner.写入端口),
        状态类结构交付{stateLayout[0], stateLayout[1], stateLayout[2],
                       stateLayout[3], stateLayout[4], stateLayout[5],
                       stateLayout[6], stateLayout[7]});
    场景结构登记请求 baseRequest{2, generation(l1), {1}};
    const auto base = 场景类数据服务::登记结构类型(
        l1, *sceneOwner.写入端口, baseRequest);
    场景特征组织扩展登记请求 featureRequest{1, generation(l1)};
    const auto feature = 场景类数据服务::登记特征组织扩展(
        l1, *sceneOwner.写入端口, featureRequest);
    场景直接包含扩展登记请求 includeRequest{1, generation(l1)};
    const auto include = 场景类数据服务::登记直接包含扩展(
        l1, *sceneOwner.写入端口, includeRequest);
    require(base.成功(baseRequest) && base.交付 &&
                feature.成功(featureRequest) && feature.交付 &&
                include.成功(includeRequest) && include.交付,
            "scene-seven-node-layout");
    scene = std::make_unique<场景类数据服务>(
        l1, std::move(*sceneOwner.写入端口), *base.交付, *existence, *state,
        *feature.交付, *include.交付);
    roots = std::make_unique<世界树根数据服务>(*existence, *scene);
  }

  L1事实基座服务 &l1() { return runtime->读取服务(); }
};

void old_layout_probe() {
  auto runtime = 建立L1事实基座运行包();
  auto delivery = owner(runtime.所有者范围签发器(), 0x7201);
  const auto before = generation(runtime.读取服务());
  (void)metadata(*delivery.写入端口, runtime.读取服务(), 1,
                 std::vector<Spec>(6, {节点种类::普通, std::nullopt}));
  场景结构登记请求 request{2, generation(runtime.读取服务()), {1}};
  const auto read = 场景类数据服务::登记结构类型(
      runtime.读取服务(), *delivery.写入端口, request);
  require(read.状态 == 场景角色数据状态::旧版本迁移拒绝 &&
              generation(runtime.读取服务()) == request.G0 &&
              request.G0 == before + 1,
          "legacy-six-node-layout-zero-write");
}

void concurrent_root_probe() {
  Fixture fixture;
  const 世界树根初始化请求 request{
      世界树根合同版本, generation(fixture.l1()), {0x7251}, {0x7252},
      {0x7253}, 256, 64};
  世界树根初始化结果 results[2];
  std::thread first([&] { results[0] = fixture.roots->初始化世界树根(request); });
  std::thread second([&] { results[1] = fixture.roots->初始化世界树根(request); });
  first.join();
  second.join();
  const auto g = generation(fixture.l1());
  const 世界树根组读取请求 read{世界树根合同版本, g, 16};
  const auto roots = fixture.scene->读取当前世界树根组(read);
  require((results[0].成功(request) || results[1].成功(request)) &&
              roots.成功(read) && roots.根组.size() == 1,
          "same-request-concurrent-single-root");
}

void core_probe() {
  old_layout_probe();
  concurrent_root_probe();
  Fixture fixture;
  const 世界树根初始化请求 rootRequest{
      世界树根合同版本, generation(fixture.l1()), {0x7301}, {0x7302},
      {0x7303}, 256, 64};
  const auto created = fixture.roots->初始化世界树根(rootRequest);
  require(created.成功(rootRequest) &&
              created.状态 == 世界树根状态::已建立 && created.事实 &&
              !created.事实->根场景.直接父 &&
              !created.事实->根场景.父语境投影,
          "atomic-empty-L1-root");
  const auto root = created.事实->根;
  const auto afterRoot = generation(fixture.l1());
  const auto replay = fixture.roots->初始化世界树根(rootRequest);
  require(replay.成功(rootRequest) &&
              replay.状态 == 世界树根状态::精确重复 &&
              replay.事实->根 == root && replay.首次发布H == created.首次发布H &&
              generation(fixture.l1()) == afterRoot,
          "root-exact-replay-zero-write");
  auto other = rootRequest;
  other.G0 = afterRoot;
  other.组合幂等身份 = {0x7311};
  other.存在幂等身份 = {0x7312};
  other.场景幂等身份 = {0x7313};
  const auto rejected = fixture.roots->初始化世界树根(other);
  require(rejected.状态 == 世界树根状态::已有不同根 &&
              generation(fixture.l1()) == afterRoot,
          "different-second-root-zero-write");
  世界树根验证请求 verify{2, afterRoot, {128, 256}};
  auto built = 建立世界树应用服务(*fixture.scene, *fixture.existence, verify);
  require(built.成功() && built.投影->根 == root &&
              built.服务->验证现实世界根(verify).成功(verify),
          "factory-independent-root-readback");
  auto &world = *built.服务;
  世界树层级位置读取请求 rootLocation{
      世界树应用合同版本, afterRoot, root,
      世界树节点视角::世界根场景, {128, 256}};
  const auto rootPosition = world.读取世界树层级位置(rootLocation);
  require(rootPosition.成功(rootLocation) && rootPosition.位置 &&
              !rootPosition.位置->直接结构父 &&
              !rootPosition.位置->父场景语境,
          "unified-root-location");

  世界树场景创建请求 childRequest{
      3, generation(fixture.l1()), {0x7401}, {0x7402}, {0x7403}, root,
      {128, 256, 128}};
  auto child = world.创建场景并纳入现实世界(childRequest);
  if (!child.成功(childRequest))
    std::cerr << "child status=" << static_cast<int>(child.结果头.状态)
              << " stage=" << static_cast<int>(child.结果头.阶段)
              << " G=" << child.结果头.Gread
              << " H=" << child.结果头.首次发布H.value_or(0)
              << " binding="
              << (child.结果头.绑定原因
                      ? static_cast<int>(*child.结果头.绑定原因)
                      : -1)
              << " projection=" << static_cast<bool>(child.投影) << '\n';
  require(child.成功(childRequest) && child.投影 &&
              child.投影->场景.直接父 &&
              child.投影->场景.父语境投影 &&
              child.投影->场景.直接父->关系 ==
                  child.投影->场景.父语境投影->结构父.关系,
          "direct-child-structure-and-context");
  const auto childId = child.投影->E;
  世界树层级位置读取请求 childLocation{
      世界树应用合同版本, generation(fixture.l1()), childId,
      世界树节点视角::场景, {128, 256}};
  auto childPosition = world.读取世界树层级位置(childLocation);
  require(childPosition.成功(childLocation) && childPosition.位置 &&
              childPosition.位置->世界根 == root &&
              childPosition.位置->直接结构父 &&
              childPosition.位置->父场景语境,
          "unified-child-scene-location");

  世界树场景创建请求 grandchildRequest{
      3, generation(fixture.l1()), {0x7404}, {0x7405}, {0x7406}, childId,
      {128, 256, 128}};
  auto grandchild = world.创建场景并纳入现实世界(grandchildRequest);
  require(grandchild.成功(grandchildRequest) && grandchild.投影,
          "deep-child-create-regression");
  const auto grandchildId = grandchild.投影->E;

  世界树存在创建请求 memberRequest{
      3, generation(fixture.l1()), {0x7411}, {0x7412}, {0x7413}, root,
      {128, 256, 128}};
  auto member = world.创建存在并纳入现实世界(memberRequest);
  require(member.成功(memberRequest) && member.投影,
          "existing-binding-create-regression");
  const auto memberId = member.投影->存在;
  世界树层级位置读取请求 memberLocation{
      世界树应用合同版本, generation(fixture.l1()), memberId,
      世界树节点视角::存在, {128, 256}};
  auto memberPosition = world.读取世界树层级位置(memberLocation);
  require(memberPosition.成功(memberLocation) && memberPosition.位置 &&
              memberPosition.位置->直接结构父 &&
              memberPosition.位置->世界根 == root,
          "unified-member-location");

  const auto structural = *memberPosition.位置->直接结构父;
  场景角色启用请求 enable{4,
                           generation(fixture.l1()),
                           {0x7421},
                           memberId,
                           root,
                           structural.关系,
                           256,
                           128};
  auto enabled = fixture.scene->启用场景角色(
      enable, 直接归属联合只读组合器(*fixture.existence, *fixture.scene));
  require(enabled.启用成功(enable) && enabled.结构父 &&
              enabled.结构父->关系 == structural.关系 &&
              enabled.父语境投影 &&
              enabled.父语境投影->结构父.关系 == structural.关系,
          "existing-E-scene-view-preserves-structural-parent");
  const auto afterEnable = generation(fixture.l1());
  const auto parentEdges = fixture.scene->读取当前场景包含父组(
      {1, afterEnable, memberId, 256});
  require(parentEdges.父组读取成功({1, afterEnable, memberId, 256}) &&
              parentEdges.包含组.size() == 1 &&
              parentEdges.包含组.front().关系.编码 == structural.关系,
          "existing-E-no-new-child-edge");
  场景角色退出请求 exit{4, afterEnable, {0x7422}, memberId, root,
                          structural.关系, 256};
  const auto exited = fixture.scene->退出场景角色(exit);
  require(exited.退出成功(exit) && exited.结构父 &&
              exited.结构父->关系 == structural.关系 &&
              exited.父语境投影 &&
              exited.父语境投影->投影边.生命周期.退出事实代次,
          "scene-view-exit-preserves-structural-parent");
  const auto jointAfter = 直接归属联合只读组合器(
      *fixture.existence, *fixture.scene).读取当前联合父(
      {1, generation(fixture.l1()), memberId, 256});
  require(jointAfter.父 && jointAfter.父->关系 == structural.关系,
          "structural-parent-remains-after-exit");
  const auto beforeV3 = generation(fixture.l1());
  auto oldEnable = enable;
  oldEnable.版本 = 3;
  oldEnable.G0 = beforeV3;
  oldEnable.幂等身份 = {0x7431};
  require(fixture.scene
                  ->启用场景角色(
                      oldEnable,
                      直接归属联合只读组合器(*fixture.existence,
                                             *fixture.scene))
                  .状态 == 场景角色数据状态::旧版本迁移拒绝 &&
              generation(fixture.l1()) == beforeV3,
          "v3-enable-migration-reject-zero-write");
  auto oldExit = exit;
  oldExit.版本 = 3;
  oldExit.G0 = beforeV3;
  oldExit.幂等身份 = {0x7432};
  require(fixture.scene->退出场景角色(oldExit).状态 ==
                  场景角色数据状态::旧版本迁移拒绝 &&
              generation(fixture.l1()) == beforeV3,
          "v3-exit-migration-reject-zero-write");
  世界树层级位置读取请求 tiny{
      世界树应用合同版本, generation(fixture.l1()), grandchildId,
      世界树节点视角::场景, {1, 1}};
  const auto limited = world.读取世界树层级位置(tiny);
  if (limited.结果头.状态 != 世界树操作状态::数量预算不足)
    std::cerr << "limited status="
              << static_cast<int>(limited.结果头.状态) << " G="
              << limited.结果头.Gread << '\n';
  require(!limited.成功(tiny) &&
              limited.结果头.状态 == 世界树操作状态::数量预算不足,
          "hierarchy-budget-failure");

  世界树成员移动请求 memberMove{
      2, generation(fixture.l1()), {0x7441}, root, childId, memberId,
      {128, 256}};
  const auto movedMember = world.移动现实世界存在(memberMove);
  require(movedMember.成功(memberMove), "existing-member-move-regression");
  世界树成员移动请求 sceneMove{
      2, generation(fixture.l1()), {0x7442}, childId, root, grandchildId,
      {128, 256}};
  const auto movedScene = world.移动现实世界场景(sceneMove);
  if (!movedScene.成功(sceneMove))
    std::cerr << "scene-move status="
              << static_cast<int>(movedScene.结果头.状态) << " reason="
              << (movedScene.结果头.场景原因
                      ? static_cast<int>(*movedScene.结果头.场景原因)
                      : -1)
              << " H=" << movedScene.结果头.首次发布H.value_or(0) << '\n';
  require(movedScene.成功(sceneMove), "existing-scene-move-regression");
  const auto afterSceneMove = generation(fixture.l1());
  const auto movedSceneReplay = world.移动现实世界场景(sceneMove);
  require(movedSceneReplay.成功(sceneMove) &&
              movedSceneReplay.结果头.状态 == 世界树操作状态::精确重复 &&
              generation(fixture.l1()) == afterSceneMove,
          "scene-four-edge-move-exact-replay");
  std::cout << "PASS-TOTAL " << passed << '\n';
}
} // namespace

int main(int argc, char **argv) {
  try {
    std::cout << std::unitbuf;
    if (argc == 2 && std::string_view(argv[1]) == "--ordinary") {
      const auto result = 运行海中鱼巣(
          {启动模式::普通控制面板});
      require(result.状态 == 程序运行状态::初始化失败 &&
                  result.失败阶段 == 程序失败阶段::真实自我形成,
              "ordinary-entry-stops-at-stage-17");
      return 0;
    }
    core_probe();
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "unexpected failure\n";
    return 2;
  }
}
