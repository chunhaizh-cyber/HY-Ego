#include "../海中鱼巣/业务/应用服务.本能根运行初始化.内部.h"
#include "../海中鱼巣/业务/应用服务.本能根运行初始化.h"
#include "../海中鱼巣/业务/应用服务.方法登记根生产初始化.h"
#include "../海中鱼巣/业务/应用服务.世界树类.h"
#include "../海中鱼巣/业务/应用服务.自我形成.h"
#include "../海中鱼巣/领域/数据服务.世界树根.h"
#include "../海中鱼巣/线程/线程_自我.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace {
using namespace 海中鱼巣;
using namespace 海中鱼巣::本能根运行初始化内部;

int 通过数 = 0;

void 要求(bool 成立, std::string_view 名称) {
  if (!成立) {
    std::cerr << "FAIL " << 名称 << '\n';
    throw std::runtime_error("instinct root runtime anchor probe failed");
  }
  ++通过数;
  std::cout << "PASS " << 名称 << '\n';
}

[[noreturn]] void 夹具失败(const char *消息) {
  throw std::runtime_error(消息);
}

std::uint64_t 当前代次(const L1事实基座服务 &l1) {
  const auto r = l1.读取中性当前事实代次({L1中性CRUD合同版本});
  if (r.状态 != L1中性读取状态::成功 || !r.事实代次)
    夹具失败("fixture generation read failed");
  return r.事实代次;
}

L1所有者范围交付 建立所有者(L1所有者范围签发器 &签发器,
                              const L1事实基座服务 &l1,
                              std::uint64_t key) {
  const L1所有者范围建立请求 请求{
      L1所有者范围CRUD合同版本, {key}, L1所有者范围种类::独占结构范围};
  auto r = 签发器.建立所有者范围(请求);
  if ((r.建立结果.状态 != L1所有者范围管理状态::成功 &&
       r.建立结果.状态 != L1所有者范围管理状态::精确重复) ||
      !r.建立结果.所有者事实 || !r.写入端口 || !r.写入端口->有效() ||
      !r.写入端口->绑定于(l1))
    夹具失败("fixture owner creation failed");
  return r;
}

using 元节点规格 =
    std::pair<节点种类, std::optional<L1所有者范围值表示种类>>;

struct 首次定位 final {
  bool 已有 = false;
  std::uint64_t G0 = 0;
};

首次定位 定位首次(L1所有者范围写端口 &端口,
                   const L1事实基座服务 &l1, std::uint64_t key) {
  const auto g = 当前代次(l1);
  const auto first = 端口.读取首次写入材料(
      {L1所有者范围首次写入读取合同版本, {key}});
  if (first.读取事实代次 != g || first.所有者 != 端口.所有者身份() ||
      first.写入幂等身份.值 != key)
    夹具失败("fixture first-write read mismatch");
  if (first.状态 == L1所有者范围读取状态::未找到) {
    if (first.首次规范化写集 || first.首次写入结果)
      夹具失败("fixture absent first-write carried payload");
    return {false, g};
  }
  if (first.状态 != L1所有者范围读取状态::成功 ||
      !first.首次规范化写集 || !first.首次写入结果 ||
      first.首次规范化写集->写入幂等身份.值 != key ||
      !first.首次规范化写集->期望事实代次)
    夹具失败("fixture first-write read failed");
  return {true, first.首次规范化写集->期望事实代次};
}

std::vector<稳定编码>
建立元节点(L1所有者范围写端口 &端口, const L1事实基座服务 &l1,
           std::uint64_t key, const std::vector<元节点规格> &规格) {
  const auto first = 定位首次(端口, l1, key);
  L1所有者范围写集请求 请求{L1所有者范围CRUD合同版本, first.G0, {key}};
  for (std::uint32_t i = 0; i < 规格.size(); ++i)
    请求.节点.push_back({{static_cast<std::uint64_t>(i) + 1},
                         规格[i].first, 规格[i].second});
  const auto r = 端口.提交所有者范围中性写集(请求);
  const auto expected = first.已有 ? L1所有者范围写入状态::精确重复
                                   : L1所有者范围写入状态::成功;
  if (r.状态 != expected ||
      r.新编码映射.size() != 规格.size())
    夹具失败("fixture metadata creation failed");
  std::vector<稳定编码> ids(规格.size());
  for (const auto &[local, id] : r.新编码映射) {
    if (!local.值 || local.值 > ids.size() || !有效(id) ||
        有效(ids[local.值 - 1]))
      夹具失败("fixture metadata mapping failed");
    ids[local.值 - 1] = id;
  }
  return ids;
}

void 忽略实例特征结构异常(存在信息身份, 特征类型身份,
                          实例特征结构异常, std::uint64_t,
                          std::uint64_t) noexcept {}

struct 真实服务栈 final {
  std::unique_ptr<L1事实基座运行包> 运行包;
  L1所有者范围交付 特征定义所有者, 特征信息所有者,
      存在所有者, 场景所有者, 状态所有者, 概念所有者, 需求所有者,
      方法登记根所有者;
  std::unique_ptr<特征值类数据服务> 特征值;
  std::unique_ptr<特征类数据服务> 特征;
  std::unique_ptr<存在类数据服务> 存在;
  std::unique_ptr<需求类数据服务> 需求;
  std::unique_ptr<状态类数据服务> 状态;
  std::unique_ptr<场景类数据服务> 场景;
  std::unique_ptr<概念树类数据服务> 概念;
  本能先天特征概念初始化结果 本能初始化;
  std::unique_ptr<原子I64特征出生数据服务> 原子I64特征出生;
  std::unique_ptr<特征概念应用服务> 特征概念;
  std::unique_ptr<世界树根数据服务> 世界根服务;
  std::unique_ptr<世界树应用服务> 世界树;
  std::unique_ptr<真实自我形成服务> 自我服务;
  std::unique_ptr<L2方法结构聚合服务> 方法结构聚合;
  std::unique_ptr<方法登记根生产初始化提供者> 方法登记根初始化;
  存在单例角色结构交付 角色结构;
  稳定编码 世界根{};
  真实自我形成结果 自我形成;
  std::optional<std::uint64_t> 根原G0;
  std::uint64_t 入口事实代次 = 0;

  explicit 真实服务栈(
      std::optional<std::filesystem::path> 持久根 = std::nullopt) {
    if (持久根) {
      auto persistent = 建立L1事实基座持久运行包_v1(
          {L1事实基座持久恢复合同版本_v1, *持久根});
      if (!persistent.成功() || !persistent.运行包)
        夹具失败("fixture persistent recovery failed");
      运行包 = std::move(persistent.运行包);
    } else {
      运行包 =
          std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    }
    auto &l1 = 运行包->读取服务();
    const auto entrance =
        l1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (entrance.状态 != L1中性读取状态::成功 &&
        !(entrance.状态 == L1中性读取状态::未找到 &&
          entrance.事实代次 == 0))
      夹具失败("fixture entrance generation read failed");
    入口事实代次 = entrance.事实代次;
    auto &签发器 = 运行包->所有者范围签发器();
    特征定义所有者 = 建立所有者(签发器, l1, 0x51001);
    特征信息所有者 = 建立所有者(签发器, l1, 0x51002);
    存在所有者 = 建立所有者(签发器, l1, 0x51003);
    场景所有者 = 建立所有者(签发器, l1, 0x51004);
    状态所有者 = 建立所有者(签发器, l1, 0x51005);
    概念所有者 = 建立所有者(签发器, l1, 0x51006);
    需求所有者 = 建立所有者(签发器, l1, 0x51007);
    方法登记根所有者 =
        建立所有者(签发器, l1, 方法登记根初始化所有者建立键);

    const auto producers = 建立元节点(
        *特征定义所有者.写入端口, l1, 0x1101,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    std::vector<元节点规格> definition(16,
                                         {节点种类::普通, std::nullopt});
    for (auto i : {2, 9, 13})
      definition[i] =
          {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (auto i : {3, 8})
      definition[i] =
          {节点种类::属性类型, L1所有者范围值表示种类::I64};
    definition[14] =
        {节点种类::属性类型, L1所有者范围值表示种类::I64组};
    (void)建立元节点(*特征定义所有者.写入端口, l1, 1,
                     definition);
    (void)建立元节点(
        *特征信息所有者.写入端口, l1, 1,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::普通, std::nullopt}});
    const auto existenceLayout = 建立元节点(
        *存在所有者.写入端口, l1, 0x1201,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    (void)建立元节点(*存在所有者.写入端口, l1,
                     0x455849535446414DULL,
                     {{节点种类::普通, std::nullopt},
                      {节点种类::普通, std::nullopt}});
    const auto stateLayout = 建立元节点(
        *状态所有者.写入端口, l1, 0x1301,
        {{节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::U64组},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64}});

    const auto existenceRoot =
        定位首次(*存在所有者.写入端口, l1, 0x2002);
    const auto sceneRoot = 定位首次(*场景所有者.写入端口, l1, 0x2003);
    if (existenceRoot.已有 != sceneRoot.已有 ||
        (existenceRoot.已有 && existenceRoot.G0 != sceneRoot.G0))
      夹具失败("fixture world-root first-write mismatch");
    if (existenceRoot.已有)
      根原G0 = existenceRoot.G0;

    特征值 = std::make_unique<特征值类数据服务>(l1);
    特征 = std::make_unique<特征类数据服务>(
        l1, std::move(*特征定义所有者.写入端口),
        std::move(*特征信息所有者.写入端口), *特征值,
        producers.front());
    if (!std::holds_alternative<std::monostate>(特征->初始化特征定义结构()) ||
        !std::holds_alternative<std::monostate>(特征->初始化准确特征结构()))
      夹具失败("fixture feature layout failed");

    const 存在单例角色结构登记请求 roleRequest{
        1, 定位首次(*存在所有者.写入端口, l1, 0x1202).G0, {0x1202}};
    const auto role = 存在类数据服务::登记单例角色结构(
        l1, *存在所有者.写入端口, roleRequest);
    if (!role.成功(roleRequest) || !role.交付)
      夹具失败("fixture role layout failed");
    角色结构 = *role.交付;
    const 实例特征结构登记请求 instanceFeatureRequest{
        2, 当前代次(l1), {0x4946525354525632ULL}};
    const auto instanceFeature = 存在类数据服务::登记实例特征结构(
        l1, *存在所有者.写入端口, instanceFeatureRequest);
    if (!instanceFeature.成功(instanceFeatureRequest) ||
        !instanceFeature.交付) {
      std::cerr << "instance-feature status="
                << static_cast<unsigned>(instanceFeature.状态)
                << " version=" << instanceFeature.版本
                << " request-g0=" << instanceFeatureRequest.G0
                << " gread=" << instanceFeature.Gread << '\n';
      夹具失败("fixture instance-feature layout failed");
    }
    存在 = std::make_unique<存在类数据服务>(
        l1, *特征, std::move(*存在所有者.写入端口), existenceLayout[0],
        existenceLayout[1], 存在当前采用结构交付{existenceLayout[2]},
        *instanceFeature.交付, 角色结构);

    const 需求结构登记请求 demandRequest{需求结构登记合同版本,
                                             当前代次(l1)};
    const auto demandRegistration = 需求类数据服务::登记需求结构(
        l1, *需求所有者.写入端口, demandRequest);
    if (!demandRegistration.成功(demandRequest) ||
        !demandRegistration.交付)
      夹具失败("fixture demand layout failed");
    需求 = std::make_unique<需求类数据服务>(
        l1, *特征, *存在, std::move(*需求所有者.写入端口),
        *demandRegistration.交付);
    状态 = std::make_unique<状态类数据服务>(
        l1, *特征, std::move(*状态所有者.写入端口),
        状态类结构交付{stateLayout[0], stateLayout[1], stateLayout[2],
                       stateLayout[3], stateLayout[4], stateLayout[5],
                       stateLayout[6], stateLayout[7]});

    const 场景结构登记请求 baseRequest{
        2, 定位首次(*场景所有者.写入端口, l1, 1).G0, {1}};
    const auto base = 场景类数据服务::登记结构类型(
        l1, *场景所有者.写入端口, baseRequest);
    if (!base.成功(baseRequest) || !base.交付)
      夹具失败("fixture scene layout failed");
    const 场景特征组织扩展登记请求 featureRequest{
        1, 定位首次(*场景所有者.写入端口, l1,
                    场景特征组织扩展登记固定幂等身份.值)
               .G0};
    const auto feature = 场景类数据服务::登记特征组织扩展(
        l1, *场景所有者.写入端口, featureRequest);
    if (!feature.成功(featureRequest) || !feature.交付)
      夹具失败("fixture scene feature extension failed");
    const 场景直接包含扩展登记请求 includeRequest{
        1, 定位首次(*场景所有者.写入端口, l1,
                    场景直接包含扩展登记固定幂等身份.值)
               .G0};
    const auto include = 场景类数据服务::登记直接包含扩展(
        l1, *场景所有者.写入端口, includeRequest);
    if (!include.成功(includeRequest) || !include.交付)
      夹具失败("fixture scene containment extension failed");
    场景 = std::make_unique<场景类数据服务>(
        l1, std::move(*场景所有者.写入端口), *base.交付, *存在, *状态,
        *feature.交付, *include.交付);

    const 纯概念结构登记请求_v2 conceptRequest{
        2, 定位首次(*概念所有者.写入端口, l1, 0x1401).G0,
        {0x1401}, 1048576};
    const auto conceptRegistration = 概念树类数据服务::登记纯概念结构_v2(
        l1, *概念所有者.写入端口, conceptRequest);
    if (!conceptRegistration.成功(conceptRequest) ||
        !conceptRegistration.交付)
      夹具失败("fixture concept layout failed");
    const 特征概念出生使用结构登记请求 featureBirthRequest{
        1, 定位首次(*概念所有者.写入端口, l1, 0x1402).G0,
        {0x1402}, *conceptRegistration.交付, 4};
    const auto featureBirth = 概念树类数据服务::登记特征概念出生使用结构(
        l1, *概念所有者.写入端口, featureBirthRequest);
    if (!featureBirth.成功(featureBirthRequest) || !featureBirth.交付)
      夹具失败("fixture feature-concept layout failed");
    const 存在概念两组结构登记请求_v1 definitionRequest{
        1, 定位首次(*概念所有者.写入端口, l1, 0x1403).G0,
        {0x1403}, *conceptRegistration.交付, 18};
    const auto completeDefinition =
        概念树类数据服务::登记存在概念两组结构_v1(
            l1, *概念所有者.写入端口, definitionRequest);
    if (!completeDefinition.成功(definitionRequest) ||
        !completeDefinition.交付)
      夹具失败("fixture existence-concept layout failed");
    概念 = std::make_unique<概念树类数据服务>(
        l1, *特征, *存在, *特征值, *场景,
        std::move(*概念所有者.写入端口), *conceptRegistration.交付,
        *featureBirth.交付, *completeDefinition.交付);

    本能先天特征概念初始化提供者 instinct(*特征, *概念);
    本能初始化 = instinct.初始化({1, 当前代次(l1),
                                  {64, 256, 0, 0, 0, 64, 0, 0}});
    if (!本能初始化.成功())
      夹具失败("fixture instinct feature initialization failed");
    原子I64特征出生 = std::make_unique<原子I64特征出生数据服务>(
        *特征, *存在, *场景, *概念);
    特征概念 = std::make_unique<特征概念应用服务>(
        *特征, *概念, *原子I64特征出生, *存在, 本能初始化,
        忽略实例特征结构异常);

    世界根服务 = std::make_unique<世界树根数据服务>(*存在, *场景);
    const 世界树根初始化请求 rootRequest{世界树根合同版本,
                                             根原G0.value_or(当前代次(l1)),
                                             {0x2001}, {0x2002},
                                             {0x2003}, 256, 64};
    const auto root = 世界根服务->初始化世界树根(rootRequest);
    if (!root.成功(rootRequest))
      夹具失败("fixture world root initialization failed");
    const 世界树根验证请求 verifyRequest{2, root.Gread, {64, 256}};
    auto world = 建立世界树应用服务(*场景, *存在, *概念, verifyRequest);
    if (!world.成功() || !world.服务)
      夹具失败("fixture world service failed");
    世界树 = std::move(world.服务);
    const 世界树读取预算 rootBudget{64, 256};
    const auto rootRead = 世界树->读取当前现实世界根(rootBudget);
    if (!rootRead.成功(rootBudget) || !rootRead.树)
      夹具失败("fixture world root read failed");
    世界根 = rootRead.树->根场景;
    自我服务 = std::make_unique<真实自我形成服务>(
        *世界树, *存在, 角色结构.项目角色);
    const 真实自我形成请求 selfRequest{
        1, 世界根, 角色结构.项目角色, {0x3001},
        {{0x3002}, {0x3003}, {0x3004}, {0x3005}},
        {256, 64, 64}, {256, 256, 256, 64, 64, 64}};
    自我形成 = 自我服务->形成(selfRequest);
    if (!自我形成.成功(selfRequest))
      夹具失败("fixture self formation failed");

    方法结构聚合 = std::make_unique<L2方法结构聚合服务>(
        l1, std::move(*方法登记根所有者.写入端口));
    方法登记根初始化 = std::make_unique<方法登记根生产初始化提供者>(
        *方法结构聚合);
  }

  std::uint64_t 取得阶段19截止() {
    const auto r = 方法登记根初始化->初始化({1});
    if (!r.成功()) {
      std::cerr << "stage19 status=" << static_cast<unsigned>(r.状态)
                << " g=" << r.最终Gread << '\n';
      夹具失败("fixture stage19 read failed");
    }
    return r.最终Gread;
  }

  真实自我读取请求 构造自我读取(std::uint64_t g) const {
    return {1, g, 世界根, 角色结构.项目角色, {256, 64, 64},
            {256, 256, 256, 64, 64, 64}};
  }

  真实自我形成结果 读取自我(std::uint64_t g) const {
    const auto request = 构造自我读取(g);
    auto r = 自我服务->读取当前自我(request);
    if (!r.成功(request))
      夹具失败("fixture fresh self read failed");
    return r;
  }
};

自我线程本能根启动材料_v1
映射自我启动材料(const 本能根运行锚点_v1 &锚点) {
  return {{锚点.自我.编码.值},
          {{锚点.安全根.需求.值}, {锚点.安全根.列表项.值},
           {锚点.安全根.实际特征.编码.值},
           {锚点.安全根.目标合同.值}},
          {{锚点.服务根.需求.值}, {锚点.服务根.列表项.值},
           {锚点.服务根.实际特征.编码.值},
           {锚点.服务根.目标合同.值}},
          锚点.事实截止代次};
}

真实自我投影 构造脚本投影() {
  真实自我投影 投影;
  投影.Gread = 100;
  投影.H = 100;
  投影.E = {10};
  投影.场景.场景角色.场景 = 投影.E;
  for (std::size_t i = 0; i < 投影.场景.场景角色.四根.size(); ++i) {
    auto &根 = 投影.场景.场景角色.四根[i];
    根.角色 = static_cast<场景根角色>(i + 1);
    根.根.编码 = {20 + static_cast<std::uint64_t>(i)};
  }
  return 投影;
}

enum class 脚本模式 : std::uint8_t {
  首次形成,
  全部恢复,
  安全F前缀后失败,
  安全根前缀后失败,
  服务F前缀后失败,
  实际F当前采用读取失败,
  实际F缺嵌套载荷,
  实际F材料未闭合,
  实际F资源失败,
  实际F已可能发布,
  安全根已可能发布,
  服务F已可能发布,
  服务根已可能发布,
  根幂等冲突,
  根材料退出,
  实际F引用冲突
};

class 脚本端口 final : public 本能根运行初始化调用端口 {
public:
  explicit 脚本端口(脚本模式 模式 = 脚本模式::首次形成,
                    std::uint64_t 漂移守卫序号 = 0)
      : 模式_(模式), 漂移守卫序号_(漂移守卫序号) {}

  本能根I64实际F结果
  形成或读取实际F(const 本能根I64实际F请求 &请求) noexcept override {
    ++F调用数_;
    核对F请求(请求);
    if (F调用数_ == 1 &&
        (模式_ == 脚本模式::实际F当前采用读取失败 ||
         模式_ == 脚本模式::实际F缺嵌套载荷 ||
         模式_ == 脚本模式::实际F材料未闭合 ||
         模式_ == 脚本模式::实际F资源失败)) {
      本能根I64实际F结果 out;
      out.原请求 = 请求;
      if (模式_ == 脚本模式::实际F当前采用读取失败)
        out.状态 = 本能根I64实际F状态::当前采用读取失败;
      else if (模式_ == 脚本模式::实际F缺嵌套载荷)
        out.状态 = 本能根I64实际F状态::IFR失败;
      else if (模式_ == 脚本模式::实际F材料未闭合)
        out.状态 = 本能根I64实际F状态::初始化交付不完整;
      else
        out.状态 = 本能根I64实际F状态::资源失败;
      return out;
    }
    if (模式_ == 脚本模式::实际F引用冲突 && F调用数_ == 1) {
      本能根I64实际F结果 out;
      out.原请求 = 请求;
      out.状态 = 本能根I64实际F状态::引用冲突;
      return out;
    }
    if ((模式_ == 脚本模式::实际F已可能发布 && F调用数_ == 1) ||
        (模式_ == 脚本模式::服务F已可能发布 && F调用数_ == 2)) {
      本能根I64实际F结果 out;
      out.原请求 = 请求;
      out.状态 = 本能根I64实际F状态::IFR失败;
      实例特征R观察结果 观察;
      观察.状态 = 实例特征R观察状态::IFR写入失败;
      观察.Gread = 请求.G0 + 1;
      实例特征IFR结果 IFR;
      IFR.状态 = 实例特征IFR状态::已可能发布;
      IFR.Gread = 观察.Gread;
      观察.IFR = IFR;
      out.IFR = std::move(观察);
      return out;
    }
    if ((模式_ == 脚本模式::安全根前缀后失败 && F调用数_ == 2)) {
      本能根I64实际F结果 out;
      out.原请求 = 请求;
      out.状态 = 本能根I64实际F状态::资源失败;
      return out;
    }

    const bool 初始调用 = F调用数_ <= 2;
    const bool 本次形成 =
        (模式_ == 脚本模式::首次形成 && 初始调用) ||
        ((模式_ == 脚本模式::安全F前缀后失败 ||
          模式_ == 脚本模式::安全根前缀后失败 ||
          模式_ == 脚本模式::服务F前缀后失败) &&
         F调用数_ == 1) ||
        (模式_ == 脚本模式::服务F前缀后失败 && F调用数_ == 2);
    if (本次形成)
      ++当前代次_;
    else
      当前代次_ = 请求.G0;
    auto out = 构造F成功(请求, 当前代次_, 本次形成);
    if (F调用数_ == 1)
      安全实际F_ = out.实际F;
    else if (F调用数_ == 2)
      服务实际F_ = out.实际F;
    return out;
  }

  本能根材料结果
  建立或读取根材料(const 本能根材料请求 &请求) override {
    ++根调用数_;
    核对根请求(请求);
    if ((模式_ == 脚本模式::安全根已可能发布 && 根调用数_ == 1) ||
        (模式_ == 脚本模式::服务根已可能发布 && 根调用数_ == 2))
      return {本能根材料状态::已可能发布, 本能根材料合同版本,
              请求.期望事实代次, std::nullopt};
    if (根调用数_ == 1 && 模式_ == 脚本模式::根幂等冲突)
      return {本能根材料状态::幂等冲突, 本能根材料合同版本,
              请求.期望事实代次, std::nullopt};
    if (根调用数_ == 1 && 模式_ == 脚本模式::根材料退出)
      return {本能根材料状态::根材料已退出, 本能根材料合同版本,
              请求.期望事实代次, std::nullopt};
    if ((模式_ == 脚本模式::安全F前缀后失败 && 根调用数_ == 1) ||
        (模式_ == 脚本模式::服务F前缀后失败 && 根调用数_ == 2))
      return {本能根材料状态::资源失败, 本能根材料合同版本,
              请求.期望事实代次, std::nullopt};

    const bool 初始调用 = 根调用数_ <= 2;
    const bool 本次形成 =
        (模式_ == 脚本模式::首次形成 && 初始调用) ||
        ((模式_ == 脚本模式::安全根前缀后失败 ||
          模式_ == 脚本模式::服务F前缀后失败) &&
         根调用数_ == 1);
    if (本次形成)
      ++当前代次_;
    else
      当前代次_ = 请求.期望事实代次;
    return 构造根成功(请求, 当前代次_, 本次形成);
  }

  需求当前事实代次核验结果
  核验当前事实代次(std::uint64_t 期望) const noexcept override {
    ++守卫调用数_;
    if (!已开始_) {
      已开始_ = true;
      当前代次_ = 期望;
    } else if (期望 != 当前代次_) {
      请求一致_ = false;
    }
    if (漂移守卫序号_ && 守卫调用数_ == 漂移守卫序号_)
      return {需求类数据状态::事实代次漂移,
              需求当前事实代次核验合同版本, 期望, 期望 + 1};
    return {需求类数据状态::已读取,
            需求当前事实代次核验合同版本, 期望, 期望};
  }

  [[nodiscard]] std::uint64_t 守卫调用数() const noexcept {
    return 守卫调用数_;
  }

  [[nodiscard]] bool 请求一致() const noexcept { return 请求一致_; }

  [[nodiscard]] std::uint64_t 当前代次() const noexcept {
    return 当前代次_;
  }

private:
  static std::array<std::uint64_t, 8>
  键组(const 本能根I64实际F请求 &请求) noexcept {
    return {请求.IFR键.值, 请求.FCv概念键.值, 请求.当前采用键.值,
            请求.F出生键.组合.值, 请求.F出生键.内容.值,
            请求.F出生键.已知.值, 请求.F出生键.组织.值,
            请求.F出生键.概念使用.值};
  }

  static bool 键组有效且互异(const std::array<std::uint64_t, 8> &键) noexcept {
    for (std::size_t i = 0; i < 键.size(); ++i) {
      if (!键[i])
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (键[i] == 键[j])
          return false;
    }
    return true;
  }

  static bool 公共F合同一致(const 本能根I64实际F请求 &a,
                            const 本能根I64实际F请求 &b) noexcept {
    return a.版本 == b.版本 && a.E == b.E && a.位置 == b.位置 &&
           a.IFR预算 == b.IFR预算 && a.R规则预算 == b.R规则预算 &&
           a.候选预算 == b.候选预算 && a.组织预算 == b.组织预算 &&
           a.概念读取预算 == b.概念读取预算 && a.概念预算 == b.概念预算 &&
           a.当前采用关系预算 == b.当前采用关系预算;
  }

  void 核对F请求(const 本能根I64实际F请求 &请求) noexcept {
    if (请求.G0 != 当前代次_ || 请求.版本 != 1 || !有效(请求.E) ||
        请求.位置.场景 != 请求.E.编码 || !有效(请求.位置.组织父) ||
        !键组有效且互异(键组(请求))) {
      请求一致_ = false;
      return;
    }
    if (F调用数_ == 1) {
      if (请求.角色 != 本能值角色::安全值)
        请求一致_ = false;
      安全F请求_ = 请求;
      return;
    }
    if (F调用数_ == 2) {
      if (!安全F请求_ || 请求.角色 != 本能值角色::服务值 ||
          !公共F合同一致(请求, *安全F请求_))
        请求一致_ = false;
      const auto a = 安全F请求_ ? 键组(*安全F请求_)
                                 : std::array<std::uint64_t, 8>{};
      const auto b = 键组(请求);
      for (const auto x : a)
        for (const auto y : b)
          if (x == y)
            请求一致_ = false;
      服务F请求_ = 请求;
      return;
    }
    auto expected = F调用数_ == 3 ? 安全F请求_ : 服务F请求_;
    if (!expected) {
      请求一致_ = false;
      return;
    }
    expected->G0 = 请求.G0;
    if (请求 != *expected)
      请求一致_ = false;
  }

  void 核对根请求(const 本能根材料请求 &请求) noexcept {
    if (请求.合同版本 != 本能根材料合同版本 ||
        请求.期望事实代次 != 当前代次_) {
      请求一致_ = false;
      return;
    }
    if (根调用数_ == 1) {
      if (请求.角色 != 本能根角色::安全 || !安全实际F_ ||
          请求.实际特征 != 安全实际F_->编码)
        请求一致_ = false;
      安全根请求_ = 请求;
      return;
    }
    if (根调用数_ == 2) {
      if (请求.角色 != 本能根角色::服务 || !服务实际F_ ||
          请求.实际特征 != 服务实际F_->编码)
        请求一致_ = false;
      服务根请求_ = 请求;
      return;
    }
    auto expected = 根调用数_ == 3 ? 安全根请求_ : 服务根请求_;
    if (!expected) {
      请求一致_ = false;
      return;
    }
    expected->期望事实代次 = 请求.期望事实代次;
    if (请求 != *expected)
      请求一致_ = false;
  }

  static 本能根I64实际F结果
  构造F成功(const 本能根I64实际F请求 &请求, std::uint64_t g,
            bool 形成) {
    const bool 安全 = 请求.角色 == 本能值角色::安全值;
    const 稳定编码 f{安全 ? 301ULL : 401ULL};
    const 特征类型身份 ft{{安全 ? 501ULL : 601ULL}};
    const 概念树概念身份 fc{{安全 ? 701ULL : 801ULL}};

    本能根I64实际F结果 out;
    out.原请求 = 请求;
    out.状态 = 形成 ? 本能根I64实际F状态::已形成
                    : 本能根I64实际F状态::已读取;
    先天I64特征概念交付 交付;
    交付.角色 = 请求.角色;
    交付.类型.身份 = ft;
    交付.完整域概念.概念 = fc;
    交付.Gread = 请求.G0;
    out.初始化交付 = std::move(交付);

    实例特征IFR结果 IFR;
    IFR.状态 = 实例特征IFR状态::已读取;
    IFR.Gread = g;
    IFR.H = g;
    IFR.投影 = 实例特征IFR完整投影{};
    out.IFR读回 = std::move(IFR);

    存在当前采用结果 当前;
    当前.状态 = 存在类数据状态::已读取;
    当前.Gread = g;
    当前.H = g;
    当前.采用 = 存在当前采用事实{{安全 ? 302ULL : 402ULL},
                                    请求.E.编码, ft, 特征信息身份{f}};
    out.当前采用 = std::move(当前);

    准确特征读取事实 准确;
    准确.Gread = g;
    准确.H = g;
    准确.信息 = {特征信息身份{f}, ft,
                 std::numeric_limits<std::int64_t>::max() / 2};
    准确.完整值 = std::numeric_limits<std::int64_t>::max() / 2;
    准确.类型关系 = {安全 ? 303ULL : 403ULL};
    准确.创建G = g;
    out.准确F = std::move(准确);

    原子I64特征出生读取结果 原子;
    原子.状态 = 原子I64特征出生状态::已读取;
    原子.Gread = g;
    原子.H = g;
    原子I64特征出生事实 原子事实;
    原子事实.F = f;
    原子事实.正式特征类型 = ft.编码;
    原子事实.FCv = fc.值;
    原子.事实 = 原子事实;
    out.原子读回 = std::move(原子);

    纯概念读取结果 概念;
    概念.状态 = 纯概念状态::已读取;
    概念.Gread = g;
    概念.H = g;
    纯概念事实 概念事实;
    概念事实.概念 = fc;
    概念.事实 = std::move(概念事实);
    out.单值概念读回 = std::move(概念);
    out.实际F = 特征信息身份{f};
    return out;
  }

  static 本能根材料结果 构造根成功(const 本能根材料请求 &请求,
                                      std::uint64_t g, bool 形成) {
    const std::uint64_t base = 请求.角色 == 本能根角色::安全 ? 1000 : 2000;
    本能根材料 材料;
    材料.角色 = 请求.角色;
    材料.根目标合同 = {base + 1};
    材料.根需求 = {base + 2};
    材料.根列表项 = {base + 3};
    材料.实际特征 = 请求.实际特征;
    材料.实际特征关系 = {base + 4};
    材料.目标合同关系 = {base + 5};
    材料.列表成员关系 = {base + 6};
    材料.目标值 = {base + 7};
    材料.目标I64值 = std::numeric_limits<std::int64_t>::max();
    材料.创建事实代次 = 1;
    材料.读取事实代次 = g;
    return {形成 ? 本能根材料状态::已形成 : 本能根材料状态::已恢复,
            本能根材料合同版本, g, std::move(材料)};
  }

  脚本模式 模式_;
  std::uint64_t 漂移守卫序号_ = 0;
  mutable std::uint64_t 守卫调用数_ = 0;
  mutable std::uint64_t 当前代次_ = 0;
  mutable bool 已开始_ = false;
  mutable bool 请求一致_ = true;
  std::uint64_t F调用数_ = 0;
  std::uint64_t 根调用数_ = 0;
  std::optional<本能根I64实际F请求> 安全F请求_, 服务F请求_;
  std::optional<本能根材料请求> 安全根请求_, 服务根请求_;
  std::optional<特征信息身份> 安全实际F_, 服务实际F_;
};

void 验证真实服务栈首次与新对象重复(真实服务栈 &栈) {
  auto &l1 = 栈.运行包->读取服务();
  const auto firstG = 栈.取得阶段19截止();
  const auto firstSelfRequest = 栈.构造自我读取(firstG);
  const auto firstSelf = 栈.读取自我(firstG);
  本能根运行初始化提供者 firstProvider{
      *栈.特征概念, *栈.存在, *栈.需求, firstSelfRequest, firstSelf};
  const 本能根运行初始化请求_v1 request{
      1, 存在信息身份{firstSelf.投影->E}};
  const auto formed = firstProvider.初始化(request);
  要求(formed.成功() &&
           formed.状态 == 本能根运行初始化状态_v1::已形成,
       "real-stack-first-formed");
  要求(映射自我启动材料(*formed.锚点).完整(),
       "real-stack-self-value-mapping");

  const auto beforeSecond = 当前代次(l1);
  const auto rejected = firstProvider.初始化(request);
  要求(rejected.状态 == 本能根运行初始化状态_v1::入口拒绝 &&
           !rejected.锚点 && 当前代次(l1) == beforeSecond,
       "real-stack-same-provider-second-call-rejected-zero-write");

  const auto repeatG = formed.锚点->事实截止代次;
  const auto repeatSelfRequest = 栈.构造自我读取(repeatG);
  const auto repeatSelf = 栈.读取自我(repeatG);
  本能根运行初始化提供者 repeatProvider{
      *栈.特征概念, *栈.存在, *栈.需求, repeatSelfRequest, repeatSelf};
  const auto restored = repeatProvider.初始化(request);
  要求(restored.成功() &&
           restored.状态 == 本能根运行初始化状态_v1::已恢复 &&
           restored.锚点 == formed.锚点 && 当前代次(l1) == repeatG,
       "real-stack-fresh-self-new-provider-restored-zero-write");
}

void 验证公开入口拒绝(真实服务栈 &栈) {
  auto &l1 = 栈.运行包->读取服务();
  const auto g = 栈.取得阶段19截止();
  const auto selfRequest = 栈.构造自我读取(g);
  const auto self = 栈.读取自我(g);
  const 本能根运行初始化请求_v1 valid{
      1, 存在信息身份{self.投影->E}};

  const auto check = [&](本能根运行初始化请求_v1 bad,
                         std::string_view name) {
    本能根运行初始化提供者 provider{
        *栈.特征概念, *栈.存在, *栈.需求, selfRequest, self};
    const auto before = 当前代次(l1);
    const auto rejected = provider.初始化(bad);
    const auto consumed = provider.初始化(valid);
    要求(rejected.状态 == 本能根运行初始化状态_v1::入口拒绝 &&
             !rejected.锚点 &&
             consumed.状态 == 本能根运行初始化状态_v1::入口拒绝 &&
             !consumed.锚点 && 当前代次(l1) == before,
         name);
  };

  auto bad = valid;
  bad.合同版本 = 2;
  check(bad, "public-provider-bad-version-rejected-once-zero-write");
  bad = valid;
  bad.唯一自我 = {};
  check(bad, "public-provider-zero-self-rejected-once-zero-write");
  bad = valid;
  bad.唯一自我 = 存在信息身份{{valid.唯一自我.编码.值 + 1}};
  check(bad, "public-provider-wrong-self-rejected-once-zero-write");

  auto forged = self;
  forged.状态 = 真实自我形成状态::入口拒绝;
  const auto before = 当前代次(l1);
  bool constructorRejected = false;
  try {
    本能根运行初始化提供者 invalid{
        *栈.特征概念, *栈.存在, *栈.需求, selfRequest, forged};
    (void)invalid;
  } catch (const std::invalid_argument &) {
    constructorRejected = true;
  }
  要求(constructorRejected && 当前代次(l1) == before,
       "public-provider-forged-self-read-rejected-zero-write");
}

void 验证真实持久栈(const std::filesystem::path &root, bool seed) {
  真实服务栈 stack{std::filesystem::absolute(root)};
  const auto g = stack.取得阶段19截止();
  const auto selfRequest = stack.构造自我读取(g);
  const auto self = stack.读取自我(g);
  本能根运行初始化提供者 provider{
      *stack.特征概念, *stack.存在, *stack.需求, selfRequest, self};
  const 本能根运行初始化请求_v1 request{
      1, 存在信息身份{self.投影->E}};
  const auto result = provider.初始化(request);
  要求(result.成功() &&
           result.状态 ==
               (seed ? 本能根运行初始化状态_v1::已形成
                     : 本能根运行初始化状态_v1::已恢复),
       seed ? "persistent-real-stack-seed-formed"
             : "persistent-real-stack-restart-restored");
  要求(映射自我启动材料(*result.锚点).完整(),
       seed ? "persistent-seed-self-value-mapping"
            : "persistent-recover-self-value-mapping");
  if (!seed)
    要求(当前代次(stack.运行包->读取服务()) == stack.入口事实代次,
         "persistent-recover-whole-stack-zero-write");
}

void 验证脚本成功与映射() {
  const auto 投影 = 构造脚本投影();
  const 本能根运行初始化请求_v1 请求{1, 存在信息身份{投影.E}};

  脚本端口 首次{脚本模式::首次形成};
  const auto formed = 执行组合(首次, 投影, 请求);
  要求(formed.成功() && formed.状态 == 本能根运行初始化状态_v1::已形成,
       "script-first-formed");
  要求(首次.守卫调用数() == 9, "script-nine-generation-guards");
  要求(首次.请求一致(), "script-first-requests-consistent");

  脚本端口 恢复{脚本模式::全部恢复};
  const auto restored = 执行组合(恢复, 投影, 请求);
  要求(restored.成功() &&
           restored.状态 == 本能根运行初始化状态_v1::已恢复,
       "script-restored-zero-publish");
  要求(恢复.请求一致(), "script-restored-requests-consistent");

  const auto &锚点 = *formed.锚点;
  const auto self = 映射自我启动材料(锚点);
  要求(self.完整(), "script-self-value-mapping");
}

void 验证脚本失败分支() {
  const auto 投影 = 构造脚本投影();
  const 本能根运行初始化请求_v1 请求{1, 存在信息身份{投影.E}};

  constexpr const char *漂移名称[]{
      "script-before-safety-f-reread-generation-drift",
      "script-after-safety-f-reread-generation-drift",
      "script-after-service-f-reread-generation-drift",
      "script-after-safety-root-reread-generation-drift",
      "script-after-service-root-reread-generation-drift"};
  for (std::uint64_t n = 5; n <= 9; ++n) {
    脚本端口 端口{脚本模式::首次形成, n};
    const auto result = 执行组合(端口, 投影, 请求);
    要求(result.状态 == 本能根运行初始化状态_v1::当前性漂移 &&
             !result.锚点,
         漂移名称[n - 5]);
  }

  struct 前缀用例 final {
    脚本模式 模式;
    const char *名称;
  };
  const 前缀用例 prefixCases[]{
      {脚本模式::安全F前缀后失败, "script-safety-f-prefix-converged"},
      {脚本模式::安全根前缀后失败, "script-safety-root-prefix-converged"},
      {脚本模式::服务F前缀后失败, "script-service-f-prefix-converged"}};
  for (const auto &c : prefixCases) {
    脚本端口 partial{c.模式};
    const auto failed = 执行组合(partial, 投影, 请求);
    要求(failed.状态 == 本能根运行初始化状态_v1::资源失败 &&
             !failed.锚点 && partial.请求一致(),
         "script-partial-prefix-failed-empty-anchor");
    auto freshProjection = 投影;
    freshProjection.Gread = partial.当前代次();
    freshProjection.H = freshProjection.Gread;
    脚本端口 recovery{脚本模式::全部恢复};
    const auto converged = 执行组合(recovery, freshProjection, 请求);
    要求(converged.成功() &&
             converged.状态 == 本能根运行初始化状态_v1::已恢复 &&
             recovery.请求一致(),
         c.名称);
  }

  struct 用例 final {
    脚本模式 模式;
    本能根运行初始化状态_v1 期望;
    const char *名称;
  };
  const 用例 cases[]{
      {脚本模式::实际F已可能发布,
       本能根运行初始化状态_v1::已可能发布,
       "script-actual-f-possibly-published"},
      {脚本模式::安全根已可能发布,
       本能根运行初始化状态_v1::已可能发布,
       "script-safety-root-possibly-published"},
      {脚本模式::服务F已可能发布,
       本能根运行初始化状态_v1::已可能发布,
       "script-service-f-possibly-published"},
      {脚本模式::服务根已可能发布,
       本能根运行初始化状态_v1::已可能发布,
       "script-service-root-possibly-published"},
      {脚本模式::根幂等冲突, 本能根运行初始化状态_v1::幂等冲突,
       "script-root-idempotency-conflict"},
      {脚本模式::根材料退出, 本能根运行初始化状态_v1::根材料已退出,
       "script-root-material-retired"},
      {脚本模式::实际F引用冲突, 本能根运行初始化状态_v1::引用冲突,
        "script-actual-f-reference-conflict"},
      {脚本模式::实际F当前采用读取失败,
       本能根运行初始化状态_v1::内部不一致,
       "script-current-adoption-read-failure-internal"},
      {脚本模式::实际F缺嵌套载荷,
       本能根运行初始化状态_v1::内部不一致,
       "script-missing-nested-payload-internal"},
      {脚本模式::实际F材料未闭合,
       本能根运行初始化状态_v1::材料未闭合,
       "script-actual-f-material-incomplete"},
      {脚本模式::实际F资源失败,
       本能根运行初始化状态_v1::资源失败,
       "script-actual-f-resource-failure"}};
  for (const auto &c : cases) {
    脚本端口 端口{c.模式};
    const auto result = 执行组合(端口, 投影, 请求);
    要求(result.状态 == c.期望 && !result.锚点 && 端口.请求一致(),
         c.名称);
  }

  const 脚本模式 unknownModes[]{脚本模式::实际F已可能发布,
                                 脚本模式::安全根已可能发布,
                                 脚本模式::服务F已可能发布,
                                 脚本模式::服务根已可能发布};
  for (std::size_t i = 0; i < std::size(unknownModes); ++i) {
    脚本端口 unknownPort{unknownModes[i]};
    const auto unknown = 执行组合(unknownPort, 投影, 请求);
    要求(unknown.状态 == 本能根运行初始化状态_v1::已可能发布 &&
             !unknown.锚点 && unknownPort.请求一致(),
         "script-possibly-published-empty-anchor");
    auto freshProjection = 投影;
    freshProjection.Gread = 101 + i;
    freshProjection.H = freshProjection.Gread;
    脚本端口 freshPort{脚本模式::全部恢复};
    const auto converged = 执行组合(freshPort, freshProjection, 请求);
    要求(converged.成功() &&
             converged.状态 == 本能根运行初始化状态_v1::已恢复 &&
             converged.锚点->事实截止代次 == freshProjection.Gread &&
             freshPort.请求一致(),
         "script-possibly-published-fresh-projection-converged");
  }

  脚本端口 端口;
  auto bad = 请求;
  bad.合同版本 = 2;
  要求(执行组合(端口, 投影, bad).状态 ==
             本能根运行初始化状态_v1::入口拒绝,
       "script-bad-version-rejected");
  bad = 请求;
  bad.唯一自我 = {{999}};
  要求(执行组合(端口, 投影, bad).状态 ==
             本能根运行初始化状态_v1::入口拒绝,
       "script-wrong-self-rejected");
}

} // namespace

int main(int argc, char **argv) {
  try {
    if (argc == 3) {
      const std::string_view mode{argv[1]};
      if (mode == "seed") {
        验证真实持久栈(std::filesystem::path{argv[2]}, true);
      } else if (mode == "recover") {
        验证真实持久栈(std::filesystem::path{argv[2]}, false);
      } else {
        throw std::runtime_error("unknown probe mode");
      }
      std::cout << "PASS total=" << 通过数 << '\n';
      return 0;
    }
    if (argc != 1)
      throw std::runtime_error("invalid probe arguments");
    真实服务栈 栈;
    验证真实服务栈首次与新对象重复(栈);
    验证公开入口拒绝(栈);
    验证脚本成功与映射();
    验证脚本失败分支();
    std::cout << "PASS total=" << 通过数 << '\n';
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "unknown validation failure\n";
    return 2;
  }
}
