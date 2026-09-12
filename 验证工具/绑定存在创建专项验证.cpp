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
import 海中鱼巣.领域.数据服务.绑定存在;
import 海中鱼巣.业务.应用服务.世界树类;
import 海中鱼巣.领域.数据服务.概念树类;
namespace {
using namespace 海中鱼巣;
int 通过数 = 0;
void 要求(bool ok, std::string_view name) {
  if (!ok) {
    std::cerr << "FAIL " << name << '\n';
    throw std::runtime_error("probe failed");
  }
  ++通过数;
  std::cout << "PASS " << name << '\n';
}
std::uint64_t 当前代次(const L1事实基座服务 &l1) {
  auto r = l1.读取中性当前事实代次({L1中性CRUD合同版本});
  要求(r.状态 == L1中性读取状态::成功 && r.事实代次, "current-generation");
  return r.事实代次;
}
L1所有者范围交付 建立所有者(L1所有者范围签发器 &issuer, std::uint64_t key) {
  auto r = issuer.建立所有者范围(
      {L1所有者范围CRUD合同版本, {key}, L1所有者范围种类::独占结构范围});
  要求(r.建立结果.状态 == L1所有者范围管理状态::成功 && r.写入端口,
       "owner-create");
  return r;
}
std::vector<稳定编码> 建立元节点(
    L1所有者范围写端口 &port, const L1事实基座服务 &l1, std::uint64_t key,
    const std::vector<
        std::pair<节点种类, std::optional<L1所有者范围值表示种类>>> &specs) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, 当前代次(l1), {key}};
  for (std::uint32_t i = 0; i < specs.size(); ++i)
    ws.节点.push_back({{i + 1}, specs[i].first, specs[i].second});
  auto saved = port.提交所有者范围中性写集(ws);
  要求(saved.状态 == L1所有者范围写入状态::成功, "metadata-write");
  std::vector<稳定编码> ids(specs.size());
  for (const auto &[k, id] : saved.新编码映射)
    if (k.值 && k.值 <= ids.size())
      ids[k.值 - 1] = id;
  for (auto id : ids)
    要求(有效(id), "metadata-id");
  return ids;
}
// 故障仅注入只读参与者回执；真实存在 owner 与 L1 提交路径不替换。
class 只读故障场景 final : public 绑定存在场景参与者,
                           private 场景直接包含只读提供者 {
  const L1事实基座服务 &l1_;
  L1所有者范围写端口 port_;
  const 场景直接包含只读提供者 &source_;

public:
  mutable unsigned calls = 0;
  unsigned failOn = 1;
  绑定存在创建状态 fault = 绑定存在创建状态::资源失败;
  std::optional<场景直接包含状态> currentFault;
  std::uint64_t failAfterG = 0;
  mutable unsigned currentFaultCalls = 0;
  只读故障场景(const L1事实基座服务 &l, L1所有者范围写端口 p,
               const 场景直接包含只读提供者 &s)
      : l1_(l), port_(std::move(p)), source_(s) {}

private:
  const L1事实基座服务 &绑定存在底座() const noexcept override { return l1_; }
  const L1所有者范围写端口 &绑定存在端口() const noexcept override {
    return port_;
  }
  bool 绑定存在结构已就绪() const noexcept override { return true; }
  bool 绑定存在幂等键可用(L1所有者范围写入幂等身份 k) const noexcept override {
    return k.值 > 1;
  }
  const 场景直接包含只读提供者 &绑定存在场景提供者() const noexcept override {
    return *this;
  }
  bool 绑定于(const L1事实基座服务 &l) const noexcept override {
    return source_.绑定于(l);
  }
  场景直接包含组结果
  读取当前场景包含父组(const 场景直接包含反向读取请求 &r) const override {
    if (currentFault && r.G0 > failAfterG) {
      ++currentFaultCalls;
      场景直接包含组结果 out;
      out.结果头.状态 = *currentFault;
      out.结果头.Gread = r.G0;
      return out;
    }
    return source_.读取当前场景包含父组(r);
  }
  场景直接包含组结果
  读取当前场景包含子组(const 场景直接包含组读取请求 &r) const override {
    return source_.读取当前场景包含子组(r);
  }
  直接归属场景角色读取结果
  读取当前场景角色位置(const 直接归属场景角色读取请求 &r) const override {
    return source_.读取当前场景角色位置(r);
  }
  绑定存在参与者结果<L1所有者范围首次写入读取结果>
  读取场景绑定首次材料(L1所有者范围写入幂等身份 k) const override {
    auto f = port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, k});
    return {绑定存在创建状态::精确重复, f.读取事实代次, f.读取事实代次, f};
  }
  绑定存在参与者结果<绑定现实树见证>
  核验绑定现实树(std::uint64_t g, const 存在初始绑定 &b, 稳定编码 root,
                 const 绑定存在创建预算 &budget,
                 const 直接归属联合只读提供者 &joint) const override {
    using S = 绑定存在创建状态;
    if (++calls == failOn)
      return {fault, g, g, {}};
    auto role = joint.读取当前场景角色位置({1, g, root});
    if (!role.成功({1, g, root}) || !role.角色)
      return {S::内部不一致, g, g, {}};
    绑定现实树见证 v{g, b.绑定节点, root, *role.角色, {}};
    auto cursor = b.绑定节点;
    while (cursor != root) {
      if (v.上行路径.size() >= budget.最大祖先数量)
        return {S::数量预算不足, g, g, {}};
      直接归属联合父读取请求 q{1, g, cursor, budget.最大关系数量};
      auto edge = joint.读取当前联合父(q);
      if (!edge.父读取成功(q) || !edge.父)
        return {S::内部不一致, g, g, {}};
      v.上行路径.push_back(*edge.父);
      cursor = edge.父->父;
    }
    return {S::精确重复, g, g, v};
  }
  绑定存在参与者结果<L1有限N分区原子参与者写集_v3>
  准备场景绑定片段(const 绑定存在创建请求 &r, std::uint64_t g) const override {
    return {绑定存在创建状态::内部不一致, g, r.G0, {}};
  }
  绑定存在参与者结果<场景绑定出生见证>
  读取场景绑定出生(std::uint64_t g, std::uint64_t h, 稳定编码,
                   const 存在初始绑定 &) const override {
    return {绑定存在创建状态::内部不一致, g, h, {}};
  }
};

} // namespace
int main(int argc, char **argv) {
  using namespace 海中鱼巣;
  try {
    std::cout << std::unitbuf;
    auto runtime = 建立L1事实基座运行包();
    auto &l1 = runtime.读取服务();
    auto &issuer = runtime.所有者范围签发器();
    auto fdef = 建立所有者(issuer, 101), finfo = 建立所有者(issuer, 102);
    auto fmeta = 建立元节点(
        *fdef.写入端口, l1, 1001,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    // 起点模型存量夹具：唯一端口在move前写入真实原布局，绝非新的公开无父入口。
    std::vector<std::pair<节点种类, std::optional<L1所有者范围值表示种类>>> ds(
        16, {节点种类::普通, std::nullopt});
    for (auto i : {2, 9, 13})
      ds[i] = {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (auto i : {3, 8})
      ds[i] = {节点种类::属性类型, L1所有者范围值表示种类::I64};
    ds[14] = {节点种类::属性类型, L1所有者范围值表示种类::I64组};
    auto dl = 建立元节点(*fdef.写入端口, l1, 1, ds);
    auto il = 建立元节点(*finfo.写入端口, l1, 1,
                         {{节点种类::普通, std::nullopt},
                          {节点种类::普通, std::nullopt},
                          {节点种类::属性类型, L1所有者范围值表示种类::I64},
                          {节点种类::属性类型, L1所有者范围值表示种类::I64},
                          {节点种类::普通, std::nullopt}});
    L1所有者范围写集请求 tw{L1所有者范围CRUD合同版本, 当前代次(l1), {1002}};
    tw.节点 = {{{1}, 节点种类::属性类型, L1所有者范围值表示种类::I64}};
    tw.关系 = {{{2}, L1所有者范围写集本地键{1}, dl[0], dl[1], 1},
               {{3}, L1所有者范围写集本地键{1}, fmeta[0], dl[4], 1},
               {{4}, L1所有者范围写集本地键{1}, fmeta[1], dl[5], 1}};
    tw.值 = {{{5},
              L1所有者范围写集本地键{1},
              dl[2],
              std::vector<std::uint64_t>{1, 1, 1, 0, 100},
              fmeta[0]}};
    tw.属性槽变更 = {{L1所有者范围写集本地键{1}, dl[2], {5}}};
    auto ts = fdef.写入端口->提交所有者范围中性写集(tw);
    要求(ts.状态 == L1所有者范围写入状态::成功, "legacy-FT-seed");
    稳定编码 legacyFT{};
    for (auto [k, id] : ts.新编码映射)
      if (k.值 == 1)
        legacyFT = id;
    std::vector<稳定编码> legacyF, legacyValues;
    std::vector<std::uint64_t> legacyBirth;
    for (std::int64_t value : {60, 60, 70}) {
      L1所有者范围写集请求 fw{
          L1所有者范围CRUD合同版本, 当前代次(l1), {1100 + legacyF.size()}};
      fw.节点 = {{{1}, 节点种类::普通, std::nullopt}};
      fw.关系 = {{{2}, L1所有者范围写集本地键{1}, il[0], il[1], 1},
                 {{3}, L1所有者范围写集本地键{1}, legacyFT, il[4], 1}};
      fw.值 = {{{4}, L1所有者范围写集本地键{1}, il[2], value, fmeta[0]}};
      fw.属性槽变更 = {{L1所有者范围写集本地键{1}, il[2], {4}}};
      auto fs = finfo.写入端口->提交所有者范围中性写集(fw);
      要求(fs.状态 == L1所有者范围写入状态::成功, "legacy-F-seed");
      for (auto [k, id] : fs.新编码映射) {
        if (k.值 == 1)
          legacyF.push_back(id);
        if (k.值 == 4)
          legacyValues.push_back(id);
      }
      legacyBirth.push_back(fs.事实代次);
    }
    特征值类数据服务 values(l1);
    特征类数据服务 features(l1, std::move(*fdef.写入端口),
                            std::move(*finfo.写入端口), values, fmeta[0]);
    auto eo = 建立所有者(issuer, 103);
    auto em = 建立元节点(*eo.写入端口, l1, 2001,
                         {{节点种类::普通, std::nullopt},
                          {节点种类::普通, std::nullopt},
                          {节点种类::普通, std::nullopt}});
    // 固定现实根夹具：owner 端口交付前登记现有存在族布局与根身份。
    auto family = 建立元节点(
        *eo.写入端口, l1, 0x455849535446414DULL,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    L1所有者范围写集请求 rootSeed{
        L1所有者范围CRUD合同版本, 当前代次(l1), {2101}};
    rootSeed.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    rootSeed.关系 = {{{2}, L1所有者范围写集本地键{1}, family[0], family[1], 1}};
    // 只在端口交付前布置存量坏图，验证读取拒绝；不增加生产绕过入口。
    rootSeed.节点.push_back({{3}, 节点种类::普通, std::nullopt});
    rootSeed.节点.push_back({{5}, 节点种类::普通, std::nullopt});
    rootSeed.关系.push_back(
        {{4}, L1所有者范围写集本地键{3}, family[0], family[1], 1});
    rootSeed.关系.push_back(
        {{6}, L1所有者范围写集本地键{5}, family[0], family[1], 1});
    rootSeed.关系.push_back(
        {{7}, L1所有者范围写集本地键{3}, L1所有者范围写集本地键{5}, em[0], 1});
    rootSeed.关系.push_back(
        {{8}, L1所有者范围写集本地键{5}, L1所有者范围写集本地键{3}, em[0], 1});
    auto rootSaved = eo.写入端口->提交所有者范围中性写集(rootSeed);
    要求(rootSaved.状态 == L1所有者范围写入状态::成功, "fixed-root-seed");
    稳定编码 C{}, cycleA{};
    for (auto [key, id] : rootSaved.新编码映射)
      if (key.值 == 1)
        C = id;
    for (auto [key, id] : rootSaved.新编码映射)
      if (key.值 == 3)
        cycleA = id;
    存在类数据服务 existence(l1, features, std::move(*eo.写入端口), em[0],
                             em[1], {em[2]});
    auto so = 建立所有者(issuer, 104), sto = 建立所有者(issuer, 105);
    auto sm = 建立元节点(*sto.写入端口, l1, 3001,
                         {{节点种类::普通, std::nullopt},
                          {节点种类::普通, std::nullopt},
                          {节点种类::普通, std::nullopt},
                          {节点种类::属性类型, L1所有者范围值表示种类::I64},
                          {节点种类::属性类型, L1所有者范围值表示种类::U64组},
                          {节点种类::属性类型, L1所有者范围值表示种类::I64},
                          {节点种类::属性类型, L1所有者范围值表示种类::I64},
                          {节点种类::属性类型, L1所有者范围值表示种类::I64}});
    状态类数据服务 state(
        l1, features, std::move(*sto.写入端口),
        {sm[0], sm[1], sm[2], sm[3], sm[4], sm[5], sm[6], sm[7]});
    场景结构登记请求 br{2, 当前代次(l1), {1}};
    auto base = 场景类数据服务::登记结构类型(l1, *so.写入端口, br);
    要求(base.成功(br), "scene-base");
    场景特征组织扩展登记请求 er{1, 当前代次(l1)};
    auto ext = 场景类数据服务::登记特征组织扩展(l1, *so.写入端口, er);
    要求(ext.成功(er), "scene-feature-layout");
    场景直接包含扩展登记请求 ir{1, 当前代次(l1)};
    auto inc = 场景类数据服务::登记直接包含扩展(l1, *so.写入端口, ir);
    要求(inc.成功(ir), "scene-containment-layout");
    场景类数据服务 scene(l1, std::move(*so.写入端口), *base.交付, existence,
                         state, *ext.交付, *inc.交付);
    直接归属联合只读组合器 joint(existence, scene);
    场景树根启用请求 rootRequest{1, 当前代次(l1), {4101}, C, 100, 100};
    auto rootCreated = scene.启用并建立场景树根(rootRequest, joint);
    要求(rootCreated.建根成功(rootRequest), "fixed-root-tree");

    绑定存在数据服务 bound(existence, scene);
    世界树应用服务 world(scene, existence, C);
    using K = 存在初始绑定种类;
    using S = 绑定存在创建状态;
    std::uint64_t next = 5000;
    auto request = [&](K kind, 稳定编码 parent) {
      const auto key = (next += 10);
      绑定存在创建请求 q{1,
                         当前代次(l1),
                         {kind, parent},
                         C,
                         存在场景绑定创建键{{key}, {key + 1}, {key + 2}},
                         {100, 100, 100}};
      if (kind == K::父存在组成)
        q.幂等键 = 存在组成绑定创建键{{key}};
      return q;
    };
    auto create = [&](绑定存在创建请求 q, const char *name) {
      auto a = bound.创建绑定存在(q);
      if (!a.成功(q))
        std::cerr << "bound-status=" << int(a.状态) << " G=" << a.Gread
                  << " H=" << a.首次发布H.value_or(0) << "\n";
      要求(a.成功(q) && a.状态 == S::已创建 && a.Gread == q.G0 + 1, name);
      const auto before = 当前代次(l1);
      auto twice = bound.创建绑定存在(q);
      要求(twice.成功(q) && twice.状态 == S::精确重复 &&
               twice.事实->新存在 == a.事实->新存在 && 当前代次(l1) == before,
           "exact-repeat-zero-write");
      return a;
    };
    auto memberQ = request(K::场景成员, C);
    auto member = create(memberQ, "root-empty-path-member");
    auto malformed = member;
    malformed.事实->绑定节点 = {};
    要求(!malformed.成功(memberQ), "success-requires-binding");
    malformed = member;
    malformed.首次发布H = memberQ.G0;
    要求(!malformed.成功(memberQ), "success-requires-new-H");
    malformed = member;
    malformed.状态 = S::已可能发布;
    malformed.事实.reset();
    要求(!malformed.成功(memberQ) && malformed.原请求 == memberQ,
         "unknown-is-not-success-and-retains-original");
    auto childQ = request(K::直接子场景, C);
    auto child = create(childQ, "root-empty-path-child-scene");
    auto compQ = request(K::父存在组成, member.事实->新存在);
    auto comp = create(compQ, "single-owner-composition");
    auto deepQ = request(K::场景成员, child.事实->新存在);
    auto deep = create(deepQ, "child-scene-member");
    auto exitChildQ = request(K::直接子场景, C);
    auto exitChild = create(exitChildQ, "parent-bound-child-for-exit");
    const auto beforeBadExit = 当前代次(l1);
    场景角色退出请求 zeroParentExit{
        3, beforeBadExit, {7991}, exitChild.事实->新存在, {}};
    auto zeroParentRejected = scene.退出场景角色(zeroParentExit);
    要求(zeroParentRejected.状态 == 场景角色数据状态::入口拒绝 &&
             当前代次(l1) == beforeBadExit,
         "scene-exit-requires-parent");
    场景角色退出请求 exitScene{
        3, 当前代次(l1), {7992}, exitChild.事实->新存在, C};
    auto sceneExited = scene.退出场景角色(exitScene);
    if (!sceneExited.退出成功(exitScene))
      std::cerr << "scene-exit-status=" << int(sceneExited.状态)
                << " G=" << sceneExited.Gread
                << " H=" << sceneExited.首次发布代次 << "\n";
    要求(sceneExited.退出成功(exitScene), "scene-parent-and-role-atomic-exit");
    const auto afterSceneExit = 当前代次(l1);
    auto sceneExitReplay = scene.退出场景角色(exitScene);
    if (!sceneExitReplay.退出成功(exitScene))
      std::cerr << "scene-exit-replay-status=" << int(sceneExitReplay.状态)
                << " G=" << sceneExitReplay.Gread
                << " H=" << sceneExitReplay.首次发布代次 << "\n";
    要求(sceneExitReplay.退出成功(exitScene) &&
             sceneExitReplay.状态 == 场景角色数据状态::精确重复 &&
             当前代次(l1) == afterSceneExit,
         "scene-parent-exit-exact-replay");
    auto wrongExit = exitScene;
    wrongExit.父场景 = child.事实->新存在;
    auto wrongExitResult = scene.退出场景角色(wrongExit);
    要求(wrongExitResult.状态 == 场景角色数据状态::幂等冲突 &&
             当前代次(l1) == afterSceneExit,
         "scene-exit-same-key-different-parent");
    for (const auto *a : {&member, &child, &comp, &deep}) {
      const auto g = 当前代次(l1);
      直接归属联合父读取请求 q{1, g, a->事实->新存在, 100};
      auto p = joint.读取当前联合父(q);
      要求(p.父读取成功(q) && p.父 && p.父->父 == a->事实->绑定节点 &&
               p.父->关系 == a->事实->绑定关系,
           "independent-unique-parent-readback");
    }
    auto reject = [&](绑定存在创建请求 q, S status, const char *name) {
      const auto g = 当前代次(l1);
      auto a = bound.创建绑定存在(q);
      if (a.状态 != status)
        std::cerr << name << " status=" << int(a.状态)
                  << " expected=" << int(status) << "\n";
      要求(a.状态 == status && !a.事实 && !a.首次发布H && 当前代次(l1) == g,
           name);
    };
    auto cycleQ = request(K::父存在组成, cycleA);
    reject(cycleQ, S::成环, "real-legacy-cycle-zero-write");
    auto bad = request(K::场景成员, C);
    bad.绑定.绑定节点 = {};
    reject(bad, S::绑定未明确, "zero-binding");
    bad = request(K::场景成员, C);
    bad.绑定.种类 = static_cast<K>(255);
    reject(bad, S::绑定未明确, "invalid-binding-kind");
    bad = request(K::场景成员, C);
    bad.绑定.绑定节点 = {0x7fffffffffffULL};
    reject(bad, S::绑定未找到, "missing-binding");
    bad = request(K::场景成员, C);
    bad.绑定.绑定节点 = em[0];
    reject(bad, S::绑定类型不符, "wrong-type-binding");
    bad = request(K::场景成员, C);
    bad.期望现实树根 = child.事实->新存在;
    reject(bad, S::绑定不在现实树, "wrong-reality-root");
    bad = request(K::场景成员, C);
    --bad.G0;
    reject(bad, S::事实代次漂移, "fresh-stale-generation");
    bad = request(K::场景成员, C);
    bad.预算.最大祖先数量 = 0;
    reject(bad, S::入口拒绝, "zero-budget");
    bad = memberQ;
    bad.绑定.绑定节点 = child.事实->新存在;
    reject(bad, S::幂等冲突, "same-keys-different-binding");
    bad = request(K::场景成员, comp.事实->新存在);
    reject(bad, S::绑定类型不符, "member-cannot-bind-as-scene");
    // 旧G重放先核对首次账，不会再次创建。
    const auto advanced = 当前代次(l1);
    auto old = bound.创建绑定存在(memberQ);
    要求(old.成功(memberQ) && old.状态 == S::精确重复 && old.Gread == advanced,
         "old-G-exact-replay");
    世界树场景创建请求 wsc{3, 当前代次(l1),   {8001}, {8002}, {8003},
                           C, {100, 100, 100}};
    auto wsr = world.创建场景并纳入现实世界(wsc);
    要求(wsr.成功(wsc), "world-v3-scene-atomic-create");
    auto brokenScene = wsr;
    brokenScene.投影->场景.场景角色.四根[0].根.编码 = {};
    要求(!brokenScene.成功(wsc), "world-rejects-missing-root-projection");
    brokenScene = wsr;
    brokenScene.投影->场景.直接父.reset();
    要求(!brokenScene.成功(wsc), "world-rejects-missing-parent-projection");
    世界树存在创建请求 we{3,      当前代次(l1), {8011},         {8012},
                          {8013}, wsr.投影->E,  {100, 100, 100}};
    auto wer = world.创建存在并纳入现实世界(we);
    要求(wer.成功(we), "world-v3-member-atomic-create");
    auto brokenMember = wer;
    brokenMember.投影->位置.成员关系.关系.编码 = {};
    要求(!brokenMember.成功(we), "world-rejects-missing-member-edge");
    auto wer2 = world.创建存在并纳入现实世界(we);
    要求(wer2.成功(we) && wer2.结果头.状态 == 世界树操作状态::精确重复,
         "world-v3-old-G-replay");
    场景直接包含迁移请求 move{
        1,   当前代次(l1), {8101}, C, child.事实->新存在, member.事实->新存在,
        100, 100};
    auto moved = scene.迁移直接存在成员(move, joint);
    要求(moved.成功(move), "legal-successor-move");
    const auto movedG = 当前代次(l1);
    auto covered = bound.创建绑定存在(memberQ);
    要求(covered.状态 == S::既有操作已被后继事实覆盖 &&
             covered.首次发布H == member.首次发布H && !covered.事实 &&
             当前代次(l1) == movedG,
         "replay-covered-no-recreation");
    auto pathParentQ = request(K::场景成员, C);
    auto pathParent = create(pathParentQ, "path-parent-create");
    auto pathChildQ = request(K::父存在组成, pathParent.事实->新存在);
    auto pathChild = create(pathChildQ, "path-child-create");
    场景直接包含写请求 detachPath{
        1, 当前代次(l1), {8111}, C, pathParent.事实->新存在, 100, 100};
    要求(scene.退出直接存在成员(detachPath, joint).成功(),
         "binding-parent-legally-detached-from-tree");
    const auto pathG = 当前代次(l1);
    auto stillSameParent =
        joint.读取当前联合父({1, pathG, pathChild.事实->新存在, 100});
    要求(stillSameParent.父 &&
             stillSameParent.父->父 == pathParent.事实->新存在,
         "child-direct-parent-unchanged-after-path-detach");
    auto pathCovered = bound.创建绑定存在(pathChildQ);
    要求(pathCovered.状态 == S::既有操作已被后继事实覆盖 &&
             pathCovered.首次发布H == pathChild.首次发布H &&
             !pathCovered.事实 && 当前代次(l1) == pathG,
         "replay-covered-after-binding-path-detach");
    reject(request(K::父存在组成, pathParent.事实->新存在), S::绑定不在现实树,
           "first-create-detached-binding-exact-rejection");
    // 原生首次账不完整时拒绝补写：已有根出生键与不存在的第二 owner 键。
    auto ownerScoped = request(K::场景成员, C);
    ownerScoped.幂等键 = 存在场景绑定创建键{
        {1}, 场景特征组织扩展登记固定幂等身份, {0x455849535446414DULL}};
    auto scopedCreated =
        create(ownerScoped, "owner-local-reserved-values-do-not-cross-reject");
    auto reserved = request(K::场景成员, C);
    std::get<存在场景绑定创建键>(reserved.幂等键).存在幂等身份 = {
        0x455849535446414DULL};
    reject(reserved, S::入口拒绝, "existence-owner-reserved-key-rejected");
    reserved = request(K::场景成员, C);
    std::get<存在场景绑定创建键>(reserved.幂等键).场景幂等身份 =
        场景特征组织扩展登记固定幂等身份;
    reject(reserved, S::入口拒绝, "scene-owner-reserved-key-rejected");
    auto partial = request(K::场景成员, C);
    partial.G0 = rootSeed.期望事实代次;
    partial.幂等键 = 存在场景绑定创建键{{9001}, {2101}, {9002}};
    reject(partial, S::内部不一致, "partial-first-material-no-repair");
    auto mismatch = partial;
    mismatch.幂等键 = 存在场景绑定创建键{{9011}, {2101}, {4101}};
    reject(mismatch, S::内部不一致, "different-first-H-no-repair");
    auto missingMap = request(K::父存在组成, C);
    missingMap.G0 = rootSeed.期望事实代次;
    missingMap.幂等键 = 存在组成绑定创建键{{2101}};
    reject(missingMap, S::内部不一致, "first-map-missing-binding");
    auto budgetQ = request(K::父存在组成, comp.事实->新存在);
    budgetQ.预算.最大祖先数量 = 1;
    reject(budgetQ, S::数量预算不足, "ancestor-budget-no-truncation");
    场景直接包含写请求 detach{
        1,   当前代次(l1), {9021}, child.事实->新存在, deep.事实->新存在,
        100, 100};
    auto detached = scene.退出直接存在成员(detach, joint);
    要求(detached.成功(), "detach-before-existence-exit");
    存在类删除请求 exitE{
        存在类数据合同版本, 当前代次(l1), {9022}, deep.事实->新存在};
    auto exited = existence.删除存在(exitE);
    要求(exited.成功(), "existing-existence-exit");
    auto exitedQ = request(K::父存在组成, deep.事实->新存在);
    reject(exitedQ, S::绑定已退出, "exited-binding-zero-write");
    auto faultOwner = 建立所有者(issuer, 107);
    只读故障场景 faulty(l1, std::move(*faultOwner.写入端口), scene);
    绑定存在数据服务 faultBound(existence, faulty);
    for (const auto status : {S::资源失败, S::事实代次漂移, S::成环,
                              S::包含冲突, S::历史材料已清理}) {
      faulty.calls = 0;
      faulty.failOn = 1;
      faulty.fault = status;
      auto q = request(K::父存在组成, member.事实->新存在);
      const auto g = 当前代次(l1);
      auto r = faultBound.创建绑定存在(q);
      要求(r.状态 == status && !r.事实 && !r.首次发布H && 当前代次(l1) == g,
           "injected-read-failure-zero-write");
    }
    faulty.calls = 0;
    faulty.failOn = 2;
    faulty.fault = S::资源失败;
    auto unknownQ = request(K::父存在组成, member.事实->新存在);
    auto unknown = faultBound.创建绑定存在(unknownQ);
    要求(unknown.状态 == S::资源失败 && !unknown.首次发布H && !unknown.事实 &&
             当前代次(l1) == unknownQ.G0 + 1,
         "confirmed-publication-read-failure-exact-status");
    faulty.calls = 0;
    faulty.failOn = 0;
    const auto unknownG = 当前代次(l1);
    auto converged = faultBound.创建绑定存在(unknownQ);
    要求(converged.成功(unknownQ) && converged.状态 == S::精确重复 &&
             converged.首次发布H == unknownQ.G0 + 1 && 当前代次(l1) == unknownG,
         "confirmed-read-failure-original-request-converges");
    using CS = 场景直接包含状态;
    for (const auto &[sourceState, expected] :
         {std::pair{CS::资源失败, S::资源失败},
          std::pair{CS::事实代次漂移, S::事实代次漂移},
          std::pair{CS::历史材料已清理, S::历史材料已清理},
          std::pair{CS::数量预算不足, S::数量预算不足},
          std::pair{CS::内部不一致, S::内部不一致}}) {
      auto q = request(K::父存在组成, member.事实->新存在);
      faulty.currentFault = sourceState;
      faulty.failAfterG = q.G0;
      faulty.currentFaultCalls = 0;
      auto failed = faultBound.创建绑定存在(q);
      要求(faulty.currentFaultCalls == 1 && failed.状态 == expected &&
               !failed.首次发布H && !failed.事实 && failed.原请求 &&
               *failed.原请求 == q && 当前代次(l1) == q.G0 + 1,
           "published-current-joint-read-exact-failure");
      faulty.currentFault.reset();
      auto replayed = faultBound.创建绑定存在(q);
      要求(replayed.成功(q) && replayed.状态 == S::精确重复 &&
               replayed.首次发布H == q.G0 + 1 && 当前代次(l1) == q.G0 + 1,
           "published-current-joint-failure-replay-converges");
    }
    要求(std::holds_alternative<std::monostate>(features.初始化特征定义结构()),
         "feature-definition-init");
    要求(std::holds_alternative<std::monostate>(features.初始化准确特征结构()),
         "feature-information-init");
    // 真实概念 owner 与自有结构登记；所有原生值来源均为所属节点。
    auto co = 建立所有者(issuer, 106);
    std::vector<std::pair<节点种类, std::optional<L1所有者范围值表示种类>>> cs(
        28, {节点种类::普通, std::nullopt});
    for (std::size_t i = 16; i < 27; ++i)
      cs[i] = {节点种类::属性类型, i == 21 || i == 24
                                       ? L1所有者范围值表示种类::U64组
                                   : i == 23 ? L1所有者范围值表示种类::I64组
                                             : L1所有者范围值表示种类::I64};
    auto ct = 建立元节点(*co.写入端口, l1, 10001, cs);
    auto cm = 建立元节点(
        *co.写入端口, l1, 10002,
        std::vector<std::pair<节点种类, std::optional<L1所有者范围值表示种类>>>(
            8, {节点种类::普通, std::nullopt}));
    std::vector<std::pair<节点种类, std::optional<L1所有者范围值表示种类>>> xs(
        24, {节点种类::普通, std::nullopt});
    for (auto i : {4, 6, 17, 22})
      xs[i] = {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (auto i : {5, 9, 10, 23})
      xs[i] = {节点种类::属性类型, L1所有者范围值表示种类::I64};
    auto xt = 建立元节点(*co.写入端口, l1, 10003, xs);
    L1所有者范围写集请求 cw{L1所有者范围CRUD合同版本, 当前代次(l1), {10004}};
    std::uint32_t ck = 1;
    auto edge = [&](稳定编码 a, 稳定编码 b, 稳定编码 t, std::int64_t role) {
      cw.关系.push_back({{ck++}, a, b, t, role});
    };
    auto attr = [&](稳定编码 n, 稳定编码 t, std::int64_t v) {
      L1所有者范围写集本地键 k{ck++};
      cw.值.push_back({k, n, t, v, n});
      cw.属性槽变更.push_back({n, t, k});
    };
    for (std::size_t i = 0; i < ct.size(); ++i)
      edge(cm[0], ct[i], ct[27], i + 1);
    edge(cm[0], cm[1], ct[1], 1);
    attr(cm[0], ct[16], 1);
    attr(cm[1], ct[16], 1);
    attr(cm[1], ct[17], 1);
    for (std::size_t i = 0; i < 4; ++i) {
      edge(cm[0], cm[i + 2], ct[0], i + 1);
      attr(cm[i + 2], ct[18], i + 1);
    }
    attr(cm[6], ct[16], 1);
    for (std::size_t i = 0; i < xt.size(); ++i)
      edge(cm[6], xt[i], ct[27], i + 1);
    auto cws = co.写入端口->提交所有者范围中性写集(cw);
    要求(cws.状态 == L1所有者范围写入状态::成功, "concept-layout-real-write");
    概念树结构交付 cl{cm[0],
                      {概念树概念身份{cm[2]}, 概念树概念身份{cm[3]},
                       概念树概念身份{cm[4]}, 概念树概念身份{cm[5]}},
                      概念树规则身份{cm[1]},
                      {ct[0],  ct[1],  ct[2],  ct[3],  ct[4],  ct[5],  ct[6],
                       ct[7],  ct[8],  ct[9],  ct[10], ct[11], ct[12], ct[13],
                       ct[14], ct[15], ct[16], ct[17], ct[18], ct[19], ct[20],
                       ct[21], ct[22], ct[23], ct[24], ct[25], ct[26], ct[27]}};
    特征概念结构交付 cx;
    cx.锚点 = cm[6];
    for (std::size_t i = 0; i < xt.size(); ++i)
      cx.类型[i] = xt[i];
    概念树类数据服务 concepts(l1, features, existence, values,
                              std::move(*co.写入端口), cl, cx,
                              概念树存在扩展交付{cm[7]});
    const auto conceptG = 当前代次(l1);
    要求(concepts.共享应用已启用(), "concept-shared-provider-ready");

    特征概念预算 fb{{10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000},
                    10000,
                    10000,
                    10000,
                    10000,
                    10000};
    概念树共享预算 cb{fb.基础, fb, 10000, 10000, 10000, 10000};
    std::vector<概念树概念身份> templates;
    for (std::uint32_t i = 0; i < 3; ++i) {
      auto ftResult =
          features.创建先天I64特征类型({fmeta[0],
                                        fmeta[1],
                                        i + 1,
                                        1,
                                        {{0, 100}},
                                        I64特征域形成参数{2, fmeta[0]}});
      auto *ft = std::get_if<特征类型身份>(&ftResult);
      要求(ft, "concept-feature-type");
      特征概念建立请求 fq{{1, 当前代次(l1), 15000 + i},
                          {*ft, {{{0, 100}}}},
                          *ft,
                          {cl.根组.特征根},
                          {},
                          fb};
      auto fc = concepts.建立特征概念(fq);
      要求(fc.成功() && fc.概念, "concept-feature-TOP");
      templates.push_back(fc.概念->身份);
    }
    auto cq = [&](std::vector<概念树概念身份> ts,
                  std::vector<概念树概念身份> parents, std::uint64_t key) {
      const auto g = 当前代次(l1);
      return 概念树存在建立请求_v2{
          2, {1, g, key}, {ts}, parents, {概念树存在引用{C}, g}, cb};
    };
    auto empty = cq({templates[0]}, {}, 16000);
    const auto emptyG = 当前代次(l1);
    auto emptyR = concepts.创建存在概念(empty);
    要求(!emptyR.成功() && emptyR.状态 == 概念树数据状态::入口拒绝 &&
             当前代次(l1) == emptyG,
         "concept-v2-empty-parent-zero-write");
    概念树概念建立请求 legacyEmpty{{1, emptyG, 16001},
                                   概念树存在定义{{templates[0]}},
                                   {},
                                   {{概念树存在引用{C}, emptyG}},
                                   cb.基础};
    auto legacyEmptyR = concepts.创建概念(legacyEmpty);
    要求(!legacyEmptyR.成功() &&
             legacyEmptyR.状态 == 概念树数据状态::入口拒绝 &&
             当前代次(l1) == emptyG,
         "concept-legacy-empty-parent-zero-write");
    auto parentQ = cq({templates[0]}, {cl.根组.存在根}, 16002);
    auto parentR = concepts.创建存在概念(parentQ);
    if (!parentR.成功())
      std::cerr << "concept-parent-status=" << int(parentR.状态) << "\n";
    要求(parentR.成功() && parentR.概念, "concept-v2-valid-parent");
    const auto firstConceptG = 当前代次(l1);
    auto parentReplay = concepts.创建存在概念(parentQ);
    要求(parentReplay.成功() && parentReplay.状态 == 概念树数据状态::精确重复 &&
             当前代次(l1) == firstConceptG,
         "concept-v2-exact-replay");
    auto multiQ =
        cq({templates[1]}, {cl.根组.存在根, parentR.概念->概念}, 16003);
    auto multiR = concepts.创建存在概念(multiQ);
    if (!multiR.成功() || multiR.概念->直接上位组.size() != 2)
      std::cerr << "multi-status=" << int(multiR.状态)
                << " edges=" << multiR.概念->直接上位组.size() << "\n";
    要求(multiR.成功() && multiR.概念->直接上位组.size() == 2,
         "concept-v2-multiple-same-root-parents");
    auto wrongQ = cq({templates[2]}, {cl.根组.特征根}, 16004);
    const auto wrongG = 当前代次(l1);
    auto wrongR = concepts.创建存在概念(wrongQ);
    要求(!wrongR.成功() && wrongR.状态 == 概念树数据状态::引用冲突 &&
             当前代次(l1) == wrongG,
         "concept-v2-wrong-root-zero-write");
    概念树生命周期迁移请求 retire{
        {1, 当前代次(l1), 16005},    multiR.概念->概念,
        multiR.概念->生命周期值事实, 概念树生命周期状态::活跃,
        概念树生命周期状态::退役,    cb.基础};
    auto retired = concepts.治理存在概念({2, retire, cb});
    要求(retired.成功(), "concept-parent-retired");
    概念树概念退出请求 exitConcept{{1, 当前代次(l1), 16006},
                                   retired.概念->概念,
                                   retired.概念->生命周期值事实,
                                   cb.基础};
    auto ended = concepts.治理存在概念({2, exitConcept, cb});
    要求(ended.成功(), "concept-parent-exited");
    auto exitedParent = cq({templates[2]}, {retired.概念->概念}, 16007);
    const auto exitedParentG = 当前代次(l1);
    auto exitedParentR = concepts.创建存在概念(exitedParent);
    // 既有应用定义先读属性；退出节点的种类属性缺失映射为内部不一致。
    要求(!exitedParentR.成功() &&
             exitedParentR.状态 == 概念树数据状态::内部不一致 &&
             当前代次(l1) == exitedParentG,
         "concept-exited-parent-zero-write");
    const auto finalG = 当前代次(l1);
    std::cout << "BOUND_EXISTENCE_OK " << 通过数 << " G=" << finalG << "\n";
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "ERROR " << e.what() << "\n";
    return 1;
  }
}
