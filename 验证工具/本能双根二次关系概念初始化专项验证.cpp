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
        {本能双根二次关系概念初始化合同版本, 当前代次(l1()), innate,
         本能双根RC纯概念预算, 本能双根RCK读取预算,
         本能双根RCK最大扫描候选数量, 本能双根RC二次关系预算});
  }
};

void 验证损坏纯概念候选不被跳过() {
  const auto verify = [](损坏纯概念候选模式 mode,
                         std::string_view name) {
    夹具 f(mode);
    const auto g = 当前代次(f.l1());
    const 纯概念定义 universal =
        通用存在概念定义{1, 通用存在定义规则::不预设特征};
    const 纯概念查询请求 request{
        2, g, g, universal, 本能双根RC纯概念预算};
    const auto result = f.概念->精确查询纯概念(request);
    要求(result.状态 == 纯概念状态::内部不一致 && !result.事实,
         std::string(name) + "-exact-query");
    const auto concepts = f.概念->读取当前I64特征概念(
        {2, g, g, 特征类型身份{{1}}, 本能双根RC纯概念预算});
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

bool K写读取用量为零(const 特征I64比较绑定结果& r) {
  return !r.读取用量.节点数 && !r.读取用量.关系数 &&
         !r.读取用量.值数 && !r.读取用量.材料总数;
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
  const 纯概念创建请求 request{
      2, g, {0x3001}, definition, 概念初始组织指定::显式顶层, {},
      本能双根RC纯概念预算};
  const auto result = f.概念->创建或复用纯概念(request);
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
  if (!result.成功() || !result.事实 || !K写读取用量为零(result))
    夹具失败("prebuilt K failed");
  return *result.事实;
}

二次关系概念事实 预建RC(夹具& f, const 二次关系定义& definition,
                         std::uint64_t key) {
  const auto g = 当前代次(f.l1());
  const 二次关系概念建立请求 request{
      1, {1, g, {key}}, definition, 概念初始组织指定::显式顶层,
      {}, {}, 本能双根RC二次关系预算};
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
        {1, 当前代次(f.l1()), f.读取先天(), 本能双根RC纯概念预算,
         本能双根RCK读取预算, 本能双根RCK最大扫描候选数量,
         本能双根RC二次关系预算});
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
        {1, 当前代次(f.l1()), freshInnate, 本能双根RC纯概念预算,
         本能双根RCK读取预算, 本能双根RCK最大扫描候选数量,
         本能双根RC二次关系预算});
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
        {1, 当前代次(f.l1()), freshInnate, 本能双根RC纯概念预算,
         本能双根RCK读取预算, 本能双根RCK最大扫描候选数量,
         本能双根RC二次关系预算});
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
          {1, {1, g, {key}}, first.安全根->正差距RC.身份, from, to,
           first.安全根->目标判断K.身份.编码,
           本能双根RC二次关系预算});
    };
    const auto cooling = retire(概念树生命周期状态::活跃,
                                概念树生命周期状态::冷却, 0x71E01);
    要求(cooling.成功(), "rc-retirement-fixture-cooling");
    const auto cooledG = 当前代次(f.l1());
    const auto cooledLookup = f.概念->查找二次关系完整定义(
        {1, {1, cooledG, cooledG}, first.安全根->正差距RC.定义,
         本能双根RC二次关系预算});
    const auto cooledRead = f.概念->读取二次关系概念(
        {1, {1, cooledG, cooledG}, first.安全根->正差距RC.身份,
         本能双根RC二次关系预算});
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

void 验证首次重复定义与预算() {
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
             replay.状态 == 特征I64比较绑定状态::精确重复 &&
             K写读取用量为零(replay),
         "k-write-replay-is-budgetless-and-zero-read-usage");
    const auto current = f.特征->读取当前I64比较绑定(
        {1, first.Gread, root->先天.类型.身份,
         特征I64比较用途::目标判断, 本能双根RCK最大扫描候选数量,
         本能双根RCK读取预算});
    const auto byId = f.特征->读取I64比较绑定(
        {1, first.Gread, first.Gread, root->目标判断K.身份,
         本能双根RCK读取预算});
    要求(current.成功() && byId.成功() &&
             current.事实->身份 == root->目标判断K.身份 &&
             byId.事实->身份 == root->目标判断K.身份 &&
             current.当前读取原请求 &&
             current.当前读取原请求->最大扫描候选数量 ==
                 本能双根RCK最大扫描候选数量 &&
             current.当前读取原请求->读取预算 == 本能双根RCK读取预算,
         "k-current-and-identity-read-use-named-budget");
  }

  const auto beforeRepeat = 当前代次(f.l1());
  const 纯概念定义 universal =
      通用存在概念定义{1, 通用存在定义规则::不预设特征};
  const 纯概念查询请求 universalRequest{
      2, beforeRepeat, beforeRepeat, universal, 本能双根RC纯概念预算};
  const auto universalRead = f.概念->精确查询纯概念(universalRequest);
  要求(universalRead.成功(universalRequest) && universalRead.事实 &&
             universalRead.事实->概念 ==
                 first.安全根->通用存在概念.概念 &&
             当前代次(f.l1()) == beforeRepeat,
         "pure-query-skips-valid-rc-family-members-and-reuses-ec");
  const I64特征概念组织读取请求 safeConceptRequest{
      2, beforeRepeat, beforeRepeat, first.安全根->先天.类型.身份,
      本能双根RC纯概念预算};
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
      {1, staleInnate.Gread, staleInnate, 本能双根RC纯概念预算,
       本能双根RCK读取预算, 本能双根RCK最大扫描候选数量,
       本能双根RC二次关系预算});
  要求(drift.状态 == 本能双根二次关系概念初始化状态::当前性漂移 &&
           !drift.安全根 && !drift.服务根,
       "generation-drift-is-structured-and-clears-delivery");
}

void 验证读取预算不足() {
  夹具 f;
  const auto initialized = f.初始化();
  if (!initialized.成功()) 夹具失败("budget fixture initialization failed");
  const auto g = 当前代次(f.l1());

  auto tinyPure = 本能双根RC纯概念预算;
  tinyPure.最大概念数 = 1;
  tinyPure.最大关系数 = 1;
  tinyPure.最大特征属性数 = 1;
  const 纯概念定义 universal =
      通用存在概念定义{1, 通用存在定义规则::不预设特征};
  const auto pure = f.概念->精确查询纯概念(
      {2, g, g, universal, tinyPure});
  要求(pure.状态 == 纯概念状态::数量预算不足 && !pure.事实,
       "pure-read-budget-shortage-is-structured");

  const 有界事实读取预算_B1 tinyK{1, 1, 1, 1};
  const auto k = f.特征->读取I64比较绑定(
      {1, g, g, initialized.安全根->目标判断K.身份, tinyK});
  要求(k.状态 == 特征I64比较绑定状态::数量预算不足 && !k.事实,
       "k-read-budget-shortage-is-structured");

  二次关系预算 tinyRc;
  tinyRc.最大节点数 = tinyRc.最大关系数 = tinyRc.最大值数 =
      tinyRc.最大材料数 = tinyRc.最大概念数 = tinyRc.最大原子数 =
          tinyRc.最大展开深度 = tinyRc.最大候选数 =
              tinyRc.最大来源数 = tinyRc.最大用途数 =
                  tinyRc.最大首次材料数 = 1;
  const auto rc = f.概念->读取二次关系概念(
      {1, {1, g, g}, initialized.安全根->正差距RC.身份, tinyRc});
  要求(rc.状态 == 二次关系数据状态::预算不足 && !rc.事实,
       "rc-read-budget-shortage-is-structured");
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
  const auto repeated = 构造普通应用上下文(配置(root));
  const auto repeatedDelivery = 读取普通应用本能双根二次关系概念初始化();
  要求(repeated.成功() && repeatedDelivery && repeatedDelivery->成功() &&
           repeatedDelivery->Gread == delivery->Gread &&
           repeatedDelivery->安全根->目标判断K.身份 ==
               delivery->安全根->目标判断K.身份 &&
           repeatedDelivery->服务根->正差距RC.身份 ==
               delivery->服务根->正差距RC.身份,
       "ordinary-repeat-revalidates-cached-value-delivery");
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
    验证首次重复定义与预算();
    验证部分材料收敛();
    验证结构化失败();
    验证损坏纯概念候选不被跳过();
    验证读取预算不足();
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
