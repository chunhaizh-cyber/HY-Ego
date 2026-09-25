#include "../海中鱼巣/业务/初始化.本能双根二次关系概念.h"
#include "../海中鱼巣/业务/初始化.二次关系概念结构.h"
#include "../海中鱼巣/装配.普通应用.h"
#include "../海中鱼巣/领域/数据服务.场景类.h"
#include "../海中鱼巣/领域/数据服务.概念树类.h"
#include "../海中鱼巣/领域/数据服务.存在类.h"
#include "../海中鱼巣/领域/数据服务.特征值类.h"
#include "../海中鱼巣/领域/数据服务.特征类.h"
#include "../海中鱼巣/领域/数据服务.状态类.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace {
using namespace 海中鱼巣;

int 通过数 = 0;

// 仅供仍保留的 I64 特征概念组织 v2 读取回归夹具使用。
constexpr 概念树预算 测试纯概念预算{
    1024, 8192, 1024, 1024, 1024, 1024, 1024, 1024};

void 要求(bool 成立, std::string_view 名称) {
  if (!成立) {
    std::cerr << "FAIL " << 名称 << '\n';
    throw std::runtime_error("instinct-root RC initialization probe failed");
  }
  ++通过数;
  std::cout << "PASS " << 名称 << '\n';
}

[[noreturn]] void 夹具失败(const char* 文本) { throw std::runtime_error(文本); }

std::uint64_t 当前代次(const L1事实基座服务& l1) {
  const auto r = l1.读取中性当前事实代次({L1中性CRUD合同版本});
  if (r.状态 != L1中性读取状态::成功 || !r.事实代次)
    夹具失败("fixture generation read failed");
  return r.事实代次;
}

L1所有者范围交付 建立所有者(L1所有者范围签发器& 签发器,
                              const L1事实基座服务& l1,
                              std::uint64_t key) {
  auto r = 签发器.建立所有者范围(
      {L1所有者范围CRUD合同版本, {key}, L1所有者范围种类::独占结构范围});
  if ((r.建立结果.状态 != L1所有者范围管理状态::成功 &&
       r.建立结果.状态 != L1所有者范围管理状态::精确重复) ||
      !r.写入端口 || !r.写入端口->有效() || !r.写入端口->绑定于(l1))
    夹具失败("fixture owner creation failed");
  return r;
}

using 元节点规格 =
    std::pair<节点种类, std::optional<L1所有者范围值表示种类>>;

std::vector<稳定编码> 建立元节点(L1所有者范围写端口& 端口,
                                 const L1事实基座服务& l1,
                                 std::uint64_t key,
                                 const std::vector<元节点规格>& 规格) {
  L1所有者范围写集请求 请求{
      L1所有者范围CRUD合同版本, 当前代次(l1), {key}};
  for (std::uint32_t i = 0; i < 规格.size(); ++i)
    请求.节点.push_back({{static_cast<std::uint64_t>(i) + 1},
                         规格[i].first, 规格[i].second});
  const auto r = 端口.提交所有者范围中性写集(请求);
  if (r.状态 != L1所有者范围写入状态::成功 ||
      r.新编码映射.size() != 规格.size())
    夹具失败("fixture metadata creation failed");
  std::vector<稳定编码> ids(规格.size());
  for (const auto& [local, id] : r.新编码映射) {
    if (!local.值 || local.值 > ids.size() || !有效(id))
      夹具失败("fixture metadata mapping failed");
    ids[local.值 - 1] = id;
  }
  return ids;
}

enum class 损坏纯概念候选模式 : std::uint8_t {
  无,
  缺少RC见证,
  RC见证错角色,
  RC见证错目标
};

struct 夹具 final {
  std::unique_ptr<L1事实基座运行包> 运行包;
  L1所有者范围交付 特征定义所有者, 特征信息所有者, 存在所有者,
      场景所有者, 状态所有者, 概念所有者, 外部所有者;
  std::unique_ptr<特征值类数据服务> 特征值;
  std::unique_ptr<特征类数据服务> 特征;
  std::unique_ptr<存在类数据服务> 存在;
  std::unique_ptr<状态类数据服务> 状态;
  std::unique_ptr<场景类数据服务> 场景;
  std::unique_ptr<概念树类数据服务> 概念;

  explicit 夹具(损坏纯概念候选模式 损坏模式 =
                    损坏纯概念候选模式::无) {
    运行包 = std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto& l1 = 运行包->读取服务();
    auto& issuer = 运行包->所有者范围签发器();
    特征定义所有者 = 建立所有者(issuer, l1, 0x71001);
    特征信息所有者 = 建立所有者(issuer, l1, 0x71002);
    存在所有者 = 建立所有者(issuer, l1, 0x71003);
    场景所有者 = 建立所有者(issuer, l1, 0x71004);
    状态所有者 = 建立所有者(issuer, l1, 0x71005);
    概念所有者 = 建立所有者(issuer, l1, 0x71006);
    外部所有者 = 建立所有者(issuer, l1, 0x71007);

    const auto producers = 建立元节点(
        *特征定义所有者.写入端口, l1, 0x71101,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    std::vector<元节点规格> definitions(16,
                                          {节点种类::普通, std::nullopt});
    for (auto i : {2, 9, 13})
      definitions[i] =
          {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (auto i : {3, 8})
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
        *存在所有者.写入端口, l1, 0x71121,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    (void)建立元节点(*存在所有者.写入端口, l1, 0x455849535446414DULL,
                     {{节点种类::普通, std::nullopt},
                      {节点种类::普通, std::nullopt}});
    const auto stateLayout = 建立元节点(
        *状态所有者.写入端口, l1, 0x71131,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::U64组},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64}});

    特征值 = std::make_unique<特征值类数据服务>(l1);
    特征 = std::make_unique<特征类数据服务>(
        l1, std::move(*特征定义所有者.写入端口),
        std::move(*特征信息所有者.写入端口), *特征值, producers.front());
    if (!std::holds_alternative<std::monostate>(特征->初始化特征定义结构()) ||
        !std::holds_alternative<std::monostate>(特征->初始化准确特征结构()))
      夹具失败("fixture feature layout failed");

    const 存在单例角色结构登记请求 roleRequest{1, 当前代次(l1), {0x1202}};
    const auto role = 存在类数据服务::登记单例角色结构(
        l1, *存在所有者.写入端口, roleRequest);
    if (!role.成功(roleRequest) || !role.交付)
      夹具失败("fixture role layout failed");
    const 实例特征结构登记请求 ifRequest{
        2, 当前代次(l1), {0x4946525354525632ULL}};
    const auto ifLayout = 存在类数据服务::登记实例特征结构(
        l1, *存在所有者.写入端口, ifRequest);
    if (!ifLayout.成功(ifRequest) || !ifLayout.交付)
      夹具失败("fixture instance-feature layout failed");
    存在 = std::make_unique<存在类数据服务>(
        l1, *特征, std::move(*存在所有者.写入端口), existenceLayout[0],
        existenceLayout[1], 存在当前采用结构交付{existenceLayout[2]},
        *ifLayout.交付, *role.交付);
    状态 = std::make_unique<状态类数据服务>(
        l1, *特征, std::move(*状态所有者.写入端口),
        状态类结构交付{stateLayout[0], stateLayout[1], stateLayout[2],
                       stateLayout[3], stateLayout[4], stateLayout[5],
                       stateLayout[6], stateLayout[7]});

    const 场景结构登记请求 baseRequest{2, 当前代次(l1), {1}};
    const auto base = 场景类数据服务::登记结构类型(
        l1, *场景所有者.写入端口, baseRequest);
    const 场景特征组织扩展登记请求 featureRequest{1, 当前代次(l1)};
    const auto sceneFeature = 场景类数据服务::登记特征组织扩展(
        l1, *场景所有者.写入端口, featureRequest);
    const 场景直接包含扩展登记请求 includeRequest{1, 当前代次(l1)};
    const auto include = 场景类数据服务::登记直接包含扩展(
        l1, *场景所有者.写入端口, includeRequest);
    if (!base.成功(baseRequest) || !base.交付 ||
        !sceneFeature.成功(featureRequest) || !sceneFeature.交付 ||
        !include.成功(includeRequest) || !include.交付)
      夹具失败("fixture scene layout failed");
    场景 = std::make_unique<场景类数据服务>(
        l1, std::move(*场景所有者.写入端口), *base.交付, *存在, *状态,
        *sceneFeature.交付, *include.交付);

    const 纯概念结构登记请求_v2 pureRequest{
        2, 当前代次(l1), {0x71151}, 1048576};
    const auto pure = 概念树类数据服务::登记纯概念结构_v2(
        l1, *概念所有者.写入端口, pureRequest);
    if (!pure.成功(pureRequest) || !pure.交付)
      夹具失败("fixture pure layout failed");
    const 特征概念出生使用结构登记请求 birthRequest{
        1, 当前代次(l1), {0x71152}, *pure.交付, 4};
    const auto birth = 概念树类数据服务::登记特征概念出生使用结构(
        l1, *概念所有者.写入端口, birthRequest);
    if (!birth.成功(birthRequest) || !birth.交付)
      夹具失败("fixture feature concept layout failed");
    const 存在概念两组结构登记请求_v1 twoRequest{
        1, 当前代次(l1), {0x71153}, *pure.交付, 18};
    const auto two = 概念树类数据服务::登记存在概念两组结构_v1(
        l1, *概念所有者.写入端口, twoRequest);
    if (!two.成功(twoRequest) || !two.交付)
      夹具失败("fixture existence concept layout failed");
    const 二次关系初始化请求 rcRequest{
        1, 当前代次(l1), {0x71154}, *pure.交付, 53};
    const auto rc = 初始化二次关系结构(
        l1, *概念所有者.写入端口, rcRequest);
    if (!rc.成功() || !rc.交付)
      夹具失败("fixture RC layout failed");
    if (损坏模式 != 损坏纯概念候选模式::无) {
      L1所有者范围写集请求 damaged{
          L1所有者范围CRUD合同版本, 当前代次(l1), {0x71155}};
      damaged.节点 = {{{1}, 节点种类::普通, std::nullopt},
                        {{2}, 节点种类::普通, std::nullopt}};
      damaged.关系.push_back(
          {{3}, pure.交付->概念族锚点, L1所有者范围写集本地键{1},
           pure.交付->类型.概念族成员, 1});
      damaged.关系.push_back(
          {{4}, L1所有者范围写集本地键{1},
           L1所有者范围写集本地键{2}, pure.交付->类型.定义成员, 1});
      if (损坏模式 != 损坏纯概念候选模式::缺少RC见证) {
        const auto target =
            损坏模式 == 损坏纯概念候选模式::RC见证错目标
                ? rc.交付->锚点
                : rc.交付->规范化规则.值;
        const auto role =
            损坏模式 == 损坏纯概念候选模式::RC见证错角色 ? 2 : 1;
        damaged.关系.push_back(
            {{5}, L1所有者范围写集本地键{1}, target,
             rc.交付->类型.规范化规则归属, role});
      }
      const auto saved =
          概念所有者.写入端口->提交所有者范围中性写集(damaged);
      if (saved.状态 != L1所有者范围写入状态::成功)
        夹具失败("damaged pure candidate fixture failed");
    }
    概念 = std::make_unique<概念树类数据服务>(
        l1, *特征, *存在, *特征值, *场景,
        std::move(*概念所有者.写入端口), *pure.交付, *birth.交付,
        *two.交付, *rc.交付, *场景);
  }

  L1事实基座服务& l1() { return 运行包->读取服务(); }

  本能先天特征概念初始化结果 读取先天() {
    本能先天特征概念初始化提供者 provider(*特征, *概念);
    return provider.初始化(
        {1, 当前代次(l1()), {64, 256, 0, 0, 0, 64, 0, 0}});
  }

  本能双根二次关系概念初始化结果 初始化() {
    const auto innate = 读取先天();
    if (!innate.成功()) 夹具失败("fixture innate initialization failed");
    本能双根二次关系概念初始化提供者 provider(*特征, *概念);
    return provider.初始化(
        {本能双根二次关系概念初始化合同版本, 当前代次(l1()), innate});
  }
};

void 验证损坏纯概念候选不被跳过() {
  const auto verify = [](损坏纯概念候选模式 mode,
                         std::string_view name) {
    夹具 f(mode);
    const auto g = 当前代次(f.l1());
    const 纯概念定义 universal =
        通用存在概念定义{1, 通用存在定义规则::不预设特征};
    const 纯概念查询请求_v3 request{3, g, g, universal};
    const auto result = f.概念->精确查询纯概念_v3(request);
    要求(result.状态 == 纯概念状态::内部不一致 && !result.事实,
         std::string(name) + "-exact-query");
    const auto concepts = f.概念->读取当前I64特征概念(
        {2, g, g, 特征类型身份{{1}}, 测试纯概念预算});
    要求(concepts.状态 == 纯概念状态::内部不一致 &&
             concepts.概念组.empty(),
         std::string(name) + "-current-i64-query");
  };
  verify(损坏纯概念候选模式::缺少RC见证,
         "family-member-without-pure-kind-or-rc-witness-is-rejected");
  verify(损坏纯概念候选模式::RC见证错角色,
         "family-member-with-wrong-rc-witness-role-is-rejected");
  verify(损坏纯概念候选模式::RC见证错目标,
         "family-member-with-wrong-rc-witness-target-is-rejected");
}

特征I64比较绑定定义 目标判断定义(特征类型身份 ft) {
  const auto max = std::numeric_limits<std::int64_t>::max();
  const 特征类标量量化合同 q{
      ft.编码, ft.编码, ft.编码, 1, 1, 0, max,
      特征类标量量纲::无量纲, 特征类标量舍入::不转换,
      特征类标量溢出::拒绝};
  return {ft,
          特征I64比较用途::目标判断,
          特征比较算法族::标量有序比较与安全差异,
          1,
          特征I64输入角色::当前事实,
          特征I64输入角色::目标值,
          0,
          q,
          1,
          std::nullopt,
          std::nullopt,
          std::nullopt,
          {{特征类标量输出提交项{特征类标量结果角色::差异, q}, ft}}};
}

二次关系定义 正差距定义(特征I64比较绑定身份 k, 概念树概念身份 fc,
                          概念树概念身份 ec, std::uint8_t mask = 4) {
  二次关系原子定义 atom;
  atom.D = {mask};
  atom.约束.FC.push_back({二次关系约束角色::共同, fc});
  atom.约束.EC.push_back({二次关系约束角色::共同, ec});
  atom.K = k;
  atom.输出 = 特征类标量结果角色::差异;
  return atom;
}

纯概念事实 预建通用EC(夹具& f) {
  const auto g = 当前代次(f.l1());
  const 纯概念定义 definition =
      通用存在概念定义{1, 通用存在定义规则::不预设特征};
  const 纯概念创建请求_v3 request{
      3, g, {0x3001}, definition, 概念初始组织指定::显式顶层, {}};
  const auto result = f.概念->创建或复用纯概念_v3(request);
  if (!result.成功(request) || !result.事实)
    夹具失败("prebuilt universal EC failed");
  return *result.事实;
}

特征I64比较绑定事实 预建K(夹具& f, 特征类型身份 ft,
                             std::uint64_t key,
                             std::optional<特征I64比较绑定定义> definition =
                                 std::nullopt) {
  const 特征I64比较绑定建立请求 request{
      1, 当前代次(f.l1()), {key}, definition.value_or(目标判断定义(ft))};
  const auto result = f.特征->建立I64比较绑定(request);
  if (!result.成功() || !result.事实)
    夹具失败("prebuilt K failed");
  return *result.事实;
}

二次关系概念事实 预建RC(夹具& f, const 二次关系定义& definition,
                         std::uint64_t key) {
  const auto g = 当前代次(f.l1());
  const 二次关系概念建立请求 request{
      2, {1, g, {key}}, definition, 概念初始组织指定::显式顶层,
      {}, {}};
  const auto result = f.概念->建立二次关系概念(request);
  if (!result.成功() || !result.事实) 夹具失败("prebuilt RC failed");
  return *result.事实;
}

void 验证部分材料收敛() {
  {
    夹具 f;
    const auto ec = 预建通用EC(f);
    const auto result = f.初始化();
    要求(result.成功() &&
             result.状态 == 本能双根二次关系概念初始化状态::已形成 &&
             result.安全根->通用存在概念.概念 == ec.概念,
         "prebuilt-ec-converges");
  }
  {
    夹具 f;
    const auto innate = f.读取先天();
    if (!innate.成功()) 夹具失败("partial K innate failed");
    const auto k = 预建K(f, innate.安全值交付->类型.身份,
                         0x49524B5341460001ULL);
    本能双根二次关系概念初始化提供者 provider(*f.特征, *f.概念);
    const auto result = provider.初始化(
        {2, 当前代次(f.l1()), f.读取先天()});
    要求(result.成功() && result.安全根->目标判断K.身份 == k.身份,
         "prebuilt-single-k-converges");
  }
  {
    夹具 f;
    const auto innate = f.读取先天();
    if (!innate.成功()) 夹具失败("partial RC innate failed");
    const auto ec = 预建通用EC(f);
    const auto k = 预建K(f, innate.安全值交付->类型.身份,
                         0x49524B5341460001ULL);
    const auto rc = 预建RC(
        f, 正差距定义(k.身份, innate.安全值交付->完整域概念.概念,
                      ec.概念),
        0x4952524353414601ULL);
    本能双根二次关系概念初始化提供者 provider(*f.特征, *f.概念);
    const auto freshInnate = f.读取先天();
    const auto result = provider.初始化(
        {2, 当前代次(f.l1()), freshInnate});
    要求(result.成功() && result.安全根->正差距RC.身份 == rc.身份,
         "prebuilt-single-rc-converges");
  }
}

void 验证结构化失败() {
  {
    夹具 f;
    const auto innate = f.读取先天();
    const auto ec = 预建通用EC(f);
    const auto k = 预建K(f, innate.安全值交付->类型.身份,
                         0x49524B5341460001ULL);
    (void)预建RC(f,
                 正差距定义(k.身份,
                             innate.安全值交付->完整域概念.概念,
                             ec.概念, 3),
                 0x4952524353414601ULL);
    本能双根二次关系概念初始化提供者 provider(*f.特征, *f.概念);
    const auto freshInnate = f.读取先天();
    const auto result = provider.初始化(
        {2, 当前代次(f.l1()), freshInnate});
    要求(result.状态 == 本能双根二次关系概念初始化状态::幂等冲突 &&
             !result.安全根 && !result.服务根,
         "wrong-rc-on-fixed-key-is-idempotency-conflict");
  }
  {
    夹具 f;
    const auto first = f.初始化();
    if (!first.成功()) 夹具失败("retirement fixture initialization failed");
    const auto retire = [&](概念树生命周期状态 from,
                            概念树生命周期状态 to,
                            std::uint64_t key) {
      const auto g = 当前代次(f.l1());
      return f.概念->迁移二次关系生命周期(
          {2, {1, g, {key}}, first.安全根->正差距RC.身份, from, to,
           first.安全根->目标判断K.身份.编码});
    };
    const auto cooling = retire(概念树生命周期状态::活跃,
                                概念树生命周期状态::冷却, 0x71E01);
    要求(cooling.成功(), "rc-retirement-fixture-cooling");
    const auto cooledG = 当前代次(f.l1());
    const auto cooledLookup = f.概念->查找二次关系完整定义(
        {2, {1, cooledG, cooledG}, first.安全根->正差距RC.定义});
    const auto cooledRead = f.概念->读取二次关系概念(
        {2, {1, cooledG, cooledG}, first.安全根->正差距RC.身份});
    要求(cooledLookup.成功() && cooledLookup.状态 ==
               二次关系数据状态::冷却命中 &&
               cooledRead.成功() &&
               cooledRead.状态 == 二次关系数据状态::冷却命中,
           "cooled-rc-owner-reads-remain-successful");
    const auto cooled = f.初始化();
    要求(cooled.成功() && cooled.安全根 &&
               cooled.安全根->正差距RC.身份 ==
                   first.安全根->正差距RC.身份,
           "cooled-rc-remains-readable-and-reused");
    const auto retired = retire(概念树生命周期状态::冷却,
                                概念树生命周期状态::退役, 0x71E02);
    要求(retired.成功(), "rc-retirement-fixture-retired");
    const auto result = f.初始化();
    要求(result.状态 ==
                 本能双根二次关系概念初始化状态::类型或概念已退出 &&
             !result.安全根 && !result.服务根,
         "retired-rc-is-structured-failure");
  }
}

void 验证首次重复定义与完整读取() {
  夹具 f;
  const auto first = f.初始化();
  要求(first.成功() &&
           first.状态 == 本能双根二次关系概念初始化状态::已形成,
       "first-call-forms-ec-two-k-two-rc");
  const auto afterFirst = 当前代次(f.l1());
  要求(first.安全根->Gread == first.Gread &&
           first.服务根->Gread == first.Gread && first.Gread == afterFirst,
       "first-call-common-cutoff");
  要求(first.安全根->通用存在概念.概念 ==
             first.服务根->通用存在概念.概念 &&
           first.安全根->先天.类型.身份 != first.服务根->先天.类型.身份 &&
           first.安全根->先天.完整域概念.概念 !=
               first.服务根->先天.完整域概念.概念 &&
           first.安全根->目标判断K.身份 !=
               first.服务根->目标判断K.身份 &&
           first.安全根->正差距RC.身份 != first.服务根->正差距RC.身份,
       "two-root-identity-isolation-and-shared-ec");

  for (const auto* root : {&*first.安全根, &*first.服务根}) {
    const auto& kd = root->目标判断K.定义;
    要求(kd == 目标判断定义(root->先天.类型.身份) &&
             kd.用途 == 特征I64比较用途::目标判断 &&
             kd.左角色 == 特征I64输入角色::当前事实 &&
             kd.右角色 == 特征I64输入角色::目标值 &&
             kd.输出组.size() == 1 &&
             kd.输出组.front().输出.角色 == 特征类标量结果角色::差异 &&
             kd.输出组.front().输出FT == root->先天.类型.身份,
         "k-definition-purpose-roles-and-output");
    const auto* atom = std::get_if<二次关系原子定义>(&root->正差距RC.定义);
    要求(atom && atom->D.掩码 == 4 && atom->K == root->目标判断K.身份 &&
             atom->输出 == 特征类标量结果角色::差异 &&
             atom->约束.FC ==
                 std::vector<二次关系概念约束>{{二次关系约束角色::共同,
                                                 root->先天.完整域概念.概念}} &&
             atom->约束.EC ==
                 std::vector<二次关系概念约束>{{二次关系约束角色::共同,
                                                 root->通用存在概念.概念}},
         "rc-d-pos-k-fc-ec-definition");
  }

  const std::array<std::pair<const 本能根二次关系概念交付*, std::uint64_t>, 2>
      roots{{{&*first.安全根, 0x49524B5341460001ULL},
             {&*first.服务根, 0x49524B5352560001ULL}}};
  for (const auto& [root, key] : roots) {
    const auto replay = f.特征->建立I64比较绑定(
        {1, root->目标判断K.创建G - 1, {key}, root->目标判断K.定义});
    要求(replay.成功() &&
             replay.状态 == 特征I64比较绑定状态::精确重复,
         "k-write-replay-is-exact");
    const auto current = f.特征->读取当前I64比较绑定_v2(
        {2, first.Gread, root->先天.类型.身份,
         特征I64比较用途::目标判断});
    const auto byId = f.特征->读取I64比较绑定_v2(
        {2, first.Gread, first.Gread, root->目标判断K.身份});
    要求(current.成功() && byId.成功() && current.Gread==first.Gread &&
             current.H==first.Gread && byId.Gread==first.Gread &&
             byId.H==first.Gread &&
             current.事实->身份 == root->目标判断K.身份 &&
             byId.事实->身份 == root->目标判断K.身份,
         "k-current-and-identity-complete-read-v2");
  }

  const auto beforeRepeat = 当前代次(f.l1());
  const 纯概念定义 universal =
      通用存在概念定义{1, 通用存在定义规则::不预设特征};
  const 纯概念查询请求_v3 universalRequest{
      3, beforeRepeat, beforeRepeat, universal};
  const auto universalRead = f.概念->精确查询纯概念_v3(universalRequest);
  要求(universalRead.成功(universalRequest) && universalRead.事实 &&
             universalRead.事实->概念 ==
                 first.安全根->通用存在概念.概念 &&
             当前代次(f.l1()) == beforeRepeat,
         "pure-query-skips-valid-rc-family-members-and-reuses-ec");
  const I64特征概念组织读取请求 safeConceptRequest{
      2, beforeRepeat, beforeRepeat, first.安全根->先天.类型.身份,
      测试纯概念预算};
  const auto safeConcepts =
      f.概念->读取当前I64特征概念(safeConceptRequest);
  要求(safeConcepts.成功(safeConceptRequest) &&
           std::any_of(safeConcepts.概念组.begin(),
                       safeConcepts.概念组.end(), [&](const auto& item) {
                         return item.概念 ==
                             first.安全根->先天.完整域概念.概念;
                       }),
       "current-i64-query-skips-valid-rc-family-members");
  const auto repeated = f.初始化();
  if (!repeated.成功() || 当前代次(f.l1()) != afterFirst)
    std::cerr << "repeat-state=" << static_cast<unsigned>(repeated.状态)
              << " before=" << afterFirst
              << " after=" << 当前代次(f.l1()) << '\n';
  要求(repeated.成功() &&
           repeated.状态 == 本能双根二次关系概念初始化状态::已恢复 &&
           当前代次(f.l1()) == afterFirst,
       "same-process-repeat-is-zero-write-recovery");
  要求(repeated.安全根->目标判断K.身份 ==
             first.安全根->目标判断K.身份 &&
           repeated.服务根->目标判断K.身份 ==
             first.服务根->目标判断K.身份 &&
           repeated.安全根->正差距RC.身份 == first.安全根->正差距RC.身份 &&
           repeated.服务根->正差距RC.身份 == first.服务根->正差距RC.身份,
       "same-process-repeat-identities-stable");

  auto staleInnate = f.读取先天();
  要求(staleInnate.成功(), "drift-fixture-innate-ready");
  L1所有者范围写集请求 driftWrite{
      L1所有者范围CRUD合同版本, 当前代次(f.l1()), {0x71FF1}};
  driftWrite.节点.push_back({{1}, 节点种类::普通, std::nullopt});
  const auto driftPublished =
      f.外部所有者.写入端口->提交所有者范围中性写集(driftWrite);
  要求(driftPublished.状态 == L1所有者范围写入状态::成功,
       "drift-fixture-published");
  本能双根二次关系概念初始化提供者 provider(*f.特征, *f.概念);
  const auto drift = provider.初始化(
      {2, staleInnate.Gread, staleInnate});
  要求(drift.状态 == 本能双根二次关系概念初始化状态::当前性漂移 &&
           !drift.安全根 && !drift.服务根,
       "generation-drift-is-structured-and-clears-delivery");
}

void 验证异义K() {
  夹具 f;
  const auto innate = f.读取先天();
  if (!innate.成功()) 夹具失败("ambiguous K innate failed");
  auto conflicting = 目标判断定义(innate.安全值交付->类型.身份);
  conflicting.上下文要求位 =
      static_cast<std::uint8_t>(特征I64上下文要求::参与者A);
  (void)预建K(f, innate.安全值交付->类型.身份,
              0x49524B5341460001ULL, conflicting);
  const auto result = f.初始化();
  要求(result.状态 == 本能双根二次关系概念初始化状态::引用冲突 &&
             !result.安全根 && !result.服务根,
         "different-current-k-definition-is-reference-conflict");
}

void 写身份(const std::filesystem::path& path,
            const 本能双根二次关系概念初始化结果& r) {
  std::ofstream out(path, std::ios::trunc);
  if (!out.is_open() || !r.成功()) 夹具失败("identity state write failed");
  out << r.安全根->通用存在概念.概念.值.值 << '\n'
      << r.安全根->目标判断K.身份.编码.值 << '\n'
      << r.服务根->目标判断K.身份.编码.值 << '\n'
      << r.安全根->正差距RC.身份.值.值 << '\n'
      << r.服务根->正差距RC.身份.值.值 << '\n';
  out.flush();
  if (!out.good()) 夹具失败("identity state flush failed");
}

std::array<std::uint64_t, 5> 读身份(const std::filesystem::path& path) {
  std::ifstream in(path);
  std::array<std::uint64_t, 5> ids{};
  if (!in.is_open()) 夹具失败("identity state read failed");
  for (auto& id : ids)
    if (!(in >> id) || !id) 夹具失败("identity state malformed");
  return ids;
}

普通应用配置 配置(const std::filesystem::path& root) {
  普通应用配置 c;
  c.L1事实基座持久存储 = {L1事实基座持久恢复合同版本_v1, root};
  c.自我线程邮箱容量 = 8;
  c.自我线程进入停门等待毫秒 = 10000;
  c.自我线程停止回收诊断等待毫秒 = 10000;
  return c;
}

int 普通应用种子(const std::filesystem::path& root,
                 const std::filesystem::path& state) {
  const auto assembly = 构造普通应用上下文(配置(root));
  要求(assembly.成功(), "ordinary-first-assembly-success");
  const auto delivery = 读取普通应用本能双根二次关系概念初始化();
  要求(delivery && delivery->成功() &&
           delivery->状态 == 本能双根二次关系概念初始化状态::已形成,
       "ordinary-first-delivery-is-real-and-formed");
  const auto self = 初始化普通应用自我();
  要求(self.请求回显 && self.成功(*self.请求回显) && self.投影 &&
           self.投影->概念.概念 == delivery->安全根->通用存在概念.概念,
       "self-form-reuses-the-same-universal-ec");
  const auto method = 初始化普通应用方法登记根();
  要求(method.成功(), "evaluator-fixture-method-root-ready");
  const auto anchor = 初始化普通应用本能根运行锚点(method);
  要求(anchor.成功() && anchor.锚点,
       "evaluator-fixture-instinct-anchor-ready");
  if (!self.投影 || !self.投影->位置.直接结构父)
    夹具失败("root-review self projection incomplete");
  自我线程正式上下文投影_v1 reviewContext;
  reviewContext.Gread = anchor.锚点->事实截止代次;
  reviewContext.世界 = {self.投影->世界根.值};
  reviewContext.自我所在场景 = {
      self.投影->位置.直接结构父->父.值};
  reviewContext.自我 = {anchor.锚点->自我.编码.值};
  reviewContext.安全根 = {{anchor.锚点->安全根.需求.值},
      {anchor.锚点->安全根.列表项.值},
      {anchor.锚点->安全根.实际特征.编码.值},
      {anchor.锚点->安全根.目标合同.值}};
  reviewContext.服务根 = {{anchor.锚点->服务根.需求.值},
      {anchor.锚点->服务根.列表项.值},
      {anchor.锚点->服务根.实际特征.编码.值},
      {anchor.锚点->服务根.目标合同.值}};
  const 自我线程根需求复核请求_v2 reviewRequest{
      自我线程根需求复核合同版本_v2, reviewContext,
      自我线程复核触发根::双根, {}, {0xE1001}, {0xE1002}};
  auto* reviewProvider = 读取普通应用自我根需求复核服务();
  要求(reviewProvider && reviewRequest.完整(),
       "ordinary-context-exposes-unique-root-review-provider");
  const auto beforeReviewG = reviewContext.Gread;
  const auto reviewed = reviewProvider->复核双根当前需求(reviewRequest);
  if (!reviewed.成功(reviewRequest))
    std::cerr << "root-review-state=" << static_cast<unsigned>(reviewed.状态)
              << " safe="
              << static_cast<unsigned>(reviewed.安全根技术状态)
              << " service="
              << static_cast<unsigned>(reviewed.服务根技术状态)
              << " g=" << reviewed.Gread << " expected=" << beforeReviewG
              << '\n';
  if (reviewed.安全根 && reviewed.服务根)
    std::cout << "ROOT-REVIEW safe-difference="
              << reviewed.安全根->准确差异
              << " service-difference=" << reviewed.服务根->准确差异 << '\n';
  要求(reviewed.成功(reviewRequest) && reviewed.安全根 && reviewed.服务根 &&
             !reviewed.写业务事实 && reviewed.Gread == beforeReviewG,
         "root-review-both-roots-success-on-one-g0");
  要求(reviewed.安全根->角色 == 本能根角色::安全 &&
             reviewed.服务根->角色 == 本能根角色::服务 &&
             reviewed.安全根->FT != reviewed.服务根->FT &&
             reviewed.安全根->K != reviewed.服务根->K &&
             reviewed.安全根->RC != reviewed.服务根->RC &&
             reviewed.安全根->求值见证.来源组.size() == 2 &&
             reviewed.服务根->求值见证.来源组.size() == 2,
         "root-review-witnesses-are-ordered-and-not-crossed");
  要求(reviewed.安全根->准确差异 >= 0 &&
             reviewed.服务根->准确差异 >= 0 &&
             (reviewed.安全根->业务结果 ==
                  自我线程根复核业务结果_v2::正差距) ==
                 (reviewed.安全根->准确差异 > 0) &&
             (reviewed.服务根->业务结果 ==
                  自我线程根复核业务结果_v2::正差距) ==
                 (reviewed.服务根->准确差异 > 0),
         "root-review-only-positive-gap-or-current-satisfied");

  auto badSafeRequest = reviewRequest;
  badSafeRequest.上下文.安全根.实际特征 = {0x7FFFFFFFFFFF1001ULL};
  要求(badSafeRequest.完整(), "bad-safe-review-request-remains-well-formed");
  const auto badSafe = reviewProvider->复核双根当前需求(badSafeRequest);
  if (badSafe.安全根技术状态 !=
          自我线程单根复核技术状态_v2::内部不一致 ||
      badSafe.服务根技术状态 !=
          自我线程单根复核技术状态_v2::已复核)
    std::cerr << "bad-safe-state=" << static_cast<unsigned>(badSafe.状态)
              << " safe="
              << static_cast<unsigned>(badSafe.安全根技术状态)
              << " service="
              << static_cast<unsigned>(badSafe.服务根技术状态) << '\n';
  要求(badSafe.安全根技术状态 ==
                 自我线程单根复核技术状态_v2::内部不一致 &&
             badSafe.服务根技术状态 ==
                 自我线程单根复核技术状态_v2::已复核 &&
             !badSafe.安全根 && !badSafe.服务根,
         "safe-ordinary-failure-still-reviews-service-and-clears-both-projections");
  auto badServiceRequest = reviewRequest;
  badServiceRequest.上下文.服务根.实际特征 = {0x7FFFFFFFFFFF1002ULL};
  要求(badServiceRequest.完整(),
       "bad-service-review-request-remains-well-formed");
  const auto badService = reviewProvider->复核双根当前需求(badServiceRequest);
  要求(badService.安全根技术状态 ==
                 自我线程单根复核技术状态_v2::已复核 &&
             badService.服务根技术状态 ==
                 自我线程单根复核技术状态_v2::内部不一致 &&
             !badService.安全根 && !badService.服务根,
         "service-ordinary-failure-preserves-safe-status-and-clears-both-projections");
  auto singleTriggerRequest = reviewRequest;
  singleTriggerRequest.触发根 = 自我线程复核触发根::安全根;
  singleTriggerRequest.正式需求定位 = singleTriggerRequest.上下文.安全根.需求;
  const auto singleTriggered =
      reviewProvider->复核双根当前需求(singleTriggerRequest);
  要求(singleTriggered.成功(singleTriggerRequest) &&
             singleTriggered.安全根技术状态 ==
                 自我线程单根复核技术状态_v2::已复核 &&
             singleTriggered.服务根技术状态 ==
                 自我线程单根复核技术状态_v2::已复核,
         "single-root-trigger-still-reviews-both-roots");

  auto* evaluator = 读取普通应用二次关系求值服务();
  要求(evaluator != nullptr, "ordinary-context-exposes-unique-evaluator");
  const 二次关系参与者材料 participant{
      anchor.锚点->自我.编码,
      {二次关系准确F来源{anchor.锚点->安全根.实际特征}}};
  const 二次关系求值请求 relationRequest{
      2, anchor.锚点->事实截止代次, anchor.锚点->事实截止代次,
      0xE001, delivery->安全根->正差距RC.身份, participant, participant};
  const auto relation = evaluator->求值二次关系(relationRequest);
  要求(relation.状态 == 二次关系判断状态::不命中 &&
             relation.Gread == relationRequest.Gread &&
             relation.H == relationRequest.H &&
             relation.定义H == relationRequest.Gread &&
             relation.请求身份 == relationRequest.请求身份 &&
             relation.条件组.size() == 1 && relation.来源组.size() == 2 &&
             relation.条件组.front().计算.成功(),
         "evaluator-real-f-zero-difference-misses-d-pos");
  auto zeroDefinition = delivery->安全根->正差距RC.定义;
  auto* zeroAtom = std::get_if<二次关系原子定义>(&zeroDefinition);
  if (!zeroAtom) 夹具失败("evaluator zero candidate definition malformed");
  zeroAtom->D.掩码 = 2;
  const 二次关系候选求值请求 candidateRequest{
      2, anchor.锚点->事实截止代次, anchor.锚点->事实截止代次,
      0xE002, zeroDefinition, participant, participant};
  const auto candidate = evaluator->求值候选定义(candidateRequest);
  要求(candidate.状态 == 二次关系判断状态::命中 &&
             candidate.Gread == candidateRequest.Gread &&
             candidate.H == candidateRequest.H &&
             candidate.定义H == candidateRequest.Gread &&
             candidate.请求身份 == candidateRequest.请求身份 &&
             candidate.条件组.size() == 1 && candidate.来源组.size() == 2 &&
             candidate.条件组.front().计算.成功(),
         "evaluator-real-f-zero-difference-hits-zero-domain");

  const auto &safeReview = *reviewed.安全根;
  const 二次关系本能根目标合同值来源 targetSource{
      本能根角色::安全, safeReview.根材料.根需求,
      safeReview.根材料.根目标合同, safeReview.根材料.目标值,
      {safeReview.根材料.实际特征}};
  const 二次关系参与者材料 targetParticipant{
      anchor.锚点->自我.编码, {targetSource}};
  const 二次关系参与者材料 currentParticipant{
      anchor.锚点->自我.编码,
      {二次关系准确F来源{safeReview.当前实际特征}}};
  const 二次关系求值请求 targetRequest{
      2, reviewContext.Gread, reviewContext.Gread, reviewRequest.原请求.值,
      safeReview.RC, targetParticipant, currentParticipant};
  const auto targetRelation = evaluator->求值二次关系(targetRequest);
  要求((targetRelation.状态 == 二次关系判断状态::命中 ||
              targetRelation.状态 == 二次关系判断状态::不命中) &&
             targetRelation.来源组.size() == 2 &&
             std::holds_alternative<二次关系本能根目标合同值来源>(
                 targetRelation.来源组[0].来源) &&
              std::holds_alternative<二次关系本能根目标合同值见证>(
                  targetRelation.来源组[0].内容),
          "evaluator-target-contract-source-produces-dedicated-witness");
  const auto 要求目标请求结构化失败 = [&](const 二次关系求值请求& 请求,
                                             const char* 名称) {
    const auto 结果 = evaluator->求值二次关系(请求);
    要求(结果.状态 != 二次关系判断状态::命中 &&
             结果.状态 != 二次关系判断状态::不命中 &&
             结果.来源组.empty() && 结果.条件组.empty(),
         名称);
  };
  auto wrongTargetE = targetRequest;
  wrongTargetE.A.E = {reviewContext.世界.值};
  要求目标请求结构化失败(
      wrongTargetE, "target-source-wrong-existence-is-structured-failure");
  auto wrongTargetRole = targetRequest;
  std::get<二次关系本能根目标合同值来源>(
      wrongTargetRole.A.来源组.front()).角色 = 本能根角色::服务;
  要求目标请求结构化失败(
      wrongTargetRole, "target-source-wrong-role-is-structured-failure");
  auto wrongTargetDemand = targetRequest;
  std::get<二次关系本能根目标合同值来源>(
      wrongTargetDemand.A.来源组.front()).根需求 = reviewed.服务根->根材料.根需求;
  要求目标请求结构化失败(
      wrongTargetDemand, "target-source-wrong-demand-is-structured-failure");
  auto wrongTargetContract = targetRequest;
  std::get<二次关系本能根目标合同值来源>(
      wrongTargetContract.A.来源组.front()).根目标合同 =
          reviewed.服务根->根材料.根目标合同;
  要求目标请求结构化失败(
      wrongTargetContract, "target-source-wrong-contract-is-structured-failure");
  auto wrongTargetValue = targetRequest;
  std::get<二次关系本能根目标合同值来源>(
      wrongTargetValue.A.来源组.front()).目标值事实 =
          reviewed.服务根->根材料.目标值;
  要求目标请求结构化失败(
      wrongTargetValue, "target-source-wrong-value-fact-is-structured-failure");
  auto wrongTargetFormedF = targetRequest;
  std::get<二次关系本能根目标合同值来源>(
      wrongTargetFormedF.A.来源组.front()).对应实际特征 =
          {reviewed.服务根->根材料.实际特征};
  要求目标请求结构化失败(
      wrongTargetFormedF, "target-source-wrong-formed-f-is-structured-failure");
  const 二次关系求值请求 reversedTargetRequest{
      2, reviewContext.Gread, reviewContext.Gread, 0xE1003,
      safeReview.RC, currentParticipant, targetParticipant};
  const auto reversedTarget =
      evaluator->求值二次关系(reversedTargetRequest);
  要求(reversedTarget.状态 == 二次关系判断状态::类型不相容 &&
             reversedTarget.来源组.empty() && reversedTarget.条件组.empty(),
         "reversed-current-target-against-target-k-is-structured-type-failure");
  const 二次关系求值请求 targetInBothRolesRequest{
      2, reviewContext.Gread, reviewContext.Gread, 0xE1007,
      safeReview.RC, targetParticipant, targetParticipant};
  const auto targetInBothRoles =
      evaluator->求值二次关系(targetInBothRolesRequest);
  要求(targetInBothRoles.状态 == 二次关系判断状态::不命中 &&
             targetInBothRoles.来源组.size() == 2 &&
             std::holds_alternative<二次关系本能根目标合同值来源>(
                 targetInBothRoles.来源组[1].来源),
         "generic-evaluator-reaches-target-source-in-b-role");
  auto historicalTargetRequest = targetRequest;
  historicalTargetRequest.H = historicalTargetRequest.Gread - 1;
  const auto historicalTarget =
      evaluator->求值二次关系(historicalTargetRequest);
  要求(historicalTarget.状态 == 二次关系判断状态::入口拒绝 &&
             historicalTarget.来源组.empty() && historicalTarget.条件组.empty(),
         "target-source-requires-current-cutoff");
  auto mixedTargetRequest = targetRequest;
  mixedTargetRequest.A.来源组.push_back(
      二次关系准确F来源{safeReview.当前实际特征});
  const auto mixedTarget = evaluator->求值二次关系(mixedTargetRequest);
  要求(mixedTarget.状态 == 二次关系判断状态::入口拒绝 &&
             mixedTarget.来源组.empty() && mixedTarget.条件组.empty(),
         "target-source-cannot-mix-with-f-source-in-one-participant");
  auto duplicateTargetRequest = targetRequest;
  duplicateTargetRequest.A.来源组.push_back(targetSource);
  const auto duplicateTarget =
      evaluator->求值二次关系(duplicateTargetRequest);
  要求(duplicateTarget.状态 == 二次关系判断状态::入口拒绝 &&
             duplicateTarget.来源组.empty(),
         "target-source-duplicate-is-rejected");
  const 二次关系候选求值请求 targetCandidateRequest{
      2, reviewContext.Gread, reviewContext.Gread, 0xE1004,
      delivery->安全根->正差距RC.定义, targetParticipant,
      currentParticipant};
  const auto targetCandidate =
      evaluator->求值候选定义(targetCandidateRequest);
  要求(targetCandidate.状态 == 二次关系判断状态::入口拒绝 &&
             targetCandidate.来源组.empty() && targetCandidate.条件组.empty(),
         "target-source-is-rejected-before-candidate-read");
  const 二次关系FC求值请求 targetFcRequest{
      2, reviewContext.Gread, reviewContext.Gread, 0xE1005,
      delivery->安全根->先天.完整域概念.概念, targetParticipant, 0};
  const auto targetFc = evaluator->求值特征概念(targetFcRequest);
  要求(targetFc.状态 == 二次关系判断状态::入口拒绝 &&
             targetFc.来源组.empty() && targetFc.已核验概念.empty(),
         "target-source-is-rejected-before-fc-read");
  const 二次关系EC求值请求 targetEcRequest{
      2, reviewContext.Gread, reviewContext.Gread, 0xE1006,
      delivery->安全根->通用存在概念.概念, targetParticipant};
  const auto targetEc = evaluator->求值存在概念(targetEcRequest);
  要求(targetEc.状态 == 二次关系判断状态::入口拒绝 &&
             targetEc.来源组.empty() && targetEc.已核验概念.empty(),
         "target-source-is-rejected-before-ec-read");
  auto invalidRequest = relationRequest;
  invalidRequest.版本 = 1;
  const auto invalid = evaluator->求值二次关系(invalidRequest);
  要求(invalid.状态 == 二次关系判断状态::入口拒绝 &&
             invalid.Gread == 0 && invalid.H == 0 && invalid.定义H == 0 &&
             invalid.条件组.empty() && invalid.来源组.empty() &&
             invalid.已核验概念.empty(),
         "evaluator-invalid-request-clears-cutoff-and-all-business-payloads");
  const auto repeated = 构造普通应用上下文(配置(root));
  const auto repeatedDelivery = 读取普通应用本能双根二次关系概念初始化();
  要求(repeated.成功() && repeatedDelivery && repeatedDelivery->成功() &&
           repeatedDelivery->Gread == delivery->Gread &&
           repeatedDelivery->安全根->目标判断K.身份 ==
               delivery->安全根->目标判断K.身份 &&
           repeatedDelivery->服务根->正差距RC.身份 ==
               delivery->服务根->正差距RC.身份,
       "ordinary-repeat-revalidates-cached-value-delivery");
  要求(读取普通应用自我根需求复核服务() == reviewProvider,
       "ordinary-repeat-preserves-unique-root-review-provider");
  写身份(state, *delivery);
  std::cout << "PASS total=" << 通过数 << '\n';
  return 0;
}

int 普通应用恢复(const std::filesystem::path& root,
                 const std::filesystem::path& state) {
  const auto expected = 读身份(state);
  const auto assembly = 构造普通应用上下文(配置(root));
  要求(assembly.成功(), "ordinary-cross-process-assembly-success");
  const auto delivery = 读取普通应用本能双根二次关系概念初始化();
  要求(delivery && delivery->成功() &&
           delivery->状态 == 本能双根二次关系概念初始化状态::已恢复,
       "ordinary-cross-process-delivery-restored");
  const std::array<std::uint64_t, 5> actual{
      delivery->安全根->通用存在概念.概念.值.值,
      delivery->安全根->目标判断K.身份.编码.值,
      delivery->服务根->目标判断K.身份.编码.值,
      delivery->安全根->正差距RC.身份.值.值,
      delivery->服务根->正差距RC.身份.值.值};
  要求(actual == expected, "cross-process-ec-k-rc-identities-stable");
  const auto self = 初始化普通应用自我();
  要求(self.请求回显 && self.成功(*self.请求回显) && self.投影 &&
           self.投影->概念.概念 == delivery->安全根->通用存在概念.概念,
       "cross-process-self-form-reuses-universal-ec");
  std::cout << "PASS total=" << 通过数 << '\n';
  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  try {
    std::cout << std::unitbuf;
    if (argc == 4) {
      const std::string_view mode{argv[1]};
      if (mode == "ordinary-seed")
        return 普通应用种子(std::filesystem::absolute(argv[2]),
                            std::filesystem::absolute(argv[3]));
      if (mode == "ordinary-recover")
        return 普通应用恢复(std::filesystem::absolute(argv[2]),
                            std::filesystem::absolute(argv[3]));
    }
    if (argc != 1) 夹具失败("invalid validation arguments");
    验证首次重复定义与完整读取();
    验证部分材料收敛();
    验证结构化失败();
    验证损坏纯概念候选不被跳过();
    验证异义K();
    std::cout << "PASS total=" << 通过数 << '\n';
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "FAIL " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "FAIL unexpected exception\n";
    return 2;
  }
}
