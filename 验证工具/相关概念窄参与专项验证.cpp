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

#include "../海中鱼巣/启动.应用程序.h"
#include "../海中鱼巣/领域/数据服务.世界树根.h"
#include "../海中鱼巣/领域/数据服务.概念树类.h"
#include "../海中鱼巣/业务/应用服务.世界树类.h"

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
  std::vector<稳定编码> producers;

  Fixture() {
    runtime = std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto &l1 = runtime->读取服务();
    auto &issuer = runtime->所有者范围签发器();
    fdefOwner = owner(issuer, 0x7101);
    finfoOwner = owner(issuer, 0x7102);
    existenceOwner = owner(issuer, 0x7103);
    sceneOwner = owner(issuer, 0x7104);
    stateOwner = owner(issuer, 0x7105);
    producers = metadata(
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

相关概念结构交付 related_layout(L1所有者范围写端口 &port,
                                   const L1事实基座服务 &l1) {
  using R = L1所有者范围值表示种类;
  std::vector<Spec> specs(23, {节点种类::普通, std::nullopt});
  specs[4] = {节点种类::属性类型, R::I64};
  specs[6] = {节点种类::属性类型, R::I64};
  specs[10] = {节点种类::属性类型, R::I64};
  specs[11] = {节点种类::属性类型, R::I64组};
  specs[12] = {节点种类::属性类型, R::U64组};
  specs[13] = {节点种类::属性类型, R::I64};
  specs[14] = {节点种类::属性类型, R::I64};
  specs[21] = {节点种类::属性类型, R::U64组};
  specs[22] = {节点种类::属性类型, R::I64};
  L1所有者范围写集请求 request{L1所有者范围CRUD合同版本,
      generation(l1), {0x8101}};
  for (std::uint32_t i = 0; i < specs.size(); ++i)
    request.节点.push_back({{i + 1}, specs[i].first, specs[i].second});
  for (std::uint32_t i = 0; i < 21; ++i)
    request.关系.push_back({{100 + i}, L1所有者范围写集本地键{1},
        L1所有者范围写集本地键{3 + i}, L1所有者范围写集本地键{3},
        static_cast<std::int64_t>(i + 1)});
  const auto saved = port.提交所有者范围中性写集(request);
  require(saved.状态 == L1所有者范围写入状态::成功, "related-layout-publish");
  std::vector<稳定编码> ids(23);
  for (const auto &[key, id] : saved.新编码映射)
    if (key.值 >= 1 && key.值 <= 23) ids[key.值 - 1] = id;
  for (const auto id : ids) require(有效(id), "related-layout-map");
  return {1, 1, ids[0], ids[1], {ids[2], ids[3], ids[4], ids[5], ids[6],
      ids[7], ids[8], ids[9], ids[10], ids[11], ids[12], ids[13], ids[14],
      ids[15], ids[16], ids[17], ids[18], ids[19], ids[20], ids[21], ids[22]}};
}

概念树结构交付 old_layout(L1所有者范围写端口 &port,
                            const L1事实基座服务 &l1,
                            std::uint64_t keyBase) {
  using R = L1所有者范围值表示种类;
  std::vector<Spec> typeSpecs(28, {节点种类::普通, std::nullopt});
  for (std::size_t i = 16; i < 27; ++i)
    typeSpecs[i] = {节点种类::属性类型,
                    i == 21 || i == 24 ? R::U64组
                    : i == 23 ? R::I64组 : R::I64};
  const auto types = metadata(port, l1, keyBase + 1, typeSpecs);
  const auto nodes = metadata(
      port, l1, keyBase + 2,
      std::vector<Spec>(6, {节点种类::普通, std::nullopt}));
  L1所有者范围写集请求 request{
      L1所有者范围CRUD合同版本, generation(l1), {keyBase + 3}};
  std::uint32_t key = 1;
  auto edge = [&](稳定编码 source, 稳定编码 target, 稳定编码 type,
                  std::int64_t role) {
    request.关系.push_back({{key++}, source, target, type, role});
  };
  auto attr = [&](稳定编码 node, 稳定编码 type, std::int64_t value) {
    const L1所有者范围写集本地键 local{key++};
    request.值.push_back({local, node, type, value, node});
    request.属性槽变更.push_back({node, type, local});
  };
  for (std::size_t i = 0; i < types.size(); ++i)
    edge(nodes[0], types[i], types[27], static_cast<std::int64_t>(i + 1));
  edge(nodes[0], nodes[1], types[1], 1);
  attr(nodes[0], types[16], 1);
  attr(nodes[1], types[16], 1);
  attr(nodes[1], types[17], 1);
  for (std::size_t i = 0; i < 4; ++i) {
    edge(nodes[0], nodes[i + 2], types[0], static_cast<std::int64_t>(i + 1));
    attr(nodes[i + 2], types[18], static_cast<std::int64_t>(i + 1));
  }
  const auto saved = port.提交所有者范围中性写集(request);
  require(saved.状态 == L1所有者范围写入状态::成功,
          "old-layout-publish");
  return {nodes[0],
          {概念树概念身份{nodes[2]}, 概念树概念身份{nodes[3]},
           概念树概念身份{nodes[4]}, 概念树概念身份{nodes[5]}},
          概念树规则身份{nodes[1]},
          {types[0], types[1], types[2], types[3], types[4], types[5],
           types[6], types[7], types[8], types[9], types[10], types[11],
           types[12], types[13], types[14], types[15], types[16], types[17],
           types[18], types[19], types[20], types[21], types[22], types[23],
           types[24], types[25], types[26], types[27]}};
}

L1有限N分区原子参与者写集_v3 fake_part(
    L1所有者范围写端口 &port, std::uint8_t participant,
    std::uint64_t g, std::uint64_t key) {
  L1有限N分区原子参与者写集_v3 out;
  out.参与者 = {participant}; out.所有者 = port.所有者身份();
  out.写集.合同版本 = L1所有者范围CRUD合同版本;
  out.写集.期望事实代次 = g; out.写集.写入幂等身份 = {key};
  out.写集.节点.push_back({{1}, 节点种类::普通, std::nullopt});
  return out;
}

相关概念读取预算 budget() { return {128,256,128,128,256,256,1,1}; }

void probe() {
  static_assert(static_cast<unsigned>(相关概念参与状态::旧格式不支持) == 18);
  static_assert(static_cast<unsigned>(概念树数据状态::旧格式不支持) == 35);
  using Prepare = 相关概念参与片段 (相关概念添加参与者::*)(
      const 相关概念参与请求&, std::uint64_t,
      L1有限N分区原子参与者身份_v3) const noexcept;
  static_assert(std::is_same_v<decltype(&相关概念添加参与者::准备相关概念片段), Prepare>);

  Fixture fixture;
  const 世界树根初始化请求 rootRequest{世界树根合同版本,
      generation(fixture.l1()), {0x8201}, {0x8202}, {0x8203}, 256, 64};
  const auto rootResult = fixture.roots->初始化世界树根(rootRequest);
  require(rootResult.成功(rootRequest) && rootResult.事实, "world-root");
  const auto root = rootResult.事实->根;
  const auto featureDefinitionInit = fixture.features->初始化特征定义结构();
  require(std::holds_alternative<std::monostate>(featureDefinitionInit),
          "feature-definition-structure-init");
  const auto featureInit = fixture.features->初始化准确特征结构();
  require(std::holds_alternative<std::monostate>(featureInit),
          "feature-structure-init");
  const auto ftResult = fixture.features->创建先天I64特征类型(
      {fixture.producers[0], fixture.producers[1], 1, 1,
       {{0,100}}, std::nullopt});
  const auto *ft = std::get_if<特征类型身份>(&ftResult);
  require(ft && 有效(*ft), "feature-type");

  auto conceptOwner = owner(fixture.runtime->所有者范围签发器(), 0x8301);
  const auto layout = related_layout(*conceptOwner.写入端口, fixture.l1());
  auto external1 = owner(fixture.runtime->所有者范围签发器(), 0x8302);
  auto external2 = owner(fixture.runtime->所有者范围签发器(), 0x8303);
  概念树类数据服务 concepts(fixture.l1(), *fixture.features, *fixture.existence,
      *fixture.values, *fixture.scene, std::move(*conceptOwner.写入端口), layout);

  const auto g1 = generation(fixture.l1());
  相关概念参与请求 featureRequest{1,g1,{0x8401},
      相关概念创建方案{相关概念类别::特征,
          概念树特征定义{概念树存在引用{root}, 概念树特征类型引用{ft->编码},
              概念树精确值{std::int64_t{12}}}, {}},
      {概念树存在引用{root}, g1}, budget()};
  const auto prepared = concepts.准备相关概念片段(featureRequest, g1, {2});
  require(prepared.状态 == 相关概念参与状态::已准备 && prepared.写集 &&
      prepared.写集->参与者.值 == 2 && prepared.写集->所有者 != external1.写入端口->所有者身份(),
      "prepare-feature-concept-M1");
  auto p1 = fake_part(*external1.写入端口, 1, g1, 0x8402);
  相关概念组合提交请求 submit1{1,g1,{0x8403},featureRequest,{p1}};
  L1所有者范围写端口 *ports1[]{&*external1.写入端口};
  const auto saved1 = concepts.提交相关概念组合事务(submit1, ports1);
  require(saved1.已进入L1 && saved1.事务结果.状态 == L1有限N分区原子事务状态_v3::已提交 &&
      saved1.事务结果.参与者结果组.size() == 2, "submit-M1");
  const auto featureRead = concepts.读取相关概念结果(
      featureRequest, generation(fixture.l1()), saved1.事务结果.共同事实代次);
  require(featureRead.状态 == 相关概念参与状态::已读取 && featureRead.概念 &&
      featureRead.概念->类别 == 相关概念类别::特征 &&
      featureRead.概念->挂接.所属概念 == featureRead.概念->概念,
      "feature-independent-readback");
  const auto replay1 = concepts.提交相关概念组合事务(submit1, ports1);
  require(replay1.已进入L1 && replay1.概念状态 == 相关概念参与状态::精确重复 &&
      replay1.事务结果.状态 == L1有限N分区原子事务状态_v3::精确重复,
      "original-key-replay");
  auto changed = submit1; changed.前序参与者写集组[0].写集.节点[0].种类 = 节点种类::属性类型;
  changed.前序参与者写集组[0].写集.节点[0].属性类型表示 = L1所有者范围值表示种类::I64;
  const auto conflict = concepts.提交相关概念组合事务(changed, ports1);
  require(conflict.已进入L1 && conflict.事务结果.状态 == L1有限N分区原子事务状态_v3::幂等冲突,
      "same-combination-key-different-request");

  const auto attachG = generation(fixture.l1());
  相关概念参与请求 attachRequest{1, attachG, {0x8451},
      相关概念精确复用方案{featureRead.概念->概念,
          相关概念类别::特征, featureRead.概念->定义},
      {概念树特征类型引用{ft->编码}, attachG}, budget()};
  const auto attachPrepared = concepts.准备相关概念片段(
      attachRequest, attachG, {2});
  require(attachPrepared.状态 == 相关概念参与状态::已准备 &&
      attachPrepared.写集 && attachPrepared.写集->写集.节点.size() == 1,
      "reuse-missing-attachment-only-formation");
  auto attachExternal = fake_part(*external1.写入端口, 1, attachG, 0x8452);
  相关概念组合提交请求 attachSubmit{
      1, attachG, {0x8453}, attachRequest, {attachExternal}};
  const auto attachSaved = concepts.提交相关概念组合事务(
      attachSubmit, ports1);
  const auto attachRead = concepts.读取相关概念结果(
      attachRequest, generation(fixture.l1()),
      attachSaved.事务结果.共同事实代次);
  require(attachSaved.事务结果.状态 ==
              L1有限N分区原子事务状态_v3::已提交 &&
      attachRead.状态 == 相关概念参与状态::已读取 &&
      attachRead.概念 && attachRead.概念->概念 == featureRead.概念->概念 &&
      std::holds_alternative<概念树特征类型引用>(
          attachRead.概念->挂接.世界引用),
      "reuse-missing-attachment-readback");

  const auto g2 = generation(fixture.l1());
  相关概念参与请求 existenceRequest{1,g2,{0x8501},
      相关概念创建方案{相关概念类别::存在,
          概念树存在定义{{featureRead.概念->概念}}, {}},
      {概念树存在引用{root}, g2}, budget()};
  auto e1 = fake_part(*external1.写入端口, 1, g2, 0x8502);
  auto e2 = fake_part(*external2.写入端口, 2, g2, 0x8503);
  相关概念组合提交请求 submit2{1,g2,{0x8504},existenceRequest,{e1,e2}};
  L1所有者范围写端口 *ports2[]{&*external1.写入端口,&*external2.写入端口};
  const auto saved2 = concepts.提交相关概念组合事务(submit2, ports2);
  require(saved2.已进入L1 && saved2.事务结果.状态 == L1有限N分区原子事务状态_v3::已提交 &&
      saved2.事务结果.参与者结果组.size() == 3 &&
      saved2.事务结果.参与者结果组.back().参与者.值 == 3, "submit-M2-concept3");
  const auto existenceRead = concepts.读取相关概念结果(
      existenceRequest, generation(fixture.l1()), saved2.事务结果.共同事实代次);
  require(existenceRead.状态 == 相关概念参与状态::已读取 && existenceRead.概念 &&
      existenceRead.概念->类别 == 相关概念类别::存在, "existence-independent-readback");

  const auto rejectionG = generation(fixture.l1());
  auto baseReject = featureRequest;
  baseReject.G0 = rejectionG;
  baseReject.挂靠.证据截止 = rejectionG;
  auto rejected = [&](const 相关概念参与请求 &request,
                      相关概念参与状态 expected,
                      std::string_view label) {
    const auto before = generation(fixture.l1());
    const auto result = concepts.准备相关概念片段(request, request.G0, {2});
    require(result.状态 == expected && !result.写集 &&
                generation(fixture.l1()) == before, label);
  };
  auto missingAttachment = baseReject;
  missingAttachment.挂靠.世界事实 = 概念树存在引用{};
  rejected(missingAttachment, 相关概念参与状态::挂靠无效,
           "empty-attachment-zero-write");
  auto categoryConflict = baseReject;
  std::get<相关概念创建方案>(categoryConflict.方案).类别 =
      相关概念类别::存在;
  rejected(categoryConflict, 相关概念参与状态::类别冲突,
           "category-definition-conflict-zero-write");
  auto signatureConflict = baseReject;
  signatureConflict.方案 = 相关概念精确复用方案{
      featureRead.概念->概念, 相关概念类别::特征,
      概念树特征定义{概念树存在引用{root},
          概念树特征类型引用{ft->编码},
          概念树精确值{std::int64_t{13}}}};
  rejected(signatureConflict, 相关概念参与状态::签名冲突,
           "reuse-signature-conflict-zero-write");
  auto crossClassParent = baseReject;
  std::get<相关概念创建方案>(crossClassParent.方案).直接上位 =
      {existenceRead.概念->概念};
  rejected(crossClassParent, 相关概念参与状态::类别冲突,
           "cross-class-parent-zero-write");
  auto cycle = baseReject;
  std::get<相关概念创建方案>(cycle.方案).直接上位 =
      {featureRead.概念->概念};
  rejected(cycle, 相关概念参与状态::上位成环,
           "self-cycle-zero-write");
  auto limited = baseReject;
  limited.预算 = {1,1,1,1,1,1,1,1};
  rejected(limited, 相关概念参与状态::数量预算不足,
           "budget-zero-write");
  auto futureEvidence = baseReject;
  futureEvidence.挂靠.证据截止 = rejectionG + 1;
  rejected(futureEvidence, 相关概念参与状态::挂靠无效,
           "cross-cutoff-zero-write");

  const auto beforeBad = generation(fixture.l1());
  auto badOrder = submit2; badOrder.前序参与者写集组[1].参与者 = {4};
  require(!concepts.提交相关概念组合事务(badOrder, ports2).已进入L1 &&
      generation(fixture.l1()) == beforeBad, "wrong-participant-zero-write");
  require(!concepts.提交相关概念组合事务(submit2,
      std::span<L1所有者范围写端口 *const>{ports2,1}).已进入L1 &&
      generation(fixture.l1()) == beforeBad, "missing-port-zero-write");
  L1所有者范围写端口 *wrongPorts[]{&*external2.写入端口,&*external1.写入端口};
  require(!concepts.提交相关概念组合事务(submit2, wrongPorts).已进入L1 &&
      generation(fixture.l1()) == beforeBad, "wrong-port-order-zero-write");
  auto duplicateOwner = submit2;
  duplicateOwner.前序参与者写集组[1].所有者 =
      duplicateOwner.前序参与者写集组[0].所有者;
  L1所有者范围写端口 *duplicatePorts[]{
      &*external1.写入端口, &*external1.写入端口};
  require(!concepts.提交相关概念组合事务(
      duplicateOwner, duplicatePorts).已进入L1 &&
      generation(fixture.l1()) == beforeBad,
      "duplicate-owner-zero-write");
  auto keyConflict = submit2;
  keyConflict.组合幂等身份 =
      keyConflict.前序参与者写集组[0].写集.写入幂等身份;
  require(!concepts.提交相关概念组合事务(keyConflict, ports2).已进入L1 &&
      generation(fixture.l1()) == beforeBad,
      "combination-owner-key-conflict-zero-write");

  const auto competitionG = generation(fixture.l1());
  相关概念参与请求 competitionRequest{1, competitionG, {0x8801},
      相关概念创建方案{相关概念类别::特征,
          概念树特征定义{概念树存在引用{root},
              概念树特征类型引用{ft->编码},
              概念树精确值{std::int64_t{14}}}, {}},
      {概念树存在引用{root}, competitionG}, budget()};
  auto competitionPart1 = fake_part(*external1.写入端口, 1,
      competitionG, 0x8802);
  auto competitionPart2 = fake_part(*external1.写入端口, 1,
      competitionG, 0x8803);
  相关概念组合提交请求 competition1{
      1, competitionG, {0x8804}, competitionRequest, {competitionPart1}};
  相关概念组合提交请求 competition2{
      1, competitionG, {0x8805}, competitionRequest, {competitionPart2}};
  相关概念组合提交结果 competitionResult1, competitionResult2;
  std::thread contender1([&] {
    competitionResult1 = concepts.提交相关概念组合事务(competition1, ports1);
  });
  std::thread contender2([&] {
    competitionResult2 = concepts.提交相关概念组合事务(competition2, ports1);
  });
  contender1.join();
  contender2.join();
  const auto committedCount =
      (competitionResult1.事务结果.状态 ==
       L1有限N分区原子事务状态_v3::已提交) +
      (competitionResult2.事务结果.状态 ==
       L1有限N分区原子事务状态_v3::已提交);
  const auto driftCount =
      (competitionResult1.事务结果.状态 ==
       L1有限N分区原子事务状态_v3::事实代次漂移) +
      (competitionResult2.事务结果.状态 ==
       L1有限N分区原子事务状态_v3::事实代次漂移);
  require(committedCount == 1 && driftCount == 1 &&
              generation(fixture.l1()) == competitionG + 1,
          "same-G-competition-single-commit");

  const 概念树预算 oldBudget{64,128,64,64,64,64,64,64};
  const auto pureNewG = generation(fixture.l1());
  const auto pureNewOldRead = concepts.读取概念图(
      {{1, pureNewG, pureNewG}, 概念树根角色::存在, false, oldBudget});
  require(pureNewOldRead.状态 == 概念树数据状态::旧格式不支持,
          "pure-new-old-entry-unsupported-35");

  auto oldOwner = owner(fixture.runtime->所有者范围签发器(), 0x8601);
  const auto oldLayout = old_layout(*oldOwner.写入端口, fixture.l1(), 0x8610);
  概念树类数据服务 oldOnly(fixture.l1(), *fixture.features,
      *fixture.existence, *fixture.values, std::move(*oldOwner.写入端口),
      oldLayout);
  const auto oldG = generation(fixture.l1());
  auto oldRequest = featureRequest;
  oldRequest.G0 = oldG;
  oldRequest.挂靠.证据截止 = oldG;
  const auto oldPrepare = oldOnly.准备相关概念片段(oldRequest, oldG, {2});
  const auto oldRelatedRead = oldOnly.读取相关概念结果(
      oldRequest, oldG, oldG);
  require(oldPrepare.状态 == 相关概念参与状态::旧格式不支持 &&
      !oldPrepare.写集 && oldPrepare.Gread == oldG &&
      oldRelatedRead.状态 == 相关概念参与状态::旧格式不支持 &&
      oldRelatedRead.Gread == oldG && oldRelatedRead.H == 0 &&
      !oldRelatedRead.概念,
      "old-only-new-entries-unsupported-18");

  auto migrationOwner = owner(fixture.runtime->所有者范围签发器(), 0x8701);
  const auto migrationOld = old_layout(
      *migrationOwner.写入端口, fixture.l1(), 0x8710);
  const auto migrationNew = related_layout(
      *migrationOwner.写入端口, fixture.l1());
  概念树类数据服务 migration(fixture.l1(), *fixture.features,
      *fixture.existence, *fixture.values, *fixture.scene,
      std::move(*migrationOwner.写入端口), migrationOld, migrationNew);
  const auto migrationG = generation(fixture.l1());
  auto migrationRequest = featureRequest;
  migrationRequest.G0 = migrationG;
  migrationRequest.挂靠.证据截止 = migrationG;
  const auto migrationPrepare = migration.准备相关概念片段(
      migrationRequest, migrationG, {2});
  const auto migrationOldRead = migration.读取概念(
      {{1, migrationG, migrationG}, migrationOld.根组.特征根, oldBudget});
  require(migrationPrepare.状态 == 相关概念参与状态::已准备 &&
      migrationPrepare.写集 &&
      migrationOldRead.状态 == 概念树数据状态::已读取,
      "migration-old-new-entries-independent");
  std::cout << "PASS-TOTAL " << passed << '\n';
}
} // namespace

int main() {
  try { std::cout << std::unitbuf; probe(); return 0; }
  catch (const std::exception &e) { std::cerr << e.what() << '\n'; return 1; }
  catch (...) { std::cerr << "unexpected failure\n"; return 2; }
}
